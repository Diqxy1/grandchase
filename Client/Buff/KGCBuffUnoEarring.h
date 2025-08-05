#pragma once
#include "KGCBuff.h"
#include "KncP2PLib/KncCommonDefine.h"
#include "GCUtil\GCRandomObj.h"
class KGCBuffUnoEarring : public KGCBuff
{
public:
    KGCBuffUnoEarring(void);
    ~KGCBuffUnoEarring(void);
    //파티클, 에니메이션, 카툰 설정 등등
    virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID);
    virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
    virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);
protected:
    int                 m_iPeriodTime;
    int                 m_iShieldTime;
    int                 m_iDungeonShieldHP;
    int                 m_iOtherShieldHP;
    int                 m_iProbability;
    std::map<int, KncStopWatch>        m_mapStopWatch;
    bool                m_bSetShildType;
};