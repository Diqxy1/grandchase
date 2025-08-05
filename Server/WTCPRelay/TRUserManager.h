#pragma once
#include "TRUser.h"
#include <map>
#include <list>
#include <Thread/Locker.h>
#include "FSM/FSMClass.h"
#include "RelayData.h"
#include "Actor.h"
#include "ActorFactory.h"
#include "TREvent.h"
#include "TRPacket.h"

class KTRUserManager
{
    KTRUserManager( const KTRUserManager& right );
    KTRUserManager& operator= (const KTRUserManager& right );

public:
    enum {
        STAT_CURRENT_USER = 1,
        STAT_RECV_COUNT = 7,
        STAT_RELAY_COUNT = 8,
        STAT_PACKET_COUNT_OVER = 9, // 분당 패킷 카운트 초과
        STAT_SEND_PACKET_FAIL  = 10, // 패킷 전송 실패.
        STAT_FAIL_SEND_USERUID = 11, // 연결 성공이후 UID등록없음.
        STAT_FAIL_UID_REGISTERED = 12, // 유저 등록처리 실패.
        STAT_USER_ZOMBIE_STATE = 13, // 2분동안 heart bit 보내지 않음.
    };

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
    void IncreaseTypeStatCount( IN const int& nType, IN const int& nCount );
    void GetTypeStatInfo( OUT std::map<int,int>& mapTypeStat );
    void SetLimitPacketSendCount( IN int nLimitPacketSendCount );
    void SetDefaultType();
protected:

    bool                            m_bCreateSessionEnable;

    mutable KncCriticalSection      m_cslist;
    std::list< KTRUserPtr >         m_listSessionTable; // 등록 미등록 모두 여기 있음.

    mutable KncCriticalSection      m_csmap;
    std::map< DWORD, KTRUserPtr >   m_mapSessionTable;  // 등록된 TRUser만 여기 있음.

    FSMclassPtr                     m_spFSM;            // TRUser의 FSM

    mutable KncCriticalSection      m_csStat;           // 통계.
    std::map<int,int>               m_mapTypeStat;      // [Type, Cnt] 통계
    DWORD                           m_dwLastTick;
    DWORD                           m_dwTickGap;
};
