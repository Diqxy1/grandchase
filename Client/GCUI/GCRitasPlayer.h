#pragma once

class KGCRitasPlayer : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasPlayer );
    /// Default constructor
    KGCRitasPlayer( void );
    /// Default destructor
    virtual ~KGCRitasPlayer( void );
    /// Copy constructor
    KGCRitasPlayer( const KGCRitasPlayer& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasPlayer& operator=( const KGCRitasPlayer& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();

	void SetDirectionRight(bool bRight_);

	DWORD GetBasketWidth();
	D3DXVECTOR2 GetBasketPos();	//{ return GetFixedWindowLocalPos() + m_pkCatchRect->GetFixedWindowLocalPos(); }
	void SetFixedWindowLocalBasketPos(const D3DXVECTOR2& vPos_);
	bool CheckCatchObj(const KGCRitasObj* pkObj_);

private:

	KD3DWnd*	m_pkCatchRectRight;
	KD3DWnd*	m_pkCatchRectLeft;

	KD3DWnd*	m_pkLimeRight;
	KD3DWnd*	m_pkLimeLeft;

	bool		m_bRight;

	D3DXVECTOR2 m_vRecPosRight;
	D3DXVECTOR2 m_vRecPosLeft;


	int			m_iAniFrame;

	KD3DWnd*	m_pkLimePt;
	KD3DWnd*	m_pkCatchRectPt;

};

DEFINE_WND_FACTORY( KGCRitasPlayer );
DECLARE_WND_FACTORY( KGCRitasPlayer );
DECLARE_WND_FACTORY_NAME( KGCRitasPlayer );
