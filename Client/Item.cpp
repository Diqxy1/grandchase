#include "stdafx.h"
//

#include "Item.h"
#include "Replay.h"
#include "Stage.h"

#include "GCCameraOrtho.h"
#include "KGCGameBoard.h"

//
#include "GCEventManager.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"
#include "KGCEmbarkManager.h"

Items::Items()
{
	Selected_Item = -1;
	Item_Cursor = -1;
	Clear_TransNPC();

	MyItm = NULL;
	Clear_All();

	m_aiItemNameID[GC_GAME_ITEM_HPUP] = STR_ID_ITEM_NAME_HPUP;
	m_aiItemNameID[GC_GAME_ITEM_SUPER] = STR_ID_ITEM_NAME_SUPER;
	m_aiItemNameID[GC_GAME_ITEM_HASTE] = STR_ID_ITEM_NAME_HASTE;
	m_aiItemNameID[GC_GAME_ITEM_WING] = STR_ID_ITEM_NAME_WING;
	m_aiItemNameID[GC_GAME_ITEM_HALLOW] = STR_ID_ITEM_NAME_HALLOW;
	m_aiItemNameID[GC_GAME_ITEM_REFLECTION] = STR_ID_ITEM_NAME_REFLECTION;
	m_aiItemNameID[GC_GAME_ITEM_MPUP] = STR_ID_ITEM_NAME_MPUP;
	m_aiItemNameID[GC_GAME_ITEM_GP] = STR_ID_ITEM_NAME_GP;
	m_aiItemNameID[GC_GAME_ITEM_BONUS] = STR_ID_ITEM_NAME_BONUS;
	m_aiItemNameID[GC_GAME_ITEM_GRAPE] = STR_ID_ITEM_NAME_GRAPE;
	m_aiItemNameID[GC_GAME_ITEM_CHICKEN] = STR_ID_ITEM_NAME_CHICKEN;
	m_aiItemNameID[GC_GAME_ITEM_WATERMELON] = STR_ID_ITEM_NAME_WATERMELON;
	m_aiItemNameID[GC_GAME_ITEM_SANTA_GIFT] = STR_ID_ITEM_NAME_SANTA_GIFT;
	//m_aiItemNameID[GC_GAME_ITEM_RICECAKE  ] = STR_ID_ITEM_NAME_RICECAKE;
	//m_aiItemNameID[GC_GAME_ITEM_EGGCOOKIE ] = STR_ID_ITEM_NAME_EGGCOOKIE;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_HPUP] = STR_ID_ITEM_NAME_HPUP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_FULLHPUP] = STR_ID_ITEM_NAME_FULL_HP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_CRITICAL] = STR_ID_ITEM_NAME_CRITICAL;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_NPC_ATTACKUP] = STR_ID_ITEM_NAME_NPC_ATTACKUP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_NPC_DEFANCEUP] = STR_ID_ITEM_NAME_NPC_DEFANCEUP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_NPC_HPUP] = STR_ID_ITEM_NAME_NPC_HPUP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_NPC_SPEEDUP] = STR_ID_ITEM_NAME_NPC_SPEEDUP;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_MOVE_ARENA] = STR_ID_ITEM_DOTA_MOVE_ARENA;
	m_aiItemNameID[GC_GAME_ITEM_DOTA_ATK] = STR_ID_ATK_RAISE;
	m_aiItemNameID[GC_GAME_ITEM_BUFF_MONSPEEDDOWN] = STR_ID_ITEM_MONSPEED;
	m_aiItemNameID[GC_GAME_ITEM_BUFF_CLEAR_DEBUFF] = STR_ID_ITEM_CLEARDEBUFF;
	m_aiItemNameID[GC_GAME_ITEM_BUFF_ATKUP] = STR_ID_ITEM_ATKUP;
	m_aiItemNameID[GC_GAME_ITEM_BUFF_MOVESPEEDUP] = STR_ID_ITEM_MOVESPEEDUP;
	m_aiItemNameID[GC_GAME_ITEM_DAMAGE_MISSILETURRET] = STR_ID_ITEM_TURRET;
	m_aiItemNameID[GC_GAME_ITEM_BUFF_JUMPUP] = STR_ID_ITEM_BUFF_JUMPUP;
	m_aiItemNameID[GC_GAME_ITEM_DAMAGE_ICE] = STR_ID_ITEM_DAMAGE_ICE;
	m_aiItemNameID[GC_GAME_ITEM_DAMAGE_SPECIAL] = STR_ID_ITEM_SPECIAL_SKILL;

	for (int i = 0; i < GC_GAME_ITEM_NUM; ++i)
		m_pTexItem[i] = NULL;
}

Items::~Items()
{
	for (int i = 0; i < GC_GAME_ITEM_NUM; ++i)
		SAFE_RELEASE(m_pTexItem[i]);

	SAFE_RELEASE(m_pTexLetter);
	SAFE_DELETE_ARRAY(MyItm);
}

void Items::Clear_All()
{
	Item* pTemp = MyItm;
	MyItm = NULL;
	MyItm = new Item[MAX_NUM_ITEM_IN_MAP];
	SAFE_DELETE_ARRAY(pTemp);
}

void Items::Clear_TransNPC()
{
	TransNPC.What = 0;
	TransNPC.Action = 0;
	TransNPC.IsRight = 0;
	TransNPC.HP = 0;
	TransNPC.x = 0;
	TransNPC.y = 0;
}

void Items::InitDevice()
{
	char szFileName[30];

	//    아이템 텍스쳐 로드
	for (int i = 0; i < GC_GAME_ITEM_NUM; ++i)
	{
		if (i == GC_GAME_ITEM_MATCH_GP) //똑같은 금화 모양이나 다른 아이템.. 코드 지저분해 이거 ㅠ
		{
			sprintf(szFileName, "Item%02d.dds", GC_GAME_ITEM_GP);
			m_pTexItem[i] = g_pGCDeviceManager2->CreateTexture(szFileName);
		}
		else
		{
			sprintf(szFileName, "Item%02d.dds", i);
			m_pTexItem[i] = g_pGCDeviceManager2->CreateTexture(szFileName);
		}
	}

	V_Item[0].SetSLVER(0, .1f, 0, 0, 0);
	V_Item[1].SetSLVER(.1f, .1f, 0, 1.0f, 0);
	V_Item[2].SetSLVER(0, 0, 0, 0, 1.0f);
	V_Item[3].SetSLVER(.1f, 0, 0, 1.0f, 1.0f);

	m_pTexLetter = g_pGCDeviceManager2->CreateTexture("letter.dds");
	V_Letter[0].SetSLVER(-.1f, .1f, 0, 0, 0);
	V_Letter[1].SetSLVER(.1f, .1f, 0, 0, 0);
	V_Letter[2].SetSLVER(-.1f, -.1f, 0, 0, 0);
	V_Letter[3].SetSLVER(.1f, -.1f, 0, 0, 0);
}

//void Items::OnResetDevice()
//{
//
//}

void Items::DeleteDevice()
{
	m_pTexLetter = NULL;
}

void Items::Frame_Move()
{
	D3DXVECTOR3 Vec;
	unsigned char Me = g_MyD3D->Get_MyPlayer();

	if (GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory())
		goto SELECT_ITEM;

	if (false == SiKGCWorldMapManager()->IsUseGameItem(SiKGCRoomManager()->GetGameMode()))
		goto SELECT_ITEM;

	int i, ii;
	static short o = 0;
	++o;
	static float sfMaxY = 0; //아이템 움직임 최고점

	if (o > LATENCY - 1)
	{
		if (g_kGlobalValue.m_kUserInfo.bHost &&
			SiKGCRoomManager()->GetSubGameMode() != GC_SGM_NO_ITEM &&
			SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_INDIGO &&
			SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH &&
			SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && // 던전에서는 기존의 아이템 안나온다
			SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_CRUSADER &&
			SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_HUNT)
		{
			static short o2 = 0;
			++o2;

			if (o2 > (short)(500 - g_MyD3D->Num_Alive * 30))
			{
				o2 = 0;
				D3DXVECTOR2 Vec;
				Vec = g_MyD3D->MyStg->Get_Random_XY();
				Vec.y += 0.3f;

				char What;

				// 퀘스트에서는 헤이스트 아이템 즐~ (죄송..^^;)
				if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_UNION_FIGHT)
					What = rand() % GC_GAME_ITEM_HASTE;//7     // 헤이스트를 제외한 것들..
				else
					What = rand() % GC_GAME_ITEM_BONUS;//8     // 일반적으로 떨어지는 아이템들..

				// 대만 송편 이벤트.. 떨어진 아이템이 송편으로 보인다
				if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_RICECAKE) == true)
				{
					// GP를 제외한 아이템을..
					if (What != GC_GAME_ITEM_GP)
					{
						What = GC_GAME_ITEM_RICECAKE; // 송편으로 바꿔버린다!
					}
				}

				Add(What, Vec);
			}
		}
		o = 0;
	}

	// 아이템과 플레이어와 충돌 검사를 수행한다.(호스트에서만 검사) --->
	for (i = 0; i < MAX_NUM_ITEM_IN_MAP; i++)
	{
		if (MyItm[i].IsEnable)
		{
			for (ii = 0; ii < MAX_PLAYER_NUM; ii++)
			{
				if (g_MyD3D->MyPlayer[ii]->m_cLife &&
					g_MyD3D->MyPlayer[ii]->uiMotion != g_MyD3D->MyPlayer[ii]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) &&
					g_MyD3D->MyPlayer[ii]->uiMotion != g_MyD3D->MyPlayer[ii]->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))
				{
					bool bCollision = false;

					if (UINT_MAX > g_MyD3D->MyPlayer[ii]->GetEmbarkUID())
					{
						KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(g_MyD3D->MyPlayer[ii]->GetEmbarkUID());
						if (pEmbark)
						{
							GCCollisionRect<float> kEmbarkRect = pEmbark->GetPushRect();
							GCCollisionRect<float> kItemRect, kColl;
							kItemRect.SetRect(((MyItm[i].x - 1.0f) - 0.1f), (MyItm[i].y + 0.32f - 0.5f), ((MyItm[i].x - 1.0f) + 0.1f), (MyItm[i].y - 0.5f));

							if (kEmbarkRect.CheckCollision(kItemRect, &kColl))
								bCollision = true;
						}
					}
					else if (g_MyD3D->MyPlayer[ii]->vPos.x - .1f<MyItm[i].x + .02f &&
						g_MyD3D->MyPlayer[ii]->vPos.x + .1f>MyItm[i].x + .02f &&
						g_MyD3D->MyPlayer[ii]->vPos.y + g_MyD3D->MyPlayer[ii]->y_Ex<MyItm[i].y + .32f &&
						g_MyD3D->MyPlayer[ii]->vPos.y + g_MyD3D->MyPlayer[ii]->y_Ex + .3f>MyItm[i].y + .32f)
					{
						bCollision = true;
					}

					if (bCollision)
					{
						MyItm[i].IsEnable = FALSE;

						if (MyItm[i].What == GC_GAME_ITEM_GP)
						{
							g_MyD3D->MyPlayer[ii]->Item_Score++;
						}
						else if (MyItm[i].What == GC_GAME_ITEM_MATCH_GP)
						{
#if NATION_CHINA
							float fRatio = g_MyD3D->MyPlayer[ii]->GetFatigueRatio() / 100.f;
							fRatio = fRatio * MyItm[i].iMatchGP;
							MyItm[i].iMatchGP = static_cast<int>(fRatio);
#endif

							if (MyItm[i].Time < 4 * GC_FPS_LIMIT) //4초 지나기 전에는 주인만 먹을 수 있다.
							{
								if (MyItm[i].cItemLord == ii)
								{
									CatchMatchCoin(i, ii);
									MyItm[i].bRenderMatchGP = true;
								}
								else
								{
									MyItm[i].IsEnable = TRUE;
								}
							}
							else //4초 이후에는 누구나 먹을 수 있다.
							{
								CatchMatchCoin(i, ii);
								MyItm[i].bRenderMatchGP = true;
							}
						}
						else if (MyItm[i].What == GC_GAME_ITEM_BONUS)
						{
							// 보너스 아이템을 먹음.
							if (g_MyD3D->MyPlayer[ii]->IsLocalPlayer())
							{
								if (g_MyD3D->MyPlayer[ii]->m_cLife < 5)
									g_MyD3D->MyPlayer[ii]->m_cLife++;
							}
						}
						else if (MyItm[i].What == GC_GAME_ITEM_CHICKEN ||
							MyItm[i].What == GC_GAME_ITEM_GRAPE ||
							MyItm[i].What == GC_GAME_ITEM_WATERMELON ||
							MyItm[i].What == GC_GAME_ITEM_EGGCOOKIE ||
							MyItm[i].What == GC_GAME_ITEM_PIGHEAD)
						{
							// 에너지 보충 아이템을 먹음
							//if(g_MyD3D->MyPlayer[ii]->IsLocalPlayer())
							PLAYER::DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
							damage.SetDamage(15.f);
#else
							damage.SetDamage(.15f);
#endif		
							damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_PLAYER, MAX_PLAYER_NUM);
							g_MyD3D->MyPlayer[ii]->Change_HP(damage);
						}
						else
						{
							if (g_MyD3D->MyPlayer[ii]->GetNumGameItem() < NUM_TAKE_ITEM)
							{
								g_MyD3D->MyPlayer[ii]->Take_Item(MyItm[i].What);
							}
							else
							{
								MyItm[i].IsEnable = TRUE;
							}
						}

						if (MyItm[i].IsEnable == FALSE)
						{
							if (g_MyD3D->MyPlayer[ii]->IsLocalPlayer())
								g_KDSound.Play("9", 0, (LATENCY + SHOWLATENCY));
							else
								g_KDSound.Play("9");

							sfMaxY = 0;
						}

						break;
					}
				}
			}
		}
	}

	for (i = 0; i < MAX_NUM_ITEM_IN_MAP; i++)
	{
		if (MyItm[i].IsEnable)
		{
			// s =s0 + vt + 1/2 at^2 공식을 활용하였다. 중력 가속도는 우선 적당한 값을 넣어 만들었다만...
			float fX = MyItm[i].Start_x + MyItm[i].x_speed * MyItm[i].Time; //x 좌표는 중력의 영향을 안 받으니깐 가만히...
			float fY = MyItm[i].Start_y + MyItm[i].y_speed * MyItm[i].Time - 0.0027f*MyItm[i].Time*MyItm[i].Time;
			if (fY > sfMaxY) //최고점에 도달하기 전까지는 땅과의 충돌 체크를 안 한다.
			{
				sfMaxY = fY;
				MyItm[i].y = fY;
				MyItm[i].x = fX;
			}
			else
			{
				if (SiKGCRoomManager()->IsMonsterGameMode())
				{
					if ((SiKGCFantasticMap()->CrashCheck(MyItm[i].x, MyItm[i].y - 0.03f)) == false)  //땅에 닿기 전까지는 움직임.
					{
						MyItm[i].y = fY;
						MyItm[i].x = fX;
					}
					else
					{
						D3DXVECTOR2 dxvCrashPos;
						SiKGCFantasticMap()->CrashCheck(MyItm[i].x, MyItm[i].y - 0.03f, dxvCrashPos);
						MyItm[i].y = dxvCrashPos.y;
					}
				}
				else
				{
					if ((g_MyD3D->MyStg->Check_Contact(MyItm[i].x, MyItm[i].y - .03f, 0)).y == -1.0f)  //땅에 닿기 전까지는 움직임.
					{
						MyItm[i].y = fY;
						MyItm[i].x = fX;
					}
					else
					{
						MyItm[i].y = g_MyD3D->MyStg->Check_Contact(MyItm[i].x, MyItm[i].y - .03f, 0).y;
					}
				}
			}

			if (MyItm[i].Life)
			{
				MyItm[i].Life--;
				MyItm[i].Time++;
			}
			else
			{
				MyItm[i].IsEnable = FALSE;
				sfMaxY = 0;
			}
		}
	}

SELECT_ITEM:
	//{{ Appended by parkch: 2005-01-18 오후 3:47:07
	Selected_Item = -1;
	for (i = 0; i < NUM_TAKE_ITEM; i++)
	{
		if (g_MyD3D->MyPlayer[Me]->GetGameItem(i) > -1)
		{
			if (Item_Cursor == i) Selected_Item = g_MyD3D->MyPlayer[Me]->GetGameItem(i);
		}
	}
	//}} Appended by parkch: 2005-01-18 오후 3:47:07
}

void Items::Add(char What, const D3DXVECTOR2& Vec, bool OnlyMe/* =false */, WORD wItemLife /* = 500 */, float xspeed /*= 0*/, float yspeed /*= 0*/)
{
	if (What < 0 || What >= GC_GAME_ITEM_NUM)
		return;

	int i;
	for (i = 0; i < MAX_NUM_ITEM_IN_MAP; ++i)
	{
		if (!MyItm[i].IsEnable && MyItm[i].bRenderMatchGP == false)
			break;
	}

	if (i == MAX_NUM_ITEM_IN_MAP)
		return;

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EGGCOOKIE) == true)
	{
		if (GC_GAME_ITEM_CHICKEN <= What && What <= GC_GAME_ITEM_WATERMELON)
			What = GC_GAME_ITEM_EGGCOOKIE;
	}
	else if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_PIGHEAD) == true)
	{
		if (GC_GAME_ITEM_CHICKEN <= What && What <= GC_GAME_ITEM_WATERMELON)
			What = GC_GAME_ITEM_PIGHEAD;
	}

	if (g_kGlobalValue.m_kUserInfo.bHost && !OnlyMe)
	{
		TransNPC.What = 1;
		TransNPC.x = MyItm[i].x = Vec.x;
		TransNPC.y = MyItm[i].y = Vec.y;
		TransNPC.Action = What;// 패킷에 아이템 생성 정보를 실어준다.
		g_MyD3D->m_NPC.PushNPC_Action(&TransNPC);
		Clear_TransNPC();
	}
	else
	{
		MyItm[i].x = Vec.x;
		MyItm[i].y = Vec.y;
	}

	MyItm[i].Start_x = MyItm[i].x;
	MyItm[i].Start_y = MyItm[i].y;
	MyItm[i].x_speed = xspeed;
	MyItm[i].y_speed = yspeed;
	MyItm[i].Life = wItemLife;
	MyItm[i].Time = 0;
	MyItm[i].IsEnable = TRUE;
	MyItm[i].IsRight = true;
	MyItm[i].What = What;
}

void Items::AddMatchGP(int iGP, const D3DXVECTOR2& vPos, bool OnlyMe, WORD wItemLife, float xspeed, float yspeed, char cItemLord, int nDropperIndex)
{
	char What = GC_GAME_ITEM_MATCH_GP;

	int i;
	for (i = 0; i < MAX_NUM_ITEM_IN_MAP; i++)
	{
		if (!MyItm[i].IsEnable && MyItm[i].bRenderMatchGP == false)
			break;
	}

	if (i == MAX_NUM_ITEM_IN_MAP)
		return;

	if (g_kGlobalValue.m_kUserInfo.bHost && !OnlyMe)
	{
		TransNPC.What = 1;
		TransNPC.x = MyItm[i].x = vPos.x;
		TransNPC.y = MyItm[i].y = vPos.y;
		TransNPC.Action = What;// 패킷에 아이템 생성 정보를 실어준다.
		g_MyD3D->m_NPC.PushNPC_Action(&TransNPC);
		Clear_TransNPC();
	}
	else
	{
		MyItm[i].x = vPos.x;
		MyItm[i].y = vPos.y;
	}

	MyItm[i].iMatchGP = iGP;
	MyItm[i].Start_x = MyItm[i].x;
	MyItm[i].Start_y = MyItm[i].y;
	MyItm[i].x_speed = xspeed;
	MyItm[i].y_speed = yspeed;
	MyItm[i].Life = wItemLife;
	MyItm[i].Time = 0;
	MyItm[i].IsEnable = TRUE;
	MyItm[i].IsRight = true;
	MyItm[i].What = What;
	MyItm[i].cItemLord = cItemLord;
	MyItm[i].DropperUID = g_MyD3D->MyPlayer[nDropperIndex]->m_kUserInfo.dwUID;
	MyItm[i].DropperCharType = (char)g_MyD3D->MyPlayer[nDropperIndex]->GetCurrentChar().iCharType;
}

void Items::Alpha_Render()
{
	int i;
	D3DXMATRIX    mat1, mat2, mat3;
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		/*g_pd3dDevice->SetFVF( FVF_LVERTEX );
		g_pd3dDevice->SetStreamSource( 0, V_Item->GetBuffer(), 0, sizeof(SLVERTEX) );*/

		static char o = 0;
		++o;
		if (o > 7) o = 0;

		for (i = 0; i < MAX_NUM_ITEM_IN_MAP; ++i)
		{
			if (MyItm[i].IsEnable)
			{
				int iDrawItem = MyItm[i].What;

				if (iDrawItem == GC_GAME_ITEM_SANTA_GIFT ||
					iDrawItem == GC_GAME_ITEM_RICECAKE ||
					iDrawItem == GC_GAME_ITEM_EGGCOOKIE ||
					iDrawItem == GC_GAME_ITEM_PIGHEAD)
				{// 좌표계가 이상해서...하드코딩으로 Translation좌표를 조정 하였습니다...죄송 -_-;;;
					D3DXMatrixScaling(&mat1, 2.0f, 2.0f, 1.0f);
					D3DXMatrixTranslation(&mat2, MyItm[i].x - 1.0f - 0.07f, MyItm[i].y - 0.5f - 0.05f, 0.35f);
					D3DXMatrixMultiply(&mat2, &mat1, &mat2);
					D3DXMatrixMultiply(&mat1, &mat2, &g_kCamera.m_matCamera);
				}
				else
				{
					D3DXMatrixTranslation(&mat2, MyItm[i].x - 1.0f, MyItm[i].y - 0.5f, 0.35f);
					D3DXMatrixMultiply(&mat1, &mat2, &g_kCamera.m_matCamera);
				}

				if (MyItm[i].Life < 240)
				{
					if (o < 4)
						continue;
				}

				if (MyItm[i].Life < 120)
				{
					if (o == o / 2 * 2)
						continue;
				}

				g_pGCDeviceManager2->DrawInWorld(m_pTexItem[iDrawItem], &mat1, V_Item.as_array());
			}
		}
		D3DXMatrixIdentity(&mat1);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
	}
	g_pGCDeviceManager2->PopState();

	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		//g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		for (i = 0; i < MAX_NUM_ITEM_IN_MAP; i++)
		{
			if (MyItm[i].bRenderMatchGP)
			{
				if (MyItm[i].fRenderTime <= 0.75f)
				{
					TCHAR eText[10] = { 0, };
					long Change = MyItm[i].iMatchGP;
					if (Change >= 0)
					{
						_stprintf(&eText[1], _T("%d"), Change);
						eText[0] = '9' + 2;

						float fItmX = MyItm[i].x - 0.95f;
						float fItmY = MyItm[i].y - 0.1f;

						if (MyItm[i].fRenderTime <= 0.3f)
						{
							Num_Render(fItmX, fItmY + MyItm[i].fRenderTime, eText, D3DCOLOR_ARGB(255, 255, 255, 255), 0.06f);
						}
						else
						{
							int iAlpha = (int)(425.0f - 570.0f * MyItm[i].fRenderTime);
							if (iAlpha < 0)
								iAlpha = 0;

							Num_Render(fItmX, fItmY + 0.3f, eText, D3DCOLOR_ARGB(iAlpha, 255, 255, 255), 0.06f);
						}
					}
					MyItm[i].fRenderTime += 0.006f;
				}
				else
				{
					MyItm[i].bRenderMatchGP = false;
					MyItm[i].fRenderTime = 0.0f;
				}
			}
		}
	}
	g_pGCDeviceManager2->PopState();
}

void Items::Delete_Item()
{
	char i;
	i = g_MyD3D->Get_MyPlayer();
	if (Item_Cursor < 0) return;
	if (g_MyD3D->MyPlayer[i]->GetGameItem(Item_Cursor) != -1)
	{
		g_MyD3D->MyPlayer[i]->EraseGameItem(Item_Cursor);
		if (g_MyD3D->MyPlayer[i]->GetGameItem(Item_Cursor) == -1) Item_Cursor--;
	}
}

void Items::My_Item_Alpha_Render()
{
	if (g_MyD3D->IsLocalPlayerObserver())
		return;

	int i, ii = 0, iii = g_MyD3D->Get_MyPlayer();
	D3DXMATRIX    mat1;

	// 자신이 적용받고 있는 아이템 표시
	for (i = 0; i < GC_GAME_ITEM_NUM; i++)
	{
		if (g_MyD3D->MyPlayer[iii]->Item_Remain_Time[i])
		{
			//D3DXMatrixTranslation( &mat1, (float)ii*.15f-.9f, -.65f, .103f );
			D3DXMatrixTranslation(&mat1, (float)ii* .1f - 1.0f, -.45f, .103f);
			g_pGCDeviceManager2->DrawInWorld(m_pTexItem[i], &mat1, V_Item.as_array());
			ii++;
		}
	}
}

// 보통 호스트에서만 호출되는 함수임
// 적용받는 플레이어를 적절히 선택해 다시 분배함
void Items::Use_Item(char Who, char What, int iTime /*iTime = -1*/, bool bNoChat /*= false*/)
{
	char i, Group = 0;//0이면 자신만...1이면 같은 팀원, 2이면 상대팀

	if (g_pkChatManager != NULL && false == bNoChat)
	{
		g_pkChatManager->AddSystemMsg(-1,
			g_pkStrLoader->GetReplacedString(STR_ID_GAMEMSG_ITEM_USE1, "ss",
				L"#c" + g_MyD3D->MyPlayer[Who]->GetStrUserColor() + g_MyD3D->MyPlayer[Who]->GetStrUserName().c_str() + L"#cx",
				g_MyD3D->MyItms->GetItemDesc(What))
		);
	}

	switch (What)
	{
	case GC_GAME_ITEM_HPUP:// 같은 팀 HP 최고치
		if (GC_GM_SURVIVAL_MODE == SiKGCRoomManager()->GetGameMode())
			Group = 0;
		else
			Group = 1;
		break;
	case GC_GAME_ITEM_SUPER:// 자신이 10초간 무적
		Group = 0;
		break;
	case GC_GAME_ITEM_HASTE:// 상대팀의 속도가 1/2로 줄어듬
		if (GC_GM_SURVIVAL_MODE == SiKGCRoomManager()->GetGameMode())
			Group = 3;
		else
			Group = 2;
		break;
	case GC_GAME_ITEM_WING:// 자신의 점프력 증가
		Group = 0;
		break;
	case GC_GAME_ITEM_HALLOW:// 자신만 잠시동안 투명
	{
		Group = 0;
		g_pMagicEffect->ClearMagicEffect(Who, EGC_SKILL_HIDE);
	}
	break;
	case GC_GAME_ITEM_REFLECTION:// 자신만 간접공격 반사
		Group = 0;
		break;
	case GC_GAME_ITEM_MPUP:// 같은 팀 MP 올려주기( 점령전은 무조건 혼자 )
		if (GC_GM_SURVIVAL_MODE == SiKGCRoomManager()->GetGameMode() ||
			GC_GM_DOTA == SiKGCRoomManager()->GetGameMode())
			Group = 0;
		else
			Group = 1;
		break;
	case GC_GAME_ITEM_BUFF_MONSPEEDDOWN://       = 25,   ///< 몬스터 이동속도 감소
	{
		for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			if (mit->second == NULL) continue;

			MONSTER* pMonster = mit->second;

			if (!pMonster->IsLive())
				continue;

			if (NPC_NONE != pMonster->m_eNPC)
				continue;

			pMonster->SetMagicEffectNoLocal(mit->first, EGC_EFFECT_MONSTER_SPEED_DOWN, 30.0f, 0);
		}
	}
	return;
	case GC_GAME_ITEM_BUFF_CLEAR_DEBUFF://       = 26,   ///< DEBUFF속성이 있는 버프 제거
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			g_MyD3D->MyPlayer[iMyPlayer]->SetMagicEffect(EGC_EFFECT_CLEAR_DEBUFF, 1.0f);
		}
	}
	return;
	case GC_GAME_ITEM_BUFF_ATKUP://              = 27,   ///< 내 공격력 증가
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			g_MyD3D->MyPlayer[iMyPlayer]->SetMagicEffect(EGC_EFFECT_REVOLT_ATK_UP, 30.0f);
		}
	}
	return;
	case GC_GAME_ITEM_BUFF_MOVESPEEDUP://        = 28,   ///< 내 이동속도 증가
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			g_MyD3D->MyPlayer[iMyPlayer]->SetMagicEffect(EGC_EFFECT_REVOLT_MOVE_UP, 30.0f);
		}
	}
	return;
	case GC_GAME_ITEM_DAMAGE_MISSILETURRET://    = 29,   ///< 마리 포탑설치
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			KGC_PID_BROAD_ADD_DAMAGE kPacket;
			kPacket.dwUID = g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.dwUID;
			kPacket.iPlayerIndex = iMyPlayer;
			kPacket.iDamageEnum = DT_GAMEITEM_MARI_MISSILETURRET;
			SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
			KGCPC_MEMO("KGC_PID_BROAD_ADD_DAMAGE");
		}
	}
	return;
	case GC_GAME_ITEM_BUFF_JUMPUP:
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			g_MyD3D->MyPlayer[iMyPlayer]->SetMagicEffect(EGC_EFFECT_JUMPUP, 30.0f);
		}
	}
	return;
	case GC_GAME_ITEM_DAMAGE_ICE://    = 31,   ///< 아르메 워록 3단필
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			KGC_PID_BROAD_ADD_DAMAGE kPacket;
			kPacket.dwUID = g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.dwUID;
			kPacket.iPlayerIndex = iMyPlayer;
			kPacket.iDamageEnum = DT_GAMEITEM_ARME_ICE;
			SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
			KGCPC_MEMO("KGC_PID_BROAD_ADD_DAMAGE");
		}
	}
	return;
	case GC_GAME_ITEM_DAMAGE_SPECIAL:// =32,    ///< 전역 필살 스킬
	{
		int iMyPlayer = g_MyD3D->Get_MyPlayer();
		if (g_MyD3D->IsPlayerIndex(iMyPlayer))
		{
			KGC_PID_BROAD_ADD_DAMAGE kPacket;
			kPacket.dwUID = g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.dwUID;
			kPacket.iPlayerIndex = iMyPlayer;
			kPacket.iDamageEnum = DT_COMBAT_AIRPLANE_SPECIAL;
			kPacket.iAddDamageType = 1;
			float fStageX = 0.0;
			if (SiKGCRoomManager()->IsMonsterGameMode())
				fStageX = SiKGCFantasticMap()->GetPixelWidth();
			else
				fStageX = 1.5f * (float)g_MyD3D->MyStg->Num_Width;
			kPacket.vPos.x = fStageX / 2.0f;
			kPacket.vPos.y = 1.0f;
			SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
			KGCPC_MEMO("KGC_PID_BROAD_ADD_DAMAGE");

			Group = 1;
			g_MyD3D->MyItms->Use_Item(iMyPlayer, GC_GAME_ITEM_SUPER, 255, true);
		}
	}
	break;
	}

	for (i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		//초기화를 안해줘서 헤이스트 아이템인 경우 이상한 현상 발생
		g_kEffectItem->Eff_Item[i].cWhat = -1;
		g_kEffectItem->Eff_Item[i].sTime = iTime;
		g_kEffectItem->Eff_Item[i].bShowChat = !bNoChat;

		if (g_MyD3D->MyPlayer[i]->m_cLife)
		{
			switch (Group)
			{
			case 0:    // 자신만 적용됨
				if (i == Who)
					g_kEffectItem->Eff_Item[i].cWhat = What;
				break;
			case 1:    // 우리팀이 모두 적용됨
				if (g_MyD3D->IsSameTeam(i, Who) == true)
					g_kEffectItem->Eff_Item[i].cWhat = What;
				break;
			case 2: // 적팀이 적용됨
				if (g_MyD3D->IsSameTeam(i, Who) == false)
					g_kEffectItem->Eff_Item[i].cWhat = What;
				break;
			case 3: // 나 이외에 모두 적용(Survival Mode)
				if (i != Who)
					g_kEffectItem->Eff_Item[i].cWhat = What;
				break;
			}
		}

		g_MyD3D->MyPlayer[i]->Eff_Item(g_kEffectItem->Eff_Item[i]);
	}

	//리플레이에 정보 저장하기.
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
		g_MyD3D->MyReplay->Set_ItemData(g_kEffectItem, Who);
}

void Items::Through_Item()
{
	char Index = g_MyD3D->Get_MyPlayer();

	if (g_MyD3D->MyPlayer[Index]->Item_Score > 0)
	{
		g_MyD3D->MyPlayer[Index]->Item_Score--;
		D3DXVECTOR2 Vec;
		Vec.x = g_MyD3D->MyPlayer[Index]->vPos.x;
		Vec.y = g_MyD3D->MyPlayer[Index]->vPos.y + .2f;
		g_kTransNPC->TransNPC.What = 1;
		g_kTransNPC->TransNPC.Action = GC_GAME_ITEM_GP;
		g_kTransNPC->TransNPC.x = Vec.x;
		g_kTransNPC->TransNPC.y = Vec.y;

		//일단 나부터 만들자
		Add(GC_GAME_ITEM_GP, Vec, true);
	}
	/*
	else if(g_MyD3D->MyPlayer[Index]->Event_Item_Score>0)
	{
		g_MyD3D->MyPlayer[Index]->Event_Item_Score--;
		D3DXVECTOR2 Vec;
		Vec.x=g_MyD3D->MyPlayer[Index]->x;
		Vec.y=g_MyD3D->MyPlayer[Index]->y+.2f;
		g_kTransNPC->TransNPC.What=1;
		g_kTransNPC->TransNPC.Action=8;
		g_kTransNPC->TransNPC.x=Vec.x;
		g_kTransNPC->TransNPC.y=Vec.y;

		//일단 나부터 만들자
		Add(8, Vec, true);
	}
	*/
	else if (g_MyD3D->MyPlayer[Index]->GetGameItem(0) != -1)
	{
		D3DXVECTOR2 Vec;
		Vec.x = g_MyD3D->MyPlayer[Index]->vPos.x;
		Vec.y = g_MyD3D->MyPlayer[Index]->vPos.y + .2f;

		g_kTransNPC->TransNPC.What = 1;
		g_kTransNPC->TransNPC.Action = g_MyD3D->MyPlayer[Index]->GetGameItem(0);
		g_kTransNPC->TransNPC.x = Vec.x;
		g_kTransNPC->TransNPC.y = Vec.y;

		//일단 나부터 만들자
		Add(g_kTransNPC->TransNPC.Action, Vec, true);

		g_MyD3D->MyPlayer[Index]->EraseGameItem(0);
		Item_Cursor = -1;
	}
}

void Items::Through_MatchGP()
{
	char Index = g_MyD3D->Get_MyPlayer();

	//한번만 들어와야.
	if (g_MyD3D->MyPlayer[Index]->m_bThrowMatchGP)
		return;

	//대전일 때, 사람한테 죽었을 때에만 해당된다.
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MATCH ||
		g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer() >= MAX_PLAYER_NUM)
		return;
	//============================================================================================
	// 드랍 확률을 구하는 부분. 필요할 경우 모듈화 필요.
	float fDropProb = 0.0f;

	int iKillLevel = (int)Exp_2_Level(
		GetCharacterExp(g_pGameBoard->m_pStartingMember[g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer()].m_mapStartExp,
			g_MyD3D->MyPlayer[g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer()]->GetCurrentChar().iCharType));
	int iDieLevel = (int)Exp_2_Level(
		GetCharacterExp(g_pGameBoard->m_pStartingMember[Index].m_mapStartExp,
			g_MyD3D->MyPlayer[Index]->GetCurrentChar().iCharType));
	fDropProb = 0.6f * (float)(100 - (iKillLevel - iDieLevel)*(iKillLevel - iDieLevel)) / 100.0f; //공식 :  GP드랍 확률 = {100 - (n-m)^2} * 0.6  ( n:죽인 유저Lv / m:죽은 유저 Lv )

	float fProb = (float)::rand() / (float)RAND_MAX;

	//printf( "fProb is :  %f\n" , fProb );
	//printf( "fDropProb is :  %f\n" , fDropProb );

	if (fProb > fDropProb)
	{
		g_MyD3D->MyPlayer[Index]->m_bThrowMatchGP = true;
		return;
	}
	//============================================================================================
	D3DXVECTOR2 Vec;
	Vec.x = g_MyD3D->MyPlayer[Index]->vPos.x;
	Vec.y = g_MyD3D->MyPlayer[Index]->vPos.y + .2f;
	g_kTransNPC->TransNPC.What = 1;
	g_kTransNPC->TransNPC.Action = GC_GAME_ITEM_MATCH_GP;
	g_kTransNPC->TransNPC.x = Vec.x;
	g_kTransNPC->TransNPC.y = Vec.y;
	g_kTransNPC->TransNPC.HP = (float)Index; // 죽은녀석

	//나오는 GP공식 : 드랍 GP 수식 : 1+(n+m)-(n-m) ( n:죽인 유저Lv / m:죽은 유저 Lv )
	int iGP = 1 + 2 * iDieLevel;
	//아이템 튀어 나오는 방향 결정.죽인 사람이 죽은 사람의 오른쪽에 있었나?
	float xspeed = (g_MyD3D->MyPlayer[Index]->vPos.x < g_MyD3D->MyPlayer[g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer()]->vPos.x) ? 0.015f : -0.015f;

	//printf("My Killer is : %d\n" , g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer());
	//일단 나부터 만들자
	AddMatchGP(iGP, Vec, true, 550, xspeed, 0.04f, g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer(), Index);
	g_MyD3D->MyPlayer[Index]->m_bThrowMatchGP = true;
}

const WCHAR* Items::GetItemDesc(int iItem) const
{
	return g_pkStrLoader->GetString(m_aiItemNameID[iItem]).c_str();
}

// 움직이지 않는 숫자만 렌더링
void Items::Num_Render(float x, float y, const TCHAR* Text, DWORD Color, float Width)
{
	D3DXMATRIX mat1, mat2, mat3;

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	V_Letter[0].Diff = V_Letter[1].Diff = Color;
	V_Letter[2].Diff = V_Letter[3].Diff = Color;

	int Feed = 0;
	int Line = 0;

	int iTexLen = (int)_tcsclen(Text);

	for (int i = 0; i < iTexLen; i++)
	{
		int T;
		T = iTexLen - i - 1;

		if (Text[T] > _T('A') - 1 && Text[T] < _T('H'))
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('A')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('A') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = 0;
			V_Letter[2].tv = V_Letter[3].tv = .1666f;
		}
		else if (Text[T] > _T('G') && Text[T] < _T('O'))
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('H')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('H') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = .1666f;
			V_Letter[2].tv = V_Letter[3].tv = .3333f;
		}
		else if (Text[T] > _T('N') && Text[T] < _T('V'))
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('O')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('O') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = .3333f;
			V_Letter[2].tv = V_Letter[3].tv = .5f;
		}
		else if (Text[T] > _T('U') && Text[T] < _T('Z') + 3)
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('V')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('V') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = .5f;
			V_Letter[2].tv = V_Letter[3].tv = .6666f;
		}
		else if (Text[T] > _T('0') - 1 && Text[T] < _T('7'))
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('0')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('0') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = .6666f;
			V_Letter[2].tv = V_Letter[3].tv = .8333f;
		}
		else if (Text[T] > _T('6') && Text[T] < _T('9') + 4)
		{
			V_Letter[0].tu = V_Letter[2].tu = (float)(Text[T] - _T('7')) / 7.0f;
			V_Letter[1].tu = V_Letter[3].tu = (float)(Text[T] - _T('7') + 1) / 7.0f;
			V_Letter[0].tv = V_Letter[1].tv = .8333f;
			V_Letter[2].tv = V_Letter[3].tv = 1.0f;
		}
		else if (Text[T] == _T('.'))
		{
			Line++;
			Feed = 0;
			if (Text[T + 1] == _T('U'))
			{
				BYTE Alpha = (BYTE)(Color >> 24);
				V_Letter[0].Diff = V_Letter[1].Diff = D3DCOLOR_ARGB(Alpha, 255, 100, 0);
				V_Letter[2].Diff = V_Letter[3].Diff = D3DCOLOR_ARGB(Alpha, 150, 30, 0);
			}
			else
			{
				BYTE Alpha = (BYTE)(Color >> 24);
				V_Letter[0].Diff = V_Letter[1].Diff = Color;
				V_Letter[2].Diff = V_Letter[3].Diff = D3DCOLOR_ARGB(Alpha, 55, 55, 55);
			}
			continue;
		}
		else if (Text[T] == _T(' '))
		{
			Feed++;
			continue;
		}
		else
		{
			int T_T = 10;
			if (Text[T] == _T('+')) T_T = 4;
			else if (Text[T] == _T('-')) T_T = 5;
			else if (Text[T] == _T('%')) T_T = 6;
			V_Letter[1].tu = V_Letter[3].tu = (float)(T_T) / 6.0f;
			V_Letter[0].tu = V_Letter[2].tu = (float)(T_T + 1) / 6.0f;
			V_Letter[0].tv = V_Letter[1].tv = .8333f;
			V_Letter[2].tv = V_Letter[3].tv = 1.0f;
		}

		D3DXMatrixTranslation(&mat2, x - Width*(float)Feed, y - (float)Line*.11f, .35f);
		D3DXMatrixMultiply(&mat3, &mat2, &g_kCamera.m_matCamera);
		D3DXMatrixScaling(&mat2, 0.4f, 0.4f, 1.0f);
		D3DXMatrixMultiply(&mat1, &mat2, &mat3);

		g_pGCDeviceManager2->DrawInWorld(m_pTexLetter, &mat1, V_Letter.as_array());
		Feed++;
	}
	g_pGCDeviceManager2->PopState();

	D3DXMatrixIdentity(&mat1);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
}

void Items::CatchMatchCoin(int iItemIndex, int iPlayerIndex)
{
	KKillCoinInfo kGPInfo;
	kGPInfo.m_dwUserUID = MyItm[iItemIndex].DropperUID;
	kGPInfo.m_cCharType = MyItm[iItemIndex].DropperCharType;
	kGPInfo.m_nCoin = MyItm[iItemIndex].iMatchGP;
	g_MyD3D->MyPlayer[iPlayerIndex]->m_vecKillCoinInfo.push_back(kGPInfo);
}