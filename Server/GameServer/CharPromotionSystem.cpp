#include "CharPromotionSystem.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "UserPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCharPromotionSystem );
ImplOstmOperatorW2A( KCharPromotionSystem );
NiImplementRootRTTI( KCharPromotionSystem );

KCharPromotionSystem::KCharPromotionSystem(void)
{
    // init
    m_mapCharPromotionInfo.clear();
}

KCharPromotionSystem::~KCharPromotionSystem(void)
{
}

ImplToStringW( KCharPromotionSystem )
{
    KLocker lock( m_csCharPromotion );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapCharPromotionInfo.size() );
}

bool KCharPromotionSystem::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharPromotionSystem::_LoadScript( OUT std::string& strScript_ )
{
    std::map< DWORD, KCharPromotion > mapCharPromotionInfo;
    mapCharPromotionInfo.clear();
    KLuaManager kLuaMng;

    const std::string strScriptName = "InitCharPromotion.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "CharUpdatePromotion" ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        GCITEMID dwItemID;
        int nEnableCharPromotion;
        std::map< char, VEC_ITEMS > mapRewards;
        std::map< char, DWORD > mapCharMinLevel;
        std::map< char, DWORD > mapMissionList;
        KCharPromotion kCharPromotion;

        // 전직 마법서 ItemID 정보 읽어오기.
        _JIF( kLuaMng.GetValue( "ItemID", dwItemID ) == S_OK, return false );
        // 최대 전직 가능 정보 읽어오기.
        _JIF( kLuaMng.GetValue( "EnablePromotion", nEnableCharPromotion ) == S_OK, return false );
        // 각 캐릭터 최소 전직 레벨 정보 읽어오기.
        _JIF( LoadMinLevelList( kLuaMng, std::string("CharMinLevel"), mapCharMinLevel ), return false );
        // 미션 등록 정보 읽어오기.
        _JIF( LoadMissionList( kLuaMng, std::string("MissionIDList"), mapMissionList ), return false );
        // 캐릭터 보상 정보 읽어오기.
        _JIF( LoadRewardInfo( kLuaMng, std::string("CharReward"), mapRewards ), return false );

        kCharPromotion.m_dwItemID = dwItemID;
        kCharPromotion.m_cEnablePromotion = (char)nEnableCharPromotion;
        kCharPromotion.m_mapCharMinLevel.swap( mapCharMinLevel );
        kCharPromotion.m_mapMissionIDList.swap( mapMissionList );
        kCharPromotion.m_mapCharReward.swap( mapRewards );

        if( !mapCharPromotionInfo.insert( std::make_pair( dwItemID, kCharPromotion ) ).second )
        {
            START_LOG( cerr, L" 중복된 아이템 정보가 있음. ItemID : " << dwItemID ) << END_LOG;
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 전직 마법서 시스템 스크립트 읽기 성공. " )
        << BUILD_LOG( mapCharPromotionInfo.size() ) << END_LOG;

    KLocker lock( m_csCharPromotion );
    m_mapCharPromotionInfo.swap( mapCharPromotionInfo );

    return true;
}

bool KCharPromotionSystem::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< char, std::vector<KDropItemInfo > >& mapRewards_ )
{
    mapRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        int nCharType;
        std::vector<KDropItemInfo > vecItems;
        vecItems.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecItems ), return false );

        if( !mapRewards_.insert( std::make_pair( (char)nCharType, vecItems ) ).second )
        {
            START_LOG( cerr, L" 중복된 CharType 정보가 있음. CharType : " << nCharType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharPromotionSystem::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KDropItemInfo>& vecRewards_ )
{
    vecRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewards_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharPromotionSystem::LoadMissionList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< char, DWORD >& mapMissionIDList_ )
{
    mapMissionIDList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i )
    {
        int nCharType;
        DWORD dwMissionID;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MissionID", dwMissionID ) == S_OK, return false );

        if( !mapMissionIDList_.insert( std::make_pair( (char)nCharType, dwMissionID ) ).second )
        {
            START_LOG( cerr, L" 중복된 CharType 정보가 있음. CharType : " << nCharType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharPromotionSystem::LoadMinLevelList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< char, DWORD >& mapCharMinLevel_ )
{
    mapCharMinLevel_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i )
    {
        int nCharType;
        DWORD dwMinLevel;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MinLevel", dwMinLevel ) == S_OK, return false );

        if( !mapCharMinLevel_.insert( std::make_pair( (char)nCharType, dwMinLevel ) ).second )
        {
            START_LOG( cerr, L" 중복된 CharType 정보가 있음. CharType : " << nCharType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharPromotionSystem::CheckEnableItem( IN GCITEMID dwItemID_, IN char cPromotion_, OUT int& nOK_ )
{
    // ok값 정의
    // 0 : 사용가능 아이템.
    // 1 : 사용 불가능 아이템.
    // 2 : 사용가능한 아이템이긴 한데 클라이언트 요청한 전직은 불가.
    std::map<DWORD, KCharPromotion>::iterator mit;

    KLocker lock( m_csCharPromotion );
    mit = m_mapCharPromotionInfo.find( dwItemID_ );
    // 사용 가능 아이템인지 체크.
    if( mit == m_mapCharPromotionInfo.end() ) {
        nOK_ = 1;
        return false;
    }
    // 해당 전직 가능한 아이템인지 체크.
    if( (int)mit->second.m_cEnablePromotion != (int)cPromotion_ ) {
        nOK_ = 2;
        return false;
    }
    nOK_ = 0;
    return true;
}

bool KCharPromotionSystem::GetMissionID( IN GCITEMID dwItemID_, IN char cCharType_, OUT DWORD& dwMissionID_ )
{
    std::map<DWORD, KCharPromotion>::iterator mit;
    std::map< char, DWORD >::iterator mitMission;
    KLocker lock( m_csCharPromotion );
    mit = m_mapCharPromotionInfo.find( dwItemID_ );
    // 사용 가능 아이템인지 체크.
    if( mit == m_mapCharPromotionInfo.end() ) return false;

    // 해당 CharType의 미션이 등록되어 있는지 체크.
    mitMission = mit->second.m_mapMissionIDList.find( cCharType_ );
    if( mitMission == mit->second.m_mapMissionIDList.end() ) return false;

    dwMissionID_ = mitMission->second;
    return true;
}

bool KCharPromotionSystem::GetPromotionReward( IN GCITEMID dwItemID_, IN char cCharType_, OUT std::vector<KDropItemInfo>& kReward_ )
{
    std::map<DWORD, KCharPromotion>::iterator mit;
    std::map< char, std::vector<KDropItemInfo> >::iterator mitReward;
    KLocker lock( m_csCharPromotion );
    mit = m_mapCharPromotionInfo.find( dwItemID_ );
    // 사용 가능 아이템인지 체크.
    if( mit == m_mapCharPromotionInfo.end() ) return false;

    // 해당 CharType의 보상아이템이 있는지 체크.
    mitReward = mit->second.m_mapCharReward.find( cCharType_ );
    if( mitReward == mit->second.m_mapCharReward.end() ) return false;

    kReward_ = mitReward->second;
    return true;
}

bool KCharPromotionSystem::CheckCharLevel( IN GCITEMID dwItemID_, IN char cCharType_, IN DWORD dwLevel_ )
{
    std::map<DWORD, KCharPromotion>::iterator mit;
    std::map< char, DWORD >::iterator mitMinLevel;
    KLocker lock( m_csCharPromotion );
    mit = m_mapCharPromotionInfo.find( dwItemID_ );
    // 사용 가능 아이템인지 체크.
    if( mit == m_mapCharPromotionInfo.end() ) return false;

    // 해당 CharType의 미션이 등록되어 있는지 체크.
    mitMinLevel = mit->second.m_mapCharMinLevel.find( cCharType_ );
    if( mitMinLevel == mit->second.m_mapCharMinLevel.end() ) return false;

    if( dwLevel_ >= mitMinLevel->second ) {
        return true;
    } else {
        return false;
    }
}

void KCharPromotionSystem::GetItemCharPromotionMinLevel( OUT std::map< DWORD, std::map<char, DWORD> >& mapItemPromotionLevel_ )
{
    mapItemPromotionLevel_.clear();

    std::map<DWORD, KCharPromotion>::iterator mit;
    KLocker lock( m_csCharPromotion );
    for( mit = m_mapCharPromotionInfo.begin(); mit != m_mapCharPromotionInfo.end(); ++mit ) {
        if( !mapItemPromotionLevel_.insert( std::make_pair( mit->first, mit->second.m_mapCharMinLevel ) ).second ) {
            START_LOG( cerr, L" 중복된 Item 정보가 있음. ItemID : " << mit->first ) << END_LOG;
        }
    }
}