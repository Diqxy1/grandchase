#include "stdafx.h"
#include "GCRoomDungeonBtnS6.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCRoomDungeonBtnS6 );
IMPLEMENT_WND_FACTORY( KGCRoomDungeonBtnS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomDungeonBtnS6, "gc_room_dungeon_btn_s6" );

KGCRoomDungeonBtnS6::KGCRoomDungeonBtnS6( void )
: m_iDungeonID(-1)
, m_pBossTexture(NULL)
, m_vBaseTexStartAxis(1.f,1.f)
, m_vTexSize(206.f, 38.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(211.f, 40.f)
, m_vThirdTexStartAxis(1.f,238.f)
, m_iIndex(0)
, m_pkHoverBox(NULL)
{
	LINK_CONTROL("dungeon_sel_box",m_pkHoverBox);
}

KGCRoomDungeonBtnS6::~KGCRoomDungeonBtnS6( void )
{
}

void KGCRoomDungeonBtnS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCRoomDungeonBtnS6::FrameMoveInEnabledState( void )
{
	if( IsLocked() ) {
		return;
	}
	if( IsMouseInWindow()  ) {
		SetTexPos(true);
	}
	else {
		SetTexPos(false);
	}
}

void KGCRoomDungeonBtnS6::OnCreate( void )
{
	m_pkHoverBox->InitState(false,false,NULL);
}

void KGCRoomDungeonBtnS6::SetDungeonID( int iID )
{
	m_iDungeonID = iID;
	m_pkHoverBox->ToggleRender(false);
	SetBossImage();
}

void KGCRoomDungeonBtnS6::OnClick( )
{
	SiKGCRoomListManager()->SetCurrentLevel(KGCRoomListManager::KRLM_LEVEL_1);
	SiKGCWorldMapManager()->SetCurrentSelectGameMode(m_iDungeonID);
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DUNGEON_SELECT ) );

}

void KGCRoomDungeonBtnS6::SetTexPos(bool bInMouse )
{
	if(bInMouse == m_bChangeState) return;

	m_bChangeState = bInMouse;

	//보스 텍스쳐 위치 정하기
	m_vImageTexStartAxis.x = m_vBaseTexStartAxis.x / m_vTextureFullSize.x;

	m_vImageTexStartAxis.y = ((bInMouse ? m_vThirdTexStartAxis.y : m_vBaseTexStartAxis.y) + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(m_iIndex) ) ) / m_vTextureFullSize.y;

	m_vImageTexEndtAxis.x = (m_vBaseTexStartAxis.x + m_vTexSize.x ) / m_vTextureFullSize.x;
	m_vImageTexEndtAxis.y = ((bInMouse ? m_vThirdTexStartAxis.y : m_vBaseTexStartAxis.y) + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(m_iIndex) ) ) / m_vTextureFullSize.y;

	m_pkHoverBox->ToggleRender(bInMouse);

};


void KGCRoomDungeonBtnS6::SetBossImage( )
{
	int iFileNum = 0;
	int iPictureNum = 0;
	bool bLock = false;

	if( SiKGCRoomListManager()->GetBossSmallImageInfo(iFileNum, iPictureNum,bLock, m_iDungeonID) == false )
	{
		m_iIndex = 0;
		SAFE_RELEASE( m_pBossTexture );
		return;
	}
	
	m_iIndex = iPictureNum;
	m_bChangeState = false;

	Lock(bLock);

	char cBuf[256];
	memset(cBuf,'\0',256);

	sprintf(cBuf,"selectboss_%d.dds",iFileNum);

	SAFE_RELEASE( m_pBossTexture );

	m_pBossTexture = g_pGCDeviceManager2->CreateTexture(cBuf);


	//보스 텍스쳐 위치 정하기
	m_vImageTexStartAxis.x = ( bLock ? m_vSecondTexStartAxis.x : m_vBaseTexStartAxis.x ) / m_vTextureFullSize.x;
	m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(iPictureNum) ) ) / m_vTextureFullSize.y;

	m_vImageTexEndtAxis.x = (bLock ? m_vSecondTexStartAxis.x + m_vTexSize.x : m_vBaseTexStartAxis.x + m_vTexSize.x ) / m_vTextureFullSize.x;
	m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(iPictureNum) ) ) / m_vTextureFullSize.y;

}

void KGCRoomDungeonBtnS6::PostDraw()
{
	if(m_pBossTexture == NULL) return;

	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

	//UI표시할 위치 정하기
	m_vImagePos.x = vPos.x;
	m_vImagePos.y = vPos.y;

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->DrawInScreen(m_pBossTexture,
		m_vImagePos.x, 
		m_vImagePos.y,
		m_vImagePos.x + m_vTexSize.x * m_fWindowScaleX, 
		m_vImagePos.y + m_vTexSize.y * m_fWindowScaleY,
		m_vImageTexStartAxis.x,
		m_vImageTexStartAxis.y, 
		m_vImageTexEndtAxis.x,
		m_vImageTexEndtAxis.y,
		false,10.0f,D3DCOLOR_ARGB(255,255,255,255), false);
	g_pGCDeviceManager2->PopState();
}

