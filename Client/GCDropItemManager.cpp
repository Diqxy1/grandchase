#include "stdafx.h"
#include "KncP2PLib\KncP2P.h"
#include "Message.h"
#include ".\gcdropitemmanager.h"
#include "Monster.h"
#include "GCDropItem.h"
#include "EveryTriggerManager.h"


ImplementSingleton(KGCDropItemManager);


namespace{
	typedef std::pair< DIL_itor, DIL_itor > TG_Equal_Range;

class TGIDFunctor:
    public std::unary_function<KGCDropItem*, bool>
{
public:
    int m_iDropUID;
    TGIDFunctor() {}
    TGIDFunctor( int iTriggerID )
    {
        m_iDropUID = iTriggerID;
    }
    ~TGIDFunctor() {}

    bool operator() ( KGCDropItem* arg )
    {
        if( arg->GetDropUID() == m_iDropUID )
            return true;
        return false;
    }
};

}

KGCDropItemManager::KGCDropItemManager(void)
{
	KGCDropItem::InitItemKindColor();
}


KGCDropItemManager::~KGCDropItemManager(void)
{
	KGCDropItem::ClearItemKindColor();
    Clear();
}

void AddItemToPacket( std::map<DWORD,KGCDropItem* >& mapList , int iTriggerID )
{
	for( DIL_itor mit = mapList.begin() ; mit != mapList.end() ; ++mit )
	{
		if( mit->second->GetTriggerID() == iTriggerID)
		{
			g_kDropItem->m_vecDropUID.push_back( mit->second->GetDropUID() );
		}
	}
}

bool KGCDropItemManager::SendDropInfo( int iTriggerID, D3DXVECTOR2 Pos )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return false;

    g_kDropItem->Type = GC_PID_DROPITEM;
    g_kDropItem->m_vecDropUID.clear();

	AddItemToPacket( m_mapDropList, iTriggerID );
	AddItemToPacket( m_mapMissionItem, iTriggerID );
    AddItemToPacket( m_mapGpList, iTriggerID );

    if( g_kDropItem->m_vecDropUID.empty() )
        return false;

    g_kDropItem->m_fX = Pos.x;
    g_kDropItem->m_fY = Pos.y;
    SERIALIZE_P2P_PACKET( (*g_kDropItem) );
	SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE );
	KGCPC_MEMO( "GC_PID_DROPITEM" );

    return true;
}

bool KGCDropItemManager::DropItem( std::vector<DWORD> vecDropUID , D3DXVECTOR2 Pos )
{
	DropItemFromList( m_mapDropList, vecDropUID, Pos);
	DropItemFromList( m_mapMissionItem, vecDropUID, Pos );
    DropItemFromList( m_mapGpList, vecDropUID, Pos );

    return true;
}

bool KGCDropItemManager::DropItemFromList( std::map<DWORD,KGCDropItem* >& mapList, std::vector<DWORD>& vecDropUID , D3DXVECTOR2& Pos)
{
	std::vector<DWORD>::iterator vitDUID;
    bool bJackPotSoundPlay = false;
	for( vitDUID = vecDropUID.begin(); vitDUID != vecDropUID.end() ; ++vitDUID )
	{
		if( m_setNoMoreDrop.find( *vitDUID ) != m_setNoMoreDrop.end() )
			continue;

        DIL_itor mit = mapList.find( *vitDUID );

		if( mit == mapList.end() )
			continue;

        if( false == bJackPotSoundPlay && mit->second->GetDropType() == KGameDropElement::RT_GP
            && mit->second->GetGoodsID() == KGameDropElement::GT_JP_SILVER )
        {
            g_KDSound.Play( "991021");
            bJackPotSoundPlay = true;
        }

        KGCPC_MEMO( "item drop" );
		mit->second->DropItem( Pos );

		m_setNoMoreDrop.insert( *vitDUID );
	}
	return true;
}

void KGCDropItemManager::FrameMove()
{
    PROFILE_SET("KGCDropItemManager::FrameMove");

    // 먹은 아이템은 리스트에서 지운다.
    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end();  )
    {
        if ( mitDropList->second->GetState() == KGCDropItem::IS_END )
        {
            SAFE_DELETE( mitDropList->second );
            mitDropList = m_mapDropList.erase( mitDropList );
        }
        else
        {
            ++mitDropList;
        }
    }

    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); )
    {
        if ( mitMission->second->GetState() == KGCDropItem::IS_END )
        {
            SAFE_DELETE( mitMission->second );
            mitMission = m_mapMissionItem.erase( mitMission );
        }
        else
        {
            ++mitMission;
        }
    }

    for( DIL_itor mitGP = m_mapGpList.begin(); mitGP != m_mapGpList.end();  )
    {
        if ( mitGP->second->GetState() == KGCDropItem::IS_END )
        {
            SAFE_DELETE( mitGP->second );
            mitGP = m_mapGpList.erase( mitGP );
        }
        else
        {
            ++mitGP;
        }
    }

    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end(); ++mitDropList )
    {
        mitDropList->second->FrameMove();
    }

    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); ++mitMission )
    {
		mitMission->second->FrameMove();
	}
    for( DIL_itor mitGP = m_mapGpList.begin(); mitGP != m_mapGpList.end(); ++mitGP )
    {
        mitGP->second->FrameMove();
    }
	m_kDropItemBox.FrameMove();

	//상자에 들어가야할 아이템들을 넣어줍니다.
	ProcessGoBoxReadyItem();

}
void KGCDropItemManager::SetItemUIDToVector( int iTriggerID, std::vector<DWORD>& vecOutList )
{
    AddItemUIDToVector(m_mapDropList, iTriggerID, vecOutList );
    AddItemUIDToVector(m_mapMissionItem, iTriggerID, vecOutList );
    AddItemUIDToVector(m_mapGpList, iTriggerID, vecOutList );
}
void KGCDropItemManager::AddItemUIDToVector( std::map<DWORD,KGCDropItem* >& mapList, int iTriggerID, std::vector<DWORD>& vecOutList )
{
    for( DIL_itor mit = mapList.begin() ; mit != mapList.end() ; ++mit )
    {
        if( mit->second->GetTriggerID() == iTriggerID)
        {
            vecOutList.push_back( mit->second->GetDropUID() );
        }
    }
}

void KGCDropItemManager::GetStageDropItem( int iCharacterIdx )
{
    // 전달된 캐릭터가 존재하는지 확인
    if( g_MyD3D->MyPlayer[iCharacterIdx]->m_kUserInfo.bLive == false )
    {   goto END_FUNC; }

    // 아이템 상태 확인 후 냠냠
    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end(); ++mitDropList )
    {
        if( mitDropList->second->GetState() != KGCDropItem::IS_DROP )
        {    continue;    }

        mitDropList->second->PlayerGetDropedItem( iCharacterIdx );
    }

    // GP 상태 확인 후 냠냠
    for( DIL_itor mitGP = m_mapGpList.begin(); mitGP != m_mapGpList.end(); ++mitGP )
    {
        if( mitGP->second->GetState() != KGCDropItem::IS_DROP )
        {    continue;    }

        mitGP->second->PlayerGetDropedItem( iCharacterIdx );
    }

    m_kDropItemBox.FrameMove();

    //상자에 들어가야할 아이템들을 넣어줍니다.
    ProcessGoBoxReadyItem();

END_FUNC:
    return;
}

#define DROP_ITEM_INSERT_GAP	(20)


void KGCDropItemManager::Alpha_Render()
{
	m_kDropItemBox.Render();
    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end(); ++mitDropList )
    {
        mitDropList->second->Render();
    }
    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); ++mitMission )
    {
        mitMission->second->Render();
    }
    for( DIL_itor mitGP = m_mapGpList.begin(); mitGP != m_mapGpList.end(); ++mitGP )
    {
        mitGP->second->Render();
    }

#if !defined( __PATH__ ) // 아이템 드랍 영역
    {
        if( g_kGlobalValue.m_bRenderBoundBox )
        {
            std::vector<D3DXVECTOR4> vecItemDropRect;
            GetItemDropRect( SiKGCRoomManager()->GetGameMode(), g_kEveryTriggerManager.GetActiveStage(), vecItemDropRect);

            std::vector<D3DXVECTOR4>::iterator vecIter = vecItemDropRect.begin();
            for(; vecIter != vecItemDropRect.end(); ++vecIter)
            {
                D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                SETVECTOR3( lefttop, float(vecIter->x), float(vecIter->y), 1.0f );
                SETVECTOR3( righttop, float(vecIter->z), float(vecIter->y), 1.0f );
                SETVECTOR3( leftbottom, float(vecIter->x), float(vecIter->w), 1.0f );
                SETVECTOR3( rightbottom, float(vecIter->z), float(vecIter->w), 1.0f );

                g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                    lefttop, righttop, leftbottom, rightbottom,0,0,0,
                    D3DCOLOR_ARGB(90, 120, 255, 165));
            }
        }
    }
#endif
}

bool KGCDropItemManager::Clear()
{
    m_setNoMoreDrop.clear();
    m_setNoMoreGet.clear();
    m_vecSomeonesItem.clear();
    m_vecGotItemList.clear();
    m_setGetGP.clear();
    m_mapGetItem.clear();

    for( DIL_itor mit = m_mapDropList.begin() ; mit != m_mapDropList.end() ; ++mit )
        SAFE_DELETE( mit->second );

    m_mapDropList.clear();

	for( DIL_itor mit = m_mapMissionItem.begin() ; mit != m_mapMissionItem.end() ; ++mit )
		SAFE_DELETE( mit->second );

	m_mapMissionItem.clear();

    for( DIL_itor mit = m_mapGpList.begin() ; mit != m_mapGpList.end() ; ++mit )
        SAFE_DELETE( mit->second );

    m_mapGpList.clear();

    // delete하면 안됨. 
//	for( DIL_itor vit = m_vecGoBoxItemList.begin() ; vit != m_vecGoBoxItemList.end() ; ++vit )
//		SAFE_DELETE( *vit );

	m_vecGoBoxItemList.clear();

	m_kDropItemBox.Init();
    
    return true;
}

void KGCDropItemManager::PushGotItem( DWORD dwUserUID_, DWORD dwDropUID_, char cDropType_ )
{
    m_setNoMoreGet.insert( dwDropUID_ );


    if( KGameDropElement::RT_GP == cDropType_)
    {
        m_setGetGP.insert( dwDropUID_ );
    }
    else
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUserUID_ );
        if ( pPlayer && pPlayer->IsLive() )
        {
            if ( ( KGameDropElement::RT_ITEM != cDropType_ && KGameDropElement::RT_CHARDROP_ITEM != cDropType_ )
                || pPlayer->m_kUserInfo.iInvenCapacity > pPlayer->m_kUserInfo.iUserItemCount )
            {
                m_mapGetItem.insert( std::map<DWORD, DWORD>::value_type( dwDropUID_, dwUserUID_) );
            }
        }
    }

    PlayerGetItem( dwUserUID_, dwDropUID_ );
}

bool KGCDropItemManager::PlayerGetItemFromList( std::map<DWORD,KGCDropItem* >& mapList, DWORD dwUserUID_, DWORD dwDropUID_ )
{
    DIL_itor mit = mapList.find( dwDropUID_ );

	if( mit == mapList.end() )
		return false;

	mit->second->PlayerGetItem( dwUserUID_ );

	return true;
}

bool KGCDropItemManager::PlayerGetItem( DWORD dwUserUID_, int dwDropUID_ )
{
	PlayerGetItemFromList( m_mapDropList, dwUserUID_, dwDropUID_ );
	PlayerGetItemFromList( m_mapMissionItem, dwUserUID_, dwDropUID_ );
    PlayerGetItemFromList( m_mapGpList, dwUserUID_, dwDropUID_ );
    return true;
}

bool KGCDropItemManager::DropItemListPreSet( std::vector< KGameDropElement > vecDropList , DWORD dwRandomSeed )
{
    m_kDropRandom.SetSeed( dwRandomSeed );
    m_kDropRandom.SetRange( 1, 1000 );
    
    std::vector< KGameDropElement >::iterator vitDrop = vecDropList.begin();
    for( vitDrop; vitDrop != vecDropList.end(); ++vitDrop )
    {
        if( KGameDropElement::RT_ITEM == (*vitDrop).m_cRewardType ||
            KGameDropElement::RT_CHARDROP_ITEM == (*vitDrop).m_cRewardType )
        {
            KGCDropItem* pItem = NULL;
            pItem = new KGCDropItem_ITEM();
            if( NULL == pItem )
                continue;
            
            pItem->SetRandomSeed( static_cast<DWORD>( m_kDropRandom.rand() ) );
            pItem->SetPacketData( *vitDrop );

            m_mapDropList.insert( std::make_pair(pItem->GetDropUID(),pItem) );
        }
    }

    return true;
}

bool KGCDropItemManager::AddDropItemListPreSet( std::vector< KGameDropElement > vecDropList )
{
    std::vector< KGameDropElement >::iterator vitDrop = vecDropList.begin();
    for( vitDrop; vitDrop != vecDropList.end(); ++vitDrop )
    {
        if( KGameDropElement::RT_ITEM == (*vitDrop).m_cRewardType ||
            KGameDropElement::RT_CHARDROP_ITEM == (*vitDrop).m_cRewardType )
        {
            KGCDropItem* pItem = NULL;
            pItem = new KGCDropItem_ITEM();
            if( NULL == pItem )
                continue;

            pItem->SetRandomSeed( static_cast<DWORD>( m_kDropRandom.rand() ) );
            pItem->SetPacketData( *vitDrop );

            m_mapDropList.insert( std::make_pair( pItem->GetDropUID(), pItem ) );
        }
    }

    return true;
}

bool KGCDropItemManager::MissionItemListPreSet( std::vector< KGameDropElement > vecMissionItem, DWORD dwRandomSeed )
{
	m_kMissionRandom.SetSeed( dwRandomSeed );
    m_kMissionRandom.SetRange( 1001, 2000 );

    std::vector< KGameDropElement >::iterator vitMission= vecMissionItem.begin();
    for( vitMission; vitMission != vecMissionItem.end(); ++vitMission )
    {
		KGCDropItem* pItem = NULL;
        pItem = new KGCDropItem_MISSION();
        if( NULL == pItem )
            continue;

        pItem->SetRandomSeed( static_cast<DWORD>( m_kMissionRandom.rand() ) );
		pItem->SetPacketData( *vitMission );
        m_mapMissionItem.insert( std::make_pair(pItem->GetDropUID(),pItem) );
	}

	return true;
}


bool KGCDropItemManager::AddMissionItemListPreSet( std::vector< KGameDropElement > vecMissionItem )
{
    std::vector< KGameDropElement >::iterator vitMission= vecMissionItem.begin();
    for( vitMission; vitMission != vecMissionItem.end(); ++vitMission )
    {
        KGCDropItem* pItem = NULL;
        pItem = new KGCDropItem_MISSION();
        if( NULL == pItem )
            continue;

        pItem->SetRandomSeed( static_cast<DWORD>( m_kMissionRandom.rand() ) );
        pItem->SetPacketData( *vitMission );
        m_mapMissionItem.insert( std::make_pair(pItem->GetDropUID(),pItem) );
    }

    return true;
}

bool KGCDropItemManager::GPListPreSet( std::vector< KGameDropElement > vecGpList_, DWORD dwRandomSeed )
{
    m_kGPRandom.SetSeed( dwRandomSeed );
    m_kGPRandom.SetRange( 2001, 3000 );

    LoadScriptGPStyle();
    std::vector< KGameDropElement >::iterator vitGP= vecGpList_.begin();
    for( vitGP; vitGP != vecGpList_.end(); ++vitGP )
    {
        (*vitGP).m_cRewardType = KGameDropElement::RT_GP;
        if( KGameDropElement::RT_GP == (*vitGP).m_cRewardType )
        {
            KGCDropItem* pItem = NULL;
            pItem = new KGCDropItem_GP();
            if( NULL == pItem )
                continue;

            pItem->SetRandomSeed( static_cast<DWORD>( m_kGPRandom.rand() ) );
            pItem->SetPacketData( *vitGP );
            m_mapGpList.insert( std::make_pair( pItem->GetDropUID(), pItem ) );
        }
    }

    return true;
}

bool KGCDropItemManager::AddGPListPreSet( std::vector< KGameDropElement > vecGpList )
{
    std::vector< KGameDropElement >::iterator vitGP= vecGpList.begin();
    for( vitGP; vitGP != vecGpList.end(); ++vitGP )
    {
        (*vitGP).m_cRewardType = KGameDropElement::RT_GP;
        if( KGameDropElement::RT_GP == (*vitGP).m_cRewardType )
        {
            KGCDropItem* pItem = NULL;
            pItem = new KGCDropItem_GP();
            if( NULL == pItem )
                continue;

            pItem->SetRandomSeed( static_cast<DWORD>( m_kGPRandom.rand() ) );
            pItem->SetPacketData( *vitGP );
            m_mapGpList.insert( std::make_pair( pItem->GetDropUID(), pItem ) );
        }
    }

    return true;
}


void KGCDropItemManager::LoadScriptGPStyle()
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "Dungeon_ExpGp.lua" ) == false )
        return;

    LUA_BEGIN_TABLE_RAII("Dungeon_Exp_Gp")
    {
        m_mapGPStyle.clear();
        for( int iLoop=1; ; ++iLoop )
        {
            LUA_BEGIN_TABLE_RAII( iLoop )
            {
                int iDungeonID = 0;
                LUA_GET_VALUE("DungeonID", iDungeonID, break);
                
                if( iDungeonID == SiKGCRoomManager()->GetGameMode() )
                {
                    LUA_BEGIN_TABLE_RAII("GP_STYLE")
                    {
                        std::vector<std::string> vecGPType;
                        vecGPType.push_back( "SILVER" );
                        vecGPType.push_back( "GOLD" );
                        vecGPType.push_back( "JP_SILVER" );
                        
                        std::vector<std::string>::iterator vecIter = vecGPType.begin();
                        for(; vecIter!=vecGPType.end(); ++vecIter)
                        {
                            LUA_BEGIN_TABLE_RAII( vecIter->c_str() )
                            {
                                SGPStyle sGpStyle;
                                GCITEMID dwGpType = KGameDropElement::GT_SILVER;

                                if( "SILVER" == (*vecIter) )
                                    dwGpType = KGameDropElement::GT_SILVER;
                                if( "GOLD" == (*vecIter) )
                                    dwGpType = KGameDropElement::GT_GOLD;
                                if( "JP_SILVER" == (*vecIter) )
                                    dwGpType = KGameDropElement::GT_JP_SILVER;

                                LUA_BEGIN_TABLE_RAII( "Mesh" )
                                {
                                    LUA_GET_VALUE_DEF( 1, sGpStyle.strMesh, "" );
                                    LUA_GET_VALUE_DEF( 2, sGpStyle.strTex, "" );
                                    LUA_GET_VALUE_DEF( 3, sGpStyle.strMotion, "" );
                                }

                                LUA_BEGIN_TABLE_RAII( "Trans" )
                                {
                                    LUA_GET_VALUE_DEF( 1, sGpStyle.vTrans.x, 0.0f );
                                    LUA_GET_VALUE_DEF( 2, sGpStyle.vTrans.y, 0.52f );
                                }

                                LUA_BEGIN_TABLE_RAII( "Scale" )
                                {
                                    LUA_GET_VALUE_DEF( 1, sGpStyle.vScale.x, 0.5f );
                                    LUA_GET_VALUE_DEF( 2, sGpStyle.vScale.y, 0.5f );
                                    LUA_GET_VALUE_DEF( 3, sGpStyle.vScale.z, 0.5f );
                                }

                                LUA_BEGIN_TABLE_RAII( "Speed" )
                                {
                                    LUA_GET_VALUE_DEF( 1, sGpStyle.vSpeed.x, 4000.0f );
                                    LUA_GET_VALUE_DEF( 2, sGpStyle.vSpeed.y, 24000.0f );
                                }

                                LUA_GET_VALUE_DEF( "StartFrame", sGpStyle.iStartFrame, 2 );

                                m_mapGPStyle.insert( std::make_pair(dwGpType, sGpStyle) );
                            }
                        }
                    }
                }
            }
            else
                break;
        }
    }
}

SGPStyle* KGCDropItemManager::GetGpStyle(int iGpType)
{
    std::map< GCITEMID, SGPStyle >::iterator mapIter = m_mapGPStyle.find( GCITEMID(iGpType) );
    if( mapIter != m_mapGPStyle.end() )
        return &mapIter->second;
    return NULL;
}

bool MonsterObserverForDropItem( int iTriggerID, D3DXVECTOR2 vPos )
{
    return SiKGCDropItemManager()->SendDropInfo( iTriggerID, vPos );
}

void KGCDropItemManager::SetObserverToMonster( int iTriggerID, int iMonsterIndex, int iMonsterID )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonsterIndex );
	if(pMonster == NULL ) return;

    if( pMonster->GetTriggerID() != iTriggerID )
        return;

    if( iTriggerID != -1 &&  pMonster->m_eNPC == NPC_NONE && pMonster->m_bMonsterTarget  && false == pMonster->m_bNotMissionCount &&
        ( g_kGlobalValue.m_kUserMission.IsMissionMonster( (EGCMonster)iMonsterID, pMonster->GetLevel() ) || IsMissionItemTrigger( iTriggerID ) ) )
        pMonster->SetMissionMonster();

    pMonster->SetDropObserver( MonsterObserverForDropItem );
}

bool KGCDropItemManager::IsMissionItemTrigger( int iTriggerID )
{
    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); ++mitMission )
    {
        if( mitMission->second->CheckTriggerID( iTriggerID ) == true )
            return true;
    }

    return false;
}

DWORD KGCDropItemManager::GetGoodsIDFromDropUID( DWORD dwUID )
{
    DIL_itor mit;
    for( mit = m_mapDropList.begin() ; mit != m_mapDropList.end() ; ++mit )
    {
        if( mit->second->GetDropUID() == dwUID )
        {
            return mit->second->GetGoodsID();
        }
    }

    return 0;
}

bool KGCDropItemManager::ClearNowDropedItem()
{
    DIL_itor mit;

    for( mit = m_mapDropList.begin() ; mit != m_mapDropList.end() ; ++mit )
    {
        if( mit->second->GetState() == KGCDropItem::IS_DROP )
        {
            mit->second->SetState( KGCDropItem::IS_NONE );
            mit->second->RemoveParticleFromItem();
        }
    }

    KGCPC_MEMO( "Drop item clear" );
    return true;
}

bool KGCDropItemManager::ClearNowDropedGP()
{
    DIL_itor mit;
    for( mit = m_mapGpList.begin() ; mit != m_mapGpList.end() ; ++mit )
    {
        if( mit->second->GetState() == KGCDropItem::IS_DROP )
        {
            mit->second->SetState( KGCDropItem::IS_NONE );
            mit->second->RemoveParticleFromItem();
        }
    }
    return true;
}

void KGCDropItemManager::ProcessGoBoxReadyItem()
{
	static std::vector<KGCDropItem* > vecGoingItem;


    DIL_itor mit;
    for( mit = m_mapDropList.begin() ; mit != m_mapDropList.end() ; ++mit )
    {
        if( mit->second->GetState() != KGCDropItem::IS_GOBOX_READY )
            continue;

        m_vecGoBoxItemList.push_back( mit->second );
        mit->second->SetState( KGCDropItem::IS_NONE );
    }

    std::vector<KGCDropItem* >::iterator vitGoingItem = vecGoingItem.begin();
    for(; vitGoingItem != vecGoingItem.end(); )
    {
        if( (*vitGoingItem)->GetState() != KGCDropItem::IS_GOBOX )
            vitGoingItem = vecGoingItem.erase( vitGoingItem );
        else
            ++vitGoingItem;
    }

	static int iNextPushCount = 20;
	if( m_vecGoBoxItemList.empty() &&
		vecGoingItem.empty() &&
		m_kDropItemBox.GetCurrentState() == KGCDropItemBox::DIB_OPENED )
	{
		iNextPushCount = 20;
		m_kDropItemBox.CloseBox();
		return;
	}

	if( m_vecGoBoxItemList.empty() == false &&
		m_kDropItemBox.GetCurrentState() == KGCDropItemBox::DIB_DISABLE )
	{
		m_kDropItemBox.OpenBox();
		return;
	}

	if(  m_kDropItemBox.GetCurrentState() != KGCDropItemBox::DIB_OPENED )
		return;

	if( m_vecGoBoxItemList.empty() )
		return;


	std::vector< KGCDropItem* >::iterator vit = m_vecGoBoxItemList.begin() ;
	if( m_vecGoBoxItemList.end() != vit && iNextPushCount > 20 )
	{
		(*vit)->SetState( KGCDropItem::IS_GOBOX );
		vecGoingItem.push_back( (*vit) );
		m_vecGoBoxItemList.erase( vit );
		iNextPushCount = 0;
	}
	else
	{
		++iNextPushCount;
	}
}

void KGCDropItemManager::Init()
{
	m_kDropItemBox.LoadBoxTexture();
    LoadDropRect();
}

std::vector< SGotItem > KGCDropItemManager::GetPlayersItemLIst()
{
	return m_vecSomeonesItem;
}

void KGCDropItemManager::ClearDungeonGetItem()
{
    D3DXVECTOR2 tmp(g_MyD3D->GetMyPlayer()->GetPosition().x, g_MyD3D->GetMyPlayer()->GetPosition().y);
    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end(); ++mitDropList )
    {
        m_mapGetItem.insert( std::map<DWORD, DWORD>::value_type( mitDropList->second->GetDropUID(), g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID ));
    }
    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); ++mitMission )
    {
        m_mapGetItem.insert( std::map<DWORD, DWORD>::value_type( mitMission->second->GetDropUID(), g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID ));
    }
    for( DIL_itor mitGP = m_mapGpList.begin(); mitGP != m_mapGpList.end(); ++mitGP )
    {
        m_setGetGP.insert( g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID );
    }

    m_vecGPListForSend.clear();
}

std::set<KGCDropItem* > KGCDropItemManager::GetAllDropItem()
{
    std::set<KGCDropItem* > setAllDropItem;
    setAllDropItem.clear();
 
    for( DIL_itor mitDropList = m_mapDropList.begin(); mitDropList != m_mapDropList.end(); ++mitDropList )
    {
        setAllDropItem.insert(mitDropList->second);
    }

    for( DIL_itor mitMission = m_mapMissionItem.begin(); mitMission != m_mapMissionItem.end(); ++mitMission )
    {
        setAllDropItem.insert(mitMission->second);
    }
    
    return setAllDropItem;
}

bool KGCDropItemManager::IsExistGetItem( KGC_PID_PLAYERGETITEM& kGetItem_ )
{
    std::vector<KGC_PID_PLAYERGETITEM>::iterator vit = m_vecGPListForSend.begin();

    if( vit != m_vecGPListForSend.end() )
    {
        kGetItem_ = (*vit);
        m_vecGPListForSend.erase(vit);
        return true;
    }
    
    return false;
}

void KGCDropItemManager::GetItemDropRect(int iGameMode, int iStage, std::vector<D3DXVECTOR4> &vecItemDropRect)
{
    std::map< std::pair<int,int>, std::vector<D3DXVECTOR4> >::iterator mapIter = m_mapItemDropRect.find( std::make_pair(iGameMode, iStage) );
    if( mapIter != m_mapItemDropRect.end() )
    {
        std::vector<D3DXVECTOR4>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter)
        {
            vecItemDropRect.push_back( (*vecIter) );
        }
    }
}

void KGCDropItemManager::LoadDropRect()
{
    KLuaManager luaMgr;


    if( GCFUNC::LoadLuaScript( luaMgr, "ItemDropRect.stg" ) == false )
    {
        MessageBoxA( NULL, "ItemDropRect.stg Load Failed!", "", MB_OK );
        return;
    }

    m_mapItemDropRect.clear();
    luaMgr.BeginTable( "ItemDropRect" );
    {
        char szTemp[1024];
        for(int iGameModeLoop=0; iGameModeLoop<NUM_GC_GAME_MODE; ++iGameModeLoop)
        {
            sprintf(szTemp, "Dungeon%d", iGameModeLoop);
            if( FAILED( luaMgr.BeginTable( szTemp ) ) ) continue;
            {
                int iStageLoop = 0;
                std::vector<D3DXVECTOR4> vecRect;
                while(1)
                {
                    vecRect.clear();
                    sprintf(szTemp, "Stage%d", iStageLoop);
                    if( FAILED( luaMgr.BeginTable( szTemp ) ) ) break;
                    {
                        int iRectLoop=1;
                        while(1)
                        {
                            D3DXVECTOR4 rect;
                            if( FAILED( luaMgr.BeginTable( iRectLoop++ ) ) ) break;
                            {
                                LUA_GET_VALUE_DEF(1, rect.x, 0);
                                LUA_GET_VALUE_DEF(2, rect.y, 0);
                                LUA_GET_VALUE_DEF(3, rect.z, 0);
                                LUA_GET_VALUE_DEF(4, rect.w, 0);
                            }
                            luaMgr.EndTable();
                            vecRect.push_back( rect );
                        }
                    }
                    luaMgr.EndTable();
                    m_mapItemDropRect.insert( std::make_pair( std::make_pair(iGameModeLoop, iStageLoop), vecRect ) );
                    ++iStageLoop;
                }
                
            }
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();
}