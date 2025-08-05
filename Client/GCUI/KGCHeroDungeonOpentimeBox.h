#pragma once

class KD3DStatic;
class KD3DScrollbar;
class KD3DSizingBox;

class KGCHeroDungeonOpentimeBox :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonOpentimeBox );
    /// Default constructor
    KGCHeroDungeonOpentimeBox( void );
    /// Default destructor
    virtual ~KGCHeroDungeonOpentimeBox( void );
    /// Copy constructor
    KGCHeroDungeonOpentimeBox( const KGCHeroDungeonOpentimeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonOpentimeBox& operator=( const KGCHeroDungeonOpentimeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        EMAX_OPEN_TIME_LIST_NUM = 15,
    };    

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KSafeArray<KD3DStatic*, EMAX_OPEN_TIME_LIST_NUM>      m_pkStaticOpentimeList;
    KD3DStatic*         m_pkStaticDungeonName;
    KD3DWnd*            m_pkBtnClose;
    KD3DSizingBox*      m_pkBackgroundInner;
    KD3DScrollbar*      m_pkScrollBar;

    std::pair<std::wstring, std::vector<std::wstring>> m_prAllOpentime;

public:
    void OnScroll();
    int GetListViewCount(int iIndex_);
    void InitScroll();
    void SetAllOpentime( std::pair<std::wstring, std::vector<std::wstring>> prOpentime_ );
    void OnClose();
};

DEFINE_WND_FACTORY( KGCHeroDungeonOpentimeBox );
DECLARE_WND_FACTORY( KGCHeroDungeonOpentimeBox );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonOpentimeBox );