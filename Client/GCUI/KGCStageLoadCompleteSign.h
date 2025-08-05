#pragma once

class KGCStageLoadCompleteSign :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCStageLoadCompleteSign );
    /// Default constructor
    KGCStageLoadCompleteSign( void );
    /// Default destructor
    virtual ~KGCStageLoadCompleteSign( void );
    /// Copy constructor
    KGCStageLoadCompleteSign( const KGCStageLoadCompleteSign& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCStageLoadCompleteSign& operator=( const KGCStageLoadCompleteSign& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {        
        
    };

protected:

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
private:
    KSafeArray<KD3DWnd*,MAX_DUNGEON_PLAYER_NUM>         m_pkCompleteSign;
    KSafeArray<KD3DStatic*,MAX_DUNGEON_PLAYER_NUM>      m_pkUserNickName;
    KSafeArray<KD3DSizingBox*,MAX_DUNGEON_PLAYER_NUM>   m_pkBack;

    std::vector<DWORD>  m_vecPlayerUID;

public:
    void InitInfo();
    void CheckPlayerLoadComplete(  DWORD dwUID_ );
    void CheckAllPlayerLoadComplete();
    void SetUserInfo( DWORD dwUID_ );
};

DEFINE_WND_FACTORY( KGCStageLoadCompleteSign );
DECLARE_WND_FACTORY( KGCStageLoadCompleteSign );
DECLARE_WND_FACTORY_NAME( KGCStageLoadCompleteSign );