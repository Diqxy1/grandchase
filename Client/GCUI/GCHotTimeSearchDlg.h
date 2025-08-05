#pragma once

class KGCHotTimeSearchDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHotTimeSearchDlg );
    /// Default constructor
    KGCHotTimeSearchDlg( void );
    /// Default destructor
    virtual ~KGCHotTimeSearchDlg( void );
    /// Copy constructor
    KGCHotTimeSearchDlg( const KGCHotTimeSearchDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHotTimeSearchDlg& operator=( const KGCHotTimeSearchDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

public:
    void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID );    
    void UpdateInfo();
    void OnOK();
    void OnClose(); 

    KD3DStatic* m_pkStaticDesc;
    KD3DStatic* m_pkStaticTimeTitle;
    KD3DStatic* m_pkStaticTime;

    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnCancle;

    KGCItemImgWnd* m_pkEquipmentItemWnd;

    time_t      m_tEventTIme;
    GCITEMID    m_itemID;
};

DEFINE_WND_FACTORY( KGCHotTimeSearchDlg );
DECLARE_WND_FACTORY( KGCHotTimeSearchDlg );
DECLARE_WND_FACTORY_NAME( KGCHotTimeSearchDlg );
