#include "Event.h"
#include <Serializer/Serializer.h>
#include <iterator>

NiImplementRootRTTI(KEvent);

KEvent::KEvent(void)
	:m_usEventID(0)
	, m_nFrom(FT_NONE)
{
}

KEvent::~KEvent(void)
{
}

ImplToStringW(KEvent)
{
	START_TOSTRINGW
		<< TOSTRINGW(m_usEventID)
		<< TOSTRINGW(GetEventIDString())
		<< TOSTRINGW(m_nFrom)
		<< TOSTRINGW(m_kbuff.GetLength());

	const std::vector<char>& buf = m_kbuff.GetBuffer();
	std::copy(buf.begin(), buf.end(), std::ostream_iterator<int, wchar_t>(stm_, L", "));

	return stm_ << std::endl;
}

SERIALIZE_DEFINE_PUT(KEvent, obj, ks)
{
	return  ks.Put(obj.m_usEventID) &&
		ks.Put(obj.m_kbuff) &&
		ks.Put(obj.m_nFrom);
}

SERIALIZE_DEFINE_GET(KEvent, obj, ks)
{
	return  ks.Get(obj.m_usEventID) &&
		ks.Get(obj.m_kbuff) &&
		ks.Get(obj.m_nFrom);
}

ImplOstmOperatorW(KEvent);