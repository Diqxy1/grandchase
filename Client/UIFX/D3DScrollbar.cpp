
#include "D3DScrollbar.h"
//
//

IMPLEMENT_CLASSNAME( KD3DScrollbar );

KD3DScrollbar::KD3DScrollbar( void )
{
    m_iPos              = 0;
    m_iGap              = 1;
    m_iMin              = 0;
    m_iMax              = 100;
    m_uiPageSize        = 100;
    m_bThumbDrag        = false;
    m_fSavePosY         = 0.0f;
    m_iPressCount       = 0;
    m_iXGab             = 0;
    m_iYGab             = 0;
    m_iUpAndDown        = 0;
    m_bStaticThumb      = false;

    m_pkBtnUp           = NULL;
    m_pkBtnDown         = NULL;
    m_pkBtnThumbTop     = NULL;
    m_pkBtnThumb        = NULL;
    m_pkBtnThumbBottom  = NULL;
    m_pkTargetWnd       = NULL;

    LINK_CONTROL( "UpBtn",          m_pkBtnUp );
    LINK_CONTROL( "DownBtn",        m_pkBtnDown );
    LINK_CONTROL( "ThumbBtnTop",    m_pkBtnThumbTop );
    LINK_CONTROL( "ThumbBtn",       m_pkBtnThumb );
    LINK_CONTROL( "ThumbBtnBottom", m_pkBtnThumbBottom );

	m_mapEvent["UpBtn"]             = &KD3DScrollbar::OnBtnUp;
    m_mapEvent["DownBtn"]           = &KD3DScrollbar::OnBtnDown;
    m_mapEvent["ThumbBtnTop"]       = &KD3DScrollbar::OnBtnThumb;
    m_mapEvent["ThumbBtn"]          = &KD3DScrollbar::OnBtnThumb;
    m_mapEvent["ThumbBtnBottom"]    = &KD3DScrollbar::OnBtnThumb;
}

KD3DScrollbar::~KD3DScrollbar( void )
{
    m_mapEvent.clear();
}

void KD3DScrollbar::SetScrollPos( int iPos )
{
    m_iPos = iPos;

    if ( m_iPos < m_iMin ) m_iPos = m_iMin;
    if ( m_iPos > m_iMax ) m_iPos = m_iMax;

    if ( m_iMin == m_iMax )
    {
        D3DXVECTOR2 vThumbPos( (float)m_iXGab, (float)(GetWidth() + m_iYGab) );
        m_pkBtnThumbTop->SetWindowPos( vThumbPos );
        vThumbPos.y += m_pkBtnThumbTop->GetHeight();
        m_pkBtnThumb->SetWindowPos( vThumbPos );
        vThumbPos.y += m_pkBtnThumb->GetHeight();
        m_pkBtnThumbBottom->SetWindowPos( vThumbPos );
    }
    else
    {
        float fPos = ( CalcShaftSize() - CalcThumbSize( true ) )
                   * ( float( m_iPos ) / float( m_iMax - m_iMin ) );
        D3DXVECTOR2 vThumbPos( (float)m_iXGab, (float)((int)(GetWidth() + fPos + m_iYGab)) );
        m_pkBtnThumbTop->SetWindowPos( vThumbPos );
        vThumbPos.y += m_pkBtnThumbTop->GetHeight();
        m_pkBtnThumb->SetWindowPos( vThumbPos );
        vThumbPos.y += m_pkBtnThumb->GetHeight();
        m_pkBtnThumbBottom->SetWindowPos( vThumbPos );
    }
}

void KD3DScrollbar::SetScrollGap( int iGap )
{
    m_iGap = iGap;
}

// 이자식 유령함수의 냄새가 풍김..-_-; 수치 넣어서 쓰면 문제 터질 확률 200%인듯;
// 최소치 개념자체가 적용안되어 있음..  항상 0으로.. 사용되어 왔음..
// 만약에 사용하면 꽃되는 현상 발생 우려됨... 쓰실분 알아서 고쳐 쓰삼...

void KD3DScrollbar::SetScrollRangeMin( int iMin )
{
    int iOldMin;
    m_iMin = iMin;
    iOldMin=m_iMin;
    if ( m_iMin > m_iMax ) m_iMin = m_iMax;
    if( false == m_bStaticThumb )
        m_pkBtnThumb->SetHeight( CalcThumbSize() );
    if ( m_iPos < m_iMin ) m_iPos = m_iMax;
    SetScrollPos( m_iPos );
}

void KD3DScrollbar::SetScrollRangeMax( int iMax )
{
    int iOldMax;
    iOldMax=m_iMax;
    m_iMax = iMax - m_uiPageSize;    
    if ( m_iMax < m_iMin ) m_iMax = m_iMin;
    if( false == m_bStaticThumb )
        m_pkBtnThumb->SetHeight( CalcThumbSize() );
    m_iPos = (int)(static_cast<double>(m_iPos)*m_iMax/static_cast<double>(iOldMax));//비율은 그대로 유지하도록 설정
    if ( m_iPos > m_iMax ) m_iPos = m_iMax;    
    SetScrollPos( m_iPos );
}


void KD3DScrollbar::SetScrollPageSize( UINT uiPageSize )
{
    m_uiPageSize = uiPageSize;
    if( false == m_bStaticThumb )
        m_pkBtnThumb->SetHeight( CalcThumbSize() );
    SetScrollPos( m_iPos );
}

void KD3DScrollbar::ActionPerformed( const KActionEvent& event )
{
    if( this->IsLocked() )
        return;

    (this->*m_mapEvent[event.m_pWnd->GetWindowName()])( event.m_dwCode );
}

void KD3DScrollbar::OnBtnUp( DWORD dwCode )
{
    if ( KD3DWnd::D3DWE_BUTTON_DOWN == dwCode )
    {
        m_iPressCount = 0;
        SetScrollPos( m_iPos - m_iGap );
        SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_DOWNBTN) );
    }

    if ( KD3DWnd::D3DWE_BUTTON_PRESS == dwCode  )
    {
        if ( ++m_iPressCount > 10 )
        {
			m_iPressCount = 3;
            SetScrollPos( m_iPos - m_iGap );
            SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_UPBTN ) );
        }
    }
}

void KD3DScrollbar::OnBtnDown( DWORD dwCode )
{
    if ( KD3DWnd::D3DWE_BUTTON_DOWN == dwCode )
    {
        m_iPressCount = 0;
        SetScrollPos( m_iPos + m_iGap );
        SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_DOWNBTN ) );
    }

    if ( KD3DWnd::D3DWE_BUTTON_PRESS == dwCode  )
    {
        if ( ++m_iPressCount > 10 )
        {
			m_iPressCount = 3;
            SetScrollPos( m_iPos + m_iGap );
            SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_DOWNBTN ) );
        }
    }
}

void KD3DScrollbar::OnBtnThumb( DWORD dwCode )
{
//    switch ( dwCode )
//    {
//        case KD3DWnd::D3DWE_BUTTON_DOWN:
//        {
//            m_bThumbDrag = true;
//            m_fSavePosY = g_pkInput->GetMousePos().y - m_pkBtnThumb->GetCurrentWindowPos().y;
//            break;
//        }
//        case KD3DWnd::D3DWE_BUTTON_UP:
//        {
//            m_bThumbDrag = false;
//            break;
//        }
//    }
}

void KD3DScrollbar::OnCreate( void )
{
    ASSERT( m_pkBtnUp           != NULL );
    ASSERT( m_pkBtnDown         != NULL );
    ASSERT( m_pkBtnThumbTop     != NULL );
    ASSERT( m_pkBtnThumb        != NULL );
    ASSERT( m_pkBtnThumbBottom  != NULL );

    m_pkBtnUp->SetSelfInputCheck( true );
    m_pkBtnDown->SetSelfInputCheck( true );
//    m_pkBtnThumbTop->SetSelfInputCheck( true );
//    m_pkBtnThumb->SetSelfInputCheck( true );
//    m_pkBtnThumbBottom->SetSelfInputCheck( true );
    m_pkBtnThumbTop->SetSelfInputCheck( false );
    m_pkBtnThumb->SetSelfInputCheck( false );
    m_pkBtnThumbBottom->SetSelfInputCheck( false );

    m_pkBtnUp->AddActionListener( this );
    m_pkBtnDown->AddActionListener( this );
//    m_pkBtnThumbTop->AddActionListener( this );
//    m_pkBtnThumb->AddActionListener( this );
//    m_pkBtnThumbBottom->AddActionListener( this );

    DWORD dwWidth   = GetWidth();
    DWORD dwHeight  = GetHeight();

    m_pkBtnDown->SetAllWindowPos( D3DXVECTOR2( m_pkBtnDown->GetFixedWindowLocalPos().x,
                                               (float)(dwHeight - dwWidth) ) );
    if( false == m_bStaticThumb )
    {
        m_pkBtnThumb->SetWidth( dwWidth );
        m_pkBtnThumb->SetHeight( CalcThumbSize() );

        m_pkBtnThumbTop->SetWidth( dwWidth );
        m_pkBtnThumbBottom->SetWidth( dwWidth );
    }
    SetScrollPos( 0 );
}

void KD3DScrollbar::FrameMoveInEnabledState( void )
{
    if(this->IsLocked() )
        return;

    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

//    if ( 0 != g_pkInput->GetZMov() && GetTargetWnd()->CheckPosInWindowBound( vMousePos ) ) // mouse wheel
//    {
//        float fPos = m_iPos - 0.05f * g_pkInput->GetZMov();
//        SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );
//        SpeakToActionListener( KActionEvent( this ) );
//    }
//    else if ( CheckPosInWindowBound( vMousePos ) )
//    {
//        if ( m_bThumbDrag && g_pkInput->BtnPressed( KInput::MBLEFT ) ) // drag thumb button
//        {
//            D3DXVECTOR2 vWndPos = GetCurrentWindowPos();
//            float fMin = vWndPos.y + GetWidth();
//            float fMax = vWndPos.y + GetHeight() - GetWidth() - m_pkBtnThumb->GetHeight();
//            float fPos = ( ( vMousePos.y - m_fSavePosY - fMin ) / ( fMax - fMin ) ) * ( m_iMax - m_iMin );
//            SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );
//            SpeakToActionListener( KActionEvent( this ) );
//        }
//        else if ( !m_pkBtnUp->CheckPosInWindowBound( vMousePos ) &&
//                  !m_pkBtnDown->CheckPosInWindowBound( vMousePos ) &&
//                  !m_pkBtnThumb->CheckPosInWindowBound( vMousePos ) &&
//                  g_pkInput->BtnDown( KInput::MBLEFT ) ) // click shaft
//        {
//            D3DXVECTOR2 vWndPos = GetCurrentWindowPos();
//            float fThumbHeight = m_pkBtnThumb->GetHeight();
//            float fMin = vWndPos.y + GetWidth();
//            float fMax = vWndPos.y + GetHeight() - GetWidth() - fThumbHeight;
//            float fPos = ( ( vMousePos.y - fMin - fThumbHeight / 2 ) / ( fMax - fMin ) ) * ( m_iMax - m_iMin );
//            SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );
//            SpeakToActionListener( KActionEvent( this ) );
//        }
//    }
    if ( 0 != g_pkInput->GetZMov() && GetTargetWnd()->CheckPosInWindowBound( vMousePos ) ) // mouse wheel
    {
        //float fPos = m_iPos - 0.05f * g_pkInput->GetZMov();
		if( g_pkInput->GetZMov() > 0 )
		{
            m_iUpAndDown = 1;
			SetScrollPos( --m_iPos );
			SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_WHEELUP ) );
		}
		else 
		{
            m_iUpAndDown = 2;
			SetScrollPos( ++m_iPos );
			SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_WHEELDOWN ) );			
		}

        //SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );		
    }
    else if ( CheckPosInWindowBound( vMousePos ) )
    {
        if ( m_pkBtnThumb->CheckPosInWindowBound( vMousePos ) )
        {
            if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
            {
                m_bThumbDrag = true;
				m_fSavePosY = g_pkInput->GetMousePos().y / m_fWindowScaleY - m_pkBtnThumb->GetCurrentWindowPos().y;
            }
            else if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
            {
                m_bThumbDrag = false;
            }
        }
        else if ( !m_pkBtnUp->CheckPosInWindowBound( vMousePos ) &&
                  !m_pkBtnDown->CheckPosInWindowBound( vMousePos ) &&
                  //!m_pkBtnThumbTop->CheckPosInWindowBound( vMousePos ) &&
                  //!m_pkBtnThumb->CheckPosInWindowBound( vMousePos ) &&
                  //!m_pkBtnThumbBottom->CheckPosInWindowBound( vMousePos ) &&
                  g_pkInput->BtnDown( KInput::MBLEFT ) ) // click shaft
        {
            m_bThumbDrag = true;
            D3DXVECTOR2 vWndPos = GetCurrentWindowPos();
            float fThumbHeight = (float)m_pkBtnThumb->GetHeight();
            float fMin = vWndPos.y + GetWidth();
            float fMax = vWndPos.y + GetHeight() - GetWidth() - fThumbHeight;
			float fPos = ( ( vMousePos.y / m_fWindowScaleY - fMin - fThumbHeight / 2 ) / ( fMax - fMin ) ) * ( m_iMax - m_iMin );
            SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );
            SpeakToActionListener( KActionEvent( this ) );
        }
    }
    else
    {
        if ( g_pkInput->BtnDown( KInput::MBLEFT ) ) m_bThumbDrag = false;
    }

    // drag thumb button
    if ( m_bThumbDrag && g_pkInput->IsMouseMoving() && g_pkInput->BtnPressed( KInput::MBLEFT ) )
    {
        D3DXVECTOR2 vWndPos = GetCurrentWindowPos();
        float fMin = vWndPos.y + GetWidth();
        float fMax = vWndPos.y + GetHeight() - GetWidth() - m_pkBtnThumb->GetHeight();
		float fPos = ( ( vMousePos.y / m_fWindowScaleY - m_fSavePosY - fMin ) / ( fMax - fMin ) ) * ( m_iMax - m_iMin );
        SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );
        SpeakToActionListener( KActionEvent( this, D3DWE_SCROLLBAR_DRAG ) );
    }
}

DWORD KD3DScrollbar::CalcThumbSize( bool bEntire /* = false */ )
{
    DWORD dwThumbMinSize = m_pkBtnThumbTop->GetHeight() + m_pkBtnThumbBottom->GetHeight();
    DWORD dwThumbMaxSize = CalcShaftSize();
    DWORD dwThumbSize;
    
    if( m_bStaticThumb )
    {
        dwThumbSize = (m_pkBtnThumbTop->GetHeight() + m_pkBtnThumb->GetHeight() + m_pkBtnThumbBottom->GetHeight());
    }
    else
    {
        if ( m_iMin == m_iMax )
        {
            dwThumbSize = dwThumbMaxSize;
        }
        else
        {
            dwThumbSize = DWORD( dwThumbMaxSize *
                ( float( m_uiPageSize ) / float( m_iMax + m_uiPageSize - m_iMin ) ) );
            if ( dwThumbSize < dwThumbMinSize ) dwThumbSize = dwThumbMinSize;
            if ( dwThumbSize > dwThumbMaxSize ) dwThumbSize = dwThumbMaxSize;
        }
        dwThumbSize = ( bEntire ? dwThumbSize : ( dwThumbSize - dwThumbMinSize ) );
    }

    return dwThumbSize;
}

DWORD KD3DScrollbar::CalcShaftSize( void )
{
    return GetHeight() - 2 * GetWidth();
}

void KD3DScrollbar::SetThumbXGab( int iGab )
{
    m_pkBtnThumbTop->SetWidth(GetWidth()-iGab*2);
    m_pkBtnThumb->SetWidth(GetWidth()-iGab*2);
    m_pkBtnThumbBottom->SetWidth(GetWidth()-iGab*2);

    m_iXGab = iGab;
}

void KD3DScrollbar::SetThumbYGab( int iGab )
{
    m_iYGab = iGab;
    m_pkBtnDown->SetFixedWindowLocalPos( D3DXVECTOR2( m_pkBtnDown->GetFixedWindowLocalPos().x,
                                (float)(GetHeight() - GetWidth() + m_iYGab*2) ) );
}

void KD3DScrollbar::ResizeBtn()
{
    m_pkBtnDown->SetAllWindowPos( D3DXVECTOR2( m_pkBtnDown->GetFixedWindowLocalPos().x,
        (float)(GetHeight() - GetWidth()) ) );
    m_pkBtnDown->UpdateWndMode();
}

void KD3DScrollbar::OnSetHeight( DWORD dwHeight )
{
	KD3DWnd::SetHeightDirect( dwHeight );

	m_pkBtnDown->SetFixedWindowLocalPos( D3DXVECTOR2( m_pkBtnDown->GetFixedWindowLocalPos().x, (float)(dwHeight - m_pkBtnDown->GetHeight()) ) );
}
