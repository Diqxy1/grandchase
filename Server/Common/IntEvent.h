#pragma once

#include "KncSmartPtr.h"
#include <string>
#include <Serializer/SerBuffer.h>
#include <boost/utility.hpp>

SmartPointer(KIntEvent);

class KIntEvent : public boost::noncopyable
{
public:
	KIntEvent(void) :m_dwSenderUID(0), m_usEventID(0), m_nFrom(0), m_nModeType(0) {}
	virtual ~KIntEvent(void) {}

public:
	std::wstring        m_strSender;
	DWORD               m_dwSenderUID;
	unsigned short      m_usEventID;
	KSerBuffer          m_kbuff;
	int                 m_nFrom;
	int                 m_nModeType;
	char                m_cCharType;
};
