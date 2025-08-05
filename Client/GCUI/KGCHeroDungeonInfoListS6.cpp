#include "StdAfx.h"
#include "KGCHeroDungeonInfoListS6.h"
#include "KGCHeroDungeonInfoS6.h"

extern std::wstring GetEnableEnteranceTimeStr( int iStartHour_, int iStartMin_, int iEndHour_, int iEndMin_ );

IMPLEMENT_CLASSNAME( KGCHeroDungeonInfoListS6 );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonInfoListS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonInfoListS6, "gc_herodungeoninfoslot" );

KGCHeroDungeonInfoListS6::KGCHeroDungeonInfoListS6(void)
: m_pkStaticName(NULL)
, m_pkStaticCondition(NULL)
{
    LINK_CONTROL("static_dungeon_name",	    m_pkStaticName );
    LINK_CONTROL("static_condition",		m_pkStaticCondition );
}

KGCHeroDungeonInfoListS6::~KGCHeroDungeonInfoListS6(void)
{
}

void KGCHeroDungeonInfoListS6::OnCreate( void )
{
    m_pkStaticName->InitState(true, true);
    m_pkStaticCondition->InitState(true, true);

    InitDungeonInfo();
}

void KGCHeroDungeonInfoListS6::OnCreateComplete( void )
{
}

void KGCHeroDungeonInfoListS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroDungeonInfoListS6::OnDestroyComplete( void )
{
}

void KGCHeroDungeonInfoListS6::FrameMoveInEnabledState( void )
{
}

void KGCHeroDungeonInfoListS6::PostChildDraw( void )
{
}

void KGCHeroDungeonInfoListS6::InitDungeonInfo()
{
    m_pkStaticName->SetText(L"");
    m_pkStaticCondition->SetText(L"");
}

void KGCHeroDungeonInfoListS6::SetDungeonDetailInfo( KGCGlobal::KDetailDungeonInfo kInfo_ )
{
	m_pkStaticName->SetText( kInfo_.wstrName );
    m_pkStaticCondition->SetText( kInfo_.wstrOpen );
}