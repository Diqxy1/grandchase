#pragma once

class KD3DStatic;
class KGCGradeMark;

class KGCGuildJoinWaitingUserInfo :	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildJoinWaitingUserInfo );
    /// Default constructor
    KGCGuildJoinWaitingUserInfo( void );
    /// Default destructor
    virtual ~KGCGuildJoinWaitingUserInfo( void );
    /// Copy constructor
    KGCGuildJoinWaitingUserInfo( const KGCGuildJoinWaitingUserInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildJoinWaitingUserInfo& operator=( const KGCGuildJoinWaitingUserInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:	
	KGCGradeMark*				m_pkGradeMark;    
	KD3DSizingBox*				m_pkSelectList;
	KD3DStatic*					m_pkStaticNIckname;
	KD3DStatic*					m_pkStatintroduce;
	KD3DStatic*					m_pkStaticApplicationDate;
	
public:
	void SetJoinWaitingUserInfo( KNGuildUserInfo& sInfo_ );
	void SetSelectCursorBox( bool bRender_ );
	bool IsSelectListBoxRender();
};

DEFINE_WND_FACTORY( KGCGuildJoinWaitingUserInfo );
DECLARE_WND_FACTORY( KGCGuildJoinWaitingUserInfo );
DECLARE_WND_FACTORY_NAME( KGCGuildJoinWaitingUserInfo );