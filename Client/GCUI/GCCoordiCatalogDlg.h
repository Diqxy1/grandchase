#pragma once

class KGCCoordiCatalogDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiCatalogDlg );
    /// Default constructor
    KGCCoordiCatalogDlg( void );
    /// Default destructor
    virtual ~KGCCoordiCatalogDlg( void );
    /// Copy constructor
    KGCCoordiCatalogDlg( const KGCCoordiCatalogDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiCatalogDlg& operator=( const KGCCoordiCatalogDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    enum { PAGE_PREV = 0, PAGE_NEXT = 1, MAX_NUM_BTN = 10 };
    std::vector< std::string > m_vecCatalog;
    
    int         m_iCurrentPage;
    int         m_iTotalPage;
         
    KD3DWnd*    m_pkBtnExit;
    KD3DWnd*    m_pkBtnPrev;
    KD3DWnd*    m_pkBtnNext;
    KSafeArray<KD3DWnd*, MAX_NUM_BTN > m_apkCatalogBtn;
    GCDeviceTexture*    m_pkItemImage;

public:
    void SetCatalogInfo( std::vector< std::string>& vecCatalog );

private:
    void OnExit( void );
    void SetItemImage( int iIndex );
    void OnPageBtn( int iPageType );
    void OnNumBtn( int iSelectNum );
};

DEFINE_WND_FACTORY( KGCCoordiCatalogDlg );
DECLARE_WND_FACTORY( KGCCoordiCatalogDlg );
DECLARE_WND_FACTORY_NAME( KGCCoordiCatalogDlg );
