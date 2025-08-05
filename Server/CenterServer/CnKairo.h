#pragma once

#include "Lottery.h"

class CnKairo
{
public:
    CnKairo(void);
    ~CnKairo(void);

    bool SetImageNum( IN int nNum, IN float fProb );
    bool Do( OUT int& nNum ) const;

protected:
    std::map< int, int > m_mapImageInfo;
    KLottery m_kLottery;
};
