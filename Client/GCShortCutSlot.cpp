#include "stdafx.h"
//

#include "GCShortCutSlot.h"
#include "Headup Display.h"
//
//
//

#include "MyD3D.h"

#include "Potion_Buff.h"
#include "ProtectRing.h"
#include "EmotionItem.h"
#include "LatencyTransfer.h"
#include "Monster.h"
////
//

KGCShortCutSlot::KGCShortCutSlot()
{
    for( int i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        m_ItemSlot[i] = NULL;
        m_ItemSlot[i] = new KGCShortCutSlotItem;
    }
    for( int i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        m_EmotSlot[i] = NULL;
        m_EmotSlot[i] = new KGCShortCutSlotItem;
    }
}

KGCShortCutSlot::~KGCShortCutSlot()
{
    for( INT index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
    {
        if( m_ItemSlot[index] == NULL )
            continue;

        SAFE_DELETE( m_ItemSlot[index] );
    }
    for( INT index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
    {
		if( m_EmotSlot[index] == NULL )
			continue;
       SAFE_DELETE( m_EmotSlot[index] );
    }
}


//////////////////////////////////////////////////////////////////////////
//						Public Member Functions							//
//////////////////////////////////////////////////////////////////////////

// Short Cut Slot을 초기화한다.
void KGCShortCutSlot::initialization()
{
    Clear();
    m_kItemDefine.LoadItem();	// stg 파일로 부터 정보를 읽어옴다.
}
void KGCShortCutSlot::Clear()
{
    ClearItem();
    ClearEmoticon();
}
void KGCShortCutSlot::ClearItem()
{
    for( INT i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        DeleteItembyIndex(i);
    }
}
void KGCShortCutSlot::ClearEmoticon()
{
    for( INT i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        DeleteEmotbyIndex(i);
    }
}

// Item 정보를 받아 빈 Slot에 삽입하고 Index를 반환. 실패하면 -1을 반환.
INT KGCShortCutSlot::InsertItem( KItem* _kInven )
{
    INT iInsertIndex = KItem::INVALID_ITEM;
    INT itemID = _kInven->m_ItemID;

    // Insert가 불가능하면  -1 반환
    if( ( iInsertIndex = CanbeInsertItem(itemID) ) == (-1) )
    {	return (-1);   }
    else
    {
        // 받아온 ItemID로
        // 각 Item에 맞는 삽입함수 호출
        SCS_ITEM_KIND itemKind = static_cast<SCS_ITEM_KIND>(m_kItemDefine.GetItemKind(itemID));
        if( InsertItem( iInsertIndex, itemKind, _kInven ) == true )
            return iInsertIndex;
    }

    return (-1);
}

INT KGCShortCutSlot::InsertEmot( KItem* kItem_ )
{
    for ( int i = 0 ; i < EMOT_SLOT_SIZE; i++ )
    {
        if ( m_EmotSlot[i]->GetItemID() == kItem_->m_ItemID )
        {
            return (-1);
        }        
    }

    for ( int i = 0 ; i < EMOT_SLOT_SIZE; i++ )
    {
        if ( m_EmotSlot[i]->GetItemID() == KItem::INVALID_ITEM )
        {
            SAFE_DELETE( m_EmotSlot[i] );
            m_EmotSlot[i] = new EmotionItem( kItem_, &m_MyPlayer, NULL );
            return i;
        }
    }
    return (-1);

}

// 장비창에서 인벤토리로 아이템을 빼고 인벤토리에 수량을 돌려준다.
void KGCShortCutSlot::DischargeItem( DWORD _itemID )
{
    int index = FindItemIndex(_itemID);
    if ( index == -1 )
    {
        return;
    }
    int quantity	= GetItemQuantity(index);
    KItem*	kInventoryItem;
    if( g_MyD3D->m_kItemMgr.FindInventory( GetItemUID(index), &kInventoryItem ) == true ){
        if( kInventoryItem != NULL )
            kInventoryItem->m_nCount += quantity;
    }
    else{
        KItem ItemInfo;

        // 뭐 막 만들자
        ItemInfo.m_ItemUID	    =  m_ItemSlot[index]->GetItemUID() ;
        ItemInfo.m_ItemID	    = m_ItemSlot[index]->GetItemID()*10;
        ItemInfo.m_nCount	= m_ItemSlot[index]->GetQuantity();

        g_pItemMgr->m_kInventory.AddItem( ItemInfo );
    }

    DeleteItembyIndex(index);
}

void KGCShortCutSlot::DischargeEmot(DWORD itemID )
{
    int index = FindEmotIndex(itemID);

    if( index == -1 )
        return;
    //KDurationItemInfo ItemInfo;
    //std::vector<KDurationItemInfo> vecItemInfo;
    //vecItemInfo.clear();

    //// 뭐 막 만들자
    //ItemInfo.m_dwGoodsUID	=  m_EmotSlot[index]->GetItemUID() ;
    //ItemInfo.m_dwGoodsID	= m_EmotSlot[index]->GetItemID()*10;
    //ItemInfo.m_dwCurrentNum	= m_EmotSlot[index]->GetQuantity();
    //vecItemInfo.push_back(ItemInfo);
    //g_MyD3D->m_kItemMgr.AddInventory(vecItemInfo);     

    DeleteEmotbyIndex( index );
}

// 입력된 Item과 같은 Type의 Item을 DisCharge 시키고 입력된 Item을 Insert시킨다
bool KGCShortCutSlot::ExchangeItem( IN KItem* kInven_ )
{
    int iSrcID = kInven_->m_ItemID;

    // 입력된 아이템이 어떤 계열인가 알아보고
    int iItemType = m_kItemDefine.GetItemKind(static_cast<DWORD>(iSrcID));
    if( iItemType == (-1) )
        return false;

    // 그 계열의 아이템이 장착된 슬롯의 Index를 알아온 뒤,
    int iIndex = FindItemType(iItemType);
    if(iIndex == (-1) )
        return false;

    // 해당 Item을 제거한다.
    int iItemID = m_ItemSlot[iIndex]->GetItemID();
    DischargeItem(iItemID);

    if( InsertItem( kInven_ ) == (-1) )
        return false;


    return true;
}

bool KGCShortCutSlot::SlotRefill()
{
    DWORD itemID	= 0;
    GCITEMUID itemUID	= 0; 
    int quantity	= 0;
    int duration	= 0;
    int itemType	= 0;
    int EQUIP_LIMIT = 0;
    KItem*	kInventoryItem;
    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
    {
        // ItemID가 -1이 아닐 때,
        kInventoryItem = NULL;
        int ID = m_ItemSlot[index]->GetItemID();
        if((itemID=static_cast<DWORD>(ID)) != (-1) )
        {
            itemUID	= GetItemUID(index);
            itemType = m_kItemDefine.GetItemKind(itemID);
            if( g_MyD3D->m_kItemMgr.FindInventory(itemUID, &kInventoryItem ) == false )
                continue;
            if( kInventoryItem == NULL )
                continue;          

			EQUIP_LIMIT = 9999;   // 9999개 장착 가능 합니다 

            // 아이템이 사용되었다면,
            quantity = GetItemQuantity(index);
            if( quantity < EQUIP_LIMIT )
            {
                // 인벤토리 창에 남아있는 아이템이 최대 착용가능 아이템 보다 많으면 채운다.
                duration = kInventoryItem->m_nCount ;
                if( (EQUIP_LIMIT-quantity) < duration )
                {
                    duration -= (EQUIP_LIMIT-quantity);
                    quantity = EQUIP_LIMIT;
                }
                // 남아있는 아이템이 하나도 없으면 지운다.
                else if( duration < 1 && quantity < 1)
                {
                    DeleteItembyIndex(index);
                }
                // 남아있는 아이템 수가 최대 착용가능 아이템 보다 적거나 같으면 모두 다 넣는다.
                else
                {
                    quantity += duration;
                    duration = 0;
                }

				if (ID != ITEMID_LISNAR_PRAY)
				{
					kInventoryItem->m_nCount = duration;
				}
            }

			if (ID == ITEMID_LISNAR_PRAY)
			{
				quantity = 1;
			}

            m_ItemSlot[index]->SetQuantity(quantity);
            m_ItemSlot[index]->SetCoolTime(0);		// 사용된 아이템인 경우 CoolTime을 초기화. 왜 여기서 하느냐고는 묻지마셈
        }
    }
    for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
    {
        if( FindEmot(index)->GetItemID() != (-1) )
            continue;
    }

    return true;
}

// Slot의 두 Index를 입력받아 서로 위치를 바꿔준다.
void KGCShortCutSlot::SwapItem( INT &_indexFrom, INT &_indexTo )
{
    swap( m_ItemSlot[_indexFrom], m_ItemSlot[_indexTo] );}

int KGCShortCutSlot::SetDamageOREmoticonAppearanceState(SHORTCUT_ITEM* pItemInfo )
{
    int iRenderDamage = -1;

    if( (0 <= pItemInfo->fDamageOREmoticonAppearanceRate) && (pItemInfo->fDamageOREmoticonAppearanceRate <= 1.f) )
    {
        int iProbDamage = 0;
        int iProbEmoticon = 0;

        if( pItemInfo->fDamageOREmoticonAppearanceRate == 0 )
        {
            iProbDamage = 0;
            iProbEmoticon = 100;
        }    
        else if( pItemInfo->fDamageOREmoticonAppearanceRate == 1 )
        {
            iProbDamage = 100;
            iProbEmoticon = 0;
        }
        else
        {
            //확률을 얻는다.
            iProbDamage = rand() % (int)(pItemInfo->fDamageOREmoticonAppearanceRate * 100);
            iProbEmoticon = rand() % (int)((1.0f - pItemInfo->fDamageOREmoticonAppearanceRate) * 100 );
        }

        //스킬 발동되고 이모티콘은 안나온다.
        if( iProbDamage >= iProbEmoticon )
            iRenderDamage = 0;
        //스킬 발동안되고 이모티콘은 나온다.
        else
            iRenderDamage = 1;
    }
    //이도저도 아니면 둘다 발동
    else
        return iRenderDamage;

    return iRenderDamage;
}
// 해당 슬롯에 있는 Item을 사용한다.
bool KGCShortCutSlot::UseItem( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    // Player 정보 갱신
    SetMyPlayerInfo();

    int iItemID			= m_ItemSlot[_index]->GetItemID();
    GCITEMUID iItemUID		= m_ItemSlot[_index]->GetItemUID();
    int iItemQuantity	= m_ItemSlot[_index]->GetQuantity();
    int iCoolTime		= m_ItemSlot[_index]->GetCoolTime();

    // 수량이 없거나 쿨타임이 적용되고 있는 경우
    if( (iItemQuantity < 1) || (iCoolTime > 0) )
        return false;

	// 아이템에 이모티콘 정보가 0이 아니면, 이모티콘 재생
	SHORTCUT_ITEM* pItemInfo = 0;
	bool bFind = m_kItemDefine.GetStructure( iItemID, pItemInfo );
	
    //iRender 값이  -1 : 둘다 발동,  0 : 스킬만 발동,   1 : 이모티콘만 발동 
    int iRenderDamage = SetDamageOREmoticonAppearanceState(pItemInfo);
    m_ItemSlot[_index]->SetDamageOREmoticonAppearance(iRenderDamage);

    if( pItemInfo->iEmoticonGoodsID && bFind )
	{
		PLAYER* pMyPlayer = g_MyD3D->GetMyPlayer();
		if( !pMyPlayer->GetEmoticon() )
		{
			LTP_EMOTICON packet;
			packet.iCharIndex = g_MyD3D->Get_MyPlayer();
			packet.iItemID = pItemInfo->iEmoticonGoodsID;
		    if( iRenderDamage != 0)
                g_LatencyTransfer.PushPacket( &packet );
		}
	}
    
    // 사용된 아이템의 효과 적용
    m_ItemSlot[_index]->Set();
    if( m_ItemSlot[_index]->Do() == true )
    {
        if( 0 == m_ItemSlot[_index]->GetQuantity() )
        {
            KItem* pInven = NULL;

            if( false == g_pItemMgr->FindInventory( iItemUID, &pInven) || pInven->m_nCount <= 0 )
            {
                g_pItemMgr->m_kInventory.RemoveItem( iItemUID );
            }

            DeleteItembyItemID( iItemID );
        }

        int iItemKind = m_kItemDefine.GetItemKind( iItemID );
        KP2P::GetInstance()->Send_UseQuickSlotReq( SCS_ITEM, _index, iItemID, iItemUID, iItemKind );	// 패킷전송

        // 특정아이템을 사용하면 호위몹도 회복해요.
        if( m_ItemSlot[_index]->GetItemID() == g_kGlobalValue.GetEscortPotionID() ){
            int iHPBefore, iHPAfter;
            float fHPRatio;
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				MONSTER *pMonster = mit->second;
				if ( pMonster->GetMonsterType() != g_kGlobalValue.GetEscortMonsterID() ) continue;
				if ( pMonster->IsDie() == true ) continue;

				iHPBefore = (int)( pMonster->GetHP() );
				iHPAfter = (int)( iHPBefore + pMonster->GetMaxHP() * 0.1f );  // 10% 회복

				if( iHPAfter <= 0) { 
					iHPAfter = 1;
				}

				if ( iHPBefore <= 0 ) 
				{
					fHPRatio = 0.0f;

				}
				else
				{
					fHPRatio = iHPAfter / (float)iHPBefore;
				}

				if( fHPRatio < 0 ) {
					fHPRatio = 0.0f; 
				}

				g_MyD3D->MyPlayer[0]->SetSummonMonsterHP_SendHost( fHPRatio, mit->first, false );

			}
        }
    }
    else
        return false;

    return true;
}

void KGCShortCutSlot::UseEmot( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return;

    if (m_EmotSlot[_index]->GetCoolTime() > 0)
    {
        return;
    }
    m_EmotSlot[_index]->Set();

    if ( !m_EmotSlot[_index]->Do() )
    {
        return;
    }
    

    for ( int i = 0 ; i < EMOT_SLOT_SIZE; i++ )
    {
        m_EmotSlot[i]->SetCoolTime( SECOND( 5 ) );
    }
}

// 해당 Item이 단축슬롯 내에 존재하는지 검사.
bool KGCShortCutSlot::isExist( DWORD _itemID )
{
    // 찾아보고
    if( FindItemIndex(_itemID) != (-1) )
        return true;
    if( FindEmotIndex(_itemID) != (-1) )
        return true;

    // 없으면 false.
    return false;
}
bool KGCShortCutSlot::isProtectRing( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    int iItemID = m_ItemSlot[_index]->GetItemID();

    return g_MyD3D->IsProtectRing( iItemID );
}

// 해당 슬롯의 Item이 가진 ID값을 반환.
INT KGCShortCutSlot::GetItemID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return -1;
    return m_ItemSlot[_index]->GetItemID();
}
// 해당 슬롯의 Item이 가진 UID값을 반환.
GCITEMUID KGCShortCutSlot::GetItemUID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetItemUID();
}
// 해당 슬롯의 Item 수량을 반환.
INT KGCShortCutSlot::GetItemQuantity( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetQuantity();
}
// 해당 슬롯의 Item에 CoolTime이 얼마나 남았는지 반환.
INT KGCShortCutSlot::GetItemCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetCoolTime();
}
// 해당 슬롯의 Item이 CoolTime 상태인지 아닌지를 반환
bool KGCShortCutSlot:: isItemCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    if( m_ItemSlot[_index]->GetCoolTime() == 0 )
        return false;

    return true;
}

// 해당 슬롯의 Item이 가진 ID값을 반환.
INT KGCShortCutSlot::GetEmotID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return -1;
    return m_EmotSlot[_index]->GetItemID();
}
// 해당 슬롯의 Item이 가진 UID값을 반환.
GCITEMUID KGCShortCutSlot::GetEmotUID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetItemUID();
}
// 해당 슬롯의 Item 수량을 반환.
INT KGCShortCutSlot::GetEmotQuantity( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetQuantity();
}
// 해당 슬롯의 Item에 CoolTime이 얼마나 남았는지 반환.
INT KGCShortCutSlot::GetEmotCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetCoolTime();
}
// 해당 슬롯의 Item이 CoolTime 상태인지 아닌지를 반환
bool KGCShortCutSlot:: isEmotCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
            return false;
    if( m_EmotSlot[_index]->GetCoolTime() == 0 )
        return false;

    return true;
}


//////////////////////////////////////////////////////////////////////////
//						Private Member Functions						//
//////////////////////////////////////////////////////////////////////////

// 해당 Index에 있는 Item을 삭제한다.
void KGCShortCutSlot::DeleteItembyIndex( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return;
    // 메모리 해제
    SAFE_DELETE(m_ItemSlot[_index]);
    m_ItemSlot[_index] = new KGCShortCutSlotItem;
}
void KGCShortCutSlot::DeleteEmotbyIndex( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return;
    // 메모리 해제
    SAFE_DELETE(m_EmotSlot[_index]);
    m_EmotSlot[_index] = new KGCShortCutSlotItem;
}

// 해당 ItemID를 가진 Item을 삭제한다.
void KGCShortCutSlot::DeleteItembyItemID( GCITEMID _itemID )
{
    // 메모리 해제
    INT indexTmp = FindItemIndex(_itemID);
    if( indexTmp < 0 || static_cast<int>( m_ItemSlot.size() ) <= indexTmp )
        return;

    SAFE_DELETE(m_ItemSlot[indexTmp]);
    m_ItemSlot[indexTmp] = new KGCShortCutSlotItem;
}
void KGCShortCutSlot::DeleteEmotbyItemID( GCITEMID _itemID )
{
    // 메모리 해제
    INT indexTmp = FindEmotIndex(_itemID);
    if( indexTmp < 0 || static_cast<int>( m_EmotSlot.size() ) <= indexTmp )
        return;

    SAFE_DELETE(m_EmotSlot[indexTmp]);
    m_EmotSlot[indexTmp] = new KGCShortCutSlotItem;
}


// ItemID를 받아 해당 Item을 가지고 있는 Slot을 찾아 해당 Slot의 Index를 반환한다. 실패하면 -1을 반환.
INT KGCShortCutSlot::FindItemIndex( DWORD _itemID )
{
    for( INT i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        if( _itemID == (m_ItemSlot[i]->GetItemID()) )
            return i;
    }

    return -1;
}
INT KGCShortCutSlot::FindEmotIndex( DWORD _itemID )
{
    for( INT i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        if( _itemID == m_EmotSlot[i]->GetItemID() )
            return i;
    }

    return -1;
}
// 이 함수는 Insert 과정에서 같은 Type의 Item을 찾아내기 위해 존재한다.
INT KGCShortCutSlot::FindItemType( INT _itemType )
{
    for( INT index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
    {
        if( m_ItemSlot[index]->GetItemKind() == _itemType )
            return index;
    }

    return (-1);
}
INT KGCShortCutSlot::FindEmotType( INT _itemType )
{
    for( INT index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
    {
        if( m_EmotSlot[index]->GetItemKind() == _itemType )
            return index;
    }

    return (-1);
}

// Slot Index를 받아서 해당 Index의 KGCShortCutSlotItem의 포인터를 반환.
KGCShortCutSlotItem* KGCShortCutSlot::FindItem( INT index )
{
    if( index < 0 || static_cast<int>( m_ItemSlot.size() ) <= index )
        return NULL;

    return m_ItemSlot[index];
}
KGCShortCutSlotItem* KGCShortCutSlot::FindEmot( INT index )
{
    if( index < 0 || static_cast<int>( m_EmotSlot.size() ) <= index )
        return NULL;

    return m_EmotSlot[index];
}

// 종류별로 묶이는 아이템인 경우 같은 종류가 있는지 찾는다.
// 있으면 true를, 없으면 false를 반환한다.
bool KGCShortCutSlot::FindSameItemType( GCITEMID _ItemID )
{
    SCS_ITEM_KIND itemKind = static_cast<SCS_ITEM_KIND>(m_kItemDefine.GetItemKind( _ItemID ));
    switch( itemKind )
    {
        // HP_POTION 계열
        case HP_POTION :
        case MP_POTION :
        case PROTECT_RING :
        case REJUVENATION_POTION:
        case CANCELATION_POTION:
        case BUFF_POTION:
        case SKILL_POTION:
        case SKILL_POTION2:
        case EVENT_POTION:
        case DAMAGE_POTION:
        case SUMMON_POTION:
        { 
            if( FindItemType( itemKind ) != (-1) )
                return true;
            else
                return false;
            break;
        }
        default :
        {
            if( FindItemIndex( _ItemID ) == -1 )
                return false;
            else
                return true;
            break;
        }
    }
}

// Slot에 새로운 Item이 들어갈 수 있는 Index를 반환. 없을 경우 -1.
INT KGCShortCutSlot::CanbeInsertItem( GCITEMID _itemID )
{
    // 해당 Item이 ItemSlot에 존재하지 않고,
    if( FindSameItemType( _itemID ) == false )
    {
        // Item이 들어갈 수 있는 빈 공간이 존재할 때, (해당 Index의 ItemID가 -1 일때 빈 공간)
        for( INT i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
        {
            if( m_ItemSlot[i]->GetItemID() == KItem::INVALID_ITEM )
                return i;
        }
    }

    return -1;
}
INT KGCShortCutSlot::CanbeInsertEmot( GCITEMID _itemID )
{
    // 해당 Emoticon이 EmoticonSlot에 존재하지 않고,
    if( FindEmotIndex( _itemID ) == KItem::INVALID_ITEM )
    {
        // Emoticon이 들어갈 수 있는 빈 공간이 존재할 때,
        for( INT i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
        {
            if( m_EmotSlot[i]->GetItemID() != KItem::INVALID_ITEM )
                return i;
        }
    }

    return KItem::INVALID_ITEM;
}

// Item 삽입 함수
bool KGCShortCutSlot::InsertItem( int _iInsertIndex, SCS_ITEM_KIND _ItemKind, KItem* _kInven )
{
    int LIMIT;

    // 만일 해당 ItemID 정보가 없으면 실패(-1)
    SHORTCUT_ITEM* structure = NULL;
    if( m_kItemDefine.GetStructure( _kInven->m_ItemID, structure ) == false )
        return false;

    LIMIT = 9999;   // 9999개 장착 가능 합니다 

    KItem kInven = (*_kInven);
    // 착용 제한 수량이 넘으면 제한 수량 까지만 넣고
	if (_kInven->m_ItemID == ITEMID_LISNAR_PRAY)
	{
		kInven.m_nCount = 1;
		_kInven->m_nCount -= 1;
	}
    else if( _kInven->m_nCount > LIMIT )
    {
        kInven.m_nCount = LIMIT;
        _kInven->m_nCount -= LIMIT;
    }
    // 제한 수량이 안 넘으면 싸그리 다 때려넣자
    else
    {
        // 수량이 0개이면 장착못한다!!
        if( _kInven->m_nCount > 0 )
        {
            kInven.m_nCount = _kInven->m_nCount;
            _kInven->m_nCount = 0;
        }
    }

    // 이러저러해서 결국은 슬롯에 들어갈 ITEM 생성
    SAFE_DELETE( m_ItemSlot[_iInsertIndex] );
    switch(_ItemKind){
        case HP_POTION :
        case MP_POTION :
        case CANCELATION_POTION:
        case REJUVENATION_POTION:
        case BUFF_POTION:
        case SKILL_POTION:
        case SKILL_POTION2:
        case EVENT_POTION:
        case LV4_SPECIAL_SKILL_POTION:
        case DAMAGE_POTION:
        case SUMMON_POTION:
            m_ItemSlot[_iInsertIndex] = new KPotion_Buff( &kInven, &m_MyPlayer, structure );
            break;
        case PROTECT_RING :
            m_ItemSlot[_iInsertIndex] = new KProtectRing( &kInven, &m_MyPlayer, structure );
            break;
        default:
            break;
    }
    m_ItemSlot[_iInsertIndex]->SetItemKind( _ItemKind );

    return true;
}

// 내 플레이어의 정보를 수집한다.	ex> 레벨, HP, MP, 공격력, 방어력 관련수치
void KGCShortCutSlot::SetMyPlayerInfo()
{
    // 레베루
    m_MyPlayer.iLevel = g_MyD3D->GetMyPlayer()->GetCurrentChar().iLevel;
    // HP
    m_MyPlayer.bIncHP = g_MyD3D->GetMyPlayer()->m_bIncHP;
    m_MyPlayer.fHP_Point = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_HP];
    m_MyPlayer.pfHP = g_MyD3D->GetMyPlayer()->GetHPPtr();
    // MP
    m_MyPlayer.pfMP = &(g_MyD3D->MyHead->Level);
    // 공격력
    m_MyPlayer.pfAtk_Point = &(g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_ATK]);
    // 방어력
    m_MyPlayer.pfDef_Point = &(g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_DEF]);
}

//////////////////////////////////////////////////////////////////////////
// 패킷 보내기 관련함수

// Slot의 정보를 서버로부터 받아와 삽입
void KGCShortCutSlot::initQuickSlot( KQuickSlot _kQuickSlot )
{
    int				iResultIndex = 0;
    GCITEMUID			dwUID = 0;
    KItem*	kInventoryItem = NULL;

    // 받아온 내용을 집어넣자
    m_kQuickSlot = _kQuickSlot;

    std::vector<KSlotData>::iterator it;
    // ItemSlot
    for( it = _kQuickSlot.m_vecItemSlot.begin() ; it != _kQuickSlot.m_vecItemSlot.end() ; it++ )
    {
        dwUID = (*it).m_kItem.m_dwUID;
        if( g_MyD3D->m_kItemMgr.FindInventory( dwUID, &kInventoryItem ) == false )
            continue;
        if( kInventoryItem == NULL )
            continue;

        iResultIndex = InsertItem( kInventoryItem );
        if( -1 != iResultIndex && iResultIndex != (*it).m_nSlotIndex )
            SwapItem( iResultIndex, (*it).m_nSlotIndex );
    }
    // Emoticon Slot
    for( it = _kQuickSlot.m_vecEmoticonSlot.begin() ; it != _kQuickSlot.m_vecEmoticonSlot.end() ; it++ )
    {
        dwUID = (*it).m_kItem.m_dwUID;
        if( g_MyD3D->m_kItemMgr.FindInventory( dwUID, &kInventoryItem ) == false )
            continue;
        if( kInventoryItem == NULL )
            continue;

        iResultIndex = InsertEmot( kInventoryItem );		
    }

    SlotRefill();
}



// SlotData가 변경되었을 때
void KGCShortCutSlot::SendQuickSlotinfo()
{
    bool bSendPack = false;

    int ItemID = 0;
    KSlotData SlotData;		// vector에서 Item 내용을 저장할 구조체

    // ITEM_SLOT에 대한 변경체크 코드. 급해서 무식하게 짰으니 누가 바꿔주실려면 바꿔주센. 기왕이면 단축슬롯 싹 새로 짜주셔도 ㄳ
    INT iQuickSlot[ITEM_SLOT_SIZE] = {-1,};
    for( std::vector<KSlotData>::iterator it = m_kQuickSlot.m_vecItemSlot.begin() ; it != m_kQuickSlot.m_vecItemSlot.end() ; it++ ){
        iQuickSlot[(*it).m_nSlotIndex] = static_cast<INT>((*it).m_kItem.m_dwID/10);
    }
    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ ){
        if( iQuickSlot[index] != m_ItemSlot[index]->GetItemID() )
            bSendPack = true;
    }


    // ITEM_SLOT에 변경된 사항이 있으면
    if( bSendPack == true ){
        std::set<int> setSlotItem;
        // Item Slot
        m_kQuickSlot.m_vecItemSlot.clear();
        setSlotItem.clear();
        for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
        {
            if( (ItemID=m_ItemSlot[index]->GetItemID()) == KItem::INVALID_ITEM ){
                continue;
            }

            if( setSlotItem.find(ItemID) != setSlotItem.end() ){
                continue;
            }

            SlotData.m_nSlotIndex = index;
            SlotData.m_kItem.m_dwID			= static_cast<DWORD>(ItemID) * 10;
            SlotData.m_kItem.m_dwUID		= m_ItemSlot[index]->GetItemUID();

            setSlotItem.insert(ItemID);
            m_kQuickSlot.m_vecItemSlot.push_back(SlotData);
        }
        m_kQuickSlot.m_vecEmoticonSlot.clear();
        setSlotItem.clear();
        for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
        {
            if( (ItemID=m_EmotSlot[index]->GetItemID()) == KItem::INVALID_ITEM ){
                continue;
            }
            if( setSlotItem.find(ItemID) != setSlotItem.end() ){
                continue;
            }

            SlotData.m_nSlotIndex = index;
            SlotData.m_kItem.m_dwID			= static_cast<DWORD>(ItemID) * 10;
            SlotData.m_kItem.m_dwUID		= m_EmotSlot[index]->GetItemUID();

            setSlotItem.insert(ItemID);
            m_kQuickSlot.m_vecEmoticonSlot.push_back(SlotData);
        }
    }
    // Emoticon Slot[ 내용 아무것도 없음. 나중에 ITEM_SLOT이랑 똑같이 하면 될 듯 ]
    

    if( bSendPack == true )
    {
        if( g_MyD3D->GetMyPlayer() != NULL )
        {
            g_MyD3D->GetMyPlayer()->m_kUserInfo.kQuickSlot = m_kQuickSlot;
        }
        g_kGlobalValue.m_kUserInfo.kQuickSlot = m_kQuickSlot;
        KP2P::GetInstance()->Send_ChangeSlotEquipReq(m_kQuickSlot);
    }
}

std::string KGCShortCutSlot::Dump() const
{
    std::stringstream stm;
    stm << "[Shortcutslot Info]" << std::endl;
    for( int i = 0; i < ITEM_SLOT_SIZE; ++i )
    {
        if( m_ItemSlot[i]->GetItemID() == -1 )
            continue;

        //stm << "ItemSlot" << i << " " << m_ItemSlot[i]->Dump() << std::endl;
    }

    for( int i = 0; i < EMOT_SLOT_SIZE; ++i )
    {
        if( m_EmotSlot[i]->GetItemID() == -1 )
            continue;

        //stm << "EmotSlot" << i << " " << m_EmotSlot[i]->Dump() << std::endl;
    }

    return stm.str();
}


void KGCShortCutSlot::FrameMove()
{
    for( int i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        m_ItemSlot[i]->DecreaseCoolTime();        
    }

    for( int i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        m_EmotSlot[i]->DecreaseCoolTime();        
    }
}

// ItemID를 받아 해당 Item을 가지고 있는 Slot을 찾아 해당 Slot의 Index를 반환한다. 실패하면 -1을 반환.
INT KGCShortCutSlot::FindItemIndexByItemUID( GCITEMUID itemUID_ )
{
    for( INT i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        if( itemUID_ == (m_ItemSlot[i]->GetItemUID()) )
            return i;
    }

    return -1;
}
INT KGCShortCutSlot::FindEmotIndexByItemUID( GCITEMUID itemUID_ )
{
    for( INT i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        if( itemUID_ == m_EmotSlot[i]->GetItemUID() )
            return i;
    }

    return -1;
}

// 해당 Item이 단축슬롯 내에 존재하는지 검사.
bool KGCShortCutSlot::isExistByItemUID( GCITEMUID itemUID_ )
{
    // 찾아보고
    if( FindItemIndexByItemUID(itemUID_) != (-1) )
        return true;
    if( FindEmotIndexByItemUID(itemUID_) != (-1) )
        return true;

    // 없으면 false.
    return false;
}

int KGCShortCutSlot::GetCurEquipItemNum(DWORD dwitemID_ )
{
    int iSlotIdx = g_MyD3D->GetMyShortCutSlot()->FindItemIndex(dwitemID_);
    int iShortCutNum = 0;

    if( iSlotIdx != -1 )
        iShortCutNum = g_MyD3D->GetMyShortCutSlot()->FindItem(iSlotIdx)->GetQuantity();

    return iShortCutNum;
}

bool KGCShortCutSlot::IsSpecialSkillItem( GCITEMID dwItemID_ )
{
    SHORTCUT_ITEM* pItemInfo = NULL;
    if( m_kItemDefine.GetStructure( static_cast<int>( dwItemID_ ), pItemInfo ) ) {
        return true;
    } else {
        return false;
    }
}

bool KGCShortCutSlot::IsEquipSpecialSkillItem( int iIndex_ )
{
    if( IsSpecialSkillItem( GetEmotID( iIndex_ ) ) ) {
        if( UseSpecialSkillItem( iIndex_ ) ) {
            return true;
        } 
    }
    return false;
}

int KGCShortCutSlot::InsertSpecialSkill( KItem* kItem_ )
{
    if( kItem_ == NULL ) {
        return -1;
    }

    SHORTCUT_ITEM* pShortcutItem = NULL;
    int iType = -1;

    if( m_kItemDefine.GetStructure( static_cast<int>( kItem_->m_ItemID), pShortcutItem ) ) {
        if( pShortcutItem != NULL ) {
            switch( pShortcutItem->iItemType )
            {
            case LV1_SPECIAL_SKILL_POTION:
                {
                    iType = 0;
                    break;
                }
            case LV2_SPECIAL_SKILL_POTION:
                {
                    iType = 1;
                    break;
                }
            case LV3_SPECIAL_SKILL_POTION:
                {
                    iType = 2;
                    break;
                }
            case LV4_SPECIAL_SKILL_POTION:
                {
                    iType = 0;
                    break;
                }
            default:
                iType = -1;
                break;
            }
        }
    }    

    if( -1 < iType && iType < SPECIAL_SKILL_SIZE ) {
        if ( m_EmotSlot[ iType ]->GetItemID() != KItem::INVALID_ITEM ) {
            g_MyD3D->GetMyShortCutSlot()->DischargeEmot( m_EmotSlot[ iType ]->GetItemID() );
        }

        if ( m_EmotSlot[ iType ]->GetItemID() == KItem::INVALID_ITEM ) {           
            SAFE_DELETE( m_EmotSlot[ iType ] );
            m_EmotSlot[ iType ] = new EmotionItem( kItem_, &m_MyPlayer, NULL );
            return iType;
        }
    }
    return (-1);
}

bool KGCShortCutSlot::UseSpecialSkillItem( INT index )
{
    if( index < 0 || SPECIAL_SKILL_SIZE <= index ) {
        return false;
    }

    if (m_EmotSlot[ index ]->GetCoolTime() > 0) {
        return false;
    }

    SHORTCUT_ITEM* pItemInfo = NULL;
    if( m_kItemDefine.GetStructure( static_cast<int>( m_EmotSlot[ index ]->GetItemID() ), pItemInfo ) ) {
        for ( int i = 0 ; i < SPECIAL_SKILL_SIZE; i++ ) {
            if( IsSpecialSkillItem( m_EmotSlot[ i ]->GetItemID() ) ) {            
                m_EmotSlot[i]->SetCoolTime( SECOND( pItemInfo->iCoolTime ) );
            }        
        }
        return true;
    } 
    
    return false;
}

int KGCShortCutSlot::GetSpecialSkillItemCoolTime( GCITEMID dwItemID_ )
{
    SHORTCUT_ITEM* pItemInfo = NULL;
    if( m_kItemDefine.GetStructure( static_cast<int>( dwItemID_ ), pItemInfo ) ) {
        return pItemInfo->iCoolTime;
    } else {
        return 0;
    }
}

bool KGCShortCutSlot::IsGameModeAllowed( EGCGameMode eGameMode, int index )
{
	if( index < 0 || static_cast<int>( m_ItemSlot.size() ) <= index )
		return false;

    //Luma 가 제작한 슬롯에서 투명 물약 사용을 비활성화합니다.
    if (false == SiKGCWorldMapManager()->IsNotUseShortCutItem(SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetDungeonLevel()))
        return true;

	int iItemID = m_ItemSlot[index]->GetItemID();
	if( iItemID > 0 )
	{
		SHORTCUT_ITEM* pItemInfo = 0;
		if( m_kItemDefine.GetStructure( iItemID, pItemInfo ) )
		{
			if(pItemInfo->m_setGameModes.size() <= 0 || pItemInfo->m_setGameModes.find(eGameMode) == pItemInfo->m_setGameModes.end() )
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool KGCShortCutSlot::HasAllowedItem(EGCGameMode eGameMode)
{
	for(size_t i = 0; i < m_ItemSlot.size(); i++)
	{
		if(IsGameModeAllowed(eGameMode, i) == true )
		{
			return true;
		}
	}
 
	return false;
}

bool KGCShortCutSlot::CanUseItem(int iIndex)
{
	if( iIndex < 0 || static_cast<int>( m_ItemSlot.size() ) <= iIndex )
	{
		return false;
	}

	int iItemID = m_ItemSlot[iIndex]->GetItemID();
	if( iItemID > 0 )
	{
		SHORTCUT_ITEM* pItemInfo = 0;
		if( m_kItemDefine.GetStructure( iItemID, pItemInfo ) )
		{
            //Luma 가 제작한 슬롯에서 투명 물약 사용을 비활성화합니다.
            if (false == g_MyD3D->GetMyShortCutSlot()->IsGameModeAllowed(SiKGCRoomManager()->GetGameMode(), iIndex))
                return false;

			if(pItemInfo->iEquipLimit == -1 || m_ItemSlot[iIndex]->GetUsedTimeCount() < pItemInfo->iEquipLimit )
			{
				return true;
			}
		}
	}

	return false;
}