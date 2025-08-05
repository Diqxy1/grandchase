#include "StdAfx.h"
#include "KGCGuildListInfo.h"

IMPLEMENT_CLASSNAME( KGCGuildListInfo );
IMPLEMENT_WND_FACTORY( KGCGuildListInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildListInfo, "gc_guild_list_info" );

KGCGuildListInfo::KGCGuildListInfo(void)
:m_pkGuildMarkIcon(NULL)
,m_pkGuildGradeNew(NULL)
,m_pkGuildGradeRegular(NULL)
,m_pkStaticGuildName(NULL)
,m_pkStaticGuildIntroduce(NULL)
,m_pkStaticGuildBirthday(NULL)
,m_pkListSelectBG(NULL)
{
    LINK_CONTROL("tex_guild_mark",			m_pkGuildMarkIcon );
    LINK_CONTROL("guild_grade_new",			m_pkGuildGradeNew );
    LINK_CONTROL("guild_grade_regular",		m_pkGuildGradeRegular );

    LINK_CONTROL("static_guild_name",		m_pkStaticGuildName );
    LINK_CONTROL("static_guild_introduce",	m_pkStaticGuildIntroduce );
	LINK_CONTROL("static_guild_birthday",	m_pkStaticGuildBirthday );	

	LINK_CONTROL("bg_list_select",			m_pkListSelectBG );	
}

KGCGuildListInfo::~KGCGuildListInfo(void)
{
}


void KGCGuildListInfo::OnCreate( void )
{
    m_pkGuildMarkIcon->InitState( true );

    m_pkGuildGradeNew->InitState( false, true, this);
    m_pkGuildGradeRegular->InitState( false, true, this);
	m_pkGuildGradeNew->SetToolTip(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_NEW));
	m_pkGuildGradeRegular->SetToolTip(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_REGULAR));
	

    m_pkStaticGuildName->InitState( true );
	m_pkStaticGuildName->SetShadow(true);
    m_pkStaticGuildName->SetWordWrap( true );
	m_pkStaticGuildName->SetShadowColor(0xff623e00);

    m_pkStaticGuildIntroduce->InitState( true );	
	m_pkStaticGuildIntroduce->SetShadow(true);
	m_pkStaticGuildIntroduce->SetShadowColor(0xff623e00);

	m_pkStaticGuildBirthday->InitState( true );
	m_pkStaticGuildBirthday->SetShadow(true);
	m_pkStaticGuildName->SetShadowColor(0xff623e00);

	m_pkListSelectBG->InitState(false);
}

void KGCGuildListInfo::OnCreateComplete( void )
{
}

void KGCGuildListInfo::ActionPerformed( const KActionEvent& event )
{

}

void KGCGuildListInfo::OnDestroyComplete( void )
{	
}

void KGCGuildListInfo::FrameMoveInEnabledState( void )
{	
}

void KGCGuildListInfo::PostChildDraw( void )
{
}

void KGCGuildListInfo::SetGuildGrade( EGuildGrade eGuildGrade_ )
{
	switch(eGuildGrade_)
	{
	case REGULAR_GUILD:
		m_pkGuildGradeNew->ToggleRender(false);
		m_pkGuildGradeRegular->ToggleRender( true );
		break;
	case NEW_GUILD:
	default:
		m_pkGuildGradeNew->ToggleRender(true);
		m_pkGuildGradeRegular->ToggleRender( false );
	}
}

void KGCGuildListInfo::SetGuildInfoData( KNGuildInfo& sNGuildInfo, bool bToolTip )
{
    if ( bToolTip )
    {
        m_pkStaticGuildName->SetForceWordWrapText( sNGuildInfo.m_strName.c_str(), true );
        m_pkStaticGuildName->SetToolTip( sNGuildInfo.m_strName );

	    CString src = sNGuildInfo.m_strComment.c_str();
	    src.Replace(L"\n", L" ");
	    std::wstring strConvertor(src);
	    m_pkStaticGuildIntroduce->SetForceWordWrapText( strConvertor, true);
	    SetGuildGrade((EGuildGrade)sNGuildInfo.m_ucGrade);
	    WCHAR strText[20];
	    swprintf(strText, 20, L"%d.%d.%d", (int)sNGuildInfo.m_kFoundingDate.m_sYear, sNGuildInfo.m_kFoundingDate.m_cMonth, sNGuildInfo.m_kFoundingDate.m_cDay);	
	    m_pkStaticGuildBirthday->SetText(strText);

	    //길드마크
	    SiKGCIconImageManager()->LoadIconImageFromHTTP(sNGuildInfo.m_strFileName);	
	    m_pkGuildMarkIcon->SetTextureName( sNGuildInfo.m_strFileName );
    }
    else
    {
        m_pkStaticGuildName->ClearToolTip();
    }
}

void KGCGuildListInfo::SetRenderListSelectBG( bool bRender_ )
{
	m_pkListSelectBG->ToggleRender(bRender_);
}

bool KGCGuildListInfo::IsRenderONListSelectBG()
{
	return m_pkListSelectBG->IsRenderOn();
}

void KGCGuildListInfo::InitGuildInfo()
{
	m_pkStaticGuildName->SetText(L"");
	m_pkStaticGuildIntroduce->SetText(L"");
	m_pkStaticGuildBirthday->SetText(L"");
	m_pkGuildGradeNew->ToggleRender(false);
	m_pkGuildGradeRegular->ToggleRender(false);
}

void KGCGuildListInfo::RenderGuildMark( bool bRender_ )
{
	m_pkGuildMarkIcon->ToggleRender(bRender_);
}