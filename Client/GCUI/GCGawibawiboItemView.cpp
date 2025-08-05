#include "stdafx.h"
#include "GCGawibawiboItemView.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboItemView );
IMPLEMENT_WND_FACTORY( KGCGawibawiboItemView );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboItemView, "gc_gawibawibo_game_view_item" );

#define	INCREASE_VALUE				(70.0f)
#define FINAL_BOSS					(10)

KGCGawibawiboItemView::KGCGawibawiboItemView( void )
:	m_pkBossFrame(NULL)
,	m_pkTitle(NULL)
,	m_pkBossBoxBK(NULL)
,	m_pkStringBox(NULL)
,	m_pkItemBoxBK(NULL)
,	m_pkItemFrame(NULL)
,	m_staticDescriptoin1(NULL)
,	m_staticDescriptoin2(NULL)
,	m_pkBtnOK(NULL)
,	m_iSelectBoss(1)
,	m_iDefaultRewardItem(NULL)
{
	LINK_CONTROL("bk_boss_frame",m_pkBossFrame);
	LINK_CONTROL("title",m_pkTitle);
	LINK_CONTROL("bk_boss_box",m_pkBossBoxBK);
	LINK_CONTROL("bk_item_frame",m_pkItemFrame);
	LINK_CONTROL("bk_description_box",m_pkStringBox);
	LINK_CONTROL("itembk_box",m_pkItemBoxBK);
	LINK_CONTROL("static_description1",m_staticDescriptoin1);
	LINK_CONTROL("static_description2",m_staticDescriptoin2);
	LINK_CONTROL("btn_ok",m_pkBtnOK);

	std::string	strUIName;
	char charUINUM[10];
	memset(charUINUM,'\0',10);
	for(int i = 1;i<11;i++)
	{
		memset(charUINUM,'\0',10);
		m_mapBossImage[i] = NULL;
		m_mapBossBox[i] = NULL;
		m_mapItemBox[i] = NULL;

		sprintf(charUINUM,"%d",i);

		strUIName = "bk_boss_box";
		strUIName += charUINUM;
		LINK_CONTROL(strUIName.c_str(),m_mapBossBox[i]);
		if(i < 10){
			strUIName = "boss_image";
			strUIName += charUINUM;
			LINK_CONTROL(strUIName.c_str(),m_mapBossImage[i]);
		}
		strUIName = "bk_item_box";
		strUIName += charUINUM;
		LINK_CONTROL(strUIName.c_str(),m_mapItemBox[i]);
	}

	LINK_CONTROL( "boss_imagefinal",   m_mapBossImage[FINAL_BOSS] );	


}

KGCGawibawiboItemView::~KGCGawibawiboItemView( void )
{
}

void KGCGawibawiboItemView::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		std::map<int,KD3DWnd*>::iterator mIter = m_mapBossImage.begin();
		for(;mIter != m_mapBossImage.end();mIter++)
		{
			if( event.m_pWnd == mIter->second)
			{
				m_iSelectBoss = mIter->first;
				SetRewardSetting();
				SetBtnLock();
			}

		}
		if(event.m_pWnd == m_pkBtnOK)
		{
			ClearItemImage();
			ToggleRender(false);
		}
	}
}

void KGCGawibawiboItemView::OnCreate( void )
{
	m_pkBossFrame->InitState(true);
	m_pkTitle->InitState(true);
	m_pkBossBoxBK->InitState(true);
	m_pkStringBox->InitState(true);
	m_pkItemBoxBK->InitState(true);
	m_pkItemFrame->InitState(true);
	m_staticDescriptoin1->InitState(true);
	m_staticDescriptoin2->InitState(true);
	m_pkStringBox->InitState(true);
	m_pkBtnOK->InitState(true,true,this);
	
	m_staticDescriptoin1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_ITEM_VIEW1));
	m_staticDescriptoin2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_ITEM_VIEW2));
	
	for(int i = 1;i<11;i++)
	{
		m_mapBossImage[i]->InitState(false,true,this);
		m_mapBossBox[i]->InitState(false);
		m_mapItemBox[i]->InitState(true);
	}
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_NOTOPMOST);

}


void KGCGawibawiboItemView::SetBossSetting()
{

	int iheight = (m_iMaxBossCnt-1)/5;

	m_pkBossFrame->SetHeight( m_pkBossFrame->GetHeight() +static_cast<int>(INCREASE_VALUE) * iheight);
	m_pkBossBoxBK->SetHeight( m_pkBossBoxBK->GetHeight() +static_cast<int>(INCREASE_VALUE) * iheight);
		
	D3DXVECTOR2 pos(0.0f,0.0f);

	for(int i = 1;i<11;i++)
	{
		pos.x = 16.0f + INCREASE_VALUE * static_cast<float>(static_cast<int>((i-1)%5));
		pos.y = 35.0f + INCREASE_VALUE * static_cast<float>(static_cast<int>((i-1)/5));

		m_mapBossBox[i]->SetWindowPos(pos);
		

		pos.x += (m_mapBossBox[i]->GetWidth()/2.0f)-(m_mapBossImage[i]->GetWidth()/2.0f);
		pos.y += (m_mapBossBox[i]->GetHeight()/2.0f)- (m_mapBossImage[i]->GetHeight()/2.0f);
		if(i <10){
			m_mapBossImage[i]->SetWindowPos(pos);
			m_mapBossImage[i]->ToggleRender(i<m_iMaxBossCnt);
		}
		m_mapBossBox[i]->ToggleRender(i<=m_iMaxBossCnt);
		if(i == m_iMaxBossCnt)
		{
			m_mapBossImage[FINAL_BOSS]->SetWindowPos(pos);
		}
	}
	m_mapBossImage[FINAL_BOSS]->ToggleRender(true);
}

void KGCGawibawiboItemView::ClearItemImage()
{
	std::vector<GCDeviceTexture*>::iterator vIter =	m_vecItemImage.begin();

	for(;vIter != m_vecItemImage.end();vIter++)
	{
		SAFE_RELEASE( *vIter );
	}

	m_vecItemImage.clear();
}


void KGCGawibawiboItemView::SetRewardSetting()
{
	int iSelectBoss =m_iSelectBoss -1;
	if(m_iSelectBoss == FINAL_BOSS)
		iSelectBoss = m_iMaxBossCnt-1;
	int iheight = (m_iMaxBossCnt-1)/5;
	float fheight = INCREASE_VALUE * static_cast<float>(iheight);
	D3DXVECTOR2 pos(0.0f,0.0f);
	
	pos.y = fheight + 111.0f; 
	m_pkItemFrame->SetWindowPos(pos);
	m_pkItemFrame->ToggleRender(true);

	pos.x = 6.0f;
	pos.y = fheight + 118.0f;
	m_pkItemBoxBK->SetWindowPos(pos);
	m_pkItemBoxBK->ToggleRender(true);

	pos.y = fheight + 276.0f;
	m_pkStringBox->SetWindowPos(pos);
	m_pkStringBox->ToggleRender(true);

	pos.x = 9.0f;
	pos.y = fheight + 286.0f;
	m_staticDescriptoin1->SetWindowPos(pos);
	m_staticDescriptoin1->ToggleRender(true);
	pos.y = fheight + 304.0f;
	m_staticDescriptoin2->SetWindowPos(pos);
	m_staticDescriptoin2->ToggleRender(true);


	pos.x = 143.0f;
	pos.y = fheight + 330.0f;
	m_pkBtnOK->SetWindowPos(pos);
	m_pkBtnOK->ToggleRender(true);
	for(int i = 1;i<11;i++)
	{
		pos.x = 11.0f + static_cast<float>((i-1)%5) * 72.0f ;
		pos.y = fheight + 124.0f + static_cast<float>(static_cast<int>((i-1)/5)) * 72.0f;

		m_mapItemBox[i]->SetWindowPos(pos);
		m_mapItemBox[i]->ToggleRender(true);
	}		
	ClearItemImage();
	int iItem = m_iClearCnt;
	if(m_mapWinReward.end() == m_mapWinReward.find(std::pair<DWORD,DWORD>(iItem,iSelectBoss)))
	{
		iItem = m_iDefaultRewardItem;
	}
	VEC_REWARD &vItem =	m_mapWinReward[std::pair<DWORD,DWORD>(iItem,iSelectBoss)];

	if(vItem.empty()) return;

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
	//float setoffsetx = 0.0f;
	//float setoffsety = 0.0f;
	//LUA_GET_VALUE_DEF( "setoffsetx", setoffsetx, 0.0f );    
	//LUA_GET_VALUE_DEF( "setoffsety", setoffsety, 0.0f );    

	VEC_REWARD::iterator vIter = vItem.begin();
	for(;vIter != vItem.end() ;++vIter)
	{
		GCDeviceTexture* pTex = g_pItemMgr->CreateItemTexture(vIter->m_ItemID/10);
		if (pTex)
		{
			m_vecItemImage.push_back(pTex);
		}
	}

}

void KGCGawibawiboItemView::SetBtnLock()
{
	std::map<int,KD3DWnd*>::iterator mIter = m_mapBossImage.begin();
	for(;mIter != m_mapBossImage.end();mIter++)
	{
		mIter->second->Lock(m_iSelectBoss == mIter->first );
	}
}

void KGCGawibawiboItemView::SetView()
{
	SetBossSetting();
	SetRewardSetting();
	SetBtnLock();
}


void KGCGawibawiboItemView::PostChildDraw()
{
	std::vector<GCDeviceTexture*>::iterator vIter =	m_vecItemImage.begin();
	std::map<int, KD3DWnd*>::iterator mitBox = m_mapItemBox.begin();
	for(;vIter != m_vecItemImage.end() && mitBox != m_mapItemBox.end() ; ++vIter , ++mitBox)
	{
		D3DXVECTOR2 vPos = GetRelocatedWindowPos();
		vPos.x += mitBox->second->GetFixedWindowLocalPos().x * m_fWindowScaleX;
		vPos.y += mitBox->second->GetFixedWindowLocalPos().y * m_fWindowScaleY;

		g_pGCDeviceManager2->DrawInScreen(*vIter,
			vPos.x , vPos.y, 
			vPos.x + mitBox->second->GetWidth() * m_fWindowScaleX, vPos.y + mitBox->second->GetHeight() * m_fWindowScaleY,
			0.0f, 0.0f, 1.0f, 1.0f ,false,0.1f,0xffffffff);

	}
	DrawItemInfo();

}

void KGCGawibawiboItemView::DrawItemInfo()
{
	int iSelectBoss =m_iSelectBoss -1;
	if(m_iSelectBoss == FINAL_BOSS)
		iSelectBoss = m_iMaxBossCnt-1;
	int iItem = m_iClearCnt;
	if(m_mapWinReward.end() == m_mapWinReward.find(std::pair<DWORD,DWORD>(iItem,iSelectBoss)))
	{
		iItem = m_iDefaultRewardItem;
	}
	POINT pt = g_pkInput->GetMousePos();
	GCItem* pItem = NULL;
	VEC_REWARD &vItem =	m_mapWinReward[std::pair<DWORD,DWORD>(iItem,iSelectBoss)];

	VEC_REWARD::iterator vIter = vItem.begin();
	D3DXVECTOR2	pos;
	for(int i = 1;vIter != vItem.end();vIter++, i++)
	{
		pos = m_mapItemBox[i]->GetRelocatedWindowPos();
		if(pos.x <pt.x && pos.x +70.f >pt.x && pos.y < pt.y && pos.y+70.f>pt.y)
		{
			pItem = g_pItemMgr->GetItemData(vIter->m_ItemID/10);
			pos.x += 70.f;
            break;
		}
	}


	if( pItem != NULL )
	{
		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( pos );
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
	}
	else
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}
