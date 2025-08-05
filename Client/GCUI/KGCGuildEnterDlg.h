#pragma once

class KGCGuildEnterDlg :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildEnterDlg );
    /// Default constructor
    KGCGuildEnterDlg( void );
    /// Default destructor
    virtual ~KGCGuildEnterDlg( void );
    /// Copy constructor
    KGCGuildEnterDlg( const KGCGuildEnterDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildEnterDlg& operator=( const KGCGuildEnterDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*            m_pkGuildInfoBtn;
    KD3DWnd*            m_pkGuildIWarBtn;
    KD3DWnd*            m_pkGuildISquareBtn;
    KD3DWnd*            m_pkCloseBtn;
    
    

public:
    void OnClickGuildWarBtn();
    void OnClickGuildquareBtn();
    void OnClose();
};

DEFINE_WND_FACTORY( KGCGuildEnterDlg );
DECLARE_WND_FACTORY( KGCGuildEnterDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildEnterDlg );