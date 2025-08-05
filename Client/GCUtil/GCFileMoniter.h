#pragma once
#include "AloFileMonitor.h"
#include <KNCSingleton.h>
//#include <vector>
//#include <string>

class KGCFileMoniter
{
	DeclareSingleton( KGCFileMoniter );
public:
	KGCFileMoniter(void);
	~KGCFileMoniter(void);
	void RegisterFile( std::string strFileName, PFnFileChanged pFunc );

private:
	std::vector< CAloFileMonitor* >	m_vecFileMoniters;
};

DefSingletonInline( KGCFileMoniter );