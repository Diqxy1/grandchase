#include "curlsession.h"
#include <sys/stat.h>
#include "../kom/kom.h"


int KCurlSession::totalwrite;
std::string KCurlSession::m_strLogFile = "";

extern void ResetReadOnly(std::string path);

bool KCurlSession::SplitURL( std::string url,std::string &server,std::string &path )
{
	std::string protocol;
	std::string urlinlower(url);	
	std::transform(urlinlower.begin(),urlinlower.end(),urlinlower.begin(),tolower);
	int serverstart = urlinlower.find_first_of("://");
	
	if(serverstart<0 )
	{
		return false;
	}

	protocol = urlinlower.substr(0,serverstart);


	std::string urlwoprotocol = url.substr(serverstart+3);

	int servernameend = urlwoprotocol.find_first_of("/");
	if(servernameend<0 )
		return false;

	server = urlwoprotocol.substr(0,servernameend);
	path = urlwoprotocol.substr(servernameend);

	return true;
}

KCurlSession::KCurlSession()
:m_ctx(NULL),exerename(false)
{	
	m_ctx = curl_easy_init();
}

KCurlSession::KCurlSession( std::string id,std::string pw ,bool rename)
:m_ctx(NULL),id(id),pw(pw),exerename(rename)
{
	m_ctx = curl_easy_init();
}
KCurlSession::~KCurlSession()
{
	curl_easy_cleanup(m_ctx);
}


size_t CurlWrite( void *source , size_t size , size_t nmemb , void *userData )
{	
	SRecieveInfo *recieveinfo = (SRecieveInfo*)userData;	

	size_t recievesize = size * nmemb;
	
	if(recievesize>0)
	{	
		recieveinfo->stream.write((const char*)source,recievesize);
		if(recieveinfo->stream.fail())
		{
			recieveinfo->stream.clear();
			return 0;
		}

		if( recieveinfo->IsStop())
			return 0;	
		recieveinfo->Read(recievesize);
		KCurlSession::AddTotalWriteLength(recievesize);
		return recievesize;
	}
	return 0;
}


//	프로그래스를 이용하면 원격지의 사이즈를 가져올수 있다.
//  필요없지만 우선 이걸로 원격지의 사이즈를 알아낸다.
int Progress_Func(SRecieveInfo *recieveinfo,
					 double t, /* dltotal */ 
					 double d, /* dlnow */	
					 double ultotal,
					 double ulnow)
{
	if (t>0)
	{
		int nfileSize = static_cast<int>(t);
		int nfileSizeOrg = recieveinfo->GetFileSize();
		if ( nfileSizeOrg > 0 && nfileSizeOrg != nfileSize )
		{
			recieveinfo->SetFileSize(nfileSize);
		}
	}

	return 0;
}

bool KCurlSession::WriteRemoteData( std::string url,std::ostream &stream,int offset,int length,
								   NETPROGRESS_CALLBACK progress ,KCSLockType<SProgressInfo> &progressinfo,int & error)
{

	
	SRecieveInfo recieveinfo(url,stream,offset,length,progress,progressinfo);

	if(recieveinfo.IsStop())
		return true;

	START_LOG( cout, L"down start file " << url << L" range : " << offset << L" " << length );

	int start = 0;
	while((start = url.find_first_of(" ")) > 0)
	{
		url.replace(start,1,"%20");
	}	

	struct curl_slist *headerlist=NULL;
	char range[64];
	
	if(offset>=0 && length>0)
	{		
		_snprintf_s(range,64,"%d-%d",offset,offset+length-1);	
		curl_easy_setopt(m_ctx,CURLOPT_RANGE, range);
	}	

	if(id.length()>0 && pw.length()>0)
	{
		std::string userpassword = id+":"+pw;
		curl_easy_setopt(m_ctx, CURLOPT_USERPWD, userpassword.c_str()); 
	}

	if(url.find_first_of("http://") == 0)
	{
		curl_easy_setopt(m_ctx,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_1);
		curl_easy_setopt(m_ctx,CURLOPT_HTTP_CONTENT_DECODING,0);
		curl_easy_setopt(m_ctx,CURLOPT_HTTP_TRANSFER_DECODING,0);	
		curl_easy_setopt(m_ctx,CURLOPT_HTTPGET,1);        
	}	
	else if( url.find_first_of("fttp://") == 0 )
	{
		curl_easy_setopt(m_ctx,CURLOPT_FTPPORT, "-");
        curl_easy_setopt(m_ctx,CURLOPT_FTP_USE_EPRT, 0);
	}
	
	curl_easy_setopt(m_ctx,CURLOPT_WRITEDATA, (void*)&recieveinfo);	
	curl_easy_setopt(m_ctx,CURLOPT_WRITEFUNCTION,CurlWrite);
	curl_easy_setopt(m_ctx,CURLOPT_URL,url.c_str());
    curl_easy_setopt(m_ctx,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(m_ctx,CURLOPT_MAXREDIRS,10);
	curl_easy_setopt(m_ctx, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(m_ctx,CURLOPT_PROGRESSFUNCTION, Progress_Func);
	curl_easy_setopt(m_ctx, CURLOPT_PROGRESSDATA, &recieveinfo);
    

#if defined ( PATCHER_CURL_LOG )

    if ( m_strLogFile.empty() )
    {
        CStringA strTime;
        CTime cTime = CTime::GetCurrentTime();
        strTime.Format(".\\Log\\Curl_%04d-%02d-%02d %02d-%02d-%02d.txt", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

        m_strLogFile = strTime.GetString();
    }


    FILE* pFile = fopen( m_strLogFile.c_str(), "at" );

    if ( pFile ) 
    {
        //  Debug 출력 모드용
        curl_easy_setopt(m_ctx, CURLOPT_VERBOSE, 1L );
        curl_easy_setopt(m_ctx, CURLOPT_STDERR, pFile );
    }
#endif


//	DWORD dwTick = GetTickCount();

	CURLcode curlcode = curl_easy_perform(m_ctx);

	long Errcode = 0;
	curl_easy_getinfo (m_ctx, CURLINFO_RESPONSE_CODE, &Errcode);
	curl_slist_free_all (headerlist);

	double fSpeed = 0;
	curl_easy_getinfo( m_ctx, CURLINFO_SPEED_DOWNLOAD, &fSpeed );

//	dwTick = GetTickCount() - dwTick;

	START_LOG( cout, L"down complate file " << url << L" range : " << offset << L" " << length );
//	START_LOG( cout, L"down Speed " << fSpeed/1024 );
//	START_LOG( cout, L"Tick Count " << dwTick );

#if defined ( PATCHER_CURL_LOG )
    if ( pFile ) 
        fclose( pFile );
#endif
	

	if(url.find_first_of("http://") == 0 && (Errcode<200 || Errcode>299))
	{	
		error = (int)Errcode;		
        START_LOG( cout, L"Error string: " << curlcode << " " << curl_easy_strerror( curlcode ) );
		START_LOG( cout, L"http code Type" << Errcode << " " << url );
		return false;
	}
	else if( url.find_first_of("fttp://") == 0 && (Errcode<200 || Errcode>299 && Errcode != 426 ) )
	{
		error = (int)Errcode;
        START_LOG( cout, L"Error string: " << curlcode << " " << curl_easy_strerror( curlcode ) );
		START_LOG( cout, L"ftp code Type" << Errcode << " " << url );
		return false;
	}

	if(curlcode == CURLE_OK)
	{
		return true;	
	}
				
	error = (int)curlcode;	

	return false;
}




std::string KCurlSession::RenameExe( std::string filename )
{
	std::string extention = filename.substr(filename.length()-4,4);
	std::transform(extention.begin(),extention.end(),extention.begin(),tolower);

	if(extention==".exe" || extention==".dll")
	{
		// Main.exe를 Kom으로 압축했을 경우
		std::string strMainFile = "main.exe";
		std::string strMainKomFile = "main.kom";
		
		UINT nPos = filename.find(strMainFile);
		if( nPos != std::string::npos )
		{
			filename.replace(nPos, strMainFile.length(),strMainKomFile);
		}
		else
		{
			filename.append("_");
		}		
	}
	return filename;
}

bool KCurlSession::DownloadFile( std::string url,std::string filename,int off /*= 0*/,int length/*=-1*/,
								NETPROGRESS_CALLBACK progress/*=NULL*/,KCSLockType<SProgressInfo> &progressinfo,int & error)
{	
	std::string filename_ = filename;
	if(exerename == true)
	{
		filename_ = RenameExe(filename_);
		url = RenameExe(url);		
	}

	std::ofstream filestream(filename_.c_str(),std::ios::binary);
	if(filestream.is_open() == false)
	{	
		error= -2;
		return false;
	}

	if(WriteRemoteData(url,filestream,off,length,progress,progressinfo,error)==false)
	{	
		filestream.close();
		return false;
	}
	filestream.close();

	//	main.kom일때 파일푼다.
	std::string strMainFile = "main.exe";
	std::string strMainKomFile = "main.kom";

	UINT nPos = filename_.find(strMainKomFile);
	if( nPos != std::string::npos )
	{
		Komfile komFile;
		if ( !komFile.Open(filename_) )
		{
			return false;
		}

		//	실제 kom파일내에는 main.exe_가 압축되어 있다.
		//  근데 어차피 main.exe로 바꿔야 하니 압축 풀면서 main.exe로 바꾼다.
		KSubfile subfile = komFile.GetSubfile("main.exe_");
		if ( !subfile.WriteDecompressedFile(strMainFile) )
		{
			return false;
		}

		DeleteFileForce(filename_);

		//	압축푼거 파일명 변경한다.
		UINT nPos = filename_.find(strMainKomFile);
		if( nPos != std::string::npos )
		{
			filename_.replace(nPos, strMainKomFile.length(),strMainFile);
		}
	
	}
	
	if(filename_ != filename)
	{
		if(MoveFileA(filename_.c_str(),filename.c_str())==FALSE)
		{
			error = -7;
			return false;
		}
	}
	return true;
}



bool KCurlSession::WriteRemoteData( std::string url,std::ostream &stream,int off/*=0*/,int length )
{
	KCSLOCK_DECLARE(SProgressInfo,progressinfo);
	int error;
	return WriteRemoteData(url,stream,off,length,NULL,progressinfo,error);
}

bool KCurlSession::DownloadFile( std::string url,std::string filename,int off/*=0*/,int length )
{
	KCSLOCK_DECLARE(SProgressInfo,progressinfo);
	int error;
	return DownloadFile(url,filename,off,length,NULL,progressinfo,error);
}


static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}
	return ret;
}


bool KCurlSession::Report( std::string url,std::string service,std::string version,char * data,int size )
{
	std::string base64 =  base64_encode((unsigned char*)data,size);

	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL;	
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "service", CURLFORM_COPYCONTENTS, service.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "version", CURLFORM_COPYCONTENTS, version.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "data", CURLFORM_PTRCONTENTS, base64.c_str(),CURLFORM_CONTENTSLENGTH,base64.length(), CURLFORM_END);

	/* Set the form info */
	curl_easy_setopt(m_ctx, CURLOPT_URL, (char*)url.c_str());
	curl_easy_setopt(m_ctx, CURLOPT_HTTPPOST, post);

	CURLcode re =curl_easy_perform(m_ctx); /* post away! */

	/* free the post data again */
	curl_formfree(post);
	
	if(re!=CURLE_OK)
		return false;
	return true;
}

bool KCurlSession::ReportCN( std::string url,std::string strGameType, std::string strGameVersion, std::string strMac, int iState ) 
{
	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL;	
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "gametype", CURLFORM_COPYCONTENTS, "yhmx", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "version", CURLFORM_COPYCONTENTS, "12.125.10", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "param", CURLFORM_COPYCONTENTS, "1", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "mac", CURLFORM_COPYCONTENTS, "12.125.10", CURLFORM_END);

	/* Set the form info */
	curl_easy_setopt(m_ctx, CURLOPT_URL, (char*)url.c_str());
	curl_easy_setopt(m_ctx, CURLOPT_HTTPPOST, post);

	CURLcode re =curl_easy_perform(m_ctx); /* post away! */

	/* free the post data again */
	curl_formfree(post);

	if(re!=CURLE_OK)
		return false;
	return true;
}


bool KCurlSession::ReportforGCLOG( std::string url,std::string service,std::string version, bool success,
								  std::string lasterror, int lasterrorcode, bool ismannual,std::string patchserverip,DWORD patchtime,
								   unsigned int local_info,unsigned int server_info,bool isEndReport,bool isPatch)
{
#ifndef USE_GC_LOG_SYSTEM
	return true;
#endif
	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL;	

	char szTemp[256],szTemp2[256];
	char szOldversion[16];
	char szNewVersion[16];

	sprintf(szOldversion,"%x",local_info);
	sprintf(szNewVersion,"%x",server_info);

	char name[100] = "";
	char ip[100] = "";
	PHOSTENT hostinfo;
	gethostname(name, sizeof(name));
	hostinfo = gethostbyname(name);
	strcpy(ip, inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));

	char strErr[32] = "";
	itoa(lasterrorcode,strErr,10);
	

	if (isEndReport == false)
		{
			//이제 mkom안씀
			//if (ismannual==true) //메뉴얼 패치의 경우 mkom이 없어 kom받는 경우가 당연한거다 리포트할 필요 없음.
			//	return;
	
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ty", CURLFORM_COPYCONTENTS, "RT_ERROR", CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "cd", CURLFORM_COPYCONTENTS, service.c_str(), CURLFORM_END);

			//원래는 에러메시지중 중요부분 스트링인데 일단 동일하게 처리
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "pm", CURLFORM_COPYCONTENTS, lasterror.c_str(), CURLFORM_END); 
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "lm", CURLFORM_COPYCONTENTS, strErr, CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "sz", CURLFORM_COPYCONTENTS, version.c_str(), CURLFORM_END);

			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ip", CURLFORM_COPYCONTENTS, patchserverip.c_str(), CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ci", CURLFORM_COPYCONTENTS, ip, CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ov", CURLFORM_COPYCONTENTS, szOldversion, CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "nv", CURLFORM_COPYCONTENTS, szNewVersion, CURLFORM_END);

		}
		else //패치 종료 패킷인 경우.
		{
			if (( GetTotalWriteLength() /1048576 < 1) || (std::string)szOldversion == (std::string)szNewVersion)
				isPatch = false;

			sprintf(szTemp,"%d",GetTotalWriteLength()/1048576);
			sprintf(szTemp2,"%d",patchtime/60000);
			
	
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ty", CURLFORM_COPYCONTENTS, isPatch ? "RT_PATCHEND":"RT_SKIP", CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "cd", CURLFORM_COPYCONTENTS, service.c_str(), CURLFORM_END); //국가코드
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "pm", CURLFORM_COPYCONTENTS, success ? "Success" : "Failure", CURLFORM_END);  // -- 성공 실패 여부
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "lm", CURLFORM_COPYCONTENTS, szTemp, CURLFORM_END); // -- 패치용량.
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "sz", CURLFORM_COPYCONTENTS, szTemp2, CURLFORM_END); // -- 다운로드 시간.
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ip", CURLFORM_COPYCONTENTS, patchserverip.c_str(), CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ci", CURLFORM_COPYCONTENTS, ip, CURLFORM_END);
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "ov", CURLFORM_COPYCONTENTS, szOldversion, CURLFORM_END); //구버젼
			curl_formadd(&post, &last, CURLFORM_COPYNAME, "nv", CURLFORM_COPYCONTENTS, szNewVersion, CURLFORM_END); //새버젼
	
	
		}
	
	/* Set the form info */
	curl_easy_setopt(m_ctx, CURLOPT_URL, (char*)url.c_str());
	curl_easy_setopt(m_ctx, CURLOPT_HTTPPOST, post);

	CURLcode re =curl_easy_perform(m_ctx); /* post away! */

	/* free the post data again */
	curl_formfree(post);

	if(re!=CURLE_OK)
		return false;
	return true;
}

//void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg,int iType, bool bReDirect)
//{
//	GenericHTTPClient http;
//	//std::string strPostMsg = (std::string)"http://localhost:1116/default.aspx";
//	std::string strPostMsg = (std::string)"http://116.120.238.62:7070/insert_report/Default_ver2.aspx";
//	std::string strTitle = GCUTIL_STR::GCStrWideToChar(NATION_CODE);
//#if defined(__OPEN_TEST__)
//	strTitle += "[OPENTEST]";	
//#elif defined(_TEST)
//	strTitle += "[internal]";
//#endif
//
//
//	std::string strMsg;
//	char szTemp[256];
//	strMsg="";
//
//	// 로그중에 특정 문자열이 들어가 있는 녀석들은 다 포함시켜 준다.
//	for (int i =0 ; i < g_strLogs.size() ; i++)
//	{
//		if (g_strLogs[i].find(strParseMsg) != std::string::npos)
//		{
//			strMsg+=g_strLogs[i];
//		}
//	}
//
//	g_Msg = g_Msg + "\n" + strMsg;	
//	g_Msg+="\nIP:"+GetIPAddress()+"\n";
//
//	if (g_Msg.length()>1500)
//	{
//		g_Msg=g_Msg.substr(g_Msg.length()-1500,1500);
//	}
//
//	if (iType == ReportType::RT_ERROR)
//	{
//		if (g_bManualPatch==true) //메뉴얼 패치의 경우 mkom이 없어 kom받는 경우가 당연한거다 리포트할 필요 없음.
//			return;
//
//		http.AddPostArguments("ty","RT_ERROR");
//		http.AddPostArguments("cd",strTitle.c_str());
//		http.AddPostArguments("pm",strParseMsg.c_str());	
//		http.AddPostArguments("lm",strLastMsg.c_str());	
//		http.AddPostArguments("mg",g_Msg.c_str(),true);
//
//		if (bReDirect==true)
//		{
//			http.AddPostArguments("ip",GCFTP::GetHostRedirectIPName().c_str()); // CDN 서버 IP 주소
//		}
//		else
//		{
//			http.AddPostArguments("ip",GCFTP::GetHostIPName().c_str()); // CDN 서버 IP 주소
//		}
//
//		http.AddPostArguments("ov",g_OldVersion.c_str());    // 구버젼
//		http.AddPostArguments("nv",g_NewVersion.c_str());    // 새버젼
//	}
//	else //패치 종료 패킷인 경우.
//		//if (iType == ReportType::RT_PATCHEND || iType == ReportType::RT_MANUAL_PATCHEND)
//	{
//		if ((strTitle=="Success" && g_dwRecvFileSize/1048576 < 1) || g_OldVersion == g_NewVersion)
//			return;
//
//		sprintf(szTemp,"%d",g_dwRecvFileSize/1048576);
//		strLastMsg = szTemp; 
//
//		http.AddPostArguments("ty",iType == ReportType::RT_PATCHEND ? "RT_PATCHEND" : "RT_MANUAL_PATCHEND");
//		http.AddPostArguments("cd",strTitle.c_str());  // 국가코드
//		http.AddPostArguments("pm",strParseMsg.c_str());	// -- 성공 실패 여부
//		http.AddPostArguments("lm",strLastMsg.c_str());	    // -- 패치용량.
//		http.AddPostArguments("sz",(timeGetTime() - g_PatcherStartTime)/60000);     // 다운로드 용량
//
//		if (bReDirect==true)
//		{
//			http.AddPostArguments("ip",GCFTP::GetHostRedirectIPName().c_str()); // CDN 서버 IP 주소
//		}
//		else
//		{
//			http.AddPostArguments("ip",GCFTP::GetHostIPName().c_str()); // CDN 서버 IP 주소
//		}
//		http.AddPostArguments("ov",g_OldVersion.c_str());    // 구버젼
//		http.AddPostArguments("nv",g_NewVersion.c_str());    // 새버젼
//
//
//	}
//
//	BOOL bOK = http.Request(strPostMsg.c_str(),GenericHTTPClient::RequestPostMethod);
//}

bool KCurlSession::SendFile( std::string url,std::string filename )
{	

	struct curl_slist *headerlist=NULL;
	const char buf[] = "Expect:";

	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL;
	
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "REPORT",
		CURLFORM_FILE, (char*)filename.c_str(),
		CURLFORM_END);

	headerlist = curl_slist_append(headerlist, buf);

	
	
	curl_easy_setopt(m_ctx, CURLOPT_URL, (char*)url.c_str());
	curl_easy_setopt(m_ctx, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(m_ctx, CURLOPT_HTTPPOST, post);

	CURLcode re =curl_easy_perform(m_ctx); /* post away! */

	/* free the post data again */
	curl_formfree(post);
	curl_slist_free_all (headerlist);	
	if(re!=CURLE_OK)
		return false;
	return true;
}