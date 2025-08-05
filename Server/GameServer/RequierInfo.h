#pragma once
#include <vector>
#include <map>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <ToString.h>

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

struct sRequire
{
    typedef std::vector< std::pair<char,DWORD> > VEC_TYPE_ITEM;

    int             m_nID;
    VEC_TYPE_ITEM   m_vecItems;
};

class KRequierInfo
{
    DeclareSingleton( KRequierInfo );
    DeclToStringW;
public:
    KRequierInfo(void);
    ~KRequierInfo(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetRequierData( IN const int nID, OUT sRequire& requireData );

    mutable KncCriticalSection  m_csRequires;
    std::map< int, sRequire >   m_mapRequires;
};

DefSingletonInline( KRequierInfo );
DeclOstmOperatorA( KRequierInfo );