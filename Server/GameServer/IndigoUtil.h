#pragma once

#include <utility>  // std::pair

namespace IndigoUtil
{
    int GetRPChange( int nMyRP, std::pair<int,int> prAvgRP, bool bIsBlueTeam, bool bWin, int iLeftArena );
}

#include <KNCSingleton.h>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <atltime.h>
#include <boost/tuple/tuple.hpp>

class KServerOpenTime
{
    DeclareSingleton( KServerOpenTime );
    DeclToStringW;

public:
    KServerOpenTime();
    virtual ~KServerOpenTime();

    void SetEnable( bool bEnable );     // 활성,비활성 여부 설정
    void CheckDBUpdate();               // DB 업데이트 강제 요청
    void Tick();                        // 시간체크, 일정 주기로 DB에서 설정사항을 업데이트받음.
    bool CheckEntranceEnable() const;   // 현재시간 채널 입장 가능여부 확인
    void UpdateDataFromDB( IN const std::vector< boost::tuple<short,CTime,CTime> > vecData );
    CTime GetToadysCloseTime() const;
    CTime GetToadysOpenTime() const;

private:
    void SetToadysEntranceTime();       // 금일 입장가능 시간

protected:
    DWORD                                   m_dwUpdateTick;  // DB에서의 데이터 갱신 시각 기억.

    bool                                    m_bCheckEnable;
    mutable KncCriticalSection              m_csOpenTime;
    std::vector< std::pair<CTime,CTime> >   m_vecExceptTime;
    std::pair<CTime,CTime>                  m_prDayOfWeek[7];
    CTime                                   m_cTodaysOpenTime;
    CTime                                   m_cTodaysCloseTime;
};

DefSingletonInline( KServerOpenTime );