#include "stdafx.h"
//
#include ".\gcpairplaysystem.h"
//
//

//
#include "utf16_cvt.h" // UnitCode 텍스트 파일을 만들기 위해.

#include "MyD3D.h"
#include "Replay.h"



#include "GCUtil/GCElapsedTime.h"
#include "GCStateMachine.h"
// 
// 

//
#include "KGCRoomManager.h"
#include "kom\blowfish.h"
#include "kom\sha1.h"


ImplementSingleton( GCPairPlaySystem );

_Mail GCPairPlaySystem::m_kMail;

bool GCPairPlaySystem::m_bFileOut = false;
bool GCPairPlaySystem::m_bDataSend = true;

GCPairPlaySystem::GCPairPlaySystem(void) : m_iFeed(0), m_bDummyReplay(false)
{
    m_iStartReportReplay = 0;
    m_iEndReportReplay = 0;

    m_mapChatLog.clear();

    m_mapChatLog[PPSCT_LOBBY] = ChatLogDeque();
    m_mapChatLog[PPSCT_ROOM] = ChatLogDeque();
    m_mapChatLog[PPSCT_GAME] = ChatLogDeque();
    m_mapChatLog[PPSCT_GUILD] = ChatLogDeque();

    m_stmReplay.Reset();

	m_Today = GetToday();

	m_newtime = NULL;
}

GCPairPlaySystem::~GCPairPlaySystem(void)
{
    m_mapChatLog.clear();
}

void GCPairPlaySystem::AddChatLog( GCPairPlaySystem::PPS_CHAT_TYPE eType, const std::wstring& strNickName, const std::wstring& strMsg )
{
    IterChatLog mit = m_mapChatLog.find( eType );
    ASSERT( (mit != m_mapChatLog.end()) && "Inavlid Chat Type" );
    if( mit != m_mapChatLog.end() )
    {
        ChatLogDeque& deqChatLog = mit->second;
        deqChatLog.push_back( std::make_pair( strNickName, strMsg ) );

        if( deqChatLog.size() > KD3DListBox::DEFAULT_MAX_LINE )
            deqChatLog.pop_front();
    }
}

void GCPairPlaySystem::ClearChatLog( GCPairPlaySystem::PPS_CHAT_TYPE eType )
{
    IterChatLog mit = m_mapChatLog.find( eType );
    ASSERT( (mit != m_mapChatLog.end()) && "Inavlid Chat Type" );
    if( mit != m_mapChatLog.end() )
    {
        ChatLogDeque& deqChatLog = mit->second;
        deqChatLog.clear();
    }
}

void GCPairPlaySystem::MakeChatLog( const ChatLogDeque& deq, const std::vector< std::wstring >& vecUsers, 
                                   std::wstringstream& stm )
{
    ChatLogDeque::const_iterator dit;
    for( dit = deq.begin(); dit != deq.end(); ++dit )
    {
        std::vector< std::wstring >::const_iterator vit = 
            std::find( vecUsers.begin(), vecUsers.end(), dit->first );
        if( vit != vecUsers.end() )
            stm << dit->first << L" : " << dit->second << L"\r\n";
    }
}

bool GCPairPlaySystem::OutputChatLog( const std::wstring& strFileName, const std::vector< std::wstring >& vecUsers, 
                                     GCPairPlaySystem::PPS_CHAT_TYPE eType/* = PPSCT_NUM*/ )
{
    std::wstringstream stmOutput;

    IterChatLog mit;
    if( eType == PPSCT_NUM )
    {
        for( mit = m_mapChatLog.begin(); mit != m_mapChatLog.end(); ++mit )
        {
            stmOutput << EnumChatTypetoStr( mit->first ) << L"\r\n";
            ChatLogDeque& deqChatlog = mit->second;
            MakeChatLog( deqChatlog, vecUsers, stmOutput );
        }
    }
    else
    {
        mit = m_mapChatLog.find( eType );
        ASSERT( (mit != m_mapChatLog.end()) && "Inavlid Chat Type" );
        if( mit != m_mapChatLog.end() )
        {
            stmOutput << EnumChatTypetoStr( mit->first ) << L"\r\n";
            ChatLogDeque& deqChatlog = mit->second;
            MakeChatLog( deqChatlog, vecUsers, stmOutput );
        }
    }

    std::wofstream wFile;
    IMBUE_NULL_CODECVT( wFile );
    wFile.open( KncUtil::toNarrowString( strFileName ).c_str(), std::ios::binary | std::ios::out );
    ASSERT( wFile.is_open() != false );
    if( !wFile.is_open() )
        return false;

    wchar_t cBom = 0xfeff;
    wFile.write( &cBom, 1 );
    wFile << stmOutput.str();
    wFile.close();

    return true;
}

std::wstring GCPairPlaySystem::EnumChatTypetoStr( GCPairPlaySystem::PPS_CHAT_TYPE eType )
{
    switch( eType )
    {
        CaseToWideString( PPSCT_LOBBY );
        CaseToWideString( PPSCT_ROOM );
        CaseToWideString( PPSCT_GAME );
        CaseToWideString( PPSCT_GUILD );
    }

    return EnumToWideString( PPSCT_NUM );
}

void GCPairPlaySystem::SaveDummyReplayHeadData()
{
    m_stmReplay.Reset();

    g_MyD3D->MyReplay->SetDummyReplayHeadData();

    // [8/3/2007 breadceo] 압축 없이 쓰고 나중에 zlib으로 압축
    // └> 압축 안하니 1분당 300K 정도 들어갑니다. 쿨럭 압축해야지..
    unsigned long ulCompressedBytes = 0;
    void* pCompressedHeadData = NULL;
    GCFUNC::CompressStream( (void*)&g_MyD3D->MyReplay->m_HeadData, pCompressedHeadData, sizeof(Replay_Head_Data), ulCompressedBytes );

    Replay_HeadData_Compress_Header sCompHeader;
    sCompHeader.ulCompressedSize = ulCompressedBytes;
    sCompHeader.dwRealSize = sizeof( g_MyD3D->MyReplay->m_HeadData );

    if ( IsDummyReplayMode() ) 
    {
        m_stmReplay.Write( (char*)&sCompHeader, sizeof( Replay_HeadData_Compress_Header ) );
        m_stmReplay.Write( (char*)pCompressedHeadData, ulCompressedBytes );
    }
	SAFE_DELETE_ARRAY( pCompressedHeadData );
    m_iStartReportReplay = 0;
    m_iEndReportReplay = 0;
}

bool GCPairPlaySystem::IsDummyReplayMode()
{
    // 이 모드들은 리플레이를 안찍는다
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON 
        || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL
        || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER
        || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT
		|| SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME
		|| SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG
		|| SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN
        || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA
		|| g_MyD3D->MyReplay->m_iCurrState == RP_VIEW ) 
        return false;

        //GC_GMC_MATCH            = 0,    // 대전
        //GC_GMC_GUILD_BATTLE     = 1,    // 길드전
        //GC_GMC_DUNGEON          = 2,    // 퀘스트
        //GC_GMC_INDIGO           = 3,    // 용사의 섬
        //GC_GMC_TUTORIAL         = 4,    // 튜토리얼
        //GC_GMC_TAG_MATCH        = 5,    // 태그 매치
        //GC_GMC_MONSTER_CRUSADER = 6,    // 몬스터 원정대
        //GC_GMC_MONSTER_HUNT     = 7,    // 몬스터 대전
		//GC_GMC_MINIGAME		  = 9,	  // 미니게임
    return true;
}

void GCPairPlaySystem::SaveDummyReplayData()
{
    if( !IsDummyReplayMode() )
        return;

    // [8/3/2007 breadceo] Replay 상태에 맞춰서 계산 결정
    g_MyD3D->MyReplay->MakeDummyData();
    m_iFeed++;
    g_MyD3D->MyReplay->m_iFeed = m_iFeed;

    RakNet::BitStream stmReplayData;
    stmReplayData.Reset();

    if(m_iFeed == GC_FPS_LIMIT)
    {
        m_iFeed=0;
        g_MyD3D->MyReplay->m_iFeed = m_iFeed;

        // [8/3/2007 breadceo] stream에 쓰기
#ifdef COMRESSED_REPLAY_PROFILE_TEST
        DWORD dwTestStart = ::timeGetTime();
#endif
        g_MyD3D->MyReplay->WriteStream( stmReplayData );

        unsigned long ulCompressedBytes = 0;
        void* pCompressedHeadData = NULL;
        GCFUNC::CompressStream( (void*)stmReplayData.GetData(), pCompressedHeadData, stmReplayData.GetNumberOfBytesUsed(), ulCompressedBytes );

        Replay_FrameData_Compress_Header sCompHeader;
        sCompHeader.ulCompressedSize = ulCompressedBytes;
        sCompHeader.dwRealSize = stmReplayData.GetNumberOfBytesUsed();

        UpdateReportReplayInfo( m_stmReplay.GetNumberOfBytesUsed(), sizeof( Replay_FrameData_Compress_Header ) + ulCompressedBytes );

        m_stmReplay.Write( (char*)&sCompHeader, sizeof( Replay_FrameData_Compress_Header ) );
        m_stmReplay.Write( (char*)pCompressedHeadData, ulCompressedBytes );
#ifdef COMRESSED_REPLAY_PROFILE_TEST
        dwTestStart = ::timeGetTime() - dwTestStart;
        START_LOG( cout, dwTestStart );
#endif
        SAFE_DELETE_ARRAY( pCompressedHeadData );
    }
}

bool GCPairPlaySystem::MakeDummyReplay(bool bStreamReset, bool bReport)
{
    RakNet::BitStream& stmReplay = m_stmReplay;
    RakNet::BitStream* stmCurrentReplay = &m_stmReplay;
    RakNet::BitStream stmReport;

    if( IsDummyReplayMode() == false )
        return false;

#define REPLAY_TIME_CHECK
#if defined(_DEBUG) && defined( REPLAY_TIME_CHECK )
    GCElapsedTime kElapsedTime( L"REPLAY_TIME_CHECK" );
#endif

    m_bDummyReplay =false;
    DeleteFile( DUMMY_REPLAY_FILE_NAME );

    HANDLE hDummyReplay = NULL;
    hDummyReplay = CreateFile( DUMMY_REPLAY_FILE_NAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( hDummyReplay == INVALID_HANDLE_VALUE )
    {
        hDummyReplay = NULL;
        ClearDummyReplay();
        return false;
    }

    if( bReport == true )
    {
        stmReplay.ResetReadPointer();
        //일단 헤더부터 복사하자!!!
        char* pHeaderData = new char[ sizeof( Replay_HeadData_Compress_Header ) ];
        stmReplay.Read( pHeaderData, sizeof( Replay_HeadData_Compress_Header ) );     //헤더 용량을 알아냅니다.
        int iHeaderSize = ((Replay_HeadData_Compress_Header*)pHeaderData)->ulCompressedSize;
        SAFE_DELETE_ARRAY( pHeaderData );

        char* pHeader = new char[sizeof( Replay_HeadData_Compress_Header) + iHeaderSize];
        stmReplay.ResetReadPointer();
        stmReplay.Read( pHeader, sizeof( Replay_HeadData_Compress_Header) + iHeaderSize );    //헤더 정보를 읽어들입니다.
        stmReport.Write( pHeader, sizeof( Replay_HeadData_Compress_Header) + iHeaderSize );
        SAFE_DELETE_ARRAY( pHeader );

        //stmReplay.ResetReadPointer();
        //stmReplay.IgnoreBits( (m_aReportReplay[m_iStartReportReplay].iStreamCursor ) * 8/*Cursor는 Byte고 파라메터는 bit니까..*/);
        //char* IgnoreRead = new char[ m_aReportReplay[m_iStartReportReplay].iStreamCursor ];
        //stmReplay.Read( IgnoreRead, m_aReportReplay[m_iStartReportReplay].iStreamCursor );
        //SAFE_DELETE_ARRAY( IgnoreRead );

        //저장된 프레임 수를 계산합니다.
        int iSize = (m_iEndReportReplay - m_iStartReportReplay);
        if( iSize < 0 )
            iSize = MAX_REPLAY_TIME_FOR_REPORT;

        int iStart = m_iStartReportReplay;

        for( int i = 0 ; i < iSize ; ++i )
        {
            int iWhere = iStart + i;

            char* pFrameData = new char[m_aReportReplay[iWhere].iStreamSize];
            if( false == stmReplay.Read( pFrameData, m_aReportReplay[iWhere].iStreamSize ) )
            {
                stmReplay.ResetReadPointer();
                return false;
            }

            stmReport.Write( pFrameData, m_aReportReplay[iWhere].iStreamSize );
            SAFE_DELETE_ARRAY( pFrameData );
        }
        stmReplay.ResetReadPointer();
        stmCurrentReplay = &stmReport;
    }

    DWORD temp;

	// 리플레이 기본 헤더 영역을 씁니다.
    float fVersion = REPLAY_VERSION;
    DWORD dwSrcByte = static_cast<DWORD>(stmCurrentReplay->GetNumberOfBytesUsed());
    WriteFile( hDummyReplay, &fVersion, sizeof(float), &temp, NULL );
    WriteFile( hDummyReplay, &dwSrcByte, sizeof(DWORD), &temp, NULL );

	// replay API에 해당하는 부분을 씁니다.
	WriteFile( hDummyReplay, &g_MyD3D->MyReplay->GetReplayAPI_Header(), sizeof( ReplayAPI_Header ), &temp, NULL );

	// 리플레이 본체에 해당하는 부분을 씁니다.
    WriteFile( hDummyReplay, stmCurrentReplay->GetData(), stmCurrentReplay->GetNumberOfBytesUsed() , &temp, NULL );

    if(temp != stmCurrentReplay->GetNumberOfBytesUsed() )
    {
        CloseHandle(hDummyReplay);
        ClearDummyReplay();
        return false;
    }

    CloseHandle(hDummyReplay);
    if( bStreamReset )
        ClearDummyReplay();

    if( bReport == true )
        stmCurrentReplay->Reset();
    stmCurrentReplay = NULL;
    // [8/3/2007 breadceo] Dummy Replay 생성 성공
    m_bDummyReplay = true;
    return true;
}

void GCPairPlaySystem::ClearDummyReplay()
{
    m_iFeed = 0;
    m_stmReplay.Reset();
}

bool GCPairPlaySystem::SendReport( DWORD dwBadGuy_, int iReason_, std::wstring strComment_ , const std::wstring& NickName_ ,const std::wstring strLogin)
{
    std::vector< DWORD > vecPlayerList;
    vecPlayerList.push_back( dwBadGuy_ );
    m_strUserComment = strComment_;

	std::wstring strReason;
	switch(iReason_)
	{
	case 1:
		strReason = L"Excessive foul language";
		break;
	case 2:
		strReason = L"Using Speed Hack";
		break;
	case 3:
		strReason = L"Using Game Hack";
		break;
	default:
		strReason = L"Etc.";
		break;
	}

    m_wstrBadGuyInifo.clear();
    m_wstrBadGuyInifo << "\r\n<< BadGuy Info >>\r\n";
    m_wstrBadGuyInifo << "\r\nNickName : " << NickName_.c_str();
    m_wstrBadGuyInifo << "\r\nUserUID : " << dwBadGuy_;
    m_wstrBadGuyInifo << "\r\nUserID : " << strLogin.c_str();
	m_wstrBadGuyInifo << "\r\nReason : " << strReason.c_str();
    //일단 서버에 패킷부터 날려 주자. 신고 횟수가 남아있으면 메일도 보내자!!
    return KP2P::GetInstance()->Send_Report_User_Req( iReason_, vecPlayerList, NickName_ );
}

bool GCPairPlaySystem::SendReport( std::wstring strNickName_ , int iReason_, std::wstring strComment_  )
{
    // 이 조건은 머지요? -_-; 나하고 신고하는 대상하고 비교하네요;;; 몰라서 주석 - by jemitgge
    //if( g_MyD3D->m_TempPlayer.m_kUserInfo.strNickName.compare( strNickName_ ) == 0 )
	//g_MyD3D->m_TempPlayer.m_kUserInfo.strLogin
	
	//2009.04.02..kamd0:신고당한 유저의 닉네임을 정보해서 일치하면 해당 유저의 UID,NickName, iReason, 로그인 아이디를 보내준다.
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName.compare(strNickName_) == 0 )
		{
			return SendReport( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, iReason_, strComment_ , strNickName_ , g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin);
		}			
    }

    return false;
}

std::wstring GCPairPlaySystem::GetOutPutFileName()
{
	//현재 날짜,시간을 초단위까지 얻어온다.
	if(m_newtime == NULL)
	{
		time_t aclock;
		time( &aclock );                 /* Get time in seconds */
		m_newtime = localtime( &aclock );  
	}
   swprintf( g_MyD3D->MyReplay->m_pFileName, MAX_PATH, L"Replay\\%4d_%2d_%2d_%2d_%2d.first", 
        m_newtime->tm_year+1900, m_newtime->tm_mon+1, m_newtime->tm_mday, m_newtime->tm_hour, m_newtime->tm_min );
	
    std::wostringstream wstrStream;
    //wstrStream<<m_BadGuyID << L"_" << g_kGlobalValue.m_kUserInfo.strNickName << L"_" << m_newtime->tm_year + 1900 <<L"-"<<m_newtime->tm_mon + 1<<L"-"<<m_newtime->tm_mday<<L"-"<<m_newtime->tm_hour<<L"-"<<m_newtime->tm_min<<L"-"<<m_newtime->tm_sec << L"_";
	
	wstrStream<<m_newtime->tm_year + 1900 <<L"-"<<m_newtime->tm_mon + 1<<L"-"<<m_newtime->tm_mday<<L"_"<<m_newtime->tm_hour<<L"-"<<m_newtime->tm_min<<L"-"<<m_newtime->tm_sec << L"_" << m_BadGuyID << L"_" << g_kGlobalValue.m_kUserInfo.strNickName;

	return wstrStream.str();
}

//2009.04.02 kamd0: 신고파일명을 얻어오는 함수
std::wstring GCPairPlaySystem::GetNotifyFileName()
{
	std::wstringstream wstrStream;
	wstrStream<<m_newtime->tm_year + 1900 <<L"-"<<m_newtime->tm_mon + 1<<L"-"<<m_newtime->tm_mday<<L"_"<<m_newtime->tm_hour<<L"-"<<m_newtime->tm_min<<L"-"<<m_newtime->tm_sec << L"_" << m_BadGuyID << L"_" << g_kGlobalValue.m_kUserInfo.strNickName << L".txt";

	return wstrStream.str();
}

bool GCPairPlaySystem::MakeZipFile( std::string strOutputFileName_ , std::vector< std::wstring > vecFileList_ )
{
//     CZipper kZipper;
// 
//     if( !kZipper.CreateZipFile( (char*)strOutputFileName_.c_str() ) )
//         return false;
// 
//     for( int i = 0 ; i < (int)vecFileList_.size() ; ++i )
//     {
//         CElementFileInfo* kFileInfo = new CElementFileInfo();
//         kFileInfo->SetFileName( (char*)strOutputFileName_.c_str() );
// 
//         if( kFileInfo->ChkUserble() )
//             kZipper.AddFile( kFileInfo );
//     }
//     kZipper.Close();

    return true;
}

void GCPairPlaySystem::OutPutFilesForReport()
{
	std::wstring strFIleName = GetOutPutFileName();
    if( m_bFileOut == true )
    {
        ClearReportDatas();
    }
    std::vector< std::wstring > vecFileList;
    //채팅 로그를 우선 파일로 빼자.
    std::vector< std::wstring > vecPlayers;
    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            vecPlayers.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName );
    }
    if( OutputChatLog( strFIleName + L".txt", vecPlayers ) == true )
        vecFileList.push_back((strFIleName + L".txt").c_str());

    //리플레이를 파일로 빼자.
#if defined( NATION_BRAZIL)
#else
    if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && IsDummyReplayMode() )
    {
        if( MakeDummyReplay( false, true ) == true )
        {
            ::CopyFileW( DUMMY_REPLAY_FILE_NAME, (strFIleName + L".first").c_str() , FALSE );
            vecFileList.push_back(( strFIleName + L".first").c_str());
        }
    }
#endif
    m_vecReportFiles = vecFileList;
    m_bFileOut = true;
    m_bDataSend = false;
}
//
//#pragma comment(lib, "Dumpuploader/MailMIME.lib" )
//#pragma comment(lib, "Dumpuploader/MailSMTP.lib" )
void GCPairPlaySystem::MailSendEnd( BOOL bSuccess_ )
{
    MIME_Free(m_kMail.dataHead1);
    MIME_Free(m_kMail.dataHead2);
    MIME_Free(m_kMail.dataBody);
    MIME_Free(m_kMail.dataAttach);

    SiGCPairPlaySystem()->ClearReportDatas();
    m_bDataSend = true;
}

bool GCPairPlaySystem::SendMail( std::string title_, std::wstring strNote_, const std::vector< std::string >& vecFileList_ )
{
    MIME_Free(m_kMail.dataHead1);
    MIME_Free(m_kMail.dataHead2);
    MIME_Free(m_kMail.dataBody);
    MIME_Free(m_kMail.dataAttach);

    //아래 리턴이 INADDR_NONE이면 잘못된거져..--;;
    unsigned long ulSMTPAddr=SMTP_TransAddr( REPORT_MAIL_SMTP );

    if( ulSMTPAddr == INADDR_NONE )
        return false;

    std::string strPassword;
    
    //  복호화
    char keystr[32];

    sprintf_s(keystr,32,"%lld", nMailSHAKey);
    SHA1_CTX sha1;
    SHA1Init(&sha1);	
    SHA1Update(&sha1,(unsigned char*)keystr,strlen(keystr));
    unsigned char sha1key[20];
    SHA1Final(sha1key,&sha1);

//  char szEncrypt[nMailPassWordLen+1] = { 0 };
    static char szDecrypt[nMailPassWordLen+1] = { 0 };    
    memset( szDecrypt, 0, sizeof( char )* (nMailPassWordLen+1) );

    CBlowFish blowfish(sha1key,20);

//    blowfish.Encrypt((const unsigned char*)REPORT_MAIL_PASSWORD, szEncrypt, nMailPassWordLen );    
    blowfish.Decrypt(szMailPassword, (unsigned char*)szDecrypt, nMailPassWordLen );
    


    //_Mail m_kMail;	//이구조체만 채움 되는디.

    m_kMail.from		=   REPORT_MAIL_FROM;
    m_kMail.to		    =   REPORT_MAIL_TO;
    m_kMail.cc		    =   NULL;//m_val_strCC.GetLength()?(LPSTR)(LPCSTR)m_val_strCC:NULL;
    m_kMail.bcc		    =   NULL;//m_val_strBCC.GetLength()?(LPSTR)(LPCSTR)m_val_strBCC:NULL;
    m_kMail.dataHead1	=   MIME_MakeHead1(m_kMail.to, m_kMail.cc);
    m_kMail.dataHead2	=   MIME_MakeHead2( "FairPlayReport"  , m_kMail.from
                            , (LPSTR)title_.c_str(), 0 );
    m_kMail.dataBody	=   MIME_MakeBody( (LPSTR)GCUTIL_STR::GCStrWideToChar( strNote_.c_str()), 0);
    m_kMail.password    =   szDecrypt;
    
    char **pFiles=NULL;
    int cFiles = (int)vecFileList_.size();
    for( int i = 0 ; i < cFiles ; ++i )
    {
        if( i == 0 )
            pFiles=(char**)malloc(sizeof(char*));
        else
		{
            pFiles=(char**)realloc( pFiles, sizeof( char* ) * (i + 1 ));
			memset(pFiles + (i), 0 , sizeof(char*)*(i +1 - i)); 
		}
		
         pFiles[i]= (char*)vecFileList_[i].c_str();
		
	}



    m_kMail.dataAttach	=MIME_MakeAttach(pFiles, cFiles);
    m_kMail.dataEnd	    =MIME_GetEnd();

    if (!SMTP_SendMail(g_MyD3D->Get_hWnd(), ulSMTPAddr, &m_kMail, MailSendEnd, NULL)){
        //실패
        MIME_Free(m_kMail.dataHead1);
        MIME_Free(m_kMail.dataHead2);
        MIME_Free(m_kMail.dataBody);
        MIME_Free(m_kMail.dataAttach);
        //MessageBox(SMTP_GetLastMsg());	//실패의 원인보여주기
        return false;
    }

    return true;
}

bool GCPairPlaySystem::ClearReportDatas()
{
    m_strUserComment = L"";

    for( int i = 0 ; i < (int)m_vecReportFiles.size() ; ++i )
    {
        DeleteFileW( m_vecReportFiles[i].c_str() );
    }
	m_StrDesc.str(L"");
	m_vecReportFiles.clear();
	m_wstrBadGuyInifo.str(L"");
    m_bFileOut = false;
    return true;
}

bool GCPairPlaySystem::SendReportData( DWORD dwSerialNum_ )
{
    bool bRet = true;
    //저장된 파일을 메일에 첨부해서 보냅니다.
    std::ostringstream strMailTitle;


    //한번 신고를 한 경우에는 그 게임에서는 다시 신고 할 수 없도록 설정
    // 이코드를다이얼로그 뜨는데다 해놓으면, 취소해도 게임중에 신고 1번만 신고된다고 지랄 댄다. bug fix 2008.2.25;
    g_kGlobalValue.m_bReportInGame = true;
	strMailTitle<<KncUtil::toNarrowString(NATION_CODE)<<dwSerialNum_<<" : "<<GCUTIL_STR::GCStrWideToChar( g_kGlobalValue.m_kUserInfo.strNickName.c_str() );

    
	m_StrDesc.clear();
	m_StrDesc << "\r\n<< UserInfo >>\r\n";
    m_StrDesc << "\r\nNickName : " << g_kGlobalValue.m_kUserInfo.strNickName;
    m_StrDesc << "\r\nUserUID : " << g_kGlobalValue.m_kUserInfo.dwUID;
    m_StrDesc << "\r\nuserID : " << g_kGlobalValue.m_kUserInfo.strLogin.c_str();
    m_StrDesc << "\r\nuserComment : " << m_strUserComment.c_str();
    m_StrDesc << "\r\n\r\n\r\n" << m_wstrBadGuyInifo.str();

	std::vector <string> m_vecMailFiles;
	int count = m_vecReportFiles.size();
    for(int i =0; i< count; i++)
	{
		std::string str = GCUTIL_STR::GCStrWideToChar(m_vecReportFiles[i].c_str());
		m_vecMailFiles.push_back(str);
	}
//FTP로 보내는 부분 주석 처리
////FTP로 보낼 경우
//#if defined(NATION_USA) 
//	//신고파일명을 얻어옴
//	//신고 파일명은 : 신고당한 아이의 닉네임(ID).년월일몇시몇분몇초.txt파일로 구성됨
//	m_NotifyFileName = GetNotifyFileName();
//
//	//신고 파일내용을 기록함
//	std::wofstream wFile;
//	IMBUE_NULL_CODECVT( wFile );
//	wFile.open( m_NotifyFileName.c_str(), std::ios::binary | std::ios::out );
//	wchar_t cBom = 0xfeff;
//	wFile.write( &cBom, 1 );
//	wFile << m_StrDesc.str();
//	wFile.close();
//
//	DWORD dwThreadID;
//	//스레드로 신고파일들을 보냄 
//	_beginthreadex(NULL,0,SendFTPFile,this,NULL,&dwThreadID);	  
//
//
//	return bRet;
////Mail로 보낼 경우
//#else
	if( SendMail( strMailTitle.str(), m_StrDesc.str(), m_vecMailFiles ) == false )
	{
		bRet = false;
	}
	return bRet;	
}

bool GCPairPlaySystem::IsFileDeleted()
{
    //파일은 만들고 보내진 않은 상황.
    if( m_bDataSend == false && m_bFileOut == true )
        return true;

    //파일을 보내고 지운 상황
    if( m_bDataSend == true && m_bFileOut == false )
        return true;

    // 파일이 아예 출력이 안된 상황
    if( m_bFileOut == false )
        return true;

    return false;
}

void GCPairPlaySystem::UpdateReportReplayInfo( int iCursor, int iSize )
{
    //일단 데이터를 기록해 준다.
    m_aReportReplay[m_iEndReportReplay].iStreamCursor = iCursor;
    m_aReportReplay[m_iEndReportReplay].iStreamSize = iSize;

    //인덱스를 넘긴다.
    ++m_iEndReportReplay;

    if( m_iEndReportReplay >= MAX_REPLAY_TIME_FOR_REPORT )
        m_iEndReportReplay = 0;

    //한바퀴 돌아온 것으로 간주
    if( m_iEndReportReplay == m_iStartReportReplay )
        ++m_iStartReportReplay;

    if( m_iStartReportReplay >= MAX_REPLAY_TIME_FOR_REPORT )
        m_iStartReportReplay = 0;
}



unsigned int GCPairPlaySystem::SendFTPFile(LPVOID Parameter)
{
	GCPairPlaySystem* pThread = ( GCPairPlaySystem* )Parameter;

	//FTP객체 생성
	pThread->m_GcFtp = new GCFTP();
	
	//FTP에 접속
	if(!pThread->m_GcFtp->ConnectFTP())
	{
		return false;
	}

	//파일을 올릴 디렉토리 명을 설정
	if(!pThread->m_GcFtp->SetDirectory(pThread->GetOutPutFileName(),pThread->m_Today))
	{
		return false;
	}

	//신고 파일을 보낸다
	if(!pThread->m_GcFtp->SendFile(pThread->m_NotifyFileName))
	{
		return false;
	}
	//보낸 파일을 resource폴더내에서 지운다.
	DeleteFileW(pThread->m_NotifyFileName.c_str());

	//스레드로 생성시에 아래의 함수가 불리지 않아 이 함수 안에서 직접 호출함
	pThread->OutPutFilesForReport();
	
	//****메일, 리플레이 파일을 FTP에 보낸다***
	//파일을 수를 얻어옴
	int ReplayChatFileCount = (int)pThread->m_vecReportFiles.size();
	

	//얻어온 파일의 수만큼 FTP서버에 추가해줌
	for(int i = 0; i < ReplayChatFileCount; i++)
	{
		if(!pThread->m_GcFtp->SendFile(pThread->m_vecReportFiles[i]))
		{
			return false;
		}
	}

	//채팅 파일 ftp파일을 지운다.
	pThread->ClearReportDatas();
	pThread->m_newtime = NULL;
	return 0;
}

std::wstring GCPairPlaySystem::GetToday()
{
	//현재 날짜,시간을 초단위까지 얻어온다.
	tm *newtime;
	time_t aclock;
	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  

	std::wstringstream wstrStream;
	wstrStream<< newtime->tm_year + 1900 <<L"-"<<newtime->tm_mon + 1<<L"-"<<newtime->tm_mday;

	return wstrStream.str();
}
