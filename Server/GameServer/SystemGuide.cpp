#include "SystemGuide.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "CommonPacket.h"
#include <KncUtilLua.h> // boost::bind

ImplementSingleton( KSystemGuide );
ImplOstmOperatorW2A( KSystemGuide );
NiImplementRootRTTI( KSystemGuide );
ImplToStringW( KSystemGuide )
{
    KLocker lock( m_csSystemGuide );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapProvideItem.size() )
        << TOSTRINGW( m_mapRequirMission.size() );
}

KSystemGuide::KSystemGuide(void)
{
    m_mapProvideItem.clear();
}

KSystemGuide::~KSystemGuide(void)
{
}

bool KSystemGuide::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSystemGuide::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "GCGuide.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    std::map<int, std::map< int, VEC_REWARD > > mapProvideItem;
    std::map<int, std::vector< DWORD > > mapRequirMission;

    _JIF( LoadGuideElements( kLuaMng, std::string("GuideElements"), mapProvideItem, mapRequirMission ), return false ); 

    {
        KLocker lock( m_csSystemGuide );
        m_mapProvideItem.swap( mapProvideItem );
        m_mapRequirMission.swap( mapRequirMission );
    }

    START_LOG( cerr, L"시스템 가이드 스크립트 로드 완료." )
        << BUILD_LOG( m_mapProvideItem.size() )
        << BUILD_LOG( m_mapRequirMission.size() )
        << END_LOG;

    return true;
}

bool KSystemGuide::LoadGuideElements( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::map< int, VEC_REWARD > >& mapProvideItem_,
                                     OUT std::map<int, std::vector< DWORD > >& mapGuideRequireMission_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    mapProvideItem_.clear();
    mapGuideRequireMission_.clear();

    int i = 1;
    while( true ) {
        std::vector< DWORD > vecRequirMission;
        std::map< int, VEC_REWARD > mapProvideItemInfo;
        int nGuideType = 0;

        if( kLuaMng_.BeginTable( i++ ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Guide_Type", nGuideType ) == S_OK, return false );
        _JIF( LoadRequireMissionList( kLuaMng_, std::string("Requirement"), vecRequirMission ), return false );
        if ( true != vecRequirMission.empty() ) {
        if( !mapGuideRequireMission_.insert( std::make_pair( nGuideType, vecRequirMission ) ).second ) {
            START_LOG( cerr, L"중복된 가이드 정보가 있음(CompleteMission) : " << nGuideType ) << END_LOG;
                return false;
        }
        }

        _JIF( LoadProvideItemList( kLuaMng_, std::string("ProvideItemList"), mapProvideItemInfo ), return false );
        if ( true != mapProvideItemInfo.empty() ) {
        if( !mapProvideItem_.insert( std::make_pair( nGuideType, mapProvideItemInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 가이드 정보가 있음(ProvideItemList) : " << nGuideType ) << END_LOG;
            return false;
        }
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}


bool KSystemGuide::LoadRequireMissionList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< DWORD >& vecRequireMission_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( LoadMissionList( kLuaMng_, std::string("CompleteMission"), vecRequireMission_ ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSystemGuide::LoadMissionList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< DWORD >& vecMissionList_ )
{
    vecMissionList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        DWORD dwMissionID = 0;
        if( kLuaMng_.GetValue( i++, dwMissionID ) != S_OK ) break;
        vecMissionList_.push_back( dwMissionID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSystemGuide::LoadProvideItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, VEC_REWARD >& mapProvideItemInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        if( kLuaMng_.BeginTable( i++ ) != S_OK ) break;
        VEC_REWARD vecProvide;
        int nCharType = -1;
        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( LoadItemList( kLuaMng_, std::string("ItemList"), vecProvide ), return false );

        if( !mapProvideItemInfo_.insert( std::make_pair( nCharType, vecProvide ) ).second ) {
            START_LOG( cerr, L" 스크립트에 중복된 캐릭터 타입이 있음 : " << nCharType ) << END_LOG;
            return false;
    }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSystemGuide::LoadItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecItemInfo_ )
{
    vecItemInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kData;
        int nGradeID = 0;

        _JIF( kLuaMng_.GetValue( 1, kData.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kData.m_nPeriod ) == S_OK, return false );
        kData.m_cGradeID = static_cast<char>( nGradeID );

        vecItemInfo_.push_back( kData );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSystemGuide::GetProvideItem( IN int& nGuideType_, IN int& nCharType_, OUT VEC_REWARD& vecProvide_ )
{
    std::map< int, std::map< int, VEC_REWARD > >::iterator mitProvide;
    std::map< int, VEC_REWARD >::iterator mitCharProvideInfo;
    vecProvide_.clear();

    mitProvide = m_mapProvideItem.find( nGuideType_ );

    if ( m_mapProvideItem.end() == mitProvide ) {
        START_LOG( cerr, L"아이템 요청을 한 가이드 타입이 유효하지 않다" )
            << BUILD_LOG( nGuideType_ )
            << END_LOG;

        return false;
    }

    mitCharProvideInfo = mitProvide->second.find( nCharType_ );
    if ( mitProvide->second.end() == mitCharProvideInfo ) {
        START_LOG( cerr, L"아이템 요청을 한 캐릭터 타입에 해당하는 정보가 없다" )
            << BUILD_LOG( nGuideType_ )
            << BUILD_LOG( nCharType_ )
            << END_LOG;

        return false;
    }

    vecProvide_ = mitCharProvideInfo->second;

    return true;
}

bool KSystemGuide::CheckGuideNeedMission( IN DWORD& dwMissionID_, OUT int& nGuideType_ )
{
    std::map<int, std::vector< DWORD > >::iterator mitMission;

    for( mitMission = m_mapRequirMission.begin(); mitMission != m_mapRequirMission.end(); mitMission++ ) {
        std::vector< DWORD >::iterator vitMissionID;
        // 완료 한 미션이 가이드를 해야 하는 미션ID인지 확인 한다
        vitMissionID = std::find( mitMission->second.begin(), mitMission->second.end(), dwMissionID_ );
        if ( mitMission->second.end() != vitMissionID ) {
            nGuideType_ = mitMission->first;
            return true;
        }
    }

    nGuideType_ = -1;
    return false;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


KSystemGuideUserData::KSystemGuideUserData()
{
    m_mapComplete.clear();
}

KSystemGuideUserData::~KSystemGuideUserData()
{
}

void KSystemGuideUserData::SetGuideCompleteInfo( IN std::map< int, KGuideCompleteInfo >& mapCompleteInfo_ )
{
    if ( false == mapCompleteInfo_.empty() ) {
        m_mapComplete = mapCompleteInfo_;
    }
}

void KSystemGuideUserData::GetGuideCompleteInfo( OUT std::map< int, KGuideCompleteInfo >& mapCompleteInfo_ )
{
    mapCompleteInfo_ = m_mapComplete;
}

void KSystemGuideUserData::UpdateGuideComplete( IN int& nGuideType_, IN const KGuideCompleteInfo& kCompleteInfo_ )
{
    std::map< int, KGuideCompleteInfo >::iterator mitComplete;

    mitComplete = m_mapComplete.find( nGuideType_ );

    if ( m_mapComplete.end() == mitComplete ) {
        m_mapComplete.insert( std::make_pair( nGuideType_, kCompleteInfo_ ) );
    }
    else {
        mitComplete->second = kCompleteInfo_;
    }
}

void KSystemGuideUserData::UpdateItemProvideComplete( IN int& nGuideType_ )
{
    std::map< int, KGuideCompleteInfo >::iterator mitComplete;

    mitComplete = m_mapComplete.find( nGuideType_ );

    if ( m_mapComplete.end() == mitComplete ) {
        KGuideCompleteInfo kCompleteInfo;
        kCompleteInfo.m_nProvideFlag = KGuideCompleteInfo::FLAG_GUIDE_ITEM_PROVIDE_COMPLETE;
        m_mapComplete.insert( std::make_pair( nGuideType_, kCompleteInfo ) );
    }
    else {
        mitComplete->second.m_nProvideFlag = KGuideCompleteInfo::FLAG_GUIDE_ITEM_PROVIDE_COMPLETE;
    }
}

void KSystemGuideUserData::GetProvideDataByGuideType( IN int& nGuideType_, OUT KGuideCompleteInfo& kCompleteInfo_ )
{
    std::map< int, KGuideCompleteInfo >::iterator mitComplete;

    mitComplete = m_mapComplete.find( nGuideType_ );

    // 가이드 아이템 지급 기록이 있는지 확인
    if ( m_mapComplete.end() != mitComplete ) {
        kCompleteInfo_.m_nProvideFlag = mitComplete->second.m_nProvideFlag;
    }
    else {
        kCompleteInfo_.m_nProvideFlag = KGuideCompleteInfo::FLAG_GUIDE_ITEM_NO_PROVIDE;
    }
}

bool KSystemGuideUserData::CheckAlreadyCompleteGuide( IN int& nGuideType_ )
{
    std::map< int, KGuideCompleteInfo >::iterator mitComplete;

    mitComplete = m_mapComplete.find( nGuideType_ );

    // 해당 가이드 완료 기록 확인
    if ( m_mapComplete.end() != mitComplete ) {
        if ( KGuideCompleteInfo::FLAG_GUIDE_COMPLETE == mitComplete->second.m_nCompleteFlag ) {
            return true;
        }
    }

    // 완료 기록이 없다
    return false;
}

bool KSystemGuideUserData::CheckAlreadyItemProvide( IN int& nGuideType_ )
{
    std::map< int, KGuideCompleteInfo >::iterator mitComplete;

    mitComplete = m_mapComplete.find( nGuideType_ );

    // 가이드 아이템 지급 기록이 있는지 확인
    if ( m_mapComplete.end() != mitComplete ) {
        if ( KGuideCompleteInfo::FLAG_GUIDE_ITEM_PROVIDE_COMPLETE == mitComplete->second.m_nProvideFlag ) {
            return true;
        }
    }

    // 받은 기록이 없다
    return false;
}