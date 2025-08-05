#include "stdafx.h"
#include "KGCMissionRewardBox_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionRewardBox_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionRewardBox_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionRewardBox_S4, "gc_mission_rewardbox_s4" );

KGCMissionRewardBox_S4::KGCMissionRewardBox_S4( void )
: m_pkBack( NULL )
, m_pkImg( NULL )
, m_cGrade( -1 )
{
    LINK_CONTROL( "background", m_pkBack );
    LINK_CONTROL( "img_shop", m_pkImg );
    LINK_CONTROL( "static_period", m_pkPeriod );

    char szTemp[128];
    for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
    {
        sprintf(szTemp, "frame_grade%d", iLoop+1);
        LINK_CONTROL( szTemp, m_pkGradeFrame[ iLoop] );
    }
}

KGCMissionRewardBox_S4::~KGCMissionRewardBox_S4( void ){}

void KGCMissionRewardBox_S4::OnCreateComplete( void )
{
    if( m_pkBack )
        m_pkBack->InitState(true, false);

    if( m_pkImg )
        m_pkImg->InitState(true, false);

    if( m_pkPeriod )
    {
        m_pkPeriod->InitState(true, false);
        m_pkPeriod->SetFontOutline( true );
        m_pkPeriod->SetAlign( DT_RIGHT );
        m_pkPeriod->SetFontSpace(1);
    }
}

void KGCMissionRewardBox_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionRewardBox_S4::FrameMoveInEnabledState( void ){}

void KGCMissionRewardBox_S4::SetBackWndMode( KD3DWnd::ED3DWndMode eMode )
{
    if( m_pkBack )
        m_pkBack->SetWndMode( eMode );
}

DWORD KGCMissionRewardBox_S4::GetSettingItemID()
{
    if( m_pkImg )
        return m_pkImg->GetItemID();
    return 0;
}

void KGCMissionRewardBox_S4::SetItem( const SMissionInfo::SReward& reawrd )
{
    if( m_pkImg )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( reawrd.m_ItemID/10 );
        m_pkImg->SetItemInfo( reawrd.m_ItemID/10, -1, (pItem->dwSlotPosition&ESP_SET_ITEM), pItem->iSetItemNumber );
        m_pkImg->ShowItem( true );

        if( m_pkPeriod )
        {
            std::wostringstream strWstring;
            if( 0 < reawrd.m_nPeriod ) {
#if defined (NATION_BRAZIL)
                m_pkPeriod->SetAlign( DT_CENTER );
#endif
                strWstring << g_pkStrLoader->GetReplacedString( STR_ID_DURATION_PERIOD, "i", reawrd.m_nPeriod );
            }
            else if( 0 < reawrd.m_nCount ) {
#if defined (NATION_BRAZIL)
                m_pkPeriod->SetAlign( DT_RIGHT );
#endif
                strWstring << g_pkStrLoader->GetReplacedString( STR_ID_DURATION_COUNT, "i", reawrd.m_nCount );
            }
            else
                strWstring.str(L"");

            m_pkPeriod->SetText( strWstring.str() );
        }

        m_cGrade = reawrd.m_cGrade;
        if ( m_cGrade == -1 ) 
            m_cGrade = pItem->cItemGrade;

        for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
        {
            if( m_cGrade == (iLoop+1) )
                m_pkGradeFrame[ iLoop ]->ToggleRender( true );
            else
                m_pkGradeFrame[ iLoop ]->ToggleRender( false );
        }
    }
}

void KGCMissionRewardBox_S4::ClearItem()
{
    if( m_pkImg )
    {
        m_pkImg->SetItemInfo( 0 );
        m_pkImg->ShowItem( false );
        m_pkPeriod->SetText( std::wstring(L"") );

        for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
            m_pkGradeFrame[ iLoop ]->ToggleRender( false );

        SetBackWndMode( D3DWM_DEFAULT );
    }
}