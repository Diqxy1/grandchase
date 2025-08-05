#pragma once
#include "odbc/Odbc.h"
#include <vector>
#include "UserPacket.h"
#include "ItemManager.h"
#include "SP.h"

struct KItem;
class KBillDB
{
public:
    static void InitFileDSN( const char* szFileDSN );
    KBillDB(void);
    ~KBillDB(void);

public:
    // Query
    bool CheckBuyLimitItem( IN const DWORD dwUID, IN const GCITEMID dwItemID );
    void CommitBuyLimitItem( IN const DWORD dwUID, IN const GCITEMID dwItemID );
    bool NickToLoginUID( IN const std::wstring& strNick, OUT DWORD& dwLoginUID );
    bool CharNickToLoginUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ );
    bool SetTimeOutMark( IN __int64 dwItemSetUID );
    bool SetBillingKey( IN const __int64 WaitItemUID, IN __int64& BillingKey );
    bool NickNameToLogin( IN const std::wstring& strNickName, OUT std::wstring& strLogin );

protected:
    // ODBC
    bool DriveConnect();
    std::wstring GetLastQuery() { return m_kODBC.m_strLastQuery; }
    KSP     m_kHelper;
private:
    static std::wstring ms_wstrFileDSN;
    KODBC   m_kODBC;

};
