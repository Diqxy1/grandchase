#include "stdafx.h"
#include "KGCEmoticonElement.h"

//
//
//
#include "KGCEmoticon.h"


IMPLEMENT_CLASSNAME( KGCEmoticonElement );
IMPLEMENT_WND_FACTORY( KGCEmoticonElement );
IMPLEMENT_WND_FACTORY_NAME( KGCEmoticonElement, "gc_emoticon_element" );


KGCEmoticonElement::KGCEmoticonElement( void )
: m_pkNumber10( NULL )
, m_pkNumber1( NULL )
, m_pkCenter( NULL )
, m_pkEdge( NULL )
, m_bIsClick( false )
, m_bIsExist( true )
, m_iItemID( 0 )
{
    LINK_CONTROL( "level1",     m_pkNumber1 );
    LINK_CONTROL( "level10",    m_pkNumber10 );
    LINK_CONTROL( "center",     m_pkCenter );
    LINK_CONTROL( "clicked",    m_pkEdge );
}

KGCEmoticonElement::~KGCEmoticonElement( void )
{
}

void KGCEmoticonElement::OnCreate( void )
{
    m_pkNumber10->InitState( true );
    m_pkNumber1->InitState( true );

    InitNumberTextureCoordinate();

    m_pkCenter->InitState( true );
    m_pkEdge->InitState( false );
    m_pkCenter->SetPredraw( true );
}

void KGCEmoticonElement::FrameMoveInEnabledState( void )
{
    if ( m_bIsClick )
        m_pkEdge->ToggleRender( true );
    else
        m_pkEdge->ToggleRender( false );
}

void KGCEmoticonElement::InitNumberTextureCoordinate()
{
    m_pkNumber1->SetNumberCoordi( 0, D3DXVECTOR4( 432.0f, 85.0f, 451.0f, 106.0f ) );
    m_pkNumber1->SetNumberCoordi( 1, D3DXVECTOR4( 452.0f, 85.0f, 471.0f, 106.0f ) );
    m_pkNumber1->SetNumberCoordi( 2, D3DXVECTOR4( 471.0f, 85.0f, 490.0f, 106.0f ) );
    m_pkNumber1->SetNumberCoordi( 3, D3DXVECTOR4( 491.0f, 85.0f, 510.0f, 106.0f ) );
    m_pkNumber1->SetNumberCoordi( 4, D3DXVECTOR4( 432.0f, 110.0f, 451.0f, 131.0f ) );
    m_pkNumber1->SetNumberCoordi( 5, D3DXVECTOR4( 452.0f, 110.0f, 471.0f, 131.0f ) );
    m_pkNumber1->SetNumberCoordi( 6, D3DXVECTOR4( 471.0f, 110.0f, 490.0f, 131.0f ) );
    m_pkNumber1->SetNumberCoordi( 7, D3DXVECTOR4( 491.0f, 110.0f, 510.0f, 131.0f ) );
    m_pkNumber1->SetNumberCoordi( 8, D3DXVECTOR4( 432.0f, 135.0f, 451.0f, 156.0f ) );
    m_pkNumber1->SetNumberCoordi( 9, D3DXVECTOR4( 452.0f, 135.0f, 471.0f, 156.0f ) );

    m_pkNumber10->SetTexCoordi( m_pkNumber1->GetTexCoordi() );
}

void KGCEmoticonElement::SetVolumn( UINT nVolume )
{
    m_nVolume = nVolume;

    if( nVolume > 99 )
    {
        nVolume = m_nVolume = 99;
    }
    JIF( m_pkNumber10 != NULL );
    JIF( m_pkNumber1 != NULL );

    if ( nVolume == 0 )
    {
        m_pkNumber10->ToggleRender( false );
        m_pkNumber1->ToggleRender( false );
        m_pkEdge->ToggleRender( false );
    }
    else if( nVolume < 10 )
    {
        m_pkNumber10->SetNumber( 0 );
        m_pkNumber10->ToggleRender( false );
        m_pkNumber1->SetNumber( nVolume );
        m_pkNumber1->ToggleRender( true );
    }
    else
    {
        UINT n10 = nVolume / 10;
        UINT n1 = nVolume - ( n10 * 10 );
        m_pkNumber10->SetNumber( n10 );
        m_pkNumber10->ToggleRender( true );
        m_pkNumber1->SetNumber( n1 );
        m_pkNumber1->ToggleRender( true );
    }
}

void KGCEmoticonElement::PostDraw( void )
{

    m_pkEmoticonData = SiKGCEmoticonManager()->GetEmoticonData( m_iItemID );
    if ( NULL == m_pkEmoticonData)
        return;	

    // 파쿠쟈!
    // sbta 들어오면 이짓 필요 없다. uv 좌표 0 0 1 1로 띄우쟈!
    D3DXVECTOR4 vUV;
    vUV.x = m_pkEmoticonData->m_safTextureCoordList[0][0];
    vUV.y = m_pkEmoticonData->m_safTextureCoordList[0][1];
    vUV.z = m_pkEmoticonData->m_safTextureCoordList[0][2];
    vUV.w = m_pkEmoticonData->m_safTextureCoordList[0][3];

    if ( !m_pkEmoticonData->m_pTexture || !m_pkEmoticonData->m_pTexture->GetDeviceTexture() )
        return;

    /*g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    {*/
        const KSafeArray<KD3DUIVertex,4>& pEmoticonVertex = m_pkCenter->GetReLocatedWindowRect();
        if ( 0 < m_nVolume )
        {
			g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData->m_pTexture,
				pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
				vUV.x, vUV.y, vUV.z, vUV.w, false, 10.0f,0xffffffff);
            /*m_pkEmoticonData->m_pTexture->SetDeviceTexture();
            g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
                vUV.x, vUV.y, vUV.z, vUV.w, false, 10.0f, 255 );*/
        }
        else
        {
            if ( false != m_bIsExist )
            {
				g_pGCDeviceManager2->DrawInScreen(m_pkEmoticonData->m_pTexture,
					pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
					vUV.x, vUV.y, vUV.z, vUV.w, false, 10.0f,D3DCOLOR_ARGB(255,128,128,128));
         /*       m_pkEmoticonData->m_pTexture->SetDeviceTexture();
                g_MyD3D->Draw_TL( pEmoticonVertex[0].m_vPos.x, pEmoticonVertex[0].m_vPos.y, pEmoticonVertex[3].m_vPos.x, pEmoticonVertex[3].m_vPos.y,
                    vUV.x, vUV.y, vUV.z, vUV.w, false, 10.0f, 255, 128, 128, 128 );*/
            }
        }
    /*}
    KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
    KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/
}

void KGCEmoticonElement::SetItemID( int iItemID )
{
    m_iItemID = iItemID;
}

bool KGCEmoticonElement::IsExistEmoticon( void )
{
    if ( m_nVolume > 0)
        return true;
    else
        return false;
}

KGCEmoticon* KGCEmoticonElement::GetEmoticonData( void )
{
    return m_pkEmoticonData;
}

void KGCEmoticonElement::OnClose( void )
{
    m_bIsClick = false;
}

void KGCEmoticonElement::OnDestroyComplete( void )
{
    OnClose();
}