#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMantisReportDlg : public KD3DDialog,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCMantisReportDlg );
	/// Default constructor
	KGCMantisReportDlg( void );
	/// Default destructor
	virtual ~KGCMantisReportDlg( void );
	/// Copy constructor
	KGCMantisReportDlg( const KGCMantisReportDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMantisReportDlg& operator=( const KGCMantisReportDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
    

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );


private:
    KD3DStatic* m_pkStaticTitle;
    KD3DStatic* m_pkStaticProject;
    KD3DStatic* m_pkStaticID;
    KD3DStatic* m_pkStaticPasswd;
    KD3DStatic* m_pkStaticScreenShot;

    KD3DCheckBox* m_pkCheckBoxScreenShot;
    KGCMultiMenu* m_pkPopupMenuProject;

    KD3DEdit* m_pkEditID;
    KD3DEdit* m_pkEditPasswd;
    KD3DEdit* m_pkEditSummary;
    KD3DEdit* m_pkEditDesc;

    KD3DWnd* m_pkBtnOk;
    KD3DWnd* m_pkBtnCancel;
    KD3DWnd* m_pkBtnDropDownProject;

    std::map<int, std::wstring> m_mapProjectList;

    KMantisReportParm m_kMantisReportParm;

public:
    void OnCheck();
    void OnClose();
    void OnPopUpMenu();
    void OnConfirm();
    void InitProjectList();
    void TakeScreenShot();

};

DEFINE_WND_FACTORY( KGCMantisReportDlg );
DECLARE_WND_FACTORY( KGCMantisReportDlg );
DECLARE_WND_FACTORY_NAME( KGCMantisReportDlg );

