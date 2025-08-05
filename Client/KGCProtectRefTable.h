#pragma once

#include <WTypes.h>
//#include <string>

namespace ProtectTable
{
    class ProtectTableHelper
    {
    public:
        ProtectTableHelper(){}
        ~ProtectTableHelper(){}
        static DWORD g_dwProtectTableChkSum;
        static UCHAR g_ucTableIndex;
        static UCHAR Get();
    };
    static const BYTE g_iTableSize = 128;
    static const BYTE g_kProtectTable[] = 
    {
        124,
        119,
        114,
        109,
        104,
        99,
        94,
        89,
        84,
        79,
        74,
        69,
        64,
        59,
        54,
        49,
        44,
        39,
        34,
        29,
        24,
        19,
        14,
        9,
        4,
        127,
        122,
        117,
        112,
        107,
        102,
        0,
        97,
        92,
        87,
        82,
        77,
        72,
        67,
        62,
        57,
        52,
        47,
        42,
        37,
        32,
        27,
        22,
        17,
        12,
        7,
        2,
        125,
        120,
        115,
        110,
        105,
        100,
        95,
        90,
        85,
        80,
        75,
        70,
        65,
        60,
        55,
        50,
        45,
        40,
        35,
        30,
        25,
        20,
        15,
        10,
        5,
        123,
        118,
        113,
        108,
        103,
        98,
        93,
        88,
        83,
        78,
        73,
        68,
        63,
        58,
        53,
        48,
        43,
        38,
        33,
        28,
        23,
        18,
        13,
        8,
        3,
        126,
        121,
        116,
        111,
        106,
        101,
        96,
        91,
        86,
        81,
        76,
        71,
        66,
        61,
        56,
        51,
        46,
        41,
        36,
        31,
        26,
        21,
        16,
        11,
        6,
        1,
    };
#ifdef _DEBUG
    class ProtectDebugHelper
    {
    public:
        ProtectDebugHelper(){}
        ~ProtectDebugHelper(){}
        static DWORD g_sdwNewCount;
    };
#endif
};

//#include <wtypes.h>
////#include <assert.h>
////#include <vector>
//#ifdef _DEBUG
//#include <dbg/dbg.hpp>
//#endif

//class KGCProtectRefTable
//{
//public:
//    KGCProtectRefTable(void);
//    virtual ~KGCProtectRefTable(void);
//
//#ifdef _DEBUG
//    inline UCHAR GetTableIndex()
//    {
//        m_dwCall++;
//        return ::rand() % m_ucTableSize;
//    }
//    inline DWORD GetCall()
//    {
//        return m_dwCall;
//    }
//    inline void SetCall( DWORD dwCall )
//    {
//        m_dwCall = dwCall;
//    }
//#else
//    inline UCHAR GetTableIndex() const
//    {
//        return ::rand() % m_ucTableSize;
//    }
//#endif
//
//    inline UCHAR GetData( UCHAR ucIndex ) const
//    {
//        assert( ucIndex >= 0 && ucIndex < m_ucTableSize );
//        return m_aSeedTable[ ucIndex ];
//    }
//    void    GenerateTable();
//
//private:
//    UCHAR*  m_aSeedTable;
//    UCHAR   m_ucTableSize;
//
//#ifdef _DEBUG
//    DWORD   m_dwCall;
//#endif
//};

// 이렇게 쓰려면 하나의 cpp에 모든 Global Value들을 모아놓고 해야한다.
// 다음기회에..
// static KGCProtectRefTable g_kProtectTable;
