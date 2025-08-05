#pragma once
#include <map>
#include <set>
#include <vector>
#include "CacheData.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

struct KGoalCountInfo; // 전방 선언
class KUserAchievement
{
public:
    KUserAchievement(void);
    ~KUserAchievement(void);

    void ClearCompleteList();
    void SetCompleteList( IN std::set<int>& setCompleteList_  );
    void GetCompleteList( OUT std::set<int>& setCompleteList_  );
    const std::set<int>& GetCompleteList();
    void AddCompleteItem( IN const int nCompleteAchievement_ );
    bool IsInCompleteList( IN const int nAchieveID_ );

    void ClearWaitList();
    void SetWaitList( IN std::set<int>& setWaitListList_  );
    void GetWaitList( OUT std::set<int>& setWaitListList_  );
    void AddWaitList( IN const int nWaitListAchievement_ );
    void AddWaitList( IN const std::set<int>& setWaitListList_ );
    bool RemoveFromWaitList( IN const int nAchieveID_ );
    bool IsInWaitList( IN const int nAchieveID_ );

    void ClearGoalInfo();
    void SetGoalInfo( IN const std::map<int, int>& mapGoalInfo_ );
    void GetGoalInfo( OUT std::map<int, int>& mapGoalInfo_ );
    const std::set<int>& GetWaitList();
    void UpdateGoalInfo( IN const std::map<int,int>& mapGoalInfo_ );

    bool GetDiffCountAndAdjust( OUT std::map<int,int>& mapDiffCount_ );
    void GetExpectCounterData( IN OUT std::map<int,int>& mapCounterIDs_ );

private:
    std::set<int> m_setCompledList;  // 업적 ID
    std::set<int> m_setWaitForComplet; // 완료 가능
    std::map<int, KCacheData<int> > m_mapGoalInfo; // 목표 정보
};
