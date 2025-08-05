#ifndef _GCCHATMANAGER_H_
#define _GCCHATMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
//#include "define.h"
//#include "KAssert.h"
////#include <string>
////#include <vector>
#include "GCPairPlaySystem.h"
//#include "KNC/UserPacket.h"

struct INVITE_INFO
{
	USHORT usRoomID;
	std::wstring strPasswd;

	INVITE_INFO()
	{
		usRoomID = 0;
		strPasswd = L"";
	}
	INVITE_INFO( const INVITE_INFO& src )
	{
		*this = src;
	}

	const INVITE_INFO& operator = ( const INVITE_INFO& rhs )
	{
		usRoomID = rhs.usRoomID;
		strPasswd = rhs.strPasswd;
		return *this;
	}
};

class KGCChatManager
{
public:
    KGCChatManager( void );
    ~KGCChatManager( void );

private:
    UNUSED_COPY_CONSTRUCTOR( KGCChatManager );
    UNUSED_ASSIGNMENT_OPERATOR( KGCChatManager );

public:
    enum EChatType
    {
        CHAT_TYPE_RESULT    = 0,
        CHAT_TYPE_ALERT     = 1,
        CHAT_TYPE_ADMIN     = 2,
        CHAT_TYPE_NORMAL    = 3,
        CHAT_TYPE_INVITE    = 4,
        CHAT_TYPE_WHISPER   = 5,
		CHAT_TYPE_SIX		= 6,
        CHAT_TYPE_KILLINFO  = 7,
		CHAT_TYPE_AUTO		= 8,
		CHAT_TYPE_COLORCHAT = 9,
        CHAT_TYPE_GUILD     = 10,    //길드챗 기능.. 시즌2 광장 채팅 들어가며 추가
        CHAT_TYPE_SHOUT     = 11,    //외치기 기능...                ''
        CHAT_TYPE_PARTY     = 12,    //파티   기능...                ''        
		CHAT_TYPE_TEAM				= 14,    //zstaiji - 팀 채팅
        CHAT_TYPE_SCREEN_SHOT       = 15,
        CHAT_TYPE_MACRO_COMMUNITY   = 16,
    };
    enum EChatCommand
    {
        CHAT_CMD_NORMAL,    ///< 보통 채팅
        CHAT_CMD_ADMIN,     ///< 운영자 특수 명령
		CHAT_CMD_BROADCAST,	///< 운영자 공지
        CHAT_CMD_WHISPER,   ///< 귓속말 채팅
        CHAT_CMD_DEVCONSOLE,
		CHAT_CMD_COLORCHAT, ///< 칼라채팅
        CHAT_CMD_GUILD, //길드챗 기능.. 시즌2 광장 채팅 들어가며 추가
        CHAT_CMD_SHOUT, //외치기 기능...                ''
        CHAT_CMD_PARTY, //파티   기능...                ''
        CHAT_CMD_NOTICE,    ///< 전광판으로 나오는 운영자공지
		CHAT_CMD_TEAM, //zstaiji - 팀 채팅

#if defined USE_SOCIAL_MOTION
        CHAT_CMD_SOCIAL_MOTION,
#endif
		CHAT_CMD_ERROR,

    };

public:
	void MessageFilter( std::wstring& strMessage );
	void RemoveColorKey( std::wstring& strMessage );
    void RemoveSpaceChar( std::wstring& strMessage );

	void ChatMsgProc( const std::wstring& strNickColor, const std::wstring& strNick, std::wstring& strMsg, char cChatType = KChatData::MSG_COMMON, DWORD dwColor = 0xffffffff );
    void AddChatMsg( const std::wstring& strMsg, std::wstring strReturnMsg) const;
    void AddChatMsg( const std::wstring& strMsg, int iChatType = CHAT_TYPE_NORMAL , DWORD dwColor = 0xffffffff, bool bNoSound = false, std::wstring strReturnMsg = L"" ) const;
	void ClearAllChatMsg();
	void AddChatMsgTo( int iStateID, const std::wstring& strMsg, int iMsgType = CHAT_TYPE_NORMAL,  bool bNoSound = false, std::wstring strReturnMsg = L"", DWORD dwColor = 0xffffffff ) const;
    void AddSystemMsg( int iIndex, const std::wstring& strMsg, int iChatType = CHAT_TYPE_RESULT ) const;
    void AddMacroCommunityMsg( DWORD dwUID, const std::wstring& strMsg, int iSoundID, DWORD dwColor ) const;
	void AddWhisperID( const std::wstring& strID );
	bool GetWhisperFilter() { return m_bWhisperFilter; }
	void SetWhisperFilter( bool WhisperFilter_ ) { m_bWhisperFilter = WhisperFilter_; }

    inline bool IsWhisperMode( void ) const { return m_bWhisperMode; }

    const std::wstring& OnWhisperInputStart( void );
    const std::wstring& OnWhisperScrollUp( void );
    const std::wstring& OnWhisperScrollDown( void );

    D3DCOLOR GetColorFromTable( int iChatType, DWORD dwColor ) const;
    EChatCommand AnalyzeChatCommand( const std::wstring& strCommand );
    EChatCommand FindChatType( const std::wstring& strCommand );
    

    GCPairPlaySystem::PPS_CHAT_TYPE GetChatLogType();
    void BindToLua();

	void SetCommend( int iCommend ) { m_eCmd = static_cast<EChatCommand>(iCommend); }
	int GetCommend( ) { return static_cast<int>(m_eCmd); }
    void SetChatUserState( KEVENT_BAD_USER_INFO_NOT& kEventInfo );
    char GetCharUserGrade();
    std::pair< int , time_t> GetRestrictData();
    bool CheckChatBlockUser();
    bool GetSplitExpCommand(int iIndex, std::wstring strCommand, OUT std::wstring& strNumber );
public:
    char                        m_cCurrentUserGrade;
    std::pair< int , time_t>    m_RestrictData;
private:
    bool                        m_bWhisperMode;
    int                         m_iWhisperIdx;
    std::vector<std::wstring>   m_vecWhisperID;
    std::wstring                m_strCommand;
	bool						m_bWhisperFilter;
	EChatCommand				m_eCmd;
};

extern KGCChatManager* g_pkChatManager;

#endif // _GCCHATMANAGER_H_
