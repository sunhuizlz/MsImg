#pragma once
#include "atlstr.h"
struct rc4_state
{
	int x, y, m[256];
};

class CRC4_
{
public:
	CRC4_(void);
	~CRC4_(void);

	CString _Crypt(BYTE* pByte, int iLength);

	int String2Byte(LPCTSTR lpStr, BYTE** ppArr);
	int Byte2String(BYTE* pAcc,int iLength,CString& strText);

	struct rc4_state rc4_data;

	//STDMETHOD(put_Key)(BSTR newVal);
	//STDMETHOD(Crypt)(BSTR strText, BSTR* strCipher);
	//STDMETHOD(Crypt2)(BSTR strText, BSTR* strCipher)
	CString Crypt(CString strText);
	CString Crypt2(CString strText);

	HRESULT CRC4_::put_Key(CString strKey);
};
