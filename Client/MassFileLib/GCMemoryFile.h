#pragma once

// SAFE_DELETE
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif

// SAFE_DELETE_ARRAY
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif

// SAFE_RELEASE
//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// SAFE_FILE_CLOSE
#ifndef SAFE_FILE_CLOSE
#define SAFE_FILE_CLOSE( p ) { if( p ) { fclose( p ); } }
#endif 

#ifndef FILESEEK_DEFINED
#define FILESEEK_DEFINED
enum
{
	FILESEEK_CUR = 0,
	FILESEEK_SET,
	FILESEEK_END,
};
#endif
#include <string>

class GCMassFileManager;

class GCMemoryFile
{
public:
	GCMemoryFile(GCMassFileManager *manager,std::string filename);
	virtual ~GCMemoryFile();
	
	int		ReadFile(void* pDest, int ReadBytes);
	char*	GetDataPointer() { return m_pCursor; }
	int		FileSeek(int iPos, int Pivot);
	int		GetFileSize() { return m_iSize; }
	bool	IsLoaded(){return m_bLoaded;}

private:
	int   m_iSize;
	int   m_iCursorPos;
	char* m_pData;
	char* m_pCursor;	
	GCMassFileManager * m_pMFManager;
	bool  m_bLoaded;
};