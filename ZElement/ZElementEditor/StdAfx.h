// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__663AA252_D1E5_4299_B38E_AEB072197D3F__INCLUDED_)
#define AFX_STDAFX_H__663AA252_D1E5_4299_B38E_AEB072197D3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#ifdef _PLATFORM_RENDER
#define CELGRASSLAND       A3DGrassLand
#define CELTERRAINOUTLINE2 A3DTerrainOutline2
#define CECSUNMOON         A3DSunMoon
#define CELGRASSTYPE       A3DGrassType
#define CELOUTLINEQUADNODE A3DOutlineQuadNode
#define CECManager         ECManager
#define CECViewport        ECViewport
#else
#define CELGRASSLAND       CELGrassLand
#define CELTERRAINOUTLINE2 CELTerrainOutline2
#define CECSUNMOON         CECSunMoon
#define CELGRASSTYPE       CELGrassType
#define CELOUTLINEQUADNODE CELOutlineQuadNode
#define ECViewport         CECViewport
#endif // _PLATFORM_RENDER
#endif // !defined(AFX_STDAFX_H__663AA252_D1E5_4299_B38E_AEB072197D3F__INCLUDED_)
