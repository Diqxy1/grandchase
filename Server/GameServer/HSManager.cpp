#include "GSSimLayer.h"
#include "HSManager.h"
#include <Serializer/SerBuffer.h>
#include <dbg/dbg.hpp >
#include <iomanip>
#include "Worker.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KHSManager );
ImplOstmOperatorW2A( KHSManager );
NiImplementRootRTTI( KHSManager );

KHSManager::KHSManager(void)
:m_hServer(NULL)
,m_dwTickGap(60000)
,m_bForce(false)
,m_strFileName("AntiCpx.hsb")
{
    m_dwLastTick = ::GetTickCount();
}

KHSManager::~KHSManager(void)
{
    ShutDown();
}

ImplToStringW( KHSManager )
{
    KLocker lock( m_csHSield );
    std::wstring strFileName( KncUtil::toWideString(m_strFileName) );

    return START_TOSTRINGW
        << TOSTRINGW( strFileName )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGWb( m_bForce );
}

bool KHSManager::Init()
{
    if( m_hServer != NULL )
    {
        START_LOG( cerr, L"핵쉴드 서버 핸들이 초기화 되어 있지 않음." ) << END_LOG;
        return false;
    }

    std::string strPath(".\\");
    strPath += m_strFileName;

    m_hServer = ::_AhnHS_CreateServerObject( strPath.c_str() );
    if ( m_hServer == ANTICPX_INVALID_HANDLE_VALUE ) 
    {
        START_LOG( cerr, L"핵쉴드 초기화 실패...." ) << END_LOG;
        return false; 
    }
    return true;
}

void KHSManager::SetConfig( IN const DWORD dwTickGap_, IN const char* szFileName_ )
{
    KLocker lock( m_csHSield );
    m_dwTickGap     = dwTickGap_;
    m_strFileName   = szFileName_;
}

void KHSManager::ShutDown()
{
    if( m_hServer == 0 ) return;
    ::_AhnHS_CloseServerHandle( m_hServer );
    m_hServer = 0;
}

AHNHS_CLIENT_HANDLE KHSManager::CreateClientHandle()
{
    AHNHS_CLIENT_HANDLE hClient = ANTICPX_INVALID_HANDLE_VALUE;

    do
    {
        if( m_hServer == NULL )
        {
            START_LOG( cerr, L"핵쉴드 서버 핸들이 생성 되어 있지 않음." ) << END_LOG;
            break;
        }

        hClient = ::_AhnHS_CreateClientObject ( m_hServer ); 

        if ( hClient == NULL ) 
        { 
            START_LOG( cerr, L"클라이언트 핸들 생성 실패." ) << END_LOG;
            break;
        }

    } while (false);

    return hClient;
}

bool KHSManager::MakeRequest( IN AHNHS_CLIENT_HANDLE hClient_, OUT KSerBuffer& buff_, IN const std::wstring& strLogin_ )
{
    if( hClient_ == 0 )
    {
        START_LOG( cerr, L"핸들이 초기화 되지 않음.. Name : " << strLogin_ ) << END_LOG;
        return false;
    }

    buff_.Clear();
    AHNHS_TRANS_BUFFER stRequestBuf;
    ::memset( &stRequestBuf, 0, sizeof(stRequestBuf) );
    DWORD dwRet = ::_AhnHS_MakeRequest ( hClient_, &stRequestBuf ); 

    if ( dwRet != ERROR_SUCCESS ) 
    {
        std::wstringstream stm;
        stm.setf( std::ios::hex, std::ios::basefield );
        stm << std::setw(8) << std::setfill(L'0') << dwRet;
        START_LOG( cerr, L"Make Request Fail...Value : " << stm.str() )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        return false;
    }

    buff_.Write( &stRequestBuf, sizeof(stRequestBuf) );
    return true;
}

bool KHSManager::VerifyResponse( IN AHNHS_CLIENT_HANDLE hClient_, IN KSerBuffer& buff_, IN const std::wstring& strLogin_ )
{
    if( hClient_ == 0 )
    {
        START_LOG( cerr, L"핸들이 초기화 되지 않음.. Name : " << strLogin_ ) << END_LOG;
        return false;
    }

    AHNHS_TRANS_BUFFER stResponseBuf;
    buff_.Reset(); // offset 초기화.
    LIF( buff_.Read( &stResponseBuf, sizeof( stResponseBuf ) ) );
    DWORD dwError = 0;
    DWORD dwRet = ::_AhnHS_VerifyResponseEx ( hClient_, stResponseBuf.byBuffer, stResponseBuf.nLength, &dwError ); 

    if ( dwRet == ANTICPX_RECOMMAND_CLOSE_SESSION ) 
    {
        std::wstringstream stm;
        stm.setf( std::ios::hex, std::ios::basefield );
        stm << std::setw(8) << std::setfill(L'0') << dwError;
        START_LOG( cerr, L"응답 검증 실패.. Value : " << dwError )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        return false;
    }

    return true;
}

void KHSManager::CloseClientHandle( OUT AHNHS_CLIENT_HANDLE& hClient_ )
{
    if( hClient_ == 0 ) return;
    ::_AhnHS_CloseClientHandle ( hClient_ );
    hClient_ = 0;
}

void KHSManager::Tick()
{
    if( !m_bForce && ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;

    m_dwLastTick = ::GetTickCount();
    m_bForce = false;
}