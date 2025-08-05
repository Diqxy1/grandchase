//#ifndef _KGCCHARWIEWWND_H_
//#define _KGCCHARWIEWWND_H_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
////#include "../uifx/D3DWnd.h"
////#include "../uifx/ActionListener.h"
//
//class GCDeviceMeshP3M;
//class GCDeviceMotionFRM;
//class GCObject;
//class KPet;
//
//class KGCCharViewWnd : public KD3DWnd,          // extends
//    public KActionListener   // implements
//{
//public:
//    DECLARE_CLASSNAME( KGCCharViewWnd );
//    /// Default constructor
//    KGCCharViewWnd( void );
//    /// Default destructor
//    virtual ~KGCCharViewWnd( void );
//    /// Copy constructor
//    KGCCharViewWnd( const KGCCharViewWnd& )
//    { ASSERT( false && "Do not use copy constructor!" ); }
//    /// Assignment operator
//    const KGCCharViewWnd& operator=( const KGCCharViewWnd& )
//    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
//
//public:
//    enum
//    {
//        VIEWPORT_OFFSET_X   = 0,
//        VIEWPORT_OFFSET_Y   = 0,
//        VIEWPORT_WIDTH      = 270,
//        VIEWPORT_HEIGHT     = 250,
//    };
//
//public:
//    void ChangeCharWndZoom( bool _bZoomUp );
//    void Initialize();
//    //====================================================================================
//    // 2007-06-20 오후 8:52:20 : 우동완
//    // 사실 RenderManager의 Render 함수가 각각의 Object들을 따로 그릴수 있게 되어서 이제 이런 부분은 필요 없지만... 
//    // 오늘은 마나가 다 되어 수정하기가 귀찮네요;
//    // 나중에 고치께요. 누군가 Claim 들어오면... -_-;
//    void UpdateMeshObject();
//    void DeleteMeshObject();
//    KPet*               m_pkPet;
//    GCObject*           m_pMeshKOGRoom;
//
//    virtual void ActionPerformed( const KActionEvent& event );
//protected:
//    virtual void OnCreate( void );
//    virtual void OnDestroy( void );
//    virtual void OnActivate( void );
//    virtual void FrameMoveInEnabledState( void );
//    virtual void PostDraw( void );
//    void SetChar(int iCharType);
//
//protected:
//    IDirect3DDevice9*   m_pd3dDevice;
//    D3DVIEWPORT9        m_viewport;
//    float               m_fAngleMove;
//
//    int                 m_iExtraCharNum;
//
//    GCDeviceMotionFRM*  m_pShopMotion;
//    GCDeviceMotionFRM*  m_pWaitMotion[GC_CHAR_NUM * NUM_JOB_LEVEL];
//    GCDeviceMotionFRM*  m_pActionMotion[GC_CHAR_NUM * NUM_JOB_LEVEL];
//    int                 m_iMotionCount;
//    float               m_fZoom;
//    float               m_fCharX;
//
//    bool                m_bAngleChanged;
//    bool                m_bMouseCatchStart;
//
//    void                ProcessCharWndAngle();
//    POINT               m_ptStartMouse;
//    float               m_fStartAngle;
//    D3DXMATRIX          m_matProject;
//};
//
//DEFINE_WND_FACTORY( KGCCharViewWnd );
//DECLARE_WND_FACTORY( KGCCharViewWnd );
//DECLARE_WND_FACTORY_NAME( KGCCharViewWnd );
//
//#endif // _KGCCHARWIEWWND_H_
