#pragma once
#include "KncSmartPtr.h"
#include "KNCSingleton.h"
#include "Channel.h"

SmartPointer( KUser ); // 전방 선언.
SmartPointer( KChannel ); // 전방 선언.
namespace luabind { class object; } // 전방선언

struct KECN_REPETITION_PROVIDE_REQ;
class KSerBuffer;
class KChannelManager
{
    DeclareSingleton( KChannelManager );
	DeclToStringW;

public: //Functions
    virtual ~KChannelManager(void);

    void SetChannelLobby( IN int nMaxUser );
    void ModifyChannelLobby( IN int nMaxUser );
    void AddChannel( const char* sz_str, int nMaxUser, const luabind::object& objGrade, int nRoomNum, const luabind::object& ModeList, char cType, const luabind::object& obIPRange );

    void Remove( const DWORD dwChannelUID );
    KChannel* Get( const DWORD dwChannelUID );
    KChannelPtr GetChannelSP( const DWORD dwChannelUID );
    KChannel* GetByChannelType( IN const int& nChannelType );

    bool EnterChannel( IN DWORD dwChannelUID, KUser* pkUser );
    void SetBroadCastingTime( unsigned int uiSendChannelList,
                              unsigned int uiSendRoomList,
                              unsigned int uiSendUserList );
    void BuildChannelList();
    void Tick();
    KUserPtr FindUser( const std::wstring& strName );              ///> 유저 검색
    KChannel* FindUserChannel( const std::wstring& strName );
    void DeleteUserChannel( const std::wstring& strName );
    void Send_NotMsg( const wchar_t* szMessage );
    void Send_NotMsgA( const char* szMessage );
    void BroadPacket( KSerBuffer& kbuff, const wchar_t* szDataDesc, bool bSendChannelLobby = true );
    template< class T > void SendToAll( const unsigned short usEventID, const T& data, bool bCompress = false );
    template< class T > void SendToAllState( const unsigned short usEventID_, const T& data_, std::set<int>& setState, bool bCompress_ = true );

    size_t GetNumChannel() const { KLocker lock(m_csChannel); return m_mapChannel.size(); }
    KSerBuffer& GetChannelListData() { return m_kbuffChannelListData; } // 이녀석이 타 쓰레드가 아니라 그냥쓰지만.. 흠...

    // 070213. kkurrung. 전체 메세지
    void QueueingMsg( KSerBuffer& kbuff ) { KLocker lock( m_csMsgQueue ); m_queMsg.push( kbuff ); }
    bool GetMsg( KSerBuffer& kbuff );

    // 070318. kkurrung. 한시간 이벤트를 위한
    void QueueingRepetition( KECN_REPETITION_PROVIDE_REQ& kPacket );
    bool GetRepetition( KECN_REPETITION_PROVIDE_REQ& kPacket );

    void ProcessRepetition();
    bool GetRandomChannel( KChannelPtr& spChannel, int nMinimalUser );

    void InitGameCategoryModeID();
    bool GetModeCategory( IN const int& nModeID, OUT int& nCategory );

protected: //Variables

    mutable KncCriticalSection          m_csChannel;
    std::map<DWORD, KChannelPtr>        m_mapChannel;

    //KEVENT_CHANNEL_LIST_NOT m_kChannelListData;             // 채널리스트 데이터. 전송 주기마다 갱신됨.
    KSerBuffer              m_kbuffChannelListData;
    unsigned int            m_uiLastSendChannelListTick;

    // 070213. kkurrung. 지금은 전체 메세지를 위해서만 사용되지만. 나중에는 perfomer를 상속받아서 동작하는형태가 아름다울것이다.
    mutable KncCriticalSection      m_csMsgQueue;
    std::queue<KSerBuffer>          m_queMsg;

    // 070318. kkurrung. 젠장 이번만이다.. 다음에는 퍼포머의 구조를 답습하자.. 젠젠젠...
    mutable KncCriticalSection                  m_csRepetition;
    std::queue<KECN_REPETITION_PROVIDE_REQ>     m_queRepetition;

    mutable KncCriticalSection      m_csCategoryModeID;
    std::map<int,std::set<int> >    m_setGameCategoryModeID; // map[ Category, set[ModeID] ]

    KChannelManager(void);
public: //Variables
    static unsigned int     ms_uiSendChannelListGap;        // 채널리스트 전송 주기. 스크립트로 읽어들임.
};
template< class T >
void KChannelManager::SendToAll( const unsigned short usEventID_, const T& data_, bool bCompress_ /*= true*/ )
{
    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    KSerializer         ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( bCompress_ ) spEvent->m_kbuff.Compress();

    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    KLocker lock( m_csChannel );
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit )
    {
        mit->second->BroadPacket( kbuff, spEvent->GetEventIDString() );
    }
    
}

template< class T >
void KChannelManager::SendToAllState( const unsigned short usEventID_, const T& data_, std::set<int>& setState, bool bCompress_ /*= true*/ )
{
    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    KSerializer         ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( bCompress_ ) spEvent->m_kbuff.Compress();

    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    KLocker lock( m_csChannel );
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit )
    {
        mit->second->BroadPacketState( kbuff, spEvent->GetEventIDString(), setState );
    }

}
DefSingletonInline( KChannelManager );