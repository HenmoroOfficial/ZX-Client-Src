/*
 * FILE: EncStr.h
 *
 * DESCRIPTION: cooperate with the pwpacker to remove the information text in the rdata section of the PE file
 *
 * CREATED BY: linzhehui, 2009
 *
 * HISTORY: Last updated 2009/05/22, use Micro and Class to facilitate the Calling replacement.
 * HISTORY: LZH  updated 2009/06/01, replace LPSTR with LPCSTR, LPWSTR with LPCWSTR to Generate compile errors when using in dangerous ways.
 */

// 常量字符串保护相关宏使用说明
// 用途：配合加壳或者外部处理工具，加密在编译后的二进制代码中出现指示性明文文本(通常是只读数据段)，不影响程序正常表现，加大程序调试分析难度。
// 性能：由于有额外解密还原代码，所以字符串还原会稍微增加额外CPU,但是算法不用很复杂可以起到很好保护效果，CPU开销基本可以忽略不记
// 编码：三种编码调用方式, _A 为ansi code, _W 为Unicode, _T 是与宏UNICODE 定义兼容的调用方式，例如TCHAR，以及我们程序里面 _AL宏出现的地方
// 调用：
// 1. 作为函数参数直接调用。程序中函数引用常量字符串的所有场合均可以使用。
//    PARAM_ENCSTR_A(const CHAR*) PARAM_ENCSTR_W(const WCHAR*) PARAM_ENCSTR_T(const TCHAR*) 作用域为函数结束。
//    部分字符串类的定义部分,由于实际调用构造函数,因此可以直接使用此组接口.
// 2. 作为局部或全局变量调用,作用域为变量作用域.
//    VAR_ENCSTR_A(xxx,"abcd"); 替换  char *xxx="abcd";
//    VAR_ENCSTR_W(xxx,"abcd"); 替换 wchar *xxx="abcd";
//    VAR_ENCSTR_T(xxx,"abcd"); 替换 TCHAR *xxx="abcd";  后续使用直接用变量 xxx
//    注意，此处如果是局部变量，该指针作用空间只是在局部变量的生命范围。如果需要全局使用，请用全局变量或STATIC_VAR方式
// 3. 静态变量.作用域同全局函数
//    STATIC_VAR_ENCSTR_A(xxx,"abcd"); 替换  const static char  *xxx="abcd"; 
// 4. const 相关问题，类似字符串常量。尽量将其作为常量使用，但是为了调用时的方便对象同时提供 LPSTR，LPCSTR 两种操作方式。编译器检查可能会忽略部分WARNING。


#ifndef _ENCSTR_H_
#define _ENCSTR_H_


#define __ENC_STR_START      "S*4&"
#define __ENC_STR_END        "E*4&"
#define __ENC_STR_STARTX     "S*4&\0\1\2\3"
#define __ENC_STR_ENDX       "E*4&\0\1\2\3"  
#define __UENC_STR_START     L"S*4&"
#define __UENC_STR_END       L"E*4&"

#define __UENC_STR_STARTX    L"S*4&\0\1\2\3"
#define __UENC_STR_ENDX      L"E*4&\0\1\2\3"

#define __ATTACH_END_STR(X) X __ENC_STR_END 
#define __ENC_STR(X)   __ENC_STR_START __ATTACH_END_STR(X)

#define __UATTACH_END_STR(X) X __UENC_STR_END 
#define __UENC_STR(X)   __UENC_STR_START __UATTACH_END_STR(X)
#define MAX_FIND_LEN 1024


#include <time.h>
#include "..\EC_Global.h"


BOOL   EncryptStr(char* pStr);
char*  DecryptStr(const char* pStr);
void   FreeDecryptStr(char* NewStr);
BOOL   EncryptStrW(WCHAR* pStr);
WCHAR* DecryptStrW(const WCHAR* pStr);
void   FreeDecryptStrW(WCHAR* NewStr);


//class XEncryptStr
//{
//public:
//	XEncryptStr(const CHAR* str)
//	{
//		m_str = DecryptStr(str);
//		m_wcs = NULL;
//	}
//	XEncryptStr(const WCHAR* wcs)
//	{
//		m_wcs = DecryptStrW(wcs);
//		m_str = NULL;
//	}
//	virtual ~XEncryptStr()
//	{
//		if(m_str)
//			FreeDecryptStr(m_str);
//		if(m_wcs)
//			FreeDecryptStrW(m_wcs);
//	}
//	operator LPSTR()
//	{
//		return m_str;
//	}
//	operator LPWSTR()
//	{
//		return m_wcs;
//	}
//private:
//	CHAR  *m_str;
//	WCHAR *m_wcs;
// };


class XEncryptStrA
{
public:
	XEncryptStrA(const CHAR* str)
	{
		m_str = NULL;
		m_str = DecryptStr(str);
	}
	virtual ~XEncryptStrA()
	{
		if(m_str)
			FreeDecryptStr(m_str);
		m_str = NULL;
	}
	/*
	operator LPSTR()
	{
		return m_str;
	}
	*/
	operator LPCSTR()
	{
		return (const CHAR*)m_str;
	}
	const CHAR* operator =(const CHAR* str)
	{
		if(m_str)
		{
			FreeDecryptStr(m_str);
			m_str = NULL;
		}
		m_str = DecryptStr(str);
	}
	/*
	operator =(CHAR* str)
	{
		if(m_str)
		{
			FreeDecryptStr(m_str);
			m_str = NULL;
		}
		m_str = DecryptStr(str);
	}
	*/

private:
	CHAR  *m_str;
};

class XEncryptStrW
{
public:
	XEncryptStrW(const WCHAR* wcs)
	{
		m_wcs = NULL;
		m_wcs = DecryptStrW(wcs);
	}
	virtual ~XEncryptStrW()
	{
		if(m_wcs)
			FreeDecryptStrW(m_wcs);
		m_wcs = NULL;
	}
	/*
	operator LPWSTR()
	{
		return m_wcs;
	}
	*/
	operator LPCWSTR()
	{
		return (const WCHAR*)m_wcs;
	}
	const WCHAR* operator =(const WCHAR* wcs)
	{
		if(m_wcs)
		{
			FreeDecryptStrW(m_wcs);
			m_wcs = NULL;
		}
		m_wcs = DecryptStrW(wcs);
	}
	/*
	operator =(WCHAR* wcs)
	{
		if(m_wcs)
		{
			FreeDecryptStrW(m_wcs);
			m_wcs = NULL;
		}
		m_wcs = DecryptStrW(wcs);
	}
	*/
private:
	WCHAR  *m_wcs;
};


#if defined(_ELEMENTCLIENT)
	// encrypt ansi string.
	#define _EA(a)   LPCSTR(XEncryptStrA(__ENC_STR(a)))
	// encrypt unicode string.
	#define _EW(a)   LPCWSTR(XEncryptStrW(__UENC_STR(L##a)))
#else
	// encrypt ansi string.
	#define _EA(a)   LPCSTR(a)	
	// encrypt unicode string.	
	#define _EW(a)   LPCWSTR(L##a)
#endif

#ifdef UNICODE
// encrypt ansi or unicode string.
#define _ET _EW	
#else
// encrypt ansi or unicode string.
#define _ET _EA 
#endif


//////////////////////////////////////////////////////////////////////////
//       程序调用的宏接口部分, 用于修改老代码
//////////////////////////////////////////////////////////////////////////

//#define PARAM_ENCSTR_A(a)    LPCSTR(XEncryptStrA( __ENC_STR(a)))
//#define PARAM_ENCSTR_W(a)   LPCWSTR(XEncryptStrW(__UENC_STR(a)))
//
//#ifdef UNICODE
//#define PARAM_ENCSTR_T PARAM_ENCSTR_W
//#else
//#define PARAM_ENCSTR_T PARAM_ENCSTR_A
// #endif

//#define VAR_ENCSTR_A(a,b) XEncryptStrA  a( __ENC_STR(b))
//#define VAR_ENCSTR_W(a,b) XEncryptStrW  a(__UENC_STR(b))
//
//#ifdef UNICODE
//#define VAR_ENCSTR_T VAR_ENCSTR_W
//#else
//#define VAR_ENCSTR_T VAR_ENCSTR_A
//#endif
//
//#define STATIC_VAR_ENCSTR_A(a,b) static XEncryptStrA  a(__ENC_STR(b))
//#define STATIC_VAR_ENCSTR_W(a,b) static XEncryptStrW a(__UENC_STR(b))
//
//#ifdef UNICODE
//#define STATIC_VAR_ENCSTR_T STATIC_VAR_ENCSTR_W
//#else
//#define STATIC_VAR_ENCSTR_T STATIC_VAR_ENCSTR_A
//#endif
//
////////////////////////////////////////////////////////////////////////////
////        字符串数组接口部分,和直接变量定义,用于直接定义和字符串数组修改
////////////////////////////////////////////////////////////////////////////
//#ifdef UNICODE
//#define XEncryptStrT XEncryptStrW
//#define ENC_STR_T    ENC_STR_W
//#else
//#define XEncryptStrT XEncryptStrA
//#define ENC_STR_T    ENC_STR_A
//#endif
//
///*
//    老代码
//	char *buf[2] ={"abcd", "cdef")}
//	int c =0;
//	c= c*c;
//	MessageBox(buf[0],buf[1]),MB_OK);
//
//	新加密代码
//	XEncryptStrA buf[2] ={ENC_STR_A("abcd"), ENC_STR_A("cdef") }
//	int c =0;
//	c= c*c;
//	MessageBox(buf[0],buf[1],MB_OK);
//
//*/
//
//
//
//#define ENC_STR_A     __ENC_STR
//#define ENC_STR_W     __UENC_STR
//#define POINTER_ENCSTR_A(a)    LPSTR(XEncryptStrA(a))
//#define POINTER_ENCSTR_W(a)   LPWSTR(XEncryptStrW(a))
//
//#ifdef UNICODE
//#define ENC_STR_T ENC_STR_W
//#define POINTER_ENCSTR_T POINTER_ENCSTR_W
//#else
//#define ENC_STR_T ENC_STR_A
//#define POINTER_ENCSTR_T POINTER_ENCSTR_A
//#endif
//
//
//
//#endif
//
//
//
//
////////////////////////////////////////////////////////////////////////////
////                        下面老版本作废
////////////////////////////////////////////////////////////////////////////
////
////
//// 	一、一般使用情况，定义时在字符串前加上 NewEncStrT (NewEncStrA, NewEncStrW)，引用时采用下面的标准方式。
//// 	TCHAR *x = "Demo";
//// 	-------------------
//// 	TCHAR *x = NewEncStrT("Demo");
//// 	... //other code
//// 	DeleteEncStrT(x);
//// 	二、常用定参函数
//// 	参考Xglb_ErrorOutput 的定义方式直接定义相关函数的替代宏，然后调用的时候直接将函数名替换成宏名称。
//// 	三、变参函数(VC6 不支持变参宏)
//// 	1. 参考Xa_LogOutput 函数的定义，调用时 如 CHAR* p=NewEncStrA(szMsg); Xa_LogOutput(ilevel,p,...);
//// 	2. 根据参数个数写若干个替代宏，调用时根据参数个数替换不同的宏名称。  Xa_LogOutput_P02  到  Xa_LogOutput_P10
//// 	四、字符串类的初试化拷贝赋值函数用 CopyAssignMentA，CopyAssignMenW，CopyAssignMentT 替代
//
//
//
////void   Xa_LogOutput(int iLevel, const char* szMsg, ...); //Xa_LogOutput(ilevel,,...);
////最基本函数使用方式宏, 需要自己在不使用时释放内存
//
//#define NewEncStrA(a)    DecryptStr(__ENC_STR(a))
//#define DeleteEncStrA(a) FreeDecryptStr(a)
//
//#define NewEncStrW(a)	 DecryptStrW(__UENC_STR(a))
//#define DeleteEncStrW(a) FreeDecryptStrW(a) 
//
//// 支持UNICODE 宏定义的方式
//#ifdef  UNICODE
//#define NewEncStrT(a)    NewEncStrW(a)
//#define DeleteEncStrT(a) DeleteEncStrW(a)
//#else
//#define NewEncStrT(a)    NewEncStrA(a)
//#define DeleteEncStrT(a) DeleteEncStrA(a)
//#endif
//
///*
//// 赋值操作符号，用于类似于 CString 类的初始化
//#define CopyAssignMentA(a,b) {\
//	CHAR *p1 = NewEncStrA(b); \
//	a = p1; \
//	DeleteEncStrA(p1); \
//} \
//
//#define CopyAssignMentW(a,b) {\
//	WCHAR *p1 = NewEncStrW(b); \
//	a = p1; \
//	DeleteEncStrW(p1); \
//} \
//
//#define CopyAssignMentT(a,b) {\
//	TCHAR *p1 = NewEncStrT(b); \
//	a = p1; \
//	DeleteEncStrT(p1); \
//} \
//
//
//// 修改了一些宏，方便修改原有的一些函数调用
//#define XRuntimeDebugInfoW_P03(a,b,c) { \
//	WCHAR *p1 = NewEncStrW(c); \
//	if(p1!=NULL) \
//	{ \
//	a->RuntimeDebugInfo(b, p1); \
//	DeleteEncStrW(p1); \
//	} \
//else{}\
//} \
//
//#define XRuntimeDebugInfoA_P03(a,b,c) { \
//	CHAR *p1 = NewEncStrA(c); \
//	if(p1!=NULL) \
//	{ \
//	a->RuntimeDebugInfo(b, p1); \
//	DeleteEncStrW(p1); \
//	} \
//else{}\
//} \
//
//
//#define XRuntimeDebugInfoW_P04(a,b,c,d) { \
//	WCHAR *p1 = NewEncStrW(c); \
//	if(p1!=NULL) \
//	{ \
//	a->RuntimeDebugInfo(b, p1, d); \
//	DeleteEncStrW(p1); \
//	} \
//else{}\
//} \
//
//#define XRuntimeDebugInfoA_P04(a,b,c,d) { \
//	CHAR *p1 = NewEncStrA(c); \
//	if(p1!=NULL) \
//	{ \
//	a->RuntimeDebugInfo(b, p1, d); \
//	DeleteEncStrW(p1); \
//	} \
//else{}\
//} \
//
////g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("NET - Server disconnected"));
//#ifdef  UNICODE
//#define XRuntimeDebugInfoT_P03    XRuntimeDebugInfoW_P03
//#define XRuntimeDebugInfoT_P04    XRuntimeDebugInfoW_P04
//#else
//#define XRuntimeDebugInfoT_P03    XRuntimeDebugInfoA_P03
//#define XRuntimeDebugInfoT_P04    XRuntimeDebugInfoA_P04
//#endif
//*/
//
//
//#define XOutputDebugStringW(a){ \
//	WCHAR *p1 = NewEncStrW(a); \
//	OutputDebugStringW(a); \
//	DeleteEncStrW(p1); \
//} \
//
//#define XOutputDebugStringA(a){ \
//	CHAR *p1 = NewEncStrA(a); \
//	OutputDebugString(a); \
//	DeleteEncStrA(p1); \
//} \
//
//#ifdef  UNICODE
//#define XOutputDebugStringT(a)    XOutputDebugStringW(a)
//#else
//#define XOutputDebugStringT(a)    XOutputDebugStringA(a
//#endif
//
//
//
//#define Xglb_ErrorOutput(a,b,c) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//	{ \
//		glb_ErrorOutput(a, p1, c); \
//		DeleteEncStrA(p1); \
//	} \
//	else{}\
//} \
//
//#define Xa_LogOutput(a,b) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//	{ \
//		a_LogOutput(a, p1); \
//		DeleteEncStrA(p1); \
//	} \
//	else{}\
//} \
//	
//#define Xa_LogOutput_P02(a,b) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//	{ \
//		a_LogOutput(a, p1); \
//		DeleteEncStrA(p1); \
//	} \
//	else{}\
//} \
//  
//#define Xa_LogOutput_P03(a,b,c) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//	{ \
//	a_LogOutput(a, p1,c); \
//	DeleteEncStrA(p1); \
//	} \
//	else{}\
//} \
//
//#define Xa_LogOutput_P04(a,b,c,d) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//
//#define Xa_LogOutput_P05(a,b,c,d,e) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//
//#define Xa_LogOutput_P06(a,b,c,d,e,f) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e,f); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//
//#define Xa_LogOutput_P07(a,b,c,d,e,f,g) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e,f,g); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//
//#define Xa_LogOutput_P08(a,b,c,d,e,f,g ,h) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e,f,g,h); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//
//
//#define Xa_LogOutput_P09(a,b,c,d,e,f,g ,h,i) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e,f,g,h,i); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//	
//#define Xa_LogOutput_P10(a,b,c,d,e,f,g ,h,i,j) { \
//	CHAR *p1 = NewEncStrA(b); \
//	if(p1!=NULL) \
//{ \
//	a_LogOutput(a, p1,c,d,e,f,g,h,i,j); \
//	DeleteEncStrA(p1); \
//} \
//else{}\
//} \
//

#endif
