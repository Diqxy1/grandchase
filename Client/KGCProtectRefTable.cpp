#include "stdafx.h"
#include ".\kgcprotectreftable.h"

DWORD ProtectTable::ProtectTableHelper::g_dwProtectTableChkSum = 0;
UCHAR ProtectTable::ProtectTableHelper::g_ucTableIndex = 0;
#ifdef _DEBUG
DWORD ProtectTable::ProtectDebugHelper::g_sdwNewCount = 0;
#endif

UCHAR ProtectTable::ProtectTableHelper::Get()
{
    g_ucTableIndex++;
    if( g_ucTableIndex >= ProtectTable::g_iTableSize )
        g_ucTableIndex = 0;

    return g_ucTableIndex;
}

//
////
////
//
//
//
//
//#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
//
//KGCProtectRefTable::KGCProtectRefTable(void) : m_aSeedTable(NULL)
//{
//    ::srand( static_cast<unsigned int>( ::time( NULL ) ) );
//    // Must be Call!!
//    GenerateTable();
//
//#ifdef _DEBUG
//    m_dwCall = 0;
//#endif
//}
//
//KGCProtectRefTable::~KGCProtectRefTable(void)
//{
//    SAFE_DELETE_ARRAY( m_aSeedTable );
//}
//
//// 항상 srand( time(NULL) ) 부분 뒤에 위치해야 함.
//void KGCProtectRefTable::GenerateTable()
//{
//    do
//    {
//        m_ucTableSize = static_cast<UCHAR>( (::rand() % UCHAR_MAX) );
//    } while( m_ucTableSize < SCHAR_MAX );
//
//    m_aSeedTable = new UCHAR[ m_ucTableSize ];
//
//    PrimeSearch search( m_ucTableSize );
//    for( int i = 0; i < m_ucTableSize; ++i )
//    {
//        m_aSeedTable[i] = search.GetNext() + 1;
//    }
//}
