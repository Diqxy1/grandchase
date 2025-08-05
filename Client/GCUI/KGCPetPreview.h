#ifndef _KGCPETPREVIEW_H_
#define _KGCPETPREVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KGCPetMessage;
class KPet;

class KGCPetPreview: public KD3DDialog,
					 public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCPetPreview );

	KGCPetPreview(void);
	virtual ~KGCPetPreview(void);
	/// Copy constructor
	KGCPetPreview( const KGCPetPreview& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPetPreview& operator=( const KGCPetPreview& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	//���� �̸����⿡ ���õǾ� �ִ� ���� �����Դϴ�.
	DWORD				m_dwNowPet;	

public:
	enum
	{
		MAX_CHAR_NAME_LEN   = 8,
		VIEWPORT_OFFSET_X   = 20,
		VIEWPORT_OFFSET_Y   = 10,
		VIEWPORT_WIDTH      = 300,
		VIEWPORT_HEIGHT     = 310,
	};
	// �̸����� â�� ���� �� ���̵� �����մϴ�.
	// ���⼭ ���õǴ� ���̵� ���� ���ҽ��� �����մϴ�.
	void SetNowPet( int iPetID_, int iPetEvolutionLevel );

	// �꿡 �������� ������ų �� �ֵ��� �ϴ� �κ��� �̸� ����ؼ� ����� �Ӵϴ�.
	void EquipPetItem( DWORD dwPetItemID_ ) {}

	void ChangeCharWndZoom( bool _bZoomUp );
	void ProcessCharWndAngle();

	void SetShopPreview();		//�������� ���Թ�ư�� ��� ��ư�� Ȱ��ȭ �˴ϴ�.
	void SetMyInfoPreview();	//�κ��丮������ Ȯ�ι�ư�� Ȱ��ȭ �˴ϴ�.
	void SetGachaPreview();

	void SetDescState( int iDescState_ )	{ m_iPetDescState = iDescState_; }	//���⿡ �����Ǵ� ���´� PetTemplate ���� Ȯ���Ͻÿ�.

protected:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );	
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

	KD3DWnd*			m_pkOk;
	KD3DWnd*			m_pkClose;
	KD3DWnd*			m_pkBuy;
	KD3DWnd*			m_pkSlash;
	KD3DWnd*			m_pkCancle;
	KD3DStatic*			m_pkPetTitle;
	KD3DStatic*			m_pkPetName;
	KD3DStatic*			m_pkPetDesc;

public:
	KGCPetMessage*		m_pkPetMsg;

protected:
	
	D3DVIEWPORT9        m_viewport;
	float               m_fAngleMove;

	//����� �ɸ����� ���� ������ ����� ����Ͽ����ϴ�.
	//GCObject*			m_pMeshKOGRoom;

	//���� �̸����� ȭ�鿡�� ������ �ִٰ� ���� �ൿ�� �ϴ� �ΰ��� �������� ���̵��� �߽��ϴ�.
	GCDeviceMotionFRM*  m_pPetWaitMotion;
	GCDeviceMotionFRM*  m_pPetActionMotion;

    int					m_iMotionCount;
	float				m_fZoom;
	float				m_fCharX;

	bool				m_bAngleChanged;
	bool				m_bMouseCatchStart;
	
	POINT				m_ptStartMouse;
	float               m_fStartAngle;

	int					m_iPetDescState;

	KPet*				m_pkPet;
    D3DXVECTOR3         m_vPetAxisCorrection;
};

DEFINE_WND_FACTORY( KGCPetPreview );
DECLARE_WND_FACTORY( KGCPetPreview );
DECLARE_WND_FACTORY_NAME( KGCPetPreview );

#endif	//_KGCPETPREVIEW_H_