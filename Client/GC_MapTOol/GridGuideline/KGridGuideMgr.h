#pragma once

#include "stdafx.h"
//#include "../WOGMapTOol.h"
#include "KGridGuide.h"
#include <KNCSingleton.h>
//#include <vector>
//#include "../KGCMap.h"

class KGridGuideMgr
{
	DeclareSingleton(KGridGuideMgr);

public:
	KGridGuideMgr(void);
	virtual ~KGridGuideMgr(void);

	void AddGridGuide(D3DVECTOR MPos, bool bVertical);
	bool AddGridGuide(KGridGuide* pGuide);
	void BuildGridGuide();
	void BuildFH();
	void FrameMove();
	void clear();
	HRESULT RenderFH();
	HRESULT Render();
	KGridGuide* GetMouseOverGuide(const D3DVECTOR& MPos);

	// getter, setter
	inline void SetGuideTerm(int iTerm) { m_fGuideTerm = static_cast<float>(iTerm * FOOTHOLDSIZE); }
	float GetGuideTerm() const { return m_fGuideTerm; }

private:
	float   m_fGuideTerm;
	std::vector< KGridGuide* > m_vecFH;
	std::vector< KGridGuide* > m_vecDefaultGuide;
	std::vector< KGridGuide* > m_vecUserGuide;
};

DefSingletonInline(KGridGuideMgr);
