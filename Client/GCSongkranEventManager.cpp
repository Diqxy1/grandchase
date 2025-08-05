#include "GCSongkranEventManager.h"

const int nFRAME_COUNT = 55;

ImplementSingleton( GCSongkranEventManager )

GCSongkranEventManager::GCSongkranEventManager(void)
: m_bGetInfo( false )
, m_fPlayerCharMoveSpeed( 0.0f )
, m_fWaterBombFlyingSpeed( 0.0f )
, m_fWaterBombRange( 0.0f )
, m_dwScore( 0 )
, m_dwGrade( 0 )
, m_bReqReward( false )
, m_bLastSupplyNot( false )
, m_dwSupplyDuration( 0 )
, m_dwDailySupplyDuration( 0 )
{
	m_mapTargetInfoList.clear();
	m_mapGradeRewardList.clear();
}

GCSongkranEventManager::~GCSongkranEventManager(void)
{
}

void GCSongkranEventManager::Reset()
{
	m_bGetInfo = false;

	m_fPlayerCharMoveSpeed = 0.0f;
	m_fWaterBombFlyingSpeed = 0.0f;
	m_fWaterBombRange = 0.0f;
	m_dwScore = 0;
	m_dwGrade = 0;
    m_bReqReward = false;
	m_bLastSupplyNot = false;

	m_dwSupplyDuration = 0;
	m_dwDailySupplyDuration = 0;


	m_iSyncPlayTime = 0;

	m_nAccTime = 0;
	m_nChargeTickTime = 0;
	
	m_mapTargetInfoList.clear();
	m_mapGradeRewardList.clear();

}

void GCSongkranEventManager::SetInfoData( KEVENT_SONGKRAN_SCRIPT_INFO_ACK kRecv )
{
	m_bGetInfo = true;	

	m_fPlayerCharMoveSpeed = kRecv.m_fPlayerCharMoveSpeed;
	m_fWaterBombFlyingSpeed = kRecv.m_fWaterBombFlyingSpeed; 
	m_fWaterBombRange = kRecv.m_fWaterBombRange;

	m_mapTargetInfoList = kRecv.m_mapTargetInfoList;	
	m_mapGradeRewardList = kRecv.m_mapGradeRewardList;

	m_dwSupplyDuration = kRecv.m_dwSupplyDuration;
	m_dwDailySupplyDuration = kRecv.m_dwDailySupplyDuration;

	m_nChargeTickTime = kRecv.m_nSupplyInterval;

}

void GCSongkranEventManager::SetUserInfo( KEVENT_SONGKRAN_USER_INFO_ACK kRecv )
{
	m_dwScore = kRecv.m_dwScore;
	m_dwGrade = kRecv.m_dwGrade;
	m_dwWaterBombDuration = kRecv.m_dwWaterBombDuration;
	m_bLastSupplyNot = kRecv.m_bFinish;

	SetRemainTime( kRecv.m_nAccTime );
}

bool GCSongkranEventManager::IsEventOpen()
{
	for ( int i = 0; i < (int)g_kGlobalValue.m_vecEventOpenInfo.size(); ++i ) 
	{
		DWORD dwEventMBoxID = g_kGlobalValue.m_vecEventOpenInfo[i].m_dwMBoxID;

		if ( dwEventMBoxID == KGCUIScene::GC_MBOX_SONGKRAN_DLG ) 
			return true;		
	}

	return false;
}

void GCSongkranEventManager::SetRemainTime( int nTime )
{
	m_nAccTime = nTime;

	m_iSyncPlayTime = (g_kGlobalValue.m_iPlayTime + static_cast<int>(m_nChargeTickTime) - m_nAccTime ) % static_cast<int>(m_nChargeTickTime) ;
}

int GCSongkranEventManager::GetRemainTime()
{
	int nViewTime = (g_kGlobalValue.m_iPlayTime - m_iSyncPlayTime) * 60 + ( timeGetTime() - g_kGlobalValue.m_dwAccTimeNot ) / 1000;

	nViewTime = (m_nChargeTickTime*60) - ( abs( nViewTime % (m_nChargeTickTime*60 )) ); 

	
	if ( m_bLastSupplyNot ) 
		nViewTime = 0;

	return nViewTime;
}

KSongkranEventTargetInfo* GCSongkranEventManager::GetTargetInfo( int nChar )
{	
	if ( nChar < CHAR_ELESIS || nChar >= CHAR_SIZE )
		return NULL;

	std::map< USHORT, KSongkranEventTargetInfo >::iterator pos = m_mapTargetInfoList.find( nChar );

	if ( pos == m_mapTargetInfoList.end() )
		return NULL;

	return &pos->second;
}

DWORD GCSongkranEventManager::GetTargetScore( int nChar )
{
	KSongkranEventTargetInfo* pTarget = GetTargetInfo( nChar );
	
	if ( !pTarget ) 
		return 0;

	return pTarget->m_dwTargetScore;
}

float GCSongkranEventManager::GetTargetSpeed( int nChar )
{
	KSongkranEventTargetInfo* pTarget = GetTargetInfo( nChar );

	if ( !pTarget ) 
		return 0.0f;

	return pTarget->m_fTargetMoveSpeed;
}

int	GCSongkranEventManager::GetTeleportFrame( int nChar )
{
	KSongkranEventTargetInfo* pTarget = GetTargetInfo( nChar );

	if ( !pTarget ) 
		return nFRAME_COUNT;

	return static_cast<int>( pTarget->m_fTeleportPeriod * nFRAME_COUNT );
}

int GCSongkranEventManager::GetResetFrame( int nChar )
{
	KSongkranEventTargetInfo* pTarget = GetTargetInfo( nChar );

	if ( !pTarget ) 
		return nFRAME_COUNT;

	return static_cast<int>( pTarget->m_fHitTargetAfterResetTime * nFRAME_COUNT );
}

int GCSongkranEventManager::GetBombCount()
{
	return m_dwWaterBombDuration;
}

KSongkranEventRewardInfo* GCSongkranEventManager::GetRewardInfo( DWORD dwGrade )
{
	std::map< DWORD, KSongkranEventRewardInfo >::iterator pos = m_mapGradeRewardList.find ( dwGrade );

	if ( pos == m_mapGradeRewardList.end() )
		return NULL;

	return &pos->second;
}

bool GCSongkranEventManager::IsHaveReward()
{
	KSongkranEventRewardInfo* pRewardInfo = GetRewardInfo( m_dwGrade );

	if ( !pRewardInfo ) 
		return false;

	if ( pRewardInfo->m_dwGradeEndScore <= m_dwScore ) 
		return true;

	return false;
}

GCITEMID GCSongkranEventManager::GetRewardItem()
{
	KSongkranEventRewardInfo* pRewardInfo = GetRewardInfo( m_dwGrade );

	if ( !pRewardInfo ) 
		return false;

	if ( pRewardInfo->m_vecReward.empty() ) 
		return 0;

	KDropItemInfo kDrop = *(pRewardInfo->m_vecReward.begin());

	return 	kDrop.m_ItemID;
}

DWORD GCSongkranEventManager::GetEndPoint()
{
	KSongkranEventRewardInfo* pRewardInfo = GetRewardInfo( m_dwGrade );

	if ( !pRewardInfo ) 
		return 0;
	
	return 	pRewardInfo->m_dwGradeEndScore;

}

KSongkranEventRewardInfo* GCSongkranEventManager::GetSpecialRewardItem( DWORD nSeq )
{
	int nCount = 0;

	std::map< DWORD, KSongkranEventRewardInfo >::iterator pos = m_mapGradeRewardList.find ( m_dwGrade );

	while( pos != m_mapGradeRewardList.end() )
	{
		KSongkranEventRewardInfo& kSongkran = pos->second;
		
		if ( kSongkran.m_bSpecialReward ) 
		{
			if ( nCount == nSeq ) 
			{
				return &pos->second;
			}
			else
			{
				nCount++;
			}
		}

		pos++;
	}

	return NULL;
}

float GCSongkranEventManager::GetRewardRate()
{
	KSongkranEventRewardInfo* pRewardInfo1 = GetRewardInfo( m_dwGrade - 1);
	KSongkranEventRewardInfo* pRewardInfo2 = GetRewardInfo( m_dwGrade );

	int nBePoint = 0;
	int nAfPoint = 0;

	if ( !pRewardInfo1 ) 
		nBePoint = 0;
	else
		nBePoint = pRewardInfo1->m_dwGradeEndScore;
	
	if ( !pRewardInfo2 ) 
		return 0.0f;
	else
		nAfPoint = pRewardInfo2->m_dwGradeEndScore;
	
	int nMaxPoint = nAfPoint - nBePoint;
	
	float fRate = static_cast<float>(m_dwScore-nBePoint) / static_cast<float>(nMaxPoint);

	if( fRate > 1.0f )         fRate = 1.0f;
	else if( fRate < 0.0f )    fRate = 0.0f;

	return fRate;
}

void GCSongkranEventManager::ReqRewardItem()
{
	if ( m_bReqReward ) 
		return;

	KP2P::GetInstance()->Send_SongkranEventGradeRewardReq();
	m_bReqReward = true;
}