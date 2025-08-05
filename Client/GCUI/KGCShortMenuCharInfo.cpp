#include "stdafx.h"
#include "KGCShortMenuCharInfo.h"
#include "GCTextColorManager.h"

IMPLEMENT_CLASSNAME( KGCShortMenuCharInfo );
IMPLEMENT_WND_FACTORY( KGCShortMenuCharInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCShortMenuCharInfo, "gc_shortmenu_charinfo" );

KGCShortMenuCharInfo::KGCShortMenuCharInfo(void)
: m_pkCharLevel(NULL)
, m_pkNickname(NULL)
, m_pkTotalAtk(NULL)
, m_pkTotalAtkVal(NULL)
, m_pkMatchRank(NULL)
, m_pkWin(NULL)
, m_pkNoneRank(NULL)
, m_pkMatchRankMark(NULL)
{

	LINK_CONTROL("level_static", m_pkCharLevel);
	LINK_CONTROL("nick_static", m_pkNickname);
	LINK_CONTROL("totalatk_static", m_pkTotalAtk);
	LINK_CONTROL("totalatk_value_static", m_pkTotalAtkVal);
	LINK_CONTROL("matchrank_static", m_pkMatchRank);
	LINK_CONTROL("win_static", m_pkWin);
	LINK_CONTROL("none_rank_static", m_pkNoneRank);
	LINK_CONTROL("matchrank_mark", m_pkMatchRankMark);

//	for(int i = 0; i < MAX_NUM_GRADE; i++ )
//	{
//		m_saCharGrade[i] = NULL;
//		LINK_CONTROL_STM( "grade" << i, m_saCharGrade[i] );
//	}
	
}

KGCShortMenuCharInfo::~KGCShortMenuCharInfo(void)
{
}

void KGCShortMenuCharInfo::OnCreate()
{
	if( m_pkCharLevel )
	{
		m_pkCharLevel->InitState( true, true );
	}

	if( m_pkNickname )
	{
		m_pkNickname->InitState( true, true );
	}

	if( m_pkTotalAtk )
	{
		m_pkTotalAtk->InitState( true, true );
		m_pkTotalAtk->SetTextFromID( STR_ID_MAX_ATTACK );
		m_pkTotalAtk->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkTotalAtkVal )
	{
		m_pkTotalAtkVal->InitState( true, true );
		m_pkTotalAtkVal->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkMatchRank )
	{
		m_pkMatchRank->InitState( true, true );
		m_pkMatchRank->SetTextFromID( STR_ID_PVP_MATCH_POINT_RANK );
		m_pkMatchRank->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkWin )
	{
		m_pkWin->InitState( false, true );
		m_pkWin->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkNoneRank )
	{
		m_pkNoneRank->InitState( false, true );
		m_pkNoneRank->SetTextFromID( STR_ID_SKILLTREE14 );
		m_pkNoneRank->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkMatchRankMark )
	{
		m_pkMatchRankMark->InitState( false );
	}

}

void KGCShortMenuCharInfo::PostChildDraw()
{
	PLAYER* pPlayer = &g_MyD3D->m_TempPlayer;

	EAbilityData stAbilityData;
	pPlayer->GetCharAbility( stAbilityData );

	m_pkTotalAtkVal->SetNumber(static_cast<int>(pPlayer->GetCharTotalAtk( stAbilityData )));

	m_pkNickname->SetText( pPlayer->m_kUserInfo.strNickName );
	m_pkNickname->SetFontColor(SiGCTextColorManager()->StringToColor(pPlayer->m_kUserInfo.strNickColor));
	m_pkNickname->SetShadow( true );
	m_pkNickname->SetShadowColor( 0xFF000000 );

	std::wstringstream charLevel;
	charLevel << g_pkStrLoader->GetString( STR_ID_CHARACTER_LV );
	charLevel << pPlayer->GetCurrentChar().iLevel;

	m_pkCharLevel->SetText( charLevel.str() );

	if( pPlayer->GetCurrentChar().kELOUserData.m_nELOType == KELOUserData::MT_PLACEMENTTEST )
	{
		m_pkMatchRankMark->ToggleRender( false );
		m_pkNoneRank->ToggleRender( true );
		m_pkWin->ToggleRender( false );
	}
	else
	{
		m_pkWin->SetNumber( pPlayer->GetCurrentChar().iWin );
		m_pkWin->ToggleRender( true );
		m_pkMatchRankMark->ToggleRender( true );
		SetGradeTex(pPlayer->GetCurrentChar().kELOUserData.m_ucGrade - 1,  m_pkMatchRankMark);
	}
}

void KGCShortMenuCharInfo::SetGradeTex(int index, KD3DWnd* pWnd)
{
	if( pWnd == NULL ) return;

	if ( index > (GC_BG_GRADE_MAX - 1) || index < 0 ) 
	{
		pWnd->ToggleRender( false );
		return;
	}

	int x_index = index / GC_BG_GRADE_MAX; 
	int y_index = index % GC_BG_GRADE_MAX; 

	float tu = static_cast<float>(x_index) * 20.f + 345.f;
	float tv = static_cast<float>(y_index) * 21.f + 2.f;
	float tu1 = static_cast<float>(x_index) * 20.f + 365.f ;
	float tv1 = static_cast<float>(y_index) * 21.f + 21.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCShortMenuCharInfo::ChangeUVtoTexel( D3DXVECTOR4& vUV )
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