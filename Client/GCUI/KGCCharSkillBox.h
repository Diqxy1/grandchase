#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;

class KGCCharSkillBox : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCharSkillBox );
	/// Default constructor
	KGCCharSkillBox( void );
	/// Default destructor
	virtual ~KGCCharSkillBox( void );
	/// Copy constructor
	KGCCharSkillBox( const KGCCharSkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCharSkillBox& operator=( const KGCCharSkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ECLICK_TRAIN_BTN   = 100,
        ECLICK_UNTRAIN_BTN   = 101,
    };

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );


private:
    KD3DImageIcon*                      m_pkSkillIcon;
    KD3DStatic*                         m_pkStaticSkillName;
    KD3DStatic*                         m_pkStaticCurSkillLevel;
    KD3DStatic*                         m_pkStaticOpenLv;
    KD3DWnd*                            m_pkWndMaster;
    KD3DWnd*                            m_pkWndLock;
    KD3DWnd*                            m_pkBtnPlus;
    KD3DWnd*                            m_pkBtnMinus;
    KD3DSizingBox*                      m_pkHoverFrame;

    EGCSkillTree                        m_eSkillID;

public:
    void SetCharSkillInfo( EGCSkillTree eSkillID_ );
    void SetBoxState( const GCSkill* pSkill_ );
    void SetMasterState( bool bMaster_ );
    void EmptyBoxState();
    void RenderHoverFrame( bool bRender_ );

    EGCSkillTree GetSkillID() { return m_eSkillID; }

};

DEFINE_WND_FACTORY( KGCCharSkillBox );
DECLARE_WND_FACTORY( KGCCharSkillBox );
DECLARE_WND_FACTORY_NAME( KGCCharSkillBox );

