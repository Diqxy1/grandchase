#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;

class KGCClassSkillBox : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCClassSkillBox );
	/// Default constructor
	KGCClassSkillBox( void );
	/// Default destructor
	virtual ~KGCClassSkillBox( void );
	/// Copy constructor
	KGCClassSkillBox( const KGCClassSkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCClassSkillBox& operator=( const KGCClassSkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ECLICK_USE_NOUSE_BTN = 100,
        ECLICK_TRAIN_SKILL = 101,
        EMAX_QUICK_KEY_TYPE = 5,
    };

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );    

private:
    KD3DImageIcon*                                  m_pkSkillIcon;
    KD3DStatic*                                     m_pkStaticSkillName;
    KD3DStatic*                                     m_pkStaticOpenLv;
    KD3DWnd*                                        m_pkWndMaster;
    KD3DWnd*                                        m_pkWndLock;
    KD3DWnd*                                        m_pkBtnPlus;
    KD3DWnd*                                        m_pkTypeHotKey;
    KD3DWnd*                                        m_pkTypeCommand;
    KD3DWnd*                                        m_pkBtnUse;
    KD3DWnd*                                        m_pkBtnNoUse;
    KD3DSizingBox*                                  m_pkHoverFrame;

    KSafeArray<KD3DWnd*, EMAX_QUICK_KEY_TYPE>       m_pkQuickKeyType;

    EGCSkillTree                                    m_eSkillID;
    
public:
    void OnUseSkill();
    void OnNoUseSkill();
    void SetClassSkillInfo( EGCSkillTree eSkillID_ );
    void SetBoxState( const GCSkill* pSkill_ );
    void SetHotKeyState( EGCSkillTree eSkillID_ );
    void SetSkillTypeState( const GCSkill* pSkill_ );
    void EmptyBoxState();
    void RenderHoverFrame( bool bRender_ );

    EGCSkillTree GetSkillID() { return m_eSkillID; }

};

DEFINE_WND_FACTORY( KGCClassSkillBox );
DECLARE_WND_FACTORY( KGCClassSkillBox );
DECLARE_WND_FACTORY_NAME( KGCClassSkillBox );

