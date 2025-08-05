#include "PreHackingCheckManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "Log4.h"

ImplementSingleton( KPreHackingCheckManager );
ImplOstmOperatorW2A( KPreHackingCheckManager );
NiImplementRootRTTI( KPreHackingCheckManager );

KPreHackingCheckManager::KPreHackingCheckManager(void)
:m_bLogShortGame(false)
,m_bIgnoreShortGame(false)
,m_dwThresholdTick(30000)
,m_fMaxIncAPRatio(2.f)
,m_dwDotaThresholdTick(1000*60*5)
,m_bPvpErrDisconnect(false)
,m_bDungeonMonErrDisconnect(false)
,m_bInfinityDungeonMonsterHackErrDisconnect( false )
,m_dwInfinityDungeonMinRoundTerm_Sec(10 * 1000)
,m_dwInfinityDungeonMaxSummonCount(1000)
{
    m_mapCheckOption.clear();
    m_mapDungeonCheckData.clear();
    m_mapMaxAtkLimit.clear();
    m_mapPvpLifeTable.clear();
    m_mapPvpLvMaxExp.clear();
    m_mapDungeonMonsterCount.clear();
}

KPreHackingCheckManager::~KPreHackingCheckManager(void)
{
}

ImplToStringW( KPreHackingCheckManager )
{
    KLocker lock0( m_csCheckOption );
    KLocker lock1( m_csMinigameCheck );
    KLocker lock2( m_csPreHackCheck );
    KLocker lock3( m_csGameCheck );
    KLocker lock4( m_csAbility );
    KLocker lock5( m_csCharacterStat );
    KLocker lock6( m_csDungeonMonsterCnt );
    KLocker lock7( m_csInfinityDungeonMonsterHack );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapDungeonCheckData.size() )
        << TOSTRINGWb( m_bLogShortGame )
        << TOSTRINGWb( m_bIgnoreShortGame )
        << TOSTRINGW( m_dwThresholdTick )
        << TOSTRINGW( m_dwDotaThresholdTick )
        << TOSTRINGW( m_mapMaxAtkLimit.size() )
        << TOSTRINGW( m_fMaxIncAPRatio )
        << TOSTRINGW( m_mapCharacterStatTable.size() )
        << TOSTRINGW( m_mapPvpLifeTable.size() )
        << TOSTRINGW( m_mapPvpLvMaxExp.size() )
        << TOSTRINGW( m_mapDungeonMonsterCount.size() )
        << TOSTRINGW( m_dwInfinityDungeonMinRoundTerm_Sec )
        << TOSTRINGW( m_dwInfinityDungeonMaxSummonCount );
}

bool KPreHackingCheckManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KPreHackingCheckManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    const std::string strScriptName = "InitPreHackCheck.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadOption( kLuaMng ), return false );
    _JIF( LoadMinigameCheckData( kLuaMng ), return false );
    _JIF( LoadDungeonCheckData( kLuaMng ), return false );
    _JIF( LoadAbuseCheckData( kLuaMng ), return false );
    _JIF( LoadAbilityCheckData( kLuaMng ), return false );
    _JIF( LoadCharacterStatTable( kLuaMng ), return false );
    _JIF( LoadPvpLifeTable( kLuaMng ), return false );
    _JIF( LoadDungeonMonsterCountTable( kLuaMng ), return false );
    _JIF( LoadInfinityDungeonMonsterHackCheck( kLuaMng ), return false );

    return true;
}

bool KPreHackingCheckManager::LoadOption( KLuaManager& kLuaMng_ )
{
    std::map< int, std::map< int, bool > > mapOption;

    _JIF( kLuaMng_.BeginTable( "CheckOption" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::map< int, bool > mapSetting;
        int nHackType = -1;         // 해킹타입ID
        bool bCheckEnable = false;  // 체크여부
        bool bWriteDB = false;      // DB기록 여부
        bool bWriteFile = false;    // File기록 여부
        bool bNoticeMsg = false;    // 클라이언트 공지 여부
        bool bInitialize = false;   // 초기화 여부

        if( kLuaMng_.GetValue( 1, nHackType ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, bCheckEnable ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, bWriteDB ) != S_OK ) break;
        if( kLuaMng_.GetValue( 4, bWriteFile ) != S_OK ) break;
        if( kLuaMng_.GetValue( 5, bNoticeMsg ) != S_OK ) break;
        if( kLuaMng_.GetValue( 6, bInitialize ) != S_OK ) break;

        mapSetting.insert( std::make_pair( OPT_CHECK_ENABLE, bCheckEnable ) );
        mapSetting.insert( std::make_pair( OPT_WRITE_DB, bWriteDB ) );
        mapSetting.insert( std::make_pair( OPT_WRITE_FILE, bWriteFile ) );
        mapSetting.insert( std::make_pair( OPT_NOTICE_MSG, bNoticeMsg ) );
        mapSetting.insert( std::make_pair( OPT_INITIALIZE, bInitialize ) );

        if( mapOption.insert( std::make_pair( nHackType, mapSetting ) ).second == false )
        {
            START_LOG( cerr, L"중복된 옵션 데이터. TypeID : " << nHackType ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"해킹체크 옵션 Load 완료. size : " << mapOption.size() ) << END_LOG;

    KLocker lock( m_csCheckOption );
    m_mapCheckOption.swap( mapOption );
    return true;
}

bool KPreHackingCheckManager::LoadMinigameCheckData( KLuaManager& kLuaMng_ )
{
    std::vector< KMiniGameScore > vecScoreTable;

    _JIF( kLuaMng_.BeginTable( "MiniGameTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KMiniGameScore kScoreInfo;
        kScoreInfo.m_prTimeScope.first = 0;
        kScoreInfo.m_prTimeScope.second = 0;
        kScoreInfo.m_fScore = 0.f;

        if( kLuaMng_.GetValue( 1, kScoreInfo.m_prTimeScope.first ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kScoreInfo.m_prTimeScope.second ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, kScoreInfo.m_fScore ) != S_OK ) break;

        vecScoreTable.push_back( kScoreInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"미니게임 점수 테이블 Load 완료. size : " << vecScoreTable.size() ) << END_LOG;

    KLocker lock( m_csMinigameCheck );
    m_vecMinigameCheckTable.swap( vecScoreTable );
    return true;
}

bool KPreHackingCheckManager::LoadDungeonCheckData( KLuaManager& kLuaMng_ )
{
    std::map< std::pair<int,int>, std::pair<int,DWORD> > mapCheckData;
    mapCheckData.clear();

    _JIF( kLuaMng_.BeginTable( "DungeonTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        std::pair<int,int> prKey;       // <DungeonID,Difficult>
        std::pair<int,DWORD>  prData;   // <MinStage,MinClearTime>
        prKey.first = -1;
        prKey.second = -1;
        prData.first = -1;
        prData.second = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, prKey.first ) == S_OK, return false );   // DungeonID
        _JIF( kLuaMng_.GetValue( 2, prKey.second ) == S_OK, return false );  // Difficult
        _JIF( kLuaMng_.GetValue( 3, prData.first ) == S_OK, return false );  // MinStage
        _JIF( kLuaMng_.GetValue( 4, prData.second ) == S_OK, return false ); // MinClearTime
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( mapCheckData.insert( std::make_pair( prKey, prData ) ).second == false )
        {
            START_LOG( cerr, L"던전체크 데이터 입력중 중복된 Key값. DungeonID : " << prKey.first << L",Difficult : " << prKey.second ) << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"던전체크 데이터 로드 완료. size : " << mapCheckData.size() ) << END_LOG;

    KLocker lock( m_csPreHackCheck );
    m_mapDungeonCheckData.swap( mapCheckData );
    return true;
}

bool KPreHackingCheckManager::LoadAbuseCheckData( KLuaManager& kLuaMng_ )
{
    bool bLogShortGame = false ;
    bool bIgnoreShortGame = false;
    DWORD dwThresholdTick = 30000;
    DWORD dwDotaThresholdTick = 1000 * 60 * 5; // 5분

    _JIF( kLuaMng_.BeginTable( "GameAbuseTable" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "LogShortGame", bLogShortGame ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "IgnoreShortGame", bIgnoreShortGame ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ThresholdTick", dwThresholdTick ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DotaThresholdTick", dwDotaThresholdTick ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"어뷰징 체크 Load 완료. LogShortGame : " << bLogShortGame << L",IgnoreShortGame : " << bIgnoreShortGame << L",ThresholdTick : " << dwThresholdTick << L",DotaThresholdTick : " << dwDotaThresholdTick ) << END_LOG;

    KLocker lock( m_csGameCheck );
    m_bLogShortGame    = bLogShortGame;
    m_bIgnoreShortGame = bIgnoreShortGame;
    m_dwThresholdTick  = dwThresholdTick;
    m_dwDotaThresholdTick = dwDotaThresholdTick;
    return true;
}

bool KPreHackingCheckManager::LoadAbilityCheckData( KLuaManager& kLuaMng_ )
{
    float fMaxIncAPRatio = 0.f;
    std::map< char, float > mapMaxAtk;

    _JIF( kLuaMng_.BeginTable( "AbilityTable" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxIncAPRatio", fMaxIncAPRatio ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "MaxAtkLimit" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nLevel = 0;
        float fMaxAtkLimit = 0.f;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fMaxAtkLimit ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( mapMaxAtk.insert( std::make_pair( (char)nLevel, fMaxAtkLimit ) ).second == false )
        {
            START_LOG( cerr, L"중복된 레벨값. Level : " << nLevel << L",MaxAtk : " << fMaxAtkLimit ) << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"능력치 체크 Load 완료. size : " << mapMaxAtk.size() ) << END_LOG;

    KLocker lock( m_csAbility );
    m_fMaxIncAPRatio = fMaxIncAPRatio;
    m_mapMaxAtkLimit.swap( mapMaxAtk );
    return true;
}

int KPreHackingCheckManager::CheckMiniGameScore( IN const DWORD dwPlayTime_, IN const DWORD dwScore_ )
{
    KLocker lock( m_csMinigameCheck );

    if( CheckOption( GHT_MINIGAME_TIME_SCORE, OPT_CHECK_ENABLE ) == false )
    {
        START_LOG( clog, L"미니게임 해킹체크 안함." ) << END_LOG;
        return GHT_NORMAL;
    }

    std::vector< KMiniGameScore >::const_iterator cvit;
    for( cvit = m_vecMinigameCheckTable.begin() ; cvit != m_vecMinigameCheckTable.end() ; ++cvit )
    {
        if( dwPlayTime_ >= cvit->m_prTimeScope.first && dwPlayTime_ <= cvit->m_prTimeScope.second )
            break;
    }

    if( cvit == m_vecMinigameCheckTable.end() )
    {
        START_LOG( cerr, L"미니게임 점수 검사중 : 테이블에 없는 시간 값." )
            << BUILD_LOG( dwPlayTime_ )
            << BUILD_LOG( dwScore_ ) << END_LOG;
        return GHT_MINIGAME_TIME_SCORE;
    }

    if( dwScore_ > dwPlayTime_ * cvit->m_fScore )
    {
        START_LOG( cerr, L"미니게임 점수 검사중 : 플레이시간 대비 점수값 초과." )
            << BUILD_LOG( dwPlayTime_ )
            << BUILD_LOG( dwScore_ )
            << BUILD_LOG( dwPlayTime_ * cvit->m_fScore ) << END_LOG;
        return GHT_MINIGAME_TIME_SCORE;
    }

    START_LOG( clog, L"미니게임 점수 검사결과 : 정상 데이터" )
        << BUILD_LOG( dwPlayTime_ )
        << BUILD_LOG( dwScore_ )
        << BUILD_LOG( dwPlayTime_ * cvit->m_fScore ) << END_LOG;

    return GHT_NORMAL;
}

int KPreHackingCheckManager::CheckMinStageCheck( IN const int nDungeonID_, IN const int nDifficult_, IN const int nStageCount_ )
{
    KLocker lock( m_csPreHackCheck );

    if( CheckOption( GHT_DUNGEON_STAGE_COUNT, OPT_CHECK_ENABLE ) == false )
    {
        START_LOG( clog, L"최소스테이지 체크 안함." ) << END_LOG;
        return GHT_NORMAL;
    }

    std::map< std::pair<int,int>, std::pair<int,DWORD> >::const_iterator cmit;
    cmit = m_mapDungeonCheckData.find( std::make_pair( nDungeonID_, nDifficult_ ) );
    if( cmit == m_mapDungeonCheckData.end() )
    {
        START_LOG( cerr, L"등록되지 않은 던전,난이도 정보. 우선은 통과. DungeonID : " << nDungeonID_ << L",Difficult : " << nDifficult_ ) << END_LOG;
        return GHT_NORMAL;
    }

    if( nStageCount_ < cmit->second.first )
    {
        START_LOG( cwarn, L"해킹의심됨..클리어 스테이지 수가 최소 스테이지 수보다 적음." )
            << BUILD_LOG( nDungeonID_ )
            << BUILD_LOG( nDifficult_ )
            << BUILD_LOG( nStageCount_ )
            << BUILD_LOG( cmit->second.first ) << END_LOG;

        return GHT_DUNGEON_STAGE_COUNT;
    }

    START_LOG( clog, L"최소 스테이지수 체크 통과.. [" << nDungeonID_ << L"/" << nDifficult_ << L"/" << nStageCount_ << L"]" ) << END_LOG;
    return GHT_NORMAL;
}

int KPreHackingCheckManager::CheckClearTimeCheck( IN const int nDungeonID_, IN const int nDifficult_, IN const DWORD dwClearTime_ )
{
    KLocker lock( m_csPreHackCheck );

    if( CheckOption( GHT_DUNGEON_CLEAR_TIME, OPT_CHECK_ENABLE ) == false )
    {
        START_LOG( clog, L"클리어시간 체크 안함." ) << END_LOG;
        return GHT_NORMAL;
    }

    std::map< std::pair<int,int>, std::pair<int,DWORD> >::const_iterator cmit;
    cmit = m_mapDungeonCheckData.find( std::make_pair( nDungeonID_, nDifficult_ ) );
    if( cmit == m_mapDungeonCheckData.end() )
    {
        START_LOG( cerr, L"등록되지 않은 던전,난이도 정보. 우선은 통과. DungeonID : " << nDungeonID_ << L",Difficult : " << nDifficult_ ) << END_LOG;
        return GHT_NORMAL;
    }

    if( dwClearTime_ < cmit->second.second )
    {
        START_LOG( cwarn, L"해킹의심됨..클리어 시간이 최소 클리어 시간보다 짧음." )
            << BUILD_LOG( nDungeonID_ )
            << BUILD_LOG( nDifficult_ ) 
            << BUILD_LOG( dwClearTime_ )
            << BUILD_LOG( cmit->second.second ) << END_LOG;

        return GHT_DUNGEON_CLEAR_TIME;
    }

    START_LOG( clog, L"클리어시간 체크 통과.. [" << nDungeonID_ << L"/" << nDifficult_ << L"/" << dwClearTime_ << L"]" ) << END_LOG;
    return GHT_NORMAL;
}

void KPreHackingCheckManager::DumpHackingProtectionMode()
{
    KLocker lock( m_csGameCheck );
    START_LOG( clog, L" Hacking Protection Mode." )
        << BUILD_LOG( m_bLogShortGame )
        << BUILD_LOG( m_bIgnoreShortGame )
        << BUILD_LOG( m_dwThresholdTick )
        << BUILD_LOG( m_dwDotaThresholdTick ) << END_LOG;
}

int KPreHackingCheckManager::CheckMaxAtkLimit( IN const char cLevel_, IN const std::vector<float>& vecAtkData_ )
{
    std::stringstream stmAtk;
    float fMaxAtk = GetMaxAtkLimit( cLevel_ );

    KLocker lock( m_csAbility );

    if( CheckOption( GHT_ATK_HACK, OPT_CHECK_ENABLE ) == false )
    {
        START_LOG( clog, L"공격력 해킹 체크 안함. 통과." ) << END_LOG;
        return GHT_NORMAL;
    }

    for( std::vector<float>::const_iterator cvit = vecAtkData_.begin() ; cvit != vecAtkData_.end() ; ++cvit )
    {
        stmAtk << *cvit << ",";

        if( *cvit > fMaxAtk )
        {
            START_LOG( cwarn, L"해킹의심됨..공격력 최대수치가 넘어감." )
                << BUILD_LOG( fMaxAtk )
                << BUILD_LOG( *cvit )
                << BUILD_LOG( KncUtil::toWideString( stmAtk.str() ) )
                << BUILD_LOG( vecAtkData_.size() ) << END_LOG;

            return GHT_ATK_HACK;
        }
    }

    START_LOG( clog, L"공격력 수치 정상. size :" << vecAtkData_.size() << L"[" << KncUtil::toWideString(stmAtk.str()) << L"]" << L", Limit : " << fMaxAtk ) << END_LOG;
    return GHT_NORMAL;
}

int KPreHackingCheckManager::CheckMaxIncreaseAtkRate( IN const std::vector<float>& vecIncAPRatio_ )
{
    std::stringstream stmAP;
    float fIncAPRatio = GetMaxIncAPRatio();

    KLocker lock( m_csAbility );

    if( CheckOption( GHT_INC_ATK_RATE_HACK, OPT_CHECK_ENABLE ) == false )
    {
        START_LOG( clog, L"공격력증가율 해킹 체크 안함. 통과." ) << END_LOG;
        return GHT_NORMAL;
    }

    for( std::vector<float>::const_iterator cvit = vecIncAPRatio_.begin() ; cvit != vecIncAPRatio_.end() ; ++cvit )
    {
        stmAP << *cvit << ",";

        if( *cvit > fIncAPRatio )
        {
            START_LOG( cwarn, L"해킹의심됨..최대 공격력증가 비율값이 넘어감." )
                << BUILD_LOG( fIncAPRatio )
                << BUILD_LOG( *cvit )
                << BUILD_LOG( KncUtil::toWideString( stmAP.str() ) )
                << BUILD_LOG( vecIncAPRatio_.size() ) << END_LOG;

            return GHT_INC_ATK_RATE_HACK;
        }
    }

    START_LOG( clog, L"공격력증가 비율값 정상. size :" << vecIncAPRatio_.size() << L"[" << KncUtil::toWideString(stmAP.str()) << L"]" << L", Limit : " << fIncAPRatio ) << END_LOG;
    return GHT_NORMAL;
}

float KPreHackingCheckManager::GetMaxAtkLimit( IN const char cLevel_ )
{
    std::map< char, float >::const_iterator cmit;

    KLocker lock( m_csAbility );
    if( m_mapMaxAtkLimit.empty() )
    {
        START_LOG( cerr, L"레벨별 최대값 데이터가 없음. 우선은 통과.. Level : " << (int)cLevel_ ) << END_LOG;
        return 99999.f; // 임의로 큰값
    }

    cmit = m_mapMaxAtkLimit.find( cLevel_ );
    if( cmit == m_mapMaxAtkLimit.end() )
    {
        START_LOG( cerr, L"해당하는 레벨값이 없음. 우선은 통과.. Level : " << (int)cLevel_ ) << END_LOG;
        return m_mapMaxAtkLimit.rbegin()->second;
    }

    return cmit->second;
}

bool KPreHackingCheckManager::CheckOption( IN const int nID_, IN const int nOptionID_ )
{
    std::map< int, std::map< int, bool > >::const_iterator cmit;
    std::map< int, bool >::const_iterator cmit2;

    KLocker lock( m_csCheckOption );

    cmit = m_mapCheckOption.find( nID_ );
    if( cmit == m_mapCheckOption.end() )
    {
        START_LOG( cerr, L"해당하는 해킹타입이 없음. TypeID : " << nID_ ) << END_LOG;
        return false;
    }

    cmit2 = cmit->second.find( nOptionID_ );
    if( cmit2 == cmit->second.end() )
    {
        START_LOG( cerr, L"해당하는 옵션값이 없음. OptionID: " << nOptionID_ ) << END_LOG;
        return false;
    }

    return cmit2->second;
}

void KPreHackingCheckManager::PrintCheckOption( std::wostream& stm_ ) const 
{
    std::map< int, std::map< int, bool > >::const_iterator cmit;
    std::map< int, bool >::const_iterator cmit2;

    KLocker lock( m_csCheckOption );

    stm_ << L" --- Hacking Check Option(size:" << m_mapCheckOption.size() << L") --- " << std::endl;
    for( cmit = m_mapCheckOption.begin() ; cmit != m_mapCheckOption.end() ; ++cmit )
    {
        stm_ << L" TypeID : " << cmit->first;
        for( cmit2 = cmit->second.begin() ; cmit2 != cmit->second.end() ; ++cmit2 )
        {
            stm_ << L", " << cmit2->first << L":" << ( cmit2->second == true ? L"True" : L"False" );
        }
        stm_ << std::endl;
    }
    stm_ << L" --- Hacking Check Option End --- " << std::endl;
}

bool KPreHackingCheckManager::LoadCharacterStatTable( KLuaManager& kLuaMng_ )
{
    std::map< std::pair<char,char>, KSimpleCharacterStat >  mapStatTable;

    _JIF( kLuaMng_.BeginTable( "CharacterStatTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nCharacterID;
        int nPromotion;
        std::pair<char,char> prKey;
        KSimpleCharacterStat kData;
        kData.Init();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nCharacterID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nPromotion ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kData.m_fIncHP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kData.m_fDamagefactor ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, kData.m_fWalkSpeed ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 6, kData.m_fRunSpeed ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        prKey.first = (char)nCharacterID;
        prKey.second = (char)nPromotion;

        mapStatTable[ prKey ] = kData;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"캐릭터 스텟 테이블 Load 완료. size : " << mapStatTable.size() ) << END_LOG;

    KLocker lock( m_csCharacterStat );
    m_mapCharacterStatTable.swap( mapStatTable );
    return true;
}

bool KPreHackingCheckManager::GetCharacterStat( IN std::pair<char,char> prKey_, OUT KSimpleCharacterStat& kData_ )
{
    std::map< std::pair<char,char>, KSimpleCharacterStat >::const_iterator cmit;

    KLocker lock( m_csCharacterStat );
    cmit = m_mapCharacterStatTable.find( prKey_ );
    _JIF( cmit != m_mapCharacterStatTable.end(), return false );
    kData_ = cmit->second;
    return true;
}

bool KPreHackingCheckManager::LoadPvpLifeTable( KLuaManager& kLuaMng_ )
{
    bool bDisconnect = false;
    std::map<int,USHORT> mapPvpLifeTable;
    std::map<int,__int64> mapPvpLvMaxExp;
    mapPvpLifeTable.clear();
    mapPvpLvMaxExp.clear();

    _JIF( kLuaMng_.BeginTable( "PvpTable" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PvpErrDisconnect", bDisconnect ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "PvpLifeTable" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMapID = -1;
        USHORT usLifeCount = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nMapID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, usLifeCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( nMapID > -1 ) {
            mapPvpLifeTable[nMapID] = usLifeCount;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "PvpLvMaxExp" ) == S_OK, return false );
    for ( int j = 1 ; ; ++j ) {
        int nLv = -1;
        DWORD dwMaxExp = 0;

        if ( kLuaMng_.BeginTable( j ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nLv ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwMaxExp ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( nLv > -1 ) {
            mapPvpLvMaxExp[nLv] = static_cast<__int64>( dwMaxExp );
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"PVP LifeCount Table Load 완료. " )
        << BUILD_LOG( bDisconnect )
        << BUILD_LOG( mapPvpLifeTable.size() )
        << BUILD_LOG( mapPvpLvMaxExp.size() )
        << END_LOG;

    KLocker lock( m_csPvpLife );
    m_bPvpErrDisconnect = bDisconnect;
    m_mapPvpLifeTable.swap( mapPvpLifeTable );
    m_mapPvpLvMaxExp.swap( mapPvpLvMaxExp );
    return true;
}

bool KPreHackingCheckManager::PvpLifeCountCheck( IN const int& nMapID_, OUT USHORT& usLifeCount_ )
{
    usLifeCount_ = 0;
    std::map<int,USHORT>::iterator mit;

    KLocker lock( m_csPvpLife );
    mit = m_mapPvpLifeTable.find( nMapID_ );
    if ( mit == m_mapPvpLifeTable.end() ) {
        return false;
    }

    usLifeCount_ = mit->second;
    return true;
}

bool KPreHackingCheckManager::GetPvpLvMaxExp( IN const int& nLv_, OUT __int64& biMaxExp_ )
{
    biMaxExp_ = 0;

    KLocker lock( m_csPvpLife );

    std::map<int,__int64>::const_iterator mit;
    mit = m_mapPvpLvMaxExp.find( nLv_ );
    if ( mit == m_mapPvpLvMaxExp.end() ) {
        return false;
    }

    biMaxExp_ = mit->second;
    return true;
}

bool KPreHackingCheckManager::LoadDungeonMonsterCountTable( KLuaManager& kLuaMng_ )
{
    bool bDungeonMonErrDisconnect = false;
    std::map<int,int> mapDungeonMonsterCount;
    mapDungeonMonsterCount.clear();

    _JIF( kLuaMng_.BeginTable( "DungeonMonsterCount" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonMonsterErrDisconnect", bDungeonMonErrDisconnect ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "MonsterCountInfo" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nModeID = -1;
        int nMonsterCount = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nMonsterCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( nModeID > -1 ) {
            mapDungeonMonsterCount[nModeID] = nMonsterCount;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Dungeon Monster Count Table Load 완료. " )
        << BUILD_LOG( bDungeonMonErrDisconnect )
        << BUILD_LOG( mapDungeonMonsterCount.size() )
        << END_LOG;

    {
        KLocker lock( m_csDungeonMonsterCnt );
        m_bDungeonMonErrDisconnect = bDungeonMonErrDisconnect;
        m_mapDungeonMonsterCount.swap( mapDungeonMonsterCount );
    }

    return true;
}

bool KPreHackingCheckManager::DungeonMonsterCountCheck( IN const int& nModeID_, OUT int& nMonsterCnt_ )
{
    nMonsterCnt_ = 0;
    std::map<int,int>::iterator mit;

    KLocker lock( m_csDungeonMonsterCnt );
    mit = m_mapDungeonMonsterCount.find( nModeID_ );
    if ( mit == m_mapDungeonMonsterCount.end() ) {
        return false;
    }

    nMonsterCnt_ = mit->second;
    return true;
}

bool KPreHackingCheckManager::LoadInfinityDungeonMonsterHackCheck( IN OUT KLuaManager& kLuaMng_ )
{
    bool bInfinityDungeonMonsterHackErrDisconnect = false;
    DWORD dwInfinityDungeonMinRoundTerm_Sec = 0;
    DWORD dwInfinityDungeonMaxSummonCount = 0;

    _JIF( kLuaMng_.BeginTable( "InfinityDungeonMonsterHackCheck" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "InfinityDungeonMonsterHackErrDisconnect", bInfinityDungeonMonsterHackErrDisconnect ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MinRoundTerm_Sec", dwInfinityDungeonMinRoundTerm_Sec ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxSummonCount", dwInfinityDungeonMaxSummonCount ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"InfinityDungeonMonsterHackCheck 로드 완료. " )
        << BUILD_LOG( bInfinityDungeonMonsterHackErrDisconnect )
        << BUILD_LOG( dwInfinityDungeonMinRoundTerm_Sec )
        << BUILD_LOG( dwInfinityDungeonMaxSummonCount )
        << END_LOG;

    {
        KLocker lock( m_csInfinityDungeonMonsterHack );

        m_bInfinityDungeonMonsterHackErrDisconnect = bInfinityDungeonMonsterHackErrDisconnect;
        m_dwInfinityDungeonMinRoundTerm_Sec = dwInfinityDungeonMinRoundTerm_Sec;
        m_dwInfinityDungeonMaxSummonCount = dwInfinityDungeonMaxSummonCount;
    }

    return true;
}

bool KPreHackingCheckManager::IsInfinityDungeonMonsterHackErrDisconnect()
{
    KLocker lock( m_csInfinityDungeonMonsterHack );

    if ( false == CheckOption( GHT_INFINITY_DUNGEON_MONSTER_HACK, OPT_CHECK_ENABLE ) ) {
        return false;
    }

    return m_bInfinityDungeonMonsterHackErrDisconnect;
}

bool KPreHackingCheckManager::IsInfinityDungeonMonsterLimitOver( IN const DWORD dwSummonGap_, IN const DWORD dwSummonMonsterCount_ )
{
    /* return true: 해킹임
       return false: 해킹 아님
    */
    KLocker lock( m_csInfinityDungeonMonsterHack );

    // 소환 간격이 너무 짧은 경우 해킹으로 처리
    if ( dwSummonGap_ < m_dwInfinityDungeonMinRoundTerm_Sec ) {
        return true;
    }

    // 너무 많이 소환하는 경우 해킹으로 처리
    if ( dwSummonMonsterCount_ > m_dwInfinityDungeonMaxSummonCount ) {
        return true;
    }

    return false;
}