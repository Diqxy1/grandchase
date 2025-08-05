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

// Short Cut Slot�� �ʱ�ȭ�Ѵ�.
void KGCShortCutSlot::initialization()
{
    Clear();
    m_kItemDefine.LoadItem();	// stg ���Ϸ� ���� ������ �о�ȴ�.
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

// Item ������ �޾� �� Slot�� �����ϰ� Index�� ��ȯ. �����ϸ� -1�� ��ȯ.
INT KGCShortCutSlot::InsertItem( KItem* _kInven )
{
    INT iInsertIndex = KItem::INVALID_ITEM;
    INT itemID = _kInven->m_ItemID;

    // Insert�� �Ұ����ϸ�  -1 ��ȯ
    if( ( iInsertIndex = CanbeInsertItem(itemID) ) == (-1) )
    {	return (-1);   }
    else
    {
        // �޾ƿ� ItemID��
        // �� Item�� �´� �����Լ� ȣ��
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

// ���â���� �κ��丮�� �������� ���� �κ��丮�� ������ �����ش�.
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

        // �� �� ������
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

    //// �� �� ������
    //ItemInfo.m_dwGoodsUID	=  m_EmotSlot[index]->GetItemUID() ;
    //ItemInfo.m_dwGoodsID	= m_EmotSlot[index]->GetItemID()*10;
    //ItemInfo.m_dwCurrentNum	= m_EmotSlot[index]->GetQuantity();
    //vecItemInfo.push_back(ItemInfo);
    //g_MyD3D->m_kItemMgr.AddInventory(vecItemInfo);     

    DeleteEmotbyIndex( index );
}

// �Էµ� Item�� ���� Type�� Item�� DisCharge ��Ű�� �Էµ� Item�� Insert��Ų��
bool KGCShortCutSlot::ExchangeItem( IN KItem* kInven_ )
{
    int iSrcID = kInven_->m_ItemID;

    // �Էµ� �������� � �迭�ΰ� �˾ƺ���
    int iItemType = m_kItemDefine.GetItemKind(static_cast<DWORD>(iSrcID));
    if( iItemType == (-1) )
        return false;

    // �� �迭�� �������� ������ ������ Index�� �˾ƿ� ��,
    int iIndex = FindItemType(iItemType);
    if(iIndex == (-1) )
        return false;

    // �ش� Item�� �����Ѵ�.
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
        // ItemID�� -1�� �ƴ� ��,
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

			EQUIP_LIMIT = 9999;   // 9999�� ���� ���� �մϴ� 

            // �������� ���Ǿ��ٸ�,
            quantity = GetItemQuantity(index);
            if( quantity < EQUIP_LIMIT )
            {
                // �κ��丮 â�� �����ִ� �������� �ִ� ���밡�� ������ ���� ������ ä���.
                duration = kInventoryItem->m_nCount ;
                if( (EQUIP_LIMIT-quantity) < duration )
                {
                    duration -= (EQUIP_LIMIT-quantity);
                    quantity = EQUIP_LIMIT;
                }
                // �����ִ� �������� �ϳ��� ������ �����.
                else if( duration < 1 && quantity < 1)
                {
                    DeleteItembyIndex(index);
                }
                // �����ִ� ������ ���� �ִ� ���밡�� ������ ���� ���ų� ������ ��� �� �ִ´�.
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
            m_ItemSlot[index]->SetCoolTime(0);		// ���� �������� ��� CoolTime�� �ʱ�ȭ. �� ���⼭ �ϴ��İ�� ��������
        }
    }
    for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
    {
        if( FindEmot(index)->GetItemID() != (-1) )
            continue;
    }

    return true;
}

// Slot�� �� Index�� �Է¹޾� ���� ��ġ�� �ٲ��ش�.
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
            //Ȯ���� ��´�.
            iProbDamage = rand() % (int)(pItemInfo->fDamageOREmoticonAppearanceRate * 100);
            iProbEmoticon = rand() % (int)((1.0f - pItemInfo->fDamageOREmoticonAppearanceRate) * 100 );
        }

        //��ų �ߵ��ǰ� �̸�Ƽ���� �ȳ��´�.
        if( iProbDamage >= iProbEmoticon )
            iRenderDamage = 0;
        //��ų �ߵ��ȵǰ� �̸�Ƽ���� ���´�.
        else
            iRenderDamage = 1;
    }
    //�̵����� �ƴϸ� �Ѵ� �ߵ�
    else
        return iRenderDamage;

    return iRenderDamage;
}
// �ش� ���Կ� �ִ� Item�� ����Ѵ�.
bool KGCShortCutSlot::UseItem( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    // Player ���� ����
    SetMyPlayerInfo();

    int iItemID			= m_ItemSlot[_index]->GetItemID();
    GCITEMUID iItemUID		= m_ItemSlot[_index]->GetItemUID();
    int iItemQuantity	= m_ItemSlot[_index]->GetQuantity();
    int iCoolTime		= m_ItemSlot[_index]->GetCoolTime();

    // ������ ���ų� ��Ÿ���� ����ǰ� �ִ� ���
    if( (iItemQuantity < 1) || (iCoolTime > 0) )
        return false;

	// �����ۿ� �̸�Ƽ�� ������ 0�� �ƴϸ�, �̸�Ƽ�� ���
	SHORTCUT_ITEM* pItemInfo = 0;
	bool bFind = m_kItemDefine.GetStructure( iItemID, pItemInfo );
	
    //iRender ����  -1 : �Ѵ� �ߵ�,  0 : ��ų�� �ߵ�,   1 : �̸�Ƽ�ܸ� �ߵ� 
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
    
    // ���� �������� ȿ�� ����
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
        KP2P::GetInstance()->Send_UseQuickSlotReq( SCS_ITEM, _index, iItemID, iItemUID, iItemKind );	// ��Ŷ����

        // Ư���������� ����ϸ� ȣ������ ȸ���ؿ�.
        if( m_ItemSlot[_index]->GetItemID() == g_kGlobalValue.GetEscortPotionID() ){
            int iHPBefore, iHPAfter;
            float fHPRatio;
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				MONSTER *pMonster = mit->second;
				if ( pMonster->GetMonsterType() != g_kGlobalValue.GetEscortMonsterID() ) continue;
				if ( pMonster->IsDie() == true ) continue;

				iHPBefore = (int)( pMonster->GetHP() );
				iHPAfter = (int)( iHPBefore + pMonster->GetMaxHP() * 0.1f );  // 10% ȸ��

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

// �ش� Item�� ���ཽ�� ���� �����ϴ��� �˻�.
bool KGCShortCutSlot::isExist( DWORD _itemID )
{
    // ã�ƺ���
    if( FindItemIndex(_itemID) != (-1) )
        return true;
    if( FindEmotIndex(_itemID) != (-1) )
        return true;

    // ������ false.
    return false;
}
bool KGCShortCutSlot::isProtectRing( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    int iItemID = m_ItemSlot[_index]->GetItemID();

    return g_MyD3D->IsProtectRing( iItemID );
}

// �ش� ������ Item�� ���� ID���� ��ȯ.
INT KGCShortCutSlot::GetItemID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return -1;
    return m_ItemSlot[_index]->GetItemID();
}
// �ش� ������ Item�� ���� UID���� ��ȯ.
GCITEMUID KGCShortCutSlot::GetItemUID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetItemUID();
}
// �ش� ������ Item ������ ��ȯ.
INT KGCShortCutSlot::GetItemQuantity( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetQuantity();
}
// �ش� ������ Item�� CoolTime�� �󸶳� ���Ҵ��� ��ȯ.
INT KGCShortCutSlot::GetItemCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return 0;
    return m_ItemSlot[_index]->GetCoolTime();
}
// �ش� ������ Item�� CoolTime �������� �ƴ����� ��ȯ
bool KGCShortCutSlot:: isItemCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return false;

    if( m_ItemSlot[_index]->GetCoolTime() == 0 )
        return false;

    return true;
}

// �ش� ������ Item�� ���� ID���� ��ȯ.
INT KGCShortCutSlot::GetEmotID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return -1;
    return m_EmotSlot[_index]->GetItemID();
}
// �ش� ������ Item�� ���� UID���� ��ȯ.
GCITEMUID KGCShortCutSlot::GetEmotUID( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetItemUID();
}
// �ش� ������ Item ������ ��ȯ.
INT KGCShortCutSlot::GetEmotQuantity( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetQuantity();
}
// �ش� ������ Item�� CoolTime�� �󸶳� ���Ҵ��� ��ȯ.
INT KGCShortCutSlot::GetEmotCoolTime( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return 0;
    return m_EmotSlot[_index]->GetCoolTime();
}
// �ش� ������ Item�� CoolTime �������� �ƴ����� ��ȯ
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

// �ش� Index�� �ִ� Item�� �����Ѵ�.
void KGCShortCutSlot::DeleteItembyIndex( INT _index )
{
    if( _index < 0 || static_cast<int>( m_ItemSlot.size() ) <= _index )
        return;
    // �޸� ����
    SAFE_DELETE(m_ItemSlot[_index]);
    m_ItemSlot[_index] = new KGCShortCutSlotItem;
}
void KGCShortCutSlot::DeleteEmotbyIndex( INT _index )
{
    if( _index < 0 || static_cast<int>( m_EmotSlot.size() ) <= _index )
        return;
    // �޸� ����
    SAFE_DELETE(m_EmotSlot[_index]);
    m_EmotSlot[_index] = new KGCShortCutSlotItem;
}

// �ش� ItemID�� ���� Item�� �����Ѵ�.
void KGCShortCutSlot::DeleteItembyItemID( GCITEMID _itemID )
{
    // �޸� ����
    INT indexTmp = FindItemIndex(_itemID);
    if( indexTmp < 0 || static_cast<int>( m_ItemSlot.size() ) <= indexTmp )
        return;

    SAFE_DELETE(m_ItemSlot[indexTmp]);
    m_ItemSlot[indexTmp] = new KGCShortCutSlotItem;
}
void KGCShortCutSlot::DeleteEmotbyItemID( GCITEMID _itemID )
{
    // �޸� ����
    INT indexTmp = FindEmotIndex(_itemID);
    if( indexTmp < 0 || static_cast<int>( m_EmotSlot.size() ) <= indexTmp )
        return;

    SAFE_DELETE(m_EmotSlot[indexTmp]);
    m_EmotSlot[indexTmp] = new KGCShortCutSlotItem;
}


// ItemID�� �޾� �ش� Item�� ������ �ִ� Slot�� ã�� �ش� Slot�� Index�� ��ȯ�Ѵ�. �����ϸ� -1�� ��ȯ.
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
// �� �Լ��� Insert �������� ���� Type�� Item�� ã�Ƴ��� ���� �����Ѵ�.
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

// Slot Index�� �޾Ƽ� �ش� Index�� KGCShortCutSlotItem�� �����͸� ��ȯ.
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

// �������� ���̴� �������� ��� ���� ������ �ִ��� ã�´�.
// ������ true��, ������ false�� ��ȯ�Ѵ�.
bool KGCShortCutSlot::FindSameItemType( GCITEMID _ItemID )
{
    SCS_ITEM_KIND itemKind = static_cast<SCS_ITEM_KIND>(m_kItemDefine.GetItemKind( _ItemID ));
    switch( itemKind )
    {
        // HP_POTION �迭
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

// Slot�� ���ο� Item�� �� �� �ִ� Index�� ��ȯ. ���� ��� -1.
INT KGCShortCutSlot::CanbeInsertItem( GCITEMID _itemID )
{
    // �ش� Item�� ItemSlot�� �������� �ʰ�,
    if( FindSameItemType( _itemID ) == false )
    {
        // Item�� �� �� �ִ� �� ������ ������ ��, (�ش� Index�� ItemID�� -1 �϶� �� ����)
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
    // �ش� Emoticon�� EmoticonSlot�� �������� �ʰ�,
    if( FindEmotIndex( _itemID ) == KItem::INVALID_ITEM )
    {
        // Emoticon�� �� �� �ִ� �� ������ ������ ��,
        for( INT i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
        {
            if( m_EmotSlot[i]->GetItemID() != KItem::INVALID_ITEM )
                return i;
        }
    }

    return KItem::INVALID_ITEM;
}

// Item ���� �Լ�
bool KGCShortCutSlot::InsertItem( int _iInsertIndex, SCS_ITEM_KIND _ItemKind, KItem* _kInven )
{
    int LIMIT;

    // ���� �ش� ItemID ������ ������ ����(-1)
    SHORTCUT_ITEM* structure = NULL;
    if( m_kItemDefine.GetStructure( _kInven->m_ItemID, structure ) == false )
        return false;

    LIMIT = 9999;   // 9999�� ���� ���� �մϴ� 

    KItem kInven = (*_kInven);
    // ���� ���� ������ ������ ���� ���� ������ �ְ�
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
    // ���� ������ �� ������ �α׸� �� ��������
    else
    {
        // ������ 0���̸� �������Ѵ�!!
        if( _kInven->m_nCount > 0 )
        {
            kInven.m_nCount = _kInven->m_nCount;
            _kInven->m_nCount = 0;
        }
    }

    // �̷������ؼ� �ᱹ�� ���Կ� �� ITEM ����
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

// �� �÷��̾��� ������ �����Ѵ�.	ex> ����, HP, MP, ���ݷ�, ���� ���ü�ġ
void KGCShortCutSlot::SetMyPlayerInfo()
{
    // ������
    m_MyPlayer.iLevel = g_MyD3D->GetMyPlayer()->GetCurrentChar().iLevel;
    // HP
    m_MyPlayer.bIncHP = g_MyD3D->GetMyPlayer()->m_bIncHP;
    m_MyPlayer.fHP_Point = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_HP];
    m_MyPlayer.pfHP = g_MyD3D->GetMyPlayer()->GetHPPtr();
    // MP
    m_MyPlayer.pfMP = &(g_MyD3D->MyHead->Level);
    // ���ݷ�
    m_MyPlayer.pfAtk_Point = &(g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_ATK]);
    // ����
    m_MyPlayer.pfDef_Point = &(g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_DEF]);
}

//////////////////////////////////////////////////////////////////////////
// ��Ŷ ������ �����Լ�

// Slot�� ������ �����κ��� �޾ƿ� ����
void KGCShortCutSlot::initQuickSlot( KQuickSlot _kQuickSlot )
{
    int				iResultIndex = 0;
    GCITEMUID			dwUID = 0;
    KItem*	kInventoryItem = NULL;

    // �޾ƿ� ������ �������
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



// SlotData�� ����Ǿ��� ��
void KGCShortCutSlot::SendQuickSlotinfo()
{
    bool bSendPack = false;

    int ItemID = 0;
    KSlotData SlotData;		// vector���� Item ������ ������ ����ü

    // ITEM_SLOT�� ���� ����üũ �ڵ�. ���ؼ� �����ϰ� ®���� ���� �ٲ��ֽǷ��� �ٲ��ּ�. ����̸� ���ཽ�� �� ���� ¥�ּŵ� ��
    INT iQuickSlot[ITEM_SLOT_SIZE] = {-1,};
    for( std::vector<KSlotData>::iterator it = m_kQuickSlot.m_vecItemSlot.begin() ; it != m_kQuickSlot.m_vecItemSlot.end() ; it++ ){
        iQuickSlot[(*it).m_nSlotIndex] = static_cast<INT>((*it).m_kItem.m_dwID/10);
    }
    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ ){
        if( iQuickSlot[index] != m_ItemSlot[index]->GetItemID() )
            bSendPack = true;
    }


    // ITEM_SLOT�� ����� ������ ������
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
    // Emoticon Slot[ ���� �ƹ��͵� ����. ���߿� ITEM_SLOT�̶� �Ȱ��� �ϸ� �� �� ]
    

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

// ItemID�� �޾� �ش� Item�� ������ �ִ� Slot�� ã�� �ش� Slot�� Index�� ��ȯ�Ѵ�. �����ϸ� -1�� ��ȯ.
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

// �ش� Item�� ���ཽ�� ���� �����ϴ��� �˻�.
bool KGCShortCutSlot::isExistByItemUID( GCITEMUID itemUID_ )
{
    // ã�ƺ���
    if( FindItemIndexByItemUID(itemUID_) != (-1) )
        return true;
    if( FindEmotIndexByItemUID(itemUID_) != (-1) )
        return true;

    // ������ false.
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

    //Luma �� ������ ���Կ��� ���� ���� ����� ��Ȱ��ȭ�մϴ�.
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
            //Luma �� ������ ���Կ��� ���� ���� ����� ��Ȱ��ȭ�մϴ�.
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