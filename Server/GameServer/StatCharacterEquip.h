#pragma once
#include "statistics.h"
#include <map>
#include "PrimitiveTypedef.h"

SmartPointer( KStatCharacterEquip );

struct KCharLvPromotion;
class KStatCharacterEquip : public KStatistics
{
public:
    KStatCharacterEquip(void);
    virtual ~KStatCharacterEquip(void);

    virtual void Tick();
    virtual void Dump( std::wostream& stm );

    void Flush();
    void AddStat( const KCharLvPromotion& kKey, const std::map<GCITEMID,DWORD>& mapStat );

protected:
    DWORD GetDataSize();
    std::map<KCharLvPromotion,std::map<GCITEMID,DWORD> > m_mapCharEquip;
};
