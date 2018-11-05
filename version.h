#pragma once;

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VERSION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VERSION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef VERSION_EXPORTS
#define VERSION_API __declspec(dllexport)
#else
#define VERSION_API __declspec(dllimport)
#endif

//// This class is exported from the version.dll
//class VERSION_API Cversion {
//public:
//	Cversion(void);
//	// TODO: add your methods here.
//};
//
//extern VERSION_API int nversion;
//
//VERSION_API int fnversion(void);


#include "mscoree.h"
//#include "fusion.h"
#include "Markup.h"
#include "metahost.h"
class CMarkup;

#include <vector>
using namespace std;

typedef vector<HMODULE> CMoudles;

class CLoadDll
{
public:
	CLoadDll();

	BOOL LoadExtendLibrary();
	BOOL FreeExtendLibrary();
	HMODULE	m_hTangramModule;
};
