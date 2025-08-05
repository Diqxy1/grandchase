#include "MagicBox.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"

ImplementSingleton( KMagicBox );
ImplOstmOperatorW2A( KMagicBox );
NiImplementRootRTTI( KMagicBox );

KMagicBox::KMagicBox(void)
:m_bEnable(false)
,m_usVersion(0)
,m_NormalKeyItemID(0)
,m_SpecialKeyItemID(0)
,m_fIncRate(0.f)
,m_tmBegin(1)
,m_tmEnd(1)
,m_fLuckRatioDBtoServer(0.f)
,m_usDecKeyCount(1)
{
    m_mapItemList.clear();
    m_mapLuckRatio.clear();
}

KMagicBox::~KMagicBox(void)
{
}

ImplToStringW( KMagicBox )
{
    KLocker lock(m_csMagicBox);
    return START_TOSTRINGW
        << TOSTRINGWb( m_bEnable )
        << TOSTRINGW( m_usVersion )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_NormalKeyItemID )
        << TOSTRINGW( m_SpecialKeyItemID )
        << TOSTRINGW( m_fIncRate )
        << TOSTRINGW( m_fLuckRatioDBtoServer )
        << TOSTRINGW( m_usDecKeyCount )
        << TOSTRINGW( m_mapItemList.size() )
        << TOSTRINGW( m_mapLuckRatio.size() );
}

bool KMagicBox::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMagicBox::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    bool bEnable = false;
    USHORT usVersion = 0;
    time_t tmBegin;
    time_t tmEnd;
    GCITEMID NormalKeyItemID = 0;
    GCITEMID SpecialKeyItemID = 0;
    float fIncRate = 0.f;
    float fLuckRatioDBtoServer = 1.f;
    USHORT usDecKeyCount = 1;

    const std::string strScriptName = "InitMagicBox.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Enable", bEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Version", usVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "NormalKeyItemID", NormalKeyItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SpecialKeyItemID", SpecialKeyItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "IncRate", fIncRate ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LuckRatioDBtoServer", fLuckRatioDBtoServer ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DecKeyCount", usDecKeyCount ) == S_OK, return false );

    START_LOG( cerr, L"Load MagicBox Script.." )
        << BUILD_LOG( bEnable )
        << BUILD_LOG( usVersion )
        << BUILD_LOG( NormalKeyItemID )
        << BUILD_LOG( SpecialKeyItemID )
        << BUILD_LOG( fIncRate )
        << BUILD_LOG( fLuckRatioDBtoServer )
        << BUILD_LOG( usDecKeyCount )
        << END_LOG;

    {
        KLocker lock(m_csMagicBox);
        m_bEnable = bEnable;
        m_usVersion = usVersion;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_NormalKeyItemID = NormalKeyItemID;
        m_SpecialKeyItemID = SpecialKeyItemID;
        m_fIncRate = fIncRate;
        m_fLuckRatioDBtoServer = fLuckRatioDBtoServer;
        m_usDecKeyCount = usDecKeyCount;
    }

    SendMagicBoxListNotToDB(); // 서버 켜진 상태에서 스크립트 수정했을 때 , 아이템 리스트를 DB 로 요청해야 함.
    return true;
}

bool KMagicBox::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    nHour = 0;
    nMin = 0;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    kLuaMng_.GetValue( 4, nHour );
    kLuaMng_.GetValue( 5, nMin );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, nMin, 0 ) );
    return true;
}

void KMagicBox::UpdateEvent( IN std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapItemList_, IN std::map<GCITEMID,float>& mapLuckRatio_ )
{
    START_LOG( cerr, L"Update MagicBox Event from DB.." )
        << BUILD_LOG( mapItemList_.size() )
        << BUILD_LOG( mapLuckRatio_.size() ) << END_LOG;

    KLocker lock(m_csMagicBox);

    m_mapItemList.swap( mapItemList_ );
    m_mapLuckRatio.swap( mapLuckRatio_ );
}

GCITEMID KMagicBox::GetNormalKeyItemID()
{
    KLocker lock(m_csMagicBox);
    return m_NormalKeyItemID;
}

GCITEMID KMagicBox::GetSpecialKeyItemID()
{
    KLocker lock(m_csMagicBox);
    return m_SpecialKeyItemID;
}

float KMagicBox::CalcNeedKeyRatio( IN const int& nClearCount_ )
{
    // 키 필요수량 배율 = 1.f + ( 아이템 힉득횟수 * m_fIncRate )
    float fNeedKeyRatio = 1.f;

    if ( nClearCount_ <= 0 ) {
        return fNeedKeyRatio;
    }

    KLocker lock(m_csMagicBox);
    fNeedKeyRatio += nClearCount_ * m_fIncRate;
    return fNeedKeyRatio;
}

void KMagicBox::GetItemList( OUT std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapList_ )
{
    KLocker lock(m_csMagicBox);
    mapList_ = m_mapItemList;
}

bool KMagicBox::GetItemInfo( IN const GCITEMID& ItemID_, OUT KDropItemInfo& kItem_, OUT int& nNeedKeyCount_ )
{
    KLocker lock(m_csMagicBox);

    std::map<GCITEMID,std::pair<KDropItemInfo,int> >::const_iterator cmit;
    cmit = m_mapItemList.find( ItemID_ );
    if ( cmit == m_mapItemList.end() ) {
        return false;
    }

    kItem_ = cmit->second.first;
    nNeedKeyCount_ = cmit->second.second;
    return true;
}

bool KMagicBox::GetItemRatio( IN const GCITEMID& ItemID_, OUT float& fRatio_ )
{
    fRatio_ = 0.f;

    KLocker lock(m_csMagicBox);

    std::map<GCITEMID,float>::const_iterator cmit;
    cmit = m_mapLuckRatio.find( ItemID_ );
    if ( cmit == m_mapLuckRatio.end() ) {
        return false;
    }

    fRatio_ = cmit->second * m_fLuckRatioDBtoServer;
    return true;
}

bool KMagicBox::IsExistItem( IN const GCITEMID& itemID_ )
{
    KLocker lock(m_csMagicBox);
    return ( m_mapItemList.find( itemID_ ) != m_mapItemList.end() );
}

bool KMagicBox::IsEventTerm()
{
    if ( false == IsEnable() ) {
        return false;
    }

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock(m_csMagicBox);
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KMagicBox::SendMagicBoxListNotToDB()
{
    if ( false == IsEventTerm() ) {
        return;
    }

    USHORT usVer = GetVer();
    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_MAGIC_BOX_LIST_NOT, L"", 0, 0, usVer );
}

USHORT KMagicBox::GetDecKeyCount()
{
    KLocker lock(m_csMagicBox);
    return m_usDecKeyCount;
}

void KMagicBox::DumpInfo()
{
    //KLocker lock(m_csMagicBox);

    START_LOG( cerr, L"--- Dump MagicBoxInfo ---" )
        << BUILD_LOG( m_bEnable )
        << BUILD_LOG( m_usVersion )
        << BUILD_LOGtm( CTime(m_tmBegin) )
        << BUILD_LOGtm( CTime(m_tmEnd) )
        << BUILD_LOG( m_NormalKeyItemID )
        << BUILD_LOG( m_SpecialKeyItemID )
        << BUILD_LOG( m_fIncRate )
        << BUILD_LOG( m_fLuckRatioDBtoServer )
        << BUILD_LOG( m_usDecKeyCount )
        << END_LOG;

    START_LOG( cerr, L"    m_mapItemList.size()" << m_mapItemList.size() ) << END_LOG;
    {
        std::map<GCITEMID, std::pair<KDropItemInfo,int> >::const_iterator cmit;
        for ( cmit = m_mapItemList.begin() ; cmit != m_mapItemList.end() ; ++cmit ) {
            START_LOG( cerr, L"        ItemID : " << cmit->second.first.m_ItemID 
                << L", Count : " << cmit->second.first.m_nDuration
                << L", Period : " << cmit->second.first.m_nPeriod
                << L", NeedKeyCount : " << cmit->second.second ) << END_LOG;
        }
    }

    START_LOG( cerr, L"    m_mapLuckRatio.size()" << m_mapLuckRatio.size() ) << END_LOG;
    {
        std::map<GCITEMID,float>::const_iterator cmit;
        for ( cmit = m_mapLuckRatio.begin() ; cmit != m_mapLuckRatio.end() ; ++cmit ) {
            START_LOG( cerr, L"        ItemID : " << cmit->first << L", LuckRatio : " << cmit->second ) << END_LOG;
        }
    }

    START_LOG( cerr, L"--- End Dump ---" ) << END_LOG;
}