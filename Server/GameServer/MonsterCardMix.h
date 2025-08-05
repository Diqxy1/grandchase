#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>

#include "Kairo.h"

class KLuaManager;
class KMonsterCardMix
{
    DeclareSingleton( KMonsterCardMix );
    NiDeclareRootRTTI( KMonsterCardMix );
    DeclToStringW;
public:
    KMonsterCardMix(void);
    ~KMonsterCardMix(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsEnable() { KLocker lock(m_csMonsterCardMixEnable); return m_bMonsterCardMixEnable; }
    USHORT GetMinMaterialCount() { KLocker lock(m_csMonsterCardMixConfig); return m_usMinMaterialCount; }
    USHORT GetMaxMaterialCount() { KLocker lock(m_csMonsterCardMixConfig); return m_usMaxMaterialCount; }
    bool GetMixReward( IN const std::vector<int>& vecMaterialGrade, OUT GCITEMID& RewardItemID );

    void GetMonsterCardInfoForPacket( OUT KEVENT_MONSTER_CARD_MIX_INFO_ACK& kPacket );

private:
    bool LoadGradePoint( IN KLuaManager& kLuaMng, OUT std::map<int,USHORT>& mapList );
    bool LoadPointReward( IN KLuaManager& kLuaMng, OUT std::map<PAIR_USHORT,Kairo>& mapList );
    bool LoadRewardGroup( IN KLuaManager& kLuaMng, OUT std::map<USHORT,Kairo>& mapList );
    bool LoadCaseRatioForPointReward( IN KLuaManager& kLuaMng, IN const std::string& strScript, OUT Kairo& kKairo, OUT std::vector<USHORT>& vecRewarGroup ); // m_mapPointRewardForPacket 만드는 기능 추가
    bool LoadCaseRatioForRewardGroup( IN KLuaManager& kLuaMng, IN const std::string& strScript, OUT Kairo& kKairo, OUT std::vector<GCITEMID>& vecCardList ); // m_mapRewardGroupForPacket 만드는 기능 추가

    USHORT GetGradePoint( IN const int& nGrade );
    USHORT GetPointReward( IN const USHORT& usPoint );
    GCITEMID GetRewardItemID( IN const USHORT& usRewardGroup );

private:
    mutable KncCriticalSection  m_csMonsterCardMixEnable;
    bool                        m_bMonsterCardMixEnable; // 몬스터카드 조합 사용할지 여부

    mutable KncCriticalSection  m_csMonsterCardMixConfig;
    USHORT                      m_usMinMaterialCount; // 최소 조합재료수
    USHORT                      m_usMaxMaterialCount; // 최대 조합재료수
    std::map<int,USHORT>        m_mapGradePoint; // 카드등급별 포인트  map[등급,포인트]
    std::map<PAIR_USHORT,Kairo> m_mapPointReward; // 획득 카드 그룹 확률  map[포인트범위,보상확률]
    std::map<USHORT,Kairo>      m_mapRewardGroup; // 그룹별 카드 구성  map[그룹넘버,보상확률]

    std::map<PAIR_USHORT,std::vector<USHORT>> m_mapPointRewardForPacket; // 클라이언트에서 요청하는 정보 전용
    std::map<USHORT,std::vector<GCITEMID>>    m_mapRewardGroupForPacket; // 클라이언트에서 요청하는 정보 전용
};
DefSingletonInline( KMonsterCardMix );
DeclOstmOperatorA( KMonsterCardMix );