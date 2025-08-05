#include "stdafx.h"
//
#include ".\emotionitem.h"
#include "MyD3D.h"


#include "LatencyTransfer.h"
#include "GCStateMachine.h"
#include "GCAchieveManager.h"

EmotionItem::EmotionItem(  KItem* _kInven, PLAYER_INFO* _pMyPlayer, SHORTCUT_ITEM* _pStruct )
{
    // Item 객체의 기본 정보
    kItem = (*_kInven);
    nowCoolTime	= 0;
    m_MyPlayer		= _pMyPlayer;
    m_ItemStruct	= _pStruct;
}

EmotionItem::~EmotionItem(void)
{
}

void EmotionItem::Set()
{

}

bool EmotionItem::Do()
{
    //KGCEmoticonInstance* pEmoticon = SiKGCEmoticonManager()->SetEmoticon( kItem.m_iItemID );

    //g_MyD3D->GetMyPlayer()->SetEmoticon( pEmoticon );
    //pEmoticon->SetPos( &D3DXVECTOR2( g_MyD3D->GetMyPlayer()->m_pObject->GetWorldMat()->_41, g_MyD3D->GetMyPlayer()->m_pObject->GetWorldMat()->_42 ) );

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE )
    {
        KP2P::GetInstance()->Send_UseEmoticon_Req( kItem.m_ItemID );
    }
    else
    {
        LTP_EMOTICON packet;
        packet.iCharIndex = g_MyD3D->Get_MyPlayer();
        packet.iItemID = kItem.m_ItemID;
        g_LatencyTransfer.PushPacket( &packet );
    }

    SiKGCAchieveManager()->OccurAction( SAchieve::USE_EMOTICON, GC_CHAR_INVALID, kItem.m_ItemID );

    return true;    
}


bool EmotionItem::DecreaseQuantity()
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

void EmotionItem::DecreaseCoolTime(INT Num)
{
    if( nowCoolTime > 0)
        nowCoolTime -= Num;
}

void EmotionItem::SetCoolTime(INT _CoolTime)
{
    int coolTime;
    if( _CoolTime == (-1) )
        coolTime = SECOND(5);
    else
        coolTime = _CoolTime;

    nowCoolTime = _CoolTime/*SECOND(5)*/;
}
