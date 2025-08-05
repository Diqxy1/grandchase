#pragma once

class KGCCharNickNameChangeDlg;

class KGCCharNickNameChangeDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharNickNameChangeDlg );
    /// Default constructor
    KGCCharNickNameChangeDlg( void );
    /// Default destructor
    virtual ~KGCCharNickNameChangeDlg( void );
    /// Copy constructor
    KGCCharNickNameChangeDlg( const KGCCharNickNameChangeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharNickNameChangeDlg& operator=( const KGCCharNickNameChangeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


    enum {
        EVDEPOT_COMMONTAB               = 1,
        EVDEPOT_EXCLUSIVETAB            = 2,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroy();

public:
    void BackGroundEnable( bool bRender = true );

    void KeydownEditNick( void );
    void LoadDefaultValue( void );

    void Onclose( void );
    void SetNickChangeBtn( bool bRender );
    void OnChageConfirm( void );
    void OnClickEditNick( void );

    void SetMode( int iMode ) { m_iMode = iMode; }
    void SetCurrentType( int iCharType ) { m_iCurrentCharType = iCharType; }
    void SetItemUID ( GCITEMUID itemUID ) { m_itemUID = itemUID; }

private:
    KD3DWnd*            m_pkWhiteBackground;
    KD3DEdit*           m_pkEditNickname;
    KD3DStatic*         m_pkNicdiscrptionFirst;
    KD3DStatic*         m_pkNicdiscrptionSecond;
    KD3DWnd*            m_pkSpacialChar;
    KD3DWnd*            m_pkCussWord;

    KD3DWnd*            m_pkBtnConfirm;
    KD3DWnd*            m_pkBtnClose;

    int                 m_iMode;
    int                 m_iCurrentCharType;

    GCITEMUID           m_itemUID;

};

DEFINE_WND_FACTORY( KGCCharNickNameChangeDlg );
DECLARE_WND_FACTORY( KGCCharNickNameChangeDlg );
DECLARE_WND_FACTORY_NAME( KGCCharNickNameChangeDlg );