#include "CYOUPacket.h"

SERIALIZE_DEFINE_PUT(KCYOUServerInfo, obj, ks)
{
	return  PUT(m_usZoneID) &&
		PUT(m_usGroupID) &&
		PUT(m_nCCU) &&
		PUT(m_strIP);
}

SERIALIZE_DEFINE_GET(KCYOUServerInfo, obj, ks)
{
	return  GET(m_usZoneID) &&
		GET(m_usGroupID) &&
		GET(m_nCCU) &&
		GET(m_strIP);
}

SERIALIZE_DEFINE_PUT(CYOUPacketHeader, obj, ks)
{
	return  PUT(m_usHeader) &&
		PUT(m_usPacketLength) &&
		PUT(m_dwPacketType) &&
		PUT(m_usPacketSquence);
}

SERIALIZE_DEFINE_GET(CYOUPacketHeader, obj, ks)
{
	return  GET(m_usHeader) &&
		GET(m_usPacketLength) &&
		GET(m_dwPacketType) &&
		GET(m_usPacketSquence);
};

SERIALIZE_DEFINE_PUT(CYOUPacketCommonBody, obj, ks)
{
	return PUT(m_ucGameserverIPLength) &&
		ks.PutRaw(&obj.m_chServerIP, sizeof(obj.m_chServerIP));
};

SERIALIZE_DEFINE_GET(CYOUPacketCommonBody, obj, ks)
{
	return GET(m_ucGameserverIPLength) &&
		ks.GetRaw(&obj.m_chServerIP, sizeof(obj.m_chServerIP));
};

SERIALIZE_DEFINE_PUT(KCYOUAuthUserInfo, obj, ks)
{
	return  PUT(m_usPacketSequence) &&
		PUT(m_strLogin) &&
		PUT(m_strPassword) &&
		PUT(m_strIP) &&
		PUT(m_strMACAddress) &&
		PUT(m_strPC) &&
		PUT(m_strServerIP) &&
		PUT(m_dwUID) &&
		PUT(m_cLogoutType) &&
		PUT(m_Character) &&
		PUT(m_usCharacterLevel) &&
		PUT(m_strCharNickname);
};

SERIALIZE_DEFINE_GET(KCYOUAuthUserInfo, obj, ks)
{
	return  GET(m_usPacketSequence) &&
		GET(m_strLogin) &&
		GET(m_strPassword) &&
		GET(m_strIP) &&
		GET(m_strMACAddress) &&
		GET(m_strPC) &&
		GET(m_strServerIP) &&
		GET(m_dwUID) &&
		GET(m_cLogoutType) &&
		GET(m_Character) &&
		GET(m_usCharacterLevel) &&
		GET(m_strCharNickname);
};

SERIALIZE_DEFINE_PUT(KCYOUCashData, obj, ks)
{
	return  PUT(m_usPacketSequence) &&
		PUT(m_strServerIP) &&
		PUT(m_strLogin) &&
		PUT(m_strUserIP) &&
		PUT(m_strSerialNumber) &&
		PUT(m_strItemInfo) &&
		PUT(m_dwUserUID) &&
		PUT(m_nCashPoint) &&
		PUT(m_usZoneID) &&
		PUT(m_usGroupID) &&
		PUT(m_usServerID) &&
		PUT(m_strCharNickname) &&
		PUT(m_usCharacterLevel);
};

SERIALIZE_DEFINE_GET(KCYOUCashData, obj, ks)
{
	return  GET(m_usPacketSequence) &&
		GET(m_strServerIP) &&
		GET(m_strLogin) &&
		GET(m_strUserIP) &&
		GET(m_strSerialNumber) &&
		GET(m_strItemInfo) &&
		GET(m_dwUserUID) &&
		GET(m_nCashPoint) &&
		GET(m_usZoneID) &&
		GET(m_usGroupID) &&
		GET(m_usServerID) &&
		GET(m_strCharNickname) &&
		GET(m_usCharacterLevel);
};

SERIALIZE_DEFINE_PUT(KCYOUChangeCashLogData, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_strLogin) &&
		PUT(m_dwUserUID) &&
		PUT(m_strCharNickname) &&
		PUT(m_dwCashPoint) &&
		PUT(m_dwCYOUPoint) &&
		PUT(m_usPacketSequence) &&
		PUT(m_strServerIP) &&
		PUT(m_strUserIP) &&
		PUT(m_strSerialNumber) &&
		PUT(m_strItemInfo);
};

SERIALIZE_DEFINE_GET(KCYOUChangeCashLogData, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_strLogin) &&
		GET(m_dwUserUID) &&
		GET(m_strCharNickname) &&
		GET(m_dwCashPoint) &&
		GET(m_dwCYOUPoint) &&
		GET(m_usPacketSequence) &&
		GET(m_strServerIP) &&
		GET(m_strUserIP) &&
		GET(m_strSerialNumber) &&
		GET(m_strItemInfo);
};