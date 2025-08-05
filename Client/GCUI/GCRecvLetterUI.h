#pragma once

class KGCRecvLetterUI : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRecvLetterUI );
    /// Default constructor
    KGCRecvLetterUI( void );
    /// Default destructor
    virtual ~KGCRecvLetterUI( void );
    /// Copy constructor
    KGCRecvLetterUI( const KGCRecvLetterUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRecvLetterUI& operator=( const KGCRecvLetterUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum { NUM_GOODS_SLOT = 5 };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DStatic* m_pkStaticDate;
    KD3DStatic* m_pkStaticFrom;
    KD3DStatic* m_pkStaticTitle;
    KD3DStatic* m_pkStaticGoods;
    KD3DStatic* m_pkStaticGP;

    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnReceive;
    KD3DWnd*    m_pkBtnSend;
    KD3DWnd*    m_pkBtnDelete;

    KD3DListBox*    m_pkListLetter;
    KSafeArray< KGCItemImgWnd*, NUM_GOODS_SLOT >    m_apkGoods;

    KD3DScrollbar*  m_pkScroll;
    int             m_iScrollTopIndex;

    KPostItemInfo m_LetterInfo;
    
    bool          m_bMouseBound;
    int           m_iBoundIndex;

private:
    void OnClickGoodsRecv( void );
    void OnClickSend( void );
    void OnClickDelete( void );
    
    void OnScroll( void );
    void ResetScroll( void );
    
    void SetMouseBound( bool bSet_, int iIndex_ = -1 );
    void ShowItemInfo( void );

    void ClearUI( void );
    void ClearInfo( void );

    void ConvertString( IN OUT std::wstring& strMsg_ );

public:
    void SetLetterInfo( POSTUID postUID_ );
    POSTUID GetPostUID( void ) { return m_LetterInfo.m_PostUID; }
    void Clear( void );

    void OnDelete( void );
    void OnClickClose( void );
};

DEFINE_WND_FACTORY( KGCRecvLetterUI );
DECLARE_WND_FACTORY( KGCRecvLetterUI );
DECLARE_WND_FACTORY_NAME( KGCRecvLetterUI );
