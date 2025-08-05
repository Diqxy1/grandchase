#pragma once

#include "KGCBuff.h"
#include "KncP2PLib/KncCommonDefine.h"
#include "GCUtil\GCRandomObj.h"

class KGCBuffHeroEarring01 : public KGCBuff
{
public:
    KGCBuffHeroEarring01(void);
    ~KGCBuffHeroEarring01(void);

    //파티클, 에니메이션, 카툰 설정 등등
    virtual bool Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */ );
	virtual bool FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ );
	virtual bool Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ );

protected:
    int                 m_iPeriodTime;
    int                 m_iShieldTime;
    int                 m_iDungeonShieldHP;
    int                 m_iOtherShieldHP;
    int                 m_iProbability;
    int                 m_iShieldType;
    std::map<int,KncStopWatch>        m_mapStopWatch;    
    static GCUTIL::GCRand_Int   ms_kRandom;

    bool                m_bSetShildType;
};

