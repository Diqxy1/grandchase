#pragma once

#define VALENTINE_DAY_CHOCOLATE_ACCESSORY	13873
#define VALENTINE_DAY_CHOCOLATE_ACCESSORY1	50186

class PLAYER;
class GCSquarePeople;

class KGCValentineDayManager
{
	DeclareSingleton(KGCValentineDayManager);
public:
	KGCValentineDayManager(void);
	~KGCValentineDayManager(void);

private:
	DWORD m_dwPollItemID;
	DWORD m_dwPollItemCurrentCount;
	BOOL m_bPollState; // 투표 가능 기간인지
	EGCCharType m_eSeletedCharType;
	BOOL m_bPollingNow; //투표dlg가 떠있는지..
	std::wstring m_strSelectedCharString;

	std::map< DWORD, std::set< CParticleEventSeqPTR> >	m_mapParticle;
	
public:
	/*void SetPollItemID( DWORD dwItemID) { m_dwPollItemID = dwItemID; }
	DWORD GetPollItemID() { return m_dwPollItemID; }
	void SetPollItemCurrentCount( DWORD dwDuration) { m_dwPollItemCurrentCount = dwDuration; }
	DWORD GetPollItemCurrentCount() { return m_dwPollItemCurrentCount; }
	void SetPollState(BOOL bPollState) { m_bPollState = bPollState; }
	BOOL GetPollState() { return m_bPollState; }
	void SetSelectedCharType(EGCCharType eCharType){ m_eSeletedCharType = eCharType; }
	EGCCharType GetSelectedCharType() { return m_eSeletedCharType; }
	void SetPollingNow(BOOL bPolingNow) { m_bPollingNow = bPolingNow; }
	BOOL GetPollingNow() { return m_bPollingNow; }
	void SetSelectedCharString(std::wstring strCharString) { m_strSelectedCharString = strCharString; }
	std::wstring GetSelectedCharString() {return m_strSelectedCharString; }*/

	//같이있으면 이펙트 발생하도록 하자.
	void NearChocolateEffect(PLAYER* pPlayer);
	void NearChocolateEffect(GCSquarePeople* pPlayer);

	void DeleteNearChocolateEffect( DWORD dwUID );
};
DefSingletonInline(KGCValentineDayManager);
