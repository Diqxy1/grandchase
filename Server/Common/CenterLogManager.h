#pragma once
#include "GenericLogfile.h"
#include <map>
#include <fstream>
#include <KNCSingleton.h>
#include <RTTI.h>
#include <ToString.h>
#include <Thread/Locker.h>

class KCenterLogManager
{
	DeclareSingleton(KCenterLogManager);
	NiDeclareRootRTTI(KCenterLogManager);
	DeclToStringW;

public:
	KCenterLogManager(void);
	~KCenterLogManager(void);

#undef _ENUM
#define _ENUM( id, comment ) id,
	enum LogType
	{
#include "CenterLogfile_def.h"
	};

	KGenericLogfile& Get(int nKey);
	void Init();
	void ChangeFile(int nKey);
	void CleanUp();
	void ToggleWrite(int nID);

protected:
	KGenericLogfile m_kUnKnown;
	static const wchar_t* ms_szFolderName[];
	std::map<int, KGenericLogfile*> m_mapLogList;
	mutable KncCriticalSection m_csMapLogList;
};
DefSingletonInline(KCenterLogManager);

#define BUILD_GLOG( data )           GLOG::tab << L#data L" : " << data << GLOG::endl
#define BUILD_GLOGc( data )          GLOG::tab << L#data L" : " << static_cast<int>(data) << GLOG::endl
#define BUILD_GLOGtm( data )         GLOG::tab << L#data L" : " << (LPCWSTR)data.Format( KNC_TIME_FORMAT ) << GLOG::endl