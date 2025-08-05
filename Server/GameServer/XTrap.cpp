#include "GSSimLayer.h"
#include "XTrap.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Worker.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KXTrap );
ImplOstmOperatorW2A( KXTrap );
NiImplementRootRTTI( KXTrap );

KXTrap::KXTrap(void)
:m_nCurrentIndex(0)
,m_dwClientTimeOut(300)
,m_dwTickGap(60000)
,m_strFileName("xtrap.CS3")
,m_dwLoadCount(0)
,m_bForce(false)
,m_bUseThemida(false)
{
    ::memset( m_ucaXTrapMap, 0, sizeof(m_ucaXTrapMap) );
    XTrap_S_FreeDll();
    m_dwLastTick = ::GetTickCount();
    m_tmLastLoad = KncUtil::TimeToInt( CTime::GetCurrentTime() );
}

KXTrap::~KXTrap(void)
{
    XTrap_S_FreeDll();
}

ImplToStringW( KXTrap )
{
    KLocker lock( m_csXTrap );
    std::wstring strFileName( KncUtil::toWideString(m_strFileName) );
    START_TOSTRINGW
        << TOSTRINGW( strFileName )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwClientTimeOut )
        << TOSTRINGW( m_nCurrentIndex )
        << TOSTRINGW( MAP_FILE_MAX_NUM )
        << TOSTRINGW( m_dwLoadCount )
        << TOSTRINGWt( CTime(m_tmLastLoad) );

    return stm_;
}

void KXTrap::SetConfig( IN const DWORD dwTickGap_, IN const DWORD dwTimeOut_, IN const char* szFileName_, bool useThemida_ )
{
    KLocker lock( m_csXTrap );
    m_dwClientTimeOut = dwTimeOut_;
    m_strFileName = szFileName_;
    m_dwTickGap = dwTickGap_;
    m_bUseThemida = useThemida_;
}

bool KXTrap::Init()
{
    _JIF( LoadMapFile( true ), return false );

    _JIF( XTrap_S_LoadDll() == XTRAP_API_RETURN_OK, return false );
    DWORD dwRet = XTrap_S_Start( GetClientTimeOut(), MAP_FILE_MAX_NUM, &m_ucaXTrapMap[0][0], NULL );
    if( dwRet != 0 )
    {
        START_LOG( cerr, L" Ret : " << dwRet ) << END_LOG;
        return false;
    }

    if ( m_bUseThemida == true ) {
        XTrap_S_SetActiveCode( XTRAP_ACTIVE_CODE_LEVEL2 ); 
    }

    return true;
}

bool KXTrap::LoadMapFile( bool bInit_ /*= false*/ )
{
    UCHAR ucaXTrapMap[XTRAP_CS4_BUFSIZE_MAP] = {0};

    FILE* pf = NULL;
    pf = fopen( m_strFileName.c_str(), "rb" );
    _JIF( pf, return false );
    DWORD dwRead = fread( ucaXTrapMap, XTRAP_CS4_BUFSIZE_MAP, 1, pf );
    fclose( pf );

    _JIF( dwRead == 1, return false );

    if( bInit_ )
    {
        for( int i = 0 ; i < MAP_FILE_MAX_NUM ; ++i )
            ::memcpy( m_ucaXTrapMap[i], ucaXTrapMap, XTRAP_CS4_BUFSIZE_MAP );
    }
    else
    {
        _JIF( UpdateMap( ucaXTrapMap ), return false );
    }
    return true;
}

bool KXTrap::UpdateMap( const UCHAR* pucBuff_ )
{
    _JIF( pucBuff_, return false );

    KLocker lock( m_csXTrap );

    // 비교 해서 다르면
    if( ::memcmp( m_ucaXTrapMap[GetPrevIndex()], pucBuff_, XTRAP_CS4_BUFSIZE_MAP ) == 0 ) return true;

    // 다음 인덱스에 카피.
    ::memcpy( m_ucaXTrapMap[GetCurrentIndex()], pucBuff_, XTRAP_CS4_BUFSIZE_MAP );

    // 현재 인덱스 변경.
    SetNextIndex();

    m_tmLastLoad = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    ++m_dwLoadCount;

    START_LOG( cerr, L"XTRAP CS3 맵파일 업데이트 완료, Loadcount : " << m_dwLoadCount )
        << END_LOG;

    return true;
}

int KXTrap::GetPrevIndex()
{
    return (m_nCurrentIndex + MAP_FILE_MAX_NUM - 1) % MAP_FILE_MAX_NUM;
}

int KXTrap::GetNextIndex()
{
    return (m_nCurrentIndex + 1) % MAP_FILE_MAX_NUM;
}

void KXTrap::SetNextIndex()
{
    m_nCurrentIndex = GetNextIndex();
}

DWORD KXTrap::GetClientTimeOut()
{
    KLocker lock( m_csXTrap );
    return m_dwClientTimeOut;
}

const void* KXTrap::GetMapBuff()
{
    KLocker lock( m_csXTrap );
    return m_ucaXTrapMap;
}

void KXTrap::Tick()
{
    if( !IsForceLoad() && ::GetTickCount() - m_dwLastTick < GetTickGap() ) return;
    m_dwLastTick = ::GetTickCount();
    m_bForce = false;
}
void KXTrap::LoadNow()
{
    m_bForce = true;
}