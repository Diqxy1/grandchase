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
    bool                                                m_bEnable; // ��� �¿���
    USHORT                                              m_usVersion; // �̺�Ʈ ����
    time_t                                              m_tmBegin; // �̺�Ʈ ���� ��¥.
    time_t                                              m_tmEnd; // �̺�Ʈ ���� ��¥.
    GCITEMID                                            m_NormalKeyItemID; // ����� ������
    GCITEMID                                            m_SpecialKeyItemID; // �ڿ쳪Ʈ�� ���¼�
    float                                               m_fIncRate; // Ű �������� = ������ ����Ƚ�� * m_fIncRate
    std::map<GCITEMID, std::pair<KDropItemInfo,int> >   m_mapItemList; // ���� ������ ���� [ItemdID, [ItemInfo,NeedKeyCount]]
    std::map<GCITEMID,float>                            m_mapLuckRatio; // �����ۺ� ��� Ȯ�� [ItemdID,LuckRatio]
    float                                               m_fLuckRatioDBtoServer; // DB���� �޾ƿ� ���Ȯ�� ��������� ���ϴ� ������
    USHORT                                              m_usDecKeyCount; // ���¼� 1ȸ���� ���¼� �Ҹ𰳼�
};
DefSingletonInline( KMagicBox );
DeclOstmOperatorA( KMagicBox );