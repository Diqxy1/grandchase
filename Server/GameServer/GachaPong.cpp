#include "GachaPong.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "SignBoard.h"
#include "GameServer.h"
#include "NetError.h"
#include "Log4.h"

ImplementSingleton( KGachaPong );
ImplOstmOperatorW2A( KGachaPong );
NiImplementRootRTTI( KGachaPong );

ImplToStringW( KGachaPong )
{
	KLocker lock( m_csGachaPong );
	return START_TOSTRINGW
		<< TOSTRINGW( m_bNoticeEnable )
		<< TOSTRINGW( m_GachaPongDiamondCoin )
		<< TOSTRINGW( m_GachaPongVPCoin )
		<< TOSTRINGW( m_GachaPongGPCoin )
		<< TOSTRINGW( m_mapGachaPongMachine.size() );

	std::map<int, KGachaPongMachine>::const_iterator it = m_mapGachaPongMachine.begin();

	for( ; it != m_mapGachaPongMachine.end(); ++it )
	{
		it->second.m_kairoMachineCapsules.ToString(stm_);
	}
}

KGachaPong::KGachaPong()
: m_bNoticeEnable(false)
, m_bEnabled(false)
, m_GachaPongDiamondCoin(0)
, m_GachaPongVPCoin(0)
, m_GachaPongGPCoin(0)
{
}

KGachaPong::~KGachaPong()
{

}

void KGachaPong::PrintMachine(int iMachineNumber)
{
	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

	if(it == m_mapGachaPongMachine.end())
	{
		START_LOG(clog, L"Machine Info not found" )  << END_LOG;
		return;
	}

	START_LOG(clog, L"Machine Info" ) 
		<< BUILD_LOG( it->second.m_iMachineType)
		<< BUILD_LOG( it->second.m_nMainRewardRecvd)
		<< BUILD_LOG( it->second.m_MainRewardCapsule.m_dwItemID)
		<< BUILD_LOG( it->second.m_MainRewardCapsule.m_nDuration)
		<< BUILD_LOG( it->second.m_MainRewardCapsule.m_nPeriod)
		<< BUILD_LOG( it->second.m_MainRewardCapsule.m_nLevel)
		<< BUILD_LOG( it->second.m_MainRewardCapsule.m_nCount)
		<< END_LOG;

	std::stringstream strItems;

	for(int i = 0; i < (int)it->second.m_vecMachineCapsules.size(); i++ )
	{
		strItems << "{ " << it->second.m_vecMachineCapsules[i].m_dwItemID << ", " << it->second.m_vecMachineCapsules[i].m_nCount << " }\n";
	}
	START_LOG( clog, L"Capsules")
		<< BUILD_LOG( strItems.str().c_str())
		<< END_LOG;


	std::stringstream strUsers;
	std::vector<DWORD> users = m_mapSubscribedPlayer.find( iMachineNumber)->second;
	
	for( int i = 0; i < users.size(); i++ )
	{
		strUsers << "UserID: " << users[i] << std::endl;
	}

	START_LOG( clog, L"Users")
		<< BUILD_LOG( strUsers.str().c_str() )
		<< END_LOG;
}

bool KGachaPong::LoadScript()
{
	std::string script = "InitGachaPong.lua";
	return _LoadScript(script);
}

bool KGachaPong::_LoadScript( IN std::string& strScript )
{
	KLuaManager kLuaMng;

	_JIF( kLuaMng.DoFile( strScript.c_str() ) == S_OK, return false );

	_JIF( kLuaMng.GetValue( "EnableEvent", m_bEnabled ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "GachaPonNoticeEnable", m_bNoticeEnable ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "GachaPonGpCoin", m_GachaPongGPCoin ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "GachaPonVpCoin", m_GachaPongVPCoin ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "GachaPonDiamondCoin", m_GachaPongDiamondCoin ) == S_OK, return false );

	std::map<int, KGachaPongMachine> mapGachaPonMachine;
	_JIF( LoadGachaPongMachineInfo( kLuaMng, std::string("InitGachaPonMachine"), mapGachaPonMachine ) , return false );

	START_LOG(clog, L"Gacha Pong Info Successfull Loaded." ) 
		<< BUILD_LOG( mapGachaPonMachine.size())
		<< END_LOG;
	{
		KLocker lock( m_csGachaPong );
		m_mapGachaPongMachine.swap( mapGachaPonMachine );
	}

	return true;
}

bool KGachaPong::LoadGachaPongMachineInfo(  IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int,KGachaPongMachine>& mapGachaPonMachine )
{
	mapGachaPonMachine.clear();

	_JIF( kLuaMng.BeginTable( strTable.c_str() ) == S_OK, return false );

	for( int i = 1; ; ++i )
	{
		if( kLuaMng.BeginTable( i ) != S_OK ) break;
		
		KGachaPongMachine gachaPongMachine;
		gachaPongMachine.m_nMainRewardRecvd = 0;
		gachaPongMachine.m_kairoMachineCapsules.EnableNoFailure();

		_JIF( kLuaMng.GetValue( "MachineType", gachaPongMachine.m_iMachineType ) == S_OK, return false );
		_JIF( kLuaMng.GetValue( "MaxCapsule", gachaPongMachine.m_nMaxCapsule ) == S_OK, return false );
		_JIF( LoadRewardListRatio(kLuaMng, std::string("CapsuleItems"), gachaPongMachine.m_nMaxCapsule, gachaPongMachine.m_vecMachineCapsules, gachaPongMachine.m_kairoMachineCapsules, gachaPongMachine.m_MainRewardCapsule ), return false );
	
		mapGachaPonMachine.insert( std::make_pair( (i - 1), gachaPongMachine ) );

		_JIF( kLuaMng.EndTable() == S_OK, return false );
	}

	_JIF( kLuaMng.EndTable() == S_OK, return false );

	return true;
}

bool KGachaPong::LoadRewardListRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN int nMaxCapsule, OUT std::vector<KGachaPongCapsule>& vecCapsule, OUT Kairo& kKairo_, OUT KGachaPongCapsule& mainRewardCapsule)
{
	vecCapsule.clear();
	kKairo_.Clear();

	_JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
	for ( int i = 1; ; ++i ) {
		if( kLuaMng_.BeginTable( i ) != S_OK ) break;

		KGachaPongCapsule capsule;

		_JIF( kLuaMng_.GetValue( 1, capsule.m_dwItemID ) == S_OK, return false );
		_JIF( kLuaMng_.GetValue( 2, capsule.m_nDuration ) == S_OK, return false );
		_JIF( kLuaMng_.GetValue( 3, capsule.m_nPeriod ) == S_OK, return false );
		_JIF( kLuaMng_.GetValue( 4, capsule.m_nLevel ) == S_OK, return false );
		_JIF( kLuaMng_.GetValue( 5, capsule.m_nCount ) == S_OK, return false );
		_JIF( kLuaMng_.GetValue( 6, capsule.m_bMainReward ) == S_OK, return false );

		_JIF( kLuaMng_.EndTable() == S_OK, return false );

		float dropChance = ((capsule.m_nCount * 100.f) / (float)nMaxCapsule) / capsule.m_nCount;

		if( capsule.m_bMainReward )
		{
			mainRewardCapsule = capsule;

			dropChance /= 3.f;
		}

		for( int j = 0; j < capsule.m_nCount; j++ )
		{
			_JIF( kKairo_.SetEquipLevelDropItem( capsule.m_dwItemID, capsule.m_nDuration, capsule.m_nPeriod, (char)-1, (char)capsule.m_nLevel, dropChance), return false );
		}

		vecCapsule.push_back( capsule );
	}

	_JIF( kLuaMng_.EndTable() == S_OK, return false );
	return true;
}

void KGachaPong::SendGachaPongMsg(IN const std::wstring& wstrData_)
{
	if( m_bNoticeEnable == false )
	{
		return;
	}

	KSignBoardData kData;
	kData.m_dwColor         = 0L;
	kData.m_ItemID          = 0L;
	kData.m_dwSenderUID     = 0L;
	kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
	kData.m_strSenderNick.clear();
	kData.m_strMsg = wstrData_;

	std::map<DWORD, std::vector< DWORD > >::iterator it = m_mapSubscribedPlayer.begin();

	for(; it != m_mapSubscribedPlayer.end(); ++it )
	{
		if( it->second.size() > 0 )
		{
			std::vector<DWORD>::iterator vit = it->second.begin();
			for( ; vit != it->second.end(); ++vit )
			{
				START_LOG(clog, L"Info send")
					<< BUILD_LOG( *vit )
					<< BUILD_LOG( wstrData_ )
					<< END_LOG;
				SiKSignBoard()->SendDataToSpecificPlayer( *vit, kData );
			}
		}
	}
}

void KGachaPong::SubscribePlayer(DWORD dwMachineNumber, DWORD dwPlayer)
{
	KLocker lock( m_csGachaPong );
	START_LOG( clog, "New subscription")
		<< BUILD_LOG(dwMachineNumber)
		<< BUILD_LOG(dwPlayer)
		<< END_LOG;

	std::map<DWORD, std::vector<DWORD>>::iterator mit = m_mapSubscribedPlayer.find(dwMachineNumber);

	if( mit != m_mapSubscribedPlayer.end() )
	{
		mit->second.push_back(dwPlayer);
	}
	else
	{
		std::vector<DWORD> vecUsers;
		vecUsers.push_back(dwPlayer);
		m_mapSubscribedPlayer.insert( std::make_pair( dwMachineNumber, vecUsers ) );
	}
}

void KGachaPong::UnsubscribePlayer(DWORD dwPlayer)
{
	KLocker lock( m_csGachaPong );
	std::map<DWORD, std::vector< DWORD > >::iterator it = m_mapSubscribedPlayer.begin();

	for(; it != m_mapSubscribedPlayer.end(); ++it )
	{
		if( it->second.size() > 0 )
		{
			std::vector<DWORD>::iterator vit = it->second.begin();
			for( ; vit != it->second.end(); ++vit )
			{
				if( *vit == dwPlayer )
				{
					it->second.erase(vit);
					return;
				}
			}
		}
	}
}

int KGachaPong::GetMachineCapsules(int iMachineNumber)
{
	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

	if( it != m_mapGachaPongMachine.end() )
	{
		return it->second.m_kairoMachineCapsules.GetCaseNum();
	}

	return -1;
}

int KGachaPong::GetMachineMainRewardCnt(int iMachineNumber)
{
	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

	if( it != m_mapGachaPongMachine.end() )
	{
		return it->second.m_MainRewardCapsule.m_nCount - it->second.m_nMainRewardRecvd;
	}

	return -1;
}

DWORD KGachaPong::GetMachineCoin(int iMachineNumber)
{
	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

	if( it != m_mapGachaPongMachine.end() )
	{
		if( it->second.m_iMachineType == 2 )
			return m_GachaPongDiamondCoin;
		if( it->second.m_iMachineType == 1 )
			return m_GachaPongVPCoin;

		return m_GachaPongGPCoin;
	}

	return 0;
}

DWORD KGachaPong::GetMachineRewardID(int iMachineNumber)
{
	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

	if( it != m_mapGachaPongMachine.end() )
	{
		return it->second.m_MainRewardCapsule.m_dwItemID;
	}

	return -1;
}

bool KGachaPong::CrackABall(IN int iMachineNumber, OUT KDropItemInfo& kItem, OUT int& iCapsule, OUT int& nCurrentCapsule )
{
	bool bSuccess = false;

	{
		KLocker lock( m_csGachaPong );
		std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.find(iMachineNumber);

		if( it == m_mapGachaPongMachine.end() )
		{
			START_LOG(cerr, "Gacha Pon Machine not found")
				<< BUILD_LOG(iMachineNumber)
				<< END_LOG;
			return false;
		}

		kItem.m_ItemID = 0;
		do
		{
			bSuccess = it->second.m_kairoMachineCapsules.DoAndRemove( kItem, iCapsule, nCurrentCapsule );
		} while(kItem.m_ItemID == 0);
		

		if(kItem.m_ItemID == it->second.m_MainRewardCapsule.m_dwItemID)
		{
			it->second.m_nMainRewardRecvd++;
			if( it->second.m_nMainRewardRecvd == it->second.m_MainRewardCapsule.m_nCount )
			{
				it->second.ReloadMachine();
			}
		}
	}

	return bSuccess;
}

void KGachaPong::PacketAssembler(OUT KEVENT_GACHA_PONG_INFO_NOT& kPacket)
{
	if(!m_bEnabled)
	{
		return;
	}

	KLocker lock( m_csGachaPong );
	std::map<int, KGachaPongMachine>::iterator it = m_mapGachaPongMachine.begin();

	for(; it != m_mapGachaPongMachine.end(); ++it)
	{
		KGachaPongMachineInfo kGachaPongMachineInfo;

		kGachaPongMachineInfo.m_dwMachineNumber = it->first;
		kGachaPongMachineInfo.m_dwState = 0;
		kGachaPongMachineInfo.m_dwGachaPonCoin = GetMachineCoin(it->first);
		kGachaPongMachineInfo.m_kGachaPongReward.m_ItemID = it->second.m_MainRewardCapsule.m_dwItemID;

		kPacket.insert(std::make_pair(it->first, kGachaPongMachineInfo));
	}
}

std::vector<DWORD> KGachaPong::GetMachineUsers( DWORD dwMachineNumber )
{
	std::map<DWORD, std::vector< DWORD > >::iterator it = m_mapSubscribedPlayer.find(dwMachineNumber);

	if( it == m_mapSubscribedPlayer.end() )
	{
		START_LOG(cerr, L"Machine does not have players")
			<< BUILD_LOG( dwMachineNumber )
			<< END_LOG;

		return std::vector<DWORD>(); 
	}

	return it->second;
}