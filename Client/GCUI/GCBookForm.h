#pragma once

class KGCBookForm : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBookForm );
    /// Default constructor
    KGCBookForm( void );
    /// Default destructor
    virtual ~KGCBookForm( void );
    /// Copy constructor
    KGCBookForm( const KGCBookForm& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBookForm& operator=( const KGCBookForm& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreate( void );

private:
    KD3DWnd*            m_pkBackStyle;
    KD3DWnd*            m_pkBtnPrev;
    KD3DWnd*            m_pkBtnNext;
    KD3DWnd*            m_pkBtnClose;

    KD3DSizingBox*      m_pkBackPage;

    KD3DStatic*         m_pkStaticPage;
    KD3DStatic*         m_pkFormAStatic;
    KD3DStatic*         m_pkFormBStatic;
    KD3DImageIcon*      m_pkFormBImageIcon;

    bool                m_bAllShowMode;
    bool                m_bStartAnimMent;
    int                 m_iCurrentPage;

    std::wstring        m_strCurrentMent;
    std::wstring        m_strMentAnim;

    std::vector< KBookData > m_vecBookData;

public:
    void OnPrev();
    void OnNext();
    void OnClose();

    void ShowBookPage( int strDesc_, int strAudio_, std::wstring& strFileName_ );
    void ShowBookAllPage();
    void SetBookFormData( const std::vector< KBookData >& vecBookData_ );
    void SetAllShowMode( bool bAllShow_ ) { m_bAllShowMode = bAllShow_; }    

    void RefreshMent();
    void SetCurrentPage(int iCurrentPage_ );

    void SetBackStyle( int iBackStyle_ );

};

DEFINE_WND_FACTORY( KGCBookForm );
DECLARE_WND_FACTORY( KGCBookForm );
DECLARE_WND_FACTORY_NAME( KGCBookForm );
