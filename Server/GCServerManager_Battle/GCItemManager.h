#ifndef _GC_ITEM_MANAGER_H_
#define _GC_ITEM_MANAGER_H_
#pragma pack( push, 1 )

#include <windows.h>
#include <vector>
#include "UserPacket.h"
#include "ItemStructure.h"

class KGCItemManager
{
private:
    struct GCItem
    {
    public:
        wchar_t         szName[30];
        wchar_t         szDesc[100];
        bool            bCash;
        BYTE            btLevel;
        short           shAttack;
        short           shDefence;
        short           shReserved1;  // 세트번호 (ver 1.1)
        short           shReserved2;  // 수량 (ver 1.1)
        short           shHpInc;
        DWORD           dwPrice;
        DWORD           dwPatternA;   // 패턴A 가격 (ver 1.1)
        DWORD           dwPatternB;   // 패턴B 가격 (ver 1.1)
        DWORD           dwExtra1;
        DWORD           dwExtra2;
        DWORD           dwExtra3;     // (ver 1.2)
        int             iGem;         // 젬 가격 (ver 1.2)
        int             iStrongLevel; // 강화 레벨 (ver 1.2)
        bool            bEffect;      // 이펙트 여부 (ver 1.2)
        int             iCrystal;     // 크리스탈 가격 (ver 1.3)

    public:
        GCItem( void )
        {
            ::ZeroMemory( this, sizeof( GCItem ) );
            shReserved1 = -1;
        }
        GCItem( const GCItem& src )
        {
            ::memcpy( this, &src, sizeof( GCItem ) );
        }
        ~GCItem( void )
        {
            // empty
        }
        const GCItem& operator=( const GCItem& src )
        {
            ::memcpy( this, &src, sizeof( GCItem ) );
            return *this;
        }
    };

    GCItem*                             m_pItemData;
    DWORD                               m_dwNumItem;
public:
    KGCItemManager();
    ~KGCItemManager();
    void GetItemInfo( std::vector<KAdminUseItemInfo>& vecItemInv );

};

#endif // _GC_ITEM_MANAGER_H_