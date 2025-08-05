#pragma once
#include "stdafx.h"

class KD3DWnd;
class KGCItemImgWnd;
class KGCPetCostumeWnd : public KD3DWnd
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCPetCostumeWnd );
	/// Default constructor
	KGCPetCostumeWnd( void );
	/// Default destructor
	virtual ~KGCPetCostumeWnd( void );
	/// Copy constructor
	KGCPetCostumeWnd( const KGCPetCostumeWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPetCostumeWnd& operator=( const KGCPetCostumeWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void SetItemID( DWORD dwItemID, bool bActive = true );
	void SetBaseItemID( DWORD dwItemID, int iEvolutionLv );
	void HideImage();
	void ShowImage();
	void OnClick();

private:
	void OnClickBuy();
	void OnClickEquip();
	void OnClickUnEquip();

private:
	int				m_iEvolutionLv;
	DWORD			m_dwBaseItemID;
	DWORD			m_dwItemID;

	KD3DWnd*		m_pkBuy;
	KD3DWnd*		m_pkEquip;
	KD3DWnd*		m_pkUnequip;
	KD3DSizingBoxBtn*	m_pkBack;
	KGCItemImgWnd*	m_pkItemImg;
	bool			m_bBuybtnClick;
};

DEFINE_WND_FACTORY( KGCPetCostumeWnd );
DECLARE_WND_FACTORY( KGCPetCostumeWnd );
DECLARE_WND_FACTORY_NAME( KGCPetCostumeWnd );
