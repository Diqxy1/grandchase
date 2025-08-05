#pragma once
#include <map>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "Kairo.h"

class KLuaManager;
struct KItemUnit;

struct KCapsuleInfo
{
    int                     m_nIndex;       // 캡슐아이템 번호
    int                     m_nPriceType;   // 화폐 타입( GP, CRYSTAL )
    int                     m_nPrice;       // 가격
    std::pair<int,int>      m_prLevelRange; // 아이템 레벨대
    KLottery                m_ltrGrade;     // 등급별 확률
    Kairo                   m_kairoBase;    // 캡슐아이템(기본)
    Kairo                   m_kairoRare;    // 캡슐아이템(레어)
    Kairo                   m_kairoEvent;   // 캡슐아이템(이벤트)
};

// pair<캐릭터ID,아이템종류>,vector<캡슐정보>
typedef std::map< std::pair<int,int>, std::vector<KCapsuleInfo> > CAPSULE_INFO_LIST;

class KGPCapsuleManager
{
    DeclareSingleton( KGPCapsuleManager );
    NiDeclareRootRTTI( KGPCapsuleManager );
    DeclToStringW;
public:
    enum ITEM_GRADE
    {
        IG_BASE,            // 기본 아이템
        IG_RARE,            // 레어 아이템
        IG_EVENT,           // 이벤트 아이템
    };

    enum EItemKind {
        EIK_WEAPON = 0,     // 무기
        EIK_ARMOR,          // 방어구
        EIK_ACCESSORIES,    // 악세사리
        EIK_RUNE,           // 강화룬
    };

    KGPCapsuleManager(void);
    ~KGPCapsuleManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetGPCapsuleEnable(){ KLocker lock(m_csCapsule); return m_bGPCapsuleEnable; }
    void GetCapsuleListNotice( OUT KEVENT_CAPSULE_LIST_NOT& mapCapsuleList );
    bool GetCurrentCapsuleInfo( IN const int nCharType, IN const int nItemKind, IN const int nIndex, IN const int nPrice,  OUT KCapsuleInfo& kCapsuleInfo );
    bool GetItemGrade( IN const KCapsuleInfo& kCapsuleInfo, OUT int& nResult );
    bool OpenCapsule( IN const KCapsuleInfo& kCapsuleInfo, IN const int nItemGrade, OUT KDropItemInfo& kItem );
    bool DoGamble( IN const Kairo& kairo, OUT KDropItemInfo& kItem );
    void DumpCapsule(); // 전체 캡슐정보 출력
    void PrintCapsuleInfo( IN const int nChar, const int nKind, const int nIndex ); // 키값으로 검색
    void PrintCapsuleItem( IN const GCITEMID dwItemID ); // ItemID로 검색

protected:
    bool LoadGradeRatio( IN KLuaManager& kLuaMng, OUT KLottery& ltr );
    bool LoadCapsuleRatio( IN KLuaManager& kLuaMng, IN const int nGrade, OUT std::vector< std::pair<KDropItemInfo,int> >& vecItemList, OUT Kairo& kairo );

protected:
    mutable KncCriticalSection  m_csCapsule;
    bool                        m_bGPCapsuleEnable;         // GP캡슐 사용여부
    CAPSULE_INFO_LIST           m_mapCapsuleInfoList;       // 서버 정보
    KEVENT_CAPSULE_LIST_NOT     m_mapCapsuleListNotice;     // 클라이언트 정보
};

DefSingletonInline( KGPCapsuleManager );
DeclOstmOperatorA( KGPCapsuleManager );