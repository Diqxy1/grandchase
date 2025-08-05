#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"
#include "KncTimer.h"

class KJustInTime
{
    DeclareSingleton( KJustInTime );
    NiDeclareRootRTTI( KJustInTime );
    DeclToStringW;

public:

    enum EVENT_USER {
        EU_ALL_USER       = 0, // 모든 유저
        EU_PCCAFE_USER    = 1, // PC방 유저
        EU_EX_PCCAFE_USER = 2, // PC방아닌 유저

        EU_MAX,
    };

    struct KJITEvent
    {
        time_t                          m_tmEventTime;
        std::vector< KDropItemInfo >    m_vecRewardList;
    };

    struct KSortEventTime
    {
    public:
        bool operator() ( const KJITEvent& A, const KJITEvent& B )
        {
            return A.m_tmEventTime < B.m_tmEventTime;
        }
    };

    KJustInTime( void );
    ~KJustInTime( void );

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool LoadEventList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<USHORT,std::map<time_t,KJITEvent> >& mapEventList_ );
    bool LoadEvent( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< time_t, KJITEvent >& mapEvent_ );
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardList_ );

    bool GetNextEvent( IN const USHORT& usUserType, OUT time_t& tmNextEvent_ );
    bool IsHaveNextEvent( IN const USHORT& usUserType );
    void Tick();
    bool GetRewardList( IN const USHORT& usUserType, IN const time_t& tmEvent, OUT std::vector< KDropItemInfo >& vecRewardList_ );
    bool UpdateReceivedRewardStatus( IN const USHORT& usUserType, IN std::pair< DWORD, time_t >& prReceivedInfo_ );
    bool IsReceivedReward( IN const USHORT& usUserType, IN std::pair< DWORD, time_t >& prReceivedInfo_ );
    bool IsNextEventToday( IN const USHORT& usUserType );
    void SetNextEvent( IN const USHORT& usUserType, IN const time_t& tmNextEvent ) { KLocker lock( m_csJIT ); m_mapNextEvent[usUserType] = tmNextEvent; }
    time_t GetNextEvent( IN const USHORT& usUserType );
    void SetCurrentEvent( IN const USHORT& usUserType, IN const time_t& tmCurrentEvent );


protected:
    mutable KncCriticalSection          m_csJIT;
    KncTimer                            m_kTick;

    // 유저들이 보상받을 이벤트 시간 정보. KEVENT_JUST_IN_TIME_ON_TIME_NOT 전달 하면서 설정됨
    std::map<USHORT,time_t>             m_mapCurrentEvent; // map[UserType,현재이벤트시간]

    // 다음 이벤트 시간 정보. 가장 가까운 다음 시간정보를 찾아서 세팅. 이후 전송할 때 오늘 날자인 경우에만 전송하게 한다.
    // GetNextEvent()에서 오늘 정보만 세팅하게 할 수 있겠지만 그럴경우 날자가 지나면 다시 검사해서 세팅해주는 추가작업이 필요하게 되어 이렇게 결정함.
    std::map<USHORT,time_t>             m_mapNextEvent; // map[UserType,다음이벤트시간]

    // JIT 이벤트 리스트
    std::map<USHORT,std::map<time_t,KJITEvent> >    m_mapEventList; // map[UserType, map[이벤트시간,보상정보] ]

    // 보상 받아간 유저 정보 기록
    std::map<USHORT,std::set< std::pair<DWORD,time_t> > >   m_mapReceivedRewardStatus; // map[UserType, set[ pair[UserUID,이벤트시간] ]
};

DefSingletonInline( KJustInTime );
DeclOstmOperatorA( KJustInTime );
