#include "MonsterCardMix.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMonsterCardMix );
ImplOstmOperatorW2A( KMonsterCardMix );
NiImplementRootRTTI( KMonsterCardMix );

KMonsterCardMix::KMonsterCardMix(void)
{
}

KMonsterCardMix::~KMonsterCardMix(void)
{
}

ImplToStringW( KMonsterCardMix )
{
    KLocker lock1( m_csMonsterCardMixEnable );
    KLocker lock2( m_csMonsterCardMixConfig );

    return START_TOSTRINGW
        << TOSTRINGWb( m_bMonsterCardMixEnable )
        << TOSTRINGW( m_usMinMaterialCount )
        << TOSTRINGW( m_usMaxMaterialCount )
        << TOSTRINGW( m_mapGradePoint.size() )
        << TOSTRINGW( m_mapPointReward.size() )
        << TOSTRINGW( m_mapRewardGroup.size() )
        << TOSTRINGW( m_mapPointRewardForPacket.size() )
        << TOSTRINGW( m_mapRewardGroupForPacket.size() );
}

bool KMonsterCardMix::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMonsterCardMix::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitMonsterCardMix.lua";
    strScript_ = strScriptName;

    bool bEnable = false;
    USHORT usMinMaterialCount = 0;
    USHORT usMaxMaterialCount = 0;
    std::map<int,USHORT> mapGradePoint;
    std::map<PAIR_USHORT,Kairo> mapPointReward;
    std::map<USHORT,Kairo> mapRewardGroup;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MonsterCardMixEnable", bEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MinMaterialCount", usMinMaterialCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxMaterialCount", usMaxMaterialCount ) == S_OK, return false );
    _JIF( LoadGradePoint( kLuaMng, mapGradePoint ), return false );
    _JIF( LoadPointReward( kLuaMng, mapPointReward ), return false );
    _JIF( LoadRewardGroup( kLuaMng, mapRewardGroup ), return false );

    START_LOG( cerr, L"Load MonsterCardMix Script.." )
        << BUILD_LOG( bEnable )
        << BUILD_LOG( usMinMaterialCount )
        << BUILD_LOG( usMaxMaterialCount )
        << BUILD_LOG( mapGradePoint.size() )
        << BUILD_LOG( mapPointReward.size() )
        << BUILD_LOG( mapRewardGroup.size() ) << END_LOG;

    {
        KLocker lock(m_csMonsterCardMixEnable);
        m_bMonsterCardMixEnable = bEnable;
    }

    {
        KLocker lock(m_csMonsterCardMixConfig);
        m_usMinMaterialCount = usMinMaterialCount;
        m_usMaxMaterialCount = usMaxMaterialCount;
        m_mapGradePoint.swap( mapGradePoint );
        m_mapPointReward.swap( mapPointReward );
        m_mapRewardGroup.swap( mapRewardGroup );
    }

    return true;
}

bool KMonsterCardMix::LoadGradePoint( IN KLuaManager& kLuaMng_, OUT std::map<int,USHORT>& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "Point" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nGrade = 0;
        USHORT usPoint = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, usPoint ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapList_[nGrade] = usPoint;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KMonsterCardMix::LoadPointReward( IN KLuaManager& kLuaMng_, OUT std::map<PAIR_USHORT,Kairo>& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "PointRewardRatio" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        PAIR_USHORT prPointRange;
        Kairo kKairo;

        std::vector<USHORT>	vecRewarGroup;
        vecRewarGroup.clear();

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.BeginTable( "PointRange" ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 1, prPointRange.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prPointRange.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( LoadCaseRatioForPointReward( kLuaMng_, std::string("RewardGroup"), kKairo, vecRewarGroup ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapList_[prPointRange] = kKairo;
        m_mapPointRewardForPacket.insert( std::make_pair( prPointRange, vecRewarGroup ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KMonsterCardMix::LoadRewardGroup( IN KLuaManager& kLuaMng_, OUT std::map<USHORT,Kairo>& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( "RewardGroup" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        USHORT GroupNum = 0;
        Kairo kKairo;

        std::vector<GCITEMID> vecCardList;
        vecCardList.clear();

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "GroupNum", GroupNum ) == S_OK, return false );
        _JIF( LoadCaseRatioForRewardGroup( kLuaMng_, std::string("RewardRatio"), kKairo, vecCardList ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapList_[GroupNum] = kKairo;
        m_mapRewardGroupForPacket.insert( std::make_pair( GroupNum, vecCardList ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KMonsterCardMix::LoadCaseRatioForPointReward( IN KLuaManager& kLuaMng_, IN const std::string& strScript_, OUT Kairo& kKairo_, OUT std::vector<USHORT>& vecRewarGroup_ )
{
    _JIF( kLuaMng_.BeginTable( strScript_.c_str() ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {
        DWORD dwType = 0;
        float fRatio = 0.f;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == kKairo_.SetPostType( dwType, fRatio ) ) {
            START_LOG( cwarn, L"Case Ratio Err.. Type : " << dwType << L", Ratio : " << fRatio ) << END_LOG;
            return false;
        }

        vecRewarGroup_.push_back( static_cast<USHORT>(dwType) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KMonsterCardMix::LoadCaseRatioForRewardGroup( IN KLuaManager& kLuaMng_, IN const std::string& strScript_, OUT Kairo& kKairo_, OUT std::vector<GCITEMID>& vecCardList_ )
{
    _JIF( kLuaMng_.BeginTable( strScript_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        DWORD dwType = 0;
        float fRatio = 0.f;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == kKairo_.SetPostType( dwType, fRatio ) ) {
            START_LOG( cwarn, L"Case Ratio Err.. Type : " << dwType << L", Ratio : " << fRatio ) << END_LOG;
            return false;
        }

        vecCardList_.push_back( dwType );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KMonsterCardMix::GetMixReward( IN const std::vector<int>& vecMaterialGrade_, OUT GCITEMID& RewardItemID_ )
{
    RewardItemID_ = 0;
    USHORT usPoint = 0;
    USHORT usRewardGroup = 0;

    std::vector<int>::const_iterator vit;
    for ( vit = vecMaterialGrade_.begin() ; vit != vecMaterialGrade_.end() ; ++vit ) {
        usPoint += GetGradePoint( *vit );
    }

    usRewardGroup = GetPointReward( usPoint );
    RewardItemID_ = GetRewardItemID( usRewardGroup );

    START_LOG( clog, L"All Point : " << usPoint << L", RewardGroup : " << usRewardGroup << L", RewardItemID : " << RewardItemID_ ) << END_LOG;
    return ( RewardItemID_ != 0 );
}

// 클라이언트에 보내 줄 정보 세팅
void KMonsterCardMix::GetMonsterCardInfoForPacket( OUT KEVENT_MONSTER_CARD_MIX_INFO_ACK& kPacket_ )
{
    kPacket_.m_mapGradePoint = m_mapGradePoint;
    kPacket_.m_mapPointReward = m_mapPointRewardForPacket;
    kPacket_.m_mapRewardGroup = m_mapRewardGroupForPacket;
}

USHORT KMonsterCardMix::GetGradePoint( IN const int& nGrade_ )
{
    KLocker lock( m_csMonsterCardMixConfig );

    std::map<int,USHORT>::iterator mit;
    mit = m_mapGradePoint.find( nGrade_ );
    if ( mit == m_mapGradePoint.end() ) {
        START_LOG( cwarn, L"등급테이블에 없는 등급값 : " << nGrade_ ) << END_LOG;
        return 0;
    }
    return mit->second;
}

USHORT KMonsterCardMix::GetPointReward( IN const USHORT& usPoint_ )
{
    KLocker lock( m_csMonsterCardMixConfig );

    PAIR_USHORT prKey(0,0);
    DWORD dwRewardGroup = 1;

    // 포인트에 맞는 보상범위 찾기
    std::map<PAIR_USHORT,Kairo>::iterator mit;
    for ( mit = m_mapPointReward.begin() ; mit != m_mapPointReward.end() ; ++mit ) {
        const USHORT& usStart = mit->first.first;
        const USHORT& usEnd = mit->first.second;

        if ( usStart <= usPoint_ && usEnd >= usPoint_ ) {
            prKey = mit->first;
            break;
        }

        if ( usStart > usPoint_ ) {
            break;
        }

        prKey = mit->first;
    }

    // 보상그룹 구하기
    mit = m_mapPointReward.find( prKey );
    if ( mit != m_mapPointReward.end() ) {
        LIF( mit->second.Do( dwRewardGroup ) );
    }

    return static_cast<USHORT>( dwRewardGroup );
}

GCITEMID KMonsterCardMix::GetRewardItemID( IN const USHORT& usRewardGroup_ )
{
    KLocker lock( m_csMonsterCardMixConfig );

    DWORD dwReward = 0;

    std::map<USHORT,Kairo>::iterator mit;
    mit = m_mapRewardGroup.find( usRewardGroup_ );
    if ( mit != m_mapRewardGroup.end() ) {
        LIF( mit->second.Do( dwReward ) );
    }

    return static_cast<GCITEMID>( dwReward );
}