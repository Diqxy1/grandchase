#include "stdafx.h"
#include "GCLotteryRewardBox.h"

IMPLEMENT_CLASSNAME( KGCLotteryRewardBox );
IMPLEMENT_WND_FACTORY( KGCLotteryRewardBox );
IMPLEMENT_WND_FACTORY_NAME( KGCLotteryRewardBox, "gc_lottery_reward_box" );

KGCLotteryRewardBox::KGCLotteryRewardBox( void )
{
	for (int i=0 ; i<NUM_GRADE ; i++)
	{
		m_pkGrade[i] = NULL;

		char temp[MAX_PATH];
		sprintf( temp, "grade%d", i );
		LINK_CONTROL( temp, m_pkGrade[i] );
	}

	m_pkImgRect = NULL;
	LINK_CONTROL( "img_rect", m_pkImgRect );

	m_iGrade = -1;
	m_iLevel = 0;
	m_gcItemID = 0;

	m_pTexItem = NULL;
}

KGCLotteryRewardBox::~KGCLotteryRewardBox( void )
{
	SAFE_RELEASE(m_pTexItem);
}

void KGCLotteryRewardBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCLotteryRewardBox::OnCreate( void )
{
	for (int i=0 ; i<NUM_GRADE ; i++)
		m_pkGrade[i]->InitState(false);
}

void KGCLotteryRewardBox::PostChildDraw()
{
	if (m_pTexItem == NULL)
		return;

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	D3DXVECTOR2 vPos = GetRelocatedWindowPos();
	vPos.x += m_pkImgRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
	vPos.y += m_pkImgRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;

	g_pGCDeviceManager2->DrawInScreen( m_pTexItem, vPos.x, vPos.y, 
		vPos.x + m_pkImgRect->GetWidth() * m_fWindowScaleX , vPos.y + m_pkImgRect->GetHeight() * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );

	g_pGCDeviceManager2->PopState();


	if (m_iGrade > -1 && m_iGrade < NUM_GRADE)
	{
		m_pkGrade[m_iGrade]->ToggleRender(true);
		m_pkGrade[m_iGrade]->Render();
		m_pkGrade[m_iGrade]->ToggleRender(false);
	}
}

void KGCLotteryRewardBox::SetGoods( GCITEMID gcItemID_, int iLevel_, int iGrade_  )
{
	m_iGrade = iGrade_;
	m_iLevel = iLevel_;
	m_gcItemID = gcItemID_/10;
	SAFE_RELEASE(m_pTexItem);
	m_pTexItem = g_pItemMgr->CreateShopItemTexture( gcItemID_ / 10 );

}

void KGCLotteryRewardBox::SetNothing()
{
	m_iGrade = -1;
	SAFE_RELEASE(m_pTexItem);
}
