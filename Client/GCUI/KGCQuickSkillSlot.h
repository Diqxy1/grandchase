#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCQuickSkillSlot : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCQuickSkillSlot );
	/// Default constructor
	KGCQuickSkillSlot( void );
	/// Default destructor
	virtual ~KGCQuickSkillSlot( void );
	/// Copy constructor
	KGCQuickSkillSlot( const KGCQuickSkillSlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCQuickSkillSlot& operator=( const KGCQuickSkillSlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );

private:
    KD3DImageIcon*                  m_pkSkillIcon;
    EGCSkillTree                    m_eSkillID;
    KD3DWnd*                        m_pkWndSelect;
    KD3DWnd*                        m_pkWndSelectLine;
    
public:
    EGCSkillTree GetSkillID() { return m_eSkillID; } 
    void SetSkillImage( EGCSkillTree eSkillID_ );
    void InitSkillImage();
    void EquipSlot( EGCSkillTree eSkillID_ );
    void UnEquipSlot();
    void SetSelectSlot( bool bSelect_ );
    bool IsSelectedSlot();
    void HoverQuickSlot( bool bHover_ );

};

DEFINE_WND_FACTORY( KGCQuickSkillSlot );
DECLARE_WND_FACTORY( KGCQuickSkillSlot );
DECLARE_WND_FACTORY_NAME( KGCQuickSkillSlot );

