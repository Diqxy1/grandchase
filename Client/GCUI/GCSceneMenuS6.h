#pragma once

class KGCSceneMenuBtnS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCSceneMenuBtnS6 );
	/// Default constructor
	KGCSceneMenuBtnS6( void );
	/// Default destructor
	virtual ~KGCSceneMenuBtnS6( void );
	/// Copy constructor
	KGCSceneMenuBtnS6( const KGCSceneMenuBtnS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSceneMenuBtnS6& operator=( const KGCSceneMenuBtnS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

#ifndef DISABLE_HERO_SHOP_ICON
	void OnClickHeroShop();
#endif

	void OnClickSkillTree();
	void OnClickCouple();
	void OnClickSquare();
	void OnClickCalendar();
	void OnClickMission();
	void OnClickMSN();
	void OnClickOption();
	void OnClickCreateBreadkup();
	void OnClickGuild();
	void OnClickEnterGuild();

	void OnClickEventBanner();
	void OnClickReplayRecord();
	void OnClickUserMigration();
	void OnClickEnchant();
	void OnClickCharSelectScene();
	void OnClickServerScene();
	void OnClickMenuOpen();
	void OnClickMenuClose();
	void OnClickGacha();
#ifdef USE_VIP_MENU_BTN // Kawan>
	void OnClickVipReward();
#endif
	void OnClickEventMiniGameBanner();
	void OnClickSurvey();    
	void OnClickMonster();
	void OnClickDepot();
	void OnClickAgit();
	void OnClickPost();
	void OnClickReplay();
    void OnClickRoyalVip();
	void OnClickGuide();

	void CreateBtnList( void );
	void SetBtnInfo( std::vector<int> &vecContents, std::vector<int> &vecEvents, std::vector<int> &vecFriends, std::vector<int> &vecSystem );

    bool IsPrivateCharDiableMenu( int nMenu );

private:
	void RegisterFunc();
	void LoadLuaTable();
	void SetBtnGroupList( std::vector<int> &vecContents, std::vector<int> &vecEvents, std::vector<int> &vecFriends, std::vector<int> &vecSystem );
	void SetBtnListPos( void );
	void InitBtnList( void );
   

private:
	KD3DSizingBox* m_pkBack;
	KD3DWnd* m_pkLine;

	typedef void (KGCSceneMenuBtnS6::*HANDLER)();
	struct MenuBtnInfo{
		int iNum;
		int iToolTip;
		std::string strUIName;
	};

	std::map<std::string, HANDLER>  m_mapEvent;
	std::map<int, MenuBtnInfo>  m_mapBtnInfo;
	std::map<int, KD3DWnd*> m_mapUIBtnList;
	std::vector< KD3DWnd*> m_vecContents;
	std::vector< KD3DWnd*> m_vecEvents;
    std::vector< KD3DWnd*> m_vecFriends;
    std::vector< KD3DWnd*> m_vecSystem;

	time_t m_tBtnListActiveTime;

    
    
};

DEFINE_WND_FACTORY( KGCSceneMenuBtnS6 );
DECLARE_WND_FACTORY( KGCSceneMenuBtnS6 );
DECLARE_WND_FACTORY_NAME( KGCSceneMenuBtnS6 );


class KGCSceneMenuS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
 
public:
    DECLARE_CLASSNAME( KGCSceneMenuS6 );
    /// Default constructor
    KGCSceneMenuS6( void );
    /// Default destructor
    virtual ~KGCSceneMenuS6( void );
    /// Copy constructor
    KGCSceneMenuS6( const KGCSceneMenuS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSceneMenuS6& operator=( const KGCSceneMenuS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
private:
	struct MenuListInfo{
		int iSceneNum;
		D3DXVECTOR2 vMenuBtnPos;
		float fBtnListPosX;
		std::vector<int> vecContents;
		std::vector<int> vecEvents;
        std::vector<int> vecFriends;
        std::vector<int> vecSystem;
	};

    D3DXVECTOR2 m_vecSkilltreebtnPos;
    D3DXVECTOR2 m_vecCreateSkilltreebtnPos;
   
private:
    enum 
    {
        POST_ICON_TOGGLE_FRAME = 10,

        POST_DEFAULT_TEXTURE_START_X = 315,
        POST_DEFAULT_TEXTURE_START_Y = 355,
        POST_DEFAULT_TEXTURE_END_X = 356,
        POST_DEFAULT_TEXTURE_END_Y = 391,

        //POST_HOVER_TEXTURE_START_X =  315,
        POST_HOVER_TEXTURE_START_Y = 397,
        //POST_HOVER_TEXTURE_END_X = 356,
        POST_HOVER_TEXTURE_END_Y = 433,
    };
    enum MOVEDIRECTION
    {
        MOVE_UP = 1,
        MOVE_DOWN = 2,

    };
public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnChangeState( void );
	void OnOpenBtnClick( );
	void OnCloseBtnClick( );
	void OnMenuBtnClick( );
	void OnClickEvent( void );
	void OnHideBtnMSG( );
	void OnShowBtnMSG( );
	void OnPostBtnClick( ); 
    void OnSkillTreeBtnClick();
    void OnInit();
	void FrameMoveInEnabledState( );
	void UpdateMarks();
    void UpdateMenuList();

	void OffMenu( const KActionEvent& event );
    bool GetOpenMenu()     { return m_bOpenMenu; }
    void ShowNotice( bool bRender );
    bool IsUpPostBtn();
    bool IsMenuOpened();

private:
	void LoadLuaTable();
	void OnChangePos( MenuListInfo &kInfo );
    void MoveButton(MOVEDIRECTION mdirection);
    void BlinkButton();

private:
	KD3DWnd* m_pkOpenBtn;
	KD3DWnd* m_pkCloseBtn;
	KD3DWnd* m_pkMenuBtn;
	KD3DWnd* m_pkMSN;
    KD3DWnd* m_pkMSNBlink;
    KD3DWnd* m_pkSkilltreeBtn;
    KD3DWnd* m_pkSkilltreeAlram;
 
    KD3DWnd* m_pkPostBtn;
	KD3DWnd* m_pkPostBlinkBtn;

	KD3DWnd* m_pkGuideBtn;
	KD3DWnd* m_pkGuideBlinkBtn;
    
public:
	KGCSceneMenuBtnS6* m_pkBtnList;

private:
	std::map<int, MenuListInfo>  m_mapMenuListInfo;

	
    
	int m_iAlphaTime;
    int m_iheight;
    int m_iToggleFrame;
    float m_Percent;
    bool m_bUp;
    bool m_bOpenMenu;
    bool m_bOpenPost;
    
    
};

DEFINE_WND_FACTORY( KGCSceneMenuS6 );
DECLARE_WND_FACTORY( KGCSceneMenuS6 );
DECLARE_WND_FACTORY_NAME( KGCSceneMenuS6 );
