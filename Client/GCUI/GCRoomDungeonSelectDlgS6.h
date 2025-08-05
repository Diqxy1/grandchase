#pragma once

class KGCRoomDungeonBtnS6;

class KGCRoomDungeonSelectDlgS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomDungeonSelectDlgS6 );
    /// Default constructor
    KGCRoomDungeonSelectDlgS6( void );
    /// Default destructor
    virtual ~KGCRoomDungeonSelectDlgS6( void );
    /// Copy constructor
    KGCRoomDungeonSelectDlgS6( const KGCRoomDungeonSelectDlgS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomDungeonSelectDlgS6& operator=( const KGCRoomDungeonSelectDlgS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KRDSD_LIST_MODE_BTN_ALL = 0,
		KRDSD_LIST_MODE_BTN_HERO = 1,
		KRDSD_LIST_MODE_BTN_RAID = 2,
		KRDSD_LIST_MODE_BTN_EVENT = 3,
		KRDSD_LIST_MODE_BTN_NUM = 4,
		KRDSD_LIST_MAX = 5,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void CreateRoomListUI( void );
	void SetShowBtnList( );
	void SetBtnList( void );
	void SetMapInfo( void );
	void InitStateBtn( void );
	void SetBtnPos( void );
	void OnClickBtnClose( void );
	void OnScrollPos( void );
	void OnClickBtnBar( const KActionEvent& event );
	void InitBtnMode();
private:
	//대륙 정보, 던전 리스트
	std::map<int, std::vector<int>> m_mapBtnInfoList;

	KD3DScrollbar* m_pkScrollbar;
	int m_iScrollpos;

	std::vector<KGCRoomDungeonBtnS6*> m_vecBtnList;
	KD3DWnd *m_pkCloseBtn;

	KSafeArray<KD3DWnd*,KRDSD_LIST_MODE_BTN_NUM > m_saModeBtnList;

	int m_iSelectMode;
};

DEFINE_WND_FACTORY( KGCRoomDungeonSelectDlgS6 );
DECLARE_WND_FACTORY( KGCRoomDungeonSelectDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomDungeonSelectDlgS6 );
