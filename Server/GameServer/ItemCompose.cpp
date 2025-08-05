#include "ItemCompose.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"

ImplementSingleton( KItemCompose );
ImplOstmOperatorW2A( KItemCompose );
NiImplementRootRTTI( KItemCompose );

KItemCompose::KItemCompose(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwMaxMaterialListNum(0)
,m_dwVersion(0)
{
    m_vecMaterialList.clear();
    m_vecRewardList.clear();
}

KItemCompose::~KItemCompose(void)
{
}

ImplToStringW( KItemCompose )
{
    KLocker lock( m_csItemCompose );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwVersion )
        << TOSTRINGW( m_dwMaxMaterialListNum )
        << TOSTRINGW( m_vecMaterialList.size() )
        << TOSTRINGW( m_vecRewardList.size() );
}


bool KItemCompose::LoadScript()
{
    return _LoadScript(std::string());
}

bool KItemCompose::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    KLuaManager kLuaMng;
    DWORD       dwMaxMaterialListNum;
    DWORD       dwVersion;
    std::vector<GCITEMID> vecMaterialList; // 아이템 재료 정보.
    std::vector<GCITEMID> vecRewardList;   // 보상 아이템 정보.
    Kairo       kKairo;

    const std::string strScriptName = "InitItemCompose.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( kLuaMng.GetValue( "Version", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxMaterialListNum", dwMaxMaterialListNum ) == S_OK, return false );
    // 재료 아이템 리스트 읽어오기.
    _JIF( LoadMaterialInfo( kLuaMng, std::string("MaterialList"), vecMaterialList ), return false );
    // 보상 아이템 확률 설정.
    _JIF( LoadRewardRatio( kLuaMng, std::string("SuccessRewardList"), kKairo, vecRewardList ), return false );

    START_LOG( cerr, L"아이템 합성 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csItemCompose );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwVersion = dwVersion;
        m_dwMaxMaterialListNum = dwMaxMaterialListNum;
        m_vecMaterialList.swap( vecMaterialList );
        m_vecRewardList.swap( vecRewardList );
        m_kairoInfo = kKairo;

        return true;
    }
}

bool KItemCompose::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KItemCompose::LoadMaterialInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<GCITEMID>& vecInfo_ )
{
    vecInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        vecInfo_.push_back( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KItemCompose::LoadRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_, OUT std::vector<GCITEMID>& vecInfo_ )
{
    vecInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
        vecInfo_.push_back( itemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KItemCompose::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csItemCompose );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KItemCompose::CheckMaterialInfo( IN const GCITEMID& dwItemID_ )
{
    std::vector<GCITEMID>::iterator vit;

    KLocker lock( m_csItemCompose );
    vit = std::find( m_vecMaterialList.begin(), m_vecMaterialList.end(), dwItemID_ );
    if( vit == m_vecMaterialList.end() ) {
        return false;
    }
    return true;
}

bool KItemCompose::DoGamble( OUT KDropItemInfo& kDropItemInfo_ )
{
    bool bSuccess;
    {
        KLocker lock( m_csItemCompose );
        bSuccess = m_kairoInfo.Do( kDropItemInfo_ );
    }
    if( !bSuccess ) {
        return false;
    }

    if( kDropItemInfo_.m_ItemID == 0 ) {
        return false;
    }

    return true;
}

void KItemCompose::GetItemComposeInfo( OUT KEVENT_ITEM_COMPOSE_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csItemCompose );
    kPacket_.m_dwMaxMaterialListNum = m_dwMaxMaterialListNum;
    kPacket_.m_vecMaterialList = m_vecMaterialList;
    kPacket_.m_vecRewardList = m_vecRewardList;
}