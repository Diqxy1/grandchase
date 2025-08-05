#include "stdafx.h"
#include "KGCGuildSettingChangeDlg.h"
#include "GCMultiMenu.h"

IMPLEMENT_CLASSNAME( KGCGuildSettingChangeDlg );
IMPLEMENT_WND_FACTORY( KGCGuildSettingChangeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildSettingChangeDlg, "gc_guild_setting_change_dlg" );

KGCGuildSettingChangeDlg::KGCGuildSettingChangeDlg(void)
:m_pkGuildName(NULL)
,m_pkStaticJoinUsage(NULL)
,m_pkGuildNameChangeDesc(NULL)
,m_pkBtnNameChangeOK(NULL)
,m_pkBtnURLChangeOK(NULL)
,m_pkEditGuildName(NULL)
,m_pkStaticGuildURL(NULL)
,m_pkGuildMarkIcon(NULL)
,m_pkTexNameChangeItem(NULL)
,m_pkTexMarkChangeItem(NULL)
,m_pkMarkChangeItemBlind(NULL)
{
	//	static
	LINK_CONTROL( "guild_name",							m_pkGuildName );
	LINK_CONTROL( "guild_name_change_desc",				m_pkGuildNameChangeDesc );
	LINK_CONTROL( "guild_mark_change_desc",				m_pkGuildMarkChangeDesc );
	LINK_CONTROL( "guild_destroy_desc",					m_pkGuildDestroyDesc );
	
	//	button
	LINK_CONTROL( "guild_name_change_btn",				m_pkGuildNameChangeBtn );
	LINK_CONTROL( "guild_name_change_cancel_btn",		m_pkGuildNameChangeCancelBtn );

	LINK_CONTROL( "guild_mark_change_btn",				m_pkGuildMarkChangeBtn );
	LINK_CONTROL( "guild_mark_change_cancel_btn",		m_pkGuildMarkChangeCancelBtn );

	LINK_CONTROL( "guild_URL_change_btn",				m_pkGuildURLChangeBtn );
	LINK_CONTROL( "guild_URL_change_cancel_btn",		m_pkGuildURLChangeCancelBtn );

	LINK_CONTROL( "guild_how_to_join_info_popup_btn",	m_pkGuildHowToJoinInfoPopupBtn );
	LINK_CONTROL( "guild_how_to_join_multimenu",		m_pkGuildHowToJoinInfoMultiMenu );

	LINK_CONTROL( "guild_destroy_btn",					m_pkGuildDestroyBtn );
	
	LINK_CONTROL( "guild_URL_edit",						m_pkGuildURLEdit );

	LINK_CONTROL( "static_guild_join_usage",			m_pkStaticJoinUsage);

	LINK_CONTROL( "btn_guild_name_change_ok",			m_pkBtnNameChangeOK);
	LINK_CONTROL( "btn_guild_url_change_ok",			m_pkBtnURLChangeOK);

	LINK_CONTROL( "edit_guild_name",					m_pkEditGuildName);
	LINK_CONTROL( "static_guild_URL",					m_pkStaticGuildURL);

	LINK_CONTROL( "guild_mark_icon",					m_pkGuildMarkIcon);

	LINK_CONTROL( "guild_name_change_item",				m_pkTexNameChangeItem);
	LINK_CONTROL( "guild_mark_change_item",				m_pkTexMarkChangeItem);

	LINK_CONTROL( "item_info_box",						m_pkItemInfoBox);

	LINK_CONTROL( "guild_name_change_box_blind",		m_pkNameChangeItemBlind);
	LINK_CONTROL( "guild_mark_change_box_blind",		m_pkMarkChangeItemBlind);

	LoadSettingChangeItem();
}

KGCGuildSettingChangeDlg::~KGCGuildSettingChangeDlg(void)
{
}

void KGCGuildSettingChangeDlg::OnCreate()
{
	m_pkGuildName->InitState( true, true, this );
	m_pkGuildName->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);
	m_pkStaticJoinUsage->InitState(true, true, this);
	m_pkGuildNameChangeDesc->InitState( true, true, this );
	m_pkGuildNameChangeDesc->SetLineSpace( 1.2f );
	m_pkGuildNameChangeDesc->SetMultiLine(true);
	m_pkGuildNameChangeDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NAME_CHANGE_DESC));
	m_pkGuildMarkChangeDesc->InitState( true, true, this );
	m_pkGuildMarkChangeDesc->SetLineSpace( 1.2f );
	m_pkGuildMarkChangeDesc->SetMultiLine(true);
	m_pkGuildMarkChangeDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MARK_CHANGE_DESC));
	m_pkGuildDestroyDesc->InitState( true, true, this );
	m_pkGuildDestroyDesc->SetLineSpace( 1.2f );
	m_pkGuildDestroyDesc->SetMultiLine(true);
	m_pkGuildDestroyDesc->SetFontColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	m_pkGuildDestroyDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_BREAK_UP_DESC));

	m_pkGuildNameChangeBtn->InitState( true, true, this );
	m_pkGuildNameChangeCancelBtn->InitState( true, true, this );

	m_pkGuildMarkChangeBtn->InitState( true, true, this );
	m_pkGuildMarkChangeCancelBtn->InitState( true, true, this );	

	m_pkGuildHowToJoinInfoPopupBtn->InitState( true, true, this );
	m_pkGuildHowToJoinInfoMultiMenu->InitState( false, true, this );
	m_pkGuildHowToJoinInfoMultiMenu->ClearMenu();
	m_pkGuildHowToJoinInfoMultiMenu->AddMenu(1, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY));
	m_pkGuildHowToJoinInfoMultiMenu->AddMenu(2, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER));
	m_pkGuildHowToJoinInfoMultiMenu->AddMenu(3, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP));

	m_pkGuildDestroyBtn->InitState( true, true, this );

	m_pkGuildURLEdit->InitState( false, true, this );

	m_pkBtnNameChangeOK->InitState( false, true, this);

	m_pkGuildURLChangeBtn->InitState( true, true, this );	
	m_pkBtnURLChangeOK->InitState( false, true, this);
	m_pkGuildURLChangeCancelBtn->InitState( true, true, this );

	m_pkEditGuildName->InitState(false,true, this);
#if defined(NATION_IDN)
    m_pkEditGuildName->SetLimitText(12);
#endif
	m_pkEditGuildName->SetFocus();

	m_pkStaticGuildURL->InitState(true);

	m_pkStaticGuildURL->SetText(g_kGlobalValue.m_kGuildInfo.m_strURL);

	m_pkTexNameChangeItem->InitState(true);
	m_pkTexNameChangeItem->SetItemInfo(*m_sSettingChangeItem.m_setNameChangeItemID.begin());
	m_pkTexMarkChangeItem->InitState(true);
	m_pkTexMarkChangeItem->SetItemInfo(*m_sSettingChangeItem.m_setMarkChangeItemID.begin());

	m_pkItemInfoBox->InitState(false, true, this);

	SetEditModeGuildName(false);
	SetEditModeGuildURL(false);

	m_pkGuildMarkIcon->InitState(true, true, this);
	m_pkMarkChangeItemBlind->InitState(false);
	m_pkNameChangeItemBlind->InitState(false);
}

void KGCGuildSettingChangeDlg::OnCreateComplete()
{
	//길드마크
	SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);	
	m_pkGuildMarkIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);

	//수량이 없는 경우에는 변경 버튼을 잠근다.
	m_pkGuildNameChangeBtn->Lock(true);
	m_pkGuildMarkChangeBtn->Lock(true);
	std::set<int>::iterator sIter = m_sSettingChangeItem.m_setNameChangeItemID.begin();
	while(sIter != m_sSettingChangeItem.m_setNameChangeItemID.end())
	{
		if((DWORD)g_pItemMgr->GetInventoryItemDuration(*sIter) > 0)
		{
			m_pkGuildNameChangeBtn->Lock(false);
		}
		sIter++;
	}

	sIter = m_sSettingChangeItem.m_setMarkChangeItemID.begin();
	while(sIter != m_sSettingChangeItem.m_setMarkChangeItemID.end())
	{
		if((DWORD)g_pItemMgr->GetInventoryItemDuration(*sIter)>0)
		{
			m_pkGuildMarkChangeBtn->Lock(false);
		}
		sIter++;
	}
}

void KGCGuildSettingChangeDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkGuildHowToJoinInfoMultiMenu && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
	{
		g_KDSound.Play( "31" );
		switch( event.m_dwlParam )
		{
		case EG_JOIN_DIRECTLY:
			if( g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY) != m_pkStaticJoinUsage->GetText() )
				KP2P::GetInstance()->Send_ChangeGuildJoinSettingReq(KNGuildInfo::JS_AUTO_ACCEPT);
			m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY));
			break;
		case EG_JOIN_AFTER:
			if( g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER) != m_pkStaticJoinUsage->GetText() )
				KP2P::GetInstance()->Send_ChangeGuildJoinSettingReq(KNGuildInfo::JS_AFTER_DECISION);
			m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER));			
			break;
		case EG_JOIN_STOP:				
			if( g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP) != m_pkStaticJoinUsage->GetText() )
				KP2P::GetInstance()->Send_ChangeGuildJoinSettingReq(KNGuildInfo::JS_STOP_JOIN);
			m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP));
			break;		
		}	
	}

	//	길드 가입 방법 변경 멀티 메뉴 팝업 버튼
	if( event.m_pWnd ==	m_pkGuildHowToJoinInfoPopupBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK){
		g_KDSound.Play( "31" );
		if( m_pkGuildHowToJoinInfoMultiMenu->IsRenderOn() )
			m_pkGuildHowToJoinInfoMultiMenu->ToggleRender(false);
		else
			m_pkGuildHowToJoinInfoMultiMenu->ToggleRender(true);
	}

	// 길드이름 편집버튼
	if( event.m_pWnd == m_pkGuildNameChangeBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		// 길드이름은 길드마스터만 변경이 가능하다.
		if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER )
		{
			SetEditModeGuildName(true);
		}
		else
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION_MSG2), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		}
	}

	// 길드이름 편집 후 확인 버튼
	if( event.m_pWnd == m_pkBtnNameChangeOK && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_kGlobalValue.m_strTempGuildName = m_pkEditGuildName->GetText();
		g_KDSound.Play( "31" );
		m_pkGuildName->SetText(m_pkEditGuildName->GetText());
		m_pkEditGuildName->SetText(L"");
		SetEditModeGuildName(false);

		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, 
			g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NAME_CHANGE_SUCCESS), g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NAME_CHANGE_ALERT_MSG), 
			KGCUIScene::GC_MBOX_GUILD_NAME_CHANGE_ALERT, KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true );		
	}

	// 길드이름 편집 취소 버튼
	if( event.m_pWnd == m_pkGuildNameChangeCancelBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		m_pkEditGuildName->SetText(L"");
		SetEditModeGuildName(false);		
    }

	// 길드 url 편집 버튼
	if( event.m_pWnd == m_pkGuildURLChangeBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		SetEditModeGuildURL(true);
	}

	// 길드 url 편집 후 확인 버튼
	if( event.m_pWnd == m_pkBtnURLChangeOK && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		KP2P::GetInstance()->Send_EditGuildUrlReq(m_pkGuildURLEdit->GetText(), g_kGlobalValue.m_kGuildInfo.m_dwUID );

		g_KDSound.Play( "31" );
		m_pkStaticGuildURL->SetText(m_pkGuildURLEdit->GetText());
		SetEditModeGuildURL(false);
	}

	// 길드 url 편집 취소 버튼
	if( event.m_pWnd == m_pkGuildURLChangeCancelBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		SetEditModeGuildURL(false);		
	}

	// 길드 마크 변경 버튼
	if( event.m_pWnd == m_pkGuildMarkChangeBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		if( g_kGlobalValue.m_kGuildInfo.m_ucMarkStatus == KNGuildInfo::MS_REQUEST_CHANGE )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MARK_CHANGE_STATE_MSG), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		}
		else if( g_kGlobalValue.m_kGuildInfo.m_ucGrade == KNGuildInfo::GG_NEWBIE )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MARK_CHANGE_FAIL1), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		}
		else
		{
			std::set<int>::iterator sIter = m_sSettingChangeItem.m_setMarkChangeItemID.begin();
			int iGuildMarkItemID = *sIter;
			for(;sIter !=m_sSettingChangeItem.m_setMarkChangeItemID.end() ;sIter++)
			{
				if(g_pItemMgr->FindInventoryForItemID(*sIter))
				{
					iGuildMarkItemID = *sIter;
					break;
				}
			}
			g_pkUIScene->m_pkGuildMarkRegistDlg->SetMarkChangeItemID(iGuildMarkItemID);
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILDMARK_REGIST_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		}
	}

	// 길드 해산 버튼
	if( event.m_pWnd == m_pkGuildDestroyBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		// 길드해산은 길드마스터만 가능하다.
		if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_DESTROY_ALERT_TITLE), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_DESTROY_ALERT_MSG), KGCUIScene::GC_MBOX_GUILD_DESTROY, KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true );
		}
		else
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_DESTROY_NOT_MASTER), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		}
	}
}

void KGCGuildSettingChangeDlg::FrameMoveInEnabledState( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkTexNameChangeItem->CheckPosInWindowBound(vMousePos) )
	{
		D3DXVECTOR2 vecPos( ( m_pkTexNameChangeItem->GetFixedWindowLocalPos().x + 30.0f ) * m_fWindowScaleX , m_pkTexNameChangeItem->GetFixedWindowLocalPos().y * m_fWindowScaleY);

		GCItem* kItem = g_pItemMgr->GetItemData(*m_sSettingChangeItem.m_setNameChangeItemID.begin());	

        if( kItem != NULL )
            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	}
	else if( m_pkTexMarkChangeItem->CheckPosInWindowBound(vMousePos) )
	{
		D3DXVECTOR2 vecPos( ( m_pkTexMarkChangeItem->GetFixedWindowLocalPos().x + 30.0f ) * m_fWindowScaleX, m_pkTexMarkChangeItem->GetFixedWindowLocalPos().y * m_fWindowScaleY);

		GCItem* kItem = g_pItemMgr->GetItemData(*m_sSettingChangeItem.m_setMarkChangeItemID.begin());	

        if( kItem != NULL )
            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );

        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	}
	else
	{
		//m_pkItemInfoBox->ToggleRender( false );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}

void KGCGuildSettingChangeDlg::SetEditModeGuildName( bool bEditMode )
{
	m_pkGuildNameChangeBtn->ToggleRender(!bEditMode);
	m_pkGuildName->ToggleRender(!bEditMode);
	m_pkBtnNameChangeOK->ToggleRender(bEditMode);
	m_pkEditGuildName->ToggleRender(bEditMode);
	m_pkEditGuildName->SetFocus();
}

void KGCGuildSettingChangeDlg::SetEditModeGuildURL( bool bEditMode )
{

	m_pkGuildURLChangeBtn->ToggleRender(!bEditMode);
	m_pkStaticGuildURL->ToggleRender(!bEditMode);
	m_pkBtnURLChangeOK->ToggleRender(bEditMode);
	m_pkGuildURLEdit->ToggleRender(bEditMode);
	m_pkGuildURLEdit->SetText(L"http://");
	m_pkGuildURLEdit->SetFocus();
}

void KGCGuildSettingChangeDlg::InitGuildSettingInfo()
{
    m_pkGuildName->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);	
	m_pkStaticGuildURL->SetText(g_kGlobalValue.m_kGuildInfo.m_strURL);

	if( g_kGlobalValue.m_kGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AUTO_ACCEPT )
		m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY));
	else if(g_kGlobalValue.m_kGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AFTER_DECISION )
		m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER));
	else
		m_pkStaticJoinUsage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP));

	SetEditModeGuildName(false);
	SetEditModeGuildURL(false);

	SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);	
	m_pkGuildMarkIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);


	m_pkMarkChangeItemBlind->ToggleRender(true);
	m_pkNameChangeItemBlind->ToggleRender(true);

	std::set<int>::iterator sIter = m_sSettingChangeItem.m_setNameChangeItemID.begin();
	while(sIter != m_sSettingChangeItem.m_setNameChangeItemID.end())
	{
		if(g_pItemMgr->FindInventoryForItemID(*sIter))
		{
			m_pkNameChangeItemBlind->ToggleRender(false);
		}
		sIter++;
	}
	sIter = m_sSettingChangeItem.m_setMarkChangeItemID.begin();
	while(sIter != m_sSettingChangeItem.m_setMarkChangeItemID.end())
	{
		if(g_pItemMgr->FindInventoryForItemID(*sIter))
		{
			m_pkMarkChangeItemBlind->ToggleRender(false);
		}
		sIter++;
	}
}

void KGCGuildSettingChangeDlg::LoadSettingChangeItem()
{
	KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "GuildSettingNeedItem.lua") == false)
	{
		return;
	}

	m_sSettingChangeItem.Init();

	LUA_BEGIN_TABLE("GuildSettingNeedItemList",return);
	{
		LUA_BEGIN_TABLE("GuildNameChangeItem",;)
		{
			for(int i=1;1;i++)
			{
				int iNameChangeItemID =0;
				LUA_GET_VALUE_NOASSERT(i, iNameChangeItemID, break );
				m_sSettingChangeItem.m_setNameChangeItemID.insert(iNameChangeItemID);
			}
		}
		LUA_END_TABLE(;);
		LUA_BEGIN_TABLE("GuildMarkChangeItem",;)
		{
			for(int i=1;1;i++)
			{
				int iMarkChangeItemID =0;
				LUA_GET_VALUE_NOASSERT(i, iMarkChangeItemID, break );
				m_sSettingChangeItem.m_setMarkChangeItemID.insert(iMarkChangeItemID);
			}
		}
		LUA_END_TABLE(;);
	}
	LUA_END_TABLE(return);

}

void KGCGuildSettingChangeDlg::PostChildDraw( void )
{
	 // #if defined( _DEBUG )
	 	//KLuaManager luaMgr;
	 	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
	 
	 	//float x1 = 0.0f, y1 = 1.0f;
	 	//LUA_GET_VALUE_DEF( "x1", x1, 1.0f );
	 	//LUA_GET_VALUE_DEF( "y1", y1, 1.0f );
		//float fScale =0.0f;
		//LUA_GET_VALUE_DEF( "scale", fScale, 1.0f );
	 //#endif

	//수량표시

	//D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();

	////wow magic number!!!
	//D3DXVECTOR2 vPos1( 177 * m_fWindowScaleX, 297 * m_fWindowScaleY);
	//
	//if (m_pkNameChangeItemBlind->IsRenderOn()==false)
	//	g_pkUIMgr->RenderNumberScale("orange_gradation_num", vPos + vPos1,(DWORD)g_pItemMgr->GetInventoryItemDuration(m_sSettingChangeItem.m_iNameChangeItemID), 0.5f);

	//D3DXVECTOR2 vPos2( 440 * m_fWindowScaleX, 297 * m_fWindowScaleY);
	//if (m_pkMarkChangeItemBlind->IsRenderOn()==false)
	//	g_pkUIMgr->RenderNumberScale("orange_gradation_num", vPos + vPos2,(DWORD)g_pItemMgr->GetInventoryItemDuration(m_sSettingChangeItem.m_iMarkChangeItemID), 0.5f);
}
