#pragma once
#include <ToString.h>
#include <RTTI.H>
#include <KNCSingleton.h>
#include <queue>
#include <Thread/Locker.h>
#include "UserPacket.h"

class KSignBoard
{
    DeclareSingleton( KSignBoard );
    NiDeclareRootRTTI( KSignBoard );
    DeclToStringW;

public:
    KSignBoard(void);
    ~KSignBoard(void);
    
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript ); // 전광판 정보 InitSignBoardInfo.lua 
    void Tick();
    DWORD GetCapacity();
    DWORD GetSize();
    bool IsCorrectScope( IN const int nType, IN const GCITEMID itemID );
    DWORD QueueingData( KSignBoardData& kData );
    DWORD QueueingAdminData( KSignBoardData& kData ); // tslayer
    void SendDataToSpecificPlayer(DWORD dwUID, KSignBoardData& kData);
    void LuaMsg( IN const char* szMsg );
    void ClearLuaMsg();

protected:

    DWORD GetTick() { KLocker lock(m_csSignBoard); return m_dwLastFlushTick; };
    void SetTick()  { KLocker lock(m_csSignBoard); m_dwLastFlushTick = ::GetTickCount(); };
    void LoudSignBoard();
    bool GetData( KSignBoardData& kData );
    bool IsLoudTick();

    mutable KncCriticalSection          m_csSignBoard;
    DWORD                               m_dwLastFlushTick;
    DWORD                               m_dwCurrentFlushGap;
    DWORD                               m_dwFlushGap;
    DWORD                               m_dwAdminFlushGap;
    DWORD                               m_dwMaxCapacity;        // 최대 전광판 개수?
    std::map<int,std::vector<GCITEMID> >   m_mapScopeData;
    std::queue< KSignBoardData >        m_queSignBoardData;
    std::queue< KSignBoardData >        m_queSignBoardAdminData;

};
DefSingletonInline( KSignBoard );
DeclOstmOperatorA( KSignBoard );
