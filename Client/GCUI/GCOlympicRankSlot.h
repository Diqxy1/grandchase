#pragma once

class KGCOlympicRankSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicRankSlot );
    /// Default constructor
    KGCOlympicRankSlot( void );
    /// Default destructor
    virtual ~KGCOlympicRankSlot( void );
    /// Copy constructor
    KGCOlympicRankSlot( const KGCOlympicRankSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicRankSlot& operator=( const KGCOlympicRankSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    enum { NUM_NATION = 8 };

private:
    KSafeArray< KD3DWnd*, NUM_NATION >    m_apkRank;
    KSafeArray< KD3DWnd*, NUM_NATION >    m_apkFlag;
    KSafeArray< KD3DWnd*, NUM_NATION >    m_apkNatoion;

    KGCUINumber*    m_pkScore;

private:
    int     m_iNation;
    int     m_iRank;
    int     m_iScore;

private:
    void RenderRankSlot( const bool bRender_ );

public:
    void ClearRankSlot( void );
    void SetRankSlot( const int iNation_, const int iRank_, const int iScore_ );

    int GetNationType( void ) { return m_iNation; }
    int GetRank( void ) { return m_iRank; }
    int GetScore( void ) { return m_iScore; }
};

DEFINE_WND_FACTORY( KGCOlympicRankSlot );
DECLARE_WND_FACTORY( KGCOlympicRankSlot );
DECLARE_WND_FACTORY_NAME( KGCOlympicRankSlot );
