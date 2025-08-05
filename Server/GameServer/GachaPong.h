#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Kairo.h"

class KLuaManager;

struct KGachaPongCapsule
{
	GCITEMID	m_dwItemID;
	int			m_nDuration;
	int			m_nPeriod;
	int			m_nLevel;
	int			m_nCount;
	bool		m_bMainReward;
};

struct KGachaPongMachine
{
	int										m_iMachineType;
	int										m_nMaxCapsule;
	int										m_nMainRewardRecvd;
	Kairo									m_kairoMachineCapsules;
	KGachaPongCapsule						m_MainRewardCapsule;
	std::vector<KGachaPongCapsule>			m_vecMachineCapsules;

	void ReloadMachine()
	{
		m_nMainRewardRecvd = 0;
		m_kairoMachineCapsules.Clear();

		std::vector<KGachaPongCapsule>::iterator it = m_vecMachineCapsules.begin();
		for(; it != m_vecMachineCapsules.end(); ++it )
		{
			for( int j = 0; j < it->m_nCount; j++ )
			{
				float fDropChance = ((1.0f * 100) / (float)m_nMaxCapsule);

				if( it->m_bMainReward )
				{
					fDropChance /= 3.f;
				}

				m_kairoMachineCapsules.SetEquipLevelDropItem( it->m_dwItemID, it->m_nDuration, it->m_nPeriod, (char)-1, (char)it->m_nLevel, fDropChance );
			}
		}
	}
};

class KGachaPong
{
	DeclareSingleton( KGachaPong );
	NiDeclareRootRTTI( KGachaPong );
	DeclToStringW;

public:
	enum 
	{
		SUCCESS = 0, // 성공복권
		FAILED  = 1, // 실패복권
	};

	KGachaPong(void);
	~KGachaPong(void);

	bool LoadScript();
	bool _LoadScript( OUT std::string& strScript );

	void PrintMachine(int iMachineNumber);

	bool IsRun()
	{
		return m_bEnabled == true;
	}

	DWORD GetGachaPongNoticeEnable() 
	{ 
		KLocker lock( m_csGachaPong ); 
		return m_bNoticeEnable; 
	}

	std::vector<DWORD> GetMachineUsers( DWORD dwMachineNumber );
	int GetMachineCapsules( int iMachineNumber );
	int GetMachineMainRewardCnt( int iMachineNumber );
	DWORD GetMachineCoin( int iMachineNumber );
	DWORD GetMachineRewardID( int iMachineNumber );
	void SendGachaPongMsg( IN const std::wstring& wstrData_ );
	bool CrackABall( IN int iMachineNumber, OUT KDropItemInfo& kItem, OUT int& iCapsule, OUT int& nCurrentCapsule );
	void SubscribePlayer( DWORD dwMachineNumber, DWORD dwPlayer );
	void UnsubscribePlayer( DWORD dwPlayer );
	void PacketAssembler(OUT KEVENT_GACHA_PONG_INFO_NOT& kPacket);

protected:
	bool LoadGachaPongMachineInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int,KGachaPongMachine>& mapGachaPonMachine );
	bool LoadRewardListRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN int nMaxCapsule, OUT std::vector<KGachaPongCapsule>& vecCapsule, OUT Kairo& kKairo_, OUT KGachaPongCapsule& mainRewardCapsule);

protected:
	mutable KncCriticalSection				m_csGachaPong;
	bool									m_bNoticeEnable;
	bool									m_bEnabled;
	GCITEMID								m_GachaPongDiamondCoin;
	GCITEMID								m_GachaPongVPCoin;
	GCITEMID								m_GachaPongGPCoin;
	std::map<int, KGachaPongMachine>		m_mapGachaPongMachine;
	std::map<DWORD, std::vector< DWORD > >	m_mapSubscribedPlayer;
};

DefSingletonInline( KGachaPong );
DeclOstmOperatorA( KGachaPong );