#include "CnKairo.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

CnKairo::CnKairo(void)
{
}

CnKairo::~CnKairo(void)
{
}

bool CnKairo::SetImageNum( IN int nNum_, IN float fProb_ )
{
    int iCaseID = m_mapImageInfo.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    m_mapImageInfo[iCaseID] = nNum_;

    return m_mapImageInfo.size() == m_kLottery.GetCaseNum();
}

bool CnKairo::Do( OUT int& nNum_ ) const
{
    int nDecision = m_kLottery.Decision();

    if( nDecision == KLottery::CASE_BLANK ) {
        nNum_ = 0;
        return true;   // 꽝. 변경할 아이템 없음.
    }

    std::map<int, int>::const_iterator mit;
    mit = m_mapImageInfo.find( nDecision );

    if( mit == m_mapImageInfo.end() ) {
        START_LOG( cerr, L"해당 인덱스 없음" )
            << BUILD_LOG( nDecision ) << END_LOG;
        return false;
    }

    nNum_ = mit->second;
    return true;
}