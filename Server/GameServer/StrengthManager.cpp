#include "CnConnector.h" // winsock
#include "StrengthManager.h"
#include "UserPacket.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "SignBoard.h"
#include "LoudMessage.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KStrengthManager );
ImplOstmOperatorW2A( KStrengthManager );
NiImplementRootRTTI( KStrengthManager );

KStrengthManager::KStrengthManager(void)
:m_dwMaxStrengthLevel(0)
,m_bNoticeEnable(false)
,m_dwNoticeMinLevel(0)
,m_dwTickGap(10000)
,m_bMigrationEnable(false)
,m_StrengthItemID(0)
{
    m_mapStrengthInfo.clear();
    m_mapUpgradeAssistInfo.clear();
    m_mapSpecificLevelInfo.clear();
    m_mapRandomLevelInfo.clear();
    m_mapBreakUpInfo.clear();
    m_mapComposeItem.clear();
    m_mapComposeType.clear();
    m_mapStrengthGradeSkill.clear();
    m_mapComposeResultRatio.clear();
    m_mapStrengthProtectionInfo.clear();
    m_mapStrengthMaterialInfo.clear();
    m_mapStrengthAttributeInfo.clear();
    m_setStrengthShopItemList.clear();
    m_vecNoticeInfo.clear();
    m_vecCompressedNotice.clear();
    m_dwLastTick = ::GetTickCount();
    m_setNonStrengthItem.clear();
    m_mapUpgradeProtectionAssistInfo.clear();
    m_mapStrengthGPCost.clear();
}

KStrengthManager::~KStrengthManager(void)
{
}

ImplToStringW( KStrengthManager )
{
    KLocker lock( m_csStrength );
    return START_TOSTRINGW
        << TOSTRINGW( m_dwMaxStrengthLevel )
        << TOSTRINGWb( m_bNoticeEnable )
        << TOSTRINGWb( m_bMigrationEnable )
        << TOSTRINGW( m_dwNoticeMinLevel )
        << TOSTRINGW( m_mapStrengthInfo.size() )
        << TOSTRINGW( m_mapUpgradeAssistInfo.size() )
        << TOSTRINGW( m_mapSpecificLevelInfo.size() )
        << TOSTRINGW( m_mapStrengthProtectionInfo.size() )
        << TOSTRINGW( m_mapRandomLevelInfo.size() )
        << TOSTRINGW( m_mapBreakUpInfo.size() )
        << TOSTRINGW( m_mapComposeItem.size() )
        << TOSTRINGW( m_mapComposeType.size() )
        << TOSTRINGW( m_mapStrengthGradeSkill.size() )
        << TOSTRINGW( m_mapComposeResultRatio.size() )
        << TOSTRINGW( m_mapStrengthAttributeInfo.size() )
        << TOSTRINGW( m_setStrengthShopItemList.size() )
        << TOSTRINGW( m_setNonStrengthItem.size() )
        << TOSTRINGW( m_mapUpgradeProtectionAssistInfo.size() )
        << TOSTRINGW( m_mapStrengthGPCost.size() );
}

bool KStrengthManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KStrengthManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    GCITEMID                                    StrengthItemID; // 강화석 ItemID
    DWORD                                       dwMaxStrengthLevel; // 최대 강화 레벨.
    bool                                        bNoticeEnable;          // 강화 공지 출력 여부.
    DWORD                                       dwNoticeMinLevel;       // 공지 최소 레벨.
    DWORD                                       dwTickGap;
    bool                                        bMigrationEnable; // 강화 마이그레이션 여부.

    std::map<std::pair<DWORD,DWORD>, Kairo>     mapStrengthInfo; // 아이템 레벨별 강화 정보.
    std::map<DWORD, int>                        mapStrengthGradeSkill; // 강화석 등급에 따른 스킬 갯수.
    std::map<GCITEMID, float>                   mapUpgradeAssistInfo; // 강화 확률 상승 아이템 정보.
    std::map<GCITEMID, std::pair<int,int>>      mapStrengthProtectionInfo; // 강화 보호 아이템 정보.
    std::map<GCITEMID, DWORD>                   mapSpecificLevelInfo; // 특정 강화 레벨 변경 아이템 정보.
    std::map<GCITEMID, Kairo>                   mapRandomLevelInfo; // 랜덤 강화 레벨 아이템 정보.
    std::map<GCITEMID, KBreakUpInfo>            mapBreakUpInfo; // 아이템 해체 정보.
    std::map<std::pair<DWORD,GCITEMID>, Kairo>  mapComposeItem; // 아이템 합성 정보.
    std::map<std::pair<DWORD,DWORD>, DWORD>  mapComposeType; // 아이템 합성 Type 정보.
    std::map< DWORD, std::pair<int, float> >    mapComposeResultRatio; // 아이템 합성 배율.
    std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >> mapStrengthMaterialInfo;
    std::map< std::pair<DWORD, DWORD>, KStrengthAttributeInfo > mapStrengthAttributeInfo; // 강화 속성 정보.
    std::set<GCITEMID>                          setStrengthShopItemList; // 상점 판매 아이템 리스트.
    std::set<GCITEMID>                          setNonStrengthItem; // 강화석 장착 못하는 아이템 리스트.
    std::map<GCITEMID, KStrengthUpgradeProtectionAssist> mapUpgradeProtectionAssistInfo; // 강화 확률 상승 보호 복합 아이템 정보.
    std::map<DWORD, std::pair<int, int>> mapStrengthGPCost;

    mapStrengthGPCost.clear();

    std::string strScriptName;
    if ( "" == strScript_ ) {
        strScriptName = "InitStrength.lua";
    }
    else {
        strScriptName = strScript_;
    }

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    // 강화 마이그레이션 여부 읽어오기.
    _JIF( kLuaMng.GetValue( "MigrationEnable", bMigrationEnable ) == S_OK, return false );
    // 공지 TickGap 정보 읽어오기.
    _JIF( kLuaMng.GetValue( "NoticeTimeGap", dwTickGap ) == S_OK, return false );
    // 강화석 등급별 스킬 갯수 정보 읽어오기.
    _JIF( LoadStrengthGradeSkillInfo( kLuaMng, std::string("StrengthGradeSkillInfo"), mapStrengthGradeSkill ), return false );
    // 레벨별 강화 설정 정보 읽어오기.
    _JIF( LoadStrengthInfo( kLuaMng, std::string("InitStrength"), StrengthItemID, dwMaxStrengthLevel, mapStrengthInfo ), return false );
    // 강화 상승 정보 읽어오기.
    _JIF( LoadUpgradeAssist( kLuaMng, std::string("StrengthUpgradeAssist"), mapUpgradeAssistInfo ), return false );
    // 강화 보호 주문서 정보 읽어오기.
    _JIF( LoadStrengthProtection( kLuaMng, std::string("StrengthProtection"), mapStrengthProtectionInfo ), return false );
    // 특정레벨 강화 정보 읽어오기.
    _JIF( LoadSpecificLevel( kLuaMng, std::string("SpecificLevelStrength"), mapSpecificLevelInfo ), return false );
    // 랜덤 레벨 강화 부적 정보 읽어오기.
    _JIF( LoadRandomLevelStrength( kLuaMng, std::string("RandomLevelStrength"), mapRandomLevelInfo ), return false );
    // 강화 해체 부적 정보 읽어오기.
    _JIF( LoadBreakUpItem( kLuaMng, std::string("BreakUpItem"), mapBreakUpInfo ), return false );
    // 아이템 합성 Type 정보 읽어오기.
    _JIF( LoadComposeType( kLuaMng, std::string("ComposeTypeDefine"), mapComposeType ), return false );
    // 아이템 합성 Ratio 정보 읽어오기.
    _JIF( LoadComposeResultRatio( kLuaMng, std::string("ComposeResultRatio"), mapComposeResultRatio ), return false );
    // 아이템 합성 정보 읽어오기.
    _JIF( LoadComposeInfo( kLuaMng, std::string("ComposeItem"), mapComposeItem ), return false );
    // 강화석 아이템 속성 Ratio 정보 읽어오기.
    _JIF( LoadStrengthAttributeRatio( kLuaMng, std::string("StrengthAttributeTable"), mapStrengthAttributeInfo ), return false );
    // 강화 상점 판매 아이템 리스트 정보 읽어오기.
    _JIF( LoadShopItemList( kLuaMng, std::string("StrengthShopItemList"), setStrengthShopItemList ), return false );
    // 강화 공지 출력 정보 읽어오기.
    _JIF( LoadNoticeInfo( kLuaMng, std::string("StrengthNotice"), bNoticeEnable, dwNoticeMinLevel ), return false );
    // 강화석 장착 못하는 아이템 리스트 읽어오기.
    _JIF( LoadSetItemList( kLuaMng, std::string("NonStrengthItemList"), setNonStrengthItem ), return false );
    // 강화석 ( 상승확률 + 보호 ) 주문서 정보 읽어오기.
    _JIF( LoadUpgradeProtectionAssist( kLuaMng, std::string("StrengthUpgradeProtectionAssist"), mapUpgradeProtectionAssistInfo ), return false );
    // 강화에 소모 되는 GP정보 읽어오기
    _JIF( LoadStrengthGPCost( kLuaMng, std::string("StrengthGPCost"), mapStrengthGPCost ), return false );
    {
        KLocker lock( m_csStrength );
        m_dwTickGap = dwTickGap;
        m_StrengthItemID = StrengthItemID;
        m_dwMaxStrengthLevel = dwMaxStrengthLevel;
        m_bNoticeEnable = bNoticeEnable;
        m_bMigrationEnable = bMigrationEnable;
        m_dwNoticeMinLevel = dwNoticeMinLevel;
        m_mapStrengthGradeSkill.swap( mapStrengthGradeSkill );
        m_mapStrengthInfo.swap( mapStrengthInfo );
        m_mapUpgradeAssistInfo.swap( mapUpgradeAssistInfo );
        m_mapStrengthProtectionInfo.swap( mapStrengthProtectionInfo );
        m_mapSpecificLevelInfo.swap( mapSpecificLevelInfo );
        m_mapRandomLevelInfo.swap( mapRandomLevelInfo );
        m_mapBreakUpInfo.swap( mapBreakUpInfo );
        m_mapComposeType.swap( mapComposeType );
        m_mapComposeResultRatio.swap( mapComposeResultRatio );
        m_mapComposeItem.swap( mapComposeItem );
        m_mapStrengthAttributeInfo.swap( mapStrengthAttributeInfo );
        m_setStrengthShopItemList.swap( setStrengthShopItemList );
        m_setNonStrengthItem.swap( setNonStrengthItem );
        m_mapUpgradeProtectionAssistInfo.swap( mapUpgradeProtectionAssistInfo );
        m_mapStrengthGPCost.swap( mapStrengthGPCost );
    }

    START_LOG( cerr, L" 강화 정보 읽기 성공." )
        << BUILD_LOG( m_dwMaxStrengthLevel )
        << BUILD_LOG( m_bNoticeEnable )
        << BUILD_LOG( m_bMigrationEnable )
        << BUILD_LOG( m_dwNoticeMinLevel )
        << BUILD_LOG( m_mapStrengthInfo.size() )
        << BUILD_LOG( m_mapUpgradeAssistInfo.size() )
        << BUILD_LOG( m_mapSpecificLevelInfo.size() )
        << BUILD_LOG( m_mapRandomLevelInfo.size() )
        << BUILD_LOG( m_mapBreakUpInfo.size() )
        << BUILD_LOG( m_mapComposeItem.size() )
        << BUILD_LOG( m_mapComposeType.size() )
        << BUILD_LOG( m_mapStrengthProtectionInfo.size() )
        << BUILD_LOG( m_mapComposeResultRatio.size() )
        << BUILD_LOG( m_mapComposeItem.size() )
        << BUILD_LOG( m_mapStrengthAttributeInfo.size() )
        << BUILD_LOG( m_setStrengthShopItemList.size() )
        << BUILD_LOG( m_setNonStrengthItem.size() )
        << BUILD_LOG( m_mapUpgradeProtectionAssistInfo.size() )
        << BUILD_LOG( m_mapStrengthGPCost.size() )
        << END_LOG;

    // 클라이언트 전달 강화 주문서 재료 리스트 만들기.
    ModifyStrengthMaterial( mapStrengthMaterialInfo );
    {
        KLocker lock( m_csStrength );
        m_mapStrengthMaterialInfo.swap( mapStrengthMaterialInfo );
    }
    return true;
}

bool KStrengthManager::LoadStrengthGradeSkillInfo( IN KLuaManager& kLuaMng_, 
                                                        IN std::string& strTable_, 
                                                            OUT std::map<DWORD, int>& mapStrengthGradeSkill_ )
{
    mapStrengthGradeSkill_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwGrade;
        int nSkillCount;

        _JIF( kLuaMng_.GetValue( 1, dwGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSkillCount ) == S_OK, return false );

        if ( !mapStrengthGradeSkill_.insert( std::make_pair( dwGrade, nSkillCount ) ).second ) {
            START_LOG( cerr, L"중복된 등급 정보 있음. Grade  : " << dwGrade ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadStrengthInfo( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT GCITEMID& StrengthItemID_, 
                                                        OUT DWORD& dwMaxStrengthLevel_, 
                                                            OUT std::map<std::pair<DWORD,DWORD>, Kairo>& mapStrengthInfo_ )
{
    mapStrengthInfo_.clear();
    StrengthItemID_ = 0L;
    dwMaxStrengthLevel_ = 0;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "StrengthItem", StrengthItemID_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxStrengthLevel", dwMaxStrengthLevel_ ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::pair<DWORD,DWORD> prData;
        Kairo kKairo;

        _JIF( kLuaMng_.GetValue( "StrengthType", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "StrengthLevel", prData.second ) == S_OK, return false );
        _JIF( LoadRatio( kLuaMng_, std::string("StrengthRatio"), kKairo ), return false );

        if ( !mapStrengthInfo_.insert( std::make_pair( prData, kKairo ) ).second ) {
            START_LOG( cerr, L"중복된 강화정보 있음. Grade : " << prData.first << ", Level : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        DWORD dwType = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostType( dwType, fProb ), return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::LoadLevelRange( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT int& nBegin_, OUT int& nEnd_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, nBegin_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nEnd_ ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::LoadUpgradeAssist( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT std::map<GCITEMID, float>& mapUpgradeAssistInfo_ )
{
    mapUpgradeAssistInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID;
        float fRatio;

        _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );

        if ( !mapUpgradeAssistInfo_.insert( std::make_pair( ItemID, fRatio ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadStrengthProtection( IN KLuaManager& kLuaMng_, 
                                                    IN std::string& strTable_,
                                                        OUT std::map<GCITEMID, std::pair<int,int>>& mapStrengthProtectionInfo_ )
{
    mapStrengthProtectionInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID = 0;
        std::pair<int,int> prData;

        _JIF( kLuaMng_.GetValue( "ItemID", ItemID ) == S_OK, return false );
        _JIF( LoadLevelRange( kLuaMng_, std::string("LevelRange"), prData.first, prData.second ), return false );

        if ( !mapStrengthProtectionInfo_.insert( std::make_pair( ItemID, prData ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadSpecificLevel( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT std::map<GCITEMID, DWORD>& mapSpecificLevelInfo_ )
{
    mapSpecificLevelInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID;
        DWORD dwLevel;

        _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwLevel ) == S_OK, return false );

        if ( !mapSpecificLevelInfo_.insert( std::make_pair( ItemID, dwLevel ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadRandomLevelStrength( IN KLuaManager& kLuaMng_, 
                                                    IN std::string& strTable_, 
                                                        OUT std::map<GCITEMID, Kairo>& mapRandomLevelInfo_ )
{
    mapRandomLevelInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID;
        Kairo kKairo;

        _JIF( kLuaMng_.GetValue( "ItemID", ItemID ) == S_OK, return false );
        _JIF( LoadRatio( kLuaMng_, std::string("LevelRatio"), kKairo ), return false );

        if ( !mapRandomLevelInfo_.insert( std::make_pair( ItemID, kKairo ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadBreakUpItem( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                    OUT std::map<GCITEMID, KBreakUpInfo>& mapBreakUpInfo_ )
{
    mapBreakUpInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KBreakUpInfo kBreakUpInfo;

        _JIF( kLuaMng_.GetValue( "ItemID", kBreakUpInfo.m_ItemID ) == S_OK, return false );
        _JIF( LoadRatio( kLuaMng_, std::string("BreakUpRatio"), kBreakUpInfo.m_kBreakUpRatio ), return false );

        if ( !mapBreakUpInfo_.insert( std::make_pair( kBreakUpInfo.m_ItemID, kBreakUpInfo ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << kBreakUpInfo.m_ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadComposeType( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT std::map<std::pair<DWORD,DWORD>, DWORD>& mapComposeType_ )
{
    mapComposeType_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::pair<DWORD,DWORD> prData;
        DWORD dwType;

        _JIF( LoadItemType( kLuaMng_, std::string("ItemCompose"), prData.first, prData.second ), return false );
        _JIF( kLuaMng_.GetValue( "ComposeType", dwType ) == S_OK, return false );

        if ( !mapComposeType_.insert( std::make_pair( prData, dwType ) ).second ) {
            START_LOG( cerr, L"중복된 합성 Type 정보 있음. ItemA : " << prData.first << ", ItmeB : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadItemType( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT DWORD& dwItemA_, OUT DWORD& dwItemB_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, dwItemA_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, dwItemB_ ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::LoadComposeInfo( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT std::map<std::pair<DWORD,GCITEMID>, Kairo>& mapComposeItem_ )
{
    mapComposeItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::pair<DWORD,GCITEMID> prData;
        Kairo kKairo;

        _JIF( kLuaMng_.GetValue( "ComposeType", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ComposeItemID", prData.second ) == S_OK, return false );
        _JIF( LoadRatio( kLuaMng_, std::string("ComposeRatio"), kKairo ), return false );

        if ( !mapComposeItem_.insert( std::make_pair( prData, kKairo ) ).second ) {
            START_LOG( cerr, L"중복된 합성정보 있음. Grade : " << prData.first << ", Level : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::GetUpgradeAssistRatio( IN const GCITEMID dwItemID_, OUT float& fAddRatio_ )
{
    std::map<GCITEMID, float>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapUpgradeAssistInfo.find( dwItemID_ );
    if( mit == m_mapUpgradeAssistInfo.end() ) {
        return false;
    }
    fAddRatio_ += mit->second;
    return true;
}

bool KStrengthManager::StrengthAction( IN const DWORD dwStrengthType_, IN const int nStrengthLevel_, IN const float fAddRatio_, IN const DWORD dwAssistType_, OUT DWORD& dwResult_ )
{
    std::map<std::pair<DWORD,DWORD>, Kairo>::iterator mit;
    std::pair<DWORD,DWORD> prData;
    prData.first = dwStrengthType_;
    prData.second = nStrengthLevel_;
    dwResult_ = -1;

    KLocker lock( m_csStrength );

    if( static_cast<int>( m_dwMaxStrengthLevel ) <= nStrengthLevel_ ) { // 최대 강화석 레벨일 경우 제자리 처리.
        dwResult_ = KStrengthManager::SR_NONE;
    }

    mit = m_mapStrengthInfo.find( prData );
    if( mit == m_mapStrengthInfo.end() ) {
        return false;
    }

    if( dwAssistType_ == KStrengthManager::AT_SPECIFIC ) {
        dwResult_ = KStrengthManager::SR_SPECIFIC;
        return true;
    }

    Kairo kKairo = mit->second;

    // 강화률 보너스 적용
    if ( fAddRatio_ > 0.f ) {
        float fPrevSuccessRatio = kKairo.GetRatio( KStrengthManager::SR_SUCCESS - 1 );
        float fRemainderRatio = 100.f - fPrevSuccessRatio;

        // 실제 보너스확률 = 이전 성공 확률 * (보너스확률 / 100.f)
        float fCurrentAddRatio = std::min<float>( fPrevSuccessRatio * (fAddRatio_ / 100.f), fRemainderRatio );
        if ( fCurrentAddRatio > 0.f ) {
            float fDecreaseRatio = fCurrentAddRatio;
            for ( int i = 0 ; i < static_cast<int>(kKairo.GetCaseNum()) ; ++i ) {
                // 성공제외
                if ( KStrengthManager::SR_SUCCESS - 1 == i ) {
                    continue;
                }

                float fCaseRatio = kKairo.GetRatio( i );
                if ( fCaseRatio <= 0.f ) {
                    continue;
                }

                // 실제 보너스확률만큼 성공이 아닌 확률들을 낮춘다.
                if ( false == kKairo.DecreaseCaseRatio( i, std::min<float>(fDecreaseRatio,fCaseRatio) ) ) {
                    continue;
                }

                fDecreaseRatio -= fCaseRatio;
                if ( fDecreaseRatio > 0.f ) {
                    continue;
                }

                break;
            }

            // 성공 확률 적용 (강화 성공률 *= 1.f + 강화 보너스)
            kKairo.IncreaseCaseRatio( KStrengthManager::SR_SUCCESS - 1, fCurrentAddRatio );
        }
    }

    bool bSuccess = kKairo.Do( dwResult_ );

    if( !bSuccess ) {
        return false;
    }
    if( dwResult_ == 0 ) {
        dwResult_ = KStrengthManager::SR_NONE;
    }

    if( dwAssistType_ == KStrengthManager::AT_NOBREAK || dwAssistType_ == KStrengthManager::AT_UPGRADE_NOBREAK ) {
        if( dwResult_ != KStrengthManager::SR_SUCCESS ) {
            dwResult_ = KStrengthManager::SR_NONE;
        }
    }

    return true;
}

bool KStrengthManager::UpdateSpecificLevel( IN const GCITEMID dwItemID_, OUT DWORD& dwLevel_ )
{
    std::map<GCITEMID, DWORD>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapSpecificLevelInfo.find( dwItemID_ );
    if( mit == m_mapSpecificLevelInfo.end() ) {
        return false;
    }

    dwLevel_ = mit->second;
    return true;
}

bool KStrengthManager::GetStrengthSkillCount( IN const DWORD dwGrade_, OUT int& nCount_ )
{
    std::map<DWORD, int>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapStrengthGradeSkill.find( dwGrade_ );
    if( mit == m_mapStrengthGradeSkill.end() ) {
        return false;
    }

    nCount_ = mit->second;
    return true;
}

bool KStrengthManager::CheckBreakUpMaterial( IN const GCITEMID ItemID_ )
{
    std::map<GCITEMID, KBreakUpInfo>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapBreakUpInfo.find( ItemID_ );
    if( mit == m_mapBreakUpInfo.end() ) {
        return false;
    }
    return true;
}

bool KStrengthManager::BreakUpAction( IN const GCITEMID ItemID_, OUT DWORD& dwResult_ )
{
    std::map<GCITEMID, KBreakUpInfo>::iterator mit;
    dwResult_ = -1;

    KLocker lock( m_csStrength );
    mit = m_mapBreakUpInfo.find( ItemID_ );
    if( mit == m_mapBreakUpInfo.end() ) {
        return false;
    }

    bool bSuccess = mit->second.m_kBreakUpRatio.Do( dwResult_ );
    if( !bSuccess ) {
        return false;
    }
    return true;
}

bool KStrengthManager::GetComposeGradeType( IN DWORD dwFirstGrade_, IN DWORD dwSecondGrade_, OUT DWORD& dwComposeType_ )
{
    std::map<std::pair<DWORD,DWORD>, DWORD>::iterator mit;
    dwComposeType_ = 0;
    std::pair<DWORD, DWORD> prData;
    prData.first = dwFirstGrade_;
    prData.second = dwSecondGrade_;

    KLocker lock( m_csStrength );
    mit = m_mapComposeType.find( prData );
    if( mit == m_mapComposeType.end() ) {
        return false;
    }
    dwComposeType_ = mit->second;
    return true;
}

bool KStrengthManager::ComposeAction( IN const DWORD dwComposeType_, IN const GCITEMID ItemID_, OUT DWORD& dwResult_ )
{
    std::map<std::pair<DWORD,GCITEMID>, Kairo>::iterator mit;
    dwResult_ = -1;
    std::pair<DWORD, GCITEMID> prData;
    prData.first = dwComposeType_;
    prData.second = ItemID_;

    KLocker lock( m_csStrength );
    mit = m_mapComposeItem.find( prData );
    if( mit == m_mapComposeItem.end() ) {
        return false;
    }

    bool bSuccess = mit->second.Do( dwResult_ );
    if( !bSuccess ) {
        return false;
    }
    return true;
}

bool KStrengthManager::ComposeResult( IN const DWORD dwComposeResultType_, IN int nFirstEnchantLevel_, IN int nSecondEnchantLevel_, OUT int& nResultLevel_, OUT int& nResultGrade_ )
{
/* 초기 기획서의 정보.
    ComposeResultTypeA = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 1.5 )
    ComposeResultTypeB = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 2.0 )
    ComposeResultTypeC = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 3.0 )
    ComposeResultTypeD = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 1.5 )
    ComposeResultTypeE = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 2.0 )
    ComposeResultTypeF = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 3.0 )
*/
    std::map< DWORD, std::pair<int, float> >::iterator mit;
    nResultLevel_ = 0;
    nResultGrade_ = 0;
    int nTotalLevel = nFirstEnchantLevel_ + nSecondEnchantLevel_;
    nTotalLevel = std::max<int>( 1, nTotalLevel );

    KLocker lock( m_csStrength );
    mit = m_mapComposeResultRatio.find( dwComposeResultType_ );
    if( mit == m_mapComposeResultRatio.end() ) {
        return false;
    }
    // 합성 배율 적용.
    nResultGrade_ = mit->second.first;
    nResultLevel_ = static_cast<int>( nTotalLevel / mit->second.second );

    if( static_cast<int>( m_dwMaxStrengthLevel ) <= nResultLevel_ ) { // 최대 강화석 레벨일 경우 최대 레벨값으로 설정.
        nResultLevel_ = m_dwMaxStrengthLevel;
    }

    return true;
}

bool KStrengthManager::LoadComposeResultRatio( IN KLuaManager& kLuaMng_, 
                                                   IN std::string& strTable_, 
                                                       OUT std::map<DWORD, std::pair<int, float>>& mapComposeResultRatio_ )
{
    mapComposeResultRatio_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwType;
        std::pair<int, float> prData;

        _JIF( kLuaMng_.GetValue( "ResultType", dwType ) == S_OK, return false );
        _JIF( LoadResultRatio( kLuaMng_, std::string("ResultRatio"), prData.first, prData.second ), return false );

        if ( !mapComposeResultRatio_.insert( std::make_pair( dwType, prData ) ).second ) {
            START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << dwType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadResultRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT int& nGrade_, OUT float& fRatio_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, nGrade_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, fRatio_ ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::CheckProtectionLevel( IN const GCITEMID ItemID_, IN const int nItemLevel_ )
{
    std::map<GCITEMID, std::pair<int,int>>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapStrengthProtectionInfo.find( ItemID_ );
    if( mit == m_mapStrengthProtectionInfo.end() ) {
        return false;
    }
    if( mit->second.first <= nItemLevel_ && mit->second.second >= nItemLevel_ ) {
        return true;
    } else {
        return false;
    }
}

void KStrengthManager::ModifyStrengthMaterial( OUT std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >>& mapStrengthMaterialInfo_ )
{
    mapStrengthMaterialInfo_.clear();
    std::pair<DWORD,DWORD> prData;
    std::set<GCITEMID> setItemList;
    std::map<GCITEMID, float>::iterator mitUpgrade;
    std::map<GCITEMID, std::pair<int,int>>::iterator mitProtection;
    std::map<GCITEMID, DWORD>::iterator mitSpecific;
    std::map<GCITEMID, KBreakUpInfo>::iterator mitBreakUp;
    std::map<std::pair<DWORD,GCITEMID>, Kairo>::iterator mitCompose;
    std::map<GCITEMID, KStrengthUpgradeProtectionAssist>::iterator mitUpgradeProtection;

    KLocker lock( m_csStrength );

    // 강화 확률 상승 아이템 정보.
    for( mitUpgrade = m_mapUpgradeAssistInfo.begin(); mitUpgrade != m_mapUpgradeAssistInfo.end(); ++mitUpgrade ) {
        setItemList.insert( mitUpgrade->first );
    }
    prData.first = KStrengthManager::ST_STRENGTH;
    prData.second = KStrengthManager::AT_UPGRADE;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
    setItemList.clear();

    // 강화 보호 아이템 정보.
    for( mitProtection = m_mapStrengthProtectionInfo.begin(); mitProtection != m_mapStrengthProtectionInfo.end(); ++mitProtection ) {
        setItemList.insert( mitProtection->first );
    }
    prData.first = KStrengthManager::ST_STRENGTH;
    prData.second = KStrengthManager::AT_NOBREAK;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
    setItemList.clear();
    m_mapProtectionInfoList = m_mapStrengthProtectionInfo;

    // 특정 강화 레벨 변경 아이템 정보.
    for( mitSpecific = m_mapSpecificLevelInfo.begin(); mitSpecific != m_mapSpecificLevelInfo.end(); ++mitSpecific ) {
        setItemList.insert( mitSpecific->first );
    }
    prData.first = KStrengthManager::ST_STRENGTH;
    prData.second = KStrengthManager::AT_SPECIFIC;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
    setItemList.clear();

    // 아이템 해체 정보.
    for( mitBreakUp = m_mapBreakUpInfo.begin(); mitBreakUp != m_mapBreakUpInfo.end(); ++mitBreakUp ) {
        setItemList.insert( mitBreakUp->first );
    }
    prData.first = KStrengthManager::ST_BREAKUP;
    prData.second = KStrengthManager::AT_UPGRADE;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
    setItemList.clear();

    // 아이템 합성 정보.
    for( mitCompose = m_mapComposeItem.begin(); mitCompose != m_mapComposeItem.end(); ++mitCompose ) {
        setItemList.insert( mitCompose->first.second );
    }
    prData.first = KStrengthManager::ST_COMPOSE;
    prData.second = KStrengthManager::AT_UPGRADE;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
    setItemList.clear();

    // ( 강화률 상승 +  보호 ) 아이템 정보.
    for( mitUpgradeProtection = m_mapUpgradeProtectionAssistInfo.begin(); mitUpgradeProtection != m_mapUpgradeProtectionAssistInfo.end(); ++mitUpgradeProtection ) {
        setItemList.insert( mitUpgradeProtection->first );
        if ( !m_mapProtectionInfoList.insert( std::make_pair( mitUpgradeProtection->first, mitUpgradeProtection->second.m_prLevelRange ) ).second ) {
            START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
        }
    }
    prData.first = KStrengthManager::ST_STRENGTH;
    prData.second = KStrengthManager::AT_UPGRADE_NOBREAK;
    if ( !mapStrengthMaterialInfo_.insert( std::make_pair( prData, setItemList ) ).second ) {
        START_LOG( cerr, L"중복된 Type 정보 있음. Type : " << prData.first << ", " << prData.second ) << END_LOG;
    }
}

void KStrengthManager::GetStrengthInfo( OUT KEVENT_STRENGTH_MATERIAL_INFO& kPacket_ )
{
    KLocker lock( m_csStrength );

    kPacket_.m_mapStrengthMaterialInfo = m_mapStrengthMaterialInfo;
    kPacket_.m_setStrengthShopItemList = m_setStrengthShopItemList;
    kPacket_.m_dwStrengthMaxLevel = m_dwMaxStrengthLevel;
    kPacket_.m_setNonStrengthItem = m_setNonStrengthItem;
    kPacket_.m_mapProtectionInfoList = m_mapProtectionInfoList;
    kPacket_.m_mapStrengthGPCost = m_mapStrengthGPCost;
}

bool KStrengthManager::LoadStrengthAttributeRatio( IN KLuaManager& kLuaMng_, 
                                                        IN std::string& strTable_, 
                                                            OUT std::map< std::pair<DWORD, DWORD>, KStrengthAttributeInfo >& mapStrengthAttributeInfo_ )
{
    mapStrengthAttributeInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KStrengthAttributeInfo kStrengthAttributeInfo;

        _JIF( kLuaMng_.GetValue( "StrengthGrade", kStrengthAttributeInfo.m_prAttributeType.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EquipItemType", kStrengthAttributeInfo.m_prAttributeType.second ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "RandomCount", kStrengthAttributeInfo.m_dwRandowCount ) == S_OK, return false );
        _JIF( LoadAttributeRatio( kLuaMng_, std::string("AttributeInfo"), kStrengthAttributeInfo.m_kAttributeRatio ), return false );

        if ( !mapStrengthAttributeInfo_.insert( std::make_pair( kStrengthAttributeInfo.m_prAttributeType, kStrengthAttributeInfo ) ).second ) {
            START_LOG( cerr, L"중복된 강화 속성 정보 있음. Grade : " << kStrengthAttributeInfo.m_prAttributeType.first << ", ItemType : " << kStrengthAttributeInfo.m_prAttributeType.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadAttributeRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        std::pair<int,float> prData;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostType( prData, fProb ), return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::GetStrengthAttributeInfo( IN const DWORD dwStrengthGrade_, IN const DWORD dwEquipItemType_, OUT std::list<std::pair<int,float>>& listAttributeList_ )
{
    std::pair<DWORD, DWORD> prData;
    prData.first = dwStrengthGrade_;
    prData.second = dwEquipItemType_;
    std::map< std::pair<DWORD, DWORD>, KStrengthAttributeInfo >::iterator mit;
    mit = m_mapStrengthAttributeInfo.find( prData );
    if( mit == m_mapStrengthAttributeInfo.end() ) {
        SET_ERROR( ERR_STRENGTH_19 );
        return false;
    }

    if( mit->second.m_dwRandowCount <= 0 ) {
        SET_ERROR( ERR_STRENGTH_19 );
        return false;
    }

    Kairo kKairo = mit->second.m_kAttributeRatio;

    DWORD dwTemp;
    for( dwTemp = 0; dwTemp < mit->second.m_dwRandowCount; ++dwTemp ) {
        std::pair<int,float> prAttribute;
        int nIndex = kKairo.Do( prAttribute );
        if( nIndex == -1 ) {
            SET_ERROR( ERR_STRENGTH_22 );
            return false;
        }
        if( kKairo.RemoveCase( nIndex ) ) {
            listAttributeList_.push_back( prAttribute );
        }
    }

    return true;
}

bool KStrengthManager::LoadShopItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setStrengthShopItemList_ )
{
    setStrengthShopItemList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    GCITEMID itemData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, itemData ) != S_OK ) break;
        setStrengthShopItemList_.insert( itemData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KStrengthManager::LoadNoticeInfo( IN KLuaManager& kLuaMng_, 
                                            IN std::string& strTable_, 
                                                OUT bool& bNoticeEnable_, 
                                                    OUT DWORD& dwNoticeMinLevel_ )
{
    bNoticeEnable_ = false;
    dwNoticeMinLevel_ = 0;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "NoticeEnable", bNoticeEnable_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "NoticeMinStrengthLevel", dwNoticeMinLevel_ ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KStrengthManager::StrengthNotify( IN const DWORD dwStrengthLevel_, IN const std::wstring wstrNickName_, IN const int nLanguageType_ )
{
    KLocker lock( m_csStrength );
    std::wstring wstrData;
    if( SiKLoudMessage()->GetStrengthMsg( wstrNickName_, dwStrengthLevel_, wstrData, nLanguageType_ ) ) {
        m_vecNoticeInfo.push_back( wstrData );
    }
}

void KStrengthManager::Tick()
{
    // 시간체크 스크립트에 설정된 시간대로.
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;
    m_dwLastTick = ::GetTickCount();

    KLocker lock( m_csStrength );
    if( m_vecNoticeInfo.empty() ) return;
    SendNoticeInfoToCenter();
}

void KStrengthManager::SendNoticeInfoToCenter()
{
    std::vector<std::wstring> vecNoticeInfo;
    vecNoticeInfo.clear();
    vecNoticeInfo.swap( m_vecNoticeInfo );
    CompressNoticeInfo( vecNoticeInfo );
    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_NOTICE_BROAD, m_vecCompressedNotice ) );
}

void KStrengthManager::SetNoticeInfo( IN std::vector<KSerBuffer> vecCompressedNotice_ )
{
    std::vector<std::wstring> vecNoticeInfo;
    std::vector<std::wstring>::iterator vit;
    UnCompressNoticeInfo( vecCompressedNotice_, vecNoticeInfo );
    for( vit = vecNoticeInfo.begin(); vit != vecNoticeInfo.end(); ++vit ) {
        KSignBoardData kData;
        kData.m_dwColor         = 0L;
        kData.m_ItemID          = 0L;
        kData.m_dwSenderUID     = 0L;
        kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
        kData.m_strSenderNick.clear();
        kData.m_strMsg = *vit;
        SiKSignBoard()->QueueingAdminData( kData );
    }
}

void KStrengthManager::CompressNoticeInfo( IN const std::vector<std::wstring> vecNoticeInfo_ )
{
    // Lock은 호출 하는곳에 걸려 있다.
    m_vecCompressedNotice.clear();
    std::vector<std::wstring>::const_iterator vit;
    for ( vit = vecNoticeInfo_.begin() ; vit != vecNoticeInfo_.end() ; ++vit ) {
        KSerBuffer kBuff;
        KSerializer ks;
        ks.BeginWriting( &kBuff );
        if( !ks.Put( *vit ) ) continue;
        ks.EndWriting();
        kBuff.Compress();
        m_vecCompressedNotice.push_back( kBuff );
    }
}

void KStrengthManager::UnCompressNoticeInfo( IN std::vector<KSerBuffer> vecCompressedNotice_, OUT std::vector<std::wstring>& vecNoticeInfo_ )
{
    vecNoticeInfo_.clear();
    std::vector<KSerBuffer>::iterator vit;
    for( vit = vecCompressedNotice_.begin() ; vit != vecCompressedNotice_.end() ; ++vit )
    {
        std::wstring wstrData;
        KSerBuffer kBuff = *vit;
        kBuff.UnCompress();
        KSerializer ks;
        ks.BeginReading( &kBuff );
        if ( !ks.Get( wstrData ) ) {
            continue;
        }
        ks.EndReading();
        vecNoticeInfo_.push_back( wstrData );
    }
}

bool KStrengthManager::LoadSetItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setItemList_ )
{
    setItemList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        GCITEMID dwItemID;
        if( kLuaMng_.GetValue( i, dwItemID ) != S_OK ) break;
        setItemList_.insert( dwItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KStrengthManager::GetNonStrengthItemList( OUT std::set<GCITEMID>& setNonStrengthItem_ )
{
    setNonStrengthItem_.clear();

    KLocker lock( m_csStrength );
    setNonStrengthItem_ = m_setNonStrengthItem;
}

bool KStrengthManager::IsUseStrengthEquipItem( IN const GCITEMID ItemID_ )
{
    KLocker lock( m_csStrength );

    std::set<GCITEMID>::iterator sit;
    sit = m_setNonStrengthItem.find( ItemID_ );
    if( sit == m_setNonStrengthItem.end() ) {
        return true;
    } else {
        return false;
    }
}

bool KStrengthManager::LoadUpgradeProtectionAssist( IN KLuaManager& kLuaMng_, 
                                                        IN std::string& strTable_,
                                                            OUT std::map<GCITEMID, KStrengthUpgradeProtectionAssist>& mapUpgradeProtectionAssistInfo_ )
{
    mapUpgradeProtectionAssistInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KStrengthUpgradeProtectionAssist kStrengthUpgradeProtectionAssist;

        _JIF( kLuaMng_.GetValue( "ItemID", kStrengthUpgradeProtectionAssist.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "fRatio", kStrengthUpgradeProtectionAssist.m_fRatio ) == S_OK, return false );
        _JIF( LoadLevelRange( kLuaMng_, std::string("LevelRange"), kStrengthUpgradeProtectionAssist.m_prLevelRange.first, kStrengthUpgradeProtectionAssist.m_prLevelRange.second ), return false );

        if ( !mapUpgradeProtectionAssistInfo_.insert( std::make_pair( kStrengthUpgradeProtectionAssist.m_ItemID, kStrengthUpgradeProtectionAssist ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << kStrengthUpgradeProtectionAssist.m_ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::LoadStrengthGPCost( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD, std::pair<int, int>>& mapStrengthGPCost_ )
{
    mapStrengthGPCost_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwLevel = 0;
        std::pair<int, int> prCost;

        _JIF( kLuaMng_.GetValue( 1, dwLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prCost.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, prCost.second ) == S_OK, return false );

        if ( !mapStrengthGPCost_.insert( std::make_pair( dwLevel, prCost ) ).second ) {
            START_LOG( cerr, L"중복된 강화 가격이 있음. Level : " << dwLevel ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KStrengthManager::CheckUpgradeProtectionLevel( IN const GCITEMID ItemID_, IN const int nItemLevel_, OUT float& fRatio_ )
{
    std::map< GCITEMID, KStrengthUpgradeProtectionAssist>::iterator mit;

    KLocker lock( m_csStrength );
    mit = m_mapUpgradeProtectionAssistInfo.find( ItemID_ );
    if( mit == m_mapUpgradeProtectionAssistInfo.end() ) {
        return false;
    }

    fRatio_ = mit->second.m_fRatio; // 추가 확률값 세팅.
    if( mit->second.m_prLevelRange.first <= nItemLevel_ && mit->second.m_prLevelRange.second >= nItemLevel_ ) {
        return true;
    } else {
        return false;
    }
}

bool KStrengthManager::GetStrengthGPCost( IN const DWORD& dwLevel_, IN const int& nEquipType_, OUT int& nGPCost_ )
{
    std::map<DWORD, std::pair<int, int>>::iterator mitCost;

    KLocker lock( m_csStrength );

    mitCost = m_mapStrengthGPCost.find( dwLevel_ );
    if( m_mapStrengthGPCost.end() == mitCost ) {
        return false;
    }

    // 무기가격 / 방어구가격 구분
    if ( KStrengthManager::EISP_WEAPON == nEquipType_ ) {
        nGPCost_ = mitCost->second.first;
    }
    else {
        nGPCost_ = mitCost->second.second;
    }

    return true;
}