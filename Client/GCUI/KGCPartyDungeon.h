#pragma once

class KGCPartyDungeon : public KD3DDialog,        // extends
                 public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCPartyDungeon );
    /// Default constructor
    KGCPartyDungeon( void );
    /// Default destructor
    virtual ~KGCPartyDungeon( void );
    /// Copy constructor
    KGCPartyDungeon( const KGCPartyDungeon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyDungeon& operator=( const KGCPartyDungeon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    
    enum {
        MAX_NUM = 2,
    };
    enum {
        ADD_SELECT_REGION_NUM = 6495,
    };


protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void OnCancel( void );
    void OnClickRegion( void );
    void OnClickDungeon( void );
    void ChangeLevelCategory( bool bRightMove );
    void SetDungeonString( int iRegion  = 0);
    void SetRegionString( void );
    void SetViewRegionString( int iRegion, int iStringNum);
    void SetViewDungeonString( int iDungeon, int iStringNum);
    void OnClickRegionMenu( const KActionEvent& event );
    void OnClickDungeonMenu( const KActionEvent& event );
    void ChangeLevelCategoryLeft( );
    void ChangeLevelCategoryRight( );
    void OnConfirm( void );
    void CheckChampionMode ( void );
    int GetDungeonIdx (int iRegion);
public:
    KD3DStatic* m_pkRegionSelStr;
    KD3DStatic* m_pkDungeonSelStr;
    KD3DStatic* m_pkRegionStr;
    KD3DStatic* m_pkDungeonStr;
    KD3DStatic*m_pkChampionStr;
    KD3DWnd* m_pkRegionSelect;
    KD3DWnd* m_pkDungeonSelect;
    KD3DWnd* m_pkLevelSelLeft;
    KD3DWnd* m_pkLevelSelRight;
    KD3DWnd* m_pkConfirm;
    KD3DWnd* m_pkCancel;
    KD3DWnd* m_pkClose;
    KD3DCheckBox* m_pkChampionCheckbox;
    KGCMultiMenu* m_pkRegionMenu;
    KGCMultiMenu* m_pkDungeonMenu;
    KGCChangeBox* m_pkLevelBox;

private:
    int m_iSelectedModeIndex;
    int m_iSelectedRegionIndex;
    int m_iSelectedLevel;
    int m_iDungeonMaxDifficulty;
    int m_iCheckOverlap;
};

DEFINE_WND_FACTORY( KGCPartyDungeon );
DECLARE_WND_FACTORY( KGCPartyDungeon );
DECLARE_WND_FACTORY_NAME( KGCPartyDungeon );