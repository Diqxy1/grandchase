#include "stdafx.h"
#include "KGCMissionSub_S4.h"


IMPLEMENT_CLASSNAME( KGCMissionSub_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionSub_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionSub_S4, "gc_mission_sub" );

KGCMissionSub_S4::KGCMissionSub_S4( void )
: m_pkSubInfoBox( NULL )
, m_pkSubRewardInfo( NULL )
, m_pkBtn_Preview( NULL )
, m_pkBtn_Complate( NULL )
, m_pkBtn_Delete( NULL )
, m_dwMainMissionID( 0 )
, m_dwBtnRenderState( 0 )
, m_iCompleteEffect( 0 )
, m_iEffectCnt( 0 )
, m_bCompleteSubEff( false )
{
    LINK_CONTROL( "subinfobox", m_pkSubInfoBox );
    LINK_CONTROL( "rewardinfo", m_pkSubRewardInfo );

    LINK_CONTROL( "btn_preview", m_pkBtn_Preview );
    LINK_CONTROL( "btn_complate", m_pkBtn_Complate );
    LINK_CONTROL( "btn_delete", m_pkBtn_Delete );

    m_vecOriPos = D3DXVECTOR2(455.0f * GC_SCREEN_DIV_WIDTH, 52.0f * GC_SCREEN_DIV_WIDTH);
}

KGCMissionSub_S4::~KGCMissionSub_S4( void ){}

void KGCMissionSub_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionSub_S4::OnCreate( void )
{
    if( m_pkBtn_Preview )
    {
        m_pkBtn_Preview->InitState( (m_dwBtnRenderState & BTN_PRIVIEW) == BTN_PRIVIEW, true, this );
        m_pkBtn_Preview->DisconnectAllEventProcedure();
        m_pkBtn_Preview->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionSub_S4::OnClickPreview );
        m_pkBtn_Preview->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionSub_S4::OnEnterCursor );
    }

    if( m_pkBtn_Complate )
    {
        m_pkBtn_Complate->InitState( (m_dwBtnRenderState & BTN_COMPLATE) == BTN_COMPLATE, true, this );
        m_pkBtn_Complate->DisconnectAllEventProcedure();
        m_pkBtn_Complate->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionSub_S4::OnClickComplate );
        m_pkBtn_Complate->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionSub_S4::OnEnterCursor );
    }

    if( m_pkBtn_Delete )
    {
        m_pkBtn_Delete->InitState( (m_dwBtnRenderState & BTN_DELETE) == BTN_DELETE, true, this );
        m_pkBtn_Delete->DisconnectAllEventProcedure();
        m_pkBtn_Delete->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionSub_S4::OnClickDelete );
        m_pkBtn_Delete->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionSub_S4::OnEnterCursor );
    }

    if( m_pkSubInfoBox )
    {
        m_pkSubInfoBox->InitState( true, true, this );
    }

    if( m_pkSubRewardInfo )
    {
        m_pkSubRewardInfo->InitState( true, true, this );
        m_pkSubRewardInfo->SetComplateInstance( m_pkBtn_Complate );
    }
}

void KGCMissionSub_S4::FrameMoveInEnabledState( void )
{
    if( m_pkSubInfoBox && m_pkSubRewardInfo )
    {
        if( 193.0f * GC_SCREEN_DIV_WIDTH != (m_pkSubRewardInfo->GetBackGroundPosY() - m_pkSubInfoBox->GetFixedWindowLocalPos().y) )
            m_pkSubInfoBox->SetPosY( (m_pkSubRewardInfo->GetBackGroundPosY() - 193.0f * GC_SCREEN_DIV_WIDTH) );
        if( 180.0f * GC_SCREEN_DIV_WIDTH != (m_pkSubRewardInfo->GetBackGroundPosY() - m_pkBtn_Delete->GetFixedWindowLocalPos().y) )
            SetDeleteBtnPosY( (m_pkSubRewardInfo->GetBackGroundPosY() - 180.0f * GC_SCREEN_DIV_WIDTH) );
    }

    if( !m_bCompleteSubEff && m_iCompleteEffect != EFFECT_NONE ) { 
        switch( m_iCompleteEffect )
        {
        case EFFECT_COMPLETE:// 미션완료버튼 이펙트
            if( m_iEffectCnt > 35 )
            {
                m_iEffectCnt = 0;
                g_ParticleManager->MissionCompleteEffect( -0.005f, 0.005f );
            }
            ++m_iEffectCnt;
            break;
        case EFFECT_REWARD:// 보상리스트박스 이펙트
            if( m_iEffectCnt > 35 )
            {
                m_iEffectCnt = 0;
                g_ParticleManager->MissionRewardEffect( 0.0f, 0.0f );
            }
            ++m_iEffectCnt;
            break;
        }
    }
    else if( m_bCompleteSubEff ) { 
        std::vector< MissionCompleteEff > vecEff;
        if( SiKGCMissionManager()->GetMissionCompleteEff( m_dwMainMissionID, vecEff ) ) { 
            for( std::vector< MissionCompleteEff >::const_iterator vit = vecEff.begin() ; vit != vecEff.end() ; ++vit ) { 
                g_ParticleManager->MissionCompleteSubEffet( vit->strParticle, vit->dxvPos.x, vit->dxvPos.y );
            }
            m_bCompleteSubEff = false;
        }
    }
}

void KGCMissionSub_S4::OnClickPreview()
{
    if( g_pkUIScene->m_pkMissionEquipPreview_S4 )
    {
        g_pkUIScene->m_pkMissionEquipPreview_S4->SetPreviewWindow( m_vecPreviewEquip );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_PREVIEW_S4_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}

void KGCMissionSub_S4::OnClickComplate()
{
    if (g_pItemMgr->CheckItemCountLimit())
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
    else
    {
        const SMissionInfo* sMainInfo = SiKGCMissionManager()->GetMissionInfo(m_dwMainMissionID);
        if (NULL == sMainInfo || NULL == m_pkSubRewardInfo)
            return;

        DWORD dwItem = m_pkSubRewardInfo->GetSelRewardItem();
        if ((0 < sMainInfo->m_vecSelectItem.size()) && (0 >= dwItem))
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_MISSION_COMPLETE_TOOLTIP), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        else {
            //임무 완료를 통해 새로운 캐릭터를 얻을 수 있도록 변경되었습니다. 루마의 손으로 만들었습니다.
            if (sMainInfo->m_iNewCharacter > -1 && false == GCFUNC::HadChar(sMainInfo->m_iNewCharacter))
                GCFUNC::CharGainParticle(sMainInfo->m_iNewCharacter);
            else
                m_bCompleteSubEff = SiKGCMissionManager()->IsCompleteEff(m_dwMainMissionID);

            KP2P::GetInstance()->Send_MissionComplete(m_dwMainMissionID, dwItem);
        }

        //임무 완료를 통해 새로운 캐릭터를 얻을 수 있도록 변경되었습니다. 루마의 손으로 만들었습니다.
        if (sMainInfo->m_iNewCharacter > -1 && false == GCFUNC::HadChar(sMainInfo->m_iNewCharacter)) {
            Result_New_Character = INT_MAX;
            KP2P::GetInstance()->Send_NewCharChoiceReq(sMainInfo->m_iNewCharacter, L"");
            if (false == g_MyD3D->WaitForServerAck(Result_New_Character, INT_MAX, 10000, INT_MAX))
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"");
            }
            GCFUNC::CharGainParticle(sMainInfo->m_iNewCharacter);
        }
    }
}

void KGCMissionSub_S4::OnClickDelete()
{
    if ( !SiKGCMissionManager()->IsEnableDelete( m_dwMainMissionID ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_DELETE_MISSION), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }


    g_kGlobalValue.m_kUserMission.ReadyMissionDelete( m_dwMainMissionID );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_MISSION_DELETE ) , L"", KGCUIScene::GC_MBOX_USE_DELETE_MISSION , 0, 0, false, true );
    g_KDSound.Play( "31" );
}

void KGCMissionSub_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCMissionSub_S4::RefreshSubMission( DWORD dwMissionUID )
{
    // 서브미션 UI초기화
    CleanUpSubMission();

    const SMissionInfo* sMainInfo = SiKGCMissionManager()->GetMissionInfo( dwMissionUID );
    if( NULL == sMainInfo )
        return;

    m_dwMainMissionID = dwMissionUID;

    // 서브미션이 없을때 함수종료하지 않는 이유 : 서브미션이 없는 메인미션도 있다.
    const SSubMissionInfo* sSubInfo = NULL;
    if( 0 < sMainInfo->m_vecSubMissions.size() )
        sSubInfo = SiKGCMissionManager()->GetSubMission( sMainInfo->m_vecSubMissions[ 0 ] );

    // 기사단장멘트
    if( g_pkUIScene->m_pkMissionUI_S4->GetMainMission() )
        g_pkUIScene->m_pkMissionUI_S4->GetMainMission()->SetStringMent( SiKGCMissionManager()->GetString( sMainInfo->m_iDescID ) );

    // 서브미션 정보셋팅
    if( sSubInfo && m_pkSubInfoBox )
    {
        int iComplateCnt = g_kGlobalValue.m_kUserMission.GetCompletionCount( dwMissionUID, sMainInfo->m_vecSubMissions[ 0 ] );
        int iMaxCnt = sSubInfo->m_sCompletion.m_iCount;
        if( iComplateCnt > iMaxCnt ) iComplateCnt = iMaxCnt;
        bool bComplate = g_kGlobalValue.m_kUserMission.IsCompleteMission( dwMissionUID );
        bool bRenderCompleteCount = true;

        for (int j = 0 ; j < (int)sSubInfo->m_vecConditionID.size(); j++ )
        {
            const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo(sSubInfo->m_vecConditionID[j]);

            if ( !sCondition )
                continue;

            if ( sCondition->m_iType != KGCMissionManager::TYPE_NOUSE )
            {
                if ( sCondition->m_iType == KGCMissionManager::TYPE_USE_POTION || 
                    sCondition->m_iType == KGCMissionManager::TYPE_SKILL_COUNT )
                    iMaxCnt = sCondition->m_pairUseCount.second;
                else
                    iMaxCnt = sCondition->m_iValue;

                if ( bComplate )
                {
                    iComplateCnt = iMaxCnt;
                    bRenderCompleteCount = false;                    
                }
                else
                    iComplateCnt = 0;

                if ( sCondition->m_iType == KGCMissionManager::TYPE_RANK )
                    bRenderCompleteCount = false;
            }
        }
        if ( bRenderCompleteCount )
            m_pkSubInfoBox->SetComplateStateText( iComplateCnt, iMaxCnt );
        m_pkSubInfoBox->SetTitleText( sSubInfo->m_iTitleID );
        m_pkSubInfoBox->SetDescriptionText( sSubInfo->m_iDescID );
        m_pkSubInfoBox->SetExpText( sMainInfo->m_iExp );
        m_pkSubInfoBox->SetGpText( sMainInfo->m_iGP );
    }

    // 보상 정보셋팅
    if( m_pkSubRewardInfo )
    {
        m_pkSubRewardInfo->SetMissionID( m_dwMainMissionID );
        m_pkSubRewardInfo->SetRewardItem( sMainInfo->m_vecRewardItem, sMainInfo->m_vecSelectItem );
    }

    // 미션스크롤 선택시 출력되는 프리뷰일때 출력위치처리
    if( g_pkUIScene->m_pkMissionUI_S4->GetPriview() )
    {
        D3DXVECTOR2 vecPos;
        vecPos.x = (GC_SCREEN_DIV_WIDTH_HALF - float(this->GetWidth()/2.0f));
        vecPos.y = (GC_SCREEN_DIV_HEIGHT_HALF - float(this->GetHeight()/2.0f));
        this->SetFixedWindowLocalPos( vecPos );

        if( m_pkBtn_Preview )
            m_pkBtn_Preview->ToggleRender( false );
        if( m_pkBtn_Complate )
            m_pkBtn_Complate->ToggleRender( false );
        if( m_pkBtn_Delete )
            m_pkBtn_Delete->ToggleRender( false );

        // 미리보기 버튼 처리
        if( TAB_CHARACTER == sMainInfo->m_iMissionType &&
            CheckEquipPriview(sMainInfo->m_vecRewardItem, sMainInfo->m_vecSelectItem) )
        {
            m_dwBtnRenderState |= BTN_PRIVIEW;
            m_pkBtn_Preview->ToggleRender( true );
        }
    }
    else
    {
        // 미리보기 버튼 처리
        if( CheckEquipPriview(sMainInfo->m_vecRewardItem, sMainInfo->m_vecSelectItem) &&
            TAB_CHARACTER == sMainInfo->m_iMissionType )
        {
            m_dwBtnRenderState |= BTN_PRIVIEW;
            m_pkBtn_Preview->ToggleRender( true );
        }

        // 완료버튼 처리
        bool bComplete = false;
        if( m_pkBtn_Complate )
        {
            m_dwBtnRenderState |= BTN_COMPLATE;
            m_pkBtn_Complate->ToggleRender( true );
            m_pkBtn_Complate->ClearToolTip();
            if( (bComplete = g_kGlobalValue.m_kUserMission.IsCompleteMission( m_dwMainMissionID )) )
            {
                if( 1 >= sMainInfo->m_vecSelectItem.size() )
                {
                    if( m_pkSubRewardInfo )
                        m_pkSubRewardInfo->SetFirstRewardBox();
                    
                    m_pkBtn_Complate->SetWndMode( D3DWM_DEFAULT );
                    SetCompleteEffect( EFFECT_COMPLETE );
                }
                else
                {
                    m_pkBtn_Complate->SetToolTip( g_pkStrLoader->GetString(STR_ID_MISSION_COMPLETE_TOOLTIP) );
                    m_pkBtn_Complate->SetWndMode( D3DWM_LOCK );
                    SetCompleteEffect( EFFECT_REWARD );
                }
            }
            else
                m_pkBtn_Complate->SetWndMode( D3DWM_LOCK );
        }

        // 삭제버튼 처리
        if( m_pkBtn_Delete )
        {
            m_dwBtnRenderState |= BTN_DELETE;
            m_pkBtn_Delete->ToggleRender( true );
        }

        // 새로운 미션일때 파티클 처리( 상하이드랍으로 들어온 미션은 확인도 안했는데 체킹될 수도 있다... IsRenderOn검사를.. )
        if( g_pkUIScene->m_pkMissionUI_S4->IsRenderOn() && !bComplete && g_kGlobalValue.m_kUserMission.IsNewMission( dwMissionUID ) )
        {
            g_kGlobalValue.m_kUserMission.SetNewMission( dwMissionUID, false );
            //g_ParticleManager->MissionNewEffect( 0.68f, 0.55f );
        }
    }
}

void KGCMissionSub_S4::CleanUpSubMission()
{
    // 서브미션 정보 초기화
    if( m_pkSubInfoBox )
        m_pkSubInfoBox->CleanUp();

    // 보상정보 초기화
    if( m_pkSubRewardInfo )
        m_pkSubRewardInfo->CleanUp();

    // 버튼 초기화
    if( m_pkBtn_Preview )
        m_pkBtn_Preview->ToggleRender( false );
    if( m_pkBtn_Complate )
        m_pkBtn_Complate->ToggleRender( false );
    if( m_pkBtn_Delete )
        m_pkBtn_Delete->ToggleRender( false );

    // 완료상태 이펙트 초기화
    SetCompleteEffect( EFFECT_NONE );
    m_iEffectCnt = 36;
    m_dwBtnRenderState = 0;
    this->SetFixedWindowLocalPos( m_vecOriPos );

    m_bCompleteSubEff = false;
}

bool KGCMissionSub_S4::CheckEquipPriview(const std::vector< SMissionInfo::SReward > &vecNormal, const std::vector< SMissionInfo::SReward > &vecSelect)
{
    GCItem *pItem;
    m_vecPreviewEquip.clear();
    DWORD aSlotPos[2] = {  ESP_SET_ITEM, ESP_WEAPON };

    // 일반보상템 확인
    for(int iLoop=0; iLoop<int(vecNormal.size()); ++iLoop)
    {
        if( NULL == (pItem = g_pItemMgr->GetItemData( vecNormal[iLoop].m_ItemID/10 )) )
            continue;

        if( (pItem->dwSlotPosition & ESP_SET_ITEM) ||
            (pItem->dwSlotPosition & ESP_WEAPON) )
        {
            m_vecPreviewEquip.push_back( (vecNormal[iLoop].m_ItemID/10) );
        }
    }

    // 선택보상템 확인
    for(int iLoop=0; iLoop<int(vecSelect.size()); ++iLoop)
    {
        if( NULL == (pItem = g_pItemMgr->GetItemData( vecSelect[iLoop].m_ItemID/10 )) )
            continue;

        if( (pItem->dwSlotPosition & ESP_SET_ITEM) ||
            (pItem->dwSlotPosition & ESP_WEAPON) )
        {
            m_vecPreviewEquip.push_back( vecSelect[iLoop].m_ItemID/10 );
        }
    }

    if( m_vecPreviewEquip.size() > 0 )
        return true;
    return false;
}

void KGCMissionSub_S4::SetDeleteBtnPosY( float fPos )
{
    D3DXVECTOR2 vecPos = m_pkBtn_Delete->GetFixedWindowLocalPos();
    vecPos.y = fPos;
    m_pkBtn_Delete->SetFixedWindowLocalPos( vecPos );
}