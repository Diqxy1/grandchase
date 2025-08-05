#ifndef _GCKGCMISSIONEQUIPPREVIEW_S4_H_
#define _GCKGCMISSIONEQUIPPREVIEW_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMissionEquipPreview_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionEquipPreview_S4 );
    /// Default constructor
    KGCMissionEquipPreview_S4( void );
    /// Default destructor
    virtual ~KGCMissionEquipPreview_S4( void );
    /// Copy constructor
    KGCMissionEquipPreview_S4( const KGCMissionEquipPreview_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionEquipPreview_S4& operator=( const KGCMissionEquipPreview_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    enum
    {
        VIEWPORT_OFFSET_X   = 32,
        VIEWPORT_OFFSET_Y   = 27,
        VIEWPORT_WIDTH      = 245,
        VIEWPORT_HEIGHT     = 255,
    };

private:
    KD3DWnd                 *m_pkBtn_Close;
    KD3DSizingBox           *m_pkBackGround;
    KD3DSizingBox           *m_pkAlphaFrame;
    std::vector<GCITEMID>   m_vecPriviewEquip;

private:
    int                 m_iCharIdx;
    int                 m_iExtraIdx;
    PLAYER              m_kPlayer;

private:
    GCDeviceMotionFRM*  m_pMotion;
    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;

private:
    unsigned char       m_cFrame;
    float               m_fZoom;
    float               m_fCharX;
    float               m_fAngleMove;
    int                 m_iMotionCount;
    bool                m_bAngleChanged;
    D3DXMATRIX          m_matProject;
    D3DVIEWPORT9        m_viewport;

private:
    float               m_fStartAngle;
    POINT               m_ptStartMouse;

private:
    void OnClickClsoe();
    void OnEnterCursor();

private:
    void UpdateMeshObject();
    void DeleteMeshObject();
    void ProcessCharWndAngle();
    void ChangeCharWndZoom( bool bZoom );

private:
    void SetChar( int iCharType );
    void InitCharacterInfo();

public:
    void SetPreview();;
    void SetPreviewWindow(std::vector<GCITEMID> &vecItemID);

    void ReloadMotion();
};

DEFINE_WND_FACTORY( KGCMissionEquipPreview_S4 );
DECLARE_WND_FACTORY( KGCMissionEquipPreview_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionEquipPreview_S4 );

#endif  // _GCKGCMISSIONEQUIPPRIVIEW_S4_H_