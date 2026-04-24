
#include "StdAfx.h"
#include "encstr.h"
BOOL EncryptStr(char* pStr)
{
	//在原文件中查找字符串并加密
	static BOOL bInitRand = FALSE;
	if(!bInitRand)
	{
		srand( (unsigned)time( NULL ));
	}
	__try
	{
		if(pStr!=NULL)
		{
			if(memcmp(__ENC_STR_STARTX,pStr,sizeof(__ENC_STR_START)-1)==0)
			{
				char *pnew = pStr+ sizeof(__ENC_STR_START)-1;

				INT32 i;
				for(i=0;i<MAX_FIND_LEN;i++)
				{
					if(memcmp(__ENC_STR_ENDX, pStr+i+sizeof(__ENC_STR_START)-1,sizeof(__ENC_STR_END)-1)==0)
						break;
				}
				
				if(i<MAX_FIND_LEN)
				{
					//memcpy(pStr+sizeof(INT32),&i,sizeof(INT32));
					//
					DWORD dwOldProtect;
					VirtualProtectEx(GetCurrentProcess(),pStr,i+sizeof(__ENC_STR_START)-1+sizeof(__ENC_STR_END)-1,PAGE_READWRITE,&dwOldProtect);

					memmove(pStr+sizeof(__ENC_STR_START)-1+sizeof(__ENC_STR_END)-1,pStr+sizeof(__ENC_STR_START)-1,i);
					memcpy(pStr,&i,sizeof(INT32));
					INT32 dwSeed = ((INT32)rand()<<8)+rand();
					memcpy(pStr+sizeof(INT32),&dwSeed,sizeof(INT32));

					BYTE ch=0;
					BYTE ch1=0;
					char * pnewstr= pStr+sizeof(__ENC_STR_START)-1+sizeof(__ENC_STR_END)-1;
					for(int j=0;j<i;j++)
					{
						int m =j % 4;
						ch1 = dwSeed>>(m*8);
						pnewstr[j] = ~pnewstr[j];
						pnewstr[j] = pnewstr[j]^ch^ch1;
						ch = ch+pnewstr[j];
					}
					VirtualProtectEx(GetCurrentProcess(),pStr,i+sizeof(__ENC_STR_START)-1+sizeof(__ENC_STR_END)-1,dwOldProtect,NULL);
					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
			return FALSE; //TOO LONG FOR THE STRING
		}

	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return TRUE;
	}
	return FALSE;

}

char*  DecryptStr(const char* pStr)
{
	char* pNewStr =NULL;
	__try
	{
		if(pStr!=NULL)
		{
			INT32 dwSeed = 0;
			INT32 dwLen = 0;
			if((pStr[2]<0x04)&&(pStr[3] ==0))
			{
				memcpy(&dwLen, pStr,sizeof(INT32));
				memcpy(&dwSeed,pStr+sizeof(INT32),sizeof(INT32));
				pNewStr = new char[dwLen+1];

				if(pNewStr)
				{
					memcpy(pNewStr,pStr+sizeof(__ENC_STR_START)-1+sizeof(__ENC_STR_END)-1,dwLen);
					pNewStr[dwLen] =0;
				}
				
				BYTE ch=0;
				BYTE ch1=0;

				for(int j=0;j<dwLen;j++)
				{
					int m=j%4;
					ch1=dwSeed>>(m*8);
					ch1=ch^ch1;
					ch=ch+pNewStr[j];
					pNewStr[j]=pNewStr[j]^ch1;
					pNewStr[j]=~pNewStr[j];	
				}
			}
			else
			{
				// 对于没有加壳的情形
				if(memcmp(__ENC_STR_STARTX,pStr,sizeof(__ENC_STR_START)-1)==0)
				{
					//const char *pnew = pStr+sizeof(__ENC_STR_START)-1;
					INT32 i;
					for(i=0;i<MAX_FIND_LEN;i++)
					{
						if(memcmp(__ENC_STR_ENDX,pStr+i+sizeof(__ENC_STR_START)-1,sizeof(__ENC_STR_END)-1)==0)
							break;
					}
					if(i<MAX_FIND_LEN)
					{
						pNewStr = new char[i+1];
						if(pNewStr)
						{
							memcpy(pNewStr,pStr+sizeof(__ENC_STR_START)-1,i);
							pNewStr[i] = 0;
						}
						
					}
				}
				else //未加密字符串，而且没有标记
				{
					INT32 i= strlen(pStr);
					if(i<MAX_FIND_LEN)
					{
						pNewStr = new char[i+1];
						if(pNewStr)
						{
							memcpy(pNewStr,pStr,i);
							pNewStr[i] = 0;
						}
					}
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if(pNewStr)
			delete[] pNewStr;
		return NULL;
	}
	return pNewStr;
}

void FreeDecryptStr(char* NewStr)
{
	if(NewStr)
		delete[] NewStr;
}

BOOL EncryptStrW(WCHAR* pStr)
{
	//在原文件中查找字符串并加密
	static BOOL bInitRand = FALSE;
	if(!bInitRand)
	{
		srand( (unsigned)time( NULL ));
	}
	__try
	{
		if(pStr!=NULL)
		{
			if(memcmp(__UENC_STR_STARTX,pStr,sizeof(__UENC_STR_START)-sizeof(WCHAR))==0)
			{
				//WCHAR *pnew = pStr+ sizeof(__UENC_STR_START)/sizeof(WCHAR)-1;

				INT32 i;
				for(i=0;i<MAX_FIND_LEN;i++)
				{
					if(memcmp(__UENC_STR_ENDX, pStr+i+sizeof(__UENC_STR_START)/sizeof(WCHAR)-1,sizeof(__UENC_STR_END)-sizeof(WCHAR))==0)
						break;
				}
				
				if(i<MAX_FIND_LEN)
				{
					//memcpy(pStr+sizeof(INT32),&i,sizeof(INT32));
					//
					DWORD dwOldProtect;
					VirtualProtectEx(GetCurrentProcess(),pStr,i*sizeof(WCHAR)+sizeof(__UENC_STR_START)-sizeof(WCHAR)+sizeof(__UENC_STR_END)-sizeof(WCHAR),PAGE_READWRITE,&dwOldProtect);

					memmove(pStr+sizeof(__UENC_STR_START)/sizeof(WCHAR)-1+sizeof(__UENC_STR_END)/sizeof(WCHAR)-1,pStr+sizeof(__UENC_STR_START)/sizeof(WCHAR)-1,i*sizeof(WCHAR));
				
					INT32 tmp =0;
					memcpy(pStr,&tmp,sizeof(INT32));
					INT32 dwSeed = ((INT32)rand()<<8)+rand();
					memcpy(pStr+sizeof(INT32)/sizeof(WCHAR),&tmp,sizeof(INT32));

					memcpy(pStr+sizeof(INT32)/sizeof(WCHAR)*2,&i,sizeof(INT32));
					memcpy(pStr+sizeof(INT32)/sizeof(WCHAR)*3,&dwSeed,sizeof(INT32));

					CHAR ch=0;
					CHAR ch1=0;
					CHAR * pnewstr= (char *)(pStr+sizeof(__UENC_STR_START)/sizeof(WCHAR)-1+sizeof(__UENC_STR_END)/sizeof(WCHAR)-1);

					for(unsigned int j=0;j<i*sizeof(WCHAR);j++)
					{
						unsigned int m =j % 4;
						ch1 = dwSeed>>(m*8);
						pnewstr[j] = ~pnewstr[j];
						pnewstr[j] = pnewstr[j]^ch^ch1;
						ch = ch+pnewstr[j];
					}
					VirtualProtectEx(GetCurrentProcess(),pStr,i*sizeof(WCHAR)+sizeof(__UENC_STR_START)-sizeof(WCHAR)+sizeof(__ENC_STR_END)-sizeof(WCHAR),dwOldProtect,NULL);
					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
			return FALSE; //TOO LONG FOR THE STRING
		}

	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return TRUE;
	}
	return FALSE;
}

WCHAR*  DecryptStrW(const WCHAR* pStrX)
{
	WCHAR* pNewStrX =NULL;
	char * pNewStr =NULL;
	CHAR*  pStr =(char *)pStrX;
	__try
	{
		if(pStr!=NULL)
		{
			INT32 dwSeed = 0;
			INT32 dwLen = 0;
			
			if((pStr[0]==0)&&(pStr[1] ==0)&&(pStr[2] ==0)&&(pStr[3] ==0)&&(pStr[4]==0)&&(pStr[5] ==0)&&(pStr[6] ==0)&&(pStr[7] ==0)&&(pStr[10] <0x04)&&(pStr[11] ==0))
			{
				memcpy(&dwLen, pStr+sizeof(INT32)*2,sizeof(INT32));
				memcpy(&dwSeed,pStr+sizeof(INT32)*3,sizeof(INT32));

				pNewStrX = new WCHAR[dwLen+1];

				if(pNewStrX)
				{
					memcpy(pNewStrX,pStr+sizeof(__UENC_STR_START)-sizeof(WCHAR)+sizeof(__UENC_STR_END)-sizeof(WCHAR),dwLen*sizeof(WCHAR));
					pNewStrX[dwLen] =0;
				
					pNewStr = (char *)pNewStrX;
					BYTE ch=0;
					BYTE ch1=0;
					
					for(int j=0;j<dwLen*2;j++)
					{
						int m=j%4;
						ch1=dwSeed>>(m*8);
						ch1=ch^ch1;
						ch=ch+pNewStr[j];
						pNewStr[j]=pNewStr[j]^ch1;
						pNewStr[j]=~pNewStr[j];	
					}
				}
			}
			else
			{
				// 对于没有加壳的情形
				if(memcmp(__UENC_STR_STARTX,pStr,sizeof(__UENC_STR_START)-sizeof(WCHAR))==0)
				{
					//char *pnew = pStr+sizeof(__UENC_STR_START)-sizeof(WCHAR);
				
					INT32 i;
					for(i=0;i<MAX_FIND_LEN;i+=2)
					{
						if(memcmp(__UENC_STR_ENDX,pStr+i+sizeof(__UENC_STR_START)-sizeof(WCHAR),sizeof(__UENC_STR_END)-sizeof(WCHAR))==0)
							break;
					}
					if(i<MAX_FIND_LEN)
					{
						
						WCHAR *tmpchar = new WCHAR[i/2+1];
						if(tmpchar)
						{
							tmpchar[i/2] = 0;
							pNewStr = (char *)tmpchar;
							memcpy(pNewStr,pStr+sizeof(__UENC_STR_START)-sizeof(WCHAR),i);
							//pNewStr[i] = 0;
						}
						
					}
				}
				else //未加密字符串，而且没有标记
				{
					INT32 i= wcslen(pStrX);
					if(i<MAX_FIND_LEN)
					{
						pNewStrX = new WCHAR[i+1];
						if(pNewStr)
						{
							wcscpy(pNewStrX,pStrX);
							pNewStrX[i] = 0;
							pNewStr =(CHAR *)pNewStrX;
						}
					}
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if(pNewStr)
			delete[] pNewStr;
		return NULL;
	}
	return (WCHAR *)pNewStr;
}

VOID FreeDecryptStrW(WCHAR* NewStr)
{
	if(NewStr)
		delete[] NewStr;
}



/*  用类的析购来解决内存释放问题，简化调用，去除所有此类X函数
void Xa_LogOutput(int iLevel, const char* szMsg, ...)
{
	//char* p= __ENC_STR(x);  printf(p1); 
	va_list vaList;
	va_start(vaList, szMsg);
	char *p1 = DecryptStr(szMsg);
	if(p1!=NULL)
	{
		a_LogOutput(iLevel, p1, vaList);
		FreeDecryptStr(p1);
	}
	else
	{
		//a_LogOutput(iLevel, szMsg, vaList);
		//Error Handling
	}
	va_end(vaList);	
}
*/
