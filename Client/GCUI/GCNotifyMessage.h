#pragma once

class KGCNotifyMessage : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCNotifyMessage );
    /// Default constructor
    KGCNotifyMessage( void );
    /// Default destructor
    virtual ~KGCNotifyMessage( void );
    /// Copy constructor
    KGCNotifyMessage( const KGCNotifyMessage& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNotifyMessage& operator=( const KGCNotifyMessage& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );


public:
    KD3DWnd*					m_pkNewSkillAdd;
    KD3DStatic*					m_pkNewSkillAddMsg;

	int							m_iToggleFrame;


public:
    void ShowNewSkillAddMessage(bool bShow);
    void HideNewSkillAddMessage();
    void SetMsgColor(BYTE byteAlpha);
    
    bool IsRenderState();
};

DEFINE_WND_FACTORY( KGCNotifyMessage );
DECLARE_WND_FACTORY( KGCNotifyMessage );
DECLARE_WND_FACTORY_NAME( KGCNotifyMessage );
