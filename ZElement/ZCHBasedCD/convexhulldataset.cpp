// ConvexHullDataSet.cpp: implementation of the CConvexHullDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "ConvexHullDataSet.h"
#include <A3DTypes.h>

#include <A3DFuncs.h>
#include <AFI.h>
#include "CDWithCH.h"



namespace CHBasedCD
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ASSERTREADSUCESS(len) { if (len == 0) return false; } 


CConvexHullDataSet::CConvexHullDataSet()
{
	m_Header.nMagic = CHF_FILE_MAGIC;
	m_Header.nVersion = CHF_FILE_VERSION;
	m_Header.nNumModel = 0;// 模型数量（主要针对umds格式模型）
	m_CHAABB.Clear();
}

CConvexHullDataSet::~CConvexHullDataSet()
{
	Release();
}

void CConvexHullDataSet::Release()
{
	int i;
	for(i=0;i<(int)m_aModels.size();i++)
	{
		m_aModels[i].m_aCHDatas.clear();
	}
	for(i = 0; i < (int)m_CHBModels.size(); i++)
	{
		for (int j = 0; j < (int)m_CHBModels[i].m_pCHBrushes.size(); j++)
			delete m_CHBModels[i].m_pCHBrushes[j];
		m_CHBModels[i].m_pCHBrushes.clear();
	}
	m_aModels.clear();
	m_CHBModels.clear();
}

//version 3, only save the convex brush data
bool CConvexHullDataSet::Save(const char* szFileName,CHFSAVERESULT& res)
{
	AFile af;
	af.Open(szFileName,AFILE_CREATENEW|AFILE_BINARY);
	const int IntSize=sizeof(int);

	CHF_HEADER modelHeader = m_Header;
	modelHeader.nVersion = 3;//version 3 here 

	modelHeader.nNumModel = (int) m_CHBModels.size();// 模型数量（主要针对umds格式模型）

	// 写入凸包数量

	DWORD RealLen;
	af.Write(&modelHeader,sizeof(CHF_HEADER),&RealLen);

	for(int i=0;i<(int)modelHeader.nNumModel;i++)
	{
		int iNumHull =(int) m_CHBModels[i].m_pCHBrushes.size();
		af.Write(&iNumHull,sizeof(int),&RealLen);
		af.WriteString(m_CHBModels[i].m_sName);
		res.nCPNum+=iNumHull;

		for(int j=0;j<iNumHull;j++)
		{
			m_CHBModels[i].m_pCHBrushes[j]->Save(&af);

		}		

	}
	af.Close();

	return true;
}

bool CConvexHullDataSet::LoadVersion1(AFile& file)
{
	/*for (unsigned int i=0; i<header.nNumModel; ++i)
	{
		CHFMODEL CHFModel;
		int iNumCH;
		file.Read(&iNumCH, sizeof(int), &dwBytes);
		for (int j=0; j<iNumCH; ++j)
		{
			CConvexHullData* pConvexHullData = new CConvexHullData;
			if(!pConvexHullData->LoadBinaryData(&file))
			{
				ReleaseConvexHull();
				file.Close();
				return false;
			}

			//		pConvexHullData->Transform(TransformMatrix(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0), A3DVECTOR3(0)));

			listConvexHullData.push_back(pConvexHullData);

			m_CHAABB.Merge(pConvexHullData->GetAABB());

			// now construct a CCDBrush from CConvexhullData;
			CQBrush qBrush;
			qBrush.AddBrushBevels(pConvexHullData);
			CCDBrush * pCDBrush = new CCDBrush();
			qBrush.Export(pCDBrush);

			m_CDBrushes.push_back(pCDBrush);

		}
		AddModel(const CHFMODEL& model)
	}*/
	return false;

}

bool CConvexHullDataSet::LoadVersion2(AFile& file)
{
	DWORD dwBytes;
	m_CHAABB.Clear();
	for (int i=0; i<(int)m_Header.nNumModel; ++i)
	{
		CHFMODEL CHFModel;
		int iNumCH;
		file.Read(&iNumCH, sizeof(int), &dwBytes);
		AString str;
		file.ReadString(str);
		strcpy(CHFModel.m_sName,str.GetBuffer(0));
		for (int j=0; j<iNumCH; ++j)
		{
			CConvexHullData CHData;
			if(!CHData.LoadBinaryData(&file))
			{
				//ReleaseConvexHull();
				return false;
			}
			CHFModel.m_aCHDatas.push_back(CHData);
			m_CHAABB.Merge(CHData.GetAABB());
		}
		AddModel(CHFModel);
	}
	return true;
}

bool CConvexHullDataSet::LoadVersion3(AFile& file, bool bPrepareForRender)
{
	DWORD dwBytes;
	m_CHAABB.Clear();
	for (int i=0; i<(int)m_Header.nNumModel; ++i)
	{
		CHBMODEL CHBModel;
		int iNumCH;
		file.Read(&iNumCH, sizeof(int), &dwBytes);
		ASSERTREADSUCESS(dwBytes);
		AString str;
		file.ReadString(str);
		strcpy(CHBModel.m_sName,str.GetBuffer(0));
		for (int j=0; j<iNumCH; ++j)
		{
			CConvexBrush* pBrush = new CConvexBrush;
			if (!pBrush->Load(&file))
				return false;
			CHBModel.m_pCHBrushes.push_back(pBrush);
			m_CHAABB.Merge(pBrush->GetAABB());
		}
		m_CHBModels.push_back(CHBModel);
		
	}
	return true;
}

bool CConvexHullDataSet::Load(const char* szFileName, bool bPrepareForRender, CHFSAVERESULT& res)
{
	bool result = true;
	AFile file;

	if (!af_IsFileExist(szFileName) || !file.Open(szFileName, AFILE_OPENEXIST))
	{
		return false;
	}
	m_CHAABB.Clear();

	Release(); //release all before load
	DWORD dwBytes;
	file.Read(&m_Header, sizeof(CHF_HEADER), &dwBytes);
	ASSERTREADSUCESS(dwBytes);

	switch(m_Header.nVersion)
	{
	case 1:
		if(!LoadVersion1(file))
			result = false;
		break;
	case 2:
		if(!LoadVersion2(file))
			result = false;
		break;
	case 3:
		if (!LoadVersion3(file, bPrepareForRender))
			result = false;
		break;
	default:
		ASSERT(0 && "Wrong CHF version!");
		result = false;
		break;
	}
	
	file.Close();

	if (!result)
	{
		Release();
	}
	else
	{
		int i, j;
		if (m_Header.nVersion < 3)
		{
			for(i=0;i<(int)m_aModels.size();i++)
			{
				int iNumHull =(int) m_aModels[i].m_aCHDatas.size();
				res.nCPNum+=iNumHull;

				for(j=0;j<iNumHull;j++)
				{
					res.nFaceNum+=m_aModels[i].m_aCHDatas[j].GetFaceNum();
					for (int k = 0; k<m_aModels[i].m_aCHDatas[j].GetFaceNum(); k++)
					{
						CFace* pFace = m_aModels[i].m_aCHDatas[j].GetFacePtr(k);
						res.nVertNum += pFace->GetVNum();
					}
				}		

			}
		}
		else if (m_Header.nVersion == 3)//use convex brush in stead
		{
			for(i=0;i<(int)m_CHBModels.size();i++)
			{
				int iNumHull =(int) m_CHBModels[i].m_pCHBrushes.size();
				res.nCPNum+=iNumHull;

				for(j=0;j<iNumHull;j++)
				{
				}		

			}
		}
	}

	return result;
}

bool CConvexHullDataSet::AddModel(const CHBMODEL& model)
{
	m_CHBModels.push_back(model);
	return true;
}

bool CConvexHullDataSet::AddModel(const CHFMODEL& model)
{
	m_aModels.push_back(model);
	
	//update convex brush
	CHBMODEL chbModel;	
	strcpy(chbModel.m_sName, model.m_sName);
	for (int i = 0; i < (int)model.m_aCHDatas.size(); i++)
	{
		CConvexBrush* pBrush = new CConvexBrush;
		CConvexHullData* pTmp = const_cast<CConvexHullData*>(&model.m_aCHDatas[i]);
		pBrush->Import(pTmp);
		chbModel.m_pCHBrushes.push_back(pBrush);
	}
	m_CHBModels.push_back(chbModel);
	
	return true;
}

void CConvexHullDataSet::Render(A3DFlatCollector* pFC, float fScale, const A3DMATRIX4* pTransMatrix, bool bRenderV, int SpecialFaceId, DWORD dwVColor, DWORD dwFColor, const A3DVECTOR3& vRayDir)
{
	for(int i=0;i<(int)m_CHBModels.size();i++)
	{
		int iNumHull =(int) m_CHBModels[i].m_pCHBrushes.size();
		for(int j=0;j<iNumHull;j++)
		{
			m_CHBModels[i].m_pCHBrushes[j]->DebugRender(pFC, fScale, dwFColor, vRayDir, pTransMatrix);
		}		

	}
}

CHFMODEL* CConvexHullDataSet::GetCHFModelByName(const char* szModelName)
{
	for(int i=0;i<(int)m_aModels.size();i++)
	{
		if (strcmp(m_aModels[i].m_sName,szModelName)==0)
		{
			return &m_aModels[i];
		}
	}
	return NULL;
}

CHBMODEL* CConvexHullDataSet::GetCHBModelByName(const char* szModelName)
{
	for(int i=0;i<(int)m_CHBModels.size();i++)
	{
		if (strcmp(m_CHBModels[i].m_sName,szModelName)==0)
		{
			return &m_CHBModels[i];
		}
	}
	return NULL;
}

CHBMODEL* CConvexHullDataSet::GetCHBModelByIndex(int index)
{
	if (index<0||index>=(int)m_CHBModels.size())
	{
		return NULL;
	}
	return &m_CHBModels[index];
}

bool CConvexHullDataSet::SaveModelConvexHullToFile(const char* szPath, const char* szModelName)
{
	//save convex brush data in new version
	CHBMODEL* model = GetCHBModelByName(szModelName);
	if (!model)
	{
		return false;
	}
	CHF_HEADER modelHeader = m_Header;
	modelHeader.nVersion = 3;//version 3 here 
	modelHeader.nNumModel = 1;

	DWORD dwBytes;
	AFile af;
	if(!af.Open(szPath, AFILE_CREATENEW | AFILE_BINARY))
		return false;
	af.Write(&modelHeader, sizeof(CHF_HEADER), &dwBytes);
	int iConvexHullNum = (int)model->m_pCHBrushes.size();
	af.Write(&iConvexHullNum, sizeof(int), &dwBytes);
	af.WriteString(szModelName);
	ConvexBrushVector::const_iterator it = model->m_pCHBrushes.begin();
	for (; it!=model->m_pCHBrushes.end(); ++it)
	{
		(*it)->Save(&af);
	}
	af.Close();

	return true;
}

CHFMODEL* CConvexHullDataSet::GetCHFModelByIndex(int index)
{
	if (index<0||index>=(int)m_aModels.size())
	{
		return NULL;
	}
	return &m_aModels[index];
}

CConvexHullDataSet::CConvexHullDataSet(const CConvexHullDataSet& CHDataSet)
{
	Assign(CHDataSet);
}
CConvexHullDataSet& CConvexHullDataSet::operator=(const CConvexHullDataSet& CHDataSet)
{
	if (this == &CHDataSet)
		return *this;
	Release();
	Assign(CHDataSet);
	return *this;
}

void CConvexHullDataSet::Assign(const CConvexHullDataSet& CHDataSet)
{
	m_aModels = CHDataSet.m_aModels;
	m_CHAABB = CHDataSet.m_CHAABB;
	m_Header = CHDataSet.m_Header;

	size_t i, j;
	
	for (i = 0; i < CHDataSet.m_CHBModels.size(); i++)
	{
		CHBMODEL chbModel;
		strcpy(chbModel.m_sName, CHDataSet.m_CHBModels[i].m_sName);

		for (j = 0; j < CHDataSet.m_CHBModels[i].m_pCHBrushes.size(); j++)
		{
			CConvexBrush* pBrush = new CConvexBrush(*(CHDataSet.m_CHBModels[i].m_pCHBrushes[j]));
			
			chbModel.m_pCHBrushes.push_back(pBrush);
		}
		m_CHBModels.push_back(chbModel);
	}
}

}	// end namespace