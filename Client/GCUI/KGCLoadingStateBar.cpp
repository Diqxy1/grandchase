#include "StdAfx.h"
#include "KGCLoadingStateBar.h"


IMPLEMENT_CLASSNAME( KGCLoadingStateBar );
IMPLEMENT_WND_FACTORY( KGCLoadingStateBar );
IMPLEMENT_WND_FACTORY_NAME( KGCLoadingStateBar, "gc_loading_state_bar" );

KGCLoadingStateBar::KGCLoadingStateBar(void)
{
    m_pkLoadingGuage			= NULL;
    m_pkUserNickName		    = NULL;
    m_pkLoadingPercent          = NULL;
    LINK_CONTROL( "loading_bar_guage",			m_pkLoadingGuage );
    LINK_CONTROL( "static_user_nickname",		m_pkUserNickName );
    LINK_CONTROL( "static_loading_percent", 	m_pkLoadingPercent );

    m_pkIconBack = NULL;
    m_pkIconBackActive = NULL;
    LINK_CONTROL( "icon_back",			m_pkIconBack );
    LINK_CONTROL( "icon_back_active",	m_pkIconBackActive );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_pkCharIcon[i] = NULL;
        char temp[128];
        sprintf_s( temp, 127, "char_icon%d", i );
        LINK_CONTROL( temp, m_pkCharIcon[i] );
    }
}

KGCLoadingStateBar::~KGCLoadingStateBar(void)
{
}

void KGCLoadingStateBar::OnCreate( void )
{
    m_pkLoadingGuage->InitState(true);

    m_pkUserNickName->InitState(true);
    m_pkUserNickName->SetAlign(DT_CENTER);
    m_pkUserNickName->SetText(L"");

    m_pkLoadingPercent->InitState(true);
    m_pkLoadingPercent->SetAlign(DT_CENTER);
    m_pkLoadingPercent->SetText(L"");

    m_pkIconBack->InitState( false );
    m_pkIconBackActive->InitState( false );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_pkCharIcon[i]->InitState( false );
    }
}

void KGCLoadingStateBar::FrameMoveInEnabledState( void )
{
}


// 던전 게임시작 시 자신의 로딩진행상태를 셋팅
void KGCLoadingStateBar::SetLoadingState( DWORD dwUserUID_, int iLoadingStete_ )
{
    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(dwUserUID_);   

    float fRate = 9.2f; // 로딩을 위한 과정을 18단계로 구분하게 되면 곱할 비율이 9.2이 나옴
    int iMaxLoadingGuage = (int)EMAX_LOADING_GUAGE_DUNGEON;
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
    {
        fRate = 6.f;
        iMaxLoadingGuage = (int)EMAX_LOADING_GUAGE_MATCH;
    }
    int iLoadingPercnt = (int)((float)iLoadingStete_ * fRate); 
    int iGuageWeight = (int)EMIN_LOADING_GUAGE;

    if( iLoadingPercnt > 100 )
        iLoadingPercnt = 100; 

    iGuageWeight = (int)(iLoadingStete_ * ((float)iMaxLoadingGuage/100.f) * fRate);

    if( iGuageWeight < EMIN_LOADING_GUAGE )
        iGuageWeight = (int)EMIN_LOADING_GUAGE;

    if( iGuageWeight > iMaxLoadingGuage )
        iGuageWeight = iMaxLoadingGuage;       

    if( pPlayer ) { 
        // 캐릭터 아이콘 
        int iCurrentChar = pPlayer->GetCurrentChar().iCharType;
        for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
            if( i == iCurrentChar ) { 
                m_pkCharIcon[i]->ToggleRender( true );
            }
            else { 
                m_pkCharIcon[i]->ToggleRender( false );
            }
        }

        // 내껀지 아닌지
        bool bMyGuage = g_kGlobalValue.m_kUserInfo.dwUID == dwUserUID_;
        m_pkIconBackActive->ToggleRender( bMyGuage );
        m_pkIconBack->ToggleRender( !bMyGuage );

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) { 
            m_pkUserNickName->SetText(L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx");
            m_pkLoadingPercent->SetText(L"");
        }
        else { 
            std::wostringstream strm;
            strm << iLoadingPercnt <<L"%";
            m_pkUserNickName->SetText(L"");
            m_pkLoadingPercent->SetText(strm.str());
        }

        m_pkLoadingGuage->SetWidth(iGuageWeight);
    }
}