#include "A3DDistObject.h"
#include "A3DFuncs.h"
#include "A3DDevice.h"
#include "A3DEngine.h"
#include "A3DTexture.h"
#include "A3DTextureMan.h"
#include "A3DStream.h"
#include "A3DDistObjectRender.h"
#include "AFile.h"
#include "A3DGFXEx.h"

extern ALog g_A3DErrLog;

//////////////////////////////////////////////////////////////////////////
// class A3DDistantObject
//		
//		interface for distant object
//////////////////////////////////////////////////////////////////////////

A3DDistantObject::A3DDistantObject()
: m_vPos(0, 0, 0), m_vDir(0, 0, 1), m_vUp(0, 1, 0), m_fScale(1.0f),
m_pCustomObject(NULL)
{
    UpdateTransform();
}

A3DDistantObject::~A3DDistantObject()
{

}

void A3DDistantObject::UpdateTransform()
{
    m_matTransform = Scaling(m_fScale, m_fScale, m_fScale) * TransformMatrix(m_vDir, m_vUp, m_vPos);
}

bool A3DDistantObject::CopyFrom(A3DDistantObject* pOther)
{
    this->m_vPos = pOther->m_vPos;
    this->m_vDir = pOther->m_vDir;
    this->m_vUp  = pOther->m_vUp;
    this->m_fScale = pOther->m_fScale;
    this->m_matTransform = pOther->m_matTransform;
    this->m_AABB = pOther->m_AABB;
    return true;
}

void A3DDistantObject::SetDirAndUp( const A3DVECTOR3& vDir, const A3DVECTOR3& vUp )
{
    m_vDir = vDir;  
    m_vUp = vUp; 
    m_vDir.Normalize(); 
    m_vUp.Normalize();
    UpdateTransform();
}

//////////////////////////////////////////////////////////////////////////
// class A3DDistantBoard
//		
//////////////////////////////////////////////////////////////////////////
static A3DVECTOR3 s_vBoardPos[4] = { A3DVECTOR3(-0.5f, 0, 0), A3DVECTOR3(0.5f, 0, 0), A3DVECTOR3(0.5f, 1, 0), A3DVECTOR3(-0.5f, 1, 0)};

A3DDistantBoard::Attachment::~Attachment()
{
    A3DRELEASE(pGFX);
}

A3DDistantBoard::A3DDistantBoard()
: m_pA3DDevice(NULL),
  m_vSize(10, 10, 10),
  m_pDiffuseTexture(NULL),
  m_pSpecularTexture(NULL),
  m_pNormalTexture(NULL),
  m_pShadowTexture(NULL),
  m_pStream(NULL),
  m_nGridCols(1),
  m_nGridRows(1),
  m_fTortuosity(0),
  m_nCurveType(CURVE_X),
  m_vSpecular(2, 2, 2, 10),
  m_fDepthScale(2)
{
}

A3DDistantBoard::~A3DDistantBoard()
{
}

bool A3DDistantBoard::Init(A3DDevice* pA3DDevice)
{
    m_pA3DDevice = pA3DDevice;
    CreateStream();
   // AttachGFX("GFX\\³¡¾°\\»ð\\Åè»ð.gfx", 0, 1000);
    return true;
}

void A3DDistantBoard::Release()
{
    A3DRELEASE(m_pStream);
    ClearAttach();
    if (m_pA3DDevice)
    {
        if (m_pDiffuseTexture)
            m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDiffuseTexture);
        if (m_pSpecularTexture)
            m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pSpecularTexture);
        if (m_pNormalTexture)
            m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pNormalTexture);
        if (m_pShadowTexture)
            m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pShadowTexture);
        m_pA3DDevice = NULL;
    }
}

A3DDistantObject* A3DDistantBoard::Clone()
{
    A3DDistantBoard* pOther = new A3DDistantBoard();
    if (!pOther)
        return NULL;
    pOther->Init(m_pA3DDevice);
    pOther->CopyFrom(this);
    pOther->SetDiffuseTexture(m_strDiffuseTexture);
    pOther->SetSpecularTexture(m_strSpecularTexture);
    pOther->SetNormalTexture(m_strNormalTexture);
    pOther->m_vSize = m_vSize;

    for (size_t i = 0; i < m_aAttachment.size(); i++)
    {
        pOther->AttachGFX(m_aAttachment[i]->strFileName, m_aAttachment[i]->vPos, m_aAttachment[i]->fScale);
    }

    return pOther;
}

bool A3DDistantBoard::TickAnimation(DWORD dwDeltaTime)
{
    for (size_t i = 0; i < m_aAttachment.size(); i++)
    {
        if (m_aAttachment[i]->pGFX)
        {
            m_aAttachment[i]->pGFX->TickAnimation(dwDeltaTime);
        }
    }

    return true;
}

bool A3DDistantBoard::Render(A3DViewport* pViewport)
{
    AfxGetA3DDistantObjectRender()->RegisterObject(this);
    return true;
}

bool A3DDistantBoard::RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel, float* pfFrac)
{
    A3DMATRIX4 matWorldInv = m_matTransform;
    matWorldInv.InverseTM();
    A3DVECTOR3 vStartObject = vStart * matWorldInv;
    A3DVECTOR3 vVecObject = vVel * matWorldInv.GetRotatePart();
    if (vVecObject.z > 1e-4)
        return false;
    float fFrac = -vStartObject.z / vVecObject.z;
    if (pfFrac && fFrac * m_fScale > *pfFrac)
        return false;

    A3DVECTOR3 vPos = vStartObject + vVecObject * fFrac;
    if (vPos.x >= -m_vSize.x / 2 && vPos.x <= m_vSize.x / 2 && vPos.y >= 0 && vPos.y <= m_vSize.y)
    {
        if (pfFrac)
            *pfFrac = fFrac * m_fScale;
        return true;
    }
    else
    {
        return false;
    }
}

void A3DDistantBoard::UpdateTransform()
{
    A3DDistantObject::UpdateTransform();
    A3DVECTOR3 vPos[4];
    for (int i = 0; i < 4; i++)
    {
        vPos[i] = s_vBoardPos[i] * m_vSize * m_matTransform;
    }
    m_AABB.Build(vPos, 4);

    for (size_t i = 0; i < m_aAttachment.size(); i++)
    {
        if (m_aAttachment[i]->pGFX)
        {
            m_aAttachment[i]->pGFX->SetPos(m_aAttachment[i]->vPos * m_matTransform);
            m_aAttachment[i]->pGFX->SetDirAndUp(m_vDir, m_vUp);
        }
    }

}

bool A3DDistantBoard::SetDiffuseTexture(const char* szTextureFile)
{
    if (m_pDiffuseTexture)
        m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDiffuseTexture);
    m_strDiffuseTexture = szTextureFile;
    m_pDiffuseTexture = LoadTexture(szTextureFile);
    return m_pDiffuseTexture != NULL;
}

bool A3DDistantBoard::SetSpecularTexture(const char* szTextureFile)
{
    if (m_pSpecularTexture)
        m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pSpecularTexture);
    m_strSpecularTexture = szTextureFile;
    m_pSpecularTexture = LoadTexture(szTextureFile);
    return m_pSpecularTexture != NULL;
}

bool A3DDistantBoard::SetNormalTexture(const char* szTextureFile)
{
    if (m_pNormalTexture)
        m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pNormalTexture);
    m_strNormalTexture = szTextureFile;
    m_pNormalTexture = LoadTexture(szTextureFile);
    return m_pNormalTexture != NULL;
}

bool A3DDistantBoard::SetShadowTexture(A3DTexture* pTexture)
{
    if (m_pShadowTexture)
        m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pShadowTexture);
    m_pShadowTexture = pTexture;
    return m_pShadowTexture != NULL;
}


A3DTexture* A3DDistantBoard::LoadTexture(const char* szTextureFile)
{
    A3DTexture* pTexture = NULL;
    if (!m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szTextureFile, &pTexture))
        return NULL;

    return pTexture;
}

bool A3DDistantBoard::CreateStream()
{
    A3DRELEASE(m_pStream);

    m_pStream = new A3DStream;

    int nVertex = (m_nGridRows + 1) * (m_nGridCols + 1);
    int nIndex = m_nGridRows * m_nGridCols * 6;
    if (!m_pStream->Init(m_pA3DDevice, sizeof(A3DBOARDVERTEX), 0, nVertex, nIndex, A3DSTRM_STATIC, A3DSTRM_STATIC))
    {
        g_A3DErrLog.Log("A3DDistantBoard::CreateStream(), failed to init stream!");
        A3DRELEASE(m_pStream);
        return false;
    }

    A3DBOARDVERTEX* pVB;
    if (!m_pStream->LockVertexBuffer(0, sizeof(A3DBOARDVERTEX) * nVertex, (BYTE**)&pVB, 0))
    {
        g_A3DErrLog.Log("A3DDistantBoard::CreateStream(), failed to lock vertex buffer!");
        A3DRELEASE(m_pStream);
        return false;
    }
    
    m_AABB.Clear();
    m_OBB.Clear();
    for (int iRow = 0; iRow <= m_nGridRows; iRow++)
    {
        for (int iCol = 0; iCol <= m_nGridCols; iCol++)
        {
            float x = iCol / (float)(m_nGridCols);
            float y = iRow / (float)(m_nGridRows);

            if (fabs(m_fTortuosity) < 1e-3f)
            {
                pVB->pos = A3DVECTOR3(x - 0.5f, y, 0.0f) * m_vSize;
                pVB->normal.Set(0, 0, 1);
                pVB->tangents.Set(-1, 0, 0, -1);
            }
            else
            {
                switch(m_nCurveType)
                {
                case CURVE_X:
                    {
                        float fPhi = (x - 0.5f) * m_fTortuosity;
                        pVB->pos.x = sinf(fPhi) / m_fTortuosity;
                        pVB->pos.y = y;
                        pVB->pos.z = (1 - cosf(fPhi)) / m_fTortuosity;
                        pVB->pos.x *= m_vSize.x;
                        pVB->pos.y *= m_vSize.y;
                        pVB->pos.z *= m_vSize.x;
                        pVB->normal.x = -pVB->pos.x;
                        pVB->normal.z = m_vSize.x * cosf(fPhi) / m_fTortuosity;
                        pVB->normal.y = 0;
                        pVB->normal.Normalize();
                        if (m_fTortuosity < 0)
                            pVB->normal *= -1;
                        A3DVECTOR3 vTangent = CrossProduct(pVB->normal, A3DVECTOR3(0, 1, 0));
                        pVB->tangents.Set(vTangent.x, vTangent.y, vTangent.z, -1);
                    }
                    break;
                case CURVE_Y:
                    {
                        float fTheta = y * m_fTortuosity;
                        pVB->pos.x = x - 0.5f;
                        pVB->pos.y = sinf(fTheta) / m_fTortuosity;
                        pVB->pos.z = (1 - cosf(fTheta)) / m_fTortuosity;
                        pVB->pos.x *= m_vSize.x;
                        pVB->pos.y *= m_vSize.y;
                        pVB->pos.z *= m_vSize.y;
                        pVB->normal.x = 0;
                        pVB->normal.z = m_vSize.y * cosf(fTheta) / m_fTortuosity;
                        pVB->normal.y = -pVB->pos.y;
                        pVB->normal.Normalize();
                        if (m_fTortuosity < 0)
                            pVB->normal *= -1;
                        pVB->tangents.Set(-1, 0, 0, -1);

                    }
                    break;
                default:
                    ASSERT(FALSE && "Unknown CurveType!");
                }
            }

            pVB->u = 1 - x;
            pVB->v = 1 - y;
            m_AABB.AddVertex(pVB->pos * m_matTransform);
            m_OBB.AddVertex(pVB->pos);
            pVB++;
        }
    }
    m_pStream->UnlockVertexBuffer();
    m_AABB.CompleteCenterExts();
    m_OBB.CompleteCenterExts();
    WORD* pIB;
    if (!m_pStream->LockIndexBuffer(0, sizeof(WORD) * nIndex, (BYTE**)&pIB, 0))
    {
        g_A3DErrLog.Log("A3DDistantBoard::CreateStream(), failed to lock index buffer!");
        A3DRELEASE(m_pStream);
        return false;
    }

    for (int iRow = 0; iRow < m_nGridRows; iRow++)
    {
        for (int iCol = 0; iCol < m_nGridCols; iCol++)
        {
            int index = iRow * (m_nGridCols + 1) + iCol;
            WORD indices[4] = { index, index + 1, index + m_nGridCols + 1, index + m_nGridCols + 2 };
            *pIB++ = indices[0];
            *pIB++ = indices[1];
            *pIB++ = indices[3];
            *pIB++ = indices[0];
            *pIB++ = indices[3];
            *pIB++ = indices[2];
        }

    }
    
    m_pStream->UnlockIndexBuffer();

    return true;
}

bool A3DDistantBoard::RenderAtOnce()
{
    m_pStream->Appear(0, false);
    m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, m_pStream->GetVertCount(), 0, m_pStream->GetIndexCount() / 3);
    return true;
}

void A3DDistantBoard::SetGrid(int nRows, int nCols)
{
    m_nGridRows = a_Max(1, nRows);
    m_nGridCols = a_Max(1, nCols);
}

#define DISTANT_BOARD_VERSION 3

bool A3DDistantBoard::LoadShadowMap(AFile* pFile)
{
    DWORD dwRead;
    DWORD dwVersion;

    if (!pFile->Read(&dwVersion, sizeof(DWORD), &dwRead))
        return false;

    if (dwVersion > DISTANT_BOARD_VERSION)
    {
        g_A3DErrLog.Log("A3DDistantBoard::Load(), Unknown version!");
        return false;
    }

    m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pShadowTexture);
    
    DWORD dwTextureSize;
    if (!pFile->Read(&dwTextureSize, sizeof(DWORD), &dwRead))
        return false;

    if (dwTextureSize == 0)
    {
        m_pShadowTexture = NULL;
        return true;
    }
    else
    {
        BYTE* pBuf = (BYTE*)a_malloctemp(dwTextureSize);
        bool bSucc = false;
        if (pFile->Read(pBuf, dwTextureSize, &dwRead) && dwRead == dwTextureSize)
        {
            m_pShadowTexture = new A3DTexture;
            m_pShadowTexture->Init(m_pA3DDevice);
            if (!m_pShadowTexture->LoadFromMemory(m_pA3DDevice, pBuf, dwTextureSize, 0, 0, A3DFMT_UNKNOWN, 0, NULL))
            {
                A3DRELEASE(m_pShadowTexture);
            }
            else
            {
                bSucc = true;
            }
        }
		a_freetemp(pBuf);
        return bSucc;
    }

}

bool A3DDistantBoard::SaveShadowMap(AFile* pFile)
{
    DWORD dwWrite;
    DWORD dwVersion = DISTANT_BOARD_VERSION;

    if (!pFile->Write(&dwVersion, sizeof(DWORD), &dwWrite))
        return false;
    DWORD dwTextureSize;
    if (!m_pShadowTexture || !m_pShadowTexture->GetD3DTexture())
    {
        dwTextureSize = 0;
        if (!pFile->Write(&dwTextureSize, sizeof(DWORD), &dwWrite))
            return false;
        return true;
    }
    else
    {
        ID3DXBuffer* pBuffer;
        if (FAILED(D3DXSaveTextureToFileInMemory(&pBuffer, D3DXIFF_DDS, m_pShadowTexture->GetD3DTexture(), NULL)))
        {
            dwTextureSize = 0;
            if (!pFile->Write(&dwTextureSize, sizeof(DWORD), &dwWrite))
                return false;
        }
        dwTextureSize = pBuffer->GetBufferSize();
        if (!pFile->Write(&dwTextureSize, sizeof(DWORD), &dwWrite))
            return false;
        if (!pFile->Write(pBuffer->GetBufferPointer(), dwTextureSize, &dwWrite))
            return false;
        pBuffer->Release();
        return true;
    }
}

bool A3DDistantBoard::Load(AFile* pFile)
{
    DWORD dwRead;
    DWORD dwVersion;
    ClearAttach();

    if (!pFile->Read(&dwVersion, sizeof(DWORD), &dwRead))
        return false;

    if (dwVersion > DISTANT_BOARD_VERSION)
    {
        g_A3DErrLog.Log("A3DDistantBoard::Load(), Unknown version!");
        return false;
    }

    m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pShadowTexture);


    if (!pFile->Read(&m_vPos, sizeof(m_vPos), &dwRead))
        return false;
    if (!pFile->Read(&m_vDir, sizeof(m_vDir), &dwRead))
        return false;
    if (!pFile->Read(&m_vUp, sizeof(m_vUp), &dwRead))
        return false;
    if (!pFile->Read(&m_fScale, sizeof(m_fScale), &dwRead))
        return false;
    if (!pFile->Read(&m_vSize, sizeof(m_vSize), &dwRead))
        return false;
    if (!pFile->Read(&m_nGridRows, sizeof(m_nGridRows), &dwRead))
        return false;
    if (!pFile->Read(&m_nGridCols, sizeof(m_nGridCols), &dwRead))
        return false;
    if (!pFile->Read(&m_fTortuosity, sizeof(m_fTortuosity), &dwRead))
        return false;
    if (!pFile->Read(&m_nCurveType, sizeof(m_nCurveType), &dwRead))
        return false;
    if (!pFile->Read(&m_vSpecular, sizeof(m_vSpecular), &dwRead))
        return false;
    if (!pFile->ReadString(m_strDiffuseTexture))
        return false;
    if (!pFile->ReadString(m_strNormalTexture))
        return false;
    if (!pFile->ReadString(m_strSpecularTexture))
        return false;
    
    if (dwVersion >= 2)
    {
        if (!pFile->Read(&m_fDepthScale, sizeof(m_fDepthScale), &dwRead))
            return false;
    }
    SetDiffuseTexture(m_strDiffuseTexture);
    SetNormalTexture(m_strNormalTexture);
    SetSpecularTexture(m_strSpecularTexture);
    UpdateTransform();
    CreateStream();

    if (!LoadShadowMap(pFile))
    {
        return false;
    }
    if (dwVersion >= 3)
    {
        DWORD dwNumAttach;
        if (!pFile->Read(&dwNumAttach, sizeof(dwNumAttach), &dwRead))
            return false;

        for (DWORD i = 0; i < dwNumAttach; i++)
        {
            AString strFileName;
            A3DVECTOR3 vPos;
            float fScale;

            if (!pFile->ReadString(strFileName))
                return false;
            if (!pFile->Read(&vPos, sizeof(vPos), &dwRead))
                return false;
            if (!pFile->Read(&fScale, sizeof(fScale), &dwRead))
                return false;

            AttachGFX(strFileName, vPos, fScale);
        }
    }


    return true;
}

bool A3DDistantBoard::Save(AFile* pFile)
{
    DWORD dwWrite;
    DWORD dwVersion = DISTANT_BOARD_VERSION;

    if (!pFile->Write(&dwVersion, sizeof(DWORD), &dwWrite))
        return false;

    if (!pFile->Write(&m_vPos, sizeof(m_vPos), &dwWrite))
        return false;
    if (!pFile->Write(&m_vDir, sizeof(m_vDir), &dwWrite))
        return false;
    if (!pFile->Write(&m_vUp, sizeof(m_vUp), &dwWrite))
        return false;
    if (!pFile->Write(&m_fScale, sizeof(m_fScale), &dwWrite))
        return false;
    if (!pFile->Write(&m_vSize, sizeof(m_vSize), &dwWrite))
        return false;
    if (!pFile->Write(&m_nGridRows, sizeof(m_nGridRows), &dwWrite))
        return false;
    if (!pFile->Write(&m_nGridCols, sizeof(m_nGridCols), &dwWrite))
        return false;
    if (!pFile->Write(&m_fTortuosity, sizeof(m_fTortuosity), &dwWrite))
        return false;
    if (!pFile->Write(&m_nCurveType, sizeof(m_nCurveType), &dwWrite))
        return false;
    if (!pFile->Write(&m_vSpecular, sizeof(m_vSpecular), &dwWrite))
        return false;
    if (!pFile->WriteString(m_strDiffuseTexture))
        return false;
    if (!pFile->WriteString(m_strNormalTexture))
        return false;
    if (!pFile->WriteString(m_strSpecularTexture))
        return false;
    if (!pFile->Write(&m_fDepthScale, sizeof(m_fDepthScale), &dwWrite))
        return false;

    if (!SaveShadowMap(pFile))
    {
        return false;
    }

    DWORD dwNumAttach = GetAttachCount();
    if (!pFile->Write(&dwNumAttach, sizeof(dwNumAttach), &dwWrite))
        return false;

    for (size_t i = 0; i < m_aAttachment.size(); i++)
    {
        if (!pFile->WriteString(m_aAttachment[i]->strFileName))
            return false;
        if (!pFile->Write(&m_aAttachment[i]->vPos, sizeof(A3DVECTOR3), &dwWrite))
            return false;
        if (!pFile->Write(&m_aAttachment[i]->fScale, sizeof(float), &dwWrite))
            return false;
    }
    return true;
}

bool A3DDistantBoard::AttachGFX(const char* szFileName, const A3DVECTOR3& vPos, float fScale)
{
    A3DGFXEx* pGFX = new A3DGFXEx;
    pGFX->Init(m_pA3DDevice);

    if (!pGFX->Load(m_pA3DDevice, szFileName))
    {
        A3DRELEASE(pGFX);
    }
    else
    {
        pGFX->SetPos(vPos * m_matTransform);
        pGFX->SetDirAndUp(m_vDir, m_vUp);
        pGFX->SetScale(fScale);
        pGFX->Start();
        pGFX->TickAnimation(0);
    }

    Attachment* pAtt = new Attachment;
    pAtt->strFileName = szFileName;
    pAtt->pGFX = pGFX;
    pAtt->vPos = vPos;
    pAtt->fScale = fScale;
    m_aAttachment.push_back(pAtt);
    return pGFX != NULL;
}

int A3DDistantBoard::GetAttachCount() const
{
    return (int) m_aAttachment.size();
}

A3DDistantBoard::Attachment* A3DDistantBoard::GetAttach(int i) const
{
    return m_aAttachment[i];
}

void A3DDistantBoard::RemoveAttach(int i)
{
    delete m_aAttachment[i];
    m_aAttachment.erase(m_aAttachment.begin() + i);
}

void A3DDistantBoard::ClearAttach()
{
    for (size_t i = 0; i < m_aAttachment.size(); i++)
    {
        delete m_aAttachment[i];
    }
    m_aAttachment.clear();
}

