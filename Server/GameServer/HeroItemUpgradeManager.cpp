#include "HeroItemUpgradeManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "ItemManager.h"
#include "UserEvent.h"
#include "Log4.h"

KHeroItemUpgradeManager::KHeroItemUpgradeManager(void)
{
    m_mapHeroItemUpgradeInfoList.clear();
}

KHeroItemUpgradeManager::~KHeroItemUpgradeManager(void)
{
}

bool KHeroItemUpgradeManager::LoadScript( IN const std::string& strScript_ )
{
    KLuaManager kLuaMng;
    _JIF( kLuaMng.DoFile( strScript_.c_str() ) == S_OK, return false );

    GCITEMID 	ItemID; // 결과물 아이템
    int      	nGrade; // 결과물 등급
    int      	nDuration; // 결과물 수량
    int      	nPeriod; // 결과물 기간
    int      	nPrice; // 결과물 GP가격
    int      	nLevel; // 결과물 레벨
    std::map< PAIR_DWORD_INT, KHeroItemUpgradeInfo > mapHeroItemUpgradeInfoList;

    mapHeroItemUpgradeInfoList.clear();

    _JIF( kLuaMng.BeginTable( "HeroItemUpgradeInfo" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        // 일단 걍 멤버 변수에 바로 읽게 만들어놨음...
        _JIF( kLuaMng.GetValue( "ItemID", ItemID ) == S_OK, return false );
        if ( kLuaMng.GetValue( "Grade", nGrade ) != S_OK ) {
            nGrade = -1;
        }
        PAIR_DWORD_INT prKey = std::make_pair(ItemID, nGrade ); // GoodsID와 Grade를 키값으로 구분한다.

        _JIF( kLuaMng.GetValue( "Count", nDuration ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Period", nPeriod ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Price", nPrice ) == S_OK, return false );
        if ( kLuaMng.GetValue( "Level", nLevel ) != S_OK ) {
            nLevel = -1;
        }

        std::vector<KDropItemInfo> vecMaterial;
        _JIF( LoadMaterials( kLuaMng, vecMaterial ), return false );

        KHeroItemUpgradeInfo kHeroItemUpgradeInfo;
        kHeroItemUpgradeInfo.m_ItemID = ItemID;
        kHeroItemUpgradeInfo.m_nGrade = nGrade;
        kHeroItemUpgradeInfo.m_nDuration = nDuration;
        kHeroItemUpgradeInfo.m_nPeriod = nPeriod;
        kHeroItemUpgradeInfo.m_nPrice = nPrice;
        kHeroItemUpgradeInfo.m_nLevel = nLevel;
        kHeroItemUpgradeInfo.m_vecMaterials.swap( vecMaterial );
        mapHeroItemUpgradeInfoList.insert( std::make_pair(prKey, kHeroItemUpgradeInfo) );

        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 영웅 아이템 업그레이드 스크립트 로드 완료 : " << strScript_.c_str() )
        << BUILD_LOG( mapHeroItemUpgradeInfoList.size() )
        << END_LOG;

    KLocker lock( m_csHeroItemUpgradeManager );
    m_mapHeroItemUpgradeInfoList.swap( mapHeroItemUpgradeInfoList );
    return true;
}

bool KHeroItemUpgradeManager::LoadMaterials( IN OUT KLuaManager& kLuaMng_, OUT std::vector<KDropItemInfo>& vecMaterial_ )
{
    vecMaterial_.clear();
    _JIF( kLuaMng_.BeginTable( "Material" ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KDropItemInfo kMaterial;
        int nGrade = -1;
        if ( kLuaMng_.GetValue( i++, kMaterial.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kMaterial.m_nDuration ) == S_OK, return false ); // 영구 아이템인 경우 -1 이라 그냥 두면 된다.
        _JIF( kLuaMng_.GetValue( i++, nGrade ) == S_OK, return false ); // 재료 아이템의 등급 정보를 읽는다.
        kMaterial.m_cGradeID = static_cast<char>( nGrade );
        vecMaterial_.push_back( kMaterial );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

// 클라 전달 할 정보 세팅 하는 access method
void KHeroItemUpgradeManager::GetHeroItemUpgradeInfo( OUT KEVENT_HERO_ITEM_UPGRADE_INFO_ACK& kPacket_ )
{
    kPacket_.clear();

    KLocker lock( m_csHeroItemUpgradeManager );

    if ( true == m_mapHeroItemUpgradeInfoList.empty() ) {
        START_LOG( cwarn, L"영웅던전 업그레이드 정보가 비었음. 스크립트 확인 필요." ) << END_LOG;

        return;
    }

    kPacket_ = m_mapHeroItemUpgradeInfoList;
}

// 클라에서 업그레이드 요청 한 prKey값에 대응 되는 데이터를 검색한다
bool KHeroItemUpgradeManager::GetUpgradeInfoByKey(  IN const PAIR_DWORD_INT& prKey_, OUT KHeroItemUpgradeInfo& upgradeItemInfo_, OUT KDropItemInfo& rewardItemInfo_ )
{
    upgradeItemInfo_.m_vecMaterials.clear();
    std::map< PAIR_DWORD_INT, KHeroItemUpgradeInfo >::iterator mit;
    KLocker lock( m_csHeroItemUpgradeManager );
    mit = m_mapHeroItemUpgradeInfoList.find( prKey_ );
    if ( mit == m_mapHeroItemUpgradeInfoList.end() ) {
        START_LOG( cwarn, L"[KHeroItemUpgradeManager] 키 값에 해당 하는 업그레이드 데이터 없음" )
            << BUILD_LOG( prKey_.first )
            << BUILD_LOG( prKey_.second )
            << END_LOG;
        return false;
    }

    if ( true == mit->second.m_vecMaterials.empty() ) {
        START_LOG( cwarn, L"[KHeroItemUpgradeManager] 키 값에 해당 하는 재료 아이템 데이터 없음")
            << BUILD_LOG( prKey_.first )
            << BUILD_LOG( prKey_.second )
            << END_LOG;
        return false;
    }

    // 업그레이드 시 필요 한 재료 아이템 설정
    upgradeItemInfo_.m_vecMaterials = mit->second.m_vecMaterials;

    // 업그레이드 시 결과물 아이템 설정
    rewardItemInfo_.m_ItemID = mit->second.m_ItemID;
    rewardItemInfo_.m_nDuration = mit->second.m_nDuration;
    rewardItemInfo_.m_nPeriod = mit->second.m_nPeriod;
    rewardItemInfo_.m_cGradeID = static_cast<char>( mit->second.m_nGrade );
    rewardItemInfo_.m_cEquipLevel = static_cast<char>( mit->second.m_nLevel );

    return true;
}