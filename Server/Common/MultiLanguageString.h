#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <dbg/dbg.hpp>

class KLuaManager;
class KMultiLanguageString
{
	DeclareSingleton(KMultiLanguageString);
	NiDeclareRootRTTI(KMultiLanguageString);
	DeclToStringW;

public:
	KMultiLanguageString(void);
	~KMultiLanguageString(void);

	bool LoadScript();
	bool _LoadScript(OUT std::string& strScript);
	bool LoadPCCafe(IN KLuaManager& kLuaMng_, OUT std::map< int, std::wstring >& mapPCCafeMessage_);
	bool GetPCCafeMsg(IN const int nCountryCode_, OUT std::wstring& strMsg_);

private:
	mutable KncCriticalSection m_csLanguage;
	std::map< int, std::wstring > m_mapPCCafeString; // 언어, string
	std::map< int, std::wstring > m_mapRecomHelper;
};

DefSingletonInline(KMultiLanguageString);
DeclOstmOperatorA(KMultiLanguageString);
