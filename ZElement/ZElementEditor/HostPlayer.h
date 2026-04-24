#pragma once

class CECModel;
class A3DViewport;

class CHostPlayer
{
public:
	CHostPlayer(void);
	~CHostPlayer(void);

	bool Init(char* strName = "Models\\NPCs\\¹ÖÎï\\ÍöÁé\\Å®ÂÞÉ²\\Å®ÂÞÉ².ecm" );
	void Release();

	bool Tick(int iDeltaTime=0);
	bool Render(A3DViewport* pA3DViewport);

	void SetStart(bool bStart=true);
	void DeltaCamDist(float fDelta);
private:
	float GetStandHeight(A3DVECTOR3 vPos);
	void ClipHostPos();		// ·ÀÖ¹³¬³ö±ß½ç
	CECModel	*m_pPlayer;
	A3DVECTOR3	m_vPos;

	bool		m_bStart;
	float		m_fCamDist;
};
