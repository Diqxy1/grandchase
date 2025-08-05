#include "stdafx.h"
#include "GCGuide.h"
//
//
//

//#include "MyD3D.h"
//#include "GCSayBalloon.h"
//#include "GCHighLight.h"

IMPLEMENT_CLASSNAME( KGCGuide );
IMPLEMENT_WND_FACTORY( KGCGuide );
IMPLEMENT_WND_FACTORY_NAME( KGCGuide, "gc_guide" );

KGCGuide::KGCGuide(void)
: m_dwTime( -1 )
, m_dwStartTime ( 0 )
, m_bUseHighlightClick( false )
, m_bCompleteAction( false )
, m_fx_hl( 0.0f )
, m_fy_hl( 0.0f )
, m_dwWidth_hl( 0 )
, m_dwHeight_hl( 0 )
, m_nCurrentStep( 0 )
, m_nCurrentGuideType( -1 )
, m_nNPCType( -1 )
, m_bItemRecieve( false )
{
    m_pkHighlight = NULL;
    LINK_CONTROL( "highlight", m_pkHighlight );

    m_pkNoHighlight = NULL;
    LINK_CONTROL( "nohighlight", m_pkNoHighlight );

    m_pkNoBlindHighlight = NULL;
    LINK_CONTROL( "noblindhighlight", m_pkNoBlindHighlight );

    char szWndName[100] = {0, };

    for( int i = 0; i < GNPCT_MAX; ++i ){
        m_pkGuideNPC[i] = NULL;
        sprintf_s( szWndName, 99, "guide_npc_%d", i );
        LINK_CONTROL( szWndName, m_pkGuideNPC[i] );
    }
    
    m_pkGiudeArrow = NULL;
    LINK_CONTROL( "guide_arrow", m_pkGiudeArrow );

    m_vecStep.clear();
}

KGCGuide::~KGCGuide(void)
{
}

void KGCGuide::OnCreate( void )
{
    m_pkHighlight->InitState( false );
    m_pkNoHighlight->InitState( false );
    m_pkNoBlindHighlight->InitState( false );

    for( int i = 0; i < GNPCT_MAX; ++i ) {
        m_pkGuideNPC[i]->InitState( false, true );
    }

    m_pkGiudeArrow->InitState( false, true );
}

void KGCGuide::ActionPerformed( const KActionEvent& event )
{
}

void KGCGuide::FrameMove()
{
    KD3DWnd::FrameMove();

    if( m_pkHighlight ) 
        m_pkHighlight->FrameMove();

    if( m_nNPCType != -1 && m_pkGuideNPC[m_nNPCType] )
        m_pkGuideNPC[m_nNPCType]->FrameMove();

    if( m_pkGiudeArrow )
        m_pkGiudeArrow->FrameMove();
}

void KGCGuide::FrameMoveInEnabledState( void )
{
    // 발동 중인 가이드가 없다면 리턴
    if( m_nCurrentGuideType == -1 )
        return;

    // 이번 스텝에서 해야 할 동작을 완료 했다면 종료
    if( m_bCompleteAction == true )
    {
        EndCurrentStep();
        return;
    }

    // 시간 확인
    if( m_dwTime == -1 ) { // 무제한 대기
        // 시간으로 넘어가는 스텝이 아님
    } else {
        DWORD dwCurrentTime = ::timeGetTime();

        if( dwCurrentTime >= m_dwStartTime + m_dwTime ){
            m_bCompleteAction = true;

            // 다음 스텝으로 넘어간다.
            ++m_nCurrentStep;
        }
    }

    // 클릭했나?!
    if( m_bUseHighlightClick && g_pkInput->UserBtnUp( KInput::MBLEFT ) ) {

        // 유저가 클릭한 위치
        POINT pt = g_pkInput->GetUserMousePos();
        D3DXVECTOR2 vMousePos( static_cast<float>(pt.x), static_cast<float>(pt.y) );
        vMousePos.x /= m_fWindowScaleX;
        vMousePos.y /= m_fWindowScaleY;

        if( ( m_fx_hl < vMousePos.x && vMousePos.x < m_fx_hl + m_dwWidth_hl ) &&
            ( m_fy_hl < vMousePos.y && vMousePos.y < m_fy_hl + m_dwHeight_hl ) ){
            // 필요한 동작을 했음
            m_bCompleteAction = true;

            // 실제로 눌렸다는 신호를 보냄
            g_pkInput->VirtualClick( pt );

            // 다음 스텝으로 넘어간다.
            ++m_nCurrentStep;
        }
    }
}


void KGCGuide::SetActive()
{
    // UI최상위로 활성화 
    SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    g_pkUIScene->SetActiveWndInStop( this );
    g_pkInput->SetVirtualMode( true );
}

void KGCGuide::SetCurrentGuide( IN int nGuideType, IN std::vector<SStep> vecStep )
{
    // 스텝 정보를 저장
    m_nCurrentGuideType = nGuideType;
    m_vecStep = vecStep;

    m_nCurrentStep = 0;

    GoNextStep();
}

void KGCGuide::EndCurrentStep()
{
    // 파티클 종료
    ClearPaticle();

    //
    g_pkUIScene->SetChildWndUpdateLock( false );
    g_pkInput->SetVirtualMode( false );

    // 다음 스텝으로
    GoNextStep();
}

// 이번 스텝에 대한 가이드 UI를 세팅하자
void KGCGuide::GoNextStep()
{
    // 현재 스텝에 대한 정보를 UI에 세팅
    if( m_nCurrentStep < static_cast<int>( m_vecStep.size() ) ) {
        SStep& kStep = m_vecStep[m_nCurrentStep];

        // 메뉴가 열려 있어서 패스해야 하는 경우라면 바로 다음 스텝으로
        if( kStep.kAction.bMenuOpenThenPass == true && g_pkUIScene->m_pkSceneMenuBtnS6->IsMenuOpened() ){
            m_bCompleteAction = true;
            return;
        }

        // 다음 스텝으로 넘어가기 위한 동작
        SetAction( kStep.kAction );

        // 하이라이트 영역
        SetHighlightRect( kStep.kHighlightInfo );

        // NPC
        SetNPCInfo( kStep.kNPCInfo );

        // Arrow
        SetArrowInfo( kStep.kArrowInfo );

        // Particle
        SetParticle( kStep.vecPaticle );

        // 하드코딩을 이용하는 스텝, 나중에(?) 개선해 볼게요
        UseExtraFunction();

        // etc
        m_bItemRecieve = false;
        m_bCompleteAction = false;
        g_pkUIScene->SetActiveWndInStop( this );
        g_pkInput->SetVirtualMode( true );

    } else {
        // 가이드 종료
        m_nCurrentGuideType = -1;
        g_MyD3D->m_kGuideMgr.EndGuide();

        g_pkUIScene->SetChildWndUpdateLock( false );
        g_pkInput->SetVirtualMode( false );
    }
}

void KGCGuide::SetAction( IN SAction& kAction )
{
    // 일정 시간이 경과한 뒤에 다음 스텝으로 넘어갈 것인가?
    if( kAction.nTime == -1 ) {
        m_dwTime = kAction.nTime;
    } else {
        m_dwTime = kAction.nTime * 1000;

        // 현재 시간을 저장
        m_dwStartTime = ::timeGetTime();
    }

    // 하이라이트 영역 클릭 동작으로 넘어갈 것인가?
    m_bUseHighlightClick = kAction.bHighlightClick;
}

void KGCGuide::SetHighlightRect( IN SHighlightInfo& kHighlightInfo )
{
    //GHT_NORMAL
    //GHT_NORMAL_WITHOUT_RECT
    //GHT_NO_BLIND_WITHOUT_RECT
    switch (kHighlightInfo.nHighLightType)
    {
    case GHT_NORMAL:
        {
            float fx = 0.0f;
            float fy = 0.0f;
            DWORD dwWitdh = 0;
            DWORD dwHeight = 0;

            // UI로 영역의 크기를 지정한 경우
            if( kHighlightInfo.bUseSizeFromUI == true ){
                KD3DWnd* pkWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, kHighlightInfo.UIScriptName.c_str(), "temp1", kHighlightInfo.UIName.c_str(), false );

                if( pkWnd != NULL ){
                    dwWitdh = pkWnd->GetWidth();
                    dwHeight = pkWnd->GetHeight();
                }

                DeleteChildByName( "temp1" );
            } else {
                dwWitdh = kHighlightInfo.dwHighlightWidth;
                dwHeight = kHighlightInfo.dwHighlightHeight;
            }

            // UI위치를 그대로 사용하는 경우
            if( kHighlightInfo.bUsePosFromUI == true ){
                KD3DWnd* pkWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, kHighlightInfo.UIScriptName.c_str(), "temp2", kHighlightInfo.UIName.c_str(), false );

                if( pkWnd != NULL ){
                    D3DXVECTOR2 vPos = pkWnd->GetFixedWindowLocalPos();
                    fx = vPos.x;
                    fy = vPos.y;
                }

                DeleteChildByName( "temp2" );
            } else {
                fx = kHighlightInfo.fHighLightPosX;
                fy = kHighlightInfo.fHighLightPosY;
            }

            // 위치 정보와 크기를 저장 (클릭에 동작하는 부분으로 하이라이트 영역보다 작음)
            m_fx_hl = fx + kHighlightInfo.ActiveOffset;
            m_fy_hl = fy + kHighlightInfo.ActiveOffset;
            m_dwWidth_hl = dwWitdh - ( kHighlightInfo.ActiveOffset * 2 );
            m_dwHeight_hl = dwHeight - ( kHighlightInfo.ActiveOffset * 2 );

            m_pkHighlight->SetHighLightRegion( fx, fy, dwHeight, dwWitdh );
            m_pkHighlight->ToggleRender( true );
            m_pkNoHighlight->ToggleRender( false );
            m_pkNoBlindHighlight->ToggleRender( false );
        }
        break;
    case GHT_NORMAL_WITHOUT_RECT:
        {
            m_pkHighlight->ToggleRender( false );
            m_pkNoHighlight->ToggleRender( true );
            m_pkNoBlindHighlight->ToggleRender( false );
        }
        break;
    case GHT_NO_BLIND_WITHOUT_RECT:
        {
            m_pkHighlight->ToggleRender( false );
            m_pkNoHighlight->ToggleRender( false );
            m_pkNoBlindHighlight->ToggleRender( true );
        }
        break;
    default:
        break;
    }

}

void KGCGuide::SetNPCInfo( IN SNPCInfo& kNPCInfo )
{
    if( kNPCInfo.nNPCType == -1 ){ // -1 : NPC 사용하지 않음
        for( int i = 0; i < GNPCT_MAX; ++i ) {
            m_pkGuideNPC[i]->ToggleRender( false );
        }

        return;
    }

    for( int i = 0; i < GNPCT_MAX; ++i ) {
        if( m_pkGuideNPC[i] == NULL )
            continue;

        if( i == kNPCInfo.nNPCType ) {
            // NPC 인덱스를 기억
            m_nNPCType = kNPCInfo.nNPCType;

            // 위치
            m_pkGuideNPC[i]->SetPosition( kNPCInfo.fNPCPosX, kNPCInfo.fNPCPosY );

            // 안내문 스트링
            m_pkGuideNPC[i]->SetDesc( kNPCInfo.nNPCDesc );

            // 보여주기
            m_pkGuideNPC[i]->ToggleRender( true );
        } else {
            m_pkGuideNPC[i]->ToggleRender( false );
        }
    }

}

void KGCGuide::SetArrowInfo( IN SArrowInfo& kArrowInfo )
{
    if( kArrowInfo.nArrowType == -1 ){ // -1 : Arrow 사용하지 않음
        m_pkGiudeArrow->ToggleRender( false );
        return;
    }

    if( m_pkGiudeArrow == NULL )
        return;

    // 화살표 타입
    m_pkGiudeArrow->SetArrowType( kArrowInfo.nArrowType );

    // 위치
    m_pkGiudeArrow->SetPosition( kArrowInfo.fArrowPosX, kArrowInfo.fArrowPosY );

    // 보여주기
    m_pkGiudeArrow->ToggleRender( true );
}

void KGCGuide::ClearPaticle()
{
    for(std::vector<CParticleEventSeqPTR>::iterator mIter = m_vecParticle.begin();mIter != m_vecParticle.end();mIter++)
    {
        g_ParticleManager->DeleteSequence( (*mIter) );
    }
    m_vecParticle.clear();
}

void KGCGuide::SetParticle( IN std::vector<SGuidePaticleInfo>& vecParticle )
{
    ClearPaticle();
    for(std::vector<SGuidePaticleInfo>::iterator vIt = vecParticle.begin();vIt != vecParticle.end();vIt++)
    {
        CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( vIt->strPaticleName.c_str(), vIt->x, vIt->y, 0.5f );
        m_vecParticle.push_back(pParticle);
        g_ParticleManager->SetLayer( pParticle, vIt->iLayer );

    }
}

void KGCGuide::SetCompleteAction( IN bool bAction )
{
    m_bCompleteAction = bAction;
    m_bItemRecieve = true;
}

void KGCGuide::UseExtraFunction()
{
    switch ( m_nCurrentGuideType )
    {
    case 1 : // GT_ENCHANT_NECKLACE_2
        if( m_nCurrentStep == 2 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 바람 목걸이를 맨 앞으로
            for(; iter != kStep.vecData.end(); ++iter ){
                if( g_pkUIScene->m_pkMyInfoScene->MakeItemOnFirstSlot( (*iter) / 10 ) == true )
                    break;
            }
        }
        break;
    case 0 : // GT_ENCHANT_NECKLACE_1
    case 7 : // GT_MONSTER_CARD_1
        if( m_nCurrentStep == 1 && m_bItemRecieve == false ){
            int nCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            KP2P::GetInstance()->Send_SystemGuideItemReq( m_nCurrentGuideType, nCharType );
        }
        break;
    case 2 : // GT_CRAFT_GEAR
        if( m_nCurrentStep == 1 && m_bItemRecieve == false ){
            int nCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            KP2P::GetInstance()->Send_SystemGuideItemReq( m_nCurrentGuideType, nCharType );
        } else if( m_nCurrentStep == 9 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 성투사 투구 레시피를 처음으로
            for(; iter != kStep.vecData.end(); ++iter ){
                if( SiGCForgeManager()->MakeRecipeOnFirstBar( (*iter) / 10 ) == true )
                    break;
            }
        }
        break;
    case 3 : // GT_BREAKUP_CRAFT
        if( m_nCurrentStep == 1 && m_bItemRecieve == false ){
            int nCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            KP2P::GetInstance()->Send_SystemGuideItemReq( m_nCurrentGuideType, nCharType );
        } else if ( m_nCurrentStep == 6 ) {
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 빅터(레어) 투구를 찾아 맨 앞으로
            for(; iter != kStep.vecData.end(); ++iter ){
                if( SiGCForgeManager()->MakeItemOnFirstSlot( (*iter) / 10 ) == true ){
                    g_pkUIScene->m_pkForgeDlg->OnClickBrakeUpBtn();
                    break;
                }
            }
        } else if ( m_nCurrentStep == 14 ) {
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 빅터 투구 레시피를 처음으로
            for(; iter != kStep.vecData.end(); ++iter ){
                if( SiGCForgeManager()->MakeRecipeOnFirstBar( (*iter) / 10 ) == true )
                    break;
            }
        }
        break;
    case 4 : // GT_CRAFT_PET_1
        if( m_nCurrentStep == 1 && m_bItemRecieve == false ){
            int nCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            KP2P::GetInstance()->Send_SystemGuideItemReq( m_nCurrentGuideType, nCharType );
        } else if( m_nCurrentStep == 3 ) {
            /////////////////////////////////////// 게이트 조각 미션 등록
            // Find Item
            KItem* pInvenItem = NULL;
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            for(; iter != kStep.vecData.end(); ++iter ){
                if( true == g_pItemMgr->FindInventoryForItemID( (*iter) / 10, &pInvenItem ) )
                {
                    GCItem* pkItemInfo = g_MyD3D->m_kItemMgr.GetItemData(  pInvenItem->m_ItemID );

                    // Regist Mission
                    Result_Kid_Gift = 100;
                    KP2P::GetInstance()->Send_RegistMission( (*iter), pInvenItem->m_ItemUID );
                    g_MyD3D->WaitForServerAck( Result_Kid_Gift, 100, 5000, 200 );
                    break;
                }
            }

        }
        break;
    case 6 : // GT_REINFORCE
        if( m_nCurrentStep == 1 && m_bItemRecieve == false ){
            int nCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            KP2P::GetInstance()->Send_SystemGuideItemReq( m_nCurrentGuideType, nCharType );
        } else if( m_nCurrentStep == 6 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 강화용 페가수스 무기를 찾아 맨 앞으로
//             for(; iter != kStep.vecData.end(); ++iter ){
//                 if( g_pkUIScene->m_pkNewEnchantMainDlg->MakeItemOnFirstSlot( (*iter) / 10 ) == true )
//                     break;
//             }
        } else if( m_nCurrentStep == 11 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 재물용 페가수스 무기를 찾아 맨 앞으로
//             for(; iter != kStep.vecData.end(); ++iter ){
//                 if( g_pkUIScene->m_pkNewEnchantMainDlg->MakeItemMattOnFirstSlot( (*iter) / 10 ) == true )
//                     break;
//             }
        }
        break;
    case 8:
        if( m_nCurrentStep == 0 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 성투사 무기 맨 앞으로
            for(; iter != kStep.vecData.end(); ++iter ){
                if( g_pkUIScene->m_pkMyInfoScene->MakeItemOnFirstSlot( (*iter) / 10 ) == true )
                    break;
            }
        } else if( m_nCurrentStep == 3 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            // 몬스터 카드
            if( g_pkUIScene->m_pkMonsterCardBook->MakeItemOnFirstSlot( (*iter) / 10 ) == true )
                break;
        }
        break;
    case 9:
        if( m_nCurrentStep == 1 ){
            g_MyD3D->OnMissionButton();
            SetCompleteAction( true );
        }
        break;
    case 10:
        if( m_nCurrentStep == 0 ){
            SStep& kStep = m_vecStep[m_nCurrentStep];
            std::vector<DWORD>::iterator iter = kStep.vecData.begin();

            for(; iter != kStep.vecData.end(); ++iter ){
                if( g_pkUIScene->m_pkMyInfoScene->MakeItemOnFirstSlot( (*iter) / 10 ) == true )
                    break;
            }
        }
        break;
    case 12:
        if( m_nCurrentStep == 1 ){
            g_MyD3D->OnMissionButton();
        }
        break;
    case 13:
        if ( m_nCurrentStep == 0 ){
            g_pkUIScene->m_pkWorldMapContainer->EventNewContinent(true);
        } else if ( m_nCurrentStep == 1 ) {
            g_pkUIScene->m_pkWorldMapContainer->EventNewContinent(false);
        }
        else if ( m_nCurrentStep == 3) {
            g_MyD3D->m_kGuideMgr.SetContinentEffect( true );
        }
        else if ( m_nCurrentStep == 4){
            g_MyD3D->m_kGuideMgr.SetContinentEffect( false );
        }
        break;
    case 14:
        if ( m_nCurrentStep == 1 ){
            g_pkUIScene->m_pkWorldMapContainer->EventNewContinent(true);
        } else if ( m_nCurrentStep == 2 ) {
            g_pkUIScene->m_pkWorldMapContainer->EventNewContinent(false);
        }
        else if ( m_nCurrentStep == 4) {
            g_MyD3D->m_kGuideMgr.SetContinentEffect( true );
        }
        else if ( m_nCurrentStep == 5){
            g_MyD3D->m_kGuideMgr.SetContinentEffect( false );
        }
        break;
    case 15: //GT_SKILL_TREE
        if ( m_nCurrentStep == 2 ){ //스킬가이드 스텝2일 경우
            KD3DWnd* pkWnd = g_pkUIScene->m_pkSkillTree->GetTrainableSkillBox();

            if( pkWnd !=  NULL ) {
                D3DXVECTOR2 vPos = pkWnd->GetCurrentWindowPos();
                m_pkHighlight->SetHighLightRegion( vPos.x - 5, vPos.y - 5, pkWnd->GetHeight() + 5, pkWnd->GetWidth() + 5 );
                m_fx_hl = vPos.x;
                m_fy_hl = vPos.y;
                m_dwWidth_hl = pkWnd->GetHeight();
                m_dwHeight_hl = pkWnd->GetWidth();
            }
        }
        break;
    case 16:
        if( m_nCurrentStep == 1 ){
            g_pkGameOverlayUI->m_pkAddtionGuide->SetGuideUserState(STATE_TUTORIAL_END);
        }
        break;
    default :
        break;
    }
}


/////////////////////////////////////// GCGuide end



IMPLEMENT_CLASSNAME( KGCGuideNPC );
IMPLEMENT_WND_FACTORY( KGCGuideNPC );
IMPLEMENT_WND_FACTORY_NAME( KGCGuideNPC, "gc_guide_npc" );

KGCGuideNPC::KGCGuideNPC(void)
{
    m_pkDesc = NULL;
    LINK_CONTROL( "static_ment", m_pkDesc );

    m_pkNPC = NULL;
    LINK_CONTROL( "npc", m_pkNPC );

    m_pkBalloon = NULL;
    LINK_CONTROL( "balloon", m_pkBalloon );

    m_strOriginalMent.clear();
    m_strMentAnim.clear();
}

KGCGuideNPC::~KGCGuideNPC(void)
{
}

void KGCGuideNPC::ActionPerformed( const KActionEvent& event )
{
    //
}

void KGCGuideNPC::OnCreate( void )
{
    m_pkDesc->InitState( true );
    m_pkDesc->SetFontColor( 0xff000000 );
    m_pkDesc->SetMultiLine( true );

    m_pkNPC->InitState( true );
    m_pkBalloon->InitState( true );
}

void KGCGuideNPC::FrameMoveInEnabledState( void )
{
    FrameMoveMent();
}

void KGCGuideNPC::SetPosition( IN float fx, IN float fy )
{
    this->SetFixedWindowLocalPos( D3DXVECTOR2( fx, fy ) );
}

void KGCGuideNPC::SetDesc( IN int nDescStrID )
{
    m_strOriginalMent = g_pkStrLoader->GetString( nDescStrID );
    m_strMentAnim.clear();
}

void KGCGuideNPC::FrameMoveMent()
{
    if( m_strOriginalMent.size() <= m_strMentAnim.size() )
        return;

    m_strMentAnim.append( m_strOriginalMent.substr( m_strMentAnim.size(), 1 ) );
    m_pkDesc->SetTextAutoMultiline( m_strMentAnim );
}


/////////////////////////////////////// GCGuideNPC end



IMPLEMENT_CLASSNAME( KGCGuideArrow );
IMPLEMENT_WND_FACTORY( KGCGuideArrow );
IMPLEMENT_WND_FACTORY_NAME( KGCGuideArrow, "gc_guide_arrow" );

KGCGuideArrow::KGCGuideArrow(void)
: nCurrentArrow( GAT_MAX )
, m_dwAnimTime( 0 )
, m_eWndState( D3DWM_DEFAULT )
{
    char szWndName[100] = {0, };

    for( int i = 0; i < GAT_MAX; ++i ){
        m_pkArrow[i] = NULL;
        sprintf_s( szWndName, 99, "arrow_%d", i );
        LINK_CONTROL( szWndName, m_pkArrow[i] );
    }

}

KGCGuideArrow::~KGCGuideArrow(void)
{
}

void KGCGuideArrow::ActionPerformed( const KActionEvent& event )
{
    //
}

void KGCGuideArrow::OnCreate( void )
{
    for( int i = 0; i < GAT_MAX; ++i ){
        m_pkArrow[i]->InitState( false );
    }
}

void KGCGuideArrow::FrameMoveInEnabledState( void )
{
    // 지정된 화살표가 없다면 리턴
    if( nCurrentArrow == GAT_MAX )
        return;

    if( m_dwAnimTime + dwFlipTime <= ::timeGetTime() )
    {
        m_dwAnimTime = ::timeGetTime();

        if( m_pkArrow[nCurrentArrow] ){

            // 디폴트와 오버시 이미지를 교차하면서 보여준다.
            if( m_pkArrow[nCurrentArrow]->GetWndMode() == D3DWM_DEFAULT )
                m_pkArrow[nCurrentArrow]->SetWndMode( D3DWM_HOVER );
            else
                m_pkArrow[nCurrentArrow]->SetWndMode( D3DWM_DEFAULT );
        }
    }
}

void KGCGuideArrow::SetArrowType( IN int nType )
{
    if( nType < 0 || nType > GAT_MAX )
        return;

    nCurrentArrow = nType;

    for( int i =0; i < GAT_MAX; ++i ){
        if( m_pkArrow[i] ){
            // 사용할 화살표만 켜줌
            if( i == nCurrentArrow )
                m_pkArrow[i]->ToggleRender( true );
            else
                m_pkArrow[i]->ToggleRender( false );
        }
    }
}

void KGCGuideArrow::SetPosition( IN float fx, IN float fy )
{
    D3DXVECTOR2 vecPos( fx, fy );
    m_pkArrow[nCurrentArrow]->SetFixedWindowLocalPos( vecPos );
}



/////////////////////////////////////// GCGuideArrow end