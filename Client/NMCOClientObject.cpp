#include "stdafx.h"
#ifdef NATION_JAPAN

//




#include "NMCOClientObject.h"

#ifdef _DEBUG
	#pragma comment( lib, "nmco2jp_d.lib" )
#else
	#pragma comment( lib, "nmco2jp.lib" )
#endif

#ifdef _USE_STLPORT

bool FriendInfoGreater::operator () ( FriendInfoValueType fi1, FriendInfoValueType fi2 )
{
	BOOL bOnline1 = TRUE;//CNMCOClientObject::GetInstance().IsOnline( fi1.uStatus );
	BOOL bOnline2 = TRUE;//CNMCOClientObject::GetInstance().IsOnline( fi2.uStatus );

	if ( bOnline1 == bOnline2 )
	{
		return ::_tcscmp( fi1.szScreenName , fi2.szScreenName ) < 0;
	}
	else if ( bOnline1 == TRUE && bOnline2 == FALSE )
	{
		return true;
	}
	return false;
}

#else

bool FriendInfoGreater( FriendInfo& fi1, FriendInfo& fi2 )
{
	BOOL bOnline1 = TRUE;//CNMCOClientObject::GetInstance().IsOnline( fi1.uStatus );
	BOOL bOnline2 = TRUE;//CNMCOClientObject::GetInstance().IsOnline( fi2.uStatus );

	if ( bOnline1 == bOnline2 )
	{
		return ::_tcscmp( fi1.szScreenName , fi2.szScreenName ) < 0;
	}
	else if ( bOnline1 == TRUE && bOnline2 == FALSE )
	{
		return true;
	}
	return false;
}

#endif

CNMCOClientObject::CNMCOClientObject()
{
	this->m_pBuf = new BYTE[ 65536 * 4 ];
	this->m_nBufSize = 65536 * 4;
}

CNMCOClientObject::~CNMCOClientObject()
{
	delete [] this->m_pBuf;
}

CNMCOClientObject& CNMCOClientObject::GetInstance( void )
{
	//static CNMCOClientObject	s_SingletonInstance;

	//return s_SingletonInstance;

	// 2006.12.14 wony
	// 원래는 위의 코드입니다
	// 위처럼 할 경우 넥슨웹을 닫았을 때 계속 넥슨 메신져 관련된 프로세스가 살아 있게 됩니다
	// 현재 그랜드체이스에서는 처음 접속 할 때 몇가지 정보들을 받아 오는데만 CNMCOClientObject를
	// 사용하고 있으므로 잠시 수정 합니다
	CNMCOClientObject	NMCOClientObject;

	return NMCOClientObject;
}

void CNMCOClientObject::Init( UInt32 uMode, UInt32 uGameCode, LPSTR pszDomain, UInt32 uSupportFlag )
{
	NMCOInit( uMode, uGameCode, pszDomain, uSupportFlag );
}

const char* CNMCOClientObject::GetCommonPath( LPSTR szPath )
{
	return NMCOGetCommonPath( szPath );
}

const char* CNMCOClientObject::GetAvatarPath( LPSTR szPath )
{
	return NMCOGetAvatarPath( szPath );
}

const char* CNMCOClientObject::GetMessengerPath( LPSTR szPath )
{
	return NMCOGetMessengerPath( szPath );
}

BOOL CNMCOClientObject::DownloadPatcher( void )
{
	return NMCODownloadPatcher();
}

BOOL CNMCOClientObject::ExecutePatcher( const char* szUrl, const char* szPatchDir, const char* szExecCmdLine, BOOL bWaitParentExit, BOOL bWaitPatchEnd )
{
	return NMCOExecutePatcher2( szUrl, szPatchDir, szExecCmdLine, bWaitParentExit, bWaitPatchEnd );
}

BOOL CNMCOClientObject::ExecuteLauncher( UInt32 uGameCode, UInt32 uLaunchType, const char* szArgument, BOOL bWaitParent )
{
	if( kGameCode_maplestory == uGameCode || kGameCode_dmatgo == uGameCode || kGameCode_qplay == uGameCode ) // 메이플스토리, 서바이벌맞고, 큐플레이 막음
	{
		return FALSE;
	}
	else
	{
	        return NMCOExecuteLauncher( uGameCode, uLaunchType, szArgument, bWaitParent );
        }
}

BOOL CNMCOClientObject::PatchNMCOSrv( const char* szDomain )
{
	return NMCOPatchNMCOSrv( szDomain );
}

void CNMCOClientObject::StartStandAloneMessenger( BOOL bStealthMode )
{
	NMCOStartStandAloneMessenger( bStealthMode );
}

UInt64 CNMCOClientObject::GetDllVersion( void )
{
	return NMCOGetDllVersion();
}

UInt64 CNMCOClientObject::GetNMCOSrvVersion( void )
{
	return NMCOGetNMCOSrvVersion();
}

void CNMCOClientObject::GetDomain( char* szDomain )
{
	NMCOGetDomain( szDomain );
}

void CNMCOClientObject::RegisterCallbackMessage( HWND hWnd, UInt32 uMessage )
{
	NMCORegisterCallbackMessage( hWnd, uMessage );
}

void CNMCOClientObject::ResetCallbackMessage( HWND hWnd )
{
	NMCOResetCallbackMessage( hWnd );
}

BOOL CNMCOClientObject::Login( LPCSTR szLocalId, LPCSTR szLoginId, LPCSTR szPassword, BOOL bMergeList, UInt64 uMsgVersion )
{
	return NMCOLogin( szLocalId, szLoginId, szPassword, bMergeList, uMsgVersion );
}

BOOL CNMCOClientObject::Logout( void )
{
	return NMCOLogout();
}

BOOL CNMCOClientObject::ReInstallConMsg( void )
{
	return NMCOReInstallConMsg();
}

UInt32 CNMCOClientObject::GetServerIndex( LPCSTR szLoginId )
{
	return NMCOGetServerIndex( szLoginId );
}

BOOL CNMCOClientObject::IsOnline( UInt32 uStatus )
{
	static BOOL		bStatusMap[] = { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE };

	if ( uStatus > 7 )
		return FALSE;

	return bStatusMap[ uStatus ];
}

BOOL CNMCOClientObject::IsOnline( void )
{
	return this->IsOnline( this->GetMyStatus() );
}

BOOL CNMCOClientObject::AddNewFriend( LPCSTR szFriendID, LPCSTR szAlias, LPCSTR szMessage )
{
	return NMCOAddNewFriend( szFriendID, szAlias, szMessage );
}

void CNMCOClientObject::ConfirmAddFriend( LPCSTR szConfirmId, BOOL bAccept, LPCSTR pszAlias )
{
	NMCOConfirmAddFriend( szConfirmId, bAccept, pszAlias );
}

void CNMCOClientObject::ChangeMyInfo( LPCSTR pszNickName, LPCSTR pszScreenName, LPCSTR pszPosition, UInt32 uStatus, UInt32 uEmotion )
{
	NMCOChangeMyInfo( pszNickName, pszScreenName, pszPosition, uStatus, uEmotion );
}

BOOL CNMCOClientObject::AddNewCategory( LPCSTR szNewCategoryName )
{
	return NMCOAddNewCategory( szNewCategoryName );
}

BOOL CNMCOClientObject::ChangeCategoryName( const char* szOldCategoryName, const char* szNewCategoryName )
{
	return NMCOChangeCategoryName( szOldCategoryName, szNewCategoryName );
}

BOOL CNMCOClientObject::DeleteCategory( const char* szCategoryName )
{
	return NMCODeleteCategory( szCategoryName );
}

BOOL CNMCOClientObject::MoveUserCategory( UInt64 uIdCode, const char* szFromCategory, const char* szToCategory )
{
	return NMCOMoveUserCategory( uIdCode, szFromCategory, szToCategory );
}

BOOL CNMCOClientObject::BlockUser( UInt64 uIdCode, BOOL bBlock )
{
	return NMCOBlockUser( uIdCode, bBlock );
}

BOOL CNMCOClientObject::DeleteUser( UInt64 uIdCode, BOOL bDelete )
{
	return NMCODeleteUser( uIdCode, bDelete );
}

BOOL CNMCOClientObject::SetUserToBlackList( const char* szLoginId, BOOL bSet, LPCSTR szMemo )
{
	return NMCOSetUserToBlackList( szLoginId, bSet, szMemo );
}

BOOL CNMCOClientObject::ChangeUserAlias( UInt64 uIdCode, LPCSTR szAlias )
{
	return NMCOChangeUserAlias( uIdCode, szAlias );
}

BOOL CNMCOClientObject::CreateChatSession( UInt64 uIdCode )
{
	FriendInfo	frdInfo;

	this->GetFriendInfo( uIdCode, frdInfo );

	if ( frdInfo.uStatus != kStatus_Offline && frdInfo.uStatus != kStatus_AppearOffline )
		return NMCOCreateChatSession( uIdCode );

	return FALSE;
}

BOOL CNMCOClientObject::CreateMultiChatSession( UInt64* aIdCodes, UInt32 uCnt )
{
	return NMCOCreateMultiChatSession( aIdCodes, uCnt );
}

BOOL CNMCOClientObject::CreateFileUploadSession( UInt64 uIdCode, UInt32 uFileCnt, const char* aFileName )
{
	// Don't transfer "the Internet Cache Files"
	{
		char szInternetCache[ MAX_PATH ];
		if ( ::SHGetSpecialFolderPathA( NULL , szInternetCache , CSIDL_INTERNET_CACHE , TRUE ) )
		{
			for ( UInt32 index = 0; index < uFileCnt; ++index )
			{
				if ( !strncmp( szInternetCache, &aFileName[ 256 * index ], strlen( szInternetCache ) ) )
				{
					return FALSE;
				}
			}
		}
	}

	FriendInfo	frdInfo;

	this->GetFriendInfo( uIdCode, frdInfo );


	if ( frdInfo.uStatus != kStatus_Offline && frdInfo.uStatus != kStatus_AppearOffline )
		return NMCOCreateFileUploadSession( uIdCode, uFileCnt, aFileName );

	return FALSE;
}

BOOL CNMCOClientObject::CreateWBSession( UInt64 uIdCode, UInt32 uSerialKey )
{
	FriendInfo	frdInfo;

	this->GetFriendInfo( uIdCode, frdInfo );

	if ( frdInfo.uStatus != kStatus_Offline && frdInfo.uStatus != kStatus_AppearOffline )
		return NMCOCreateWBSession( uIdCode, uSerialKey );

	return FALSE;
}

BOOL CNMCOClientObject::FindUser( FindUserRequestInfo& findUserRequestInfo )
{
	return NMCOFindUser( findUserRequestInfo );
}

BOOL CNMCOClientObject::SendNoteMsg( NoteInfo& noteInfo )
{
	return NMCOSendNoteMsg( noteInfo );
}

BOOL CNMCOClientObject::SendAccuseMsg( UInt32 uType , UInt32 uSubType , UInt32 uAccuserDomain , const char* szAccuserId , UInt32 uAccusedDomain , const char* szAccusedId , const char* szReason , const char* szEvidence , UInt32 uGameCode /* = 0 */ )
{
	return NMCOSendAccuseMsg( uType , uSubType , uAccuserDomain , szAccuserId , uAccusedDomain , szAccusedId , szReason , szEvidence , uGameCode );
}

BOOL CNMCOClientObject::SendNoteExMsg( NoteInfoEx& noteInfoEx )
{
	return NMCOSendNoteExMsg( noteInfoEx );
}

BOOL CNMCOClientObject::GetFindUserResult( FindUserInfo* aFindUserInfo, UInt32& uListSize, UInt32 uToken )
{
	return NMCOGetFindUserResult( aFindUserInfo, uListSize, uToken );
}

BOOL CNMCOClientObject::DeleteFindUserResult( UInt32 uToken )
{
	return NMCODeleteFindUserResult( uToken );
}

UInt32 CNMCOClientObject::GetUnreadNoteCount( void )
{
	return NMCOGetUnreadNoteCount();
}

BOOL CNMCOClientObject::GetNoteBox( UInt32 uBoxType, NoteInfo* aNoteList, UInt32& uListSize, BOOL bTrimServerExt, UInt32 uSerialNo )
{
	return NMCOGetNoteBox( uBoxType, aNoteList, uListSize, bTrimServerExt, uSerialNo );
}

BOOL CNMCOClientObject::ProcessNote( UInt32 uSerialNo, UInt32 uOperation )
{
	return NMCOProcessNote( uSerialNo, uOperation );
}

BOOL CNMCOClientObject::GetServerMessage( CustomMessageData* pMessage )
{
	return NMCOGetServerMessage( pMessage );
}

BOOL CNMCOClientObject::GetAddFriendConfirmMessage( CustomAddFriendConfirmData* pMessage, BOOL bTrimServerExt )
{
	return NMCOGetAddFriendConfirmMessage( pMessage, bTrimServerExt );
}

BOOL CNMCOClientObject::GetFriendList( FriendsCategoryList& fcList, BOOL bTrimServerExt )
{
	SInt32	nBufSize = this->m_nBufSize;

	BOOL	bRetVal = NMCOGetFriendList( this->m_pBuf, nBufSize, bTrimServerExt );

	if ( !bRetVal )
	{
		delete [] this->m_pBuf;

		this->m_pBuf = new BYTE[ nBufSize ];
		this->m_nBufSize = nBufSize;

		bRetVal = NMCOGetFriendList( this->m_pBuf, nBufSize, bTrimServerExt );
	}

	if ( bRetVal )
	{
		UInt32 uCCount, uFCount, uPtr = 0;
		
		fcList.clear();

		CopyMemory( &uCCount, this->m_pBuf, 4 );	uPtr += 4;

		for ( UInt32 i = 0; i < uCCount; i++ )
		{
			CategoryInfo	cateInfo;

			CopyMemory( cateInfo.szCategoryName, this->m_pBuf + uPtr, CATEGORY_SIZE );		uPtr += CATEGORY_SIZE;
			CopyMemory( &uFCount, this->m_pBuf + uPtr, 4 );									uPtr += 4;

			for ( UInt32 j = 0; j < uFCount; j++ )
			{
				FriendInfo	frdInfo;

				CopyMemory( &frdInfo, this->m_pBuf + uPtr, sizeof( FriendInfo ) );
				uPtr += sizeof( FriendInfo );

				cateInfo.aFriends.push_back( frdInfo );
			}
			fcList.push_back( cateInfo );
		}
	}

	return bRetVal;
}

BOOL CNMCOClientObject::GetMyClubList( ClubInfoList& clubList )
{
	ClubInfo	aClubInfo[ 100 ];
	UInt32		uListSize = 100;
	BOOL		bRetVal = NMCOGetMyClubList( aClubInfo, uListSize );

	if ( bRetVal )
	{
		clubList.clear();
		for ( UInt32 i = 0 ; i < uListSize ; ++i )
		{
			clubList.push_back( aClubInfo[ i ] );
		}
	}

	return bRetVal;
}

BOOL CNMCOClientObject::GetBlackList( FriendList& frdList, BOOL bTrimServerExt )
{
	SInt32	nBufSize = this->m_nBufSize;

	BOOL	bRetVal = NMCOGetBlackList( this->m_pBuf, nBufSize, bTrimServerExt );

	if ( !bRetVal )
	{
		delete [] this->m_pBuf;

		this->m_pBuf = new BYTE[ nBufSize ];
		this->m_nBufSize = nBufSize;

		bRetVal = NMCOGetBlackList( this->m_pBuf, nBufSize, bTrimServerExt );
	}

	if ( bRetVal )
	{
		UInt32		uCnt;
		UInt32		uPtr = 0;
		FriendInfo	frdInfo;

		CopyMemory( &uCnt, this->m_pBuf, 4 );
		uPtr = 4;

		frdList.clear();

		for ( UInt32 i = 0; i < uCnt; ++i )
		{
			CopyMemory( &frdInfo, &this->m_pBuf[ uPtr ], sizeof( FriendInfo ) );
			frdList.push_back( frdInfo );
			uPtr += sizeof( FriendInfo );
		}

	}

	return bRetVal;
}

UInt64 CNMCOClientObject::GetIdCodeFromIdx( UInt32 uIdx )
{
	return NMCOGetIdCodeFromIdx( uIdx );
}

BOOL CNMCOClientObject::GetFriendInfo( UInt64 uIdCode, FriendInfo& frdInfo, BOOL bTrimServerExt )
{
	return NMCOGetFriendInfo( uIdCode, frdInfo, bTrimServerExt );
}

BOOL CNMCOClientObject::GetMyLoginId( LPSTR pszLoginId, BOOL bTrimServerExt )
{
	return NMCOGetMyLoginId( pszLoginId, bTrimServerExt );
}

BOOL CNMCOClientObject::GetMyNickName( LPSTR pszNickName )
{
	return NMCOGetMyNickName( pszNickName );
}

BOOL CNMCOClientObject::GetMyRealName( LPSTR pszRealName )
{
	return NMCOGetMyRealName( pszRealName );
}

BOOL CNMCOClientObject::GetMyScreenName( LPSTR pszScreenName )
{
	return NMCOGetMyScreenName( pszScreenName );
}

BOOL CNMCOClientObject::GetMyPosition( LPSTR pszPosition )
{
	return NMCOGetMyPosition( pszPosition );
}

SInt32 CNMCOClientObject::GetMyStatus( void )
{
	return NMCOGetMyStatus();
}

UInt64 CNMCOClientObject::GetMyIdCode( void )
{
	return NMCOGetMyIdCode();
}

UInt32 CNMCOClientObject::GetMyOID( void )
{
	return NMCOGetMyOID();
}

UInt32 CNMCOClientObject::GetMyAge( void )
{
	return NMCOGetMyAge();
}

UInt32 CNMCOClientObject::GetMySex( void )
{
	return NMCOGetMySex();
}

UInt32 CNMCOClientObject::GetMyAreaCode( void )
{
	return NMCOGetMyAreaCode();
}

UInt32 CNMCOClientObject::GetMySchoolCode( void )
{
	return NMCOGetMySchoolCode();
}

SYSTEMTIME CNMCOClientObject::GetMyBirthDay( void )
{
	return NMCOGetMyBirthDay();
}

UInt32 CNMCOClientObject::GetMyMaskInfo( void )
{
	return NMCOGetMyMaskInfo();
}

BOOL CNMCOClientObject::GetMyAvatarInfo( AvatarInfo& aiInfo )
{
	return NMCOGetMyAvatarInfo( aiInfo );
}

UInt32 CNMCOClientObject::GetMyEmotion( void )
{
	UInt32 uTestVal = NMCOGetMyEmotion();

//	ASSERT( uTestVal > 0 );
//	ASSERT( uTestVal <= 5 );
	return uTestVal;
}

void CNMCOClientObject::GetMySessionKey( UInt64& uSessionKeyHigh, UInt64& uSessionKeyLow )
{
	NMCOGetMySessionKey( uSessionKeyHigh, uSessionKeyLow );
}

BOOL CNMCOClientObject::SendRefreshMessage( UInt32 uRefreshType )
{
	return NMCOSendRefreshMessage( uRefreshType ); 
}

BOOL CNMCOClientObject::GetConfig( UInt32 uConfigType, LPSTR szValue )
{
	return NMCOGetConfig( uConfigType, szValue );
}

BOOL CNMCOClientObject::SetConfig( UInt32 uConfigType, LPCSTR szValue )
{
	return NMCOSetConfig( uConfigType, szValue );
}

BOOL CNMCOClientObject::GetSessionVal( const char* szValName, char* szValueOut )
{
	return NMCOGetSessionVal( szValName, szValueOut );
}

BOOL CNMCOClientObject::SetSessionVal( const char* szValName, const char* szValue, BOOL bAttachToProcess /* = FALSE */ )
{
	return NMCOSetSessionVal( szValName, szValue, bAttachToProcess );
}

BOOL CNMCOClientObject::GetUserInfo( const char* szLoginId, FriendInfo& frdInfo, BOOL bTrimServerExt )
{
	return NMCOGetUserInfo( szLoginId, frdInfo, bTrimServerExt );
}

BOOL CNMCOClientObject::GetRequestSessionMessage( CustomRequestSessionData* pMessage, BOOL bTrimServerExt )
{
	return NMCOGetRequestSessionMessage( pMessage, bTrimServerExt );
}

BOOL CNMCOClientObject::AcceptSession( UInt32 uDataKey, BOOL bAccept )
{
	return NMCOAcceptSession( uDataKey, bAccept );
}

void CNMCOClientObject::GetAvatarFullImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo, UInt32& uOffsetX, UInt32& uOffsetY )
{
	NMCOGetAvatarFullImage( aiAvatarInfo, uEmotion, aItemNo, aFileNo, uOffsetX, uOffsetY );
}

void CNMCOClientObject::GetAvatarHeadImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo )
{
	NMCOGetAvatarHeadImage( aiAvatarInfo, uEmotion, aItemNo, aFileNo );
}

void CNMCOClientObject::GetAvatarIconImage( AvatarInfo& aiAvatarInfo, UInt32 uEmotion, UInt32* aItemNo, UInt32* aFileNo )
{
	NMCOGetAvatarIconImage( aiAvatarInfo, uEmotion, aItemNo, aFileNo );
}

const char* CNMCOClientObject::GetAvatarFileName( UInt32 uItemNo, UInt32 uFileNo, char* szFileNameOut )
{
	return NMCOGetAvatarFileName( uItemNo, uFileNo, szFileNameOut );
}

UInt32 CNMCOClientObject::DownloadAvatarImage( UInt32 uItemNo, UInt32 uFileNo, HWND hCallbackWnd, UInt32 uCallbackMsg, BOOL bReload )
{
	return NMCODownloadAvatarImage( uItemNo, uFileNo, hCallbackWnd, uCallbackMsg, bReload );
}

const char* CNMCOClientObject::GetBannerFileName( const char* szBannerUrl, char* szFileNameOut )
{
	return NMCOGetBannerFileName( szBannerUrl, szFileNameOut );
}

UInt32 CNMCOClientObject::DownloadBannerImage( const char* szBannerUrl, HWND hCallbackWnd, UInt32 uCallbackMsg, BOOL bReload )
{
	return NMCODownloadBannerImage( szBannerUrl, hCallbackWnd, uCallbackMsg, bReload );
}

BOOL CNMCOClientObject::GetRecommendGameList( GameInfoList& gameList )
{
	GameInfo	aGameInfo[ 100 ];
	UInt32		uListSize = 100;
	BOOL		bRetVal;

	bRetVal = NMCOGetRecommendGameList( aGameInfo, uListSize );

	if ( bRetVal )
	{
		gameList.clear();
		for ( UInt32 i = 0; i < uListSize; ++i )
			gameList.push_back( aGameInfo[ i ] );
	}

	return bRetVal;
}

BOOL CNMCOClientObject::GetMyGameList( GameInfoList& gameList )
{
	GameInfo	aGameInfo[ 100 ];
	UInt32		uListSize = 100;
	BOOL		bRetVal;

	bRetVal = NMCOGetMyGameList( aGameInfo, uListSize );

	if ( bRetVal )
	{
		gameList.clear();
		for ( UInt32 i = 0; i < uListSize; ++i )
			gameList.push_back( aGameInfo[ i ] );
	}

	return bRetVal;
}

UInt32 CNMCOClientObject::GetMyGameListSize( void )
{
	BOOL		bRetVal;
	UInt32		uListSize = 100;
	
	bRetVal = NMCOGetMyGameListSize( uListSize );


	if ( bRetVal )
	{
		return uListSize;
	}
	else
	{
		return 0;
	}
}

BOOL CNMCOClientObject::GetGameGroupList( GameGroupList& ggList )
{
	SInt32	nBufSize = this->m_nBufSize;

	BOOL	bRetVal = NMCOGetGameGroupList( this->m_pBuf, nBufSize );

	if ( !bRetVal )
	{
		delete [] this->m_pBuf;

		this->m_pBuf		= new BYTE[ nBufSize ];
		this->m_nBufSize	= nBufSize;

		bRetVal = NMCOGetGameGroupList( this->m_pBuf, nBufSize );
	}

	if ( bRetVal )
	{
		UInt32 uGCount, uICount, uPtr = 0;
		
		ggList.clear();

		CopyMemory( &uGCount, this->m_pBuf, 4 );	uPtr += 4;

		for ( UInt32 i = 0; i < uGCount; i++ )
		{
			GameGroupInfo	ggInfo;

			CopyMemory( ggInfo.szGroupName, this->m_pBuf + uPtr, 256 );		uPtr += 256;
			CopyMemory( &uICount, this->m_pBuf + uPtr, 4 );					uPtr += 4;

			for ( UInt32 j = 0; j < uICount; j++ )
			{
				GameInfo	giInfo;

				CopyMemory( &giInfo, this->m_pBuf + uPtr, sizeof( GameInfo ) );
				uPtr += sizeof( GameInfo );

				ggInfo.aGameList.push_back( giInfo );
			}
			ggList.push_back( ggInfo );
		}
	}

	return bRetVal;
}

BOOL CNMCOClientObject::AddToMyGame( UInt32 uGameCode, UInt32 uIndex )
{
	return NMCOAddToMyGame( uGameCode, uIndex );
}

BOOL CNMCOClientObject::DeleteFromMyGame( UInt32 uGameCode )
{
	return NMCODeleteFromMyGame( uGameCode );
}

BOOL CNMCOClientObject::MoveMyGameItem( UInt32 uGameCode, UInt32 uIndex )
{
	return NMCOMoveMyGameItem( uGameCode, uIndex );
}

void CNMCOClientObject::OpenDlg( UInt32 uDlgType )
{
	NMCOOpenDlg( uDlgType );
}

BOOL CNMCOClientObject::GetURL( UInt32 uType, UInt32 uIndex, char* szUrlOut, UInt32 uParam, BOOL bUseLoginDirect, LPVOID pParam, BOOL bPopup )
{
	return NMCOGetURL2( uType, uIndex, szUrlOut, uParam, bUseLoginDirect, pParam, bPopup );
}

BOOL CNMCOClientObject::GetLoginDirectURL( const char* szUrl , char* szUrlOut, BOOL bPopup )
{
	return NMCOGetLoginDirectURL2( szUrl, szUrlOut, bPopup );
}

#endif // NATION_JAPAN