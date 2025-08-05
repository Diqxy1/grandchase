#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"
#include "CommonPacket.h"

class KLuaManager;
struct KEVENT_CHARISMAS_EVENT_NOT;

class KCharismasPresent
{
    DeclareSingleton( KCharismasPresent );
    NiDeclareRootRTTI( KCharismasPresent );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KCharismasPresent(void);
    ~KCharismasPresent(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsRun(); // 이벤트 진행중인지 체크.
    bool GetItemConfig( IN KDropItemInfo kDropItemInfo, OUT DWORD& dwSledgeUseCnt, OUT DWORD& dwBonusPoint );
    bool GetDonationInfo( IN const DWORD dwSledgeUseCnt, OUT DWORD& dwBonusPoint );
    bool GetGradeRewardList( IN const DWORD dwGrade, OUT VEC_REWARD& vecReward );
    DWORD GetVersion(){ KLocker lock( m_csCharismasPresent ); return m_dwVersion; }; 
    GCITEMID GetSledgeItem(){ KLocker lock( m_csCharismasPresent ); return m_SledgeItem; }; 
    DWORD GetMaxGrade(){ KLocker lock( m_csCharismasPresent ); return m_dwMaxGrade; }; 
    DWORD GetMaxPoint(){ KLocker lock( m_csCharismasPresent ); return m_dwMaxPoint; }; 
    bool GetLoopEnable(){ KLocker lock( m_csCharismasPresent ); return m_bLoopEnable; }; 
    bool GetPointGrade( IN const DWORD dwBonusPoint, OUT DWORD& dwGrade );
    void GetCharismasPresentInfo( OUT KEVENT_CHARISMAS_EVENT_NOT& kPacket );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadPresentItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<KDropItemInfo,std::pair<DWORD,DWORD>>& mapPresentItemInfo );
    bool LoadDropItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT KDropItemInfo& kDropItemInfo );
    bool LoadItemConfig( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::pair<DWORD,DWORD>& prData );
    bool LoadDonationInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,DWORD>& mapDonationInfo );
    bool LoadGradeRewardList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,VEC_REWARD>& mapGradeRewardList );
    bool LoadGradeDescInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc );

protected:
    mutable KncCriticalSection                  m_csCharismasPresent;
    time_t                                      m_tmBegin;          // 이벤트 시작 날짜.
    time_t                                      m_tmEnd;            // 이벤트 종료 날짜.
    GCITEMID                                    m_SledgeItem;
    DWORD                                       m_dwVersion;
    DWORD                                       m_dwMaxGrade;
    DWORD                                       m_dwMaxPoint;
    bool                                        m_bLoopEnable;
    std::map<KDropItemInfo, std::pair<DWORD,DWORD>> m_mapPresentItemInfo;
    std::map<DWORD,DWORD>                       m_mapDonationInfo;
    std::map<DWORD,VEC_REWARD>                  m_mapGradeRewardList;
    std::map<DWORD,std::pair<DWORD,DWORD>>      m_mapGradeDesc;
};

DefSingletonInline( KCharismasPresent );
DeclOstmOperatorA( KCharismasPresent );

//=========================================================================================//

class KCharismasUserData
{
public:
    KCharismasUserData();
    ~KCharismasUserData();
    bool IsLoaded() { return m_bLoaded; };
    void SetUserCharismasEventData( IN const DWORD dwGrade, IN const DWORD& dwBonusPoint, IN const DWORD& dwMaxPoint, IN const bool& bInitEnable );
    void GetUserCharismasEventData( OUT DWORD& dwGrade, OUT DWORD& dwBonusPoint );
    bool CheckUserGrade( IN const DWORD dwGrade );

protected:
    bool                                        m_bLoaded;
    DWORD                                       m_dwGrade;
    DWORD                                       m_dwBonusPoint;
};