#pragma once
#include <RTTI.H>
#include <ToString.h>
#include "KncSmartPtr.h"
#include <KNCSingleton.h>
#include <Thread/Locker.h>

#include "CenterPacket.h"
#include "Lua/KLuaManager.h"
#include "KncTimer.h"

class KSquareSetInfo
{
public:
    std::string     m_strName; // 광장 이름
    DWORD           m_dwMaxUserNum; 
    SOCKADDR_IN     m_Address;
    int             m_nType; // 광장 타입 , 
    std::string     m_strServerIP; // 서버 IP

    //void SetInfo( const char* szName, DWORD dwMaxNum );
    void SetInfo( const char* szName_, DWORD dwMaxNum_, int nType_ );
    void SetAddress( const char* szAddress, unsigned short usPort );
};

SmartPointer(KCnSquare);
class KCnSquareManager
{
    DeclareSingleton( KCnSquareManager );
    NiDeclareRootRTTI( KCnSquareManager );
    DeclToStringW;
public:
    KCnSquareManager();
    virtual ~KCnSquareManager();

private:
    int                             m_iLastUID;
    std::map<DWORD,DWORD>           m_mapUserInSquare;       // <UserUID, SquareUID>
    std::map<DWORD,KCnSquarePtr>    m_mapSquares;            // 전체 광장 리스트

    typedef std::map< DWORD, std::map< int, DWORD > > GuildContainer;
    typedef std::map< int, DWORD >  GuildIndexContainer;
    std::map< DWORD, std::map< int, DWORD > > m_mapGuildSquare;  // 길드 광장 리스트 < GuildUID, < GuildSquareIndex, SquareUID > >
    std::vector<KCnSquarePtr>       m_vecSquares;
    mutable KncCriticalSection      m_csSquares;

    std::vector<KSquareInfo>        m_vecBuildList;

    std::vector<KSquareSetInfo>     m_vecGuildSquareSetInfo;
    KncTimer                        m_kTimer;

public:
    bool LoadScript( IN std::shared_ptr<lua_State>& sptLua_ );
    bool AddSquare( KSquareSetInfo& kInfo_, const char* szTableName ); //, DWORD dwGuildID = 0 ); // 광장 추가 스크립트..
    KCnSquarePtr AddSquareWithGet( KSquareSetInfo& kInfo_, const char* szTableName ); //, DWORD dwGuildID = 0 ); // 광장 추가 스크립트..

    void AddGuildSquareSetInfo( IN KSquareSetInfo& kInfo_ );
    
    bool EnterSquare( IN KSquareUserInfo& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket );
    bool LeaveSquare( IN DWORD dwUID );

    bool EnterMinigame( IN DWORD dwUserUID_ );
    bool ExitMinigame( IN DWORD dwUserUID_, IN KCenterPtr spCenter_ );
    void GetSquareList( OUT std::vector<KSquareInfo>& vecSquareList );
    void GetGuildSquareList( IN DWORD dwGuildUID, IN OUT std::vector<KSquareInfo>& vecSquareList );
    KCnSquarePtr GetSquare( IN DWORD dwSquareID );

    void BuildSquareList( IN bool bInit );
    void RemoveSquare( IN KCnSquarePtr spSquare );

    bool EnterGuildSquare( IN KECN_ENTER_GUILD_SQUARE_REQ& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_GUILD_SQUARE_ACK& kPacket );
    void DumpSquareList();
    void Tick();
    bool ForceExitMinigame( IN DWORD dwUserUID_ );
    void UserPositionSync( KSquarePos& kPacket, std::vector<KPosUID>& vecPosition );

private:
    bool AddUserToSquare( IN KCnSquarePtr spSquare, IN KSquareUserInfo& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket );
    KCnSquarePtr GetUserSquare( IN DWORD dwUserUID );
    KSquareInfo GetBlankGuildSquareInfo( IN const DWORD dwGuildUID, IN const int iIndex ) const;
};

DefSingletonInline( KCnSquareManager );
DeclOstmOperatorA( KCnSquareManager );