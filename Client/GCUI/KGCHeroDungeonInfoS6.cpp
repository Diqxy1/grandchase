#include "stdafx.h"
#include "KGCWorldMapManager.h"
#include "KGCHeroDungeonInfoS6.h"
#include "KGCHeroDungeonInfoListS6.h"
#include "KGCHeroDungeonOpentimeBoxS6.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonInfoS6 );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonInfoS6, "gc_herodungeoninfos6dlg" );

std::wstring GetAmPm( int iTime_ )
{
	if( iTime_ >= 12 && iTime_ < 24 )
		return L"PM";
	else
		return L"AM";    
}

int GetTwelveTime( int iTime_ )
{
	if( iTime_ > 12 )
		return iTime_ - 12;
	else
	{
		if( iTime_ == 0 )
			iTime_ = 12;
		return iTime_;
	}
}

std::wstring GetEnableEnteranceTimeStr( int iStartHour_, int iStartMin_, int iEndHour_, int iEndMin_ )
{
	std::wstringstream str;

	if( iStartHour_ == 0 )
		iStartHour_ = 24;
	if( iEndHour_ == 0 )
		iEndHour_ = 24;
#if defined(NATION_USA) || defined( NATION_EU ) // �̱���AM,PM�� �ڿ� ����.

	if( GetTwelveTime(iStartHour_) < 10 )
		str << L"0";

	str << GetTwelveTime(iStartHour_) << L":";

	if( iStartMin_ < 10 )
		str << L"0";

	str << iStartMin_ << GetAmPm(iStartHour_)<<L" ~ " ;

	if( GetTwelveTime(iEndHour_) < 10 )
		str << L"0";

	str<< GetTwelveTime(iEndHour_) << L":";

	if( iEndMin_ < 10 )
		str << L"0";

	str<< iEndMin_;   
	str<< GetAmPm(iEndHour_);
#else
	str << GetAmPm(iStartHour_);

	if( GetTwelveTime(iStartHour_) < 10 )
		str << L"0";

	str << GetTwelveTime(iStartHour_) << L":";

	if( iStartMin_ < 10 )
		str << L"0";

	str << iStartMin_ << L"~" << GetAmPm(iEndHour_);

	if( GetTwelveTime(iEndHour_) < 10 )
		str << L"0";

	str<< GetTwelveTime(iEndHour_) << L":";

	if( iEndMin_ < 10 )
		str << L"0";

	str<< iEndMin_;   
#endif
	return str.str();
}

KGCHeroDungeonInfoS6::KGCHeroDungeonInfoS6(void)
: m_pkBtnClose ( NULL )
, m_pkBtnLeft ( NULL )
, m_pkBtnRight ( NULL )
{
	for( int i = 0; i < EMAX_DUNGEON_LIST_NUM; i++ )
	{
		m_pkDungeonInfoSlot[i] = NULL;
		if( i < 10 ) {
			LINK_CONTROL_STM("hero_slot0" << i, m_pkDungeonInfoSlot[i]);
		}
		else {
			LINK_CONTROL_STM("hero_slot" << i, m_pkDungeonInfoSlot[i]);
		}
	}

	LINK_CONTROL( "btn_close",		m_pkBtnClose);
	LINK_CONTROL( "btn_left",		m_pkBtnLeft);
	LINK_CONTROL( "btn_right",		m_pkBtnRight);
}

KGCHeroDungeonInfoS6::~KGCHeroDungeonInfoS6(void)
{
}

void KGCHeroDungeonInfoS6::OnCreate()
{
	if( m_pkBtnClose )
	{
		m_pkBtnClose->InitState( true, true, this );
		m_pkBtnClose->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkBtnRight )
	{
		m_pkBtnRight->InitState( false, true, this );
	}

	if( m_pkBtnLeft )
	{
		m_pkBtnLeft->InitState( false, true, this );
	}

	for( int i = 0; i < EMAX_DUNGEON_LIST_NUM; i++ )
	{
		m_pkDungeonInfoSlot[i]->InitState( false, true, this );
	}
}

void KGCHeroDungeonInfoS6::OnCreateComplete()
{
	for( int i = 0; i < EMAX_DUNGEON_LIST_NUM; ++i )
	{
		if( i >= g_kGlobalValue.GetEventDungeonListSize() )
			break;
		m_pkDungeonInfoSlot[i]->ToggleRender( true );
		m_pkDungeonInfoSlot[i]->SetDungeonDetailInfo( g_kGlobalValue.m_vecEventDungeonList[i] );
	}
}

void KGCHeroDungeonInfoS6::OnDestroyComplete()
{
}

void KGCHeroDungeonInfoS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
}

void KGCHeroDungeonInfoS6::FrameMoveInEnabledState( void )
{
}

void KGCHeroDungeonInfoS6::PostChildDraw( void )
{
}

int KGCHeroDungeonInfoS6::GetListViewCount(int iIndex_)
{
	int iListViewCount = static_cast<int>(m_pkDungeonInfoSlot.size() ) - iIndex_;

	if(iListViewCount < EMAX_DUNGEON_LIST_NUM)
		return iListViewCount;

	return EMAX_DUNGEON_LIST_NUM;
}

void KGCHeroDungeonInfoS6::OnExit()
{
	this->ToggleRender( false );
	g_pkUIScene->EndMessageBox();
}