#include "StdAfx.h"
#include "KGCGuildSquareMenu.h"

IMPLEMENT_CLASSNAME( KGCGuildSquareMenu );
IMPLEMENT_WND_FACTORY( KGCGuildSquareMenu );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildSquareMenu, "gc_guild_square_menu" );

KGCGuildSquareMenu::KGCGuildSquareMenu(void)
{
	char str[MAX_PATH] = {0,};
	for ( int i = 0; i < MAX_BTN_NUM; ++i )
	{
		m_pkTitleMenu[i] = NULL;
		sprintf_s( str, "btn_menu_title%d", i );
		LINK_CONTROL( str, m_pkTitleMenu[i] );

		m_pkBtnMenu[i] = NULL;
		sprintf_s( str, "btn_menu_box%d", i );
		LINK_CONTROL( str, m_pkBtnMenu[i] );
	}
}

KGCGuildSquareMenu::~KGCGuildSquareMenu(void)
{
}


void KGCGuildSquareMenu::OnCreate( void )
{
    
#if defined (GUILD_RENEW_S4 )
    for( int i = BTN_GUILD_INFO; i < MAX_BTN_NUM; i++ )
    {
        m_pkTitleMenu[i]->InitState(true);
        m_pkBtnMenu[i]->InitState(false,true,this);
    }
#else
    for( int i = 1; i < MAX_BTN_NUM; i++ )
    {
        m_pkTitleMenu[i]->InitState(true);
        m_pkBtnMenu[i]->InitState(false,true,this);
    }
#endif
}

void KGCGuildSquareMenu::OnCreateComplete( void )
{
}

void KGCGuildSquareMenu::ActionPerformed( const KActionEvent& event )
{
}

void KGCGuildSquareMenu::OnDestroyComplete( void )
{
}



bool UserIsGuildManager()
{
	return	(g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER) || 
			(g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL) ;
}

void KGCGuildSquareMenu::FrameMoveInEnabledState( void )
{
#if defined (GUILD_RENEW_S4 )
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

    bool	bIsBound		=	false;
    int		iBoundWndIdx	=	0;	

    for( int i = BTN_GUILD_INFO; i < MAX_BTN_NUM; ++i )
    {
        m_pkBtnMenu[i]->ToggleRender(false);
        if( m_pkTitleMenu[i]->CheckPosInWindowBound(vMousePos) ){
            m_pkBtnMenu[i]->ToggleRender(true);

            if( g_pkInput->BtnUp(KInput::MBLEFT) ){
                bIsBound		=	true;
                iBoundWndIdx	=	i;
            }
        }
    }

    if( bIsBound ){
        g_KDSound.Play( "31" );
        this->ToggleRender(false);

        KD3DWnd*	pBoundWnd	=	m_pkBtnMenu[iBoundWndIdx];
        pBoundWnd->ToggleRender(true);

        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_DLG);

        KP2P::GetInstance()->Send_GuildNoticeListReq();
        if( g_kGlobalValue.m_mapNGuildUserInfo.empty() ){
            KP2P::GetInstance()->Send_GuildMemberListReq();
        }

        if(			BTN_GUILD_INFO	==	iBoundWndIdx ){
            g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_INFO_TAB);
        }
        else if(	BTN_GUILD_MEMBER ==	iBoundWndIdx ){
            g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_MEMBER_TAB);
        }
        else if(	BTN_GUILD_MANAGE ==	iBoundWndIdx ){

            if( UserIsGuildManager() ){
                g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_MANAGE_TAB);
            }
            else{
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE),
                    g_pkStrLoader->GetString(STR_ID_NEW_GUILD_SQUARE_MENU_LIMIT_MSG), KGCUIScene::GC_MBOX_USE_NORMAL, 
                    KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
            }
        }

        KP2P::GetInstance()->Send_GuildPointReq();
    }
#else
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    for( int i = 1; i < MAX_BTN_NUM; ++i )
    {
        m_pkBtnMenu[i]->ToggleRender(false);
        if( m_pkTitleMenu[i]->CheckPosInWindowBound(vMousePos) )
        {
            m_pkBtnMenu[i]->ToggleRender(true);

            if( m_pkBtnMenu[i]->CheckPosInWindowBound(vMousePos) && g_pkInput->BtnUp(KInput::MBLEFT))
            {
                g_KDSound.Play( "31" );
                this->ToggleRender(false);
                if( i != BTN_GUILD_MATCH )
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_DLG);
                    KP2P::GetInstance()->Send_GuildNoticeListReq();

                    if( g_kGlobalValue.m_mapNGuildUserInfo.empty() )
                    {
                        KP2P::GetInstance()->Send_GuildMemberListReq();
                    }
                    if( i == BTN_GUILD_INFO )
                        g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_INFO_TAB);						
                    else if( i == BTN_GUILD_MEMBER )
                        g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_MEMBER_TAB);
                    else if( i == BTN_GUILD_MANAGE )
                    {
                        if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER || 
                            g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL )
                        {
                            g_pkUIScene->m_pkGuildDlg->SwapGuildTab(KGCGuildDlg::GUILD_MANAGE_TAB);
                        }
                        else
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE),
                                g_pkStrLoader->GetString(STR_ID_NEW_GUILD_SQUARE_MENU_LIMIT_MSG), KGCUIScene::GC_MBOX_USE_NORMAL, 
                                KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
                        }
                    }
                    KP2P::GetInstance()->Send_GuildPointReq();
                }
                else
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE),
                        g_pkStrLoader->GetString(STR_ID_NEW_GUILD_BATTLE_COMMING_SOON), KGCUIScene::GC_MBOX_USE_NORMAL, 
                        KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
                }
            }
        }
    }
#endif
}

void KGCGuildSquareMenu::PostChildDraw( void )
{
}