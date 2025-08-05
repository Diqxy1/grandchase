#include "stdafx.h"
#include ".\kgc3dobject.h"
#include "GCRenderManager.h"


#include "MyD3D.h"
//

KGC3DObject::KGC3DObject(void) 
: m_pkModel(NULL)
, m_bRender(false)
, m_fScale(1.0f)
, m_bIsRight(true)
, m_ucFrame(0)
, m_uiMotion(0)
, m_eCartoon(CARTOON_NORMAL)
, m_bIsLive(false)
, m_pCurMotion(NULL)
, m_Slow_Count(0)
{
    m_vecMotion.clear();
    m_vPos.x = 0.0f;
    m_vPos.y = 0.0f;
    m_vLocalPos.x = 0.0f;
    m_vLocalPos.y = 0.0f;
	m_vLocalPos.z = 0.5f;
    m_fZaxis = 0.5f;
}

KGC3DObject::~KGC3DObject(void)
{
}

void KGC3DObject::Init( void )
{
	SAFE_REMOVE_DELETE(m_pkModel);

	m_pkModel = SiKGCRenderManager()->CreateObject();

	m_pkModel->SetMeshComputePaletteInterface( NULL );
	m_pkModel->SetRender( false );
	m_pkModel->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE );
}

void KGC3DObject::Init( int iTech_ )
{
	SAFE_REMOVE_DELETE(m_pkModel);

	m_pkModel = SiKGCRenderManager()->CreateObject(iTech_);

	m_pkModel->SetMeshComputePaletteInterface( NULL );
	m_pkModel->SetRender( false );
	m_pkModel->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE );
}

void KGC3DObject::Release()
{
    SAFE_REMOVE_DELETE( m_pkModel );

    for( int i = 0; i < (int)m_vecMotion.size(); ++i )
    {
        SAFE_RELEASE( m_vecMotion[i] );
    }
    m_vecMotion.clear();
}

void KGC3DObject::FrameMove()
{
    if( m_bIsLive == false )
        return;

	if( m_pkModel == NULL )
		return;

	if( m_vecMotion.size() <= m_uiMotion || m_uiMotion < 0 )
		return;

	if( m_vecMotion[m_uiMotion]->GetDeviceState() != GCDS_LOADED )
	{
		g_kGlobalValue.m_bIncompletePatch = true;
		return;
	}

    D3DXMATRIX matScale, matRot, matTrans, matWorld;

    union SHORT_TO_FLOAT
    {
        short short_[2];
        float float_;
    };

    SHORT_TO_FLOAT shot;
    shot.short_[0] = m_vecMotion[m_uiMotion]->GetFrameData(m_ucFrame).Reserved1;
    shot.short_[1] = m_vecMotion[m_uiMotion]->GetFrameData(m_ucFrame).Reserved2;

    m_pkModel->SetRender( m_bRender );

    D3DXMatrixScaling( &matScale, m_fScale, m_fScale, 0.35f );
    if( true == m_bIsRight )
    {
        D3DXMatrixRotationY( &matRot, 0.0f );
    }
    else
    {
        D3DXMatrixRotationY( &matRot, 3.14159f );
        shot.float_ *= -1.0f;
    }
    D3DXMatrixTranslation( &matTrans, m_vPos.x + m_vLocalPos.x, m_vPos.y + m_vLocalPos.y/* + m_vecMotion[m_uiMotion]->GetFrameData( m_cFrame ).Pos_y * m_fScale*/, m_vLocalPos.z + shot.float_ * m_fScale );
    matWorld = matScale * matRot * matTrans;

	m_pkModel->SetWorldMat( &matWorld );
	m_pkModel->SetFrame( &m_vecMotion[m_uiMotion]->GetFrameData(m_ucFrame) );

	/*if( g_MyD3D->m_KGCOption.IsCartoon() )
	{
	m_pkModel->SetCartoon( CARTOON_DEFAULT );
	}
	m_pkModel->SetTexture( -1 );*/

	//====================================================================================
	// 2009.02.13 : Jemitgge
	// Comment : Player에 Slow_Count가 걸려있으면 그대로 멈춰라~
	if ( m_Slow_Count <= 0 )
	{
		m_ucFrame++;
	}
    
    if( m_vecMotion[m_uiMotion]->GetNum_Frame() - 1 < m_ucFrame )
    {
        m_ucFrame = 0;
    }
}

D3DXVECTOR2 KGC3DObject::GetMotionPos() const
{
    SFrame& frame = m_vecMotion[m_uiMotion]->GetFrameData( m_ucFrame );
    return D3DXVECTOR2( frame.Plus_x * m_fScale, frame.Pos_y * m_fScale );
}

// [12/3/2007] breadceo. Init을 하고 Add해야함
void KGC3DObject::AddMesh( const std::string& strMesh, const std::string& strTex, std::string strID, DWORD technique_ )
{
    m_pkModel->AddMesh( strMesh, strTex, -1, m_eCartoon, strID, technique_ );
}

// [1/7/2008] breadceo. 궁수처럼 활질할 필요는 없으므로 항상 버퍼링을 사용한다
void KGC3DObject::AddMotion( const std::string& strMotion )
{
    GCDeviceMotionFRM* pFrm = g_pGCDeviceManager2->CreateMotionFRM( strMotion );
    ASSERT( true != pFrm->IsDisable() );

	// 모션이 없으면 크래쉬가 나서 없는 모션이면 홀드모션으로 대체함.
	if( pFrm->IsDisable() )
	{
		pFrm = g_pGCDeviceManager2->CreateMotionFRM( "1455.frm" );
		g_kGlobalValue.m_bIncompletePatch = true;
	}

    if( m_vecMotion.end() == std::find( m_vecMotion.begin(), m_vecMotion.end(), pFrm ) )
    {
		m_vecMotion.push_back(pFrm);
    }
    else
    {
        SAFE_RELEASE( pFrm );
    }
}

void KGC3DObject::AddMotion( UINT uiMotion )
{
    char strFileName[MAX_PATH];
    if( uiMotion < 1000 )
    {
        sprintf( strFileName, "%03d.frm", (int)uiMotion );
    }
    else
    {
        sprintf( strFileName, "%d.frm", (int)uiMotion );
    }

	AddMotion( strFileName );
}

void KGC3DObject::SetMotionFromIndex( UINT val )
{
    if( 0 > val || m_vecMotion.size() <= val )
    {
        ASSERT( !"overflow" );
        return;
    }

    m_uiMotion = val;
    m_ucFrame = 0;
    m_pCurMotion = m_vecMotion[m_uiMotion];
}

void KGC3DObject::SetMotionFromMotionID( UINT val )
{
    char strFileName[MAX_PATH];
    if( val < 1000 )
    {
        sprintf( strFileName, "%03d.frm", (int)val );
    }
    else
    {
        sprintf( strFileName, "%d.frm", (int)val );
    }

    for( UINT i = 0; i < (UINT)m_vecMotion.size(); ++i )
    {
        if( 0 == m_vecMotion[i]->GetDeviceID().compare( strFileName ) )
        {
            m_uiMotion = i;
            m_ucFrame = 0;
            m_pCurMotion = m_vecMotion[i];
            return;
        }
    }

    ASSERT( !"Cannot Find Motion by ID" );
}

GCDeviceMotionFRM* KGC3DObject::GetMotionPtr() const
{
    ASSERT( NULL != m_pCurMotion );

	// 참조할려는 모션이 없다면 홀드모션으로 돌려주겠다. 아니면 크래쉬 난다....
	// 패치를 제대로 안받고 게임을 실행한 유저들은 크래쉬 날 확률이 상당히 크다
	if( m_pCurMotion == NULL )
		return g_pGCDeviceManager2->CreateMotionFRM( "1455.frm" );

    return m_pCurMotion;
}


void KGC3DObject::SetShellMatrixScale(float val)
{ 
    m_pkModel->SetShellMatrixScale( val ); 
}

void KGC3DObject::SetScale(float val)
{ 
    m_fScale = val;
    if ( m_pkModel )
        m_pkModel->SetVolume(val);
}

void KGC3DObject::SetRender( bool val )
{
    m_bRender = val;
    if( m_pkModel != NULL )
        m_pkModel->SetRender( m_bRender );
}

void KGC3DObject::SetTechnique( int iTech_ )
{ 
	if (m_pkModel) 
	{
		m_pkModel->SetTechnique(iTech_);
	}
}