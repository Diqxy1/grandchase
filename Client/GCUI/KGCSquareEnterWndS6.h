#pragma once

const int MAX_CHANNEL_SLOT = 7;
const int BAR_GAP = 2;
const int SQUARE_CHANNEL_CLOSE = 0;

class KGCSquareChannelBar;

class KGCSquareEnterWndS6 : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSquareEnterWndS6 );
    /// Default constructor
    KGCSquareEnterWndS6( void );
    /// Default destructor
    virtual ~KGCSquareEnterWndS6( void );
    /// Copy constructor
    KGCSquareEnterWndS6( const KGCSquareEnterWndS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSquareEnterWndS6& operator=( const KGCSquareEnterWndS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnCreateComplete();
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

public:
    void OnHighQuality();
    void OnLowQuality();
    void OnClose();
    void UpdateChannelBar();
    void SetSquareKind(int iKind_);
    void UpdateList();
protected:
    KD3DCheckBox* m_pkLowQuality;
    KD3DCheckBox* m_pkHighQuality;
    KD3DWnd*	m_pkTitleChannelNormal;
    KD3DWnd*	m_pkTitleChannelGuild;
    KD3DWnd* m_pkClose;
    KD3DStatic* m_pkLowQualityStatic;
    KD3DStatic* m_pkHighQualityStatic;

    KGCSquareChannelBar* m_pkChannelBar;
    KSafeArray<KGCSquareChannelBar*,MAX_CHANNEL_SLOT> m_pChannelBar;

    bool m_bIsGuild;
    DWORD m_dwUpdateFrame;

};

DEFINE_WND_FACTORY( KGCSquareEnterWndS6 );
DECLARE_WND_FACTORY( KGCSquareEnterWndS6 );
DECLARE_WND_FACTORY_NAME( KGCSquareEnterWndS6 );