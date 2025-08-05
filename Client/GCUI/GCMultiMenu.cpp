#include "stdafx.h"
#include ".\gcmultimenu.h"
//

//


//

IMPLEMENT_CLASSNAME( KGCMultiMenu );
IMPLEMENT_WND_FACTORY( KGCMultiMenu );
IMPLEMENT_WND_FACTORY_NAME( KGCMultiMenu, "gc_multimenu" );

KGCMultiMenu::KGCMultiMenu(void)
: m_iWidth( 8 )
{
	m_pSizingBox = NULL;

    m_bRenderMsgBox = false;
	
	LINK_CONTROL( "back", m_pSizingBox );
}

KGCMultiMenu::~KGCMultiMenu(void)
{
}

void KGCMultiMenu::OnCreate( void )
{
	m_pSizingBox->InitState( true, true, this);

	//#if defined (NATION_BRAZIL)
	////m_pSizingBox->SetSize(120,30);
	//m_pSizingBox->SetSize(110,70);
	//#endif
}

void KGCMultiMenu::FrameMoveInEnabledState( void )
{
}

void KGCMultiMenu::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        event.m_pWnd->SetFontColor( 0xffffff00 );
    else if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
        event.m_pWnd->SetFontColor( 0xffffffff );
    else if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
		for( int i = 0 ; i < (int)this->GetNumChild() ; ++i )
		{
			if( this->GetChild(i) == event.m_pWnd )
			{
				SpeakToActionListener( KActionEvent( this, K3DWE_MENU_CLICKED, m_mapMenuKey[i] ) );
                m_bRenderMsgBox = true;
                ToggleRender( false );
                return;
			}
		}        
    }
}

// void KGCMultiMenu::SetBadGuy( std::wstring strNickName_ )
// {
//     m_strBadGuy = strNickName_;
// }
// 
// void KGCMultiMenu::SetIndex( int iIndex_ )
// {
//     m_iIndex = iIndex_;
// }

void KGCMultiMenu::SetIsRenderMsgBox( bool bRender )
{
    m_bRenderMsgBox = bRender;
}

void KGCMultiMenu::AddMenu( int iKey, std::wstring strStatic, D3DCOLOR color /*= 0xffffffff*/, bool bActive /*= false*/, ED3DWndUIAlignType eWndAlignType )
{
	KD3DStatic* pStatic = new KD3DStatic;

	pStatic->SetSelfInputCheck( true );
	pStatic->AddActionListener( bActive?this:NULL );
	pStatic->ToggleRender( true );
	pStatic->SetFontColor( color );
	//pStatic->SetAlign( dwAlign_ );

	D3DXVECTOR2 vStaticOffSet( 0, (float)(GetNumChild()-1) * 17 + 5);

	m_mapMenuKey[GetNumChild()] = iKey;
	AddChildWithRegistToLua(pStatic);

	int iStaticWidth = g_pkFontMgr->GetWidth( strStatic.c_str() );

	PopupHeightChange();
	pStatic->SetOffset(D3DXVECTOR2(5,0));
	pStatic->SetWindowPos(vStaticOffSet);
	pStatic->SetWndState(D3DWS_ENABLED);
	pStatic->SetWidth( static_cast<DWORD>(iStaticWidth+30) );
	pStatic->SetHeight(15);
	pStatic->SetText( strStatic );	//???몃? 留?留? ????二쇱???⑸?? 洹몃??Width ????癒뱁


    if(m_iWidth < iStaticWidth)
    {
        m_iWidth = iStaticWidth;
        PopupWidthChange( m_iWidth);
    }

    pStatic->SetAlignType(eWndAlignType);
}

void KGCMultiMenu::PopupWidthChange( int iWidth )
{
	const int iFrameThick = 8;
	m_pSizingBox->SetWidth( iWidth + iFrameThick * 2 );
	SetWidthDirect( iWidth + iFrameThick * 2 );
}

void KGCMultiMenu::PopupHeightChange()
{
	const int iFrameThick = 3;
	int iHeight = ( GetNumChild() - 1) * 17;
	m_pSizingBox->SetHeight( iHeight + iFrameThick * 2 );
	SetHeightDirect( iHeight + iFrameThick * 2 );
}

void KGCMultiMenu::ClearMenu()
{
	while( (int)this->GetNumChild() > 1 )
	{
		this->DeleteChild(1);
	}
	m_iWidth = 8;
}
