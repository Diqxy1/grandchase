#pragma once

class KGCMsgShutDown : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMsgShutDown );
    /// Default constructor
    KGCMsgShutDown( void );
    /// Default destructor
    virtual ~KGCMsgShutDown( void );
    /// Copy constructor
    KGCMsgShutDown( const KGCMsgShutDown& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMsgShutDown& operator=( const KGCMsgShutDown& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroy( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );


    void OnCloseMsgBox();

protected:
    KD3DStatic* m_pkDescText;
    KD3DStatic* m_pkTimeText;    
    KD3DWnd*    m_pkOK;
    KD3DSizingBox*			m_pkBackDesc;


//  남은 시간 체크
    int         m_nRemainTime;
    int         m_nShutdownType;
    DWORD       m_dwBeforeTick;


    void SetTimeText();


};

DEFINE_WND_FACTORY( KGCMsgShutDown );
DECLARE_WND_FACTORY( KGCMsgShutDown );
DECLARE_WND_FACTORY_NAME( KGCMsgShutDown );
