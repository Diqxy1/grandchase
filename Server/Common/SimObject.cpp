#include "SimObject.h"
#include "time.h"       // _strtime(), _strdate()
//#include <KncUtil.h>    // toWideString

//#undef max
//#include <boost/random.hpp>

NiImplementRootRTTI(KSimObject);

KSimObject::KSimObject(void) : m_dwUID(0)
{
	wchar_t buff1[128] = { 0 };
	wchar_t buff2[128] = { 0 };
	wchar_t buff3[128] = { 0 };

	static unsigned __int64 sSeedNum = 0;

	swprintf(buff3, 128, L"SOB_%s_%s_%020d",
		_wstrdate(buff1),
		_wstrtime(buff2),
		sSeedNum++);

	m_strName = buff3;
}

KSimObject::~KSimObject(void)
{
}

ImplToStringW(KSimObject)
{
	return START_TOSTRINGW
		<< TOSTRINGW(m_strName)
		<< TOSTRINGW(GetRefCount());
}