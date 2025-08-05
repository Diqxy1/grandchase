
#include "patch.h"
#include "Define.h"
#include <tlhelp32.h>
#ifdef GRID_DOWN
#include "GridInclude/GridLoad_Define.h"
#endif
#include "GCGlobalValue.h"

#undef Module32First  
#undef Module32Next  
#undef MODULEENTRY32  
#undef PMODULEENTRY32  
#undef LPMODULEENTRY32 

#undef Process32First  
#undef Process32Next  
#undef PROCESSENTRY32
#undef PPROCESSENTRY32
#undef LPPROCESSENTRY32 


const int nFTPDownSize = 50;

extern void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg, std::string strCapacity, int iType );

KPatchFileInfo::KPatchFileInfo( xmlNode * node )
	:adler32(0),size(0),filetime(0)
{
	xmlAttr * attribute =  node->properties;
	while(attribute)
	{		
		if(attribute->children && attribute->children->content)  
		{
			const int contentlen = strlen((char*)attribute->children->content);

			if(contentlen > 0)
			{
				if(strcmp((char*)attribute->name,"Checksum")==0)
				{
					_snscanf_s((char*)attribute->children->content,contentlen,"%x",&adler32);
				}			
				else if(strcmp((char*)attribute->name,"Size")==0)
				{
					_snscanf_s((char*)attribute->children->content,contentlen,"%d",&size);
				}			
				else if(strcmp((char*)attribute->name,"FileTime")==0)
				{
					_snscanf_s((char*)attribute->children->content,contentlen,"%x",&filetime);
				}
			}
		}		
		attribute=attribute->next;
	}
}

KPatchFileInfo::KPatchFileInfo( std::string filename )
	:adler32(0),size(0),filetime(0)
{
	Komfile kom;
	bool iskom = false;
	if(filename.find_last_of(".kom") == filename.length() - 1)		
		iskom = kom.Open(filename);

	if(iskom)
	{		
		filetime = kom.GetFileTime();
		adler32 = kom.GetAdler32();
		WIN32_FILE_ATTRIBUTE_DATA data;
		GetFileAttributesExA(filename.c_str(),GetFileExInfoStandard,&data);
		size = data.nFileSizeLow;
	}
	else
	{
		adler32 = AdlerCheckSum::adler32(filename.c_str(),&size);
	}		
}



bool KPatchInfo::ParsePatchinfo( char* data ,int len)
{
	XMLDocument doc;
	doc.ReadMemory(data,len);

	xmlNode * patchinfo = doc.GetRootElement();
	if(patchinfo->type == XML_ELEMENT_NODE && strcmp((char*)patchinfo->name,"Patchinfo")==0)
	{	
		xmlAttr * att =  patchinfo->properties;
		while(att)
		{
			if(strcmp((char*)att->name,"RenameExe")==0 && att->children)
			{
				if(strcmp((char*)att->children->content,"true")==0)
				{
					renameexe = true;
				}
				break;							
			}
			att = att->next;
		}		
		
		xmlNode * files = patchinfo->children;

		if(files == NULL)			
			return false;		

		while(files)
		{			
			if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"Files")==0)
			{			
				xmlNode * file = files->children;
				while(file)
				{
					if(file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0)
					{	
						std::string key;
						xmlAttr * attribute =  file->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 && attribute->children)
							{
								key =(char*)attribute->children->content;
								break;							
							}
							attribute = attribute->next;
						}						
						std::transform(key.begin(),key.end(),key.begin(),tolower);
						fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(key, KPatchFileInfo(file)));
					}
					file = file->next;
				}				
			}
			else if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"PatcherFiles")==0)
			{
				xmlNode * file = files->children;
				while(file)
				{
					if(file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0)
					{	
						std::string key;
						xmlAttr * attribute =  file->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 && attribute->children)
							{
								key =(char*)attribute->children->content;
								break;							
							}
							attribute = attribute->next;
						}	
						std::transform(key.begin(),key.end(),key.begin(),tolower);						
						patcherfiles.insert(key);
					}
					file = file->next;
				}
			}
			else if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"VerifyFiles")==0)
			{
				xmlNode * file = files->children;
				while(file)
				{
					if(file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0)
					{	
						std::string key;
						xmlAttr * attribute =  file->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 && attribute->children)
							{
								key =(char*)attribute->children->content;
								break;							
							}							
						}						
						std::transform(key.begin(),key.end(),key.begin(),tolower);						
						verifyfiles.insert(key);
					}
					file = file->next;
				}
			}

			files = files->next;
		}
	}
	else
	{	
		return false;
	}
	return true;
}


bool KPatchInfo::Open(KCurlSession &session,std::string url )
{	
	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);;
	if(session.WriteRemoteData(url,stream)==false)
		return false;

	verifyfiles.clear();
	patcherfiles.clear();
	fileinfos.clear();

	int len = stream.tellp();
	
	if ( len <= 0 ) 
		return false;

	boost::scoped_array<char> data(new char[len]);
	stream.read(data.get(),len);
	return ParsePatchinfo(data.get(),len);
}

bool KPatchInfo::Open( std::string filename )
{
	std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);

	if(filestream.is_open()==false)
		return false;

	verifyfiles.clear();
	patcherfiles.clear();
	fileinfos.clear();

	filestream.seekg (0, std::ios::end);
	int len = (int)filestream.tellg();
	
	if ( len <= 0 ) 
		return false;

	filestream.seekg(0,std::ios::beg);
	boost::scoped_array<char> data(new char[len]);
	filestream.read(data.get(),len);
	filestream.close();

	return ParsePatchinfo(data.get(),len);
}

bool KPatchInfo::Open( KPatchInfo & l,std::string dir )
{
	verifyfiles.clear();
	patcherfiles.clear();
	fileinfos.clear();

	std::map<std::string,KPatchFileInfo> &l_fileinfos = l.GetFileInfos();
	std::map<std::string,KPatchFileInfo>::const_iterator i = l_fileinfos.begin();

	while(i != l_fileinfos.end() && KGCGlobalValue::IsStoped() == false )
	{
		std::string filename(dir);
		if(filename.find_last_of('\\') +1 != filename.length())
			filename.append("\\");

		filename.append((*i).first);

		if(GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES) // 존재하면 인포를 만들자 
		{			
			fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(i->first, KPatchFileInfo(filename)));
		}
		i++;
	}
	return true;
}


void KPatchInfo::Diff( KPatchInfo &l,KPatchInfo&r )
{	
	fileinfos.clear();

	std::map<std::string,KPatchFileInfo>::iterator iter_left = l.fileinfos.begin();

	while(iter_left != l.fileinfos.end() && KGCGlobalValue::IsStoped() == false)
	{
		std::map<std::string,KPatchFileInfo>::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right != r.fileinfos.end())
		{
			if(iter_left->second != iter_right->second)
			{
				fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(iter_left->first,iter_left->second));
			}
		}
		else
		{
			fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(iter_left->first,iter_left->second));
		}	

		iter_left++;
	}
}


UINT KPatchInfo::GetFileSize()
{
	UINT totalsize = 0;
	std::map<std::string,KPatchFileInfo>::iterator mapIter = fileinfos.begin();
    for(; mapIter != fileinfos.end(); ++mapIter)
	{
		    totalsize += mapIter->second.GetSize();
	}
	return totalsize;
}


bool KPatchInfo::operator==( KPatchInfo & r )
{
	if(fileinfos.size()!=r.fileinfos.size())
		return false;

	std::map<std::string,KPatchFileInfo>::iterator iter_left = fileinfos.begin();

	while(iter_left != fileinfos.end())
	{
		std::map<std::string,KPatchFileInfo >::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right == r.fileinfos.end())
			return false;

		if(iter_left->second != iter_right->second)
			return false;		

		iter_left++;
	}
	return true;
}


void KPatchInfo::Erase( std::string filename)
{	
	fileinfos.erase(filename);
}



bool KPatchInfo::IsEmpty()
{	
	return fileinfos.empty();
}

KPatchFileInfo KPatchInfo::Find( std::string filename)
{	
	std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.find(filename);

	if(i != fileinfos.end())
		return i->second;

	return KPatchFileInfo();
}

bool KPatchInfo::Exist( std::string filename)
{	
	std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.find(filename);
	return i!=fileinfos.end();
}

unsigned int KPatchInfo::GetCheckSum()
{
	unsigned int adler=0;
	std::map<std::string,KPatchFileInfo>::iterator iter_left = fileinfos.begin();
	while(iter_left != fileinfos.end())
	{
		adler+= iter_left->second.GetAdler32();
		iter_left++;
	}
	return adler;
}

int KPatchLib::GetPatchMode( std::string patchmodeurl)
{
    KCurlSession session(id,pw,renameexe);
    std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);;
    if(session.WriteRemoteData(patchmodeurl,stream)==false)
        return 0;

    std::string patchmodestr = stream.str();
    int start = patchmodestr.find_first_of("=");
    int end = patchmodestr.find_last_of(";");

    if( start < 0 || end <0)
        return 0;

#if defined( _KOREAN ) || defined( __OPEN_TEST__ ) || defined( _KOREA_INTERNAL_ )
    if(  strstr(patchmodestr.c_str(), "KR") )
        return atoi( patchmodestr.substr(start+1,end).c_str() );
#endif

    return 0;
}

bool KPatchLib::GetPatchPath( std::string patchpathurl)
{
	KCurlSession session(id,pw,renameexe);
	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);;
	if(session.WriteRemoteData(patchpathurl,stream)==false)
		return false;

	std::string patchpathstr = stream.str();
	int start = patchpathstr.find_first_of("<");
	int end = patchpathstr.find_last_of(">");

	if( start < 0 || end <0)
		return false;

	start+=1;	
	patchpath = patchpathstr.substr(start,end-start);

	if(patchpath.find_last_of('/') +1 != patchpath.length())
		patchpath.append("/");

	std::string server;
	std::string path;
	KCurlSession::SplitURL(patchpath,server,path);
	

	struct hostent     *myent;
	struct in_addr  myen;
	long int *add;

	myent = gethostbyname(server.c_str());
	if(myent &&  *myent->h_addr_list)
	{
		add = (long int *)*myent->h_addr_list;
		myen.s_addr = *add;
		patchserverip = inet_ntoa(myen);		
	}

	return true;
}


char tourl(char c)
{
	if(c == '\\')
		return '/';
	return c;
}

bool IsDirectory(std::string dir)
{
	DWORD attribute= GetFileAttributesA(dir.c_str());

	if(attribute != INVALID_FILE_ATTRIBUTES && attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}

std::string GetDir(std::string path)
{
	std::string dirname (path,0,path.find_last_of("\\"));
	return dirname;
}

bool CreateParentDirectory(std::string dir)
{		
	if(IsDirectory(dir) == true)
		return true;
	
	std::string parentdir = GetDir(dir);
	if(IsDirectory(parentdir) ==false)
	{
		if(CreateParentDirectory(parentdir)==false)
			return false;
	}
	
	if(CreateDirectoryA(dir.c_str(),NULL) == 0)
	{
		return false;
	}			
	return true;
}



KPatchLib::KPatchLib()
:ismannual(false)
{
	Init();
}

KPatchLib::KPatchLib( std::string id,std::string pw )
:ismannual(false),id(id),pw(pw)
{
	Init();
}


KPatchLib::KPatchLib( bool mannual,std::string mannualpatchpath )
:ismannual(mannual)
{
	Init();

	char path[2048];

	LPSTR fileptr;	
	GetFullPathNameA(mannualpatchpath.c_str(),2048,path,&fileptr);
	patchpath = path;
    m_OriginalPatchpath = path;
	if(patchpath.find_last_of('\\') +1 != patchpath.length())
		patchpath.append("\\");	
}


void KPatchLib::Init()
{
	patchtime = 0;
	patched = false;
	renameexe = false;
	isstoped = false;
	errorcount = 0;
	mutex = NULL;
	lasterrorcode = 0;
	KGCGlobalValue::m_bIsStoped = false;


	char path[2048];
	GetModuleFileNameA(NULL,path,2048);
	std::string pathstr = path;
	std::transform(pathstr.begin(),pathstr.end(),pathstr.begin(),tolower);

	int last = pathstr.find_last_of('\\');	
	patchername = pathstr.substr(last+1);
	patcherdir = pathstr.substr(0,last+1);

	LPSTR fileptr;	
	GetFullPathNameA(".",2048,path,&fileptr);
#if defined( _DEBUG )
	localpath = path;
#else
    localpath = patcherdir;
#endif
	std::transform(localpath.begin(),localpath.end(),localpath.begin(),tolower);
	if(localpath.find_last_of('\\') +1 != localpath.length())
		localpath.append("\\");	

	vecLangInfo.clear();
}

bool KPatchLib::VerifyPatcherName( std::string name )
{
	std::transform(name.begin(),name.end(),name.begin(),tolower);

	return name == patchername;
}


bool KPatchLib::VerifyPatcherPath( std::string path )
{
	std::transform(path.begin(),path.end(),path.begin(),tolower);

	return path == localpath;
}

std::string KPatchLib::GetLastErrMsg( )
{

	LPSTR lpMsgBuf = NULL;
	if( 0 == ::FormatMessageA( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		::GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPSTR) &lpMsgBuf,
		0,
		NULL ) )
	{
		if( lpMsgBuf )
			LocalFree( lpMsgBuf );
		return std::string("Err");
	}

	std::string strRet( lpMsgBuf );
	::LocalFree( lpMsgBuf );

	return strRet;
}


bool KPatchLib::Run( std::string commandline )
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	ZeroMemory( &pi, sizeof(pi) );	

	if(TRUE == CreateProcessA(NULL,(LPSTR)commandline.c_str(),NULL,NULL,FALSE,0,NULL,localpath.c_str(),&si,&pi))
		return true;

	lasterrorcode = ::GetLastError();

	return false;
}

bool KPatchLib::RunExecute( std::string filename, std::string param )
{

	_SHELLEXECUTEINFOA execinfo;
	ZeroMemory(&execinfo,sizeof(_SHELLEXECUTEINFOA));
	execinfo.cbSize = sizeof(_SHELLEXECUTEINFOA);
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOW;
	execinfo.lpVerb = "open";
	execinfo.lpFile = filename.c_str();
	execinfo.lpParameters = param.c_str();

	bool bExcueSuccess = ShellExecuteExA( &execinfo ); 
	if(bExcueSuccess == true)
	{
		return true;
	}

	lasterrorcode = ::GetLastError();
	if(lasterrorcode == 0)
		lasterrorcode = -1;

	return false;
}


void KPatchLib::DownloadFiles(NETSTATRT_CALLBACK start
                              , NETPROGRESS_CALLBACK progress
                              , NETPROGRESS_CALLBACK progress2
                              , NETERROR_CALLBACK error, COMPLETE_CALLBACK complete
							  , NETGRIDDOWNLOADADD_CALLBACK  addfile
							  , NETGRIDDOWNLOADSTART_CALLBACK startgriddown
							  , IS_START_CALLBACK isStart
							  )
{	
	if ( isStart ) 
	{
		//	업데이트 시작 명령이 올때까지 기다린다.
		while( !isStart() )
		{
			if ( IsStoped() ) 
				return;
				
			Sleep(1);
		}
	}

	if(start) {
		Start();
        start();
	}

	//	다국어 관련 처리
	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		START_LOG( cout, L"patchlib->ConvertFileList()"  );
		// 나중에 patchStart지점으로 옮겨야 함
		// serverinfo.xml 변환 작업
		if ( ConvertFileList() == false ) 
		{
			// 임시코드
			START_LOG( cerr, L"patchlib->ConvertFileList() Fail!!!" ); 
		}
	}


	KCurlSession::SetTotalWriteLength(0);
	DWORD begintime = timeGetTime();

	if(ismannual ==false)
	{	
		std::set<std::string> & filenames = server_info.GetVerifyFiles();
		std::set<std::string>::iterator iter = filenames.begin();
		while(iter!=filenames.end() && IsStoped() ==false)
		{
			if(server_info.Exist(*iter)==true)
			{			
				Komfile kom;			
				bool verify = true;
				if(kom.Open(localpath + *iter))			
					verify = kom.Verify();

				if(verify==false)
				{
					DeleteFileForce(localpath + *iter);
				}
			}
			iter++;
		}
	}

	if(IsStoped() == false)
		GetLocalPatchInfo();

	if(IsStoped() == false)
		Diff();

    // patchinfo.kom 받자 
	if(IsPatchComplete() == false) 
	{	
		KCSLOCK_BLOCK(diff_info)
		{
			if(local_info.Size()/4 < diff_info.Size())
			{	
				DeleteFileForce(localpath+"patchinfo.kom");
				KCurlSession session(id,pw,renameexe);
				if(session.DownloadFile(patchpath+"patchinfo.kom",localpath+"patchinfo.kom") == true)
				{
					patchinfokom.reset(new Komfile());
					if(patchinfokom->Open(localpath+"patchinfo.kom")==false)
					{
						patchinfokom.reset();
					}
				}
			}
		}
	}

    // Diff시 제거된 파일 다운로드 루프
    m_iNumPatchFile = 0;
    KGCGlobalValue::m_dwPatcherStartTime = timeGetTime();

	while(IsPatchComplete() == false && IsStoped() == false)
	{
		std::map<std::string,KPatchFileInfo> fileinfos;
		KCSLOCK_BLOCK(diff_info)
		{	
			fileinfos = diff_info.GetFileInfos();
            if( 0 == m_iNumPatchFile )
                m_iNumPatchFile = diff_info.Size();
		}

        // Diff된 리스트 다운로드 루프
        std::map<std::string,KPatchFileInfo>::iterator mapIter = fileinfos.begin();
		while(mapIter != fileinfos.end() && IsStoped() == false)
		{
			std::pair<std::string,KPatchFileInfo> info(mapIter->first, mapIter->second);

			if(ismannual)   {   DownloadMannualFile(info,progress,error);   }
			else            {   DownloadFile(info,progress,progress2,error,addfile);  }
			mapIter++;


			// 태국 50메가 이상일때 FTP 주소 바꾼다.
			if ( KGCGlobalValue::m_bReDirectFTP && !ismannual )
			{
				if( IsFTPDownSize( KCurlSession::GetTotalWriteLength() ) )
				{
					Send_ErrorReport( "Redirect", "Kamuse", std::string("0"), 1 );
					START_LOG( cout, L"DownLoad File Size 50MB : " << KCurlSession::GetTotalWriteLength() );

					// pacthpath 주소 변경
					std::string strPatchPath = KGCGlobalValue::GetPatchPathFileName( true );
					if ( !GetPatchPath( strPatchPath ) ) 
					{
						Stop();
						return;
					}

					// 다시 50메가 넘을때를 방지하기 위해서...
					KGCGlobalValue::m_bReDirectFTP = false;
				}
			}
            // 에러발생시 스레드 종료
			if(IsError() && IsStoped() == false)
				{
				if( complete )
				{
					patchtime = (timeGetTime() - begintime);
					patched= true;
					complete(false,true);				
				}
				Stop();
				return;
			}
		}

        if( startgriddown &&
            0 < KGCGlobalValue::m_iPatchMode && 
            KGCGlobalValue::m_iGridDownState == KGCGlobalValue::GRID_OK ) 
	    {
            startgriddown();
        }
	}

	if(IsStoped() == false)
	{
		patchtime = (timeGetTime() - begintime);
		patched= true;

		if(ismannual)
		{
			DeleteDirForce(m_OriginalPatchpath);
		}

		if(complete)
			complete(true,true);	
	}
    else
    {
//		이때 이미 다이얼로그가 사라져서 쓰레드가 안드로메다로 가는것 같다.
//		if(complete)    
//			complete(false, true);
    }
}


void KPatchLib::DownloadMannualFile( std::pair<std::string,KPatchFileInfo> info,NETPROGRESS_CALLBACK progress,NETERROR_CALLBACK error )
{
	if(IsStoped())	
		return;	

	boost::shared_ptr<KCurlSession> session(new KCurlSession(id,pw,renameexe));	

	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.SetFile(info.first,info.second.GetSize());	
	}

	std::string remotefilename = patchpath;
	remotefilename.append(info.first);
	std::string filename = localpath + info.first;
	std::string headfilename = remotefilename+".head";
	Komfile remote_kom;
	Komfile head_kom;
	Komfile local_kom;
	
	
	if(CreateParentDirectory(GetDir(filename))==false)		
	{	
		OnError(info.first,KPatchLib::DIRCREATE);
		Sleep(MAX_DELAY);
		return;
	}


	if(head_kom.Open(headfilename) == true) // 헤드가 있으면 
	{
		if(remote_kom.Open(remotefilename) && 
			local_kom.Open(filename)) // 콤이고 양쪽에 있는 경우 
		{
			Komfile dest_kom;
			dest_kom.MannualJoin(head_kom,local_kom,remote_kom);

			SKomWriteInfo komwriteinfo(progress,NULL,progressinfo);
			if(dest_kom.Save(filename,komwriteinfo) == false)
			{	
				OnError(info.first,KPatchLib::WRITEKOM);
				return;	
			}
		}
		else
		{						
			OnError(info.first,KPatchLib::OPENKOM);			
			return;		
		}
	}
	else // 업으면 그냥 복사 
	{
		DeleteFileForce(filename);

		if(CopyFileA(remotefilename.c_str(),filename.c_str(),FALSE)==FALSE)
		{
			OnError(info.first,KPatchLib::FILEDELETE);
			return;
		}
	}

	if(IsStoped())	
		return;

	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.TotalRead(info.second.GetSize());
	}	

	KCSLOCK_BLOCK(diff_info)
	{
		diff_info.Erase(info.first);
	}	
}

void KPatchLib::DownloadFile( std::pair<std::string,KPatchFileInfo> info,
                              NETPROGRESS_CALLBACK progress,
                              NETPROGRESS_CALLBACK progress2,
                              NETERROR_CALLBACK error,
                              NETGRIDDOWNLOADADD_CALLBACK  addfile)
{
    // 스레드 동작 중 다이얼로그가 종료될 수도 있음
	if(IsStoped())	
		return;

    // 다운로드 받을 섹션생성
	boost::shared_ptr<KCurlSession> session(new KCurlSession(id,pw,renameexe));

    // 프로그래스바 정보에 다운받을 파일정보셋팅
	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.SetFile(info.first,info.second.GetSize());	
	}

    // 패치받을 서버와 패치받을 파일정보셋팅
	std::string url = patchpath;
	url.append(info.first);
	std::string filename = localpath + info.first;
	std::transform(url.begin(),url.end(),url.begin(),tourl);

    // 받을 파일이 kom파일인지 확인(존재하는 파일은 비교해서 받을려고)
	Komfile remote_kom;
	Komfile local_kom;
	bool iskom = false;
	if(filename.find_last_of(".kom") == filename.length() - 1)		
		iskom = local_kom.Open(filename);

	int errorcode = 0;

    // 받을 디렉터리가 존재하는지 확인
	if(CreateParentDirectory(GetDir(filename))==false)		
	{	
		OnError(info.first,KPatchLib::DIRCREATE);
		Sleep(MAX_DELAY);
		return;
	}

    // Local에 존재하는 파일이 아니면 무조건 다운로드
	if(iskom == false )
	{
        // 기존 파일 제거
		DeleteFileForce(filename);

        // Job에 파일추가 실패시 기존 다운로드로 받자
		if( !AddGridFile(url, localpath,  info, addfile) )
        {
            // 다운로드 완료 후 리턴됨.
            if(session->DownloadFile(url,filename,0,info.second.GetSize(),progress,progressinfo,errorcode)==false)
            {	
                OnError(info.first,errorcode);	
                DeleteFileForce(filename);
                if(IsFileExists(filename)==true)
                {
                    std::string newfilename = filename+".tmp";
                    MoveFileExA(filename.c_str(),newfilename.c_str(),MOVEFILE_REPLACE_EXISTING);
                }
                Sleep(MAX_DELAY);
                return;	
            }
		}
        else
            return;

        // 스레드 동작 중 다이얼로그가 종료될 수도 있음
        if(IsStoped())	
            return;	

        // 다운받은 파일 예외처리
		KPatchFileInfo info2(filename);
		if(info.second != info2)
		{	
			OnError(info.first,KPatchLib::VERIFY);			
			
			if(IsError() && info.first == "main.exe")
			{	
				if(error)
					error(info.first,KPatchLib::VERIFY);
			}
			DeleteFileForce(filename);
			Sleep(MAX_DELAY);
			return;	
		}
	}

    // Local에 존재하는 파일이면 비교해서 다운로드
	else 
	{
		bool saved = false;

        // 파일인포확인
		if(patchinfokom.get() != NULL &&
			patchinfokom->IsExist(info.first) == true) // 패치인포 콤에서 헤드를 얻어오기 시도한다
		{
			KSubfile subfile = patchinfokom->GetSubfile(info.first);

			std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
			
			if(subfile.WriteDecompressed(stream)==true)
			{
				if(remote_kom.Open(stream,session,url)==true)
				{
					Komfile dest_kom;
					int diffcount = dest_kom.LeftOuterJoin(remote_kom,local_kom);
					if(diffcount > (dest_kom.GetFileCount()* 0.7)) 
					{	
						DeleteFileForce(filename);		
						return;
					}
					SKomWriteInfo komwriteinfo(progress, progress2, progressinfo);
					if(dest_kom.Save(filename,komwriteinfo) == true)
					{							
						saved = true;
					}					
				}
			}			
		}

		if(saved==false)
		{
			if(remote_kom.Open(session,url)) // 콤이고 양쪽에 있는 경우 
			{
				Komfile dest_kom;
				int diffcount = dest_kom.LeftOuterJoin(remote_kom,local_kom);
				if(diffcount > (dest_kom.GetFileCount()* 0.7)) 
				{	
					DeleteFileForce(filename);		
					return;
				}
				SKomWriteInfo komwriteinfo(progress, progress2, progressinfo);
				if(dest_kom.Save(filename,komwriteinfo) == false)
				{	
					DeleteFileForce(filename);
					return;	
				}
			}
			else
			{						
				OnError(info.first,KPatchLib::OPENKOM);
				Sleep(MAX_DELAY);
				return;		
			}
		}
	}

    // 스레드 동작 중 다이얼로그가 종료될 수도 있음
	if(IsStoped())	
        return;	

    // 다운받은 파일 크기를 더해준다.
	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.TotalRead(info.second.GetSize());
	}

    // diff정보에서 다운받은 파일 제거한다.
	KCSLOCK_BLOCK(diff_info)
	{
		diff_info.Erase(info.first);
	}
}


bool KPatchLib::AddGridFile(std::string url, std::string localpath,  std::pair<std::string,KPatchFileInfo> info, NETGRIDDOWNLOADADD_CALLBACK  addfile)
{
    if( addfile &&
        0 < KGCGlobalValue::m_iPatchMode &&
        KGCGlobalValue::m_iGridDownState == KGCGlobalValue::GRID_OK )
    {
        // 파일 Size가 5MB이상일때만 그리드로 받을 수 있도록 JobList에 추가한다.
        // Byte단위 Size이므로 => 1024(byte) * 1024(kbyte) = 1MB
        if( info.second.GetSize() > (1024 * 1024 * 5) )
        {
            SGridDownFileInfo downinfo( url.c_str(), localpath.c_str(), info.first.c_str(), info.second.GetSize() );
            return addfile(downinfo);
        }
    }
    return false;
}


void KPatchLib::Diff()
{
	KCSLOCK_BLOCK(diff_info)
	{	
		diff_info.Diff(server_info,local_info);

		if(diff_info.Exist(patchername))
		{
			diff_info.Erase(patchername);
		}

		KCSLOCK_BLOCK(progressinfo)
		{
			progressinfo.totallength = diff_info.GetFileSize();
			progressinfo.totalreadlength = 0;
		}
	}
}



bool KPatchLib::CheckPatcherFiles()
{
	std::set<std::string> & filenames = server_info.GetPatcherFiles();
	std::set<std::string>::iterator i = filenames.begin();

	while(i!=filenames.end())
	{
		if(server_info.Exist(*i)==true)
        {
            KPatchFileInfo info = server_info.Find(*i);
            KPatchFileInfo info2(localpath+(*i));

            if(info != info2)            
			    patcherfiles.push_back(*i);
        }
		i++;
	}
	
    if(patcherfiles.empty() == true)
        return false;

    return true;
}

bool KPatchLib::DownloadPatcherFiles()
{
	std::vector<std::string>::iterator i = patcherfiles.begin();	
	while(i!=patcherfiles.end())
	{
		if(DownloadPatcherFile(*i)==false)
			return false;
		i++;
	}
	return true;
}



void KPatchLib::ClearPatcherFiles(  )
{
	std::set<std::string> &filenames = server_info.GetPatcherFiles();
	std::set<std::string>::iterator i = filenames.begin();	
	while(i!=filenames.end())
	{
		DeleteFileForce(localpath + *i + ".tmp");			
		i++;
	}
}



bool KPatchLib::DownloadPatcherFile(std::string filename)
{
	KCurlSession session(id,pw,renameexe);
	std::string localfilename =localpath+filename;
	std::string tempfilename = localfilename+".tmp";
	std::string tempfilename2 = tempfilename+".tmp";

	if( false== server_info.Exist(filename))
	{
		return false;
	}

	KPatchFileInfo info =server_info.Find(filename);

	std::string url = patchpath;

	url.append(filename);

	std::transform(url.begin(),url.end(),url.begin(),tourl);

	if(session.DownloadFile(url,tempfilename2)==false)
	{
		return false;
	}	
	
	if(GetFileAttributesA(localfilename.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
	}

	if(TRUE == MoveFileExA(localfilename.c_str(),tempfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
		else
		{
			MoveFileExA(tempfilename.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING);
			return false;
		}
	}
	return true;
}


bool KPatchLib::DownloadServerInfoFile( )
{
#if defined( _CHINA ) || defined(_CHINA_TEST)
	KCurlSession session(id,pw,renameexe);
	std::string localfilename =localpath+PATCH_SERVER_FILE_NAME;
	//std::string tempfilename = localfilename;

	//if( false== server_info.Exist("serverlist.txt"))
	//{
	//	return false;
	//}

	KPatchFileInfo info = server_info.Find(PATCH_SERVER_FILE_NAME);



	std::string url = PATCH_SERVER_INFO_SRC;
#ifdef _TEST
	std::string strURL_INI( KGCGlobalValue::GetDataFromINI( "ServerListURL" ) );    

	if ( strURL_INI != "NULL" ) {
		url = strURL_INI;
	}
	else {
		url = PATCH_TEST_SERVER_INFO_SRC;

		url.append(PATCH_SERVER_FILE_NAME);
	}
#else
	url.append(PATCH_SERVER_FILE_NAME);
#endif

	std::transform(url.begin(),url.end(),url.begin(),tourl);

	if(session.DownloadFile(url,localfilename)==false)
	{
		return false;
	}	
#endif
	return true;
}


bool KPatchLib::DownloadVersionInfoFile( )
{
	KCurlSession session(id,pw,renameexe);

	std::string localfilename = localpath + "versioninfo.txt";

	std::string url = patchpath + "versioninfo.txt";

	std::transform(url.begin(),url.end(),url.begin(),tourl);

	if(session.DownloadFile(url,localfilename)==false)
	{
		return false;
	}	

	return true;
}



bool KPatchLib::GetBooleanFromDat( std::string url ,bool &re)
{
	KCurlSession session(id,pw,renameexe);
	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);;	

	if(session.WriteRemoteData(url,stream) == false)
		return false;

	std::string str = stream.str();

	std::transform(str.begin(),str.end(),str.begin(),tolower);
	
	int start = str.find_first_of("<")+1;
	int end = str.find_last_of(">") - 1;

	if(start<0 || end<start)
		return false;

	std::string str2 = str.substr(start,end-start+1);

	if(str2=="true")
		re =true;
	else 
		re = false;

	
	return true;
}


bool KPatchLib::CreateMutex( std::wstring str )
{
	mutex = ::CreateMutex( NULL, FALSE, str.c_str() );
	if ( ( NULL == mutex ) || ( ERROR_ALREADY_EXISTS == ::GetLastError() ) )
	{
		return false;
	}
	return true;
}

void KPatchLib::ReleaseMutex()
{
	if ( NULL != mutex )
	{
		::ReleaseMutex( mutex );
		::CloseHandle( mutex );
	}
}

KPatchLib::~KPatchLib()
{		
}

void KPatchLib::OnError( std::string url,int errorcode )
{
	errorcount++;
	lasterror = url;
	lasterrorcode = errorcode;

    START_LOG( cerr, "lasterror : " << lasterror << " errorcode : " << errorcode );
}


void KPatchLib::Report(int successtype, std::string url,std::string service,std::string version )
{
	boost::thread t(boost::bind(&KPatchLib::SendReport,this, successtype,url,service,version));
}

void KPatchLib::SendReport(int successtype, std::string url,std::string service,std::string version )
{	
	try
	{	
		KCurlSession curl;

#if defined(USING_GCLOG_TEMPORARY)
		if (successtype == 0)
		{
			curl.ReportforGCLOG(url,service,version,true,lasterror,lasterrorcode,
				ismannual,patchserverip,patchtime,local_info.GetCheckSum(),server_info.GetCheckSum(),true, IsPatched());
		}		
		else if (successtype == 1)
		{
			lasterror = GetOSInfo();
			version = GetLocalPath();


			curl.ReportforGCLOG(url,service,version,false,lasterror,lasterrorcode,
				ismannual,patchserverip,patchtime,local_info.GetCheckSum(),server_info.GetCheckSum(),false, IsPatched());
		}
		else{
			curl.ReportforGCLOG(url,service,version,false,lasterror,lasterrorcode,
				ismannual,patchserverip,patchtime,local_info.GetCheckSum(),server_info.GetCheckSum(),false, IsPatched());
		}
#else
		char tmp[2048];
		
		_snprintf_s(tmp,2048,"%d,%s,%d,%d,%s,%08x,%08x,%d,%d,%d",
			successtype,//patchcompleted
			lasterror.c_str(),
			lasterrorcode,
			ismannual,// is mannual
			patchserverip.c_str(),
			local_info.GetCheckSum(),// oldver
			server_info.GetCheckSum(),// newver
			patchtime/1000,// time sec
			KGCGlobalValue::m_nLanguage,//langType
			KCurlSession::GetTotalWriteLength()/1024 // write kb	
			);
		curl.Report(url,service,version,tmp,strlen(tmp));		
#endif
	}	
	catch (...)
	{

	}
}

std::string KPatchLib::GetOSInfo()
{
	OSVERSIONINFOA osVersion;
	std::string strSystemOsData;

	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	if ( GetVersionExA( (OSVERSIONINFOA*)&osVersion ) )
	{
		switch( osVersion.dwPlatformId )
		{
		case VER_PLATFORM_WIN32s:
			{
				strSystemOsData = "Windows 3.1 ";
				break;
			}
		case VER_PLATFORM_WIN32_WINDOWS:
			{
				switch( osVersion.dwMinorVersion )
				{
				case 0:
					strSystemOsData = "windows 95 ";
					break;
				case 10:
					strSystemOsData = "windows 98 ";
					break;
				case 90:
					strSystemOsData = "Windows Me ";
				default:
					strSystemOsData = "Windows ?? ";
					break;
				}
				break;
			}
		case VER_PLATFORM_WIN32_NT:
			{
				switch( osVersion.dwMajorVersion )
				{
				case 3:
					strSystemOsData = "Windows  NT 3.51 ";
					break;
				case 4:
					strSystemOsData = "Windows NT 4.0 ";
					break;
				case 5:
					switch( osVersion.dwMinorVersion)
					{
					case 0:
						strSystemOsData = "Windows 2000 ";
						break;
					case 1:
						strSystemOsData = "Windows XP ";
						break;
					case 2:
						strSystemOsData = "Windows Server 2003 ";
						break;
					default:
						strSystemOsData = "Windows NT family ";
						break;
					}
					break;
				case 6:
					switch( osVersion.dwMinorVersion)
					{
					case 0:
						strSystemOsData = "Windows Vista ";
						break;
					case 1:
						strSystemOsData = "Windows 7 ";
						break;					
					default:
						strSystemOsData = "Windows Vista family ";
						break;
					}
					break;
				default:
					strSystemOsData = "Windows NT family ";
					break;
				}
				break;
			}
		default:
			{
				CHAR buffer[100];				
				strSystemOsData = "OS를 찾울수 없습니다.";
				sprintf( buffer, "%s (%d)", strSystemOsData.c_str() , (int)osVersion.dwPlatformId );
				strSystemOsData = buffer;
				break;
			}
		} // end switch

		CHAR buffer[100];
		sprintf(buffer,"%d",osVersion.dwBuildNumber);

		strSystemOsData += osVersion.szCSDVersion;
		strSystemOsData += "(Build ";
		strSystemOsData += buffer;
		strSystemOsData += ")";
	} // end if 
	else
		strSystemOsData = "OS정보를 읽어올수 없습니다.";

	return strSystemOsData;
}

bool KPatchLib::IsPatchComplete()
{	
	KCSLOCK_BLOCK(diff_info)
	{
		return diff_info.IsEmpty();
	}	
	return false;
}

void KPatchLib::Stop()
{
	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.isstoped = true;
		KGCGlobalValue::m_bIsStoped = true;
	}	
}

void KPatchLib::Start()
{
	KCSLOCK_BLOCK(progressinfo)
	{
		progressinfo.isstoped = false;
		KGCGlobalValue::m_bIsStoped = false;
	}	
}

bool KPatchLib::IsStoped()
{
	KCSLOCK_BLOCK(progressinfo)
	{
		return progressinfo.isstoped;
	}	
}
//	50메가 넘었는지 체크한다.
bool KPatchLib::IsFTPDownSize( DWORD dwFileSize_ )
{
	if ( dwFileSize_ == 0 )
		return false;

	dwFileSize_ /= 1048576;
	if ( dwFileSize_ > nFTPDownSize  )
	{
		return true;
	}

	return false;
}
bool KPatchLib::GetLangInfo()
{
	KCurlSession session(id,pw,renameexe);

	std::string url = patchpath+LANGINFOFILE;

	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);;
	if(session.WriteRemoteData(url,stream)==false)
		return false;

	int len = stream.tellp();

	if ( len <= 0 ) 
		return false;

	boost::scoped_array<char> data(new char[len]);
	stream.read(data.get(),len);
	return ParseLanginfo(data.get(),len);
}


bool KPatchLib::ParseLanginfo( char* data ,int len)
{
	XMLDocument doc;
	doc.ReadMemory(data,len);

	xmlNode* language = doc.GetRootElement();
	if(language->type == XML_ELEMENT_NODE && strcmp((char*)language->name,"Language")==0)
	{	
		xmlNode* nations = language->children;

		if(nations == NULL)			
			return false;		

		while(nations)
		{			
			if(nations->type == XML_ELEMENT_NODE && strcmp((char*)nations->name,"Nations")==0)
			{			
				xmlNode* nation = nations->children;
				while(nation)
				{
					if(nation->type == XML_ELEMENT_NODE && strcmp((char*)nation->name,"Nation")==0)
					{	
						std::string lang;
						xmlAttr * attribute =  nation->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 && attribute->children)
							{
								lang =(char*)attribute->children->content;
								break;							
							}
							attribute = attribute->next;
						}						
//						std::transform(lang.begin(),lang.end(),lang.begin(),tolower);
						vecLangInfo.push_back(lang);
					}
					nation = nation->next;
				}				
			}		

			nations = nations->next;
		}
	}
	else
	{	
		return false;
	}
	return true;
}

//	클라이언트내에 저장되어 있는 언어 정보
bool KPatchLib::LoadCurLang()
{
	std::string filename(localpath+LANGOPTFILE);

	std::ifstream filestream(filename.c_str(),std::ios_base::in );

	if(filestream.is_open()==false)
	{
		KGCGlobalValue::m_nLanguage = 0;
		return false;
	}

	std::string strData;

	filestream.seekg (0, std::ios::end);
	int len = (int)filestream.tellg();

	if ( len <= 0 ) 
		return false;

	filestream.seekg(0,std::ios::beg);
	boost::scoped_array<char> data(new char[len+1]);
	filestream.read(data.get(),len);
	filestream.close();
	data[len] = 0;

	strData = data.get();

	KGCGlobalValue::m_nLanguage = 0;
	for ( int i =0; i < vecLangInfo.size(); ++i ) 
	{
		if ( vecLangInfo[i] == strData ) 
			KGCGlobalValue::m_nLanguage = i;
	}

	return true;
}

//	클라이언트내에 저장되어 있는 언어 정보
bool KPatchLib::SaveCurLang()
{
	std::string filename(localpath+LANGOPTFILE);

	std::ofstream filestream(filename.c_str(),std::ios_base::out );

	if(!filestream.is_open())
	{
		return false;
	}

	std::string strData = vecLangInfo[KGCGlobalValue::m_nLanguage];

	filestream.write( strData.c_str(), strData.size() );
	filestream.close();

	return true;
}

//	언어별로 필요한 파일만 제외하고 모두 삭제한다.
bool KPatchLib::ConvertFileList()
{
	if ( vecLangInfo.empty() ) 
	{
		return false;
	}

	if ( KGCGlobalValue::m_nLanguage < 0 || KGCGlobalValue::m_nLanguage >= vecLangInfo.size() ) 
	{
		KGCGlobalValue::m_nLanguage = 0;
	}

	//	파일리스트에서 다른언어 파일들은 정보에서 삭제한다.
	for ( int i = 0; i < vecLangInfo.size(); ++i ) 
	{
		if ( i == KGCGlobalValue::m_nLanguage ) 
			continue;

		std::string findText = "_" + vecLangInfo[i] + ".kom";
		std::transform(findText.begin(),findText.end(),findText.begin(),tolower);

		std::map<std::string,KPatchFileInfo>& fileinfos = server_info.GetFileInfos();

		std::map<std::string,KPatchFileInfo>::iterator pos = fileinfos.begin();
		std::map<std::string,KPatchFileInfo>::iterator end = fileinfos.end();

		for ( ; pos != end; ) 
		{
			std::string key = pos->first;

			if ( std::string::npos != key.find( findText ) ) 
			{
				pos = fileinfos.erase( pos );
			}
			else
			{
				pos++;
			}
		}
	}

	return true;
}


std::string KPatchLib::GetLangName( int nLang )
{
	std::string strLang;

	if ( nLang < 0 || nLang >= vecLangInfo.size() ) 
	{
		return strLang;
	}

	return vecLangInfo[nLang];
}