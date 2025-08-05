#include "GPCapsuleManager.h"
#include "Lua/KLuaManager.h"
#include <KncUtilLua.h> // boost::bind
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGPCapsuleManager );
ImplOstmOperatorW2A( KGPCapsuleManager );
NiImplementRootRTTI( KGPCapsuleManager );

KGPCapsuleManager::KGPCapsuleManager(void)
:m_bGPCapsuleEnable(false)
{
}

KGPCapsuleManager::~KGPCapsuleManager(void)
{
}

ImplToStringW( KGPCapsuleManager )
{
    KLocker lock( m_csCapsule );

    return START_TOSTRINGW
        << TOSTRINGWb( m_bGPCapsuleEnable )
        << TOSTRINGW( m_mapCapsuleInfoList.size() )
        << TOSTRINGW( m_mapCapsuleListNotice.size() );
}

bool KGPCapsuleManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGPCapsuleManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    bool                        bEnable;
    CAPSULE_INFO_LIST           mapServerInfo; // 서버정보
    KEVENT_CAPSULE_LIST_NOT     mapClientInfo; // 클라이언트정보

    const std::string strScriptName = "InitGPCapsule.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "CapsuleInfo" ) == S_OK, return false );

    // GP캡슐 사용여부
    _JIF( kLuaMng.GetValue( "CapsuleEnable", bEnable ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;

        std::pair<int,int> prKey;   // 캐릭터타입,아이템종류
        KCapsuleInfo kServerInfo;   // 서버 캡슐 정보
        KCapsule kClientInfo;       // 클라이언트 캡슐 정보
        int nIndex = -1;

        prKey.first = -1;
        prKey.second = -1;
        kServerInfo.m_nIndex = -1;
        kServerInfo.m_nPriceType = -1;
        kServerInfo.m_nPrice = -1;
        kServerInfo.m_prLevelRange.first = -1;
        kServerInfo.m_prLevelRange.second = -1;

        // 아이템 식별키(캐릭터타입,아이템종류,캡슐아이템번호)
        _JIF( kLuaMng.GetValue( "CharType", prKey.first ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "ItemKind", prKey.second ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Index", kServerInfo.m_nIndex ) == S_OK, return false );
        // 가격
        _JIF( kLuaMng.GetValue( "PriceType", kServerInfo.m_nPriceType ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Price", kServerInfo.m_nPrice ) == S_OK, return false );
        // 아이템레벨대
        _JIF( kLuaMng.BeginTable( "LevelRange" ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 1, kServerInfo.m_prLevelRange.first ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, kServerInfo.m_prLevelRange.second ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        // 아이템등급별 확률
        _JIF( LoadGradeRatio( kLuaMng, kServerInfo.m_ltrGrade ), return false );

        // 기본 보상 아이템
        _JIF( kLuaMng.BeginTable( "BaseList" ) == S_OK, return false );
        _JIF( LoadCapsuleRatio( kLuaMng, KGPCapsuleManager::IG_BASE, kClientInfo.m_vecItemList, kServerInfo.m_kairoBase ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        // 레어 보상 아이템
        _JIF( kLuaMng.BeginTable( "RareList" ) == S_OK, return false );
        _JIF( LoadCapsuleRatio( kLuaMng, KGPCapsuleManager::IG_RARE, kClientInfo.m_vecItemList, kServerInfo.m_kairoRare ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        // 이벤트 보상 아이템
        _JIF( kLuaMng.BeginTable( "EventList" ) == S_OK, return false );
        _JIF( LoadCapsuleRatio( kLuaMng, KGPCapsuleManager::IG_EVENT, kClientInfo.m_vecItemList, kServerInfo.m_kairoEvent ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        _JIF( kLuaMng.EndTable() == S_OK, return false );

        // 클라이언트 정보 채우기
        kClientInfo.m_nIndex = kServerInfo.m_nIndex;
        kClientInfo.m_nPriceType = kServerInfo.m_nPriceType;
        kClientInfo.m_nPrice = kServerInfo.m_nPrice;
        kClientInfo.m_prLevelRange = kServerInfo.m_prLevelRange;

        // 컨테이너에 정보 싣기
        mapServerInfo[prKey].push_back( kServerInfo );
        mapClientInfo[prKey].push_back( kClientInfo );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    KLocker lock( m_csCapsule );
    m_bGPCapsuleEnable = bEnable;
    m_mapCapsuleInfoList.swap( mapServerInfo );
    m_mapCapsuleListNotice.swap( mapClientInfo );

    START_LOG( cerr, L"GP캡슐 스크립트 읽기 완료. size : " << m_mapCapsuleInfoList.size() << L", " << m_mapCapsuleListNotice.size() ) << END_LOG;
    return true;
}

bool KGPCapsuleManager::LoadGradeRatio( IN KLuaManager& kLuaMng_, OUT KLottery& ltr_ )
{
    float fBaseRatio = 0.f;
    float fRareRatio = 0.f;
    float fEventRatio = 0.f;
    float fSum = 0.f;

    _JIF( kLuaMng_.GetValue( "BaseRatio", fBaseRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "RareRatio", fRareRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "EventRatio", fEventRatio ) == S_OK, return false );

    fSum = fBaseRatio + fRareRatio + fEventRatio;
    if( fSum != 100.f )
    {
        START_LOG( cerr, L"GradeRatio 합이 100퍼센트가 아님. : " << fSum )
            << BUILD_LOG( fBaseRatio )
            << BUILD_LOG( fRareRatio )
            << BUILD_LOG( fEventRatio ) << END_LOG;
        return false;
    }

    // 확률 정보 입력
    LIF( ltr_.AddCase( KGPCapsuleManager::IG_BASE, fBaseRatio) );
    LIF( ltr_.AddCase( KGPCapsuleManager::IG_RARE, fRareRatio) );
    LIF( ltr_.AddCase( KGPCapsuleManager::IG_EVENT, fEventRatio) );

    return true;
}

bool KGPCapsuleManager::LoadCapsuleRatio( IN KLuaManager& kLuaMng_, IN const int nGrade_, OUT std::vector< std::pair<KDropItemInfo,int> >& vecItemList_, OUT Kairo& kairo_ )
{
    float fSum = 0.f;

    for( int i = 1 ; ; ++i )
    {
        KDropItemInfo kItem;
        float fRatio = 0.f;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fRatio ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( kairo_.SetPostItem( kItem.m_ItemID, kItem.m_nDuration, kItem.m_nPeriod, fRatio ) == false ) {
            START_LOG( cerr, L"Capsule Item 확률 초과. ItemID : " << kItem.m_ItemID << L",Ratio : " << fRatio ) << END_LOG;
            return false;
        }

        fSum += fRatio; // 총확률 체크용
        vecItemList_.push_back( std::make_pair( kItem, nGrade_ ) ); // 클라이언트 전달(아이템,아이템등급)
    }

    if( fSum < 99.999f )
    {
        START_LOG( cerr, L"Capsule Item 확률 총합이 100프로가 안된다. Sum : " << fSum << L", ItemGrade : " << nGrade_ ) << END_LOG;
        return false;
    }

    return true;
}

void KGPCapsuleManager::GetCapsuleListNotice( OUT KEVENT_CAPSULE_LIST_NOT& mapCapsuleList_ )
{
    KLocker lock( m_csCapsule );
    mapCapsuleList_ = m_mapCapsuleListNotice;
}

bool KGPCapsuleManager::GetCurrentCapsuleInfo( IN const int nCharType_, IN const int nItemKind_, IN const int nIndex_, IN const int nPrice_, OUT KCapsuleInfo& kCapsuleInfo_ )
{
    std::vector<KCapsuleInfo>::iterator vit;
    CAPSULE_INFO_LIST::iterator mit;

    KLocker lock( m_csCapsule );
    // 키값으로 캡슐정보 찾기(캐릭터타입,아이템종류,Index)
    mit = m_mapCapsuleInfoList.find( std::make_pair(nCharType_,nItemKind_) );
    _JIF( mit != m_mapCapsuleInfoList.end(), return false );

    vit = std::find_if( mit->second.begin(), mit->second.end(),
          boost::bind( &KCapsuleInfo::m_nIndex, _1 ) == nIndex_ );

    _JIF( vit != mit->second.end(), return false );

    kCapsuleInfo_ = *vit;
    return true;
}

bool KGPCapsuleManager::GetItemGrade( IN const KCapsuleInfo& kCapsuleInfo_, OUT int& nResult_ )
{
    nResult_ = -1;
    nResult_ = kCapsuleInfo_.m_ltrGrade.Decision();
    START_LOG( clog, L"아이템 등급 결정. result : " << nResult_ ) << END_LOG;

    return ( nResult_ != KLottery::CASE_BLANK );
}

bool KGPCapsuleManager::OpenCapsule( IN const KCapsuleInfo& kCapsuleInfo_, IN const int nItemGrade_, OUT KDropItemInfo& kItem_ )
{
    switch( nItemGrade_ )
    {
    case KGPCapsuleManager::IG_BASE:
        _JIF( DoGamble( kCapsuleInfo_.m_kairoBase, kItem_ ), return false );
        break;
    case KGPCapsuleManager::IG_RARE:
        _JIF( DoGamble( kCapsuleInfo_.m_kairoRare, kItem_ ), return false );
        break;
    case KGPCapsuleManager::IG_EVENT:
        _JIF( DoGamble( kCapsuleInfo_.m_kairoEvent, kItem_ ), return false );
        break;
    default:
        {
            START_LOG( cerr, L"없는 캡슐 아이템 등급입니다. ItemGrade : " << nItemGrade_ ) << END_LOG;
            SET_ERROR( ERR_CAPSULE_06 );
            return false;
        }
    }

    START_LOG( clog, L"캡슐 아이템 얻기 성공. ItemID : " << kItem_.m_ItemID << L", Count : " << kItem_.m_nDuration << L", Period : " << kItem_.m_nPeriod ) << END_LOG;
    return true;
}

bool KGPCapsuleManager::DoGamble( IN const Kairo& kairo_, OUT KDropItemInfo& kItem_ )
{
    bool bSuccess = false;
    bSuccess = kairo_.Do( kItem_ );

    if( !bSuccess ) {
        // 캡슐 아이템인데 Lottry에 등록되지 않음.
        SET_ERROR( ERR_CAPSULE_05 );
        return false;
    }

    if( kItem_.m_ItemID == 0 )
    {
        SET_ERROR( ERR_CAPSULE_04 ); // 꽝
        return false;
    }

    return true;
}

void KGPCapsuleManager::DumpCapsule()
{
    std::wstringstream stm;
    KLocker lock( m_csCapsule );
    // 클라이언트에 보내는 캡슐 정보
    stm << L"-- Capsule List(Client) --" << std::endl;
    KEVENT_CAPSULE_LIST_NOT::iterator mit;
    for( mit = m_mapCapsuleListNotice.begin() ; mit != m_mapCapsuleListNotice.end() ; ++mit )
    {
        // 키값,캡슐아이템사이즈
        stm << L"Key : " << mit->first.first << L"," << mit->first.second << ", Size : " << mit->second.size() << std::endl;

        std::vector<KCapsule>::iterator vit;
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            stm << L" Index:" << vit->m_nIndex
                << L",PriceType:" << vit->m_nPriceType
                << L",Price:" << vit->m_nPrice
                << L",Level:" << vit->m_prLevelRange.first << L"~" << vit->m_prLevelRange.second
                << L",Size:" << vit->m_vecItemList.size() << std::endl;
        }
        stm << L"----" << std::endl;
    }
    stm << std::endl;

    // 서버가 가지고 있는 캡슐 정보
    stm << L"-- Capsule List(Server) --" << std::endl;
    CAPSULE_INFO_LIST::iterator mit2;
    for( mit2 = m_mapCapsuleInfoList.begin() ; mit2 != m_mapCapsuleInfoList.end() ; ++mit2 )
    {
        // 키값,캡슐아이템사이즈
        stm << L"Key : " << mit2->first.first << L"," << mit2->first.second << ", Size : " << mit2->second.size() << std::endl;

        std::vector<KCapsuleInfo>::iterator vit2;
        for( vit2 = mit2->second.begin(); vit2 != mit2->second.end(); ++vit2 )
        {
            stm << L" Index:" << vit2->m_nIndex
                << L",PriceType:" << vit2->m_nPriceType
                << L",Price:" << vit2->m_nPrice
                << L",Level:" << vit2->m_prLevelRange.first << L"~" << vit2->m_prLevelRange.second
                << L",GradeRatio:" << vit2->m_ltrGrade.GetTotalProb() << L"/" << vit2->m_ltrGrade.GetRemaindProb() << L"(" << vit2->m_ltrGrade.GetCaseNum() << L")"
                << L",BaseList:" << vit2->m_kairoBase.GetItemNum()
                << L",RareList:" << vit2->m_kairoRare.GetItemNum()
                << L",EventList:" << vit2->m_kairoEvent.GetItemNum() << std::endl;
        }
        stm << L"----" << std::endl;
    }
    stm << std::endl;

    //std::cout2 << stm.str();

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KGPCapsuleManager::PrintCapsuleInfo( IN const int nChar_, const int nKind_, const int nIndex_ )
{
    std::wstringstream stm;
    KEVENT_CAPSULE_LIST_NOT::iterator mit;
    std::vector<KCapsule>::iterator vit;
    std::vector< std::pair<KDropItemInfo,int> >::iterator vit2;

    KLocker lock( m_csCapsule );
    mit = m_mapCapsuleListNotice.find( std::make_pair( nChar_, nKind_ ) );
    if( mit == m_mapCapsuleListNotice.end() )
    {
        START_LOG( cerr, L"해당하는 캡슐정보가 없습니다(Key). [" << nChar_ << L"/" << nKind_ << L"/" << nIndex_ << L"]" ) << END_LOG;
        return;
    }

    vit = std::find_if( mit->second.begin(), mit->second.end(),
                        boost::bind( &KCapsule::m_nIndex, _1 ) == nIndex_ );
    if( vit == mit->second.end() )
    {
        START_LOG( cerr, L"해당하는 캡슐정보가 없습니다(Index). [" << nChar_ << L"/" << nKind_ << L"/" << nIndex_ << L"]" ) << END_LOG;
        return;
    }

    // 해당 키값의 캡슐 아이템정보(클라이언트)를 출력
    stm << L"-- Capsule Info[" << nChar_ << L"/" << nKind_ << L"/" << nIndex_ << L"] --" << std::endl;
    for( vit2 = vit->m_vecItemList.begin() ; vit2 != vit->m_vecItemList.end() ; ++vit2 )
    {
        stm << L" " << vit2->first.m_ItemID
            << L", " << vit2->first.m_nDuration
            << L", " << vit2->first.m_nPeriod
            << L", " << vit2->second << std::endl;
    }
    stm << L"-- Capsule Info End --" << std::endl;

    //std::cout2 << stm.str();

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KGPCapsuleManager::PrintCapsuleItem( IN const GCITEMID dwItemID_ )
{
    KEVENT_CAPSULE_LIST_NOT::iterator mit;
    std::vector<KCapsule>::iterator vit;
    std::vector< std::pair<KDropItemInfo,int> >::iterator vit2;
    // std::wstringstream stm; 사용하지 않은 지역변수 주석처리.
    bool bResult = false;

    KLocker lock( m_csCapsule );
    // 아이템ID로 아이템정보 찾기(클라이언트 보내는 리스트)
    for( mit = m_mapCapsuleListNotice.begin() ; mit != m_mapCapsuleListNotice.end() ; ++mit )
    {
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            for( vit2 = vit->m_vecItemList.begin() ; vit2 != vit->m_vecItemList.end() ; ++vit2 )
            {
                if( vit2->first.m_ItemID == dwItemID_ )
                {
                    START_LOG( cerr, L"캡슐 아이템 정보[" << mit->first.first
                        << L"/" << mit->first.second
                        << L"/" << vit->m_nIndex
                        << L"] ItemID : " << vit2->first.m_ItemID
                        << L",Count:" << vit2->first.m_nDuration
                        << L",Period:" << vit2->first.m_nPeriod
                        << L",Grade : " << vit2->second ) << END_LOG;
                    bResult = true;
                    break;
                }
            }
        }
    }

    if( bResult == false )
    {
        START_LOG( cerr, L"클라이언트 리스트에 해당하는 캡슐아이템이 없습니다. " << dwItemID_ ) << END_LOG;
    }
}