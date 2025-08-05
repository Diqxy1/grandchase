#pragma once

class KGCDungeonRankSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonRankSlot );
    /// Default constructor
    KGCDungeonRankSlot( void );
    /// Default destructor
    virtual ~KGCDungeonRankSlot( void );
    /// Copy constructor
    KGCDungeonRankSlot( const KGCDungeonRankSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonRankSlot& operator=( const KGCDungeonRankSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_RANK_NUM = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void InitUI();

public:
    void SetRankData( bool bMyData, KDungeonRankInfo& kRankInfo );
    void SetRank( int nRank );


public:
    KD3DWnd*            m_pkMyBack;
    KGCCharacterUI*     m_pkChar;

    KD3DStatic*         m_pkStaticName;
    KD3DStatic*         m_pkStaticStage;
    KD3DStatic*         m_pkStaticTime;

    KGCUINumber*    m_pkMyRankNum;
    KGCUINumber*    m_pkRankNum;

    KSafeArray<KD3DStatic*, EM_RANK_NUM>    m_pkTopRank;
    KSafeArray<KD3DStatic*, EM_RANK_NUM>    m_pkMyTopRank;
};

DEFINE_WND_FACTORY( KGCDungeonRankSlot );
DECLARE_WND_FACTORY( KGCDungeonRankSlot );
DECLARE_WND_FACTORY_NAME( KGCDungeonRankSlot );
