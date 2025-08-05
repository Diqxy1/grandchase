#include "stdafx.h"
#include "KGCPackageItemBox.h"

IMPLEMENT_CLASSNAME( KGCPackageItemBox );
IMPLEMENT_WND_FACTORY( KGCPackageItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPackageItemBox, "gc_package_item_box" );

KGCPackageItemBox::KGCPackageItemBox(void)
: m_pkBack(NULL)
, m_pkImg(NULL)
, m_pkPeriod(NULL)
{
    LINK_CONTROL( "background", m_pkBack );
    LINK_CONTROL( "img_shop", m_pkImg );
    LINK_CONTROL( "static_period", m_pkPeriod );

    char szTemp[256];
    for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
    {
        m_pkGradeFrame[iLoop] = NULL;
        sprintf(szTemp, "frame_grade%d", (iLoop+1));
        LINK_CONTROL( szTemp, m_pkGradeFrame[iLoop] );
    }
}

KGCPackageItemBox::~KGCPackageItemBox(void)
{
}

void KGCPackageItemBox::OnCreate( void )
{
    if( m_pkBack )
        m_pkBack->InitState( true );

    if( m_pkImg )
        m_pkImg->InitState( true, true, this );

    if( m_pkPeriod )
        m_pkImg->InitState( true );

    for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
    {
        if( m_pkGradeFrame[iLoop] )
            m_pkGradeFrame[iLoop]->InitState( false );
    }
}

void KGCPackageItemBox::ActionPerformed( const KActionEvent& event ){}

void KGCPackageItemBox::FrameMoveInEnabledState( void ){}

bool KGCPackageItemBox::SetItemInfoBox()
{
    // 아이템 ID 0 하면 지나감 
    if( m_pkImg->GetItemID() <= 0 ) {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return false;
    }

    D3DXVECTOR2 vPos = this->GetCurrentWindowPos();
    vPos.y += GetHeight();
    vPos.x *= m_fWindowScaleX;    vPos.y *= m_fWindowScaleY;

    GCItem* kItem = g_pItemMgr->GetItemData( m_pkImg->GetItemID() );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );

    return true;
}

void KGCPackageItemBox::SetItem( KDropItemInfo *paItemInfo )
{
    if( paItemInfo )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( paItemInfo->m_ItemID / 10 );
        if( pItem && m_pkImg )
        {
            m_pkImg->SetItemInfo( paItemInfo->m_ItemID / 10, -1, (pItem->dwSlotPosition&ESP_SET_ITEM), pItem->iSetItemNumber );
            m_pkImg->ShowItem( true );

            if( m_pkPeriod )
            {
                std::wostringstream strWstring;
                if( 0 < paItemInfo->m_nPeriod )
                    strWstring << g_pkStrLoader->GetReplacedString( STR_ID_DURATION_PERIOD, "i", paItemInfo->m_nPeriod );
                else if( 0 < paItemInfo->m_nDuration )
                    strWstring << g_pkStrLoader->GetReplacedString( STR_ID_DURATION_COUNT, "i", paItemInfo->m_nDuration );
                else
                    strWstring.str(L"");

                m_pkPeriod->SetText( strWstring.str() );
            }

            for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
            {
                if( paItemInfo->m_cGradeID == (iLoop+1) )
                    m_pkGradeFrame[ iLoop ]->ToggleRender( true );
                else
                    m_pkGradeFrame[ iLoop ]->ToggleRender( false );
            }
        }
    }
    else
    {
        if( m_pkImg && m_pkPeriod )
        {
            m_pkImg->SetItemInfo( 0 );
            m_pkImg->ShowItem( false );
            m_pkPeriod->SetText( std::wstring(L"") );
        }
        for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
        {
            if( m_pkGradeFrame[ iLoop ] )
                m_pkGradeFrame[ iLoop ]->ToggleRender( false );
        }
    }
}

void KGCPackageItemBox::SetBoxSize(int iWidth, int iHeight)
{
    m_pkBack->SetSize( iHeight, iWidth );
    m_pkImg->SetHeight( iHeight );
    m_pkImg->SetWidth( iWidth );
    m_pkPeriod->SetHeight( iHeight-36 );
    m_pkPeriod->SetWidth( iWidth-16 );
    for(int iLoop=0; iLoop<(KItem::GRADE_NUM-1); ++iLoop)
    {
        if( m_pkGradeFrame[iLoop] )
        {
            m_pkGradeFrame[iLoop]->SetHeight( iHeight-7 );
            m_pkGradeFrame[iLoop]->SetWidth( iWidth-8 );
        }
    }
}