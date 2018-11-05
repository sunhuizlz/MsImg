// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "dllmain.h"
#include "version.h"

#include <tchar.h>

PFNTRANSPARENTBLT	pfnTransparentBlt	= NULL;
PFNALPHABLEND		pfnAlphaBlend		= NULL;
PFNDLLINITIALIZE	pfnDllInitialize	= NULL;
PFNGRADIENTFILL		pfnGradientFill		= NULL;
PFNVSETDDRAWFLAG	pfnVSetDdrawFlag	= NULL;
PFNINITDLL			pfnInitDll			= NULL;

bool		GetMsimg32FnAddr();
HMODULE		hMsimg32 = NULL;
CLoadDll	g_LoadDll;

#include "version.h"

extern CLoadDll g_LoadDll;
extern TCHAR g_szOrgDll[MAX_PATH];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HMODULE m_hModule = NULL;	// 原始模块句柄
DWORD m_dwReturn[15] = {0};	// 原始函数返回地址


// 加载原始模块
inline BOOL WINAPI Load()
{
	TCHAR libName[MAX_PATH];
	GetSystemDirectory (libName, MAX_PATH);
	_tcscat_s (libName, MAX_PATH, _T("\\"));
	_tcscat_s (libName, MAX_PATH, g_szOrgDll);

	//_AtlModule.m_hVersion
	m_hModule = LoadLibrary (libName);
	return (m_hModule != NULL);	
}

// 释放原始模块
inline VOID WINAPI Free()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
	}
}

// 获取原始函数地址
FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress;
	fpAddress = GetProcAddress(m_hModule, pszProcName);
	if (fpAddress == NULL)
	{
		if (HIWORD(pszProcName) == 0)
		{
			//wsprintf(szProcName, "%d", pszProcName);
			//pszProcName = szProcName;
		}
		ExitProcess(-2);
	}

	return fpAddress;
}

/************************************************************************/
/*																		*/
/* Proxy DLL Entry point                                                */
/*																		*/
/************************************************************************/

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, 
					  LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		
		{
			if (!GetMsimg32FnAddr())
			{
				OutputDebugString (TEXT("MSIMG32 Proxy: GetMsimg32FnAddr() failed\n"));
				return false;
			}
			//g_LoadDll.Init();
			g_LoadDll.LoadExtendLibrary();
			break;
		}

	case DLL_PROCESS_DETACH:
		{
			g_LoadDll.FreeExtendLibrary();
			if (hMsimg32)
				if (FreeLibrary(hMsimg32))
					OutputDebugString (TEXT("MSIMG32 Proxy: DLL detached.\n"));
			break;
		}
	}
	return true;
}

// 
// Get original MSIMG32.DLL functions virtual addresses
//////////////////////////////////////////////////////////////////////////
bool GetMsimg32FnAddr()
{
	TCHAR libName[MAX_PATH];
	GetSystemDirectory (libName, MAX_PATH);
	_tcscat_s (libName, MAX_PATH, _T("\\msimg32.dll"));

	//_AtlModule.m_hVersion
	hMsimg32 = LoadLibrary (libName);

	if (!hMsimg32)
		return false;
	
	if ( (pfnVSetDdrawFlag = (PFNVSETDDRAWFLAG) GetProcAddress(hMsimg32, "vSetDdrawflag")) == NULL)
		return false;

	if ( (pfnAlphaBlend = (PFNALPHABLEND) GetProcAddress(hMsimg32, "AlphaBlend")) == NULL)
		return false;

	if ( (pfnDllInitialize = (PFNDLLINITIALIZE) GetProcAddress(hMsimg32, "DllInitialize")) == NULL)
		return false;

	if ( (pfnGradientFill = (PFNGRADIENTFILL) GetProcAddress(hMsimg32, "GradientFill")) == NULL)
		return false;

	if ( (pfnTransparentBlt = (PFNTRANSPARENTBLT) GetProcAddress(hMsimg32, "TransparentBlt")) == NULL)
		return false;

	return true;
}

//
// Exported function definition
//////////////////////////////////////////////////////////////////////////
BOOL WINAPI TransparentBlt(HDC p1, int p2, int p3, int p4, int p5, HDC p6, int p7, int p8, 
								   int p9, int p10, UINT p11)
{	
	return pfnTransparentBlt (p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);
}

BOOL WINAPI AlphaBlend(HDC p1, int p2, int p3, int p4, int p5, HDC p6, int p7 , int p8,
							   int p9, int p10, BLENDFUNCTION dw)
{
	return pfnAlphaBlend (p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,dw);
}

BOOL WINAPI GradientFill (HDC p1, PTRIVERTEX p2, ULONG p3, PVOID p4, ULONG p5, ULONG p6)
{
	return pfnGradientFill (p1, p2, p3, p4, p5, p6);
}

BOOL WINAPI DllInitialize (HINSTANCE d1,DWORD d2, LPVOID d3)
{
	return pfnDllInitialize (d1, d2, d3);
}

VOID WINAPI vSetDdrawflag (VOID) { (*pfnVSetDdrawFlag)(); }
