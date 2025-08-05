#pragma once
#include "GCGradeTable.h"
#include <Thread/Locker.h>

class KGCGradeDecider
{
public:
    KGCGradeDecider(void);
    ~KGCGradeDecider(void);

    typedef KGCGradeTable               Table;
    typedef std::map<int,KGCGradeTable> TableList;
private:
    mutable KncCriticalSection      m_csTable;
    std::map<int,KGCGradeTable>     m_mapTables;

    int GetFreeIndex() const ;
public:
    int GetGrade( int iTableIndex );
    size_t GetTableCount() { KLocker lock( m_csTable ); return m_mapTables.size(); }
    const TableList& GetContainer() const { return m_mapTables; }
    bool GetTable( IN int iTableIndex, OUT Table& kTable_ );
    int AddTable( IN Table& kTable_ );

    bool LoadScript();
    bool SaveScript( IN std::string strFileName_ );

    std::string DumpTables();
};
