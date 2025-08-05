#pragma once

class KGCRecordOptionBox;

class KGCReplayRecordAdvanceDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayRecordAdvanceDlg );
    /// Default constructor
    KGCReplayRecordAdvanceDlg( void );
    /// Default destructor
    virtual ~KGCReplayRecordAdvanceDlg( void );
    /// Copy constructor
    KGCReplayRecordAdvanceDlg( const KGCReplayRecordAdvanceDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayRecordAdvanceDlg& operator=( const KGCReplayRecordAdvanceDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreate( void );
    
    KD3DStatic* m_pkStaticResolutionInfo;
    KD3DStatic* m_pkStaticFrameInfo;
    KD3DStatic* m_pkStaticSoundInfo;
    KD3DStatic* m_pkStaticNowResolution;
    KD3DStatic* m_pkStaticNowFrame;
    KD3DStatic* m_pkStaticSE;
    KD3DStatic* m_pkStaticBGM;

    KD3DWnd*    m_pkOpenResolutionBtn;
    KD3DWnd*    m_pkOpenFrameBtn;

    KGCRecordOptionBox* m_pkResolutionOptionBox;
    KGCRecordOptionBox* m_pkFrameOptionBox;

    std::vector<std::wstring> m_vecResolutionInfo;
    std::vector<std::wstring> m_vecFrameInfo;

    KD3DWnd*    m_pkCheckboxBGM;
    KD3DWnd*    m_pkCheckboxSE;
    KD3DWnd*    m_pkCheckBGM;
    KD3DWnd*    m_pkCheckSE;

public:
    void OnOpenResolutionOption( void );
    void OnOpenFrameOption( void );

    void SetResoiutionOption( void );
    void SetFrameOption( void );

    void SetInfo( void );
    void OnCheckBGM(void );
    void OnCheckSE( void );

    void SetCommonResolutionOption(int iIndex_);
    void SetCommonFrameOption(int iIndex_);

};

DEFINE_WND_FACTORY( KGCReplayRecordAdvanceDlg );
DECLARE_WND_FACTORY( KGCReplayRecordAdvanceDlg );
DECLARE_WND_FACTORY_NAME( KGCReplayRecordAdvanceDlg );
