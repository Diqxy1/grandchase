#include "PostConfig.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "GameServer.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KPostConfig );
ImplOstmOperatorW2A( KPostConfig );
NiImplementRootRTTI( KPostConfig );

KPostConfig::KPostConfig(void)
:m_bPostEnable(false)
,m_bSantaPost(false)
,m_bNewPostAlarm(false)
,m_bUserSendPost(false)
,m_bUserPostItem(false)
,m_bUserPostGP(false)
,m_dwCheckNewPostTickGap(1000*60*3)
,m_usOnePageLetterCount(5)
,m_usNewLetterTitleCount(5)
,m_usTitleLength(20)
,m_usMessageLength(200)
,m_nLetterCost(1000)
,m_fGPCostRatio(0.05f)
,m_fItemCostRatio(0.05f)
,m_nSystemPostStorageTime(30)
,m_nUserPostStorageTime(-1)
{
    m_setNotPostItemID.clear();
}

KPostConfig::~KPostConfig(void)
{
}

ImplToStringW( KPostConfig )
{
    KLocker lock1( m_csPostEnable );
    KLocker lock2( m_csPostConfig );
    KLocker lock3( m_csSystemConfig );
    KLocker lock4( m_csLetterConfig );

    return START_TOSTRINGW
        << TOSTRINGWb( m_bPostEnable )
        << TOSTRINGWb( m_bSantaPost )
        << TOSTRINGWb( m_bNewPostAlarm )
        << TOSTRINGWb( m_bUserSendPost )
        << TOSTRINGWb( m_bUserPostItem )
        << TOSTRINGWb( m_bUserPostGP )
        << TOSTRINGW( m_dwCheckNewPostTickGap )
        << TOSTRINGW( m_usOnePageLetterCount )
        << TOSTRINGW( m_usNewLetterTitleCount )
        << TOSTRINGW( m_usTitleLength )
        << TOSTRINGW( m_usMessageLength )
        << TOSTRINGW( m_nLetterCost )
        << TOSTRINGW( m_fGPCostRatio )
        << TOSTRINGW( m_fItemCostRatio )
        << TOSTRINGW( m_nSystemPostStorageTime )
        << TOSTRINGW( m_nUserPostStorageTime )
        << TOSTRINGW( m_setNotPostItemID.size() );
}

bool KPostConfig::LoadScript()
{
    return _LoadScript(std::string());
}

bool KPostConfig::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    bool bPostEnable = false;
    bool bSantaPost = false;
    bool bNewPostAlarm = false;
    bool bUserSendPost = false;
    bool bUserPostItem = false;
    bool bUserPostGP = false;
    DWORD dwCheckNewPostTickGap = 0L;
    USHORT usOnePageLetterCount = 0;
    USHORT usNewLetterTitleCount = 0;
    USHORT usTitleLength = 0;
    USHORT usMessageLength = 0;
    int nLetterCost = 0;
    float fGPCostRatio = 0.f;
    float fItemCostRatio = 0.f;
    int nSystemPostStorageTime = -1;
    int nUserPostStorageTime = -1;
    std::set<GCITEMID> setNotPostItemID;
    setNotPostItemID.clear();

    const std::string strScriptName = "InitPostConfig.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PostEnable", bPostEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SantaPost", bSantaPost ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "NewPostAlarm", bNewPostAlarm ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UserSendPost", bUserSendPost ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UserPostItem", bUserPostItem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UserPostGP", bUserPostGP ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CheckNewPostTickGap", dwCheckNewPostTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "OnePageLetterCount", usOnePageLetterCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "NewLetterTitleCount", usNewLetterTitleCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TitleLength", usTitleLength ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MessageLength", usMessageLength ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LetterCost", nLetterCost ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "fGPCostRatio", fGPCostRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "fItemCostRatio", fItemCostRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SystemPostStorageTime", nSystemPostStorageTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UserPostStorageTime", nUserPostStorageTime ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "NotPostItemID" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        GCITEMID ItemID = 0;
        if ( kLuaMng.GetValue( i, ItemID ) != S_OK ) break;
        if ( ItemID == 0 ) continue;
        m_setNotPostItemID.insert( ItemID );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load PostCongit Script.." )
        << BUILD_LOG( bPostEnable )
        << BUILD_LOG( bSantaPost )
        << BUILD_LOG( bNewPostAlarm )
        << BUILD_LOG( bUserSendPost )
        << BUILD_LOG( bUserPostItem )
        << BUILD_LOG( bUserPostGP )
        << BUILD_LOG( dwCheckNewPostTickGap )
        << BUILD_LOG( usOnePageLetterCount )
        << BUILD_LOG( usNewLetterTitleCount )
        << BUILD_LOG( usTitleLength )
        << BUILD_LOG( usMessageLength )
        << BUILD_LOG( nLetterCost )
        << BUILD_LOG( fGPCostRatio )
        << BUILD_LOG( fItemCostRatio )
        << BUILD_LOG( nSystemPostStorageTime )
        << BUILD_LOG( nUserPostStorageTime )
        << BUILD_LOG( setNotPostItemID.size() ) << END_LOG;

    {
        KLocker lock(m_csPostEnable);
        m_bPostEnable = bPostEnable;
    }

    {
        KLocker lock(m_csPostConfig);
        m_bSantaPost = bSantaPost;
        m_bNewPostAlarm = bNewPostAlarm;
        m_bUserSendPost = bUserSendPost;
        m_bUserPostItem = bUserPostItem;
        m_bUserPostGP = bUserPostGP;
    }

    {
        KLocker lock(m_csSystemConfig);
        m_dwCheckNewPostTickGap = dwCheckNewPostTickGap;
        m_usOnePageLetterCount = usOnePageLetterCount;
        m_usNewLetterTitleCount = usNewLetterTitleCount;
    }

    {
        KLocker lock(m_csLetterConfig);
        m_usTitleLength = usTitleLength;
        m_usMessageLength = usMessageLength;
        m_nLetterCost = nLetterCost;
        m_fGPCostRatio = fGPCostRatio;
        m_fItemCostRatio = fItemCostRatio;
        m_nSystemPostStorageTime = nSystemPostStorageTime;
        m_nUserPostStorageTime = nUserPostStorageTime;
        m_setNotPostItemID.swap( setNotPostItemID );
    }
    return true;
}

std::wstring KPostConfig::GetPostString( IN std::wstring strKey_ )
{
    wchar_t szTemp[255] = {0};

    GetPrivateProfileStringW( L"Post", strKey_.c_str(), L"", szTemp, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strPostString(szTemp);

    return strPostString;
}

void KPostConfig::GetPostPresentString( IN std::wstring strMsgID_, IN std::wstring wstrNickName_, OUT std::wstring& wstrMsg_ )
{
    wchar_t szTemp[255] = {0};
    GetPrivateProfileStringW( L"Post", strMsgID_.c_str(), L"", szTemp, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strPostString(szTemp);

    boost::replace_all( strPostString, L"__Nick__", wstrNickName_ );
    wstrMsg_ = strPostString;
}