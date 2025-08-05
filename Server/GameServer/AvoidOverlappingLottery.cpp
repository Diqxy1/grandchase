#include <dbg/dbg.hpp >
#include "Log4.h"
#include "AvoidOverlappingLottery.h"
#include <algorithm>
#include <KncUtilLua.h> // boost::bind

ImplementSingleton( KAvoidOverlappingLottery );
ImplOstmOperatorW2A( KAvoidOverlappingLottery );
NiImplementRootRTTI( KAvoidOverlappingLottery );
ImplToStringW( KAvoidOverlappingLottery )
{
    KLocker lock( m_csAOLottery );

    int n=1;
    return START_TOSTRINGW
        << TOSTRINGW( n );
}

KAvoidOverlappingLottery::KAvoidOverlappingLottery(void)
{
}

KAvoidOverlappingLottery::~KAvoidOverlappingLottery(void)
{
}

bool KAvoidOverlappingLottery::GetNonOverlappedRandomRewardByRatio( IN std::vector< KRatioRewardInfo >& vecRewardInfo_, IN int& nCount_, OUT VEC_REWARD& vecReward_ )
{
    // 이 함수 사용을 위해서는 컨테이너의 요소들이
    // 낮은 숫자부터 오름차순으로 정렬 되어 있어야 원하는 동작을 한다

    vecReward_.clear();

    if ( 0 >= nCount_ ) {
        START_LOG( cerr, L"비중복 랜덤 보상을 받을 개수 값이 잘못 됨 nCount_ : " << nCount_ ) << END_LOG;
        return false;
    }

    // 사본에 복사한 뒤 사용하자 (아래에서 엘레멘트 삭제 할 것임)
    std::vector< KRatioRewardInfo > vecRewardInfo;
    vecRewardInfo = vecRewardInfo_;

    srand( (unsigned) time( NULL ) );

    std::vector< KRatioRewardInfo >::iterator vitReward;
    // 원하는 개수만큼 보상을 뽑는다
    for ( int i = 0; i < nCount_; i++ ) {
        // 테이블의 마지막에 설정 된 비율 값으로 랜덤 수치를 구한다
        int nModuloValue = vecRewardInfo.back().m_nRatio;
        int nRandValue = rand() % nModuloValue;
        int nEqualRatio = 0;
        bool bRatioDecide = false;
        VEC_REWARD vecEqualRatioReward; // 동일 비율로 설정한 아이템들 중에서 랜덤 뽑기 위한 지역 변수
        vecEqualRatioReward.clear();

        // 랜덤 돌린 값으로 설정한 리스트 요소들 중 어떤 아이템인지 확인
        for ( vitReward = vecRewardInfo.begin(); vitReward != vecRewardInfo.end(); vitReward++ ) {
            // 랜덤 값이 설정 한 비율 값 보다 작다면 획득 처리
            if ( nRandValue < vitReward->m_nRatio ) {
                // 비율 값을 한번만 저장
                if ( false == bRatioDecide ) {
                    nEqualRatio = vitReward->m_nRatio;
                    bRatioDecide = true;
                }
                // 동일한 비율 값인지 판단
                if ( true == bRatioDecide && nEqualRatio == vitReward->m_nRatio ) {
                    KDropItemInfo kInfo;

                    kInfo.m_ItemID = vitReward->m_dwItemID;
                    kInfo.m_nDuration = vitReward->m_nCount;
                    kInfo.m_nPeriod = vitReward->m_nPeriod;

                    vecEqualRatioReward.push_back( kInfo );
                }
            }
        }

        // 처음에는 윗쪽에서 바로 VEC_REWARD에 집어넣고 break했었는데
        // 동일한 비율로 설정할 수 있게 해달라는 추가 요청으로 작업 함
        // 윗쪽에서 임시 컨테이너에 동일 비율 아이템들 넣어놓고 밑에서 랜덤하게 하나 뽑아줌
        if ( false == vecEqualRatioReward.empty() ) {
            KDropItemInfo kInfo;

            if ( 1 < vecEqualRatioReward.size() ) {
                // 동일 비율 설정 된 아이템 섞어서 처음 것으로 지급
                random_shuffle( vecEqualRatioReward.begin(), vecEqualRatioReward.end() );
                kInfo = vecEqualRatioReward.front();
            }
            else {
                kInfo = vecEqualRatioReward.front();
            }

            vecReward_.push_back( kInfo );
            vitReward = std::find_if( vecRewardInfo.begin(), vecRewardInfo.end(),
                boost::bind( &KRatioRewardInfo::m_dwItemID, _1 ) == kInfo.m_ItemID );

            if ( vecRewardInfo.end() != vitReward ) {
                // 획득한 아이템은 전체 획득 리스트에서 빼준다
                vecRewardInfo.erase( vitReward );
                // 아래에서 vitReward 사용 하면 안되므로(잘못 된 메모리 참조) 혹시나 해서 continue
                continue;
            }
        }
    }

    if ( true == vecReward_.empty() ) {
        START_LOG( cerr, L"계산 결과 보상 받을 컨테이너가 비었음. GetNonOverlappedRandomRewardByRatio()" ) << END_LOG;
        return false;
    }

    return true;
}