#pragma once

////#include <deque>
////#include <map>
////#include <vector>
#include <KNCSingleton.h>
//#include <sstream>
#include "KncP2PLib/RaknetSrc/BitStream.h"

#include "Newpatcher/MailSMTPHeader.h"
#include "Newpatcher/MailMIMEHeader.h"
#include "GCFTP.h"
#define DUMMY_REPLAY_FILE_NAME L"PairPlay.DAT"

struct SReportReplay
{
    int iStreamCursor;
    int iStreamSize;

    SReportReplay()
    {
        iStreamCursor = 0;
        iStreamSize = 0;
    }
};

class GCPairPlaySystem
{
    // [8/2/2007 breadceo] enum, typedef
public:
    enum PPS_CHAT_TYPE
    {
        PPSCT_LOBBY = 0,
        PPSCT_ROOM,
        PPSCT_GAME,
        PPSCT_GUILD,

        PPSCT_NUM,
    };

    typedef std::pair< std::wstring, std::wstring > PairChatMsg;
    typedef std::deque< PairChatMsg > ChatLogDeque;
    typedef std::map< PPS_CHAT_TYPE, ChatLogDeque >::iterator IterChatLog;

    // [8/2/2007 breadceo] Singleton
    DeclareSingleton( GCPairPlaySystem );
public:
    GCPairPlaySystem(void);
    virtual ~GCPairPlaySystem(void);

    void AddChatLog( GCPairPlaySystem::PPS_CHAT_TYPE eType, const std::wstring& strNickName, const std::wstring& strMsg );
    void ClearChatLog( GCPairPlaySystem::PPS_CHAT_TYPE eType );

    bool OutputChatLog( const std::wstring& strFileName, const std::vector< std::wstring >& vecUsers, 
        GCPairPlaySystem::PPS_CHAT_TYPE eType = PPSCT_NUM );
    void MakeChatLog( const ChatLogDeque& deq, const std::vector< std::wstring >& vecUsers, 
        std::wstringstream& stm );
    std::wstring EnumChatTypetoStr( GCPairPlaySystem::PPS_CHAT_TYPE eType );

    void SaveDummyReplayHeadData();
    void SaveDummyReplayData();
    bool MakeDummyReplay( bool bStreamReset = true , bool bReport = false );

    void ClearDummyReplay();

    bool IsDummyReplaySucceed() { return m_bDummyReplay; }

    bool IsDummyReplayMode();

    //Send Report
    bool SendReport( DWORD dwBadGuy_ , int iReason_, std::wstring strComment_ , const std::wstring& NickName_ , const std::wstring strLogin);
    bool SendReport( std::wstring strNickName_ , int iReason_, std::wstring strComment_  );

    //Send Mail & Files
    bool SendReportData( DWORD dwSerialNum_ );
    void OutPutFilesForReport();
    bool ClearReportDatas();
    std::wstring GetOutPutFileName();
    bool MakeZipFile( std::string strOutputFileName_ , std::vector< std::wstring > vecFileList_ );
    bool SendMail( std::string title_, std::wstring strNote_, const std::vector< std::string >& vecFileList_ );
    static void MailSendEnd( BOOL bSuccess_ );
    bool IsFileDeleted();

	// 2009.04.02 kamd0: 신고내용파일 얻어오는 함수, FTP로 파일 보내는 함수 추가,신고당한 유저 nickname얻기 추가
	std::wstring GetNotifyFileName();
	inline void SetBadGuyNickName(std::wstring NickName){ m_BadGuyID = NickName;}

	// 2009.04.09 kamd0: 오늘의 날짜(년일월)를 얻어온다.
	std::wstring GetToday();

	static unsigned int WINAPI SendFTPFile(LPVOID Parameter); 

private:
    // [8/2/2007 breadceo] Chat Type 에 따른 Map
    std::map< PPS_CHAT_TYPE, ChatLogDeque > m_mapChatLog;

    // [8/3/2007 breadceo] 임시 replay stream
    RakNet::BitStream m_stmReplay;

    // [8/3/2007 breadceo] Replay 와의 호완을 위해 존재
    int m_iFeed;

    // [8/3/2007 breadceo] 더미 리플레이 생성 성공?
    bool m_bDummyReplay;

    // 2007.08.20 milennium9 메일 보내기위한 객체
    static _Mail m_kMail;

    // 2007.08.20 milennium9 리플레이와 채팅로그 삭제를 위한 리스트
    std::vector<std::wstring>  m_vecReportFiles;

    // 2007.08.20 milennium9 유저의 멘트
    std::wstring m_strUserComment;
    std::wstringstream m_wstrBadGuyInifo;

	// 2009.04.01 kamd0:신고대상의 별명를 저장. 추후 ID로 수정 예정
	std::wstring m_BadGuyID;

	// 2009.04.09 kamd0:오늘 날짜를 저장하는 변수
	std::wstring m_Today;

    // 2007.09.05 milennium9 파일 출력했음을 기억하는 변수. 나중에 지우려고..
    static bool m_bFileOut;
    static bool m_bDataSend;

	// 2009.04.06 kamd0:FTP를 접속하기 위한 객체
	GCFTP *m_GcFtp;

	std::wstringstream m_StrDesc;

	std::wstring m_NotifyFileName;

	//2009.04.14 kamd0:모든 신고 파일들의 시간을 동일하게 하기 위한 시간 변수
	tm *m_newtime;


    // 2007.09.06 한시간 넘게 리플레이를 찍었을 때, 메일로 날리기 빡신 용량이 된다. 
    // 환형 배열을 사용해서 30분으로 제한하자.
#define MAX_REPLAY_TIME_FOR_REPORT      1800    //1800초 기록한다. 60초 * 30분
    KSafeArray<SReportReplay, MAX_REPLAY_TIME_FOR_REPORT > m_aReportReplay;
    int m_iStartReportReplay;
    int m_iEndReportReplay;
    void UpdateReportReplayInfo( int iCursor, int iSize );

};

DefSingletonInline( GCPairPlaySystem );
