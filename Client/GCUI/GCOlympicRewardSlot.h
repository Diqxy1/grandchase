#pragma once

class KGCOlympicRewardSlot : public KD3DWnd    // extends
                                ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicRewardSlot );
    /// Default constructor
    KGCOlympicRewardSlot( void );
    /// Default destructor
    virtual ~KGCOlympicRewardSlot( void );
    /// Copy constructor
    KGCOlympicRewardSlot( const KGCOlympicRewardSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicRewardSlot& operator=( const KGCOlympicRewardSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*        m_pkItemBack;
    KD3DWnd*        m_pkItemRecvFrame;
    KD3DWnd*        m_pkBtnRecv;
    KD3DWnd*        m_pkShadow;
    KD3DStatic*     m_pkStaticScore;
    KGCItemImgWnd*  m_pkItemImg;

private:
    std::vector< KDropItemInfo >    m_vecItemInfo;
    DWORD                           m_dwScore;
    bool                            m_bActive;
    bool                            m_bCanRecv;
    bool                            m_bShowItemData;

public:
    void ClearRewadSlot( void );
    void SetRewardSlot( const IN std::vector< KDropItemInfo >& vecReward_, const DWORD dwScore_, const bool bActive_, const bool bCanRecv_ );
    
    DWORD    GetScore( void ) { return m_dwScore; }
    GCITEMID GetRewardItemID( void );

    bool     IsActive( void ) { return m_bActive; }
    void     SetActive( const bool bActive_ ) { m_bActive = bActive_; }
    bool     IsCanRecv( void ) { return m_bCanRecv; }
    void     SetCanRecv( const bool bCanRecv_ ) { m_bCanRecv = bCanRecv_; }

    void OnClickRecv( void );
    void ShowItemData( void );
};

DEFINE_WND_FACTORY( KGCOlympicRewardSlot );
DECLARE_WND_FACTORY( KGCOlympicRewardSlot );
DECLARE_WND_FACTORY_NAME( KGCOlympicRewardSlot );
