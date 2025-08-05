#pragma once

class KGCAdventureRewardItemWnd : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAdventureRewardItemWnd );
    /// Default constructor
    KGCAdventureRewardItemWnd( void );
    /// Default destructor
    virtual ~KGCAdventureRewardItemWnd( void );
    /// Copy constructor
    KGCAdventureRewardItemWnd( const KGCAdventureRewardItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAdventureRewardItemWnd& operator=( const KGCAdventureRewardItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void ShowItemInfo( void );
    void OnMouseEnter( void );
    void OnMouseLeave( void );
    void SetRewardItemBoxInfo( GCITEMID dwItemID );
    void SetContinentObtainCnt( DWORD dwObtainCnt, DWORD dwMaxCnt );
    void RenderOff();
    void RenderOn();
    void IsRender( bool bRender );
private:
    KD3DStatic* m_pkStaticItemNum;
    KD3DSizingBox* m_pkStaticBack;
    KGCItemImgWnd* m_pkItemImage;

    bool m_bMouseBound;
    GCITEMID m_dwItemID;
    DWORD m_MaxCnt;

};

DEFINE_WND_FACTORY( KGCAdventureRewardItemWnd );
DECLARE_WND_FACTORY( KGCAdventureRewardItemWnd );
DECLARE_WND_FACTORY_NAME( KGCAdventureRewardItemWnd );
