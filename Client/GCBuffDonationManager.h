#pragma once

enum BuffDonationBuffState {
    BDBS_NONE = -1,
    BDBS_INACTIVE = 0,
    BDBS_ACTIVE = 1,
};

class GCBuffDonationManager
{
    DeclareSingleton(GCBuffDonationManager);

    GCITEMID m_dwDonationItemID;     // 기부 해야 되는 아이템이 어떤건지 알려 줌
    __int64  m_biTotalDonationPoint; // 전체 기부 수량
    __int64  m_biActiveBuffPoint;    // 버프 걸어주는 기준치
    int      m_nRecoveryChance;      // 스킬 사용 시 HP/MP회복 확률(0~100)
    int      m_nBuffStateFlag;       // 버프 상태 (-1 : 설정 안됨, 0 : 버프 해제, 1 : 버프 발동)

    bool m_bParticleNeedToRefresh;      // 게이지 파티클 갱신이 필요한가
    bool m_bRenderBuffOnParticle;       // 버프 발동 파티클을 그리고 있는가

    CParticleEventSeqPTR m_pParticleGuage;  // 게이지 파티클
    CParticleEventSeqPTR m_pParticleBead1;   // 버프 발동 파티클
    CParticleEventSeqPTR m_pParticleBead2;   // 버프 발동 파티클

public:

    GCBuffDonationManager(void);
    ~GCBuffDonationManager(void);

    void SetBuffDonationInfo( IN KEVENT_BUFF_DONATION_LOGIN_INFO_NOT& kRecv_ );
    void OnEventBuffDonationTotalDataNot( IN KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kRecv_ );
    void OnEventBuffDonationAck( IN KEVENT_BUFF_DONATION_ACK& kRecv_ );
    void Donating( IN int nCountInput );
    int GetDonationItemCount();
    bool IsBuffOn();
    void SetRandomBuff();
    void FullHPMP();
    float GetGuageCoordinateV();
    float GetGuageScale();
    void UpdateParticles();
    void ClearParticle();
    
};
DefSingletonInline(GCBuffDonationManager);
