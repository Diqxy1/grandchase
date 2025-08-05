#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;

class KGCAwakeningSkillBox : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCAwakeningSkillBox );
	/// Default constructor
	KGCAwakeningSkillBox( void );
	/// Default destructor
	virtual ~KGCAwakeningSkillBox( void );
	/// Copy constructor
	KGCAwakeningSkillBox( const KGCAwakeningSkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAwakeningSkillBox& operator=( const KGCAwakeningSkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ECLICK_TRAINNIG_BTN = 100,
        EMAX_AWAKENING_CLASS = 4,
    };

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DImageIcon*                                  m_pkAwakeningSkillIcon;
    KD3DStatic*                                     m_pkDescAwakening;
    KSafeArray< KD3DWnd*, EMAX_AWAKENING_CLASS >    m_pkAwakeningClass;
    KD3DWnd*                                        m_pkBtnPlus;
    EGCSkillTree                                    m_eSkillID;

public:
    void InitAwakeningSkillBox();
    void SetAwakeningSkill( EGCSkillTree eSkillID_ );

};

DEFINE_WND_FACTORY( KGCAwakeningSkillBox );
DECLARE_WND_FACTORY( KGCAwakeningSkillBox );
DECLARE_WND_FACTORY_NAME( KGCAwakeningSkillBox );

