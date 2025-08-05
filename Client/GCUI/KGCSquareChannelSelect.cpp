#include "stdafx.h"
#include "KGCSquareChannelSelect.h"
#include "../Square/GCSquare.h"
#include "KGCSquareChannelBar.h"
//

IMPLEMENT_CLASSNAME( KGCSquareChannelSelect );
IMPLEMENT_WND_FACTORY( KGCSquareChannelSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareChannelSelect, "gc_square_channel_select" );

#define ERR_SHOW_TIME (boost::mpl::multiplies<\
						boost::mpl::int_<55>,\
						boost::mpl::int_<3>\
					>::type::value)

KGCSquareChannelSelect::KGCSquareChannelSelect( void )
: m_pkErrStatic(NULL)
, m_pkErrString(NULL)
, m_dwErrShow(0)
, m_iChannelNum(0)
{
    for( int i = 0; i < MAX_SQUARE_CHANNEL_NUM; ++i )
    {
        m_pkChannelBar[i] = NULL;
    }

    std::stringstream stm;
    for( int i = 0; i < MAX_SQUARE_CHANNEL_NUM; ++i )
    {
        stm.str("");
        stm << "channel_bar" << i + 1;
        LINK_CONTROL( stm.str(), m_pkChannelBar[i] );
    }

    LINK_CONTROL( "error_string", m_pkErrString );
    LINK_CONTROL( "err_static", m_pkErrStatic );
}

KGCSquareChannelSelect::~KGCSquareChannelSelect( void )
{
}

void KGCSquareChannelSelect::OnCreate( void )
{
    for( int i = 0; i < MAX_SQUARE_CHANNEL_NUM; ++i )
    {
        m_pkChannelBar[i]->InitState( false );
    }

    m_pkErrString->InitState( false );
    m_pkErrStatic->InitState( false );
}

void KGCSquareChannelSelect::UpdateData()
{
    for( int i = 0; i < MAX_SQUARE_CHANNEL_NUM; ++i )
    {
        m_pkChannelBar[i]->ToggleRender( false );
    }

    std::vector< KSquareInfo > vecSquareInfo = SiGCSquare()->GetSquareInfo();
    ASSERT( vecSquareInfo.size() > 0 );
    int iEnd = (int)vecSquareInfo.size() > MAX_SQUARE_CHANNEL_NUM 
                ? MAX_SQUARE_CHANNEL_NUM : (int)vecSquareInfo.size();
	m_iChannelNum = iEnd;
    for( int i = 0; i < iEnd; ++i )
    {
        m_pkChannelBar[i]->ToggleRender( true );
        m_pkChannelBar[i]->SetSquareChannelInfo( vecSquareInfo[i], i );
    }
}

void KGCSquareChannelSelect::ActionPerformed( const KActionEvent& event )
{
}

void KGCSquareChannelSelect::SetError( int iStrID )
{
    m_pkErrString->ToggleRender( true );
    m_pkErrStatic->SetTextAutoMultiline( g_pkStrLoader->GetString(iStrID) );
    m_pkErrStatic->ToggleRender( true );
    m_dwErrShow = 0;
}

void KGCSquareChannelSelect::FrameMoveInEnabledState()
{
    if( m_pkErrStatic != NULL && m_pkErrString->IsRenderOn() == true )
    {
        m_dwErrShow++;

        if( m_dwErrShow >= ERR_SHOW_TIME )
        {
            m_dwErrShow = 0;
            m_pkErrString->ToggleRender( false );
            m_pkErrStatic->ToggleRender( false );
        }
    }
}