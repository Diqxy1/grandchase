#include "stdafx.h"
#include ".\gcsquare2dobj.h"

#include "GCCameraOrtho.h"
#include "GCBuffDonationManager.h"
//




KGCSquare2DObj::KGCSquare2DObj(void)
: m_iCurrTexIndex( 0 )
, m_cAlpha( 0xff )
, m_fWidth( 0.0f )
, m_fHeight( 0.0f )
, m_fXrotation ( 0.0f)
, m_fYrotation ( 0.0f)
, m_fZrotation ( 0.0f)
, m_fXaxisOffset( 0.0f )
, m_fYaxisOffset( 0.0f )
, m_fZaxisOffset( 0.0f )
, m_fXscale ( 1.0f)
, m_fYscale ( 1.0f)
, m_fXtrans ( 0.0f)
, m_fYtrans ( 0.0f)
, m_iPicked( -1 )
, m_bMoveWithCamera( false )
, m_dwGapRandTime( 0 )
, m_dwBeforeTime( 0 )
, m_dwNowTime( 0 )
{
    m_eType = SOT_NEWSBOARD;
}

KGCSquare2DObj::~KGCSquare2DObj(void)
{
}

void KGCSquare2DObj::Render()
{
    // 화령 축제 오브젝트
    if ( m_eType == SOT_BUFF_BEAD_ACTIVE && SiGCBuffDonationManager()->IsBuffOn() == false )
        return;

	D3DXMATRIX    mat, mat1, mat2, mat3;

	if( m_vecTex.empty() )
		return;

	if( m_iCurrTexIndex < 0 || m_iCurrTexIndex >= (int)m_vecTex.size() )
	{
		ASSERT( !"Texture Index is wrong!!!" );
		return;
	}

	if( m_vecTex[m_iCurrTexIndex] == NULL )
	{
		ASSERT( !"Texture is NULL!!" );
		return;
	}

	if( m_vecTex[m_iCurrTexIndex]->GetDeviceState() == GCDS_NOT_LOADED )
		m_vecTex[m_iCurrTexIndex]->AddRefCount();

	if( m_vecTex[m_iCurrTexIndex] == NULL )
	{
		ASSERT( !"Wrong Texture in vector!!!" );
		return;
	}

	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


		SLVERTEX v[4];
		v[0].SetSLVER( 0,     1.0f,     0,       0,      0,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[1].SetSLVER(  1.0f,    1.0f,     0,    1.0f,      0,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[2].SetSLVER( 0,    0,      0,       0,    1.0f,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[3].SetSLVER(  1.0f,   0,      0,    1.0f,    1.0f,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );

        // UV 값과 아래 스케일 값을 조절해서 게이지 표현
        if( m_eType == SOT_BUFF_GUAGE ) {
            float fV = SiGCBuffDonationManager()->GetGuageCoordinateV();
            v[0].SetSLVER( 0,     1.0f,     0,       0,      fV,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
            v[1].SetSLVER(  1.0f,    1.0f,     0,    1.0f,      fV,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
        }

        D3DXMatrixIdentity( &mat1 );
        D3DXMatrixTranslation( &mat2, m_fXaxisOffset, m_fYaxisOffset, m_fZaxisOffset );    
        D3DXMatrixMultiply( &mat1, &mat1, &mat2 );
        
        if( m_eType == SOT_BUFF_GUAGE ) {
            float fYscale = m_fYscale * SiGCBuffDonationManager()->GetGuageScale();
            D3DXMatrixScaling( &mat3, m_fXscale, fYscale, 1.0f );
        } else {
            D3DXMatrixScaling( &mat3, m_fXscale, m_fYscale, 1.0f );
        }

        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
        D3DXMatrixRotationYawPitchRoll( &mat3, m_fYrotation, m_fXrotation, m_fZrotation );
        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
        D3DXMatrixTranslation( &mat2, m_BasicData.vPos.x - 1.0f + m_fXtrans, m_BasicData.vPos.y - 0.507f + m_fYtrans , 0.35f );    
        D3DXMatrixMultiply( &mat1, &mat1, &mat2 );
        if( m_bMoveWithCamera == false )
            D3DXMatrixMultiply( &mat1, &mat1, &g_kCamera.m_matCamera );

        g_pGCDeviceManager2->DrawInWorld(m_vecTex[m_iCurrTexIndex],&mat1,v);
        
	}
	g_pGCDeviceManager2->PopState();
}

int KGCSquare2DObj::AddTexture( const char* strID )
{
	m_vecTex.push_back( g_pGCDeviceManager2->CreateTexture( strID ) );
	return (int)m_vecTex.size() - 1;
}

void KGCSquare2DObj::SelectRandomTexture()
{
    m_dwNowTime = ::timeGetTime();
    if( (m_dwNowTime-m_dwBeforeTime) > m_dwGapRandTime )
    {
        int iTemp, iSize = m_vecTex.size();
        while( iSize > 1 )
        {
            if( m_iCurrTexIndex != (iTemp = rand()%iSize) )
            {
                m_iCurrTexIndex = iTemp;
                break;
            }
        }
        m_dwBeforeTime = m_dwNowTime;
    }
}

void KGCSquare2DObj::SetCurrTexIndex( int iIndex )
{
	if( m_vecTex.empty() )
		return;

	if( iIndex < 0 || iIndex >= (int) m_vecTex.size() )
		return;

	m_iCurrTexIndex = iIndex;
}

int KGCSquare2DObj::GetCurrTexIndex()
{
	return m_iCurrTexIndex;
}

void KGCSquare2DObj::LoadObject( std::string strObjName )
{

}

void KGCSquare2DObj::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCSquare2DObj>( L, "KGCSquare2DObj" );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "ObjType", 		&KGCSquare2DObj::strObjType  );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "iCurrTexIndex", 	&KGCSquare2DObj::m_iCurrTexIndex );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "cAlpha", 		&KGCSquare2DObj::m_cAlpha );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Picked", 		&KGCSquare2DObj::m_iPicked );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Width", 			&KGCSquare2DObj::m_fWidth );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Height", 		&KGCSquare2DObj::m_fHeight );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Yrotation", 		&KGCSquare2DObj::m_fYrotation );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Xrotation", 		&KGCSquare2DObj::m_fXrotation );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Zrotation", 		&KGCSquare2DObj::m_fZrotation );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Xaxis", 			&KGCSquare2DObj::m_fXaxisOffset );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Yaxis", 			&KGCSquare2DObj::m_fYaxisOffset );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Zaxis", 			&KGCSquare2DObj::m_fZaxisOffset );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Xscale", 		&KGCSquare2DObj::m_fXscale );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Yscale", 		&KGCSquare2DObj::m_fYscale );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Xtrans", 		&KGCSquare2DObj::m_fXtrans );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "Ytrans", 		&KGCSquare2DObj::m_fYtrans );
    lua_tinker::class_mem<KGCSquare2DObj>( L, "GapRandTime",    &KGCSquare2DObj::m_dwGapRandTime );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetType", 			    &KGCSquare2DObj::SetType );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetScale", 			    &KGCSquare2DObj::SetScale );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetRotation", 			&KGCSquare2DObj::SetRotation );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetTrans", 			    &KGCSquare2DObj::SetTrans );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetPosition", 			&KGCSquare2DObj::SetPosition );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetSize", 				&KGCSquare2DObj::SetSize );
    lua_tinker::class_def<KGCSquare2DObj>( L, "CameraMatrixOn", 		&KGCSquare2DObj::CameraMatrixOn  );
    lua_tinker::class_def<KGCSquare2DObj>( L, "CameraMatrixOff", 		&KGCSquare2DObj::CameraMatrixOff  );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetAlpha", 			    &KGCSquare2DObj::SetAlpha  );
    lua_tinker::class_def<KGCSquare2DObj>( L, "GetPosition", 			&KGCSquare2DObj::GetPosition  );
    lua_tinker::class_def<KGCSquare2DObj>( L, "AddTexture", 			&KGCSquare2DObj::AddTexture );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SetRandomTime", 	        &KGCSquare2DObj::SetRandomTime );
    lua_tinker::class_def<KGCSquare2DObj>( L, "SelectRandomTexture", 	&KGCSquare2DObj::SelectRandomTexture );
}

void KGCSquare2DObj::SetPosition( float x, float y )
{
	m_BasicData.vPos.x = x;
	m_BasicData.vPos.y = y;
}

bool KGCSquare2DObj::Load()
{
	return true;
}

bool KGCSquare2DObj::TimeShareLoading()
{
	return true;
}

void KGCSquare2DObj::SetType( SQUARE_OBJ_TYPE eType )
{
    m_eType = eType;
}

void KGCSquare2DObj::SetScale( float x, float y )
{
	m_fXscale = x;
	m_fYscale = y;
}

void KGCSquare2DObj::SetRotation( float x, float y, float z )
{
	m_fXrotation = x;
	m_fYrotation = y;
	m_fZrotation = z;
}

void KGCSquare2DObj::SetTrans( float x, float y )
{
	m_fXtrans = x;
	m_fYtrans = y;
}

bool KGCSquare2DObj::Picking( const D3DXVECTOR2& vMousePos, const int iMouseBtn )
{
	if( vMousePos.x >= m_BasicData.vPos.x && vMousePos.x <= m_BasicData.vPos.x + m_fWidth )
	{
		if( vMousePos.y >= m_BasicData.vPos.y && vMousePos.y <= m_BasicData.vPos.y + m_fHeight )
		{
			m_iPicked = iMouseBtn;
			return true;
		}
	}
	return false;
}

void KGCSquare2DObj::FrameMove()
{
	m_iPicked = -1;
}

void KGCSquare2DObj::SetSize( float fWidth, float fHeight )
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

void KGCSquare2DObj::SetAlpha( int iAlpha )
{
	m_cAlpha = iAlpha;
}

D3DXVECTOR3* KGCSquare2DObj::GetPosition()
{
	return &m_BasicData.vPos;
}

void KGCSquare2DObj::SetRandomTime(int iTime)
{
    m_dwGapRandTime = iTime;
}