#include "stdafx.h"
#include ".\gcdrawcharacter.h"
#include "GCRenderManager.h"
#include "MyD3D.h"

KGCDrawCharacter::KGCDrawCharacter(void)
: m_fAngleMove( 0.0f )
, m_iMotionCount( 0 )
, m_fZoom( 1.7f )
, m_fCharX( 0.0f )
, m_fYow( 0.0f )
, m_fPitch( 0.0f )
, m_fRoll( 0.0f )
, m_fPosX( 0.0f )
, m_fPosY( 0.0f )
, m_fPosz( 0.0f )
, m_iMotionLoop( DC_ONCE )
, m_pBackground( NULL )
, m_pCurrentMotion( NULL )
{
}

KGCDrawCharacter::KGCDrawCharacter( IDirect3DDevice9* pDevice )
: m_pd3dDevice( pDevice )
{
    KGCDrawCharacter();
}

KGCDrawCharacter::~KGCDrawCharacter(void)
{
    Clear();
}

void KGCDrawCharacter::Draw()
{    
	g_pGCDeviceManager2->Flush();
	D3DVIEWPORT9 orgViewport;

	D3DXMATRIX matRot,matTrans,matScale,matWorld;
	m_pd3dDevice->GetViewport( &orgViewport );
	m_pd3dDevice->SetViewport( &m_viewport );

	m_pd3dDevice->SetFVF( SKINVERTEX::FVF );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

	D3DXMatrixScaling( &matScale ,m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
	D3DXMatrixRotationY( &matRot, m_fAngleMove );
	D3DXMatrixTranslation( &matTrans, 0.0f, 0.081f - m_fZoom * 0.25f, 0.32f );   

	matWorld = matScale * matRot * matTrans;

    if ( m_Player.m_pLire3Arrow )
    {
        m_Player.m_pLire3Arrow->m_bRender = false;
    }
    
    switch( m_Player.Extra_Char_Num % GC_CHAR_NUM )
    {
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

     if( m_pBackground != NULL )
	{
		m_pBackground->SetWorldMat( &matWorld );

		g_RenderManager->Render( false , m_pBackground , EGCShaderTechnique::NOCARTOON_NOEDGE );
	}


	if( m_pCurrentMotion != NULL )
	{
		float fPosX = m_fCharX * CHAR_SCALE_RATIO - 0.15f;
		float fPosY = m_pCurrentMotion->GetFrameData(m_Player.cFrame).Pos_y - 0.8f;
		float fPosZ = 3.0f;

		m_fCharX += m_pCurrentMotion->GetFrameData(m_Player.cFrame).Plus_x;

		D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
		D3DXMatrixRotationY( &matRot, m_fAngleMove );
		D3DXMatrixRotationYawPitchRoll( &matRot, m_fYow, m_fPitch, m_fRoll );
		D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

		matWorld = matScale * matRot * matTrans;
		m_Player.m_pObject->SetWorldMat( &matWorld );
		m_Player.m_pObject->SetFrame( &m_pCurrentMotion->GetFrameData( (int) m_Player.cFrame ) );
        m_Player.m_pObject->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE * 0.7f );

		g_RenderManager->Render( false , m_Player.m_pObject );

        if( !m_Player.m_bFrameLock && !m_Player.m_bBuffFrameLock )
		    ++m_Player.cFrame;

		if( m_Player.cFrame > m_pCurrentMotion->GetNum_Frame() - 1 )
		{
			ChangeMotion();
		}
        else
        {
            // 챠일드 메쉬 프레임무브
            m_Player.m_pObject->SetScale( D3DXVECTOR3(m_fZoom, m_fZoom, m_fZoom) );
            m_Player.m_pObject->ChildFrameMove();
	}
	}
	else
	{
		ChangeMotion();
	}

	// 뷰포트 복구
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
	m_pd3dDevice->SetViewport( &orgViewport );
	m_pd3dDevice->SetTexture( 1, NULL );
}

void KGCDrawCharacter::SetViewPort( D3DXVECTOR2 vPosition, DWORD dwWidth, DWORD dwHeight )
{
	m_viewport.X = static_cast<DWORD>( vPosition.x * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Y = static_cast<DWORD>( vPosition.y * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.Width = static_cast<DWORD>( dwWidth * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( dwHeight * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;


	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixPerspectiveLH( &matLook, float( dwWidth ) / float( dwHeight ),
		1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
	m_matProject = matLook * matScale * matTran;
}

void KGCDrawCharacter::SetViewPort( DWORD dwPosX, DWORD dwPosY, DWORD dwWidth, DWORD dwHeight )
{
	m_viewport.X = dwPosX;
	m_viewport.Y = dwPosY;
	m_viewport.Width = dwWidth;
	m_viewport.Height = dwHeight;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;


	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixPerspectiveLH( &matLook, float( dwWidth ) / float( dwHeight ),
		1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
	m_matProject = matLook * matScale * matTran;
}

void KGCDrawCharacter::AddMotion( std::string DeviceID, int iMotionID )
{
	GCDeviceMotionFRM* pMotion = g_pGCDeviceManager->CreateMotionFRM( DeviceID );

	if( pMotion == NULL )
	{
		ASSERT( !"이상한 모션을 로드하라고 시키네요" );
		return;
	}
	if( pMotion->IsDisable() )
	{
		ASSERT( !"없는 모션을 로드하라고 시킨듯..." );
		SAFE_RELEASE(pMotion);
		return;
	}

    m_vecMotionID.push_back( iMotionID );
	m_vecMotions.push_back( pMotion );
}

void KGCDrawCharacter::Clear()
{
	for( int i = 0 ; i < (int)m_vecMotions.size() ; ++i )
	{
		SAFE_RELEASE( m_vecMotions[i] );
	}
    m_vecMotionID.clear();
	m_vecMotions.clear();
	m_Player.cFrame = 0;
	m_Player.ReleasePlayerResource();
	m_pCurrentMotion = NULL;

	if( m_pBackground )
	{
		g_RenderManager->RemoveObject( m_pBackground );
		SAFE_DELETE( m_pBackground );
	}
}

void KGCDrawCharacter::SetBackgroundMesh( std::string MeshID )
{

}

void KGCDrawCharacter::ChangeMotion()
{
    m_fCharX = 0.0f;

	static int iMotionIndex = 0;
	if( m_vecMotions.empty() )
    {
        m_Player.m_bFrameLock = true;
		return;
    }

    m_Player.m_bFrameLock = false;
	if( iMotionIndex > (int)m_vecMotions.size() )
		iMotionIndex = 0;

	m_pCurrentMotion = m_vecMotions[iMotionIndex];

    // 엑스트라 매시 모션처리
    m_Player.CreateShowRoomETCMotionFile( m_vecMotionID[iMotionIndex] );

	switch( m_iMotionLoop )
	{
	case DC_ONCE:
		--m_Player.cFrame;
		break;
	case DC_LOOP_ONE:
		m_Player.cFrame = 0;
		break;
	case DC_LOOP_ALL:
		++iMotionIndex;
		m_Player.cFrame = 0;
		break;
	case DC_LOOP_RANDOM:
		++iMotionIndex;	// milennium9 test code 
		m_Player.cFrame = 0;
		break;
	}
}

void KGCDrawCharacter::SetPlayerInfo( SUserInfo sUserInfo )
{
	m_Player.m_kUserInfo = sUserInfo;
	CreatePlayer();
	m_Player.AutoSetExtra();
	m_Player.cFrame = 0;
}

void KGCDrawCharacter::CreatePlayer()
{
    m_Player.SetMyPlayer(-1, true);
	m_Player.ReleasePlayerResource();
    m_Player.RemovePlayerObject();
	m_Player.UpdatePlayerResource( true );    
	//m_Player.m_pObject->SetShellMatrixScale( 0.25f );
	m_Player.AutoSetExtra();
}

void KGCDrawCharacter::SetD3DDevice( IDirect3DDevice9* pDevice )
{
	if( pDevice == NULL )
	{
		ASSERT( !"왜 NULL을 할당하려 하실까??" );
		return;
	}
	m_pd3dDevice = pDevice;
}

void KGCDrawCharacter::ClearMotionList()
{
    m_vecMotionID.clear();
	m_vecMotions.clear();
	m_pCurrentMotion = NULL;
}
