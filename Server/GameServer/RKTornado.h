#pragma once
#include <map>
#include <set>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "Kairo.h"

class KLuaManager;
struct KItemUnit;

struct KTornadoInfo
{
    DWORD                               m_dwCharType; // CharType
    Kairo                               m_kKairoNormal; // 전체 아이템 리스트
    Kairo                               m_kKairoNonFail; // 꽝 아이템 제외 리스트
    Kairo                               m_kKairoFail; // 꽝 아이템 리스트
    std::map< DWORD, std::set<GCITEMID> > m_mapTypeItemList; // < Type, ItemList >
    std::vector<KDropItemInfo>          m_vecFailRewardList; // 꽝 아이템 보상 리스트.
};

class KRKTornado
{
    DeclareSingleton( KRKTornado );
    NiDeclareRootRTTI( KRKTornado );
    DeclToStringW;
public:
    enum ACTION_TYPE {
        AT_NORMAL       = 0, // NORMAL
        AT_NONFAIL      = 1, // 꽝아이템 제외.
    };

    enum ITEM_TYPE {
        IT_EQUIPMENT    = 0, // 장비
        IT_GOAL         = 1, // 목표
        IT_NORMAL       = 2, // 일반
        IT_FAIL         = 3, // 꽝 
    };

    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KRKTornado(void);
    ~KRKTornado(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun(); // 릭쿤 토네이도 이벤트 진행중인지 체크.
    bool DoGamble( IN DWORD& dwCharType, IN DWORD& dwType ,OUT KDropItemInfo& kItem );
    bool GetFailItemList( IN DWORD& dwCharType, IN DWORD& dwType ,OUT std::vector<KDropItemInfo>& vecFailRewardList );
    bool IsCheckTornadoCoin( IN GCITEMID& dwItemID );
    bool IsCheckTornadoFailItem( IN GCITEMID& dwItemID );
    DWORD GetFailItemCount(){ KLocker lock( m_csTornado ); return m_dwFailItemCount; }
    bool GetItemType( IN GCITEMID& dwItemID, OUT DWORD& dwType );
    void GetNonFailRewardList( OUT std::map<DWORD,std::map<DWORD, std::set<GCITEMID> > >& mapNonFailRewardList );
    void GetEquipmentList( IN DWORD& dwCharType, OUT std::set<GCITEMID>& setItemList );
    bool GetEquipemntItem( IN DWORD& dwCharType, IN const GCITEMID dwItemID, OUT KDropItemInfo& kReward );
    bool DoGambleFail( IN DWORD& dwCharType, OUT KDropItemInfo& kItem );
	void GetExposeItem( OUT GCITEMID& dwExposeItemToUser );
    bool SendRKTornadoItemListNot();

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadRewardList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KDropItemInfo>& vecFailReward );
    bool LoadItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setItemList );
    bool LoadRewardRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    bool LoadItemTypeList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, std::set<GCITEMID> >& mapTypeItemList );
    void SetRewardType( IN std::map< DWORD, KTornadoInfo >& mapTotalTornadoInfo, OUT std::map< GCITEMID, DWORD >& mapTotalRewardType );
protected:
    mutable KncCriticalSection              m_csTornado;
    time_t                                  m_tmBegin;           // 닉쿤 토네이도 이벤트 시작 날짜.
    time_t                                  m_tmEnd;             // 닉쿤 토네이도 이벤트 종료 날짜.
    int                                     m_nTornadoVersion;
    GCITEMID                                m_dwTornadoMapItemID;   // 토네이도 말판 ItemID
    GCITEMID                                m_dwTornadoCoinItemID;  // 토네이도 코인 ItemID
    GCITEMID                                m_dwTornadoFailItemID;  // 토네이도 FailItemID
	GCITEMID                                m_dwExposeItemToUser;   // 최초 실행 시 유저에게 보여 줄 아이템
    DWORD                                   m_dwFailItemCount;      // 럭키 ItemCount

    std::map< DWORD, std::map< DWORD, std::set<GCITEMID> > >   m_mapNonFailRewardList; // 꽝아이템 제외 보상리스트.
    std::map< DWORD, KTornadoInfo >                         m_mapTotalTornadoInfo; // 릭쿤 토네이도 전체 정보.
    std::map< GCITEMID, DWORD >                             m_mapTotalRewardType;
};

DefSingletonInline( KRKTornado );
DeclOstmOperatorA( KRKTornado );