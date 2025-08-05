#pragma once

class KGCOlympicScheduleSlot : public KD3DWnd    // extends
                                 ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicScheduleSlot );
    /// Default constructor
    KGCOlympicScheduleSlot( void );
    /// Default destructor
    virtual ~KGCOlympicScheduleSlot( void );
    /// Copy constructor
    KGCOlympicScheduleSlot( const KGCOlympicScheduleSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicScheduleSlot& operator=( const KGCOlympicScheduleSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    enum { NUM_NATION = 8 };

    KD3DStatic* m_pkStaticGameDay;
    KD3DStatic* m_pkStaticGameName;
    KD3DStatic* m_pkStaticWin;
    KD3DStatic* m_pkStaticWinNick;
    
    KSafeArray< KD3DWnd*, NUM_NATION > m_apkFlag;

private:
    int m_iNation;

public:
    void ClearScheduleSlot( void );
    void SetScheduleSlot( const std::wstring& strGameDay_, const std::wstring& strGameName_, const std::wstring& strWinNick_, const int iNation_, const bool bToday_ );

    int GetNationType( void ) { return m_iNation; }
};

DEFINE_WND_FACTORY( KGCOlympicScheduleSlot );
DECLARE_WND_FACTORY( KGCOlympicScheduleSlot );
DECLARE_WND_FACTORY_NAME( KGCOlympicScheduleSlot );
