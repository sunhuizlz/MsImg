// version.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "version.h"
#include "shellapi.h"
#include "Markup.h"
#include "RC4_.h"
#include "mscoree.h"
#include "../CommonFile/TangramXmlParse.h"
#include "metahost.h"
//#include "fusion.h"

typedef HRESULT (__stdcall *TangramClrCreate)(LPCWSTR pTypeName, REFIID riid, void **ppObject);
typedef HRESULT (__stdcall *TangramCLRConnector)(LPCWSTR , LPCWSTR , DWORD , REFCLSID , REFIID , LPVOID FAR *);
//typedef HRESULT (__stdcall *CreateAsmCache)(IAssemblyCache	**ppAsmCache, DWORD dwReserved);
typedef HRESULT (__stdcall *TangramLoadLibraryShim)(LPCWSTR szDllName, LPCWSTR szVersion, LPVOID pvReserved, HMODULE* phModDll);
typedef HRESULT (__stdcall *TangramCLRCreateInstance)(REFCLSID clsid, REFIID riid, /*iid_is(riid)*/ LPVOID *ppInterface);

TCHAR g_szOrgDll[MAX_PATH];
CString m_strCurVer = _T("");

CLoadDll::CLoadDll()
{
	m_hTangramModule = nullptr;
}

#include <shlobj.h>

BOOL CLoadDll::LoadExtendLibrary()
{
	TCHAR cPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,cPath,MAX_PATH);
	CString strPath = cPath;
	int nPos = strPath.ReverseFind('\\');
	CString strName = strPath.Mid(nPos+1);
	if(strName.CompareNoCase(_T("lync.exe")))
		return false;
	TCHAR szPath[MAX_PATH] = {0};
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, szPath);
	strPath = szPath;
	strPath += _T("\\Tangram\\Tangram.Dll");
	m_hTangramModule = LoadLibrary(strPath);
	return true;
}

BOOL CLoadDll::FreeExtendLibrary()
{
	if(m_hTangramModule)
		::FreeLibrary(m_hTangramModule);
	return true;
}

extern TCHAR g_szOrgDll[MAX_PATH];
extern CLoadDll g_LoadDll;
