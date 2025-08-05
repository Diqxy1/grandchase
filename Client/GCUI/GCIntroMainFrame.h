#pragma once

class KGCIntroMainFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCIntroMainFrame );
    /// Default constructor
    KGCIntroMainFrame( void );
    /// Default destructor
    virtual ~KGCIntroMainFrame( void );
    /// Copy constructor
    KGCIntroMainFrame( const KGCIntroMainFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCIntroMainFrame& operator=( const KGCIntroMainFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	
	enum{
		DFIFFICULT_MAX_LEVEL = 3,
	};

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();

	void SetIntroMainFrame(const int& iCharType_ );
	
	
	void PlayMovie();
private:

	void ReadyToPlayMovie();
	void SetDifficultLevel_();
	void SetJopAndPromotionName();
	void SetCharDesc();
	void SetCharWeaponInfo();

	KD3DStatic* m_pkStaticDifficult;
	KD3DStatic* m_pkStaticSetDifficult;

	KD3DWnd*	m_pkDifficultStar[DFIFFICULT_MAX_LEVEL];

	KD3DWnd*	m_pkImgCharName[GC_CHAR_NUM];
	KD3DWnd*	m_pkCharImage[GC_CHAR_NUM];

    KD3DWnd*	m_pkPromotionImage[NUM_JOB_LEVEL +1];
    KD3DWnd*    m_pkPromotionStep[NUM_JOB_LEVEL - 1];

	KD3DStatic*	m_pkStaticWeapon;

	KD3DStatic*	m_pkStaticCharDesc;
	KD3DWnd*	m_pkMovieDrawRect;
	KD3DWnd*	m_pkBtnMoviePlay;

	int			m_iCharType;
};

DEFINE_WND_FACTORY( KGCIntroMainFrame );
DECLARE_WND_FACTORY( KGCIntroMainFrame );
DECLARE_WND_FACTORY_NAME( KGCIntroMainFrame );
