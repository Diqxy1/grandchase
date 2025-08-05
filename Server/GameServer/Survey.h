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

struct KDropItemInfo;
struct KSurveyInfo;
struct KSurveyQuestion;
struct KSurveyInfoPack;
class KSerBuffer;

class KSurvey
{
    DeclareSingleton( KSurvey );
    NiDeclareRootRTTI( KSurvey );
    DeclToStringW;

public:
    enum SurveyType
    {
        ST_GAME_CONNECT         = 0,    // 게임서버 접속시.
        ST_GAME_CLEAR           = 1,    // 던전 클리어시.
        ST_CAHR_LEVELUP         = 2,    // 캐릭터 레벨업시.
        ST_MAX,
    };

    enum UserType 
    {
        UT_NEW                   = 1,    // 신규 유저.
        UT_SAVE                  = 2,    // 기존 유저.
        UT_RETURN                = 3,    // 복귀 유저.
        RT_MAX,
    };

    KSurvey(void);
    ~KSurvey(void);
    bool IsRun() const;

    void UpdateSurveyData( IN const KECN_SURVEY_DATA_NOT& kCenterPacket );
    int GetSurveyVersion(){ KLocker lock( m_csSurvey ); return m_nVersion; };
    bool IsReturnUser( IN const time_t& tmLastLogin );
    bool IsNewUser( IN const time_t& tmFirstLogin );
    bool GetSurveyList( IN const int& nLanguageCode, IN const int& nType, IN const int& nDataFirst, IN const int& nDataSecond, OUT std::map< int, KSerBuffer >& mapData );
    bool GetTypeRewardList( IN const int& nType, OUT std::vector< KDropItemInfo >& vecReward );
    void GetRewardList( OUT std::map< int, std::vector< KDropItemInfo > >& mapSurveyRewardInfo );
    bool CheckValidSurvey( IN const int& nLanguageCode, IN std::map< int, std::vector<int> >& mapNumberAnswer, IN std::map< int, std::wstring >& mapStringAnswer );

protected:
    mutable KncCriticalSection                          m_csSurvey;

    int                                                 m_nVersion;
    time_t                                              m_tmBegin;
    time_t                                              m_tmEnd;
    time_t                                              m_tmReturnTime;

    std::map< int, std::map< int, KSerBuffer > >   m_mapCompressedSurveyInfoPack;
    std::map< int, std::map< std::pair<int,int>, std::vector<int> > > m_mapSurveyTypeInfo; // 각 타입별 설문지 정보.
    std::map< int, std::vector< KDropItemInfo > >       m_mapSurveyRewardInfo;
};

DefSingletonInline( KSurvey );
DeclOstmOperatorA( KSurvey );

//=========================================================================================//

class KSurveyUserData
{
public:
    KSurveyUserData();
    ~KSurveyUserData();

    void SetUserSurveyData( IN const std::map< int, std::set< std::pair<int,int> > >& mapUserSurveyInfo );
    bool CheckSurveyStatus( IN const int& nType, IN const int& nDataFirst, IN const int& nDataSecond );
    bool UpdateSurveyStatus( IN const int& nType, IN const int& nDataFirst, IN const int& nDataSecond );

protected:
    std::map< int, std::set< std::pair<int,int> > > m_mapUserSurveyInfo;
};