#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "UserPacket.h"
#include "KncTimer.h"

class KLuaManager;

class KTimeDrop
{
    DeclareSingleton( KTimeDrop );
    NiDeclareRootRTTI( KTimeDrop );
    DeclToStringW;

public:
    enum DROP_RECEIVER {
        DR_ALL     = 0,
        DR_PC_BANG = 1,
        DR_NORMAL  = 2,
    };

    KTimeDrop(void);
    ~KTimeDrop(void);

    void Tick();
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpInfo();

    bool IsEventTerm()          { KLocker lock( m_csTimeDrop ); return m_bIsEventTerm; }
    int GetDropInterval()       { KLocker lock( m_csTimeDrop ); return m_nDropInterval; }
    bool GetCurrentDropItem( IN const int& nIdx, IN const bool& bPCBang, OUT std::vector<KItem>& vecItems );

private:
    bool LoadTime( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT time_t& tmTime );
    bool LoadDropItem( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map< int, std::map<USHORT,std::vector<KItem> > >& mapItems );
    bool LoadItem( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<USHORT,std::vector<KItem> >& vecItems );

    void UpdateEventTerm();
    int GetDailyMaxDrop()       { KLocker lock( m_csTimeDrop ); return m_nDailyMaxDrop; }

protected:
    mutable KncCriticalSection  m_csTimeDrop;
    time_t                      m_tmBegin;      // 이벤트 시작시간
    time_t                      m_tmEnd;        // 이벤트 끝시간
    int                         m_nDropInterval;// 드랍 간격(분)
    int                         m_nDailyMaxDrop;// 1일 최대 드랍 횟수
    std::map< int, std::map<USHORT,std::vector<KItem> > > m_mapDropItem;// 회차별 드랍 아이템 map< Index, map<Receiver,Item> >

    KncTimer                    m_kTimer;
    bool                        m_bIsEventTerm; // 이벤트 활성화 여부
};

DefSingletonInline( KTimeDrop );
DeclOstmOperatorA( KTimeDrop );

//=========================================================================================//

class KTimeDropCheck
{
public:
    KTimeDropCheck();
    ~KTimeDropCheck();

    void SetInfo( IN const int& nAccPlayTime, IN const int& nCurrentDropCount );
    void CheckTimeDrop( IN const DWORD& dwUID, IN const std::wstring& strLogin, IN const char cCharType_, IN const bool& bPCBang, IN const bool& bChangeDate );
    void SendUpdateTimeDropToDB( IN const DWORD& dwUID, IN const std::wstring& strLogin, IN const char cCharType_, IN const time_t& tmDate );

    int GetAccPlayTime() { return m_nAccPlayTime; }
    void IncAccPlayTime() { ++m_nAccPlayTime; }
    void RestAccPlayTime() { m_nAccPlayTime = 0; }

    int GetNextDropIndex() { return (m_nDropCount+1); }
    int GetDropCount() { return m_nDropCount; }
    void IncDropCount() { ++m_nDropCount; }
    void ResetDropCount() { m_nDropCount = 0; }

private:
    int         m_nAccPlayTime; // 최초/Drop받은 후의 PlayTime (분)
    int         m_nDropCount; // 드랍아이템 받은 회수
};