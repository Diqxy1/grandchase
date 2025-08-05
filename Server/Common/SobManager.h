#pragma once
#include <map>
#include <Windows.h>    // CRITICAL_SECTION
#include "SimObject.h"
#include "NetError.h"
#include "Thread/Locker.h"

#include "dbg/dbg.hpp"
#include "Log4.h"

template< class spTptr >
class KSobManager
{
public:
	KSobManager() { InitializeCriticalSection(&m_csSob); }
	virtual ~KSobManager() { DeleteCriticalSection(&m_csSob); }

	bool Add(typename spTptr::element_type& kSob);
	bool Add(spTptr& spSob);
	bool Delete(const std::wstring& strName);
	bool GetNameByPointer(IN const typename spTptr::element_type* pkT, OUT std::wstring& strName);
	typename spTptr GetByName(const std::wstring& strName);
	typename spTptr GetByIndex(int nIndex);

	size_t GetCount() const
	{
		KLocker lock(m_csSob);
		return m_mapSob.size();
	}

	mutable CRITICAL_SECTION         m_csSob;
	std::map<std::wstring, spTptr>   m_mapSob;
};

template< class spTptr >
bool KSobManager<spTptr>::Add(typename spTptr::element_type& kSob_)
{
	KLocker lock(m_csSob);

	std::map<std::wstring, spTptr>::iterator     mit;
	mit = m_mapSob.find(kSob_.GetName());
	if (mit != m_mapSob.end())
	{
		SET_ERROR(ERR_INTERNAL_03);
		PRINT_LASTERROR;
		return false;
	}

	m_mapSob.insert(std::make_pair(
		kSob_.GetName(), std::static_pointer_cast<spTptr::element_type>(kSob_.shared_from_this())));

	SET_ERROR(NET_OK);
	return true;
}

template< class spTptr >
bool KSobManager<spTptr>::Add(spTptr& spSob_)
{
	KLocker lock(m_csSob);

	if (!NiIsKindOf(spTptr::element_type, spSob_.get()))    // another type
	{
		START_LOG(cerr, L"another type")
			<< BUILD_LOG(spTptr::element_type::m_RTTI.GetName())
			<< BUILD_LOG(spSob_->GetRTTI()->GetName()) << END_LOG;
		SET_ERROR(ERR_INTERNAL_01);
		return false;
	}

	return Add(*spSob_);
}

template< class spTptr >
bool KSobManager<spTptr>::Delete(const std::wstring& strName_)
{
	KLocker lock(m_csSob);

	if (m_mapSob.find(strName_) == m_mapSob.end())
	{
		START_LOG(cerr, L"삭제하고자 하는 이름이 없음")
			<< BUILD_LOG(strName_)
			<< BUILD_LOG(m_mapSob.size()) << END_LOG;
		SET_ERROR(ERR_INTERNAL_04);
		return false;
	}

	m_mapSob.erase(strName_);
	SET_ERROR(NET_OK);
	return true;
}

template< class spTptr >
bool KSobManager<spTptr>::GetNameByPointer(IN const typename spTptr::element_type* pkT, OUT std::wstring& strName)
{
	KLocker lock(m_csSob);

	std::map<std::wstring, spTptr>::iterator mit;
	for (mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++)
	{
		if (mit->second.get() == pkT)
		{
			START_LOG(cerr, L"변경된 sob 이름 복구 시도 성공. " << pkT->GetName() << L" -> " << mit->first) << END_LOG;
			strName = mit->first;
			return true;
		}
	}

	return false;
}

template< class spTptr >
typename spTptr KSobManager<spTptr>::GetByName(const std::wstring& strName_)
{
	KLocker lock(m_csSob);

	std::map<std::wstring, spTptr>::iterator mit;
	mit = m_mapSob.find(strName_);

	if (mit == m_mapSob.end())
	{
		// 050120 log level Warn -> clog
		START_LOG(clog, L"No Name. wanted:" << strName_) << END_LOG;
		return spTptr();
	}

	return mit->second;
}

template< class spTptr >
typename spTptr KSobManager<spTptr>::GetByIndex(int nIndex_)
{
	KLocker lock(m_csSob);

	if (nIndex_ < 0 || nIndex_ >(int)m_mapSob.size())
	{
		START_LOG(cwarn, L"bad index. wanted:" << nIndex_)
			<< BUILD_LOG(m_mapSob.size()) << END_LOG;
		return spTptr();
	}

	std::map<std::wstring, spTptr>::iterator mit;
	int i = 0;
	mit = m_mapSob.begin();

	while (i != nIndex_ && mit != m_mapSob.end())
	{
		i++;
		mit++;
	}

	if (mit == m_mapSob.end())
	{
		START_LOG(cwarn, L"unknown. wanted:" << nIndex_)
			<< BUILD_LOG(m_mapSob.size()) << END_LOG;
		return spTptr();
	}

	return mit->second;
}