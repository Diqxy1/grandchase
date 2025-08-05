#include "stdafx.h"
#include "KGCCharGetInfoPanel.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCharGetInfoPanel );
IMPLEMENT_WND_FACTORY( KGCCharGetInfoPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCCharGetInfoPanel, "gc_char_get_info_panel" );

KGCCharGetInfoPanel::KGCCharGetInfoPanel( void ) 
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        m_apkCharName[i] = NULL;

        char szTemp[MAX_PATH];
        sprintf_s(szTemp, MAX_PATH - 1, "char_name%d", i);
        LINK_CONTROL( szTemp, m_apkCharName[i] );
    }

    m_pkStringBack = NULL;
    LINK_CONTROL("char_string_back", m_pkStringBack);

    m_pkCharString = NULL;
    LINK_CONTROL( "char_string", m_pkCharString );

    m_iCharType = -1;
}

KGCCharGetInfoPanel::~KGCCharGetInfoPanel( void )
{

}

void KGCCharGetInfoPanel::OnCreate( void )
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
        if (m_apkCharName[i])
            m_apkCharName[i]->ToggleRender( false );




    m_pkStringBack->ToggleRender(false);

    m_pkCharString->SetFontColor(0xffffffff);
    m_pkCharString->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkCharString->SetShadow(true);
    m_pkCharString->ToggleRender(false);
    m_pkCharString->SetMultiLine(true);
    m_pkCharString->SetAlign(DT_LEFT);
}

void KGCCharGetInfoPanel::SetNewCharInfo(int iCharType)
{
    if( iCharType < 0 || iCharType > GC_CHAR_NUM )
        return;

    m_iCharType = iCharType;

    int iStrNum = 0;
    if( m_iCharType > GC_CHAR_ARME )
        iStrNum = m_iCharType - 3;

    switch( m_iCharType ) {
        case GC_CHAR_ZERO:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER11) );
            break;

        case GC_CHAR_LEY:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER12) );
            break;
        
        case GC_CHAR_LUPUS:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER13) );
            break;

        case GC_CHAR_ASIN:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER15) );
            break;
        case GC_CHAR_LIME:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER16) );
            break;
        case GC_CHAR_EDEL:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER17) );
            break;
        case GC_CHAR_BEIGAS:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_ITEM_CHAR_TYPE_COMMON) );
            break;
        default:
            m_pkCharString->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_GET_NEW_CHARACTER3 + iStrNum) );
            break;

    }
}

void KGCCharGetInfoPanel::SetShow(bool bShow)
{
    if( m_iCharType < 0 || m_iCharType > GC_CHAR_NUM )
        return;

    this->ToggleRender(bShow);
    m_apkCharName[m_iCharType]->ToggleRender( bShow );
    m_pkStringBack->ToggleRender(bShow);
    m_pkCharString->ToggleRender( bShow );
}
