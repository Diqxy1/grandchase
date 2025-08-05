#include "StdAfx.h"

#include "GCCoupleSystem.h"


#include "Square/GCSquare.h"
#include "Square/GCSquarePeople.h"
#include "GCUI/GCMyInfoScene.h"

SCoupleInfoSimple::SCoupleInfoSimple( const KSquareUserInfo& src_ )
{
	dwUID = src_.m_dwUID;
	dwPartnerUID = src_.m_dwCoupleUID;
	strCoupleNickName = src_.m_strPartnerNick;
	strCoupleNickColor = src_.m_strPartnerNickColor;

	const std::vector<DWORD>& vecEquip = src_.m_vecCoupleEquip;
	for( int i = 0 ; i < (int)vecEquip.size() ; ++i )
	{
		SUserItem sUseritem;
		sUseritem.cEnchantLevel = 0;
		sUseritem.iItemID = vecEquip[i]/10;
		sUseritem.dwItemUID = 0;
		vecItemInfo.push_back( sUseritem );
	}
}

SCoupleInfoSimple::SCoupleInfoSimple( const KInDoorUserInfo& src_ )
{
	dwUID = src_.m_dwUserUID;
	dwPartnerUID = src_.m_dwCoupleUID;
	strCoupleNickName = src_.m_strPartnerNick;
	strCoupleNickColor = src_.m_strPartnerNickColor;

	const std::vector<DWORD>& vecEquip = src_.m_vecCoupleEquip;
	for( int i = 0 ; i < (int)vecEquip.size() ; ++i )
	{
		SUserItem sUseritem;
		sUseritem.cEnchantLevel = 0;
		sUseritem.iItemID = vecEquip[i]/10;
		sUseritem.dwItemUID = 0;
		vecItemInfo.push_back( sUseritem );
	}
}

SCoupleInfoSimple::SCoupleInfoSimple( const SCoupleInfo& src_, bool bPartner )
{
	dwUID = bPartner?src_.dwPartnerUID:src_.dwUID;
	dwPartnerUID = bPartner?src_.dwUID:src_.dwPartnerUID;
	strCoupleNickName = bPartner?src_.strNickName:src_.strPartnerNick;
	strCoupleNickColor = bPartner ? src_.strNickName : src_.strPartnerNickColor;
	vecItemInfo = bPartner?src_.vecPartnerItem:src_.vecItemInfo;
}

SCoupleInfoSimple::SCoupleInfoSimple( const KCoupleInfo& src_, bool bPartner )
{
	dwUID = bPartner?src_.m_dwPartnerUID:src_.m_dwUID;
	dwPartnerUID = bPartner?src_.m_dwUID:src_.m_dwPartnerUID;
	strCoupleNickName = bPartner?g_kGlobalValue.m_kUserInfo.strNickName:src_.m_strPartnerNick;
	strCoupleNickColor = bPartner ? g_kGlobalValue.m_kUserInfo.strNickColor : src_.m_strPartnerNickColor;

	const std::vector<KCoupleEquip>& vecEquip = bPartner?src_.m_vecPartnerEquip:src_.m_vecMyEquip;
	for( int i = 0 ; i < (int)vecEquip.size() ; ++i )
	{
		SUserItem sUseritem;
		sUseritem.cEnchantLevel = 0;
		sUseritem.iItemID = vecEquip[i].m_dwItemID/10;
		sUseritem.dwItemUID =  vecEquip[i].m_dwItemUID;
		vecItemInfo.push_back( sUseritem );
	}
}

SCoupleInfo& SCoupleInfo::operator= ( const KCoupleInfo& _src )
{
	dwUID = _src.m_dwUID;
	dwPartnerUID = _src.m_dwPartnerUID;
	iCharType = _src.m_cCharType;
	iPartnerChar = _src.m_cPartnerCharType;
	bPosition = _src.m_tmRegDate < _src.m_tmPartnerRegDate;
	bPartnerPosition = _src.m_tmRegDate > _src.m_tmPartnerRegDate;
	strNickName = g_kGlobalValue.m_kUserInfo.strNickName;
	strNickColor = g_kGlobalValue.m_kUserInfo.strNickColor;
	strPartnerNick = _src.m_strPartnerNick;
	strPartnerNickColor = _src.m_strPartnerNickColor;
	ConvertItemList( _src.m_vecMyEquip, false );
	ConvertItemList( _src.m_vecPartnerEquip, true );
	m_tmRegDate = _src.m_tmRegDate;

	return *this;
}

void SCoupleInfo::ConvertItemList( std::vector<KCoupleEquip> vecItemList, bool bPartner )
{
	KUserItemVector& CoupleItem = (bPartner?vecPartnerItem:vecItemInfo);
    CoupleItem.clear();
	for( int i = 0 ; i < (int)vecItemList.size() ; ++i )
	{
		SUserItem sUseritem;
		vecItemList[i].m_dwItemUID =  vecItemList[i].m_dwItemUID;
		vecItemList[i].m_dwItemID /= 10;
		CoupleItem.push_back( sUseritem = vecItemList[i] );
	}
}

void SCoupleInfo::Init()
{
	dwUID = 0;
	dwPartnerUID = 0;
	iCharType = 0;
	iPartnerChar = 0;
	bPosition = true;
	bPartnerPosition = false;
	strNickName.clear();
	strNickName = L"FFFFFF";
	strPartnerNick.clear();
	strPartnerNickColor = L"FFFFFF";
	vecItemInfo.clear();
	vecPartnerItem.clear();
	m_tmRegDate;
}

ImplementSingleton( KGCCoupleSystem )

KGCCoupleSystem::KGCCoupleSystem(void)
: m_bIamSolo( true )
, m_bTodayDeny( false )
, m_eState( GCCS_SOLO )
, m_ePartnerState( GCCS_SOLO )
{
}

KGCCoupleSystem::~KGCCoupleSystem(void)
{
}

bool KGCCoupleSystem::Am_I_Solo()
{
	return m_bIamSolo;
}

DWORD KGCCoupleSystem::GetUID_of_my_honey()
{
	return m_sMyCoupleInfo.dwPartnerUID;
}

std::wstring KGCCoupleSystem::GetNickName_of_my_honey()
{
	return m_sMyCoupleInfo.strPartnerNick;
}

std::wstring KGCCoupleSystem::GetNickColor_of_my_honey()
{
	return m_sMyCoupleInfo.strPartnerNickColor;
}

bool KGCCoupleSystem::Are_you_Solo( DWORD dwUID )
{
	if( dwUID == 0 )
		return true;

	if( m_sMyCoupleInfo.dwUID == dwUID )
		return m_sMyCoupleInfo.dwPartnerUID == 0;

	if( m_mapCoupleUserInfo.find( dwUID ) == m_mapCoupleUserInfo.end() )
		return true;

	return m_mapCoupleUserInfo[dwUID].dwPartnerUID == 0;
}

std::wstring KGCCoupleSystem::GetNickName_of_your_honey( DWORD dwUID )
{
	if( m_sMyCoupleInfo.dwUID == dwUID )
		return m_sMyCoupleInfo.strPartnerNick;

	if( m_mapCoupleUserInfo.find( dwUID ) == m_mapCoupleUserInfo.end() )
		return L"";

	return m_mapCoupleUserInfo[dwUID].strCoupleNickName;
}

std::wstring KGCCoupleSystem::GetNickColor_of_your_honey(DWORD dwUID)
{
	if (m_sMyCoupleInfo.dwUID == dwUID)
		return m_sMyCoupleInfo.strPartnerNickColor;

	if (m_mapCoupleUserInfo.find(dwUID) == m_mapCoupleUserInfo.end())
		return L"FFFFFF";

	if(m_mapCoupleUserInfo[dwUID].strCoupleNickColor == L"")
		return L"FFFFFF";

	return m_mapCoupleUserInfo[dwUID].strCoupleNickColor;
}

void KGCCoupleSystem::AddCoupleUserInfo( SCoupleInfoSimple sCoupleInfo )
{
	if( sCoupleInfo.dwPartnerUID == 0 )
		return;

	if( m_mapCoupleUserInfo.find(sCoupleInfo.dwUID) != m_mapCoupleUserInfo.end() )
	{
		ASSERT(!"이미 커플 정보가 있다!!" );
	}
	m_mapCoupleUserInfo[sCoupleInfo.dwUID] = sCoupleInfo;
}

void KGCCoupleSystem::SetMyCoupleFullInfo( KCoupleInfo& kCoupleInfo )
{
	m_bIamSolo = kCoupleInfo.m_dwPartnerUID == 0;

	m_sMyCoupleInfo = kCoupleInfo;
	m_vecCoupleInven = kCoupleInfo.m_vecCoupleInven;
	for( int i = 0 ; i < (int)m_vecCoupleInven.size() ; ++i )
	{
		m_vecCoupleInven[i].m_dwID /= 10;
		m_vecCoupleInven[i].m_dwUID =  m_vecCoupleInven[i].m_dwUID;
	}

	if( m_bIamSolo == false )
		SetState( GCCS_COUPLE );
}

bool KGCCoupleSystem::Are_you_My_Honey( DWORD dwUID )
{
	if( m_bIamSolo )
	{
		return false;
	}
	else
	{
		return m_sMyCoupleInfo.dwPartnerUID == dwUID;
	}
}

void KGCCoupleSystem::SendPropose( DWORD dwPartnerUID, DWORD dwRingID, GCITEMUID dwRingUID, std::wstring strMessage )
{
	KEVENT_MAKE_COUPLE_REQ kPacket;
	kPacket.m_dwPartnerUID = dwPartnerUID;
	kPacket.m_dwRequester = g_kGlobalValue.m_kUserInfo.dwUID;
	kPacket.m_kRingItem.m_dwID =dwRingID * 10;
	kPacket.m_kRingItem.m_dwUID =  dwRingUID ;

	kPacket.m_strMsg = strMessage;
	KP2P::GetInstance()->Send_MakeCoupleReq( kPacket );

	SetState( GCCS_WAIT );
}

std::vector<KSimpleItem> KGCCoupleSystem::GetRingList( bool bInventory, bool bCoupleInventory, bool bFilterOverlap /*= false */ )
{
	std::vector<KSimpleItem> vecReturnList;
	std::set<DWORD> setOverlap;

	if( bCoupleInventory )
	{
		
		for( int i = 0 ; i < (int)m_vecCoupleInven.size() ; ++i )
		{
			if( bFilterOverlap && setOverlap.find( m_vecCoupleInven[i].m_dwID ) != setOverlap.end() )
				continue;

			GCItem* pItem = g_pItemMgr->GetItemData( m_vecCoupleInven[i].m_dwID );

			if( !(pItem->dwSlotPosition & ESP_A_FINGER ) ||
				IsEquipedCoupleItem( m_vecCoupleInven[i].m_dwID ) )
				continue;

			KSimpleItem kItem;
			kItem.m_dwID = m_vecCoupleInven[i].m_dwID;
			kItem.m_dwUID = m_vecCoupleInven[i].m_dwUID;
			vecReturnList.push_back(kItem);
			setOverlap.insert( m_vecCoupleInven[i].m_dwID );
		}
	}

	if( bInventory )
	{
		std::vector<KItem> vecRingList = g_pItemMgr->GetInventoryItemList( GIK_COUPLE_ITEM, -1, ESP_A_FINGER );
		vecReturnList.reserve(vecRingList.size());
		for( int i = 0 ; i < (int)vecRingList.size() ; ++i )
		{
			if( bFilterOverlap && setOverlap.find( vecRingList[i].m_ItemID ) != setOverlap.end() )
				continue;

			if( IsEquipedCoupleItem( vecRingList[i].m_ItemID ) )
				continue;

			KSimpleItem kItem;
			kItem.m_dwID = vecRingList[i].m_ItemID;
			kItem.m_dwUID = vecRingList[i].m_ItemUID;
			vecReturnList.push_back(kItem);
			setOverlap.insert( vecRingList[i].m_ItemID );
		}
	}

	return vecReturnList;
}

void KGCCoupleSystem::SendAccept( bool bAccept )
{
	KP2P::GetInstance()->Send_MakeCoupleAns( bAccept );
}

void KGCCoupleSystem::SendDenyToday()
{
	KP2P::GetInstance()->Send_MakeCoupleReqDeny( 0 );
}

void KGCCoupleSystem::SendDenyThisUser( DWORD dwUserUID )
{
	KP2P::GetInstance()->Send_MakeCoupleReqDeny( dwUserUID );
}

void KGCCoupleSystem::SendGoodbye( std::wstring strMsg )
{
	KP2P::GetInstance()->Send_DivorceCoupleReq( strMsg );
}

void KGCCoupleSystem::RemoveCoupleUserInfo( DWORD dwUserUID )
{
	std::map< DWORD, SCoupleInfoSimple >::iterator mit;
	if( (mit = m_mapCoupleUserInfo.find( dwUserUID )) == m_mapCoupleUserInfo.end() )
		return;

	m_mapCoupleUserInfo.erase( mit );
}

void KGCCoupleSystem::InitMyCoupleInfo()
{
	m_sMyCoupleInfo.Init();
	m_bIamSolo = true;
	SetState( GCCS_SOLO );
	SetPartnerState(GCCS_SOLO);
}

void KGCCoupleSystem::InitTempCoupleInfo()
{
	m_sTempCoupleInfo.Init();
}

std::vector<KSimpleItem> KGCCoupleSystem::GetCoupleItem( int iCol, int iRow, int iCurrentLine )
{
	std::vector<KSimpleItem> vecReturn;
	std::vector<KSimpleItem> vecInven = m_vecCoupleInven;
	_FilterEquipedItem( vecInven );

	if( iCol * iRow >= (int)vecInven.size() )
	{
		vecReturn = vecInven;
	}
	else if( ( iCol * iCurrentLine ) - (int)vecInven.size() <= iCol * iRow )
	{
		vecReturn = std::vector<KSimpleItem>( vecInven.begin() + (iCol*iCurrentLine),
										 vecInven.end() );
	}
	else 
	{
		vecReturn = std::vector<KSimpleItem>( vecInven.begin() + iCol*iCurrentLine, 
			vecInven.begin() + ( iCol*iRow + iCol+iCurrentLine) );
	}

	return vecReturn;
}

void KGCCoupleSystem::_FilterEquipedItem( std::vector<KSimpleItem>& vecItems )
{
	for( int i = 0 ; i < (int)vecItems.size() ; ++i )
	{
		if( IsEquipedCoupleItem(vecItems[i].m_dwID) == false )
			continue;

		vecItems.erase( vecItems.begin() + i );
		--i;
	}
}

int KGCCoupleSystem::GetCoupleItemNum()
{
	return (int)m_vecCoupleInven.size();
}

namespace{
	DWORD g_dwItemID = 0;
	GCITEMUID g_dwItemUID = 0;
	bool g_bEquipDirect = 0;
}

void KGCCoupleSystem::SendInsertToInven()
{
	KP2P::GetInstance()->Send_InsertCoupleInven( g_dwItemID, g_dwItemUID, g_bEquipDirect );
}

void KGCCoupleSystem::ReadyToInsertInven( DWORD dwItemID, GCITEMUID dwItemUID, bool bEquipDirect /*= true */ )
{
	g_dwItemID = dwItemID;
	g_dwItemUID = dwItemUID;
	g_bEquipDirect = bEquipDirect;
}

void KGCCoupleSystem::ClearCoupleUserInfo()
{
	m_mapCoupleUserInfo.clear();
}

void KGCCoupleSystem::SendEquipItemChange( KSimpleItem kItem )
{
	//반지는 착용하는게 아냐~
	if( g_pItemMgr->GetItemData( kItem.m_dwID )->dwSlotPosition & ESP_A_FINGER )
		return;

	// 있는 아이템은 빼고 없는 아이템은 넣는다.
	m_MyEquip = m_sMyCoupleInfo.vecItemInfo;
	bool bEquiped = false;
	DWORD dwCheckOverlap = 0;

	GCItem* pkEquip = g_pItemMgr->GetItemData( kItem.m_dwID );
	if( pkEquip == NULL )
	{
		ASSERT( !"아쒸 장비 없잖아!!!");
		return;
	}
	for( int i = 0 ; i < (int)m_MyEquip.size() ; ++i )
	{
		GCItem* pkEquiped = g_pItemMgr->GetItemData( m_MyEquip[i].iItemID );

		if( !(dwCheckOverlap & pkEquip->dwSlotPosition) )
		{
			dwCheckOverlap|=pkEquiped->dwSlotPosition;
		}
		else
		{
			m_MyEquip.erase( m_MyEquip.begin() + i );
			continue;
		}

		if( m_MyEquip[i].iItemID == kItem.m_dwID )
		{
			m_MyEquip.erase( m_MyEquip.begin() + i );
			bEquiped = true;
			break;
		}

		if(	pkEquip->dwSlotPosition &  pkEquiped->dwSlotPosition )
		{
			dwCheckOverlap |= pkEquip->dwSlotPosition;
			m_MyEquip.erase( m_MyEquip.begin() + i );
			break;
		}
	}
	if( bEquiped == false )
	{
		SUserItem sEquip;
		sEquip.cEnchantLevel = -1;
		sEquip.dwItemUID = kItem.m_dwUID;
		sEquip.iItemID = kItem.m_dwID;
		m_MyEquip.push_back( sEquip );
	}
	_SendEquipItemChange();
}

void KGCCoupleSystem::SendEquipItemChange( DWORD dwItemID )
{
	//반지는 착용하는게 아냐~
	if( g_pItemMgr->GetItemData( dwItemID )->dwSlotPosition & ESP_A_FINGER )
		return;

	for( int i = 0 ; i < (int)m_sMyCoupleInfo.vecItemInfo.size() ; ++i )
	{
		if( m_sMyCoupleInfo.vecItemInfo[i].iItemID == dwItemID )
		{
			KSimpleItem kItem;
			kItem.m_cItemType = 0;
			kItem.m_dwID = m_sMyCoupleInfo.vecItemInfo[i].iItemID;
			kItem.m_dwUID = m_sMyCoupleInfo.vecItemInfo[i].dwItemUID;
			SendEquipItemChange( kItem );
			break;
		}
	}
}

void KGCCoupleSystem::ApplyEquipItemChange()
{
	m_sMyCoupleInfo.vecItemInfo = m_MyEquip;
}

KUserItemVector KGCCoupleSystem::GetEquipItem( bool bRight, bool bMyCouple )
{
	SCoupleInfo& Couple = bMyCouple?m_sMyCoupleInfo:m_sTempCoupleInfo;

	KUserItemVector vecEquip;
	if( Couple.bPosition == bRight )
	{
		vecEquip = Couple.vecItemInfo;
		_EquipBaseItem( Couple.iCharType, vecEquip );
		return vecEquip;
	}
	else if( Couple.bPartnerPosition == bRight )
	{
		vecEquip = Couple.vecPartnerItem;
		_EquipBaseItem( Couple.iPartnerChar, vecEquip );
		return vecEquip;
	}

	ASSERT( !"o미 커플이 같은 방향에 서 있음... 버그닷!!" );
	vecEquip = Couple.vecItemInfo;
	_EquipBaseItem( m_sMyCoupleInfo.iCharType, vecEquip );
	return vecEquip;
}

void KGCCoupleSystem::AddItemToInven( const KSimpleItem& kItem )
{
	KSimpleItem kTemp = kItem;
	kTemp.m_dwID /= 10;
	kTemp.m_dwUID =  kItem.m_dwUID;
	m_vecCoupleInven.push_back( kTemp );

	//창비창 UI를 갱신해 주자!!
	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCCoupleSystem::_EquipBaseItem( int iCharType, KUserItemVector& vecItem, bool bForceEquipAll /*= false */ )
{
	DWORD dwFlag = 0;
	GCItem* ItemInfo = NULL;

	for( int i = 0; i < (int)vecItem.size(); i++ )
	{
		dwFlag |= g_pItemMgr->GetItemData( vecItem[i].iItemID )->dwSlotPosition;
	}

	SUserItem sItem;
	sItem.dwItemUID = 0;
	sItem.cEnchantLevel = -1;
	if ( !( dwFlag & ESP_HELMET ) )// 머리가 없다면 기본 머리를 넣는다.
	{
		sItem.iItemID = g_pItemMgr->GetBaseItemID( iCharType, ESP_HELMET );
		vecItem.push_back( sItem );
	}
	if ( !( dwFlag & ESP_JACKET ) )// 상의가 없다면 기본 상의를 넣는다.
	{
		sItem.iItemID = g_pItemMgr->GetBaseItemID( iCharType, ESP_JACKET );
		vecItem.push_back( sItem );
	}
	if ( !( dwFlag & ESP_PANTS ) )// 하의가 없다면 기본 하의를 넣는다.
	{
		sItem.iItemID = g_pItemMgr->GetBaseItemID( iCharType, ESP_PANTS );
		vecItem.push_back( sItem );
	}
}

KUserItemVector KGCCoupleSystem::_FilterBaseItem( const KUserItemVector& vecItem )
{
	KUserItemVector vecReturn;
	for( int i = 0 ; i < (int)vecItem.size() ; ++i )
	{
		if( g_pItemMgr->IsBaseItem( vecItem[i].iItemID ) == true )
			continue;

		vecReturn.push_back( vecItem[i] );
	}
	return vecReturn;
}

void KGCCoupleSystem::SendCharacterChange( int iCharType )
{
	KP2P::GetInstance()->Send_ChangeCoupleCharType( iCharType );
}

void KGCCoupleSystem::RecvEquipItemChange( const std::vector<DWORD>& vecItem )
{
	m_sMyCoupleInfo.vecPartnerItem.clear();
	for( int i = 0 ; i < (int)vecItem.size() ; ++i )
	{
		SUserItem sItem;
		sItem.cEnchantLevel = -1;
		sItem.dwItemUID = 0;
		sItem.iItemID = vecItem[i]/10;
		m_sMyCoupleInfo.vecPartnerItem.push_back( sItem );
	}
}

KUserItemVector KGCCoupleSystem::GetMyEquipItem()
{
	return GetEquipItem( m_sMyCoupleInfo.bPosition, true );
}

void KGCCoupleSystem::RecvChangeCharacter( int iCharType , bool bPartners )
{
	(!bPartners?m_sMyCoupleInfo.iCharType:m_sMyCoupleInfo.iPartnerChar) = iCharType;
}

int KGCCoupleSystem::GetCoupleCharacter( bool bMyCouple, bool bPartner )
{
	SCoupleInfo& Couple = (bMyCouple?m_sMyCoupleInfo:m_sTempCoupleInfo);
	return bPartner?Couple.iPartnerChar:Couple.iCharType;
}

bool KGCCoupleSystem::GetPosition( bool bMyCouple )
{
	return (bMyCouple?m_sMyCoupleInfo:m_sTempCoupleInfo).bPosition;
}

int KGCCoupleSystem::GetMyCoupleDay()
{
	CTimeSpan cSpan =  g_kGlobalValue.m_tmServerTime - CTime( m_sMyCoupleInfo.m_tmRegDate + g_kGlobalValue.m_tmServerSyncTime);
	return (int)cSpan.GetDays() + 1;
}

int KGCCoupleSystem::GetYourCoupleDay()
{
	CTimeSpan cSpan = g_kGlobalValue.m_tmServerTime - CTime( m_sTempCoupleInfo.m_tmRegDate+ g_kGlobalValue.m_tmServerSyncTime );
	return (int)cSpan.GetDays() + 1;
}

DWORD KGCCoupleSystem::GetRingItemID( bool bMyCouple )
{
	SCoupleInfo& Couple = bMyCouple?m_sMyCoupleInfo:m_sTempCoupleInfo;

    // 내꺼
    for( KUserItemVector::const_iterator itMy = Couple.vecItemInfo.begin() ; itMy != Couple.vecItemInfo.end() ; ++itMy ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( itMy->iItemID );
        if( pItem && ( pItem->dwSlotPosition & ESP_A_FINGER ) ) { 
            return pItem->dwGoodsID;
        }
    }

    // 파트너꺼 
    for( KUserItemVector::const_iterator itPartner = Couple.vecPartnerItem.begin() ; itPartner != Couple.vecPartnerItem.end() ; ++itPartner ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( itPartner->iItemID );
        if( pItem && ( pItem->dwSlotPosition & ESP_A_FINGER ) ) { 
            return pItem->dwGoodsID;
        }
    }

	START_LOG( clog, "반지가 없다!!!" );
	return 0;
}

void KGCCoupleSystem::SetTempCoupleFullInfo( SCoupleInfo& sCoupleInfo )
{
	m_sTempCoupleInfo = sCoupleInfo;
}

bool KGCCoupleSystem::GetTempPosition()
{
	return m_sTempCoupleInfo.bPosition;
}

std::wstring KGCCoupleSystem::GetNickName( bool bMyCouple, bool bPartner )
{
	SCoupleInfo& Couple = bMyCouple?m_sMyCoupleInfo:m_sTempCoupleInfo;

	return bPartner?L"#c" + Couple.strPartnerNickColor + Couple.strPartnerNick + L"#cX" : L"#c" + Couple.strNickColor + Couple.strNickName + L"#cX";
}

void KGCCoupleSystem::RingAbility( PLAYER* pPlayer, short& shAttack, short& shDefence, short& shHpInc )
{
	if( !pPlayer->IsLocalPlayer() || m_bIamSolo )
		return;

	int iLevel = pPlayer->GetCurrentChar().iLevel;
	GCItem* pItem = g_pItemMgr->GetItemData( GetRingItemID( true ) );

	shAttack += pItem->iAttack;
	shDefence += pItem->iDefence;
	shHpInc += pItem->iHealth;

	if( pItem->iLimitedLevelOfLevelRatioAbility > 0 )
	{
		if( iLevel > pItem->iLimitedLevelOfLevelRatioAbility )
			iLevel = pItem->iLimitedLevelOfLevelRatioAbility;

		shAttack += (short)(iLevel * pItem->fLevelRatioAttackRate);
		shDefence += (short)(iLevel * pItem->fLevelRatioDefenceRate);
		shHpInc += (short)(iLevel * pItem->fLevelRatioHealthRate);
	}
}

bool KGCCoupleSystem::IsInCoupleInven( GCITEMUID dwItemUID )
{
	for( int i = 0 ; i < (int)m_vecCoupleInven.size() ; ++i )
	{
		if( m_vecCoupleInven[i].m_dwUID == dwItemUID )
			return true;
	}
	return false;
}

bool KGCCoupleSystem::IsEquipedCoupleItem( DWORD dwItemID )
{
	for( int i = 0 ; i < (int)m_sMyCoupleInfo.vecItemInfo.size() ; ++i )
	{
		if( m_sMyCoupleInfo.vecItemInfo[i].iItemID == dwItemID )
			return true;
	}
	return false;
}

void KGCCoupleSystem::SendRingChange( DWORD dwItemID, GCITEMUID dwItemUID, std::wstring strMessage )
{
	KP2P::GetInstance()->Send_ChangeCoupleRingReq( dwItemID , dwItemUID, strMessage, !IsInCoupleInven( dwItemUID ) );
}

void KGCCoupleSystem::SendRingAccept( bool bAccept )
{
	KP2P::GetInstance()->Send_ChangeCoupleRingAns( bAccept );
}

void KGCCoupleSystem::RecvExpiredItemList( const std::vector<KSimpleItem>& vecItems )
{
	for( int i = 0 ; i < (int)vecItems.size() ; ++i )
	{
		_RemoveItemInCoupleInven( vecItems[i].m_dwUID );
		_UnEquipItem( vecItems[i].m_dwID );
		g_pItemMgr->m_kInventory.RemoveItem( vecItems[i].m_dwUID );
	}
	g_pItemMgr->UpdateUserItem( &g_kGlobalValue.m_kUserInfo );
	m_MyEquip = m_sMyCoupleInfo.vecItemInfo;
	_SendEquipItemChange();
}

void KGCCoupleSystem::_RemoveItemInCoupleInven( GCITEMUID dwUID )
{
	for( int i = 0 ; i < (int)m_vecCoupleInven.size() ; ++i )
	{
		if( m_vecCoupleInven[i].m_dwUID == dwUID )
		{
			m_vecCoupleInven.erase( m_vecCoupleInven.begin() + i );
			break;
		}
	}
}

void KGCCoupleSystem::_UnEquipItem( DWORD dwItemID )
{
	KUserItemVector& Equip = m_sMyCoupleInfo.vecItemInfo;
    std::vector<SUserItem>::iterator vit;
    
    
	for( int i = 0 ; i < (int)Equip.size() ; ++i )
	{
        int nEquipItemID = Equip[i].iItemID * 10;
		if( nEquipItemID == dwItemID )
		{
            vit = std::find_if( m_sMyCoupleInfo.vecPartnerItem.begin(), m_sMyCoupleInfo.vecPartnerItem.end(),
                boost::BOOST_BIND( &SUserItem::iItemID, _1 ) == Equip[i].iItemID );
            if ( vit != m_sMyCoupleInfo.vecPartnerItem.end() ) {
                m_sMyCoupleInfo.vecPartnerItem.erase( vit );
            }
			Equip.erase( Equip.begin() + i );
			break;
		}
	}
}

void KGCCoupleSystem::_SendEquipItemChange()
{
	std::vector<DWORD> vecEquip;
	for( int i = 0 ; i < (int)m_MyEquip.size() ; ++i )
	{
		vecEquip.push_back( m_MyEquip[i].iItemID * 10 );
	}

	KP2P::GetInstance()->Send_ChangeCoupleEquip( vecEquip );
}

DWORD KGCCoupleSystem::GetCoupleUID( DWORD dwUID )
{
	if( dwUID == g_kGlobalValue.m_kUserInfo.dwUID && Am_I_Solo() == false )
		return m_sMyCoupleInfo.dwPartnerUID;

	if( m_mapCoupleUserInfo.find( dwUID ) == m_mapCoupleUserInfo.end() )
		return 0;

	return m_mapCoupleUserInfo[dwUID].dwPartnerUID;
}

bool KGCCoupleSystem::CheckCoupleWithMe( DWORD dwUID, bool bSameTeam )
{
	if( bSameTeam == false )
		return m_mapCoupleUserInfo.find( GetCoupleUID(dwUID) ) != m_mapCoupleUserInfo.end();

	if( m_mapCoupleUserInfo.find( GetCoupleUID(dwUID) ) == m_mapCoupleUserInfo.end() )
		return false;

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
		if( pPlayer->m_kUserInfo.bLive == false )
			continue;

		if( pPlayer->m_kUserInfo.dwUID != GetCoupleUID(dwUID) )
			continue;

		return g_MyD3D->GetMyPlayer()->m_kUserInfo.iTeam == pPlayer->m_kUserInfo.iTeam;
	}
	return false;
}

bool KGCCoupleSystem::IsCoupleDay()
{
	if( GetMyCoupleDay() < 100 )
		return false;
	return GetMyCoupleDay() % 100 == 0;
}

void KGCCoupleSystem::NearCoupleEffect( PLAYER* pPlayer )
{
	DWORD dwMyUID = pPlayer->m_kUserInfo.dwUID;
	if( pPlayer->m_kUserInfo.bLive == false || PS_DEAD == pPlayer->m_ePlayerState)
	{
		_DeleteNearEffect( dwMyUID );
		return;
	}

	DWORD dwPartnerUID = GetCoupleUID( dwMyUID );
	if( dwPartnerUID == 0 )
	{
		_DeleteNearEffect( dwMyUID );
		return;
	}

	if( GetCoupleUID( dwPartnerUID ) == 0 )
	{
		_DeleteNearEffect( dwMyUID );
		_DeleteNearEffect( dwPartnerUID );
		return;
	}

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false ) ||
			( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwMyUID ) ||
			( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID != dwPartnerUID )
			)
			continue;

		D3DXVECTOR3 vMyPos = pPlayer->GetPosition();
		D3DXVECTOR3 vPartnerPos = g_MyD3D->MyPlayer[i]->GetPosition();

		if( ASMfabs(vMyPos.x - vPartnerPos.x) > 0.5f ||
			ASMfabs(vMyPos.y - vPartnerPos.y) > 0.5f ||
			g_MyD3D->MyPlayer[i]->m_cLife <= 0 )
		{
			_DeleteNearEffect( dwMyUID );
		}
		else
		{
            if( m_mapParticle.find( dwMyUID ) != m_mapParticle.end() && m_mapParticle.size() != 0 )
                continue;

			DWORD dwRingID = GetRingID( dwMyUID );
			if( dwRingID == 0 )
				continue;

			GCItem* pItem = g_pItemMgr->GetItemData(dwRingID);
			if( pItem->eMoneyType == EMT_CASH )
			{
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("Couple_Light_1", 0.0f, 0.0f) );
                m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("Couple_Light_2", 0.0f, 0.0f) );
                m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("Couple_heart_Big_1", 0.0f, 0.0f) );
                m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("Couple_heart_Big_2", 0.0f, 0.0f) );				
			}
			else
			{
                m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("couple_heart_1", 0.0f, 0.0f) );
                m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle("couple_heart_2", 0.0f, 0.0f) );
			}
		}
	}
}

void KGCCoupleSystem::NearCoupleEffect( GCSquarePeople* pPlayer )
{
	// 나간놈들을 다 빼주자.
	std::map< DWORD, std::set< CParticleEventSeqPTR> >::iterator mit;
	std::vector< DWORD > vecEraseList;
	for( mit = m_mapParticle.begin() ; mit != m_mapParticle.end() ; ++mit )
	{
		if( GetCoupleUID( mit->first ) != 0 )
			continue;

		vecEraseList.push_back( mit->first );
	}
	for( int i = 0 ; i < (int)vecEraseList.size() ; ++i )
	{
		_DeleteNearEffect( vecEraseList[i] );
		m_mapParticle.erase( vecEraseList[i] );
	}

	DWORD dwMyUID = pPlayer->GetUID();
	DWORD dwPartnerUID = GetCoupleUID( dwMyUID );
	if( dwPartnerUID == 0 )
	{
		_DeleteNearEffect( dwMyUID );
		return;
	}

	GCSquarePeople* pPartner = SiGCSquare()->GetSquarePeople( dwPartnerUID );

	if( pPartner == NULL )
	{
		_DeleteNearEffect( dwMyUID );
		return;
	}

	D3DXVECTOR3 vMyPos = pPlayer->GetPos();
	D3DXVECTOR3 vPartnerPos = pPartner->GetPos();

	if( ASMfabs(vMyPos.x - vPartnerPos.x) > 0.5f ||
		ASMfabs(vMyPos.y - vPartnerPos.y) > 0.5f )
	{
		_DeleteNearEffect( dwMyUID );
	}
	else
	{
		while( true )
		{
			if( m_mapParticle.find( dwMyUID ) != m_mapParticle.end()&& m_mapParticle.size() != 0 )
				break;

			DWORD dwRingID = GetRingID( dwMyUID );
			if( dwRingID == 0 )
				break;

			GCItem* pItem = g_pItemMgr->GetItemData(dwRingID);
			if( pItem->eMoneyType == EMT_CASH )
			{
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "Couple_Light_1", 0.0f, 0.0f ) );
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "Couple_Light_2", 0.0f, 0.0f ) );
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "Couple_heart_Big_1", 0.0f, 0.0f ) );
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "Couple_heart_Big_2", 0.0f, 0.0f ) );
			}
			else
			{
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "couple_heart_1", 0.0f, 0.0f ) );
				m_mapParticle[dwMyUID].insert( pPlayer->AddTraceParticle( "couple_heart_2", 0.0f, 0.0f ) );
			}
		}
	}
}

void KGCCoupleSystem::_DeleteNearEffect( DWORD dwUID )
{
	if( m_mapParticle.find( dwUID ) != m_mapParticle.end() )
	{
		std::set< CParticleEventSeqPTR>::iterator sit;
		for( sit = m_mapParticle[dwUID].begin() ; sit != m_mapParticle[dwUID].end() ; ++sit )
		{
			g_ParticleManager->DeleteSequence( (*sit) );
		}
		m_mapParticle.erase( dwUID );
	}
}

DWORD KGCCoupleSystem::GetRingID( DWORD dwUserUID )
{
	if( m_sMyCoupleInfo.dwUID == dwUserUID ||
		m_sMyCoupleInfo.dwPartnerUID == dwUserUID )
	{
		KUserItemVector& vecItemInfo = m_sMyCoupleInfo.vecItemInfo;
		for( int i = 0 ; i < (int)vecItemInfo.size() ; ++i )
		{
			GCItem* pItem = g_pItemMgr->GetItemData( vecItemInfo[i].iItemID );

			if( pItem->dwSlotPosition & ESP_A_FINGER )
				return pItem->dwGoodsID;
		}
		return 0;
	}

	std::map< DWORD, SCoupleInfoSimple >::iterator mit = m_mapCoupleUserInfo.find( dwUserUID );
	if( mit == m_mapCoupleUserInfo.end() )
		return 0;

	KUserItemVector& vecItemInfo = m_mapCoupleUserInfo[dwUserUID].vecItemInfo;
	for( int i = 0 ; i < (int)vecItemInfo.size() ; ++i )
	{
		GCItem* pItem = g_pItemMgr->GetItemData( vecItemInfo[i].iItemID );

		if( pItem->dwSlotPosition & ESP_A_FINGER )
			return pItem->dwGoodsID;
	}
	return 0;
}

bool KGCCoupleSystem::GetCoupleItemVector( DWORD dwUserUID, IN KUserItemVector** ppVecCoupleItem_ )
{
	if( m_sMyCoupleInfo.dwUID == dwUserUID ||
		m_sMyCoupleInfo.dwPartnerUID == dwUserUID )
	{
		*ppVecCoupleItem_ = &(m_sMyCoupleInfo.vecItemInfo);
        return true;
	}

	std::map< DWORD, SCoupleInfoSimple >::iterator mit = m_mapCoupleUserInfo.find( dwUserUID );
	if( mit != m_mapCoupleUserInfo.end() )
    {
        *ppVecCoupleItem_ = &(mit->second.vecItemInfo);
		return true;
    }

    *ppVecCoupleItem_ = NULL;
	return false;
}

void KGCCoupleSystem::RecvChangeCoupleRing( const KSimpleItem& kItem, bool bInCoupleInven )
{
	KUserItemVector& vecItemInfo = m_sMyCoupleInfo.vecItemInfo;

	_UnEquipItem( GetRingID( m_sMyCoupleInfo.dwUID ) * 10 );

	SUserItem sItem;
	sItem.iItemID = kItem.m_dwID / 10;
	sItem.dwItemUID =  kItem.m_dwUID;
	vecItemInfo.push_back( sItem );

	if( bInCoupleInven == false )
	{
		g_pItemMgr->m_kInventory.RemoveItem( sItem.dwItemUID );
	}
}

void KGCCoupleSystem::RecvCoupleChat( std::wstring strMessage )
{
	if(!m_qCoupleChat.empty())
		m_qCoupleChat.pop();

	m_qCoupleChat.push( strMessage );
}

bool KGCCoupleSystem::IsCoupleChatExist()
{
	return !m_qCoupleChat.empty();
}

std::wstring KGCCoupleSystem::GetCoupleChat()
{
	std::wstring strMsg = m_qCoupleChat.front();
	m_qCoupleChat.pop();
	return strMsg;
}

void KGCCoupleSystem::SetState( GCCoupleState eState )
{
	 m_eState = eState;

	 if( m_bIamSolo == true )
		 return;

	 KP2P::GetInstance()->Send_CoupleWindowStateReq( (int)m_eState );
}

std::pair<bool, SCoupleItemInfo> KGCCoupleSystem::CoupleItemScript( int iItemID )
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "CoupleItem.stg" ) == false )
		return std::make_pair( false, SCoupleItemInfo() );

	SCoupleItemInfo sCoupleItem;
	LUA_BEGIN_TABLE( "CoupleItem", return std::make_pair( false, SCoupleItemInfo() ) )
	{
		do {
			LUA_BEGIN_TABLE( iItemID, return std::make_pair( false, SCoupleItemInfo() ) )
			{
				sCoupleItem.iItemID = iItemID;

				if( SUCCEEDED( luaMgr.BeginTable( "Particle" ) ) )
				{
					for( int i = 1;; ++i )
					{
						LUA_BEGIN_TABLE(i, break )
						{
							SCoupleParticle sParticle;
							LUA_GET_VALUE( 1, sParticle.strName, continue );
							LUA_GET_VALUE( 2, sParticle.vPos.x, continue );
							LUA_GET_VALUE( 3, sParticle.vPos.y, continue );
							LUA_GET_VALUE( 4, sParticle.vPos.z, continue );
							LUA_GET_VALUE( 5, sParticle.iLayer, continue );
							sCoupleItem.vecParticle.push_back( sParticle );
						}
						LUA_END_TABLE( break );
					}
					luaMgr.EndTable();
				}

				if( SUCCEEDED( luaMgr.BeginTable( "Mesh" ) ) )
				{
					for( int i = 1;; ++i )
					{
						LUA_BEGIN_TABLE(i, break )
						{
							SCoupleMesh sMesh;
							LUA_GET_VALUE( 1, sMesh.strMesh, continue );
							LUA_GET_VALUE( 2, sMesh.strTexture, continue );
							LUA_GET_VALUE( 3, sMesh.vTranslation.x, continue );
							LUA_GET_VALUE( 4, sMesh.vTranslation.y, continue );
							LUA_GET_VALUE( 5, sMesh.vTranslation.z, continue );
							LUA_GET_VALUE( 6, sMesh.vScale.x, continue );
							LUA_GET_VALUE( 7, sMesh.vScale.y, continue );
							LUA_GET_VALUE( 8, sMesh.vScale.z, continue );
							LUA_GET_VALUE( 9, sMesh.vRotation.x, continue );
							LUA_GET_VALUE( 10, sMesh.vRotation.y, continue );
							LUA_GET_VALUE( 11, sMesh.vRotation.z, continue );
							sCoupleItem.vecMesh.push_back( sMesh );
						}
						LUA_END_TABLE( break )
					}
					luaMgr.EndTable();
				}

				if( SUCCEEDED( luaMgr.BeginTable( "BackGround" ) ) )
				{
					SCoupleBG& sBG = sCoupleItem.sBackground;
					LUA_GET_VALUE( 1, sBG.strMesh, continue );
					LUA_GET_VALUE( 2, sBG.strTexture, continue );
					luaMgr.EndTable();
				}
			}
			LUA_END_TABLE(  break )
		} while (false);
	}
	LUA_END_TABLE( ASSERT(!"Table End Error" ) )

	return std::make_pair( true, sCoupleItem );
}

void KGCCoupleSystem::FixEquipItemChange( const std::vector<KCoupleEquip>& vecEquipList )
{
	m_sMyCoupleInfo.vecItemInfo.clear();
	const std::vector<KCoupleEquip>& vecEquip = vecEquipList;
	for( int i = 0 ; i < (int)vecEquip.size() ; ++i )
	{
		SUserItem sUseritem;
		sUseritem.iItemID = vecEquip[i].m_dwItemID/10;
		sUseritem.dwItemUID =  vecEquip[i].m_dwItemUID;
		m_sMyCoupleInfo.vecItemInfo.push_back( sUseritem );
	}
}