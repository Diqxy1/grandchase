#include "stdafx.h"
#include ".\gcrankingelement.h"



//
#include "MyD3D.h"
#include "GCCharacterUI.h"
#include "GCSizingThreeBox.h"
#include "GCIconImageManager.h"

IMPLEMENT_CLASSNAME( KGCRankingElement );
IMPLEMENT_WND_FACTORY( KGCRankingElement );
IMPLEMENT_WND_FACTORY_NAME( KGCRankingElement, "gc_ranking_element" );

KGCRankingElement::KGCRankingElement(void)
: m_pkRank(NULL)
, m_pkNick(NULL)
, m_pkWin(NULL)
, m_pkLose(NULL)
, m_pkWinRate(NULL)
, m_pkGuage( NULL )
, m_pkGuageFrame( NULL )
, m_iGuageGap( -1 )
, m_pkNoGuildMark( NULL )
{
	for( int i = 0 ; i < GUILD_MARK_COLOR_NUM ; ++i )
	{
		m_aMark[i] = -1;
	}

	LINK_CONTROL( "rank",				m_pkRank );
	LINK_CONTROL( "nickname",			m_pkNick );
	LINK_CONTROL( "win",				m_pkWin );
	LINK_CONTROL( "lose",				m_pkLose );
	LINK_CONTROL( "winrate",			m_pkWinRate );
	LINK_CONTROL( "char",				m_pkChar );
	LINK_CONTROL( "expguage",			m_pkGuage );
	LINK_CONTROL( "expguage_frame",		m_pkGuageFrame );
	LINK_CONTROL( "noguild_mark",		m_pkNoGuildMark );

	LINK_CONTROL( "guild_name",			m_pkGuildName );
	LINK_CONTROL( "g_point",			m_pkGPoint );
	LINK_CONTROL( "guild_mark_icon",	m_pkGuildMark );
}

KGCRankingElement::~KGCRankingElement(void){}


void KGCRankingElement::OnCreate( void )
{
	bool	bUseForGuild	=	true;
	
    m_pkRank->			InitState( true );					//	내 랭킹
	m_pkWin->			InitState( true );					//	내 승리
	m_pkNick->			InitState( !bUseForGuild );			//	내 닉넴
	m_pkLose->			InitState( true );					//	내 패배
	m_pkWinRate->		InitState( true );					//	내 승률

	m_pkChar->			InitState( !bUseForGuild, true );	//	내 아이콘
	m_pkChar->			SetChar( 0 );						//	내 아이콘
	
	m_pkGuage->			InitState( false, true );			//	내 게이지
	m_pkNoGuildMark->	InitState( true );					//	길드 마크

	//길드일때 사용하는 윈도우들
	m_pkGuildName->		InitState( bUseForGuild, true );	//	길드 이름		-> toggle 내 아이콘
	m_pkGPoint->		InitState( bUseForGuild, true );	//	길드 포인트		-> toggle 내 닉넴
	//m_pkGuildMark->		InitState( bUseForGuild, true );
	

	if( m_iGuageGap == -1 )
		m_iGuageGap = (static_cast<int>(m_pkGuageFrame->GetWidth()) - static_cast<int>(m_pkGuage->GetWidth())) - (static_cast<int>(m_pkGuage->GetFixedWindowLocalPos().x) - static_cast<int>(m_pkGuageFrame->GetFixedWindowLocalPos().x));
}


void KGCRankingElement::ActionPerformed( const KActionEvent& event ){}


void KGCRankingElement::ExpRankingMode()
{
	m_pkWin->ToggleRender( false );
	m_pkLose->ToggleRender( false );
	m_pkWinRate->ToggleRender( false );

	m_pkGuage->ToggleRender( true );
	m_pkGuageFrame->ToggleRender( true );

	m_pkGuildName->	ToggleRender( false );
	m_pkGPoint->	ToggleRender( false );
}

void KGCRankingElement::WinrateRankingMode()
{
	m_pkWin->ToggleRender( true );
	m_pkLose->ToggleRender( true );
	m_pkWinRate->ToggleRender( true );

	m_pkGuage->ToggleRender( false );
	m_pkGuageFrame->ToggleRender( false );

	m_pkGuildName->	ToggleRender( false );
	m_pkGPoint->	ToggleRender( false );
}

void KGCRankingElement::SetRankingInfo( const KWinRateRank* ranking )
{
	if( ranking == NULL ) 
	{
		reset();
		return;
	}
	m_pkRank->SetText( ranking->m_nRank );
	std::wstring strNick = ranking->m_strNick;
	GCUTIL_STR::LTrim( strNick );
	GCUTIL_STR::RTrim( strNick );
	m_pkNick->SetText( strNick );
	m_pkChar->ToggleRender( true );
	m_pkChar->SetChar( ranking->m_cCharType );
	m_pkWin->SetText( ranking->m_dwWin );
	m_pkLose->SetText( ranking->m_dwLose );

	std::wstringstream stm;
    stm.precision(2);
    stm<< std::fixed << ranking->m_fWinRate << "%";
	m_pkWinRate->SetText( stm.str() );

	SetGuildMark( ranking->m_strMarkName );
}

void KGCRankingElement::SetRankingInfo( const KExpRank* ranking )
{
	if( ranking == NULL ) 
	{
		reset();
		return;
	}
	m_pkRank->SetText( ranking->m_nRank );
	std::wstring strNick = ranking->m_strNick;
	GCUTIL_STR::LTrim( strNick );
	GCUTIL_STR::RTrim( strNick );
	m_pkNick->SetText( strNick );
	m_pkChar->ToggleRender( true );
	m_pkChar->SetChar( ranking->m_cCharType );
	m_pkGuage->ToggleRender( true );

	SetGuildMark( ranking->m_strMarkName );

	int iFullGuage = m_pkGuageFrame->GetWidth() - m_iGuageGap;
	int iExpGuage = (int)(((float)iFullGuage * 0.5f ) * ( 0.01 * (float)(100 - ranking->m_nRank) ));
	int iRankGuage = (int)(((float)iFullGuage * 0.5f ) * ( (float)Exp_2_Level(ranking->m_biExp) / (float)g_kGlobalValue.m_kRanking.GetBestExpRankerLevel() ));

	m_pkGuage->SetWidth( iExpGuage + iRankGuage );
}

void KGCRankingElement::SetRankingInfo( const KMatchRank* ranking )
{
    if( ranking == NULL ) 
    {
        reset();
        return;
    }
    m_pkRank->SetText( ranking->m_nRank );
    std::wstring strNick = ranking->m_strNick;
    GCUTIL_STR::LTrim( strNick );
    GCUTIL_STR::RTrim( strNick );
    m_pkNick->SetText( strNick );
    m_pkChar->ToggleRender( true );
    m_pkChar->SetChar( ranking->m_cCharType );
    m_pkWin->SetText( ranking->m_dwELOWin );
    m_pkLose->SetText( ranking->m_dwELOLose );

    std::wstringstream stm;
    stm.precision(2);
    stm<< std::fixed << ranking->m_fWinRate << "%";
    m_pkWinRate->SetText( stm.str() );

    SetGuildMark( ranking->m_strMarkName );
}

void KGCRankingElement::reset()
{
	m_pkRank->		SetText();
	m_pkWin->		SetText();
	m_pkNick->		SetText();
	m_pkLose->		SetText();
	m_pkWinRate->	SetText();

	m_pkChar->			ToggleRender( false );
	m_pkGuage->			ToggleRender( false );
	m_pkGuageFrame->	ToggleRender( false );
	m_pkNoGuildMark->	ToggleRender( false );

 	m_pkGuildName->		ToggleRender( false );
 	m_pkGPoint->		ToggleRender( false );
	m_pkGuildMark->		ToggleRender( false );
}

void KGCRankingElement::PostChildDraw( void )
{
    //if ( true == m_pkChar->IsRenderOn() )
    //{        D3DXVECTOR2 vPos( m_pkNoGuildMark->GetFixedWindowPos() );
    //    float x = ( vPos.x / 400.0f ) - 0.966f;
    //    float y = ( -vPos.y / 300.0f * 0.75f ) + 0.7225f;
    //    g_MyD3D->m_KGCGuildMark.Render( m_aMark[0], m_aMark[1], m_aMark[2], x, y, 0.085f );
    //}
}

void KGCRankingElement::SetGuildMark( const std::wstring& strMarkFile )
{
	//	해서 아이콘을 로드.
	SiKGCIconImageManager()->LoadIconImageFromHTTP( strMarkFile );
	std::wstring bufString;
	if (strMarkFile.empty()) {
		bufString = L"defaultmark.dds"; 
    }
    else {
        bufString = strMarkFile;
    }
	m_pkGuildMark->SetTextureName( bufString );
	m_pkGuildMark->InitState( true, true, this );
}

//------------------------------------------------------------------------
//	길드 랭킹을 위한 추가 함수
void KGCRankingElement::ToggleUseForGuild( bool bGuildUse )
{
	m_pkNick->ToggleRender( !bGuildUse );		//	내 닉넴
	m_pkChar->ToggleRender( !bGuildUse );		//	내 아이콘
	//아래 두개는 길드 사용일때만 활성화
	m_pkGuildName->ToggleRender( bGuildUse );	//	길드 이름		-> toggle 내 아이콘
	m_pkGPoint->ToggleRender( bGuildUse );		//	길드 포인트		-> toggle 내 닉넴
}

void KGCRankingElement::SetRankingInfo( const KNGuildRankInfo* ranking, const float fGPointTop )
{
	//	fGPointTop의 값을 주지 않으면 100 퍼센트의 게이지가 만들어진다.
	if( NULL == ranking ){
		reset();
		return;
	}
	
	m_pkRank->SetText( ranking->m_nRank );			//	순위
	//	m_pkNoGuildMark->Set						//	길드 마크
	m_pkGuildName->SetText( ranking->m_strName );	//	길드 이름
	m_pkGPoint->SetText( ranking->m_dwPoint );		// G/B -Point

	m_pkWin->SetText( ranking->m_nWin );			//	승리
	m_pkLose->SetText( ranking->m_nLose );			//	패배

	//	승율
	std::wstringstream stm;				
	stm.precision(2);
	stm<< std::fixed << ranking->m_fRatio << "%";
	m_pkWinRate->SetText( stm.str() );

	//	게이지 정보
	int		iFullGuage = m_pkGuageFrame->GetWidth() - m_iGuageGap;	//	만땅 게이지의 픽셀 길이
	
	float	fGuage		=	static_cast<float>(iFullGuage);			//	만땅 게이지의 픽셀 길이 캐스팅
	float	fTop		=	0.0f;									//	1위 GPoint
	
	fTop	=	( fGPointTop == fTop ) ? static_cast<float>(ranking->m_dwPoint) : fGPointTop;
    if(fTop < 1.0f)
        fTop = 1.0f;
	fGuage	=	fGuage * static_cast<float>(ranking->m_dwPoint) / fTop;	//	그래프의 비율은 ( 자신의 포인트 / 탑 포인트 )
    if(fGuage < 1.0f || fGuage > 160.0f )
        fGuage = 1.0f;

	m_pkGuage->SetWidth( static_cast<int>( fGuage ) );

	SetGuildMark( ranking->m_strMarkName );
}

void KGCRankingElement::TotalRankingMode()
{
	m_pkWin->		ToggleRender( false );
	m_pkLose->		ToggleRender( false );
	m_pkWinRate->	ToggleRender( false );

	m_pkGuage->		ToggleRender( true );
	m_pkGuageFrame->ToggleRender( true );

	m_pkGuildName->	ToggleRender( true );
	m_pkGPoint->	ToggleRender( true );
}
//------------------------------------------------------------------------
