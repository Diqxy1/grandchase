#pragma once

class KGCRoomUserSlotPanelS6;
class KGCRoomChatPanelS6;

class KGCRoomUserPanelS6 : public KD3DWnd    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCRoomUserPanelS6);
    /// Default constructor
    KGCRoomUserPanelS6(void);
    /// Default destructor
    virtual ~KGCRoomUserPanelS6(void);
    /// Copy constructor
    KGCRoomUserPanelS6(const KGCRoomUserPanelS6&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCRoomUserPanelS6& operator=(const KGCRoomUserPanelS6&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);
    virtual void FrameMoveInEnabledState(void);

public:
    void LevelUpEffectOnEnableFrameMove();
    void InitPrevValue();
    void InitLevelupEffect();

public:
    void UpdateGameData( bool bUpdateExtraModel = false );

public:

    void Destroy();
    void ClearUserEnchant();

public:
    void SetRoomMemberPingInfo(std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_);
    void SetLevelUpPlayer(const int i_);

    void SetUserSlot();
    KGCRoomUserSlotPanelS6* GetUserSlotPanel();

public:
    KGCRoomUserSlotPanelS6* m_pkUserSlotPanel_6;
    KGCRoomUserSlotPanelS6* m_pkUserSlotPanel_4;

public:
    std::map<DWORD, int> m_mapPlayerInfo;
    std::vector< int >  m_vecLevelUpPlayer;

    KSafeArray< int, MAX_PLAYER_NUM > m_apkPrevLevel;
    KSafeArray< int, MAX_PLAYER_NUM > m_apkPrevCharType;
};

DEFINE_WND_FACTORY(KGCRoomUserPanelS6);
DECLARE_WND_FACTORY(KGCRoomUserPanelS6);
DECLARE_WND_FACTORY_NAME(KGCRoomUserPanelS6);