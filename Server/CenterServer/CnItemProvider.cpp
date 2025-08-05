#include "CenterSimLayer.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "CenterDBThread.h"
#include "Lua/KLuaManager.h"
#include "CnItemProvider.h"

ImplementSingleton( KCnItemProvider );
ImplOstmOperatorW2A( KCnItemProvider );
NiImplementRootRTTI( KCnItemProvider );
ImplToStringW( KCnItemProvider )
{
    KLocker lock( m_csItemProvider );
    return START_TOSTRINGW
        << TOSTRINGW( m_nOneTimeProvisionUserCount );
}

KCnItemProvider::KCnItemProvider(void)
: m_kProvideTimer( 60 * 1000 * 30 )
, m_bLock(false)
{
    m_vecProvideInfo.clear();
}

KCnItemProvider::~KCnItemProvider(void)
{
}

bool KCnItemProvider::LoadScript()
{
    KLuaManager kLuaMng;

    int nOneTimeProvisionUserCount = 0;
    DWORD dwTickGap = 0;

    _JIF( kLuaMng.DoFile( "CnItemProvider.lua" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "OneTimeProvisionUserCount", nOneTimeProvisionUserCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    {
        KLocker lock( m_csItemProvider );
        m_nOneTimeProvisionUserCount = nOneTimeProvisionUserCount;        
    }
    m_kProvideTimer.SetInterval( dwTickGap );

    START_LOG( cerr, L"센터 아이템 지급(CnItemProvider) 스크립트 읽기 완료. " )
        << BUILD_LOG( nOneTimeProvisionUserCount )
        << BUILD_LOG( dwTickGap )
        << END_LOG;

    return true;
}

// tick 돌아서 아이템 지급 event를 center db thread로 보냄
// 다른 객체에서 요청한 아이템 전부 지급할 때 까지 이 동작 반복
void KCnItemProvider::Tick()
{
    if ( m_kProvideTimer.CheckTime() ) {
        if ( false == m_vecProvideInfo.empty() ) {
            KLocker lock( m_csItemProvider );

            // 아이템 지급 DB REQ
            KDB_ECN_ITEM_PROVIDE_REQ kPacket;
            kPacket.m_vecProvideInfo = m_vecProvideInfo;
            kPacket.m_nOneTimeProvisionUserCount = m_nOneTimeProvisionUserCount;
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ECN_ITEM_PROVIDE_REQ, L"", 0, 0, kPacket );
        }
    }
}

// 지급 요청 받으면 보상 지급 컨테이너 세팅해서 tick 돌자
bool KCnItemProvider::ItemProvideReq( IN const VEC_FOR_REWARD_PROVIDE& vecProvideInfo_, IN const int& nFromWhat_ )
{
    KLocker lock( m_csItemProvider );

    // 처리 중인 것이 없을 때만 요청 가능
    if ( false == IsLock() ) {
        m_vecProvideInfo.clear();
        m_vecProvideInfo = vecProvideInfo_;
        m_nFrom = nFromWhat_;
        // 잠금
        SetLock();

        return true;
    }

    return false;
}

void KCnItemProvider::ItemProvideCompleteHandler()
{
    // from what에 따라서 각 객체에 아이템 지급 완료 되었다는 함수 호출
    switch ( m_nFrom ) {
        case FROM_SC_GROUP_PURCHASE:
            // 소셜커머스는 실제 사용하지는 않는다
            break;

        default:
            START_LOG( cwarn, L"아이템 지급 완료 처리 시 등록 된 객체 정보가 아님" ) << END_LOG;
            break;
    }

    // 잠금 해제
    ReleaseLock();
}

// 아이템 지급 완료 후 지급 정보 벡터에서 지워준다
void KCnItemProvider::ProvideCompleteProcedure( IN VEC_FOR_REWARD_PROVIDE& vecRemove_ )
{
    VEC_FOR_REWARD_PROVIDE::iterator vitRemove;
    VEC_FOR_REWARD_PROVIDE::iterator vitProvide;

    KLocker lock( m_csItemProvider );

    // DB Thread에서 이 객체가 가지고 있는 벡터 순서대로 for문 돌면서 처리했기 때문에
    // 순서대로 비교했을 때 값이 다르면 지급 처리 정상적으로 되지 않은 것으로 판단하고 다음 것으로 넘어감
    vitProvide = m_vecProvideInfo.begin();
    for ( vitRemove = vecRemove_.begin(); vitRemove != vecRemove_.end(); vitRemove++ ) {
        if ( vitProvide->m_dwUserUID != vitRemove->m_dwUserUID ) {
            if ( m_vecProvideInfo.end() == vitProvide + 1 ) {
                break;
            }
            vitProvide++;
            continue;
        }

        vitProvide = m_vecProvideInfo.erase( vitProvide );
    }

    if ( true == m_vecProvideInfo.empty() ) {
        // 지급할 정보가 비었으면 지급 완료 절차 밟자(잠금 해제 필수)
        ItemProvideCompleteHandler();
    }
}