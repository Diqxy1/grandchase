#include "StdAfx.h"
#include ".\generatemkom.h"
#include "io.h"
#include <algorithm>
#include <string>
#include <map>
#include "CRC_32.h"
#include "GCUtil/GCUtil.h"

GenerateMKom::GenerateMKom(void)
{
}

GenerateMKom::~GenerateMKom(void)
{
}


void GenerateMKom::SetMKomPath(std::string str)
{
    if (str=="")
    {
        m_bMakeMKOM=false;
        return;
    }

    if ( '\\' != str[str.size() - 1] )
    {
        str.append( "\\" );
    }
    m_strMKomPath=str;
    m_bMakeMKOM=true;
}


bool GenerateMKom::AddNewFile(std::string dir,std::string file, LogManager *logmanager)
{
	if (m_bMakeMKOM==false)
	{	
		return true;
	}

    std::string strOldFile=dir;
    std::string strMKomFile=m_strMKomPath;    

    strOldFile.append(file);
    strMKomFile.append(file);

	transform( strMKomFile.begin(), strMKomFile.end(), strMKomFile.begin(), tolower ); 
    if (!MakeDir(strMKomFile.c_str()))
    {
        std::cout << "파일이 들어갈 폴더의 생성에 실패 하였습니다.\n";
    }
    
    if (!CopyFile(strOldFile.c_str(),strMKomFile.c_str(),1))
    {
        std::cout << strMKomFile.c_str() << "파일을 복사 하지 못하였습니다.\n";
    }

	
    //추가된 파일이 Kom 파일인 경우 MKOM으로 만드러 준다.
    if(GCMassFile::IsKomFile(strMKomFile) && _access(strMKomFile.c_str(),0)!=-1)	//파일의 확장자가 kom인지 확인합니다.
    {
        std::string strMKomFileFolderPath;
        strMKomFileFolderPath=m_strMKomPath+file.substr(0,file.find_last_of('.'));
        transform( strMKomFileFolderPath.begin(), strMKomFileFolderPath.end(), strMKomFileFolderPath.begin(), tolower ); 
        //std::cout << strMKomFileFolderPath <<std::endl;
	
        if (!MakeDir(strMKomFileFolderPath+"\\"))
        {
            std::cout << "MKOM 파일이 들어갈 폴더의 생성에 실패 하였습니다.\n";
            return false;
        }
        
        //복사할 파일이므로 업데이트 여부 검사할 필요가 없음. 그냥 모든 파일 MKOM으로 만듬
        GCMassFile Massfile;
        Massfile.UpgradeMassFile(strMKomFile.c_str());
        Massfile.LoadMassFile(strMKomFile.c_str());
        

        UINT FileNo = Massfile.GetFileCount();
        GCMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
        memset(&FileInfo,0,sizeof(FileInfo));
        FileInfo.dwFlag = MFI_REALDATA;

        std::vector<std::string> vecFileList;
        vecFileList.clear();
        for(int i = 0; i < (int)FileNo; i++)
        {
            Massfile.GetMemberFile(i,&FileInfo);
            vecFileList.push_back(FileInfo.strFileName); 
            FileInfo.pRealData = NULL;
        }
        std::string strMKomFileName=strMKomFileFolderPath +"\\";

        Massfile.ModifyMiniMassFile(&Massfile,(char *)(strMKomFileFolderPath +"\\").c_str(),&vecFileList);
        Massfile.GenerateMassFileXml(strMKomFile,strMKomFileFolderPath +"\\crc.xml");
        Massfile.ReleaseMassFile();

		///////////////////////////////////////////////////////////////////////////////////////////////////
		DWORD dwPathFileSize = 0;
		std::string strPathFileCRC = "";
		int nCount = 0;

		
		for( int i = 0 ; i < (int)FileNo ; ++i )
		{
			Massfile.GetMemberFile( i, &FileInfo);
			std::string strKomPath = file.substr(0, file.find_last_of('.'));
			strKomPath.append("\\");

			std::string strMemberFileName =  strKomPath + FileInfo.strFileName;
			std::string strFileName = m_strMKomPath + strKomPath + FileInfo.strFileName;	

			if(std::string::npos == strMemberFileName.find("crc.xml",0))
			{
				strMemberFileName.append(".mkom");
				strFileName.append(".mkom");
			}

			DWORD dwMemberFileSize = 0;

			nCount = 0;
			
			std::string  strMemberFileCRC = "";
			
			do{
				strMemberFileCRC = CRC_32::GetCRCFromFile(strFileName.c_str(), dwMemberFileSize);

                if ( nCount )
                {
                    Sleep(100);
                }
				if(++nCount > 30)
				{
					::MessageBox(NULL, "GetCRCFromFile Failed!", "Error!!", MB_OK);           
					PostQuitMessage(-1);
                }                
			}while(TRUE == strMemberFileCRC.empty());

			int nVersion = 1;
			char szFileInfo[MAX_PATH] = {0,};
			GCUTIL_STR::ToLower(strMemberFileName);
			sprintf(szFileInfo, "[AddFile] %s [Version] %d [Size] %d [CRC] %s",
				strMemberFileName.c_str(), nVersion, dwMemberFileSize, strMemberFileCRC.c_str());	

			logmanager->PushLogData(LOG_ADDFILE,szFileInfo); 
		}
     }

     return true;
}

bool GenerateMKom::UpdateFile(std::string dirOld,std::string dirNew, std::string file, LogManager *logmanager)
{
    if (m_bMakeMKOM==false)
    {
        return true;
    }

    std::string strOldFile=dirOld;
    std::string strNewFile=dirNew;
    std::string strMKomFile=m_strMKomPath;
    strOldFile.append(file);
    strNewFile.append(file);
    strMKomFile.append(file);

    transform( strMKomFile.begin(), strMKomFile.end(), strMKomFile.begin(), tolower ); 
    if (!MakeDir(strMKomFile.c_str()))
    {
        std::cout << "파일이 들어갈 폴더의 생성에 실패 하였습니다.\n";
    }

    //std::cout << strNewFile << " Copy ->  " << strMKomFile << std::endl;


    if (!CopyFile(strNewFile.c_str(),strMKomFile.c_str(),1))
    {
        std::cout << strMKomFile.c_str() << "파일을 복사 하지 못하였습니다.\n";
    }

    //추가된 파일이 Kom 파일인 경우 MKOM으로 만드러 준다.
    if(GCMassFile::IsKomFile(strMKomFile) && _access(strMKomFile.c_str(),0)!=-1)	//파일의 확장자가 kom인지 확인합니다.
    {
//        char str[200];
//        char szCurWorkingDir[_MAX_PATH];
        std::string strMKomFileFolderPath;
        strMKomFileFolderPath=m_strMKomPath+file.substr(0,file.find_last_of('.'));
        transform( strMKomFileFolderPath.begin(), strMKomFileFolderPath.end(), strMKomFileFolderPath.begin(), tolower ); 

        //std::cout << strMKomFileFolderPath <<std::endl;

        if (!MakeDir(strMKomFileFolderPath+"\\"))
        {
            std::cout << "MKOM 파일이 들어갈 폴더의 생성에 실패 하였습니다.\n";
            return false;
        }

        //복사할 파일이므로 업데이트 여부 검사할 필요가 없음. 그냥 모든 파일 MKOM으로 만듬
        GCMassFile Massfile,MassfileOld;
        Massfile.UpgradeMassFile(strMKomFile.c_str());
        Massfile.LoadMassFile(strMKomFile.c_str());
        MassfileOld.LoadMassFile(strOldFile.c_str());
        
        UINT FileNo = Massfile.GetFileCount();
        UINT FileNoOld = MassfileOld.GetFileCount();
        //std::cout << strMKomFile << "("<< FileNo << ")"<< "//" << strOldFile << "("<< FileNoOld << ")"<< "\n";
        
        GCMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
        GCMassFile::MASSFILE_MEMBERFILEINFO FileInfoOld;
        
        memset(&FileInfo,0,sizeof(FileInfo));
        memset(&FileInfoOld,0,sizeof(FileInfoOld));
        FileInfo.dwFlag = MFI_REALDATA;
        FileInfoOld.dwFlag = MFI_REALDATA;

        std::map<std::string,std::string> mapFileList;
        mapFileList.clear();


        //업데이트 or 추가된 파일을 MKOM으로 만들기 위해 백터 생성
		std::vector<std::string> vecAddFile;
		std::vector<std::string> vecUpdateFile;
        std::vector<std::string> vecFileList;
        vecFileList.clear();

        //std::cout << FileNo << std::endl;
        // 현재 버젼의 파일 목록을 맵에 삽입
        for(int i = 0; i < (int)FileNo; i++)
        {
            Massfile.GetMemberFile(i,&FileInfo);
            if (strcmp(strlwr(FileInfo.strFileName),"crc.xml")==0) continue;
            //std::cout << "PrintCRC:" << FileInfo.strFileName << CRC_32::GetCRCFromBuffer(FileInfo.pRealData,FileInfo.ulSize) << FileInfo.ulSize << std::endl;
            std::pair<std::string,std::string> pData(strlwr(FileInfo.strFileName),CRC_32::GetCRCFromBuffer(FileInfo.pRealData,FileInfo.ulSize));
            mapFileList.insert(pData);
            FileInfo.pRealData = NULL;
        }

        // 그중 기존 버젼의 파일과 같은 것들은 삭제
        for(int i = 0; i < (int)FileNoOld; i++)
        {
            MassfileOld.GetMemberFile(i,&FileInfoOld);
            //std::cout << "PrintCRC:" << FileInfoOld.strFileName << CRC_32::GetCRCFromBuffer(FileInfoOld.pRealData,FileInfoOld.ulSize) << FileInfoOld.ulSize << std::endl;
            if (strcmp(strlwr(FileInfoOld.strFileName),"crc.xml")==0) continue;
            std::string strFileCRC=CRC_32::GetCRCFromBuffer(FileInfoOld.pRealData,FileInfoOld.ulSize);
            std::map<std::string, std::string>::iterator k = mapFileList.find(strlwr(FileInfoOld.strFileName));
            //std::cout << k->first << " CRC::" << strFileCRC << FileInfoOld.strFileName << " CRC:" << k->second << std::endl;
            if (k!=mapFileList.end())
            {	
                if (k->second==strFileCRC)
                {
                    //완전히 같은 경우
                    //char strOutput[200];
					//sprintf(strOutput,"Same MKOMFile :%s",k->first.c_str());
                    //logmanager->PushLogData(LOG_SAMEFILE_MKOM,strOutput);
                }
                else
                {	
                    //업데이트 경우
                    vecFileList.push_back(k->first);
					vecUpdateFile.push_back(k->first);
                    //char strOutput[200];
                    //sprintf(strOutput,"Update MKOMFile :%s",k->first.c_str());
                    //logmanager->PushLogData(LOG_UPDATEFILE_MKOM,strOutput);
                }
                mapFileList.erase(k);
            }
            else
            {
				int nDeleteVersion = 0;
				DWORD dwDeleteFileSize = 0;
				std::string strCRC = "";
				std::string strMKomFileName = file.substr(0,file.find_last_of('.'));
				
				strMKomFileName.append("\\");
				strMKomFileName.append(FileInfoOld.strFileName);
				strMKomFileName.append(".mkom");

				int nVersion = 1;
				char szFileInfo[MAX_PATH] = {0,};
				GCUTIL_STR::ToLower(strMKomFileName);
				sprintf(szFileInfo, "[DeleteFile] %s [Version] %d [Size] %d [CRC] %s",
					strMKomFileName.c_str(), nDeleteVersion, dwDeleteFileSize, strCRC.c_str());	

				logmanager->PushLogData(LOG_DELETEFILE,szFileInfo); 
				//지워진 경우
                //char strOutput[200];
                //sprintf(strOutput,"Delete MKOMFile :%s",FileInfoOld.strFileName);
                //logmanager->PushLogData(LOG_DELETEFILE_MKOM,strOutput);
            }
            FileInfoOld.pRealData = NULL;
        }
        MassfileOld.ReleaseMassFile();

        std::string strMKomFileName=strMKomFileFolderPath +"\\";
        for (std::map<std::string, std::string>::iterator k = mapFileList.begin(); k != mapFileList.end(); std::advance(k, 1))
        {
            //Add된 경우
            vecFileList.push_back(k->first);
			vecAddFile.push_back(k->first);

            //char strOutput[200];
            //sprintf(strOutput,"Add MKOMFile :%s",k->first.c_str());
            //logmanager->PushLogData(LOG_ADDFILE_MKOM,strOutput);
        }

        Massfile.ModifyMiniMassFile(&Massfile,(char *)(strMKomFileFolderPath +"\\").c_str(),&vecFileList);
        Massfile.GenerateMassFileXml(strMKomFile,strMKomFileFolderPath +"\\crc.xml");
        Massfile.ReleaseMassFile();

		for(std::vector<std::string>::iterator k = vecAddFile.begin(); k != vecAddFile.end(); std::advance(k, 1))
		{	
			
			int nVersion = 1;
			
			std::string strMKomFileName = file.substr(0,file.find_last_of('.'));
			strMKomFileName.append("\\");
			strMKomFileName.append((*k));
			strMKomFileName.append(".mkom");

			std::string strRealFileName = m_strMKomPath +strMKomFileName;

			DWORD dwTempFileSize = 0;
			
			int nCount = 0;
			std::string strMKomFileCRC = "";
			
			do{
				strMKomFileCRC = CRC_32::GetCRCFromFile(strRealFileName.c_str(), dwTempFileSize);
                if ( nCount )
                {
                    Sleep(100);
                }

				if(++nCount > 30)
				{
					::MessageBox(NULL, "GetCRCFromFile Failed!", "Error!!", MB_OK);           
					PostQuitMessage(-1);
				}
			}while(TRUE == strMKomFileCRC.empty());

			char szFileInfo[MAX_PATH] = {0,};
			GCUTIL_STR::ToLower(strMKomFileName);
			sprintf(szFileInfo, "[AddFile] %s [Version] %d [Size] %d [CRC] %s",
				strMKomFileName.c_str(), nVersion, dwTempFileSize, strMKomFileCRC.c_str());	

			logmanager->PushLogData(LOG_ADDFILE,szFileInfo); 
		}

		for(std::vector<std::string>::iterator k = vecUpdateFile.begin(); k != vecUpdateFile.end(); std::advance(k, 1))
		{

			int nVersion = 2;

			std::string strMKomFileName = file.substr(0,file.find_last_of('.'));
			strMKomFileName.append("\\");
			strMKomFileName.append((*k));
			strMKomFileName.append(".mkom");

			std::string strRealFileName = m_strMKomPath +strMKomFileName;

			DWORD dwTempFileSize = 0;
			int nCount = 0;
			std::string strMKomFileCRC = "";
	
			do{
				strMKomFileCRC = CRC_32::GetCRCFromFile(strRealFileName.c_str(), dwTempFileSize);
                if ( nCount )
                {
                    Sleep(100);
                }
				if(++nCount > 30)
				{
					::MessageBox(NULL, "GetCRCFromFile Failed!", "Error!!", MB_OK);           
					PostQuitMessage(-1);
				}				
			}while(TRUE == strMKomFileCRC.empty());

			char szFileInfo[MAX_PATH] = {0,};
			GCUTIL_STR::ToLower(strMKomFileName);
			sprintf(szFileInfo, "[UpdateFile] %s [Version] %d [Size] %d [CRC] %s",
				strMKomFileName.c_str(), nVersion, dwTempFileSize, strMKomFileCRC.c_str());	

			logmanager->PushLogData(LOG_UPDATEFILE,szFileInfo); 
		}

    }
    return true;
}

bool GenerateMKom::MakeDir(std::string dir) {
    //std::cout << dir << std::endl;
    dir=dir.substr(0,dir.find_last_of('\\')).c_str();    
    if (dir.length()==0) return false;

    do 
    {

        if (_access(dir.c_str(),0)!=-1) 
        {
            return true;
        }
        else if (CreateDirectory(dir.c_str(), NULL))
        {
            //std::cout <<  dir.c_str() << " 디렉토리 생성\n";
            return true;
        }

    } while(MakeDir(dir));
    // 상위 디렉토리가 존재하거나, 디렉토리를 생성 하였다면, 다시 반복한다.

	return false;
}
