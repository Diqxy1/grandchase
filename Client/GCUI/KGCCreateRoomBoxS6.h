#pragma once

class KGCCheckBox;
class KGCRoomExtraOptionRankPanel;
class KGCRoomExtraOptionCharPanel;

class KGCCreateRoomBoxS6 : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCreateRoomBoxS6 );
    /// Default constructor
    KGCCreateRoomBoxS6( void );
    /// Default destructor
    virtual ~KGCCreateRoomBoxS6( void );
    /// Copy constructor
    KGCCreateRoomBoxS6( const KGCCreateRoomBoxS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCreateRoomBoxS6& operator=( const KGCCreateRoomBoxS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
#if defined( NATION_PHILIPPINE )
        MAX_EDIT_LEN = 26,
#else
        MAX_EDIT_LEN = 16,
#endif
        MAX_PASSWOD_EDIT_LEN = 14,
        MAX_ROOM_OPTION = 5,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void UpdateState( void );
    void RenderBtnSet( int index );
    void OnClickBtnClose( );
    void OnCreateRoom( void );
    void OnClickColorBtn( );

    void CheckPaint();

    void OnClickOpenOptionsBtn();
    void OnClickCloseOptionsBtn();
    void EnableOptionInfo(bool bEnable);
    void OnClickRankPanelSet();
    void OnClickCharPanelSet();

    KGCDungeonTitleColorS6 *m_pkColorBtnSet;

    KD3DWnd *m_pkCreateRoom;
    KD3DEdit *m_pkRoomName;
    KD3DEdit *m_pkRoomPassword;
    KD3DWnd *m_pkClose;
    KD3DWnd *m_pkPaintTitle;

    KD3DWnd* m_pkOptionCloseBtn;
    KD3DWnd* m_pkOptionOpenBtn;

    KGCCheckBox* m_pkUsePetCheck;
    KGCCheckBox* m_pkUse4SPCheck;
    KGCCheckBox* m_pkUseBallanceCheck;

    KD3DWnd* m_pkBackgroundExtended;

    KD3DWnd* m_pkRankSetBtn;
    KD3DWnd* m_pkRankModifyBtn;
    KD3DWnd* m_pkCharSetBtn;
    KD3DWnd* m_pkCharModifyBtn;

    KSafeArray<KD3DStatic*, MAX_ROOM_OPTION> m_saExtraOptions;

    KGCRoomExtraOptionRankPanel* m_pkRankPanel;
    KGCRoomExtraOptionCharPanel* m_pkCharPanel;
};

DEFINE_WND_FACTORY( KGCCreateRoomBoxS6 );
DECLARE_WND_FACTORY( KGCCreateRoomBoxS6 );
DECLARE_WND_FACTORY_NAME( KGCCreateRoomBoxS6 );