#include "stdafx.h"
//

#include "KMci.h"
#include "DamageManager.h"
#include "Spark.h"
#include "GCCameraOrtho.h"
#include "Controls.h"
#include "KDInput.h"

#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"

#include "KGCAgit.h"
#include "KGCAgitGameManager.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"
#include "Message.h"


KGCAgit::KGCAgit( void )
: m_fEventTime( 0.0f )
, m_dwEndTimer( 0 )
{
}

KGCAgit::~KGCAgit( void )
{

}

HRESULT KGCAgit::InitAtGame()
{
    m_fEventTime = 0.0f;
    m_dwEndTimer = 0;

    g_pkUIScene->m_pkGameOverlayUI->ToggleRender( false );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );

    SiKGCAgitGameManager()->SetExitGame( false );
    g_pkAgitOverlayUI->Init( SiKGCAgitGameManager()->GetAgitUID() );
    g_pkAgitOverlayUI->ToggleRender( true );

//  if( SiKGCAgitGameManager()->GetAgitUID() == g_kGlobalValue.m_kUserInfo.dwUID ) { 
//      g_kGlobalValue.m_kUserInfo.bHost = true;
//  }

    return S_OK;
}

HRESULT KGCAgit::InitAtLoading()
{
    HRESULT iResult = S_FALSE;
    if( LoadAzitMotion() )  iResult = S_OK;
    return iResult;
}

HRESULT KGCAgit::CustomFrameMove( float fElapsedTime )
{
    if( SiKGCAgitGameManager()->IsObjectPick() == false ) {
        if( SiKGCAgitGameManager()->IsNeedUpdate() ) {
            g_pkAgitOverlayUI->UpdatePlaceCollectUI();
            g_pkAgitOverlayUI->UpdatePrePlaceUI();
        }
    }

    SendP2PPacket( GC_PID_RELAY_PING );
    SiKGCAgitGameManager()->FrameMove();

    SiKGCEmbarkManager()->FrameMove();

    // �÷��̾ ���� ������ ������Ʈ �� �浹ó���� ������ ����Ѵ�.
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Calculate_Body_Rect();
        g_MyD3D->MyPlayer[i]->Frame_Move();
    }

    // ���Ϳ� ���� ������ ������Ʈ
    g_kMonsterManager.FrameMove();


    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Crash_Check();
        g_MyD3D->m_akPet[i]->FrameMove();
    }

    //// �մ��� ������Ʈ ���� ó�� �ʿ� ���� 
    //if( g_kGlobalValue.m_kUserInfo.dwUID != SiKGCAgitGameManager()->GetAgitInfo().m_dwUID )
    //    return S_OK;

    // ��ŷ ó�� 
    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
    D3DXVECTOR2 dxvPickPos(dxvMousePos);
    g_MyD3D->MyCtrl->Picking( dxvPickPos );

    bool bMouseInUI = g_pkAgitOverlayUI->MouseInUI( dxvMousePos );

    if( !g_pkUIScene->IsStop() &&
        SiKGCAgitGameManager()->IsObjectPick() == false )
    {
        if( SiKGCAgitGameManager()->GetAgitEditState() && bMouseInUI == false ) {
            if( g_pkInput->BtnDown( KInput::MBRIGHT ) )
                bMouseInUI = Picking( KInput::MBRIGHT );
            else if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
                bMouseInUI = Picking( KInput::MBLEFT );
        }
        else if( SiKGCAgitGameManager()->IsPrePlace() ) {
            if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
                bMouseInUI = Picking( KInput::MBLEFT );
        }
    }

    // ���콺 Ŭ���� ���� ������Ʈ ���� �ǵ��� 
    if( SiKGCAgitGameManager()->GetAgitEditState() && ( SiKGCAgitGameManager()->IsObjectPick() || SiKGCAgitGameManager()->IsCharacterPick() ) )
    {
        // ������ Ŭ�� �� ȸ�� 
        if( g_pkInput->BtnDown( KInput::MBRIGHT ) ) {
            SiKGCAgitGameManager()->RemovePickedObject();
            SiKGCAgitGameManager()->RemovePickCharacter();
        }

        if( g_pkInput->BtnUp( KInput::MBLEFT ) ) {
            if( bMouseInUI ) {
                SiKGCAgitGameManager()->RemovePickedObject();
                SiKGCAgitGameManager()->RemovePickCharacter();
            }
            else {
                SiKGCAgitGameManager()->SetObjectPickOff();
                SiKGCAgitGameManager()->SetCharacterPickOff();
            }
        }

        // ��ŷ�� ������Ʈ l���콺 ���� �����̰� 
        if( !g_pkUIScene->IsStop() )
        {
            const POINT& ptMouse = g_pkInput->GetMousePos();
            D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
            g_MyD3D->MyCtrl->Picking( dxvMousePos );

            SiKGCAgitGameManager()->UpdatePickedObjectPos( dxvMousePos );
            SiKGCAgitGameManager()->UpdatePickedCharacterPos( dxvMousePos );
        }
    }

    // �̸���ġ 
    if( SiKGCAgitGameManager()->IsPrePlace() && false == bMouseInUI )
    {
        // ��ŷ�� ������Ʈ l���콺 ���� �����̰� 
        if( !g_pkUIScene->IsStop() )
        {
            const POINT& ptMouse = g_pkInput->GetMousePos();
            D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
            g_MyD3D->MyCtrl->Picking( dxvMousePos );

            SiKGCAgitGameManager()->UpdatePickedObjectPos( dxvMousePos );
        }
        
        if( SiKGCAgitGameManager()->IsObjectPick() && g_pkInput->BtnUp( KInput::MBLEFT ) ) {
            SiKGCAgitGameManager()->SetObjectPickOff();
        }

    }

    // Space ó�� -> ���ѽɱ� / ��Ȯ / ���ֱ� ( ä�� ��Ŀ�� Ȯ�� ) 
    if( g_pkInput->IsDown( DIK_SPACE ) && g_pkAgitOverlayUI->GetCursorFocus() != KGCAgitOverlayUI::ECF_CHATBOX )
    {
        if( SiKGCAgitGameManager()->GetAgitUID() == g_kGlobalValue.m_kUserInfo.dwUID )  // �츮���϶��� �˻� 
        {
            if( g_pkUIScene->IsStop() == false )    // �޽��� �ڽ� �ȶ��� �� 
            {
                if( g_pkAgitOverlayUI->IsRenderOnAgitUI() == false )
                {
                    D3DXVECTOR2 dxvPos( g_MyD3D->GetMyPlayer()->GetPosition().x, g_MyD3D->GetMyPlayer()->GetPosition().y );
                    SiKGCAgitGameManager()->ChecKBoundToflowerPot( dxvPos );    // ȭ�а���
                    SiKGCAgitGameManager()->CheckBoundToFairyTree( dxvPos );    // ������ ���� ���� 
                    SiKGCAgitGameManager()->CheckBoundToGuestBook( dxvPos );    // ������ ���� 
                    SiKGCAgitGameManager()->CheckBoundToTraining( dxvPos );     // �Ʒü� ���� 

                    // �ɱ� ��� 
                    if( true == SiKGCAgitGameManager()->CheckEnableSitDownObject( dxvPos ) ) {
                        SiKGCSocialMotionManager()->SetMotion( MID_SOCIAL_SIT_DOWN_MAN );
                    }

                    // ���� ���
                    if( true == SiKGCAgitGameManager()->CheckEnableLieObject( dxvPos ) ) { 
                        SiKGCSocialMotionManager()->SetMotion( MID_SOCIAL_LIE_MAN );
                }
                }
                else if ( g_pkAgitOverlayUI->IsRenderOnHarvestUI() == true )
                {
                    bool bBonus = g_pkAgitOverlayUI->HarvestStop();
                    if( bBonus )
                        g_KDSound.Play( "984034" );
                    else
                        g_KDSound.Play( "984035" );

                    KP2P::GetInstance()->Send_HarvestFlowerPotReq( SiKGCAgitGameManager()->GetNowPlantPotUID().first, bBonus );
                }
            }
        }
        else    // �츮�� �ƴϸ� ������� �ױ��Ͻÿ�
        {
            if( g_pkAgitOverlayUI->IsRenderOnAgitUI() == false )
            {
                D3DXVECTOR2 dxvPos( g_MyD3D->GetMyPlayer()->GetPosition().x, g_MyD3D->GetMyPlayer()->GetPosition().y );
                SiKGCAgitGameManager()->CheckBoundToGuestBook( dxvPos );    // ������ ���� 

                // �ɱ� ��� 
                if( true == SiKGCAgitGameManager()->CheckEnableSitDownObject( dxvPos ) ) {
                    SiKGCSocialMotionManager()->SetMotion( MID_SOCIAL_SIT_DOWN_MAN );
                }
            }
        }
    }

    // Ŭ���� ĳ���� ��� 
    if( false == bMouseInUI && g_pkInput->BtnDown( KInput::MBLEFT ) && false == SiKGCAgitGameManager()->GetAgitEditState() ) { 
        std::pair<int, int> ptCheck  = SiKGCAgitGameManager()->CheckPickToAgitChar( dxvMousePos );
        if( -1 != ptCheck.first ) { 
            SiKGCAgitGameManager()->SetAgitCharacterSayBaloon( ptCheck.second, ptCheck.first, KAgitCharacterPos::SPT_CLICK );
        }
    }

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}

HRESULT KGCAgit::CalcExpAndGP()
{
    return S_OK;
}

bool KGCAgit::CheckGameEnd()
{
    return SiKGCAgitGameManager()->GetExitGame();
}

void KGCAgit::GameEndProcess()
{
    g_kCamera.m_bZoomViewMode = false;
}


void KGCAgit::GameEnd()
{
    //	��ǳ��
    g_pkAgitOverlayUI->DeleteAllSayBalloon();

    // �������
    g_pGraphicsHelper->UnLoadPlayerActionMotion();
    g_pGraphicsHelper->UnLoadPlayerModel();

    // ����Ʈ�� �޽����� ����
    g_kGlobalValue.m_ExtraMesh.ClearMeshInfo();

    SiKGCAgitGameManager()->SetExitGame( true );

    SiKGCAgitGameManager()->SaveAgitObject();
    SiKGCAgitGameManager()->ClearObject();
    SiKGCAgitGameManager()->ClearPlaceCharUID();
    g_pkAgitOverlayUI->OnExitAgit();

    if( SiKGCAgitGameManager()->GetRegisterState() )
        SiKGCAgitGameManager()->UnRegisterAgitList();

    SiKGCAgitGameManager()->DestroyAgitCharacter();
}

bool KGCAgit::Picking( int iMouseBtn )
{
    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );

    if( false == SiKGCAgitGameManager()->PickUpCharacter( dxvMousePos ) ) {
        SiKGCAgitGameManager()->PickUpObject( dxvMousePos );
    }

    return g_pkAgitOverlayUI->MouseInUI( dxvMousePos );
}

bool KGCAgit::GetAgitMousePos( D3DXVECTOR2& dxvPos )
{
    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
    g_MyD3D->MyCtrl->Picking( dxvMousePos );

    dxvPos.x = dxvMousePos.x * TOINDEXFACTOR;
    dxvPos.y = dxvMousePos.y * TOINDEXFACTOR;

    return SiKGCFantasticMap()->IsValidBlock( dxvMousePos.x, dxvMousePos.y );
}

bool KGCAgit::LoadAzitMotion()
{
    return LoadScript();
}

bool KGCAgit::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "MotionEnum.stg" ) == false )
    {
        MessageBoxA( NULL, "Enum.stg Load Failed!", "", MB_OK );
        return false;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "AzitTemplate.stg" ) == false )
    {
        MessageBoxA( NULL, "DotaTemplate.stg Load Failed!", "", MB_OK );
        return false;
    }

    // ����Ʈ ��� ����
    if( false == LoadMotionScript( luaMgr ) )
        return false;

    // ����Ʈ�� �޽� ����
    if( false == LoadExtraMeshScript( luaMgr ) )
        return false;

    return true;
}

bool KGCAgit::LoadMotionScript(KLuaManager &luaMgr)
{
    g_kGlobalValue.m_vecAzitMotion.clear();

    int iTemp;
    LUA_BEGIN_TABLE_RAII( "AZIT_MOTION" )
    {
        LUA_BEGIN_TABLE_RAII( "CommonMotion" )
        {
            for(int iLoop=1; ; ++iLoop)
            {
                LUA_GET_VALUE_DEF(iLoop, iTemp, -1);

                if( -1 == iTemp )
                    break;

                g_kGlobalValue.m_vecAzitMotion.push_back( iTemp );
            }
        }
        else
            return false;
    }
    else
        return false;

    return true;
}

bool KGCAgit::LoadExtraMeshScript(KLuaManager &luaMgr)
{
    SExtraMeshInfo sAzitExtraMesh;
    LUA_BEGIN_TABLE_RAII( "AZIT_EXTRA_MESH" )
    {
        for(int iLoopMesh=1; ; ++iLoopMesh)
        {
            LUA_BEGIN_TABLE_RAII( iLoopMesh )
            {
                LUA_GET_VALUE_DEF( "MeshName", sAzitExtraMesh.strModelFile, "");
                LUA_GET_VALUE_DEF( "TextureName", sAzitExtraMesh.strTextureFile, "");
                LUA_GET_VALUE_DEF( "BaseMotion", sAzitExtraMesh.strBaseMotionFile, "");
                LUA_GET_VALUE_DEF( "BaseBoneIdx", sAzitExtraMesh.iBoneIdx, 0 );

                LUA_BEGIN_TABLE_RAII( "Condition" )
                {
                    SEM_PlayerMotionCondition sEM_PlayerMotionCondition;
                    for(int iLoopCondition=1; ; ++iLoopCondition)
                    {
                        LUA_BEGIN_TABLE_RAII( iLoopCondition )
                        {
                            LUA_GET_VALUE_DEF( 1, sEM_PlayerMotionCondition.iConditionID, -1 );

                            switch( sEM_PlayerMotionCondition.iConditionID )
                            {
                            case CONDITION_PLAYERMOTION:
                                {
                                    LUA_GET_VALUE_DEF( 2, sEM_PlayerMotionCondition.iPlayerMotionID, -1 );
                                    LUA_GET_VALUE_DEF( 3, sEM_PlayerMotionCondition.strPC_MotionName, "" );
                                    LUA_GET_VALUE_DEF( 4, sEM_PlayerMotionCondition.bWeaponToggle, false );
                                }
                                sEM_PlayerMotionCondition.strObjName = sAzitExtraMesh.strModelFile;
                                sAzitExtraMesh.vecCondition_PM.push_back( sEM_PlayerMotionCondition );
                                break;
                            }
                        }
                        else
                            break;
                    }
                }

                g_kGlobalValue.m_ExtraMesh.SetExtraMeshInfo( sAzitExtraMesh );
            }
            else
                break;
        }
    }
 
    return true;
}