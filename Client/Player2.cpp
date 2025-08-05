#include "stdafx.h"
#include "Player.h"
#include "GCUI/GCMyInfoScene.h"
#include "Packet.h"


void PLAYER::AllUnEquipItem( char cCharType )
{
    SCharInfo &sCharInfo = m_kUserInfo.GetCurrentChar( cCharType );
    if( sCharInfo.iCharType != cCharType )
        return;
    
    // 아이템 정보 초기화
    sCharInfo.vecItemInfo.clear();
    g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd( -1 );

    g_pItemMgr->EquipBaseItem( &sCharInfo );

    // 무기체인지 정보쓰기!
#ifdef WEAPONCHANGE
    KEquipItemInfo kChangeWeaponItem;
    SetWeaponChangeItem(sCharInfo.iCharType, false, kChangeWeaponItem );
#endif

    sCharInfo.kPetInfo.Init();
}

void PLAYER::AllUnEquipCoordiItem( char cCharType )
{
    SCharInfo &sCharInfo = m_kUserInfo.GetCurrentChar( cCharType );
    if( sCharInfo.iCharType != cCharType )
        return;

    sCharInfo.vecLookInfo.clear();
}