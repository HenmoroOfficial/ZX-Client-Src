#ifndef _A3DGRASSBLOCK_H_
#define _A3DGRASSBLOCK_H_

#include "A3DGeometry.h"
#include "ARect.h"
#include "abasedef.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassBlock
//		b block of grasses in one rectangle on the terrain
//	
///////////////////////////////////////////////////////////////////////////

struct GRASSVERTEX;
struct GRASSCLUSTER;
struct A3DGRASSINSTANCEDATA;

class ARandomGen;
class A3DStream;
class A3DGrassLand;
class A3DGrassBitsMap;
class A3DGrassType;
class A3DGrassInteractor;

class A3DGrassBlock
{
public:

private:
	A3DGrassLand*					m_pGrassLand;	// the grass land instance
	A3DGrassBitsMap*				m_pGrassBitsMap;// the grass's bits map which this block resides on
	
	int								m_nNumGrasses;	// number of grasses

	A3DVECTOR3						m_vecCenter;	// center of this block
	bool							m_bBuilt;		// flag indicates whether this block's grasses has been built
	bool							m_bAboutToBuild;// flag indicates this block needs build
	bool							m_bAboutToDestroy; // flag indicates this block need to be destroyed
	GRASSVERTEX *					m_pGrassesVerts;// verts in this block
	GRASSCLUSTER*					m_pClusters;	// 每簇草的信息

public:
	inline const ARectI& GetBlockRect()	const	{ return m_rectBlock; }
	inline const A3DAABB& GetAABB()	const		{ return m_aabb; }
	inline const A3DVECTOR3& GetCenter() const	{ return m_vecCenter; }

	inline bool IsBuilt() const				{ return m_bBuilt; }
	inline bool IsAboutToBuild() const		{ return m_bAboutToBuild; }
	inline bool IsAboutToDestroy() const	{ return m_bAboutToDestroy; }
	inline void SetAboutToBuild()			{ m_bAboutToBuild = true; }
	inline void SetAboutToDestroy()			{ m_bAboutToDestroy = true; }
	inline void ClearAboutToBuild()			{ m_bAboutToBuild = false; }
	inline void ClearAboutToDestroy()		{ m_bAboutToDestroy = false; }

	inline int GetNumGrasses() const		{ return m_nNumGrasses; }
	inline GRASSVERTEX * GetGrassesVerts() const { return m_pGrassesVerts; }
	inline GRASSCLUSTER* GetClusters() const { return m_pClusters; }
	//	Get vertex or instance data stream
	virtual A3DStream* GetInstStream() const { return NULL; }
	//	Get render grass num
	int GetRenderGrassNum() const { return m_nRenderGrassNum; }
public:
	A3DGrassBlock();
	virtual ~A3DGrassBlock();

	// Function Init(A3DGrassBitsMap * pBitsMap, const ARectI& rectBlock)
	//		Initialize the block, but not build
	// IN:
	//	pBitsMap		bits map of the terrain which this block blongs to
	//	rectBlock		the range of this block in the bits map
	//
	bool Init(A3DGrassBitsMap * pBitsMap, const ARectI& rectBlock);
	bool Release();

	// Function BuildGrasses()
	//		Build grasses from the bits map
	//
	bool BuildGrasses();

	// Function DestroyGrasses()
	//		release all grasses that have been built, and set m_bBuilt to false
	//
	bool DestroyGrasses();

	// Function UpdateGrassOffset()
	//		check each grass with the interactor and decide the status of the grass
	//
	bool UpdateGrassOffset(float fDeltaTime);
protected:

	A3DAABB							m_aabb;			// bounding box for visible determination
	ARectI							m_rectBlock;	// the range of this block
	int								m_nRenderGrassNum;	// number of grasses ready to be rendered
	//	Update AABB y
	inline void UpdateAABBY(float y)
	{
		if (y < m_aabb.Mins.y)
			m_aabb.Mins.y = y;

		if (y > m_aabb.Maxs.y)
			m_aabb.Maxs.y = y;
	}
	inline A3DGrassBitsMap*	GetGrassBitsMap() const { return m_pGrassBitsMap; }

private:

	virtual bool ImplBuildGrasses(ARandomGen& GrassScatter, int s);
	virtual bool ImplDestroyGrasses();
	//	Update grass offset
	virtual bool ImplUpdateGrassOffset(A3DGrassInteractor* pInteractor, float fDeltaTime)  { return true; }
};

//	Grass block used in the instance rendering
class A3DGrassBlockHW : public A3DGrassBlock
{
	typedef A3DGrassBlock base_class;

	enum GRASS_PRESS_STATE
	{
		Free = 0,			//	free standing
		Pressed,			//	pressed
		Shaking,			//	shaking
	};

	struct GRASS_INSTS
	{
		GRASS_INSTS()
			: m_pGrassInstTrans(NULL)
			, m_pGrassMoveStatus(NULL)
			, m_pSpeed(NULL)
		{

		}

		~GRASS_INSTS()
		{
			Clear();
		}

		void SetGrassRenderNum(int iGrassRenderNum);
		void Clear();

		A3DGRASSINSTANCEDATA*	GetInstData() const { return m_pGrassInstTrans; }
		a_byte*					GetMoveStatus() const { return m_pGrassMoveStatus; }
		A3DVECTOR3*				GetSpeed() const { return m_pSpeed; }

	private:
		A3DGRASSINSTANCEDATA*	m_pGrassInstTrans;		// grass instance transform information
		a_byte*					m_pGrassMoveStatus;		// grass instance move status
		A3DVECTOR3*				m_pSpeed;				// grass move speed
	};

public:		//	Constructors and Destructor

	explicit A3DGrassBlockHW();
	~A3DGrassBlockHW();

private:

	//	Generate a mask array for filling the grass verts buffer
	//	ASSUME nGrassMaskSize > nNumLimit
	//	we choose nNumLimit number of grasses from pGrassMask randomly leave them as false
	//	and set the rest to be true
	bool GenGrassMaskArray(bool* pGrassMask, int nGrassMaskSize, int nNumLimit);
	//	Get vertex or instance data stream
	virtual A3DStream* GetInstStream() const { return m_pInstStream; }
	//	Build grass resource in HWInstance mode
	virtual bool ImplBuildGrasses(ARandomGen& GrassScatter, int s);
	//	Destroy grass resource in HWInstance mode
	virtual bool ImplDestroyGrasses();
	//	Update grass offset
	virtual bool ImplUpdateGrassOffset(A3DGrassInteractor* pInteractor, float fDeltaTime);

	GRASS_INSTS			m_aInstData;
	A3DStream*			m_pInstStream;		// transform stream used in HWInstancing
};

void AfxSetGrassPressFactor(float fRadius, float fAmount);
void AfxSetGrassShakeFactor(float fSpeed, float fDamping);

#endif