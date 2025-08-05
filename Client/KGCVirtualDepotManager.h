#pragma once

#include "KNCSingleton.h"

#define MAX_MOVE_INVEN_MAX_SIZE     100

class KGCVirtualDepotManager
{
    DeclareSingleton( KGCVirtualDepotManager );

    enum {
        MAX_VIRTUAL_DEPOT_WIDTH = 5,
        MAX_VIRTUAL_INVEN_WIDTH = 4,
        MAX_VIRTUAL_DEPOT_HEIGHT = 7,
        MAX_VIRTUAL_INVEN_HEIGHT = 7,
    };

    enum VDEPOT_SORT_TYPE
    {
        SORT_TYPE_EQUIP = 0,
        SORT_TYPE_COORDISHOP = 1,
        SORT_TYPE_SELECT_LEVEL_ITEM = 2,
        SORT_TYPE_PET = 3,
        SORT_TYPE_STRONG_ITEM = 4,
        SORT_TYPE_STRONG_ITEM_NOT_STRONG = 5,
        SORT_TYPE_MONSTER_CARD = 6,
        SORT_TYPE_MISSION_SCROLL = 7,
        SORT_SHORTCUT_SLOT = 8,
        SORT_TYPE_TREASUREBOX_ITEM = 9,
        SORT_DEFAULT_ITEM = 10,
    };
public:
    KGCVirtualDepotManager(void);
    ~KGCVirtualDepotManager(void);

public:
    int GetDepotWidthVD()   { return MAX_VIRTUAL_DEPOT_WIDTH; }
    int GetInvenWidthVD()   { return MAX_VIRTUAL_INVEN_WIDTH; }
    int GetDepotHeightVD()  { return MAX_VIRTUAL_DEPOT_HEIGHT; }
    int GetInvenHeightVD()  { return MAX_VIRTUAL_INVEN_HEIGHT; }

    int GetDepotMaxSlotVD()     { return MAX_VIRTUAL_DEPOT_WIDTH * MAX_VIRTUAL_DEPOT_HEIGHT;}
    int GetDepotMaxImageVD()    { return MAX_VIRTUAL_DEPOT_WIDTH * MAX_VIRTUAL_DEPOT_HEIGHT;}
    int GetInvenMaxSlotVD()     { return MAX_VIRTUAL_INVEN_HEIGHT * MAX_VIRTUAL_INVEN_WIDTH; }
    int GetInvenMaxImageVD()    { return MAX_VIRTUAL_INVEN_HEIGHT * MAX_VIRTUAL_INVEN_WIDTH; }

public:

    void SetCurrentChar( int iCharType ) { m_iCurrentChar = iCharType; }
    int GetCurrentChar( void ) { return m_iCurrentChar; }

    void SetDepotItemList( IN KEVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT& kRecv );
    void SetDepotRestrictList( IN KEVENT_DEPOT_RESTRICT_ITEM_NOT& kRecv );
    void SortItemList( void );
    void SetItemCategory( void );
    void SendMoveItemData();

    KPetInfo* GetPetInfo(IN KItem *pItem);
    KItem* CommonFindGetItem( IN const GCITEMUID nItemUID );
    KItem* ExclusiveFindGetItem( IN const GCITEMUID nItemUID );
    std::vector<GCITEMUID> GetDepotCommonItemList();
    std::vector<GCITEMUID> DepotExclusiveItemList();
    bool FindDepotInComplateCommonItem( IN const GCITEMUID nItemUID );

    std::vector< KItem > GetCurrentInvenList( void );
    KItem* InvenFindGetItem( IN const GCITEMUID nItemUID );

    void MoveDepotToInvenItem( KItem kItem, int nCount, bool bCountItem);
    void MoveInvenToDepotCommon( KItem kItem, int nCount, bool bCountItem );
    void MoveInvenToDepotCharItem( KItem kItem, int nCount, bool bCountItem );
    void ClearMyInvenItem( void ) { m_mapMoveMyInvenItemList.clear(); }

    bool IsEmptyItemList( void );
    bool IsEmptyMyInvenItemList( void );

    void MoveDataResualt( IN KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK& kRecv );

    void SetRecvPacket( bool bRecv ) { m_bIsRecvPack = bRecv; }
    bool GetRecvPacket( void ) { return m_bIsRecvPack; }

    void SetCurrentTab( int iTab ) { m_iCurrentTab = iTab; }
    int GetCurrentTab( void ) { return m_iCurrentTab; }

    void SetUpdateUI( bool iUpdate ) { m_bIsUpdateUI = iUpdate; }
    bool GetUpdateUI( void ) { return m_bIsUpdateUI; }
    bool IsExclusiveItem( KItem kItem );
    void ClearDepotData( void ) { m_vecDepotItemList.clear(); }

    void SetFirstLogOn(bool bFirst) { m_bFirstLogOn = bFirst; }
    bool GetFirstLogOn( void ) { return m_bFirstLogOn; }

    void LimitMoveItems( void );
    int InvenLimitSize( void );

    void SetCloseDlg( bool bClose ) { m_bCloseDlg = bClose; }
    bool GetClossDlg( void ) { return m_bCloseDlg; }

    bool IsVirtualDepotItem( IN const KItem kItem );


private:
    std::vector< KItem >                    m_vecDepotItemList;                 // 아이템 리스트 전체
    std::vector< KItem >                    m_vecDepotCommonItemList;           // r공용 아이템.
    std::vector< KItem >                    m_vecDepotInComplateComonItemList;  // 공용 아이템 중 특정 캐릭터에 없는 아이템 ( 빅헤드 )
    std::vector< KItem >                    m_vecDepotExclusiveItemList;        // 아이템 리스트 중 옮길수 없는 아이템 ( 캐릭터 전용 아이템 )
    std::map<int, std::vector< KItem >>     m_mapMoveMyInvenItemList;           // 가상 창고에서 나의 인벤에 옮긴 리스트
    std::set< GCITEMID >                    m_setDpotRestrictItem;              // DepotManager 스크립트에 정의된 아이템들

    

    bool    m_bIsRecvPack;          // 패킷을 받았을때 한번만 가상창고 버튼 갱신을 위해서
    int     m_iCurrentTab;          // 현재 활성화 되어 있는 탭 ( 공용 및 캐릭터 탭 )
    bool    m_bIsUpdateUI;
    int     m_iCurrentChar;

    bool    m_bFirstLogOn;

    bool    m_bCloseDlg;

};


DefSingletonInline( KGCVirtualDepotManager );