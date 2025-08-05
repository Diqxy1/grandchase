#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;
class KGCQuickSkillSlot;

class KGCQuickSkillBox : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCQuickSkillBox );
	/// Default constructor
	KGCQuickSkillBox( void );
	/// Default destructor
	virtual ~KGCQuickSkillBox( void );
	/// Copy constructor
	KGCQuickSkillBox( const KGCQuickSkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCQuickSkillBox& operator=( const KGCQuickSkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {               
        ECLICK_SKILL_SLOT       = 1,
        ECLICK_MOVE_BTN         = 2,
        ESELECT_SKILL_WND_OFF   = 3,        
        ESKILL_SLOT_COUNT       = 5,
    };    

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );


private:
    KSafeArray< KGCQuickSkillSlot*, ESKILL_SLOT_COUNT >       m_pkQuickSkillSlot;
    KD3DStatic*         m_pkStaticCurSlotType;
    KD3DWnd*            m_pkBtnPrev;
    KD3DWnd*            m_pkBtnNext;
    int                 m_iCurSelectedSlotIndex;  // 현재 선택되어진 슬롯 인덱스
    

public:
    void OnPrev();
    void OnNext();
    void UpdateSkillSlot();
    void SetQuickSkillSlot( EGCSkillTree eSkillID_ );

};

DEFINE_WND_FACTORY( KGCQuickSkillBox );
DECLARE_WND_FACTORY( KGCQuickSkillBox );
DECLARE_WND_FACTORY_NAME( KGCQuickSkillBox );

