#pragma once
#include <Thread/Locker.h>
//#include "KLuabinder.h"
//#include <map>
//#include "GCItem.h"
#include "Packet.h"
struct KInventoryItem;
class KEnchantStrong : public lua_tinker::lua_value
{
public:
    KDeclareLuaInvoke
    KEnchantStrong(void);
    ~KEnchantStrong(void);

    enum ENCHANT_ACTION
    {
        EA_SUCCESS      = 0, // 성공
        EA_WEAKNESS     = 1, // 하락
        EA_BREAK        = 2, // 깨짐
        EA_NONE         = 3, // 제자리
        EA_DROP         = 4, // 초기화
        EA_SPECIFIC     = 5, // 구강화 주문서 강화
        EA_MAX,
    };
    enum ENCHANT_GRADE
    {
        EG_FIRST        = 0,
        EG_SECOND       = 1,
        EG_THIRD        = 2,
        EG_MAX,
    };

    void ClearEnchantLimitLv();
    void ClearEnchantAssist();

    void SetEnchantFailResult( const UINT uiLv_, const UINT uiResult_ );
    void SetCoreItemID( const DWORD dwItemID_ );
    DWORD GetCoreItemID(){ return m_dwCoreItemID; }
    void AddEnchantAssistItem( const DWORD dwItemID_, const float fIncreaseRate_ );
    UINT GetNeedCoreCount( const DWORD dwItemID_, const UINT uiEnchantLv_ );
    ENCHANT_ACTION GetEnchantFailResult( const UINT uiEnchantLv_ );
	std::wstring GetEnchantFailResultStr( const UINT uiEnchantLv_ );

	GCItemAbility GetAbility( KItem* pInvenItem_ );
	GCItemAbility GetAbility( SUserItem* pUserItem_ );
    GCItemAbility GetAbility( GCItem* pItem_, char cEnchantLevel_);

    bool IsEnchantAssistItem( const DWORD dwItemID_ );
    void RegisterLuabind();
	void SetSecondaryRatio( const float fRatio_ );

protected:
    std::map<UINT, UINT>                                                    m_mapEnchantLimitLv;    
    DWORD                                                                   m_dwCoreItemID;
    //====================================================================================
    // 2007-10-15 오후 10:36:08 : Asirion 
    // GoodsID, 상승률
    std::map< DWORD, float >                                                m_mapEnchantAssist;

	float				m_fRatio;	// 무지개 이벤트때 비쥬/젬 소모량
};
