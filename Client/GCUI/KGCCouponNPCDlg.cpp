#include "stdafx.h"
#include "P2P.h"
#include "../uifx/KInput.h"
#include "GCDescBox.h"
#include "GCSizingThreeBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "KGCCouponNPCDlg.h"
#include "./gcui/GCItemInfoBox.h"
#include "KGCCouponNPCEventWnd.h"
#include "KDSound.h"

IMPLEMENT_CLASSNAME( KGCCouponNPCDlg );
IMPLEMENT_WND_FACTORY( KGCCouponNPCDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCouponNPCDlg, "gc_coupon_npc_dlg" );

KGCCouponNPCDlg::KGCCouponNPCDlg(void)
: m_iCheckSelect(-1)
, m_iCurPage(1)
, m_iSelectEventUID(-1)
, m_bClickedInputBtn(false)
, m_iCountInput(0)
{
	m_vecCouponInfo.clear();

	for( int i = 0 ; i < MAX_COUPON_WND ; i++ )
	{
		m_pkEventWnd[i] = NULL;
		std::stringstream str;
		str<<"coupon_event_contents"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkEventWnd[i] );

		m_iCurPageEventUID[i] = -1;
	}

	m_pkDisableInput	= NULL;
	m_pkEnableInput		= NULL;
#if defined(COUPON_TWO_INPUT)
	m_pkDisablePasswordInput = NULL;
	m_pkEnablePasswordInput  = NULL;
#endif
	m_pkBtnEnableInput	= NULL;
	m_pkBtnDisableInput	= NULL;
	m_pkBtnPrevPage		= NULL;
	m_pkBtnNextPage		= NULL;
	m_pkBtnClose		= NULL;
	m_pkStaticPage		= NULL;
	m_pkEdit			= NULL;
#if defined(COUPON_TWO_INPUT)
	m_pkEdit2			= NULL;
#endif
	m_pkStaticDesc		= NULL;
	
	LINK_CONTROL( "frame_coupon_disable",		m_pkDisableInput );
	LINK_CONTROL( "frame_coupon_enable",		m_pkEnableInput );
#if defined(COUPON_TWO_INPUT)
	LINK_CONTROL( "frame_password_disable",		m_pkDisablePasswordInput );
	LINK_CONTROL( "frame_password_enable",		m_pkEnablePasswordInput );
#endif
	LINK_CONTROL( "btn_coupon_input_disable",	m_pkBtnDisableInput );
	LINK_CONTROL( "btn_coupon_input_enable",	m_pkBtnEnableInput  );
	LINK_CONTROL( "btn_prev_page",				m_pkBtnPrevPage );
	LINK_CONTROL( "btn_next_page",				m_pkBtnNextPage );
	LINK_CONTROL( "btn_close",					m_pkBtnClose );
	LINK_CONTROL( "static_page",				m_pkStaticPage );
	LINK_CONTROL( "edit_coupon_input",			m_pkEdit );
#if defined(COUPON_TWO_INPUT)
	LINK_CONTROL( "edit_coupon_input2",			m_pkEdit2 );
#endif
	LINK_CONTROL( "static_coupon_desc",			m_pkStaticDesc );

	m_vecWebAddressInfo.clear();
}

KGCCouponNPCDlg::~KGCCouponNPCDlg(void)
{
}

void KGCCouponNPCDlg::OnCreate( void )
{
	for( int i = 0 ; i < MAX_COUPON_WND ; i++ )
	{
		m_pkEventWnd[i]->InitState( true, true, this );
		m_iCurPageEventUID[i] = -1;
	}

	m_pkDisableInput->InitState(true);
	m_pkEnableInput->InitState(false);
#if defined(COUPON_TWO_INPUT)
	m_pkDisablePasswordInput->InitState(true);
	m_pkEnablePasswordInput->InitState(false);
#endif
	m_pkBtnDisableInput->InitState( true, true, this );
	m_pkBtnEnableInput->InitState( false, true, this );
	m_pkBtnPrevPage->InitState( true, true, this );
	m_pkBtnNextPage->InitState( true, true, this );
	m_pkBtnClose->InitState( true, true, this );	

	m_pkBtnClose->SetHotKey( DIK_ESCAPE );

	m_pkStaticPage->InitState(true, true);
	m_pkStaticPage->SetAlign(DT_CENTER);

	m_pkEdit->InitState( false, false, this );
#if defined(COUPON_TWO_INPUT)
	m_pkEdit2->InitState(false, false, this);
	m_pkEdit2->SetText(L"");
#endif
	m_pkEdit->SetText(L"");
	m_pkEdit->SetFocus();

	m_pkStaticDesc->InitState( true, true );
	m_pkStaticDesc->SetAlign(DT_CENTER);
	m_pkStaticDesc->SetFontColor( 0xFFA9A9A9 );	

	m_iSelectEventUID = -1;
	m_iCheckSelect = -1;
	m_iCurPage = 1;

	LoadWebAddress();
	MovePage(m_iCurPage);
}

void KGCCouponNPCDlg::FrameMoveInEnabledState( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	for( int i = 0 ; i < MAX_COUPON_WND ; i++ )
	{
		if( m_iCheckSelect != i || m_iCheckSelect == -1 )
			m_pkEventWnd[i]->m_bSelectedWnd = false;

		if( m_pkEventWnd[i]->CheckPosInWindowBound(vMousePos) && m_pkEventWnd[i]->GetActiveWnd() )
		{
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				m_pkEventWnd[i]->m_bSelectedWnd = true;
				m_iCheckSelect = i;
				if( m_iCurPageEventUID[i] != -1 )
					m_iSelectEventUID = m_iCurPageEventUID[i];

				m_pkEdit->ToggleRender(true);
#if defined(COUPON_TWO_INPUT)
				m_pkEdit2->ToggleRender(true);
#endif
				m_pkEnableInput->ToggleRender(true);
				m_pkBtnEnableInput->ToggleRender(true);
				m_pkDisableInput->ToggleRender(false);
#if defined(COUPON_TWO_INPUT)
				m_pkEnablePasswordInput->ToggleRender(true);
				m_pkDisablePasswordInput->ToggleRender(false);
#endif
				m_pkBtnDisableInput->ToggleRender(false);
				m_pkStaticDesc->ToggleRender(false);
			}
		}

		if( !m_pkEventWnd[i]->m_bSelectedWnd )
		{
			m_pkEventWnd[i]->RenderSelectWnd(false);
		}
	}

	if( m_pkBtnPrevPage->CheckPosInWindowBound(vMousePos) )
	{
		if( g_pkInput->BtnUp( KInput::MBLEFT ) )
		{
			g_KDSound.Play( "GPC_BUTTON_CLICK" );
			m_iCurPage--;
			if( m_iCurPage != 0 )
			{
				MovePage( m_iCurPage );
			}
			else
			{
				m_iCurPage = 1;
			}
		}
	}
	else if( m_pkBtnNextPage->CheckPosInWindowBound(vMousePos) )
	{
		if( g_pkInput->BtnUp( KInput::MBLEFT ) )
		{
			g_KDSound.Play( "GPC_BUTTON_CLICK" );
			m_iCurPage++;
			if( m_iCurPage <= GetMaxPage() )
			{
				MovePage( m_iCurPage );
			}
			else
			{
				m_iCurPage = GetMaxPage();
			}
		}
	}
}

void KGCCouponNPCDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkBtnClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		//g_KDSound.Play( "GPC_BUTTON_CLICK" );
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}

	if( event.m_pWnd == m_pkBtnEnableInput && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && !m_bClickedInputBtn)
	{
		OnClickInputBtn();
	}

	//GCWND_MSG_MAP( m_pkEdit,			KD3DWnd::D3DWE_EDIT_RETURN, OnClickInputBtn );
}

void KGCCouponNPCDlg::OnCreateComplete()
{
}

void KGCCouponNPCDlg::OnDestroyComplete()
{
}


void KGCCouponNPCDlg::PostChildDraw()
{
}

int KGCCouponNPCDlg::GetMaxPage()
{
	if( (int)m_vecCouponInfo.size() % MAX_COUPON_WND == 0 )
	{
		if( (int)m_vecCouponInfo.size() <= MAX_COUPON_WND )
			return 1;
		else
			return (int)m_vecCouponInfo.size() / MAX_COUPON_WND;
	}
	else
	{
		return (int)((int)m_vecCouponInfo.size() / MAX_COUPON_WND + 1);
	}
}

void KGCCouponNPCDlg::MovePage( int iPage_ )
{
	m_iCurPage = iPage_;

	WCHAR strText[20];
	swprintf(strText, 20, L"%d / %d", iPage_, GetMaxPage());	
	m_pkStaticPage->SetText( strText );

	if( iPage_ == GetMaxPage() )
	{
		if( (int)m_vecCouponInfo.size() <= MAX_COUPON_WND )
			DrawEventList(iPage_, (int)m_vecCouponInfo.size() );			
		else if( (int)m_vecCouponInfo.size() % MAX_COUPON_WND == 0 ) 
			DrawEventList(iPage_, MAX_COUPON_WND );			
		else
			DrawEventList(iPage_, (int)m_vecCouponInfo.size() % MAX_COUPON_WND );			
	}
	else
		DrawEventList(iPage_, MAX_COUPON_WND );

	InitWindow();
}

void KGCCouponNPCDlg::DrawEventList( int iPage_, int iCurPageEventListNum_ )
{
	int iStartIndex = iPage_ * MAX_COUPON_WND - MAX_COUPON_WND;

	//쿠폰테스트 
	if( iStartIndex >(int)m_vecCouponInfo.size() )
		return;

	for( int i = 0 ; i < iCurPageEventListNum_ ; i++ )
	{
		m_pkEventWnd[i]->SetText( m_vecCouponInfo[i + iStartIndex ].m_strEventName, m_vecCouponInfo[i + iStartIndex ].m_strContents);
		m_iCurPageEventUID[i] = m_vecCouponInfo[i + iStartIndex ].m_nEventUID;
		m_pkEventWnd[i]->SetActiveWnd(m_vecCouponInfo[i + iStartIndex ].m_bActive);
		m_pkEventWnd[i]->SetEventTexture(m_iCurPageEventUID[i]);
		m_pkEventWnd[i]->RenderEventState(m_vecCouponInfo[i + iStartIndex ].m_bNew);
		m_pkEventWnd[i]->SetEmptyWnd(false);
#if defined(NATION_IDN)
        m_pkEventWnd[i]->SetWebButtonOff();
#endif
		for( int j = 0; j < (int)m_vecWebAddressInfo.size() ; j++ )
		{
			if( m_vecWebAddressInfo[j].first == m_iCurPageEventUID[i] )
			{
				m_pkEventWnd[i]->m_strWebAddress = m_vecWebAddressInfo[j].second;
			}
		}
	}

	// 내용이 없는 녀석들은 아무것도 출력하지 않는다.
	if( iCurPageEventListNum_ < MAX_COUPON_WND )
	{
		int iNotRenderLine = MAX_COUPON_WND - iCurPageEventListNum_;

		for(int i = 1 ; i <= iNotRenderLine ; i++ )
		{
			std::wstring str = L"";
			m_pkEventWnd[ MAX_COUPON_WND - i ]->SetText(str,str);		
			m_pkEventWnd[ MAX_COUPON_WND - i ]->RenderEventState(false);
			m_pkEventWnd[ MAX_COUPON_WND - i ]->SetEmptyWnd(true);
		}
	}
}

void KGCCouponNPCDlg::SetCouponInfo( std::vector< KGameCouponInfo >& vecCouponInfo_ )
{
	m_vecCouponInfo.clear();
	m_vecCouponInfo = vecCouponInfo_;
	MovePage(m_iCurPage);
}

bool KGCCouponNPCDlg::CheckInvalidCouponString(const std::wstring& str)
{
	for (int i=0;i< (int)str.length();i++)
	{
		//39 = 싱글 따옴표 (')
		if (str[i]==39 || str[i]=='-' || ( str[i]>='a' && str[i]<='z') || ( str[i]>='A' && str[i]<='Z')
			|| ( str[i]>='0' && str[i]<='9'))
			continue;
		return false;
	}
	return true;
}

void KGCCouponNPCDlg::OnClickInputBtn()
{
	std::wstring strCouponID = m_pkEdit->GetText();
#if defined(COUPON_TWO_INPUT)
	std::wstring strPassWD = m_pkEdit2->GetText();
#else
	std::wstring strPassWD = L"";
#endif

	//조건에 벗어나는 스트링 넣은 경우..
	if (CheckInvalidCouponString(strCouponID) == false || CheckInvalidCouponString(strPassWD) == false)
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COUPON_NPC_INCORRECT_COUPON), L"", 
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );

		g_pkUIScene->m_pkCouponNPCDlg->m_bClickedInputBtn = false;
		return;
	}
#if defined(COUPON_TWO_INPUT)
	KP2P::GetInstance()->Send_UseGameCouponReq( m_iSelectEventUID, m_pkEdit->GetText(),m_pkEdit2->GetText());
#else
	KP2P::GetInstance()->Send_UseGameCouponReq( m_iSelectEventUID, m_pkEdit->GetText(),L"");
#endif

	InitWindow();
	m_bClickedInputBtn = true;
	m_pkEdit->SetText(L"");
#if defined(COUPON_TWO_INPUT)
	m_pkEdit2->SetText(L"");
#endif

}

void KGCCouponNPCDlg::InitWindow()
{
	for( int i = 0 ; i < MAX_COUPON_WND ; i++ )
	{
		m_pkEventWnd[i]->RenderSelectWnd(false);
		m_pkEventWnd[i]->m_bSelectedWnd = false;
	}

	m_pkEnableInput->ToggleRender(false);	
	m_pkDisableInput->ToggleRender(true);

#if defined(COUPON_TWO_INPUT)
	m_pkEnablePasswordInput->ToggleRender(false);	
	m_pkDisablePasswordInput->ToggleRender(true);
#endif
	
	m_pkBtnEnableInput->ToggleRender(false);
	m_pkBtnDisableInput->ToggleRender(true);
	m_pkEdit->ToggleRender(false);
	m_pkEdit->SetText(L"");

#if defined(COUPON_TWO_INPUT)
	m_pkEdit2->ToggleRender(false);
	m_pkEdit2->SetText(L"");	
#endif
	m_pkStaticDesc->SetText( g_pkStrLoader->GetString(STR_ID_COUPON_NPC_DESC) );
	m_pkStaticDesc->ToggleRender(true);
}

void KGCCouponNPCDlg::LoadWebAddress()
{
	KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "EventCouponWebAddress.lua", false) == false)
	{
		return;
	}

	m_vecWebAddressInfo.clear();

	LUA_BEGIN_TABLE("EventCouponWebAddressList",return);
	{
		DWORD				dwEventID;
		std::string		strWebAddress;

		for(int i = 1; ; i++)
		{
			LUA_BEGIN_TABLE(i,break)
			{
				LUA_GET_VALUE_NOASSERT("EventUID", dwEventID, ; );
				LUA_GET_VALUE_NOASSERT("WebAddress", strWebAddress, ; );
				m_vecWebAddressInfo.push_back(std::make_pair(dwEventID, strWebAddress));
			}			
			LUA_END_TABLE(return);
		}
	}
	LUA_END_TABLE(return);
}

bool KGCCouponNPCDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    KP2P::GetInstance()->Send_GameCouponListReq();
    return true;
}