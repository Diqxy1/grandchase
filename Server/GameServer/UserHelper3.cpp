#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "CoupleSystem.h"
#include "support_User.h"
#include <boost/bind.hpp>
#include "SignBoard.h"
#include "Square.h"
#include "LoudMessage.h"
#include "Room.h"
#include "ChannelManager.h"
#include "Worker.h"
#include "GCHelper.h"
#include "GPCapsuleManager.h"
#include "RecomHelper.h"
#include "GameServer.h"
#include "GachaManager.h"
#include "MissionPack.h"
#include "PreHackingCheckManager.h"
#include "KStatPing.h"
#include "ResultManager.h"
#include "LogManager.h"
#include "Rainbow.h"
#include "CooperationEvent.h"
#include "ItemTradeManager.h"
#include "MissionEvent.h"
#include "NpcGift.h"
#include "DicePlay.h"

#include "GameDrop.h"
#include "IndigoUtil.h"
#include "GuildManager.h"
#include "SkillManager.h"
#include "IPAdvantage.h"
#include "CnConnector.h"
#include "PetHelper.h"
#include "StatDungeon.h"

#include "GSGash.h"
#include "LoginOutStat.h"
#include "FailRate.h"
#include "SpecialReward.h"
#include "ItemManager.h"
#include "EventMonsterManager.h"
#include "Lottery.h"
#include "HeroDungeonManager.h"
#include "Gawibawibo.h"
#include "Socks.h"
#include "SkillManager.h"
#include "SHCheckPoint.h"
#include "StatGP.h"
#include "GachaManager.h"
#include "VipEventManager.h"
#include "RKTornado.h"
#include "DotaManager.h"
#include "AgitHelper.h"
#include "AgentConnector.h"
#include "ShutdownUser.h"
#include "NMNetCafeManager.h"
#include "Survey.h"
#include "Escort.h"
#include "PostConfig.h"
#include "EclipseTimeEvent.h"
#include "JobMissionGuide.h"
#include "JustInTime.h"
#include "SpecialMonsterManager.h"
#include "LevelResetEvent.h"
#include "SealedChar.h"
#include "OlympicManager.h"
#include "GetLimit.h"
#include "DungeonManager.h"
#include "ResultReward.h"
#include "PVPMatchManager.h"
#include "MatchConnector.h"
#include "RitasChristmas.h"
#include "RankManager.h"
#include "StrengthManager.h"
#include "VirtualCash.h"
#include "CharPromotionSystem.h"
#include "NewDonationManager.h"
#include "EclipseCollect.h"
#include "ItemCompose.h"
#include "GachaLottery.h"
#include "ChannelTypeEvent.h"
#include "FatigueSystem.h"
#include "TongDonation.h"
#include "InfinityDungeon.h"
#include "MissionGettableCondition.h"

#define CLASS_TYPE  KUser

void KUser::InitCoupleInfo()
{
    m_kCoupleInfo.Clear();
}

bool KUser::ChkMakeCoupleState(KUser* pkUser_, int nUserState_)
{
    if (!pkUser_) return false;
    if (pkUser_->GetStateID() != nUserState_) return false;
    if (SiKCoupleSystem()->IsInCoupleReqInfo(pkUser_->GetUID())) return false;

    switch (nUserState_)
    {
    case KUserFSM::STATE_ROOM:
        _JIF(pkUser_->m_pkRoom, return false);
        break;
    case KUserFSM::STATE_SQUARE:
        _JIF(pkUser_->m_spSquare, return false);
        break;
    case KUserFSM::STATE_AGIT:
        _JIF(m_dwStayAgitUID > 0, return false);
        break;
    default:
        return false;
    };

    return true;
}

void KUser::SetCoupleDeny(DWORD dwRequester /*= 0 */)
{
    if (m_kCoupleInfo.m_bAllDeny) return; // 이미 모두 차단 이므로..
    if (dwRequester == 0)
        m_kCoupleInfo.m_bAllDeny = true;
    else
        m_kCoupleInfo.m_setDenyUsers.insert(dwRequester);

    QUEUEING_EVENT_TO_DB(EVENT_COUPLE_REQ_DENY_SET, dwRequester);
}

bool KUser::IsInCoupleInven(GCITEMID dwItemID_)
{
    std::vector<KSimpleItem>::iterator vit;
    vit = std::find_if(m_kCoupleInfo.m_vecMyInven.begin(), m_kCoupleInfo.m_vecMyInven.end(),
        boost::bind(&KSimpleItem::m_dwID, _1) == dwItemID_);
    if (vit != m_kCoupleInfo.m_vecMyInven.end()) return true;

    vit = std::find_if(m_kCoupleInfo.m_vecPartnerInven.begin(), m_kCoupleInfo.m_vecPartnerInven.end(),
        boost::bind(&KSimpleItem::m_dwID, _1) == dwItemID_);
    if (vit != m_kCoupleInfo.m_vecPartnerInven.end()) return true;

    return false;
}

bool KUser::IsInCoupleInven(std::vector<GCITEMID> vecItemIDs)
{
    std::vector<GCITEMID>::iterator vit;
    for (vit = vecItemIDs.begin(); vit != vecItemIDs.end(); ++vit)
    {
        if (!IsInCoupleInven(*vit)) return false;
    }
    return true;
}

void KUser::AddMyInven(KSimpleItem& kItem_)
{
    m_kCoupleInfo.m_vecMyInven.push_back(kItem_);
}

void KUser::AddPartnerInven(KSimpleItem& kItem_)
{
    m_kCoupleInfo.m_vecPartnerInven.push_back(kItem_);
}

bool KUser::EquipCoupleItem(GCITEMID dwItemID_, bool bPartner_ /*= false*/)
{
    KCoupleEquip kCoupleItem;
    KSimpleItem kItem;
    if (!SiKCoupleSystem()->GetCoupleItem(dwItemID_, kCoupleItem)) return false;

    std::vector<KCoupleEquip>::iterator vit;
    bool bEquip = false;
    if (bPartner_)
    {
        if (!FindItemFromCoupleInven(kCoupleItem.m_dwItemID, m_kCoupleInfo.m_vecPartnerInven, kItem))
        {
            if (!FindItemFromCoupleInven(kCoupleItem.m_dwItemID, m_kCoupleInfo.m_vecMyInven, kItem))
                return false;
        }
        kCoupleItem.m_dwItemUID = kItem.m_dwUID;

        // 루프 돌면서 동일 슬롯 동일 캐릭이면... 빼고 넣는다.. 아니면. 그냥 인서트 한다.
        for (vit = m_kCoupleInfo.m_vecPartnerEquip.begin(); vit != m_kCoupleInfo.m_vecPartnerEquip.end(); ++vit)
        {
            if (vit->m_cEquipSlot == kCoupleItem.m_cEquipSlot)
            {
                *vit = kCoupleItem;
                bEquip = true;
                break;
            }
        }

        if (!bEquip)
            m_kCoupleInfo.m_vecPartnerEquip.push_back(kCoupleItem);
    }
    else
    {
        if (!FindItemFromCoupleInven(kCoupleItem.m_dwItemID, m_kCoupleInfo.m_vecPartnerInven, kItem))
        {
            if (!FindItemFromCoupleInven(kCoupleItem.m_dwItemID, m_kCoupleInfo.m_vecMyInven, kItem))
                return false;
        }
        kCoupleItem.m_dwItemUID = kItem.m_dwUID;

        for (vit = m_kCoupleInfo.m_vecMyEquip.begin(); vit != m_kCoupleInfo.m_vecMyEquip.end(); ++vit)
        {
            if (vit->m_cEquipSlot == kCoupleItem.m_cEquipSlot)
            {
                *vit = kCoupleItem;
                bEquip = true;
                break;
            }
        }

        if (!bEquip)
            m_kCoupleInfo.m_vecMyEquip.push_back(kCoupleItem);
    }

    return true;
}

bool KUser::EquipCoupleItem(std::vector<GCITEMID>& vecItemIDs, bool bPartner /*= false*/)
{
    if (bPartner)
    {
        ClearEquipExRing(m_kCoupleInfo.m_vecPartnerEquip);
    }
    else
    {
        ClearEquipExRing(m_kCoupleInfo.m_vecMyEquip);
    }
    std::vector<GCITEMID>::iterator vit;
    for (vit = vecItemIDs.begin(); vit != vecItemIDs.end(); ++vit)
    {
        if (SiKCoupleSystem()->IsRingItem(*vit))
            continue;

        EquipCoupleItem(*vit, bPartner);
    }
    return true;
}

void KUser::MakeCoupleInfomation(KEVENT_COUPLE_INFORMATION_ACK& kData_)
{
    if (m_kCoupleInfo.m_dwCoupleState != KCoupleSystem::CS_COUPLE) return;

    if (m_kCoupleInfo.m_tmRegDate < m_kCoupleInfo.m_tmPartnerRegDate)
    {
        // 내가 신청자.
        kData_.m_dwRequesterUID = GetUID();
        kData_.m_dwRequesteeUID = GetPartnerUID();
        kData_.m_strRequester = m_strNickName;
        kData_.m_strRequestee = m_kCoupleInfo.m_strPartnerNick;
        kData_.m_cRequesterChar = m_kCoupleInfo.m_cCharType;
        kData_.m_cRequesteeChar = m_kCoupleInfo.m_cPartnerCharType;
        kData_.m_tmRegDate = m_kCoupleInfo.m_tmRegDate;
        kData_.m_vecRequesterEquip = m_kCoupleInfo.m_vecMyEquip;
        kData_.m_vecRequesteeEquip = m_kCoupleInfo.m_vecPartnerEquip;
    }
    else
    {
        // 상대방이 신청자.
        kData_.m_dwRequesterUID = GetPartnerUID();
        kData_.m_dwRequesteeUID = GetUID();
        kData_.m_strRequestee = m_strNickName;
        kData_.m_strRequester = m_kCoupleInfo.m_strPartnerNick;
        kData_.m_cRequesterChar = m_kCoupleInfo.m_cPartnerCharType;
        kData_.m_cRequesteeChar = m_kCoupleInfo.m_cCharType;
        kData_.m_tmRegDate = m_kCoupleInfo.m_tmPartnerRegDate;
        kData_.m_vecRequesteeEquip = m_kCoupleInfo.m_vecMyEquip;
        kData_.m_vecRequesterEquip = m_kCoupleInfo.m_vecPartnerEquip;
    }
}

bool KUser::IsCouple()
{
    return (m_kCoupleInfo.m_dwCoupleState == KCoupleSystem::CS_COUPLE);
}

void KUser::AdjustCoupleData(KCoupleInfo& kInfo_)
{
    m_kCoupleInfo.m_dwUID = kInfo_.m_dwUID;
    m_kCoupleInfo.m_cCharType = kInfo_.m_cCharType;
    m_kCoupleInfo.m_tmRegDate = kInfo_.m_tmRegDate;
    m_kCoupleInfo.m_vecMyInven = kInfo_.m_vecMyInven;
    m_kCoupleInfo.m_vecMyEquip = kInfo_.m_vecMyEquip;
    m_kCoupleInfo.m_dwPartnerUID = kInfo_.m_dwPartnerUID;
    m_kCoupleInfo.m_strPartnerNick = kInfo_.m_strPartnerNick;
    m_kCoupleInfo.m_strPartnerNickColor = kInfo_.m_strPartnerNickColor;
    m_kCoupleInfo.m_cPartnerCharType = kInfo_.m_cPartnerCharType;
    m_kCoupleInfo.m_tmPartnerRegDate = kInfo_.m_tmPartnerRegDate;
    m_kCoupleInfo.m_vecPartnerInven = kInfo_.m_vecPartnerInven;
    m_kCoupleInfo.m_vecPartnerEquip = kInfo_.m_vecPartnerEquip;
    m_kCoupleInfo.m_vecCoupleInven = kInfo_.m_vecCoupleInven;
    //m_kCoupleInfo.m_setDenyUsers            = kInfo_.m_setDenyUsers
    //m_kCoupleInfo.m_bAllDeny                = kInfo_.m_bAllDeny
    m_kCoupleInfo.m_dwCoupleState = KCoupleSystem::CS_COUPLE;



    // 상태, 거절 정보는 갱신 하지 않는다.
}

void KUser::GetCoupleEquip(std::vector<GCITEMID>& vecCoupleEquip_)
{
    vecCoupleEquip_.clear();
    std::vector<KCoupleEquip>::iterator vit;
    for (vit = m_kCoupleInfo.m_vecMyEquip.begin(); vit != m_kCoupleInfo.m_vecMyEquip.end(); ++vit)
    {
        vecCoupleEquip_.push_back(vit->m_dwItemID);
    }
}

void KUser::CoupleBoostCalc(IN OUT float& fExpBoost_, IN OUT float& fGpBoost_)
{
    if (GetCoupleState() != KCoupleSystem::CS_COUPLE) return;

    if (!ValidateCoupleInfo()) {
        START_LOG(cerr, L"커플 데이터가 유효하지 않다." << m_strName)
            << BUILD_LOG(m_kCoupleInfo.m_strPartnerNick)
            << END_LOG;
        return;
    }

    // 기념일이면
    if (SiKCoupleSystem()->IsMemorialDay(GetCoupleRegDate()))
    {
        fExpBoost_ += std::max<float>(0.f, SiKCoupleSystem()->GetMemorialExp());
        fGpBoost_ += std::max<float>(0.f, SiKCoupleSystem()->GetMemorialGp());

        START_LOG(clog, L"커플 기념일 추가 보너스 Name : " << m_strName)
            << BUILD_LOG(SiKCoupleSystem()->GetMemorialExp())
            << BUILD_LOG(SiKCoupleSystem()->GetMemorialGp()) << END_LOG;

        // 기념일 버프를 받는 경우 반지로 인한 추가 보너스를 받지 않는다
        return;
    }

    JIF(m_pkRoom);

    // 같은팀일 경우에만 보너스를 받는다(던전/대전 모두 확인)
    if (!m_pkRoom->IsSameTeam(GetUID(), GetPartnerUID())) return;

    std::vector<KCoupleEquip>::iterator vit;
    for (vit = m_kCoupleInfo.m_vecMyEquip.begin(); vit != m_kCoupleInfo.m_vecMyEquip.end(); ++vit)
    {
        if (SiKCoupleSystem()->IsRingItem(vit->m_dwItemID))
        {
            SCoupleBenefit kBenefit;
            if (SiKCoupleSystem()->GetBenefitInfo(vit->m_dwItemID, kBenefit))
            {
                fExpBoost_ += std::max<float>(0.f, kBenefit.m_fExpRatio);
                fGpBoost_ += std::max<float>(0.f, kBenefit.m_fGPRatio);

                START_LOG(clog, L"커플 반지로 인한 추가 보너스 Name : " << m_strName)
                    << BUILD_LOG(vit->m_dwItemID)
                    << BUILD_LOG(kBenefit.m_fExpRatio)
                    << BUILD_LOG(kBenefit.m_fGPRatio) << END_LOG;
            }
            break;
        }
    }
}

void KUser::ChangeRing(std::vector<KCoupleEquip>& vecCoupleEquip_, GCITEMID dwItemID_, GCITEMUID dwUID_)
{
    std::vector<KCoupleEquip>::iterator vit;
    for (vit = vecCoupleEquip_.begin(); vit != vecCoupleEquip_.end(); ++vit)
    {
        if (SiKCoupleSystem()->IsRingItem(vit->m_dwItemID))
        {
            vit->m_dwItemID = dwItemID_;
            vit->m_dwItemUID = dwUID_;
            return;
        }
    }

    KCoupleEquip kEquip;
    if (SiKCoupleSystem()->GetCoupleItem(dwItemID_, kEquip))
    {
        kEquip.m_dwItemUID = dwUID_;
        vecCoupleEquip_.push_back(kEquip);
    }
}

void KUser::GetCoupleChangeEquip(std::vector<KCoupleEquip>& vecDelEquip_, std::vector<KCoupleEquip>& vecUpdateEquip_)
{

    vecDelEquip_.clear();
    vecUpdateEquip_.clear();

    std::vector<KCoupleEquip>::iterator vitInit;
    std::vector<KCoupleEquip>::iterator vitCurrent;
    for (vitInit = m_kInitEquip.begin(); vitInit != m_kInitEquip.end(); ++vitInit)
    {
        vitCurrent = std::find_if(m_kCoupleInfo.m_vecMyEquip.begin(), m_kCoupleInfo.m_vecMyEquip.end(),
            boost::bind(&KCoupleEquip::m_cEquipSlot, _1) == vitInit->m_cEquipSlot);

        if (vitCurrent == m_kCoupleInfo.m_vecMyEquip.end())
        {
            if (vitInit->m_cEquipSlot == 0)
            {
                START_LOG(cerr, L"왜 장착에서 빠졌나? Name : " << m_strName)
                    << BUILD_LOG(vitInit->m_dwItemID)
                    << BUILD_LOG(vitInit->m_dwItemUID)
                    << BUILD_LOGc(vitInit->m_cEquipSlot) << END_LOG;
                continue;
            }

            vecDelEquip_.push_back(*vitInit);
            continue;
        }

        if (vitInit->m_dwItemID != vitCurrent->m_dwItemID)
        {
            vecUpdateEquip_.push_back(*vitCurrent);
        }
    }

    for (vitCurrent = m_kCoupleInfo.m_vecMyEquip.begin(); vitCurrent != m_kCoupleInfo.m_vecMyEquip.end(); ++vitCurrent)
    {
        vitInit = std::find_if(m_kInitEquip.begin(), m_kInitEquip.end(),
            boost::bind(&KCoupleEquip::m_cEquipSlot, _1) == vitCurrent->m_cEquipSlot);
        if (vitInit == m_kInitEquip.end())
        {
            vecUpdateEquip_.push_back(*vitCurrent);
            continue;
        }
    }
}

void KUser::CleanupCoupleData()
{
    SRequestInfo kReqData;
    if (SiKCoupleSystem()->GetCoupleRequestInfo(GetUID(), kReqData))
    {
        SiKCoupleSystem()->DeleteCoupleRequestInfo(kReqData.m_dwRequestee, kReqData.m_dwRequester);

        KEVENT_MAKE_COUPLE_ACK kPacket;
        DWORD dwUserUID;
        if (GetUID() == kReqData.m_dwRequestee)
        {
            dwUserUID = kReqData.m_dwRequester;

            if (kReqData.m_nState == KCoupleSystem::MCS_REQ)
            {
                kPacket.m_nOK = 4;
                SiKGSSimLayer()->m_kActorManager.SendTo(dwUserUID, KUserEvent::EVENT_MAKE_COUPLE_ACK, kPacket);
            }
        }
        else
        {
            dwUserUID = kReqData.m_dwRequestee;
            if (kReqData.m_nState == KCoupleSystem::MCS_DB_WORK)
            {
                kPacket.m_nOK = 4;
                SiKGSSimLayer()->m_kActorManager.SendTo(dwUserUID, KUserEvent::EVENT_MAKE_COUPLE_ACK, kPacket);
            }
        }
    }

    if (SiKCoupleSystem()->GetRingRequestInfo(GetUID(), kReqData))
    {
        SiKCoupleSystem()->DeleteRingRequestInfo(kReqData.m_dwRequestee, kReqData.m_dwRequester);

        KEVENT_MAKE_COUPLE_ACK kPacket;
        DWORD dwUserUID;
        if (GetUID() == kReqData.m_dwRequestee)
        {
            dwUserUID = kReqData.m_dwRequester;

            if (kReqData.m_nState == KCoupleSystem::MCS_REQ)
            {
                kPacket.m_nOK = 6;
                SiKGSSimLayer()->m_kActorManager.SendTo(dwUserUID, KUserEvent::EVENT_CHANGE_COUPLE_RING_ACK, kPacket);
            }
        }
        else
        {
            dwUserUID = kReqData.m_dwRequestee;
            if (kReqData.m_nState == KCoupleSystem::MCS_DB_WORK)
            {
                kPacket.m_nOK = 6;
                SiKGSSimLayer()->m_kActorManager.SendTo(dwUserUID, KUserEvent::EVENT_CHANGE_COUPLE_RING_ACK, kPacket);
            }
        }
    }

    if (IsCouple() && m_nCoupleWindowState != 0)
    {
        m_nCoupleWindowState = 0;
        SiKGSSimLayer()->m_kActorManager.SendTo(GetPartnerUID(), KUserEvent::EVENT_COUPLE_WINDOW_STATE_NOT, m_nCoupleWindowState);
    }
}

void KUser::SendCoupleNotToSquareUser(SCoupleSignBoard& sBoardInfo_)
{
    if (!m_spSquare) return;
    KEVENT_MAKE_COUPLE_NOT kPacket;

    kPacket.m_dwUID1 = GetUID();
    kPacket.m_strNick1 = m_strNickName;
    kPacket.m_dwUID2 = GetPartnerUID();
    kPacket.m_strNick2 = m_kCoupleInfo.m_strPartnerNick;
    GetCoupleEquip(kPacket.m_vecCoupleEquip);
    m_spSquare->SendToAll(KUserEvent::EVENT_MAKE_COUPLE_NOT, kPacket);


    KSignBoardData kData;
    kData.m_dwColor = sBoardInfo_.m_dwFontColor;
    kData.m_ItemID = sBoardInfo_.m_dwSignBoardID; // 사랑 하트 전광판
    kData.m_dwSenderUID = GetUID();
    kData.m_eType = sBoardInfo_.m_nType;
    kData.m_strSenderNick.clear();

    if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG))
    {
        START_LOG(cwarn, L"SendCoupleNotToSquareUser")
            << BUILD_LOG(kData.m_dwColor)
            << BUILD_LOG(kData.m_ItemID)
            << BUILD_LOG(kData.m_dwSenderUID)
            << BUILD_LOG(kData.m_eType)
            << BUILD_LOG(m_strNickName) << END_LOG;
    }

    if (SiKLoudMessage()->GetCoupleMsg(m_strNickName, m_kCoupleInfo.m_strPartnerNick, kData.m_strMsg, GetLanguageCode()))
    {
        SiKSignBoard()->QueueingAdminData(kData);
        START_LOG(cwarn, L"커플 메세지 전송함.")
            << BUILD_LOG(m_strNickName)
            << BUILD_LOG(m_kCoupleInfo.m_strPartnerNick)
            << BUILD_LOG(kData.m_strMsg) << END_LOG;
    }
    else
    {
        START_LOG(cwarn, L"커플 메세지를 가져올 수 없음.")
            << BUILD_LOG(m_strNickName)
            << BUILD_LOG(m_kCoupleInfo.m_strPartnerNick)
            << BUILD_LOG(kData.m_strMsg) << END_LOG;
    }

    SiKChannelManager()->SendToAll(KUserEvent::EVENT_CHAT_NOT, kData.m_strMsg.c_str());
}

bool KUser::IsInDenyList(DWORD dwUID_)
{
    if (m_kCoupleInfo.m_bAllDeny) return true;
    return (m_kCoupleInfo.m_setDenyUsers.find(dwUID_) != m_kCoupleInfo.m_setDenyUsers.end());
}

void KUser::CoupleDivorceNotityToOther(KUser* pkUser_, std::pair<DWORD, DWORD> prData_)
{
    if (!pkUser_) return;
    if (pkUser_->GetStateID() == KUserFSM::STATE_ROOM && pkUser_->m_pkRoom)
    {
        pkUser_->m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL_EX_ME, pkUser_->m_strName, KUserEvent::EVENT_DIVORCE_COUPLE_OTHER_NOT, prData_);
    }
    else if (pkUser_->GetStateID() == KUserFSM::STATE_SQUARE && pkUser_->m_spSquare)
    {
        pkUser_->m_spSquare->SendToAllExMe(pkUser_->GetUID(), KUserEvent::EVENT_DIVORCE_COUPLE_OTHER_NOT, prData_);
    }
}

void KUser::AdjustCoupleEquipExpired()
{
    std::vector<KCoupleEquip>::iterator vit;
    vit = m_kCoupleInfo.m_vecMyEquip.begin();
    while (vit != m_kCoupleInfo.m_vecMyEquip.end())
    {
        if (!m_kInventory.FindItemByItemUID(vit->m_dwItemUID))
        {
            vit = m_kCoupleInfo.m_vecMyEquip.erase(vit);
            continue;
        }
        ++vit;
    }
}


void KUser::ConnectionExpiredCoupleItem()
{
    std::vector<KSimpleItem> vecExpiredItems;
    vecExpiredItems.clear();
    std::vector<KSimpleItem>::iterator vit;
    for (vit = m_kCoupleInfo.m_vecMyInven.begin(); vit != m_kCoupleInfo.m_vecMyInven.end(); ++vit)
    {
        if (!SiKCoupleSystem()->IsCoupleItem(vit->m_dwID)) continue;

        if (SiKCoupleSystem()->IsRingItem(vit->m_dwID)) continue;

        if (!m_kInventory.FindItemByItemUID(vit->m_dwUID))
        {
            vecExpiredItems.push_back(*vit);
        }
    }

    DumpCoupleInven();
    if (!vecExpiredItems.empty())
    {
        DumpExpiredCoupleItem(vecExpiredItems, m_strName);
        for (vit = vecExpiredItems.begin(); vit != vecExpiredItems.end(); ++vit)
        {
            RemoveCoupleMyInven(vit->m_dwUID);
            RemoveCoupleMyEquip(vit->m_dwUID);
            RemoveCoupleInven(vit->m_dwUID);
        }

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(GetPartnerUID());
        if (spUser)
        {
            spUser->m_kCoupleInfo.m_vecPartnerEquip = m_kCoupleInfo.m_vecMyEquip;
            spUser->m_kCoupleInfo.m_vecPartnerInven = m_kCoupleInfo.m_vecMyInven;
            //패킷 보낸다.
            spUser->SendPacket(KUserEvent::EVENT_COUPLEITEM_EXPIRED_NOT, vecExpiredItems);
        }
        QUEUEING_EVENT_TO_DB(EVENT_COUPLEITEM_EXPIRED_NOT, vecExpiredItems);
    }
    DumpCoupleInven();
}

void KUser::ChkCoupleItemExpired(std::vector<KItem>& vecExpiredItems_)
{
    std::vector<KSimpleItem> vecExpired;
    std::vector<KItem>::iterator vit;
    for (vit = vecExpiredItems_.begin(); vit != vecExpiredItems_.end(); ++vit)
    {
        if (!SiKCoupleSystem()->IsCoupleItem(vit->m_ItemID)) continue;

        if (RemoveCoupleMyInven(vit->m_ItemUID))
        {
            RemoveCoupleMyEquip(vit->m_ItemUID);
            KSimpleItem kItem;
            kItem.m_dwID = vit->m_ItemID;
            kItem.m_dwUID = vit->m_ItemUID;
            vecExpired.push_back(kItem);
        }
    }

    if (!vecExpired.empty())
    {
        DumpExpiredCoupleItem(vecExpired, m_strName);
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(GetPartnerUID());
        if (spUser)
        {
            spUser->m_kCoupleInfo.m_vecPartnerEquip = m_kCoupleInfo.m_vecMyEquip;
            spUser->m_kCoupleInfo.m_vecPartnerInven = m_kCoupleInfo.m_vecMyInven;
            //패킷 보낸다.
            spUser->SendPacket(KUserEvent::EVENT_COUPLEITEM_EXPIRED_NOT, vecExpired);
        }

        QUEUEING_EVENT_TO_DB(EVENT_COUPLEITEM_EXPIRED_NOT, vecExpired);
    }
}

bool KUser::RemoveCoupleMyInven(GCITEMUID itemUID_)
{
    std::vector<KSimpleItem>::iterator vit;
    vit = std::find_if(m_kCoupleInfo.m_vecMyInven.begin(), m_kCoupleInfo.m_vecMyInven.end(),
        boost::bind(&KSimpleItem::m_dwUID, _1) == itemUID_);
    if (vit == m_kCoupleInfo.m_vecMyInven.end()) return false;
    m_kCoupleInfo.m_vecMyInven.erase(vit);
    return true;
}

bool KUser::RemoveCoupleMyEquip(GCITEMUID itemUID_)
{
    std::vector<KCoupleEquip>::iterator vit;
    vit = std::find_if(m_kCoupleInfo.m_vecMyEquip.begin(), m_kCoupleInfo.m_vecMyEquip.end(),
        boost::bind(&KCoupleEquip::m_dwItemUID, _1) == itemUID_);
    if (vit == m_kCoupleInfo.m_vecMyEquip.end()) return false;
    m_kCoupleInfo.m_vecMyEquip.erase(vit);
    return true;
}

bool KUser::FindItemFromCoupleInven(GCITEMID dwItemID_, std::vector<KSimpleItem>& vecInven_, KSimpleItem& kItem_)
{
    std::vector<KSimpleItem>::iterator vit;
    vit = std::find_if(vecInven_.begin(), vecInven_.end(),
        boost::bind(&KSimpleItem::m_dwID, _1) == dwItemID_);

    if (vit == vecInven_.end()) return false;
    kItem_ = *vit;
    return true;
}

void KUser::ClearEquipExRing(std::vector<KCoupleEquip>& vecEquips_)
{
    vecEquips_.erase(std::remove_if(vecEquips_.begin(), vecEquips_.end(),
        boost::bind(&KCoupleEquip::m_cEquipSlot, _1) > 0),
        vecEquips_.end());
}

void KUser::DumpCoupleInfo()
{
    //CTime tmRegMy( m_kCoupleInfo.m_tmRegDate );
    //CTime tmRegPartner( m_kCoupleInfo.m_tmPartnerRegDate);
    //std::Warn << L"-- Dump Couple Info -- " << dbg::endl
    //    << L" Name : " << m_strName << dbg::endl
    //    << L" Nick : " << m_strNickName << dbg::endl
    //    << L" UID : " << m_kCoupleInfo.m_dwUID << dbg::endl
    //    << L" CharType : " << (int)m_kCoupleInfo.m_cCharType << dbg::endl
    //    << L" RegDate : " << (LPCWSTR)tmRegMy.Format( KNC_TIME_FORMAT ) << dbg::endl
    //    << L" Inven Size : " << m_kCoupleInfo.m_vecMyInven.size() << dbg::endl
    //    << L" Equip Size : " << m_kCoupleInfo.m_vecMyEquip.size() << dbg::endl
    //    << L" PUID : " << GetPartnerUID() << dbg::endl
    //    << L" PNick : " << m_kCoupleInfo.m_strPartnerNick << dbg::endl
    //    << L" PCharType : " << (int)m_kCoupleInfo.m_cPartnerCharType << dbg::endl
    //    << L" PRegDate : " << (LPCWSTR)tmRegPartner.Format( KNC_TIME_FORMAT ) << dbg::endl
    //    << L" PEquip Size : " << m_kCoupleInfo.m_vecPartnerEquip.size() << dbg::endl
    //    << L"-- End Dump Couple Info -- " << dbg::endl << dbg::endl;
}

time_t KUser::GetCoupleRegDate()
{
    return (m_kCoupleInfo.m_tmRegDate > m_kCoupleInfo.m_tmPartnerRegDate ? m_kCoupleInfo.m_tmPartnerRegDate : m_kCoupleInfo.m_tmRegDate);
}

void KUser::DumpCoupleEquip(std::vector<KCoupleEquip>& vecEquip_, std::wstring& strLogin_)
{
    //START_LOG( cwarn, L" --- Dump Equip ---- " << strLogin_  ) << END_LOG;

    //std::vector<KCoupleEquip>::iterator vit;
    //for( vit = vecEquip_.begin() ; vit != vecEquip_.end() ; ++vit )
    //{
    //    std::Warn << L"[" << vit->m_dwItemID << L", "
    //        << vit->m_dwItemUID << L", "
    //        << (int)vit->m_cEquipSlot << L"]";
    //}

    //START_LOG( cwarn, ENDL << L" --- Dump Equip END ---- "  ) << END_LOG;
}

void KUser::DumpExpiredCoupleItem(std::vector<KSimpleItem>& vecExpiredItems_, std::wstring& strLogin_)
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    //START_LOG( cwarn, L" --- Dump ExpiredItems ---- " << strLogin_  ) << END_LOG;
    //std::vector<KSimpleItem>::iterator vit;
    //for( vit = vecExpiredItems_.begin() ; vit != vecExpiredItems_.end() ; ++vit )
    //{
    //    std::Warn << L"[" << vit->m_dwID << L", " << vit->m_dwUID << L"]";
    //}
    //START_LOG( cwarn, ENDL << L" --- Dump ExpiredItems END ---- "  ) << END_LOG;
}

void KUser::DumpCoupleInven()
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    //START_LOG( cwarn, L" --- Dump CoupleInven ---- " << m_strName  ) << END_LOG;
    //std::vector<KSimpleItem>::iterator vit;
    //for( vit = m_kCoupleInfo.m_vecMyInven.begin() ; vit != m_kCoupleInfo.m_vecMyInven.end() ; ++vit )
    //{
    //    std::Warn << L"[" << vit->m_dwID << L", " << vit->m_dwUID << L"] ";
    //}
    //START_LOG( cwarn, ENDL << L" ------------ "  ) << END_LOG;
    //for( vit = m_kCoupleInfo.m_vecPartnerInven.begin() ; vit != m_kCoupleInfo.m_vecPartnerInven.end() ; ++vit )
    //{
    //    std::Warn << L"[" << vit->m_dwID << L", " << vit->m_dwUID << L"] ";
    //}
    //START_LOG( cwarn, ENDL << L" --- Dump CoupleInven END ---- "  ) << END_LOG;

}
void KUser::ClearCoupleDenyInfo()
{
    if (!IsCouple()) return;
    m_kCoupleInfo.m_bAllDeny = false;
    m_kCoupleInfo.m_setDenyUsers.clear();
}

void KUser::CharacterCachingDataSync()
{
    std::map<char, KCharacterInfo>::iterator mit;
    for (mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit)
    {
        LIF(mit->second.m_biInitExp <= mit->second.m_biExp);
        mit->second.m_biInitExp = std::max<__int64>(mit->second.m_biInitExp, mit->second.m_biExp);
        mit->second.m_iInitWin = mit->second.m_iWin;
        mit->second.m_iInitLose = mit->second.m_iLose;
        mit->second.m_biInitTotalExp = std::max<__int64>(mit->second.m_biInitTotalExp, mit->second.m_biTotalExp);
    }
}

//void KUser::AddRelayLog( std::vector<DWORD>& kPacket_, DWORD dwUID_ )
//{
//    if( SiKGSSimLayer()->IsEnableRelayDataStat() == false ) // default : true , 실시간 변경 가능
//        return;
//
//    kPacket_.push_back( dwUID_ ); // 자신의 UID 를 더한다
//
//    JIF( !kPacket_.empty() );
//
//    std::vector<DWORD> vecRoomUserUIDs; // 방에 있는 유저들 UID
//
//    if ( m_pkRoom ) {
//        m_pkRoom->GetUserUIDList( vecRoomUserUIDs );
//    }
//    else {
//        START_LOG( cerr, L"AddRelayLog 룸 없음, UserUID :" << dwUID_ ) << END_LOG;
//        return;
//    }
//
//    // 두 벡터를 정렬하고  방에서 가져온 UID 벡터와 유저가 보낸 벡터가 동일한지 검사
//    
//    std::sort( vecRoomUserUIDs.begin(), vecRoomUserUIDs.end() );
//    std::sort( kPacket_.begin(), kPacket_.end() );
//
//    if( vecRoomUserUIDs != kPacket_ )
//    {
//        // File Log
//        SiKGSSimLayer()->AddRelayDataStat( kPacket_ );
//    }
//}

void KUser::SendChatLogNot(IN const std::wstring& strChannel_, IN const int nRoomID_, IN const std::wstring& strMsg_)
{
    if (SiKGCHelper()->GetChatRecordEnable() == false)
    {
        START_LOG(clog, L"채팅기록 안남김(기능OFF)") << END_LOG;
        return;
    }

    if (SiKGCHelper()->IsChatRecordUserUID(GetUID()) == false)
    {
        START_LOG(clog, L"채팅기록 대상유저가 아님.") << END_LOG;
        return;
    }

    int nCurrentQueSize = KTThreadManager<KWorker>::GetInstance()->GetQueueSize();
    int nLimitQueSize = SiKGCHelper()->GetChatLogQueLimit();

    if (nCurrentQueSize > nLimitQueSize)
    {
        START_LOG(cerr, L"채팅로그 보내기 실패. 채팅로그 큐 사이즈 한계치를 넘었음")
            << BUILD_LOG(m_strName)
            << BUILD_LOG(nCurrentQueSize)
            << BUILD_LOG(nLimitQueSize) << END_LOG;
        return;
    }

    KChatLog kPcaket;
    kPcaket.m_dwUID = GetUID();
    kPcaket.m_strNick = m_strNickName;
    kPcaket.m_strChannel = strChannel_;
    kPcaket.m_nRoomID = nRoomID_;
    kPcaket.m_strMsg = strMsg_;
    kPcaket.m_cCharType = m_cCurrentCharacter;
    kPcaket.m_tmTime = KncUtil::TimeToInt(CTime::GetCurrentTime());

    KTThreadManager<KWorker>::GetInstance()->QueueingEvent(KUserEvent::EVENT_CHAT_LOG_NOT, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPcaket);
}

void KUser::DumpAutoMission(IN const std::wstring& strFunc_, IN const std::vector<DWORD>& vecAuto_)
{
    if (vecAuto_.empty()) return;
    std::wstringstream stm;
    std::copy(vecAuto_.begin(), vecAuto_.end(),
        std::ostream_iterator<DWORD, wchar_t>(stm, L","));
    START_LOG(cwarn, L"AutoMission, Name : " << m_strName)
        << BUILD_LOG(strFunc_)
        << BUILD_LOG(stm.str()) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}
void KUser::AddStatisticsPing(KStatPingInfo& kStatPingInfo_)
{
    KStatPingPtr spStat = GET_STAT(KStatPing, SI_STAT_PING_STAT);
    if (spStat) {
        spStat->AddStat(kStatPingInfo_);
    }
}

void KUser::SendCapsuleListNot()
{
    if (SiKGPCapsuleManager()->GetGPCapsuleEnable() == false) {
        return;
    }

    KEVENT_CAPSULE_LIST_NOT kCapsuleList;

    SiKGPCapsuleManager()->GetCapsuleListNotice(kCapsuleList);
    START_LOG(clog, L"GP Capsule list Notice. size : " << kCapsuleList.size()) << END_LOG;

    SEND_COMPRESS_PACKET(EVENT_CAPSULE_LIST_NOT, kCapsuleList);
}

// 09.04.10. kkurrung. 추천 (5th))
void KUser::UpdateRecom5th(IN const KSimpleDate& kAttendDate_)
{
    // 이벤트 기간인가?
    if (!SiKRecomHelper()->IsLimitEventTerm()) return;

    // 이벤트 혜택을 받는 유저
    if (m_MyRecomInfo.m_nState < KRecomHelper::RBS_ACCEPT_RECOM) return;

    // 출석 날짜가 연계 기간인가?
    time_t tmAttend = KncUtil::TimeToInt(CTime(kAttendDate_.m_sYear, kAttendDate_.m_cMonth, kAttendDate_.m_cDay, 0, 0, 0));
    if (m_MyRecomInfo.m_tmBeginDate > tmAttend || m_MyRecomInfo.m_tmEndDate <= tmAttend) return;

    // 이벤트 기간에 첫 접속이었다면 신규인 상태로...
    bool bWasNewUser = SiKRecomHelper()->IsEventTerm(KncUtil::TimeToInt(m_tmFirstLoginTime));

    // 첫 출석인지 그냥 출석인지 DB T 에 가서 체크 하자..
    QUEUEING_EVENT_TO_DB(EVENT_RECOM_DAILY_ATTENDANCE_REQ, bWasNewUser);
}
void KUser::GetOnlineFriendsList(OUT std::vector<DWORD>& vecFriends_)
{
    vecFriends_ = m_vecOnlineFriends;
}

bool KUser::FindInDoorUserInfo(IN DWORD dwUID_)
{ // woosh가 만들었는데, 함수이름과 기능이 다르다. 만들 때도 알고 있었을 거야
    _JIF(m_pkRoom, return false);

    const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo(dwUID_);

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(dwUID_);
    //bool bIsNewUser = spUser->IsNewUserAfterEventDate();
    _JIF(spUser, return false);

    if (pkInDoorUserInfo && (spUser->IsNewUserAfterEventDate() || IsNewUserAfterEventDate()))
    { //  090413  woosh.  친구가 신규거나, 내가 신규일 때  참.
        return true;
    }
    else
    {
        return false;
    }
}

bool KUser::IsNewUserAfterEventDate()
{
    if (SiKGSSimLayer()->IsNewUserAfterFriendEvent(m_tmFirstLoginTime))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void KUser::CheckGachaReelPointReq()
{
    // 가챠 릴시스템 사용시.
    if (SiKGachaManager()->IsReelPointEnable() == false) {
        return;
    }

    QUEUEING_ID_TO_DB(EVENT_CHECK_GACHA_REEL_POINT_REQ);
}

void KUser::UpdateGachaReelPointToDB()
{
    // 가챠 릴시스템 사용시. RP저장
    if (SiKGachaManager()->IsReelPointEnable() == false) {
        return;
    }

    // 변경된 릴포인트 정보만 DB에 기록하도록 한다
    QUEUEING_EVENT_TO_DB(EVENT_GACHA_REEL_POINT_UPDATE_DB, m_mapGachaReelPointDiff);
}

void KUser::CheckGCClubConnectBonus(int nSessionInfo_)
{
    if (SiKGCHelper()->GetGCClubExtensionEnable() == false)
    {
        START_LOG(clog, L"GC클럽 확장기능 사용하지 않음.") << END_LOG;
        return;
    }

    if (nSessionInfo_ != SI_NEW_ACCOUNT &&
        nSessionInfo_ != SI_TODAY_FIRST_LOGIN_NO_LAS &&
        nSessionInfo_ != SI_TODAY_FIRST_LOGIN_LAS)
    {
        START_LOG(clog, L"최초 접속이 아님.") << END_LOG;
        return;
    }

    if (IsPremiumUser() == false)
    {
        START_LOG(clog, L"GC클럽 아이템이 없음.") << END_LOG;
        return;
    }

    // 보너스 아이템목록 받아오기
    std::vector<KDropItemInfo> vecBonusItemList;
    SiKGCHelper()->GetGCClubConnectBonusItem(vecBonusItemList);

    if (vecBonusItemList.empty())
    {
        START_LOG(clog, L"보너스 아이템이 없음.") << END_LOG;
        return;
    }

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_GET_GCCLUB_CONNECT_BONUS_REQ, vecBonusItemList);

END_PROC:
    START_LOG(clog, L"보너스 아이템 size : " << vecBonusItemList.size()) << END_LOG;
}

void KUser::CheckGCClubmapDayAttendBonus()
{
    if (SiKGCHelper()->GetGCClubExtensionEnable() == false)
    {
        START_LOG(clog, L"GC클럽 확장기능 사용하지 않음.") << END_LOG;
        return;
    }

    if (IsPremiumUser() == false)
    {
        START_LOG(clog, L"GC클럽 아이템이 없음.") << END_LOG;
        return;
    }

    // 보너스 아이템목록 받아오기
    std::vector<KDropItemInfo> vecBonusItemList;
    SiKGCHelper()->GetGCClubDayAttendBonusItem(vecBonusItemList);

    if (vecBonusItemList.empty())
    {
        START_LOG(clog, L"보너스 아이템이 없음.") << END_LOG;
        return;
    }

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_GET_GCCLUB_ATTEND_BONUS_REQ, vecBonusItemList);

END_PROC:
    START_LOG(clog, L"보너스 아이템 size : " << vecBonusItemList.size()) << END_LOG;
}

void KUser::SendMissionPackListNot()
{
    KEVENT_MISSION_PACK_LIST_NOT kMissionList;

    SiKMissionPack()->GetMissionList(kMissionList);
    START_LOG(clog, L"Mission Package list Notice. size : " << kMissionList.size()) << END_LOG;

    SEND_COMPRESS_PACKET(EVENT_MISSION_PACK_LIST_NOT, kMissionList);
}

void KUser::SendHellTicketFreeModeNot()
{
    std::set<int> setGameMode;
    SiKGCHelper()->GetHellItemFreeMode(setGameMode);
    SEND_DATA(EVENT_HELL_TICKET_FREE_MODE_NOT, setGameMode);
}

bool KUser::GetEquipItem(IN const char& cCharType_, OUT std::vector<KEquipItemInfo>& vecEquipItemInfo_)
{
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cCharType_);
    if (!pkCharacterInfo)
        return false;

    vecEquipItemInfo_ = pkCharacterInfo->m_vecEquipItems;
    return true;
}

void KUser::FirstNotEvents()
{
    if (SiKRainbow()->IsRainbowEventTerm()) {
        KRainBowEvent kPacket;
        if (SiKRainbow()->GetEventData(kPacket))
            SEND_PACKET(EVENT_RAINBOW_EVENT_NOT);
    }

    if (SiKCooperationEvent()->IsEventerTerm()) {
        KEVENT_CO_OP_EVENT_NOT kPacket;
        kPacket.m_nEventID = SiKCooperationEvent()->GetEventID();
        kPacket.m_dwMoneyItemID = SiKCooperationEvent()->GetMoneyItem();
        LIF(SiKCooperationEvent()->GetItemList(kPacket.m_mapEventItems));
        SEND_PACKET(EVENT_CO_OP_EVENT_NOT);
    }

    if (SiKEscort()->IsRun()) {   // 이벤트 기간일때만 전달.
        // 호위 이벤트 몬스터 정보 전달.
        KEVENT_ESCORT_INFO_NOT kPacket;
        SiKEscort()->GetEscortInfoAll(kPacket);
        SEND_PACKET(EVENT_ESCORT_INFO_NOT);
    }

    // 091019 tgkwon. 유저의 보드게임 정보 가져오기.
    // 보드판 정보를 늦게 주면 서버에서 유저정보를 DB에서 가져오는 시간이 늦어지기 때문에
    // 해당 처리가 위에 올라와야 한다.
    // 이벤트기간일때만 해당 정보를 가져오자.
    if (SiKDicePlay()->IsRun()) {
        QUEUEING_ID_TO_DB(EVENT_GET_USER_BOARD_GAME_INFO);
    }

    if (SiKGawibawibo()->IsRun()) { // 가위바위보 이벤트 기간일 때에만 가져오자
        QUEUEING_ID_TO_DB(DB_EVENT_GAWIBAWIBO_INFO_REQ);
    }

    if (SiKSocks()->IsRun()) {
        QUEUEING_ID_TO_DB(DB_EVENT_SOCKS_INFO_REQ);
    }

    if (SiKNewDonationManager()->IsRun()) {
        // 신년 이벤트 기부 NPC 정보 전송.
        KEVENT_NEW_YEAR_DONATION_NOT kPacket;
        SiKNewDonationManager()->GetNPCInfo(kPacket);
        SEND_PACKET(EVENT_NEW_YEAR_DONATION_NOT);
    }

    if (SiKPlantTree()->IsEventTerm()) {
        // 나무 나무 이벤트 맵정보 전달.
        KEVENT_PLANT_TREE_MAP_NOT kPacket;
        SiKPlantTree()->GetPlantTreeInfo(kPacket);
        SEND_PACKET(EVENT_PLANT_TREE_MAP_NOT);
    }

    {
        // 릭쿤 토네이도 정보 전달.
        KEVENT_RKTORNADO_ITEM_LIST_NOT kPacket;
        if (SiKRKTornado()->IsRun()) { // 이벤트 기간일때 정보 전달.
            kPacket.m_dwLuckyEnableCount = SiKRKTornado()->GetFailItemCount();
            SiKRKTornado()->GetNonFailRewardList(kPacket.m_mapNonFailRewardList);
            SiKRKTornado()->GetExposeItem(kPacket.m_dwExposeItemToUser);
            kPacket.m_bEventEnable = true;
        }
        else {
            kPacket.m_dwLuckyEnableCount = 0;
            kPacket.m_mapNonFailRewardList.clear();
            kPacket.m_bEventEnable = false;
        }
        SEND_PACKET(EVENT_RKTORNADO_ITEM_LIST_NOT);
    }

    // 스핑크스 OX 이벤트 정보전달.
    if (SiKSphinx()->IsRun()) { // 이벤트 기간일때만 전달.
        KEVENT_SPHINX_ITEM_LIST_NOT kPacket;
        SiKSphinx()->GetRewardList(kPacket);
        SEND_PACKET(EVENT_SPHINX_ITEM_LIST_NOT);
    }
    // 크리스마스 이벤트 정보전달.
    if (SiKCharismasPresent()->IsRun()) { // 이벤트 기간일때만 전달.
        KEVENT_CHARISMAS_EVENT_NOT kPacket;
        SiKCharismasPresent()->GetCharismasPresentInfo(kPacket);
        SEND_PACKET(EVENT_CHARISMAS_EVENT_NOT);
    }
    // JIT
    {
        KEVENT_JUST_IN_TIME_NEXT_TIME_NOT kPacket;
        if (CheckJustInTimeEvent(kPacket.m_tmJIT, kPacket.m_mapUserTypeEvent)) {
            SEND_PACKET(EVENT_JUST_IN_TIME_NEXT_TIME_NOT);
        }
    }

    // 일식 시간 이벤트 전송
    if (SiKEclipseTimeEvent()->IsRun()) {
        KEVENT_ECLIPSE_TIME_EVENT_INFO_NOT kPacket;
        SiKEclipseTimeEvent()->GetEclipseTimeInfo(kPacket);
        SEND_PACKET(EVENT_ECLIPSE_TIME_EVENT_INFO_NOT);
    }

    if (SiKEclipseCollect()->IsRun()) {
        time_t tmDate;
        SiKEclipseCollect()->GetBeginTime(tmDate);
        QUEUEING_EVENT_TO_DB(DB_EVENT_ECLIPSE_COLLECT_REWARD_CHECK_REQ, tmDate);
    }

    if (SiKSongkran()->IsRun()) {
        // 송크란 이벤트 접속 시 유저 데이터 동기화
        QUEUEING_EVENT_TO_DB(DB_EVENT_SONGKRAN_USER_INFO_REQ, m_bFirstLoginToday);
    }

    if (SiKAdventure()->IsRun()) {
        // 유저 접속 시 모험 이벤트 데이터 가져오기
        int nInitValue = 1;
        QUEUEING_EVENT_TO_DB(DB_EVENT_ADVENTURE_DATA_REQ, nInitValue);
    }

    if (SiKLevelResetEvent()->IsRun()) {
        // 레벨 리셋 정보 가져오기
        DWORD dwVersion = SiKLevelResetEvent()->GetVersion();
        QUEUEING_EVENT_TO_DB(DB_EVENT_LEVEL_RESET_EVENT_INFO_REQ, dwVersion);
    }

    if (SiKOlympicManager()->IsRun()) {
        bool bIsEnable = SiKOlympicManager()->IsGamePlayEnable();
        SEND_DATA(EVENT_OLYMPIC_INFO_NOT, bIsEnable);
    }

    if (SiKItemCompose()->IsEventTerm()) {
        KEVENT_ITEM_COMPOSE_INFO_NOT kPacket;
        SiKItemCompose()->GetItemComposeInfo(kPacket);
        SEND_PACKET(EVENT_ITEM_COMPOSE_INFO_NOT);
    }

    if (SiKGachaLottery()->IsRun()) {
        KEVENT_GACHA_LOTTERY_ITEM_INFO_NOT kPacket;
        SiKGachaLottery()->GetGachaLotteryItemInfo(kPacket.m_mapGachaSuccessList, kPacket.m_mapGachaFailedList);
        SEND_PACKET(EVENT_GACHA_LOTTERY_ITEM_INFO_NOT);
    }

    if (SiKBuffDonation()->IsEventTerm()) {   // 이벤트 기간일때만 전달.
        QUEUEING_ID_TO_DB(EVENT_BUFF_DONATION_LOGIN_INFO_REQ);
    }

    // 누적 출석 기간이면 DB에서 데이터 얻고 출석 갱신함
    if (SiKAccumulateAttendanceManager()->IsRun()) {
        QUEUEING_ID_TO_DB(EVENT_ACCUMULATE_ATTENDANCE_REQ);
    }
}

void KUser::DumpDropData(IN std::vector<KGameDropElement>& vecDrop_)
{
    //if( vecDrop_.empty() ) return;

    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;

    //START_LOG( cwarn, L"____DumpDrop____"  ) << END_LOG;
    //std::vector<KGameDropElement>::iterator vit;
    //for( vit = vecDrop_.begin() ; vit != vecDrop_.end() ; ++vit )
    //{
    //    std::Warn << L"{" << vit->m_nTriggerID << L"," << vit->m_dwItemID << L"}, ";
    //}
    //std::Warn << dbg::endl;
}

void KUser::SendItemTradeListNot()
{
    if (SiKItemTradeManager()->GetItemTradeEnable() == false)
        return;

    KEVENT_ITEM_TRADE_LIST_NOT kPacket;

    SiKItemTradeManager()->GetItemTradeListNotice(kPacket.m_mapTradeList, kPacket.m_mapTradeAllList);
    START_LOG(clog, L"ItemTrade size : " << kPacket.m_mapTradeList.size() << L"ItemTrade size : " << kPacket.m_mapTradeAllList.size()) << END_LOG;

    SEND_PACKET(EVENT_ITEM_TRADE_LIST_NOT);
}

int KUser::GetGachaReelPointSum(IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_)
{
    std::map< int, std::map< std::pair< DWORD, DWORD >, std::map< int, int > > >::iterator mitGachaReelPoint;
    std::map< std::pair< DWORD, DWORD >, std::map< int, int > >::iterator mitCharTypeReelPoint;
    std::map< int, int >::iterator mitLevelReelPoint;

    mitGachaReelPoint = m_mapGachaReelPoint.find(nVersion_);
    if (mitGachaReelPoint == m_mapGachaReelPoint.end()) {
        return 0;
    }

    std::map< std::pair< DWORD, DWORD >, std::map< int, int > >& mapCharTypeReelPoint = mitGachaReelPoint->second;

    std::pair< DWORD, DWORD > prData(dwItemID_, dwType_);

    mitCharTypeReelPoint = mapCharTypeReelPoint.find(prData);
    if (mitCharTypeReelPoint == mapCharTypeReelPoint.end()) {
        return 0;
    }

    std::map< int, int >& mapLevelReelPoint = mitCharTypeReelPoint->second;

    mitLevelReelPoint = mapLevelReelPoint.find(nLevel_);
    if (mitLevelReelPoint == mapLevelReelPoint.end()) {
        return 0;
    }

    return mitLevelReelPoint->second;
}

void KUser::SetGachaReelPointSum(IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_, IN const int nReelPoint_)
{
    std::pair< DWORD, DWORD > prData(dwItemID_, dwType_);

    m_mapGachaReelPoint[nVersion_][prData][nLevel_] = nReelPoint_;
    m_mapGachaReelPointDiff[nVersion_][prData][nLevel_] = nReelPoint_;
}

bool KUser::GetGachaNotExistList(IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, OUT std::map< int, GCITEMID >& mapNotExistList_)
{
    mapNotExistList_.clear();
    std::map< int, std::set< GCITEMID > > mapCollectionList;
    std::map< int, std::set< GCITEMID > >::iterator mit;
    std::set< GCITEMID >::iterator sit;
    int nIndex = 0;

    SiKGachaManager()->GetRewardItemList(nVersion_, dwItemID_, mapCollectionList);
    mit = mapCollectionList.find(dwType_);
    if (mapCollectionList.empty() || mit == mapCollectionList.end()) {
        return false;
    }

    // 인벤토리 찾기
    for (sit = mit->second.begin(); sit != mit->second.end(); ++sit)
    {
        if (m_kInventory.FindItemByItemID(*sit) == NULL) {
            if (m_kInventory.FindDepotItemByItemID(*sit) == NULL) {
                // 보유하지 않은 아이템 리스트 구하기
                mapNotExistList_.insert(std::make_pair(nIndex, *sit));
                ++nIndex;
            }
        }
    }

    if (true == mapNotExistList_.empty()) {
        return false;
    }

    return true;
}

bool KUser::GetGachaNotExistList_Level(IN const int nVersion_, IN const DWORD dwItemID_, IN const DWORD dwType_, IN const int nLevel_, OUT std::map< int, GCITEMID >& mapNotExistList_)
{
    mapNotExistList_.clear();
    std::map< int, std::set<GCITEMID> > mapCollectionList;
    std::map< int, std::set<GCITEMID> >::iterator mit;
    std::set<GCITEMID>::iterator sit;
    int nIndex = 0;

    SiKGachaManager()->GetRewardItemList(nVersion_, dwItemID_, mapCollectionList);
    mit = mapCollectionList.find(dwType_);
    if (mapCollectionList.empty() || mit == mapCollectionList.end()) {
        return false;
    }

    // 인벤토리 찾기
    for (sit = mit->second.begin(); sit != mit->second.end(); ++sit)
    {
        if (m_kInventory.FindItemByItemIDandLevel(*sit, nLevel_) == NULL) {
            if (m_kInventory.FindDepotItemByItemIDandLevel(*sit, nLevel_) == NULL) {
                // 보유하지 않은 아이템 리스트 구하기
                mapNotExistList_.insert(std::make_pair(nIndex, *sit));
                ++nIndex;
            }
        }
    }

    if (true == mapNotExistList_.empty()) {
        return false;
    }

    return true;
}

int KUser::CalcGachaReelPoint(IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwNotExistListSize_, IN const float fRate_)
{
    float fGrade = 0.f;
    float fDiv = 0.f;
    int nMinRandomNum = 0;
    int nMaxRandomNum = 0;

    // 가챠 등급정보 찾기(Grade,Div)
    SiKGachaManager()->GetGachaInfo(nVersion_, dwItemID_, fGrade, fDiv);

    // 최소 랜덤수 구함(남은아이템수/Div*등급점수*10)
    nMinRandomNum = static_cast<int>(dwNotExistListSize_ / fDiv * fGrade * 10);

    // 가챠아이템ID로 자기등급 찾기(등급점수:낡은=3,고급=6,최고급=9)
    // 최대 랜덤수 구함(남은아이템수*등급점수*10)
    nMaxRandomNum = static_cast<int>(dwNotExistListSize_ * fGrade * 10);

    // 랜덤수 산출(최소~최대 사이)
    int nDiff = nMaxRandomNum - nMinRandomNum;
    int nRP = static_cast<int>((nDiff * fRate_) + nMinRandomNum + 1);

    START_LOG(clog, L"Calc RP Result.")
        << BUILD_LOG(dwNotExistListSize_)
        << BUILD_LOG(nMinRandomNum)
        << BUILD_LOG(nMaxRandomNum)
        << BUILD_LOG(nRP) << END_LOG;

    return std::max< int >(0, nRP);
}

void KUser::CheckMissionEvent()
{
    // 일단 현재의 이벤트 정보와 지워져야 할 MissionID들을 받아온다.
    KMissionCurrentEvent kData;
    SiKMissionEvent()->GetEventData(kData);
    KEVENT_MISSION_ITEM_NOT kPacket_;
    // DB로 넘겨야 하니깐 해당일의 정보를 담아두자~!
    kPacket_.m_CurrentData = kData;

    // 일단 아이템 검색을 진행한다.
    std::set<int>::iterator sit;
    for (sit = kData.m_setItemList.begin(); sit != kData.m_setItemList.end(); ++sit)
    {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID(*sit);
        if (pkItem)
        {
            // 가지고 있는 아이템의 갯수를 0으로 만들어 준다.
            pkItem->m_nCount = 0;
            // 해당 아이템을 클라이언트에게 전송할 패킷에 세팅한다.
            KItem kdItemInfo;
            kdItemInfo = *pkItem;
            kPacket_.m_vecItemDel.push_back(kdItemInfo);
        }

        // 해당 미션이 등록되어 있는지 확인한다.
        // 해당 ID의 경우 메인 미션 ID이므로 메인 미션 ID로 해서 
        // 서브 미션 ID도 함께 지워주는 작업을 진행한다.

        // 지우려고 하는 미션이 슬롯에 존재하는가?
        if (IsRegistedMission(*sit))
        {
            KEVENT_REMOVE_MISSION kPacket1;
            kPacket1.m_dwMissionID = *sit;

            // DB큐잉
            QUEUEING_EVENT_TO_DB(EVENT_REMOVE_MISSION_REQ, kPacket1);

            if (!EraseMissionDataFromSlot(*sit))
            {
                START_LOG(cerr, L"실제 미션 제거 시점에서 해당 미션을 찾을수 없음.. Name : " << m_strName)
                    << BUILD_LOG(*sit) << END_LOG;
            }
            else
            {
                kPacket_.m_setMissionList.insert(*sit);
            }
        }
    }

    // DB에 해당 Item을 넣어주는 작업에 대해서 진행하자 ~~!!!
    QUEUEING_EVENT_TO_DB_CHECK(EVENT_MISSION_ITEM_NOT, kPacket_);

END_PROC:
    START_LOG(clog, L"CheckMissionEvent")
        << BUILD_LOG(kPacket_.m_vecItemDel.size())
        << BUILD_LOG(kPacket_.m_vecItemAdd.size())
        << BUILD_LOG(kPacket_.m_setMissionList.size())
        << BUILD_LOG(kPacket_.m_CurrentData.m_vecMissions.size())
        << BUILD_LOG(kPacket_.m_CurrentData.m_setItemList.size()) << END_LOG;
}

void KUser::SendSubscriptionInfoNotice(IN const KECN_SUBSCRIPTION_INFO_UPDATE_NOT& mapEventInfo_)
{
    KEVENT_SUBSCRIPTION_INFO_NOT kPacket;
    KECN_SUBSCRIPTION_INFO_UPDATE_NOT::const_iterator cmit;
    MAP_INT_DWORD::const_iterator cmit2;

    for (cmit = mapEventInfo_.begin(); cmit != mapEventInfo_.end(); ++cmit)
    {
        DWORD dwMyGiftCount = 0L;   // 내가 획득한 경품 수량

        cmit2 = m_mapMySubscriptionGiftCount.find(cmit->first);
        if (cmit2 != m_mapMySubscriptionGiftCount.end())
        {
            dwMyGiftCount = cmit2->second;
        }

        // 클라이언트에 전달할 패킷 만들기
        kPacket[cmit->first] = std::make_pair(cmit->second, dwMyGiftCount);
    }
    START_LOG(clog, L"Subscription Info Notice. size : " << kPacket.size()) << END_LOG;

    SEND_COMPRESS_PACKET(EVENT_SUBSCRIPTION_INFO_NOT, kPacket);
}

void KUser::SendNpcGiftsNot()
{
    std::set< int > setEventList;
    SiKNpcGift()->GetEventList(setEventList);
    SEND_DATA(EVENT_NPC_GIFTS_NOT, setEventList);
}

// 유저의 보드게임 정보 초기화 여부 체크.
void KUser::InitUserBoardInfo(IN int nOK_, IN OUT KUserBoardInfo& kUserBoard_)
{
    // 하루의 특정시간이 지났는지 체크를 하고..,지났으면 초기화를 해야겠지?
    KDB_EVENT_INIT_DICE_PLAY_REQ kDBPacket;
    DWORD dwInitHour = 0;
    std::wstring wstrDiceInitTime;

    dwInitHour = SiKDicePlay()->GetInitHour();
    m_tmDiceLastTime = CTime::GetCurrentTime();
    wstrDiceInitTime = (LPCTSTR)(m_tmDiceLastTime.Format(_T("%Y-%m-%d %H:%M:%S"))); // 현재 시간을 DB에 저장하기 위한 Format으로 변경.

    // nOK값이 2일 리턴될 때는 날이 바뀌었다는 의미이다.
    if (nOK_ == 2)
    {
        kUserBoard_.m_dwRemainCount = SiKDicePlay()->GetDailyMax();  // 주사위 굴릴수 있는 횟수 초기화.
        kUserBoard_.m_dwGiveCount = SiKDicePlay()->GetDailyFree(); // 무료지급 횟수 초기화.
        kUserBoard_.m_dwRemainTime = SiKDicePlay()->GetFreeTickGap(); // 무료 시간으로 초기화.
        START_LOG(clog, L"하루가 지나서 유저의 보드게임 정보 초기화.")
            << BUILD_LOG(m_strName)
            << BUILD_LOG(kUserBoard_.m_dwRemainCount)
            << BUILD_LOG(kUserBoard_.m_dwGiveCount) << END_LOG;
        kDBPacket.m_kMyBoardInfo = kUserBoard_;
        kDBPacket.m_wstrDicePlayTime = wstrDiceInitTime;
        // 초기화를 진행한 후 해당 정보 세팅후에 DB에 해당값 세팅을 진행한다.
        QUEUEING_EVENT_TO_DB(DB_EVENT_INIT_DICE_PLAY_REQ, kDBPacket);
    }
    kUserBoard_.m_dwRemainCount = SiKDicePlay()->GetDailyMax();
    kUserBoard_.m_dwRemainTime = std::min<DWORD>(kUserBoard_.m_dwRemainTime, SiKDicePlay()->GetFreeTickGap());

    SetUserBoardInfo(kUserBoard_);
}

// 하루가 지났는지를 체크.
void KUser::CheckUserBoardInfo(IN OUT KUserBoardInfo& kUserBoard_)
{
    // 하루의 특정시간이 지났는지 체크를 하고..,
    // 지났으면 초기화를 해야겠지?
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tmCurrentTime = KncUtil::TimeToInt(tmCurrent); // 현재 시간 int변환.
    time_t tmLastTime = KncUtil::TimeToInt(m_tmDiceLastTime); // 마지막 주사위 받은 시간 int 변환.

    KDB_EVENT_INIT_DICE_PLAY_REQ kDBPacket;
    DWORD dwInitHour = 0;
    std::wstring wstrDiceInitTime;

    wstrDiceInitTime = (LPCTSTR)(tmCurrent.Format(_T("%Y-%m-%d %H:%M:%S"))); // 현재 시간을 DB에 저장하기 위한 Format으로 변경.

    // 초기화 시간에 대한 체크.
    if (tmCurrent.GetDay() == m_tmInitDiceTime.GetDay() && tmCurrent.GetHour() == m_tmInitDiceTime.GetHour()) {
        kUserBoard_.m_dwRemainCount = SiKDicePlay()->GetDailyMax();  // 주사위 굴릴수 있는 횟수 초기화.
        kUserBoard_.m_dwGiveCount = SiKDicePlay()->GetDailyFree(); // 무료지급 횟수 초기화.
        kUserBoard_.m_dwRemainTime = SiKDicePlay()->GetFreeTickGap(); // 무료 시간으로 초기화.
        m_tmDiceLastTime = CTime::GetCurrentTime(); // 마지막 주사위 받은 시간 세팅.
        CTimeSpan ts(1, 0, 0, 0);
        m_tmInitDiceTime = m_tmDiceLastTime + ts;     // 다음 갱신 시간 저장.

        kDBPacket.m_kMyBoardInfo = kUserBoard_;
        kDBPacket.m_wstrDicePlayTime = wstrDiceInitTime;
        // 초기화를 진행한 후 해당 정보 세팅후에 DB에 해당값 세팅을 진행한다.
        QUEUEING_EVENT_TO_DB(DB_EVENT_INIT_DICE_PLAY_REQ, kDBPacket);

        START_LOG(clog, L"하루가 지나서 유저의 보드게임 정보 초기화.")
            << BUILD_LOG(m_strName)
            << BUILD_LOG(kUserBoard_.m_dwRemainCount)
            << BUILD_LOG(kUserBoard_.m_dwGiveCount) << END_LOG;
    }
    else {
        DWORD dwGapTime = tmCurrentTime - tmLastTime; // 현재 시간과 마지막 주사위 받은 시간과의 차이 가져오기.
        DWORD dwFreeTime = SiKDicePlay()->GetFreeTickGap(); // 무료로 지급하는 시간 Gap 가져오기.(초단위)
        if (dwGapTime >= kUserBoard_.m_dwRemainTime && kUserBoard_.m_dwGiveCount > 0)
        {
            kUserBoard_.m_dwDiceCount += 1;       // 주사위 굴릴수 있는 횟수 하나 증가시켜 주고..,
            kUserBoard_.m_dwGiveCount -= 1;         // 받을수 있는 주사위 횟수 하나 줄이고..,
            DWORD dwRemainResult = dwGapTime - kUserBoard_.m_dwRemainTime;
            kUserBoard_.m_dwRemainTime = dwFreeTime - dwRemainResult;
        }
        else
        {   // 무료 주사위 지급이 끝나면 남은 시간(초)를 0으로 세팅하자.
            if (kUserBoard_.m_dwGiveCount == 0) kUserBoard_.m_dwRemainTime = 0;
            else kUserBoard_.m_dwRemainTime -= dwGapTime;
        }
    }
    m_tmDiceLastTime = CTime::GetCurrentTime(); // 마지막 주사위 로직 계산된 시간 저장.
    SetUserBoardInfo(kUserBoard_);    // 세팅한 주사위 정보 저장하기.
}

// 유저 보드게임 정보 세팅.
void KUser::SetUserBoardInfo(IN KUserBoardInfo kUserBoard_)
{
    // 주사위 초기화 대한 보완 코드.
    // 주사위 초기화 되는 시간에 서버이동이나 로그아웃 하는 유저들을 강제로 초기화 해준다.
    CTime tmCurrent = CTime::GetCurrentTime();
    DWORD dwInitHour = SiKDicePlay()->GetInitHour();
    if (tmCurrent.GetHour() == dwInitHour && tmCurrent.GetMinute() < 2) {
        kUserBoard_.m_dwRemainCount = SiKDicePlay()->GetDailyMax();  // 주사위 굴릴수 있는 횟수 초기화.
        kUserBoard_.m_dwGiveCount = SiKDicePlay()->GetDailyFree(); // 무료지급 횟수 초기화.
        kUserBoard_.m_dwRemainTime = SiKDicePlay()->GetFreeTickGap(); // 무료 시간으로 초기화.
    }
    m_kUserBoardInfo = kUserBoard_;
}

// 유저 보드게임 정보 가져오기.
void KUser::GetUserBoardInfo(OUT KUserBoardInfo& kUserBoard_)
{
    kUserBoard_ = m_kUserBoardInfo;
}

// 유저 보드게임 정보 저장.
void KUser::UpdateUserBoardInfo(OUT DWORD& dwInitHour_, OUT KUserBoardInfo& kUserBoardInfo_, OUT std::wstring& strDiceLastTime_)
{
    KUserBoardInfo kUserBoardInfo;
    kUserBoardInfo.Clear();
    GetUserBoardInfo(kUserBoardInfo);
    CheckUserBoardInfo(kUserBoardInfo); // 주사위 이벤트 유저의 정보 계산.
    kUserBoardInfo_ = kUserBoardInfo;
    dwInitHour_ = SiKDicePlay()->GetTotalMapCount(); // 주사위 판 전체 MapCount정보.
    strDiceLastTime_ = (LPCTSTR)(m_tmDiceLastTime.Format(_T("%Y-%m-%d %H:%M:%S"))); // 유저의 주사위 받은 정보.
}

bool KUser::IsPCBangUser()
{
    return SiKIPAdvantage()->IsPCBangUser(m_cPCBangType);
}

void KUser::SendExitInfoToCenter()
{
    KECN_LOG_OUT_NOT kPacket;
    kPacket.m_dwAuthType = GetAuthType();
    kPacket.m_dwUID = GetUID();
    kPacket.m_strLogin = GetName();
    kPacket.m_dwUID = ::ntohl(GetIP());
    kPacket.m_strPassword = m_strPasswd;

    LIF(SiKCnConnector()->SendPacket(KCenterEvent::ECN_LOG_OUT_NOT, kPacket));

}

void KUser::GetFriendInviteList(OUT std::vector<KInviteUserInfo>& vecInviteList, IN char cChannelType_)
{
    vecInviteList.clear();
    std::vector<DWORD>::iterator vit;

    // -1 이면 던전 이고..
   // 아니면 해당 채널..
    for (vit = m_vecOnlineFriends.begin(); vit != m_vecOnlineFriends.end(); ++vit)
    {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*vit);
        if (!spUser || !spUser->m_pkChannel) continue; // 광장에 있는 유저도 채널로비 채널에는 포함 되어 있음..

        //친구가 초대 가능한 곳에 있는가?
        if (spUser->GetStateID() != KUserFSM::STATE_CHANNEL &&
            spUser->GetStateID() != KUserFSM::STATE_CHANNELLOBBY &&
            spUser->GetStateID() != KUserFSM::STATE_SQUARE) continue;

        KInviteUserInfo kData;
        if (spUser->GetInviteInfo(kData))
            vecInviteList.push_back(kData);

    }

}

void KUser::UpdateAdvertisingExposureCountToDB()
{
    if (m_mapAdvertisingExposureCount.empty())
        return;

    QUEUEING_EVENT_TO_DB(EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, m_mapAdvertisingExposureCount);
}

bool KUser::ShortGameCheck(KUserPtr& spUser, IN bool bWin_, OUT bool& bRet_, OUT KGameResultOut& kResultOut_, OUT KEndGameAck& kAck_)
{
    if (m_pkRoom->IsShortGame() && bWin_)
    {
        spUser->GetDefaultResultOut(kResultOut_);
        kAck_.m_vecGameResult.push_back(kResultOut_);
        bRet_ = false;
        kAck_.m_setHackingUserList.insert(spUser->m_dwUID);
        return false;
    }
    return true;
}

void KUser::UpdateIndigoInfo(IN const KGameResultIn& vitResultIn_, IN int nGameMode_, IN bool bWin_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_)
{
    // spUser 는 밖에서 검증하므로 안에서는 하지 않겠습니다. 
    if ((nGameMode_ != GC_GM_INDIGO_TEAM) || (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO) == false)) {
        return;
    }

    if (bWin_) {
        spUser->m_iIndigoWin += 1;
    }
    else {
        spUser->m_iIndigoLose += 1;
    }

    kResultOut_.m_iIndigoWin = spUser->m_iIndigoWin;
    kResultOut_.m_iIndigoLose = spUser->m_iIndigoLose;

    // 용사의 섬은 한판하면 방이 깨지므로 방원의 정보를 업데이트 할 필요가 없다.
    const KInDoorUserInfo* pkStartingInfo = m_pkRoom->FindInDoorUserInfo(spUser->m_strName);
    if (pkStartingInfo) {
        int nGetRp = IndigoUtil::GetRPChange(spUser->m_iRP,
            m_pkRoom->m_prAvgRP,
            pkStartingInfo->m_iTeam == 1,
            vitResultIn_.m_bWin,
            3 - vitResultIn_.m_iNumDies);

        spUser->m_iRP += nGetRp;
        kResultOut_.m_iRankingPoint = spUser->m_iRP;

        Indigo_Log(vitResultIn_, nGetRp, spUser->GetIPStr()); // 용섬 로그..
    }
    End_Indigo_Log();
}

// #UpdateGuildPoint
void KUser::UpdateGuildPoint(IN const KGameResultIn& vitResultIn_, IN int nGameMode_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_)
{
    if (spUser->m_kGuildUserInfo.m_dwGuildUID != 0) // nGameMode_ == GC_GM_GUILD_BATTLE &&
    {
        spUser->m_kGuildUserInfo.m_dwBattlePoint = SiKGuildManager()->GetGuildBattlePoint(spUser->GetGuildUID());

        SiKGuildManager()->UpdateGuildPoint(spUser->m_kGuildUserInfo.m_dwGuildUID, vitResultIn_.m_iContPoint, spUser->GetUID(), 0, 0);

        KNGuildInfo kGuildInfo;
        if (SiKGuildManager()->GetGuildInfo(spUser->m_kGuildUserInfo.m_dwGuildUID, kGuildInfo))
        {
            kResultOut_.m_dwGuildPoint = kGuildInfo.m_dwPoint;
        }

        kResultOut_.m_dwGuildUID = spUser->m_kGuildUserInfo.m_dwGuildUID;
        kResultOut_.m_dwContPoint = vitResultIn_.m_iContPoint; // 이번에 얻는 길드포인트(개인 기여)

        spUser->AddGuildContributePoint(kResultOut_.m_dwContPoint);
    }
}

void KUser::UpdateDifficultyGameMode(IN const KGameResultIn& vitResultIn_, IN int nGameMode_, IN int nDifficulty_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_)
{
    if (false == KRoom::IsDifficultyMode(nGameMode_)) {
        return;
    }

    // 승리일때만 기록
    if (false == vitResultIn_.m_bWin) {
        return;
    }

    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mitTotal;
    mitTotal = spUser->m_mapDifficulty.find(spUser->m_cCurrentCharacter);
    if (mitTotal == spUser->m_mapDifficulty.end()) {
        return;
    }

    std::map<int, KDungeonUserInfo>::iterator mit;
    mit = mitTotal->second.find(nGameMode_);
    if (mit == mitTotal->second.end()) {
        START_LOG(cwarn, L"초기값 분명 채워줬는데, 던전 클리어정보가 없음.. Login : " << spUser->GetName())
            << BUILD_LOG(nGameMode_) << END_LOG;
        return;
    }

    // 클리어 난이도 기록
    mit->second.m_kModeDifficulty.Set(nDifficulty_);
    spUser->UpdateDungeonClearInfo(nGameMode_, nDifficulty_, mit->second.m_kModeDifficulty, mit->second.m_kClearData);

    // 패킷에 집어 넣어준다.
    kResultOut_.m_mapDifficulty = mitTotal->second;

    START_LOG(clog, L"난이도 있는 게임 모드 클리어.. Login : " << spUser->GetName())
        << BUILD_LOG(nGameMode_)
        << BUILD_LOG(nDifficulty_) << END_LOG;
}

bool KUser::GetChatExpGP(OUT bool& bActiveChatEvent_, OUT float& fChatBoostExp_, OUT float& fChatBoostGP_)
{// exp, gp 를 가져온다는 것은 이 함수를 두 가지 용도로 쓴다는 의미인가?  exp와 gp 를 나누는게 더 좋은가
    _JIF(m_pkRoom, return false);

    bActiveChatEvent_ = m_pkRoom->IsChatEventReward();

    if (bActiveChatEvent_ == true)
    {
        fChatBoostExp_ = m_pkRoom->GetChatEventEXPRatio();
        fChatBoostGP_ = m_pkRoom->GetChatEventGPRatio();
    }
    return true;
}

void KUser::CalcStageClearRatio(IN int nGameMode_, IN int nDifficulty_, IN size_t sizeOfClearStage_, OUT float& fStageClearRatio_)
{
    std::pair<int, int> prStageCount;
    if (KRoom::IsDungeonMode(nGameMode_) && SiKResultManager()->GetStageCountInfo(nGameMode_, nDifficulty_, prStageCount))
    {
        if (0 >= prStageCount.second) {
            fStageClearRatio_ = 0.f;
        }
        else {
            fStageClearRatio_ = (float)sizeOfClearStage_ / (float)prStageCount.second;  // 스테이지 달성률 구한다
        }
    }

    if (nGameMode_ == GC_GM_ANGELS_EGG || nGameMode_ == GC_GM_DOTA) {
        fStageClearRatio_ = 0.f;
    }

    _LOG_SUCCESS(fStageClearRatio_ >= 0, L"StageClearRatio 의 결과 , fStageClearRatio_ : " << fStageClearRatio_)
        << BUILD_LOG(nGameMode_)
        << BUILD_LOG(nDifficulty_)
        << BUILD_LOG(sizeOfClearStage_)
        << BUILD_LOG(prStageCount.second) << END_LOG;
}

void KUser::UpdateWinLoseRecord(IN int nGameMode_, IN bool bWin_, IN int nUserCount_, IN char cCharType_,
    OUT int& iWin_, OUT int& iLose_, OUT KGameResultOut& kResultOut_)
{
    if (nGameMode_ == GC_GM_SURVIVAL_MODE || nGameMode_ == GC_GM_TEAM_MODE ||
        nGameMode_ == GC_GM_GUILD_BATTLE || nGameMode_ == GC_GM_TAG_TEAM ||
        nGameMode_ == GC_GM_TAG_SURVIVAL || nGameMode_ == GC_GM_DEATH_TEAM) {// 데쓰매치는 모두 다 나가서

        if (bWin_) {
            if (nUserCount_ > 1 || nGameMode_ != GC_GM_DEATH_TEAM) {
                iWin_ += 1;
            }
        }
        else {
            iLose_ += 1;
        }
    }

    kResultOut_.m_vecWin.push_back(std::make_pair(cCharType_, iWin_));
    kResultOut_.m_vecLose.push_back(std::make_pair(cCharType_, iLose_));
}

void KUser::SetInitValue_KCharacterExpInfoOut(OUT KCharacterExpInfoOut& kOutData_, IN const KCharacterInfo* pkCharacterInfo_)
{
    kOutData_.m_cCharType = pkCharacterInfo_->m_cCharType;
    kOutData_.m_biOldExp = pkCharacterInfo_->m_biExp;
    kOutData_.m_biExp = pkCharacterInfo_->m_biExp;
    kOutData_.m_kOldSkillInfo = pkCharacterInfo_->m_kSkillInfo;
    kOutData_.m_kSkillInfo = pkCharacterInfo_->m_kSkillInfo;
}

void KUser::PVP_NKillEvent(IN const std::vector<KGameResultIn>& vecGameResult_, OUT std::vector< std::wstring >& vecStrLogin_)
{
    std::vector<KGameResultIn>::const_iterator vit1;

    for (vit1 = vecGameResult_.begin(); vit1 != vecGameResult_.end(); ++vit1)
    {
        if (vit1->m_bWin == true && vit1->m_iNumKills >= 5)
        {
            // 방으로 로긴 넘겨서   어느 편인지 체크
            const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo(vit1->m_strLogin);

            JIF(pkInDoorUserInfo);
            m_pkRoom->SetPvPEvent(pkInDoorUserInfo->m_iTeam, vecStrLogin_); // vecStrLogin 는 안에서 clear함
            //pkInDoorUserInfo->m_iTeam;  // 이긴 팀이고 동시에  어떤 플레이어가 5명을 kill 했다면
            // 이긴 팀의 팀멤버 로긴 벡터가 넘어온다면
            break;
        }
    }
}

// 방장의 게임결과 계산에서 다른 유저의 아이템 지워준다. 
void KUser::ClearUsedQuantityItem(IN const std::vector<KGameResultIn>& vecGameResult_)
{
    // 060630. jseop. 사용한 아이템 기록 지움.
    std::vector<KGameResultIn>::const_iterator vit;
    for (vit = vecGameResult_.begin(); vit != vecGameResult_.end(); ++vit)
    {
        KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(vit->m_strLogin));

        if (spUser)
        {
            spUser->m_vecUsedDurationItem.clear();
        }
    }
}

// 자신의 사용한 아이템 기록 지움
void KUser::ClearUsedQuantityItem()
{
    m_vecUsedDurationItem.clear();
}

void KUser::DecreaseHellModeTicket(IN const std::vector<KGameResultIn>& vecGameResult_)
{
    if (false == m_pkRoom->IsHellMode())
    {
        return;
    }

    // 게임종료시 입장권 삭제하는 경우.
    if (SiKGCHelper()->HellItemCheckEnable(m_pkRoom->m_iGameMode) == true &&
        SiKGCHelper()->IsEndGameHellTicketDec() == true)
    {
        //방 유저별로 헬 입장권 깎기
        std::vector<KGameResultIn>::const_iterator vit;
        for (vit = vecGameResult_.begin(); vit != vecGameResult_.end(); vit++)
        {
            KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(vit->m_strLogin));

            if (!spUser)
            {
                START_LOG(cwarn, L"해당 유저를 찾을 수 없음. Login : " << vit->m_strLogin) << END_LOG;
                continue;
            }

            //헬모드 입장권 깎기.
            if (spUser->m_kInventory.FindItemByItemID(SiKGCHelper()->GetDungeonFreePassID())) {
                START_LOG(clog, L"던전 프리패스 소유자라서 헬모드 티켓 감소 안함 Name : " << vit->m_strLogin) << END_LOG;
                continue;
            }

            if (!spUser->CheckIfDurItemExist(SiKGCHelper()->GetHellItemID(), 1, true))
            {
                START_LOG(cerr, L"헬모드 시작시 체크 했는데 아이템이 없음.. Name : " << vit->m_strLogin) << END_LOG;
            }
        }
    }
}

void KUser::NotifyPremiumToRoomUsers()
{
    USER_PREMIUM_INFO mapPremium;
    m_pkRoom->GetPremiumInfo(mapPremium);
    LIF(m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_PREMIUM_NOT, mapPremium));
}

void KUser::ApplyPetReward(IN const KGameResultOut& kResultOut_)
{
    std::vector< std::pair<char, KPetInfo> >::const_iterator vit;

    DWORD dwCharNumNum = kResultOut_.m_vecPetInfo.size();
    for (vit = kResultOut_.m_vecPetInfo.begin(); vit != kResultOut_.m_vecPetInfo.end(); ++vit)
    {
        const KCharacterInfo* pkCharacterInfo = NULL;
        pkCharacterInfo = GetCharacterInfo(vit->first);
        if (pkCharacterInfo == NULL)
            continue;

        KPetInfo* pkPetInfo = NULL;
        if (GetPetInfo(pkCharacterInfo->m_kEquipPetInfo.m_dwUID, &pkPetInfo))
        {
            pkPetInfo->m_dwEXP = vit->second.m_dwEXP;
            pkPetInfo->m_iLevel = vit->second.m_iLevel;
            pkPetInfo->m_iSatiation = vit->second.m_iSatiation;

            START_LOG(clog, L"보상받은 펫의 정보")
                << BUILD_LOG(m_strName)
                << BUILD_LOG(pkPetInfo->m_dwUID)
                << BUILD_LOG(pkPetInfo->m_dwID)
                << BUILD_LOG(pkPetInfo->m_dwEXP)
                << BUILD_LOG(pkPetInfo->m_iSatiation)
                << BUILD_LOG(pkPetInfo->m_iLevel) << END_LOG;
        }
    }
}

void KUser::DBUpdateIndigo()
{
    if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO)) {
        KDB_EVENT_UPDATE_INDIGO_REQ kReq;
        kReq.m_dwUserUID = m_dwUID;
        kReq.m_iRPChange = m_iRP.GetChangeValue();
        kReq.m_iWinChange = m_iIndigoWin.GetChangeValue();
        kReq.m_iLoseChange = m_iIndigoLose.GetChangeValue();
        if (kReq.m_iRPChange != 0 || kReq.m_iWinChange != 0 || kReq.m_iLoseChange != 0) {
            QUEUEING_EVENT_TO_DB(DB_EVENT_UPDATE_INDIGO_REQ, kReq);
        }
    }
}


void KUser::ExitStatistics()
{
    CTime cDisconnectionTime = CTime::GetCurrentTime();                 // 종료시 시간
    CTimeSpan cPlayTimeSpan = cDisconnectionTime - m_cConnectionTime;   // PlayTime
    LONGLONG dPlayTime = cPlayTimeSpan.GetTotalMinutes();
    //if ( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) == false ) 
    {//플레이 타임이 최소 10분 이상 되어야 기록
        // playtime 저장
        KDB_EVENT_INSERT_STATISTICS_NOT kNot;
        kNot.m_strStartDate = (LPCTSTR)(m_cConnectionTime.Format(_T("%Y-%m-%d %H:%M:%S")));
        kNot.m_strEndDate = (LPCTSTR)(cDisconnectionTime.Format(_T("%Y-%m-%d %H:%M:%S")));
        kNot.m_cGrade = m_cGrade;
        kNot.m_dwPlayTime = static_cast<DWORD>(dPlayTime);
        kNot.m_bPCBang = IsPCBangUser();
        kNot.m_bGuildServer = SiKGSSimLayer()->CheckTypeFlag(ST_GUILD);
        kNot.m_strIP = KncUtil::toWideString(GetIPStr());

        kNot.m_vecCharData.reserve(m_mapCharacterInfo.size()); // 흠...
        std::map<char, KCharacterInfo>::const_iterator cmit;
        for (cmit = m_mapCharacterInfo.begin(); cmit != m_mapCharacterInfo.end(); ++cmit) {
            KCharStat charStat;
            charStat.m_cCharType = cmit->second.m_cCharType;
            charStat.m_biDiffExp = cmit->second.m_biExp - m_mapInitExp[cmit->second.m_cCharType];
            charStat.m_biExp = cmit->second.m_biExp;
            charStat.m_dwLevel = cmit->second.m_dwLevel;
            charStat.m_dwPlayCount = m_dwPlayCount[charStat.m_cCharType];
            charStat.m_biTotalExp = cmit->second.m_biTotalExp;
            charStat.m_biTotalDiffExp = cmit->second.m_biTotalExp - m_mapTotalInitExp[cmit->second.m_cCharType];

            switch (charStat.m_cCharType) {
            case GC_CHAR_ELESIS:    charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_KNIGHT_ID) > 0); break;
            case GC_CHAR_LIRE:      charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_ARCHER_ID) > 0); break;
            case GC_CHAR_ARME:      charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_MAGICIAN_ID) > 0); break;
            case GC_CHAR_LAS:       charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_LAS_ID) > 0); break;
            case GC_CHAR_RYAN:      charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_RYAN_ID) > 0); break;
            case GC_CHAR_RONAN:     charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_RONAN_ID) > 0); break;
            case GC_CHAR_AMY:       charStat.m_bTurtleBack = (GetDurItemCount(TURTLE_AMY_ID) > 0); break;
            default:
                charStat.m_bTurtleBack = false;
            }
            kNot.m_vecCharData.push_back(charStat);
        }
        kNot.m_dwPTTime = GetPlayTime();
        QUEUEING_EVENT_TO_DB(DB_EVENT_INSERT_STATISTICS_NOT, kNot);
    }

    AddStat(); // logout stat
    AddStat_Zombie_SendBuffer();

    // 대전해킹 통계 파일기록
    if (SiKPreHackingCheckManager()->CheckOption(KPreHackingCheckManager::GHT_PVP_CHECK, KPreHackingCheckManager::OPT_WRITE_FILE)) {
        if (false == m_mapPvpCategoryHackCount.empty()) {
            std::wstringstream stm;
            stm << GetName() << L" | ";

            std::map<char, USHORT>::iterator mit;
            for (mit = m_mapPvpCategoryHackCount.begin(); mit != m_mapPvpCategoryHackCount.end(); ++mit) {
                stm << static_cast<int>(mit->first) << L" | "
                    << mit->second << L" | ";
            }
            stm << std::endl;

            TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_PVP_HACK, std::wstring(L""), 0, m_cCurrentCharacter, stm.str());
            m_mapPvpCategoryHackCount.clear();

            stm.clear();
            stm.str(L"");
        }
    }
    DWORD dwDurationTime = static_cast<DWORD>(dPlayTime);
    AddStatPlayTime(m_dwPVPPlayTime, m_dwPVEPlayTime, dwDurationTime);
}

void KUser::GashLogOut()
{
    if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_GASH) && GetAuthType() == KGSSimLayer::AT_GASH) {
        KGASH_ACCOUNTING kNot;
        kNot.m_strLogin = KncUtil::toNarrowString(m_strName);
        kNot.m_strIP = GetIPStr();
        kNot.m_dwUserUID = m_dwUID;
        KTThreadManager<KGSGash>::GetInstance()->QueueingEvent(KUserEvent::DB_EVENT_EXIT_GAME_NOT, m_strName.c_str(), 0, m_cCurrentCharacter, kNot);
    }
}

void KUser::AddStat()
{
    KLoginOutStatPtr spStat = GET_STAT(KLoginOutStat, SI_LOGINOUT_STAT);
    if (!spStat) {
        return;
    }

    if (m_bServerMigrationReserved) {
        spStat->AddStat(KLoginOutStat::LOGOUT_MIGRATION);
    }
    else {
        spStat->AddStat(KLoginOutStat::LOGOUT);

        // PC방 접속종료
        if (IsPCBangUser()) {
            spStat->AddStat(KLoginOutStat::PCBANG_LOGOUT);
        }
    }
}

void KUser::AddStat_Zombie_SendBuffer()
{
    if (GetDisconnReason() == DRW_ZOMBIE) {
        SiKFailRate()->IncreaseCount(KFailRate::ZOMBIE_COUNT); // 060913. 좀비로 죽은 유저수 카운트(DB)

        KLoginOutStatPtr spStat = GET_STAT(KLoginOutStat, SI_LOGINOUT_STAT);
        if (spStat) {
            spStat->AddStat(KLoginOutStat::ZOMBIE_OUT); // 분단위 파일 기록용
        }
    }

    if (m_kSockObj.m_bSendBufferFull) {
        SetDisconnReason(DRW_SEND_BUFFER_FULL);

        std::wstringstream stm;
        stm << L"Login : " << m_strName << L", Time : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << GLOG::endl;

        TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_SEND_BUFF_FULL, GetName(), GetUID(), m_cCurrentCharacter, stm.str());

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }
}

bool KUser::GetGuildInfo(OUT KNGuildInfo& guildInfo_)
{
    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return false;
    }

    KGuildPtr spGuild = SiKGuildManager()->GetGuild(m_kGuildUserInfo.m_dwGuildUID);
    if (!spGuild) {
        return false;
    }

    guildInfo_ = spGuild->GetNGuildInfo();

    if (guildInfo_.m_dwUID == 0) {
        return false;
    }
    return true;
}

std::wstring KUser::GetGuildMarkName()
{
    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return std::wstring();
    }

    KGuildPtr spGuild = SiKGuildManager()->GetGuild(m_kGuildUserInfo.m_dwGuildUID);
    if (!spGuild) {
        return std::wstring();
    }

    KNGuildInfo guildInfo_ = spGuild->GetNGuildInfo();

    if (guildInfo_.m_dwUID == 0) {
        guildInfo_.m_strFileName.clear();
    }

    return guildInfo_.m_strFileName;
}

void KUser::CenterChat(KChatData& kPacket_)
{
    // Center
    KECN_CHAT_REQ kPacket;
    kPacket.m_dwServerUID = SiKGameServer()->GetUID();
    kPacket.m_kChatData = kPacket_;
    kPacket.m_kChatData.m_dwSenderUID = GetUID();
    kPacket.m_kChatData.m_strSenderNick = GetNickName();
    kPacket.m_kChatData.m_strSenderNickColor = GetNickColor();
    SiKCnConnector()->SendPacket(KCenterEvent::ECN_CHAT_REQ, kPacket);

}

void KUser::UpdateMapDifficulty(IN const char& cCharType_, IN const std::map<int, KDungeonUserInfo>& mapDifficulty_)
{
    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mitTotal;
    mitTotal = m_mapDifficulty.find(cCharType_);
    if (mitTotal != m_mapDifficulty.end()) {
        mitTotal->second = mapDifficulty_;
    }

    START_LOG(clog, L"난이도 덮어쓰기")
        << BUILD_LOG(mapDifficulty_.size()) << END_LOG;
}

void KUser::UpdateGuildPoint(IN DWORD& dwGuildPoint_)
{
    START_LOG(clog, L"길드 기여도  적용 전, Point: " << m_kGuildUserInfo.m_dwContributePoint)
        << BUILD_LOG(GetUID()) << END_LOG;

    dwGuildPoint_ = std::max<DWORD>(dwGuildPoint_, m_kGuildUserInfo.m_dwContributePoint);
    m_kGuildUserInfo.m_dwContributePoint = dwGuildPoint_;

    START_LOG(clog, L"길드 기여도  적용 후, Point: " << m_kGuildUserInfo.m_dwContributePoint)
        << BUILD_LOG(GetUID()) << END_LOG;
}

void KUser::InitLoadingCompleteTick()
{
    SetTick(KUser::GAME_START_TICK);
    m_bRecvLoadingComplete = false;
}

void KUser::GetGuildUserInfo(OUT KNGuildUserInfo& kInfo_)
{
    kInfo_.Init();

    if (m_kGuildUserInfo.m_dwGuildUID != 0) {
        KGuildPtr spGuild = SiKGuildManager()->GetGuild(m_kGuildUserInfo.m_dwGuildUID);

        if (spGuild != NULL) {
            LIF(spGuild->GetNGuildUserInfo(GetUID(), kInfo_));
        }
    }
}

void KUser::GetSimpleGuildUserInfo(OUT KNSimpleGuildUserInfo& kInfo_)
{
    kInfo_.Init();

    if (m_kGuildUserInfo.m_dwGuildUID != 0) {
        kInfo_ = m_kGuildUserInfo;
    }
}

void KUser::AdjustBoostInfo(IN OUT KGameResultIn& kResultIn_)
{
    std::vector< KCharacterExpInfo >::iterator vit;
    for (vit = kResultIn_.m_vecCharExp.begin(); vit != kResultIn_.m_vecCharExp.end(); ++vit) {

        std::map<int, float>::iterator mit;
        for (mit = kResultIn_.m_mapExpBoost.begin(); mit != kResultIn_.m_mapExpBoost.end(); ++mit) {
            vit->m_mapExpBoost[mit->first] += mit->second;
        }

        for (mit = kResultIn_.m_mapSpxBoost.begin(); mit != kResultIn_.m_mapSpxBoost.end(); ++mit) {
            vit->m_mapSpxBoost[mit->first] += mit->second;
        }
    }

    kResultIn_.m_mapExpBoost.clear();
    kResultIn_.m_mapSpxBoost.clear();
}

void KUser::CalcGPBoostInfo(IN OUT KGameResultIn& kResultIn_, IN const float fStageClearRatio_)
{
    kResultIn_.m_nAddtionalGp = (int)(kResultIn_.m_nAddtionalGp * fStageClearRatio_);
    float fRatio = 0.f;
    std::map<int, float>::iterator mit;
    for (mit = kResultIn_.m_mapGpBoost.begin(); mit != kResultIn_.m_mapGpBoost.end(); ++mit) {
        fRatio += mit->second;
    }

    kResultIn_.m_nTotalRecvGP += (int)((kResultIn_.m_nBaseGP + kResultIn_.m_nAddtionalGp) * fRatio);

    //if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    std::Warn << L"--- Dump CalcGPBoostInfo ---" << dbg::endl
    //        << L" Login : " << GetName() << dbg::endl
    //        << L" kResultIn_.m_nTotalRecvGP : " << kResultIn_.m_nTotalRecvGP << dbg::endl
    //        << L" kResultIn_.m_nBaseGP : " << kResultIn_.m_nBaseGP << dbg::endl
    //        << L" kResultIn_.m_nAddtionalGp : " << kResultIn_.m_nAddtionalGp << dbg::endl
    //        << L" fRatio : " << fRatio << dbg::endl
    //        << L"--- End Dump ---" << dbg::endl;
    //}
}

void KUser::CalcCharacterBoostInfo(IN const int nModeID_, IN KUserPtr& spUser_, IN OUT std::vector< KCharacterExpInfo >& vecCharExp_, IN const float fStageClearRatio_)
{
    __int64 biMaxExp = SiKResultManager()->GetExpByLv(SiKResultManager()->GetMaxLevel());
    float fMaxAddRatio = SiKResultManager()->GetMaxAddRatio();
    int nMaxAddExp = SiKResultManager()->GetMaxAddExp();

    std::vector< KCharacterExpInfo >::iterator vit;
    for (vit = vecCharExp_.begin(); vit != vecCharExp_.end(); ++vit) {

        std::map<int, float>::iterator mit;

        float fExpRatio = 0.f;
        for (mit = vit->m_mapExpBoost.begin(); mit != vit->m_mapExpBoost.end(); ++mit) {
            fExpRatio += mit->second;
        }

        // 경험치 롤백에 대한 값체크 항목.
        // fExpRatio에 대한 체크.
        // fStageClearRatio_에 대한 체크.
        // vit->m_nBaseExp에 대한 체크.
        // vit->m_nAdditionnalExp에 대한 체크.
        if (vit->m_nBaseExp < 0 || vit->m_nAdditionnalExp < 0) {
            START_LOG(cerr, L"획득 경험치가 음수. LoginID : " << GetName())
                << BUILD_LOGc(vit->m_cCharType)
                << BUILD_LOG(vit->m_nBaseExp)
                << BUILD_LOG(vit->m_nAdditionnalExp)
                << BUILD_LOG(fStageClearRatio_)
                << BUILD_LOG((int)vit->m_nAdditionnalExp * fStageClearRatio_) << END_LOG;
        }

        // 이상값 체크.
        // 오버 플로우에 대한 체크는 어차피 다음 로직 처리에서 하기 때문에 여기서 중복 체크 의미 없음.
        if (spUser_) {
            KCharacterInfo* pkCharacterInfo = spUser_->GetCharacterInfo(vit->m_cCharType);
            if (pkCharacterInfo) {
                // 가용할 수 있는 경험치보다 큰가?
                if (biMaxExp <= vit->m_nBaseExp + vit->m_nAdditionnalExp) {
                    START_LOG(cerr, L"획득 경험치가 이상값. LoginID : " << GetName())
                        << BUILD_LOGc(vit->m_cCharType)
                        << BUILD_LOG(pkCharacterInfo->m_biInitExp)
                        << BUILD_LOG(pkCharacterInfo->m_biExp)
                        << BUILD_LOG(vit->m_nBaseExp)
                        << BUILD_LOG(vit->m_nAdditionnalExp) << END_LOG;
                }
            }
        }

        // ( 몬스터 경험치 + 추가 경험치( 던전 클리어 경험치 획득 부분 ) ) * fExpBoost 을 얻은 부분에 대한 확인.

        int nAdditionalExp = static_cast<int>(vit->m_nAdditionnalExp * fStageClearRatio_);
        int nTotalRecvExp = vit->m_nAdditionnalExp + nAdditionalExp;
        nTotalRecvExp += static_cast<int>((vit->m_nBaseExp) * fExpRatio);

        // 경험치 이상 획득에 대한 추가 로그 작업 및 보정 작업.
        if (false == SiKInfinityDungeon()->IsInfinityDungeon(nModeID_)) {
            if (nTotalRecvExp >= nMaxAddExp || vit->m_nBaseExp >= nMaxAddExp) {
                START_LOG(cerr, L"추가 획득 경험치가 스크립트에 정해놓은 수치값보다 많다.")
                    << BUILD_LOG(nTotalRecvExp)
                    << BUILD_LOG(vit->m_nBaseExp)
                    << BUILD_LOG(fExpRatio) << END_LOG;
            }
        }

        // 말이 되지 않는 추가 확률값.
        if (fExpRatio > fMaxAddRatio) {
            nTotalRecvExp = vit->m_nAdditionnalExp + nAdditionalExp;
            nTotalRecvExp += static_cast<int>((vit->m_nBaseExp) * fMaxAddRatio);

            START_LOG(cerr, L"추가 획득 확률값이 스크립트에 정해놓은 수치값보다 많다.")
                << BUILD_LOG(fExpRatio)
                << BUILD_LOG(fMaxAddRatio)
                << BUILD_LOG(nTotalRecvExp)
                << END_LOG;
        }

        vit->m_nTotalRecvExp = nTotalRecvExp;

        if (biMaxExp <= vit->m_nTotalRecvExp) {
            START_LOG(cerr, L"획득 경험치가 최대 경험치 Table의 통보다 많다.")
                << BUILD_LOG(vit->m_nTotalRecvExp)
                << BUILD_LOG(vit->m_nAdditionnalExp)
                << BUILD_LOG(fExpRatio) << END_LOG;
        }
    }
}

void KUser::AdjustResultInAndOut(IN const KGameInfo& kGameInfo_, IN OUT KGameResultIn& kResultIn_, IN const __int64 biMaxExp_, IN OUT KGameResultOut& kResultOut_)
{

    // 펫을 제외 하고 적용 한다.
    // 현재 정보를 Out 에 적용 한다.
    // In 을 User에 적용 한다.
    // In을 Out 에 적용한다.
    std::vector< KCharacterExpInfo  >::const_iterator vitExp;
    for (vitExp = kResultIn_.m_vecCharExp.begin(); vitExp != kResultIn_.m_vecCharExp.end(); ++vitExp)
    {
        // 해당 유저의 한수입니다.
        KCharacterInfo* pkCharacterInfo = GetCharacterInfo(vitExp->m_cCharType);
        if (pkCharacterInfo == NULL) continue;

        KCharacterExpInfoOut kOutData;
        kOutData.Clear();

        kOutData.m_cCharType = pkCharacterInfo->m_cCharType;
        kOutData.m_biOldExp = pkCharacterInfo->m_biExp;
        kOutData.m_nAdditionnalExp = vitExp->m_nAdditionnalExp;
        kOutData.m_kOldSkillInfo = pkCharacterInfo->m_kSkillInfo;
        kOutData.m_mapExpBoost = vitExp->m_mapExpBoost;
        kOutData.m_mapSpxBoost = vitExp->m_mapExpBoost;
        kOutData.m_nBaseExp = vitExp->m_nBaseExp;

        // 캐릭터에 적용.
        // EXP
        if (CheckExpCalcErr(ECC_ADJUSTRESULTINANDOUT_DEFAULT, kGameInfo_.m_iGameMode, 0, vitExp->m_cCharType, vitExp->m_nTotalRecvExp, pkCharacterInfo->m_biExp)) {
            __int64 biCurruntExp = pkCharacterInfo->m_biExp;
            biCurruntExp += vitExp->m_nTotalRecvExp;
            biCurruntExp = std::min<__int64>(biMaxExp_, biCurruntExp); // 최대 Exp 제한
            biCurruntExp = std::max<__int64>(biCurruntExp, USER_MIN_EXP); // 최소 Exp 보장

            LIF(CheckExpCalcErr(ECC_ADJUSTRESULTINANDOUT_CALC, kGameInfo_.m_iGameMode, 0, vitExp->m_cCharType, vitExp->m_nTotalRecvExp, pkCharacterInfo->m_biExp));
            pkCharacterInfo->m_biExp = std::max<__int64>(pkCharacterInfo->m_biExp, biCurruntExp);
            UpdateGrade();
        }

        DWORD dwOldLevel = pkCharacterInfo->m_dwLevel;
        // TotalExp( 누적 경험치 )
        pkCharacterInfo->m_biTotalExp = std::max<__int64>(0, pkCharacterInfo->m_biTotalExp + vitExp->m_nTotalRecvExp);
        pkCharacterInfo->m_biTotalExp = std::min<__int64>(pkCharacterInfo->m_biTotalExp, _I64_MAX);
        pkCharacterInfo->m_biTotalExp = std::max<__int64>(pkCharacterInfo->m_biTotalExp, USER_MIN_EXP);
        // 레벨 계산.
        pkCharacterInfo->m_dwLevel = SiKResultManager()->Exp_2_Level(pkCharacterInfo->m_biExp);

        // SPX 계산
        // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
        if (dwOldLevel < pkCharacterInfo->m_dwLevel) {
            SiKSkillManager()->GetSkillPointInfo(pkCharacterInfo->m_cCharType, pkCharacterInfo->m_cPromotion, pkCharacterInfo->m_dwLevel, pkCharacterInfo->m_kSkillInfo.m_nLvSPPoint);
        }

        // 현재 정보.가져옴
        kOutData.m_dwGetSPExp = vitExp->m_dwTotalRecvSpx;
        kOutData.m_biExp = pkCharacterInfo->m_biExp;
        kOutData.m_nTotalRecvExp = vitExp->m_nTotalRecvExp;
        kOutData.m_kSkillInfo = pkCharacterInfo->m_kSkillInfo;
        // ELO 정보 갱신.
        kOutData.m_nELORatingPoint = pkCharacterInfo->m_kELOUserData.m_nRatingPoint;
        kOutData.m_nELOWin = pkCharacterInfo->m_kELOUserData.m_nELOWin;
        kOutData.m_nELOLose = pkCharacterInfo->m_kELOUserData.m_nELOLose;
        kOutData.m_ucGrade = pkCharacterInfo->m_kELOUserData.m_ucGrade;
        kOutData.m_nMatchTotalCount = pkCharacterInfo->m_kELOUserData.m_nMatchTotalCount;
        kOutData.m_nELOType = pkCharacterInfo->m_kELOUserData.m_nELOType;
        //        kOutData.
        m_dwPlayCount[kOutData.m_cCharType] += 100000 + (m_bIsFinalQuest ? 1 : 0);
        // 결과 구조체에 넣기.
        kResultOut_.m_vecExps.push_back(std::make_pair(pkCharacterInfo->m_cCharType, kOutData));

        // WIN/LOSE
        UpdateWinLoseRecord(kGameInfo_.m_iGameMode, kResultIn_.m_bWin, (int)kGameInfo_.m_vecUserUIDs.size(), pkCharacterInfo->m_cCharType,
            pkCharacterInfo->m_iWin, pkCharacterInfo->m_iLose, kResultOut_);

        // 길드 추가처리
        if (dwOldLevel < pkCharacterInfo->m_dwLevel) {
            SendGuildMemberLevelupNotice(pkCharacterInfo->m_cCharType, (char)pkCharacterInfo->m_dwLevel);
        }
    }

    int nCurrentGP = kResultIn_.m_nTotalRecvGP;

    // 몬스터 원정대 예외처리
    if (GC_GM_MONSTER_CRUSADER == kGameInfo_.m_iGameMode) {
        nCurrentGP += kResultIn_.m_nBaseGP;
    }

    IncreaseGP(KGpStatInfo::GIT_GAME_CLEAR_BONUS, false, nCurrentGP);

    kResultOut_.m_nGamePoint = GetGP();
    kResultOut_.m_nBaseGP = kResultIn_.m_nBaseGP;
    kResultOut_.m_nTotalRecvGP = kResultIn_.m_nTotalRecvGP;
    kResultOut_.m_mapGpBoost = kResultIn_.m_mapGpBoost;
    kResultOut_.m_nStageBonusGP = kResultIn_.m_nAddtionalGp;

    //if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    std::Warn << L"-- Dump AdjustResultInAndOut --" << dbg::endl
    //        << L" Login : " << GetName() << dbg::endl
    //        << L" CurrentGP : " <<  nCurrentGP << dbg::endl
    //        << L" kResultOut_.m_nGamePoint : " << kResultOut_.m_nGamePoint << dbg::endl
    //        << L" kResultOut_.m_nBaseGP : " << kResultOut_.m_nBaseGP << dbg::endl
    //        << L" kResultOut_.m_nTotalRecvGP : " << kResultOut_.m_nTotalRecvGP << dbg::endl
    //        << L" kResultOut_.m_mapGpBoost.size() : " << kResultOut_.m_mapGpBoost.size() << dbg::endl
    //        << L" kResultOut_.m_nStageBonusGP : " << kResultOut_.m_nStageBonusGP << dbg::endl
    //        << L"--- end dump ---" << dbg::endl;
    //}
}

void KUser::FillBuyerData(IN OUT KBuySellItemReq& kBuyInfo_)
{
    const KCharacterInfo* pkChar = GetCurrentCharacter();
    if (pkChar != NULL) {
        kBuyInfo_.m_cCharType = pkChar->m_cCharType;
        kBuyInfo_.m_nCharLevel = pkChar->m_dwLevel;
    }
}

bool KUser::GetSocketItem(const IN GCITEMUID ItemUID_, OUT KItem& kItem_)
{
    KItemPtr pkItem = m_kInventory.FindItemByItemUID(ItemUID_);

    if (pkItem == NULL) {
        SET_ERROR(ERR_SOCKET_OPEN_00);
        return false;
    }

    if (pkItem) {
        kItem_ = *pkItem;
        return true;
    }

    return false;
}

bool KUser::CheckSocketState(IN std::vector<KSocketInfo>& vecSocket_, IN char cSlotID_, OUT KSocketInfo& kSocketInfo_, IN char SocketState_)
{
    std::vector<KSocketInfo>::iterator vitSocket;

    vitSocket = std::find_if(vecSocket_.begin(), vecSocket_.end(),
        boost::bind(&KSocketInfo::m_cSlotID, _1) == cSlotID_);

    if (vitSocket == vecSocket_.end()) {
        SET_ERROR(ERR_SOCKET_OPEN_03);
        return false;
    }

    if (vitSocket->m_cState != SocketState_) {
        SET_ERROR(ERR_SOCKET_OPEN_04);
        return false;
    }

    kSocketInfo_ = *vitSocket;
    return true;
}

bool KUser::UpdateSocketState(IN OUT KItem& kItem_, IN KSocketInfo& kSocketInfo_, IN char SocketState_, IN char NewSocketState_, IN bool bMonsterCardRelease_ /*=false */)
{
    KItemPtr pkItem = m_kInventory.FindItemByItemUID(kItem_.m_ItemUID);

    if (pkItem == NULL) {
        SET_ERROR(ERR_SOCKET_OPEN_00);
        return false;
    }

    std::vector<KSocketInfo>::iterator vitSocket;

    vitSocket = std::find_if(pkItem->m_vecSocket.begin(), pkItem->m_vecSocket.end(),
        boost::bind(&KSocketInfo::m_cSlotID, _1) == kSocketInfo_.m_cSlotID);

    // 슬롯 ID 에 해당하는 소켓이 있는가    
    if (vitSocket == pkItem->m_vecSocket.end()) {
        SET_ERROR(ERR_SOCKET_OPEN_03);
        return false;

    }

    // 소켓 상태 확인
    if (vitSocket->m_cState != SocketState_) {
        SET_ERROR(ERR_SOCKET_OPEN_04);
        return false;

    }

    // 소켓 상태 변경
    vitSocket->m_cState = NewSocketState_;
    vitSocket->m_CardItemID = kSocketInfo_.m_CardItemID;

    if (bMonsterCardRelease_) { // EVENT_MONSTER_CARD_REMOVE_ACK 에서 사용
        vitSocket->m_CardItemID = 0;
    }

    kItem_ = *pkItem; // ack 에 보낼 데이터에 적용
    return true;
}

bool KUser::CheckSocketCashItem(IN GCITEMID CashItemID_,
    IN KItem& kItem_,
    OUT KItem& CashItem_,
    IN bool bUseItem_ /*=false */)
{
    // 캐쉬 아이템 ID 확인
    if (CashItemID_ != SOCKET_OPEN_ITEM_ID) {
        SET_ERROR(ERR_SOCKET_OPEN_02);
        return false;
    }

    KItemPtr pkCashItem = m_kInventory.FindItemByItemID(CashItemID_);

    if (pkCashItem == NULL) {
        SET_ERROR(ERR_SOCKET_OPEN_01);
        return false;
    }
    if (pkCashItem) {
        CashItem_ = *pkCashItem;

        int nUseCount = 0;

        nUseCount = SiKItemManager()->GetUseSocketItemCount(kItem_);

        if (nUseCount > pkCashItem->m_nCount) {
            SET_ERROR(ERR_SOCKET_OPEN_01); // 캐쉬 아이템 수량 부족
            return false;
        }
        if (bUseItem_ == true) {
            pkCashItem->m_nCount -= nUseCount;
            CashItem_ = *pkCashItem;
        }
    }
    return true;
}

bool KUser::UseSocketCashItem(IN GCITEMID CashItemID_,
    IN KItem& kItem_,
    OUT KItem& CashItem_)
{
    if (CheckSocketCashItem(CashItemID_,
        kItem_,
        CashItem_,
        true) == false) {

        return false;
    }

    return true;
}

bool KUser::CheckSocketGP(IN KItem& kItem_)
{
    int nConsumeGP = 0;
    DWORD dwEquipLevel = 0;
    SiKItemManager()->GetItemEquipLevel(kItem_.m_ItemID, dwEquipLevel);
    if (dwEquipLevel == 0) {
        dwEquipLevel = static_cast<DWORD>(kItem_.m_sEquipLevel);
    }
    SiKItemManager()->m_kSocketCashItem.GetConsumeGP(dwEquipLevel, (int)kItem_.m_cGradeID, nConsumeGP);

    if (GetGP() < nConsumeGP) {
        SET_ERROR(ERR_SOCKET_OPEN_12);
        return false;
    }

    return true;
}

bool KUser::UseSocketGP(IN KItem& kItem_, OUT int& nGamePoint_)
{
    int nConsumeGP = 0;
    DWORD dwEquipLevel = 0;
    SiKItemManager()->GetItemEquipLevel(kItem_.m_ItemID, dwEquipLevel);
    if (dwEquipLevel == 0) {
        dwEquipLevel = static_cast<DWORD>(kItem_.m_sEquipLevel);
    }
    SiKItemManager()->m_kSocketCashItem.GetConsumeGP(dwEquipLevel, (int)kItem_.m_cGradeID, nConsumeGP);

    if (GetGP() < nConsumeGP) {
        SET_ERROR(ERR_SOCKET_OPEN_12);
        return false;
    }

    // 감소된 gp 보내주기
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP(KGpStatInfo::GDT_USE_SOCKET, bDBUpdate, nConsumeGP);
    nGamePoint_ = GetGP();
    return true;
}

int KUser::RecomLevelUpCheck(IN const int nOldLv_, IN const int nCurrentLv_, IN const int nRewardLv_, IN OUT std::vector<KDropItemInfo>& vecItem_)
{
    KDB_EVENT_RECOMMEND_LEVELUP_NOT kDBPacket;
    kDBPacket.Clear();
    kDBPacket.m_nCurrentLevel = nCurrentLv_;

    int nCount = 0;

    // 이벤트 기간인지?
    if (SiKRecomHelper()->IsEventTerm()) {
        // 신규나 복귀 유저인지?
        if ((IsNewRecomUser() || IsReturnRecomUser()) && m_MyRecomInfo.m_nState != KRecomHelper::RBS_WAIT_RECOM) {
            nCount = SiKRecomHelper()->AddLevelUpReward(nOldLv_, nCurrentLv_, nRewardLv_, vecItem_);
            kDBPacket.m_dwUserUID = m_MyRecomInfo.m_dwLoginUID;
        }
    }

    // 반갑다 친구야 이벤트 아이템이 지급되었으면 해당 아이템의 지급 로그를 DB에 쓰자.
    if (nCount > 0 || kDBPacket.m_bReturnLevelUpCheck) {
        QUEUEING_EVENT_TO_DB(DB_EVENT_RECOMMEND_LEVELUP_NOT, kDBPacket);
    }

    START_LOG(clog, L"RecomLevelUpCheck")
        << BUILD_LOG(SiKRecomHelper()->IsEventTerm())
        << BUILD_LOG(IsNewRecomUser())
        << BUILD_LOG(IsReturnRecomUser())
        << BUILD_LOG(nCount)
        << BUILD_LOG(nOldLv_)
        << BUILD_LOG(nCurrentLv_)
        << BUILD_LOG(vecItem_.size())
        << BUILD_LOG(kDBPacket.m_bReturnLevelUpCheck)
        << BUILD_LOG(SiKRecomHelper()->IsRecomLevel(nOldLv_, nCurrentLv_)) << END_LOG;

    return nCount;
}

bool KUser::GetItemBreakupCost(IN const KItem& kItem_, OUT int& nBreakupCost_)
{
    nBreakupCost_ = 0;
    DWORD dwEquipLevel = 0;
    SiKItemManager()->GetItemEquipLevel(kItem_.m_ItemID, dwEquipLevel);
    if (dwEquipLevel == 0) {
        dwEquipLevel = static_cast<DWORD>(kItem_.m_sEquipLevel);
    }
    if (!SiKItemManager()->m_kSocketCashItem.GetConsumeGP(dwEquipLevel, (int)kItem_.m_cGradeID, nBreakupCost_))
        return false;

    nBreakupCost_ = (int)(nBreakupCost_ * SiKItemManager()->m_kItemBreakup.GetPriceRatio()); // 비율 적용.
    return true;
}

void KUser::UpdateBuyCheckItemCount(IN const GCITEMID dwItemID_)
{
    // DB에 해당 아이템의 정보를 보내주고 구매한 Count를 올리자.
    QUEUEING_EVENT_TO_DB(DB_EVENT_UPDATE_BUY_CHECK_COUNT_REQ, dwItemID_);
}

bool KUser::SendGuildListReqToDB(IN const int nPageNum_, IN const UCHAR ucSortType_, IN const UCHAR ucOrderBy_)
{
    std::vector<DWORD> vecPage;
    std::vector<DWORD> vecTemp;
    int nPageSize = SiKGuildManager()->GetGuildPageSize();

    vecPage.clear();
    SiKGuildManager()->GetGuildPageIDListSort(ucSortType_, vecTemp);

    int nGuildCount = 0;
    int nCurrentPageNum = 1;

    std::vector<DWORD>::iterator vit;
    for (vit = vecTemp.begin(); vit != vecTemp.end(); ++vit) {
        // 빌드해야 할 페이지 번호가 맞으면 GuildUID 채워넣기.
        if (nCurrentPageNum == nPageNum_) {
            vecPage.push_back(*vit);

            if (vecPage.size() < (UINT)nPageSize) {
                continue;
            }
            break;
        }

        ++nGuildCount;
        if (nGuildCount < nPageSize) {
            continue;
        }

        // 페이지 채우면 페이지번호 증가
        ++nCurrentPageNum;
        nGuildCount = 0;
    }

    if (vecPage.empty()) {
        START_LOG(cerr, L"빌드할 페이지 정보가 비어있음.")
            << BUILD_LOG(nPageNum_)
            << BUILD_LOGc(ucSortType_)
            << BUILD_LOGc(ucOrderBy_) << END_LOG;
        return false;
    }

    KGuildPageBuild kDBPacket;
    kDBPacket.m_nPagenum = nPageNum_;
    kDBPacket.m_nTotalPageNum = SiKGuildManager()->GetTotalGuildPageNum();
    kDBPacket.m_ucSortType = ucSortType_;
    kDBPacket.m_ucOrderBy = ucOrderBy_;
    kDBPacket.m_vecGuildUID.swap(vecPage);

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_GUILD_LIST_REQ, kDBPacket);

END_PROC:
    START_LOG(clog, L"SendGuildListReqToDB")
        << BUILD_LOG(kDBPacket.m_nPagenum)
        << BUILD_LOG(kDBPacket.m_nTotalPageNum)
        << BUILD_LOGc(kDBPacket.m_ucSortType)
        << BUILD_LOGc(kDBPacket.m_ucOrderBy)
        << BUILD_LOG(kDBPacket.m_vecGuildUID.size()) << END_LOG;

    return true;
}

void KUser::SendUpdateGuildMemberNickNotice()
{
    // 길드원이면 닉네임 변경정보 갱신
    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return;
    }

    KGuildUserName KCnPacket;
    KCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    KCnPacket.m_dwUserUID = GetUID();
    KCnPacket.m_strNickName = GetNickName();

    LIF(SiKCnConnector()->SendPacket(KCenterEvent::ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KCnPacket));

    START_LOG(clog, L"센터서버에 길드 맴버 닉네임 변경 알림, GuildUID :" << m_kGuildUserInfo.m_dwGuildUID)
        << BUILD_LOG(GetUID())
        << BUILD_LOG(GetName())
        << BUILD_LOG(GetNickName()) << END_LOG;
}

void KUser::SendGuildMemberLevelupNotice(IN const char cCharType_, IN const char cLevel_)
{
    // 길드원이면 레벨업 알림
    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return;
    }

    KGuildUserCharacter kCnPacket;
    kCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kCnPacket.m_dwUserUID = GetUID();
    kCnPacket.m_cCharType = cCharType_;
    kCnPacket.m_cLevel = cLevel_;

    LIF(SiKCnConnector()->SendPacket(KCenterEvent::ECN_GUILD_MEMBER_LEVELUP_NOT, kCnPacket));

    START_LOG(clog, L"센터서버에 길드 맴버 레벨업 알림, GuildUID :" << m_kGuildUserInfo.m_dwGuildUID)
        << BUILD_LOG(GetUID())
        << BUILD_LOG(GetName())
        << BUILD_LOGc(cCharType_)
        << BUILD_LOGc(cLevel_) << END_LOG;
}

void KUser::SendGuildPageReq(IN const UCHAR ucSortType_)
{
    LIF(SiKCnConnector()->SendPacket(KCenterEvent::ECN_GUILD_PAGE_REQ, ucSortType_));
    START_LOG(clog, L"센터서버에 길드 페이지정보 요청. SortType :" << (int)ucSortType_) << END_LOG;
}

bool KUser::IsPremiumUser()
{
    bool bPremiumUser = false;

    std::map<GCITEMID, USHORT> mapPremiumItemID;
    SiKGCHelper()->GetGCClubItemID(mapPremiumItemID);

    std::map<GCITEMID, USHORT>::const_iterator cmit;
    for (cmit = mapPremiumItemID.begin(); cmit != mapPremiumItemID.end(); ++cmit) {
        if (m_kInventory.FindItemByItemID(cmit->first) == NULL) {
            continue;
        }

        bPremiumUser = true;
        break;
    }
    return bPremiumUser;
}

void KUser::CheckInventoryPremiumItem(OUT KPremiumInfo& kPremiumInfo_)
{
    kPremiumInfo_.Init();

    std::map<GCITEMID, USHORT> mapPremiumItemID;
    SiKGCHelper()->GetGCClubItemID(mapPremiumItemID);

    std::map<GCITEMID, USHORT>::const_iterator cmit;
    for (cmit = mapPremiumItemID.begin(); cmit != mapPremiumItemID.end(); ++cmit) {
        if (m_kInventory.FindItemByItemID(cmit->first) == NULL) {
            continue;
        }

        kPremiumInfo_.AddType(cmit->second);
    }

    std::map<int, std::set<GCITEMID> > mapTicketType;
    SiKGCHelper()->GetDungeonTicketTypeList(mapTicketType);

    std::map<int, std::set<GCITEMID> >::const_iterator cmitTicket;
    for (cmitTicket = mapTicketType.begin(); cmitTicket != mapTicketType.end(); ++cmitTicket) {

        std::set<GCITEMID>::const_iterator csit;
        for (csit = cmitTicket->second.begin(); csit != cmitTicket->second.end(); ++csit) {
            KItemPtr pkItem = m_kInventory.FindItemByItemID(*csit);
            if (pkItem == NULL) {
                continue;
            }

            if (pkItem->m_nCount == 0) {
                continue;
            }

            kPremiumInfo_.AddType(cmitTicket->first);

            START_LOG(clog, L"유저 보유한 프리미엄 아이템. Type : " << cmitTicket->first)
                << BUILD_LOG(cmitTicket->second.size())
                << BUILD_LOG(pkItem->m_nCount)
                << BUILD_LOG(pkItem->m_nInitCount)
                << BUILD_LOG(pkItem->m_ItemID)
                << BUILD_LOG(pkItem->m_ItemUID) << END_LOG;
            break;
        }
    }
}

bool KUser::IsPremiumItem(IN const GCITEMID ItemID_)
{
    std::map<GCITEMID, USHORT> mapPremiumItemID;
    SiKGCHelper()->GetGCClubItemID(mapPremiumItemID);
    return (mapPremiumItemID.find(ItemID_) != mapPremiumItemID.end());
}

void KUser::AddStatDungeon(IN KGameResultIn& kResultIn_, IN int nType_, IN const int nTotalUsers)
{
    // 던전 통계
    if (m_pkRoom == NULL)
        return;

    if (KRoom::IsDungeonMode(m_pkRoom->m_iGameMode) == false)
        return;

    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if (pkCharacter == NULL)
        return;

    KDungeonStat kData;
    kData.m_nGameMode = m_pkRoom->m_iGameMode;
    kData.m_nStageID = kResultIn_.m_nLastStageID;
    kData.m_nCharLevel = static_cast<int>(pkCharacter->m_dwLevel);
    kData.m_nPlayTime = static_cast<int>((::timeGetTime() - m_pkRoom->m_dwStartGameTime) / (1000 * 60));
    kData.m_nLifeCount = kResultIn_.m_nLifeCount;
    kData.m_nRegenCount = kResultIn_.m_nRegenCount;
    kData.m_nClearStageCount = kResultIn_.m_nClearStageCount;
    kData.m_nResult = kResultIn_.m_nResult;

    KStatDungeonPtr spStat = GET_STAT(KStatDungeon, SI_STAT_DUNGEON);
    if (spStat != NULL) {
        spStat->AddStat(kData);
    }

    int nDeductVitality = 0;
    SiKVitalityManager()->GetDeductVitality(m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, nDeductVitality);

    // 활력 시스템 통계(활력이 충분하지 못한 상태로 활력을 소모하는 던전을 플레이한 경우에만 기록함)
    int nRemainVitality = 0;
    if (true == m_pkRoom->GetRemainVitalityOnStartGame(GetUID(), static_cast<int>(pkCharacter->m_cCharType), nRemainVitality)) {
        if (nRemainVitality < nDeductVitality) {
            // LoginUID, ModeID, Difficulty, PlayTime, Party, CharType, CharLevel, 종료 타입(0: 정상종료, 1: 게임 중 나가기), RemainVitality
            KEVENT_STAT_VITALITY_DUNGEON_PLAY_NOT kPacket;
            kPacket.m_dwLoginUID = GetUID();
            kPacket.m_nModeID = m_pkRoom->m_iGameMode;
            kPacket.m_nDifficulty = m_pkRoom->m_nDifficulty;
            kPacket.m_dwPlayTime = static_cast<int>((::timeGetTime() - m_pkRoom->m_dwStartGameTime) / (1000 * 60));
            kPacket.m_nTotalUsers = nTotalUsers;
            kPacket.m_nCharType = static_cast<int>(pkCharacter->m_cCharType);
            kPacket.m_nCharLevel = static_cast<int>(pkCharacter->m_dwLevel);
            kPacket.m_nRemainVitality = nRemainVitality;
            if (KEVENT_STAT_DUNGEON_STAGE_NOT::END_GAME == nType_) {
                kPacket.m_nClearType = KEVENT_STAT_VITALITY_DUNGEON_PLAY_NOT::ESVDPNT_ENDGAME;
            }
            else if (KEVENT_STAT_DUNGEON_STAGE_NOT::LEAVE_GAME == nType_) {
                kPacket.m_nClearType = KEVENT_STAT_VITALITY_DUNGEON_PLAY_NOT::ESVDPNT_LEAVE;
            }
            else {
                kPacket.m_nClearType = 0;
            }

            QUEUEING_EVENT_TO_DB(EVENT_STAT_VITALITY_DUNGEON_PLAY_NOT, kPacket);
        }
    }

    // 던전 스테이지 통계
    if (!kResultIn_.m_mapDungeonPlayState.empty()) {
        KEVENT_STAT_DUNGEON_STAGE_NOT kPacket;
        kPacket.m_nType = nType_;
        kPacket.m_dwCharLevel = pkCharacter->m_dwLevel;
        kPacket.m_mapDungeonPlayState = kResultIn_.m_mapDungeonPlayState;
        QUEUEING_EVENT_TO_DB(EVENT_STAT_DUNGEON_STAGE_NOT, kPacket);
    }

    if (!kResultIn_.m_vecDungeonCharStat.empty()) {
        std::vector<KDungeonCharInfo>::iterator vit;
        for (vit = kResultIn_.m_vecDungeonCharStat.begin(); vit != kResultIn_.m_vecDungeonCharStat.end(); ++vit) {
            vit->m_dwLoginUID = kResultIn_.m_dwUID;
            vit->m_dwCharType = static_cast<DWORD>(pkCharacter->m_cCharType);
            vit->m_dwLevel = pkCharacter->m_dwLevel;
            vit->m_dwDungeonID = m_pkRoom->m_iGameMode;
            vit->m_dwDifficult = static_cast<int>(m_pkRoom->m_nDifficulty);
            vit->m_dwPlayTime = (DWORD)(::timeGetTime() - m_pkRoom->m_dwStartGameTime) / 1000;
        }

        QUEUEING_EVENT_TO_DB(EVENT_STAT_DUNGEON_CHARACTER_NOT, kResultIn_.m_vecDungeonCharStat);
    }

    if (kResultIn_.m_prUseSkillCount.first != 0 || kResultIn_.m_prUseSkillCount.second != 0) {
        QUEUEING_EVENT_TO_DB(EVENT_STAT_CHARACTER_SKILL_COUNT, kResultIn_.m_prUseSkillCount);
    }

    // 캐릭터 던전 시야 등급 시간에 대한 통계.
    if (!kResultIn_.m_kUserViewGradeTime.m_mapViewGradeTime.empty()) {
        kResultIn_.m_kUserViewGradeTime.m_dwUserUID = kResultIn_.m_dwUID;
        kResultIn_.m_kUserViewGradeTime.m_dwDungeonID = m_pkRoom->m_iGameMode;
        kResultIn_.m_kUserViewGradeTime.m_dwCharType = (DWORD)pkCharacter->m_cCharType;
        QUEUEING_EVENT_TO_DB(EVENT_STAT_VIEW_GRADE_TIME_NOT, kResultIn_.m_kUserViewGradeTime);
    }

    {
        // 중국 CCB 통계 추가.
        KEVENT_STAT_DUNGEON_LEAVE_NOT kDBPacket;
        kDBPacket.m_nGameMode = m_pkRoom->m_iGameMode;
        kDBPacket.m_nDifficult = m_pkRoom->m_nDifficulty;
        kDBPacket.m_dwUserUID = kResultIn_.m_dwUID;
        kDBPacket.m_cCharType = pkCharacter->m_cCharType;
        kDBPacket.m_wstrGameStartDate = m_pkRoom->m_tmGameStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
        if (nType_ == KEVENT_STAT_DUNGEON_STAGE_NOT::LEAVE_GAME) {
            CTime tmCurrentTime = CTime::GetCurrentTime();
            kDBPacket.m_wstrGameEndDate = tmCurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
        }
        else {
            kDBPacket.m_wstrGameEndDate = m_pkRoom->m_tmGameEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));
        }
        kDBPacket.m_nType = nType_;
        QUEUEING_EVENT_TO_DB(EVENT_STAT_DUNGEON_LEAVE_NOT, kDBPacket);
    }

    // 달성 과제 시스템 통계
    if (false == kResultIn_.m_kDungeonSubjectStat.m_mapSubjectClearInfo.empty()) {
        // 클라이언트에서 통계 구조체에 담아주는 정보를 일부 무시하고, KGameResultIn의 값을 사용하여 기록한다
        kResultIn_.m_kDungeonSubjectStat.m_dwLoginUID = kResultIn_.m_dwUID;
        kResultIn_.m_kDungeonSubjectStat.m_nGameMode = m_pkRoom->m_iGameMode;
        kResultIn_.m_kDungeonSubjectStat.m_nDifficulty = m_pkRoom->m_nDifficulty;
        // 아래 정보는 클라이언트에서 담아주는 정보로 기록한다
        //kResultIn_.m_kDungeonSubjectStat.m_nCharType = static_cast< int >( pkCharacter->m_cCharType );
        //kResultIn_.m_kDungeonSubjectStat.m_nCharLevel = static_cast< int >( pkCharacter->m_dwLevel );
        //kResultIn_.m_kDungeonSubjectStat.m_nCharPromotion = static_cast< int >( pkCharacter->m_cCurrentPromotion ); // 테스트시 m_cCurrentPromotion의 정보가 클라랑 다른 문제가 있었다

        QUEUEING_EVENT_TO_DB(EVENT_STAT_DUNGEON_SUBJECT_NOT, kResultIn_.m_kDungeonSubjectStat);
    }
}

bool KUser::IsNewRecomUser()
{
    if (m_MyRecomInfo.m_dwLoginUID == 0) return false;
    return (SiKRecomHelper()->IsNewBeginTerm(KncUtil::TimeToInt(m_tmFirstLoginTime)));
}

bool KUser::PacketHackCheck(IN const int nFrom_)
{
    if (false == SiKGSSimLayer()->GetPacketHackCheck()) {
        return true;
    }

    if (nFrom_ == KUserEvent::FT_INNER) {
        return true;
    }

    std::wstringstream stm;
    stm << KNC_TIME_STRING << L"|" << GetName() << L"|" << KUserEvent::EVENT_BUY_FOR_CASH_ACK << L"|" << nFrom_;

    TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_PACKET_HACK, GetName(), GetUID(), 0, stm.str());

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    START_LOG(cerr, L"패킷 해킹. LoginID : " << GetName() << L",EventID : " << KUserEvent::EVENT_BUY_FOR_CASH_ACK << L",from :" << nFrom_) << END_LOG;
    return false;
}

void KUser::ClearPressInfo()
{
    m_kPressInfo.m_dwElapsedTime = ::GetTickCount();
    m_kPressInfo.m_nState = KPressureState::PS_NORMAL;
}

void KUser::SetPressInfo(IN const int nState_)
{
    m_kPressInfo.m_nState = nState_;
    m_kPressInfo.m_dwElapsedTime = ::GetTickCount();
}

void KUser::GetPressInfo(OUT int& nState_, OUT DWORD& dwElapsedTime_)
{
    nState_ = m_kPressInfo.m_nState;
    dwElapsedTime_ = ::GetTickCount() - m_kPressInfo.m_dwElapsedTime;
}

void KUser::ClearIdleInfo()
{
    m_nIdleState = 0;
}

void KUser::SetIdleInfo(IN const bool bIdle_)
{
    m_nIdleState = (bIdle_ == true ? 1 : 0);
}

void KUser::GetIdleInfo(OUT int& nIdleState_)
{
    nIdleState_ = m_nIdleState;
}

void KUser::SetEventMonsterInfo(IN const int nModeID_, IN const int nDifficulty_, OUT std::vector<KEventMonster>& vecMonster_, OUT std::vector<KDropItemInfo>& vecPartyRewardInfo_)
{
    std::pair<int, int> prStageCount; // min,max stageCount
    std::set<int> setMonsterID;
    std::vector< KEventMonInfo > vecMonsterInfo;
    bool bEventOn = false;
    bool bEclipseEventOn = false;

    vecPartyRewardInfo_.clear();

    if (false == SiKEventMonsterManager()->IsActiveEvent()) {
        START_LOG(clog, L"이벤트몬스터 발동중이 아님.") << END_LOG;
        return;
    }

    if (false == SiKResultManager()->GetStageCountInfo(nModeID_, nDifficulty_, prStageCount)) {
        START_LOG(clog, L"해당 스테이지 정보가 없음. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
        return;
    }

    bEventOn = SiKEventMonsterManager()->CheckEvent(nModeID_, setMonsterID);
    if (!bEventOn) {
        START_LOG(clog, L"해당 던전에 이벤트몬스터 정보 없음. ModeID : " << nModeID_) << END_LOG;
    }

    if (SiKEclipseTimeEvent()->IsEventOn()) {
        bEclipseEventOn = SiKEventMonsterManager()->CheckEclipseEvent(nModeID_, setMonsterID);
        if (!bEclipseEventOn) {
            START_LOG(clog, L"해당 던전에 Eclipse 이벤트몬스터 정보 없음. ModeID : " << nModeID_) << END_LOG;
        }
    }

    if (!bEclipseEventOn && !bEventOn) {
        return;
    }

    std::set<int>::iterator sit;
    for (sit = setMonsterID.begin(); sit != setMonsterID.end(); ++sit) {
        KEventMonInfo kInfo;
        if (false == SiKEventMonsterManager()->GetEventMonInfo(*sit, kInfo)) {
            continue;
        }

        vecMonsterInfo.push_back(kInfo);
    }

    START_LOG(clog, L"진행중인 이벤트 몬스터 정보 size : " << vecMonsterInfo.size())
        << BUILD_LOG(nModeID_)
        << BUILD_LOG(nDifficulty_)
        << BUILD_LOG(prStageCount.first)
        << BUILD_LOG(prStageCount.second) << END_LOG;

    PAIR_INT prDungeonLv;
    SiKEventMonsterManager()->GetDungeonLevel(nModeID_, nDifficulty_, prDungeonLv);

    // 이벤트 몬스터 정보 설정
    std::vector< KEventMonInfo >::iterator vit;
    for (vit = vecMonsterInfo.begin(); vit != vecMonsterInfo.end(); ++vit) {
        KEventMonster kMonsterInfo;
        kMonsterInfo.m_nMonsterID = vit->m_nMonsterID;
        kMonsterInfo.m_bIsBoss = vit->m_bIsBoss;
        kMonsterInfo.m_nSpecial = vit->m_nSpecial;

        if (vit->m_bAutoLevel && prDungeonLv.first > 0) {
            kMonsterInfo.m_nMonLv = prDungeonLv.first;
        }
        else {
            kMonsterInfo.m_nMonLv = SiKEventMonsterManager()->GetMonsterLevel(vit->m_nMonsterID);
        }

        float fMonsterAppearRate = 0.f;
        std::map< int, float >::iterator mitAppear = vit->m_mapAppearRate.find(nDifficulty_);

        if (vit->m_mapAppearRate.end() != mitAppear) {
            fMonsterAppearRate = mitAppear->second;
        }

        kMonsterInfo.m_nTriggerID = -1;
        kMonsterInfo.m_nStageNum = -1;

        // 이벤트 몬스터의 파티 전체 획득 아이템 정보 세팅(room 정보에 세팅할거임)
        std::vector<std::pair<KDropItemInfo, float> >::iterator vitParty;
        for (vitParty = vit->m_vecPartyReward.begin(); vitParty != vit->m_vecPartyReward.end(); vitParty++) {
            vecPartyRewardInfo_.push_back(vitParty->first);
        }

        if (kMonsterInfo.m_bIsBoss ||
            kMonsterInfo.m_nSpecial == KEventMonster::OPT_LUPIN ||
            kMonsterInfo.m_nSpecial == KEventMonster::OPT_GATE) {
            // 던전당 1회 출현
            if (DropDecision(fMonsterAppearRate * 100)) {

                // 등장하는 스테이지가 정해져 있는가
                if (vit->m_mapModeStage[nModeID_].empty()) {
                    kMonsterInfo.m_nStage = (prStageCount.second > 0 ? SiKGameDrop()->GetRandomNum() % prStageCount.second : -1);
                    START_LOG(clog, L"랜덤 스테이지내에서 이벤트 몬스터 등장 설정..")
                        << BUILD_LOG(vit->m_nMonsterID)
                        << BUILD_LOG(nModeID_)
                        << BUILD_LOG(prStageCount.second)
                        << BUILD_LOG(kMonsterInfo.m_nStage) << END_LOG;
                }
                else {
                    std::random_shuffle(vit->m_mapModeStage[nModeID_].begin(), vit->m_mapModeStage[nModeID_].end());
                    kMonsterInfo.m_nStage = *(vit->m_mapModeStage[nModeID_].begin());
                    START_LOG(clog, L"정해진 스테이지내에서 이벤트 몬스터 등장 설정..")
                        << BUILD_LOG(vit->m_nMonsterID)
                        << BUILD_LOG(nModeID_)
                        << BUILD_LOG(vit->m_mapModeStage[nModeID_].size())
                        << BUILD_LOG(kMonsterInfo.m_nStage) << END_LOG;
                }
                kMonsterInfo.m_vecReward.clear();

                // drop item
                SetEventMonsterDropItem(*vit, kMonsterInfo, vecMonster_);
            }
        }
        else {
            // 스테이지마다 출현 가능
            if (vit->m_mapModeStage[nModeID_].empty()) {
                for (int i = 0; i < prStageCount.second; ++i) {
                    kMonsterInfo.m_nStage = i;
                    kMonsterInfo.m_vecReward.clear();

                    if (false == DropDecision(fMonsterAppearRate * 100)) {
                        continue;
                    }

                    // drop item
                    SetEventMonsterDropItem(*vit, kMonsterInfo, vecMonster_);
                }
            }
            else {
                std::vector<int>::iterator vitStage;
                for (vitStage = vit->m_mapModeStage[nModeID_].begin(); vitStage != vit->m_mapModeStage[nModeID_].end(); ++vitStage) {
                    kMonsterInfo.m_nStage = *vitStage;
                    kMonsterInfo.m_vecReward.clear();

                    if (false == DropDecision(fMonsterAppearRate * 100)) {
                        continue;
                    }

                    // drop item
                    SetEventMonsterDropItem(*vit, kMonsterInfo, vecMonster_);
                }
            }
        }
    }

    int nTriggerID = INT_MAX;
    // TriggerID setting
    std::vector<KEventMonster>::iterator vitMonster;
    for (vitMonster = vecMonster_.begin(); vitMonster != vecMonster_.end(); ++vitMonster) {
        vitMonster->m_nTriggerID = nTriggerID;
        --nTriggerID;
    }

    START_LOG(clog, L"이벤트 몬스터 설정됨. size : " << vecMonster_.size())
        << BUILD_LOG(nModeID_)
        << BUILD_LOG(nDifficulty_) << END_LOG;
}

bool KUser::DropDecision(IN const float fRate_)
{
    KLottery kLottery;
    kLottery.AddCase(1, fRate_);
    return (kLottery.Decision() != KLottery::CASE_BLANK);
}

void KUser::SetEventMonsterDropItem(IN KEventMonInfo& kEventMonsterInfo_, IN KEventMonster& kEventMonster_, OUT std::vector<KEventMonster>& vecEventMonster_)
{
    // drop item
    std::vector<std::pair<KDropItemInfo, float> >::iterator vitDrop;
    for (vitDrop = kEventMonsterInfo_.m_vecReward.begin(); vitDrop != kEventMonsterInfo_.m_vecReward.end(); ++vitDrop) {
        const float& fDropRate = vitDrop->second;

        if (DropDecision(fDropRate * 100)) {
            kEventMonster_.m_vecReward.push_back(vitDrop->first);
        }
    }

    // party drop item
    std::vector<std::pair<KDropItemInfo, float> >::iterator vitPartyDrop;
    for (vitPartyDrop = kEventMonsterInfo_.m_vecPartyReward.begin(); vitPartyDrop != kEventMonsterInfo_.m_vecPartyReward.end(); ++vitPartyDrop) {
        const float& fDropRate = vitPartyDrop->second;

        if (DropDecision(fDropRate * 100)) {
            kEventMonster_.m_vecReward.push_back(vitPartyDrop->first);
        }
    }

    vecEventMonster_.push_back(kEventMonster_);

    //childmonster setting
    std::vector<KEventChildMonInfo>::iterator vitChildMon;
    for (vitChildMon = kEventMonsterInfo_.m_vecChildMonster.begin(); vitChildMon != kEventMonsterInfo_.m_vecChildMonster.end(); ++vitChildMon)
    {
        KEventMonster kChildMonsterInfo;
        kChildMonsterInfo = kEventMonster_;
        kChildMonsterInfo.m_nMonsterID = vitChildMon->m_nChildMonsterID;
        kChildMonsterInfo.m_vecReward.clear();

        std::vector<std::pair<KDropItemInfo, float> >::iterator vitChildMonsterDrop;
        for (vitChildMonsterDrop = vitChildMon->m_vecChildReward.begin(); vitChildMonsterDrop != vitChildMon->m_vecChildReward.end(); ++vitChildMonsterDrop) {
            const float& fDropRate = vitChildMonsterDrop->second;

            if (DropDecision(fDropRate * 100)) {
                kChildMonsterInfo.m_vecReward.push_back(vitChildMonsterDrop->first);
            }
        }
        vecEventMonster_.push_back(kChildMonsterInfo);
    }
}

void KUser::AddEventMonsterDrop(IN const std::vector<KEventMonster>& vecEventMon_, IN const std::vector<DWORD>& m_vecUserUIDs_, OUT std::vector<KGameDropElement>& vecDrop_)
{
    int nDropCount = 0;
    DWORD dwUserSize = m_vecUserUIDs_.size();

    // 이벤트몬스터 드랍아이템 정보에 같이 넣어주기.
    std::vector<KEventMonster>::const_iterator cvit;
    for (cvit = vecEventMon_.begin(); cvit != vecEventMon_.end(); ++cvit) {
        if (cvit->m_vecReward.empty()) {
            continue;
        }

        std::vector<KDropItemInfo>::const_iterator cvitDrop;
        for (cvitDrop = cvit->m_vecReward.begin(); cvitDrop != cvit->m_vecReward.end(); ++cvitDrop) {
            KGameDropElement kDropEl;
            kDropEl.m_dwUID = 0;
            kDropEl.m_dwUserUID = (dwUserSize > 0 ? m_vecUserUIDs_[SiKGameDrop()->GetRandomNum() % dwUserSize] : 0);
            kDropEl.m_nTriggerID = cvit->m_nTriggerID;
            kDropEl.m_nMonID = cvit->m_nMonsterID;
            kDropEl.m_nStageCountType = -1;
            kDropEl.m_cRewardType = KGameDropElement::RT_ITEM;
            kDropEl.m_dwItemID = cvitDrop->m_ItemID;
            kDropEl.m_nCount = cvitDrop->m_nDuration; // 수량
            kDropEl.m_nPeriod = cvitDrop->m_nPeriod; // 영구(-1)/기간
            kDropEl.m_nStrongLV = 0;
            kDropEl.m_fRatio = 1.f;
            vecDrop_.push_back(kDropEl);
            ++nDropCount;
        }
    }

    START_LOG(clog, L"이벤트몬스터 드랍 추가 설정. size : " << nDropCount)
        << BUILD_LOG(vecDrop_.size())
        << BUILD_LOG(vecEventMon_.size())
        << BUILD_LOG(dwUserSize) << END_LOG;
}

void KUser::AddEventMonsterTrigger(IN const std::vector<KEventMonster>& vecEventMon_, IN OUT std::vector<sTriggerInfo>& vecTriggers_)
{
    // 이벤트몬스터 트리거 정보에 삽입
    std::vector<KEventMonster>::const_iterator cvit;
    for (cvit = vecEventMon_.begin(); cvit != vecEventMon_.end(); ++cvit) {
        sTriggerInfo sInfo;
        sInfo.m_nTriggerID = cvit->m_nTriggerID;
        sInfo.m_nMonsterID = cvit->m_nMonsterID;
        sInfo.m_nMonLv = cvit->m_nMonLv;
        sInfo.m_nStage = cvit->m_nStage;
        sInfo.m_nIsBoss = cvit->m_bIsBoss;
        vecTriggers_.push_back(sInfo);
    }
}

void KUser::AddEventMonsterChampion(IN const int nDifficulty_, IN const std::vector<sTriggerInfo>& vecTriggers_, IN const std::vector<KEventMonster>& vecEventMon_, IN OUT std::vector<KChampionInfo>& vecChampions_)
{
    // 챔피언 모드에서 이벤트 몬스터를 100% 챔피언으로 설정
    if (nDifficulty_ < GC_GMD_CHAMPION_BEGIN) {
        return;
    }

    int nBossTriggerID = 0;
    DWORD dwProperty = 1;

    std::vector<sTriggerInfo>::const_iterator cvitTrigger;
    cvitTrigger = std::find_if(vecTriggers_.begin(), vecTriggers_.end(),
        boost::bind(&sTriggerInfo::m_nIsBoss, _1) > 0);
    if (cvitTrigger != vecTriggers_.end()) {
        nBossTriggerID = cvitTrigger->m_nTriggerID;
    }

    if (nBossTriggerID > 0) {
        std::vector<KChampionInfo>::const_iterator cvitChamp;
        cvitChamp = std::find_if(vecChampions_.begin(), vecChampions_.end(),
            boost::bind(&KChampionInfo::m_nTriggerID, _1) == nBossTriggerID);

        if (cvitChamp != vecChampions_.end()) {
            dwProperty = cvitChamp->m_dwProperty;
        }
    }

    std::vector<KEventMonster>::const_iterator cvit;
    for (cvit = vecEventMon_.begin(); cvit != vecEventMon_.end(); ++cvit) {
        KChampionInfo kInfo;
        kInfo.m_nTriggerID = cvit->m_nTriggerID;
        kInfo.m_nMonsterID = cvit->m_nMonsterID;
        kInfo.m_dwProperty = dwProperty;
        vecChampions_.push_back(kInfo);
    }

    START_LOG(clog, L"이벤트몬스터 챔피언 추가 설정.")
        << BUILD_LOG(vecChampions_.size())
        << BUILD_LOG(vecEventMon_.size())
        << BUILD_LOG(dwProperty) << END_LOG;
}

void KUser::DumpEventMonsterInfo(IN const std::vector<KEventMonster>& vecEventMonInfo_)
{
    START_LOG(clog, L"========= EventMonster Dump =========")
        << BUILD_LOG(vecEventMonInfo_.size()) << END_LOG;
    START_LOG(cwarn, L"TriggerID | MonsterID | MonLv | Stage | StageNum | IsBoss | Special | ItemID, Count, Period") << END_LOG;
    std::vector<KEventMonster>::const_iterator cvit;
    for (cvit = vecEventMonInfo_.begin(); cvit != vecEventMonInfo_.end(); ++cvit) {
        START_LOG(clog, L"" << cvit->m_nTriggerID << L" | "
            << cvit->m_nMonsterID << L" | "
            << cvit->m_nMonLv << L" | "
            << cvit->m_nStage << L" | "
            << cvit->m_nStageNum << L" | "
            << cvit->m_bIsBoss << L" | "
            << cvit->m_nSpecial << L" | ") << END_LOG;

        std::vector<KDropItemInfo>::const_iterator cvitDrop;
        for (cvitDrop = cvit->m_vecReward.begin(); cvitDrop != cvit->m_vecReward.end(); ++cvitDrop) {
            START_LOG(clog, L"" << cvitDrop->m_ItemID << L", "
                << cvitDrop->m_nDuration << L", "
                << cvitDrop->m_nPeriod << L" | ") << END_LOG;
        }

    }
    START_LOG(cwarn, L"========= End Dump =========") << END_LOG;
}

void KUser::CheckLowPing()
{
    if (!m_kPingTimer.CheckTime())
        return;

    if (!SiKGSSimLayer()->IsPingEnable()) {
        return;
    }

    JIF(m_pkRoom);

    if (m_pkRoom->GetCount() <= 1) {
        return;
    }

    DWORD kPacket = 0L;
    //로딩 완료 패킷을 받으면 시간 초기화를 한번 하겠다.
    if (GetLoadingComplete()) { // 이부분은 클라 로딩이 완료된 이후 부터 체크 하도록 하겠음.
        kPacket = m_kPingInfo.GetPingGap();
        DWORD dwLimitGap = SiKGSSimLayer()->GetPingLimitGap();
        if (kPacket > dwLimitGap) {
            m_kPingInfo.UpdateLastTick();
        }
    }

    kPacket = m_kPingInfo.GetPing();
    DWORD dwLimit = SiKGSSimLayer()->GetPingLimit();
    if (kPacket > dwLimit) {
        SEND_PACKET(EVENT_LOW_PING_NOT);

        boost::array<DWORD, KClientPing::MAX_PING_COUNT> aPingInfo;
        m_kPingInfo.GetAvgPingInfo(aPingInfo);

        std::wstringstream stm;

        stm << KNC_TIME_STRING
            << L"|Low_Ping"
            << L"|" << m_strName
            << L"|" << dwLimit
            << L"|" << kPacket
            << L"|Avg Ping|";

        boost::array<DWORD, KClientPing::MAX_PING_COUNT>::iterator ait;
        for (ait = aPingInfo.begin(); ait != aPingInfo.end(); ++ait) {
            stm << *ait << L"|";
        } // for

        TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_LOW_PING, GetName(), GetUID(), 0, stm.str());

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    } // if
}

bool KUser::AddStatInitExpInfo(IN const char cChartype_, IN const __int64 biInitExp_, IN const __int64 biTotalInitExp_)
{
    bool bRet = m_mapInitExp.insert(std::map< char, __int64 >::value_type(cChartype_, biInitExp_)).second;
    bRet = m_mapTotalInitExp.insert(std::map< char, __int64 >::value_type(cChartype_, biTotalInitExp_)).second;
    _LOG_SUCCESS(bRet, L" 초기 통계 기록 Name : " << GetName()
        << L", Char : " << (int)(cChartype_)
        << L", InitExp : " << biInitExp_
        << L", InitExp : " << biTotalInitExp_) << END_LOG;
    return bRet;
}

bool KUser::IsEquipedLookItem(IN const GCITEMUID itemUID_)
{
    std::map<char, KLookEquipChar>::iterator mit;
    for (mit = m_mapLookEquip.begin(); mit != m_mapLookEquip.end(); ++mit) {
        if (mit->second.IsEquiped(itemUID_))
            return true;
    }
    return false;
}

KLookEquipChar* KUser::GetLookEquip(IN const char cCharType_)
{
    std::map<char, KLookEquipChar>::iterator mit;
    mit = m_mapLookEquip.find(cCharType_);
    if (mit == m_mapLookEquip.end())
        return NULL;
    return &(mit->second);
}

bool KUser::IsLookItem(IN const std::set<GCITEMUID>& setLookItems_)
{
    std::set<GCITEMUID>::const_iterator sit;
    for (sit = setLookItems_.begin(); sit != setLookItems_.end(); ++sit) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID(*sit);
        if (!spItem) {
            return false;
        }

        if (!spItem->CheckItemType(KItem::TYPE_LOOK)) {
            return false;
        }
    }

    return true;
}

void KUser::ReleaseCurrentLookEquip(IN const std::map<char, KCoordiDiffInfo>& mapChangeEquip_)
{
    std::map<char, KCoordiDiffInfo>::const_iterator mitReq;
    for (mitReq = mapChangeEquip_.begin(); mitReq != mapChangeEquip_.end(); ++mitReq) {
        std::map<char, KLookEquipChar>::iterator mitEquip;
        mitEquip = m_mapLookEquip.find(mitReq->first);
        if (mitEquip == m_mapLookEquip.end()) {
            // Log
            continue;
        }

        LIF(mitEquip->second.UnEquipItem(KLookEquipChar::LC_COORDI_0, mitReq->second.m_setRelease));
    }
}

void KUser::EquipCurrentLookEquip(IN const std::map<char, KCoordiDiffInfo>& mapChangeEquip_)
{
    std::map<char, KCoordiDiffInfo>::const_iterator mitReq;
    for (mitReq = mapChangeEquip_.begin(); mitReq != mapChangeEquip_.end(); ++mitReq) {
        std::map<char, KLookEquipChar>::iterator mitEquip;
        mitEquip = m_mapLookEquip.find(mitReq->first);
        if (mitEquip == m_mapLookEquip.end()) {
            // Log
            continue;
        }

        LIF(mitEquip->second.EquipItem(KLookEquipChar::LC_COORDI_0, mitReq->second.m_setEquip));
    }
}

void KUser::ExtractCurrentLookUID(OUT std::map<char, std::set<GCITEMUID> >& mapLookEquip_)
{
    mapLookEquip_.clear();
    std::map<char, KLookEquipChar>::iterator mit;
    for (mit = m_mapLookEquip.begin(); mit != m_mapLookEquip.end(); ++mit) {
        KCoordiInfo kCoordi;
        mit->second.GetCurrentCoordiInfo(kCoordi);
        mapLookEquip_.insert(std::map<char, std::set<GCITEMUID> >::value_type(mit->first, kCoordi.m_setEquip));
    }
}

void KUser::ExtractCurrentLookItem(OUT std::map<char, std::vector<KEquipItemInfo> >& mapLookEquip_)
{
    mapLookEquip_.clear();
    std::map<char, KLookEquipChar>::iterator mit;
    for (mit = m_mapLookEquip.begin(); mit != m_mapLookEquip.end(); ++mit) {
        if (mit->first != GetCurrentCharType()) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }

        KCoordiInfo kCoordi;
        mit->second.GetCurrentCoordiInfo(kCoordi);

        std::vector<KEquipItemInfo> vecLookItemID;
        ExtractItemFromItemUID(kCoordi.m_setEquip, vecLookItemID);
        mapLookEquip_.insert(std::map<char, std::vector<KEquipItemInfo> >::value_type(mit->first, vecLookItemID));

        KCharacterInfo* pkCharacterInfo = GetCharacterInfo(mit->first);
        if (pkCharacterInfo == NULL) {
            START_LOG(cerr, L" 캐릭터의 룩 장착 정보를 적용하는 시점에서 해당 캐릭터가 존재하지 않음. Name : " << m_strName
                << L", Char Type : " << (int)(mit->first)) << END_LOG;
            continue;
        }
        pkCharacterInfo->m_vecLookEquips = vecLookItemID;
    }
}

void KUser::ExtractCurrentLookItem(IN const char cCharType_, OUT std::vector<KEquipItemInfo>& vecLookEquip_)
{
    vecLookEquip_.clear();

    std::map<char, KLookEquipChar>::iterator mit;
    mit = m_mapLookEquip.find(cCharType_);
    if (mit == m_mapLookEquip.end()) {
        return;
    }

    KCoordiInfo kCoordi;
    mit->second.GetCurrentCoordiInfo(kCoordi);
    ExtractItemFromItemUID(kCoordi.m_setEquip, vecLookEquip_);
}

void KUser::ExtractItemFromItemUID(IN const std::set<GCITEMUID>& setItemUID_, OUT std::vector<KEquipItemInfo>& vecItemID_)
{
    vecItemID_.clear();

    std::set<GCITEMUID>::const_iterator sit;
    for (sit = setItemUID_.begin(); sit != setItemUID_.end(); ++sit) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID(*sit);
        if (spItem) {
            KEquipItemInfo kItem;
            kItem.m_dwID = spItem->m_ItemID;
            kItem.m_dwUID = spItem->m_ItemUID;
            kItem.m_cEnchantLevel = spItem->m_cEnchantLevel;
            kItem.m_cGradeID = spItem->m_cGradeID;
            kItem.m_nEquipLevel = static_cast<int>(spItem->m_sEquipLevel);
            kItem.m_cEnchantEquipGradeID = spItem->m_cEnchantEquipGradeID;
            kItem.m_vecSocket = spItem->m_vecSocket;
            kItem.m_vecAttribute = spItem->m_vecAttribute;
            kItem.m_DesignCoordiID = spItem->m_DesignCoordiID;

            vecItemID_.push_back(kItem);
        }
    }
}


void KUser::ReleaseLookEquip(IN const GCITEMUID iteUID_)
{
    std::map<char, KLookEquipChar>::iterator mit;
    for (mit = m_mapLookEquip.begin(); mit != m_mapLookEquip.end(); ++mit) {
        mit->second.ForceUnEquipItem(iteUID_);
    }
}

void KUser::ExtractDiffLookEquip(OUT std::map<char, std::map<int, KCoordiDiffInfo> >& mapDiffLookEquip)
{
    std::map<char, KLookEquipChar>::iterator mit;
    for (mit = m_mapLookEquip.begin(); mit != m_mapLookEquip.end(); ++mit) {

        std::map<int, KCoordiDiffInfo> mapDiffInfo;
        mit->second.GetDiffCoordiInfo(mapDiffInfo);
        LIF(mapDiffLookEquip.insert(std::map<char, std::map<int, KCoordiDiffInfo> >::value_type(mit->first, mapDiffInfo)).second);
    }
}

bool KUser::CheckBundleSellDuplicate(IN const KEVENT_BUNDLE_SELL_ITEM_REQ& kPacket_)
{
    std::set<GCITEMUID> setDuplCheck;

    std::vector<GCITEMUID>::const_iterator vit;
    for (vit = kPacket_.m_vecEraseList.begin(); vit != kPacket_.m_vecEraseList.end(); ++vit) {
        if (setDuplCheck.find(*vit) != setDuplCheck.end()) {
            return true;
        }
        setDuplCheck.insert(*vit);
    }

    for (vit = kPacket_.m_vecSellNormalItem.begin(); vit != kPacket_.m_vecSellNormalItem.end(); ++vit) {
        if (setDuplCheck.find(*vit) != setDuplCheck.end()) {
            return true;
        }
        setDuplCheck.insert(*vit);
    }

    std::vector<std::pair<GCITEMUID, int> >::const_iterator vit1;
    for (vit1 = kPacket_.m_vecSellCountItem.begin(); vit1 != kPacket_.m_vecSellCountItem.end(); ++vit1) {
        if (setDuplCheck.find(vit1->first) != setDuplCheck.end()) {
            return true;
        }
        setDuplCheck.insert(vit1->first);
    }

    return false;
}


void KUser::VerifyBundleSellCount(IN OUT std::vector<std::pair<GCITEMUID, int> >& vecSellCount_, IN OUT std::map<GCITEMUID, int>& mapFailInfo_)
{
    std::vector<std::pair<GCITEMUID, int> >::iterator vit;
    for (vit = vecSellCount_.begin(); vit != vecSellCount_.end(); ) {

        if (vit->second < 0) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first, NetError::ERR_BUNDLE_SELL_04));
            vit = vecSellCount_.erase(vit);
            continue;
        }

        KItemPtr spItem = m_kInventory.FindItemByItemUID(vit->first);
        if (!spItem) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first, NetError::ERR_BUNDLE_SELL_01));
            vit = vecSellCount_.erase(vit);
            continue;
        }

        if (spItem->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID()) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first, NetError::ERR_BUNDLE_SELL_02));
            vit = vecSellCount_.erase(vit);
            continue;
        }

        // 수량 아이템이 맞는지.
        if (spItem->m_nCount < 0 || spItem->m_nPeriod >= 0) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first, NetError::ERR_BUNDLE_SELL_03));
            vit = vecSellCount_.erase(vit);
            continue;
        }

        // 수량은 충분 한지.
        if (spItem->m_nCount < vit->second) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first, NetError::ERR_BUNDLE_SELL_05));
            vit = vecSellCount_.erase(vit);
            continue;
        }

        ++vit;
    }
}

void KUser::VerifyBundleSellNormal(IN OUT std::vector<GCITEMUID>& vecSellNormal_, IN OUT std::map<GCITEMUID, int>& mapFailInfo_)
{
    std::vector<GCITEMUID>::iterator vit;
    std::set<GCITEMUID> setSellNormalEquip;
    setSellNormalEquip.clear();
    std::set<GCITEMUID>::iterator sit;

    for (vit = vecSellNormal_.begin(); vit != vecSellNormal_.end(); ) {

        KItemPtr spItem = m_kInventory.FindItemByItemUID(*vit);
        if (!spItem) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_01));
            vit = vecSellNormal_.erase(vit);
            continue;
        }

        if (spItem->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID()) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_02));
            vit = vecSellNormal_.erase(vit);
            continue;
        }

        if (spItem->m_nCount != KItem::UNLIMITED_ITEM || spItem->m_nPeriod != KItem::UNLIMITED_ITEM) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_06));
            vit = vecSellNormal_.erase(vit);
            continue;
        }
        // 연계 아이템에 되한 처리.
        if (spItem->m_EnchantEquipItemUID > 0) {
            KItemPtr spItemEquip = m_kInventory.FindItemByItemUID(spItem->m_EnchantEquipItemUID);
            if (spItemEquip) {
                setSellNormalEquip.insert(spItemEquip->m_ItemUID);
            }
        }
        ++vit;
    }
    if (!setSellNormalEquip.empty()) { // 연계 아이템에 되한 삭제 리스트에 넣어주기.
        for (sit = setSellNormalEquip.begin(); sit != setSellNormalEquip.end(); ++sit) {
            vecSellNormal_.push_back(*sit);
        }
    }
}

void KUser::VerifyBundleSellErase(IN OUT std::vector<GCITEMUID>& vecEraseList_, IN OUT std::map<GCITEMUID, int>& mapFailInfo_)
{
    std::vector<GCITEMUID>::iterator vit;
    for (vit = vecEraseList_.begin(); vit != vecEraseList_.end(); ) {

        KItemPtr spItem = m_kInventory.FindItemByItemUID(*vit);
        if (!spItem) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_01));
            vit = vecEraseList_.erase(vit);
            continue;
        }

        if (spItem->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID()) {
            mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_02));
            vit = vecEraseList_.erase(vit);
            continue;
        }

        ++vit;
    }
}


int KUser::GetBundleSelItemErrNo(IN const int nNetErrCode_)
{
    int nOK = -99;
    switch (nNetErrCode_)
    {
        ERR_CASE(NET_OK, nOK, 0);
        ERR_CASE(ERR_BUNDLE_SELL_00, nOK, 1); // 판매 아이템이 중복이 있습니다. )
        ERR_CASE(ERR_BUNDLE_SELL_01, nOK, 2); // Item이 존재 하지 않음. )
        ERR_CASE(ERR_BUNDLE_SELL_02, nOK, 3); // PC방 아이템을 팔려고 했음. )
        ERR_CASE(ERR_BUNDLE_SELL_03, nOK, 4); // 수량 아이템이 아닌데 수량 판매 시도 )
        ERR_CASE(ERR_BUNDLE_SELL_04, nOK, 5); // 음수 수량을 판매 하려고 함. )
        ERR_CASE(ERR_BUNDLE_SELL_05, nOK, 6); // 보유 수량 보다 많은 수를 판매하려고 함. )
        ERR_CASE(ERR_BUNDLE_SELL_06, nOK, 7); // 영구 아이템이 아닌데 영구 아이템처럼 팔려고 함. )
        ERR_CASE(ERR_BUNDLE_SELL_07, nOK, 8); // 요청 패킷이 비어 있음. )
        ERR_CASE(ERR_BUNDLE_SELL_08, nOK, 9); // 처리 과정에서 전체가 오류 발생. )
        ERR_CASE(ERR_BUNDLE_SELL_09, nOK, 12); // 유저 캐싱 데이터 동기화 실패. )
        ERR_CASE(ERR_BUNDLE_SELL_10, nOK, 13); // 수량 아이템 DB 동기화 실패. )
        ERR_CASE(ERR_BUNDLE_SELL_11, nOK, 14); // 아이템 삭제 DB 처리 실패. )
        ERR_CASE(ERR_BUNDLE_SELL_12, nOK, 15); // 수량 아이템 판매 DB 처리 실패. )
        ERR_CASE(ERR_BUNDLE_SELL_13, nOK, 16); // 일반 아이템 판매 DB 처리 실패. )
        ERR_CASE(ERR_BUNDLE_SELL_14, nOK, 17); // 판매 하려는 아이템이 출석 포인트 아이템입니다. )

        ERR_CASE(ERR_NOT_FINISHED_YET, nOK, -98); // 이미 처리 중임..
    default:
        START_LOG(cerr, L"알수 없는 오류 Name : " << m_strName)
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
        nOK = -99;
    }

    return nOK;
}

void KUser::BundleSellExtractSellCountItems(IN const std::vector<std::pair<GCITEMUID, int> >& vecSellCountItems_, OUT std::vector<std::pair<KItem, int> >& vecExtractSellCountItems_)
{
    // 이미 앞에서 많은것을 검사 했기 때문에 여기서는 크게 검사하지 않는다.
    vecExtractSellCountItems_.clear();
    std::vector<std::pair<GCITEMUID, int> >::const_iterator vit;
    for (vit = vecSellCountItems_.begin(); vit != vecSellCountItems_.end(); ++vit) {
        KItemPtr spItem = m_kInventory.FindDurationItemByItemUID(vit->first);
        if (spItem) {
            vecExtractSellCountItems_.push_back(std::pair<KItem, int>(*spItem, vit->second));
        }
    }
}

void KUser::BundleSellExtractSellNormalItems(IN const std::vector<GCITEMUID>& vecSellNormalItem_, OUT std::vector<KItem>& vecExtractSellNormalItem_)
{
    // 이미 앞에서 많은것을 검사 했기 때문에 여기서는 크게 검사하지 않는다.
    vecExtractSellNormalItem_.clear();
    std::vector<GCITEMUID>::const_iterator vit;
    for (vit = vecSellNormalItem_.begin(); vit != vecSellNormalItem_.end(); ++vit) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID(*vit);
        if (spItem) {
            vecExtractSellNormalItem_.push_back(*spItem);
        }
    }
}

void KUser::RemoveItemFromInven(IN const std::vector<GCITEMUID>& vecItems_)
{
    std::vector<GCITEMUID>::const_iterator vit;
    for (vit = vecItems_.begin(); vit != vecItems_.end(); ++vit) {
        LIF(m_kInventory.RemoveItem(*vit));
    }
}

void KUser::AddStatisticsAvgPing()
{
    JIF(m_pkRoom);

    std::vector<DWORD> vecUserUIDs;
    m_pkRoom->GetUserUIDList(vecUserUIDs);

    std::vector<DWORD>::const_iterator vit;
    for (vit = vecUserUIDs.begin(); vit != vecUserUIDs.end(); ++vit) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*vit);
        if (!spUser) continue;

        KStatPingInfo kKey;
        kKey.m_nType = 1; // TCP Relay Server Ping.
        kKey.m_wstrSendCCode = L"ZZ";
        kKey.m_wstrRecvCCode = L"ZZ";
        kKey.m_nPingTime = spUser->m_kPingInfo.GetPing();
        AddStatisticsPing(kKey);
    }

}

void KUser::SendHeroDungeonList()
{
    // 전체 영웅던전 정보전달
    KDropItemInfo kPointItem;
    SiKHeroDungeonManager()->GetPointItem(kPointItem);

    KEVENT_HERO_DUNGEON_INFO_NOT kPacket;
    kPacket.m_PointItemID = kPointItem.m_ItemID;
    kPacket.m_tmResetTime = SiKHeroDungeonManager()->GetResetTime();
    SiKHeroDungeonManager()->GetHeroDungeonList(kPacket.m_vecDungeonInfo);

    SEND_COMPRESS_PACKET(EVENT_HERO_DUNGEON_INFO_NOT, kPacket);
}

bool KUser::HeroDungeonCheck(IN const int& nModeID_, IN const int& nDifficulty_)
{

    // 영웅던전이 아니면 통과
    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return true;
    }

    if (false == SiKHeroDungeonManager()->IsActiveMode()) {
        START_LOG(cwarn, L"영웅던전 모드가 비활성화 상태입니다. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_61);
        return false;
    }

    KHeroDungeon kDungeonInfo;
    if ( SiKHeroDungeonManager()->IsHeroDungeon(nModeID_) )
    {
        if ( false == SiKHeroDungeonManager()->GetHeroDungeonInfo( nModeID_, kDungeonInfo ) ) {
            START_LOG( cwarn, L"영웅던전 정보가 없음. ModeID : " << nModeID_ ) << END_LOG;
            SET_ERROR( ERR_ROOM_62 );
            return false;
        }
    }

    if (false == HeroDungeonCharacterCheck(kDungeonInfo.m_dwMinCharacterLv)) {
        START_LOG(cwarn, L"최소 레벨조건을 만족하는 캐릭터가 없음. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_63);
        return false;
    }

    // 난이도 체크하는 영웅던전
    if (kDungeonInfo.m_bDifficultyCheck) {
        KDungeonUserInfo kInfo;
        if (false == GetDungeonUserInfo(nModeID_, kInfo)) {
            START_LOG(cwarn, L"mit == m_mapDifficulty.end()    ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
            SET_ERROR(ERR_ROOM_23);
            return false;
        }

        if (nDifficulty_ > 0) {
            // 해당난이도나 이전 난이도 기록이 있으면 된다
            if (false == (kInfo.m_kModeDifficulty.Get(nDifficulty_ - 1) || kInfo.m_kModeDifficulty.Get(nDifficulty_))) {
                SET_ERROR(ERR_ROOM_23);
                return false;
            }

            // 플레이가능한 최대난이도 체크
            if (nDifficulty_ > static_cast<int>(kInfo.m_kClearData.m_shMaxDifficulty)) {
                START_LOG(cwarn, L"플레이 가능한 최대난이도 보다 높은 영던 난이도를 플레이하려고 함. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
                SET_ERROR(ERR_ROOM_23);
                return false;
            }
        }
    }

    USHORT usTimeIndex = 0;
    if (false == HeroDungeonEnterTimeCheck(kDungeonInfo.m_mapOpenTime, usTimeIndex)) {
        START_LOG(cwarn, L"입장 시간이 아닙니다. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_64);
        return false;
    }

    // 내부에서 에러 설정.
    if (false == HeroDungeonPlayLimitCheck(nModeID_, usTimeIndex, kDungeonInfo.m_usMaxClearPerDay)) {
        START_LOG(cwarn, L"플레이 제한 상태입니다. ModeID : " << nModeID_) << END_LOG;
        return false;
    }

    return true;
}

bool KUser::HeroDungeonLevelCheck(IN const int nModeID_, IN const int nCharIndex_)
{
    // 영웅던전이 아니면 통과
    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return true;
    }

    KHeroDungeon kDungeonInfo;
    _JIF(SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID_, kDungeonInfo), return false);

    std::map<char, KCharacterInfo>::const_iterator cmit;
    cmit = m_mapCharacterInfo.find(nCharIndex_);
    if (cmit == m_mapCharacterInfo.end()) {
        START_LOG(cwarn, L"보유하지 않은 캐릭터로 교체하려고함. Character Index : " << nCharIndex_) << END_LOG;
        return false;
    }

    //std::Info << L"Login:" << GetName() << L",ModeID:" << nModeID_ << L",CharIndex:" << nCharIndex_
    //    << L",MinLv:" << kDungeonInfo.m_dwMinCharacterLv << L",CharLv:" << cmit->second.m_dwLevel << dbg::endl;

    if (cmit->second.m_dwLevel < kDungeonInfo.m_dwMinCharacterLv) {
        START_LOG(cwarn, L"최소 레벨조건을 만족하지 못함. Character Index : " << nCharIndex_)
            << BUILD_LOG(nModeID_)
            << BUILD_LOG(cmit->second.m_dwLevel)
            << BUILD_LOG(kDungeonInfo.m_dwMinCharacterLv) << END_LOG;
        return false;
    }

    return true;
}

bool KUser::UpdateHeroDungeonPlay(IN const int nModeID_, IN const bool bClear_)
{
    // 영웅던전이 아니면 통과
    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return true;
    }

    // 영웅던전 모드가 비활성화 상태면 기록하지 말고 통과
    if (false == SiKHeroDungeonManager()->IsActiveMode()) {
        return true;
    }

    // 영웅던전 정보가 없음
    KHeroDungeon kDungeonInfo;
    _JIF(SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID_, kDungeonInfo), return false);

    CTime tmToday(CTime::GetCurrentTime());
    time_t tmCurrent = KncUtil::TimeToInt(tmToday);
    USHORT usTimeIndex = 0;
    bool bPlayYesterday = true;
    int nYear = tmToday.GetYear();
    int nMonth = tmToday.GetMonth();
    int nDay = tmToday.GetDay();

    std::map<USHORT, std::pair<time_t, time_t> >::iterator mitTime;
    for (mitTime = kDungeonInfo.m_mapOpenTime.begin(); mitTime != kDungeonInfo.m_mapOpenTime.end(); ++mitTime) {
        time_t tmOpen = KncUtil::TimeToInt(CTime(nYear, nMonth, nDay, CTime(mitTime->second.first).GetHour(), CTime(mitTime->second.first).GetMinute(), 0));
        time_t tmClose = KncUtil::TimeToInt(CTime(nYear, nMonth, nDay, CTime(mitTime->second.second).GetHour(), CTime(mitTime->second.second).GetMinute(), 59));

        if (tmOpen <= tmCurrent && tmClose >= tmCurrent) {
            usTimeIndex = mitTime->first;
            bPlayYesterday = false;
            break;
        }

        if (tmOpen > tmCurrent) {
            break;
        }

        usTimeIndex = mitTime->first;
        bPlayYesterday = false;
    }

    if (bPlayYesterday) {
        START_LOG(clog, L"어제 플레이해서 오늘 첫 플레이시간대 전에 끝난 유저다.. Login : " << GetName())
            << BUILD_LOG(nModeID_)
            << BUILD_LOGtm(tmToday) << END_LOG;
        return true;
    }

    std::map< int, KHeroPlayData >::iterator mitPlay;
    mitPlay = m_mapHeroDungeonClearInfo.find(nModeID_);
    if (mitPlay != m_mapHeroDungeonClearInfo.end()) {
        CTime tmPlayDate(mitPlay->second.m_tmPlayDate);
        if (tmPlayDate.GetYear() < tmToday.GetYear() ||
            (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() < tmToday.GetMonth()) ||
            (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() <= tmToday.GetMonth() && tmPlayDate.GetDay() < tmToday.GetDay())) {
            mitPlay->second.m_ClearCount = 0;
            START_LOG(clog, L"영던 클리어데이터가 이전 날짜라서 클리어 카운트 초기화. ModeID : " << nModeID_) << END_LOG;
        }

        mitPlay->second.m_nPlayIndex = usTimeIndex;
        mitPlay->second.m_ClearCount += (bClear_ ? 1 : 0);
        mitPlay->second.m_tmPlayDate = tmCurrent;
    }
    else {
        KHeroPlayData kPlayData;
        kPlayData.m_nPlayIndex = usTimeIndex;
        kPlayData.m_ClearCount = (bClear_ ? 1 : 0);
        kPlayData.m_tmPlayDate = tmCurrent;
        m_mapHeroDungeonClearInfo[nModeID_] = kPlayData;
    }

    //std::clog << L"[UpdateHeroDungeonPlay] " << GetName() << L" / " << usTimeIndex << L" / " << bClear_ << L" / " << nModeID_
    //    << L" / " << m_mapHeroDungeonClearInfo.size() << L" / " << m_mapHeroDungeonClearInfo[nModeID_].m_ClearCount << dbg::endl;

    // DB 업데이트 요청
    KDB_EVENT_USER_HERO_DUNGEON_INFO_UPDATE_NOT kDBPacket;
    kDBPacket.m_nModeID = nModeID_;
    kDBPacket.m_nPlayIndex = usTimeIndex;
    kDBPacket.m_nClearCount = m_mapHeroDungeonClearInfo[nModeID_].m_ClearCount;
    QUEUEING_EVENT_TO_DB(DB_EVENT_USER_HERO_DUNGEON_INFO_UPDATE_NOT, kDBPacket);
    return true;
}

void KUser::UpdateHeroDungeonPlay(IN const int nModeID_, IN KUserPtr& spUser_, IN const KGameResultIn& kGameResultIn_)
{
    // 영웅던전이 아니면 통과
    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return;
    }

    // 영웅던전 모드가 비활성화 상태면 기록하지 말고 통과
    if (false == SiKHeroDungeonManager()->IsActiveMode()) {
        return;
    }

    LIF(spUser_->UpdateHeroDungeonPlay(nModeID_, kGameResultIn_.m_bWin));
}

bool KUser::HeroDungeonCharacterCheck(IN const DWORD& dwMinCharacterLv_)
{
    // 영웅던전 캐릭터 레벨제한 체크
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for (cmit = m_mapCharacterInfo.begin(); cmit != m_mapCharacterInfo.end(); ++cmit) {
        if (cmit->second.m_dwLevel >= dwMinCharacterLv_) {
            break;
        }
    }

    if (cmit == m_mapCharacterInfo.end()) {
        return false;
    }

    return true;
}

bool KUser::HeroDungeonEnterTimeCheck(IN const std::map<USHORT, std::pair<time_t, time_t> >& mapTimeTable_, OUT USHORT& usTimeIndex_)
{
    return true;
}

bool KUser::GetHeroPlayData(IN const int& nModeID_, OUT KHeroPlayData& kData_)
{
    return false;
}

bool KUser::HeroDungeonPlayLimitCheck(IN const int& nModeID_, IN const USHORT& usTimeIndex_, IN const USHORT& usMaxClearPerDay_)
{
    KHeroPlayData kPlayData;
    if (false == GetHeroPlayData(nModeID_, kPlayData)) {
        // 해당 던전 플레이 정보가 없으면 통과
        return true;
    }

    CTime tmPlayDate(kPlayData.m_tmPlayDate);
    CTime tmToday(CTime::GetCurrentTime());

    // 시간 데이터가 이전꺼면 제거하고 통과
    if (tmPlayDate.GetYear() < tmToday.GetYear() ||
        (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() < tmToday.GetMonth()) ||
        (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() <= tmToday.GetMonth() && tmPlayDate.GetDay() < tmToday.GetDay())) {
        m_mapHeroDungeonClearInfo.erase(nModeID_);
        return true;
    }

    if (kPlayData.m_ClearCount >= usMaxClearPerDay_) {
        START_LOG(cwarn, L"일일 클리어할 수 있는 최대 회수를 플레이했음. ModeID : " << nModeID_)
            << BUILD_LOG(kPlayData.m_ClearCount)
            << BUILD_LOG(usMaxClearPerDay_) << END_LOG;
        SET_ERROR(ERR_ROOM_66);
        return false;
    }

    return true;
}

bool KUser::HeroDungeonStartCheck(IN const int nModeID_)
{
    if (IsAdmin()) {
        return true;
    }

    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return true;
    }

    if (false == SiKHeroDungeonManager()->IsActiveMode()) {
        START_LOG(cwarn, L"영웅던전 모드가 비활성화 상태입니다. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_61);
        return false;
    }

    KHeroDungeon kDungeonInfo;
    if (false == SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID_, kDungeonInfo)) {
        START_LOG(cwarn, L"영웅던전 정보가 없음. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_62);
        return false;
    }

    if (false == HeroDungeonCharacterCheck(kDungeonInfo.m_dwMinCharacterLv)) {
        START_LOG(cwarn, L"최소 레벨조건을 만족하는 캐릭터가 없음. ModeID : " << nModeID_ << L",CharacterLv : " << kDungeonInfo.m_dwMinCharacterLv) << END_LOG;
        SET_ERROR(ERR_ROOM_63);
        return false;
    }

    const KCharacterInfo* pkChar = GetCurrentCharacter();
    if (pkChar == NULL) {
        SET_ERROR(ERR_ROOM_35);
        return false;
    }

    if (pkChar->m_dwLevel < kDungeonInfo.m_dwMinCharacterLv) {
        START_LOG(cwarn, L"최소 레벨제한에 만족하는 캐릭터가 아님. MinLv : " << kDungeonInfo.m_dwMinCharacterLv)
            << BUILD_LOG(pkChar->m_dwLevel)
            << BUILD_LOGc(pkChar->m_cCurrentPromotion)
            << BUILD_LOGc(pkChar->m_cPromotion) << END_LOG;
        SET_ERROR(ERR_ROOM_63);
        return false;
    }

    USHORT usTimeIndex = 0;
    if (false == GetHeroDungeonTimeIndex(kDungeonInfo.m_mapOpenTime, usTimeIndex)) {
        START_LOG(cwarn, L"입장 시간이 없습니다. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_64);
        return false;
    }

    // 내부에서 에러 설정.
    if (false == HeroDungeonPlayLimitCheck(nModeID_, usTimeIndex, kDungeonInfo.m_usMaxClearPerDay)) {
        START_LOG(cwarn, L"플레이 제한 상태입니다. ModeID : " << nModeID_ << L",TimeIndex : " << usTimeIndex) << END_LOG;
        return false;
    }

    START_LOG(clog, L"Hero Dungeon Start Check.. ModeID : " << nModeID_ << L",CharLv : " << kDungeonInfo.m_dwMinCharacterLv << L",TimeIdx : " << usTimeIndex) << END_LOG;
    return true;
}

bool KUser::GetHeroDungeonTimeIndex(IN const std::map<USHORT, std::pair<time_t, time_t> >& mapTimeTable_, OUT USHORT& usTimeIndex_)
{
    return true;
}

bool KUser::SetHeroDungeonCharIndex(IN const int& nModeID_, IN OUT KInDoorUserInfo& kUserInfo_)
{
    if (false == SiKHeroDungeonManager()->IsHeroDungeon(nModeID_)) {
        return true;
    }

    KHeroDungeon kDungeonInfo;
    if (false == SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID_, kDungeonInfo)) {
        START_LOG(cwarn, L"영웅던전 정보가 없음. ModeID : " << nModeID_) << END_LOG;
        SET_ERROR(ERR_ROOM_62);
        return false;
    }

    return SetHeroDungeonCharIndex(kDungeonInfo.m_dwMinCharacterLv, kUserInfo_.m_cCharIndex, kUserInfo_.m_cPromotion);
}

bool KUser::SetHeroDungeonCharIndex(IN const DWORD& dwMinCharLv_, IN OUT char& cCharIndex_, IN OUT char& cPromotion_)
{
    std::map<char, KCharacterInfo>::const_iterator cmit;
    cmit = m_mapCharacterInfo.find(cCharIndex_);
    if (cmit == m_mapCharacterInfo.end()) {
        START_LOG(cwarn, L"존재하지 않는 캐릭터를 선택. CharIndex : " << static_cast<int>(cCharIndex_)) << END_LOG;
        SET_ERROR(ERR_ROOM_35);
        return false;
    }

    if (cmit->second.m_dwLevel >= dwMinCharLv_) {
        return true;
    }

    SET_ERROR(ERR_ROOM_63);

    return false;
}

USHORT KUser::GetUserChannelCode()
{
    std::map<USHORT, USHORT>::const_iterator cmit;
    cmit = m_mapChannellingTypeCode.find((USHORT)GetUserChannelType());
    _JIF(cmit != m_mapChannellingTypeCode.end(), return 0);
    return cmit->second;
}

void KUser::SendExpUpdateToDBNotice()
{
    KEVENT_EXP_UPDATE_TO_DB_NOT mapExpData;
    GetDBUpdateExpData(mapExpData);
    QUEUEING_EVENT_TO_DB(EVENT_EXP_UPDATE_TO_DB_NOT, mapExpData);
}

void KUser::CheckServerListByAuthLevel(IN OUT std::vector< KServerInfo >& vecList_)
{
    int nCheckAuthLevel = SiKGSSimLayer()->GetServerListAuthLevel();
    bool IsPassAuthLevel = false;
    int nEnableServerPart = 1; // 일반계정 접근 가능한 서버part

    // 허용계정등급인가?
    if (GetAuthLevel() >= nCheckAuthLevel) {
        IsPassAuthLevel = true;
    }

    // 허용계정등급이 접근가능한 서버Part 설정
    if (IsPassAuthLevel) {
        nEnableServerPart = -1;
    }

    // 허용계정등급이고, 운영자면 모두 전달
    if (IsPassAuthLevel && IsAdmin()) {
        return;
    }

    // 운영자 계정이 아니면 ServerPart 0 이하의 서버는 보이지 않음.
    std::vector< KServerInfo > vecEnableList;
    vecEnableList.reserve(vecList_.size());

    std::vector< KServerInfo >::iterator vit;
    for (vit = vecList_.begin(); vit != vecList_.end(); ++vit) {
        if (vit->m_iServerPart >= nEnableServerPart) {
            vecEnableList.push_back(*vit);
        }
    }

    vecList_.swap(vecEnableList);
}

void KUser::UpdateGawibawiboPoint()
{
    if (SiKGawibawibo()->IsRun() == false) {
        return;
    }
    // 하루에 받을 수 있는 포인트가 제한되어 있다
    if (SiKGawibawibo()->GetLimitPointPerDay() <= m_kGawibawiboInfo.m_usReceiveCount) {
        START_LOG(clog, L"오늘 받을 수 있는 가위바위보 포인트 초과 : " << m_strName)
            << BUILD_LOG(m_kGawibawiboInfo.m_usReceiveCount) << END_LOG;
        return;
    }
    ++m_kGawibawiboInfo.m_dwRemainTime;

    // 접속시간 (분) 이  포인트 누적시간 단위를 초과하면  포인트 더하기, 저장시간 리셋
    if (m_kGawibawiboInfo.m_dwRemainTime > 0 && m_kGawibawiboInfo.m_dwRemainTime % SiKGawibawibo()->GetPointTimeGap() == 0) {
        ++m_kGawibawiboInfo.m_dwTryPoint;
        ++m_kGawibawiboInfo.m_usReceiveCount;
        m_kGawibawiboInfo.m_dwRemainTime = 0;

        SEND_ID(EVENT_GAWIBAWIBO_TRY_POINT_NOT);
    }

    START_LOG(clog, L"가위바위보 시간 누적 : " << m_strName)
        << BUILD_LOG(m_kGawibawiboInfo.m_dwRemainTime)
        << BUILD_LOG(m_kGawibawiboInfo.m_dwTryPoint)
        << BUILD_LOG(m_kGawibawiboInfo.m_usReceiveCount) << END_LOG;
}

void KUser::InitGawibawiboData(int nDayChange_)
{
    if (nDayChange_ == KCalendar::UR_RELOAD || nDayChange_ == KCalendar::UR_NEXTDAY) {
        START_LOG(clog, L"날짜가 바뀌어 가위바위보 데이터 초기화 전 받은 포인트:" << m_kGawibawiboInfo.m_usReceiveCount) << END_LOG;
        // 오늘 받은 포인트 초기화 (지급 제한에 사용되는 값)
        m_kGawibawiboInfo.m_usReceiveCount = 0;

        QUEUEING_EVENT_TO_DB(DB_EVENT_GAWIBAWIBO_INIT_INFO_REQ, m_kGawibawiboInfo);
    }
    else {
        RetryInit();
    }
}

void KUser::RetryInit()
{
    CTime tmCurrent = CTime::GetCurrentTime();
    // 0시 0분 ~ 0시 1분 사이에 서버이동하면서  리시브카운트가 있는 경우 , 초기화 재진행.
    if (tmCurrent.GetHour() == 0 && tmCurrent.GetMinute() < 2 && m_kGawibawiboInfo.m_usReceiveCount > 0) {

        m_kGawibawiboInfo.m_usReceiveCount = 0;

        QUEUEING_EVENT_TO_DB(DB_EVENT_GAWIBAWIBO_INIT_INFO_REQ, m_kGawibawiboInfo);

        START_LOG(cerr, L"서버 이동&초기화 타이밍 문제로 재초기화 시도" << GetName())
            << BUILD_LOG(m_kGawibawiboInfo.m_usReceiveCount) << END_LOG;
    }
}

bool KUser::CheckExpCalcErr(IN const USHORT& usCase_, IN const int& nModeID_, IN const DWORD& dwMissionID_, IN const char& cCharType_, IN const int& nTotalRecvExp_, IN OUT __int64& biExp_)
{
    __int64 biInitExp = m_mapInitExp[cCharType_];
    __int64 biCalcExp = biExp_ + nTotalRecvExp_;
    __int64 biLimitExp = _I64_MAX;

    if (biInitExp <= biExp_ &&       // DB에서 받아온 초기 경험치보다 큰가?
        0 <= nTotalRecvExp_ &&       // 획득 경험치가 0보다 큰가?
        0 <= biCalcExp &&            // 계산된 경험치가 0보다 큰가?
        biLimitExp - biExp_ > nTotalRecvExp_) {   // 계산된 경험치가 경험치 제한치보다 작은가?
        return true;
    }

    if (SiKLevelResetEvent()->IsRun()) {
        return true;
    }

    if (SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP)) {
        return true;
    }

    // 에러 기록
    std::wstringstream stm;
    stm << KNC_TIME_STRING
        << L" | " << usCase_
        << L" | " << nModeID_
        << L" | " << dwMissionID_
        << L" | " << GetName()
        << L" | " << static_cast<int>(cCharType_)
        << L" | " << biInitExp
        << L" | " << nTotalRecvExp_
        << L" | " << biExp_
        << L" | " << static_cast<int>(SiKResultManager()->Exp_2_Level(biInitExp))
        << L" | " << static_cast<int>(SiKResultManager()->Exp_2_Level(biExp_));

    TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_EXP_CALC_ERR, GetName(), GetUID(), 0, stm.str());

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    START_LOG(cerr, L"DB에서 받아온 경험치로 클리핑. LoginID : " << GetName())
        << BUILD_LOG(usCase_)
        << BUILD_LOG(nModeID_)
        << BUILD_LOG(dwMissionID_)
        << BUILD_LOGc(cCharType_)
        << BUILD_LOG(biInitExp)
        << BUILD_LOG(nTotalRecvExp_)
        << BUILD_LOG(biExp_) << END_LOG;

    biExp_ = biInitExp;
    return false;
}

KItemPtr KUser::GetItemExEquipByItemID(IN const GCITEMID ItemID_)
{
    std::vector<GCITEMUID> vecItemUIDs;
    std::vector<GCITEMUID>::iterator vitUID;

    m_kInventory.FindItemUIDsByItemID(ItemID_, vecItemUIDs);
    if (vecItemUIDs.empty()) {
        return KItemPtr();
    }

    for (vitUID = vecItemUIDs.begin(); vitUID != vecItemUIDs.end(); ++vitUID) {
        // 장착중인 아이템 제외.
        if (IsEquipedItem(*vitUID)) {
            continue;
        }

        if (NULL == m_kInventory.FindItemByItemUID(*vitUID)) {
            continue;
        }

        return m_kInventory.FindItemByItemUID(*vitUID);
    }

    return KItemPtr();
}

// #UpdateSocksTime
void KUser::UpdateSocksTime()
{
    if (false == SiKSocks()->IsRun()) {
        return;
    }
    if (!m_kSocksTimer.CheckTime()) {
        return;
    }

    std::vector< GCITEMID > vecCompleteSocks;
    vecCompleteSocks.clear();

    std::map< DWORD, bool > mapTimeChange;
    SiKSocks()->CountdownRemainTime(m_mapSocksInfo, mapTimeChange);
    SiKSocks()->CheckSocksDone(m_mapSocksInfo, vecCompleteSocks, mapTimeChange);

    if (!vecCompleteSocks.empty()) { // 걸었던 양말 중 완료 리스트 존재하면 디비에 저장하기
        std::map< GCITEMID, KSocksInfo > mapSocksInfoDB; // 내릴 양말만 디비로 보내기(sp호출최소화)
        mapSocksInfoDB.clear();
        std::vector< GCITEMID >::const_iterator vit;

        for (vit = vecCompleteSocks.begin(); vit != vecCompleteSocks.end(); ++vit) {
            std::map< GCITEMID, KSocksInfo >::const_iterator mit;
            mit = m_mapSocksInfo.find(*vit);

            if (mit != m_mapSocksInfo.end()) {
                mapSocksInfoDB[mit->first] = mit->second;
            }
        }
        //QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SOCKS_INFO_UPDATE, mapSocksInfoDB );
        KSocksResult kPacket;
        kPacket.m_nOK = 0;
        kPacket.m_kSocksInfo = m_mapSocksInfo;

        SEND_PACKET(EVENT_SOCKS_HANGUP_COMPLETE_NOT);
    }
}

void KUser::ExchangeSocks(const std::vector<KGameResultOut>& m_vecGameResult_)
{
    // map< UserUID, vector < 양말아이템 > >
    KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT mapInsertItem;

    std::vector<KGameResultOut>::const_iterator vit;
    for (vit = m_vecGameResult_.begin(); vit != m_vecGameResult_.end(); ++vit) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(vit->m_strLogin);
        if (!spUser) {
            START_LOG(cwarn, L"양말 교환 중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vit->m_strLogin) << END_LOG;
            continue;
        }
        //  양말 아이디,  pair< 재료 아이디, 수량 > 
        std::map< GCITEMID, std::pair< GCITEMID, int > > mapMaterial;
        SiKSocks()->GetSocksMaterialList(mapMaterial);

        KSocksExchangeData kSocksExchangeData;

        std::map< GCITEMID, std::pair< GCITEMID, int > >::iterator mit;
        for (mit = mapMaterial.begin(); mit != mapMaterial.end(); ++mit) {
            KItemPtr spItem = spUser->m_kInventory.FindItemByItemID(mit->second.first);
            if (!spItem) {
                continue;
            }

            int MaterialCount = std::max<int>(mit->second.second, 1); // 나누기 0 방지
            int MultipleCount = std::max<int>((spItem->m_nCount / MaterialCount), 1); // 곱하기 0 방지
            int TotalUseCount = MaterialCount * MultipleCount;

            if (TotalUseCount <= spItem->m_nCount) {
                spItem->m_nCount -= TotalUseCount; // 재료 아이템 소모

                KSocksInfo kSocksInfo;
                SiKSocks()->GetSocksInfo(mit->first, kSocksInfo);

                kSocksInfo.m_Socks.m_nDuration *= MultipleCount;

                kSocksExchangeData.m_vecExchangeSocks.push_back(kSocksInfo.m_Socks);
                kSocksExchangeData.m_vecUseItem.push_back(*spItem);
            }
        } // end of for ( spUser ...

        if (false == kSocksExchangeData.m_vecExchangeSocks.empty()) {
            mapInsertItem.insert(std::make_pair(spUser->GetUID(), kSocksExchangeData));
        }

    }
    // 아이템을 디비에 넣기 위한 요청
    if (false == mapInsertItem.empty()) {
        QUEUEING_EVENT_TO_DB(DB_EVENT_SOCKS_INSERT_ITEM, mapInsertItem);
    }
}

void KUser::SetInitGawibawiboPosition()
{
    m_kGawibawiboInfo.m_dwCurrentPosition = 0; // 말 위치 초기화
}

void KUser::SetRetryPosition()
{
    m_kGawibawiboInfo.m_dwCurrentPosition += SiKGawibawibo()->MarkRetryPosition();
}

void KUser::DeleteRetryPosition()
{
    if (m_kGawibawiboInfo.m_dwCurrentPosition >= SiKGawibawibo()->MarkRetryPosition()) {
        m_kGawibawiboInfo.m_dwCurrentPosition -= SiKGawibawibo()->MarkRetryPosition();
    }
}

void KUser::AddGawibawiboTurnCount()
{
    ++m_kGawibawiboInfo.m_dwTurnCount;
}

void KUser::MoveGawibawiboCurrentPosition()
{
    ++m_kGawibawiboInfo.m_dwCurrentPosition;
}

void KUser::AddGawibawiboPlayCount()
{
    ++m_kGawibawiboInfo.m_dwPlayCount; // 플레이횟수 증가
}

void KUser::AddGawibawiboRetryPoint()
{
    if (m_kGawibawiboInfo.m_dwPlayCount > 0 && m_kGawibawiboInfo.m_dwPlayCount % SiKGawibawibo()->GetRetryPointPerPlays() == 0) {
        ++m_kGawibawiboInfo.m_dwRetryPoint;
    }
}

void KUser::CalcMaterialsByFactor(IN const USHORT& usFactor_, IN OUT std::vector<KDropItemInfo>& vecMaterials_)
{
    std::vector<KDropItemInfo> vecTemp;
    std::vector<KDropItemInfo>::iterator vit;
    for (vit = vecMaterials_.begin(); vit != vecMaterials_.end(); ++vit) {
        if (vit->m_nDuration == KItem::UNLIMITED_ITEM) {
            for (int i = 0; i < usFactor_; ++i) {
                vecTemp.push_back(*vit);
            }
        }
        else {
            vit->m_nDuration *= usFactor_;
            vecTemp.push_back(*vit);
        }
    }
    vecMaterials_.swap(vecTemp);
}

void KUser::SetEndGameResultGPInfo(IN const std::vector<KGameResultIn>& vecGameResult_)
{
    std::map<DWORD, int> mapResultGP;
    std::vector<KGameResultIn>::const_iterator cvit;
    for (cvit = vecGameResult_.begin(); cvit != vecGameResult_.end(); ++cvit) {
        mapResultGP[cvit->m_dwUID] = std::max<int>(0, cvit->m_nBaseGP);
    }

    JIF(m_pkRoom);
    m_pkRoom->SetResultGP(mapResultGP);

    if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
        START_LOG(cwarn, L"--- Dump Set User Result GP (size:" << mapResultGP.size() << L")---") << END_LOG;
        std::map<DWORD, int>::const_iterator cmit;
        for (cmit = mapResultGP.begin(); cmit != mapResultGP.end(); ++cmit) {
            START_LOG(cwarn, L"UserUID : " << cmit->first << L",ResultGP : " << cmit->second) << END_LOG;
        }
        START_LOG(cwarn, L"--- End Dump ---") << END_LOG;
    }
}

void KUser::CalcRewardType(IN const int& nModeID_, OUT int& nRewardType_, IN OUT std::vector<KGameResultOut>& vecGameResult_)
{
    // 던전 모드만.
    if (false == KRoom::IsDungeonMode(nModeID_)) {
        return;
    }

    if (false == SiKDungeonManager()->GetDungeonRewardType(nModeID_, nRewardType_)) {
        return;
    }

    std::vector<KGameResultOut>::iterator vit;
    for (vit = vecGameResult_.begin(); vit != vecGameResult_.end(); ++vit) {
        // 시즌4.5 스코어에 따른 보상상자 키 개수 차등지급.
        //if ( vit->m_nScore > 0 )
        vit->m_usSpecialRewardKey = 1;
    }
}

void KUser::GetCurruntCharType(IN const std::map<DWORD, std::vector<std::pair<char, char> > >& mapCharType_, IN OUT std::vector<KRewardInfo>& vecReward_)
{
    std::vector<KRewardInfo>::iterator vit;
    for (vit = vecReward_.begin(); vit != vecReward_.end(); ++vit) {

        std::map<DWORD, std::vector<std::pair<char, char> > >::const_iterator cmit;
        cmit = mapCharType_.find(vit->m_dwUID);
        if (cmit == mapCharType_.end()) {
            vit->m_cCharType = 0;
            continue;
        }

        if (cmit->second.empty()) {
            vit->m_cCharType = 0;
            continue;
        }

        vit->m_cCharType = cmit->second.begin()->first;
    }
}

bool KUser::IsSpecialDropCharacterBox(IN const int& nDifficulty_)
{
    int nBoxID = SiKSpecialReward()->GetDifficultBoxID(nDifficulty_);
    __JIF(nBoxID != -1, return false);

    std::vector<USHORT> vecBoxType;
    __JIF(SiKSpecialReward()->GetBoxInfo(nBoxID, vecBoxType), return false);

    std::vector<USHORT>::const_iterator cvit;
    cvit = std::find(vecBoxType.begin(), vecBoxType.end(), KRewardInfo::RT_CHARACTER);
    return (cvit != vecBoxType.end());
}

bool KUser::CheckCharacterType(IN const DWORD& dwUID_, IN const std::map<DWORD, std::vector<std::pair<char, char> > >& mapCharJob_, IN const char& cCharType_)
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(dwUID_);
    if (!spUser) {
        START_LOG(cwarn, L"대상 유저가 접속중이 아니다.") << END_LOG;
        return false;
    }

    std::map<DWORD, std::vector<std::pair<char, char> > >::const_iterator cmitChar;
    cmitChar = mapCharJob_.find(dwUID_);
    if (cmitChar == mapCharJob_.end()) {
        return false;
    }

    if (cmitChar->second.empty()) {
        return false;
    }

    std::vector<std::pair<char, char> >::const_iterator cvit;
    cvit = std::find_if(cmitChar->second.begin(), cmitChar->second.end(),
        boost::bind(&std::pair<char, char>::first, _1) == cCharType_);
    if (cvit == cmitChar->second.end()) {
        return false;
    }

    return true;
}

void KUser::GetCharExpReward(IN const std::vector<DWORD>& vecUID_, IN const std::map<DWORD, std::vector<std::pair<char, char> > >& mapRoomChar_, OUT std::map< DWORD, KCharExpReward >& mapInfo_)
{
    mapInfo_.clear();

    std::vector<DWORD>::const_iterator cvit;
    for (cvit = vecUID_.begin(); cvit != vecUID_.end(); ++cvit) {

        KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*cvit));
        if (!spUser) continue;

        std::map<DWORD, std::vector<std::pair<char, char> > >::const_iterator cmit;
        cmit = mapRoomChar_.find(*cvit);
        if (cmit == mapRoomChar_.end()) continue;
        if (cmit->second.empty()) continue;

        char cCharType = cmit->second.begin()->first; // 유저의 캐릭터 타입
        KCharacterInfo* pkCharInfo = spUser->GetCharacterInfo(cCharType);
        if (!pkCharInfo) continue;

        KCharExpReward kExpInfo;
        kExpInfo.m_cCharType = cCharType;
        kExpInfo.m_biExp = pkCharInfo->m_biExp;
        kExpInfo.m_biTotalExp = pkCharInfo->m_biTotalExp;
        kExpInfo.m_dwOldLevel = pkCharInfo->m_dwLevel;
        kExpInfo.m_dwLevel = pkCharInfo->m_dwLevel;
        kExpInfo.m_fRewardExp = 0;

        mapInfo_[*cvit] = kExpInfo;
    }
}

void KUser::DumpDungeonDropInfo(IN const std::vector< KGameDropElement >& vecDrop_)
{
    //START_LOG( cwarn, L"--- Dump DungeonDrop Info (size:" << vecDrop_.size() << L") ---"  ) << END_LOG;
    //std::vector< KGameDropElement >::const_iterator cvit;
    //for ( cvit = vecDrop_.begin() ; cvit != vecDrop_.end() ; ++cvit ) {
    //    std::Warn << L" DropUID:" << cvit->m_dwUID
    //        << L", UserUID:" << cvit->m_dwUserUID
    //        << L", TriggerID:" << cvit->m_nTriggerID
    //        << L", MonID:" << cvit->m_nMonID
    //        << L", RewardType:" << cvit->m_cRewardType
    //        << L", ItemID:" << cvit->m_dwItemID
    //        << L", Count:" << cvit->m_nCount
    //        << L", Period:" << cvit->m_nPeriod << dbg::endl;
    //}
    //START_LOG( cwarn, L"--- End Dump ---"  ) << END_LOG;
}

void KUser::LevelUpDrop(IN KUserPtr& spUser_, IN const char& cCharType_, IN const char& cOldLv_, IN const char& cCurruntLv_)
{
    JIF(spUser_);

    KEVENT_SHANGHAI_DROP_REQ shReq;
    shReq.m_vecDropItems.clear();
    shReq.m_vecAutoMission.clear();
    shReq.m_vecDropPostItems.clear();
    shReq.m_cCharType = cCharType_;

    KSHCheckPoint::SHChecData shData;
    shData.m_cCurrentCharID = cCharType_;
    shData.m_cOldLevel = cOldLv_;
    shData.m_cLevel = cCurruntLv_;
    shData.m_strLogin = spUser_->GetName();

    START_LOG(clog, L"SHANGHAI_DROPCHECK")
        << BUILD_LOGc(shData.m_cCurrentCharID)
        << BUILD_LOGc(shData.m_cOldLevel)
        << BUILD_LOGc(shData.m_cLevel) << END_LOG;

    // 레벨업 아이템 지급
    if (spUser_->m_tmFirstLoginTime > SiKGSSimLayer()->GetSHAdhustTime() &&
        SiKSHCheckPoint()->DropCheck(shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, KUser::USER_CT_TOTAL)) {
        START_LOG(clog, L"SHANGHAI_DROP Item : " << shReq.m_vecDropItems.size() << "PostItem : " << shReq.m_vecDropPostItems.size() << L", Mission : " << shReq.m_vecAutoMission.size()) << END_LOG;
    }

    // PC방 유저 아이템 지급
    if (spUser_->IsPCBangUser()) {
        int nCount = SiKIPAdvantage()->AddLevelUpReward(shData.m_cOldLevel, shData.m_cLevel, shReq.m_vecDropItems);
        START_LOG(clog, L"PC방 유저가 레벨업 해서 아이템 받음.. Name : " << spUser_->GetName() << L", Count : " << nCount) << END_LOG;
    }

    // 반갑다 친구야 아이템 지급
    if ((spUser_->IsNewRecomUser() || spUser_->IsReturnRecomUser()) && m_MyRecomInfo.m_nState >= KRecomHelper::RBS_ACCEPT_RECOM) {
        std::pair<int, int> prRecomInfo = spUser_->GetNewRecomUserInfo();
        int nCount = spUser_->RecomLevelUpCheck(shData.m_cOldLevel, shData.m_cLevel, prRecomInfo.second, shReq.m_vecDropItems);
        START_LOG(clog, L"반갑다 친구야 신규/복귀 유저가 레벨업 해서 아이템 받음.. Name : " << spUser_->GetName() << L", Count : " << nCount)
            << BUILD_LOGc(shData.m_cOldLevel)
            << BUILD_LOGc(shData.m_cLevel)
            << BUILD_LOG(prRecomInfo.first)
            << BUILD_LOG(prRecomInfo.second) << END_LOG;
    }

    // 채널링 아이템 지급
    if (SiKSHCheckPoint()->DropCheck(shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, spUser_->GetUserChannelType())) {
        START_LOG(clog, L"채널링 유저  레벨업 해서 아이템 받음.. Name : " << spUser_->GetName())
            << BUILD_LOGc(shData.m_cOldLevel)
            << BUILD_LOGc(shData.m_cLevel) << END_LOG;
    }

    // 드랍 미션 정보 킵
    if (!shReq.m_vecAutoMission.empty()) {
        spUser_->m_kLevelUpDropInfo.m_vecAutoMission.insert(spUser_->m_kLevelUpDropInfo.m_vecAutoMission.end(), shReq.m_vecAutoMission.begin(), shReq.m_vecAutoMission.end());
    }
    // 드랍 아이템 정보 킵
    if (!shReq.m_vecDropItems.empty()) {
        spUser_->m_kLevelUpDropInfo.m_vecDropItems.insert(spUser_->m_kLevelUpDropInfo.m_vecDropItems.end(), shReq.m_vecDropItems.begin(), shReq.m_vecDropItems.end());
    }
    // 드랍 우편 아이템 정보 킵
    if (!shReq.m_vecDropPostItems.empty()) {
        spUser_->m_kLevelUpDropInfo.m_vecDropPostItems.insert(spUser_->m_kLevelUpDropInfo.m_vecDropPostItems.end(), shReq.m_vecDropPostItems.begin(), shReq.m_vecDropPostItems.end());
    }


    //  레벨 초기화 이벤트 달성 여부 체크
    if (SiKLevelResetEvent()->IsRun() && shData.m_cOldLevel < shData.m_cLevel)
    {
        std::vector<KDropItemInfo> vecReward;
        bool bCashType = false;

        bool bReward = spUser_->CheckRewardLevel(shData.m_cCurrentCharID, shData.m_cLevel, bCashType, vecReward);

        if (bReward)
        {
            KDB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ kDBPacket;
            kDBPacket.m_dwVersion = SiKLevelResetEvent()->GetVersion();
            kDBPacket.m_cCharType = shData.m_cCurrentCharID;
            kDBPacket.m_vecItemInfo.swap(vecReward);

            //  보상 지급 요청 및 데이터 셋팅    
            _QUEUEING_EVENT_TO_DB(DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ, kDBPacket, spUser_);
        }
    }
}

void KUser::CheckLevelUpDrop(IN const KEVENT_SHANGHAI_DROP_REQ& kPacket_)
{
    m_kLevelUpDropInfo.m_cCharType = kPacket_.m_cCharType;
    if (!kPacket_.m_vecAutoMission.empty()) {
        m_kLevelUpDropInfo.m_vecAutoMission.insert(m_kLevelUpDropInfo.m_vecAutoMission.end(), kPacket_.m_vecAutoMission.begin(), kPacket_.m_vecAutoMission.end());
    }

    if (!kPacket_.m_vecDropItems.empty()) {
        m_kLevelUpDropInfo.m_vecDropItems.insert(m_kLevelUpDropInfo.m_vecDropItems.end(), kPacket_.m_vecDropItems.begin(), kPacket_.m_vecDropItems.end());
    }

    if (!kPacket_.m_vecDropPostItems.empty()) {
        m_kLevelUpDropInfo.m_vecDropPostItems.insert(m_kLevelUpDropInfo.m_vecDropPostItems.end(), kPacket_.m_vecDropPostItems.begin(), kPacket_.m_vecDropPostItems.end());
    }

    if (m_kLevelUpDropInfo.m_vecAutoMission.empty() && m_kLevelUpDropInfo.m_vecDropItems.empty() && m_kLevelUpDropInfo.m_vecDropPostItems.empty()) {
        START_LOG(clog, L"레벨업으로 받은 미션이나 아이템이 없음. LoginID : " << GetName()) << END_LOG;
        return;
    }

    QueueingEvent(KUserEvent::EVENT_SHANGHAI_DROP_REQ, m_kLevelUpDropInfo);
    DumpAutoMission(std::wstring(L"CheckLevelUpDrop"), m_kLevelUpDropInfo.m_vecAutoMission);

    // DB에 등록했으면 초기화(중요)
    m_kLevelUpDropInfo.m_cCharType = 0;
    m_kLevelUpDropInfo.m_vecAutoMission.clear();
    m_kLevelUpDropInfo.m_vecDropItems.clear();
    m_kLevelUpDropInfo.m_vecDropPostItems.clear();
}

void KUser::DumpMonsterDropGP(IN const std::map<std::pair<int, int>, std::map<PAIR_DWORD, USHORT> >& mapDropGP_)
{
    //if ( false == SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    return;
    //}

    //DWORD dwSumGP = 0;

    //START_LOG( cwarn, L"--- Dump MonsterDrop GP (size:" << mapDropGP_.size() << L") ---"  ) << END_LOG;

    //std::map<std::pair<int,int>, std::map<PAIR_DWORD,USHORT> >::const_iterator cmitMon;
    //for ( cmitMon = mapDropGP_.begin() ; cmitMon != mapDropGP_.end() ; ++cmitMon ) {

    //    std::Warn << L"MonID : " << cmitMon->first.first << L",TriggerID : " << cmitMon->first.second;

    //    DWORD dwMonGP = 0;

    //    std::map<PAIR_DWORD,USHORT>::const_iterator cmitCoin;
    //    for ( cmitCoin = cmitMon->second.begin() ; cmitCoin != cmitMon->second.end() ; ++cmitCoin ) {
    //        std::Warn << L",CoinType : " << cmitCoin->first.first << L",GP : " << cmitCoin->first.second << L",count : " << cmitCoin->second;

    //        DWORD dwCoinGP = cmitCoin->first.second * cmitCoin->second;
    //        dwMonGP += dwCoinGP;

    //        std::Warn << L",CurrentGP : " << dwCoinGP;
    //    }

    //    START_LOG( cwarn, L" [MonGP : " << dwMonGP << L"]"  ) << END_LOG;
    //    dwSumGP += dwMonGP;
    //}

    //START_LOG( cwarn, L"Sum GP : " << dwSumGP  ) << END_LOG;
    //START_LOG( cwarn, L"--- End Dump ---"  ) << END_LOG;
}

void KUser::SetInitGP(IN const int nCurrentGP_)
{
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if (pkCharacterInfo == NULL) {
        return;
    }
    // 초기GP값 설정
    pkCharacterInfo->m_nInitGP = std::max<int>(nCurrentGP_, 0);
    pkCharacterInfo->m_nInitGP = std::min<int>(nCurrentGP_, SiKGSSimLayer()->m_iMaxGamePoint);
}

void KUser::SetGP(IN const int nCurrentGP_)
{
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if (pkCharacterInfo == NULL) {
        return;
    }
    // GP값 설정
    pkCharacterInfo->m_nGamePoint = std::max<int>(nCurrentGP_, 0);
    pkCharacterInfo->m_nGamePoint = std::min<int>(nCurrentGP_, SiKGSSimLayer()->m_iMaxGamePoint);
}

void KUser::SetInitGP(IN const int nCurrentGP_, IN const char cCharType_)
{
    KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cCharType_);
    if (pkCharacterInfo == NULL) {
        return;
    }
    // 초기GP값 설정
    pkCharacterInfo->m_nInitGP = std::max<int>(nCurrentGP_, 0);
    pkCharacterInfo->m_nInitGP = std::min<int>(nCurrentGP_, SiKGSSimLayer()->m_iMaxGamePoint);
}

void KUser::SetGP(IN const int nCurrentGP_, IN const char cCharType_)
{
    KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cCharType_);
    if (pkCharacterInfo == NULL) {
        return;
    }
    // GP값 설정
    pkCharacterInfo->m_nGamePoint = std::max<int>(nCurrentGP_, 0);
    pkCharacterInfo->m_nGamePoint = std::min<int>(nCurrentGP_, SiKGSSimLayer()->m_iMaxGamePoint);
}

void KUser::IncreaseGP(IN const int& nType_, IN const bool bDBUpdate_, IN OUT int& nIncGP_)
{
    // GP 증가시
    nIncGP_ = std::max<int>(nIncGP_, 0);
    if (nIncGP_ <= 0) {
        return;
    }

    // GP 상한선 커트
    int nLimitGP = SiKGSSimLayer()->m_iMaxGamePoint - GetGP();
    nIncGP_ = std::min<int>(nIncGP_, nLimitGP);

    //m_iGamePoint += nIncGP_;
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if (pkCharacterInfo == NULL) {
        return;
    }
    pkCharacterInfo->m_nGamePoint += nIncGP_;

    if (bDBUpdate_) {
        pkCharacterInfo->m_nInitGP += nIncGP_;
    }
    else {
        m_nIncGpSum += nIncGP_;
    }

    int nModeID = -1;
    if (m_pkRoom != NULL) {
        nModeID = m_pkRoom->m_iGameMode;
    }

    int nCharType = 0;
    int nPromotion = 0;
    int nLevel = 0;

    KCharacterInfo* pkCharInfo = GetCurrentCharacter();
    if (pkCharInfo) {
        nCharType = static_cast<int>(pkCharInfo->m_cCharType);
        nPromotion = static_cast<int>(pkCharInfo->m_cCurrentPromotion);
        nLevel = static_cast<int>(pkCharInfo->m_dwLevel);
    }

    // GP통계
    KStatGPPtr spStat = GET_STAT(KStatGP, SI_STAT_GP);
    if (spStat) {
        KGpStatInfo kData(nType_, static_cast<__int64>(nIncGP_), nModeID, nCharType, nLevel, nPromotion);
        spStat->AddStat(KGpStatInfo::ST_GP_INC, kData);
    }

    START_LOG(clog, L"Type : " << nType_
        << L", IncGP : " << nIncGP_
        << L", GetGP : " << GetGP()
        << L", Login : " << GetName()) << END_LOG;

    if (nModeID > -1) START_LOG(cwarn, L", ModeID : " << nModeID) << END_LOG;

}

void KUser::DecreaseGP(IN const int& nType_, IN const bool bDBUpdate_, IN const int& nDecGP_)
{
    // GP 감소시
    int nLimitGP = std::min<int>(GetGP(), nDecGP_);

    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if (pkCharacterInfo == NULL) {
        return;
    }
    pkCharacterInfo->m_nGamePoint -= nDecGP_;
    pkCharacterInfo->m_nGamePoint = std::max<int>(pkCharacterInfo->m_nGamePoint, 0);

    if (bDBUpdate_) {
        pkCharacterInfo->m_nInitGP -= nDecGP_;
    }
    else {
        m_nDecGpSum += nDecGP_;
    }

    int nCharType = 0;
    int nPromotion = 0;
    int nLevel = 0;

    KCharacterInfo* pkCharInfo = GetCurrentCharacter();
    if (pkCharInfo) {
        nCharType = static_cast<int>(pkCharInfo->m_cCharType);
        nPromotion = static_cast<int>(pkCharInfo->m_cCurrentPromotion);
        nLevel = static_cast<int>(pkCharInfo->m_dwLevel);
    }

    // GP통계
    KStatGPPtr spStat = GET_STAT(KStatGP, SI_STAT_GP);
    if (spStat) {
        KGpStatInfo kData(nType_, static_cast<__int64>(nDecGP_), -1, nCharType, nLevel, nPromotion);
        spStat->AddStat(KGpStatInfo::ST_GP_DEC, kData);
    }
}

void KUser::ApplyChangeWeapon(IN const std::map<char, KChangeEquipment >& mapEquip_)
{
    std::map<char, KChangeEquipment >::const_iterator cmit;
    for (cmit = mapEquip_.begin(); cmit != mapEquip_.end(); ++cmit) {
        if (cmit->first != GetCurrentCharType()) {
            continue;
        }

        KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cmit->first);
        if (pkCharacterInfo == NULL) {
            START_LOG(cwarn, L"해당 캐릭터가 없음. Name : " << m_strName) << END_LOG;
            continue;
        }

        std::map<char, std::pair<bool, GCITEMUID> >::iterator mitEquip;
        mitEquip = m_mapChangeWeapon.find(cmit->first);
        if (mitEquip == m_mapChangeWeapon.end()) {
            START_LOG(cwarn, L"해당 캐릭터의 체인지무기 정보가 없다. CharType : " << cmit->first) << END_LOG;
            continue;
        }

        if (false == mitEquip->second.first) {
            START_LOG(cwarn, L"해당 캐릭터의 체인지무기는 언락 상태이다. CharType : " << cmit->first) << END_LOG;
            continue;
        }

        mitEquip->second.second = cmit->second.m_kChangeWeaponItem.m_dwUID;
        pkCharacterInfo->m_kChangeWeaponItem = cmit->second.m_kChangeWeaponItem;

        START_LOG(clog, L"체인지무기 ItemUID 설정. ItemID : " << cmit->second.m_kChangeWeaponItem.m_dwID << L",ItemUID : " << cmit->second.m_kChangeWeaponItem.m_dwUID) << END_LOG;
    }
}

void KUser::AddChangeWeaponEquipInfo(IN const std::pair<GCITEMID, GCITEMUID>& prChangeWeapon_, IN OUT std::vector<KEquipItemInfo>& vecEquip_)
{
    if (prChangeWeapon_.first == 0) return;

    std::vector<KEquipItemInfo>::iterator vit;
    vit = std::find_if(vecEquip_.begin(), vecEquip_.end(),
        boost::bind(&KEquipItemInfo::m_dwUID, _1) == prChangeWeapon_.second);
    if (vit != vecEquip_.end()) {
        return;
    }

    KItemPtr pkItem = m_kInventory.FindItem(prChangeWeapon_.first, prChangeWeapon_.second);
    JIF(pkItem);

    KEquipItemInfo kTemp;
    kTemp.m_dwID = pkItem->m_ItemID;
    kTemp.m_dwUID = pkItem->m_ItemUID;
    kTemp.m_cEnchantLevel = pkItem->m_cEnchantLevel;
    kTemp.m_cGradeID = pkItem->m_cGradeID;
    kTemp.m_nEquipLevel = static_cast<int>(pkItem->m_sEquipLevel);
    kTemp.m_vecSocket = pkItem->m_vecSocket;
    kTemp.m_vecAttribute = pkItem->m_vecAttribute;
    kTemp.m_cEnchantEquipGradeID = pkItem->m_cEnchantEquipGradeID;
    vecEquip_.push_back(kTemp);
}

void KUser::DumpEquipItems(IN const int& nCharType_, IN const KEquipItemInfo& kChangeWeapon_, IN const std::vector<KEquipItemInfo>& vecEquip_)
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    //if ( vecEquip_.empty() ) return;

    //START_LOG( cwarn, L"--- Dump Equip Items (size:" << vecEquip_.size() << L",char:" << nCharType_ << L") ---"  ) << END_LOG;

    //std::vector<KEquipItemInfo>::const_iterator cvit;
    //for ( cvit = vecEquip_.begin() ; cvit != vecEquip_.end() ; ++cvit ) {
    //    std::Warn << L"ItemID : " << cvit->m_dwID
    //        << L",ItemUID : " << cvit->m_dwUID
    //        << L",EnchantLv : " << (int)cvit->m_cEnchantLevel
    //        << L",Grade : " << (int)cvit->m_cGradeID
    //        << L",Socket size : " << cvit->m_vecSocket.size()
    //        << L",Attribute size : "  << cvit->m_vecAttribute.size()
    //        << L",IsChangeWeapon : " << (cvit->m_dwID == kChangeWeapon_.m_dwID && cvit->m_dwUID == kChangeWeapon_.m_dwUID ? L"True" : L"False")
    //        << dbg::endl;
    //}
    //START_LOG( cwarn, L"--- End Dump ---"  ) << END_LOG;
}

void KUser::DumpSetSkillReq(IN const KChangeSkillSet& kInfo_)
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;

    //START_LOG( cwarn, L"=== Dump SetSkillReq (Login:" << GetName() << L") ==="  ) << END_LOG;

    //{
    //    std::map< std::pair<char,char>, int >::const_iterator cmitSet;
    //    START_LOG( cwarn, L"kInfo_.m_mapSetting.size() : " << kInfo_.m_mapSetting.size()  ) << END_LOG;
    //    for ( cmitSet = kInfo_.m_mapSetting.begin() ; cmitSet != kInfo_.m_mapSetting.end() ; ++cmitSet ) {
    //        std::Warn << L"Char : " << (int)cmitSet->first.first
    //            << L", Promotion : " << (int)cmitSet->first.second
    //            << L", Set : " << cmitSet->second << dbg::endl;
    //    }
    //    std::Warn << dbg::endl;
    //}

    //{
    //    START_LOG( cwarn, L"kInfo_.m_mapSkillSets.size() : " << kInfo_.m_mapSkillSets.size()  ) << END_LOG;
    //    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::const_iterator cmitSkill;
    //    for ( cmitSkill = kInfo_.m_mapSkillSets.begin() ; cmitSkill != kInfo_.m_mapSkillSets.end() ; ++cmitSkill ) {
    //        std::Warn << L"Char : " << (int)cmitSkill->first.first
    //            << L", Promotion : " << (int)cmitSkill->first.second
    //            << L", skill size : " << cmitSkill->second.size() << dbg::endl;

    //        std::vector<KSPSetInfo>::const_iterator cvitSP;
    //        for ( cvitSP = cmitSkill->second.begin() ; cvitSP != cmitSkill->second.end() ; ++cvitSP ) {
    //            std::Warn << std::tab << L"SetID : " << cvitSP->m_nSPSetID;

    //            std::vector<KSkillSlot>::const_iterator cvitSlot;
    //            for ( cvitSlot = cvitSP->m_vecSkills.begin() ; cvitSlot != cvitSP->m_vecSkills.end() ; ++cvitSlot ) {
    //                std::Warn << L"{" << cvitSlot->m_nSlotID
    //                    << L"," << cvitSlot->m_nSkillID << L"},"<< dbg::endl;
    //            }
    //        }
    //        std::Warn << dbg::endl;
    //    }
    //}

    //START_LOG( cwarn, L"=== End Dump ==="  ) << END_LOG;
}

void KUser::DumpServerList(IN const std::vector<KServerInfo>& vecList_)
{
    if (!SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) return;

    START_LOG(cwarn, L"=== Dump ServerList (size:" << vecList_.size() << L") ===") << END_LOG;
    std::vector<KServerInfo>::const_iterator cvit;
    for (cvit = vecList_.begin(); cvit != vecList_.end(); ++cvit) {
        START_LOG(cwarn, cvit->Str()) << END_LOG;
    }
    START_LOG(cwarn, L"=== End Dump ===") << END_LOG;
}

float KUser::GetGuildBonus(IN const int& nBonusType_)
{
    float fBonusRatio = 0.f;

    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return 0.f;
    }

    KGuildPtr spGuild = SiKGuildManager()->GetGuild(m_kGuildUserInfo.m_dwGuildUID);
    __JIF(spGuild != NULL, return 0.f);

    KNGuildInfo kGuild = spGuild->GetNGuildInfo();
    USHORT usGuildLv = SiKGuildManager()->GetGuildLv(kGuild.m_dwPoint);
    USHORT usUserContributeGuildLv = SiKGuildManager()->GetUserContributeGuildLv(m_kGuildUserInfo.m_dwContributePoint);
    fBonusRatio = SiKGuildManager()->GetGuildBonus(nBonusType_, std::min<USHORT>(usUserContributeGuildLv, usGuildLv));

    START_LOG(clog, L"GuildBonus.. type : " << nBonusType_)
        << BUILD_LOG(usGuildLv)
        << BUILD_LOG(usUserContributeGuildLv)
        << BUILD_LOG(fBonusRatio) << END_LOG;

    return fBonusRatio;
}

void KUser::AddGuildContributePoint(IN const int guildPoint_)
{
    if (guildPoint_ > 0) {
        m_kGuildUserInfo.m_dwContributePoint += guildPoint_;
    }

    START_LOG(clog, L"길드 기여도 변화" << GetName())
        << BUILD_LOG(guildPoint_)
        << BUILD_LOG(m_kGuildUserInfo.m_dwContributePoint) << END_LOG;
}

void KUser::ApplyGuildBPoint(IN const KEndGameReq& kReq_, IN OUT KEndGameAck& kAck_)
{ // 클라이언트 결과 표시를 위한 변수 세팅
    //std::vector<KGameResultOut>::iterator vitAck;
    //for ( vitAck = kAck_.m_vecGameResult.begin(); vitAck != kAck_.m_vecGameResult.end(); ++vitAck ) {

    //    std::vector< std::pair< DWORD, int > >::const_iterator vit;

    //    for ( vit = kReq_.m_vecGuildBPoint.begin(); vit != kReq_.m_vecGuildBPoint.end(); ++vit ) {
    //        if ( vit->first == vitAck->m_dwGuildUID ) {
    //            vitAck->m_nGuildBattlePointDiff = vit->second;
    //            vitAck->m_nGuildBattlePoint = SiKGuildManager()->GetGuildBattlePoint( vitAck->m_dwGuildUID );
    //        }
    //    }        
    //}

    kAck_.m_vecGuildBPoint = kReq_.m_vecGuildBPoint;
}

void KUser::DivideChannelByBPoint(IN OUT KEndGameAck& kAck_)
{
    // ApplyGuildBPoint 가 이 함수보다 먼저 호출된다는 것을 가정
    std::vector<KGameResultOut>::iterator vitAck;
    for (vitAck = kAck_.m_vecGameResult.begin(); vitAck != kAck_.m_vecGameResult.end(); ++vitAck) {

        DWORD dwBattlePoint = SiKGuildManager()->GetGuildBattlePoint(vitAck->m_dwGuildUID);
        vitAck->m_ChannelGrade = SiKGuildManager()->DivideChannelGrade(dwBattlePoint);
    }
}

std::wstring KUser::GetGuildName()
{
    if (m_kGuildUserInfo.m_dwGuildUID == 0) {
        return std::wstring();
    }

    KGuildPtr spGuild = SiKGuildManager()->GetGuild(m_kGuildUserInfo.m_dwGuildUID);
    if (!spGuild) {
        return std::wstring();
    }

    KNGuildInfo guildInfo_ = spGuild->GetNGuildInfo();

    if (guildInfo_.m_dwUID == 0) {
        return std::wstring();
    }
    return guildInfo_.m_strName;
}

DWORD KUser::GetGuildBattlePoint()
{
    DWORD ret = SiKGuildManager()->GetGuildBattlePoint(GetGuildUID());
    return ret;
}

void KUser::SetGuildInfo(IN const KNGuildUserInfo& kInfo_)
{
    if (kInfo_.m_dwGuildUID == 0) {
        m_kGuildUserInfo.Init();
        return;
    }

    m_kGuildUserInfo.m_dwGuildUID = kInfo_.m_dwGuildUID;
    m_kGuildUserInfo.m_cMemberLevel = kInfo_.m_cMemberLevel;
    m_kGuildUserInfo.m_cServerUID = kInfo_.m_cServerUID;
    m_kGuildUserInfo.m_dwContributePoint = kInfo_.m_dwContributePoint;
    m_kGuildUserInfo.m_cServerUID = (char)(SiKGameServer()->GetUID());

    //길드 매니저에 등록

    if (false == SiKGuildManager()->Register(m_kGuildUserInfo.m_dwGuildUID, GetThisPtr())) {
        START_LOG(cerr, L"인증시, 길드매니저에 길드원 등록 실패. 초기화함.")
            << BUILD_LOG(m_kGuildUserInfo.m_dwGuildUID)
            << BUILD_LOG(GetUID())
            << BUILD_LOG(GetName()) << END_LOG;
        m_kGuildUserInfo.Init();
    }
}

bool KUser::IsTH3BBUser()
{
    return SiKIPAdvantage()->IsTH3BBUser(m_cUserBenfitType);
}

bool KUser::CheckGachaObtained(IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_)
{
    std::map< int, std::set< std::pair< DWORD, DWORD > > >::iterator mitGachaSetObtained;
    mitGachaSetObtained = m_mapGachaSetObtained.find(nVersion_);
    if (mitGachaSetObtained == m_mapGachaSetObtained.end()) {
        return true;
    }

    std::set< std::pair< DWORD, DWORD > >& setGachaSetObtained = mitGachaSetObtained->second;

    std::pair< DWORD, DWORD > prData;
    prData.first = dwGachaItemID_;
    prData.second = dwType_;
    std::set< std::pair<DWORD, DWORD> >::iterator sit;
    sit = setGachaSetObtained.find(prData);
    if (sit != setGachaSetObtained.end()) {
        return false;
    }

    return true;
}

void KUser::InsertGachaObtained(IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_)
{
    std::pair< DWORD, DWORD > prData(dwGachaItemID_, dwType_);

    m_mapGachaSetObtained[nVersion_].insert(prData);
}

bool KUser::CheckGachaObtainedByType(IN const int nVersion_, IN const DWORD dwType_)
{
    if (false == SiKGachaManager()->GetGachaDecreaseEnable(nVersion_)) { // 해당 기능 사용 여부에 대한 체크.
        return false;
    }

    std::map< int, std::set< std::pair< DWORD, DWORD > > >::iterator mitGachaSetObtained;
    mitGachaSetObtained = m_mapGachaSetObtained.find(nVersion_);
    if (mitGachaSetObtained == m_mapGachaSetObtained.end()) {
        return false;
    }

    // 해당 타입의 보상을 받았는지 확인.
    std::set< std::pair< DWORD, DWORD > >& setGachaSetObtained = mitGachaSetObtained->second;

    std::set< std::pair< DWORD, DWORD > >::iterator sitGachaSetObtained;
    sitGachaSetObtained = std::find_if(setGachaSetObtained.begin(), setGachaSetObtained.end(),
        boost::bind(&std::set< std::pair< DWORD, DWORD > >::value_type::second, _1) == dwType_);

    if (sitGachaSetObtained == setGachaSetObtained.end()) {
        return false;
    }

    return true;
}

bool KUser::CheckGachaObtained_Level(IN const int nVersion_, IN const DWORD dwGachaItemID_, IN const DWORD dwType_, IN const int nLevel_)
{
    std::map< int, std::map< std::pair< DWORD, int >, std::set< DWORD > > >::iterator mitGachaSetObtainedLevel;
    mitGachaSetObtainedLevel = m_mapGachaSetObtainedLevel.find(nVersion_);
    if (mitGachaSetObtainedLevel == m_mapGachaSetObtainedLevel.end()) {
        return true;
    }

    std::map< std::pair< DWORD, int >, std::set< DWORD > >& mapGachaSetObtained = mitGachaSetObtainedLevel->second;

    std::pair< DWORD, int > prData;
    prData.first = dwGachaItemID_;
    prData.second = nLevel_;
    //prData.second = dwType_;
    std::map< std::pair<DWORD, int>, std::set<DWORD> >::iterator mit;

    mit = mapGachaSetObtained.find(prData);
    if (mit != mapGachaSetObtained.end() && mit->second.find(dwType_) != mit->second.end()) {
        return false;
    }

    return true;
}

void KUser::InsertGachaObtained_Level(IN const int nVersion_, IN const DWORD dwGachaItemID_, IN const DWORD dwType_, IN const int nLevel_)
{
    std::pair< DWORD, int > prData(dwGachaItemID_, nLevel_);

    m_mapGachaSetObtainedLevel[nVersion_][prData].insert(dwType_);
}

bool KUser::CheckGachaObtainedByType_Level(IN const int nVersion_, IN const DWORD dwType_)
{
    if (false == SiKGachaManager()->GetGachaDecreaseEnable(nVersion_)) { // 해당 기능 사용 여부에 대한 체크.
        return false;
    }

    std::map< int, std::map< std::pair< DWORD, int >, std::set< DWORD > > >::iterator mitGachaSetObtainedLevel;
    mitGachaSetObtainedLevel = m_mapGachaSetObtainedLevel.find(nVersion_);
    if (mitGachaSetObtainedLevel == m_mapGachaSetObtainedLevel.end()) {
        return false;
    }

    // 해당 타입의 보상을 받았는지 확인.
    std::map< std::pair< DWORD, int >, std::set< DWORD > >& mapGachaSetObtained = mitGachaSetObtainedLevel->second;

    std::map< std::pair< DWORD, int >, std::set< DWORD > >::iterator mitGachaSetObtained;
    for (mitGachaSetObtained = mapGachaSetObtained.begin(); mitGachaSetObtained != mapGachaSetObtained.end(); ++mitGachaSetObtained) {
        if (mitGachaSetObtained->second.find(dwType_) != mitGachaSetObtained->second.end()) {
            return true;
        }
    }

    return false;
}

void KUser::SendVipEventUserRewardUpdateNot()
{
    if (false == SiKVipEventManager()->IsEventExist()) {
        return;
    }

    time_t tmFirstLogin = KncUtil::TimeToInt(m_tmFirstLoginTime);
    QUEUEING_EVENT_TO_DB(DB_EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT, tmFirstLogin);
}

void KUser::DumpInDoorCharInfo(IN const std::vector<KInDoorCharInfo>& vecList_)
{
    //if ( false == SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    return;
    //}

    //START_LOG( cwarn, L"--- Dump InDoorCharInfo ---"  ) << END_LOG;

    //std::vector<KInDoorCharInfo>::const_iterator cvit;
    //for ( cvit = vecList_.begin() ; cvit != vecList_.end() ; ++cvit ) {
    //    std::clog2 << std::tab << L"CharType : " << (int)cvit->m_cCharType << dbg::endl
    //        << std::tab << L"Promotion : " << (int)cvit->m_cPromotion << dbg::endl
    //        << std::tab << L"CurrentPromotion : " << (int)cvit->m_cCurrentPromotion << dbg::endl
    //        << std::tab << L"CharLv : " << cvit->m_dwLevel << dbg::endl;

    //    START_LOG( cwarn, std::tab << L"EquipItem(size:" << cvit->m_vecEquipItems.size() << L")"  ) << END_LOG;
    //    std::vector<KEquipItemInfo>::const_iterator cvitEquip;
    //    for ( cvitEquip = cvit->m_vecEquipItems.begin() ; cvitEquip != cvit->m_vecEquipItems.end() ; ++cvitEquip ) {
    //        DumpEquipItemInfo( *cvitEquip );
    //    }

    //    START_LOG( cwarn, std::tab << L"ChangeItem"  ) << END_LOG;
    //    DumpEquipItemInfo( cvit->m_kChangeWeaponItem );
    //}

    //START_LOG( cwarn, L"--- End Dump ---"  ) << END_LOG;
}

void KUser::DumpEquipItemInfo(IN const KEquipItemInfo& kItem_)
{
    //if ( false == SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    return;
    //}

    //std::Warn << std::tab << L"ItemID : " << kItem_.m_dwID
    //    << L",ItemUID : " << kItem_.m_dwUID
    //    << L",EnchantLv : " << (int)kItem_.m_cEnchantLevel
    //    << L",Grade : " << (int)kItem_.m_cGradeID
    //    << L",EquipLv : " << kItem_.m_sEquipLevel
    //    << L",EnchantEquipGrade : " << kItem_.m_cEnchantEquipGradeID
    //    << L",SocketSize : " << kItem_.m_vecSocket.size()
    //    << L",AttributeSize : " << kItem_.m_vecAttribute.size() << dbg::endl;
}

bool KUser::GetRKTornadoNotExistList(IN DWORD& dwCharType_, OUT std::map<int, GCITEMID>& mapNotExistList_)
{
    mapNotExistList_.clear();
    std::set<GCITEMID> setItemList;
    std::set<GCITEMID>::iterator sit;
    int nIndex = 0;

    SiKRKTornado()->GetEquipmentList(dwCharType_, setItemList);

    // 인벤토리 찾기
    for (sit = setItemList.begin(); sit != setItemList.end(); ++sit)
    {
        if (m_kInventory.FindItemByItemID(*sit) == NULL)
        {
            // 보유하지 않은 아이템 리스트 구하기
            mapNotExistList_.insert(std::make_pair(nIndex, *sit));
            ++nIndex;
        }
    }
    return true;
}

bool KUser::CheckSameNationGroup(IN const std::wstring& wstrUserCCode_, IN const std::wstring& wstrRoomHostCCode_)
{
    if (wstrUserCCode_ == wstrRoomHostCCode_) {
        return true;
    }
    return false;
}

bool KUser::GetNationRewardExpGP(OUT bool& bActiveNationReward_, OUT float& fNationBoostExp_, OUT float& fNationBoostGP_)
{
    _JIF(m_pkRoom, return false);

    bActiveNationReward_ = m_pkRoom->GetNationRewardEnable();

    if (bActiveNationReward_ && SiKGCHelper()->IsNationRewardCheck()) {
        SiKGCHelper()->GetNationExpGPRewardRatio(fNationBoostExp_, fNationBoostGP_);
    }
    return true;
}

bool KUser::GetSocketCashItem(IN GCITEMID CashItemID_,
    IN KItem& kItem_,
    OUT KItem& CashItem_,
    OUT int& nUseCount_)
{
    // 캐쉬 아이템 ID 확인
    if (CashItemID_ != SOCKET_OPEN_ITEM_ID) {
        SET_ERROR(ERR_SOCKET_OPEN_02);
        return false;
    }

    KItemPtr pkCashItem = m_kInventory.FindItemByItemID(CashItemID_);

    if (pkCashItem == NULL) {
        SET_ERROR(ERR_SOCKET_OPEN_01);
        return false;
    }
    if (pkCashItem) {
        CashItem_ = *pkCashItem;

        int nUseCount = 0;
        nUseCount = SiKItemManager()->GetUseSocketItemCount(kItem_);

        if (nUseCount > pkCashItem->m_nCount) {
            SET_ERROR(ERR_SOCKET_OPEN_01); // 캐쉬 아이템 수량 부족
            return false;
        }
        nUseCount_ = nUseCount;
    }
    return true;
}

void KUser::CheckGameInfo(OUT KEVENT_START_GAME_BROAD& kPacket_)
{
    JIF(m_pkRoom);

    MAP_CHAR_LEVEL mapCharLevelInfo;
    m_pkRoom->GetCharLevelInfo(mapCharLevelInfo);

    kPacket_.m_mapDotaItemInfo.clear();
    kPacket_.m_mapUserDPointInfo.clear();

    // 던전모드 체크
    if (KRoom::IsPvPMode(m_pkRoom->m_iGameMode)) {
        if (SiKDotaManager()->CheckDotaMode(m_pkRoom->m_iGameMode)) {
            SiKDotaManager()->GetDotaItemList(kPacket_.m_mapDotaItemInfo);
        }
        SiKDotaManager()->GetDotaModeInfo(m_pkRoom->m_iGameMode, kPacket_.m_kDotaModeInfo);
        { // DPoint 최대 누적치 넣어주기.
            std::map<DWORD, std::pair<char, int> >::iterator mit;
            for (mit = mapCharLevelInfo.begin(); mit != mapCharLevelInfo.end(); ++mit) {
                int nDPoint = 1000; // DPoint의 기본값을 Max로 고정.
                SiKDotaManager()->GetDotaDPointInfo(mit->second.second, nDPoint);
                std::pair<char, int> prData(mit->second.first, nDPoint);
                kPacket_.m_mapUserDPointInfo[mit->first] = prData;
            }
        }
    }
    m_pkRoom->SetDotaInfo(kPacket_.m_kDotaModeInfo);
}

void KUser::AddStatDungeonAdd(IN const KGameResultIn& kResultIn_)
{
    // 던전 통계
    if (m_pkRoom == NULL)
        return;

    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if (pkCharacter == NULL)
        return;

    KDungeonStatAddA kAddData;
    KUser* pkUser = m_pkRoom->GetHostPtr();
    if (pkUser) {
        kAddData.m_dwLoginUID = pkUser->GetUID();
    }
    else {
        kAddData.m_dwLoginUID = 0;
    }
    kAddData.m_nModeID = m_pkRoom->m_iGameMode;
    kAddData.m_nDifficult = m_pkRoom->m_nDifficulty;
    kAddData.m_mapItemUseCount = kResultIn_.m_mapItemUseCount;
    kAddData.m_mapItemBuyCount = kResultIn_.m_mapItemBuyCount;
    kAddData.m_mapZoneDeathCount = kResultIn_.m_mapZoneDeathCount;
    kAddData.m_mapDeathType = kResultIn_.m_mapDeathType;
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tmCurrentTime = KncUtil::TimeToInt(tmCurrent); // 현재 시간 int변환.
    kAddData.m_tEndGameTime = tmCurrentTime;

    QUEUEING_EVENT_TO_DB(EVENT_DUNGEON_STAT_INFO, kAddData);
    START_LOG(clog, L"던전 통계 추가") << END_LOG;
}

void KUser::SetEnterSquareTime()
{
    m_dwEnterSquareTime = ::timeGetTime();
}

DWORD KUser::GetLeaveSquareTime()
{
    m_dwLeaveSquareTime = ::timeGetTime() - m_dwEnterSquareTime;
    return m_dwLeaveSquareTime;
}

void KUser::ExitParty()
{
    if (GetPartyUID() == 0) {
        return;
    }

    if (!IsMatchSoloParty()) { // 매칭 솔로 파티가 아닐경우에만 Center에 전송.
        KPartyData kData;
        KPartyMember kMember;
        kMember.m_dwUID = GetUID();

        kData.m_dwPartyUID = m_kParty.m_dwPartyUID;
        kData.m_vecPartyPeople.push_back(kMember);

        SiKCnConnector()->SendPacket(KCenterEvent::ECN_LEAVE_PARTY_REQ, kData);

        START_LOG(clog, L"파티 탈퇴 요청 보내기, Login : " << GetName())
            << BUILD_LOG(kData.m_dwPartyUID) << END_LOG;
    }

    m_kParty.Init();

    START_LOG(clog, L"파티 초기화, Login : " << GetName()) << END_LOG;
}

int KUser::GetPartyIndex()
{
    int returnIndex = -1;

    size_t index = 0;
    for (index = 0; index < m_kParty.m_vecPartyPeople.size(); ++index) {
        if (m_kParty.m_vecPartyPeople[index].m_dwUID == GetUID()) {
            returnIndex = index;
            break;
        }
    }

    START_LOG(clog, L"파티에서 몇 번째 위치인가 , Position : " << returnIndex)
        << BUILD_LOG(m_kParty.m_dwPartyUID) << END_LOG;

    return returnIndex;
}

void KUser::CheckAgitBonus()
{
    if (::GetTickCount() - GetTick(AGIT_BONUS) < 1000 * 10) {
        return;
    }

    KItemPtr pkItem;
    KDropItemInfo kDropItem;
    kDropItem.m_ItemID = SiKAgitHelper()->GetSeedItemID();
    kDropItem.m_nDuration = SiKAgitHelper()->GetSeedDropCount();
    kDropItem.m_nPeriod = -1;

    if (m_dwStayAgitUID == GetUID()) {
        START_LOG(clog, L"자기 아지트에선 보너스 없음.. AgitUID : " << m_dwStayAgitUID << L", LoginID : " << GetName()) << END_LOG;
        goto END_PROC2;
    }

    // 유효하지 않은 아지트UID
    if (m_dwStayAgitUID == 0) {
        StateTransition(KUserFSM::INPUT_TO_CHANNELLOBBY);
        START_LOG(clog, L"아지트 퇴장처리..(아지트UID없음) Login : " << GetName()) << END_LOG;
        SEND_ID(EVENT_LEAVE_AGIT_ACK);
        goto END_PROC2;
    }

    // 씨앗 드랍주기 체크
    if (::GetTickCount() - GetTick(AGIT_STAY) < SiKAgitHelper()->GetSeedDropTime()) {
        START_LOG(clog, L"씨앗 드랍주기가 아니다..  Login : " << GetName())
            << BUILD_LOG(SiKAgitHelper()->GetSeedDropTime())
            << BUILD_LOG(::GetTickCount() - GetTick(AGIT_STAY)) << END_LOG;
        goto END_PROC2;
    }

    pkItem = m_kInventory.FindItemByItemID(kDropItem.m_ItemID);
    if (pkItem != NULL) {

        if (pkItem->m_nCount >= SiKAgitHelper()->GetInvenLimitSeedCount()) {
            if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
                START_LOG(clog, L"씨앗 최대 보유개수 제한. Login : " << GetName())
                    << BUILD_LOG(pkItem->m_ItemID)
                    << BUILD_LOG(pkItem->m_ItemUID)
                    << BUILD_LOG(pkItem->m_nCount)
                    << BUILD_LOG(SiKAgitHelper()->GetInvenLimitSeedCount()) << END_LOG;
                goto END_PROC;
            }
        }

        if (m_nTodaySeedDropCount >= SiKAgitHelper()->GetDailyMaxSeedCount()) {
            if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
                START_LOG(clog, L"씨앗 일일 획득 제한. Login : " << GetName())
                    << BUILD_LOG(pkItem->m_ItemID)
                    << BUILD_LOG(pkItem->m_ItemUID)
                    << BUILD_LOG(m_nTodaySeedDropCount)
                    << BUILD_LOG(SiKAgitHelper()->GetDailyMaxSeedCount()) << END_LOG;
                goto END_PROC;
            }
        }

        kDropItem.m_nDuration = std::min<int>(kDropItem.m_nDuration, SiKAgitHelper()->GetInvenLimitSeedCount() - pkItem->m_nCount); // 인벤보유제한
        kDropItem.m_nDuration = std::min<int>(kDropItem.m_nDuration, SiKAgitHelper()->GetDailyMaxSeedCount() - m_nTodaySeedDropCount); // 일일획득제한
    }

    kDropItem.m_nDuration = std::max<int>(kDropItem.m_nDuration, 0);
    if (kDropItem.m_nDuration <= 0) {
        if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
            START_LOG(clog, L"획득할 수 있는 씨앗의 개수가 0 이하다. Login : " << GetName())
                << BUILD_LOG(kDropItem.m_ItemID)
                << BUILD_LOG(kDropItem.m_nDuration) << END_LOG;
            goto END_PROC;
        }
    }

    // 획득할 수량을 먼저 더해준다.
    m_nTodaySeedDropCount += kDropItem.m_nDuration;
    START_LOG(clog, L"체류시간 보너스 씨앗 드랍수량 설정.. Login : " << GetName())
        << BUILD_LOG(m_nTodaySeedDropCount)
        << BUILD_LOG(kDropItem.m_ItemID)
        << BUILD_LOG(kDropItem.m_nDuration) << END_LOG;

    QUEUEING_EVENT_TO_DB(EVENT_AGIT_STAY_BONUS_DROP_REQ, kDropItem);

END_PROC:
    SetTick(AGIT_STAY);
END_PROC2:
    SetTick(AGIT_BONUS);
}

float KUser::GetFairyTreeExpBonus()
{
    float fExpBonus = 0.f;

    std::map<int, float>::iterator mit;
    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_EXP);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fExpBonus += mit->second;
        return fExpBonus;
    }

    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_EXP_GP);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fExpBonus += mit->second;
        return fExpBonus;
    }

    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_NEW_BUFF);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fExpBonus += mit->second;
        return fExpBonus;
    }

    return fExpBonus;
}

float KUser::GetFairyTreeGpBonus()
{
    float fGpBonus = 0.f;

    std::map<int, float>::iterator mit;
    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_GP);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fGpBonus += mit->second;
        return fGpBonus;
    }

    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_EXP_GP);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fGpBonus += mit->second;
        return fGpBonus;
    }

    mit = m_mapFairyTreeFruitBuff.find(KFairyTreeConfig::FBT_NEW_BUFF);
    if (mit != m_mapFairyTreeFruitBuff.end()) {
        fGpBonus += mit->second;
        return fGpBonus;
    }

    return fGpBonus;
}

void KUser::UpdateServerTimeNotice()
{
    if (::GetTickCount() - GetTick(SERVER_TIME_NOT) < 1000 * 60) {
        return;
    }

    ServerTimeNot();
    SetTick(SERVER_TIME_NOT);
}

bool KUser::GetAgitUserInfo(OUT KAgitUserInfo& kUserInfo_)
{
    // 아지트 입장할 캐릭터 정보가 없음.
    const KCharacterInfo* pkCharacterInfo = NULL;
    KPetInfo* pkPetInfo = NULL;

    pkCharacterInfo = GetCurrentCharacter();
    if (pkCharacterInfo == NULL) {
        return false;
    }

    kUserInfo_.m_nServerUID = SiKGameServer()->GetUID();
    kUserInfo_.m_dwUID = GetUID();
    kUserInfo_.m_strNick = GetNickName();
    kUserInfo_.m_cCharType = pkCharacterInfo->m_cCharType;
    kUserInfo_.m_cPromotion = pkCharacterInfo->m_cCurrentPromotion;
    kUserInfo_.m_dwLevel = pkCharacterInfo->m_dwLevel;
    kUserInfo_.m_vecEquipItems = pkCharacterInfo->m_vecEquipItems;
    ExtractCurrentLookItem(pkCharacterInfo->m_cCharType, kUserInfo_.m_vecLookEquips);

    kUserInfo_.m_kPetInfo.m_dwPetID = 0;
    if (GetPetInfo(pkCharacterInfo->m_kEquipPetInfo.m_dwUID, &pkPetInfo)) {
        kUserInfo_.m_kPetInfo.m_dwPetID = pkPetInfo->m_dwID;
        kUserInfo_.m_kPetInfo.m_cPetPromotion = pkPetInfo->m_cPromotion;
        kUserInfo_.m_kPetInfo.m_cPetLV = pkPetInfo->m_iLevel;
        kUserInfo_.m_kPetInfo.m_strPetName = pkPetInfo->m_strName;
        kUserInfo_.m_kPetInfo.m_kCostume = pkPetInfo->m_kCostume;
    }
    GetSkillSettingData(pkCharacterInfo->m_cCharType, kUserInfo_.m_mapSkills);

    return true;
}

void KUser::CheckFlowerPotDrop(IN const DWORD& dwMapID_, OUT std::vector<KDropItemInfo>& vecAddItem_, OUT std::vector<GCITEMUID>& vecEraseItem_)
{
    vecAddItem_.clear();
    vecEraseItem_.clear();

    KAgitMap kMap;
    if (false == SiKAgitHelper()->GetCurrentMapInfo(dwMapID_, kMap)) {
        START_LOG(clog, L"해당 아지트 맵ID 정보가 없음. MapID : " << dwMapID_) << END_LOG;
        return;
    }

    std::set<GCITEMID> setPotID;
    SiKAgitHelper()->GetNormalPotID(setPotID);

    if (setPotID.empty()) {
        START_LOG(clog, L"일반 화분 ItemID정보가 비어있음.") << END_LOG;
        return;
    }

    std::vector<GCITEMUID> vecPotUID;
    std::set<GCITEMID>::iterator sit;
    for (sit = setPotID.begin(); sit != setPotID.end(); ++sit) {
        std::vector<GCITEMUID> vecTemp;
        m_kInventory.FindItemUIDsByItemID(*sit, vecTemp);
        vecPotUID.insert(vecPotUID.end(), vecTemp.begin(), vecTemp.end());
    }

    if (vecPotUID.size() > kMap.m_usMaxFlowerPot) {
        // 제거할 화분 UID 리스트
        std::sort(vecPotUID.begin(), vecPotUID.end());
        USHORT usSize = std::min<USHORT>(kMap.m_usMaxFlowerPot, vecPotUID.size());
        vecEraseItem_.insert(vecEraseItem_.end(), vecPotUID.begin() + usSize, vecPotUID.end());

        START_LOG(cwarn, L"맵ID보다 많은 화분 개수를 보유하고 있음. LoginID : " << GetName())
            << BUILD_LOG(dwMapID_)
            << BUILD_LOG(vecPotUID.size())
            << BUILD_LOG(kMap.m_usMaxFlowerPot)
            << BUILD_LOG(vecEraseItem_.size()) << END_LOG;
        return;
    }

    if (vecPotUID.size() < kMap.m_usMaxFlowerPot) {
        // 추가할 화분 아이템 리스트
        KDropItemInfo kDropItem;
        kDropItem.m_ItemID = *(setPotID.begin());
        kDropItem.m_nDuration = -1;
        kDropItem.m_nPeriod = -1;
        for (USHORT i = 0; i < kMap.m_usMaxFlowerPot - vecPotUID.size(); ++i) {
            vecAddItem_.push_back(kDropItem);
        }

        START_LOG(cwarn, L"맵ID보다 적은 화분 개수를 보유하고 있음. LoginID : " << GetName())
            << BUILD_LOG(dwMapID_)
            << BUILD_LOG(vecPotUID.size())
            << BUILD_LOG(kMap.m_usMaxFlowerPot)
            << BUILD_LOG(vecAddItem_.size()) << END_LOG;
        return;
    }

    START_LOG(clog, L"맵ID에 해당하는 화분 개수를 보유하고 있음. LoginID : " << GetName())
        << BUILD_LOG(dwMapID_)
        << BUILD_LOG(vecPotUID.size())
        << BUILD_LOG(kMap.m_usMaxFlowerPot) << END_LOG;
}

void KUser::LeaveAgit()
{
    if (m_dwStayAgitUID == 0) {
        return;
    }

    // 아지트 떠남
    int nSID = SiKAgitHelper()->GetAgitSID(m_dwStayAgitUID);
    KEAG_LEAVE_AGIT_REQ kAgPacket;
    kAgPacket.m_dwAgitUID = m_dwStayAgitUID;
    kAgPacket.m_dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket(nSID, KAgentEvent::EAG_LEAVE_AGIT_REQ, kAgPacket);
}

void KUser::AgentUserDisconnectNot()
{
    // 내 접속 종료 알림
    KUserTimeParam kAgPacket;
    kAgPacket.m_dwUserUID = GetUID();
    kAgPacket.m_tmTime = KncUtil::TimeToInt(CTime::GetCurrentTime());
    SEND_DATA_TO_AGENT_MASTER(EAG_USER_DISCONNECT_NOT, kAgPacket);
}

void KUser::CheckInventoryBuffItem(OUT std::set<GCITEMID>& setItemID_)
{
    setItemID_.clear();

    std::vector<KInvenBuffItem> vecInvenBuffItemID;
    SiKItemManager()->GetInvenBuffItemList(vecInvenBuffItemID);

    std::vector<KInvenBuffItem>::iterator vit;
    for (vit = vecInvenBuffItemID.begin(); vit != vecInvenBuffItemID.end(); ++vit) {
        if (m_kInventory.FindItemByItemID(vit->m_ItemID) == NULL) {
            continue;
        }

        setItemID_.insert(vit->m_ItemID);
    }

    START_LOG(clog, L"인벤토리 버프아이템 리스트.. size : " << setItemID_.size()) << END_LOG;
}

void KUser::CheckShutdownUser()
{
    KShutdownUserConfig kConfig;
    SiKShutdownUser()->GetConfig(kConfig);

    if (false == kConfig.m_bEnable) {
        return;
    }

    if (false == kConfig.m_bGameTickCheck) {
        return;
    }

    if (::GetTickCount() - GetTick(SHUTDOWN_CHECK_TICK) < kConfig.m_dwShutdownTick) {
        return;
    }

    if (false == SiKShutdownUser()->CheckAge(GetUserAge(), GetIP(), KShutdownUser::CT_GAME_TICK)) {
        KEVENT_SHUTDOWN_ALARM_NOT kPacket;
        kPacket.m_prAge.first = kConfig.m_prAge.first;
        kPacket.m_prAge.second = kConfig.m_prAge.second;
        kPacket.m_prStartTime = kConfig.m_prStartTime;
        kPacket.m_prEndTime = kConfig.m_prEndTime;
        kPacket.m_usPrevTime = 0;

        // 클라이언트 디스플레이용으로 +1 해줍니다.
        kPacket.m_prAge.second += 1;
        SEND_PACKET(EVENT_SHUTDOWN_USER_DISCONNECT_NOT);

        START_LOG(cwarn, L"Shutdown Target User.. Login : " << GetName())
            << BUILD_LOG(kConfig.m_dwShutdownTick)
            << BUILD_LOG(GetUserAge())
            << BUILD_LOG(kConfig.m_prAge.first)
            << BUILD_LOG(kConfig.m_prAge.second) << END_LOG;

        // 서버에서 종료시키지 않고, 클라이언트에서 패킷을 받은후 종료를 진행합니다.
        //ReserveDestroy( DRW_SHUTDOWN_USER );
    }

    SetTick(SHUTDOWN_CHECK_TICK);
}

void KUser::CheckGameConnectSurvey(IN const DWORD& dwChannelType_, IN const time_t& tFirstLoginTime_, IN const time_t& tLastLoginTime_, OUT KEVENT_SURVEY_LIST_NOT& kSurveyData_)
{
    int nUserType = 0;
    int nLanguageCode = GetLanguageCode();
    if (SiKSurvey()->IsNewUser(tFirstLoginTime_)) { // 유저 타입 결정.
        nUserType = KSurvey::UT_NEW;
    }
    else if (SiKSurvey()->IsReturnUser(tLastLoginTime_)) {
        nUserType = KSurvey::UT_RETURN;
    }
    else {
        nUserType = KSurvey::UT_SAVE;
    }

    if (m_kSurveyUserData.CheckSurveyStatus(KSurvey::ST_GAME_CONNECT, dwChannelType_, nUserType)) {
        kSurveyData_.m_nType = KSurvey::ST_GAME_CONNECT;
        kSurveyData_.m_nDataFirst = dwChannelType_;
        kSurveyData_.m_nDataSecond = nUserType;
        __LIF(SiKSurvey()->GetSurveyList(nLanguageCode, KSurvey::ST_GAME_CONNECT, dwChannelType_, nUserType, kSurveyData_.m_mapSurveyInfo));
    }
    else {
        kSurveyData_.m_nType = -1;
        kSurveyData_.m_nDataFirst = -1;
        kSurveyData_.m_nDataSecond = -1;
        kSurveyData_.m_mapSurveyInfo.clear();
    }
}

// 필요한 정보.
void KUser::CheckSurveyInfo(IN const int& nGameMode_,
    IN const int& nDifficult_,
    IN std::vector<KGameResultIn>& vecGameResult_,
    IN std::map<DWORD, std::pair<int, DWORD>>& m_mapLevelUpUser_)
{
    int nLanguageCode = GetLanguageCode();

    std::vector<KGameResultIn>::iterator vitBroad;
    for (vitBroad = vecGameResult_.begin(); vitBroad != vecGameResult_.end(); ++vitBroad) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(vitBroad->m_strLogin);
        if (!spUser) {
            START_LOG(cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vitBroad->m_strLogin) << END_LOG;
            continue;
        }
        // 레벨업한 정보 있는지 확인하고 설문조사 전달해야 하는지 체크.
        if (!m_mapLevelUpUser_.empty()) {
            std::map<DWORD, std::pair<int, DWORD>>::iterator mit;
            mit = m_mapLevelUpUser_.find(spUser->GetUID());
            if (mit != m_mapLevelUpUser_.end()) {
                if (spUser->m_kSurveyUserData.CheckSurveyStatus(KSurvey::ST_CAHR_LEVELUP, mit->second.first, mit->second.second)) {
                    KEVENT_SURVEY_LIST_NOT kPacket;
                    kPacket.m_nType = KSurvey::ST_CAHR_LEVELUP;
                    kPacket.m_nDataFirst = mit->second.first;
                    kPacket.m_nDataSecond = mit->second.second;
                    __LIF(SiKSurvey()->GetSurveyList(nLanguageCode, KSurvey::ST_CAHR_LEVELUP, mit->second.first, mit->second.second, kPacket.m_mapSurveyInfo));
                    spUser->QueueingEvent(KUserEvent::EVENT_SURVEY_LIST_NOT, kPacket);
                    continue;
                } // if
            } // if
        } // if

        if (false == spUser->m_kSurveyUserData.CheckSurveyStatus(KSurvey::ST_GAME_CLEAR, nGameMode_, nDifficult_)) {
            START_LOG(clog, L"CheckSurveyStatus.. Login : " << spUser->GetName() << L", ModeID : " << nGameMode_ << L", Difficult : " << nDifficult_) << END_LOG;
            continue;
        }// if

        KEVENT_SURVEY_LIST_NOT kPacket;
        kPacket.m_nType = KSurvey::ST_GAME_CLEAR;
        kPacket.m_nDataFirst = nGameMode_;
        kPacket.m_nDataSecond = nDifficult_;
        __LIF(SiKSurvey()->GetSurveyList(nLanguageCode, KSurvey::ST_GAME_CLEAR, nGameMode_, nDifficult_, kPacket.m_mapSurveyInfo));
        spUser->QueueingEvent(KUserEvent::EVENT_SURVEY_LIST_NOT, kPacket);


    } // for

    START_LOG(clog, L"CheckSurveyInfo..")
        << BUILD_LOG(nGameMode_)
        << BUILD_LOG(nDifficult_)
        << BUILD_LOG(vecGameResult_.size())
        << BUILD_LOG(m_mapLevelUpUser_.size())
        << BUILD_LOG(nLanguageCode) << END_LOG;
}

void KUser::SetUserLevelUpInfo(IN const DWORD& dwUID_, IN const int& nCharType_, IN const DWORD& dwLevel_, IN OUT std::map<DWORD, std::pair<int, DWORD>>& mapLevelUpUser_)
{
    std::map<DWORD, std::pair<int, DWORD>>::iterator mit;
    std::pair<int, DWORD> prData;
    prData.first = nCharType_;
    prData.second = dwLevel_;

    mit = mapLevelUpUser_.find(dwUID_);
    if (mit == mapLevelUpUser_.end()) {
        mapLevelUpUser_[dwUID_] = prData;
    }
    else {
        mit->second = prData;
    }
}

void KUser::SendUserLogOutInfo()
{
    KECN_USER_EXIT_GAME_NOT kCnPacket;
    kCnPacket.m_wstrLogin = m_strName;
    kCnPacket.m_wstrIP = KncUtil::toWideString(GetIPStr());
    kCnPacket.m_biUniqueKey = m_biUniqueKey;

    if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_NTREEV)) {
        kCnPacket.m_nGameCode = KUser::NG_GC_US;
        kCnPacket.m_nUniqueKey = m_nUniqueKey;
    }
    else if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_SGI_EU)) {
        kCnPacket.m_nGameCode = KUser::NG_GC_EU;
    }
    LIF(SiKCnConnector()->SendPacket(KCenterEvent::ECN_USER_EXIT_GAME_NOT, kCnPacket));
}

void KUser::SendNMCafeLogin(UINT PlayTime_/*=0*/)
{
    if (false == SiKGSSimLayer()->IsNMCafeEnable()) return;
    if (false == SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_NETMARBLE)) return;
    //if ( false == m_bNMNetcafeActive ) return;

    KNmNetcafeParam kPacket;
    kPacket.m_UserCN = m_strNMUserCN;
    kPacket.m_PlayTime = PlayTime_;
    kPacket.m_IpAddress = static_cast<DWORD>(GetIP());
    KTThreadManager<NetCafeManager>::GetInstance()->QueueingEvent(KUserEvent::EVENT_NMNETCAFE_LOGIN_REQ, m_strName.c_str(), m_dwUID, 0, kPacket);

    START_LOG(clog, L"넷마블PC방 로그인(차감) 요청.. Login : " << GetName())
        << BUILD_LOG(KncUtil::toWideString(kPacket.m_UserCN))
        << BUILD_LOG(GetIP())
        << BUILD_LOG(GetIPStr())
        << BUILD_LOG(PlayTime_) << END_LOG;
}

void KUser::SendNMCafeLogout()
{
    if (false == SiKGSSimLayer()->IsNMCafeEnable()) return;
    if (false == SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_NETMARBLE)) return;

    KNmNetcafeParam kPacket;
    kPacket.m_UserCN = m_strNMUserCN;
    kPacket.m_IpAddress = static_cast<DWORD>(GetIP());
    KTThreadManager<NetCafeManager>::GetInstance()->QueueingEvent(KUserEvent::EVENT_NMNETCAFE_LOGOUT_REQ, m_strName.c_str(), m_dwUID, 0, kPacket);

    START_LOG(clog, L"넷마블PC방 로그아웃 패킷전달.. Login : " << GetName())
        << BUILD_LOG(KncUtil::toWideString(kPacket.m_UserCN))
        << BUILD_LOG(GetIP())
        << BUILD_LOG(GetIPStr()) << END_LOG;
}

void KUser::CheckLevelUpSurvey(IN const int& nCharType_, IN const int& nLevel_)
{
    int nLanguageCode = GetLanguageCode();

    if (m_kSurveyUserData.CheckSurveyStatus(KSurvey::ST_CAHR_LEVELUP, nCharType_, nLevel_)) {
        KEVENT_SURVEY_LIST_NOT kPacket;
        kPacket.m_nType = KSurvey::ST_CAHR_LEVELUP;
        kPacket.m_nDataFirst = nCharType_;
        kPacket.m_nDataSecond = nLevel_;
        __LIF(SiKSurvey()->GetSurveyList(nLanguageCode, KSurvey::ST_CAHR_LEVELUP, nCharType_, nLevel_, kPacket.m_mapSurveyInfo));
        SEND_PACKET(EVENT_SURVEY_LIST_NOT);
    }
}

void KUser::LeaveGameCheckSurveyInfo(IN const int& nCharType_,
    IN const DWORD& dwLevel_,
    IN std::wstring wstrLogin_)
{
    int nLanguageCode = GetLanguageCode();

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(wstrLogin_);
    if (!spUser) {
        START_LOG(cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << wstrLogin_) << END_LOG;
        return;
    }
    // 레벨업한 정보 있는지 확인하고 설문조사 전달해야 하는지 체크.
    if (spUser->m_kSurveyUserData.CheckSurveyStatus(KSurvey::ST_CAHR_LEVELUP, nCharType_, dwLevel_)) {
        KEVENT_SURVEY_LIST_NOT kPacket;
        kPacket.m_nType = KSurvey::ST_CAHR_LEVELUP;
        kPacket.m_nDataFirst = nCharType_;
        kPacket.m_nDataSecond = dwLevel_;
        __LIF(SiKSurvey()->GetSurveyList(nLanguageCode, KSurvey::ST_CAHR_LEVELUP, nCharType_, dwLevel_, kPacket.m_mapSurveyInfo));
        spUser->QueueingEvent(KUserEvent::EVENT_SURVEY_LIST_NOT, kPacket);
    } // if
}
bool KUser::DungeonTicketCheck(IN const int& nGameMode_)
{
    if (IsAdmin()) {
        START_LOG(clog, L"운영자는 검사 안함.") << END_LOG;
        return true;
    }

    if (false == KRoom::IsDungeonMode(nGameMode_)) {
        START_LOG(clog, L"던전 모드가 아니면 통과. ModeID : " << nGameMode_) << END_LOG;
        return true;
    }

    if (m_kInventory.FindItemByItemID(SiKGCHelper()->GetDungeonFreePassID())) {
        START_LOG(clog, L"던전 프리패스 소유자는 통과. ModeID : " << nGameMode_) << END_LOG;
        return true;
    }

    KDungeonTicket kTicket;
    if (false == SiKGCHelper()->GetDungeonTicketInfo(nGameMode_, kTicket)) {
        START_LOG(clog, L"티켓 체크하는 던전이 아니면 통과. ModeID : " << nGameMode_) << END_LOG;
        return true;
    }


    if (kTicket.m_mapEnterTicket.empty()) {
        START_LOG(clog, L"입장티켓 체크 안하면 통과. ModeID : " << nGameMode_) << END_LOG;
        return true;
    }

    std::map<GCITEMID, int>::iterator mit;
    for (mit = kTicket.m_mapEnterTicket.begin(); mit != kTicket.m_mapEnterTicket.end(); ++mit) {
        const GCITEMID& TicketItemID = mit->first;
        const UINT& uiNeedCount = mit->second;
        UINT uiInvenCount = GetDurItemCount(TicketItemID);

        if (uiInvenCount >= uiNeedCount) {
            START_LOG(clog, L"입장티켓 조건을 만족하면 통과. ModeID : " << nGameMode_)
                << BUILD_LOG(TicketItemID)
                << BUILD_LOG(uiInvenCount)
                << BUILD_LOG(uiNeedCount) << END_LOG;
            return true;
        }
    }

    START_LOG(cwarn, L"입장티켓을 보유하고 있지않음. ModeID : " << nGameMode_)
        << BUILD_LOG(kTicket.m_nModeID)
        << BUILD_LOG(kTicket.m_mapEnterTicket.size())
        << BUILD_LOG(kTicket.m_mapBossTicket.size()) << END_LOG;
    return false;
}

void KUser::SendDungeonTicketListNot()
{
    KEVENT_DUNGEON_TICKET_LIST_NOT kPacket;
    SiKGCHelper()->GetDungeonTicketList(kPacket);
    SEND_PACKET(EVENT_DUNGEON_TICKET_LIST_NOT);
    START_LOG(clog, L"던전별 티켓정보 리스트 전달.. size : " << kPacket.size()) << END_LOG;
}

void KUser::SendUserConnectNoticeToAgentMaster()
{
    KUserTimeParam kAgPacket;
    kAgPacket.m_dwUserUID = GetUID();
    kAgPacket.m_tmTime = KncUtil::TimeToInt(CTime::GetCurrentTime());
    SEND_DATA_TO_AGENT_MASTER(EAG_USER_CONNECT_NOT, kAgPacket);
    START_LOG(clog, L"내 접속알림. AgitUID : " << GetUID() << L", LoginID : " << GetName()) << END_LOG;
}

bool KUser::CheckItemInvenLimit(IN const GCITEMID& ItemID_)
{
    if (SiKItemManager()->m_kItemConfig.IsNoneSizeItem(ItemID_)) {
        START_LOG(clog, L"인벤토리 차지하지 않는 아이템.. ItemID : " << ItemID_) << END_LOG;
        SET_ERROR(NET_OK);
        return true;
    }

    // 아지트 인벤토리 체크
    bool bAgitObject = SiKAgitHelper()->IsObjectItem(ItemID_);
    if (bAgitObject) {
        if (m_kInventory.GetAgitObjectItemNum() >= SiKAgitHelper()->GetMaxInvenSize()) {
            START_LOG(cwarn, L"아지트아이템 받기에 충분한 인벤 공간이 없음.. Login : " << GetName())
                << BUILD_LOG(ItemID_)
                << BUILD_LOG(m_kInventory.GetAgitObjectItemNum())
                << BUILD_LOG(SiKAgitHelper()->GetMaxInvenSize()) << END_LOG;

            SET_ERROR(ERR_ITEM_BUY_CHECK_04);
            return false;
        }
    }

    // 코디 인벤토리 체크
    bool bLookItem = SiKLookEquipment()->IsLookItemID(ItemID_);
    if (bLookItem) {
        if (m_kInventory.GetLookItemNum() >= GetTrueLookInvenCapacity()) {
            START_LOG(cwarn, L"코디아이템 받기에 충분한 인벤 공간이 없음.. Login : " << GetName())
                << BUILD_LOG(ItemID_)
                << BUILD_LOG(m_kInventory.GetLookItemNum())
                << BUILD_LOG(GetCharLookInvenCapacity()) << END_LOG;

            SET_ERROR(ERR_ITEM_BUY_CHECK_03);
            return false;
        }
    }

    // 일반 인벤토리 체크
    if (false == bLookItem && false == bAgitObject) {
        if (m_kInventory.GetTotalEnableSize() >= GetTrueInvenCapacity()) {
            START_LOG(cwarn, L"일반아이템 첨부 받기에 충분한 인벤 공간이 없음.. Login : " << GetName())
                << BUILD_LOG(ItemID_)
                << BUILD_LOG(m_kInventory.GetTotalEnableSize())
                << BUILD_LOG(GetTrueInvenCapacity()) << END_LOG;

            SET_ERROR(ERR_ITEM_BUY_CHECK_02);
            return false;
        }
    }

    START_LOG(clog, L"Check ItemID : " << ItemID_ << L", Login : " << GetName())
        << BUILD_LOG(bLookItem)
        << BUILD_LOG(bAgitObject)
        << BUILD_LOG(m_kInventory.GetTotalEnableSize())
        << BUILD_LOG(GetTrueInvenCapacity()) << END_LOG;

    SET_ERROR(NET_OK);
    return true;
}

bool KUser::CheckItemCount(IN const GCITEMID& ItemID_)
{
    // 아이템 보유량 제한 체크
    if (false == SiKAgitHelper()->IsObjectItem(ItemID_)) {
        START_LOG(clog, L"아지트 아이템이 아님.. ItemID : " << ItemID_) << END_LOG;
        return true;
    }

    int nLimitCount = 0;
    if (false == SiKAgitHelper()->GetObjectItemInvenLimitCount(ItemID_, nLimitCount)) {
        START_LOG(clog, L"인벤토리 보유량 체크하는 아이템이 아님.. ItemID : " << ItemID_) << END_LOG;
        return true;
    }

    int nItemInvenCount = m_kInventory.GetItemCount(ItemID_);
    if (nLimitCount > 0 && nItemInvenCount >= nLimitCount) {
        START_LOG(cwarn, L"인벤토리 아이템 소지한도 초과함.. LoginID :" << GetName())
            << BUILD_LOG(ItemID_)
            << BUILD_LOG(nLimitCount)
            << BUILD_LOG(nItemInvenCount) << END_LOG;
        return false;
    }

    return true;
}

void KUser::CheckEscortInfo(OUT KEVENT_START_GAME_BROAD& kPacket)
{
    kPacket.m_dwEscortMonsterID = 0;

    JIF(m_pkRoom);

    if (SiKEscort()->IsRun()) {
        PAIR_DWORD prKey = std::make_pair(m_pkRoom->m_iGameMode, static_cast<DWORD>(m_pkRoom->m_nDifficulty));
        kPacket.m_bIsActiveEscort = SiKEscort()->IsHaveEscortInfo(prKey, kPacket.m_dwEscortMonsterID);
    }
    else {
        kPacket.m_bIsActiveEscort = false;
    }

    START_LOG(clog, L"Escort Event Active Info is setted: " << kPacket.m_bIsActiveEscort << ", MonID: " << kPacket.m_dwEscortMonsterID) << END_LOG;
}

void KUser::GetDBUpdateDataGP(OUT KGPData& kGPData_)
{
    // DB thread로 보낸 캐싱 데이터는 항상 성공한다고 가정한다.
    kGPData_.m_iInitGP = GetInitGP();
    kGPData_.m_iGamePoint = GetGP();
    kGPData_.m_nIncGpSum = m_nIncGpSum;
    kGPData_.m_nDecGpSum = m_nDecGpSum;

    SetInitGP(GetGP());
    m_nIncGpSum = 0;
    m_nDecGpSum = 0;
}

void KUser::CheckNewPost()
{
    if (false == SiKPostConfig()->IsPostEnable()) return;

    // 시간 체크
    if (::GetTickCount() - GetTick(NEW_POST_UPDATE) < SiKPostConfig()->GetCheckNewPostTickGap()) {
        return;
    }

    QueueingEvent(KUserEvent::EVENT_RECEIVE_LETTER_NOT, GetUID());
    SetTick(NEW_POST_UPDATE);
}

void KUser::UpdateCharLvPlayTime(IN const int& nCharType_, IN const DWORD& dwLv_)
{
    int nTimeDiff = GetCharLvStartGameTimeDiff();
    if (nTimeDiff <= 0) return;

    __LIF(IsCharExist(static_cast<char>(nCharType_)));
    __LIF(dwLv_ > 0);
    __LIF(dwLv_ <= SiKResultManager()->GetMaxLevel());

    m_mapUserCharLvPlayTime[nCharType_][dwLv_] += nTimeDiff;

    START_LOG(clog, L"유저 캐릭터 레벨 플레이시간 통계 누적.. LoginID : " << GetName())
        << BUILD_LOG(nCharType_)
        << BUILD_LOG(dwLv_)
        << BUILD_LOG(nTimeDiff)
        << BUILD_LOG(m_mapUserCharLvPlayTime[nCharType_][dwLv_]) << END_LOG;
}

int KUser::GetCharLvStartGameTimeDiff()
{
    int nTimeDiff = (::GetTickCount() - m_dwCharLvStartGameTime) / (60 * 1000);
    if (nTimeDiff > 0) {
        SetCharLvStartGameTime();
    }

    return nTimeDiff;
}

void KUser::UpdateDungeonClearInfo(IN const int& nModeID_, IN const int& nDifficulty_, IN const KncBitStream& kModeDifficulty_, IN OUT KDungeonClearData& kClearData_)
{
    if ( false == SiKHeroDungeonManager()->IsHeroDungeon( nModeID_ ) ) {
        return;
    }

    KHeroDungeon kInfo;
    bool bHeroDungeon = SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID_, kInfo); // 영웅던전이면 정보 획득
    int nLastDifficulty = SiKResultManager()->GetLastDifficulty(nModeID_); // 던전 최고 난이도

    // 마지막 난이도 갱신 (공통)
    kClearData_.m_shLastDifficulty = static_cast<SHORT>(nDifficulty_);

    if (nLastDifficulty < 0) {
        START_LOG(cerr, L"던전 최고 난이도 정보가 없음.. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_ << L", LsatDifficulty : " << nLastDifficulty) << END_LOG;
        return;
    }

    if (bHeroDungeon) {
        // 영웅던전 처리

        // 플레이 가능한 최대 난이도 갱신
        if (nDifficulty_ == static_cast<int>(kClearData_.m_shMaxDifficulty)) {
            kClearData_.m_shMaxDifficulty = std::min<int>(static_cast<int>(kClearData_.m_shMaxDifficulty + 1), nLastDifficulty);
            START_LOG(clog, L"플레이가능한 영웅던전 최대 난이도 갱신.. Login : " << GetName() << L", ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_ << L", MaxDifficulty : " << kClearData_.m_shMaxDifficulty) << END_LOG;
        }

        // 최고난이도를 클리어했나?
        if (nDifficulty_ >= nLastDifficulty) {
            // 던전 클리어 갱신 체크
            if (false == kClearData_.m_bClear) {
                kClearData_.m_bClear = true;
                START_LOG(clog, L"영웅던전 클리어 기록.. Login : " << GetName() << L", ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
            }

            // 난이도 초기화 하는지
            if (kInfo.m_bResetDifficulty) {
                kClearData_.m_shMaxDifficulty = 0;
                START_LOG(clog, L"영웅던전 최대 난이도 초기화.. Login : " << GetName() << L", ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
            }

            // 보상 있으면 보상개수 초기화
            if (kInfo.m_usClearRewardCount > 0) {
                USHORT usLeftCount = kClearData_.m_usLeftRewardCount;
                kClearData_.m_usLeftRewardCount = kInfo.m_usClearRewardCount;
                START_LOG(clog, L"영웅던전 클리어 보상개수 초기화.. Login : " << GetName() << L", ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_)
                    << BUILD_LOG(usLeftCount)
                    << BUILD_LOG(kInfo.m_usClearRewardCount) << END_LOG;
            }

            START_LOG(clog, L"영웅던전 최고 난이도 클리어.. Login : " << GetName() << L", ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
        }

        KDB_EVENT_UPDATE_DUNGEON_CLEAR_INFO_NOT kDBPacket;
        kDBPacket.m_nModeID = nModeID_;
        kDBPacket.m_kClearData = kClearData_;
        QUEUEING_EVENT_TO_DB(DB_EVENT_UPDATE_DUNGEON_CLEAR_INFO_NOT, kDBPacket);

        //m_iPvExp += 1;
        //QUEUEING_EVENT_TO_DB(EVENT_PVEXP_UPDATE_TO_DB_NOT, m_iPvExp);
    }
    else {
        START_LOG(clog, L"일반던전 처리는 안함. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
    }

    START_LOG(clog, L"던전 클리어 정보 Update.. Login : " << GetName())
        << BUILD_LOG(nModeID_)
        << BUILD_LOG(nDifficulty_)
        << BUILD_LOG(nLastDifficulty)
        << BUILD_LOG(bHeroDungeon)
        << BUILD_LOG(kClearData_.m_bClear)
        << BUILD_LOG(kClearData_.m_shLastDifficulty)
        << BUILD_LOG(kClearData_.m_shMaxDifficulty)
        << BUILD_LOG(kClearData_.m_usLeftRewardCount) << END_LOG;
}

bool KUser::GetDungeonUserInfo(IN const int& nModeID_, OUT KDungeonUserInfo& kInfo_)
{
    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mitTotal;
    mitTotal = m_mapDifficulty.find(m_cCurrentCharacter);
    if (mitTotal == m_mapDifficulty.end()) {
        return false;
    }

    std::map<int, KDungeonUserInfo>::const_iterator mit;
    mit = mitTotal->second.find(nModeID_);
    if (mit == mitTotal->second.end()) {
        return false;
    }

    kInfo_ = mit->second;
    return true;
}

void KUser::GetCharEquipInfo(OUT std::set<DWORD>& setChangeEquipInfo_)
{
    setChangeEquipInfo_ = m_setChangeEquipInfo;
}

// 개기일식 이벤트가 발동되면, 발동주기 동안 1회의 보상을 받을 수 있게 하는 부분
void KUser::UpdateEclipsePlotTime()
{
    if (!m_kEclipsePlotTimer.CheckTime()) {
        return;
    }

    m_kEclipsePlot.m_bCurrentEventState = SiKEclipseTimeEvent()->IsEventOn();

    if (m_kEclipsePlot.m_bCurrentEventState == true && m_kEclipsePlot.m_bPrevEventState == false) {
        if (SiKEclipseTimeEvent()->GetCurrentPeriod() >= KncUtil::TimeToInt(m_cConnectionTime)) {
            m_kEclipsePlot.m_bRewardable = true; // 보상 받는 부분에서 false 로 바꿔준다. // file:.\User8.cpp#EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ
            SEND_ID(EVENT_ECLIPSE_PLOT_TIME_REWARD_READY_NOT); // 클라이언트에 개기일식 발동시간에 따른 보상 받을 수 있음을 알리는 패킷
        }
    }
    m_kEclipsePlot.m_bPrevEventState = m_kEclipsePlot.m_bCurrentEventState; // 개기일식 on/off 감지하는 변수

}

bool KUser::PvpHackCheck(IN const char& cCharType_, IN const DWORD& dwLv_, IN const __int64& biExpRewardSum_, IN OUT __int64& biExpReward_)
{
    // 최대 획득가능한 경험치보상 체크
    if (biExpReward_ == 0) {
        return true;
    }

    __int64 biMaxExpReward_ = 0;
    if (false == SiKPreHackingCheckManager()->GetPvpLvMaxExp(dwLv_, biMaxExpReward_)) {
        START_LOG(cwarn, L"해당하는 레벨의 획득가능한 최대 경험치값이 테이블에 없음..")
            << BUILD_LOG(dwLv_)
            << END_LOG;
        return false;
    }

    if (biExpRewardSum_ >= biMaxExpReward_) {
        biExpReward_ = 0;
        START_LOG(cwarn, L"획득한 대전경험치가 획득가능한 수치를 초과함.")
            << BUILD_LOGc(cCharType_)
            << BUILD_LOG(biExpRewardSum_)
            << BUILD_LOG(biMaxExpReward_)
            << END_LOG;
        return false;
    }

    biExpReward_ = std::min<__int64>(biExpReward_, biMaxExpReward_ - biExpRewardSum_);
    if (biExpReward_ == 0) {
        START_LOG(cwarn, L"획득할 수 있는 경험치가 없음.")
            << BUILD_LOGc(cCharType_)
            << BUILD_LOG(biExpRewardSum_)
            << BUILD_LOG(biMaxExpReward_)
            << END_LOG;
        return false;
    }

    return true;
}

void KUser::PvpHackDisconnectCheck(IN const char& cModeCategory_)
{
    // 파일 통계용
    ++m_mapPvpCategoryHackCount[cModeCategory_];

    // 접속종료 처리 체크
    if (false == SiKPreHackingCheckManager()->IsPvpErrDisconnect()) {
        return;
    }

    KEVENT_CLOSE_CONNECTION_NOT kPacket;
    kPacket.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_DETECT_HACK;
    kPacket.m_strMessage = SiKGameServer()->m_stringTable.GetValue(L"str_1");
    SEND_DATA(EVENT_CLOSE_CONNECTION_NOT, kPacket);

    int nType = KPreHackingCheckManager::HT_CLIENT_HACKING;
    QUEUEING_EVENT_TO_DB(EVENT_INSERT_HACK_INFO_NOT, nType);
    ReserveDestroy(DRW_CLIENT_HACKING);
}

void KUser::UpdateCharEquipInfo(IN const char& cCharType_)
{
    std::set<DWORD>::const_iterator csit;

    csit = m_setChangeEquipInfo.find(cCharType_);
    if (csit == m_setChangeEquipInfo.end()) {
        m_setChangeEquipInfo.insert(cCharType_);
    }
}


void KUser::UpdateStrengthEquipItemInfo(IN std::map<GCITEMUID, GCITEMUID> mapStrengthItem_, IN OUT std::vector<KItem>& vecItem_, OUT std::vector<KItem>& vecInvenItem_)
{
    vecInvenItem_.clear();
    KItemPtr pkStrengthItem; // 강화석 아이템.
    std::map<GCITEMUID, GCITEMUID>::iterator mit;
    for (mit = mapStrengthItem_.begin(); mit != mapStrengthItem_.end(); ++mit) {
        pkStrengthItem = m_kInventory.FindItemByItemUID(mit->second);
        if (!pkStrengthItem) {
            START_LOG(cerr, L"창고 아이템에 연결된 강화석이 인벤토리에 존재하지 않음. ItemUID : " << mit->second)
                << END_LOG;
            continue;
        }

        std::vector<KItem>::iterator vit = std::find_if(vecItem_.begin(), vecItem_.end(), boost::bind(&KItem::m_ItemUID, _1) == mit->first);
        if (vit != vecItem_.end()) {
            vit->m_cEnchantEquipGradeID = pkStrengthItem->m_cGradeID;
            pkStrengthItem->m_cEnchantLevel = vit->m_cEnchantLevel;
            pkStrengthItem->m_EnchantEquipItemUID = vit->m_ItemUID;
            vecInvenItem_.push_back(*pkStrengthItem);
        }
        else {
            START_LOG(cerr, L"인벤소유의 강화석의 장착 아이템을 DB에서 가져오지 못함 ItemUID : " << mit->first)
                << END_LOG;
        }
    }
}

void KUser::CheckJobMissionGuide(IN const DWORD& dwUID_, IN const char& cCharType_, IN const int& nLevel_)
{
    PAIR_CHAR_INT prKey = std::make_pair(cCharType_, nLevel_);
    KEVENT_JOB_MISSION_GUIDE_NOT kPacket;

    if (SiKJobMissionGuide()->GetJobMissionGuide(prKey, kPacket)) {
        SiKGSSimLayer()->m_kActorManager.SendTo(dwUID_, KUserEvent::EVENT_JOB_MISSION_GUIDE_NOT, kPacket);
    }
}

void KUser::CheckJobMissionInfo(IN std::vector< KGameResultIn >& vecGameResult_)
{
    JIF(m_pkRoom);

    std::vector< KGameResultIn >::iterator vitBroad;
    for (vitBroad = vecGameResult_.begin(); vitBroad != vecGameResult_.end(); ++vitBroad) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName< KUser >(vitBroad->m_strLogin);
        if (!spUser) {
            START_LOG(cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vitBroad->m_strLogin) << END_LOG;
            continue;
        }
        // 레벨업한 정보 있는지 확인하고 전직 미션 가이드 체크
        int nCharType;
        DWORD dwCharLevel;
        if (m_pkRoom->CheckLevelUpEnable(spUser->GetUID(), nCharType, dwCharLevel)) {
            CheckJobMissionGuide(spUser->GetUID(), static_cast<char>(nCharType), static_cast<int>(dwCharLevel));
            START_LOG(clog, L"Name : " << spUser->GetName()) << END_LOG;
        }
    }
    START_LOG(clog, L"Check Job Mission Info")
        << BUILD_LOG(vecGameResult_.size())
        << BUILD_LOG(m_pkRoom->m_mapLevelUpUser.size()) << END_LOG;
}

bool KUser::CheckJustInTimeEvent(OUT time_t& tmNext_, OUT std::map<USHORT, bool>& mapUserType_)
{
    tmNext_ = 0;
    mapUserType_.clear();

    std::map<time_t, std::set<USHORT> > mapUserTypeEvent; // map[JIT_time,set[UserType]]
    mapUserTypeEvent.clear();

    // 이벤트 대상별 JIT 이벤트가 존재하는지 체크
    for (int i = KJustInTime::EU_ALL_USER; i < KJustInTime::EU_MAX; ++i) {
        const int& nUserType = i;
        bool bIsNextEvent = SiKJustInTime()->IsNextEventToday(nUserType);
        if (false == bIsNextEvent) {
            continue;
        }

        // 이벤트 대상 다음JIT시간 가져오기
        time_t tmNextEvent = 0;
        SiKJustInTime()->GetNextEvent(nUserType, tmNextEvent);

        if (tmNextEvent != 0) {
            mapUserTypeEvent[tmNextEvent].insert(nUserType);
        }
    }

    if (mapUserTypeEvent.empty()) {
        START_LOG(clog, L"JIT 이벤트 없음.. size : " << mapUserTypeEvent.size())
            << END_LOG;
        return false;
    }

    if (mapUserTypeEvent.begin()->second.empty()) {
        START_LOG(cwarn, L"JIT 이벤트 유효한 설정이 없음.. size : " << mapUserTypeEvent.size())
            << END_LOG;
        return false;
    }

    // 다음 JIT 시간 설정
    tmNext_ = mapUserTypeEvent.begin()->first;

    // 동일시간에 여러 이벤트가 있을 경우, 이벤트 대상자인지 체크
    std::set<USHORT>::iterator sit;
    for (sit = mapUserTypeEvent.begin()->second.begin(); sit != mapUserTypeEvent.begin()->second.end(); ++sit) {
        const USHORT& usUserType = *sit;
        bool bIsEventUser = false;

        switch (usUserType) {
        case KJustInTime::EU_ALL_USER:
            bIsEventUser = true;
            break;
        case KJustInTime::EU_PCCAFE_USER:
            if (IsPCBangUser()) {
                bIsEventUser = true;
            }
            break;
        case KJustInTime::EU_EX_PCCAFE_USER:
            if (false == IsPCBangUser()) {
                bIsEventUser = true;
            }
            break;
        default:
            START_LOG(cwarn, L"정의되지않은 JIT 대상유저 타입 : " << usUserType)
                << END_LOG;
            continue;
        }

        mapUserType_[usUserType] = bIsEventUser;

        START_LOG(clog, L"JIT 이벤트 대상유저 정보 설정")
            << BUILD_LOGtm(CTime(tmNext_))
            << BUILD_LOG(usUserType)
            << BUILD_LOG(bIsEventUser)
            << END_LOG;
    }

    START_LOG(clog, L"JustInTimeNextTime Notice.. Login : " << GetName())
        << BUILD_LOGtm(CTime(tmNext_))
        << BUILD_LOG(mapUserType_.size())
        << END_LOG;

    return true;
}

bool KUser::GetProperty(IN Kairo& kPropertyCount_, IN std::vector<Kairo>& vecProperty_, OUT DWORD& dwProperty_)
{
    dwProperty_ = 0;
    DWORD dwPropertyCount;
    kPropertyCount_.Do(dwPropertyCount);

    dwPropertyCount = std::max<DWORD>(0, dwPropertyCount);
    dwPropertyCount = std::min<DWORD>(dwPropertyCount, (DWORD)vecProperty_.size());

    if (dwPropertyCount <= 0) return false;

    std::random_shuffle(vecProperty_.begin(), vecProperty_.end());
    std::vector<Kairo>::iterator vit;
    DWORD dwPropertyType = 0;
    for (vit = vecProperty_.begin(); vit != vecProperty_.end(); ++vit) {
        if (dwPropertyCount == 0) {
            return true;
        }
        vit->Do(dwPropertyType);
        dwPropertyType = std::max<DWORD>(dwPropertyType, 0);
        dwProperty_ += dwPropertyType;
        --dwPropertyCount;
    }
    return true;
}

void KUser::GetSpecialMonsterInfo(IN const int nModeID_, IN const int nDifficulty_, OUT KSpecialMonster& kSpecialMonster_)
{
    std::pair<int, int> prStageCount; // min,max stageCount
    KSpecialMonInfo kSpecialMonInfo;
    KSpecialMonster kSpecialMonster;

    if (!SiKSpecialMonsterManager()->IsActive()) {
        START_LOG(clog, L" Special 몬스터 시스템 기능 OFF.") << END_LOG;
        return;
    }

    if (!SiKResultManager()->GetStageCountInfo(nModeID_, nDifficulty_, prStageCount)) {
        START_LOG(clog, L"해당 스테이지 정보가 없음. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
        return;
    }

    if (!SiKSpecialMonsterManager()->GetSpecialMonsterInfo(nModeID_, nDifficulty_, kSpecialMonInfo)) {
        START_LOG(clog, L"Special 몬스터 정보가 없음. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_) << END_LOG;
        return;
    }

    kSpecialMonster.m_nMonsterID = kSpecialMonInfo.m_nMonsterID;
    kSpecialMonster.m_nMonLv = kSpecialMonInfo.m_nMonLv;
    if (kSpecialMonInfo.m_bPropertyEnable) { // 몬스터 추가 속성을 사용할려면..,
        if (!GetProperty(kSpecialMonInfo.m_kPropertyCount, kSpecialMonInfo.m_vecProperty, kSpecialMonster.m_dwProperty)) {
            START_LOG(clog, L" Special 추가 속성 정보가 없음.")
                << BUILD_LOG(kSpecialMonInfo.m_vecProperty.size())
                << END_LOG;
        }
    }

    kSpecialMonster_ = kSpecialMonster;

    START_LOG(clog, L"Special 몬스터 설정")
        << BUILD_LOG(nModeID_)
        << BUILD_LOG(nDifficulty_)
        << BUILD_LOG(kSpecialMonster.m_nMonsterID)
        << BUILD_LOG(kSpecialMonster.m_nMonLv)
        << BUILD_LOG(kSpecialMonster.m_dwProperty)
        << END_LOG;
}

void KUser::CheckAdventureEvent()
{
    JIF(m_pkRoom);

    // 룸에 있던 기존 데이터 초기화
    m_pkRoom->ClearAdventureMissionMonsterData();

    if (!SiKAdventure()->IsRun()) {
        return;
    }

    std::vector< sTriggerInfo > vecTriggers;
    KGameInfo kGameInfo;
    std::map< PAIR_DWORD, KAdventureMonsterInfo > mapAdventureMonsterInfo;
    KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT kPacket;

    vecTriggers.clear();
    m_pkRoom->GetGameInfo(kGameInfo);
    kPacket.clear();

    // 챔피언 모드에서만 동작하도록
    if (kGameInfo.m_nDifficulty < GC_GMD_CHAMPION_BEGIN) {
        START_LOG(clog, L"챔피언 모드가 아님. 모험 이벤트 전리품 드랍하지 않음")
            << BUILD_LOG(kGameInfo.m_iGameMode)
            << BUILD_LOG(kGameInfo.m_nDifficulty)
            << END_LOG;
        return;
    }

    SiKAdventure()->GetAdventureMonsterInfo(mapAdventureMonsterInfo);     // 몬스터 정보 가져오고
    SiKGameDrop()->GetTriggerInfo(kGameInfo.m_iGameMode, kGameInfo.m_nDifficulty, vecTriggers);   // 트리거 정보 가져와서

    // 트리거 전체 순회하면서 모험 이벤트 몬스터 리스트에 (던전ID, 몬스터ID)로 검색
    std::vector< sTriggerInfo >::iterator vitTrigger = vecTriggers.begin();
    for (; vitTrigger != vecTriggers.end(); ++vitTrigger) {
        std::map< PAIR_DWORD, KAdventureMonsterInfo >::iterator mitMonsterInfo;
        PAIR_DWORD prKey = std::make_pair(static_cast<DWORD>(kGameInfo.m_iGameMode), static_cast<DWORD>(vitTrigger->m_nMonsterID));
        mitMonsterInfo = mapAdventureMonsterInfo.find(prKey);
        if (mitMonsterInfo != mapAdventureMonsterInfo.end()) {
            // 있으면 확률 계산한담에 드랍이다 결정될 경우 패킷에 추가
            // 확률 계산
            float fDropProb = SiKGameDrop()->GetRandomNum() / (float)(UINT_MAX);
            fDropProb = std::max< float >(0.f, fDropProb);
            fDropProb = std::min< float >(1.f, fDropProb);

            if (mitMonsterInfo->second.m_fDropChance < fDropProb) {
                continue;
            }

            KAdventureMonster kAdventureMonster;
            kAdventureMonster.m_nTriggerID = vitTrigger->m_nTriggerID;
            kAdventureMonster.m_nContinentID = mitMonsterInfo->second.m_prDropItemInfo.first;
            kAdventureMonster.m_dwCollectID = mitMonsterInfo->second.m_prDropItemInfo.second;
            kAdventureMonster.m_dwDropCount = mitMonsterInfo->second.m_dwDropCount;

            // 패킷에 추가
            if (!kPacket.insert(std::make_pair(kAdventureMonster.m_nTriggerID, kAdventureMonster)).second) {
                START_LOG(cerr, L"중복된 TriggerID가 존재함. 모험 이벤트 드랍 안함")
                    << BUILD_LOG(vitTrigger->m_nTriggerID)
                    << END_LOG;
                kPacket.clear();
                break;
            }
        }
    }

    // 룸에 저장하고
    m_pkRoom->SetAdventureMissionMonsterData(kPacket);

    // 던전 시작중인 유저들에게 해당 정보 방송
    m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT, kPacket, true);
    START_LOG(clog, L"모험 이벤트 미션 정보 룸에 저장, 패킷 전송.")
        << BUILD_LOG(kPacket.size())
        << END_LOG;
}

bool KUser::RemoveCoupleInven(GCITEMUID itemUID_)
{
    std::vector<KSimpleItem>::iterator vit;
    vit = std::find_if(m_kCoupleInfo.m_vecCoupleInven.begin(), m_kCoupleInfo.m_vecCoupleInven.end(),
        boost::bind(&KSimpleItem::m_dwUID, _1) == itemUID_);
    if (vit == m_kCoupleInfo.m_vecCoupleInven.end()) return false;
    m_kCoupleInfo.m_vecCoupleInven.erase(vit);
    return true;
}

void KUser::UpdateCompleteMission(IN const DWORD& dwMissionID_)
{
    std::set<DWORD>::iterator sit;
    sit = m_setCompleteMission.find(dwMissionID_);
    if (sit == m_setCompleteMission.end()) {
        m_setCompleteMission.insert(dwMissionID_);
    }
}

bool KUser::CheckCompleteMission(IN const std::vector<DWORD>& vecMissionInfo_)
{
    std::set<DWORD>::iterator sit;
    std::vector<DWORD>::const_iterator vit;

    for (vit = vecMissionInfo_.begin(); vit != vecMissionInfo_.end(); ++vit) {
        sit = m_setCompleteMission.find(*vit);
        if (sit != m_setCompleteMission.end()) {
            return true;
        }
    }
    return false;
}

void KUser::GetSkillInfoData(IN const char& cCharType_, OUT std::vector<KSPInfo>& vecSPInfo_)
{
    vecSPInfo_.clear();
    const KCharacterInfo* pkChar = GetCharacterInfo(cCharType_);
    JIF(pkChar); // 일어 나면 안됨.
    KSPInfo kSPInfo;
    std::map<std::pair<char, char>, KSPInfo >::iterator mit;
    for (mit = m_mapTrainedSkill.begin(); mit != m_mapTrainedSkill.end(); ++mit) {
        if (mit->first.first == cCharType_) {
            if (mit->second.m_cCharType == -1) {
                mit->second.m_cCharType = cCharType_;
            }
            kSPInfo = mit->second;
            vecSPInfo_.push_back(kSPInfo);
        }
    }
}

void KUser::CheckAdventureCollectingMission(IN KUserPtr& spUser_, IN const int& nTriggerID_)
{
    JIF(m_pkRoom);
    JIF(spUser_);

    KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT::iterator mit;
    mit = m_pkRoom->m_kAdventureMissionMonsterData.find(nTriggerID_);
    if (mit != m_pkRoom->m_kAdventureMissionMonsterData.end()) {
        // 잡은 몬스터가 모험 이벤트 전리품을 드랍할 경우
        std::map< int, std::map< DWORD, int > >::iterator mitCollectData;
        mitCollectData = spUser_->m_kAdventureUserData.m_mapCollectData.find(mit->second.m_nContinentID);
        if (mitCollectData != spUser_->m_kAdventureUserData.m_mapCollectData.end()) {
            // 유저 데이터를 서버는 항상 0 이상의 값으로 갖고있기 때문에 항상 검색이 성공한다
            std::map< DWORD, int >::iterator mitCollect;
            mitCollect = mitCollectData->second.find(mit->second.m_dwCollectID);
            if (mitCollect != mitCollectData->second.end()) {
                int nCount = mitCollect->second + static_cast<int>(mit->second.m_dwDropCount);
                DWORD dwMaxCount = SiKAdventure()->GetMaxCollectCount(mit->second.m_nContinentID, mit->second.m_dwCollectID);
                nCount = std::min< int >(nCount, static_cast<int>(dwMaxCount));
                mitCollect->second = nCount;

                START_LOG(clog, L"모험 이벤트 전리품 미션처리. nTriggerID: " << nTriggerID_
                    << ", nContinentID: " << mit->second.m_nContinentID
                    << ", dwCollectID: " << mit->second.m_dwCollectID
                    << ", nCollectCount: " << mitCollect->second
                    << ", dwMaxCount: " << dwMaxCount)
                    << END_LOG;
            }
        }
    }
}

void KUser::SendSongkranWaterBombNot(IN const DWORD& dwTotalSuppliedDuration_, IN const DWORD& dwSuppliedDuration_)
{
    // 물풍선 변경정보 DB갱신.
    KDB_EVENT_SONGKRAN_WATERBOMB_NOT kDBPacket;
    kDBPacket.m_dwTotalSuppliedDuration = dwTotalSuppliedDuration_;
    kDBPacket.m_dwSuppliedDuration = dwSuppliedDuration_;
    QUEUEING_EVENT_TO_DB(DB_EVENT_SONGKRAN_WATERBOMB_NOT, kDBPacket);
}

bool KUser::CheckRewardLevel(IN const char& cCharType_, IN const char& cCharLevel_, OUT bool& bCashType, IN OUT std::vector<KDropItemInfo>& vecItemInfo_)
{
    bCashType = false;

    // 보상 레벨 확인
    if (cCharLevel_ < SiKLevelResetEvent()->GetRewardLevel())
        return false;

    // 초기화 캐릭터인지 확인
    std::map<char, std::pair<bool, bool>>::iterator iter = m_mapLevelResetCharInfo.find(cCharType_);
    if (iter == m_mapLevelResetCharInfo.end())
        return false;

    // 이미 보상 받았는지 확인
    if (iter->second.second)
        return false;

    bCashType = iter->second.first;

    //  CharType, CashType
    SiKLevelResetEvent()->GetCharRewardList(cCharType_, bCashType, vecItemInfo_);

    return true;
}


void KUser::CheckUserSealedCharInfo(IN std::vector< KGameResultIn >& vecGameResult_)
{
    JIF(m_pkRoom);

    std::vector< KGameResultIn >::iterator vitBroad;
    for (vitBroad = vecGameResult_.begin(); vitBroad != vecGameResult_.end(); ++vitBroad) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName< KUser >(vitBroad->m_strLogin);
        if (!spUser) {
            START_LOG(cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vitBroad->m_strLogin) << END_LOG;
            continue;
        }
        // 레벨업한 정보 있는지 확인하고 봉인 캐릭터 정보 갱신
        int nCharType;
        DWORD dwCharLevel;
        if (m_pkRoom->CheckLevelUpEnable(spUser->GetUID(), nCharType, dwCharLevel)) {
            UpdateUserSealedCharInfo(spUser, static_cast<char>(nCharType), static_cast<int>(dwCharLevel));

            START_LOG(clog, L"봉인 캐릭터 정보 갱신. Name: " << spUser->GetName()) << END_LOG;
        }
    }
    START_LOG(clog, L"클리어시 유저 봉인 캐릭터 정보 확인")
        << BUILD_LOG(vecGameResult_.size())
        << BUILD_LOG(m_pkRoom->m_mapLevelUpUser.size()) << END_LOG;
}

void KUser::UpdateUserSealedCharInfo(IN KUserPtr spUser_, IN const char& cCharType_, IN const int& nLevel_)
{
    JIF(spUser_);

    int nCharType = static_cast<int>(cCharType_);
    int nObjectLevel = 0;
    if (SiKSealedChar()->IsEventTerm(nCharType)) {
        if (false == SiKSealedChar()->GetObjectLevel(nCharType, nObjectLevel)) {
            START_LOG(clog, L"봉인 캐릭터 정보 패킷 설정 중 목표 레벨 가져오기 오류" << nCharType) << END_LOG;
        }
        if (nLevel_ >= nObjectLevel) {
            KDB_EVENT_SEALED_CHARACTER_UPDATE_NOT kDBPacket;
            kDBPacket.m_nCharType = nCharType;
            kDBPacket.m_bSealed = false;

            _QUEUEING_EVENT_TO_DB(DB_EVENT_SEALED_CHARACTER_UPDATE_NOT, kDBPacket, spUser_);
        }
    }
}

void KUser::CheckOlympicBossGateDrop(IN KUserPtr spUser_, IN std::vector< KItem >& vecDropItem_)
{
    JIF(spUser_);

    if (false == SiKOlympicManager()->IsRun()) {
        return;
    }

    KEVENT_OLYMPIC_BOSSGATE_RESULT_NOT kPacket;

    SiKOlympicManager()->GetCurrentOlympicGameInfo(kPacket.m_tmTodayDate, kPacket.m_nGameType, kPacket.m_nTeamType);

    KOlympicGameInfo kOlympicGameInfo;
    SiKOlympicManager()->GetOlympicGameInfo(kPacket.m_nGameType, kOlympicGameInfo);

    // 현재 올림픽 이벤트 보스게이트가 진행중인지 확인
    if (kPacket.m_nGameType != KOlympicGameInfo::EOGT_BOSSGATE) {
        return;
    }

    kPacket.m_nIncreasedData = 0;

    std::vector< KDropItemInfo > vecRewardInfo;
    SiKOlympicManager()->GetGameRewardInfo(kPacket.m_nGameType, vecRewardInfo);

    std::vector< KDropItemInfo >::iterator vitRewardInfo = vecRewardInfo.begin();
    for (; vitRewardInfo != vecRewardInfo.end(); ++vitRewardInfo) {
        std::vector< KItem >::iterator vitDropItem;
        vitDropItem = std::find_if(vecDropItem_.begin(), vecDropItem_.end(),
            boost::bind(&KItem::m_ItemID, _1) == vitRewardInfo->m_ItemID);
        if (vitDropItem == vecDropItem_.end()) {
            continue;
        }

        if (kOlympicGameInfo.m_nRewardCount == 0) {
            kPacket.m_nIncreasedData += 1;
        }
        else {
            kPacket.m_nIncreasedData += kOlympicGameInfo.m_nRewardCount;
        }
    }

    m_kOlympicUserData.GetUserOlympicUserData(kPacket.m_tmTodayDate, kPacket.m_nMyAccumulateData);

    if (!SiKOlympicManager()->CheckGameResultData(kPacket.m_nGameType, kPacket.m_nMyAccumulateData, kPacket.m_nIncreasedData)) {
        // 설정된 게임의 정보가 없음.
        START_LOG(cerr, L"올림픽 이벤트 보스게이트 누적 포인트 갱신 처리 중 설정된 게임의 정보를 찾을 수 없음")
            << BUILD_LOG(kPacket.m_nMyAccumulateData)
            << BUILD_LOG(kPacket.m_nIncreasedData)
            << BUILD_LOG(kPacket.m_tmTodayDate)
            << BUILD_LOG(kPacket.m_nGameType)
            << BUILD_LOG(kPacket.m_nTeamType)
            << END_LOG;

        return;
    }

    // 보스게이트 처리는 TimeIndex 저장을 하지 않기에 증가치가 0이하인 경우 DB 큐잉을 하지 않는다
    if (kPacket.m_nIncreasedData > 0) {
        QUEUEING_EVENT_TO_DB(EVENT_OLYMPIC_BOSSGATE_RESULT_NOT, kPacket);
    }

    START_LOG(clog, L"올림픽 이벤트 보스게이트 누적 포인트 갱신 요청, User : " << m_strName)
        << BUILD_LOG(kPacket.m_nMyAccumulateData)
        << BUILD_LOG(kPacket.m_nIncreasedData)
        << BUILD_LOG(kPacket.m_tmTodayDate)
        << BUILD_LOG(kPacket.m_nGameType)
        << BUILD_LOG(kPacket.m_nTeamType)
        << END_LOG;
}

bool KUser::ValidateCoupleInfo()
{
    if (m_kCoupleInfo.m_tmRegDate == 0 || m_kCoupleInfo.m_tmPartnerRegDate == 0) {
        return false;
    }
    return true;
}

bool KUser::InsertDefaultSkillInfo(IN const int& nSkillID_, IN OUT KCharacterInfo& kCharacterInfo_)
{
    const KCharacterInfo* pkChar = NULL;
    SSkillElement kSkill;

    // 이 스킬이 존재 하는가?
    if (!SiKSkillManager()->GetSkill(nSkillID_, kSkill)) {
        START_LOG(cerr, L"기본 지급 스킬정보 없음")
            << BUILD_LOG(nSkillID_)
            << END_LOG;
        return false;
    }

    // 해당 캐릭터가 존재 하는가?
    pkChar = GetCharacterInfo(kCharacterInfo_.m_cCharType);
    if (!pkChar) {
        START_LOG(cerr, L"캐릭터가 존재하지 않음")
            << BUILD_LOGc(kCharacterInfo_.m_cCharType)
            << END_LOG;
        return false;
    }

    if (IsTrainedSkill(kSkill)) {
        START_LOG(clog, L"이미 배운 스킬")
            << BUILD_LOGc(kCharacterInfo_.m_cCharType)
            << END_LOG;
        return false;
    }

    AddTrainedSkill(kSkill);
    return true;
}

void KUser::CheckUserCharLevelUpEvent(IN const DWORD& dwUID_, IN std::set<char>& setCharInfo_)
{
    std::map<char, KCharacterInfo>::iterator mit;
    std::vector<KDropItemInfo> vecItemInfo;
    vecItemInfo.clear();
    KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;

    for (mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit) {
        if (SiKGCHelper()->ServerConnectCharLevelReward(mit->first, mit->second.m_dwLevel, setCharInfo_, vecItemInfo)) {
            kDBPacket.m_setCharInfo.insert(mit->first);
        }
    }
    if (!vecItemInfo.empty()) {
        kDBPacket.m_vecRewardInfo = vecItemInfo;
        QUEUEING_EVENT_TO_DB(DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket);
    }
}

void KUser::SendInventoryInfo(IN const std::vector<KItem>& vecItem_)
{
    if (vecItem_.empty()) {
        KEVENT_VERIFY_INVENTORY_NOT kPacket;
        SEND_COMPRESS_PACKET(EVENT_VERIFY_INVENTORY_NOT, kPacket);
        return;
    }

    // MaxIndex 구하는 작업.
    DWORD dwOneItemNum = SiKGSSimLayer()->GetMaxItemNum();
    dwOneItemNum = std::max<int>(1000, dwOneItemNum);

    DWORD dwMaxIndex = ((vecItem_.size() - 1) / dwOneItemNum) + 1;
    DWORD dwLastItemNum = vecItem_.size() - (dwOneItemNum * (dwMaxIndex - 1));

    KEVENT_VERIFY_INVENTORY_NOT kPacket;
    kPacket.m_nMaxIndex = dwMaxIndex;
    std::vector<KItem>::const_iterator vit;
    std::vector<KItem> vecItem;
    vecItem.clear();
    vecItem.reserve(dwOneItemNum);
    DWORD dwCount = 0;
    DWORD dwCurrentIndex = 1;
    for (vit = vecItem_.begin(); vit != vecItem_.end(); ++vit) {
        vecItem.push_back(*vit);
        ++dwCount;
        if (dwCount == dwOneItemNum) {
            kPacket.m_nCurrentIndex = dwCurrentIndex;
            kPacket.m_vecInv.clear();
            kPacket.m_vecInv.swap(vecItem);
            SEND_COMPRESS_PACKET(EVENT_VERIFY_INVENTORY_NOT, kPacket);
            ++dwCurrentIndex;
            if (dwCurrentIndex == dwMaxIndex) {
                vecItem.reserve(dwLastItemNum);
            }
            else {
                vecItem.reserve(dwOneItemNum);
            }
            dwCount = 0;
        }
    }
    if (!vecItem.empty()) {
        kPacket.m_nCurrentIndex = dwCurrentIndex;
        kPacket.m_vecInv.clear();
        kPacket.m_vecInv.swap(vecItem);
        SEND_COMPRESS_PACKET(EVENT_VERIFY_INVENTORY_NOT, kPacket);
    }
}

void KUser::CheckGetLimit(IN OUT bool& bItemDropped_, OUT std::map< DWORD, DROP_ITEMS >& mapEachUserDrops_)
{
    JIF(m_pkRoom);

    if (false == SiKGetLimit()->IsRun()) {
        return;
    }

    if (false == bItemDropped_) {
        return;
    }

    std::vector< KGetLimitInfo > vecGetLimitInfo;
    if (false == SiKGetLimit()->GetGetLimitInfo(std::make_pair(m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty), vecGetLimitInfo)) {
        return;
    }

    std::map< DWORD, DROP_ITEMS > mapTempEachUserDrops;
    mapTempEachUserDrops.clear();
    std::map< DWORD, DROP_ITEMS >::iterator mitEachUserDrops = mapEachUserDrops_.begin();
    for (; mitEachUserDrops != mapEachUserDrops_.end(); ++mitEachUserDrops) {
        KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >(mitEachUserDrops->first));
        if (NULL == spUser) {
            continue;
        }

        if (true == mitEachUserDrops->second.empty()) {
            continue;
        }

        // 드랍한 아이템이 획득 제한 리스트에 있는지 확인
        DROP_ITEMS vecTempDropItems;
        vecTempDropItems.clear();
        DROP_ITEMS::iterator vitDropItems = mitEachUserDrops->second.begin();
        for (; vitDropItems != mitEachUserDrops->second.end(); ++vitDropItems) {
            std::vector< KGetLimitInfo >::iterator vitGetLimitInfo;
            vitGetLimitInfo = std::find_if(vecGetLimitInfo.begin(), vecGetLimitInfo.end(),
                boost::bind(&KGetLimitInfo::m_dwItemID, _1) == vitDropItems->m_ItemID);

            if (vitGetLimitInfo == vecGetLimitInfo.end()) {
                continue;
            }

            // 해당 유저 인벤을 검사하여 제한이 넘는지 검사하고 획득할 수 있는 한계로 획득 수량 처리
            KItemPtr pkItem = spUser->m_kInventory.FindDurationItemByItemID(vitGetLimitInfo->m_dwItemID);
            if (NULL == pkItem) {
                continue;
            }

            int nAfterDuration = pkItem->m_nCount + vitDropItems->m_nDuration;
            if (pkItem->m_nCount >= static_cast<int>(vitGetLimitInfo->m_dwLimit)) {
                // 이미 제한 이상으로 갖고 있는 경우 획득 리스트에서 제거해야 한다(모았다가 나중에 한번에 지운다)
                vecTempDropItems.push_back(*vitDropItems);
            }
            else if (nAfterDuration > static_cast<int>(vitGetLimitInfo->m_dwLimit)) {
                // 획득 수량 최대치로 보정 처리
                int nNewGetDuration = nAfterDuration - vitGetLimitInfo->m_dwLimit;
                vitDropItems->m_nDuration = nNewGetDuration;

                START_LOG(clog, L"획득 제한 이벤트로 아이템 획득 수량이 조절됨")
                    << BUILD_LOG(nAfterDuration)
                    << BUILD_LOG(vitGetLimitInfo->m_dwLimit)
                    << BUILD_LOG(nNewGetDuration)
                    << END_LOG;
            }
        }

        // 획득 리스트에서 제거. mitEachUserDrops->second 벡터가 비게 되는 경우는 나중에 추가로 제거처리 해준다
        EraseVectorAFromB(vecTempDropItems, mitEachUserDrops->second);

        // 처리 이후 드랍한 아이템이 없게되는 경우도 모았다가 한번에 지운다
        if (true == mitEachUserDrops->second.empty()) {
            mapTempEachUserDrops.insert(std::make_pair(mitEachUserDrops->first, mitEachUserDrops->second));
        }
    }

    // 획득 리스트에서 제거2
    EraseMapAFromB(mapTempEachUserDrops, mapEachUserDrops_);

    if (true == mapEachUserDrops_.empty()) {
        bItemDropped_ = false;
    }
}

void KUser::EraseVectorAFromB(IN DROP_ITEMS& A_, OUT DROP_ITEMS& B_)
{
    if (true == A_.empty()) {
        return;
    }

    DROP_ITEMS::iterator vitA;
    for (vitA = A_.begin(); vitA != A_.end(); ++vitA) {
        DROP_ITEMS::iterator vitB;
        vitB = std::find_if(B_.begin(), B_.end(),
            boost::bind(&KDropItemInfo::m_ItemID, _1) == vitA->m_ItemID);
        if (vitB != B_.end()) {
            B_.erase(vitB);
        }
    }
}

void KUser::EraseMapAFromB(IN std::map< DWORD, DROP_ITEMS >& A_, OUT std::map< DWORD, DROP_ITEMS >& B_)
{
    if (true == A_.empty()) {
        return;
    }

    std::map< DWORD, DROP_ITEMS >::iterator mitA;
    for (mitA = A_.begin(); mitA != A_.end(); ++mitA) {
        std::map< DWORD, DROP_ITEMS >::iterator mitB;
        mitB = B_.find(mitA->first);
        if (mitB != B_.end()) {
            B_.erase(mitB);
        }
    }
}

bool KUser::IsLINBenefitUser()
{
    return SiKIPAdvantage()->IsLINBenefitUser(m_cUserBenfitType);
}

bool KUser::CheckServerTypeEnable(IN const int& nGameMode_)
{
    if (SiKGSSimLayer()->CheckTypeFlag(ST_DUNGEON)) {
        if (!KRoom::IsDungeonMode(nGameMode_)) {
            return false;
        }
    }

    if (SiKGSSimLayer()->CheckTypeFlag(ST_PVP)) {
        if (!KRoom::IsPvPMode(nGameMode_)) {
            return false;
        }
    }
    return true;
}

void KUser::UpdateELOInfo(IN const int& nPlayType_, IN const KGameResultIn& vitResultIn_, IN int nGameMode_, IN bool bWin_, IN KUserPtr& spUser_, OUT KGameResultOut& kResultOut_)
{
    if ((nGameMode_ != GC_GM_AUTOMATCH_TEAM) || (SiKGSSimLayer()->CheckTypeFlag(ST_PVP) == false)) {
        return;
    }

    const KInDoorUserInfo* pkStartingInfo = m_pkRoom->FindInDoorUserInfo(spUser_->m_strName);
    if (pkStartingInfo) {
        KCharacterInfo* pkCharacterInfo = spUser_->GetCharacterInfo(pkStartingInfo->m_cCharIndex);
        if (pkCharacterInfo == NULL) {
            START_LOG(cerr, L"선택된 캐릭터 정보가 없음")
                << BUILD_LOGc(pkStartingInfo->m_cCharIndex)
                << END_LOG;
            return;
        }

        if (bWin_) {
            pkCharacterInfo->m_kELOUserData.m_nELOWin += 1;
        }
        else {
            pkCharacterInfo->m_kELOUserData.m_nELOLose += 1;
        }

        if (pkCharacterInfo->m_kELOUserData.m_nELOType == KELOUserData::MT_PLACEMENTTEST) {
            SiKPVPMatchManager()->CalcPlacementTestRatingPoint(m_pkRoom->m_prELOTeamRPAvg,
                pkStartingInfo->m_iTeam == 1,
                vitResultIn_.m_bWin,
                pkCharacterInfo->m_kELOUserData.m_nConstantK,
                pkCharacterInfo->m_kELOUserData.m_nMatchTotalCount,
                pkCharacterInfo->m_kELOUserData.m_nLastWinLose,
                pkCharacterInfo->m_kELOUserData.m_nRatingPoint);
        }
        else {
            SiKPVPMatchManager()->CalcRatingPoint(m_pkRoom->m_prELOTeamRPAvg,
                pkStartingInfo->m_iTeam == 1,
                vitResultIn_.m_bWin,
                pkCharacterInfo->m_kELOUserData.m_nRatingPoint);
        }
        pkCharacterInfo->m_kELOUserData.m_nMatchTotalCount += 1;
        if (pkCharacterInfo->m_kELOUserData.m_nELOType != KELOUserData::MT_PLACEMENTTEST) { // 배치고사가 아닐 경우만 등급계산.
            SiKPVPMatchManager()->CalcCharELOGrade(pkCharacterInfo->m_kELOUserData.m_nRatingPoint, pkCharacterInfo->m_kELOUserData.m_ucGrade);
        }
        SiKPVPMatchManager()->CalcELOType(pkCharacterInfo->m_kELOUserData.m_nMatchTotalCount, pkCharacterInfo->m_kELOUserData.m_nELOType);
        // 클라이언트 전달용 데이터 갱신.
        std::vector< std::pair<char, KCharacterExpInfoOut> >::iterator vit;
        for (vit = kResultOut_.m_vecExps.begin(); vit != kResultOut_.m_vecExps.end(); ++vit) {
            if (vit->first == pkCharacterInfo->m_cCharType) {
                vit->second.m_nELORatingPoint = pkCharacterInfo->m_kELOUserData.m_nRatingPoint;
                vit->second.m_nELOWin = pkCharacterInfo->m_kELOUserData.m_nELOWin;
                vit->second.m_nELOLose = pkCharacterInfo->m_kELOUserData.m_nELOLose;
                vit->second.m_ucGrade = pkCharacterInfo->m_kELOUserData.m_ucGrade;
                vit->second.m_nMatchTotalCount = pkCharacterInfo->m_kELOUserData.m_nMatchTotalCount;
                vit->second.m_nELOType = pkCharacterInfo->m_kELOUserData.m_nELOType;
            }
        }
        KELOUserData kELOUserData = pkCharacterInfo->m_kELOUserData;

        // DB에 ELO 계산된 정보 저장.
        KDB_EVENT_MATCH_CHAR_DATA_REQ kDBPacket;
        SetELOResultInfo(pkCharacterInfo->m_cCharType, kELOUserData, kDBPacket);
        _QUEUEING_EVENT_TO_DB(DB_EVENT_MATCH_CHAR_DATA_REQ, kDBPacket, spUser_);

        // 캐릭터 ELO 데이터 DB 초기값이랑 동기화 작업.
        CalcCharELOData(pkCharacterInfo->m_kELOUserData);

        // 대전 매칭 통계 작업.
        KDB_EVENT_MATCH_ELO_RESULT_REQ kDBStatPacket;
        DWORD dwDurationTime = spUser_->GetMatchDurationTime();
        spUser_->InitMatchTick();
        int nType = KDB_EVENT_MATCH_ELO_RESULT_REQ::ST_NORMAL;
        if (nPlayType_ == KRoom::PT_LEAVE_GAME) {
            nType = KDB_EVENT_MATCH_ELO_RESULT_REQ::ST_LEAVE;
        }
        SetELOResultStatInfo(nType, spUser_->m_dwUID, pkCharacterInfo->m_cCharType, pkCharacterInfo->m_dwLevel, m_pkRoom->m_prELOTeamRPAvg, pkStartingInfo->m_iTeam, kELOUserData, m_pkRoom->m_nModeInfo, bWin_, dwDurationTime, kDBStatPacket);
        spUser_->GetMatchSuccessLoopCount(kDBStatPacket.m_nLoopCount);
        kDBStatPacket.m_nTotalPartyNum = m_pkRoom->GetNormalUserNum(); // 현재 룸에 남아 있는 유저.
        _QUEUEING_EVENT_TO_DB(DB_EVENT_MATCH_ELO_RESULT_REQ, kDBStatPacket, spUser_);
    }
}

void KUser::GetMatchInviteUserList(OUT std::vector<KMatchInviteUserInfo>& vecMatchInviteList_)
{
    vecMatchInviteList_.clear();
    std::vector<DWORD>::iterator vit;

    for (vit = m_vecOnlineFriends.begin(); vit != m_vecOnlineFriends.end(); ++vit) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*vit);
        if (!spUser || !spUser->m_pkChannel) {
            continue;
        }

        //친구가 초대 가능한 곳에 있는가?
        if (spUser->GetStateID() != KUserFSM::STATE_CHANNEL &&
            spUser->GetStateID() != KUserFSM::STATE_CHANNELLOBBY) {
            continue;
        }

        KMatchInviteUserInfo kMatchInviteUserInfo;
        if (spUser->GetMatchInviteUserInfo(kMatchInviteUserInfo)) {
            vecMatchInviteList_.push_back(kMatchInviteUserInfo);
        }
    }
    m_vecMatchPartyInviteList = vecMatchInviteList_; // 유저객체에 알고 있는 정보 갱신.
}

bool KUser::GetMatchInviteUserInfo(OUT KMatchInviteUserInfo& kMatchInviteUserInfo_)
{
    if (m_bInviteDeny) { // 초대 거부상태일 경우의 처리..,
        return false;
    }

    kMatchInviteUserInfo_.m_dwUserUID = GetUID();
    kMatchInviteUserInfo_.m_strNickName = m_strNickName;
    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if (pkCharacter != NULL) {
        kMatchInviteUserInfo_.m_cCharType = pkCharacter->m_cCharType;
        kMatchInviteUserInfo_.m_strCharNickName = pkCharacter->m_strCharName;
    }

    return true;
}

bool KUser::IsJoinParty()
{
    if (m_kParty.m_dwPartyUID == 0) {
        return false;
    }
    return true;
}

bool KUser::SetSingleParty(IN const int& nModeInfo_, IN const char& cCharType_, IN const int& nExceptModeID_)
{
    KPartyData kPartyData;
    KPartyMember kPartyMaster;

    kPartyMaster.m_dwUID = GetUID();
    kPartyMaster.m_strNickName = GetNickName();
    kPartyMaster.m_IsHost = true;
    kPartyMaster.m_ServerUID = SiKGameServer()->GetServerID();
    kPartyMaster.m_cCharType = cCharType_;
    KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cCharType_);
    if (pkCharacterInfo) {
        kPartyMaster.m_nRP = pkCharacterInfo->m_kELOUserData.m_nRatingPoint;
    }
    else {
        START_LOG(cerr, L"존재하지 않는 캐릭터를 선택해서 상태변경 요청.")
            << BUILD_LOG(m_strName)
            << BUILD_LOG(nModeInfo_)
            << BUILD_LOGc(cCharType_)
            << END_LOG;
        return false;
    }

    kPartyData.m_vecPartyPeople.push_back(kPartyMaster); // 방장 넣기
    kPartyData.m_nModeInfo = nModeInfo_;
    kPartyData.m_dwPartyType = KPartyData::PT_MATCH_SOLO;
    kPartyData.m_dwServerUID = SiKGameServer()->GetServerID(); // 방장의 ServerUID를 넣어준다.
    kPartyData.m_dwPartyUID = GetUID(); // 매칭 솔로 파티일 경우 유저의 UID로 관리.
    kPartyData.m_dwMatchPartyUID = GetUID(); // 매칭 솔로 파티일 경우 유저의 UID로 관리.
    kPartyData.m_nExceptModeID = nExceptModeID_;
    if (!GetPartyRP(kPartyData.m_vecPartyPeople, kPartyData.m_nRP)) {
        START_LOG(cerr, L"파티 멤버가 없음.")
            << END_LOG;
    }
    m_kParty = kPartyData;

    return true;
}

bool KUser::GetPartyRP(IN const std::vector<KPartyMember>& vecPartyPeople_, OUT int& nRP_)
{
    if (vecPartyPeople_.empty()) {
        return false;
    }
    int nSize = vecPartyPeople_.size();
    int nTotalRP = 0;
    std::vector<KPartyMember>::const_iterator cvit;
    for (cvit = vecPartyPeople_.begin(); cvit != vecPartyPeople_.end(); ++cvit) {
        nTotalRP += cvit->m_nRP;
    }

    int nResult = static_cast<int>(nTotalRP / nSize);
    int nDefineRP = SiKPVPMatchManager()->GetDefineRP(nResult);
    // 규칙으로 정의된 테이블에서 파티의 RP가 변경.
    // 매칭 서버에서 RP값을 키로해서 map으로 컨테이너를 구분해서 관리하기 때문.
    nRP_ = nDefineRP;
    return true;
}

bool KUser::IsMatchSoloParty()
{
    if (m_kParty.m_dwPartyType == KPartyData::PT_MATCH_SOLO) {
        return true;
    }
    return false;
}

bool KUser::CheckPartyMemberState(IN const std::vector<KPartyMember>& vecPartyPeople_)
{
    std::vector<KPartyMember>::const_iterator cvit;
    for (cvit = vecPartyPeople_.begin(); cvit != vecPartyPeople_.end(); ++cvit) {
        if (cvit->m_dwUID == GetUID()) {
            continue;
        }
        if (cvit->m_dwState == KPartyMember::US_NORMAL) {
            return false;
        }
    }
    return true;
}

bool KUser::IsMatchParty()
{
    if (!IsJoinParty()) { // 파티에 가입되어 있는지 체크..,
        return false;
    }

    if (m_kParty.m_dwPartyType == KPartyData::PT_NORMAL) {
        return false;
    }
    return true;
}

void KUser::ChangeUserFindMatchState(IN const DWORD& dwUserUID_)
{
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_dwUID == dwUserUID_) {
                vit->m_dwState = KPartyMember::US_READY;
                return;
            }
        }
    }
}

bool KUser::CheckMatchPartyState(OUT DWORD& dwHostUID_)
{
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_IsHost) {
                dwHostUID_ = vit->m_dwUID;
            }

            if (vit->m_dwState != KPartyMember::US_READY) {
                return false;
            }
        }
    }
    return true;
}

void KUser::SetMatchRoomInfo(OUT KRoomInfo& kRoomInfo_)
{
    kRoomInfo_.m_usMaxUsers = 6;
    kRoomInfo_.m_cGameCategory = GC_GMC_AUTOMATCH;
    kRoomInfo_.m_iGameMode = GC_GM_AUTOMATCH_TEAM;
    kRoomInfo_.m_iP2PVersion = GetP2PVersion();
    kRoomInfo_.m_bPublic = true;
    kRoomInfo_.m_iSubGameMode = 1; // 노아이템전 게임 모드
}

void KUser::JoinRoomMatchPartyMember(IN USHORT& usRoomID_)
{
    JIF(m_pkRoom);
    KJoinRoomReqInfo kJoinRoomReqInfo;
    kJoinRoomReqInfo.m_usRoomID = usRoomID_;
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_dwUID == GetUID()) {
                continue;
            }
            kJoinRoomReqInfo.m_nTeamType = mit->first; // 팀정보 넣어주기. 초기값 -1은 선택 팀 사용하지 않음.
            KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vit->m_dwUID));
            if (!spUser) {
                m_pkRoom->IncreaseMatchFailCount(); // 실패한 인원수 Counting.
                continue;
            }
            spUser->GetMatchIndoorUserInfo(kJoinRoomReqInfo.m_kInDoorUserInfo); // 매칭 방입장 유저 정보 넣어주기.
            spUser->QueueingEvent(KUserEvent::EVENT_JOIN_ROOM_REQ, kJoinRoomReqInfo);
        }
    }
}

void KUser::CalcCharELOData(IN OUT KELOUserData& kELOUserData_)
{
    kELOUserData_.m_nInitELOWin = kELOUserData_.m_nELOWin;
    kELOUserData_.m_nInitELOLose = kELOUserData_.m_nELOLose;
    kELOUserData_.m_nInitMatchTotalCount = kELOUserData_.m_nMatchTotalCount;
    kELOUserData_.m_nInitRatingPoint = kELOUserData_.m_nRatingPoint;
}

void KUser::GetMatchMyTeamType(OUT int& nTeamType_)
{
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_dwUID == GetUID()) {
                nTeamType_ = mit->first;
                return;
            }
        }
    }
}

bool KUser::SetPartyMemberRP(IN OUT std::vector<KPartyMember>& vecPartyPeople_)
{
    if (vecPartyPeople_.empty()) {
        return false;
    }

    std::vector<KPartyMember>::iterator vit;
    for (vit = vecPartyPeople_.begin(); vit != vecPartyPeople_.end(); ++vit) {
        KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vit->m_dwUID));
        if (spUser == NULL) {
            START_LOG(cerr, L"해당 유저 객체 없음")
                << BUILD_LOG(vit->m_dwUID)
                << END_LOG;
            return false;
        }

        int nCharType = static_cast<int>(vit->m_cCharType);
        KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo(nCharType);
        if (pkCharacterInfo == NULL) {
            START_LOG(cerr, L"선택된 캐릭터 정보가 없음")
                << BUILD_LOG(nCharType)
                << END_LOG;
            return false;
        }

        vit->m_nRP = pkCharacterInfo->m_kELOUserData.m_nRatingPoint;
    }
    return true;
}

void KUser::SetMatchStartTick()
{
    m_dwMatchStartTick = ::timeGetTime();
}

void KUser::SetMatchEndTick()
{
    m_dwMatchEndTick = ::timeGetTime();
}

void KUser::InitMatchTick()
{
    m_dwMatchStartTick = ::timeGetTime();
    m_dwMatchEndTick = ::timeGetTime();
}

DWORD KUser::GetMatchDurationTime()
{
    DWORD dwDurationTime = (m_dwMatchEndTick - m_dwMatchStartTick) / 1000;
    return dwDurationTime;
}

void KUser::CheckUserDelMatchTime(IN DWORD& dwDurationTime_, IN int& nModeInfo_)
{
    KDB_EVENT_DEL_MATCH_REQ kDBPacket;
    DWORD dwMinDelMatchTime = SiKPVPMatchManager()->GetMinDelMatchTime();
    if (dwDurationTime_ < dwMinDelMatchTime) {
        return;
    }

    kDBPacket.m_nDelType = KDB_EVENT_DEL_MATCH_REQ::DT_CANCLE;
    SiKPVPMatchManager()->GetSeasonInfo(kDBPacket.m_nSeasonYear, kDBPacket.m_nSeasonVersion);
    kDBPacket.m_dwUserUID = GetUID();
    kDBPacket.m_nModeInfo = nModeInfo_;
    kDBPacket.m_dwDurationTime = dwDurationTime_;
    GetMatchPartyCharInfo(kDBPacket.m_cCharType, kDBPacket.m_nRP);

    QUEUEING_EVENT_TO_DB(DB_EVENT_DEL_MATCH_REQ, kDBPacket);
}

void KUser::SendUserMatchRefuseInfo(IN DWORD& dwDurationTime_, IN int& nModeInfo_)
{
    KDB_EVENT_DEL_MATCH_REQ kDBPacket;

    kDBPacket.m_nDelType = KDB_EVENT_DEL_MATCH_REQ::DT_REFUSE;
    SiKPVPMatchManager()->GetSeasonInfo(kDBPacket.m_nSeasonYear, kDBPacket.m_nSeasonVersion);
    kDBPacket.m_dwUserUID = GetUID();
    kDBPacket.m_nModeInfo = nModeInfo_;
    kDBPacket.m_dwDurationTime = dwDurationTime_;
    GetMatchPartyCharInfo(kDBPacket.m_cCharType, kDBPacket.m_nRP);

    QUEUEING_EVENT_TO_DB(DB_EVENT_DEL_MATCH_REQ, kDBPacket);
}

void KUser::SetELOResultInfo(IN const char& cCharType_, IN KELOUserData& kELOUserData_, OUT KDB_EVENT_MATCH_CHAR_DATA_REQ& kDBPacket_)
{
    kDBPacket_.m_cCharType = cCharType_;
    SiKPVPMatchManager()->GetSeasonInfo(kDBPacket_.m_nSeasonYear, kDBPacket_.m_nSeasonVersion);
    kDBPacket_.m_kELOUserData = kELOUserData_;
}

void KUser::SetELOResultStatInfo(IN const int& nType_, IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const DWORD& dwLevel_, IN const std::pair<int, int> prELOTeamRPAvg_, IN const int& nTeam_, IN const KELOUserData& kELOUserData_, IN const int& nModeInfo_, IN const bool& bWin_, IN const DWORD& dwMatchWaitTime_, OUT KDB_EVENT_MATCH_ELO_RESULT_REQ& kDBPacket_)
{
    kDBPacket_.m_dwUserUID = dwUserUID_;
    SiKPVPMatchManager()->GetSeasonInfo(kDBPacket_.m_nSeasonYear, kDBPacket_.m_nSeasonVersion);
    kDBPacket_.m_cCharType = cCharType_;
    kDBPacket_.m_dwLevel = dwLevel_;
    kDBPacket_.m_nReceiveRP = kELOUserData_.m_nRatingPoint - kELOUserData_.m_nInitRatingPoint;
    kDBPacket_.m_nTotalRP = kELOUserData_.m_nRatingPoint;
    kDBPacket_.m_nModeInfo = nModeInfo_;
    if (bWin_) {
        kDBPacket_.m_nWinLose = 1;
    }
    else {
        kDBPacket_.m_nWinLose = 0;
    }

    if (KDB_EVENT_MATCH_ELO_RESULT_REQ::ST_LEAVE == nType_) { // 게임중 나갔을 경우 WinLose 타입 2로 설정.
        kDBPacket_.m_nWinLose = 2;
    }
    kDBPacket_.m_dwMatchWaitTime = dwMatchWaitTime_;

    if (nTeam_ == 0) {
        kDBPacket_.m_nMyTeamRP = prELOTeamRPAvg_.second;
        kDBPacket_.m_nOtherTeamRP = prELOTeamRPAvg_.first;
    }
    else {
        kDBPacket_.m_nMyTeamRP = prELOTeamRPAvg_.first;
        kDBPacket_.m_nOtherTeamRP = prELOTeamRPAvg_.second;
    }
}

void KUser::SetMatchIndoorUserInfo(IN KInDoorUserInfo& kMatchIndoorUserInfo_)
{
    m_kMatchIndoorUserInfo = kMatchIndoorUserInfo_;
}

void KUser::GetMatchIndoorUserInfo(OUT KInDoorUserInfo& kMatchIndoorUserInfo_)
{
    kMatchIndoorUserInfo_ = m_kMatchIndoorUserInfo;
}

void KUser::SendToStartFailMatchMemberNot()
{
    std::map<int, std::vector<KPartyMember>> mapMatchParty;
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    mapMatchParty = m_mapMatchParty;
    for (mit = mapMatchParty.begin(); mit != mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_dwUID == GetUID()) {
                continue;
            }
            KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vit->m_dwUID));
            if (!spUser) {
                continue;
            }
            spUser->QueueingEvent(KUserEvent::EVENT_START_GAME_FAIL_NOT, spUser->m_dwUID);
        }
    }
}

bool KUser::CheckPartyHost(IN const DWORD& dwUserUID_)
{
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_IsHost) {
                if (dwUserUID_ == vit->m_dwUID) {
                    return true;
                }
            }
        }
    }
    return false;
}

void KUser::NofityMatchServer()
{
    if (IsMatchParty()) { // 매칭 파티이고..,
        KPartyData kPartyData;
        int nMatchSID = 0;
        SiKMatchConnector()->GetFirstSID(nMatchSID);
        kPartyData = m_kParty;

        if (IsMatchSoloParty()) { // 매칭 솔로 파티이고..,
            SEND_DATA_TO_MATCHSERVER(nMatchSID, EVENT_MATCH_DEL_USER_REQ, kPartyData);
            return;
        }
        else {
            SEND_DATA_TO_MATCHSERVER(nMatchSID, EVENT_MATCH_DEL_USER_REQ, kPartyData);
            return;
        }
    }
}

void KUser::SetMatchRoomEnable(IN bool bMatchCreateRoom_)
{
    m_kParty.m_bMatchCreateRoom = bMatchCreateRoom_;
}

bool KUser::IsMatchRoomEnable()
{
    if (m_kParty.m_bMatchCreateRoom) {
        return false;
    }
    return true;
}

void KUser::GetMatchPartyCharInfo(OUT char& cCharType_, OUT int& nRP_)
{
    cCharType_ = -1;
    nRP_ = 0;
    std::vector<KPartyMember>::const_iterator cvit;
    for (cvit = m_kParty.m_vecPartyPeople.begin(); cvit != m_kParty.m_vecPartyPeople.end(); ++cvit) {
        if (cvit->m_dwUID == GetUID()) {
            cCharType_ = cvit->m_cCharType;
            nRP_ = cvit->m_nRP;
        }
    }
}

void KUser::GetMatchSuccessLoopCount(OUT int& nLoopCount_)
{
    nLoopCount_ = m_nMatchSuccessLoopCount;
    m_nMatchSuccessLoopCount = 0;
}

void KUser::CreateRoomFailMatchPartyMember()
{
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for (mit = m_mapMatchParty.begin(); mit != m_mapMatchParty.end(); ++mit) {
        for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            if (vit->m_dwUID == GetUID()) {
                continue;
            }
            KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vit->m_dwUID));
            if (!spUser) {
                continue;
            }
            spUser->QueueingEvent(KUserEvent::EVENT_START_GAME_FAIL_NOT, spUser->m_dwUID);
        }
    }
}

void KUser::SendMatchTimeTable()
{
    if (SiKGSSimLayer()->CheckTypeFlag(ST_PVP) == false) {
        return;
    }

    KEVENT_MATCH_TIME_TABLE_NOT kPacket;
    SiKPVPMatchManager()->GetMatchTimeTable(kPacket);
    SEND_PACKET(EVENT_MATCH_TIME_TABLE_NOT);
}

void KUser::LoadRitasChristmasEventInfo()
{
    if (true == SiKRitasChristmas()->IsEventTerm()) {
        QUEUEING_ID_TO_DB(EVENT_RITAS_CHRISTMAS_USER_INFO_REQ);
    }
}

bool KUser::GetRitasChristmasReward(IN Kairo& kKairo_, OUT KDropItemInfo& kAddItem_)
{
    if (false == kKairo_.Do(kAddItem_)) {
        return false;
    }

    if (0 == kAddItem_.m_ItemID) {
        START_LOG(cerr, L"리타의 크리스마스 이벤트 보상 아이템 가져왔는데 꽝이 나옴") << END_LOG;
        return false;
    }

    return true;
}

bool KUser::CheckRitasChristmasBasketSize(IN int nGetSize_)
{
    KRitasChristmasPlayInfo kPlayInfo;

    SiKRitasChristmas()->GetPlayInfo(kPlayInfo);

    if (kPlayInfo.m_nBasketSize < nGetSize_) {
        return false;
    }

    return true;
}

bool KUser::CalcRitasChristmasPoint(IN std::vector< int >& vecGetObjectID_, OUT int& nGetPoint_)
{
    nGetPoint_ = 0;

    std::map< int, KRitasChristmasObjectInfo > mapObjectInfo;

    SiKRitasChristmas()->GetObjectInfo(mapObjectInfo);

    if (true == mapObjectInfo.empty()) {
        START_LOG(cerr, L"포인트 계산 중 오브젝트 정보가 비어있음") << END_LOG;
        return false;
    }

    std::vector< int >::iterator vitGetObjectID;
    for (vitGetObjectID = vecGetObjectID_.begin(); vitGetObjectID != vecGetObjectID_.end(); ++vitGetObjectID) {
        nGetPoint_ += GetRitasChristmasObjectPoint(*vitGetObjectID, mapObjectInfo);
    }

    // 바구니 비우기 점수 제한 체크
    nGetPoint_ = std::min< int >(SiKRitasChristmas()->GetBasketEmptyPointLimit(), nGetPoint_);

    return true;
}

int KUser::GetRitasChristmasObjectPoint(IN int nObjectID_, IN std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_)
{
    std::map< int, KRitasChristmasObjectInfo >::iterator mitObjectInfo;
    mitObjectInfo = mapObjectInfo_.find(nObjectID_);
    if (mitObjectInfo == mapObjectInfo_.end()) {
        START_LOG(cerr, L"포인트 계산 중 요청한 오브젝트 정보가 없음")
            << BUILD_LOG(nObjectID_)
            << END_LOG;
        // 오브젝트 정보가 없으면 0점 처리한다
        return 0;
    }

    return mitObjectInfo->second.m_nPoint;
}

void KUser::CalcRitasChristmasFeverBonusPoint(IN int nGetFeverObjectCount_, OUT int& nGetBonusPoint_, OUT int& nGetPremiumRewardCount_)
{
    std::map< int, KRitasChristmasObjectInfo > mapObjectInfo;

    SiKRitasChristmas()->GetObjectInfo(mapObjectInfo);

    if (true == mapObjectInfo.empty()) {
        START_LOG(cerr, L"보너스 포인트 계산 중 오브젝트 정보가 비어있음") << END_LOG;
        return;
    }

    std::map< int, KRitasChristmasObjectInfo >::iterator mitObjectInfo;
    mitObjectInfo = std::find_if(mapObjectInfo.begin(), mapObjectInfo.end(),
        boost::bind(&KRitasChristmasObjectInfo::m_nObjectType,
            boost::bind(&std::map< int, KRitasChristmasObjectInfo >::value_type::second, _1)) == KRitasChristmasObjectInfo::OT_FEVER);

    if (mitObjectInfo != mapObjectInfo.end()) {
        // 보너스 점수 계산
        nGetBonusPoint_ += mitObjectInfo->second.m_nPoint * nGetFeverObjectCount_;

        // 피버 점수 제한 체크
        nGetBonusPoint_ = std::min< int >(SiKRitasChristmas()->GetFeverPointLimit(), nGetBonusPoint_);

        // 프리미엄 상자 획득 수 계산
        nGetPremiumRewardCount_ = nGetFeverObjectCount_ / SiKRitasChristmas()->GetPremiumPerFeverObjectCount();
    }
}

bool KUser::CheckDayPassed(IN time_t tmLast_)
{
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmLast = CTime(tmLast_);

    if (tmCurrent.GetYear() > tmLast.GetYear()
        || tmCurrent.GetMonth() > tmLast.GetMonth()
        || tmCurrent.GetDay() > tmLast.GetDay()) {
        return true;
    }

    return false;
}

void KUser::CheckRitasChristmasStartCoinSupply()
{
    if (false == SiKRitasChristmas()->IsEventTerm()) {
        return;
    }

    time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime());
    int nCoinSupplyInterval = SiKRitasChristmas()->GetCoinSupplyInterval();
    int nCoinSupplyCount = SiKRitasChristmas()->GetCoinSupplyCount();
    int nCoinMaxQuantity = SiKRitasChristmas()->GetCoinMaxQuantity();
    GCITEMID dwCoinDoubleGainItemID = SiKRitasChristmas()->GetCoinDoubleGainItemID();
    bool bStartCoinReceived = false;

    // ( 현재 시간 - 마지막으로 시작 코인 받은 시간 ) > 시작 코인이 충전되는 시간 간격 확인
    if (tmCurrent - m_kRitasChristmasUserData.m_tmStartCoinReceived > nCoinSupplyInterval) {
        KItemPtr pkItem = m_kInventory.FindItemByItemID(dwCoinDoubleGainItemID);
        if (NULL != pkItem) {
            // 코인 부스터를 가지고 있을 경우 두배 지급함
            nCoinSupplyCount *= 2;
        }

        // 최대 시작 코인 충전 수 보정
        m_kRitasChristmasUserData.m_nStartCoinCount = std::min< int >(nCoinMaxQuantity, m_kRitasChristmasUserData.m_nStartCoinCount + nCoinSupplyCount);

        // 마지막으로 시작 코인을 받은 시간 갱신
        /* 시작 코인 보유가 최대치여서 실제로 지급받지 못하더라도 여기에서 시간을 다시 갱신하기 때문에
           접속시간 기준 nCoinSupplyInterval 마다 충전이 이루어진다 */
        m_kRitasChristmasUserData.m_tmStartCoinReceived = tmCurrent;

        bStartCoinReceived = true;
    }

    START_LOG(clog, L"리타의 크리스마스 이벤트 시작 코인 충전 확인. Name : " << m_strName)
        << BUILD_LOG(GetUID())
        << BUILD_LOG(bStartCoinReceived)
        << BUILD_LOG(nCoinSupplyInterval)
        << BUILD_LOG(nCoinSupplyCount)
        << BUILD_LOG(nCoinMaxQuantity)
        << BUILD_LOG(m_kRitasChristmasUserData.m_nStartCoinCount)
        << BUILD_LOG(tmCurrent)
        << BUILD_LOG(m_kRitasChristmasUserData.m_tmStartCoinReceived)
        << BUILD_LOGtm(CTime(tmCurrent))
        << BUILD_LOGtm(CTime(m_kRitasChristmasUserData.m_tmStartCoinReceived))
        << END_LOG;

}

bool KUser::CheckDuplicateItemUID(IN std::vector< GCITEMUID >& vecItemUID_)
{
    if (vecItemUID_.empty()) {
        START_LOG(cerr, L"제작 재료 아이템의 UID확인 중 재료 아이템이 비어 있음. Name : " << m_strName)
            << END_LOG;
        return false;
    }

    std::set< GCITEMUID > setTempUID;
    setTempUID.clear();

    std::vector< GCITEMUID >::iterator vitUID = vecItemUID_.begin();
    for (; vitUID != vecItemUID_.end(); ++vitUID) {
        setTempUID.insert(*vitUID);
    }

    return (vecItemUID_.size() == setTempUID.size());
}

void KUser::GetMyMatchRankInfo()
{
    std::set<char> kPacket;
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for (cmit = m_mapCharacterInfo.begin(); cmit != m_mapCharacterInfo.end(); ++cmit) {
        kPacket.insert(cmit->first);
    }

    QUEUEING_EVENT_TO_DB(DB_EVENT_MY_MATCH_RANK_INFO_REQ, kPacket);
}

void KUser::SendRankRewardInfo()
{
    KEVENT_MATCH_RANK_REWARD_NOT kPacket;
    SiKRankManager()->GetRankRewardInfo(kPacket);
    SEND_PACKET(EVENT_MATCH_RANK_REWARD_NOT);
}

bool KUser::GetGachaUseVersionsInfo( OUT std::vector< KGachaNewList >& vecGachaUseVersions_ )
{
    SiKGachaManager()->GetGachaUseVersions(vecGachaUseVersions_);

    return true;
}

bool KUser::CheckStrengthActionAssistType(IN const DWORD dwAssistType_, IN const GCITEMUID dwAssistItemID_)
{
    switch (dwAssistType_) {
    case KStrengthManager::AT_NOUSE:
        if (0 != dwAssistItemID_) {
            return false;
        }

        break;
    case KStrengthManager::AT_UPGRADE:
    case KStrengthManager::AT_NOBREAK:
    case KStrengthManager::AT_SPECIFIC:
    case KStrengthManager::AT_RANDOM:
    case KStrengthManager::AT_UPGRADE_NOBREAK:
        if (0 == dwAssistItemID_) {
            return false;
        }

        break;
    default:
        return false;

        break;
    }

    return true;
}

void KUser::CheckEscortDrop(IN bool bIsEscortMonsterAlive_, IN DWORD dwEscortMonID_, IN int nGameMode_, IN int nDifficulty_, IN DWORD dwLevel_, OUT std::vector< KDropItemInfo >& vecdrops_)
{
    // 이벤트 기간중에 던전을 시작하여 호위 몬스터가 클라에서 표시되었지만 보상을 받지 못하는 경우를
    // 방지하기 위해서 여기에서 호위 이벤트 기간을 확인하지 않는다

    // 만약 문제가 생긴다면 이 곳 결과처리에서도 이벤트 기간을 확인하도록 한다

    /*if ( false == SiKEscort()->IsRun() ) {
        return;
    }*/

    PAIR_DWORD prKey = std::make_pair(nGameMode_, nDifficulty_);
    if (false == SiKEscort()->IsValidUserLevel(prKey, dwLevel_)) {
        return;
    }

    if (false == SiKEscort()->IsEscortMonster(dwEscortMonID_)) {
        return;
    }

    if (false == SiKEscort()->IsHaveRewardInfo(nGameMode_, nDifficulty_, dwEscortMonID_)) {
        return;
    }

    if (true == bIsEscortMonsterAlive_) {  // true면 살아있음
        // 살아있는 보상 아이템 지급
        KEscort::KEscortReward kAliveReward;
        if (false == SiKEscort()->GetAliveReward(dwEscortMonID_, kAliveReward)) {
            return;
        }

        KDropItemInfo kDropItemInfo;
        std::vector< Kairo >::iterator vit;
        for (vit = kAliveReward.m_vecKairoClearReward.begin(); vit != kAliveReward.m_vecKairoClearReward.end(); ++vit) {
            if (false == vit->Do(kDropItemInfo)) {
                continue;
            }
            if (kDropItemInfo.m_ItemID == 0) {
                continue;
            }
            vecdrops_.push_back(kDropItemInfo);
        }
        for (vit = kAliveReward.m_vecKairoExtraReward.begin(); vit != kAliveReward.m_vecKairoExtraReward.end(); ++vit) {
            if (false == vit->Do(kDropItemInfo)) {
                continue;
            }
            if (kDropItemInfo.m_ItemID == 0) {
                continue;
            }
            vecdrops_.push_back(kDropItemInfo);
        }
    }
    else {
        // 호위 몹 죽었을 경우 보상 아이템 지급
        KEscort::KEscortReward kDeadReward;
        if (false == SiKEscort()->GetDeadReward(dwEscortMonID_, kDeadReward)) {
            return;
        }

        KDropItemInfo kDropItemInfo;
        std::vector< Kairo >::iterator vit;
        for (vit = kDeadReward.m_vecKairoClearReward.begin(); vit != kDeadReward.m_vecKairoClearReward.end(); ++vit) {
            if (false == vit->Do(kDropItemInfo)) {
                continue;
            }
            if (kDropItemInfo.m_ItemID == 0) {
                continue;
            }
            vecdrops_.push_back(kDropItemInfo);
        }
        for (vit = kDeadReward.m_vecKairoExtraReward.begin(); vit != kDeadReward.m_vecKairoExtraReward.end(); ++vit) {
            if (false == vit->Do(kDropItemInfo)) {
                continue;
            }
            if (kDropItemInfo.m_ItemID == 0) {
                continue;
            }
            vecdrops_.push_back(kDropItemInfo);
        }
    }
}

void KUser::SendEventInfo(IN KEVENT_VERIFY_ACCOUNT_ACK& kVerifyPacket_, IN std::map<int, std::set<std::pair<int, int> > >& mapUserSurveyInfo_, IN int& nMissionEventCheck_, IN std::set<DWORD>& setStringIDList_)
{
    if (SiKSurvey()->IsRun()) { // 이벤트 기간일때만 전달.
        // 설문 보상 정보 전달.
        KEVENT_SURVEY_REWARD_LIST_INFO kPacket;
        SiKSurvey()->GetRewardList(kPacket.m_mapSurveyReward);
        SEND_PACKET(EVENT_SURVEY_REWARD_LIST_INFO);

        m_kSurveyUserData.SetUserSurveyData(mapUserSurveyInfo_);
        DWORD dwChannelType = GetUserChannelType();
        CheckGameConnectSurvey(dwChannelType, kVerifyPacket_.m_tFirstLoginTime, kVerifyPacket_.m_tLastLoginTime, kVerifyPacket_.m_kSurveyList); // 게임접속시 전달할 설문있는지 확인.
    }
    // 매일 다른 미션 지급 이벤트 진행.
    // 이벤트가 진행중이고 아이템을 받지 않았을 경우만 아이템, 미션 작업 처리해 주러 감.
    if (SiKMissionEvent()->IsMissionEventTerm() && nMissionEventCheck_ != 0) {
        CheckMissionEvent();
    }
    else {
        KEVENT_MISSION_DATE_CHANGE_ACK kPacket;
        kPacket.m_nOK = 1; // 이미 미션이 지급된 유저. ERR_MISSION_EVENT_00
        SEND_PACKET(EVENT_MISSION_DATE_CHANGE_ACK);
    }

    // 이벤트 내용 출력 여부 확인.
    if (!setStringIDList_.empty()) {
        KEVENT_STRING_INFO_NOT kStringList;
        kStringList.swap(setStringIDList_);
        SEND_COMPRESS_PACKET(EVENT_STRING_INFO_NOT, kStringList);
    }

    if (SiKChannelTypeEvent()->IsEventTerm()) {
        // 채널링 이벤트 체크.
        KDB_CHANNELING_EVENT_CHECK kDBPacket;
        kDBPacket.m_dwUserChannelType = GetUserChannelType();
        kDBPacket.m_tmFirstLoginTime = kVerifyPacket_.m_tFirstLoginTime;
        QUEUEING_EVENT_TO_DB(DB_CHANNELING_EVENT_CHECK, kDBPacket);
    }
    LoadRitasChristmasEventInfo();  // 리타의 크리스마스 이벤트 유저정보 가져와서 캐싱하고 클라에도 전달

}

void KUser::SendUserDetailInfo(const char cLastPlayCharacter_ /* = -99*/)
{
    {
        // 인벤토리 차지 안하는 아이템 리스트 전달.
        KEVENT_NONE_INVEN_ITEM_LIST_NOT kPacket;
        SiKItemManager()->m_kItemConfig.GetNonInvenSizeItemList(kPacket.m_setNonInvenSizeItem);
        SiKItemManager()->m_kItemConfig.GetHideInvenItemList(kPacket.m_setHideInvenItem);
        kPacket.m_MaximumPCBangItemUID = SiKIPAdvantage()->GetMaximumPCBangItemUID();
        SEND_COMPRESS_PACKET(EVENT_NONE_INVEN_ITEM_LIST_NOT, kPacket);
    }

    {
        // 강화석 정보 전달.
        KEVENT_STRENGTH_MATERIAL_INFO kPacket;
        SiKStrengthManager()->GetStrengthInfo(kPacket);
        SEND_PACKET(EVENT_STRENGTH_MATERIAL_INFO);
    }

    //SendUserGCPointInfo(); // 출석부 정보 전달.
    GetMyRankInfo();
    SendRankRewardInfo();
    GetMyMatchRankInfo();
    if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO)) {
        SendIndigoRank();
    }
    UpdateGachaKeyBuyCount(0, 0);
    CheckGachaReelPointReq(); // 가챠 릴 시스템

    SendHeroDungeonList();
    // VIP이벤트 유저보상지급 요청
    SendVipEventUserRewardUpdateNot();
    QUEUEING_ID_TO_DB(DB_EVENT_SELECT_GACHA_NOTICE_POPUP_INFO);

    {
        KEVENT_USER_CHANGE_WEAPON_NOT kPacket;
        kPacket = m_mapChangeWeapon;
        SEND_PACKET(EVENT_USER_CHANGE_WEAPON_NOT);
    }

    {
        KEVENT_NEW_CHAR_CARD_INFO_NOT kPacket;
        SiKGCHelper()->GetCharacterCardInfo(kPacket);
        SEND_PACKET(EVENT_NEW_CHAR_CARD_INFO_NOT);
    }

    {
        int kPacket;
        SiKVirtualCash()->GetLimitRatio(kPacket);
        SEND_PACKET(EVENT_VIRTUAL_CASH_LIMIT_RATIO_NOT);
    }

    QUEUEING_ID_TO_DB(EVENT_CURRENT_VIRTUAL_CASH_NOT); // 가상 캐쉬 정보 얻기

    QUEUEING_ID_TO_DB(EVENT_GET_NASTY_INFO_REQ); // 해킹 정보 가져 오기..
    QUEUEING_ID_TO_DB(EVENT_GET_USER_HACKING_COUNT_REQ); // 누적 해킹횟수 요청(위에꺼랑 다르다)
    QUEUEING_ID_TO_DB(EVENT_CONNECTION_MSG_REQ); // 접속 메세지 요청
    SEND_DATA(EVENT_BAD_USER_INFO_NOT, m_kBadUserInfo); // 불량 유저 정보.
    SEND_DATA(EVENT_COLLECTION_MISSION_NOT, m_vecCollectionSlot);
    SendHellTicketFreeModeNot(); // 헬모드 티켓 체크안하는 게임모드 전달
    CheckBonusRefill();
    QUEUEING_ID_TO_DB(EVENT_FULL_COUPLE_INFO_REQ);
    QUEUEING_ID_TO_DB(EVENT_CHECK_LUNA_NEWYEAR_REQ);
    SendVIPItemListNot(); // 로얄VIP뱃지 ItemList 전달.
    SendCapsuleListNot(); // 캡슐 정보 전달
    SendMissionPackListNot(); // 미션 패키지 리스트
    FirstNotEvents();
    SendItemTradeListNot(); // 아이템 교환정보 리스트 전달.
    SendNpcGiftsNot();
    QUEUEING_ID_TO_DB(EVENT_MY_SUBSCRIPTION_INFO_REQ);
    // 추천한 유저가 Lv을 달성했는지 확인.
    QUEUEING_ID_TO_DB(DB_EVENT_RECOMMEND_LEVELUP_CHECK);
    // 유저의 TimeDrop Info 받아오기
    QUEUEING_ID_TO_DB(DB_EVENT_GET_TIME_DROP_INFO_REQ);
    {
        // 전직 마법서 시스템
        // 아이템에 따른 캐릭터 타입별 최소 레벨 정보 전달 
        KEVENT_ITEM_CHARPROMOTION_LEVEL_NOT kPacket;
        SiKCharPromotionSystem()->GetItemCharPromotionMinLevel(kPacket);
        SEND_PACKET(EVENT_ITEM_CHARPROMOTION_LEVEL_NOT);
    }

    {
        // GP 속성 초기화 아이템 리스트 전달.
        MAP_GCITEMID_SET kPacket;
        SiKItemManager()->m_kAttributeDecider.GetInitGPAttributeItemList(kPacket);
        SEND_PACKET(EVENT_GP_ATTRIBUTE_INIT_ITEM_LIST);
    }

    {   // GP 속성 랜덤으로 고르는 소모 아이템 리스트
        MAP_GCITEMID_SET kPacket;
        SiKItemManager()->m_kAttributeDecider.GetRandomGPAttributeItemList(kPacket);
        SEND_PACKET(EVENT_GP_ATTRIBUTE_RANDOM_ITEM_LIST);
    }

    {
        // 랜덤 속성 아이템 리스트 전달.
        MAP_GCITEMID_SET kPacket;
        SiKItemManager()->m_kAttributeDecider.GetInitAttributeItemList(kPacket);
        SEND_PACKET(EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_LIST);
    }

    {
        // 개별 랜덤 속성 주문서 아이템 리스트 전달.
        MAP_GCITEMID_SET kPacket;
        SiKItemManager()->m_kAttributeDecider.GetSingleRandomAttributeItemList(kPacket);
        SEND_PACKET(EVENT_SINGLE_RANDOM_ATTRIBUTE_ITEM_LIST_NOT);
    }

    // 일일 팝업창 실행여부
    QUEUEING_ID_TO_DB(EVENT_TODAYS_POPUP_INFO_REQ);
    {
        // 스킬 레벨 전달.
        KEVENT_MAX_CHAR_SP_LEVEL_NOT kPacket;
        SiKSkillManager()->GetMaxCharSPLevel(kPacket);
        SEND_PACKET(EVENT_MAX_CHAR_SP_LEVEL_NOT);
    }

    {
        // 길드 포인트, 레벨 테이블 전달
        KEVENT_GUILD_LEVEL_TABLE_NOT kPacket;
        SiKGuildManager()->GetGuildPointLevel(kPacket);
        SEND_PACKET(EVENT_GUILD_LEVEL_TABLE_NOT);
    }

    if (SiKPostConfig()->IsPostEnable() && cLastPlayCharacter_ != -1) {
        QUEUEING_ID_TO_DB(DB_EVENT_INIT_POST_LETTER_LIST_REQ);
        START_LOG(clog, L"유저 우편함 정보 요청. Login : " << GetName())
            << END_LOG;
    }

    LoadErrandEventInfo(); // 심부름왕 이벤트 유저 정보 가져 와서 캐싱

    if (true == SiKTongDonation()->IsEventTerm()) {
        QUEUEING_ID_TO_DB(DB_EVENT_TONG_DONATION_USER_AMOUNT_INFO_REQ);
    }

    // 일반/에픽 미션 완료 한 목록 얻어서 클라에 전달
    QUEUEING_ID_TO_DB(DB_EVENT_FINISHED_MISSION_LIST_REQ);

    {
        // 미션 등록에 필요한 조건 정보 클라에 전달
        KEVENT_MISSION_GETTABLE_CONDITION_INFO_NOT kPacket;
        SiKMissionGettableCondition()->GetMissionGettableCondition(kPacket);
        SEND_PACKET(EVENT_MISSION_GETTABLE_CONDITION_INFO_NOT);
    }
}

void KUser::NewUserSHDropCheck()
{
    if (m_tmFirstLoginTime > SiKGSSimLayer()->GetSHAdhustTime()) {
        //상하이 드랍. 0단계에 주는것 있는지 확인한다.
        KEVENT_SHANGHAI_DROP_REQ shReq;

        START_LOG(clog, L"New User SHANGHAI_DROPCHECK") << END_LOG;
        if (SiKSHCheckPoint()->NewUserDropCheck(shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, m_strName, KUser::USER_CT_TOTAL)) {
            START_LOG(clog, L"New User SHANGHAI_DROP(Total) size : ")
                << BUILD_LOG(shReq.m_vecDropItems.size())
                << BUILD_LOG(shReq.m_vecDropPostItems.size())
                << BUILD_LOG(shReq.m_vecAutoMission.size()) << END_LOG;
        }

        if (SiKSHCheckPoint()->NewUserDropCheck(shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, m_strName, GetUserChannelType())) {
            START_LOG(clog, L"New User SHANGHAI_DROP(Channelling) size : ")
                << BUILD_LOG(shReq.m_vecDropItems.size())
                << BUILD_LOG(shReq.m_vecDropPostItems.size())
                << BUILD_LOG(shReq.m_vecAutoMission.size()) << END_LOG;
        }
        shReq.m_cCharType = GetCurrentCharType();
        CheckLevelUpDrop(shReq);
    }
}

void KUser::SendUserGCPointInfo()
{
    if (SiKGCPoint()->IsGCPointSwitch() == true) { // 쥐씨 포인트 시스템 사용한다면 
        m_kCalendar.SetOwnerLogin(m_strName);
        std::vector<KItem> vecWeeklyReward;
        std::vector<KItem> vecMonthlyReward;

        SiKGSSimLayer()->GetWeeklySelectRewardList(vecWeeklyReward);//주개근보상리스트 갱신
        SEND_COMPRESS_PACKET(EVENT_WEEKLY_REWARD_LIST_NOT, vecWeeklyReward);

        SiKGSSimLayer()->GetMonthlySelectRewardList(vecMonthlyReward);//월개근보상리스트 갱신
        SEND_COMPRESS_PACKET(EVENT_MONTHLY_REWARD_LIST_NOT, vecMonthlyReward);

        QUEUEING_ID_TO_DB(EVENT_LOAD_POINTSYSTEM_INFO_REQ); // 달력 정보 가져 오기.
        QUEUEING_ID_TO_DB(EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT); // 달개근시 예약된 보상기회.
    }
}

void KUser::SendUserAgitInfo()
{
    if (SiKGSSimLayer()->IsAgitEnable()) {
        QUEUEING_ID_TO_DB(EVENT_TODAYS_SEED_DROP_COUNT_REQ);
        START_LOG(clog, L"일일 획득한 씨앗 개수 요청. AgitUID : " << GetUID() << L", LoginID : " << GetName()) << END_LOG;

        SendUserConnectNoticeToAgentMaster();

        QUEUEING_ID_TO_DB(EVENT_FAIRY_TREE_BUFF_REQ);
        START_LOG(clog, L"열매버프 확인 요청. AgitUID : " << GetUID() << L", LoginID : " << GetName()) << END_LOG;

        QUEUEING_ID_TO_DB(EVENT_AGIT_USER_OPTION_REQ);
        START_LOG(clog, L"아지트 초대설정 확인 요청. AgitUID : " << GetUID() << L", LoginID : " << GetName()) << END_LOG;
    }
}

bool KUser::IsDataReceive()
{
    return m_bDataReceive;
}

void KUser::AddStatPVPBalance(IN const KGameResultIn& kResultIn_)
{
    // 대전 통계
    if (m_pkRoom == NULL) {
        return;
    }

    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if (pkCharacter == NULL) {
        return;
    }

    KDB_EVENT_PVP_BALANCE_STAT_REQ kDBPacket;

    kDBPacket.m_dwLoginUID = kResultIn_.m_dwUID;
    kDBPacket.m_cCharType = pkCharacter->m_cCharType;
    kDBPacket.m_cCharPromotion = kResultIn_.m_cCurrentPromotion;
    kDBPacket.m_dwLevel = pkCharacter->m_dwLevel;
    kDBPacket.m_nModeID = m_pkRoom->m_iGameMode;
    kDBPacket.m_nPlayTime = (int)(::timeGetTime() - m_pkRoom->m_dwStartGameTime) / (1000 * 60);
    if (!kResultIn_.m_bWin) {
        kDBPacket.m_nResult = 1;
    }
    USHORT usLifeCount;
    m_pkRoom->GetUserLifeCount(kResultIn_.m_dwUID, usLifeCount);
    kDBPacket.m_nLifeCount = usLifeCount;
    kDBPacket.m_kUserSkillStat = kResultIn_.m_kUserSkillStat;

    QUEUEING_EVENT_TO_DB(DB_EVENT_PVP_BALANCE_STAT_REQ, kDBPacket);
}

void KUser::SendPetVestedItemNot()
{
    KEVENT_PET_VESTED_ITEM_NOT kPacket;
    SiKPetHelper()->GetPetVestedItem(kPacket);
    SEND_PACKET(EVENT_PET_VESTED_ITEM_NOT);
}

int KUser::CheckFatigueTime() // EVENT_COMPLETE_MISSION_REQ 와 KRoom::StartGame 에서 호출.
{
    if (!SiKFatigueSystem()->IsRun()) {
        return -1;
    }

    if (m_bAdult == true) {
        return -1;
    }

    int nRatio = SiKFatigueSystem()->GetRewardRatio(GetAccOnlineTime() + GetElapsedTimeAfterAuth(), GetName());

    return nRatio;
}

void KUser::LoadErrandEventInfo()
{
    if (true == SiKErrandManager()->IsRun()) {
        QUEUEING_ID_TO_DB(DB_EVENT_ERRAND_USER_INFO_REQ);
    }
}

void KUser::UpdateErrandTime()
{
    if (true == SiKErrandManager()->IsRun()) { // 심부름왕 심부름 목록 갱신(플레이 가능 여부 갱신)
        if (!m_kErrandTimer.CheckTime()) {
            return;
        }

        if (true == m_kErrandUserData.CheckErrandPlayableStateUpdate()) {
            // 대기 시간이 지나고 플레이 가능
            m_kErrandUserData.SetPlayable(true);

            bool bPlayable;
            m_kErrandUserData.GetPlayable(bPlayable);

            // 플레이 가능하다고 send_data하고 로그 남기자
            SEND_DATA(EVENT_ERRAND_LIST_PROVIDE_NOT, bPlayable);

            START_LOG(clog, L"심부름 이벤트 대기 시간을 다 기다려서 플레이 가능한 상태가 됨. LoginID : " << GetName()) << END_LOG;
        }
    }
}

void KUser::AddPlayTime(IN const DWORD& dwTime_, IN const DWORD& dwModeType_)
{
    m_dwPlayTime += dwTime_;
    /* Room객체에 설정
    enum {
    MT_MODETYPE_PVP = 0,
    MT_MODETYPE_PVE = 1,
    MT_MODETYPE_MAX,
    };
    */
    if (dwModeType_ == 0) { // 대전 playTime
        m_dwPVPPlayTime += dwTime_;
    }
    else { // 던전 playTime
        m_dwPVEPlayTime += dwTime_;
    }
}

void KUser::AddStatPlayTime(IN const DWORD& dwPVPPlayTime_, IN const DWORD& dwPVEPlayTime_, IN const DWORD& dPlayTime_)
{
    KEVENT_STAT_USER_PLAY_TIME_NOT kDBPacket;
    kDBPacket.m_dwUserUID = GetUID();
    kDBPacket.m_dwPVPPlayTime = (dwPVPPlayTime_ / (60 * 1000));
    kDBPacket.m_dwPVEPlayTime = (dwPVEPlayTime_ / (60 * 1000));
    kDBPacket.m_dwUserDurationTime = dPlayTime_;
    QUEUEING_EVENT_TO_DB(EVENT_STAT_USER_PLAY_TIME_NOT, kDBPacket);
}

bool KUser::CheckUserAttributeMigrationPossible(IN const GCITEMUID& dwItemUID_, IN const time_t& tmRegDate_, OUT DWORD& dwChangePossbileCount_)
{
    dwChangePossbileCount_ = 0;
    std::map<GCITEMUID, DWORD>::iterator mit;
    mit = m_mapAttributeMigrationItemInfo.find(dwItemUID_);

    // 아이템 기간 체크.
    if (!SiKItemManager()->m_kAttributeDecider.CheckItemChangePossible(tmRegDate_)) {
        return false;
    }

    if (mit == m_mapAttributeMigrationItemInfo.end()) {
        return true;
    }

    if (mit->second >= SiKItemManager()->m_kAttributeDecider.GetUserChangeMaxCount()) {
        return false;
    }

    dwChangePossbileCount_ = mit->second;

    return true;
}

void KUser::IncreaseAttributeChangePossbileCount(IN const GCITEMUID& dwItemUID_, OUT DWORD& dwChangePossbileCount_)
{
    std::map<GCITEMUID, DWORD>::iterator mit;
    mit = m_mapAttributeMigrationItemInfo.find(dwItemUID_);
    if (mit == m_mapAttributeMigrationItemInfo.end()) {
        m_mapAttributeMigrationItemInfo[dwItemUID_] = 1;
    }
    else {
        mit->second += 1;
    }
}

void KUser::GetCharSlotNum(OUT DWORD& dwSlotNum_, IN char cCharType_ /*-1*/)
{
    dwSlotNum_ = 0;
    std::map<char, KCharacterInfo>::iterator mit;
    if (cCharType_ == -1) {
        cCharType_ = m_cCurrentCharacter;
    }
    mit = m_mapCharacterInfo.find(cCharType_);
    if (mit != m_mapCharacterInfo.end()) {
        dwSlotNum_ = mit->second.m_dwSlotNum;
    }
}

void KUser::GetCharNickNameInfo(IN const char& cCharType_, OUT std::wstring& wstrNickName_)
{
    wstrNickName_.clear();
    std::map<char, KCharacterInfo>::iterator mit;

    mit = m_mapCharacterInfo.find(cCharType_);
    if (mit != m_mapCharacterInfo.end()) {
        wstrNickName_ = mit->second.m_strCharName;
    }
}

int KUser::GetCharInvenCapacity()
{
    std::map<char, KCharacterInfo>::iterator mit;
    int nInvenCapacity = 0;

    mit = m_mapCharacterInfo.find(m_cCurrentCharacter);
    if (mit == m_mapCharacterInfo.end()) {
        return nInvenCapacity;
    }

    nInvenCapacity = mit->second.m_nInvenCapacity;
    return nInvenCapacity;
}

int KUser::GetTrueInvenCapacity()
{
    std::map<char, KCharacterInfo>::iterator mit;
    int nInvenCapacity = 0;

    for (auto& element : m_mapCharacterInfo) {
        nInvenCapacity += element.second.m_nInvenCapacity;
    }

    return nInvenCapacity;
}

void KUser::UpdateCharInvenCapacity(IN const int& nAddInvenCapacity_, OUT int& nTotalInvenCapacity_)
{
    nTotalInvenCapacity_ = 0;
    std::map<char, KCharacterInfo>::iterator mit;

    mit = m_mapCharacterInfo.find(m_cCurrentCharacter);
    if (mit != m_mapCharacterInfo.end()) {
        mit->second.m_nInvenCapacity += nAddInvenCapacity_;
        nTotalInvenCapacity_ = mit->second.m_nInvenCapacity;
    }
}

void KUser::UpdateCharNickNameInfo(IN const char& cCharType_, IN const std::wstring& wstrNickName_)
{
    std::map<char, KCharacterInfo>::iterator mit;

    mit = m_mapCharacterInfo.find(cCharType_);
    if (mit != m_mapCharacterInfo.end()) {
        mit->second.m_strCharName = wstrNickName_;
    }
}

void KUser::GetCharQuickSlotInfo(OUT KQuickSlot& kQuickSlot_)
{
    std::map< char, KQuickSlot >::iterator mit;
    mit = m_mapQuickSlot.find(m_cCurrentCharacter);
    if (mit != m_mapQuickSlot.end()) {
        kQuickSlot_ = mit->second;
    }
}

bool KUser::CheckChatEnable()
{
    time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime());

    // 채팅 금지 등급이 아닐 경우..,
    if (m_kBadUserInfo.m_cCurrentGrade != KUser::AL_CHAT_BLOCK) {
        return true;
    }

    // 채팅금지 기간이 영구일 경우..,
    if (m_kBadUserInfo.m_nPeriodType == KEVENT_BAD_USER_INFO_NOT::PT_UNLIMITED) {
        return false;
    }

    // 채팅 금지 기간이 지났을 경우.
    if (m_kBadUserInfo.m_tmLimit > tmCurrent) {
        return false;
    }
    return true;
};

void KUser::GetUserDifficultInfo(OUT std::map<int, KDungeonUserInfo>& mapDifficulty_)
{
    mapDifficulty_.clear();

    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mit;
    mit = m_mapDifficulty.find(m_cCurrentCharacter);
    if (mit != m_mapDifficulty.end()) {
        mapDifficulty_ = mit->second;
    }
}

void KUser::UpdateCharDifficultInfo(IN const char& cCharType_, IN std::map<int, KDungeonUserInfo>& mapDifficulty_)
{
    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mit;
    mit = m_mapDifficulty.find(cCharType_);
    if (mit == m_mapDifficulty.end()) {
        m_mapDifficulty[cCharType_] = mapDifficulty_;
    }
}

bool KUser::ChangeUserChatState(IN const bool& bChatEnable_)
{
    if (bChatEnable_) {
        if (m_kBadUserInfo.m_cCurrentGrade == KUser::AL_NORMAL) {
            return false;
        }
        m_kBadUserInfo.m_cCurrentGrade = KUser::AL_NORMAL;
    }
    else {
        if (m_kBadUserInfo.m_cCurrentGrade == KUser::AL_CHAT_BLOCK) {
            return false;
        }
        m_kBadUserInfo.m_cCurrentGrade = KUser::AL_CHAT_BLOCK;
    }
    return true;
}

void KUser::CheckUpgradeGuildGradeGift()
{
    // 길마인 유저의 경우에만 처리한다
    if (KNGuildUserInfo::GL_MASTER != m_kGuildUserInfo.m_cMemberLevel) {
        return;
    }

    QUEUEING_EVENT_TO_DB(EVENT_CHECK_UPGRADE_GUILD_GRADE_GIFT_NOT, m_kGuildUserInfo.m_dwGuildUID);
}

void KUser::DungeonMonsterHackDisconnectCheck(IN const int& nModeID_)
{
    // 접속종료 처리 체크
    if (false == SiKPreHackingCheckManager()->IsMonsterCntErrDisconnect()) {
        return;
    }
    else {
        START_LOG(cerr, L"설정된 몬스터 갯수보다 많은 몬스터를 죽였음. Name : " << m_strName)
            << BUILD_LOG(nModeID_)
            << END_LOG;
    }

    KEVENT_CLOSE_CONNECTION_NOT kPacket;
    kPacket.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_DETECT_HACK;
    kPacket.m_strMessage = SiKGameServer()->m_stringTable.GetValue(L"str_15");
    SEND_DATA(EVENT_CLOSE_CONNECTION_NOT, kPacket);

    KEVENT_INSERT_DUNGEON_HACK_NOT kDBPacket;
    kDBPacket.m_nHackType = KPreHackingCheckManager::HT_DUNGEON_MONSTER_HACKING;
    kDBPacket.m_nModeID = nModeID_;
    QUEUEING_EVENT_TO_DB(EVENT_INSERT_DUNGEON_HACK_NOT, kDBPacket);
    ReserveDestroy(DRW_CLIENT_HACKING);
}

bool KUser::InsertSkillInfo(IN SSkillElement kSkill_)
{
    if (IsTrainedSkill(kSkill_)) {
        START_LOG(clog, L"이미 배운 스킬")
            << BUILD_LOGc(kSkill_.m_nID)
            << END_LOG;
        return false;
    }

    AddDBUpdatedTrainedSkill(kSkill_);
    return true;
}

bool KUser::AddDBUpdatedTrainedSkill(IN const SSkillElement& kSkill_)
{
    // 해당 함수는 DB에 이미 스킬 배운 정보를 기록한 다음 호출해야 한다.
    std::map<std::pair<char, char>, KSPInfo >::iterator mit;
    std::pair<char, char> prKey(kSkill_.m_cCharType, kSkill_.m_cPromotion);
    mit = m_mapTrainedSkill.find(prKey);
    // 스킬이 없을 경우에 대하여 추가.
    if (mit == m_mapTrainedSkill.end()) {
        KSPInfo kSpInfo;
        kSpInfo.m_cCharType = kSkill_.m_cCharType;
        kSpInfo.m_cPromotion = kSkill_.m_cPromotion;
        kSpInfo.m_vecSkills.push_back(kSkill_.m_nID);
        m_mapTrainedSkill[prKey] = kSpInfo;
    }
    else {
        m_mapTrainedSkill[prKey].m_vecSkills.push_back(kSkill_.m_nID);
    }

    return true;
}

void KUser::SendUserSystemInfo(IN int m_iSessionInfo_)
{
    CheckHellItemConnection(m_iSessionInfo_); // 접속시 헬 아이템 체크.
    ServerTimeNot();
    CheckGCClubConnectBonus(m_iSessionInfo_); // 접속시 GC클럽 보너스지급 확인
    // 룩 정보 가져 오기.
    QUEUEING_ID_TO_DB(DB_EVENT_FULL_LOOK_INFO_REQ);

    SendUserAgitInfo();
    // 넷마블 PC방 혜택 체크문의
    SendNMCafeLogin();
    SendDungeonTicketListNot();
    SendMatchTimeTable();
    SendPetVestedItemNot();
    CheckCharacterGraduate(KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_CONNECT);
    QUEUEING_ID_TO_DB(DB_EVENT_SYSTEM_GUIDE_COMPLETE_INFO_REQ); // 유저의 시스템 가이드 완료 정보 얻기
    QUEUEING_ID_TO_DB(DB_EVENT_USER_ATTRIBUTE_MIGRATION_INFO_REQ); // 유저 개편 재설정 정보 가져오기.
    QUEUEING_ID_TO_DB(EVENT_USER_HERO_DUNGEON_INFO_REQ);
}

void KUser::SetVirtualDepotItemList(IN std::vector<KItem>& vecVirtualDepot_)
{
    m_vecVirtualDepot.clear();
    m_vecVirtualDepot = vecVirtualDepot_;
}

bool KUser::IsValidItemCount(IN const KItem& kCountItem_)
{
    std::vector< KItem >::iterator vitVDItem;
    vitVDItem = std::find_if(m_vecVirtualDepot.begin(), m_vecVirtualDepot.end(),
        boost::bind(&KItem::m_ItemUID, _1) == kCountItem_.m_ItemUID);

    if (m_vecVirtualDepot.end() != vitVDItem) {
        if (kCountItem_.m_nCount <= vitVDItem->m_nCount) {
            return true;
        }
    }

    // 가상창고의 수량보다 많은 수량을 요청 함
    return false;
}

bool KUser::GetVirtualDepotItemByItemUID(IN const GCITEMUID& dwItemUID_, OUT KItem& kItem_)
{
    std::vector< KItem >::iterator vitVDItem;
    vitVDItem = std::find_if(m_vecVirtualDepot.begin(), m_vecVirtualDepot.end(),
        boost::bind(&KItem::m_ItemUID, _1) == dwItemUID_);

    if (m_vecVirtualDepot.end() != vitVDItem) {
        kItem_ = *vitVDItem;
        return true;
    }

    return false;
}

int KUser::GetCharLookInvenCapacity()
{
    std::map<char, KCharacterInfo>::iterator mit;
    int nLookInvenCapacity = 0;

    mit = m_mapCharacterInfo.find(m_cCurrentCharacter);
    if (mit == m_mapCharacterInfo.end()) {
        return nLookInvenCapacity;
    }

    nLookInvenCapacity = mit->second.m_nInvenCapacity;
    return nLookInvenCapacity;
}

int KUser::GetTrueLookInvenCapacity()
{
    std::map<char, KCharacterInfo>::iterator mit;
    int nLookInvenCapacity = 0;

    for (auto& element : m_mapCharacterInfo) {
        nLookInvenCapacity += element.second.m_nInvenCapacity;
    }

    return nLookInvenCapacity;
}

void KUser::UpdateCharLookInvenCapacity(IN const int& nExtendSize_)
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find(m_cCurrentCharacter);
    if (mit != m_mapCharacterInfo.end()) {
        mit->second.m_nLookInvenCapacity += nExtendSize_;
    }
}

void KUser::AddVirtualDepotItemList(IN std::vector<KItem>& vecAddItems_)
{
    std::vector<KItem>::iterator vitAdd;
    for (vitAdd = vecAddItems_.begin(); vitAdd != vecAddItems_.end(); ++vitAdd) {
        // 가상창고 가장 앞쪽으로 넣어줌
        if (KItem::VIRTUAL_DEPOT == vitAdd->m_cInvenType) {
            m_vecVirtualDepot.insert(m_vecVirtualDepot.begin(), *vitAdd);
        }
    }
}

bool KUser::CheckTCPRelayStatus(IN const USHORT& usRoomID_, IN const DWORD& dwTRServerIP_, IN const USHORT& usTRServerPort_, IN const std::vector<KSimpleServerInfo>& vecUseTCPServerList_, IN const std::vector<KSimpleServerInfo>& vecNoUseTCPServerList_, OUT KEVENT_RELAY_SERVER_STATUS_ACK& kPacket_)
{
    int nIndex;
    std::wstring wstrUseIP;
    std::string strUseIP;
    USHORT usUsePort;
    in_addr address;
    std::vector<KSimpleServerInfo>::const_iterator cvit;
    address.s_addr = dwTRServerIP_;
    std::wstring strIP = KncUtil::toWideString(::inet_ntoa(address));
    if (vecNoUseTCPServerList_.empty() || vecUseTCPServerList_.empty()) {
        kPacket_.m_nOK = 0;
        return true;
    }

    for (cvit = vecNoUseTCPServerList_.begin(); cvit != vecNoUseTCPServerList_.end(); ++cvit) {
        if (cvit->m_strIP == strIP && cvit->m_usPort == usTRServerPort_) {
            int nSize = vecUseTCPServerList_.size();
            nSize = std::max<int>(1, nSize);
            nIndex = usRoomID_ % nSize;
            wstrUseIP = vecUseTCPServerList_[nIndex].m_strIP;
            usUsePort = vecUseTCPServerList_[nIndex].m_usPort;

            strUseIP = KncUtil::toNarrowString(wstrUseIP);

            SOCKADDR_IN addrTRServer;
            addrTRServer.sin_addr.s_addr = inet_addr(strUseIP.c_str());
            kPacket_.m_nOK = 1; // TR 서버 변경.
            kPacket_.m_dwTRelayServerIP = addrTRServer.sin_addr.S_un.S_addr;
            kPacket_.m_usTRelayServerPort = usUsePort;
            return false;
        }
    }

    return true;
}

bool KUser::CheckGradeUpgrade(IN const char& cRequestGradeID_, IN const char& cCurrentGradeID_)
{
    if (cCurrentGradeID_ == KItem::GRADE_MYSTIC) { // 현재 등급이 최대 등급이면..,
        return false;
    }

    if (cRequestGradeID_ <= cCurrentGradeID_) { // 요청 등급이 현재 등급보다 같거나 낮을 경우..,
        return false;
    }

    if (cRequestGradeID_ != (cCurrentGradeID_ + 1)) { // 바로 상위 등급이 아닐 경우..,
        return false;
    }

    return true;
}

void KUser::SetCharEquipItemDetail(IN const std::vector<KItem>& vecInv_, IN OUT std::vector<KEquipItemInfo>& vecEquipItems_)
{
    std::vector<KEquipItemInfo>::iterator vit;
    for (vit = vecEquipItems_.begin(); vit != vecEquipItems_.end(); ++vit) {
        // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
        std::vector<KItem>::const_iterator vitItem;
        vitItem = std::find_if(vecInv_.begin(), vecInv_.end(),
            boost::bind(&KItem::m_ItemUID, _1) == vit->m_dwUID);

        // 보유 하고 있지 않으면 장착 하지 않음..
        if (vitItem == vecInv_.end()) {
            continue;
        }

        vit->m_dwID = vitItem->m_ItemID;
        vit->m_vecAttribute = vitItem->m_vecAttribute;
        vit->m_vecSocket = vitItem->m_vecSocket;
        vit->m_cGradeID = vitItem->m_cGradeID;
        vit->m_nEquipLevel = static_cast<int>(vitItem->m_sEquipLevel);
        vit->m_cEnchantLevel = vitItem->m_cEnchantLevel;
        vit->m_cEnchantEquipGradeID = vitItem->m_cEnchantEquipGradeID;
        vit->m_DesignCoordiID = vitItem->m_DesignCoordiID;
    }
}

void KUser::SetCharLookEquipDetail(IN const std::vector<KItem>& vecInv_, IN OUT std::vector<KEquipItemInfo>& vecLoogEquip_)
{
    std::vector<KEquipItemInfo>::iterator vit;
    for (vit = vecLoogEquip_.begin(); vit != vecLoogEquip_.end(); ++vit) {
        // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
        std::vector<KItem>::const_iterator vitItem;
        vitItem = std::find_if(vecInv_.begin(), vecInv_.end(),
            boost::bind(&KItem::m_ItemUID, _1) == vit->m_dwUID);

        // 보유 하고 있지 않으면 장착 하지 않음..
        if (vitItem == vecInv_.end()) {
            continue;
        }

        vit->m_dwID = vitItem->m_ItemID;
        vit->m_vecAttribute = vitItem->m_vecAttribute;
        vit->m_vecSocket = vitItem->m_vecSocket;
        vit->m_cGradeID = vitItem->m_cGradeID;
        vit->m_nEquipLevel = static_cast<int>(vitItem->m_sEquipLevel);
        vit->m_cEnchantLevel = vitItem->m_cEnchantLevel;
        vit->m_cEnchantEquipGradeID = vitItem->m_cEnchantEquipGradeID;
        vit->m_DesignCoordiID = vitItem->m_DesignCoordiID;
    }
}

void KUser::SetCharChangeWeaponEquipDetail(IN const std::vector<KItem>& vecInv_, IN OUT KEquipItemInfo& kChangeWeaponItem_)
{
    // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
    std::vector<KItem>::const_iterator vitItem;
    vitItem = std::find_if(vecInv_.begin(), vecInv_.end(),
        boost::bind(&KItem::m_ItemUID, _1) == kChangeWeaponItem_.m_dwUID);

    // 보유 하고 있지 않으면 장착 하지 않음..
    if (vitItem != vecInv_.end()) {
        kChangeWeaponItem_.m_dwID = vitItem->m_ItemID;
        kChangeWeaponItem_.m_vecAttribute = vitItem->m_vecAttribute;
        kChangeWeaponItem_.m_vecSocket = vitItem->m_vecSocket;
        kChangeWeaponItem_.m_cGradeID = vitItem->m_cGradeID;
        kChangeWeaponItem_.m_nEquipLevel = static_cast<int>(vitItem->m_sEquipLevel);
        kChangeWeaponItem_.m_cEnchantLevel = vitItem->m_cEnchantLevel;
        kChangeWeaponItem_.m_cEnchantEquipGradeID = vitItem->m_cEnchantEquipGradeID;
        kChangeWeaponItem_.m_DesignCoordiID = vitItem->m_DesignCoordiID;
    }
}

bool KUser::GetLookEquipItem(IN const char& cCharType_, OUT std::vector<KEquipItemInfo>& vecLookEquipInfo_)
{
    vecLookEquipInfo_.clear();
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo(cCharType_);
    if (!pkCharacterInfo) {
        return false;
    }

    vecLookEquipInfo_ = pkCharacterInfo->m_vecLookEquips;
    return true;
}

void KUser::SetCharEquipItem(IN const std::vector<KItem> vecRewardItem_, IN OUT KCharacterInfo& kCharacterInfo_)
{
    std::vector<KItem>::const_iterator cvit;
    for (cvit = vecRewardItem_.begin(); cvit != vecRewardItem_.end(); ++cvit) {
        KEquipItemInfo equipElement;
        equipElement.m_dwID = cvit->m_ItemID;
        equipElement.m_dwUID = cvit->m_ItemUID;

        kCharacterInfo_.m_vecEquipItems.push_back(equipElement);
    }
}

void KUser::ApplyCharEquipItem(IN const char& cCharType_, IN const std::vector<KEquipItemInfo>& vecEquipItems_)
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find(cCharType_);
    if (mit != m_mapCharacterInfo.end()) {
        mit->second.m_vecEquipItems = vecEquipItems_;
    }
}

void KUser::SetCharEquipPet(IN const std::map<GCITEMUID, KPetInfo>& mapPetInfo_, IN OUT KPetInfo& kEquipPetInfo_)
{
    if (kEquipPetInfo_.m_dwUID == 0) { // 펫이 장착되지 않은 경우 처리 안함.
        return;
    }

    std::map<GCITEMUID, KPetInfo>::const_iterator cmit;
    for (cmit = mapPetInfo_.begin(); cmit != mapPetInfo_.end(); ++cmit) {
        if (cmit->first == kEquipPetInfo_.m_dwUID) {
            kEquipPetInfo_.m_vecInitEquipItem = cmit->second.m_vecInitEquipItem;
            kEquipPetInfo_.m_vecEquipItem = cmit->second.m_vecEquipItem;
            kEquipPetInfo_.m_kCostume = cmit->second.m_kCostume;
        }
    }
}

void KUser::UpdateDepotDataReq()
{
    KDepotUpdateData kDepotUpdateData;
    kDepotUpdateData.m_bAckToUser = true;
    GetDBDepotUpdateData(kDepotUpdateData);
    QUEUEING_EVENT_TO_DB(DB_EVENT_UPDATE_DEPOT_DATA_REQ, kDepotUpdateData);

    // 공용창고 수량 아이템 로그 전달.
    KDB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ kDBLogPacket;
    GetInvenDepotMoveInfo(kDBLogPacket);
    QUEUEING_EVENT_TO_DB(DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ, kDBLogPacket);
}

void KUser::InsertInvenDepotMoveInfo(IN const KInvenDepotMoveLog& kInvenDepotMoveLog_)
{
    m_vecInvenDepotMoveLog.push_back(kInvenDepotMoveLog_);
}

void KUser::GetInvenDepotMoveInfo(OUT std::vector<KInvenDepotMoveLog>& vecInvenDepotMoveLog_)
{
    vecInvenDepotMoveLog_.clear();
    m_vecInvenDepotMoveLog.swap(vecInvenDepotMoveLog_);
}

void KUser::UpdateDungeonBestRecordData(IN KUser& kUser_, IN const int& nGameMode_, IN const int& nDifficulty_, IN const int& nRank_, IN const DWORD& dwClearTime_)
{
    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mitCharAndMode;
    std::map<int, KDungeonUserInfo>::iterator mitMode;

    int nRankData = nRank_;
    DWORD dwClearTimeData = dwClearTime_;

    // 클라에서 받은 값 보정
    nRankData = std::min<int>(nRankData, SiKResultManager()->GetMaxRankLimit());
    nRankData = std::max<int>(nRankData, 0);

    dwClearTimeData = std::min<DWORD>(dwClearTimeData, ULONG_MAX);
    dwClearTimeData = std::max<DWORD>(dwClearTimeData, 0);

    // 캐릭터 별 정보를 얻는다
    mitCharAndMode = kUser_.m_mapDifficulty.find(kUser_.GetCurrentCharType());
    if (kUser_.m_mapDifficulty.end() == mitCharAndMode) {
        return;
    }
    // 해당 게임 모드의 정보를 얻는다
    mitMode = mitCharAndMode->second.find(nGameMode_);
    if (mitCharAndMode->second.end() == mitMode) {
        return;
    }

    std::map< int, KUsersDungeonRecordData >::iterator mitDungeonRecord;
    // 컨트롤 피드백 레코드 데이터를 얻는다. 없으면 새로 기록 있다면 최고 기록인지 확인 후 기록
    mitDungeonRecord = mitMode->second.m_kClearData.m_mapBestRecord.find(nDifficulty_);
    if (mitMode->second.m_kClearData.m_mapBestRecord.end() != mitDungeonRecord) {
        bool bDataUpdated = false;
        // 최고 기록을 갱신했는지 확인
        if (mitDungeonRecord->second.m_nBestClearRank < nRankData) {
            mitDungeonRecord->second.m_nPrevBestClearRank = mitDungeonRecord->second.m_nBestClearRank;
            mitDungeonRecord->second.m_nBestClearRank = std::max<int>(nRankData, mitDungeonRecord->second.m_nBestClearRank);
            mitDungeonRecord->second.m_bClearRankChanged = true;
            bDataUpdated = true;
        }
        if (mitDungeonRecord->second.m_dwBestClearTime > dwClearTimeData) {
            mitDungeonRecord->second.m_dwPrevBestClearTime = mitDungeonRecord->second.m_dwBestClearTime;
            mitDungeonRecord->second.m_dwBestClearTime = std::min<DWORD>(dwClearTimeData, mitDungeonRecord->second.m_dwBestClearTime);
            mitDungeonRecord->second.m_bClearTimeChanged = true;
            bDataUpdated = true;
        }

        if (true == bDataUpdated) {
            START_LOG(clog, L"컨트롤 레코드 최고 기록 갱신 됨")
                << BUILD_LOG(kUser_.GetUID())
                << BUILD_LOG(kUser_.GetCurrentCharType())
                << BUILD_LOG(nGameMode_)
                << BUILD_LOG(nDifficulty_)
                << BUILD_LOG(mitDungeonRecord->second.m_nBestClearRank)
                << BUILD_LOG(mitDungeonRecord->second.m_dwBestClearTime)
                << BUILD_LOG(mitDungeonRecord->second.m_nPrevBestClearRank)
                << BUILD_LOG(mitDungeonRecord->second.m_dwPrevBestClearTime)
                << END_LOG;
        }
    }
    else { // 기존의 기록이 없다면 바로 기록
        KUsersDungeonRecordData kRecordData;
        kRecordData.m_nBestClearRank = nRankData;
        kRecordData.m_bClearRankChanged = true;
        kRecordData.m_dwBestClearTime = dwClearTimeData;
        kRecordData.m_bClearTimeChanged = true;

        mitMode->second.m_kClearData.m_mapBestRecord.insert(std::make_pair(nDifficulty_, kRecordData));

        START_LOG(clog, L"첫 클리어로 컨트롤 레코드 데이터 기록 함")
            << BUILD_LOG(kUser_.GetUID())
            << BUILD_LOG(kUser_.GetCurrentCharType())
            << BUILD_LOG(nGameMode_)
            << BUILD_LOG(nDifficulty_)
            << BUILD_LOG(mitDungeonRecord->second.m_nBestClearRank)
            << BUILD_LOG(mitDungeonRecord->second.m_dwBestClearTime)
            << END_LOG;
    }
}

void KUser::ProcessPrevRecordDataWhenFirstClear(IN KUser& kUser_, IN const int& nGameMode_, IN const int& nDifficulty_)
{
    // 이 함수는 GameEndAck패킷을 보낸 이 후에 불려야 함

    std::map< char, std::map<int, KDungeonUserInfo> >::iterator mitCharAndMode;
    std::map<int, KDungeonUserInfo>::iterator mitMode;
    std::map< int, KUsersDungeonRecordData >::iterator mitDungeonRecord;

    // 캐릭터 별 정보를 얻는다
    mitCharAndMode = kUser_.m_mapDifficulty.find(kUser_.GetCurrentCharType());
    if (kUser_.m_mapDifficulty.end() == mitCharAndMode) {
        return;
    }
    // 해당 게임 모드의 정보를 얻는다
    mitMode = mitCharAndMode->second.find(nGameMode_);
    if (mitCharAndMode->second.end() == mitMode) {
        return;
    }

    // 컨트롤 피드백 레코드 데이터를 얻는다
    mitDungeonRecord = mitMode->second.m_kClearData.m_mapBestRecord.find(nDifficulty_);
    if (mitMode->second.m_kClearData.m_mapBestRecord.end() != mitDungeonRecord) {
        // 첫 클리어의 경우 Prev값이 0이므로 현재의 값으로 기록하자
        if (0 == mitDungeonRecord->second.m_nPrevBestClearRank || 0 == mitDungeonRecord->second.m_dwPrevBestClearTime) {
            mitDungeonRecord->second.m_nPrevBestClearRank = mitDungeonRecord->second.m_nBestClearRank;
            mitDungeonRecord->second.m_dwPrevBestClearTime = mitDungeonRecord->second.m_dwBestClearTime;
        }
    }
}

bool KUser::CheckCharLimitLevel(IN const DWORD dwLevel_)
{
    std::map<char, KCharacterInfo>::iterator mit;
    for (mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit) {
        if (mit->second.m_dwLevel >= dwLevel_) {
            return true;
        }
    }
    return false;
}