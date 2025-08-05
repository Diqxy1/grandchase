#include "MultiLanguageString.h"
#include "Lua/KLuaManager.h"
#include "../GameServer/GameServer.h"

//FILE_NAME_FOR_LOG

ImplementSingleton(KMultiLanguageString);
ImplOstmOperatorW2A(KMultiLanguageString);
NiImplementRootRTTI(KMultiLanguageString);

KMultiLanguageString::KMultiLanguageString(void)
{
}

KMultiLanguageString::~KMultiLanguageString(void)
{
}

ImplToStringW(KMultiLanguageString)
{
	return stm_;
}

bool KMultiLanguageString::LoadScript()
{
	return _LoadScript(std::string());
}

bool KMultiLanguageString::_LoadScript(OUT std::string& strScript_)
{
	KLuaManager kLuaMng;
	const std::string strScriptName = "MultiLangugeString.lua";
	strScript_ = strScriptName;

	std::map< int, std::wstring > mapPCCafeMessage;

	_JIF(kLuaMng.DoFile(strScriptName.c_str()) == S_OK, return false);
	_JIF(LoadPCCafe(kLuaMng, mapPCCafeMessage), return false);

	KLocker lock(m_csLanguage);
	m_mapPCCafeString.swap(mapPCCafeMessage);

	return true;
}

bool KMultiLanguageString::LoadPCCafe(IN KLuaManager& kLuaMng_, OUT std::map< int, std::wstring >& mapPCCafeMessage_)
{
	_JIF(kLuaMng_.BeginTable("PCCafeMessage") == S_OK, return false);
	for (int i = 1; ; ++i)
	{
		if (kLuaMng_.BeginTable(i) != S_OK) break;

		std::string strMsgNum;
		int nLanguageCode;

		_JIF(kLuaMng_.GetValue(1, nLanguageCode) == S_OK, return false);
		_JIF(kLuaMng_.GetValue(2, strMsgNum) == S_OK, return false);
		_JIF(kLuaMng_.EndTable() == S_OK, return false);

		std::wstring strPCCafeMsg = SiKGameServer()->m_stringTable.GetValue(KncUtil::toWideString(strMsgNum).c_str());

		mapPCCafeMessage_.insert(std::make_pair(nLanguageCode, strPCCafeMsg));
	}
	_JIF(kLuaMng_.EndTable() == S_OK, return false);
	return true;
}

bool KMultiLanguageString::GetPCCafeMsg(IN const int nCountryCode_, OUT std::wstring& strMsg_)
{
	std::map< int, std::wstring >::const_iterator mit;
	mit = m_mapPCCafeString.find(nCountryCode_);

	if (mit == m_mapPCCafeString.end()) {
		return false;
	}

	strMsg_ = mit->second;

	return true;
}