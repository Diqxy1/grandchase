#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "UserPacket.h"

class KLuaManager;
class KMagicBox
{
    DeclareSingleton( KMagicBox );
    NiDeclareRootRTTI( KMagicBox );
    DeclToStringW;
public:
    KMagicBox(void);
    ~KMagicBox(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void UpdateEvent( IN std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapItemList, IN std::map<GCITEMID,float>& mapLuckRatio );

    GCITEMID GetNormalKeyItemID();
    GCITEMID GetSpecialKeyItemID();

    bool IsEnable() { KLocker lock(m_csMagicBox); return m_bEnable; }
    float CalcNeedKeyRatio( IN const int& nClearCount );

    void GetItemList( OUT std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapList );
    bool GetItemInfo( IN const GCITEMID& ItemID, OUT KDropItemInfo& kItem, OUT int& nNeedKeyCount );
    bool GetItemRatio( IN const GCITEMID& ItemID, OUT float& fRatio );
    USHORT GetVer() { KLocker lock(m_csMagicBox); return m_usVersion; }

    bool IsExistItem( IN const GCITEMID& itemID );
    bool IsEventTerm();
    void SendMagicBoxListNotToDB();
    USHORT GetDecKeyCount();

    void DumpInfo();

private:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );

private:
    mutable KncCriticalSection                          m_csMagicBox;
    bool                                                m_bEnable; // 기능 온오프
    USHORT                                              m_usVersion; // 이벤트 버전
    time_t                                              m_tmBegin; // 이벤트 시작 날짜.
    time_t                                              m_tmEnd; // 이벤트 종료 날짜.
    GCITEMID                                            m_NormalKeyItemID; // 고대의 마법석
    GCITEMID                                            m_SpecialKeyItemID; // 코우나트의 마력석
    float                                               m_fIncRate; // 키 증가비율 = 아이템 힉득횟수 * m_fIncRate
    std::map<GCITEMID, std::pair<KDropItemInfo,int> >   m_mapItemList; // 보상 아이템 정보 [ItemdID, [ItemInfo,NeedKeyCount]]
    std::map<GCITEMID,float>                            m_mapLuckRatio; // 아이템별 행운 확률 [ItemdID,LuckRatio]
    float                                               m_fLuckRatioDBtoServer; // DB에서 받아온 행운확률 서버적용시 곱하는 배율값
    USHORT                                              m_usDecKeyCount; // 마력석 1회사용시 마력석 소모개수
};
DefSingletonInline( KMagicBox );
DeclOstmOperatorA( KMagicBox );