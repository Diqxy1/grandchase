#pragma once

class KGCSellInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSellInfo );
    /// Default constructor
    KGCSellInfo( void );
    /// Default destructor
    virtual ~KGCSellInfo( void );
    /// Copy constructor
    KGCSellInfo( const KGCSellInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSellInfo& operator=( const KGCSellInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void InitNumbers();
    void InitPriceList();
    void SetSellItemNum( int iNum_ );
    void AddReward( GCITEMUID itemUID_, int iType_, int iPrice_ );
    void RemoveReward( GCITEMUID itemUID_ );
    void Update();

private:
    virtual void ActionPerformed( const KActionEvent& event ) {};
    virtual void OnCreate( void );
    virtual void OnInitialize();

    void SetRewardGP( int iGP_ );
    void SetRewardGem( int iGem_ );
    void SetRewardCrystal( int iCrystal_ );
    int GetRewardGP() const { return m_pkNumRewardGP->GetNumber(); }
    int GetRewardGem() const { return m_pkNumRewardGem->GetNumber(); }
    int GetRewardCrystal() const { return m_pkNumRewardCrystal->GetNumber(); }
    void OnClickSell();
    void OnClickExit();

private:
    KGCUINumber*    m_pkNumSellItem;
    KGCUINumber*    m_pkNumRewardGP;
    KGCUINumber*    m_pkNumRewardGem;
    KGCUINumber*    m_pkNumRewardCrystal;

    KD3DWnd*        m_pkBtnSell;
    KD3DWnd*        m_pkBtnExit;

    KD3DStatic*     m_pkOwnGP;
    KD3DStatic*     m_pkOwnGem;
    KD3DStatic*     m_pkOwnPoint;
    KD3DStatic*     m_pkOwnCrystal;
    KD3DStatic*     m_pkHeroCoinText;

    std::map< GCITEMUID, std::pair<int,int> > m_mapPriceInfo;
};

DEFINE_WND_FACTORY( KGCSellInfo );
DECLARE_WND_FACTORY( KGCSellInfo );
DECLARE_WND_FACTORY_NAME( KGCSellInfo );
