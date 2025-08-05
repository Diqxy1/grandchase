#include "stdafx.h"
#include "KGCWorldMapIcon.h"

//

#include "KGCWorldMapPanel.h"
#include "KGCWorldMapContainer.h"


IMPLEMENT_CLASSNAME( KGCWorldMapIcon );
IMPLEMENT_WND_FACTORY( KGCWorldMapIcon );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapIcon, "gc_worldmap_icon" );

#define ICON_TOGGLE_FRAME       (10)
#define ICON_NAME_MAX_HEIGHT    (8.0f)

KGCWorldMapIcon::KGCWorldMapIcon( void )
: m_bRenderEffect(false)
{
    m_iObjectType = WORLDMAP_ICON;

    m_pkBaseImg     = NULL;
    m_pkActiveImg   = NULL;
    m_pkHoverImg1   = NULL;
    m_pkMissionIcon = NULL;
    //m_pkNameTag = NULL;
    m_pkOutLine = NULL;
    LINK_CONTROL( "base_img",   m_pkBaseImg );
    LINK_CONTROL( "active_img",   m_pkActiveImg );
    LINK_CONTROL( "hover_img",   m_pkHoverImg );

    LINK_CONTROL( "outline", m_pkOutLine );
    LINK_CONTROL( "hover_img1", m_pkHoverImg1 );
    LINK_CONTROL( "mission_icon", m_pkMissionIcon );
    LINK_CONTROL( "icon_name", m_pkName );

    char szName[MAX_PATH];
    for (int i = 0 ; i < EICON_MAX ; ++i)
    {
        m_pkEpicMissionIcon[i] = NULL;
        sprintf( szName, "epic_icon%d", i );
        LINK_CONTROL( szName, m_pkEpicMissionIcon[i] );
    }

    
	m_iToggleFrame = ICON_TOGGLE_FRAME;
    m_iIndex = -1;
    m_iDirection = -1;
    m_bCurrentIconActived = false;
}

KGCWorldMapIcon::~KGCWorldMapIcon(void)
{
}

void KGCWorldMapIcon::OnCreate()
{
    if( m_pkBaseImg )
        m_pkBaseImg->InitState( true );
    if(m_pkActiveImg)
        m_pkActiveImg->InitState( false );
    if(m_pkHoverImg)
        m_pkHoverImg->InitState( false );
    
    
    /*m_pkHoverImg1->InitState( false );
    m_pkHoverImg2->InitState( false );*/
    if( m_pkHoverImg1 != NULL )
        m_pkHoverImg1->InitState( false );
    if( m_pkOutLine != NULL )
        m_pkOutLine->InitState( false );
    if( m_pkMissionIcon != NULL )
        m_pkMissionIcon->InitState( false );

    if( m_pkName )
        m_pkName->InitState( true );
    /*if ( m_pkNameTag != NULL)
        m_pkNameTag->InitState(true);*/

    for (int i = 0 ; i < EICON_MAX ; ++i)
    {
        if ( m_pkEpicMissionIcon[i] )
        {
            m_pkEpicMissionIcon[i]->InitState(false);
        }
    }
	
}

void KGCWorldMapIcon::ActionPerformed( const KActionEvent& event )
{
    //GCWND_MSG_MAP( m_pkActiveImg, KD3DWnd::D3DWE_CURSOR_ENTER, OnHoverImg );
}

void KGCWorldMapIcon::OnHoverImg( bool bInBound )
{
    //던전 아이콘이 활성화 되어있는 상태에서 마우스 커서가 범위 안에 들어가면 마우스 오버 이미지를 그린다.
    if( m_pkActiveImg->IsRenderOn() && bInBound )
    {
        m_pkHoverImg->ToggleRender( true );
        m_pkName->SetWndMode(D3DWM_HOVER);
    }
    //마우스 커서가 아이콘을 찍고 있지 않으면 오버 이미지 끈다.
    else if( !bInBound )
    {
        m_pkHoverImg->ToggleRender( false );
        m_pkName->SetWndMode(D3DWM_DEFAULT);
    }
}

void KGCWorldMapIcon::CheckImages( void )
{
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    bool bCheckBound = CheckPosInWindowBound( vMousePos );
	const KSafeArray<KGCWorldMapPanel*, NUM_WORLDMAP >& worldmappanel = g_pkUIScene->m_pkWorldMapContainer->GetWolrdMapPanel();

    if( m_pkMissionIcon != NULL )
    {
        if( bCheckBound || m_pkMissionIcon->CheckPosInWindowBound( vMousePos ) )
        {
            SiKGCWorldMapManager()->ShowDungeonMissionList( m_iIndex );
        }
        else
        {
            SiKGCWorldMapManager()->HideDungeonMissionList( m_iIndex );
        }
    }
    if( m_pkActiveImg != NULL)
        OnHoverImg(bCheckBound);
    
    if( m_pkOutLine !=NULL && bCheckBound )
    {
        m_pkOutLine ->ToggleRender( true );
        m_pkHoverImg1->ToggleRender( true );
    }
    else  if(m_pkOutLine !=NULL && !bCheckBound )
    {
        m_pkOutLine ->ToggleRender( false );
        m_pkHoverImg1->ToggleRender( false );
    }
    //// 마우스가 윈도우 범위 안에 들어왔다면?
    //if( bCheckBound)
    //{        
    //    m_pkHoverImg1->ToggleRender( true );
    //    m_pkHoverImg2->ToggleRender( true );
    //}
    //else if( m_pkHoverImg1->IsRenderOn() || m_pkHoverImg2->IsRenderOn())
    //{
    //    m_pkBaseImg->ToggleRender( true );
    //    m_pkHoverImg1->ToggleRender( false );
    //    m_pkHoverImg2->ToggleRender( false );
    //}
  
}

void KGCWorldMapIcon::RenderDungeonIcon( bool bRenderDungeonIcon )
{
    if( bRenderDungeonIcon )
    {
        m_pkActiveImg->ToggleRender( true );
        m_pkBaseImg->ToggleRender( false );
        m_bCurrentIconActived = true;
    }
    else
    {
        m_pkActiveImg->ToggleRender( false );
        m_pkBaseImg->ToggleRender( true );
        m_bCurrentIconActived = false;
    }
        
}

bool KGCWorldMapIcon::GetCurrentIconActived()
{
    return m_bCurrentIconActived;
}

void KGCWorldMapIcon::FrameMoveInEnabledState( void )
{
 
     // 깜빡 깜빡 거리게 해주겠음
     if( m_pkOutLine != NULL && m_pkOutLine->IsRenderOn() )
     {
         int iOpacity = ICON_TOGGLE_FRAME;
         switch( m_iToggleFrame / ICON_TOGGLE_FRAME )
         {
         case 0: // 0 ~ ( ALERT_TOGGLE_FRAME - 1 )
             iOpacity = m_iToggleFrame;
             break;
         case 1: // ( ALERT_TOGGLE_FRAME ) ~ ( ALERT_TOGGLE_FRAME * 2 - 1 )
             iOpacity = ICON_TOGGLE_FRAME;
             break;
         case 2: // ( ALERT_TOGGLE_FRAME * 2 ) ~ ( ALERT_TOGGLE_FRAME * 3 - 1 )
             iOpacity = ICON_TOGGLE_FRAME * 3 - m_iToggleFrame;
             break;
         }
 
         BYTE alpha = static_cast<BYTE>( 255.0f * static_cast<float>( iOpacity ) / static_cast<float>( ICON_TOGGLE_FRAME ) );
 
         m_pkOutLine->SetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
     }
     m_iToggleFrame = ( m_iToggleFrame + 1 ) % ( ICON_TOGGLE_FRAME * 3 );
 
     if ( m_bRenderEffect )
     {
         m_bRenderEffect = false;
         m_pkOutLine->ToggleRender(m_bRenderEffect);
     }
}

void KGCWorldMapIcon::PreDraw( void )
{    
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

void KGCWorldMapIcon::PostChildDraw( void )
{
	g_pGCDeviceManager2->PopState();

}

void KGCWorldMapIcon::SetMissionIcon( bool bRender )
{
    if( m_pkMissionIcon == NULL )
        return;

    m_pkMissionIcon->ToggleRender( bRender );
}


void KGCWorldMapIcon::SetNameTag(const int& iDun_)
{
    //m_pkNameTag->ToggleRender(true);
    /*EGCGameMode eMode = SiKGCWorldMapManager()->GetGameMode(iDun_);
    m_pkNameTag->SetName( SiKGCWorldMapManager()->GetCurrentDungeonName(eMode) );*/
}

void KGCWorldMapIcon::EffectContinentBlink( bool bRender )
{
    m_bRenderEffect = bRender;
    m_pkOutLine->ToggleRender( m_bRenderEffect );

}

void KGCWorldMapIcon::SetEpicMissionIcon( int iconType )
{
    if( m_pkEpicMissionIcon[iconType] == NULL )
        return;

    for (int i = 0 ; i < EICON_MAX ; ++i)
    {
        if ( i == iconType )
            m_pkEpicMissionIcon[i]->ToggleRender(true);
        else
            m_pkEpicMissionIcon[i]->ToggleRender(false);
    }
}

void KGCWorldMapIcon::AllOffEpicMissionIcon()
{
    for (int i = 0 ; i < EICON_MAX ; ++i)
    {
        if ( m_pkEpicMissionIcon[i] )
        {
            m_pkEpicMissionIcon[i]->ToggleRender(false);
        }
    }
}
