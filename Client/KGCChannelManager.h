#pragma once
#include "KNCSingleton.h"
#include "ChannelType.h"
#include "Packet.h"

class KGCChannelManager
{
    DeclareSingleton( KGCChannelManager );

public:
    KGCChannelManager( void );
    ~KGCChannelManager( void );

protected:
    std::vector<KChannelInfo>   m_vecChannelList;
    std::vector<KChannelInfo>   m_vecMatchChannelList;
    DWORD                       m_dwDungeonChannelUID;
    DWORD                       m_dwGetLastMatchChannelUID;
    // 채널 검색을 빠르게 해볼려고 MAP을 HashTable처럼 사용해 봅니다.
    std::map< DWORD, std::pair< std::vector<KChannelInfo>*, int > > m_ChannelUIDHash;

    void ClearChannelList();

public:
    KChannelInfo* GetChannel( DWORD dwChannelUID_ ); //해당 UID를 가진 채널을 찾아서 반환합니다.
    DWORD GetDungeonChannelUID(){ return m_dwDungeonChannelUID; }
    std::vector<KChannelInfo>& GetMatchChannelList(){ return m_vecMatchChannelList; }

protected:
    DWORD           m_dwCurrentChannelUID;
    EChannelType    m_eCurrentChannelType;

public:
    void SetCurrentChannel( DWORD UID_ );
    DWORD GetCurrentChannel() { return m_dwCurrentChannelUID;}
    // 광장에서 미니게임 하려고 만든겁니다. -_-; 딴데서는 쓰면 안되요... uid 넣으세요.. 그냥 
    void SetCurrentChannelType( const EChannelType eCurrentChannelType ) { m_eCurrentChannelType = eCurrentChannelType; }
    EChannelType GetCurrentChannelType(){ return m_eCurrentChannelType; }
    std::wstring GetCurrentChannelName();

    void UpdateChannelList( std::vector<KChannelInfo>& kChannelInfo );

    bool CheckChannel( EChannelType eChannelType ){ return m_eCurrentChannelType == eChannelType; }
    bool IsChannelBonus();
    bool IsMatchChannel();
    DWORD GetPrevMatchChannel() { return m_dwGetLastMatchChannelUID; }
    void InitPrevMatchChannel() { m_dwGetLastMatchChannelUID = 0xFFFFFFFF; }
    bool MoveChannel(DWORD dwChannelUID);
    bool MoveChannelType(int eChannelType);

	bool GoAutoMatch();
	bool GoCustomMatch();

    bool IsAutoMatchChannel();
    bool IsPVPBallanceServer();
    bool IsCreateLimitServer();
};

DefSingletonInline( KGCChannelManager );
