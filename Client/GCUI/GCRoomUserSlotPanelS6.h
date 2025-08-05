#pragma once

class KGCRoomUserSlotPanelS6 : public KD3DWnd    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCRoomUserSlotPanelS6);
    /// Default constructor
    KGCRoomUserSlotPanelS6(void);
    /// Default destructor
    virtual ~KGCRoomUserSlotPanelS6(void);
    /// Copy constructor
    KGCRoomUserSlotPanelS6(const KGCRoomUserSlotPanelS6&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCRoomUserSlotPanelS6& operator=(const KGCRoomUserSlotPanelS6&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);

public:
    void SetRoomMemberPingInfo(std::map<DWORD, int>& mapPlayerInfo, std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_);
    void LevelUpEffect(int levelUpPlayerIndex);
    void ClearUserEnchant();
    void SetMaxUserSlot(int nMaxUserSlot) { m_nMaxUserSlot = nMaxUserSlot; }

public:
    void OnClickUserSlot(int nIndex);
    void Destroy();

public:
    void UpdateGameData( bool bUpdateExtraMotion = false );

public:
    KSafeArray<KGCRoomUserSlotS6*, MAX_PLAYER_NUM>     m_apkUserSlot;
    KSafeArray<KD3DWnd*, MAX_PLAYER_NUM>               m_apkUserMyBack;

public:
    int m_nMaxUserSlot;

};

DEFINE_WND_FACTORY(KGCRoomUserSlotPanelS6);
DECLARE_WND_FACTORY(KGCRoomUserSlotPanelS6);
DECLARE_WND_FACTORY_NAME(KGCRoomUserSlotPanelS6);
