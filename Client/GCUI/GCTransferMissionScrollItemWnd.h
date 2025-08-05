#pragma once

class KGCTransferMissionScrollItemWnd : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTransferMissionScrollItemWnd );
    /// Default constructor
    KGCTransferMissionScrollItemWnd( void );
    /// Default destructor
    virtual ~KGCTransferMissionScrollItemWnd( void );
    /// Copy constructor
    KGCTransferMissionScrollItemWnd( const KGCTransferMissionScrollItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTransferMissionScrollItemWnd& operator=( const KGCTransferMissionScrollItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnBuy;
    KD3DWnd*            m_pkIconGP;
    KD3DWnd*            m_pkIconCash;

    KGCItemImgWnd*      m_pkMissionItemImgWnd;

    KD3DStatic*         m_pkStaticItemName;
    KD3DStatic*         m_pkStaticItemPrice;
    KD3DStatic*         m_pkStaticItemDesc;

    //
    DWORD               m_dwItemID;


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void SetItemID( DWORD& dwItemID ) { m_dwItemID = dwItemID / 10; }
    void SetItemInfo();

    void OnClickBuy();
    void OnClickImgWnd();

};

DEFINE_WND_FACTORY( KGCTransferMissionScrollItemWnd );
DECLARE_WND_FACTORY( KGCTransferMissionScrollItemWnd );
DECLARE_WND_FACTORY_NAME( KGCTransferMissionScrollItemWnd );
