#include "stdafx.h"
#include "KGCEmoticonSlotRow.h"
#include "KGCEmoticonWindow.h"

//
//
//
#include "../MyD3D.h"
#include "../Procedure.h"

#include "dxutil.h"
//
//
#include "GCMyInfoScene.h"
#include "KGCEmoticon.h"


IMPLEMENT_CLASSNAME( KGCEmoticonWindow );
IMPLEMENT_WND_FACTORY( KGCEmoticonWindow );
IMPLEMENT_WND_FACTORY_NAME( KGCEmoticonWindow, "gc_emoticon_window" );


KGCEmoticonWindow::KGCEmoticonWindow( void )
: m_pkCloseBtn( NULL )
, m_pkShakeBtn( NULL )
, m_pkMaterial1( NULL )
, m_pkMaterial2( NULL )
, m_pkMaterial3( NULL )
, m_pkNewEmoticon( NULL )
, m_pkCubeName( NULL )
, m_pkCubeManual( NULL )
, m_pkResultMsg( NULL )
, m_pkResultMsgBack( NULL )
, m_pkMaterial_Num_1( NULL )
, m_pkMaterial_Num_2( NULL )
, m_pkMaterial_Num_3( NULL )
, m_iClickedSlotNum( 0 )
, m_bShakeItem( false )
, m_bCompleteShake( false )
, m_fEffectTime( 0.f )
, m_pkTitleBtn( NULL )
, m_pkSelEmoticonBtn( NULL )
, m_pkPetBtn( NULL )
, m_pkMonsterCardBtn( NULL )
{
    char str[256] = "";
    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        m_pkEmoticonRow[i] = NULL;
        sprintf( str, "emoticon_slot_row%d", i+1 );
        LINK_CONTROL( str,               m_pkEmoticonRow[i] );
    }

    LINK_CONTROL( "shake_button",        m_pkShakeBtn );
    LINK_CONTROL( "close_button",        m_pkCloseBtn );
    LINK_CONTROL( "material1",           m_pkMaterial1 );
    LINK_CONTROL( "material2",           m_pkMaterial2 );
    LINK_CONTROL( "material3",           m_pkMaterial3 );
    LINK_CONTROL( "new_emoticon",        m_pkNewEmoticon );
    LINK_CONTROL( "cube_name",           m_pkCubeName );
    LINK_CONTROL( "manual",              m_pkCubeManual );
    LINK_CONTROL( "result_msg",          m_pkResultMsg );
    LINK_CONTROL( "result_msg_back",     m_pkResultMsgBack );
    LINK_CONTROL( "material_num1",       m_pkMaterial_Num_1 );
    LINK_CONTROL( "material_num2",       m_pkMaterial_Num_2 );
    LINK_CONTROL( "material_num3",       m_pkMaterial_Num_3 );

    LINK_CONTROL( "title_button",            m_pkTitleBtn );
    LINK_CONTROL( "emoticon_button_big",     m_pkSelEmoticonBtn );
    LINK_CONTROL( "pet_button",              m_pkPetBtn );
    LINK_CONTROL( "monster_card_button",     m_pkMonsterCardBtn );
}

KGCEmoticonWindow::~KGCEmoticonWindow( void )
{
}

void KGCEmoticonWindow::OnCreate( void )
{
    m_pkShakeBtn->InitState( true, true, this );
    m_pkShakeBtn->Lock( true );
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    m_pkMaterial1->InitState( true, true, this );
    m_pkMaterial2->InitState( true, true, this );
    m_pkMaterial3->InitState( true, true, this );
    m_pkNewEmoticon->InitState( true, true, this );

    m_pkCubeName->InitState( true );
    m_pkCubeName->SetText( g_pkStrLoader->GetString(STR_ID_EMOTICON_CUBE_NAME) );

    m_pkCubeManual->InitState( true );
    m_pkCubeManual->SetMultiLine( true );
    m_pkCubeManual->SetText( g_pkStrLoader->GetString(STR_ID_EMOTICON_CUBE_MANUAL) );

    m_pkResultMsg->InitState( false );
    m_pkResultMsg->SetMultiLine( true );
    m_pkResultMsgBack->InitState( false );

    m_pkMaterial_Num_1->InitState( false );
    m_pkMaterial_Num_2->InitState( false );
    m_pkMaterial_Num_3->InitState( false );

    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        m_pkEmoticonRow[i]->InitState( true, true, this );
        m_pkEmoticonRow[i]->SetRowIndex( i );
    }

    for ( int i = 0; i < 4; i++ )
    {
        m_pkEmoticonData[i] = NULL;
        m_iRowIndex[i]      = -1;
        m_iColumnIndex[i]   = -1;
    }

	m_pkTitleBtn->InitState( true, true, this );
	m_pkSelEmoticonBtn->InitState( true, true, this );
	m_pkPetBtn->InitState( true, true, this );

	//resource 에서 On/Off 하도록. 즉 리소스에 존재 하면 작동하도록
	if (m_pkMonsterCardBtn)
		m_pkMonsterCardBtn->InitState( true, true, this );

}

void KGCEmoticonWindow::ActionPerformed( const KActionEvent& event )
{

    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        if ( event.m_pWnd == m_pkEmoticonRow[i] && m_pkEmoticonRow[i]->GetIsRowClick()
            && g_pkInput->BtnUp( KInput::MBLEFT ) )
        {
            // 어딘가 클릭됐다면!
            if ( NULL != m_pkEmoticonData[0] && NULL != m_pkEmoticonData[1] && NULL != m_pkEmoticonData[2] )
            {
                // 꽉 차쏩! 마지막 아이 바꿔주쟈 ㅋ 
                for ( int j = 0; j < 3; ++j )
                {
                    if ( m_iRowIndex[j] == i && m_iColumnIndex[j] == m_pkEmoticonRow[i]->GetOverColumnIndex() || -1 == m_pkEmoticonRow[i]->GetOverColumnIndex() )
                    {
                        // 이미 클릭된 아이 있네염 -_-;
                        return;
                    }
                }

                m_pkEmoticonRow[m_iRowIndex[2]]->TurnOffSlotEdge( m_iColumnIndex[2] );
                if ( i != m_iRowIndex[2] )
                    m_pkEmoticonRow[m_iRowIndex[2]]->InitOverColumnIndex();
                m_pkEmoticonData[2]  = m_pkEmoticonRow[i]->GetOverEmoticonData();
                m_iRowIndex[2]       = i;
                m_iColumnIndex[2]    = m_pkEmoticonRow[i]->GetOverColumnIndex();
                m_pkEmoticonRow[2]->SetIsRowClick( false );
            }
            else
            {
                for ( int j = 0; j < 3; ++j )
                {
                    if ( m_iRowIndex[j] == i && m_iColumnIndex[j] == m_pkEmoticonRow[i]->GetColumnIndex() )
                    {
                        // 이미 클릭된 아이 있네염 -_-;
                        m_pkEmoticonRow[i]->SetIsRowClick( false );
                        return;
                    }
                }

                for ( int j = 0; j < 3; ++j )
                {
                    if ( NULL == m_pkEmoticonData[j] )
                    {
                        m_pkEmoticonData[j]  = m_pkEmoticonRow[i]->GetEmoticonData();
                        m_iRowIndex[j]       = i;
                        m_iColumnIndex[j]    = m_pkEmoticonRow[i]->GetColumnIndex();
                        m_pkEmoticonRow[i]->SetIsRowClick( false );
                        DecreseClickableSlotNum();
                        break;
                    }
                }
            }
        }
    }

    if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        g_KDSound.Play( "31" ); 

        // 뭔가 올려져 있다면!
        if ( event.m_pWnd == m_pkMaterial1 && NULL != m_pkEmoticonData[0] )
        {
            m_pkEmoticonData[0] = NULL;
            m_pkEmoticonRow[m_iRowIndex[0]]->TurnOffSlotEdge( m_iColumnIndex[0] );
            m_pkEmoticonRow[m_iRowIndex[0]]->SetIsRowClick( false );
            m_pkEmoticonRow[m_iRowIndex[0]]->InitOverColumnIndex();
            m_iRowIndex[0] = -1;
            m_iColumnIndex[0] = -1;
            m_pkMaterial_Num_1->ToggleRender( false );
            IncreseClickableSlotNum();
        }
        if ( event.m_pWnd == m_pkMaterial2 && NULL != m_pkEmoticonData[1] )
        {
            m_pkEmoticonData[1] = NULL;
            m_pkEmoticonRow[m_iRowIndex[1]]->TurnOffSlotEdge( m_iColumnIndex[1] );
            m_pkEmoticonRow[m_iRowIndex[1]]->SetIsRowClick( false );
            m_pkEmoticonRow[m_iRowIndex[1]]->InitOverColumnIndex();
            m_iRowIndex[1] = -1;
            m_iColumnIndex[1] = -1;
            m_pkMaterial_Num_2->ToggleRender( false );
            IncreseClickableSlotNum();
        }
        if ( event.m_pWnd == m_pkMaterial3 && NULL != m_pkEmoticonData[2] )
        {
            m_pkEmoticonData[2] = NULL;
            m_pkEmoticonRow[m_iRowIndex[2]]->TurnOffSlotEdge( m_iColumnIndex[2] );
            m_pkEmoticonRow[m_iRowIndex[2]]->SetIsRowClick( false );
            m_pkEmoticonRow[m_iRowIndex[2]]->InitOverColumnIndex();
            m_iRowIndex[2] = -1;
            m_iColumnIndex[2] = -1;
            m_pkMaterial_Num_3->ToggleRender( false );
            IncreseClickableSlotNum();
        }
        if ( event.m_pWnd == m_pkCloseBtn )
        {
            if ( m_bShakeItem != m_bCompleteShake )
                return;

            InitAll();
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
        else if ( event.m_pWnd == m_pkShakeBtn )
        {
            LockWnd( true );
            m_bShakeItem = true;

            if ( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().GetCurrentGP() < 3000 )
            {
                // 쥐피 3000 보다 작으면 서버에게 보내지 말쟈 -_-;;
                Result_New_Emoticon = 2;
            }
            else
            {
                KP2P::GetInstance()->Send_EmoticonMixReq( static_cast<DWORD>(m_pkEmoticonData[0]->m_iItemID * 10), static_cast<DWORD>(m_pkEmoticonData[1]->m_iItemID * 10)
                    , static_cast<DWORD>(m_pkEmoticonData[2]->m_iItemID * 10) );

                g_MyD3D->WaitForServerAck( Result_New_Emoticon, INT_MAX, 10000, TIME_OUT_VALUE );
            }
        }
        else if( event.m_pWnd == m_pkTitleBtn )
        {
            if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
                SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_TITLE_COLLECTION ) );
        }
        else if( event.m_pWnd == m_pkPetBtn )
        {
            if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
                SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_PET_COLLECTION ) );
        }
        else if( event.m_pWnd == m_pkMonsterCardBtn )
        {
			if (m_pkMonsterCardBtn)
			{
				if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
				{
					InitAll();
					SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_BOOK, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , true, false);
				}
			} 
        }
    }
}

void KGCEmoticonWindow::PostChildDraw( void )
{	
    for ( int i = 0; i < 4; i++)
    {
        if ( NULL != m_pkEmoticonData[i] )
        {
            D3DXVECTOR4 vUV;
            vUV.x = m_pkEmoticonData[i]->m_safTextureCoordList[0][0];
            vUV.y = m_pkEmoticonData[i]->m_safTextureCoordList[0][1];
            vUV.z = m_pkEmoticonData[i]->m_safTextureCoordList[0][2];
            vUV.w = m_pkEmoticonData[i]->m_safTextureCoordList[0][3];

            if ( !m_pkEmoticonData[i]->m_pTexture || !m_pkEmoticonData[i]->m_pTexture->GetDeviceTexture() )
                return;

            KSafeArray<KD3DUIVertex,4> pEmoticonVertex;
            switch ( i )
            {
                case 0:
                    pEmoticonVertex = m_pkMaterial1->GetReLocatedWindowRect();
                    m_pkEmoticonRow[m_iRowIndex[0]]->TurnOnSlotEdge( m_iColumnIndex[0] );
                    m_pkMaterial_Num_1->ToggleRender( true );
                    break;
                case 1:
                    pEmoticonVertex = m_pkMaterial2->GetReLocatedWindowRect();
                    m_pkEmoticonRow[m_iRowIndex[1]]->TurnOnSlotEdge( m_iColumnIndex[1] );
                    m_pkMaterial_Num_2->ToggleRender( true );
                    break;
                case 2:
                    pEmoticonVertex = m_pkMaterial3->GetReLocatedWindowRect();
                    m_pkEmoticonRow[m_iRowIndex[2]]->TurnOnSlotEdge( m_iColumnIndex[2] );
                    m_pkMaterial_Num_3->ToggleRender( true );
                    break;
                default :
                    pEmoticonVertex = m_pkNewEmoticon->GetReLocatedWindowRect();
                    break;
            }

			g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData[i]->m_pTexture,
				pEmoticonVertex[0].m_vPos.x + 3.f *m_fWindowScaleX, pEmoticonVertex[0].m_vPos.y + 3.f*m_fWindowScaleY, pEmoticonVertex[3].m_vPos.x - 3.f*m_fWindowScaleX, pEmoticonVertex[3].m_vPos.y - 3.f * m_fWindowScaleY,
				vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f,0xffffffff);

            /*g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
            {
                m_pkEmoticonData[i]->m_pTexture->SetDeviceTexture();
                g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x + 3.f, pEmoticonVertex[0].m_vPos.y + 3.f, pEmoticonVertex[3].m_vPos.x - 3.f, pEmoticonVertex[3].m_vPos.y - 3.f,
                    vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f, 255 );
            }
            KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
            KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/
        }
    }

    OnShake();
}

void KGCEmoticonWindow::OnDestroyComplete()
{
    InitAll();
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCEmoticonWindow::FrameMoveInEnabledState( void )
{
    if ( m_iClickedSlotNum == 3 )
    {
        if ( m_bShakeItem != m_bCompleteShake )
        {
            m_pkShakeBtn->Lock( true );
        }
        else
        {
            for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
                m_pkEmoticonRow[i]->SetClickable( false );
            m_pkShakeBtn->Lock( false );
        }
    }
    else if ( m_iClickedSlotNum < 3 && m_iClickedSlotNum >= 0 )
    {
        for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
            m_pkEmoticonRow[i]->SetClickable( true );
        m_pkShakeBtn->Lock( true );
    }
    else
        ASSERT( false && "클릭 가능한 숫자가 이상해!" );
}

void KGCEmoticonWindow::IncreseClickableSlotNum()
{
    if ( m_iClickedSlotNum > 0 && m_iClickedSlotNum <= 3 )
        m_iClickedSlotNum--;
    else
        ASSERT( false && "클릭 가능한 숫자가 이상해!" );
}

void KGCEmoticonWindow::DecreseClickableSlotNum()
{
    if ( m_iClickedSlotNum < 3 && m_iClickedSlotNum >= 0 )
        m_iClickedSlotNum++;
    else
    {
        ASSERT( false && "클릭 가능한 숫자가 이상해!" );
    }
}

void KGCEmoticonWindow::InitAll( void )
{
    for ( int i = 0; i < 4; ++i )
    {
        m_pkEmoticonData[i] = NULL;
        if( m_iRowIndex[i] != -1 )
        {
            m_pkEmoticonRow[m_iRowIndex[i]]->TurnOffSlotEdge( m_iColumnIndex[i] );
            m_pkEmoticonRow[m_iRowIndex[i]]->InitOverColumnIndex();
        }
        m_iRowIndex[i] = -1;
        m_iColumnIndex[i] = -1;
    }
    m_iClickedSlotNum = 0;
    m_pkMaterial_Num_1->ToggleRender( false );
    m_pkMaterial_Num_2->ToggleRender( false );
    m_pkMaterial_Num_3->ToggleRender( false );
}

void KGCEmoticonWindow::LockWnd( bool bLock )
{
    m_pkCloseBtn->Lock( bLock );
    m_pkShakeBtn->Lock( bLock );
    m_pkMaterial1->Lock( bLock );
    m_pkMaterial2->Lock( bLock );
    m_pkMaterial3->Lock( bLock );
    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
        m_pkEmoticonRow[i]->LockAll( bLock );
}

void KGCEmoticonWindow::OnShake( void )
{
    static int iTmp = 7179;
    if ( m_bShakeItem )
    {
        int iLoopNum = 10;
        if ( (iTmp + (iLoopNum * 2) ) <= 7228 )
        {
            static float fAdjustYpos = 2.0f;
            const KSafeArray<KD3DUIVertex,4>& pEmoticonVertex = m_pkNewEmoticon->GetReLocatedWindowRect();

            for ( int i = 0; i < iLoopNum ; ++i )
            {
                if ( pEmoticonVertex[3].m_vPos.y - fAdjustYpos > pEmoticonVertex[0].m_vPos.y )
                    fAdjustYpos += 1.5f;
                else
                {
                    // 첫번째 그림이 다 넘어갔다!! 다음그림으로 스위치!!
                    fAdjustYpos = 2.0f;
                    iTmp++;
                }

                // 슬롯 머신처럼 돌아가게 하기위해서!! 
                // 첫번째 그림!
                m_pkEmoticonData[3] = SiKGCEmoticonManager()->GetEmoticonData( iTmp + i );
                if ( NULL == m_pkEmoticonData[3])
                    return;

                // 파쿠쟈
                // sbta 들어오면 이짓 필요 없다. uv 좌표 0 0 1 1로 띄우쟈!
                D3DXVECTOR4 vUV;
                vUV.x = m_pkEmoticonData[3]->m_safTextureCoordList[0][0];
                vUV.y = m_pkEmoticonData[3]->m_safTextureCoordList[0][1];
                vUV.z = m_pkEmoticonData[3]->m_safTextureCoordList[0][2];
                vUV.w = m_pkEmoticonData[3]->m_safTextureCoordList[0][3];

				g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData[3]->m_pTexture,
					pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y - fAdjustYpos,
					vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f);
                /*m_pkEmoticonData[3]->m_pTexture->SetDeviceTexture();
                g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y - fAdjustYpos,
                    vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f, 255 );*/


                // 두번째 그림!
                float fTmpPosY = pEmoticonVertex[3].m_vPos.y - fAdjustYpos - 1.f;
                m_pkEmoticonData[3] = SiKGCEmoticonManager()->GetEmoticonData( iTmp + 1 + i );
                if ( NULL == m_pkEmoticonData[3])
                    return;

                vUV.x = m_pkEmoticonData[3]->m_safTextureCoordList[0][0];
                vUV.y = m_pkEmoticonData[3]->m_safTextureCoordList[0][1];
                vUV.z = m_pkEmoticonData[3]->m_safTextureCoordList[0][2];
                vUV.w = m_pkEmoticonData[3]->m_safTextureCoordList[0][3];

				g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData[3]->m_pTexture,
					pEmoticonVertex[0].m_vPos.x, fTmpPosY, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
					vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f);
                /*m_pkEmoticonData[3]->m_pTexture->SetDeviceTexture();
                g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x, fTmpPosY, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
                    vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f, 255 );*/
            }
        }
        else if ( INT_MAX == Result_New_Emoticon )
        {
            // 아직 패킷이 안왔네.. 끊임없이 돌아라~
            iTmp = 7228 - ( (iTmp + (iLoopNum * 2)) - 7228 );
        }
        else if ( 0 == Result_New_Emoticon )
        {
            // 성공이다! 
            m_bShakeItem = false;
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("item_mixture 01", 0.24f, -0.4f, 0.5F), GC_LAYER_UI);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("item_mixture 02", 0.24f, -0.4f, 0.5F), GC_LAYER_UI);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("item_mixture 03", 0.24f, -0.4f, 0.5F), GC_LAYER_UI);
            m_fEffectTime = DXUtil_Timer( TIMER_GETAPPTIME );
        }
        else if ( 1 == Result_New_Emoticon || 2 == Result_New_Emoticon || 3 == Result_New_Emoticon || 4 == Result_New_Emoticon || 5 == Result_New_Emoticon )
        {
            // 실패다!
            m_bShakeItem = false;
            m_fEffectTime = DXUtil_Timer( TIMER_GETAPPTIME );
        }
    }
    
    else if ( 0 == Result_New_Emoticon  )
    {
        // 2010.01.19. kkurrung. 아래 아이템 아이디는 Procedure 에서 이미 /10 이 적용 된 상태 이다..
        // 야호!! 가는거야!
        m_pkEmoticonData[3] = SiKGCEmoticonManager()->GetEmoticonData( g_kDurItemInfo.m_ItemID );
        // 오픈 되지 않은 놈을 받을수도 ... 그럼 오픈 시켜주고 띄우쟈!!
        SiKGCEmoticonManager()->SetEmoticonOpen( g_kDurItemInfo.m_ItemID );

        if ( NULL == m_pkEmoticonData[3])
            return;

        const KSafeArray<KD3DUIVertex,4>& pEmoticonVertex = m_pkNewEmoticon->GetWindowRect();
        // 파쿠쟈
        // sbta 들어오면 이짓 필요 없다. uv 좌표 0 0 1 1로 띄우쟈!
        D3DXVECTOR4 vUV;
        vUV.x = m_pkEmoticonData[3]->m_safTextureCoordList[0][0];
        vUV.y = m_pkEmoticonData[3]->m_safTextureCoordList[0][1];
        vUV.z = m_pkEmoticonData[3]->m_safTextureCoordList[0][2];
        vUV.w = m_pkEmoticonData[3]->m_safTextureCoordList[0][3];

		/*
        m_pkEmoticonData[3]->m_pTexture->SetDeviceTexture();
        g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
            vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f, 255 );*/

		g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData[3]->m_pTexture,
			pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
			vUV.x, vUV.y, vUV.z, vUV.w, false, 0.0f);

        Result_New_Emoticon = INT_MAX;

        for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
            m_pkEmoticonRow[i]->UpdateSlotInfo();

        m_pkResultMsgBack->ToggleRender( true );
        m_pkResultMsg->SetText( g_pkStrLoader->GetString( STR_ID_EMOTICON_COMPLETE ) );
        m_pkResultMsg->ToggleRender( true );

        m_bCompleteShake = true;
    }
    else if ( 1 == Result_New_Emoticon || 2 == Result_New_Emoticon || 3 == Result_New_Emoticon || 4 == Result_New_Emoticon || 5 == Result_New_Emoticon )
    {
        // 실패다!
        // 0 성공
        // 1 조합할 이모티콘 개수가 3개가 아닙니다
        // 2 GP가 부족함
        // 3 해당 이모티콘 아이템이 없음
        // 4 이모티콘 조합 실패
        // 5 이모티콘 조합 아이템 DB 작업 실패
        int iStr = STR_ID_EMOTICON_ERROR0 + Result_New_Emoticon - 1;
        m_pkResultMsgBack->ToggleRender( true );
        m_pkResultMsg->SetText( g_pkStrLoader->GetString( iStr ) );
        m_pkResultMsg->ToggleRender( true );
        Result_New_Emoticon = INT_MAX;
        m_bCompleteShake = true;
        LockWnd( false );
        InitAll();
        iTmp = 7179;
    }
    else if ( INT_MAX == Result_New_Emoticon && m_bCompleteShake && m_fEffectTime + 2.0f <= DXUtil_Timer( TIMER_GETAPPTIME ) )
    {
        m_pkResultMsgBack->ToggleRender( false );
        m_pkResultMsg->ToggleRender( false );
        m_bCompleteShake = false;
        LockWnd( false );
        InitAll();
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory(), g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurItemSlot() );
        iTmp = 7179;
    }
}
