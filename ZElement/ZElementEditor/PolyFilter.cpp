// Filename	: PolyFilter.cpp
// Creator	: 
// Date		: 2009/06/29
// Date     : 2009/07/10

#include "stdafx.h"
#include "PolyFilter.h"
#include <algorithm>
#include <fstream>
#include <cmath>
#include <shellapi.h>

namespace PolyFilter
{
	// Maximum R/G/B value for clamp during calculating
	RGBType g_maxRGBValue = 255;

	// RGBType clamp during calculating
	static RGBType Clamp(float value)
	{
		if (value < 0)
			value = 0;
		if (value > g_maxRGBValue)
			value = g_maxRGBValue;
		return (RGBType)value;
	}

	// Class for accessing color image
	//
	class Image
	{
	public:
		Image(RGBType *data, int width, int height);
		~Image();
		
		bool Empty()const;
		int  GetWidth()const;
		int  GetHeight()const;
		const RGBType *GetPixel(int i, int j)const;
		void Filter(int i, int j, float alpha);
		void Filter(int i, int j, float alpha, const RGBType *blendColor, float blendAlpha);

	private:
		bool IsValid(int i, int j)const;
		RGBType * GetPixel(int i, int j);
		const RGBType * GetPixelImpl(int i, int j)const;
		
	private:
		RGBType *_data;
		int _width;
		int _height;
	};
	
	// Class for generating filter area from polygons
	//
	class BitImage
	{
	public:
		BitImage(int width, int height);
		~BitImage();
		
		void AddPolygonArea(const Polygon &polygon, int mark=1);
		void ExpandOnePixel(int mark);
		
		void Reset(int mark=0);
		
		int  GetWidth()const;
		int  GetHeight()const;
		int  GetPixel(int x, int y)const;
		bool GetNearestPixel(int x, int y, int refMark, int testArea, float &dist)const;
		
		bool DebugOutput(const char *fileName)const;
		
	private:
		bool IsWithinImage(int x, int y)const;
		bool IsNotWithinImageSimple(const int p0[2], const int p1[2])const;
		bool FindNearestClipPoint(const int p0[2], const int p1[2], int a[2]);
		bool ClipLine(int p0[2], int p1[2]);
		void DrawLine(const int p0[2], const int p1[2], int mark);
		void DrawPixel(int x, int y, int mark);
		
	private:
		int *_bits;
		int _width;
		int _height;
	};
	
	// Class for filtering image with Image and BitImage
	//
	class Filter
	{		
	public:
		Filter();
		virtual ~Filter();
		
		void SetImage(Image *image);
		void SetPolygon(const PolygonArray &polygons);
		void SetExpandWidth(int expandWidth);
		void SetFilterRatio(float alpha);

		void SetBlendColor(const RGBType *blendColor);
		void SetBlendAlpha(float blendAlpha);

		void Execute(void);
		
	private:
		void FilterInnerPolygon(const BitImage &bit, int mark);
		void FilterOuterPolygon(const BitImage &bit, int polygonMark=1);
		
	private:
		// Image data
		Image *_image;
		
		// Filter parameter
		PolygonArray _polygons;
		int _expandWidth;
		float _filterRatio;

		// Filter image data and ratio
		RGBType _blendColor[3];
		float _blendAlpha;
	};

	// type definition used in Scan Line algorithm
	typedef float Float;
	
	// Polygon Edge in Scan Line algorithm
	//
	struct Edge
	{
		int ymax, ymin;
		Float x;
		Float deltax;
		Edge * nextEdge;
		
		bool operator < (const Edge &rhs)const
		{
			return (this->x < rhs.x)
				|| (this->x == rhs.x)
				&& (this->deltax < rhs.deltax);
		}
	};
	
	// Edge Table in Scan Line algorithm
	//
	struct EdgeTable
	{
		typedef Edge* Line;
		
		EdgeTable(Edge * edges, int edgeCount);
		
		Line & operator[](int y)
		{
			return _lines[y + _offset];
		}
		
		const Line & operator[](int y)const
		{
			return _lines[y + _offset];
		}
		
		int From()const
		{
			return _ymin;
		}
		
		int To()const
		{
			return _ymax;
		}
		
		bool Empty()const
		{
			for (int i = 0; i < _size; ++ i)
			{
				if (_lines[i])
					return false;
			}
			return true;
		}
		
		~EdgeTable()
		{
			delete []_lines;
			_lines = NULL;
		}
		
		Line * _lines;
		int _size;
		int _offset;
		int _ymin;
		int _ymax;
	};
	
	// struct ActiveEdgeList in Scan Line algorithm
	//
	struct ActiveEdgeList
	{
		ActiveEdgeList()
			: _head(NULL)
		{}
		
		bool Empty()const
		{
			return _head == NULL;
		}
		
		void Add(Edge * rhs);
		void Update(int newY);
		
		Edge * _head;
	};
	
	// Auxiliary function in Scan Line algorithm
	int FetchLeftPixel(const Edge *e);	
	int FetchRightPixel(const Edge *e);

	// Function:  Execute(RGBType *, int, int, const PolygonArray &, float, int, const RGBType *, float)
	// Parameter: [in, out] rgb, row-first image color array, each three element corresponding to a pixel's rgb color
	//            [in] width, image pixel width for parameter rgb
	//            [in] height, image pixel height for parameter rgb
	//            [in] polygons, polygon array within and around which color is to be filtered
	//            [in] alpha, ratio for filtering pixels within polygon array, must be within [0, 1]
	//            [in] expandWidth, in pixels, indicating area around each polygon which is to be filtered
	//            [in] blendColor, color to blend with parameter rgb within and around polygons
	//            [in] blendAlpha, ratio of parameter blendColor to blend with parameter rgb within polygons, must be within [0, 1]
	//
	void Execute(RGBType *rgb, int width, int height
		, const PolygonArray &polygons
		, float alpha
		, int expandWidth
		, const RGBType *blendColor/* =0 */
		, float blendAlpha/* =0 */)
	{
		if (!rgb	    ||
			width <= 0  ||
			height <= 0 ||
			polygons.size() < 1 ||
			alpha < 0   ||
			alpha > 1   ||
			expandWidth < 0)
			return;

		if (blendColor &&
			(blendAlpha < 0 || blendAlpha > 1))
			return;
		
		Filter filter;
		
		Image image(rgb, width, height);
		filter.SetImage(&image);
		
		filter.SetPolygon(polygons);
		filter.SetFilterRatio(alpha);
		filter.SetExpandWidth(expandWidth);

		filter.SetBlendColor(blendColor);
		filter.SetBlendAlpha(blendAlpha);
		
		filter.Execute();
	}

	// Class Image methods
	//

	Image::Image(RGBType *data, int width, int height)
		: _data(data)
		, _width(width)
		, _height(height)
	{}

	Image::~Image()
	{}

	bool Image::Empty()const
	{
		return !_data
			|| _width <= 0
			|| _height <= 0;
	}

	int Image::GetWidth()const
	{
		return _width;
	}

	int Image::GetHeight()const
	{
		return _height;
	}
	
	bool Image::IsValid(int i, int j)const
	{
		bool valid(true);
		if (i < 0 || i >= GetWidth() ||
			j < 0 || j >= GetHeight())
			valid = false;
		return valid;
	}

	const RGBType * Image::GetPixelImpl(int i, int j)const
	{
		RGBType * pixel = NULL;
		if (IsValid(i, j))
		{
			int offset = j * GetWidth() + i;
			offset *= 3;
			pixel = _data + offset;
		}
		return pixel;
	}

	const RGBType * Image::GetPixel(int i, int j)const
	{
		return GetPixelImpl(i, j);
	}

	RGBType * Image::GetPixel(int i, int j)
	{
		return const_cast<RGBType *>(GetPixelImpl(i, j));
	}

	void Image::Filter(int i, int j, float alpha)
	{
		RGBType * pColor = GetPixel(i, j);
		if (!pColor)
			return;

		pColor[0] = Clamp(pColor[0]*alpha + 0.5f);
		pColor[1] = Clamp(pColor[1]*alpha + 0.5f);
		pColor[2] = Clamp(pColor[2]*alpha + 0.5f);
	}

	void Image::Filter(int i, int j, float alpha, const RGBType *blendColor, float blendAlpha)
	{
		RGBType *pixel = GetPixel(i, j);
		if (!pixel)
			return;

		if (blendColor)
		{
			pixel[0] = Clamp(pixel[0]*alpha + blendColor[0]*blendAlpha + 0.5f);
			pixel[1] = Clamp(pixel[1]*alpha + blendColor[1]*blendAlpha + 0.5f);
			pixel[2] = Clamp(pixel[2]*alpha + blendColor[2]*blendAlpha + 0.5f);
		}
		else
		{
			pixel[0] = Clamp(pixel[0]*alpha + 0.5f);
			pixel[1] = Clamp(pixel[1]*alpha + 0.5f);
			pixel[2] = Clamp(pixel[2]*alpha + 0.5f);
		}
	}

	// Class BitImage methods
	//

	BitImage::BitImage(int width, int height)
		: _width(width)
		, _height(height)
		, _bits(NULL)
	{
		if (GetWidth() > 0 && GetHeight() > 0)
		{
			int pixelCount = GetWidth() * GetHeight();
			_bits = new int [pixelCount];
			memset(_bits, 0, sizeof(int) * pixelCount);
		}
	}

	BitImage::~BitImage()
	{
		delete []_bits;
	}

	void BitImage::Reset(int mark)
	{
		if (mark == 0)
		{
			int pixelCount = GetWidth() * GetHeight();
			memset(_bits, 0, sizeof(int) * pixelCount);
			return;
		}

		for (int h = 0; h < GetHeight(); ++ h)
		{
			int lineOffset = h * GetWidth();
			for (int w = 0; w < GetWidth(); ++ w)
				_bits[lineOffset + w] = mark;
		}
	}

	int BitImage::GetWidth()const
	{
		return _width;
	}

	int BitImage::GetHeight()const
	{
		return _height;
	}

	int BitImage::GetPixel(int x, int y)const
	{
		if (!IsWithinImage(x, y))
			return -1;
		return _bits[y * GetWidth() + x];
	}

	bool BitImage::GetNearestPixel(int x, int y, int refMark, int testArea, float &dist)const
	{
		// Find nearest pixel with mark refMark to given pos with given area
		// Return distance between the two
		//

		// Calculate test range on x axis
		int xFrom = x-testArea;
		xFrom = max(xFrom, 0);
		int xTo = x+testArea;
		xTo = min(xTo, GetWidth()-1);

		// Calculate test range on y axis
		int yFrom = y-testArea;
		yFrom = max(yFrom, 0);
		int yTo = y+testArea;
		yTo = min(yTo, GetHeight()-1);

		// Find the nearest pixel
		bool found(false);
		int minSquareDist = testArea*testArea;
		int temp(0);
		for (int i = xFrom; i < xTo; ++ i)
		{
			for (int j = yFrom; j < yTo; ++ j)
			{
				if (GetPixel(i, j) == refMark)
				{
					temp = (x-i)*(x-i) + (y-j)*(y-j);
					if (temp <= minSquareDist)
					{
						found = true;
						minSquareDist = temp;
					}
				}
			}
		}

		// Calculate the distance between the two
		if (found)
			dist = sqrt((double)minSquareDist);

		return found;
	}

	void BitImage::AddPolygonArea(const Polygon &polygon, int mark/* =1 */)
	{
		// Convert polygon to pixels within current image
		// The parameter mark is used to mark the polygon
		//

		// Validate polygon
		size_t vertexCount = polygon.size();
		if (vertexCount < 3)
			return;

		// Raster given polygon with the Scan Line algorithm

		// Build all edges
		abase::vector<Edge> edges;
		edges.reserve(vertexCount);
		int i(0), j(0);
		int si(0), sj(0);
		for (i = 0, j = 1; i < vertexCount; ++ i, j = (i+1)%vertexCount)
		{
			// Ignore horizontal edge
			if (polygon[i].y == polygon[j].y)
				continue;

			// Add one edge
			edges.push_back(Edge());
			Edge & dst = edges.back();
			
			if (polygon[i].y < polygon[j].y)
			{
				si = i;
				sj = j;
			}
			else
			{
				si = j;
				sj = i;
			}
			
			dst.ymin = polygon[si].y;
			dst.ymax = polygon[sj].y;
			dst.x = polygon[si].x;
			dst.deltax = (polygon[sj].x - polygon[si].x) / (Float)(polygon[sj].y - polygon[si].y);
			dst.nextEdge = NULL;
		}
		
		if (edges.size() <= 0)
			return;

		// Build Edge Table
		EdgeTable edgeTable(&edges[0], (int)edges.size());

		// Begin to raster
		ActiveEdgeList activeEdgeList;
		int y = edgeTable.From();
		
		Edge * p = NULL;
		Edge * q = NULL;
		int p0[2], p1[2];
		while (!edgeTable.Empty()
			|| !activeEdgeList.Empty())
		{
			// Fetch from current edge table line
			EdgeTable::Line & line = edgeTable[y];
			
			// Add to active edge list
			activeEdgeList.Add(line);
			
			// Remove from edge table
			line = NULL;
			
			// Fill polygon
			p = activeEdgeList._head;
			while (p)
			{
				q = p->nextEdge;
				
				// Draw line [p, q]
				p0[0] = FetchLeftPixel(p);
				p0[1] = y;
				p1[0] = FetchRightPixel(q);
				p1[1] = y;
				
				if (p0[0] <= p1[0])
					DrawLine(p0, p1, mark);
				
				p = q->nextEdge;
			}
			
			// Move to next line
			++ y;
			activeEdgeList.Update(y);
		}
	}

	void BitImage::DrawLine(const int p0[2], const int p1[2], int mark)
	{
		// Convert line area to pixels within current image
		// The parameter is used to mark the area
		//

		// Make point copy
		int xy0[2] = {p0[0], p0[1]};
		int xy1[2] = {p1[0], p1[1]};
		
		// Clip within image
		if (!ClipLine(xy0, xy1))
			return;
		
		// Take care when line length == 0
		int dx = xy1[0] - xy0[0];
		int dy = xy1[1] - xy0[1];
		if (dx == 0 &&
			dy == 0)
		{
			DrawPixel(xy0[0], xy0[1], mark);
			return;
		}
		
		// Check m
		bool drawNormal = true;
		if (dx == 0	||
			fabs(dy/(Float)dx) > 1)
		{
			drawNormal = false;
		}
		
		// draw using DDA
		if (drawNormal)
		{
			// assure x0 < x1
			if (dx < 0)
			{
				std::swap(xy0[0], xy1[0]);
				std::swap(xy0[1], xy1[1]);
				dx = -dx;
				dy = -dy;
			}
			
			Float y = static_cast<Float>(xy0[1]);
			Float m = dy/(Float)dx;
			
			for (int x = xy0[0]; x <= xy1[0]; ++ x)
			{
				DrawPixel(x, (int)(y+0.5f), mark);				
				y += m;
			}
		}
		else
		{
			// assure y0 < y1
			if (dy < 0)
			{
				std::swap(xy0[0], xy1[0]);
				std::swap(xy0[1], xy1[1]);
				dx = -dx;
				dy = -dy;
			}
			
			Float x = static_cast<Float>(xy0[0]);
			Float m = dx/(Float)dy;
			
			for (int y = xy0[1]; y <= xy1[1]; ++ y)
			{
				DrawPixel((int)(x+0.5f), y, mark);				
				x += m;
			}
		}
	}

	void BitImage::DrawPixel(int x, int y, int mark)
	{
		// Mark given pixel as polygon area
		//
		if (IsWithinImage(x, y))
		{
			int offset = y * GetWidth() + x;
			_bits[offset] = mark;
		}
		else
		{
			assert(false);
		}
	}

	bool BitImage::ClipLine(int p0[2], int p1[2])
	{
		// Return true if still visible after clip
		//		
		int a[2];
		if (!FindNearestClipPoint(p0, p1, a))
			return false;
		
		int b[2];
		if (!FindNearestClipPoint(p1, p0, b))
			return false;
		
		p0[0] = a[0];
		p0[1] = a[1];
		
		p1[0] = b[0];
		p1[1] = b[1];
		
		return true;
	}

	bool BitImage::IsWithinImage(int x, int y)const
	{
		return (x >= 0
			&& x < GetWidth()
			&& y >= 0
			&& y < GetHeight());
	}
	
	bool BitImage::IsNotWithinImageSimple(const int p0[2], const int p1[2])const
	{
		// Check if line segment is apparently invisible
		//
		
		int a;
		
		a = max(p0[0], p1[0]);
		if (a < 0)
			return true;
		
		a = min(p0[0], p1[0]);
		if (a >= GetWidth())
			return true;
		
		a = max(p0[1], p1[1]);
		if (a < 0)
			return true;
		
		a = min(p0[1], p1[1]);
		if (a >= GetHeight())
			return true;
		
		return false;
	}

	bool BitImage::FindNearestClipPoint(const int p0[2], const int p1[2], int a[2])
	{
		// Find nearest point near p0 on clip boundary
		// Return false if not found
		// Or else set the result in a[2]
		
		if (IsWithinImage(p0[0], p0[1]))
		{
			a[0] = p0[0];
			a[1] = p0[1];
			return true;
		}
		
		// using mid-point method
		
		if (IsNotWithinImageSimple(p0, p1))
			return false;
		
		int p[2] = {p0[0], p0[1]};
		int q[2] = {p1[0], p1[1]};
		int m[2];
		
		while (true)
		{
			// get segment mid point
			m[0] = (p[0] + q[0])/2;
			m[1] = (p[1] + q[1])/2;
			
			// test critical condition
			if (m[0] == p[0] &&
				m[1] == p[1] ||
				m[0] == q[0] &&
				m[1] == q[1])
			{
				if (IsWithinImage(p[0], p[1]))
				{
					a[0] = p[0];
					a[1] = p[1];
					return true;
				}
				if (IsWithinImage(q[0], q[1]))
				{
					a[0] = q[0];
					a[1] = q[1];
					return true;
				}
				return false;
			}
			
			if (IsWithinImage(m[0], m[1]))
			{
				// must on [p, m]
				q[0] = m[0];
				q[1] = m[1];
				continue;
			}
			
			// try find on [p, m]
			if (FindNearestClipPoint(p, m, a))
				return true;
			
			// try find on [m, q]
			if (FindNearestClipPoint(m, q, a))
				return true;
			
			// cannot find 
			return false;
		}
	}

	void BitImage::ExpandOnePixel(int mark)
	{
		// Expand current polygon area by one pixel around
		// The parameter mark is used to mark the new expanded area
		//
		int width = GetWidth();
		int height = GetHeight();
		for (int j = 0; j < height; ++ j)
		{
			for (int i = 0; i < width; ++ i)
			{
				if (GetPixel(i, j) != 0)
					continue;

				bool valid = false;
				
				for (int y = -1; y <= 1; ++ y)
				{
					for (int x = -1; x <= 1; ++ x)
					{
						int cur = GetPixel(i+x, j+y);
						if (cur > 0 && cur != mark)
						{
							valid = true;
							break;
						}
					}
				}

				if (valid)
					DrawPixel(i, j, mark);
			}
		}
	}

	bool BitImage::DebugOutput(const char *fileName)const
	{
		// Output current polygon area to txt file for debug
		//
		if (!fileName)
			return false;

		std::ofstream file(fileName, std::ios_base::trunc);
		if (!file.is_open())
			return false;

		bool result(false);
		while (true)
		{
			for (int j = GetHeight()-1; j >= 0; -- j)
			{
				for (int i = 0; i < GetWidth(); ++ i)
					file << GetPixel(i, j);

				file << std::endl;
			}

			result = true;
			break;
		}

		file.close();
		return result;
	}

	// Class Filter methods
	//

	Filter::Filter()
		: _image(NULL)
		, _expandWidth(0)
		, _filterRatio(1.0f)
		, _blendAlpha(0.0f)
	{
		SetBlendColor(NULL);
	}

	Filter::~Filter()
	{}

	void Filter::SetImage(Image *image)
	{
		_image = image;
	}

	void Filter::SetPolygon(const PolygonArray &polygons)
	{
		_polygons = polygons;
	}

	void Filter::SetExpandWidth(int expandWidth)
	{
		if (expandWidth >= 0)
			_expandWidth = expandWidth;
	}

	void Filter::SetFilterRatio(float alpha)
	{
		if (alpha >= 0 &&
			alpha <= 1)
			_filterRatio = alpha;
	}

	void Filter::SetBlendColor(const RGBType *blendColor)
	{
		if (blendColor)
		{
			_blendColor[0] = blendColor[0];
			_blendColor[1] = blendColor[1];
			_blendColor[2] = blendColor[2];
		}
		else
		{
			_blendColor[0] = 0;
			_blendColor[1] = 0;
			_blendColor[2] = 0;
		}
	}

	void Filter::SetBlendAlpha(float blendAlpha)
	{
		if (blendAlpha >= 0 &&
			blendAlpha <= 1)
			_blendAlpha = blendAlpha;
	}

	// Implement the filter action with all parameters
	//
	void Filter::Execute()
	{
		if (!_image || _image->Empty())
			return;

		BitImage bit(_image->GetWidth(), _image->GetHeight());

		for (int i = 0; i < _polygons.size(); ++ i)
		{
			// For each polygon
			const Polygon & polygon = _polygons[i];

			int mark = 1;

			// Filter inner polygon area with given ratio
			bit.Reset();
			bit.AddPolygonArea(polygon, mark);
			FilterInnerPolygon(bit, mark);

			// Filter expanded area with smooth ratio
			if (_expandWidth > 0)
			{
				// Construct expanded area
				for (int j = 0; j < _expandWidth; ++ j)
				{
					++ mark;
					bit.ExpandOnePixel(mark);
				}

				// Filter smoothly
				FilterOuterPolygon(bit);
			}
		}
	}

	// Filter Method 1: Filter image pixels with given mark
	//
	void Filter::FilterInnerPolygon(const BitImage &bit, int mark)
	{
		for (int x = 0; x < _image->GetWidth(); ++ x)
			for (int y = 0; y < _image->GetHeight(); ++ y)
				if (bit.GetPixel(x, y) == mark)
						_image->Filter(x, y, _filterRatio, _blendColor, _blendAlpha);
	}

	// Filter Method 2: Filter marked image pixels within given distance to given polygon
	//
	void Filter::FilterOuterPolygon(const BitImage &bit, int polygonMark/* =1 */)
	{
		float dist(0);
		float dist2(0);
		float ratio(0);
		float ratio2(0);
		for (int x = 0; x < _image->GetWidth(); ++ x)
		{
			for (int y = 0; y < _image->GetHeight(); ++ y)
			{
				if (bit.GetPixel(x, y) > polygonMark)
				{
					if (bit.GetNearestPixel(x, y, polygonMark, _expandWidth, dist))
					{
						dist2 = dist/_expandWidth;
						ratio = _filterRatio + dist2*(1-_filterRatio);
						if (ratio > 1) ratio = 1;
						ratio2 = _blendAlpha*(1-dist2);
						if (ratio2 < 0) ratio2 = 0;
						_image->Filter(x, y, ratio, _blendColor, ratio2);
					}
				}
			}
		}
	}

	// struct EdgeTable methods
	//
	EdgeTable::EdgeTable(Edge * edges, int edgeCount)
	{
		int yMin = edges[0].ymin;
		int yMax = edges[0].ymax;
		int i(0);
		
		for (i = 1; i < edgeCount; ++ i)
		{
			Edge & e = edges[i];
			if (e.ymin < yMin)
				yMin = e.ymin;
			if (e.ymax > yMax)
				yMax = e.ymax;
		}
		
		_offset = -yMin;
		_size = (yMax - yMin + 1);
		_lines = new Line [_size];
		_ymin = yMin;
		_ymax = yMax;
		::memset(_lines, 0, sizeof(Line) * _size);
		
		for (i = 0; i < edgeCount; ++ i)
		{
			Edge * e = &edges[i];
			Line & line = (*this)[e->ymin];
			if (line == NULL)
			{
				line = e;
				e->nextEdge = NULL;
			}
			else
			{
				Edge * p = line;
				
				while (p->nextEdge)
				{
					if (*(p->nextEdge) < *e)
					{
						p = p->nextEdge;
						continue;
					}
					break;
				}
				
				if (*p < *e)
				{
					e->nextEdge = p->nextEdge;
					p->nextEdge = e;
				}
				else
				{
					e->nextEdge = p;
					line = e;
				}
			}
		}
	}
	
	// struct ActiveEdgeList methods
	//
	void ActiveEdgeList::Add(Edge * rhs)
	{
		if (rhs == NULL)
			return;
		
		if (_head == NULL)
		{
			_head = rhs;
			rhs = rhs->nextEdge;
			_head->nextEdge = NULL;
		}
		
		while (rhs)
		{
			Edge * p = _head;
			
			while (p->nextEdge)
			{
				if (*(p->nextEdge) < *rhs)
				{
					p = p->nextEdge;
					continue;
				}
				break;
			}
			
			Edge * next = rhs->nextEdge;
			if (*p < *rhs)
			{
				rhs->nextEdge = p->nextEdge;
				p->nextEdge = rhs;
			}
			else
			{
				rhs->nextEdge = p;
				_head = rhs;
			}
			rhs = next;
		}
	}
	
	void ActiveEdgeList::Update(int newY)
	{
		Edge * newHead = NULL;
		Edge * newHeadTail = NULL;
		
		Edge * p = _head;
		while (p)
		{
			if (p->ymax == newY)
			{
				p = p->nextEdge;
				continue;
			}
			
			if (!newHead)
			{
				newHead = newHeadTail = p;
			}
			else
			{
				newHeadTail->nextEdge = p;
				newHeadTail = p;
			}
			
			p = p->nextEdge;
			newHeadTail->nextEdge = NULL;
			newHeadTail->x += newHeadTail->deltax;
		}
		
		_head = newHead;
	}

	// Other auxiliary function for Scan Line algorithm
	//
	int FetchLeftPixel(const Edge *e)
	{
		int ret = (int)e->x;
		
		if (fabs(ret-e->x) > 1e-6f)
			ret += 1;
		
		return ret;
	}
	
	int FetchRightPixel(const Edge *e)
	{
		int ret = (int)e->x;
		
		if (fabs(ret-e->x) < 1e-6f)
			ret -= 1;
		
		return ret;
	}
}