#include "stdafx.h"
#include "MyD3D.h"
#include "Headup Display.h"
#include "KDSound.h"
#include "KDInput.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCHeadupDisplay.h"
#include "GCUI/GCOnGameExpBar.h"
#include "DamageManager.h"
#include "GCGameResult.h"
#include "GCStateMachine.h"
#include "Monster.h"
////#include "Frame Load.h"
#include "Procedure.h"
#include <Thread/Locker.h>
//#include <sstream>
#include <iomanip>
#include <GCDeviceLib/KD3DDeviceState.h>
#include "GCGraphicsHelper.h"
#include "GCStateGame.h"
#include "KGCGameBoard.h"
#include "GCCameraOrtho.h"
#include "Replay.h"
#include "Pet.h"
#include "GCEventManager.h"
#include "KGCRoomManager.h"
#include "GCUI/KGCSquareOverlayUI.h"
#include "KGCAnnounceMgr.h"
#include "GCEclipseEventManager.h"
//#include "KGCMagicEffect.h"
//#include "gcui/GCUIScene.h"
#include "MonsterManager.h"
#include "Message.h"

#define PIXELTOTEXEL(p) ( (p)/256.0f )
#define PIXELTOSIZE(p) ( p.0f / 400.0f )
#define PIXELTOTRANS(p) ( p / 400.0f )

Headup::Headup()
{
    Clear();
    ComboClear();
    Level = 0;
    IsBlackMagic = true;
    m_IsMPDamage = false;
    m_MaxCombo = 0;

    m_pTexPointer = NULL;
    m_pTexPointer2 = NULL;
    m_pTexLetter = NULL;
    m_pTexBlessingBar1 = NULL;
    m_pTexBlessingBar2 = NULL;  
    
    m_vecDamageString.clear();
    m_iDamageTextCount = 0;
}

Headup::~Headup()
{
    SAFE_DEVICE_RELEASE(m_pTexPointer);
    SAFE_DEVICE_RELEASE(m_pTexPointer2);
    SAFE_DEVICE_RELEASE(m_pTexLetter);
    SAFE_DEVICE_RELEASE(m_pTexBlessingBar1);
    SAFE_DEVICE_RELEASE(m_pTexBlessingBar2);
}

void Headup::Event_Combo(bool bAerial, float m_fHP )
{
    if(m_fHP<0) return;
    if(Combo_Count>0 && (ShowCombo || Combo_Count==1))
    {
        Combo_Count++;
        bEventComboCheck = true;
        bTotalScoreEffect = true;

        g_pkQuestManager->CalcMaxCombo( Combo_Count );
		
		//�̼����ǿ� ���� �޺� ����
        g_MyD3D->GetMyPlayer()->AddControlInfoComboCount(Combo_Count);
		
        if( Combo_Count == 2 )
        {
            g_MyD3D->GetMyPlayer()->SetComboState(true);
            if( g_kGlobalValue.m_kUserInfo.bHost == false && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
            {
                KGC_PID_QUEST_RANK_COMBO_STATE kPacket;
                kPacket.dwUID = g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID;
                kPacket.bIsComboState = true;
                kPacket.bAttacked = false;

				SendP2PPacket( &kPacket, sizeof(kPacket), SiKGCRoomManager()->GetHostUID(), _RELIABLE );
				KGCPC_MEMO( "KGC_PID_QUEST_RANK_COMBO_STATE" );
            }            
        }
        if( bAerial ) Combo_Count_Aerial++;
        Combo_HP+=m_fHP;
        Combo_Time=timeGetTime();
        ShowCombo=true;
    }
    else
    {
        Combo_Count=1;
        Combo_Count_Aerial = 0;
        Combo_HP=m_fHP;
        Combo_Time=timeGetTime();
        ShowCombo=false;
    }

    SiKGCAchieveManager()->OccurAction( SAchieve::COMBO_RECORD, g_MyD3D->GetMyPlayer()->GetCharType(), Combo_Count+Combo_Count_Aerial );
}

void Headup::FrameMove_Combo()
{
    if( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Slow_Count>0 || true == g_pkGameOverlayUI->IsShowDialogue() )
    {
        Combo_Time=timeGetTime();
    }

    if(ShowCombo)
    {
        Combo_Pitch+=(1.1f-Combo_Pitch)*.1f;
        if(Combo_Pitch>1.0f) Combo_Pitch=1.0f;
        if(Combo_Time+m_dwBreakGap<timeGetTime()) 
        {
            if( ShowCombo )
                SiKGCAchieveManager()->OccurAction( SAchieve::COMBO_BREAK, g_MyD3D->GetMyPlayer()->GetCharType() );

            g_pkQuestManager->SetComboKillPoint( true );
            g_pkQuestManager->ComboBreakCheck( true );
            g_pGameBoard->SetComboBreak(true);
            bDisappearComboCheck = true;
            ShowCombo=false;

            if( g_MyD3D->GetMyPlayer()->GetcomboState() && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
            {
                g_MyD3D->GetMyPlayer()->SetComboState(false);

                if( g_kGlobalValue.m_kUserInfo.bHost == false )
                {
                    KGC_PID_QUEST_RANK_COMBO_STATE kPacket;
                    kPacket.dwUID = g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID;
                    kPacket.bIsComboState = false;
                    kPacket.bAttacked = false;

					SendP2PPacket( &kPacket, sizeof(kPacket), SiKGCRoomManager()->GetHostUID(), _RELIABLE );    
					KGCPC_MEMO( "KGC_PID_QUEST_RANK_COMBO_STATE" );    
                }                
            }
        }
#ifdef LEVEL_DESIGN_STAT_CALC
        Show_Combo_HP+=(Combo_HP-Show_Combo_HP)*0.2f;
#else
        Show_Combo_HP+=((Combo_HP*100.0f)-Show_Combo_HP)*0.2f;
#endif
    }
    else
    {
        Combo_Pitch*=.7f;
    }
}

void Headup::Render_Combo()
{
	//����...
	float fWindowScaleX = g_pGCDeviceManager->GetWindowScaleX() / (1024.0f/768.0f);
	float fWindowScaleY = g_pGCDeviceManager->GetWindowScaleY() / (1024.0f/768.0f);
	float fWindowScaleY_Screen = g_pGCDeviceManager->GetWindowScaleY();

	if (fWindowScaleX < 1 || fWindowScaleY < 1)
	{
		fWindowScaleX = 1;
		fWindowScaleY = 1;
	}

    if(g_MyD3D->m_KGCOption.Is3Back() && Combo_Pitch>.1f)
    {
        float X_Pos=(float)((int)(Combo_Pitch*180.0f-180.0f));
		float Y_Pos = 200.0f; //�� �Ʒ� �ڵ� �������ֽ� ���� ã���ϴ�. �� gg

       
        g_pGCDeviceManager2->PushState();        
        {
            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


            g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
											,X_Pos * fWindowScaleX 
											,(Y_Pos - 1) * fWindowScaleY_Screen
											,(X_Pos+180.0f) * fWindowScaleX
											,(Y_Pos - 1) * fWindowScaleY_Screen + 64 * fWindowScaleY
											,0,0.611328125f,0.3515625f,0.740234375f);

            if(Combo_Count<10)
            {
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					, (X_Pos+112.0f) * fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen
					, (X_Pos+140.0f) * fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Combo_Count*0.056640625f),0.87890625f,(float)((Combo_Count+1)*0.056640625f),0.962890625f);

                if( Combo_Count_Aerial > 0 )
                {
                    g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
						,(X_Pos+139.5f) * fWindowScaleX
						,(Y_Pos - 1) * fWindowScaleY_Screen
						,(X_Pos+169.5f) * fWindowScaleX
						,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
						,(float)(10*0.056640625f),0.87890625f,(float)((10+1.6f)*0.056640625f),0.962890625f);       // + ǥ��
                    if( Combo_Count_Aerial < 10 )
                    {
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+164.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							,X_Pos+191.0f * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Combo_Count_Aerial*0.056640625f),0.87890625f,(float)((Combo_Count_Aerial+1)*0.056640625f),0.962890625f);       // + ǥ��
                    }
                    else
                    {
                        int Ct=0;
                        if(Combo_Count_Aerial>99) Combo_Count_Aerial=99;
                        Ct=Combo_Count_Aerial%10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+218.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							,(X_Pos+245.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                        Ct=Combo_Count_Aerial/10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+191.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							,(X_Pos+218.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                    }
                }
            }
            else if (Combo_Count<100)
            {
                int Ct=0;
                Ct=Combo_Count%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+127.0f) * fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen
					,(X_Pos+155.0f) * fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                Ct=Combo_Count/10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+100.0f )* fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen
					,(X_Pos+128.0f) * fWindowScaleX
					,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);

                if( Combo_Count_Aerial > 0 )
                {
                    g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
						,(X_Pos+156.5f) * fWindowScaleX
						,(Y_Pos - 1) * fWindowScaleY_Screen
						,(X_Pos+184.5f) * fWindowScaleX
						,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
						,(float)(10*0.056640625f),0.87890625f,(float)((10+1.4f)*0.056640625f),0.962890625f);       // + ǥ��
                    if( Combo_Count_Aerial < 10 )
                    {
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+183.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							, (X_Pos+211.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Combo_Count_Aerial*0.056640625f),0.87890625f,(float)((Combo_Count_Aerial+1)*0.056640625f),0.962890625f);       // + ǥ��
                    }
                    else
                    {
                        int Ct=0;
                        if(Combo_Count_Aerial>99) Combo_Count_Aerial=99;
                        Ct=Combo_Count_Aerial%10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+210.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							,(X_Pos+238.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                        Ct=Combo_Count_Aerial/10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
							,(X_Pos+183.0f)* fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen
							,(X_Pos+211.0f) * fWindowScaleX
							,(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY
							,(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                    }
                }
            }
            else if (Combo_Count<1000)
            {
                int Ct=0;
                int Ct2=0;
                Ct=Combo_Count%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+154.0f) * fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+182.0f) * fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct*0.056640625f), 0.87890625f,(float)((Ct+1)*0.056640625f), 0.962890625f);
                Ct=Combo_Count/10;
                Ct2=Ct%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+127.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+155.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct2*0.056640625f),0.87890625f,(float)((Ct2+1)*0.056640625f),0.962890625f);
                Ct2=Ct/10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+100.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+128.0f) * fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct2*0.056640625f),0.87890625f,(float)((Ct2+1)*0.056640625f),0.962890625f);

                if( Combo_Count_Aerial > 0 )
                {
                    g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
						(X_Pos+183.5f)* fWindowScaleX,
						(Y_Pos - 1) * fWindowScaleY_Screen,
						(X_Pos+209.5f)* fWindowScaleX,
						(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
						(float)(10*0.056640625f),0.87890625f,(float)((10+1.4f)*0.056640625f),0.962890625f);       // + ǥ��
                    if( Combo_Count_Aerial < 10 )
                    {
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+208.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+236.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Combo_Count_Aerial*0.056640625f),0.87890625f,(float)((Combo_Count_Aerial+1)*0.056640625f),0.962890625f);       // + ǥ��
                    }
                    else
                    {
                        int Ct=0;
                        if(Combo_Count_Aerial>99) Combo_Count_Aerial=99;
                        Ct=Combo_Count_Aerial%10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+235.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+263.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                        Ct=Combo_Count_Aerial/10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+208.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+236.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);       // + ǥ��

                    }
                }
            }
            else
            {
                int Ct=0;
                int Ct2=0;
                int Ct3=0;
                if(Combo_Count>9999) Combo_Count=9999;
                Ct=Combo_Count%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+181.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+209.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                Ct=Combo_Count/10;
                Ct2=Ct%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+154.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+182.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct2*0.056640625f),0.87890625f,(float)((Ct2+1)*0.056640625f),0.962890625f);
                Ct2=Ct/10;
                Ct3=Ct2%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+127.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+155.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct3*0.056640625f),0.87890625f,(float)((Ct3+1)*0.056640625f),0.962890625f);
                Ct3=Ct2/10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
					(X_Pos+100.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen,
					(X_Pos+128.0f)* fWindowScaleX,
					(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
					(float)(Ct3*0.056640625f),0.87890625f,(float)((Ct3+1)*0.056640625f),0.962890625f);

                if( Combo_Count_Aerial > 0 )
                {
                    g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
						(X_Pos+210.5f)* fWindowScaleX,
						(Y_Pos - 1) * fWindowScaleY_Screen,
						(X_Pos+238.5f)* fWindowScaleX,
						(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
						(float)(10*0.056640625f),0.87890625f,(float)((10+1.4f)*0.056640625f),0.962890625f);       // + ǥ��
                    if( Combo_Count_Aerial < 10 )
                    {
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+237.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+265.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Combo_Count_Aerial*0.056640625f),0.87890625f,(float)((Combo_Count_Aerial+1)*0.056640625f),0.962890625f);       // + ǥ��
                    }
                    else
                    {
                        int Ct=0;
                        if(Combo_Count_Aerial>99) Combo_Count_Aerial=99;
                        Ct=Combo_Count_Aerial%10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+264.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+292.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);
                        Ct=Combo_Count_Aerial/10;
                        g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3),
							(X_Pos+237.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen,
							(X_Pos+265.0f)* fWindowScaleX,
							(Y_Pos - 1) * fWindowScaleY_Screen + 42 * fWindowScaleY,
							(float)(Ct*0.056640625f),0.87890625f,(float)((Ct+1)*0.056640625f),0.962890625f);       // + ǥ��

                    }
                }
            }

            int iDisplay_Score = (int)(Show_Combo_HP + 0.99f);

            if(iDisplay_Score<10)
            {
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+100.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+116.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(iDisplay_Score*0.03125f),0.96875f,(float)((iDisplay_Score+1)*0.03125f),1.0f);
            }
            else if(iDisplay_Score<100)
            {
                int Ct=0;
                Ct=iDisplay_Score%10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+100.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+116.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.03125f),0.96875f,(float)((Ct+1)*0.03125f),1.0f);
                Ct=iDisplay_Score/10;
                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+84.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+100.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.03125f),0.96875f,(float)((Ct+1)*0.03125f),1.0f);
            }
            else
            {
                int Ct=0;
                if(iDisplay_Score>999) iDisplay_Score=999;
                Ct=iDisplay_Score%10;

                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+100.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+116.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.03125f),0.96875f,(float)((Ct+1)*0.03125f),1.0f);
                Ct=(iDisplay_Score/10)%10;

                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+84.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+100.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.03125f),0.96875f,(float)((Ct+1)*0.03125f),1.0f);
                Ct=iDisplay_Score/100;

                g_pGCDeviceManager2->DrawInScreen(g_pkUIMgr->GetDeviceTexture(3)
					,(X_Pos+68.0f) * fWindowScaleX
					,(Y_Pos + 43) * fWindowScaleY_Screen
					,(X_Pos+84.0f) * fWindowScaleX
					,(Y_Pos - 43) * fWindowScaleY_Screen + 42 * fWindowScaleY
					,(float)(Ct*0.03125f),0.96875f,(float)((Ct+1)*0.03125f),1.0f);
            }
        }
       g_pGCDeviceManager2->PopState();
    }
}

void Headup::InitDevice()
{
    m_pTexPointer = g_pGCDeviceManager2->CreateTexture( "Energybar.dds" );
    m_pTexPointer2 = g_pGCDeviceManager2->CreateTexture( "bar2.dds" );
    m_pTexLetter = g_pGCDeviceManager2->CreateTexture( "letter.dds" );
    m_pTexBlessingBar1 = g_pGCDeviceManager2->CreateTexture( "blessingbar1.dds" );
    m_pTexBlessingBar2 = g_pGCDeviceManager2->CreateTexture( "blessingbar2.dds" );

    V_Letter[0].SetSLVER(-.1f,.1f,0,0,0);
    V_Letter[1].SetSLVER(.1f,.1f,0,0,0);
    V_Letter[2].SetSLVER(-.1f,-.1f,0,0,0);
    V_Letter[3].SetSLVER(.1f,-.1f,0,0,0);    

    LoadPuzzleKeySequenceList();
}

void Headup::DeleteDevice()
{
    SAFE_DEVICE_RELEASE(m_pTexPointer);
    SAFE_DEVICE_RELEASE(m_pTexPointer2);
    SAFE_DEVICE_RELEASE(m_pTexLetter);
    SAFE_DEVICE_RELEASE(m_pTexBlessingBar1);
    SAFE_DEVICE_RELEASE(m_pTexBlessingBar2);
}

void Headup::Clear_All_Cheat()
{
    IsCheatMode = false;
}

void Headup::Add_HP_Changer( int Playersource, int PlayerNum, float Change, bool IsMPDamage, CString strText, DWORD dwAttackType, float fHitPosX, float fHitPosY, int iTextType )
{
    if( Change < 1 )
        return;
		//MAX ������ ����
    if( Change > 9999999 )
        Change = 9999999;

    if( !g_MyD3D->m_KGCOption.Is3Back() )
        return;

    m_IsMPDamage = IsMPDamage;
	m_strText = strText;	
	
	D3DXVECTOR2 vecPos;
	if( 0.0f == fHitPosX && 0.0f == fHitPosY )
	{
		if( g_MyD3D->IsPlayerIndex( PlayerNum ) )
		{        
			vecPos.x = g_MyD3D->MyPlayer[PlayerNum]->vPos.x;
			vecPos.y = g_MyD3D->MyPlayer[PlayerNum]->vPos.y - 0.2f;
		}
		else if( g_kMonsterManager.IsMonsterIndex( PlayerNum-MAX_PLAYER_NUM) )
		{
			MONSTER* pMonster = g_kMonsterManager.GetMonster(PlayerNum-MAX_PLAYER_NUM);

			vecPos.x = pMonster->GetHitPos().x;
			vecPos.y = pMonster->GetHitPos().y - 0.2f;

			// �������� ���ð���ϱ� ������ ��Ʈ��ũ �÷��̾��� ��� HitPos�� ���õ��� ���� ���� �ֽ��ϴ�...
			if( (0.0f == vecPos.x) && (-1.0f <= vecPos.y && vecPos.y <= 1.0f) )
			{
				vecPos.x = pMonster->GetX();
				vecPos.y = pMonster->GetY() - 0.2f;
			}
		}
	}
	else
	{
		vecPos.x = fHitPosX;
		vecPos.y = fHitPosY - 0.3f;
	}

	EDamageTextType eDmgTextType;

    std::string ParticleText = "DamageTextCritical_";

    if( -1 < iTextType )
    {
        eDmgTextType = static_cast<EDamageTextType>(iTextType);
    }
    else
    {
        if( m_IsMPDamage )
        {
            eDmgTextType = EDAMAGE_TEXT_BLUE;
        }
        else if( dwAttackType & PAA_CRITICAL )
        {
            eDmgTextType = EDAMAGE_TEXT_CRITICAL;
            g_KDSound.Play( "critical_attack" );
            ParticleText = g_pFontSkinMgr->GetFontSkinParticle(Playersource);
        }
        else
        {        
            eDmgTextType = EDAMAGE_TEXT_NORMAL;
        }
    }

    // ������ ���� �������
	Num_Render_Particle( Playersource, ParticleText, eDmgTextType, vecPos.x - 1.0f, vecPos.y, Change );
}

HRESULT Headup::Frame_Move()
{
    if ( IsCheatMode )
    {
        switch(g_MyD3D->m_pStateMachine->GetState())
        {
        case GS_GAME:                

            if ( g_pkGameOverlayUI != NULL && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
            {
                if ( false == g_pkGameOverlayUI->HasEditFocus() )
                {
                    if ( (diks[DIK_RETURN] || diks[DIK_NUMPADENTER]) && !(Old_diks[DIK_RETURN] || Old_diks[DIK_NUMPADENTER] ))
                    {
                        g_pkGameOverlayUI->HandleChatEvent();
                    }
                }
            }
            break;            
        }
    }
    else
    {
        switch(g_MyD3D->m_pStateMachine->GetState())
        {
        case GS_GAME:  

            if ( g_pkGameOverlayUI != NULL && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
            {
                if ( false == g_pkGameOverlayUI->HasEditFocus() )
                {
                    if ( (diks[DIK_RETURN] || diks[DIK_NUMPADENTER]) && !(Old_diks[DIK_RETURN] || Old_diks[DIK_NUMPADENTER]))
                    {
                        /************************************************************************/
                        /* Replay View �����϶����� ä�ø�带 �ٲ��� �ʵ��� ����
                        2006-03-08 ����
                        /************************************************************************/
#pragma message("Replay removing")
//                         if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW || g_pkUIScene->IsMsgBoxModal() )
//                             return S_OK;

                        if ( g_pkGameOverlayUI->HasChatEventHandling() ) 
                        {
                            // ����Ʈ ��Ʈ���� ä�� ó������ ������ ���´�.
                            g_pkGameOverlayUI->SetChatEventHandling( false );
                        }
                        else
                        {
                            IsCheatMode = true;
                            g_pkGameOverlayUI->ShowChatToggleMsg( false );
                            g_pkGameOverlayUI->SetEditText( NULL );
                            g_pkGameOverlayUI->SetEditFocus();
                        }
                    }
                    else if( diks[DIK_F10] && !Old_diks[DIK_F10])//F10�� ������ �� ä��â�� �������� �Ѵ�.
                    {
                        g_pkGameOverlayUI->ToggleChatList();
                    }
                }
            }
            break;       
        }
    }

    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
        return E_FAIL;

    FrameMove_Combo();

    // ī�޶� �ٶ󺸰� �ִ� ��ġ ( ��ƼŬ trace�� ���� ��ǥ�Դϴ� )
    m_vCameraLookPos.x = g_kCamera.m_fLookX;
    m_vCameraLookPos.y = g_kCamera.m_fLookY;
    m_vCameraLookPos.z = 1.0f;

    MoonFestival();

    if( Show_Count )
    {
        Show_Count++;
        for(int i=0;i<30;i++)
        {
            if(Letter_Text[i]==NULL)
                break;
            if(Show_Count==i*5+2)
                g_KDSound.Play( "75" );
            if(Show_Count<(i+3)*5 && Show_Count>i*5)
                Show_Text[i]+=(1.0f-Show_Text[i])*.3f;
            else if(Show_Count<(i+3)*5+80 && Show_Count>i*5+80)
                Show_Text[i]*=.7f;
        }
        if(Show_Count > ((int)strlen(Letter_Text)+2)*5+80 )
            Show_Count=0;
    }

    // ���� �ڽ��� Life�� �����ִ� �� ����...
    static char Life=-1;
    if(Life!=g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_cLife)
    {
        Life=g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_cLife;
    }
    

    if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
    {
        if(SuperPoint>0)
        {
            SuperPoint-=.01f;
            if(SuperPoint<0)
                SuperPoint=0;
        }

        if(!IsResultComplete)
        {
            // ��Ȳ�� �ʱ�ȭ ����
            // ���� ������ �ִ��� ���� ������ 100
            for(int i=0;i<MAX_PLAYER_NUM;i++)
            {
                if(Old_Life[i]>0)
                {
                    if(Change_Life[i]!=g_MyD3D->MyPlayer[i]->m_cLife )
                    {
                        Change_Life[i]=g_MyD3D->MyPlayer[i]->m_cLife;
                        if(Panel_Show_Count<25)
                            Panel_Show_Count=100-Panel_Show_Count;
                        else if(Panel_Show_Count<75)
                            Panel_Show_Count=75;
                    }
                    if(Panel_Show_Count==75 && Change_Life[i]!=Old_Life[i])
                        Old_Life[i]=Change_Life[i];

                }
            }
        }

        if(Panel_Show_Count) Panel_Show_Count--;
    }

    RenderDamamgeNumber();

    return S_OK;
}

void Headup::Start_Text(const char* Text, bool BigLetter)
{
    IsBigLetter=BigLetter;
    Show_Count=1;
    for(int i=0;i<30;i++)
        Show_Text[i]=.01f;
    strcpy(Letter_Text,Text);
}


// �������� �ʴ� ���ڸ� ������
void Headup::Num_Render(float x, float y, const TCHAR* Text, DWORD Color, float Width, bool RightSort, bool Is2X, float Size)
{
    D3DXMATRIX mat1,mat2,mat3;

	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    
    V_Letter[0].Diff=V_Letter[1].Diff=Color;
    V_Letter[2].Diff=V_Letter[3].Diff=Color;


    int Feed=0;
    int Line=0;

    int iTexLen = (int)_tcsclen(Text);

    for(int i = 0; i < iTexLen; i++)
    {
        int T;
        if( RightSort )
            T = iTexLen - i -1;
        else
            T=i;

        if(Text[T]>_T('A')-1 && Text[T]<_T('H'))
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('A'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('A')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=0;
            V_Letter[2].tv=V_Letter[3].tv=.1666f;
        }
        else if(Text[T]>_T('G') && Text[T]<_T('O'))
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('H'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('H')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.1666f;
            V_Letter[2].tv=V_Letter[3].tv=.3333f;
        }
        else if(Text[T]>_T('N') && Text[T]<_T('V'))
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('O'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('O')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.3333f;
            V_Letter[2].tv=V_Letter[3].tv=.5f;
        }
        else if(Text[T]>_T('U') && Text[T]<_T('Z')+3)
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('V'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('V')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.5f;
            V_Letter[2].tv=V_Letter[3].tv=.6666f;
        }
        else if(Text[T]>_T('0')-1 && Text[T]<_T('7'))
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('0'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('0')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.6666f;
            V_Letter[2].tv=V_Letter[3].tv=.8333f;
        }
        else if(Text[T]>_T('6') && Text[T]<_T('9')+4)
        {
            V_Letter[0].tu=V_Letter[2].tu=(float)(Text[T]-_T('7'))/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Text[T]-_T('7')+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.8333f;
            V_Letter[2].tv=V_Letter[3].tv=1.0f;
        }
        else if(Text[T]==_T('.'))
        {
            Line++;
            Feed=0;
            if(Text[T+1]==_T('U'))
            {
                BYTE Alpha=(BYTE)(Color>>24);
                V_Letter[0].Diff=V_Letter[1].Diff=D3DCOLOR_ARGB(Alpha,255,100,0);
                V_Letter[2].Diff=V_Letter[3].Diff=D3DCOLOR_ARGB(Alpha,150,30,0);
            }
            else
            {
                BYTE Alpha=(BYTE)(Color>>24);
                V_Letter[0].Diff=V_Letter[1].Diff=Color;
                V_Letter[2].Diff=V_Letter[3].Diff=D3DCOLOR_ARGB(Alpha,55,55,55);
            }
            
            continue;
        }
        else if(Text[T]==_T(' '))
        {
            if(RightSort) Feed++;
            else Feed--;
            
            continue;
        }
        else
        {
            int T_T=10;
            if(Text[T]==_T('+')) T_T=4;
            else if(Text[T]==_T('-')) T_T=5;
            else if(Text[T]==_T('%')) T_T=6;
            V_Letter[1].tu=V_Letter[3].tu=(float)(T_T)/6.0f;
            V_Letter[0].tu=V_Letter[2].tu=(float)(T_T+1)/6.0f;
            V_Letter[0].tv=V_Letter[1].tv=.8333f;
            V_Letter[2].tv=V_Letter[3].tv=1.0f;
        }
        

        if(Is2X) D3DXMatrixScaling ( &mat2, .514f*Size, .6f*Size, 1.0f );
        else D3DXMatrixScaling ( &mat2, .257f*Size, .3f*Size, 1.0f );
        D3DXMatrixTranslation( &mat3, x-Width*(float)Feed*Size, y-(float)Line*.11f, .11f );
        D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
		g_pGCDeviceManager2->DrawInWorld(m_pTexLetter,&mat1,V_Letter);

        if(RightSort) Feed++;
        else Feed--;
    }
    g_pGCDeviceManager2->PopState();

    D3DXMatrixIdentity( &mat1 );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
}

void Headup::Letter_Render(float x, float y)
{
    if(!Show_Count) return;
    float Letter2X=1.0f;
    if(IsBigLetter) Letter2X=1.5f;
    D3DXMATRIX mat1,mat2,mat3;

    float Pitch_x=-(float)strlen(Letter_Text)*.05f+.05f;

    int iSize = (int)strlen(Letter_Text);
    for(int i = 0 ; i < iSize ; i++)
    {
        if(Show_Text[i]<.01f || Letter_Text[i]==' ')
        {
            Pitch_x+=.1f;
            continue;
        }
        BYTE Color=250-(BYTE)(Show_Text[i]*250.0f);
        V_Letter[0].Diff=V_Letter[1].Diff=V_Letter[2].Diff=V_Letter[3].Diff=D3DCOLOR_ARGB(50+(BYTE)(Show_Text[i]*150.0f),0,0,0);
        if(Letter_Text[i]>'A'-1 && Letter_Text[i]<'H'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'A')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'A'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=0;
            V_Letter[2].tv=V_Letter[3].tv=.1666f;
        }else if(Letter_Text[i]>'G' && Letter_Text[i]<'O'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'H')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'H'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.1666f;
            V_Letter[2].tv=V_Letter[3].tv=.3333f;
        }else if(Letter_Text[i]>'N' && Letter_Text[i]<'V'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'O')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'O'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.3333f;
            V_Letter[2].tv=V_Letter[3].tv=.5f;
        }else if(Letter_Text[i]>'U' && Letter_Text[i]<'Z+3'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'V')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'V'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.5f;
            V_Letter[2].tv=V_Letter[3].tv=.6666f;
        }else if(Letter_Text[i]>'0'-1 && Letter_Text[i]<'7'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'0')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'0'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.6666f;
            V_Letter[2].tv=V_Letter[3].tv=.8333f;
        }else if(Letter_Text[i]>'6'){
            V_Letter[0].tu=V_Letter[2].tu=(float)(Letter_Text[i]-'7')/7.0f;
            V_Letter[1].tu=V_Letter[3].tu=(float)(Letter_Text[i]-'7'+1)/7.0f;
            V_Letter[0].tv=V_Letter[1].tv=.8333f;
            V_Letter[2].tv=V_Letter[3].tv=1.0f;
        }else{
         
            continue;
        }


        short Alpha=50+(BYTE)(Show_Text[i]*200.0f);
        if(Alpha>255) Color=255;
        V_Letter[0].Diff=V_Letter[1].Diff=V_Letter[2].Diff=V_Letter[3].Diff=D3DCOLOR_ARGB(255/*Alpha*/,255,255,0);

        if(Show_Count>i*5+80){
            D3DXMatrixScaling ( &mat1, .4f*Letter2X, (4.9f-Show_Text[i]*4.5f)*Letter2X, 1.0f );
            D3DXMatrixRotationY(&mat3, (1.0f-Show_Text[i])*1.57f);
            D3DXMatrixMultiply( &mat2, &mat1, &mat3 );
            D3DXMatrixTranslation( &mat3, Pitch_x+x, y, .11f );
            D3DXMatrixMultiply( &mat1, &mat2, &mat3 );

            
			g_pGCDeviceManager2->DrawInWorld(m_pTexLetter,&mat1,V_Letter);
        }else{

            // �׸��ڰ�
            Color=(BYTE)((1.0f-Show_Text[i])*200.0f);
            V_Letter[0].Diff=V_Letter[1].Diff=V_Letter[2].Diff=V_Letter[3].Diff=D3DCOLOR_ARGB(255,255,255,0);
            

            D3DXMatrixScaling ( &mat2, (1.5f-Show_Text[i]*1.1f)*Letter2X, (1.5f-Show_Text[i]*1.1f)*Letter2X, 1.0f );
            D3DXMatrixTranslation( &mat3, (Pitch_x+x)*(3.0f-Show_Text[i]*2.0f), y, .11f );
            D3DXMatrixMultiply( &mat1, &mat2, &mat3 );

			g_pGCDeviceManager2->DrawInWorld(m_pTexLetter,&mat1,V_Letter);
        }
        Pitch_x+=.1f;
    }

    D3DXMatrixIdentity( &mat1 );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
}

void Headup::Render()
{
    if( GS_GAME != g_MyD3D->m_pStateMachine->GetState() )
        return;

        //Draw_HP_Changer();
        Letter_Render();
        //Render_Combo();
        Render_ExpGp();
        //Render_Title();
}

void Headup::Alpha_Render()
{
    D3DXMATRIX mat1,mat2,mat3;
    SLVERTEX* v = NULL;
    int Index=g_MyD3D->Get_MyPlayer();

        g_pd3dDevice->SetFVF( FVF_LVERTEX );

	
	

        // ȭ�� ��½
        if(White_Count==1 || White_Count==2)
        {
		g_pGCDeviceManager2->PushState();
		g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
            D3DXMatrixTranslation( &mat1, 0, 0, .5f );
		g_pGCDeviceManager2->DrawInWorld(NULL,&mat1,2.0f, 1.5f, 0,0,0,0);
		g_pGCDeviceManager2->PopState();
		
        }
        // ���� ���� �� �׸���
        if(Show_Count)
        {
        
            float Height=Show_Text[0];
            float Height2=Show_Text[strlen(Letter_Text)-1];
            if(Height<Height2) Height=Height2;
            if(Height>.1f)
            {
			g_pGCDeviceManager2->PushState();
            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

			
                D3DXMatrixTranslation( &mat1,0,-.01f,.8f );
            //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
            //g_MyD3D->Draw_Billboard(2.0f,Height*.12f,.0195f,.91f,.3789f,.91f,255,0,0,100);
			g_pGCDeviceManager2->DrawInWorld(NULL,&mat1,
				2.0f,Height*.12f,.0195f,.91f,.3789f,.91f,0,0,0,
				D3DCOLOR_ARGB(255,0,0,100));

                D3DXMatrixTranslation( &mat1,-.5f,.08f*Height-.01f,.8f );
//            g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
  //          g_MyD3D->Draw_Billboard(Height*4.0f,.02f,.0195f,.91f,.3789f,.91f,255,0,0,100);
			g_pGCDeviceManager2->DrawInWorld(NULL,&mat1,
				Height*4.0f,.02f,.0195f,.91f,.3789f,.91f,0,0,0,
				D3DCOLOR_ARGB(255,0,0,100));

                D3DXMatrixTranslation( &mat1,.2f,-.07f*Height-.01f,.8f );
//            g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
  //          g_MyD3D->Draw_Billboard(Height*2.4f,.005f,.0195f,.91f,.3789f,.91f,255,0,0,100);
			g_pGCDeviceManager2->DrawInWorld(NULL,&mat1,
				Height*2.4f,.005f,.0195f,.91f,.3789f,.91f,0,0,0,
				D3DCOLOR_ARGB(255,0,0,100));

			g_pGCDeviceManager2->PopState();
            }
        }

    D3DXMatrixIdentity( &mat1 );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
}

void Headup::Render_ExpGp()
{
    // Exp �ö󰡴°� �����ְ���..
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );

        for( int i = 0 ; i < (int)m_vecShowExpGpInfo.size() ; ++i )
        {
            if( m_vecShowExpGpInfo[i].fRenderTime <= 0.5f )
            {
                TCHAR eText[15] = { 0, };
                long Change = m_vecShowExpGpInfo[i].iExp;
                if( Change >= 0 )
                {
                    if( m_vecShowExpGpInfo[i].iKind == KGCExpGpInfo::EG_EXP )
                    {
                        _stprintf( &eText[4], _T("%d"), Change );
                        eText[0] = 'E';
                        eText[1] = 'X';
                        eText[2] = 'P';
                        eText[3] = ' ';
                    }
                    else
                    {
                        _stprintf( &eText[3], _T("%d"), Change );
                        eText[0] = 'G';
                        eText[1] = 'P';
                        eText[2] = ' ';
                    }

                    float fPosX = m_vecShowExpGpInfo[i].fPosX;
                    float fPosY = m_vecShowExpGpInfo[i].fPosY;

                    D3DXVECTOR3 vExpPos;
                    D3DXVec3TransformCoord( &vExpPos, &D3DXVECTOR3( fPosX, fPosY, 0 ), &g_kCamera.m_matCamera );

                    if( m_vecShowExpGpInfo[i].fRenderTime <= 0.25f )
                    {
                        Num_Render( vExpPos.x , vExpPos.y + m_vecShowExpGpInfo[i].fRenderTime, eText, D3DCOLOR_ARGB(255,255,255,255), 0.04f );
                    }
                    else
                    {
                        int iAlpha = (int)( 425.0f - 425.0f * 2.0f * m_vecShowExpGpInfo[i].fRenderTime );
                        if(iAlpha < 0 )
                            iAlpha = 0;

                        Num_Render( vExpPos.x , vExpPos.y + 0.3f , eText, D3DCOLOR_ARGB(iAlpha,255,255,255), 0.04f );
                    }
                }
                m_vecShowExpGpInfo[i].fRenderTime += 0.006f;
            }
        }

        for ( int i = (int)m_vecShowExpGpInfo.size() - 1; i >= 0; --i )
        {
            if( m_vecShowExpGpInfo[i].fRenderTime > 0.5f )
            {
                m_vecShowExpGpInfo.erase( m_vecShowExpGpInfo.begin() + i );
            }
        }
    }
    g_pGCDeviceManager2->PopState();
}

void Headup::InitDataForTutorial()
{
    White_Count = 0;
    Level = 2;
    Panel_Show_Count = 0;
    IsCheatMode = false;
	IsBlackMagic = true;

}

void Headup::Clear()
{
    Letter_Text[0] = ' ';
    Letter_Text[1] = ' ';
    SetReservedMessage( L"" );

    SuperPoint = 0;
    White_Count = 0;
    // IsBlackMagic = true;
    // Level = 0;

    Panel_Show_Count = 0;
    IsCheatMode = false;

    // ���߿� Exp ��� â�� ������ �� �ʿ�
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        Score_Pitch[i] = -3.0f;
        Score_Bright[i] = 0;
    }

    for(int i=0;i<10;i++)
        Show_Text[i]=.01f;

    if( m_pExpEvent1 != NULL )
        g_ParticleManager->DeleteSequence( m_pExpEvent1 );

    if( m_pExpEvent2 != NULL )
        g_ParticleManager->DeleteSequence( m_pExpEvent2 );

    m_pExpEvent1 = NULL;
    m_pExpEvent2 = NULL;
    
    Clear_All_Cheat();
}

void Headup::CreateEventParticle()
{
    if( m_pExpEvent1 == NULL && m_pExpEvent2 == NULL )
    {
        string strSequenceName1;
        string strSequenceName2;
        bool bEnableEvent = false;

        if( g_kGlobalValue.m_kChattingEvent.GetSuccessEvent() )
        {
#if defined( NATION_TAIWAN )
            strSequenceName1 = "harvest_event_tw";
#elif defined( NATION_HONGKONG )
            strSequenceName1 = "harvest_event_hk";
#else
            strSequenceName1 = "kor_independent_event";
#endif
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_VALENTINE ) )
        {
            strSequenceName1 = "exp_event";
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_GP_EXP ) ||
            SiKGCEventManager()->IsActiveEvent( EVENT_ID_CARNATION ) ||
            SiKGCEventManager()->IsActiveEvent( EVENT_ID_GP2_PVPMODE ) || 
            SiKGCEventManager()->IsActiveEvent( EVENT_ID_TAG_MATCH ) ||
			SiKGCEventManager()->IsActiveEvent( EVENT_ID_HEART_WING ) ||
			SiKGCEventManager()->IsActiveEvent( EVENT_ID_CONTINENT_EXP_GP ) ||
            SiKGCEventManager()->IsActiveEvent( EVENT_ID_EXP_BONUS )
			)
        {
            strSequenceName1 = "exp_gp_event";
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_MOON_FESTIVAL ) ) // �߼� ( �޶ߴ� �̺�Ʈ )
        {
            strSequenceName1 = "exp_gp_event";
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_SUN_FESTIVAL ) ) // �� ( �ضߴ� �̺�Ʈ )
        {
            strSequenceName1 = "exp_gp_event";
            bEnableEvent = true;
        }
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_LOI_FESTIVAL ) ) // �±� ( ���̲����� �̺�Ʈ )
		{
			strSequenceName1 = "exp_gp_event";
			bEnableEvent = true;
		}
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_WHITEDAY ) ) // ȭ��Ʈ����
        {
            strSequenceName1 = "exp_gp_event";
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_AMY ) ) // ���̹� �̺�Ʈ
        {
            strSequenceName1 = "amy_event";
            bEnableEvent = true;
        }
        else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_DEATHMATCH_BONUS ) &&
                 SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ) // ������ġ �̺�Ʈ
        {
            strSequenceName1 = "deathmatch_bonus";
            bEnableEvent = true;
        }
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_WELCOME_SEASON2) ) //����2 ȯ�� �̺�Ʈ
		{
			strSequenceName1 = "exp_gp_event";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_FRIENDS_DAY ) )
		{
			strSequenceName1 = "exp_gp_event";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_EMBLEM ) )
		{
			strSequenceName1 = "skilltree_event";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_ABYSSKNIGHT ) )
		{
			strSequenceName1 = "abyssknight_event";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_FIGHTDAY ) )
		{
			strSequenceName1 = "pvpday";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_1010_DAY_GP100 ) )
		{
			strSequenceName1 = "double10_100";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_BLUESTONE_DUNGEON ) )
		{
			strSequenceName1 = "bluestonedungeon";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_AMY_TOGETHER ) ) // ȭ��Ʈ����
		{
			strSequenceName1 = "exp_gp_event";
			bEnableEvent = true;
		}
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_ARME_SKILLTREE ) )
		{
			strSequenceName1 = "armeskilltree_event";
			bEnableEvent = true;
		}
		//else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_RONAN_TOGETHER ) )
		//{
		//	for(int i = 0 ; i < MAX_PLAYER_NUM; ++i)
		//	{
		//		if(SiKGCRoomManager()->GetGameModeCategory() ==  GC_GMC_DUNGEON   &&g_MyD3D->GetPlayerNum() > 1&&g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && GC_CHAR_RONAN == g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType && g_MyD3D->IsMyTeam(i))
		//		{
		//			strSequenceName1 = "ronantogether";
		//			bEnableEvent = true;
		//			break;
		//		}
		//	}
		//}		

        if( bEnableEvent )
        {
            strSequenceName2 = "exp_event_particle";
            m_pExpEvent1 = g_ParticleManager->CreateSequence( strSequenceName1, 0.0f, -0.15f, 1.0f );
            m_pExpEvent2 = g_ParticleManager->CreateSequence( strSequenceName2, 0.0f, -0.15f, 1.0f );

            // �̺�Ʈ ǥ�ÿ� ��ƼŬ���� ������ ȭ�� �߾ӿ� ��ġ��Ű�� �ʹ�
   //         TRACE_INFO stTrace;
   //         stTrace.m_pvPos = &m_vCameraLookPos;
   //         stTrace.m_fParticleTraceTime = 5.0f;

   //         m_pExpEvent1->SetTrace( &stTrace );
			//m_pExpEvent2->SetTrace( &stTrace );

            if( m_pExpEvent1 != NULL && m_pExpEvent2 != NULL )
            {
			    g_ParticleManager->SetLayer( m_pExpEvent1, GC_LAYER_UI );
			    g_ParticleManager->SetLayer( m_pExpEvent2, GC_LAYER_UI );
            }
        }
    }
}

void Headup::SetRenderExpGp( int iPlayerIndex, int iValue, int iKind , D3DXVECTOR2 vPos )
{
    KGCExpGpInfo kExpInfo;
    kExpInfo.iKind = iKind;
    kExpInfo.iPlayerIndex = iPlayerIndex;
    kExpInfo.iExp = iValue;
    kExpInfo.fRenderTime = 0.0f;

    if( iKind == KGCExpGpInfo::EG_EXP)
    {
        if( g_MyD3D->Get_MyPlayer() == iPlayerIndex )
        {
            kExpInfo.fPosX = g_kLocalPlayerLatency->kRenderData[0].vPos.x - 1.0f + 0.15f;
            kExpInfo.fPosY = g_kLocalPlayerLatency->kRenderData[0].vPos.y - 0.3f;
        }
        else
        {
            kExpInfo.fPosX = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x - 1.0f + 0.15f;
            kExpInfo.fPosY = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.y - 0.3f;
        }
    }
    else
    {
        kExpInfo.fPosX = vPos.x;
        kExpInfo.fPosY = vPos.y;
    }

    m_vecShowExpGpInfo.push_back( kExpInfo );
}

void Headup::MoonFestival()
{
    // ���� ���� �� 2�� �Ŀ�..
    // ������ ������ ��� �о� ��Ÿ�� ������ ������.
    // ���� ��ƼŬ ��� �ÿ��� emblem.lua �� �߰������ּ���~!!!!
    if( StartGameTime  == GC_FPS_LIMIT * 2 )
    {
        // Title_Emblem �������� �ʱ�ȭ
        m_vecTitle.clear();
        m_mapTitleID.clear();
        m_strEmblemParticleName.clear();

        if(SiGCEclipseEventManager()->IsEventTime()){
            if(SiGCEclipseEventManager()->IsActiveEvent())
                m_strEmblemParticleName.push_back("event_eclipse_emblem");
        }

        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL )
            return;

        KLuaManager luaMgr;

        if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
            return;

        if( GCFUNC::LoadLuaScript( luaMgr , "emblem.lua" ) == false )
            return ;
        LUA_BEGIN_TABLE( "EVENT_EMBLEM", ;)
            for( int i = 1; ; i++ ){
                LUA_BEGIN_TABLE(i,break)
                    bool b_OK = false;
                bool bGameMode = false;
                bool bOnly = false;
                int	   iEventID = -1;
                int	   GameMode = -1;

                LUA_BEGIN_TABLE("IS_ACTIVE_EVENT",;)
                    for(int i=1;;i++)
                    {
                        LUA_GET_VALUE_NOASSERT(i,iEventID,/*IS_ACTIVE_EVENT[i]*/break);
                        if( SiKGCEventManager()->IsActiveEvent(iEventID)){
                            b_OK = TRUE;
                            break;
                        }
                    }
                    LUA_END_TABLE(/*IS_ACTIVE_EVENT*/;);
                    if(!b_OK){	LUA_END_TABLE(/* EVENT_EMBLEM[i] */;) continue;}

                    LUA_BEGIN_TABLE("GAMEMODE",;)
                        for(int i=1;;i++)
                        {
                            LUA_GET_VALUE_NOASSERT(i,GameMode,/*IS_ACTIVE_EVENT[i]*/break);
                            if( SiKGCRoomManager()->GetGameModeCategory() ==GameMode){
                                bGameMode = TRUE;
                                break;
                            }
                        }
                        LUA_END_TABLE(/*GAMEMODE*/;);
                        if(!bGameMode){	LUA_END_TABLE(/* EVENT_EMBLEM[i] */;) continue;}

                        LUA_GET_VALUE_DEF("B_ONLY",bOnly,false);

                        if(bOnly)
                            m_strEmblemParticleName.clear();

                        string strSequenceName;
                        LUA_GET_VALUE_NOASSERT( "STR_PARTICLE_NAME", strSequenceName, break );
                        m_strEmblemParticleName.push_back(strSequenceName);
                        LUA_END_TABLE(/*  EVENT_EMBLEM[i] */;);
                        if(bOnly)
                            break;
            }
            LUA_END_TABLE(/* EVENT_EMBLEM */;);

            LUA_BEGIN_TABLE_RAII( "TITLE_EMBLEM" )
            {
                for( int i = 1; ; i++ ){
                    LUA_BEGIN_TABLE(i,break)
                        bool bGameMode = false;
                    int GameMode = -1;
                    int iTtitleID = -1;

                    LUA_GET_VALUE_NOASSERT( "TITLE_ID", iTtitleID, break );

                    LUA_BEGIN_TABLE("GAMEMODE",;)
                        for(int i=1;;i++)
                        {
                            LUA_GET_VALUE_NOASSERT(i,GameMode,/*IS_ACTIVE_EVENT[i]*/break);
                            if( SiKGCRoomManager()->GetGameModeCategory() ==GameMode){
                                bGameMode = TRUE;
                                break;
                            }
                        }
                        LUA_END_TABLE(/*GAMEMODE*/;);
                        if(!bGameMode){	LUA_END_TABLE(/* EVENT_EMBLEM[i] */;) continue;}

                        string strSequenceName;
                        int iBuffID;
                        LUA_GET_VALUE_NOASSERT( "STR_PARTICLE_NAME", strSequenceName, break );
                        LUA_GET_VALUE_NOASSERT( "BUFF_ID", iBuffID, break );


                        int nPlyer = g_MyD3D->Get_MyPlayer();
                        // ���� ȿ���� ���� Īȣ�� ���õǵ��� ����!
                        for( int i = 0; i < MAX_PLAYER_NUM; i++ )
                        {
                            if( g_MyD3D->MyPlayer[i]->IsLive() && 
                                g_MyD3D->IsSameTeam( nPlyer, i ) &&
                                g_pItemMgr->IsEquipTitleItem( &g_MyD3D->MyPlayer[i]->GetCurrentChar(), iTtitleID) )
                            {
                                // �Ʒ��ּ� : ������ �ؽ��� ���°� ����
                                // m_strEmblemParticleName.push_back(strSequenceName);
                                m_mapTitleID.insert(std::make_pair(iTtitleID, std::make_pair(iBuffID, i)));
                            }
                        }
                        LUA_END_TABLE(/*  EVENT_EMBLEM[i] */;);
                }

#if !defined( NATION_KOREA ) 
                if( 0 < int(m_mapTitleID.size()) )
                    SetTitleEmblem( m_mapTitleID );
#else 
                if( SiKGCChannelManager()->IsPVPBallanceServer() )
                {  
                    // ����ȭ + �����̸� ������
                }
                else { 
                    if( 0 < int(m_mapTitleID.size()) )
                        SetTitleEmblem( m_mapTitleID );
                }
#endif
            }
    }

	//4�ʸ��� �������� �ִٸ� ��Ÿ����~!
	if( !(StartGameTime  %( GC_FPS_LIMIT * 4 )) )
	{
		if(int nStrEmblem = StartGameTime  /( GC_FPS_LIMIT * 4 ))
		{
			if(nStrEmblem<= static_cast<int>( m_strEmblemParticleName.size()) )
			{
				CParticleEventSeqPTR pTemp;
				TRACE_INFO stTrace;
				stTrace.m_pvPos = &m_vCameraLookPos;
				stTrace.m_fParticleTraceTime = 5.0f;

				pTemp = g_ParticleManager->CreateSequence( m_strEmblemParticleName[nStrEmblem-1], m_vCameraLookPos.x, m_vCameraLookPos.y, m_vCameraLookPos.z );
                if (pTemp)
				    pTemp->SetTrace( &stTrace );
					
				pTemp = g_ParticleManager->CreateSequence( "harvest02", m_vCameraLookPos.x, m_vCameraLookPos.y, m_vCameraLookPos.z );
				if (pTemp)
					pTemp->SetTrace( &stTrace );
					
				pTemp = g_ParticleManager->CreateSequence( "harvest03", m_vCameraLookPos.x, m_vCameraLookPos.y, m_vCameraLookPos.z );
				if (pTemp)
					pTemp->SetTrace( &stTrace );

                if( m_strEmblemParticleName[nStrEmblem-1] == "event_eclipse_emblem" )
                {
                    KSignBoardData kTempSignboard;
                    kTempSignboard.m_eType = KSignBoardData::SBT_SERVER_ADMIN;
                    kTempSignboard.m_strMsg = g_pkStrLoader->GetString(STR_ID_ECLIPSE_TIME_TITLE);
                    SiKGCAnnounceMgr()->ReserveAnnounce( kTempSignboard );

                }

			}
		}
	}

    // Ÿ��Ʋ ���� ���( �ں����� ���Ŀ� ó�� )
    if( false == m_vecTitle.empty() && !(StartGameTime  %( GC_FPS_LIMIT * 4 )) )
    {
        int nStrEmblem = StartGameTime  / ( GC_FPS_LIMIT * 4 );
        if( nStrEmblem > static_cast<int>( m_strEmblemParticleName.size() ) )
        {
            nStrEmblem -= static_cast<int>( m_strEmblemParticleName.size() );
            if( nStrEmblem <= static_cast<int>( m_vecTitle.size()) && g_MyD3D->IsPlayerIndex(m_vecTitle[nStrEmblem-1].second) )
            {
                std::wstring wstrTemp = g_pkStrLoader->GetReplacedString( STR_ID_DOTA_TITLE_BUFF, "ss", g_MyD3D->MyPlayer[ m_vecTitle[nStrEmblem-1].second ]->GetStrUserName().c_str(), 
                    g_pItemMgr->GetItemName( m_vecTitle[nStrEmblem-1].first ).c_str() );

                // ����Ʈ Ÿ��Ʋ �޽��� ���( �������� �ɾ���� �ȴ�. )
                //float fX = (800.0f * g_pGCDeviceManager->GetWindowScaleX()) / 2.0f;
                //float fY = GC_SCREEN_HEIGHT * g_pGCDeviceManager->GetWindowScaleY()  / 3.0f;
                //g_pkFontMgrTitle->OutTextMultiline( int(fX), int(fY), wstrTemp.c_str(), D3DCOLOR_ARGB( 255, 255, 255, 255 ), 1.0f, NULL, DT_CENTER );

                // ��� ���� ���
                KSignBoardData kTempSignboard;
                kTempSignboard.m_eType = KSignBoardData::SBT_SERVER_ADMIN;
                kTempSignboard.m_strMsg = wstrTemp;
                SiKGCAnnounceMgr()->ReserveAnnounce( kTempSignboard );
            }
        }
    }
}

void Headup::SetTitleEmblem()
{
    SetTitleEmblem( m_mapTitleID );
}

void Headup::SetTitleEmblem( std::map<int,std::pair<int,int>>& mapTitleID )
{
	const int nDotaTitleNum = 4;
	//	������ ���Դϴ�. 
	const int DotaTitleID[nDotaTitleNum] = { 61147, 61146, 61145, 61144 };

	bool bSelectTitle = false;

	for ( int i = 0; i < nDotaTitleNum; ++i  )
	{
		if ( bSelectTitle )
		{
			mapTitleID.erase( DotaTitleID[i] );
			continue;
		}

		if ( mapTitleID.find( DotaTitleID[i] ) != mapTitleID.end() )
		{
			bSelectTitle = true;		
		}
	} 

	std::map<int,std::pair<int,int>>::iterator	pos = mapTitleID.begin();
	std::map<int,std::pair<int,int>>::iterator	pos_end = mapTitleID.end();

    int iDotaTitleLength = sizeof(DotaTitleID)/sizeof(int);
	for ( ; pos != pos_end; ++pos )
	{
		int iBuffID = pos->second.first;

        // ������ Īȣ �����̸� Ÿ��Ʋ ������ֵ��� �����ϱ�
        for(int iLoop=0; iLoop<iDotaTitleLength; ++iLoop)
        {
            if( pos->first == DotaTitleID[iLoop] )
                m_vecTitle.push_back( std::make_pair(DotaTitleID[iLoop], pos->second.second) );
        }
        
        // ���� ���� ���̸� ������ �ɾ��ش�.
		for( int j = 0; j < MAX_PLAYER_NUM; j++ )
		{
			if( g_kGlobalValue.m_kUserInfo.iTeam == g_MyD3D->MyPlayer[j]->m_kUserInfo.iTeam )
            {
				g_MyD3D->MyPlayer[j]->SetMagicEffect(iBuffID, 9999999 );
		}  
	}
}
}

void Headup::SetComboTimer( bool bSetCurtime_, DWORD dwBreakGap_ )
{
    if ( bSetCurtime_ )
    {
        Combo_Time = ::timeGetTime();
    }

    m_dwBreakGap = dwBreakGap_;
}

void Headup::ComboClear( void )
{
    Show_Count = 0;
    ShowCombo = false;
    Combo_Count = 0;
    bEventComboCheck = false;
    bDisappearComboCheck = false;
    bTotalScoreEffect = false;
    Combo_Count_Aerial = 0;
    Combo_HP = 0;
    Show_Combo_HP = 0;
    Combo_Pitch = 0;
    Combo_Time = ::timeGetTime();
    m_dwBreakGap = 2000;
}

void Headup::Num_Render_Particle(int Playersource, std::string ParticleText, EDamageTextType eDmgType, float fPosX, float fPoxY, float usHP )
{
    TCHAR Text[MAX_PATH] = { 0, };
    _stprintf( Text, _T("%d"), (int)usHP );

    char str[MAX_PATH] = {0,};
    char strExtraNumber[MAX_PATH] = {0,};

    int iTextlen = (int)_tcsclen(Text);

    int iextraParticles = 0;

    KDamageString kDamageString;
    KDamageString kDamageExtraString;
    KDamageString kDamageExtraOther;

    std::string m_ExtraNumberParticle = "";
    std::string m_ExtraParticles = "";

    if ( eDmgType == EDAMAGE_TEXT_CRITICAL )
    {
        m_ExtraNumberParticle = g_pFontSkinMgr->GetExtraNumberFontSkinParticle( Playersource );
        m_ExtraParticles = g_pFontSkinMgr->GetExtraFontSkinParticle( Playersource );
    }

    for( int i = 0; i < iTextlen; i++ )
    {
        string strSeq, strSeqExtraNumber;
        for( int j = 0; j < EDAMAGE_TEXT_MAX_NUM; j++ )
        {
            if( Text[i] == j + '0' )
            {
                if ( eDmgType == EDAMAGE_TEXT_NORMAL && g_pkQuestManager->GetAttackTypeState() == 0 )
                    sprintf_s( str, "DamageTextBack_%d", j );
                else if (eDmgType == EDAMAGE_TEXT_NORMAL && g_pkQuestManager->GetAttackTypeState() == 1 )
                    sprintf_s( str, "DamageTextAerial_%d", j );
                else if ( eDmgType == EDAMAGE_TEXT_NORMAL )
                    sprintf_s( str, "DamageTextNormal_%d", j );
                else if (eDmgType == EDAMAGE_TEXT_CRITICAL)
                {
                    sprintf_s(str, "%s%d", ParticleText.c_str(), j); //Sistema de Skin de Dano
                    if (m_ExtraNumberParticle != "")
                        sprintf_s(strExtraNumber, "%s%d", m_ExtraNumberParticle.c_str(), j);
                }
                strSeq = str;
                strSeqExtraNumber = strExtraNumber;
                break;
            }
        }
        kDamageString.vecDamageString.push_back( strSeq );
        kDamageExtraString.vecDamageString.push_back( strSeqExtraNumber );
        kDamageExtraOther.vecDamageString.push_back( m_ExtraParticles );
    }
    g_pkQuestManager->SetAttackTypeState(-1);

    if (eDmgType == EDAMAGE_TEXT_VIOLET || eDmgType == EDAMAGE_TEXT_CRITICAL)
    {
        kDamageString.fInterval = 0.08f;
        if ( m_ExtraNumberParticle != "" )
            kDamageExtraString.fInterval = 0.08f;
        if( m_ExtraParticles != "" )
            kDamageExtraOther.fInterval = 0.08f;
    }
    else
    {
        kDamageString.fInterval = 0.05f;
    }

    kDamageString.fPosX = fPosX - ( kDamageString.vecDamageString.size() - 1 ) * 0.5f * kDamageString.fInterval;
    kDamageString.fPosY = fPoxY - 0.1f;
    kDamageString.iFrameCount = 0;

    if ( m_ExtraNumberParticle != "" )
    {
        kDamageExtraString.fPosX = kDamageString.fPosX;
        kDamageExtraString.fPosY = fPoxY - 0.1f;
        kDamageExtraString.iFrameCount = 0;
    }

    if ( m_ExtraParticles != ""  )
    {
        kDamageExtraOther.fPosX = kDamageString.fPosX;
        kDamageExtraOther.fPosY = fPoxY - 0.1f;
        kDamageExtraOther.iFrameCount = 0;
    }

    m_vecDamageString.push_back( kDamageString );
    if ( m_ExtraNumberParticle != "" )
        m_vecDamageString.push_back( kDamageExtraString );
    if ( m_ExtraParticles != ""  )
        m_vecDamageString.push_back( kDamageExtraOther );

	if( eDmgType == EDAMAGE_TEXT_CRITICAL )
	{		
        if ( kDamageString.vecDamageString.size() > 2 )
        {
            kDamageString.fPosX = fPosX + kDamageString.vecDamageString.size() * 0.25f * kDamageString.fInterval;
            if ( m_ExtraNumberParticle != "" )
                kDamageExtraString.fPosX = kDamageString.fPosX;
            if ( m_ExtraParticles != ""  )
                kDamageExtraOther.fPosX = kDamageString.fPosX;
        }
        else
        {
            kDamageString.fPosX = fPosX + kDamageString.fInterval;
            if ( m_ExtraNumberParticle != "" )
                kDamageExtraString.fPosX = kDamageString.fPosX;
            if ( m_ExtraParticles != ""  )
                kDamageExtraOther.fPosX = kDamageString.fPosX;
        }
		
        kDamageString.fPosY = fPoxY -0.1f;
		kDamageString.fInterval = 0.0f;
		kDamageString.vecDamageString.clear();
		kDamageString.vecDamageString.push_back( "Critical_D_T_BG" );
		kDamageString.vecDamageString.push_back( "Critical_D_T_Line" );

        m_vecDamageString.push_back(kDamageString);

        if ( m_ExtraNumberParticle != "" )
        {
            kDamageExtraString.fPosY = fPoxY - 0.1f;
            kDamageExtraString.fInterval = 0.0f;
            kDamageExtraString.vecDamageString.clear();
            kDamageExtraString.vecDamageString.push_back( "Critical_D_T_BG" );
            kDamageExtraString.vecDamageString.push_back( "Critical_D_T_Line" );

            m_vecDamageString.push_back( kDamageExtraString );
        }

        if ( m_ExtraParticles != ""  )
        {
            kDamageExtraOther.fPosY = fPoxY - 0.1f;
            kDamageExtraOther.fInterval = 0.0f;
            kDamageExtraOther.vecDamageString.clear();
            kDamageExtraOther.vecDamageString.push_back( "Critical_D_T_BG" );
            kDamageExtraOther.vecDamageString.push_back( "Critical_D_T_Line" );

            m_vecDamageString.push_back( kDamageExtraOther );
        }
	}
}

void Headup::RenderDamamgeNumber()
{
    // �ڵ� ��������!
    if ( m_vecDamageString.empty() )
        return;

    std::vector<KDamageString>::iterator vit = m_vecDamageString.begin();
    for( vit; vit != m_vecDamageString.end(); )
    {
        if( (*vit).iFrameCount == -1 )
            break;

        std::vector<string>::iterator vvit = (*vit).vecDamageString.begin();
        if ( vvit == (*vit).vecDamageString.end() )
        {
            // �� �̻� �׸� ������ ���ڰ� ������ �� ������ ��Ʈ���� �����!
            vit = m_vecDamageString.erase( vit );
        }
        else
        {
            int i = (*vit).iLastCount;
            for ( ;vvit != (*vit).vecDamageString.end(); )
            {
                // 5 frame���� �� ���ھ� �׸���!
                if ( (*vit).iFrameCount == i * 1 )
                {
                    (*vit).fPosX += (*vit).fInterval;
                    g_ParticleManager->CreateSequence( (*vvit), (*vit).fPosX, (*vit).fPosY, 0.5f );
                    // ������ �׷ȴ� ������ count���� ����Ѵ�.
                    (*vit).iLastCount = (i + 1);
                    // �׸� ���ڴ� �����.
                    (*vit).vecDamageString.erase( vvit );
                    vvit = (*vit).vecDamageString.end();
                }
                else
                    ++vvit;
            }

            (*vit).iFrameCount++;
            ++vit;
        }
    }
}

void Headup::RenderPuzzleKey( int iKeyValue_, D3DXVECTOR2 vPos_ )
{
    vPos_.x += ( PUZZLE_KEY_EFFECT_GAP * static_cast<float>( m_iPuzzleKeyEffectCnt ) );
    m_vecPuzzleKeyEffectPos.push_back( vPos_ );
    std::map< int, std::string >::iterator mit = m_mapPuzzleKeySequence.find( iKeyValue_ );

    if( mit != m_mapPuzzleKeySequence.end() ) {
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( mit->second, vPos_.x - PUZZLE_KEY_EFFECT_X_OFFSET, vPos_.y - PUZZLE_KEY_EFFECT_Y_OFFSET, 0.5f ),
            GC_LAYER_CHAR_OVER_UI_1 );
        m_iPuzzleKeyEffectCnt++;
    }
}

void Headup::RenderPuzzleKeyPushEffect( int iOrder_ )
{
    if( iOrder_ >= static_cast<int>( m_vecPuzzleKeyEffectPos.size() ) ) {
        return;
    }

    for( int i = 0; i < static_cast<int>( m_vecPuzzleKeyPushSequence.size() ); i++ ) {
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( m_vecPuzzleKeyPushSequence[ i ], m_vecPuzzleKeyEffectPos[ iOrder_ ].x  - PUZZLE_KEY_EFFECT_X_OFFSET, 
            m_vecPuzzleKeyEffectPos[ iOrder_ ].y - PUZZLE_KEY_EFFECT_Y_OFFSET, 0.5f ), GC_LAYER_CHAR_OVER_UI_2 );
    }
}

void Headup::RenderPuzzleKeyFailEffect( int iOrder_ )
{
    if( iOrder_ >= static_cast<int>( m_vecPuzzleKeyEffectPos.size() ) ) {
        return;
    }

    for( int i = 0; i < static_cast<int>( m_vecPuzzleKeyFailSequence.size() ); i++ ) {
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( m_vecPuzzleKeyFailSequence[ i ], m_vecPuzzleKeyEffectPos[ iOrder_ ].x  - PUZZLE_KEY_EFFECT_X_OFFSET, 
            m_vecPuzzleKeyEffectPos[ iOrder_ ].y - PUZZLE_KEY_EFFECT_Y_OFFSET, 0.5f ), GC_LAYER_CHAR_OVER_UI_2 );
    }
}

void Headup::LoadPuzzleKeySequenceList()
{
    if( !m_mapPuzzleKeySequence.empty() ||
        !m_vecPuzzleKeyPushSequence.empty() ) {
        return;
    }

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "PuzzleKeyList.stg" ) == false )
        return;

    m_iPuzzleKeyEffectCnt = 0;
    m_mapPuzzleKeySequence.clear();
    m_vecPuzzleKeyPushSequence.clear();
    m_vecPuzzleKeyFailSequence.clear();

    LUA_BEGIN_TABLE_RAII( "PuzzleKeySequenceList" )
    {
        LUA_BEGIN_TABLE_RAII( "PushSequence" )
        {
            std::string strSeq;
            for( int i = 1; ; i++ ) {
                LUA_GET_VALUE_DEF( i, strSeq, "" );

                if( strSeq == "" ) {
                    break;
                } else {
                    m_vecPuzzleKeyPushSequence.push_back( strSeq );
                }                
            }            
        }

        LUA_BEGIN_TABLE_RAII( "FailSequence" )
        {
            std::string strSeq;
            for( int i = 1; ; i++ ) {
                LUA_GET_VALUE_DEF( i, strSeq, "" );

                if( strSeq == "" ) {
                    break;
                } else {
                    m_vecPuzzleKeyFailSequence.push_back( strSeq );
                }                
            }            
        }

        LUA_BEGIN_TABLE_RAII( "KeySequence" )
        {
            for(int i = 1; ; i++) 
            {                
                LUA_BEGIN_TABLE_RAII( i ) 
                {
                    int iKeyValue;
                    std::string strSeq;
                    LUA_GET_VALUE_DEF(1, iKeyValue, 0 );
                    LUA_GET_VALUE_DEF(2, strSeq, "" );
                    m_mapPuzzleKeySequence.insert( std::make_pair( iKeyValue, strSeq ) );
                }
                else  
                {
                    break;
                }
            }
        }
    }
}

void Headup::InitPuzzleKeyInfo()
{
    m_vecPuzzleKeyEffectPos.clear();
    m_iPuzzleKeyEffectCnt = 0;
}