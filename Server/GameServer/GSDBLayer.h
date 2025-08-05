#pragma once

#include "DBLayer.h"
#include "ToString.h"
#include "UserPacket.h"
#include <KNCSingleton.h>

class KODBC;
class KGSDBLayer : public KDBLayer
{
    DeclToStringW;
    NiDeclareRTTI;
    DeclareSingleton( KGSDBLayer );

public:
    KGSDBLayer(void);

public:
    virtual ~KGSDBLayer(void);
    virtual void RegToLua( std::shared_ptr<lua_State> sptlua );

    bool GetComment( IN KODBC* pkODBC, IN int iCommentID, OUT std::wstring& strComment );
    bool GetItemName( IN KODBC* pkODBC, IN int iItemID, OUT std::wstring& strItemName );
    void ClearCommentTable();
    void ClearItemNameTable();
    void Init_ODBC( const char* szMainODBC, const char* szStatODBC );

protected:
    // util function
    virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager

    struct KCommentTable {   // db의 comment table을 얻어와 캐싱한다.
        mutable KncCriticalSection  m_cs;
        std::map<int,std::wstring>   m_mapComment;
    } m_kCommentTable;

    struct KItemName {  // db에서 아이템 이름을 얻어와서 캐싱한다.
        mutable KncCriticalSection  m_cs;
        std::map<int,std::wstring>  m_mapItemName;
    } m_kItemNameTable;


};

DefSingletonInline( KGSDBLayer );