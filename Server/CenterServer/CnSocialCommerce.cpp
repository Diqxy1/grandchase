#include "CenterSimLayer.h"
#include "CnSocialCommerce.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "CenterDBThread.h"
#include "Lua/KLuaManager.h"
#include "CnItemProvider.h"
#include "GCEnum.h"
#include "../GameServer/ItemManager.h"

ImplementSingleton( KCnSocialCommerce );
ImplOstmOperatorW2A( KCnSocialCommerce );
NiImplementRootRTTI( KCnSocialCommerce );
ImplToStringW( KCnSocialCommerce )
{
    KLocker lock( m_csSocialCommerce );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapInfoOfSellType.size() )
        << TOSTRINGW( m_nGetUserCount )
        << TOSTRINGW( m_bChanged );
}

KCnSocialCommerce::KCnSocialCommerce(void)
: m_kUpdateCheckTimer( 60 * 1000 )
,m_kListUpdateTimer( 60 * 1000 * 30 )
,m_kRewardListGetTimer( 60 * 1000 * 30 )
,m_nGetUserCount(0)
,m_bChanged( true )
{
    m_mapInfoOfSellType.clear();
}

KCnSocialCommerce::~KCnSocialCommerce(void)
{
}

bool KCnSocialCommerce::LoadScript()
{
    KLuaManager kLuaMng;

    DWORD dwUpdateCheckTimer = 0;
    DWORD dwListUpdateTimer = 0;
    DWORD dwRewardListGetTimer = 0;
    int nCount;

    _JIF( kLuaMng.DoFile( "CnSocialCommerce.lua" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "CheckDataTickGap", dwUpdateCheckTimer ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DataUpdateTickGap", dwListUpdateTimer ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GetRewardListTickGap", dwRewardListGetTimer ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GetUserCount", nCount ) == S_OK, return false );

    {
        KLocker lock( m_csSocialCommerce );
        m_nGetUserCount = nCount;
    }
    m_kUpdateCheckTimer.SetInterval( dwUpdateCheckTimer );
    m_kListUpdateTimer.SetInterval( dwListUpdateTimer );
    m_kRewardListGetTimer.SetInterval( dwRewardListGetTimer );

    START_LOG( cerr, L"센터 소셜커머스 데이터 로드 완료. " )
        << BUILD_LOG( dwUpdateCheckTimer )
        << BUILD_LOG( dwListUpdateTimer )
        << BUILD_LOG( dwRewardListGetTimer )
        << BUILD_LOG( nCount )
        << END_LOG;

    return true;
}


void KCnSocialCommerce::Tick()
{
    // DB설정 정보가 변경 되었는지 확인
    CheckChangeDBSettingDataTick();
    // 공동구매 달성 여부 갱신을 위해서 일정 주기로 전체 리스트 얻음
    UpdateAllListTick();
    // 공동구매 달성 된 아이템에 대해 보상 유저 정보를 얻어서 지급 요청 한다
    GetRewardUserListTick();
}

void KCnSocialCommerce::CheckChangeDBSettingDataTick()
{
    if ( m_kUpdateCheckTimer.CheckTime() ) {
        bool bCheckChange = true;
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT, L"", 0, 0, bCheckChange );
    }

    if ( CheckChange() ) {
        // 게임서버 전체로 데이터 전송
        KECN_SOCIAL_COMMERCE_INIT_DATA_NOT kPacket;
        GetSocialCommerceSettingData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SOCIAL_COMMERCE_INIT_DATA_NOT, kPacket );

        m_bChanged = false;
    }
}

void KCnSocialCommerce::UpdateAllListTick()
{
    if ( m_kListUpdateTimer.CheckTime() ) {
        bool bCheckChange = false;
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT , L"", 0, 0, bCheckChange );
    }
}

void KCnSocialCommerce::GetRewardUserListTick()
{
    if ( m_kRewardListGetTimer.CheckTime() ) {
        // ItemProvider에서 처리 중인 데이터 없는지 확인
        if ( false == SiKCnItemProvider()->IsLock() ) {
            KDB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ kPacket;
            DWORD dwEventUID = 0;

            // 공동구매 성공 된 녀석을 그냥 하나씩 뽑는다 (지급 완료 처리 되면 그 다음 녀석으로 뽑아옴)
            GetGroupPurchaseSuccessInfo( dwEventUID, kPacket.m_nGetType );

            // 공동구매 성공한 정보를 얻었다면 DB처리
            if ( 0 != dwEventUID ) {
                kPacket.m_dwEventUID = dwEventUID;
                SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ , L"", 0, 0, kPacket );
            }
        }
        else {
            START_LOG( clog, L"ItemProvider에서 아직 아이템 지급 처리 중이다" ) << END_LOG;
        }
    }
}

bool KCnSocialCommerce::CheckChange()
{
    KLocker lock( m_csSocialCommerce );
    if ( m_bChanged ) {
        m_bChanged = false;
        return true;
    }

    return false;
}

void KCnSocialCommerce::GetSocialCommerceSettingData( OUT KECN_SOCIAL_COMMERCE_INIT_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csSocialCommerce );
    kPacket_ = m_mapInfoOfSellType;
}

void KCnSocialCommerce::UpdateSocialCommerceSettingData( IN const std::map<int, KInformationDataOfSellType>& mapInfoOfSellType_, IN bool& bChanged_ )
{
    KLocker lock( m_csSocialCommerce );

    if ( true == bChanged_ ) {
    m_mapInfoOfSellType = mapInfoOfSellType_;
    }
    else { // Global bit 변경이 아니면 판매 정보만 갱신(그냥 덮으면 보상 정보 날아간다)
        std::map<int, KInformationDataOfSellType>::iterator mitAll;
        std::map<int, KInformationDataOfSellType>::iterator mitFromDB;
        std::map<int, KInformationDataOfSellType> mapFromDBData = mapInfoOfSellType_;
        for ( mitAll = m_mapInfoOfSellType.begin(); mitAll != m_mapInfoOfSellType.end(); mitAll++ ) {
            mitFromDB = mapFromDBData.find( mitAll->first );
            if ( mapFromDBData.end() != mitFromDB ) {
                mitAll->second.m_mapSellItem = mitFromDB->second.m_mapSellItem;
            }
        }
    }
    
    m_bChanged = bChanged_;
}

void KCnSocialCommerce::GetGroupPurchaseSuccessInfo( OUT DWORD& dwEventUID_, OUT int& nGetType_ )
{
    dwEventUID_ = 0;

    std::map<int, KInformationDataOfSellType>::iterator mitAll;
    std::map<DWORD, KSCSellInfo>::iterator mitSell;

    KLocker lock( m_csSocialCommerce );

    for( mitAll = m_mapInfoOfSellType.begin(); mitAll != m_mapInfoOfSellType.end(); mitAll++ ) {
        // 공동구매 타입에 한해서 검색한다
        if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE > mitAll->first ||
            KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS < mitAll->first ) {
            continue;
        }


        for ( mitSell = mitAll->second.m_mapSellItem.begin(); mitAll->second.m_mapSellItem.end() != mitSell; mitSell++ ) {
            if ( KSCSellInfo::FLAG_SELL_GROUPPURCHASE_SUCCESS == mitSell->second.m_nGroupPurchaseFlag ) {
                dwEventUID_ = mitSell->first;
                // 아이템 지급 경로 세팅
                if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE == mitAll->first ) {
                    nGetType_ = KItemManager::EWIT_SC_GROUP_PURCHASE_ONE_PLUS_ONE;
                }
                else {
                    nGetType_ = KItemManager::EWIT_SC_GROUP_PURCHASE_BONUS;
                }
                return;
            }
        }
    }
}

bool KCnSocialCommerce::GetRewardListByEventUID( IN const DWORD& dwEventUID_, OUT VEC_REWARD& vecReward_ )
{
    std::map<int, KInformationDataOfSellType>::iterator mitAll;
    std::map<DWORD, VEC_REWARD>::iterator mitReward;

    KLocker lock( m_csSocialCommerce );
    for( mitAll = m_mapInfoOfSellType.begin(); mitAll != m_mapInfoOfSellType.end(); mitAll++ ) {
        // 공동구매 타입에 한해서 검색한다
        if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE > mitAll->first ||
            KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS < mitAll->first ) {
                continue;
        }

        // EventUID에 해당 하는 보상 아이템 정보를 얻는다
        mitReward = mitAll->second.m_mapRewardItem.find( dwEventUID_ );
        if ( mitAll->second.m_mapRewardItem.end() != mitReward ) {
            vecReward_ = mitReward->second;
            return true;
        }
    }
    
    return false;
}

void KCnSocialCommerce::SetGroupPurchaseRewardInfo( IN VEC_GROUP_PURCHASE_REWARD& vecInfo_ )
{
    KLocker lock( m_csSocialCommerce );
    m_vecRewardInfo.clear();
    m_vecRewardInfo = vecInfo_;
}

void KCnSocialCommerce::GetGroupPurchaseRewardInfo( OUT VEC_GROUP_PURCHASE_REWARD& vecInfo_ )
{
    KLocker lock( m_csSocialCommerce );
    vecInfo_ = m_vecRewardInfo;
}
