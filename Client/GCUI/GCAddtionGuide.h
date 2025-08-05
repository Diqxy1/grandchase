#pragma once

class KGCAddtionGuide : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAddtionGuide );
    /// Default constructor
    KGCAddtionGuide( void );
    /// Default destructor
    virtual ~KGCAddtionGuide( void );
    /// Copy constructor
    KGCAddtionGuide( const KGCAddtionGuide& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAddtionGuide& operator=( const KGCAddtionGuide& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
	virtual void OnInitialize();


	void SetShownKey(int iCount_ , ...);
	void RegistKey(int iKey_ , int iInput_);
	UINT CheckTheMon();
	void SettingScene();

	void ToggleRenderAll( bool bRender );

	void IinitAtGoWorldMap();

	void KeyChangeMotion();

	void LerpForMotion(int iSetKey_ ,int iGoalKey_ , int iStartKey_ = -1);

	void InitKey();

public:
	enum {
		NONE_KEY = -1,
		KEY_UP = 0,
		KEY_LEFT = 1,
		KEY_DOWN = 2,
		KEY_RIGHT = 3,
		KEY_A = 4,
		KEY_S = 5,
		KEY_D = 6,
		KEY_F = 7,
		KEY_Z = 8,
		KEY_G = 9,

		MAX_KEY

	};

	enum ANI_STATE{
		OPENING = 0,
		ENDING = 1,
        CHECK_MON = 2,
		NOTHING = 3
	};

	enum GUIDE_STATE{
		STATE_GAME_ON = 0,
		STATE_GAME_OFF = 1,
		STATE_TUTORIAL = 2,
        STATE_CHECKMON = 3,
        STATE_RENDER_OFF = 4,
	};

	enum TUTORIAL_MOTION{
		DIRECTION_Z = 0,
		DIRECTION_ASDF = 1,
		ASDF_DIRECTION = 2,
		ASDF_Z = 3,
		Z_DIRECTION = 4,
		Z_ASDF = 5,
        ASDFG = 6,
		STATE_DIRECTION,
		STATE_Z,
		STATE_ASDF,
        STATE_NOTHING,
	};

	enum {
		MIDDLE_TERM_TIME = 60
	};

	ANI_STATE GetAniState() { return m_eAniState; }
	void SetAniState(ANI_STATE eAniState_ ) { m_eAniState = eAniState_; }

	GUIDE_STATE GetState()	{ return m_eState; }
	void SetState(GUIDE_STATE eState_ )	{ m_eState = eState_; }

	void ShowAddtionGuide( int iShowFrame_ = 275 );

	void ToggleGauge(bool bToggle_ , int iMonIndex_ = -1 );

	void KeyEvent();

	void SetGauge(float fGauge_ );
	void OnHide( void );

	void OnExit();
	void OnHelp();

	void CanToSetChangMotion(TUTORIAL_MOTION eMotion_ );

	void SetPassTutorial(bool bPass_) { m_bPassedTutorial=bPass_; }
	void SetLockKey(bool bLockKey_ )	{ m_bLockKey = bLockKey_; }
    void SetNearPortal( bool bIsNearPortal);
    void CheckSpecialMonster();
    void EndTrialForestDungeon();
    int GetGuideUserState();
    void SetGuideUserState( int eState);
private:


	GUIDE_STATE m_eState;
	ANI_STATE m_eAniState;

	TUTORIAL_MOTION m_eTutorialMotion;

	int     m_iAlpha;
	int     m_iShowFrame;
	int     m_iFadeInFrame;
	int     m_iFadeOutFrame;
	float   m_fAlphaChange;

	KD3DWnd*    m_pkBack;

	KSafeArray<KD3DWnd*,MAX_KEY> m_arrpKey;

	KSafeArray<D3DXVECTOR2,MAX_KEY> m_arrKeyPos;

	KSafeArray<D3DXVECTOR2,MAX_KEY> m_arrKeyWidthHeigh;

	KD3DStatic* m_pkStaticDesc;


	KD3DWnd*    m_pkGaugeBar;
	KD3DWnd*    m_pkGauge;

	DWORD m_dwOrigineGaugeWidth;

	D3DXVECTOR4 m_vGaugeTexCoord;

	KD3DWnd* m_pkBntHelp;
	D3DXVECTOR2 m_vBntHelpOrginPos;
	KD3DWnd* m_pkMpKeyLeft;
	KD3DWnd* m_pkKeyRight;
	KD3DWnd* m_pkApKeyLeft;

	KD3DWnd* m_pkBntExit;
	KD3DWnd* m_pkAddExplain;

	bool m_bPassedTutorial;

	bool m_bMonsterisInTheDungeon;
	bool m_bCheckNearMonster;
	bool m_bCheckNearSpecialMon;
    bool m_bIsNearPortal;
    bool m_bExperienceTutorial;
    int m_eGuideUserState;


	float m_fMotionTimer;

	bool m_bLockKey;


	int m_iTutorialTempTime;
};

DEFINE_WND_FACTORY( KGCAddtionGuide );
DECLARE_WND_FACTORY( KGCAddtionGuide );
DECLARE_WND_FACTORY_NAME( KGCAddtionGuide );
