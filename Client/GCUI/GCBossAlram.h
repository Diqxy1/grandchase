#pragma once

class KGCBossAlram : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBossAlram );
    /// Default constructor
    KGCBossAlram( void );
    /// Default destructor
    virtual ~KGCBossAlram( void );
    /// Copy constructor
    KGCBossAlram( const KGCBossAlram& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBossAlram& operator=( const KGCBossAlram& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void StartAlram( const int iBlinkFrame_, const int iBlinkCount_ );  // 깜빨거릴 프레임, 깜빡거릴 수 

private:
    enum 
    { 
        ALRAM_UI_CNT = 2, 
        ALRAM_ALPHA_MIN = 0,
        ALRAM_ALPHA_MAX = 255,
    };

    int m_iBlinkFrame;
    int m_iBlinkCount;
    int m_iCurrentFrame;
    int m_iDivAlpha;
    KSafeArray< KD3DWnd*, ALRAM_UI_CNT >   m_pkAlram;

    void SetAlramAlpha( int iAlpha_ );
    void ToggleRenderAlram( const bool bToggle_ );
};

DEFINE_WND_FACTORY( KGCBossAlram );
DECLARE_WND_FACTORY( KGCBossAlram );
DECLARE_WND_FACTORY_NAME( KGCBossAlram );
