#pragma once

class KGCRitasProgressBar : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasProgressBar );
    /// Default constructor
    KGCRitasProgressBar( void );
    /// Default destructor
    virtual ~KGCRitasProgressBar( void );
    /// Copy constructor
    KGCRitasProgressBar( const KGCRitasProgressBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasProgressBar& operator=( const KGCRitasProgressBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void SetBlindWidth(DWORD dwWidth_ );
	void SetTime(float fTime_);
	void Init(D3DXVECTOR2 vPos_ , DWORD dwWidth_ );
private:
	KD3DWnd*	m_pkProgressBarBlinder;
	KD3DStatic*	m_pkStaticTime;
};

DEFINE_WND_FACTORY( KGCRitasProgressBar );
DECLARE_WND_FACTORY( KGCRitasProgressBar );
DECLARE_WND_FACTORY_NAME( KGCRitasProgressBar );
