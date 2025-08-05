#include "StdAfx.h"
#include "KGCGuildMemberInfo.h"

IMPLEMENT_CLASSNAME( KGCGuildMemberInfo );
IMPLEMENT_WND_FACTORY( KGCGuildMemberInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMemberInfo, "gc_guild_member_info" );

KGCGuildMemberInfo::KGCGuildMemberInfo(void)
:m_pkGuildMaster(NULL)
,m_pkGuildGeneral(NULL)
,m_pkGradeMark(NULL)
,m_pkStaticNickName(NULL)
,m_pkStaticIntroduce(NULL)
,m_pkStaticActivity(NULL)
,m_pkStaticLastConnection(NULL)
,m_pkStaticManage(NULL)
,m_pkBtnManagePopUp(NULL)
,m_pkManagePopUpMenu(NULL)
,m_pkSelectSignBox(NULL)
,m_bUsePopUpMenu(false)
,m_dwUserUID(0)
{
    LINK_CONTROL("guild_master",			m_pkGuildMaster );
    LINK_CONTROL("guild_general",			m_pkGuildGeneral );
    LINK_CONTROL("grade_mark",				m_pkGradeMark );

    LINK_CONTROL("static_nickname",			m_pkStaticNickName );
    LINK_CONTROL("static_introduce",		m_pkStaticIntroduce );
	LINK_CONTROL("static_activity",			m_pkStaticActivity );	
	LINK_CONTROL("static_management",		m_pkStaticManage );
	LINK_CONTROL("static_last_connection",	m_pkStaticLastConnection );
	

	LINK_CONTROL("button_management_popup",	m_pkBtnManagePopUp );
	LINK_CONTROL("popupbox",				m_pkManagePopUpMenu );

	LINK_CONTROL("bg_list_select",			m_pkSelectSignBox );
}

KGCGuildMemberInfo::~KGCGuildMemberInfo(void)
{
}


void KGCGuildMemberInfo::OnCreate( void )
{
	m_pkSelectSignBox->InitState(false);
    m_pkGuildMaster->InitState( false );
    m_pkGuildGeneral->InitState( false );
	m_pkGradeMark->InitState( false );
    //m_pkGradeMark->InitState( true );
    m_pkStaticNickName->InitState( true );
    m_pkStaticNickName->SetShadowColor(0xff623e00);
    m_pkStaticIntroduce->InitState( true );
    m_pkStaticIntroduce->SetShadowColor(0xff623e00);
	m_pkStaticIntroduce->SetWordWrap(true);
	m_pkStaticActivity->InitState( true );
	m_pkStaticActivity->SetShadowColor(0xff623e00);
	m_pkStaticLastConnection->InitState( true );
	m_pkStaticLastConnection->SetShadowColor(0xff623e00);
	m_pkStaticManage->InitState( false );
	m_pkStaticManage->SetShadowColor(0xff623e00);

	m_pkBtnManagePopUp->InitState(false, true, this);
	m_pkManagePopUpMenu->InitState( false, true, this );
	m_pkManagePopUpMenu->ClearMenu();

	//어순 땜에 출력해야 할 단어의 위치기 틀리다. 한 스트링으로 쓸걸 그랬나;;; 근데 말 하나 차이 땜에 또 스트링 추가할려니 뭐해서 
	//이렇게 해둠... 일단 어순이 같은 나라는 디파인 처리 해둠.
#if defined(NATION_BRAZIL) || defined(NATION_USA) || defined(NATION_PHILIPPINE ) || defined(NATION_THAILAND) || defined( NATION_EU )
	m_pkManagePopUpMenu->AddMenu( 1, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_COMMITION) + L" " + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION) );
	m_pkManagePopUpMenu->AddMenu( 2, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_COMMITION) + L" " + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GENERAL_COMMITION) );
	m_pkManagePopUpMenu->AddMenu( 3, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_COMMITION) + L" " + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NORMAL_COMMITION) );
	m_pkManagePopUpMenu->AddMenu( 4, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_EXPULSION) );
#else
	m_pkManagePopUpMenu->AddMenu( 1, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION));
	m_pkManagePopUpMenu->AddMenu( 2, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GENERAL_COMMITION));
	m_pkManagePopUpMenu->AddMenu( 3, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NORMAL_COMMITION));
	m_pkManagePopUpMenu->AddMenu( 4, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_EXPULSION));
#endif
}

void KGCGuildMemberInfo::OnCreateComplete( void )
{
}

void KGCGuildMemberInfo::ActionPerformed( const KActionEvent& event )
{
	if( !m_bUsePopUpMenu )
		return; 

	if( event.m_pWnd == m_pkBtnManagePopUp && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( m_pkManagePopUpMenu->IsRenderOn() )
			m_pkManagePopUpMenu->ToggleRender(false);
		else
			m_pkManagePopUpMenu->ToggleRender(true);
		g_KDSound.Play( "31" );
	}

	if( event.m_pWnd == m_pkManagePopUpMenu && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
	{
		switch( event.m_dwlParam )
		{
		case EMG_MASTER:
			SetGuildGrade(KNGuildUserInfo::GL_MASTER);			
			SetChangeGuildMemberGrade(KNGuildUserInfo::GL_MASTER);
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION) + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_COMMITION), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION_MSG1), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
			break;
		case EMG_GENERAL:
			SetGuildGrade(KNGuildUserInfo::GL_GENERAL);
			SetChangeGuildMemberGrade(KNGuildUserInfo::GL_GENERAL);
			break;
		case EMG_BREAK_UP:	
			SetGuildGrade(KNGuildUserInfo::GL_DRUMOUT);
			SetChangeGuildMemberGrade(KNGuildUserInfo::GL_DRUMOUT);
			break;		
		case EMG_NORMAL:
			SetGuildGrade(KNGuildUserInfo::GL_NORMAL);
			SetChangeGuildMemberGrade(KNGuildUserInfo::GL_NORMAL);
			break;
		}	
		g_KDSound.Play( "31" );
	}
}

void KGCGuildMemberInfo::OnDestroyComplete( void )
{
}

void KGCGuildMemberInfo::FrameMoveInEnabledState( void )
{
}

void KGCGuildMemberInfo::PostChildDraw( void )
{
}

void KGCGuildMemberInfo::SetUserInfo( const KNGuildUserInfo& info_ )
{
	WCHAR strText[20];
	swprintf(strText, 20, L"%d.%d.%d", (int)info_.m_kLastLoginDate.m_sYear, info_.m_kLastLoginDate.m_cMonth, info_.m_kLastLoginDate.m_cDay);	
	m_pkStaticLastConnection->SetText(strText);	
	m_pkStaticActivity->SetText(info_.m_dwContributePoint);
    //m_pkStaticActivity->SetText ( (int)g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint );
    m_pkStaticActivity->SetAlign( DT_CENTER );
	std::wstring strNickName = info_.m_strNickName;
	std::wstring strNickColor = info_.m_strNickColor;
	if (strNickColor == L"")
		strNickColor = L"FFFFFF";
	GCUTIL_STR::Trim( strNickName );
	m_pkStaticNickName->SetForceWordWrapText(L"#c" + strNickColor + strNickName + L"#cx", true);
	m_pkStaticNickName->SetToolTip(info_.m_strNickName);
	//m_pkGradeMark->SetGrade( info_.m_ucCharacterGrade);
	if( !m_bUsePopUpMenu )
	{
		CString src = info_.m_strMyComment.c_str();
		src.Replace(L"\n", L" ");
		std::wstring strConvertor(src);
		m_pkStaticIntroduce->SetForceWordWrapText(strConvertor, true);
	}

	// 길드마스터이면 자신의 팝업메뉴를 사용하지 않겠다. 길드마스터가 아니면 전유저의 팝업유저메뉴를 사용하지 않는다.
	if((info_.m_cMemberLevel == KNGuildUserInfo::GL_MASTER && m_bUsePopUpMenu) || g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL )
		m_pkBtnManagePopUp->ToggleRender(false);

	m_dwUserUID = info_.m_dwUserUID;
    
    bool iIsSameUserID = false;
	std::map<char, std::set<DWORD>>::iterator mit = g_kGlobalValue.m_mapNGuildChangeMemberGrade.find(KNGuildUserInfo::GL_DRUMOUT);
    if( mit != g_kGlobalValue.m_mapNGuildChangeMemberGrade.end() )
    {
        std::set<DWORD>::iterator sit = mit->second.find(m_dwUserUID);
        if( sit != mit->second.end() )
            if ( m_dwUserUID == *sit )
                iIsSameUserID = true;
    }
    
    int iChangeMemberGrade = -1;
    if( !g_kGlobalValue.m_mapNGuildChangeMemberGrade.empty() )
        iChangeMemberGrade = GetChangeMemberGrade(m_dwUserUID);
     
    if( iChangeMemberGrade == -1 )
    {
        if ( iIsSameUserID )
            SetGuildGrade( iChangeMemberGrade );  
        else
            SetGuildGrade( (int)info_.m_cMemberLevel );
    }
    else
        SetGuildGrade(iChangeMemberGrade);
}

void KGCGuildMemberInfo::SetGuildGrade( int iUserLevel_ )
{
    m_pkGuildMaster->ToggleRender( iUserLevel_ == KNGuildUserInfo::GL_MASTER );
    m_pkGuildGeneral->ToggleRender( iUserLevel_ == KNGuildUserInfo::GL_GENERAL );

	if(m_bUsePopUpMenu)
	{
		switch(iUserLevel_)
		{
		case KNGuildUserInfo::GL_DRUMOUT:
			m_pkStaticManage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_EXPULSION));
			break;
		case KNGuildUserInfo::GL_GENERAL:
			m_pkStaticManage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GENERAL_COMMITION));
			break;
		case KNGuildUserInfo::GL_MASTER:
			m_pkStaticManage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION));
			break;
		case KNGuildUserInfo::GL_NORMAL:
		default:
			m_pkStaticManage->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NORMAL_COMMITION));
			break;
		}
	}
}

void KGCGuildMemberInfo::UsePopUpMenu( bool bUse_ )
{
	m_pkStaticIntroduce->ToggleRender( !bUse_ );
	m_pkStaticManage->ToggleRender( bUse_ );
	m_pkBtnManagePopUp->ToggleRender(bUse_);
	m_bUsePopUpMenu = bUse_;
}

void KGCGuildMemberInfo::SetSelectCursorBox( bool bRender_ )
{
	m_pkSelectSignBox->ToggleRender(bRender_);
}

bool KGCGuildMemberInfo::IsSelectListBoxRender()
{
	return m_pkSelectSignBox->IsRenderOn();
}

void KGCGuildMemberInfo::SetRenderPopupMenu( bool bRender_ )
{
	m_pkManagePopUpMenu->ToggleRender(bRender_);
}

void KGCGuildMemberInfo::SetChangeGuildMemberGrade( UCHAR ucMemberGrade_ )
{
    static KNGuildUserInfo::GuildUserAuthLevel arrEnAuthLevel[] = {
        KNGuildUserInfo::GL_DRUMOUT, 
        KNGuildUserInfo::GL_JOINER, 
        KNGuildUserInfo::GL_NORMAL, 
        KNGuildUserInfo::GL_GENERAL,
        KNGuildUserInfo::GL_MASTER, 
    };

    for( size_t i=0; i<sizeof(arrEnAuthLevel)/sizeof(KNGuildUserInfo::GuildUserAuthLevel); ++i ) {

        std::map<char, std::set<DWORD>>::iterator mit = g_kGlobalValue.m_mapNGuildChangeMemberGrade.find( static_cast<int>(arrEnAuthLevel[i]) );
        if( ucMemberGrade_ != static_cast<int>(arrEnAuthLevel[i]) )
        {
            std::map<char, std::set<DWORD>>::iterator mit = g_kGlobalValue.m_mapNGuildChangeMemberGrade.find(KNGuildUserInfo::GL_MASTER);
            if( mit != g_kGlobalValue.m_mapNGuildChangeMemberGrade.end() )
                g_kGlobalValue.m_mapNGuildChangeMemberGrade[static_cast<int>(arrEnAuthLevel[i])].erase(m_dwUserUID);
        }
    }   

	g_kGlobalValue.m_mapNGuildChangeMemberGrade[(int)ucMemberGrade_].insert(m_dwUserUID);
}

int KGCGuildMemberInfo::GetChangeMemberGrade(DWORD dwMemberUID_)
{
    static KNGuildUserInfo::GuildUserAuthLevel arrEnAuthLevel[] = {
        KNGuildUserInfo::GL_DRUMOUT,
        KNGuildUserInfo::GL_JOINER, 
        KNGuildUserInfo::GL_NORMAL, 
        KNGuildUserInfo::GL_GENERAL, 
        KNGuildUserInfo::GL_MASTER, 
    };

    for( size_t i=0; i<sizeof(arrEnAuthLevel)/sizeof(KNGuildUserInfo::GuildUserAuthLevel); ++i ) {

        std::map<char, std::set<DWORD>>::iterator mit = g_kGlobalValue.m_mapNGuildChangeMemberGrade.find( static_cast<int>(arrEnAuthLevel[i]) );
        if( mit != g_kGlobalValue.m_mapNGuildChangeMemberGrade.end() )
        {   		
            std::set<DWORD>::iterator sit = mit->second.find(dwMemberUID_);
            if( sit != mit->second.end() )
                return static_cast<int>( arrEnAuthLevel[i] );
        }
    } 
   
	return -1;
}