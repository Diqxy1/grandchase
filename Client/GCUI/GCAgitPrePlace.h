#pragma once

class KGCAgitPrePlace : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitPrePlace );
    /// Default constructor
    KGCAgitPrePlace( void );
    /// Default destructor
    virtual ~KGCAgitPrePlace( void );
    /// Copy constructor
    KGCAgitPrePlace( const KGCAgitPrePlace& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitPrePlace& operator=( const KGCAgitPrePlace& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

    void SetPrePlaceInfo( GCItem* pItem_, int iMoneyType_, int iPrice_, PAIR_USHORT_DWORD pairIndex_ = std::pair<USHORT, DWORD>(0, 0) );
    void UpdateData();
    void Clear( void );
    void Close();

private:
    void OnBuyBtn( void );
    void OnCancleBtn( void );

    void StartDrag();

private:
    KD3DWnd*    m_pkBtnBuy;
    KD3DWnd*    m_pkBtnCancle;
    KD3DStatic* m_pkStaticItemName;
    KD3DStatic* m_pkStaticPrice;
    GCDeviceTexture* m_pkMoneyTex;
    KGCItemImgWnd*  m_pkItemImg;
    KD3DWnd*            m_pkItemFrame;

    PAIR_USHORT_DWORD m_PairIndex;
    GCItem* m_pItemData;
    int     m_iMoneyType;
    int     m_iPrice;
    bool    m_bObjectPick;
};

DEFINE_WND_FACTORY( KGCAgitPrePlace );
DECLARE_WND_FACTORY( KGCAgitPrePlace );
DECLARE_WND_FACTORY_NAME( KGCAgitPrePlace );
