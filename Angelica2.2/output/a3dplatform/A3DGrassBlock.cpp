#include "A3DFuncs.h"
#include "A3DGrassBlock.h"
#include "A3DGrassland.h"
#include "A3DGrassType.h"
#include "A3DGrassBitsMap.h"
#include "A3DGrassInteractor.h"
#include "A3DStream.h"
#include <algorithm>
#include <vector>

A3DGrassBlock::A3DGrassBlock()
: m_pClusters(NULL)
{
	m_pGrassBitsMap		= NULL;

	m_pGrassesVerts		= NULL;
	m_nNumGrasses		= 0;
	m_nRenderGrassNum	= 0;

	m_bBuilt			= false;
	m_bAboutToBuild		= false;
	m_bAboutToDestroy	= false;
}

A3DGrassBlock::~A3DGrassBlock()
{
}

bool A3DGrassBlock::Init(A3DGrassBitsMap * pBitsMap, const ARectI& rectBlock)
{
	m_pGrassBitsMap		= pBitsMap;
	m_pGrassLand		= pBitsMap->GetGrassType()->GetGrassLand();
	m_rectBlock			= rectBlock;

	m_bBuilt			= false;

	A3DVECTOR3 vecDeltaX = A3DVECTOR3(m_pGrassBitsMap->GetGridSize(), 0.0f, 0.0f);
	A3DVECTOR3 vecDeltaZ = A3DVECTOR3(0.0f, 0.0f, -m_pGrassBitsMap->GetGridSize());
	A3DVECTOR3 vecOrigin = m_pGrassBitsMap->GetOrigin() + vecDeltaX * m_rectBlock.left + vecDeltaZ * m_rectBlock.top;

	m_aabb.Mins = m_pGrassBitsMap->GetOrigin() + vecDeltaX * m_rectBlock.left + vecDeltaZ * m_rectBlock.top;
	m_aabb.Mins.y = 0.0f;
	m_aabb.Maxs = m_pGrassBitsMap->GetOrigin() + vecDeltaX * m_rectBlock.right + vecDeltaZ * m_rectBlock.bottom;
	m_aabb.Maxs.y = 800.0f;
	float temp = m_aabb.Mins.z;
	m_aabb.Mins.z = m_aabb.Maxs.z;
	m_aabb.Maxs.z = temp;
	m_aabb.CompleteCenterExts();

	m_vecCenter = m_aabb.Center;
	return true;
}

bool A3DGrassBlock::Release()
{
	DestroyGrasses();
	return true;
}

bool A3DGrassBlock::BuildGrasses()
{
	// now try to destroy old ones
	DestroyGrasses();

	if( !m_pGrassBitsMap || !m_pGrassBitsMap->GetGrassType() )
		return true;

	// then build the grasses here
	
	int nPitch = m_pGrassBitsMap->GetWidth() / 8;
	BYTE * pBitsMap = m_pGrassBitsMap->GetBitsMap();
	
	ARandomGen grassScatter;
	grassScatter.Init(m_rectBlock.left + m_rectBlock.top);

	int s = 5 - int(m_pGrassBitsMap->GetGrassType()->GetGrassLand()->GetLODLevel() * 4 + 0.5f);
	if( s == 5 )
	{
		m_nNumGrasses = 0;
		m_nRenderGrassNum = 0;
		m_bBuilt = true;
		m_bAboutToDestroy = false;
		m_bAboutToBuild = false;
		return true;
	}
	else if( s < 1 ) 
		s = 1;

	// first scan to get out the number of grasses
	m_nNumGrasses = 0;
	for(int y=m_rectBlock.top; y<m_rectBlock.bottom; y+=s)
	{
		for(int x=m_rectBlock.left; x<m_rectBlock.right; x+=s)
		{
			BYTE data = pBitsMap[y * nPitch + (x >> 3)];
			if( data & (1 << (7 - (x & 7))) )
			{
				m_nNumGrasses ++;
			}
		}
	}
	
	if( m_nNumGrasses == 0 )
	{
		m_nRenderGrassNum = 0;
		m_bBuilt = true;
		m_bAboutToDestroy = false;
		m_bAboutToBuild = false;
		return true;
	}

	ImplBuildGrasses(grassScatter, s);

	m_bBuilt = true;
	m_bAboutToDestroy = false;
	m_bAboutToBuild = false;
	return true;
}

bool A3DGrassBlock::ImplBuildGrasses(ARandomGen& grassScatter, int s)
{
	A3DVECTOR3 vecDeltaX = A3DVECTOR3(m_pGrassBitsMap->GetGridSize(), 0.0f, 0.0f);
	A3DVECTOR3 vecDeltaZ = A3DVECTOR3(0.0f, 0.0f, -m_pGrassBitsMap->GetGridSize());
	A3DVECTOR3 vecOrigin = m_pGrassBitsMap->GetOrigin() + vecDeltaX * m_rectBlock.left + vecDeltaZ * m_rectBlock.top;

	int nNumVertOne = m_pGrassBitsMap->GetGrassType()->GetNumOneGrassVert();
	GRASSVERTEX * pVertsOne = m_pGrassBitsMap->GetGrassType()->GetVertsTemplate();

	m_pGrassesVerts = new GRASSVERTEX[m_nNumGrasses * nNumVertOne];
	m_pClusters = new GRASSCLUSTER[m_nNumGrasses];
	GRASSVERTEX * pVerts = m_pGrassesVerts;
	GRASSCLUSTER* pClusters = m_pClusters;

	float vSize = m_pGrassBitsMap->GetGrassType()->GetDefineData().vSize;
	float vSizeVar = m_pGrassBitsMap->GetGrassType()->GetDefineData().vSizeVar;
	float vSoftness = m_pGrassBitsMap->GetGrassType()->GetDefineData().vSoftness;
	A3DVECTOR3 vecLineOrigin = vecOrigin;
	BYTE * pBitsMap = m_pGrassBitsMap->GetBitsMap();
	const int nPitch = m_pGrassBitsMap->GetWidth() / 8;

	for(int y=m_rectBlock.top; y<m_rectBlock.bottom; y+=s)
	{
		A3DVECTOR3 vecPos = vecLineOrigin;
		for(int x=m_rectBlock.left; x<m_rectBlock.right; x+=s)
		{
			BYTE data = pBitsMap[y * nPitch + (x >> 3)];

			// let random generator run
			A3DVECTOR3 vecGrassRandPos = vecPos;
			vecGrassRandPos.x += grassScatter.RandomFloat(-0.5f, 0.5f) * m_pGrassBitsMap->GetGridSize();
			vecGrassRandPos.z += grassScatter.RandomFloat(-0.5f, 0.5f) * m_pGrassBitsMap->GetGridSize();
			vecGrassRandPos.y = m_pGrassBitsMap->GetGrassType()->GetGrassLand()->GetHeightAt(vecGrassRandPos) - 0.02f;
			float vRad = grassScatter.RandomFloat(0.0f, 6.28f);
			float vScale = vSize * grassScatter.RandomFloat(1.0f - vSizeVar, 1.0f + vSizeVar);
			A3DMATRIX4 tm = RotateY(vRad) * Scaling(vScale, vScale, vScale) * Translate(vecGrassRandPos.x, vecGrassRandPos.y, vecGrassRandPos.z);
			int nPhase = int(fabs(vecGrassRandPos.x - 10000.0f) * 2.0f + grassScatter.RandomFloat(0.0f, 10.0f)) % WIND_BUF_LEN;

			if( data & (1 << (7 - (x & 7))) )
			{
				pClusters->aabb.Clear();
				pClusters->dwFlag = 0;
				pClusters->offset = 0;
				pClusters->vDir.Set(0,0,0);

				// has one grass here
				for(int i=0; i<nNumVertOne; i++)
				{
					pVerts[i] = pVertsOne[i];

					pVerts[i].pos = pVerts[i].pos * tm;
					pVerts[i].normal.x = float(nPhase);
					pVerts[i].normal.y *= vSoftness;
					pClusters->aabb.AddVertex(pVerts[i].pos);
					UpdateAABBY(pVerts[i].pos.y);
				}

				pClusters->aabb.CompleteCenterExts();
				pClusters++;
				pVerts += nNumVertOne;
			}

			vecPos = vecPos + vecDeltaX;
		}

		vecLineOrigin = vecLineOrigin + vecDeltaZ;
	}

	return true;
}

bool A3DGrassBlock::DestroyGrasses()
{
	ImplDestroyGrasses();

	m_bBuilt = false;
	m_bAboutToDestroy = false;
	m_bAboutToBuild = false;

	m_nRenderGrassNum = 0;
	m_nNumGrasses = 0;
	return true;
}

bool A3DGrassBlock::ImplDestroyGrasses()
{
	if( m_pGrassesVerts )
	{
		delete [] m_pGrassesVerts;
		m_pGrassesVerts = NULL;
	}
	
	if(m_pClusters != NULL)
	{
		delete [] m_pClusters;
		m_pClusters = NULL;
	}

	return true;
}

bool A3DGrassBlock::UpdateGrassOffset(float fDeltaTime)
{
	A3DGrassInteractor* pInteractor = m_pGrassLand->GetGrassInteractor();
	if ( !pInteractor )
		return false;

	ARectF rcBlock(m_aabb.Mins.x, m_aabb.Mins.z, m_aabb.Maxs.x, m_aabb.Maxs.z);
    
    const A3DVECTOR3& vPlayerCenter = pInteractor->GetCenter();
    const A3DVECTOR3& vBlockCenter = m_aabb.Center;
    
    float fDist = 0;
    if (vPlayerCenter.x < m_aabb.Mins.x)
    {
        float d = m_aabb.Mins.x - vPlayerCenter.x;
        fDist += d * d;
    }
    else if (vPlayerCenter.x > m_aabb.Maxs.x)
    {
        float d = m_aabb.Maxs.x - vPlayerCenter.x;
        fDist += d * d;
    }
    if (vPlayerCenter.z < m_aabb.Mins.z)
    {
        float d = m_aabb.Mins.z - vPlayerCenter.z;
        fDist += d * d;
    }
    else if (vPlayerCenter.z > m_aabb.Maxs.z)
    {
        float d = m_aabb.Maxs.z - vPlayerCenter.z;
        fDist += d * d;
    }

    if (fDist > 20)
        return true;

	rcBlock.Normalize();
	if ((pInteractor->GetArea() & rcBlock).IsEmpty())
		return false;
    if (fDeltaTime > 0.1f)
        fDeltaTime = 0.1f;
	return ImplUpdateGrassOffset(m_pGrassLand->GetGrassInteractor(), fDeltaTime);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement A3DGrassBlockHW
//	
///////////////////////////////////////////////////////////////////////////

void A3DGrassBlockHW::GRASS_INSTS::SetGrassRenderNum(int iGrassRenderNum)
{
	Clear();

	m_pGrassInstTrans = new A3DGRASSINSTANCEDATA[iGrassRenderNum];
	memset( m_pGrassInstTrans, 0, sizeof(A3DGRASSINSTANCEDATA) * iGrassRenderNum );
	m_pGrassMoveStatus = new a_byte[iGrassRenderNum];
	memset( m_pGrassMoveStatus, 0, sizeof(a_byte) * iGrassRenderNum );
	m_pSpeed = new A3DVECTOR3[iGrassRenderNum];
	memset( m_pSpeed, 0, sizeof(A3DVECTOR3) * iGrassRenderNum );
}

void A3DGrassBlockHW::GRASS_INSTS::Clear()
{
	delete [] m_pGrassInstTrans;
	m_pGrassInstTrans = NULL;
	delete [] m_pGrassMoveStatus;
	m_pGrassMoveStatus = NULL;
	delete [] m_pSpeed;
	m_pSpeed = NULL;
}

A3DGrassBlockHW::A3DGrassBlockHW()
: m_pInstStream(NULL)
{

}

A3DGrassBlockHW::~A3DGrassBlockHW()
{

}

//	Generate a mask array for filling the grass verts buffer
//	ASSUME nGrassMaskSize > nNumLimit
//	we choose nNumLimit number of grasses from pGrassMask randomly leave them as false
//	and set the rest to be true
bool A3DGrassBlockHW::GenGrassMaskArray(bool* pGrassMask, int nGrassMaskSize, int nNumLimit)
{
	ASSERT( nGrassMaskSize > nNumLimit );

	std::vector<int> aGrassIndexs;
	aGrassIndexs.reserve(nGrassMaskSize);
	for (int i = 0; i < nGrassMaskSize; ++i)
		aGrassIndexs.push_back(i);

	std::random_shuffle(aGrassIndexs.begin(), aGrassIndexs.end());

	for (int i = nNumLimit; i < nGrassMaskSize; ++i)
		pGrassMask[aGrassIndexs[i]] = true;

	return true;
}

bool A3DGrassBlockHW::ImplBuildGrasses(ARandomGen& GrassScatter, int s)
{
	A3DGrassLand* pGrassLand = GetGrassBitsMap()->GetGrassType()->GetGrassLand();
	if (!pGrassLand)
		return false;

	ASSERT( GetNumGrasses() > 0 );

	const int nPitch = GetGrassBitsMap()->GetWidth() / 8;
	BYTE * pBitsMap = GetGrassBitsMap()->GetBitsMap();
	A3DGrassType* pGrassType = GetGrassBitsMap()->GetGrassType();

	//const int nMaxRenderGrassNum = pGrassType->GetNumBufGrasses();
	const int nTotalGrassNum = GetNumGrasses();
	const int nMaxRenderGrassNum = nTotalGrassNum;
	AArray<bool, bool> aIgnoreGrassMask;
	aIgnoreGrassMask.SetSize(nTotalGrassNum, 16);
	memset(aIgnoreGrassMask.GetData(), 0, nTotalGrassNum * sizeof(bool));
	if (nTotalGrassNum > nMaxRenderGrassNum)
	{
		GenGrassMaskArray(aIgnoreGrassMask.GetData(), nTotalGrassNum, nMaxRenderGrassNum);
	}

	//	this limitation is added to make HW instance rendering has 
	//	the same appearance with the Stream instance rendering
	m_nRenderGrassNum = a_Min(nMaxRenderGrassNum, nTotalGrassNum);
	const int nTotalNumVerts = m_nRenderGrassNum;

	std::vector<int> aLODs;
	aLODs.reserve(m_nRenderGrassNum);
	for (int i = 0; i < m_nRenderGrassNum; ++i)
	{
		aLODs.push_back(i);
	}

	std::random_shuffle(aLODs.begin(), aLODs.end());

	m_aInstData.SetGrassRenderNum(nTotalNumVerts);

	//	then build the grasses here
	A3DVECTOR3 vecDeltaX = A3DVECTOR3(GetGrassBitsMap()->GetGridSize(), 0.0f, 0.0f);
	A3DVECTOR3 vecDeltaZ = A3DVECTOR3(0.0f, 0.0f, -GetGrassBitsMap()->GetGridSize());
	A3DVECTOR3 vecOrigin = GetGrassBitsMap()->GetOrigin() + vecDeltaX * m_rectBlock.left + vecDeltaZ * m_rectBlock.top;

	const A3DGrassType::GRASSDEFINE_DATA& define_data = GetGrassBitsMap()->GetGrassType()->GetDefineData();

	int nGrassIdx = 0;
	int nGrassRenderIdx = 0;
	A3DGRASSINSTANCEDATA* pGrassInstTrans = m_aInstData.GetInstData();
	ASSERT( m_aInstData.GetInstData() );

	A3DVECTOR3 vecLineOrigin = vecOrigin;
	for(int y=m_rectBlock.top; y<m_rectBlock.bottom; y+=s)
	{
		A3DVECTOR3 vecPos = vecLineOrigin;
		for(int x=m_rectBlock.left; x<m_rectBlock.right; x+=s)
		{
			BYTE data = pBitsMap[y * nPitch + (x >> 3)];

			// let random generator run
			A3DVECTOR3 vecGrassRandPos = vecPos;
			if (define_data.bRandomPos)
			{
				vecGrassRandPos.x += GrassScatter.RandomFloat(-0.5f, 0.5f) * GetGrassBitsMap()->GetGridSize();
				vecGrassRandPos.z += GrassScatter.RandomFloat(-0.5f, 0.5f) * GetGrassBitsMap()->GetGridSize();
			}
			vecGrassRandPos.y = pGrassLand->GetHeightAt(vecGrassRandPos) - 0.02f;

			float vRad = GrassScatter.RandomFloat(0.0f, 6.28f);
			float vScale = define_data.vSize * GrassScatter.RandomFloat(1.0f - define_data.vSizeVar, 1.0f + define_data.vSizeVar);

			//	Test if current bit has one grass exist
			if( data & (1 << (7 - (x & 7))) )
			{
				//	Test if this grass will be rendered
				if (!aIgnoreGrassMask[nGrassIdx])
				{
					UpdateAABBY(vecGrassRandPos.y);
					A3DGRASSINSTANCEDATA& instData = pGrassInstTrans[aLODs[nGrassRenderIdx]];
					instData.worldPos = vecGrassRandPos;
					instData.scaleRot.Set(cosf(vRad), sinf(vRad), vScale);
					nGrassRenderIdx++;
				}

				nGrassIdx ++;
			}

			vecPos = vecPos + vecDeltaX * s;
		}

		vecLineOrigin = vecLineOrigin + vecDeltaZ * s;
	}

	m_aabb.CompleteCenterExts();
	std::auto_ptr<A3DStream> pInstStream(new A3DStream);
	if (!pInstStream->Init(pGrassLand->GetA3DDevice()
		, sizeof(A3DGRASSINSTANCEDATA)
		, 0
		, GetNumGrasses()
		, 0
		, A3DSTRM_REFERENCEPTR
		, 0))
		return false;

	pInstStream->SetVertexRef(reinterpret_cast<BYTE*>(m_aInstData.GetInstData()));
	pInstStream->SetVerts(m_aInstData.GetInstData(), m_nRenderGrassNum);
	m_pInstStream = pInstStream.release();
	return true;
}


//	Destroy grass resource in HWInstance mode
bool A3DGrassBlockHW::ImplDestroyGrasses()
{
	A3DRELEASE( m_pInstStream );

	m_aInstData.Clear();
	return true;
}

static float SPEED_FACTOR = 70.f;
static float DAMPING_FACTOR = 4.5f;
static float PRESS_RADIUS = 1.0f;
static float PRESS_AMOUNT = 1.0f;

void AfxSetGrassShakeFactor(float fSpeed, float fDamping)
{
    SPEED_FACTOR = fSpeed;
    DAMPING_FACTOR = fDamping;
}

void AfxSetGrassPressFactor(float fRadius, float fAmount)
{
    PRESS_RADIUS = fRadius;
    PRESS_AMOUNT = fAmount;
}

inline A3DVECTOR3 GetOffsetH(const A3DVECTOR3& v)
{
	return A3DVECTOR3(v.x, 0, v.z);	
}
//	Update grass offset
bool A3DGrassBlockHW::ImplUpdateGrassOffset(A3DGrassInteractor* pInteractor, float fDeltaTime)
{
    //if (GetAsyncKeyState(VK_F9) < 0)
    //{
    //    if (GetAsyncKeyState(VK_SHIFT) < 0)
    //        SPEED_FACTOR -= 0.1f * fDeltaTime;
    //    else
    //        SPEED_FACTOR += 0.1f * fDeltaTime;
    //    a_Clamp(SPEED_FACTOR, 1.0f, 180.0f);
    //    AString txtDbg;
    //    txtDbg.Format("SPEED_ACTOR = %g\n", SPEED_FACTOR);
    //    OutputDebugStringA(txtDbg);
    //}
    //if (GetAsyncKeyState(VK_F10) < 0)
    //{
    //    if (GetAsyncKeyState(VK_SHIFT) < 0)
    //        DAMPING_FACTOR -= 0.01f * fDeltaTime;
    //    else
    //        DAMPING_FACTOR += 0.01f * fDeltaTime;
    //    a_Clamp(DAMPING_FACTOR, 0.0f, 10.0f);
    //    AString txtDbg;
    //    txtDbg.Format("DAMPING_FACTOR = %g\n", DAMPING_FACTOR);
    //    OutputDebugStringA(txtDbg);
    //}
	__int64 time = ACounter::GetMicroSecondNow();

	float fGrassHeight = GetGrassBitsMap()->GetGrassType()->GetGrassHeight();
	a_ClampFloor(fGrassHeight, 0.1f);

	bool bUnChanged = true;
	for (int i = 0; i < m_nRenderGrassNum; ++i)
	{
		float fMagnitude = 0.0f;
		A3DVECTOR3 vOffset;

		A3DGRASSINSTANCEDATA& instData = m_aInstData.GetInstData()[i];
		a_byte& state = m_aInstData.GetMoveStatus()[i];
		A3DVECTOR3& speed = m_aInstData.GetSpeed()[i];
        
        if ((instData.worldPos - pInteractor->GetCenter()).SquaredMagnitudeH() > 20)
            continue;

        if (state == Shaking)
        {
            instData.offset += speed * fDeltaTime;
            speed += -GetOffsetH(instData.offset) * SPEED_FACTOR * fDeltaTime;
            speed += -speed * DAMPING_FACTOR * fDeltaTime;
            //speed *= 0.9f;
            bUnChanged = false;
            instData.offset.y = 0.0f;

            if (speed.SquaredMagnitudeH() < 0.001f
                && instData.offset.SquaredMagnitudeH() < 0.001f)
            {
                state = Free;
                instData.offset.Clear();
            }
        }


		if (pInteractor->IsGrassBePressed(instData.worldPos, 0.1f, PRESS_RADIUS, fGrassHeight, vOffset))
		{
			//	Be pressed
			vOffset.y = 0;

			float fLength = vOffset.Normalize() / PRESS_RADIUS;
			a_Clamp(fLength, 0.3f, 1.0f);
            float fPressLength = sqrtf(1.0f -fLength * fLength) * PRESS_AMOUNT;
            A3DVECTOR3 vGrassOff = vOffset * fPressLength;
            if (fPressLength + 0.0001f >= instData.offset.MagnitudeH())
            {
			    instData.offset.Set(vGrassOff.x, 1, vGrassOff.z);
			    speed.Set(0, 0, 0);
			    state = Pressed;
            }
            else
            {
                if (fabs(vGrassOff.x) > fabs(instData.offset.x))
                    instData.offset.x = vGrassOff.x;
                if (fabs(vGrassOff.z) > fabs(instData.offset.z))
                    instData.offset.z = vGrassOff.z;
                state = Shaking;
            }
            bUnChanged = false;
		}
		else
		{
			//	unpressed
			//	check is free or shaking
			//	From pressed to unpressed
			if (state == Pressed)
			{
				state = Shaking;
				//	y is used as a flag to make shader do the right thing
				//	when pressed, we do not consider the wind, but during the shaking or free we let the wind affects
				instData.offset.y = 0.0f;
			}
        }
	}
    
    __int64 tt = ACounter::GetMicroSecondNow(); 
    __int64 t = tt - time;
	if (!bUnChanged)
		m_pInstStream->SetVerts(m_aInstData.GetInstData(), m_nRenderGrassNum);

	pInteractor->AddUpdateTime(ACounter::GetMicroSecondNow() - time);

    //AString strOutput;
    //strOutput.Format("Time = %d, %d\n", t, ACounter::GetMicroSecondNow() - tt);
    //OutputDebugStringA(strOutput);
    return true;
}
