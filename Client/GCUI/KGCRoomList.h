#ifndef _KGCROOMLIST_H_
#define _KGCROOMLIST_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCRoomBar;
class KD3DStatic;
class KGCRoomFilterBox;

class KGCRoomList: public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRoomList );
    /// Default constructor
    KGCRoomList( void );
    /// Default destructor
    virtual ~KGCRoomList( void );
    /// Copy constructor
    KGCRoomList( const KGCRoomList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomList& operator=( const KGCRoomList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    

    enum ROOM_LIST_TYPE
    {
        RLT_DUNGEON,
        RLT_MATCH,
        RLT_GUILDMATCH,
        RLT_MAX,
    };

    enum ROOM_BAR_NUM
    {
        RBN_DUNGEON = 6,
        RBN_MATCH =7,
        RBN_GUILDMATCH = 7,
    };

    enum
    {
        NUM_ROOM_LIST = 10,
    };

public:
    virtual void FrameMoveInEnabledState();

    void OnCreateRoom();
    void OnQuickJoin();
    void UpdateRoomList();
    void OnPrevPage();
    void OnNextPage();
    void OnRoomFilterBox();
    void OnClose();
    void OnAllRoom();
    void OnWaitRoom();
    void InitRoomList( ROOM_LIST_TYPE eRooListType_ );    
    void TurnOnRoomFilterBox( bool m_bUse );
    void OnFilterBtnClick();

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete();

private:
    KD3DWnd*            m_pkCreateRoomButton;
    KD3DWnd*            m_pkQuickJoinButton;
    KD3DWnd*            m_pkWaitRoomButton;
    KD3DWnd*            m_pkAllRoomButton;

    KD3DWnd*            m_pkPrevButton;
    KD3DWnd*            m_pkNextButton;

    KD3DStatic*         m_pkPageInfo;

    KSafeArray<KGCRoomBar*,NUM_ROOM_LIST> m_apkRoomBar;

    // 대전일 경우..
    KGCRoomFilterBox*   m_pkRoomFilterBox;

    // 던전일 경우..
    KSafeArray<KD3DWnd*,4> m_apkFilterLevelButton;
    KD3DWnd*            m_pkCloseButton;

    int                 m_iSelectRoom;
    int                 m_iRoombarNum;
    ROOM_LIST_TYPE                 m_eRoomListType;

	int					m_iPrevMaxPage;
	int					m_iPrevPage;

    KD3DWnd*			m_pkFilterBtn;
	KD3DWnd*			m_pkFilterOnBtn;
};

DEFINE_WND_FACTORY( KGCRoomList );
DECLARE_WND_FACTORY( KGCRoomList );
DECLARE_WND_FACTORY_NAME( KGCRoomList );

#endif //_KGCROOMLIST_H_