#include ".\skillmanager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSkillManager );
NiImplementRootRTTI( KSkillManager );
ImplOstmOperatorW2A( KSkillManager );

std::wostream& operator<<( std::wostream& stm_, const std::pair<const int,SSkillElement>& data_ )
{
    return stm_ << data_.first;
}

ImplToStringW( KSkillManager )
{
    KLocker lock1( m_csConfig );
    KLocker lock( m_csSkills );
    START_TOSTRINGW
        << TOSTRINGW( m_sSkillConfig.m_nMaxSkillCount )
        << TOSTRINGW( m_sSkillConfig.m_setRemoveSkillItem.size() )
        << TOSTRINGW( m_sSkillConfig.m_dwUnLockItemID )
        << TOSTRINGW( m_sSkillConfig.m_nUnLockCount )
        << TOSTRINGW( m_mapSkills.size() );

    stm_ << L"---- Skill ID : ";
    std::copy( m_mapSkills.begin(), m_mapSkills.end(), 
        std::ostream_iterator<std::map<int,SSkillElement>::value_type,wchar_t>( stm_, L", " ) );

    return stm_;
}

KSkillManager::KSkillManager(void)
{
}

KSkillManager::~KSkillManager(void)
{
}

bool KSkillManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSkillManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSkillInfo.lua";
    strScript_ = strScriptName;
    GCITEMID SkillOpenItemID;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadSPXScript( kLuaMng ), return false );
    _JIF( LoadConfig( kLuaMng ), return false );
    _JIF( LoadMaxTrainPoint( kLuaMng ), return false );
    _JIF( LoadMaxCharSPLevel( kLuaMng ), return false );
    _JIF( LoadSkillExceptionID( kLuaMng ), return false );
    _JIF( LoadCharSPXType( kLuaMng ), return false );
    _JIF( LoadAddCharSPX( kLuaMng ), return false );
    _JIF( LoadExceptionCharTypeList( kLuaMng ), return false );
    _JIF( LoadAPCharAddSlotOpenInfo( kLuaMng ), return false );
    _JIF( LoadMissionRewardSkillInfo( kLuaMng ), return false );
    _JIF( LoadAbilitySkillInfo( kLuaMng ), return false );
    _JIF( LoadOldSkillTreeUseCharacter( kLuaMng ), return false );
    _JIF( LoadSkillTreeOpenCharInfo( kLuaMng ), return false );
    _JIF( LoadSkillScroolInfo( kLuaMng ), return false );

    _JIF( kLuaMng.GetValue( "SkillOpenKeyItemInfo", SkillOpenItemID ) == S_OK, return false );
    START_LOG( cerr, L"4단필 오픈 키 정보 읽기 완료 : " << SkillOpenItemID ) << END_LOG;
    {
        KLocker lock( m_csSkillTreeOpenChar );
        m_SkillOpenItemID = SkillOpenItemID;
    }

    // InitSkillTree.lua 파일 Load.
    _JIF( LoadSkillTreeScript(strScript_), return false );

    return true;
}

bool KSkillManager::LoadSkillTreeScript( OUT std::string& strScript_ )
{
    std::map<int,SSkillElement> mapSkills;
    std::map< int, char>        mapLockGroups;
    std::string strRequire( "Enum.stg" );

    std::set<std::pair<std::string, std::string>> setSkillTreeOpenCharInfo;
    std::set<std::pair<std::string, std::string>>::iterator sit;

    GetSkillTreeOpenCharInfo( setSkillTreeOpenCharInfo );

    KLuaManager kLuaMng;

    for( sit = setSkillTreeOpenCharInfo.begin(); sit != setSkillTreeOpenCharInfo.end(); ++sit ) {
        std::string strFileName( "SkillTree\\InitSkillTree" );
        strFileName = strFileName + sit->first + "_" + sit->second + ".lua";
        _JIF( kLuaMng.DoFile( strRequire.c_str() ) == S_OK, return false );
        _JIF( kLuaMng.DoFile( strFileName.c_str() ) == S_OK, return false );
        _JIF( kLuaMng.BeginTable( "SkillTree" ) == S_OK, return false );
        int nCharType = ::atoi( sit->first.c_str() );
        for( int i = 1 ; ; ++i ) {
            SSkillElement sSkill;
            if( kLuaMng.BeginTable( i ) != S_OK )
                break;
            _JIF( LoadSkillElement( kLuaMng, sSkill ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            if( !sSkill.m_bEnable ) continue; // 활성화 되지 않은 스킬은 읽지도 말자.

            if( nCharType != (int)sSkill.m_cCharType ) {
                sSkill.m_cCharType = static_cast<char>( nCharType );
                START_LOG( cerr, L" 스킬 CharType 오류 , SkillID : " << sSkill.m_nID ) << END_LOG;
            }
            mapLockGroups[sSkill.m_nLockGroupID] = sSkill.m_cCharType;
            // insert
            if( !mapSkills.insert( std::make_pair( sSkill.m_nID, sSkill ) ).second ) {
                START_LOG( cerr, L"스킬 로드시 중복된 ID가 존재, ID : " << sSkill.m_nID ) << END_LOG;
            }
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        strScript_ = strFileName;
    }

    START_LOG( cerr, L" 스킬 로드 완료, Count : " << mapSkills.size() ) << END_LOG;
    START_LOG( cerr, L" Lockgroup 로드 완료, Count : " << mapLockGroups.size() ) << END_LOG;

    KLocker lock( m_csSkills );
    m_mapSkills.swap( mapSkills );
    m_mapLockGroups.swap( mapLockGroups );

    return true;
}

bool KSkillManager::LoadSkillElement( IN KLuaManager& kLuaMng_, OUT SSkillElement& sSkill_ )
{
    int nCharType;
    int nPromotion;
    _JIF( kLuaMng_.GetValue( "ID", sSkill_.m_nID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "GroupID", sSkill_.m_nGroupID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CharLv", sSkill_.m_nCharLv ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Promotion", nPromotion ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Enable", sSkill_.m_bEnable ) == S_OK, return false );
    sSkill_.m_cCharType     = (char)nCharType;
    sSkill_.m_cPromotion    = (char)nPromotion;
    _JIF( kLuaMng_.GetValue( "LearnSP", sSkill_.m_nLearnSP ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "LockGroup", sSkill_.m_nLockGroupID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Base", sSkill_.m_bBase ) == S_OK, return false );

    sSkill_.m_bOrCheckPreList = false;
    kLuaMng_.GetValue( "OrCheck_PreList", sSkill_.m_bOrCheckPreList );

    // Pre
    _JIF( kLuaMng_.BeginTable( "PreList" ) == S_OK, return false );
    _JIF( LoadList( kLuaMng_, sSkill_.m_setPreList ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // Next
    _JIF( kLuaMng_.BeginTable( "NextList" ) == S_OK, return false );
    _JIF( LoadList( kLuaMng_, sSkill_.m_setNextList ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSkillManager::LoadList( IN KLuaManager& kLuaMng_, OUT std::set<int>& setList_ )
{
    for( int i = 1 ; ; ++i )
    {
        int nID = -1;
        if( S_OK != kLuaMng_.GetValue( i, nID ) ) break;
        if( !setList_.insert( nID ).second )
        {
            START_LOG( cerr, L" 해당 리스트에 중복된 ID가 존재, ID : " << nID ) << END_LOG;
            return false;
        }
    }
    return true;
}

bool KSkillManager::LoadSPXScript( KLuaManager& kLuaMng_ )
{
    std::map< DWORD, std::map<int,int> > mapSPXTotalTable;
    _JIF( kLuaMng_.BeginTable( "SPXTotalInfo" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        DWORD dwSPXType = 0;
        std::map<int,int> mapSPXTable;
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        _JIF( kLuaMng_.GetValue( "SPXType", dwSPXType ) == S_OK, return false );

        _JIF( LoadSPXTableInfo( kLuaMng_, mapSPXTable ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        // insert
        if( !mapSPXTotalTable.insert( std::make_pair( dwSPXType, mapSPXTable ) ).second )
        {
            START_LOG( cerr, L"Total SPX 중복된 Type 존재 Type : " << dwSPXType ) << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 스킬 전체 읽기 완료, Count : " << mapSPXTotalTable.size() ) << END_LOG;

    KLocker lock( m_csSPXTable );
    m_mapSPXTotalTable.swap( mapSPXTotalTable );

    return true;
}

bool KSkillManager::LoadConfig( KLuaManager& kLuaMng_ )
{
    SSkillConfig sSkillConfig;
    _JIF( kLuaMng_.BeginTable( "SkillConfig" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxSkillSetNum", sSkillConfig.m_nMaxSkillCount ) == S_OK, return false );

    {
        _JIF( kLuaMng_.BeginTable( "SkillRemoveItem" ) == S_OK, return false );
        for( int i = 1 ; ; ++i )
        {
            int nRemoveItem = 0;
            if( kLuaMng_.GetValue( i, nRemoveItem ) != S_OK ) break;
            if( nRemoveItem == 0 ) break;
            sSkillConfig.m_setRemoveSkillItem.insert( nRemoveItem );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    {
        _JIF( kLuaMng_.BeginTable( "SkillResetItem" ) == S_OK, return false );
        for( int i = 1 ; ; ++i )
        {
            int nResetItem = 0;
            if( kLuaMng_.GetValue( i, nResetItem ) != S_OK ) break;
            if( nResetItem == 0 ) break;
            sSkillConfig.m_setResetSkillItem.insert( nResetItem );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    {
        _JIF( kLuaMng_.BeginTable( "UnLockGoupItem" ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 1, sSkillConfig.m_dwUnLockItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, sSkillConfig.m_nUnLockCount ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // 검증.
    _JIF( sSkillConfig.CheckValid(), return false );

    START_LOG( cerr, L" 스킬 config 읽기 완료. " ) << END_LOG;
    KLocker lock( m_csConfig );
    m_sSkillConfig = sSkillConfig;
    return true;
}

bool KSkillManager::LoadMaxTrainPoint( KLuaManager& kLuaMng_ )
{
    std::map<std::pair<char,char>, int> mapMaxTrainSP;
    _JIF( kLuaMng_.BeginTable( "MaxTrainSkillPoint" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nCharType;
        int nPromotion;
        int nCount;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nPromotion ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        std::pair<char,char> prKey((char)nCharType, (char)nPromotion );
        // insert
        if( !mapMaxTrainSP.insert( std::make_pair( prKey, nCount ) ).second )
        {
            START_LOG( cerr, L"Char Type, Promotion이 존재, Char : " << nCharType
                << L", Promotion : " << nPromotion ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"MaxTrainSkillPoint 읽기 완료 : " << mapMaxTrainSP.size() ) << END_LOG;
    KLocker lock( m_csMaxTrainSP );
    m_mapMaxTrainSP.swap( mapMaxTrainSP );
    return true;
}

void KSkillManager::PrintSkill( int nID_ )
{
    SSkillElement sSkill;
    if( !GetSkill( nID_, sSkill ) )
        std::wcout << L" Can't Find Skill : " << nID_ << std::endl;

    std::wcout << L" :: Skill Info :: " << std::endl
        << TOSTRINGW( sSkill.m_nID )
        << TOSTRINGW( sSkill.m_nGroupID );

    std::wcout << L"    PreList : " ;
    std::copy( sSkill.m_setPreList.begin(), sSkill.m_setPreList.end(), 
        std::ostream_iterator<std::set<int>::value_type,wchar_t>( std::wcout, L", " ) );

    std::wcout << std::endl << L"    NextList : " ;
    std::copy( sSkill.m_setNextList.begin(), sSkill.m_setNextList.end(), 
        std::ostream_iterator<std::set<int>::value_type,wchar_t>( std::wcout, L", " ) );

    std::wcout << std::endl;
    std::wcout << TOSTRINGW( sSkill.m_nCharLv )
        << TOSTRINGWc( sSkill.m_cCharType)
        << TOSTRINGWc( sSkill.m_cPromotion)
        << TOSTRINGWb( sSkill.m_bEnable)
        << TOSTRINGW( sSkill.m_nLearnSP)
        << TOSTRINGW( sSkill.m_nLockGroupID)
        << TOSTRINGWb( sSkill.m_bBase);

}

bool KSkillManager::GetSkill( IN int nID_, OUT SSkillElement& sSkill_ )
{
    std::map<int,SSkillElement>::iterator mit;
    KLocker lock( m_csSkills );
    mit = m_mapSkills.find( nID_ );
    if( mit == m_mapSkills.end() )
        return false;
    sSkill_ = mit->second;
    return true;
}

void KSkillManager::InitSkillInfo( OUT KSkillInfo& kSkill_, IN const char cCharType_, IN const char cPromotion_, IN const DWORD dwLevel_ )
{
/*
    // 캐릭터 생성시에 시작 SP Lv 추가
    KLocker lock( m_csMaxCharSPLevel );
    std::map< std::pair<char,char>, std::pair<int,int> >::iterator mit;
    std::pair<char,char> prKey( nCharType_, nPromotion_ );
    mit = m_mapMaxCharSPLevel.find( prKey );
    if( mit == m_mapMaxCharSPLevel.end() ) { // 시작 레벨 설정이 없는 캐릭터는 0으로 초기화.
        kSkill_.m_nLv = 0;
    } else {
        kSkill_.m_nLv = mit->second.first;
    }
    kSkill_.m_dwSkillExp    = 0;
    kSkill_.m_dwMaxSkillExp = 0;
*/
    // 현재의 CharSPLv설정과 MaxCharSPLV설정을 한다.
    GetSkillPointInfo( cCharType_, cPromotion_, dwLevel_, kSkill_.m_nLvSPPoint );
    GetMaxSPX( cCharType_, cPromotion_, kSkill_.m_nMaxSPPoint );
}

int KSkillManager::GetAccSPCount( char cCharType_, int nLv_ )
{
    nLv_ = std::max<int>( nLv_, 0 );
    return nLv_;
}
/*
void KSkillManager::UpdateSpInfo( IN OUT int& nSpLv_, IN OUT DWORD& dwSPX_, IN DWORD dwObtainSpx_, IN const char cChar_, IN const char cCharType_ )
{
    int nLv = nSpLv_;
    DWORD dwSPX = dwSPX_ + dwObtainSpx_;
    SSPXTable sSpxTable;

    do 
    {
        // 해당 Char의 전직이 최대 spx Level이 있는지 체크한다.
        if( !CheckMaxCharSPLevel( cChar_, cCharType_, nLv, dwSPX ) ){
            nSpLv_ = nLv;
            dwSPX_ = dwSPX;
            break;
        }
 
        if( !GetSpxTable( cChar_, nLv, sSpxTable ) )
        {
            START_LOG( cerr, L"해당 SPX 테이블 정보 가져 오지 못함., LV : " << nLv ) << END_LOG;
            break;
        }

        if( sSpxTable.m_dwCapacity > dwSPX )
        {
            nSpLv_ = nLv;
            dwSPX_ = dwSPX;
            break;
        }

        nSpLv_ = nLv;
        dwSPX_ = sSpxTable.m_dwCapacity;
        dwSPX -= sSpxTable.m_dwCapacity;

        ++nLv;

    } while ( IsInSkillLvRange( cChar_, nLv ) );
}

bool KSkillManager::IsInSkillLvRange( IN char cChar_, IN int nLv_ )
{
    int nSPXType = 0;
    std::map<int,SSPXTable> mapSPXTable;
    // 해당 캐릭터의 SPXType 체크.
    _JIF( GetCharSPXTableType( cChar_, nSPXType ), return false );
    // SPXType의 전체 SPXTable 가져오기.
    _JIF( GetCharSPXTable( nSPXType, mapSPXTable ), return false );

    if( mapSPXTable.empty() ) return false;
    if( mapSPXTable.begin()->first > nLv_ ) return false;
    if( mapSPXTable.rbegin()->first < nLv_ ) return false;
    return true;
}
*/
void KSkillManager::InitSkillSetting( char cMaxPromotion, std::map<char, int>& m_mapEquipSkillSet_ )
{
    m_mapEquipSkillSet_.clear();
    for( char cIndex = 0 ; cIndex <= cMaxPromotion ; ++cIndex )
    {
        m_mapEquipSkillSet_[cIndex] = 0;
    }
}


bool KSkillManager::IsRemoveSkillItem( GCITEMID dwItemID_ )
{
    KLocker lock( m_csConfig);
    return (m_sSkillConfig.m_setRemoveSkillItem.find( dwItemID_ ) !=  m_sSkillConfig.m_setRemoveSkillItem.end());
}

void KSkillManager::InitSkillSets( IN const std::map<char, KCharacterInfo>& mapCharacterInfo_,
                                    OUT std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSets_ ) // 스킬 세트 초기구성
{
    mapSkillSets_.clear();
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = mapCharacterInfo_.begin() ; cmit != mapCharacterInfo_.end() ; ++cmit ) {

        for ( char cCount = 0 ; cCount <= cmit->second.m_cPromotion ; ++cCount ) { // (최대) 전직 만큼 순회
            std::pair<char,char> prKey( cmit->second.m_cCharType, cCount ); // 캐릭터, 전직 묶어서 키 생성
            KSPSetInfo kInfo;

            for ( int i = 0 ; i < SiKSkillManager()->GetMaxSkillSetNum() ; ++i ) { // 스킬세트는 2개
                kInfo.m_nSPSetID = i;
                mapSkillSets_[prKey].push_back( kInfo ); // 빈 데이터 미리 넣어놓기
            }

        }
    }

}

int KSkillManager::GetMaxTrainSP( char cChar_, char cPromotion_ )
{
    KLocker lock( m_csMaxTrainSP );
    std::map<std::pair<char,char>, int>::iterator mit;
    std::pair<char,char> prKey( cChar_, cPromotion_ );
    mit = m_mapMaxTrainSP.find( prKey );
    if( mit == m_mapMaxTrainSP.end() ) return 0;
    return mit->second;
}

int KSkillManager::CalcSpendSP( std::vector<int>& vecTrainSkill_ )
{
    int nAccCount = 0;
    std::vector<int>::iterator vit;
    for( vit = vecTrainSkill_.begin() ; vit != vecTrainSkill_.end() ; ++vit )
    {
        SSkillElement sSkill;
        if( GetSkill( *vit, sSkill ) )
        {
            nAccCount += sSkill.m_nLearnSP;
        }
    }
    return nAccCount;
}

bool KSkillManager::IsLockGroupID( int nLockGroupID_, char& cCharType_ )
{
    KLocker lock( m_csSkills );
    std::map< int, char>::iterator mit;
    mit = m_mapLockGroups.find( nLockGroupID_ );
    if( mit == m_mapLockGroups.end() ) return false;
    cCharType_ = mit->second;
    return true;
}

GCITEMID KSkillManager::GetUnLockItemID()
{
    KLocker lock( m_csConfig );
    return m_sSkillConfig.m_dwUnLockItemID;
}

int KSkillManager::GetUnLockItemCount()
{
    KLocker lock( m_csConfig );
    return m_sSkillConfig.m_nUnLockCount;
}

bool KSkillManager::LoadMaxCharSPLevel( KLuaManager& kLuaMng_ )
{
    std::map< std::pair<char,char>, std::pair<int,int> > mapMaxCharSPLevel;
    mapMaxCharSPLevel.clear();
    _JIF( kLuaMng_.BeginTable( "MaxCharSkillLevel" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nCharType;
        int nPromotion;
        int nStartLevel;
        int nEndLevel;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nPromotion ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nStartLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nEndLevel ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        std::pair<char,char> prKey((char)nCharType, (char)nPromotion );
        std::pair<int,int> pkData( nStartLevel,nEndLevel );
        // insert
        if( !mapMaxCharSPLevel.insert( std::make_pair( prKey, pkData ) ).second )
        {
            START_LOG( cerr, L"Max Skill LevelChar Type, Promotion이 존재, Char : " << nCharType
                << L", Promotion : " << nPromotion ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"mapMaxCharSPLevel 읽기 완료 : " << mapMaxCharSPLevel.size() ) << END_LOG;
    KLocker lock( m_csMaxCharSPLevel );
    m_mapMaxCharSPLevel.swap( mapMaxCharSPLevel );
    return true;
}

bool KSkillManager::CheckMaxCharSPLevel( IN const char cChar_, IN const char cPromotion_, IN OUT int& nLevel_, IN OUT DWORD& dwSpx )
{
    KLocker lock( m_csMaxCharSPLevel );
    std::map< std::pair<char,char>, std::pair<int,int> >::iterator mit;
    std::pair<char,char> prKey( cChar_, cPromotion_ );
    mit = m_mapMaxCharSPLevel.find( prKey );
    if( mit == m_mapMaxCharSPLevel.end() ) return true; // 해당 캐릭터 전직은 최대 SPX Level이 없음.
    // 해당 charType의 StartLevel,EndLevel의 SPX Table로 체크.
    // 시작 레벨 체크
    if( nLevel_ < mit->second.first ) {
        nLevel_ = mit->second.first;
    }
    // 종료 레벨 체크
    if( nLevel_ >= mit->second.second ) {
        nLevel_ = mit->second.second;
        dwSpx = 0;
        return false;
    }
    return true;
}

void KSkillManager::GetMaxCharSPLevel( OUT std::map< std::pair<char,char>, std::pair<int,int> >& mapData_ )
{
    KLocker lock( m_csMaxCharSPLevel );
    mapData_ = m_mapMaxCharSPLevel;
}

bool KSkillManager::LoadSkillExceptionID( IN KLuaManager& kLuaMng_ )
{
    std::map<char, std::set<int>> mapSkillExceptionID;
    mapSkillExceptionID.clear();
    _JIF( kLuaMng_.BeginTable( "SkillExceptionID" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nCharType;
        std::set<int> setExceptionID;
        _JIF( kLuaMng_.GetValue( "Char", nCharType ) == S_OK, return false );
        _JIF( LoadExceptionID( kLuaMng_, std::string("SkillException"), setExceptionID ), return false );

        // insert
        if( !mapSkillExceptionID.insert( std::make_pair( (char)nCharType, setExceptionID ) ).second )
        {
            START_LOG( cerr, L"Add Skill Point  Promotion이 존재, Char : " << nCharType ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"SkillExceptionID 읽기 완료 : " << mapSkillExceptionID.size() ) << END_LOG;
    KLocker lock( m_csSkillExceptionID );
    m_mapSkillExceptionID.swap( mapSkillExceptionID );
    return true;
}

bool KSkillManager::LoadExceptionID( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setExceptionID_ )
{
    setExceptionID_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setExceptionID_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSkillManager::CheckSkillExceptionID( IN const char cChar_, IN const int nID_ )
{
    KLocker lock( m_csSkillExceptionID );
    std::map<char, std::set<int>>::iterator mit;
    mit = m_mapSkillExceptionID.find( cChar_ );
    if( mit == m_mapSkillExceptionID.end() ) return false;
    bool bResult = std::find( mit->second.begin(), mit->second.end(), nID_ ) != mit->second.end();
    if( false == bResult ) return false;
    return true;
}

bool KSkillManager::LoadSPXTableInfo( IN KLuaManager& kLuaMng_, OUT std::map<int,int>& mapSPXTableInfo_ )
{
    mapSPXTableInfo_.clear();
    _JIF( kLuaMng_.BeginTable( "SPXInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nSPLv = 0;
        int nSPPoint = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        _JIF( kLuaMng_.GetValue( 1, nSPLv ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSPPoint ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        // insert
        if( !mapSPXTableInfo_.insert( std::make_pair( nSPLv, nSPPoint ) ).second )
        {
            START_LOG( cerr, L"SPX 중복된 Lv가 존재, Lv : " << nSPLv ) << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" SPX Table 정보 로드 완료, Count : " << mapSPXTableInfo_.size() ) << END_LOG;

    return true;
}

bool KSkillManager::LoadCharSPXType( IN KLuaManager& kLuaMng_ )
{
    std::map<char, int> mapCharSPXType;
    mapCharSPXType.clear();
    _JIF( kLuaMng_.BeginTable( "CharSkillType" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nChar;
        int nSPXType;

        _JIF( kLuaMng_.GetValue( 1, nChar ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSPXType ) == S_OK, return false );

        // insert
        if( !mapCharSPXType.insert( std::make_pair( (char)nChar, nSPXType ) ).second )
        {
            START_LOG( cerr, L"Char Type이 존재, CharType : " << nSPXType ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"CharSkillType 읽기 완료 : " << mapCharSPXType.size() ) << END_LOG;
    KLocker lock( m_csCharSPXType );
    m_mapCharSPXType.swap( mapCharSPXType );
    return true;
}

bool KSkillManager::GetCharSPXTableType( IN const char cChar_, OUT int& nCharSPXType_ )
{
    KLocker lock( m_csCharSPXType );
    std::map<char, int>::iterator mit;
    mit = m_mapCharSPXType.find( cChar_ );
    if( mit == m_mapCharSPXType.end() ) return false;
    nCharSPXType_ = mit->second;
    return true;
}

bool KSkillManager::GetCharSPXTable( IN int nSPXType_, OUT std::map<int,int>& mapSPXTable_ )
{
    mapSPXTable_.clear();

    KLocker lock( m_csSPXTable );
    std::map< DWORD, std::map<int,int> >::iterator tmit;
    tmit = m_mapSPXTotalTable.find( nSPXType_ );
    if( tmit == m_mapSPXTotalTable.end() ) return false;
    mapSPXTable_ = tmit->second;
    return true;
}

bool KSkillManager::LoadAddCharSPX( KLuaManager& kLuaMng_ )
{
    std::map<std::pair<char,char>, int> mapAddCharSPX;
    _JIF( kLuaMng_.BeginTable( "CharAddSkillPoint" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nCharType;
        int nPromotion;
        int nAddCount;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nPromotion ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nAddCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        std::pair<char,char> prKey((char)nCharType, (char)nPromotion );
        // insert
        if( !mapAddCharSPX.insert( std::make_pair( prKey, nAddCount ) ).second )
        {
            START_LOG( cerr, L"Char Type, Promotion이 존재, Char : " << nCharType
                << L", Promotion : " << nPromotion ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"CharAddSkillPoint 읽기 완료 : " << mapAddCharSPX.size() ) << END_LOG;
    KLocker lock( m_csAddCharSPX );
    m_mapAddCharSPX.swap( mapAddCharSPX );
    return true;
}

bool KSkillManager::GetSkillPointInfo( IN const char cChar_, IN const char cPromotion_, IN const DWORD dwLevel_, OUT int& nSkillPoint_ )
{
    KLocker lock( m_csAddCharSPX );

    int nSPXType = 0;
    std::map<std::pair<char,char>, int>::iterator mitAddSPX;
    std::map<int,int> mapSPXTable;
    std::map<int,int>::iterator mitSPXTable;
    std::pair<char,char> prKey( cChar_, cPromotion_ );

    // 해당 캐릭터의 SPXType 체크
    if( !GetCharSPXTableType( cChar_, nSPXType ) ) {
        // 없으면 에러 처리하고 DefaultSPXType을 넣어주자.
        START_LOG( cerr, L"해당 캐릭터의 SPXType없음 Char : " << cChar_ ) << END_LOG;
    }

    // 가져온 SPXType으로 SPXTable가져오기.
    if( !GetCharSPXTable( nSPXType, mapSPXTable ) ) {
        return false; // default SPXTable도 없으면 에러처리.
    }

    // 각 캐릭터 레벨에 맞는 SPPoint 찾기.
    mitSPXTable = mapSPXTable.find( dwLevel_ );
    if( mitSPXTable == mapSPXTable.end() ) {
        mitSPXTable = --mapSPXTable.end();
        nSkillPoint_ = mitSPXTable->second; // 레벨에 맞는 SPPoint가 없으면 MaxSPPoint Return
    }
    else {
        nSkillPoint_ = mitSPXTable->second;
    }

    // 추가 SPPoint설정이 있는지 체크.
    mitAddSPX = m_mapAddCharSPX.find( prKey );
    if( mitAddSPX != m_mapAddCharSPX.end() ) {
        nSkillPoint_ += mitAddSPX->second;
    }
    return true;
}

bool KSkillManager::GetMaxSPX( IN const char cChar_, IN const char cPromotion_, OUT int& nMaxSkillPoint_ )
{
    KLocker lock( m_csAddCharSPX );

    int nSPXType = 0;
    std::map<std::pair<char,char>, int>::iterator mitAddSPX;
    std::map<int,int> mapSPXTable;
    std::map<int,int>::iterator mitSPXTable;
    std::pair<char,char> prKey( cChar_, cPromotion_ );

    // 해당 캐릭터의 SPXType 체크
    if( !GetCharSPXTableType( cChar_, nSPXType ) ) {
        // 없으면 에러 처리하고 DefaultSPXType을 넣어주자.
        START_LOG( cerr, L"해당 캐릭터의 SPXType없음 Char : " << cChar_ ) << END_LOG;
    }

    // 가져온 SPXType으로 SPXTable가져오기.
    if( !GetCharSPXTable( nSPXType, mapSPXTable ) ) {
        return false; // default SPXTable도 없으면 에러처리.
    }

    // 각 캐릭터 레벨에 맞는 SPPoint 찾기.
    mitSPXTable = --mapSPXTable.end();
    nMaxSkillPoint_ = mitSPXTable->second; // 레벨에 맞는 SPPoint가 없으면 MaxSPPoint Return

    // 추가 SPPoint설정이 있는지 체크.
    mitAddSPX = m_mapAddCharSPX.find( prKey );
    if( mitAddSPX != m_mapAddCharSPX.end() ) {
        nMaxSkillPoint_ += mitAddSPX->second;
    }
    return true;
}

bool KSkillManager::SetNewCharSkillInfo( IN const char& cChar_, IN const int& nLv_, OUT int& nSkillPoint_, OUT int& nMaxSkillPoint_ )
{
    KLocker lock( m_csAddCharSPX );

    int nSPXType = 0;
    char cPromotion = 0;
    DWORD dwLevel = 0;

    std::map<std::pair<char,char>, int>::iterator mitAddSPX;
    std::map<int,int> mapSPXTable;
    std::map<int,int>::iterator mitSPXTable;
    std::pair<char,char> prKey( cChar_, cPromotion );

    // 해당 캐릭터의 SPXType 체크
    if( !GetCharSPXTableType( cChar_, nSPXType ) ) {
        // 없으면 에러 처리하고 DefaultSPXType을 넣어주자.
        START_LOG( cerr, L"해당 캐릭터의 SPXType없음 Char : " << cChar_ ) << END_LOG;
    }

    // 가져온 SPXType으로 SPXTable가져오기.
    if( !GetCharSPXTable( nSPXType, mapSPXTable ) ) {
        return false; // default SPXTable도 없으면 에러처리.
    }

    // 각 캐릭터 레벨에 맞는 SPPoint 찾기.
    if ( !mapSPXTable.empty() ) {
        mitSPXTable = mapSPXTable.find( nLv_ );
        if ( mitSPXTable != mapSPXTable.end() ) {
            nSkillPoint_ = mitSPXTable->second;
        }
        else {
            nSkillPoint_ = mapSPXTable.begin()->second;
        }
    }

    // 추가 SPPoint설정이 있는지 체크.
    mitAddSPX = m_mapAddCharSPX.find( prKey );
    if( mitAddSPX != m_mapAddCharSPX.end() ) {
        nSkillPoint_ += mitAddSPX->second;
    }

    // MaxSkillSPX 세팅.
    GetMaxSPX( cChar_, cPromotion, nMaxSkillPoint_ );

    return true;
}

bool KSkillManager::GetCharStartSPX( IN const int nSPXType_, OUT DWORD& dwStartSPX_ )
{
    KLocker lock( m_csAddCharSPX );

    std::map<int,int> mapSPXTable;
    std::map<int,int>::iterator mitSPXTable;

    // 가져온 SPXType으로 SPXTable가져오기.
    if( !GetCharSPXTable( nSPXType_, mapSPXTable ) ) {
        return false; // default SPXTable도 없으면 에러처리.
    }

    // 처음 시작 레벨 SPX 값 세팅.
    mitSPXTable = mapSPXTable.begin();
    dwStartSPX_ = mitSPXTable->second;

    return true;
}

bool KSkillManager::LoadExceptionCharTypeList( IN KLuaManager& kLuaMng_ )
{
    std::set<char> setExceptionCharType;
    setExceptionCharType.clear();

    _JIF( kLuaMng_.BeginTable( "CharExceptionType" ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setExceptionCharType.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"CharExceptionType 읽기 완료 : " << setExceptionCharType.size() ) << END_LOG;

    KLocker lock( m_csExceptionCharType );
    m_setExceptionCharType.swap( setExceptionCharType );

    return true;
}

bool KSkillManager::CheckExceptionCharType( IN const char cChar_ )
{
    KLocker lock( m_csExceptionCharType );
    std::set<char>::iterator sit;
    sit = m_setExceptionCharType.find( cChar_ );
    if( sit == m_setExceptionCharType.end() ) return false;
    return true;
}

bool KSkillManager::IsResetSkillItem( IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csConfig);
    return (m_sSkillConfig.m_setResetSkillItem.find( ItemID_ ) !=  m_sSkillConfig.m_setResetSkillItem.end());
}

bool KSkillManager::LoadAPCharAddSlotOpenInfo( KLuaManager& kLuaMng_ )
{
    std::map<GCITEMID,std::set<int>>     mapAPCharAddSlotOpenInfo;
    int                                  nMaxCharSlotNum;
    mapAPCharAddSlotOpenInfo.clear();

    _JIF( kLuaMng_.BeginTable( "APCharAddSlotOpenItem" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxCharSlotNum", nMaxCharSlotNum ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        GCITEMID ItemID;
        std::set<int> setCharType;
        _JIF( kLuaMng_.GetValue( "ItemID", ItemID ) == S_OK, return false );
        _JIF( LoadSetInfo( kLuaMng_, std::string("CharType"), setCharType ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapAPCharAddSlotOpenInfo.insert( std::make_pair( ItemID, setCharType ) ).second ) {
            START_LOG( cerr, L"이미 존재하는 Item ID : " << ItemID )
                << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"APCharAddSlotOpenItem 읽기 완료 : " << mapAPCharAddSlotOpenInfo.size() ) << END_LOG;
    KLocker lock( m_csAPCharAddSlotOpen );
    m_nMaxCharSlotNum = nMaxCharSlotNum;
    m_mapAPCharAddSlotOpenInfo.swap( mapAPCharAddSlotOpenInfo );
    return true;
}

bool KSkillManager::CheckAPCharAddSlotOpen( IN const GCITEMID& ItemID_, IN const int& nCharType_ )
{
    std::map<GCITEMID,std::set<int>>::const_iterator cmit;
    std::set<int>::const_iterator csit;
    cmit = m_mapAPCharAddSlotOpenInfo.find( ItemID_ );
    if( cmit == m_mapAPCharAddSlotOpenInfo.end() ) {
        return false;
    }
    csit = cmit->second.find( nCharType_ );
    if( csit == cmit->second.end() ) {
        return false;
    }
    return true;
}

bool KSkillManager::LoadSetInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setDungeonInfo_ )
{
    setDungeonInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setDungeonInfo_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSkillManager::LoadMissionRewardSkillInfo( KLuaManager& kLuaMng_ )
{
    std::map<DWORD, int>                 mapSkillMissionInfo; // 클라이언트 전달용.
    std::map<DWORD, std::pair<int,DWORD>> mapDBUpdateSkillMissionInfo;
    mapSkillMissionInfo.clear();
    mapDBUpdateSkillMissionInfo.clear();

    _JIF( kLuaMng_.BeginTable( "MissionRewardSkill" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nSkillID;
        DWORD dwMissionID;
        DWORD dwType; // 0 : GP, 1 : Cash
        std::pair<int,DWORD> prData;
        _JIF( kLuaMng_.GetValue( 1, dwMissionID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSkillID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapSkillMissionInfo.insert( std::make_pair( dwMissionID, nSkillID ) ).second ) {
            START_LOG( cerr, L"MissionID 존재, MissionID : " << dwMissionID
                << L", SkillID : " << nSkillID )
                << END_LOG;
            return false;
        }

        prData.first = nSkillID;
        prData.second = dwType;
        if( !mapDBUpdateSkillMissionInfo.insert( std::make_pair( dwMissionID, prData ) ).second ) {
            START_LOG( cerr, L"MissionID 존재, MissionID : " << dwMissionID
                << L", SkillID : " << nSkillID )
                << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"MissionRewardSkill 읽기 완료 : " << mapSkillMissionInfo.size() ) << END_LOG;

    KLocker lock( m_csMissionRewardSkill );
    m_mapSkillMissionInfo.swap( mapSkillMissionInfo );
    m_mapDBUpdateSkillMissionInfo.swap( mapDBUpdateSkillMissionInfo );
    return true;
}

bool KSkillManager::CheckMissionRewardSkill( IN const int& nSkillID_, OUT std::vector<DWORD>& vecMissionInfo_ )
{
    vecMissionInfo_.clear();
    std::map<DWORD, int>::iterator mit;

    KLocker lock( m_csMissionRewardSkill );
    for( mit = m_mapSkillMissionInfo.begin(); mit != m_mapSkillMissionInfo.end(); ++mit ) {
        if( mit->second == nSkillID_ ) {
            vecMissionInfo_.push_back( mit->first );
        }
    }
    if( vecMissionInfo_.empty() ) {
        return false;
    }
    return true;
}

bool KSkillManager::CheckRewardSkillMission( IN const DWORD& dwMissionID_, OUT int& nSkillID_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock( m_csMissionRewardSkill );
    mit = m_mapSkillMissionInfo.find( dwMissionID_ );
    if( mit == m_mapSkillMissionInfo.end() ) {
        return false;
    }
    nSkillID_ = mit->second;
    return true;
}

void KSkillManager::GetSkillMissionInfo( OUT std::map<DWORD, int>& mapSkillMissionInfo_ )
{
    mapSkillMissionInfo_.clear();

    KLocker lock( m_csMissionRewardSkill );
    mapSkillMissionInfo_ = m_mapSkillMissionInfo;
}

void KSkillManager::GetDBUpdateSkillMissionInfo( OUT std::map<DWORD, std::pair<int,DWORD>>& mapDBUpdateSkillMissionInfo_ )
{
    mapDBUpdateSkillMissionInfo_.clear();

    KLocker lock( m_csMissionRewardSkill );
    mapDBUpdateSkillMissionInfo_ = m_mapDBUpdateSkillMissionInfo;
}

bool KSkillManager::GetSkillMissionID( IN const DWORD& dwMissionID_, OUT int& nSkillID_ )
{
    std::map<DWORD, int>::iterator mit;

    KLocker lock( m_csMissionRewardSkill );
    mit = m_mapSkillMissionInfo.find( dwMissionID_ );
    if( mit != m_mapSkillMissionInfo.end() ) {
        nSkillID_ = mit->second;
        return true;
    }
    return false;
}

bool KSkillManager::LoadAbilitySkillInfo( KLuaManager& kLuaMng_ )
{
    std::map<char, std::set<int>>        mapAbilitySkillInfo;
    mapAbilitySkillInfo.clear();

    _JIF( kLuaMng_.BeginTable( "CharAbilitySkillInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        int nChar;
        std::set<int> setSkill;
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( LoadSetInfo( kLuaMng_, std::string("SkillInfo"), setSkill ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapAbilitySkillInfo.insert( std::make_pair( (char)nChar, setSkill ) ).second ) {
            START_LOG( cerr, L"이미 CharType : " << nChar )
                << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"CharAbilitySkillInfo 읽기 완료 : " << mapAbilitySkillInfo.size() ) << END_LOG;

    KLocker lock( m_csAbilitySkill );
    m_mapAbilitySkillInfo.swap( mapAbilitySkillInfo );
    return true;
}

bool KSkillManager::CheckAbilitySkillChar( IN const char& cChar_ )
{
    std::map<char, std::set<int>>::const_iterator cmit;

    KLocker lock( m_csAbilitySkill );
    cmit = m_mapAbilitySkillInfo.find( cChar_ );
    if( cmit == m_mapAbilitySkillInfo.end() ) {
        return false;
    }
    return true;
}

bool KSkillManager::CheckAbilitySkill( IN const char& cChar_, IN const int& nSkillID_ )
{
    std::map<char, std::set<int>>::const_iterator cmit;
    std::set<int>::const_iterator csit;

    KLocker lock( m_csAbilitySkill );
    cmit = m_mapAbilitySkillInfo.find( cChar_ );
    if( cmit == m_mapAbilitySkillInfo.end() ) {
        return false;
    }
    csit = cmit->second.find( nSkillID_ );
    if( csit == cmit->second.end() ) {
        return false;
    }
    return true;
}

bool KSkillManager::CheckExceptionSkill( IN const int& nSkillID_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock( m_csMissionRewardSkill );

    for( mit = m_mapSkillMissionInfo.begin(); mit != m_mapSkillMissionInfo.end(); ++mit ) {
        if( mit->second == nSkillID_ ) {
            return true;
        }
    }

    return false;
}

bool KSkillManager::LoadSkillTreeOpenCharInfo( KLuaManager& kLuaMng_ )
{
    std::set<std::pair<std::string, std::string>> setSkillTreeOpenCharInfo;
    setSkillTreeOpenCharInfo.clear();

    _JIF( kLuaMng_.BeginTable( "SkillTreeOpenCharInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        std::pair<std::string, std::string> prData;
        _JIF( kLuaMng_.GetValue( 1, prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        setSkillTreeOpenCharInfo.insert( prData );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"SkillTreeOpenCharInfo 읽기 완료 : " << setSkillTreeOpenCharInfo.size() ) << END_LOG;

    KLocker lock( m_csSkillTreeOpenChar );
    m_setSkillTreeOpenCharInfo.swap( setSkillTreeOpenCharInfo );
    return true;
}

bool KSkillManager::LoadOldSkillTreeUseCharacter( KLuaManager& kLuaMng_ )
{
    std::set<char> setSkillUseCharacter;
    setSkillUseCharacter.clear();

    _JIF( kLuaMng_.BeginTable( "OldSkillTreeUseCharacter" ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setSkillUseCharacter.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    KLocker lock( m_csOldSkillUseCharacter );
    m_setOldSkillUseCharacter.swap( setSkillUseCharacter );

    return true;
}

void KSkillManager::GetOldSkillTreeUseCharacter( OUT std::set<char>& setChar_ )
{
    setChar_.clear();

    KLocker lock( m_csOldSkillUseCharacter );

    setChar_ = m_setOldSkillUseCharacter;
}

void KSkillManager::GetSkillTreeOpenCharInfo( OUT std::set<std::pair<std::string, std::string>>& setSkillTreeOpenCharInfo_ )
{
    setSkillTreeOpenCharInfo_.clear();

    KLocker lock( m_csSkillTreeOpenChar );
    setSkillTreeOpenCharInfo_ = m_setSkillTreeOpenCharInfo;
}

bool KSkillManager::IsOldSkillTreeUseCharacter( IN const char cCharacter_ )
{
    KLocker lock( m_csOldSkillUseCharacter );
    
    std::set<char>::const_iterator sit;

    sit = m_setOldSkillUseCharacter.find( cCharacter_ );
    if ( sit != m_setOldSkillUseCharacter.end() ) {
        return true;
    }
    return false;
}

bool KSkillManager::IsSkillOpenKeyUseSkill( IN const int& nSkillID_ )
{
    std::map<DWORD, int>::iterator mit;

    KLocker lock( m_csMissionRewardSkill );
    for( mit = m_mapSkillMissionInfo.begin(); mit != m_mapSkillMissionInfo.end(); ++mit ) {
        if( mit->second == nSkillID_ ) {
            return true;
        }
    }

    return false;
}

bool KSkillManager::LoadSkillScroolInfo( KLuaManager& kLuaMng_ )
{
    std::map<GCITEMID, int> mapSkillScroolInfo;
    mapSkillScroolInfo.clear();

    _JIF( kLuaMng_.BeginTable( "SkillScroolInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        GCITEMID ItemID;
        int nSkillID;
        _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSkillID ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapSkillScroolInfo.insert( std::make_pair( ItemID, nSkillID ) ).second ) {
            START_LOG( cerr, L"이미 존재하는 ItemID : " << ItemID )
                << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"SkillScroolInfo 읽기 완료 : " << mapSkillScroolInfo.size() ) << END_LOG;

    KLocker lock( m_csSkillTreeOpenChar );
    m_mapSkillScroolInfo.swap( mapSkillScroolInfo );
    return true;
}

bool KSkillManager::GetSkillScroolInfo( IN const GCITEMID& dwItemID_, OUT int& nSkillID_ )
{
    std::map<GCITEMID, int>::iterator mit;

    KLocker lock( m_csSkillTreeOpenChar );
    mit = m_mapSkillScroolInfo.find( dwItemID_ );
    if( mit == m_mapSkillScroolInfo.end() ) {
        return false;
    }
    nSkillID_ = mit->second;
    return true;
}