#include "stdafx.h"
#include "GCIntroNickNameFrame.h"

IMPLEMENT_CLASSNAME( KGCIntroNickNameFrame );
IMPLEMENT_WND_FACTORY( KGCIntroNickNameFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCIntroNickNameFrame, "gc_intro_nickname_frame" );

KGCIntroNickNameFrame::KGCIntroNickNameFrame( void )
: m_bClickedNicknameEdit(false)
{
	m_pkPositive = NULL;
	LINK_CONTROL( "positive", m_pkPositive );

	m_pkSpacialChar = NULL;
	LINK_CONTROL( "spacialchar", m_pkSpacialChar );

	m_pkCussWord = NULL;
	LINK_CONTROL( "cussword", m_pkCussWord );

	m_pkEditNickname = NULL;
	LINK_CONTROL( "edit_nickname", m_pkEditNickname );

    m_bIsValidNickName = false;

    m_bPositive = false;
}

KGCIntroNickNameFrame::~KGCIntroNickNameFrame( void )
{
}

void KGCIntroNickNameFrame::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkEditNickname,                KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditNick );

}

void KGCIntroNickNameFrame::OnCreate( void )
{
	m_pkPositive->InitState(false);
	m_pkSpacialChar->InitState(false);
	m_pkCussWord->InitState(false);

	m_pkEditNickname->InitState(true,true,this);
	m_pkEditNickname->SetText(g_pkStrLoader->GetString( STR_ID_ENTER_YOUR_NICKNAME ).c_str());

    m_pkEditNickname->SetText(L"");

#ifdef NATION_KOREA
	m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE/2 );
#else
	m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE );
#endif 
}

void KGCIntroNickNameFrame::FrameMoveInEnabledState()
{
	// 키를 누르는 소리
	if( m_pkEditNickname->HasFocus())
	{
		for ( int i = 0; i < 256; i++ )
		{
			if ( g_pkInput->IsDown( i ) ){
				g_KDSound.Play( "71" );
				KeydownEditNick();
			}
		}
	}

}
void KGCIntroNickNameFrame::LoadDefaultValue()
{
    m_pkEditNickname->SetText(g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ).c_str()); 
    m_pkEditNickname->SetFocus( false );
    m_bClickedNicknameEdit = false;
}

void KGCIntroNickNameFrame::KeydownEditNick()
{
	std::wstring strCheckNewNickname = m_pkEditNickname->GetText();  //글자를 얻는다.  

	m_pkPositive->ToggleRender(false);
	m_pkCussWord->ToggleRender(false);
	m_pkSpacialChar->ToggleRender(false);

	if( strCheckNewNickname.empty() )
	{
		m_bIsValidNickName = false;
        m_bPositive = false;
        return;
	}

	if(!SiKSlangFilter()->FilterCheck( strCheckNewNickname, KSlangFilter::SLANG_FILTER_NICKNAME )) 
	{
		m_pkCussWord->ToggleRender(true);
        m_bIsValidNickName = false;
        m_bPositive = false;
		return;
	}
	if(!SiKSlangFilter()->ValidCheckNickName( strCheckNewNickname)) 
	{
		m_pkSpacialChar->ToggleRender(true);
        m_bIsValidNickName = false;
        m_bPositive = false;
		return;
	}

	m_pkPositive->ToggleRender(true);
    m_bIsValidNickName = true;
    m_bPositive = true;
}

void KGCIntroNickNameFrame::OnClickEditNick()
{
    m_bClickedNicknameEdit = true;
	m_pkEditNickname->SetFocus();

}

bool KGCIntroNickNameFrame::IsClickNickNameEdit()
{
    return m_bClickedNicknameEdit;
}