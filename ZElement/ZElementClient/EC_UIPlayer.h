/********************************************************************
  created:	   23/5/2006  
  filename:	   ECUIPlayer.h
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#pragma  once

#include "AUI\\AUIObject.h"

class CECLoginPlayer;
class A2DSprite;

class CECUIPlayer : public AUIObject  
{
public:
	CECUIPlayer();
	virtual ~CECUIPlayer();
	bool CreateDemoPlayer(int templateID);
	void SetAngle(int angle) { m_Angle = angle;};
	void UpdateEquips();
	bool Render();
	bool Tick();
	bool SetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);

	static bool RegisterPropertyType();
private:
	CECLoginPlayer * m_pDemoPlayer;
	int              m_TempPlayerId;  //template player id
	int              m_Angle;
	A2DSprite *		 m_pA2DSprite;          //background image

	bool             InitBkImg(const char * szFileName);
};



