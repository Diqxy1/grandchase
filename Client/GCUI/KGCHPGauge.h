#ifndef _GCHPGUAGE_H_
#define _GCHPGUAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GAUGE_BAR_NUM	5
#define MAX_GAUGE_HP	1500000.0f
#define EFFECT_TIME_GAP 2000 // 2000ms
#define DECREASE_VALUE 1 // pixel

class KGCHPGauge : public KD3DWnd // extends
{
public:
	DECLARE_CLASSNAME( KGCHPGauge );
	/// Default constructor
	KGCHPGauge( void );
	/// Default destructor
	virtual ~KGCHPGauge( void );
	/// Copy constructor
	KGCHPGauge( const KGCHPGauge& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHPGauge& operator=( const KGCHPGauge& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	enum
	{
		MAX_GAUGE_LENGTH = 482,
	};


	float       m_fNowHP;
	float       m_fMaxHP;

protected:
	virtual void OnCreate( void );
    
	void ModulateGauge( void );

public:
	void SetNowHP( float fNowHP );
	void SetMaxHP( float fMaxHP ); 
	void SetMonID( int iMonID);

protected:
	KD3DWnd*    m_pkGaugeBar[GAUGE_BAR_NUM];
    KD3DWnd*    m_pkHpEffect;

	int			m_iBarNum;
	DWORD		m_dwGaugeHP;
    float       m_fOldHp;
    int         m_iOldBarNum;
	int			m_iMonID;
    DWORD       m_dwTime;

	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

    void HpEffect( void );
	void MakeTopChild( KD3DWnd* pWnd );

};

DEFINE_WND_FACTORY( KGCHPGauge );
DECLARE_WND_FACTORY( KGCHPGauge );
DECLARE_WND_FACTORY_NAME( KGCHPGauge );

#endif // 
