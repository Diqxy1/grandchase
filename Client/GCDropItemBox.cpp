#include "stdafx.h"
#include ".\gcdropitembox.h"



KGCDropItemBox::KGCDropItemBox(void)
: m_iFrame( 0 )
, m_iFrameCount( 0 )
, m_fx ( -0.4f)
, m_fy ( 0.02f)
, m_fXscale ( 0.3f)
, m_fYscale ( 0.3f)
, m_cAlpha ( 0xff)
, m_pCurTexture( NULL )
, m_iCurTex( 0 )
, m_eState( DIB_DISABLE )
{
	for( int i = 0 ; i < DI_DROPITEM_BOX_FRAME ; ++i )
	{
		m_pTexture[i] = NULL;
	}
}

KGCDropItemBox::~KGCDropItemBox(void)
{

}

void KGCDropItemBox::Init()
{
	m_iFrame	= 0 ;
	m_iFrameCount	= 0 ;
	m_fx	= -0.4f;
	m_fy	= 0.02f;
	m_fXscale	= 0.3f;
	m_fYscale	= 0.3f;
	m_cAlpha	= 0xff;
	m_pCurTexture	= NULL ;	
	m_iCurTex	= 0 ;
	m_eState	= DIB_DISABLE ;
}

void KGCDropItemBox::Render()
{
	D3DXMATRIX    mat, mat1, mat2, mat3;

 	if( m_pCurTexture == NULL )
 	{
 		return;
 	}

	if( m_pCurTexture->GetDeviceState() == GCDS_NOT_LOADED )
		m_pCurTexture->AddRefCount();

	if( m_pCurTexture == NULL )
		return;

	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		D3DXMatrixIdentity( &mat1 );
		D3DXMatrixScaling( &mat3, m_fXscale, m_fYscale, 1.0f );
		D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
		D3DXMatrixTranslation( &mat2, m_fx - 1.0f , m_fy - 0.507f , 0.35f );    
		D3DXMatrixMultiply( &mat1, &mat1, &mat2 );				

		g_pGCDeviceManager2->DrawInWorld(m_pCurTexture,&mat1,
			1,1,0,0,1,1,0,0,0,D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ));		
	}
	g_pGCDeviceManager2->PopState();
}

void KGCDropItemBox::OpenBox()
{
	m_eState = DIB_SHOW;
}

void KGCDropItemBox::CloseBox()
{
	m_eState = DIB_CLOSING;
}

void KGCDropItemBox::FrameMove()
{
	switch( m_eState )
	{
	case DIB_SHOW:
		m_iFrame = 0;
		if( m_fx < 0.2f )
		{
			m_fx += 0.01f;
		}
		else
		{
			m_fx = 0.2f;
			m_eState = DIB_OPENING;
			return;
		}
		break;
	case DIB_OPENING:
		if( m_iFrame == DI_DROPITEM_BOX_FRAME - 1 )
		{
			m_eState = DIB_OPENED;
			m_iFrameCount = 0;
			return;
		}

		if( m_iFrameCount > 3 )
		{
			m_iFrameCount = 0; 
			++m_iFrame;
		}
		break;
	case DIB_OPENED:
		return;
	case DIB_CLOSING:
		if( m_iFrameCount > 3 )
		{
			m_iFrameCount = 0; 
			--m_iFrame;
		}
		break;
	case DIB_DISAPPEAR:
		if( m_fx > -0.4f )
		{
			m_fx -= 0.01f;
			return;
		}
		else
		{
			m_fx = -0.4f;
			m_eState = DIB_DISABLE;
			return;
		}
		break;
	case DIB_DISABLE:
		return;
	}

	if( m_iFrame >= DI_DROPITEM_BOX_FRAME )
	{
		m_iFrame = DI_DROPITEM_BOX_FRAME - 1;
		m_eState = DIB_OPENED;
		return;
	}
	
	if( m_iFrame < 0 )
	{
		m_iFrame = 0;
		m_eState = DIB_DISAPPEAR;
		return;
	}

	m_pCurTexture = m_pTexture[m_iFrame];

	++m_iFrameCount;
}



void KGCDropItemBox::LoadBoxTexture()
{
	for( int i = 0 ; i < DI_DROPITEM_BOX_FRAME ; ++i )
	{
		m_pTexture[i] = NULL;
		std::stringstream strFileName;
		strFileName<<"box 0"<<i + 1<<".dds";
		m_pTexture[i] = g_pGCDeviceManager2->CreateTexture( strFileName.str() );
	}
	m_pCurTexture = m_pTexture[0];
}