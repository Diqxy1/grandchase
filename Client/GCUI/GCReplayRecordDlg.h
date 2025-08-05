#pragma once

class KGCReplayRecordCommonDlg;
class KGCReplayRecordAdvanceDlg;
class KGCReplayExplain;

class KGCReplayRecordDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayRecordDlg );
    /// Default constructor
    KGCReplayRecordDlg( void );
    /// Default destructor
    virtual ~KGCReplayRecordDlg( void );
    /// Copy constructor
    KGCReplayRecordDlg( const KGCReplayRecordDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayRecordDlg& operator=( const KGCReplayRecordDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    KGCReplayRecordCommonDlg*   m_pkReplayCommon;
    KGCReplayRecordAdvanceDlg*  m_pkReplayAdvance;
    KGCReplayExplain*           m_pkReplayExplain;

    KD3DWnd*    m_pkCommon;
    KD3DWnd*    m_pkAdvance;
    KD3DWnd*    m_pkExplain;
    KD3DWnd*    m_pkOnCommon;
    KD3DWnd*    m_pkOnAdvance;
    KD3DWnd*    m_pkOnExplain;

    KD3DWnd*    m_pkOK;
    KD3DWnd*    m_pkCancel;
    KD3DWnd*    m_pkCloseDlg;

public:
    void OnTabCommon( void );
    void OnTabAdvance( void );
    void OnTabExplain( void );

    void OnOK( void );
    void OnClose( void );
    void OnEscape( void );

    void SetAdvanceOption( int iResolutionIndex_, int iFrameIndex_ );
};

DEFINE_WND_FACTORY( KGCReplayRecordDlg );
DECLARE_WND_FACTORY( KGCReplayRecordDlg );
DECLARE_WND_FACTORY_NAME( KGCReplayRecordDlg );
