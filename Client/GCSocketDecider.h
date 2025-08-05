#pragma once
#include "GCSocketTable.h"
#include <Thread/Locker.h>

class KGCSocketDecider
{
public:
    KGCSocketDecider(void);
    ~KGCSocketDecider(void);

private:
    typedef std::map<int,KGCSocketTable> TableList;

    mutable KncCriticalSection    m_csTables;
    std::map<int,KGCSocketTable>    m_mapSocketTables;
    std::map<int,KGCSocketTable>    m_mapBasicTables;
   

    bool _GetSocketList( IN int iIndex, IN TableList& mapList_, OUT std::list<int>& SocketList_, IN bool bDump = false );
    bool SaveList( std::ofstream& file, std::string strTableName, TableList& mapList );
public:
    bool GetSocketList( IN int iIndex, OUT std::list<int>& SocketList_, IN bool bDump = false );
    bool GetBasicSocketList( IN int iGrade, OUT std::list<int>& SocketList_, IN bool bDump = false );
    
    bool LoadScript();
    bool SaveScript( IN std::string strFileName_ );


};
