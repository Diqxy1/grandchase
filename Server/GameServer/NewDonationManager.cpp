#include "CnConnector.h" // winsock
#include "NewDonationManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "userevent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KNewDonationManager );
ImplOstmOperatorW2A( KNewDonationManager );
NiImplementRootRTTI( KNewDonationManager );

KNewDonationManager::KNewDonationManager(void)
:m_tmBegin(0)
,m_tmEnd(0)
,m_dwDonationItemId(0)
,m_dwTickGap(60*1000)
,m_dwMinType(0)
,m_dwMaxType(0)
{
    m_mapTotalDonation.clear();
    m_mapDeltaDonation.clear();
    m_mapNPCInfo.clear();
    m_dwLastTick = ::GetTickCount();
}

KNewDonationManager::~KNewDonationManager(void)
{
}

ImplToStringW( KNewDonationManager )
{
    KLocker lock( m_csNewDonation );
    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwMinType )
        << TOSTRINGW( m_dwMaxType )
        << TOSTRINGW( m_dwDonationItemId );
    DumpCurrentDonationState( stm_ );
    return stm_;
}

bool KNewDonationManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KNewDonationManager::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwTickGap;
    GCITEMID       dwDonationItemID;
    DWORD       dwMinType;
    DWORD       dwMaxType;
    std::map<DWORD,std::vector<std::pair<DWORD,int>>> mapNPCInfo;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitNewDonationInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DonationItemID", dwDonationItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MinTypeNum", dwMinType ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxTypeNum", dwMaxType ) == S_OK, return false );

    _JIF( LoadNPCInfo( kLuaMng, mapNPCInfo ), return false );

    START_LOG( cerr, L"신년 기부 이벤트 로드 완료." ) << END_LOG;

    KLocker lock( m_csNewDonation );
    m_tmBegin = tmBegin;
    m_tmEnd   = tmEnd;
    m_dwTickGap = dwTickGap;
    m_dwDonationItemId = dwDonationItemID;
    m_dwMinType = dwMinType;
    m_dwMaxType = dwMaxType;
    m_mapNPCInfo.swap( mapNPCInfo );

    return true;
}

bool KNewDonationManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KNewDonationManager::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<std::pair<DWORD,int> >& vecInfos_ )
{
    vecInfos_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        std::pair<DWORD, int> prData;
        if( kLuaMng_.GetValue( i++, prData.first ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, prData.second ) == S_OK, return false );
        vecInfos_.push_back( prData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KNewDonationManager::LoadNPCInfo( IN KLuaManager& kLuaMng_, OUT std::map<DWORD, std::vector<std::pair<DWORD, int> > >& mapNPCInfo_ )
{
    DWORD dwType = 0;
    mapNPCInfo_.clear();
   _JIF( kLuaMng_.BeginTable( "NPCInfo" ) == S_OK, return false );

   for( int i = 1 ; ; ++i )
   {
       if( kLuaMng_.BeginTable( i ) != S_OK ) break;
       int nChar = -1;
       std::vector<std::pair<DWORD,int>> vecInfo;
       _JIF( kLuaMng_.GetValue( "Type", dwType ) == S_OK, return false );
       _JIF( LoadItems( kLuaMng_, std::string("GoalInfo"), vecInfo ), return false );

       mapNPCInfo_[dwType] = vecInfo;

       _JIF( kLuaMng_.EndTable() == S_OK, return false );
   }
   _JIF( kLuaMng_.EndTable() == S_OK, return false );

   return true;
}

bool KNewDonationManager::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csNewDonation );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KNewDonationManager::InitProcess()
{
    SendDeltaDonationToCenter();
}

void KNewDonationManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - m_dwLastTick < m_dwTickGap ) return;
    
    m_dwLastTick = dwCurrentTick;

    // 이벤트 기간이 아니면 그냥 넘어간다.
    if( !IsRun() ) return;

    // 센터서버와의 동기화.
    SendDeltaDonationToCenter();
}

void KNewDonationManager::UpdateTotalDonationFromCenter( IN std::map<DWORD, int> mapTotalDonation_ )
{
    KLocker lock( m_csNewDonation );
    m_mapTotalDonation = mapTotalDonation_;
}

bool KNewDonationManager::SendDeltaDonationToCenter()
{
    std::map<DWORD, int> mapDonationInfo;
    mapDonationInfo.clear();

    KLocker lock( m_csNewDonation );
    mapDonationInfo.swap( m_mapDeltaDonation );           // 차이값에 대해서만 전송한다.

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_NEW_DONATION_REQ, mapDonationInfo ) );

    return false;
}

void KNewDonationManager::GetTypeTotalDonation( IN DWORD dwType_, OUT int& nTotalDonation_ )
{
    KLocker lock( m_csNewDonation );
    std::map<DWORD,int>::iterator mit;
    mit = m_mapTotalDonation.find( dwType_ );
    if( mit == m_mapTotalDonation.end() ) {
        nTotalDonation_ = 0;
    }
    else {
        nTotalDonation_ = mit->second;
    }
}

void KNewDonationManager::AddDeltaDonationPoint( IN DWORD dwType_, IN int nDeltaPoint_ )
{
    KLocker lock( m_csNewDonation );
    // 우선 Center에 더해줄 데이터에 대해서만 더해주자.
    std::map<DWORD, int>::iterator mit;
    mit = m_mapDeltaDonation.find( dwType_ );
    if( mit == m_mapDeltaDonation.end() ) {
        m_mapDeltaDonation[dwType_] = nDeltaPoint_;
    }
    else {
        mit->second += nDeltaPoint_;
    }

    // 전체 정보에 대해서 한번더 저장하자.
    std::map<DWORD, int>::iterator mit1;
    mit1 = m_mapTotalDonation.find( dwType_ );
    if( mit1 == m_mapTotalDonation.end() ) {
        m_mapTotalDonation[dwType_] = nDeltaPoint_;
    }
    else {
        mit1->second += nDeltaPoint_;
    }
}

void KNewDonationManager::GetNPCInfo( OUT std::map<DWORD, std::vector<std::pair<DWORD, int> > >& mapNPCInfo_ )
{
    KLocker lock( m_csNewDonation );
    mapNPCInfo_ = m_mapNPCInfo;
}

bool KNewDonationManager::CheckDonationItemID( IN GCITEMID dwItemID_ )
{
    KLocker lock( m_csNewDonation );
    if( m_dwDonationItemId == dwItemID_ ) return true;
    return false;
}

void KNewDonationManager::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock( m_csNewDonation );
    tmDate_ = m_tmBegin;
}

void KNewDonationManager::GetMinMaxType( OUT DWORD& dwMinType_, OUT DWORD& dwMaxType_ )
{
    KLocker lock( m_csNewDonation );
    dwMinType_ = m_dwMinType;
    dwMaxType_ = m_dwMaxType;
}

void KNewDonationManager::DumpCurrentDonationState( std::wostream& stm_ ) const 
{
    stm_ << L" --- NPC Donation Info ---- " << std::endl;
    std::map<DWORD, int>::const_iterator mit;
    for( mit = m_mapTotalDonation.begin(); mit != m_mapTotalDonation.end(); ++mit ) {
        stm_ << L"[" << mit->first
            << L", " << mit->second
            << L"]" << std::endl;
    }
}

void KNewDonationManager::GetDonationItemID ( OUT GCITEMID& dwItemID_ )
{
    KLocker lock( m_csNewDonation );
    dwItemID_ = m_dwDonationItemId;
}