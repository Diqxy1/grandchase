#include "stdafx.h"
#include "GCRoomUserPotionS6.h"

IMPLEMENT_CLASSNAME( KGCRoomUserPotionS6 );
IMPLEMENT_WND_FACTORY( KGCRoomUserPotionS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomUserPotionS6, "gc_room_user_potion_s6" );

KGCRoomUserPotionS6::KGCRoomUserPotionS6( void )
: m_pkPotionGroup( NULL ) 
{
    LINK_CONTROL( "potiongroup", m_pkPotionGroup);

    char str[MAX_PATH] = { 0, };
    char str2[MAX_PATH] = { 0, };

    for (int i = 0; i < ITEM_SLOT_SIZE; ++i)
    {
        m_apkItemFrame[i] = NULL;
        m_apTextureItem[i] = NULL;
        m_apkItemFrameImage[i] = NULL;

        if (i == 0) {
            LINK_CONTROL("item_frame", m_apkItemFrame[i]);
        }
        else {
            sprintf(str, "item_frame%d", i + 1);
            LINK_CONTROL(str, m_apkItemFrame[i]);
        }

        sprintf( str2, "item_frame_image%d", i + 1 );
        LINK_CONTROL( str2, m_apkItemFrameImage[i] );
    }
}

KGCRoomUserPotionS6::~KGCRoomUserPotionS6( void )
{
}

void KGCRoomUserPotionS6::ActionPerformed( const KActionEvent& event )
{
}


void KGCRoomUserPotionS6::OnCreate( void )
{
    if (m_pkPotionGroup)
        m_pkPotionGroup->InitState(false, true, this);

    //Luma 가 수행한 클래스 최적화
    for (int i = 0; i < ITEM_SLOT_SIZE; ++i)
    {
        if(m_apkItemFrame[i])
            m_apkItemFrame[i]->InitState(false, true, this);

        if (m_apkItemFrameImage[i])
            m_apkItemFrameImage[i]->InitState(false, true, this);
    }
}

void KGCRoomUserPotionS6::SetSlotItem()
{
    int iItemID = 0;
    for (int index = 0; index < ITEM_SLOT_SIZE; ++index)
    {
        SAFE_RELEASE(m_apTextureItem[index]);
        iItemID = g_MyD3D->GetMyShortCutSlot()->GetItemID(index);
        if (iItemID == KItem::INVALID_ITEM)
            m_apTextureItem[index] = NULL;
        else
            m_apTextureItem[index] = g_pItemMgr->CreateShopItemTexture(iItemID);
    }
}

void KGCRoomUserPotionS6::SetSlotStatus( bool bStat ){
    if (m_pkPotionGroup)
	{
		m_pkPotionGroup->ToggleRender( bStat );
	}	

    //Luma 가 수행한 클래스 최적화
    for (int i = 0; i < ITEM_SLOT_SIZE; ++i)
    {
        if (m_apkItemFrame[i])
            m_apkItemFrame[i]->ToggleRender( bStat );

        if (m_apkItemFrameImage[i])
            m_apkItemFrameImage[i]->ToggleRender( bStat );
    }
}

void KGCRoomUserPotionS6::SetSlotImage( int m_iIndex ){
	int quantity = 0;
	float fSize  = 4.0f;
	float fPos_x = 0.5f;

    //Luma 가 수행한 클래스 최적화
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_MyD3D->Get_MyPlayer() == m_iIndex )
    {
	    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
	    {
		    if( m_apTextureItem[index] != NULL )
		    {
                quantity = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(index);
			    g_pGCDeviceManager2->DrawInScreen(m_apTextureItem[index], m_apkItemFrame[index]->GetRelocatedWindowPos().x + (fPos_x + 4) * m_fWindowScaleX, m_apkItemFrame[index]->GetRelocatedWindowPos().y + 4.0f * m_fWindowScaleX, m_apkItemFrame[index]->GetRelocatedWindowPos().x + (fPos_x + 35.0f) * m_fWindowScaleX, m_apkItemFrame[index]->GetRelocatedWindowPos().y + 37.3f * m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f);

			    if( quantity < 10 )
				    g_pkUIMgr->RenderNumber( "room_potion_number", D3DXVECTOR2(m_apkItemFrame[index]->GetRelocatedWindowPos().x+(fPos_x+4.5f)* m_fWindowScaleX, m_apkItemFrame[index]->GetRelocatedWindowPos().y+24.0f * m_fWindowScaleY), quantity );
			    else
				    g_pkUIMgr->RenderNumber( "room_potion_number", D3DXVECTOR2(m_apkItemFrame[index]->GetRelocatedWindowPos().x+(fPos_x+4.5f)* m_fWindowScaleX, m_apkItemFrame[index]->GetRelocatedWindowPos().y+24.0f * m_fWindowScaleY), quantity );
		    }
	    }
    }
}

void KGCRoomUserPotionS6::SlotRelease(){
    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
		SAFE_RELEASE( m_apTextureItem[index] );
}