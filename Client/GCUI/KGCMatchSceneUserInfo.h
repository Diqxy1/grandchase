#ifndef _KGCMATCHSCENEUSERINFO_H_
#define _KGCMATCHSCENEUSERINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KPet;
class KGCCountryGroup;

// 클래스 이름은 UerInfo인데.. 정보는 안보여주고 그냥 캐릭터만 보여주는 클래스임;;
class KGCMatchSceneUserInfo : public KD3DWnd,          // extends
                              public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMatchSceneUserInfo );
    /// Default constructor
    KGCMatchSceneUserInfo( void );
    /// Default destructor
    virtual ~KGCMatchSceneUserInfo( void );
    /// Copy constructor
    KGCMatchSceneUserInfo( const KGCMatchSceneUserInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMatchSceneUserInfo& operator=( const KGCMatchSceneUserInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        VIEWPORT_OFFSET_X   = 6,
        VIEWPORT_OFFSET_Y   = 3,
        VIEWPORT_WIDTH      = 280,
        VIEWPORT_HEIGHT     = 285,
    };

public:
    void ChangeCharWndZoom( bool _bZoomUp );
    virtual void OnInitialize();
	void ResetUIforResolution();
    //====================================================================================
    // 2007-06-20 오후 8:52:20 : 우동완
    // 사실 RenderManager의 Render 함수가 각각의 Object들을 따로 그릴수 있게 되어서 이제 이런 부분은 필요 없지만... 
    // 오늘은 마나가 다 되어 수정하기가 귀찮네요;
    // 나중에 고치께요. 누군가 Claim 들어오면... -_-;
    void UpdateMeshObject( bool bRenderRoom = true, bool bRenderPet = true );
    void DeleteMeshObject();
    KPet*               m_pkPet;
    GCObject*           m_pMeshKOGRoom;

    virtual void ActionPerformed( const KActionEvent& event );

    void SetProjectionMatrix( int iOffsetX, int iOffsetY, int iWidth, int iHeight );

    void ReloadMotion();


protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void OnActivate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

    void SetChar(int iCharType);
    void UpdateTransMatrix();

protected:
    
    D3DVIEWPORT9        m_viewport;
    float               m_fAngleMove;

    KD3DWnd*            m_pkLeft;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkName;

    KGCCountryGroup*    m_pkCountryGroup;

    int                 m_iExtraCharNum;

    GCDeviceMotionFRM*  m_pShopMotion;

    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;

    int                 m_iMotionCount;
    float               m_fZoom;
    float               m_fCharX;

    bool                m_bAngleChanged;
    bool                m_bMouseCatchStart;

    void                ProcessCharWndAngle();
	POINT               m_ptStartMouse;
    float               m_fStartAngle;
    D3DXMATRIX          m_matProject;
    UCHAR               m_ucFrame;
    bool                m_bKOGRoom;    
    D3DXMATRIX          m_matTrans;
    bool                m_bRenderPet;

};

DEFINE_WND_FACTORY( KGCMatchSceneUserInfo );
DECLARE_WND_FACTORY( KGCMatchSceneUserInfo );
DECLARE_WND_FACTORY_NAME( KGCMatchSceneUserInfo );

#endif // _KGCMATCHSCENEUSERINFO_H_
