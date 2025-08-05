#include "stdafx.h"
#include "GCSphinxResultItemBox.h"

IMPLEMENT_CLASSNAME( KGCSphinxResultItemBox );
IMPLEMENT_WND_FACTORY( KGCSphinxResultItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxResultItemBox, "gc_sphinx_item_box" );

KGCSphinxResultItemBox::KGCSphinxResultItemBox( void )
: m_pkItemImage(NULL)
, m_pkItemNum(NULL)
, m_ItemID(0)
{
    LINK_CONTROL("number",m_pkItemNum);
}

KGCSphinxResultItemBox::~KGCSphinxResultItemBox( void )
{
}

void KGCSphinxResultItemBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCSphinxResultItemBox::OnCreate( void )
{
    m_pkItemNum->InitState(true,false,NULL);
}

void KGCSphinxResultItemBox::PostDraw()
{
    if(m_pkItemImage != NULL)
        m_pkItemImage->Render();
}

void KGCSphinxResultItemBox::SetItem( GCITEMID itemID, int num /**/) 
{
    ClearImage();

    m_pkItemImage = new KGC2DObject();

    m_pkItemImage->AddTexture(g_pItemMgr->CreateItemTexture(itemID/10));

    m_pkItemImage->SetAxisOffset( 0.0f, 0.0f,0.0f );
    m_pkItemImage->SetSize( 53.0f , 53.0f );
    m_pkItemImage->CameraMatrixOn();

    m_pkItemNum->SetNumber( num );
    m_pkItemNum->SetStringType( "blue_gradation_num" );

    m_ItemID = itemID/10;
}

void KGCSphinxResultItemBox::SetItemImagePos()
{
    float posx = 0.0f;
    float posy = 0.0f;

    posx = GetFixedWindowPos().x + 0.0f ;
    posy = -1 * GetFixedWindowPos().y + 459.0f * GC_SCREEN_DIV_WIDTH;
    

    m_pkItemImage->SetPositionPixel(  posx, posy );
}

void KGCSphinxResultItemBox::ClearImage()
{
    if(m_pkItemImage != NULL)
    {
        m_pkItemImage->RemoveAllTexture();
        SAFE_DELETE(m_pkItemImage);
    }
}

void KGCSphinxResultItemBox::OnDestroy()
{
    ClearImage();
}