#pragma once

class KGCInviteAgitSellBox: public KD3DWnd,
                        public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCInviteAgitSellBox );
    /// Default constructor
    KGCInviteAgitSellBox( void );
    /// Default destructor
    virtual ~KGCInviteAgitSellBox( void );
    /// Copy constructor
    KGCInviteAgitSellBox( const KGCInviteAgitSellBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInviteAgitSellBox& operator=( const KGCInviteAgitSellBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DStatic*     m_pkStaticDesc;
    KD3DWnd*        m_pkBtnOK;
    KD3DWnd*        m_pkBtnCancel;
    KD3DWnd*        m_pkBtnClose;
    KGCItemImgWnd*  m_pkSellItemImg;

    std::pair<GCITEMID, GCITEMUID> m_prItemInfo;

public:    
    void OnClose();
    void OnSell();
    void SetSellItem( std::pair<GCITEMID, GCITEMUID> prItemInfo_ );

};

DEFINE_WND_FACTORY( KGCInviteAgitSellBox );
DECLARE_WND_FACTORY( KGCInviteAgitSellBox );
DECLARE_WND_FACTORY_NAME( KGCInviteAgitSellBox );