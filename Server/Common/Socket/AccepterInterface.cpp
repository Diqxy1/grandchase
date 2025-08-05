#include "AccepterInterface.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

IAccepter::IAccepter()
{
	m_kMaxTimeInfo.m_dwTerm = 0;
	m_kMaxTimeInfo.m_cTime = CTime::GetCurrentTime();
	// m_kMaxTimeInfo의 나머지 변수들은 생성자가 따로 있으므로 초기화해줄 필요 없다.
}

void IAccepter::DumpMaxTimeInfo(OUT std::wostream& stm_)
{
	KLocker lock(m_kMaxTimeInfo.m_cs);

	stm_ << " -- Max accepter waiting time. NagleOn : "
		<< std::boolalpha << GetNagleOpt() << std::noboolalpha << std::endl
		<< "  class : " << m_strClassName << std::endl
		<< "  user IP : " << m_kMaxTimeInfo.m_strIP << std::endl
		<< "  duration : " << m_kMaxTimeInfo.m_dwTerm << std::endl
		<< "  executed time : " << (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void IAccepter::ResetMaxTimeInfo()
{
	KLocker lock(m_kMaxTimeInfo.m_cs);

	m_kMaxTimeInfo.m_dwTerm = 0;
	m_kMaxTimeInfo.m_cTime = CTime::GetCurrentTime();
	m_kMaxTimeInfo.m_strIP = (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_kMaxTimeInfo.m_strIP.append(L" 에 데이터 초기화 되었습니다.");
}

bool IAccepter::GetNagleOpt()
{
	START_LOG(cerr, L"Pure Virtual Function Call... Check Please....") << END_LOG;
	return true;
}