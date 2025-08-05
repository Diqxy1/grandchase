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
	//현재 미리보기에 셋팅되어 있는 펫의 정보입니다.
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
	// 미리보기 창에 보일 펫 아이디를 셋팅합니다.
	// 여기서 셋팅되는 아이디에 따라서 리소스를 관리합니다.
	void SetNowPet( int iPetID_, int iPetEvolutionLevel );

	// 펫에 아이템을 장착시킬 수 있도록 하는 부분을 미리 고려해서 만들어 둡니다.
	void EquipPetItem( DWORD dwPetItemID_ ) {}

	void ChangeCharWndZoom( bool _bZoomUp );
	void ProcessCharWndAngle();

	void SetShopPreview();		//샵에서는 구입버튼과 취소 버튼이 활성화 됩니다.
	void SetMyInfoPreview();	//인벤토리에서는 확인버튼만 활성화 됩니다.
	void SetGachaPreview();

	void SetDescState( int iDescState_ )	{ m_iPetDescState = iDescState_; }	//여기에 설정되는 상태는 PetTemplate 에서 확인하시오.

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

	//배경은 케릭터의 배경과 동일한 배경을 사용하였습니다.
	//GCObject*			m_pMeshKOGRoom;

	//펫은 미리보기 화면에서 가만히 있다가 뭔가 행동을 하는 두가지 움직임을 보이도록 했습니다.
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