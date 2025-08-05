#pragma once

class KGCDungeonRankingClearData;
class KGCDungeonRankingPanel;

class KGCDungeonRankingDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonRankingDlg );
    /// Default constructor
    KGCDungeonRankingDlg( void );
    /// Default destructor
    virtual ~KGCDungeonRankingDlg( void );
    /// Copy constructor
    KGCDungeonRankingDlg( const KGCDungeonRankingDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonRankingDlg& operator=( const KGCDungeonRankingDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_TOOLTIP_NUM = 4,
    };

    enum
    {
        EM_CURRENT_RANK = 0,
        EM_OLD_RANK = 1,
        EM_SIZE_RANK = 2,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );

public:
    void UpdateBtnUI();
    void UpdateHelpUI( bool bRender );
    void UpdateRankingUI();

    void UpdateClearData();
    void UpdateRankingData();

    void LockUI( bool bLock );

    void SetPage( int nPage );

public:
    void OnClose();
    void OnHelp();
    void OnHelpClose();

    void OnCurRanking();
    void OnOldRanking();

public:
    int m_nState;

public:

    KD3DWnd* m_pkCloseBtn;
    KD3DWnd* m_pkHelpBtn;
    KD3DWnd* m_pkHelpOnBtn;

    KD3DWnd* m_pkCurRankignBtn;
    KD3DWnd* m_pkOldRankingBtn;

    KD3DWnd* m_pkCurRankImg;
    KD3DWnd* m_pkOldRankImg;

    KSafeArray<KD3DWnd*,EM_TOOLTIP_NUM> m_pkToolTip;
    KSafeArray<KD3DStatic*,EM_TOOLTIP_NUM> m_pkToolTipText;

    KD3DWnd*	m_pkEclipse;    // Help용 화면 가리개


    KGCDungeonRankingClearData* m_pkClearData;
    KGCDungeonRankingPanel*     m_pkRankingPanel;

};

DEFINE_WND_FACTORY( KGCDungeonRankingDlg );
DECLARE_WND_FACTORY( KGCDungeonRankingDlg );
DECLARE_WND_FACTORY_NAME( KGCDungeonRankingDlg );
