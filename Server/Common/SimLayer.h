#pragma once

#include <iostream>
#include "ActorManager.h"
#include "Actor.h"          // KActorPtr
#include "ActorFactory.h"   // KActorFactoryPtr
#include "KNCSingleton.h"
#include <KncLua.h>
#include "ThreadManager.h"
#include "KncException.h"
#include <boost/array.hpp>

class KSimLayer : public KThread
{
	//DeclareSingleton(KSimLayer);
	DeclToStringW;
	NiDeclareRootRTTI(KSimLayer);
	DeclareException;
protected:
	KSimLayer();
public:
	virtual ~KSimLayer();

	enum AuthType       // 060411. 인증 방식. 한번에 하나의 방식만 설정할 수 있어야 한다.
	{
		AT_NETMARBLE = 0x00000001,   // 넷마블   - 한국
		AT_GASH = 0x00000004,   // 감마니아 - 대만, 홍콩
		AT_LEVELUP = 0x00000010,   // 레벨업   - 브라질
		AT_ASIASOFT = 0x00000040,   // 아시아소프트 - 싱가폴
		AT_NTREEV = 0x00000080,   // 엔트리브 - 미국
		AT_LEVELUP_PH = 0X00000100,	// 레벨업   - 필리핀
		AT_FUNBOX_TH = 0X00000200,   // FUNBOX   - 태국
		AT_MEGAXUS = 0X00000400,   // 메가수스 - 인도네시아
		AT_LIN = 0X00000800,   // LIN      - 남미
		AT_SGI_EU = 0x00001000,   // SGI - 유럽
		AT_CYOU = 0x00002000,   // 창유- 중국
	};

	enum BillingType
	{
		BT_WEB,             // Web을 통한 빌링.
		BT_GASH,            // Gash를 통한 빌링
		BT_BR_PAYLETTER,    // PayLetter 브라질 빌링
		BT_PAYLETTER,       // PayLetter 필리핀 빌링
		BT_TH_PAYLETTER,    // PayLetter 태국 빌링
		BT_ID_PAYLETTER,    // PayLetter 인도네시아 빌링
		BT_LA_NCASH,        // NCash 남미 빌링.
		BT_CN_CYOU,         // CYOU 중국 빌링
	};

	enum {
		MAX_TICK_COUNT = 20,
	};

	virtual void Init();
	virtual void Tick();
	virtual void ShutDown();

	// derive to Child...
	virtual void RegToLua(std::shared_ptr<lua_State> L);

	// for lua.
	void AddTypeFlag(DWORD dwFlag) { m_dwServerType |= dwFlag; }
	void DeleteTypeFlag(DWORD dwFlag) { m_dwServerType &= ~dwFlag; }
	bool CheckTypeFlag(DWORD dwFlag) const { return (m_dwServerType & dwFlag) > 0; }
	DWORD GetTypeFlag() const { return m_dwServerType; }

	void AddAuthTypeFlag(DWORD dwFlag) { m_dwAuthType |= dwFlag; }
	void DeleteAuthTypeFlag(DWORD dwFlag) { m_dwAuthType &= ~dwFlag; }
	bool CheckAuthTypeFlag(DWORD dwFlag) const { return (m_dwAuthType & dwFlag) > 0; }
	const std::wstring GetAuthTypeStr(DWORD dwFlag) const;

	void SetBillingType(int iType) { m_dwBillingType = iType; }
	int GetBillingType() { return m_dwBillingType; }
	const wchar_t* GetBillingTypeStr() const;
	bool CheckBillingTypeFlag(DWORD dwFlag) { return (m_dwBillingType & dwFlag) > 0; }

	void SetCreateActorEnable(bool bEnable)
	{
		m_kActorManager.SetCreateActorEnable(bEnable);
		m_kActorManager2.SetCreateActorEnable(bEnable);
	}

protected:  // util function
	virtual void Run(); // derived from KThread
	void SetMaxElasedTick(DWORD dwElapsedTick);
	void SetAvgElapsedTick(DWORD dwElapsedTick);
	const DWORD GetElapsedTick() const;
public:
	KActorManager       m_kActorManager;    // 생성된 모든 유저들을 관리. 전체 메세지나 찾기 등에 쓰임.
	KActorManager       m_kActorManager2;

protected:
	std::vector<KThreadManager*>    m_vecpThreadMgr;
	DWORD                           m_dwServerType;
	DWORD                           m_dwAuthType;            // 060411. florist. 서버 인증 방식.
	DWORD                           m_dwBillingType;         // 060525. kkurrung. Billing을 위한 타입

	std::map< DWORD, std::wstring > m_mapAuthTypeStrings;   // 인증 타입의 문자열들.

	DWORD                           m_dwElapsedTick;        // tick 처리 소요 시간
	DWORD                           m_dwKernelTick;
	DWORD                           m_dwUserTick;
	DWORD                           m_dwMaxElapsedTick;     // tick 최대 소요 시간
	boost::array<DWORD, MAX_TICK_COUNT> m_aTickInfo;
	int                             m_nCurrentIndex;
	KncCriticalSection              m_csElapsedTick;

	DWORD                           m_dwAccumulateTick;     // 누적 tick
	int                             m_nAccumulateEvent;     // 누적 이벤트 수
};
