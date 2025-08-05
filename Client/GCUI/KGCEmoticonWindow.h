#ifndef _GC_EMOTICON_WINDOW_H_
#define _GC_EMOTICON_WINDOW_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

#if defined( NATION_BRAZIL ) || defined( NATION_USA )
#define EMOTICON_ROW_MAX 9
#else
#define EMOTICON_ROW_MAX 7
#endif

class KD3DStatic;
class KGCEmoticonSlotRow;
class KGCEmoticon;
class KGCEmoticonWindow : public KD3DWnd,          // extends
                           public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEmoticonWindow );
    /// Default constructor
    KGCEmoticonWindow( void );
    /// Default destructor
    virtual ~KGCEmoticonWindow( void );
    /// Copy constructor
    KGCEmoticonWindow( const KGCEmoticonWindow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEmoticonWindow& operator=( const KGCEmoticonWindow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );
    void InitAll( void );
    void IncreseClickableSlotNum();
    void DecreseClickableSlotNum();
    void LockWnd( bool bLock );
    void OnShake( void );

private:
    KD3DWnd*            m_pkCloseBtn;
    KD3DWnd*            m_pkShakeBtn;
    KD3DWnd*            m_pkMaterial1;
    KD3DWnd*            m_pkMaterial2;
    KD3DWnd*            m_pkMaterial3;
    KD3DWnd*            m_pkNewEmoticon;
    KD3DWnd*            m_pkResultMsgBack;
    KD3DWnd*            m_pkMaterial_Num_1;
    KD3DWnd*            m_pkMaterial_Num_2;
    KD3DWnd*            m_pkMaterial_Num_3;
    KD3DStatic*         m_pkCubeName;
    KD3DStatic*         m_pkCubeManual;
    KD3DStatic*         m_pkResultMsg;
    KSafeArray<KGCEmoticonSlotRow*,EMOTICON_ROW_MAX> m_pkEmoticonRow;

    KSafeArray<KGCEmoticon*,4> m_pkEmoticonData;
    KSafeArray<int,4> m_iRowIndex;
    KSafeArray<int,4> m_iColumnIndex;
    int                 m_iClickedSlotNum;
    bool                m_bShakeItem;
    bool                m_bCompleteShake;
    float               m_fEffectTime;

    KD3DWnd*    m_pkSelEmoticonBtn;
    KD3DWnd*    m_pkTitleBtn;
    KD3DWnd*    m_pkPetBtn;
    KD3DWnd*    m_pkMonsterCardBtn;
};


DEFINE_WND_FACTORY( KGCEmoticonWindow );
DECLARE_WND_FACTORY( KGCEmoticonWindow );
DECLARE_WND_FACTORY_NAME( KGCEmoticonWindow );

#endif // _GC_EMOTICON_LIST_BOX_H_
