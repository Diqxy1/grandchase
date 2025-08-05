// NOTE : codeproject에 있는 CRC 코드를 변형함. (by parkch)

/****************************************************************************
CRC_32.h : file for the CRC_32 class
written by PJ Arends
pja@telus.net

based on the CRC-32 code found at
http://www.createwindow.com/programming/crc32/crcfile.htm

For updates check http://www3.telus.net/pja/crc32.htm

-----------------------------------------------------------------------------
This code is provided as is, with no warranty as to it's suitability or usefulness
in any application in which it may be used. This code has not been tested for
UNICODE builds, nor has it been tested on a network ( with UNC paths ).

This code may be used in any way you desire. This file may be redistributed by any
means as long as it is not sold for profit, and providing that this notice and the
authors name are included.

If any bugs are found and fixed, a note to the author explaining the problem and
fix would be nice.
-----------------------------------------------------------------------------
****************************************************************************/

#ifndef _CRC_32_H_
#define _CRC_32_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vector>
#include <string>

class CRC_32
{
public:
	static CRC_32& GetInstance(void);

	static std::string GetCRCFromBuffer(const void* pvBuf, UINT uiSize);
	static std::string GetCRCFromFile(const char* szFilePath);
	static std::string GetCRCFromString(const std::string& str);

private:
	/////////////////////////////////////////////////////////////////////////////
	//
	//  CRC_32 constructor  (private member function)
	//    Sets up the CRC-32 reference table
	//
	//  Parameters :
	//    None
	//
	//  Returns :
	//    Nothing
	//
	/////////////////////////////////////////////////////////////////////////////
	CRC_32(void)
	{
		// This is the official polynomial used by CRC-32 
		// in PKZip, WinZip and Ethernet. 
		ULONG ulPolynomial = 0x04C11DB7;

		// 256 values representing ASCII character codes.
		for (int i = 0; i <= 0xFF; i++)
		{
			Table[i] = Reflect(i, 8) << 24;
			for (int j = 0; j < 8; j++)
				Table[i] = (Table[i] << 1) ^ (Table[i] & (1 << 31) ? ulPolynomial : 0);
			Table[i] = Reflect(Table[i], 32);
			IndexTable[Table[i] & 0xFF] = i;
			IndexTable2[i] = Table[i] & 0xFF;
			InverseTable[Table[i] >> 24] = (Table[i] << 8) ^ i;
		}

		for (int i = 0; i <= 0xFF; i++)
		{
			ReverseTable[ByteTable[i * 4 + 3]] = i;
			ReverseHighTable[i] = ByteTable[i * 4 + 3];
		}

		for (int i = 0; i <= 0xFF; i++)
		{
			Table[(Table[i] & 0xFF) + 320] = i | Table[i] & 0xFFFFFF00;
		}
	}

public:
	/////////////////////////////////////////////////////////////////////////////
	//
	//  CRC_32::CalcCRC  (public member function)
	//    calculates the CRC-32 value for the given buffer
	//
	//  Parameters :
	//    buffer      [in] : a pointer to the data bytes
	//    size        [in] : the size of the buffer
	//
	//  Returns :
	//    returns the CRC-32 value of the buffer
	//    returns NULL if an error occurs
	//
	/////////////////////////////////////////////////////////////////////////////
	inline DWORD CalcCRC(const void* buffer, UINT size) const
	{
		// check the validity of the data
		if (!buffer || !size)
			return 0;

		// calculate CRC directly
		DWORD CRC = 0xFFFFFFFF;
		Calculate(reinterpret_cast<const BYTE*>(buffer), size, CRC);
		return CRC ^ 0xFFFFFFFF;
	}

//#ifdef  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
	__forceinline void CryptAlgorithm3(BYTE* buffer, UINT size, BYTE* encryptedBuffer, UINT encryptedSize, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		UINT tmpSize = size;
		while (tmpSize--)
		{
			DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
			DWORD xorByte = xorKey ^ (~(*pbyte++) & 0xFF);
			crc = (xorByte | (Table[xorByte] & 0xFFFFFF00)) ^ (crc >> 8);
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
		CRC_ = crc;

		uXORIndex %= xorSize;

		std::vector<DWORD> crcList;

		pbyte = encryptedBuffer;
		tmpSize = encryptedSize;
		crcList.push_back(crc);
		while (tmpSize--)
		{
			BYTE xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
			BYTE xorByte = xorKey ^ (~(*pbyte) & 0xFF);
			*pbyte++ = xorKey ^ (Table[xorByte] & 0xFF);
			crc = (xorByte | (Table[xorByte] & 0xFFFFFF00)) ^ (crc >> 8);
			crcList.push_back(crc);
			uXORIndex = (uXORIndex + 1) % xorSize;
		}

		if (encryptedSize >= 4)
		{
			uXORIndex %= xorSize;

			DWORD ByteCrc1 = crc;
			DWORD ByteIndex1 = ReverseTable[(crc >> 24)];
			DWORD ByteCrc2 = (ByteCrc1 ^ (ByteIndex1 | (Table[ByteIndex1] & 0xFFFFFF00))) << 8;
			DWORD ByteIndex2 = ReverseTable[(ByteCrc2 >> 24)];
			DWORD ByteCrc3 = (ByteCrc2 ^ (ByteIndex2 | (Table[ByteIndex2] & 0xFFFFFF00))) << 8;
			DWORD ByteIndex3 = ReverseTable[(ByteCrc3 >> 24)];
			DWORD ByteCrc4 = (ByteCrc3 ^ (ByteIndex3 | Table[ByteIndex3] & 0xFFFFFF00)) << 8;
			DWORD ByteIndex4 = ReverseTable[(ByteCrc4 >> 24)];

			pbyte[0] = (CRC_ ^ pXORTable[uXORIndex]) ^ Table[ByteIndex4];
			uXORIndex = (uXORIndex + 1) % xorSize;

			pbyte[1] = (((CRC_ >> 8) & 0xFF) ^ ByteIndex4^ pXORTable[uXORIndex]) ^ Table[ByteIndex3];
			uXORIndex = (uXORIndex + 1) % xorSize;

			CRC_ = (CRC_ >> 8) ^ (ByteIndex4 | (Table[ByteIndex4] & 0xFFFFFF00));
			CRC_ = (CRC_ >> 8) ^ (ByteIndex3 | (Table[ByteIndex3] & 0xFFFFFF00));

			pbyte[2] = (CRC_ ^ pXORTable[uXORIndex]) ^ Table[ByteIndex2];

			CRC_ = (CRC_ >> 8) ^ (ByteIndex2 | (Table[ByteIndex2] & 0xFFFFFF00));
			uXORIndex = (uXORIndex + 1) % xorSize;

			pbyte[3] = (CRC_ ^ pXORTable[uXORIndex]) ^ Table[ByteIndex1];
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
	}

	__forceinline void CryptAlgorithm2(BYTE* buffer, UINT size, BYTE* encryptedBuffer, UINT encryptedSize, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		UINT tmpSize = size;
		while (tmpSize--)
		{
			BYTE TableIndex = ((crc & 0xFF) ^ (*pbyte++) ^ pXORTable[uXORIndex]);
			crc = (crc >> 8) ^ Table[TableIndex];
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
		CRC_ = crc;

		uXORIndex %= xorSize;

		pbyte = encryptedBuffer;
		tmpSize = encryptedSize;
		while (tmpSize--)
		{
			BYTE xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
			BYTE xorByte = xorKey ^ (~(*pbyte) & 0xFF);
			*pbyte++ = xorKey ^ (Table[xorByte] & 0xFF);
			crc = (xorByte | (Table[xorByte] & 0xFFFFFF00)) ^ (crc >> 8);
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
	}

	bool Process4Bytes(int index, BYTE* pbyte, const BYTE* pXORTable, DWORD uXORIndex, DWORD xorSize, DWORD currentCrc, DWORD desiredCrc) const
	{
		if (index == 3)
		{
			for (int n = 0; n < 256; n++)
			{
				DWORD crc = currentCrc;
				DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
				BYTE reverseByte = IndexTable[n ^ xorKey];
				crc = (reverseByte | (Table[reverseByte] & 0xFFFFFF00)) ^ (crc >> 8);

				if (crc == desiredCrc)
				{
					*pbyte = ~(xorKey ^ reverseByte);
					return true;
				}
			}
		}
		else
		{
			for (int n = 0; n < 256; n++)
			{
				DWORD crc = currentCrc;
				DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
				BYTE reverseByte = IndexTable[n ^ xorKey];
				crc = (reverseByte | (Table[reverseByte] & 0xFFFFFF00)) ^ (crc >> 8);

				if (Process4Bytes(index + 1, pbyte + 1, pXORTable, (uXORIndex + 1) % xorSize, xorSize, crc, desiredCrc) == true)
				{
					*pbyte = ~(xorKey ^ reverseByte);
					return true;
				}
			}
		}

		return false;
	}

	__forceinline void EncryptAlgorithm3(BYTE* buffer, UINT size, BYTE* encryptBuffer, UINT encryptSize, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		UINT tmpSize = size;
		while (tmpSize--)
		{
			DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
			DWORD xorByte = xorKey ^ (~(*pbyte++) & 0xFF);
			crc = (xorByte | (Table[xorByte] & 0xFFFFFF00)) ^ (crc >> 8);
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
		CRC_ = crc;

		uXORIndex %= xorSize;

		if (encryptSize < 4)
		{
			pbyte = encryptBuffer;
			tmpSize = encryptSize;
			while (tmpSize--)
			{
				DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
				BYTE reverseByte = IndexTable[*pbyte ^ xorKey];
				*pbyte++ = ~(xorKey ^ reverseByte);
				crc = (reverseByte | (Table[reverseByte] & 0xFFFFFF00)) ^ (crc >> 8);
				uXORIndex = (uXORIndex + 1) % xorSize;
			}
		}
		else
		{
			BYTE* outputBuffer = new BYTE[encryptSize + 1024];
			memcpy(outputBuffer + 4, encryptBuffer, encryptSize);
			*(DWORD*)outputBuffer = *(DWORD*)(outputBuffer + encryptSize);

			DWORD ReverseCRC = CRC_;

			UINT xorIndex1 = (uXORIndex + encryptSize + 0) % xorSize;
			UINT xorIndex2 = (uXORIndex + encryptSize + 1) % xorSize;
			UINT xorIndex3 = (uXORIndex + encryptSize + 2) % xorSize;
			UINT xorIndex4 = (uXORIndex + encryptSize + 3) % xorSize;

			DWORD tmpCrc = CRC_;

			BYTE FirstByte = (tmpCrc ^ pXORTable[xorIndex1]) ^ outputBuffer[encryptSize];
			DWORD CrcIndex4 = IndexTable[FirstByte];
			DWORD ByteIndex4 = ReverseTable[ByteTable[CrcIndex4 * 4 + 3]];

			BYTE SecondByte = ((tmpCrc >> 8) ^ ByteIndex4^ pXORTable[xorIndex2]) ^ outputBuffer[encryptSize + 1];
			DWORD CrcIndex3 = IndexTable[SecondByte];
			DWORD ByteIndex3 = ReverseTable[ByteTable[CrcIndex3 * 4 + 3]];

			tmpCrc = (tmpCrc >> 8) ^ (ByteIndex4 | (Table[ByteIndex4] & 0xFFFFFF00));
			tmpCrc = (tmpCrc >> 8) ^ (ByteIndex3 | (Table[ByteIndex3] & 0xFFFFFF00));

			BYTE ThirdByte = (tmpCrc ^ pXORTable[xorIndex3]) ^ outputBuffer[encryptSize + 2];
			DWORD CrcIndex2 = IndexTable[ThirdByte];
			DWORD ByteIndex2 = ReverseTable[ByteTable[CrcIndex2 * 4 + 3]];

			tmpCrc = (tmpCrc >> 8) ^ (ByteIndex2 | (Table[ByteIndex2] & 0xFFFFFF00));

			BYTE FourthByte = (tmpCrc ^ pXORTable[xorIndex4]) ^ outputBuffer[encryptSize + 3];
			DWORD CrcIndex1 = IndexTable[FourthByte];
			DWORD ByteIndex1 = ReverseTable[ByteTable[CrcIndex1 * 4 + 3]];

			DWORD ByteCrc4 = Table[CrcIndex4];
			DWORD ByteCrc3 = (ByteCrc4 >> 8) ^ (ByteIndex3 | Table[ByteIndex3] & 0xFFFFFF00);
			DWORD ByteCrc2 = (ByteCrc3 >> 8) ^ (ByteIndex2 | Table[ByteIndex2] & 0xFFFFFF00);
			DWORD ByteCrc1 = (ByteCrc2 >> 8) ^ (ByteIndex1 | Table[ByteIndex1] & 0xFFFFFF00);

			ReverseCRC = ByteCrc1;

			pbyte = outputBuffer + encryptSize - 1;
			tmpSize = encryptSize;

			DWORD tmpXorIndex = uXORIndex % xorSize;
			uXORIndex = (uXORIndex + encryptSize - 1) % xorSize;
			std::vector<DWORD> crcList;
			crcList.push_back(ReverseCRC);
			while (tmpSize--)
			{
				BYTE reverseByte = ReverseTable[(ReverseCRC >> 24)];
				BYTE lastCrcByte = (IndexTable2[reverseByte] & 0xFF) ^ pXORTable[uXORIndex] ^ *pbyte;

				if (tmpSize == 3)
				{
					if (Process4Bytes(0, outputBuffer, pXORTable, tmpXorIndex, xorSize, CRC_, ReverseCRC) == true)
						break;
					else
					{
						//MessageBox(NULL, "Error!", "Algorithm 3/4 something went wrong", MB_OK);
						break;
					}
				}
				else
				{
					ReverseCRC = (((reverseByte |
						(Table[reverseByte] & 0xFFFFFF00)) << 8) ^
						(ReverseCRC << 8) & 0xFFFFFF00) | lastCrcByte;
					crcList.push_back(ReverseCRC);

					DWORD xorKey = ((ReverseCRC & 0xFF) ^ pXORTable[uXORIndex]);
					BYTE reverseByte2 = IndexTable[*pbyte ^ xorKey];
					*pbyte-- = ~(xorKey ^ reverseByte2);
				}

				uXORIndex = (uXORIndex == 0 ? xorSize - 1 : uXORIndex - 1);
			}

			memcpy(encryptBuffer, outputBuffer, encryptSize);
			delete[] outputBuffer;
		}
	}

	__forceinline void EncryptAlgorithm2(BYTE* buffer, UINT size, BYTE* encryptBuffer, UINT encryptSize, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		UINT tmpSize = size;
		while (tmpSize--)
		{
			crc = (crc >> 8) ^ Table[(crc & 0xFF) ^ (*pbyte++) ^ pXORTable[uXORIndex]];
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
		CRC_ = crc;

		uXORIndex %= xorSize;

		pbyte = encryptBuffer;
		tmpSize = encryptSize;
		while (tmpSize--)
		{
			DWORD xorKey = ((crc & 0xFF) ^ pXORTable[uXORIndex]);
			BYTE reverseByte = IndexTable[*pbyte ^ xorKey];
			*pbyte++ = ~(xorKey ^ reverseByte);
			crc = (reverseByte | (Table[reverseByte] & 0xFFFFFF00)) ^ (crc >> 8);
			uXORIndex = (uXORIndex + 1) % xorSize;
		}
	}

	__forceinline void CalculateWithoutEncrypt(const BYTE* buffer, UINT size, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		const BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		while (size--)
		{
			crc = (crc >> 8) ^ Table[(crc & 0xFF) ^ (*pbyte++) ^ pXORTable[uXORIndex]];
			uXORIndex++;
			if (uXORIndex == xorSize)
				uXORIndex = 0;
		}
		CRC_ = crc;
	}
	__forceinline void CalculateAndDecrypt(BYTE* buffer, UINT size, const BYTE* pXORTable, UINT xorSize, ULONG& CRC_) const
	{
		if (buffer == NULL || size == 0 || pXORTable == NULL || xorSize == 0)
			return;

		// calculate the CRC
		BYTE* pbyte = buffer;
		ULONG   crc = CRC_;
		UINT    uXORIndex = 0;
		ULONG   ulComp, ulStored, ulEncrypted;
		while (size--)
		{
			ulComp = (crc & 0xFF) ^ pXORTable[uXORIndex];
			ulStored = *pbyte ^ 0xFF ^ ulComp;
			ulEncrypted = Table[ulStored];
			(*pbyte++) = (BYTE)((ulEncrypted & 0xFF) ^ ulComp);
			crc = (crc >> 8) ^ ((ulEncrypted & 0xFFFFFF00) | ulStored);
			uXORIndex++;
			if (uXORIndex == xorSize)
				uXORIndex = 0;
		}
		CRC_ = crc;
	}
//#endif  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION


private:
	/////////////////////////////////////////////////////////////////////////////
	//
	//  CRC_32::Calculate  (private member function)
	//    Calculates the CRC-32 value for the given buffer
	//
	//  Parameters :
	//    buffer [in] : pointer to the data bytes
	//    size   [in] : the size of the buffer
	//    CRC    [in] : the initial CRC-32 value
	//          [out] : the new CRC-32 value
	//
	//  Returns :
	//    Nothing
	//
	/////////////////////////////////////////////////////////////////////////////
	inline void Calculate(const BYTE* buffer, UINT size, ULONG& CRC) const
	{
		// calculate the CRC
		const BYTE* pbyte = buffer;

		while (size--)
			CRC = (CRC >> 8) ^ Table[(CRC & 0xFF) ^ *pbyte++];
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	//  CRC_32::Reflect  (private member function)
	//    used by the constructor to help set up the CRC-32 reference table
	//
	//  Parameters :
	//    ref [in] : the value to be reflected
	//    ch  [in] : the number of bits to move
	//
	//  Returns :
	//    the new value
	//
	/////////////////////////////////////////////////////////////////////////////
	inline ULONG Reflect(ULONG ref, char ch) const
	{
		ULONG value = 0;

		// Swap bit 0 for bit 7
		// bit 1 for bit 6, etc.
		for (int i = 1; i < (ch + 1); i++)
		{
			if (ref & 1)
				value |= 1 << (ch - i);
			ref >>= 1;
		}
		return value;
	}

private:
	union
	{
		ULONG Table[600];
		BYTE ByteTable[600 * 4];
	};
	ULONG InverseTable[256];
	BYTE IndexTable[256];
	BYTE IndexTable2[256];
	BYTE ReverseTable[256];
	BYTE ReverseHighTable[256];
};

#ifdef CRC
#undef CRC
#endif

#define CRC (CRC_32::GetInstance())

#endif // _CRC_32_H_