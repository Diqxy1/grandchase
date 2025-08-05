#pragma once

class KGCRoomListBtnSetS6;

class KGCRaidDungeonInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRaidDungeonInfoS6 );
    /// Default constructor
    KGCRaidDungeonInfoS6( void );
    /// Default destructor
    virtual ~KGCRaidDungeonInfoS6( void );
    /// Copy constructor
    KGCRaidDungeonInfoS6( const KGCRaidDungeonInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRaidDungeonInfoS6& operator=( const KGCRaidDungeonInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
	enum {
		KDSI_ITEM_VIEW_NUM = 4,
	};

	enum {
		KRDI_JOIN_OK = 0,
		KRDI_JOIN_NOT = 1,
		KRDI_JOIN_BELOW = 2,
		KRDI_JOIN_TYPE_NUM,
	};

	enum {
		KRDI_ERR_JOIN_OK = 0,
		KRDI_ERR_JOIN_NOT = 1,
		KRDI_ERR_JOIN_BELOW = 2,
		KRDI_ERR_JOIN_TIME = 3,
		KRDI_ERR_JOIN_TYPE_NUM,
	};

	enum {
		KDSI_BTN_SET_0 = 0,
		KDSI_BTN_SET_1 = 1,
		KDSI_BTN_SET_2 = 2,
		KDSI_BTN_SET_3 = 3,
		KDSI_BTN_SET_4 = 4,
		KDSI_BTN_SET_5 = 5,
		KDSI_BTN_SET_NUM = 6,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetItemInfo( void );
	void FrameMoveInEnabledState( void );
	void DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd);
	void SetDungeonInfo();
	void SetRaidDungeon( void );
	std::wstring GetRaidDungeonConditionString( int iJoinType );
	int ConvertErrType( int iJoinType );
	void UpdateDificulty(void);
	void RenderBtnSet(int index);
	void SetCorruptionIcon(void);
private:
	KSafeArray<KGCRoomListBtnSetS6*, KDSI_BTN_SET_NUM> m_saBtnSet;
	KSafeArray<KGCItemImgWnd*, KDSI_ITEM_VIEW_NUM> m_saItemImage;
	KSafeArray<KD3DWnd*, KRDI_JOIN_TYPE_NUM> m_saJoinTypeWnd;
	KSafeArray<KD3DWnd*, 2> m_pCorruptionIcon;
	KD3DStatic *m_pkOpenTime;
	KD3DStatic *m_pkCondition;
	KD3DWnd* LevelBG;

};

DEFINE_WND_FACTORY( KGCRaidDungeonInfoS6 );
DECLARE_WND_FACTORY( KGCRaidDungeonInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCRaidDungeonInfoS6 );
