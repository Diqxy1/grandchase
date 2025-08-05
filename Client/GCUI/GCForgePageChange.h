#pragma once

class KGCForgePageChange : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCForgePageChange );
    /// Default constructor
    KGCForgePageChange( void );
    /// Default destructor
    virtual ~KGCForgePageChange( void );
    /// Copy constructor
    KGCForgePageChange( const KGCForgePageChange& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCForgePageChange& operator=( const KGCForgePageChange& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void OnClickPrePageBtn( const KActionEvent& event );
    void OnClickNextPageBtn( const KActionEvent& event );

    void SetMaxPage(int iMax);
    void SetPage(int iIndex);

    void RefreshPageNum();
private:
    KD3DWnd *m_pkPrePageBtn;
    KD3DWnd *m_pkNextPageBtn;

    KD3DStatic *m_pkStaticPage;

    int         m_iMaxPage;
    int         m_iCurrentPage;
};

DEFINE_WND_FACTORY( KGCForgePageChange );
DECLARE_WND_FACTORY( KGCForgePageChange );
DECLARE_WND_FACTORY_NAME( KGCForgePageChange );
