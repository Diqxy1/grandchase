#ifndef __PACKETRES_H__
#define __PACKETRES_H__

#include "public.h"

class BLAuthUserFBRet : public Packet
{
public:
	BLAuthUserFBRet();
	virtual  ~BLAuthUserFBRet();

	/* 共用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETAUTHUSERFB;
	}

public:
	/* 设置、获取数据接口 */
	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_UCHAR           GetAuthRet(PCI_VOID) { return m_uAuthRet; }

	PCI_VOID            SetAuthRet(PCI_UCHAR uAuthRet) { m_uAuthRet = uAuthRet; }

	PCI_CHAR            GetForbiddenInf(PCI_VOID) { return m_forbiddenInf; }

	PCI_VOID            SetForbiddenInf(PCI_CHAR forbiddenInf) { m_forbiddenInf = forbiddenInf; }

	PCI_INT             GetOnlineTime(PCI_VOID) { return m_nOnlineTime; }

	PCI_VOID            SetOnlineTime(PCI_INT nOnlineTime) { m_nOnlineTime = nOnlineTime; }

	PCI_INT             GetOutlintTime(PCI_VOID) { return m_nOutlineTime; }

	PCI_VOID            SetOutlintTime(PCI_INT nOutlineTime) { m_nOutlineTime = nOutlineTime; }

	PCI_CHAR            GetMobileBindInf(PCI_VOID) { return m_mobileBindInf; }

	PCI_VOID            SetMobileBindInf(PCI_CHAR mobileBindInf) { m_mobileBindInf = mobileBindInf; }

	PCI_CHAR            GetIPBindInf(PCI_VOID) { return m_IPBindInf; }

	PCI_VOID            SetIPBindInf(PCI_CHAR IPBindInf) { m_IPBindInf = IPBindInf; }

	PCI_CHAR            GetCardBindInf(PCI_VOID) { return m_cardBindInf; }

	PCI_VOID            SetCardBindInf(PCI_CHAR cardBindInf) { m_cardBindInf = cardBindInf; }

	PCI_CHAR            GetMACBindInf(PCI_VOID) { return m_MACBindInf; }

	PCI_VOID            SetMACBindInf(PCI_CHAR MACBindInf) { m_MACBindInf = MACBindInf; }

	PCI_CHAR            GetBeAuthedInf(PCI_VOID) { return m_beAuthedInf; }

	PCI_VOID            SetBeAuthedInf(PCI_CHAR beAuthedInf) { m_beAuthedInf = beAuthedInf; }

	PCI_CHAR            GetRealNameBindInf(PCI_VOID) { return m_realNameBindInf; }

	PCI_VOID            SetRealNameBindInf(PCI_CHAR realNameBindInf) { m_realNameBindInf = realNameBindInf; }

	PCI_CHAR            GetFCMStatus(PCI_VOID) { return m_fcmStatus; }

	PCI_VOID            SetFCMStatus(PCI_CHAR fcmStatus) { m_fcmStatus = fcmStatus; }

	PCI_CHAR            GetPhoneLockedInf(PCI_VOID) { return m_phoneLockedInf; }

	PCI_VOID            SetPhoneLockedInf(PCI_CHAR phoneLockedInf) { m_phoneLockedInf = phoneLockedInf; }

	PCI_CHAR            GetPhoneServiceInf(PCI_VOID) { return m_phoneServiceInf; }

	PCI_VOID            SetPhoneServiceInf(PCI_CHAR phoneServiceInf) { m_phoneServiceInf = phoneServiceInf; }

	PCI_CHAR            GetPasspodBindInf(PCI_VOID) { return m_passpodBindInf; }

	PCI_VOID            SetPasspodBindInf(PCI_CHAR passpodBindInf) { m_passpodBindInf = passpodBindInf; }

	PCI_USHORT          GetPasspodExpireDays(PCI_VOID) { return m_passpodBindInf; }

	PCI_VOID            SetPasspodExpireDays(PCI_USHORT uPasspodExpireDays) { m_uPasspodExpireDays = uPasspodExpireDays; }

	PCI_USHORT          GetSecurityCode(PCI_VOID) { return m_uSecurityCode; }

	PCI_VOID            SetSecutityCode(PCI_USHORT uSecurityCode) { m_uSecurityCode = uSecurityCode; }

	PCI_USHORT          GetServerGroup(PCI_VOID) { return m_uServerGroup; }

	PCI_VOID            SetServerGroup(PCI_USHORT uServerGroup) { m_uServerGroup = uServerGroup; }

	PCI_CHAR*           GetServerAddress(PCI_VOID) { return m_szServerAddress; }

	PCI_VOID            SetServerAddress(PCI_CHAR* szServerAddress)
	{
		SetServerAddressLen(strlen(szServerAddress));
		strncpy(m_szServerAddress, szServerAddress, sizeof(m_szServerAddress) - 1);
	}

	PCI_CHAR*           GetServerName(PCI_VOID) { return m_szServerName; }

	PCI_VOID            SetServerName(PCI_CHAR* szServerName)
	{
		SetServerNameLen(strlen(szServerName));
		strncpy(m_szServerName, szServerName, sizeof(m_szServerName) - 1);
	}

	PCI_CHAR*           GetSegcode(PCI_VOID) { return m_szSegcode; }

	PCI_VOID            SetSegcode(PCI_CHAR* szSegcode)
	{
		SetSegcodeLen(strlen(szSegcode));
		strncpy(m_szSegcode, szSegcode, sizeof(m_szSegcode) - 1);
	}

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetServerAddressLen(PCI_VOID) { return m_uServerAddressLen; }
	PCI_VOID            SetServerAddressLen(PCI_UCHAR uServerAddressLen) { m_uServerAddressLen = uServerAddressLen; }
	PCI_UCHAR           GetServerNameLen(PCI_VOID) { return m_uServerNameLen; }
	PCI_VOID            SetServerNameLen(PCI_UCHAR uServerNameLen) { m_uServerNameLen = uServerNameLen; }
	PCI_UCHAR           GetSegcodeLen(PCI_VOID) { return m_uSegcodeLen; }
	PCI_VOID            SetSegcodeLen(PCI_UCHAR uSegcodeLen) { m_uSegcodeLen = uSegcodeLen; }

private:
	/* 成员变量 */
	PCI_UCHAR           m_uCNLen;                           // 用户名长度
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];               // 用户名
	PCI_UCHAR           m_uAuthRet;                         // 认证返回值
	PCI_CHAR            m_forbiddenInf;                     // 是否在防沉迷限制中
	PCI_INT             m_nOnlineTime;                      // 在线时长
	PCI_INT		    m_nOutlineTime;                     // 离线时长
	PCI_CHAR            m_mobileBindInf;                    // 手机绑定
	PCI_CHAR            m_IPBindInf;                        // IP绑定
	PCI_CHAR            m_cardBindInf;                      // 卡绑定
	PCI_CHAR            m_MACBindInf;                       // MAC地址绑定
	PCI_CHAR            m_beAuthedInf;                      // 授权信息
	PCI_CHAR            m_realNameBindInf;                  // 实名认证
	PCI_CHAR            m_fcmStatus;                        // 防沉迷阶段定义
	PCI_CHAR            m_phoneLockedInf;                   // 电话密宝
	PCI_CHAR            m_phoneServiceInf;                  // 电话密宝开启信息
	PCI_CHAR            m_passpodBindInf;                   // 硬件密宝绑定信息
	PCI_USHORT          m_uPasspodExpireDays;               // 硬件密宝过期天数
	PCI_USHORT          m_uSecurityCode;                    // 安全码

	PCI_USHORT            m_uServerGroup;                     // 服务器组信息
	PCI_UCHAR           m_uServerAddressLen;
	PCI_CHAR            m_szServerAddress[PCI_MAX_IP_LEN];        // 服务器地址
	PCI_UCHAR           m_uServerNameLen;
	PCI_CHAR            m_szServerName[PCI_SERVERNAME_LEN];   // 服务器名称

	PCI_UCHAR			m_uSegcodeLen;
	PCI_CHAR			m_szSegcode[PCI_MAX_PASSWD_LEN];		// 返回值中填写seg码
};

/********************************************************************
created date:
filename:      PacketRet/BLAuthUserRet.h

description:   用户认证协议返回包
 ********************************************************************/

enum LoginType
{
	LOGIN_SUCCESS = 0x01,          //成功登陆，返回用户点数信息
	LOGIN_NOUSER = 0x02,          //用户不存在
	LOGIN_ERRPWD = 0x03,          //密码错误
	LOGIN_ONSAME = 0x04,          //此用户在本组在线
	LOGIN_ONANOTHER = 0x05,          //在不同组在线,返回所在服务器的组号
	LOGIN_ERROTHERS = 0x06,          //其他错误
	LOGIN_FORBIDDEN = 0x07,          //被禁止登陆(包括ip,停权)

	LOGIN_IPOVERFLOW = 0x21,          //本日登陆ip过高，禁止登录
	LOGIN_INBLACKLST = 0x22,          //该登陆IP在黑名单中
	LOGIN_NOT_IN_SERVICE = 0x23,   		//服务器处于停服维护状态

	LOGIN_PASSPOD_LACK_BIND = 0x31,          //玩家没有绑定硬件密保
	LOGIN_PASSPOD_LACK_PASS = 0x32,          //玩家绑定了硬件密保但是没有输入密保密码
	LOGIN_PASSPOD_USED = 0x33,          //这个密保密码已经使用过
	LOGIN_PASSPOD_FAILED = 0x34,          //密保验证失败
	LOGIN_PASSPOD_EXPIRED = 0x35,          //密保已经过期
	LOGIN_PASSPOD_DISABLED = 0x36,          //密保已经被禁用
	LOGIN_PASSPOD_NEEDSYN = 0x37,          //用户需要校准令牌
	LOGIN_PASSPOD_PRIORITY = 0x39,          //绑定硬件密保和密保卡，但是输入了密保卡，应该输入安全级别高的硬件密保
	LOGIN_PASSPOD_PASSCARD_NOUSE = 0x40,        //绑定硬件密保和密保卡，但是未输入任何安全验证，应该输入硬件密宝

	LOGIN_NOACTIVE = 0xA0,          //add by handx 2008-09-01
	LOGIN_NEEDREGISTER = 0xA1,          //需要到客户端注册
	LOGIN_LACKIP = 0xB1,          //用户绑定ip
	LOGIN_LACKMOBILE = 0xB2,          //手机以及锁定账号

	LOGIN_NOLACKEDCARD = 0xC1,          //用户未申请密宝卡功能
	LOGIN_LACKEDCARD = 0xC2,          //用户申请了密宝卡功能但没有使用

	LOGIN_LOCKCARD_ERR = 0xC3,          //用户输入的密宝卡信息错误
	LOGIN_SERVERBIND_ERR = 0xC4,          //服务器要求用户秘宝绑定
	LOGIN_PHONELOCKED_ERR = 0xC5,          //用户申请了电话秘宝服务，但未激活帐号。
	LOGIN_PHONELOCKED_MUSTBE = 0xC6,          //用户必须绑定电话密保才能登陆

	LOGIN_FORBIDDEN_LACKNAME = 0xC7,          //用户没有输入实名信息禁止登陆

	LOGIN_MAC_ERROR = 0xD1,          // mac 匹配失败
	LOGIN_MAC_BIND = 0xD2,          //mac申请绑定
	LOGIN_FORBINDDEN_REPEAT = 0xE1,          //防沉迷帐号同时只能有一个帐号在线
	LOGIN_NEED_CONFIRMPROTOCOL = 0xF1,          //用户需要确认协议
	LOGIN_COFIRM_PROTOCOL = 0xF2,          // 用户确认协议

	LOGIN_MOBILE_BIND = 0xFF           //短信密保用户锁定账号
};

class BLAuthUserRet : public Packet
{
public:
	BLAuthUserRet();
	virtual  ~BLAuthUserRet();

	/* 共用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETAUTHUSER;
	}

public:
	/* 设置、获取数据接口 */
	PCI_CHAR*           GetEN(PCI_VOID) { return m_szEN; }

	PCI_VOID	    SetEN(PCI_CHAR* szEN)
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

	PCI_UCHAR           GetAuthRet(PCI_VOID) { return m_uAuthRet; }

	PCI_VOID            SetAuthRet(PCI_UCHAR uAuthRet) { m_uAuthRet = uAuthRet; }

	PCI_CHAR            GetForbiddenInf(PCI_VOID) { return m_forbiddenInf; }

	PCI_VOID            SetForbiddenInf(PCI_CHAR forbiddenInf) { m_forbiddenInf = forbiddenInf; }

	PCI_INT             GetOnlineTime(PCI_VOID) { return m_nOnlineTime; }

	PCI_VOID            SetOnlineTime(PCI_INT nOnlineTime) { m_nOnlineTime = nOnlineTime; }

	PCI_INT             GetOutlintTime(PCI_VOID) { return m_nOutlineTime; }

	PCI_VOID            SetOutlintTime(PCI_INT nOutlineTime) { m_nOutlineTime = nOutlineTime; }

	PCI_CHAR            GetMobileBindInf(PCI_VOID) { return m_mobileBindInf; }

	PCI_VOID            SetMobileBindInf(PCI_CHAR mobileBindInf) { m_mobileBindInf = mobileBindInf; }

	PCI_CHAR            GetIPBindInf(PCI_VOID) { return m_IPBindInf; }

	PCI_VOID            SetIPBindInf(PCI_CHAR IPBindInf) { m_IPBindInf = IPBindInf; }

	PCI_CHAR            GetCardBindInf(PCI_VOID) { return m_cardBindInf; }

	PCI_VOID            SetCardBindInf(PCI_CHAR cardBindInf) { m_cardBindInf = cardBindInf; }

	PCI_CHAR            GetMACBindInf(PCI_VOID) { return m_MACBindInf; }

	PCI_VOID            SetMACBindInf(PCI_CHAR MACBindInf) { m_MACBindInf = MACBindInf; }

	PCI_CHAR            GetBeAuthedInf(PCI_VOID) { return m_beAuthedInf; }

	PCI_VOID            SetBeAuthedInf(PCI_CHAR beAuthedInf) { m_beAuthedInf = beAuthedInf; }

	PCI_CHAR            GetRealNameBindInf(PCI_VOID) { return m_realNameBindInf; }

	PCI_VOID            SetRealNameBindInf(PCI_CHAR realNameBindInf) { m_realNameBindInf = realNameBindInf; }

	PCI_CHAR            GetFCMStatus(PCI_VOID) { return m_fcmStatus; }

	PCI_VOID            SetFCMStatus(PCI_CHAR fcmStatus) { m_fcmStatus = fcmStatus; }

	PCI_CHAR            GetPhoneLockedInf(PCI_VOID) { return m_phoneLockedInf; }

	PCI_VOID            SetPhoneLockedInf(PCI_CHAR phoneLockedInf) { m_phoneLockedInf = phoneLockedInf; }

	PCI_CHAR            GetPhoneServiceInf(PCI_VOID) { return m_phoneServiceInf; }

	PCI_VOID            SetPhoneServiceInf(PCI_CHAR phoneServiceInf) { m_phoneServiceInf = phoneServiceInf; }

	PCI_CHAR            GetPasspodBindInf(PCI_VOID) { return m_passpodBindInf; }

	PCI_VOID            SetPasspodBindInf(PCI_CHAR passpodBindInf) { m_passpodBindInf = passpodBindInf; }

	PCI_USHORT          GetPasspodExpireDays(PCI_VOID) { return m_passpodBindInf; }

	PCI_VOID            SetPasspodExpireDays(PCI_USHORT uPasspodExpireDays) { m_uPasspodExpireDays = uPasspodExpireDays; }

	PCI_USHORT          GetSecurityCode(PCI_VOID) { return m_uSecurityCode; }

	PCI_VOID            SetSecutityCode(PCI_USHORT uSecurityCode) { m_uSecurityCode = uSecurityCode; }

	PCI_USHORT          GetServerGroup(PCI_VOID) { return m_uServerGroup; }

	PCI_VOID            SetServerGroup(PCI_USHORT uServerGroup) { m_uServerGroup = uServerGroup; }

	PCI_CHAR*           GetServerAddress(PCI_VOID) { return m_szServerAddress; }

	PCI_VOID            SetServerAddress(PCI_CHAR* szServerAddress)
	{
		SetServerAddressLen(strlen(szServerAddress));
		strncpy(m_szServerAddress, szServerAddress, sizeof(m_szServerAddress) - 1);
	}

	PCI_CHAR*           GetServerName(PCI_VOID) { return m_szServerName; }

	PCI_VOID            SetServerName(PCI_CHAR* szServerName)
	{
		SetServerNameLen(strlen(szServerName));
		strncpy(m_szServerName, szServerName, sizeof(m_szServerName) - 1);
	}

	PCI_UCHAR			GetEType(PCI_VOID) { return m_uEType; }

	PCI_VOID			SetEType(PCI_UCHAR uEType) { m_uEType = uEType; }

protected:
	PCI_UCHAR			GetENLen(PCI_VOID) { return m_uENLen; }
	PCI_VOID			SetENLen(PCI_UCHAR uENLen) { m_uENLen = uENLen; }
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }
	PCI_UCHAR           GetServerAddressLen(PCI_VOID) { return m_uServerAddressLen; }
	PCI_VOID            SetServerAddressLen(PCI_UCHAR uServerAddressLen) { m_uServerAddressLen = uServerAddressLen; }
	PCI_UCHAR           GetServerNameLen(PCI_VOID) { return m_uServerNameLen; }
	PCI_VOID            SetServerNameLen(PCI_UCHAR uServerNameLen) { m_uServerNameLen = uServerNameLen; }
private:
	/* 成员变量 */
	PCI_UCHAR			m_uENLen;							//登录名长度
	PCI_CHAR			m_szEN[PCI_MAX_CN_LEN];					//登录名
	PCI_UCHAR           m_uCNLen;                           // 用户名长度
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];               // 用户名
	PCI_UCHAR           m_uAuthRet;                         // 认证返回值
	PCI_CHAR            m_forbiddenInf;                     // 是否在防沉迷限制中
	PCI_INT             m_nOnlineTime;                      // 在线时长
	PCI_INT             m_nOutlineTime;                     // 离线时长
	PCI_CHAR            m_mobileBindInf;                    // 手机绑定
	PCI_CHAR            m_IPBindInf;                        // IP绑定
	PCI_CHAR            m_cardBindInf;                      // 卡绑定
	PCI_CHAR            m_MACBindInf;                       // MAC地址绑定
	PCI_CHAR            m_beAuthedInf;                      // 授权信息
	PCI_CHAR            m_realNameBindInf;                  // 实名认证
	PCI_CHAR            m_fcmStatus;                        // 防沉迷阶段定义
	PCI_CHAR            m_phoneLockedInf;                   // 电话密宝
	PCI_CHAR            m_phoneServiceInf;                  // 电话密宝开启信息
	PCI_CHAR            m_passpodBindInf;                   // 硬件密宝绑定信息
	PCI_USHORT          m_uPasspodExpireDays;               // 硬件密宝过期天数
	PCI_USHORT          m_uSecurityCode;                    // 安全码

	PCI_USHORT            m_uServerGroup;                     // 服务器组信息
	PCI_UCHAR           m_uServerAddressLen;
	PCI_CHAR            m_szServerAddress[PCI_MAX_IP_LEN];        // 服务器地址
	PCI_UCHAR           m_uServerNameLen;
	PCI_CHAR            m_szServerName[PCI_SERVERNAME_LEN];   // 服务器名称
	PCI_UCHAR			m_uEType;							//登陆类型
};

/********************************************************************
created date:
filename:      PacketRet/BLAutoBillRet.h

description:   每900秒发一次计费包
 ********************************************************************/

class BLAutoBillRet : public Packet
{
public:
	BLAutoBillRet();
	virtual ~BLAutoBillRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETBILLKEEPING;
	}

public:
	PCI_CHAR*           GetUserName(PCI_VOID) { return m_szUserName; }

	PCI_VOID            SetUserName(PCI_CHAR* szUserName)
	{
		SetUserNameLen(strlen(szUserName));
		strncpy(m_szUserName, szUserName, sizeof(m_szUserName) - 1);
	}

	PCI_UCHAR           GetAuthRet(PCI_VOID) { return m_uAuthRet; }

	PCI_VOID            SetAuthRet(PCI_UCHAR uAuthRet) { m_uAuthRet = uAuthRet; }

protected:
	PCI_UCHAR           GetUserNameLen(PCI_VOID) { return m_uUserNameLen; }
	PCI_VOID            SetUserNameLen(PCI_UCHAR uUserNameLen) { m_uUserNameLen = uUserNameLen; }

private:
	PCI_UCHAR           m_uUserNameLen;
	PCI_CHAR            m_szUserName[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uAuthRet;
};

/********************************************************************
created date:
filename:      PacketRet/BLBillEndRet.h

description:   玩家下线计费结束返回包
 ********************************************************************/

enum BILLEND
{
	BILLEND_SUCCESS = 0x01,
	BILLEND_FAIL = 0x00
};

class BLBillEndRet : public Packet
{
public:
	BLBillEndRet();
	virtual ~BLBillEndRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETBILLEND;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR GetRet() const { return m_uRet; }

	PCI_VOID SetRet(PCI_UCHAR val) { m_uRet = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }

private:
	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR   m_uRet;
};

/********************************************************************
  created date:
filename:      PacketRet/BLBillStartRet.h

description:   玩家上线返回包
 ********************************************************************/

class BLBillStartRet : public Packet
{
public:
	BLBillStartRet();
	virtual ~BLBillStartRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETBILLSTART;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR GetRet() const { return m_uRet; }

	PCI_VOID SetRet(PCI_UCHAR val) { m_uRet = val; }

	PCI_USHORT GetQuestionID() const { return m_uQuestionID; }

	PCI_VOID SetQuestionID(PCI_USHORT val) { m_uQuestionID = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }

private:
	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR   m_uRet;
	PCI_USHORT  m_uQuestionID;
};

/********************************************************************
  created date:
filename:      PacketRet/GMBillLotteryRet.h

description:   充值抽奖返回包
 ********************************************************************/

class BLCardLotteryCheckRet : public Packet
{
public:
	BLCardLotteryCheckRet();
	virtual ~BLCardLotteryCheckRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCARDLOTTERYCHECK;
	}

public:
	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCn; }
	PCI_VOID            SetCN(const PCI_CHAR* szCn)
	{
		SetCNLen(strlen(szCn));
		strncpy(m_szCn, szCn, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR           GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID            SetResult(PCI_UCHAR uResult) { m_uResult = uResult; }

	PCI_CHAR*	        GetAward() { return m_szAward; }

	PCI_VOID            SetAward(const PCI_CHAR* val)
	{
		SetAwardLen(strlen(val));
		strncpy(m_szAward, val, sizeof(m_szAward) - 1);
	}

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCnLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCnLen) { m_uCnLen = uCnLen; }
	PCI_UCHAR           GetAwardLen() const { return m_uAwardLen; }
	PCI_VOID            SetAwardLen(PCI_UCHAR val) { m_uAwardLen = val; }

private:
	PCI_UCHAR           m_uCnLen;                      // 用户名长度
	PCI_CHAR            m_szCn[PCI_MAX_CN_LEN];          // 用户名
	PCI_UCHAR           m_uResult;                     // 认证返回值
	PCI_UCHAR		m_uAwardLen;					   //奖品信息长度
	PCI_CHAR		m_szAward[PCI_MAX_AWARD_LEN];
};

/********************************************************************
  created date:
filename:     PacketRet/BLCardLotteryRet.h
author:

description:   抽奖返回包
 ********************************************************************/

class BLCardLotteryRet : public Packet
{
public:
	BLCardLotteryRet();
	virtual ~BLCardLotteryRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCARDLOTTERY;
	}

public:
	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID  SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR GetResult() const { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_CHAR*	GetAward() { return m_szAward; }

	PCI_VOID SetAward(const PCI_CHAR* val)
	{
		SetAwardLen(strlen(val));
		strncpy(m_szAward, val, sizeof(m_szAward) - 1);
	}

protected:
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetAwardLen() const { return m_uAwardLen; }
	PCI_VOID SetAwardLen(const PCI_UCHAR val) { m_uAwardLen = val; }

private:
	PCI_UCHAR      m_uCnLen;                            //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                //用户名字符串
	PCI_UCHAR      m_uResult;                           //抽奖结果
	PCI_UCHAR		m_uAwardLen;						//奖品信息长度
	PCI_CHAR		m_szAward[PCI_MAX_AWARD_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/BLCnLotteryCheckRet.h
author:

description:   抽奖前背包空间检测返回包
 ********************************************************************/

class BLCnLotteryCheckRet : public Packet
{
public:
	BLCnLotteryCheckRet();
	virtual ~BLCnLotteryCheckRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCNLOTTERYCHECK;
	}

public:
	/* 设置,获取数据接口 */
	PCI_CHAR*           GetCn(PCI_VOID) { return m_szCn; }

	PCI_VOID            SetCn(const PCI_CHAR* szCn)
	{
		SetCnLen(strlen(szCn));
		strncpy(m_szCn, szCn, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR           GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID            SetResult(PCI_UCHAR uResult) { m_uResult = uResult; }

	PCI_CHAR*	GetAward() { return m_szAward; }

	PCI_VOID SetAward(const PCI_CHAR* val)
	{
		SetAwardLen(strlen(val));
		strncpy(m_szAward, val, sizeof(m_szAward) - 1);
	}

protected:
	PCI_UCHAR           GetCnLen(PCI_VOID) { return m_uCnLen; }
	PCI_VOID            SetCnLen(PCI_UCHAR uCnLen) { m_uCnLen = uCnLen; }
	PCI_UCHAR GetAwardLen() const { return m_uAwardLen; }
	PCI_VOID SetAwardLen(PCI_UCHAR val) { m_uAwardLen = val; }

private:
	/* 成员变量 */
	PCI_UCHAR           m_uCnLen;                   // 用户名长度
	PCI_CHAR            m_szCn[PCI_MAX_CN_LEN];       // 用户名
	PCI_UCHAR           m_uResult;                 // 认证返回值
	PCI_UCHAR		m_uAwardLen;						//奖品信息长度
	PCI_CHAR		m_szAward[PCI_MAX_AWARD_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/BLCnLotteryRet.h
author:

description:   新版账号抽奖返回包
 ********************************************************************/

class BLCnLotteryRet : public Packet
{
public:
	BLCnLotteryRet();
	virtual ~BLCnLotteryRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCNLOTTERY;
	}

public:
	// 使用数据接口
	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	const PCI_CHAR* GetSerialNo() const { return m_szSerialNo; }

	PCI_VOID SetSerialNo(const PCI_CHAR* val) { strncpy(m_szSerialNo, val, sizeof(m_szSerialNo) - 1); }

	PCI_UCHAR GetResult() const { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_CHAR*	GetAward() { return m_szAward; }

	PCI_VOID SetAward(const PCI_CHAR* val)
	{
		SetAwardLen(strlen(val));
		strncpy(m_szAward, val, sizeof(m_szAward) - 1);
	}

	// 数据
protected:
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetAwardLen() const { return m_uAwardLen; }
	PCI_VOID SetAwardLen(PCI_UCHAR val) { m_uAwardLen = val; }

private:
	PCI_UCHAR      m_uCnLen;                             //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                 //用户名字符串
	PCI_CHAR       m_szSerialNo[PCI_MAX_SERIAL_NO_LEN];  //流水号String,定长21字节,服务器组号+随机码 必须唯
	PCI_UCHAR      m_uResult;                            //抽奖结果
	PCI_UCHAR	   m_uAwardLen;						     //奖品信息长度
	PCI_CHAR	   m_szAward[PCI_MAX_AWARD_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/BLCnPointRet.h

description:   用户点数查询协议
 ********************************************************************/

class BLCnPointRet : public Packet
{
public:
	BLCnPointRet();
	virtual ~BLCnPointRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCNPOINT;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_UCHAR GetRet(PCI_VOID) { return m_uRet; }

	PCI_VOID SetRet(PCI_UCHAR value) { m_uRet = value; }

	PCI_UINT GetPoint(PCI_VOID) { return m_uPoint; }

	PCI_VOID SetPoint(PCI_UINT value) { m_uPoint = value; }

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR	 m_uRet;
	PCI_UINT     m_uPoint;
};

/********************************************************************
created date:
filename:      PacketRet/BLConfirmProtocolRet.h

description:   确认畅游协议
 ********************************************************************/

enum
{
	CONFIRM_PROTOCOL_SUCCEEDED = 0x00,
	CONFIRM_PROTOCOL_FAILED = 0x01
};

class BLConfirmProtocolRet : public Packet
{
public:
	BLConfirmProtocolRet();
	virtual    ~BLConfirmProtocolRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCONFIRMPROTOCOL;
	}

public:
	PCI_CHAR*           GetUserName(PCI_VOID) { return m_szUserName; }

	PCI_VOID            SetUserName(PCI_CHAR* szUserName)
	{
		SetUserNameLen(strlen(szUserName));
		strncpy(m_szUserName, szUserName, sizeof(m_szUserName) - 1);
	}

	PCI_UCHAR           GetAuthRet(PCI_VOID) { return m_uAuthRet; }

	PCI_VOID            SetAuthRet(PCI_UCHAR uAuthRet) { m_uAuthRet = uAuthRet; }

protected:
	PCI_UCHAR           GetUserNameLen(PCI_VOID) { return m_uUserNameLen; }
	PCI_VOID            SetUserNameLen(PCI_UCHAR uUserNameLen) { m_uUserNameLen = uUserNameLen; }

private:
	PCI_UCHAR           m_uUserNameLen;
	PCI_CHAR            m_szUserName[PCI_MAX_CN_LEN];
	PCI_UCHAR           m_uAuthRet;
};

/********************************************************************
created date:
filename:      PacketRet/BLConnectRet.h

description:   游戏服务器连接返回包
 ********************************************************************/

class BLConnectRet : public Packet
{
public:
	BLConnectRet();
	virtual ~BLConnectRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETCONNECT;
	}

public:
	PCI_USHORT GetReturn() const { return m_uReturn; }
	void   SetReturn(PCI_USHORT val) { m_uReturn = val; }

private:
	PCI_USHORT m_uReturn;
};

/********************************************************************
created date:
filename:      PacketRet/BLDisableUserRet.h

description:   用户停权解权协议
 ********************************************************************/

class BLDisableUserRet : public Packet
{
public:
	BLDisableUserRet();
	virtual ~BLDisableUserRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETDISABLEUSER;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_UCHAR GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR value) { m_uResult = value; }

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR    m_uResult;
};

/********************************************************************
  created date:
filename:      PacketRet/BLExchangeYBRet.h
author:

description:   元宝消耗记录
 ********************************************************************/

enum STORE
{
	STORE_BUYSUCCESS = 0x00,           //成功
	STORE_RESEND = 0x01,           //已经扣点，包重发情况用
	STORE_LACKMONEY = 0x02,           //金额不足
	STORE_LISTNOERR = 0x03,           //物品编码不存在
	STORE_OTHERERR = 0x09            //其他错误
};

class BLExchangeYBRet : public Packet
{
public:
	BLExchangeYBRet();
	virtual ~BLExchangeYBRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETEXCHANGEYB;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_CHAR* GetSerialNo(PCI_VOID) { return m_szSerialNo; }

	PCI_VOID SetSerialNo(const PCI_CHAR* value) { strncpy(m_szSerialNo, value, sizeof(m_szSerialNo) - 1); }

	PCI_UCHAR GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR value) { m_uResult = value; }

	PCI_UINT GetPoint(PCI_VOID) { return m_uPoint; }

	PCI_VOID SetPoint(PCI_UINT value) { m_uPoint = value; }

	PCI_CHAR*	GetGoods() { return m_szGoods; }

	PCI_VOID SetGoods(const PCI_CHAR* val)
	{
		SetGoodsLen(strlen(val));
		strncpy(m_szGoods, val, sizeof(m_szGoods) - 1);
	}

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }
	PCI_UCHAR GetGoodsLen() const { return m_uGoodsLen; }
	PCI_VOID SetGoodsLen(PCI_UCHAR val) { m_uGoodsLen = val; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_CHAR     m_szSerialNo[PCI_MAX_SERIAL_NO_LEN];    //流水号String, 定长21字节,服务器组号+随机码,必须唯一
	PCI_UCHAR    m_uResult;
	PCI_UINT     m_uPoint;
	PCI_UCHAR    m_uGoodsLen;
	PCI_CHAR	 m_szGoods[PCI_MAX_GOODS_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/LBImmeLotterySpecifyRet.h

description:   即时兑奖CE返回包
 ********************************************************************/

class BLImmeLotteryConfirmRet : public Packet
{
public:
	BLImmeLotteryConfirmRet();
	virtual ~BLImmeLotteryConfirmRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETIMMELOTTERYTYPECONFIRM;
	}

public:
	PCI_VOID	SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_CHAR*	GetCN() { return m_szCn; }

	PCI_VOID	SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_UCHAR	GetResult() { return m_uResult; }

	PCI_VOID    SetAwardList(const PCI_CHAR* val)
	{
		SetAwardListLen(strlen(val));
		strncpy(m_szAwardlist, val, sizeof(m_szAwardlist) - 1);
	}

	PCI_CHAR*   GetAwardList() { return m_szAwardlist; }

	PCI_VOID    SetTypelimit(const PCI_CHAR* val)
	{
		SetTypelimitLen(strlen(val));
		strncpy(m_szTypelimit, val, sizeof(m_szTypelimit) - 1);
	}

	PCI_CHAR*  GetTypelimit() { return m_szTypelimit; }

	PCI_VOID    SetCnlimit(const PCI_CHAR* val)
	{
		SetCnlimitLen(strlen(val));
		strncpy(m_szCnlimit, val, sizeof(m_szCnlimit) - 1);
	}

	PCI_CHAR*  GetCnlimit() { return m_szCnlimit; }

	PCI_VOID    SetSN(const PCI_CHAR *val) { strncpy(m_szSN, val, sizeof(m_szSN)); }

	PCI_CHAR *  GetSN() { return m_szSN; }

protected:
	PCI_VOID    SetAwardListLen(PCI_UCHAR val) { m_uAwardlistLen = val; }
	PCI_UCHAR   GetAwardListLen() { return m_uAwardlistLen; }
	PCI_VOID	SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR	GetCNLen() { return m_uCnLen; }
	PCI_VOID    SetTypelimitLen(PCI_UCHAR val) { m_uTypelimitLen = val; }
	PCI_UCHAR   GetTypelimitLen() { return m_uTypelimitLen; }
	PCI_VOID    SetCnlimitLen(PCI_UCHAR val) { m_uCnlimitLen = val; }
	PCI_UCHAR   GetCnlimitLen() { return m_uCnlimitLen; }

private:
	PCI_UCHAR      m_uCnLen;                            //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                  //用户名字符串
	PCI_UCHAR      m_uResult;                           //抽奖结果
	PCI_UCHAR      m_uAwardlistLen;
	PCI_CHAR	   m_szAwardlist[PCI_AWARDLIST_LEN];
	PCI_UCHAR      m_uTypelimitLen;
	PCI_CHAR	   m_szTypelimit[PCI_LIMITINFO_LEN];
	PCI_UCHAR      m_uCnlimitLen;
	PCI_CHAR	   m_szCnlimit[PCI_LIMITINFO_LEN];
	PCI_CHAR       m_szSN[PCI_MAX_SERIAL_NO_LEN];
};

/********************************************************************
  created date:
filename:      PacketRet/LBImmeLotterySpecifyRet.h

description:   即时兑奖CD返回包
 ********************************************************************/

class BLImmeLotterySpecifyRet : public Packet
{
public:
	BLImmeLotterySpecifyRet();
	virtual ~BLImmeLotterySpecifyRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETIMMELOTTERYTYPESPECIFY;
	}

public:

	PCI_VOID	SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_CHAR*	GetCN() { return m_szCn; }

	PCI_VOID	SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_UCHAR	GetResult() { return m_uResult; }

	PCI_VOID    SetAwardList(const PCI_CHAR* val)
	{
		SetAwardListLen(strlen(val));
		strncpy(m_szAwardlist, val, sizeof(m_szAwardlist) - 1);
	}

	PCI_CHAR*  GetAwardList() { return m_szAwardlist; }

	PCI_VOID    SetTypelimit(const PCI_CHAR* val)
	{
		SetTypelimitLen(strlen(val));
		strncpy(m_szTypelimit, val, sizeof(m_szTypelimit) - 1);
	}

	PCI_CHAR*  GetTypelimit() { return m_szTypelimit; }

	PCI_VOID    SetCnlimit(const PCI_CHAR* val)
	{
		SetCnlimitLen(strlen(val));
		strncpy(m_szCnlimit, val, sizeof(m_szCnlimit) - 1);
	}

	PCI_CHAR*  GetCnlimit() { return m_szCnlimit; }

protected:
	PCI_VOID	SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR	GetCNLen() { return m_uCnLen; }
	PCI_VOID    SetAwardListLen(PCI_UCHAR val) { m_uAwardlistLen = val; }
	PCI_UCHAR   GetAwardListLen() { return m_uAwardlistLen; }
	PCI_VOID    SetTypelimitLen(PCI_UCHAR val) { m_uTypelimitLen = val; }
	PCI_UCHAR   GetTypelimitLen() { return m_uTypelimitLen; }
	PCI_VOID    SetCnlimitLen(PCI_UCHAR val) { m_uCnlimitLen = val; }
	PCI_UCHAR   GetCnlimitLen() { return m_uCnlimitLen; }

private:
	PCI_UCHAR      m_uCnLen;                            //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                  //用户名字符串
	PCI_UCHAR      m_uResult;                           //抽奖结果
	PCI_UCHAR      m_uAwardlistLen;
	PCI_CHAR	   m_szAwardlist[PCI_AWARDLIST_LEN];
	PCI_UCHAR      m_uTypelimitLen;
	PCI_CHAR	   m_szTypelimit[PCI_LIMITINFO_LEN];
	PCI_UCHAR      m_uCnlimitLen;
	PCI_CHAR	   m_szCnlimit[PCI_LIMITINFO_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/LBImmeLotteryTypeCheckRet.h

description:   即时兑奖CC返回包
 ********************************************************************/

class BLImmeLotteryTypeCheckRet : public Packet
{
public:
	BLImmeLotteryTypeCheckRet();
	virtual ~BLImmeLotteryTypeCheckRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETIMMELOTTERYTYPECHECK;
	}

public:
	PCI_VOID	SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_CHAR*	GetCN() { return m_szCn; }

	PCI_VOID	SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_UCHAR	GetResult() { return m_uResult; }

	PCI_VOID	SetLotteryType(const PCI_CHAR * val)
	{
		SetLotteryTypeLen(strlen(val));
		strncpy(m_szLotteryType, val, sizeof(m_szLotteryType) - 1);
	}

	PCI_CHAR*	GetLotteryType() { return m_szLotteryType; }
protected:
	PCI_VOID	SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR	GetCNLen() { return m_uCnLen; }
	PCI_VOID	SetLotteryTypeLen(const PCI_UCHAR val) { m_uLotteryTypeLen = val; }
	PCI_UCHAR	GetLotteryTypeLen() { return m_uLotteryTypeLen; }

private:
	PCI_UCHAR      m_uCnLen;                            //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                //用户名字符串
	PCI_UCHAR      m_uResult;                           //抽奖结果
	PCI_UCHAR	   m_uLotteryTypeLen;						//奖品信息长度
	PCI_CHAR	   m_szLotteryType[PCI_MAX_LOTTERYTYPE_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/BLKeepAliveRet.h
author:

description:   Login与Billing保持连接回应类
 ********************************************************************/

class BLKeepAliveRet : public Packet
{
public:
	BLKeepAliveRet();
	virtual ~BLKeepAliveRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETKEEPLIVE;
	}

public:
	PCI_USHORT GetRet() { return m_uRet; }
	void SetRet(PCI_USHORT uRet) { m_uRet = uRet; }

private:
	PCI_USHORT m_uRet;
};

/********************************************************************
created date:
filename:      PacketRet/BLMiBaoCardChekRet.h

description:   密保卡验证协议返回包
 ********************************************************************/

enum ChekType
{
	CHEK_MIBAOCARD_SUCCESS = 0X00,			//秘宝卡验证成功
	CHEK_MIBAOCARD_PASSERR = 0x01,          //用户输入的密宝卡密码错误
	CHEK_MIBAOCARD_NOBIND = 0x03,          //没有绑定秘宝卡
	CHEK_MIBAOCARD_OTHERERR = 0x06          //其他错误。
};

class BLMiBaoCardChekRet : public Packet
{
public:
	BLMiBaoCardChekRet();
	virtual  ~BLMiBaoCardChekRet();

	/* 共用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETMIBAOCARDCHED;
	}

public:
	/* 设置、获取数据接口 */
	PCI_CHAR*           GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID            SetCN(PCI_CHAR* szCN)
	{
		SetCNLen(strlen(szCN));
		strncpy(m_szCN, szCN, sizeof(m_szCN) - 1);
	}

	PCI_UCHAR           GetChekRet(PCI_VOID) { return m_uChekRet; }

	PCI_VOID            SetChekRet(PCI_UCHAR uChekRet) { m_uChekRet = uChekRet; }

	PCI_CHAR            GetCardBindInf(PCI_VOID) { return m_cardBindInf; }

	PCI_VOID            SetCardBindInf(PCI_CHAR cardBindInf) { m_cardBindInf = cardBindInf; }

protected:
	PCI_UCHAR           GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID            SetCNLen(PCI_UCHAR uCNLen) { m_uCNLen = uCNLen; }

private:
	/* 成员变量 */
	PCI_UCHAR           m_uCNLen;                           // 用户名长度
	PCI_CHAR            m_szCN[PCI_MAX_CN_LEN];               // 用户名
	PCI_UCHAR           m_uChekRet;                         // 认证返回值
	PCI_CHAR            m_cardBindInf;                      // 卡绑定
	PCI_UCHAR           m_uServerAddressLen;
	PCI_CHAR            m_szServerAddress[PCI_MAX_IP_LEN];        // 服务器地址
};

/********************************************************************
  created date:
filename:      PacketLogic/LBOTPCheckLogic.h
author:

description:   Passpodcheck
 ********************************************************************/

class BLOTPCheckRet : public Packet
{
public:
	BLOTPCheckRet();
	virtual ~BLOTPCheckRet();

	/* 公用继承接口 */
	virtual PCI_BOOL    Write(OutputStream   & oStream) const;
	virtual PCI_BOOL    Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_BL_RETOPTCHECKED;
	}

public:
	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, m_uCNLen);
	}

	PCI_UCHAR GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR value) { m_uResult = value; }

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }

private:
	/* 数据 */
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN + 1];
	PCI_UCHAR    m_uResult;
};

/********************************************************************
  created date:
filename:      PacketRet/BLPhoneLockedRet.h

description:   电话密宝
 ********************************************************************/

class BLPhoneLockedRet : public Packet
{
public:
	BLPhoneLockedRet();
	virtual ~BLPhoneLockedRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_BL_RETPHONELOCKED;
	}

public:

	PCI_CHAR * GetCN(PCI_VOID) { return m_szCN; }

	PCI_VOID SetCN(const PCI_CHAR * value)
	{
		SetCNLen(strlen(value));
		strncpy(m_szCN, value, sizeof(m_szCN) - 1);
	}

	PCI_UCHAR GetResult(PCI_VOID) { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR value) { m_uResult = value; }

protected:
	PCI_UCHAR GetCNLen(PCI_VOID) { return m_uCNLen; }
	PCI_VOID SetCNLen(PCI_UCHAR value) { m_uCNLen = value; }

private:
	PCI_UCHAR    m_uCNLen;
	PCI_CHAR     m_szCN[PCI_MAX_CN_LEN];
	PCI_UCHAR    m_uResult;
};

/********************************************************************
created date:
filename:      PacketRet/BLServBuyLogRet.h
author:

description:   元宝消耗日志记录返回包
 ********************************************************************/

class BLServBuyLogRet : public Packet
{
public:
	BLServBuyLogRet();
	virtual ~BLServBuyLogRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETBUYLOG;
	}

public:
	PCI_CHAR* GetSN(PCI_VOID) { return m_szSN; }
	PCI_VOID SetSN(const PCI_CHAR* szSN) { strncpy(m_szSN, szSN, sizeof(m_szSN) - 1); }

	PCI_UCHAR GetRet(PCI_VOID) { return m_uRet; }
	PCI_VOID SetRet(PCI_UCHAR uRet) { m_uRet = uRet; }

public:
	PCI_CHAR  m_szSN[PCI_MAX_SERIAL_NO_LEN];
	PCI_UCHAR m_uRet;
};

/********************************************************************
	created date:
	filename:      PacketRet/BLServRestartRet.h

	description:   服务器重启返回包
********************************************************************/

class BLServRestartRet : public Packet
{
public:
	BLServRestartRet();
	virtual ~BLServRestartRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETKICKALL;
	}

public:
	PCI_UCHAR GetRet(PCI_VOID) { return m_uRet; }
	PCI_VOID SetRet(PCI_UCHAR uRet) { m_uRet = uRet; }

public:
	PCI_UCHAR m_uRet;
};

/********************************************************************
  created date:
filename:      PacketRet/BLSwitchRoleRet.h

description:   玩家切换角色返回包
 ********************************************************************/

class BLSwitchRoleRet : public BLBillEndRet
{
public:
	BLSwitchRoleRet();
	virtual ~BLSwitchRoleRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_BL_RETSWITCHROLE;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR GetRet() const { return m_uRet; }

	PCI_VOID SetRet(PCI_UCHAR val) { m_uRet = val; }

protected:
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }

private:
	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR   m_uRet;
};

class BLSwitchServerRet : public Packet
{
public:
	BLSwitchServerRet();
	virtual ~BLSwitchServerRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETSWITCHSERVER;
	}

public:
	const PCI_CHAR* GetCn() const { return m_szCn; }
	PCI_VOID SetCn(PCI_CHAR *val) { strncpy(m_szCn, val, sizeof(m_szCn) - 1); }
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }

	PCI_UCHAR GetRet() const { return m_uRet; }
	PCI_VOID SetRet(PCI_UCHAR val) { m_uRet = val; }

protected:
	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN];
	PCI_UCHAR   m_uRet;
};

/********************************************************************
created date:
filename:      PacketRet/BLUnivCardLotteryRet.h

description:   通用卡抽奖返回包
 ********************************************************************/

class BLUnivCardLotteryRet : public Packet
{
public:
	BLUnivCardLotteryRet();
	virtual ~BLUnivCardLotteryRet();

	/* 公用继承接口 */
	virtual PCI_BOOL   Write(OutputStream   & oStream) const;
	virtual PCI_BOOL   Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return  PACKET_BL_RETUNIVCARDLOTTERY;
	}

public:
	const PCI_CHAR* GetCN() const { return m_szCn; }

	PCI_VOID  SetCN(const PCI_CHAR* val)
	{
		SetCNLen(strlen(val));
		strncpy(m_szCn, val, sizeof(m_szCn) - 1);
	}

	PCI_UCHAR GetResult() const { return m_uResult; }

	PCI_VOID SetResult(PCI_UCHAR val) { m_uResult = val; }

	PCI_CHAR*	GetAward() { return m_szAward; }

	PCI_VOID SetAward(const PCI_CHAR* val)
	{
		SetAwardLen(strlen(val));
		strncpy(m_szAward, val, sizeof(m_szAward) - 1);
	}

protected:
	PCI_UCHAR GetCNLen() const { return m_uCnLen; }
	PCI_VOID SetCNLen(PCI_UCHAR val) { m_uCnLen = val; }
	PCI_UCHAR GetAwardLen() const { return m_uAwardLen; }
	PCI_VOID SetAwardLen(const PCI_UCHAR val) { m_uAwardLen = val; }

private:
	PCI_UCHAR      m_uCnLen;                            //用户名长度
	PCI_CHAR       m_szCn[PCI_MAX_CN_LEN];                //用户名字符串
	PCI_UCHAR      m_uResult;                           //抽奖结果
	PCI_UCHAR		m_uAwardLen;						//奖品信息长度
	PCI_CHAR		m_szAward[PCI_MAX_AWARD_LEN];
};

/********************************************************************
created date:
filename:      PacketRet/BLUserInfoCollectRet.h
author:
description:   用户信息收集返回包处理逻辑
 ********************************************************************/

class BLUserInfoCollectRet : public Packet
{
public:
	BLUserInfoCollectRet();
	virtual ~BLUserInfoCollectRet();

	/* 公用继承接口 */
	virtual PCI_BOOL Write(OutputStream  & oStream) const;
	virtual PCI_BOOL Read(InputStream & iStream);

	virtual PacketID_t GetPacketID() const
	{
		return PACKET_BL_RETUSERINFOCOLLECT;
	}

public:
	//使用数据接口
	const PCI_CHAR* GetSvrIP() const { return m_szSvrIP; }

	PCI_VOID SetSvrIP(PCI_CHAR *val)
	{
		SetSvrIPLen(strlen(val));
		strncpy(m_szSvrIP, val, PCI_MAX_IP_LEN);
	}

	const PCI_CHAR* GetCn() const { return m_szCn; }

	PCI_VOID SetCn(PCI_CHAR *val)
	{
		SetCnLen(strlen(val));
		strncpy(m_szCn, val, PCI_MAX_CN_LEN);
	}

	PCI_CHAR GetRet()const { return m_uRet; }

	PCI_VOID SetRet(PCI_CHAR val) { m_uRet = val; }

protected:
	PCI_UCHAR GetSvrIPLen() const { return m_nSvrIPLen; }
	PCI_VOID SetSvrIPLen(PCI_UCHAR val) { m_nSvrIPLen = val; }
	PCI_UCHAR GetCnLen() const { return m_nCnLen; }
	PCI_VOID SetCnLen(PCI_UCHAR val) { m_nCnLen = val; }

private:
	/* 数据 */
	PCI_UCHAR   m_nSvrIPLen;
	PCI_CHAR    m_szSvrIP[PCI_MAX_IP_LEN + 1];

	PCI_UCHAR   m_nCnLen;
	PCI_CHAR    m_szCn[PCI_MAX_CN_LEN + 1];

	PCI_CHAR	m_uRet;
};

#endif
