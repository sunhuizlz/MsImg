#include "stdafx.h"
#include "RC4_.h"




void rc4_setup( struct rc4_state *s, unsigned char *key,  int length );
void rc4_crypt( struct rc4_state *s, unsigned char *data, int length );

void rc4_setup( struct rc4_state *s, unsigned char *key,  int length )
{
	int i, j, k, *m, a;

	s->x = 0;
	s->y = 0;
	m = s->m;

	for( i = 0; i < 256; i++ )
	{
		m[i] = i;
	}

	j = k = 0;

	for( i = 0; i < 256; i++ )
	{
		a = m[i];
		j = (unsigned char) ( j + a + key[k] );
		m[i] = m[j]; m[j] = a;
		if( ++k >= length ) k = 0;
	}
}

void rc4_crypt( struct rc4_state *s, unsigned char *data, int length )
{ 
	int i, x, y, *m, a, b;

	x = s->x;
	y = s->y;
	m = s->m;

	for( i = 0; i < length; i++ )
	{
		x = (unsigned char) ( x + 1 ); a = m[x];
		y = (unsigned char) ( y + a );
		m[x] = b = m[y];
		m[y] = a;
		data[i] ^= m[(unsigned char) ( a + b )];
	}

	s->x = x;
	s->y = y;
}


CRC4_::CRC4_(void)
{
}

CRC4_::~CRC4_(void)
{
}

HRESULT CRC4_::put_Key(CString strKey)
{
	// TODO: Add your implementation code here

	int iKey = 0;
	BYTE *rc4_key = NULL;

	iKey = String2Byte(strKey,&rc4_key);

	memset(&rc4_data,0,sizeof(rc4_data));
	rc4_setup(&rc4_data,rc4_key,iKey);

	delete rc4_key;
	return S_OK;
}

CString CRC4_::_Crypt(BYTE* pByte, int iLength)
{
	rc4_crypt(&rc4_data,(unsigned char*)pByte,iLength);

	CString str;
	Byte2String(pByte,iLength,str);
	return str;
}

int CRC4_::String2Byte(LPCTSTR lpStr, BYTE** ppArr)
{
	int iLength = _tcslen(lpStr)/2;

	BYTE* pArr = new BYTE[iLength+1];
	*ppArr = pArr;
	pArr[iLength] = 0;
	int iNum = 0;

	for(int i = 0; i<iLength; i++)
	{
		_stscanf(&lpStr[i*2],_T("%02x"),&iNum);
		pArr[i] = iNum;
	}
	return iLength;
}

int CRC4_::Byte2String(BYTE* pAcc,int iLength,CString& strText)
{
	strText = _T("");

	for(int i = 0; i<iLength; i++)
	{
		CString str;
		str.Format(_T("%02X"),pAcc[i]);
		strText += str;
	}
	return strText.GetLength();
}

CString CRC4_::Crypt(CString strText)
{
	CString str(strText);
	USES_CONVERSION;
	char* pText = T2A(str);

	CString strC = _Crypt((BYTE*)pText,strlen(pText));

	//*strCipher = strC.AllocSysString();
	return strC;
}

CString CRC4_::Crypt2(CString strText)
{
	CString str = strText;
	BYTE *pByte = NULL;
	int iLen = String2Byte(str,&pByte);

	CString strC = _Crypt(pByte,iLen);

	delete pByte;

	String2Byte(strC,&pByte);

	char* pChar = (char*)pByte;

	USES_CONVERSION;
	strC = CString(A2T(pChar));

	//*strCipher = strC.AllocSysString();

	delete pByte;
	return strC;
}