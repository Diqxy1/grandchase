#include "stdafx.h"

extern std::wstring GetEnableEnteranceTimeStr( int iStartHour_, int iStartMin_, int iEndHour_, int iEndMin_ );

KGCHeroDungeonManager::KGCHeroDungeonManager()
: m_PointItemID ( 0 )
, m_tmResetTime ( 0 ) 
{
    m_vecDungeonInfo.clear();
    m_mapDungeonPersonalRecord.clear();
    m_mapMyCurDungeonRank.clear();
    m_mapMyOldDungeonRank.clear();
    m_mapCurDungeonRank.clear();
    m_mapOldDungeonRank.clear();
}

KGCHeroDungeonManager::~KGCHeroDungeonManager()
{
}

void KGCHeroDungeonManager::Release()
{
    m_vecDungeonInfo.clear();
    m_mapDungeonPersonalRecord.clear();
    m_mapMyCurDungeonRank.clear();
    m_mapMyOldDungeonRank.clear();
    m_mapCurDungeonRank.clear();
    m_mapOldDungeonRank.clear();
}

void KGCHeroDungeonManager::SetHeroDungeonInfo( const std::vector<KHeroDungeon>& _vecDungeonInfo, 
                                         GCITEMID _PointItemID, time_t _tmResetTime )
{
    m_vecDungeonInfo = _vecDungeonInfo;
    m_PointItemID = _PointItemID;
    m_tmResetTime = _tmResetTime;
}

std::wstring KGCHeroDungeonManager::GetInitTime()
{
    //CTime curTime = CTime::GetCurrentTime();
    CTime curTime = g_kGlobalValue.m_tmServerTime;
    int iCurChangeMinute = curTime.GetHour() * 60 + curTime.GetMinute();

    CTime resetTime(m_tmResetTime + g_kGlobalValue.m_tmServerSyncTime);
    int iResetChangeMinute = resetTime.GetHour() * 60 + resetTime.GetMinute();
    int iRemainTime;

    if( iResetChangeMinute == 0 )
        iRemainTime = 1440 - iCurChangeMinute;
    else if( iResetChangeMinute < iCurChangeMinute )
        iRemainTime = 1440 - iCurChangeMinute + iResetChangeMinute;
    else if( iResetChangeMinute > iCurChangeMinute )
        iRemainTime = iResetChangeMinute - iCurChangeMinute;
    else
        iRemainTime = 0;

    if( iRemainTime == 0 )
        return g_pkStrLoader->GetReplacedString( STR_ID_HERO_DUNGEON_ENTER_INIT_TIME, "ii", 0, 0 );
    else
        return g_pkStrLoader->GetReplacedString( STR_ID_HERO_DUNGEON_ENTER_INIT_TIME, "ii", static_cast<int>(iRemainTime/60), iRemainTime%60 );

}

std::wstring KGCHeroDungeonManager::GetNextEntranceTime( EGCGameMode _eGameMode )
{
    std::pair<std::pair<int,int>,std::pair<int,int>> _nextTime;
    _nextTime = std::make_pair(std::make_pair(0,0), std::make_pair(0,0));
    GetNextTime( _eGameMode, _nextTime );
    
    return GetEnableEnteranceTimeStr(_nextTime.first.first,_nextTime.first.second,_nextTime.second.first,_nextTime.second.second);
}

std::wstring KGCHeroDungeonManager::GetOpenEntranceTime( EGCGameMode _eGameMode )
{
	std::pair<std::pair<int,int>,std::pair<int,int>> _openTime;
	_openTime = std::make_pair(std::make_pair(0,0), std::make_pair(0,0));
	GetOpenTime( _eGameMode, _openTime );

	return GetEnableEnteranceTimeStr(_openTime.first.first,_openTime.first.second,_openTime.second.first,_openTime.second.second);
}

bool KGCHeroDungeonManager::GetHeroDungeon( EGCGameMode _eGameMode, KHeroDungeon& kHeroDungeon )
{
    std::vector<KHeroDungeon>::iterator vit = std::find_if( m_vecDungeonInfo.begin(), 
        m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == _eGameMode );


    if( vit != m_vecDungeonInfo.end() )
    {
        kHeroDungeon = *vit;
        return true;
    }

    return false;
}

bool KGCHeroDungeonManager::GetOpenTime( EGCGameMode _eGameMode, HERO_OPEN_TIME& _sOpenTime )
{
    std::vector<KHeroDungeon>::iterator vit = std::find_if( m_vecDungeonInfo.begin(), 
        m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == _eGameMode );    

    //CTime curTime = CTime::GetCurrentTime();
    CTime curTime = g_kGlobalValue.m_tmServerTime;

    int iCurChangeMinute = curTime.GetHour() * 60 + curTime.GetMinute();    
    if( vit != m_vecDungeonInfo.end() )
    {
        std::wstringstream str;

        std::map<USHORT,std::pair<time_t,time_t> >::iterator mit = vit->m_mapOpenTime.begin();                
        for( ; mit != vit->m_mapOpenTime.end(); ++mit )
        {
            CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

            int iStartChangeMinute  = startTime.GetHour() * 60 + startTime.GetMinute();
            int iEndChangeMinute    = endTime.GetHour() * 60 + endTime.GetMinute();     

            
            //현재 시간에 입장 가능한 시간
            if( iCurChangeMinute >= iStartChangeMinute && iCurChangeMinute <= iEndChangeMinute )
            {   
                _sOpenTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                                            std::make_pair(endTime.GetHour(),endTime.GetMinute()));
              
                return true;
            }
            //  다음 입장 가능한 시간
            else if( iCurChangeMinute < iStartChangeMinute )
            {
                _sOpenTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                                            std::make_pair(endTime.GetHour(),endTime.GetMinute()));

                return false;
            }
        }
        
        //  금일 다음 입장 가능시각이 없다면 여기까지 온다.
        //  최초 입장시각을 리턴한다.
        mit = vit->m_mapOpenTime.begin();
        if ( mit != vit->m_mapOpenTime.end() )
        {
            CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

            _sOpenTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                std::make_pair(endTime.GetHour(),endTime.GetMinute()));

            return false;
        }
    } 

    return false;
}

bool KGCHeroDungeonManager::GetNextTime( EGCGameMode _eGameMode, HERO_OPEN_TIME& _sNextTime )
{
    std::vector<KHeroDungeon>::iterator vit = std::find_if( m_vecDungeonInfo.begin(), 
        m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == _eGameMode );    

    //CTime curTime = CTime::GetCurrentTime();
    CTime curTime = g_kGlobalValue.m_tmServerTime;

    int iCurChangeMinute = curTime.GetHour() * 60 + curTime.GetMinute();    

    if( vit != m_vecDungeonInfo.end() )
    {
        std::wstringstream str;

        std::map<USHORT,std::pair<time_t,time_t> >::iterator mit = vit->m_mapOpenTime.begin();                
        for( ; mit != vit->m_mapOpenTime.end(); ++mit )
        {
            CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

            int iStartChangeMinute  = startTime.GetHour() * 60 + startTime.GetMinute();
            int iEndChangeMinute    = endTime.GetHour() * 60 + endTime.GetMinute();

            //현재 입장 가능시간이면 그 다음 시간 가져온다.
            if( iCurChangeMinute >= iStartChangeMinute && iCurChangeMinute <= iEndChangeMinute )
            {   
                mit++;
                if( mit == vit->m_mapOpenTime.end() )
                    mit = vit->m_mapOpenTime.begin();

                CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
                CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

                _sNextTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                    std::make_pair(endTime.GetHour(),endTime.GetMinute()));

                return true;
            }
            else if( iCurChangeMinute < iStartChangeMinute )
            {
                _sNextTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                    std::make_pair(endTime.GetHour(),endTime.GetMinute()));

                return false;
            }            
        }

        //  금일 다음 입장 가능시각이 없다면 여기까지 온다.
        //  최초 입장시각을 리턴한다.
        mit = vit->m_mapOpenTime.begin();
        if ( mit != vit->m_mapOpenTime.end() )
        {
            CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

            _sNextTime = std::make_pair(std::make_pair(startTime.GetHour(),startTime.GetMinute()), 
                std::make_pair(endTime.GetHour(),endTime.GetMinute()));

            return false;
        }
    } 

    return false;
}

void KGCHeroDungeonManager::SetHeroItemCatalog( std::map<PAIR_USHORT_DWORD, KManufactureItem>& mapCatalog )
{
    if( !m_mapCatalog.empty() )
        return;

    m_mapCatalog.clear();
    m_mapCatalog = mapCatalog;
}

void KGCHeroDungeonManager::SetHeroItemMaterial( std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterial )
{
    if( !m_mapMaterial.empty() )
        return;

    m_mapMaterial.clear();
    m_mapMaterial = mapMaterial;
}

bool KGCHeroDungeonManager::GetClearReward ( EGCGameMode emGameMode, std::map<INDEX_GCITEMID,KManufactureItem>& _mapClearReward )
{
    _mapClearReward.clear();


    for ( int i = 0; i < (int)m_vecDungeonInfo.size(); ++i ) 
    {
        if ( emGameMode == static_cast<EGCGameMode>(m_vecDungeonInfo[i].m_nModeID) ) 
        {
            _mapClearReward = m_vecDungeonInfo[i].m_mapClearReward;
            return true;
        }
    }

    return false;
}

int  KGCHeroDungeonManager::GetNumReward( EGCGameMode emGameMode )
{
    for ( int i = 0; i < (int)m_vecDungeonInfo.size(); ++i ) 
    {
        if ( emGameMode == static_cast<EGCGameMode>(m_vecDungeonInfo[i].m_nModeID) ) 
        {
            return m_vecDungeonInfo[i].m_usClearRewardCount;
        }
    }

    return 0;
}

int KGCHeroDungeonManager::GetClearMonsterLevel( EGCGameMode emGameMode )
{
    for ( int i = 0; i < (int)m_vecDungeonInfo.size(); ++i ) 
    {
        if ( emGameMode == static_cast<EGCGameMode>(m_vecDungeonInfo[i].m_nModeID) ) 
        {
            return m_vecDungeonInfo[i].m_dwMonsterLv;
        }
    }

    return 0;
}

//  첫번째 오픈 시간 전이다.
bool KGCHeroDungeonManager::IsFirstOpenTime( int nGameMode )
{
    std::vector<KHeroDungeon>::iterator vit = std::find_if( m_vecDungeonInfo.begin(), 
        m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == nGameMode );    

    //CTime curTime = CTime::GetCurrentTime();
    CTime curTime = g_kGlobalValue.m_tmServerTime;

    int iCurChangeMinute = curTime.GetHour() * 60 + curTime.GetMinute();    
    if( vit == m_vecDungeonInfo.end() )
        return false;

    std::map<USHORT,std::pair<time_t,time_t> >::iterator mit = vit->m_mapOpenTime.begin();
    if ( mit == vit->m_mapOpenTime.end() ) 
        return false;
    
    CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
    int iStartChangeMinute  = startTime.GetHour() * 60 + startTime.GetMinute();

    if ( iCurChangeMinute < iStartChangeMinute ) 
        return true;

    return false;
}

std::pair<int, int> KGCHeroDungeonManager::GetClearInfo( EGCGameMode _eGameMode )
{
	std::vector<KHeroDungeon>::iterator vit = std::find_if( m_vecDungeonInfo.begin(), 
		m_vecDungeonInfo.end(),
		boost::bind(&KHeroDungeon::m_nModeID,_1) == _eGameMode );    

	if ( vit == m_vecDungeonInfo.end() ) {
		return std::pair<int, int>(0,0);
	}

	return std::pair<int, int>(GetHeroDungeonPlayCount(_eGameMode), vit->m_usMaxClearPerDay);
}

void KGCHeroDungeonManager::SetHeroItemUpgradeInfo( const IN KEVENT_HERO_ITEM_UPGRADE_INFO_ACK& Info_ )
{
    if( !m_mapUpgradeInfo.empty() )
        return;

    m_mapUpgradeInfo.clear();
    m_mapUpgradeInfo = Info_;
}

void KGCHeroDungeonManager::SetHeroPlayData( std::map< int, KHeroPlayData > _mapHeroPlayData )
{
    m_mapHeroPlayData = _mapHeroPlayData;
}

void KGCHeroDungeonManager::ReSetHeroPlayData()
{
    std::map< int, KHeroPlayData >::iterator mit = m_mapHeroPlayData.begin();
    for( ; mit != m_mapHeroPlayData.end(); ++mit)
    {
        mit->second.m_ClearCount = 0;
    }
}

void KGCHeroDungeonManager::AddHeroDungeonPlayCount( int nGameMode )
{
    //  최초 시간보다 이전시간에 클리어 했다면 스킵한다.
    if ( IsFirstOpenTime( nGameMode ) ) 
        return;

    std::map< int, KHeroPlayData >::iterator mit = m_mapHeroPlayData.find( nGameMode );

    if( mit != m_mapHeroPlayData.end() ) {
        if( m_mapHeroPlayData[nGameMode].m_ClearCount < GetClearInfo( (EGCGameMode)nGameMode ).second )
            mit->second.m_ClearCount++;
    }
    else{
        m_mapHeroPlayData[nGameMode] = KHeroPlayData();
        m_mapHeroPlayData[nGameMode].m_ClearCount = 1;
    }
    m_mapHeroPlayData[nGameMode].m_IsTicketUsed = false;
}

void KGCHeroDungeonManager::SetHeroDungeonPlayTicket( int nGameMode )
{
    std::map< int, KHeroPlayData >::iterator mit = m_mapHeroPlayData.find( nGameMode );

    if ( mit != m_mapHeroPlayData.end() ) {
        mit->second.m_IsTicketUsed = true;
    }
}

int KGCHeroDungeonManager::GetHeroDungeonPlayCount( int nGameMode )
{
    std::map< int, KHeroPlayData >::iterator mit = m_mapHeroPlayData.find( nGameMode );

    if( mit != m_mapHeroPlayData.end() )
        return mit->second.m_ClearCount;
    return 0;
}

bool KGCHeroDungeonManager::GetHeroDungeonPlayTicket( int nGameMode )
{
    std::map< int, KHeroPlayData >::iterator mit = m_mapHeroPlayData.find(nGameMode);

    if (mit != m_mapHeroPlayData.end())
        return mit->second.m_IsTicketUsed;
    return true;
}

void KGCHeroDungeonManager::SetDungeonPersonalRecord( std::map< int, std::map< int, KDungeonPersonalRecord > > _mapDungeonPersonalRecord )
{
    m_mapDungeonPersonalRecord = _mapDungeonPersonalRecord;
}

bool KGCHeroDungeonManager::GetDungeonPersonalRecord( int nGameMode, std::map< int, KDungeonPersonalRecord >& _mapDungeonPersonalRecord )
{
    _mapDungeonPersonalRecord.clear();

    std::map< int, std::map< int, KDungeonPersonalRecord > >::iterator iter = m_mapDungeonPersonalRecord.find( nGameMode );

    if ( iter == m_mapDungeonPersonalRecord.end() )
        return false;

    _mapDungeonPersonalRecord = iter->second;

    return true;
}

void KGCHeroDungeonManager::SetMyDungeonRank( bool bCurrent, int nCharType, int nModeID, const KDungeonRankInfo& kDungeonRank )
{
    if ( bCurrent )
    {
        std::map< int, std::map< int, KDungeonRankInfo > >::iterator iter = m_mapMyCurDungeonRank.find( nCharType );
        if ( iter == m_mapMyCurDungeonRank.end() )
        {
            std::map< int, KDungeonRankInfo > mapCurDungeonRank;
            
            mapCurDungeonRank.insert( std::make_pair( nModeID, kDungeonRank ) );
            m_mapMyCurDungeonRank.insert( std::make_pair( nCharType, mapCurDungeonRank ) );
        }
        else
        {
            std::map< int, KDungeonRankInfo >& mapCurDungeonRank = iter->second;
            mapCurDungeonRank[nModeID] = kDungeonRank;
        }

        
    }
    else
    {

        std::map< int, std::map< int, KDungeonRankInfo > >::iterator iter = m_mapMyOldDungeonRank.find( nCharType );
        if ( iter == m_mapMyOldDungeonRank.end() )
        {
            std::map< int, KDungeonRankInfo > mapOldDungeonRank;

            mapOldDungeonRank.insert( std::make_pair( nModeID, kDungeonRank ) );
            m_mapMyOldDungeonRank.insert( std::make_pair( nCharType, mapOldDungeonRank ) );
        }
        else
        {
            std::map< int, KDungeonRankInfo >& mapOldDungeonRank = iter->second;
            mapOldDungeonRank[nModeID] = kDungeonRank;
        }
    }
}

bool KGCHeroDungeonManager::GetMyDungeonRank( bool bCurrent, int nCharType, int nModeID, KDungeonRankInfo& kDungeonRank )
{
    if ( bCurrent )
    {
        std::map< int, std::map< int, KDungeonRankInfo > >::iterator iter = m_mapMyCurDungeonRank.find( nCharType );
        if ( iter == m_mapMyCurDungeonRank.end() )
            return false;


        std::map< int, KDungeonRankInfo >& mapCurDungeonRank = iter->second;
        std::map< int, KDungeonRankInfo >::iterator iter_mode = mapCurDungeonRank.find( nModeID );
        if ( iter_mode == mapCurDungeonRank.end() )
            return false;

        if ( iter_mode->second.m_nRank == -1 )
            return false;

        kDungeonRank = iter_mode->second;
        
        return true;        
    }
    else
    {
        std::map< int, std::map< int, KDungeonRankInfo > >::iterator iter = m_mapMyOldDungeonRank.find( nCharType );
        if ( iter == m_mapMyOldDungeonRank.end() )
            return false;


        std::map< int, KDungeonRankInfo >& mapOldDungeonRank = iter->second;
        std::map< int, KDungeonRankInfo >::iterator iter_mode = mapOldDungeonRank.find( nModeID );
        if ( iter_mode == mapOldDungeonRank.end() )
            return false;

        if ( iter_mode->second.m_nRank == -1 )
            return false;

        kDungeonRank = iter_mode->second;

        return true;   
    }
}

void KGCHeroDungeonManager::SetDungeonRank( bool bCurrent, int nModeID, int nPageNum, std::vector< KDungeonRankInfo >& vecDungeonRank )
{
    if ( bCurrent )
    {
        std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>::iterator iter = m_mapCurDungeonRank.find( nModeID );
        if ( iter == m_mapCurDungeonRank.end() )
        {
            std::map<int, std::vector< KDungeonRankInfo >> mapDungeonRank;
            mapDungeonRank.insert( std::make_pair( nPageNum, vecDungeonRank ) );

            m_mapCurDungeonRank.insert( std::make_pair( nModeID, mapDungeonRank ) );
        }
        else
        {
            std::map<int, std::vector< KDungeonRankInfo >>& mapDungeonRank = iter->second;

            std::map<int, std::vector< KDungeonRankInfo >>::iterator iterPage = mapDungeonRank.find( nPageNum );
            
            if ( iterPage == mapDungeonRank.end() )
            {
                mapDungeonRank.insert( std::make_pair( nPageNum, vecDungeonRank ) );
            }
            else
            {
                iterPage->second = vecDungeonRank;
            }
        }
    }
    else
    {
        std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>::iterator iter = m_mapOldDungeonRank.find( nModeID );
        
        if ( iter == m_mapOldDungeonRank.end() )
        {
            std::map<int, std::vector< KDungeonRankInfo >> mapDungeonRank;
            mapDungeonRank.insert( std::make_pair( nPageNum, vecDungeonRank ) );

            m_mapOldDungeonRank.insert( std::make_pair( nModeID, mapDungeonRank ) );
        }
        else
        {
            std::map<int, std::vector< KDungeonRankInfo >>& mapDungeonRank = iter->second;

            std::map<int, std::vector< KDungeonRankInfo >>::iterator iterPage = mapDungeonRank.find( nPageNum );

            if ( iterPage == mapDungeonRank.end() )
            {
                mapDungeonRank.insert( std::make_pair( nPageNum, vecDungeonRank ) );
            }
            else
            {
                iterPage->second = vecDungeonRank;
            }
        }
    }
}

bool KGCHeroDungeonManager::GetDungeonRank( bool bCurrent, int nModeID, int nPageNum, std::vector< KDungeonRankInfo >& vecDungeonRank )
{
    vecDungeonRank.clear();

    if ( bCurrent )
    {
        std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>::iterator iter = m_mapCurDungeonRank.find( nModeID );
        
        if ( iter == m_mapCurDungeonRank.end() )
            return false;

        std::map<int, std::vector< KDungeonRankInfo >>& mapDungeonRank = iter->second;
        
        std::map<int, std::vector< KDungeonRankInfo >>::iterator iterPage = mapDungeonRank.find( nPageNum );
        if ( iterPage == mapDungeonRank.end() )
            return false;

        vecDungeonRank = iterPage->second;

    }
    else
    {
        std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>::iterator iter = m_mapOldDungeonRank.find( nModeID );

        if ( iter == m_mapOldDungeonRank.end() )
            return false;

        std::map<int, std::vector< KDungeonRankInfo >>& mapDungeonRank = iter->second;

        std::map<int, std::vector< KDungeonRankInfo >>::iterator iterPage = mapDungeonRank.find( nPageNum );
        if ( iterPage == mapDungeonRank.end() )
            return false;

        vecDungeonRank = iterPage->second;
    }

    return true;
}