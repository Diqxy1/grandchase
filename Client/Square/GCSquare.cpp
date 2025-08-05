#include "stdafx.h"
//
#include ".\gcsquare.h"

#include "GCSquarePeople.h"
#include "GCSquarePeopleStateFunctions.h"
#include "GCSquarePeopleFrameInfo.h"
#include "GCSquarePeopleActivitySensor.h"


#include "Spark.h"


#include "Procedure.h"
//
#include "GCUI/KGCSquareOverlayUI.h"
#include "MapSystem/KGCLayerManager.h"
//
#include "Controls.h"

//
//
#include "GCSquare2DObj.h"
#include "GCSquare3DObj.h"
#include "GCSquareNPC.h"
#include "PetAI.h"
#include "GCSquarePet.h"
#include "GCSquare3DObj.h"
#include "GCSquareNPC.h"

#include "gcui/KGCShortCutBox.h"
#include "gcui/GCSquareLoadingScene.h"

#include "../GCRenderManager.h"

#include "../KGCValentineDayManager.h"
#include "GCCameraOrtho.h"

#include "gcui/KGCCouponNPCDlg.h"
#include "gcui/KGCSubscriptionDlg.h"
#include "P2P.H"
#include "Message.h"
#include "KGCPartyManager.h"
#include "GCClientErrorManager.h"
#include "ClientErr.h"
#include "KGCContributionManager.h"


ImplementSingleton( GCSquare );

GCSquare::GCSquare(void)
: m_bLoadingComplete(false)
, m_bSparkLoading(false)
, m_bSoundLoading(false)
, m_dwRelayServerIP(0)
, m_usRelayServerPort(0)
, m_bConnectToSquare(false)
, m_dwSumOfFrame(0)
, m_dwSumCount(0)
, m_dwMaxRenderCount(0)
, m_dwPrevCheckTime(0)
, m_bSendReq(false)
, m_iEventBannerType(-1)
, m_iSquareType(0)
{
	m_pTextureGPCItem = NULL;
	m_vGPCPlayerPos.x = 0.0f;
	m_vGPCPlayerPos.y = 0.0f;
	m_vGPCPlayerPos.z = 0.0f;
	m_iGPCRenderTime = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			m_pCampFireParticle[j][i] = NULL;
		}
	}

	m_vecFireWorksBigParticle1.clear();
	m_vecFireWorksBigParticle2.clear();
	m_vecFireWorksMiddleParticle.clear();
	m_vecFireWorksSmallParticle.clear();
	m_vecFireWorksSmallBgParticle.clear();
	m_vecFireWorksMiddleBgParticle.clear();

	m_bCreateFireWorks = false;
	m_bCreateCampFire = false;
	m_iResetTime = 0;

//skrjsemfdlwlakfk!TIdghksksek.
	//m_vecNonPeople.clear();
    m_mapPeople.clear();
    m_mapPet.clear();
    m_vecPeopleUID.clear();
    m_vecSortedPeoplebyDistance.clear();
    m_vecNearPeopleUID.clear();
    m_vecPeopleReserve.clear();
    m_vecPetReserve.clear();
    m_lPrevSavedState.clear();
    m_vec3DObjcet.clear();

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<GCSquare>( L, "GCSquareClass" );
    //	lua_tinker::class_mem<>( L, "iCurrTexIndex", 	&GCSquare::m_iCurrTexIndex );
    lua_tinker::class_def<GCSquare>( L, "AddSquareObject",          &GCSquare::AddSquareObject );
    lua_tinker::class_def<GCSquare>( L,  "GetLocalPeople",          &GCSquare::GetLocalPeople  );
    lua_tinker::class_def<GCSquare>( L,  "GetWorldMousePos",        &GCSquare::GetWorldMousePos  );
    lua_tinker::class_def<GCSquare>( L, "Create2DObject",           &GCSquare::Create2DObject  );
    lua_tinker::class_def<GCSquare>( L, "AddParticle",              &GCSquare::AddParticle  );
    lua_tinker::class_def<GCSquare>( L, "Get3DObjectPos",           &GCSquare::Get3DObjectPos );;
    lua_tinker::class_def<GCSquare>( L, "SetEventBannerType",       &GCSquare::SetEventBannerType);
    lua_tinker::class_def<GCSquare>( L, "GetSquareType",            &GCSquare::GetSquareType );

    lua_tinker::class_def<GCSquare>( L,  "GetGuildLevel",           &GCSquare::GetGuildLevel  );
    lua_tinker::class_def<GCSquare>( L,  "ShowMessageBox",          &GCSquare::ShowMessageBox  );
    lua_tinker::class_def<GCSquare>( L,  "SendGwcBoardReq",          &GCSquare::SendGwcBoardReq  );
    lua_tinker::class_def<GCSquare>( L,  "Send_NewYearDonationInfoReq",          &GCSquare::Send_NewYearDonationInfoReq  );

    lua_tinker::decl( L, "GCSquare", this );

	IGCSquareObj::BindLua();
	KGCSquare2DObj::BindLua();
}

GCSquare::~GCSquare(void)
{
    CloseSquare();
}

void GCSquare::SquareInit()
{
    GCSquarePeopleStateFunctions::SetStateFunstions();
    //GCSquarePeopleFrameInfo::SetFrameInfoProc();
	GCSquarePeopleActivitySensor::SetActivityTransitionFunc();
}

void GCSquare::CreateCampFire()
{
	std::string strTemp = "Campfire_0";
	
	
	for(int  i = 0; i < 4; i++)
	{
		char szTemp[20] = {0,};
		std::string strLast = itoa(i + 1 , szTemp, 10);
		m_pCampFireParticle[0][i] = g_ParticleManager->CreateSequence(strTemp + strLast, 2.35f-2.44f, 0.06f, 0.5f);
		m_pCampFireParticle[1][i] = g_ParticleManager->CreateSequence(strTemp + strLast, 8.35f+1.337f, 0.06f, 0.5f);
	}

	m_bCreateCampFire = true;
}

void GCSquare::CreateFireWorks()
{
	//for(int i = 0; i < 2; i++)
	//{	
	//	float fx = ((rand() % 10000) / 10000.0f) * 6.0f + 3.0f;
	//	float fy = ((rand() % 10000) / 10000.0f) * 2.0f + 3.5f; // 6 - 2.5 = 3.5 

	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big01", 9.2765245f-1.0f, 5.2665005f+0.3f, 0.5f));
	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big02", 9.2765245f-1.0f, 5.2665005f+0.3f, 0.5f));

	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big01", 5.9810023f-1.5f ,4.4700003f+0.75f, 0.5f));
	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big02", 5.9810023f-1.5f ,4.4700003f+0.75f, 0.5f));

	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big01", 2.7179828f-1.0f ,5.2665005f+0.3f, 0.5f));
	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_big02", 2.7179828f-1.0f ,5.2665005f+0.3f, 0.5f));

	m_vecFireWorksMiddleBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_middle_bg", 6.0f, 5.0f, 0.5f));
	m_vecFireWorksSmallBgParticle.push_back(g_ParticleManager->CreateSequence("Flame_small_bg", 6.0f, 5.0f, 0.5f));

	m_bCreateFireWorks = true;
}

void GCSquare::ChangeFireWorksPos()
{
	//for(int  i = 0; i < (int)m_vecFireWorksBigParticle1.size(); i++)
	//{
	//	float fx = ((rand() % 10000) / 10000.0f) * 6.0f + 3.0f;
	//	float fy = ((rand() % 10000) / 10000.0f) * 2.0f + 3.5f;

	//	if(false == g_ParticleManager->IsLiveInstance(m_vecFireWorksBigParticle1[i]))
	//	{
	//		m_vecFireWorksBigParticle1[i] = g_ParticleManager->CreateSequence("Flame_big01", fx, fy, 0.5f);
	//	}
	//}

	//for(int  i = 0; i < (int)m_vecFireWorksBigParticle2.size(); i++)
	//{
	//	float fx = ((rand() % 10000) / 10000.0f) * 6.0f + 3.0f;
	//	float fy = ((rand() % 10000) / 10000.0f) * 2.0f + 3.5f;

	//	if(false == g_ParticleManager->IsLiveInstance(m_vecFireWorksBigParticle2[i]))
	//	{
	//		m_vecFireWorksBigParticle2[i] = g_ParticleManager->CreateSequence("Flame_big02", fx, fy, 0.5f);
	//	}
	//}
}
void GCSquare::DeleteCampFire()
{
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0 ; j < 4; j++)
		{
			if(m_pCampFireParticle[i][j])
			{
				g_ParticleManager->DeleteSequence(m_pCampFireParticle[i][j]);
				m_pCampFireParticle[i][j] = NULL;
			}
		}
	}

	m_bCreateCampFire = false;
}

void GCSquare::DeleteFireWorks()
{
	//개수가 같으니까 같이쓸게요. ㅈㅅ
	/*for(int i = 0 ; i < (int)m_vecFireWorksBigParticle1.size(); i++)
	{
		if ( g_ParticleManager->IsLiveInstance( m_vecFireWorksBigParticle1[i] )  )
		{
			g_ParticleManager->DeleteSequence( m_vecFireWorksBigParticle1[i] );
		}
	}
	m_vecFireWorksBigParticle1.clear();

	for(int i = 0 ; i < (int)m_vecFireWorksBigParticle2.size(); i++)
	{
		if ( g_ParticleManager->IsLiveInstance( m_vecFireWorksBigParticle2[i] )  )
		{
			g_ParticleManager->DeleteSequence( m_vecFireWorksBigParticle2[i] );
		}
	}
	m_vecFireWorksBigParticle2.clear();*/

	for(int i = 0 ; i < (int)m_vecFireWorksSmallBgParticle.size(); i++)
	{
		if ( g_ParticleManager->IsLiveInstance( m_vecFireWorksSmallBgParticle[i] )  )
		{
			g_ParticleManager->DeleteSequence( m_vecFireWorksSmallBgParticle[i] );
		}
	}
	m_vecFireWorksSmallBgParticle.clear();

	for(int i = 0 ; i < (int)m_vecFireWorksMiddleBgParticle.size(); i++)
	{
		if ( g_ParticleManager->IsLiveInstance( m_vecFireWorksMiddleBgParticle[i] )  )
		{
			g_ParticleManager->DeleteSequence( m_vecFireWorksMiddleBgParticle[i] );
		}
	}
	m_vecFireWorksMiddleBgParticle.clear();

	m_bCreateFireWorks = false;
}

KItemIDVector GCSquare::GetEquipmentForLoadModel( KSquareUserInfo& kSquarePeople )
{
    KItemIDVector vecMerge;

    vecMerge.reserve( kSquarePeople.m_vecLookEquips.size() );

    for ( int i = 0; i < (int)kSquarePeople.m_vecLookEquips.size(); ++i ) 
    {
        GCITEMID itemID = kSquarePeople.m_vecLookEquips[i].m_dwID;
        //  코디합성 여부 확인
        if ( kSquarePeople.m_vecLookEquips[i].m_DesignCoordiID != 0 ) 
        {
            itemID = kSquarePeople.m_vecLookEquips[i].m_DesignCoordiID;
        }

        if ( g_pItemMgr->IsTransparentCoordiItem( itemID ) )
        {
            continue;
        }

        vecMerge.push_back( itemID );
    }


    //  현재 무기 전직 정보
    int nJobLevel = -1;

    for(int i = 0; i < (int)kSquarePeople.m_vecEquips.size(); i++) 
    {
        GCItem* pItemInfo = g_pItemMgr->GetItemData( kSquarePeople.m_vecEquips[i]/10 );
        if( pItemInfo == NULL ) {
            continue;
        }

        //  무기 아이템의 전직 정보를 저장해둔다.
        if ( pItemInfo->dwSlotPosition & ESP_WEAPON )
        {
            nJobLevel = pItemInfo->iNeedChangeJobLevelForEquippingItem;
        }

        bool bEquiped = false;
        std::vector<KEquipItemInfo>::iterator sit = kSquarePeople.m_vecLookEquips.begin();
        for( int j = 0 ; j < (int)kSquarePeople.m_vecLookEquips.size() ; ++j ) 
        {
            GCITEMID itemID = kSquarePeople.m_vecLookEquips[j].m_dwID;
            //  코디합성 여부 확인
            if ( kSquarePeople.m_vecLookEquips[j].m_DesignCoordiID != 0 ) 
            {
                itemID = kSquarePeople.m_vecLookEquips[j].m_DesignCoordiID;
            }

            GCItem* pLookItemInfo = g_pItemMgr->GetItemData( itemID / 10 );
            CONTINUE_NIL( pLookItemInfo );


            // 투명 코디이면 체크 안한다.
            if ( g_pItemMgr->IsTransparentCoordiItem( pLookItemInfo->dwGoodsID ) )
                continue;

            // 뭔가 장착 위치가 겹친다. 통복장이면 어쩌나?
            if( pLookItemInfo->dwSlotPosition & pItemInfo->dwSlotPosition )
            {
                if ( pItemInfo->dwSlotPosition & ESP_WEAPON )
                {
                    if ( pItemInfo->iNeedChangeJobLevelForEquippingItem ==
                        pLookItemInfo->iNeedChangeJobLevelForEquippingItem )
                    {
                        bEquiped = true;
                    }
                }
                else
                {
                    bEquiped = true;
                }
            }
        }
        if( bEquiped == false ) {
            vecMerge.push_back( kSquarePeople.m_vecEquips[i] );
        }
    }

    //  무기 전직 정보가 다른 아이템은 모두 지운다. 
    //  아바타 무기를 체크해서 기본무기랑 다른 부분은 다 지운다.
    KItemIDVector::iterator pos = vecMerge.begin();
    for ( ; pos != vecMerge.end(); )
    {
        GCItem* pItemInfo = g_pItemMgr->GetItemData( (*pos) / 10 );
        if ( pItemInfo && ( pItemInfo->dwSlotPosition & ESP_WEAPON ) )
        {
            if ( pItemInfo->iNeedChangeJobLevelForEquippingItem != nJobLevel )
            {
                pos = vecMerge.erase( pos );
                continue;
            }
        }
        ++pos;
    }

#if defined USE_COORDI_SHOP
    KItemIDVector vecTemp;
    for( std::vector<GCITEMID>::iterator it = vecMerge.begin() ; it != vecMerge.end() ; ++it )
    {
        GCITEMID itemID = (*it)/10;
        if( g_pItemMgr->IsBaseCoordiItem( itemID ) )
        {
            GCItem* pViewItem = g_pItemMgr->GetItemData( itemID );
            CONTINUE_NIL( pViewItem );

            GCITEMID BaseItemID = g_pItemMgr->GetBaseItemID( kSquarePeople.m_cCharType, static_cast<ESLOTPOSITION>(pViewItem->dwSlotPosition) );
            if( BaseItemID == UINT_MAX )
                continue;

            GCItem* pBaseItem = g_pItemMgr->GetItemData( BaseItemID );
            CONTINUE_NIL( pBaseItem );

            vecTemp.push_back( pBaseItem->dwGoodsID * 10 );

        }
        else
        {
            vecTemp.push_back( *it );
        }
    }

    vecMerge.swap( vecTemp );
#endif

    return vecMerge;

}

void GCSquare::AddSquarePeople( DWORD dwPeopleUID, const KSquareUserInfo& sInfo_ )
{
    // [1/31/2008] breadceo. 예약 리스트에 있을 수 있나!?
    for( IterReservePeople vit = m_vecPeopleReserve.begin(); vit != m_vecPeopleReserve.end(); ++vit )
    {
        std::pair< DWORD, IGCSquareObj* >& kReserve = (*vit);
        if( kReserve.first == dwPeopleUID )
        {
            return;
        }
    }

    IterPeople mit = m_mapPeople.find( dwPeopleUID );
    if( mit == m_mapPeople.end() )
    {
        GCSquarePeople* pNewPeople = new GCSquarePeople( dwPeopleUID );
		KSquareUserInfo kSquarePeople = sInfo_;

        // 화면에 보여질 캐릭터 출력한다.
        KItemIDVector vecMerge = GetEquipmentForLoadModel( kSquarePeople );
        kSquarePeople.m_vecEquips = vecMerge;

		// 20091026	[광장 세트 아이템 모션 작업] : 이동석
		int iSetItem = g_pItemMgr->GetSetItemNumberByEquipVector( kSquarePeople.m_vecEquips );
		if( iSetItem != -1 )
		{
			if( g_pItemMgr->IsSetItemMotion(iSetItem) )
			{
                kSquarePeople.m_vecEquips.clear();

				for( int i = 0 ; i < (int)vecMerge.size(); i++ )
                {
                    //std::vector<GCITEMID>& vecEquips = kSquarePeople.m_vecEquips;
                    GCItem* pItem = g_pItemMgr->GetItemData( vecMerge[i]/10 );
                    CONTINUE_NIL( pItem );
					if ( pItem->dwSlotPosition & ESP_WEAPON )
						continue;
					if ( pItem->dwSlotPosition & ESP_A_NECKLET )
                        continue;

					//# 광장에서 응원모션일 경우 손에 흔드는거
					if( g_pItemMgr->IsSetItemCheerUpMotion(iSetItem))
					{
						if ( pItem->dwSlotPosition & ESP_GLOVES )
						{
							kSquarePeople.m_vecEquips.push_back(KGCItemManager::ITEM_GWC_GLOVE * 10);
						}
					}
				
					kSquarePeople.m_vecEquips.push_back(vecMerge[i]);
				}				
			}
		}
		//========================================================================

        pNewPeople->SetPeopleInfo( kSquarePeople );
        m_mapPeople.insert( std::make_pair( dwPeopleUID, pNewPeople ) );

        if( sInfo_.m_kPetInfo.m_dwPetID != 0 )
        {
            GCSquarePet* pNewPet = new GCSquarePet();
            pNewPet->SetMaster( pNewPeople );
            m_mapPet.insert( std::make_pair( dwPeopleUID, pNewPet ) );
        }

        g_pkSquareOverlayUI->AddSquareUserListOrder( sInfo_ );

        if ( false == GetSquarePeople( dwPeopleUID )->IsLocalPlayer() )
            g_kGlobalValue.AddCurrentStateUser( sInfo_.m_strNick );
        
#ifdef ENABLE_SQUARE_LOGGING
        START_LOG( clog, L" People객체 추가 : " << dwPeopleUID );
#endif
        m_vecPeopleUID.push_back( dwPeopleUID );
    }

    g_pkUIScene->m_pkSquareOverlayUI->UpdateSquareInfo();
}

void GCSquare::AddSquareObject( ESquareLayer eLayer, IGCSquareObj* pObj )
{
    m_mapSquareObject.insert( std::make_pair(eLayer, pObj) );

    if( pObj->GetInit() == false )
        pObj->SetInit( pObj->Load() );
}

void GCSquare::AddParticle( char* pParticleName, float fPosX, float fPosY, float fPosZ )
{
	g_ParticleManager->CreateSequence( pParticleName, fPosX, fPosY, fPosZ );
}

void GCSquare::RemoveSquarePeople( DWORD dwPeopleUID )
{
    IterPeople mit = m_mapPeople.find( dwPeopleUID );
    if( mit == m_mapPeople.end() )
    {
        return;
    }

    GCSquarePeople* pPeople = static_cast< GCSquarePeople* >( mit->second );
    g_kGlobalValue.DelCurrentStateUser( pPeople->GetPeopleInfo().m_strNick );

    SAFE_DELETE( mit->second );
    m_mapPeople.erase( mit );
    std::vector< DWORD >::iterator vit = std::find( m_vecPeopleUID.begin(), m_vecPeopleUID.end(), dwPeopleUID );
    if( vit != m_vecPeopleUID.end() )
    {
        m_vecPeopleUID.erase( vit );
    }

    IterPet mitPet = m_mapPet.find( dwPeopleUID );
    if( mitPet != m_mapPet.end() )
    {
        SAFE_DELETE( mitPet->second );
        m_mapPet.erase( mitPet );
    }

    for( IterReservePeople vit = m_vecPeopleReserve.begin(); vit != m_vecPeopleReserve.end(); ++vit )
    {
        std::pair< DWORD, IGCSquareObj* >& kReserve = (*vit);
        if( kReserve.first == dwPeopleUID )
        {
            SAFE_DELETE( kReserve.second );
            m_vecPeopleReserve.erase( vit );
            break;
        }
    }

    for( IterReservePet vitPet = m_vecPetReserve.begin(); vitPet != m_vecPetReserve.end(); ++vitPet )
    {
        std::pair< DWORD, GCSquarePet* >& kReserve = (*vitPet);
        if( kReserve.first == dwPeopleUID )
        {
            SAFE_DELETE( kReserve.second );
            m_vecPetReserve.erase( vitPet );
            break;
        }
    }

#ifdef ENABLE_SQUARE_LOGGING
    START_LOG( clog, L" People객체 제거 : " << dwPeopleUID );
#endif

    g_pkSquareOverlayUI->RemoveSquareUserListOrder( dwPeopleUID );
    g_pkUIScene->m_pkSquareOverlayUI->UpdateSquareInfo();
}

bool GCSquare::SquareObjInit()
{
	SquareObjMap::iterator mmit;
	for( mmit = m_mapSquareObject.begin() ; mmit != m_mapSquareObject.end() ; ++mmit )
	{
		if( (*mmit).second->GetInit() == false )
		{
			(*mmit).second->SetInit( (*mmit).second->Load() );
			return false;
		}
	}
    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
            if ( (*vit)->GetInit() == false )
            {
                (*vit)->SetInit( (*vit)->Load() );
                return false;
            }
    }

	return true;
}

bool GCSquare::SquarePeopleInit()
{
	IterPeople mit = m_mapPeople.begin();
	for( ; mit != m_mapPeople.end(); ++mit )
	{
		if( mit->second->GetInit() == false )
		{
			mit->second->SetInit( mit->second->Load() );
			return false;
		}
	}

	return true;
}


// FrameMove를 돌면서 로딩을 하기 때문에 조금씩 로드하면 됩니다.
// 조금 로드하고 Return 시켜도 LoadingComplete가 true가 될때까지
// 계속 이 함수는 호출 됩니다.
void GCSquare::SquareLoading()
{   
    if( false == m_bConnectToSquare )
    {
        return Connect();
    }
	else
	{
		Result_Enter_Square = 0;
	}

	if( SquarePeopleInit() == false ) return;
	if( SquareObjInit() == false ) return;
    if( SquarePetInit() == false ) return;

    m_dwMaxRenderCount = (int)m_mapPeople.size();

    if( false == m_bSparkLoading )
    {
        g_MyD3D->MySparks->Load();
        m_bSparkLoading = true;
        return;
    }

    if( false == m_bSoundLoading )
    {
        // 여기에다가 기타 리소스 ( 사운드, 파티클 등 로딩하면 되겠다 )
        g_KDSound.LoadFromIndex( 0 );
        m_bSoundLoading = true;
        return;
    }

    // [1/7/2008] breadceo. 이까지 오면 로딩 끝
    LoadingComplete( true );
}

void GCSquare::ClearSquareObj()
{
	SquareObjMap::iterator mmit;
	for( mmit = m_mapSquareObject.begin() ; mmit != m_mapSquareObject.end() ; ++mmit )
	{
		SAFE_DELETE( (*mmit).second );
	}
	m_mapSquareObject.clear();

    Iter3DObject vit;
    for( vit = m_vec3DObjcet.begin() ; vit != m_vec3DObjcet.end() ; ++vit )
    {
            SAFE_DELETE( (*vit) );
    }
    m_vec3DObjcet.clear();
}

void GCSquare::CloseSquare()
{
    //IterNonPeople vit = m_vecNonPeople.begin();
    //for( ; vit != m_vecNonPeople.end(); ++vit )
    //{
    //    SAFE_DELETE( (*vit) );
    //}
    //m_vecNonPeople.clear();
    // 문제가 있는듯 ;;
	if(g_pkSquareOverlayUI)
		g_pkSquareOverlayUI->ClearSquareUserListOrder();

    IterPeople mit = m_mapPeople.begin();    
    for( ; mit != m_mapPeople.end(); ++mit )
    {
        SAFE_DELETE( mit->second );
    }
    m_mapPeople.clear();
    m_vecPeopleUID.clear();
    m_vecNearPeopleUID.clear();

    IterPet mitPet = m_mapPet.begin();
    for( ; mitPet != m_mapPet.end(); ++mitPet )
    {
        SAFE_DELETE( mitPet->second );
    }
    m_mapPet.clear();

    IterReservePeople vitReserve = m_vecPeopleReserve.begin();
    for( ; vitReserve != m_vecPeopleReserve.end(); ++vitReserve )
    {
        SAFE_DELETE( (*vitReserve).second );
    }
    m_vecPeopleReserve.clear();
    m_vecSortedPeoplebyDistance.clear();

    for( IterReservePet vitPet = m_vecPetReserve.begin(); vitPet != m_vecPetReserve.end(); ++vitPet )
    {
        SAFE_DELETE( (*vitPet).second );
    }
    m_vecPetReserve.clear();

	ClearSquareObj();
    
    SiGCBuffDonationManager()->ClearParticle();

    LoadingComplete( false );

	SAFE_RELEASE(m_pTextureGPCItem);

    m_iEventBannerType = -1;

}

bool GCSquare::SquareReservePeopleLoading()
{
	// [1/22/2008] breadceo. 예약로딩
	if( !m_vecPeopleReserve.empty() )
	{
		IterReservePeople vit = m_vecPeopleReserve.begin();
		// 하나 로딩 끝났으면
		if( true == vit->second->TimeShareLoading() )
		{
			vit->second->SetInit( true );
			vit->second->SetRender( true );
			m_mapPeople.insert( std::make_pair( vit->first, vit->second ) );
			m_vecPeopleUID.push_back( vit->first );
			GCSquarePeople* pPeople = static_cast< GCSquarePeople* >( vit->second );
			g_pkSquareOverlayUI->AddSquareUserListOrder( pPeople->GetPeopleInfo() );
			m_vecPeopleReserve.erase( vit );

			if ( false == GetSquarePeople( pPeople->GetPeopleInfo().m_dwUID )->IsLocalPlayer() )
				g_kGlobalValue.AddCurrentStateUser( pPeople->GetPeopleInfo().m_strNick );

#ifdef ENABLE_SQUARE_LOGGING
			START_LOG( clog, L" TimeShareLoading 끝 People객체 추가 : " << vit->first );
#endif
			g_pkUIScene->m_pkSquareOverlayUI->UpdateSquareInfo();
		}
	}

    if( !m_vecPetReserve.empty() )
    {
        IterReservePet vit = m_vecPetReserve.begin();
        if( true == vit->second->TimeShareLoading() )
        {
            vit->second->SetInit( true );
            vit->second->SetRender( true );
            m_mapPet.insert( (*vit) );
            m_vecPetReserve.erase( vit );
        }
    }

	return true;
}

void GCSquare::FrameMove_People()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

	IterPeople mit = m_mapPeople.begin();
	for( ; mit != m_mapPeople.end(); ++mit )
	{
		mit->second->FrameMove();
	}
}

void GCSquare::FrameMove_Object()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

	SquareObjMap::iterator mmit;
	for( mmit = m_mapSquareObject.begin() ; mmit != m_mapSquareObject.end() ; ++mmit )
	{
		(*mmit).second->FrameMove();
	}

    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );
    g_MyD3D->MyCtrl->Picking( vMousePos.x, vMousePos.y );

    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
            (*vit)->FrameMove();
            if ( (*vit)->Picking( vMousePos, KInput::MBLEFT ) )
            {
                (*vit)->SetEdgeColor( EDGE_COLOR_ENEMY );
            }
            else
            {
                (*vit)->SetEdgeColor( EDGE_COLOR_FRIEND );
            }
    }
}

void GCSquare::FrameMove()
{
    ++StartGameTime;
	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_FIREWORKS))
	{
		if(!m_bCreateFireWorks)
		{
			CreateFireWorks();
		}
	}else{
		if(m_bCreateFireWorks)
		{
			DeleteFireWorks();
		}
	}

	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_CAMPFIRE))
	{
		if(!m_bCreateCampFire)
		{
			CreateCampFire();
		}
	}else{
		if(m_bCreateCampFire)
		{
			DeleteCampFire();
		}
	}

    // 화령 축제 파티클
    SiGCBuffDonationManager()->UpdateParticles();

    g_MyD3D->Story++;
    CheckPerformance();

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
	lua_checkstack( L, 10 );
	lua_pushstring( L, "SquareFrameMove" );
	lua_rawget( L, LUA_GLOBALSINDEX );
	if( lua_type( L, -1 ) == LUA_TFUNCTION )
	{
		lua_pcall( L, 0, 0, 0 );
	}
	else
	{
		lua_pop( L, 1 );
	}

	FrameMove_People();
	FrameMove_Object();
    FrameMove_Effect();
    KGCPC_BEGIN("FrameMove_Pet");
    FrameMove_Pet();
    KGCPC_END();

	SquareReservePeopleLoading();
    
    SiKGCEmoticonManager()->FrameMove();

	//g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );

#ifdef TIME_SHARE_LOADING_TEST
    if( g_MyD3D->MyCtrl->k_Fresh_Joke == true )
    {
        // ENABLE_SOLO_TEST 랑 별개로 리소스 로딩만 테스트 해보기 위한코드
        g_MyD3D->MyCtrl->k_Fresh_Joke = false;
        GCSquarePeople* pLocalPeople = static_cast<GCSquarePeople*>( GetLocalPeople() );
        int idx = 1;
        while( true )
        {
            GCSquarePeople* pTestPeople = GetSquarePeople( pLocalPeople->GetUID() + idx );
            if( pTestPeople == NULL )
            {
                AddSquarePeopleReserve( pLocalPeople->GetUID() + idx, pLocalPeople->GetPeopleInfo() );
                break;
            }
            idx++;
        }
    }
#endif
}

void GCSquare::Render()
{
    m_dwSumOfFrame += m_iCurFPS;
    m_dwSumCount++;

// 광장에 배경은 발판에 붙어있다!  발판 뒤에 레이어에 무슨짓을 하더라도 안보일껄?! ㅋㅋㅋㅋ
// 	SiKGCFantasticMap()->Render( GC_LAYER_SKY  );
//     g_ParticleManager->Render( GC_LAYER_SKY );
//     g_ParticleManager->Render( GC_LAYER_BETWEEN_SKY_BLOCK );
    SiKGCFantasticMap()->Render( GC_LAYER_BLOCK );
    g_ParticleManager->Render( GC_LAYER_BLOCK );
    g_ParticleManager->Render( GC_LAYER_BETWEEN_BLOCK_CHAR );
    ShadowRender();
		
			//if(GetLocalPeople()->GetPos().y >= 3.0f)
			//{
			//	g_ParticleManager->Render( GC_LAYER_BETWEEN_SKY_BLOCK );
			//}
		
    ObjectRender();

	g_ParticleManager->Render( GC_LAYER_BETWEEN_BLOCK_CHAR );

}

void GCSquare::ObjectRender()
{
	Render_Object( SL_BEFORE_PEOPLE );
	Render_Peoples();
}

void GCSquare::Render_Peoples()
{
	IterPeople mit = m_mapPeople.begin();
	for( ; mit != m_mapPeople.end(); ++mit )
	{
		mit->second->Render();
	}
}

void GCSquare::Render_Object( ESquareLayer eLayer )
{
	std::pair< SquareObjMap::iterator, SquareObjMap::iterator > mmit;
	mmit = m_mapSquareObject.equal_range( eLayer );

	SquareObjMap::iterator mit;
	for( mit = mmit.first; mit != mmit.second ; mit++ )
	{
		(*mit).second->Render();
	}


    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
            (*vit)->Render();
    }

#if defined(GP_CAPSULE)
	RenderGPCItem();
#endif
}

DWORD GCSquare::GetNextSquarePeopleUID( DWORD dwUID ) const
{
    ASSERT( !m_mapPeople.empty() );

    ConstIterPeople mit = m_mapPeople.find( dwUID );
    if( mit == m_mapPeople.end() )
    {
        mit = m_mapPeople.begin();
        return mit->first;
    }
    else
    {
        mit++;
        if( mit == m_mapPeople.end() )
        {
            mit = m_mapPeople.begin();
        }
        return mit->first;
    }
}

GCSquarePeople* GCSquare::GetSquarePeople( DWORD dwUID ) const
{
    ConstIterPeople mit = m_mapPeople.find( dwUID );
    if( mit != m_mapPeople.end() )
    {
        return static_cast<GCSquarePeople*>(mit->second);
    }
    else
    {
        // [1/8/2008] breadceo. 내 클라이언트에 없을 수도 있다
        return NULL;
    }
}

GCSquarePeople* GCSquare::GetSquarePeopleByChracterNickName( const std::wstring& strCharNickName_ ) const
{
    std::map< DWORD, IGCSquareObj* >::const_iterator mit = m_mapPeople.begin();
    for( ; mit != m_mapPeople.end() ; ++mit ) { 
        GCSquarePeople* pPeople = static_cast< GCSquarePeople* >( mit->second );
        if( 0 == pPeople->GetPeopleInfo().m_strCharNickName.compare( strCharNickName_ ) ) { 
            return pPeople;
        }
    }

    return NULL;
}

void GCSquare::LoadingComplete( bool bComplete )
{
    m_bLoadingComplete = bComplete;
    m_bSparkLoading = bComplete;
    m_bSoundLoading = bComplete;
    m_bConnectToSquare = bComplete;
    m_bSendReq = bComplete;
}

void GCSquare::ShadowRender()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        IterPeople mit = m_mapPeople.begin();
        for( ; mit != m_mapPeople.end(); ++mit )
        {
            mit->second->ShadowRender();
        }

        IterPet mitPet = m_mapPet.begin();
        for( ; mitPet != m_mapPet.end(); ++mitPet )
        {
            mitPet->second->ShadowRender();
        }
    }
    g_pGCDeviceManager2->PopState();
}

void GCSquare::SparkRender()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      FALSE );
        g_MyD3D->MySparks->Render();
    }
    g_pGCDeviceManager2->PopState();
}

void GCSquare::AddSquarePeopleReserve( DWORD dwPeopleUID, const KSquareUserInfo& sInfo_ )
{
	for( IterReservePeople vit = m_vecPeopleReserve.begin(); vit != m_vecPeopleReserve.end(); ++vit )
	{
		std::pair< DWORD, IGCSquareObj* >& kReserve = (*vit);
		if( kReserve.first == dwPeopleUID )
		{
			kReserve.second->SetPos( D3DXVECTOR3( sInfo_.m_kSquarePos.m_fXpos, sInfo_.m_kSquarePos.m_fYpos, 0.0f ) );
			return;
		}
	}

	// 이미 있으면 리턴
	if( m_mapPeople.end() != m_mapPeople.find( dwPeopleUID ) )
		return;

	GCSquarePeople* pNewPeople = new GCSquarePeople( dwPeopleUID );
	KSquareUserInfo kSquarePeople = sInfo_;

    // 화면에 보여질 캐릭터 출력한다.
    KItemIDVector vecMerge = GetEquipmentForLoadModel( kSquarePeople );
#if defined USE_COORDI_SHOP
    std::vector<GCITEMID> vecTemp;
    for( std::vector<GCITEMID>::iterator it = vecMerge.begin() ; it != vecMerge.end() ; ++it )
    {
        GCITEMID itemID = (*it)/10;
        if( g_pItemMgr->IsBaseCoordiItem( itemID ) )
        {
            GCItem* pViewItem = g_pItemMgr->GetItemData( itemID );
            CONTINUE_NIL( pViewItem );

            GCITEMID BaseItemID = g_pItemMgr->GetBaseItemID( kSquarePeople.m_cCharType, static_cast<ESLOTPOSITION>(pViewItem->dwSlotPosition) );
            GCItem* pBaseItem = g_pItemMgr->GetItemData( BaseItemID );
            CONTINUE_NIL( pBaseItem );

            vecTemp.push_back( pBaseItem->dwGoodsID * 10 );

        }
        else
        {
            vecTemp.push_back( *it );
        }
    }

    vecMerge.swap( vecTemp );
#endif
	kSquarePeople.m_vecEquips = vecMerge;

	// 20091026	[광장 세트 아이템 모션 작업] : 이동석
	int iSetItem = g_pItemMgr->GetSetItemNumberByEquipVector( kSquarePeople.m_vecEquips );
	if( iSetItem != -1 )
	{
		if( g_pItemMgr->IsSetItemMotion(iSetItem) )
		{
			kSquarePeople.m_vecEquips.clear();

			for( int i = 0 ; i < (int)vecMerge.size(); i++ )
			{
				//std::vector<GCITEMID>& vecEquips = kSquarePeople.m_vecEquips;
                GCItem* pItem = g_pItemMgr->GetItemData( vecMerge[i]/10 );
                CONTINUE_NIL( pItem );
				if ( pItem->dwSlotPosition & ESP_WEAPON )
					continue;
				if ( pItem->dwSlotPosition & ESP_A_NECKLET )
					continue;

				//# 광장에서 응원모션일 경우 손에 흔드는거
				if( g_pItemMgr->IsSetItemCheerUpMotion(iSetItem))
				{
					if ( pItem->dwSlotPosition & ESP_GLOVES )
					{
						kSquarePeople.m_vecEquips.push_back(KGCItemManager::ITEM_GWC_GLOVE * 10);
					}
				}

				kSquarePeople.m_vecEquips.push_back(vecMerge[i]);
			}				
		}
	}

	pNewPeople->SetPeopleInfo( kSquarePeople );
	pNewPeople->SetPos( D3DXVECTOR3( sInfo_.m_kSquarePos.m_fXpos, sInfo_.m_kSquarePos.m_fYpos, 0.0f ) );
	m_vecPeopleReserve.push_back( std::make_pair( dwPeopleUID, pNewPeople ) );

	if( sInfo_.m_kPetInfo.m_dwPetID != 0 )
	{
		GCSquarePet* pNewPet = new GCSquarePet();
		pNewPet->SetMaster( pNewPeople );
		m_vecPetReserve.push_back( std::make_pair( dwPeopleUID, pNewPet ) );
	}
}

void GCSquare::UpdateSquarePeoplePosition( const std::vector<KPosUID>& vecPosInfo )
{
    std::vector<KPosUID>::const_iterator vit;
    for( vit = vecPosInfo.begin(); vit != vecPosInfo.end(); ++vit )
    {
        const KPosUID& Info = (*vit);
        GCSquarePeople* pPeople = GetSquarePeople( Info.m_dwUID );
        if( NULL != pPeople )
        {
            pPeople->SetPos( D3DXVECTOR3( Info.m_fXPos, Info.m_fYPos, 0.0f ) );
        }
    }
}

float GCSquare::GetLoadPercent() const
{
    if( false == m_bConnectToSquare || ( m_mapPeople.empty() && m_mapPet.empty() ) )
    {
        return 0.0f;
    }

    int iLoadComplete = 0;
    ConstIterPeople mit = m_mapPeople.begin();
    for( ; mit != m_mapPeople.end(); ++mit )
    {
        if( mit->second->GetInit() == true )
        {
            iLoadComplete++;
        }
    }

    ConstIterPet mitPet = m_mapPet.begin();
    for( ; mitPet != m_mapPet.end(); ++mitPet )
    {
        if( mitPet->second->GetInit() == true )
        {
            iLoadComplete++;
        }
    }

    float fPeopleLoading = static_cast<float>( iLoadComplete ) / 
                           static_cast<float>( m_mapPeople.size() + m_mapPet.size() ) * 0.7f;

    if( true == m_bConnectToSquare )
    {
        fPeopleLoading += 0.1f;
    }
    if( true == m_bSparkLoading )
    {
        fPeopleLoading += 0.1f;
    }
    if( true == m_bSoundLoading )
    {
        fPeopleLoading += 0.1f;
    }

    return fPeopleLoading;
}

void GCSquare::Connect()
{
    if( false == m_bConnectToSquare && false == m_bSendReq )
    {
        if( g_MyD3D->m_pStateMachine->IsJoinedChannel() )
        {
            g_MyD3D->LeaveChannel();
            g_MyD3D->m_pStateMachine->SetJoinChannel( false );
        }
        else if( g_MyD3D->m_pStateMachine->IsJoinedGuild() )
        {
            g_MyD3D->LeaveGuildRoom();
            g_MyD3D->m_pStateMachine->SetJoinGuild( false );
        }

		g_pkUIScene->m_pkSquareLoadingScene->RenderLoadingType();

		if( m_iSquareType == KSquareInfo::ST_GUILD )
			SiKP2P()->Send_EnterSquareReq( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType, -1, ST_GUILD, m_iGuildSquareIndex );
		else
			SiKP2P()->Send_EnterSquareReq( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType, GetSquareID() );

        m_bSendReq = true;
        return;
    }
    else if( true == m_bSendReq && INT_MAX == Result_Enter_Square ) // 아직 패킷 못받은거
    {
        return;
    }
    else if( Result_Enter_Square == 3 )
    {
        g_MyD3D->m_pStateMachine->GoState( GS_SQUARE_END );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_CHANNEL2 ),
                                                         L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
    else if( Result_Enter_Square != 0 )
    {
        g_MyD3D->m_pStateMachine->GoState( GS_SQUARE_END );

        std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_NPGG_RECONNECT );
        strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
            KUserEvent::EVENT_ENTER_SQUARE_ACK, Result_Enter_Square );	

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RELAY_SERVER_CONNECT_FAILED ),
                                                         strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    // [1/19/2008] breadceo. 원래는 UDP Relay Server를 써야되지만 지금은 TCP 로
    
    if( !ConnectTRServer() )
    {
        // 광장도 떠나야함
        SiKP2P()->Send_LeaveSquareReq();
        g_MyD3D->m_pStateMachine->GoState( GS_SQUARE_END );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RELAY_SERVER_CONNECT_FAILED ),
            g_pkStrLoader->GetString( STR_ID_NPGG_RECONNECT ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
    m_bConnectToSquare = true;
    Result_Enter_Square = INT_MAX;
}


bool GCSquare::ConnectTRServer()
{
    // [1/19/2008] breadceo. 원래는 UDP Relay Server를 써야되지만 지금은 TCP 로
    in_addr in;
    in.S_un.S_addr = GetRelayServerIP();
    SiKP2P()->DisConnectTCPRelay();
    if ( !SiKP2P()->m_spTRUserProxy->Connect( inet_ntoa(in), GetRelayServerPort(), g_kGlobalValue.ServerInfo.dwUserUID ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_TCP_CONNECT_FAIL), L"" );
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TCP_CONNECT_ERROR );        
    }

    SiKP2P()->m_spTRUserProxy->SetPingTimeoutLimit( GetRelayPingLimit() );
    SiKP2P()->m_spTRUserProxy->SetPingSendGap( GetRelayPingInterval() );

    // [1/19/2008] breadceo. 서버와 연결이 안되면 에러!
    if( false == SiKP2P()->m_spTRUserProxy->IsConnected() )
    {
        return false;
    }
    return true;
}

#define PERFORMANCE_DOWN_FRAME (15)
void GCSquare::CheckPerformance()
{
    static DWORD s_dwAvg = 0;

    if( 0 == m_dwPrevCheckTime )
    {
        m_dwPrevCheckTime = ::timeGetTime();
    }

    // 3초동안 누적해서 평균 프레임이 15이하면 하나씩 줄인다
    if( ::timeGetTime() - m_dwPrevCheckTime >= (DWORD)3000 )
    {
        SortingbyDistance();
        s_dwAvg = (DWORD)( m_dwSumCount == 0 ? 0 : m_dwSumOfFrame / m_dwSumCount );
        if( s_dwAvg <= (DWORD)PERFORMANCE_DOWN_FRAME )
        {
            // 줄인다
            m_dwMaxRenderCount -= 3;
            if( m_dwMaxRenderCount < 1 )
            {
                m_dwMaxRenderCount = 1;
            }
        }
        else
        {
            // 늘린다
            m_dwMaxRenderCount++;
            if( m_dwMaxRenderCount > (DWORD)m_mapPeople.size() )
            {
                m_dwMaxRenderCount = (DWORD)m_mapPeople.size();
            }
        }
        m_dwSumCount = 0;
        m_dwSumOfFrame = 0;
        m_dwPrevCheckTime = ::timeGetTime();
        PerformanceUp();
    }
}

class KLessFunc
{
public:
    KLessFunc() {}
    ~KLessFunc() {}

    bool operator()(const std::pair< DWORD, float >& _Left, const std::pair< DWORD, float >& _Right) const
    {
        return (_Left.second < _Right.second);
    }
};

void GCSquare::SortingbyDistance()
{
    GCSquarePeople* pLocal = GetSquarePeople( g_kGlobalValue.m_kUserInfo.dwUID );

    std::vector< std::pair< DWORD, float > > vecUIDDistance;
    for( IterPeople mit = m_mapPeople.begin(); mit != m_mapPeople.end(); ++mit )
    {
        if( mit->first == g_kGlobalValue.m_kUserInfo.dwUID )
        {
            continue;
        }

        D3DXVECTOR3 vDistance( mit->second->GetPos().x - pLocal->GetPos().x,
            mit->second->GetPos().y - pLocal->GetPos().y, 0 );
        float fDist = D3DXVec3Length( &vDistance );
        // 일정거리 이상은 그리지 않겠음 대략 한화면 정도
        if( fDist > 3.0f )
            continue;
        vecUIDDistance.push_back( std::make_pair( mit->first, fDist ) );
    }

    std::sort( vecUIDDistance.begin(), vecUIDDistance.end(), KLessFunc() );

    // 가까운 순부터
    m_vecSortedPeoplebyDistance.clear();
    for( int i = 0; i < (int)vecUIDDistance.size(); ++i )
    {
        m_vecSortedPeoplebyDistance.push_back( vecUIDDistance[i].first );
    }
}

void GCSquare::PerformanceUp()
{
    IterPeople mitMe = m_mapPeople.find( g_kGlobalValue.m_kUserInfo.dwUID );
    ASSERT( mitMe != m_mapPeople.end() );
    GCSquarePeople* pLocal = static_cast<GCSquarePeople*>(mitMe->second);
    pLocal->SetRender( true );

    for( IterPeople mit = m_mapPeople.begin(); mit != m_mapPeople.end(); ++mit )
    {
        if( mit->first == g_kGlobalValue.m_kUserInfo.dwUID )
        {
            continue;
        }

        mit->second->SetRender( false );
    }

    // 결국 넌 나만 그려지겠군..
    if( m_dwMaxRenderCount == 1 )
    {
        return;
    }

    m_dwMaxRenderCount = min( m_dwMaxRenderCount, (DWORD)m_vecSortedPeoplebyDistance.size() );

    for( int i = 0; i < (int)m_dwMaxRenderCount; ++i )
    {
        GCSquarePeople* pPeople = GetSquarePeople( m_vecSortedPeoplebyDistance[i] );
        if( pPeople != NULL )
        {
            pPeople->SetRender( true );
        }
    }
}

KSquareInfo GCSquare::GetCurrentSquareInfo()
{
    ASSERT( !m_vecSquareInfo.empty() );

    std::vector< KSquareInfo >::iterator vit;
    vit = std::find_if( m_vecSquareInfo.begin(), m_vecSquareInfo.end(), 
        boost::bind( &KSquareInfo::m_dwID, _1 ) == m_dwSquareID );

    if( vit != m_vecSquareInfo.end() )
    {
        // [1/28/2008] breadceo. 부를때마다 업데이트
        vit->m_dwCurrentUserNum = (DWORD)m_mapPeople.size();
        return (*vit);
    }
    ASSERT( !"Invalid SquareID" );
    return (*m_vecSquareInfo.begin());
}

void GCSquare::Picking( OUT std::vector< KSquareUserInfo >& vecRet, const int iMouseBtn  )
{
    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );

    if(g_pkSquareOverlayUI->m_pkShortCutBox->CheckPosInWindowBoundWithChild(vMousePos))
        return;

    g_MyD3D->MyCtrl->Picking( vMousePos.x, vMousePos.y );

    if( iMouseBtn == KInput::MBRIGHT )
    {
        vecRet.clear();

        for( ConstIterPeople mitPeople = m_mapPeople.begin(); mitPeople != m_mapPeople.end(); ++mitPeople )
        {
            GCSquarePeople* pPeople = static_cast< GCSquarePeople* >( mitPeople->second );
            if( true == pPeople->Picking( vMousePos, iMouseBtn ) )
            {
                vecRet.push_back( pPeople->GetPeopleInfo() );
            }
        }
    }

    SquareObjMap::iterator mmit;
    for( mmit = m_mapSquareObject.begin() ; mmit != m_mapSquareObject.end() ; ++mmit )
    {
        (*mmit).second->Picking( vMousePos, iMouseBtn );
    }

    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
        if ( !(*vit)->Picking( vMousePos, iMouseBtn ) )
            continue;

        if ( (*vit)->GetSubType() == SOST_MINIGAME_NPC )
        {            
            KP2P::GetInstance()->Send_MiniGameMyRank();
            KP2P::GetInstance()->Send_MiniGameTopRank( 0 );
            KP2P::GetInstance()->Send_MiniGameTopRank( 1 );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MINIGAME_DLG );            
        }

        if ( (*vit)->GetSubType() == SOST_GP_CAPSULE_NPC || (*vit)->GetSubType() == SOST_GP_CAPSULE_NPC2)
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GP_CAPSULE_DLG );
        }

        if ( (*vit)->GetSubType() == SOST_COUPON_NPC )
        {
            if( g_pkUIScene->m_pkCouponNPCDlg->m_iCountInput > 5 )
            {	
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COUPON_NP_OVER_INCORRECT_INPUT), L"", 
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPON_NPC_DLG );
            }			
        }

        if ( (*vit)->GetSubType() == SOST_GUILD_JOIN_NPC )
        {
#ifndef  _OPEN_TEST_
            KP2P::GetInstance()->Send_SearchGuildListReq(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, L"");
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_LIST_DLG );						
#endif
        }
        //	파란 산타
        if ( (*vit)->GetSubType() == SOST_SUBSCRIPTION_NPC1 )
        {
            KP2P::GetInstance()->Send_CurrentSubscriptionGiftNumReq();
            g_pkUIScene->m_pkSubscriptionDlg->SetBagType(  SOST_SUBSCRIPTION_NPC1  );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUBSCRIPTION_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, SOST_SUBSCRIPTION_NPC1 );			
        }

        // 빨간 산타
        if ( (*vit)->GetSubType() == SOST_SUBSCRIPTION_NPC2 )
        {
            KP2P::GetInstance()->Send_CurrentSubscriptionGiftNumReq();
            g_pkUIScene->m_pkSubscriptionDlg->SetBagType(SOST_SUBSCRIPTION_NPC2 );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUBSCRIPTION_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, SOST_SUBSCRIPTION_NPC2 );			
        }

        //	찢어진 산타~ 가아니라 황금산타... 이건 이거대로 무섭군.
        if ( (*vit)->GetSubType() == SOST_SUBSCRIPTION_NPC3 )
        {
            KP2P::GetInstance()->Send_CurrentSubscriptionGiftNumReq();
            g_pkUIScene->m_pkSubscriptionDlg->SetBagType(SOST_SUBSCRIPTION_NPC3 );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUBSCRIPTION_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, SOST_SUBSCRIPTION_NPC3 );			
        }


        if ( (*vit)->GetSubType() == SOST_RAINBOW_NPC )
        {
            if( g_kGlobalValue.m_bIsRainbowEvent )
            {
                if( g_kGlobalValue.m_bJackPot )
                    KP2P::GetInstance()->Send_JackPotItemReq();
                else
#if defined(FULLMOON_EVENT)
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_FULLMOON_SQUARE) );	
#else
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_RAINBOW_SQUARE) );	
#endif
            }
        }

        if ( (*vit)->GetSubType() == SOST_CONTRIBUTION_NPC )
        {
            // Square3DObject.lua NPC 속성 중에 LPARAM 값으로 기부 타입을 정해 주세요.
            SiKGCContributionManager()->SetNPCType( (*vit)->m_dwLParam );
            KP2P::GetInstance()->Send_TongDonationInfoReq( (*vit)->m_dwLParam );
            //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CONTRIBUTION_DLG );
        }

        //if ( (*vit)->GetSubType() == SOST_HAPPYNEWYEAR_NPC )
        //{
        //    if( KP2P::GetInstance()->Send_LunaNewYearReq() )
        //    {
        //        int iResult_LunaNewYearReq = -1;
        //        g_MyD3D->WaitForServerAck( iResult_LunaNewYearReq, INT_MAX, 0xffffffff, TIME_OUT_VALUE );
        //    }
        //}

        //// 캐릭터 시장 선거 이벤트
        //if ( (*vit)->GetSubType() == SOST_CHARACTER_NPC )
        //{
        //    if(SiKGCValentineDayManager()->GetPollState() )
        //    {
        //        if( FALSE == SiKGCValentineDayManager()->GetPollingNow())
        //        {
        //            KItem* pkInvenItem = NULL;

        //            if(g_pItemMgr->FindInventoryForItemID(SiKGCValentineDayManager()->GetPollItemID(), &pkInvenItem))
        //                SiKGCValentineDayManager()->SetPollItemCurrentCount(pkInvenItem->m_nCount);
        //            else 
        //                SiKGCValentineDayManager()->SetPollItemCurrentCount( 0 );

        //            SiKGCValentineDayManager()->SetPollingNow( TRUE );
        //            SiKGCValentineDayManager()->SetSelectedCharType( (*vit)->m_egcCharType );
        //            SiKGCValentineDayManager()->SetSelectedCharString(g_pkStrLoader->GetString((*vit)->m_dwNickNameStringID));
        //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_VALENTINEDAY_DLG);
        //        }
        //    }
        //    else
        //    {
        //        //현재 투표기간이 아님
        //        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VALENTINE_DAY_ERROR1), L"",
        //            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        //    }
        //}

//        if( (*vit)->GetSubType() == SOST_GWC_NPC )
//        {
//#if defined (GWC_EVENT_2011)
//            SendGwcBoardReq();
//#else
//            KP2P::GetInstance()->Send_GWCEventResultReq();
//#endif
//        }

        //if( (*vit)->GetSubType() == SOST_NEWYEAR_NPC )
        //{
        //    KP2P::GetInstance()->Send_NewYearDonationInfoReq((*vit)->m_dwLParam);

        //}

        if( (*vit)->GetSubType() == SOST_ITEM_WORKING_SELECT_NPC )
        {
            if ( SiKGCPartyManager()->IsParty() )
                SiKGCPartyManager()->LeaveParty();
            //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FORGE_DLG );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FORGE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType );
        }

        if ( (*vit)->GetSubType() == SOST_VOTE_NPC )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TOURNAMENT_BOX, g_pkStrLoader->GetReplacedString(STR_ID_VOTE_NICKNAME, "s", 
                g_pkStrLoader->GetString((*vit)->GetNickNameStringID()).c_str()),L"",        
                KGCUIScene::GC_MBOX_USE_TOURNAMENT_VOTE, (*vit)->m_dwLParam,(*vit)->m_dwRParam , true);
        }

        if( (*vit)->GetSubType() == SOST_NEW_ENCHANT_MAIN_DLG_NPC )
        {
            if ( SiKGCPartyManager()->IsParty() )
                SiKGCPartyManager()->LeaveParty();
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_S6_DLG );
            
        }
        if( (*vit)->GetSubType() == SOST_ITEM_TRADE_NPC )
        {
            DWORD dwItemID = g_MyD3D->m_kItemMgr.GetTradeableItemID();

            if( dwItemID != 0 ) {
                Result_ItemTrade = INT_MAX;
                KP2P::GetInstance()->Send_ItemTradeReq(g_MyD3D->m_kItemMgr.GetItemTradeInfo( dwItemID ).first, 
                    g_MyD3D->m_kItemMgr.GetItemTradeInfo( dwItemID ).second, 0);
                g_MyD3D->WaitForServerAck( Result_ItemTrade, INT_MAX, 2000, TIME_OUT_VALUE );
            } else {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_LEVEL_DOWN_ERROR3), L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
            }

        }
    }
}

void GCSquare::RefreshRenderTech()
{
    switch( g_MyD3D->m_KGCOption.GetQualityCount() )
    {
    case 6:
        g_RenderManager->SetRenderTech(0);
        break;
    case 5:
        g_RenderManager->SetRenderTech(1);
        break;
    default:
        g_RenderManager->SetRenderTech(2);
        break;
    }
}

void GCSquare::ClearResourceOnSquare()
{
	g_MyD3D->MySparks->RemoveAll();
    g_MyD3D->MySparks->UnLoad();
    g_pkSquareOverlayUI->DeleteAllSayBalloon();

	if(m_bCreateCampFire)
		DeleteCampFire();

	if(m_bCreateFireWorks)
		DeleteFireWorks();

    g_MyD3D->m_pPetAnim->ClearInstList();
    g_ParticleManager->DeleteAllInstance();
    g_AnimManager.StartGame();
    g_KDSound.FreeGameSound();
}

bool GCSquare::LoadObjectScript()
{

    if( GetSquareType() == KSquareInfo::ST_GUILD )
		return GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "SquareObject_guild.lua" );
	else
#if defined(LOADING_IMAGE_FROM_SERVER_SCRIPT)				
		return GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), KncUtil::toNarrowString(g_kGlobalValue.GetScriptNameSquareObject()) );
#else
		return GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "SquareObject.lua" );
#endif

	if(-1 != m_iEventBannerType)
    {
        KP2P::GetInstance()->Send_AdvertisingExposureCountNot(m_iEventBannerType);
    }

    return true;
}

KGCSquare2DObj* GCSquare::Create2DObject()
{
	return new KGCSquare2DObj();
}

IGCSquareObj* GCSquare::GetLocalPeople()
{
	if( m_mapPeople.count( g_kGlobalValue.m_kUserInfo.dwUID ) == 0 )
		return NULL;

	return m_mapPeople[g_kGlobalValue.m_kUserInfo.dwUID];
}

D3DXVECTOR3* GCSquare::GetWorldMousePos()
{
	const POINT& ptMouse = g_pkInput->GetMousePos();
	D3DXVECTOR3 vMousePos( (float)ptMouse.x, (float)ptMouse.y, 0.0f );
	g_MyD3D->MyCtrl->UpdateMousePos( vMousePos );

	return &g_MyD3D->MyCtrl->m_vMousePos;
}
int GCSquare::GetGuildLevel()
{
#if defined (GUILD_STORE_S4)
    if  (g_pkUIScene->m_pkGuildShop == NULL)
        return 0;

    int iGLevel = 0;
    int iGPoint = g_kGlobalValue.m_kGuildInfo.m_dwPoint;
    int icontribution = g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint;
    

    std::map<USHORT,int>::iterator iterTable;
    for (iterTable = g_kGlobalValue.m_mapGuildLevelTable.begin() ; iterTable != g_kGlobalValue.m_mapGuildLevelTable.end() ; ++iterTable)
    {
        if (iterTable->second > iGPoint)
            break;
        else
            iGLevel = iterTable->first;
    }

    return iGLevel;
#endif

    return -1;
}

void GCSquare::ShowMessageBox()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GUILD_NO_GUILDLEVELLIMIT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , false, true );
}

void GCSquare::FrameMove_Effect()
{
    //if( false == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //    return;

    g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );
    g_AnimManager.FrameMove();
    //g_MyD3D->m_pMapAnim->FrameMove( GC_ELAPSED_TIME );
    g_MyD3D->m_pPetAnim->FrameMove( GC_ELAPSED_TIME );
    g_pkPetAI->FrameMove();
}

void GCSquare::Render_Effect()
{
    //if( false == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //    return;
    
    g_AnimManager.Render();
    g_ParticleManager->Render( GC_LAYER_BETWEEN_CHAR_FRONT );
}

void GCSquare::Render_PetAnim()
{
    //if( false == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //    return;

    g_ParticleManager->Render( GC_LAYER_CHAR );
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        g_MyD3D->m_pPetAnim->Render();
    }
    g_pGCDeviceManager2->PopState();

    //g_MyD3D->m_pMapAnim->Render();
}

void GCSquare::Render_Damage()
{

}

void GCSquare::Render_EtcParticle()
{
    g_ParticleManager->Render( GC_LAYER_FRONT_MAP );
    g_ParticleManager->Render( GC_LAYER_FACE );
    g_ParticleManager->Render( GC_LAYER_UI );
	g_ParticleManager->Render( GC_LAYER_CUSTOM2 ); //그외 광장 전체 이펙트
}

void GCSquare::ResetText()
{
	IterPeople mit = m_mapPeople.begin();
	for( ; mit != m_mapPeople.end(); ++mit )
	{
		static_cast<GCSquarePeople*>(mit->second)->SetPlayerName();
		static_cast<GCSquarePeople*>(mit->second)->ResetPetName();

	}
}

void GCSquare::Render_Text()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        IterPeople mit = m_mapPeople.begin();
        for( ; mit != m_mapPeople.end(); ++mit )
        {
            static_cast<GCSquarePeople*>(mit->second)->DrawNickName();
            static_cast<GCSquarePeople*>(mit->second)->DrawGCClubMark();
			static_cast<GCSquarePeople*>(mit->second)->DrawGuildMark();
        }

        IterPet mitPet = m_mapPet.begin();
        for( ; mitPet != m_mapPet.end(); ++mitPet )
        {
            static_cast<GCSquarePet*>(mitPet->second)->RenderPetName();
        }
    }
    g_pGCDeviceManager2->PopState();
}

GCSquarePeople* GCSquare::GetSquareReservePeople( DWORD dwUID ) const
{
    if( m_vecPeopleReserve.empty() )
        return NULL;

    ConstIterReservePeople vit;
    for( vit = m_vecPeopleReserve.begin(); vit != m_vecPeopleReserve.end(); ++vit )
    {
        if( (*vit).first == dwUID )
        {
            return static_cast<GCSquarePeople*>( (*vit).second );
        }
    }

    return NULL;
}

bool GCSquare::SquarePetInit()
{
    IterPet mitPet = m_mapPet.begin();
    for( ; mitPet != m_mapPet.end(); ++mitPet )
    {
        if( mitPet->second->GetInit() == false )
        {
            mitPet->second->SetInit( mitPet->second->Load() );
            return false;
        }
    }

    return true;
}

void GCSquare::FrameMove_Pet()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    IterPet mitPet = m_mapPet.begin();
    for( ; mitPet != m_mapPet.end(); ++mitPet )
    {
        mitPet->second->FrameMove();
    }
}

GCSquarePet* GCSquare::GetSquarePetPtr( DWORD dwUID )
{
    ConstIterPet cmit = m_mapPet.find( dwUID );
    if( cmit == m_mapPet.end() )
    {
        ConstIterReservePet cvit = m_vecPetReserve.begin();
        for( ; cvit != m_vecPetReserve.end(); ++cvit )
        {
            const std::pair< DWORD, GCSquarePet* >& prPet = (*cvit);
            if( prPet.first == dwUID )
            {
                return prPet.second;
            }
        }

        ASSERT( cvit != m_vecPetReserve.end() );
    }

    return cmit->second;
}

void GCSquare::Load3DObjcetScript( void )
{
    KLuaManager luaMgr;

	if( GetSquareType() == KSquareInfo::ST_GUILD )
	{
		if( GCFUNC::LoadLuaScript( luaMgr , "Square3DObject_guild.lua" ) == false )
			return ;
	}
	else
	{
#if defined(LOADING_IMAGE_FROM_SERVER_SCRIPT)
		if( GCFUNC::LoadLuaScript( luaMgr , KncUtil::toNarrowString(g_kGlobalValue.GetScriptNameSquare3DObject()) ) == false )
#else
		if( GCFUNC::LoadLuaScript( luaMgr , "Square3DObject.lua" ) == false )
#endif
			return ;
	}

    LUA_BEGIN_TABLE( "SQUARE3DOBJECT_DATA", return )
    {
        KGCSquare3DObj *kSquare3dObj;// = new KGCSquare3DObj();
        int iType, iCharType;
        bool bIsRight, bUseNickNameString;

        float fWidth, fHeight, fSpeechTimer, fSpeechDist;
		D3DXVECTOR3 vTemp;
        D3DXVECTOR2 vOffset, vNickNameOffset;
		DWORD dwNickNameStrignID;
        DWORD dwCartoonTexture;
		DWORD dwLParam;
		DWORD dwRParam;

        for ( int i = 1; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                LUA_GET_VALUE_DEF( "OBJECT_TYPE", iType, -1 );

				switch( iType )
				{
				case SOT_NPC:
					kSquare3dObj = new KGCSquareNPC();	break;
				default:
					kSquare3dObj = new KGCSquare3DObj();	break;
				}

                kSquare3dObj->SetType( static_cast<SQUARE_OBJ_TYPE>(iType) );
                LUA_GET_VALUE_DEF( "OBJECT_SUBTYPE", iType, -1 );
                kSquare3dObj->SetSubType( static_cast<SQUARE_OBJ_SUBTYPE>(iType) );
                LUA_GET_VALUE_DEF( "POS_X", vTemp.x, 0.0f );
                LUA_GET_VALUE_DEF( "POS_Y", vTemp.y, 0.0f );
				LUA_GET_VALUE_DEF( "POS_Z", vTemp.z, 0.0f );
                LUA_GET_VALUE_DEF( "ISRIGHT", bIsRight, true );
                LUA_GET_VALUE_DEF( "WIDTH", fWidth, 0.17f );
                LUA_GET_VALUE_DEF( "HEIGHT", fHeight, 0.3f );
				LUA_GET_VALUE_DEF( "SPEECHTIMER", fSpeechTimer, 10.0f );
				LUA_GET_VALUE_DEF( "CHAR_TYPE", iCharType, -1 );
				LUA_GET_VALUE_DEF( "USE_NICKNAME_STRING", bUseNickNameString, false );
				LUA_GET_VALUE_DEF( "NICKNAME_STRING_ID", dwNickNameStrignID, 0 );
                LUA_GET_VALUE_DEF( "CARTOON_TEXTURE", dwCartoonTexture, 0 );
				LUA_GET_VALUE_DEF( "NICKNAME_OFFSET_X", vNickNameOffset.x, 0.15f );
				LUA_GET_VALUE_DEF( "NICKNAME_OFFSET_Y", vNickNameOffset.y, 0.0f );
				LUA_GET_VALUE_DEF( "SPEECH_OFFSET_X", vOffset.x, 0.0f );
				LUA_GET_VALUE_DEF( "SPEECH_OFFSET_Y", vOffset.y, 0.0f );
				LUA_GET_VALUE_DEF( "SPEECH_DISTANCE", fSpeechDist, 50.0f );

				LUA_GET_VALUE_DEF( "LPARAM", dwLParam, 0 );
				LUA_GET_VALUE_DEF( "RPARAM", dwRParam, 0 );

				kSquare3dObj->m_fNicknameOffsetX = vNickNameOffset.x;
				kSquare3dObj->m_fNicknameOffsetY = vNickNameOffset.y;
				kSquare3dObj->m_fSpeechOffsetX = vOffset.x;
				kSquare3dObj->m_fSpeechOffsetY = vOffset.y;
				kSquare3dObj->m_iSpeechDistance = (int)fSpeechDist;                				
              
				kSquare3dObj->m_fNicknameOffsetX = vNickNameOffset.x;
				kSquare3dObj->m_fNicknameOffsetY = vNickNameOffset.y;
                kSquare3dObj->SetPosition( vTemp );
                kSquare3dObj->SetIsRight( bIsRight );
                kSquare3dObj->SetBodyRect( D3DXVECTOR2(fWidth, fHeight) );
				kSquare3dObj->SetSpeechTimer( fSpeechTimer );
				kSquare3dObj->SetCharType((EGCCharType)iCharType);
				kSquare3dObj->SetIsUseNickNameString( bUseNickNameString );
				kSquare3dObj->SetNickNameStringID( dwNickNameStrignID );
                kSquare3dObj->SetCartoonTexture((EGCCartoonTexture)dwCartoonTexture);

				kSquare3dObj->m_dwLParam = dwLParam;
				kSquare3dObj->m_dwRParam = dwRParam;

                LoadMeshInfo( luaMgr, kSquare3dObj->m_sMeshInfo );

                LUA_BEGIN_TABLE( "SPEECH", break )
                {
                    int iTemp;
                    for( int i = 1; ; ++i )
                    {
                        LUA_GET_VALUE_NOASSERT( i, iTemp, break );
                        kSquare3dObj->m_vecStringID.push_back( iTemp );
                    }
					LUA_END_TABLE( break )
                }
			
				if( SUCCEEDED(luaMgr.BeginTable( "PARTICLE") ) )
				{
					for( int i = 1; ; ++i )
					{
						TRACE_PARTICLE_INFO* pParticleInfo = NULL;
						pParticleInfo = new TRACE_PARTICLE_INFO();

						if( NULL == pParticleInfo)
							break;

						LUA_BEGIN_TABLE( i, break )
						{
							LUA_GET_VALUE_DEF( 1, pParticleInfo->m_strParticleName, "NULL" );
							LUA_GET_VALUE_DEF( 2, pParticleInfo->m_iBoneIndex, 0 );
							LUA_GET_VALUE_DEF( 3, pParticleInfo->m_fXOffset, 0.0f );
							LUA_GET_VALUE_DEF( 4, pParticleInfo->m_fYOffset, 0.0f );

							// 							m_pObject->GetGCObject()->GetAngleBone( (*itor)->m_iBoneIndex, &matBone );
							// 							D3DXMatrixMultiply(&matBone, &matBone, m_pObject->GetGCObject()->GetWorldMat());
							// 							vecTracePos.x = matBone._41 + (*itor)->m_fXOffset;
							// 							vecTracePos.y = matBone._42 + (*itor)->m_fYOffset;
							pParticleInfo->m_pParticleSequence = g_ParticleManager->CreateSequence(pParticleInfo->m_strParticleName, 0.0f, 0.0f, 0.5f);
							kSquare3dObj->m_vecTraceParticleInfo.push_back( pParticleInfo );
							LUA_END_TABLE(break);
						}
					}
					LUA_END_TABLE( break )
				}
				LUA_END_TABLE( break )
			}
            m_vec3DObjcet.push_back( kSquare3dObj );
        }
    }
    LUA_END_TABLE( return );

    //std::stringstream stm;
    //Dump( stm );
}

void GCSquare::LoadMeshInfo( KLuaManager& luaMgr, MESH_INFO& meshInfo )
{
	meshInfo.Init();
	LUA_BEGIN_TABLE( "MESHINFO", return )
	{
		LUA_BEGIN_TABLE( "MESHLIST", assert(!"No Meshlist Table") )
		{
			std::string strMesh;
			std::string strTex;
			for( int i = 1; ; ++i )
			{
				LUA_BEGIN_TABLE( i, break )
				{
					LUA_GET_VALUE_DEF( 1, strMesh, "NULL" );
					LUA_GET_VALUE_DEF( 2, strTex, "NULL" );

					meshInfo.m_vecStrMesh.push_back( strMesh );
					meshInfo.m_vecStrTex.push_back( strTex );
				}
				LUA_END_TABLE(break);
			}
		}
		LUA_END_TABLE( assert(!"end table Meshlist Table" ) );

		LUA_BEGIN_TABLE( "MOTIONLIST", assert(!"No Motionlist Table") )
		{
			std::string strMotion;
			for( int i = 1; ; ++i )
			{
				LUA_GET_VALUE_NOASSERT( i, strMotion, break );
				meshInfo.m_vecStrMotion.push_back( strMotion );
			}
		}
		LUA_END_TABLE( assert(!"end table Motionlist Table" ) );

		LUA_GET_VALUE_DEF( "SCALE", meshInfo.m_fScale, CHAR_SCALE_RATIO );
		LUA_GET_VALUE_DEF( "TECHNIQUE", meshInfo.m_iTechnique, -1 );
        LUA_GET_VALUE_DEF( "USE_AFTER_IMAGE", meshInfo.m_iAfterImage, -1 );

		LUA_BEGIN_TABLE( "LOCALPOS", goto NoLocalPos )
		{
			LUA_GET_VALUE_DEF( 1, meshInfo.m_vLocalPos.x, 0.0f );
			LUA_GET_VALUE_DEF( 2, meshInfo.m_vLocalPos.y, 0.0f );
			LUA_GET_VALUE_DEF( 3, meshInfo.m_vLocalPos.z, 0.5f );
		}
		LUA_END_TABLE( goto NoLocalPos );
NoLocalPos:

		LUA_BEGIN_TABLE( "MESH_CHILDPARTICLE", goto NoMeshChildParticle )
		{
			MESH_CHILD_PARTICLE kTemp;

			for( int i = 1; ; ++i )
			{
				LUA_BEGIN_TABLE( i, break )
				{
					kTemp.Init();
					LUA_GET_VALUE( 1, kTemp.m_strParticle, return );
					LUA_GET_VALUE( 2, kTemp.m_fOffSetX, return );
					LUA_GET_VALUE( 3, kTemp.m_fOffSetY, return );
					LUA_GET_VALUE( 4, kTemp.m_iCreateFrame, return );
					LUA_GET_VALUE( 5, kTemp.m_iMontionIndex, return );
					LUA_GET_VALUE_DEF( 6, kTemp.m_bDieTogether, false );
                    LUA_GET_VALUE_DEF( 7, kTemp.m_iBoneIndex, -1 );
                    LUA_GET_VALUE_DEF( 8, kTemp.m_bDotParticle, false );
                    LUA_GET_VALUE_DEF( 9, kTemp.m_fTraceTime, -1 );

					meshInfo.m_vecMeshChildParticle.push_back( kTemp );
				}
				LUA_END_TABLE(break);
			}
		}
		LUA_END_TABLE( goto NoMeshChildParticle );
NoMeshChildParticle:

		LUA_BEGIN_TABLE( "MESH_SOUNDLIST", goto NoMeshSoundList )
		{
			MESH_SOUNDLIST kTemp;

			for( int i = 1; ; ++i )
			{
				LUA_BEGIN_TABLE( i, break )
				{
					kTemp.Init();
					LUA_GET_VALUE( 1, kTemp.m_strSound, return );
					LUA_GET_VALUE( 2, kTemp.m_iStartFrame, return );
					LUA_GET_VALUE( 3, kTemp.m_iMontionIndex, return );

					meshInfo.m_vecMeshSoundList.push_back( kTemp );
				}
				LUA_END_TABLE(break);
			}
		}
		LUA_END_TABLE( goto NoMeshSoundList );
NoMeshSoundList:

		LUA_BEGIN_TABLE( "MESH_CREATEDAMAGE", goto NoMeshCreateDamage )
		{
			MESH_CREATEDAMAGE kTemp;

			for( int i = 1; ; ++i )
			{
				LUA_BEGIN_TABLE( i, break )
				{
					kTemp.Init();
					LUA_GET_VALUE( 1, kTemp.m_iDamageIndex, return );
					LUA_GET_VALUE( 2, kTemp.m_fOffSetX, return );
					LUA_GET_VALUE( 3, kTemp.m_fOffSetY, return );
					LUA_GET_VALUE( 4, kTemp.m_iCreateFrame, return );
					LUA_GET_VALUE( 5, kTemp.m_iMontionIndex, return );
                    LUA_GET_VALUE_DEF( 6, kTemp.m_bRightLeftOffset, false );

					meshInfo.m_vecMeshCreateDamage.push_back( kTemp );
				}
				LUA_END_TABLE(break);
			}
		}
		LUA_END_TABLE( goto NoMeshCreateDamage );
NoMeshCreateDamage:

		LUA_BEGIN_TABLE( "ANIMATION", assert(!"No animation Table") )
		{
			int iLife;
			int iMotionIdx;
			for( int i = 1; ; ++i )
			{
				LUA_BEGIN_TABLE( i, break )
				{
					LUA_GET_VALUE_DEF( 1, iLife, 0 );
					LUA_GET_VALUE_DEF( 2, iMotionIdx, 0 );

					meshInfo.m_vecAnimInfo.push_back( std::make_pair( iLife, iMotionIdx ) );
				}
				LUA_END_TABLE( break );
			}
		}
		LUA_END_TABLE( assert(!"end table animation table" ) );		

	}
	LUA_END_TABLE( return );
}

void GCSquare::SetNPCSpeechList( SQUARE_OBJ_SUBTYPE eNPC_Subtype, std::wstring strSpeech )
{
    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
		if ( (*vit)->GetType() != SOT_NPC )
			continue;

        if ( (*vit)->GetSubType() == eNPC_Subtype )
        {
            ((KGCSquareNPC*)(*vit))->AddNPCSpeechList( strSpeech );
        }
    }
}

D3DXVECTOR3 GCSquare::Get3DObjectPos( SQUARE_OBJ_SUBTYPE eNPC_Subtype )
{
//#if !defined( ENABLE_MINIGAME ) //이게 왜 들어가서 MINIGAME안키면 3D 오브젝트 다 안나옴-_-;
//    //if ( eNPC_Subtype == SOST_MINIGAME_NPC )
//        return D3DXVECTOR3( -10.0f, -10.0f, 0.0f );
//#endif

    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
        if ( (*vit)->GetSubType() == eNPC_Subtype )
        {
            return (*vit)->GetObjectPos();
        }
    }
	return D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
}

void GCSquare::ReloadNPCSpeechList( SQUARE_OBJ_SUBTYPE eNPC_Subtype )
{
    Iter3DObject vit;
    for ( vit = m_vec3DObjcet.begin(); vit != m_vec3DObjcet.end(); ++vit )
    {
        if ( (*vit)->GetType() != SOT_NPC )
            continue;

        if ( (*vit)->GetSubType() == eNPC_Subtype )
        {
            ((KGCSquareNPC*)(*vit))->ReloadNPCSpeech();
        }
    }
}

void GCSquare::SetGPCItemID( DWORD dwItemID_, DWORD dwUID_, int iTime_ )
{
	m_pTextureGPCItem = g_pItemMgr->CreateShopItemTexture( dwItemID_ / 10 );

	GCSquarePeople* pkGPCPlayer = GetSquarePeople(dwUID_);
	D3DXVECTOR3 vPos = pkGPCPlayer->GetPos();
	m_vGPCPlayerPos = vPos;
	m_iGPCRenderTime = iTime_;

	g_ParticleManager->CreateSequence( "Item_Get_02", vPos.x - 0.95f, vPos.y - 0.07f, 0.5f );	
	g_ParticleManager->CreateSequence( "Item_Get_01", vPos.x - 0.95f, vPos.y - 0.07f, 0.5f );
}

void GCSquare::RenderGPCItem()
{
	if( m_pTextureGPCItem == NULL)
		return;

	m_iGPCRenderTime--;

	if( (int)(GC_FPS_LIMIT * 3.0f - 10.0f) < m_iGPCRenderTime )
		return;

	if( m_iGPCRenderTime <= 0 )
	{
		SAFE_RELEASE(m_pTextureGPCItem);
		return;
	}

	D3DXMATRIX mat;
	D3DXVECTOR3 vPos;
	D3DXVECTOR2 vTemp;

	D3DXMatrixIdentity( &mat );
	D3DXMatrixMultiply( &mat, &mat, &g_kCamera.m_matCamera );
	::D3DXVec3TransformCoord( &vPos, &m_vGPCPlayerPos, &mat );

	vTemp.x = int( ( vPos.x - 0.01f ) * (400.0f  * GC_SCREEN_DIV_WIDTH)  ) + 0.5f;
	vTemp.y = int( ( -vPos.y + 0.74f ) * (400.0f * GC_SCREEN_DIV_WIDTH) ) + 0.5f;

	g_pItemMgr->Draw_ShopImage( vTemp, (50.0f * GC_SCREEN_DIV_WIDTH), (50.0f* GC_SCREEN_DIV_WIDTH), m_pTextureGPCItem, false);

	m_iGPCRenderTime--;
}

void GCSquare::SetSquareInfo( const std::vector< KSquareInfo >& vecInfo )
{
	m_vecSquareInfo.clear();
	for( int i = 0; i < (int)vecInfo.size(); i++ )
	{
		if( m_iSquareType == vecInfo[i].m_nType )
			m_vecSquareInfo.push_back(vecInfo[i]);
	}

	if( m_iSquareType == KSquareInfo::ST_GUILD )
	{
		for( int i = 0; i < (int)m_vecSquareInfo.size(); i++ )
		{
			m_vecSquareInfo[i].m_strName = g_pkStrLoader->GetReplacedString( STR_ID_NEW_GUILD_SQAURE_CHNNEL, "i", i+1 );
		}
	}
}

void GCSquare::LoadSquareBaseEquipScript()
{
    m_vecSquareBaseEquip.clear();

    KLuaManager luaMgr; 

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "SquarePeopleBaseEquip.stg") == false)
        return;

    LUA_BEGIN_TABLE("BASEEQUIP",return);
    {
        for(int i = 1; ; i++)
        {
            KSquareBaseEquip kSquareBaseEquipTemp;
            LUA_BEGIN_TABLE(i,break)
            {
                LUA_GET_VALUE("CHAR",kSquareBaseEquipTemp.iCharNum,return);
                LUA_BEGIN_TABLE("PARTS",return)
                {
                    int iTemp = 0;
                    for(int j = 1; ; j++)
                    {
                        LUA_GET_VALUE_NOASSERT(j,iTemp,break)
                            kSquareBaseEquipTemp.vecParts.push_back(iTemp);
                    }
                }
                LUA_END_TABLE(return);
            }
            m_vecSquareBaseEquip.push_back(kSquareBaseEquipTemp);
            LUA_END_TABLE(return)
        }
    }
    LUA_END_TABLE(return)
}

std::vector<KSquareBaseEquip> GCSquare::GetSquareBaseEquipContainer()
{
    return m_vecSquareBaseEquip;
}

void GCSquare::SendGwcBoardReq()
{
    Result_GwcRakinglistReq = INT_MAX;
    KP2P::GetInstance()->Send_GWCRakingListReq();
    g_MyD3D->WaitForServerAck( Result_GwcRakinglistReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void GCSquare::Send_NewYearDonationInfoReq( IN const DWORD dwType_ )
{
    KP2P::GetInstance()->Send_NewYearDonationInfoReq( dwType_ );
}
