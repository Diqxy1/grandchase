#ifndef _NMCOCLIENTOBJECT_H_
#define _NMCOCLIENTOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NATION_JAPAN

#include "NMDataType.h"

#ifdef _USE_STLPORT

#include <functional>

typedef std::vector< FriendInfo >::value_type	FriendInfoValueType;

struct FriendInfoGreater : public std::binary_function< FriendInfoValueType, FriendInfoValueType, bool > 
{
	bool operator() ( std::vector< FriendInfo >::value_type fi1, std::vector< FriendInfo >::value_type fi2 );
};

#else

bool FriendInfoGreater( FriendInfo& fi1, FriendInfo& fi2 );

#endif

class CNMCOClientObject	// This is singleton class!
{
//protected:
//	CNMCOClientObject();

// 2006.12.14 wony
// 원래는 생성자가 protected이고 GetInstance 함수를 이용해서만 객체를 사용할 수 있었는데..
// singleton 객체라 프로세스가 죽지 않는 현상이 있어서 수정합니다.
public:
	CNMCOClientObject();

public:
	~CNMCOClientObject();

public:
	// For	Singleton
	static CNMCOClientObject&	GetInstance( void );

protected:
	BYTE*		m_pBuf;
	SInt32		m_nBufSize;

public:
	// Main Members
	void		Init( UInt32 uMode, UInt32 uGameCode, LPSTR pszDomain /* 32 */, UInt32 uSupportFlag = kSupportFlag_FromGameCode );

	const char* GetCommonPath( LPSTR szPath /* MAX_PATH */ );
	const char* GetAvatarPath( LPSTR szPath /* MAX_PATH */ );
	const char* GetMessengerPath( LPSTR szPath /* MAX_PATH */ );

	BOOL		DownloadPatcher( void );
	BOOL		ExecutePatcher( const char* szUrl /* 256 */, const char* szPatchDir /* 256 */, const char* szExecCmdLine /* 256 */, BOOL bWaitParentExit = TRUE, BOOL bWaitPatchEnd = FALSE );
	BOOL		ExecuteLauncher( UInt32 uGameCode, UInt32 uLaunchType, const char* szArgument = NULL, BOOL bWaitParent = FALSE );
	BOOL		PatchNMCOSrv( const char* szDomain /* 32 */ );

	void		StartStandAloneMessenger( BOOL bStealthMode = FALSE );

	UInt64		GetDllVersion( void );
	UInt64		GetNMCOSrvVersion( void );
	void		GetDomain( char* szDomain );

	void		RegisterCallbackMessage( HWND hWnd, UInt32 uMessage );
	void		ResetCallbackMessage( HWND hWnd = NULL );

	BOOL		Login( LPCSTR szLocalId /* 64 */, LPCSTR szLoginId /* 64 */, LPCSTR szPassword /* 32 */, BOOL bMergeList, UInt64 uMsgVersion = 0 );
	BOOL		Logout( void );
	BOOL		ReInstallConMsg( void );
	UInt32		GetServerIndex( LPCSTR szLoginId /* 64 */ );

	BOOL		IsOnline( UInt32 uStatus );
	BOOL		IsOnline( void );

	BOOL		AddNewFriend( LPCSTR szFriendID /* 64 */, LPCSTR szAlias /* 32 */, LPCSTR szMessage /* 256 */);
	void		ConfirmAddFriend( LPCSTR szConfirmId /* 64 */, BOOL bAccept, LPCSTR pszAlias = NULL /* 32 */);
	void		ChangeMyInfo( LPCSTR pszNickName = NULL, LPCSTR pszScreenName = NULL /* 64 */, LPCSTR pszPosition = NULL /* 256 */, UInt32 uStatus = 0xFFFFFFFF, UInt32 uEmotion = 0xFFFFFFFF );
	BOOL		AddNewCategory( LPCSTR szNewCategoryName /* 32 */);
	BOOL		ChangeCategoryName( LPCSTR szOldCategoryName /* 32 */, LPCSTR szNewCategoryName /* 32 */);
	BOOL		DeleteCategory( const char* szCategoryName /* 32 */);
	BOOL		MoveUserCategory( UInt64 uIdCode, LPCSTR szFromCategory /* 32 */, LPCSTR szToCategory /* 32 */);

	BOOL		BlockUser( UInt64 uIdCode, BOOL bBlock = TRUE );
	BOOL		DeleteUser( UInt64 uIdCode, BOOL bDelete = TRUE );
	BOOL		SetUserToBlackList( LPCSTR szLoginId, BOOL bSet, LPCSTR szMemo );
	BOOL		ChangeUserAlias( UInt64 uIdCode, LPCSTR szAlias /* 32 */);

	BOOL		CreateChatSession( UInt64 uIdCode );
	BOOL		CreateMultiChatSession( UInt64* aIdCode, UInt32 uCnt );
	BOOL		CreateFileUploadSession( UInt64 uIdCode, UInt32 uFileCnt, const char* aFileName );
	BOOL		CreateWBSession( UInt64 uIdCode, UInt32 uSerialKey );

	BOOL		FindUser( FindUserRequestInfo& findUserRequestInfo );

	BOOL		SendNoteMsg( NoteInfo& noteInfo );
	BOOL		SendNoteExMsg( NoteInfoEx& noteInfoEx );
	// codeDomain = 1 : korean
	BOOL		SendAccuseMsg( UInt32 uType , UInt32 uSubType , UInt32 uAccuserDomain , const char* szAccuserId , UInt32 uAccusedDomain , const char* szAccusedId , const char* szReason , const char* szEvidence , UInt32 uGameCode = 0 );
	BOOL		GetFindUserResult( FindUserInfo* aFindUserInfo, UInt32& uListSize, UInt32 uToken );
	BOOL		DeleteFindUserResult( UInt32 uToken );
	UInt32		GetUnreadNoteCount( void );
	BOOL		GetNoteBox( UInt32 uBoxType, NoteInfo* aNoteList, UInt32& uListSize, BOOL bTrimServerExt = FALSE, UInt32 uSerialNo = 0xFFFFFFFF );
	BOOL		ProcessNote( UInt32 uSerialNo, UInt32 uOperation );

	BOOL		GetServerMessage( CustomMessageData* pMessage );
	BOOL		GetAddFriendConfirmMessage( CustomAddFriendConfirmData* pMessage, BOOL bTrimServerExt = FALSE );

	BOOL		GetFriendList( FriendsCategoryList& fcList, BOOL bTrimServerExt = FALSE );
	BOOL		GetBlackList( FriendList& frdList, BOOL bTrimServerExt = FALSE );
	BOOL		GetMyClubList( ClubInfoList& clubList );

	UInt64		GetIdCodeFromIdx( UInt32 uIdx );
	BOOL		GetFriendInfo( UInt64 uIdCode, FriendInfo& frdInfo, BOOL bTrimServerExt = FALSE );
	BOOL		GetMyLoginId( LPSTR pszLoginId /* 32 */, BOOL bTrimServerExt = FALSE );
	BOOL		GetMyRealName( LPSTR pszRealName /* 64 */);
	BOOL		GetMyNickName( LPSTR pszNickName /* 32 */);
	BOOL		GetMyScreenName( LPSTR pszScreenName /* 64 */);
	BOOL		GetMyPosition( LPSTR pszPosition /* 256 */);
	SInt32		GetMyStatus( void );
	UInt64		GetMyIdCode( void );
	UInt32		GetMyOID( void );
	UInt32		GetMyAge( void );
	UInt32		GetMySex( void );
	UInt32		GetMyAreaCode( void );
	UInt32		GetMySchoolCode( void );
	SYSTEMTIME	GetMyBirthDay( void );
	BOOL		GetMyAvatarInfo( AvatarInfo& aiInfo );
	UInt32		GetMyEmotion( void );
	void		GetMySessionKey( UInt64& uSessionKeyHigh, UInt64& uSessionKeyLow );
	UInt32		GetMyMaskInfo( void );

	BOOL		SendRefreshMessage( UInt32 uRefreshType );

	BOOL		GetConfig( UInt32 uConfigType, char* szValueOut /* 256 */);
	BOOL		SetConfig( UInt32 uConfigType, const char* szValue /* 256 */);

	BOOL		GetSessionVal( const char* szValName /* 32 */, char* szValueOut /* 256 */);
	BOOL		SetSessionVal( const char* szValName /* 32 */, const char* szValue /* 256 */, BOOL bAttachToProcess = FALSE );

	BOOL		GetUserInfo( const char* szLoginId, FriendInfo& frdInfo, BOOL bTrimServerExt = FALSE );

	BOOL		GetRequestSessionMessage( CustomRequestSessionData* pMessage, BOOL bTrimServerExt = FALSE );
	BOOL		AcceptSession( UInt32 uDataKey, BOOL bAccept );

	void		GetAvatarFullImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo, UInt32& uOffsetX, UInt32& uOffsetY );
	void		GetAvatarHeadImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo );
	void		GetAvatarIconImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo );
	const char* GetAvatarFileName( UInt32 uItemNo, UInt32 uFileNo, char* szFileNameOut );

	UInt32		DownloadAvatarImage( UInt32 uItemNo, UInt32 uFileNo, HWND hCallbackWnd, UInt32 uCallbackMsg, BOOL bReload = FALSE );

	const char* GetBannerFileName( const char* szBannerUrl, char* szFileNameOut );
	UInt32		DownloadBannerImage( const char* szBannerUrl, HWND hCallbackWnd, UInt32 uCallbackMsg, BOOL bReload );

	BOOL		GetRecommendGameList( GameInfoList&	gameList );
	BOOL		GetMyGameList( GameInfoList& gameList );
	UInt32		GetMyGameListSize( void );
	BOOL		GetGameGroupList( GameGroupList& ggList );

	BOOL		AddToMyGame( UInt32 uGameCode, UInt32 uIndex );
	BOOL		DeleteFromMyGame( UInt32 uGameCode );
	BOOL		MoveMyGameItem( UInt32 uGameCode, UInt32 uIndex );

	void		OpenDlg( UInt32 uDlgType );

	BOOL		GetURL( UInt32 uType /* kURLType_XXX */, UInt32 uIndex, char* szUrlOut /* INTERNET_MAX_URL_LENGTH */, UInt32 uParam, BOOL bUseLoginDirect, LPVOID pParam, BOOL bPopup );
	BOOL		GetLoginDirectURL( const char* szUrl /* INTERNET_MAX_URL_LENGTH */, char* szUrlOut /* INTERNET_MAX_URL_LENGTH */, BOOL bPopup );
};

#endif // NATION_JAPAN

#endif // _NMCOCLIENTOBJECT_H_