#pragma once

enum DonationUIType
{
	DUIT_NORMAL = 0,
	DUIT_EVENT,
	DUIT_MAX
};

class KGCContributionManager
{
	DeclareSingleton(KGCContributionManager);
public:
	KGCContributionManager(void);
	~KGCContributionManager(void);

private:
	GCITEMID                                m_dwDonationItemID;     // 기부할 아이템
	DWORD                                   m_dwDonationUnit;       // 한버에 기부할 단위(수량)
	std::vector< KItem >                    m_vecDonationRewards;   // 기부 보상으로 받을 아이템들(비어있음: 보상없음)
	DWORD                                   m_dwClientUIType;       // 클라이언트 UI 타입
	std::vector< KTongDonationGoalInfo >    m_vecGoalInfo;          // 해당 NPC의 기부 단계별 수치와 스트링 번호
	int                                     m_nUserAmount;          // 해당 NPC의 개인 기부 누적량
	int                                     m_nTotalAmount;         // 해당 NPC의 전체 기부 누적량
	int                                     m_nCurrentNPCType;      // 현재 NPC 타입

public:
	//
	void OnEventTongDonationInfoAck(IN KEVENT_TONG_DONATION_INFO_ACK& kRecv_);
	void OnEventTongDonationDonateAck(IN KEVENT_TONG_DONATION_DONATE_ACK& kRecv_);
	int GetDonationMyItemCount();
	int GetDonationMyTotalCount() { return m_nUserAmount; }
	int GetDonationTotalCount() { return m_nTotalAmount; }
	const std::vector< KTongDonationGoalInfo >& GetDonationGoalInfo() { return m_vecGoalInfo; }
	GCITEMID GetDonationItemID() { return m_dwDonationItemID; }
	void Donating(int nCount);
	void SetNPCType(int nNPCType) { m_nCurrentNPCType = nNPCType; }

	//void LoadContributionEvent();

	//void LoadNewYearStringScript();
	//void SetNewYearString();
	//void SetNewContributionEvent(DWORD dwType);

	//std::wstring GetNewYearEventName(int i,int j){ return m_EventNamestring[i][j];}
	//std::wstring GetNewYearEventDesc(int i,int j){ return m_EventDescstring[i][j];}
	//CONTRIBUTION_EVENT_STATE GetNewYearEventState(int i,int j){ return m_EventState[i][j];}
	//std::vector< std::pair<DWORD, int> >  GetewYearContributionGoal(int i) { return m_pkNewYearContributionGoal[i];}

	//__int64	GetTotalContributionGP(){ return m_dwTotalContributionGP; }
	//void SetTotalContributionGP( const __int64& iContributionGP ){ m_dwTotalContributionGP = iContributionGP; }

	//__int64	GetTotalContributionNewYearPoint(DWORD Type){ return m_dwNewYearTotalContributionGP[Type]; }
	//void SetTotalContributionNewYearPoint( DWORD Type, const __int64& iContributionGP ){ m_dwNewYearTotalContributionGP[Type] = iContributionGP; }

	//int	GetMyTotalContributionGP(){ return m_iMyTotalContributionGP; }
	//void SetMyTotalContributionGP( const int& iMyContributionGP ){ m_iMyTotalContributionGP = iMyContributionGP; }

	//bool IsContributionAbleDay() { return m_iContributionState == CS_CAN; }
	//void SetIsContributionState( const int& iContributionState) { m_iContributionState = iContributionState; }

	//EventBYContributedGP* GetContributionEvent(int iIndex );
	//void SetContributionEventState();

	//void SetContributionEventState(std::vector<KItemDonationInfo> m_Info);

	//void SetNewYearContributionNPCNumber(int Number){	m_iNpcNumber = Number;}
	//int GetNewYearContributionNPCNumber(){	return m_iNpcNumber;}

	//void SetContributionRanker( const std::vector< KDotantionRank >& vecRanker )
	//{
	//	m_vecContributionRanker = vecRanker;
	//	std::sort( m_vecContributionRanker.begin(), m_vecContributionRanker.end(), KRankerSortByRanking() );
	//}
	//KDotantionRank* GetContributionRanker(int iIndex);

	//void SetContributionLocker(std::set<int> setSuccessGrade);
	//bool IsExistRanker(int iIndex);
	//bool IsExistEvent(int iIndex);
	//bool IsCompleteContributionEvent(int iIndex);
	//bool IsCompleteContributionEventLocker(int iIndex);

	//int GetInputGP() { return m_iInputGP; }
	//void SetInputGP( int iInputGP) { m_iInputGP = iInputGP; }

	std::wstring ConvertNumToCommaString(__int64 dwNumber);
	std::string ConvertNumToCommaUIString(__int64 dwNumber);

	//DWORD GetContributionUnitType() { return m_dwContributionUnitType; }
	//void SetContributionUnitType( const DWORD& dwContributionUnitType) { m_dwContributionUnitType = dwContributionUnitType;}

	//void SetMinContributionPoint( int iMinContributionPoint ) { m_iMinContributionPoint = iMinContributionPoint; }
	//int GetMinContributionPoint() { return m_iMinContributionPoint; }

	//void SetUnitTypeString(const std::wstring& strUnitTypeString) { m_strUnitTypeString = strUnitTypeString; }
	//std::wstring GetUnitTypeString() { return m_strUnitTypeString;}

	//void SetNewYearContributionInfo(std::map< DWORD, std::vector< std::pair<DWORD, int> > > pNewYearContributionInfo);
};
DefSingletonInline(KGCContributionManager);
