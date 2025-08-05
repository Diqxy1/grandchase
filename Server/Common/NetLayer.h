#pragma once

#include "Socket/Accepter.h"
#include <KncLua.h>
#include <ToString.h>
#include <RTTI.h>
#include <boost/scoped_ptr.hpp>

class KNetLayer
{
	//DeclareSingleton(KNetLayer);  // 싱글턴으로 Server, Center 모두에서 쓰일 경우, 테스트가 곤란해진다
	DeclToStringW;
	NiDeclareRootRTTI(KNetLayer);

protected:
	KNetLayer(void);  // KNetLayer의 객체가 직접적으로 생성되지 않도록 생성자의 접근 권한을 protected로 한다.
public:
	virtual ~KNetLayer(void);

	void ResetMaxData();

	// derive to child...
	virtual bool Init();
	virtual void ShutDown();
	virtual void RegToLua(std::shared_ptr<lua_State> L);

	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	void SetCheckSequenceNum(bool bCheckSequenceNum_) { KLocker lock(m_csCheckSequenceNum); m_bCheckSequenceNum = bCheckSequenceNum_; }
	bool GetCheckSequenceNum() { KLocker lock(m_csCheckSequenceNum); return m_bCheckSequenceNum; }
	void SetPacketLimitNum(UINT uiPacketAuthLimitNum_) { KLocker lock(m_csPacketAuthLimitNum); m_uiPacketAuthLimitNum = uiPacketAuthLimitNum_; }
	UINT GetPacketAuthLimitNum() { KLocker lock(m_csPacketAuthLimitNum); return m_uiPacketAuthLimitNum; }

	void SetUserPacketAttackInfo(IN bool bHackCheckEnable, IN int nPacketSendCount, IN int nLogPacketSendCount, IN DWORD dwSendCountTickGap);
	bool GetPacketHackCheckEnable() { KLocker lock(m_csUserPacketAttack); return m_bHackCheckEnable; }
	int GetPacketHackSendCount() { KLocker lock(m_csUserPacketAttack); return m_nPacketSendCount; }
	int GetLogPacketHackSendCount() { KLocker lock(m_csUserPacketAttack); return m_nLogPacketSendCount; }
	DWORD GetPacketHackTickGap() { KLocker lock(m_csUserPacketAttack); return m_dwSendCountTickGap; }
	void UpdateUserPacketAttackInfo();

public:
	unsigned short m_usPortMaster;
	unsigned short m_usPortSlave;

protected:
	std::vector<IAccepterPtr>   m_vecAccepter;

	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	mutable KncCriticalSection  m_csCheckSequenceNum;
	bool                        m_bCheckSequenceNum;
	mutable KncCriticalSection  m_csPacketAuthLimitNum;
	UINT                        m_uiPacketAuthLimitNum;

	// 130819 tgkwon. User Packet Attack 방지( 패킷 공격 방지 )
	mutable KncCriticalSection  m_csUserPacketAttack;
	bool                        m_bHackCheckEnable;
	int                         m_nPacketSendCount;
	int                         m_nLogPacketSendCount;
	DWORD                       m_dwSendCountTickGap;
};
//DefSingletonInline(KNetLayer);
