#include ".\shcheckpoint.h"
#include <boost/format.hpp>
#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include "lua/KLuaManager.h"
#include "PostConfig.h"

#include "Log4.h"
namespace {
    struct KComp
    {
        bool operator() ( const KCheckCondition& a, const KCheckCondition& b )
        {
            if( a.m_cLevel < b.m_cLevel ) return true;
            return false;
        }

        bool operator() ( const KCheckCondition& a, const char& b )
        {
            if( a.m_cLevel < b ) return true;
            return false;
        }

        bool operator() ( const char& a, const KCheckCondition& b )
        {
            if( a < b.m_cLevel ) return true;
            return false;
        }
    };
}

//FILE_NAME_FOR_LOG

ImplementSingleton( KSHCheckPoint );
NiImplementRootRTTI( KSHCheckPoint );
ImplOstmOperatorW2A( KSHCheckPoint );

KSHCheckPoint::KSHCheckPoint(void) {}
KSHCheckPoint::~KSHCheckPoint(void) {}

ImplToStringW( KSHCheckPoint )
{
    DumpCheckPoint( stm_ );
    return stm_;
}

bool KSHCheckPoint::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSHCheckPoint::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > > mapCheckCondition;
    std::map< std::pair<char,char>,std::vector<DWORD> > mapRegistMissionAfterResetPunishment;
    mapCheckCondition.clear();
    mapRegistMissionAfterResetPunishment.clear();

    const std::string strScriptName = "InitSHCheck.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "SHInfoList" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        std::map<std::pair<char,char>,std::vector<KCheckCondition>  > mapCondition;
        DWORD dwUserType = 0;
        mapCondition.clear();

        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "UserType", dwUserType ) == S_OK, return false );
        _JIF( LoadCondition( kLuaMng, std::string("CheckPoint"), mapCondition ), return false );

        if( mapCheckCondition.insert( std::make_pair( dwUserType, mapCondition ) ).second == false )
        {
            START_LOG( cerr, L"중복된 유저 데이터. UserType : " << dwUserType ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    std::map< char, DWORD > mapCharExclusiveDungeonMission;
    _JIF( LoadCharExclusiveDungeonMission( kLuaMng, mapCharExclusiveDungeonMission ), return false );

    std::map< std::pair<char,char>,std::vector<DWORD> > mapCharMissionList;
    CalcCharExclusiveDungeonMission( mapCheckCondition, mapCharExclusiveDungeonMission, mapCharMissionList );

    _JIF( LoadRegistMissionAfterResetPunishment( kLuaMng, mapRegistMissionAfterResetPunishment ), return false );

    START_LOG( cerr, L"상하이 드랍 로드 완료.. Count : " << mapCheckCondition.size() ) << END_LOG;

    {
        KLocker lock( m_csCheckCondition );
        m_mapCheckCondition.swap( mapCheckCondition );
        m_mapCharExclusiveDungeonMission.swap( mapCharExclusiveDungeonMission );
        m_mapCharMissionList.swap( mapCharMissionList );
        m_mapRegistMissionAfterResetPunishment.swap( mapRegistMissionAfterResetPunishment );
    }

    return true;
}

bool KSHCheckPoint::NewUserDropCheck( IN OUT std::vector<KDropItemInfo>& vecItem_, IN OUT std::vector<KDropItemInfo>& vecPostItem_, IN OUT std::vector<DWORD>& vecAutoMission_, IN const std::wstring strLogin_, IN const DWORD dwUserType_ )
{
    bool bRet = false;
    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > >::iterator mit;
    KLocker lock( m_csCheckCondition );
    mit = m_mapCheckCondition.find( dwUserType_ );
    if( mit == m_mapCheckCondition.end() ) return false;

    // 캐릭터별로
    std::map<std::pair<char,char>,std::vector<KCheckCondition> >::iterator mitChar;
    for ( mitChar = mit->second.begin() ; mitChar != mit->second.end() ; ++mitChar ) {
        if( mitChar->first.second > 1 ) {
            continue;
        }
        // 0레벨 미션/아이템 등록
        std::vector<KCheckCondition>::iterator vit;
        for ( vit = mitChar->second.begin() ; vit != mitChar->second.end() ; ++vit ) {
            if ( vit->m_cLevel < 0 ) {
                continue;
            }

            if ( vit->m_cLevel > 1 ) {
                break;
            }

            if ( vit->m_bRegister ) {
                // 미션등록
                vecAutoMission_.push_back( vit->m_kItemInfo.m_ItemID );
            }
            else {

                // 인벤토리 지급( 우편 비활성이면 무조껀 ) 
                if ( !SiKPostConfig()->IsPostEnable() || vit->m_nDropType == KCheckCondition::EM_DROP_INVENTORY ) 
                {
                // 아이템 지급
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
                kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
                kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
                vecItem_.push_back( kItemUnit );
            }
                // 우편 지급
                else if ( vit->m_nDropType == KCheckCondition::EM_DROP_POST ) 
                {
                    // 아이템 지급
                    KDropItemInfo kItemUnit;
                    kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
                    kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
                    kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
                    vecPostItem_.push_back( kItemUnit );
                }
            }

            START_LOG( clog, L"상하이 드랍 받음, Name : " << strLogin_ )
                << BUILD_LOGc( vit->m_cCharID )
                << BUILD_LOG( vit->m_kItemInfo.m_ItemID )
                << BUILD_LOG( vit->m_kItemInfo.m_nDuration )
                << BUILD_LOG( vit->m_kItemInfo.m_nPeriod )
                << BUILD_LOG( vit->m_nDropType )
                << BUILD_LOG( vit->m_bRegister ) << END_LOG;
        }
    }

    if ( !vecAutoMission_.empty() || !vecPostItem_.empty() || !vecItem_.empty() ) {
        bRet = true;
    }

    START_LOG( clog, L"상하이 드랍 결과, Name : " << strLogin_ )
        << BUILD_LOG( vecAutoMission_.size() )
        << BUILD_LOG( vecPostItem_.size() )
        << BUILD_LOG( vecItem_.size() ) << END_LOG;

    return bRet;
}

bool KSHCheckPoint::DropCheck( IN OUT std::vector<KDropItemInfo>& vecItem_, IN OUT std::vector<KDropItemInfo>& vecPostItem_, OUT std::vector<DWORD>& vecAutoMission_, IN const SHChecData& shData_, IN const DWORD dwUserType_ )
{
//    vecAutoMission_.clear();
    bool bRet = false;

    // 레벨이 증가 하였는가?
    if ( shData_.m_cOldLevel >= shData_.m_cLevel ) {
        START_LOG( clog, L"레벨이 증가하지 않았다. LoginID : " << shData_.m_strLogin
            << L",CharType : " << (int)shData_.m_cCurrentCharID
            << L",OldLevel : " << (int)shData_.m_cOldLevel
            << L",Level : " << (int)shData_.m_cLevel ) << END_LOG;
        return false;
    }

    KLocker lock( m_csCheckCondition );

    // 채널링 체크
    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > >::iterator mit;
    mit = m_mapCheckCondition.find( dwUserType_ );
    if ( mit == m_mapCheckCondition.end() ) {
        START_LOG( clog, L"해당 채널링타입의 상하이 드랍이 없음. LoginID : " << shData_.m_strLogin << L",Type : " << dwUserType_ ) << END_LOG;
        return false ;
    }

    // 캐릭터 체크
    std::map<std::pair<char,char>,std::vector<KCheckCondition> >::iterator mitChar;
    mitChar = mit->second.find( std::make_pair( shData_.m_cCurrentCharID, shData_.m_cLevel ) );
    if ( mitChar == mit->second.end() ) {
        START_LOG( clog, L"해당 캐릭터 상하이 드랍이 없음. LoginID : " << shData_.m_strLogin
            << L",CharType : " << (int)shData_.m_cCurrentCharID
            << L",CharLevel : " << (int)shData_.m_cLevel )
            << END_LOG;
        return false;
    }

    // 레벨 체크
    std::vector<KCheckCondition>::iterator vit;
    for ( vit = mitChar->second.begin() ; vit != mitChar->second.end() ; ++vit ) {
        if ( vit->m_bRegister ) {
            // 미션 지급
            vecAutoMission_.push_back( vit->m_kItemInfo.m_ItemID );
        }
        else {
            
            // 인벤토리 지급
            if ( !SiKPostConfig()->IsPostEnable() || vit->m_nDropType == KCheckCondition::EM_DROP_INVENTORY ) 
            {
            // 아이템 지급
            KDropItemInfo kItemUnit;
            kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
            kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
            kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
            vecItem_.push_back( kItemUnit );
        }
            // 우편 지급
            else if ( vit->m_nDropType == KCheckCondition::EM_DROP_POST ) 
            {
                // 아이템 지급
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
                kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
                kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
                vecPostItem_.push_back( kItemUnit );
            }
        }

        START_LOG( clog, L"상하이 드랍 받음, Name : " << shData_.m_strLogin )
            << BUILD_LOGc( vit->m_cCharID )
            << BUILD_LOG( vit->m_kItemInfo.m_ItemID )
            << BUILD_LOG( vit->m_kItemInfo.m_nDuration )
            << BUILD_LOG( vit->m_kItemInfo.m_nPeriod )
            << BUILD_LOG( vit->m_nDropType )
            << BUILD_LOG( vit->m_bRegister ) << END_LOG;
    }

    if ( !vecAutoMission_.empty() || !vecPostItem_.empty() || !vecItem_.empty() ) {
        bRet = true;
    }

    START_LOG( clog, L"상하이 드랍 결과, Name : " << shData_.m_strLogin )
        << BUILD_LOG( vecAutoMission_.size() )
        << BUILD_LOG( vecPostItem_.size() )
        << BUILD_LOG( vecItem_.size() ) << END_LOG;

    return bRet;
}

void KSHCheckPoint::DumpCheckPoint( OUT std::wostream& stm_ ) const
{
    boost::wformat fmt( L"[%d] %d/ %d/ %d/ %d/ %d %s\n" );
    stm_ << L" -- Check Point -- " << std::endl;
    KLocker lock( m_csCheckCondition );
    stm_ << L" size : " << m_mapCheckCondition.size() << std::endl;

    int nIndex =0;

    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > >::const_iterator mit;
    for ( mit = m_mapCheckCondition.begin() ; mit != m_mapCheckCondition.end() ; ++mit ,++nIndex ) {

        std::map<std::pair<char,char>,std::vector<KCheckCondition> >::const_iterator mitChar;
        for( mitChar = mit->second.begin() ; mitChar != mit->second.end() ; ++mitChar ) {

            std::vector<KCheckCondition>::const_iterator vit;
            for( vit = mitChar->second.begin(); vit != mitChar->second.end(); ++vit ) {
                fmt % nIndex
                    % static_cast<int>(vit->m_cCharID)
                    % static_cast<int>(vit->m_cLevel)
                    % vit->m_kItemInfo.m_ItemID
                    % vit->m_kItemInfo.m_nDuration
                    % vit->m_kItemInfo.m_nPeriod
                    % ((vit->m_bRegister)? (L"true"): (L"false"))
                    % vit->m_nDropType;

                stm_ << fmt.str();
            }
        }
    }
}

bool KSHCheckPoint::LoadCondition( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<std::pair<char,char>,std::vector<KCheckCondition> >& mapConditionList_ )
{
    mapConditionList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        //{ ID, CharID, Level, Item, Factor, Register }
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KCheckCondition kCondition;
        int nCharType = -1;
        int nLevel = -1;
        std::pair<char,char> prData;

        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kCondition.m_kItemInfo.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kCondition.m_kItemInfo.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, kCondition.m_kItemInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 6, kCondition.m_bRegister ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 7, kCondition.m_nDropType ) == S_OK, return false );

        //  자동 등록은 우편이 될수 없다.
        if ( kCondition.m_nDropType == KCheckCondition::EM_DROP_POST && kCondition.m_bRegister ) 
        {
            START_LOG( cerr, L"자동 지급은 우편으로 할수 없습니다. DropType : " << kCondition.m_nDropType )
                << END_LOG;
            return false;
        }
        
        kCondition.m_cLevel = (char)nLevel;
        kCondition.m_cCharID = (char)nCharType;
        prData = std::make_pair( kCondition.m_cCharID, kCondition.m_cLevel );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( nCharType < 0 )
        {
            START_LOG( cerr, L"캐릭터 타입이 음수임.. ItemID : " << kCondition.m_kItemInfo.m_ItemID ) << END_LOG;
            return false;
        }

        mapConditionList_[prData].push_back( kCondition );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSHCheckPoint::NewCharacterDropCheck( IN OUT std::vector<KDropItemInfo>& vecItem_, IN OUT std::vector<KDropItemInfo>& vecPostItem_, IN OUT std::vector<DWORD>& vecAutoMission_, IN const SHChecData& shData_, IN const DWORD dwUserType_ )
{
    bool bRet = false;
    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > >::iterator mit;
    KLocker lock( m_csCheckCondition );
    mit = m_mapCheckCondition.find( dwUserType_ );
    if( mit == m_mapCheckCondition.end() ) return false;

    // 캐릭터 체크
    std::map<std::pair<char,char>,std::vector<KCheckCondition> >::iterator mitChar;
    mitChar = mit->second.find( std::make_pair( shData_.m_cCurrentCharID, shData_.m_cLevel ) );
    if ( mitChar == mit->second.end() ) {
        return false;
    }

    // 현재 레벨 미션/아이템 등록
    std::vector<KCheckCondition>::iterator vit;
    for ( vit = mitChar->second.begin() ; vit != mitChar->second.end() ; ++vit ) {
        if ( vit->m_bRegister ) {
            // 미션등록
            vecAutoMission_.push_back( vit->m_kItemInfo.m_ItemID );
        }
        else {
            // 인벤토리 지급
            if ( !SiKPostConfig()->IsPostEnable() || vit->m_nDropType == KCheckCondition::EM_DROP_INVENTORY ) {
                // 아이템 지급
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
                kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
                kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
                vecItem_.push_back( kItemUnit );
            }
            // 우편 지급
            else if ( vit->m_nDropType == KCheckCondition::EM_DROP_POST ) {
                // 아이템 지급
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID  = vit->m_kItemInfo.m_ItemID;
                kItemUnit.m_nDuration = vit->m_kItemInfo.m_nDuration;
                kItemUnit.m_nPeriod = vit->m_kItemInfo.m_nPeriod;
                vecPostItem_.push_back( kItemUnit );
            }
        }

        START_LOG( clog, L"상하이 드랍 받음, Name : " << shData_.m_strLogin )
            << BUILD_LOGc( vit->m_cCharID )
            << BUILD_LOG( vit->m_kItemInfo.m_ItemID )
            << BUILD_LOG( vit->m_kItemInfo.m_nDuration )
            << BUILD_LOG( vit->m_kItemInfo.m_nPeriod )
            << BUILD_LOG( vit->m_nDropType )
            << BUILD_LOG( vit->m_bRegister ) << END_LOG;
    }

    if ( !vecAutoMission_.empty() || !vecPostItem_.empty() || !vecItem_.empty() ) {
        bRet = true;
    }

    START_LOG( clog, L"상하이 드랍 결과, Name : " << shData_.m_strLogin )
        << BUILD_LOG( vecAutoMission_.size() )
        << BUILD_LOG( vecPostItem_.size() )
        << BUILD_LOG( vecItem_.size() ) << END_LOG;

    return bRet;
}

bool KSHCheckPoint::LoadCharExclusiveDungeonMission( IN KLuaManager& kLuaMng_, OUT std::map< char, DWORD >& mapCharExclusiveDungeonMission_ )
{
    mapCharExclusiveDungeonMission_.clear();
    _JIF( kLuaMng_.BeginTable( "CharacterExclusiveDungeonMission" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        DWORD dwLimitLevel = 0;
        std::set<int> setDungeonInfo;
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Level", dwLimitLevel ) == S_OK, return false );

        if( mapCharExclusiveDungeonMission_.insert( std::make_pair( (char)nChar, dwLimitLevel ) ).second == false ) {
            START_LOG( cerr, L"특정 던전을 가진 캐릭터 중복된 ID. CharID :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KSHCheckPoint::CalcCharExclusiveDungeonMission( IN const std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > > mapCheckCondition_, IN const std::map< char, DWORD > mapCharExclusiveDungeonMission_, OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapCharMissionList_ )
{
    mapCharMissionList_.clear();

    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > >::const_iterator cmit;
    std::map< char, DWORD >::const_iterator cmitCharLevel;
    std::vector<KCheckCondition>::const_iterator cvit;
    for( cmit = mapCheckCondition_.begin(); cmit != mapCheckCondition_.end(); ++cmit ) {
        for( cmitCharLevel = mapCharExclusiveDungeonMission_.begin(); cmitCharLevel != mapCharExclusiveDungeonMission_.end(); ++cmitCharLevel ) {
            for( DWORD dwCharLevel = 1; dwCharLevel <= cmitCharLevel->second; ++dwCharLevel ) {
                std::pair<char,char> prData = std::make_pair( cmitCharLevel->first, (char)dwCharLevel );
                std::map<std::pair<char,char>,std::vector<KCheckCondition> >::const_iterator cmitTotal;
                cmitTotal = cmit->second.find( prData );
                if( cmitTotal != cmit->second.end() ) {
                    for( cvit = cmitTotal->second.begin(); cvit != cmitTotal->second.end(); ++cvit ) {
                        mapCharMissionList_[prData].push_back( cvit->m_kItemInfo.m_ItemID );
                    }
                }
            }
        }
    }
}

void KSHCheckPoint::GetCharMissionList( OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapCharMissionList_ )
{
    mapCharMissionList_.clear();

    KLocker lock( m_csCheckCondition );
    mapCharMissionList_ = m_mapCharMissionList;
}

bool KSHCheckPoint::LoadRegistMissionAfterResetPunishment( IN KLuaManager& kLuaMng_, OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapRegistMissionAfterResetPunishment_ )
{
    mapRegistMissionAfterResetPunishment_.clear();
    _JIF( kLuaMng_.BeginTable( "RegistMissionListAfterResetPunishment" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::vector<DWORD> vecMission;

        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( LoadRegMission( kLuaMng_, std::string("MissionID"), vecMission ), return false );

        // 초기화 후 레벨은 1임
        std::pair<char,char> prData = std::make_pair( (char)nChar, 1 );

        if( mapRegistMissionAfterResetPunishment_.insert( std::make_pair( prData, vecMission ) ).second == false ) {
            START_LOG( cerr, L"초기화 처벌 시 재등록 미션 중복 됨 CharID :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSHCheckPoint::LoadRegMission( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<DWORD>& vecRegMission_ )
{
    vecRegMission_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        //{ ID, CharID, Level, Item, Factor, Register }
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwMissionID = 0;
        _JIF( kLuaMng_.GetValue( 1, dwMissionID ) == S_OK, return false );

        vecRegMission_.push_back( dwMissionID );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KSHCheckPoint::GetRegistAginMissionList( OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapRegistMissionAfterResetPunishment_ )
{
    mapRegistMissionAfterResetPunishment_.clear();

    KLocker lock( m_csCheckCondition );
    mapRegistMissionAfterResetPunishment_ = m_mapRegistMissionAfterResetPunishment;
}