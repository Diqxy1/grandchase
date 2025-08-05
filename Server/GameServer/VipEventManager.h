#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "UserPacket.h"

typedef USHORT  PAY_INDEX;
typedef USHORT  REWARD_ID;
typedef int     REWARD_TYPE;

class KItemBox
{
public:
    KItemBox(void);
    ~KItemBox(void);
    void SetList( IN std::vector<KDropItemInfo>& vecList )  { m_vecItemList.swap( vecList ); }
    void GetList( OUT std::vector<KDropItemInfo>& vecList ) { vecList = m_vecItemList; }
    void DumpList();
private:
    std::vector<KDropItemInfo>  m_vecItemList;
};

    void SetEventRange( IN const time_t& tmStart, IN const time_t& tmEnd );
    void GetEventRange( OUT time_t& tmStart, OUT time_t& tmEnd );
    bool IsActive();
    bool IsEnd();
    bool IsNewbieUser( IN const time_t& tmFirstLogin );

    void SetEventEnable( IN const int& nEventType, IN const bool& bEnable );
    bool IsEventEnable( IN const int& nEventType );

class KLuaManager;
class KVipEventManager
{
    DeclareSingleton( KVipEventManager );
    NiDeclareRootRTTI( KVipEventManager );
    DeclToStringW;

public:
    enum {
        NEWBIE_REWARD = 0,
        VIP_REWARD = 1,
    };

    KVipEventManager(void);
    ~KVipEventManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void SendVipEventInfoReqToDB();
    void UpdateEvent( IN std::map<USHORT,KVipEvent>& mapInfo );
    void DumpInfo();

    void GetEventList( OUT std::map<USHORT,KVipEvent>& mapList );
    bool GetRewardItemByType( IN const USHORT& usType, OUT std::map<USHORT,std::vector<KDropItemInfo> >& mapList );
    void GetPayTable( OUT std::map<PAY_INDEX,PAIR_INT>& mapList );
    void GetRewardByVer( OUT std::map<USHORT,std::map<USHORT,USHORT> >& mapList );
    bool GetCurrentReward( IN const USHORT& usVer, IN const int& nType, IN const std::set<USHORT>& setPayIndex, OUT std::map<USHORT,std::vector<KDropItemInfo> >& mapRewardItem );
    bool IsEventExist();

    bool IsActive( IN const std::pair<time_t,time_t>& prEventRange );
    bool IsEnd( IN const std::pair<time_t,time_t>& prEventRange );
    bool IsNewbieUser( IN const time_t& tmFirstLogin, IN const std::pair<time_t,time_t>& prEventRange );

private:
    bool LoadRewardItem( KLuaManager& kLuaMng, OUT std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> >& mapList );
    bool LoadRewardList( KLuaManager& kLuaMng, OUT REWARD_TYPE& rewardType, OUT std::map<REWARD_ID,KItemBox>& mapList );
    bool LoadReward( KLuaManager& kLuaMng, OUT REWARD_ID& rewardID, OUT std::vector<KDropItemInfo>& vecList );
    bool LoadPayTable( KLuaManager& kLuaMng, OUT std::map<PAY_INDEX,PAIR_INT>& mapList );
    bool LoadRewardByVer( KLuaManager& kLuaMng, OUT std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >& mapList );
    bool LoadRewardTable( KLuaManager& kLuaMng, OUT std::map<PAY_INDEX,REWARD_ID>& mapList );

    void DumpRewardItem();
    void DumpPayTable();
    void DumpRewardByVer();
    void DumpEventByVer();

private:
    mutable KncCriticalSection                          m_csVipEvent;
    std::map<REWARD_TYPE,std::map<REWARD_ID,KItemBox> > m_mapRewardItem; // 보상타입별 보상아이템리스트 [보상타입(신규,VIP), [보상ID,보상그룹] ]
    std::map<PAY_INDEX,std::pair<int,int> >             m_mapPayTable; // 구매금액 테이블 [금액ID, [min~max] ]
    std::map<USHORT,std::map<PAY_INDEX,REWARD_ID> >     m_mapRewardByVer; // 버전별 보상ID [Ver, [금액ID,보상ID] ]
    std::map<USHORT,KVipEvent>                          m_mapEventByVer; // 버전별 이벤트정보 [Ver, 이벤트정보]
};
DefSingletonInline( KVipEventManager );
DeclOstmOperatorA( KVipEventManager );