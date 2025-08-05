#include "stdafx.h"
#include "KGCReplayBoxS6.h"

IMPLEMENT_CLASSNAME( KGCReplayBoxS6 );
IMPLEMENT_WND_FACTORY( KGCReplayBoxS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayBoxS6, "gc_replay_box_s6" );

KGCReplayBoxS6::KGCReplayBoxS6( void )
: m_pkScrollbar(NULL)
, m_pkExit(NULL)
, m_pkReplayBar(NULL)
{
    LINK_CONTROL( "replay_scroll", m_pkScrollbar );
    LINK_CONTROL( "replay_exit", m_pkExit);
    LINK_CONTROL( "replay_bar", m_pkReplayBar);

    for( int i = 0 ; i < (int)m_pReplayBar.size() ; ++i )
    {
        m_pReplayBar[i] = NULL;
    }

}

KGCReplayBoxS6::~KGCReplayBoxS6( void )
{
}

void KGCReplayBoxS6::OnCreate()
{
    m_pkScrollbar->SetSelfInputCheck( false );
    m_pkScrollbar->AddActionListener( this );
    m_pkScrollbar->SetScrollPageSize( REPLAY_BAR_NUM );
    m_pkScrollbar->SetScrollPos( 0 );

    m_pkExit->InitState( true, true, this );
    m_pkExit->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCReplayBoxS6::OnClose);
    m_pkExit->SetHotKey(DIK_ESCAPE);

    m_iTopIndex = 0;

    m_pkReplayBar->InitState( false );
    float fStartPosX = m_pkReplayBar->GetFixedWindowLocalPos().x;
    float fStartPosY = m_pkReplayBar->GetFixedWindowLocalPos().y;

    // 슬롯 생성
    std::vector<KD3DWnd*> vecSlotList = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_replay_box_s6.stg", "", "replay_box_s6\\replay_bar", true, REPLAY_BAR_NUM );
    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwHeight = (*iterSlot)->GetHeight();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_pReplayBar[i] = static_cast<KGCReplayBar*>(*iterSlot);
            m_pReplayBar[i]->InitState( true, true, this );

            D3DXVECTOR2 vPos;
            vPos.x = fStartPosX;
            vPos.y = fStartPosY + static_cast<float>( (i)*(dwHeight+REPLAY_BAR_GAP) );
            m_pReplayBar[i]->SetFixedWindowLocalPos(vPos);
        }
    }
}

void KGCReplayBoxS6::OnCreateComplete()
{
    RequestList();
}

void KGCReplayBoxS6::ActionPerformed(const KActionEvent& event )
{
    // 스크롤바를 조작했다면 리스트를 갱신한다.
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_pkScrollbar->GetScrollPos();
        if ( m_iTopIndex != iTop )
        {
            g_KDSound.Play( "73" );
            m_iTopIndex = iTop;
            UpdateReplayList();
        }
        return;
    }

    for (int i =0; i < REPLAY_BAR_NUM ; i ++)
    {
        if ( m_pReplayBar[i] != NULL && event.m_pWnd == m_pReplayBar[i] )
            if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
            {
                m_pReplayBar[i]->CursorEnter();
            }
            else if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE)
            {
                m_pReplayBar[i]->CursorLeave();
            }
    }
}

void KGCReplayBoxS6::FrameMoveInEnabledState()
{

}
void KGCReplayBoxS6::OnClose()
{
    g_KDSound.Play( "32" );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCReplayBoxS6::RequestList( void )
{
    g_MyD3D->MyReplay->GetList();
    UpdateReplayList();	
}

void KGCReplayBoxS6::UpdateReplayList( void )
{
    static WCHAR s_strFileName[MAX_PATH] = {0,};
    // 스크롤바의 최대치를 조정한다.
    m_pkScrollbar->SetScrollRangeMax( g_MyD3D->MyReplay->m_iFileNum /*이 숫자는 반드시 MyReplay의 GetList 함수를 부른 다음에야 제대로 값을 가진다.*/);     

    // 리플레이 파일 리스트 정보를 입력한다.
    WCHAR strSep[] = {L"\\"};
    for ( int i = 0; i < REPLAY_BAR_NUM; i++ )
    {
        if ( m_iTopIndex + i < g_MyD3D->MyReplay->m_iFileNum )
        {
            m_pReplayBar[i]->ToggleRender( true );
            wcscpy( s_strFileName, g_MyD3D->MyReplay->m_vecFileList[m_iTopIndex+i].c_str() );
            WCHAR* strFileName = wcstok(s_strFileName, strSep );
            strFileName = wcstok( NULL, strSep );
            m_pReplayBar[i]->SetBarText(strFileName);
            m_pReplayBar[i]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCReplayBoxS6::OnClickReplayBar);
        }
        else
        {
            m_pReplayBar[i]->ToggleRender( false );
        }

    }
}

void KGCReplayBoxS6::OnClickReplayBar()
{
    g_KDSound.Play( "31" );
    OnClose();
    g_MyD3D->m_pStateMachine->GoReplay();
}