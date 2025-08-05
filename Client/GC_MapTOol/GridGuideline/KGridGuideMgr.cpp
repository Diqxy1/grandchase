#include ".\KGridGuideMgr.h"

ImplementSingleton(KGridGuideMgr);

KGridGuideMgr::KGridGuideMgr(void)
	: m_fGuideTerm(static_cast<float>(static_cast<int>(FOOTHOLDSIZE*5.0f)))
{
	m_vecDefaultGuide.clear();
	m_vecUserGuide.clear();
	m_vecFH.clear();
}

KGridGuideMgr::~KGridGuideMgr(void)
{
	clear();
}

void KGridGuideMgr::clear()
{
	for (int i = 0; i < (int)m_vecFH.size(); ++i)
	{
		SAFE_DELETE(m_vecFH[i]);
	}
	m_vecFH.clear();
	for (int i = 0; i < (int)m_vecDefaultGuide.size(); ++i)
	{
		SAFE_DELETE(m_vecDefaultGuide[i]);
	}
	m_vecDefaultGuide.clear();
	for (int i = 0; i < (int)m_vecUserGuide.size(); ++i)
	{
		SAFE_DELETE(m_vecUserGuide[i]);
	}
	m_vecUserGuide.clear();
}

void KGridGuideMgr::FrameMove()
{
	std::vector< KGridGuide* >::iterator vit;
	for (vit = m_vecUserGuide.begin(); vit != m_vecUserGuide.end(); )
	{
		KGridGuide* pGuide = (*vit);
		D3DXVECTOR2 vPos = pGuide->GetGridGuidePos();
		if (vPos.x < -FOOTHOLDSIZE || vPos.x > SiKGCMap()->GetPixelWidth() + FOOTHOLDSIZE ||
			vPos.y < -FOOTHOLDSIZE || vPos.y > SiKGCMap()->GetPixelHeight() + FOOTHOLDSIZE)
		{
			vit = m_vecUserGuide.erase(vit);
			g_AppFormView->GetRenderView()->ClearCurGridGuide();
		}
		else ++vit;
	}
}

HRESULT KGridGuideMgr::RenderFH()
{
	std::for_each(m_vecFH.begin(), m_vecFH.end(),
		boost::bind(&KGridGuide::RenderFH, _1));

	return S_OK;
}

HRESULT KGridGuideMgr::Render()
{
	std::for_each(m_vecDefaultGuide.begin(), m_vecDefaultGuide.end(),
		boost::bind(&KGridGuide::Render, _1));
	std::for_each(m_vecUserGuide.begin(), m_vecUserGuide.end(),
		boost::bind(&KGridGuide::Render, _1));

	return S_OK;
}

void KGridGuideMgr::BuildFH()
{
	for (int i = 0; i < (int)m_vecFH.size(); ++i)
	{
		SAFE_DELETE(m_vecFH[i]);
	}
	m_vecFH.clear();

	for (int i = 0; i <= SiKGCMap()->GetBlockWidth(); ++i)
	{
		float fGuidePos = static_cast<float>(static_cast<int>(FOOTHOLDSIZE * i));
		KGridGuide* pGuide = new KGridGuide();
		pGuide->SetGridGuidePos(D3DXVECTOR2(fGuidePos, 0));
		pGuide->SetDefault(true);
		pGuide->SetVertical(true);

		m_vecFH.push_back(pGuide);
	}

	for (int i = 0; i <= SiKGCMap()->GetBlockHeight(); ++i)
	{
		float fGuidePos = static_cast<float>(static_cast<int>(FOOTHOLDSIZE * i));
		KGridGuide* pGuide = new KGridGuide();
		pGuide->SetGridGuidePos(D3DXVECTOR2(0, fGuidePos));
		pGuide->SetDefault(true);
		pGuide->SetVertical(false);

		m_vecFH.push_back(pGuide);
	}
}

void KGridGuideMgr::BuildGridGuide()
{
	for (int i = 0; i < (int)m_vecDefaultGuide.size(); ++i)
	{
		SAFE_DELETE(m_vecDefaultGuide[i]);
	}
	m_vecDefaultGuide.clear();

	for (int i = 0; i <= SiKGCMap()->GetPixelWidth(); )
	{
		KGridGuide* pGuide = new KGridGuide();
		pGuide->SetGridGuidePos(D3DXVECTOR2((float)i, 0));
		pGuide->SetDefault(true);
		pGuide->SetVertical(true);
		i += static_cast<int>(m_fGuideTerm);

		m_vecDefaultGuide.push_back(pGuide);
	}

	for (int i = 0; i <= SiKGCMap()->GetPixelHeight(); )
	{
		KGridGuide* pGuide = new KGridGuide();
		pGuide->SetGridGuidePos(D3DXVECTOR2(0, (float)i));
		pGuide->SetDefault(true);
		pGuide->SetVertical(false);
		i += static_cast<int>(m_fGuideTerm);

		m_vecDefaultGuide.push_back(pGuide);
	}
}

void KGridGuideMgr::AddGridGuide(D3DVECTOR MPos, bool bVertical)
{
	KGridGuide* pGuide = new KGridGuide();
	pGuide->SetGridGuidePos(D3DXVECTOR2(MPos.x, MPos.y));
	pGuide->SetDefault(false);
	pGuide->SetVertical(bVertical);

	m_vecUserGuide.push_back(pGuide);
}

bool KGridGuideMgr::AddGridGuide(KGridGuide* pGuide)
{
	if (std::find(m_vecUserGuide.begin(), m_vecUserGuide.end(), pGuide) == m_vecUserGuide.end())
	{
		m_vecUserGuide.push_back(pGuide);
		return true;
	}
	return false;
}

KGridGuide* KGridGuideMgr::GetMouseOverGuide(const D3DVECTOR& MPos)
{
	std::vector< KGridGuide* >::iterator vit;
	for (vit = m_vecUserGuide.begin(); vit != m_vecUserGuide.end(); ++vit)
	{
		KGridGuide* pGuide = (*vit);
		if (pGuide->IsMouseOver(MPos) == true)
		{
			return pGuide;
		}
	}
	return NULL;
}