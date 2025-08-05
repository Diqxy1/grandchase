#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GCD3DWndEffect.h"
class KD3DStatic;
class KGCItemImgWnd;
class KGCD3DWndEffect;
class KGCNumberControl;

const int BOX_NUM = 4;

class KGCBonusBoxSelect_S4_Row : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBonusBoxSelect_S4_Row );
    /// Default constructor
    KGCBonusBoxSelect_S4_Row( void );
    /// Default destructor
    virtual ~KGCBonusBoxSelect_S4_Row( void );
    /// Copy constructor
    KGCBonusBoxSelect_S4_Row( const KGCBonusBoxSelect_S4_Row& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBonusBoxSelect_S4_Row& operator=( const KGCBonusBoxSelect_S4_Row& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& even );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    //virtual void PostChildDraw( void );

    KD3DStatic*         m_pkStaticOwner;

    KD3DSizingBox*      m_pkItemImgBackground[ BOX_NUM ];
    KD3DWnd*            m_pkBackgroundBox[ BOX_NUM ];
    KD3DWnd*            m_pkBackgroundSpliter[ BOX_NUM ];
    KD3DStatic*         m_pkStaticItemName[ BOX_NUM ];
    KGCItemImgWnd*      m_pkItemImg[ BOX_NUM ];
    KD3DWnd*            m_pkBonusBox[ BOX_NUM ];
    KD3DWnd*            m_pkBonusSPBox[ BOX_NUM ];
    bool                m_bSelected;
    bool                m_bSelectedIndex[BOX_NUM];

    KD3DWnd*            m_pkBoxNumber[ BOX_NUM ];
    KD3DWnd*            m_pkBoxNumberBack[ BOX_NUM ];

    DWORD               m_dwTimeCheck;
    DWORD               m_dwSelectLimitTime;

    int                 m_iKeyNum;
    KGCNumberControl*   m_pk10PosNum;
    KGCNumberControl*   m_pk1PosNum;

    KD3DWnd*            m_pkMyCharFace[ GC_CHAR_NUM ];
    bool                m_bDisableClick;
private:
    KRewardInfo         m_kRewardInfo;
    void ClickBonusBox( int iSelectIndex_ );
    DWORD               m_dwUID;
    bool                m_bSpecial;
    bool                m_bMine;

public:
    void SetSpecialBoxInfo( KRewardInfo kRewardInfo_ );
    bool SetMyKeyNum( int iKey_ );
    void AutoSelect( void );
    void InitRow( int iPlayerIndex, bool bMine_ );
    void InitBeforeBoxOpen( void );
    void InitAbsent( void );
    void EnableClick();
    DWORD GetOwnerUID() { return m_dwUID; }
    void SetClickedBonusBox( int iSelectIndex_ );
    void OpenBonusBoxEffect( void );
    void SetBoxType( bool bSpecial_ ) { m_bSpecial = bSpecial_; }
};

DEFINE_WND_FACTORY( KGCBonusBoxSelect_S4_Row );
DECLARE_WND_FACTORY( KGCBonusBoxSelect_S4_Row );
DECLARE_WND_FACTORY_NAME( KGCBonusBoxSelect_S4_Row );

