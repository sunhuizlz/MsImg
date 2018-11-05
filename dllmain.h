#include <windows.h>
#pragma warning( disable:4273 ) // Silence compiler

//
// Function pointer typedefs for exports we are forwarding              
//
typedef BOOL (WINAPI *PFNTRANSPARENTBLT) (HDC,int,int,int,int,HDC,int,int,int,int,UINT);
typedef VOID (WINAPI *PFNVSETDDRAWFLAG)  (VOID);
typedef BOOL (WINAPI *PFNALPHABLEND)	(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
typedef BOOL (WINAPI *PFNGRADIENTFILL)	(HDC,PTRIVERTEX,ULONG,PVOID,ULONG,ULONG);
typedef BOOL (WINAPI *PFNDLLINITIALIZE)(HINSTANCE, DWORD, LPVOID);

// Function pointer typedef to injected-DLL initialization
typedef void (*PFNINITDLL) (void);	

#ifdef __cplusplus
extern "C" 
{
#endif 
	//
	// Function protoypes
	//
	BOOL WINAPI TransparentBlt(HDC, int, int, int, int, HDC, int, int, int, int, UINT);
	BOOL WINAPI AlphaBlend    (HDC, int , int, int, int, HDC, int , int, int, int, BLENDFUNCTION);
	BOOL WINAPI GradientFill  (HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
	BOOL WINAPI DllInitialize (HINSTANCE, DWORD, LPVOID);
	VOID WINAPI vSetDdrawflag (VOID);

#ifdef __cplusplus
}
#endif // __CPLUSPLUS