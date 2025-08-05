#include "GSSimLayer.h"
#include "RKTornado.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <KncUtilLua.h> // boost::bind
#include <kncutil.h>
#include "NetError.h"
#include "Log4.h"
#include "UserPacket.h"
#include "UserEvent.h"

ImplementSingleton( KRKTornado );
ImplOstmOperatorW2A( KRKTornado );
NiImplementRootRTTI( KRKTornado );

KRKTornado::KRKTornado(void)
:m_nTornadoVersion(0)
,m_dwTornadoMapItemID(0)
,m_dwTornadoCoinItemID(0)
,m_dwTornadoFailItemID(0)
,m_dwExposeItemToUser(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwFailItemCount(0)
{
    m_mapNonFailRewardList.clear();
    m_mapTotalTornadoInfo.clear();
    m_mapTotalRewardType.clear();
}

KRKTornado::~KRKTornado(void)
{
}

ImplToStringW( KRKTornado )
{
    KLocker lock( m_csTornado );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_nTornadoVersion )
        << TOSTRINGW( m_dwTornadoMapItemID )
        << TOSTRINGW( m_dwTornadoCoinItemID )
        << TOSTRINGW( m_dwTornadoFailItemID )
        << TOSTRINGW( m_dwFailItemCount )
        << TOSTRINGW( m_mapNonFailRewardList.size() )
        << TOSTRINGW( m_mapTotalTornadoInfo.size() )
        << TOSTRINGW( m_mapTotalRewardType.size() );
}

bool KRKTornado::LoadScript()
{
    return _LoadScript(std::string());
}

bool KRKTornado::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    int         nTornadoVersion;
    GCITEMID       dwTornadoMapItemID;   // 토네이도 말판 ItemID
    GCITEMID       dwTornadoCoinItemID;  // 토네이도 코인 ItemID
    GCITEMID       dwTornadoFailItemID;  // 토네이도 FailItemID
	GCITEMID       dwExposeItemToUser;   // 노출 아이템 ID
    DWORD       dwFailItemCount;      // 럭키 ItemCount
    std::map< DWORD, std::map< DWORD, std::set<GCITEMID> > >   mapNonFailRewardList; // 꽝아이템 제외 보상리스트.
    std::map< DWORD, KTornadoInfo >                         mapTotalTornadoInfo; // 릭쿤 토네이도 전체 정보.
    std::map< GCITEMID, DWORD >                                mapTotalRewardType; // 전체 아이템 리스트.

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitRKTornado.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "TornadoVersion", nTornadoVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TornadoItemID", dwTornadoMapItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TornadoCoinItemID", dwTornadoCoinItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TornadoFailCoinItemID", dwTornadoFailItemID ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "ExposeItemToUser", dwExposeItemToUser ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TorandoFailCoinCount", dwFailItemCount ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "InitTornado" ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        KTornadoInfo kTornadoInfo;

        _JIF( kLuaMng.GetValue( "CharType", kTornadoInfo.m_dwCharType ) == S_OK, return false );
        _JIF( LoadRewardRatio( kLuaMng, std::string("RewardItemList"), kTornadoInfo.m_kKairoNormal ), return false );
        _JIF( LoadRewardRatio( kLuaMng, std::string("NonFailRewardItemList"), kTornadoInfo.m_kKairoNonFail ), return false );
        _JIF( LoadRewardRatio( kLuaMng, std::string("FailRewardItemList"), kTornadoInfo.m_kKairoFail ), return false );
        _JIF( LoadItemTypeList( kLuaMng, std::string("TypeItemList"), kTornadoInfo.m_mapTypeItemList ), return false );
        _JIF( LoadRewardList( kLuaMng, std::string("FailItemList"), kTornadoInfo.m_vecFailRewardList ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if( !mapTotalTornadoInfo.insert( std::make_pair( kTornadoInfo.m_dwCharType, kTornadoInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 캐릭터 정보 있음. Type : " << kTornadoInfo.m_dwCharType ) << END_LOG;
        }

        if( !mapNonFailRewardList.insert( std::make_pair( kTornadoInfo.m_dwCharType, kTornadoInfo.m_mapTypeItemList ) ).second ) {
            // 불필요한 로그 제거.
            // START_LOG( cerr, L" 중복된 캐릭터 ItemType 정보 있음. CharType : " << kTornadoInfo.m_dwCharType ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    SetRewardType( mapTotalTornadoInfo, mapTotalRewardType );

    START_LOG( cerr, L"릭쿤 토네이도 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csTornado );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_nTornadoVersion = nTornadoVersion;
        m_dwTornadoMapItemID = dwTornadoMapItemID; 
        m_dwTornadoCoinItemID = dwTornadoCoinItemID; 
        m_dwTornadoFailItemID = dwTornadoFailItemID; 
		m_dwExposeItemToUser = dwExposeItemToUser;
        m_dwFailItemCount = dwFailItemCount; 
        m_mapTotalTornadoInfo.swap( mapTotalTornadoInfo );
        m_mapNonFailRewardList.swap( mapNonFailRewardList );
        m_mapTotalRewardType.swap( mapTotalRewardType );
        return true;
    }
}

bool KRKTornado::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KRKTornado::LoadRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        float fProb = 0.f;
        //bool bDisplay = false;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KRKTornado::LoadItemTypeList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, std::set<GCITEMID> >& mapTypeItemList_ )
{
    mapTypeItemList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwType = 0;
        std::set<GCITEMID> setItemList;

        _JIF( kLuaMng_.GetValue( "Type", dwType ) == S_OK, return false );
        _JIF( LoadItemList( kLuaMng_, std::string("ItemList"), setItemList ), return false );

        // 서버 아이템 타입 리스트 Load.
        if( !mapTypeItemList_.insert( std::make_pair( dwType, setItemList ) ).second ) {
            START_LOG( cerr, L" 중복된 Type 정보가 있음 Type : " << dwType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KRKTornado::LoadItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setItemList_ )
{
    setItemList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    GCITEMID dwData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, dwData ) != S_OK ) break;
        setItemList_.insert( dwData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KRKTornado::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KDropItemInfo>& vecFailReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KDropItemInfo kItem;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecFailReward_.push_back( kItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KRKTornado::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csTornado );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KRKTornado::DoGamble( IN DWORD& dwCharType_, IN DWORD& dwType_ ,OUT KDropItemInfo& kItem_ )
{
    std::map< DWORD, KTornadoInfo >::iterator mit;
    mit = m_mapTotalTornadoInfo.find( dwCharType_ );
    if( mit == m_mapTotalTornadoInfo.end() ) {
        SET_ERROR( ERR_RKTORNODO_03 );
        return false;
    }

    bool bSuccess;
    {
        KLocker lock( m_csTornado );
        if( dwType_ == KRKTornado::AT_NORMAL ) {
            bSuccess = mit->second.m_kKairoNormal.Do( kItem_ );
        } else if( dwType_ == KRKTornado::AT_NONFAIL ) {
            bSuccess = mit->second.m_kKairoNonFail.Do( kItem_ );
        } else{
            SET_ERROR( ERR_RKTORNODO_03 );
            return false;
        }
    }
    if( kItem_.m_ItemID == 0 )
    {
        SET_ERROR( ERR_RKTORNODO_03 );
        return false;
    }

    if( bSuccess ) return true;

    SET_ERROR( ERR_RKTORNODO_03 );
    return false;
}

bool KRKTornado::DoGambleFail( IN DWORD& dwCharType_, OUT KDropItemInfo& kItem_ )
{
    std::map< DWORD, KTornadoInfo >::iterator mit;
    mit = m_mapTotalTornadoInfo.find( dwCharType_ );
    if( mit == m_mapTotalTornadoInfo.end() ) {
        SET_ERROR( ERR_RKTORNODO_03 );
        return false;
    }

    bool bSuccess;
    {
        KLocker lock( m_csTornado );
        bSuccess = mit->second.m_kKairoFail.Do( kItem_ );
    }

    if( kItem_.m_ItemID == 0 )
    {
        SET_ERROR( ERR_RKTORNODO_03 );
        return false;
    }

    if( bSuccess ) return true;

    SET_ERROR( ERR_RKTORNODO_03 );
    return false;
}

bool KRKTornado::GetFailItemList( IN DWORD& dwCharType_, IN DWORD& dwType_ ,OUT std::vector<KDropItemInfo>& vecFailRewardList_ )
{
    vecFailRewardList_.clear();
    if( dwType_ != KRKTornado::IT_FAIL ) { // 꽝 아이템이 아닐경우 추가 보상 없음.
        SET_ERROR( ERR_RKTORNODO_04 );
        return false;
    }

    std::map< DWORD, KTornadoInfo >::iterator mit;
    mit = m_mapTotalTornadoInfo.find( dwCharType_ );
    if( mit == m_mapTotalTornadoInfo.end() ) {
        SET_ERROR( ERR_RKTORNODO_03 );
        return false;
    }

    KLocker lock( m_csTornado );
    vecFailRewardList_ = mit->second.m_vecFailRewardList;
    return true;
}

bool KRKTornado::IsCheckTornadoCoin( IN GCITEMID& dwItemID_ )
{
    KLocker lock( m_csTornado );
    if( dwItemID_ == m_dwTornadoCoinItemID ) {
        return true;
    } else {
        return false;
    }
}

bool KRKTornado::IsCheckTornadoFailItem( IN GCITEMID& dwItemID_ )
{
    KLocker lock( m_csTornado );
    if( dwItemID_ == m_dwTornadoFailItemID ) {
        return true;
    } else {
        return false;
    }
}

void KRKTornado::SetRewardType( IN std::map< DWORD, KTornadoInfo >& mapTotalTornadoInfo_, OUT std::map< GCITEMID, DWORD >& mapTotalRewardType_ )
{
    mapTotalRewardType_.clear();

    std::map< DWORD, KTornadoInfo >::iterator mit;
    std::map< DWORD, std::set<GCITEMID> >::iterator mitReward;
    std::set<GCITEMID>::iterator sit;
    for( mit = mapTotalTornadoInfo_.begin(); mit != mapTotalTornadoInfo_.end(); ++mit ) {
        for( mitReward = mit->second.m_mapTypeItemList.begin(); mitReward != mit->second.m_mapTypeItemList.end(); ++mitReward ) {
            for( sit = mitReward->second.begin(); sit != mitReward->second.end(); ++sit ) {
                if( !mapTotalRewardType_.insert( std::make_pair( *sit, mitReward->first ) ).second ) {
                    START_LOG( cwarn, L" 중복된 캐릭터 Item정보 있음. Item : " << *sit ) 
                        << END_LOG;
                }
            }
        }
    }
}

bool KRKTornado::GetItemType( IN GCITEMID& dwItemID_, OUT DWORD& dwType_ )
{
    std::map< GCITEMID, DWORD >::iterator mit;
    KLocker lock( m_csTornado );
    mit = m_mapTotalRewardType.find( dwItemID_ );
    if( mit == m_mapTotalRewardType.end() ) {
        dwType_ = KRKTornado::IT_FAIL;
        return false;
    }
    dwType_ = mit->second;
    return true;
}

void KRKTornado::GetNonFailRewardList( OUT std::map<DWORD,std::map<DWORD, std::set<GCITEMID> > >& mapNonFailRewardList_ )
{
    mapNonFailRewardList_.clear();
    mapNonFailRewardList_ = m_mapNonFailRewardList;
}

void KRKTornado::GetEquipmentList( IN DWORD& dwCharType_, OUT std::set<GCITEMID>& setItemList_ )
{
    std::map< DWORD, KTornadoInfo >::iterator mit;
    std::map< DWORD, std::set<GCITEMID> >::iterator mitList;
    mit = m_mapTotalTornadoInfo.find( dwCharType_ );
    if( mit == m_mapTotalTornadoInfo.end() ) {
        return;
    }
    mitList = mit->second.m_mapTypeItemList.find( KRKTornado::IT_EQUIPMENT );
    if( mitList == mit->second.m_mapTypeItemList.end() ) {
        return;
    }

    setItemList_ = mitList->second;
}

bool KRKTornado::GetEquipemntItem( IN DWORD& dwCharType_, IN const GCITEMID dwItemID_, OUT KDropItemInfo& kReward_ )
{
    std::map< DWORD, KTornadoInfo >::iterator mit;
    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KDropItemInfo> vecItemList;
    mit = m_mapTotalTornadoInfo.find( dwCharType_ );
    if( mit == m_mapTotalTornadoInfo.end() ) {
        return false;
    }
    mit->second.m_kKairoNormal.GetItemList( vecItemList );
    vit = std::find_if( vecItemList.begin(), vecItemList.end(),
        boost::bind( &KDropItemInfo::m_ItemID, _1 ) == dwItemID_ );

    if( vit == vecItemList.end() ) {
        START_LOG( cerr, L"릭쿤 토네이도 보상정보에 해당 아이템이 없음. CharType : " << dwCharType_ << L", ItemID : " <<  dwItemID_ ) << END_LOG;
        return false;
    }

    kReward_ = *vit;
    START_LOG( clog, L"CharType : " << dwCharType_ << L", ItemID : " <<  dwItemID_ ) << END_LOG;
    return true;
}

void KRKTornado::GetExposeItem( OUT GCITEMID& dwExposeItemToUser_ )
{
	dwExposeItemToUser_ = m_dwExposeItemToUser;
}

bool KRKTornado::SendRKTornadoItemListNot()
{
    // 릭쿤 토네이도 정보 전달.
    KEVENT_RKTORNADO_ITEM_LIST_NOT kPacket;
    if ( true == IsRun() ) { // 이벤트 기간일때 정보 전달.
        kPacket.m_dwLuckyEnableCount = GetFailItemCount();
        GetNonFailRewardList( kPacket.m_mapNonFailRewardList );
        GetExposeItem( kPacket.m_dwExposeItemToUser );
        kPacket.m_bEventEnable = true;
    } else {
        kPacket.m_dwLuckyEnableCount = 0;
        kPacket.m_mapNonFailRewardList.clear();
        kPacket.m_bEventEnable = false;
    }

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_RKTORNADO_ITEM_LIST_NOT, kPacket );

    return true;
}
