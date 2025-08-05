#include "stdafx.h"
#include ".\gcresulttower.h"
#include "MyD3D.h"

#include "KGCGameBoard.h"
#include "KGCEmoticon.h"
//
#include "buff/KGCBuffManager.h"
#include "GCResultNick.h"
#include "GCResultScore.h"
#include "../KGCRoomManager.h"
#include "GCResultDamage.h"

IMPLEMENT_CLASSNAME( KGCResultTower );
IMPLEMENT_WND_FACTORY( KGCResultTower );
IMPLEMENT_WND_FACTORY_NAME( KGCResultTower, "gc_result_tower" );

KGCResultTower::KGCResultTower(void)
: m_iPlayerIndex( -1 )
, m_iTeam( 0 )
, m_iRank( 0 )
, m_iExp( 0 )
, m_iGp( 0 )
, m_iKill( 0 )
, m_iDeath( 0 )
, m_bLevelUp( false )
, m_bSPLevelUp( false )
, m_bMVP( false )
, m_bRenderExpGp( false )
, m_iAnimFrame( 0 )
, m_pkDamage(NULL)
{
    //Init();

    m_pkNickName = NULL;
    m_pkScore = NULL;
    m_pkWin = NULL;
    m_pkLose = NULL;
    m_pkLevelUp = NULL;
	m_pkSPLevelUp = NULL;	
	m_pkLocalUser = NULL;
	m_pkMVP = NULL;
    m_pkEmoticonBox = NULL;
    m_pkColorFrame[TEAM_CANABAN] = NULL;
    m_pkColorFrame[TEAM_SERDIN] = NULL;

    LINK_CONTROL( "score_frame_canaban",    m_pkColorFrame[TEAM_CANABAN] );
    LINK_CONTROL( "score_frame_serdin",     m_pkColorFrame[TEAM_SERDIN] );
    LINK_CONTROL( "score",                  m_pkScore );
    LINK_CONTROL( "nickname",               m_pkNickName );
    LINK_CONTROL( "win",                    m_pkWin );
    LINK_CONTROL( "lose",                   m_pkLose );
    LINK_CONTROL( "levelup",                m_pkLevelUp );
	LINK_CONTROL( "splevelup",              m_pkSPLevelUp );
	LINK_CONTROL( "local_player",			m_pkLocalUser );
	LINK_CONTROL( "mvp",		m_pkMVP );
    LINK_CONTROL( "emoticon",               m_pkEmoticonBox );
    LINK_CONTROL( "damage",                 m_pkDamage );

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		m_pkRankNum[i] = NULL;
		char strRank[MAX_PATH];
		sprintf( strRank, "rank%d", i + 1 );
		LINK_CONTROL( strRank,   m_pkRankNum[i] );
	}
}

KGCResultTower::~KGCResultTower(void)
{
}

void KGCResultTower::OnCreate()
{
    m_pkWin->InitState( false, false, NULL );
    m_pkLose->InitState( false, false, NULL );
    m_pkLevelUp->InitState( false, false, NULL );
	m_pkSPLevelUp->InitState( false, false, NULL );
    m_pkNickName->InitState( true, true, this );
    m_pkScore->InitState( true, true, this );
	m_pkLocalUser->InitState( false, false, NULL );
    m_pkEmoticonBox->ToggleRender( true );

	m_pkMVP->InitState( m_bMVP, false, NULL );
    m_pkColorFrame[TEAM_CANABAN]->InitState( TEAM_CANABAN == m_iTeam, true, this );
    m_pkColorFrame[TEAM_SERDIN]->InitState( TEAM_SERDIN == m_iTeam, true, this );
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		m_pkRankNum[i]->InitState( i == m_iRank , false, NULL );
	}
    m_pkDamage->InitState(true, true, this);
   //Init();
}

void KGCResultTower::Init()
{
	//변수들을 초기화 합니다.
    m_iPlayerIndex = -1;
    m_iTeam = TEAM_NUM;
    m_iExp = 0;
    m_iGp = 0;
    m_iKill = 0;
    m_iDeath = 0;
    m_bLevelUp = false;
	m_bSPLevelUp = false;
    m_bMVP = false;
	m_bRenderExpGp = false;
    m_strNick.clear();

	m_kCharacter.Clear();
	m_kCharacter.SetD3DDevice( g_pd3dDevice );
	D3DXVECTOR2 vPos = GetFixedWindowPos();
	vPos.y -= 150;
	m_kCharacter.SetViewPort( vPos, 150, 100 );

	//타워의 높이를 초기화 합니다.
	SetTowerHeight( RT_DEFAULT_HEIGHT );
	m_dwEndHeight = RT_DEFAULT_HEIGHT;
	SetTeam( TEAM_SERDIN );	//일단 팀은 무조건 세르딘으로 셋팅합니다. (색깔 땜시롱 )
	SetRank( -1 );	//랭크는 없습니다.
	m_pkNickName->Init();	//닉네임을 지웁니다.
	m_pkScore->Init();	//스코어도 지웁니다.
	m_pkLocalUser->ToggleRender( false );

	m_iAnimFrame = 0;
    m_bLevelUpEffectOn  =   false;
    m_bSkillUpEffectOn  =   false;
    m_iDamage = 0;
    m_iHealDamage = 0;
    m_pkDamage->Init();
}

void KGCResultTower::SetTeam( int iTeam )
{
    if( iTeam != TEAM_CANABAN &&
        iTeam != TEAM_SERDIN )
    {
        ASSERT( !"이상한 팀이 입력되었네요. 원인 파악 해주세요!!" );
        iTeam = TEAM_CANABAN;
    }
    m_iTeam = iTeam;
	for( int i = 0 ; i < TEAM_NUM ; ++i )
	{
		m_pkColorFrame[i]->ToggleRender( i == m_iTeam );
	}
	m_pkNickName->SetTeam( m_iTeam );
}

void KGCResultTower::SetNickName( int iLevel, std::wstring strNick )
{
    if( strNick.empty() )
    {
        ASSERT( !"닉네임이 빈 문자열이네요." );
        return;
    }

    std::wstringstream str;
    str<<g_pkStrLoader->GetString( STR_ID_LEVEL )<<iLevel<<L" "<<strNick;

    m_strNick.swap( str.str() );

    m_pkNickName->SetNick( m_strNick );
}

void KGCResultTower::SetRank( int iRank )
{
    m_iRank = iRank;

	if( m_iRank == 0 )
	{
		SetMVP( true && !IsTeamMode() );
	}

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		m_pkRankNum[i]->ToggleRender( m_iRank == i && !IsTeamMode() );
	}

	SetEndHeight( m_iRank );
}

void KGCResultTower::SetKill( int iKill )
{
    if( iKill < 0 )
    {
        ASSERT( !"음수값이 들어올 수 있나요??" );
        iKill = 0;
    }
    m_iKill = iKill;
	m_pkScore->SetKill( m_iKill );
}

void KGCResultTower::SetDeath( int iDeath )
{
    if( iDeath < 0 )
    {
        ASSERT( "음수 값이 들어올 수 있나요??" );
        iDeath = 0;
    }
    m_iDeath = iDeath;
	m_pkScore->SetDeath( m_iDeath );
}

void KGCResultTower::SetLevelUp( bool bLevelUp /*= true */ )
{
    if( false == m_bLevelUp && bLevelUp ){
        //  m_iPlayerIndex
        //  여기가 최초 한번 들어오는 곳
        m_bLevelUpEffectOn = true;
    }
    m_bLevelUp = bLevelUp;

#if !defined( NEW_LEVELUP_EFFECT )
	m_pkLevelUp->ToggleRender( bLevelUp );
#endif
}

void KGCResultTower::SetSPLevelUp(  int iSPLevelUp )
{
    if( false == m_bSPLevelUp && iSPLevelUp>0 ){
        //  여기가 최초 한번 들어오는 곳
        m_bSkillUpEffectOn = true;
    }
	m_bSPLevelUp = iSPLevelUp>0;

#if !defined( NEW_LEVELUP_EFFECT )
	m_pkSPLevelUp->ToggleRender( m_bSPLevelUp );
#endif
}

void KGCResultTower::SetMVP( bool bMVP /*= true */ )
{
    m_bMVP = bMVP;
	m_pkMVP->ToggleRender( m_bMVP );
}

void KGCResultTower::SetPlayerIndex( int iIndex )
{
    if( iIndex < 0 || iIndex >= MAX_PLAYER_NUM )
    {
        ASSERT( !" 인덱스가 이상합니다." );
        iIndex = 0;
    }
    m_iPlayerIndex = iIndex;
    SetNickName( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.GetCurrentChar().iLevel, g_MyD3D->MyPlayer[m_iPlayerIndex]->GetStrUserName() );
	SetCharacter( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo );
	SetRank( g_pGameBoard->GetRank(m_iPlayerIndex) );
	SetKill( g_pGameBoard->GetHowKill( m_iPlayerIndex ) );
	SetDeath( g_pGameBoard->GetHowDie( m_iPlayerIndex ) );
    //테스트 코드 
    SetDamage( static_cast<int> ( g_pGameBoard->GetTotalDamage( m_iPlayerIndex )) );

	if( IsTeamMode() )
	{
		SetTeam( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.iTeam );
		SetWin( g_pGameBoard->IsWin( iIndex ) );
		
		if( g_pGameBoard->IsWin( iIndex ) == true )
			m_dwEndHeight = RT_DEFAULT_HEIGHT + RT_HEIGHT_TERM * 6;	//이긴 팀이 높이 올라간다.
		else
			m_dwEndHeight = RT_DEFAULT_HEIGHT + RT_HEIGHT_TERM * 2; //진팀은 낮게 올라간다.
	}

	//SetMotionToChar( m_iPlayerIndex , ( !IsTeamMode() || g_pGameBoard->IsWin( iIndex ) ) );
    SetMotionToChar( m_iPlayerIndex , ( g_pGameBoard->IsWin( iIndex ) ) );
	m_pkLocalUser->ToggleRender( g_MyD3D->MyPlayer[m_iPlayerIndex]->IsLocalPlayer() );
	g_MyD3D->MyPlayer[m_iPlayerIndex]->ToggleRenderPlayer( false );
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex);
}

void KGCResultTower::PostDraw()
{

    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
    m_kCharacter.Draw();
    // 이모티콘을 ㎢摸?! 이모티콘 그려주쟈!
    if ( m_kCharacter.GetPlayer().GetEmoticon() )
    {
        m_kCharacter.GetPlayer().GetEmoticon()->SetPos( &D3DXVECTOR2( 1.2f, 0.48f ) );
        //m_kCharacter.GetPlayer().GetEmoticon()->SetPos( &D3DXVECTOR2( m_kCharacter.GetPlayer().m_pObject->GetWorldMat()->_41, m_kCharacter.GetPlayer().m_pObject->GetWorldMat()->_42 ) );
        m_kCharacter.GetPlayer().GetEmoticon()->Render();
    }
}

void KGCResultTower::SetCharacter( SUserInfo& sUserInfo )
{
    m_kCharacter.GetPlayer().SetResultTower( true );
    m_kCharacter.SetPlayerInfo( sUserInfo );
    sUserInfo.GetCurrentChar();
}

void KGCResultTower::SetMotionToChar( int iPlayerIndex, bool bWin )
{
	m_kCharacter.ClearMotionList();
    SUserInfo& sUserInfo = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo;
    SCharInfo& sCharInfo = sUserInfo.GetCurrentChar();

    int iMotionID;
    if( bWin )
        iMotionID = g_MyD3D->MyPlayer[iPlayerIndex]->GetCurFormTemplate().WIN;
    else
        iMotionID = g_MyD3D->MyPlayer[iPlayerIndex]->GetCurFormTemplate().RESULT_LOSE;

    std::string strShopMotion = boost::str( boost::format("%03d.frm")%iMotionID );

    m_kCharacter.AddMotion( strShopMotion, iMotionID );
}
 
void KGCResultTower::FrameMoveInEnabledState()
{
	if( m_pkColorFrame[TEAM_SERDIN]->GetHeight() < m_dwEndHeight )
	{
		SetTowerHeight( m_pkColorFrame[TEAM_SERDIN]->GetHeight() + 1 );
		++m_iAnimFrame;
	}
	else if( m_pkColorFrame[TEAM_SERDIN]->GetHeight() > m_dwEndHeight )
	{
		SetTowerHeight( m_pkColorFrame[TEAM_SERDIN]->GetHeight() - 1 );
		++m_iAnimFrame;
	}
	else
	{
		D3DXVECTOR2 vPos = m_pkScore->GetFixedWindowPos();
		vPos.y += (m_pkScore->GetHeight() + 3 );
		vPos.x = m_pkMVP->GetFixedWindowPos().x;
		m_pkMVP->SetWindowPosDirect( vPos );
		m_pkMVP->ToggleRender( m_bMVP );
		m_bRenderExpGp = true;
		
		// 레벨없 표시가 안 되고 있으면
		if( m_iPlayerIndex != -1  &&  m_bLevelUp == false)
		{
			SetLevelUp( g_pGameBoard->IsLevelUp( m_iPlayerIndex ) );
		}


#if defined( NEW_LEVELUP_EFFECT )
        float fx = 440.0f;
        float fy = 220.0f;

        D3DXVECTOR2 vPos_Levup = this->GetFixedWindowLocalPos();
        DWORD dwHeight = m_pkColorFrame[TEAM_SERDIN]->GetHeight();
        fy += dwHeight;

        float fXDiv = ( 400.0f * GC_SCREEN_DIV_WIDTH );
        float fYDiv = ( 405.0f * GC_SCREEN_DIV_HEIGHT );

        if( m_bLevelUpEffectOn && m_bSkillUpEffectOn ){
            g_ParticleManager->LevelUpEffect( (vPos_Levup.x - fx) / fXDiv ,  (-vPos_Levup.y + fy) / fYDiv );
            g_ParticleManager->SkillUpEffect( (vPos_Levup.x - fx) / fXDiv ,  (-vPos_Levup.y + fy + 40.0f ) / fYDiv );
        }
        else if( m_bLevelUpEffectOn ){
            g_ParticleManager->LevelUpEffect( (vPos_Levup.x - fx) / fXDiv ,  (-vPos_Levup.y + fy) / fYDiv );
        }
        else if( m_bSkillUpEffectOn ){
            g_ParticleManager->SkillUpEffect( (vPos_Levup.x - fx) / fXDiv ,  (-vPos_Levup.y + fy) / fYDiv );
        }
        m_bLevelUpEffectOn = false;
        m_bSkillUpEffectOn = false;
#endif
    }
}

void KGCResultTower::SetTowerHeight( DWORD dwHeight )
{
	for( int i = 0 ; i < TEAM_NUM ; ++i )
	{
		int iDelta = (int)m_pkColorFrame[i]->GetHeight();
		m_pkColorFrame[i]->SetHeight( dwHeight );
		iDelta -= m_pkColorFrame[i]->GetHeight();

        D3DXVECTOR2 vPos = m_pkColorFrame[i]->GetFixedWindowLocalPos();

        vPos.y += (float)iDelta;	//5는 위 아래의 Edge의 높이가 다르기 때문에..
        m_pkColorFrame[i]->SetFixedWindowLocalPos( vPos );

        D3DXVECTOR2 vPos_Scoreboard = vPos;
        vPos_Scoreboard.x += 4 * GC_SCREEN_DIV_WIDTH;	vPos_Scoreboard.y += 4 * GC_SCREEN_DIV_HEIGHT;	// 4는 내부에 들어가는 Score와의 갭
        m_pkScore->SetFixedWindowLocalPos( vPos_Scoreboard );

        D3DXVECTOR2 vPos_Damageboard = vPos;
        vPos_Damageboard.x = 0;
        vPos_Damageboard.y += 60 * GC_SCREEN_DIV_HEIGHT;
        m_pkDamage->SetFixedWindowLocalPos( vPos_Damageboard );

        D3DXVECTOR2 vPos_NickName = vPos;
        vPos_NickName.x -= 2 * GC_SCREEN_DIV_WIDTH;	vPos_NickName.y -= 27 * GC_SCREEN_DIV_HEIGHT;	// 31은 스코어와 닉네임 박스와의 갭
        m_pkNickName->SetFixedWindowLocalPos( vPos_NickName );

        D3DXVECTOR2 vPos_ViewPort = m_pkColorFrame[i]->GetFixedWindowPos();
        vPos_ViewPort.y -= RT_VIEWPORT_HEIGHT * GC_SCREEN_DIV_HEIGHT;
        vPos_ViewPort.x -= 40 * GC_SCREEN_DIV_WIDTH ;
        m_kCharacter.SetViewPort( vPos_ViewPort, static_cast<DWORD>(RT_VIEWPORT_WIDTH * GC_SCREEN_DIV_WIDTH), static_cast<DWORD>(RT_VIEWPORT_HEIGHT * GC_SCREEN_DIV_HEIGHT));

		D3DXVECTOR2 vPos_LocalPlayer = m_pkLocalUser->GetFixedWindowLocalPos();
		vPos_LocalPlayer.y = vPos_NickName.y - (float)RT_VIEWPORT_HEIGHT * GC_SCREEN_DIV_HEIGHT - 40 ;
		m_pkLocalUser->SetFixedWindowLocalPos( vPos_LocalPlayer );

        D3DXVECTOR2 vPos_Emoticon = m_pkEmoticonBox->GetFixedWindowLocalPos();
        vPos_Emoticon.y = vPos_NickName.y - (float)RT_VIEWPORT_HEIGHT + 40 * GC_SCREEN_DIV_HEIGHT;
        m_pkEmoticonBox->SetFixedWindowLocalPos( vPos_Emoticon );
	}
}

void KGCResultTower::SetWin( bool bWin )
{
	if( bWin == true )
	{
		m_pkWin->ToggleRender( true && IsTeamMode());
		m_pkLose->ToggleRender( false && IsTeamMode());
	}
	else
	{
		m_pkWin->ToggleRender( false && IsTeamMode());
		m_pkLose->ToggleRender( true && IsTeamMode());
	}
}

void KGCResultTower::SetEndHeight( int iRank )
{
	if( iRank == -1 )
	{
		m_dwEndHeight = RT_DEFAULT_HEIGHT - RT_HEIGHT_TERM;
		return;
	}

	m_dwEndHeight = RT_DEFAULT_HEIGHT + RT_HEIGHT_TERM * (MAX_PLAYER_NUM - iRank);
}



void KGCResultTower::PostChildDraw()
{
	if( m_iPlayerIndex != -1 )
	{

		// Exp & GP 내용을 Rendering
		if( m_bRenderExpGp == true )
		{			
			D3DXVECTOR2 vPos_exp = m_pkNickName->GetRelocatedWindowPos();
			std::stringstream str;
            
            vPos_exp.y -= 180 * m_fWindowScaleY;

#if defined ( GUILD_RENEW_S4)
            g_pkUIMgr->RenderNumber( "contribution", vPos_exp, static_cast<UINT64>(0), D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true);
            vPos_exp.x += 54 * m_fWindowScaleX;
            str.str("");
            str << g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.iGuildScore;
            g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos_exp,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true,true);
#else
#if !defined(NATION_PHILIPPINE) && !defined (NATION_THAILAND) && !defined (NATION_IDN) && !defined(NATION_EU)
            g_pkUIMgr->RenderNumber( "spexp", vPos_exp, static_cast<UINT64>(0), D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true);
#endif
            vPos_exp.x += 54 * m_fWindowScaleX;
#if !defined(NATION_PHILIPPINE) && !defined (NATION_THAILAND) && !defined (NATION_IDN)&& !defined(NATION_EU)
            str.str("");
            str<<g_pGameBoard->GetChangedSPExp( m_iPlayerIndex );
            g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos_exp,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true,true);
#endif

#endif

			vPos_exp.x -= 54 * m_fWindowScaleX;
			vPos_exp.y -= 20 * m_fWindowScaleY;
			g_pkUIMgr->RenderNumber( "exp", vPos_exp, static_cast<UINT64>(0), D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true);
			vPos_exp.x += 30 * m_fWindowScaleX;			
			str.str("");
			str<<g_pGameBoard->GetChangedExp( m_iPlayerIndex );
			g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos_exp,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);

			vPos_exp.x -= 30 * m_fWindowScaleX;
			vPos_exp.y -= 20 * m_fWindowScaleY;
			g_pkUIMgr->RenderNumber( "gp", vPos_exp, static_cast<UINT64>(0), D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true);
			vPos_exp.x += 30 * m_fWindowScaleX;
			str.str("");
			str<<g_pGameBoard->GetChangeGp( m_iPlayerIndex );
			g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos_exp,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);
		

		}
        RenderEmoticon();

		D3DXVECTOR2 vPos_Levup = m_pkNickName->GetFixedWindowLocalPos();


#if !defined( NEW_LEVELUP_EFFECT )
        //  구버젼 레벨업 표시 코드
		// Levelup 내용을 Rendering
		if( m_bLevelUp == true )
		{
            vPos_Levup.y -= 35;
            m_pkLevelUp->SetWindowPosDirect(vPos_Levup);
		}

		// Levelup 내용을 Rendering
#ifdef SKILL_TREE
		if( m_bSPLevelUp == true )
		{
			int sp10 = g_pGameBoard->IsSPLevelUp(m_iPlayerIndex)/10;
			int sp1 = g_pGameBoard->IsSPLevelUp(m_iPlayerIndex)%10;
			float movex=0;

			char str[4];
			str[0]='0'+10;			

			if(sp10>0)
			{
				str[1]='0'+sp10;
				str[2]='0'+sp1;
				str[3]='\0';				
				movex=33;
			}
			else
			{
				str[1]='0'+g_pGameBoard->IsSPLevelUp(m_iPlayerIndex);
				str[2]='\0';
				movex=22;
			}				
			
			vPos_Levup.x+=movex;	
			vPos_Levup.y -= 35;
			m_pkSPLevelUp->SetWindowPosDirect(vPos_Levup);

			D3DXVECTOR2 vPos = m_pkSPLevelUp->GetRelocatedWindowPos();
			vPos.x-=movex * m_fWindowScaleX;
			vPos.y+=12 * m_fWindowScaleY;
#ifndef		NATION_PHILIPPINE 
			g_pkUIMgr->RenderNumber( "red_gradation_num", vPos, str, D3DXCOLOR(0xffffffff), true, false, -1.0f );
#endif
		}		
#endif //SKILL_TREE
#endif //NEW_LEVELUP_EFFECT

	}
}

bool KGCResultTower::IsTeamMode()
{
	return ( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT ||
			SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER );
}

void KGCResultTower::RenderEmoticon( void )
{
    // 이놈아도.. -1일때가 있는데;;; 어설트가 저러면;; 
    //ASSERT( m_iPlayerIndex >= 0 && m_iPlayerIndex < MAX_DUNGEON_PLAYER_NUM );
    if ( ( 0 <= m_iPlayerIndex && MAX_PLAYER_NUM > m_iPlayerIndex ) && NULL != g_MyD3D->MyPlayer[m_iPlayerIndex]->GetEmoticon() )
    {
        D3DXVECTOR2 vPos( m_pkEmoticonBox->GetRelocatedWindowPos() );
        g_MyD3D->MyPlayer[m_iPlayerIndex]->RenderEmoticon( vPos.x, vPos.y, vPos.x + 50 *m_fWindowScaleX, vPos.y + 50 *m_fWindowScaleY);
    }
}

void KGCResultTower::SetDamage( int iDamage)
{
    if( iDamage < 0 ) {iDamage = 0; }

    m_iDamage = iDamage;
    m_pkDamage->SetDamage( m_iDamage, m_iTeam);
}