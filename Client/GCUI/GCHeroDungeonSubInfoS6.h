#pragma once
class KGCTowerLevelSelector;

class KGCHeroDungeonSubInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonSubInfoS6 );
    /// Default constructor
    KGCHeroDungeonSubInfoS6( void );
    /// Default destructor
    virtual ~KGCHeroDungeonSubInfoS6( void );
    /// Copy constructor
    KGCHeroDungeonSubInfoS6( const KGCHeroDungeonSubInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonSubInfoS6& operator=( const KGCHeroDungeonSubInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetHeroDungeonV1_2( void );
    void SetHeroDungeonV1_3( void );
	void SetHeroDungeonV30( void );

    void SetRenderHeroDungeonV1_2( bool bOn );
    void SetRenderHeroDungeonV1_3( bool bOn );
    void SetRenderHeroDungeonV30( bool bOn );

    void OnRankingButton();
    void SetCorruptionIcon( void );
	void SetHeroDungeonInfo();
	void SetItemInfo( void );
	void FrameMoveInEnabledState( void );
	void DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd);
	void OnCheckRewards( void );
	enum {
		KHDSI_CIRCLE_NUM = 5,
		KHDSI_TITLE_NUM =2,
		KDSI_ITEM_VIEW_NUM = 4,
	};

private:
    KD3DWnd *m_pkRankingBtn;

	KSafeArray<KD3DWnd *,KHDSI_CIRCLE_NUM> m_saCircle;
	KSafeArray<KGCItemImgWnd*, KDSI_ITEM_VIEW_NUM> m_saItemImage;
    KSafeArray<KD3DWnd*, 2> m_pCorruptionIcon;

	KD3DStatic *m_pkClearNum;

	KGCTowerLevelSelector* m_pkTowerLevelSelector;

	KD3DWnd* m_pkCheckReward;
	KD3DWnd* m_pkGetReward;
    std::pair<int, int> pairNum;

};

DEFINE_WND_FACTORY( KGCHeroDungeonSubInfoS6 );
DECLARE_WND_FACTORY( KGCHeroDungeonSubInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonSubInfoS6 );
