#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CenterPacket.h"
#include "UserPacket.h"
#include <atltime.h>

class KLuaManager;

class KConnectionGiftBox
{
    DeclareSingleton( KConnectionGiftBox );
    NiDeclareRootRTTI( KConnectionGiftBox );
    DeclToStringW;

public:
    KConnectionGiftBox( void );
    ~KConnectionGiftBox( void );

    bool SetFromConnectionGiftBoxInfoNot( IN const KECN_CONNECTION_GIFTBOX_INFO_NOT& kPacket_ );
    bool IsRunGiftFirstCharacter();
    bool GetGiftFirstCharacter( IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecItems_ );
    bool IsGiftEventUserTerm();
    int GetGiftEventUserVersion();
    time_t GetDecideBeginTimeNewUser() { KLocker lock( m_csConnectionGiftBox ); return m_tmDecideBeginTimeNewUser; };
    int GetNewUserEffectDuration() { KLocker lock( m_csConnectionGiftBox ); return m_nNewUserEffectDuration; };
    bool GetNewUserGiftGradeInfo( IN const int nNewUserCurrentGrade_, OUT KGiftGradeInfo& kGiftGradeInfo_ );
    bool GetEventUserGiftGradeInfo( IN const int nEventUserCurrentGrade_, OUT KGiftGradeInfo& kGiftGradeInfo_ );
    bool GetNewUserNextGift( IN const int nCharType_, IN const int nCurrentGrade_, OUT std::vector< std::pair< int, std::vector< KDropItemInfo > > >& vecNextGift_ );
    bool GetEventUserNextGift( IN const int nCharType_, IN const int nCurrentGrade_, OUT std::vector< std::pair< int, std::vector< KDropItemInfo > > >& vecNextGift_ );
    bool GetNewUserCurrentGradeRewards( IN const int nNewUserCurrentGrade_, IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecRewards_ );
    bool GetEventUserCurrentGradeRewards( IN const int nEventUserCurrentGrade_, IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecRewards_ );

private:
    mutable KncCriticalSection                  m_csConnectionGiftBox;

    bool                                        m_bUseGiftFirstCharacter;
    std::map< int, KCharGiftItem >              m_mapGiftFirstCharacter;    // 첫 캐릭터 생성시 지급할 아이템 정보( key: CharType )

    bool                                        m_bUseGiftNewUser;
    time_t                                      m_tmDecideBeginTimeNewUser; // 신규유저 판단용 기준날짜
    int                                         m_nNewUserEffectDuration;   // 신규판정을 받은 이후 신규유저로 취급되는 기간 설정(int)
    std::map< int, KGiftGradeInfo >             m_mapGiftNewUser;           // 신규유저 선물 정보( key: 단계 )

    int                                         m_nGiftEventUserVersion;    // 이벤트 버전
    time_t                                      m_tmGiftEventBegin;         // 이벤트 시작 시간
    time_t                                      m_tmGiftEventEnd;           // 이벤트 종료 시간
    std::map< int, KGiftGradeInfo >             m_mapGiftEventUser;         // 이벤트유저 선물 정보( key: 단계 )
};

DefSingletonInline( KConnectionGiftBox );
DeclOstmOperatorA( KConnectionGiftBox );

class KConnectionGiftBoxUserData
{
public:
    struct KGiftBoxUserData {
        int                                         m_nLastGetGrade;        // 마지막으로 받은 선물상자 단계
        int                                         m_nAccPlayTime;         // 현재 단계에서 누적된 시간(분)

        KGiftBoxUserData() : m_nLastGetGrade( 0 ), m_nAccPlayTime( 0 ) {};
    };

    KConnectionGiftBoxUserData();
    ~KConnectionGiftBoxUserData();

    int GetNewUserLastGetGrade( IN const int nCharType_ ) { return m_mapNewUser[ nCharType_ ].m_nLastGetGrade; };
    int GetNewUserCurrentGrade( IN const int nCharType_ ) { return m_mapNewUser[ nCharType_ ].m_nLastGetGrade + 1; };
    int GetNewUserAccPlayTime( IN const int nCharType_ ) { return m_mapNewUser[ nCharType_ ].m_nAccPlayTime; };
    int GetEventUserLastGetGrade( IN const int nCharType_ ) { return m_mapEventUser[ nCharType_ ].m_nLastGetGrade; };
    int GetEventUserCurrentGrade( IN const int nCharType_ ) { return m_mapEventUser[ nCharType_ ].m_nLastGetGrade + 1; };
    int GetEventUserAccPlayTime( IN const int nCharType_ ) { return m_mapEventUser[ nCharType_ ].m_nAccPlayTime; };
    void SetNewUser( IN const bool bIsNewUser_ ) { m_bIsNewUser = bIsNewUser_; };
    void SetNewUserAccPlayTime( IN const int nCharType_, IN const int nAccPlayTime_ ) { m_mapNewUser[ nCharType_ ].m_nAccPlayTime = nAccPlayTime_; };
    void SetEventUserAccPlayTime( IN const int nCharType_, IN const int nAccPlayTime_ ) { m_mapEventUser[ nCharType_ ].m_nAccPlayTime = nAccPlayTime_; };
    void SetData( IN const int nCharType_, IN const KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ );
    void IncreaseNewUserAccPlayTime( IN const int nCharType_ ) { m_mapNewUser[ nCharType_ ].m_nAccPlayTime++; };
    void IncreaseEventUserAccPlayTime( IN const int nCharType_ ) { m_mapEventUser[ nCharType_ ].m_nAccPlayTime++; };
    void IncreaseNewUserLastGetGrade( IN const int nCharType_ ) { m_mapNewUser[ nCharType_ ].m_nLastGetGrade++; };
    void IncreaseEventUserLastGetGrade( IN const int nCharType_ ) { m_mapEventUser[ nCharType_ ].m_nLastGetGrade++; };
    bool IsNewUser() { return m_bIsNewUser; };
    int GetNewUserRemainAccPlayTime( IN const int nCharType_ );
    int GetEventUserRemainAccPlayTime( IN const int nCharType_ );
    void ResetNewUserAccPlayTime( IN const int nCharType_ ) { m_mapNewUser[ nCharType_ ].m_nAccPlayTime = 0; };
    void ResetEventUserAccPlayTime( IN const int nCharType_ ) { m_mapEventUser[ nCharType_ ].m_nAccPlayTime = 0; };

public:
    bool                                            m_bIsNewUser;           // 신규유저 혜택 대상여부
    std::map< int, KGiftBoxUserData >               m_mapNewUser;           // 신규유저 접속 선물상자 유저 데이터( key: chartype )
    std::map< int, KGiftBoxUserData >               m_mapEventUser;         // 이벤트유저 접속 선물상자 유저 데이터( key: chartype )
};
