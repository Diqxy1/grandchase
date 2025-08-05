#include "stdafx.h"
//
#include "KGCHeroDungeonLevelBox.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonLevelBox );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonLevelBox );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonLevelBox, "gc_hero_dungeon_level_box" );

KGCHeroDungeonLevelBox::KGCHeroDungeonLevelBox( void )
{
    m_pkLowLevelLeft        = NULL;
    m_pkLowLevelRight       = NULL;

    LINK_CONTROL( "lowlevel_left",      m_pkLowLevelLeft );
    LINK_CONTROL( "lowlevel_right",     m_pkLowLevelRight );

    for( int i = 0; i < EMAX_DIGITS_NUM; i++ )
    {
        m_pkHeroCoinDigits[i] = NULL;
        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1, "herocoin%d", i);
        LINK_CONTROL( szBuf, m_pkHeroCoinDigits[i] );
    }

}

KGCHeroDungeonLevelBox::~KGCHeroDungeonLevelBox(void)
{
}

void KGCHeroDungeonLevelBox::OnCreate()
{
    m_pkLowLevelLeft->InitState( true );
    m_pkLowLevelRight->InitState( true );

    for( int i = 0; i < EMAX_DIGITS_NUM; i++ )
    {
        m_pkHeroCoinDigits[i]->InitState(false);
    }
}

void KGCHeroDungeonLevelBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroDungeonLevelBox::FrameMoveInEnabledState( void )
{
}

void KGCHeroDungeonLevelBox::SetHeroDungeonLevel( int iLevel_ )
{     
    m_pkLowLevelLeft->SetNumber( iLevel_ / 10 );
    m_pkLowLevelRight->SetNumber( iLevel_ % 10 );
}

void KGCHeroDungeonLevelBox::SetHeroCoin( int iCoin_ )
{
    for( int i = 0; i < EMAX_DIGITS_NUM; i++ )
        m_pkHeroCoinDigits[i]->ToggleRender(false);

    if( iCoin_ == 0 )
    {
        m_pkHeroCoinDigits[0]->ToggleRender(true);
        m_pkHeroCoinDigits[0]->SetNumber(0);
    }
    else
    {       
        int iDigit = 0;
        int iMaxDigits = static_cast<int>(EMAX_DIGITS);
        int iDightIdx = 0;
        int iValue = iCoin_;
        for( int i = 0; i < EMAX_DIGITS_NUM; i++ )
        {
            iDigit = static_cast<int>(iValue / iMaxDigits);

            if( iCoin_ >= iMaxDigits )
            {
                m_pkHeroCoinDigits[iDightIdx]->ToggleRender(true);
                m_pkHeroCoinDigits[iDightIdx]->SetNumber(iDigit);
                iDightIdx++;
            }

            iValue = iValue - iDigit * iMaxDigits;
            iMaxDigits /= 10;
        }
    }

}