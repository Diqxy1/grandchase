#pragma once

#include <vector>
#include <map>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <ToString.h>
#include "UserPacket.h"
#include "PrimitiveTypedef.h"

class KLuaManager;
struct KDropItemInfo;

// 영던 개편으로 추가 한 클래스
// 기존의 팬텀피닉스 아이템 구매는 동일하게 KManufactureS3 사용
// 개편은 기존의 제작/해체 시스템 변경할 계획 없어서 새로운 클래스 제작(재료 아이템 등급 추가 됨)
// 기획자 요청으로 동일한 스크립트 (InitHeroItemInfo.lua) 에 테이블만 추가하여
// 해당 스크립트 영던 아이템 구매에서도 읽고 이 클래스에서도 읽는다 (기존 구매 동일 동작)
class KHeroItemUpgradeManager
{
public:
    KHeroItemUpgradeManager(void);
    ~KHeroItemUpgradeManager(void);
    bool LoadScript( IN const std::string& strScript );
    void GetHeroItemUpgradeInfo( OUT KEVENT_HERO_ITEM_UPGRADE_INFO_ACK& kPacket );
    bool GetUpgradeInfoByKey(  IN const PAIR_DWORD_INT& prKey_, OUT KHeroItemUpgradeInfo& upgradeItemInfo, OUT KDropItemInfo& rewardItemInfo );

private:
    bool LoadMaterials( IN OUT KLuaManager& kLuaMng, OUT std::vector<KDropItemInfo>& vecMaterial );

private:
    KncCriticalSection                               m_csHeroItemUpgradeManager;
    std::map< PAIR_DWORD_INT, KHeroItemUpgradeInfo > m_mapHeroItemUpgradeInfoList; // ItemID와 Grade를 키값으로 사용
};
