#include "stdafx.h"
#include "KGCShortMenuCharView.h"
#include "KGCFaceLift.h"
#include <GCUtil/GCFileMoniter.h>
#include "GCRenderManager.h"

static bool m_bReloadShortMenuData = false;
void ReLoadShortMenuData()
{
	KGCGlobal::LoadShortMenuInfo();
	m_bReloadShortMenuData = true;
}

IMPLEMENT_CLASSNAME( KGCShortMenuCharView );
IMPLEMENT_WND_FACTORY( KGCShortMenuCharView );
IMPLEMENT_WND_FACTORY_NAME( KGCShortMenuCharView, "gc_shortmenu_charview" );

#define _DELTAZOOM 0.01f
#define _MAXZOOM 0.23f
#define _MINZOOM 0.18f

KGCShortMenuCharView::KGCShortMenuCharView(void)
{
	m_pBackgroundTex = NULL;
	m_pkRenderFrame = NULL;
	LINK_CONTROL("renderframe", m_pkRenderFrame);

	m_pMeshKOGRoom = NULL;
	m_pCharMotion = NULL;
	m_iMotionCount = 0;
	m_cFrame = 0;
	m_fCharX = 0.0f;

	m_fAngleMove = 0.0f;
	m_fZoom = 0.25;
	m_bAngleChanged = false;

	m_iExtraCharNum = 0;

	m_pkPet = NULL;

	m_pWaitMotion = NULL;
	m_pActionMotion = NULL;

	m_bRemoveWeapon = false;
}

KGCShortMenuCharView::~KGCShortMenuCharView(void)
{
	SAFE_DELETE( m_pMeshKOGRoom );
	SAFE_RELEASE( m_pBackgroundTex );
	m_pCharMotion = NULL;

	if( m_pkPet != NULL )
		m_pkPet->EndPet();

	SAFE_DELETE( m_pkPet );
}

void KGCShortMenuCharView::OnDestroy( void )
{
	SAFE_RELEASE( m_pBackgroundTex );
	SAFE_DELETE( m_pMeshKOGRoom );
	SAFE_RELEASE( m_pBackgroundTex );
	m_pCharMotion = NULL;
	m_pWaitMotion = NULL;
	m_pActionMotion = NULL;

	m_bRemoveWeapon = false;

	if( m_pkPet != NULL )
		m_pkPet->EndPet();

	SAFE_DELETE( m_pkPet );
}

void KGCShortMenuCharView::OnCreate()
{
	m_pkRenderFrame->InitState( false );

	if( m_pkPet == NULL )
		m_pkPet = new KPet();

	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixPerspectiveLH( &matLook, float( m_pkRenderFrame->GetWidth() ) / float( m_pkRenderFrame->GetHeight() ), 1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
	m_matProject = matLook * matScale * matTran;

	g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_LOADING );

	SiKGCFileMoniter()->RegisterFile(g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath("shortmenucharview.lua"), ReLoadShortMenuData);
}

void KGCShortMenuCharView::PostChildDraw()
{
	g_pGCDeviceManager2->Flush();
	D3DVIEWPORT9 orgViewport;

	D3DXMATRIX matRot,matTrans,matScale,matWorld;
	g_pd3dDevice->GetViewport( &orgViewport );
	g_pd3dDevice->SetViewport( &m_viewport );

	g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

	float fScale = 5.2f;
	float fFootStageY = -0.15f;
	float fFootStageZ = 0.38f;
	float fFootStageMul = 0.44f;

	D3DXMatrixScaling( &matScale ,m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
	D3DXMatrixTranslation( &matTrans, 0.0f, fFootStageY + ( _MAXZOOM - m_fZoom ) * fFootStageMul, fFootStageZ );   

	matWorld = matScale * matTrans;
	if(m_pMeshKOGRoom != NULL)
	{
		m_pMeshKOGRoom->SetWorldMat( &matWorld );
		g_pGCDeviceManager2->DrawInScreen( m_pBackgroundTex, (float)m_viewport.X - 90.f, (float)m_viewport.Y * 1.28f, 
			(float)m_viewport.Width * 1.28f, (float)m_viewport.Height * 1.28f, 
			0.f, 0.f, 
			1.0f, 1.0f);
		g_RenderManager->Render(false, m_pMeshKOGRoom, EGCShaderTechnique::CARTOON_BLACKEDGE);
		g_pGCDeviceManager2->PushState();
		g_pGCDeviceManager2->PopState();
	}

	if( m_pCharMotion != NULL )
	{
		std::string strMotionName = "";
		if ( m_cFrame > (int)m_pCharMotion->GetNum_Frame() - 1 )
		{
			if( m_bRemoveWeapon == false )
			{
				m_iMotionCount++;
				if( m_iMotionCount == 5 )
				{
					if( m_pActionMotion )
					{
						m_pCharMotion = m_pActionMotion;
						m_iMotionCount = 0;
						m_fCharX = 0.0f;
						SiKGCFaceLift()->SetCurMotionName( m_pActionMotion->GetDeviceID() );
					}
				}
				else
				{
					if( m_pWaitMotion )
					{
						m_pCharMotion = m_pWaitMotion;
						m_fCharX = 0.0f;
						SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion->GetDeviceID() );
					}
				}
			}

			m_cFrame = 0;
		}

		float fPosX = 0.01f + m_fCharX * CHAR_SCALE_RATIO;
		float fPosY = m_pCharMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.06f;
		float fPosZ = 0.303f;
		float fCharScale = 1.0f;

		m_fCharX += m_pCharMotion->GetFrameData(m_cFrame).Plus_x;

		fPosX = fPosX * cos( m_fAngleMove );

		D3DXMatrixScaling( &matScale , m_fZoom * fCharScale, m_fZoom * fCharScale, m_fZoom * fCharScale );
		D3DXMatrixRotationY( &matRot, m_fAngleMove );
		D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

		matWorld = matScale * matRot * matTrans;

		m_Player.m_pObject->SetWorldMat( &matWorld );
		m_Player.m_pObject->SetFrame( &m_pCharMotion->GetFrameData( (int)m_cFrame ) );
		m_Player.SetRenderFaceLift( m_Player.GetDefaultFaceLiftType() );
		SiKGCFaceLift()->SetFaceLiftByMotionName( &m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );
		g_RenderManager->Render( false , m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
		m_cFrame++;

		if( m_pkPet && m_pkPet->m_iPetID != -1 && !g_MyD3D->m_KGCOption.GetPetViewHide() )
		{
			float fScale = 2.5f * m_pkPet->m_fScale;
			D3DXMATRIX mat1, mat2, mat3, mat4, matWorld;

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
}

void KGCShortMenuCharView::ActionPerformed( const KActionEvent& event )
{
}

void KGCShortMenuCharView::FrameMoveInEnabledState( void )
{
	if ((m_pkPet->m_pObject && g_MyD3D->m_KGCOption.GetPetViewHide()) || (!m_pkPet->m_pObject && !g_MyD3D->m_KGCOption.GetPetViewHide()))
		UpdatePet();

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
	bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, false);

	if( !m_bAngleChanged )
	{
		m_fAngleMove += 0.005f;
		if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
	}

	if( g_pkInput->ISWheelUp() && bCheckBound )
		ChangeCharWndZoom(true);
	else if( g_pkInput->ISWheelDown() && bCheckBound )
		ChangeCharWndZoom(false);

	static bool Internal = false;

	if( g_pkInput->BtnDown() == true &&  bCheckBound )
	{
		m_bAngleChanged = true;
		m_ptStartMouse = g_pkInput->GetMousePos();
		m_fStartAngle = m_fAngleMove;
		Internal = true;
	}
	else if ( g_pkInput->BtnDown() == true && !bCheckBound ) 
	{
		Internal = false;
	}

	if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE && bCheckBound )
	{
		m_bAngleChanged = false;
	}

	if( g_pkInput->BtnPressed() && m_bAngleChanged == true && Internal == true )
		ProcessCharWndAngle();

	if( m_Player.m_pObject )
	{
		m_Player.SetRender( true );
		m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
		m_Player.m_pObject->ChildFrameMove();
	}
}

void KGCShortMenuCharView::SetChar()
{
	m_cFrame = 0;
	m_fCharX = 0.0f;
	m_iMotionCount = 0;
	m_iExtraCharNum = m_Player.Extra_Char_Num;
}

void KGCShortMenuCharView::UpdateMeshObject( void )
{
	m_Player.ReleasePlayerResource();
	m_Player.SetMyPlayer(-1, true);	

	int iBeforeExtraCharNum = m_iExtraCharNum;
	m_iExtraCharNum = m_Player.Extra_Char_Num;

	if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
	{
		ReloadMotion();
	}

	if( m_pWaitMotion != NULL && m_pCharMotion == NULL)
	{
		m_pCharMotion = m_pWaitMotion;
	}

	m_Player.m_iPlayerIndex = 0;
	m_Player.m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;

	m_Player.UpdatePlayerResource(true);

	switch( m_iExtraCharNum % GC_CHAR_NUM )
	{
	case GC_CHAR_DIO:
		m_Player.ToggleExtraMesh( "WEAPON", true );
		break;
	case GC_CHAR_ZERO:
		m_Player.ToggleExtraMesh( "GRANDARK_2ND", false );
		m_Player.ToggleExtraMesh( "GRANDARK_2ND_1", false );
		m_Player.ToggleExtraMesh( "GRANDARK_2ND_2", false );
		m_Player.ToggleExtraMesh( "DOG_GRANDARK", false );
		break;
	case GC_CHAR_LUPUS:
		m_Player.ToggleExtraMesh( "WEAPON_0", true );
		m_Player.ToggleExtraMesh( "WEAPON_1", true );
		break;
	default:
		m_Player.ToggleExtraMesh( "WEAPON", m_Player.GetCurFormTemplate().WeaponRenderBasically );
		m_Player.ToggleExtraMesh( "MERGE_WEAPON", m_Player.GetCurFormTemplate().LeftWeaponRenderBasically );
		break;
	}
	
	if( m_Player.m_pObject )
	{
		m_Player.m_pObject->SetShellMatrixScale( 0.15f );  
	}

	if ( m_pMeshKOGRoom )
	{
		SAFE_DELETE(m_pMeshKOGRoom);
	}

	m_pMeshKOGRoom = new GCObject();
	m_pBackgroundTex = g_pGCDeviceManager2->CreateTexture( m_kCharInfo.m_strBackgroundTexture );
	m_pMeshKOGRoom->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(m_kCharInfo.m_strFootholdModel), g_pGCDeviceManager2->CreateTexture(m_kCharInfo.m_strFootholdTexture));
}

void KGCShortMenuCharView::DeleteMeshObject( void )
{
	m_pkPet->EndPet();
	m_Player.ReleasePlayerResource();
	if ( m_pMeshKOGRoom )
	{
		g_RenderManager->RemoveObject( m_pMeshKOGRoom );
		SAFE_DELETE( m_pMeshKOGRoom );
	}
}

void KGCShortMenuCharView::ChangeCharWndZoom( bool bZoom )
{
	if( bZoom )
		m_fZoom += _DELTAZOOM;
	else
		m_fZoom -= _DELTAZOOM;

	if( m_fZoom > _MAXZOOM )
		m_fZoom = _MAXZOOM;

	if( m_fZoom < _MINZOOM )
		m_fZoom = _MINZOOM;
}

void KGCShortMenuCharView::ProcessCharWndAngle( void )
{
	POINT CurrentMousePoint = g_pkInput->GetMousePos();
	m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * _DELTAZOOM;
}

void KGCShortMenuCharView::UpdatePet()
{
	KPetInfo kPet = m_Player.GetCurrentChar().kPetInfo;
	int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
	int iPetEvolutionLevel = (int)kPet.m_cPromotion;
	int iPetLevel = kPet.m_iLevel;
	DWORD dwPetCostumeID = kPet.m_kCostume.m_dwID;

	if( m_pkPet != NULL )
	{
		if( kPet.m_dwUID != 0 && iPetID != -1 && !g_MyD3D->m_KGCOption.GetPetViewHide())
		{
			if( iPetID != m_pkPet->m_iPetID ||
				iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
				iPetLevel != m_pkPet->m_iLevel ||
				dwPetCostumeID != m_pkPet->GetCostumeID() )
			{
				m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID, true );
				m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
			}
		}
		else
		{
			m_pkPet->EndPet();
		}
	}
}

void KGCShortMenuCharView::ReloadMotion()
{
	if( m_pActionMotion == NULL )
	{	
		GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );	
	}
	else
	{
		m_pWaitMotion = m_pActionMotion; 
	}
}

void KGCShortMenuCharView::SetPreviewWindow( PLAYER* pPlayer )
{
	D3DXVECTOR2 vPos( GetFixedWindowPos() );

	int nWidthX = m_pkRenderFrame->GetWidth();
	int nHeightX = m_pkRenderFrame->GetHeight();        
	D3DXVECTOR2 vtPos = m_pkRenderFrame->GetCurrentWindowLocalPos();

	m_viewport.X = static_cast<DWORD>(( static_cast<int>(vtPos.x) + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>(( static_cast<int>(vtPos.y) + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( nWidthX * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( nHeightX * g_pGCDeviceManager2->GetWindowScaleY() );

	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	m_Player = *pPlayer;
	m_Player.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
	m_Player.m_kUserInfo.SetCurrentChar( m_Player.GetCurrentChar().iCharType );
	m_Player.AutoSetExtra();

	bool removeWeapon = false;
	m_kCharInfo = g_kGlobalValue.GetShortMenuInfoForCharType(m_Player.GetCurrentChar().iCharType);

	if( rand()%30 > 15)
	{
		std::string motion = m_kCharInfo.m_vecMotion[rand() % m_kCharInfo.m_vecMotion.size()];
		m_pActionMotion = g_pGCDeviceManager2->CreateMotionFRM( motion );
		SCharInfo& charInfo = m_Player.GetCurrentChar();
		g_pItemMgr->UnEquipWeapon(&charInfo);
		m_bRemoveWeapon = true;
	}

	SetChar();
	ReloadMotion();
	UpdateMeshObject();
	UpdatePet();
}