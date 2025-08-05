#pragma once

class KGCSavingGauge:
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCSavingGauge );
    /// Default constructor
    KGCSavingGauge( void );
    /// Default destructor
    virtual ~KGCSavingGauge( void );
    /// Copy constructor
    KGCSavingGauge( const KGCSavingGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSavingGauge& operator=( const KGCSavingGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    struct KCharSavingInfo
    {
        float fRecoveryTotalTime;
        float fRecoveryWaitTime;
        float fPenaltyRecoveryRate;
        float fPenaltyEmptyCooltime;
        void Init()
        {
            fRecoveryTotalTime = 0.f;
            fRecoveryWaitTime = 0.f;
            fPenaltyRecoveryRate = 0.f;
            fPenaltyEmptyCooltime = 0.f;
        }
    };

public:
    void SetGauge( float fAmount_ );    
    void LoadCharSavingInfo();
    void AutoRecovery();
    void InitGaugeTexCoords();
    void InitCurCharSavingInfo();

    float GetRealGaugeAmount() { return m_fReserveRealAmount; }
    float GetCooltimeGaugeAmount() { return m_fReserveCooltimeAmount; }

    void SetUseGauge( bool bUse_ );
    
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    D3DXVECTOR4 m_vCooltimeGaugeCoords; // m_pkCooltimeGauge 텍스쳐 좌표
    D3DXVECTOR4 m_vRealGaugeCoords; // m_pkRealGauge 텍스쳐 좌표

    KD3DWnd*    m_pkCooltimeGauge; // 쿨타임 공백 게이지
    KD3DWnd*    m_pkRealGauge; // 실제 게이지
    KD3DWnd*    m_pkEmptyGauge; // 모두 소진 시 표시 기능

    //KD3DWnd*    m_pkEmptyCooltimeWatch; // 게이지 모두 소진 시 패널티 쿨타임 표시위해서 일단

    KncStopWatch m_kStopWatch;

    bool m_bUse;

    bool m_bFullState; // 게이지 풀 상태인지
    bool m_bSameStateGauge; //
    float m_fReserveRealAmount;
    float m_fReserveCooltimeAmount;

    std::map< int, KCharSavingInfo > m_mapCharSavingInfo;
    KCharSavingInfo m_kCurCharSavingInfo;
};


DEFINE_WND_FACTORY( KGCSavingGauge );
DECLARE_WND_FACTORY( KGCSavingGauge );
DECLARE_WND_FACTORY_NAME( KGCSavingGauge );