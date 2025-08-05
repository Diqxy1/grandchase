#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <set>
#include "UserPacket.h"
#include "CenterPacket.h"

class KAdventure
{
    DeclareSingleton( KAdventure );
    NiDeclareRootRTTI( KAdventure );
    DeclToStringW;

public:
    KAdventure(void);
    ~KAdventure(void);

    void SetAdventureEventData( IN const KECN_ADVENTURE_DATA_NOT& kPacket_ );
    bool IsRun() const;
    int GetVersion();

    bool GetAdventureData( OUT KEVENT_ADVENTURE_DATA_ACK& kPacket_ );

    bool GetContinentRewardInfo( IN const int& nContinentID_, OUT KContinentRewardInfo& kContinentRewardInfo_ );
    bool GetFinalRewardInfo( OUT KDropItemInfo& kFinalRewardInfo_ );

    bool CheckContidionContinentReward( IN const int& nContinentID_, IN std::map< DWORD, int >& mapCollectData_ );
    bool CheckContidionFinalReward( IN std::map< int, bool >& mapContinentData_ );

    bool GetContinentRewardListInfo( OUT std::map< int, KContinentRewardInfo >& mapContinentRewardInfo_ );

    void InitAdventureUserData( OUT std::map< int, std::map< DWORD, int > >& mapCollectData_, OUT std::map< int, bool >& mapContinentData_ );

    bool GetAdventureMonsterInfo( OUT std::map< PAIR_DWORD, KAdventureMonsterInfo >& mapAdventureMonsterInfo_ );

    DWORD GetMaxCollectCount( IN const int& nContinentID_, IN const DWORD& dwCollectID_ );

    void GetCollectDataDiff( IN std::map< int, std::map< DWORD, int > >& mapCollectDataPrev_, OUT std::map< int, std::map< DWORD, int > >& mapCollectData_ );
    void GetCollectDiff( IN std::map< DWORD, int >& mapCollectPrev_, OUT std::map< DWORD, int >& mapCollect_ );

    bool AdjustCollectData( OUT std::map< int, std::map< DWORD, int > >& mapCollectData_ );

protected:
    mutable KncCriticalSection                      m_csAdventure;

    // 센터서버로부터 받은 이벤트 정보
    std::map< PAIR_DWORD, KAdventureMonsterInfo >   m_mapAdventureMonsterInfo;     // 몬스터 정보;     key: pair< m_dwDungeonID, m_dwMonsterID >
    std::map< int, KContinentRewardInfo >           m_mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
    KDropItemInfo                                   m_kFinalRewardInfo;            // 최종 보상 정보;

    // 이벤트 기간 정보.. 요 정보로 게임 서버가 이벤트 ON/OFF를 판단함
    std::map< int, KAdventureEventDate >            m_mapAdventureEventDate;       // 모험 이벤트 기간 정보; key: EventID(발동 세부 기간; 게임서버는 요 데이터만 유지하면 된다)
};

DefSingletonInline( KAdventure );
DeclOstmOperatorA( KAdventure );

class KAdventureUserData
{ 
public:
    KAdventureUserData();
    ~KAdventureUserData();

    void SetAdventureUserData( IN OUT std::map< int, std::map< DWORD, int > >& mapCollectData_, IN OUT std::map< int, bool >& mapContinentData_ );
    bool IsRecievedContinentReward( IN const int& nContinentID_ );
    bool SetRecieveContinentReward( IN const int& nContinentID_, IN const bool& bRecieved_ );
    bool SetRecieveFinalReward( IN const int& nCharType_, IN const bool& bRecieved_ );

    std::map< int, std::map< DWORD, int > >         m_mapCollectData;       // 전리품 획득 정보(<대륙 ID, < Collect ID, 얼마나 모았나 > >)
    std::map< int, std::map< DWORD, int > >         m_mapCollectDataPrev;   // 이전 전리품 획득 정보(게임 시작시 저장해두는 용도)
    std::map< int, bool >                           m_mapContinentData;     // 대륙 보상 정보(대륙ID, 지급했나)
};

/*

1. 패킷 플로우

1) DB_EVENT_ADVENTURE_DATA_REQ
- 유저 접속 시 S->DB 로 유저의 전리품 상황, 대륙별 보상, 최종 보상 받은 정보 받아와 서버에 저장하고

2) EVENT_ADVENTURE_DATA_ACK
- 접속한 유저에게 C->S 전리품 상황, 대륙별 보상, 최종 보상 받은 정보, 대륙 보상 리스트, 최종 보상 리스트를 전달합니다
접속시 한번 전달하고 이후 EVENT_ADVENTURE_DATA_REQ 로 요청마다 응답으로 보냅니다
오류 번호: ERR_ADVENTURE_01, 02

3) EVENT_ADVENTURE_DATA_REQ
- 클라이언트가 서버에게 유저데이터, 대륙보상, 최종보상 아이템 리스트를 요청합니다 C->S
noparam 으로 보내시면 됩니다
필요하시면 UI 클릭시마다 요청하셔도 됩니다
응답으로 2) EVENT_ADVENTURE_DATA_ACK 를 받습니다

5) EVENT_ADVENTURE_CONTINENT_REWARD_REQ C->S
- 클라이언트가 서버에게 대륙 보상을 달라고 요청 합니다
보상 받고 싶은 대륙ID 넣어서 보내주시면 됩니다
대륙ID는 커먼패킷의 KContinentRewardInfo 구조체에 정의되어 있습니다
m_kDropItemInfo는 빈 벡터로 보내주세요

6) EVENT_ADVENTURE_CONTINENT_REWARD_ACK S->C
- 서버가 클라에게 대륙 보상 요청 결과 보냅니다
m_kDropItemInfo 에 받은 보상 아이템 리스트 들어있구요
오류코드는
ERR_ADVENTURE_01, 02, 03, 04, 05, 06, 07, 13 보내고 내용은 NetError_def.h 참조 해주세요

7) EVENT_ADVENTURE_FINAL_REWARD_REQ C->S
- 클라가 서버에게 최종 보상을 달라고 요청 합니다
보상 받고 싶은 캐릭터 Type 넣어서 보내주시면 됩니다
캐릭터타입은 0 엘리시스부터 쭉 올라가 14 가 린입니다.. GCEnum값하고 맞췄어요

8) EVENT_ADVENTURE_FINAL_REWARD_ACK S->C
- 서버가 클라에게 최종 보상 요청 결과 보냅니다
m_kDropItemInfo 에 받은 보상 아이템 리스트 들어있구요
오류코드는 ERR_ADVENTURE_01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 14, 15
ERR_ADVENTURE_05 은 5, ERR_ADVENTURE_15 는 15.. 이런식으로 m_nOK 값이 설정됩니다
- 최종 보상을 받은 결과가 0으로 성공이면
서버에서 DB에 전리품 획득 카운트, 대륙보상 받은 여부, 최종 보상 받은 캐릭터 정보 갱신을 합니다
성공 값을 받으면 클라이언트 쪽에서도 해당 내용을 초기화합니다


게임시작, 게임종료 시 필요한 전리품 획득하는 처리 부분은 위에 반영되지 않았습니다


*/

// EVENT_ADVENTURE_CONTINENT_REWARD_ACK의 오류 코드
// 0 : 대륙 보상 요청 성공
// 1 : 이벤트기간이아님 
// 2 : 모험 이벤트 보상 정보가 없음
// 3 : 요청한대륙의보상아이템을이미받았음 
// 4 : 모험이벤트대륙보상정보가없음 
// 5 : 요청한대륙의보상아이템이보상리스트에없음 
// 6 : 대륙 보상 받기 DB 실패
// 7 : 대륙 보상 받기 DB 실패2
// 13 :  대륙보상을받을조건이아님  
// -98 : 아직처리중인작업입니다.

// EVENT_ADVENTURE_FINAL_REWARD_ACK의 오류 코드
// 0  : 최종 보상 요청 성공
// 1  : 이벤트 기간이 아님
// 2  : 모험 이벤트 보상 정보가 없음
// 3  : 요청한 대륙의 보상 아이템을 이미 받았음
// 4  : 모험 이벤트 대륙 보상 정보가 없음
// 5  : 요청한 대륙의 보상 아이템이 보상 리스트에 없음
// 6  : 대륙 보상 받기 DB 실패
// 7  : 대륙 보상 받기 DB 실패2
// 8  : 요청한 캐릭터의 보상 아이템을 이미 받았음
// 9  : 요청한 캐릭터의 모험 이벤트 최종 보상 정보가 없음
// 10 : 요청한 캐릭터의 보상 아이템이 보상 리스트에 없음
// 11 : 최종 보상 받기 DB 실패
// 12 : 최종 보상 받기 DB 실패2
// 15 : 모험 이벤트 미션 정보가 없음
// -98 : 아직처리중인작업입니다.

/*

최종 보상쪽 수정됨

서버 코드에서 스크립트 구조 변경되었고
이에따라 최종보상 리스트 자료구조가 맵에서 KDropItemInfo 변수로 변경됨
관련 코드들 전부 변경되었고
최종 보상 받은 정보를 서버나 DB에 따로 기록하지 않게 되었음.. 즉 최종 보상을 계속 받을 수 있음

클라에서 EVENT_ADVENTURE_FINAL_REWARD_REQ 를 C->S 로 ID만 보내게 변경됨(noparam)
그리고 서버에서 DB_EVENT_ADVENTURE_FINAL_REWARD_REQ 만들어서 db요청으로 사용..

commonpacket의 KFinalReward 제거

서버가 클라에게 주는 EVENT_ADVENTURE_FINAL_REWARD_ACK 에 멤버추가함..
-> KEVENT_ADVENTURE_DATA_ACK                       m_kAdventureData;      // 모험 이벤트 초기화된 전리품 정보 전달
최종 보상 보낸 뒤 EVENT_ADVENTURE_DATA_ACK 패킷 한번 더 보내는걸 위 ACK에 넣어서 한번에 전달하도록 수정
CharType 멤버 제거

EVENT_ADVENTURE_DATA_ACK S->C 응답 패킷에
유저의 최종 보상 받은 정보 (m_mapFinalData) 제거
m_mapFinalRewardInfo 가 m_kFinalRewardInfo 로 변경

*/
