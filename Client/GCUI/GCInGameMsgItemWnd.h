#pragma once

class KD3DStatic;
class KGCUINumber;
class KGCItemImgWnd;
class KD3DSizingBox;
class KGCInGameMsgItemWnd : public KD3DDialog, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCInGameMsgItemWnd );
    /// Default constructor
    KGCInGameMsgItemWnd( void );
    /// Default destructor
    virtual ~KGCInGameMsgItemWnd( void );
    /// Copy constructor
    KGCInGameMsgItemWnd( const KGCInGameMsgItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInGameMsgItemWnd& operator=( const KGCInGameMsgItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void SetItem( GCItem* pItem, int iGrade, int iPeriod, int iCount );
    void SetETCImg( std::string strFileName );

private:
    void SetItemImg( GCItem* pItem );
    void SetGradeFrame( int iGradeID );
    void SetDuration( int iCount );
    void SetItemName( std::wstring strItemName );

public:
    GCItem              m_kItem;

public:
    KGCItemImgWnd*	    m_pkItemImg;
    KGCUINumber*        m_pkSmallDurationNum;
    KD3DStatic*         m_pkStaticName;
    KD3DSizingBox*      m_pkGradeFrame[KItem::GRADE_NUM];
};

DEFINE_WND_FACTORY( KGCInGameMsgItemWnd );
DECLARE_WND_FACTORY( KGCInGameMsgItemWnd );
DECLARE_WND_FACTORY_NAME( KGCInGameMsgItemWnd );
