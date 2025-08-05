#pragma once

class KD3DStatic;
class KGCGradeMark;
class KGCMultiMenu;

class KGCGuildMemberInfo :	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildMemberInfo );
    /// Default constructor
    KGCGuildMemberInfo( void );
    /// Default destructor
    virtual ~KGCGuildMemberInfo( void );
    /// Copy constructor
    KGCGuildMemberInfo( const KGCGuildMemberInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMemberInfo& operator=( const KGCGuildMemberInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EGuildMemberGrade
	{
		EMG_MASTER		=1,
		EMG_GENERAL		=2,
		EMG_NORMAL		=3,
		EMG_BREAK_UP	=4,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
    KD3DWnd*            m_pkGuildMaster;
    KD3DWnd*            m_pkGuildGeneral;
	KD3DWnd*            m_pkBtnManagePopUp;
    KD3DStatic*         m_pkStaticNickName;
    KD3DStatic*         m_pkStaticIntroduce;
	KD3DStatic*         m_pkStaticActivity;
	KD3DStatic*         m_pkStaticLastConnection;
	KD3DStatic*         m_pkStaticManage;
	KD3DSizingBox*		m_pkSelectSignBox;

	KGCGradeMark*       m_pkGradeMark;
	KGCMultiMenu*		m_pkManagePopUpMenu;

	bool				m_bUsePopUpMenu;
	DWORD				m_dwUserUID;
public:
    void SetUserInfo( const KNGuildUserInfo& info_ );
    void SetGuildGrade( int iUserLevel_ );
	void UsePopUpMenu( bool bUse_ );
	void SetSelectCursorBox( bool bRender_ );
	bool IsSelectListBoxRender();
	void SetRenderPopupMenu( bool bRender_ );
	void SetChangeGuildMemberGrade( UCHAR ucMemberGrade_ );
	int GetChangeMemberGrade(DWORD dwMemberUID_);
};

DEFINE_WND_FACTORY( KGCGuildMemberInfo );
DECLARE_WND_FACTORY( KGCGuildMemberInfo );
DECLARE_WND_FACTORY_NAME( KGCGuildMemberInfo );