#pragma once

class KGCGwcRewardCatalogDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGwcRewardCatalogDlg );
    /// Default constructor
    KGCGwcRewardCatalogDlg( void );
    /// Default destructor
    virtual ~KGCGwcRewardCatalogDlg( void );
    /// Copy constructor
    KGCGwcRewardCatalogDlg( const KGCGwcRewardCatalogDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGwcRewardCatalogDlg& operator=( const KGCGwcRewardCatalogDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void PostChildDraw();

public:
    void OnClickClose();
    void SetItemImage();
public:
    KD3DWnd* m_pkExit;
    GCDeviceTexture* m_pkItemImage;
};

DEFINE_WND_FACTORY( KGCGwcRewardCatalogDlg );
DECLARE_WND_FACTORY( KGCGwcRewardCatalogDlg );
DECLARE_WND_FACTORY_NAME( KGCGwcRewardCatalogDlg );
