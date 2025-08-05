#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "CommonPacket.h"
#include "KncTimer.h"

class KCnOneaday
{
    DeclareSingleton( KCnOneaday );
    NiDeclareRootRTTI( KCnOneaday );
    DeclToStringW;

public:
    KCnOneaday(void);
    ~KCnOneaday(void);

    void Init();

    void SetItemList( std::vector<KOneadayItem>& vecOneadayItemList_ );
    void GetItemList( std::vector<KOneadayItem>& vecOneadayItemList_ );
    void Tick();
    void SetKItems( std::vector<KItem>& vecOneadayItem_ );
    void UpdateItemCount( IN std::vector<KOneadayItem>& vecOneadayItem_ );

private:
    void QueuingToDB( IN const bool bFirst_ );

private:
    mutable KncCriticalSection m_csOneaday;
    std::vector<KOneadayItem> m_vecOneadayItemList;
    KncTimer m_kTimer;
    std::vector<KItem> m_vecOneadayKItem;
};

DefSingletonInline( KCnOneaday );
DeclOstmOperatorA( KCnOneaday );