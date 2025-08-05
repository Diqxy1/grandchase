#ifndef _GCPETINFOBOX_H_
#define _GCPETINFOBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KD3DStatic;

class KGCPetInfoBox : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCPetInfoBox );
	/// Default constructor
	KGCPetInfoBox( void );
	/// Default destructor
	virtual ~KGCPetInfoBox( void );
	/// Copy constructor
	KGCPetInfoBox( const KGCPetInfoBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPetInfoBox& operator=( const KGCPetInfoBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void NoMoreFoodAnim();
    void OneFrameMove() { return FrameMoveInEnabledState(); }

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	void PetFeeding( int iSatiation );

	void SetHungryRate( int iSatiation );
	void FrameMoveHungryRate();
	std::wstring GetPetState( int iSatiation );

	void ShowPetInfo( void );

protected:
	GCITEMUID		m_dwPetUID;
	KD3DStatic*		m_pkStaticPetName;
	KD3DStatic*		m_pkStaticPetLevel;
	KD3DStatic*		m_pkStaticPetHungryRate;
	KD3DStatic*		m_pkStaticSatiation;
	
	KD3DSizingBox*	m_pkPetWhiteGauge;	

	KSafeArray<KD3DWnd*,5> m_apkPetFood;

	KD3DStatic*		m_pkStaticAK;
	KD3DStatic*		m_pkStaticDF;
	KD3DStatic*		m_pkStaticVT;


private:
	int				m_iFoodCount;
	int				m_iSatiation;
	int				m_iOldSatiation;
	int				m_iProcessSatiation;

public:
	bool			m_bChange;
};

DEFINE_WND_FACTORY( KGCPetInfoBox );
DECLARE_WND_FACTORY( KGCPetInfoBox );
DECLARE_WND_FACTORY_NAME( KGCPetInfoBox );

#endif // _GCPETINFOBOX_H_
