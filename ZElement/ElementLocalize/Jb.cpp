#include "stdafx.h"
#include "jb.h"

void j2b(TCHAR* szText, TCHAR* szBuffer)
{
	int n = _tcslen(szText); 
	int r=LCMapString(
		MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC),
		LCMAP_TRADITIONAL_CHINESE,
		szText,n,NULL,0);
	
	TCHAR *ft= new TCHAR[r+1];
	
	r=LCMapString(
		MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC),
		LCMAP_TRADITIONAL_CHINESE,
		szText,n,ft,r+1);//这个api搞掂简体转繁体,下面会打印繁体出来
	ft[r]=0; 
	
	if(r) 
	{
		char *ps= new char[2*r + 2];
		BOOL f=FALSE;
		int r1=WideCharToMultiByte(950,0,ft,r,ps,2*r + 2,"?",&f);//代码页切换搞掂gb->big5
		ps[r1]=0;
		int r2 = MultiByteToWideChar(950,0,ps,r1,ft,r);
		ft[r2]=0; 
		delete []ps;
	}
	lstrcpy(szBuffer,ft);
	if(ft) delete []ft;
	
}




