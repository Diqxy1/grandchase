#include "IPAdvantage.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
//#include "odbc/Odbc.h"
#include "GSDBLayer.h"
#include "UserPacket.h"
#include "GameServer.h"
#include "PetHelper.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

namespace
{
    class KCompIP
    {
    public:
        bool operator() ( const std::pair<DWORD,DWORD>& pr1, const std::pair<DWORD,DWORD>& pr2 )
        {
            if( pr1.first < pr2.first ) return true;
            return false;
        }

        bool operator() ( const DWORD dwVal, const std::pair<DWORD,DWORD>& prIP )
        {
            if( dwVal < prIP.first && dwVal < prIP.second )
                return true;
            return false;
        }

        bool operator() ( const std::pair<DWORD,DWORD>& prIP, const DWORD dwVal )
        {
            if( dwVal > prIP.first && dwVal > prIP.second )
                return true;
            return false;
        }
    };
};

ImplementSingleton( KIPAdvantage );
ImplOstmOperatorW2A( KIPAdvantage );
NiImplementRootRTTI( KIPAdvantage );

KIPAdvantage::KIPAdvantage(void)
:m_dwPCBangUID(2147483000)
,m_bEnableUserPerRatio(false)
,m_MaximumPCBangItemUID(10000)
{
    m_mapPetReward.clear();
    m_mapPetCostumeReward.clear();
    m_mapRatioInfo.clear();
}

KIPAdvantage::~KIPAdvantage(void)
{
}

ImplToStringW( KIPAdvantage )
{

    KLocker lock( m_csIPAdvantage );
    return START_TOSTRINGW
        << TOSTRINGW( m_MaximumPCBangItemUID )
        << TOSTRINGW( m_strMsg )
        << TOSTRINGW( m_mapRatioInfo.size() )
        << TOSTRINGW( m_mapDungeonClearReward.size() )
        << TOSTRINGW( m_mapLevelUpReward.size() )
        << TOSTRINGW( m_vecDailyConnectionReward.size() )
        << TOSTRINGW( m_mapTotalConnectionReward.size() )
        << TOSTRINGW( m_mapPetReward.size() );
}

bool KIPAdvantage::LoadScript()
{
    return _LoadScript(std::string());
}

bool KIPAdvantage::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitIPAdvantage.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadPCBangItemUID( kLuaMng ), return false );

    std::string strMsgID;
    _JIF( kLuaMng.GetValue( "msgID", strMsgID ) == S_OK, return false );
    std::wstring strMsg = SiKGameServer()->m_stringTable.GetValue( KncUtil::toWideString( strMsgID ).c_str() );

    std::vector< std::pair<std::string,int> > vecODBCKey;
    _JIF( LoadODBCKey( kLuaMng, vecODBCKey ), return false );

    bool bEnableUserPerRatio;
    _JIF( kLuaMng.GetValue( "EnableUserPerPCRatio", bEnableUserPerRatio ) == S_OK, return false );

    std::map<DWORD,KRatioInfo> mapRatioInfo;
    _JIF( LoadTotalRatioInfo( kLuaMng, mapRatioInfo ), return false );

    std::map<int,VEC_ITEMS> mapDungeonClearReward;
    _JIF( LoadDungeonClearReward( kLuaMng, mapDungeonClearReward ), return false );

    std::map<int,VEC_ITEMS> mapLevelUpReward;
    _JIF( LoadLevelUpReward( kLuaMng, mapLevelUpReward ), return false );

    VEC_ITEMS vecDailyConnectionReward;
    _JIF( LoadDailyConnectionReward( kLuaMng, vecDailyConnectionReward ), return false );

    //std::map<char, std::vector<KFakeRewardInfo> > mapConnectionReward;
    std::map< DWORD, std::map<char, std::vector<KFakeRewardInfo> > > mapTotalConnectionReward;
    _JIF( LoadTotalConnectionReward( kLuaMng, mapTotalConnectionReward ), return false );

    std::map<GCITEMUID, std::vector<KPetInfo>> mapPetReward;
    _JIF( LoadTotalPetInfo( kLuaMng, mapPetReward ), return false );

    std::map< DWORD, KDropItemInfo > mapPetCostumeReward;
    _JIF( LoadPetCostumeReward( kLuaMng, mapPetCostumeReward ), return false );

    std::vector<std::pair<DWORD,DWORD>> vecTestIPRange;
    _JIF( LoadTestIP( kLuaMng, vecTestIPRange ), return false );

    std::map< std::wstring, char > mapLinBenefitTestID;    // 남미 PC방 혜택 테스트 ID
    _JIF( LoadLINBenefitTestID( kLuaMng, mapLinBenefitTestID ), return false );

    START_LOG( cerr, L"IPAdvantage 정보 읽기 완료." )
        << BUILD_LOG( strMsg )
        << BUILD_LOG( vecODBCKey.size() )
        << BUILD_LOG( mapRatioInfo.size() )
        << BUILD_LOG( mapDungeonClearReward.size() )
        << BUILD_LOG( mapLevelUpReward.size() )
        << BUILD_LOG( vecDailyConnectionReward.size() )
        << BUILD_LOG( mapTotalConnectionReward.size() )
        << BUILD_LOG( mapPetReward.size() )
        << BUILD_LOG( vecTestIPRange.size() )
        << BUILD_LOG( mapLinBenefitTestID.size() ) << END_LOG;
        
    std::sort( vecTestIPRange.begin(), vecTestIPRange.end(), KCompIP() ); 

    std::sort( vecTestIPRange.begin(), vecTestIPRange.end(), KCompIP() );

    {
        KLocker lock( m_csIPAdvantage );

        m_strMsg.swap( strMsg );
        m_bEnableUserPerRatio = bEnableUserPerRatio;
        m_mapRatioInfo.swap( mapRatioInfo );
        m_mapDungeonClearReward.swap( mapDungeonClearReward );
        m_mapLevelUpReward.swap( mapLevelUpReward );
        m_vecDailyConnectionReward.swap( vecDailyConnectionReward );
        m_mapTotalConnectionReward.swap( mapTotalConnectionReward );
        m_vecODBCKey.swap( vecODBCKey );
        m_mapPetReward.swap( mapPetReward );
        m_mapPetCostumeReward.swap( mapPetCostumeReward );
        m_vecTestIPRange.swap( vecTestIPRange );
        m_mapLinBenefitTestID.swap( mapLinBenefitTestID );
    }

    return true;
}

bool KIPAdvantage::LoadODBCKey( IN KLuaManager& kLuaMng_, OUT std::vector< std::pair<std::string,int> >& vecODBCKey_ )
{
    _JIF( kLuaMng_.BeginTable( "ODBC_KeyString") == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<std::string,int> prData;

        _JIF( kLuaMng_.GetValue( 1, prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecODBCKey_.push_back( prData );

    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadTotalRatioInfo( IN KLuaManager& kLuaMng_, OUT std::map<DWORD,KRatioInfo>& mapRatioInfo_ )
{
    mapRatioInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "Ratio" ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        DWORD dwUserType;
        KRatioInfo kRatioInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "UserType", dwUserType ) == S_OK, return false );
        _JIF( LoadRatioInfo( kLuaMng_, kRatioInfo ), return false );

        if( !mapRatioInfo_.insert( std::make_pair( dwUserType, kRatioInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 UserType정보가 있음. ChannelType : " << dwUserType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadRatioInfo( IN KLuaManager& kLuaMng_, OUT KRatioInfo& kRatioInfo_ )
{
    _JIF( kLuaMng_.BeginTable( "UserConfig" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "Lune", kRatioInfo_.m_fLuneRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Necklace", kRatioInfo_.m_fNecklaceRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonDrop", kRatioInfo_.m_fDungeonDropRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Exp", kRatioInfo_.m_fExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Gp", kRatioInfo_.m_fGpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Spx", kRatioInfo_.m_fSpxRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PetExp", kRatioInfo_.m_fPetExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PerUserExp", kRatioInfo_.m_fPerUserExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PerUserGp", kRatioInfo_.m_fPerUserGpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PerUserSpx", kRatioInfo_.m_fPerUserSpxRatio ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadDungeonClearReward( IN KLuaManager& kLuaMng_, OUT std::map<int,VEC_ITEMS>& mapDungeonClearReward_ )
{
    mapDungeonClearReward_.clear();
    _JIF( kLuaMng_.BeginTable( "DungeonClearReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::vector<int> vecInt;
        VEC_ITEMS vecItems;
        _JIF( LoadIntVector( kLuaMng_, std::string("Dungeon"), vecInt ), return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecItems ), return false );

        std::vector<int>::iterator vit;
        for( vit = vecInt.begin() ; vit != vecInt.end() ; ++vit )
        {
            std::map<int,VEC_ITEMS>::iterator mit;
            mit = mapDungeonClearReward_.find( *vit );
            if( mit == mapDungeonClearReward_.end() )
                mapDungeonClearReward_.insert( std::make_pair( *vit, vecItems ) );
            else
                mit->second.insert( mit->second.end(), vecItems.begin(), vecItems.end() );
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadLevelUpReward( IN KLuaManager& kLuaMng_, OUT std::map<int,VEC_ITEMS>& mapLevelUpReward_ )
{

    _JIF( kLuaMng_.BeginTable( "LevelUpReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::vector<int> vecInt;
        VEC_ITEMS vecItems;
        _JIF( LoadIntVector( kLuaMng_, std::string("Level"), vecInt ), return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecItems ), return false );

        std::vector<int>::iterator vit;
        for( vit = vecInt.begin() ; vit != vecInt.end() ; ++vit )
        {
            std::map<int,VEC_ITEMS>::iterator mit;
            mit = mapLevelUpReward_.find( *vit );
            if( mit == mapLevelUpReward_.end() )
                mapLevelUpReward_.insert( std::make_pair( *vit, vecItems ) );
            else
                mit->second.insert( mit->second.end(), vecItems.begin(), vecItems.end() );
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadDailyConnectionReward( IN KLuaManager& kLuaMng_, OUT VEC_ITEMS& vecItems_ )
{
    vecItems_.clear();
    _JIF( kLuaMng_.BeginTable( "DailyConnectionReward" ) == S_OK, return false );
	__LIF( LoadItems( kLuaMng_, std::string("Item"), vecItems_ ) );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::LoadTotalConnectionReward( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::map<char, std::vector<KFakeRewardInfo> > >& mapTotalConnectionReward_ )
{
    mapTotalConnectionReward_.clear();
    _JIF( kLuaMng_.BeginTable( "ConnectionReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwUserType = 0;
        std::map<char, std::vector<KFakeRewardInfo> > mapConnectionReward;
        _JIF( kLuaMng_.GetValue( "UserType", dwUserType ) == S_OK, return false );

        _JIF( LoadConnectionReward( kLuaMng_, mapConnectionReward ), return false );

        if( !mapTotalConnectionReward_.insert( std::make_pair( dwUserType, mapConnectionReward ) ).second ) {
            START_LOG( cerr, L" 중복된 UserType정보가 있음. ChannelType : " << dwUserType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::LoadConnectionReward( IN KLuaManager& kLuaMng_, OUT std::map<char, std::vector<KFakeRewardInfo> >& mapConnectionReward_ )
{
    mapConnectionReward_.clear();
    _JIF( kLuaMng_.BeginTable( "ItemInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        KFakeRewardInfo kData;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadIntRange( kLuaMng_, std::string("LevelRange"), kData.m_prLevelRange.first, kData.m_prLevelRange.second ), return false );
        _JIF( LoadConnectionItems( kLuaMng_, std::string("FakeItem"), kData.m_mapItems ), return false );
        
        mapConnectionReward_[(char)nChar].push_back( kData );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecItems_ )
{
    vecItems_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecItems_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadIntVector( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<int>& vecInt_ )
{
    vecInt_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        int nNum = -1;
        if( kLuaMng_.GetValue( i, nNum ) != S_OK ) break;
        if( nNum != -1 )
            vecInt_.push_back( nNum );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KIPAdvantage::LoadIntRange( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT int& nBegin_, OUT int& nEnd_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, nBegin_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nEnd_ ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

//char KIPAdvantage::CheckPCBangUser( IN const std::wstring& strIP_ )
//{
//    char cRet = PCBANG_NONE;
//
//    std::vector< std::pair<std::string,int> > vecODBCKey;
//    
//    {
//        KLocker lock( m_csIPAdvantage );
//        vecODBCKey = m_vecODBCKey;
//    }
//    
//
//    if ( CheckTestIP( strIP_ ) == true ) {
//        cRet = PCBANG_PUBLISHER;
//        return cRet;
//    }
//
//    if( vecODBCKey.empty() ) return cRet;
//    
//    std::vector< std::pair<std::string,int> >::iterator vit;
//    
//    for( vit = vecODBCKey.begin() ; vit != vecODBCKey.end() ; ++vit )
//    {
//        KODBC* pkODBCPCBang = SiKGSDBLayer()->GetODBC( vit->first.c_str() );
//        if( pkODBCPCBang == NULL ) continue;
//
//        if( QueryPCBangUser( strIP_, *pkODBCPCBang ) )
//        {
//            cRet = (char)vit->second;
//            break;
//        }
//    }
//
//    return cRet;
//}

bool KIPAdvantage::IsPCBangUser( IN const char cType_ )
{
    return (cType_ == PCBANG_PUBLISHER || cType_ == PCBANG_KOG);
}

bool KIPAdvantage::IsTH3BBUser( IN const char cType_ )
{
    return (cType_ == TH3BB_USER);
}

bool KIPAdvantage::IsLINBenefitUser( IN const char cType_ )
{
    return (cType_ == LIN_BENEFIT_TYPE1 || cType_ == LIN_BENEFIT_TYPE2 || cType_ == LIN_BENEFIT_TYPE3 || cType_ == LIN_BENEFIT_TYPE4 );
}

//bool KIPAdvantage::QueryPCBangUser( IN const std::wstring& strIP_, IN KODBC& kODBC_ )
//{
//    KLocker lock( kODBC_.GetCS() );
//
//    _DBJIF( kODBC_.Query( L"{ call dbo.sp_executesql "
//        L"( N'execute dbo.ADBILLGAME_PLUS_IP_CHECK @1, @2 output, @3 output; select @2', "
//        L"N'@1 nvarchar(15), @2 int, @3 int', N'%s', %d, %d ) }",
//        strIP_.c_str(), 0, 0 ), return false, (&kODBC_) );
//
//    int nRet = 0;
//    if( kODBC_.BeginFetch() )
//    {
//        VERIFY_QUERY( kODBC_ >> nRet );
//    }
//    kODBC_.EndFetch();
//
//    return (nRet != 0);
//}

void KIPAdvantage::GetDailyConnectionItem( OUT VEC_ITEMS& vecItems_ )
{
    vecItems_.clear();
    KLocker lock( m_csIPAdvantage );
    vecItems_ = m_vecDailyConnectionReward;
}

//void KIPAdvantage::GetConnectionItem( IN const char cCharType_, IN const int nLevel_, IN OUT DWORD& dwUID_,
//                                      IN OUT std::vector<KItemInfo>& vecNormal_, IN OUT std::vector<KDurationItemInfo>& vecCount_ )
//{
//    // TODO : No Use
//    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
//
//    KItemInfo kNormalItem;
//    kNormalItem.m_cEnchantLevel   = 0;
//    kNormalItem.m_nPeriod         = -1;
//    kNormalItem.m_tRegDate        = tmCurrent;
//    kNormalItem.m_tStartDate      = tmCurrent;
//    kNormalItem.m_tEndDate        = tmCurrent;
//
//    KDurationItemInfo kCountItem;
//    kCountItem.m_bDirty = false;
//
//    KLocker lock( m_csIPAdvantage );
//
//    std::map<char, std::vector<KFakeRewardInfo> >::iterator mit;
//    mit = m_mapConnectionReward.find( cCharType_ );
//    if( mit == m_mapConnectionReward.end() ) return;
//    std::vector<KFakeRewardInfo>::iterator vit;
//    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
//    {
//        if( vit->m_prLevelRange.first <= nLevel_ && vit->m_prLevelRange.second >= nLevel_ )
//        {
//            std::vector<KDropItemInfo>::iterator vit1;
//            for( vit1 = vit->m_vecItems.begin() ; vit1 != vit->m_vecItems.end() ; ++vit1 )
//            {
//                if( vit1->second <= 0 ) // 일반
//                {
//                    kNormalItem.m_dwID  = vit1->first;
//                    kNormalItem.m_dwUID = dwUID_++;
//                    vecNormal_.push_back( kNormalItem );
//                }
//                else // 수량
//                {
//                    kCountItem.m_dwGoodsID      = vit1->first;
//                    kCountItem.m_dwGoodsUID     = dwUID_++;
//                    kCountItem.m_dwInitNum      = (DWORD)vit1->second;
//                    kCountItem.m_dwCurrentNum   = (DWORD)vit1->second;
//                    vecCount_.push_back( kCountItem );
//                }
//            }
//            break;
//        }
//    }
//}

void KIPAdvantage::GetConnectionItem( IN const char cCharType_, IN const int nLevel_, IN OUT std::vector<KItem>& vecInven_, IN const DWORD dwUserType_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KItem kItem;
    kItem.m_cEnchantLevel   = 0;
    kItem.m_cGradeID        = 0;
    kItem.m_nPeriod         = -1;
    kItem.m_tRegDate        = tmCurrent;
    kItem.m_tStartDate      = tmCurrent;
    kItem.m_tEndDate        = tmCurrent;
    kItem.m_cCharType       = cCharType_;

    KLocker lock( m_csIPAdvantage );
    std::map< DWORD, std::map<char, std::vector<KFakeRewardInfo> > >::iterator mitTotal;
    std::map<char, std::vector<KFakeRewardInfo> >::iterator mit;

    mitTotal = m_mapTotalConnectionReward.find( dwUserType_ );
    if( mitTotal == m_mapTotalConnectionReward.end() ) return;
    mit = mitTotal->second.find( cCharType_ );
    if( mit == mitTotal->second.end() ) return;
    std::vector<KFakeRewardInfo>::iterator vit;
    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    {
        if( vit->m_prLevelRange.first <= nLevel_ && vit->m_prLevelRange.second >= nLevel_ )
        {
            std::map< DWORD, KDropItemInfo >::iterator vit1;
            for( vit1 = vit->m_mapItems.begin() ; vit1 != vit->m_mapItems.end() ; ++vit1 )
            {
                kItem.m_ItemID = vit1->second.m_ItemID;
                kItem.m_nInitCount = vit1->second.m_nDuration;
                kItem.m_nCount = vit1->second.m_nDuration;
                kItem.m_nPeriod = vit1->second.m_nPeriod;
                kItem.m_ItemUID = vit1->first;
                vecInven_.push_back( kItem );
            }
            break;
        }
    }
    // PC방 펫 코스튬 정보 넣어주기.
    std::map< DWORD, KDropItemInfo >::iterator vitPet;
    if( m_mapPetCostumeReward.empty() ) return;
    for( vitPet = m_mapPetCostumeReward.begin(); vitPet != m_mapPetCostumeReward.end(); ++vitPet ) {
            kItem.m_ItemID = vitPet->second.m_ItemID;
            kItem.m_nInitCount = vitPet->second.m_nDuration;
            kItem.m_nCount = vitPet->second.m_nDuration;
            kItem.m_nPeriod = vitPet->second.m_nPeriod;
            kItem.m_ItemUID = vitPet->first;
            vecInven_.push_back( kItem );
    }
}

void KIPAdvantage::GetExpGpSpxBonus( IN const DWORD dwUserType_, OUT float& fExpRatio_, OUT float& fGpRatio_, OUT float& fSpxRatio_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return;
    }

    fExpRatio_ = mit->second.m_fExpRatio;
    fGpRatio_  = mit->second.m_fGpRatio;
    fSpxRatio_ = mit->second.m_fSpxRatio;
}

void KIPAdvantage::GetUserPerRatio( IN const DWORD dwUserType_, IN int nCount_, OUT float& fExpRatio_, OUT float& fGpRatio_, OUT float& fSpxRatio_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return;
    }

    fExpRatio_ += nCount_ * mit->second.m_fPerUserExpRatio;
    fGpRatio_  += nCount_ * mit->second.m_fPerUserGpRatio;
    fSpxRatio_ += nCount_ * mit->second.m_fPerUserSpxRatio;
}

float KIPAdvantage::GetPetExpRatio( IN const DWORD dwUserType_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return 0.f;
    }
    return mit->second.m_fPetExpRatio;
}

float KIPAdvantage::GetLuneStrongBonus( IN const DWORD dwUserType_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return 0.f;
    }

    return mit->second.m_fLuneRatio;
}

float KIPAdvantage::GetNecklaceStrongBonus( IN const DWORD dwUserType_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return 0.f;
    }

    return mit->second.m_fNecklaceRatio;
}

float KIPAdvantage::GetDungeonDropBonus( IN const DWORD dwUserType_ )
{
    KLocker lock(m_csIPAdvantage );
    std::map<DWORD, KRatioInfo>::iterator mit;
    mit = m_mapRatioInfo.find( dwUserType_ );
    if( mit == m_mapRatioInfo.end() ) {
        return 0.f;
    }

    return mit->second.m_fDungeonDropRatio;
}

int KIPAdvantage::AddLevelUpReward( IN const int nOldLv_, IN const int nCurrentLv_, IN OUT VEC_ITEMS& vecItem_ )
{
    int nCount = 0;
    if( nOldLv_ >= nCurrentLv_ ) return nCount;

    for ( int i = nCurrentLv_ ; i > nOldLv_ ; --i )
    {
        std::map<int, VEC_ITEMS >::iterator mit;
        mit = m_mapLevelUpReward.find( i );
        if( mit == m_mapLevelUpReward.end() ) continue;

        if( mit->second.empty() ) continue;

        vecItem_.insert( vecItem_.end(), mit->second.begin(), mit->second.end() );
        nCount += (int)mit->second.size();
    }
    return nCount;
}

int KIPAdvantage::AddDungeonClearReward( IN const int nGameMode_, IN OUT VEC_ITEMS& vecItem_ )
{
    int nCount = 0;
    std::map<int, VEC_ITEMS >::iterator mit;
    mit = m_mapDungeonClearReward.find( nGameMode_ );
    if( mit == m_mapDungeonClearReward.end() ) return nCount;

    if( mit->second.empty() ) return nCount;

    vecItem_.insert( vecItem_.end(), mit->second.begin(), mit->second.end() );
    nCount += (int)mit->second.size();
    return nCount;
}

std::wstring KIPAdvantage::GetMsg()
{
    KLocker lock( m_csIPAdvantage );
    return m_strMsg;
}

bool KIPAdvantage::LoadTotalPetInfo( IN KLuaManager& kLuaMng_, OUT std::map<GCITEMUID, std::vector<KPetInfo>>& mapPetReward_ )
{
    mapPetReward_.clear();

    _JIF( kLuaMng_.BeginTable( "PetConnectionReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwUserType = 0;
        std::vector<KPetInfo> vecPetInfo;

        _JIF( kLuaMng_.GetValue( "UserType", dwUserType ) == S_OK, return false );
        _JIF( LoadPetReward( kLuaMng_, vecPetInfo ), return false );

        if( !mapPetReward_.insert( std::make_pair( dwUserType, vecPetInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 UserType정보가 있음. UserType : " << dwUserType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::LoadPetReward( IN KLuaManager& kLuaMng_, OUT std::vector<KPetInfo>& vcePetReward_ )
{
    vcePetReward_.clear();
    std::string strPetName;

    _JIF( kLuaMng_.BeginTable( "PetInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KPetInfo kPetInfo;
        kPetInfo.Init();
        int nType = -1;

        _JIF( kLuaMng_.GetValue( "PetItemID", kPetInfo.m_dwID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "PetItemUID", kPetInfo.m_dwUID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "PetStrName", strPetName ) == S_OK, return false );
        kPetInfo.m_strName = SiKGameServer()->m_stringTable.GetValue( KncUtil::toWideString( strPetName ).c_str() );
        _JIF( LoadSimpleItems( kLuaMng_, std::string("PetEquipItem"), kPetInfo.m_vecEquipItem ), return false );
        vcePetReward_.push_back( kPetInfo );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    SetPCBangPetInfo( vcePetReward_ ); // Pet의 기본정보 세팅.
    return true;
}

bool KIPAdvantage::LoadSimpleItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KSimpleItem>& vecSimpleItem_ )
{
    vecSimpleItem_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KSimpleItem kSimpleItem;
        int nItemType;
        if( kLuaMng_.GetValue( i++, kSimpleItem.m_dwID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, nItemType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kSimpleItem.m_dwUID ) == S_OK, return false );
        kSimpleItem.m_cItemType = nItemType;
        vecSimpleItem_.push_back( kSimpleItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

// PC방 펫을 위한 기본적인 세팅.
void KIPAdvantage::SetPCBangPetInfo( IN OUT std::vector<KPetInfo>& vecPetInfo_ )
{
    std::vector<KPetInfo>::iterator vit;
    for( vit = vecPetInfo_.begin(); vit != vecPetInfo_.end(); ++vit ) {
        // Pet 초기 생성할 때의 기본적인 세팅.
        for( int i = 0; i <= 2; i++ ) {
            vit->m_mapInitExp.insert( std::make_pair( i , SiKPetHelper()->GetPetMinExp() ) );
        }
        vit->m_dwEXP = SiKPetHelper()->GetPetMinExp();
        vit->m_iSatiation = KIPAdvantage::MININUM_PCPET_SATIATION;
        vit->m_iInitSatiation  = vit->m_iSatiation;
        vit->m_iLevel          = SiKPetHelper()->GetPetLevel( vit->m_dwEXP );
        vit->m_nHatchingID = SiKPetHelper()->GetNextID( vit->m_dwID );
    }
}

void KIPAdvantage::GetPCBangPetItem( IN const char cCharType_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_, 
                                        IN OUT std::vector<KItem>& vecItems_, IN DWORD dwUserType_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    time_t tmYesterday = KncUtil::TimeToInt( CTime::GetCurrentTime() - CTimeSpan( 1, 0, 0, 0 ) );

    KItem kItem;
    kItem.m_cEnchantLevel   = 0;
    kItem.m_cGradeID        = 0;
    kItem.m_nPeriod         = -1;
    kItem.m_tRegDate        = tmCurrent;
    kItem.m_tStartDate      = tmCurrent;
    kItem.m_tEndDate        = tmYesterday;
    kItem.m_cCharType       = cCharType_;

    KLocker lock( m_csIPAdvantage );

    std::map<GCITEMUID,std::vector<KPetInfo>>::iterator mit;
    std::vector<KPetInfo>::iterator vit;
    mit = m_mapPetReward.find( dwUserType_ );
    if( mit == m_mapPetReward.end() ) {
        return;
    }
    for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
        if( mapPetInfo_.insert( std::make_pair( vit->m_dwUID, *vit ) ).second == false ) {
            START_LOG( cerr, L"PC방 펫 입력시 중복 데이터. PetItemID : " << vit->m_dwID ) << END_LOG;
        }
        else {
            // PetItem UserInven에 넣어주기.
            kItem.m_ItemID = vit->m_dwID;
            kItem.m_ItemUID = vit->m_dwUID;
            vecItems_.push_back( kItem );

            // Pet 장착 정보 UserInven에 넣어주기.
            std::vector<KSimpleItem>::iterator vit1;
            for( vit1 = vit->m_vecEquipItem.begin(); vit1 != vit->m_vecEquipItem.end(); ++vit1 )
            {
                kItem.m_ItemID = vit1->m_dwID;
                kItem.m_ItemUID = vit1->m_dwUID;
                kItem.m_nInitCount = (int)vit1->m_cItemType;
                kItem.m_nCount = (int)vit1->m_cItemType;
                kItem.m_nPeriod = KItem::UNLIMITED_ITEM;
                vecItems_.push_back( kItem );
            }

            START_LOG( clog, L"PC방 펫 추가.. ItemID : " << kItem.m_ItemID << L", ItemUID : " << kItem.m_ItemUID << L", Pet Size : " << mapPetInfo_.size() ) << END_LOG;
        }
    }
}

bool KIPAdvantage::LoadPetCostumeReward( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, KDropItemInfo >& mapItems_ )
{
    mapItems_.clear();
    _JIF( kLuaMng_.BeginTable( "PetCostumeItem" ) == S_OK, return false );
	__LIF( LoadConnectionItems( kLuaMng_, std::string("Item"), mapItems_ ) );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::LoadTestIP( IN KLuaManager& kLuaMng_, OUT std::vector<std::pair<DWORD,DWORD>>& vecIP_ )
{
    vecIP_.clear();
    _JIF( kLuaMng_.BeginTable( "TestIP" ) == S_OK, return false );

    for ( int i = 1 ;; ++i ) {
        std::string strFirst;
        std::string strSecond;
        std::pair<DWORD,DWORD> prIP;

        if( kLuaMng_.BeginTable( i ) != S_OK) break;
        _JIF( kLuaMng_.GetValue( 1, strFirst ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, strSecond ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        prIP.first = ::ntohl( inet_addr( strFirst.c_str() ) );
        prIP.second = ::ntohl( inet_addr( strSecond.c_str() ) );
        vecIP_.push_back( prIP );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::CheckTestIP( IN const std::wstring& strIP_ )
{
    // TestIP Check
    DWORD dwIP = ::ntohl( inet_addr( KncUtil::toNarrowString( strIP_ ).c_str() ) );
    bool bRet = std::binary_search( m_vecTestIPRange.begin(), m_vecTestIPRange.end(), dwIP, KCompIP() );
    START_LOG( clog, L"Check Test IP : " << strIP_ << L" (" << dwIP << L")" << L", IsInRange : " << bRet ) << END_LOG;
    return bRet;
}

bool KIPAdvantage::LoadConnectionItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KDropItemInfo >& mapItems_ )
{
    mapItems_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        DWORD nItemUID = 0;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, nItemUID ) == S_OK, return false );

        if( mapItems_.insert( std::make_pair( nItemUID, kData ) ).second == false )
        {
            START_LOG( cerr, L"중복된 ItemUID : " << nItemUID ) << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}


bool KIPAdvantage::LoadPCBangItemUID( IN KLuaManager& kLuaMng_ )
{
    GCITEMUID MinItemUID = ULONG_MAX;
    _JIF( kLuaMng_.GetValue( "MaximumPCBangItemUID", MinItemUID ) == S_OK, return false );
    START_LOG( clog, L"Load Minimum PCBang ItemUID.. " << MinItemUID ) << END_LOG;

    KLocker lock( m_csIPAdvantage );
    m_MaximumPCBangItemUID = MinItemUID;
    return true;
}

bool KIPAdvantage::LoadLINBenefitTestID( IN KLuaManager& kLuaMng_, OUT std::map< std::wstring, char >& mapLinBenefitTestID_ )
{
    mapLinBenefitTestID_.clear();

    int nUseLINBenefitTest = 0;
    _JIF( kLuaMng_.GetValue( "USE_LIN_BENEFIT_TEST", nUseLINBenefitTest ) == S_OK, return false );

    if ( nUseLINBenefitTest == 0 ) {
        // 사용 안함 설정이면 ID맵을 empty로 유지시킨다
        return true;
    }

    _JIF( kLuaMng_.BeginTable( "LINBenefitTestID" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        std::string strLogin;
        int nBenefitType = 0;
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( 1, strLogin ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nBenefitType ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapLinBenefitTestID_.insert( std::make_pair( KncUtil::toWideString( strLogin ), static_cast< char >( nBenefitType ) ) ).second ) {
            START_LOG( cerr, L"남미 PC방 혜택 테스트용 ID가 중복됨: " << KncUtil::toWideString( strLogin ) ) << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KIPAdvantage::GetLINBenefitTestID( IN std::wstring& wsLogin_, OUT char& cBenefitType_ )
{
    /*
    return value
        false: ID맵이 비어있는 경우
        true: 등록된 ID맵이 있는 경우
    */

    if ( m_mapLinBenefitTestID.empty() ) {
        return false;
    }

    std::map< std::wstring, char >::iterator mitLinBenefitTestID;
    mitLinBenefitTestID = m_mapLinBenefitTestID.find( wsLogin_ );
    if ( mitLinBenefitTestID == m_mapLinBenefitTestID.end() ) {
        return false;
    }

    cBenefitType_ = mitLinBenefitTestID->second;

    return true;
}
