#pragma once
#include <Thread/Locker.h>
#include <UserPacket.h>
#include <KNCSingleton.h>
#include <boost/random.hpp>
#include "Lua/KLuaManager.h"

struct sLevelBonus
{
    int m_iBeginLv;
    int m_iEndLv;

    sLevelBonus()
    :m_iBeginLv(-1)
    ,m_iEndLv(-1)
    {}
};

struct sTimeEvent
{
    enum EVENT_MODE{
        EM_PERFECT_DAY     = 37,    // 퍼펙트데이(해당 던전들을 퍼펙트로 클리어시 Exp+100 보너스)
        EM_FREE_HELL_MODE  = 51,    // 헬모드 프리 이벤트(해당 던전들은 헬모드 입장권 체크안함)
        EM_CONTINENT_EVENT = 71,    // 대륙 이벤트(해당 던전들은 헬모드 입장권 체크안함)
        EM_FRIEND_TOGETHER = 79,    // 친구와함께 이벤트
        EM_LEVEL_SERVER    = 95,    // 친구와함께 이벤트
    };

    int             m_nEventUID;
    int             m_nEventID;
    bool            m_bApplyType;       // 이벤트 혜택대상 (true : 전체, false : 해당 유저)
    // 보너스확률
    float           m_fExpRatio;
    float           m_fGpRatio;
    // 발동조건
    float           m_fActiveRate;      // 발동 확률
    std::map< int, std::set<int> >   m_mapCharTypePromotion;   // 캐릭터타입,set<직업>
    std::map< char, std::set<DWORD> > m_mapEquipItemIDA;      // [char Type, equips]
    std::map< char, std::set<DWORD> > m_mapEquipItemIDB;      // [char Type, equips]
    sLevelBonus     m_sLevelBonus;      // 레벨범위
    std::map< int, std::set<int> >     m_mapModeList;      // < 게임모드,set<타입> >
    int             m_nPartyNum;        // 파티원체크(-1:체크안함,1~6파티원체크)
    float           m_fPartyExpBonus;   // 파티Exp보너스
    float           m_fPartyGpBonus;    // 파티Gp보너스
    std::set<int>   m_setServerNum;     // 특정 서버용
    float           m_fStrengthRatio;   // 강화확률
    int             m_nDropRatio;       // 드랍 추가 확률

    bool operator==(const sTimeEvent& right) const
    {
        return ( m_nEventID  == right.m_nEventID );
    }

    sTimeEvent()
    :m_nEventUID(-1)
    ,m_nEventID(-1)
    ,m_bApplyType(false)
    ,m_fExpRatio(0.f)
    ,m_fGpRatio(0.f)
    ,m_fActiveRate(0.f)
    ,m_nPartyNum(-1)
    ,m_fPartyExpBonus(0.f)
    ,m_fPartyGpBonus(0.f)
    ,m_fStrengthRatio(0.f)
    ,m_nDropRatio(0)
    {
        m_mapCharTypePromotion.clear();
        m_mapModeList.clear();
        m_mapEquipItemIDA.clear();
        m_mapEquipItemIDB.clear();
        m_setServerNum.clear();
    }

    bool CheckActiveRate( IN const float fRatio_ );
    bool CheckServerBonus( IN const int nServerNum_ );
    bool CheckModeBonus( IN const int nModeID_, const int nDifficult_, const int nMapID_ );
};

class KTimeEventManager
{
    DeclareSingleton( KTimeEventManager );
    DeclToStringW;

public:
    KTimeEventManager(void);
    ~KTimeEventManager(void);

    void UpdateTimeEvent( IN std::vector<sTimeEvent>& vecEventList );
    bool GetCorrectEvents( IN const KGameInfo& kInfo,
                           IN const std::vector<KSimpleInDoorUserInfo>& vecStartingUserInfo,
                           OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    int GetEventCount()const { KLocker lock( m_csEventList); return (int)m_vecEventList.size(); }

    void CalcBoostInfo( IN OUT KEndGameReq& kPacket,
                        IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList,
                        IN KGameInfo& kGameInfo,
                        IN std::vector< std::wstring >& vecStrLogin );

    bool GetSimpleTimeEvents( OUT std::set<int>& setActiveEventIDList );

    void CalcLevelBonus( IN OUT KEndGameReq& kPacket_,
                         IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ );

    void CalcFriendEvent( IN OUT KEndGameReq& kPacket_,
                          IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ );

    void SetServerSort( IN const int nSort_ )   { KLocker lock( m_csEventList );    m_nServerSort = nSort_; }
    int GetServerSort()                         { KLocker lock( m_csEventList );    return m_nServerSort; }
    float GetStrengthBonus();
    float GetDungeonDropRatioBonus( IN const int& nModeID );

protected:
    bool GetApplicableEvent( IN const KGameInfo& kInfo,
                             IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                             OUT std::vector<sTimeEvent>& vecCommonEventList,
                             OUT std::vector<sTimeEvent>& vecPairEventList,
                             OUT std::vector<sTimeEvent>& vecSingleEventList,
                             OUT std::vector<sTimeEvent>& vecCharJobEventList,
                             OUT std::vector<sTimeEvent>& vecOneItemTwoUserList,
                             OUT std::vector<sTimeEvent>& vecLevelEventList,
                             OUT std::vector<sTimeEvent>& vecFriendEventList );
    void ApplyCommonEvent( IN const std::vector<sTimeEvent>& vecCommonEventList,
                           IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                           OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplyPairEvent( IN const std::vector<sTimeEvent>& vecPairEventList,
                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                         OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplySingleEvent( IN const std::vector<sTimeEvent>& vecSingleEventList,
                           IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                           OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplyCharJobEvent( IN const std::vector<sTimeEvent>& vecCharJobEventList,
                            IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                            OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplyOneItemTwoUserEvent( IN const std::vector<sTimeEvent>& vecOneItemTwoUserList,
                                   IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                                   OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplyLevelEvent( IN const std::vector<sTimeEvent>& vecLevelEventList,
                          IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                          OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void ApplyFriendEvent( IN const std::vector<sTimeEvent>& vecFriendEventList,
                           IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo,
                           OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );

    void DumpEventInfo( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList,                        IN const KGameInfo& kGameInfo );    void DumpEventInfo( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ );

    void EraseDuplicatedEvent( std::vector<std::pair<DWORD,char> >& vecApplyCharA_, std::vector<std::pair<DWORD,char> >& vecApplyCharB_ );

    void FindMemberWinEvent( std::vector< std::wstring >& vecStrLogin_, std::wstring& m_strLogin_ ,bool& bWin_ );

    bool GetCharJobEvent( IN const sTimeEvent& timeEvent_, IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_, std::vector<sTimeEvent>& vecCharJobEventList_ );

    bool CheckOneItemTwoUsersEvent( IN const sTimeEvent& timeEvent_ );

    bool CheckLevelEvent( IN const sTimeEvent& timeEvent_, IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_, std::vector<sTimeEvent>& vecLevelEventList_ );

    bool CheckFriendEvent( const IN sTimeEvent& timeEvent_, IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_, std::vector<sTimeEvent>& vecFriendEventList_ );

    void CalcEventRatio( IN const std::wstring strLogin_, IN const sTimeEvent& sTimeEvent_, IN std::vector< std::wstring >& vecStrLogin_, IN OUT KGameResultIn& kResultIn_, IN OUT KCharacterExpInfo& kCharExp_ );

protected:
    mutable KncCriticalSection                  m_csEventList;
    std::vector< sTimeEvent >                   m_vecEventList;         // 디봇 설정된 이벤트
    int                                         m_nServerSort;
    
    boost::lagged_fibonacci607 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_int<DWORD> > die;
};
DefSingletonInline( KTimeEventManager );
DeclOstmOperatorA( KTimeEventManager );
