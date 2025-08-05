#pragma once

class KD3DStatic;

class KGCGuildListInfo :	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildListInfo );
    /// Default constructor
    KGCGuildListInfo( void );
    /// Default destructor
    virtual ~KGCGuildListInfo( void );
    /// Copy constructor
    KGCGuildListInfo( const KGCGuildListInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildListInfo& operator=( const KGCGuildListInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum EGuildGrade{
		NEW_GUILD		= 1,
		REGULAR_GUILD	= 2,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
    KD3DImageIcon*		m_pkGuildMarkIcon;
    KD3DWnd*            m_pkGuildGradeNew;
	KD3DWnd*            m_pkGuildGradeRegular;
    KD3DStatic*         m_pkStaticGuildName;
    KD3DStatic*         m_pkStaticGuildIntroduce;
	KD3DStatic*         m_pkStaticGuildBirthday;

	KD3DSizingBox*		m_pkListSelectBG;

public:
	void SetGuildGrade( EGuildGrade eGuildGrade_ );
	void InitGuildInfo();
	void SetGuildInfoData( KNGuildInfo& sNGuildInfo, bool bToolTip );
	void SetRenderListSelectBG( bool bRender_ );
	bool IsRenderONListSelectBG();
	void RenderGuildMark( bool bRender_ );
};

DEFINE_WND_FACTORY( KGCGuildListInfo );
DECLARE_WND_FACTORY( KGCGuildListInfo );
DECLARE_WND_FACTORY_NAME( KGCGuildListInfo );