#pragma once

class KGCRitasRankInfo;
class KGCRitasRankList : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasRankList );
    /// Default constructor
    KGCRitasRankList( void );
    /// Default destructor
    virtual ~KGCRitasRankList( void );
    /// Copy constructor
    KGCRitasRankList( const KGCRitasRankList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasRankList& operator=( const KGCRitasRankList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum{
		NUM_SHOW_RANK = 4,
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();


	void InitRankList();
	void OnScoll();
	void UpdateRankInfo();

private:
	KGCRitasRankInfo*	m_pkRankInfo[NUM_SHOW_RANK];

	KD3DWnd*			m_pkBtnScroll;
	KD3DWnd*			m_pkScrollRect;

	std::vector<D3DXVECTOR2> m_vecScrollPos;

	int	m_iCurrentStep;
	int m_iMaxStep;
};

DEFINE_WND_FACTORY( KGCRitasRankList );
DECLARE_WND_FACTORY( KGCRitasRankList );
DECLARE_WND_FACTORY_NAME( KGCRitasRankList );
