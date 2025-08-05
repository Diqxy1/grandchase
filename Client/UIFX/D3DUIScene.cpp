#include "stdafx.h"
#include "D3DUIScene.h"
//
//
#include "GCUI/KGCUIHelper.h"

IMPLEMENT_CLASSNAME( KD3DUIScene );

KD3DUIScene::KD3DUIScene( const std::string& strSceneName /* = "" */ )
{
    m_strSceneName      = strSceneName;
    m_eState            = D3DUIS_DISABLED;
    m_bActivationManage = false;
    m_bStop             = false;

    m_vecWnd.clear();
    m_mapLink.clear();
}

KD3DUIScene::~KD3DUIScene( void )
{
    m_vecWnd.clear();
    m_mapLink.clear();
}

HRESULT KD3DUIScene::FrameMove( void )
{
    PROFILE_SET("KD3DUIScene::FrameMove");

    /////////////////////////////////////////////////////////////////////
	// Date :  06-12-14 
	// milennium9@kogstudios.com 권상구
	//*******************************************************************
	// UI Helper의 시간조절 등을 위해서 framemove를 호출해 준다.
	/////////////////////////////////////////////////////////////////////
	g_pkUIHelper->FrameMove();

    switch ( m_eState )
    {
        case D3DUIS_DISABLED:
        {
            FrameMoveInDisabledState();

            break;
        }
        case D3DUIS_ENTER:
        {
            FrameMoveInEnterState();

			UINT i;
            for ( i = 0; i < m_vecWnd.size(); ++i )
            {
                if ( KD3DWnd::D3DWS_ENABLED == m_vecWnd[i]->GetWndState() )
				{
					break;
            }
            }

            if ( i == m_vecWnd.size() )
            {
                m_eState = D3DUIS_ENABLED;
                OnEnterComplete();
            }

            break;
        }
        case D3DUIS_ENABLED:
        {
            FrameMoveInEnabledState();

            if ( m_bActivationManage ) CheckMouseClick();

            break;
        }
        case D3DUIS_LEAVE:
        {
            FrameMoveInLeaveState();

			UINT i;
            for ( i = 0; i < m_vecWnd.size(); ++i )
            {
                if ( KD3DWnd::D3DWS_DISABLED == m_vecWnd[i]->GetWndState() )
				{
					break;
				}
            }

            if ( i == m_vecWnd.size() )
            {
                m_eState = D3DUIS_DISABLED;
                OnLeaveComplete();
            }

            break;
        }
    }
	if ( false == m_bStop || GetReverse() == false )
	{
        // 기본적으로 이벤트 처리는 앞에 그려지는 UI가 우선이기 때문에
        // 컨테이너를 역방향으로 이터레이션 하면서 FrameMove를 호출한다.
        // 하지만 초기화 문제로 인해서 Scene이 바뀔때 한번은 정방향으로
        // FrameMove를 돌려줘야 정상적으로 동작한다.

        if( m_bReverse ) {
            for ( int i = static_cast<int>(m_vecWnd.size()) - 1; i >= 0; --i )
            {
                m_vecWnd[i]->FrameMove();
            }
        }
        
        if( !m_bReverse ){
            for ( int i = 0 ; i < static_cast<int>(m_vecWnd.size()) ; ++i )
            {
                m_vecWnd[i]->FrameMove();
            }
            SetReverse( true );
        }
	}

    return S_OK;
}

HRESULT KD3DUIScene::Render( void )
{
    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        m_vecWnd[i]->Render();
    }

    return S_OK;
}

void KD3DUIScene::AddWindow( KD3DWnd* pWnd )
{
    pWnd->SetUIScene( this );

    if ( 0 != m_mapLink.count( pWnd->GetWindowName() ) )
    {
        *m_mapLink[pWnd->GetWindowName()] = pWnd;
    }

    if ( pWnd->IsTopMost() )
    {
        m_vecWnd.push_back( pWnd );
    }
    else
    {
		UINT i;
        for ( i = 0; i < m_vecWnd.size(); ++i )
        {
            if ( m_vecWnd[i]->IsTopMost() )
			{
				break;
        }
        }
        m_vecWnd.insert( m_vecWnd.begin() + i, pWnd );
    }
}

void KD3DUIScene::ResetResolution()
{
    for ( std::vector<KD3DWnd*>::iterator it = m_vecWnd.begin(); it != m_vecWnd.end(); ++it ) {
        if( (*it) != NULL ){
            (*it)->ResetUIforResolution();
        }
	}
}

void KD3DUIScene::RemoveAllWindow( void )
{
    OnClear();
    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        m_vecWnd[i]->RemoveAllChild(true);
        SAFE_DELETE( m_vecWnd[i] );
    }
    m_vecWnd.clear();
}

void KD3DUIScene::EnterUIScene( void )
{

    m_eState = D3DUIS_ENTER;

    OnEnter();

    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        if ( m_vecWnd[i]->IsAutoCreate() ) 
			m_vecWnd[i]->Create( true );
    }
}

void KD3DUIScene::LeaveUIScene( void )
{
    m_eState = D3DUIS_LEAVE;

    OnLeave();

    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        m_vecWnd[i]->Destroy();
    }
}

void KD3DUIScene::SetActiveWindow( KD3DWnd* pWnd )
{
    UINT iActiveWnd     = (UINT)m_vecWnd.size();
    UINT iActiveFriend  = (UINT)m_vecWnd.size();
    //std::string strFriend = pWnd->GetFriendWindowName();
	std::string strFriend = "";

    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        if ( m_vecWnd[i] == pWnd )
        {
            m_vecWnd[i]->Activate( true );
            iActiveWnd = i;
        }
        else if ( !strFriend.empty() && m_vecWnd[i]->GetWindowName() == strFriend )
        {
            m_vecWnd[i]->Activate( true );
            iActiveFriend = i;
        }
        else
        {
            m_vecWnd[i]->Activate( false );
        }
    }

    KD3DWnd* pFriend = ( iActiveFriend == m_vecWnd.size() ? NULL : m_vecWnd[iActiveFriend] );

    if ( iActiveWnd != m_vecWnd.size() )
    {
        m_vecWnd.erase( m_vecWnd.begin() + iActiveWnd );

        if ( pWnd->IsTopMost() )
        {
            m_vecWnd.push_back( pWnd );
        }
        else
        {
			UINT i;
            for ( i = 0; i < m_vecWnd.size(); ++i )
            {
                if ( m_vecWnd[i]->IsTopMost() )
				{
					break;
				}
            }

            m_vecWnd.insert( m_vecWnd.begin() + i, pWnd );
        }
    }

    if ( pFriend != NULL ) // activate friend window if it exist.
    {
        for ( UINT i = 0; i < m_vecWnd.size(); ++i )
        {
            if ( pFriend == m_vecWnd[i] )
            {
                m_vecWnd.erase( m_vecWnd.begin() + i );
                break;
            }
        }

        if ( pFriend->IsTopMost() )
        {
            m_vecWnd.push_back( pFriend );
        }
        else	
        {
			UINT i;
            for ( i = 0; i < m_vecWnd.size(); ++i )
            {
                if ( m_vecWnd[i]->IsTopMost() ) 
				{
					break;
				}
            }

            m_vecWnd.insert( m_vecWnd.begin() + i, pFriend );
        }
    }
}

void KD3DUIScene::CheckMouseClick( void )
{
	return;
    D3DXVECTOR2 vMousePos( (float)(g_pkInput->GetMousePos().x), (float)(g_pkInput->GetMousePos().y) );
    BOOL        bClick = g_pkInput->BtnUp( KInput::MBLEFT );

    for ( int i = (int)m_vecWnd.size() - 1; i >= 0; --i )
    {
        if ( m_vecWnd[i]->CheckPosInWindowBoundWithChild( vMousePos ) && bClick )
        {
            if ( false == m_vecWnd[i]->IsActive() )
            {
                SetActiveWindow( m_vecWnd[i] );
            }
            return;
        }
    }

    if ( bClick ) ::SetFocus( g_hUIMainWnd );
}

void KD3DUIScene::Stop( bool bStop )
{
    m_bStop = bStop;
}

bool KD3DUIScene::IsStop( void ) const
{
    return m_bStop;
}



void KD3DUIScene::SetChildOrder( IN KD3DWnd* pScrWnd_, IN KD3DWnd* pTargetWnd_, bool bNext )
{
    //일단 두 윈도우 포인터가 유효해야 함.
    std::vector<KD3DWnd*>::iterator itorScr = std::find( m_vecWnd.begin(), m_vecWnd.end(), pScrWnd_ );
    if( itorScr == m_vecWnd.end() )
        return;

    itorScr = m_vecWnd.erase( itorScr );

    std::vector<KD3DWnd*>::iterator itorTarget = std::find( m_vecWnd.begin(), m_vecWnd.end(), pTargetWnd_ );
    if( itorTarget == m_vecWnd.end() ) {
        m_vecWnd.insert( itorScr, pScrWnd_ );
        return;
    }

    // erase 함수 때문에 iterator가 무효화 되었음으로 재 검색
    itorTarget = std::find( m_vecWnd.begin(), m_vecWnd.end(), pTargetWnd_ );
    m_vecWnd.insert( (bNext?++itorTarget:itorTarget), pScrWnd_ );
}

void KD3DUIScene::SetChildOrderTop( IN KD3DWnd* pScrWnd_ )
{
    std::vector<KD3DWnd*>::iterator itorScr = std::find( m_vecWnd.begin(), m_vecWnd.end(), pScrWnd_ );
    if( itorScr == m_vecWnd.end() )
        return;

    m_vecWnd.erase( itorScr );
    m_vecWnd.push_back( pScrWnd_ );
}

void KD3DUIScene::SetChildOrderBottom( IN KD3DWnd* pScrWnd_ )
{
    std::vector<KD3DWnd*>::iterator itorScr = std::find( m_vecWnd.begin(), m_vecWnd.end(), pScrWnd_ );
    if( itorScr == m_vecWnd.end() )
        return;

    m_vecWnd.erase( itorScr );
    m_vecWnd.insert( m_vecWnd.begin(), pScrWnd_ );
}

int KD3DUIScene::GetMemorySize()
{
    int nTotalSize = 0;
#if !defined( __PATH__ )

    for ( int i = 0; i < (int)m_vecWnd.size(); ++i ) 
    {
        nTotalSize += m_vecWnd[i]->GetMemorySize();
    }
#endif

    return nTotalSize;
}