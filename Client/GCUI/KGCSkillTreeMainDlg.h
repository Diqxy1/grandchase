#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;
class KGCCharSkillBox;
class KGCClassSkillBox;
class KGCQuickSkillBox;
class KGCSkillSelectWnd;
class KGCAwakeningSkillDlg;
class KGCSkillDescBox;

class KGCSkillTreeMainDlg : public KD3DDialog,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillTreeMainDlg );
	/// Default constructor
	KGCSkillTreeMainDlg( void );
	/// Default destructor
	virtual ~KGCSkillTreeMainDlg( void );
	/// Copy constructor
	KGCSkillTreeMainDlg( const KGCSkillTreeMainDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillTreeMainDlg& operator=( const KGCSkillTreeMainDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ESKILL_BOX_COUNT = 6,
        EWIDTH_PROGRESS_BAR = 241,
        EMAX_AWAKENING_SKILL_LV = 60,
    };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KSafeArray< KGCCharSkillBox*, ESKILL_BOX_COUNT >            m_pkCharSkillBox;
    KSafeArray< KGCClassSkillBox*, ESKILL_BOX_COUNT >           m_pkClassSkillBox;
    KGCQuickSkillBox*                                           m_pkQuickSkillBox;
    KGCSkillSelectWnd*                                          m_pkSkillSelectWnd;
    KD3DWnd*                                                    m_pkBtnClose;
    KD3DWnd*                                                    m_pkBtnInitSkill;
    KD3DWnd*                                                    m_pkBtnCancelSettings;
    KD3DStatic*                                                 m_pkStaticSkillPoint;
    KD3DWnd*                                                    m_pkBtnSelectClass;
    KGCMultiMenu*                                               m_pkPopupMenuClass;
    KD3DStatic*                                                 m_pkStaticClass;

    KGCAwakeningSkillDlg*                                       m_pkAwakeningSkillDlg;

    KGCSkillDescBox*                                            m_pkSkillDescBox;

    KD3DScrollbar*                                              m_pkScrollBarCharSkill;
    KD3DWnd*                                                    m_pkScrollRangeCharSkill;
    KD3DScrollbar*                                              m_pkScrollBarClassSkill;
    KD3DWnd*                                                    m_pkScrollRangeClassSkill;

    KD3DWnd*                                                    m_pkAwakeningProgressBar;
    KD3DStatic*                                                 m_pkStaticAwakeningProgress;
    KD3DStatic*                                                 m_pkStaticNextAwakeningLV;

    D3DXVECTOR4                                                 m_vAwakeningProgressCoords;

    std::vector< EGCSkillTree >                                 m_vecCharSkills;
    std::map< char, std::vector< EGCSkillTree > >               m_mapClassSkills;
    std::vector< EGCSkillTree >                                 m_vecCurClassSkills;
    std::vector< EGCSkillTree >                                 m_vecAwakeningSkills;
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >    m_mapOriAllEquipedSkillInfo;
    std::map< std::pair<EGCCharType, char>, SkillSet >          m_mapOriTrainedSkill;
    std::map< std::pair<EGCCharType, char>, SkillSet >          m_mapOriTutorialTrainedSkill;
    std::map< EGCSkillGroup, std::vector< EGCSkillTree > >      m_mapLvSkills;

    char m_cUICharType;
    char m_cUIPromotion;


public:
    void OnClose();
    void OnScrollCharSkill();
    void OnScrollClassSkill();
    void OnCancelSkillSettings();
    void OnInitSkillSettings();

    void UpdateSkillTreeMainDlg();
    void UpdateSkillTreeData();
    void ShowSkillSelectWnd( bool bRender_ );

    void SetLockScrollBar( bool bLock_ );
    void SetUICharType( char cCharType_ ) { m_cUICharType = cCharType_; }
    void SetUIPromotion( char cUIPromotion_ ) { m_cUIPromotion = cUIPromotion_; }

    EGCSkillTree GetUntrainSkillID( EGCSkillTree eSkillID_ );

    void RenderAwakeningProgress();
    void InitSkillTreeSettings();

    bool IsChangeSkillEquipInfo();
};

DEFINE_WND_FACTORY( KGCSkillTreeMainDlg );
DECLARE_WND_FACTORY( KGCSkillTreeMainDlg );
DECLARE_WND_FACTORY_NAME( KGCSkillTreeMainDlg );

