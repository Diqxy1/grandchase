#include "stdafx.h"
#include "KGCMissionReward_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionReward_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionReward_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionReward_S4, "gc_mission_reward_s4" );

KGCMissionReward_S4::KGCMissionReward_S4( void )
: m_pkBackGround( NULL )
, m_pkBack_Rew_Nor( NULL )
, m_pkBack_Rew_Sel( NULL )
, m_pkBnt_Rew_Nor( NULL )
, m_pkBnt_Rew_Sel( NULL )
, m_pkPerentBtnComplate( NULL )
, m_dwAniEndHeight( 0 )
, m_dwSelRewardItem( 0 )
, m_dwMainMissionID( 0 )
{
    LINK_CONTROL( "background_raward", m_pkBackGround );
    LINK_CONTROL( "back_reward_normal1", m_pkBack_Rew_Nor );
    LINK_CONTROL( "back_reward_select1", m_pkBack_Rew_Sel );
    LINK_CONTROL( "back_reward_normal2", m_pkBnt_Rew_Nor );
    LINK_CONTROL( "back_reward_select2", m_pkBnt_Rew_Sel );

    char szTemp[256];
    for(int iLoop=0; iLoop<NUM_REWARDITEM_BOX; ++iLoop)
    {
        sprintf(szTemp, "rewardbox_nor%d", iLoop);
        LINK_CONTROL( szTemp, m_pkRewardBox_Nor[iLoop] );
        sprintf(szTemp, "rewardbox_sel%d", iLoop);
        LINK_CONTROL( szTemp, m_pkRewardBox_Sel[iLoop] );
    }
}

KGCMissionReward_S4::~KGCMissionReward_S4( void ){}

void KGCMissionReward_S4::OnCreate( void )
{
    if( m_pkBackGround )
        m_pkBackGround->InitState( true, false );
    if( m_pkBack_Rew_Nor )
        m_pkBack_Rew_Nor->InitState( true, false );
    if( m_pkBack_Rew_Sel )
        m_pkBack_Rew_Sel->InitState( true, false );
    if( m_pkBnt_Rew_Nor )
        m_pkBnt_Rew_Nor->InitState( true, false );
    if( m_pkBnt_Rew_Sel )
        m_pkBnt_Rew_Sel->InitState( true, false );

    for(int iLoop=0; iLoop<NUM_REWARDITEM_BOX; ++iLoop)
    {
        if( m_pkRewardBox_Nor[iLoop] )
        {
            m_pkRewardBox_Nor[iLoop]->InitState( true, true, this );
        }

        if( m_pkRewardBox_Sel[iLoop] )
        {
            m_pkRewardBox_Sel[iLoop]->InitState( true, true, this );
            m_pkRewardBox_Sel[iLoop]->DisconnectAllEventProcedure();
            m_pkRewardBox_Sel[iLoop]->ConnectEventProcedure<KGCMissionReward_S4,void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCMissionReward_S4::OnClickSelItem, _1, m_pkRewardBox_Sel[iLoop]) );
        }
    }

    m_dwAniEndHeight = REWARDBOX_ORI_HEIGHT;
}

void KGCMissionReward_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionReward_S4::FrameMoveInEnabledState( void )
{
    bool bHover = false;
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    for(int iLoop = 0; iLoop < NUM_REWARDITEM_BOX; ++iLoop)
    {
        if( m_pkRewardBox_Nor[ iLoop ]->CheckPosInWindowBound( vMousePos ) )
            bHover = SetItemInfoBox( m_pkRewardBox_Nor[ iLoop ] );
        if( m_pkRewardBox_Sel[ iLoop ]->CheckPosInWindowBound( vMousePos ) )
            bHover = SetItemInfoBox( m_pkRewardBox_Sel[ iLoop ] );
        if( bHover )
            break;
    }

    if( bHover == false )
    {   g_pkUIScene->m_pkItemInformationBox->ToggleRender( false ); }

    if( m_pkBackGround && m_dwAniEndHeight != m_pkBackGround->GetHeight() )
        AnimationControll();
}

void KGCMissionReward_S4::CleanUp()
{
    m_dwSelRewardItem = 0;
    m_dwMainMissionID = 0;

    for(int iLoop=0; iLoop < NUM_REWARDITEM_BOX; ++iLoop)
    {
        m_pkRewardBox_Nor[ iLoop ]->ClearItem();
        m_pkRewardBox_Sel[ iLoop ]->ClearItem();
    }
}

void KGCMissionReward_S4::OnClickSelItem( KGCMissionRewardBox_S4* pWnd )
{
    if( pWnd && !g_pkUIScene->m_pkMissionUI_S4->GetPriview() )
    {
        m_dwSelRewardItem = pWnd->GetSettingItemID() * 10;
        if( m_dwSelRewardItem )
        {
            // 아이템박스 모드 변경
            for(int iLoop=0; iLoop<NUM_REWARDITEM_BOX; ++iLoop)
            {
                if( m_pkRewardBox_Sel[iLoop] )
                    m_pkRewardBox_Sel[iLoop]->SetBackWndMode( D3DWM_DEFAULT );
            }
            pWnd->SetBackWndMode( D3DWM_ACTIVE );

            // 미션 완료상태 확인 후 완료버튼처리
            if( m_pkPerentBtnComplate && g_kGlobalValue.m_kUserMission.IsCompleteMission( m_dwMainMissionID ) )
            {
                // 파티클 끌 시점
                g_pkUIScene->m_pkMissionUI_S4->GetSubMission()->SetCompleteEffect( 1 );
                m_pkPerentBtnComplate->SetWndMode( D3DWM_DEFAULT );
            }

            g_KDSound.Play( "31" );
        }
    }
}

void KGCMissionReward_S4::SetRewardItem( const std::vector< SMissionInfo::SReward >& vecNorItem, const std::vector< SMissionInfo::SReward >& vecSelItem )
{
    for(int iLoop=0; iLoop < NUM_REWARDITEM_BOX; ++iLoop)
    {
        // Normal RewardItems 셋팅
        if( iLoop < int(vecNorItem.size()) )
            SetRewardItem( m_pkRewardBox_Nor[iLoop], vecNorItem[iLoop] );
        else
            m_pkRewardBox_Nor[iLoop]->ClearItem();

        // Select RewardItems 셋팅
        if( iLoop < int(vecSelItem.size()) )
            SetRewardItem( m_pkRewardBox_Sel[iLoop], vecSelItem[iLoop] );
        else
            m_pkRewardBox_Sel[iLoop]->ClearItem();
    }

    // 애니메이션 셋팅
    m_dwAniEndHeight = REWARDBOX_ORI_HEIGHT;
    if( 0 == vecSelItem.size() )
        m_dwAniEndHeight -= GC_SCREEN_DIV_SIZE_INT(61);
    if( 0 == vecNorItem.size() )
        m_dwAniEndHeight -= GC_SCREEN_DIV_SIZE_INT(61);

    //작아지는 애니메이션( 커지는 애니메이션은 애니메이션이 끝날때 Toggle한다. )
    if( m_dwAniEndHeight <= m_pkBackGround->GetHeight() )
    {
        SetRenderState( (0 < vecSelItem.size()), (0 < vecNorItem.size()) );
    }

    //사이즈가 같아서 애니메이션이 발동되지 않는 상황에서
    //선택보상만 있을경우가 발생했다..
    //그래서 애니메이션이 없어도 어긋나지 않도록 
    //위치 한번 싹!~ 맞춰주고 시작하자!!
    RefreshWndPosition();
}

void KGCMissionReward_S4::SetRewardItem( KGCMissionRewardBox_S4* pItemWnd, const SMissionInfo::SReward& reawrd )
{
    if( NULL == pItemWnd )
        return;

    pItemWnd->SetItem( reawrd );
}

void KGCMissionReward_S4::SetFirstRewardBox()
{
    OnClickSelItem( m_pkRewardBox_Sel[0] );
}

bool KGCMissionReward_S4::SetItemInfoBox( KGCMissionRewardBox_S4 *pWnd )
{
    if( (NULL == pWnd) || (0 == pWnd->GetSettingItemID()) )
        return false;

    D3DXVECTOR2 vPos = pWnd->GetCurrentWindowPos();
    vPos.y += pWnd->GetHeight();
    vPos.x *= m_fWindowScaleX;    vPos.y *= m_fWindowScaleY;
    DrawItemInfoBox( true, pWnd->GetSettingItemID(), pWnd->GetGrade(), vPos  );
    return true;
}

void KGCMissionReward_S4::DrawItemInfoBox( bool bRender, DWORD dwGoodsID, char cGrade, D3DXVECTOR2 vPos )
{
    GCItem* kItem = NULL;
    kItem = g_pItemMgr->GetItemData( dwGoodsID );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem, -1, NULL, NULL, NULL, cGrade );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
}

void KGCMissionReward_S4::SetRenderState( bool bSelectBox, bool bNormalBox )
{
    if( m_pkBnt_Rew_Sel )
        m_pkBnt_Rew_Sel->ToggleRender( bSelectBox );
    if( m_pkBack_Rew_Sel )
        m_pkBack_Rew_Sel->ToggleRender( bSelectBox );
    if( m_pkBnt_Rew_Nor )
        m_pkBnt_Rew_Nor->ToggleRender( bNormalBox );
    if( m_pkBack_Rew_Nor )
        m_pkBack_Rew_Nor->ToggleRender( bNormalBox );
    for(int iLoop=0; iLoop<NUM_REWARDITEM_BOX; ++iLoop)
    {
        if( m_pkRewardBox_Sel[iLoop] )
            m_pkRewardBox_Sel[iLoop]->ToggleRender( bSelectBox );
        if( m_pkRewardBox_Nor[iLoop] )
            m_pkRewardBox_Nor[iLoop]->ToggleRender( bNormalBox );
    }
}

void KGCMissionReward_S4::AnimationControll()
{
    if( m_pkBackGround )
    {
        // 커지는 애니메이션
        if( m_dwAniEndHeight > m_pkBackGround->GetHeight() )
            Animation( ANIMATION_SPEED );
        // 작아지는 애니메이션
        else
            Animation( -ANIMATION_SPEED );
    }
}

void KGCMissionReward_S4::Animation( int iAniSpeed )
{
    bool bAniEnd = false;
    DWORD dwTempPos = m_pkBackGround->GetHeight() + iAniSpeed;

    // 애니메이션 종료조건 체크
    if( iAniSpeed > 0 )
    {
        if( (bAniEnd = (m_dwAniEndHeight < dwTempPos)) )
            dwTempPos = m_dwAniEndHeight;
    }
    else
    {
        if( (bAniEnd = (m_dwAniEndHeight > dwTempPos)) )
            dwTempPos = m_dwAniEndHeight;
    }

    // 사이즈 애니메이션
    int iGap = int(m_pkBackGround->GetHeight()-dwTempPos);
    m_pkBackGround->SetHeight( dwTempPos );

    // 위치 애니메이션
    D3DXVECTOR2 vecPos = m_pkBackGround->GetFixedWindowLocalPos();
    vecPos.y += iGap;
    m_pkBackGround->SetFixedWindowLocalPos( vecPos );

    // 컨트롤 위치 애니메이션 처리
    RefreshWndPosition();

    // 애니메이션이 끝날때 컨트롤들 랜더상태변경
    if( bAniEnd )
    {
        SetRenderState( (0 != m_pkRewardBox_Sel[0]->GetSettingItemID()), (0 != m_pkRewardBox_Nor[0]->GetSettingItemID()) );
    }
}

/*
    백그라운드에서 일반보상 박스와의 갭
        - 배경 : 7.0f;
        - 박스 : 9.0f;

    백그라운드에서 선택보상 박스와의 갭
        - 배경 : 69.0f;
        - 박스 : 71.0f;
*/
void KGCMissionReward_S4::RefreshWndPosition()
{
    float fSelect_Back = (0 == m_pkRewardBox_Nor[0]->GetSettingItemID()) ? 7.0f : 69.0f;
    float fSelect_Bnt = (0 == m_pkRewardBox_Nor[0]->GetSettingItemID()) ? 9.0f : 71.0f;
	fSelect_Back *= GC_SCREEN_DIV_HEIGHT;
	fSelect_Bnt *= GC_SCREEN_DIV_HEIGHT;

    SetPosFromGapBackGroundAndControll(m_pkBack_Rew_Nor, 7.0f);
    SetPosFromGapBackGroundAndControll(m_pkBnt_Rew_Nor, 9.0f);
    SetPosFromGapBackGroundAndControll(m_pkBack_Rew_Sel, fSelect_Back);
    SetPosFromGapBackGroundAndControll(m_pkBnt_Rew_Sel, fSelect_Bnt);

    for(int iLoop=0; iLoop<NUM_REWARDITEM_BOX; ++iLoop)
    {
        SetPosFromGapBackGroundAndControll(m_pkRewardBox_Nor[ iLoop ], 9.0f);
        SetPosFromGapBackGroundAndControll(m_pkRewardBox_Sel[ iLoop ], fSelect_Bnt);
    }
}

void KGCMissionReward_S4::SetPosFromGapBackGroundAndControll(KD3DWnd *pControll, float fGap)
{
    if( pControll && m_pkBackGround )
    {
        D3DXVECTOR2 vecBackPos = m_pkBackGround->GetFixedWindowLocalPos();
        D3DXVECTOR2 vecControllPos = pControll->GetFixedWindowLocalPos();
        vecControllPos.y = vecBackPos.y + fGap;
        pControll->SetFixedWindowLocalPos( vecControllPos );
    }
}