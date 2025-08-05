#pragma once
#include "stdafx.h"

class KD3DWnd;
class KD3DView;
class KGCPetCostumeWnd;
class KPet;
class KGCPetCostumeInven : public KD3DDialog
	, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCPetCostumeInven );
	/// Default constructor
	KGCPetCostumeInven( void );
	/// Default destructor
	virtual ~KGCPetCostumeInven( void );
	/// Copy constructor
	KGCPetCostumeInven( const KGCPetCostumeInven& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPetCostumeInven& operator=( const KGCPetCostumeInven& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GCPCI_SLOT_NUM = 4,
	};

	enum{
		GCPCI_BTN_LEFT = 0,
		GCPCI_BTN_RIGHT = 1,
		GCPCI_BTN_NUM,
	};

public:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );


	void OnClose();
	void OnClickPageMove( bool bRight );

private:
    void ProcessCharWndAngle();
    void ChangeCharWndZoom( bool bZoom );
	void SetPetModel( int iPetID, int iPetPromotion, DWORD dwCostume );
    void SetSlotInfo();
private:
	KSafeArray<KGCPetCostumeWnd*,GCPCI_SLOT_NUM> m_apkPetItemSlot;
	KSafeArray<KD3DWnd*,GCPCI_BTN_NUM> m_apkPageBtn;
	KD3DWnd*		m_pkClose;
	KD3DView*		m_pkPreview;
	KD3DStatic*		m_pkHowToGetPetCostume;

    D3DVIEWPORT9    m_viewport;
    float           m_fZoom;
    float           m_fAngleMove;
    float           m_fStartAngle;
    POINT			m_ptStartMouse;
    D3DXVECTOR3     m_vPetAxisCorrection;

    KPet*           m_pkPet;

    bool            m_bAngleChanged;

    DWORD   m_dwPetItemID;
	DWORD	m_dwPetCostume;
	int     m_iFirstSlotIndex;
    int     m_iPromotionLv;
	std::vector<DWORD> m_vecCostumeList;
};

DEFINE_WND_FACTORY( KGCPetCostumeInven );
DECLARE_WND_FACTORY( KGCPetCostumeInven );
DECLARE_WND_FACTORY_NAME( KGCPetCostumeInven );
