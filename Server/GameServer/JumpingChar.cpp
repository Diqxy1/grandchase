#include "JumpingChar.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KJumpingChar );
ImplOstmOperatorW2A( KJumpingChar );
NiImplementRootRTTI( KJumpingChar );

KJumpingChar::KJumpingChar(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwJumpingCharLevel(70) // 기본 70 Lv로 기본값 세팅.
{
    m_setJumpingCharEnableList.clear();
    m_mapDungeonClearList.clear();
    m_mapJumpingRewardItem.clear();
    m_mapSkillRewardList.clear();
    m_mapSkillGroupRewardList.clear();
    m_mapMissionClearList.clear();
    m_vecDefaultRewardList.clear();
    m_mapJumpingCharPromotion.clear();
}

KJumpingChar::~KJumpingChar(void)
{
}

ImplToStringW( KJumpingChar )
{
    KLocker lock( m_csJumpingChar );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwJumpingCharLevel )
        << TOSTRINGW( m_mapJumpingCharPromotion.size() )
        << TOSTRINGW( m_setJumpingCharEnableList.size() )
        << TOSTRINGW( m_setJumpingCharEnableList.size() )
        << TOSTRINGW( m_mapDungeonClearList.size() )
        << TOSTRINGW( m_mapJumpingRewardItem.size() )
        << TOSTRINGW( m_mapSkillRewardList.size() )
        << TOSTRINGW( m_mapSkillGroupRewardList.size() )
        << TOSTRINGW( m_mapMissionClearList.size() )
        << TOSTRINGW( m_vecDefaultRewardList.size() )
        << TOSTRINGW( m_mapMissionRegisterList.size() );
}

bool KJumpingChar::LoadScript()
{
    return _LoadScript(std::string());
}

bool KJumpingChar::_LoadScript( OUT std::string& strScript_ )
{
    time_t tmBegin;
    time_t tmEnd;
    DWORD  dwJumpingCharLevel;
    std::map<char, char> mapJumpingCharPromotion;
    std::set<char> setJumpingCharEnableList;
    std::map<char, std::vector<int>> mapDungeonClearList;
    std::map<char, VEC_REWARD> mapJumpingRewardItem;
    std::map<char, std::vector<int>> mapSkillRewardList;
    std::map<char, std::vector<int>> mapSkillGroupRewardList;
    std::map<char, std::vector<int>> mapMissionClearList;
    std::map<char, std::vector<int>> mapMissionRegisterList;
    VEC_REWARD vecDefaultRewardList;
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitJumpingChar.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( kLuaMng.GetValue( "JumpingCharLevel", dwJumpingCharLevel ) == S_OK, return false );
    // 점핑 캐릭터 클리어 던전 정보 가져오기.
    _JIF( LoadJumpingCharPromotion( kLuaMng, std::string("JumpingCharPromotion"), mapJumpingCharPromotion ), return false );

    // 점핑 가능한 캐릭터 리스트 가져오기.
    _JIF( LoadSetCharData( kLuaMng, std::string("JumpingEnableCharList"), setJumpingCharEnableList ), return false );

    // 점핑 캐릭터 클리어 던전 정보 가져오기.
    _JIF( LoadDungeonClearList( kLuaMng, std::string("CharClearDungeonInfo"), mapDungeonClearList ), return false );

    // 점핑 캐릭터 보상 아이템 정보 가져오기.
    _JIF( LoadRewardItemInfo( kLuaMng, std::string("CharRewardItemInfo"), mapJumpingRewardItem ), return false );

    // 점핑 캐릭터 스킬 보상 정보 가져오기.
    _JIF( LoadRewardSkillList( kLuaMng, std::string("CharRewardSkillInfo"), mapSkillRewardList ), return false );

    // 점핑 캐릭터 스킬 그룹 보상 정보 가져오기.
    _JIF( LoadRewardUnLockSkillList( kLuaMng, std::string("CharRewardUnlockSkillGroup"), mapSkillGroupRewardList ), return false );

    // 점핑 캐릭터 미션 클리어 리스트 정보 가져오기.
    _JIF( LoadClearMissionList( kLuaMng, std::string("CharClearMissionInfo"), mapMissionClearList ), return false );

    // 점핑 캐릭터 하지 않는 유저 보상 아이템 정보 가져오기.
    _JIF( LoadReward( kLuaMng, std::string("DefaultRewardItem"), vecDefaultRewardList ), return false );

    // 점핑 캐릭터 미션 등록 리스트 정보 가져오기.
    _JIF( LoadRegisterMissionList( kLuaMng, std::string("CharRegisterMissionInfo"), mapMissionRegisterList ), return false );

    START_LOG( cerr, L"점핑 캐릭터 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csJumpingChar );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwJumpingCharLevel = dwJumpingCharLevel;
        m_mapJumpingCharPromotion.swap( mapJumpingCharPromotion );
        m_setJumpingCharEnableList.swap( setJumpingCharEnableList );
        m_mapDungeonClearList.swap( mapDungeonClearList );
        m_mapJumpingRewardItem.swap( mapJumpingRewardItem );
        m_mapSkillRewardList.swap( mapSkillRewardList );
        m_mapSkillGroupRewardList.swap( mapSkillGroupRewardList );
        m_mapMissionClearList.swap( mapMissionClearList );
        m_vecDefaultRewardList.swap( vecDefaultRewardList );
        m_mapMissionRegisterList.swap( mapMissionRegisterList );

        return true;
    }
}

bool KJumpingChar::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KJumpingChar::LoadVectorCharData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<char>& vecCharInfo_ )
{
    vecCharInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        vecCharInfo_.push_back( (char)nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KJumpingChar::LoadVectorIntData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<int>& vecIntInfo_ )
{
    vecIntInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        vecIntInfo_.push_back( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KJumpingChar::LoadDungeonClearList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, std::vector<int>>& mapDungeonClearList_ )
{
    mapDungeonClearList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<int> vecInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadVectorIntData( kLuaMng_, std::string("DungeonClearList"), vecInfo ), return false );

        if( mapDungeonClearList_.insert( std::make_pair( (char)nChar, vecInfo ) ).second == false ) {
            START_LOG( cerr, L"던전 클리어 정보 중복 데이터 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::LoadRewardItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< char, VEC_REWARD >& mapJumpingRewardItem_ )
{
    mapJumpingRewardItem_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        VEC_REWARD vecItemInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("DefaultItem"), vecItemInfo ), return false );

        if( mapJumpingRewardItem_.insert( std::make_pair( (char)nChar, vecItemInfo ) ).second == false ) {
            START_LOG( cerr, L"점핑 캐릭터 보상 아이템 세팅중 중복된 캐릭터 CharType :" << nChar )
                << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KJumpingChar::LoadRewardSkillList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, std::vector<int>>& mapSkillRewardList_ )
{
    mapSkillRewardList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<int> vecInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadVectorIntData( kLuaMng_, std::string("RewardSkillList"), vecInfo ), return false );

        if( mapSkillRewardList_.insert( std::make_pair( (char)nChar, vecInfo ) ).second == false ) {
            START_LOG( cerr, L"스킬 보상 정보 중복 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::LoadRewardUnLockSkillList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, std::vector<int>>& mapSkillGroupRewardList_ )
{
    mapSkillGroupRewardList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<int> vecInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadVectorIntData( kLuaMng_, std::string("UnlockGroupList"), vecInfo ), return false );

        if( mapSkillGroupRewardList_.insert( std::make_pair( (char)nChar, vecInfo ) ).second == false ) {
            START_LOG( cerr, L"스킬 그룹 보상 정보 중복 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::LoadClearMissionList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, std::vector<int>>& mapMissionClearList_ )
{
    mapMissionClearList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<int> vecInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadVectorIntData( kLuaMng_, std::string("ClearMissionList"), vecInfo ), return false );

        if( mapMissionClearList_.insert( std::make_pair( (char)nChar, vecInfo ) ).second == false ) {
            START_LOG( cerr, L"미션 클리어 정보 중복 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KJumpingChar::GetJumpingCharEnableList( OUT std::set<char>& setJumpingCharEnableList_ )
{
    KLocker lock( m_csJumpingChar );
    setJumpingCharEnableList_ = m_setJumpingCharEnableList;
}

bool KJumpingChar::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csJumpingChar );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KJumpingChar::GetJumpingCharDefaultReward( OUT VEC_REWARD& vecReward_ )
{
    KLocker lock( m_csJumpingChar );
    if( m_vecDefaultRewardList.empty() ) {
        return false;
    }

    vecReward_ = m_vecDefaultRewardList;
    return true;
}

bool KJumpingChar::GetJumpingCharReward( IN const char& cCharType_, OUT VEC_REWARD& vecReward_ )
{
    vecReward_.clear();
    std::map<char, VEC_REWARD>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapJumpingRewardItem.find( cCharType_ );
    if( mit == m_mapJumpingRewardItem.end() ) {
        return false;
    }
    vecReward_ = mit->second;
    return true;
}

bool KJumpingChar::GetJumpingCharMissionClearList( IN const char& cCharType_, OUT std::vector<int>& vecMissionInfo_ )
{
    vecMissionInfo_.clear();
    std::map<char, std::vector<int>>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapMissionClearList.find( cCharType_ );
    if( mit == m_mapMissionClearList.end() ) {
        return false;
    }
    vecMissionInfo_ = mit->second;
    return true;
}

bool KJumpingChar::GetJumpingCharDungeonClearList( IN const char& cCharType_, OUT std::vector<int>& vecDungeonInfo_ )
{
    vecDungeonInfo_.clear();
    std::map<char, std::vector<int>>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapDungeonClearList.find( cCharType_ );
    if( mit == m_mapDungeonClearList.end() ) {
        return false;
    }
    vecDungeonInfo_ = mit->second;
    return true;
}

bool KJumpingChar::GetJumpingCharSkillList( IN const char& cCharType_, OUT std::vector<int>& vecSkillInfo_ )
{
    vecSkillInfo_.clear();
    std::map<char, std::vector<int>>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapSkillRewardList.find( cCharType_ );
    if( mit == m_mapSkillRewardList.end() ) {
        return false;
    }
    vecSkillInfo_ = mit->second;
    return true;
}

bool KJumpingChar::GetJumpingCharSkillGroupList( IN const char& cCharType_, OUT std::vector<int>& vecSkillGroupInfo_ )
{
    vecSkillGroupInfo_.clear();
    std::map<char, std::vector<int>>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapSkillGroupRewardList.find( cCharType_ );
    if( mit == m_mapSkillGroupRewardList.end() ) {
        return false;
    }
    vecSkillGroupInfo_ = mit->second;
    return true;
}

bool KJumpingChar::LoadSetCharData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<char>& setInfo_ )
{
    setInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setInfo_.insert( (char)nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KJumpingChar::LoadJumpingCharPromotion( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, char>& mapJumpingCharPromotion_ )
{
    mapJumpingCharPromotion_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        int nPromotion = -1;
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "CharPromotion", nPromotion ) == S_OK, return false );

        if( mapJumpingCharPromotion_.insert( std::make_pair( (char)nChar, (char)nPromotion ) ).second == false ) {
            START_LOG( cerr, L"캐릭터 전직정보 중복 데이터 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::GetJumpingCharPromtion( IN const char& cCharType_, OUT char& cPromotion_ )
{
    std::map<char,char>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapJumpingCharPromotion.find( cCharType_ );
    if( mit == m_mapJumpingCharPromotion.end() ) {
        return false;
    }
    cPromotion_ = mit->second;
    return true;
}

bool KJumpingChar::LoadRegisterMissionList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char, std::vector<int>>& mapMissionRegisterList_ )
{
    mapMissionRegisterList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<int> vecInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadVectorIntData( kLuaMng_, std::string("RegisterMissionList"), vecInfo ), return false );

        if( mapMissionRegisterList_.insert( std::make_pair( (char)nChar, vecInfo ) ).second == false ) {
            START_LOG( cerr, L"등록미션 정보 중복 CharType :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJumpingChar::GetJumpingCharRegisterMission( IN const char& cCharType_, OUT std::vector<int>& vecMission_ )
{
    vecMission_.clear();
    std::map<char, std::vector<int>>::iterator mit;

    KLocker lock( m_csJumpingChar );
    mit = m_mapMissionRegisterList.find( cCharType_ );
    if( mit == m_mapMissionRegisterList.end() ) {
        return false;
    }
    vecMission_ = mit->second;
    return true;
}