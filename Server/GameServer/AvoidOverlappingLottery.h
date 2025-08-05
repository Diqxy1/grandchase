#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "Lua/KLuaManager.h"
#include "UserPacket.h"

// 중복 되지 않는 보상 획득을 위한 클래스
// KRatioRewardInfo 컨테이너와 수량을 넘기면 비율에 따라 지정 수량의 보상을 담아준다
// 동작 방식 설명 :
//      1. KRatioRewardInfo 컨테이너 비율이 2, 5, 10, 18 으로 4개 설정 되었다고 가정
//      2. 가장 마지막 수치인 30을 기준으로 랜덤 수치를 결정 한다
//      3. 처음 요소부터 마지막 요소까지 순회하면서 2번에서 결정 한 랜덤 수치가 해당 요소보다 작은지 판별 한다
//      4. 만약 랜덤 수치가 7이 나왔다면 10보다 작으므로 두번째 보상을 획득 하게 된다
// 비율 계산 방법 :
//      2, 5, 10, 18의 비율은 뒤에서 앞을 빼면 2, 3, 5, 8이고 (가장 처음은 0을 뺌)
//	    가장 뒤의 수치 18로 %를 구하면 11.11, 16.66, 27.77, 44.44 (100 / 18 * 뺀 값 << 계산 공식이지만 실제로 소숫점에 의한 손실은 일어나지 않는다)

class KAvoidOverlappingLottery
{
    DeclareSingleton( KAvoidOverlappingLottery );
    NiDeclareRootRTTI( KAvoidOverlappingLottery );
    DeclToStringW;
public:
    KAvoidOverlappingLottery(void);
    ~KAvoidOverlappingLottery(void);

    bool GetNonOverlappedRandomRewardByRatio( IN std::vector< KRatioRewardInfo >& vecRewardInfo, IN int& nCount, OUT VEC_REWARD& vecReward );

private:
    mutable KncCriticalSection m_csAOLottery;
};

DefSingletonInline( KAvoidOverlappingLottery );
DeclOstmOperatorA( KAvoidOverlappingLottery );