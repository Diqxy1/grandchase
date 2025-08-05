#include "VipEventManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

KItemBox::KItemBox(void)
{
}

KItemBox::~KItemBox(void)
{
}

void KItemBox::DumpList()
{
    //std::cout << L"--- Item List (size:" << m_vecItemList.size() << L") ---" << dbg::endl;
    //std::vector<KDropItemInfo>::const_iterator cvit;
    //for ( cvit = m_vecItemList.begin() ; cvit != m_vecItemList.end() ; ++cvit ) {
    //    std::cout << L"ItemID : " << cvit->m_ItemID << L", Count : " << cvit->m_nDuration << L", Period : " << cvit->m_nPeriod << dbg::endl;
    //}
    //std::cout << L"--- End ---" << dbg::endl;
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/

ImplementSingleton( KVipEventManager );
ImplOstmOperatorW2A( KVipEventManager );
NiImplementRootRTTI( KVipEventManager );

KVipEventManager::KVipEventManager(void)
{
}

KVipEventManager::~KVipEventManager(void)
{
}

ImplToStringW( KVipEventManager )
{
    KLocker lock(m_csVipEvent);
    return START_TOSTRINGW
        << TOSTRINGW( m_mapRewardItem.size() )
        << TOSTRINGW( m_mapPayTable.size() )
        << TOSTRINGW( m_mapRewardByVer.size() )
        << TOSTRINGW( m_mapEventByVer.size() );
}

bool KVipEventManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KVipEventManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> > mapRewardItem;
    std::map<PAY_INDEX,std::pair<int,int> > mapPayTable;
    std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> > mapRewardByVer;

    const std::string strScriptName = "InitVipReward.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadRewardItem( kLuaMng, mapRewardItem ), return false );
    _JIF( LoadPayTable( kLuaMng, mapPayTable ), return false );
    _JIF( LoadRewardByVer( kLuaMng, mapRewardByVer ), return false );

    START_LOG( cerr, L"Load VIP-Event script.." )
        << BUILD_LOG( mapRewardItem.size() )
        << BUILD_LOG( mapPayTable.size() )
        << BUILD_LOG( mapRewardByVer.size() ) << END_LOG;

    KLocker lock( m_csVipEvent );
    m_mapRewardItem.swap( mapRewardItem );
    m_mapPayTable.swap( mapPayTable );
    m_mapRewardByVer.swap( mapRewardByVer );
    return true;
}

bool KVipEventManager::LoadRewardItem( KLuaManager& kLuaMng_, OUT std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "RewardItem" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        REWARD_TYPE rewardType = -1;
        std::map<REWARD_ID,KItemBox> mapReward;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( LoadRewardList( kLuaMng_, rewardType, mapReward ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( rewardType < 0 ) continue;
        if ( mapReward.empty() ) continue;
        _JIF( mapList_.insert( std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >::value_type(rewardType, mapReward) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load RewardItem.. size : " << mapList_.size() ) << END_LOG;
    return true;
}

bool KVipEventManager::LoadRewardList( KLuaManager& kLuaMng_, OUT REWARD_TYPE& rewardType_, OUT std::map<REWARD_ID,KItemBox>& mapList_ )
{
    rewardType_ = -1;
    mapList_.clear();

    _JIF( kLuaMng_.GetValue( "Type", rewardType_ ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "Reward" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        REWARD_ID rewardID = 0;
        std::vector<KDropItemInfo> vecReward;
        KItemBox kReward;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( LoadReward( kLuaMng_, rewardID, vecReward ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( rewardID == 0 ) continue;
        if ( vecReward.empty() ) continue;
        kReward.SetList( vecReward );
        _JIF( mapList_.insert( std::map<REWARD_ID,KItemBox>::value_type( rewardID, kReward ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KVipEventManager::LoadReward( KLuaManager& kLuaMng_, OUT REWARD_ID& rewardID_, OUT std::vector<KDropItemInfo>& vecList_ )
{
    rewardID_ = 0;
    vecList_.clear();

    _JIF( kLuaMng_.GetValue( "RewardID", rewardID_ ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "ItemList" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KDropItemInfo kItem;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( kItem.m_ItemID == 0 ) continue;
        vecList_.push_back( kItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KVipEventManager::LoadPayTable( KLuaManager& kLuaMng_, OUT std::map<PAY_INDEX,std::pair<int,int> >& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "PayTable" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        PAY_INDEX payIndex = 0;
        std::pair<int,int> prPayRange(0,0);

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, payIndex ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prPayRange.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, prPayRange.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( payIndex == 0 ) continue;
        if ( prPayRange.first < 0 ) continue;
        if ( prPayRange.second < 0 ) continue;
        if ( prPayRange.second < prPayRange.first ) continue;
        _JIF( mapList_.insert( std::map<PAY_INDEX,std::pair<int,int> >::value_type( payIndex, prPayRange ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load Pay Table.. size : " << mapList_.size() ) << END_LOG;
    return true;
}

bool KVipEventManager::LoadRewardByVer( KLuaManager& kLuaMng_, OUT std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "RewardByVer" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        USHORT usVer = 0;
        REWARD_ID newbieRewardID = 0;
        std::map<PAY_INDEX,REWARD_ID> mapRewardTable;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "Ver", usVer ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Newbie", newbieRewardID ) == S_OK, return false );
        _JIF( LoadRewardTable( kLuaMng_, mapRewardTable ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( usVer == 0 ) continue;
        _JIF( mapRewardTable.insert( std::map<PAY_INDEX,REWARD_ID>::value_type(NEWBIE_REWARD, newbieRewardID) ).second, return false );
        _JIF( mapList_.insert( std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >::value_type( usVer, mapRewardTable ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load Reward by Version.. size : " << mapList_.size() ) << END_LOG;
    return true;
}

bool KVipEventManager::LoadRewardTable( KLuaManager& kLuaMng_, OUT std::map<PAY_INDEX,REWARD_ID>& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "VIP" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        PAY_INDEX payIndex = 0;
        REWARD_ID rewardID = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, payIndex ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, rewardID ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( payIndex == 0 ) continue;
        if ( rewardID == 0 ) continue;
        _JIF( mapList_.insert( std::map<PAY_INDEX,REWARD_ID>::value_type(payIndex,rewardID) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KVipEventManager::SendVipEventInfoReqToDB()
{
    if ( IsEventExist() ) {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_VIPEVENT_INFO_REQ, L"", 0, 0 );
    }
}

void KVipEventManager::UpdateEvent( IN std::map<USHORT,KVipEvent>& mapInfo_ )
{
    // Version별 이벤트 기간 등의 설정정보는 DB에서 받아옴.
    START_LOG( cerr, L"Update VIP-Event from DB.. size : " << mapInfo_.size() ) << END_LOG;
    KLocker lock( m_csVipEvent );
    m_mapEventByVer.swap( mapInfo_ );
}

void KVipEventManager::DumpInfo()
{
    DumpRewardItem();
    DumpPayTable();
    DumpRewardByVer();
    DumpEventByVer();
}

void KVipEventManager::DumpRewardItem()
{
    //KLocker lock( m_csVipEvent );

    //std::cout << L"--- Dump Reward Item (size:" << m_mapRewardItem.size() << L") ---" << dbg::endl;

    //std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >::iterator mit;
    //for ( mit = m_mapRewardItem.begin() ; mit != m_mapRewardItem.end() ; ++mit ) {
    //    std::cout << L"RewardType : " << mit->first << dbg::endl;

    //    std::map<REWARD_ID,KItemBox>::iterator mitGroup;
    //    for ( mitGroup = mit->second.begin() ; mitGroup != mit->second.end() ; ++mitGroup ) {
    //        std::cout << L"RewardID : " << mitGroup->first << dbg::endl;
    //        mitGroup->second.DumpList();
    //    }
    //    std::cout << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KVipEventManager::DumpPayTable()
{
    //KLocker lock( m_csVipEvent );

    //std::cout << L"--- Dump Pay Table (size:" << m_mapPayTable.size() << L") ---" << dbg::endl;

    //std::map<PAY_INDEX,std::pair<int,int> >::const_iterator cmit;
    //for ( cmit = m_mapPayTable.begin() ; cmit != m_mapPayTable.end() ; ++cmit ) {
    //    std::cout << L"ID : " << cmit->first
    //        << L", " << cmit->second.first
    //        << L"~" << cmit->second.second << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KVipEventManager::DumpRewardByVer()
{
    //KLocker lock( m_csVipEvent );

    //std::cout << L"--- Dump Reward By Ver (size:" << m_mapRewardByVer.size() << L") ---" << dbg::endl;

    //std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >::const_iterator cmit;
    //for ( cmit = m_mapRewardByVer.begin() ; cmit != m_mapRewardByVer.end() ; ++cmit ) {
    //    std::cout << L"Ver : " << cmit->first << dbg::endl;

    //    std::map<PAY_INDEX,REWARD_ID>::const_iterator cmitPayReward;
    //    for ( cmitPayReward = cmit->second.begin() ; cmitPayReward != cmit->second.end() ; ++cmitPayReward ) {
    //        std::cout << L"PayID : " << cmitPayReward->first
    //            << L", RewardID : " << cmitPayReward->second << dbg::endl;
    //    }
    //    std::cout << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KVipEventManager::DumpEventByVer()
{
    //CTime tmToday( CTime::GetCurrentTime() );
    //time_t tmCurrent = KncUtil::TimeToInt( tmToday );

    //KLocker lock( m_csVipEvent );

    //std::cout << L"--- Dump Event By Ver (size:" << m_mapEventByVer.size() << L") ---" << dbg::endl;

    //std::map<USHORT,KVipEvent>::const_iterator cmit;
    //for ( cmit = m_mapEventByVer.begin() ; cmit != m_mapEventByVer.end() ; ++cmit ) {
    //    const time_t& tmOpen = cmit->second.m_prEventRange.first;
    //    const time_t& tmClose = cmit->second.m_prEventRange.second;

    //    std::cout << L"ver : " << cmit->second.m_usVersion
    //        << L", IsActive : " << (IsActive(cmit->second.m_prEventRange) ? L"TRUE" : L"FALSE")
    //        << L", date : " << (LPCWSTR)CTime(tmOpen).Format( KNC_TIME_FORMAT )
    //        << L" ~ " << (LPCWSTR)CTime(tmClose).Format( KNC_TIME_FORMAT )
    //        << L", CurrentTime : " << (LPCWSTR)tmToday.Format( KNC_TIME_FORMAT ) << dbg::endl;

    //    std::map<int,bool>::const_iterator cmitOpt;
    //    for ( cmitOpt = cmit->second.m_mapEventEnable.begin() ; cmitOpt != cmit->second.m_mapEventEnable.end() ; ++cmitOpt ) {
    //        std::cout << L"EventType : " << cmitOpt->first
    //            << L", Enable : " << (cmitOpt->second ? L"TRUE" : L"FALSE") << dbg::endl;
    //    }
    //    std::cout << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KVipEventManager::GetEventList( OUT std::map<USHORT,KVipEvent>& mapList_ )
{
    KLocker lock( m_csVipEvent );
    mapList_ = m_mapEventByVer;
}

bool KVipEventManager::GetRewardItemByType( IN const USHORT& usType_, OUT std::map<USHORT,std::vector<KDropItemInfo> >& mapList_ )
{
    mapList_.clear();

    KLocker lock( m_csVipEvent );

    std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >::iterator mitType;
    mitType = m_mapRewardItem.find( usType_ );
    __JIF( mitType != m_mapRewardItem.end(), return false );

    std::map<REWARD_ID,KItemBox>::iterator mitReward;
    for ( mitReward = mitType->second.begin() ; mitReward != mitType->second.end() ; ++mitReward ) {
        std::vector<KDropItemInfo> vecList;
        mitReward->second.GetList( vecList );
        mapList_[ mitReward->first ] = vecList;
    }
    return true;
}

void KVipEventManager::GetPayTable( OUT std::map<PAY_INDEX,PAIR_INT>& mapList_ )
{
    KLocker lock( m_csVipEvent );
    mapList_ = m_mapPayTable;
}

void KVipEventManager::GetRewardByVer( OUT std::map<USHORT,std::map<USHORT,USHORT> >& mapList_ )
{
    KLocker lock( m_csVipEvent );
    mapList_ = m_mapRewardByVer;
}

bool KVipEventManager::GetCurrentReward( IN const USHORT& usVer_, IN const int& nType_, IN const std::set<USHORT>& setPayIndex_, OUT std::map<USHORT,std::vector<KDropItemInfo> >& mapRewardItem_ )
{
    // 누적시키면서 부르기 때문에 컨테이너 초기화 하면 안됨.
    KLocker lock( m_csVipEvent );

    std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >::iterator mitReward;
    mitReward = m_mapRewardItem.find( nType_ );
    if ( mitReward == m_mapRewardItem.end() ) {
        START_LOG( cwarn, L"해당 Type의 보상그룹이 없음. nType_ : " << nType_ ) << END_LOG;
        return false;
    }

    std::map<REWARD_ID,KItemBox>& mapRewardGroup = mitReward->second;

    std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >::iterator mitVer;
    mitVer = m_mapRewardByVer.find( usVer_ );
    if ( mitVer == m_mapRewardByVer.end() ) {
        START_LOG( cwarn, L"해당 버전의 보상정보가 없음. usVer_ : " << usVer_ ) << END_LOG;
        return false;
    }

    std::map<PAY_INDEX,REWARD_ID>& mapPayRewardID = mitVer->second;

    std::set<USHORT>::const_iterator csit;
    for ( csit = setPayIndex_.begin() ; csit != setPayIndex_.end() ; ++csit ) {
        const PAY_INDEX& payIndex = *csit;

        std::map<PAY_INDEX,REWARD_ID>::iterator mitPayID;
        mitPayID = mapPayRewardID.find( payIndex );
        if ( mitPayID == mapPayRewardID.end() ) {
            START_LOG( cwarn, L"해당 금액ID 보상정보가 없음. payIndex : " << payIndex ) << END_LOG;
            continue;
        }

        const USHORT& usRewardID = mitPayID->second;

        std::map<REWARD_ID,KItemBox>::iterator mitItemList;
        mitItemList = mapRewardGroup.find( usRewardID );
        if ( mitItemList == mapRewardGroup.end() ) {
            START_LOG( cwarn, L"해당 보상ID 아이템리스트가 없음. usRewardID : " << usRewardID ) << END_LOG;
            continue;
        }

        std::vector<KDropItemInfo> vecItem;
        mitItemList->second.GetList( vecItem );
        mapRewardItem_[payIndex].swap( vecItem );
    }

    if ( mapRewardItem_.empty() ) {
        START_LOG( clog, L"보상할 아이템이 없음." ) << END_LOG;
        return false;
    }

    return true;
}

bool KVipEventManager::IsActive( IN const std::pair<time_t,time_t>& prEventRange_ )
{
    CTime tmToday( CTime::GetCurrentTime() );
    time_t tmCurrent = KncUtil::TimeToInt( tmToday );
    const time_t& tmOpen = prEventRange_.first;
    const time_t& tmClose = prEventRange_.second;
    return ( tmOpen <= tmCurrent && tmCurrent <= tmClose );
}

bool KVipEventManager::IsEnd( IN const std::pair<time_t,time_t>& prEventRange_ )
{
    CTime tmToday( CTime::GetCurrentTime() );
    time_t tmCurrent = KncUtil::TimeToInt( tmToday );
    const time_t& tmOpen = prEventRange_.first;
    const time_t& tmClose = prEventRange_.second;
    return ( tmOpen > tmCurrent && tmClose > tmCurrent );
}

bool KVipEventManager::IsNewbieUser( IN const time_t& tmFirstLogin_, IN const std::pair<time_t,time_t>& prEventRange_ )
{
    const time_t& tmOpen = prEventRange_.first;
    const time_t& tmClose = prEventRange_.second;
    return ( tmOpen <= tmFirstLogin_ && tmFirstLogin_ <= tmClose );
}

bool KVipEventManager::IsEventExist()
{
    KLocker lock( m_csVipEvent );
    return ( !m_mapRewardByVer.empty() );
}
