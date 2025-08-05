#include "stdafx.h"
#include "GCRecordOptionBox.h"

IMPLEMENT_CLASSNAME( KGCRecordOptionBox );
IMPLEMENT_WND_FACTORY( KGCRecordOptionBox );
IMPLEMENT_WND_FACTORY_NAME( KGCRecordOptionBox, "gc_record_option_box" );

KGCRecordOptionBox::KGCRecordOptionBox( void )
{
    for( int i=0 ; i< MAX_LIST; ++i )
    {
        m_apkStaticInfo[i] = NULL;

        char szTemp[MAX_PATH];
        sprintf_s(szTemp, MAX_PATH - 1, "static_info%d", i);
        LINK_CONTROL(szTemp, m_apkStaticInfo[i]);
    }

    m_pkScrollBar = NULL;
    LINK_CONTROL("scroll", m_pkScrollBar);

    m_iIndex = -1;
}

KGCRecordOptionBox::~KGCRecordOptionBox( void )
{

}

void KGCRecordOptionBox::OnCreate( void )
{
    for( int i=0 ; i<MAX_LIST ; ++i )
    {
        m_apkStaticInfo[i]->InitState(true, true, this);
        m_apkStaticInfo[i]->SetAlign( DT_LEFT );
        m_apkStaticInfo[i]->SetFontColor(0xffffffff);
    }

    m_pkScrollBar->SetSelfInputCheck(false);
    m_pkScrollBar->SetHeight(75);
    m_pkScrollBar->SetScrollPos(0);
    m_pkScrollBar->SetScrollGap( 1 );
    m_pkScrollBar->SetThumbYGab( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollRangeMax( 4 );
    m_pkScrollBar->ResizeBtn();
}

void KGCRecordOptionBox::ActionPerformed( const KActionEvent& event )
{
    for( int i=1 ; i<MAX_LIST ; ++i )
    {
        if( event.m_pWnd == m_apkStaticInfo[i] )
        {
            m_apkStaticInfo[i]->SetFontColor(0xffffff00);
            m_apkStaticInfo[0]->SetText( m_apkStaticInfo[i]->GetText() );

        }
        else
            m_apkStaticInfo[i]->SetFontColor(0xffffffff);
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for( int i=1 ; i<MAX_LIST ; ++i )
        {
            if( event.m_pWnd == m_apkStaticInfo[i] )
            {
                m_iIndex = i;
                SetNowValue();
                SpeakToActionListener( KActionEvent( this, FET_OK ) );
            }
        }
    }
}

void KGCRecordOptionBox::SetNowValue( void )
{
    m_apkStaticInfo[0]->SetText( m_apkStaticInfo[m_iIndex]->GetText() );
}

void KGCRecordOptionBox::SetList( std::vector<std::wstring> &vecList )
{
    for( int i=0 ; i< (int)vecList.size() ; ++i )
        m_apkStaticInfo[ i+1 ]->SetText( vecList[i] );
}