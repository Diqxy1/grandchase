#ifndef _GCSCENE_H_
#define _GCSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCMainFrame;
class KGCAlertWnd;
class KGCJustInTime;

enum ButtonType
{
    BTN_WORLDMAP = 0,
    BTN_MATCH,
	BTN_GUILD_MATCH,
    BTN_SHOP,
#if defined USE_COORDI_SHOP
    BTN_COORDI_SHOP,
#endif
	BTN_MY_INFO,
    BTN_SKILL_TREE,
    NUM_BUTTON,
};

enum AlertButtonType
{
    ALERT_BTN_MISSION = 0,
    ALERT_BTN_MSN,
    ALERT_BTN_CALENDAR,
    ALERT_BTN_MYINFOSCENE,
    ALERT_BTN_SKILLTREE,
    ALERT_BTN_COUPLE,

    NUM_ALERT_BUTTON,
};

enum EM_HOT_ITEM
{
    EM_HOT_ITEM_WEAPON = 0,
    EM_HOT_ITEM_ARMOR,
    EM_HOT_ITEM_BOX,
    EM_HOT_ITEM_SIZE,
};

class KGCScene : public KD3DWnd, // extends
    public KActionListener    // implements
{
public:
    //DECLARE_CLASSNAME( KGCScene );
    /// Default constructor
    KGCScene( void );
    /// Default destructor
    virtual ~KGCScene( void );
    /// Copy constructor
    KGCScene( const KGCScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCScene& operator=( const KGCScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event ) = 0;
    void SetButtonLock( ButtonType eButton, bool bLock, std::wstring strToolTip = L"" );
    void SetButtonRender( ButtonType eButton, bool bRender );

    void SetAlertButtonLock( AlertButtonType eButton, bool bLock );
    void SetAlertButtonRender( AlertButtonType eButton, bool bRender );

    void EnterScene();
    void LeaveScene();
    virtual void InitScene( void );
    virtual void BindToLua();
    virtual void Destroy( void );

protected:
    virtual void OnCreate( void );
    virtual void OnEnterScene() {};
    virtual void OnLeaveScene() {};
    virtual void FrameMoveInEnabledState();
    virtual void PostChildDraw( void );

    // just in time event
    void JustInTimeFrameMove();


protected:
    CParticleEventSeqPTR m_pEffUITabNew;
    static std::set<KGCScene*>  ms_setScenes;

    KGCMainFrame*               m_pkMainFrame;
    KSafeArray<KD3DWnd*,NUM_BUTTON> m_apkButton;

    KSafeArray<KGCAlertWnd*,NUM_ALERT_BUTTON> m_apkAlertButton;

    KSafeArray< KD3DWnd*, EM_HOT_ITEM_SIZE >    m_apkHotItem;

    KD3DWnd*                    m_pkJustInTime_btn;

	int							m_iToggleFrame;
	
    // ≫oA¡ ¾E¸®¹I ÆAÆ¼A￢
    CParticleEventSeqPTR        m_pkParticleShopButton;
    CParticleEventSeqPTR        m_pkParticleAvatarButton;	

public:
	void ShowHotItemType( DWORD _dwType );
    void SetHotItemTypePos( int iIndex_ );
    bool IsLockGuildBtn( );
    
    void ActiveJustInTime();
    void ReqJustIntimeReward();
};
/*
DEFINE_WND_FACTORY( KGCScene );
DECLARE_WND_FACTORY( KGCScene );
DECLARE_WND_FACTORY_NAME( KGCScene );
*/

#endif // _GCSCENE_H_
