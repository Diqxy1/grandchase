#include "GCInGameMsgItemWnd.h"

IMPLEMENT_CLASSNAME( KGCInGameMsgItemWnd );
IMPLEMENT_WND_FACTORY( KGCInGameMsgItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCInGameMsgItemWnd, "gc_ingamemsg_item_wnd" );

KGCInGameMsgItemWnd::KGCInGameMsgItemWnd()
{
    m_pkItemImg = NULL;
    m_pkSmallDurationNum = NULL;
    m_pkStaticName = NULL;

    for(int iLoop=0; iLoop<KItem::GRADE_NUM; ++iLoop )
    {
        m_pkGradeFrame[iLoop] = NULL;
        LINK_CONTROL_STM( "item_frame_grade"<<iLoop, m_pkGradeFrame[iLoop] );
    }

    LINK_CONTROL( "item_img", m_pkItemImg );
    LINK_CONTROL( "number_duration_small", m_pkSmallDurationNum );
    LINK_CONTROL( "item_name", m_pkStaticName );
}

KGCInGameMsgItemWnd::~KGCInGameMsgItemWnd(){}
void KGCInGameMsgItemWnd::OnCreateComplete( void ){}
void KGCInGameMsgItemWnd::ActionPerformed( const KActionEvent& event ){}
void KGCInGameMsgItemWnd::FrameMoveInEnabledState( void ){}
void KGCInGameMsgItemWnd::SetETCImg( std::string strFileName )
{
    GCDeviceTexture* pDeviceTex = g_pGCDeviceManager2->CreateTexture( strFileName );
    if( NULL == pDeviceTex )
        return;

    m_pkItemImg->ShowItem( true );
    m_pkItemImg->SetItemTexture( pDeviceTex );
    m_pkItemImg->ToggleRender( true );

    m_pkSmallDurationNum->ToggleRender( false );
    m_pkStaticName->ToggleRender( false );
    for(int iLoop=0; iLoop<KItem::GRADE_NUM; ++iLoop )
    {
        if( m_pkGradeFrame[iLoop] == NULL )
            continue;

        m_pkGradeFrame[iLoop]->ToggleRender( false );
    }
}
void KGCInGameMsgItemWnd::SetItem( GCItem* pItem, int iGrade, int iPeriod, int iCount )
{
    if( NULL == pItem )
        return;

    SetItemImg( pItem );
    SetGradeFrame( iGrade );
    SetDuration( iCount );
    SetItemName( pItem->strItemName.c_str() );

    m_kItem = *pItem;
}

void KGCInGameMsgItemWnd::SetItemImg( GCItem* pItem )
{
    m_pkItemImg->ShowItem( true );
    m_pkItemImg->SetItemInfo( pItem->dwGoodsID, -1, (pItem->dwSlotPosition&ESP_SET_ITEM), pItem->iSetItemNumber );
    m_pkItemImg->ToggleRender( true );
}

void KGCInGameMsgItemWnd::SetGradeFrame( int iGradeID )
{
    if( iGradeID < 0 || iGradeID >= KItem::GRADE_NUM )
        iGradeID = KItem::GRADE_NORMAL;

    for(int iLoop=0; iLoop<KItem::GRADE_NUM; ++iLoop )
    {
        if( m_pkGradeFrame[iLoop] == NULL )
            continue;

        m_pkGradeFrame[iLoop]->ToggleRender( (iLoop == iGradeID) );
    }
}

void KGCInGameMsgItemWnd::SetDuration( int iCount )
{
    if( 0 < iCount )
    {
        m_pkSmallDurationNum->SetNumber( iCount );
        m_pkSmallDurationNum->SetStringType( "very_small_num" );
        m_pkSmallDurationNum->ToggleRender( true );
    }
    else
    {
        m_pkSmallDurationNum->ToggleRender( false );
    }
}

void KGCInGameMsgItemWnd::SetItemName( std::wstring strItemName )
{
    m_pkStaticName->SetAlign( DT_CENTER );
    m_pkStaticName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
    m_pkStaticName->SetFontOutline( true, D3DCOLOR_ARGB(255,0,0,0) );
    m_pkStaticName->SetAutoScroll( true );
    m_pkStaticName->SetText( strItemName.c_str() );
    m_pkStaticName->ToggleRender( true );
}