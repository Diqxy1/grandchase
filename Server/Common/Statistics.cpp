#include ".\statistics.h"
#include <time.h>
#define AN_HOUR     1000*60*60
KStatistics::KStatistics(void)
	:m_nSIID(-1)
	, m_dwFlushGap(-1)
	, m_dwRowCount(0xffffffff)
	, m_bWriteToDB(false)
{
	ResetTime();
}

KStatistics::~KStatistics(void)
{
}

void KStatistics::Init(int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_)
{
	m_nSIID = nID_;
	m_dwFlushGap = nFlushGap_;
	m_bWriteToDB = bWriteToDB_;
	if (nRowCount_ <= 0)
		m_dwRowCount = 0xffffffff;
	else
		m_dwRowCount = (DWORD)nRowCount_;
}

DWORD KStatistics::GetElapsed()
{
	return m_dwStartTime - ::GetTickCount();
}

void KStatistics::ResetTime()
{
	m_dwStartTime = ::GetTickCount();
}

void KStatistics::SetDesc(const std::wstring& strTile_, const std::wstring& strDesc_)
{
	m_strTitle = strTile_;
	m_strDesc = strDesc_;
}

void KStatistics::Dump(std::wostream& stm_)
{
	stm_ << L" ID : " << m_nSIID
		<< L"(" << m_strTitle << L")"
		<< L", Flush Gap : " << m_dwFlushGap
		<< L", Max RowCount : " << m_dwRowCount
		<< L", DB Write : " << (m_bWriteToDB ? L"true" : L"false");
}