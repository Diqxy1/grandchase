#pragma once

#include "KNCSingleton.h"

struct RitasComboInfo{
	int	m_iObjID;
	int m_iNum;
	bool m_bClear;

	RitasComboInfo()
	{
		m_iObjID = -1;
		m_iNum = 0;
		m_bClear = false;
	}
};

class GCRitasChrismasManager
{
	DeclareSingleton( GCRitasChrismasManager );
public:
	GCRitasChrismasManager(void);
	~GCRitasChrismasManager(void);

	D3DXVECTOR2 GetInitPlayerPos()	{ return m_vInitPlayerPos; }
	
private:
	bool m_bGetInfo;
	D3DXVECTOR2 m_vInitPlayerPos;


	int                                         m_nStartCoinCount;
	int                                         m_nContinueCoinCount;
	int                                         m_nPremiumRewardCount;
	int                                         m_nMaxPoint;
	int                                         m_nSpecialRewardCount;

	int                                         m_nCurrentPoint;
	int                                         m_nCurrentStageNum;
	bool                                        m_bContinuable;
	bool                                        m_bClearLastStage;

	KRitasChristmasPlayInfo                     m_kPlayInfo;
	std::map< int, KRitasChristmasObjectInfo >  m_mapObjectInfo;
	std::map< int, KRitasChristmasStageInfo >   m_mapStageInfo;

	std::vector< KRitasChristmasRankInfo >		m_vecRankInfo;

	int											m_nMyRank;


	//ObjID,갯수,성공여부
	std::vector<RitasComboInfo>				m_vecCombo;

	//보상창 미리보기아이템들
	std::vector<GCITEMID>					m_vecRewardItemID;

public:
	enum {
		Max_COMBO_INFO = 3
	};


	bool HaveInfo()	{ return m_bGetInfo; }

	void FindMyRank();
	void ClearComboInfo()	{ m_vecCombo.clear(); }

	void SetBaseInfo(const KEVENT_RITAS_CHRISTMAS_USER_INFO_ACK& kRecv_ );
	void SetPlayInfo(const KEVENT_RITAS_CHRISTMAS_PLAY_INFO_ACK& kRecv_ );
	void SetRankInfo(const KEVENT_RITAS_CHRISTMAS_RANK_INFO_ACK& kRecv_ );
	void SetStageStartInfo(const KEVENT_RITAS_CHRISTMAS_STAGE_START_ACK& kRecv_ );
	void SetEmptyBasket(const KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_ACK& kRecv_ );
	void SetStageClearInfo(const KEVENT_RITAS_CHRISTMAS_STAGE_END_ACK& kRecv_ );

	void SetRewardInfo(KEVENT_RITAS_CHRISTMAS_GET_REWARD_ACK& kRecv_ );

	void SetRewarItemList(const KEVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_ACK& kRecv_ );


	KRitasChristmasPlayInfo*						GetPlayInfo()			{ return &m_kPlayInfo; }
	std::map< int, KRitasChristmasObjectInfo >		GetObjContainer()		{ return m_mapObjectInfo; }
	std::map< int, KRitasChristmasStageInfo >		GetStageContainer()		{ return m_mapStageInfo; }
	std::vector< KRitasChristmasRankInfo >			GetRankInfoContainer()	{ return m_vecRankInfo; }
	std::vector<RitasComboInfo>						GetComboInfo()			{ return m_vecCombo; }
	std::vector<GCITEMID>							GetRewardItemID()		{ return m_vecRewardItemID; }

	int	GetStartCoinCnt()		{ return m_nStartCoinCount; }
	int	GetMaxPoint()			{ return m_nMaxPoint; }
	int	GetPrimiumNum()			{ return m_nPremiumRewardCount; }
	int GetMyRank()				{ return m_nMyRank; }
	int GetCurrentPoint()		{ return m_nCurrentPoint; }
	bool GetClearLastStage()	{ return m_bClearLastStage; }
	bool GetCanContinue()		{ return (m_bContinuable && (m_nContinueCoinCount>0)) ; }
	int	GetSpecialNum()			{ return m_nSpecialRewardCount; }
	int GetContinuePoint()		{ return m_nContinueCoinCount; }

	void GenerateCombo(int iStageNum_ );
	void UpdateComboInfo( std::vector< int >&  vecBasket_ );
	bool IsClearedAllCombo();

	void SetCurrentPoint( int iValue_ );

	void InitTmepInfo();
    void SetContinueCoin( const KEVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK& kRecv_ );
};
DefSingletonInline( GCRitasChrismasManager );


