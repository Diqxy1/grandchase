#pragma once

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;

class KGCHeroDungeonShopSidePanel : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME(KGCHeroDungeonShopSidePanel);
    /// Default constructor
    KGCHeroDungeonShopSidePanel(void);
    /// Default destructor
    virtual ~KGCHeroDungeonShopSidePanel(void);
    /// Copy constructor
    KGCHeroDungeonShopSidePanel(const KGCHeroDungeonShopSidePanel&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCHeroDungeonShopSidePanel& operator=(const KGCHeroDungeonShopSidePanel&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);
    virtual void OnDestroy(void);
    virtual void FrameMoveInEnabledState(void);
    virtual void PostChildDraw(void);

public:
    void InitCharacterInfo();
    void SetPreviewWindow();
    void EquipItem(GCItem* item);
    void UnequipItem(GCItem* item);
    void UnequipItems();
    void ReloadAnimationMotion();

private:
    void UpdateMeshObject();
    void ProcessCharWndAngle();
    void ChangeCharWndZoom(bool bZoom);

public:

    KD3DWnd* m_pkImgRenderPos;   //render_position
    KD3DWnd* m_pkViewBasic;

    PLAYER              m_Player;

    GCObject* m_pMeshFootStage;
    D3DVIEWPORT9        m_viewport;
    D3DXMATRIX          m_matProject;

    GCDeviceMotionFRM* m_pWaitMotion;
    GCDeviceMotionFRM* m_pActionMotion;
    GCDeviceMotionFRM* m_pAnimationMotion;

    int					m_iMotionCount;
    int                 m_cFrame;
    float				m_fCharX;

    POINT               m_ptStartMouse;

    float               m_fStartAngle;
    float               m_fStartStageAngle;
    float               m_fAngleMove;
    float               m_fStageAngleMove;
    float				m_fZoom;

    bool				m_bAngleChanged;

    int                 m_iExtraCharNum;
};

DEFINE_WND_FACTORY(KGCHeroDungeonShopSidePanel);
DECLARE_WND_FACTORY(KGCHeroDungeonShopSidePanel);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonShopSidePanel);
