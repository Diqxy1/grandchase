#pragma once

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;

class KGCHeroItemPreview : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroItemPreview );
    /// Default constructor
    KGCHeroItemPreview( void );
    /// Default destructor
    virtual ~KGCHeroItemPreview( void );
    /// Copy constructor
    KGCHeroItemPreview( const KGCHeroItemPreview& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroItemPreview& operator=( const KGCHeroItemPreview& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreate( void );

private:
    enum
    {
        MAX_CHAR_NAME_LEN   = 8,
        VIEWPORT_OFFSET_X   = 18,
        VIEWPORT_OFFSET_Y   = 26,
        VIEWPORT_WIDTH      = 197,
        VIEWPORT_HEIGHT     = 229,
    };

    KSafeArray<KD3DWnd*, GC_CHAR_NUM>   m_apkCharacterName;
    KSafeArray<KD3DWnd*, GC_CHAR_NUM>   m_apkCharacterFace;
    KD3DWnd*            m_pkBtnClose;
    KD3DWnd*            m_pkImgRenderPos;   //render_position

    
    int                 m_iCurrentCharacterName;
    bool                m_bClose;

    PLAYER              m_Player;
    std::vector<GCItem*> m_vecItem;    
    GCObject*           m_pMeshKOGRoom;
    D3DVIEWPORT9        m_viewport;
    D3DXMATRIX          m_matProject;
    
    GCDeviceMotionFRM*  m_pShopMotion;

    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;


    int					m_iMotionCount;
    int                 m_cFrame;
    float				m_fCharX;

    POINT               m_ptStartMouse;
    float               m_fStartAngle;
    float               m_fAngleMove;
    float				m_fZoom;
    bool				m_bAngleChanged;
   
    int                 m_iExtraCharNum;

    KGCItemInformationBox* m_pkHeroItemPrevInfo;

public:
    void SetPreviewWindow(int iCharacterType, std::vector<GCItem*>& pItem);
    void InitCharacterInfo();   
    bool GetClose( void ) { return m_bClose; }
    void SetClose(bool bClose) { m_bClose = bClose; }

    void ReloadMotion();

private:
    void SetCharacterName( int iCharacterType );
    void SetChar(int iCharType );
    void UpdateMeshObject( void );
    void DeleteMeshObject( void );    

    void ChangeCharWndZoom(bool bZoom);
    void ProcessCharWndAngle( void );
};

DEFINE_WND_FACTORY( KGCHeroItemPreview );
DECLARE_WND_FACTORY( KGCHeroItemPreview );
DECLARE_WND_FACTORY_NAME( KGCHeroItemPreview );
