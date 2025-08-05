#pragma once

class KGCEscortMonHPBar : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEscortMonHPBar );
    /// Default constructor
    KGCEscortMonHPBar( void );
    /// Default destructor
    virtual ~KGCEscortMonHPBar( void );
    /// Copy constructor
    KGCEscortMonHPBar( const KGCEscortMonHPBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEscortMonHPBar& operator=( const KGCEscortMonHPBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    enum { MIN_BAR = 0, MAX_BAR = 129, WARNING_TIME = 275 };
    enum
    {
        IMG_0 = 0,
        IMG_1,
        MAX_IMG,
    };

private:
    float           m_fCurHP;
    float           m_fMaxHP;
    
    int             m_iTimer;
    int             m_iAlpha;
    float           m_fAlphaChange;

    int             m_iSelectImg;

    KSafeArray<KD3DWnd*,MAX_IMG>m_apkMonIcon;
    KD3DSizingBox*  m_pkBarBack;
    KD3DSizingBox*  m_pkHPBar;

public:
    void ShowHPBar( bool bShow_ );
    void SetHPBar( float fMaxHP_, float fCurHP_ );
    void SetWarning( void );
    void ClearHPBar( void );
    void SelectImg( int iImgIndex );
};

DEFINE_WND_FACTORY( KGCEscortMonHPBar );
DECLARE_WND_FACTORY( KGCEscortMonHPBar );
DECLARE_WND_FACTORY_NAME( KGCEscortMonHPBar );
