//////////////////////////////////////////////////////////////////////
//	Created by Niuyadong
//  2010-01-26
//  
//  
//////////////////////////////////////////////////////////////////////

#ifndef	_CONVEXHULLDATASET_H_
#define _CONVEXHULLDATASET_H_

#include <A3DVector.h>
#include <A3DGeometry.h>
#include <AArray.h>
#include "Face.h"
#include <vector.h>
#include <AFile.h>
#include <A3DFlatCollector.h>
#include "ConvexBrush.h"


#define CHF_FILE_MAGIC (unsigned int) (('c'<<24) | ('h' << 16) | ('f' << 8) | ('p'))
//-----------------------------------------------------------------------------------
//v1 按照索引导出凸包数据								#define CHF_FILE_VERSION (unsigned int) (0x00000001)
//v2 按照model名称导出凸包数据，主要针对UMDs			#define CHF_FILE_VERSION (unsigned int) (0x00000002)
//v3     .chb（current version）                        #define CHF_FILE_VERSION (unsigned int) (0x00000003)                   
//-----------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Version: 0x00000001
// 开始导出数据
// 导出数据格式：二进制格式
//-------------------Head info-----------------------------
// 1 int 文件类型
// 2 int 版本
// 3 int 模型数量m
// ------------------模型1的数据---------------------------
// 凸包数量n
// 凸包1的数据
// 凸包2的数据
//	...		...
// 凸包n的数据
// ------------------模型2的数据---------------------------
// 凸包数量n
// 凸包1的数据
// 凸包2的数据
//	...		...
// 凸包n的数据
// ------------------...	...----------------------------
// ------------------...	...----------------------------
// ------------------模型m的数据---------------------------
// 凸包数量n
// 凸包1的数据
// 凸包2的数据
//	...		...
// 凸包n的数据
//////////////////////////////////////////////////////////////////////////
#define CHF_FILE_VERSION (unsigned int) (0x00000003)


namespace CHBasedCD
{

	struct CHF_HEADER
	{
		unsigned int nMagic;
		unsigned int nVersion;
		unsigned int nNumModel;
	};

	struct CHFSAVERESULT 
	{
		int nFaceNum ;
		int nVertNum ;
		int nCPNum ;

		CHFSAVERESULT()
		{
			nFaceNum = 0;
			nVertNum = 0;
			nCPNum = 0;
		}
	};

typedef abase::vector<CConvexHullData> ConvexHullDataVector;
typedef abase::vector<CConvexBrush*>    ConvexBrushVector;


struct  CHFMODEL
{
	char m_sName[128];
	ConvexHullDataVector m_aCHDatas;
};

struct CHBMODEL
{
	char m_sName[128];
	ConvexBrushVector m_pCHBrushes;//element is pointer here
};

typedef abase::vector<CHFMODEL> CHFModelVector;
typedef abase::vector<CHBMODEL> CHBModelVector;

class CConvexHullDataSet  
{
public:
	
	CConvexHullDataSet();
	virtual ~CConvexHullDataSet();

	void Render(A3DFlatCollector* pFC, float fScale = 1, const A3DMATRIX4* pTransMatrix=NULL, bool bRenderV=false, int SpecialFaceId = -1, DWORD dwVColor=0xffff0000, DWORD dwFColor=0xa0ffff00, const A3DVECTOR3& vRayDir=A3DVECTOR3(1.0f,1.0f,1.0f));
	
	//void Transform(const A3DMATRIX4& mtxTrans);

	bool Save(const char* szFileName,CHFSAVERESULT& res);
	//set bPrePareForRender true when need to Render it
	bool Load(const char* szFileName, bool bPrePareForRender, CHFSAVERESULT& res);

	bool AddModel(const CHBMODEL& model);
	bool AddModel(const CHFMODEL& model);
	int GetCHFVersion(){ return m_Header.nVersion;}
	int GetModelNum() const { return m_Header.nNumModel; }
	const A3DAABB& GetAABB() const{ return m_CHAABB;}

	//注意：按名字存储或者索引只是针对UMDS的模型，其他单体模型是按照磁盘上的文件名称存储的而非A3DObject：：m_strName （因为UMD的这个名字与磁盘不符，而ecm3的这个名字根本就没有过！！）
	bool SaveModelConvexHullToFile(const char* szPath, const char* szModelName);
	CHFMODEL* GetCHFModelByName(const char* szModelName);
	CHFMODEL* GetCHFModelByIndex(int index);

	CHBMODEL* GetCHBModelByName(const char* szModelName);
	CHBMODEL* GetCHBModelByIndex(int index);

	void Release();

	CConvexHullDataSet(const CConvexHullDataSet& CHDataSet);
	CConvexHullDataSet& operator=(const CConvexHullDataSet& CHDataSet);

protected:
	void Assign(const CConvexHullDataSet& CHDataSet);

	CHFModelVector	m_aModels;//version 1 and version 2
	A3DAABB			m_CHAABB;
	CHF_HEADER		m_Header;

	CHBModelVector  m_CHBModels;//version 3
	
protected:

	bool LoadVersion1(AFile& file);
	bool LoadVersion2(AFile& file);
	bool LoadVersion3(AFile& file, bool bPrepareForRender);

};

}	// end namespace

#endif // _CONVEXHULLDATASET_H_
