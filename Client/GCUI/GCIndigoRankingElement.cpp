#include "stdafx.h"
#include ".\gcindigorankingelement.h"
//
//
#include "GCCharacterUI.h"

IMPLEMENT_CLASSNAME( KGCIndigoRankingElement );
IMPLEMENT_WND_FACTORY( KGCIndigoRankingElement );
IMPLEMENT_WND_FACTORY_NAME( KGCIndigoRankingElement, "gc_indigo_element" );

KGCIndigoRankingElement::KGCIndigoRankingElement(void)
:m_pkRank(NULL)
,m_pkNickName(NULL)
,m_pkIndigoData(NULL)
,m_pkCharacter(NULL)
{
	LINK_CONTROL( "indigo_rank", m_pkRank );
	LINK_CONTROL( "indigo_nickname", m_pkNickName );
	LINK_CONTROL( "indigo_data", m_pkIndigoData );
	LINK_CONTROL( "indigo_char", m_pkCharacter );
}

KGCIndigoRankingElement::~KGCIndigoRankingElement(void)
{
}

void KGCIndigoRankingElement::OnCreate( void )
{
	m_pkRank->InitState( true );
	m_pkNickName->InitState( true );
	m_pkIndigoData->InitState( true );
	m_pkCharacter->InitState( true, true, NULL );
}

void KGCIndigoRankingElement::SetElementData( int iRank, const std::wstring& strNickName, int iCharType, const std::wstring& strData )
{
	std::wstringstream strRank;
	strRank<<iRank;

	m_pkRank->SetText( strRank.str() );
	m_pkNickName->SetText( strNickName );
    m_pkIndigoData->SetText( strData );
	m_pkCharacter->ToggleRender( true );
	m_pkCharacter->SetChar( iCharType );
}

void KGCIndigoRankingElement::SetElementData( const KIndigoRankInfo& kRankInfo )
{
	WCHAR str[MAX_PATH];
	swprintf( str, MAX_PATH, L"%s%0.0f%%", g_pkStrLoader->GetString( STR_ID_WINRATE ).c_str(), kRankInfo.m_fWinRate );
	std::wstring strData(str);
	SetElementData( kRankInfo.m_nRank, kRankInfo.m_strNick, kRankInfo.m_cCharType, strData );
}
void KGCIndigoRankingElement::SetEmpty()
{
	m_pkRank->SetText();
	m_pkNickName->SetText();
	m_pkIndigoData->SetText();
	m_pkCharacter->ToggleRender(false);
}