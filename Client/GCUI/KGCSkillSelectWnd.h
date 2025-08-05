#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCSkillSelectIcon;

class KGCSkillSelectWnd : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCSkillSelectWnd );
    /// Default constructor
    KGCSkillSelectWnd( void );
    /// Default destructor
    virtual ~KGCSkillSelectWnd( void );
    /// Copy constructor
    KGCSkillSelectWnd( const KGCSkillSelectWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSkillSelectWnd& operator=( const KGCSkillSelectWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ECLICK_SKILL_ICON   = 100,
        EICON_LINE_COUNT    = 3,
        EICON_ROW_COUNT     = 4,
        EMAX_SKILL_ICON     = 16,
        EICON_OFFSET_X      = 60,
        EICON_OFFSET_Y      = 60,
        EICON_START_X       = 15,
        EICON_START_Y       = 55,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    std::vector< KGCSkillSelectIcon* >      m_vecSkillIcon;
    std::vector< EGCSkillTree >             m_vecSkillList;
    KD3DScrollbar*                          m_pkScrollBar;

public:
    void CreateUI();
    void OnScroll();
    void UpdateSkillIcons();
    int GetMaxLineCount();

};

DEFINE_WND_FACTORY( KGCSkillSelectWnd );
DECLARE_WND_FACTORY( KGCSkillSelectWnd );
DECLARE_WND_FACTORY_NAME( KGCSkillSelectWnd );




class KGCSkillSelectIcon : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCSkillSelectIcon );
	/// Default constructor
	KGCSkillSelectIcon( void );
	/// Default destructor
	virtual ~KGCSkillSelectIcon( void );
	/// Copy constructor
	KGCSkillSelectIcon( const KGCSkillSelectIcon& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillSelectIcon& operator=( const KGCSkillSelectIcon& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );

private:
    KD3DImageIcon*                          m_pkSkillIcon;
    EGCSkillTree                            m_eSkillID;
    KD3DWnd*                                m_pkWndSpecial;
    KD3DWnd*                                m_pkBack;
    
public:
    EGCSkillTree GetSkillID() { return m_eSkillID; } 
    void SetSkillIcon( EGCSkillTree eSkillID_ );
    void InitSkillIcon();    
    void RenderSelectBack( bool bRender_ );

};

DEFINE_WND_FACTORY( KGCSkillSelectIcon );
DECLARE_WND_FACTORY( KGCSkillSelectIcon );
DECLARE_WND_FACTORY_NAME( KGCSkillSelectIcon );

