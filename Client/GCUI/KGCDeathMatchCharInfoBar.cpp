#include "stdafx.h"
#include "KGCDeathMatchCharInfoBar.h"
//
#include "MyD3D.h"


#include "KGCGameBoard.h"
#include "KGCGradeMark.h"

IMPLEMENT_CLASSNAME( KGCDeathMatchCharInfoBar );
IMPLEMENT_WND_FACTORY( KGCDeathMatchCharInfoBar );
IMPLEMENT_WND_FACTORY_NAME( KGCDeathMatchCharInfoBar, "gc_deathmatch_char_bar" );

KGCDeathMatchCharInfoBar::KGCDeathMatchCharInfoBar(void)
{
    m_pkGradeMark       = NULL;
    m_pkGuildMark       = NULL;
    m_pkGuildStatic     = NULL;
    m_pkNickNameStatic  = NULL;
    m_pkDamageStatic    = NULL;
    m_pkKillStatic      = NULL;
    m_pkDeathdStatic    = NULL;
    m_pkPingStatic      = NULL;

    LINK_CONTROL( "grade_mark",         m_pkGradeMark       );     
    LINK_CONTROL( "window_guild_mark",  m_pkGuildMark       );  
    LINK_CONTROL( "static_guild_name",  m_pkGuildStatic     );
    LINK_CONTROL( "static_nickname",    m_pkNickNameStatic  );
    LINK_CONTROL( "static_damage",      m_pkDamageStatic    );
    LINK_CONTROL( "static_kill",        m_pkKillStatic      );
    LINK_CONTROL( "static_death",       m_pkDeathdStatic    );
    LINK_CONTROL( "static_ping",        m_pkPingStatic      );

    m_iPlayerIndex = 0;

}

KGCDeathMatchCharInfoBar::~KGCDeathMatchCharInfoBar(void)
{
}

void KGCDeathMatchCharInfoBar::UpdateGameData()
{
    if ( !g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
    {
        ToggleRender( false );
        return;
    }

    ToggleRender( true );
    
    m_pkNickNameStatic->SetText( g_MyD3D->MyPlayer[m_iPlayerIndex]->GetStrUserName() );
    m_pkGuildStatic->SetText( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.strGuildName );

    m_iGuildMark = 0;
    //((BYTE*)( &m_iGuildMark ))[0] = g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.aiGuildMark[0];
    //((BYTE*)( &m_iGuildMark ))[1] = g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.aiGuildMark[1];
    //((BYTE*)( &m_iGuildMark ))[2] = g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.aiGuildMark[2];

    int iGrade;
    {
        iGrade = g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.GetUserGrade();
    }
    m_pkGradeMark->SetGrade( iGrade );

}

void KGCDeathMatchCharInfoBar::PostChildDraw()
{
	D3DXVECTOR2 vGuildMarkPos;
    vGuildMarkPos.x = (  ( m_pkGuildMark->GetFixedWindowPos().x + m_pkGuildMark->GetWidth() / 2 ) / 400.0f ) - 1.0f;
    vGuildMarkPos.y = ( ( -m_pkGuildMark->GetFixedWindowPos().y - m_pkGuildMark->GetHeight() / 2 ) / 300.0f * 0.75f ) + 0.75f;

    //길드 마크를 그린다.
    //if ( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.iGuildID > -1 )
    //{
    //    g_MyD3D->m_KGCGuildMark.Render( ((BYTE*)(&m_iGuildMark))[0],
    //        ((BYTE*)(&m_iGuildMark))[1],
    //        ((BYTE*)(&m_iGuildMark))[2],
    //        vGuildMarkPos.x,
    //        vGuildMarkPos.y,
    //        (  m_pkGuildMark->GetWidth() / 400.0f ) );
    //}

    D3DXVECTOR2 vPosKillNum = m_pkKillStatic->GetRelocatedWindowPos();
    D3DXVECTOR2 vPosDeathNum = m_pkDeathdStatic->GetRelocatedWindowPos();
//    D3DXVECTOR2 vPosDamageNum = m_pkDamageStatic->GetCurrentWindowPos();

    vPosKillNum.x += 13 * m_fWindowScaleX;
    vPosDeathNum.x += 13 * m_fWindowScaleX;
    //vPosDamageNum.x += 50;

    g_pkUIMgr->RenderNumber( "blue_gradation_num",vPosKillNum,g_pGameBoard->GetHowKill( m_iPlayerIndex ),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), false, false, -1.0f ,true);
    g_pkUIMgr->RenderNumber( "blue_gradation_num",vPosDeathNum,g_pGameBoard->GetHowDie( m_iPlayerIndex ),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), false, false, -1.0f ,true);
    //g_pkUIMgr->RenderNumber( "blue_gradation_num",vPosDamageNum,g_pGameBoard->GetHowKill( m_iPlayerIndex ),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), false, false, -1.0f );
}
