#include "GSSimLayer.h"
#include "GameDrop.h"
#include "GCEnum.h"
#include <dbg/dbg.hpp>
#include <KncUtil.h>
#include "Lua/KLuaManager.h"
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include "User.h"
#include <boost/algorithm/string.hpp>  // boost::trim
#include "ResultManager.h"
#include "GCHelper.h"
#include "Log4.h"
#include "VitalityManager.h"
#include <boost/foreach.hpp>

#define REG_TRIGGER_SCRIPT(x,y)     vecTriggerTable.push_back( std::make_pair(x,y) )

void sMonsterInfo::AppendNormalDrop( IN const int nTriggerID_, IN const int nDungeonID_, IN const int nDifficult_,
                                     OUT std::vector< KGameDropElement >& vecMonsterDrop_, IN float fScale_, IN int nStageCount_, 
                                     IN const DWORD& dwDropType_, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo_ )
{
    std::pair<int,int> prKey( nDungeonID_, nDifficult_ );
    std::map<std::pair<int,int>, sMonDrops>::iterator mitTotal;
    std::map<std::pair<int,int>, sMonCharDrops>::iterator mitChar;

    if ( KGameDrop::DT_CHAR == dwDropType_ ) {
        // 플레이하는 캐릭터 드랍일 경우
        mitChar = m_mapMonCharDrops.find( prKey );
        if( mitChar == m_mapMonCharDrops.end() ) {
            START_LOG( clog, L"해당 몬스터는 해당 던전, 난이도에 드랍이 없습니다.." )
                << BUILD_LOG( m_nMonID )
                << BUILD_LOG( nDungeonID_ )
                << BUILD_LOG( nDifficult_ )
                << BUILD_LOG( nTriggerID_ ) << END_LOG;
            return;
        }
        AppendDrop( nTriggerID_, mitChar->second.m_vecDefaultDrop, vecMonsterDrop_, fScale_, nStageCount_ );
        AppendCharGroupDrop( nTriggerID_, mitChar->second.m_mapCharDefaultGroup, vecMonsterDrop_, fScale_, nStageCount_, mapCharLevelInfo_ );
    }
    else {
        // 전체 캐릭터 드랍일 경우( KGameDrop::DT_TOTAL )
        mitTotal = m_mapMonDrops.find( prKey );
        if( mitTotal == m_mapMonDrops.end() ) {
            START_LOG( clog, L"해당 몬스터는 해당 던전, 난이도에 드랍이 없습니다.." )
                << BUILD_LOG( m_nMonID )
                << BUILD_LOG( nDungeonID_ )
                << BUILD_LOG( nDifficult_ )
                << BUILD_LOG( nTriggerID_ ) << END_LOG;
            return;
        }
        AppendDrop( nTriggerID_, mitTotal->second.m_vecDefaultDrop, vecMonsterDrop_, fScale_, nStageCount_ );
        AppendGroupDrop( nTriggerID_, mitTotal->second.m_vecDefaultGroup, vecMonsterDrop_, fScale_, nStageCount_, mapCharLevelInfo_ );
    }
}

void sMonsterInfo::AppendGpDrop( IN const int nTriggerID_, IN const int nMonLv_, IN const int nGP_, IN const sGPDropInfo& kInfo_, OUT std::vector< KGameDropElement >& vecMonsterGpDrop_ )
{
    if( nGP_ <= 0 ) {
        START_LOG( clog, L"해당 몬스터레벨의 GP 보상이 없음." )
            << BUILD_LOG( m_nMonID )
            << BUILD_LOG( nTriggerID_ )
            << BUILD_LOG( nMonLv_ ) << END_LOG;
        return;
    }

    for ( int i = 0 ; i < kInfo_.m_nGoldCount ; ++i )
        AppendGP( nTriggerID_, kInfo_.m_dwGoldID, nGP_, kInfo_.m_fGoldRatio, vecMonsterGpDrop_ );

    for ( int i = 0 ; i < kInfo_.m_nSilverCount ; ++i )
        AppendGP( nTriggerID_, kInfo_.m_dwSilverID, nGP_, kInfo_.m_fSilverRatio, vecMonsterGpDrop_ );
}

void sMonsterInfo::AppendGP(  IN const int nTriggerID_, IN const GCITEMID dwItemID_, IN const int nGP_, IN const float fRatio,  OUT std::vector< KGameDropElement >& vecMonsterGpDrop_ )
{
    KGameDropElement kDropEl;
    kDropEl.m_dwUID             = 0;
    kDropEl.m_dwUserUID         = 0;
    kDropEl.m_nTriggerID        = nTriggerID_;
    kDropEl.m_nMonID            = m_nMonID;
    kDropEl.m_nStageCountType   = -1; // stageCount에 따른 보스 식별(일반 몬스터는 -1)
    kDropEl.m_cRewardType       = 0;
    kDropEl.m_dwItemID          = dwItemID_;
    kDropEl.m_nCount            = nGP_;
    kDropEl.m_nPeriod           = 0;
    kDropEl.m_nStrongLV         = -1;
    kDropEl.m_fRatio            = fRatio;
    vecMonsterGpDrop_.push_back( kDropEl );
}

void sMonsterInfo::AppendChampionDrop( IN const int nTriggerID_, IN const int nDungeonID_, IN const int nDifficult_,
                                       OUT std::vector< KGameDropElement >& vecMonsterDrop_, IN float fScale_, IN const DWORD& dwDropType_, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo_ )
{
    std::pair<int,int> prKey( nDungeonID_, nDifficult_ );
    std::map<std::pair<int,int>, sMonDrops>::iterator mitTotal;
    std::map<std::pair<int,int>, sMonCharDrops>::iterator mitChar;

    if( dwDropType_ == KGameDrop::DT_TOTAL ) {
        mitTotal = m_mapMonDrops.find( prKey );
        if( mitTotal == m_mapMonDrops.end() ) {
            START_LOG( clog, L"해당 몬스터는 해당 던전, 난이도에 드랍이 없습니다.." )
                << BUILD_LOG( m_nMonID )
                << BUILD_LOG( nDungeonID_ )
                << BUILD_LOG( nDifficult_ )
                << BUILD_LOG( nTriggerID_ ) << END_LOG;

            return;
        }
        AppendDrop( nTriggerID_, mitTotal->second.m_vecChampDrop, vecMonsterDrop_, fScale_, -1 );
        AppendGroupDrop( nTriggerID_, mitTotal->second.m_vecChampGroup, vecMonsterDrop_, fScale_, -1, mapCharLevelInfo_ );
    }
    else {
        mitChar = m_mapMonCharDrops.find( prKey );
        if( mitChar == m_mapMonCharDrops.end() ) {
            START_LOG( clog, L"해당 몬스터는 해당 던전, 난이도에 드랍이 없습니다.." )
                << BUILD_LOG( m_nMonID )
                << BUILD_LOG( nDungeonID_ )
                << BUILD_LOG( nDifficult_ )
                << BUILD_LOG( nTriggerID_ ) << END_LOG;

            return;
        }
        AppendDrop( nTriggerID_, mitChar->second.m_vecChampDrop, vecMonsterDrop_, fScale_, -1 );
        AppendCharGroupDrop( nTriggerID_, mitChar->second.m_mapCharChampGroup, vecMonsterDrop_, fScale_, -1, mapCharLevelInfo_ );
    }
}

void sMonsterInfo::AppendDrop( IN const int nTriggerID_,
                               IN std::vector< sMondropElement >& vecDrop_,
                               OUT std::vector< KGameDropElement >& vecMonsterDrop_,
                               IN float fScale_,
                               IN int nStageCount_ )
{
    if( vecDrop_.empty() ) {
        return;
    }

    fScale_ = std::max<float>( 0.f, fScale_ );
    fScale_ = std::min<float>( 1.f, fScale_ );

    float fRatio = 0.f;
    std::vector< sMondropElement >::iterator vit;

    for( vit = vecDrop_.begin() ; vit != vecDrop_.end() ; ++vit )
    {
        fRatio = vit->m_fRatio + (vit->m_fRatio * fScale_);

        if( fRatio < SiKGameDrop()->GetRatio() ) {
            continue;
        }

        KGameDropElement kDropEl;
        kDropEl.m_dwUID             = 0;
        kDropEl.m_dwUserUID         = 0;
        kDropEl.m_nTriggerID        = nTriggerID_;
        kDropEl.m_nMonID            = m_nMonID;
        kDropEl.m_nStageCountType   = nStageCount_; // stageCount에 따른 보스 식별(일반 몬스터는 -1)
        kDropEl.m_cRewardType       = vit->m_cRewardType;
        kDropEl.m_dwItemID          = vit->m_dwItemID;
        kDropEl.m_nCount            = vit->m_nCount;
        kDropEl.m_nPeriod           = vit->m_nPeriod;
        kDropEl.m_nStrongLV         = vit->m_nStrongLv;
        kDropEl.m_fRatio            = vit->m_fRatio;
        vecMonsterDrop_.push_back( kDropEl );
    }
}

void sMonsterInfo::AppendGroupDrop( IN const int nTriggerID_,
                                    IN std::vector< std::pair<KLottery, std::vector<sMondropElement> > >& vecGroupDrop_,
                                    OUT std::vector< KGameDropElement >& vecMonsterDrop_,
                                    IN float fScale_,
                                    IN int nStageCount_,
                                    IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo )
{
    if( vecGroupDrop_.empty() ) {
        return;
    }

    std::vector< std::pair<KLottery, std::vector<sMondropElement> > >::iterator vit;
    for( vit = vecGroupDrop_.begin() ; vit != vecGroupDrop_.end() ; ++vit )
    {
        int nDecision =  vit->first.Decision( fScale_ );
        if( nDecision <= 0 )
            continue;
        std::vector<sMondropElement>& vecDrops = vit->second;
        std::vector<sMondropElement>::iterator vit2;
        vit2 = std::find_if( vecDrops.begin(), vecDrops.end(),
                boost::bind( &sMondropElement::m_dwItemID, _1) == static_cast< DWORD >( nDecision ) );

        if( vit2 == vecDrops.end() ) {
            continue;
        }

        KGameDropElement kDropEl;
        kDropEl.m_dwUID             = 0;
        kDropEl.m_dwUserUID         = 0;
        kDropEl.m_nTriggerID        = nTriggerID_;
        kDropEl.m_nMonID            = m_nMonID;
        kDropEl.m_nStageCountType   = nStageCount_; // stageCount에 따른 보스 식별(일반 몬스터는 -1)
        kDropEl.m_cRewardType       = vit2->m_cRewardType;
        kDropEl.m_dwItemID          = vit2->m_dwItemID;
        kDropEl.m_nCount            = vit2->m_nCount;
        kDropEl.m_nPeriod           = vit2->m_nPeriod;
        kDropEl.m_nStrongLV         = vit2->m_nStrongLv;
        kDropEl.m_fRatio            = vit2->m_fRatio;
        vecMonsterDrop_.push_back( kDropEl );
    }
}

void sMonsterInfo::AppendCharGroupDrop( IN const int nTriggerID_,
                                   IN std::map<char, std::pair<KLottery, std::vector<sMondropElement>>>& mapCharDefaultGroup_,
                                   OUT std::vector< KGameDropElement >& vecMonsterDrop_,
                                   IN float fScale_,
                                   IN int nStageCount_,
                                   IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo_ )
{
    std::map<DWORD, std::pair<char,int> >::const_iterator cmit;
    std::map<char, std::pair<KLottery, std::vector<sMondropElement>>>::iterator mit;

    if( mapCharDefaultGroup_.empty() || mapCharLevelInfo_.empty() ) {
        return;
    }

    DWORD dwSize = mapCharLevelInfo_.size();
    DWORD dwRandomNum = rand()%dwSize;
    DWORD dwReceiveUserID = 0;
    char cSelectCharType = -1;
    DWORD dwLoopCnt = 0;

    // 유저 선택한 캐릭터 정보 가져오기.
    for( cmit = mapCharLevelInfo_.begin(); cmit != mapCharLevelInfo_.end(); ++cmit ) {
        if( dwLoopCnt == dwRandomNum ) {
            dwReceiveUserID = cmit->first;
            cSelectCharType = cmit->second.first;
            break;
        }
        ++dwLoopCnt;
    }

    if( cSelectCharType == -1 ) {
        return;
    }

    mit = mapCharDefaultGroup_.find( cSelectCharType );
    if( mit == mapCharDefaultGroup_.end() ) {
        return;
    }

    int nDecision =  mit->second.first.Decision( fScale_ );
    if( nDecision <= 0 ) {
        return;
    }

    std::vector<sMondropElement>& vecDrops =  mit->second.second;
    std::vector<sMondropElement>::iterator vit2;
    vit2 = std::find_if( vecDrops.begin(), vecDrops.end(),
        boost::bind( &sMondropElement::m_dwItemID, _1) == static_cast< DWORD >( nDecision ) );

    if( vit2 == vecDrops.end() ) {
        return;
    }

    KGameDropElement kDropEl;
    kDropEl.m_dwUID             = 0;
    kDropEl.m_dwUserUID         = dwReceiveUserID; // 받을 유저의 UID를 지정한다
    kDropEl.m_nTriggerID        = nTriggerID_;
    kDropEl.m_nMonID            = m_nMonID;
    kDropEl.m_nStageCountType   = nStageCount_; // stageCount에 따른 보스 식별(일반 몬스터는 -1)

    /* 캐릭터 게임 드랍용 타입
       이 타입을 사용하는 경우 최초 드랍 리스트 생성시 미리 지정된 유저에게 아이템을 우선 지급한다
       차원의 문(무한 던전) 드랍에 게임 드랍들을 붙이면서 타입이 추가되었다
       기존의 버그성(받을 유저가 미리 지정되었지만 실제 획득 시점에는 새로 주사위를 굴리는) 동작을 수정하는 목적이 있다
       CharGameDrop.lua 스크립트에 직접 별도의 타입을 지정하는 형태로 변경하는 형태를 고려하였으나
       수정 이후의 불필요한 혼란을 방지하겠다는 핑계로 작업하지 않았다 */
    kDropEl.m_cRewardType       = KGameDropElement::RT_CHARDROP_ITEM;

    kDropEl.m_dwItemID          = vit2->m_dwItemID;
    kDropEl.m_nCount            = vit2->m_nCount;
    kDropEl.m_nPeriod           = vit2->m_nPeriod;
    kDropEl.m_nStrongLV         = vit2->m_nStrongLv;
    kDropEl.m_fRatio            = vit2->m_fRatio;
    vecMonsterDrop_.push_back( kDropEl );
}

//-----------------------------------

DWORD sChampionInfo::GetProperty()
{
    DWORD dwProperty = 0L;
    int nPropertyCount = m_kPropertyCount.Decision();

    nPropertyCount = std::max<int>( 0, nPropertyCount );
    nPropertyCount = std::min<int>( nPropertyCount, (int)m_vecProperty.size() );

    if( nPropertyCount <= 0 ) return 0L;

    std::random_shuffle( m_vecProperty.begin(), m_vecProperty.end() );
    std::vector< KLottery >::iterator vit = m_vecProperty.begin();
    while( nPropertyCount > 0 )
    {
        if( vit == m_vecProperty.end() ) break;

        dwProperty += (DWORD)(std::max<int>( vit->Decision(), 0 ));
        ++vit;
        --nPropertyCount;
    }

    return dwProperty;
}
//----------------------

ImplementSingleton( KGameDrop );
ImplOstmOperatorW2A( KGameDrop );

std::wostream& operator<< ( std::wostream& stm_, const std::pair<const int,sMonsterInfo >& data_ )
{
    return stm_ << data_.first << L"(" << data_.second.m_mapMonDrops.size() << L")";
}
std::wostream& operator<< ( std::wostream& stm_, const std::pair<const DWORD,std::vector<sMonPropertyDropInfo> >& data_ )
{
    return stm_ << data_.first << L"(" << data_.second.size() << L")";
}

std::wostream& operator<< ( std::wostream& stm_, const std::pair<const __int64,sChampionInfo >& data_ )
{
    KGameDrop::UChampKey key;
    key.m_biChampKey = data_.first;
    return stm_ << L"(" << key.m_uaKeyList[0] << L":" << key.m_uaKeyList[1] << L":" << key.m_uaKeyList[2] << L")";
}

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const DWORD,T>& data_ ) { return stm_ << data_.first; }

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const int,T>& data_ ) { return stm_ << data_.first; }

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const std::pair<int,char>,T>& data_ )
{
    return stm_ << data_.first.first << L"(" << (int)data_.first.second << L")";
}

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const std::pair<int,int>,T>& data_ )
{
    return stm_ << data_.first.first << L"(" << data_.first.second << L")";
}

ImplToStringW( KGameDrop )
{
    KLocker lock( m_csMonsterInfo );
    KLocker lock1( m_csDungeonTrigger );
    KLocker lock2( m_csChampionTable );
    KLocker Lock3( m_csGPDrop );

    stm_ << TOSTRINGW( m_mapMonsterInfo.size() )
        << TOSTRINGW( m_mapDungeonTrigger.size() )
        << TOSTRINGW( m_mapChampionTable.size() )
        << TOSTRINGW( m_fSilverCoinRatio )
        << TOSTRINGW( m_fGoldCoinRatio )
        << TOSTRINGW( m_fJackPotRatio )
        << TOSTRINGW( m_nJacpotCoinCount );

    stm_ << std::endl << L" Mon Drop : ";

    std::copy( m_mapMonsterInfo.begin(), m_mapMonsterInfo.end(), 
        std::ostream_iterator<std::map< int, sMonsterInfo >::value_type,wchar_t>( stm_, L", " ) );

    stm_ << std::endl << std::endl << L" DungeonTrigger : ";

    std::copy( m_mapDungeonTrigger.begin(), m_mapDungeonTrigger.end(), 
        std::ostream_iterator<std::map<std::pair<int,int>, std::map<int,std::vector<sTriggerInfo> > >::value_type,wchar_t>( stm_, L", " ) );

    stm_ << std::endl << std::endl << L" Champion : ";

    std::copy( m_mapChampionTable.begin(), m_mapChampionTable.end(), 
        std::ostream_iterator<std::map<__int64, sChampionInfo>::value_type,wchar_t>( stm_, L", " ) );

    return stm_;

}

KGameDrop::KGameDrop(void)
:uint32( 1, UINT_MAX )
,die(rng, uint32)
,m_fMultiplyRatio(0)
,m_fSilverCoinRatio(0.f)
,m_fGoldCoinRatio(0.f)
,m_fJackPotRatio(0.f)
,m_nJacpotCoinCount(0)

{
    rng.seed( ::GetTickCount() );
    m_mapMonsterInfo.clear();
    m_mapMonsterCharInfo.clear();
    m_mapMonsterDropRatio.clear();
}

KGameDrop::~KGameDrop(void)
{
}

bool KGameDrop::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGameDrop::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadMonsterScript(strScript_), return false );
    _JIF( LoadTriggerScript(strScript_), return false );
    _JIF( LoadChampionScript(strScript_), return false );
    _JIF( LoadGPDropInfo(strScript_), return false );
    _JIF( LoadMonsterRewardScript(strScript_), return false );
    _JIF( LoadCharMonsterScript(strScript_), return false );
    _JIF( LoadMonsterDropRatio(strScript_), return false );

    return true;
}

bool KGameDrop::LoadMonsterScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< int, sMonsterInfo > mapMonsterInfo;

    std::string strScriptName = "GameDrop.lua";
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "Mon" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) == S_OK )
        {
            sMonsterInfo monInfo;
            _JIF( LoadMonsterInfo( kLuaMng, monInfo ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );           

            if( mapMonsterInfo.find( monInfo.m_nMonID ) != mapMonsterInfo.end() )
            {
                START_LOG( cerr, L" 동일한 몬스터 정보가 존재합니다. Mon ID : " << monInfo.m_nMonID ) << END_LOG;
                return false;
            }
            mapMonsterInfo[monInfo.m_nMonID] = monInfo;
        }
        else
            break;
    }    
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"몬스터 정보 로드 완료,... Size : " << mapMonsterInfo.size() ) << END_LOG;
    KLocker lock( m_csMonsterInfo );
    m_mapMonsterInfo.swap( mapMonsterInfo );
    return true;
}

bool KGameDrop::LoadDropElement( IN OUT KLuaManager& kLuaMng_, OUT sMondropElement& dropElement_ )
{
    std::string strType;
    _JIF( 0 == kLuaMng_.GetValue( "RewardType", strType ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "ItemID", dropElement_.m_dwItemID ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "Count", dropElement_.m_nCount ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "Period", dropElement_.m_nPeriod ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "Strong", dropElement_.m_nStrongLv ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "Ratio", dropElement_.m_fRatio ), return false );

    //RT_EXP      = 0,
    //RT_GP       = 1,
    //RT_ITEM     = 2,
    boost::to_lower( strType );
    if( strType.compare( "item" ) == 0 )
        dropElement_.m_cRewardType = 2;
    else if( strType.compare( "gp" ) == 0 )
        dropElement_.m_cRewardType = 1;
    else
    {
        START_LOG( cerr, L" 알수 없는 보상 타입 입니다. Type : " << KncUtil::toWideString( strType ) ) << END_LOG;
        return false;
    }

    ApplyBoostRatio( dropElement_.m_fRatio );

    return true;
}

bool KGameDrop::LoadMonsterInfo( IN OUT KLuaManager& kLuaMng_, OUT sMonsterInfo& monInfo_ )
{
    monInfo_.m_nMonID = -1;
    monInfo_.m_mapMonDrops.clear();

    _JIF( 0 == kLuaMng_.GetValue( "MonID", monInfo_.m_nMonID ) , return false );

    // Drop 정보 읽기..
    _JIF( kLuaMng_.BeginTable( "DropInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        sMonDrops monDrops;
        _JIF( 0 == kLuaMng_.GetValue( "DungeonID", monDrops.m_nDungeonID ), return false );
        _JIF( 0 == kLuaMng_.GetValue( "Difficult", monDrops.m_nDifficult ), return false );

//         if( monDrops.m_nDifficult > 0 )
//             --monDrops.m_nDifficult;

        // Drop Table 읽기..
        if( kLuaMng_.BeginTable( "Drop" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            sMondropElement monDropEle;
            _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monDrops.m_vecDefaultDrop.push_back( monDropEle );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // DropGroup 읽기
        if( kLuaMng_.BeginTable( "Drop_Group" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            KLottery kLottery;
            std::vector<sMondropElement> vecDropElement;

            for( int k = 1 ; ; ++k )
            {
                if( kLuaMng_.BeginTable( k ) != S_OK ) break;
                sMondropElement monDropEle;
                _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
                vecDropElement.push_back( monDropEle );
                kLottery.AddCase( (int)monDropEle.m_dwItemID, monDropEle.m_fRatio * 100.f );
            }
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monDrops.m_vecDefaultGroup.push_back( std::make_pair(kLottery, vecDropElement) );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // Champion Drop Table 읽기.
        if( kLuaMng_.BeginTable( "ChampionDrop" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            sMondropElement monDropEle;
            _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monDrops.m_vecChampDrop.push_back( monDropEle );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // ChampionDrop_Group 읽기
        if( kLuaMng_.BeginTable( "ChampionDrop_Group" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            KLottery kLottery;
            std::vector<sMondropElement> vecDropElement;

            for( int k = 1 ; ; ++k )
            {
                if( kLuaMng_.BeginTable( k ) != S_OK ) break;
                sMondropElement monDropEle;
                _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
                vecDropElement.push_back( monDropEle );
                kLottery.AddCase( (int)monDropEle.m_dwItemID, monDropEle.m_fRatio * 100.f );
            }
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monDrops.m_vecChampGroup.push_back( std::make_pair(kLottery, vecDropElement) );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        //------------------------------------------------//
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        std::pair<int,int> prKey( monDrops.m_nDungeonID, monDrops.m_nDifficult );
        monInfo_.m_mapMonDrops.insert( std::make_pair( prKey,monDrops ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}
bool KGameDrop::LoadTriggerScript( OUT std::string& strScript_ )
{
    std::vector< std::pair<int,std::string> > vecTriggerTable;

    // ModeID/Table
    REG_TRIGGER_SCRIPT( GC_GM_QUEST0  , std::string( "Dungeon_TrialForest" ) );             // 시련의 숲
    REG_TRIGGER_SCRIPT( GC_GM_QUEST1  , std::string( "Dungeon_TrialTower" ) );              // 시련의 탑
    REG_TRIGGER_SCRIPT( GC_GM_QUEST2  , std::string( "Dungeon_SerdinOutside" ) );           // 세르딘성 외곽
    REG_TRIGGER_SCRIPT( GC_GM_QUEST3  , std::string( "Dungeon_CarryBeach" ) );              // 캐리비치
    REG_TRIGGER_SCRIPT( GC_GM_QUEST4  , std::string( "Dungeon_OrcsTemple" ) );              // 오크사원
    REG_TRIGGER_SCRIPT( GC_GM_QUEST5  , std::string( "Dungeon_GorgosDungeon" ) );           // 고르고스 던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST6  , std::string( "Dungeon_ElvenForest" ) );             // 엘프의 숲
    REG_TRIGGER_SCRIPT( GC_GM_QUEST7  , std::string( "Dungeon_OathGorge" ) );               // 맹세의 계곡
    REG_TRIGGER_SCRIPT( GC_GM_QUEST8  , std::string( "Dungeon_ObilivionSwamp" ) );          // 망각의 늪
    REG_TRIGGER_SCRIPT( GC_GM_QUEST9  , std::string( "Dungeon_DeadsTomb" ) );               // 망자의 묘지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST10 , std::string( "Dungeon_ForgattenCity" ) );           // 잊혀진 도시
    REG_TRIGGER_SCRIPT( GC_GM_QUEST11 , std::string( "Dungeon_GaikozsCastle" ) );           // 가이코즈의 성
    REG_TRIGGER_SCRIPT( GC_GM_QUEST12 , std::string( "Dungeon_PatuseisSea" ) );             // 파투세이의 바다
    REG_TRIGGER_SCRIPT( GC_GM_QUEST13 , std::string( "Dungeon_Elia_Continent" ) );          // 엘리아 대륙
    REG_TRIGGER_SCRIPT( GC_GM_QUEST14 , std::string( "Dungeon_TempleOfFire" ) );            // 불의 사원
    REG_TRIGGER_SCRIPT( GC_GM_QUEST15 , std::string( "Dungeon_HellBridge" ) );              // 헬 브릿지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST16 , std::string( "Dungeon_KazeazesCastle" ) );          // 카제아제의 성
    REG_TRIGGER_SCRIPT( GC_GM_QUEST17 , std::string( "Dungeon_CastulUnderFloor" ) );        // 카스툴 유적지 하층
    REG_TRIGGER_SCRIPT( GC_GM_QUEST18 , std::string( "Dungeon_CastulOverFloor" ) );         // 카스툴 유적지 상층
    REG_TRIGGER_SCRIPT( GC_GM_QUEST19 , std::string( "Dungeon_Mesias_Final" ) );            // 베르메시아의 최후
    REG_TRIGGER_SCRIPT( GC_GM_QUEST20 , std::string( "Dungeon_New_World" ) );               // 제니아 접경지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST21 , std::string( "Dungeon_Temple_Of_Circulation" ) );   // 순환의 신전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST22 , std::string( "Dungeon_Rock_Canyon" ) );             // 소실의 협곡
    REG_TRIGGER_SCRIPT( GC_GM_QUEST23 , std::string( "Dungeon_Angry_Forest" ) );            // 성난 엔트의 숲
    REG_TRIGGER_SCRIPT( GC_GM_QUEST24 , std::string( "Dungeon_SiverKnight_Ruins" ) );       // 실버나이츠의 폐허
    REG_TRIGGER_SCRIPT( GC_GM_QUEST25 , std::string( "Dungeon_Water" ) );                   // 미명의 호수
    REG_TRIGGER_SCRIPT( GC_GM_QUEST26 , std::string( "Dungeon_Fire" ) );                    // 이그니스의 숲
    REG_TRIGGER_SCRIPT( GC_GM_QUEST27 , std::string( "Dungeon_Swamp" ) );                   // 늪던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST28 , std::string( "Dungeon_Island_Of_Nature" ) );        // 실버랜드 원시의 섬
    REG_TRIGGER_SCRIPT( GC_GM_QUEST29 , std::string( "Dungeon_LavaContinent" ) );           // 실버랜드 드레이크 웜
    REG_TRIGGER_SCRIPT( GC_GM_QUEST30 , std::string( "Dungeon_VictorFortress" ) );          // 실버랜드 빅터의 요새
    REG_TRIGGER_SCRIPT( GC_GM_QUEST31 , std::string( "Dungeon_Altar_Of_Harmony" ) );        // 제니아 빙하 조율의 제단
    REG_TRIGGER_SCRIPT( GC_GM_QUEST32 , std::string( "Dungeon_GiantRock" ) );               // 제니아 파괴의 신전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST33 , std::string( "Dungeon_LifeWoodland" ) );            // 제니아 생명의 숲
    REG_TRIGGER_SCRIPT( GC_GM_QUEST34 , std::string( "Dungeon_Castle_Of_Rule" ) );          // 제니아 지배의 성
    REG_TRIGGER_SCRIPT( GC_GM_QUEST35 , std::string( "Dungeon_35" ) );                   // 베르메시아 보스 던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST36 , std::string( "Dungeon_36" ) );                   // 실버랜드 보스 던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST37 , std::string( "Dungeon_37" ) );                   // 엘리아 보스 던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST38 , std::string( "Dungeon_38" ) );                   // 제니아 보스 던전
    REG_TRIGGER_SCRIPT( GC_GM_QUEST39 , std::string( "Dungeon_39" ) );                   // GoblinLaborCamp
    REG_TRIGGER_SCRIPT( GC_GM_QUEST40 , std::string( "Dungeon_40" ) );                   // UndergroundTunnel
    REG_TRIGGER_SCRIPT( GC_GM_QUEST41 , std::string( "Dungeon_41" ) );                   // KungjiVillage
    REG_TRIGGER_SCRIPT( GC_GM_QUEST42 , std::string( "Dungeon_42" ) );                   // DwarvenBase
    REG_TRIGGER_SCRIPT( GC_GM_QUEST43 , std::string( "Dungeon_43" ) );                   // 신기루 사막
    REG_TRIGGER_SCRIPT( GC_GM_QUEST44 , std::string( "Dungeon_44" ) );                   // 드워프 보급로
    REG_TRIGGER_SCRIPT( GC_GM_QUEST45 , std::string( "Dungeon_45" ) );                   // 썬더 해머
    REG_TRIGGER_SCRIPT( GC_GM_QUEST46 , std::string( "Dungeon_46" ) );                   // 영웅던전 엘리아
    REG_TRIGGER_SCRIPT( GC_GM_QUEST47 , std::string( "Dungeon_47" ) );                   // 영웅던전 베르메시아
    REG_TRIGGER_SCRIPT( GC_GM_QUEST48 , std::string( "Dungeon_48" ) );                   // 고대왕국의잔해
    REG_TRIGGER_SCRIPT( GC_GM_QUEST49 , std::string( "Dungeon_49" ) );                   // Event Carnival
    REG_TRIGGER_SCRIPT( GC_GM_QUEST50 , std::string( "Dungeon_50" ) );                   // Event RelayDungeon
    REG_TRIGGER_SCRIPT( GC_GM_QUEST51 , std::string( "Dungeon_51" ) );                   // Event Hero
    REG_TRIGGER_SCRIPT( GC_GM_QUEST52 , std::string( "Dungeon_52" ) );                   // 릴레이 영던
    REG_TRIGGER_SCRIPT( GC_GM_QUEST53 , std::string( "Dungeon_53" ) );                   // Monster Train
    REG_TRIGGER_SCRIPT( GC_GM_QUEST54 , std::string( "Dungeon_54" ) );                   // 배고픈 펫들의 반란.
    REG_TRIGGER_SCRIPT( GC_GM_QUEST55 , std::string( "Dungeon_55" ) );                   // 카루엘 접경지(KaruelBorder)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST56 , std::string( "Dungeon_56" ) );                   // 카루엘(Karuel)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST57 , std::string( "Dungeon_57" ) );                   // 이벤트던전(야시장)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST58 , std::string( "Dungeon_58" ) );                   // 레이드던전.
    REG_TRIGGER_SCRIPT( GC_GM_QUEST59 , std::string( "Dungeon_59" ) );                   // 사막도적마을
    REG_TRIGGER_SCRIPT( GC_GM_QUEST60 , std::string( "Dungeon_60" ) );                   // 라 협곡
    REG_TRIGGER_SCRIPT( GC_GM_QUEST61 , std::string( "Dungeon_61" ) );                   // 세트의 피라미드
    REG_TRIGGER_SCRIPT( GC_GM_QUEST62 , std::string( "Dungeon_62" ) );                   // 코우나트 붕괴지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST63 , std::string( "Dungeon_63" ) );                   // 코우나트 멘붕괴지( 이벤트 던전 )
    REG_TRIGGER_SCRIPT( GC_GM_QUEST64 , std::string( "Dungeon_64" ) );                   // 이벤트 던전(슈팅던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST65 , std::string( "Dungeon_65" ) );                   // 아델 - 저주받은 정원
    REG_TRIGGER_SCRIPT( GC_GM_QUEST66 , std::string( "Dungeon_66" ) );                   // 아델 - 오염된 지하실
    REG_TRIGGER_SCRIPT( GC_GM_QUEST67 , std::string( "Dungeon_67" ) );                   // 아델 - 망령이 깃든 복도
    REG_TRIGGER_SCRIPT( GC_GM_QUEST68 , std::string( "Dungeon_68" ) );                   // 아델 - 배신자의 화실
    REG_TRIGGER_SCRIPT( GC_GM_QUEST69 , std::string( "Dungeon_69" ) );                   // 아델 - 어둠의 물들 서제
    REG_TRIGGER_SCRIPT( GC_GM_QUEST70 , std::string( "Dungeon_70" ) );                   // 아델 - 아라크네 둥지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST71 , std::string( "Dungeon_71" ) );                   // 베이가스 - 골리아스 서식지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST72 , std::string( "Dungeon_72" ) );                   // 베이가스 - 고대 골렘의 무덤
    REG_TRIGGER_SCRIPT( GC_GM_QUEST73 , std::string( "Dungeon_73" ) );                   // 베이가스 - 그림자 밀림
    REG_TRIGGER_SCRIPT( GC_GM_QUEST74 , std::string( "Dungeon_74" ) );                   // 베이가스 - 라세르타의 사냥터
    REG_TRIGGER_SCRIPT( GC_GM_QUEST75 , std::string( "Dungeon_75" ) );                   // 베이가스 - 절망의 대지
    REG_TRIGGER_SCRIPT( GC_GM_QUEST76 , std::string( "Dungeon_76" ) );                   // 베이가스 - 차원의 경계
    REG_TRIGGER_SCRIPT( GC_GM_QUEST77 , std::string( "Dungeon_77" ) );                   // 엘리아 전장(사냥터)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST78 , std::string( "Dungeon_78" ) );                   // 제니아 전장(사냥터)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST79 , std::string( "Dungeon_79" ) );                   // 아툼 전장(사냥터)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST80 , std::string( "Dungeon_80" ) );                   // 아케메디아 전장(사냥터)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST81 , std::string( "Dungeon_81" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST82 , std::string( "Dungeon_82" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST83 , std::string( "Dungeon_83" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST84 , std::string( "Dungeon_84" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST85 , std::string( "Dungeon_85" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST86 , std::string( "Dungeon_86" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST87 , std::string( "Dungeon_87" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST88 , std::string( "Dungeon_88" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST89 , std::string( "Dungeon_89" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST90 , std::string( "Dungeon_90" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST91 , std::string( "Dungeon_91" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST92 , std::string( "Dungeon_92" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST93 , std::string( "Dungeon_93" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST94 , std::string( "Dungeon_94" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST95 , std::string( "Dungeon_95" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST96 , std::string( "Dungeon_96" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST97 , std::string( "Dungeon_97" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST98 , std::string( "Dungeon_98" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST99 , std::string( "Dungeon_99" ) );                   // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST100 , std::string( "Dungeon_100" ) );                 // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST101 , std::string( "Dungeon_101" ) );                 // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST102 , std::string( "Dungeon_102" ) );                 // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST103  , std::string( "Dungeon_103" ) );                 // 소멸의 탑(만렙컨트롤던전)
    REG_TRIGGER_SCRIPT( GC_GM_QUEST104  , std::string( "Dungeon_104" ) );                 // 소멸의 탑(만렙컨트롤던전)

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitTriggerInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    std::map< std::pair<int,int>, std::map<int,std::vector<sTriggerInfo> > > mapDungeonTrigger;
    _JIF( kLuaMng.BeginTable( "TriggerInfo" ) == S_OK, return false );
    
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        int nModeID;
        _JIF( 0 == kLuaMng.GetValue( "ID", nModeID ), return false );

        std::vector< std::pair<int,std::string> >::iterator vit;
        vit = std::find_if( vecTriggerTable.begin(), vecTriggerTable.end(),
                boost::bind( &std::pair<int,std::string>::first, _1) == nModeID );

        if( vit == vecTriggerTable.end() )
        {
            START_LOG( cerr, L"알수 없는 던젼의 트리거 정보 입니다. ID: " << nModeID ) << END_LOG;
            return false;
        }

        // 던전 난이도별 트리거 정보
        // 일반 난이도
        for ( int j = GC_GMD_NORMAL_BEGIN ; j < GC_GMD_CHAMPION_BEGIN ; ++j ) {
            const int& nDifficulty = j;
            std::stringstream stm;
            stm << "Lv_" << nDifficulty;

            if ( kLuaMng.BeginTable( stm.str().c_str() ) != S_OK ) {
                break;
            }

            std::map<int,std::vector< sTriggerInfo > > mapTrigger;
            if ( false == LoadTriggerTable( kLuaMng, mapTrigger ) ) {
                START_LOG( cerr, L"일반난이도 트리거 정보 읽는중 오류 Dungeon : " << KncUtil::toWideString( vit->second ) )
                    << BUILD_LOG( KncUtil::toWideString(stm.str()) )
                    << BUILD_LOG( nDifficulty ) << END_LOG;
                return false;
            }
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            std::pair<int,int> prKey( nModeID, nDifficulty );
            if ( mapDungeonTrigger.find( prKey ) != mapDungeonTrigger.end() ) {
                START_LOG( cerr, L"이미 중복된 퀘스트와 레벨이 있음. Key : " << prKey.first << L" / " << prKey.second ) << END_LOG;
                continue;
            }
            mapDungeonTrigger[prKey] = mapTrigger;
        }

        // 챔피언 난이도
        for ( int j = GC_GMD_CHAMPION_BEGIN ; ; ++j ) {
            const int& nDifficulty = j;
            std::stringstream stm;
            stm << "Lv_" << nDifficulty;

            if ( kLuaMng.BeginTable( stm.str().c_str() ) != S_OK ) {
                break;
            }

            std::map<int,std::vector< sTriggerInfo > > mapTrigger;
            if ( false == LoadTriggerTable( kLuaMng, mapTrigger ) ) {
                START_LOG( cerr, L"챔피언난이도 트리거 정보 읽는중 오류 Dungeon : " << KncUtil::toWideString( vit->second ) )
                    << BUILD_LOG( KncUtil::toWideString(stm.str()) )
                    << BUILD_LOG( nDifficulty ) << END_LOG;
                return false;
            }
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            std::pair<int,int> prKey( nModeID, nDifficulty );
            if ( mapDungeonTrigger.find( prKey ) != mapDungeonTrigger.end() ) {
                START_LOG( cerr, L"이미 중복된 퀘스트와 레벨이 있음. Key : " << prKey.first << L" / " << prKey.second ) << END_LOG;
                continue;
            }
            mapDungeonTrigger[prKey] = mapTrigger;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"트리거 정보 읽기 완료.. Size : " << mapDungeonTrigger.size() ) << END_LOG;
    KLocker lock( m_csDungeonTrigger );
    m_mapDungeonTrigger.swap( mapDungeonTrigger );
    return true;
}

bool KGameDrop::LoadTriggerTable( IN OUT KLuaManager& kLuaMng_, OUT std::map<int,std::vector< sTriggerInfo > >& mapTrigger_ )
{
    mapTrigger_.clear();
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        sTriggerInfo triggerInfo;
        _JIF( 0 == kLuaMng_.GetValue( 1, triggerInfo.m_nTriggerID ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, triggerInfo.m_nMonsterID ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 3, triggerInfo.m_nMonLv ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 4, triggerInfo.m_nStage ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 5, triggerInfo.m_nIsBoss ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        mapTrigger_[triggerInfo.m_nStage].push_back( triggerInfo );
    }
    return true;
}

bool KGameDrop::LoadChampionScript( OUT std::string& strScript_ )
{
    std::map<__int64, sChampionInfo> mapChampionTable;
    mapChampionTable.clear();

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitChampion.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "ChampionInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) == S_OK )
        {
            sChampionInfo sChampInfo;
            _JIF( LoadChampionInfo( kLuaMng, sChampInfo ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            UChampKey uChampKey;
            uChampKey.m_uaKeyList[0] = sChampInfo.m_nDungeonID;
            uChampKey.m_uaKeyList[1] = sChampInfo.m_nDungeonLv;
            uChampKey.m_uaKeyList[2] = sChampInfo.m_nMobGrade;
            uChampKey.m_uaKeyList[3] = 0;

            if( !mapChampionTable.insert( std::make_pair( uChampKey.m_biChampKey, sChampInfo ) ).second )
            {
                START_LOG( cerr, L"챔피언 정보중 중복 되는것이 있음. File : " << L"InitChampion.lua" )
                    << BUILD_LOG( uChampKey.m_uaKeyList[0] )
                    << BUILD_LOG( uChampKey.m_uaKeyList[1] )
                    << BUILD_LOG( uChampKey.m_uaKeyList[2] )
                    << BUILD_LOG( uChampKey.m_uaKeyList[3] )
                    << BUILD_LOG( uChampKey.m_biChampKey )
                    << END_LOG;
                return false;
            }
        }
        else
            break;
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 챔피언 정보 로드 완료.. Size : " << mapChampionTable.size() ) << END_LOG;

    KLocker lock( m_csChampionTable );
    m_mapChampionTable.swap( mapChampionTable );

    return true;
}

// 공통으로 쓰자...
bool KGameDrop::LoadChampionInfo( IN OUT KLuaManager& kLuaMng_, OUT sChampionInfo& champInfo_ )
{
    _JIF( 0 == kLuaMng_.GetValue( "DungeonID", champInfo_.m_nDungeonID ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "DungeonLv", champInfo_.m_nDungeonLv ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "MobGrade", champInfo_.m_nMobGrade ), return false );

    // ChampionCount 읽기..
    _JIF( kLuaMng_.BeginTable( "ChampionCount" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            int nCase = 0;
            float fRatio = 0.f;
            _JIF( 0 == kLuaMng_.GetValue( 1, nCase ), return false );
            _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            champInfo_.m_kChampionCount.AddCase( nCase, fRatio );
        }
        else
            break;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    //Property Count 읽기..
    _JIF( kLuaMng_.BeginTable( "PropertyCount" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            int nCase = 0;
            float fRatio = 0.f;
            _JIF( 0 == kLuaMng_.GetValue( 1, nCase ), return false );
            _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            champInfo_.m_kPropertyCount.AddCase( nCase, fRatio );
        }
        else
            break;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // Property 읽기..
    _JIF( kLuaMng_.BeginTable( "Property" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KLottery kLttery;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            // 데이터는 여기에...^^;;;
            int nCase = 0;
            float fRatio = 0.f;
            _JIF( 0 == kLuaMng_.GetValue( 1, nCase ), return false );
            _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            kLttery.AddCase( nCase, fRatio );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // 벡터에 넣기..
        champInfo_.m_vecProperty.push_back( kLttery );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::LoadGPDropInfo( OUT std::string& strScript_ )
{
    float fSilverCoinRatio = 0.f;
    float fGoldCoinRatio = 0.f;
    float fJackPotRatio = 0.f;
    int nJacpotCoinCount = 0;
    KLottery SilverCoinCount;
    KLottery GoldCoinCount;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitGPDropInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( 0 == kLuaMng.GetValue( "SilverCoinRatio", fSilverCoinRatio ), return false );
    _JIF( 0 == kLuaMng.GetValue( "GoldCoinRatio", fGoldCoinRatio ), return false );
    _JIF( 0 == kLuaMng.GetValue( "JackPotRatio", fJackPotRatio ), return false );
    _JIF( 0 == kLuaMng.GetValue( "JackPotCoinCount", nJacpotCoinCount ), return false );
    _JIF( LoadLottery( kLuaMng, std::string("NormalSilver"), SilverCoinCount ), return false );
    _JIF( LoadLottery( kLuaMng, std::string("NormalGold"), GoldCoinCount ), return false );

    {
        KLocker lock( m_csGPDrop );
        m_fSilverCoinRatio = fSilverCoinRatio;
        m_fGoldCoinRatio = fGoldCoinRatio;
        m_fJackPotRatio = fJackPotRatio;
        m_nJacpotCoinCount = nJacpotCoinCount;
        m_SilverCoinCount = SilverCoinCount;
        m_GoldCoinCount = GoldCoinCount;
    }
    START_LOG( cerr, L"GP 드랍 설정 로드 완료.." ) << END_LOG;
    return true;
}

bool KGameDrop::LoadLottery( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT KLottery& kLottery_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return true );

    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nCase = 0;
        float fRatio = 0.f;
        _JIF( 0 == kLuaMng_.GetValue( 1, nCase ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
        fRatio *= 100.f;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        kLottery_.AddCase( nCase, fRatio );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KGameDrop::GetChampions( IN const KGameInfo& kInfo_, OUT std::vector<KChampionInfo>& vecChampions_ )
{
    vecChampions_.clear();

    UChampKey uChampKey;
    uChampKey.m_uaKeyList[0] = kInfo_.m_iGameMode;
    uChampKey.m_uaKeyList[1] = kInfo_.m_nDifficulty;
    uChampKey.m_uaKeyList[2] = uChampKey.m_uaKeyList[3] = 0;

    std::pair<int,int> prKey( kInfo_.m_iGameMode, kInfo_.m_nDifficulty);
    std::map<int, std::vector<sTriggerInfo> > mapTriggers;
    sChampionInfo champInfo;
    {
        KLocker lock( m_csChampionTable );
        std::map<__int64, sChampionInfo>::iterator mit;
        mit = m_mapChampionTable.find( uChampKey.m_biChampKey );
        if( mit == m_mapChampionTable.end() )
            return;
        champInfo = mit->second;
    }

    {
        KLocker lock( m_csDungeonTrigger );
        std::map< std::pair<int,int>, std::map<int, std::vector<sTriggerInfo> > >::iterator mit;
        mit = m_mapDungeonTrigger.find( prKey );
        if( mit == m_mapDungeonTrigger.end() )
            return;
        mapTriggers = mit->second;
    }

    std::map<int, std::vector<sTriggerInfo> >::iterator mit;
    for( mit = mapTriggers.begin() ; mit != mapTriggers.end() ; ++mit )
    {
        std::random_shuffle( mit->second.begin(), mit->second.end() );
        int nChampCount = std::min<int>(champInfo.GetChampionCount(), (int)mit->second.size() ); // 한 스테이지에 나올 챔피언 마리수
        if( nChampCount <= 0 )
            continue;

        KChampionInfo kChampInfo;
        std::vector<sTriggerInfo>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            if( vit->m_nIsBoss > 0 )
            {
                UChampKey uChampKey;
                uChampKey.m_uaKeyList[0] = kInfo_.m_iGameMode;
                uChampKey.m_uaKeyList[1] = kInfo_.m_nDifficulty;
                uChampKey.m_uaKeyList[2] = vit->m_nIsBoss;
                uChampKey.m_uaKeyList[3] = 0;

                std::map<__int64, sChampionInfo>::iterator mitChamp;
                mitChamp = m_mapChampionTable.find( uChampKey.m_biChampKey );
                if( mitChamp == m_mapChampionTable.end() ) continue;

                kChampInfo.m_dwProperty = mitChamp->second.GetProperty();
                kChampInfo.m_nTriggerID = vit->m_nTriggerID;
                kChampInfo.m_nMonsterID = vit->m_nMonsterID;
                vecChampions_.push_back( kChampInfo );
            }
            else
            {
                // 지정된 챔피언 마리수만큼 나오면 더이상
                // 챔피언을 만들지 않지만.. 보스에게 설정된 챔피언 이 있을지 모르니 계속 루프 돈다.
                if( nChampCount <= 0 ) continue;

                kChampInfo.m_dwProperty = champInfo.GetProperty();
                kChampInfo.m_nTriggerID = vit->m_nTriggerID;
                kChampInfo.m_nMonsterID = vit->m_nMonsterID;
                vecChampions_.push_back( kChampInfo );
                --nChampCount;
            }
        }
    }
}

bool KGameDrop::GetTriggerInfo( IN int nGameMode_, IN int nDifficult_, OUT std::vector<sTriggerInfo>& vecTriggerInfo_ )
{
    vecTriggerInfo_.clear();
    // 트리거, 몬스터 쌍의 리스트를 얻어 온다.
    std::pair<int,int> prKey( nGameMode_, nDifficult_ );
    std::map< std::pair<int,int>, std::map<int,std::vector<sTriggerInfo> > >::const_iterator mit;

    KLocker lock( m_csDungeonTrigger );
    mit = m_mapDungeonTrigger.find( prKey );
    if( mit == m_mapDungeonTrigger.end() )
    {
        START_LOG( clog, L"트리거 목록 얻기 실패. GameMode : " << prKey.first << L", Difficult : " << prKey.second ) << END_LOG;
        return false;
    }
    _JIF( !mit->second.empty(), return false );

    std::map<int,std::vector<sTriggerInfo> >::const_iterator mit2;
    for( mit2 = mit->second.begin() ; mit2 != mit->second.end() ; ++mit2 )
    {
        vecTriggerInfo_.insert( vecTriggerInfo_.end(), mit2->second.begin(), mit2->second.end() );
    }
    return true;
}

bool KGameDrop::GetDropInfo( IN const KGameInfo& kInfo_, OUT std::vector< KGameDropElement >& vecMonsterDrop_, OUT std::vector< KGameDropElement >& vecGpDrop_,
                             OUT std::vector<KChampionInfo>& vecChampions_, IN const float fScale_ )
{
    vecMonsterDrop_.clear();
    vecChampions_.clear();
    vecGpDrop_.clear();

    // 트리거 정보를 가져 온다..
    std::vector<sTriggerInfo> vecTriggerList;
    if( !GetTriggerInfo( kInfo_.m_iGameMode, kInfo_.m_nDifficulty, vecTriggerList ) )
        return false;

    //챔피언 정보를 만들어 낸다.
    GetChampions( kInfo_, vecChampions_ );

//    float fScale = GetScaleInfo( kInfo_ ); // 파티원 수에 따라 드랍 확률 조정.
    float fScale = 0.f;
    fScale += fScale_;

    // 게임 드랍 비율 설정
    DWORD dwDropType = KGameDrop::DT_TOTAL;
    if ( false == MonsterDropResult( kInfo_.m_iGameMode, kInfo_.m_nDifficulty, dwDropType ) ) {
        START_LOG( clog, L"게임 드랍 비율 설정 되어 있지 않아서 Total GameDrop으로 설정.") << END_LOG;
    }

    sGPDropInfo kInfo;
    MakeGPDropInfo( kInfo );

    // 트리거를 이터레이션 하면서 드랍 정보를 가져 온다.
    std::vector<sTriggerInfo>::const_iterator vit;
    for( vit = vecTriggerList.begin() ; vit != vecTriggerList.end() ; ++vit ) {
        KLocker lock( m_csMonsterInfo );
        std::map< int, sMonsterInfo >::iterator mitTotal;
        std::map< int, sMonsterInfo >::iterator mitChar;

        if ( KGameDrop::DT_CHAR == dwDropType ) {
            // 플레이하는 캐릭터 드랍일 경우
            mitChar = m_mapMonsterCharInfo.find( vit->m_nMonsterID );

            if( mitChar == m_mapMonsterCharInfo.end() ) continue;

            float fMobGrade = SiKGCHelper()->GetMobDropRatioByGrade( vit->m_nIsBoss );
            mitChar->second.AppendNormalDrop( vit->m_nTriggerID, kInfo_.m_iGameMode, kInfo_.m_nDifficulty, vecMonsterDrop_, fScale + fMobGrade, -1, dwDropType, kInfo_.m_mapCharLevelInfo );

            float fGP = GetMonGp( mitChar->second.m_nMonID, vit->m_nMonLv ); // 몬스터 GP 보상 체크.
            int nGP = static_cast<int>( fGP );
            mitChar->second.AppendGpDrop( vit->m_nTriggerID, vit->m_nMonLv, nGP, kInfo, vecGpDrop_ );

            if( std::find_if( vecChampions_.begin(), vecChampions_.end(),
                boost::bind( &KChampionInfo::m_nTriggerID, _1 ) == vit->m_nTriggerID ) != vecChampions_.end() )
            {
                mitChar->second.AppendChampionDrop( vit->m_nTriggerID, kInfo_.m_iGameMode, kInfo_.m_nDifficulty, vecMonsterDrop_, fScale + fMobGrade, dwDropType, kInfo_.m_mapCharLevelInfo );
            }
        }
        else {
            // 전체 캐릭터 드랍일 경우( KGameDrop::DT_TOTAL )
            mitTotal = m_mapMonsterInfo.find( vit->m_nMonsterID );

            if( mitTotal == m_mapMonsterInfo.end() ) continue;

            float fMobGrade = SiKGCHelper()->GetMobDropRatioByGrade( vit->m_nIsBoss );
            mitTotal->second.AppendNormalDrop( vit->m_nTriggerID, kInfo_.m_iGameMode, kInfo_.m_nDifficulty, vecMonsterDrop_, fScale + fMobGrade, -1, dwDropType, kInfo_.m_mapCharLevelInfo );

            float fGP = GetMonGp( mitTotal->second.m_nMonID, vit->m_nMonLv ); // 몬스터 GP 보상 체크.
            int nGP = static_cast<int>( fGP );
            mitTotal->second.AppendGpDrop( vit->m_nTriggerID, vit->m_nMonLv, nGP, kInfo, vecGpDrop_ );

            if( std::find_if( vecChampions_.begin(), vecChampions_.end(),
                boost::bind( &KChampionInfo::m_nTriggerID, _1 ) == vit->m_nTriggerID ) != vecChampions_.end() )
            {
                mitTotal->second.AppendChampionDrop( vit->m_nTriggerID, kInfo_.m_iGameMode, kInfo_.m_nDifficulty, vecMonsterDrop_, fScale + fMobGrade, dwDropType, kInfo_.m_mapCharLevelInfo );
            }
        }
    } // vit

    DWORD dwSize = kInfo_.m_vecUserUIDs.size();

    if( dwSize > 0 ) {
        std::vector< KGameDropElement >::iterator vitDrop;
        for ( vitDrop = vecMonsterDrop_.begin() ; vitDrop != vecMonsterDrop_.end() ; ++vitDrop ) {
            if( vitDrop->m_dwUserUID == 0 ) { // UserID가 세팅되지 않은 유저에 한해서 적용.
                vitDrop->m_dwUserUID = kInfo_.m_vecUserUIDs[GetRandomNum() % dwSize];
            }
        }
    }

    // 게임 시작시(StartGame) 유저별 잔여 활력 수치에 따라 드랍 리스트를 조정한다
    SiKVitalityManager()->ApplyNoVitalityRatioToItemDropList( kInfo_, vecMonsterDrop_ );

    return true;
}

void KGameDrop::SetNumbering( IN OUT KDropData& dropData_ )
{
    DWORD dwCount = dropData_.m_vecMissionDrop.size() + dropData_.m_vecMonsterDrop.size() + dropData_.m_vecGpDrop.size();

    std::set<DWORD> setRandNum;
    while( setRandNum.size() <= dwCount )
    {
        setRandNum.insert( GetRandomNum() );
    }
    std::set<DWORD>::iterator sit = setRandNum.begin();

    std::vector< KGameDropElement >::iterator vit;
    for( vit = dropData_.m_vecMonsterDrop.begin() ; vit != dropData_.m_vecMonsterDrop.end() ; ++vit )
    {
        JIF( sit != setRandNum.end() );
        vit->m_dwUID = *sit;
        ++sit;
    }

    for( vit = dropData_.m_vecMissionDrop.begin() ; vit != dropData_.m_vecMissionDrop.end() ; ++vit )
    {
        JIF( sit != setRandNum.end() );
        vit->m_dwUID = *sit;
        ++sit;
    }

    for( vit = dropData_.m_vecGpDrop.begin() ; vit != dropData_.m_vecGpDrop.end() ; ++vit )
    {
        JIF( sit != setRandNum.end() );
        vit->m_dwUID = *sit;
        ++sit;
    }
}

bool KGameDrop::PopDropData( IN OUT KDropData& kReservedDrop_, IN DWORD dwDropUID_, OUT KGameDropElement& dropEl_, IN DWORD dwUserUID_ )
{
    std::vector< KGameDropElement >::iterator vit;

    // 일반 드랍 리스트 에서 찾고.
    vit = std::find_if( kReservedDrop_.m_vecMonsterDrop.begin(), kReservedDrop_.m_vecMonsterDrop.end(),
        boost::bind( &KGameDropElement::m_dwUID, _1 ) == dwDropUID_ );

    if( vit != kReservedDrop_.m_vecMonsterDrop.end() )
    {
        dropEl_ = *vit;
        kReservedDrop_.m_vecMonsterDrop.erase( vit );
        return true;
    }

    // 없다면 미션 드랍 리스트에서 찾고
    vit = std::find_if( kReservedDrop_.m_vecMissionDrop.begin(), kReservedDrop_.m_vecMissionDrop.end(),
        boost::bind( &KGameDropElement::m_dwUID, _1 ) == dwDropUID_ );

    // 미션인 경우 받는 유저가 정해져 있음.
    if( vit != kReservedDrop_.m_vecMissionDrop.end() )
    {
        if( dwUserUID_ != vit->m_dwUserUID ) // 로그 때문에.
        {
            START_LOG( cerr, L"미션 보상중 받아야 될 유저와 받는 유저가 다름 Item ID : " << vit->m_dwItemID
                << L" : {" << vit->m_dwUserUID << L"," << dwUserUID_ << L"}" ) << END_LOG;
            return false;
        }

        dropEl_ = *vit;
        kReservedDrop_.m_vecMissionDrop.erase( vit );
        return true;
    }
    //없다면 실패..
    return false;
}

bool KGameDrop::DistributeDrops( IN OUT KDropData& kReservedDrop_, IN OUT KEndGameReq& kReq_,
                                 OUT std::map<DWORD, DROP_ITEMS>& mapEachUserDrops_ )
{
    std::map<DWORD,DWORD>::iterator mit;
    for( mit = kReq_.m_mapDropResult.begin() ; mit != kReq_.m_mapDropResult.end() ; ++mit )
    {
        KGameDropElement dropEl;
        if( !PopDropData( kReservedDrop_, mit->first, dropEl, mit->second ) )
            continue;

        switch( dropEl.m_cRewardType )
        {
        case KGameDropElement::RT_EXP:
        case KGameDropElement::RT_GP:
            break;
        case KGameDropElement::RT_ITEM:
        case KGameDropElement::RT_CHARDROP_ITEM:
        case KGameDropElement::RT_MISSION_ITEM:
            {
                KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( mit->second );
                if( !spUser )
                    continue;
                // 유저의 최대 인벤토리 사이즈보다 현재 보유한 아이템 갯수가 작을때만 아이템 지급.
                if( spUser->GetTrueInvenCapacity() <= spUser->m_kInventory.GetTotalEnableSize() && dropEl.m_cRewardType != KGameDropElement::RT_MISSION_ITEM ) {
                    START_LOG( clog, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급 실패." )
                        << BUILD_LOG( spUser->GetUID() )
                        << BUILD_LOG( spUser->GetName() )
                        << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                        << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;
                    continue;
                }

                std::vector<KGameResultIn>::iterator vitresult;
                vitresult = std::find_if( kReq_.m_vecGameResult.begin(), kReq_.m_vecGameResult.end(),
                    boost::bind( &KGameResultIn::m_strLogin, _1 ) == spUser->GetName() );
                if( vitresult == kReq_.m_vecGameResult.end() )
                    continue;
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID = dropEl.m_dwItemID;
                kItemUnit.m_nDuration = dropEl.m_nCount;
                kItemUnit.m_nPeriod = dropEl.m_nPeriod;
                kItemUnit.m_cCharType = spUser->GetCurrentCharType();
                //kItemUnit.??? = vit2->m_nStrongLV;
                mapEachUserDrops_[spUser->GetUID()].push_back( kItemUnit );
            }
            break;
        default:
            START_LOG( cerr, L"알수 없는 드랍 타입." )
                << BUILD_LOGc( dropEl.m_cRewardType ) << END_LOG;
        }
    }

    return true;
}

bool KGameDrop::DistributeRareDrops( IN OUT KDropData& kReservedDrop_, 
                                    IN OUT KEndGameReq& kReq_,
                                    OUT std::map<DWORD, DROP_ITEMS>& mapEachUserDrops_ )
{
    if( kReq_.m_mapRouletteList.empty() )
        return true;

    //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    //{
    //    std::map< DWORD, std::vector<DWORD> >::iterator mit;
    //    std::Info << L"룰렛 리스트 : ";
    //    for( mit = kReq_.m_mapRouletteList.begin() ; mit != kReq_.m_mapRouletteList.end() ; ++mit )
    //    {
    //        std::Info << L"(" << mit->first << L"),";
    //    }
    //    std::Info << dbg::endl;
    //}

    std::map< DWORD, std::vector<DWORD> >::iterator mit;
    for( mit = kReq_.m_mapRouletteList.begin() ; mit != kReq_.m_mapRouletteList.end() ; ++mit )
    {
        if( mit->second.empty() ) //받을 유저가 없다면 스킵
            continue;

        std::vector< KGameDropElement >::iterator vit;
        vit = std::find_if( kReservedDrop_.m_vecMonsterDrop.begin(), kReservedDrop_.m_vecMonsterDrop.end(),
            boost::bind(  &KGameDropElement::m_dwUID, _1 ) == mit->first );

        if( vit == kReservedDrop_.m_vecMonsterDrop.end() )
            continue;

        std::random_shuffle( mit->second.begin(), mit->second.end() ); // 유저들을 는다.

        std::vector<DWORD>::iterator vit2;
        for( vit2 = mit->second.begin() ; vit2 != mit->second.end() ; ++vit2 ) // 혹시라도라는 생각에 인 유저들을 이터레이션 한다.
        {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit2 );
            if( !spUser ) // 섞인 유저의 포인터 유효성
                continue;

            // 유저의 최대 인벤토리 사이즈보다 현재 보유한 아이템 갯수가 작을때만 아이템 지급.
            if( spUser->GetTrueInvenCapacity() <= spUser->m_kInventory.GetTotalEnableSize() ) {
                START_LOG( clog, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급 실패." )
                    << BUILD_LOG( spUser->GetUID() )
                    << BUILD_LOG( spUser->GetName() )
                    << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                    << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;
                continue;
            }

            std::vector<KGameResultIn>::iterator vitresult;
            vitresult = std::find_if( kReq_.m_vecGameResult.begin(), kReq_.m_vecGameResult.end(),
                boost::bind( &KGameResultIn::m_strLogin, _1 ) == spUser->GetName() );
            if( vitresult == kReq_.m_vecGameResult.end() ) // 혹시 결과 리스트에 없다면... 다음녀석을 선택한다.
                continue;

            KDropItemInfo kItemUnit;
            kItemUnit.m_ItemID = vit->m_dwItemID;
            kItemUnit.m_nDuration = vit->m_nCount;
            kItemUnit.m_nPeriod = vit->m_nPeriod;
            kItemUnit.m_cCharType = spUser->GetCurrentCharType();
            //kItemUnit.??? = vit2->m_nStrongLV;
            mapEachUserDrops_[spUser->GetUID()].push_back( kItemUnit );

            if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
            {
                START_LOG( clog, L"룰렛.. : " << spUser->GetName() << L"(" << kItemUnit.m_ItemID << L":" << mit->first << L")"  ) << END_LOG;
            }
            break;
        }

        kReservedDrop_.m_vecMonsterDrop.erase( vit );
    }
    return false;
}

bool KGameDrop::AdjustDrops( IN const std::map<DWORD, DROP_ITEMS>& mapInDrops_,
                             OUT std::map<DWORD, DROP_ITEMS>& mapOutDrops_ )
{
    std::map<DWORD, DROP_ITEMS>::const_iterator mit;
    for ( mit = mapInDrops_.begin() ; mit != mapInDrops_.end() ; ++mit ) {
        mapOutDrops_[mit->first].insert( mapOutDrops_[mit->first].end(), mit->second.begin(), mit->second.end() );
    }

    return true;
}

float KGameDrop::GetRatio()
{
    return GetRandomNum()/(float)(UINT_MAX);
}

float KGameDrop::GetMonExp( int nMonID_, int nLv_ )
{
    KLocker lock(m_csMonReward);

    std::map< int, sLevelReward >::iterator mitReward;
    std::map< int, sMonAddRatio >::iterator mitRatio;
    mitReward = m_mapMonLevelReward.find( nLv_ );
    __JIF( mitReward != m_mapMonLevelReward.end(), return 0 );
    mitRatio = m_mapMonAddRatio.find( nMonID_ );
    __JIF( mitRatio != m_mapMonAddRatio.end(), return 0 );
    return ( mitReward->second.m_nExp * mitRatio->second.m_fExpRatio );
}

float KGameDrop::GetMonGp( int nMonID_, int nLv_ )
{
    KLocker lock(m_csMonReward);

    std::map< int, sLevelReward >::iterator mitReward;
    std::map< int, sMonAddRatio >::iterator mitRatio;
    mitReward = m_mapMonLevelReward.find( nLv_ );
    __JIF( mitReward != m_mapMonLevelReward.end(), return 0 );
    mitRatio = m_mapMonAddRatio.find( nMonID_ );
    __JIF( mitRatio != m_mapMonAddRatio.end(), return 0 );
    return ( mitReward->second.m_nGP * mitRatio->second.m_fGPRatio );
}

float KGameDrop::GetMonSpx( int nMonID_, int nLv_ )
{
    KLocker lock(m_csMonReward);

    std::map< int, sLevelReward >::iterator mitReward;
    std::map< int, sMonAddRatio >::iterator mitRatio;
    mitReward = m_mapMonLevelReward.find( nLv_ );
    __JIF( mitReward != m_mapMonLevelReward.end(), return 0 );
    mitRatio = m_mapMonAddRatio.find( nMonID_ );
    __JIF( mitRatio != m_mapMonAddRatio.end(), return 0 );
    return ( mitReward->second.m_nSP * mitRatio->second.m_fSPRatio );
}

float KGameDrop::GetScaleInfo( IN const KGameInfo& kInfo_ )
{
    switch( kInfo_.m_nStartUserNum )
    {
    case 2: return 0.07f;
    case 3: return 0.14f;
    case 4: return 0.21f;
    }

    return 0.f;
}

void KGameDrop::ApplyBoostRatio( IN OUT float& fRatio_ )
{
    KLocker lock(m_csMultiplyRatio);
    fRatio_ += fRatio_ * m_fMultiplyRatio;
}

void KGameDrop::MakeGPDropInfo( OUT sGPDropInfo& kInfo_ )
{
    KLocker lock(m_csGPDrop);
    kInfo_.m_dwSilverID = KGameDropElement::GT_SILVER;
    kInfo_.m_fSilverRatio = m_fSilverCoinRatio;
    kInfo_.m_dwGoldID = KGameDropElement::GT_GOLD;
    kInfo_.m_fGoldRatio = m_fGoldCoinRatio;

    if ( GetRatio() <= m_fJackPotRatio ) {
        kInfo_.m_dwSilverID = KGameDropElement::GT_JP_SILVER;
        kInfo_.m_nSilverCount = m_nJacpotCoinCount;
    }
    else {
        kInfo_.m_nSilverCount = m_SilverCoinCount.Decision();
    }
    kInfo_.m_nGoldCount = m_GoldCoinCount.Decision();
}

bool KGameDrop::LoadMonsterRewardScript( OUT std::string& strScript_ )
{
    std::map< int, sLevelReward > mapMonLevelReward;
    std::map< int, sMonAddRatio > mapMonAddRatio;

    KLuaManager kLuaMng;
    const std::string strScriptName = "MonsterRewardInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 보상 정보 읽어오기.
    _JIF( LoadLevelRewardInfo( kLuaMng, std::string("MonLevelReward"), mapMonLevelReward ), return false );
    _JIF( LoadRewardRatioInfo( kLuaMng, std::string("MonAddRatio"), mapMonAddRatio ), return false );

    START_LOG( cerr, L"몬스터 보상 정보( Lv, Ratio ) 로드 완료. " ) << END_LOG;

    KLocker lock( m_csMonReward );

    m_mapMonLevelReward.swap( mapMonLevelReward );
    m_mapMonAddRatio.swap( mapMonAddRatio );

    return true;
}

bool KGameDrop::LoadLevelRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, sLevelReward >& mapMonLevelReward_ )
{
    mapMonLevelReward_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        int nLevel;
        sLevelReward sLvReward;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Level", nLevel ) == S_OK, return false );
        _JIF( LoadLevelReward( kLuaMng_, std::string("Reward"), sLvReward ), return false );

        if( !mapMonLevelReward_.insert( std::make_pair( nLevel, sLvReward ) ).second ) {
            START_LOG( cerr, L" 중복된 Level 정보가 있음. Level : " << nLevel ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::LoadRewardRatioInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, sMonAddRatio >& mapMonAddRatio_ )
{
    mapMonAddRatio_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        int nMonID;
        sMonAddRatio sMonRatio;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "ID", nMonID ) == S_OK, return false );
        _JIF( LoadRewardRatio( kLuaMng_, std::string("Ratio"), sMonRatio ), return false );

        if( !mapMonAddRatio_.insert( std::make_pair( nMonID, sMonRatio ) ).second ) {
            START_LOG( cerr, L" 중복된 몬스터 정보가 있음. MonID : " << nMonID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::LoadLevelReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT sLevelReward& sLvReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    sLevelReward sData;
    if( kLuaMng_.GetValue( 1, sData.m_nExp) != S_OK ) return false;
    _JIF( kLuaMng_.GetValue( 2, sData.m_nSP ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, sData.m_nGP ) == S_OK, return false );

    if ( sData.m_nExp < 0 || sData.m_nGP < 0 ) {
        START_LOG( cerr, L"Exp나 GP 설정값에 음수가 있음." )
            << BUILD_LOG( sData.m_nExp  )
            << BUILD_LOG( sData.m_nGP ) << END_LOG;
        return false;
    }

    sLvReward_ = sData;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::LoadRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT sMonAddRatio& sMonRatio_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    
    sMonAddRatio sData;
    if( kLuaMng_.GetValue( 1, sData.m_fExpRatio) != S_OK ) return false;
    _JIF( kLuaMng_.GetValue( 2, sData.m_fSPRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, sData.m_fGPRatio ) == S_OK, return false );
    sMonRatio_ = sData;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

int KGameDrop::GetCharLvMonExp( int nLv_ )
{
    KLocker lock(m_csMonReward);

    std::map< int, sLevelReward >::iterator mitReward;
    mitReward = m_mapMonLevelReward.find( nLv_ );
    __JIF( mitReward != m_mapMonLevelReward.end(), return 0 );
    return mitReward->second.m_nExp;
}

int KGameDrop::GetCharLvMonGP( int nLv_ )
{
    KLocker lock(m_csMonReward);

    std::map< int, sLevelReward >::iterator mitReward;
    mitReward = m_mapMonLevelReward.find( nLv_ );
    __JIF( mitReward != m_mapMonLevelReward.end(), return 0 );
    return mitReward->second.m_nGP;
}

bool KGameDrop::DistributeDrop( IN OUT KDropData& kDropInfo_, IN const std::vector<DWORD>& vecUserUIDs_, IN const DWORD& dwUserUID_, IN const DWORD& dwDropUID_, OUT std::pair<DWORD,KDropItemInfo>& prUserDrops_,
                               IN const std::map<DWORD, int>& mapFatigueUserList_ )
{
    std::vector<DWORD>::const_iterator cvit;
    cvit = std::find( vecUserUIDs_.begin(), vecUserUIDs_.end(), dwUserUID_ );
    if ( cvit == vecUserUIDs_.end() ) {
        START_LOG( cwarn, L"룸 유저리스트에 일치하는 유저가 없음. UserUID : " << dwUserUID_ << L", UserList size : " << vecUserUIDs_.size() ) << END_LOG;
        return false;
    }

    KGameDropElement dropEl;
    if ( false == PopDropData( kDropInfo_, dwDropUID_, dropEl, dwUserUID_ ) ) {
        START_LOG( cwarn, L"서버 드랍정보와 요청한 드랍이 일치하지 않는다. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    std::map<DWORD, int>::const_iterator mitFatigue;
    mitFatigue = mapFatigueUserList_.find( dwUserUID_ );
    if ( mitFatigue != mapFatigueUserList_.end() ) {
        START_LOG( cwarn, L"중국 몰입방지 해당 유저라서 아이템 드랍하지 않음" << dwUserUID_ ) << END_LOG;
        return false;
    }

    switch( dropEl.m_cRewardType )
    {
    case KGameDropElement::RT_EXP:
    case KGameDropElement::RT_GP:
        break;
    case KGameDropElement::RT_ITEM:
    case KGameDropElement::RT_CHARDROP_ITEM:
    case KGameDropElement::RT_MISSION_ITEM:
        {
            // 게임서버에 지급대상 유저가 접속중인지.
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( dwUserUID_ );
            if ( !spUser ) {
                return false;
            }

            if ( false == spUser->CheckItemInvenLimit( dropEl.m_dwItemID ) && dropEl.m_cRewardType != KGameDropElement::RT_MISSION_ITEM ) {
                START_LOG( cwarn, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급 실패.. ItemID : " << dropEl.m_dwItemID )
                    << BUILD_LOG( spUser->GetUID() )
                    << BUILD_LOG( spUser->GetName() )
                    << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                    << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;
                return false;
            }

            KDropItemInfo kItemUnit;
            kItemUnit.m_ItemID = dropEl.m_dwItemID;
            kItemUnit.m_nDuration = dropEl.m_nCount;
            kItemUnit.m_nPeriod = dropEl.m_nPeriod;
            kItemUnit.m_cCharType = spUser->GetCurrentCharType();

            prUserDrops_ = std::make_pair( dwUserUID_, kItemUnit );
        }
        break;
    default:
        START_LOG( cerr, L"알 수 없는 드랍 타입. DropType : " << static_cast< int >( dropEl.m_cRewardType ) ) << END_LOG;
        return false;
    }

    return true;
}

bool KGameDrop::DistributeRareDrop( IN OUT KDropData& kDropInfo_, IN const std::vector<DWORD>& vecUserUIDs_, IN OUT std::vector<DWORD>& vecRouletUserUIDs_,
                                   IN const DWORD& dwDropUID_, OUT std::pair<DWORD,KDropItemInfo>& prUserDrops_, IN std::vector<KDropItemInfo> vecPartyDrop_,
                                   OUT std::vector< std::pair<DWORD,KDropItemInfo> >& vecPartyDrops_, IN const std::map<DWORD, int>& mapFatigueUserList_ )
{
    bool bGiveAllPartyPlayer = false;

    if ( true == vecRouletUserUIDs_.empty() ) {
        START_LOG( clog, L"받을 유저가 없다. DropUID : " << dwDropUID_ ) << END_LOG;
        return false;
    }

    std::vector< KGameDropElement >::iterator vitDrop;
    vitDrop = std::find_if( kDropInfo_.m_vecMonsterDrop.begin(), kDropInfo_.m_vecMonsterDrop.end(),
        boost::bind( &KGameDropElement::m_dwUID, _1 ) == dwDropUID_ );

    if ( kDropInfo_.m_vecMonsterDrop.end() == vitDrop ) {
        START_LOG( cwarn, L"해당하는 드랍 정보가 없음. DropUID : " << dwDropUID_ ) << END_LOG;
        return false;
    }

    /* 이렇게 분기로 처리하지 말고 좀 더 좋은 구조로 고치면 좋을 것 같다
       차원의 문 드랍 처리인 DecideWhoGetDropItem() 함수의 구현처럼
       GetAffordableInvenUsers() 함수를 활용하는 방법으로 생각해보면 좋을 것 같다 */
    if ( KGameDropElement::RT_CHARDROP_ITEM == vitDrop->m_cRewardType && 0 != vitDrop->m_dwUserUID ) {
        // 획득할 유저가 정해져 있는 아이템인 경우 처리

        // 유저의 최대 인벤토리 사이즈보다 현재 보유한 아이템 갯수가 작을때만 아이템 지급.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( vitDrop->m_dwUserUID );
        if ( NULL != spUser ) {
            if ( false == spUser->CheckItemInvenLimit( vitDrop->m_dwItemID ) ) {
                START_LOG( cwarn, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급 실패.. ItemID : " << vitDrop->m_dwItemID )
                    << BUILD_LOG( spUser->GetUID() )
                    << BUILD_LOG( spUser->GetName() )
                    << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                    << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;
            }
            else {
                std::vector< DWORD >::const_iterator cvitRoom;
                cvitRoom = std::find( vecUserUIDs_.begin(), vecUserUIDs_.end(), spUser->GetUID() );
                if ( vecUserUIDs_.end() == cvitRoom ) {
                    START_LOG( cwarn, L"룸 유저리스트에 일치하는 유저가 없음. UserUID : " << spUser->GetUID() << L", UserList size : " << vecUserUIDs_.size() ) << END_LOG;
                }
                else {
                    KDropItemInfo kItemUnit;
                    kItemUnit.m_ItemID = vitDrop->m_dwItemID;
                    kItemUnit.m_nDuration = vitDrop->m_nCount;
                    kItemUnit.m_nPeriod = vitDrop->m_nPeriod;
                    kItemUnit.m_cCharType = spUser->GetCurrentCharType();
                    prUserDrops_ = std::make_pair( spUser->GetUID(), kItemUnit );
                }
            }
        }
    }
    else {
        // 획득할 유저가 정해져 있지 않은 아이템인 경우 처리

        // 파티 전체 획득 가능한 아이템인지 확인
        std::vector<KDropItemInfo>::iterator vitPartyDropInfo;
        vitPartyDropInfo = std::find_if( vecPartyDrop_.begin(), vecPartyDrop_.end(),
            boost::bind( &KDropItemInfo::m_ItemID, _1 ) == vitDrop->m_dwItemID );
        if ( vecPartyDrop_.end() != vitPartyDropInfo ) {
            bGiveAllPartyPlayer = true;
        }

        // 유저들을 섞는다.
        std::random_shuffle( vecRouletUserUIDs_.begin(), vecRouletUserUIDs_.end() );

        std::vector<DWORD>::const_iterator cvitUser;
        for ( cvitUser = vecRouletUserUIDs_.begin() ; cvitUser != vecRouletUserUIDs_.end() ; ++cvitUser ) {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *cvitUser );
            if ( !spUser ) continue;

            std::map<DWORD, int>::const_iterator mitFatigue;
            mitFatigue = mapFatigueUserList_.find( *cvitUser );
            if ( mitFatigue != mapFatigueUserList_.end() ) {
                START_LOG( cwarn, L"중국 몰입방지 해당 유저라서 아이템 드랍하지 않음 DistributeRareDrop, UserUID : " << *cvitUser ) << END_LOG;
                continue;
            }

            // 유저의 최대 인벤토리 사이즈보다 현재 보유한 아이템 갯수가 작을때만 아이템 지급.
            if ( false == spUser->CheckItemInvenLimit( vitDrop->m_dwItemID ) ) {
                START_LOG( cwarn, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급 실패.. ItemID : " << vitDrop->m_dwItemID )
                    << BUILD_LOG( spUser->GetUID() )
                    << BUILD_LOG( spUser->GetName() )
                    << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                    << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;
                continue;
            }

            std::vector<DWORD>::const_iterator cvitRoom;
            cvitRoom = std::find( vecUserUIDs_.begin(), vecUserUIDs_.end(), spUser->GetUID() );
            if ( cvitRoom == vecUserUIDs_.end() ) {
                START_LOG( cwarn, L"룸 유저리스트에 일치하는 유저가 없음. UserUID : " << spUser->GetUID() << L", UserList size : " << vecUserUIDs_.size() ) << END_LOG;
                continue;
            }

            KDropItemInfo kItemUnit;
            kItemUnit.m_ItemID = vitDrop->m_dwItemID;
            kItemUnit.m_nDuration = vitDrop->m_nCount;
            kItemUnit.m_nPeriod = vitDrop->m_nPeriod;
            kItemUnit.m_cCharType = spUser->GetCurrentCharType();
            prUserDrops_ = std::make_pair( spUser->GetUID(), kItemUnit );

            if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
                START_LOG( clog, L"룰렛 결과. LoginID : " << spUser->GetName() << L", ItemID : " << kItemUnit.m_ItemID << L", DropUID : " << dwDropUID_  ) << END_LOG;
            }

            if ( true == bGiveAllPartyPlayer ) {
                vecPartyDrops_.push_back( prUserDrops_ );
            }
            else {
                break;
            }
        }
    }

    kDropInfo_.m_vecMonsterDrop.erase( vitDrop );

    return true;
}

bool KGameDrop::LoadCharMonsterScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< int, sMonsterInfo > mapMonsterCharInfo;

    std::string strScriptName = "CharGameDrop.lua";
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "Mon" ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng.BeginTable( i ) == S_OK ) {
            sMonsterInfo monInfo;
            _JIF( LoadMonsterCharInfo( kLuaMng, monInfo ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );           

            if( mapMonsterCharInfo.find( monInfo.m_nMonID ) != mapMonsterCharInfo.end() ) {
                START_LOG( cerr, L" 동일한 몬스터 정보가 존재합니다. Mon ID : " << monInfo.m_nMonID ) << END_LOG;
                return false;
            }
            mapMonsterCharInfo[monInfo.m_nMonID] = monInfo;
        }
        else
            break;
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"몬스터 캐릭터별 드랍 정보 로드 완료,... Size : " << mapMonsterCharInfo.size() ) << END_LOG;
    KLocker lock( m_csMonsterInfo );
    m_mapMonsterCharInfo.swap( mapMonsterCharInfo );
    return true;
}

bool KGameDrop::LoadMonsterCharInfo( IN OUT KLuaManager& kLuaMng_, OUT sMonsterInfo& monInfo_ )
{
    monInfo_.m_nMonID = -1;
    monInfo_.m_mapMonCharDrops.clear();

    _JIF( 0 == kLuaMng_.GetValue( "MonID", monInfo_.m_nMonID ) , return false );

    // Drop 정보 읽기..
    _JIF( kLuaMng_.BeginTable( "DropInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        sMonCharDrops monCharDrops;
        _JIF( 0 == kLuaMng_.GetValue( "DungeonID", monCharDrops.m_nDungeonID ), return false );
        _JIF( 0 == kLuaMng_.GetValue( "Difficult", monCharDrops.m_nDifficult ), return false );

        // Drop Table 읽기..
        if( kLuaMng_.BeginTable( "Drop" ) != S_OK ) break;
        for( int j = 1 ; ; ++j ) {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            sMondropElement monDropEle;
            _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monCharDrops.m_vecDefaultDrop.push_back( monDropEle );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // Champion Drop Table 읽기.
        if( kLuaMng_.BeginTable( "ChampionDrop" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            sMondropElement monDropEle;
            _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            monCharDrops.m_vecChampDrop.push_back( monDropEle );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // DropGroup 읽기
        if( kLuaMng_.BeginTable( "CharDropInfo" ) != S_OK ) break;
        for( int j = 1 ; ; ++j ) {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            int nCharType;
            _JIF( 0 == kLuaMng_.GetValue( "CharType", nCharType ) , return false );
            char cCharType = static_cast<char>( nCharType );

            // DropGroup 읽기
            {
                if( kLuaMng_.BeginTable( "Drop_Group" ) != S_OK ) break;
                KLottery kLottery;
                std::vector<sMondropElement> vecDropElement;

                for( int m = 1 ; ; ++m ) {
                    if( kLuaMng_.BeginTable( m ) != S_OK ) break;
                    sMondropElement monDropEle;
                    _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
                    _JIF( kLuaMng_.EndTable() == S_OK, return false );
                    vecDropElement.push_back( monDropEle );
                    kLottery.AddCase( (int)monDropEle.m_dwItemID, monDropEle.m_fRatio * 100.f );
                }
                monCharDrops.m_mapCharDefaultGroup[cCharType] = std::make_pair(kLottery, vecDropElement);
                _JIF( kLuaMng_.EndTable() == S_OK, return false ); // close Drop_Group Table
            }

            // ChampionDrop_Group 읽기
            {
                if( kLuaMng_.BeginTable( "ChampionDrop_Group" ) != S_OK ) break;
                KLottery kLottery;
                std::vector<sMondropElement> vecDropElement;

                for( int m = 1 ; ; ++m ) {
                    if( kLuaMng_.BeginTable( m ) != S_OK ) break;
                    sMondropElement monDropEle;
                    _JIF( LoadDropElement( kLuaMng_, monDropEle), return false );
                    _JIF( kLuaMng_.EndTable() == S_OK, return false );
                    vecDropElement.push_back( monDropEle );
                    kLottery.AddCase( (int)monDropEle.m_dwItemID, monDropEle.m_fRatio * 100.f );
                }
                monCharDrops.m_mapCharChampGroup[cCharType] = std::make_pair(kLottery, vecDropElement);
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // CharGameDrpo Table Close

         std::pair<int,int> prKey( monCharDrops.m_nDungeonID, monCharDrops.m_nDifficult );
         monInfo_.m_mapMonCharDrops.insert( std::make_pair( prKey, monCharDrops ) );
         _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::LoadMonsterDropRatio( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< std::pair<int,int>, Kairo > mapMonsterDropRatio;
    mapMonsterDropRatio.clear();

    std::string strScriptName = "GameDropRatio.lua";
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "DungeonDropRatio" ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng.BeginTable( i ) == S_OK ) {
            std::pair<int,int> prData;
            Kairo kKairo;

            _JIF( 0 == kLuaMng.GetValue( "DungeonID", prData.first ), return false );
            _JIF( 0 == kLuaMng.GetValue( "Difficult", prData.second ), return false );
            _JIF( LoadRatio( kLuaMng, std::string("DropRatio"), kKairo ), return false );

            if ( !mapMonsterDropRatio.insert( std::make_pair( prData, kKairo ) ).second ) {
                START_LOG( cerr, L"중복된 던전 몬스터 배율 정보 있음. MonID : " << prData.first << ", DungeonID : " << prData.second )
                    << END_LOG;
            }
            _JIF( kLuaMng.EndTable() == S_OK, return false );
        } else {
            break;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"몬스터 캐릭터별 드랍 정보 로드 완료,... Size : " << mapMonsterDropRatio.size() ) << END_LOG;

    KLocker lock( m_csMonsterInfo );
    m_mapMonsterDropRatio.swap( mapMonsterDropRatio );
    return true;
}

bool KGameDrop::LoadRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        DWORD dwType = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostType( dwType, fProb ), return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGameDrop::MonsterDropResult( IN const int& nDungeonID_, IN const int& nDifficult_, OUT DWORD& dwResult_ )
{
    std::pair<int,int> prData;
    prData.first = nDungeonID_;
    prData.second = nDifficult_;

    KLocker lock( m_csMonsterInfo );
    std::map< std::pair<int,int>, Kairo >::iterator mit;
    mit = m_mapMonsterDropRatio.find( prData );
    if( mit == m_mapMonsterDropRatio.end() ) {
        START_LOG( cerr, L"던전 몬스터 배율 정보 없음. MonID : " << prData.first << ", DungeonID : " << prData.second )
            << END_LOG;
        dwResult_ = KGameDrop::DT_TOTAL;
        return false;
    }

    Kairo kKairo = mit->second;
    bool bSuccess = kKairo.Do( dwResult_ );


    if( !bSuccess ) {
        dwResult_ = KGameDrop::DT_TOTAL;
        return false;
    }

    return true;
}

void KGameDrop::DeleteFatigueUserDistributeItem( IN std::map<DWORD, DROP_ITEMS>& mapEachUserDrops_, 
                                                IN std::map<DWORD, int>& mapFatigueUserList_ ) // 과몰입 유저의 아이템 획득 제거.
{

    std::map<DWORD, int>::const_iterator mit;
    for ( mit = mapFatigueUserList_.begin(); mit != mapFatigueUserList_.end(); ++mit ) {
        if( mapEachUserDrops_.find( mit->first ) != mapEachUserDrops_.end() ) {
            mapEachUserDrops_.erase( mit->first );
        }
    }
}

bool KGameDrop::MakeIDReservedDropInfoFromGameDrop( IN const KGameInfo& kGameInfo_,
                                                    IN const float fDropEventScale_,
                                                    IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_,
                                                    IN DWORD dwIDReservedDropLastUID_,
                                                    OUT std::list< KGameDropElement >& lstIDReservedDrops_,
                                                    OUT DWORD& dwIDReservedDropLastUIDAfter_ )
{
    // 게임 드랍 비율 설정
    DWORD dwDropType = KGameDrop::DT_TOTAL;
    if ( false == MonsterDropResult( kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, dwDropType ) ) {
        START_LOG( clog, L"게임 드랍 비율 설정 되어 있지 않아서 Total GameDrop으로 설정.") << END_LOG;
    }

    std::vector< KGameDropElement > vecMonsterDrop;
    vecMonsterDrop.clear();

    BOOST_FOREACH( KIDMonsterInfo& rIDSummonedMonster, lstIDSummonedMonsters_ ) {
        KLocker lock( m_csMonsterInfo );

        if ( KGameDrop::DT_CHAR == dwDropType ) {
            // 플레이하는 캐릭터 드랍일 경우
            std::map< int, sMonsterInfo >::iterator mitChar;
            mitChar = m_mapMonsterCharInfo.find( rIDSummonedMonster.m_nMonID );
            if ( m_mapMonsterCharInfo.end() == mitChar ) {
                continue;
            }

            mitChar->second.AppendNormalDrop( rIDSummonedMonster.m_nMonUID, kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, vecMonsterDrop, fDropEventScale_, -1, dwDropType, kGameInfo_.m_mapCharLevelInfo );
        }
        else {
            // 전체 캐릭터 드랍일 경우( KGameDrop::DT_TOTAL )
            std::map< int, sMonsterInfo >::iterator mitTotal;
            mitTotal = m_mapMonsterInfo.find( rIDSummonedMonster.m_nMonID );
            if ( m_mapMonsterInfo.end() == mitTotal ) {
                continue;
            }

            mitTotal->second.AppendNormalDrop( rIDSummonedMonster.m_nMonUID, kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, vecMonsterDrop, fDropEventScale_, -1, dwDropType, kGameInfo_.m_mapCharLevelInfo );
        }
    }

    BOOST_FOREACH( KGameDropElement& rGameDropElement, vecMonsterDrop ) {
        rGameDropElement.m_dwUID = dwIDReservedDropLastUID_++;
    }

    lstIDReservedDrops_.insert( lstIDReservedDrops_.end(), vecMonsterDrop.begin(), vecMonsterDrop.end() );
    dwIDReservedDropLastUIDAfter_ = dwIDReservedDropLastUID_;

    return true;
}
