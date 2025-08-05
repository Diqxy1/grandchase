#pragma once
#include <WinSock2.h>   // wsaBuffer

#include "AccepterInterface.h"

#include "NetCommon.h"  // CLOSE_SOCKET
#include "IOCP.h"
#include "dbg/dbg.hpp"
#include "KncException.h"

class KSession;
template< class T, bool bNagleAlgOn = true>
class KAccepter : public IAccepter
{
public:
    KAccepter(u_short usPort);
    virtual ~KAccepter(void) {}
    void ShutDown() { End(); }
    virtual bool GetNagleOpt() { return bNagleAlgOn; }
    virtual void End(DWORD dwTimeOut = 3000);

protected:
    virtual void Run();
    inline void Loop();
    inline bool Init();

protected:
    SOCKET          m_sock;
};

template< class T, bool bNagleAlgOn >
KAccepter<T, bNagleAlgOn>::KAccepter(u_short usPort)
{
    m_strClassName = T::m_RTTI.GetName();

    m_sock = ::WSASocket(AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP,
        NULL,
        0,
        WSA_FLAG_OVERLAPPED); // overlapped I/O

    if (m_sock == INVALID_SOCKET)
    {
        ::WSACleanup();
        return;
    } // if

    // server restart�� ���� ��� ������ address�� ����� �� �ְ� ����Ѵ�.
    char bOK = true;
    int nRet;
    nRet = ::setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &bOK, sizeof(char));

    SOCKADDR_IN local;  // Ư�� port�� binding�� �Ѵ�.
    local.sin_family = AF_INET;

    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(usPort);

    if (::bind(m_sock, (SOCKADDR*)&local, sizeof(local)) == SOCKET_ERROR)
    {
        CLOSE_SOCKET(m_sock);
        ::WSACleanup();
        return;
    } // if

    if (SOCKET_ERROR == ::listen(m_sock, SOMAXCONN))
    {
        ::shutdown(m_sock, SD_BOTH);
        ::closesocket(m_sock);
        m_sock = INVALID_SOCKET;
        ::WSACleanup();
        return;
    } // if

    return;
}

template< class T, bool bNagleAlgOn >
void KAccepter<T, bNagleAlgOn>::End(DWORD dwTimeout_ /*dwTimeout = 3000*/)
{
    if (NULL != m_hThread)
    {
        CLOSE_SOCKET(m_sock);     // �߰�. accept ���� ������ �ݴ´�.

        SetEvent(m_hKillEvent);
        ::WaitForSingleObject(m_hThread, dwTimeout_);
        std::cout << "close KAccepter Thread.\n" << std::endl;
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    } // if
} // End()

template< class T, bool bNagleAlgOn >
void KAccepter<T, bNagleAlgOn>::Run()
{
    DWORD ret;

    while (true)
    {
        ret = ::WaitForSingleObject(m_hKillEvent, 0);     // 0.5s ����

        if (ret == WAIT_OBJECT_0) break;

        if (ret == WAIT_TIMEOUT) Loop();

        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

    }// while
}

template< class T, bool bNagleAlgOn >
void KAccepter<T, bNagleAlgOn>::Loop()
{
    SOCKET      socket = INVALID_SOCKET;
    SOCKADDR_IN sinRemote;
    int         size = sizeof(sinRemote);

    socket = ::accept(m_sock, (SOCKADDR*)&sinRemote, &size);

    if (socket == INVALID_SOCKET)
    {
        START_LOG(cerr, L"INVALID_SOCKET. WSAError : " << GET_WSA_MSG) << END_LOG;
        return;
    }

    if (!bNagleAlgOn)    // Nagle option�� ����. �����͸� ��Ƽ� ������ �ʰ� �� ��� �����ϵ��� �Ѵ�.
    {
        int sock_opt = 1;
        if (::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&sock_opt, sizeof(sock_opt)) == SOCKET_ERROR)
        {
            START_LOG(cerr, GET_WSA_MSG)
                << BUILD_LOG(WSAGetLastError()) << END_LOG;
            return;
        }
    }

    std::shared_ptr<T> spActorChild = T::Create();
    JIF(spActorChild);

    KSocketObject* pkSockObj = spActorChild->GetSockObj();

    if (!pkSockObj)
    {
        START_LOG(cerr, L"Create Actor Failed") << END_LOG;
        CLOSE_SOCKET(socket);
        return;
    }

    pkSockObj->SetSocketInfo(socket, sinRemote);
    LIF(SiKIocp()->AssociateSocket(spActorChild));
    pkSockObj->InitRecv();
    pkSockObj->OnAcceptConnection();

    DWORD dwElapTime = ::GetTickCount();  // 040428. ó�� �ҿ�ð��� ���Ѵ�.( �޴����� ���� �ð� ����)

    T::AddManager(spActorChild);

    dwElapTime = ::GetTickCount() - dwElapTime;

    {   // locking scope
        KLocker lock(m_kMaxTimeInfo.m_cs);

        if (dwElapTime > m_kMaxTimeInfo.m_dwTerm)
        {
            m_kMaxTimeInfo.m_strIP = KncUtil::toWideString(inet_ntoa(sinRemote.sin_addr));
            m_kMaxTimeInfo.m_cTime = CTime::GetCurrentTime();
            m_kMaxTimeInfo.m_dwTerm = dwElapTime;

            START_LOG(clog, L"accepter ��ü ���� ���ð� ����. Remote IP : " << m_kMaxTimeInfo.m_strIP)
                << BUILD_LOG(m_kMaxTimeInfo.m_dwTerm) << END_LOG;
        }
    }   // locking scope
}
