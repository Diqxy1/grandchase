#include "stdafx.h"
#include "GCPresentCashItemWnd.h"

IMPLEMENT_CLASSNAME( KGCPresentCashItemWnd );
IMPLEMENT_WND_FACTORY( KGCPresentCashItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCPresentCashItemWnd, "gc_present_cashitem_wnd" );

KGCPresentCashItemWnd::KGCPresentCashItemWnd( void )
{
	m_pFriendListBtn = NULL;
	m_pCloseBtn = NULL;
	m_pCashBtn = NULL;
	m_pVpBtn = NULL;
	LINK_CONTROL( "btn_friend_list",		m_pFriendListBtn );
	LINK_CONTROL( "btn_close",				m_pCloseBtn );
	LINK_CONTROL( "btn_cash",				m_pCashBtn );
	LINK_CONTROL( "btn_vp",					m_pVpBtn );

	m_pkStaticItemName = NULL;
	LINK_CONTROL( "static_item_name",		m_pkStaticItemName );

	m_pkStaticTitlePrice = NULL;
	m_pkStaticTitleMyCash = NULL;
	m_pkStaticTitleMyVp = NULL;
	m_pkStaticPrice = NULL;
	m_pkStaticMyCash = NULL;
	m_pkStaticMyVp = NULL;
	LINK_CONTROL( "static_title_price",		m_pkStaticTitlePrice );
	LINK_CONTROL( "static_title_my_cash",	m_pkStaticTitleMyCash );
	LINK_CONTROL( "static_title_my_vp",		m_pkStaticTitleMyVp );
	LINK_CONTROL( "static_price",			m_pkStaticPrice );
	LINK_CONTROL( "static_my_cash",			m_pkStaticMyCash );
	LINK_CONTROL( "static_my_vp",			m_pkStaticMyVp );

	m_pkStaticLength = NULL;
	m_pkStaticTip = NULL;
	LINK_CONTROL( "static_length",			m_pkStaticLength );
	LINK_CONTROL( "static_tip",				m_pkStaticTip );

	m_pkEditReceiver = NULL;
	LINK_CONTROL( "edit_receiver",			m_pkEditReceiver );

	m_pkEditMsg = NULL;
	LINK_CONTROL( "edit_msg",				m_pkEditMsg );

	m_pkItemImgWnd = NULL;
	m_pkSetItemImgWnd = NULL;
	LINK_CONTROL( "item_img",				m_pkItemImgWnd );
	LINK_CONTROL( "setitem_img",			m_pkSetItemImgWnd );

	m_pCheckNickBtn = NULL;
	LINK_CONTROL( "btn_check_nick",			m_pCheckNickBtn );

	m_bCheckedNickname = false;
	m_wcCheckNicknameTemp[0] = NULL;
	m_bPresent = false;
}

KGCPresentCashItemWnd::~KGCPresentCashItemWnd( void )
{
}

void KGCPresentCashItemWnd::OnCreate( void )
{
	m_pFriendListBtn->InitState( true, true, this );
	m_pFriendListBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClickFriendListBtn );

	m_pCloseBtn->InitState( true, true, this );
	m_pCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClose );
	m_pCloseBtn->SetHotKey( DIK_ESCAPE );

	m_pCashBtn->InitState( false, true, this ); // 현재 캐쉬 선물하기를 지원하지 않음 (퍼블리셔에 문의)
	m_pCashBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClickCashBtn );

	m_pVpBtn->InitState( true, true, this );
	m_pVpBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClickVpBtn );

	m_pkStaticItemName->InitState( true );
	m_pkStaticItemName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
	m_pkStaticItemName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );

	m_pkStaticTitlePrice->InitState( true );
	m_pkStaticTitlePrice->SetText( /*L"가격"*/g_pkStrLoader->GetString( STR_ID_BUY_CASH_BOX04 ) );
	m_pkStaticTitlePrice->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
	m_pkStaticTitlePrice->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );

	m_pkStaticTitleMyCash->InitState( false ); // 현재 캐시 적용 안함
	m_pkStaticTitleMyCash->SetText( /*L"보유 캐시"*/g_pkStrLoader->GetString( STR_ID_PRESENT_OWN_CASH ) );

	m_pkStaticTitleMyVp->InitState( true );
	m_pkStaticTitleMyVp->SetText( /*L"보유 VP"*/g_pkStrLoader->GetString( STR_ID_PRESENT_OWN_VP ) );

	m_pkStaticPrice->InitState( true );
	m_pkStaticPrice->SetAlign( DT_RIGHT );
	m_pkStaticPrice->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
	m_pkStaticPrice->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );

	m_pkStaticMyCash->InitState( false ); // 현재 캐시 적용 안함
	m_pkStaticMyCash->SetAlign( DT_RIGHT );

	m_pkStaticMyVp->InitState( true );
	m_pkStaticMyVp->SetAlign( DT_RIGHT );

	m_pkStaticLength->InitState( true, true, this );
	m_pkStaticLength->SetAlign( DT_CENTER );

	m_pkStaticTip->InitState( true );
	m_pkStaticTip->SetAlign( DT_CENTER );
	m_pkStaticTip->SetMultiLine( true );
	m_pkStaticTip->SetFontColor( D3DCOLOR_ARGB(255, 128, 128, 128) );
	m_pkStaticTip->SetText( g_pkStrLoader->GetString( STR_ID_PRESENT_TIP ) );

	m_pkEditReceiver->InitState( true, true, this );
	m_pkEditReceiver->SetMultiLine( false );
#ifdef NATION_KOREA
	m_pkEditReceiver->SetLimitText( GC_NICKNAME_SIZE/2 );
#else
	m_pkEditReceiver->SetLimitText( GC_NICKNAME_SIZE );
#endif
	m_pkEditReceiver->SetText(L"");
	m_pkEditReceiver->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClickEdit );

	m_pkEditMsg->InitState( true, true, this );
	m_pkEditMsg->SetMultiLine( true );
	m_pkEditMsg->SetNewLine( true );
	m_pkEditMsg->SetLimitLine( EDIT_LIMIT_LINE );
	m_pkEditMsg->SetLimitText( EDIT_LIMIT_TEXT );
	m_pkEditMsg->SetText(L"");

	m_pkItemImgWnd->InitState( false );
	m_pkSetItemImgWnd->InitState( false );

	m_pCheckNickBtn->InitState( true, true, this );
	m_pCheckNickBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentCashItemWnd::OnClickCheckNickBtn );
}

void KGCPresentCashItemWnd::OnCreateComplete()
{
	SetGoodsInfo();
	m_pkEditReceiver->SetFocus();
	m_bPresent = true;
	SetCheckedNickname( false );
}


void KGCPresentCashItemWnd::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCPresentCashItemWnd::FrameMoveInEnabledState( void )
{
	//사용자 편의
	if( m_pkEditReceiver->HasFocus() ) // 받는 유저에서 엔터를 하면, 닉네임 체크를 했으면 Msg에 포커스를 넘기고, 아니면 닉네임 체크를 바로 한다.
	{	
		if( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_NUMPADENTER ) )
		{
			if( m_bCheckedNickname )
				m_pkEditMsg->SetFocus();
			else
				OnClickCheckNickBtn();
		}
		else if( g_pkInput->IsDown( DIK_TAB ) ) // TAB키로 받는유저 -> Msg로 포커스 이동
		{
			m_pkEditMsg->SetFocus();
		}
	}
	else if( m_pkEditMsg->HasFocus() && g_pkInput->IsDown( DIK_TAB ) ) // TAB키로 Msg -> 받는유저로 포커스 이동
	{
		m_pkEditReceiver->SetFocus();
	}
	else if( !m_pkEditMsg->HasFocus() && !m_pkEditReceiver->HasFocus() ) // 포커스를 잃어도, 항상 포커스를 준다.
	{	
		if( m_bCheckedNickname == false )
			m_pkEditReceiver->SetFocus();
		else
			m_pkEditMsg->SetFocus();
	}

	if( m_pkStaticLength->IsRenderOn() )
	{
		WCHAR strTemp[20];
		std::wstring strCurNum = m_pkEditMsg->GetText();
		swprintf( strTemp, 20, L"%d / %d", strCurNum.length(), EDIT_LIMIT_TEXT );
		m_pkStaticLength->SetText(strTemp);
	}

	if( m_pkEditReceiver->HasFocus() && m_bCheckedNickname ) // 닉네임 확인 후, 닉네임을 수정하면 닉네임 확인 다시 해야 한다.
	{
		if( wcscmp( m_wcCheckNicknameTemp, m_pkEditReceiver->GetText() ) != 0 )
		{
			SetCheckedNickname( false );
		}
	}
}

void KGCPresentCashItemWnd::OnClickFriendListBtn( void )
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PRESENT_FRIEND_LIST, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true ); // KWAK PRESENT
}

void KGCPresentCashItemWnd::OnClose( void ) // CLOSE 할 때 반드시 이 함수를 거치세요
{
	m_bPresent = false;
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_PRESENT_CASH_ITEM_WND );//꺼도 다시켜져서..
}

void KGCPresentCashItemWnd::OnClickCashBtn( void )
{
	// 현재 캐쉬 선물하기를 지원하지 않음.
	//if( CheckReadyToBuy() )
	//{
	//	//g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	//}
}

void KGCPresentCashItemWnd::OnClickVpBtn( void )
{
	if( CheckReadyToBuy() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
}

void KGCPresentCashItemWnd::OnClickEdit( void )
{
	m_pkEditReceiver->SetFocus();
}

void KGCPresentCashItemWnd::SetGoodsInfo( void )
{
	// 현재 상품에 대한 정보를 얻자
	GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( g_pItemMgr->GetCurrentGoodsID() );

	if( pkItem == NULL )
		return;

	// 상품정보 저장
	m_kItem = *pkItem;

	// 아이템 이름
	m_pkStaticItemName->SetTextAutoMultiline( GetItemName() );

	// 캐쉬금액
	m_pkStaticPrice->SetText( m_kItem.dwPrice );
	m_pkStaticMyCash->SetText( g_kGlobalValue.m_dwCash );
	m_pkStaticMyVp->SetText( g_kGlobalValue.m_dwVirtualCash );

	// 아이템 이미지
	if ( m_kItem.dwSlotPosition & ESP_SET_ITEM )
	{
		m_pkSetItemImgWnd->ToggleRender( true );
		m_pkSetItemImgWnd->SetItemInfo( m_kItem.iSetItemNumber, m_kItem.dwGoodsID, -1.0f );
		m_pkSetItemImgWnd->ShowItem( true );
		m_pkItemImgWnd->ToggleRender( false );
		m_pkItemImgWnd->ShowItem( false );
	} else {
		m_pkItemImgWnd->ToggleRender( true );
		m_pkItemImgWnd->SetItemInfo( m_kItem.dwGoodsID );
		m_pkItemImgWnd->ShowItem( true );
		m_pkSetItemImgWnd->ToggleRender( false );
		m_pkSetItemImgWnd->ShowItem( false );
	}
}

void KGCPresentCashItemWnd::SetNickname( std::wstring& wsNickName )
{
	if( wcslen( wsNickName.c_str() ) != 0 )
		m_pkEditReceiver->SetText( wsNickName.c_str() );
}

void KGCPresentCashItemWnd::OnClickCheckNickBtn( void )
{
	if( m_bCheckedNickname )
	{
		// Message : 이미 확인 된 닉네임 입니다.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PRESENT_ALREADY_CHECKED_NICKNAME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

	if( wcslen( m_pkEditReceiver->GetText() ) == 0 )
	{
		// Message : 닉네임을 입력해 주세요.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

	// 캐릭터 닉네임 사용 체크
	Result_ReportNickName = INT_MAX;
	KP2P::GetInstance()->SendNickNameValidityCheckReq( m_pkEditReceiver->GetText() );
	g_MyD3D->WaitForServerAck(Result_ReportNickName, INT_MAX, 10000, -6 );

	// 닉네임 확인 결과
	if( Result_ReportNickName == 0 ) // 성공
	{
		// Message : 닉네임이 확인되었습니다.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PRESENT_NICKNAME_IS_CHECKED_NOW ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		SetCheckedNickname( true );
	}
	else if( Result_ReportNickName == 1 ) // 나 자신에게
	{
		// Message : 자신에게 선물을 보낼 수 없습니다.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PRESENT_CANNOT_SEND_TO_MYSELF ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
	else // 기타
	{
		// Message : 존재하지 않는 닉네임 입니다.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_EXISTED_NICKNAME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
}

bool KGCPresentCashItemWnd::CheckReadyToBuy( void )
{
	if( m_bCheckedNickname )
		return true;
	else
	{
		// Message : 닉네임 확인을 해 주세요.
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PRESENT_PLEASE_CHECK_NICKNAME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return false;
	}
}

void KGCPresentCashItemWnd::SetCheckedNickname( bool bCheckedNickname )
{
	m_bCheckedNickname = bCheckedNickname;
	if( bCheckedNickname)
		wcscpy( m_wcCheckNicknameTemp, m_pkEditReceiver->GetText() );

	m_pCheckNickBtn->SetWndMode( bCheckedNickname ? D3DWM_LOCK : D3DWM_DEFAULT );
}

std::wstring KGCPresentCashItemWnd::GetComment( void )
{
	if( wcslen( m_pkEditMsg->GetText() ) == 0 ) // 메시지가 없으면, 기본 시스템 메시지 전송
	{
		std::wstring wstrID = g_kGlobalValue.m_kUserInfo.strNickName;
		return wstrID + g_pkStrLoader->GetString( STR_ID_PRESENT_DEFAULT_MESSAGE ); // OOO님이 보내신 선물입니다.
	}
	return m_pkEditMsg->GetText();
}

std::wstring KGCPresentCashItemWnd::GetItemName( void )
{
	int iCharType = -1;
	DWORD dwCommonCharType = 0; 
	for( int i = 0 ; i < GC_CHAR_NUM; ++i)
	{
		dwCommonCharType = dwCommonCharType | ( 1 << i );
	}

	if(dwCommonCharType != m_kItem.dwCharType ) {
		iCharType = m_kItem.GetCharType();
	}

	wstringstream strm;

	if( iCharType == -1 ) { //공용 아이템일 경우 (공용)
		strm << m_kItem.strItemName << L"(" << g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON ) << L")";
	}
	else { //캐릭터 전용 아이템일 경우 직업을 빨간색으로 표시 (ㅇㅇ 전용)
		strm << m_kItem.strItemName << L"#cff0000" << L"(" << GCFUNC::GetCharName( iCharType ) << L" " << g_pkStrLoader->GetString( STR_ID_IS_EXCLUSIVE ) << L")";
	}
	return strm.str();
}