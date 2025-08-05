#include "StatRoomJoin.h"
#include <dbg/dbg.hpp>
#include "Log4.h"


KStatRoomJoin::KStatRoomJoin(void)
:m_bFileExist(false)
{
    m_mapRoomJoinCount.clear();
}

KStatRoomJoin::~KStatRoomJoin(void)
{
}

void KStatRoomJoin::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_kRoomJoin.CloseFile();
    m_bFileExist = m_kRoomJoin.OpenFile( std::wstring( L"RoomJoinKind" ) );

}

void KStatRoomJoin::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    START_LOG( clog , L" m_mapRoomJoinCount : GetDataSize : " << m_mapRoomJoinCount.size() ) << END_LOG;

    Flush();
    ResetTime();
}

void KStatRoomJoin::AddStat( UINT joinType_ )
{
    KLocker lock( m_csStat );

    m_mapRoomJoinCount[joinType_] += 1;
}

void KStatRoomJoin::Flush()
{
    KLocker lock( m_csStat );

    JIF( m_bFileExist );
    if ( m_mapRoomJoinCount.empty() ) {
        return;
    }

    m_kRoomJoin << L"[" << KNC_TIME_STRING << L"]" << GLOG::endl;

    std::map< UINT, DWORD >::const_iterator mit;

    for( mit = m_mapRoomJoinCount.begin(); mit != m_mapRoomJoinCount.end(); ++mit ) {
        std::wstring strTitle;
        if ( mit->first == 0 ) {
            strTitle = L"Normal Join ";
        }
        else {
            strTitle = L"Quick Join ";
        }
        m_kRoomJoin << strTitle
            << L"Count : " << mit->second << L"  | ";
    }
    m_kRoomJoin << GLOG::endl;

    m_mapRoomJoinCount.clear();
}

void KStatRoomJoin::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapRoomJoinCount.size() << std::endl;
}
