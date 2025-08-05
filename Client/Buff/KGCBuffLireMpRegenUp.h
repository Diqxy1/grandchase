#pragma once

#include "KGCBuff.h"

class KGCBuffLireMpRegenUp : public KGCBuff
{
public:
	KGCBuffLireMpRegenUp(void);
	~KGCBuffLireMpRegenUp(void);

	//설정 해제
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

protected:
};
