#include "CnClientContentsManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterServer.h"
#include "Log4.h"

ImplementSingleton( KCnClientContentsManager );
ImplOstmOperatorW2A( KCnClientContentsManager );
NiImplementRootRTTI( KCnClientContentsManager );

KCnClientContentsManager::KCnClientContentsManager(void)
: m_nMaxLevel( 85 )
,m_bSmartPanelMailListEnable(false)
,m_SlotItem(0)
{
    m_mapChampionRatio.clear();
    m_vecEventOpenInfo.clear();
    m_vecSmartPanelMailList.clear();
    m_mapChannelImageDownloadURLList.clear();
}

KCnClientContentsManager::~KCnClientContentsManager(void)
{
}

ImplToStringW( KCnClientContentsManager )
{
    KLocker lock( m_csContents );
    return START_TOSTRINGW
        << TOSTRINGW( m_nMaxLevel )
        << TOSTRINGW( m_vecChannelCategories.size() )
        << TOSTRINGW( m_vecCategoryModes.size() )
        << TOSTRINGW( m_vecCategoryInfo.size() )
        << TOSTRINGW( m_mapCharactes.size() )
        << TOSTRINGW( m_vecCharSP4OpenInfo.size() )
        << TOSTRINGW( m_vecChannelNewsImageNumber.size() )
        << TOSTRINGW( m_mapLoadingImageName.size() )
        << TOSTRINGW( m_mapClientScriptName.size() )
        << TOSTRINGW( m_vecExceptionMotionID.size() )
        << TOSTRINGW( m_vecEventBannerInfo.size() )
        << TOSTRINGW( m_setCharSkillEnable.size() )
        << TOSTRINGW( m_setDLLBlackList.size() )
        << TOSTRINGW( m_mapChampionRatio.size() )
        << TOSTRINGW( m_vecEventOpenInfo.size() )
        << TOSTRINGW( m_vecSmartPanelMailList.size() )
        << TOSTRINGW( m_mapChannelImageDownloadURLList.size() );
}

bool KCnClientContentsManager::LoadScript()
{
    std::vector< std::pair< int, std::vector< int > > >                     vecChannelCategories;
    std::vector< std::pair< int, std::vector< int > > >                     vecCategoryModes;
    std::vector <std::pair< std::pair< int, bool >, std::vector< int > > >  vecCategoryInfo;
    std::map< int, std::set< char > >                                       mapCharactes;
    std::vector< int >                                                      vecCharSP4OpenInfo;
    int                                                                     nMaxLevel;
    std::vector< int >                                                      vecChannelNewsImageNumber;
    std::map< int, std::vector< std::wstring > >                            mapLoadingImageName;
    std::map< int, std::wstring >                                           mapClientScriptName;
    std::vector< int >                                                      vecExceptionMotionID;
    std::map< int, std::vector< std::wstring > >                            mapPVPLoadingImageName;
    CnKairo                                                                 cnKairo;
    std::vector< std::pair< std::wstring, int > >                           vecEventBannerInfo;
    std::set< DWORD >                                                       setCharSkillEnable;
    std::map< int, std::wstring >                                           mapServerBenefitMsg;
    std::set< std::wstring >                                                setDLLBlackList;
    std::map< int, float >                                                  mapChampionRatio;
    std::vector< KEventOpenInfo >                                           vecEventOpenInfo;
    std::map< int, KMonsterAbility >                                        mapMonsterAbility;
    std::map< int, DWORD >                                                  mapCharOpenType;
    std::map< int, GCITEMID >                                               mapCharCashGoods;
    std::vector< int >                                                      vecDisableMenuList;
    bool                                                                bSmartPanelMailListEnable;
    std::vector<std::wstring>                                           vecSmartPanelMailList;
    std::map<int, std::wstring>                                         mapChannelImageDownloadURLList;
    GCITEMID                                                            SlotItem;

    KLuaManager kLuaMng;
    _JIF( kLuaMng.DoFile( "InitClientContents.lua" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxLevel", nMaxLevel ) == S_OK, return false );
    _JIF( LoadVecPair( kLuaMng, std::string("Channel_Category"), vecChannelCategories ), return false );
    _JIF( LoadVecPair( kLuaMng, std::string("Category_Mode"), vecCategoryModes ), return false );
    _JIF( LoadMapInfo( kLuaMng, vecCategoryInfo ), return false );
    _JIF( LoadCharacterInfo( kLuaMng, mapCharactes ), return false );
    _JIF( LoadCharacterSP4OpenInfo( kLuaMng, vecCharSP4OpenInfo ), return false );

    _JIF( LoadChannelNews( kLuaMng, vecChannelNewsImageNumber ), return false );
    _JIF( LoadLoadingImage( kLuaMng, mapLoadingImageName, std::string("LoadingImageFileName"), cnKairo  ), return false );
    _JIF( LoadPVPLoadingImage( kLuaMng, mapPVPLoadingImageName, std::string("PVPLoadingImageFileName") ), return false );
    _JIF( LoadClientScriptName( kLuaMng, mapClientScriptName ), return false );
    _JIF( LoadExceptionMotionID( kLuaMng, vecExceptionMotionID ), return false );
    _JIF( LoadEventBannerInfo( kLuaMng, vecEventBannerInfo ), return false );
    _JIF( LoadCharSkillInfo( kLuaMng, setCharSkillEnable ), return false );

    if ( LoadBenefitMsg( kLuaMng, mapServerBenefitMsg ) == false ) {
        START_LOG( cwarn, L"서버별 혜택 메세지 설정 실패" ) << END_LOG;
    }

    _JIF( LoadDLLBlackList( kLuaMng, setDLLBlackList ), return false );
    _JIF( LoadChampionRatio( kLuaMng, mapChampionRatio ), return false );
    _JIF( LoadEvnetOpenInfo( kLuaMng, vecEventOpenInfo ), return false );
    _JIF( LoadEventMonsterAbility( kLuaMng, mapMonsterAbility ), return false );
    _JIF( LoadCharOpenType( kLuaMng, mapCharOpenType ), return false );
    _JIF( LoadCharCashGoods( kLuaMng, mapCharCashGoods ), return false );
    _JIF( LoadDisableMenuList( kLuaMng, vecDisableMenuList ), return false );
    _JIF( kLuaMng.GetValue( "SmartPanelMailListEnable", bSmartPanelMailListEnable ) == S_OK, return false );
    _JIF( LoadSmartPanelMailList( kLuaMng, vecSmartPanelMailList ), return false );
    _JIF( LoadChannelImageDownloadURLList( kLuaMng, mapChannelImageDownloadURLList ), return false );
    _JIF( kLuaMng.GetValue( "SlotItemNum", SlotItem ) == S_OK, return false );

    START_LOG( cerr, L"클라이언트 컨텐츠 오픈 정보 로드 완료..")
        << BUILD_LOG( nMaxLevel )
        << BUILD_LOG( vecChannelCategories.size() )
        << BUILD_LOG( vecCategoryModes.size() )
        << BUILD_LOG( vecCategoryInfo.size() )
        << BUILD_LOG( mapCharactes.size() )
        << BUILD_LOG( vecCharSP4OpenInfo.size() )
        << BUILD_LOG( vecChannelNewsImageNumber.size() )
        << BUILD_LOG( mapLoadingImageName.size() )
        << BUILD_LOG( mapClientScriptName.size() )
        << BUILD_LOG( vecExceptionMotionID.size() )
        << BUILD_LOG( vecEventBannerInfo.size() )
        << BUILD_LOG( setCharSkillEnable.size() )
        << BUILD_LOG( mapServerBenefitMsg.size() )
        << BUILD_LOG( setDLLBlackList.size() )
        << BUILD_LOG( mapChampionRatio.size() )
        << BUILD_LOG( vecEventOpenInfo.size() )
        << BUILD_LOG( mapMonsterAbility.size() )
        << BUILD_LOG( mapCharOpenType.size() )
        << BUILD_LOG( mapCharCashGoods.size() )
        << BUILD_LOG( vecDisableMenuList.size() )
        << BUILD_LOG( vecSmartPanelMailList.size() )
        << BUILD_LOG( mapChannelImageDownloadURLList.size() )
        << BUILD_LOG( SlotItem )
        << END_LOG;

    KLocker lock( m_csContents );
    m_vecChannelCategories.swap( vecChannelCategories );
    m_vecCategoryModes.swap( vecCategoryModes );
    m_vecCategoryInfo.swap( vecCategoryInfo );
    m_mapCharactes.swap( mapCharactes );
    m_vecCharSP4OpenInfo.swap( vecCharSP4OpenInfo );
    m_nMaxLevel = nMaxLevel;
    m_vecChannelNewsImageNumber.swap( vecChannelNewsImageNumber );
    m_mapLoadingImageName.swap( mapLoadingImageName );
    m_mapClientScriptName.swap( mapClientScriptName );
    m_vecExceptionMotionID.swap( vecExceptionMotionID );
    m_mapPVPLoadingImageName.swap( mapPVPLoadingImageName );
    m_CnKairoInfo = cnKairo;
    m_vecEventBannerInfo.swap( vecEventBannerInfo );
    m_setCharSkillEnable.swap( setCharSkillEnable );
    m_mapServerBenefitMsg.swap( mapServerBenefitMsg );
    m_setDLLBlackList.swap( setDLLBlackList );
    m_mapChampionRatio.swap( mapChampionRatio );
    m_vecEventOpenInfo.swap( vecEventOpenInfo );
    m_mapMonsterAbility.swap( mapMonsterAbility );
    m_mapCharOpenType.swap( mapCharOpenType );
    m_mapCharCashGoods.swap( mapCharCashGoods );
    m_vecDisableMenuList.swap(vecDisableMenuList );
    m_bSmartPanelMailListEnable = bSmartPanelMailListEnable;
    m_vecSmartPanelMailList.swap( vecSmartPanelMailList );
    m_mapChannelImageDownloadURLList.swap( mapChannelImageDownloadURLList );
    m_SlotItem = SlotItem;

    return true;
}

void KCnClientContentsManager::LoadNumSequence( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecSequence_ )
{
    vecSequence_.clear();
    for( int i = 1 ; ; ++i )
    {
        int nSequence = -1;
        if( kLuaMng_.GetValue( i, nSequence ) != S_OK ) return;
        vecSequence_.push_back( nSequence );
    }
}

bool KCnClientContentsManager::LoadVecPair( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<std::pair<int, std::vector<int> > >& vec_pair_ )
{
    vec_pair_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nKeyID = -1;
        std::vector<int> vecIDs;
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nKeyID ) == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( 2 ) == S_OK, return false );
        LoadNumSequence( kLuaMng_, vecIDs );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vec_pair_.push_back( std::make_pair(nKeyID, vecIDs) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnClientContentsManager::LoadMapInfo( IN KLuaManager& kLuaMng_, OUT std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >& vecModeInfo_ )
{
    vecModeInfo_.clear();
    _JIF( kLuaMng_.BeginTable( "Category_Info" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        std::pair<int,bool> prKey( -1, false );
        std::vector<int> vecMaps;
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, prKey.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prKey.second ) == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( 3 ) == S_OK, return false );
        LoadNumSequence( kLuaMng_, vecMaps );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecModeInfo_.push_back( std::make_pair(prKey, vecMaps) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnClientContentsManager::LoadCharacterInfo( IN KLuaManager& kLuaMng_, OUT std::map<int,std::set<char>>& mapCharactes_ )
{
    mapCharactes_.clear();
    _JIF( kLuaMng_.BeginTable( "Character_Open_Info" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        int nCharType = -1;
        std::set<char> setCharPromotion;
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( LoadSetInfo( kLuaMng_, std::string("CharPromotion"), setCharPromotion ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        if( !mapCharactes_.insert( std::make_pair(nCharType, setCharPromotion) ).second ) {
            START_LOG( cerr, L"중복된 캐릭터가 있음.. ID : " << nCharType ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnClientContentsManager::LoadCharacterSP4OpenInfo( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecCharSP4OpenInfo_ )
{
    vecCharSP4OpenInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "Character_SP4_Open_Info" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        int iCharType = 0;
        if ( kLuaMng_.GetValue( i, iCharType ) != S_OK )
            break;

        vecCharSP4OpenInfo_.push_back( iCharType );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}


bool KCnClientContentsManager::GetContentsInfo( OUT std::vector<std::pair<int, std::vector<int> > >& vecChannelCategories_,
                                                OUT std::vector<std::pair<int, std::vector<int> > >& vecCategoryModes_,
                                                OUT std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >& vecCategoryInfo_,
                                                OUT std::map<int,std::set<char>>& mapCharactes_ ,
                                                OUT std::vector<int>& vecCharSP4OpenInfo_,
												OUT std::map<int,DWORD>& mapCharOpenType_,
												OUT std::map<int,GCITEMID>& mapCharCashGoods_,
                                                OUT std::vector<int>& vecDisableMenuList_,
                                                OUT GCITEMID& SlotItem_
                                                )
{
    vecChannelCategories_.clear();
    vecCategoryModes_.clear();
    vecCategoryInfo_.clear();
    mapCharactes_.clear();
    vecCharSP4OpenInfo_.clear();
    mapCharOpenType_.clear();
    mapCharCashGoods_.clear();
    vecDisableMenuList_.clear();

    KLocker lock( m_csContents );
    if ( m_vecChannelCategories.empty() && m_vecCategoryModes.empty() && m_vecCategoryInfo.empty()
        && m_mapCharactes.empty() && m_vecCharSP4OpenInfo.empty() && m_mapCharOpenType.empty()
        && m_mapCharCashGoods.empty() && m_vecDisableMenuList.empty() ) {
        return false;
    }

    vecChannelCategories_   = m_vecChannelCategories;
    vecCategoryModes_       = m_vecCategoryModes;
    vecCategoryInfo_        = m_vecCategoryInfo;
    mapCharactes_           = m_mapCharactes;
    vecCharSP4OpenInfo_     = m_vecCharSP4OpenInfo;
    mapCharOpenType_        = m_mapCharOpenType;
    mapCharCashGoods_       = m_mapCharCashGoods;
    vecDisableMenuList_     = m_vecDisableMenuList;
    SlotItem_               = m_SlotItem;
    return true;
}

bool KCnClientContentsManager::LoadChannelNews( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecNumber_ )
{
    int nNumber = 0;
    vecNumber_.clear();

    _JIF( kLuaMng_.BeginTable( "ChannelNewsImageNumber" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.GetValue( i, nNumber ) != S_OK )
            break;

        vecNumber_.push_back( nNumber );
        START_LOG( clog, L"Loading Channel News. Image Number : " << nNumber ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadLoadingImage( IN KLuaManager& kLuaMng_, OUT std::map<int, std::vector<std::wstring> >& mapFileName_, 
                                                IN std::string& LoadingImageTableName_, OUT CnKairo& cnKairo_ )
{
    std::string strTmp;
    std::vector<std::wstring> prStr;
    int nIndex = 0;
    mapFileName_.clear();

    _JIF( kLuaMng_.BeginTable( LoadingImageTableName_.c_str() ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        float fProb = 0.f;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
		strTmp.clear();
        prStr.clear();
        _JIF( kLuaMng_.GetValue( 1, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.GetValue( 2, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
		_JIF( kLuaMng_.GetValue( 3, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.GetValue( 4, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.GetValue( 5, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapFileName_[nIndex] = prStr;
        _JIF( cnKairo_.SetImageNum( nIndex, fProb ), return false );

        START_LOG( clog, L"Loading Image Index : " << nIndex << L", File Name : " << prStr[ 0 ] << L", " << prStr[ 1 ] << L", " << prStr[ 2 ] << L", " << prStr[ 3 ] ) << END_LOG;

        ++nIndex;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadClientScriptName( IN KLuaManager& kLuaMng_, OUT std::map<int, std::wstring >& mapScriptName_ )
{
    std::string strTmp;
    int nIndex = 0;
    mapScriptName_.clear();

    _JIF( kLuaMng_.BeginTable( "ClientScriptName" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nIndex ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, strTmp ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapScriptName_[nIndex] = ( KncUtil::toWideString( strTmp ) );
        START_LOG( clog, L"Client Script Index : " << nIndex << L",File Name : " << KncUtil::toWideString( strTmp  )) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadBenefitMsg( IN KLuaManager& kLuaMng_, OUT std::map<int, std::wstring >& mapScriptName_ )
{
    std::string strTmp;
    int nIndex = 0;
    mapScriptName_.clear();

    _JIF( kLuaMng_.BeginTable( "BenefitMsg" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nIndex ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, strTmp ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapScriptName_[nIndex] = ( KncUtil::toWideString( strTmp ) );
        START_LOG( clog, L"Server Benefit Message, ServerPart : " << nIndex << L", Msg : " << KncUtil::toWideString( strTmp ) ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetChannelNewsImageNumber( OUT std::vector<int>& vecNumber_ )
{
    KLocker lock( m_csContents );
    vecNumber_ = m_vecChannelNewsImageNumber;
}

void KCnClientContentsManager::GetLoadingImage( OUT std::vector<std::wstring>& vecFileName_ )
{
    vecFileName_.clear();

    KLocker lock( m_csContents );

    if ( m_mapLoadingImageName.empty() ) return;

    int nIdx = 0;
    DoImageGamble( nIdx );

    vecFileName_ = m_mapLoadingImageName[nIdx];
}

void KCnClientContentsManager::GetPVPLoadingImage( OUT std::map< int, std::vector<std::wstring> >& mapFileName_ )
{
    mapFileName_.clear();
    
    KLocker lock( m_csContents );

    if ( m_mapPVPLoadingImageName.empty() ) return;
    
    mapFileName_ = m_mapPVPLoadingImageName;
}


void KCnClientContentsManager::GetClientScriptName( OUT std::map<int, std::wstring >& mapScriptName_ )
{
    KLocker lock( m_csContents );
    mapScriptName_ = m_mapClientScriptName;
}

bool KCnClientContentsManager::LoadExceptionMotionID( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecNumber_ )
{
    int nNumber = 0;
    vecNumber_.clear();

    _JIF( kLuaMng_.BeginTable( "ExceptionMotionID" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.GetValue( i, nNumber ) != S_OK )
            break;

        vecNumber_.push_back( nNumber );
        START_LOG( clog, L"Loading Exception Motions. Motion Number : " << nNumber ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetExceptionMotionID( OUT std::vector<int>& vecNumber_ )
{
    KLocker lock( m_csContents );
    vecNumber_ = m_vecExceptionMotionID;
}

bool KCnClientContentsManager::DoImageGamble( OUT int& nNum_ )
{
    nNum_ = 0; // Init
    bool bSuccess;
    {
        KLocker lock( m_csContents );
        bSuccess = m_CnKairoInfo.Do( nNum_ );
    }
    if( bSuccess ) return true;

    if( nNum_ == KLottery::CASE_BLANK ) {
        nNum_ = 0;
        return false;
    }
    return false;
}

bool KCnClientContentsManager::LoadPVPLoadingImage( IN KLuaManager& kLuaMng_, OUT std::map< int, std::vector< std::wstring > >& mapFileName_,
                                                IN std::string& LoadingImageTableName_ )
{
    std::string strTmp;
    std::vector< std::wstring > prStr;
    int nIndex = 0;
    mapFileName_.clear();

    _JIF( kLuaMng_.BeginTable( LoadingImageTableName_.c_str() ) == S_OK, return false );
    for (int i = 1; ; ++i ) {
        strTmp.clear();
        prStr.clear();

        prStr.clear();

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.GetValue( 2, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.GetValue( 3, strTmp ) == S_OK, return false );
        prStr.push_back( KncUtil::toWideString( strTmp ) );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapFileName_[ nIndex ] = prStr;

        START_LOG( clog, L"PVPLoading Image Index : " << nIndex << L", File Name : " << prStr[ 0 ] << L", " << prStr[ 1 ] << L", " << prStr[ 2 ] ) << END_LOG;

        ++nIndex;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadEventBannerInfo( IN KLuaManager& kLuaMng_, OUT std::vector< std::pair<std::wstring, int> >& vecEventBannerInfo_ )
{
    std::string strTmp;
    int nIndex = 0;
    vecEventBannerInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "EVENT_BANNER_TEXTURE" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        std::pair<std::wstring,int> prData;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, strTmp ) == S_OK, return false );
        prData.first = KncUtil::toWideString( strTmp );
        _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecEventBannerInfo_.push_back( prData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetEventBannerInfo( OUT std::vector< std::pair<std::wstring, int> >& vecEventBannerInfo_ )
{
    KLocker lock( m_csContents );

    vecEventBannerInfo_ = m_vecEventBannerInfo;
}

bool KCnClientContentsManager::LoadCharSkillInfo( IN KLuaManager& kLuaMng_, OUT std::set<DWORD>& setCharSkillEnable_ )
{
    setCharSkillEnable_.clear();
    int nIndex = 0;

    _JIF( kLuaMng_.BeginTable( "CHARSKILL_ENABLE" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        DWORD dwCharType = 0;
         if ( kLuaMng_.GetValue( i, dwCharType ) != S_OK )
             break;

        setCharSkillEnable_.insert( dwCharType );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetCharSkillInfo( OUT std::set< DWORD >& setCharSkillEnable_ )
{
    setCharSkillEnable_.clear();

    KLocker lock( m_csContents );
    setCharSkillEnable_ = m_setCharSkillEnable;
}

void KCnClientContentsManager::SetServerBenefitMsg( IN OUT std::vector< KServerInfo >& kServerList_ )
{
    std::vector< KServerInfo >::iterator vit;
    std::map<int, std::wstring >::const_iterator mit;

    for ( vit = kServerList_.begin(); vit != kServerList_.end(); ++vit ) {

        mit = m_mapServerBenefitMsg.find( vit->m_iServerPart );

        if ( mit != m_mapServerBenefitMsg.end() ) {
            vit->m_strBenefitMsg = mit->second;
        }
    }
}

bool KCnClientContentsManager::LoadDLLBlackList( IN KLuaManager& kLuaMng_, OUT std::set<std::wstring >& setBlackList_ )
{
    int nIndex = 0;
    setBlackList_.clear();

    _JIF( kLuaMng_.BeginTable( "DLLBlackList" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::string strDLLName;
        if ( kLuaMng_.GetValue( 1, strDLLName ) != S_OK ) {
            break;
        }
        setBlackList_.insert( KncUtil::toWideString( strDLLName ) );
        START_LOG( clog, L"DLL Black List : " << KncUtil::toWideString( strDLLName ) ) << END_LOG;

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetDLLBlackList( OUT std::set<std::wstring >& setBlackList_ )
{
    KLocker lock( m_csContents );
    setBlackList_ = m_setDLLBlackList;
}

bool KCnClientContentsManager::LoadChampionRatio( IN KLuaManager& kLuaMng_, OUT std::map<int,float>& mapChampionRatio_ )
{
    int nIndex = 0;
    float fRatio = 0.f;
    mapChampionRatio_.clear();

    _JIF( kLuaMng_.BeginTable( "ChampionAbilityRate" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nIndex ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapChampionRatio_[nIndex] = fRatio;
        START_LOG( clog, L" RatioIndex : " << nIndex << L",Ratio : " << fRatio ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetChampionRatio( OUT std::map< int, float >& mapChampionRatio_ )
{
    mapChampionRatio_.clear();

    KLocker lock( m_csContents );
    mapChampionRatio_ = m_mapChampionRatio;
}

bool KCnClientContentsManager::LoadEvnetOpenInfo( IN KLuaManager& kLuaMng_, OUT std::vector<KEventOpenInfo>& vecEventOpenInfo_ )
{
    vecEventOpenInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "EventOpenInfo" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        std::string strTmp;
        KEventOpenInfo kEventOpenInfo;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "EventID", kEventOpenInfo.m_dwEventID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EventMBoxID", kEventOpenInfo.m_dwMBoxID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EventTextureFileName", strTmp ) == S_OK, return false );
        kEventOpenInfo.m_wstrFileName = KncUtil::toWideString( strTmp );
        // 구매제한 아이템 리스트 읽어오기.
        _JIF( kLuaMng_.BeginTable( "EventItemList" ) == S_OK, return false );
        for( int j = 1 ; ; ++j ) {
            GCITEMID dwItemID;
            if( kLuaMng_.GetValue( j, dwItemID ) != S_OK ) break;
            kEventOpenInfo.m_vecItemList.push_back( dwItemID );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // EventItemList

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); 

        vecEventOpenInfo_.push_back( kEventOpenInfo ) ;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetEventOpenInfo( OUT std::vector< KEventOpenInfo >& vecEventOpenInfo_ )
{
    vecEventOpenInfo_.clear();

    KLocker lock( m_csContents );
    vecEventOpenInfo_ = m_vecEventOpenInfo;
}

void KCnClientContentsManager::GetEventMonsterAbility( OUT std::map< int, KMonsterAbility >& mapMonsterList_ )
{
    mapMonsterList_.clear();

    KLocker lock( m_csContents );
    mapMonsterList_ = m_mapMonsterAbility;
}

bool KCnClientContentsManager::LoadEventMonsterAbility( IN KLuaManager& kLuaMng_, OUT std::map<int, KMonsterAbility>& mapMonsterList_ )
{
    mapMonsterList_.clear();

    _JIF( kLuaMng_.BeginTable( "EventMonsterAbility" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        KMonsterAbility kMonsterAbility;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "MonsterID", kMonsterAbility.m_nMonsterNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "AttackRatio", kMonsterAbility.m_nAttackRatio ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DefenceRatio", kMonsterAbility.m_nDefenceRatio ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "HPRatio", kMonsterAbility.m_nHPRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); 

        mapMonsterList_.insert(std::pair<int,KMonsterAbility>( kMonsterAbility.m_nMonsterNum,kMonsterAbility )) ;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadSetInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<char>& setCharPromotion_ )
{
    setCharPromotion_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setCharPromotion_.insert( (char)nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}
bool KCnClientContentsManager::LoadCharOpenType( IN KLuaManager& kLuaMng_, OUT std::map< int, DWORD >& mapCharOpenType_ )
{
    mapCharOpenType_.clear();

    _JIF( kLuaMng_.BeginTable( "Char_Open_Type" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        int nCharType = -1;
        DWORD dwOpenType = -1;
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwOpenType ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        if ( false == mapCharOpenType_.insert( std::make_pair( nCharType, dwOpenType ) ).second ) {
            START_LOG( cerr, L"중복된 캐릭터가 있음.. ID : " << nCharType ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadCharCashGoods( IN KLuaManager& kLuaMng_, OUT std::map< int, GCITEMID >& mapCharCashGoods_ )
{
    mapCharCashGoods_.clear();

    _JIF( kLuaMng_.BeginTable( "Char_Cash_Goods" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        int nCharType = -1;
        GCITEMID dwCashGoods = -1;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwCashGoods ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        if ( false == mapCharCashGoods_.insert( std::make_pair( nCharType, dwCashGoods ) ).second ) {
            START_LOG( cerr, L"중복된 캐릭터가 있음.. ID : " << nCharType ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadDisableMenuList( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecDisableMenuList_ )
{
    vecDisableMenuList_.clear();

    _JIF( kLuaMng_.BeginTable( "Disable_Menu_Btn_List" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        int iBtnNum = 0;
        if ( kLuaMng_.GetValue( i, iBtnNum ) != S_OK )
            break;

        vecDisableMenuList_.push_back( iBtnNum );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnClientContentsManager::LoadSmartPanelMailList( IN KLuaManager& kLuaMng_, OUT std::vector<std::wstring>& vecSmartPanelMailList_ )
{
    vecSmartPanelMailList_.clear();

    _JIF( kLuaMng_.BeginTable( "SmartPanelMailList" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        std::string strTemp;
        strTemp.clear();
        if ( kLuaMng_.GetValue( i, strTemp ) != S_OK )
            break;

        vecSmartPanelMailList_.push_back( KncUtil::toWideString( strTemp ) );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetSmartPanelInfo( OUT bool& bSmartPanelMailListEnable_, OUT std::vector<std::wstring>& vecSmartPanelMailList_ )
{
    vecSmartPanelMailList_.clear();

    KLocker lock( m_csContents );
    bSmartPanelMailListEnable_ = m_bSmartPanelMailListEnable;
    vecSmartPanelMailList_ = m_vecSmartPanelMailList;
}

bool KCnClientContentsManager::LoadChannelImageDownloadURLList( IN KLuaManager& kLuaMng_, OUT std::map<int, std::wstring>& mapChannelImageDownloadURLList_ )
{
    mapChannelImageDownloadURLList_.clear();

    _JIF( kLuaMng_.BeginTable( "ChannelImageDownloadURLList" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nType;
        std::string strTemp;
        strTemp.clear();
        _JIF( kLuaMng_.GetValue( 1, nType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, strTemp ) == S_OK, return false );

        if( mapChannelImageDownloadURLList_.insert( std::make_pair( nType, KncUtil::toWideString( strTemp ) ) ).second == false ) {
            START_LOG( cerr, L"중국 채널 이미지 전달중 중복 Type :" << nType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnClientContentsManager::GetChannelImageDownloadURLList( OUT std::map<int, std::wstring>& mapChannelImageDownloadURLList_ )
{
    mapChannelImageDownloadURLList_.clear();

    KLocker lock( m_csContents );
    mapChannelImageDownloadURLList_ = m_mapChannelImageDownloadURLList;
}
