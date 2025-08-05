#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;
class KGCAwakeningSkillBox;

class KGCAwakeningSkillDlg : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCAwakeningSkillDlg );
	/// Default constructor
	KGCAwakeningSkillDlg( void );
	/// Default destructor
	virtual ~KGCAwakeningSkillDlg( void );
	/// Copy constructor
	KGCAwakeningSkillDlg( const KGCAwakeningSkillDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAwakeningSkillDlg& operator=( const KGCAwakeningSkillDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        EMAX_AWAKENING_BOX_LINE = 3,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KSafeArray< KGCAwakeningSkillBox*, EMAX_AWAKENING_BOX_LINE > m_pkAwakeningSkillBox;
    KD3DCheckBox*       m_pkCheckBoxLock;
    KD3DWnd*            m_pkBtnClose;
    KD3DWnd*            m_pkBtnOpen;
    KD3DScrollbar*      m_pkScroll;

    D3DXVECTOR2         m_vStartWndPos;
    D3DXVECTOR2         m_vEndWndPos;
    D3DXVECTOR2         m_vAnimWndPos;

    bool                m_bCloseWnd;
    bool                m_bAnimation;
    bool                m_bFirstOpen;

    std::vector< EGCSkillTree > m_vecAwakeningSkill;

public:
    void OnClose();
    void OnOpen();
    void OnCheck();
    void OnScroll();
    void UpdateAwakeningSkillDlg( std::vector< EGCSkillTree >& vecSkillID_ );

};

DEFINE_WND_FACTORY( KGCAwakeningSkillDlg );
DECLARE_WND_FACTORY( KGCAwakeningSkillDlg );
DECLARE_WND_FACTORY_NAME( KGCAwakeningSkillDlg );