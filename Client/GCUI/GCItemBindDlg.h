#pragma once

class KGCItemBindDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemBindDlg );
    /// Default constructor
    KGCItemBindDlg( void );
    /// Default destructor
    virtual ~KGCItemBindDlg( void );
    /// Copy constructor
    KGCItemBindDlg( const KGCItemBindDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemBindDlg& operator=( const KGCItemBindDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

private:
    KGCItemImgWnd*                          m_pkItemImg;
    KSafeArray< KD3DWnd*, GC_CHAR_NUM >     m_apkCharIcon;

    KD3DStatic* m_pkStaticItemName;
    KD3DStatic* m_pkStaticCharName;

    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnCancle;
    KD3DWnd*    m_pkConfirmPopup;

    GCITEMUID   m_itemUID;
    char        m_cCharType;

    KItem*      m_pInvenItem;
    GCItem*     m_pItemData;

private:
    void OnClickClose( void );
    void OnClickOK( void );
    void OnClickCancle( void );

    void EnterMouseCursorOKBtn( void );
    void LeaveMouseCursorOKBtn( void );

    void SetCharacterIcon( void );
    void SetItemImg( void );
    void SetStatics( void );
    void InitDlgInfo( void );

    void BindPetItem( void );

};

DEFINE_WND_FACTORY( KGCItemBindDlg );
DECLARE_WND_FACTORY( KGCItemBindDlg );
DECLARE_WND_FACTORY_NAME( KGCItemBindDlg );
