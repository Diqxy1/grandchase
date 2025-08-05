#include "stdafx.h"
#include "KGCGuildBattleInfo.h"

IMPLEMENT_CLASSNAME( KGCGuildBattleInfo );
IMPLEMENT_WND_FACTORY( KGCGuildBattleInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildBattleInfo, "gc_guildbattle_info" );

KGCGuildBattleInfo::KGCGuildBattleInfo( void )
: m_pkGuildImage(NULL)
, m_pkGuildName(NULL)
, m_pkdBattlePointNum(NULL)
, m_pkBattlePointStr(NULL)
, m_pkNoGuildImg(NULL)
{
    LINK_CONTROL( "guild_name" , m_pkGuildName);
    LINK_CONTROL( "bpoint" , m_pkBattlePointStr);
    LINK_CONTROL( "point_num", m_pkdBattlePointNum);
    LINK_CONTROL( "guild_image", m_pkGuildImage);
    LINK_CONTROL( "no_guild_icon", m_pkNoGuildImg);
}

KGCGuildBattleInfo::~KGCGuildBattleInfo( void )
{
}

void KGCGuildBattleInfo::OnCreate()
{
    m_pkGuildName->InitState(true);
    m_pkGuildName->SetText(L"");
    m_pkGuildName->SetAlign(DT_CENTER);

    m_pkBattlePointStr->InitState(true);
    m_pkBattlePointStr->SetText( g_pkStrLoader->GetString(STR_ID_GUILD_BATTLE_POINT) );
    m_pkdBattlePointNum->InitState(true);
    m_pkdBattlePointNum->SetText(L"");
    m_pkdBattlePointNum->SetAlign( DT_RIGHT );
    m_pkGuildImage->InitState(true);
    m_pkNoGuildImg->InitState(false, true, this);
}

void KGCGuildBattleInfo::ActionPerformed( const KActionEvent& event )
{

}


void KGCGuildBattleInfo::FrameMoveInEnabledState()
{

}

void KGCGuildBattleInfo::SetInformation(std::wstring wstrGuildMarkName_, std::wstring wstrGuildName_, DWORD  dwBattlePoint_ )
{
    m_pkGuildName->SetForceWordWrapText( wstrGuildName_ , true);
    m_pkGuildName->SetToolTip( wstrGuildName_);

    if (wstrGuildName_ == L"" )
        m_pkdBattlePointNum->SetText( L"" );
    else
        m_pkdBattlePointNum->SetText( dwBattlePoint_ );

    SiKGCIconImageManager()->LoadIconImageFromHTTP(wstrGuildMarkName_);
    m_pkGuildImage->SetTextureName(wstrGuildMarkName_);
    CheckGuildMark( wstrGuildMarkName_, wstrGuildName_ );
}

void KGCGuildBattleInfo::CheckGuildMark( std::wstring  wstrGuildMarkName_ , std::wstring wstrGuildName_ )
{
    //길드 마크가 기본이거나 없을때는 길드 아이콘으로 대체 한다.
    if ( wstrGuildMarkName_ == L"defaultmark.dds" ||  wstrGuildMarkName_ == L"" ) {
        m_pkNoGuildImg->ToggleRender( true );
    }
    else {
        m_pkNoGuildImg->ToggleRender( false );
    }
}

