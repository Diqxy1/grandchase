#pragma once

class KGCLevelupInfoBox : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCLevelupInfoBox );
    /// Default constructor
    KGCLevelupInfoBox( void );
    /// Default destructor
    virtual ~KGCLevelupInfoBox( void );
    /// Copy constructor
    KGCLevelupInfoBox( const KGCLevelupInfoBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLevelupInfoBox& operator=( const KGCLevelupInfoBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void InitInfo( void );
    void OnLevelup( void );
    void FrameMoveLevelupInfo( void );

private:
    enum{ X_POS = 480, Y_POS = 292,  ON_TIME = 2000 };

    KD3DWnd*    m_pkBackground;
    D3DXVECTOR2 m_dxvLevelPos;
    D3DCOLOR    m_dwColor;
    int         m_iLevel;
    DWORD       m_dwTimer;
    bool        m_bShow;
};

DEFINE_WND_FACTORY( KGCLevelupInfoBox );
DECLARE_WND_FACTORY( KGCLevelupInfoBox );
DECLARE_WND_FACTORY_NAME( KGCLevelupInfoBox );
