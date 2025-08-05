#pragma once

#include "KNCSingleton.h"

class KGCDepotManager
{
    DeclareSingleton( KGCDepotManager );

	enum {
		MAX_DEPOT_WIDTH = 5,
		MAX_INVEN_WIDTH = 4,
		MAX_DEPOT_INVEN_HEIGHT = 7,
	};
public:
    KGCDepotManager(void);
    ~KGCDepotManager(void);

	void OnQuantityDepotMove(GCITEMUID m_ItemUID, int DepotStatus);

    bool IsInsertDepotItem( GCItem* pItem, KItem* pkItem );

	void SetCurrentSafe(int iSafe){m_iCurrentSafe = iSafe;}
	int GetCurrentSafe(){return m_iCurrentSafe;}

	void SetDepotMaxHeight(int iMaxHeight){m_iDepotMaxHeight = iMaxHeight;}
	int GetDepotMaxHeight(){return m_iDepotMaxHeight;}

	void SetDepotMaxSafe(int iMaxSafe){m_iDepotMaxSafe = iMaxSafe;}
	int GetDepotMaxSafe(){return m_iDepotMaxSafe;}

	int GetDepotMaxSlot(){return MAX_DEPOT_WIDTH * m_iDepotMaxHeight;}
	int GetDepotMaxLockSlot(){return MAX_DEPOT_WIDTH * (m_iDepotMaxHeight - 1);}
	int GetInvenMaxSlot(){return MAX_DEPOT_INVEN_HEIGHT * MAX_INVEN_WIDTH;}
	int GetInvenMaxImage(){return ( MAX_DEPOT_INVEN_HEIGHT) * MAX_INVEN_WIDTH;}

	int GetDepotWidth(){return MAX_DEPOT_WIDTH;}
	int GetInvenWidth(){return MAX_INVEN_WIDTH;}
	int GetDepotInvenHeight(){return MAX_DEPOT_INVEN_HEIGHT;}

    int GetSafeOpenSize();
    int GetSlotOpenSize( int iSafe = -1 );

	void SetAllItemInventoryList();
	void SetCharacterDepotInfo(KDepotInfo kRecv);
	void SetDepotCharTabInfo(KEVENT_DEPOT_CHAR_TAB_INFO_ACK kRecv);
	void SetDepotInfo(KEVENT_DEPOT_INFO_ACK kRecv);
    void SetCharacterDepotRecv(KDepotInfo kDepot);


    void Send_DepotInfo();
    void Send_DepotInfoByCharTabReq();

    void SendAddDepotItem(KItem *pItem, int iSlot, int iSafe = -1, const int iAddCount_ = -1 );
    void SendMoveDepotItem(KItem *pItem, int iSlot);
    void SendDelDepotItem(KItem *pItem, const int iDellCount = -1 );

    void SendSafeAdd( );
    void SendSlotAdd( ); 

	void AddDepotItem(KItem kItem, bool DivID = false);
	void AddDepotItemList(std::vector<KItem> vecItem);

	void AddInvenItem(GCITEMUID dwItemUID);
	void DelInvenItem(GCITEMUID dwItemUID, const bool bCountItem_ = false );

    bool InsertDepotItem( KItem *pItem, const int iInsertCount_ = -1 ); //  인벤토리에서 바로 아이템 삽입.
    bool InsertDepotItemSetectSlot( KItem* pItem, const int iInsertCount_ = -1, int iSlot_ = -1 );
    void DelDepotItem(KItem kItem, const bool bDiv_ = true, const bool bCountItem = false );

	KItem* GetDepotItemInfo(GCITEMUID dwItemUID);
    GCItem* GetDepotItemDataByItemID( const GCITEMID itemID_ );
    std::vector<GCITEMUID> GetShowItemInventoryList();
    std::vector<KItem> GetDepotItemList( int iSafe = -1 );

    void ClearRecvItemList();

    bool IsRecvDepotData( int iSafe = -1 );

	bool FindExtendSafeItem( );
	bool FindExtendSlotItem( );
	void BuyItem(GCITEMID buyItemID );

	GCITEMID GetSafeExtendItemID(){return m_iSafeExtendItemID;}
	GCITEMID GetSlotExtendItemID(){return m_iSlotExtendItemID;}

	void SwapPetInfo(KItem kItem, std::map<GCITEMUID, KPetInfo> &delList, std::map<GCITEMUID, KPetInfo> &addlist);
	KPetInfo* GetPetInfo(KItem *pItem);

    std::vector<GCITEMID> CheckItemList(std::vector<GCITEMID> vecItemInfo);
    void Send_DepotInfoByCharTabReqForGacha();
    bool CheckDepotItem(GCITEMID iItemID);
    bool CheckDepotItemForLevel(GCITEMID iItemID , int iItemLevel );
    void Initialize();

private:
	int m_iCurrentCharacter;
	int m_iCurrentSafe;

	int m_iDepotMaxHeight;
	int m_iDepotMaxSafe;
	
	GCITEMID m_iSlotExtendItemID;
	GCITEMID m_iSafeExtendItemID;

    std::vector<GCITEMUID> m_vecAllInventoryItem;    //  아이템UID

    std::map<int,int>   m_mapCharaterDepotInfo;    // 창고탭, 슬롯 사이즈
    std::map<int,bool>  m_mapCharaterDepotRecv;    // 창고탭, 데이터 수신 여부
    std::map<GCITEMUID, KItem> m_mapDepotItemList;  //  창고 전체 아이템 리스트

private:
    int m_iCurrentSelectSlot;
    std::set< GCITEMID > m_setDpotRestrictItem;

public:
    void SetCurrentSelectSlot( const int iSlot_ ) { m_iCurrentSelectSlot = iSlot_; }
    int  GetCurrentSelectSlot( void ) const { return m_iCurrentSelectSlot; }

    void SetDepotRestrictItem( const std::set< GCITEMID >& setList_ );
    bool IsRestrictItem( const GCITEMID itemID );
};

DefSingletonInline( KGCDepotManager );
