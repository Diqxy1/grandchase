#pragma once
#include <boost/tuple/tuple.hpp>
#include <map>
#include <vector>
#include <set>
#include "PrimitiveTypedef.h"
#include "UserPacket.h"
#include "CommonSP.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

// 전방 선언
class KODBC;
struct KItem;
struct KGiftInfo;
struct KBuySellItemReq;
struct KCharacterInfo;
struct KHeroPlayData;
class KSP
{
private:
    KODBC& m_kODBC;
    KSP(void);
public:
    KSP( KODBC& kODBC ) : m_kODBC(kODBC), m_kCommonSP(kODBC) {};
    ~KSP(void);

public:
    int CheckSellMoneyType( IN const GCITEMID itemID );
    //bool GetPetSupplyItems( IN const DWORD dwUID, IN const DWORD dwPetID, IN const int nPromotion, OUT std::vector<KItem>& vecItems );
    bool GetPrice( IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod, OUT int& nPrice, OUT int& nMoneyType );
    bool UpdateAttribute( IN const DWORD dwUserUID, IN const int nSlotID, IN const int nAttType, IN const float fAttValue, IN OUT KItem& kItem, IN const char cCharType_ );
    bool InitAttribute( IN const DWORD dwUserUID, IN const int nSlotID, IN OUT KItem& kItem ); // 속성초기화.
    bool UpdateSocket( IN const DWORD dwUserUID, IN const int nSlotID, IN const GCITEMID CardID, IN const int nSocState, IN OUT KItem& kItem, IN const char cCharType_ );
    bool GetWaitItemFullList( IN const DWORD dwUserUID_, IN const char cCharType_, OUT std::vector< boost::tuple<__int64,KItem,int> >& vecItems_ );
    bool GetWaitItemList( IN const DWORD dwUserUID, IN const char cCharType_, IN const int nWaitType, OUT std::vector<std::pair<__int64,KItem>>& vecItems );
    bool GetGiftInfoList( IN const DWORD dwUserUID, IN const char cCharType_, std::vector<KGiftInfo>& vecGiftInfo );
    bool InsertItemToInven( IN const DWORD dwUID, IN const char cCharType_, IN const __int64& WUID, IN const int nGetType, IN OUT KItem& kItem );
    bool InsertItemToInvenForInsertDepot( IN const DWORD dwUID_, IN const char cCharType_, IN const __int64& WUID_, IN const int nGetType_, IN OUT KItem& kItem_ );

    bool InsertWaitItemToInven( IN const DWORD dwUserUID, IN const char cCharType_, IN const int nGetType, IN OUT std::vector<KItem>& vecItems );
    bool InsertSingleWaitItemToInvenOrVDepot( IN const DWORD dwUserUID, IN const char cCharType_, IN const __int64 itemWUID, IN const int nGetType, IN OUT std::vector<KItem>& vecItems );
    bool InsertSingleWaitItemToInvenForDepot( IN const DWORD dwUserUID, IN const char cCharType_, IN const __int64 itemWUID, IN const int nGetType, IN OUT std::vector<KItem>& vecItems );
    bool InsertPetItem( IN const DWORD dwUserUID, IN const char cCharType_, IN const int nPetID, IN const int nPromotion, IN OUT std::vector<KItem>& vecItems );
    bool RemovePet( IN const DWORD dwUID, IN const DWORD dwPetID, IN const GCITEMUID dwPetUID );
    int BuyItemToWait( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const KBuySellItemReq& kBuyInfo, IN const std::wstring& strComment, OUT __int64& waitUID, OUT std::wstring& wstrItemName );
    int BuyItemToWait( IN const DWORD dwUserUID, IN const std::wstring& strBuyerLogin, IN const GCITEMID itemID,
        IN const int nCount, IN const int nPeriod, IN const int nMoneyType, IN const int nPrice,
        IN const int nCharType, IN const int nCharLevel, IN const std::wstring& strComment, IN const int nGrade, OUT __int64& waitUID, OUT std::wstring& wstrItemName );

    int BuyItemToWait_Mixed( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const KBuySellItemReq& kBuyInfo,
                             IN const int nMoneyType2, IN const int nPrice2, IN const std::wstring& strComment, OUT __int64& waitUID );
    int BuyItemToWait_Mixed( IN const DWORD dwUserUID, IN const std::wstring& strBuyerLogin, IN const GCITEMID itemID, 
                                   IN const int nGrade, IN const int nCount, IN const int nPeriod, IN const int nMoneyType, IN const int nPrice,
                                   IN const int nCharType, IN const int nCharLevel, IN const int nMoneyType2, IN const int nPrice2,
                                   IN const std::wstring& strComment, OUT __int64& waitUID_ );

    bool RollbackBuyItem( IN const __int64& waitItemUID );
    bool UpdateWaitItemToEnabled( IN const __int64 waitItemUID );
    bool UpdateWaitItemToDelete( IN const __int64 waitItemUID );
    bool GetWaitInnerItemList( IN const DWORD dwUserUID, IN const char cCharType_, IN const __int64 waitItemUID, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems );
    bool SellItem( IN const DWORD dwUID_, IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, OUT int& nFullGP_, OUT __int64& waitUID_, IN const char cCharType_ ); // 아이템 팔기.
    bool DivideSellItem( IN const DWORD dwUID, IN const GCITEMUID itemUID, IN const GCITEMID itemID, IN const int nFactor, OUT int& nFullGP, OUT __int64& nWUID, IN const char cCharType_ ); // 수량 나눠 팔기.
    bool InsertFullWaitItemToInven( IN const DWORD dwUserUID, IN const char cCharType_, IN OUT std::vector<KItem>& vecItems );

    bool GetGCPointPriceAndInfo( IN const GCITEMID ItemID, OUT int& nPeriod, OUT int& nDuration, OUT int& nPrice );
    bool GetGCPointPriceAndInfo_CashCheck( IN const GCITEMID ItemID, IN const bool& bIsCashItem, OUT int& nPeriod, OUT int& nDuration, OUT int& nPrice );
    bool IsGCPointItem( IN const GCITEMID ItemID_ );
    bool IsGCPointItem_CashCheck( IN const GCITEMID ItemID );

    int GetBreakupReelPoint( IN const DWORD dwLoginUID );
    bool SetBreakupReelPoint( IN const DWORD dwLoginUID, IN const int nReelCount );
    bool EraseItemByBreakup( IN const DWORD dwLoginUID, IN const char cCharType_, IN const GCITEMUID itemUID );
    void GetItemBreakupExceptionList( OUT std::set<GCITEMID>& setExceptionList );
    bool InsertCashWaitItemToInven( IN const DWORD dwUserUID, IN const char cCharType_, IN const std::wstring& strLogin_, IN OUT KEVENT_GET_WEB_CASH_ITEM_ACK& kPacket_ );
    bool StatrecordResolution( IN const DWORD dwLoginUID, IN const int nWidth, IN const int nHeight, IN const int nMaxWidth, IN const int nMaxHeight );
    bool LowLevelCharEquip( IN const DWORD dwLoginUID );
    // 해당 함수는 대기 아이템으로 집어 넣고 난 다음에 해당 UID를 그대로 사용해서 하는 함수이다.
    bool InsertSingleItemJob( IN const DWORD dwLoginUID, IN const char cCharType_, IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod, IN const int nGetType, IN OUT std::vector<KItem>& vecItems, IN int nGrade = -1, IN int nItemLevel = 0, IN int nEnchantLevel = 0, IN int nStatus = 2 );
    bool InsertSingleItemJob_WaitUID( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const GCITEMID itemID_,
        IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_, OUT __int64& waitUID_, IN int nGrade_ = -1, IN int nItemLevel_ = 0, IN int nEnchantLevel_ = 0, IN int nStatus_ = 2 ); // out 인자로 wuid 추가
    bool InsertSingleItemJobForDepot( IN const DWORD dwLoginUID, IN const char cCharType_, IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod, IN const int nGetType, IN OUT std::vector<KItem>& vecItems, IN int nGrade = -1, IN int nItemLevel = 0, IN int nEnchantLevel = 0, IN int nStatus = 2 );
    
    bool GetUserHeroDungeonInfo( IN const DWORD dwUserUID, IN const char cCharType_, IN OUT std::map< int, KHeroPlayData >& mapData );
    int UpdateUserHeroDungeonInfo( IN const DWORD dwUserUID, IN const char cCharType_, IN const int nModeID, IN const int nPlayIndex, IN const int nClearCount );

    bool GetCharacterInfo_1( IN const DWORD dwLoginUID_, IN const char cCharType_, OUT KCharacterInfo& kCharInfo_ );

    // kkurrung 룩 변환.
    bool NormalItemToCoordiItem( IN const DWORD dwLoginUID, IN const GCITEMUID itemUID, IN const char cCharType, OUT int& nOK );
    bool CoordiItemToNormalItem( IN const DWORD dwLoginUID_, IN const GCITEMUID itemUID_, IN const char cCharType_ );
    bool UIGAUserItemCoordi_select( IN const DWORD dwLoginUID_, OUT std::vector<GCITEMUID>& vecLookUIDs_, IN const char cCharType_ );
    bool UIGAUserItemCoordi_select_20130415( IN const DWORD dwLoginUID_, IN const char cCharType_, OUT std::vector<KItem>& vecLookItem_ );
    bool UIGAUserItemCoordiEquip_select( IN const DWORD dwLoginUID, OUT std::vector< boost::tuple<char, int, GCITEMUID > >& vecLookEquips );
    bool SetUIGACharItemCoordiEquip( IN const DWORD dwLoginUID_, IN const std::vector<KItem>& vecInven, IN OUT std::map<char,KCharacterInfo>& mapCharacterInfo, IN std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi );
    bool UIGAUserItemCoordiName_select( IN const DWORD dwLoginUID_, OUT std::vector< boost::tuple<char, int, std::wstring> >& vecCoordiName_ ); // < charType, Coordi ID, Name >
    bool UIGAUserItemCoordiEquip_delete( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const GCITEMUID itemUID_ );
    bool UIGAUserItemCoordiEquip_insert( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const GCITEMUID itemUID_ );
    bool UIGAUserItemCoordiName_merge( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const std::wstring& strCoordiName_ );
    bool UIGAUserItemCoordi_merge( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const KItem& kItem_ );


    bool GetChangeWeaponInfoFromDB( IN const DWORD& dwUserUID, OUT std::map< char, std::pair<bool,GCITEMUID> >& mapChangeWeapon );
    bool UpdateChangeWeaponInfoToDB( IN const DWORD& dwUserUID, IN const int& nCharType, IN const bool& bOpen, IN const GCITEMUID& ItemUID );

    bool QueryPCBangUser( IN const std::wstring& strIP_, IN KODBC& kODBC_ );
    
    void GetConnectionMsgSelect( IN const int msgID_, OUT std::wstring& msg_ );
    void GetConnectionMsgUserSelect( IN const DWORD dwUID_, OUT std::vector<int>& vecMsgID_ );
    int UpdateUserNickname( IN const DWORD& dwUID, IN const std::wstring& strOldNick, IN const std::wstring& strNewNick_, IN const GCITEMUID& ItemUID );
    int UpdateUserNickcolor( IN const DWORD& dwUID, IN const std::wstring& strNewColor_, IN const GCITEMUID& ItemUID );
    int UpdateUserNicknameNoItem( IN const DWORD& dwUID, IN const std::wstring& strOldNick, IN const std::wstring& strNewNick );
    int UpdateUserNickcolorNoItem( IN const DWORD& dwUID, IN const std::wstring& strNewColor );
    bool LoginToUserUID( IN const std::wstring& strLogin, OUT DWORD& dwLoginUID );
    bool NickToUserUID( IN const std::wstring& strNick, OUT DWORD& dwLoginUID );
    bool CharNickToUserUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ );
    bool GetFashionCatalog( OUT std::map< int, std::pair<int,GCITEMID> >& mapItemList );
    bool FriendFriendUserType_select( IN const DWORD& dwUserUID, IN OUT int& nUserType, IN const int nVersion );
    bool SkillCount_merge( IN const std::map< std::pair<int,int>, std::map<int,int> >& mapStat_ );
    bool SkillTreeID_insert( IN const DWORD& dwUserUID, IN const int& nCharType, IN const int& nPromotion, IN const int& nSkillID );
    bool SkillTreeID_delete( IN const DWORD& dwUserUID, IN const int& nCharType, IN const int& nPromotion, IN const int& nSkillID );

    bool PetInfoEquip_insert( IN const DWORD& dwUserUID, IN const char& cCharType, IN const GCITEMUID& dwPetUID, IN const GCITEMUID& ItemUID );
    bool PetInfoEquip_delete( IN const DWORD& dwUserUID, IN const char& cCharType, IN const GCITEMUID& dwPetUID, IN const GCITEMUID& ItemUID );

    bool UpdateLevel( IN const DWORD dwUserUID, IN const int nLevel, IN const int nType, IN OUT KItem& kItem, IN const char cCharType_ );
    bool PackageItem_select( IN const DWORD dwMaxPackageNum, OUT std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList );
    bool ChoiceDrop_insert( IN const DWORD& dwUserUID, IN const GCITEMID& BoxItemID, IN const GCITEMID& SelectItemID );
    bool QueryUserBenfitCheck( IN const std::wstring& strIP, IN KODBC& kODBC );
    bool UpdateUserModeCount( IN int nModeCode_, IN int nModeCount_, IN KODBC& kODBC_ );

    bool VIPCustomerInfo_select( OUT std::map<USHORT,KVipEvent>& mapInfo, IN std::map< USHORT, std::map< USHORT, USHORT > >& mapRewardByVer_ );
    bool VIPCustomerPoint_select( IN const DWORD& dwUserUID, IN const USHORT& usVer, OUT int& nPoint );
    bool VIPCustomerReward_select( IN const DWORD& dwUserUID, IN const USHORT& usVer, OUT std::map<int,bool>& mapInfo );
    bool VIPCustomerReward_update( IN const DWORD& dwUserUID, IN const USHORT& usVer, IN const int& nPayTableID );

    bool MagicBox_select( IN const USHORT& usVer, OUT std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapItemList, OUT std::map<GCITEMID,float>& mapLuckRatio );
    bool MagicBoxRegister_update( IN const DWORD& dwUserUID, IN const USHORT& usVer, IN const GCITEMID& ItemID );
    bool MagicBoxRegister_select( IN const DWORD& dwUserUID, IN const USHORT& usVer, OUT GCITEMID& ItemID );
    bool MagicBoxStoneInfo_update( IN const DWORD& dwUserUID, IN const USHORT& usVer, IN const GCITEMID& ItemID, IN const int& nUseKeyDiff, IN const bool& bClear );
    bool MagicBoxStoneInfo_select( IN const DWORD& dwUserUID, IN const USHORT& usVer, OUT std::map<GCITEMID,std::pair<int,int> >& mapList );

    bool MergeAttribute( IN const int nType, IN const DWORD dwUserUID, IN const int nSlotID, IN const int nAttType, IN const float fAttValue, IN OUT KItem& kItem, IN const char cCharType_ );

    int UpdateStrengthItemInfo( IN const DWORD dwUID, IN const GCITEMUID dwStrengthItemUID, IN const int nLevel, IN const GCITEMUID dwEquipItemUID, IN const char cGrade, IN const char cCharType_ );
    KCommonSP& GetCommonSP() { return m_kCommonSP; };

    // 강화석 마이그레이션
    bool UserItemStrength_migration011( IN const DWORD& dwUID, IN const int& nNO, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration012( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration099( IN const DWORD& dwUID, IN const GCITEMUID& ItemUID, IN const int& nEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration021( IN const DWORD& dwUID, IN const int& nNO, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration022( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration031( IN const DWORD& dwUID, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration032( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration041( IN const DWORD& dwUID, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration042( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration051( IN const DWORD& dwUID, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration052( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration061( IN const DWORD& dwUID, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );
    bool UserItemStrength_migration062( IN const DWORD& dwUID, IN const char cCharType_, IN const int& nEnchantLv );
    bool UserItemStrength_migration071( IN const DWORD& dwUID, IN const int& nNO, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv, IN const char cCharType_ );

    bool AgiteObjPos_merge( IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nPosX, IN const int& nPosY );
    bool AgiteObjPos_select( IN const DWORD& dwUserUID, OUT std::vector<KAgitObjectPos>& vecObjectPos );
    bool AgitHelper_merge( IN const DWORD& dwUserUID, IN const KBrownieInfo& kInfo );

    bool UpdateItemGrade( IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nGrade ); // 아이템 등급 변경
    // 우편
    int Post_insert( IN const DWORD& dwToUserUID, IN const char& cCharType, IN const char& cScriptType, IN const DWORD& dwFromUserUID, IN const std::wstring& strTitle, 
                    IN const std::wstring& strMsg, OUT POSTUID& PostUID_, IN int nGP = 0, IN int nStorageTime = -1 );
    int PostItem_insert( IN const DWORD& dwToUserUID, IN const char& cCharType, IN const POSTUID& PostUID, IN const GCITEMUID& ItemUID, IN const int& nCount );
    int WaitItem_insert_20111028( IN const DWORD& dwToUserUID, IN const char cCharType_, IN const GCITEMID& ItemID, IN const int& nGrade, IN const int& nPeriod, IN const int& nCount, 
                                 IN const int& nGetType, OUT GCITEMUID& ItemUID, IN int nItemLevel = 0, IN int nStrengthLV = 0, IN char cStatus = 12 );
    bool Post_select( IN const DWORD& dwUserUID, IN const char& cCharType, OUT std::vector<KPostItemInfo>& vecList );
    bool PostItem_select( IN const DWORD& dwUserUID, IN const char& cCharType, OUT std::map<POSTUID,std::vector<KItem> >& mapList );
    int PostItem_delete( IN const DWORD& dwUserUID, IN const char& cCharType, IN const POSTUID& PostUID, IN const GCITEMUID& ItemUID );
    bool Post_delete( IN const DWORD& dwUserUID, IN const char& cCharType, IN const POSTUID& PostUID, IN const int& nDelState );
    int Post_State_update( IN const DWORD& dwUserUID, IN const char& cCharType, IN const POSTUID& PostUID, IN const int nState );
    int WaitItem_Status_update( IN const DWORD& dwToUserUID, IN const char cCharType_, IN const GCITEMUID& ItemUID, IN const char& cBeforeStatus, IN const char& cAfterStatus );
    int WaitItem_Chang_update( IN const DWORD& dwToUserUID, IN const GCITEMUID& ItemUID, IN const DWORD& dwFromUserUID, IN const char& cBeforeStatus, IN const char& cAfterStatus );
    bool WaitItem_post_select( IN const DWORD& dwUserUID, IN const char cCharType_, OUT std::map<GCITEMUID,KItem>& mapList );
    bool Post_NewPost_select( IN const DWORD& dwUserUID, IN const char& cCharType, OUT std::vector<KPostItemInfo>& vecList );
    bool PostItem_detail_select( IN const DWORD& dwUserUID, IN const POSTUID& PostUID, IN const char& cScriptType, IN const GCITEMUID& ItemUID, OUT GCITEMID& ItemID, OUT int& nCount, OUT int& nPeriod );

    bool CharacterLevelPeriod_merge_20111122( IN const DWORD& dwUserUID, IN const int& nCharType, IN const DWORD& dwLv, IN const int& nTimeDiff );
    void UserWareHouseTotalselect( IN const DWORD& dwUID, OUT std::vector<GCITEMUID>& vecWareHouseItem );
    void UserItemStrengthInfo( IN const DWORD& dwUID, IN const char cCharType_, IN std::vector<GCITEMUID>& vecWareHouseItem, IN OUT std::vector<KItem>& vecItem, OUT std::vector<GCITEMUID>& vecRemoveItem );
    bool UserGameClear2_select_20111213( IN const DWORD& dwUserUID, IN const char& cCharType, IN OUT std::map<int,KDungeonUserInfo>& mapList );
    bool UserGameClear2_merge_20111213( IN const DWORD& dwUserUID, IN const char& cCharType, IN const int& nModeID, IN const KDungeonClearData& kInfo );
    bool UserItemLevel_select_20120103( IN const DWORD& dwUserUID, IN OUT std::vector<KItem>& vecItem, IN const char cCharType_ );

    // 창고
    bool UserStorage_merge( IN const DWORD& dwUserUID_, IN const KDepotInfo& kDepot_ );
    bool UserStorage_select( IN const DWORD& dwUserUID_, OUT std::map<char, KDepotInfo >& mapDepotInfo_ );
    bool UserStorage_select_by_char_tab( IN const DWORD& dwUserUID_, IN OUT KDepotInfo& kDepot_ );
    bool WareHouse_Merge( IN const DWORD& dwUserUID_, IN const KDepotItem& kDepot_ );
    bool WareHouse_select( IN const DWORD& dwUserUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Delete( IN const DWORD& dwUserUID_, IN const KDepotItem& kDepot_, OUT int& nCheckRet );
    bool UserItem_DelState_Update( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& Inven_OR_Depot_, IN const char cCharType_ );
    bool GoodsInfoList_CharType_select( const IN GCITEMID& ItemID_, OUT char& charType_ );
    bool WareHouse_Count_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Attribute_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Enchant_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Level_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Strength_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, IN OUT std::vector<KItem>& vecItem, OUT std::map<GCITEMUID, GCITEMUID>& mapStrengthItem_ );
    bool WareHouse_Socket_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    
    bool UserItem_LoginUID_CharType_select( IN const DWORD dwUID_, IN const char cCharType_, OUT std::vector<KItem>& vecItem );
    bool WareHouse_Period_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem );
    bool Item_DelState_Select( IN const DWORD dwUID_, IN const GCITEMUID& ItemUID_, OUT char delState_ );
    bool WareHouse_Coordi_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ );
    bool WareHouse_Pet_select(IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::map< GCITEMUID, KPetInfo >& mapPetInfo_ );

    bool PTEVPlayTimeEvent( IN const DWORD dwUID_, IN const std::wstring& strStartDate, IN const std::wstring& strEndDate );
    void UIGAUserItemDuration_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem );
    void UIGAUserItemPeriod_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem );
    void UIGAUserItemAttribute_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem );
    void UIGAUserItemSocket_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem );
    void UIGAUserItem_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem );

    bool UIGAUserItemCharacter_delete( IN const DWORD& dwUID_, IN const char cCharType_, IN const GCITEMUID& itemUID_ );
    int UIGAUserItemCharacter_insert( IN const DWORD& dwUserUID_, IN const char cCharType_, IN const __int64& WUID_ );

    bool CheckCashback( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN const GCITEMID dwItemID_, IN const int nPrice_, OUT DWORD& dwAddCashPoint_, OUT DWORD& dwResultCashPoint_ );
    bool CheckCashbackFirstBuyRatio( IN const DWORD dwUID_, OUT int& nVersion_, OUT int& nCashbackRatio_ );
    bool CheckCashbackEventRatio( IN const GCITEMID dwItemID_, OUT int& nVersion_, OUT int& nCashbackRatio_ );
    bool CheckCashbackDefaultRatio( IN const GCITEMID dwItemID_, OUT int& nCashbackRatio_ );
    int GetCurrentVirtualCash( IN const DWORD dwUID, IN const std::wstring& strLogin );
    bool VirtualCashMergeWithType( IN const DWORD dwUserUID_, IN const DWORD dwItemID_, IN const int nType_, IN const int nAddPoint_, IN const int nSubtractPoint_ );
    bool Insert_CashBack_FirstBuyUser_ToDB( IN const DWORD dwUID_, IN const int nVersion_, IN const GCITEMID dwItemID_ );
    bool Insert_CashBack_EventUser_ToDB( IN const DWORD dwUID_, IN const int nVersion_, IN const GCITEMID dwItemID_, IN const int nRate_ );
    bool GetUserItemReceiveDate( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem );
    bool GetUserAttributeMigrationInfo( IN const DWORD dwUID, OUT std::map<GCITEMUID, DWORD>& mapAttributeMigrationItemInfo );
    bool UserItemAttributeResetCountMerge( IN const DWORD dwUserUID, IN const GCITEMUID dwItemUID );
    bool WareHouse_ReceiveDate_select( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem );
    bool SetCooriComposeInfo( IN const DWORD& dwUID, IN const GCITEMUID& AbilityItemUID, IN const GCITEMUID& DesignItemUID, IN const GCITEMID& DesignCoordiItemID );
    bool GetUserItemCooriCompose( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem, OUT std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi );
    bool WareHouse_ItemCooriCompose( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem );
    bool InsertClientNetworkStat( IN const std::wstring& strIP, IN const int& nID, IN const int& nCount );
    bool DeleteAttribute( IN const DWORD dwUserUID, IN const GCITEMUID& ItemUID );

    int UpdateMoneyConsumptionStat( IN const DWORD& dwUID_, IN const int& nMoneyType_, IN const int& nAmount_, IN const __int64& biWaitItemUID_  );
    bool User_auth_update( IN const DWORD& dwUserUID, IN const bool& bChatEnable );

    int GetTodayEventCount( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_ );
    bool SetTodayEventDoneDiff( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ );
    bool SetTodayEventDone( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ );
    bool UpdateItemDelState( IN const DWORD dwUID, IN const GCITEMUID ItemUID, IN const DWORD dwDelState );
    bool GetSellCountByEventUID_SocialCommerce( IN const DWORD& dwEventUID, IN int& nSellCount );
    void VirtualDepotItemAttribute_select( IN const DWORD& dwUserUID, IN OUT std::vector<KItem>& vecItem );
    void VirtualDepotItemSocket_select( IN const DWORD& dwUserUID, IN OUT std::vector<KItem>& vecItem );
    bool VirtualDepotLevel_select( IN const DWORD& dwUserUID, IN OUT std::vector<KItem>& vecItem );
    void VirtualDepotStrengthInfo_select( IN const DWORD& dwUID, IN OUT std::vector<KItem>& vecItem );
    void VirtualDepotItemReceiveDate_select( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem );
    void VirtualDepotItemCooriCompose_select( IN const DWORD dwUID, IN OUT std::vector<KItem>& vecItem );
    bool CharNickToLoginUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ );
    bool LoginUIDToLogin( IN const DWORD dwLoginUID_, OUT std::wstring& strCharNick_ );
    bool InsertInvenDepotMoveLog( IN const KInvenDepotMoveLog& kInvenDepotMoveLog );
    bool IgnorableErrorProcessingAfterCallSP( IN OUT int& nReturnValue ); // KCommonSP와 KSP 양쪽에 있으므로 수정시 주의가 필요하다
    bool InsertCharMissionInfo( IN const std::map< std::pair<char,char>,std::vector<DWORD> > mapCharMissionList, IN const int nType_ );

private:
    bool GetWaitInnerItemList_SetItem( IN const __int64 waitItemUID, IN const KItem& kItem, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems );
    bool GetWaitInnerItemList_Package( IN const __int64 waitItemUID, IN const KItem& kItem, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems );
    bool CheckUserItem_merge( IN const DWORD& dwUserUID, IN const GCITEMID& ItemID, IN const char& cGrade, IN const GCITEMUID& WaitItemUID,
        IN const int nPeriod, IN const int nDuration, IN const USHORT usAttributeSize, IN const USHORT usSocketSize, OUT GCITEMUID& ItemUID, IN const char cCharType_, OUT bool& bMerged_ );
    int UIGAUserItem_merge_20130415( IN const DWORD& dwUserUID_, IN const GCITEMID& ItemID_, IN const char& cGrade_, IN const GCITEMUID& WaitItemUID_, OUT GCITEMUID& ItemUID_ );
    bool UserItemPeriod_merge( IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nPeriod, OUT time_t& tmEndDate );
    bool UserItemCount_merge( IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nCountDiff, OUT int& nCount );
    bool UpdateSocket_merge( IN const DWORD& dwUserUID, IN const int& nSlotID, IN const GCITEMID& CardID, IN const int& nSocState, IN OUT KItem& kItem, IN const char cCharType_ );
    bool UpdateLevel_merge( IN const DWORD dwUserUID, IN const int nType, IN const int nLevel, IN OUT KItem& kItem, IN const char cCharType_ );

private:
    KCommonSP m_kCommonSP;
};
