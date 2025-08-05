#pragma once
#include "GCItemImgWnd.h"

class KGCItemImgWnd;

class KGCGuildItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGuildItemBox );
    /// Default constructor
    KGCGuildItemBox( void );
    /// Default destructor
    virtual ~KGCGuildItemBox( void );
    /// Copy constructor
    KGCGuildItemBox( const KGCGuildItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildItemBox& operator=( const KGCGuildItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );


    void OnBuyButton(); 

    void SetItemBox(KManufactureItem* pManufacture_, int iTabIndex_);

    void ClearBox();

    void ToggleDisPlay(bool bToggle_);

    void ToggleName(bool bToggle_);

    bool IsDisPlay()    { return m_bDisplay; }

    KManufactureItem* GetManufactureItem()  { return m_pkManufacture; }


    int m_iBuyIndex;

private:
    KD3DWnd*            m_pkBtnBuy;

    KD3DStatic*         m_pkStaticItemnum;
    KD3DStatic*         m_pkStaticItemPrice;

    KD3DWnd*            m_pkImageCoin;
    KD3DWnd*            m_pkImageGP;

    KGCItemImgWnd*      m_pkItemImage;
    KManufactureItem*   m_pkManufacture;

    int                 m_iTabIndex;
    bool                m_bDisplay;
};

DEFINE_WND_FACTORY( KGCGuildItemBox );
DECLARE_WND_FACTORY( KGCGuildItemBox );
DECLARE_WND_FACTORY_NAME( KGCGuildItemBox );
