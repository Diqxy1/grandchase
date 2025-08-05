#pragma once

#include "KGCHeroDungeonInfoList.h"

class KD3DStatic;
class KD3DScrollbar;
class KD3DSizingBox;
class KGCHeroDungeonInfoList;
class KGCHeroDungeonOpentimeBox;

class KGCHeroDungeonInfo :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonInfo );
    /// Default constructor
    KGCHeroDungeonInfo( void );
    /// Default destructor
    virtual ~KGCHeroDungeonInfo( void );
    /// Copy constructor
    KGCHeroDungeonInfo( const KGCHeroDungeonInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonInfo& operator=( const KGCHeroDungeonInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        EMAX_DUNGEON_LIST_NUM = 15,
    };    

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KSafeArray<KGCHeroDungeonInfoList*, EMAX_DUNGEON_LIST_NUM>      m_pkDungeonInfoList;
    KD3DSizingBox*                  m_pkBackground;
    KD3DScrollbar*                  m_pkScrollBar;
    KGCHeroDungeonOpentimeBox*      m_pkOpentimeBox;

    std::vector<KGCHeroDungeonInfoList::KDetailDungeonInfo> m_vecHeroDungeonInfo;

    KncStopWatch        m_kStopWatch;

public:
    void OnScroll();
    int GetListViewCount(int iIndex_);
    void InitScroll();
    void SetHeroDungeonInfo( std::vector<KHeroDungeon> vecDungeonInfo_ );
    void SetSelectDungeonAllOpentime( std::pair<std::wstring, std::vector<std::wstring>> prOpentime_ );
    bool IsRenderOnOpentimeBox();
};

DEFINE_WND_FACTORY( KGCHeroDungeonInfo );
DECLARE_WND_FACTORY( KGCHeroDungeonInfo );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonInfo );