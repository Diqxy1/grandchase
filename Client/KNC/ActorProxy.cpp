#include "stdafx.h"
#include "ActorProxy.h"


NiImplementRTTI( KActorProxy, KSession );


KActorProxy::KActorProxy() 
:
    KSession( true ), 
    m_bThrowWinMsg( false ),
    m_nAckOK( -1 ),
    m_iClassID( 0 )
{
    m_spEventAck.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
}

ImplToStringW( KActorProxy )
{
    return START_TOSTRING_PARENTW( KSession );
}

void KActorProxy::PostEvent( KEventPtr spEvent_ )
{
    // SendMessage하면 메세지가 처리될때까지 기다리지만, PostMessage이므로 큐잉만 한다.
    // cmd : WM_APP + class ID
    // wParam : Event ID
    // lParam : data 의 포인터

    if( m_bThrowWinMsg )
    {
        dbg::clog << L"post message : " << spEvent_->GetEventIDString() << dbg::endl;

        KSerBuffer* pBuff = new KSerBuffer( spEvent_->m_kbuff );
        
        ::PostMessage( m_hWnd, WM_APP + m_iClassID,
            static_cast<WPARAM>(spEvent_->m_usEventID),
            reinterpret_cast<LPARAM>(pBuff) );
    }
    else
    {
        KLocker lock( m_csRecvMsg );
        m_queRecvMsg.push( spEvent_ );
    }
}

void KActorProxy::SetHwnd( HWND hWnd_ )
{
    m_hWnd = hWnd_;

    if( ::IsWindow( m_hWnd ) )
        m_bThrowWinMsg = true;
}

void KActorProxy::SetThrowWinMsg( bool bThrowWinMsg_ )
{
    m_bThrowWinMsg = bThrowWinMsg_;

    if( m_bThrowWinMsg && ::IsWindow( m_hWnd ) == FALSE )
    {
        START_LOG( cerr, L"Invalid Window Handle" );
        assert( !"KUserProxy에 올바른 HWND를 설정하세요" );
    }
}

int KActorProxy::ConnectNAuth( IN const KServerInfo& kServerInfo_, 
                               IN const KEventPtr spEvent_, 
                               IN DWORD dwTimeOut_ )
{
    if( !KSession::Connect( kServerInfo_.m_strConnIP.c_str(), kServerInfo_.m_usPort, true ) )
    {
        START_LOG( cerr, L"game server connect failed. IP : " 
            << kServerInfo_.m_strConnIP
            << ":" << kServerInfo_.m_usPort );
        return -1;
    }

    KSerializer ks;
    KSerBuffer kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent_ );
    ks.EndWriting();

    if( ! KSession::SendPacket( kbuff, L"Authentification request packet." ) )
    {
        START_LOG( cerr, L"sending verify account req packet failed." );
        return -2;
    }

    m_nAckOK = -99; // unused-error code.

    if( m_bUseIocp )    // IOCP를 사용하는 경우 자체 스레드가 없다.
    {
        DWORD dwTick = ::GetTickCount();
        while( GetTickCount() - dwTick < dwTimeOut_ && m_nAckOK == -99 )
        {
            Sleep( 30 );
            Tick();
        }

        if( m_nAckOK != -99 )   // ack를 제대로 받은 경우.
            return m_nAckOK;
    }
    else
    {
        switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) ) {
        case WAIT_OBJECT_0: // ack를 제대로 받은 경우.
            return m_nAckOK;
            break;
        case WAIT_TIMEOUT: // 시간 초과
            START_LOG( cerr, L"verify account timeout." );
            break;
        default:
            START_LOG( cerr, L"invalide event object." );
        }
    }

    return -3;
}
