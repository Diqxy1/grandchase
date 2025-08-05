#pragma once
// This class is singleton
#include <vector>
#include <map>
#include "KNCSingleton.h"
#include "UserPacket.h"
#include "Thread/Locker.h"

class KPeriodNotice
{
    DeclareSingleton( KPeriodNotice );

private:
    KPeriodNotice(void);
    virtual ~KPeriodNotice(void);

public:
    void UpdatePeriodNotice( IN const std::map<int, KPeriodNoticeInfo>& mapNoticeInfo ); // DB 공지 적용
    bool CompareNoticeUID( IN const std::vector<int>& vecNotice ); // DB 에서 가져온 데이터 변화있는지 검사
    void MakeNoticePacket(); // User 에서 보낼 패킷을 미리 압축,생성
    bool GetNoticePacket( OUT std::vector<int>& vecNotice, OUT KSerBuffer& kbuff );
    void DumpNotice();
    int GetMinPeriod() { return m_nMinimumPeriod; };
    void SetNoticeCntInfo();
    void SendAdminData( IN const std::wstring& wstrMsg );
    void ClearPeriodNotice();

protected:
    KncCriticalSection  m_csPeriodNotice;
    std::map<int, KPeriodNoticeInfo> m_mapNoticeInfo;
    std::map<int, int> m_mapNoticeCnt;
    std::vector<int> m_vecPeriodNoticeID; // 변화 감지용
    KSerBuffer m_kNoticebuff;
    int   m_nMinimumPeriod;
};

DefSingletonInline( KPeriodNotice );