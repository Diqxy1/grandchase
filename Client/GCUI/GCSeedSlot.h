#pragma once

class KGCSeedSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSeedSlot );
    /// Default constructor
    KGCSeedSlot( void );
    /// Default destructor
    virtual ~KGCSeedSlot( void );
    /// Copy constructor
    KGCSeedSlot( const KGCSeedSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSeedSlot& operator=( const KGCSeedSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DStatic* m_pkStaticSeedDesc;
    KD3DStatic* m_pkStaticGrowthDesc;
    KD3DStatic* m_pkStaticStarDesc;
    KD3DStatic* m_pkStaticSeedNum;
    KD3DStatic* m_pkStaticGrowthNum;
    KD3DStatic* m_pkStaticStarNum;
    KD3DStatic* m_pkStaticSlotDesc;

    KD3DWnd*    m_pkBtnSelectDefault;
    KD3DWnd*    m_pkBtnSelectActive;

public:
    void InitSeedSlot( int iSeedLevel_, int iSeedNum_, int iGrowthNum_, int iStarNum_ );
    void Clear( void );
    void SetActive( bool bSet_ );
    bool IsActive( void ) { return m_pkBtnSelectActive->IsRenderOn(); }


private:
    void OnClickBtn( KD3DWnd* pBtn_ );
};

DEFINE_WND_FACTORY( KGCSeedSlot );
DECLARE_WND_FACTORY( KGCSeedSlot );
DECLARE_WND_FACTORY_NAME( KGCSeedSlot );
