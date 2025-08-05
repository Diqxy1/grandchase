#include "PresentBox.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KPresentBox );
ImplOstmOperatorW2A( KPresentBox );
NiImplementRootRTTI( KPresentBox );

KPresentBox::KPresentBox(void)
:m_dwPresentBoxItem(0)
,m_dwVersion(1)
,m_tmBegin(1)
,m_tmEnd(1)
{
    m_setPresentItemList.clear();
    m_mapTradeItemList.clear();
    m_vecRewardItemList.clear();
}

KPresentBox::~KPresentBox(void)
{
}

ImplToStringW( KPresentBox )
{
    KLocker lock( m_csPresentBox );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwVersion )
        << TOSTRINGW( m_dwPresentBoxItem )
        << TOSTRINGW( m_setPresentItemList.size() )
        << TOSTRINGW( m_mapTradeItemList.size() )
        << TOSTRINGW( m_vecRewardItemList.size() );
}

bool KPresentBox::LoadScript()
{
    return _LoadScript(std::string());
}

bool KPresentBox::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    GCITEMID       dwItemID;
    DWORD       dwVersion;
    std::set<GCITEMID>                     setPresentItemList; // 선물상자 조각 아이템 리스트.
    std::map< DWORD, VEC_REWARD >       mapTradeItemList; // 조각 아이템 교환 리스트.
    VEC_REWARD vecRewardItemList; // 보상 아이템 리스트.
    Kairo kKairo;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitPresentBox.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "PresentBoxVersion", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PresentBoxItemID", dwItemID ) == S_OK, return false );
    _JIF( LoadPresentBox( kLuaMng, std::string("PresentBoxItemList"), setPresentItemList ), return false );
    _JIF( LoadTradeItemList( kLuaMng, std::string("TradeItemList"), mapTradeItemList ), return false );
    _JIF( LoadRewardItemList( kLuaMng, std::string("RewardItemList"), vecRewardItemList, kKairo ), return false );

    START_LOG( cerr, L"선물상자 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csPresentBox );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwVersion = dwVersion;
        m_dwPresentBoxItem = dwItemID; 
        m_setPresentItemList.swap( setPresentItemList );
        m_mapTradeItemList.swap( mapTradeItemList );
        m_vecRewardItemList.swap( vecRewardItemList );
        m_kairoReward = kKairo;

        return true;
    }
}

bool KPresentBox::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KPresentBox::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPresentBox::LoadPresentBox( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setPresentBoxInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        GCITEMID dwData;
        if( kLuaMng_.GetValue( i++, dwData ) != S_OK ) break;
        setPresentBoxInfo_.insert( dwData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPresentBox::LoadTradeItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD, VEC_REWARD>& mapTradeList_ )
{
    mapTradeList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;

        GCITEMID dwItemID = 0;
        VEC_REWARD vecItems;
        _JIF( kLuaMng_.GetValue( "ItemID" , dwItemID ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("TradeItem"), vecItems ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapTradeList_.insert( std::make_pair( dwItemID, vecItems) ).second )
        {
            START_LOG( cerr, L"중복된 키가 있음.. Key : " << dwItemID ) << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPresentBox::LoadRewardItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardList_, OUT Kairo& kairo_ )
{
    vecRewardList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        float fProb = 0.f;
        KDropItemInfo kData;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
        kData.m_ItemID = itemID;
        kData.m_nDuration = nDuration;
        kData.m_nPeriod = nPeriod;
        vecRewardList_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPresentBox::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csPresentBox );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KPresentBox::GetTradeItem( IN const GCITEMID dwItemID_, OUT VEC_REWARD& vecTradeInfo_ )
{
    vecTradeInfo_.clear();
    std::map< DWORD, VEC_REWARD >::iterator mit;
    KLocker lock( m_csPresentBox );
    mit = m_mapTradeItemList.find( dwItemID_ );
    _JIF( mit != m_mapTradeItemList.end(), return false );
    vecTradeInfo_ = mit->second;
    return true;
}

void KPresentBox::GetPresentBoxItemList( OUT std::set<GCITEMID>& setItemList_ )
{
    KLocker lock( m_csPresentBox );
    setItemList_ = m_setPresentItemList;
}

void KPresentBox::GetRewardItemList( OUT VEC_REWARD& vecRewardInfo_ )
{
    KLocker lock( m_csPresentBox );
    vecRewardInfo_ = m_vecRewardItemList;
}

bool KPresentBox::DoGamble( OUT KDropItemInfo& kItem_ )
{
    bool bSuccess;
    {
        KLocker lock( m_csPresentBox );
        bSuccess = m_kairoReward.Do( kItem_ );
    }
    if( bSuccess ) return true;

    if( kItem_.m_ItemID == KLottery::CASE_BLANK )
    {
        SET_ERROR( ERR_PRESENTBOX_04 );
        return false;
    }

    SET_ERROR( ERR_PRESENTBOX_05 );
    return false;
}

void KPresentBox::GetTradeItemList( OUT std::map<DWORD, VEC_REWARD>& mapTradeList_ )
{
    KLocker lock( m_csPresentBox );
    mapTradeList_ = m_mapTradeItemList;
}