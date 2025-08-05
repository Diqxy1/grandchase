#include "SpecialReward.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Kairo.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSpecialReward );
ImplOstmOperatorW2A( KSpecialReward );
NiImplementRootRTTI( KSpecialReward );

KSpecialReward::KSpecialReward(void)
{
    // 아이템 박스Type 등록
    m_setItemBox.insert( KRewardInfo::RT_CHARACTER );
    m_setItemBox.insert( KRewardInfo::RT_ITEM_1 );
    m_setItemBox.insert( KRewardInfo::RT_ITEM_2 );
}

KSpecialReward::~KSpecialReward(void)
{
}

ImplToStringW( KSpecialReward )
{
    KLocker lock( m_csDrops );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapBoxInfo.size() )
        << TOSTRINGW( m_mapDifficultBoxRate.size() )
        << TOSTRINGW( m_mapDungeonBoxDropList.size() )
        << TOSTRINGW( m_mapBoxGPRate.size() );
}

bool KSpecialReward::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSpecialReward::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< int, std::vector<USHORT> > mapBoxInfo;
    std::map< int, KLottery > mapDifficultBoxRate;
    std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> > mapDungeonBoxDropList;
    std::map< USHORT, float > mapBoxGPRate;

    const std::string strScriptName = "InitSpecialReward.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadBoxInfo( kLuaMng, mapBoxInfo ), return false );
    _JIF( LoadDifficultBoxRate( kLuaMng, mapDifficultBoxRate ), return false );
    _JIF( LoadDungeonBoxDropList( kLuaMng, mapDungeonBoxDropList ), return false );
    _JIF( LoadBoxGPRate( kLuaMng, mapBoxGPRate ), return false );

    START_LOG( cerr, L"Special Drop 읽기 완료." )
        << BUILD_LOG( mapBoxInfo.size() )
        << BUILD_LOG( mapDifficultBoxRate.size() )
        << BUILD_LOG( mapDungeonBoxDropList.size() )
        << BUILD_LOG( mapBoxGPRate.size() ) << END_LOG;

    KLocker lock( m_csDrops );
    m_mapBoxInfo.swap( mapBoxInfo );
    m_mapDifficultBoxRate.swap( mapDifficultBoxRate );
    m_mapDungeonBoxDropList.swap( mapDungeonBoxDropList );
    m_mapBoxGPRate.swap( mapBoxGPRate );
    return true;
}

bool KSpecialReward::LoadBoxInfo( IN KLuaManager& kLuaMng_, OUT std::map< int, std::vector<USHORT> >& mapBoxInfo_ )
{
    mapBoxInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "BoxIDtype" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        std::vector<USHORT> vecBoxType;
        vecBoxType.reserve( 4 );
        int nBoxID = 0;
        _JIF( kLuaMng_.GetValue( 1, nBoxID ) == S_OK, return false );

        for ( int j = 2 ; ; ++j ) {
            USHORT usBoxType = 0;
            if ( kLuaMng_.GetValue( j, usBoxType ) != S_OK ) break;
            vecBoxType.push_back( usBoxType );
        }

        if ( false == mapBoxInfo_.insert( std::make_pair( nBoxID, vecBoxType ) ).second ) {
            START_LOG( cerr, L"증복된 BoxID 입니다. BoxID : " << nBoxID ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load SpecialDrop BoxID-Type Info.. size : " << mapBoxInfo_.size() ) << END_LOG;
    return true;
}

bool KSpecialReward::LoadDifficultBoxRate( IN KLuaManager& kLuaMng_, OUT std::map< int, KLottery >& mapDifficultBoxRate_ )
{
    mapDifficultBoxRate_.clear();

    _JIF( kLuaMng_.BeginTable( "DifficultBoxRate" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nDifficulty = -1;
        KLottery kData;

        _JIF( kLuaMng_.GetValue( "Difficulty", nDifficulty ) == S_OK, return false );
        _JIF( LoadDropBoxRate( kLuaMng_, kData ), return false );

        if ( false == mapDifficultBoxRate_.insert( std::make_pair( nDifficulty, kData ) ).second ) {
            START_LOG( cerr, L"증복된 난이도 입니다. Difficulty : " << nDifficulty ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load SpecialDrop Difficulty-BoxRate Info.. size : " << mapDifficultBoxRate_.size() ) << END_LOG;
    return true;
}

bool KSpecialReward::LoadDropBoxRate( IN KLuaManager& kLuaMng_, IN OUT KLottery& kData_ )
{
    _JIF( kLuaMng_.BeginTable( "DropBoxRate" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        USHORT usBoxID = 0;
        float fRatio = 0.f;
        _JIF( kLuaMng_.GetValue( 1, usBoxID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kData_.AddCase( usBoxID, fRatio ), return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSpecialReward::LoadDungeonBoxDropList( IN KLuaManager& kLuaMng_, OUT std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> >& mapDungeonBoxDropList_ )
{
    mapDungeonBoxDropList_.clear();

    _JIF( kLuaMng_.BeginTable( "DungeonBoxDropList" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        Kairo kData;
        DUNGEON_KEY prDungeonKey;
        BOX_KEY prBoxKey;

        _JIF( kLuaMng_.GetValue( "DungeonID", prDungeonKey.first ) == S_OK, return false );
        //_JIF( kLuaMng_.GetValue( "Difficulty", prDungeonKey.second ) == S_OK, return false );
        if ( kLuaMng_.GetValue( "Difficulty", prDungeonKey.second ) != S_OK ) {
            START_LOG( cerr, L"DungeonID : " << prDungeonKey.first << L", i count : " << i ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.GetValue( "BoxType", prBoxKey.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "CharType", prBoxKey.second ) == S_OK, return false );
        _JIF( LoadKairo( kLuaMng_, std::string("DropList"), prDungeonKey, prBoxKey, kData ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapDungeonBoxDropList_[prDungeonKey].insert( std::make_pair(prBoxKey,kData) ).second ) {
            START_LOG( cerr, L"중복된 키값입니다." )
                << BUILD_LOG( prDungeonKey.first )
                << BUILD_LOG( prDungeonKey.second )
                << BUILD_LOG( prBoxKey.first )
                << BUILD_LOG( prBoxKey.second ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load SpecialDrop Dungeon BoxDrop List Info.. size : " << mapDungeonBoxDropList_.size() ) << END_LOG;
    return true;
}

bool KSpecialReward::LoadKairo( IN OUT KLuaManager& kLuaMng_, IN const std::string& strTable_, IN DUNGEON_KEY& prDungeonKey_, IN BOX_KEY& prBoxKey_, OUT Kairo& kData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID nItemID = 0;
        int nCount = 0;
        int nPeriod = 0;
        float fRatio = 0.f;
        _JIF( kLuaMng_.GetValue( 1, nItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fRatio ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        __LIF( kData_.SetPostItem( nItemID, nCount, nPeriod, fRatio ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSpecialReward::LoadBoxGPRate( IN KLuaManager& kLuaMng_, OUT std::map< USHORT, float >& mapBoxGPRate_ )
{
    mapBoxGPRate_.clear();

    _JIF( kLuaMng_.BeginTable( "GPRate" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        USHORT usBoxType = 0;
        float fGPRate = 0.f;

        _JIF( kLuaMng_.GetValue( 1, usBoxType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fGPRate ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapBoxGPRate_.insert( std::make_pair(usBoxType,fGPRate/100) ).second ) {
            START_LOG( cerr, L"중복된 BoxType 입니다. BoxType : " << usBoxType ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load SpecialDrop Box GP Rate.. size : " << mapBoxGPRate_.size() ) << END_LOG;
    return true;
}

float KSpecialReward::GetBoxGPRate( IN const USHORT& usBoxType_ )
{
    KLocker lock( m_csDrops );

    std::map< USHORT, float >::const_iterator cmit;
    cmit = m_mapBoxGPRate.find( usBoxType_ );
    if ( cmit == m_mapBoxGPRate.end() ) {
        START_LOG( clog, L"해당 BoxType의 GP 보너스 정보가 없음. BoxType : " << usBoxType_ ) << END_LOG;
        return 0.f;
    }

    return cmit->second;
}

bool KSpecialReward::GetBoxInfo( IN const int& nBoxID_, OUT std::vector<USHORT>& vecBoxType_ )
{
    KLocker lock( m_csDrops );
    std::map< int, std::vector<USHORT> >::const_iterator cmit;
    std::vector<USHORT>::const_iterator cvit;
    cmit = m_mapBoxInfo.find( nBoxID_ );
    if ( cmit == m_mapBoxInfo.end() ) {
        START_LOG( cwarn, L"해당 BoxID가 없음. nBoxID_ : " << nBoxID_ ) << END_LOG;
        return false;
    }

    vecBoxType_ = cmit->second;
    return true;
}

int KSpecialReward::GetDifficultBoxID( IN const int& nDifficult_ )
{
    KLocker lock( m_csDrops );
    std::map< int, KLottery >::const_iterator cmit;
    cmit = m_mapDifficultBoxRate.find( nDifficult_ );
    if ( cmit == m_mapDifficultBoxRate.end() ) {
        START_LOG( cwarn, L"해당 난이도의 BoxID가 없음. nDifficult_ : " << nDifficult_ ) << END_LOG;
        return -1;
    }

    int nDecision = cmit->second.Decision();
    if ( nDecision == KLottery::CASE_BLANK ) {
        START_LOG( cwarn, L"해당 난이도의 BoxID 확률이 100% 미만. nDifficult_ : " << nDifficult_ << L",총확률 :" << cmit->second.GetTotalProb() ) << END_LOG;
        return -1;
    }

    return nDecision;
}

bool KSpecialReward::GetBoxDropItem( IN const DUNGEON_KEY& prModeKey_, IN const BOX_KEY& prBoxKey_, OUT KDropItemInfo& kItem_ )
{
    KLocker lock( m_csDrops );
    std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> >::const_iterator cmitMode;
    cmitMode = m_mapDungeonBoxDropList.find( prModeKey_ );
    if ( cmitMode == m_mapDungeonBoxDropList.end() ) {
        START_LOG( cwarn, L"해당 던전난이도의 Box정보가 없음. ModeID : " << prModeKey_.first << L", Difficult : " << prModeKey_.second ) << END_LOG;
        return false;
    }

    std::map<BOX_KEY,Kairo>::const_iterator cmitBox;
    cmitBox = cmitMode->second.find( prBoxKey_ );
    if ( cmitBox == cmitMode->second.end() ) {
        START_LOG( cwarn, L"해당 BoxType/CharType의 Box정보가 없음." )
            << BUILD_LOG( prModeKey_.first )
            << BUILD_LOG( prModeKey_.second )
            << BUILD_LOG( prBoxKey_.first )
            << BUILD_LOG( prBoxKey_.second ) << END_LOG;
        return false;
    }

    cmitBox->second.Do( kItem_ );
    return true;
}

void KSpecialReward::DumpBoxInfo()
{
    //KLocker lock( m_csDrops );

    //std::cout << L"=== Dump Box Info (size:" << m_mapBoxInfo.size() << L") ===" << dbg::endl;

    //std::map< int, std::vector<USHORT> >::const_iterator cmit;
    //for ( cmit = m_mapBoxInfo.begin() ; cmit != m_mapBoxInfo.end() ; ++cmit ) {
    //    std::cout << L"BoxID : " << cmit->first << L" (size:" << cmit->second.size() << L")" << dbg::endl
    //              << L"BoxType : ";
    //    std::vector<USHORT>::const_iterator cvit;
    //    for( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit ) {
    //        std::cout << *cvit << L", ";
    //    }
    //    std::cout << dbg::endl;
    //}

    //std::cout << L"=== End Dump ===" << dbg::endl;
}

void KSpecialReward::DumpDifficultBoxRate()
{
    //KLocker lock( m_csDrops );

    //std::cout << L"=== Dump Difficult Box Rate (size:" << m_mapBoxInfo.size() << L") ===" << dbg::endl;

    //std::map< int, KLottery >::const_iterator cmit;
    //for ( cmit = m_mapDifficultBoxRate.begin() ; cmit != m_mapDifficultBoxRate.end() ; ++cmit ) {
    //    std::cout << L"Difficulty : " << cmit->first
    //              << L", CaseNum : " << cmit->second.GetCaseNum()
    //              << L", TotalProb : " << cmit->second.GetTotalProb() << dbg::endl;
    //}

    //std::cout << L"=== End Dump ===" << dbg::endl;
}

bool KSpecialReward::DumpBoxItemList( IN const int& nModeID_, IN const int& nDiff_, IN const USHORT& usBoxType_, IN const int& nCharType_ )
{
    //KLocker lock( m_csDrops );

    //std::cout << L"=== Dump Box Item List (size:" << m_mapDungeonBoxDropList.size() << L") ===" << dbg::endl;

    //std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> >::const_iterator cmitMode;
    //cmitMode = m_mapDungeonBoxDropList.find( std::make_pair( nModeID_, nDiff_ ) );
    //if ( cmitMode == m_mapDungeonBoxDropList.end() ) {
    //    std::cout << L"해당 던전난이도 정보가 없음."
    //              << L" ModeID : " << nModeID_
    //              << L", Difficult : " << nDiff_ << dbg::endl
    //              << L"=== End Dump ===" << dbg::endl;
    //    return false;
    //}

    //std::map<BOX_KEY,Kairo>::const_iterator cmitBox;
    //cmitBox = cmitMode->second.find( std::make_pair( usBoxType_, nCharType_ ) );
    //if ( cmitBox == cmitMode->second.end() ) {
    //    std::cout << L"해당 BoxType/CharType 정보가 없음."
    //              << L" BoxType : " << usBoxType_
    //              << L", CharType : " << nCharType_ << dbg::endl
    //              << L"=== End Dump ===" << dbg::endl;
    //    return false;
    //}

    //Kairo kKairo = cmitBox->second;
    //std::vector<KDropItemInfo> vecRewardItems;
    //kKairo.GetItemList( vecRewardItems );

    //std::cout << L"[Current Item List (size:" << vecRewardItems.size() << L")]" << dbg::endl;

    //std::vector<KDropItemInfo>::const_iterator cvit;
    //for ( cvit = vecRewardItems.begin() ; cvit != vecRewardItems.end() ; ++cvit ) {
    //    std::cout << L"ItemID : " << cvit->m_ItemID
    //              << L", Count : " << cvit->m_nDuration
    //              << L", Period : " << cvit->m_nPeriod << dbg::endl;
    //}

    //std::cout << L"=== End Dump ===" << dbg::endl;
    return true;
}

bool KSpecialReward::IsGPBox( IN const USHORT& usBoxType_ )
{
    return ( m_mapBoxGPRate.find( usBoxType_ ) != m_mapBoxGPRate.end() );
}

bool KSpecialReward::IsItemBox( IN const USHORT& usBoxType_ )
{
    return ( m_setItemBox.find( usBoxType_ ) != m_setItemBox.end() );
}