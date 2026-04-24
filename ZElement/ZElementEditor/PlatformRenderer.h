/*
 * FILE: PlatformRenderer.h
 *
 * DESCRIPTION: Render using A3DPlatform ElementRneder class
 *
 * CREATED BY: Duyuxin, 2004/6/8
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#pragma once
#ifdef _ANGELICA22
class CElementMap;
class ElementRender;
class A3DViewport;
class A3DSkinRenderBase;

class PlatformRenderer 
{
public:
    PlatformRenderer();
    ~PlatformRenderer();

    bool Init();
    void Release();

    bool Render(A3DViewport* pViewport, CElementMap* pMap);
    
    ElementRender* GetElementRender() const { return m_pElementRender; }
protected:
    CElementMap* m_pMap;
    ElementRender* m_pElementRender;
    A3DSkinRenderBase* m_pSkinRender2;

};     

#endif // #ifdef _ANGELICA22