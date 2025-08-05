#include "stdafx.h"
// KGCPlayerCommon.cpp: implementation of the KGCPlayerCommon class.
//
//////////////////////////////////////////////////////////////////////
//


#include "KDInput.h"


#include "KGCPlayerCommon.h"
#include "Controls.h"
//
#include "Stage.h"
#include "GCCameraOrtho.h"
//
//

#include "DamageManager.h"
//
//
#include "KGCRoomManager.h"
#include "GCPortal.h"

#include "Monster.h"

#include "KGCPlayerElesis.h"
#include "KGCPlayerElesis2.h"
#include "KGCPlayerElesis3.h"
#include "KGCPlayerElesis4.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/KGCDotaShop.h"
#include "GCEventManager.h"

#include "Message.h"

#include "GCStateGame.h"

unsigned char   g_Push_Count    = 0;
unsigned char   g_Max_Push	    = 0;
int             g_Hot_Count     = 0;
std::set<std::string> KGCPlayerCommon::ms_setLoadedTemplate;
KLuaManager KGCPlayerCommon::ms_kLuaMgr_PlayerTemplate;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerCommon::KGCPlayerCommon()
{

}

KGCPlayerCommon::~KGCPlayerCommon()
{

}

bool KGCPlayerCommon::CheckPortal(PLAYER* pPlayer, bool bKeyProc)
{	
	if( SiKGCRoomManager()->IsMonsterGameMode() == false )
	{

		if(pPlayer->IsContact && pPlayer->ContactBlock>4 && pPlayer->ContactBlock<9)
		{
			pPlayer->SetPlayerState(MID_COMMON_INTO_THE_TELEPORT);
			pPlayer->cFrame=0;
			pPlayer->y_Speed = 0;	
			pPlayer->x_Speed = 0;
			return true;
		}
	}

	if(g_MyD3D->MyCtrl->k_Fresh_Down && pPlayer->GetOwnMotion(MID_COMMON_INTO_TELEPORT)!=pPlayer->uiMotion)
	{	
		if(pPlayer->IsContact && KGCPortal::CheckPortal(pPlayer) )
		{
			pPlayer->SetPlayerState(MID_COMMON_INTO_TELEPORT);
			pPlayer->cFrame=0;
			pPlayer->NoCheckContact = 0;
			pPlayer->y_Speed = 0;	
			pPlayer->x_Speed = 0;		

			return true;
		}
		else if(pPlayer->IsContact && KGCTeleporter::CheckTeleporter(pPlayer))
		{				
			pPlayer->SetPlayerState(MID_COMMON_INTO_TELEPORT);
			pPlayer->cFrame=0;
			pPlayer->NoCheckContact = 0;
			pPlayer->y_Speed = 0;	
			pPlayer->x_Speed = 0;	
			return true;
		}		
	}
	return false;
}

// �̰� ��Ÿ��ƽ �ʿ��� ���� MID_COMMON_INTO_TELEPORT
int KGCPlayerCommon::OnIntoPortal( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
		return 0;

    // ���Ӹ�尡 ��Ÿ�����϶��� ��Ÿ�ӵ��� ��Ż�̿��� �����Ѵ�.
    if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
    {
        KGCGameModeInterface* pMode = g_pStateGame->GetCurGameModePtr();
        if( pMode && 
            0 < pMode->GetArenaCoolTime() ||
            0 < pMode->GetGameCoolTime() )
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
            return 0;
        }
    }

	if( pPlayer->cFrame == 69)
	{			
		KGCTeleporter *next_teleporter = KGCTeleporter::GetNextTeleporter(KGCTeleporter::CheckTeleporter(pPlayer));
		if(pPlayer->IsContact && next_teleporter)
		{
			pPlayer->vPos.x = next_teleporter->GetPosX()-0.15f;
			pPlayer->vPos.y = next_teleporter->GetPosY()+0.1f;
			pPlayer->x_Speed=0.00001f;
			pPlayer->y_Speed=0.00001f;

            // ���Ӹ�尡 ��Ÿ�����϶��� �ʿ���.( �̵���Ż ���� ī�޶� ������ ���󰡴°� �ƴ϶� ��! ���󰡰� ����������Żó�� �̵��Ǳ���.. )
            if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
            {
                // ��Ÿ���� ���� ����������!!
                if( NULL != g_pkUIScene->m_pkDotaShop )
                    g_pkUIScene->m_pkDotaShop->CloseUI();

                pPlayer->SetSuper( "", 0, 0, 55, true );
                pPlayer->SetDotaStage( next_teleporter->GetDotaStage() );
                g_pkUIScene->m_pkGameOverlayUI->UpdateDotaMiniMap();
                g_kCamera.SetDirectCameraPosition(pPlayer->vPos.x, pPlayer->vPos.y, KGCCameraOrtho::POS_MODE_DIRECT);

                // ��ȯ�� ������ ��ġ�� �θ���ġ�� ���� ���ش�.
				for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
					if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) == false )
						continue;
					DWORD dwUID = mit->second->GetSummonPlayerUID();
					PLAYER* pSummonPlayer = g_MyD3D->GetPlayerByUID( dwUID );
					if( pSummonPlayer && dwUID == pPlayer->m_kUserInfo.dwUID )
					{
						mit->second->SetX( pPlayer->vPos.x );
						mit->second->SetY( pPlayer->vPos.y );
					}

				}


                // �� ��ġ�� ������ �˸���!!(�츮��)
                KGC_PID_BROAD_DOTA_ZONE_POSITION kPacket( GC_PID_BROAD_DOTA_ZONE_POSITION, 
                    pPlayer->m_kUserInfo.dwUID, pPlayer->m_iPlayerIndex, pPlayer->GetDotaStage(), true, D3DXVECTOR2(pPlayer->vPos.x, pPlayer->vPos.y) );
                SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
                KGCPC_MEMO( "GC_PID_BROAD_DOTA_ZONE_POSITION" );

                // ó�� ��Ż�̵��� �ߴٸ� ���̵� �޽����� ���������!!
                g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_IN_ZONE );
            }
			return 0;
		}
		else if(pPlayer->IsContact)
		{			
			KGCPortal * portal = KGCPortal::CheckPortal(pPlayer);
			if(portal)
			{
				pPlayer->ProcessMetamorphosisNormalForm( false );				
				pPlayer->vPos.x = portal->GetPosX()-0.15f;
				pPlayer->x_Speed=0.00001f;
				pPlayer->IsContact = true;
				pPlayer->SetPlayerState(MID_COMMON_WAIT_TELEPORT);				
				return 0;
			}
		}
		
		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);		
	}
	return 0;
}

// �̰� ���� �ʿ��� ���� MID_COMMON_INTO_THE_TELEPORT
int KGCPlayerCommon::OnPortal(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
		return 0;

	if ( SiKGCRoomManager()->IsMonsterGameMode() == false )
	{
	
		if( pPlayer->cFrame == 69 && 0 <= pPlayer->m_cTargetPortalBlock && pPlayer->m_cTargetPortalBlock <= 4)
		{
			pPlayer->vPos.x = g_MyD3D->MyStg->PortalPosition[pPlayer->m_cTargetPortalBlock].x;
			pPlayer->vPos.y = g_MyD3D->MyStg->PortalPosition[pPlayer->m_cTargetPortalBlock].y;
			pPlayer->m_cTargetPortalBlock = 0;
		}
	}
	return 0;
}

int KGCPlayerCommon::OnWaitPortal(PLAYER* pPlayer, bool bKeyProc)
{	
	if( bKeyProc )
		return 0;	
	
	if(pPlayer->cFrame>69)
	{
		if( true == g_MyD3D->MyCtrl->k_Fresh_Down && !KGCPortal::CheckOpen())
		{
            if( INT_MAX > pPlayer->GetEmbarkTypeOfInPortal() )
            {
                KGC_PID_BROAD_EMBARK_SUMMON_ACK kPacket;
                kPacket.dwPlayerUID = pPlayer->GetPlayerUID();
                kPacket.dwEmbarkUID = pPlayer->GetEmbarkUIDOfInPortal();
                kPacket.iEmbarkType = pPlayer->GetEmbarkTypeOfInPortal();
                kPacket.fPosX = pPlayer->vPos.x;
                kPacket.fPosY = pPlayer->vPos.y;
                kPacket.bIsRight = pPlayer->GetIsRight();
                kPacket.bEmbakation = true;
                SiKGCEmbarkManager()->SummonEmbark( &kPacket );
                SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
                KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_SUMMON_ACK" );

                pPlayer->SetEmbarkTypeOfInPortal( INT_MAX );
                pPlayer->SetEmbarkUIDOfInPortal( INT_MAX );
                return 0;
            }
			pPlayer->SetPlayerState(MID_COMMON_INTO_TELEPORT);
			pPlayer->cFrame=70;
		}

		pPlayer->m_bFrameLock=true;	
	}

	return 0;
}

int KGCPlayerCommon::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if ( !pPlayer ) 
        return 0;

    if(bKeyProc)
    {
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPlayer->bIsRight = true;
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPlayer->bIsRight = false;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
            if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( pPlayer->GetCurFormTemplate().JUMP_DASH != -1 )
        {       
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
        {
            pPlayer->Jump();
            g_MyD3D->MyCtrl->k_Up = false;
            pPlayer->m_iJumpPossibleFrame = 0 ;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            return KGCPlayerCommon::DoubleJumpFunc( pPlayer );			            
        }
        
        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f )
    {
        //�������� ���� ����
        if( pPlayer->cFrame > 20 )
            pPlayer->cFrame = 20;
    }
    else 
    {
        if( pPlayer->cFrame < 20 )
            pPlayer->cFrame = 20;
        //���� �ݺ��ϴ� ������..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerCommon::OnEmptyEndFrameLock(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
        return 0;

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //���� �ݺ��ϴ� ������..
        pPlayer->m_bFrameLock = true;

    return 0;
}

//�������ٰ� �Ͼ�� ...
int KGCPlayerCommon::OnDownAndStandUp(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //���� ������ �ϴ� ���� -> Tiger Rolling Attack�� �ߵ��� �� ����
        // MID_COMMON_FRONTDOWN_AND_STANDUP ������ �׶���ٿ� ������ �ߵ��� �� �־����
        if( ( 0.0f != pPlayer->m_fAbility[ABILITY_ROLLING] ) && pPlayer->IsContact && pPlayer->GetHP() > 0 && pPlayer->cFrame >= 10 && pPlayer->cFrame <= 35 )
            //pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH != -1 && )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
            {
                pPlayer->bIsRight = true;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;                 
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
            {
                pPlayer->bIsRight = false;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
        }

        if( pPlayer->GetCurFormTemplate().WAKE != -1 && pPlayer->GetHP() != 0.0f)
        {           
            if(pPlayer->cFrame > 10 && pPlayer->cFrame < 24 && g_MyD3D->MyCtrl->k_Fresh_Punch)
            {
                pPlayer->m_bReserveWake = true;
                return 0;
            }
            if(pPlayer->cFrame == 24 && pPlayer->m_bReserveWake)
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAKE);
                pPlayer->cFrame = 0;
                pPlayer->m_bReserveWake = false;
            }
        }

        if( pPlayer->GetCurFormTemplate().DOWN_STAND_UP_EVASION != -1 && 
            pPlayer->IsContact && 
            pPlayer->GetHP() > 0 && 
            pPlayer->cFrame >= 20 && pPlayer->cFrame <= 35) {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
            {
                pPlayer->bIsRight = false;
                pPlayer->cFrame = 0;                 
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOWN_STAND_UP_EVASION);
                return 0;
            }
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
            {
                pPlayer->bIsRight = true;
                pPlayer->cFrame = 0;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOWN_STAND_UP_EVASION);
                return 0;
            }
        }
    }
    if( pPlayer->cFrame == 10 )
    {
        pPlayer->m_bReserveWake = false;
    }
    return 0;
}

int KGCPlayerCommon::OnEmptyCancel(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            g_MyD3D->MyCtrl->k_Fresh_Left_Dash = g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
        }
        return 0;
    }

    return OnEmpty(pPlayer,bKeyProc);
}

//������ �������ٰ� �Ͼ�� ... WAKE�� �ȵȴٴ� �������� ����
int KGCPlayerCommon::OnFrontDownAndStandUp(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //���� ������ �ϴ� ���� -> Tiger Rolling Attack�� �ߵ��� �� ����
        // MID_COMMON_FRONTDOWN_AND_STANDUP ������ �׶���ٿ� ������ �ߵ��� �� �־����
        if( ( 0.0f != pPlayer->m_fAbility[ABILITY_ROLLING] ) && pPlayer->IsContact && pPlayer->GetHP() > 0 && pPlayer->cFrame > 14 && pPlayer->cFrame < 29 )
            //pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH != -1 && )
        {
            if(  g_MyD3D->MyCtrl->k_Fresh_Right_Dash)
            {
                pPlayer->bIsRight = true;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
            {
                pPlayer->bIsRight = false;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
        }
    }
    return 0;
}

int KGCPlayerCommon::OnHold( PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Right ) 
		{
			g_Push_Count++;
			g_MyD3D->MyCtrl->k_Fresh_Right = false;
		}   
		if ( g_MyD3D->MyCtrl->k_Fresh_Left )
		{
			g_Push_Count++;
			g_MyD3D->MyCtrl->k_Fresh_Left = false;
		}
		return 0;
	}

    if( pPlayer->m_bIsContinousDamage && pPlayer->Slow_Count == 0)
	{
		if( !o10 )
        {
            PLAYER::DamageInfo damage;
            damage.SetDamage( pPlayer->m_fContinousDamage );
            if ( pPlayer->GetLastKillmePlayer() >= MAX_PLAYER_NUM )
                damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_MONSTER, pPlayer->GetLastKillmePlayer() - MAX_PLAYER_NUM );
            else
                damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_PLAYER, pPlayer->GetLastKillmePlayer() );

			pPlayer->Change_HP( damage );
        }
	}

	if( g_Push_Count >= g_Max_Push )
	{
		pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
		return 0;
	}

	if ( pPlayer->uiMotion == pPlayer->GetOwnMotion( MID_COMMON_HOLD_NOEFFECT ) )
	{
		if ( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 2 )
		{
			pPlayer->cFrame = 0;
		}
	}

	return 0;
 }

int KGCPlayerCommon::OnStoneCurse(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Right ) 
        {
            g_Push_Count++;
            g_MyD3D->MyCtrl->k_Fresh_Right = false;
        }   
        if ( g_MyD3D->MyCtrl->k_Fresh_Left )
        {
            g_Push_Count++;
            g_MyD3D->MyCtrl->k_Fresh_Left = false;
        }
        return 0;
    }   

	if( pPlayer->m_bIsContinousDamage && pPlayer->Slow_Count == 0)
	{
		if( pPlayer->GetLastKillmePlayer() < MAX_PLAYER_NUM )
		{
            if( !o10 ) {
                PLAYER::DamageInfo damage;
                damage.SetDamage( pPlayer->m_fContinousDamage );
                damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_PLAYER, pPlayer->GetLastKillmePlayer() );
				pPlayer->Change_HP( damage );
            }
		}
	}

    // ���� Ŀ���� �Ϸ�Ǿ� �������� ���δ�.
    if( pPlayer->cFrame == 148 && (pPlayer->IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
    {                
        if( pPlayer->GetLastKillmePlayer() < MAX_PLAYER_NUM )
        {
            CDamage* pDamage = g_MyD3D->m_pDamageManager->GetOriginalDamage( pPlayer->m_iStoneCurseType );

#ifdef LEVEL_DESIGN_STAT_CALC
            float fDamage = -(pDamage->m_fStoneMaxDmg - (float)g_Push_Count * pDamage->m_fDecreasePerPush);
#else
            float fDamage = -(pDamage->m_fStoneMaxDmg - (float)g_Push_Count * (pDamage->m_fDecreasePerPush * 0.01f));
#endif
            if( fDamage >= 0.0f )
                fDamage = pDamage->m_fStoneMinDmg;

            PLAYER::DamageInfo damage;
            damage.SetDamage( fDamage );
            damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_PLAYER, pPlayer->GetLastKillmePlayer() );
            pPlayer->Change_HP( damage );
        }
        else // ���Ϳ��� ���� ����Ŀ��
        {
#ifdef LEVEL_DESIGN_STAT_CALC
            float fMaxDamage = 80.f;
            float fPushBonus = 1.5f;
            const float fMinDamage = 20.f;
#else
            float fMaxDamage = 0.8f;
            float fPushBonus = 0.015f;
            const float fMinDamage = 0.2f;
#endif
            float fDamage = -( fMaxDamage - (float)g_Push_Count * fPushBonus );
            if( fDamage >= -fMinDamage )
            {
                fDamage = -fMinDamage;
            }

            fDamage -= fDamage * g_MyD3D->MyPlayer[pPlayer->m_iPlayerIndex]->m_fAbility[ABILITY_ABNORMAL_CONDITION_RESISTANCE];
            PLAYER::DamageInfo damage;
            damage.SetDamage( fDamage );
            damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_MONSTER, pPlayer->GetLastKillmePlayer() - MAX_PLAYER_NUM );
            pPlayer->Change_HP( damage );
        }

		if ( pPlayer->m_iStoneCurseType == DT_SPIDERQUEEN_WEB_BODY )
		{
			pPlayer->Direct_Motion_Input(MID_COMMON_BIGDAMAGE);
			pPlayer->Super=20;
		}
		else
		{
			pPlayer->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP);
			pPlayer->Super=30;
		}
    }

    return 0;   
}

int KGCPlayerCommon::OnBackTeleport(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Up )
		{   
			if( pPlayer->IsContact )
			{           
				if(!pPlayer->bIsRight)
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
				else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Up = false;
				return 0;
			}           
			pPlayer->NoCheckContact = 0;
		}

		if( g_MyD3D->MyCtrl->k_Down )
		{
			g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

			if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
			{
				return 0;
			}

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}


		if( pPlayer->Extra_Char_Num == CID_ARME4 )
		{
			if ( g_MyD3D->MyCtrl->k_Fresh_Punch )
			{
				pPlayer->SetPlayerState( MID_ARME4_DASHATK2 );
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				pPlayer->cFrame = 0;
			}
		}
	}
	return 0;
}

int KGCPlayerCommon::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Up )
        {   
            if( pPlayer->IsContact )
            {           
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
                else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

                pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }           
            pPlayer->NoCheckContact = 0;
        }

// 		if( ( ( pPlayer->bIsRight && ISPUSHED(k_Right) ) || (!pPlayer->bIsRight && ISPUSHED(k_Left) ) ) && pPlayer->GetCurrentChar().iCharType == GC_CHAR_UNO) 
// 		{
// 			if( pPlayer->cFrame >= 10 )
// 			{
// 				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().RUN );
// 				pPlayer->cFrame = 0;
// 				return 0;
// 			}
// 			
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {   
            if( !pPlayer->IsContact && pPlayer->GetCurFormTemplate().JUMP_DASH_ATK && pPlayer->GetCurFormTemplate().JUMP_DASH_ATK != -1 )
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
            else
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);

            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            return 0;
        }
        if( g_MyD3D->MyCtrl->k_Down )
        {
            g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }
        
        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerCommon::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if (pPlayer->GetCurrentChar().iCharType == GC_CHAR_JIN && SiKGCRoomManager()->IsMonsterGameMode() == true)
        {
            if (pPlayer->GetMP() == 0.0f) {
                if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON))
                    pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON);
                if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON))
                    pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON);
            }
        }
        //    if (g_MyD3D->MyCtrl->k_Skill_Key)
        //    {
        //        if (g_MyD3D->MyCtrl->k_Left) {
        //            g_MyD3D->MyCtrl->k_Skill_Key = false;
        //            g_MyD3D->MyCtrl->k_Left = false;
        //            if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON))
        //                pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON);
        //            if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON))
        //                pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON);
        //            pPlayer->SetPlayerState(MID_JIN1_DIAMONDBODY);
        //            pPlayer->cFrame = 0;
        //            return 0;
        //        }
        //        if (g_MyD3D->MyCtrl->k_Right) {
        //            g_MyD3D->MyCtrl->k_Skill_Key = false;
        //            g_MyD3D->MyCtrl->k_Right = false;
        //            if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON))
        //                pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KUMKANG_DUNGEON);
        //            if (pPlayer->IsMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON))
        //                pPlayer->ClearMagicEffect(EGC_SKILL_JIN_RENEW_KYUHWA_DUNGEON);
        //            pPlayer->SetPlayerState(MID_JIN1_HOLLYHOCK);
        //            pPlayer->cFrame = 0;
        //            return 0;
        //        }
        //        return 0;
        //    }
        //}

		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPlayer->bIsRight = true;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                if( g_kCamera.CheckDashZoom() )
                g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }
        else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPlayer->bIsRight = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
                g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }		

        if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
                g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Down )
        {
            g_MyD3D->MyCtrl->k_Down = false;
			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 40;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			{
				if( pPlayer->Next_Attack_Count != 255 )
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
				}
			}
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
            }
            //KeyProc�ڿ��� ���� Frameó�� �ϴ� �κ��� �����Ƿ� Frame = 0���� ���ݴϴ�.
            //KeyProc�� �ƴϸ� Frame = -1�� �غ��ô�!

            pPlayer->NoCheckContact = 0;

            pPlayer->cFrame = 0;

            g_MyD3D->MyCtrl->k_Fresh_Punch = false;

            //�����ý��� ��� Ÿ�ݿ� ���������� �޺��� �̾���
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            return 0;
        }
        pPlayer->x_Speed = 0.0f;

        if( g_MyD3D->MyCtrl->k_Right )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
                pPlayer->cFrame = 0;
            pPlayer->bIsRight = true;
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
            return 0;
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
                pPlayer->cFrame = 0;
            pPlayer->bIsRight = false;
            pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
            return 0;
        }
        else        //Walk���¿��� Wait���·� ���ƿ���...
        {
            if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
                pPlayer->cFrame = 0;

            if( pPlayer->GetHP() > 0.0f )
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk�� �������� ����ϱ� ����..�־�� �ڵ���
            else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
            pPlayer->x_Speed = 0.0f;
            
        }
		
		if( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BURNNING_MODE ) //��
		{			
			if( g_MyD3D->MyCtrl->Burnning_X_Count == 10 && 
				!pPlayer->m_bIsBurnningMode && !pPlayer->m_bIsMovedAfterCharge )
			{
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().BURNNING_CHARGE ); 
				pPlayer->cFrame = 0;
			}
		}
        return 0;
    }
    
	pPlayer->m_bJumpDashed = false;
    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //���� �ݺ��ϴ� ������..
        pPlayer->cFrame = 0;
    
    return 0;
}

int KGCPlayerCommon::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        OnWait(pPlayer,true);
        return 0;
    }

    pPlayer->SetWindSpeed();

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )      //���� �ݺ��ϴ� ������..
        pPlayer->cFrame = 0;

    return 0;
}

int KGCPlayerCommon::OnRun( PLAYER* pPlayer, bool bKeyProc )
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
                else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

                pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);                
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }

        if( g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;

            if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
                return 0;            

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            if( pPlayer->IsContact )
                pPlayer->y_Speed = -0.015f;

            pPlayer->NoCheckContact = 20;
            return 0;
        }
        if(pPlayer->IsContact)
        {
            if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left)
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right)
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( ( ( pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Right ) ||
            ( !pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )//����
        {
            pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame();
            return 0;
        }
        return 0;
    }

    if(pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 ) //���� �ݺ��ϴ� ������..
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();

    return 0;
}


int KGCPlayerCommon::OnEmptyNoContactCheck(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
        return 0;

    pPlayer->x_Speed = 0.0f;

    if( pPlayer->cFrame < (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 11 )
    {
        pPlayer->NoCheckContact = 10;
        pPlayer->y_Speed = 0.0f;
        pPlayer->Gravity = 100;
    }
    return 0;
}

int KGCPlayerCommon::OnEmptyLoop( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
        return 0;

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame()-1 )
        pPlayer->cFrame = 0;

    return 0;
}

int KGCPlayerCommon::OnFreezedByPixie(PLAYER* pPlayer, bool bKeyProc)
{
    if ( bKeyProc )
        return 0;

    pPlayer->m_bFrameLock = true;

    if( g_MyD3D->MyCtrl->k_Fresh_Left || g_MyD3D->MyCtrl->k_Fresh_Right )
    {
        pPlayer->cFrame += pPlayer->m_iPlusFreezeCount;
        
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )
        {
            pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
            pPlayer->m_bFrameLock = false;
        }
    }  
    return 0;
}

bool KGCPlayerCommon::LoadPlayerTemplate( const std::vector< EGCSkillTree >& setSkill, std::string strFileName, PLAYER_TEMPLATE& Template, char cCharNum )
{
	// cCharNum���� CID_COMMON�� �Ѿ���� ����ü��� ���̴�... �׳� �׷��� ¥���Ҵ�..
	// ó���� ��ųƮ�� ���� ��, ����ü ��ų�� ���� ���ǰ� ������ �� �ϴ�...
	// ����� ��ȹ�ڰ� �ϴ� ���� ���� ����... ��Ե� �� ���� ��Ե� �� ��������... =_-;
    static bool bLoadedOtherScript = false;
    KLuaManager& luaMgr = ms_kLuaMgr_PlayerTemplate;

    if( bLoadedOtherScript == false ) {
        if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
            return false;

        if( GCFUNC::LoadLuaScript( luaMgr, "MotionEnum.stg" ) == false )
            return false;

        if( GCFUNC::LoadLuaScript( luaMgr, "DamageEnum.stg" ) == false )
            return false;

        if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
            return false;

        if( GCFUNC::LoadLuaScript( luaMgr, "SkillChangeMotion.lua" ) == false )
            return false;

        bLoadedOtherScript = true;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, strFileName, false ) == false )
        return false;

//#ifndef LEVEL_DESIGN_STAT_CALC
    //GCFUNC::LoadLuaScript( luaMgr, "TemplateInfo.lua", false );


    //int iStandard = 0;
    //int iExtend = 0;
    //LUA_BEGIN_TABLE_RAII( "TemplateInfo" ) 
    //{
    //    LUA_BEGIN_TABLE_RAII( "MotionList" )
    //    { 
    //        LUA_GET_VALUE_DEF( "Standard", iStandard, 0 );
    //        LUA_GET_VALUE_DEF( "Extend", iExtend, 0 );
    //    }
    //}
//#endif
    char strFunc[MAX_PATH];
    std::vector< EGCSkillTree >::const_iterator sit = setSkill.begin();
	
    while( sit != setSkill.end() )
    {
        const GCSkill* pSkill = SiGCSKT()->GetSkill( (EGCSkillTree)(*sit) );
        if( pSkill && pSkill->GetCharType() == (EGCCharType)( cCharNum % GC_CHAR_NUM ) ) {
		    sprintf( strFunc, "Skill_ChangeMotion[%d](%d)", (int)(*sit), (int)cCharNum );
		    luaMgr.DoString( strFunc, strlen(strFunc) );

            sprintf( strFunc, "Skill_ChangeMotion_%d (%d)", (int)(*sit), (int)cCharNum );
            luaMgr.DoString( strFunc, strlen(strFunc) );
        }
		
		++sit;
		
    }

    float fTemp;
    int iTemp;
    DWORD dwTemp;
    BOOL bTemp;

    // ���� ��ġ �ʱ�ȭ
    Template.vLeftWeaponBase = D3DXVECTOR3(0.0f,0.0f,0.0f);
    Template.vLeftWeaponEnd = D3DXVECTOR3(0.0f,0.0f,0.0f);
    Template.vWeaponBase = D3DXVECTOR3(0.0f,0.0f,0.0f);
    Template.vWeaponEnd = D3DXVECTOR3(0.0f,0.0f,0.0f);

    LUA_BEGIN_TABLE_RAII( "Template" )
    {
        LUA_GET_VALUE( "fCharHeight",       fTemp, return false )
            Template.fCharHeight = fTemp;
        LUA_GET_VALUE_DEF( "fPlayingScale",     fTemp, 0.0f )
            Template.fPlayingScale = fTemp;
        LUA_GET_VALUE( "fIncHP",            fTemp, return false )
            Template.fIncHP = fTemp;
        LUA_GET_VALUE( "dwStopOnDamage",    dwTemp, return false )
            Template.dwStopOnDamage = dwTemp;
        LUA_GET_VALUE( "iNextAttackCount",  iTemp, return false )
            Template.iNextAttackCount = iTemp;
        LUA_GET_VALUE_DEF( "fDamagefactor",     fTemp, 1.0f );
        Template.fDamagefactor = fTemp;
        // �� - ���װ����� ���� �ð����庯����
        LUA_GET_VALUE_DEF( "fBPChargeTime", fTemp, 0.0f );
        Template.fBPChargeTime = fTemp;
        LUA_GET_VALUE_DEF( "fBPStopDelayTime", fTemp, 0.0f );
        Template.fBPStopDelayTime = fTemp;
        LUA_GET_VALUE_DEF( "fBPDecreaseTime", fTemp, 1.0f );
        Template.fBPDecreaseTime = fTemp;
        LUA_GET_VALUE_DEF( "fBPHitIncrease", fTemp, 0.0f );
        Template.fBPHitIncrease = fTemp;
        //���װ����� ������ ����Ʈ�� �߰� ���� �Ұǰ�?
        LUA_GET_VALUE_DEF( "bUseBurnCharge", Template.bUseBurnCharge , false );

        // ���� 1������ ���ð������� ���� ����
        LUA_GET_VALUE_DEF( "fHGPCumulativeNum", fTemp, 0.0f );
        Template.fHGPCumulativeNum = fTemp;
        LUA_GET_VALUE_DEF( "fHGPDecreaseTime", fTemp, 0.0f );
        Template.fHGPDecreaseTime = fTemp;

        LUA_GET_VALUE_DEF( "fBPPerAttackPlayer", fTemp, 0.0f );
        Template.fBPPerAttackPlayer = fTemp;
        LUA_GET_VALUE_DEF( "fBPPerAttackMonster", fTemp, 0.0f );
        Template.fBPPerAttackMonster = fTemp;
        LUA_GET_VALUE_DEF( "fBPPerAttackedMe", fTemp, 0.0f );
        Template.fBPPerAttackedMe = fTemp;

        // ĳ���� ���� �ɷ� Ÿ��
        LUA_GET_VALUE_DEF( "iSubAbilityType", iTemp, SUB_ABILITY_NONE );        
        Template.iSubAbilityType = iTemp;

        // ���� ����Ʈ Ư��
        LUA_GET_VALUE_DEF( "iQPChargeFrame", iTemp, 0 );
        Template.iQPChargeFrame = iTemp;
        LUA_GET_VALUE_DEF( "iMaxQP", iTemp, 0 );
        Template.iMaxQP = iTemp;
        LUA_GET_VALUE_DEF( "iRageModeHpPercent", iTemp, -1 );
        Template.iRageModeHpPercent = iTemp;

		LUA_GET_VALUE_DEF( "iMaxBP", iTemp, -1 );
		Template.iMaxBP = iTemp;

		LUA_GET_VALUE_DEF( "fSpecial4HPReduceRatio", fTemp, 0.0f );
		Template.fSpecial4HPReduceRation = fTemp;

        LUA_GET_VALUE_DEF( "fMaxMarbleAmount", fTemp, 0.0f );
        Template.fMaxMarbleAmount = fTemp;

        LUA_GET_VALUE_DEF( "iWeaponBoneIndex", Template.iWeaponBoneIndex, BID_RIGHT_WRIST );
        LUA_GET_VALUE_DEF( "iLeftWeaponBoneIndex", Template.iWeaponBoneIndexLeft, BID_LEFT_WRIST );

        LUA_GET_VALUE_DEF( "WeaponRenderBasically", Template.WeaponRenderBasically, true );
        LUA_GET_VALUE_DEF( "LeftWeaponRenderBasically", Template.LeftWeaponRenderBasically, false );

		LUA_GET_VALUE_DEF( "NeedBurningForSpecial", Template.BurningCostForSpecial, 1.0f );

        LUA_GET_VALUE_DEF( "fManaRecoveryRatioByAttackMon", fTemp, -1.0f );
        Template.fManaRecoveryRatioByAttackMon = fTemp;

        LUA_GET_VALUE_DEF( "fManaAutoRecoveryRatio", fTemp, -1.0f );
        Template.fManaAutoRecoveryRatio = fTemp;
	
        LUA_BEGIN_TABLE_RAII("Speed" )
        {
            LUA_GET_VALUE( "fJumpYSpeed",           fTemp, return false )
                Template.fJumpYSpeed = fTemp;
            LUA_GET_VALUE( "fJumpXSpeed",           fTemp, return false )
                Template.fJumpXSpeed = fTemp;
            LUA_GET_VALUE( "fDoubleJumpYSpeed",         fTemp, return false )
                Template.fDoubleJumpYSpeed = fTemp;
            LUA_GET_VALUE( "fDoubleJumpXSpeed",         fTemp, return false )
                Template.fDoubleJumpXSpeed = fTemp;
            LUA_GET_VALUE( "fRunJumpYSpeed",            fTemp, return false )
                Template.fRunJumpYSpeed = fTemp;
            LUA_GET_VALUE( "fRunJumpXSpeed",            fTemp, return false )
                Template.fRunJumpXSpeed = fTemp;
            LUA_GET_VALUE( "fDashJumpYSpeed",           fTemp, return false )
                Template.fDashJumpYSpeed = fTemp;
            LUA_GET_VALUE( "fDashJumpXSpeed",           fTemp, return false )
                Template.fDashJumpXSpeed = fTemp;
            LUA_GET_VALUE( "fWalkSpeed",            fTemp, return false )
                Template.fWalkSpeed = fTemp;
            LUA_GET_VALUE( "fRunSpeed",         fTemp, return false )
                Template.fRunSpeed = fTemp;
        }
        else
            return false;

        LUA_BEGIN_TABLE_RAII("Motion" )
        {
            LUA_GET_VALUE( "SPECIAL1_W",            iTemp, return false )
                Template.SPECIAL1_W = iTemp;
            Template.ORIGINAL1_W = iTemp;
            LUA_GET_VALUE( "SPECIAL2_W",            iTemp, return false )
                Template.SPECIAL2_W = iTemp;
            Template.ORIGINAL2_W = iTemp;
            LUA_GET_VALUE( "SPECIAL3_W",            iTemp, return false )
                Template.SPECIAL3_W = iTemp;
            Template.ORIGINAL3_W = iTemp;
            LUA_GET_VALUE( "SPECIAL1_B",            iTemp, return false )
                Template.SPECIAL1_B = iTemp;
            Template.ORIGINAL1_B = iTemp;
            LUA_GET_VALUE( "SPECIAL2_B",            iTemp, return false )
                Template.SPECIAL2_B = iTemp;
            Template.ORIGINAL2_B = iTemp;
            LUA_GET_VALUE( "SPECIAL3_B",            iTemp, return false )
                Template.SPECIAL3_B = iTemp;
            Template.ORIGINAL3_B = iTemp;            

            if( luaMgr.BeginTable( "SPECIAL_ADD_MOTION" ) == S_OK )
            {
                for( int i = 1; ; i++ )
                {
                    LUA_GET_VALUE_NOASSERT( i,            iTemp, break )
                        Template.SPECIAL_ADD_MOTION.push_back(iTemp);
                }
                luaMgr.EndTable();
            }

            LUA_GET_VALUE( "RUN",                   iTemp, return false )
                Template.RUN = iTemp;
            LUA_GET_VALUE( "ATK1",                  iTemp, return false )
                Template.ATK1 = iTemp;
            LUA_GET_VALUE( "ATKEND",                iTemp, return false )
                Template.ATKEND = iTemp;
            LUA_GET_VALUE( "WAIT",                  iTemp, return false )
                Template.WAIT = iTemp;
            LUA_GET_VALUE( "FATAL_WAIT",            iTemp, return false )
                Template.FATAL_WAIT = iTemp;
            LUA_GET_VALUE( "WALK",                  iTemp, return false )
                Template.WALK = iTemp;
            LUA_GET_VALUE( "DASH",                  iTemp, return false )
                Template.DASH = iTemp;
            LUA_GET_VALUE( "DASH_ATK",              iTemp, return false )
                Template.DASH_ATK = iTemp;
            LUA_GET_VALUE( "WIN",                   iTemp, return false )
                Template.WIN = iTemp;
            LUA_GET_VALUE( "START",                 iTemp, return false )
                Template.START = iTemp;
            LUA_GET_VALUE( "SELECT",                iTemp, return false )
                Template.SELECT = iTemp;
            LUA_GET_VALUE( "RESULT_WIN",            iTemp, return false )
                Template.RESULT_WIN = iTemp;
            LUA_GET_VALUE( "RESULT_LOSE",           iTemp, return false )
                Template.RESULT_LOSE = iTemp;
            LUA_GET_VALUE( "JOKE",                  iTemp, return false )
                Template.JOKE = iTemp;
            LUA_GET_VALUE( "CRITICAL_ATK",          iTemp, return false )
                Template.CRITICAL_ATK = iTemp;
            LUA_GET_VALUE( "DOUBLE_ATK",            iTemp, return false )
                Template.DOUBLE_ATK = iTemp;

            //������ �����ִ� ���·� Dash�ϸ� �ߵ� �Ǵ� ���..
            //Tiger Rolling Action Ÿ�̰ż��� ��� ���� �ִٸ� ���� �ִ�. -1�̸� ��� ����
            LUA_GET_VALUE( "GROUND_DOWN_DASH",          iTemp, return false )
                Template.GROUND_DOWN_DASH = iTemp;
            // ZŰ ��Ÿ�� ������ ��� ------------------------------------------------------
            LUA_GET_VALUE( "ATK1_FASTINPUT",            iTemp, return false )
                Template.ATK1_FASTINPUT = iTemp;
            LUA_GET_VALUE( "iFastInputNeedCount",       iTemp, return false )
                Template.iFastInputNeedCount = iTemp;

            // ���� �Ͼ�鼭 ġ���� --------------------------------------------------
            LUA_GET_VALUE( "CATCH",                     iTemp, return false )
                Template.CATCH = iTemp;
            LUA_GET_VALUE( "WAKE",                      iTemp, return false )
                Template.WAKE = iTemp;
            // ���� ���� -------------------------------------------------------------------
            LUA_GET_VALUE( "JUMP_READY",                iTemp, return false )
                Template.JUMP_READY = iTemp;
            LUA_GET_VALUE( "JUMP",                      iTemp, return false )
                Template.JUMP = iTemp;
            LUA_GET_VALUE( "JUMP_ATK",                  iTemp, return false )
                Template.JUMP_ATK = iTemp;
            LUA_GET_VALUE( "JUMP_DASH",                 iTemp, return false )
                Template.JUMP_DASH = iTemp;
            LUA_GET_VALUE( "JUMP_DASH_ATK",             iTemp, return false )
                Template.JUMP_DASH_ATK = iTemp;
            // �������ݽ� ���� �浹 ���ϵ��� ����� ����
            LUA_GET_VALUE( "iJumpAtkNoCheckContact",    iTemp, return false )
                Template.iJumpAtkNoCheckContact = iTemp;

            // 2�� ���� ����
            LUA_GET_VALUE( "DOUBLE_JUMP",               iTemp, return false )
                Template.DOUBLE_JUMP = iTemp;
            LUA_GET_VALUE( "DOUBLE_JUMP_ATK",           iTemp, return false )
                Template.DOUBLE_JUMP_ATK = iTemp;
            LUA_GET_VALUE( "DOUBLE_JUMP_LAND",          iTemp, return false )
                Template.DOUBLE_JUMP_LAND = iTemp;

            // ȭ�� ���� --------------------------------------------------------------------
            LUA_GET_VALUE( "ENABLE_ARROWDEF",           bTemp, return false )
                Template.ENABLE_ARROWDEF = bTemp;
            LUA_GET_VALUE( "ARROWDEF_GROUND1",          iTemp, return false )
                Template.ARROWDEF_GROUND1 = iTemp;
            LUA_GET_VALUE( "ARROWDEF_GROUND2",          iTemp, return false )
                Template.ARROWDEF_GROUND2 = iTemp;
            LUA_GET_VALUE( "ARROWDEF_SKY",              iTemp, return false )
                Template.ARROWDEF_SKY = iTemp;
            LUA_GET_VALUE( "ARROW_REFLECTION",          iTemp, return false )
                Template.ARROW_REFLECTION = iTemp;
            LUA_GET_VALUE( "iRefelectionRate",          iTemp, return false )
                Template.iRefelectionRate = iTemp;
            LUA_GET_VALUE( "BURNNING_CHARGE",	        iTemp, return false )
                Template.BURNNING_CHARGE = iTemp;
            LUA_GET_VALUE( "BURNNING_ACTIVE",	        iTemp, return false )
                Template.BURNNING_ACTIVE = iTemp;
            LUA_GET_VALUE_DEF( "DOWN_STAND_UP_EVASION",     iTemp, -1 )
                Template.DOWN_STAND_UP_EVASION = iTemp;

            LUA_BEGIN_TABLE_RAII( "ENEMY_ATK_GUARD_MOTION" )
            {
                Template.mapEnemyAtkGuardMotion.clear();
                int iReady;
                int iPlay;
                for(int i = 1; ; i++) 
                {                
                    LUA_BEGIN_TABLE_RAII( i ) 
                    {
                        LUA_GET_VALUE_DEF( "READY", iReady, -1 );
                        LUA_GET_VALUE_DEF( "PLAY", iPlay, -1 );

                        Template.mapEnemyAtkGuardMotion.insert( std::make_pair( iReady, iPlay ) );
                    } 
                    else 
                    {
                        break;
                    }
                }
            }

            LUA_GET_VALUE_DEF( "SID_EXPANSION_MANA",     iTemp, -1 )
                Template.SID_EXPANSION_MANA = iTemp;

            LUA_GET_VALUE_DEF( "SID_SPECIAL4",            iTemp, -1 )
                Template.SID_SPECIAL4 = iTemp;

			LUA_GET_VALUE_DEF( "SID_SPECIAL4_1",            iTemp, -1 )
				Template.SID_SPECIAL4_1 = iTemp;

            LUA_GET_VALUE_DEF( "SAVING_STANCE",     iTemp, -1 )
                Template.SAVING_STANCE = iTemp;

            LUA_GET_VALUE_DEF( "SAVING_ATTACK",     iTemp, -1 )
                Template.SAVING_ATTACK = iTemp;

            LUA_GET_VALUE_DEF( "SAVING_EVASION",     iTemp, -1 )
                Template.SAVING_EVASION = iTemp;
            LUA_GET_VALUE_DEF( "SAVING_SKILL_ID",     iTemp, -1 )
                Template.SAVING_SKILL_ID = iTemp;
        }
        else
            return false;

        LUA_BEGIN_TABLE_RAII("Sound")
        {
            // 2007/2/14. iridology. ���� ��������� ������ �� �ְ� �ٲٸ���..
            LUA_GET_VALUE( "ATK0",          Template.sSound.ATK[0], return false )
                LUA_GET_VALUE( "ATK1",          Template.sSound.ATK[1], return false )
                LUA_GET_VALUE( "ATK2",          Template.sSound.ATK[2], return false )
                LUA_GET_VALUE( "ATK3",          Template.sSound.ATK[3], return false )
                LUA_GET_VALUE( "ATK4",          Template.sSound.ATK[4], return false )
                LUA_GET_VALUE( "AH",            Template.sSound.AH[0], return false )
                LUA_GET_VALUE_DEF( "AH2", Template.sSound.AH[1], Template.sSound.AH[0] );
            LUA_GET_VALUE_DEF( "AH3", Template.sSound.AH[2], Template.sSound.AH[0] );
            LUA_GET_VALUE_DEF( "AH4", Template.sSound.AH[3], Template.sSound.AH[0] );
            LUA_GET_VALUE( "AHH",           Template.sSound.AHH, return false )
                LUA_GET_VALUE( "AHHH",          Template.sSound.AHHH, return false )
                LUA_GET_VALUE( "STAND",         Template.sSound.STAND[0], return false )
                LUA_GET_VALUE_DEF( "STAND_EX",  Template.sSound.STAND[1], -1 )
        }
        else
            return false;

        LUA_BEGIN_TABLE_RAII( "Weapon" )
        {
            LUA_BEGIN_TABLE_RAII("Base" )
            {
                LUA_GET_VALUE_DEF("x", Template.vWeaponBase.x, 0.0f );
                LUA_GET_VALUE_DEF("y", Template.vWeaponBase.y, 0.0f );
                LUA_GET_VALUE_DEF("z", Template.vWeaponBase.z, 0.0f );
            }

            LUA_BEGIN_TABLE_RAII("End" )
            {
                LUA_GET_VALUE_DEF("x", Template.vWeaponEnd.x, 0.0f );
                LUA_GET_VALUE_DEF("y", Template.vWeaponEnd.y, 0.0f );
                LUA_GET_VALUE_DEF("z", Template.vWeaponEnd.z, 0.0f );
            }

            LUA_BEGIN_TABLE_RAII("LeftBase" )
            {
                LUA_GET_VALUE_DEF("x", Template.vLeftWeaponBase.x, 0.0f );
                LUA_GET_VALUE_DEF("y", Template.vLeftWeaponBase.y, 0.0f );
                LUA_GET_VALUE_DEF("z", Template.vLeftWeaponBase.z, 0.0f );
            }

            LUA_BEGIN_TABLE_RAII("LeftEnd" )
            {
                LUA_GET_VALUE_DEF("x", Template.vLeftWeaponEnd.x, 0.0f );
                LUA_GET_VALUE_DEF("y", Template.vLeftWeaponEnd.y, 0.0f );
                LUA_GET_VALUE_DEF("z", Template.vLeftWeaponEnd.z, 0.0f );
            }
        }

        LUA_BEGIN_TABLE_RAII("TagInfo" )
        {
            int iMotion;
            LUA_GET_VALUE_DEF( "TAG_START", iMotion, MID_COMMON_STONECURSE );
            Template.sTagTemplate.m_uiStartMotion = iMotion;
            LUA_GET_VALUE_DEF( "TAG_END", iMotion, MID_COMMON_STONECURSE );
            Template.sTagTemplate.m_uiEndMotion = iMotion;
            LUA_GET_VALUE_DEF( "TAG_ATK", iMotion, MID_COMMON_STONECURSE );
            Template.sTagTemplate.m_uiAtkMotion = iMotion;
        }
        else {
            ASSERT(!"No Tag Information" );
        }


        //���� ����� ������� �ʴ� �ɸ��͵��� � ����� ����� �� �ҷ����� �κ��Դϴ�.
        Template.mapCommonToOwnMotion.clear();
        LUA_BEGIN_TABLE_RAII("OwnMotion")
        {
            int iCommonMotion;
            int iOwnMotion;
            for( int i = 1 ; ; ++i )
            {
                LUA_BEGIN_TABLE_RAII(i)
                {
                    LUA_GET_VALUE_NOASSERT( 1 , iCommonMotion   , return false );
                    LUA_GET_VALUE_NOASSERT( 2 , iOwnMotion      , return false );

                    Template.mapCommonToOwnMotion[iCommonMotion].first = iCommonMotion;
                    Template.mapCommonToOwnMotion[iCommonMotion].second = iOwnMotion;
                }
                else
                    break;
            }
        }

        Template.vecSummonMonsterID.clear();
        LUA_BEGIN_TABLE_RAII("SummonMonsterList")
        {
            int iMonsterID;
            for( int i = 1 ; ; ++i )
            {
                LUA_GET_VALUE_NOASSERT( i, iMonsterID, break; );
                Template.vecSummonMonsterID.push_back( iMonsterID );
            }
        }

        Template.vecHitEffect.clear();
        LUA_BEGIN_TABLE_RAII("HitEffect")
        {
            int iEffectKind;
            int iEffectNum;
            std::string strParticleName;
            for( int i = 1 ; ; ++i )
            {
                LUA_BEGIN_TABLE_RAII( i )
                {
                    LUA_GET_VALUE( 1, iEffectKind, return false );
                    LUA_GET_VALUE( 2, iEffectNum, return false );
                    LUA_GET_VALUE_NOASSERT( 3, strParticleName, ; );

                    HIT_EFFECT hit;
                    hit.first = iEffectKind;
                    hit.second = iEffectNum;
                    hit.strParticleName = strParticleName;

                    Template.vecHitEffect.push_back( hit );
                }
                else
                    break;
            }
        }

        Template.sMetamorphosisTemplate.init();
        LUA_BEGIN_TABLE_RAII("Metamorphosis")
        {
            LUA_GET_VALUE( "FormID", Template.sMetamorphosisTemplate.iFormID, return false );
            LUA_GET_VALUE_DEF( "FormChangeCompleteMotion", Template.sMetamorphosisTemplate.FormChangeCompleteMotion, Template.WAIT );
            LUA_GET_VALUE( "StartIncApRatio", Template.sMetamorphosisTemplate.StartIncApRatio, return false );
            LUA_GET_VALUE( "StartIncDpRatio", Template.sMetamorphosisTemplate.StartIncDpRatio, return false );
            LUA_GET_VALUE( "StartHPRatio", Template.sMetamorphosisTemplate.StartHPRatio, return false );

            LUA_GET_VALUE( "DamageRatio", Template.sMetamorphosisTemplate.fDamageRatio, return false );
            LUA_GET_VALUE( "UseItem", Template.sMetamorphosisTemplate.iUseItem, return false );

            LUA_GET_VALUE( "EndIncApRatio", Template.sMetamorphosisTemplate.EndIncApRatio, return false );
            LUA_GET_VALUE( "EndIncDpRatio", Template.sMetamorphosisTemplate.EndIncDpRatio, return false );
            LUA_GET_VALUE( "EndHPRatio", Template.sMetamorphosisTemplate.EndHPRatio, return false );            

            LUA_BEGIN_TABLE_RAII("StartEffect" )
            {
                METAMORPHOSIS_TEMPLATE::S_Effect sEffect;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE(1, sEffect.iEffectKind , return false);
                        LUA_GET_VALUE(2, sEffect.strEffectName  , return false);
                        LUA_GET_VALUE(3, sEffect.iParticleNum , return false);
                        LUA_GET_VALUE(4, sEffect.fOffset , return false);

                        Template.sMetamorphosisTemplate.vecStartEffect.push_back( sEffect );
                    }
                    else
                        break;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("StartBuffEffect")
            {
                METAMORPHOSIS_TEMPLATE::S_StartBuffEffect sStartBuffEffect;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE(1, sStartBuffEffect.iEffectID , return false);
                        LUA_GET_VALUE(2, sStartBuffEffect.iGoodsID  , return false);
                        LUA_GET_VALUE(3, sStartBuffEffect.fBuffTime , return false);

                        Template.sMetamorphosisTemplate.vecStartBuffEffect.push_back( sStartBuffEffect );
                    }
                    else
                        break;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("MetamorphosisModel")
            {
                METAMORPHOSIS_TEMPLATE::S_MetamorphosisModel sMetamorphosisModel;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE(1, sMetamorphosisModel.strModelFileName, return false);
                        LUA_GET_VALUE(2, sMetamorphosisModel.iGoodsID  , return false);

                        Template.sMetamorphosisTemplate.vecMetamorphosisModel.push_back( sMetamorphosisModel );
                    }
                    else
                        break;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("MetamorphosisShoadow")
            {               
                LUA_GET_VALUE("Model", Template.sMetamorphosisTemplate.sMetamorphosisShoadow.strModelFileName, return false);
                LUA_GET_VALUE("Texture", Template.sMetamorphosisTemplate.sMetamorphosisShoadow.strTextureFileName, return false);                   
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("MetamorphosisTexture")
            {
                METAMORPHOSIS_TEMPLATE::S_MetamorphosisTexture sMetamorphosisTexture;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE(1, sMetamorphosisTexture.strTextureFileName , return false);
                        LUA_GET_VALUE(2, sMetamorphosisTexture.iGoodsID , return false);

                        Template.sMetamorphosisTemplate.vecMetamorphosisTexture.push_back( sMetamorphosisTexture );
                    }
                    else
                        break;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("EndEffect")
            {
                METAMORPHOSIS_TEMPLATE::S_Effect sEffect;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE_DEF(1, sEffect.iEffectKind , 0);
                        LUA_GET_VALUE_DEF(2, sEffect.strEffectName  , "NULL");
                        LUA_GET_VALUE_DEF(3, sEffect.iParticleNum , 0);
                        LUA_GET_VALUE_DEF(4, sEffect.fOffset , 0);

                        Template.sMetamorphosisTemplate.vecEndEffect.push_back( sEffect );
                    }
                    else
                        break;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII("EndBuffEffect")
            {
                METAMORPHOSIS_TEMPLATE::S_EndBuffEffect sEndBuffEffect;
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII(i)
                    {
                        LUA_GET_VALUE_DEF(1, sEndBuffEffect.iEffectID , 0);
                        LUA_GET_VALUE_DEF(2, sEndBuffEffect.iGoodsID  , 0);
                        LUA_GET_VALUE_DEF(3, sEndBuffEffect.fBuffTime , 0);

                        Template.sMetamorphosisTemplate.vecEndBuffEffect.push_back( sEndBuffEffect );
                    }
                    else
                        break;
                }
            }
            else
                return false;


            if( luaMgr.BeginTable( "EndClearBuffList" ) == S_OK )
            {
                for( int i = 1; ; i++ )
                {
                    LUA_GET_VALUE_NOASSERT( i,            iTemp, break )
                        Template.sMetamorphosisTemplate.setEndClearBuffList.insert(iTemp);
                }
                luaMgr.EndTable();
            }




            LUA_BEGIN_TABLE_RAII( "MetamorphosisExtraMesh" )
            {
                for( int i = 1; i ; i++ )
                {
                    EXTRA_MESH mesh;

                    LUA_BEGIN_TABLE_RAII( i )
                    {
                        LUA_GET_VALUE_DEF( "ID", mesh.strID, "" );
                        LUA_GET_VALUE_DEF( "MESH", mesh.strMeshName, "" );
                        LUA_GET_VALUE_DEF( "TEXTURE", mesh.strTextureName, "" );
                        LUA_GET_VALUE_DEF( "CARTOON", mesh.dwCartoon, 0 );
                        LUA_GET_VALUE_DEF( "TECHNIQUE", mesh.dwTechnique, -1 );
                        //��Ǵ޸� �𵨵�
                        LUA_GET_VALUE_DEF( "BONEID", mesh.iBoneId, -1 );
                        LUA_GET_VALUE_DEF( "MOTION", mesh.strMotionName, "" );					
                        LUA_GET_VALUE_DEF( "POS_X", mesh.vOptionalPos.x, 0.0f );
                        LUA_GET_VALUE_DEF( "POS_Y", mesh.vOptionalPos.y, 0.0f );
                        LUA_GET_VALUE_DEF( "POS_Z", mesh.vOptionalPos.z, 0.0f );

                    }
                    else
                        break;
                    
                    Template.sMetamorphosisTemplate.vecMetamorphosisExtraMesh.push_back( mesh );
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "QuickSlotSkill" )
        {
            for( int i = 1; ; ++i )
            {
                if( FAILED( luaMgr.GetValue( i, iTemp ) ) )
                    break;

                Template.vecQuickSlotSkill.push_back( iTemp );
            }
        }        

        LUA_BEGIN_TABLE_RAII( "SpecialFaceTex")
        {
            LUA_GET_VALUE( "Texture" , Template.sPlayerSpecialFace.strFaceTextureName , return false);
            LUA_BEGIN_TABLE_RAII( "Movement")
            {
                for( int i = 1 ; i < 4 ; ++i )
                {                
                    float fx = 0, fy = 0;
                    LUA_GET_VALUE( (i*2) - 1 , fx, return false);
                    LUA_GET_VALUE( (i*2)     , fy, return false);
                    Template.sPlayerSpecialFace.avFacePosition[i-1].x = fx;
                    Template.sPlayerSpecialFace.avFacePosition[i-1].y = fy;
                }
            }
            else
                return false;

            LUA_BEGIN_TABLE_RAII( "TextureUV" )
            {
                for( int i = 1 ; ; ++i )
                {
                    LUA_BEGIN_TABLE_RAII( i )
                    {
                        SPECIAL_FACE_UVXY sUVXY;

                        LUA_BEGIN_TABLE_RAII( "Texture_LeftTop" )
                        {                        
                            float fx = 0, fy = 0;
                            LUA_GET_VALUE( 1, fx , return false );
                            LUA_GET_VALUE( 2, fy , return false );
                            sUVXY.vTextureLeftTop.x = fx;
                            sUVXY.vTextureLeftTop.y = fy;
                        }
                        else
                            return false;

                        LUA_BEGIN_TABLE_RAII( "Texture_RightButtom" )
                        {
                            float fx = 0, fy = 0;
                            LUA_GET_VALUE( 1, fx , return false );
                            LUA_GET_VALUE( 2, fy , return false );
                            sUVXY.vTextureRightButtom.x = fx;
                            sUVXY.vTextureRightButtom.y = fy;
                        }
                        else
                            return false;

                        LUA_BEGIN_TABLE_RAII( "RightSide" )
                        {
                            LUA_BEGIN_TABLE_RAII( "Screen_LeftTop" )
                            {
                                float fx = 0, fy = 0;
                                LUA_GET_VALUE( 1, fx , return false );
                                LUA_GET_VALUE( 2, fy , return false );
                                sUVXY.vRScreenLeftTop.x = fx;
                                sUVXY.vRScreenLeftTop.y = fy;
                            }
                            else
                                return false;

                            LUA_BEGIN_TABLE_RAII( "Screen_RightButtom" )
                            {
                                float fx = 0, fy = 0;
                                LUA_GET_VALUE( 1, fx , return false );
                                LUA_GET_VALUE( 2, fy , return false );
                                sUVXY.vRScreenRightButtom.x = fx;
                                sUVXY.vRScreenRightButtom.y = fy;
                            }
                            else
                                return false;

                        }
                        else
                            return false;

                        LUA_BEGIN_TABLE_RAII( "LeftSide" )
                        {
                            LUA_BEGIN_TABLE_RAII( "Screen_LeftTop" )
                            {
                                float fx = 0, fy = 0;
                                LUA_GET_VALUE( 1, fx , return false );
                                LUA_GET_VALUE( 2, fy , return false );
                                sUVXY.vLScreenLeftTop.x = fx;
                                sUVXY.vLScreenLeftTop.y = fy;
                            }
                            else
                                return false;

                            LUA_BEGIN_TABLE_RAII( "Screen_RightButtom" )
                            {
                                float fx = 0, fy = 0;
                                LUA_GET_VALUE( 1, fx , return false );
                                LUA_GET_VALUE( 2, fy , return false );
                                sUVXY.vLScreenRightButtom.x = fx;
                                sUVXY.vLScreenRightButtom.y = fy;
                            }
                            else
                                return false;

                        }
                        else
                            return false;


                        Template.sPlayerSpecialFace.vecFaceUVXY.push_back( sUVXY );
                    }
                    else
                        break;
                }
            }
            else
                return false;
        }

        LUA_BEGIN_TABLE_RAII( "OtherSpecialFaceTex")
        {
            LUA_GET_VALUE_DEF( "HowToCheck" , Template.iOtherFaceHowToCheck , -1 );
            LUA_GET_VALUE_DEF( "CheckValue" , Template.iOtherFaceCheckValue , -1 );            
            LUA_GET_VALUE_DEF( "Texture" , Template.strOtherFaceTextureName , "" );
        }

        LUA_BEGIN_TABLE_RAII( "SoundList" )
        {
            int iTemp;
            for( int i = 1; ; ++i )
            {
                LUA_GET_VALUE_NOASSERT( i, iTemp, break );
                Template.setCharSound.insert( iTemp );
            }
        }

        LUA_BEGIN_TABLE_RAII( "FireInfoList" )
        {
            int iTemp;
            for( int i = 1; ; ++i )
            {
                LUA_GET_VALUE_NOASSERT( i, iTemp, break );
                Template.setMeshAnimation.insert( iTemp );
            }
        }

        LUA_BEGIN_TABLE_RAII( "ExtraMesh" )
        {
            for( int i = 1; i ; i++ )
            {
                EXTRA_MESH mesh;

                LUA_BEGIN_TABLE_RAII( i )
                {
                    LUA_GET_VALUE_DEF( "ID", mesh.strID, "" );
                    LUA_GET_VALUE_DEF( "MESH", mesh.strMeshName, "" );
                    LUA_GET_VALUE_DEF( "TEXTURE", mesh.strTextureName, "" );
                    LUA_GET_VALUE_DEF( "CARTOON", mesh.dwCartoon, 0 );
                    LUA_GET_VALUE_DEF( "TECHNIQUE", mesh.dwTechnique, -1 );
                    //��Ǵ޸� �𵨵�
                    LUA_GET_VALUE_DEF( "BONEID", mesh.iBoneId, -1 );
                    LUA_GET_VALUE_DEF( "MOTION", mesh.strMotionName, "" );					
                    LUA_GET_VALUE_DEF( "POS_X", mesh.vOptionalPos.x, 0.0f );
                    LUA_GET_VALUE_DEF( "POS_Y", mesh.vOptionalPos.y, 0.0f );
                    LUA_GET_VALUE_DEF( "POS_Z", mesh.vOptionalPos.z, 0.0f );

                }
                else
                    break;
                Template.vecExtraMesh.push_back( mesh );
            }
        }

        Template.mapFaceLiftMesh.clear();
        LUA_BEGIN_TABLE_RAII( "FaceLiftMesh" )
        {
            for( int i = 1; i ; i++ )
            {
                int iID = -1;
                FACE_LIFT_MESH mesh;

                LUA_BEGIN_TABLE_RAII( i )
                {
                    LUA_GET_VALUE_DEF( "ID", iID, -1 );
                    LUA_GET_VALUE_DEF( "MESH", mesh.strMeshName, "" );
                    LUA_GET_VALUE_DEF( "TEXTURE", mesh.strTextureName, "" );
                }
                else
                    break;

                Template.mapFaceLiftMesh.insert( std::make_pair( iID, mesh ) );
            }
        }

        Template.vecItemMeshOption.clear();
        LUA_BEGIN_TABLE_RAII( "ItemMeshOption" )
        {
            for( int i = 1; i ; i++ )
            {
                LUA_BEGIN_TABLE_RAII( i )
                {
                    ITEM_MESH_OPTION option;
                    LUA_GET_VALUE_DEF( "iConSlotPosition", option.dwConSlotPosition, 0 );
                    LUA_GET_VALUE_DEF( "iConPlaying", option.iConPlaying, -1 );
                    LUA_GET_VALUE_DEF( "iConSquare", option.iConSquare, -1 );

                    LUA_GET_VALUE_DEF( "BoneID", option.iBoneID, -1 );
                    LUA_GET_VALUE_DEF( "Technique", option.iTechnique, -1 );
                    LUA_GET_VALUE_DEF( "IsNotLoad", option.bIsNotLoad, false );
                    LUA_GET_VALUE_DEF( "IsRender", option.bIsRender, true );
                    LUA_GET_VALUE_DEF( "IsOnlyTraceParentPos", option.bIsOnlyTraceParentPos, false );
                    LUA_GET_VALUE_DEF( "IsAutoChildNameAsLiveLoopCnt", option.bIsAutoChildNameAsLiveLoopCnt, false );
                    LUA_GET_VALUE_DEF( "ChildName", option.strChildName, "" );
                    LUA_GET_VALUE_DEF( "LiveModelMotionName", option.strLiveMotionName, "" );
                    
                    LUA_BEGIN_TABLE_RAII( "Position" )
                    {
                        LUA_GET_VALUE_DEF( 1, option.vPos.x, 0.0f );
                        LUA_GET_VALUE_DEF( 2, option.vPos.y, 0.0f );
                        LUA_GET_VALUE_DEF( 3, option.vPos.z, 0.0f );
                    }
                    LUA_BEGIN_TABLE_RAII( "Scale" )
                    {
                        LUA_GET_VALUE_DEF( 1, option.vScale.x, DEFAULT_PLAYER_SCALE_SIZE );
                        LUA_GET_VALUE_DEF( 2, option.vScale.y, DEFAULT_PLAYER_SCALE_SIZE );
                        LUA_GET_VALUE_DEF( 3, option.vScale.z, DEFAULT_PLAYER_SCALE_SIZE );
                    }

                    LUA_BEGIN_TABLE_RAII( "AddItemLiveModel" )
                    {
                        option.vecAddItemLiveModel.clear();
                        for(int iLoop=1; ; ++iLoop)
                        {
                            LUA_BEGIN_TABLE_RAII( iLoop )
                            {
                                ADD_ITEM_LIVEMODEL kAddLiveModel;
                                LUA_GET_VALUE_DEF( 1, kAddLiveModel.strChildName, "" );
                                LUA_GET_VALUE_DEF( 2, kAddLiveModel.strMotion, "" );
                                LUA_GET_VALUE_DEF( 3, kAddLiveModel.iBoneID, -1 );
                                LUA_GET_VALUE_DEF( 4, kAddLiveModel.vPos.x, 0.0f );
                                LUA_GET_VALUE_DEF( 5, kAddLiveModel.vPos.y, 0.0f );
                                LUA_GET_VALUE_DEF( 6, kAddLiveModel.vPos.z, 0.0f );
                                LUA_GET_VALUE_DEF( 7, kAddLiveModel.bRender, true );
                                LUA_GET_VALUE_DEF( 8, kAddLiveModel.bOnlyTraceParentPos, false );
                                LUA_GET_VALUE_DEF( 9, kAddLiveModel.bUseBoneIDFromArg, false );
                                LUA_GET_VALUE_DEF( "Technique", kAddLiveModel.iTechnique, -1 );

                                option.vecAddItemLiveModel.push_back( kAddLiveModel );
                            }
                            else
                                break;
                        }
                    }


                    Template.vecItemMeshOption.push_back( option );
                }
                else
                    break;
            }
        }

        LUA_BEGIN_TABLE_RAII( "AddMesh" )
        {
            for( int i = 1; i ; i++ )
            {
                LUA_BEGIN_TABLE_RAII( i )
                {
                    ADD_MESH mesh;
                    LUA_GET_VALUE_DEF( "iConBigHead", mesh.iConBigHead, -1 );
                    LUA_GET_VALUE_DEF( "iConEquipItem", mesh.iConEquipItem, -1 );
                    LUA_GET_VALUE_DEF( "iConNonEquipItem", mesh.iConNonEquipItem, -1 );
                    LUA_GET_VALUE_DEF( "iConGameModeCategory", mesh.iConGameModeCategory, -1 );
                    LUA_GET_VALUE_DEF( "iConPlaying", mesh.iConPlaying, -1 );

                    LUA_GET_VALUE_DEF( "bIsCoordi", mesh.bIsCoordi, false );

                    LUA_GET_VALUE_DEF( "bIsRender", mesh.bIsRender, true );
                    LUA_GET_VALUE_DEF( "bIsExtraMesh", mesh.bIsExtraMesh, false );
                    LUA_GET_VALUE_DEF( "ChildName", mesh.strChildName, "" );
                    LUA_GET_VALUE_DEF( "TextureName", mesh.strTextureName, "" );
                    LUA_GET_VALUE_DEF( "MeshName", mesh.strMeshName, "" );
                    LUA_GET_VALUE_DEF( "MeshID", mesh.strID, "" );
                    LUA_GET_VALUE_DEF( "Technique", mesh.iTechnique, -1 );
                    LUA_GET_VALUE_DEF( "bIsSecondBaseMesh", mesh.bIsSecondBaseMesh, false );
                    LUA_GET_VALUE_DEF( "iConEquipSetItem", mesh.iConEquipSetItem, -1 );
                    LUA_GET_VALUE_DEF( "iConNonEquipSetItem", mesh.iConNonEquipSetItem, -1 );

                    

                    LUA_BEGIN_TABLE_RAII( "LiveInfo" )
                    {
                        int iBoneID;
                        std::string strMotionName;
                        LUA_GET_VALUE_DEF( 1, strMotionName, "" );
                        LUA_GET_VALUE_DEF( 2, iBoneID, -1 );
                        mesh.piLiveInfo = std::make_pair(strMotionName.c_str(), iBoneID);
                    }

                    Template.vecAddMesh.push_back( mesh );
                }
                else
                    break;
            }
        }

        LUA_BEGIN_TABLE_RAII( "MotionList" )
        {
            int iTemp;
            LUA_BEGIN_TABLE_RAII( "Standard" )
            {
                for( int i = 1; ; ++i )
            {
                    if( FAILED( luaMgr.GetValue( i, iTemp ) ) )
                        break;
                    
                    Template.setStandardCharMotion.insert( iTemp );
            }
        }
            LUA_BEGIN_TABLE_RAII( "Extend" )
            {
                for( int i = 1; ; ++i )
                {
                    if( FAILED( luaMgr.GetValue( i, iTemp ) ) )
                        break;

                    Template.setExtendCharMotion.insert( iTemp );
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "DamageList" )
        {
            for( int i = 1; ; ++i )
            {
                if( FAILED( luaMgr.GetValue( i, iTemp ) ) )
                    break;

                Template.setDamageList.insert( iTemp );
            }
        }

        do 
        {
            LUA_BEGIN_TABLE_RAII( "Slash" )
            {
                LUA_GET_VALUE_DEF( "Texture", Template.sPlayerSlash.strSlashName, "slash.dds" );
                LUA_GET_VALUE( "Index", Template.sPlayerSlash.iIndex, break );

                for( int i = 0; i < sizeof(Template.sPlayerSlash.aiElemental) / sizeof(int); ++i )
                {
                    Template.sPlayerSlash.aiElemental[i] = Template.sPlayerSlash.iIndex;
                }
                LUA_BEGIN_TABLE_RAII( "Elemental" )
                {
                    LUA_GET_VALUE( "FIRE", Template.sPlayerSlash.aiElemental[0], break );
                    LUA_GET_VALUE( "ICE", Template.sPlayerSlash.aiElemental[1], break );
                    LUA_GET_VALUE( "LIGHTNING", Template.sPlayerSlash.aiElemental[2], break );
                    LUA_GET_VALUE( "DARK", Template.sPlayerSlash.aiElemental[3], break );
                    LUA_GET_VALUE( "WIND", Template.sPlayerSlash.aiElemental[4], break );
                    LUA_GET_VALUE( "SHINING", Template.sPlayerSlash.aiElemental[5], break );
                }
            }
        } while (false);

        Template.BaseEffect.vecParticleInfo.clear();
        LUA_BEGIN_TABLE_RAII("BaseEffect")
        {
            LUA_GET_VALUE( "Condition", Template.BaseEffect.iCondition, -1 );
            LUA_BEGIN_TABLE_RAII("Particle")
            {
                CharBaseParticle baseparticle;
                for( int iLoop = 1 ; ; ++iLoop )
                {
                    LUA_BEGIN_TABLE_RAII( iLoop )
                    {
                        LUA_GET_VALUE_DEF(1, baseparticle.strSequenceName, "");
                        LUA_GET_VALUE_DEF(2, baseparticle.fOffsetX, 0);
                        LUA_GET_VALUE_DEF(3, baseparticle.fOffsetY, 0);
                        LUA_GET_VALUE_DEF(4, baseparticle.fTraceTime, 0.5);
                        LUA_GET_VALUE_DEF(5, baseparticle.bIsRight, false);
                        LUA_GET_VALUE_DEF(6, baseparticle.iPositionType, 0);
                        LUA_GET_VALUE_DEF(7, baseparticle.iMinLevel, 0);
                        LUA_GET_VALUE_DEF(8, baseparticle.iMaxLevel, MAX_LEVEL);

                        Template.BaseEffect.vecParticleInfo.push_back( baseparticle );
                    }
                    else
                        break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "SpecialBuff" )
        {
            Template.mapSpecialBuff.clear();
            int iIndex;
            int iBuffEnum;
            float fBuffTime;
            int iType;
            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {
                    LUA_GET_VALUE_DEF(1, iIndex, 0 );
                    LUA_GET_VALUE_DEF(2, iBuffEnum, -1 );
                    LUA_GET_VALUE_DEF(3, fBuffTime, 0.f );
                    LUA_GET_VALUE_DEF(4, iType, SPECIAL_BUFF_NORMAL );

                    Template.mapSpecialBuff[ iType ].insert( std::make_pair( iIndex, std::make_pair( iBuffEnum, fBuffTime ) ) );
                } 
                else 
                {
                    break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "PuzzleKeyPattern" )
        {
            Template.mapPuzzleKeyPattern.clear();
            Template.setPuzzleKeyList.clear();
            std::vector<int> vecAtt;

            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {                    
                    for( int j = 1; ; ++j ) {
                        int iValue = 0;
                        if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                            break;
                        }
                        vecAtt.push_back( iValue );
                        Template.setPuzzleKeyList.insert( iValue );
                    }

                    Template.mapPuzzleKeyPattern.insert( std::make_pair( i - 1, vecAtt ) );
                    vecAtt.clear();
                } 
                else 
                {
                    break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "BurningChargeList" )
        {
            Template.vecBurnSytemInfo.clear();
            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {
                    SBurnSytemInfo sBurnInfo;

                    LUA_BEGIN_TABLE_RAII( "DAMAGE_LIST" )
                    {
                        for( int j = 1; ; ++j ) {
                            int iValue = 0;
                            if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                                break;
                            }

                            sBurnInfo.m_vecDamageList.push_back( iValue );
                        }                        
                    }

                    LUA_GET_VALUE_DEF( "BURNING_AMOUNT", sBurnInfo.m_fBurnChargeNum, 0 );
                    LUA_GET_VALUE_DEF( "CHECK_ONLY_ONE_UNIT", sBurnInfo.m_bCheckOnlyOneUnit, false );

                    Template.vecBurnSytemInfo.push_back( sBurnInfo );
                } 
                else 
                {
                    break;
                }
            }
        }

		LUA_BEGIN_TABLE_RAII( "BloodChargeList" )
		{
			Template.vecBloodSytemInfo.clear();
			for(int i = 1; ; i++) 
			{                
				LUA_BEGIN_TABLE_RAII( i ) 
				{
					SBloodSytemInfo sBloodInfo;

					LUA_BEGIN_TABLE_RAII( "DAMAGE_LIST" )
					{
						for( int j = 1; ; ++j ) {
							int iValue = 0;
							if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
								break;
							}

							sBloodInfo.m_vecDamageList.push_back( iValue );
						}                        
					}

					LUA_GET_VALUE_DEF( "BLOOD_AMOUNT", sBloodInfo.m_fBloodChargeNum, 0 );
					LUA_GET_VALUE_DEF( "CHECK_ONLY_ONE_UNIT", sBloodInfo.m_bCheckOnlyOneUnit, false );

					Template.vecBloodSytemInfo.push_back( sBloodInfo );
				} 
				else 
				{
					break;
				}
			}
		}

        LUA_BEGIN_TABLE_RAII( "MarbleChargeMotionList" )
        {
            Template.vecMarbleSytemInfo.clear();
            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {
                    SMarbleSytemInfo sMarbleInfo;
                    LUA_BEGIN_TABLE_RAII( "MOTION_LIST" )
                    {
                        for( int j = 1; ; ++j ) {
                            int iValue = 0;
                            if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                                break;
                            }

                            sMarbleInfo.m_vecMotionList.push_back( iValue );
                        }                        
                    }

                    LUA_BEGIN_TABLE_RAII( "DAMAGE_LIST" )
                    {
                        for( int j = 1; ; ++j ) {
                            int iValue = 0;
                            if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                                break;
                            }

                            sMarbleInfo.m_vecDamageList.push_back( iValue );
                        }                        
                    }

                    LUA_GET_VALUE_DEF( "MARBLE_AMOUNT", sMarbleInfo.m_fMarbleChargeNum, 0 );
                    LUA_GET_VALUE_DEF( "CHECK_ONLY_ONE_UNIT", sMarbleInfo.m_bCheckOnlyOneUnit, false );

                    Template.vecMarbleSytemInfo.push_back( sMarbleInfo );
                } 
                else 
                {
                    break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "ControlCountCoolTimeSkillList" )
        {
            Template.mapControlCountCoolTimeSkillList.clear();
            int iSkilID;
            float fValue;
            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {
                    LUA_GET_VALUE_DEF(1, iSkilID, -1 );
                    LUA_GET_VALUE_DEF(2, fValue, 0.f );

                    Template.mapControlCountCoolTimeSkillList.insert( std::make_pair( iSkilID, fValue ) );
                } 
                else 
                {
                    break;
                }
            }
        }

        char str[MAX_PATH] = {0,};            
        LUA_BEGIN_TABLE_RAII( "CombinationSkillList" )
        {
            Template.mapCombinationSkillList.clear();
            for(int i = 1; ; i++) 
            {   
                SCombinationSkillSetInfo sComboSkillSet;
                sprintf_s( str, "COMBINATION_%d", i );               
                LUA_BEGIN_TABLE_RAII( str ) 
                {
                    LUA_BEGIN_TABLE_RAII( "SKILL_LIST" )
                    {
                        for( int j = 1; ; ++j ) {
                            int iValue = 0;
                            if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                                iValue = -1;
                                break;
                            }
                            if( iValue >= 0 ) {
                                sComboSkillSet.vecSkillList.push_back( std::make_pair( iValue, false ) );
                            }
                        }
                    }
                    LUA_GET_VALUE_DEF( "AUTO_SKILL_ID", sComboSkillSet.prAutoRunSkillInfo.first, -1 );
                    LUA_GET_VALUE_DEF( "NEED_MANA", sComboSkillSet.prAutoRunSkillInfo.second, 0.f );

                    Template.mapCombinationSkillList.insert( std::make_pair( i, sComboSkillSet ) );
                } 
                else 
                {
                    break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "StatisticsMotionList" )
        {
            for( int i = 1; ; ++i ) {
                int iValue = 0;
                if( luaMgr.GetValue( i, iValue ) == E_FAIL ) {
                    break;
                }
                Template.setStatsSkillMotionList.insert( iValue );
            }                        
        }

        LUA_BEGIN_TABLE_RAII( "SklillCommandList" )
        {
            Template.m_mapCommandList.clear();
            for(int i = 0; ; i++) 
            {
                sprintf_s( str, "CommandList%d", i );
                LUA_BEGIN_TABLE_RAII( str )
                {
                    SSkillCommandList sSkillCommandList;
                    LUA_BEGIN_TABLE_RAII( "Name" )
                    {
                        int iNameStringID;
                        int iColorA, iColorR, iColorG, iColorB;
                        LUA_GET_VALUE_DEF( "NameStringID", iNameStringID, -1 );
                        LUA_BEGIN_TABLE_RAII( "NameColor" )
                        {
                            LUA_GET_VALUE_DEF(1, iColorA, 0 );
                            LUA_GET_VALUE_DEF(2, iColorR, 0 );
                            LUA_GET_VALUE_DEF(3, iColorG, 0 );
                            LUA_GET_VALUE_DEF(4, iColorB, 0 );
    }

                        sSkillCommandList.m_iNameStringID = iNameStringID;
                        sSkillCommandList.m_dwColor = D3DCOLOR_ARGB( iColorA, iColorR, iColorG, iColorB );
                    }

                    LUA_BEGIN_TABLE_RAII( "Command" )
                    {
                        for(int j = 1; ; j++) 
                        {
                            SSkillCommand sSkillCommand;
                            LUA_BEGIN_TABLE_RAII( j )
                            {
                                UINT uiMotionID;

                                LUA_GET_VALUE_DEF( 1, sSkillCommand.m_iSkillID, -1 );
                                for(int k = 2; ; k++) 
                                {
                                    if( luaMgr.GetValue( k, uiMotionID ) == E_FAIL ) {
                                        uiMotionID = -1;
                                        break;
                                    }
                                    if (uiMotionID != -1)
                                        sSkillCommand.m_vecMotionID.push_back(uiMotionID);
                                }
                                sSkillCommandList.m_vecCommandList.push_back(sSkillCommand);
                            }
    else
                            {
                                break;
                            }
                        }
                    }
                    Template.m_mapCommandList.insert( std::make_pair( i, sSkillCommandList ) );
                }
                else 
                {
                    break;
                }
            }
        }

        LUA_BEGIN_TABLE_RAII( "TutorialSklillCommandList" )
        {
            Template.m_mapTutorialCommandList.clear();
            for(int i = 0; ; i++) 
            {
                sprintf_s( str, "CommandList%d", i );
                LUA_BEGIN_TABLE_RAII( str )
                {
                    SSkillCommandList sSkillCommandList;
                    LUA_BEGIN_TABLE_RAII( "Name" )
                    {
                        int iNameStringID;
                        int iColorA, iColorR, iColorG, iColorB;
                        LUA_GET_VALUE_DEF( "NameStringID", iNameStringID, -1 );
                        LUA_BEGIN_TABLE_RAII( "NameColor" )
                        {
                            LUA_GET_VALUE_DEF(1, iColorA, 0 );
                            LUA_GET_VALUE_DEF(2, iColorR, 0 );
                            LUA_GET_VALUE_DEF(3, iColorG, 0 );
                            LUA_GET_VALUE_DEF(4, iColorB, 0 );
                        }

                        sSkillCommandList.m_iNameStringID = iNameStringID;
                        sSkillCommandList.m_dwColor = D3DCOLOR_ARGB( iColorA, iColorR, iColorG, iColorB );
                    }

                    LUA_BEGIN_TABLE_RAII( "Command" )
                    {
                        for(int j = 1; ; j++) 
                        {
                            SSkillCommand sSkillCommand;
                            LUA_BEGIN_TABLE_RAII( j )
                            {
                                UINT uiMotionID;

                                LUA_GET_VALUE_DEF( 1, sSkillCommand.m_iSkillID, -1 );
                                for(int k = 2; ; k++) 
                                {
                                    if( luaMgr.GetValue( k, uiMotionID ) == E_FAIL ) {
                                        uiMotionID = -1;
                                        break;
                                    }
                                    if (uiMotionID != -1)
                                        sSkillCommand.m_vecMotionID.push_back(uiMotionID);
                                }
                                sSkillCommandList.m_vecCommandList.push_back(sSkillCommand);
                            }
                            else 
                            {
                                break;
                            }
                        }
                    }
                    Template.m_mapTutorialCommandList.insert( std::make_pair( i, sSkillCommandList ) );
                }
                else 
                {
                    break;
                }
            }
        }
    }
    else
        return false;


    return true;
}

// [8/5/2007 breadceo] ��򰡿��� ���µ� ��Ȯ�ϰ� �� �𸣰ڴ�..
void KGCPlayerCommon::MakeSoundLoadList()
{
    std::vector<int> vecLoadList;
    vecLoadList.push_back( 49 );
    vecLoadList.push_back( 40 );
    vecLoadList.push_back( 0 );
    vecLoadList.push_back( 7 );
    vecLoadList.push_back( 207 );
    vecLoadList.push_back( 222 );
    vecLoadList.push_back( 12 );
    vecLoadList.push_back( 241 );
    vecLoadList.push_back( 4 );
    vecLoadList.push_back( 296 );

    std::for_each( vecLoadList.begin(), vecLoadList.end(),
        boost::bind( &KDSound::PreparetoLoad, boost::ref(g_KDSound), _1 ) );
}

int KGCPlayerCommon::OnElesisCommonCombo(PLAYER* pPlayer, bool bKeyProc)
{
	switch( pPlayer->Extra_Char_Num )
	{
		case CID_ELESIS1:
			return KGCPlayerElesis::OnComboAtk( pPlayer, bKeyProc );
			break;
		case CID_ELESIS2:
			return KGCPlayerElesis2::OnComboAtk( pPlayer, bKeyProc );
			break;
		case CID_ELESIS3:
			return KGCPlayerElesis3::OnComboAtk( pPlayer, bKeyProc );
			break;
		case CID_ELESIS4:
			return KGCPlayerElesis4::OnCombo( pPlayer, bKeyProc );
			break;
	}

	return 0;
}

int KGCPlayerCommon::OnVerticalDashToJump(PLAYER* pPlayer, bool bKeyProc)
{	
	if( !pPlayer->IsContact )
	{
		if( bKeyProc )
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Punch )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
				if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;	
				return 0;
			}
		}	

		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
			switch( pPlayer->Extra_Char_Num )
			{
			case CID_ARME1:
			case CID_ARME2:
				pPlayer->cFrame = 20;	
				break;
			case CID_ARME3:
				pPlayer->cFrame = 16;	
				break;
			case CID_ARME4:
				pPlayer->cFrame = 40;	
				break;
			}		
		}			
	}	
	else
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT);
		pPlayer->cFrame = 0;
	}
	return 0;
}

int KGCPlayerCommon::OnBurnningCharge(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}		

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			g_MyD3D->MyCtrl->k_Down = false;
			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            bool    bIsPlayerAbleToDownStair = g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y);

            if( pPlayer->IsContact && !bIsPlayerAbleToDownStair || pPlayer->vPos.y <= -0.5f){
				return 0;
            }
            if( bIsPlayerAbleToDownStair ){
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
                g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
			}

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 40;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			{
				if( pPlayer->Next_Attack_Count != 255 )
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
				}
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
			}

			pPlayer->NoCheckContact = 0;

			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;

			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			return 0;
		}
		//pPlayer->x_Speed = 0.0f;
	
		if( g_MyD3D->MyCtrl->k_Right )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = true;
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			pPlayer->m_bIsMovedAfterCharge = true;
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = false;
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
	}

	float fBP = 3.0f / ( pPlayer->GetCurFormTemplate().fBPChargeTime * (float)GC_FPS_LIMIT );					
	pPlayer->m_fBurnningPoint += fBP;

	if( pPlayer->m_fBurnningPoint >= 3.0f ) 
	{
		pPlayer->m_fBurnningPoint = 3.0f;
	}

    if (g_MyD3D->MyCtrl->k_Push_Burnning && !pPlayer->m_bIsBurnningMode && pPlayer->m_fBurnningPoint == 3.0f)
    {
        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().BURNNING_ACTIVE);
        pPlayer->ChangeBurnningTemplate();

        pPlayer->cFrame = 0;
        return 0;
    }

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 2 )
		pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - GC_FPS_LIMIT;

    if (!g_MyD3D->MyCtrl->k_Push_Burnning)
    {
        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
        pPlayer->cFrame = 0;
    }

	return 0;
}

int KGCPlayerCommon::OnMotionToChangeBodyAngle( PLAYER* pPlayer, bool bKeyProc )
{
	static int minFrame = 0;
	static int maxFrame = 0;
	float angleMovement = 0.025f;

	switch ( pPlayer->uiMotion )
	{
	case MID_SKILL_LIRE_METALSHOT_LV3:
		minFrame = 27;
		maxFrame = 37;
		break;
	case MID_SKILL_LIRE_SPECIAL2_ATK:
		minFrame = 0;
		maxFrame = 2;
		angleMovement = 0.075f;
		break;
	case MID_SKILL_LIRE_HEADSHOT_LV1:
		minFrame = 0;
		maxFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
		angleMovement = 0.075f;
		break;
	case MID_SKILL_LIRE_HEADSHOT_LV2:
		minFrame = 0;
		maxFrame = 57;
		angleMovement = 0.075f;
		break;
	case MID_SKILL_LIRE_PIERCESHOT_LV1:
	case MID_SKILL_LIRE_PIERCESHOT_LV2:
		minFrame = 20;
		maxFrame = 28;
		angleMovement = 0.075f;
		break;
	case MID_SKILL_LIRE_FIRE_ARROW_LV1:
	case MID_SKILL_LIRE_FIRE_ARROW_LV2:
	case MID_SKILL_LIRE_POISON_ARROW:
	case MID_SKILL_LIRE_FROZEN_ARROW:
		minFrame = 15;
		maxFrame = 21;
		break;
	case MID_SKILL_LIRE_CONTINUOUS_ATK_LV1:
	case MID_SKILL_LIRE_CONTINUOUS_ATK_LV2:
		minFrame = 21;
		maxFrame = 86;
		break;
	default:
		minFrame = 0;
		maxFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
		angleMovement = 0.025f;
		break;
	}

	if(bKeyProc)
	{
		//====================================================================================
		// 2009.02.03 : Jemitgge
		// Comment : Ư�� ��� Ư�� ������ �߿� ���� ���� �ٲ� �� �ְ�!!
		if( pPlayer->cFrame >= minFrame && pPlayer->cFrame < maxFrame )
		{
			if ( angleMovement <= 0.025f && pPlayer->Slow_Count >= 17 )
			{
				return 0;
			}
			
			//if( diks[DIK_UP] )
			if( diks[g_MyD3D->MyCtrl->m_keyUp] )
			{
				pPlayer->Body_Angle += angleMovement;
				if(pPlayer->Body_Angle > 1.57f) 
					pPlayer->Body_Angle=1.57f;
			}
			//else if( diks[DIK_DOWN] )
			else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->Body_Angle -= angleMovement;
				if( pPlayer->Body_Angle < -0.8f ) 
					pPlayer->Body_Angle = -0.8f;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int KGCPlayerCommon::OnMotionToJumpEnd( PLAYER* pPlayer, bool bKeyProc )
{
	//====================================================================================
    // 2009.02.03 : Jemitgge
    // Comment : � ����� ������ jump������� ���ư���, ��κ� jump�� ������ 
	//			 ���������� ���ư��ϴ�. �װ� ���ִ� ��!! 
	if(bKeyProc)
		return 0;

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)
	{
		pPlayer->Direct_Motion_Input( pPlayer->GetCurFormTemplate().JUMP );
		pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() -1;
		if( pPlayer->y_Speed >= 0.0f )
		{
			pPlayer->y_Speed = 0.015f;
		}
	}

	return 0;
}

int KGCPlayerCommon::OnContinueousCombo( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->Next_Attack_Count > 0 )
			{
				pPlayer->Next_Attack_Count = 255;
				return 0;
			}
		}
	}

	return 0;
}

int KGCPlayerCommon::OnPositionFix( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame == 0 )
    {
        pPlayer->x_Speed = 0.0f;
    }

    return KGCPlayerCommon::OnEmpty( pPlayer , bKeyProc );
}

int KGCPlayerCommon::DashJumpFunc( PLAYER* pPlayer, int iStateID )
{
    if( pPlayer->m_iJumpActionLimitCount > 4 )
        return 0;

    if( !iStateID )
        iStateID = pPlayer->GetCurFormTemplate().JUMP;
    
    if( !pPlayer->SetPlayerState(iStateID) )
        return 0;
    
    pPlayer->m_iJumpActionLimitCount++;
    if(pPlayer->bIsRight)
        pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
    else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
    
    pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

    pPlayer->cFrame = 0;
    g_MyD3D->MyCtrl->k_Up = false;
    return 0;
}

int KGCPlayerCommon::DoubleJumpFunc( PLAYER* pPlayer, int iStateID )
{

    if( pPlayer->m_iJumpActionLimitCount > 4 )
        return 0;
    
    if( !iStateID )
        iStateID = pPlayer->GetCurFormTemplate().DOUBLE_JUMP;

    if( !pPlayer->SetPlayerState(iStateID) )
        return 0;

    pPlayer->m_iJumpActionLimitCount++;
    if(pPlayer->bIsRight)
        pPlayer->x_Speed += pPlayer->GetCurFormTemplate().fDoubleJumpXSpeed;
    else pPlayer->x_Speed -= pPlayer->GetCurFormTemplate().fDoubleJumpXSpeed;

    pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDoubleJumpYSpeed;

    pPlayer->cFrame = 0;
    g_MyD3D->MyCtrl->k_Up = false;
    return 0;
}

int KGCPlayerCommon::OnEmpty( PLAYER* pPlayer, bool bKeyProc )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();

	lua_checkstack( L, 20 );					// ���� ����� üũ �մϴ�. ( 20���� ������ ���������� Ȯ�� �մϴ�. )
	lua_pushstring( L, "StateFunc" );		// Player_Action ���̺��� �ҷ����� ���� �̸��� Push �մϴ�.
	lua_rawget( L, LUA_GLOBALSINDEX );			// �۷ι� ���̺��� Player_Action Ű�� ã���ϴ�.
	lua_pushinteger( L, pPlayer->uiMotion );			// Player_Action�� Ű���� �ֽ��ϴ�.    
	lua_rawget( L, -2 );						// Player_Action���̺��� Ű���� �ش��ϴ� �κ��� ã���ϴ�.
	if( lua_isfunction(L, -1) )	// ã������ Function�� �ƴϸ� �پ� �ѽ��ϴ�.
	{
        lua_pushboolean( L, bKeyProc );
        lua_pushinteger( L, pPlayer->m_iPlayerIndex );
		if( 0 != lua_pcall( L, 2, -1, 0 ) )
        {
#if !defined(__PATH__)
            printf( "%s\n", lua_tostring( L, -1 ) );
#endif
            lua_pop( L, -1 );
        }
	}
	else
	{
		lua_pop( L, -1 );		// Ű�� ã�� ���� Pop �մϴ�.        
        lua_pop( L, -1 );		// Ű�� ã�� ���� Pop �մϴ�.        
	}

    return 0;

}

int KGCPlayerCommon::OnSavingStance( PLAYER* pPlayer, bool bKeyProc )
{
    if(bKeyProc) {
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) {
            pPlayer->SetIsRight( true );
            pPlayer->Change_Motion( pPlayer->GetCurFormTemplate().SAVING_EVASION );
            return 0;
        }else if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash ) {
            pPlayer->SetIsRight( false );
            pPlayer->Change_Motion( pPlayer->GetCurFormTemplate().SAVING_EVASION );
            return 0;
        }

        // ���̺� ������ �� ����...
        //if( g_MyD3D->MyCtrl->k_Fresh_Punch ) {
        //    pPlayer->Change_Motion( pPlayer->GetCurFormTemplate().SAVING_ATTACK );
        //    return 0;
        //}
    }
    return 0;
}

void KGCPlayerCommon::RegisterLuaBind()
{

    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnJump", &KGCPlayerCommon::OnJump );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnDownAndStandUp", &KGCPlayerCommon::OnDownAndStandUp );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnFrontDownAndStandUp", &KGCPlayerCommon::OnFrontDownAndStandUp );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnStoneCurse", &KGCPlayerCommon::OnStoneCurse );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnDash", &KGCPlayerCommon::OnDash );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnWalk", &KGCPlayerCommon::OnWalk );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnRun", &KGCPlayerCommon::OnRun );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnBurnningCharge", &KGCPlayerCommon::OnBurnningCharge );
    
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnWait", &KGCPlayerCommon::OnWait );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnPortal", &KGCPlayerCommon::OnPortal );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnWaitPortal", &KGCPlayerCommon::OnWaitPortal );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "OnIntoPortal", &KGCPlayerCommon::OnIntoPortal );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "CheckPortal", &KGCPlayerCommon::CheckPortal );
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "DashJumpFunc", &KGCPlayerCommon::DashJumpFunc );
 
}
