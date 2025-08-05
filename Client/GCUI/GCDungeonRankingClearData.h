#pragma once

class KGCDungeonRankingClearData : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonRankingClearData );
    /// Default constructor
    KGCDungeonRankingClearData( void );
    /// Default destructor
    virtual ~KGCDungeonRankingClearData( void );
    /// Copy constructor
    KGCDungeonRankingClearData( const KGCDungeonRankingClearData& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonRankingClearData& operator=( const KGCDungeonRankingClearData& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


    enum 
    {
        EM_STAGE_NUM = 10,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void UpdateUI();

public:
    KSafeArray<KD3DWnd*,EM_STAGE_NUM>       m_pkStageClear;
    KSafeArray<KD3DStatic*,EM_STAGE_NUM>    m_pkStageClearText;

    KD3DWnd*                                m_pkClearNoData;
    KD3DWnd*                                m_pkClearBack;
};

DEFINE_WND_FACTORY( KGCDungeonRankingClearData );
DECLARE_WND_FACTORY( KGCDungeonRankingClearData );
DECLARE_WND_FACTORY_NAME( KGCDungeonRankingClearData );
