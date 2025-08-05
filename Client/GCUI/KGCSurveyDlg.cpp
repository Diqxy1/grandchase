#include "stdafx.h"
#include "KGCSurveyDlg.h"

IMPLEMENT_CLASSNAME( KGCSurveyDlg );
IMPLEMENT_WND_FACTORY( KGCSurveyDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSurveyDlg, "gc_survey_dlg" );

struct KSurveyQuestion;

KGCSurveyDlg::KGCSurveyDlg( void )
: m_pkBtnYes( NULL )
, m_pkBtnNo( NULL )
, m_pkStaticIntroduce( NULL )
, m_pkStaticRewardDesc( NULL )
, m_pkStaticReward( NULL )
, m_pkSurveyForm( NULL )
, m_iType( 0 )
, m_iDataFirst( 0 )
, m_iDataSecond( 0 )
, m_bResolvedFirstQuestion( false )
, m_bReserveQuestion( false )
{
    m_mapSurveyInfo.clear();
    m_mapNormalReward.clear();
    m_vecQuestionList.clear();

    for( int i = 0; i < EITEM_WND_NUM; i++ ) {
        char str[MAX_PATH] = {0,};
        m_pkImgNormalReward[i] = NULL;        
        sprintf( str, "lc_normal_reward%d", i );  
        LINK_CONTROL( str, m_pkImgNormalReward[i] );
    }

    LINK_CONTROL( "lc_btn_yes", m_pkBtnYes );
    LINK_CONTROL( "lc_btn_no", m_pkBtnNo );
    LINK_CONTROL( "lc_static_introduce", m_pkStaticIntroduce );
    LINK_CONTROL( "lc_static_reward_desc", m_pkStaticRewardDesc );
    LINK_CONTROL( "lc_static_reward", m_pkStaticReward );

    LINK_CONTROL( "survey_form", m_pkSurveyForm );

}

KGCSurveyDlg::~KGCSurveyDlg( void )
{
}

void KGCSurveyDlg::OnCreate( void )
{
    m_pkSurveyForm->InitState( false, true, this );
    m_pkBtnYes->InitState( true, true, this );
    m_pkBtnNo->SetHotKey( DIK_ESCAPE );
    m_pkBtnNo->InitState( true, true, this );

    
    m_pkStaticIntroduce->InitState( true );
    m_pkStaticIntroduce->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SURVEY_INTRODUCE_MAIN ) );

    m_pkStaticRewardDesc->SetFontColor( D3DCOLOR_ARGB( 255, 255, 218, 129 ) );
    m_pkStaticRewardDesc->SetFontOutline( true, D3DCOLOR_ARGB( 255, 19, 35, 51 ) );
    m_pkStaticRewardDesc->InitState( true );
    m_pkStaticRewardDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SURVEY_INTRODUCE_SUB ) );

    m_pkStaticReward->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
    m_pkStaticReward->SetFontOutline( true,  D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
    m_pkStaticReward->InitState( true );
    m_pkStaticReward->SetAlign( DT_CENTER );
    m_pkStaticReward->SetText( g_pkStrLoader->GetString( STR_ID_MISSION_DLG3 ) );
    
}

void KGCSurveyDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnYes,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnYes );
    GCWND_MSG_MAP( m_pkBtnNo,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnNo );
}

void KGCSurveyDlg::OnYes()
{
    g_KDSound.Play( "31" );
    m_bReserveQuestion = false;
    SetQuestionData();
    m_pkSurveyForm->ToggleRender(true);

    int iNum = m_pkSurveyForm->GetFirstQuestionNumber();

    m_pkSurveyForm->RenderQuestion( iNum );
}

void KGCSurveyDlg::OnNo()
{
    g_KDSound.Play( "31" );
    m_bReserveQuestion = false;
    this->ToggleRender(false);
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCSurveyDlg::SetQuestionList( int iType_, int iDataFirst_, int iDataSecond_, std::map< int, KSurveyQuestion >& mapSurveyInfo_ )
{
    m_iType = iType_;
    m_iDataFirst = iDataFirst_;
    m_iDataSecond = iDataSecond_;
    m_mapSurveyInfo = mapSurveyInfo_;

    sQuestion sQ;
    sQ.m_iType = iType_;
    sQ.m_iDataFirst = iDataFirst_;
    sQ.m_iDataSecond = iDataSecond_;
    sQ.m_mapSurveyInfo = mapSurveyInfo_;

    m_vecQuestionList.push_back( sQ );

}

void KGCSurveyDlg::SetRewardList( std::map< int, std::vector< KDropItemInfo > >& mapNormalReward_ )
{
    m_mapNormalReward = mapNormalReward_;
}

void KGCSurveyDlg::RenderRewardItem()
{
    // 이미지 초기화 
    for( int i = 0; i < EITEM_WND_NUM; i++ ) {
        m_pkImgNormalReward[i]->SetItemInfo( 0 );
    }

    sQuestion sQ;
    GetQuestion( sQ );
    int iType_ = sQ.m_iType;

    std::map< int, std::vector< KDropItemInfo > >::iterator mit;
    mit = m_mapNormalReward.find( iType_ );
    if( mit == m_mapNormalReward.end() ) {
        return;
    }

    for( int i = 0; i < static_cast< int >( mit->second.size() ); i++ ) {
        if( i < EITEM_WND_NUM ) {
            m_pkImgNormalReward[i]->SetItemInfo( mit->second[i].m_ItemID / 10 );
            m_pkImgNormalReward[i]->ShowItem( true );
        }
    }

    //for( int i = 0; i < EITEM_WND_NUM; i++ ) {
    //    m_pkImgNormalReward[i]->InitState( true );
    //}
}

void KGCSurveyDlg::OnSurveyRewardAck( std::vector< KItem >& vecReward_ )
{
    std::vector< KItem > vecItemNum;
    std::set< GCITEMUID > setExist;     // 이미 수량을 계산한 아이템인가 확인용.

    vecItemNum.clear();
    setExist.clear();

    std::vector< KItem >::iterator vit = vecReward_.begin();
    for ( ; vit != vecReward_.end(); ++vit ) {
        std::set< GCITEMUID >::iterator sit;
        sit = setExist.find( vit->m_ItemUID );
        if( sit != setExist.end() ) {
            continue;
        }
        setExist.insert( vit->m_ItemID );


        KItem kDiffInfo;
        kDiffInfo.m_ItemUID = vit->m_ItemUID;

        if ( vit->m_nCount == KItem::UNLIMITED_ITEM ) {  // 일반 아이템일 경우(장비; 영구, 수량아님)
            kDiffInfo.m_nCount = KItem::UNLIMITED_ITEM;
        }
        else {                                              // 수량 아이템일 경우 새로 얻은 수량만 출력되도록 처리
            DWORD dwReceivedCount = vit->m_nCount;
            KItem* bufItem = g_pItemMgr->GetInventoryItem( vit->m_ItemUID );
            if( bufItem != NULL ) {
                dwReceivedCount -= bufItem->m_nCount;
            }
            kDiffInfo.m_nCount = dwReceivedCount;

            GCITEMUID shortcutItemUID = -1;
            int iShortcutItemDuration = 0;
            for ( int slotIndex = 0; slotIndex < ITEM_SLOT_SIZE; ++slotIndex ) {
                shortcutItemUID = g_MyD3D->GetMyShortCutSlot()->GetItemUID( slotIndex );
                if ( shortcutItemUID == vit->m_ItemUID ) {
                    iShortcutItemDuration = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity( slotIndex );
                    break;
                }
                else {
                    iShortcutItemDuration = 0;
                }
            }
            kDiffInfo.m_nCount -= iShortcutItemDuration;
        }

        // 기간 아이템일 경우 처리를 해야 하는데... 해주세요
        //if ( vit->m_nPeriod == KItem::UNLIMITED_ITEM ) {  // 기간제 아이템일 경우
        //    kDiffInfo.m_nPeriod = KItem::UNLIMITED_ITEM;
        //}
        //else {
        //    DWORD dwReceivedPeriod = vit->m_nPeriod;
        //    KItem* bufItem = g_pItemMgr->GetInventoryItem( vit->m_ItemUID );
        //    if( bufItem != NULL ) {
        //        dwReceivedPeriod -= bufItem->m_nPeriod;
        //    }
        //    kDiffInfo.m_nPeriod = dwReceivedPeriod;
        //}
        // 넣고 보니 KGCItemReceiveBox에서 기간 표시가 지원되지 않는다... 그래서 주석처리
        vecItemNum.push_back( kDiffInfo );
    }

    g_pItemMgr->m_kInventory.AddItemList( vecReward_, true );

    if( !vecItemNum.empty() )
    {
        std::vector< KItem >::iterator vpit;
        for( vpit = vecItemNum.begin(); vpit != vecItemNum.end(); vpit++ )
            g_pkUIScene->m_pkItemReceiveBox->AddItem( vpit->m_ItemUID, vpit->m_nCount );
    }

    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateData();

    OnNo();
    g_pkUIScene->m_pkItemReceiveBox->SetEnableSell( true );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_RECEIVE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
}

void KGCSurveyDlg::SetQuestionData()
{
    m_pkSurveyForm->SetSurveyData( m_iType, m_iDataFirst, m_iDataSecond, m_mapSurveyInfo );
}

bool KGCSurveyDlg::IsHaveQuestion()
{
    return !m_vecQuestionList.empty();
}

void KGCSurveyDlg::SetHaveQuestion( bool bHaveQuestion_ )
{
    if ( !bHaveQuestion_ ) {
        m_vecQuestionList.clear();
    }
}

void KGCSurveyDlg::AddQuestion( sQuestion& sQuestion_ )
{
    m_vecQuestionList.push_back( sQuestion_ );
}

void KGCSurveyDlg::OnEventSurveyListNot( KEVENT_SURVEY_LIST_NOT& kRecv, bool bReserve_ )
{
    if ( kRecv.m_nType != -1
        && kRecv.m_nDataFirst != -1
        && kRecv.m_nDataSecond != -1
        && !kRecv.m_mapSurveyInfo.empty() ) {

        std::map< int, KSurveyQuestion > mapSurveyQuestion;

        std::map< int, KSerBuffer >::iterator mit;
        for ( mit = kRecv.m_mapSurveyInfo.begin(); mit != kRecv.m_mapSurveyInfo.end(); ++mit ) {
            KSurveyQuestion kSurveyQuestion;
            KSerBuffer kBuff = mit->second;
            kBuff.UnCompress();
            KSerializer ks;
            ks.BeginReading( &kBuff );
            ks.Get( kSurveyQuestion );
            ks.EndReading();

            if ( kSurveyQuestion.m_iQuestionType < 0 || kSurveyQuestion.m_iQuestionType > 201 )
                return;

            if ( !mapSurveyQuestion.insert( std::make_pair( mit->first, kSurveyQuestion ) ).second ) {
                // 맵에 삽입 중 오류
                return;
            }
        }

        SetQuestionList( kRecv.m_nType, kRecv.m_nDataFirst, kRecv.m_nDataSecond, mapSurveyQuestion );

        m_bReserveQuestion = bReserve_;
        if ( !m_bReserveQuestion ) {   // 즉시 보여줌
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );
        }

        RenderRewardItem();
    }
}

void KGCSurveyDlg::GetQuestion( sQuestion& sQuestion_ )
{
    sQuestion_ = m_vecQuestionList.back();
    m_iType = sQuestion_.m_iType;
    m_iDataFirst = sQuestion_.m_iDataFirst;
    m_iDataSecond = sQuestion_.m_iDataSecond;
    m_mapSurveyInfo = sQuestion_.m_mapSurveyInfo;

    if ( m_iType != 0 ) {
        m_vecQuestionList.pop_back();
    }
}
