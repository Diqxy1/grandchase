#ifndef _GCPETWND_H_
#define _GCPETWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DSizingBox.h"

class KD3DStatic;
class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KPet;
class KGCPetMessage;

class KGCPetWnd : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCPetWnd );
	/// Default constructor
	KGCPetWnd( void );
	/// Default destructor
	virtual ~KGCPetWnd( void );
	/// Copy constructor
	KGCPetWnd( const KGCPetWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPetWnd& operator=( const KGCPetWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum
	{
		MAX_PET_NAME_LEN   = 8,
		VIEWPORT_OFFSET_X   = 8,
		VIEWPORT_OFFSET_Y   = 8,
		VIEWPORT_WIDTH      = 190,
		VIEWPORT_HEIGHT     = 220,
	};

public:
	void ChangeCharWndZoom( bool _bZoomUp );

	virtual void ActionPerformed( const KActionEvent& event );
	void ResetUIforResolution();
protected:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
	virtual void OnActivate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );
	

protected:
	D3DVIEWPORT9        m_viewport;
	float               m_fAngleMove;
	KD3DWnd*			m_pkExpBack;
	KD3DWnd*			m_pkExp;
	KD3DWnd*            m_pkInit;
    KD3DWnd*            m_pkUnEquip;
	KD3DStatic*			m_pkExpPercentStatic;	

	KD3DWnd*			m_pkMagicStoneBtn;

	bool                m_bShowInfo;
	int					m_iExtraCharNum;

	GCDeviceMotionFRM*  m_pShopMotion;


    KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM> m_pWaitMotion;
	KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM> m_pActionMotion;

    int					m_iMotionCount;
	float				m_fZoom;

	bool				m_bAngleChanged;
	bool				m_bMouseCatchStart;

	void				ProcessCharWndAngle();
	POINT				m_ptStartMouse;
	float               m_fStartAngle;

	KD3DWnd*            m_pkViewPortRect;

public:
	KPet*				m_pkPet;
	KGCPetMessage*		m_pkPetMsg;
    D3DXVECTOR3         m_vPetAxisCorrection;

	void SetPetExp( bool bRender = true );
};

DEFINE_WND_FACTORY( KGCPetWnd );
DECLARE_WND_FACTORY( KGCPetWnd );
DECLARE_WND_FACTORY_NAME( KGCPetWnd );

#endif // _GCPETWND_H_
