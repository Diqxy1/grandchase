#pragma once

class KGCEscortMerryDesc : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEscortMerryDesc );
    /// Default constructor
    KGCEscortMerryDesc( void );
    /// Default destructor
    virtual ~KGCEscortMerryDesc( void );
    /// Copy constructor
    KGCEscortMerryDesc( const KGCEscortMerryDesc& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEscortMerryDesc& operator=( const KGCEscortMerryDesc& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void ShowEscortMerryDesc( int iSrtingID_, int iShowFrame_ );

private:
    void OnHide( void );

private:
    int     m_iAlpha;
    int     m_iShowFrame;
    int     m_iFadeInFrame;
    int     m_iFadeOutFrame;
    float   m_fAlphaChange;

    KD3DWnd*    m_pkBack;
    KD3DWnd*    m_pkCharacter;
    KD3DStatic* m_pkStaticDesc;

};

DEFINE_WND_FACTORY( KGCEscortMerryDesc );
DECLARE_WND_FACTORY( KGCEscortMerryDesc );
DECLARE_WND_FACTORY_NAME( KGCEscortMerryDesc );
