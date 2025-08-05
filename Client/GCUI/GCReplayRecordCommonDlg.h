#pragma once

class KGCFileFindDialog;

class KGCReplayRecordCommonDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayRecordCommonDlg );
    /// Default constructor
    KGCReplayRecordCommonDlg( void );
    /// Default destructor
    virtual ~KGCReplayRecordCommonDlg( void );
    /// Copy constructor
    KGCReplayRecordCommonDlg( const KGCReplayRecordCommonDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayRecordCommonDlg& operator=( const KGCReplayRecordCommonDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    enum RecordQuality
    {
        QUALITY_LOW = 0,
        QUALITY_MID,
        QUALITY_HIGH,

        NUM_QUALITY,
    };

    KD3DWnd*    m_pkQualityPlusBtn;
    KD3DWnd*    m_pkQualityMinusBth;
    KD3DWnd*    m_pkCodecLinkBtn;
    KD3DWnd*    m_pkOpenBtn;

    KSafeArray<KD3DWnd*, NUM_QUALITY>   m_apkRecordQuality;
    KSafeArray<KD3DWnd*, NUM_QUALITY>   m_apkRecordQualityActive;

    KD3DStatic* m_pkStaticControlQuality;
    KD3DStatic* m_pkStaticShotcutKeyInfo;
    KD3DStatic* m_pkStaticStartKeyInfo;
    KD3DStatic* m_pkStaticEndKeyInfo;
    KD3DStatic* m_pkStaticWarning;
    KD3DStatic* m_pkStaticCapacity;

    int m_iNowQuality;
    std::wstring m_strReplayPath;

public:
    int          GetQuality( void ) { return m_iNowQuality; }    
    std::wstring GetPath( void ) { return m_strReplayPath; }
    
    void SetQuality(int IQuality_) { m_iNowQuality = IQuality_; }
    void SetQualityBar(int iQuality_);
    void SetAdvanceOption( int iResolutionIndex_, int iFrameIndex_ );
    void SetCapacity( void );

    void OnOpenPath( void );
    void OnQualityPlus( void );
    void OnQualityMinus( void );
    void OnCodecLink(void);
};

DEFINE_WND_FACTORY( KGCReplayRecordCommonDlg );
DECLARE_WND_FACTORY( KGCReplayRecordCommonDlg );
DECLARE_WND_FACTORY_NAME( KGCReplayRecordCommonDlg );
