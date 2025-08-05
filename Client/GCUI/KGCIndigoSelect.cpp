#include "stdafx.h"
#include "KGCIndigoSelect.h"

//

#include "../MyD3D.h"
//
IMPLEMENT_CLASSNAME( KGCIndigoSelect );
IMPLEMENT_WND_FACTORY( KGCIndigoSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCIndigoSelect, "gc_indigo_select" );

KGCIndigoSelect* g_pIndigoSelect = NULL;

KGCIndigoSelect::KGCIndigoSelect(void)
{
	g_pIndigoSelect = this;

	char strWnd[MAX_PATH];

	for( int i=0 ; i<GC_CHAR_NUM ; ++i )
	{
		m_ppkCharFace[i] = NULL;
		m_ppkCandChar[i] = NULL;

		sprintf( strWnd, "char_face%d", i+1 );
		LINK_CONTROL( strWnd, m_ppkCharFace[i] );

		sprintf( strWnd, "cand_char%d", i+1 );
		LINK_CONTROL( strWnd, m_ppkCandChar[i] );
	}

	for( int i=0 ; i<3 ; ++i )
	{
		m_ppkSelectChar[i] = NULL;	

		sprintf( strWnd, "select_char%d", i+1 );
		LINK_CONTROL( strWnd, m_ppkSelectChar[i] );
	}

    m_pkBtnRight = NULL;
    m_pkBtnLeft = NULL;
    LINK_CONTROL( "btn_right", m_pkBtnRight );
    LINK_CONTROL( "btn_left", m_pkBtnLeft );
}

KGCIndigoSelect::~KGCIndigoSelect(void)
{
}

void KGCIndigoSelect::CandidateVisible()
{
    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        if (m_ppkCandChar[i])
            m_ppkCandChar[i]->ToggleRender( false );
    }
    for( int i = 0; i < (int)m_vecNowVisible.size(); ++i )
    {
        m_ppkCandChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->ToggleRender(true);
        m_ppkCandChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->SetAllWindowPos(m_vecPos[i]);
    }
}

void KGCIndigoSelect::InitNowVisible()
{
    // 이미 벡터에 뭔가 들어있다면
    int iSelectIndex = 0;
    if( m_vecNowVisible.size() > 0 )
        iSelectIndex = m_vecNowVisible[0];

    m_iCurSelectIndex = iSelectIndex;
    m_vecNowVisible.clear();

    if( m_vecChar.empty() )
        return;

    int iCharSize = (int)m_vecChar.size();
    for( int i = 0; i < 3; ++i )
        m_vecNowVisible.push_back( ( iSelectIndex + i ) % iCharSize );
}

void KGCIndigoSelect::Init()
{
    static bool bFirst = true;
    if( bFirst )
    {
        bFirst = false;
        for( int i = 0; i < 3; ++i )
        {
            m_vecPos[i] = m_ppkCandChar[i]->GetFixedWindowLocalPos();
        }
    }

	m_vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;
	m_iCharNum = (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size();

	for( int i = 0; i < GC_CHAR_NUM; ++i )
	{
        if (m_ppkCharFace[i] == NULL)
            continue;

		m_ppkCharFace[i]->ToggleRender( false );
		m_ppkCandChar[i]->Lock( false );
		m_ppkCandChar[i]->InitState( false, true, this );
	}

	for( int i=0 ; i<m_iCharNum; ++i )
    {
		if (m_vecChar[i].iCharType< 0 || m_vecChar[i].iCharType >=GC_CHAR_NUM)
			continue;

		m_ppkCandChar[m_vecChar[i].iCharType]->InitState( true, true, this );
    }

	for( int i=0 ; i<3 ; ++i )
		m_ppkSelectChar[i]->InitState( false, false, NULL );

	memset( m_pCharSlot, -1, sizeof( m_pCharSlot ) );

    m_pkBtnRight->InitState( true, true, this );
    m_pkBtnLeft->InitState( true, true, this );

    InitNowVisible();
    CandidateVisible();
}

void KGCIndigoSelect::OnCreate( void )
{
    // 2007.10.23 wony
    // 로긴서버에 접속하는 타이밍에 init을 하다가..
    // 캐릭터 정보 없는데 접근해서 크래쉬 남..
    // 여기서 init 하지말쟈..=ㅅ =
	//Init();
}

void KGCIndigoSelect::SelectNowVisible( int iSelectIndex, bool bIncrease )
{
    m_vecNowVisible.clear();
    if( 3 == (int)m_vecChar.size() )
    {
        iSelectIndex = 0;
    }

    if( bIncrease )
    {
        if( ++iSelectIndex == (int)m_vecChar.size() )
            iSelectIndex = 0;
    }
    else
    {
        if( --iSelectIndex == -1 )
            iSelectIndex = (int)m_vecChar.size() - 1;
    }
    m_vecNowVisible.push_back( iSelectIndex );

    for( int i = 0; i < 2; ++i )
    {
        if( ++iSelectIndex == (int)m_vecChar.size() )
            iSelectIndex = 0;
        m_vecNowVisible.push_back( iSelectIndex );
    }

    CandidateVisible();
}

void KGCIndigoSelect::ActionPerformed( const KActionEvent& event )
{
    if( g_MyD3D->IsLocalPlayerObserver() )
        return;

    if( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        // 자동 시작 할 때에 시작 패킷 보내기 전까지 계속 캐릭터 변경을
        // 하면 문제가 생길 수 있어서 시작하기 3초전 부터는 캐릭터 변경을
        // 할 수 없도록 합니다.
        int iStartDelay = g_pkUIScene->RoomButtonGetStartDelay();
        if( iStartDelay != 0 && iStartDelay < GC_FPS_LIMIT * 3 /* 3초 */ )
            return;

        for( int i=0; i< m_iCharNum; ++i )
        {
            if( event.m_pWnd == m_ppkCandChar[m_vecChar[i].iCharType] && m_ppkCandChar[m_vecChar[i].iCharType]->IsRenderOn() )
            {
                OnCandChar( m_vecChar[i].iCharType, event.m_dwCode );
                break;
            }
        }

        for( int i=0; i < 3; ++i )
        {
            if( event.m_pWnd == m_ppkSelectChar[i] )
            {
                OnSelectChar( m_vecChar[i].iCharType, event.m_dwCode );
                break;
            }
        }

        if( event.m_pWnd == m_pkBtnRight )
        {
            SelectNowVisible( m_iCurSelectIndex, true );
            if( ++m_iCurSelectIndex == (int)m_vecChar.size() )
                m_iCurSelectIndex = 0;
        }
        if( event.m_pWnd == m_pkBtnLeft )
        {
            SelectNowVisible( m_iCurSelectIndex, false );
            if( --m_iCurSelectIndex == -1 )
                m_iCurSelectIndex = (int)m_vecChar.size() - 1;
        }
    }
}

void KGCIndigoSelect::PreDraw( void )
{
	// 이놈은 LINEAR로 필터링 합니다. (이미지 축소시 깨짐 현상 때문)
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

void KGCIndigoSelect::PostChildDraw( void )
{
	g_pGCDeviceManager2->PopState();
}

void KGCIndigoSelect::OnCandChar( int iIndex, DWORD dwCode )
{
	// 케릭터 선택
	if( dwCode == D3DWE_BUTTON_CLICK )
	{
		int iSlot = FindCharSlot();
		if( iSlot == -1 )
			return;

		m_ppkCandChar[iIndex]->Lock( true );
		m_ppkSelectChar[iSlot]->InitState( true, true, this );
		m_ppkCharFace[iIndex]->ToggleRender( true );
		m_ppkCharFace[iIndex]->SetFixedWindowLocalPos( m_ppkSelectChar[iSlot]->GetFixedWindowLocalPos() );
		m_pCharSlot[iSlot] = iIndex;
        g_kGlobalValue.m_kUserInfo.aiTagSlot[iSlot] = iIndex;

        KChangeRoomUserInfo kUserInfo;
        g_kGlobalValue.m_kUserInfo.SetChangeRoomUserInfo( &kUserInfo , KChangeRoomUserInfo::RUI_INDIGO_SLOT );
        for ( int i = 0; i < 3; i++ )
        {
            if ( m_pCharSlot[i] < 0 )
                continue;
            kUserInfo.m_acTagModeInfo[i][1] = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetPromotion( m_pCharSlot[i] );
        }
        kUserInfo.m_nUserSlot = g_MyD3D->Get_MyPlayer();
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( kUserInfo );
	}
}

void KGCIndigoSelect::OnSelectChar( int iIndex, DWORD dwCode )
{
	// 케릭터 취소
	if( dwCode == D3DWE_BUTTON_CLICK )
	{
		m_ppkSelectChar[iIndex]->InitState( false, false, NULL );
		m_ppkCharFace[m_pCharSlot[iIndex]]->ToggleRender( false );
		m_ppkCandChar[m_pCharSlot[iIndex]]->Lock( false );
		m_pCharSlot[iIndex] = -1;
        g_kGlobalValue.m_kUserInfo.aiTagSlot[iIndex] = -1;
		// 선택한 케릭터 갱신
        KChangeRoomUserInfo kUserInfo;
        g_kGlobalValue.m_kUserInfo.SetChangeRoomUserInfo( &kUserInfo , KChangeRoomUserInfo::RUI_INDIGO_SLOT );
        for ( int i = 0; i < 3; i++ )
        {
            if ( m_pCharSlot[i] < 0 )
                continue;
            kUserInfo.m_acTagModeInfo[i][1] = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetPromotion( m_pCharSlot[i] );
        }
        kUserInfo.m_nUserSlot = g_MyD3D->Get_MyPlayer();
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( kUserInfo );
	}
}

int KGCIndigoSelect::FindCharSlot()
{
	for( int i=0 ; i<3 ; ++i )
	{
		if( m_pCharSlot[i] == -1 )
			return i;
	}

	return -1;
}

void KGCIndigoSelect::UpdateSlot()
{
	Init();
	memcpy( m_pCharSlot, g_kGlobalValue.m_kUserInfo.aiTagSlot, 3*sizeof(g_kGlobalValue.m_kUserInfo.aiTagSlot[0]) );

	for( int i = 0; i < 3; ++i )
	{
		if( m_pCharSlot[i] > -1 && m_pCharSlot[i] < GC_CHAR_NUM )
		{
			m_ppkSelectChar[i]->InitState( true, true, this );
			m_ppkCharFace[m_pCharSlot[i]]->ToggleRender( true );
			m_ppkCharFace[m_pCharSlot[i]]->SetFixedWindowLocalPos( m_ppkSelectChar[i]->GetFixedWindowLocalPos() );

			m_ppkCandChar[m_pCharSlot[i]]->Lock( true );
		}
	}

    memcpy( g_kGlobalValue.m_kUserInfo.aiTagSlot, m_pCharSlot, sizeof(g_kGlobalValue.m_kUserInfo.aiTagSlot[0])*3 );
}

int KGCIndigoSelect::GetUsedSlotCount()
{
    int iUsedSlotCount = 0;
    for ( int i = 0; i < 3; i++ )
    {
        if ( m_pCharSlot[i] != -1 )
            iUsedSlotCount++;
    }
    return iUsedSlotCount;
}
