#include "DragManager.h"

ImplementSingleton( KGCDragManager )

KGCDragManager::KGCDragManager(void)
: m_iDragState(KGDM_DRAG_READY)
, m_pkImage(NULL)
, m_pItem(NULL)
, m_funcProgress(NULL)
, m_fImageWidth( 0.0f )
, m_fImageHeght( 0.0f )
{

}

KGCDragManager::~KGCDragManager(void)
{
}

void KGCDragManager::InitDragState( )
{
	//이벤트 등록 할려 했는데 굳이 함수를 부르면 되는걸 이벤트 불러서 쓸까 생각중... 
	//밑에 비어있으면 걍 함수로 쓰는거임


}

void KGCDragManager::ClearDragState( )
{
	if( m_pkImage != NULL){
		m_pkImage->RemoveAllTexture();
		SAFE_DELETE(m_pkImage);
	}
	m_pkImage = 0;
	m_pItem = NULL;
	m_funcProgress = NULL;
}

void KGCDragManager::ReadyDrag( )
{
	SetDragState( KGDM_DRAG_READY );
}


void KGCDragManager::StartDrag( KItem *pItem , float fImageWidth_, float fImageHeght_, boost::function1<int,bool> a_funcProgress, KPetInfo *pPet /*= NULL*/ )
{
    m_fImageWidth = fImageWidth_;
    m_fImageHeght = fImageHeght_;
	ClearDragState();
	if(pItem == NULL || a_funcProgress == NULL){ 
		ReadyDrag();
		return;
	}
	m_pItem = pItem;
	m_pkImage = new KGC2DObject();
        
    GCITEMID itemID = m_pItem->m_ItemID;
    if ( m_pItem->m_DesignCoordiID != 0 )
    {
        itemID = m_pItem->m_DesignCoordiID;
    }


    KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_pItem->m_ItemID );

    if( kTmpEmoticon == NULL )
    {
        if( pPet != NULL ){
            m_pkImage->AddTexture(g_pItemMgr->CreatePetItemTexture( itemID, pPet->m_cPromotion ));
        }
        else{
            m_pkImage->AddTexture(g_pItemMgr->CreateItemTexture( itemID, m_pItem->m_cCharType < 0 ? 0 : -1 ));
        }
    }

	m_pkImage->SetAxisOffset( 0.0f, 0.0f,0.0f );
	m_pkImage->SetSize( fImageWidth_ * GC_SCREEN_DIV_WIDTH, fImageHeght_ * GC_SCREEN_DIV_WIDTH );
	m_pkImage->CameraMatrixOn();
	m_pkImage->SetAlpha(200);

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousepos;
	vMousepos.x = static_cast<float>(pt.x) / (g_pGCDeviceManager->GetWindowScaleX()) - 30.f * GC_SCREEN_DIV_WIDTH ;
	vMousepos.y = (static_cast<float>(pt.y) / (g_pGCDeviceManager->GetWindowScaleY())) * -1.f + 480.f * GC_SCREEN_DIV_WIDTH ;

	m_pkImage->SetPositionPixel( vMousepos.x, vMousepos.y  );

	m_funcProgress = a_funcProgress;

	SetDragState( KGDM_DRAG_PROGRESS );
}

void KGCDragManager::ProgressDrag( )
{
	//프레임을 돌려준다.
	bool iKeyUp = false;
	if(g_pkInput->BtnUp(KInput::MBLEFT))
		iKeyUp = true;

	int result = m_funcProgress(iKeyUp);
	switch(result)
	{
	case KGDM_DRAG_PROGRESS:
		break;
	case KGDM_DRAG_END : //
		EndDrag();
		break;
	case KGDM_DRAG_CANCEL:
		CancelDrag();
		break;
	default:
		break;
	}
}

void KGCDragManager::Render( )
{
	if(m_pkImage == NULL) return;
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousepos;
	vMousepos.x = static_cast<float>(pt.x) / (g_pGCDeviceManager->GetWindowScaleX()) - 30.f* GC_SCREEN_DIV_WIDTH;
	vMousepos.y = (static_cast<float>(pt.y) / (g_pGCDeviceManager->GetWindowScaleY())) * -1.f + 480.f* GC_SCREEN_DIV_WIDTH;

	m_pkImage->SetPositionPixel( vMousepos.x, vMousepos.y);
	m_pkImage->Render();

    KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_pItem->m_ItemID );
    if ( NULL != kTmpEmoticon )
    {
        vMousepos.x = static_cast<float>(pt.x) - 30.0f / (g_pGCDeviceManager->GetWindowScaleX());
        vMousepos.y = (static_cast<float>(pt.y) - 30.0f / (g_pGCDeviceManager->GetWindowScaleY()));
        // 이모티콘 데이터라면!!!!
        kTmpEmoticon->RenderForShop( vMousepos.x, vMousepos.y, vMousepos.x + m_fImageWidth, vMousepos.y + m_fImageHeght );
    }
}

void KGCDragManager::EndDrag( )
{
	//조건 승락시
	SetDragState( KGDM_DRAG_END );
	ClearDragState();
	ReadyDrag();
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DRAG_ITEM_END ) );
}

void KGCDragManager::CancelDrag( )
{
	SetDragState( KGDM_DRAG_CANCEL );
	ClearDragState();
	ReadyDrag();
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DRAG_ITEM_END ) );
}


void KGCDragManager::SetDragState( int iState )
{
    m_iDragState = iState;
}