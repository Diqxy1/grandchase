#include "GCGradeDecider.h"
#include "KLuaManager.h"
#include <fstream>
#include <dbg/dbg.hpp>
FILE_NAME_FOR_LOG
KGCGradeDecider::KGCGradeDecider(void)
{
}

KGCGradeDecider::~KGCGradeDecider(void)
{
}

bool KGCGradeDecider::LoadScript()
{
    KLuaManager luaMgr;

    _JIF( GCFUNC::LoadLuaScript( luaMgr, "ItemGradeTable.lua" ), return false );

    int iIndex = 0;

    _JIF( luaMgr.BeginTable( "ItemGradeTable" ) == S_OK, return false );
    
    TableList mapTables;
    while ( ++iIndex ) {

        KGCGradeTable kTable;

        if ( kTable.LoadScript( iIndex, luaMgr ) == false ) {
            break;
        }

        mapTables.insert( TableList::value_type( iIndex, kTable ) );
    }
    luaMgr.EndTable();

    KLocker lock( m_csTable );
    m_mapTables = mapTables;

    return true;
}

bool KGCGradeDecider::SaveScript( IN std::string strFileName_ )
{
    std::ofstream file;

    file.open( strFileName_.c_str() );
    
    if( file.is_open() == false )
    {
        START_LOG( cerr, L"File is not opended!!!" );
        return false;
    }

    Table::SaveEnum( file );

    file<<"\n\nItemGradeTable = {\n";

    KLocker lock( m_csTable );
    TableList::iterator mit;
    for( mit = m_mapTables.begin() ; mit != m_mapTables.end() ; ++mit )
    {
        mit->second.SaveTable( file, 1 );
    }

    file<<"}\n";
    return true;
}

int KGCGradeDecider::GetGrade( int iTableIndex )
{
    KLocker lock( m_csTable );

    TableList::iterator mit = m_mapTables.find( iTableIndex );

    if( mit == m_mapTables.end() )
        return Table::FAIL_GRADE;

    return mit->second.GetGrade();
}

std::string KGCGradeDecider::DumpTables()
{
    TableList::iterator mit;

    KLocker lock( m_csTable );

    std::stringstream stm;
    for( mit = m_mapTables.begin() ; mit != m_mapTables.end() ; ++mit )
    {
        stm<<"==============================================================="<<std::endl;
        stm<<"Table Index : "<<mit->first<<std::endl;
        stm<<"---------------------------------------------------------------"<<std::endl;
        std::map<int, int> mapCounter;
        int iLoop = 10000;
        for( int i = 0 ; i < iLoop ; ++i )
        {
            int iGrade = mit->second.GetGrade();
            ++mapCounter[iGrade];
        }

        std::map<int, int>::iterator mit;
        for( mit = mapCounter.begin() ; mit != mapCounter.end() ; ++mit )
        {
            stm<< mit->first << "\t:\t"<< mit->second 
                << "( " << static_cast<float>(mit->second) / static_cast<float>(iLoop) * 100.f << "% )" << std::endl;
        }

        stm<<"==============================================================="<<std::endl;
    }
    return stm.str();
}

bool KGCGradeDecider::GetTable( IN int iTableIndex, OUT Table& kTable_ )
{
    KLocker lock( m_csTable );

    TableList::iterator mit = m_mapTables.find( iTableIndex );

    if( mit == m_mapTables.end() )
        return false;

    kTable_ = mit->second;
    return true;
}

int KGCGradeDecider::GetFreeIndex() const
{
    TableList::const_iterator mit;

    int iLastIndex = 0;

    KLocker lock( m_csTable );

    for( mit = m_mapTables.begin() ; mit != m_mapTables.end() ; ++mit )
    {
        if( mit->first > iLastIndex )
            iLastIndex = mit->first;
    }
    return iLastIndex + 1;
}

int KGCGradeDecider::AddTable( IN KGCGradeTable& kTable_ )
{
    KLocker lock( m_csTable );
    int iIndex = GetFreeIndex();
    kTable_.SetIndex( iIndex );

    m_mapTables.insert( TableList::value_type( iIndex, kTable_ ) );
    return iIndex;
}