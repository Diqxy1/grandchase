#include "stdafx.h"
//

#include "MyD3D.h"
#include "ProtectRing.h"
#include "QuestGameManager.h"
#include "KGCRoomManager.h"

#include "message.h"
#include "Headup Display.h"
#include "../KncP2PLib/KncCommonDefine.h"

#include "GCStateGame.h"


KProtectRing::KProtectRing(  KItem* _kInven, PLAYER_INFO* _pMyPlayer, SHORTCUT_ITEM* _pStruct )
{
	// Item 객체의 기본 정보
	kItem = (*_kInven);
	nowCoolTime	= 0;
	m_MyPlayer		= _pMyPlayer;
	m_ItemStruct	= _pStruct;
}
KProtectRing::~KProtectRing()
{}


//////////////////////////////////////////////////////////////////////////
//						Public Member Functions							//
//////////////////////////////////////////////////////////////////////////

void KProtectRing::Set()
{
	// 하는 일 없음
	// 뭐 나중에 다른 Item 추가되다 보면 쓸 일이 있겠지요q
}

bool KProtectRing::Do()
{
    // 게임 중인 플레이어 중에 죽은 자가 있는가
    bool bUse = false;
    for( int index = 0 ; index < MAX_PLAYER_NUM ; index++ )	{
        PLAYER* pPlayer = g_MyD3D->MyPlayer[ index ];
        if( pPlayer && (pPlayer->m_kUserInfo.bLive || pPlayer->m_kUserInfo.bHost) ) { 
            if( pPlayer->m_cLife == 0  ) { 
                bUse = true;
            }
        }
    }
    if( bUse == false )
        return false;

    // 게임모드 정보는 제대로 받아왔는가
    KGCGameModeInterface* pMode;
	pMode = g_pStateGame->GetCurGameModePtr();
	if( pMode == NULL )
		return false;
	
	// 게임이 끝나진 않았는가
	int iMyPlayer = g_MyD3D->Get_MyPlayer();
	if( iMyPlayer == 7 || g_pkQuestManager->m_bEndGame == true )
		return false;
	
	// 채팅 모드는 아닌가
	if( g_MyD3D->MyHead->IsCheatMode == true )
		return false;
	
	// 살아날 상황은 되는가
	if( pMode->VerifyRebirth() == false )
		return false;

	//방장에게 패킷을 던진다....
	//방장이 verify한뒤 notify한다.
	KGC_PID_QUEST_REBIRTH packet;
	packet.bRequest = true;
	SendP2PPacket( (void*)&packet, sizeof( KGC_PID_QUEST_REBIRTH ), SiKGCRoomManager()->GetHostUID(), _RELIABLE );
	KGCPC_MEMO( "KGC_PID_QUEST_REBIRTH" );

	//g_MyD3D->GetMyPlayer()->m_cLife

	SetCoolTime();
	DecreaseQuantity();

	return true;
}

void KProtectRing::SetCoolTime(INT _CoolTime)
{
	int coolTime;
	if( _CoolTime == (-1) )
		coolTime = m_ItemStruct->iCoolTime;
	else
		coolTime = _CoolTime;

	nowCoolTime = SECOND(coolTime);
}

void KProtectRing::DecreaseCoolTime(INT _Num)
{
	if( nowCoolTime > 0)
		nowCoolTime -=_Num;
}

bool KProtectRing::DecreaseQuantity()
{
    if( kItem.m_nCount != KItem::UNLIMITED_ITEM )
		kItem.m_nCount--;

	if( kItem.m_nCount >= 1 )
	{
		return true;
	}
	else
		return false;
}

std::string KProtectRing::Dump() const
{
    std::stringstream stm;
    stm << "ProtectedRing " << KGCShortCutSlotItem::Dump();
    return stm.str();
}
