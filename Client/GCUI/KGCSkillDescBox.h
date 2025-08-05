#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemImgWnd;
class KD3DStatic;
class KGCUINumber;

class KGCSkillDescBox : public KD3DWnd,
                             public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillDescBox );
	/// Default constructor
	KGCSkillDescBox( void );
	/// Default destructor
	virtual ~KGCSkillDescBox( void );
	/// Copy constructor
	KGCSkillDescBox( const KGCSkillDescBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillDescBox& operator=( const KGCSkillDescBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        EBOX_TYPE_A         = 0,
        EBOX_TYPE_B         = 1,
        EBOX_HEIGHT_TYPE_A  = 230,
        EBOX_HEIGHT_TYPE_B  = 326,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DSizingBox*                      m_pkBackGroundBox;
    KD3DStatic*                         m_pkStaticName;
    KD3DStatic*                         m_pkStaticsubInfo1;
    KD3DStatic*                         m_pkStaticsubInfo2;
    KD3DWnd*                            m_pkWndCommandSkill;
    KD3DWnd*                            m_pkWndSlotSkill;
    KD3DWnd*                            m_pkWndSpecial;
    KD3DStatic*                         m_pkStaticSkillType;
    KD3DStatic*                         m_pkStaticUseDesc;
    KD3DStatic*                         m_pkStaticSkillDesc;

public:    
    void SetSkillDesc( int iType_, EGCSkillTree eSkillID_ );
    void RenderInfoTypeB( bool bRender_ );
    void SetPosition( D3DXVECTOR2& vPos_ );

};

DEFINE_WND_FACTORY( KGCSkillDescBox );
DECLARE_WND_FACTORY( KGCSkillDescBox );
DECLARE_WND_FACTORY_NAME( KGCSkillDescBox );

