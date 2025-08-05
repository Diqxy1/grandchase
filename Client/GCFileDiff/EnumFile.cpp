#include "stdafx.h"
#include "EnumFile.h"
#include "CRC_32.h"
#include <sstream>
#include <io.h>
#include <iostream>
#include <fstream>
#include "GCUtil/GCUtil.h"
#include "MassFileLib/GCMassFile.h"
#include <set>


using namespace std;
#define COMP_BUFFER_SIZE	(512*1024)

char compbuffer0[COMP_BUFFER_SIZE];
char compbuffer1[COMP_BUFFER_SIZE];

void DeleteFileAbsolutely( const char* szFileName ) ;

struct GC_Xml_Data{
    std::string strFileName;
    std::string strCRC;
    int   iFileSize;
    int   iVersion;
    ~GC_Xml_Data()
    {        
    }
};


void replace(std::string& text,  const std::string& find_token,  const std::string& replace_token)
{
    size_t i = 0;
    while ((i = text.find(find_token)) != std::string::npos)
        text.replace(i, find_token.size(), replace_token);
}


void GetFileNameExceptFolder( char* szDest, const char* szSrc )
{
	int iDestFeed, iSrcFeed;
    for ( iDestFeed = 0, iSrcFeed = 0;
        NULL != szSrc[iSrcFeed];
        iDestFeed++, iSrcFeed++ )
    {
        szDest[iDestFeed] = szSrc[iSrcFeed];
        if ( '\\' == szSrc[iSrcFeed] ) iDestFeed = -1;
    }
    szDest[iDestFeed] = '\0';
}


KEnumFile::KEnumFile()
{
    memset( m_szFileName, 0, _MAX_FNAME );
    //strcpy( m_szCurPath, szCurDir ); // for full path 
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth          = 0;

    // xml
    m_pxmlSignElement = 0;
    m_pxmlDirElement  = 0;
    m_pxmlFileElement = 0;

    m_pxmlDoc         = NULL;

    m_pDirMap         = NULL;
    m_pFileMap        = NULL;       
} // KEumFile()


KEnumFile::~KEnumFile()
{
    memset(m_szFileName, 0, _MAX_FNAME );
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth    = 0;

    DirMap::iterator    dit;
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if

    FileMap::iterator   fit;
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if

    SAFE_DELETE( m_pxmlDoc );
} // ~KEnumFile()

void KEnumFile::SetMKomPath(std::string str)
{
    m_KomProc.SetMKomPath(str);
}

bool KEnumFile::Signing(TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    node->ToElement()->SetAttribute("Name", strSign);

    return true;
} // Signing()


bool KEnumFile::Verify(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strOrgSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    std::string strCurSign = node->ToElement()->Attribute("Name");

    return (strOrgSign == strCurSign);
} // Verify()


TiXmlElement* KEnumFile::GetSignElement(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlNode* node = NULL;
    node = xmlDoc_.FirstChild("PatchInfo");
    node = node->FirstChild("Signature");
    if (NULL == node)
    {
        TiXmlElement item("Signature");
        xmlDoc_.ToElement()->InsertEndChild(item);
        node = xmlDoc_.FirstChild("PatchInfo")->FirstChild("Signature");
        ASSERT(node);
    }
    return node->ToElement();
} // GetSignElement()


const std::string KEnumFile::GetFileListStr(const TiXmlDocument& xmlDoc_) // by parkch
{
    std::ostringstream xmlStream;
    xmlStream << xmlDoc_;
    std::string strStream = xmlStream.str();
    std::string::size_type beginPos = strStream.find("<File>");
    std::string::size_type endPos   = strStream.find("</File>");
    if (std::string::npos == beginPos || std::string::npos == endPos)
    {
        strStream.erase();
        return strStream;
    }
    return strStream.substr(beginPos, endPos);
} // GetFileListStr()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               const char* szFileName_)
{
    if( NULL != szFileName_)
    {
        strcpy( m_szFileName, szFileName_ );
        m_pxmlDoc         = new TiXmlDocument( szFileName_ );
        m_pDirMap         = new DirMap;
        m_pFileMap        = new FileMap;
    }
    else
    {
        return false;
    }

    //    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
    //                          "<PatchInfo>"
    //                          "<Directory>\n"
    //                          "</Directory>"
    //                          "<File>\n"
    //                          "</File>"
    //                          "</PatchInfo>";
    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
        "<PatchInfo>"
        "<Signature>"
        "</Signature>"
        "<Directory>\n"
        "</Directory>"
        "<File>\n"
        "</File>"
        "</PatchInfo>";
    m_pxmlDoc->Parse( szStart );

    if( m_pxmlDoc->Error() )
    {
        printf( "Error in %s: %s\n", m_pxmlDoc->Value(), m_pxmlDoc->ErrorDesc() );
        return false;
    } // if

    TiXmlNode* node = 0;
    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();

    // enumerate file
    SetCurrentDirectory( szCurDir_ );
    EnumFile();

    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );

    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for

    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
        //{{ Modified by ct 2003-04-29 오후 04:54:04
        //        char buf[40];
        //}} Modified by ct 2003-04-29 오후 04:54:04
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );
        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum );
       
        //{{ Modified by ct 2003-04-29 오후 04:53:34
        /*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
        */
        //}} Modified by ct 2003-04-29 오후 04:53:34

        m_pxmlFileElement->InsertEndChild( item );
    } // for

    m_pxmlDoc->SaveFile();

    // 파괴
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if

    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if

    SAFE_DELETE( m_pxmlDoc );

    return true;
}


void KEnumFile::EnumFile()
{
    HANDLE          hFile;
    WIN32_FIND_DATA	wfd;

    hFile = FindFirstFile( "*.*", &wfd );

    while( FindNextFile(hFile, &wfd) )
    {
        // parent directory인 경우 skip
        if( strcmp(wfd.cFileName, "..") == 0 ) 
            continue;

        // directory인 경우
        if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            SetCurrentDirectory( wfd.cFileName );
            m_nDepth++;

            strcat( m_szCurPath, wfd.cFileName );
            strcat( m_szCurPath, "\\" );

            // Add item to Directory element
            KDirInfo*   pkDir = new KDirInfo;
            strcpy( pkDir->m_szName, m_szCurPath );
            (*m_pDirMap)[pkDir->m_szName] = pkDir;

            EnumFile();
        }
        else
        {
            int nTempLen;
            char szTempPath[_MAX_PATH];

            nTempLen = sprintf( szTempPath, "%s%s", m_szCurPath, wfd.cFileName );
            szTempPath[nTempLen++] = 0;

            // Add item to File element
            KFileInfo*  pkFile  = new KFileInfo;
            strcpy( pkFile->m_szName, szTempPath );
            pkFile->m_dwSize    = wfd.nFileSizeLow;
            pkFile->m_usVersion = 1;
            ::CharUpper( wfd.cFileName );
            std::string strFileName( wfd.cFileName );

            // .des 와 GrandChase.ini 는 체크섬 계산되지 않도록 수정 
            // by DD 2008/06/09 
            if ( ( strFileName.find( ".EXE" ) != std::string::npos ) || 
                ( strFileName.find( ".DLL" ) != std::string::npos ) ||
                ( strFileName.find( ".FRM" ) != std::string::npos ) ||
                ( strFileName.find( ".STG" ) != std::string::npos ) ||
                ( strFileName.find( ".LUA" ) != std::string::npos ) ||
                ( strFileName.find( ".KOM" ) != std::string::npos ) ||
                ( strFileName.find( ".KSF" ) != std::string::npos ) ||
                ( strFileName.find( ".DES" ) != std::string::npos ) || 
                ( strFileName.find( ".DDS" ) != std::string::npos ) || 
                ( strFileName.find( ".INI" ) != std::string::npos ) )
            {
				int nErrorCount = 0;
				DWORD dwTempSize = 0;
				do{
					if(strFileName == "GRANDCHASE.INI" )
					{
						break;
					}
					pkFile->m_strCheckSum = CRC_32::GetCRCFromFile( wfd.cFileName, dwTempSize );
                    if ( nErrorCount )
                    {
                        Sleep(100);
                    }

                    pkFile->m_strCheckSum = CRC_32::GetCRCFromFile( wfd.cFileName, dwTempSize );

					if (nErrorCount++ > 30)
					{
						::MessageBox(NULL, "GetCRCFromFile Failed!", "Error!!", MB_OK);                   
						PostQuitMessage(-1);
					}					
				}while(true == pkFile->m_strCheckSum.empty());
            }
            else
            {
                pkFile->m_strCheckSum.erase();
            }
            //{{ Modified by ct 2003-04-29 오후 04:54:24
            //            pkFile->m_ftLastWriteTime = wfd.ftLastWriteTime;
            //}} Modified by ct 2003-04-29 오후 04:54:24
            (*m_pFileMap)[pkFile->m_szName] = pkFile;

        } // else
    } // while

    if( m_nDepth ) 
    {
        if( m_nDepth == 1 ) 
            m_szCurPath[0] = 0;
        else 
        {
            *strrchr( m_szCurPath, '\\' ) = '\0';
            *(strrchr( m_szCurPath, '\\' ) + 1) = '\0';
        } // else

        SetCurrentDirectory( ".." );
        m_nDepth--;
    } // if

    if( hFile != INVALID_HANDLE_VALUE )
        FindClose( hFile );
} // EnumFile()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               TiXmlDocument& xmldoc_)
{
    m_pDirMap         = new DirMap;
    m_pFileMap        = new FileMap;

    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >"
        "<PatchInfo>"
        "<Signature>"
        "</Signature>"
        "<Directory>"
        "</Directory>"
        "<File>"
        "</File>"
        "</PatchInfo>";

    xmldoc_.Parse( szStart );
    if( xmldoc_.Error() )
    {
        printf( "Error in %s: %s\n", xmldoc_.Value(), xmldoc_.ErrorDesc() );
        return false;
    } // if

    //xmldoc_.Print( stdout );

    TiXmlNode* node = 0;
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();

    // enumerate file
    GetCurrentDirectory( _MAX_PATH, m_szCurWorkingDir );
    SetCurrentDirectory( szCurDir_ );
    EnumFile();
    SetCurrentDirectory( m_szCurWorkingDir );

    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );

    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for

    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
        //{{ Modified by ct 2003-04-29 오후 04:56:05
        /*
        char buf[40];
        */
        //}} Modified by ct 2003-04-29 오후 04:56:05
        char *temp;

        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );

        temp = fit->second->m_szName;

        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum );

        //{{ Modified by ct 2003-04-29 오후 04:55:35

        /*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
        */
        //}} Modified by ct 2003-04-29 오후 04:55:35

        m_pxmlFileElement->InsertEndChild( item );
    } // for

    //xmldoc_.SaveFile();
    //xmldoc_.Print( stdout );

    // 파괴
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if

    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if

    return true;
}

bool KEnumFile::CompareFileVersion(const TiXmlDocument& xmlOldDoc_, 
                                   const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
        "<PatchInfo>"
        "<Signature>"
        "</Signature>"
        "<Directory>\n"
        "</Directory>"
        "<File>\n"
        "</File>"
        "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);

    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );

    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );

    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        //dirElementNew->InsertEndChild(*(node->Clone()));
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );

    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;

    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    while( node != 0 || nodeOld != 0 )
    {
        if( nodeOld != 0 && node == 0 )
        {
            // Old 파일 삭제될 경우
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } // if

        if( nodeOld == 0 && node != 0 )
        {
            // new list에 추가 되어야 할 경우            
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
            node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            nodeOld->ToElement()->Attribute("Size", &nSizeOld);
            nodeOld->ToElement()->Attribute("Version", &nVersionOld);

            //{{ Modified by ct 2003-04-29 오후 04:57:32
            /*
            FILETIME ft;
            FILETIME ftOld;
            ft.dwHighDateTime   = atol( node->ToElement()->Attribute("LastHigh") );
            ft.dwLowDateTime    = atol( node->ToElement()->Attribute("LastLow") );
            ftOld.dwHighDateTime= atol( nodeOld->ToElement()->Attribute("LastHigh") );
            ftOld.dwLowDateTime = atol( nodeOld->ToElement()->Attribute("LastLow") );
            */
            //}} Modified by ct 2003-04-29 오후 04:57:32


            //{{ Modified by ct 2003-04-29 오후 04:57:57
            /*
            if( (nSize != nSizeOld) ||
            (nVersion != nVersionOld) ||
            (CompareFileTime(&ft, &ftOld) != 0) ) // 시간이 다른 경우
            {
            */
            //}} Modified by ct 2003-04-29 오후 04:57:57
            if( (nSize != nSizeOld) && 
                (nVersion != nVersionOld) ) // 시간이 다른 경우
            {
                // file의 version이 변경된 경우
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")) + 1);
            }
            else
            {
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")));
            }
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // Old이 삭제되는 경우
                nodeOld = nodeOld->NextSibling("Item");
            }
            else
            {
                //fileElementNew->InsertEndChild(*(node->Clone()));
                fileElementNew->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    return true;
} // CompareFileVersion()


bool KEnumFile::CreateUpdateFileList(const char* szLocalFileName_, 
                                     const char* szServerFileName_, 
                                     TiXmlDocument& xmlUpdateDoc_)
{
    char szPath[_MAX_PATH];
    char szLocalFileFullPath[_MAX_PATH];
    GetCurrentDirectory( _MAX_PATH, szPath );
    strcat( szPath, "\\" );

    // local filelist
    strcpy( szLocalFileFullPath, szPath );
    strcat( szLocalFileFullPath, szLocalFileName_ );

    TiXmlDocument docLocal_;

    //TiXmlDocument docLocal(szLocalFileFullPath);
    TiXmlDocument docLocal(szLocalFileFullPath);
    if( !docLocal_.LoadFile(szLocalFileFullPath) )
    {
        //        const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
        //                                "<PatchInfo>"
        //                                "<Signature>"
        //                                "</Signature>"
        //                                "<Directory>\n"
        //                                "</Directory>"
        //                                "<File>\n"
        //                                "</File>"
        //                                "</PatchInfo>";
        //        docLocal.Parse(szInitDoc);
        CreateFileList(szPath, docLocal);
        KEnumFile::Signing(docLocal);
        docLocal.SaveFile();
    }
    else
    {
        docLocal.LoadFile();
        if (false == Verify(docLocal))
        {
            docLocal.Clear();
            CreateFileList(szPath, docLocal);
            KEnumFile::Signing(docLocal);
            docLocal.SaveFile();
        }
    }

    // server filelist
    TiXmlDocument docServer( szServerFileName_ );
    if( !docServer.LoadFile() )
    {
        printf( "Could not load file. Error='%s'. Exiting.\n", docServer.ErrorDesc() );
        return false;
    }
    //docServer.Print( stdout );

    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
        "<PatchInfo>"
        //{{ Appended by parkch: 2003-11-18 오전 9:23:19
        "<PatchVersion>"
        "</PatchVersion>"
        //}} Appended by parkch: 2003-11-18 오전 9:23:19
        "<Signature>"
        "</Signature>"
        "<Directory>\n"
        "</Directory>"
        "<File>\n"
        "</File>"
        "</PatchInfo>";

    //TiXmlDocument docDownload( "downloadfilelist.xml" );
    xmlUpdateDoc_.Parse(szInitDoc);

    TiXmlNode*      node = 0;
    TiXmlNode*      nodeLocal = 0;
    TiXmlNode*      nodeDownload = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementLocal = 0;
    TiXmlElement*   dirElementDownload = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementLocal = 0;
    TiXmlElement*   fileElementDownload = 0;

    node = docServer.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );

    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    dirElementLocal = nodeLocal->FirstChild("Directory")->ToElement();
    assert( dirElementLocal );

    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    dirElementDownload = node->FirstChild("Directory")->ToElement();
    assert( dirElementDownload );

    node = dirElement->FirstChild("Item");
    nodeLocal = dirElementLocal->FirstChild("Item");


    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local 파일 삭제될 경우
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if

        if( nodeLocal == 0 && node != 0 )
        {
            // server의 list에 추가 되어야 할 경우            
            //dirElementDownload->InsertEndChild(*(node->Clone()));
            dirElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
            node->ToElement()->Attribute("Name")) == 0) )
        {
            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local이 삭제되는 경우
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                //dirElementDownload->InsertEndChild(*(node->Clone()));
                dirElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while


    node = docServer.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );

    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    fileElementLocal = nodeLocal->FirstChild("File")->ToElement();
    assert( fileElementLocal );

    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    fileElementDownload = node->FirstChild("File")->ToElement();
    assert( fileElementDownload );

    int nSize = 0,
        nSizeLocal = 0,
        nVersion = 0,
        nVersionLocal = 0;
    std::string strCheckSum;
    std::string strCheckSumLocal;
    std::string strCheckSumActual;
    char szActualFilePath[_MAX_PATH];

    node = fileElement->FirstChild("Item");
    nodeLocal = fileElementLocal->FirstChild("Item");
    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local 파일 삭제될 경우
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if

        if( nodeLocal == 0 && node != 0 )
        {
            // server의 list에 추가 되어야 할 경우            
            // fileElementDownload->InsertEndChild(*(node->Clone()));
            fileElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
            node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            strCheckSum = node->ToElement()->Attribute("CheckSum");
            nodeLocal->ToElement()->Attribute("Size", &nSizeLocal);
            nodeLocal->ToElement()->Attribute("Version", &nVersionLocal);
            strCheckSumLocal = nodeLocal->ToElement()->Attribute("CheckSum");

            //{{ Appended by parkch: 2003-07-09 오후 5:24:01
            // 실제 파일의 경로를 구한다.
            ZeroMemory( szActualFilePath, _MAX_PATH );
            strcpy( szActualFilePath, szPath );
            strcat( szActualFilePath, node->ToElement()->Attribute("Name") );

            // 업데이트 리스트에 넣을 파일들을 선택한다.
            if( -1 == _access( szActualFilePath, 0 ) ) // 0. 파일 존재 여부 체크
            {
                // 파일이 존재하지 않으면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nVersion != nVersionLocal ) // 1. 버전 체크
            {
                // 버전이 다르면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nSize != nSizeLocal ) // 2. 파일 크기 체크
            {
                // 파일 크기가 다르면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( !strCheckSum.empty() ) // 체크섬 체크가 필요한 파일의 경우
            {
				DWORD dwFileSizeTemp = 0;
				int nCRCErrorCnt = 0;
                // 중요 파일의 경우, 해킹 방지를 위해 체크섬으로 유효한지 판단한다.
                while("" == (strCheckSumActual = CRC_32::GetCRCFromFile(szActualFilePath, dwFileSizeTemp)))
				{
                    if ( nCRCErrorCnt )
                    {
                        Sleep(100);
                    }
					if(nCRCErrorCnt++ > 30)
					{
						MessageBox(NULL, "GetCRCFromFile Failed!!", "Error", MB_OK);
						PostQuitMessage(-1);
					}
				}

                if ( strCheckSum != strCheckSumLocal ) // 3. 새 리스트의 체크섬 체크
                {
                    // 새 리스트의 체크섬이 다르면 업데이트 리스트에 추가.
                    fileElementDownload->InsertEndChild(*node);
                }
                else if( strCheckSum != strCheckSumActual ) // 4. 실제 파일의 체크섬 체크
                {
                    // 실제 파일의 체크섬이 다르면 업데이트 리스트에 추가.
                    fileElementDownload->InsertEndChild(*node);
                }
            }
            //}} Appended by parkch: 2003-07-09 오후 5:24:01

            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local이 삭제되는 경우
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                // fileElementDownload->InsertEndChild(*(node->Clone()));
                fileElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    KEnumFile::Signing(docServer);
    docServer.SaveFile();

    return true;
} //CreateUpdateFileList()



int KEnumFile::IsDiffFiles(const TCHAR* szSrcName, const TCHAR* szDstName)
{
    FILE* fpSrc = NULL;
    FILE* fpDst = NULL;
    fpSrc = ::_tfopen(szSrcName, "rb");
    fpDst = ::_tfopen(szDstName, "rb");

    // 1. error case check
    // std::cout << "step 1: check error case" << endl;
    if (fpSrc == NULL || fpDst == NULL)
    {
        ::fprintf(stderr, "Warnning: %s or %s not exist\n", szSrcName, szDstName);
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return 1;
    }

    
    // 2. size compare
    //cout << "step 2: size compare" << endl;
    fpos_t posSrc = NULL;
    fpos_t posDst = NULL;
    ::fseek(fpSrc, 0, SEEK_END);
    ::fgetpos(fpSrc, &posSrc);
    ::fseek(fpSrc, 0, SEEK_SET);
    ::fseek(fpDst, 0, SEEK_END);
    ::fgetpos(fpDst, &posDst);
    ::fseek(fpDst, 0, SEEK_SET);
    if (posSrc != posDst)
    {
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return 2;
    }

    // 3. read the file contents
    // cout << "step 3: read the contents" << endl;
    size_t ReadByte0 = 0;
    size_t ReadByte1 = 0;

    do 
    {
        ReadByte0 = fread(compbuffer0,1,COMP_BUFFER_SIZE,fpSrc);
        ReadByte1 = fread(compbuffer1,1,COMP_BUFFER_SIZE,fpDst);
        if( ReadByte0 != ReadByte1 )
        {
            fprintf(stderr, "ERROR: Something prob(0)\n");
            return 3;
        }
        if( memcmp(compbuffer0, compbuffer1, ReadByte0 ) )
        {
            return 4;
        }
    }	 
    while( ReadByte0 != 0 && ReadByte1 != 0 );





    /*

    char* bufSrc = NULL;
    char* bufDst = NULL;
    bufSrc = new char[(size_t)posSrc];
    bufDst = new char[(size_t)posDst];
    if (bufSrc == NULL || bufDst == NULL)
    {
    fprintf(stderr, "ERROR: cannot allocate memory\n");
    if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
    if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
    if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
    if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
    return true;
    }
    ::fread(bufSrc, sizeof(char), (size_t)posSrc, fpSrc);
    ::fread(bufDst, sizeof(char), (size_t)posDst, fpDst);
    if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
    if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }

    // 4. byte-by-byte compare
    // cout << "step 4: byte compare" << endl;
    char* ptrSrc = bufSrc;
    char* ptrDst = bufDst;
    while (ptrSrc < bufSrc + posSrc)
    {
    if (*ptrSrc++ != *ptrDst++)
    {
    if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
    if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
    return true;
    }
    }
    if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
    if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
    */
    if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
    if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
    return -1;
} // IsDiffFiles()


bool KEnumFile::CompareFileVersion(const char* szOldPath_, const TiXmlDocument& xmlOldDoc_, 
                                   const char* szNewPath_, const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
        "<PatchInfo>"
        "<Signature>"
        "</Signature>"
        "<Directory>\n"
        "</Directory>"
        "<File>\n"
        "</File>"
        "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);

    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );

    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );

    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );

    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );

    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;


    // 2007 7.13 래진 - 파일 변동 사항 로그로 출력
    fstream kfLogfile;
    int nAddFileCount = 0;			//추가될 파일 수
    int nDeleteFileCount = 0;		//지워질 파일 수
    int nUpdateFileCount = 0;		//변경된 파일 수
    int nNotUpdateFileCount = 0;	//변경되지 않은 파일 수
    int nOldFileCount = 0;			//기존 버젼의 파일 수
    int nNewFileCount = 0;			//새 버젼의 파일 수

    std::string strOldPath;
    std::string strNewPath;
    strOldPath = szOldPath_;
    strNewPath = szNewPath_;

    kfLogfile.open( "log.txt", std::ios::out );
    logmanager.ResetLogData();


    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    SYSTEMTIME st;
    GetSystemTime(&st);
    int iTotalPatchSize = 0;
    char strOutput[200];

    std::map<std::string,GC_Xml_Data> mapXmlData;
    mapXmlData.clear();
    while(nodeOld!=0)  
    {
        GC_Xml_Data gcXmlData;
        gcXmlData.strFileName=(std::string)nodeOld->ToElement()->Attribute("Name");
        GCUTIL_STR::ToLower(gcXmlData.strFileName);
        gcXmlData.iVersion=atoi(nodeOld->ToElement()->Attribute("Version"));
        gcXmlData.strCRC=nodeOld->ToElement()->Attribute("CheckSum");
        gcXmlData.iFileSize=atoi(nodeOld->ToElement()->Attribute("Size"));

        std::pair<std::string,GC_Xml_Data> pData(gcXmlData.strFileName,gcXmlData);
        mapXmlData.insert(pData);
        nodeOld=nodeOld->NextSibling("Item");
    }

    while(node!=0)  
    {
        GC_Xml_Data gcXmlData;
        gcXmlData.strFileName=(std::string)node->ToElement()->Attribute("Name");
        GCUTIL_STR::ToLower(gcXmlData.strFileName);
        gcXmlData.iVersion=atoi(node->ToElement()->Attribute("Version"));
        gcXmlData.strCRC=node->ToElement()->Attribute("CheckSum");
        gcXmlData.iFileSize=atoi(node->ToElement()->Attribute("Size"));
        int iVer;

        std::map<std::string, GC_Xml_Data>::iterator k = mapXmlData.find(gcXmlData.strFileName);

        if (k == mapXmlData.end())
		{
            //최근 버젼에 있으나 예전 버젼에 없는 경우.
            sprintf(strOutput,"[AddFile] %s [Version] %d [Size] %d [CRC] %s",gcXmlData.strFileName.c_str(),gcXmlData.iVersion, gcXmlData.iFileSize, gcXmlData.strCRC.c_str());
            //std::cout << strOutput << std::endl;
            if (m_KomProc.AddNewFile(strNewPath,node->ToElement()->Attribute("Name"),&logmanager)==false)
                return false;;
            logmanager.PushLogData(LOG_ADDFILE,strOutput);
            nDeleteFileCount++;
            iVer=1;
        }
        else
        {
            std::string strOldFileName=strOldPath;
            std::string strNewFileName=strNewPath;

            strOldFileName.append(gcXmlData.strFileName);
            strNewFileName.append(k->second.strFileName);

            //std::cout << strOldFileName << std::endl;
            //std::cout << strNewFileName << std::endl;

            if( IsDiffFiles(strOldFileName.c_str(),
                strNewFileName.c_str())!=-1) // 파일을 비교
            {
                //이름은 같으나 내용이 변경 된 경우 업데이트의 경우이다.
                iTotalPatchSize += nSize;
                iVer=k->second.iVersion+1;
                sprintf(strOutput,"[UpdateFile] %s [Version] %d [Size] %d [CRC] %s", gcXmlData.strFileName.c_str(),iVer, gcXmlData.iFileSize, gcXmlData.strCRC.c_str());
                //std::cout << strOutput << std::endl;
                logmanager.PushLogData(LOG_UPDATEFILE,strOutput);
                if (m_KomProc.UpdateFile(strOldPath,strNewPath,node->ToElement()->Attribute("Name"),&logmanager)==false)
                    return false;
            }
            else
            {
                //완벽하게 일치 하는 경우 같은 파일로 인식
                iVer=k->second.iVersion;
                //sprintf(strOutput,"Same File :%s :: Version %d",k->second.strFileName.c_str(),iVer);
                //std::cout << strOutput << std::endl;
                //logmanager.PushLogData(LOG_SAMEFILE,strOutput);
                nNotUpdateFileCount++;
            }
            mapXmlData.erase(k);
        }
        node->ToElement()->SetAttribute("Version", iVer);
        //node->ToElement()->SetAttribute("Version",10);
        fileElementNew->InsertEndChild(*node);
        node=node->NextSibling("Item");
    }

    for (std::map<std::string, GC_Xml_Data>::iterator k = mapXmlData.begin(); k != mapXmlData.end(); advance(k, 1))
    {
        // 지워지지 않고 남은 것은 새 버젼.
		sprintf(strOutput,"[DeleteFile] %s [Version] %d [Size] %d [CRC] %s",k->second.strFileName.c_str(),k->second.iVersion, k->second.iFileSize, k->second.strCRC.c_str());
        //std::cout << strOutput << std::endl;
        logmanager.PushLogData(LOG_DELETEFILE,strOutput);

    }

    logmanager.PrintLogDate(&kfLogfile);
    return true;
} // CompareFileVersion()

bool KEnumFile::CreateCRCKOM( const char* szCurDir, const char* szFileName, const char* szOutFileName )
{
    try
    {

        std::string strCurDir = szCurDir;
        std::string strVerFileName = szFileName;
        std::string strOutFileName = szOutFileName;

        std::string strVerFileFullName = strCurDir + strVerFileName;
        // CREATE CRC.KOM ...        
        TiXmlDocument xml( strVerFileFullName.c_str() );
        xml.LoadFile();

        TiXmlNode* node = xml.FirstChild( "PatchInfo" );
        node = node->FirstChild("File");
        node = node->FirstChild("Item");

        std::vector<string> vecCrcList;
        do
        {
            std::string strFileName = node->ToElement()->Attribute("Name");

            if ( strFileName.find(".kom") != std::string::npos )
            {
                std::string strFileFullName = strCurDir + strFileName;
                GCMassFile mass;
                GCMassFile::MASSFILE_MEMBERFILEINFO mem;

                if ( mass.GetMemberFile( strFileFullName.c_str(), "crc.xml", &mem ) )
                {

                    replace( strFileName, ".kom" , ".xml" );
                    char szTemp[MAX_PATH];
                    GetFileNameExceptFolder( szTemp, strFileName.c_str() );

                    strFileFullName = strCurDir + szTemp;
                    GCUTIL_STR::ToLower(strFileFullName);
                    vecCrcList.push_back( strFileFullName );

                    {
                        FILE* pCrcFile;                
                        pCrcFile = fopen( strFileFullName.c_str(), "wb" );
                        fwrite( mem.pRealData, mem.ulSize, 1, pCrcFile );
                        fclose( pCrcFile );
                    }
                }
            }
        } while(node = node->NextSibling("Item") );

        //중복된 파일은 걸러드린다.
        std::set<std::string> setCheckDuplicate;
        std::set<std::string> setDuplicatedFile;
        for ( int i = 0 ; i < vecCrcList.size(); i++ )
        {
            char szTemp[MAX_PATH];
            GetFileNameExceptFolder( szTemp, vecCrcList[i].c_str() );

            if ( setCheckDuplicate.find( szTemp ) != setCheckDuplicate.end() )
            {
                setDuplicatedFile.insert( szTemp );  
            }
            else
            {
                setCheckDuplicate.insert( szTemp );

            }            
        }

        for ( int i = vecCrcList.size() - 1 ; i >= 0; i-- )
        {
            char szTemp[MAX_PATH];
            GetFileNameExceptFolder( szTemp, vecCrcList[i].c_str() );

            if ( setDuplicatedFile.find( szTemp ) != setDuplicatedFile.end() )
            {
                DeleteFileAbsolutely( vecCrcList[i].c_str() );
                vecCrcList.erase( vecCrcList.begin() + i );
            }
        }


        strOutFileName = strCurDir + strOutFileName;

        {
            GCMassFile massResult;
            massResult.ModifyMassFile( &massResult, (char*)strOutFileName.c_str() , &vecCrcList );
        }


        for ( int i = 0 ; i < vecCrcList.size(); i++ )
        {
            DeleteFileAbsolutely( vecCrcList[i].c_str() );
        }   

        return true;    
    }
    catch (...)
    {
    	return false;
    }

}
