//#include "StdAfx.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "Tea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

minTea::minTea(const char* szKey)
{
	setKey(szKey);
}

minTea::minTea(void)
{
	// Default Encrypt Key from SCV :-)
//	char* szKey = "Show me the money by SCV";
//	char* szKey = "Block Sheep Wall by Prob";
	char* szKey = "Steven Seagal Neck Break";

	setKey(szKey);
}

minTea::~minTea(void)
{
}

bool minTea::setKey(const char* szKey)
{
	// 2013.06.03 nodefeat. Key값이 16자면 되는데 >로 체크해서 >= 로 수정
	assert(strlen(szKey) >= TEA_KEY_LENGTH);

	memset(realKey, 0, TEA_KEY_LENGTH);

	UINT keyLen = (UINT)strlen(szKey);

	// Make the key TEA_KEY_LENGTH chars (128 bit) by repeating characters
	for (int n = 0; n < TEA_KEY_LENGTH; n++)
	{
		realKey[n] = szKey[n%keyLen];
	}

	return true;
}

void minTea::encrypt(char* szData, acCArray<char>& encryptedText, int& nMaxLength)
{
	int keyLen = nMaxLength;
	while (keyLen > 0)
	{
		if (szData[keyLen - 1] != 0)
			break;
		--keyLen;
	}

	acCArray<char> text;
	text.Allocate((int)keyLen + 1, false);
	text.SetLength((int)keyLen);

	memcpy(text.AddressOf(), szData, keyLen);

	// The text must be larger than 4 bytes
	while (text.GetLength() <= 4)
	{
		text.PushLast(0);
	}

	// Padd text with nulls until even dividable with 4
	while (text.GetLength() & 3)
	{
		text.PushLast(0);
		// 2013.05.31 nodefeat.
		// 패딩 후에 길이가 길어지는 것이 당연하지만 if (nEncryptedLength > nMaxLength)
		// 처리를 하고 있어서 4로 나누어 지지 않는 길이의 문자열은 암호화 되지 않는 문제가 있었음
		// 패딩 시 nMaxLength도 증가하도록 처리 함
		++nMaxLength;
	}

	// Encrypt the message
	encrypt((UINT*)text.AddressOf(),
		text.GetLength() / 4,
		(UINT*)realKey);

	// Store the encrypted text for later decryption
	encryptedText.SetLength(text.GetLength());
	memcpy(encryptedText.AddressOf(),
		text.AddressOf(),
		text.GetLength());

	encryptedText.PushLast(0);
}

void minTea::decrypt(char* szData, acCArray<char>& decryptedText, int& nMaxLength)
{
	// strlen으로는 문자열 길이 계산을 정확하게 할 수 없어서 수정
	/*
	// Get the encryption key from the text box
	UINT keyLen = (UINT) strlen (szData);

	// Retrieve the encrypted text for decryption
	decryptedText.SetLength ((int) strlen(szData));

	memcpy (decryptedText.AddressOf(), szData, strlen(szData));
	*/

	// szData를 0으로 초기화하고 사용했다는 가정하에 길이 계산
	int keyLen = nMaxLength;
	while (keyLen > 0)
	{
		if (szData[keyLen - 1] != 0)
			break;
		--keyLen;
	}

	// encode할때와 같은 길이를 얻기 위해서 keyLen를 4의 배수로 맞춤
	while (keyLen & 3) {
		++keyLen;
		// 패딩한 길이 반영
		++nMaxLength;
	}

	// Retrieve the encrypted text for decryption
	decryptedText.SetLength(keyLen);

	memcpy(decryptedText.AddressOf(), szData, keyLen);

	// Decrypt the message
	decrypt((UINT*)decryptedText.AddressOf(),
		decryptedText.GetLength() / 4,
		(UINT*)realKey);

	decryptedText.PushLast(0);
}

int minTea::encrypt(UINT *v, UINT n, UINT *k)
{
	if (n < 2) return -1;

	// TEA routine as per Wheeler & Needham, Oct 1998

	char *pChar = (char*)v;

	UINT z = v[n - 1], y = v[0], delta = 0x9E3779B9;
	UINT mx, e, q = UINT(floor(6 + 52.0f / n)), sum = 0;

	while (q-- > 0) // 6 + 52/n operations gives between 6 & 32 mixes on each word
	{
		sum += delta;
		e = sum >> 2 & 3;
		UINT p;
		// 		for( p = 0; p < n-1; p++ )
		// 		{
		// 			y = v[p+1];
		// 			mx = (z>>5 ^ y<<2) + (y>>3 ^ z<<4) ^ (sum^y) + (k[p&3 ^ e] ^ z);
		// 			z = v[p] += mx;
		// 		}
		// 		y = v[0];
		// 		mx = (z>>5 ^ y<<2) + (y>>3 ^ z<<4) ^ (sum^y) + (k[p&3 ^ e] ^ z);
		// 		z = v[n-1] += mx;

		for (p = 0; p < n; p++)
		{
			y = v[(p + 1) % n];
			mx = (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
			z = v[p] += mx;
		}
	}

	return 0;
}

int minTea::decrypt(UINT *v, UINT n, UINT *k)
{
	if (n < 2) return -1;

	// TEA routine as per Wheeler & Needham, Oct 1998

	char *pChar = (char*)v;

	UINT z = v[n - 1], y = v[0], delta = 0x9E3779B9;
	UINT mx, e, q = UINT(floor(6 + 52.0f / n)), sum = q*delta;

	while (sum != 0)
	{
		e = sum >> 2 & 3;
		int p;
		// 		for( p = n-1; p > 0; p-- )
		// 		{
		// 			z = v[p-1];
		// 			mx = (z>>5 ^ y<<2) + (y>>3 ^ z<<4) ^ (sum^y) + (k[p&3 ^ e] ^ z);
		// 			y = v[p] -= mx;
		// 		}
		// 		z = v[n-1];
		// 		mx = (z>>5 ^ y<<2) + (y>>3 ^ z<<4) ^ (sum^y) + (k[p&3 ^ e] ^ z);
		// 		y = v[0] -= mx;
		// 		sum -= delta;

		for (p = n - 1; p >= 0; p--)
		{
			z = v[p > 0 ? p - 1 : n - 1];
			mx = (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
			y = v[p] -= mx;
		}

		sum -= delta;
	}

	return 0;
}

bool minTea::encrypt(char* szData, int nMaxLength)
{
	acCArray<char> encryptedText;
	encrypt(szData, encryptedText, nMaxLength);

	int nEncryptedLength = encryptedText.GetLength();

	if (nEncryptedLength > nMaxLength)
	{
		/// Encrypted length error
		return false;
	}
	else
	{
		memcpy(szData, encryptedText.AddressOf(), nEncryptedLength);
		return true;
	}

	return true;
}

bool minTea::decrypt(char* szData, int nMaxLength)
{
	acCArray<char> decryptedText;
	decrypt(szData, decryptedText, nMaxLength);

	int nDecryptedLength = decryptedText.GetLength();

	if (nDecryptedLength > nMaxLength)
	{
		/// Decrypted length error
		return false;
	}
	else
	{
		memcpy(szData, decryptedText.AddressOf(), nDecryptedLength);
		return true;
	}

	return true;
}

// 2013.05.31 nodefeat. GUI 환경에서 TCHAR/wchar <-> char 간의 변환 시
// 암호화 된 문자열의 스트링은 동일하지만 구성하는 배열 요소의 값은 다른 문제가 있어서
// 16진수로 처리하도록 함수 추가
void minTea::CharToHexChar(char* pSrc, int nLenSrc, char* pDst, int nLenDst)
{
	// check NULL and length .
	if ((pSrc == NULL) || (nLenSrc == 0) || (pDst == NULL) || (nLenDst == 0))
		return;

	// check length : nLenDst must be bigger than nLenSrc as double of nLenSrc !
	if (nLenDst < (nLenSrc * 2))
		return;

	int     cnt;
	char    strTmp[3] = { 0 };

	for (cnt = 0; cnt < nLenSrc; cnt++)
	{
		memset(strTmp, 3, 0);
		sprintf_s(strTmp, sizeof(strTmp), "%02X", (unsigned char)pSrc[cnt]);
		memcpy(&pDst[cnt * 2], strTmp, 2);
	}
}

void minTea::HexCharToChar(char *pSrc, int nLenSrc, char *pDst, int nLenDst)
{
	// check NULL and length .
	if ((pSrc == NULL) || (nLenSrc == 0) || (pDst == NULL) || (nLenDst == 0))
		return;

	// check length : nLenDst must be bigger than nLenSrc as half of nLenSrc !
	if (nLenDst < (nLenSrc / 2))
		return;

	int             cnt;
	char            strConv[2] = { 0 };
	unsigned char   aChar;

	for (cnt = 0; cnt < (nLenSrc / 2); cnt++)
	{
		strConv[0] = pSrc[(cnt * 2) + 0];
		strConv[1] = pSrc[(cnt * 2) + 1];

		Hex2Char(strConv, &aChar);

		pDst[cnt] = aChar;
	}
}

void minTea::Hex2Char(char const* aChar, unsigned char *rHex)
{
	int     cnt;

	*rHex = 0;

	if (strlen(aChar) < 2)
		return;

	for (cnt = 0; cnt < 2; cnt++)
	{
		if (*(aChar + cnt) >= '0' && *(aChar + cnt) <= '9')
		{
			*rHex = (*rHex << 4) + (*(aChar + cnt) - '0');
		}
		else
			if (*(aChar + cnt) >= 'A' && *(aChar + cnt) <= 'F')
			{
				*rHex = (*rHex << 4) + (*(aChar + cnt) - 'A' + 10);
			}
			else
				if (*(aChar + cnt) >= 'a' && *(aChar + cnt) <= 'f')
				{
					*rHex = (*rHex << 4) + (*(aChar + cnt) - 'a' + 10);
				}
				else
					break;
	}
}