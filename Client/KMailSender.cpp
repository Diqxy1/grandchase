#include "stdafx.h"
#include ".\kmailsender.h"


#include <KSingleton.h>
#include "SystemAnalysis/SystemAnalysis.h"




///////////////////////////////////////
#if defined(USE_HTTPPOST)
#include "curl/curl.h"
#endif


#include "Zip/Zipper.h"



static bool bMailSend = false;

static _Mail kMail;



KMailSender::KMailSender(void)
{
}

KMailSender::~KMailSender(void)
{
}



void MailSendFin( BOOL bSuccess_ )
{
    bMailSend = true;
}

void KMailSender::SendMail( std::string strSMTP, std::string strFrom, std::string strTo, std::string strTitle, std::string strBody, HWND hWnd)
{
    bMailSend = false;
    //LoadDLL();

    MIME_Free(kMail.dataHead1);
    MIME_Free(kMail.dataHead2);
    MIME_Free(kMail.dataBody);
    MIME_Free(kMail.dataAttach);

    //아래 리턴이 INADDR_NONE이면 잘못된거져..--;;
    unsigned long ulSMTPAddr=SMTP_TransAddr( (char*)strSMTP.c_str() );

    if( ulSMTPAddr == INADDR_NONE )
        return;

    //_Mail kMail;	//이구조체만 채움 되는디.

    kMail.from		=   (char*)strFrom.c_str();
    kMail.to		    =   (char*)strTo.c_str();
    kMail.cc		    =  NULL;
    kMail.bcc		    =   NULL;//m_val_strBCC.GetLength()?(LPSTR)(LPCSTR)m_val_strBCC:NULL;
    kMail.dataHead1	    =   MIME_MakeHead1(kMail.to, kMail.cc);
    kMail.dataHead2	    =   MIME_MakeHead2( (char*)strFrom.c_str()  , kMail.from , (char*)strTitle.c_str() , 0 );
    kMail.dataBody	    =   MIME_MakeBody( (char*)strBody.c_str(), 0);
    kMail.dataEnd	    =   MIME_GetEnd();

    if (!SMTP_SendMail( hWnd , ulSMTPAddr, &kMail, MailSendFin, NULL ))
    {
        //실패
        MIME_Free(kMail.dataHead1);
        MIME_Free(kMail.dataHead2);
        MIME_Free(kMail.dataBody);
        MIME_Free(kMail.dataAttach);        

    }

    MSG Message;

    while( !bMailSend )
    {
        if ( GetMessage(&Message,0,0,0) )
        {
            TranslateMessage( &Message );
            DispatchMessage( &Message );
        }
    }
}


void KMailSender::SendCurrentGameGuardInfo( std::string strAddInfo )
{
	std::string strFullInfo;
	std::string strTitle;
	std::vector< std::string > vecFileList_;
	CZipper zipper;
	std::string strZipFileName;

	char temp[1024] = {0,};

	strFullInfo += "\nCPU : ";
	strFullInfo += KncUtil::toNarrowString( KSingleton<KSystemAnalysis>::GetInstance()->GetSystemCpuData() ).c_str() ;
	strFullInfo += "\nDXVersion : ";
	strFullInfo += KncUtil::toNarrowString( KSingleton<KSystemAnalysis>::GetInstance()->GetSystemDirectXVersion() ).c_str() ;
	strFullInfo += "\nOS : ";
	strFullInfo += KncUtil::toNarrowString( KSingleton<KSystemAnalysis>::GetInstance()->GetSystemOsData() ).c_str() ;

#if defined( USE_XTRAP )
		strTitle += "XTRAP.";
#elif !defined(NO_GAMEGUARD)
		strTitle += "NPGG.";
#endif
	strTitle += KncUtil::toNarrowString(NATION_CODE).c_str();
	strTitle +=".";
	strTitle += strAddInfo.c_str();
	//strTitle += g_kGlobalValue.GetGameVersionA().c_str();

	CTime time = CTime::GetCurrentTime();
	strZipFileName = (CStringA)time.Format( _T("%Y%m%d_%H%M%S") );

	strZipFileName = strAddInfo+"Report"+strZipFileName+".zip";

	zipper.CreateZipFile(GCUTIL_STR::GCStrCharToWide(strZipFileName.c_str()));

#if !defined(NO_GAMEGUARD)
		::CopyFileA("GameGuard\\npgg.erl","GameGuard\\npgg.erl_" , FALSE );
		::CopyFileA("GameGuard\\npgl.erl","GameGuard\\npgl.erl_" , FALSE );
		::CopyFileA("GameGuard\\npgm.erl","GameGuard\\npgm.erl_" , FALSE );
		::CopyFileA("GameGuard\\npgmup.erl","GameGuard\\npgmup.erl_" , FALSE );
		::CopyFileA("GameGuard\\npsc.erl","GameGuard\\npsc.erl_" , FALSE );	

		zipper.AddFile(L"GameGuard\\npgg.erl_");
		zipper.AddFile(L"GameGuard\\npgl.erl_");
		zipper.AddFile(L"GameGuard\\npgm.erl_");
		zipper.AddFile(L"GameGuard\\npgmup.erl_");
		zipper.AddFile(L"GameGuard\\npsc.erl_");
#elif defined( USE_XTRAP )
		HANDLE hFile = ::CreateFileA("xtrap", GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		zipper.AddFile(L"xtrap");
		CloseHandle(hFile);
#endif
		zipper.Close();

	vecFileList_.clear();
	vecFileList_.push_back(strZipFileName);

	char **pFiles=NULL;
	int cFiles = vecFileList_.size();
	for( int i = 0 ; i < cFiles ; ++i )
	{
		if( i == 0 )
			pFiles=(char**)malloc(sizeof(char*));
		else
			pFiles=(char**)realloc( pFiles, sizeof( char* ) * (i + 1 ));
		pFiles[i]= (char*)vecFileList_[i].c_str();
	}

	SendHttpPost( HTTP_URL, strTitle, strFullInfo, pFiles[0]);

#if !defined(NO_GAMEGUARD)
	DeleteFileA( "GameGuard\\npgg.erl_" );
	DeleteFileA( "GameGuard\\npgl.erl_" );
	DeleteFileA( "GameGuard\\npgm.erl_" );
	DeleteFileA( "GameGuard\\npgmup.erl_" );
	DeleteFileA( "GameGuard\\npsc.erl_" );	
#elif defined( USE_XTRAP )
	DeleteFileA( "xtrap" );	
#endif

	DeleteFileA( strZipFileName.c_str());
}

void KMailSender::SendHttpPost( std::string strTo, std::string strTitle, std::string strBody, std::string strFileName )
{
#if defined(USE_HTTPPOST)

	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";


	curl_global_init( CURL_GLOBAL_ALL );

	
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "TITLE",
		CURLFORM_COPYCONTENTS, 	(char*)strTitle.c_str(),
		CURLFORM_END);

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "TEXT",
		CURLFORM_COPYCONTENTS, (char*)strBody.c_str(),
		CURLFORM_END);

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "BINARY",
		CURLFORM_FILE, (char*)strFileName.c_str(),
		CURLFORM_END);

	CURL *ctx = curl_easy_init();

	if( NULL == ctx) 
	{
		START_LOG( cerr, "Unable to initialize cURL interface" );
		return;
	}

	headerlist = curl_slist_append(headerlist, buf);


	/* what URL that receives this POST */
	curl_easy_setopt(ctx, CURLOPT_URL, (char*)strTo.c_str());

	curl_easy_setopt(ctx, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(ctx, CURLOPT_HTTPPOST, formpost);
	res = curl_easy_perform(ctx);

	curl_easy_cleanup(ctx);
	curl_formfree(formpost);
	curl_slist_free_all (headerlist);

#endif

}
