#pragma once

class KGCRoomListBtnSetS6;
class KGCItemImgWnd;
class KGCTowerLevelSelector;

class KGCDungeonSubInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonSubInfoS6 );
    /// Default constructor
    KGCDungeonSubInfoS6( void );
    /// Default destructor
    virtual ~KGCDungeonSubInfoS6( void );
    /// Copy constructor
    KGCDungeonSubInfoS6( const KGCDungeonSubInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonSubInfoS6& operator=( const KGCDungeonSubInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

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
	void OnClickBtn( const KActionEvent& event );
	void UpdateChangeState( void );
    void UpdateData( void );
	
	void SetCorruptionIcon( void );
    void RenderBtnSet( int index );
	void SetItemInfo( void );
	void FrameMoveInEnabledState( void );
	void DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd);
private:
	KGCTowerLevelSelector* m_pkTowerLevelSelector;
	KSafeArray<KGCRoomListBtnSetS6*, KDSI_BTN_SET_NUM> m_saBtnSet;
	KSafeArray<KGCItemImgWnd*, KDSI_BTN_SET_4> m_saItemImage;
	KSafeArray<KD3DWnd*, 2> m_pCorruptionIcon;
	KD3DWnd* m_pkTimesBG;
	KD3DStatic* m_pkClearNum;
};

DEFINE_WND_FACTORY( KGCDungeonSubInfoS6 );
DECLARE_WND_FACTORY( KGCDungeonSubInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCDungeonSubInfoS6 );
