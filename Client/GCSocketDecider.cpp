#include <stdafx.h>
#include "GCSocketDecider.h"
#include <fstream>
#include <dbg/dbg.hpp>
FILE_NAME_FOR_LOG
KGCSocketDecider::KGCSocketDecider(void)
{
}

KGCSocketDecider::~KGCSocketDecider(void)
{
}

bool KGCSocketDecider::GetSocketList( IN int iIndex, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/ )
{
    return _GetSocketList( iIndex, m_mapSocketTables, SocketList_, bDump );
}

bool KGCSocketDecider::GetBasicSocketList( IN int iGrade, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/ )
{
    return _GetSocketList( iGrade, m_mapBasicTables, SocketList_, bDump );
}

bool KGCSocketDecider::_GetSocketList( IN int iIndex, IN TableList& mapList_, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/ )
{
    KLocker lock( m_csTables );
    TableList::iterator mit = mapList_.find( iIndex );

    if( mit == mapList_.end() )
        return false;

    return mit->second.GetSocketList( SocketList_, bDump );
}

bool KGCSocketDecider::LoadScript()
{
    KLuaManager luaMgr;
    std::map<int,KGCSocketTable> mapSocketTables;
    std::map<int,KGCSocketTable> mapBasicTables;

    _JIF( GCFUNC::LoadLuaScript( luaMgr, "ItemSocketTable.lua" ), return false );

    int iIndex = 0;
    _JIF( luaMgr.BeginTable( "ItemSocketTable" ) == S_OK, return false );

    do{
        KGCSocketTable kTable;

        if( kTable.LoadScript( iIndex, luaMgr ) == false ) {
            break;
        }

        mapSocketTables.insert( TableList::value_type( iIndex, kTable ) );
    }while( ++iIndex );

    luaMgr.EndTable();


    iIndex = 0;
    _JIF( luaMgr.BeginTable( "DefaultSocketTable" ) == S_OK, return false );

    
    do{
        KGCSocketTable kTable;

        if( kTable.LoadScript( iIndex, luaMgr ) == false )
            break;

        mapBasicTables.insert( TableList::value_type( iIndex, kTable ) );
    }while( ++iIndex );
    luaMgr.EndTable();

    KLocker lock( m_csTables );
    m_mapSocketTables.swap( mapSocketTables );
    m_mapBasicTables.swap( mapBasicTables );

    return true;
}

bool KGCSocketDecider::SaveScript( IN std::string strFileName_ )
{
    std::ofstream file;

    file.open( strFileName_.c_str() );

    if( file.is_open() == false )
    {
        START_LOG( cerr, L"File is not opened!!!" );
        return false;
    }

    KGCSocketTable::SaveEnum( file );

    file<<"\n\n-- Socket Table for each Grade\n";
    SaveList( file, "DefaultSocketTable", m_mapBasicTables );
    file<<"-- Socket Table for special items\n";
    SaveList( file, "ItemSocketTable", m_mapSocketTables );
    return true;
}

bool KGCSocketDecider::SaveList( std::ofstream& file, std::string strTableName, TableList& mapList )
{
    file<<strTableName<<" = {\n";

    TableList::iterator mit;
    KLocker lock( m_csTables );
    for( mit = mapList.begin() ; mit != mapList.end() ; ++mit )
    {
        mit->second.SaveTable( file, 1 );
    }

    file<<"}\n\n\n";

    return true;
}

