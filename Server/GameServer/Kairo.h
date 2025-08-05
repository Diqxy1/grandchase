#pragma once

#include "Lottery.h"
#include "UserPacket.h"

class Kairo
{
    DeclToStringW;

public:
    enum RewardType 
    {
        RT_NORMAL            = 1,    // 일반 아이템 지급.
        RT_STRENGTH          = 2,    // 강화석 박힌 아이템 지급.
        AT_MAX,
    };

    enum ItemGoodsID
    {
        IG_STRENGTH         = 627270,   // 강화석 ItemID
        IG_MAX,
    };

    Kairo(void);
    virtual ~Kairo(void);

    bool SetPostItem( GCITEMID nItemID, int nDuration, int nPeriod, float fProb );
    bool SetEnchantDropItem( IN GCITEMID nItemID, IN int nDuration, IN int nPeriod, IN char cGradeID, IN char cEnchantLevel, IN float fProb );
    bool SetStrengthEquipDropItem( IN GCITEMID nItemID, IN char cGradeID, IN char cEnchantLevel, IN char cEquipType, IN char cStrengthGradeID, IN char cStrengthEnchantLevel, IN float fProb );
    bool SetStrengthPeriodEquipDropItem( IN GCITEMID nItemID, IN char cGradeID, IN char cEnchantLevel, IN int nPeriod, IN char cEquipType, IN char cStrengthGradeID, IN char cStrengthEnchantLevel, IN float fProb );
    bool SetEquipLevelDropItem( IN GCITEMID nItemID, IN int nDuration, IN int nPeriod, IN char cGradeID, IN char cEquipLevel, IN float fProb );
	
    bool Do( OUT KDropItemInfo& kUnit ) const;
	bool DoAndRemove( OUT KDropItemInfo& kUnit, OUT int& nDecision , OUT int& nMaxCase );
    void GetItemList( OUT std::set<GCITEMID>& setRewardItems_ );
    void GetItemList( OUT std::vector<GCITEMID>& vecRewardItems_ );
    void GetItemList( OUT std::vector<KDropItemInfo>& vecRewardItems_ );
    DWORD GetItemNum() const { return m_mapPostItem.size(); };
    bool SetPostType( DWORD dwType, float fProb );
    bool Do( OUT DWORD& dwType ) const;
    bool GetMaxCase( IN int nExceptCaseID, OUT int& nMaxCaseID );
    bool DecreaseCaseRatio( IN int nCaseID, IN float fRatio );
    bool SetPostType( IN std::pair<int,float> prData, IN float fProb );
    int Do( OUT std::pair<int,float>& prData ) const;
    bool RemoveCase( IN int nIndexID );
    bool IncreaseCaseRatio( IN int nCaseID, IN float fRatio );
    bool Do( OUT std::vector<KDropItemInfo>& vecItem ) const;
    void SetKairoType( IN DWORD dwType );
    DWORD GetKairoType() const { return m_dwType; };

	void Clear();
	void EnableNoFailure();

    size_t GetCaseNum() const   { return m_kLottery.GetCaseNum(); }
    float GetRatio( int nIndex ){ return m_kLottery.GetRatio(nIndex); }

protected:
    std::map<int,KDropItemInfo>             m_mapPostItem;  // key : sequential id from [0..]. itemid로 하면 동일 아이템 중복이 불가능해진다.
    KLottery                                m_kLottery;
    std::map<int, DWORD>                    m_mapPostType;
    std::map<int,std::pair<int,float>>      m_mapPostPrData;
    std::map<int, std::vector<KDropItemInfo>> m_mapPostVecItem; // 강화석 장착 아이템 제공.
    DWORD                                   m_dwType; // 0 : Normal, 1 : 강화석 장착된 아이템.
};