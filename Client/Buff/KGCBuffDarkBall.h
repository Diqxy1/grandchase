#pragma once

#include "KGCBuff.h"
#define DEFAULT_COOL 10

class KGCBuffDarkBall : public KGCBuff
{
public:
	KGCBuffDarkBall(void);
	~KGCBuffDarkBall(void);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

protected:
	int m_iCoolTime;
};
