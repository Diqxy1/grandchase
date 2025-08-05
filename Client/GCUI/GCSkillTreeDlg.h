#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DComboBox.h"
//#include "../uifx/D3DSizingBox.h"

class KGCSkilltreeField;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;
class KGCSkillBox;
class KGCSkillSlotBox;

class KGCSkillTreeDlg:	public KD3DDialog,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillTreeDlg );
	/// Default constructor
	KGCSkillTreeDlg( void );
	/// Default destructor
	virtual ~KGCSkillTreeDlg( void );
	/// Copy constructor
	KGCSkillTreeDlg( const KGCSkillTreeDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillTreeDlg& operator=( const KGCSkillTreeDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	//virtual void OnDestroy( void );
    virtual void OnDestroyComplete();
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	virtual void PostChildDraw();
	virtual void PreDraw();
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

public:
    enum { OPEN_NORMAL = 0, OPEN_NEW_CHAR = 1 };

	void SetSelectedChar( int iCharType, int iPromotion, bool bChangeRight);
	void SetSelectedChar( int iCharType, int iPromotion);
    void SetSelectedChar( int iCharType_ );
	void Update();
	void UpdateTab();
	bool ReloadSkillTree( std::string strTabname );	
	void ThreadSafeUpdate();
    void SetEnable( bool bEnable )   {  m_bEnable = bEnable;    }
    bool IsEnable()                  {  return m_bEnable;       }
	KGCSkillSlotBox*m_pkSkillSlotBox;
	KGCDescBox*		m_pkDescBox;

private:
	bool			m_bFirstCreate;
	int				m_iSelectedChar;
	int				m_iPromotion;
	int				m_iSetID;
	bool			m_bFieldFlip;
	bool			m_bProcedureUpdateCall;
    bool            m_bEnable;
	std::string		m_strCurrentTab;

	KD3DWnd*		m_pkAlertNormal;
	KD3DWnd*		m_pkAlertSpecial;
	KD3DWnd*		m_pkAlertCommon;
	KD3DWnd*		m_pkBtnSkillLeft;
	KD3DWnd*		m_pkBtnSkillRight;
	KD3DWnd*		m_pkBtnReset;
	KD3DWnd*		m_pkBtnModeChange;
	KD3DWnd*		m_pkBtnClose;
	KD3DWnd*		m_pkTabSpecial;
	KD3DWnd*		m_pkTabSpecialAct;
	KD3DWnd*		m_pkTabNormalSkill;
	KD3DWnd*		m_pkTabNormalSkillAct;

	KD3DWnd*		m_pkTabActiveSkill;
	KD3DWnd*		m_pkTabActiveSkillAct;
	KD3DWnd*		m_pkTabPassiveSkill;
	KD3DWnd*		m_pkTabPassiveSkillAct;

	KD3DWnd*		m_pkTabCommonSkill;
	KD3DWnd*		m_pkTabCommonSkillAct;
	KD3DWnd*		m_pkTabTransform;
	KD3DWnd*		m_pkTabTransformAct;	
	KD3DWnd*		m_pkTransFormMenu;
	KSafeArray<KD3DWnd*,2> m_pkFlipButton;
    KD3DWnd*		m_pkTypeSwapBtn;

    KSafeArray< KD3DWnd*, NUM_JOB_LEVEL + 1 >   m_pkPromotion;
	KD3DWnd*		m_pkPreviewSkillField;

	KD3DStatic*		m_pkCurrentSp;
	KD3DStatic*		m_pkCurrentSpText;
	KD3DStatic*		m_pkMaxSp;
	KD3DStatic*		m_pkMaxSpText;
	KD3DStatic*		m_pkUsingSp;
	KD3DStatic*		m_pkUsingSpText;
	KD3DStatic*		m_pkCharMaxSp;
	KD3DStatic*		m_pkCharMaxSpText;
	KD3DStatic*		m_pkLevelString;
	KD3DStatic*		m_pkCurrentSkillLevel;
	KD3DStatic*		m_pkCurrentSkillExpPercent;
	KD3DStatic*		m_pkNextSkillDesc;

	KD3DStatic*		m_pkSkillRemovePointText;
	KD3DStatic*		m_pkSkillRemovePoint;
	KD3DStatic*		m_pkCharacterLevel;

	KD3DComboBox*	m_pkComboSkillSet;

	KGCCharacterUI*	m_pkCharacterName;
	KSafeArray<KGCSkilltreeField*,2> m_pkSkilltreeField;
	KD3DWnd*		m_pkLockerField;

	KD3DSizingBox*	m_pkSkillLevelGuage;
	KD3DSizingBox*	m_pkSkillLevelGuageBack;

	KD3DSizingBox*	m_pkLeftField;
	KD3DSizingBox*	m_pkRightField;
	KD3DSizingBox*	m_pkbetweenField;
	KGCSkillBox*	m_pkPreviewSkillBox;

	const GCSkill*	m_pkPreviewSkill;
	std::map<int, std::set<int> > m_mapOpenCharIndex;

    KD3DSizingBox*	m_pkBackAsin;
    KD3DStatic*     m_pkStaticAsin;

    KSafeArray< KD3DWnd*, GC_CHAR_NUM >     m_apkWaitImg;

    enum
    {
        TABOPEN_NORMALSKILL = 1,
        TABOPEN_SPECIAL = 2,
        TABOPEN_COMMONSKILL = 4,
        TABOPEN_ACTIVE = 8,
        TABOPEN_PASSIVE = 16,
        TABOPEN_TRANSFORM = 32,
    };
    std::map<int, map<int, UCHAR> > m_mapOpenTab;

    bool    m_bOpenNewChar;

	KD3DWnd*	m_pkLevelDrawPos;
    KD3DWnd* m_pkCommingSoon;
    bool    m_bActivePassive;

private:
	void OnTransformTabUI();
	void OnSepcialTabUI();
	void OnNormalTabUI();
	void OnCommonTabUI();
	void OnClickTransformSkillTab();
	void OnClickSpeicalTab();
	void OnClickNormalSkillTab();
	void OnClickCommonSkillTab();
	
	void OnClickActiveTab();
	void OnClickPassiveTab();
	
	void OnClickFlipBtn( KD3DWnd* pWnd, DWORD dwCode );    
	void OnClickCharPromotion( KD3DWnd* pWnd, DWORD dwCode );
	void OnClickSkillLeft();
	void OnClickSkillRight();

public:
	void OnClickClose();

private:
	void OnClickReset();
	void OnChangeSkillSet();
    void OnClickSwapBtn();

	bool LoadSkillTreeTab( KLuaManager& luaMgr, std::string strTable );
	bool LoadSkilltreeField( KLuaManager& luaMgr, int iFieldIndex );
	bool LoadSkillGroup( KLuaManager& luaMgr, int iFieldIndex );
	bool LoadSkillLocker( KLuaManager& luaMgr );
	bool AddLocker( int iLockID, D3DXVECTOR2 vPos, DWORD dwWidth, DWORD dwHeight, int iUnlockPos );
	void UpdateLocker();
	void UpdatePromotionBtn();
	bool UpdateFieldSize( std::string strTable );

	void HideSubSkillBox( int iFieldIndex );
	void ShowSubSkillBox( int iFieldIndex );

	void InitOpenChar();

	void FrameMove_BlinkAlert();
	void FrameMove_FlipCheck();

	bool CheckFlipCondition();

	int GetRemovePoint();



public:
	std::wstring GetSkillTitle( const GCSkill* pSkill );
	std::wstring GetMiddleDesc( const GCSkill* pSkill );
	std::wstring GetSimpleDesc( const GCSkill* pSkill );

	int GetSetID() { return m_iSetID; }
	bool CheckSkillOpenChar(int iChar);
    int GetEndSkillOpenPromotion( int iChar, int iPromotion );
	void UpdatePreviewSkill( bool bTutorial = false );

    void SetPassiveTabClick( bool bIsActivePassiveTab );
    bool IsClickPassiveTab() { return m_bActivePassive; }
    void UpdateSkillTab();
    void CheckDioTab();
    KD3DWnd* GetTrainableSkillBox();
    KD3DWnd* FindTrainableSkillBox();


public:
    bool m_bIsSelectedSkillB;
};

DEFINE_WND_FACTORY( KGCSkillTreeDlg );
DECLARE_WND_FACTORY( KGCSkillTreeDlg );
DECLARE_WND_FACTORY_NAME( KGCSkillTreeDlg );