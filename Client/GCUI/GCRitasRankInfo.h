#pragma once

class KGCRitasRankInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasRankInfo );
    /// Default constructor
    KGCRitasRankInfo( void );
    /// Default destructor
    virtual ~KGCRitasRankInfo( void );
    /// Copy constructor
    KGCRitasRankInfo( const KGCRitasRankInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasRankInfo& operator=( const KGCRitasRankInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum{
		NUM_SPECIAL_GRADE = 3
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void SetRankInfo( std::wstring wstrNickName_, int iScore_ , int iGrade_ );
	void ClearInfo();
private:
	KD3DWnd*	m_pkGrade[NUM_SPECIAL_GRADE];

	KD3DStatic*	m_pkStaticNickname;
	KD3DStatic*	m_pkStaticScore;

	KGCUINumber*	m_pkRank;
};

DEFINE_WND_FACTORY( KGCRitasRankInfo );
DECLARE_WND_FACTORY( KGCRitasRankInfo );
DECLARE_WND_FACTORY_NAME( KGCRitasRankInfo );
