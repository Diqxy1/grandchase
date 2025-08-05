#pragma once

class KGCSongkranRewardDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSongkranRewardDlg );
    /// Default constructor
    KGCSongkranRewardDlg( void );
    /// Default destructor
    virtual ~KGCSongkranRewardDlg( void );
    /// Copy constructor
    KGCSongkranRewardDlg( const KGCSongkranRewardDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSongkranRewardDlg& operator=( const KGCSongkranRewardDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
	virtual void FrameMoveInEnabledState( void );

public:
	void OnClose();
	void SetText( int iItemID, int iItemSize );
	void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID );

public:
	int				m_iItemID;
	int				m_iItemSize;

	KD3DWnd*		m_pkBtnOK;
	KGCItemImgWnd*	m_pkRewardImg;
	KD3DStatic*		m_pkRewardDesc;

};

DEFINE_WND_FACTORY( KGCSongkranRewardDlg );
DECLARE_WND_FACTORY( KGCSongkranRewardDlg );
DECLARE_WND_FACTORY_NAME( KGCSongkranRewardDlg );
