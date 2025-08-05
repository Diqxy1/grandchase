#pragma once

class KD3DStatic;
class KGCHeroDungeonInfo;
class KGCHeroDungeonShop;
class KGCHeroDungeonReward;
class KGCHeroDungeonUpgrade;

class KGCHeroDungeonDlg :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonDlg );
    /// Default constructor
    KGCHeroDungeonDlg( void );
    /// Default destructor
    virtual ~KGCHeroDungeonDlg( void );
    /// Copy constructor
    KGCHeroDungeonDlg( const KGCHeroDungeonDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonDlg& operator=( const KGCHeroDungeonDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum 
	{
		ETAB_HERO_DUNGEON_SHOP      = 0,
		ETAB_HERO_DUNGEON_INFO      = 1,
        ETAB_HERO_DUNGEON_REWARD1   = 2,
        ETAB_HERO_ITEM_UPGRADE      = 3,
        ETAB_HERO_DUNGEON_SIZE      = 4,
	};

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    KGCHeroDungeonInfo*     m_pkDungeonInfo;
    KGCHeroDungeonShop*     m_pkDungeonItemShop;
    KGCHeroDungeonReward*   m_pkDungeonItemReward1;
    KGCHeroDungeonUpgrade*  m_pkItemUpgrade;

private:
    KSafeArray<KD3DWnd*,ETAB_HERO_DUNGEON_SIZE>     m_pkTabWnd;
    KSafeArray<KD3DWnd*,ETAB_HERO_DUNGEON_SIZE>     m_pkTabWndOn;

    KD3DWnd*            	m_pkClose1;
    KD3DWnd*            	m_pkClose2;
    KD3DWnd*            	m_pkTitleVIP;

public:
    void OnTabDungeonInfo();
    void OnTabItemShop();
    void OnTabDungeonReward1();
    void OnTabItemUpgrade();
    
    void UpdateReward();

    void OnChangeTab( int nTab );
    bool IsRenderItemPreview();
    bool IsESCAPEKey();
    void ReleaseLock();

    void OnClose();
};

DEFINE_WND_FACTORY( KGCHeroDungeonDlg );
DECLARE_WND_FACTORY( KGCHeroDungeonDlg );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonDlg );