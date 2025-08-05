#include "stdafx.h"
#include "GCCoordiViewChar.h"
#include "KGCFaceLift.h"
#include "GCRenderManager.h"

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM   0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM     0.32f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM     0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM MAXZOOM

IMPLEMENT_CLASSNAME( KGCCoordiViewChar );
IMPLEMENT_WND_FACTORY( KGCCoordiViewChar );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiViewChar, "gc_coordiviewchar" );

KGCCoordiViewChar::KGCCoordiViewChar( void )
: m_pkBackground( NULL )
, m_pkRenderPosition( NULL )
{
	LINK_CONTROL( "back",				m_pkBackground );
	LINK_CONTROL( "render_position",	m_pkRenderPosition );

	//Character rendering stuff
	m_pMeshFootStage = NULL;
	m_pShopMotion = NULL;
	m_iMotionCount = 0;
	m_cFrame = 0;
	m_fCharX = 0.0f;

	m_fAngleMove = 0.0f;
	m_fZoom = 0.25;
	m_bAngleChanged = false;

	m_iExtraCharNum = 0;
	m_pItem = NULL;
}

KGCCoordiViewChar::~KGCCoordiViewChar( void )
{
	m_pShopMotion = NULL;
}

void KGCCoordiViewChar::OnCreateComplete()
{
	if( m_pkBackground )
	{
		m_pkBackground->InitState( true );
		m_pkBackground->SetPredraw( true );
	}

	if( m_pkRenderPosition )
	{
		m_pkRenderPosition->InitState( true );
		m_pkRenderPosition->SetPredraw( true );
	}

	InitPreview();

	m_iCurCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();
	m_iCurCharJob = g_kGlobalValue.m_kUserInfo.GetPromotion(m_iCurCharType);
	SetView();
}

void KGCCoordiViewChar::ActionPerformed( const KActionEvent& event )
{

}

void KGCCoordiViewChar::FrameMoveInEnabledState()
{
	FrameMove_Preview();
}

void KGCCoordiViewChar::PostDraw()
{
	RenderPreview();
}

void KGCCoordiViewChar::OnDestroy()
{
	m_iCurCharType = 0;
	m_iCurCharJob = 0;
	m_iCurrentCharacter = 0;
	m_mapItems.clear();

	DeleteMeshObject();
}

//Character rendering stuff
void KGCCoordiViewChar::ResetPlayer()
{
	m_Player = g_MyD3D->m_TempPlayer;
	m_Player.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
	m_iCurCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();
	m_iCurCharJob = g_kGlobalValue.m_kUserInfo.GetPromotion(m_iCurCharType);
	SetChar( m_iCurCharType );
	UpdateMeshObject();
}

void KGCCoordiViewChar::SetChar(int iCharType)
{
	InitCharacterInfo();
	m_Player.m_kUserInfo.SetCurrentChar( iCharType );

	m_cFrame = 0;
	m_fCharX = 0.0f;
	m_iMotionCount = 0;

	int iBeforeExtraCharNum = m_iExtraCharNum;
	m_iExtraCharNum = g_MyD3D->m_TempPlayer.Extra_Char_Num;

	if (m_iExtraCharNum < 0 || m_iExtraCharNum >= CID_MAX)
		return;

	m_iMotionCount = 0;

	if (m_iExtraCharNum != iBeforeExtraCharNum)
	{
		ReloadMotion();
	}
}

void KGCCoordiViewChar::InitPreview()
{
	for( int j = 0; j < NUM_JOB_LEVEL; j++ )
	{
		for( int i = 0; i < GC_CHAR_NUM; i++ )
		{
			char strTemp[MAX_PATH];
			std::string strShopMotion;
			sprintf( strTemp, "shop_%d_%d", i, j );

			strShopMotion = strTemp;

			// i + j * GC_CHAR_NUM == m_iExtraCharNum
			m_pWaitMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_0.frm" );
			if( m_pWaitMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{
				//assert(0);
				SAFE_RELEASE(m_pWaitMotion[i + j * GC_CHAR_NUM ]);
			}
			m_pActionMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_1.frm" );
			if( m_pActionMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{
				//assert(0);
				SAFE_RELEASE(m_pActionMotion[i + j * GC_CHAR_NUM ]);
			}
		}
	}

	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
	m_matProject = matLook * matScale * matTran;
}

void KGCCoordiViewChar::RenderPreview()
{
	// Çà·Ä ±¸ÇÏ´Â °÷ 
	g_pGCDeviceManager2->Flush();
	D3DVIEWPORT9 orgViewport;

	D3DXMATRIX matRot,matTrans,matScale,matWorld;
	g_pd3dDevice->GetViewport( &orgViewport );
	g_pd3dDevice->SetViewport( &m_viewport );

	g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
	//g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0L );

	// ¹ßÆÇ Ãâ·Â  
	float fScale = 4.1f;
	float fFootStageY = -0.15f;
	float fFootStageZ = 0.395f;
	float fFootStageMul = 0.46f;
	float fAngle = 0.9f;

	D3DXMatrixScaling( &matScale ,m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
	D3DXMatrixRotationY( &matRot, m_fStageAngleMove );
	D3DXMatrixTranslation( &matTrans, 0.0f, fFootStageY + ( MAXZOOM - m_fZoom ) * fFootStageMul, fFootStageZ );   

	matWorld = matScale * matRot * matTrans;
	if( m_pMeshFootStage != NULL )
	{
		m_pMeshFootStage->SetWorldMat( &matWorld );
		g_RenderManager->Render( false, m_pMeshFootStage, EGCShaderTechnique::CARTOON_NOEDGE );
	}

	if( m_pShopMotion != NULL )
	{
		if( !m_Player.m_pObject )
		{
			ResetPlayer();
		} 

		if ( m_cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
		{
			m_iMotionCount++;
			if( m_iMotionCount == 5 )
			{
				if( m_pActionMotion[m_Player.Extra_Char_Num] != NULL )
				{
					// ¸ð¼Ç?Ì ¹Ù²î¾ú?½
					m_pShopMotion = m_pActionMotion[m_Player.Extra_Char_Num];
					m_iMotionCount = 0;
					m_fCharX = 0.0f;// X¸¦ ÃÊ±âÈ­

					SiKGCFaceLift()->SetCurMotionName( m_pActionMotion[m_Player.Extra_Char_Num]->GetDeviceID() );
				}
			}
			else
			{
				if( m_pWaitMotion[m_Player.Extra_Char_Num] != NULL )
				{
					// ¸ð¼Ç?Ì ¹Ù²î¾ú?½
					m_pShopMotion = m_pWaitMotion[m_Player.Extra_Char_Num];
					m_fCharX = 0.0f;// X¸¦ ÃÊ±âÈ­

					SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion[m_Player.Extra_Char_Num]->GetDeviceID() );
				}
			}

			m_cFrame = 0;
		}

		float fPosX = m_fCharX * CHAR_SCALE_RATIO;
		float fPosY = m_pShopMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.03f;
		float fPosZ = 0.37f;

		m_fCharX += m_pShopMotion->GetFrameData(m_cFrame).Plus_x;

		fPosX = fPosX * cos( m_fAngleMove );

		D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
		D3DXMatrixRotationY( &matRot, m_fAngleMove );
		D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

		matWorld = matScale * matRot * matTrans;

		m_Player.m_pObject->SetWorldMat( &matWorld );
		m_Player.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );
		m_Player.SetRenderFaceLift( m_Player.GetDefaultFaceLiftType() );
		SiKGCFaceLift()->SetFaceLiftByMotionName( &m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );
		g_RenderManager->Render( false ,m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);

		m_cFrame++;
	}        

	// ºäÆ÷Æ® º¹±¸
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
	g_pd3dDevice->SetViewport( &orgViewport );
	g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCCoordiViewChar::FrameMove_Preview( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
	bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, true);

	if (g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum || !g_MyD3D->m_TempPlayer.m_pObject)
	{
		int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
		g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar(iSelectChar);
		SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
		g_MyD3D->m_TempPlayer.RemovePlayerObject();
		g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
	}

	if( !m_bAngleChanged )
	{
		m_fAngleMove += 0.005f;
		m_fStageAngleMove += 0.0035f;
		if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
		if ( m_fStageAngleMove >= 2 * D3DX_PI ) m_fStageAngleMove = 0.0f;
	}

	if( g_pkInput->ISWheelUp() && bCheckBound )
		ChangeCharWndZoom(true);
	else if( g_pkInput->ISWheelDown() && bCheckBound )
		ChangeCharWndZoom(false);

	static bool bInternal = false;

	if( g_pkInput->BtnDown() == true &&  bCheckBound )
	{
		m_bAngleChanged = true;
		m_ptStartMouse = g_pkInput->GetMousePos();
		m_fStartAngle = m_fAngleMove;
		m_fStartStageAngle = m_fStageAngleMove;
		bInternal = true;
	}
	else if( g_pkInput->BtnDown() == true &&  !bCheckBound )
		bInternal = false;

	if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE  )
	{
		m_bAngleChanged = false;
	}

	if( g_pkInput->BtnPressed() && m_bAngleChanged == true && bInternal == true )
		ProcessCharWndAngle();

	if( m_Player.m_pObject )
	{
		m_Player.SetRender( true );
		m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
		m_Player.m_pObject->ChildFrameMove();
	}
}

void KGCCoordiViewChar::OnViewBasicClick()
{
	g_KDSound.Play( "31" );
	m_mapItems.clear();
	UpdateMeshObject();
}

void KGCCoordiViewChar::SetView()
{
	if( 0 > m_iExtraCharNum || m_iExtraCharNum > CID_MAX)
	{
		m_iExtraCharNum = m_iCurCharType;
		m_Player.Extra_Char_Num = m_iCurCharType;
	}

	if( m_pWaitMotion[m_iExtraCharNum] != NULL )
		m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
	else
	{
		m_iExtraCharNum = m_iCurCharType;
		m_Player.Extra_Char_Num = m_iCurCharType;
		if( m_pWaitMotion[m_iExtraCharNum] != NULL )
			m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
	}

	D3DXVECTOR2 vPos( GetFixedWindowPos() );
	m_viewport.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X * GC_SCREEN_DIV_WIDTH + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y * GC_SCREEN_DIV_WIDTH + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( VIEWPORT_WIDTH * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( VIEWPORT_HEIGHT * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

void KGCCoordiViewChar::UpdateMeshObject()
{
	SetView();
	g_pItemMgr->UnequipItemAll( &(m_Player.m_kUserInfo.GetCurrentChar( m_iCurCharType )) );

	if( !m_mapItems.empty() )
	{
		std::map<DWORD, GCItem*>::iterator it = m_mapItems.begin();

		while( it != m_mapItems.end() )
		{
			g_pItemMgr->EquipItem( &(m_Player.m_kUserInfo.GetCurrentChar( m_iCurCharType )), it->second);
			++it;
		}
	}
    
	m_Player.AutoSetExtra();

	int iBeforeExtraCharNum = m_iExtraCharNum;
	m_iExtraCharNum = m_Player.Extra_Char_Num;

	//  캐릭터가 바꼈을 경우
	if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
	{
		ReloadMotion();
	}

	m_Player.UpdatePlayerResourceClean(true);

	switch( m_iExtraCharNum % GC_CHAR_NUM )
	{
		case GC_CHAR_DIO:
			m_Player.ToggleExtraMesh( "WEAPON", true );
			m_Player.ToggleExtraMesh( "MERGE_WEAPON", false );
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
		m_Player.m_pObject->SetShellMatrixScale( 0.25f );  

    if( NULL == m_pMeshFootStage )
    {
        m_pMeshFootStage = new GCObject();
        m_pMeshFootStage->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "footstage.p3m" ), g_pGCDeviceManager2->CreateTexture( "footstage.dds" ) );
    }
}

void KGCCoordiViewChar::DeleteMeshObject()
{
	m_Player.ReleasePlayerResource();

	if( m_pMeshFootStage )
	{
		g_RenderManager->RemoveObject( m_pMeshFootStage );
		SAFE_DELETE( m_pMeshFootStage );
	}

	for( int j = 0; j < NUM_JOB_LEVEL; j++ )
	{
		for( int i = 0; i < GC_CHAR_NUM; i++ )
		{
			SAFE_RELEASE(m_pWaitMotion[i + j * GC_CHAR_NUM ]);
			SAFE_RELEASE(m_pActionMotion[i + j * GC_CHAR_NUM ]);
		}
	}
}

void KGCCoordiViewChar::InitCharacterInfo()
{
	std::map<int, std::set< char> > mapCharInfo = g_kGlobalValue.GetOpenedCharacterInfo();
	std::map<int, std::set< char> >::iterator mit = mapCharInfo.begin();

	m_Player.m_kUserInfo.Init();

	for( ; mit != mapCharInfo.end(); ++mit )
	{
		SCharInfo charInfo;
		charInfo.iCharType = mit->first;
		if ( charInfo.iCharType == GC_CHAR_DIO )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_DIO ).setPromotion;
		else if ( charInfo.iCharType == GC_CHAR_ZERO )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_ZERO ).setPromotion;
		else if ( charInfo.iCharType == GC_CHAR_LEY )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_LEY ).setPromotion;
		else
			charInfo.setPromotion = mit->second;

		m_Player.m_kUserInfo.AddChar(charInfo);
	}
}

void KGCCoordiViewChar::ChangeCharWndZoom( bool bZoom )
{
	if( bZoom )
		m_fZoom += DELTAZOOM;
	else
		m_fZoom -= DELTAZOOM;

	if( m_fZoom > MAXZOOM )
		m_fZoom = MAXZOOM;

	if( m_fZoom < MINZOOM )
		m_fZoom = MINZOOM;
}

void KGCCoordiViewChar::ProcessCharWndAngle()
{
	POINT CurrentMousePoint = g_pkInput->GetMousePos();

	m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCCoordiViewChar::SetItem(GCItem* pItem)
{
	std::map<DWORD, GCItem*>::iterator it = m_mapItems.find(pItem->dwSlotPosition);

	if( it == m_mapItems.end() )
	{
		m_mapItems.insert( std::make_pair( pItem->dwSlotPosition, pItem ) );
	}
	else
	{
		if( it->second->dwGoodsID == pItem->dwGoodsID )
		{
			it = m_mapItems.erase( it );
		}
		else
		{
			it = m_mapItems.erase( it );
			m_mapItems.insert( std::make_pair( pItem->dwSlotPosition, pItem ) );
		}
	}

	UpdateMeshObject();
}

void KGCCoordiViewChar::RemoveItem( GCItem* pItem )
{
	std::map<DWORD, GCItem*>::iterator it = m_mapItems.find( pItem->dwSlotPosition );

	if ( it != m_mapItems.end() )
	{
		if ( it->second->dwGoodsID == pItem->dwGoodsID )
		{
			it = m_mapItems.erase( it );
		}
	}

	UpdateMeshObject();
}

void KGCCoordiViewChar::ReloadMotion()
{
	GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion[m_iExtraCharNum], &m_pActionMotion[m_iExtraCharNum] );
}

std::vector<GCITEMID> KGCCoordiViewChar::GetEquipedItemIDVector()
{
	std::vector<GCITEMID> v;

	for (std::map<DWORD, GCItem*>::iterator it = m_mapItems.begin(); it != m_mapItems.end(); ++it) {
		v.push_back( it->second->dwGoodsID * 10 );
	}

	return v;
}