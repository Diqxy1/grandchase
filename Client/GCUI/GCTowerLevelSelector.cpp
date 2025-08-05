#include "stdafx.h"
#include "GCTowerLevelSelector.h"

IMPLEMENT_CLASSNAME( KGCTowerLevelSelector );
IMPLEMENT_WND_FACTORY( KGCTowerLevelSelector );
IMPLEMENT_WND_FACTORY_NAME( KGCTowerLevelSelector, "gc_towerlevel" );

KGCTowerLevelSelector::KGCTowerLevelSelector( void )
: m_pkLevelBackground(NULL)
, m_pkLeftBtn(NULL)
, m_pkRightBtn(NULL)
, m_pkLevelNum0(NULL)
, m_pkLevelNum00(NULL)
{
	LINK_CONTROL("level_background",m_pkLevelBackground);	
	LINK_CONTROL("left_btn",m_pkLeftBtn);	
	LINK_CONTROL("right_btn",m_pkRightBtn);	
	LINK_CONTROL("level_0x",m_pkLevelNum0);	
	LINK_CONTROL("level_x0",m_pkLevelNum00);	
}

KGCTowerLevelSelector::~KGCTowerLevelSelector( void )
{
}

void KGCTowerLevelSelector::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCTowerLevelSelector::OnCreate( void )
{
	m_pkLevelBackground->InitState(true);

	m_pkLeftBtn->InitState(true,true,this);
	m_pkLeftBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCTowerLevelSelector::OnLeftButton);
	m_pkRightBtn->InitState(true,true,this);
	m_pkRightBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCTowerLevelSelector::OnRightButton);
	
	m_pkLevelNum0->InitState(true);
	m_pkLevelNum00->InitState(false);
}

void KGCTowerLevelSelector::OnLeftButton()
{
	ChangeSubGameMode( false );
}

void KGCTowerLevelSelector::OnRightButton()
{
	ChangeSubGameMode( true );
}

void KGCTowerLevelSelector::ChangeSubGameMode( bool bRight )
{
	if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
	{
		if ( 0 == g_pkUIHelper->GetUILockTime( LOCK_ROOM ) )
		{
			int iDungeonLevel = SiKGCRoomManager()->GetDungeonLevel();
			g_pkUIScene->m_pkRoomSceneS6->ChangeCurrentLevel( iDungeonLevel, bRight );
			SetDungeonLevel();
		}
	}
	else
	{
		SiKGCRoomManager()->ChangeSubGameModeTower( bRight );
		SetDungeonLevel();
	}
}

void KGCTowerLevelSelector::SetDungeonLevel()
{
	int iDungeonLevel = SiKGCRoomManager()->GetDungeonLevel();

	SetImageLevel(iDungeonLevel+1);
}

void KGCTowerLevelSelector::SetImageLevel(int iLevel)
{
	int iNum = iLevel / 10;
	if ( iNum == 0 ) {
		m_pkLevelNum00->ToggleRender(false);
	}
	else{
		m_pkLevelNum00->ToggleRender(true);
		SetNumberTex(iNum , m_pkLevelNum00);
	}
	iNum = iLevel % 10;
	SetNumberTex(iNum , m_pkLevelNum0);
}

void KGCTowerLevelSelector::SetNumberTex( int index, KD3DWnd *pWnd )
{
	if(pWnd == NULL) return;

	int x_index = index % 10 ; 

	float tu = static_cast<float>(x_index) * 24.f + 264.f;
	float tv = 456.f;
	float tu1 = static_cast<float>(x_index) * 24.f + 276.f ;
	float tv1 = 468.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCTowerLevelSelector::ChangeUVtoTexel( D3DXVECTOR4& vUV )
{
	float fMagicNumber=0.5f;
	if (vUV.x>vUV.z)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
	vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (vUV.y>vUV.w)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
	vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

	TEXEL_BOUND(vUV.x);
	TEXEL_BOUND(vUV.y);
	TEXEL_BOUND(vUV.z);
	TEXEL_BOUND(vUV.w);
}