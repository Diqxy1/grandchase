#include "stdafx.h"
#include ".\SquareScene.h"
#include "GCCameraOrtho.h"
//
#include "./Square/GCSquare.h"
#include "./MapSystem/KGCLayerManager.h"
#include "Spark.h"
#include "KTDGFrustum.h"
//
#include "GCUI/KGCSquareOverlayUI.h"
#include "KDInput.h"


#include "../BuddySystem/KFriendShip.h"
#include "gcui/KGCShortCutBox.h"
#include "gcui/GCSquareSayBalloon.h"
#include "GCRenderManager.h"
#include "KSingleton.h"
#include "gcui/GCSceneMenuBtn.h"

IMPLEMENT_CLASSNAME( KGCSquareScene );
IMPLEMENT_WND_FACTORY( KGCSquareScene );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareScene, "gc_square_scene" );

KGCSquareScene::KGCSquareScene(void)
{

}

KGCSquareScene::~KGCSquareScene(void)
{
}

void KGCSquareScene::OnCreate( void )
{

}

void KGCSquareScene::OnCreateComplete( void )
{

}

void KGCSquareScene::OnDestroyComplete( void )
{
}

void KGCSquareScene::FrameMoveInEnabledState( void )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE ||  g_pkSquareOverlayUI == NULL )
    {
        return;
    }

	SiKFriendShip()->CheckAlramAddFriend();

	// Stop 상태이면 메시지 박스가 떠 있거나 예외적인 상황이므로
	// Picking을 잠시 꺼줍니다.
	if( !g_pkUIScene->IsStop() )
	{
		// picking right button
		if ( g_pkInput->BtnDown( KInput::MBRIGHT ) )
		{
			if ( g_pkSquareOverlayUI->IsMouseControlAvailable( SQUARE_FOCUS_PICKING ) )
			{
				g_pkSquareOverlayUI->Picking( KInput::MBRIGHT );
			}
		}
		
		// picking left button
		if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
		{
			if ( g_pkSquareOverlayUI->IsMouseControlAvailable( SQUARE_FOCUS_PICKING ) )
			{
				g_pkSquareOverlayUI->Picking( KInput::MBLEFT );
			}
		}
	}

    // [2/19/2008] breadceo. mouse wheel 체크	
    if( g_pkInput->ISWheelUp() && g_pkUIScene->IsMsgBoxModal() == false && g_pkUIScene->IsEmptyWaitMessageBox() )
    {
        // 항상 체크하는거 보다 휠메세지 들어왔을때만..
        if( g_pkSquareOverlayUI->IsMouseControlAvailable( SQUARE_FOCUS_WHEEL ) )
        {
            g_kCamera.m_fZoom += 0.035f;
            if( g_kCamera.m_fZoom > 2.0f )
            {
                g_kCamera.m_fZoom = 2.0f;
            }
        }
    }
    else if( g_pkInput->ISWheelDown() && g_pkUIScene->IsMsgBoxModal() == false && g_pkUIScene->IsEmptyWaitMessageBox() )
    {
        if( g_pkSquareOverlayUI->IsMouseControlAvailable( SQUARE_FOCUS_WHEEL ) )
        {
            g_kCamera.m_fZoom -= 0.035f;
            if( g_kCamera.m_fZoom < 1.0f )
            {
                g_kCamera.m_fZoom = 1.0f;
            }
        }
    }

    //광장에서 탭키를 눌렀을때 모든 UI들을 꺼준다.
    if( diks[DIK_TAB] && !Old_diks[DIK_TAB] )
    {
        g_pkSquareOverlayUI->ToggleRender( !g_pkSquareOverlayUI->IsRenderOn() );
        if ( !g_pkSquareOverlayUI->IsRenderOn() )
            g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF ) );
        else
            g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON ) );
    }

	//TAB눌러서 촬영 모드 일때 ESC나 엔터 눌러도 풀리도록 설정
	if (g_pkSquareOverlayUI->IsRenderOn()==false)
	{
		if ( diks[DIK_ESCAPE] && !Old_diks[DIK_ESCAPE] )
		{
			g_pkSquareOverlayUI->ToggleRender( !g_pkSquareOverlayUI->IsRenderOn() );
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}
		else if ( diks[DIK_RETURN] && !Old_diks[DIK_RETURN] )
		{
			g_pkSquareOverlayUI->ToggleRender( !g_pkSquareOverlayUI->IsRenderOn() );
			g_pkInput->IgnoreDown(DIK_RETURN,30);
		}
	}
	
	//TAB키 눌러서 촬영모드에서도 동작하도록 여기에서 일부기능들 돌린다.
	g_pkSquareOverlayUI->TimeCheck();
	g_pkSquareOverlayUI->CreateParticle();

    //채팅 모드중에
    //if ( true == g_pkSquareOverlayUI->HasEditFocus() )
    if (KGCSquareOverlayUI::ECF_CHATBOX==g_pkSquareOverlayUI->GetCursorFocus())
    {
        // 엔터친 경우 포커스 끕니다. , ESC 누른 경우도 추가 합니다.
        if ( (diks[DIK_RETURN] || diks[DIK_NUMPADENTER]) && !(Old_diks[DIK_RETURN] || Old_diks[DIK_NUMPADENTER] ) || (diks[DIK_ESCAPE] && !Old_diks[DIK_ESCAPE] )
            )
        {
            g_pkSquareOverlayUI->HandleChatEvent();
        }
    }
    else if (KGCSquareOverlayUI::ECF_MAINWND==g_pkSquareOverlayUI->GetCursorFocus())
    {
        //메인 모드일때 아닌데, 엔터친 경우
        if ( (diks[DIK_RETURN] || diks[DIK_NUMPADENTER]) && !(Old_diks[DIK_RETURN] || Old_diks[DIK_NUMPADENTER] ))
        {
            g_pkSquareOverlayUI->SetEditFocus();
        }
        //else if (diks[DIK_ESCAPE] && !Old_diks[DIK_ESCAPE] )
        //{
        //    if ( false == g_pkUIScene->IsMsgBoxModal() )
        //    {                
        //       g_pkSquareOverlayUI->ModalExitMsgBox();
        //    }
        //}

#if defined(EMOTICON_SYSTEM)
#if defined(NATION_THAILAND)
        if ( diks[DIK_MINUS] && !Old_diks[DIK_MINUS] )
#else
		if ( diks[DIK_GRAVE] && !Old_diks[DIK_GRAVE] )
#endif
        {
            g_pkSquareOverlayUI->m_pkShortCutBox->ToggleShowState();
        }

        for ( int i = 0 ; i < 10 ; i++ )
        {
            if ( diks[DIK_1 + i] && !Old_diks[DIK_1 + i] )
            {
                g_MyD3D->GetMyShortCutSlot()->UseEmot(i);
            }
        }
#endif 
    }



    // [1/28/2008] breadceo. 광장에서는 외곽선을 끈다.
//     if( true == SiKGCRenderManager()->GetUsingVS() )
//     {
//         if( g_MyD3D->m_KGCOption.GetQualityCount() > 5 )
//         {
//             SiKGCRenderManager()->SetRenderTech(1);
//         }
//     }
    SiGCSquare()->FrameMove();
    SiKGCFantasticMap()->FrameMove( GC_ELAPSED_TIME );
	SiKGCFantasticMap()->Water_FrameMove();
    g_MyD3D->MySparks->FrameMove();
    g_kCamera.FrameMoveInSquare();
    g_kCamera.UpdateCameraMatrixForSquare();
    //g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    //g_KDSound.FrameMove();
}



#include "Square/GCSquarePeople.h"

void KGCSquareScene::ResetUIforResolution()
{
	SiGCSquare()->ResetText();	
}

void KGCSquareScene::PostDraw( void )
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE )
    {
        SiKGCFantasticMap()->Render( true );
        SiGCSquare()->Render();
        g_MyD3D->m_pFrustum->Construct( 1000.0f );
		
        g_RenderManager->Render();
		
        SparkRender();

        SiKGCLayerManager()->AlphaRender();
        SiKGCEmoticonManager()->Render();
		SiGCSquare()->Render_Object( GCSquare::SL_AFTER_PEOPLE );
		SiKGCFantasticMap()->Water_Render();
        SiGCSquare()->Render_EtcParticle();			
        SiGCSquare()->Render_Text();			


        if( false == g_pkSquareOverlayUI->IsRenderOn() )
        {
            if( true == g_pkSquareOverlayUI->GetSayBalloon()->IsRenderOn() )
            {
                g_pkSquareOverlayUI->GetSayBalloon()->Render();
            }
        }

		// 광장 높이 올라가기 대회
		if( false == g_kGlobalValue.IsAdmin() )
		{
			if( g_pkSquareOverlayUI->m_bRallyEvent )
			{
				FRect rtBount; 
				rtBount.left = 3.55f;	
				rtBount.top = 5.17f;
				rtBount.right = 3.95f;
				rtBount.bottom = 5.6f;

				//WCHAR szReMainTime[100];
				D3DXVECTOR2 vMyPos = SiGCSquare()->GetSquarePeople( g_kGlobalValue.m_kUserInfo.dwUID )->GetPos();
				if( rtBount.PtInRect( vMyPos.x, vMyPos.y ) )
				{
					//wsprintf(szReMainTime, L"들어왔구먼");
					const int iAllSec = ((timeGetTime() - g_pkSquareOverlayUI->m_dwRallyTime) / 1000);
					const int iMin = (int)(iAllSec/60);
					const int iSec = (int)(iAllSec%60);

					WCHAR szTime[100];
					wsprintf(szTime, L"%02d:%02d", iMin, iSec);

					KP2P::GetInstance()->Send_ArriveGoal( szTime );
					
					g_pkSquareOverlayUI->m_bRallyEvent = false;
				}
				//else
				//{
				//	wsprintf(szReMainTime, L"나갔구먼");
				//}

				//g_MyD3D->MyHAN_Draw( g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f*0, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );
			}
		}
	}
    //return S_OK;
}

void KGCSquareScene::SparkRender()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      FALSE );

        SiGCSquare()->Render_PetAnim();
        g_MyD3D->MySparks->Render();
        SiGCSquare()->Render_Damage();
    }
    g_pGCDeviceManager2->PopState();

    SiGCSquare()->Render_Effect();
}

void KGCSquareScene::OnEnterScene()
{
    g_pkUIScene->m_pkSquareOverlayUI->ToggleRender( true );
    g_pkUIScene->EnableMessageBox( true );
    g_pkUIScene->m_pkSquareOverlayUI->StartSquare();
    g_pkSquareOverlayUI->Initialize();
}

void KGCSquareScene::ActionPerformed( const KActionEvent& event )
{

}