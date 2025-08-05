#include "GCSocialCommerceManager.h"
#include "GCUI/GCSocialCommerceDlg.h"

ImplementSingleton(KGCSocialCommerceManager);

KGCSocialCommerceManager::KGCSocialCommerceManager(void)
{
    m_mapSocialCommerceInfo.clear();
}

KGCSocialCommerceManager::~KGCSocialCommerceManager(void)
{
}

void KGCSocialCommerceManager::SetEventInfo( IN KEVENT_SOSCIAL_COMMERCE_INFO_ACK& kRecv_ )
{
    // 데이터 저장
    m_mapSocialCommerceInfo.swap( kRecv_ );

    // UI 세팅
    g_pkUIScene->m_pkSocialCommerceDlg->SetGoodsInfo( m_mapSocialCommerceInfo );
}