#include "CoupleSystem.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "NetError.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCoupleSystem );
NiImplementRootRTTI( KCoupleSystem );
ImplOstmOperatorW2A( KCoupleSystem );

ImplToStringW( KCoupleSystem )
{
    KLocker lock( m_csMemorial );
    START_TOSTRINGW
        << TOSTRINGW( m_kMemorialInfo.m_ExpRatio )
        << TOSTRINGW( m_kMemorialInfo.m_GpRatio )
        << TOSTRINGW( m_kMemorialInfo.m_SpxRatio )
        << TOSTRINGW( m_kMemorialInfo.m_nMemorialDateGap )
        << TOSTRINGW( m_kMemorialInfo.m_nMemorialMonthGap )
        << TOSTRINGW( m_kMemorialInfo.m_nMemorialYearGap );

    return stm_;
}

KCoupleSystem::KCoupleSystem(void)
{
    m_kMemorialInfo.Clear();
}

KCoupleSystem::~KCoupleSystem(void)
{
}

bool KCoupleSystem::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCoupleSystem::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitCoupleInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadCoupleItems( kLuaMng ), return false );
    _JIF( LoadCoupleBenefit( kLuaMng ), return false );
    _JIF( LoadMemorialDay( kLuaMng ), return false );
    _JIF( LoadCoupleSignBoard( kLuaMng ), return false );
    return true;
}

bool KCoupleSystem::LoadCoupleItems( KLuaManager& kLuaMng_ )
{
    std::map<DWORD,KCoupleEquip> mapCoupleItems;
    _JIF( kLuaMng_.BeginTable( "CoupleItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        GCITEMID nItemID;
        int nEquipPos;
        _JIF( kLuaMng_.GetValue( 1, nItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nEquipPos ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        KCoupleEquip kEquip;
        kEquip.m_dwItemID   = nItemID;
        kEquip.m_cEquipSlot = (char)(nEquipPos);

        if( !mapCoupleItems.insert( std::make_pair( kEquip.m_dwItemID, kEquip) ).second )
        {
            START_LOG( cerr, L"커플 아이템중 중복된 아이템이 존재. ItemID : " << nItemID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"커플 아이템 읽기 완료. Size : " << mapCoupleItems.size() ) << END_LOG;
    KLocker lock( m_csCouple );
    m_mapCoupleItems.swap( mapCoupleItems );
    return true;
}

bool KCoupleSystem::LoadCoupleBenefit( KLuaManager& kLuaMng_ )
{
    std::map<DWORD,SCoupleBenefit > mapBenefitItems;
    _JIF( kLuaMng_.BeginTable( "CoupleBenefit" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        SCoupleBenefit kBenefit;
        GCITEMID nItemID;
        _JIF( kLuaMng_.GetValue( 1, nItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kBenefit.m_fExpRatio ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kBenefit.m_fGPRatio ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kBenefit.m_fSPXRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        kBenefit.m_dwItemID = nItemID;
 
        if( !mapBenefitItems.insert( std::make_pair( kBenefit.m_dwItemID, kBenefit) ).second )
        {
            START_LOG( cerr, L"커플 효과 아이템중 중복된 아이템이 존재. ItemID : " << nItemID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"커플 효과 아이템 읽기 완료. Size : " << mapBenefitItems.size() ) << END_LOG;
    KLocker lock( m_csCouple );
    m_mapBenefitItems.swap( mapBenefitItems );
    return true;
}

bool KCoupleSystem::LoadMemorialDay( KLuaManager& kLuaMng_ )
{
    SMemorialDayInfo kMemorialInfo;
    _JIF( kLuaMng_.BeginTable( "CoupleMemorialDay" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "Date_Gap", kMemorialInfo.m_nMemorialDateGap ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Month_Gap", kMemorialInfo.m_nMemorialMonthGap ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Year_Gap", kMemorialInfo.m_nMemorialYearGap ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "Benefit" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, kMemorialInfo.m_ExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kMemorialInfo.m_GpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kMemorialInfo.m_SpxRatio ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" CoupleMemorialDay 로드 완료.." )
        << BUILD_LOG( kMemorialInfo.m_nMemorialDateGap )
        << BUILD_LOG( kMemorialInfo.m_nMemorialMonthGap )
        << BUILD_LOG( kMemorialInfo.m_nMemorialYearGap )
        << BUILD_LOG( kMemorialInfo.m_ExpRatio )
        << BUILD_LOG( kMemorialInfo.m_GpRatio )
        << BUILD_LOG( kMemorialInfo.m_SpxRatio ) << END_LOG;

    KLocker lock( m_csMemorial );
    m_kMemorialInfo = kMemorialInfo;
    return true;
}

bool KCoupleSystem::LoadCoupleSignBoard( KLuaManager& kLuaMng_ )
{
    std::map<DWORD, SCoupleSignBoard>   mapCoupleSignBoard;
    _JIF( kLuaMng_.BeginTable( "CoupleSquareSignBoard" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        SCoupleSignBoard sCoupleSignBoard;
        _JIF( kLuaMng_.GetValue( 1, sCoupleSignBoard.m_dwRingID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, sCoupleSignBoard.m_dwSignBoardID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, sCoupleSignBoard.m_dwFontColor ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, sCoupleSignBoard.m_nType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, sCoupleSignBoard.m_bEnableBlock ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );


        if( !mapCoupleSignBoard.insert( std::make_pair( sCoupleSignBoard.m_dwRingID, sCoupleSignBoard) ).second )
        {
            START_LOG( cerr, L"커플 전광판 정보가 중복 되었음. ItemID : " << sCoupleSignBoard.m_dwRingID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"커플 전광판 정보 읽기 완료. Size : " << mapCoupleSignBoard.size() ) << END_LOG;
    KLocker lock( m_csCoupleSignBoard );
    m_mapCoupleSignBoard.swap( mapCoupleSignBoard );
    return true;
}

bool KCoupleSystem::IsCoupleRequestable( DWORD dwState )
{
    if( CS_NONE == dwState ) return true;

    switch( dwState )
    {
    case CS_NONE: return true;
    case CS_NOTLOADED:      SET_ERROR( ERR_COUPLE_08 ); break;
    case CS_COUPLE:         SET_ERROR( ERR_COUPLE_09 ); break;case CS_REQUEST:        SET_ERROR( ERR_COUPLE_10 ); break;
    case CS_REQUEST_FROM:   SET_ERROR( ERR_COUPLE_11 ); break;
    case CS_DENY:           SET_ERROR( ERR_COUPLE_12 ); break;
    default:
         SET_ERROR( ERR_COUPLE_13 ); break;
    }
    return false;
}

bool KCoupleSystem::GetCoupleItem( IN GCITEMID dwItemID_, OUT KCoupleEquip& kItem_ )
{
    std::map<DWORD,KCoupleEquip>::iterator mit;
    KLocker lock( m_csCouple );
    mit = m_mapCoupleItems.find( dwItemID_ );
    if( mit == m_mapCoupleItems.end() ) return false;

    kItem_ = mit->second;
    return true;
}

bool KCoupleSystem::IsRingItem( GCITEMID dwItemID_ )
{
    KCoupleEquip kItem;
    if( !GetCoupleItem( dwItemID_, kItem ) ) return false;

    if( kItem.m_cEquipSlot != 0 ) return false; // 슬롯 포지션이 반지가 아니다.

    return true;
}

bool KCoupleSystem::IsInReqInfo( std::map<DWORD,SRequestInfo >& mpaReqData_, DWORD dwUID_ )
{
    return (mpaReqData_.find( dwUID_ ) != mpaReqData_.end());
}

bool KCoupleSystem::AddRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData_, SRequestInfo& reqData_ )
{
    if( IsInReqInfo( mpaReqData_, reqData_.m_dwRequester ) || IsInReqInfo( mpaReqData_, reqData_.m_dwRequestee ) )
    {
        return false;
    }

    reqData_.m_nState = MCS_REQ;
    mpaReqData_.insert( std::make_pair( reqData_.m_dwRequester, reqData_ ) );
    mpaReqData_.insert( std::make_pair( reqData_.m_dwRequestee, reqData_ ) );
    return true;
}

bool KCoupleSystem::GetRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData_, DWORD dwUID_, OUT SRequestInfo& sReqInfo_ )
{
    std::map<DWORD,SRequestInfo >::iterator mit;

    mit = mpaReqData_.find( dwUID_ );
    if( mit == mpaReqData_.end() ) return false;
    sReqInfo_ = mit->second;
    return true;
}

void KCoupleSystem::DeleteRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData_, DWORD dwRequester_, DWORD dwRequestee_ )
{
    mpaReqData_.erase( dwRequester_ );
    mpaReqData_.erase( dwRequestee_ );
}

bool KCoupleSystem::UpdateReqInfoState( std::map<DWORD,SRequestInfo >& mpaReqData_, DWORD dwUID_, int nState_ )
{
    std::map<DWORD,SRequestInfo >::iterator mit;
    std::map<DWORD,SRequestInfo >::iterator mit2;
    DWORD dwUID2;

    mit = mpaReqData_.find( dwUID_ );
    if( mit == mpaReqData_.end() ) return false;

    dwUID2 = (mit->second.m_dwRequester == dwUID_? mit->second.m_dwRequestee : mit->second.m_dwRequester);
    mit2 = mpaReqData_.find( dwUID2 );
    if( mit2 == mpaReqData_.end() ) return false;

    mit->second.m_nState = nState_;
    mit2->second.m_nState = nState_;
    return true;    
}

int KCoupleSystem::GetReqInfoState( std::map<DWORD,SRequestInfo >& mpaReqData_, DWORD dwUID_ )
{
    std::map<DWORD,SRequestInfo >::iterator mit;
    mit = mpaReqData_.find( dwUID_ );
    if( mit == mpaReqData_.end() ) return -1;

    return mit->second.m_nState;
}

void KCoupleSystem::SwapCoupleInfo( IN OUT KCoupleInfo& kInfo_ )
{
    std::swap( kInfo_.m_dwUID, kInfo_.m_dwPartnerUID );
    std::swap( kInfo_.m_cCharType, kInfo_.m_cPartnerCharType );
    std::swap( kInfo_.m_tmRegDate, kInfo_.m_tmPartnerRegDate );
    std::swap( kInfo_.m_vecMyEquip, kInfo_.m_vecPartnerEquip );
    std::swap( kInfo_.m_vecMyInven, kInfo_.m_vecPartnerInven );
}

bool KCoupleSystem::GetBenefitInfo( IN GCITEMID dwItemID_, OUT SCoupleBenefit& kInfo_ )
{
    KLocker lock( m_csCouple );
    std::map<DWORD,SCoupleBenefit >::iterator mit;
    mit = m_mapBenefitItems.find( dwItemID_ );
    if( mit == m_mapBenefitItems.end() ) return false;
    kInfo_ = mit->second;
    return true;
}


bool KCoupleSystem::AddCoupleRequestInfo( DWORD dwRequester_, DWORD dwRequestee_, IN KSimpleItem& kItem_ )
{
    SRequestInfo data;
    data.m_dwRequester  = dwRequester_;
    data.m_dwRequestee  = dwRequestee_;
    data.m_dwItemID     = kItem_.m_dwID;
    data.m_dwItemUID    = kItem_.m_dwUID;
    data.m_bInCoupleInven =  false;

    KLocker lock( m_csRequest );
    return AddRequestInfo( m_mapCoupleRequestInfo, data );
}
bool KCoupleSystem::GetCoupleRequestInfo( DWORD dwUID_, OUT SRequestInfo& sReqInfo_ )
{
    KLocker lock( m_csRequest );
    return GetRequestInfo( m_mapCoupleRequestInfo, dwUID_, sReqInfo_ );
}

void KCoupleSystem::DeleteCoupleRequestInfo( DWORD dwRequester_, DWORD dwRequestee_ )
{
    KLocker lock( m_csRequest );
    DeleteRequestInfo( m_mapCoupleRequestInfo, dwRequester_, dwRequestee_ );
}

bool KCoupleSystem::IsInCoupleReqInfo( DWORD dwUID_ )
{
    KLocker lock( m_csRequest );
    return IsInReqInfo( m_mapCoupleRequestInfo, dwUID_ );
}

bool KCoupleSystem::UpdateCoupleReqInfoState( DWORD dwUID_, int nState_ )
{
    KLocker lock( m_csRequest );
    return UpdateReqInfoState( m_mapCoupleRequestInfo, dwUID_, nState_ );
}

int KCoupleSystem::GetCoupleReqInfoState( DWORD dwUID_ )
{
    KLocker lock( m_csRequest );
    return GetReqInfoState( m_mapCoupleRequestInfo, dwUID_ );
}


bool KCoupleSystem::AddRingRequestInfo( SRequestInfo& reqData_ )
{
    KLocker lock( m_csRequest );
    return AddRequestInfo( m_mapRingRequestInfo, reqData_ );
}
bool KCoupleSystem::GetRingRequestInfo( DWORD dwUID_, OUT SRequestInfo& sReqInfo_ )
{
    KLocker lock( m_csRequest );
    return GetRequestInfo( m_mapRingRequestInfo, dwUID_, sReqInfo_ );
}

void KCoupleSystem::DeleteRingRequestInfo( DWORD dwRequester_, DWORD dwRequestee_ )
{
    KLocker lock( m_csRequest );
    DeleteRequestInfo( m_mapRingRequestInfo, dwRequester_, dwRequestee_ );
}

bool KCoupleSystem::IsInRingReqInfo( DWORD dwUID_ )
{
    KLocker lock( m_csRequest );
    return IsInReqInfo( m_mapRingRequestInfo, dwUID_ );
}

bool KCoupleSystem::UpdateRingReqInfo( DWORD dwUID_, int nState_ )
{
    KLocker lock( m_csRequest );
    return UpdateReqInfoState( m_mapRingRequestInfo, dwUID_, nState_ );
}

int KCoupleSystem::GetRingReqInfo( DWORD dwUID_ )
{
    KLocker lock( m_csRequest );
    return GetReqInfoState( m_mapRingRequestInfo, dwUID_ );
}

bool KCoupleSystem::ChkEquipEqual( std::vector<KCoupleEquip>& vecEquip1_, std::vector<KCoupleEquip>& vecEquip2_ )
{
    if( vecEquip2_.size() != vecEquip1_.size() ) return false;
    std::vector<KCoupleEquip>::iterator vit;
    for( vit = vecEquip1_.begin() ; vit != vecEquip1_.end() ; ++vit )
    {
        std::vector<KCoupleEquip>::iterator vit1;
        vit1 = std::find_if( vecEquip2_.begin(), vecEquip2_.end(), boost::bind( &KCoupleEquip::m_dwItemID, _1 ) == vit->m_dwItemID );
        if( vit1 == vecEquip2_.end() )
        {
            return false;
        }
    }
    return true;
}

void KCoupleSystem::GetEquipList( std::vector<KCoupleEquip>& vecFullEquip_, std::vector<DWORD>& vecSimpleEquip_ )
{
    vecSimpleEquip_.clear();
    if( vecFullEquip_.empty() ) return;
    vecSimpleEquip_.reserve( vecFullEquip_.size() );
    std::vector<KCoupleEquip>::iterator vit;
    for( vit = vecFullEquip_.begin() ; vit != vecFullEquip_.end() ; ++vit )
    {
        vecSimpleEquip_.push_back( vit->m_dwItemID );
    }
}

bool KCoupleSystem::IsCoupleItem( GCITEMID itemID_ )
{
    KLocker lock( m_csCouple );
    return (m_mapCoupleItems.find( itemID_ ) != m_mapCoupleItems.end());
}

void KCoupleSystem::GetRegDate( CTime& tmOut, time_t tmRegDate_ )
{
    CTime tmTmp( tmRegDate_ );
    CTime tmRegDate( tmTmp.GetYear(), tmTmp.GetMonth(), tmTmp.GetDay(), 0, 0, 0 );
    tmOut = tmRegDate;
}
void KCoupleSystem::GetCurrentDate( CTime& tmCurrentDate_ )
{
    CTime tmTmp = CTime::GetCurrentTime();
    CTime tmCurrentDate( tmTmp.GetYear(), tmTmp.GetMonth(), tmTmp.GetDay(), 0, 0, 0 );
    tmCurrentDate_ = tmCurrentDate;
}

bool KCoupleSystem::IsMemorialDay( time_t tmRegDate_ )
{
    // 년 월 일 만 가지고 처리 해야 한다.. 시분초는 빼야 한다.
    CTime tmRegDate;
    CTime tmCurrent;
    if( !CheckValidValue( tmRegDate_ ) ) {
        START_LOG( cerr, L"잘못된 time_t 변수값" )
            << END_LOG;
        return false;
    }

    GetRegDate( tmRegDate, tmRegDate_ );
    GetCurrentDate( tmCurrent );

    CTimeSpan tmSpan = tmCurrent - tmRegDate;

    KLocker lock( m_csMemorial );
    if( m_kMemorialInfo.m_nMemorialDateGap > 0 &&
        ((int)tmSpan.GetDays() + 1) % m_kMemorialInfo.m_nMemorialDateGap == 0 )
    {
        return true;
    }

    if( m_kMemorialInfo.m_nMemorialMonthGap > 0 &&
        tmRegDate.GetDay() == tmCurrent.GetDay() &&
        ( tmRegDate.GetMonth() != tmCurrent.GetMonth() || tmRegDate.GetYear() != tmCurrent.GetYear() ) )
    {
        return true;
    }

    if( m_kMemorialInfo.m_nMemorialYearGap > 0 &&
        tmRegDate.GetDay() == tmCurrent.GetDay() &&
        tmRegDate.GetMonth() == tmCurrent.GetMonth() &&
        tmRegDate.GetYear() != tmCurrent.GetYear() )
    {
        return true;
    }

    return false;
}

float KCoupleSystem::GetMemorialExp()
{
    KLocker lock( m_csMemorial );
    return m_kMemorialInfo.m_ExpRatio;
}

float KCoupleSystem::GetMemorialGp()
{
    KLocker lock( m_csMemorial );
    return m_kMemorialInfo.m_GpRatio;
}

float KCoupleSystem::GetMemorialSpx()
{
    KLocker lock( m_csMemorial );
    return m_kMemorialInfo.m_SpxRatio;
}

bool KCoupleSystem::GetCoupleSignBoard( DWORD dwRingID_, SCoupleSignBoard& signBoard_ )
{
    std::map<DWORD, SCoupleSignBoard>::iterator mit;
    KLocker lock( m_csCoupleSignBoard );
    mit = m_mapCoupleSignBoard.find( dwRingID_ );
    if( mit == m_mapCoupleSignBoard.end() )
    {
        START_LOG( cwarn, L"해당 커플링에 대한 싸인보드 정보가 없음. RingID : " << dwRingID_ ) << END_LOG;
        return false;
    }

    signBoard_ = mit->second;
    return true;
}

time_t KCoupleSystem::GetCoupleRegDate( IN const KCoupleInfo& kCoupleInfo_ )
{
    return ( kCoupleInfo_.m_tmRegDate > kCoupleInfo_.m_tmPartnerRegDate ? kCoupleInfo_.m_tmPartnerRegDate : kCoupleInfo_.m_tmRegDate );
}

bool KCoupleSystem::CheckValidValue( IN const time_t& tmDate_ )
{
    if( tmDate_ < TIME_T_MINIMUM ) {
        return false;
    }
    return true;
}