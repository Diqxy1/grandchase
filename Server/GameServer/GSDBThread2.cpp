#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "NetError.h"
#include "EnchantStrong.h"
#include "ResultManager.h"
#include <boost/bind.hpp>
#include "support_User.h"
#include "Rainbow.h"
#include "IPAdvantage.h"
#include "LogManager.h"
#include "GCHelper.h"
#include "ItemManager.h"
#include "VitalityManager.h"
#include <boost/foreach.hpp>
#include "TongDonation.h"
#include "DungeonRankManager.h"

#define CLASS_TYPE KGSDBThread

namespace
{
    class KAttributeComp
    {
    public:
        bool operator() (const KAttributeInfo& a, const KAttributeInfo& b) {
            if (a.m_cSlotID < b.m_cSlotID) {
                return true;
            }

            return false;
        }
    };
}

_INT_IMPL_ON_FUNC(EVENT_STRONG_LEVELUP_REQ, KDB_EVENT_STRONG_LEVELUP_REQ)
{
    EVENT_CHECK_OUT(EVENT_STRONG_LEVELUP_REQ);

    KDB_EVENT_STRONG_LEVELUP_ACK kDBPacket;
    kDBPacket.m_dwWeakAssistItemID = kPacket_.m_kPacket.m_dwWeakAssistItemID;
    kDBPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;

    KEVENT_STRONG_LEVELUP_ACK kPacket;
    kPacket.m_nOK = -99;

    int nNextLevel = -1;
    GCITEMID dwNextItemID = 0;
    int nKindItem = -1;
    int nCharType = -1;
    int nNowLevel = -1;
    int nDecision = -1;
    DWORD dwUseMaterialCount = 0;
    KItem kBeforeItem; //현재 보유 아이템
    KItem kNextItem; //강화 이후 아이템. 
    KLottery kEnchantUnit;
    std::vector< std::pair<int, float> > vecAssistRatio; // 강화 보조 
    KRainBowData kRainbow;
    bool bRainbow = SiKRainbow()->GetEventByID(KRainbow::RE_NECKLACE_STRONG, kRainbow);

    SET_ERROR(ERR_UNKNOWN);

    // 아이템 강화 정보 받아오기
    JIF(GetStrongItemLevel(kPacket_.m_kPacket.m_dwGoodsID, nKindItem, nNowLevel, nCharType));

    // 등록되지 않은 강화 타입
    if (nKindItem < 0 || nKindItem >= KEnchantStrong::EK_MAX) {
        SET_ERR_GOTO(ERR_STRONG_01, END_PROC);
    }

    // 강화에 필요한 재료(젬) 개수 구하기
    if (SiKEnchantStrong()->GetNeedMaterialCount(nNowLevel + 1, dwUseMaterialCount) == false) {
        SET_ERR_GOTO(ERR_STRONG_03, END_PROC);
    }

    // 레인 보우에 의해서 사용 젬 개수 조절.
    if (bRainbow && dwUseMaterialCount > 1) {
        kRainbow.m_fSecondaryRatio = std::min<float>(1.f, kRainbow.m_fSecondaryRatio);
        kRainbow.m_fSecondaryRatio = std::max<float>(0.f, kRainbow.m_fSecondaryRatio);
        dwUseMaterialCount = (DWORD)((dwUseMaterialCount * kRainbow.m_fSecondaryRatio) + 0.5f);
    }

    // 강화재료(Gem)는 충분한가?
    if (kPacket_.m_kMaterialItem.m_nCount < (int)dwUseMaterialCount) {
        SET_ERR_GOTO(ERR_STRONG_04, END_PROC);
    }

    // 강화 정보 있는가?
    if (SiKEnchantStrong()->GetEnchantUnit(nKindItem, nNowLevel + 1, kPacket_.m_nMaterialIndex, kEnchantUnit) == false) {
        SET_ERR_GOTO(ERR_STRONG_08, END_PROC);
    }

    // 하락방지 아이템 사용시, 확률 조정
    if (SiKEnchantStrong()->GetStrongAssist(kPacket_.m_kPacket.m_dwWeakAssistItemID, nNowLevel + 1, vecAssistRatio) == true) {
        std::wstringstream stm;
        stm << L"Before" << std::endl;
        kEnchantUnit.ToString(stm);

        std::vector< std::pair<int, float> >::const_iterator vit;
        for (vit = vecAssistRatio.begin(); vit != vecAssistRatio.end(); ++vit) {
            kEnchantUnit.MultipleCase(vit->first, vit->second);
        }
        stm << L"After" << std::endl;
        kEnchantUnit.ToString(stm);

        START_LOG(clog, L"강화 확률 조정, Name : " << strLogin_)
            << stm.str() << END_LOG;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    // 캐릭터별 강화 확률 조정
    if (SiKEnchantStrong()->GetStrongAssistByCharacter(nCharType, nNowLevel + 1, vecAssistRatio) == true) {
        std::wstringstream stm;
        stm << L"Before" << std::endl;
        kEnchantUnit.ToString(stm);

        std::vector< std::pair<int, float> >::const_iterator vit;
        for (vit = vecAssistRatio.begin(); vit != vecAssistRatio.end(); ++vit) {
            kEnchantUnit.MultipleCase(vit->first, vit->second);
        }
        stm << L"After" << std::endl;
        kEnchantUnit.ToString(stm);

        START_LOG(clog, L"캐릭터별 강화 확률 조정, Name : " << strLogin_)
            << stm.str() << END_LOG;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    // 레인 보우가 적용 될 경우
    if (bRainbow == true) {
        kEnchantUnit.MultipleCase(KEnchantStrong::EA_SUCCESS, kRainbow.m_fPrimaryRatio + 1.f);
    }

    // PC방 유저 보너스
    if (SiKIPAdvantage()->IsPCBangUser(kPacket_.m_cPCBangType) == true) {
        kEnchantUnit.MultipleCase(KEnchantStrong::EA_SUCCESS, SiKIPAdvantage()->GetNecklaceStrongBonus(KIPAdvantage::PCBANG_PUBLISHER) + 1.f);
    }

    // 길드 보너스
    if (kPacket_.m_fGuildBonus > 0.f) {
        kEnchantUnit.MultipleCase(KEnchantStrong::EA_SUCCESS, kPacket_.m_fGuildBonus + 1.f);
    }

    // PC방 유저 보너스
    if (SiKIPAdvantage()->IsTH3BBUser(kPacket_.m_cUserBenfitType) == true) {
        kEnchantUnit.MultipleCase(KEnchantStrong::EA_SUCCESS, SiKIPAdvantage()->GetNecklaceStrongBonus(KIPAdvantage::TH3BB_USER) + 1.f);
    }

    // 강화 확률 결정
    nDecision = kEnchantUnit.Decision();
    if (nDecision == KLottery::CASE_BLANK) {
        nDecision = KEnchantStrong::EA_NONE;
    }

    int nRet = UpdateEnchantStrongItem(dwUID_, nCharType, kPacket_.m_kPacket.m_dwGoodsUID, kPacket_.m_kPacket.m_dwGoodsID, nDecision, dwNextItemID);

    if (nRet != 0) { //0 : 강화 성공
        switch (nRet)
        {
        case -1: SET_ERR_GOTO(ERR_STRONG_09, END_PROC); // -1 : 해당 유저 정보가 존재하지 않음
        case -2: SET_ERR_GOTO(ERR_STRONG_10, END_PROC); // -2 : 강화를 해야하는 아이템이 존재하지 않음
        case -3: SET_ERR_GOTO(ERR_STRONG_11, END_PROC); // -3 : 잼 아이템이 없거나 강화에 필요한 잼수량보다 작게 보유하고 있음 
        case -4: SET_ERR_GOTO(ERR_STRONG_12, END_PROC); // -4 : 강화할 아이템 정보가 없거나 강화할 수 있는 아이템이 아님
        case -5: SET_ERR_GOTO(ERR_STRONG_13, END_PROC); // -5 : 강화할 아이템의 정보가 GoodsInfoList에 해당 정보와 일치하지 않음
        case -6: SET_ERR_GOTO(ERR_STRONG_14, END_PROC); // -6 : 강화될 아이템 정보가 없음
        case -7: SET_ERR_GOTO(ERR_STRONG_15, END_PROC); // -7 : 강화될 아이템의 정보가 GoodsInfoList에 해당 정보와 일치하지 않음
        default:
            SET_ERR_GOTO(ERR_DB_COMMON_00, END_PROC); //-101이하 : 트랜잭션 에러
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if (!CheckItemDuration(dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, kPacket_.m_kMaterialItem.m_nInitCount, kPacket_.m_kMaterialItem.m_nCount)) {
        START_LOG(clog, L"목걸이 강화 재료 아이템 창고 수량 동기화. Name : " << strLogin_)
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_kMaterialItem.m_ItemUID)
            << BUILD_LOG(kPacket_.m_kMaterialItem.m_nInitCount)
            << BUILD_LOG(kPacket_.m_kMaterialItem.m_nCount)
            << END_LOG;
    }

    // 사용된 강화 재료템의 차감
    if (UseCountItem(dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, (int)dwUseMaterialCount) == false) {
        START_LOG(cerr, L"낙장불입..사용된 강화 재료템의 차감 실패. Name : " << strLogin_) << END_LOG;
    }

    // 강화재료템 개수 갱신( = 현재개수 - 사용개수 )
    kPacket_.m_kMaterialItem.m_nCount -= dwUseMaterialCount;
    kPacket_.m_kMaterialItem.m_nInitCount = kPacket_.m_kMaterialItem.m_nCount;
    kDBPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;

    // 오나료..  셋팅하자.
    kBeforeItem.m_ItemID = kPacket_.m_kPacket.m_dwGoodsID;
    kBeforeItem.m_ItemUID = kPacket_.m_kPacket.m_dwGoodsUID;

    kNextItem.m_nPeriod = -1;
    kNextItem.m_ItemID = dwNextItemID;
    kNextItem.m_ItemUID = kPacket_.m_kPacket.m_dwGoodsUID;

    kPacket.m_vecUpdateItem.push_back(kNextItem);
    kPacket.m_vecDelItem.push_back(kBeforeItem);
    kPacket.m_nOK = nDecision; // 0: 성공, 1: 하락, 2: 깨짐, 3:제자리
    SET_ERROR(NET_OK);

END_PROC:

    if (NetError::GetLastNetError() != NetError::NET_OK) {
        switch (NetError::GetLastNetError())
        {
            ERR_CASE(ERR_STRONG_00, kPacket.m_nOK, -1); // 강화 할수 없음_NextLevel또는NextGoodsID 오류
            ERR_CASE(ERR_STRONG_01, kPacket.m_nOK, -2); // ItemKind값이 잘못 되었음
            ERR_CASE(ERR_STRONG_02, kPacket.m_nOK, -3); // 해당 캐릭터 정보가 존재하지 않음
            ERR_CASE(ERR_STRONG_03, kPacket.m_nOK, -4); // 사용 되는 젬 개수 정보가 없음
            ERR_CASE(ERR_STRONG_04, kPacket.m_nOK, -5); // 젬이 부족합니다
            ERR_CASE(ERR_STRONG_05, kPacket.m_nOK, -6); // 강화 쿼리 실패
        default:
            kPacket.m_nOK = -99;
            START_LOG(cerr, L"강화 알수 없는 오류 , Name : " << strLogin_)
                << BUILD_LOG(NetError::GetLastNetError())
                << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
        }
    }

    kDBPacket.m_kPacket = kPacket;
    _QUEUING_ACK_TO_USER(EVENT_STRONG_LEVELUP_ACK, kDBPacket);

    _LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK, L"Strong Levelup Result : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket_.m_kPacket.m_dwGoodsID)
        << BUILD_LOG(kPacket_.m_kPacket.m_dwGoodsUID)
        << BUILD_LOG(kPacket_.m_kPacket.m_dwWeakAssistItemID)
        << BUILD_LOG(kPacket_.m_kMaterialItem.m_nCount)
        << BUILD_LOG(kPacket_.m_kMaterialItem.m_ItemUID)
        << BUILD_LOG(kPacket_.m_kMaterialItem.m_ItemID)
        << BUILD_LOGc(kPacket_.m_cPCBangType)
        << BUILD_LOG(kPacket_.m_fGuildBonus)
        << BUILD_LOG(nDecision)           // 강화 결과
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(dwNextItemID)        //다음 강화 아이템
        << BUILD_LOG(nKindItem)           //종류
        << BUILD_LOG(nCharType)           //캐릭터 타입
        << BUILD_LOG(kRainbow.m_nEventID)
        << BUILD_LOG(kRainbow.m_fPrimaryRatio)
        << BUILD_LOG(kRainbow.m_fSecondaryRatio)
        << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

_INT_IMPL_ON_FUNC(EVENT_BUY_FOR_GP_REQ, KDB_EVENT_BUY_FOR_GP_REQ)
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_GP_REQ);

    KEVENT_BUY_FOR_GP_ACK kPacket;
    __int64 waitItemUID = 0;
    kPacket.m_nOK = -99;
    std::wstring wstrItemName = L"";
    SET_ERROR(ERR_UNKNOWN);

    if (!UpdateUserData(kPacket_.m_kData)) {
        SET_ERR_GOTO(ERR_BUY_ITEM_02, buy_end); // ÇÊ¼öµ¥ÀÌÅÍ µ¿±âÈ­ ½ÇÆÐ.
    }

    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    m_kHelper.GetPrice(kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, kBuyInfo.m_nPrice, kBuyInfo.m_nMoneyType);

    kPacket.m_nUseGP = kBuyInfo.m_nPrice;

    if (kBuyInfo.m_nMoneyType != KItemManager::EMT_GAMEPOINT)
    {
        SET_ERR_GOTO(ERR_BUY_ITEM_06, buy_end);
    }

    if (!CheckBuyLimitItem(dwUID_, kPacket_.m_kBuyInfo.m_ItemID))
    {
        SET_ERR_GOTO(ERR_BUY_ITEM_07, buy_end);
    }

    if (m_kHelper.BuyItemToWait(dwUID_, strLogin_, kBuyInfo, std::wstring(L""), waitItemUID, wstrItemName) != 0)
    {
        SET_ERR_GOTO(ERR_BUY_ITEM_01, buy_end);
    }

    if (waitItemUID == KItem::INVALID_ITEM)
    {
        START_LOG(cerr, L"WaitItemUID°¡ 0 ÀÌ´Ù.") << END_LOG;
        SET_ERR_GOTO(ERR_BUY_ITEM_01, buy_end);
    }

    if (kPacket_.m_kData.m_iGamePoint < kPacket.m_nUseGP) {
        m_kHelper.RollbackBuyItem(waitItemUID);
        SET_ERR_GOTO(ERR_BUY_ITEM_00, buy_end); // ÀÜ¾×ÀÌ ºÎÁ·ÇÔ.
    }

    m_kHelper.UpdateWaitItemToEnabled(waitItemUID);

    if (m_kHelper.InsertSingleWaitItemToInvenOrVDepot(dwUID_, cCharType_, waitItemUID, KItemManager::EWIT_GP_BUY, kPacket.m_vecItems) == false)
    {
        SET_ERR_GOTO(ERR_BUY_ITEM_01, buy_end); // ¾ÆÀÌÅÛ »ðÀÔ ½ÇÆÐ
    }

    CommitBuyLimitItem(dwUID_, kPacket_.m_kBuyInfo.m_ItemID);
    m_kHelper.UpdateMoneyConsumptionStat(dwUID_, KItemManager::EMC_GP, kPacket_.m_kBuyInfo.m_nCount, waitItemUID);

    SET_ERROR(NET_OK);

buy_end:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER(EVENT_BUY_FOR_GP_ACK);

    START_LOG(clog, L"GP·Î ¾ÆÀÌÅÛ ±¸ÀÔ OK : " << kPacket.m_nOK)
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket.m_nUseGP)
        << BUILD_LOG(kPacket.m_vecItems.size())
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_ItemID)
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_nCount)
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_nPeriod)
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_nMoneyType)
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_nPrice) << END_LOG;
}

_INT_IMPL_ON_FUNC(EVENT_BUY_FOR_GEM_REQ, KDB_EVENT_BUY_FOR_GEM_REQ)
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_GEM_REQ);

    KEVENT_BUY_FOR_GEM_ACK      kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_moneyItem = kPacket_.m_moneyItem;
    __int64 biWaitUID = 0;

    SET_ERROR(ERR_UNKNOWN);
    if (!UpdateUserData(kPacket_.m_kData)) {
        SET_ERR_GOTO(ERR_BUY_ITEM_02, buy_end); // ÇÊ¼öµ¥ÀÌÅÍ µ¿±âÈ­ ½ÇÆÐ.
    }

    int nMoneyType;
    m_kHelper.GetPrice(kPacket_.m_kPacket.m_ItemID, kPacket_.m_kPacket.m_nDuration, kPacket_.m_kPacket.m_nPeriod, kPacket.m_nPrice, nMoneyType);

    if (nMoneyType != KItemManager::EMT_GEM) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, buy_end);
    }

    if (kPacket.m_nPrice < kPacket_.m_moneyItem.m_nCount) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, buy_end);
    }

    if (!m_kHelper.InsertSingleItemJob_WaitUID(dwUID_, cCharType_, kPacket_.m_kPacket.m_ItemID, kPacket_.m_kPacket.m_nDuration, kPacket_.m_kPacket.m_nPeriod, KItemManager::EWIT_GEM_BUY, kPacket.m_vecItem, biWaitUID)) {
        SET_ERR_GOTO(ERR_BUY_ITEM_01, buy_end); // ¾ÆÀÌÅÛ »ðÀÔ ½ÇÆÐ
    }

    m_kHelper.UpdateMoneyConsumptionStat(dwUID_, KItemManager::EMC_GEM, kPacket_.m_kPacket.m_nDuration, biWaitUID);

    SET_ERROR(NET_OK);
buy_end:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER(EVENT_BUY_FOR_GEM_ACK);

    START_LOG(clog, L"Áª À¸·Î ¾ÆÀÌÅÛ ±¸¸Å : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket_.m_moneyItem.m_ItemID)
        << BUILD_LOG(kPacket_.m_moneyItem.m_nCount)
        << BUILD_LOG(kPacket.m_moneyItem.m_nCount)
        << BUILD_LOG(kPacket.m_nPrice) << END_LOG;
}

_INT_IMPL_ON_FUNC(EVENT_BUY_FOR_CRYSTAL_REQ, KDB_EVENT_BUY_FOR_CRYSTAL_REQ)
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_CRYSTAL_REQ);

    KEVENT_BUY_FOR_CRYSTAL_ACK      kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_moneyItem = kPacket_.m_moneyItem;

    SET_ERROR(ERR_UNKNOWN);
    if (!UpdateUserData(kPacket_.m_kData)) {
        SET_ERR_GOTO(ERR_BUY_ITEM_02, buy_end); // ÇÊ¼öµ¥ÀÌÅÍ µ¿±âÈ­ ½ÇÆÐ.
    }

    int nMoneyType;
    m_kHelper.GetPrice(kPacket_.m_kPacket.m_ItemID, kPacket_.m_kPacket.m_nDuration, kPacket_.m_kPacket.m_nPeriod, kPacket.m_nPrice, nMoneyType);

    if (nMoneyType != KItemManager::EMT_CRYSTAL) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, buy_end);
    }

    if (kPacket.m_nPrice > kPacket_.m_moneyItem.m_nCount) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, buy_end);
    }

    if (!m_kHelper.InsertSingleItemJob(dwUID_, cCharType_, kPacket_.m_kPacket.m_ItemID, kPacket_.m_kPacket.m_nDuration, kPacket_.m_kPacket.m_nPeriod, KItemManager::EWIT_CRISTAL_BUY, kPacket.m_vecItem)) {
        SET_ERR_GOTO(ERR_BUY_ITEM_01, buy_end);
    }

    SET_ERROR(NET_OK);

buy_end:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER(EVENT_BUY_FOR_CRYSTAL_ACK);

    START_LOG(clog, L"Å©¸®½ºÅ»·Î ¾ÆÀÌÅÛ ±¸¸Å : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket_.m_moneyItem.m_ItemID)
        << BUILD_LOG(kPacket_.m_moneyItem.m_nCount)
        << BUILD_LOG(kPacket.m_moneyItem.m_nCount)
        << BUILD_LOG(kPacket.m_nPrice) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM(EVENT_QUERY_INVENTORY_INFO_REQ)
{
    EVENT_CHECK_OUT(EVENT_QUERY_INVENTORY_INFO_REQ);

    KEVENT_QUERY_INVENTORY_INFO_ACK kPacket;
    LIF(m_kHelper.InsertFullWaitItemToInven(dwUID_, cCharType_, std::vector<KItem>()));
    std::vector<std::pair<GCITEMUID, GCITEMID>> vecDesignCoordi;
    GetItemListFromDB(dwUID_, cCharType_, kPacket.m_vecInv, vecDesignCoordi);
    AdjustLookItemInfo(dwUID_, kPacket.m_vecInv, cCharType_);

    START_LOG(clog, L"¾ÆÀÌÅÛ °³¼ö Ãâ·Â")
        << BUILD_LOG(kPacket.m_vecInv.size()) << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_QUERY_INVENTORY_INFO_ACK);
}

_INT_IMPL_ON_FUNC(DB_EVENT_CHECK_EXPIRATION_ITEM_REQ, std::vector<KItem>)
{
    // ±â°£Á¦ ¾ÆÀÌÅÛ ±â°£ ¸¸·á
    int nFailCount = 0;
    std::vector<KItem>::iterator vit;

    for (vit = kPacket_.begin(); vit != kPacket_.end(); vit++)
    {
        /*
        L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

        { call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
        @01: @01iLoginUID_input int
        @02: @02iItemUID_input  bigint
        @04: @03iDelstate_input tinyint

        1 return ( @01 )
        @01: OK
            0        : ¼º°ø
            -1       : ¸ÞÀÎ ¾ÆÀÌÅÛ Á¤º¸ ¾øÀ½
            -801~-899: ¹«½Ã°¡´É ¿¡·¯
            -901~-999: Ä¡¸íÀûÀÎ ¿¡·¯
            -1000    : »ç¿ë ÁßÁöµÈ spÀÎµ¥ »ç¿ëÇÏ·Á ÇÔ
        */
        if (!m_kODBC.Query(L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }",
            dwUID_, vit->m_ItemUID, KItemManager::EDEL_DEL_PERIOD, cCharType_))
        {
            START_LOG(cerr, L"¸¸±â¾ÆÀÌÅÛ »èÁ¦Ã³¸® ½ÇÆÐ. Name : " << strLogin_)
                << BUILD_LOG(vit->m_nPeriod)
                << BUILD_LOG(vit->m_ItemID)
                << BUILD_LOG(vit->m_ItemUID)
                << END_LOG;

            nFailCount++;
            vit->m_nPeriod = UNKNOWN_ERROR;

            continue;
        }

        if (!m_kODBC.BeginFetch())
        {
            START_LOG(cerr, L"¸¸±â¾ÆÀÌÅÛ »èÁ¦Ã³¸® ½ÇÆÐ. Name : " << strLogin_)
                << BUILD_LOG(vit->m_nPeriod)
                << BUILD_LOG(vit->m_ItemID)
                << BUILD_LOG(vit->m_ItemUID)
                << END_LOG;

            nFailCount++;
            vit->m_nPeriod = UNKNOWN_ERROR;

            continue;
        }

        int nOK = -99;
        VERIFY_QUERY(m_kODBC >> nOK);
        m_kODBC.EndFetch();

        LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

        if (nOK != 0 && nOK != -1) {
            nFailCount++;
            vit->m_nPeriod = UNKNOWN_ERROR;
        }

        _LOG_SUCCESS(nOK == 0 || nOK == -1, L"¸¸±â¾ÆÀÌÅÛ »èÁ¦Ã³¸® ½ÇÆÐ. Name : " << strLogin_)
            << BUILD_LOG(nOK)
            << BUILD_LOG(vit->m_nPeriod)
            << BUILD_LOG(vit->m_ItemID)
            << BUILD_LOG(vit->m_ItemUID)
            << END_LOG;

    } // end for

    std::vector<KItem>& kPacket = kPacket_;

    QUEUING_ACK_TO_USER(EVENT_ITEM_EXPIRED_NOT);

    LOG_CONDITION(nFailCount > 0, cerr, clog)
        << BUILD_LOG(kPacket.size())
        << BUILD_LOG(nFailCount) << END_LOG;
}

INT_IMPL_ON_FUNC(DB_EVENT_VITALITY_INFO_REQ)
{
    // ÀÌ ÀÌº¥Æ®´Â À¯Àúº° Á¢¼Ó½Ã, ÃÊ±âÈ­ ½Ã°£¿¡ Á¢¼Ó ÁßÀÎ ¸ðµç À¯ÀúÀÇ ¿äÃ»À¸·Î µé¾î¿Â´Ù
    // kPacket_: true: °­Á¦·Î ÃÊ±âÈ­, false: ÃÊ±âÈ­ ½Ã°£ Á¶°Ç ¸¸Á·½Ã¿¡¸¸ ÃÊ±âÈ­
    time_t tmLastInitialize = 0;
    KEVENT_VITALITY_INFO_NOT kPacket;

    SET_ERROR(ERR_UNKNOWN);

    // È°·Â ¸¶Áö¸· ÃÊ±âÈ­ ½Ã°£À¸·Î ÀÌ¹ø¿¡ ÃÊ±âÈ­ÇØ¾ß ÇÒÁö È®ÀÎ
    Get_VitalitySystem_LastResetTime_FromDB(dwUID_, tmLastInitialize);

    if (true == kPacket_.m_bNeedForceInit || true == CheckNeedVitalityReset(tmLastInitialize)) {
        // true == kPacket_.m_bNeedForceInit ÀÎ °æ¿ì: Á¢¼Ó ÁßÀÎ À¯ÀúµéÀÇ È°·Â Á¤º¸ ÃÊ±âÈ­
        // true == CheckNeedVitalityReset( tmLastInitialize ) ÀÎ °æ¿ì: »õ·Î Á¢¼ÓÇÑ À¯ÀúÀÇ È°·Â Á¤º¸ ÃÊ±âÈ­

        if (true == kPacket_.m_bVitalityInfoLoaded) {
            // ·Î±× ±â·ÏÀ» À§ÇØ ÃÊ±âÈ­ ½ÃÁ¡¿¡ ¼­¹ö¿¡ Ä³½ÌµÈ ÀÜ¿© È°·Â Á¤º¸¸¦ DB¿¡ ±â·Ï
            Update_VitalitySystem_RechargePoint_ToDB(dwUID_, kPacket_.m_kVitalityUpdateData.m_nRechargePoint, kPacket_.m_kVitalityUpdateData.m_nRechargePointCounter);
            Update_VitalitySystem_RemainVitality_ToDB(dwUID_, kPacket_.m_mapCharacterInfo, kPacket_.m_kVitalityUpdateData.m_mapRemainVitality, KVitalityManager::EVUT_BEFORE_INIT);
        }

        // ÀçÃæÀü Æ÷ÀÎÆ® Á¤º¸ ÃÊ±â°ª ¼³Á¤
        Initialize_VitalitySystem_RechargePoint(kPacket.m_nRechargePoint, kPacket.m_nRechargePointCounter);

        // È°·Â ÃÊ±â°ª ¼³Á¤
        Initialize_VitalitySystem_RemainVitality(kPacket_.m_mapCharacterInfo, kPacket.m_mapRemainVitality);

        // ÃÊ±âÈ­µÈ È°·Â Á¤º¸ À¯Àú µ¥ÀÌÅÍ DB¿¡ ±â·Ï
        Update_VitalitySystem_RechargePoint_ToDB(dwUID_, kPacket.m_nRechargePoint, kPacket.m_nRechargePointCounter);
        Update_VitalitySystem_RemainVitality_ToDB(dwUID_, kPacket_.m_mapCharacterInfo, kPacket.m_mapRemainVitality, KVitalityManager::EVUT_INIT);

        // È°·Â ¸¶Áö¸· ÃÊ±âÈ­ ½Ã°£ °»½Å(DB½Ã°£À¸·Î ±â·ÏÇÔ)
        Update_VitalitySystem_LastResetTime_ToDB(dwUID_);
    }
    else {
        // ÀçÃæÀü Æ÷ÀÎÆ® Á¤º¸ Á¶È¸
        Get_VitalitySystem_RechargePoint_FromDB(dwUID_, kPacket.m_nRechargePoint, kPacket.m_nRechargePointCounter);

        // Ä³¸¯ÅÍº° ÀÜ¿© È°·Â Á¶È¸(Ä³¸¯ÅÍº° ÃÖ´ë È°·ÂÀ¸·Î º¸Á¤À» Æ÷ÇÔÇÑ´Ù)
        Get_VitalitySystem_RemainVitality_FromDB(dwUID_, kPacket_.m_mapCharacterInfo, kPacket.m_mapRemainVitality);
    }

    SET_ERROR(NET_OK);

    //END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket.m_nRechargePoint)
        << BUILD_LOG(kPacket.m_nRechargePointCounter)
        << BUILD_LOG(kPacket.m_mapRemainVitality.size())
        << BUILD_LOG(kPacket_.m_bNeedForceInit)
        << BUILD_LOG(kPacket_.m_mapCharacterInfo.size())
        << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_VITALITY_INFO_NOT);
}

INT_IMPL_ON_FUNC(DB_EVENT_VITALITY_RECHARGE_NOT)
{
    Update_VitalitySystem_RechargePoint_ToDB(dwUID_, kPacket_.m_nRechargePoint, kPacket_.m_nRechargePointCounter);

    Update_VitalitySystem_RemainVitality_Character_ToDB(dwUID_, kPacket_.m_kCharacterInfo, kPacket_.m_nRemainVitality, kPacket_.m_nVitalityUpdateType);

    START_LOG(clog, L"È°·Â ½Ã½ºÅÛ È°·Â ÃæÀü Åë°è DB±â·Ï(User)") << END_LOG;
}

INT_IMPL_ON_FUNC(EVENT_STAT_VITALITY_DUNGEON_PLAY_NOT)
{
    VitalitySystemDungeonPlayStat_insert(kPacket_);

    START_LOG(clog, L"È°·Â ½Ã½ºÅÛ ´øÀü ÇÃ·¹ÀÌ Åë°è DB±â·Ï(User)") << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM(DB_EVENT_TONG_DONATION_TOTAL_AMOUNT_INFO_REQ)
{
    time_t tmCurrentVersion = SiKTongDonation()->GetCurrentVersion();
    std::vector< DWORD > vecNPCType;
    SiKTongDonation()->GetNPCTypes(vecNPCType);

    BOOST_FOREACH(DWORD & rNPCType, vecNPCType) {
        int nEventType = SiKTongDonation()->GetEventType(rNPCType);
        int nTotalAmount = GetTotalServerCountData(tmCurrentVersion, nEventType);
        SiKTongDonation()->SetTotalAmount(tmCurrentVersion, rNPCType, static_cast<DWORD>(nTotalAmount));
    }
}

INT_IMPL_ON_FUNC_NOPARAM(DB_EVENT_TONG_DONATION_USER_AMOUNT_INFO_REQ)
{
    KEVENT_TONG_DONATION_AMOUNT_INFO_ACK kPacket;

    time_t tmCurrentVersion = SiKTongDonation()->GetCurrentVersion();
    std::vector< DWORD > vecNPCType;
    SiKTongDonation()->GetNPCTypes(vecNPCType);

    BOOST_FOREACH(DWORD & rNPCType, vecNPCType) {
        int nEventType = SiKTongDonation()->GetEventType(rNPCType);
        int nUserAmount = GetTodayEventCount(dwUID_, nEventType, tmCurrentVersion);
        kPacket[tmCurrentVersion][rNPCType] = static_cast<DWORD>(nUserAmount);
    }

    QUEUING_ACK_TO_USER(EVENT_TONG_DONATION_AMOUNT_INFO_ACK);
}

INT_IMPL_ON_FUNC(EVENT_TONG_DONATION_DONATE_REQ)
{
    EVENT_CHECK_OUT(EVENT_TONG_DONATION_DONATE_REQ);

    KEVENT_TONG_DONATION_DONATE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kDonationItem = kPacket_.m_kDonationItem;

    SET_ERROR(ERR_UNKNOWN);

    BOOST_FOREACH(KItem & rItem, kPacket_.m_vecDonationRewards) {
        if (false == m_kHelper.InsertSingleItemJob(dwUID_, cCharType_, rItem.m_ItemID, rItem.m_nCount, rItem.m_nPeriod,
            KItemManager::EWIT_TONG_DONATION_DONATE_REWARD, kPacket.m_vecDonationRewards)) {
            START_LOG(cerr, L"ÅëÇÕ ±âºÎ ÀÌº¥Æ® ±âºÎÇÒ ¶§ ¹Þ´Â º¸»ó ¾ÆÀÌÅÛ ¾ò±â DB ÀÛ¾÷ ½ÇÆÐ. Name : " << strLogin_)
                << BUILD_LOG(rItem.m_ItemID)
                << BUILD_LOG(rItem.m_nCount)
                << BUILD_LOG(rItem.m_nPeriod) << END_LOG;
            SET_ERR_GOTO(ERR_TONG_DONATION_ERROR_05, END_PROC);
        }
    }

    // ±âºÎÇÒ ¾ÆÀÌÅÛÀÇ delFactor = ¼­¹öÀÇ Ä³½Ì ¼ö·® - DBÀÇ ¼ö·® + ±âºÎÇÒ ¼ö·®
    int nDelFactor = kPacket_.m_kDonationItem.m_nCount - kPacket_.m_kDonationItem.m_nInitCount + kPacket_.m_dwDonateCount;
    if (false == UpdateCountItem(dwUID_, kPacket_.m_kDonationItem.m_ItemUID, nDelFactor)) {
        START_LOG(cerr, L"±âºÎ ¾ÆÀÌÅÛ ¼ö·® µ¿±âÈ­ ½ÇÆÐ. Name : " << strLogin_) << END_LOG;
        SET_ERR_GOTO(ERR_TONG_DONATION_ERROR_06, END_PROC);
    }
    kPacket.m_kDonationItem.m_nCount -= kPacket_.m_dwDonateCount;
    kPacket.m_kDonationItem.m_nCount = std::max< int >(0, kPacket.m_kDonationItem.m_nCount);

    int nEventType = SiKTongDonation()->GetEventType(kPacket_.m_dwNPCType);
    SetTodayEventDoneDiff(dwUID_, nEventType, kPacket_.m_tmVersion, kPacket_.m_dwDonateCount);
    kPacket.m_nUserAmount = GetTodayEventCount(dwUID_, nEventType, kPacket_.m_tmVersion);

    AddServerCountData(kPacket_.m_dwDonateCount, nEventType, kPacket_.m_tmVersion);
    kPacket.m_nTotalAmount = GetTotalServerCountData(kPacket_.m_tmVersion, nEventType);

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket.m_nUserAmount)
        << BUILD_LOG(kPacket.m_nTotalAmount)
        << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_TONG_DONATION_DONATE_ACK);
}

INT_IMPL_ON_FUNC_NOPARAM(EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT)
{
    Get_DungeonPersonalRecord_UserData(dwUID_, cCharType_);
}

INT_IMPL_ON_FUNC_NOPARAM(DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ)
{
    Get_DungeonRank_PreviousSeason_UserRank(dwUID_);
}

INT_IMPL_ON_FUNC(DB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ);

    KEVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nSlotID = kPacket_.m_nSlotID;

    KItem kTargetItem = kPacket_.m_kTargetItem;
    kTargetItem.m_vecAttribute.clear();

    std::vector< KAttributeInfo >::iterator vit;
    std::list< std::pair< int, float > >::iterator lit;

    SET_ERROR(ERR_UNKNOWN);

    // »õ ¼Ó¼º DB¿¡ Àû¿ë
    if (true == kPacket_.m_lstAttributeList.empty()) {
        SET_ERR_GOTO(ERR_SINGLE_RANDOM_ATTRIBUTE_07, END_PROC);
    }

    /* À¯Àú ½º·¹µå¿¡¼­ º¯°æµÉ »õ ¼Ó¼ºÀ» ¸Ç Ã³À½¿¡ ³Ö¾îµÎ¾ú´Ù°¡
       ÀÌ °÷ DB ½º·¹µå¿¡¼­ ±× ¼Ó¼º¸¸ DB¿¡ ±â·ÏÇÑ µÚ ÃÖÁ¾ ¼Ó¼º ¸®½ºÆ®(±âÁ¸+»õ)¸¦ À¯Àú ½º·¹µå¿¡ ³Ñ±â·Á°í Çß´Âµ¥
       UpdateAttribute() ÇÔ¼ö¿¡¼­ KItem ±¸Á¶Ã¼ÀÇ m_vecAttribute¿¡ Á÷Á¢ push_back ÇÏ´Â ·ÎÁ÷¿¡ ¸ÂÃß·Á´Ï ÁöÀúºÐÇØÁö´Â °Í °°´Ù
       »õ·Î ÀÛ¼ºÇÑ´Ù¸é UpdateAttribute()¿Í ºñ½ÁÇÑ µ¿ÀÛÀ» ÇÏµÇ KItem ±¸Á¶Ã¼ ´ë½Å ¼Ó¼º ¸®½ºÆ®¸¦ µû·Î ³Ñ°ÜÁÖ´Â ÇÔ¼ö¸¦ ¸¸µé¾î »ç¿ëÇÏ°Ú´Ù */
    for (vit = kPacket_.m_kTargetItem.m_vecAttribute.begin(); kPacket_.m_kTargetItem.m_vecAttribute.end() != vit; ++vit) {
        if (kPacket_.m_nSlotID != static_cast<int>(vit->m_cSlotID)) {
            kTargetItem.m_vecAttribute.push_back(*vit);
        }
    }

    lit = kPacket_.m_lstAttributeList.begin();
    if (false == m_kHelper.UpdateAttribute(dwUID_, kPacket_.m_nSlotID, lit->first, lit->second, kTargetItem, cCharType_)) {
        SET_ERR_GOTO(ERR_SINGLE_RANDOM_ATTRIBUTE_08, END_PROC);
    }

    std::sort(kTargetItem.m_vecAttribute.begin(), kTargetItem.m_vecAttribute.end(), KAttributeComp());

    kPacket.m_kTargetItem = kTargetItem;
    kPacket.m_kSingleRandomAttributeItem = kPacket_.m_kUseItem;

    // °³º° ·£´ý ¼Ó¼º ¾ÆÀÌÅÛÀÇ ¼­¹ö, DB ¼ö·® µ¿±âÈ­
    if (false == CheckItemDuration(dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount)) {
        START_LOG(clog, L"°³º° ¼Ó¼º ·£´ý ÁÖ¹®¼­ ¾ÆÀÌÅÛ DB ¼ö·® µ¿±âÈ­ ½ÇÆÐ. Name : " << strLogin_)
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_kUseItem.m_ItemUID)
            << BUILD_LOG(kPacket_.m_kUseItem.m_nInitCount)
            << BUILD_LOG(kPacket_.m_kUseItem.m_nCount)
            << END_LOG;
    }

    // °³º° ·£´ý ¼Ó¼º ¾ÆÀÌÅÛÀÇ ¼­¹ö, DB ¼ö·® µ¿±âÈ­2
    if (false == UseCountItem(dwUID_, kPacket_.m_kUseItem.m_ItemUID, 1)) {
        kPacket.m_kSingleRandomAttributeItem.m_nCount = 0;

        SET_ERR_GOTO(ERR_SINGLE_RANDOM_ATTRIBUTE_09, END_PROC);
    }

    kPacket.m_kSingleRandomAttributeItem.m_nCount -= 1;
    kPacket.m_kSingleRandomAttributeItem.m_nCount = std::max< int >(kPacket.m_kSingleRandomAttributeItem.m_nCount, 0);

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket_.m_nSlotID)
        << BUILD_LOG(kPacket.m_nOK)
        << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK);
}
