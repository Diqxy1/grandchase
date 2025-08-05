#pragma once

class KGCPlaytimeNotice : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPlaytimeNotice );
    /// Default constructor
    KGCPlaytimeNotice( void );
    /// Default destructor
    virtual ~KGCPlaytimeNotice( void );
    /// Copy constructor
    KGCPlaytimeNotice( const KGCPlaytimeNotice& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlaytimeNotice& operator=( const KGCPlaytimeNotice& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    // ALPHA에니메이션 시작 시간 = 1 * 55 
    // 전체 출력 시간  = 3 * 55
    enum { NUM_NOTICE = 2, ALPHA_START_TIME = 110, SHOW_TIME = 165 };
    enum { TIME_POS_X = 609, TIME_POS_Y = 572 };

private:    
    KSafeArray< KD3DWnd*, NUM_NOTICE >  m_apkNotice;

    int         m_iPlayTime;
    int         m_iShowTimer;
    int         m_iShowIndex;
    int         m_iShowAlpha;
    bool        m_bShow;

	KGCUINumber* m_pkTimeValue;



public:
    void StartNotice( int iIndex_, int iPlayTime_ = 0 );

private:
    void NextNotice( void );
    void StopNotice( void );

};

DEFINE_WND_FACTORY( KGCPlaytimeNotice );
DECLARE_WND_FACTORY( KGCPlaytimeNotice );
DECLARE_WND_FACTORY_NAME( KGCPlaytimeNotice );
