#pragma once

class KGCRecvLetterSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRecvLetterSlot );
    /// Default constructor
    KGCRecvLetterSlot( void );
    /// Default destructor
    virtual ~KGCRecvLetterSlot( void );
    /// Copy constructor
    KGCRecvLetterSlot( const KGCRecvLetterSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRecvLetterSlot& operator=( const KGCRecvLetterSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DSizingBox* m_pkFrame;
    KD3DSizingBox* m_pkActiveFrame;
    KGCItemImgWnd* m_pkItemImg;

    KD3DWnd*    m_pkIconBeforeRead;
    KD3DWnd*    m_pkIconAfterRead;
    KD3DWnd*    m_pkIconSystem;
    KD3DWnd*    m_pkIconNormal;

    KD3DWnd*    m_pkBtnCheckDefault;
    KD3DWnd*    m_pkBtnCheckActive;

    KD3DStatic* m_pkStaticDate;
    KD3DStatic* m_pkStaticTitle;
    KD3DStatic* m_pkStaticGoods;
    KD3DStatic* m_pkStaticRemain;

    KPostItemTitleInfo  m_LetterInfo;
    bool m_bRead;

public:
    void OnClickCheck( void );
    void OnClickSlot( void );
    
    bool IsCheck( void );
    void SetCheck( bool bCheck_ );

    void SetLetterInfo( KPostItemTitleInfo& Info_ );
    void Clear( void );

    POSTUID GetPostUID( void ) { return m_LetterInfo.m_PostUID; }
    bool IsWithItem( void );
    
    void SetReadMail( bool bRead );
    bool GetReadMail();
};

DEFINE_WND_FACTORY( KGCRecvLetterSlot );
DECLARE_WND_FACTORY( KGCRecvLetterSlot );
DECLARE_WND_FACTORY_NAME( KGCRecvLetterSlot );
