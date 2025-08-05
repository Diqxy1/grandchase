#pragma once
#include "Room.h"
#include "RoomFSM.h"
#include "KncSmartPtr.h"
#include <ToString.h>

SmartPointer( KRoomManager );

struct KQuickJoinOption
{
    USHORT                  m_usMaxUser; // 최대 유저.
    USHORT                  m_usRoomID;  // 입장할 RoomID.

    KQuickJoinOption():m_usMaxUser(0),m_usRoomID(0) {}
};

class KRoomManager /*: public NiRefObject*/
{
    DeclToStringW;
public:
    KRoomManager( unsigned short usRoomNum );
    virtual ~KRoomManager(void);

    bool CreateRoom( IN OUT KUser* pkUser,
                     IN     KEVENT_CREATE_ROOM_REQ& kReq,
                     OUT    KEVENT_CREATE_ROOM_ACK& kAck );

    bool JoinRoom( IN OUT   KUser* pkUser,
                   IN       KJoinRoomReqInfo& kReq,
                   OUT      KJoinRoomAckInfo& kAck );

    bool QuickJoinRoom( IN OUT  KUser* pkUser, 
                        IN OUT  KJoinRoomReqInfo& kReq,
                        OUT     KJoinRoomAckInfo& kAck );

    bool ReserveRoomObject( unsigned short usRoomNum );
    void BuildPages( OUT std::map<int, std::vector<KListPage> >& mapCompList,
                     IN std::map<int, std::vector<KSimpleRoomInfo> >& mapRoomList, int nPageSize );

    void BuildDungeonRoomList( OUT std::map<int, std::vector<KListPage> >& mapWaitList,
                               OUT std::map<int, std::vector<KListPage> >& mapFullList,
                               OUT std::map<int, std::vector<KGameModeState> >& mapModeState );

    void BuildFilteredBattleRoomList( OUT std::map<int, std::vector<KListPage> >& mapList_,
                                      IN const KRoomOptions kRoomOptions, IN const char cType);

    void BuildBattleRoomList( OUT std::map<int, std::vector<KListPage> >& mapWaitList,
                              OUT std::map<int, std::vector<KListPage> >& mapFullList );
    
    void BuildGuildBattleRoomList( OUT std::map<int, std::vector<KListPage> >& mapWaitList_, 
        OUT std::map<int, std::vector<KListPage> >& mapFullList_,
        IN const DWORD guildUID_ );
    
    int GetRoomNum() { return m_vecRoom.size(); }

    void DumpInfo( std::wostream& stm_ ) const;

    void Tick();
    bool IncreaseMatchFailCount( IN const USHORT& usRoomID );

public: // debugging을 목적으로 luabind에 등록할 함수들
    KRoomPtr GetRoom( IN unsigned short usRoomID );
    void DeActive( IN unsigned short usRoomID );

    KRoom* GetRoomForLua( IN unsigned short usRoomID );

protected:
    inline bool FindClosedRoom( OUT KRoomPtr& spRoom );
    void DumpRoomList( std::vector<KRoomPtr>& vecRoomList );
    void CheckRoomUserMatching( IN const USHORT& usRoomID, IN const USHORT& usMaxUsers, IN const USHORT& usUsers, IN std::map<DWORD, DWORD>& mapJoinOption, IN OUT USHORT& usOPMaxUser, IN OUT USHORT& usOPRoomID );

protected:

    std::vector<KRoomPtr>   m_vecRoom;
    static FSMclassPtr      ms_spRoomFSM;

    DWORD                   m_dwResultTick; // 070213. kkurrung. Tick Count가 많아지면 GetTick/SetTick을 만든다.
};
