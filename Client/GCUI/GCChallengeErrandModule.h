#pragma once

class KGCChallengeErrandModule : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandModule );
    /// Default constructor
    KGCChallengeErrandModule( void );
    /// Default destructor
    virtual ~KGCChallengeErrandModule( void );
    /// Copy constructor
    KGCChallengeErrandModule( const KGCChallengeErrandModule& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandModule& operator=( const KGCChallengeErrandModule& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum eErrandModuleName {
        NAME_BREAD = 0,
        NAME_FLOWER = 1,
        NAME_BOOK = 2,
        NAME_EGG = 3,
        NAME_FISH = 4,
        NAME_TOFU = 5,
        NAME_APPLE = 6,
        NAME_CARROT = 7,
        NAME_MEAT = 8,
        NAME_HOME = 9
    };

public:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    float StayLocationX( void );
    float StayLocationY( void );

    void SetShopIndex( int iIndex ) { m_eModuleName = iIndex; }
    int GetShopName( void ) { return m_eModuleName; }

    void SetModuleIndex( int iIndex ) { m_nIndex = iIndex; }
    int GetModuleIndex( void ) { return m_nIndex; }

    void SetRewardItem( KItem kItem );
    void SetImgRenderOff( );
    void SetImgRenderOn( );
    void UpdateRewardRender( bool bRender );

    
    bool GetMerry( void );
    void SetMerry( bool bMerry );
    void MoveMerryAmin();
    void MerryRender( bool bRender );
    void ClearItem( void );


private:
    KD3DWnd*    m_pkPlayerLocation;
    KD3DWnd*    m_pkDefaultImg;
    KD3DWnd*    m_pkHoverImg;
    KD3DWnd*    m_pkSpace;

    KD3DWnd*        m_pkRewardBack;
    KD3DStatic*     m_pkItemGuide;
    KGCItemImgWnd*  m_pkImgItem;

    KD3DWnd*        m_pkMerry;

    int m_nIndex;
    int m_eModuleName;
    bool m_bIsMerry;
    int m_iAniFrame;
};

DEFINE_WND_FACTORY( KGCChallengeErrandModule );
DECLARE_WND_FACTORY( KGCChallengeErrandModule );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandModule );
