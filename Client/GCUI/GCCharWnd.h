#ifndef _GCCHARWND_H_
#define _GCCHARWND_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../define.h"

class KD3DStatic;
class KGCQuestGauge;
class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KPet;

class KGCCharWnd : public KD3DWnd,          // extends
                   public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharWnd );
    /// Default constructor
    KGCCharWnd( void );
    /// Default destructor
    virtual ~KGCCharWnd( void );
    /// Copy constructor
    KGCCharWnd( const KGCCharWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharWnd& operator=( const KGCCharWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

public:
    void ChangeCharWndZoom( bool _bZoomUp );
    void OnInitialize();
	void ResetUIforResolution();
    //====================================================================================
    // 2007-06-20 ���� 8:52:20 : �쵿��
    // ��� RenderManager�� Render �Լ��� ������ Object���� ���� �׸��� �ְ� �Ǿ ���� �̷� �κ��� �ʿ� ������... 
    // ������ ������ �� �Ǿ� �����ϱⰡ �����׿�;
    // ���߿� ��ġ����. ������ Claim ������... -_-;
    void UpdateMeshObject();
    void DeleteMeshObject();
    void UpdatePet();
	KPet*				m_pkPet;
    GCObject*			m_pMeshKOGRoom;
    void SetTitleItemID( int iTitleItemID );

    virtual void ActionPerformed( const KActionEvent& event );
	int GetExtraNum( void ) { return m_iExtraCharNum; }
	void SetChar(int iCharType);
	void UpdateCharacterSelect();

    bool isRoomFrozenRotation;
    void FreezeRoomRotation(bool froze) { isRoomFrozenRotation = true;  };


	void SetUseRoomObject(bool bUseRoomObject_)		{ m_bUseRoomObject = bUseRoomObject_; };

    void ReloadMotion();

    void SetZoom(float zoom);


	D3DVIEWPORT9        m_viewport;
protected:
    virtual void OnCreate( void );
	virtual void OnDestroy( void );
    virtual void OnActivate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
	void SetPlayerExp();    



protected:    
    
    float               m_fAngleMove;

	KD3DWnd*            m_pkBack_Color;
    KD3DWnd*            m_pkInit;
    KD3DWnd*            m_pkInfo;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkName;
	KD3DWnd*			m_pkExpBack;
	KD3DWnd*			m_pkExp;
	KD3DStatic*			m_pkExpPercentStatic;
    KGCQuestGauge*      m_pkQuestGauge;

    bool                m_bShowInfo;
	int					m_iExtraCharNum;

    KD3DStatic*         m_pkInfoDesc;

	GCDeviceMotionFRM*  m_pShopMotion;

    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;


	int					m_iMotionCount;
	float				m_fZoom;
	float				m_fCharX;

	bool				m_bAngleChanged;
	bool				m_bMouseCatchStart;

	void				ProcessCharWndAngle();
	POINT				m_ptStartMouse;
	float               m_fStartAngle;
    D3DXMATRIX          m_matProject;

    GCDeviceTexture*    m_pTexTitleItem;
    // ���� â�� �������� CharWnd�� �� �ִٸ� ������ �����~ 
    // g_MyD3D->m_TempPlayer.cFrame �������� ���� �����̴� ��
    int                 m_cFrame;

	bool				m_bUseRoomObject;
};

DEFINE_WND_FACTORY( KGCCharWnd );
DECLARE_WND_FACTORY( KGCCharWnd );
DECLARE_WND_FACTORY_NAME( KGCCharWnd );

#endif // _GCCHARWND_H_
