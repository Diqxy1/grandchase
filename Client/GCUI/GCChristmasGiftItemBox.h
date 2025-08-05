#pragma once

class KGCChristmasGiftItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasGiftItemBox );
    /// Default constructor
    KGCChristmasGiftItemBox( void );
    /// Default destructor
    virtual ~KGCChristmasGiftItemBox( void );
    /// Copy constructor
    KGCChristmasGiftItemBox( const KGCChristmasGiftItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasGiftItemBox& operator=( const KGCChristmasGiftItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnSend;
    KGCUINumber*        m_pkPriceNum;
    KGCItemImgWnd*      m_pkGiftItemImgWnd;

    int                 m_nIndex;
    bool                m_bMouseBound;
    GCITEMID            m_dwItemID;
    DWORD               m_dwSledgeUseCount;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

    void SetGiftItemBoxInfo( GCITEMID dwItemID, DWORD dwSledgeUseCount );
    void OnClickSendGift();

private:
    void ShowItemInfo( void );
    void OnMouseEnter( void );
    void OnMouseLeave( void );
};

DEFINE_WND_FACTORY( KGCChristmasGiftItemBox );
DECLARE_WND_FACTORY( KGCChristmasGiftItemBox );
DECLARE_WND_FACTORY_NAME( KGCChristmasGiftItemBox );
