#pragma once

class KGCDungeonTitleColorS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonTitleColorS6 );
    /// Default constructor
    KGCDungeonTitleColorS6( void );
    /// Default destructor
    virtual ~KGCDungeonTitleColorS6( void );
    /// Copy constructor
    KGCDungeonTitleColorS6( const KGCDungeonTitleColorS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonTitleColorS6& operator=( const KGCDungeonTitleColorS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		KDTC_COLOR_MAX = 9,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnClickColorBtn( const KActionEvent& event );
	void ClickColorBtn( int iIndex );
	void SetCheckPos( int iIndex );
	void SetColorBtn( );
    int currentIndex;
private:
	KSafeArray<KD3DWnd*,KDTC_COLOR_MAX> m_saColorBtn;
	KD3DWnd* m_pkCheck;

#if defined(NATION_PHILIPPINE)
    KD3DWnd* m_pkGMColorBtn;
#endif

};

DEFINE_WND_FACTORY( KGCDungeonTitleColorS6 );
DECLARE_WND_FACTORY( KGCDungeonTitleColorS6 );
DECLARE_WND_FACTORY_NAME( KGCDungeonTitleColorS6 );
