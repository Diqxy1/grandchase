#pragma once
#include "SobManager.h"
#include "RoomManager.h"

SmartPointer( KUser ); // 전방 선언.
SmartPointer( KChannel );
class KChannel : public KSimObject, protected KSobManager<KUserPtr>
{
    DeclToStringW;
    NiDeclareRTTI;

public:
#include "ChannelType.h"

    KChannel();
    virtual ~KChannel();

    bool Enter( KUserPtr::element_type& pkUser );
    bool Leave( KUserPtr::element_type& pkUser );
    size_t GetCount() { return m_mapSob.size(); }   // KSobManager<> 의 함수와 똑같음
    bool ReserveRoomObject( unsigned short usRoomNum );

    const std::wstring& GetChannelName() const          { KLocker lock( m_csChannelInfo ); return m_strChannelName; }
    void SetChannelName(const std::wstring& strName_)   { KLocker lock( m_csChannelInfo ); m_strChannelName = strName_; }
    void SetType( char cType )                          { KLocker lock( m_csChannelInfo ); m_cType = cType; }
    char GetType()                                      { KLocker lock( m_csChannelInfo ); return m_cType; }
    void SetModeList( std::vector<int>& vecModeList )   { KLocker lock( m_csChannelInfo ); m_vecModeList = vecModeList; }
    void GetModeList( std::vector<int>& vecModeList )   { KLocker lock( m_csChannelInfo ); vecModeList = m_vecModeList; }
    void SetMaxUsers( int nMaxUsers )                   { KLocker lock( m_csChannelInfo ); m_usMaxUsers = (unsigned short)nMaxUsers; }
    unsigned short GetMaxUser()                         { KLocker lock( m_csChannelInfo ); return m_usMaxUsers; }
    void SetRoomNum( int nRoomNum );
    void SetIPRange( std::pair<DWORD,DWORD>& prIPRange ){ KLocker lock( m_csChannelInfo ); m_prIPRange =prIPRange; }
    bool IsCorrectIPRange( DWORD dwIP );

    void SetGradeRange( std::pair<int,int> prGradeRange ){ m_prGradeRange = prGradeRange; };
    std::pair<int,int> GetGrade(){ return m_prGradeRange; };
    void ModifyChannel( const char* sz_str, int nMaxUser, const luabind::object&  objGrade, int nRoomNum, const luabind::object& ModeListv, char cType );
    void GetFilteredPvPRoomList(char cType, KRoomOptions kRoomOptions, OUT KPagePvPInfo& kRoomPage_);
    bool Send_ChatMsg( const KChatData& kNot );
    void BroadPacket( KSerBuffer& kbuff, const wchar_t* szDataDesc );
    void BroadPacketState( KSerBuffer& kbuff, const wchar_t* szDataDesc, std::set<int>& setState );
    void BroadCasting();
    
    // 멤버변수를 인자로 받기도 하지만 더이상 최적화 말자. 외부(KUser)에서 BuildRoomList를 사용할 수도 있다.
    void BuildUserList( OUT std::vector<KListPage>& vecLobbyUserList,
                        OUT std::vector<KListPage>& vecLobbyFullUserList );
    void BuildRoomList( OUT std::map<int, std::vector<KListPage> >& mapWaitList,
                        OUT std::map<int, std::vector<KListPage> >& mapFullList,
                        OUT std::map<int, std::vector<KGameModeState> >& mapModeState );

    void GetCorrectRoomList( bool bWait, int nType, int nDifficult, int nPage, OUT KPageInfo& kRoomPage );
    void GetRoomList( IN std::map<int, std::vector<KListPage> >& mapRoomList, int nType, int nDifficult, int nPage, OUT KPageInfo& kRoomPage );
    void GetUserList( IN int nPage, OUT KPageInfo& kPageUser );

    void GetInfo( OUT KChannelInfo& kInfo );
    bool CheckEnteranceGrade( char cGrade );
    bool CheckGuildEnteranceGrade( const int cGrade );

    KUserPtr FindUser( const std::wstring& strName );              ///> 유저 검색
    void DeleteUser( const IN std::wstring& strName_ );

    // 070318. kkurrung. 한시간 이벤트용.(룸에 있는 유저를 얻어낸다. STATE_ROOM, STATE_PLAYING )
    bool GetRepetitionUserInfo( KUserPtr& spUser );

    void CompressUserList( IN std::vector<KInviteUserInfo>& vecUserList, OUT std::vector<KListPage>& vecUserListPage );
    void GetAllUserList( IN int nPage, OUT KPageInfo& kPageUser );
    bool IsPvPChannel();
    void GetGameModeState( IN const std::set<int>& setModeList, OUT std::map<int, std::vector<KGameModeState> >& mapGameModeState );

    void GetCorrectGuildRoomList( bool bWait_, int nType_, int nDifficult_, int nPage_, OUT KPageInfo& kRoomPage_, IN DWORD guildUID_ );

public: // for luabind.
    KRoomManager* GetRoomManager() const { return m_spRoomManager.get(); }

protected:
    mutable KncCriticalSection      m_csChannelInfo;    // 채널 정보를 보호하기 위한 크리티컬 섹션
    std::wstring                    m_strChannelName;   // 채널 이름 변경시 보호하기 위해서 이름 데이터 넣음.
    unsigned short                  m_usMaxUsers;       // 최대
    std::vector<int>                m_vecModeList;      // *이 채널에서 플레이 가능한 게임 모드
    char                            m_cType;
    std::pair<int,int>              m_prGradeRange;
    
    unsigned int                    m_uiLastSendRoomList;
    unsigned int                    m_uiLastSendUserList;
    std::pair<DWORD,DWORD>          m_prIPRange;

public:
    KRoomManagerPtr                 m_spRoomManager;
    static KChannelPtr              ms_spChannelLobby;

    // channel manager, user 등에서 이를 참조한다.
    static unsigned int             ms_uiSendRoomListGap;
    static unsigned int             ms_uiSendUserListGap;

    std::map<int, std::vector<KListPage> >  m_mapWaitList;
    std::map<int, std::vector<KListPage> >  m_mapFullList;
    std::map<int, std::vector<KGameModeState> > m_mapModeState;
    std::vector<KGameModeState> vecDummyStatus;

    std::vector<KListPage>          m_vecLobbyUserList;     // 초대가능한 유저목록(초대거절인 유저는 제외됨)
    std::vector<KListPage>          m_vecLobbyFullUserList; // 로비대기중인 모든 유저목록
};

DeclOstmOperatorW( KChannel );
