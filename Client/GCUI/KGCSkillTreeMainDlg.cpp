#include "stdafx.h"
#include "gcui/KGCSkillTreeMainDlg.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCSkillTreeMainDlg );
IMPLEMENT_WND_FACTORY( KGCSkillTreeMainDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillTreeMainDlg, "gc_skill_tree_main_dlg" );

KGCSkillTreeMainDlg::KGCSkillTreeMainDlg(void)
: m_pkQuickSkillBox( NULL )
, m_pkSkillSelectWnd( NULL )
, m_pkBtnClose( NULL )
, m_pkBtnInitSkill( NULL )
, m_pkBtnCancelSettings( NULL )
, m_pkStaticSkillPoint( NULL )
, m_pkScrollBarCharSkill( NULL )
, m_pkScrollRangeCharSkill( NULL )
, m_pkScrollBarClassSkill( NULL )
, m_pkScrollRangeClassSkill( NULL )
, m_pkBtnSelectClass( NULL )
, m_pkPopupMenuClass( NULL )
, m_pkStaticClass( NULL )
, m_pkAwakeningSkillDlg( NULL )
, m_pkSkillDescBox( NULL )
, m_vAwakeningProgressCoords( 0.0f, 0.0f, 0.0f, 0.0f )
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < ESKILL_BOX_COUNT; ++i )
    {
        m_pkCharSkillBox[i] = NULL;
        sprintf( str, "char_skill_box_%d", i );  
        LINK_CONTROL( str, m_pkCharSkillBox[i] ); 

        m_pkClassSkillBox[i] = NULL;
        sprintf( str, "class_skill_box_%d", i );  
        LINK_CONTROL( str, m_pkClassSkillBox[i] ); 
    }

    LINK_CONTROL( "quick_skill_box",            m_pkQuickSkillBox );
    LINK_CONTROL( "skill_select_wnd",           m_pkSkillSelectWnd );
    LINK_CONTROL( "btn_close",                  m_pkBtnClose );
    LINK_CONTROL( "btn_init_skill",             m_pkBtnInitSkill );
    LINK_CONTROL( "btn_cancel_setttings",       m_pkBtnCancelSettings );
    LINK_CONTROL( "static_skill_point",         m_pkStaticSkillPoint );
    LINK_CONTROL( "scroll_char_skill",          m_pkScrollBarCharSkill );
    LINK_CONTROL( "range_scroll_char_skill",    m_pkScrollRangeCharSkill );    
    LINK_CONTROL( "scroll_class_skill",         m_pkScrollBarClassSkill );
    LINK_CONTROL( "range_scroll_class_skill",   m_pkScrollRangeClassSkill );
    LINK_CONTROL( "btn_select_class",           m_pkBtnSelectClass );    
    LINK_CONTROL( "popupbox_class",             m_pkPopupMenuClass );    
    LINK_CONTROL( "static_current_class",       m_pkStaticClass );
    LINK_CONTROL( "awakening_prograss_bar",     m_pkAwakeningProgressBar );
    LINK_CONTROL( "static_awakening_progress",  m_pkStaticAwakeningProgress );
    LINK_CONTROL( "static_next_awakening_lv",   m_pkStaticNextAwakeningLV );
    LINK_CONTROL( "awakening_skill_dlg",        m_pkAwakeningSkillDlg );
    LINK_CONTROL( "skill_desc_box",             m_pkSkillDescBox );

}

KGCSkillTreeMainDlg::~KGCSkillTreeMainDlg(void)
{
}

void KGCSkillTreeMainDlg::OnCreate( void )
{       
    for( int i = 0; i < ESKILL_BOX_COUNT; i++ ) {
        m_pkCharSkillBox[i]->InitState( true, true, this );
        m_pkClassSkillBox[i]->InitState( true, true, this );
    }

    m_pkQuickSkillBox->InitState( true, true, this );
    m_pkSkillSelectWnd->InitState( false, true, this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnInitSkill->InitState( true, true, this );
    m_pkBtnCancelSettings->InitState( true, true, this );
    m_pkBtnSelectClass->InitState( true, true, this );
    m_pkPopupMenuClass->InitState( false, true, this );
    m_pkStaticClass->InitState( true );
    m_pkStaticClass->SetText( GCFUNC::GetPromotionName( m_cUICharType, m_cUIPromotion ) );

    m_pkAwakeningSkillDlg->InitState( true, true, this );

    // 캐릭터 스킬 스크롤
    m_pkScrollBarCharSkill->InitState( true, true, this );
    m_pkScrollBarCharSkill->SetTargetWnd( m_pkScrollRangeCharSkill );
    m_pkScrollRangeCharSkill->InitState( true );

    // 클래스 스킬 스크롤
    m_pkScrollBarClassSkill->InitState( true, true, this );    
    m_pkScrollBarClassSkill->SetTargetWnd( m_pkScrollRangeClassSkill );
    m_pkScrollRangeClassSkill->InitState( true );

    // 각성 달성 진행율 바 관련
    m_pkAwakeningProgressBar->InitState( true );
    m_pkStaticAwakeningProgress->InitState( true );
    m_pkStaticAwakeningProgress->SetAlign( DT_CENTER );
    m_pkStaticAwakeningProgress->SetText(L"");
    m_pkStaticNextAwakeningLV->InitState( true );
    m_pkStaticNextAwakeningLV->SetAlign( DT_RIGHT );
    m_pkStaticNextAwakeningLV->SetText(L"");

    m_pkStaticSkillPoint->InitState( true );
    m_pkStaticSkillPoint->SetAlign( DT_CENTER );

    m_pkSkillDescBox->InitState( false );

    m_vAwakeningProgressCoords = m_pkAwakeningProgressBar->GetWndTexCoords();

    SiKGCSkillTree()->GetAllEquipedSkillInfo( m_mapOriAllEquipedSkillInfo );
    SiKGCSkillTree()->GetAllTrainedSkillInfo( m_mapOriTrainedSkill );
    SiKGCSkillTree()->GetTutorialTrainingSkill( m_mapOriTutorialTrainedSkill );
    SiKGCSkillTree()->SetCurUIPromotion( m_cUIPromotion );
    UpdateSkillTreeMainDlg();

    RenderAwakeningProgress();
}

void KGCSkillTreeMainDlg::OnCreateComplete( void )
{
    OnScrollCharSkill();
    OnScrollClassSkill();
    m_pkAwakeningSkillDlg->UpdateAwakeningSkillDlg( m_vecAwakeningSkills );
}

void KGCSkillTreeMainDlg::InitSkillTreeSettings()
{
    m_vAwakeningProgressCoords = m_pkAwakeningProgressBar->GetWndTexCoords();

    SiKGCSkillTree()->GetAllEquipedSkillInfo( m_mapOriAllEquipedSkillInfo );
    SiKGCSkillTree()->GetAllTrainedSkillInfo( m_mapOriTrainedSkill );
    SiKGCSkillTree()->GetTutorialTrainingSkill( m_mapOriTutorialTrainedSkill );
    SiKGCSkillTree()->SetCurUIPromotion( m_cUIPromotion );
    UpdateSkillTreeMainDlg();

    RenderAwakeningProgress();

    OnScrollCharSkill();
    OnScrollClassSkill();
    m_pkAwakeningSkillDlg->UpdateAwakeningSkillDlg( m_vecAwakeningSkills );
}

void KGCSkillTreeMainDlg::ActionPerformed( const KActionEvent& event )
{
    // 퀵 스킬 슬롯에서 슬롯 아이콘을 눌렀을 시
    if( event.m_pWnd == m_pkQuickSkillBox && event.m_dwlParam == KGCQuickSkillBox::ECLICK_SKILL_SLOT ) {
        ShowSkillSelectWnd( true );
        SetLockScrollBar( true );
        return;
    }

    // 스킬 셀렉트 박스에서 이전, 다음 버튼을 클릭 시
    if( event.m_pWnd == m_pkQuickSkillBox && event.m_dwlParam == KGCQuickSkillBox::ECLICK_MOVE_BTN ) {
        OnScrollClassSkill();        
        return;
    }

    // 스킬 셀렉트 박스에서 퀵스킬 슬롯을 클릭 시
    if( event.m_pWnd == m_pkQuickSkillBox && event.m_dwlParam == KGCQuickSkillBox::ESELECT_SKILL_WND_OFF ) {
        ShowSkillSelectWnd( false );
        SetLockScrollBar( false );
        return;
    }

    // 이 밑은 스킬 셀렉트 박스가 그려지고 있으면 무시하도록 하자.
    if( m_pkSkillSelectWnd->IsRenderOn() )
        return;

    _GCWND_MSG_MAP( m_pkBtnClose,			        KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    _GCWND_MSG_MAP( m_pkBtnCancelSettings,			KD3DWnd::D3DWE_BUTTON_CLICK, OnCancelSkillSettings );
    _GCWND_MSG_MAP( m_pkBtnInitSkill,			    KD3DWnd::D3DWE_BUTTON_CLICK, OnInitSkillSettings );    

    GCWND_MAP( m_pkScrollBarCharSkill, OnScrollCharSkill );
    GCWND_MAP( m_pkScrollBarCharSkill, OnScrollClassSkill );

    // 아래 for문에서 스킬 익히기, 제거하기를 처리한다. 
    // 되돌리기 기능인해 바로 패킷을 보내지는 않고 저장을 해두고 창을 끌 때 한꺼번에 보내는 구조로 되어있다.
    for( int i = 0; i < ESKILL_BOX_COUNT; i++ ) {
        if( event.m_pWnd == m_pkCharSkillBox[i] && event.m_dwlParam == KGCCharSkillBox::ECLICK_TRAIN_BTN ) {
            if ( g_kGlobalValue.IsTutorialMode() ) {
                SiKGCSkillTree()->TutorialTrainingSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            } else {
                SiKGCSkillTree()->Server_TrainSkill( static_cast< int >( event.m_dwCode ) );
                SiKGCSkillTree()->EquipSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
                SiKGCSkillTree()->SaveTrainSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            }
            UpdateSkillTreeData();
            OnScrollCharSkill();
        } else if( event.m_pWnd == m_pkCharSkillBox[i] && event.m_dwlParam == KGCCharSkillBox::ECLICK_UNTRAIN_BTN ) {
            if ( g_kGlobalValue.IsTutorialMode() ) {
                SiKGCSkillTree()->TutorialUnTrainingSkill( GetUntrainSkillID( static_cast< EGCSkillTree >( event.m_dwCode ) ) );            
            } else {
                SiKGCSkillTree()->Server_UnTrainSkill( static_cast< int >( GetUntrainSkillID( static_cast< EGCSkillTree >( event.m_dwCode ) ) ) );
                SiKGCSkillTree()->SaveRemoveSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            }
            UpdateSkillTreeData();
            OnScrollCharSkill();
        } else if( event.m_pWnd == m_pkClassSkillBox[i] && event.m_dwlParam == KGCClassSkillBox::ECLICK_USE_NOUSE_BTN ) {
            UpdateSkillTreeData();
            OnScrollClassSkill();
        } else if( event.m_pWnd == m_pkClassSkillBox[i] && event.m_dwlParam == KGCClassSkillBox::ECLICK_TRAIN_SKILL ) {
            SiKGCSkillTree()->Server_TrainSkill( static_cast< int >( event.m_dwCode ) );
            const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            if( pSkill && pSkill->m_pHotKeySkill == NULL ) {
                SiKGCSkillTree()->EquipSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            }
            SiKGCSkillTree()->SaveTrainSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            UpdateSkillTreeData();
            OnScrollClassSkill();
        }
    }
   
    // 스킬 셀렉트 박스에서 스킬 아이콘을 선택 했을 시
    if( event.m_pWnd == m_pkSkillSelectWnd && event.m_dwlParam == KGCSkillSelectWnd::ECLICK_SKILL_ICON ) {
        m_pkQuickSkillBox->SetQuickSkillSlot( static_cast< EGCSkillTree >( event.m_dwCode ) );        
        OnScrollClassSkill();
        SetLockScrollBar( false );
        return;
    }    

    if( event.m_pWnd == m_pkBtnSelectClass && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        if( m_pkPopupMenuClass->IsRenderOn() )
            m_pkPopupMenuClass->ToggleRender(false);
        else
            m_pkPopupMenuClass->ToggleRender(true);
        g_KDSound.Play( "31" );
    }

    if( event.m_pWnd == m_pkPopupMenuClass && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
    {
        m_pkPopupMenuClass->ToggleRender(false);
        SiKGCSkillTree()->SetCurUIPromotion( static_cast< char >(event.m_dwlParam) );
        m_pkStaticClass->SetText( GCFUNC::GetPromotionName( SiKGCSkillTree()->GetCurUICharType(), SiKGCSkillTree()->GetCurUIPromotion() ) );
        UpdateSkillTreeMainDlg();
        OnScrollClassSkill();
        return;
    }
}


void KGCSkillTreeMainDlg::FrameMoveInEnabledState( void )
{
    if ( m_pkSkillSelectWnd->IsRenderOn() ) {
        if( g_pkInput->IsDown( DIK_ESCAPE )  ) {
            ShowSkillSelectWnd( false );
            SetLockScrollBar( false );
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        }        
        return;
    }

    if ( g_pkInput->IsDown( DIK_ESCAPE ) && m_pkSkillSelectWnd->IsRenderOn() == false ) {
        OnClose();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        return;
    }

    if( m_pkPopupMenuClass->IsRenderOn() )        
        return;

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( m_pkScrollRangeCharSkill->CheckPosInWindowBound(vMousePos) ) {
        if( g_pkInput->ISWheelUp() ) {
            if( m_pkScrollBarCharSkill->GetScrollPos() >= 0 ) {
                m_pkScrollBarCharSkill->SetScrollPos( m_pkScrollBarCharSkill->GetScrollPos() );
                OnScrollCharSkill();
                g_KDSound.Play( "73" );
            }
        } else if( g_pkInput->ISWheelDown() ) {
            if( m_pkScrollBarCharSkill->GetScrollPos() <= m_pkScrollBarCharSkill->GetScrollRangeMax() ) {
                m_pkScrollBarCharSkill->SetScrollPos( m_pkScrollBarCharSkill->GetScrollPos() );
                OnScrollCharSkill();
                g_KDSound.Play( "73" );
            }
        }	
    } else if( m_pkScrollRangeClassSkill->CheckPosInWindowBound(vMousePos) ) {
        if( g_pkInput->ISWheelUp() ) {
            if( m_pkScrollBarClassSkill->GetScrollPos() >= 0 ) {
                m_pkScrollBarClassSkill->SetScrollPos( m_pkScrollBarClassSkill->GetScrollPos() );
                OnScrollClassSkill();
                g_KDSound.Play( "73" );
            }
        } else if( g_pkInput->ISWheelDown() ) {
            if( m_pkScrollBarClassSkill->GetScrollPos() <= m_pkScrollBarClassSkill->GetScrollRangeMax() ) {
                m_pkScrollBarClassSkill->SetScrollPos( m_pkScrollBarClassSkill->GetScrollPos() );
                OnScrollClassSkill();
                g_KDSound.Play( "73" );
            }
        }	
    }

    int iHoverIndex = -1;
    int iBoxType = 0;
    EGCSkillTree eSkillID = (EGCSkillTree)-1;
    D3DXVECTOR2 vDescBoxPos;
    for( int i = 0; i < ESKILL_BOX_COUNT; i++ ) {
        if( m_pkCharSkillBox[i]->CheckPosInWindowBound(vMousePos) ) {
            if( m_vecCharSkills.size() < ESKILL_BOX_COUNT && i >= static_cast< int >( m_vecCharSkills.size() ) ) {
                continue;
            }

            float yPos;
            if( i < 2 ) {
                yPos = m_pkCharSkillBox[i]->GetFixedWindowLocalPos().y;
            } else {
                yPos = m_pkCharSkillBox[i]->GetFixedWindowLocalPos().y - KGCSkillDescBox::EBOX_HEIGHT_TYPE_A + m_pkCharSkillBox[i]->GetHeight();
            }

            m_pkCharSkillBox[i]->RenderHoverFrame( true );
            eSkillID = m_pkCharSkillBox[i]->GetSkillID();
            iHoverIndex = i;
            iBoxType = KGCSkillDescBox::EBOX_TYPE_A;
            vDescBoxPos = D3DXVECTOR2( m_pkCharSkillBox[i]->GetFixedWindowLocalPos().x + m_pkCharSkillBox[i]->GetWidth(), yPos );            
        } else if( m_pkClassSkillBox[i]->CheckPosInWindowBound(vMousePos) ) {
            if( m_vecCurClassSkills.size() < ESKILL_BOX_COUNT && i >= static_cast< int >( m_vecCurClassSkills.size() ) ) {
                continue;
            }

            float yPos;
            if( i < 2 ) {
                yPos = m_pkClassSkillBox[i]->GetFixedWindowLocalPos().y;
            } else {
                yPos = m_pkClassSkillBox[i]->GetFixedWindowLocalPos().y - m_pkSkillDescBox->GetHeight() + m_pkClassSkillBox[i]->GetHeight();
            }
            
            m_pkClassSkillBox[i]->RenderHoverFrame( true );
            eSkillID = m_pkClassSkillBox[i]->GetSkillID();
            iHoverIndex = i;
            iBoxType = KGCSkillDescBox::EBOX_TYPE_B;
            vDescBoxPos = D3DXVECTOR2( m_pkClassSkillBox[i]->GetFixedWindowLocalPos().x + m_pkCharSkillBox[i]->GetWidth(), yPos );
        } else {
            m_pkCharSkillBox[i]->RenderHoverFrame( false );            
            m_pkClassSkillBox[i]->RenderHoverFrame( false );           
        }
    }

    if( iHoverIndex != -1 && 
        ( m_pkCharSkillBox[iHoverIndex]->CheckPosInWindowBound(vMousePos) || m_pkClassSkillBox[iHoverIndex]->CheckPosInWindowBound(vMousePos) ) ) {
        m_pkSkillDescBox->ToggleRender( true );
        m_pkSkillDescBox->SetSkillDesc( iBoxType, eSkillID );
        m_pkSkillDescBox->SetPosition( vDescBoxPos );
    } else {
        m_pkSkillDescBox->ToggleRender( false );
    }
}

void KGCSkillTreeMainDlg::OnScrollCharSkill()
{
    int iRealindex = m_pkScrollBarCharSkill->GetScrollPos();
    int iMaxListView = static_cast<int>( m_vecCharSkills.size() ) - iRealindex;

    if( iMaxListView >= ESKILL_BOX_COUNT) {
        iMaxListView = ESKILL_BOX_COUNT;
    }

    for(int i = 0; i < iMaxListView; ++i ) {
        int iIndex = i + iRealindex;

        if( 0 <= iIndex && iIndex < static_cast<int>( m_vecCharSkills.size() ) ) {		
            m_pkCharSkillBox[i]->ToggleRender( true );
            if( static_cast<int>( m_vecCharSkills.size() ) > iIndex) {
                m_pkCharSkillBox[i]->SetCharSkillInfo( m_vecCharSkills[iIndex] );
            }
            else {
                m_pkCharSkillBox[i]->EmptyBoxState();
            }
        }
    }

    for(int i = iMaxListView; i < ESKILL_BOX_COUNT; ++i)
    {
        m_pkCharSkillBox[i]->EmptyBoxState();
    }
}

void KGCSkillTreeMainDlg::OnScrollClassSkill()
{    
    std::vector< EGCSkillTree > vecClassSkills;
    std::map< char, std::vector< EGCSkillTree > >::iterator mit = m_mapClassSkills.find( SiKGCSkillTree()->GetCurUIPromotion() );
    if( mit != m_mapClassSkills.end() ) {
        vecClassSkills = mit->second;
    }

    int iRealindex = m_pkScrollBarClassSkill->GetScrollPos();
    int iMaxListView = static_cast<int>( vecClassSkills.size() ) - iRealindex;

    if( iMaxListView >= ESKILL_BOX_COUNT) {
        iMaxListView = ESKILL_BOX_COUNT;
    }

    for(int i = 0; i < iMaxListView; ++i ) {
        int iIndex = i + iRealindex;

        if( 0 <= iIndex && iIndex < static_cast<int>( vecClassSkills.size() ) ) {		
            m_pkClassSkillBox[i]->ToggleRender( true );
            if( static_cast<int>( vecClassSkills.size() ) > iIndex) {
                m_pkClassSkillBox[i]->SetClassSkillInfo( vecClassSkills[iIndex] );
            }
            else {
                m_pkClassSkillBox[i]->EmptyBoxState();
            }
        }
    }

    for(int i = iMaxListView; i < ESKILL_BOX_COUNT; ++i)
    {
        m_pkClassSkillBox[i]->EmptyBoxState();
    }
}

void KGCSkillTreeMainDlg::OnClose()
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> > mapCurEquipedSkillInfo;
    SiKGCSkillTree()->GetAllEquipedSkillInfo( mapCurEquipedSkillInfo );

    std::map< std::pair<EGCCharType, char>, SkillSet > mapCurTrainedSkill;
    SiKGCSkillTree()->GetAllTrainedSkillInfo( mapCurTrainedSkill );

    // 스킬 창 열었을 시 장착정보와 현재 장착정보가 틀리면 설정변경 확인 메시지 박스를 띄워주자.
    if( m_mapOriAllEquipedSkillInfo != mapCurEquipedSkillInfo || 
        m_mapOriTrainedSkill != mapCurTrainedSkill ||
        IsChangeSkillEquipInfo() == true ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_SKILL_TREE_CHANGE_MESSAGE ), L"",
            KGCUIScene::GC_MBOX_USE_CHAGE_SKILL_SETTINGS, 0, 0, false, true, true );
    } else {
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    }        
}

void KGCSkillTreeMainDlg::OnCancelSkillSettings()
{
    SiKGCSkillTree()->SetAllEquipedSkillInfo( m_mapOriAllEquipedSkillInfo );
    SiKGCSkillTree()->SetAllTrainedSkillInfo( m_mapOriTrainedSkill );
    SiKGCSkillTree()->SetTutorialTrainingSkill( m_mapOriTutorialTrainedSkill );
    UpdateSkillTreeMainDlg();
    OnScrollCharSkill();
    OnScrollClassSkill();
}

void KGCSkillTreeMainDlg::OnInitSkillSettings()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_SKILL_TREE_CHANGE_MESSAGE ), L"",
        KGCUIScene::GC_MBOX_USE_INIT_SKILL_SETTINGS, 0, 0, false, true );
}

void KGCSkillTreeMainDlg::ShowSkillSelectWnd( bool bRender_ )
{
    m_pkSkillSelectWnd->ToggleRender( bRender_ );
    m_pkSkillSelectWnd->UpdateSkillIcons();
}

void KGCSkillTreeMainDlg::UpdateSkillTreeMainDlg()
{    
    m_pkQuickSkillBox->UpdateSkillSlot();    

    SiKGCSkillTree()->SetCurUICharType( m_cUICharType );    

    UpdateSkillTreeData();    

    // 캐릭터 스킬 스크롤 셋팅
    int iFileCount = static_cast<int>( m_vecCharSkills.size() );
    int iScrollMax = 0;
    int iCount =  iFileCount - ESKILL_BOX_COUNT;
    if( iCount > 0 ) {
        iScrollMax = iCount;
    }

    m_pkScrollBarCharSkill->InitScroll( 0, 1, 0, iScrollMax + 1, 2, 0, 1, true );    
 

    // 클래스 스킬 스크롤 셋팅
    std::map< char, std::vector< EGCSkillTree > >::iterator mit = m_mapClassSkills.find( m_cUIPromotion );
    if( mit != m_mapClassSkills.end() ) {
        iFileCount = static_cast<int>( mit->second.size() );
    } else {
        iFileCount = 0;
    }

    iScrollMax = 0;
    iCount =  iFileCount - ESKILL_BOX_COUNT;
    if( iCount > 0 ) {
        iScrollMax = iCount;
    }

    m_pkScrollBarClassSkill->InitScroll( 0, 1, 0, iScrollMax + 1, 2, 0, 1, true );        
}

void KGCSkillTreeMainDlg::SetLockScrollBar( bool bLock_ )
{
    m_pkScrollBarCharSkill->Lock( bLock_ );
    m_pkScrollBarClassSkill->Lock( bLock_ );
}

void KGCSkillTreeMainDlg::RenderAwakeningProgress()
{
    if( m_vecAwakeningSkills.empty() ) {
        return;
    }

    float fAmount = 0.0; // 레벨비례한 비율 공식 적용하자.
    int iCurAwakeningLv = 0;

    if ( g_kGlobalValue.IsTutorialMode() ) {
        fAmount = 1.f;
        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( m_vecAwakeningSkills[ static_cast< int >( m_vecAwakeningSkills.size() ) - 1 ] );
        if( pSkill ) {
            iCurAwakeningLv = pSkill->GetLv();
        } else {
            iCurAwakeningLv = EMAX_AWAKENING_SKILL_LV;
        }
    } else {
        int iCharLv = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel;
        int iLevelRange = 0;

        for( int i = 0; i < static_cast< int >( m_vecAwakeningSkills.size() ); i++ ) {
            const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( m_vecAwakeningSkills[i] );
            // 각성 차수를 익히지 않았다면...
            if( pSkill && !SiKGCSkillTree()->IsLearnedSkill( m_vecAwakeningSkills[i] ) ) {
                if( i == 0 ) {
                    iLevelRange = pSkill->GetLv();
                } else {
                    const GCSkill* pPrevSkill = SiKGCSkillTree()->GetSkill( m_vecAwakeningSkills[i - 1] );
                    if( pPrevSkill ) {
                        iLevelRange = pSkill->GetLv() - pPrevSkill->GetLv();
                    } else {
                        iLevelRange = -1;
                    }
                }
                iCurAwakeningLv = pSkill->GetLv();
                break;
            }
        }

        // 이미 모든 각성을 다 마스터한 상태이다.
        if( iLevelRange == 0 ) {
            fAmount = 1.f;
        } else if( iLevelRange > 0 ){
            fAmount = static_cast< float >( iCurAwakeningLv / iLevelRange );
        }        
    }    

    float fTU = m_vAwakeningProgressCoords.x + fAmount * ( m_vAwakeningProgressCoords.z - m_vAwakeningProgressCoords.x );
    m_pkAwakeningProgressBar->SetWndTexCoords( D3DXVECTOR4( m_vAwakeningProgressCoords.x,
        m_vAwakeningProgressCoords.y, fTU,
        m_vAwakeningProgressCoords.w ) );

    m_pkAwakeningProgressBar->SetWidthDirect( static_cast<int>( fAmount * EWIDTH_PROGRESS_BAR ) );

    WCHAR strTemp[MAX_PATH];
    swprintf( strTemp, MAX_PATH, L"%5.1f%%", fAmount * 100.f );
    m_pkStaticAwakeningProgress->SetText( strTemp );
    swprintf( strTemp, MAX_PATH, L"LV.%d", iCurAwakeningLv );
    m_pkStaticNextAwakeningLV->SetText( strTemp );
}

void KGCSkillTreeMainDlg::UpdateSkillTreeData()
{
    m_vecCharSkills.clear();
    m_mapClassSkills.clear();
    m_vecAwakeningSkills.clear();
    m_mapLvSkills.clear();
    std::vector< EGCSkillTree > vecOriCharSkills = SiKGCSkillTree()->GetSkillsPerClass( m_cUICharType );    
    for( int i = 0; i < static_cast< int >( vecOriCharSkills.size() ); i++ ) {
        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( vecOriCharSkills[i] );
        if( pSkill && pSkill->m_iCurSkillLv != -1 ) {
            m_mapLvSkills[ pSkill->GetGroupID() ].push_back( vecOriCharSkills[i] );
        } else if ( pSkill && pSkill->m_iAwakeningLv != -1 ) {
            m_vecAwakeningSkills.push_back( pSkill->GetID() );
        }
        else {
            m_vecCharSkills.push_back( vecOriCharSkills[i] );
        }
    }

    std::sort( m_vecAwakeningSkills.begin(), m_vecAwakeningSkills.end(), SortByLevelToSkillID() );

    m_pkAwakeningSkillDlg->UpdateAwakeningSkillDlg( m_vecAwakeningSkills );

    // 스킬 레벨을 가지는 녀석들은 하나만 담아준다.
    std::map< EGCSkillGroup, std::vector< EGCSkillTree > >::iterator mIter = m_mapLvSkills.begin();
    for( mIter; mIter != m_mapLvSkills.end(); ++mIter ) {
        //std::sort( mIter->second.begin(), mIter->second.end(), DescendingSortByLevelToSkillID() );
        for( int i = 0; i < static_cast< int >( mIter->second.size() ); i++ ) {
            if( !SiKGCSkillTree()->IsLearnedSkill( mIter->second[i] ) ) {
                m_vecCharSkills.push_back( mIter->second[i] );
                break;
            }

            // 마지막 레벨이고 여기까지 내려 왔다면 이미 마스터한 상태임
            if( i == static_cast< int >( mIter->second.size() ) -1 ) {
                m_vecCharSkills.push_back( mIter->second[i] );
            }
        }
    }

    std::sort( m_vecCharSkills.begin(), m_vecCharSkills.end(), SortByLevelToSkillID() );

    m_pkPopupMenuClass->ClearMenu();
    if ( g_kGlobalValue.IsTutorialMode() ) {
        if( m_cUICharType < GC_CHAR_DIO ) {
            for( int i = 0; i < NUM_JOB_LEVEL; i++ ) {
                std::vector< EGCSkillTree > vecSkillID = SiKGCSkillTree()->GetSkillsPerClass( m_cUICharType, (char)i );
                m_mapClassSkills.insert( std::make_pair( (char)i, vecSkillID ) );
                m_pkPopupMenuClass->AddMenu( i, GCFUNC::GetPromotionName( m_cUICharType, (char)i ) );
            }
        }
    } else {
        std::set< char > setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar().setPromotion;
        int iIndex = 0;        
        for( std::set< char >::iterator sit = setPromotion.begin(); sit != setPromotion.end(); ++sit ) {
            // 클래스스킬을 클래스별로 저장한다.
            std::vector< EGCSkillTree > vecSkillID = SiKGCSkillTree()->GetSkillsPerClass( m_cUICharType, *sit );
            m_mapClassSkills.insert( std::make_pair( *sit, vecSkillID ) );
            m_pkPopupMenuClass->AddMenu( iIndex, GCFUNC::GetPromotionName( m_cUICharType, *sit ) );
            iIndex++;
        }
    }

    std::map< char, std::vector< EGCSkillTree > >::iterator mit = m_mapClassSkills.find( SiKGCSkillTree()->GetCurUIPromotion() );
    if( mit != m_mapClassSkills.end() ) {
        m_vecCurClassSkills = mit->second;
    }

    m_pkStaticSkillPoint->SetText( SiKGCSkillTree()->GetExtraSP( m_cUICharType ) );
}

EGCSkillTree KGCSkillTreeMainDlg::GetUntrainSkillID( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );
    if( pSkill ) {
        std::map< EGCSkillGroup, std::vector< EGCSkillTree > >::iterator mit = m_mapLvSkills.find( pSkill->GetGroupID() );
        if( mit != m_mapLvSkills.end() ) {
            for( int i = 0; i < static_cast< int >( mit->second.size() ); i++ ) {
                if( mit->second[i] == eSkillID_ ) {
                    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( mit->second[i] );
                    // 맥스레벨이고 배운 상태면 지꺼 보내면 된다.
                    int iRealIndex;
                    if( pSkill->m_iMaxSkillLv == pSkill->m_iCurSkillLv && SiKGCSkillTree()->IsLearnedSkill( pSkill->GetID() ) ) {
                        iRealIndex = i;
                    } else {
                        iRealIndex = i - 1; //이전 스킬아이디가 필요함.
                    }                            
                    if( iRealIndex >= 0 ) {
                        return mit->second[iRealIndex];                        
                    }
                }
            }                    
        } else {
            if( SiKGCSkillTree()->IsLearnedSkill( eSkillID_ ) ) {
                return eSkillID_;
            }
        }
    }

    return (EGCSkillTree)-1;
}

bool KGCSkillTreeMainDlg::IsChangeSkillEquipInfo()
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >    mapCurEquipedSkillInfo;
    SiKGCSkillTree()->GetAllEquipedSkillInfo( mapCurEquipedSkillInfo );
    return m_mapOriAllEquipedSkillInfo != mapCurEquipedSkillInfo;
}
