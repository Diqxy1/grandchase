#pragma once

class KGCOlympicGawibawiboGameInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicGawibawiboGameInfo );
    /// Default constructor
    KGCOlympicGawibawiboGameInfo( void );
    /// Default destructor
    virtual ~KGCOlympicGawibawiboGameInfo( void );
    /// Copy constructor
    KGCOlympicGawibawiboGameInfo( const KGCOlympicGawibawiboGameInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
	const KGCOlympicGawibawiboGameInfo& operator=( const KGCOlympicGawibawiboGameInfo& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

    void	SetActionFunc(boost::function0<void> a_Func){m_ActionClickFunc = a_Func;}
    void    SetReqInfo( time_t  tmTodayDate, int nGameType, int nTeamType );
    void    SetGameInfo(int iConinCount, int iAccumulateData );

private:
    KD3DWnd*	m_pkBtnChallenge;

    int			m_iTryPoint;
    int			m_iAccumulateData;

    time_t      m_tmTodayDate;
    int         m_nGameType;
    int         m_nTeamType;

    boost::function0<void>					m_ActionClickFunc;

};

DEFINE_WND_FACTORY( KGCOlympicGawibawiboGameInfo );
DECLARE_WND_FACTORY( KGCOlympicGawibawiboGameInfo );
DECLARE_WND_FACTORY_NAME( KGCOlympicGawibawiboGameInfo );
