#pragma once
#include "damage.h"

class CHardDamageKazeazeDarkBall :public CDamage
{
public:
	CHardDamageKazeazeDarkBall(void);
	virtual ~CHardDamageKazeazeDarkBall(void);
	virtual void FrameMove(CDamageInstance* pDamageInstance );

	float m_fBrokenTime;
};
