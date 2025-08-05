#pragma once
#include "TRUser.h"
#include <map>
#include <list>
#include <Thread/Locker.h>
#include "FSM/FSMClass.h"
#include "RelayData.h"
#include "Actor.h"
#include "ActorFactory.h"

class KTRUserManager
{
    KTRUserManager( const KTRUserManager& right );
    KTRUserManager& operator= (const KTRUserManager& right );

public:
    KTRUserManager(void);
    ~KTRUserManager(void);

    KTRUserPtr CreateTRUser();
    KTRUserPtr GetTRUser( const DWORD dwUID );  // map에서 얻어 낸다.
    void Tick();
    
    void SetCreateEnable( bool bEnable )    { m_bCreateSessionEnable = bEnable; }
    bool Regist( KTRUserPtr& pkTRUser );  // 절반만 UID 등록처리된 유저를 map에 등록.
    size_t GetCountTot()const { KLocker lock( m_cslist ); return m_listSessionTable.size(); }
    size_t GetCountReg()const { KLocker lock( m_csmap ); return m_mapSessionTable.size(); }

    void  Relay( KRelayDataPtr& spData );
    //void DelUser( KTRUser trUser_ );
protected:

    bool                            m_bCreateSessionEnable;

    mutable KncCriticalSection      m_cslist;
    std::list< KTRUserPtr >         m_listSessionTable; // 등록 미등록 모두 여기 있음.

    mutable KncCriticalSection      m_csmap;
    std::map< DWORD, KTRUserPtr >   m_mapSessionTable;  // 등록된 TRUser만 여기 있음.

    FSMclassPtr                     m_spFSM;            // TRUser의 FSM
};
