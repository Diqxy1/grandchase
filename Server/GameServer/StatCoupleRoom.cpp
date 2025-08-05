#include "StatCoupleRoom.h"
#include <dbg/dbg.hpp>

KStatCoupleRoom::KStatCoupleRoom(void)
{
    ClearData();
}

KStatCoupleRoom::~KStatCoupleRoom(void)
{
}

void KStatCoupleRoom::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogFileCouple.CloseFile();
    m_klogFileCouple.OpenFile( std::wstring ( L"CoupleRoom" ) );
    ClearData();
}

void KStatCoupleRoom::ClearData()
{
    m_dwCoupleUserOpen  = 0;
    m_dwOtherUserOpen   = 0;
}


void KStatCoupleRoom::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatCoupleRoom::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Couple : " << m_dwCoupleUserOpen
        << L", Other : " << m_dwOtherUserOpen << std::endl;
    ClearData();

}


void KStatCoupleRoom::Flush()
{
    KLocker lock( m_csStat );
    m_klogFileCouple << KNC_TIME_STRING << GLOG::endl;
    m_klogFileCouple << L"Couple : " << m_dwCoupleUserOpen
        << L", Other : " << m_dwOtherUserOpen << GLOG::endl;
    ClearData();
}

void KStatCoupleRoom::AddStat( bool bCouple  )
{
    KLocker lock( m_csStat );
    if( bCouple )
    {
        ++m_dwCoupleUserOpen;
    }
    else
    {
        ++m_dwOtherUserOpen;
    }
}