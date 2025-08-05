#pragma once

class KGCBookForm;

class KGCEclipseOldBooks : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEclipseOldBooks );
    /// Default constructor
    KGCEclipseOldBooks( void );
    /// Default destructor
    virtual ~KGCEclipseOldBooks( void );
    /// Copy constructor
    KGCEclipseOldBooks( const KGCEclipseOldBooks& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEclipseOldBooks& operator=( const KGCEclipseOldBooks& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        MAX_OLD_BOOKS_NUM = 9,
        PARTICLE_NUM = 2,
    };

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KSafeArray< KD3DWnd*, MAX_OLD_BOOKS_NUM > m_pkOldBooks;
    KD3DWnd*            m_pkBtnCombination;
    KD3DWnd*            m_pkBtnClose;
    KD3DWnd*            m_pkBtnReadBook;

    KD3DImageIcon*      m_pkAllAcquiredOldBook;    
    
    KD3DStatic*         m_pkStaticDesc;    

    KGCBookForm*        m_pkBookForm;
    KGCItemImgWnd*      m_pkImgRewardItem;
    
    std::map< int, KBookData > m_mapOldBooksData;
    std::set< GCITEMID > m_setItemID;
    GCITEMID            m_dwRewardItemID;

    bool                m_bAllAcquired;

    int                 m_iParticleLife;

    KSafeArray< CParticleEventSeqPTR, PARTICLE_NUM > m_pParticle;

public:
    void OnClose();
    void OnCombination();

    void LoadOldBookData();
    void CheckAcquiredOldBooks();    
    void ShowCombinationEffect();

    void ShowCompetionEffect( bool bShow_ );
    void SetCompetionEffect();

    void SetItemIDList( std::set< GCITEMID >& setItemID_ ) { m_setItemID =  setItemID_; }
    void SetAllAcquiredOldBooks( bool bCheck_ ) { m_bAllAcquired = bCheck_; }
    void SetRewardItemInfo( std::vector<KDropItemInfo>& vecRewardItem_ );        
};

DEFINE_WND_FACTORY( KGCEclipseOldBooks );
DECLARE_WND_FACTORY( KGCEclipseOldBooks );
DECLARE_WND_FACTORY_NAME( KGCEclipseOldBooks );
