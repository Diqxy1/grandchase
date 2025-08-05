#include "DBLayer.h"
#include "odbc/Odbc.h"

NiImplementRTTI(KDBLayer, KThreadManager);

KDBLayer::KDBLayer(void)
{
}

KDBLayer::~KDBLayer(void)
{
}

ImplToStringW(KDBLayer)
{
	START_TOSTRING_PARENTW(KThreadManager);

	stm_ << L" -- odbc -- (size:" << m_mapODBC.size() << L")" << std::endl;

	std::map<std::wstring, KODBCPtr >::const_iterator mit;

	for (mit = m_mapODBC.begin(); mit != m_mapODBC.end(); mit++)
	{
		stm_ << L"  Name : " << mit->first << *mit->second;
	}

	KODBC::DumpMaxTime(stm_);

	return stm_;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KDBLayer::RegToLua(std::shared_ptr<lua_State> sptlua_)
{
	luabind::module(sptlua_.get())
		[
			luabind::class_<KDBLayer, KThreadManager>("KDBLayer")
			.def("AddODBC", &KDBLayer::AddODBC)
		.def("RemoveODBC", &KDBLayer::RemoveODBC)
		.def("GetODBC", &KDBLayer::GetODBC)
		];
}

bool KDBLayer::AddODBC(const char* szName_, const char* szFileDSN_)
{
	KODBCPtr spODBC(new KODBC);

	std::wstring wstrFileDSN = KncUtil::toWideString(szFileDSN_);

	if (!spODBC->DriverConnect(wstrFileDSN.c_str()))
		return false;

	KLocker lock(m_csMapODBC);

	std::wstring strName = KncUtil::toWideString(szName_, strlen(szName_));

	std::map<std::wstring, KODBCPtr >::iterator mit;
	mit = m_mapODBC.find(strName);

	if (mit != m_mapODBC.end())
	{
		//KLOGL(( 0, "AddODBC(), Err:이미 동일한 이름의 DB가 존재" ));
		return false;
	} // if

	m_mapODBC[strName] = spODBC;

	return true;
}

void KDBLayer::RemoveODBC(const char* szName_)
{
	KLocker lock(m_csMapODBC);

	std::wstring strName = KncUtil::toWideString(szName_, strlen(szName_));
	std::map<std::wstring, KODBCPtr >::iterator   mit;
	mit = m_mapODBC.find(strName);
	if (mit != m_mapODBC.end())
	{
		m_mapODBC.erase(mit);
	} // if
}

KODBC* KDBLayer::GetODBC(const char* szName_)
{
	KLocker lock(m_csMapODBC);

	std::wstring strName = KncUtil::toWideString(szName_, strlen(szName_));
	std::map<std::wstring, KODBCPtr >::iterator   mit;
	mit = m_mapODBC.find(strName);
	if (mit != m_mapODBC.end())
	{
		return mit->second.get();
	} // if

	return NULL;
}