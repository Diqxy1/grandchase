#pragma once

class KGCHeroTicketSystem : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroTicketSystem );
    /// Default constructor
    KGCHeroTicketSystem( void );
    /// Default destructor
    virtual ~KGCHeroTicketSystem( void );
    /// Copy constructor
    KGCHeroTicketSystem( const KGCHeroTicketSystem& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroTicketSystem& operator=( const KGCHeroTicketSystem& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    void UpdateDialog( void );
    void OnConfirmButton( void );
	void OnExitButton( void );

private:
	KGCItemImgWnd*	m_pkItemImg;
	KD3DStatic*     m_pkQuantity;
	KD3DStatic*     m_pkDescription;
	KD3DWnd*        m_pkConfirmBtn;
	KD3DWnd*        m_pkExitBtn;
    KItem*          m_pkTicketItem;
    EGCGameMode     emGameMode;
    GAME_STATE      emState;
    KHeroDungeon    HeroDungeonInfo;
};

DEFINE_WND_FACTORY( KGCHeroTicketSystem );
DECLARE_WND_FACTORY( KGCHeroTicketSystem );
DECLARE_WND_FACTORY_NAME( KGCHeroTicketSystem );
