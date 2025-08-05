#pragma once
#include "Lua/KLuaManager.h"
#include <map>
#include "NUserPacket.h"
#include <Thread/Locker.h>
#include "CenterPacket.h"


class KCnSocketConsumeItem
{
public:
    KCnSocketConsumeItem(void);
    ~KCnSocketConsumeItem(void);

    enum{
        FAIL_GRADE = -1,

        GRADE_NORMAL = 0,
        GRADE_RARE = 1,
        GRADE_EPIC = 2,
        GRADE_LEGEND = 3,
        GRADE_MYSTIC = 4,

        GRADE_NUM
    };

private:
    std::map<DWORD, KSocketConsumeInfo > m_mapItemUseCount;
    std::map< DWORD, KSocketConsumeInfo > m_mapConsumeGP;

public:
    bool LoadScript();
    void GetCashItemNum( IN int EquipLevel, IN int ItemLevel, OUT int& ItemCount );

    bool LoadScript_GP();
    void GetConsumeGP( IN int EquipLevel, IN int ItemLevel, OUT int& GP );

    void GetTableInfo( OUT KENU_SOCKET_TABLE_INFO_NOT& kPacket );
    void GetTableInfo( OUT KECN_SOCKET_ITEM_GP_TABLE_NOT& kPacket );


    bool LoadScript_Item();


protected:
    mutable KncCriticalSection m_csConsume;
    GCITEMID m_SocketOpenItem;

private:


};