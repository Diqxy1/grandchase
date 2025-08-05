#include "UserAchievement.h"
#include "UserPacket.h"

KUserAchievement::KUserAchievement(void)
{
}

KUserAchievement::~KUserAchievement(void)
{
}

void KUserAchievement::ClearCompleteList()
{
    m_setCompledList.clear();
}

void KUserAchievement::SetCompleteList( IN std::set<int>& setCompleteList_ )
{
    m_setCompledList.swap( setCompleteList_ );
}

void KUserAchievement::GetCompleteList( OUT std::set<int>& setCompleteList_ )
{
    setCompleteList_ = m_setCompledList;
}

const std::set<int>& KUserAchievement::GetCompleteList()
{
    return m_setCompledList;
}

void KUserAchievement::AddCompleteItem( IN const int nCompleteAchievement_ )
{
    m_setCompledList.insert( nCompleteAchievement_ );
}

bool KUserAchievement::IsInCompleteList( IN const int nAchieveID_ )
{
    return (m_setCompledList.find( nAchieveID_ ) != m_setCompledList.end());
}

void KUserAchievement::ClearWaitList()
{
    m_setWaitForComplet.clear();
}

void KUserAchievement::SetWaitList( IN std::set<int>& setWaitListList_ )
{
    m_setWaitForComplet.swap( setWaitListList_ );
}

void KUserAchievement::GetWaitList( OUT std::set<int>& setWaitListList_ )
{
    setWaitListList_ = m_setWaitForComplet; 
}

const std::set<int>& KUserAchievement::GetWaitList()
{
    return m_setWaitForComplet;
}

void KUserAchievement::AddWaitList( IN const int nWaitListAchievement_ )
{
    m_setWaitForComplet.insert( nWaitListAchievement_ );
}

void KUserAchievement::AddWaitList( IN const std::set<int>& setWaitListList_ )
{
    if ( setWaitListList_.empty() )
        return;

    m_setWaitForComplet.insert( setWaitListList_.begin(), setWaitListList_.end() );
}

bool KUserAchievement::RemoveFromWaitList( IN const int nAchieveID_ )
{
    std::set<int>::iterator sit;
    sit = m_setWaitForComplet.find( nAchieveID_ );

    if ( sit == m_setWaitForComplet.end() ) {
        return false;
    }

    m_setWaitForComplet.erase( sit );
    return true;
}

bool KUserAchievement::IsInWaitList( IN const int nAchieveID_ )
{
    return (m_setWaitForComplet.find( nAchieveID_ ) != m_setWaitForComplet.end());
}

void KUserAchievement::ClearGoalInfo()
{
    m_mapGoalInfo.clear();
}

void KUserAchievement::SetGoalInfo( IN const std::map<int, int>& mapGoalInfo_ )
{
    m_mapGoalInfo.clear();
    std::map<int, int>::const_iterator mit;
    for ( mit = mapGoalInfo_.begin() ; mit != mapGoalInfo_.end() ; ++mit ) {
        m_mapGoalInfo.insert( std::map<int, KCacheData<int> >::value_type(mit->first, mit->second ) );
    }
}

void KUserAchievement::GetGoalInfo( OUT std::map<int, int>& mapGoalInfo_ )
{
    mapGoalInfo_.clear();
    std::map<int, KCacheData<int> >::iterator mit;
    for( mit = m_mapGoalInfo.begin() ; mit != m_mapGoalInfo.end() ; ++mit ) {
        mapGoalInfo_.insert( std::map<int, int>::value_type( mit->first, mit->second.GetValue() ) );
    }
}

void KUserAchievement::UpdateGoalInfo( IN const std::map<int,int>& mapGoalInfo_ )
{
    std::map<int,int>::const_iterator mit;
    for ( mit = mapGoalInfo_.begin() ; mit != mapGoalInfo_.end() ; ++mit ) {

        std::map<int, KCacheData<int> >::iterator mitGoal;

        mitGoal = m_mapGoalInfo.find( mit->first );
        if ( mitGoal != m_mapGoalInfo.end() ) {
            mitGoal->second.SetValue( mit->second );
        }
        else { // 없다면 추가..
            m_mapGoalInfo.insert( std::map<int, KCacheData<int> >::value_type(mit->first, mit->second ) );
        }
    }
}

bool KUserAchievement::GetDiffCountAndAdjust( OUT std::map<int,int>& mapDiffCount_ )
{
    mapDiffCount_.clear();
    std::map<int, KCacheData<int> >::iterator mit;
    for ( mit = m_mapGoalInfo.begin() ; mit != m_mapGoalInfo.end() ; ++mit ) {
        if ( mit->second.IsChanged() ) {
            mapDiffCount_.insert( std::map<int,int>::value_type( mit->first, mit->second.GetChangeValue() ) );
            mit->second.AdjustValue();
        }
    }
    return ( !mapDiffCount_.empty() );
}

void KUserAchievement::GetExpectCounterData( IN OUT std::map<int,int>& mapCounterIDs_ )
{
    std::map<int,int>::iterator mit;
    for( mit = mapCounterIDs_.begin() ; mit != mapCounterIDs_.end() ; ++mit ) {
        std::map<int, KCacheData<int> >::iterator mitGoal;
        mitGoal = m_mapGoalInfo.find( mit->first );
        if ( mitGoal != m_mapGoalInfo.end() ) {
            mit->second += mitGoal->second.GetValue();
        }
    }
}