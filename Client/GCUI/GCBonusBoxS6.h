#pragma once


class KGCBonusItemS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCBonusItemS6 );
	/// Default constructor
	KGCBonusItemS6( void );
	/// Default destructor
	virtual ~KGCBonusItemS6( void );
	/// Copy constructor
	KGCBonusItemS6( const KGCBonusItemS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBonusItemS6& operator=( const KGCBonusItemS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetItemInfo( GCITEMID iItemID );
private:
	KGCItemImgWnd* m_pkItemImage;
	KD3DStatic* m_pkItemName;

};

DEFINE_WND_FACTORY( KGCBonusItemS6 );
DECLARE_WND_FACTORY( KGCBonusItemS6 );
DECLARE_WND_FACTORY_NAME( KGCBonusItemS6 );



class KGCBonusBoxUserInfoS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCBonusBoxUserInfoS6 );
	/// Default constructor
	KGCBonusBoxUserInfoS6( void );
	/// Default destructor
	virtual ~KGCBonusBoxUserInfoS6( void );
	/// Copy constructor
	KGCBonusBoxUserInfoS6( const KGCBonusBoxUserInfoS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBonusBoxUserInfoS6& operator=( const KGCBonusBoxUserInfoS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetUserInfo( int iChar, std::wstring strNick );
private:
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkChar;
	KD3DStatic* m_pkUserNick;

};

DEFINE_WND_FACTORY( KGCBonusBoxUserInfoS6 );
DECLARE_WND_FACTORY( KGCBonusBoxUserInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCBonusBoxUserInfoS6 );


class KGCBonusBoxS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBonusBoxS6 );
    /// Default constructor
    KGCBonusBoxS6( void );
    /// Default destructor
    virtual ~KGCBonusBoxS6( void );
    /// Copy constructor
    KGCBonusBoxS6( const KGCBonusBoxS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBonusBoxS6& operator=( const KGCBonusBoxS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	bool IsComplete() {return m_bComplete;}
	void SetComplete( bool bComplete ) { m_bComplete = bComplete;}
	void FrameMoveInEnabledState( void );
	void SetRewardItem( std::map<int, KDropItemInfo> mapRewardItem, int iChar, std::wstring strNickName );
	void SetTime(){	m_tmStartTime =  GCUTIL_TIME::GetCurTime();}
private:

	KD3DSizingBox* m_pkBack;
	KGCBonusBoxUserInfoS6* m_pkUserInfo;
	KGCBonusItemS6* m_pkBaseItem;
	KGCBonusItemS6* m_pkAddItem;

	bool m_bComplete;

	time_t m_tmStartTime;
	
};

DEFINE_WND_FACTORY( KGCBonusBoxS6 );
DECLARE_WND_FACTORY( KGCBonusBoxS6 );
DECLARE_WND_FACTORY_NAME( KGCBonusBoxS6 );
