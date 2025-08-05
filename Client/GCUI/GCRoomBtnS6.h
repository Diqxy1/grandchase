#pragma once

class KGCCategoryBox;

class KGCRoomBtnBackS6 : public KD3DWndActiveChild
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCRoomBtnBackS6 );
	/// Default constructor
	KGCRoomBtnBackS6( void );
	/// Default destructor
	virtual ~KGCRoomBtnBackS6( void );
	/// Copy constructor
	KGCRoomBtnBackS6( const KGCRoomBtnBackS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomBtnBackS6& operator=( const KGCRoomBtnBackS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void OnClickBtn( void );
	void InitWnd( void );
private:
	KSafeArray<KD3DWnd*,4> m_saBack;

};

DEFINE_WND_FACTORY( KGCRoomBtnBackS6 );
DECLARE_WND_FACTORY( KGCRoomBtnBackS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomBtnBackS6 );

class KGCRoomBtnS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomBtnS6 );
    /// Default constructor
    KGCRoomBtnS6( void );
    /// Default destructor
    virtual ~KGCRoomBtnS6( void );
    /// Copy constructor
    KGCRoomBtnS6( const KGCRoomBtnS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomBtnS6& operator=( const KGCRoomBtnS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();

	void SetRoomBtnInfo( KSimpleRoomInfo &kRoomInfo );
	void ClearBarInfo( void );
	void OnBarInfo( void );
	KSimpleRoomInfo GetRoomInfo(){ return m_kRoomInfo; }
	void OnClickBtn( void );
	void InitWnd( void );
private:
	KD3DStatic *m_pkRoomNumber;
	KD3DStatic *m_pkRoomStr;
	KD3DStatic *m_pkRoomUser;

	KD3DWnd *m_pkLock;
	KD3DWnd *m_pkState;

	KD3DWnd *m_pkGCBlog;
	KD3DWnd *m_pkGCClub;
	KD3DWnd *m_pkGoldenTicket;
	KD3DWnd *m_pkPremiumGCClub;

	KSimpleRoomInfo m_kRoomInfo;

	KGCCategoryBox *m_pkCategory;

	KD3DWnd* m_pkBack;

	enum {
		OPTION_ATTRIBUTE_CORRECTION = 0,
		OPTION_ALLOWED_CHARS = 1,
		OPTION_ALLOWED_RANKS = 2,
		OPTION_BLOCK_PET_ATTACK = 3,
		OPTION_BLOCK_SPECIAL4_ATTACK = 4,

		MAX_OPTIONS = 5,
	};

	KSafeArray< KD3DWnd*, MAX_OPTIONS > m_saOptions;
	void OnClickAllowedRank();
	void OnClickBlockedChars();
};

DEFINE_WND_FACTORY( KGCRoomBtnS6 );
DECLARE_WND_FACTORY( KGCRoomBtnS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomBtnS6 );
