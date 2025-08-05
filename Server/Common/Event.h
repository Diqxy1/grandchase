#pragma once

#include <Serializer/SerBuffer.h>
#include "KncSmartPtr.h"
#include "RTTI.h"
#include <dbg/dbg.hpp>
#include <ToString.h>
#include <boost/utility.hpp>

SmartPointer(KEvent);

class KEvent;
SERIALIZE_DEFINE_TAG(KEvent, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KEvent);
class KEvent : public boost::noncopyable
{
	NiDeclareRootRTTI(KEvent);
	DeclToStringW;

public:
	enum FROM_TYPE {
		FT_NONE = 0,
		FT_INNER,
		FT_OUTTER,
		FT_MAX
	};
	KEvent(void);
	virtual ~KEvent(void);

	virtual const wchar_t* GetEventIDString() const { return L"virtual function called."; }

	template <class T>
	void Serialize(const T& data);

	//SERIALIZE_DECLARE_FRIEND( KEvent );

//protected:
	unsigned short      m_usEventID;
	KSerBuffer          m_kbuff;
	int                 m_nFrom; //0, internal, outer
};

template <class T>
void KEvent::Serialize(const T& data_)
{
	KSerializer ks;
	m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting(&m_kbuff);
	ks.Put(data_);
	ks.EndWriting();
}

DeclOstmOperatorW(KEvent);
// ms_szEventID[]   : 이벤트 아이디의 문자열 변수
// GetEventIDString : 이벤트 아이디의 문자열을 얻어온다
#define DeclareEvent( classname ) \
public: \
    virtual const wchar_t*  GetEventIDString() const; \
    static const wchar_t*   GetEventIDString( unsigned short usEventID ); \
    static const wchar_t*   ms_szEventID[]; \

// GetEventIDString()   : implement
#define ImplementEvent( classname ) \
    const wchar_t* classname::GetEventIDString() const \
    { \
        if( m_usEventID >= EVENT_EVENTID_SENTINEL ) \
        { \
            return ms_szEventID[ EVENT_EVENTID_SENTINEL ]; \
        } \
        return ms_szEventID[ m_usEventID ]; \
    } \
    const wchar_t* classname::GetEventIDString( unsigned short usEventID ) \
    { \
        if( usEventID >= EVENT_EVENTID_SENTINEL ) \
        { \
            return ms_szEventID[ EVENT_EVENTID_SENTINEL ]; \
        } \
        return ms_szEventID[ usEventID ]; \
    }
