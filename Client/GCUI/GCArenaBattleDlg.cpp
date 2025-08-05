#include "stdafx.h"
#include "GCArenaBattleDlg.h"
//
//
//
#include "../MyD3D.h"


//
//
//
//
#include "KGCGameBoard.h"
#include "../GCStateMachine.h"
#include "KGCRoomManager.h"

/*
사실은 게임 내에서 여러 Dialog를 컨트롤 하려고 만들었는데
튜토리얼의 Custom한 Dialog가 되고 말았다-_-;;
*/
IMPLEMENT_CLASSNAME( KGCArenaBattleDlg );
IMPLEMENT_WND_FACTORY( KGCArenaBattleDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCArenaBattleDlg, "gc_arenabattledlg" );

class CSortKillDie
{
public:
    bool operator()( const Player_Work& p, const Player_Work& q ) const
    { 
		if( p.How_Kill > q.How_Kill )
			return true;
		else if( p.How_Kill < q.How_Kill )
			return false;
		else if( p.How_Die < q.How_Die )
			return true;
		else
			return false;
	}
};

KGCArenaBattleDlg* g_pkArenaBattleDlg = NULL;

KGCArenaBattleDlg::KGCArenaBattleDlg( void )
{
    g_pkArenaBattleDlg  =   this;
    m_pkScoreBoard    =   NULL;
    for ( int i = 0; i < MAX_PLAYER_NUM + 2; i++ )
        m_apkKillDeathStatic[i] = NULL;

    m_uiStrCount        =   0;
    LINK_CONTROL( "scoreboard",         m_pkScoreBoard );
    LINK_CONTROL( "killdeathstatic0",   m_apkKillDeathStatic[0] );
    LINK_CONTROL( "killdeathstatic1",   m_apkKillDeathStatic[1] );
    LINK_CONTROL( "killdeathstatic2",   m_apkKillDeathStatic[2] );
    LINK_CONTROL( "killdeathstatic3",   m_apkKillDeathStatic[3] );
    LINK_CONTROL( "killdeathstatic4",   m_apkKillDeathStatic[4] );
    LINK_CONTROL( "killdeathstatic5",   m_apkKillDeathStatic[5] );
    LINK_CONTROL( "killdeathstatic6",   m_apkKillDeathStatic[6] );
    LINK_CONTROL( "killdeathstatic7",   m_apkKillDeathStatic[7] );
}

KGCArenaBattleDlg::~KGCArenaBattleDlg( void )
{
}

void KGCArenaBattleDlg::OnCreate( void )
{
    Init();
}
void KGCArenaBattleDlg::Init()
{
    for ( int i = 0; i < MAX_PLAYER_NUM + 2; i++ )
        m_apkKillDeathStatic[i]->ToggleRender( false );
}
void KGCArenaBattleDlg::OnDestroy( void )
{
}

void KGCArenaBattleDlg::FrameMoveInEnabledState( void )
{
}

void KGCArenaBattleDlg::PostChildDraw( void )
{
}

void KGCArenaBattleDlg::AddString(  std::wstring strPlayerName_, int iKills, int iDeath, DWORD dwColor_ )
{
    if ( m_uiStrCount == MAX_PLAYER_NUM + 2 )
        return;   
    WCHAR strKillAndDeath[32] = {0,};
    WCHAR strPlayerName[MAX_NICKNAME_SIZE + 32] = {0,}; // 닉네임 크기보다 넉넉하게 잡아 놓는다.
    swprintf( strPlayerName, MAX_NICKNAME_SIZE + 32, L" %s", strPlayerName_.c_str() );
    swprintf( strKillAndDeath, 32, L"%2u/%2u", iKills, iDeath );
    m_apkKillDeathStatic[m_uiStrCount]->SetText( strPlayerName );
    m_apkKillDeathStatic[m_uiStrCount]->SetFontColor( dwColor_ );
    ((KD3DStatic*)m_apkKillDeathStatic[m_uiStrCount]->GetChild(0))->SetText( strKillAndDeath );
    ((KD3DStatic*)m_apkKillDeathStatic[m_uiStrCount]->GetChild(0))->SetFontColor( dwColor_ );
	m_apkKillDeathStatic[m_uiStrCount]->GetChild(0)->ToggleRender( true );
    m_apkKillDeathStatic[m_uiStrCount]->ToggleRender( true );
    m_uiStrCount++;
}

void KGCArenaBattleDlg::AddTeamSection( std::wstring strTeam_, DWORD dwColor_, UINT uiTeam_ )
{
    if ( m_uiStrCount == MAX_PLAYER_NUM + 2 )
        return;   
    WCHAR strKillAndDeath[32] = {0,};
    int iKills = 0, iDeath = 0;
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) && g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == (int)uiTeam_)
        {
            iKills += g_pGameBoard->m_pStartingMember[i].How_Kill;
            iDeath += g_pGameBoard->m_pStartingMember[i].How_Die;
        }
    }
    swprintf( strKillAndDeath, 32, L"%2u/%2u", iKills, iDeath );
    m_apkKillDeathStatic[m_uiStrCount]->SetText( strTeam_ );
    m_apkKillDeathStatic[m_uiStrCount]->SetFontColor( dwColor_ );
    ((KD3DStatic*)m_apkKillDeathStatic[m_uiStrCount]->GetChild(0))->SetText( strKillAndDeath );
    ((KD3DStatic*)m_apkKillDeathStatic[m_uiStrCount]->GetChild(0))->SetFontColor( dwColor_ );
	m_apkKillDeathStatic[m_uiStrCount]->GetChild(0)->ToggleRender( true );
    m_apkKillDeathStatic[m_uiStrCount]->ToggleRender( true );
    m_uiStrCount++;
}
#define             BLUE_TEAM               1
#define             RED_TEAM                0
void KGCArenaBattleDlg::UpdateDlg()
{
	//게임 도중이 아닌데..부르면 안되삼.
	if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
		return;

    m_uiStrCount = 0;
    Init();
	
    std::vector<Player_Work> vecPlayerWork;
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        vecPlayerWork.push_back( g_pGameBoard->m_pStartingMember[i] );
    }
    std::sort( vecPlayerWork.begin(), vecPlayerWork.end(), CSortKillDie() );

    if ( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT )
    {
        g_pkArenaBattleDlg->AddTeamSection( g_pkStrLoader->GetString( STR_ID_BLUE_TEAM ), D3DCOLOR_ARGB(255,168,166,255), BLUE_TEAM );
        for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            SUserInfo kUserInfo = g_MyD3D->MyPlayer[i]->m_kUserInfo;
            if ( kUserInfo.bLive == true )
            {
                if ( kUserInfo.iTeam == BLUE_TEAM )
                {
                    if ( kUserInfo.dwUID == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.dwUID )
                    {
                        g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFF00 );
                    }
                    else
                    {
                        g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFFFF );
                    }
                }
            }
        }
        g_pkArenaBattleDlg->AddTeamSection( g_pkStrLoader->GetString( STR_ID_RED_TEAM ), D3DCOLOR_ARGB(255,224,113,40), RED_TEAM );
        for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            SUserInfo kUserInfo = g_MyD3D->MyPlayer[i]->m_kUserInfo;
            if ( kUserInfo.bLive == true )
            {
                if ( kUserInfo.iTeam == RED_TEAM )
                {
                    if ( kUserInfo.dwUID == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.dwUID )
                    {
                        g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFF00 );
                    }
                    else
                    {
                        g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFFFF );
                    }
                }
            }
        }
    }
    else if ( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
    {
        for( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            SUserInfo kUserInfo = g_MyD3D->MyPlayer[ i ]->m_kUserInfo;
            if ( kUserInfo.bLive == true )
            {
                if ( kUserInfo.dwUID == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.dwUID )
                {
                    g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFF00 );
                }
                else
                {
                    g_pkArenaBattleDlg->AddString( kUserInfo.strNickName.c_str(), vecPlayerWork[i].How_Kill, vecPlayerWork[i].How_Die, 0xFFFFFFFF );
                }
            }
        }
    }    
}
