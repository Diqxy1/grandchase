#pragma once

class KGCSocialCommerceDlg;

class KGCSocialCommerceManager
{
    DeclareSingleton(KGCSocialCommerceManager);

    // 이벤트 정보
    KEVENT_SOSCIAL_COMMERCE_INFO_ACK    m_mapSocialCommerceInfo;

public:
    void SetEventInfo( IN KEVENT_SOSCIAL_COMMERCE_INFO_ACK& kRecv_ );

    KGCSocialCommerceManager(void);
    ~KGCSocialCommerceManager(void);

};
DefSingletonInline(KGCSocialCommerceManager);
