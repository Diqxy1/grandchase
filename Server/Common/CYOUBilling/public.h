/********************************************************************
 *created date:
 *filename:     public.h
 *author:
 *contents: PacketDefine, pconst, ptyoes, AssertEx, ErrCode, public,
 *	    ThreadLock, TimeEx, IOStream, BillLog, Packet
description:libpci.a/pci.lib Head File
 ********************************************************************/
#ifndef __PUBLIC_H__
#define __PUBLIC_H__

typedef enum TYPES
{
	TYPE_MIN = 0x00000000,

	PACKET_LB_CONNECT = 0x000000A0,
	PACKET_BL_RETCONNECT = 0x000000A0,

	PACKET_LB_KEEPLIVE = 0x000000A1,
	PACKET_BL_RETKEEPLIVE = 0x000000A1,

	PACKET_LB_AUTHUSER = 0x000000A2,
	PACKET_BL_RETAUTHUSER = 0x000000A2,

	PACKET_LB_BILLSTART = 0x000000A3,
	PACKET_BL_RETBILLSTART = 0x000000A3,

	PACKET_LB_BILLEND = 0x000000A4,
	PACKET_BL_RETBILLEND = 0x000000A4,

	PACKET_LB_BILLKEEPING = 0x000000A6,
	PACKET_BL_RETBILLKEEPING = 0x000000A6,

	PACKET_LB_CONFIRMPROTOCOL = 0x000000A7,
	PACKET_BL_RETCONFIRMPROTOCOL = 0x000000A7,

	PACKET_LB_KICKALL = 0x000000A9,
	PACKET_BL_RETKICKALL = 0x000000A9,

	PACKET_LB_SWITCHROLE = 0x000000AA,
	PACKET_BL_RETSWITCHROLE = 0x000000AA,

	PACKET_LB_SWITCHSERVER = 0x000000AB,
	PACKET_BL_RETSWITCHSERVER = 0x000000AB,

	PACKET_LB_DISABLEUSER = 0x000000B1,
	PACKET_BL_RETDISABLEUSER = 0x000000B1,

	PACKET_LB_PHONELOCKED = 0x000000B2,
	PACKET_BL_RETPHONELOCKED = 0x000000B2,

	PACKET_LB_MIBAOCARDCHED = 0x000000B3,
	PACKET_BL_RETMIBAOCARDCHED = 0x000000B3,

	PACKET_LB_OPTCHECKED = 0x000000BA,
	PACKET_BL_RETOPTCHECKED = 0x000000BA,

	PACKET_LB_USERINFOCOLLECT = 0x000000BB,
	PACKET_BL_RETUSERINFOCOLLECT = 0x000000BB,

	PACKET_LB_CARDLOTTERY = 0x000000C1,
	PACKET_BL_RETCARDLOTTERY = 0x000000C1,

	PACKET_LB_CNLOTTERY = 0x000000C3,
	PACKET_BL_RETCNLOTTERY = 0x000000C3,

	PACKET_LB_BUYLOG = 0x000000C5,
	PACKET_BL_RETBUYLOG = 0x000000C5,

	PACKET_LB_CARDLOTTERYCHECK = 0x000000C6,
	PACKET_BL_RETCARDLOTTERYCHECK = 0x000000C6,

	PACKET_LB_EXCHANGEYB = 0x000000E1,
	PACKET_BL_RETEXCHANGEYB = 0x000000E1,

	PACKET_LB_CNPOINT = 0x000000E2,
	PACKET_BL_RETCNPOINT = 0x000000E2,

	PACKET_LB_CNLOTTERYCHECK = 0x000000E4,
	PACKET_BL_RETCNLOTTERYCHECK = 0x000000E4,

	PACKET_LB_UNIVCARDLOTTERY = 0x000000CA,
	PACKET_BL_RETUNIVCARDLOTTERY = 0x000000CA,

	PACKET_LB_IMMELOTTERYTYPECHECK = 0x000000CC,
	PACKET_BL_RETIMMELOTTERYTYPECHECK = 0x000000CC,

	PACKET_LB_IMMELOTTERYTYPESPECIFY = 0x000000CD,
	PACKET_BL_RETIMMELOTTERYTYPESPECIFY = 0x000000CD,

	PACKET_LB_IMMELOTTERYTYPECONFIRM = 0x000000CE,
	PACKET_BL_RETIMMELOTTERYTYPECONFIRM = 0x000000CE,

	PACKET_LB_AUTHUSERFB = 0x000100A2,
	PACKET_BL_RETAUTHUSERFB = 0x000100A2,

	TYPE_MAX = 0xFFFFFFFF
}TYPES;

typedef enum PCI_CONST
{
	PCI_FALSE = 0,
	PCI_TRUE = 1,

	PCI_MIN_IP_LEN = 7,
	PCI_MAX_IP_LEN = 16,

	PCI_MIN_CN_LEN = 5,
	PCI_MAX_CN_LEN = 51,

	PCI_MIN_PASSWD_LEN = 1,
	PCI_MAX_PASSWD_LEN = 33,

	PCI_MIN_ROLE_NAME_LEN = 1,
	PCI_MAX_ROLE_NAME_LEN = 61,

	PCI_PASS_POD_LEN = 9,

	PCI_PC_VALUE_LEN = 41,

	PCI_MIBAO_CARD_LEN = 3,

	PCI_MIN_CARD_LEN = 1,
	PCI_MAX_CARD_LEN = 21,

	PCI_MAX_SERIAL_NO_LEN = 22,

	PCI_MIN_REASON_LEN = 1,
	PCI_MAX_REASON_LEN = 255,

	PCI_MIN_GOODS_LEN = 1,
	PCI_MAX_GOODS_LEN = 255,

	PCI_MIN_MEMO_LEN = 0,
	PCI_MAX_MEMO_LEN = 51,

	PCI_MAX_PASS_LEN = 33,

	PCI_MIN_REMARK_LEN = 1,
	PCI_MAX_REMARK_LEN = 255,

	PCI_MIN_GAMENAME_LEN = 1,
	PCI_MAX_GAMENAME_LEN = 31,

	PCI_MIN_DRAWTYPE_LEN = 1,
	PCI_MAX_DRAWTYPE_LEN = 11,

	PCI_SERVERNAME_LEN = 64,

	PCI_MAX_AWARD_LEN = 254,

	PCI_AWARDLIST_LEN = 101,

	PCI_LIMITINFO_LEN = 81,

	PCI_MAX_LOTTERYTYPE_LEN = 255,

	PCI_MAX_PATH = 255,
}PCI_CONST;

typedef char                    PCI_CHAR;
typedef short                   PCI_SHORT;
typedef int                     PCI_INT;
typedef unsigned char           PCI_UCHAR;
typedef unsigned short          PCI_USHORT;
typedef unsigned int            PCI_UINT;

#if (PCI_LONG64)
typedef long                    PCI_LONG;
typedef unsigned long           PCI_ULONG;
#else
typedef long long               PCI_LONG;
typedef unsigned long long      PCI_ULONG;
#endif

typedef void                    PCI_VOID;
typedef PCI_SHORT               PCI_BOOL;

typedef struct
{
	PCI_CHAR PosX[PCI_MIBAO_CARD_LEN];
	PCI_CHAR PosY[PCI_MIBAO_CARD_LEN];
	PCI_CHAR PosZ[PCI_MIBAO_CARD_LEN];
	PCI_CHAR ValueX[PCI_MIBAO_CARD_LEN];
	PCI_CHAR ValueY[PCI_MIBAO_CARD_LEN];
	PCI_CHAR ValueZ[PCI_MIBAO_CARD_LEN];
}MibaoCard;

PCI_VOID __assert__(const PCI_CHAR* file, PCI_UINT line, const PCI_CHAR* func, const PCI_CHAR* expr);
PCI_VOID __assertex__(const PCI_CHAR* file, PCI_UINT line, const PCI_CHAR* func, const PCI_CHAR* expr, const PCI_CHAR* msg);
PCI_VOID __assertspecial__(const PCI_CHAR* file, PCI_UINT line, const PCI_CHAR* func, const PCI_CHAR* expr, const PCI_CHAR* msg);

#ifdef WIN32
#define Assert(expr) ((VOID)((expr)?0:(__assert__(__FILE__,__LINE__,__FUNCTION__,#expr),0)))
#define AssertEx(expr,msg) ((VOID)((expr)?0:(__assertex__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#define AssertSpecial(expr,msg) ((VOID)((expr)?0:(__assertspecial__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#else  //Linux
#define Assert(expr) {if(!(expr)){__assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr);}}
#define AssertEx(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
#endif

#define PKG_0003 "PKG-0003: Packet content Err. "
#define SYS_0005 "SYS-0005: Function in param invalid. "
#define SYS_0006 "SYS-0006: Work Dir Setting invalid. "

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32	//WIN
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define bzero(s, n) memset(s, 0, n)
#define __ENTER_FUNCTION  { try{
#define __LEAVE_FUNCTION  } catch(...){AssertSpecial(PCI_FALSE,__FUNCTION__);}}
#define        tvsnprintf        _vsnprintf
#define        tstricmp          _stricmp
#define        tsnprintf         _snprintf
#else 		//Linux
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdarg.h>
#define __ENTER_FUNCTION  { try{
#define __LEAVE_FUNCTION  } catch(...){AssertSpecial(PCI_FALSE,__PRETTY_FUNCTION__);}}
#define        tvsnprintf        vsnprintf
#define        tstricmp          strcasecmp
#define        tsnprintf         snprintf
#endif

// 빳년傑
class CMutex
{
public:
	CMutex()
	{
		__ENTER_FUNCTION
#ifdef WIN32
			InitializeCriticalSection(&m_Mutex);
#else //Linux
			pthread_mutex_init(&m_Mutex, NULL);
#endif //WIN32
		__LEAVE_FUNCTION
	}
	~CMutex()
	{
		__ENTER_FUNCTION
#ifdef WIN32
			DeleteCriticalSection(&m_Mutex);
#else //Linux
			pthread_mutex_destroy(&m_Mutex);
#endif //WIN32
		__LEAVE_FUNCTION
	}

public:
	PCI_VOID lock(PCI_VOID);
	PCI_VOID unlock(PCI_VOID);

private:
#ifdef WIN32
	CRITICAL_SECTION m_Mutex;
#else //Linux
	pthread_mutex_t m_Mutex;
#endif //WIN32
};

using namespace std;

//class CTime
//{
//	public:
//		CTime();
//		virtual ~CTime();
//		static PCI_INT GetDate(PCI_CHAR * str);
//		static PCI_INT GetTime(PCI_CHAR * str);
//};

class InputStream
{
public:
	InputStream(PCI_CHAR* pbuf) : m_pbuf(pbuf) {};
	~InputStream() {};

public:
	PCI_VOID Read(PCI_INT &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_INT));
		val = ntohl(val);
		m_pbuf += sizeof(PCI_INT);
	};

	PCI_VOID Read(PCI_UINT &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_UINT));
		val = ntohl(val);
		m_pbuf += sizeof(PCI_UINT);
	};

	PCI_VOID Read(PCI_SHORT &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_SHORT));
		val = ntohs(val);
		m_pbuf += sizeof(PCI_SHORT);
	};

	PCI_VOID Read(PCI_USHORT &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_USHORT));
		val = ntohs(val);
		m_pbuf += sizeof(PCI_USHORT);
	};

	PCI_VOID Read(PCI_LONG &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_LONG));
		PCI_INT  tmpH;
		PCI_UINT tmpL;
		memcpy(&tmpH, &val, sizeof(PCI_INT));
		memcpy(&tmpL, (PCI_UINT*)(&val) + 1, sizeof(PCI_UINT));

		val = 0;

		val = ntohl(tmpH);
		val = val << 32;
		val += ntohl(tmpL);

		m_pbuf += sizeof(PCI_LONG);
	};

	PCI_VOID Read(PCI_ULONG &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_ULONG));
		PCI_UINT tmpL, tmpH;
		memcpy(&tmpH, &val, sizeof(PCI_UINT));
		memcpy(&tmpL, (PCI_UINT*)(&val) + 1, sizeof(PCI_UINT));

		val = 0;

		val = ntohl(tmpH);
		val = val << 32;
		val += ntohl(tmpL);

		m_pbuf += sizeof(PCI_ULONG);
	};

	PCI_VOID Read(PCI_UCHAR &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_UCHAR));
		m_pbuf += sizeof(PCI_UCHAR);
	};

	PCI_VOID Read(PCI_CHAR &val)
	{
		memcpy(&val, m_pbuf, sizeof(PCI_CHAR));
		m_pbuf += sizeof(PCI_CHAR);
	};

	PCI_VOID Read(PCI_CHAR* val, PCI_UINT size)
	{
		memcpy(val, m_pbuf, size);
		m_pbuf += size;
	};

public:
	PCI_CHAR * GetBuffer(PCI_VOID)
	{
		return m_pbuf;
	}

private:
	PCI_CHAR *m_pbuf;
};

class OutputStream
{
public:
	OutputStream(PCI_CHAR* pbuf) : m_pbuf(pbuf) {};
	~OutputStream() {};

public:
	PCI_VOID Write(PCI_INT val)
	{
		val = htonl(val);
		memcpy(m_pbuf, &val, sizeof(PCI_INT));
		m_pbuf += sizeof(PCI_INT);
	};

	PCI_VOID Write(PCI_UINT val)
	{
		val = htonl(val);
		memcpy(m_pbuf, &val, sizeof(PCI_UINT));
		m_pbuf += sizeof(PCI_UINT);
	};

	PCI_VOID Write(PCI_SHORT val)
	{
		val = htons(val);
		memcpy(m_pbuf, &val, sizeof(PCI_SHORT));
		m_pbuf += sizeof(PCI_SHORT);
	};

	PCI_VOID Write(PCI_USHORT val)
	{
		val = htons(val);
		memcpy(m_pbuf, &val, sizeof(PCI_USHORT));
		m_pbuf += sizeof(PCI_USHORT);
	};

	PCI_VOID Write(PCI_CHAR val)
	{
		memcpy(m_pbuf, &val, sizeof(PCI_CHAR));
		m_pbuf += sizeof(PCI_CHAR);
	};

	PCI_VOID Write(PCI_UCHAR val)
	{
		memcpy(m_pbuf, &val, sizeof(PCI_UCHAR));
		m_pbuf += sizeof(PCI_UCHAR);
	}

	PCI_VOID Write(PCI_CHAR* val, PCI_UINT size)
	{
		memcpy(m_pbuf, val, size);
		m_pbuf += size;
	};

	PCI_VOID Write(const PCI_CHAR* val, PCI_UINT size)
	{
		memcpy(m_pbuf, val, size);
		m_pbuf += size;
	};

	PCI_VOID Write(PCI_LONG val)
	{
		PCI_INT tmpH;
		PCI_UINT tmpL;
		memcpy(&tmpH, &val, sizeof(PCI_UINT));
		memcpy(&tmpL, (PCI_UINT*)(&val) + 1, sizeof(PCI_UINT));

		val = 0;

		val = htonl(tmpH);
		val = val << 32;
		val += htonl(tmpL);

		memcpy(m_pbuf, &val, sizeof(PCI_LONG));
		m_pbuf += sizeof(PCI_LONG);
	};

	PCI_VOID Write(PCI_ULONG val)
	{
		PCI_UINT tmpL, tmpH;
		memcpy(&tmpH, &val, sizeof(PCI_UINT));
		memcpy(&tmpL, (PCI_UINT*)(&val) + 1, sizeof(PCI_UINT));

		val = 0;

		val = htonl(tmpH);
		val = val << 32;
		val += htonl(tmpL);

		memcpy(m_pbuf, &val, sizeof(PCI_ULONG));
		m_pbuf += sizeof(PCI_ULONG);
	};

	PCI_USHORT WritePacketSize(PCI_UCHAR* pbuf)
	{
		PCI_USHORT uSize = (PCI_USHORT)((PCI_CHAR*)m_pbuf - (PCI_CHAR*)pbuf - 6);
		uSize = htons(uSize);
		memcpy(pbuf + 2, &uSize, sizeof(PCI_USHORT));
		return ntohs(uSize);
	}

public:
	PCI_CHAR * GetBuffer(PCI_VOID)
	{
		return m_pbuf;
	}
private:
	PCI_CHAR *m_pbuf;
};

#define ERR_LOGFILE              "log/err.log"
#define ASSERT_LOGFILE           "log/assert.log"

//pci휑羚쨌쓺
extern PCI_CHAR g_work_dir[PCI_MAX_PATH];

//삿혤匡숭댕鬼
PCI_INT Getfilesize(const PCI_CHAR *__path);

//휑羚畇흙늚턍,醵똑찹
PCI_VOID DiskLog(PCI_CHAR* filename, PCI_CHAR const* msg, ...);
// 畇던댄轎쯤돨휑羚
PCI_VOID DiskErrLog(PCI_CHAR const* errcode, PCI_CHAR const* msg, ...);

//?阮첫ㅧ笞렙?PCI_VOID SetWorkDir(const PCI_CHAR *path, PCI_USHORT length);

typedef PCI_UINT PacketID_t;

class Packet
{
public:
	Packet()
	{
		m_Index = 0;
		bzero(m_szServerIP, sizeof(m_szServerIP));
	}

	virtual ~Packet() {}

	virtual PCI_BOOL  Read(InputStream   & iStream) = 0;
	virtual PCI_BOOL  Write(OutputStream & oStream) const = 0;

	virtual PacketID_t GetPacketID() const = 0;

	PCI_USHORT GetPacketIndex() const { return m_Index; }
	PCI_VOID   SetPacketIndex(const PCI_USHORT Index) { m_Index = Index; }

	const PCI_CHAR  *  GetServerIP() const { return m_szServerIP; }
	PCI_VOID  SetServerIP(const PCI_CHAR *ServerIP) { strncpy(m_szServerIP, ServerIP, sizeof(m_szServerIP) - 1); }

private:
	PCI_USHORT m_Index;                   //션쩌login관埼죗뵀，닖관櫓뗍혤

	PCI_CHAR   m_szServerIP[PCI_MAX_IP_LEN];    //션쩌login돨IP
};

#endif
