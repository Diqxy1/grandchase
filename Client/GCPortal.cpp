#include "stdafx.h"
//
#include "MyD3D.h"
#include "GCPortal.h"






#include "GCCameraOrtho.h"
#include "./GCUI/GCGameOverlayUI.h"
#include "./GCUI/GCAddtionGuide.h"

#include <QuestGameManager.h>


int KGCPortal::portal_countdown;
int KGCPortal::framecount;
std::set<KGCPortal*> KGCPortal::portal_set;

KGCPortal::KGCPortal(void)
{
	m_pPortal = NULL;
	m_bCrashCheck = false;
	m_bEnter = false;
	portal_set.insert(this);

	m_type = 2;

	m_pParticle1 = NULL;
	m_pParticle2 = NULL;
	m_pParticle3 = NULL;
	m_pParticle4 = NULL;
	m_pParticle5 = NULL;
	m_pParticle6 = NULL;

	m_pCursorParticle1 = NULL;
	m_pCursorParticle2 = NULL;
	m_pCursorParticle3 = NULL;
	m_pCursorParticle4 = NULL;


}

KGCPortal::~KGCPortal(void)
{
	// empty
	Destroy();
	portal_set.erase(this);
}

void KGCPortal::Create(void)
{
	KGCDoor::Create();

	m_pPortal = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence("GameObjectBase"));

	D3DXVECTOR2 pos = D3DXVECTOR2(GetPosX(), GetPosY());

	m_pPortal->GetMatrix()->Move(pos.x - 1.15f, pos.y - 0.43f, 42.0f);
	m_pPortal->GetMatrix()->Scale(0.0028f, 0.0028f, 1.0f);
	m_pPortal->Start(CKTDGAnim::PT_LOOP);
	m_pPortal->SetNowFrame(m_type);
	m_pPortal->Wait();
	m_pPortal->SetShow(true);

	m_pDirectionArrow->SetPosition(
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() + 0.5f, 3.0f);

	m_pParticle1 = g_ParticleManager->CreateSequence("Portal_Zone01",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);
	m_pParticle2 = g_ParticleManager->CreateSequence("Portal_Zone02",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);
	m_pParticle3 = g_ParticleManager->CreateSequence("Portal_Zone03",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);
	m_pParticle4 = g_ParticleManager->CreateSequence("Portal_Zone04",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);
	m_pParticle5 = g_ParticleManager->CreateSequence("Portal_Zone05",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);
	m_pParticle6 = g_ParticleManager->CreateSequence("Portal_Zone06",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 3.9f);

	m_pCursorParticle1 = g_ParticleManager->CreateSequence("cursor01",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.12f, 1.9f);
	m_pCursorParticle2 = g_ParticleManager->CreateSequence("cursor02",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.12f, 1.9f);
	m_pCursorParticle3 = g_ParticleManager->CreateSequence("cursor03",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.12f, 1.9f);
#ifndef NATION_BRAZIL
	m_pCursorParticle4 = g_ParticleManager->CreateSequence("cursor04",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.12f, 1.9f);
#endif

	m_pParticle1->SetShow(false);
	m_pParticle2->SetShow(false);
	m_pParticle3->SetShow(false);
	m_pParticle4->SetShow(false);
	m_pParticle5->SetShow(false);
	m_pParticle6->SetShow(false);

	m_pCursorParticle1->SetShow(false);
	m_pCursorParticle2->SetShow(false);
	m_pCursorParticle3->SetShow(false);
#ifndef NATION_BRAZIL
	m_pCursorParticle4->SetShow(false);
#endif

	g_KDSound.LoadFromIndex(609);

	SetWidth(0);
	SetHeight(0);

	InitTraceIcon();
}

void KGCPortal::Destroy(void)
{
	KGCDoor::Destroy();

	g_KDSound.Free(609);

	m_vecUser.clear();
	if (g_MyD3D->m_pMapAnim)
	{
		g_MyD3D->m_pMapAnim->DeleteInst(m_pPortal);
	}

	if (g_ParticleManager)
	{
		g_ParticleManager->DeleteSequence(m_pParticle1);
		g_ParticleManager->DeleteSequence(m_pParticle2);
		g_ParticleManager->DeleteSequence(m_pParticle3);
		g_ParticleManager->DeleteSequence(m_pParticle4);
		g_ParticleManager->DeleteSequence(m_pParticle5);
		g_ParticleManager->DeleteSequence(m_pParticle6);

		g_ParticleManager->DeleteSequence(m_pCursorParticle1);
		g_ParticleManager->DeleteSequence(m_pCursorParticle2);
		g_ParticleManager->DeleteSequence(m_pCursorParticle3);
#ifndef NATION_BRAZIL
		g_ParticleManager->DeleteSequence(m_pCursorParticle4);
#endif
	}

	m_pParticle1 = NULL;
	m_pParticle2 = NULL;
	m_pParticle3 = NULL;
	m_pParticle4 = NULL;
	m_pParticle5 = NULL;
	m_pParticle6 = NULL;
	m_pPortal = NULL;

	m_pCursorParticle1 = NULL;
	m_pCursorParticle2 = NULL;
	m_pCursorParticle3 = NULL;
	m_pCursorParticle4 = NULL;
}

void KGCPortal::Enable(void)
{
	KGCDoor::Enable();
}


KGCPortal* KGCPortal::CheckPortal(PLAYER* player)
{
	for (set<KGCPortal*>::iterator i = portal_set.begin(); i != portal_set.end(); i++)
	{
		if ((*i)->IsLive())
		{
			GCCollisionRect<float> rect;
			GCCollisionRect<float> player_rect = player->GetCollisionRect();
			player_rect.m_Top = (player_rect.m_Bottom + player_rect.m_Top) * 0.5f;

			if (player_rect.CheckCollision((*i)->GetCollision(), &rect))
			{
				return (*i);
			}
		}
	}
	return NULL;
}


bool KGCPortal::CheckOpen()
{
	for (set<KGCPortal*>::iterator i = portal_set.begin(); i != portal_set.end(); i++)
	{
		if ((*i)->GetDoorState() == IDS_OPEN)
			return true;

		if ((*i)->GetCurHit() >= g_MyD3D->GetLivePlayerNum())
			return true;
	}
	return false;
}


void KGCPortal::UpdatePortal()
{
	const int player_num = g_MyD3D->GetLivePlayerNum();
	int sum_hit = 0;
	int max_hit = 0;
	KGCPortal* max_portal = NULL;
	KGCPortal* host_portal = NULL;
	bool live = false;

	if (player_num <= 0)
		return;


	for (set<KGCPortal*>::iterator i = portal_set.begin(); i != portal_set.end(); i++)
	{
		if ((*i)->IsLive())
			live = true;

		sum_hit += (*i)->GetCurHit();
		if ((*i)->GetCurHit() > max_hit)
		{
			max_hit = (*i)->GetCurHit();
			max_portal = (*i);
		}

		if (g_kGlobalValue.m_kUserInfo.bHost)
		{
			PLAYER* player = g_MyD3D->GetMyPlayer();

			GCCollisionRect<float> rect;
			GCCollisionRect<float> player_rect = player->GetCollisionRect();
			player_rect.m_Top = (player_rect.m_Bottom + player_rect.m_Top) * 0.5f;

			if (player->uiMotion == player->GetOwnMotion(MID_COMMON_WAIT_TELEPORT) &&
				player_rect.CheckCollision((*i)->GetCollision(), &rect))
			{
				host_portal = (*i);
			}
		}

	}

	if (live)
	{
		int overhalf;
		switch (player_num)
		{
		case 1:
			overhalf = 1;
			break;
		case 2:
			overhalf = 1;
			break;
		case 3:
			overhalf = 2;
			break;
		case 4:
			overhalf = 2;
			break;
		default:
			assert(!"플레이어가 1~4명이 아니라니 어쩌라고");
			break;
		}
		if (sum_hit >= overhalf)
		{
			if (portal_countdown == -1)
			{
				portal_countdown = 6;
				framecount = 55;
			}
		}
		else
		{
			portal_countdown = -1;
			g_pkGameOverlayUI->StartNumberCount(-1);
			framecount = 0;
		}

		if (portal_countdown != -1)
		{
			framecount++;
			if (framecount > 55)
			{
				framecount = 0;
				portal_countdown--;

				if (portal_countdown == 5)
				{
					g_pkGameOverlayUI->StartNumberCount(5);
				}
				else if (portal_countdown == 0)
				{
					if (g_kGlobalValue.m_kUserInfo.bHost)
					{
						if (max_portal)
						{
							if (host_portal && host_portal->GetCurHit() == max_portal->GetCurHit())
							{
								host_portal->SetDoorState(KGCDoor::IDS_OPEN);
								g_pkGameOverlayUI->StartNumberCount(-1);
								portal_countdown = -1;
							}
							else
							{
								max_portal->SetDoorState(KGCDoor::IDS_OPEN);
								g_pkGameOverlayUI->StartNumberCount(-1);
								portal_countdown = -1;
							}
						}
						else if (host_portal)
						{
							host_portal->SetDoorState(KGCDoor::IDS_OPEN);
							g_pkGameOverlayUI->StartNumberCount(-1);
							portal_countdown = -1;
						}
						else
						{
							assert(!"맥스 포탈도 없고 호스트 포탈도 없는데 어쩌라고 !!!");
						}
					}
				}

			}
		}

		if (g_kGlobalValue.m_kUserInfo.bHost)
		{
			if (player_num == max_hit)
			{
				max_portal->SetDoorState(KGCDoor::IDS_OPEN);
				portal_countdown = -1;
				g_pkGameOverlayUI->StartNumberCount(-1);
			}
		}
	}

}



void KGCPortal::FrameMove(float fElapsedTime /* = GC_ELAPSED_TIME */)
{
	m_vecUser.clear();
	KGCDoor::FrameMove(fElapsedTime);

	bool show = !(!IsLive() && (IDS_NORMAL == m_enDoorState));
	//if( g_pkQuestManager->IsStageLoadComplete() && IsLive() == false )
	//    show = false;
	m_pParticle1->SetShow(show);
	m_pParticle2->SetShow(show);
	m_pParticle3->SetShow(show);
	m_pParticle4->SetShow(show);
	m_pParticle5->SetShow(show);
	m_pParticle6->SetShow(show);

	m_pCursorParticle1->SetShow(false);
	m_pCursorParticle2->SetShow(false);
	m_pCursorParticle3->SetShow(false);
#ifndef NATION_BRAZIL
	m_pCursorParticle4->SetShow(false);
#endif

	int c = 0;
	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && 0 != g_MyD3D->MyPlayer[i]->m_cLife)
		{
			GCCollisionRect<float> rect;
			GCCollisionRect<float> player_rect = g_MyD3D->MyPlayer[i]->GetCollisionRect();
			player_rect.m_Top = (player_rect.m_Bottom + player_rect.m_Top) * 0.5f;

			if (player_rect.CheckCollision(GetCollision(), &rect))
			{
				if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
				{
					m_pCursorParticle1->SetShow(show);
					m_pCursorParticle2->SetShow(show);
					m_pCursorParticle3->SetShow(show);
#ifndef NATION_BRAZIL
					m_pCursorParticle4->SetShow(show);
#endif
				}

				//if(g_MyD3D->MyPlayer[i]->uiMotion==g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_WAIT_TELEPORT))
				if (g_MyD3D->GetPlayer(i)->uiMotion == g_MyD3D->GetPlayer(i)->GetOwnMotion(MID_COMMON_WAIT_TELEPORT))
				{
					c++;
					m_vecUser.push_back(i);
				}
				else if (g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_INTO_TELEPORT) &&
					(g_MyD3D->MyPlayer[i]->cFrame == 1 || g_MyD3D->MyPlayer[i]->cFrame == 70))
				{
					// Sound & Effect 
					g_KDSound.Play("609");
					ShowEffect();
				}
			}
		}
	}
	SetCurHit(c);

	AllPortalTraceIcon();
}


void KGCPortal::Render(void)
{
	m_pPortal->SetShow(true);
	m_pPortal->Render();
	m_pPortal->SetShow(false);

	if (!m_vecUser.empty())
	{

		D3DXVECTOR3 vPos;
		D3DVIEWPORT9  viewport;
		D3DXMATRIX   projection, view, world;
		D3DXMatrixIdentity(&world);
		g_pd3dDevice->GetViewport(&viewport);
		g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &projection);
		g_pd3dDevice->GetTransform(D3DTS_VIEW, &view);


		g_pGCDeviceManager2->PushState();
		{
			g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

			D3DXVECTOR3 pos = m_pPortal->GetMatrix()->GetPos();
			pos.x += 0.03f;
			pos.y += 0.01f;
			D3DXVec3Project(&vPos, &pos,
				&viewport, &projection, &view, &g_kCamera.m_matCamera);

			for (int i = 0; i < (int)m_vecUser.size(); i++)
			{
				vPos.y += 16;
				g_MyD3D->GetPlayer(m_vecUser[i])->Draw_UserNickNameInPortal((int)(vPos.x + 50), (int)vPos.y);
			}
		}
		g_pGCDeviceManager2->PopState();

	}

}

GCCollisionRect<float> KGCPortal::GetCollision(void)
{
	GCCollisionRect<float> kCollisionRect;

	float fHeight = m_pPortal->GetRealHeight() * 0.01f;
	float fWidth = m_pPortal->GetRealWidth() * 0.01f;

	float fX = m_pPortal->GetMatrix()->GetPos().x;
	float fY = m_pPortal->GetMatrix()->GetPos().y;

	kCollisionRect.m_Top = fY + fHeight + 0.2f;
	kCollisionRect.m_Bottom = fY + 0.2f;

	kCollisionRect.m_Left = fX - (fWidth);
	kCollisionRect.m_Right = fX + (fWidth);


	return kCollisionRect;
}

GCCollisionRect<float> KGCPortal::GetCollision2(void)
{
	GCCollisionRect<float> kCollisionRect;

	float fHeight = m_pPortal->GetRealHeight() * 0.05f;
	float fWidth = m_pPortal->GetRealWidth() * 0.05f;

	float fX = m_pPortal->GetMatrix()->GetPos().x;
	float fY = m_pPortal->GetMatrix()->GetPos().y;

	kCollisionRect.m_Top = fY + fHeight + 0.2f;
	kCollisionRect.m_Bottom = fY + 0.2f;

	kCollisionRect.m_Left = fX - (fWidth);
	kCollisionRect.m_Right = fX + (fWidth);


	return kCollisionRect;
}


bool KGCPortal::IsCrashCheck(void)
{
	return false;
}

void KGCPortal::SetDoorState(KGCDoor::IRON_DOOR_STATE enState)
{
	if (GetDoorState() == IDS_OPEN)
		return;

	KGCDoor::SetDoorState(enState);

	if (g_kGlobalValue.m_kUserInfo.bHost)
	{
		SendPacketCurDoorState();
	}
}

void KGCPortal::ShowEffect()
{
	g_ParticleManager->CreateSequence("Portal_Zone_Effect01",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 1.9f);
	g_ParticleManager->CreateSequence("Portal_Zone_Effect02",
		m_pPortal->GetMatrix()->GetXPos(),
		m_pPortal->GetMatrix()->GetYPos() - 0.02f, 1.9f);
}

bool KGCPortal::IsPortalInScreen()
{
	FRect rt = g_kCamera.m_rtCamera;
	rt.top += g_kCamera.m_fLookTermY;
	rt.bottom += g_kCamera.m_fLookTermY;

	for (set<KGCPortal*>::iterator i = portal_set.begin(); i != portal_set.end(); i++)
	{
		if ((*i)->IsLive())
		{
			GCCollisionRect<float> rect = (*i)->GetCollision();

			if (rect.CheckCollision(rt))
				return true;
		}
	}

	return false;
}

void KGCPortal::ProcessTraceIcon()
{
	m_vTraceIconPos = m_pPortal->GetMatrix()->GetPos();
	FRect rt = g_kCamera.m_rtCamera;
	rt.bottom += g_kCamera.m_fLookTermY;
	rt.top = rt.bottom + (rt.top - rt.bottom) * 0.8f;
	rt.AdjustPtInRect(m_vTraceIconPos.x, m_vTraceIconPos.y, 0.09f);

	D3DXVECTOR2 vDIrection = m_vTraceIconPos - m_pPortal->GetMatrix()->GetPos();

	D3DXVec2Normalize(&vDIrection, &vDIrection);

	float fAngle = acos(vDIrection.x);

	if (vDIrection.y < 0.0f)
	{
		fAngle = (D3DX_PI - fAngle) + D3DX_PI;
		fAngle += D3DX_PI / 2.0f;
	}
	else
	{
		fAngle += D3DX_PI / 2.0f;

	}


	for (unsigned int i = 0; i < m_vecTraceIcon.size(); i++)
	{
		if (IsPortalInScreen() == false && g_pkQuestManager->m_bPortalInScreen == false)
			m_vecTraceIcon[i]->SetShow(true);
		else
			m_vecTraceIcon[i]->SetShow(false);
		m_vecTraceIcon[i]->SetBeginPos(m_vTraceIconPos);
		if (i == 1)
			m_vecTraceIcon[i]->SetAdditinalZRot(fAngle);

	}
}

void KGCPortal::AllPortalTraceIcon()
{
	for (set<KGCPortal*>::iterator i = portal_set.begin(); i != portal_set.end(); i++)
	{
		if ((*i)->IsLive())
		{
			(*i)->ProcessTraceIcon();
		}
	}
}

void KGCPortal::InitTraceIcon()
{
	if (m_vecTraceIcon.empty())
	{
		TRACE_INFO trace;
		trace.m_fParticleTraceTime = 5.0f;
		trace.m_pvPos = &m_vTraceIconPos;

		CParticleEventSeqPTR pSeq = g_ParticleManager->CreateSequence("P001", 0.0f, 0.0f, 0.0f);
		if (pSeq != NULL) {
			pSeq->SetTrace(&trace);
			pSeq->SetBeginPos(m_vTraceIconPos);
			pSeq->SetShow(false);
			m_vecTraceIcon.push_back(pSeq);
		}

		if (pSeq != NULL) {
			pSeq = g_ParticleManager->CreateSequence("P002", 0.0f, 0.0f, 0.0f);
			pSeq->SetTrace(&trace);
			pSeq->SetShow(false);
			m_vecTraceIcon.push_back(pSeq);
		}

        if (pSeq != NULL){
            pSeq = g_ParticleManager->CreateSequence( "P003", 0.0f, 0.0f, 0.0f );
            pSeq->SetTrace( &trace );
            pSeq->SetBeginPos( m_vTraceIconPos);
            pSeq->SetShow( false );
            m_vecTraceIcon.push_back( pSeq );
        }
        
    }
}