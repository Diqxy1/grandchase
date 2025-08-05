#include "StdAfx.h"
#include ".\kgcdrama.h"

#include "Monster.h"
#include "GCCameraOrtho.h"



#include "../gcui/GCGameOverlayUI.h"


int             KGCDrama::ms_iFrame;
bool            KGCDrama::ms_bDramaEnd;
DramaFunc       KGCDrama::ms_DramaFunc;

KGCDrama::KGCDrama( void )
{
    ms_iFrame = 0;
    ms_bDramaEnd = true;

    ms_mapDrama.clear();
    ms_DramaFunc = NULL;
    
    AddDrama( GC_GM_QUEST6, 1, DramaQuest6_0 );
    AddDrama( GC_GM_QUEST6, 2, DramaQuest6_1 );    
    AddDrama( GC_GM_QUEST7, 1, DramaQuest7_0 );

    AddDrama( GC_GM_QUEST21, 1, DramaQuestNeo2_4 );
    AddDrama( GC_GM_QUEST21, 2, DramaQuestNeo2_5 );
    AddDrama( GC_GM_QUEST21, 3, DramaQuestNeo2_6 );
    AddDrama( GC_GM_QUEST21, 4, DramaQuestNeo2_7 );

	BindLua();
    
}

KGCDrama::~KGCDrama(void)
{
}

bool KGCDrama::Start( int iQuest_, int iDramaIndex_ )
{
    ms_DramaFunc = GetDrama( iQuest_, iDramaIndex_ );
    
    if (ms_DramaFunc)
    {
        ms_iFrame = 0;
        ms_bDramaEnd = false;
        return true;
    }
    else
    {
        ms_DramaFunc = NULL;
        ms_bDramaEnd = true;
        return false;
    }
}

void KGCDrama::FrameMove()
{
    if ( ms_DramaFunc )
    {
        ++ms_iFrame;
        if ( !ms_DramaFunc() )
        {
			End();
        }        
    }

}

void KGCDrama::AddDrama( int iQuest_, int iDramaIndex_, DramaFunc Drama_ )
{
    ms_mapDrama[ std::make_pair( iQuest_, iDramaIndex_) ] = Drama_ ;

}

DramaFunc KGCDrama::GetDrama( int iQuest_, int iDramaIndex_ )
{
	char strDrama[MAX_PATH];
	sprintf( strDrama, "DungeonDrama%d_%d.lua", iQuest_, iDramaIndex_ );
	if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strDrama, false ) == true )
	{
		return LuaDrama;
	}

    std::map< std::pair< int , int > , DramaFunc >::iterator mit = ms_mapDrama.find( std::make_pair( iQuest_, iDramaIndex_) );

    if ( mit == ms_mapDrama.end() )
    {
        return NULL;
    }

    return mit->second;
}

bool KGCDrama::LuaDrama()
{
	GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), "state, msg = pcall( DungeonDramaFrameMove ); if state == false then print( msg ) end" );
	return true;
}

bool KGCDrama::DramaQuestNeo2_4()
{
	if ( ms_iFrame == 0 )
	{
		g_kCamera.SetTargetMonster( 0 );
	}

	if ( ms_iFrame == 23 )
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(0);
		if( pMonster != NULL ) {
			g_ParticleManager->CreateSequence( "Vanessa2_start01", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.3f );
			g_ParticleManager->CreateSequence( "Vanessa2_start02", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.3f );
			g_ParticleManager->CreateSequence( "Vanessa2_start03", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.3f );
			g_ParticleManager->CreateSequence( "Vanessa2_start04", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.3f );
		}
	}

	if ( ms_iFrame == 25 )
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(0);
		MONSTER* pMonster1 = g_kMonsterManager.GetMonster(1);
		if( pMonster != NULL ) {
			pMonster->EndMonster();
			pMonster->ToggleRender( false );

			pMonster1->ToggleRender( true );
			pMonster1->SetX( pMonster->GetX() );
			pMonster1->SetY( pMonster->GetY() );
			pMonster1->SetChampionProperty( pMonster->GetChampionProperty() );
		}
	}

	if ( ms_iFrame >= 35 )
	{
		g_kCamera.InitCamera();
		return false;
	}

	return true;
}

bool KGCDrama::DramaQuestNeo2_5()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;
	
		if ( pMonster->GetMonsterType() != MON_GODOFCIRCULATION) continue;

		if ( ms_iFrame == 1 ) {
			g_kCamera.SetTargetMonster( 1 );
			g_kCamera.m_fWideMode = 7.0f;
		}

		if ( ms_iFrame == 55 * 1 ) {
			pMonster->SetState( "ATTACK01" );
		}

		if ( pMonster->GetMonsterType() != MON_GODOFCIRCULATION && pMonster->IsLive() ) {
			if ( ms_iFrame == 55 * 2 ) {
				pMonster->Turn();
				pMonster->SetMovement( true );
				pMonster->SetFrameLock( false );
				pMonster->DirectMotionInput( "DOWN_UP01" );
				pMonster->ActionCallBack( "Damage" );
				pMonster->SetSpeedX( -0.06f );
				pMonster->SetSpeedY( 0.06f );
				pMonster->SetHP( 0.0f );  
			}
			if ( ms_iFrame == 55 * 4 ) {		
				pMonster->EndMonster();                               
			}
		}
		if ( ms_iFrame > 55 * 5.5 ) {
			g_kCamera.InitCamera();
			return false;
		}
		return true;
	}

	g_kCamera.InitCamera();
    return false;
}

bool KGCDrama::DramaQuestNeo2_6()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;

		if ( pMonster->GetMonsterType() != MON_AMY || !pMonster->IsLive() ) continue;

		if ( ms_iFrame == 4 )
		{
			g_kCamera.SetTargetMonster( mit->first );
		}

		if ( ms_iFrame == 15 )
		{   
			pMonster->SetState( "ATTACK01" );        
		}

		if ( ms_iFrame >= 55 * 2.5 )
		{
			g_kCamera.InitCamera();         
		}

		if ( ms_iFrame >= 55 * 3 )
		{
			g_kCamera.InitCamera();
			return false;
		}

		return true;
	}

	return true;
}

bool KGCDrama::DramaQuestNeo2_7()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;

		if ( pMonster->GetMonsterType() != MON_AMY || !pMonster->IsLive() ) continue;

		if ( ms_iFrame == 5 )
		{
			g_kCamera.SetTargetMonster( mit->first );
			g_kCamera.m_fWideMode = 15.0f;
		}


		if ( ms_iFrame == 15 )
		{   
			pMonster->SetState( "ATTACK02" );
		}

		if ( ms_iFrame == 55 * 3 )
		{
			for ( int i = 0 ; i < MAX_DUNGEON_PLAYER_NUM ; i++ )
			{
				if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
				{
					g_MyD3D->MyPlayer[i]->Heal_To_Player( 12 );                
				}

			}
		}

		if ( ms_iFrame == 55 * 5 )
		{
			pMonster->SetState( "WALK" );
			g_kCamera.InitCamera();
			g_kCamera.m_fWideMode = 10.0f;
		}


		if ( ms_iFrame == 55 * 4.5 )
		{
			pMonster->SetState( "WALK" );
			g_kCamera.InitCamera();
			g_kCamera.m_fWideMode = 10.0f;
		}

		if ( ms_iFrame >= 55 * 6 )
		{
			pMonster->EndMonster();        
		}

		if ( ms_iFrame >= 55 * 6.5 )
		{
			g_kCamera.InitCamera();
			return false;
		}

		return true;

	}

	return true;
}

bool KGCDrama::DramaQuest6_0()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;

		if( ( pMonster->GetMonsterType() != MON_RYAN_WOLF && pMonster->GetMonsterType() != MON_RYAN ) ||
			!pMonster->IsLive() || pMonster->IsDie() || g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) ) continue;

		if ( ms_iFrame == 5 )
		{
			g_kCamera.SetTargetMonster( mit->first );
		}
		
		if ( ms_iFrame == 55 )
		{
			g_ParticleManager->CreateSequence( "change_to_lyan01", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.5f,0.5f  );
			g_ParticleManager->CreateSequence( "change_to_lyan02", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.5f,0.5f  );
			g_ParticleManager->CreateSequence( "change_to_lyan03", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.5f,0.5f  );
			g_ParticleManager->CreateSequence( "change_to_lyan04", pMonster->GetX() - 1.0f,pMonster->GetY() - 0.5f,0.5f  );
			g_ParticleManager->CreateSequence( "eyelight",         pMonster->GetX() - 1.0f,pMonster->GetY() - 0.3f,0.5f  );
		}

		if ( ms_iFrame == 57 )
		{
			pMonster->ToggleRender( false );
			pMonster->m_bLive = false;
			pMonster->Wait_Room( MON_RYAN, pMonster->GetX(),pMonster->GetY(), pMonster->GetIsRight(), true, pMonster->m_iLevel, 0, false );
			pMonster->m_bLive = true;
			pMonster->ToggleRender( true );       
			pMonster->SetMovement( false );
			pMonster->SetInvincible( true );        

		}

		if ( ms_iFrame >= 55 * 3 )
		{
			g_kCamera.InitCamera();
			return false;
		}

		return true;
	}

	g_kCamera.InitCamera();
	return false;
}


bool KGCDrama::DramaQuest7_0()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;

		if (pMonster->GetMonsterType() == MON_RONAN && pMonster->IsLive() && !pMonster->IsDie() )
		{
			if ( ms_iFrame == 5 )
			{
				g_kCamera.SetTargetMonster( mit->first );        
			}

			if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
			{
				if ( ms_iFrame == 60 )
				{   
					pMonster->SetState( "ATTACK01" );
				}

				if ( ms_iFrame == 55 * 4 )
				{
					pMonster->SetState( "ATTACK02" );             
				}
			}    

			if ( ms_iFrame == 55 * 2 )
			{        
				for ( int i = 0 ; i < MAX_DUNGEON_PLAYER_NUM ; i++ )
				{
					if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
					{
						g_MyD3D->MyPlayer[i]->Heal_To_Player( 14 );                
					}
				}
			}

			if ( ms_iFrame >= 55 * 6.5 )
			{
				g_kCamera.InitCamera();
				pMonster->EndMonster();
				return false;
			}

			return true;
		}
	}

	g_kCamera.InitCamera();
    return false;
}

bool KGCDrama::DramaQuest6_1()
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;

		if ( pMonster == NULL ) continue;

		if( ( pMonster->GetMonsterType() == MON_RYAN ) &&
			pMonster->IsLive() || g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
		{
			pMonster->ToggleRender( false );
			pMonster->EndMonster();
			break;
		}
	}

	g_kCamera.InitCamera();
    return false;
}

void KGCDrama::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCDrama>( L, "KGCDramaClass" );
    lua_tinker::class_def<KGCDrama>( L,  "GetFrame", 		&KGCDrama::GetFrame  );
    lua_tinker::class_def<KGCDrama>( L,  "CreateParticleToMonster",  &KGCDrama::CreateParticleToMonster  );
    lua_tinker::class_def<KGCDrama>( L,  "ChangeMonster", 	&KGCDrama::ChangeMonster  );
    lua_tinker::class_def<KGCDrama>( L,  "EndDrama", 		&KGCDrama::End  );
    lua_tinker::class_def<KGCDrama>( L,  "FindMonster", 	&KGCDrama::FindMonster  );
    lua_tinker::class_def<KGCDrama>( L,  "SetDialogue", 	&KGCDrama::SetDialogue  );
    lua_tinker::class_def<KGCDrama>( L,  "GetPos", 			&KGCDrama::GetPos  );;

    lua_tinker::decl( L, "KGCDrama", this );

    GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "DungeonDramaFunc.lua", true );
}

int KGCDrama::GetFrame()
{
	return ms_iFrame;
}

void KGCDrama::CreateParticleToMonster( char* strName, float offetX, float offsetY )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(0);
	if( pMonster != NULL )
		g_ParticleManager->CreateSequence( strName, pMonster->GetX() + offetX,pMonster->GetY() + offsetY );
}

void KGCDrama::ChangeMonster( int fromIndex, int toIndex, int iMonType, bool bBoss /*= false*/ )
{
	MONSTER* from = g_kMonsterManager.GetMonster(fromIndex);
	MONSTER* to = g_kMonsterManager.GetMonster( toIndex );

	if( from != NULL && to != NULL )
		to->Wait_Room( iMonType, from->GetX(), from->GetY(), from->GetIsRight(), true, from->GetLevel(), from->GetChampionProperty(), bBoss );
}

D3DXVECTOR2 KGCDrama::GetPos( int iMonsterIndex )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
	if( pMonster == NULL )
		return D3DXVECTOR2(0.0f,0.0f);

	return D3DXVECTOR2(pMonster->GetX(),pMonster->GetY());
}

void KGCDrama::End()
{
	ms_bDramaEnd = true;
	ms_DramaFunc = NULL;
}

MONSTER* KGCDrama::FindMonster( int iMonsterType )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if ( pMonster == NULL) continue;

		if ( pMonster->GetMonsterType() == iMonsterType && pMonster->IsLive() )
		{
			return pMonster;
		}
	}

	return NULL;
}

void KGCDrama::SetDialogue( bool bRight, int iSpeeker, int iSpeech )
{
	g_pkGameOverlayUI->SetDialogue( bRight, iSpeeker, iSpeech );	
}