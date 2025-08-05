/********************************************************************
 * created date:
 * filename: Packet/PacketReq.h
 *
 *description: 用户请求协议发送包
 * ******************************************************************/
#ifndef __PACKETREQ_H__
#define __PACKETREQ_H__

#include "public.h"
 /********************************************************************
 created date:
 filename:      Packet/LBAuthUser.h

 description:   用户登录认证协议发送包
  ********************************************************************/

class LBAuthUserFB : public Packet
{
public:
	LBAuthUserFB();
	virtual ~LBAuthUserFB();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_AUTHUSERFB;
	}

public:
	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_CHAR*           GetPassword(PCI_VOID) { return m_szPassword; }

	PCI_VOID            SetPassword(PCI_CHAR* szPassword)
	{
		SetPasswordLen(strlen(szPassword));
		strncpy(m_szPassword, szPassword, sizeof(m_szPassword) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	PCI_USHORT          GetRoleLevel(PCI_VOID) { return m_uRoleLevel; }

	PCI_VOID            SetRoleLevel(PCI_USHORT uRoleLevel) { m_uRoleLevel = uRoleLevel; }

	MibaoCard       GetMibaoCard(PCI_VOID) { return m_mibaoCard; }

	PCI_VOID            SetMibaoCard(MibaoCard mibaoCard) { m_mibaoCard = mibaoCard; }

	PCI_CHAR*           GetPasspod(PCI_VOID) { return m_szPasspod; }

	PCI_VOID            SetPassPod(PCI_CHAR* szPasspod) { strncpy(m_szPasspod, szPasspod, sizeof(m_szPasspod) - 1); }

	PCI_CHAR*           GetMACMD5Value(PCI_VOID) { return m_szMACMD5Value; }

	PCI_VOID            SetMACMD5Value(PCI_CHAR* szMACMD5Value) { strncpy(m_szMACMD5Value, szMACMD5Value, sizeof(m_szMACMD5Value) - 1); }

	PCI_CHAR*  GetPCValue(PCI_VOID) { return m_szPCValue; }

	PCI_VOID   SetPCValue(PCI_CHAR* szPCValue)
	{
		SetPCLen(strlen(szPCValue));
		strncpy(m_szPCValue, szPCValue, sizeof(m_szPCValue) - 1);
	}

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetPasswordLen(PCI_VOID) { return m_uPasswordLen; }
	PCI_VOID            SetPasswordLen(PCI_UCHAR uPasswordLen) { m_uPasswordLen = uPasswordLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR  GetPCLen(PCI_VOID) { return m_uPCLen; }
	PCI_VOID   SetPCLen(PCI_UCHAR uPCLen) { m_uPCLen = uPCLen; }
private:
	PCI_UCHAR           m_uCNLen;
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uPasswordLen;
	PCI_CHAR            m_szPassword[PCI_MAX_PASSWD_LEN];
	PCI_UCHAR           m_uUserIPLen;
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];

	PCI_USHORT          m_uRoleLevel;
	MibaoCard       m_mibaoCard;
	PCI_CHAR            m_szPasspod[PCI_PASS_POD_LEN];
	PCI_CHAR            m_szMACMD5Value[PCI_MAX_PASSWD_LEN];
	PCI_UCHAR           m_uPCLen;
	PCI_CHAR            m_szPCValue[PCI_PC_VALUE_LEN];
};

/********************************************************************
created date:
filename:      Packet/LBAuthUser.h

description:   用户登录认证协议发送包
 ********************************************************************/

class LBAuthUser : public Packet
{
public:
	LBAuthUser();
	virtual ~LBAuthUser();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_AUTHUSER;
	}

public:
	PCI_CHAR*			GetEN(PCI_VOID) { return m_szEN; }

	PCI_VOID			SetEN(PCI_CHAR* szEN)
	{
		SetENLen(strlen(szEN));
		strncpy(m_szEN, szEN, sizeof(m_szEN) - 1);
	}

	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_CHAR*           GetPassword(PCI_VOID) { return m_szPassword; }

	PCI_VOID            SetPassword(PCI_CHAR* szPassword)
	{
		SetPasswordLen(strlen(szPassword));
		strncpy(m_szPassword, szPassword, sizeof(m_szPassword) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	PCI_USHORT          GetRoleLevel(PCI_VOID) { return m_uRoleLevel; }

	PCI_VOID            SetRoleLevel(PCI_USHORT uRoleLevel) { m_uRoleLevel = uRoleLevel; }

	MibaoCard       GetMibaoCard(PCI_VOID) { return m_mibaoCard; }

	PCI_VOID            SetMibaoCard(MibaoCard mibaoCard) { m_mibaoCard = mibaoCard; }

	PCI_CHAR*           GetPasspod(PCI_VOID) { return m_szPasspod; }

	PCI_VOID            SetPassPod(PCI_CHAR* szPasspod) { strncpy(m_szPasspod, szPasspod, sizeof(m_szPasspod) - 1); }

	PCI_CHAR*           GetMACMD5Value(PCI_VOID) { return m_szMACMD5Value; }

	PCI_VOID            SetMACMD5Value(PCI_CHAR* szMACMD5Value) { strncpy(m_szMACMD5Value, szMACMD5Value, sizeof(m_szMACMD5Value) - 1); }

	PCI_CHAR*  GetPCValue(PCI_VOID) { return m_szPCValue; }

	PCI_VOID   SetPCValue(PCI_CHAR* szPCValue)
	{
		SetPCLen(strlen(szPCValue));
		strncpy(m_szPCValue, szPCValue, sizeof(m_szPCValue) - 1);
	}

	PCI_UCHAR			GetEType(PCI_VOID) { return m_uEType; };

	PCI_VOID			SetEType(PCI_UCHAR uEType) { m_uEType = uEType; }

protected:
	PCI_UCHAR			GetENLen(PCI_VOID) { return m_uENLen; }
	PCI_VOID			SetENLen(PCI_UCHAR uENLen) { m_uENLen = uENLen; }
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetPasswordLen(PCI_VOID) { return m_uPasswordLen; }
	PCI_VOID            SetPasswordLen(PCI_UCHAR uPasswordLen) { m_uPasswordLen = uPasswordLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR  GetPCLen(PCI_VOID) { return m_uPCLen; }
	PCI_VOID   SetPCLen(PCI_UCHAR uPCLen) { m_uPCLen = uPCLen; }
private:
	PCI_UCHAR			m_uENLen;
	PCI_CHAR			m_szEN[PCI_MAX_CN_LEN];
	PCI_UCHAR			m_uEType;
	PCI_UCHAR           m_uCNLen;
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uPasswordLen;
	PCI_CHAR            m_szPassword[PCI_MAX_PASSWD_LEN];
	PCI_UCHAR           m_uUserIPLen;
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];

	PCI_USHORT          m_uRoleLevel;
	MibaoCard       m_mibaoCard;
	PCI_CHAR            m_szPasspod[PCI_PASS_POD_LEN];
	PCI_CHAR            m_szMACMD5Value[PCI_MAX_PASSWD_LEN];
	PCI_UCHAR           m_uPCLen;
	PCI_CHAR            m_szPCValue[PCI_PC_VALUE_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBAutoBill.h

description:   每900秒游戏服务器向billing发一次包
 ********************************************************************/

class LBAutoBill : public Packet
{
public:
	LBAutoBill();
	virtual ~LBAutoBill();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_BILLKEEPING;
	}

public:
	PCI_CHAR*           GetUserName(PCI_VOID) { return m_szUserName; }

	PCI_VOID            SetUserName(PCI_CHAR* szUserName)
	{
		SetUserNameLen(strlen(szUserName));
		strncpy(m_szUserName, szUserName, sizeof(m_szUserName) - 1);
	}

	PCI_USHORT          GetRoleLevel(PCI_VOID) { return m_uRoleLevel; }

	PCI_VOID            SetRoleLevel(PCI_USHORT uRoleLevel) { m_uRoleLevel = uRoleLevel; }

protected:
	PCI_UCHAR           GetUserNameLen(PCI_VOID) { return m_uUserNameLen; }
	PCI_VOID            SetUserNameLen(PCI_UCHAR uUserNameLen) { m_uUserNameLen = uUserNameLen; }

private:
	PCI_UCHAR           m_uUserNameLen;
	PCI_CHAR            m_szUserName[PCI_MAX_CN_LEN];
	PCI_USHORT          m_uRoleLevel;
};

/********************************************************************
  created date:
filename:      Packet/LBBillEnd.h

description:   玩家下线发送包
 ********************************************************************/

class LBBillEnd : public Packet
{
public:
	LBBillEnd();
	virtual ~LBBillEnd();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_BILLEND;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_USHORT GetRoleLevel() const { return m_uRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_uRoleLevel = val; }

	PCI_UCHAR GetType() const { return m_uDisconType; }

	PCI_VOID SetType(PCI_UCHAR val) { m_uDisconType = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_uCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_uCnLen = val; }

private:
	PCI_UCHAR   m_uCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_USHORT  m_uRoleLevel;
	PCI_UCHAR   m_uDisconType;
};

/********************************************************************
  created date:
filename:      Packet/LBBillStart.h

description:   玩家上线发送包
 ********************************************************************/

class LBBillStart : public Packet
{
public:
	LBBillStart();
	virtual ~LBBillStart();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_BILLSTART;
	}

public:

	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR *GetRoleName() const { return m_szRoleName; }

	void SetRoleName(PCI_CHAR *val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_ULONG GetGuid() const { return m_uGuid; }

	void SetGuid(PCI_ULONG val) { m_uGuid = val; }

	PCI_USHORT GetRoleLevel() const { return m_nRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_nRoleLevel = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_nRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_nRoleNameLen = val; }

private:

	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];

	PCI_UCHAR   m_nRoleNameLen;
	PCI_CHAR    m_szRoleName[PCI_MAX_ROLE_NAME_LEN];

	PCI_ULONG    m_uGuid;
	PCI_USHORT  m_nRoleLevel;
};

/********************************************************************
  created date:
filename:     Packet/LBCardLotteryCheck.h

description:   充值抽奖发送包
 ********************************************************************/

class LBCardLotteryCheck : public Packet
{
public:
	LBCardLotteryCheck();
	virtual ~LBCardLotteryCheck();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CARDLOTTERYCHECK;
	}

public:
	PCI_CHAR*           GetCardNo(PCI_VOID) { return m_szCardNo; }

	PCI_VOID            SetCardNo(PCI_CHAR* szCardNo)
	{
		SetCardNoLen(strlen(szCardNo));
		strncpy(m_szCardNo, szCardNo, sizeof(m_szCardNo) - 1);
	}

	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCn; }

	PCI_VOID            SetCN(PCI_CHAR* szCn)
	{
		SetCNLen(strlen(szCn));
		strncpy(m_szCn, szCn, sizeof(m_szCn) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR*           GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID            SetRoleName(PCI_CHAR* szRoleName)
	{
		SetRoleNameLen(strlen(szRoleName));
		strncpy(m_szRoleName, szRoleName, sizeof(m_szRoleName) - 1);
	}

protected:
	PCI_UCHAR           GetCardNoLen(PCI_VOID) { return m_uCardNoLen; }
	PCI_VOID            SetCardNoLen(PCI_UCHAR uCardNoLen) { m_uCardNoLen = uCardNoLen; }
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCnLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCnLen) { m_uCnLen = uCnLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR           GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID            SetRoleNameLen(PCI_UCHAR uRoleNameLen) { m_uRoleNameLen = uRoleNameLen; }
private:
	PCI_UCHAR           m_uCardNoLen;                        // 游戏卡号长度
	PCI_CHAR            m_szCardNo[PCI_MAX_CARD_LEN];          // 游戏卡号
	PCI_UCHAR           m_uCnLen;                            // 用户名长度
	PCI_CHAR            m_szCn[PCI_MAX_CN_LEN];                // 用户名
	PCI_UCHAR           m_uUserIPLen;                        // IP长度
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];                // 用户IP
	PCI_UCHAR           m_uRoleNameLen;                      // 角色名长度
	PCI_CHAR            m_szRoleName[PCI_MAX_ROLE_NAME_LEN];   // 角色名
};

/********************************************************************
  created date:
filename:      Packet/LBCardLottery.h

description:   卡抽奖请求包
 ********************************************************************/

class LBCardLottery : public Packet
{
public:
	LBCardLottery();
	virtual ~LBCardLottery();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_CARDLOTTERY;
	}

public:
	// 使用数据接口
	const PCI_CHAR* GetCardNo() const { return m_szCardNo; }

	PCI_VOID SetCardNo(PCI_CHAR* val)
	{
		SetCardNoLen(strlen(val));
		strncpy(m_szCardNo, val, sizeof(m_szCardNo) - 1);
	}

	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID  SetCN(PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, sizeof(m_szUserIP) - 1);
	}

	const PCI_CHAR* GetRoleName() const { return m_szRoleName; }

	PCI_VOID  SetRoleName(PCI_CHAR* val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_ULONG GetRoleGuid() const { return m_uRoleGuid; }

	PCI_VOID SetRoleGuid(PCI_ULONG val) { m_uRoleGuid = val; }

	PCI_USHORT GetRoleLevel() const { return m_uRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_uRoleLevel = val; }

protected:
	PCI_UCHAR GetCardNoLen() const { return m_uCardNoLen; }
	PCI_VOID SetCardNoLen(PCI_UCHAR val) { m_uCardNoLen = val; }
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID  SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_uRoleNameLen = val; }

private:
	// 数据
	PCI_UCHAR      m_uCardNoLen;
	PCI_CHAR       m_szCardNo[PCI_MAX_CARD_LEN];
	PCI_UCHAR      m_uCnLen;
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR      m_uUserIPLen;
	PCI_CHAR       m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR      m_uRoleNameLen;
	PCI_CHAR       m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
	PCI_ULONG       m_uRoleGuid;
	PCI_USHORT     m_uRoleLevel;
};

/********************************************************************
  created date:
filename:      Packet/LBCnLotteryCheck.h

description:   抽奖前背包空间检测接收包
 ********************************************************************/

class LBCnLotteryCheck : public Packet
{
public:
	LBCnLotteryCheck();
	virtual ~LBCnLotteryCheck();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CNLOTTERYCHECK;
	}

public:
	PCI_CHAR*           GetCn(PCI_VOID) { return m_szCn; }

	PCI_VOID            SetCn(PCI_CHAR* szCn)
	{
		SetCnLen(strlen(szCn));
		strncpy(m_szCn, szCn, sizeof(m_szCn) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR*           GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID            SetRoleName(PCI_CHAR* szRoleName)
	{
		SetRoleNameLen(strlen(szRoleName));
		strncpy(m_szRoleName, szRoleName, sizeof(m_szRoleName) - 1);
	}

protected:
	PCI_UCHAR           GetCnLen(PCI_VOID) { return m_uCnLen; }
	PCI_VOID            SetCnLen(PCI_UCHAR uCnLen) { m_uCnLen = uCnLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR           GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID            SetRoleNameLen(PCI_UCHAR uRoleNameLen) { m_uRoleNameLen = uRoleNameLen; }
private:
	PCI_UCHAR           m_uCnLen;                           //用户名长度
	PCI_CHAR            m_szCn[PCI_MAX_CN_LEN];               //用户名
	PCI_UCHAR           m_uUserIPLen;                       //用户IP地址长度
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];               //用户IP地址
	PCI_UCHAR           m_uRoleNameLen;                     //角色名长度
	PCI_CHAR            m_szRoleName[PCI_MAX_ROLE_NAME_LEN];  //角色名
};

/********************************************************************
  created date:
filename:      Packet/LBCnLottery.h

description:   新版账号抽奖
 ********************************************************************/

class LBCnLottery : public Packet
{
public:
	LBCnLottery();
	virtual ~LBCnLottery();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CNLOTTERY;
	}
public:
	// 使用数据接口
	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID SetCN(PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, sizeof(m_szUserIP) - 1);
	}

	const PCI_CHAR* GetRoleName() const { return m_szRoleName; }

	PCI_VOID  SetRoleName(PCI_CHAR* val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_ULONG GetRoleGuid() const { return m_uRoleGuid; }

	PCI_VOID SetRoleGuid(PCI_ULONG val) { m_uRoleGuid = val; }

	PCI_USHORT GetRoleLevel() const { return m_uRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_uRoleLevel = val; }

	const PCI_CHAR* GetSerialNo() const { return m_szSerialNo; }

	PCI_VOID SetSerialNo(PCI_CHAR* val) { strncpy(m_szSerialNo, val, sizeof(m_szSerialNo) - 1); }

protected:
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_uRoleNameLen = val; }
private:
	PCI_UCHAR      m_uCnLen;                             //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                 //用户名字符串
	PCI_UCHAR      m_uUserIPLen;                         //用户IP地址长度
	PCI_CHAR       m_szUserIP[PCI_MAX_IP_LEN];                 //用户IP地址
	PCI_UCHAR      m_uRoleNameLen;                       //角色名称长度
	PCI_CHAR       m_szRoleName[PCI_MAX_ROLE_NAME_LEN];    //角色名称
	PCI_ULONG       m_uRoleGuid;                     //角色ID
	PCI_USHORT     m_uRoleLevel;                         //角色级别
	PCI_CHAR       m_szSerialNo[PCI_MAX_SERIAL_NO_LEN];    //流水号String, 定长21字节,服务器组号+随机码,必须唯一
};

/********************************************************************
  created date:
filename:      Packet/LBCnPoint.h

description:   用户点数查询协议
 ********************************************************************/

class LBCnPoint : public Packet
{
public:
	LBCnPoint();
	virtual ~LBCnPoint();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CNPOINT;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_CHAR * GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID SetUserIP(const PCI_CHAR* value)
	{
		SetUserIPLen(strlen(value));
		strncpy(m_szUserIP, value, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR * GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID SetRoleName(const PCI_CHAR * value)
	{
		SetRoleNameLen(strlen(value));
		strncpy(m_szRoleName, value, sizeof(m_szRoleName) - 1);
	}

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }
	PCI_UCHAR GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR value) { m_uUserIPLen = value; }
	PCI_UCHAR GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR value) { m_uRoleNameLen = value; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR    m_uUserIPLen;
	PCI_CHAR     m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR    m_uRoleNameLen;
	PCI_CHAR     m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBConfirmProtocol.h

description:   确认畅游协议
 ********************************************************************/

class LBConfirmProtocol : public Packet
{
public:
	LBConfirmProtocol();
	virtual ~LBConfirmProtocol();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CONFIRMPROTOCOL;
	}

public:
	PCI_CHAR*           GetUserName(PCI_VOID) { return m_szUserName; }

	PCI_VOID            SetUserName(PCI_CHAR* szUserName)
	{
		SetUserNameLen(strlen(szUserName));
		strncpy(m_szUserName, szUserName, sizeof(m_szUserName) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

protected:
	PCI_UCHAR           GetUserNameLen(PCI_VOID) { return m_uUserNameLen; }
	PCI_VOID            SetUserNameLen(PCI_UCHAR uUserNameLen) { m_uUserNameLen = uUserNameLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }

private:
	PCI_UCHAR           m_uUserNameLen;
	PCI_CHAR            m_szUserName[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uUserIPLen;
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBConnect.h

description:   Login连接Billing服务器请求包
 ********************************************************************/

class LBConnect : public Packet
{
public:
	LBConnect();
	virtual ~LBConnect();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_CONNECT;
	}

public:
	PCI_USHORT GetGroup() const { return m_uGroup; }
	PCI_VOID SetGroup(PCI_USHORT val) { m_uGroup = val; }

	PCI_USHORT GetZone() const { return m_uZone; }
	PCI_VOID SetZone(PCI_USHORT val) { m_uZone = val; }

private:
	PCI_USHORT        m_uZone;
	PCI_USHORT        m_uGroup;
};

/********************************************************************
  created date:
filename:      Packet/LBDisableUser.h

description:   用户停权解权协议
 ********************************************************************/

class LBDisableUser : public Packet
{
public:
	LBDisableUser();
	virtual ~LBDisableUser();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_DISABLEUSER;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_CHAR * GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID SetRoleName(const PCI_CHAR * value)
	{
		SetRoleNameLen(strlen(value));
		strncpy(m_szRoleName, value, sizeof(m_szRoleName) - 1);
	}

	PCI_CHAR * GetIP(PCI_VOID) { return m_szIP; }

	PCI_VOID SetIP(const PCI_CHAR* value)
	{
		SetIPLen(strlen(value));
		strncpy(m_szIP, value, sizeof(m_szIP) - 1);
	}

	PCI_ULONG GetGuid(PCI_VOID) { return m_uGuid; }

	PCI_VOID SetGuid(PCI_ULONG value) { m_uGuid = value; }

	PCI_USHORT GetLevel(PCI_VOID) { return m_uRoleLevel; }

	PCI_VOID SetLevel(PCI_USHORT value) { m_uRoleLevel = value; }

	PCI_UCHAR GetStatus(PCI_VOID) { return m_uStatus; }

	PCI_VOID SetStatus(PCI_UCHAR value) { m_uStatus = value; }

	PCI_CHAR * GetReason(PCI_VOID) { return m_szReason; }

	PCI_VOID SetReason(const PCI_CHAR * value)
	{
		SetReasonLen(strlen(value));
		strncpy(m_szReason, value, sizeof(m_szReason) - 1);
	}

	PCI_CHAR * GetOperateCN(PCI_VOID) { return m_szOperateCN; }

	PCI_VOID SetOperateCN(const PCI_CHAR * value)
	{
		SetOperateCNLEN(strlen(value));
		strncpy(m_szOperateCN, value, sizeof(m_szOperateCN) - 1);
	}

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }
	PCI_UCHAR GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR value) { m_uRoleNameLen = value; }
	PCI_UCHAR GetIPLen(PCI_VOID) { return m_uIPLen; }
	PCI_VOID SetIPLen(PCI_UCHAR value) { m_uIPLen = value; }
	PCI_UCHAR GetReasonLen(PCI_VOID) { return m_uReasonLen; }
	PCI_VOID SetReasonLen(PCI_UCHAR value) { m_uReasonLen = value; }
	PCI_UCHAR GetOperateCNLen(PCI_VOID) { return m_uOperateCNLen; }
	PCI_VOID SetOperateCNLEN(PCI_UCHAR value) { m_uOperateCNLen = value; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR    m_uRoleNameLen;
	PCI_CHAR     m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
	PCI_UCHAR    m_uIPLen;
	PCI_CHAR     m_szIP[PCI_MAX_IP_LEN];
	PCI_ULONG     m_uGuid;
	PCI_USHORT   m_uRoleLevel;
	PCI_UCHAR    m_uStatus;
	PCI_UCHAR    m_uReasonLen;
	PCI_CHAR     m_szReason[PCI_MAX_REASON_LEN];
	PCI_UCHAR    m_uOperateCNLen;
	PCI_CHAR     m_szOperateCN[PCI_MAX_ROLE_NAME_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBExchangeYB.h

description:   用户消耗元宝记录协议
 ********************************************************************/

class LBExchangeYB : public Packet
{
public:
	LBExchangeYB();
	virtual ~LBExchangeYB();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_EXCHANGEYB;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_CHAR * GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID SetUserIP(const PCI_CHAR* value)
	{
		SetUserIPLen(strlen(value));
		strncpy(m_szUserIP, value, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR * GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID SetRoleName(const PCI_CHAR * value)
	{
		SetRoleNameLen(strlen(value));
		strncpy(m_szRoleName, value, sizeof(m_szRoleName) - 1);
	}

	PCI_CHAR* GetSerialNo(PCI_VOID) { return m_szSerialNo; }

	PCI_VOID SetSerialNo(const PCI_CHAR* value) { strncpy(m_szSerialNo, value, sizeof(m_szSerialNo) - 1); }

	PCI_CHAR*	GetGoods() { return m_szGoods; }

	PCI_VOID SetGoods(PCI_CHAR* val)
	{
		SetGoodsLen(strlen(val));
		strncpy(m_szGoods, val, sizeof(m_szGoods) - 1);
	}

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }
	PCI_UCHAR GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR value) { m_uUserIPLen = value; }
	PCI_UCHAR GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR value) { m_uRoleNameLen = value; }
	PCI_UCHAR GetGoodsLen() const { return m_uGoodsLen; }
	PCI_VOID SetGoodsLen(PCI_UCHAR val) { m_uGoodsLen = val; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR    m_uUserIPLen;
	PCI_CHAR     m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR    m_uRoleNameLen;
	PCI_CHAR     m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
	PCI_CHAR     m_szSerialNo[PCI_MAX_SERIAL_NO_LEN];    //流水号String, 定长21字节,服务器组号+随机码,必须唯一
	PCI_UCHAR	 m_uGoodsLen;
	PCI_CHAR	 m_szGoods[PCI_MAX_GOODS_LEN];
};

class LBImmeLotteryConfirm : public Packet
{
public:
	LBImmeLotteryConfirm(PCI_VOID);
	virtual ~LBImmeLotteryConfirm(PCI_VOID);

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_IMMELOTTERYTYPECONFIRM;
	}

public:
	const PCI_CHAR* GetCN() { return m_szCN; }

	PCI_VOID SetCN(PCI_CHAR *val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCN, val, sizeof(m_szCN) - 1);
	}

	const PCI_CHAR* GetGameName()const { return m_szGameName; }

	PCI_VOID SetGameName(PCI_CHAR *val)
	{
		SetGameNameLen(strlen(val));
		strncpy(m_szGameName, val, sizeof(m_szGameName) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID SetUserIP(PCI_CHAR *val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR  GetCategory()const { return m_DrawCategory; }

	PCI_VOID SetCategory(PCI_CHAR var) { m_DrawCategory = var; }

	const PCI_CHAR* GetDrawType() const { return m_szDrawType; }

	PCI_VOID SetDrawType(PCI_CHAR *var)
	{
		SetDrawTypeLen(strlen(var));
		strncpy(m_szDrawType, var, sizeof(m_szDrawType) - 1);
	}

	const PCI_CHAR* GetSN() const { return m_szSN; }

	PCI_VOID SetSN(PCI_CHAR*var) { strncpy(m_szSN, var, sizeof(m_szSN) - 1); }

	const PCI_ULONG  GetRoleGUID() const { return m_RoleGUID; }

	PCI_VOID SetRoleGUID(PCI_ULONG val) { m_RoleGUID = val; }

protected:
	PCI_UCHAR GetCNLen() const { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCNLen = val; }
	PCI_UCHAR GetGameNameLen() { return m_uGameNameLen; }
	PCI_VOID SetGameNameLen(PCI_UCHAR val) { m_uGameNameLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetDrawTypeLen() const { return m_uDrawTypeLen; }
	PCI_VOID SetDrawTypeLen(PCI_UCHAR val) { m_uDrawTypeLen = val; }
private:

	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];

	PCI_UCHAR    m_uGameNameLen;
	PCI_CHAR     m_szGameName[PCI_MAX_GAMENAME_LEN];

	PCI_UCHAR    m_uDrawTypeLen;
	PCI_CHAR     m_szDrawType[PCI_MAX_DRAWTYPE_LEN];

	PCI_CHAR     m_DrawCategory;

	PCI_UCHAR    m_uUserIPLen;
	PCI_CHAR     m_szUserIP[PCI_MAX_IP_LEN];

	PCI_CHAR     m_szSN[PCI_MAX_SERIAL_NO_LEN];
	PCI_ULONG  m_RoleGUID;
};

class LBImmeLotterySpecify : public Packet
{
public:
	LBImmeLotterySpecify(PCI_VOID);
	virtual ~LBImmeLotterySpecify(PCI_VOID);

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_IMMELOTTERYTYPESPECIFY;
	}

public:

	const PCI_CHAR* GetCN() { return m_szCN; }

	PCI_VOID  SetCN(PCI_CHAR* var)
	{
		SetCNLen(strlen(var));
		strncpy(m_szCN, var, sizeof(m_szCN) - 1);
	}

	const PCI_CHAR* GetGameName()const { return m_szGameName; }

	PCI_VOID  SetGameName(PCI_CHAR* var)
	{
		SetGameNameLen(strlen(var));
		strncpy(m_szGameName, var, sizeof(m_szGameName) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* var)
	{
		SetUserIPLen(strlen(var));
		strncpy(m_szUserIP, var, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR  GetCategory()const { return m_DrawCategory; }

	PCI_VOID  SetCategory(PCI_CHAR var) { m_DrawCategory = var; }

	const PCI_CHAR* GetDrawType() const { return m_szDrawType; }

	PCI_VOID SetDrawType(PCI_CHAR* var)
	{
		SetDrawTypeLen(strlen(var));
		strncpy(m_szDrawType, var, sizeof(m_szDrawType) - 1);
	}

protected:
	PCI_UCHAR GetGameNameLen() { return m_uGameNameLen; }
	PCI_VOID  SetGameNameLen(PCI_UCHAR var) { m_uGameNameLen = var; }
	PCI_UCHAR GetDrawTypeLen() const { return m_uDrawTypeLen; }
	PCI_VOID  SetDrawTypeLen(PCI_UCHAR var) { m_uDrawTypeLen = var; }
	PCI_UCHAR GetCNLen() const { return m_uCNLen; }
	PCI_VOID  SetCNLen(PCI_UCHAR var) { m_uCNLen = var; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID  SetUserIPLen(PCI_UCHAR var) { m_uUserIPLen = var; }
private:

	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];

	PCI_UCHAR    m_uGameNameLen;
	PCI_CHAR     m_szGameName[PCI_MAX_GAMENAME_LEN];

	PCI_UCHAR    m_uDrawTypeLen;
	PCI_CHAR     m_szDrawType[PCI_MAX_DRAWTYPE_LEN];

	PCI_CHAR     m_DrawCategory;

	PCI_UCHAR    m_uUserIPLen;
	PCI_CHAR     m_szUserIP[PCI_MAX_IP_LEN];
};

class LBImmeLotteryTypeCheck : public Packet
{
public:
	LBImmeLotteryTypeCheck(PCI_VOID);
	virtual ~LBImmeLotteryTypeCheck(PCI_VOID);

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_IMMELOTTERYTYPECHECK;
	}

public:
	const PCI_CHAR* GetCN() { return m_szCN; }

	PCI_VOID SetCN(PCI_CHAR* var)
	{
		SetCNLen(strlen(var));
		strncpy(m_szCN, var, sizeof(m_szCN) - 1);
	}

	const PCI_CHAR* GetGameName()const { return m_szGameName; }

	PCI_VOID SetGameName(PCI_CHAR* var)
	{
		SetGameNameLen(strlen(var));
		strncpy(m_szGameName, var, sizeof(m_szGameName) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* var)
	{
		SetUserIPLen(strlen(var));
		strncpy(m_szUserIP, var, sizeof(m_szUserIP) - 1);
	}

protected:
	PCI_UCHAR GetCNLen() const { return m_uCNLen; }
	PCI_VOID  SetCNLen(PCI_UCHAR var) { m_uCNLen = var; }
	PCI_UCHAR GetGameNameLen() { return m_uGameNameLen; }
	PCI_VOID  SetGameNameLen(PCI_UCHAR var) { m_uGameNameLen = var; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID  SetUserIPLen(PCI_UCHAR var) { m_uUserIPLen = var; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];

	PCI_UCHAR    m_uGameNameLen;
	PCI_CHAR     m_szGameName[PCI_MAX_GAMENAME_LEN];

	PCI_UCHAR    m_uUserIPLen;
	PCI_CHAR     m_szUserIP[PCI_MAX_IP_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBKeepAlive.h
author:

description:   Login与Billing保持连接类
 ********************************************************************/

class LBKeepAlive : public Packet
{
public:
	LBKeepAlive();
	virtual ~LBKeepAlive();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_KEEPLIVE;
	}

public:
	//使用数据接口
	PCI_USHORT GetGroup() { return m_uGroup; }

	PCI_VOID SetGroup(PCI_USHORT uGroup) { m_uGroup = uGroup; }

	PCI_USHORT GetZone() { return m_uZone; }

	PCI_VOID SetZone(PCI_USHORT uZone) { m_uZone = uZone; }

	PCI_USHORT GetUsers() { return m_uUsers; }

	PCI_VOID SetUsers(PCI_USHORT uUsers) { m_uUsers = uUsers; }

	PCI_CHAR*  GetMemo(PCI_VOID) { return m_szMemo; }

	PCI_VOID   SetMemo(PCI_CHAR* szMemo)
	{
		SetMemoLen(strlen(szMemo));
		strncpy(m_szMemo, szMemo, sizeof(m_szMemo) - 1);
	}

protected:
	PCI_UCHAR  GetMemoLen() { return m_uMemoLen; }
	PCI_VOID SetMemoLen(PCI_UCHAR uLen) { m_uMemoLen = uLen; }
private:
	/* 数据 */
	PCI_USHORT    m_uGroup;
	PCI_USHORT    m_uZone;
	PCI_USHORT    m_uUsers;
	PCI_UCHAR     m_uMemoLen;
	PCI_CHAR      m_szMemo[PCI_MAX_MEMO_LEN];
};

/********************************************************************
  created date:
filename:      Packet/LBMiBaoCardChek.h

description:  密保卡验证协议包
 ********************************************************************/

class LBMiBaoCardChek : public Packet
{
public:
	LBMiBaoCardChek();
	virtual ~LBMiBaoCardChek();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_MIBAOCARDCHED;
	}

public:
	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	MibaoCard       GetMibaoCard(PCI_VOID) { return m_mibaoCard; }

	PCI_VOID            SetMibaoCard(MibaoCard mibaoCard)
	{
		SetMiBaoPosLen(6);
		SetMiBaoValueLen(6);
		m_mibaoCard = mibaoCard;
	}

	PCI_CHAR            GetMibaoCardBind(PCI_VOID) { return m_mibaoCardBind; }

	PCI_VOID            SetMibaoCardBind(PCI_CHAR mibaoCardBind) { m_mibaoCardBind = mibaoCardBind; }

	PCI_UCHAR			GetOperateType(PCI_VOID) { return m_uType; }

	PCI_VOID			SetOperateType(PCI_UCHAR uType) { m_uType = uType; }

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR			GetMiBaoPosLen(PCI_VOID) { return m_uCardPosLen; }
	PCI_VOID			SetMiBaoPosLen(PCI_UCHAR uMiBaoPosLen) { m_uCardPosLen = uMiBaoPosLen; }
	PCI_UCHAR			GetMiBaoValueLen(PCI_VOID) { return m_uCardValueLen; }
	PCI_VOID			SetMiBaoValueLen(PCI_UCHAR uMiBaoValueLen) { m_uCardValueLen = uMiBaoValueLen; }
private:
	PCI_UCHAR           m_uCNLen;
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uUserIPLen;
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR			m_uCardPosLen;
	PCI_UCHAR			m_uCardValueLen;
	MibaoCard       m_mibaoCard;
	PCI_CHAR            m_mibaoCardBind;
	PCI_UCHAR			m_uType;
};

/********************************************************************
  created date:
filename:      Packet/LBOTPCheck.h
author:

description:   Passpodcheck
 ********************************************************************/

class LBOTPCheck : public Packet
{
public:
	LBOTPCheck();
	virtual ~LBOTPCheck();

	/* 公用继承接口 */
	virtual PCI_BOOL Write(OutputStream  & oStream) const;
	virtual PCI_BOOL Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_OPTCHECKED;
	}

public:
	// 使用数据接口

	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR* val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, PCI_MAX_CN_LEN);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, PCI_MAX_IP_LEN);
	}

	const PCI_CHAR* GetRoleName() const { return m_szRoleName; }

	PCI_VOID  SetRoleName(PCI_CHAR* val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, PCI_MAX_ROLE_NAME_LEN);
	}

	const PCI_CHAR* GetOTP() const { return m_szOTP; }

	PCI_VOID  SetOTP(PCI_CHAR* val)
	{
		SetOTPLen(strlen(val));
		strncpy(m_szOTP, val, PCI_MAX_PASS_LEN);
	}

protected:
	PCI_UCHAR GetCnLen() const { return m_uCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_uRoleNameLen = val; }
	PCI_UCHAR GetOTPLen() const { return m_uOTPLen; }
	PCI_VOID SetOTPLen(PCI_UCHAR val) { m_uOTPLen = val; }
private:
	// 数据
	PCI_UCHAR      m_uCnLen;                             //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN + 1];                 //用户名字符串
	PCI_UCHAR      m_uRoleNameLen;                       //角色名称长度
	PCI_CHAR       m_szRoleName[PCI_MAX_ROLE_NAME_LEN + 1];    //角色名称
	PCI_UCHAR      m_uUserIPLen;                         //用户IP地址长度
	PCI_CHAR       m_szUserIP[PCI_MAX_IP_LEN + 1];                 //用户IP地址
	PCI_UCHAR	   m_uOTPLen;                            //硬件密保长度
	PCI_CHAR	   m_szOTP[PCI_MAX_PASS_LEN + 1];				 //硬件密报验证码
};

/********************************************************************
  created date:
filename:      Packet/LBPhoneLocked.h

description:   电话密宝
 ********************************************************************/

class LBPhoneLocked : public Packet
{
public:
	LBPhoneLocked();
	virtual ~LBPhoneLocked();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_PHONELOCKED;
	}

public:
	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID SetCN(PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, sizeof(m_szUserIP) - 1);
	}

	const PCI_CHAR* GetRoleName() const { return m_szRoleName; }

	PCI_VOID  SetRoleName(PCI_CHAR* val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_UCHAR GetType() const { return m_uType; }
	PCI_VOID  SetType(PCI_UCHAR val) { m_uType = val; }

protected:
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_uRoleNameLen = val; }
private:
	PCI_UCHAR      m_uCnLen;
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR      m_uRoleNameLen;
	PCI_CHAR       m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
	PCI_UCHAR      m_uUserIPLen;
	PCI_CHAR       m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR	   m_uType;
};

/********************************************************************
  created date:
filename:      Packet/LBServBuyLog.h
author:

description:   服务器元宝交易请求包
 ********************************************************************/

class LBServBuyLog : public Packet
{
public:
	LBServBuyLog();
	virtual ~LBServBuyLog();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_BUYLOG;
	}

public:
	PCI_CHAR*           GetSN(PCI_VOID) { return m_szSN; }

	PCI_VOID            SetSN(PCI_CHAR* szSN) { memcpy(m_szSN, szSN, (PCI_MAX_SERIAL_NO_LEN - 1) * sizeof(PCI_CHAR)); m_szSN[PCI_MAX_SERIAL_NO_LEN - 1] = '\0'; }

	PCI_USHORT          GetZone(PCI_VOID) { return m_uZone; }

	PCI_VOID            SetZone(PCI_USHORT uZone) { m_uZone = uZone; }

	PCI_UINT            GetGroup(PCI_VOID) { return m_uGroup; }

	PCI_VOID            SetGroup(PCI_UINT uWorld) { m_uGroup = uWorld; }

	PCI_UINT            GetSceneID(PCI_VOID) { return m_uSceneID; }

	PCI_VOID            SetSceneID(PCI_UINT uSceneID) { m_uSceneID = uSceneID; }

	PCI_ULONG       GetGUID(PCI_VOID) { return m_uGUID; }

	PCI_VOID            SetGUID(PCI_ULONG uGUID) { m_uGUID = uGUID; }

	PCI_UINT            GetTransTime(PCI_VOID) { return m_uTransTime; }

	PCI_VOID            SetTransTime(PCI_UINT uTransTime) { m_uTransTime = uTransTime; }

	PCI_UINT            GetJewelNum(PCI_VOID) { return m_uJewelNum; }

	PCI_VOID            SetJewelNum(PCI_UINT uJewelNum) { m_uJewelNum = uJewelNum; }

	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_CHAR*           GetRoleName(PCI_VOID) { return m_szRoleName; }

	PCI_VOID            SetRoleName(PCI_CHAR* szRoleName)
	{
		SetRoleNameLen(strlen(szRoleName));
		strncpy(m_szRoleName, szRoleName, sizeof(m_szRoleName) - 1);
	}

	PCI_USHORT          GetUserLevel(PCI_VOID) { return m_uUserLevel; }

	PCI_VOID            SetUserLevel(PCI_USHORT uUserLevel) { m_uUserLevel = uUserLevel; }

	PCI_CHAR*           GetUserIP(PCI_VOID) { return m_szUserIP; }

	PCI_VOID            SetUserIP(PCI_CHAR* szUserIP)
	{
		SetUserIPLen(strlen(szUserIP));
		strncpy(m_szUserIP, szUserIP, sizeof(m_szUserIP) - 1);
	}

	PCI_CHAR*           GetRemark(PCI_VOID) { return m_szRemark; }

	PCI_VOID            SetRemark(PCI_CHAR* szRemark)
	{
		SetRemarkLen(strlen(szRemark));
		strncpy(m_szRemark, szRemark, sizeof(m_szRemark) - 1);
	}

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetRoleNameLen(PCI_VOID) { return m_uRoleNameLen; }
	PCI_VOID            SetRoleNameLen(PCI_UCHAR uRoleNameLen) { m_uRoleNameLen = uRoleNameLen; }
	PCI_UCHAR           GetUserIPLen(PCI_VOID) { return m_uUserIPLen; }
	PCI_VOID            SetUserIPLen(PCI_UCHAR uUserIPLen) { m_uUserIPLen = uUserIPLen; }
	PCI_UCHAR           GetRemarkLen(PCI_VOID) { return m_uRemarkLen; }
	PCI_VOID            SetRemarkLen(PCI_UCHAR uRemarkLen) { m_uRemarkLen = uRemarkLen; }
private:
	PCI_CHAR            m_szSN[PCI_MAX_SERIAL_NO_LEN]; // 流水号
	PCI_USHORT          m_uZone;                     // 服务器区ID
	PCI_USHORT          m_uGroup;                    // 服务器组ID
	PCI_UINT            m_uSceneID;                  // 场景ID
	PCI_ULONG       m_uGUID;                     // 玩家ID
	PCI_UINT            m_uTransTime;                // 购买时间
	PCI_UINT            m_uJewelNum;                 // 元宝数量

	PCI_UCHAR           m_uCNLen;                    // 用户名长度
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];        // 用户名
	PCI_UCHAR           m_uRoleNameLen;              // 角色名长度
	PCI_CHAR            m_szRoleName[PCI_MAX_ROLE_NAME_LEN];   // 角色名
	PCI_USHORT          m_uUserLevel;                // 用户级别
	PCI_UCHAR           m_uUserIPLen;                // 用户IP地址长度
	PCI_CHAR            m_szUserIP[PCI_MAX_IP_LEN];        // 用户IP地址
	PCI_UCHAR           m_uRemarkLen;                // 备注长度
	PCI_CHAR            m_szRemark[PCI_MAX_REMARK_LEN];    // 备注
};

/********************************************************************
  created date:
filename:      Packet/LBServRestart.h

description:   服务器重启请求包
 ********************************************************************/

class LBServRestart : public Packet
{
public:
	LBServRestart();
	virtual ~LBServRestart();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_LB_KICKALL;
	}

public:
	PCI_USHORT          GetZone(PCI_VOID) { return m_uZone; }
	PCI_VOID            SetZone(PCI_USHORT uZone) { m_uZone = uZone; }

	PCI_USHORT          GetGroup(PCI_VOID) { return m_uGroup; }
	PCI_VOID            SetGroup(PCI_USHORT uGroup) { m_uGroup = uGroup; }

private:
	PCI_USHORT          m_uZone;
	PCI_USHORT          m_uGroup;
};

/********************************************************************
  created date:
filename:      Packet/LBSwitchRole.h

description:   玩家切换角色包
 ********************************************************************/

class LBSwitchRole : public Packet
{
public:
	LBSwitchRole();
	virtual ~LBSwitchRole();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_SWITCHROLE;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_USHORT GetRoleLevel() const { return m_uRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_uRoleLevel = val; }

	PCI_UCHAR GetType() const { return m_uSwitchType; }

	PCI_VOID SetType(PCI_UCHAR val) { m_uSwitchType = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_uCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_uCnLen = val; }
private:
	PCI_UCHAR   m_uCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_USHORT  m_uRoleLevel;
	PCI_UCHAR   m_uSwitchType;
};

class LBSwitchServer : public Packet
{
public:
	LBSwitchServer();
	virtual ~LBSwitchServer();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_SWITCHSERVER;
	}

public:

	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR *GetRoleName() const { return m_szRoleName; }

	PCI_VOID SetRoleName(PCI_CHAR *val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_ULONG GetGuid() const { return m_uGuid; }

	PCI_VOID SetGuid(PCI_ULONG val) { m_uGuid = val; }

	PCI_USHORT GetRoleLevel() const { return m_nRoleLevel; }

	PCI_VOID SetRoleLevel(PCI_USHORT val) { m_nRoleLevel = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_nRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_nRoleNameLen = val; }
private:

	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];

	PCI_UCHAR   m_nRoleNameLen;
	PCI_CHAR    m_szRoleName[PCI_MAX_ROLE_NAME_LEN];

	PCI_ULONG    m_uGuid;
	PCI_USHORT  m_nRoleLevel;
};

/********************************************************************
  created date:
filename:      Packet/LBUnivCardLottery.h

description:   通用卡抽奖请求包
 ********************************************************************/

class LBUnivCardLottery : public Packet
{
public:
	LBUnivCardLottery();
	virtual ~LBUnivCardLottery();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_UNIVCARDLOTTERY;
	}

public:
	// 使用数据接口
	const PCI_CHAR* GetCardNo() const { return m_szCardNo; }

	PCI_VOID SetCardNo(PCI_CHAR* val)
	{
		SetCardNoLen(strlen(val));
		strncpy(m_szCardNo, val, sizeof(m_szCardNo) - 1);
	}

	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID  SetCN(PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID  SetUserIP(PCI_CHAR* val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, sizeof(m_szUserIP) - 1);
	}

	const PCI_CHAR* GetRoleName() const { return m_szRoleName; }

	PCI_VOID  SetRoleName(PCI_CHAR* val)
	{
		SetRoleNameLen(strlen(val));
		strncpy(m_szRoleName, val, sizeof(m_szRoleName) - 1);
	}

	PCI_ULONG GetRoleGUID() const { return m_RoleGUID; }

	PCI_VOID SetRoleGUID(PCI_ULONG val) { m_RoleGUID = val; }

	PCI_UCHAR GetGameType() const { return m_uGameType; }

	PCI_VOID SetGameType(PCI_UCHAR val) { m_uGameType = val; }

	PCI_UCHAR GetCardType() const { return m_uCardType; }

	PCI_VOID SetCardType(PCI_UCHAR val) { m_uCardType = val; }

protected:
	PCI_UCHAR GetCardNoLen() const { return m_uCardNoLen; }
	PCI_VOID SetCardNoLen(PCI_UCHAR val) { m_uCardNoLen = val; }
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_uUserIPLen; }
	PCI_VOID  SetUserIPLen(PCI_UCHAR val) { m_uUserIPLen = val; }
	PCI_UCHAR GetRoleNameLen() const { return m_uRoleNameLen; }
	PCI_VOID SetRoleNameLen(PCI_UCHAR val) { m_uRoleNameLen = val; }
private:
	// 数据
	PCI_UCHAR      m_uCardNoLen;
	PCI_CHAR       m_szCardNo[PCI_MAX_CARD_LEN];
	PCI_UCHAR      m_uCnLen;
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR      m_uUserIPLen;
	PCI_CHAR       m_szUserIP[PCI_MAX_IP_LEN];
	PCI_UCHAR      m_uRoleNameLen;
	PCI_CHAR       m_szRoleName[PCI_MAX_ROLE_NAME_LEN];
	PCI_ULONG  m_RoleGUID;
	PCI_UCHAR	   m_uGameType;
	PCI_UCHAR	   m_uCardType;
};

/********************************************************************
  created date:
filename:      Packet/LBUserInfoCollect.h
author:
description:   用户信息收集包
 ********************************************************************/

 // 用户信息收集协议 0xBB
 // 类型代码
 // 抱团信息查询协议代码为0xBB。
 // 功能
 // 游戏传过来的玩家个人信息打印日志

#define MAX_USERINFO_LEN 500

class LBUserInfoCollect : public Packet
{
public:
	LBUserInfoCollect();
	virtual ~LBUserInfoCollect();

	/* 公用继承接口 */
	virtual PCI_BOOL Write(OutputStream   & oStream) const;
	virtual PCI_BOOL Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_LB_USERINFOCOLLECT;
	}

	virtual PCI_UINT GetPacketSize() const
	{
		PCI_UINT uSize = sizeof(PCI_UCHAR) + sizeof(PCI_CHAR)*m_nSvrIPLen + sizeof(PCI_UCHAR)
			+ sizeof(PCI_CHAR)*m_nCnLen + sizeof(PCI_UCHAR)
			+ sizeof(PCI_CHAR)*m_nUserIPLen + sizeof(PCI_USHORT)
			+ sizeof(PCI_CHAR)*m_nUserInfoLen;
		return uSize;
	}

public:
	//使用数据接口
	//游戏服务器IP
	const PCI_CHAR* GetSvrIP() const { return m_szSvrIP; }

	PCI_VOID SetSvrIP(const PCI_CHAR *val)
	{
		SetSvrIPLen(strlen(val));
		strncpy(m_szSvrIP, val, PCI_MAX_IP_LEN);
	}

	//账号
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, PCI_MAX_CN_LEN);
	}

	//用户IP
	const PCI_CHAR* GetUserIP() const { return m_szUserIP; }

	PCI_VOID SetUserIP(PCI_CHAR *val)
	{
		SetUserIPLen(strlen(val));
		strncpy(m_szUserIP, val, PCI_MAX_IP_LEN);
	}
	//用户信息
	const PCI_CHAR* GetUserInfo()const { return m_szUserInfo; }

	PCI_VOID SetUserInfo(PCI_CHAR *val)
	{
		SetUserInfoLen(strlen(val));
		strncpy(m_szUserInfo, val, MAX_USERINFO_LEN);
	}
protected:
	PCI_UCHAR GetSvrIPLen() const { return m_nSvrIPLen; }
	PCI_VOID SetSvrIPLen(PCI_UCHAR val) { m_nSvrIPLen = val; }
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }
	PCI_UCHAR GetUserIPLen() const { return m_nUserIPLen; }
	PCI_VOID SetUserIPLen(PCI_UCHAR val) { m_nUserIPLen = val; }
	PCI_USHORT GetUserInfoLen() const { return m_nUserInfoLen; }
	PCI_VOID SetUserInfoLen(PCI_USHORT val) { m_nUserInfoLen = val; }

private:
	/* 数据 */
	PCI_UCHAR   m_nSvrIPLen;
	PCI_CHAR    m_szSvrIP[PCI_MAX_IP_LEN + 1];

	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN + 1];

	PCI_UCHAR   m_nUserIPLen;
	PCI_CHAR    m_szUserIP[PCI_MAX_IP_LEN + 1];

	PCI_USHORT   m_nUserInfoLen;
	PCI_CHAR    m_szUserInfo[MAX_USERINFO_LEN + 1];
};

#endif
