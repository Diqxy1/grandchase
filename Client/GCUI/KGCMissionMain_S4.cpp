#include "stdafx.h"
#include "KGCMissionMain_S4.h"
#include "GCMoveModel.h"

IMPLEMENT_CLASSNAME( KGCMissionMain_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionMain_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionMain_S4, "gc_main_mission" );

KGCMissionMain_S4::KGCMissionMain_S4( void )
: m_pkStatic_Knight( NULL )
, m_pkBack_Knight( NULL )
, m_pkBack_Model( NULL )
, m_pkScrollbar( NULL )
, m_pkCharSelector( NULL )
, m_pkBtn_Prograss( NULL )
, m_pkBack_NotMission_1( NULL )
, m_pkBack_NotMission_2( NULL )
, m_pkBtn_Chain( NULL )
, m_iMissiontUIDIdx( 0 )
, m_iListBarIdx( 0 )
, m_iTabIdx( 0 )
, m_iScrollIdx( 0 )
, m_bPrograssMode( false )
{
    memset(m_iTabCnt, 0, sizeof(int) * TAB_NUM);
    memset(m_pkTabCnt, 0, sizeof(KD3DStatic*) * TAB_NUM);
    memset(m_pkTab, 0, sizeof(KD3DWnd*) * TAB_NUM * TAB_STATE_NUM);
    memset(m_pkMissionList, 0, sizeof(KGCMissionListBar_S4*) * NUM_MISSION_LIST);
    memset(m_pkStaticTitle, 0, sizeof(KD3DStatic*) * NUM_TITLE);

    LINK_CONTROL( "knight_ment", m_pkStatic_Knight );
    LINK_CONTROL( "knight_ballon", m_pkBack_Knight );
    LINK_CONTROL( "mission_npc_model", m_pkBack_Model );
    
    LINK_CONTROL( "scrollbar", m_pkScrollbar );
    LINK_CONTROL( "char_selecter", m_pkCharSelector );
    LINK_CONTROL( "btn_prograssmode", m_pkBtn_Prograss );
    LINK_CONTROL( "btn_chainmode", m_pkBtn_Chain );
    LINK_CONTROL( "back_NotMission1", m_pkBack_NotMission_1 );
    LINK_CONTROL( "back_NotMission2", m_pkBack_NotMission_2 );

    char szTemp[128];
    for(int iLoopX=0; iLoopX<TAB_NUM; ++iLoopX)
    {
        for(int iLoopY=0; iLoopY<TAB_STATE_NUM; ++iLoopY)
        {
            sprintf(szTemp, "tab_%d_%d", iLoopX, iLoopY);
            LINK_CONTROL( szTemp, m_pkTab[iLoopX][iLoopY] );
        }
        sprintf(szTemp, "static_cnt%d", iLoopX);
        LINK_CONTROL( szTemp, m_pkTabCnt[iLoopX] );
    }
    for(int iLoop=0; iLoop<NUM_TITLE; ++iLoop)
    {
        sprintf(szTemp, "static_title%d", iLoop);
        LINK_CONTROL( szTemp, m_pkStaticTitle[iLoop] );
    }
    for(int iLoop=0; iLoop<NUM_MISSION_LIST; ++iLoop)
    {
        sprintf(szTemp, "list_bar%d", iLoop);
        LINK_CONTROL( szTemp, m_pkMissionList[iLoop] );
    }
}

KGCMissionMain_S4::~KGCMissionMain_S4( void ){}

void KGCMissionMain_S4::OnCreateComplete( void )
{
    if( !g_pkUIScene->m_pkMissionUI_S4->GetPriview() )
    {
        m_fMentRadio = 0.0f;    m_fModelRadio = 0.0f;
        m_iMent_Pos[ANI_START] = D3DXVECTOR2(676.0f * GC_SCREEN_DIV_WIDTH, 407.0f * GC_SCREEN_DIV_WIDTH);
        m_iMent_Pos[ANI_END] = D3DXVECTOR2(171.0f * GC_SCREEN_DIV_WIDTH, 407.0f * GC_SCREEN_DIV_WIDTH);
        m_iSayBox_Pos[ANI_START] = D3DXVECTOR2(676.0f * GC_SCREEN_DIV_WIDTH, 390.0f * GC_SCREEN_DIV_WIDTH);
        m_iSayBox_Pos[ANI_END] = D3DXVECTOR2(109.0f * GC_SCREEN_DIV_WIDTH, 390.0f * GC_SCREEN_DIV_WIDTH);
        m_iKnight_Pos[ANI_START] = D3DXVECTOR2(-414.0f * GC_SCREEN_DIV_WIDTH, 189.0f * GC_SCREEN_DIV_WIDTH);
        m_iKnight_Pos[ANI_END] = D3DXVECTOR2(-171.0f * GC_SCREEN_DIV_WIDTH, 189.0f * GC_SCREEN_DIV_WIDTH);

        // 기사단장
        if( m_pkBack_Knight )
            m_pkBack_Knight->InitState(true, false);
        if( m_pkBack_Model )
            m_pkBack_Model->InitState(true, false);
        if( m_pkStatic_Knight )
        {
            m_pkStatic_Knight->InitState(true, false);
            m_pkStatic_Knight->SetAlign( DT_VCENTER );
            m_pkStatic_Knight->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
            m_pkStatic_Knight->SetFontColor( 0xff000000 );
            m_pkStatic_Knight->SetFontSpace( 0 );
            SetStringMent( g_pkStrLoader->GetString( STR_ID_MISSION_KNIGHT_BASIC_MENT) );
        }

        // 엘리시스( 등록된미션이 없다 )
        if( m_pkBack_NotMission_1 )
            m_pkBack_NotMission_1->InitState( false, false );
        if( m_pkBack_NotMission_2 )
            m_pkBack_NotMission_2->InitState( false, false );

        // 캐릭터 선택UI
        if( m_pkCharSelector )
        {
            m_pkCharSelector->InitState(true, true, this);
            m_pkCharSelector->SetCallBack( boost::bind( &KGCMissionMain_S4::RefreshMainMission, this, _1, _2) );
        }

        // 리스트 출력모드(연계, 진행상황)
        if( m_pkBtn_Prograss )
        {
            m_pkBtn_Prograss->InitState(m_bPrograssMode, true, this);
            m_pkBtn_Prograss->DisconnectAllEventProcedure();
            m_pkBtn_Prograss->ConnectEventProcedure<KGCMissionMain_S4, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCMissionMain_S4::OnClickPrograssMode, _1, false ) );
            m_pkBtn_Prograss->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionMain_S4::OnEnterCursor );
            m_pkBtn_Prograss->SetToolTip( g_pkStrLoader->GetString( STR_ID_MISSTION_MODE_2 ) );
        }
        if( m_pkBtn_Chain )
        {
            m_pkBtn_Chain->InitState(!m_bPrograssMode, true, this);
            m_pkBtn_Chain->DisconnectAllEventProcedure();
            m_pkBtn_Chain->ConnectEventProcedure<KGCMissionMain_S4, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCMissionMain_S4::OnClickPrograssMode, _1, true) );
            m_pkBtn_Chain->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionMain_S4::OnEnterCursor );
            m_pkBtn_Chain->SetToolTip( g_pkStrLoader->GetString( STR_ID_MISSTION_MODE_1 ) );
        }

        // 탭 셋팅
        m_iTabIdx = 0;
        for(int iLoopX=0; iLoopX<TAB_NUM; ++iLoopX)
        {
            if( m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ] )
            {
                m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ]->InitState( (iLoopX!=m_iTabIdx), true, this);
                m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ]->DisconnectAllEventProcedure();
                m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ]->ConnectEventProcedure<KGCMissionMain_S4,void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCMissionMain_S4::OnClickTab, _1, iLoopX ) );
                m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ]->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionMain_S4::OnEnterCursor );
                m_pkTab[ iLoopX ][ TAB_STATE_DESABLE ]->SetToolTip( g_pkStrLoader->GetString( 8261 + iLoopX ) );
            }
            if( m_pkTab[ iLoopX ][ TAB_STATE_ENABLE ] )
            {
                m_pkTab[ iLoopX ][ TAB_STATE_ENABLE ]->InitState((iLoopX==m_iTabIdx), true, this);
                m_pkTab[ iLoopX ][ TAB_STATE_ENABLE ]->SetToolTip( g_pkStrLoader->GetString( 8261 + iLoopX ) );
            }
            if( m_pkTabCnt[ iLoopX ] )
            {
                m_pkTabCnt[ iLoopX ]->InitState(true, false);
                m_pkTabCnt[ iLoopX ]->SetFontSpace( 2 );
            }
        }

        // 타이틀 셋팅

        int wszTemp[4] = { STR_ID_MISSION_LEVEL, STR_ID_MISSION_TITLE, STR_ID_MISSION_LINK, STR_ID_MISSION_PROGRESS };

        for(int iLoop=0; iLoop<NUM_TITLE; ++iLoop)
        {
            if( m_pkStaticTitle[iLoop] )
            {
                m_pkStaticTitle[iLoop]->InitState( false, false );
                m_pkStaticTitle[iLoop]->SetText( g_pkStrLoader->GetString ( wszTemp[iLoop] ) );
                m_pkStaticTitle[iLoop]->SetFontColor( D3DCOLOR_ARGB(255, 252, 246, 224) );
                m_pkStaticTitle[iLoop]->SetFontOutline( true, D3DCOLOR_ARGB(255, 27, 21, 5) );
                m_pkStaticTitle[iLoop]->SetAlign( DT_CENTER );
            }
        }

        // 출력모드 토글셋팅
        SetPrograssModeToggle();

        // 스크롤바 셋팅
        if( m_pkScrollbar )
        {
            m_pkScrollbar->SetSelfInputCheck(true);
            m_pkScrollbar->AddActionListener(this);

            m_pkScrollbar->SetHeight( GC_SCREEN_DIV_SIZE_INT(256) );
            m_pkScrollbar->SetScrollPos( 0 );
            m_pkScrollbar->SetScrollGap( 1 );
            m_pkScrollbar->SetThumbYGab( 0 );
            m_pkScrollbar->SetScrollPageSize( 1 );
            m_pkScrollbar->SetScrollRangeMin( 0 );
        }

        // 리스트바 셋팅
        for(int iLoop=0; iLoop<NUM_MISSION_LIST; ++iLoop)
        {
            if( m_pkMissionList[iLoop] )
            {
                m_pkMissionList[ iLoop ]->InitState( true, true, this);
                m_pkMissionList[ iLoop ]->SetBarIndex( iLoop );
                m_pkMissionList[ iLoop ]->SetCallBack( boost::bind( &KGCMissionMain_S4::OnClickListBar, this, _1 ) );
            }
        }

        // 메인미션 갱신
        RefreshMainMission( m_pkCharSelector->GetCharIdx(), true );
    }
}

void KGCMissionMain_S4::FrameMoveInEnabledState( void )
{
    if( (1.0f > m_fMentRadio) && m_pkStatic_Knight && m_pkBack_Knight )
    {
        SetInterpolationRadio( m_fMentRadio, ANI_MENT_SPEED);
        D3DXVECTOR2 vecPos = LinearInterpolation(m_iMent_Pos[ANI_START], m_iMent_Pos[ANI_END], m_fMentRadio);
        m_pkStatic_Knight->SetFixedWindowLocalPos( vecPos );
        vecPos = LinearInterpolation(m_iSayBox_Pos[ANI_START], m_iSayBox_Pos[ANI_END], m_fMentRadio);
        m_pkBack_Knight->SetFixedWindowLocalPos( vecPos );
    }

//     if( (1.0f > m_fModelRadio) && m_pkBack_Model )
//     {
//         SetInterpolationRadio( m_fModelRadio, ANI_KNIGHT_SPEED);
//         D3DXVECTOR2 vecPos = LinearInterpolation(m_iKnight_Pos[ANI_START], m_iKnight_Pos[ANI_END], m_fModelRadio);
//         m_pkBack_Model->SetFixedWindowLocalPos( vecPos );
//     }
}

// 보간률 갱신
void KGCMissionMain_S4::SetInterpolationRadio(float &fRadio, float fSpeed)
{
    if( fRadio < 1.0f )    { fRadio += fSpeed; }
    else                   { fRadio = 1.0f; }
}

// 애니메이션을 위한 선형보간
template<typename T>
T KGCMissionMain_S4::LinearInterpolation(T &t1, T &t2, float &fRatio)
{
    return T( (t1 * (1.0f - fRatio)) + (t2 * fRatio) );
}

void KGCMissionMain_S4::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkScrollbar )
    {
        int iTemp = m_pkScrollbar->GetScrollPos();
        if( iTemp != m_iScrollIdx )
        {
            g_KDSound.Play( "73" );
            m_iScrollIdx = iTemp;
            SetListBarColor( true );
            RefreshMissionList();
        }
    }
}

void KGCMissionMain_S4::OnClickListBar( int iListIdx )
{
    int iUIDIdx = iListIdx+m_iScrollIdx;
    if( (0 > iListIdx || iListIdx > NUM_MISSION_LIST) ||                                // 인덱스범위 체크
        (m_vecMissionUID.size() <= UINT( iUIDIdx )) ||                                  // UID리스트크기와 인덱스크기 체크
        !g_kGlobalValue.m_kUserMission.IsUserMission( m_vecMissionUID[ iUIDIdx ] ) )    // 해당 인덱스 UID의 미션이 존재하는지 체크
        return;

    // 인덱스 처리
    m_iMissiontUIDIdx = iUIDIdx;
    m_iListBarIdx = iListIdx;
    SetListBarColor();

    // 
	m_pkBack_Model->SetModelID(SiKGCMissionManager()->GetMissionNPCID(m_vecMissionUID[ iUIDIdx ]));
    m_pkBack_Model->SetModelImage();

    // 서브미션 연결부
    if( g_pkUIScene->m_pkMissionUI_S4->GetSubMission() )
        g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->RefreshSubMission( m_vecMissionUID[ m_iMissiontUIDIdx ] );

    g_KDSound.Play( "31" );
}

void KGCMissionMain_S4::OnClickTab(int iTabIdx)
{
    m_pkTab[ m_iTabIdx ][ TAB_STATE_DESABLE ]->ToggleRender( true );
    m_pkTab[ m_iTabIdx ][ TAB_STATE_ENABLE ]->ToggleRender( false );
    m_pkTab[ iTabIdx ][ TAB_STATE_DESABLE ]->ToggleRender( false );
    m_pkTab[ iTabIdx ][ TAB_STATE_ENABLE ]->ToggleRender( true );
    m_iTabIdx = iTabIdx;
    RefreshUI();

    g_KDSound.Play( "31" );
}

void KGCMissionMain_S4::OnClickPrograssMode( bool bSwitch )
{
    m_bPrograssMode = bSwitch;
    for(int iLoop=0; iLoop<NUM_MISSION_LIST; ++iLoop)
    {
        if( m_pkMissionList[ iLoop ] )
            m_pkMissionList[ iLoop ]->SetProgressMode( m_bPrograssMode );
    }

    // 메인미션 리스트 출력모드 Toggle
    SetPrograssModeToggle();
    RefreshMissionList();
}

void KGCMissionMain_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCMissionMain_S4::RefreshUI()
{
    if( m_pkCharSelector )
        RefreshMainMission( m_pkCharSelector->GetCharIdx(), true );
}

void KGCMissionMain_S4::RefreshMainMission( int iCharIdx, bool bAutoSelect )
{
    if( 0 > iCharIdx || iCharIdx > GC_CHAR_NUM )
        return;

    // 미션리스트 받아오기
    m_vecMissionUID.clear();
    m_vecCharMissionUID.clear();
    m_vecCharMissionUID = g_kGlobalValue.m_kUserMission.GetCharacterAndCommonMission( iCharIdx );

    // 탭 처리
    int iCnt;
    WCHAR wszTemp[128];
    D3DXVECTOR2 vecPos;
    for(int iLoop=0; iLoop<TAB_NUM; ++iLoop)
    {
        iCnt = GetTabMission( iLoop, (iLoop == m_iTabIdx) );
        wsprintf(wszTemp, L"(%d)", iCnt);
        if( m_pkTabCnt[ iLoop ] )
        {
            m_pkTabCnt[ iLoop ]->SetText( std::wstring( wszTemp ) );
            vecPos = GetTabTextPos( iLoop, (iLoop == m_iTabIdx) );
#if defined(NATION_EU)
            vecPos.x += 13;
#endif
            if( iLoop == m_iTabIdx )
            {
                m_pkTabCnt[ iLoop ]->SetFontColor( D3DCOLOR_ARGB(255, 255, 251, 240) );
                m_pkTabCnt[ iLoop ]->SetFontOutline( true, D3DCOLOR_ARGB(255, 27, 21, 7) );
                m_pkTabCnt[ iLoop ]->SetWindowPos( vecPos );
            }
            else
            {
                m_pkTabCnt[ iLoop ]->SetFontColor( D3DCOLOR_ARGB(255, 166, 158, 139) );
                m_pkTabCnt[ iLoop ]->SetFontOutline( true, D3DCOLOR_ARGB(255, 47, 33, 17) );
                m_pkTabCnt[ iLoop ]->SetWindowPos( vecPos );
            }
        }
    }

    // 등록된 미션이 없을 때 처리 : 서브미션 초기화
    if( 0 == m_vecMissionUID.size() )
    {
        if( g_pkUIScene->m_pkMissionUI_S4->GetSubMission() )
        {
            g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->CleanUpSubMission();
            g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->ToggleRender( false );
        }

        if( m_pkBack_NotMission_1 )
            m_pkBack_NotMission_1->ToggleRender( true );
        if( m_pkBack_NotMission_2 )
            m_pkBack_NotMission_2->ToggleRender( true );

        RefreshScrollBar( 0 );
        RefreshMissionList();
        // 기본 기사단장 이미지
        m_pkBack_Model->SetModelID( MOVE_MODEL_NPC_CAPTAIN ); //4번 리뉴얼기사단장을 기본으로
        m_pkBack_Model->SetModelImage();

        SetStringMent( g_pkStrLoader->GetString( STR_ID_MISSION_KNIGHT_BASIC_MENT ) );
        return;
    }
    else if( g_pkUIScene->m_pkMissionUI_S4->GetSubMission() )
    {
        g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->CleanUpSubMission();
        g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->ToggleRender( true );

        if( m_pkBack_NotMission_1 )
            m_pkBack_NotMission_1->ToggleRender( false );
        if( m_pkBack_NotMission_2 )
            m_pkBack_NotMission_2->ToggleRender( false );
    }

    // 적정 레벨순으로 UID정렬(정렬된 순서대로 메인미션 리스트바에 출력될꺼임)
    m_vecSorting.resize( m_vecMissionUID.size() );
    m_vecSorting.clear();
    MergeSorting( &m_vecMissionUID, 0, (m_vecMissionUID.size()-1) );

    // 미션 자동선택 인덱스 처리(완료한 미션, 새로운 미션, 최근 선택미션, 리스트 최상단 미션)
    AutoMissionSelect( bAutoSelect );

    // 스크롤바, 리스트바 리셋팅
    // 리스트바 셋팅 중 UID에 오류가 있었다면 다시 미션자동선택과 스크롤바셋팅, 리스트바 재정렬이 필요하다.
    RefreshScrollBar( m_vecMissionUID.size() );
    if( RefreshMissionList() )
    {
        AutoMissionSelect( bAutoSelect );
        RefreshScrollBar( m_vecMissionUID.size() );
        RefreshMissionList();
    }

    // 리스트바 자동선택
    OnClickListBar( m_iListBarIdx );
}

D3DXVECTOR2 KGCMissionMain_S4::GetTabTextPos( int iTab, bool bEnable )
{
    switch( iTab )
    {
    case TAB_ALL:
        return bEnable?D3DXVECTOR2(66.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH):D3DXVECTOR2(62.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH);
    case TAB_NORMAL:
        return bEnable?D3DXVECTOR2(143.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH):D3DXVECTOR2(139.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH);
    case TAB_CHARACTER:
        return bEnable?D3DXVECTOR2(224.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH):D3DXVECTOR2(226.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH);
    case TAB_EVENT:
        return bEnable?D3DXVECTOR2(301.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH):D3DXVECTOR2(303.0f * GC_SCREEN_DIV_WIDTH, 62.0f * GC_SCREEN_DIV_WIDTH);
    }
    return D3DXVECTOR2(0.0f, 0.0f);
}

void KGCMissionMain_S4::AutoMissionSelect( bool bAutoSelect )
{
    if( bAutoSelect && !g_kGlobalValue.m_kLoginInfo.bNewUser )
    {
        // 완료한 미션이 있는지 확인
        int iNewMissionIdx = -1;
        for(int iLoop=0; iLoop<int(m_vecMissionUID.size()); ++iLoop)
        {
            if( g_kGlobalValue.m_kUserMission.IsCompleteMission( m_vecMissionUID[ iLoop ] ) )
            {
                m_iMissiontUIDIdx = iLoop;
                m_iScrollIdx = (iLoop-(NUM_MISSION_LIST-1)) < 0 ? 0 : (iLoop-(NUM_MISSION_LIST-1));
                m_iListBarIdx = iLoop - m_iScrollIdx;
                return;
            }
            if( g_kGlobalValue.m_kUserMission.IsNewMission( m_vecMissionUID[ iLoop ] ) )
                if( 0 > iNewMissionIdx ) iNewMissionIdx = iLoop;
        }

        // 새로운 미션이 있는지 확인
        if( 0 <= iNewMissionIdx )
        {
            m_iMissiontUIDIdx = iNewMissionIdx;
            m_iScrollIdx = (iNewMissionIdx-(NUM_MISSION_LIST-1)) < 0 ? 0 : (iNewMissionIdx-(NUM_MISSION_LIST-1));
            m_iListBarIdx = iNewMissionIdx - m_iScrollIdx;
            return;
        }

        // 최근 선택한 미션 자동선택은 빼기로 했음... 이유는...
        // 유저가 보유한 캐릭터별로 최근 선택했던 미션상태를 유지해야하므로 
        // DB, 서버, 클라 전체 작업이 필요하고 별로 유용한 기능이 아니라는 판단에..
    }

    // 최 상단 미션 선택
    m_iScrollIdx = 0;
    m_iListBarIdx = 0;
    m_iMissiontUIDIdx = 0;
}

bool KGCMissionMain_S4::RefreshMissionList()
{
    bool bResult = false;
    WCHAR wszTemp[512];
    for(int iLoop=0; iLoop<NUM_MISSION_LIST; ++iLoop)
    {
        if( m_pkMissionList[ iLoop ] )
        {
            // UID인덱스 구하기
            UINT iUIDIdx = iLoop+m_iScrollIdx;

            // 정보초기화
            m_pkMissionList[ iLoop ]->ClearText();
            m_pkMissionList[ iLoop ]->SetProgressBarToggle( false );
            m_pkMissionList[ iLoop ]->SetMissionTypeIcon( 0 );

            if( m_vecMissionUID.size() > iUIDIdx )
            {
                // 미션존재확인( 없으면 리스트에서 UID제거 )
                if( g_kGlobalValue.m_kUserMission.IsUserMission( m_vecMissionUID[ iUIDIdx ] ) == false )
                {
                    bResult = true;
                    m_vecMissionUID.erase( find(m_vecMissionUID.begin(), m_vecMissionUID.end(), m_vecMissionUID[ iUIDIdx ] ) );
                    --iLoop;
                    continue;
                }

                // 완료된 미션인지, 새로운 미션인지 확인
                bool bComplate = g_kGlobalValue.m_kUserMission.IsCompleteMission( m_vecMissionUID[ iUIDIdx ] );
                m_pkMissionList[ iLoop ]->SetComplateMark( bComplate );

                if( !bComplate )
                    m_pkMissionList[ iLoop ]->SetNewMark( g_kGlobalValue.m_kUserMission.IsNewMission( m_vecMissionUID[ iUIDIdx ] ) );

                // 미션리스트에서 정보얻어오기( 없으면 리스트에서 UID제거 )
                const SMissionInfo* sInfo = SiKGCMissionManager()->GetMissionInfo( m_vecMissionUID[ iUIDIdx ] );
                if( NULL == sInfo )
                {
                    bResult = true;
                    m_vecMissionUID.erase( find(m_vecMissionUID.begin(), m_vecMissionUID.end(), m_vecMissionUID[ iUIDIdx ] ) );
                    --iLoop;
                    continue;
                }

                // 미션 진행상태 프로그래스바 셋팅
                if( m_bPrograssMode )
                {
                    SetPrograssBarInfo( iLoop, iUIDIdx );
                    m_pkMissionList[ iLoop ]->SetProgressBarToggle( m_bPrograssMode );
                }

                // 출력할 텍스트 만들기
                if( 0 <= sInfo->m_iCorrectLevel)
                    wsprintf(wszTemp, L"%d", sInfo->m_iCorrectLevel);
                else
                    wsprintf(wszTemp, L"-");
                m_pkMissionList[ iLoop ]->SetLevelText( std::wstring( wszTemp ) );

                if( (0 <= sInfo->m_iCurrentChain) && (0 <= sInfo->m_iTotalChain) )
                    wsprintf(wszTemp, L"%d-%d", sInfo->m_iTotalChain, sInfo->m_iCurrentChain);
                else
                    wsprintf(wszTemp, L"-");
                m_pkMissionList[ iLoop ]->SetChainText( std::wstring( wszTemp ) );
                m_pkMissionList[ iLoop ]->SetTitleText( SiKGCMissionManager()->GetString( sInfo->m_iTitleID ) );

                //출력할 미션타입 설정
                m_pkMissionList[ iLoop ]->SetMissionTypeIcon( sInfo->m_iMissionType );
            }
        }
    }
    return bResult;
}

void KGCMissionMain_S4::RefreshScrollBar( UINT iElementCnt )
{
    m_pkScrollbar->SetScrollRangeMax( (iElementCnt<NUM_MISSION_LIST) ? 1 : (iElementCnt-(NUM_MISSION_LIST-1)) );
    m_pkScrollbar->ResizeBtn();
    m_pkScrollbar->SetScrollPos( m_iScrollIdx );
}

void KGCMissionMain_S4::SetPrograssBarInfo( int iListBarIdx, int iUIDIdx )
{
    if( (0 > iListBarIdx || iListBarIdx >= NUM_MISSION_LIST) ||
        (0 > iUIDIdx || iUIDIdx > int(m_vecMissionUID.size())) )
        return;

    const SMissionInfo* sMainInfo = SiKGCMissionManager()->GetMissionInfo( m_vecMissionUID[ iUIDIdx ] );
    if( NULL == sMainInfo )
        return;

    const SSubMissionInfo* sSubInfo = SiKGCMissionManager()->GetSubMission( sMainInfo->m_vecSubMissions[ 0 ] );
    if( NULL == sSubInfo )
        return;

    int iComplateCnt = g_kGlobalValue.m_kUserMission.GetCompletionCount( m_vecMissionUID[ iUIDIdx ], sMainInfo->m_vecSubMissions[ 0 ] );
    int iMaxCnt = sSubInfo->m_sCompletion.m_iCount;
    if( iComplateCnt > iMaxCnt ) iComplateCnt = iMaxCnt;
    m_pkMissionList[ iListBarIdx ]->SetProgressBarValue(iComplateCnt, 0, iMaxCnt);
}

void KGCMissionMain_S4::SetPrograssModeToggle()
{

    if( m_pkBtn_Prograss )
        m_pkBtn_Prograss->ToggleRender( m_bPrograssMode );
    if( m_pkBtn_Chain )
        m_pkBtn_Chain->ToggleRender( !m_bPrograssMode );

    m_pkStaticTitle[TITLE_LEVEL]->ToggleRender( !m_bPrograssMode );

    m_pkStaticTitle[TITLE_CHAIN]->ToggleRender( !m_bPrograssMode );

    m_pkStaticTitle[TITLE_PROGRASS]->ToggleRender( m_bPrograssMode );

    m_pkStaticTitle[TITLE_NAME]->ToggleRender( true );
  
#if !defined(NATION_USA) && !defined( NATION_EU )
    m_pkStaticTitle[TITLE_NAME]->SetFixedWindowLocalPos( m_bPrograssMode?D3DXVECTOR2(70.0f * GC_SCREEN_DIV_WIDTH,87.0f * GC_SCREEN_DIV_WIDTH):D3DXVECTOR2(140.0f * GC_SCREEN_DIV_WIDTH,87.0f * GC_SCREEN_DIV_WIDTH) );
#endif
}

void KGCMissionMain_S4::SetListBarColor( bool bScroll )
{
    // 메인미션 텍스트 색상 처리
    int iUIDIdx;
    D3DCOLOR dwFontColor, dwOutlineColor;
    const SMissionInfo* sInfo;
    for(int iLoop=0; iLoop<NUM_MISSION_LIST; ++iLoop)
    {
        dwFontColor = D3DCOLOR_ARGB(255, 252, 246, 224);
        dwOutlineColor = D3DCOLOR_ARGB(255, 27, 21, 5);
        iUIDIdx = iLoop+m_iScrollIdx;
        if( 0 <= iUIDIdx && iUIDIdx < int(m_vecMissionUID.size()) )
        {
            if( (sInfo = SiKGCMissionManager()->GetMissionInfo( m_vecMissionUID[ iUIDIdx ] )) )
                GetLevelColor( sInfo->m_iCorrectLevel, dwFontColor, dwOutlineColor );
        }
        m_pkMissionList[ iLoop ]->SetFontColor( dwFontColor );
        m_pkMissionList[ iLoop ]->SetOutlineColor( dwOutlineColor );
        m_pkMissionList[ iLoop ]->SwapBackGround( false );
    }

    // 선택된 바 텍스트 색상 처리
    if( bScroll ) iUIDIdx = (m_iMissiontUIDIdx-m_iScrollIdx);
    else          iUIDIdx = m_iListBarIdx;
    if( 0 <= iUIDIdx && iUIDIdx < NUM_MISSION_LIST )
    {
        m_pkMissionList[ iUIDIdx ]->SetFontColor( D3DCOLOR_ARGB(255, 255, 215, 3) );
        m_pkMissionList[ iUIDIdx ]->SetOutlineColor( D3DCOLOR_ARGB(255, 0, 0, 2) );
        m_pkMissionList[ iUIDIdx ]->SwapBackGround( true );
    }
}

void KGCMissionMain_S4::SetStringMent( int iStringID )
{
    m_pkStatic_Knight->SetTextAutoMultiline( g_pkStrLoader->GetString( iStringID ) );
}

void KGCMissionMain_S4::SetStringMent( std::wstring strMent )
{
    m_pkStatic_Knight->SetTextAutoMultiline( strMent );
}

int KGCMissionMain_S4::GetTabMission( int iTabIdx, bool bPushUID )
{
    int iCnt=0;
    std::vector< DWORD >::iterator vecIter = m_vecCharMissionUID.begin();
    for(; vecIter != m_vecCharMissionUID.end(); ++vecIter)
    {
        if ( SiKGCMissionManager()->GetMissionInfo( *vecIter ) )
        {
            if( (0 == iTabIdx) ||                
                SiKGCMissionManager()->GetMissionInfo( *vecIter )->m_iMissionType == iTabIdx ||
                ( 1 == iTabIdx && SiKGCMissionManager()->GetMissionInfo( *vecIter )->m_iMissionType == TAB_EPIC ) )
            {
                ++iCnt;
                if( bPushUID )
                    m_vecMissionUID.push_back( *vecIter );
            }
        }
    }
    return iCnt;
}

void KGCMissionMain_S4::GetLevelColor( int iMissionLevel, OUT DWORD &dwFontColor, OUT DWORD &dwOutlineColor )
{
    SCharInfo charinfo;
    charinfo = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar( m_pkCharSelector->GetCharIdx() );
    int iLevel = iMissionLevel - charinfo.iLevel;

    // 어두운색<=  (-5     0      5)   => 붉은색, 레벨이 없으면 적정레벨과 동일하게
    if( (-5 <= iLevel) && (iLevel <= 5) || (iMissionLevel < 0) )
    {
        dwFontColor = D3DCOLOR_ARGB(255, 252, 246, 224);
        dwOutlineColor = D3DCOLOR_ARGB(255, 27, 21, 5);
    }
    else if( -5 > iLevel )
    {
        dwFontColor = D3DCOLOR_ARGB(255, 144, 143, 138);
        dwOutlineColor = D3DCOLOR_ARGB(255, 69, 56, 22);
    }
    else if( 5 < iLevel )
    {
        dwFontColor = D3DCOLOR_ARGB(255, 254, 76, 92);
        dwOutlineColor = D3DCOLOR_ARGB(255, 47, 2, 0);
    }
}

void KGCMissionMain_S4::Merge(std::vector< DWORD > *pData, int iLeft, int iMiddle, int iRight)
{
    int iAIdx, iBIdx, iBufIdx;

    // 부분배열 인덱스 결정( A[ Left~Middle ], B[ (Middle+1)~Right ])
    iAIdx= iLeft; iBIdx=iMiddle+1; iBufIdx=iLeft;

    // 두 부분배열을 비교하여 정렬(DataTemp에 값 복사)
    const SMissionInfo* sInfo;
    while( (iAIdx<=iMiddle) && (iBIdx<=iRight) )
    {
        // 적정레벨 얻어오기
        sInfo = SiKGCMissionManager()->GetMissionInfo( (*pData)[iAIdx] );
        if( NULL == sInfo )
        {
            ++iAIdx;
            continue;
        }
        int iAIdxLevel = sInfo->m_iCorrectLevel;    

        sInfo = SiKGCMissionManager()->GetMissionInfo( (*pData)[iBIdx] );
        if( NULL == sInfo )
        {
            ++iBIdx;
            continue;
        }
        int iBIdxLevel = sInfo->m_iCorrectLevel;

        // 레벨 비교
        if( iAIdxLevel <= iBIdxLevel )
        {	m_vecSorting[iBufIdx++] = (*pData)[iAIdx++];	}
        else
        {	m_vecSorting[iBufIdx++] = (*pData)[iBIdx++];	}
    }

    // 남아있는 B배열((Middle+1)~iRight)의 데이터 DataTemp에 복사
    if(iAIdx > iMiddle)
    {
        for(int iLoop=iBIdx; iLoop<=iRight; ++iLoop)
        {	m_vecSorting[iBufIdx++] = (*pData)[iLoop];	}
    }
    // 남아있는 A배열(Left~Middle)의 데이터 DataTemp에 복사
    else
    {
        for(int iLoop=iAIdx; iLoop<=iMiddle; ++iLoop)
        {	m_vecSorting[iBufIdx++] = (*pData)[iLoop];	}
    }

    // 합병정렬된 데이터 복사
    for(int iLoop=iLeft; iLoop<=iRight; ++iLoop)
    {	(*pData)[iLoop] = m_vecSorting[iLoop];	}
}

void KGCMissionMain_S4::MergeSorting(std::vector< DWORD > *pData, int iLeft, int iRight)
{
    // Left와 Right가 엇갈리지 않았을때 재귀호출
    if( iLeft < iRight)
    {
        int iMiddle = 	(iLeft + iRight)/2;
        MergeSorting(pData, iLeft, iMiddle);        // 좌측분할
        MergeSorting(pData, iMiddle+1, iRight);     // 우측분할
        Merge(pData, iLeft, iMiddle, iRight);       // 합병
    }
}