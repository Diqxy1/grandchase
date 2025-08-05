#pragma once


class KGCSelectedCharFrame;
class KGCCharIcon;

class KGCCharIntroFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharIntroFrame );
    /// Default constructor
    KGCCharIntroFrame( void );
    /// Default destructor
    virtual ~KGCCharIntroFrame( void );
    /// Copy constructor
    KGCCharIntroFrame( const KGCCharIntroFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharIntroFrame& operator=( const KGCCharIntroFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:


	enum {
		ICON_STAR_GAP = 5,
		BETWEEN_ICON_GAP = 1,
		ICON_END_GAP = 5,
		CHAR_ICON_COLUMN = 3,
		CHAR_ICON_ROW = 10,
		BLINK_LIMIT_TIME = GC_FPS_LIMIT
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	virtual void OnInitialize();

	virtual void FrameMoveInEnabledState( void );

	virtual void PostChildDraw();


	void ArrayCharIcon();

	void SetSelectedCharIntro();

	void GoPreScene();

	void CreatCashParticle();
	void OffCashParticle();
private:
	KGCCharIcon*	m_pkCharIcon[GC_CHAR_NUM];
	KD3DWnd*		m_pkCharImage[GC_CHAR_NUM];
	KD3DWnd* m_pkFocusingIcon;
	KD3DWnd* m_pkCharFocus;
	
	KD3DWnd*		m_pkBtnIconPageUp;
	KD3DWnd*		m_pkBtnIconPageDown;

	KD3DWnd*		m_pkBtnBefore;

	KD3DWnd*		m_pkNoSelectedCharFrame;

	KGCSelectedCharFrame*	m_pkSelectedCharFrame;
	KD3DSizingBox*		m_pkCharIconArrayRect;

	int			m_cntBlinkTime;

	std::map<int,std::vector<CParticleEventSeqPTR>>	m_mapCashParticle;

	bool m_bEmptyNoHaveChar;
    bool m_bEmptyNoCreateChar;

};

DEFINE_WND_FACTORY( KGCCharIntroFrame );
DECLARE_WND_FACTORY( KGCCharIntroFrame );
DECLARE_WND_FACTORY_NAME( KGCCharIntroFrame );
