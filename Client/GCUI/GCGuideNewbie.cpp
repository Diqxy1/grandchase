#include "stdafx.h"
#include ".\gcguidenewbie.h"
//
//
//

#include "MyD3D.h"
#include "GCSayBalloon.h"
#include "GCHighLight.h"

IMPLEMENT_CLASSNAME( KGCGuideNewbie );
IMPLEMENT_WND_FACTORY( KGCGuideNewbie );
IMPLEMENT_WND_FACTORY_NAME( KGCGuideNewbie, "gc_guide_newbie" );

KGCGuideNewbie::KGCGuideNewbie(void)
: m_pkUpPoint( NULL )
, m_pkMent( NULL )
, m_fPointOffset( 0.0f )
, m_bIsUpPoint( false )
, m_bIsRight( false )
, m_bShowMouseClick( false)
, m_pkMouseClick1(NULL)
, m_pkMouseClick2(NULL)
, m_dwAnimTime( 0 )
, m_iCompleateAction(-1)
{
    m_pkKnight[ GCGN_LEFT_KNIGHT ] = NULL;
    m_pkKnight[ GCGN_RIGHT_KNIGHT] = NULL;
    m_pkHighLight = NULL;

    LINK_CONTROL( "up_sign"	, m_pkUpPoint );
    LINK_CONTROL( "down_sign", m_pkDownPoint );
    LINK_CONTROL( "left_knight", m_pkKnight[GCGN_LEFT_KNIGHT] );
    LINK_CONTROL( "right_knight", m_pkKnight[GCGN_RIGHT_KNIGHT] );
    LINK_CONTROL( "say_balloon_left", m_pkSayBalloon[GCGN_LEFT_KNIGHT] );
    LINK_CONTROL( "say_balloon_right", m_pkSayBalloon[GCGN_RIGHT_KNIGHT] );
    LINK_CONTROL( "static_ment", m_pkMent );
    LINK_CONTROL( "highlight", m_pkHighLight );
		
	LINK_CONTROL( "mouse_click1", m_pkMouseClick1);
	LINK_CONTROL( "mouse_click2", m_pkMouseClick2);
	
}

KGCGuideNewbie::~KGCGuideNewbie(void)
{
}

void KGCGuideNewbie::OnCreate( void )
{
    m_pkKnight[GCGN_RIGHT_KNIGHT]->InitState( true, true, this );
    m_pkKnight[GCGN_LEFT_KNIGHT]->InitState( true, true, this );
    m_pkSayBalloon[GCGN_LEFT_KNIGHT]->InitState( true, true, this );
    m_pkSayBalloon[GCGN_RIGHT_KNIGHT]->InitState( true, true, this );
    m_pkUpPoint->InitState( false );
    m_pkDownPoint->InitState( false );
    m_pkMent->InitState( true );
    m_pkMent->SetFontColor( 0xff000000 );
    m_pkHighLight->InitState( true, true );

	if(m_pkMouseClick1)
	{
		m_pkMouseClick1->InitState(false);
	}

	if(m_pkMouseClick2)
	{
		m_pkMouseClick2->InitState(false);
	}
}

void KGCGuideNewbie::ActionPerformed( const KActionEvent& event )
{
}

void KGCGuideNewbie::FrameMoveInEnabledState( void )
{
    
}

void KGCGuideNewbie::SetPointPos( float x, float y, bool bIsUp )
{
    m_vPointPos.x = x;
    m_vPointPos.y = y;
    m_bIsUpPoint = bIsUp;
    if( bIsUp == true )
    {
        m_pkUpPoint->SetWindowPosDirect( D3DXVECTOR2( x, y ) );
    }
    else
    {
        m_pkDownPoint->SetWindowPosDirect( D3DXVECTOR2( x, y ) );
    }
    m_pkUpPoint->ToggleRender( bIsUp );
    m_pkDownPoint->ToggleRender( !bIsUp );
}

void KGCGuideNewbie::SetDesc( const std::wstring& strDesc )
{
    m_strOriginalMent = strDesc;
    m_strMentAnim.clear();
}

void KGCGuideNewbie::SetSide( bool bIsRight )
{
	m_bIsRight = bIsRight;
	m_pkKnight[GCGN_RIGHT_KNIGHT]->ToggleRender( bIsRight );
	m_pkKnight[GCGN_LEFT_KNIGHT]->ToggleRender( !m_pkKnight[GCGN_RIGHT_KNIGHT]->IsRenderOn() );
	m_pkSayBalloon[GCGN_RIGHT_KNIGHT]->ToggleRender( bIsRight );
	m_pkSayBalloon[GCGN_LEFT_KNIGHT]->ToggleRender( !m_pkSayBalloon[GCGN_RIGHT_KNIGHT]->IsRenderOn() );
}

void KGCGuideNewbie::SetHighLightPos( float fx, float fy, DWORD dwHeight, DWORD dwWidth )
{
	m_pkHighLight->SetHighLightRegion( fx, fy, dwHeight, dwWidth );
    m_pkHighLight->ToggleRender(true );
}

void KGCGuideNewbie::SetView(bool arg_)
{
    if(arg_ == true ) return; 
    m_pkKnight[GCGN_RIGHT_KNIGHT]->ToggleRender( arg_);
    m_pkKnight[GCGN_LEFT_KNIGHT]->ToggleRender( arg_);
    m_pkSayBalloon[GCGN_LEFT_KNIGHT]->ToggleRender( arg_);
    m_pkSayBalloon[GCGN_RIGHT_KNIGHT]->ToggleRender( arg_);
    m_pkUpPoint->ToggleRender( arg_ );
    m_pkDownPoint->ToggleRender( arg_ );
    m_pkMent->ToggleRender( arg_ );
    m_pkHighLight->ToggleRender( arg_);
    m_pkHighLight->SetHighLightRegion( 0.0f, 0.0f, 0, 0 );

    g_pkInput->SetVirtualMode( false );
}


void KGCGuideNewbie::RefreshSayBalloonPos()
{
    D3DXVECTOR2 vBalloonPos;
    if( m_bIsRight == true )
    {
        vBalloonPos = m_pkSayBalloon[GCGN_RIGHT_KNIGHT]->GetFixedWindowLocalPos();
    }
    else
    {
        vBalloonPos = m_pkSayBalloon[GCGN_LEFT_KNIGHT]->GetFixedWindowLocalPos();
    }

#if defined( NATION_KOREA ) || defined( NATION_USA )
    vBalloonPos.x += 20.0f;
    vBalloonPos.y += 12.0f;
#else
	vBalloonPos.x += 10.0f;
	vBalloonPos.y += 10.0f;
#endif
    m_pkMent->SetWindowPosDirect( vBalloonPos );
}

void KGCGuideNewbie::RefreshPointPos( int iFrame )
{
    m_fPointOffset = static_cast<float>( 10.0 * sin( static_cast<double>(iFrame) / 10.0 ));
    if( m_fPointOffset * pow( float(-1), (int)m_bIsUpPoint ) > 0.0f )
        m_fPointOffset *= -1.0f;

    if( m_bIsUpPoint == true )
        m_pkUpPoint->SetWindowPosDirect( D3DXVECTOR2( m_vPointPos.x, m_vPointPos.y + m_fPointOffset) );
    else
        m_pkDownPoint->SetWindowPosDirect( D3DXVECTOR2( m_vPointPos.x, m_vPointPos.y + m_fPointOffset) );
}

bool KGCGuideNewbie::CheckGoNextGuide()
{
	

	return false;
}

bool KGCGuideNewbie::GoNextGuide()
{
    return true;	
}

void KGCGuideNewbie::RefreshMent()
{
    if( m_strOriginalMent.size() <= m_strMentAnim.size() )
        return;

    m_strMentAnim.append( m_strOriginalMent.substr( m_strMentAnim.size(), 1 ) );
    m_pkMent->SetTextAutoMultiline( m_strMentAnim );
}

void KGCGuideNewbie::ClearPaticle()
{
    for(std::vector<CParticleEventSeqPTR>::iterator mIter = m_vecParticle.begin();mIter != m_vecParticle.end();mIter++)
    {
        g_ParticleManager->DeleteSequence( (*mIter) );
    }
    m_vecParticle.clear();
}

void KGCGuideNewbie::SetParticle(std::vector<SGuidePaticleInfo>& vecParticle)
{
    ClearPaticle();
    for(std::vector<SGuidePaticleInfo>::iterator vIt = vecParticle.begin();vIt != vecParticle.end();vIt++)
    {
        CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( vIt->strPaticleName.c_str(), vIt->x, vIt->y, 0.5f );
        m_vecParticle.push_back(pParticle);
        g_ParticleManager->SetLayer( pParticle, vIt->iLayer );

    }
}

void KGCGuideNewbie::SetMouseClickPos(D3DXVECTOR2 vPos_)
{
	if(m_pkMouseClick1)
		m_pkMouseClick1->SetWindowPosDirect(vPos_);
	
	if(m_pkMouseClick2)
		m_pkMouseClick2->SetWindowPosDirect(vPos_);
}

void KGCGuideNewbie::ToggleRender( bool bRender )
{
    KD3DWnd::ToggleRender(bRender);

    if(!bRender)
    {
        g_pkUIScene->SetChildWndUpdateLock( false );
        g_pkInput->SetVirtualMode( false );
        m_iCompleateAction = -1;
        ClearPaticle();
    }
}