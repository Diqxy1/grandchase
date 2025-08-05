#pragma once
#include "Lottery.h"
#include "KLuaManager.h"
#include <list>
#include <Thread/Locker.h>

class KGCSocketTable
{
public:
    KGCSocketTable(void);
    ~KGCSocketTable(void);

    enum SocketTableType{
        STT_RANDOM       = 0,
        STT_FIXED        = 2,
    };

    enum SocketType{
        ST_OPENED   = 0,
        ST_CLOSED   = 1,
        ST_RANDOM   = 2,
        ST_NUM,
    };

private:
    int         m_iTotalCount;
    int         m_iIndex;

    struct SSocket{
        int     m_iType;
        int     m_iCount;
        float   m_fProb;
    };
    std::map<SocketType,SSocket>    m_mapSocketList;

    std::string GetTab( int iTab )
    {
        std::stringstream stm;
        while( iTab-- )
        {
            stm<<"\t";
        }
        return stm.str();
    }
    static std::string GetEnumString( int iEnum );

public:
    bool GetSocketList( OUT std::list<int>& SocketList_, IN bool bDump = false );
    bool LoadScript( IN int iIndex_, IN OUT KLuaManager& luaMgr );
    bool SaveTable( IN OUT std::ofstream& file, IN int iIndex_ );

    int GetSocketCount() const { return m_iTotalCount; }

    static void SaveEnum( IN OUT std::ofstream& file );
};
