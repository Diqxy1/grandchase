#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Kairo.h"

class KLuaManager;

struct KGachaLotteryInfo
{
    char                        m_cCharType;
    Kairo                       m_kairoRewardType;
    Kairo                       m_kairoSuccessReward;
    Kairo                       m_kairoFailReward;
    std::map<GCITEMID, DWORD>   m_mapGradeInfo;
};

class KGachaLottery
{
    DeclareSingleton( KGachaLottery );
    NiDeclareRootRTTI( KGachaLottery );
    DeclToStringW;

public:
    enum {
        SUCCESS = 0, // 성공복권
        FAILED  = 1, // 실패복권
    };

    KGachaLottery(void);
    ~KGachaLottery(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool DoGambleRewardType( IN const char& cCharType, OUT DWORD& dwRewardType );
    bool DoGambleItemInfo( IN const char& cCharType, IN const DWORD& dwType, OUT KDropItemInfo& kItem );
    bool IsRun(); // 이벤트 진행중인지 체크.
    void GetGachaLotteryItemInfo( OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaSuccessList, std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaFailedList );
    GCITEMID GetGachaLotteryItemID() { KLocker lock( m_csGachaLottery ); return m_GachaLotteryItemID; }
    DWORD GetGachaLotteryVersion() { KLocker lock( m_csGachaLottery ); return m_dwVersion; }
    DWORD GetGachaLotteryNoticeEnable() { KLocker lock( m_csGachaLottery ); return m_bNoticeEnable; }
    bool CheckItemGrade( IN const char& cCharType, IN GCITEMID& DisplayItemID, OUT std::wstring& wstrMsg );
    void SendGachaLotteryMsg( IN std::wstring& wstrData );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadGachaLotteryInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char,KGachaLotteryInfo>& mapGachaLotteryInfo, OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaSuccessList, OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaFailedList );
    bool LoadRewardTypeRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kKairo );
    bool LoadRewardListRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kKairo, OUT std::vector<std::pair<GCITEMID,int>>& vecItemInfo );
    bool LoadGradeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, DWORD>& mapGradeInfo );
    bool LoadGradeDesc( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, std::wstring>& mapGradeDescInfo );
    bool GetGradeStringDesc( IN const DWORD& dwGrade, OUT std::wstring& wstrMsg );

protected:
    mutable KncCriticalSection              m_csGachaLottery;
    bool                                    m_bNoticeEnable;     // 가챠 공지 설정 여부.
    time_t                                  m_tmBegin;           // 이벤트 시작 날짜.
    time_t                                  m_tmEnd;             // 이벤트 종료 날짜.
    DWORD                                   m_dwVersion;
    GCITEMID                                m_GachaLotteryItemID;
    std::map<char, KGachaLotteryInfo>       m_mapGachaLotteryInfo;
    std::map< char, std::vector< std::pair< GCITEMID, int > > >        m_mapGachaSuccessList;
    std::map< char, std::vector< std::pair< GCITEMID, int > > >        m_mapGachaFailedList;
    std::map<DWORD, std::wstring>           m_mapGradeDescInfo;
};

DefSingletonInline( KGachaLottery );
DeclOstmOperatorA( KGachaLottery );