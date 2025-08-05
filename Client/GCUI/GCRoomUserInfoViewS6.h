#pragma once

class KGCRoomUserInfoViewS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomUserInfoViewS6 );
    /// Default constructor
    KGCRoomUserInfoViewS6( void );
    /// Default destructor
    virtual ~KGCRoomUserInfoViewS6( void );
    /// Copy constructor
    KGCRoomUserInfoViewS6( const KGCRoomUserInfoViewS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomUserInfoViewS6& operator=( const KGCRoomUserInfoViewS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState(void);
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

public:
    void SetUserInfo( PLAYER* pPlayer = NULL );
    void UpdateUserInfo( PLAYER* pPlayer );
    void UpdateCharMark( PLAYER* pPlayer );
    void UpdateGrade( PLAYER* pPlayer );

public:


public:
    KD3DStatic*			m_pkID;
    KD3DStatic*			m_pkLevel;

    KD3DStatic*			m_pkWin;
    KD3DStatic*			m_pkGuildName;
    KD3DStatic*         m_pkClassName;
    KD3DStatic*         m_pkJobName;
    KD3DStatic*         m_pkCoupleName;
    KD3DStatic*         m_pkBonusText;

    KD3DWnd* m_pkExpGaugeRect;
    KD3DWnd* m_pkExpGauge;
    KD3DStatic* m_pkStaticExpValue;

    KD3DWnd* m_pkGuildIconDrawRect;

	KSafeArray<KD3DWnd*,GC_GRADE_NUM> m_pkGrade;
    
    GCImageIconTexture*			m_pkTexGuildMark;
};

DEFINE_WND_FACTORY( KGCRoomUserInfoViewS6 );
DECLARE_WND_FACTORY( KGCRoomUserInfoViewS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomUserInfoViewS6 );
