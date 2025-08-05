#pragma once

class KGCSceneMenuBtn : public KD3DWnd   // extends
{
public:
    DECLARE_CLASSNAME( KGCSceneMenuBtn );
    /// Default constructor
    KGCSceneMenuBtn( void );
    /// Default destructor
    virtual ~KGCSceneMenuBtn( void );
    /// Copy constructor
    KGCSceneMenuBtn( const KGCSceneMenuBtn& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSceneMenuBtn& operator=( const KGCSceneMenuBtn& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum 
    {
        EM_HIDE_TYPE_VIP = 0,
        EM_HIDE_TYPE_MONSTER_CARD = 1,
        EM_HIDE_SIZE,
    };

public:
    virtual void OnCreate( void );
    virtual void OnDestroy();
    virtual void FrameMoveInEnabledState();

	enum { LIMIT_MENU_OPENTIME = 300 };
private:
    void FrameMove_OpenStart();
    void FrameMove_MoveUp();
    void FrameMove_ChangeWidthInit();
    void FrameMove_ChangeWidth( DWORD dwTargetWidth );
    void FrameMove_MoveUpBtnsInit();
    void FrameMove_MoveUpBtns( int iCount, D3DXVECTOR2 vPos );
    void FrameMove_HideStartInit();
    void FrameMove_HideStart( D3DXVECTOR2 vPos, D3DXVECTOR2 vOrigin );
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
    void OnClickEventBanner();
    void OnClickReplayRecord();
    void OnClickUserMigration();
    void OnClickEnchant();

	void OnClickCharSelectScene();
	void OnClickServerScene();

    void OnClickMenuOpen();
    void OnClickMenuClose();

    void HideSceneButtons( const KActionEvent& event_ );
    void CheckSubBoxOpen();

    void DisableSceneBtns( const KActionEvent& event_ );
    void ToggleButtons();

    void EnableEventCreateInfo();
    void DisableEventCreateInfo();
    void AllRenderOff();
    void AllRenderOn();
    void OnClickGacha();
#ifdef USE_VIP_MENU_BTN // Kawan>
    void OnClickVipReward();
#endif
    void OnClickEventMiniGameBanner();
    void OnClickSurvey();    

	void OnClickMonster();
	void OnClickDepot();

    bool IsInRoomEnableMini( const std::string& strWndName_ );    // 대기방 안에서 떠도 되는 미니 버튼인가?

private:
    KD3DSizingBox*            m_pkSubBox;
    KD3DWnd*            m_pkMenuOpenBtn;
    KD3DWnd*            m_pkMenuCloseBtn;
    KD3DSizingBox*      m_pkFrame;
    std::vector<KD3DWnd*> m_vecMiniBtns;
    std::vector<KD3DWnd*> m_vecBtns;
    std::map<KD3DWnd::uid_type,std::pair<int,int>> m_mapMarkAlpha;
    boost::function<void (KGCSceneMenuBtn*)> m_pSubBoxFunc;
    std::set<std::string> m_setExcept;


	UINT		m_iCntMenuOpenTime;

public:
    void UpdateMarks();
    void RenderMenuOpenBtn();

};

DEFINE_WND_FACTORY( KGCSceneMenuBtn );
DECLARE_WND_FACTORY( KGCSceneMenuBtn );
DECLARE_WND_FACTORY_NAME( KGCSceneMenuBtn );
