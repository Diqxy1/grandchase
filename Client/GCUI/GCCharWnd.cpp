#include "stdafx.h"
#include "GCCharWnd.h"
#include "GCQuestGauge.h"

//
//
#include "../MyD3D.h"

//




//
//
//
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCCharacterSelectBox.h"
#include "KGCHeroDungeonDlg.h"
#include "KGCHeroDungeonShop.h"
//

#include "GCRenderManager.h"
#include "../GCStateMachine.h"
#include "KGCRoomManager.h"
#include "../gcui/KGCVipGachaBox.h"
#include "KGCFaceLift.h"
#include "KGCMatchManager.h"
#include "KGCPartyManager.h"

IMPLEMENT_CLASSNAME( KGCCharWnd );
IMPLEMENT_WND_FACTORY( KGCCharWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCharWnd, "gc_char_wnd" );

#define DELTAZOOM 0.01f
#define MAXZOOM 0.3f
#define MINZOOM 0.12f

KGCCharWnd::KGCCharWnd( void )
{
    m_iExtraCharNum = 0;
    m_fAngleMove = 0.0f;

    m_pkInit = NULL;
    m_pkInfo = NULL;

    m_pkQuestGauge = NULL;

    m_pMeshKOGRoom = NULL;
    m_bShowInfo = false;
    m_pkInfoDesc = NULL;
    m_pShopMotion = NULL;
    m_iMotionCount = 0;
    m_cFrame = 0;
    isRoomFrozenRotation = false;

    LINK_CONTROL( "character_init_button", m_pkInit );
    LINK_CONTROL( "character_info_button", m_pkInfo );

    for(int i=0;i<GC_CHAR_NUM;++i)
    {

        m_pkName[i] = NULL;
        char strCharName[MAX_PATH];
        sprintf(strCharName,"character_name%d",i);
        LINK_CONTROL( strCharName, m_pkName[i] );
    }
    LINK_CONTROL( "quest_gauge", m_pkQuestGauge );
    LINK_CONTROL( "char_info_static", m_pkInfoDesc );
    LINK_CONTROL( "exp_back", m_pkExpBack );
    LINK_CONTROL( "exp", m_pkExp );
    LINK_CONTROL( "exp_pecent_static", m_pkExpPercentStatic );

    m_fZoom = 0.25f;
    m_fCharX = 0.0f;
    m_bAngleChanged = false;

    m_pkPet = NULL;
    m_pTexTitleItem = NULL;

	m_pkBack_Color = NULL;
	LINK_CONTROL( "back_color", m_pkBack_Color );

	m_bUseRoomObject = true;

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
}

KGCCharWnd::~KGCCharWnd( void )
{
    SAFE_DELETE( m_pMeshKOGRoom );
    m_pShopMotion = NULL;
    if( m_pkPet != NULL )
        m_pkPet->EndPet();
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
    SAFE_DELETE( m_pkPet );

    SAFE_RELEASE( m_pTexTitleItem );
}

void KGCCharWnd::OnDestroy( void )
{
    SAFE_RELEASE( m_pTexTitleItem );
}

void KGCCharWnd::OnCreate( void )
{
	m_pkBack_Color->InitState(false);

    if ( m_pkInit ) 
    {
        m_pkInit->SetSelfInputCheck( true );
        m_pkInit->AddActionListener( this );
    }

    if ( m_pkInfo ) 
    {
        m_pkInfo->SetSelfInputCheck( true );
        m_pkInfo->AddActionListener( this );
    }    
	
    if ( m_pkQuestGauge ) 
    {
        m_pkQuestGauge->SetSelfInputCheck( false );
        m_pkQuestGauge->SetMaximumVal( 300 );
    } 

    if( m_pkPet == NULL )
        m_pkPet = new KPet();
    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( GetWidth() ) / float( GetHeight() ),
                               1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;
    ReloadMotion();
}

void KGCCharWnd::OnActivate( void )
{
    //Initialize¿¡ ±¸ÇöÇÕ½Ã´Ù..
}

void KGCCharWnd::SetPlayerExp()
{
    if ( m_pkExpBack && m_pkExp && m_pkExpPercentStatic )  
    {
        float fPercent = GetCurrentExpPercent( g_MyD3D->m_TempPlayer.GetCurrentChar().biExp );
        int iPercentHeight = (int)(m_pkExpBack->GetHeight() * fPercent / 100);
        D3DXVECTOR2 winpos = m_pkExpBack->GetFixedWindowLocalPos();

        winpos.y += (float)m_pkExpBack->GetHeight() - iPercentHeight;
        m_pkExp->SetFixedWindowLocalPos(winpos);
        m_pkExp->SetHeight(iPercentHeight);

        WCHAR strTemp[10];
        swprintf(strTemp, 10, L"%3.1f%%", fPercent);
        m_pkExpPercentStatic->SetText(strTemp);
    }
}

void KGCCharWnd::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
    bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, true);

	if( !m_bAngleChanged )
	{
		m_fAngleMove += 0.005f;
		if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
	}

    if (g_pkInput->ISWheelUp() && bCheckBound)
        ChangeCharWndZoom(true);
    else if (g_pkInput->ISWheelDown() && bCheckBound)
        ChangeCharWndZoom(false);

    static bool bInternal = false;

    if (g_pkInput->BtnDown() == true && bCheckBound)
    {
        m_bAngleChanged = true;
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
        bInternal = true;
    }
    else if (g_pkInput->BtnDown() == true && !bCheckBound)
        bInternal = false;

    if (g_pkInput->BtnDown(KInput::MBRIGHT) == TRUE)
    {
        m_bAngleChanged = false;
    }

    if (g_pkInput->BtnPressed() && m_bAngleChanged == true && bInternal == true)
        ProcessCharWndAngle();

    if (g_MyD3D->m_TempPlayer.m_pObject)
    {
        g_MyD3D->m_TempPlayer.SetRender(true);
        g_MyD3D->m_TempPlayer.EgoItemFrameMove(UCHAR(m_cFrame));
        g_MyD3D->m_TempPlayer.m_pObject->ChildFrameMove();
    }

}

void KGCCharWnd::PostDraw( void )
{
    if( g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex == -1 )
        return;

    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

    D3DXMatrixScaling( &matScale ,m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
    D3DXMatrixRotationY( &matRot, m_fAngleMove );
    if (isRoomFrozenRotation) {
        D3DXMatrixTranslation(&matTrans, 0.0f, 0.081f - (MAXZOOM - m_fZoom) * 1.143f, 0.32f);
    }
    else {
        D3DXMatrixTranslation(&matTrans, 0.0f, 0.081f - (MAXZOOM - m_fZoom) * 0.25f, 0.32f);
    }

    matWorld = matScale * (!isRoomFrozenRotation ? matRot * matTrans : matTrans);
    if(m_pMeshKOGRoom != NULL){
        m_pMeshKOGRoom->SetWorldMat( &matWorld );
        g_RenderManager->Render( false , m_pMeshKOGRoom , EGCShaderTechnique::NOCARTOON_NOEDGE );
    }

    if( m_pShopMotion != NULL )
    {
        if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum || !g_MyD3D->m_TempPlayer.m_pObject)
        {
            int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
            g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iSelectChar );
            SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
            g_MyD3D->m_TempPlayer.RemovePlayerObject();
            g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
            UpdatePet();
        }        

        if ( m_cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
        {
            m_iMotionCount++;
            if( m_iMotionCount == 5 )
            {
                if( m_pActionMotion )
                {
                    // ¸ð¼ÇÀÌ ¹Ù²î¾úÀ½
                    m_pShopMotion = m_pActionMotion;
                    m_iMotionCount = 0;
                    m_fCharX = 0.0f;// X¸¦ ÃÊ±âÈ­

                    SiKGCFaceLift()->SetCurMotionName( m_pActionMotion->GetDeviceID() );                    
                }
            }
            else
            {
                if( m_pWaitMotion )
                {
                    // ¸ð¼ÇÀÌ ¹Ù²î¾úÀ½
                    m_pShopMotion = m_pWaitMotion;
                    m_fCharX = 0.0f;// X¸¦ ÃÊ±âÈ­

                    SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion->GetDeviceID() );
                }
            }

            m_cFrame = 0;
        }
        
        float fPosX = m_fCharX * CHAR_SCALE_RATIO;
        float fPosY = m_pShopMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.06f;
        float fPosZ = 0.303f;
        float fCharScale = 1.0f;    // Ä³¸¯ÅÍ ½ºÄÉÀÏ

        if ( g_MyD3D->m_pStateMachine->IsStateCharSelect() )
        {
            fCharScale = 1.25f;
        }

        m_fCharX += m_pShopMotion->GetFrameData(m_cFrame).Plus_x;
        
        fPosX = fPosX * cos( m_fAngleMove );

        D3DXMatrixScaling( &matScale , m_fZoom * fCharScale, m_fZoom * fCharScale, m_fZoom * fCharScale );
        D3DXMatrixRotationY( &matRot, m_fAngleMove );
        D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

        matWorld = matScale * matRot * matTrans;
        g_MyD3D->m_TempPlayer.m_pObject->SetWorldMat( &matWorld );
        g_MyD3D->m_TempPlayer.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );

        g_MyD3D->m_TempPlayer.SetRenderFaceLift( g_MyD3D->m_TempPlayer.GetDefaultFaceLiftType() );
        SiKGCFaceLift()->SetFaceLiftByMotionName( &g_MyD3D->m_TempPlayer, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );

        g_RenderManager->Render( false , g_MyD3D->m_TempPlayer.m_pObject );
        m_cFrame++;

        // ¿©±â ±×·ÁÁÖÀÚ Æê¶Ç..=¤µ=; Ä³¸¯ÅÍ ¿·¿¡ À§Ä¡½ÃÄÑ Áà¾ß ÇÑ´Ù
        // »çÀÌÁîµµ Á¶±Ý ÀÛ°Ô µé¾î°¡¾ß ÇÏ°ÚÁö..
        if( m_pkPet->m_iPetID != -1 && g_MyD3D->m_KGCOption.GetPetViewHide() == false )
        {
            float fScale = 2.5f * m_pkPet->m_fScale;
            D3DXMATRIX mat1, mat2, mat3, mat4, matWorld;

            if ( g_MyD3D->m_pStateMachine->IsStateCharSelect() )
            {
                fScale *= fCharScale;
            }
            
            // »óÇÏ
            ::D3DXMatrixTranslation( &mat4, -0.3f / fScale, -0.57f / fScale, -0.2f / fScale);
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );

			if ( m_pkPet->m_iPetID == EGCPETID::PET_KELBY )
				m_pkPet->usMotion = EGCPETMOTION::SHOP_ACTION;
			else
				m_pkPet->usMotion = EGCPETMOTION::WAIT;

            float fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.303f;

            fPosX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;
            fPosY += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Pos_y * m_fZoom * fScale + m_pkPet->m_fDistY * m_fZoom * 2.5f;

            fPosX = fPosX * cos( m_fAngleMove );

            ::D3DXMatrixTranslation( &mat3, fPosX, fPosY, fPosZ );
            matWorld = mat4 * mat1 * mat2 * mat3;
            m_pkPet->m_pObject->SetWorldMat( &matWorld );

            m_pkPet->m_pObject->SetFrame( &m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ) );  

            m_pkPet->cFrame++;
            m_pkPet->cFrame %= m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame();
            m_pkPet->m_pObject->SetShellMatrixScale( fScale * m_fZoom );

            g_RenderManager->Render( false , m_pkPet->m_pObject );
        }
    }

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );

	if( m_pTexTitleItem )
	{
		D3DXVECTOR2 vPos = GetRelocatedWindowPos() + m_pkBack_Color->GetFixedWindowLocalPos();
		g_pItemMgr->Draw_ShopImage(vPos,static_cast<float>(m_pkBack_Color->GetWidth()/3) * m_fWindowScaleX, static_cast<float>(m_pkBack_Color->GetWidth()/3) * m_fWindowScaleY, m_pTexTitleItem, false );
        
	}
}

void KGCCharWnd::SetChar(int iCharType)
{
    for(int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (m_pkName[i])
            m_pkName[i]->ToggleRender( i == iCharType );
    }
    g_MyD3D->m_TempPlayer.AutoSetExtra();
    m_cFrame = 0;
    m_fCharX = 0.0f;

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = g_MyD3D->m_TempPlayer.Extra_Char_Num;

	if (m_iExtraCharNum<0 || m_iExtraCharNum >= CID_MAX)
		return;

    m_iMotionCount = 0;

    //  Ä³¸¯ÅÍ°¡ ¹Ù²¼À» °æ¿ì
    if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
    {
        ReloadMotion(); 
    }

    if( m_pWaitMotion != NULL )
        m_pShopMotion = m_pWaitMotion;

    //printf("ÄÉ¸¯ÅÍ°¡ %dÀ¸·Î ¹Ù²¼À½ : FileName(%s)\n", m_iExtraCharNum, m_pShopMotion->GetDeviceID().c_str() );

    int iTitleITemID = g_pItemMgr->GetEquipTitleItem( &g_MyD3D->m_TempPlayer.GetCurrentChar() );
    SetTitleItemID( iTitleITemID );

    SetPlayerExp();
}

void KGCCharWnd::ResetUIforResolution()
{
	//View port Àç ±¸¼º ÇØÁÜ..
	D3DXVECTOR2 vPos( GetFixedWindowPos() );
	D3DXVECTOR2 vAddLocalPos( m_pkBack_Color->GetFixedWindowLocalPos());
	m_viewport.X = static_cast<DWORD>((vPos.x +vAddLocalPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>((vPos.y + vAddLocalPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( m_pkBack_Color->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( m_pkBack_Color->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	KD3DWnd::ResetUIforResolution();
}

void KGCCharWnd::OnInitialize()
{
    SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
    //g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
	D3DXVECTOR2 vPos( GetFixedWindowPos() );
	D3DXVECTOR2 vAddLocalPos( m_pkBack_Color->GetFixedWindowLocalPos());
	m_viewport.X = static_cast<DWORD>( (vPos.x +vAddLocalPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>( (vPos.y + vAddLocalPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( m_pkBack_Color->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( m_pkBack_Color->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

void KGCCharWnd::UpdateMeshObject()
{
    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);	
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
    g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
    if( g_MyD3D->m_TempPlayer.m_pObject )
        g_MyD3D->m_TempPlayer.m_pObject->SetShellMatrixScale( 0.25f );  

    UpdatePet();        



	if ( m_bUseRoomObject)
	{
		if ( !m_pMeshKOGRoom )
		{
			m_pMeshKOGRoom = new GCObject();
#ifdef _DEBUG
			m_pMeshKOGRoom->m_strName = "·ë";
#endif
			m_pMeshKOGRoom->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "MyRoom.p3m" ), g_pGCDeviceManager2->CreateTexture( "MyRoom.dds") );
		}
	}
}

void KGCCharWnd::DeleteMeshObject()
{
    m_pkPet->EndPet();
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
    if ( m_pMeshKOGRoom )
    {
        g_RenderManager->RemoveObject( m_pMeshKOGRoom );
        SAFE_DELETE( m_pMeshKOGRoom );
    }
}
void KGCCharWnd::UpdateCharacterSelect()
{
	UpdateMeshObject();
	SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
}

void KGCCharWnd::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        g_KDSound.Play( "30" );
        return;
    }

    if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK ) return;

    if ( event.m_pWnd == m_pkInit )
    {
        // ³»Á¤º¸ ÅÇ¿¡¼­´Â ÀÔÇôº¸±âÇØÁ¦°¡ µ¿ÀÛÇÏÁö ¾Ê´Â´Ù.
        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "vip_gacha_box" )
            return;

        g_KDSound.Play( "31" );
        SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
        UpdateMeshObject();
        g_pkUIScene->m_pkCashShopScene->Update();
#ifndef DISABLE_MAP_SHOP // Kawan>
        g_pkUIScene->m_pkGPShopScene->Update();
#endif
        if ( g_pkUIScene->m_pkVipGachaBox )
        {
            g_pkUIScene->m_pkVipGachaBox->Dismount();
        }
    }
    else if ( event.m_pWnd == m_pkInfo )
    {

        g_KDSound.Play( "31" );
        if ( m_bShowInfo )  {
            m_bShowInfo = false;
        }
        else {
            m_bShowInfo = true;             
        }
    }
}



void KGCCharWnd::ChangeCharWndZoom( bool _bZoomUp )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if( CheckPosInWindowBound(vMousePos) == true )
    {
        // 0.25~0.15
#ifndef DISABLE_MAP_SHOP // Kawan>
        if( g_MyD3D->m_pStateMachine->IsStateMyInfo() || g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH || g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
#else
        if (g_MyD3D->m_pStateMachine->IsStateMyInfo() || g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
#endif
        {
            if( _bZoomUp )
                m_fZoom += DELTAZOOM;
            else
                m_fZoom -= DELTAZOOM;

            if( m_fZoom > MAXZOOM )
                m_fZoom = MAXZOOM;

            if( m_fZoom < MINZOOM )
                m_fZoom = MINZOOM;
        }
        else return;
    }
}

void KGCCharWnd::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCCharWnd::UpdatePet()
{
    // Æê : ÀÚ±â Á¤º¸Ã¢¿¡¼­ Ç¥½ÃÇØ ÁÖ´Â Æê
    // Æê º¸´Â Ã¢°ú Ä³¸¯ÅÍ º¸´Â Ã¢ÀÇ Æê Á¤º¸¸¦ ¸ðµÎ °»½ÅÇÑ´Ù
    KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
    int iPetEvolutionLevel = (int)kPet.m_cPromotion;
    int iPetLevel = kPet.m_iLevel;

    // 2008/01/30 Jaeho ÀÌºÎºÐÀÌ ¾ø¾î¼­
    // 1. ÆêÀÖ´Â Ä³¸¯ÅÍ ¼±ÅÃ»óÅÂ 2. Æê ¾ø´Â Ä³¸¯ÅÍ º¯°æ
    // ÀÇ °æ¿ì. 1¹ø Ä³¸¯ÅÍ°¡ °¡Áö°í ÀÖ¶² ÆêÀÌ »ç¶óÁöÁö ¾Ê°í À¯ÁöµÊ...
    // breadceo. ¹Ø¿¡ Àû¿ëµÇ¾îÀÖÀ½.. µ¿½Ã¿¡ ÀÛ¾÷Çß³ªº¸³×
    //if( m_pkPet )
    //    m_pkPet->EndPet();

    if ( iPetID != -1 )
    {
        if( iPetID != m_pkPet->m_iPetID ||
            iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
            iPetLevel != m_pkPet->m_iLevel )
        {
            m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID, true );
            m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
            //m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );
        }
    }
    else if( NULL != m_pkPet )
    {
        m_pkPet->EndPet();
    }
}

void KGCCharWnd::SetTitleItemID( int iTitleItemID )
{
    SAFE_RELEASE( m_pTexTitleItem );

    if( -1 == iTitleItemID )
        return;

    GCItem *pItem = g_pItemMgr->GetItemData( iTitleItemID );
    if( NULL == pItem )
        return;

    if( GIK_TITLE_ITEM == pItem->eItemKind
		|| g_pItemMgr->IsRallyTitleItemID( pItem->dwGoodsID ) )
    {
        m_pTexTitleItem = g_pItemMgr->CreateItemTexture( iTitleItemID, 0 );
        if( 0 == m_pTexTitleItem->GetDeviceTexture() )
            SAFE_RELEASE( m_pTexTitleItem );    
    }
}

void KGCCharWnd::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}

void KGCCharWnd::SetZoom(float zoom) {
    m_fZoom = zoom;
}