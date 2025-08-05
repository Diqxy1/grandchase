#pragma once

enum // state
{
    AD_SHOP_NOTHING = 0,      // 알리미 없음
    AD_SHOP_NEW = 1,          // 신상품
    AD_SHOP_DC = 2,           // 할인상품
    AD_SHOP_EVENT = 3,        // 이벤트 한정 상품
    AD_SHOP_HOT = 4,          // 인기상품
};

class KGCShopAdvertising
{
private:
    std::vector< CParticleEventSeqPTR >     m_vecParticle;
    int                                     m_nADShop;           //  상점버튼에 붙일 파티클 종류
    int                                     m_nADAvatarShop;     //  코디샵버튼에 붙일 파티클 종류

public:
    KGCShopAdvertising( void );
    ~KGCShopAdvertising( void );

    KGCShopAdvertising( const KGCShopAdvertising& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCShopAdvertising& operator=( const KGCShopAdvertising& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void SetShopAdType( int num ){ m_nADShop = num; }
    void SetAvatarShopAdType( int num ){ m_nADAvatarShop = num; }
    void AddParticle( std::string sequenceName, float x = 0.0f,float y = 0.0f, float z = 0.5f, EGCRenderLayer layer = GC_LAYER_UI );
    void CreatePaticles();
    void ClearPaticles();
    void RenderParticle( bool bRender );


};

extern KGCShopAdvertising g_kShopAdvertising;