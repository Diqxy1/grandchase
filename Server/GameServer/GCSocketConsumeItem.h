#pragma once
#include "Lua/KLuaManager.h"
#include <map>
#include <Thread/Locker.h>
#include "UserPacket.h"
#include "CenterPacket.h"
class KGCSocketConsumeItem
{
public:
    KGCSocketConsumeItem(void);
    ~KGCSocketConsumeItem(void);

    enum{
        FAIL_GRADE = -1,

        GRADE_NORMAL = 0,
        GRADE_RARE = 1,
        GRADE_EPIC = 2,
        GRADE_LEGEND = 3,
        GRADE_MYSTIC = 4,

        GRADE_NUM
    };

    enum CONSUME_KIND {
        GAMEPOINT,
        CASHITEM,
    };

private:
    std::map<DWORD, KSocketConsumeInfo> m_mapItemUseCount;
    std::map< DWORD, KSocketConsumeInfo > m_mapConsumeGP;

public:

    void GetCashItemNum( IN DWORD EquipLevel, IN int ItemLevel, OUT int& ItemCount );

    bool GetConsumeGP( IN DWORD EquipLevel, IN int ItemLevel, OUT int& GP );

    void UpdateItemTable( IN KECN_SOCKET_ITEM_GP_TABLE_NOT& kPacket );

protected:

private:
    mutable KncCriticalSection m_csSocketInfo;

};