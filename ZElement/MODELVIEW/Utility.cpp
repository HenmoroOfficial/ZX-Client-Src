/*
 * FILE: Utility.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: MaYuanzheng, 2010/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 ZhuXian Studio, All Rights Reserved.
 */


#include "A3DEngine.h"
#include "A3DTexture.h"
#include "AFI.h"
#include "A3DShader.h"
#include "A3DShaderMan.h"
#include "Utility.h"
#include "Render.h"


///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////
void glb_ReplaceShader(A3DSkin * pSkin)
{
	if( !pSkin )
		return;

	int i;
	for(i=0; i < pSkin->GetTextureNum(); i++)
	{
		A3DTexture* pTex = pSkin->GetTexture(i);
		const char* szTexMap = pTex->GetMapFile();
		bool bAlphaTex = pTex->IsAlphaTexture();

		if (szTexMap && szTexMap[0])
		{
			if( pTex->IsShaderTexture() )
			{
			}
			else
			{
				char szFileTitle[MAX_PATH];
				af_GetFileTitle(szTexMap, szFileTitle, MAX_PATH);
				strlwr(szFileTitle);

				if( strstr(szFileTitle, "a_") == szFileTitle )
				{
					A3DShader* pReplaceShader;
					pReplaceShader = g_Render.GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor1.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(true);
					}
					continue;
				}
				
				if( strstr(szFileTitle, "r_") == szFileTitle )
				{
					A3DShader* pReplaceShader;
					pReplaceShader = g_Render.GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(false);
					}
					continue;
				}

				if( strstr(szFileTitle, "zx_") == szFileTitle )
				{
					A3DShader* pReplaceShader = g_Render.GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor2.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						
						if( pReplaceShader->GetGeneralProps().pPShader )
						{
							char szTexMap2[MAX_PATH];
							af_GetFilePath(szTexMap, szTexMap2, MAX_PATH);
							strcat(szTexMap2, "\\nb_");
							strcat(szTexMap2, szFileTitle);
							pReplaceShader->ChangeStageTexture(1, A3DSDTEX_NORMAL, (DWORD)szTexMap2);
						}
						else
						{
							pReplaceShader->ShowStage(2, false);
						}

						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(true);
					}
				}
			}
		}
	}
}



/*	Change file's extension

	szFile (in, out): file name.
	szNewExt: new extension of file
*/
void glb_ChangeExtension(char* szFile, char* szNewExt)
{
#ifdef _DEBUG
	
	int iLen = strlen(szFile);
	if (iLen <= 4)
	{
		ASSERT(iLen > 4);
		return;
	}

#endif

	char* pTemp = strrchr(szFile, '.');
	if (!pTemp)
	{
		strcat(szFile, ".");
		strcat(szFile, szNewExt);
	}
	else
	{
		strcpy(pTemp+1, szNewExt);
	}
}
