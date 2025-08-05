#include "User.h"
#include "TimeEventManager.h"
#include "LogManager.h"
#include "GSSimLayer.h"
#include "ResultManager.h"
#include "LogManager.h"
#include "room.h"
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

bool sTimeEvent::CheckActiveRate( IN const float fRatio_ )
{
    return ( fRatio_ <= m_fActiveRate );
}

bool sTimeEvent::CheckServerBonus( IN const int nServerNum_ )
{
    std::set<int>::const_iterator csit;

    if( m_setServerNum.empty() ) return true;
    csit = m_setServerNum.find( nServerNum_ );
    return ( csit != m_setServerNum.end() );
}

bool sTimeEvent::CheckModeBonus( IN const int nModeID_, const int nDifficult_, const int nMapID_ )
{
    std::map< int, std::set<int> >::const_iterator cmit;

    if( m_mapModeList.empty() ) return true;
    cmit = m_mapModeList.find( nModeID_ );
    if( cmit == m_mapModeList.end() ) return false;
    if( cmit->second.find( -1 ) != cmit->second.end() ) return true;

    if( KRoom::IsDungeonMode( nModeID_ ) == true )
        return ( cmit->second.find( nDifficult_ ) != cmit->second.end() );

    if( KRoom::IsPvPMode( nModeID_ ) == true )
        return ( cmit->second.find( nMapID_ ) != cmit->second.end() );

    return false;
}

//---------------------------------------------------------------------------------------------------

ImplementSingleton( KTimeEventManager );
ImplOstmOperatorW2A( KTimeEventManager );
ImplToStringW( KTimeEventManager )
{
    return stm_ << L"Time Event Size : " << GetEventCount();
}

KTimeEventManager::KTimeEventManager(void)
:uint32( 1, UINT_MAX )
,die(rng, uint32)
,m_nServerSort(0)
{
    rng.seed( ::GetTickCount() );
    m_vecEventList.clear();
}

KTimeEventManager::~KTimeEventManager(void)
{
}

void KTimeEventManager::UpdateTimeEvent( IN std::vector<sTimeEvent>& vecEventList_ )
{
    std::wstringstream stm;
    KLocker lock( m_csEventList );
    stm << L"Time Event Update Old : " << m_vecEventList.size() << L", New : " << vecEventList_.size();
    //std::clog2 << stm.str();

    m_vecEventList = vecEventList_;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KTimeEventManager::GetCorrectEvents( IN const KGameInfo& kInfo_,
                                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                         OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    _JIF( !vecUserInfo_.empty(), return false );
    mapActiveEventList_.clear();
    
    std::vector< sTimeEvent > vecCommonEventList;
    std::vector< sTimeEvent > vecPairEventList;   // 두개의 아이템.
    std::vector< sTimeEvent > vecSingleEventList; // 하나의 아이템.
    std::vector< sTimeEvent > vecCharJobEventList; // 유저 한 명이 가진 아이템이 모두에게 혜택을 주는 경우 
    std::vector< sTimeEvent > vecOneItemTwoUserList; // 아이템 하나를 두 유저 이상이 착용해야 혜택을 주는 경우
    std::vector< sTimeEvent > vecLevelEventList; // 레벨 범위 이벤트
    std::vector< sTimeEvent > vecFriendEventList; //친구함께 이벤트

    // 게임 모드, 확률등으로 한번 걸러냄, 아무 이벤트도 않걸러지면 return false;
    if( !GetApplicableEvent( kInfo_, vecUserInfo_, vecCommonEventList, vecPairEventList, vecSingleEventList, vecCharJobEventList, vecOneItemTwoUserList, vecLevelEventList, vecFriendEventList ) )
        return false;

    if( !vecCommonEventList.empty() )
        ApplyCommonEvent( vecCommonEventList, vecUserInfo_, mapActiveEventList_ );

    if( !vecPairEventList.empty() )
        ApplyPairEvent( vecPairEventList, vecUserInfo_, mapActiveEventList_ );

    if( !vecSingleEventList.empty() )
        ApplySingleEvent( vecSingleEventList, vecUserInfo_, mapActiveEventList_ );

    if( !vecCharJobEventList.empty() )
        ApplyCharJobEvent( vecCharJobEventList, vecUserInfo_, mapActiveEventList_ );

    if( !vecOneItemTwoUserList.empty() )
        ApplyOneItemTwoUserEvent( vecOneItemTwoUserList, vecUserInfo_, mapActiveEventList_ );

    if( !vecLevelEventList.empty() )
        ApplyLevelEvent( vecLevelEventList, vecUserInfo_, mapActiveEventList_);

    if( !vecFriendEventList.empty() )
        ApplyFriendEvent( vecFriendEventList, vecUserInfo_, mapActiveEventList_ );

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpEventInfo( mapActiveEventList_, kInfo_ );

    return true;
}

bool KTimeEventManager::GetApplicableEvent( IN const KGameInfo& kInfo_,
                                            IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                            OUT std::vector<sTimeEvent>& vecCommonEventList_,
                                            OUT std::vector<sTimeEvent>& vecPairEventList_,
                                            OUT std::vector<sTimeEvent>& vecSingleEventList_,
                                            OUT std::vector<sTimeEvent>& vecCharJobEventList_,
                                            OUT std::vector<sTimeEvent>& vecOneItemTwoUserList_,
                                            OUT std::vector<sTimeEvent>& vecLevelEventList_,
                                            OUT std::vector<sTimeEvent>& vecFriendEventList_ )
{
    KLocker lock( m_csEventList );
    if( m_vecEventList.empty() )
        return false;

    int nServerSort = GetServerSort();
    unsigned int uiPartyNum = vecUserInfo_.size();

    std::vector< sTimeEvent >::iterator vit;
    for ( vit = m_vecEventList.begin() ; vit != m_vecEventList.end() ; ++vit ) { 
        float fRatio = die() / (float)(UINT_MAX);

        // 발동조건 체크
        if ( vit->CheckActiveRate( fRatio ) == false ) { 
            START_LOG( clog, L"이벤트 확률로 발동하지 않음. Ratio : " << fRatio  ) << END_LOG;
            continue;
        }

        if ( vit->CheckServerBonus( nServerSort ) == false ) { 
            START_LOG( clog, L"이벤트 대상 서버가 아니다. ServerSort : "<< nServerSort  ) << END_LOG;
            continue;
        }

        if ( vit->CheckModeBonus( kInfo_.m_iGameMode, kInfo_.m_nDifficulty, kInfo_.m_iMapID ) == false ) { 
            START_LOG( clog, L"이벤트 대상 게임모드나 난이도 혹은 맵이 아니다. Mode : "<< kInfo_.m_iGameMode << L", Difficulty : " << kInfo_.m_nDifficulty << L", MapID : " << kInfo_.m_iMapID  ) << END_LOG;
            continue;
        }

        // 파티원수 체크하는 경우, 해당 파티원 수가 적으면 처리 대상이 아니다.
        if ( vit->m_nPartyNum != -1 && (int)uiPartyNum < vit->m_nPartyNum ) { 
            START_LOG( clog, L"파티원수를 체크하는데 파티원수가 적다. MyPartyNum : " << uiPartyNum << L"/ CheckPartyNum : " << vit->m_nPartyNum  ) << END_LOG;
            continue;
        }

        // 캐릭터와 전직에 제한된 이벤트의 경우
        if( !vit->m_mapCharTypePromotion.empty() &&
            GetCharJobEvent( *vit, vecUserInfo_, vecCharJobEventList_ ) == false )
        {
            START_LOG( clog, L"이벤트 대상 캐릭터와 전직이 없습니다."  ) << END_LOG;
            continue;
        }

        if ( CheckLevelEvent( *vit, vecUserInfo_, vecLevelEventList_ ) == true ) { 
            START_LOG( clog, L"레벨 이벤트" )
                << BUILD_LOG( vit->m_sLevelBonus.m_iBeginLv )
                << BUILD_LOG( vit->m_sLevelBonus.m_iEndLv )
                << BUILD_LOG( vecLevelEventList_.size() )
                << END_LOG;
            continue;
        }
        if ( vit->m_sLevelBonus.m_iBeginLv != -1 ) { 
            START_LOG( clog, L"레벨 이벤트 세팅인데 여기까지 왔다면 무효." )
                << BUILD_LOG( vit->m_sLevelBonus.m_iBeginLv )
                << END_LOG;
            continue;
        }

        if ( CheckFriendEvent( *vit, vecUserInfo_, vecFriendEventList_ ) == true ) {  // 논리적인 개선 필요..
            START_LOG( clog, L"친구 함께 이벤트" )
                << BUILD_LOG( vecFriendEventList_.size() )
                << END_LOG;
            continue;
        }

        if ( vit->m_nEventID == sTimeEvent::EM_FRIEND_TOGETHER ) {  // 하...하...하드... 매직...
            START_LOG( clog, L"친구 함께 이벤트인데 여기까지 왔다면 무효" )
                << BUILD_LOG( vecFriendEventList_.size() )
                << END_LOG;
            continue;
        }

        if ( vit->m_mapEquipItemIDA.empty() && vit->m_mapEquipItemIDB.empty() ) { 
            vecCommonEventList_.push_back( *vit );
            continue;
        }
        else if ( !vit->m_mapEquipItemIDA.empty() && !vit->m_mapEquipItemIDB.empty() ) {  // 두개의 아이템인 경우            
            if ( CheckOneItemTwoUsersEvent( *vit ) ) { // 두개의 아이템인 척 하지만, 사실은 하나의 아이템을 두 명이상 착용해야 작동하는 이벤트 (대만전용) //  090212  woosh. 
                vecOneItemTwoUserList_.push_back( *vit );
            }
            else {
                vecPairEventList_.push_back( *vit ); // 두개의 아이템인 경우
            }
            continue;
        }
        else if ( !vit->m_mapEquipItemIDA.empty() && vit->m_mapEquipItemIDB.empty() ) {  // 하나의 아이템인 경우
            vecSingleEventList_.push_back( *vit );
            continue; // 일관성?
        }

        //SiKLogManager()->Get( KLogManager::LT_EVENT_ERR )
        //    << L"이벤트 적용 조건 이상..." << GLOG::endl
        //    << BUILD_GLOG( vit->m_nEventID )
        //    << BUILD_GLOG( vit->m_mapModeList.size() )
        //    << BUILD_GLOG( vit->m_fGpRatio )
        //    << BUILD_GLOG( vit->m_fExpRatio )
        //    << BUILD_GLOG( vit->m_bApplyType )
        //    << BUILD_GLOG( vit->m_mapEquipItemIDA.size() )
        //    << BUILD_GLOG( vit->m_mapEquipItemIDB.size() )
        //    << BUILD_GLOG( vit->m_nPartyNum )
        //    << BUILD_GLOG( vit->m_fPartyExpBonus )
        //    << BUILD_GLOG( vit->m_fPartyGpBonus )
        //    << BUILD_GLOG( vit->m_fActiveRate );

    }

    // 아무것도 건질게 없다.
    return ( !vecCommonEventList_.empty() || !vecPairEventList_.empty() || !vecSingleEventList_.empty() 
             || !vecCharJobEventList_.empty() || !vecOneItemTwoUserList_.empty() || !vecLevelEventList_.empty() ||!vecFriendEventList_.empty() );
}

void KTimeEventManager::ApplyCommonEvent( IN const std::vector<sTimeEvent>& vecCommonEventList_,
                                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                         OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    // 이터레이션 하면서 무조건 넣어둔다.
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;

    for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) {  // 유저만큼
        for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) {  // 캐릭터만큼
            std::pair<DWORD,char> prKey = std::make_pair( cvit->m_dwUserUID, cvit2->first ); // 태그매치에서 이 for 가 두 번 돈다. 동일 이벤트 중복으로 들어가면 문제

            mapActiveEventList_[ prKey ].insert( mapActiveEventList_[ prKey ].end(), // 태그매치에서 같은 캐릭터를 두 번 넣을 때  이벤트가 중복될 수 있는지 확인해볼 것
                vecCommonEventList_.begin(), vecCommonEventList_.end() ); // 태그매치에서 동일캐릭만 선택일 때 키가 같은데, 추가가 될까?
            // sorting
            std::sort( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
            // unique & erase
            mapActiveEventList_[ prKey ].erase( std::unique( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ prKey ].end() );
        }
    }
}

void KTimeEventManager::ApplyCharJobEvent( IN const std::vector<sTimeEvent>& vecCharJobEventList_,
                                          IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                          OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();

    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;
    std::map<int, std::set<int> >::const_iterator cmit;
    std::set<int>::const_iterator csit;

    for ( cvitEvent = vecCharJobEventList_.begin(); cvitEvent != vecCharJobEventList_.end(); ++cvitEvent ) { 
        if ( cvitEvent->m_bApplyType ) { 
            vecEventToAll.push_back( *cvitEvent );
            continue;
        }

        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) {  // 스타트 유저 정보
            for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) {  // 캐릭터 & 전직 정보
                if ( cvitEvent->m_mapCharTypePromotion.empty() ) { 
                    continue;
                }

                cmit = cvitEvent->m_mapCharTypePromotion.find( cvit2->first );
                if ( cmit == cvitEvent->m_mapCharTypePromotion.end() ) { 
                    continue;
                }

                csit = cmit->second.find( -1 ); // 직업체크 안하는 경우.
                if ( csit == cmit->second.end() ) { 
                    csit = cmit->second.find( cvit2->second ); // 직업체크
                    if ( csit == cmit->second.end() ) { 
                        continue;
                    }
                }

                std::pair<DWORD,char> prKey = std::make_pair( cvit->m_dwUserUID, cvit2->first ); // 키 = <유저, 캐릭터>
                mapActiveEventList_[ prKey ].push_back( *cvitEvent );

                // sorting
                std::sort( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                    boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
                // unique & erase
                mapActiveEventList_[ prKey ].erase( std::unique( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                    boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ prKey ].end() );
            }
        }

    }

    if( !vecEventToAll.empty() )
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );
}

void KTimeEventManager::ApplyPairEvent( IN const std::vector<sTimeEvent>& vecPairEventList_,
                                       IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                       OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    // 이녀석이 제일 곤란하다.. ㅡㅡ;;;
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();

    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;  // 캐릭터타입,직업
    std::vector< std::pair<DWORD,char> >::const_iterator cvit3; // 유저UID,캐릭터타입

    for ( cvitEvent = vecPairEventList_.begin() ; cvitEvent != vecPairEventList_.end() ; ++cvitEvent ) { 
        std::vector<std::pair<DWORD,char> > vecApplyCharA; // A Item가지고 있는 캐릭
        std::vector<std::pair<DWORD,char> > vecApplyCharB; // B Item가지고 있는 캐릭
        vecApplyCharA.clear();
        vecApplyCharB.clear();

        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) { 
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID );
            if( !spUser )
                continue;

            for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) { 
                // 장착 유무를 확인한다.
                // 한 캐릭터가 두종류를 동시에 장착할 수 없다.
                if ( spUser->IsExistEquipItem( cvit2->first, cvitEvent->m_mapEquipItemIDA ) ) { 
                    vecApplyCharA.push_back( std::make_pair( cvit->m_dwUserUID, cvit2->first ) );
                }
                else if ( spUser->IsExistEquipItem( cvit2->first, cvitEvent->m_mapEquipItemIDB ) ) { 
                    vecApplyCharB.push_back( std::make_pair( cvit->m_dwUserUID, cvit2->first ) );
                }
            }
        }

        // 두종류의 아이템을 장착하고있는 캐릭이 있으면....
        if ( !vecApplyCharA.empty() && !vecApplyCharB.empty() ) {             
            if ( cvitEvent->m_bApplyType ) { 
                vecEventToAll.push_back( *cvitEvent );
                continue;
            }

            // vecApplyChar 를 이터레이션 하면서 해당 캐릭터에게 넣어 준다.
            for ( cvit3 = vecApplyCharA.begin() ; cvit3 != vecApplyCharA.end() ; ++cvit3 ) { 
                mapActiveEventList_[*cvit3].push_back( *cvitEvent );
            }

            for ( cvit3 = vecApplyCharB.begin() ; cvit3 != vecApplyCharB.end() ; ++cvit3 ) { 
                mapActiveEventList_[*cvit3].push_back( *cvitEvent );
            }

        }
    }

    if( !vecEventToAll.empty() )
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );

}

void KTimeEventManager::ApplySingleEvent( IN const std::vector<sTimeEvent>& vecSingleEventList_,
                                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                         OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    // 유저와 캐릭터를 이터레이션 하면서 해당 캐릭터의 장착 정보를 확인한다.
    // 개인용이면 바로 적용, 전체용이면 따로 보관했다 이터레이션 하면서 추가. (ApplyCommonEvent를 호출하면 끝인가?)
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();

    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;

    // 적용할 이벤트를 이터레이션 한다.
    for ( cvitEvent = vecSingleEventList_.begin() ; cvitEvent != vecSingleEventList_.end() ; ++cvitEvent ) { 
        // 유저를 이터레이션 한다.
        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) { 
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID );
            if( !spUser )
                continue;

            // 캐릭터를 이터레이션 한다
            for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) { 
                // 장착 유무를 확인한다.
                if ( spUser->IsExistEquipItem( cvit2->first, cvitEvent->m_mapEquipItemIDA ) == false ) { 
                    // 장착하고 있지 않다.
                    continue;
                }

                // 장착 하고 있다면 전체에게 적용할것인가?
                if ( cvitEvent->m_bApplyType == true ) { 
                    // 전체 적용
                    vecEventToAll.push_back( *cvitEvent );
                    continue;
                }

                std::pair<DWORD,char> prKey = std::make_pair( cvit->m_dwUserUID, cvit2->first );
                mapActiveEventList_[ prKey ].push_back( *cvitEvent );

                // sorting
                std::sort( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                    boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
                // unique & erase
                mapActiveEventList_[ prKey ].erase( std::unique( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                    boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ prKey ].end() );
            }

        }
    }

    if ( !vecEventToAll.empty() ) { 
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );
    }

}

void KTimeEventManager::ApplyOneItemTwoUserEvent( IN const std::vector<sTimeEvent>& vecOneItemTwoUserList_,
                                                 IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                                 OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();

    //int nEventCount = 0; // 몇 명의 유저가  이벤트 아이템을 가지고 있는가

    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;
    std::vector<std::pair<DWORD,char> >::const_iterator cvit3;

    // 적용할 이벤트를 이터레이션 한다.
    for ( cvitEvent = vecOneItemTwoUserList_.begin() ; cvitEvent != vecOneItemTwoUserList_.end() ; ++cvitEvent ) { 
        std::vector<std::pair<DWORD,char> > vecApplyChar; // Item가지고 있는 캐릭
        vecApplyChar.clear();

        // 유저를 이터레이션 한다.
        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) { 
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID );
            if( !spUser )
                continue;

            // 캐릭터를 이터레이션 한다
            for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) { 
                // 장착 유무를 확인한다.
                if ( spUser->IsExistEquipItem( cvit2->first, cvitEvent->m_mapEquipItemIDA ) == true ) { 
                    vecApplyChar.push_back( std::make_pair( cvit->m_dwUserUID, cvit2->first ) );
                }
            }

            // 2명 이상 장착 하고 있다면.
            if ( vecApplyChar.size() >= 2 ) { 
                // 전체에게 적용
                if ( cvitEvent->m_bApplyType ) { 
                    vecEventToAll.push_back( *cvitEvent );
                    continue;
                }

                for ( cvit3 = vecApplyChar.begin() ; cvit3 != vecApplyChar.end() ; ++cvit3 ) { 
                    mapActiveEventList_[ *cvit3 ].push_back( *cvitEvent );

                    // sorting
                    std::sort( mapActiveEventList_[ *cvit3 ].begin(), mapActiveEventList_[ *cvit3 ].end(), 
                        boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
                    // unique & erase
                    mapActiveEventList_[ *cvit3 ].erase( std::unique( mapActiveEventList_[ *cvit3 ].begin(), mapActiveEventList_[ *cvit3 ].end(), 
                        boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ *cvit3 ].end() );
                }
            }


        }
    }

    if ( !vecEventToAll.empty() ) { 
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );
    }
}

void KTimeEventManager::ApplyLevelEvent( IN const std::vector<sTimeEvent>& vecLevelEventList_,
                                        IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                        OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();
    int iLevel;

    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;

    for ( cvitEvent = vecLevelEventList_.begin() ; cvitEvent != vecLevelEventList_.end() ; ++cvitEvent ) {  // 이벤트
        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) {  // 유저
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID ); 
            if( !spUser )
                continue;

            for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) {  // 캐릭터
                const KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo( cvit2->first );
                if( !pkCharacterInfo )
                    continue;

                iLevel = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );

                if ( cvitEvent->m_sLevelBonus.m_iBeginLv <= iLevel && cvitEvent->m_sLevelBonus.m_iEndLv >= iLevel ) {  // 유저의 레벨과 이벤트 레벨 범위가 일치하는지 체크.
                    std::pair<DWORD,char> prKey = std::make_pair( cvit->m_dwUserUID, cvit2->first ); // UserUID, character
                    mapActiveEventList_[ prKey ].push_back( *cvitEvent );

                    // sorting
                    std::sort( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                        boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
                    // unique & erase
                    mapActiveEventList_[ prKey ].erase( std::unique( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                        boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ prKey ].end() );
                }
            }
        }
    }

    if ( !vecEventToAll.empty() ) { 
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );
    }
}

void KTimeEventManager::ApplyFriendEvent( IN const std::vector<sTimeEvent>& vecFriendEventList_,
                                          IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                          OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    std::vector<sTimeEvent> vecEventToAll; // 모두에게 적용될 이벤트..
    vecEventToAll.clear();

    std::vector<sTimeEvent>::const_iterator vit;

    for ( vit = vecFriendEventList_.begin() ; vit != vecFriendEventList_.end() ; ++vit ) {  // 이벤트
        std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
        std::vector<DWORD> vecFriends;
        std::vector<DWORD>::iterator vitFriend;
        std::vector< std::pair<char,char> >::const_iterator vitCharacter;

        for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) {  // 유저
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID); 
            if( !spUser )
                continue;

            for ( vitCharacter =  cvit->m_vecCharTypeJob.begin() ; vitCharacter != cvit->m_vecCharTypeJob.end() ; ++vitCharacter ) {  // 캐릭터
                spUser->GetOnlineFriendsList( vecFriends );

                for ( vitFriend = vecFriends.begin(); vitFriend != vecFriends.end(); ++vitFriend ) { 
                    if ( spUser->FindInDoorUserInfo( *vitFriend ) == true ) {
                        std::pair<DWORD,char> prKey = std::make_pair( cvit->m_dwUserUID, vitCharacter->first ); // UserUID, character
                        mapActiveEventList_[ prKey ].push_back( *vit );

                        // sorting
                        std::sort( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                            boost::bind(&sTimeEvent::m_nEventUID, _1) < boost::bind(&sTimeEvent::m_nEventUID, _2));
                        // unique & erase
                        mapActiveEventList_[ prKey ].erase( std::unique( mapActiveEventList_[ prKey ].begin(), mapActiveEventList_[ prKey ].end(), 
                            boost::bind(&sTimeEvent::m_nEventUID, _1) == boost::bind(&sTimeEvent::m_nEventUID, _2) ), mapActiveEventList_[ prKey ].end() );

                        break;
                    }
                }
            }
        }
    }

    if ( !vecEventToAll.empty() ) { 
        ApplyCommonEvent( vecEventToAll, vecUserInfo_, mapActiveEventList_ );
    }
}

void KTimeEventManager::CalcLevelBonus( IN OUT KEndGameReq& kPacket_,
                                       IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpEventInfo( mapActiveEventList_ );

    // get my level.
    // compare  my level with  bonus level range.
    std::vector<KGameResultIn>::iterator vit;
    int iLevel;
    for ( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit ) { 
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        std::vector< KCharacterExpInfo >::iterator vit2;
        for ( vit2 = vit->m_vecCharExp.begin() ; vit2 != vit->m_vecCharExp.end() ; ++vit2 ) { 
            // 해당유저/캐릭터 쌍으로 이벤트가 존재하는지 찾는다.
            std::pair<DWORD,char> prKey( std::make_pair(spUser->GetUID(), vit2->m_cCharType) );
            std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit;
            mit = mapActiveEventList_.find( prKey );

            const KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo( vit2->m_cCharType );
            if( !pkCharacterInfo )
                continue;
            iLevel = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );
            // 해당유저/캐릭터 쌍으로 이벤트가 존재한다면
            if ( mit != mapActiveEventList_.end() ) { 
                // 해당하는 이벤트 리스트를 이터레이션 한다.
                std::vector<sTimeEvent>::iterator vit3;
                for ( vit3 = mit->second.begin() ; vit3 != mit->second.end() ; ++vit3 ) {  // 이벤트 개수
                    if ( vit3->m_sLevelBonus.m_iBeginLv <= iLevel &&  vit3->m_sLevelBonus.m_iEndLv >= iLevel ) { 
                        float fExp = 0.0f;
                        float fGP = 0.0f;
                        // 플레이한 캐릭터로 나눈 배율을 적용한다.
                        if ( vit3->m_nEventID == sTimeEvent::EM_LEVEL_SERVER ) {
                            fExp = vit2->m_mapExpBoost[KCharacterExpInfoOut::BT_SERVER] += std::max<float>(0.f, vit3->m_fExpRatio * 1.f / vit->m_vecCharExp.size());
                            fGP = vit->m_mapGpBoost[KCharacterExpInfoOut::BT_SERVER] += std::max<float>(0.f, vit3->m_fGpRatio * 1.f / vit->m_vecCharExp.size());
                        }
                        else {
                            fExp = vit2->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fExpRatio * 1.f / vit->m_vecCharExp.size());
                            fGP = vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fGpRatio * 1.f / vit->m_vecCharExp.size());
                        }
                        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
                            START_LOG( cwarn,  L"Check Time Event(Level) : "<< spUser->GetName() << L"(Char/Job) : "
                                << (int)vit2->m_cCharType << L"/" << (int)vit2->m_cCharJob  
                                << L"[" << vit3->m_fExpRatio * 1.f / vit->m_vecCharExp.size()
                                << L"],[" << vit3->m_fGpRatio * 1.f / vit->m_vecCharExp.size()
                                << L"]" ) << END_LOG;
                        }
                    }
                }
            }
        }
    }
}

void KTimeEventManager::CalcBoostInfo( IN OUT KEndGameReq& kPacket_,
                                      IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_,
                                      IN KGameInfo& kGameInfo_,
                                      IN std::vector< std::wstring >& vecStrLogin_ )
{
    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpEventInfo( mapActiveEventList_, kGameInfo_ );

    // 파티원의 캐릭터와 전직정보(태그매치는 제외한다)
    std::vector< std::pair<char,char> > vecPartyCharTypeJob;
    if ( kGameInfo_.m_iGameMode != GC_GM_TAG_TEAM && kGameInfo_.m_iGameMode != GC_GM_TAG_SURVIVAL ) { 
        for ( std::vector<KGameResultIn>::const_iterator cvit = kPacket_.m_vecGameResult.begin() ;
            cvit != kPacket_.m_vecGameResult.end() ; ++cvit ) {
            if ( !cvit->m_vecCharExp.empty() ) { 
                vecPartyCharTypeJob.push_back( std::make_pair( cvit->m_vecCharExp.begin()->m_cCharType, cvit->m_vecCharExp.begin()->m_cCharJob ) );
            }
        }
    }

    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit ) { 
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser ) {
            continue;
        }

        // vit->m_vecCharExp가 empty이면 for문을 돌지 않는다.
        // 플레이한 캐릭터를 이터레이션 한다.
        std::vector< KCharacterExpInfo >::iterator vit2;
        for ( vit2 = vit->m_vecCharExp.begin() ; vit2 != vit->m_vecCharExp.end() ; ++vit2 ) { 
            // 해당유저/캐릭터 쌍으로 이벤트가 존재하는지 찾는다.
            std::pair<DWORD,char> prKey( std::make_pair(spUser->GetUID(), vit2->m_cCharType) );
            std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit;
            mit = mapActiveEventList_.find( prKey );
            // 해당유저/캐릭터 쌍으로 이벤트가 존재한다면
            if ( mit != mapActiveEventList_.end() ) { 
                // 해당하는 이벤트 리스트를 이터레이션 한다.
                std::vector<sTimeEvent>::iterator vit3;
                for ( vit3 = mit->second.begin() ; vit3 != mit->second.end() ; ++vit3 ) {  // 이벤트 개수
                    bool bApplyEvent = false;
                    std::map<int, std::set<int> >::const_iterator cmitCharEvent;
                    std::set<int>::const_iterator csitPromotion;

                    if ( vit3->m_sLevelBonus.m_iBeginLv != -1 ) { 
                        START_LOG( clog, L"Level 보너스는 따로 계산한다." ) << END_LOG;
                        continue;
                    }

                    if ( vit3->m_nEventID == sTimeEvent::EM_FRIEND_TOGETHER ) { 
                        START_LOG( clog, L"친구 이벤트는 따로 계산한다." ) << END_LOG;
                        continue;
                    }

                    if ( vit3->m_bApplyType == true ) { 
                        START_LOG( clog, L"ApplyType 이 true로 모두 적용된다." ) << END_LOG;
                        CalcEventRatio( spUser->GetName(), *vit3, vecStrLogin_, *vit, *vit2 );
                        continue;
                    }

                    if ( vit3->m_mapCharTypePromotion.empty() ) { 
                        START_LOG( clog, L"캐릭터 이벤트 설정없음. 이벤트 적용함." ) << END_LOG;
                        CalcEventRatio( spUser->GetName(), *vit3, vecStrLogin_, *vit, *vit2 );
                        continue;
                    }

                    cmitCharEvent = vit3->m_mapCharTypePromotion.find( vit2->m_cCharType );
                    if ( cmitCharEvent != vit3->m_mapCharTypePromotion.end() ) { 
                        csitPromotion = cmitCharEvent->second.find( -1 );
                        if ( csitPromotion != cmitCharEvent->second.end() ) { 
                            START_LOG( clog, L"이벤트 대상 캐릭터/직업 : " << (int)vit2->m_cCharType << L" / -1" ) << END_LOG;
                            CalcEventRatio( spUser->GetName(), *vit3, vecStrLogin_, *vit, *vit2 );
                            continue;
                        }

                        csitPromotion = cmitCharEvent->second.find( vit2->m_cCharJob );
                        if ( csitPromotion != cmitCharEvent->second.end() ) { 
                            START_LOG( clog, L"이벤트 대상 캐릭터/직업 : " << (int)vit2->m_cCharType << L" / " << (int)vit2->m_cCharJob ) << END_LOG;
                            CalcEventRatio( spUser->GetName(), *vit3, vecStrLogin_, *vit, *vit2 );
                            continue;
                        }

                        START_LOG( clog, L"이벤트 대상 캐릭터/직업이 아니다 : " << (int)vit2->m_cCharType << L" / " << (int)vit2->m_cCharJob ) << END_LOG;
                    }

                    if ( vit3->m_fPartyExpBonus > 0.f || vit3->m_fPartyGpBonus > 0.f ) { 
                        // 파티보너스가 설정되어 있는 경우
                        bool bIsPartyBonus = false;

                        std::vector< std::pair<char,char> >::iterator vitPartyInfo;
                        for ( vitPartyInfo = vecPartyCharTypeJob.begin() ; vitPartyInfo != vecPartyCharTypeJob.end() ; ++vitPartyInfo ) { 
                            cmitCharEvent = vit3->m_mapCharTypePromotion.find( vitPartyInfo->first );
                            if ( cmitCharEvent == vit3->m_mapCharTypePromotion.end() ) { 
                                START_LOG( clog, L"이벤트 대상 캐릭터가 없음." ) << END_LOG;
                                continue;
                            }

                            csitPromotion = cmitCharEvent->second.find( -1 );
                            if ( csitPromotion == cmitCharEvent->second.end() ) { 
                                csitPromotion = cmitCharEvent->second.find( vit2->m_cCharJob );
                                if ( csitPromotion == cmitCharEvent->second.end() ) { 
                                    START_LOG( clog, L"이벤트 대상 직업이 없음." ) << END_LOG;
                                    continue;
                                }
                            }

                            bIsPartyBonus = true;
                            START_LOG( clog, L"파티 보너스를 받는다. exp bonus : " << vit3->m_fPartyExpBonus << L", gp bonus : " << vit3->m_fPartyGpBonus  ) << END_LOG;
                            break;
                        }

                         // 파티보너스를 받는 경우.
                        if ( bIsPartyBonus == true ) { 
                            // 플레이한 캐릭터로 나눈 배율을 적용한다.
                            vit2->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fPartyExpBonus * 1.f / vit->m_vecCharExp.size());
                            vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fPartyGpBonus * 1.f / vit->m_vecCharExp.size());

                            if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
                                START_LOG( cwarn,  L"Check Time Event : "<< spUser->GetName() << L"(Char/Job/ApplyEvent) : "
                                    << (int)vit2->m_cCharType << L"/" << (int)vit2->m_cCharJob << L"/" << bApplyEvent
                                    << L"[" << vit3->m_fPartyExpBonus * 1.f / vit->m_vecCharExp.size()
                                    << L"],[" << vit3->m_fPartyGpBonus * 1.f / vit->m_vecCharExp.size()
                                    << L"]" ) << END_LOG;
                            }
                            continue;
                        }
                    }

                    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
                        START_LOG( cwarn, L"*** 시간제한이벤트 조건이 맞지 않음 ***" )
                            << BUILD_LOG( vit->m_strLogin )
                            << BUILD_LOG( vit3->m_nEventID )    // 이벤트ID
                            << BUILD_LOGc( vit2->m_cCharType )  // 내 직업
                            << BUILD_LOGc( vit2->m_cCharJob )   // 내 전직
                            << BUILD_LOG( vit3->m_fExpRatio )   // 경험치배율
                            << BUILD_LOG( vit3->m_fGpRatio )    // GP 배율
                            << BUILD_LOG( vit3->m_fPartyExpBonus )// 파티보너스
                            << BUILD_LOG( vit3->m_fPartyGpBonus ) << END_LOG;
                    }

                }
            }

        }
    }
}

void KTimeEventManager::DumpEventInfo( const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_,
                                      const KGameInfo& kGameInfo_ )
{
    START_LOG( cwarn, L"=== Dump Game Info ==== " ) 
        << BUILD_LOG( kGameInfo_.m_nStartUserNum )
        << BUILD_LOG( kGameInfo_.m_nEndUserNum )
        << BUILD_LOGc( kGameInfo_.m_cGameCategory )
        << BUILD_LOG( kGameInfo_.m_iGameMode )
        << BUILD_LOG( kGameInfo_.m_iSubGameMode )
        << BUILD_LOG( kGameInfo_.m_nDifficulty )
        << BUILD_LOG( kGameInfo_.m_iMapID )
        << BUILD_LOG( kGameInfo_.m_bRandomableMap )
        << BUILD_LOG( kGameInfo_.m_nMonsterID )
        << BUILD_LOG( kGameInfo_.m_nMonsterCount )
        << BUILD_LOG( kGameInfo_.m_biTotalExp )
        << BUILD_LOG( kGameInfo_.m_dwTotalGp )
        << BUILD_LOGc( kGameInfo_.m_cChannelType )
        << BUILD_LOG( kGameInfo_.m_nTotalLv )
        << BUILD_LOG( kGameInfo_.m_vecUserUIDs.size() )
        << BUILD_LOG( kGameInfo_.m_vecChampInfo.size() )
        << BUILD_LOG( kGameInfo_.m_mapPromotionInfo.size() ) << END_LOG;

    DumpEventInfo( mapActiveEventList_ );
}

void KTimeEventManager::DumpEventInfo( const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::const_iterator mit;
    std::vector<sTimeEvent>::const_iterator cvitEvent;
    std::map<int, std::set<int> >::const_iterator cmitCharEvent;
    std::set<int>::const_iterator csitPromotion;

    START_LOG( cwarn, L"=== Dump Event Info (Level/Friend Event)==== "  ) << END_LOG;
    for ( mit = mapActiveEventList_.begin() ; mit != mapActiveEventList_.end() ; ++mit ) { 
        START_LOG( cwarn, L" UID : " << mit->first.first << L", Char : " << (int)mit->first.second  ) << END_LOG;
        START_LOG( cwarn,  L"Data : " ) << END_LOG;

        for ( cvitEvent = mit->second.begin() ; cvitEvent != mit->second.end() ; ++cvitEvent ) { 
            START_LOG( cwarn,  L"( Event ID :" << cvitEvent->m_nEventID
                << L", ExpRatio : " << cvitEvent->m_fExpRatio
                << L", GPRatio : " << cvitEvent->m_fGpRatio
                << L", Apply: " << (cvitEvent->m_bApplyType == true ? L"T":L"F")
                << L", ActiveRate : " << cvitEvent->m_fActiveRate ) << END_LOG;

            for ( cmitCharEvent = cvitEvent->m_mapCharTypePromotion.begin() ; cmitCharEvent != cvitEvent->m_mapCharTypePromotion.end() ; ++cmitCharEvent ) { 
                START_LOG( cwarn,  L", Char : " << cmitCharEvent->first
                    << L", Job : " ) << END_LOG;
                for ( csitPromotion = cmitCharEvent->second.begin() ; csitPromotion != cmitCharEvent->second.end() ; ++csitPromotion ) { 
                    START_LOG( cwarn,  *csitPromotion << L"," ) << END_LOG;
                }
            }

            START_LOG( cwarn,  L", Begin Level : " << cvitEvent->m_sLevelBonus.m_iBeginLv
                << L", End Level : " << cvitEvent->m_sLevelBonus.m_iEndLv
                << L")" ) << END_LOG;
        }
    }
}

bool KTimeEventManager::GetSimpleTimeEvents( OUT std::set<int>& setActiveEventIDList_ )
{
    KLocker lock( m_csEventList );

    setActiveEventIDList_.clear();

    if( m_vecEventList.empty() ) return false;

    std::vector< sTimeEvent >::iterator vit;
    for ( vit = m_vecEventList.begin() ; vit != m_vecEventList.end() ; ++vit ) { 
        setActiveEventIDList_.insert( vit->m_nEventID );
    }

    return true;

}

// vecApplyCharB_ 중에 vecApplyCharA_ 와 중복되는 것이 있으면 삭제
void KTimeEventManager::EraseDuplicatedEvent( std::vector<std::pair<DWORD,char> >& vecApplyCharA_, std::vector<std::pair<DWORD,char> >& vecApplyCharB_ )
{
    std::vector<std::pair<DWORD,char> >::iterator vit1;
    std::vector<std::pair<DWORD,char> >::iterator vitErase;
    for ( vit1 = vecApplyCharA_.begin() ; vit1 != vecApplyCharA_.end() ; ++vit1 ) { 
        vitErase = std::find( vecApplyCharB_.begin(), vecApplyCharB_.end(), *vit1 );

        if ( vitErase != vecApplyCharB_.end() ) { 
            vecApplyCharB_.erase( vitErase );
        }
    }
}

void KTimeEventManager::FindMemberWinEvent( std::vector< std::wstring >& vecStrLogin_, std::wstring& m_strLogin_ , bool& bWin_ )
{
    // 아래 for에서 if 조건이 참이면 vit4는 vecStrLogin_.end() 가 아닌 것( 우승팀 멤버중 한 명)
    std::vector< std::wstring >::iterator vit4; // vecStrLogin_ 에 우승팀 멤버 로긴아이디가 들어있다
    bWin_ = false;
    for ( vit4 = vecStrLogin_.begin(); vit4 != vecStrLogin_.end() ; ++vit4 ) { 
        if ( *vit4 == m_strLogin_ ) { 
            START_LOG( clog, L"우승팀 멤버중에 있는 사람, Name : " << *vit4  ) << END_LOG;
            bWin_ = true;
            break;
        }
    }
    if ( vit4 == vecStrLogin_.end() ) { 
        bWin_ = false;
        //START_LOG( clog, L"우승팀 아님 Name :" << vit->m_strLogin  ) << END_LOG;
    }
}

bool KTimeEventManager::GetCharJobEvent( IN const sTimeEvent& timeEvent_,
                                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                         OUT std::vector<sTimeEvent>& vecCharJobEventList_ )
{
    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;
    std::map<int, std::set<int> >::const_iterator cmit;
    std::set<int>::const_iterator csit;

    bool bValidEvent = false;

    for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) {  // 스타트 유저 정보
        for ( cvit2 = cvit->m_vecCharTypeJob.begin() ; cvit2!= cvit->m_vecCharTypeJob.end() ; ++cvit2) {  // 캐릭터 & 전직 정보
            cmit = timeEvent_.m_mapCharTypePromotion.find( cvit2->first );
            if ( cmit == timeEvent_.m_mapCharTypePromotion.end() ) { 
                START_LOG( clog, L"이벤트 대상 캐릭터가 아니다. CharType : " << (int)cvit2->first ) << END_LOG;
                continue;
            }

            csit = cmit->second.find( -1 ); // 직업 검사 안하는 경우.
            if ( csit != cmit->second.end() ) { 
                START_LOG( clog, L"이벤트 대상 직업 검사 안함. CharType : " << (int)cvit2->first << L", Promotion : " << (int)cvit2->second ) << END_LOG;
                vecCharJobEventList_.push_back( timeEvent_ );
                bValidEvent = true;
                continue;
            }

            csit = cmit->second.find( cvit2->second );
            if ( csit == cmit->second.end() ) { 
                START_LOG( clog, L"이벤트 대상 직업이 아니다. Promotion : " << (int)cvit2->second ) << END_LOG;
                continue;
            }

            START_LOG( clog, L"이벤트 대상 캐릭터/직업. CharType : " << (int)cvit2->first << L", Promotion : " << (int)cvit2->second ) << END_LOG;
            vecCharJobEventList_.push_back( timeEvent_ );
            bValidEvent = true;
        }
    }
    //vit->m_nCharType 이 vecUserUIDs_ 에 없다면 무효
    if ( true == bValidEvent ) { 
        START_LOG( clog, L"이벤트 대상 캐릭터/직업이 있다." ) << END_LOG;
        return true;
    }

    START_LOG( clog, L"이벤트 대상 캐릭터/직업이 없다." ) << END_LOG;
    return false;
}

bool KTimeEventManager::CheckOneItemTwoUsersEvent( const IN sTimeEvent& timeEvent_ ) // 한 아이템을 두 유저 이상 가진 경우 true를 반환한다
{
    std::map<char,std::set<DWORD> >::const_iterator mitA;
    std::map<char,std::set<DWORD> >::const_iterator mitB;
    std::set<DWORD>::const_iterator sitA;
    std::set<DWORD>::const_iterator sitB;
    for ( mitA = timeEvent_.m_mapEquipItemIDA.begin(); mitA != timeEvent_.m_mapEquipItemIDA.end() ; ++mitA ) { 
        for ( mitB = timeEvent_.m_mapEquipItemIDB.begin(); mitB != timeEvent_.m_mapEquipItemIDB.end() ; ++mitB ) { 
            if ( ( mitA->first == -1 && mitB->first == -1 ) ) {  // DB 로부터 온 이벤트라면 char 부분에 -1이다
                for ( sitA = mitA->second.begin(); sitA != mitA->second.end(); ++sitA ) { 
                    sitB = mitB->second.find( *sitA );
                    if ( sitB != mitB->second.end() ) { 
                        return true; // 이 깊은 depth의 괄호들을 단순하게 만들고 싶다면 구조체에 데이터를 추가하면 된다.
                    }                // 한 데이터가 여러 의미를 나타내기 때문에 이런 일이 발생했다.
                }
            }
        }
    }

    return false;
}

bool KTimeEventManager::CheckLevelEvent( IN const sTimeEvent& timeEvent_,
                                         IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                         OUT std::vector<sTimeEvent>& vecLevelEventList_ ) // 잘 생각해서 만들자..
{
    if ( timeEvent_.m_sLevelBonus.m_iBeginLv == -1 && timeEvent_.m_sLevelBonus.m_iEndLv == -1 ) { 
        return false;
    }

    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator cvit2;

    int iLevel = 0;
    for ( cvit = vecUserInfo_.begin() ; cvit != vecUserInfo_.end() ; ++cvit ) { 
        for ( cvit2 = cvit->m_vecCharTypeJob.begin(); cvit2 != cvit->m_vecCharTypeJob.end() ; ++cvit2 ) {  // 태그매치만 아니면 이러한 이터레이션이 없지 않은가.
            // get user's info
            // compare with ...   level event 
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID );
            if( !spUser )
                continue;

            const KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo( cvit2->first );
            if( !pkCharacterInfo )
                continue;

            iLevel = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );

            if ( timeEvent_.m_sLevelBonus.m_iBeginLv <= iLevel && timeEvent_.m_sLevelBonus.m_iEndLv >= iLevel ) {  // 유저의 레벨과 이벤트 레벨 범위가 일치하는지 체크.
                vecLevelEventList_.push_back( timeEvent_ );
            }

        }
    }

    if( !vecLevelEventList_.empty() )
        return true;

    return false;
}

bool KTimeEventManager::CheckFriendEvent( IN const sTimeEvent& timeEvent_,
                                          IN const std::vector<KSimpleInDoorUserInfo>& vecUserInfo_,
                                          OUT std::vector<sTimeEvent>& vecFriendEventList_ )
{
    if ( timeEvent_.m_nEventID != sTimeEvent::EM_FRIEND_TOGETHER ) {  // 이것이 하드코딩이다  매직
        return false;
    }

    std::vector<KSimpleInDoorUserInfo>::const_iterator cvit;
    std::vector< std::pair<char,char> >::const_iterator vit;

    std::vector<DWORD> vecFriends;
    std::vector<DWORD>::iterator vitFriend;
    bool bFriendEventOn = false;

    for ( cvit = vecUserInfo_.begin(); cvit != vecUserInfo_.end() ; ++cvit ) { 
        for ( vit = cvit->m_vecCharTypeJob.begin(); vit != cvit->m_vecCharTypeJob.end() ; ++vit ) {  // 태그매치만 아니면 이러한 이터레이션이 없지 않은가.
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->m_dwUserUID );
            if( !spUser ) {
                continue;
            }
            spUser->GetOnlineFriendsList( vecFriends );

            for ( vitFriend = vecFriends.begin(); vitFriend != vecFriends.end(); ++vitFriend ) { 
                if ( spUser->FindInDoorUserInfo( *vitFriend ) == true ) {
                    vecFriendEventList_.push_back( timeEvent_ );
                    START_LOG( clog, L"방에서 내 온라인 친구 찾음" )
                        << BUILD_LOG( spUser->GetName() )
                        << BUILD_LOG( *vitFriend )
                        << BUILD_LOG( vecFriendEventList_.size() )
                        << END_LOG;
                    break; // 친구 이벤트는 여러 친구가 있어도 한 번만 적용되야 함.
                }
            }
        }
    }

    if ( !vecFriendEventList_.empty() ) { 
        START_LOG( clog, L"친구함께 이벤트 가능 상태" )
            << BUILD_LOG( vecFriendEventList_.size() )
            << END_LOG;
        return true;
    }

    return false;
}

void KTimeEventManager::CalcFriendEvent( IN OUT KEndGameReq& kPacket_,
                                         IN std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpEventInfo( mapActiveEventList_ );

    // get my level.
    // compare  my level with  bonus level range.
    std::vector<KGameResultIn>::iterator vit;
    std::vector<DWORD> vecFriends;
    std::vector<DWORD>::iterator vitFriend;

    for ( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit ) { 
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        std::vector< KCharacterExpInfo >::iterator vit2;
        for ( vit2 = vit->m_vecCharExp.begin() ; vit2 != vit->m_vecCharExp.end() ; ++vit2 ) { 
            // 해당유저/캐릭터 쌍으로 이벤트가 존재하는지 찾는다.
            std::pair<DWORD,char> prKey( std::make_pair(spUser->GetUID(), vit2->m_cCharType) );
            std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit;
            mit = mapActiveEventList_.find( prKey );

            bool bFriendEventOn = false;
            // 해당유저/캐릭터 쌍으로 이벤트가 존재한다면
            if ( mit != mapActiveEventList_.end() ) { 
                // 해당하는 이벤트 리스트를 이터레이션 한다.
                std::vector<sTimeEvent>::iterator vit3;
                for ( vit3 = mit->second.begin() ; vit3 != mit->second.end() ; ++vit3 ) {  // 이벤트 개수
                    if( vit3->m_nEventID != sTimeEvent::EM_FRIEND_TOGETHER )
                        continue;

                    spUser->GetOnlineFriendsList( vecFriends );
                    for ( vitFriend = vecFriends.begin(); vitFriend != vecFriends.end(); ++vitFriend ) { 
                        if ( spUser->FindInDoorUserInfo( *vitFriend ) == true ) { 
                            float fExp = 0.0f;
                            float fGP = 0.0f;
                            // 플레이한 캐릭터로 나눈 배율을 적용한다.
                            fExp = vit2->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fExpRatio * 1.f / vit->m_vecCharExp.size());
                            fGP = vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, vit3->m_fGpRatio * 1.f / vit->m_vecCharExp.size());

                            bFriendEventOn = false;
                            if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
                                START_LOG( cwarn,  L"Check Time Event(Friends!!) : "<< spUser->GetName() << L"(Char/Job) : "
                                    << (int)vit2->m_cCharType << L"/" << (int)vit2->m_cCharJob  
                                    << L"Exp :[" << vit3->m_fExpRatio * 1.f / vit->m_vecCharExp.size()
                                    << L"], GP :[" << vit3->m_fGpRatio * 1.f / vit->m_vecCharExp.size()
                                    << L"]" ) << END_LOG;
                            }
                            break; // 친구 이벤트는 여러 친구가 있어도 한 번만 적용되야 한다
                        }
                    }
                }
            }
        }
    }
}

void KTimeEventManager::CalcEventRatio( IN const std::wstring strLogin_, IN const sTimeEvent& sTimeEvent_, IN std::vector< std::wstring >& vecStrLogin_, IN OUT KGameResultIn& kResultIn_, IN OUT KCharacterExpInfo& kCharExp_ )
{
    bool bApplyEvent = false;
    bool bWin = false;

    FindMemberWinEvent( vecStrLogin_, kResultIn_.m_strLogin, bWin ); // 대전 이벤트를 밖으로 빼야 된다.

    if ( SiKResultManager()->CheckPVPEvent( sTimeEvent_.m_nEventID, kResultIn_.m_iNumKills, bWin ) ) {  //  080603  woosh. 대전이벤트 하드코딩
        // 플레이한 캐릭터로 나눈 배율을 적용한다.
        kCharExp_.m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, sTimeEvent_.m_fExpRatio * 1.f / kResultIn_.m_vecCharExp.size());
        kResultIn_.m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, sTimeEvent_.m_fGpRatio * 1.f / kResultIn_.m_vecCharExp.size());
        bApplyEvent = true;
    }

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
        START_LOG( cwarn,  L"Check Time Event : "<< strLogin_ << L"(Char/Job/ApplyEvent) : "
            << (int)kCharExp_.m_cCharType << L"/" << (int)kCharExp_.m_cCharJob << L"/" << bApplyEvent
            << L". EXP : " << sTimeEvent_.m_fExpRatio * 1.f / kResultIn_.m_vecCharExp.size()
            << L", GP : " << sTimeEvent_.m_fGpRatio * 1.f / kResultIn_.m_vecCharExp.size() )  << END_LOG;
    }
}

float KTimeEventManager::GetStrengthBonus()
{
    float fBonusRatio = 0.f;

    KLocker lock( m_csEventList );
    if( m_vecEventList.empty() ) {
        return false;
    }

    std::vector<sTimeEvent>::iterator vit;
    for ( vit = m_vecEventList.begin() ; vit != m_vecEventList.end() ; ++vit ) {
        if ( vit->m_fStrengthRatio > 0.f ) {
            fBonusRatio += vit->m_fStrengthRatio;
        }
    }

    return fBonusRatio;
}

float KTimeEventManager::GetDungeonDropRatioBonus( IN const int& nModeID_ )
{
    float fBonusRatio = 0.f;

    KLocker lock( m_csEventList );
    if( m_vecEventList.empty() ) {
        return false;
    }

    std::vector<sTimeEvent>::iterator vit;
    std::map< int, std::set<int> >::const_iterator cmit;
    for ( vit = m_vecEventList.begin() ; vit != m_vecEventList.end() ; ++vit ) {
        // 모드 정보 없을 경우..,
        if( vit->m_mapModeList.empty() ) {
            continue;
        }

        // 해당 모드가 포함되지 않을 경우..,
        cmit = vit->m_mapModeList.find( nModeID_ );
        if( cmit == vit->m_mapModeList.end() ) {
            continue;
        }

        // Drop정보가 있을 경우에는 세팅해준다.
        if ( vit->m_nDropRatio > 0 ) {
            fBonusRatio += static_cast<float>( vit->m_nDropRatio * 0.01f );
        }
    }

    return fBonusRatio;
}
