#include "StdAfx.h"
#include "KGCValentineDayManager.h"

#include "../Square/GCSquarePeople.h"
#include "../Square/GCSquare.h"

ImplementSingleton(KGCValentineDayManager);


KGCValentineDayManager::KGCValentineDayManager(void)
:m_dwPollItemID(0)
,m_bPollState(false)
,m_dwPollItemCurrentCount(0)
,m_eSeletedCharType(GC_CHAR_INVALID)
,m_bPollingNow(FALSE)
,m_strSelectedCharString(L"")
{

}

KGCValentineDayManager::~KGCValentineDayManager(void)
{
}


void KGCValentineDayManager::NearChocolateEffect( PLAYER* pPlayer )
{
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ||
		g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
		return;

	DWORD dwMyUID = pPlayer->m_kUserInfo.dwUID;
	if( pPlayer->m_kUserInfo.bLive == false || PS_DEAD == pPlayer->m_ePlayerState)
	{
		DeleteNearChocolateEffect( dwMyUID );
		return;
	}

	KUserItemVector::iterator itor = std::find_if( pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.begin(), pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.end(), boost::bind( &SUserItem::iItemID, _1 ) == VALENTINE_DAY_CHOCOLATE_ACCESSORY);
	if(itor == pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.end() )
	{
        itor = std::find_if( pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.begin(), pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.end(), boost::bind( &SUserItem::iItemID, _1 ) == VALENTINE_DAY_CHOCOLATE_ACCESSORY1);
        if(itor == pPlayer->m_kUserInfo.GetCurrentChar().vecItemInfo.end() )
        {
	        DeleteNearChocolateEffect( dwMyUID );
	        return;
        }
	}

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{	
		if( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false ) ||
			( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwMyUID ) )
			continue;

		itor = std::find_if( g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.begin(), 
							g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.end(), 
                            boost::bind( &SUserItem::iItemID, _1 ) == VALENTINE_DAY_CHOCOLATE_ACCESSORY );
    
        if(itor == g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.end() ){
            itor = std::find_if( g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.begin(), 
                g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.end(), 
                boost::bind( &SUserItem::iItemID, _1 ) == VALENTINE_DAY_CHOCOLATE_ACCESSORY1 );

            if(itor == g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().vecItemInfo.end() )
                continue;
        }
		D3DXVECTOR3 vMyPos = pPlayer->GetPosition();
		D3DXVECTOR3 vPartnerPos = g_MyD3D->MyPlayer[i]->GetPosition();

		if( ASMfabs(vMyPos.x - vPartnerPos.x) > 0.5f ||
			ASMfabs(vMyPos.y - vPartnerPos.y) > 0.5f)
			continue;
		
		if( m_mapParticle.find( dwMyUID ) == m_mapParticle.end() )
		{
            MSG_PARTICE_CREATE_INFO msgParticle("valentine_chocolate_heart_1",0.0f,0.0f,0,0.0f,true,5.0f);
            m_mapParticle[dwMyUID].insert( pPlayer->CreateParticle( msgParticle ) );
            msgParticle.strSeqname = "valentine_chocolate_heart_2";
            m_mapParticle[dwMyUID].insert( pPlayer->CreateParticle( msgParticle ) );
            msgParticle.strSeqname = "valentine_chocolate_heart_3";
            m_mapParticle[dwMyUID].insert( pPlayer->CreateParticle( msgParticle ) );
            msgParticle.strSeqname = "valentine_chocolate_heart_4";
            m_mapParticle[dwMyUID].insert( pPlayer->CreateParticle( msgParticle ) );				
		}

		return;
	}

	std::map< DWORD, std::set< CParticleEventSeqPTR> >::iterator mit =  m_mapParticle.find( dwMyUID );

	if(mit != m_mapParticle.end() )
	{
		std::set< CParticleEventSeqPTR>::iterator sit = (*mit).second.begin();

		for(sit = (*mit).second.begin(); sit != (*mit).second.end();)
		{
			CParticleEventSeqPTR sit2 = (*sit);

			if( g_ParticleManager->IsLiveInstance(sit2) && sit2->GetKillTime() != 0)
			{
				++sit;
			}
			else
			{
				g_ParticleManager->DeleteSequence(sit2);
				sit = (*mit).second.erase(sit);			
			}
		}

		if( (*mit).second.empty())
		{
			m_mapParticle.erase(mit);
		}
	}

	//DeleteNearChocolateEffect( dwMyUID );
}

void KGCValentineDayManager::NearChocolateEffect( GCSquarePeople* pPlayer )
{
	DWORD dwMyUID = pPlayer->GetUID();

	std::vector<DWORD>::const_iterator itor = std::find( pPlayer->GetPeopleInfo().m_vecEquips.begin(), pPlayer->GetPeopleInfo().m_vecEquips.end(), VALENTINE_DAY_CHOCOLATE_ACCESSORY * 10);

	if(itor ==  pPlayer->GetPeopleInfo().m_vecEquips.end() )
	{

        itor = std::find( pPlayer->GetPeopleInfo().m_vecEquips.begin(), pPlayer->GetPeopleInfo().m_vecEquips.end(), VALENTINE_DAY_CHOCOLATE_ACCESSORY1 * 10);
        if(itor ==  pPlayer->GetPeopleInfo().m_vecEquips.end() )
        {
            DeleteNearChocolateEffect( dwMyUID );
            return;
        }
	}

	std::vector< DWORD > vecNearPeople = SiGCSquare()->GetNearPeopleUIDVector();

	std::vector< DWORD >::iterator vit;

	for(vit = vecNearPeople.begin(); vit != vecNearPeople.end(); ++vit)
	{
		if( (*vit) == dwMyUID )
			continue;

		GCSquarePeople* pSquarePeople = SiGCSquare()->GetSquarePeople( (*vit));

		if(NULL == pSquarePeople )
			continue;

		std::vector<DWORD>::const_iterator itor2 = std::find( pSquarePeople->GetPeopleInfo().m_vecEquips.begin(), pSquarePeople->GetPeopleInfo().m_vecEquips.end(), VALENTINE_DAY_CHOCOLATE_ACCESSORY * 10);

        if(pSquarePeople->GetPeopleInfo().m_vecEquips.end() == itor2 ){
            itor2 = std::find( pSquarePeople->GetPeopleInfo().m_vecEquips.begin(), pSquarePeople->GetPeopleInfo().m_vecEquips.end(), VALENTINE_DAY_CHOCOLATE_ACCESSORY1 * 10);
            if(pSquarePeople->GetPeopleInfo().m_vecEquips.end() == itor2 ){
    			continue;
            }
        }

		D3DXVECTOR3 vMyPos = pPlayer->GetPos();
		D3DXVECTOR3 vSquarePos = pSquarePeople->GetPos();

		if( ASMfabs(vMyPos.x - vSquarePos.x) > 0.5f ||
			ASMfabs(vMyPos.y - vSquarePos.y) > 0.5f)
			continue;

		if( m_mapParticle.find( dwMyUID ) == m_mapParticle.end() )
		{
			m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "valentine_chocolate_heart_1", 0, 0.0f ) );
			m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "valentine_chocolate_heart_2", 0, 0.0f ) );
			m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "valentine_chocolate_heart_3", 0, 0.0f ) );
			m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "valentine_chocolate_heart_4", 0, 0.0f ) );
		}
		return;
	}

	//DeleteNearChocolateEffect( dwMyUID );

	std::map< DWORD, std::set< CParticleEventSeqPTR> >::iterator mit =  m_mapParticle.find( dwMyUID );

	if(mit != m_mapParticle.end() )
	{
		std::set< CParticleEventSeqPTR>::iterator sit = (*mit).second.begin();

		for(sit = (*mit).second.begin(); sit != (*mit).second.end();)
		{
			CParticleEventSeqPTR si2 = (CParticleEventSeqPTR)*sit;

			if( g_ParticleManager->IsLiveInstance(si2) && si2->GetKillTime() != 0)
			{
				++sit;
			}
			else
			{
				g_ParticleManager->DeleteSequence((*sit));
				sit = (*mit).second.erase(sit);			
			}
		}

		if( (*mit).second.empty())
		{
			m_mapParticle.erase(mit);
		}
	}
}

void KGCValentineDayManager::DeleteNearChocolateEffect( DWORD dwUID  )
{
	if( m_mapParticle.find( dwUID ) != m_mapParticle.end() )
	{
		std::set< CParticleEventSeqPTR>::iterator sit;
		for( sit = m_mapParticle[dwUID].begin() ; sit != m_mapParticle[dwUID].end() ; ++sit )
		{
			g_ParticleManager->DeleteSequence( (*sit) );
		}
		m_mapParticle.erase( dwUID );
	}
}
