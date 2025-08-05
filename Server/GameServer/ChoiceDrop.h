#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "UserPacket.h"

class KLuaManager;

class KChoiceDrop
{
    DeclareSingleton( KChoiceDrop );
    NiDeclareRootRTTI( KChoiceDrop );
    DeclToStringW;

public:
    KChoiceDrop(void);
    ~KChoiceDrop(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpInfo();
    void GetBoxList( OUT std::set<GCITEMID>& setList );
    bool GetBoxInfo( IN const GCITEMID& BoxItemID, OUT KChoiceBox& kInfo );

private:
    bool LoadBoxInfo( KLuaManager& kLuaMng, OUT std::map<USHORT, KDropItemInfo>& mapList );

protected:
    mutable KncCriticalSection      m_csChoiceDrop;
    std::set<GCITEMID>              m_setBoxItemID; // BoxItemID
    std::map<GCITEMID,KChoiceBox>   m_mapeBoxList; // 보상상자 [ItemID, 박스정보]

};
DefSingletonInline( KChoiceDrop );
DeclOstmOperatorA( KChoiceDrop );
