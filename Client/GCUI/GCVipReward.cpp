#include "stdafx.h"
#include "GCVipReward.h"

IMPLEMENT_CLASSNAME( KGCVipReward );
IMPLEMENT_WND_FACTORY( KGCVipReward );
IMPLEMENT_WND_FACTORY_NAME( KGCVipReward, "gc_vip_reward" );

KGCVipReward::KGCVipReward( void )
: m_pkBtnClose(NULL)
, m_pkBtnConfirm(NULL)
, m_pkImgPeriodStr(NULL)
, m_pkImgLine(NULL)
, m_pkTapNewUser(NULL)
, m_pkTapVipUser(NULL)
, m_pkTapNewUserOff(NULL)
, m_pkTapVipUserOff(NULL)
, m_pkMainBackground(NULL)
, m_pkSubBackground(NULL)
, m_pkBenefitListBG(NULL)
, m_pkYellowFrame(NULL)
, m_pkVipBenefitAbout(NULL)
, m_pkMyPurchaseStr(NULL)
, m_pkMyPurchaseAmount(NULL)
, m_pkFinalRewardStr(NULL)
, m_dwMainBGHeight(0)
, m_dwYellowFrameBGHeight(0)
, m_dwSubBGHeight(0)
, m_iCurrentMode(VIP_USER_REWARD_MODE)
, m_fPeriodGapY(0.0f)
, m_fInvervalGapY(0.0f)
, m_pkFinalItemWnd(NULL)
, m_dwFinalItemID(-1)
, m_pkAboutVipUser(NULL)
, m_pkAboutNewbuyUser(NULL)
, m_pkImgBenefitStr(NULL)
, m_pkTitleVipUser(NULL)
, m_pkTitleNewbuyUser(NULL)
, m_nSizeOfPayTable(-1)
, m_pkRewardUI(NULL)
, m_pkSizingNewUserBenefitBGBar(NULL)
, m_pkbtnNewUserBenefit(NULL)       
, m_pkstaticNewUserBenefit(NULL)
, m_pkSizingNewUserBenefitBG(NULL)
{

    LINK_CONTROL( "sizing_new_user_benefit_bg_bar",         m_pkSizingNewUserBenefitBGBar  );
    LINK_CONTROL( "btn_new_user_view_benefit",          m_pkbtnNewUserBenefit                );
    LINK_CONTROL( "static_new_user_benefit",           m_pkstaticNewUserBenefit             );
    LINK_CONTROL( "sizing_new_user_benefit_bg",        m_pkSizingNewUserBenefitBG        );

    LINK_CONTROL( "reward_item_list_box",       m_pkRewardUI );
    LINK_CONTROL( "btn_close",                  m_pkBtnClose            );
    LINK_CONTROL( "btn_confirm",                m_pkBtnConfirm          );
    LINK_CONTROL( "img_line",                   m_pkImgLine          );
    LINK_CONTROL( "img_period_string",          m_pkImgPeriodStr          );

    LINK_CONTROL( "btn_new_user",                m_pkTapNewUser          );
    LINK_CONTROL( "btn_vip_user",                m_pkTapVipUser          );
    LINK_CONTROL( "btn_new_user_off",                m_pkTapNewUserOff);
    LINK_CONTROL( "btn_vip_user_off",                m_pkTapVipUserOff       );


    LINK_CONTROL( "main_background",            m_pkMainBackground          );
    LINK_CONTROL( "sizing_frame_yellow",            m_pkYellowFrame          );
    LINK_CONTROL( "sub_background",             m_pkSubBackground          );
    LINK_CONTROL( "sizing_benefit_list_bg",     m_pkBenefitListBG          );


    LINK_CONTROL( "static_vip_benefit_about",   m_pkVipBenefitAbout     );
    LINK_CONTROL( "static_my_purchase_string",  m_pkMyPurchaseStr       );
    LINK_CONTROL( "static_my_purchase_amount",  m_pkMyPurchaseAmount    );
    LINK_CONTROL( "static_final_reward_string", m_pkFinalRewardStr      );

    LINK_CONTROL( "img_final_reward_img",       m_pkFinalItemWnd      );
    LINK_CONTROL( "img_new_buy_user_about_string",      m_pkAboutNewbuyUser      );
    LINK_CONTROL( "img_vip_about_string",               m_pkAboutVipUser      );



    LINK_CONTROL( "img_benefit_string",               m_pkImgBenefitStr);

    LINK_CONTROL( "img_new_buy_user_title_string",               m_pkTitleNewbuyUser);
    LINK_CONTROL( "img_vip_title_string",               m_pkTitleVipUser);




    for( int i = 0; i < ITEM_LIST_MAX_COUNT; ++i )
    {
        m_apkIntervalBG[i] = NULL;
        m_apkIntervalStr[i] = NULL;  
        m_apkViewBenefit[i] = NULL; 

        LINK_CONTROL_STM( "sizing_benefit_background" << i , m_apkIntervalBG[i] );
        LINK_CONTROL_STM( "static_amount_to_benefit" << i , m_apkIntervalStr[i] );
        LINK_CONTROL_STM( "btn_view_benefit" << i ,         m_apkViewBenefit[i] );
    }

    for( int i = 0; i < WEEK_LIST_MAX_COUNT; ++i )
    {
        m_apkPeriodBG[i] = NULL;
        m_apkPeriodBGLock[i] = NULL;
        m_apkPeriodStrSecond[i] = NULL;
        m_apkPeriodStrFirst[i] = NULL;

        LINK_CONTROL_STM( "sizing_period_background" << i , m_apkPeriodBG[i] );
        LINK_CONTROL_STM( "sizing_period_background_lock" << i , m_apkPeriodBGLock[i] );
        LINK_CONTROL_STM( "static_period_second" << i ,     m_apkPeriodStrSecond[i] );
        LINK_CONTROL_STM( "static_period_first" << i ,      m_apkPeriodStrFirst[i] );
    }


}

KGCVipReward::~KGCVipReward( void )
{
}


void KGCVipReward::OnCreate( void )
{   
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkSizingNewUserBenefitBGBar->InitState( false, true, this ); 
    m_pkbtnNewUserBenefit->InitState( false , true, this ); 
    m_pkstaticNewUserBenefit->InitState( false );
    m_pkSizingNewUserBenefitBG->InitState( false );


    m_pkRewardUI->InitState(false, true, this);
    m_pkBtnClose->InitState( true, true, this );        
    m_pkBtnConfirm->InitState( true, true, this );  

    m_pkImgLine->InitState( true );
    m_pkImgPeriodStr->InitState( true );

    m_pkVipBenefitAbout->InitState( true ); 
    m_pkMyPurchaseStr->InitState( true );    
    m_pkMyPurchaseAmount->InitState( true ); 
    m_pkFinalRewardStr->InitState( true ); 
    m_pkMainBackground->InitState( true );
    m_pkSubBackground->InitState( true );
    m_pkBenefitListBG->InitState( true );
    m_pkYellowFrame->InitState( true );
    m_pkAboutNewbuyUser->InitState( false );
    m_pkAboutVipUser->InitState( true );
    m_pkTitleVipUser->InitState( true );
    m_pkTitleNewbuyUser->InitState( false );
    m_pkImgBenefitStr->InitState( true );
    m_pkFinalItemWnd->InitState( true );

    for( int i = 0; i < ITEM_LIST_MAX_COUNT; ++i )
    {  
        //if ( i < ITEM_LIST_DEFAULT_VIEW_COUNT ) {
        //    m_apkIntervalBG[i]->InitState(true, true, this);
        //    m_apkIntervalStr[i]->InitState(true);
        //    m_apkViewBenefit[i]->InitState(true, true, this);
        //} 
        //else {
            m_apkIntervalBG[i]->InitState(false, true, this);
            m_apkIntervalStr[i]->InitState(false);
            m_apkViewBenefit[i]->InitState(false, true, this);
        //}
    }

    for( int i = 0; i < WEEK_LIST_MAX_COUNT; ++i )
    {
        m_apkPeriodBG[i]->InitState(true);
        m_apkPeriodBGLock[i]->InitState(true);
        m_apkPeriodStrFirst[i]->InitState(true);
        m_apkPeriodStrSecond[i]->InitState(true);

        m_apkPeriodStrFirst[i]->SetFontSpace(1);
        m_apkPeriodStrFirst[i]->SetAlign(DT_CENTER);
    }


    float fFontSpace = 1.0f;
#if defined(NATION_IDN) || defined(NATION_USA) || defined( NATION_LATINAMERICA )
    fFontSpace = 0.0f;
#endif
    m_pkVipBenefitAbout->SetFontSpace(fFontSpace); 

    m_pkMyPurchaseStr->SetFontSpace(1);
#if defined ( NATION_USA )
    m_pkMyPurchaseAmount->SetFontSpace( fFontSpace );
#else
    m_pkMyPurchaseAmount->SetFontSpace(1);
#endif
    m_pkMyPurchaseAmount->SetAlign(DT_RIGHT);
    m_pkFinalRewardStr->SetFontSpace(fFontSpace);
    m_pkFinalRewardStr->SetAlign(DT_CENTER);


    m_pkMyPurchaseStr->SetFontOutline(true, D3DCOLOR_ARGB( 255, 20, 32, 46));
    m_pkFinalRewardStr->SetFontColor( D3DCOLOR_ARGB( 255, 255, 198, 0 ));

    m_pkVipBenefitAbout->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_VIP_DESC ) );  
    m_pkMyPurchaseStr->SetText(g_pkStrLoader->GetString( STR_ID_VIP_MY_PUCHASE_AMOUNT ) );   
    m_pkFinalRewardStr->SetText(g_pkStrLoader->GetString( STR_ID_VIP_FINAL_REWARD ) );   

    m_pkstaticNewUserBenefit->SetText( g_pkStrLoader->GetString( STR_ID_VIP_NEW_USER_BENEFIT ));

    //UI들 정렬
    static bool bInit = true;
    if( bInit ) {
        bInit = false;

        LoadFinalVipReward();

        m_dwMainBGHeight = m_pkMainBackground->GetHeight();
        m_dwYellowFrameBGHeight = m_pkYellowFrame->GetHeight();
        m_dwSubBGHeight = m_pkSubBackground->GetHeight();
        m_dwBenefitListBGHeight = m_pkBenefitListBG->GetHeight();
        m_vtBtnConfirmInitPos = m_pkBtnConfirm->GetFixedWindowLocalPos();
        m_vtImgLineInitPos = m_pkImgLine->GetFixedWindowLocalPos();
        m_vtImgPeriodInitPos = m_pkImgPeriodStr->GetFixedWindowLocalPos();

        //기간부분 UI정렬
        m_vtPeriodBGInitPos               = m_apkPeriodBG[0]->GetFixedWindowLocalPos();
        m_vtPeriodFirstStrOffset      = m_apkPeriodStrFirst[0]->GetFixedWindowLocalPos();
        m_vtPeriodSecondStrOffset     = m_apkPeriodStrSecond[0]->GetFixedWindowLocalPos();

        m_vtPeriodFirstStrOffset -= m_vtPeriodBGInitPos;
        m_vtPeriodSecondStrOffset -= m_vtPeriodBGInitPos;

        POINT ptPeriodBGWH;
        ptPeriodBGWH.x = m_apkPeriodBG[0]->GetWidth();
        ptPeriodBGWH.y = m_apkPeriodBG[0]->GetHeight();

        //m_fGapY = -3.0f;

        for( int i = 0 ; i < WEEK_LIST_MAX_COUNT ; ++i ) {
            D3DXVECTOR2 vtTemp( m_vtPeriodBGInitPos );
            vtTemp.y = m_vtPeriodBGInitPos.y + (m_fPeriodGapY + ptPeriodBGWH.y) * i ;

            if ( m_apkPeriodBG[i] != NULL) 
                m_apkPeriodBG[i]->SetFixedWindowLocalPos( vtTemp );
            if ( m_apkPeriodBGLock[i] != NULL )
                m_apkPeriodBGLock[i]->SetFixedWindowLocalPos( vtTemp ); 

            if ( m_apkPeriodStrFirst[i] != NULL )
                m_apkPeriodStrFirst[i]->SetFixedWindowLocalPos( vtTemp + m_vtPeriodFirstStrOffset); 

            if ( m_apkPeriodStrSecond[i] != NULL )
                m_apkPeriodStrSecond[i]->SetFixedWindowLocalPos( vtTemp + m_vtPeriodSecondStrOffset); 
        }


        //구매 금액 구간 UI정렬 //"금액별 혜택 안내"
        D3DXVECTOR2 vtInvervalBGPos      = m_apkIntervalBG[0]->GetFixedWindowLocalPos();
        D3DXVECTOR2 vtIntervalStrOffset   = m_apkIntervalStr[0]->GetFixedWindowLocalPos();
        D3DXVECTOR2 vtViewBenefitffset   = m_apkViewBenefit[0]->GetFixedWindowLocalPos();

        vtIntervalStrOffset -= vtInvervalBGPos;
        vtViewBenefitffset -= vtInvervalBGPos;

        POINT ptInvervalBGWH;
        ptInvervalBGWH.x = m_apkIntervalBG[0]->GetWidth();
        ptInvervalBGWH.y = m_apkIntervalBG[0]->GetHeight();

        for( int i = 0 ; i < ITEM_LIST_MAX_COUNT ; ++i ) {
            D3DXVECTOR2 vtTemp( vtInvervalBGPos );
            vtTemp.y = vtInvervalBGPos.y + (m_fInvervalGapY + ptInvervalBGWH.y) * i ;

            if ( m_apkIntervalBG[i] != NULL) 
                m_apkIntervalBG[i]->SetFixedWindowLocalPos( vtTemp );

            if ( m_apkIntervalStr[i] != NULL )
                m_apkIntervalStr[i]->SetFixedWindowLocalPos( vtTemp + vtIntervalStrOffset); 

            if ( m_apkViewBenefit[i] != NULL )
                m_apkViewBenefit[i]->SetFixedWindowLocalPos( vtTemp + vtViewBenefitffset); 
        }
    }

#if defined (USE_VIP_NEW_USER_TAB)
    m_pkTapNewUser->InitState(false, false);
    m_pkTapNewUserOff->InitState(true, true, this);
    
    m_pkTapVipUser->InitState(true, false);
    m_pkTapVipUserOff->InitState(false, true, this);
#else
    m_pkTapNewUser->InitState(false, false, NULL);
    m_pkTapNewUserOff->InitState(true, true, NULL);

    D3DXVECTOR2 vecPosBtn = m_pkTapNewUser->GetFixedWindowLocalPos();
    D3DXVECTOR2 vecPosOffBtn = m_pkTapNewUserOff->GetFixedWindowLocalPos();
    m_pkTapVipUser->InitState(true, false);
    m_pkTapVipUser->SetFixedWindowLocalPos( vecPosBtn );

    m_pkTapVipUserOff->InitState(false, true, this);
    m_pkTapVipUser->SetFixedWindowLocalPos( vecPosOffBtn );
#endif

    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVipReward::OnClose );
    m_pkBtnConfirm->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVipReward::OnClose );
#if defined (USE_VIP_NEW_USER_TAB)
    m_pkTapNewUserOff->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVipReward::ViewNewUserRewardUI);
#endif
    m_pkTapVipUserOff->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVipReward::ViewVipUserRewardUI);
    m_pkbtnNewUserBenefit->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVipReward::ViewNewUserBenefit );
}


void KGCVipReward::VipEventUserInfoReq()
{
    std::map<USHORT,KVipEvent>::iterator itor = m_vipEventList.m_mapEventByVer.begin();
    if(m_vipEventList.m_mapEventByVer.empty()) return;

    for( ; itor != m_vipEventList.m_mapEventByVer.end() ; ++itor ) 
    {
        CTime startTime(itor->second.m_prEventRange.first+ g_kGlobalValue.m_tmServerSyncTime);
        CTime endTime(itor->second.m_prEventRange.second+ g_kGlobalValue.m_tmServerSyncTime);
        time_t curTime = KncUtil::TimeToInt(g_kGlobalValue.m_tmServerTime) + g_kGlobalValue.m_tmServerSyncTime;

        if ( startTime < g_kGlobalValue.m_tmServerTime && endTime > g_kGlobalValue.m_tmServerTime )
        {
            KP2P::GetInstance()->Send_VipEventUserInfoReq(itor->first);
            break;
        }
    }
}


void KGCVipReward::OnCreateComplete( void )
{
    KP2P::GetInstance()->Send_VipEventListReq();
}

void KGCVipReward::ActionPerformed( const KActionEvent& event )
{
    if ( m_pkRewardUI->IsRenderOn() )
        return;

    if (  event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        for( int i = 0; i < ITEM_LIST_MAX_COUNT; ++i )
        {
            if ( event.m_pWnd == m_apkIntervalBG[i] || event.m_pWnd == m_apkViewBenefit[i] ) {
                OnClickInterval( i );
            }
        }
    }
}


void KGCVipReward::ViewNewUserBenefit( )
{

    m_pkRewardUI->ToggleRender( true );
    m_pkRewardUI->ClearItemList();
    std::map<USHORT,std::vector<KDropItemInfo> >::iterator itor = m_vipEventList.m_mapNewbieRewardItem.begin();
    for ( ; itor != m_vipEventList.m_mapNewbieRewardItem.end() ; ++itor ) {
        std::vector<KDropItemInfo>::iterator itemItor = itor->second.begin();
        for ( ; itemItor != itor->second.end() ; ++itemItor ) {
            m_pkRewardUI->AddItem( itemItor->m_ItemID / 10, itemItor->m_nDuration );
        }
    }
    m_pkRewardUI->RefreshPage();

    m_pkRewardUI->m_pkImgTitleVipUser->ToggleRender( false);
    m_pkRewardUI->m_pkImgTitleNewBuyUser->ToggleRender( true );

    SetNewUserGuideMent();
}

void KGCVipReward::OnClickInterval( int i )
{
    int nRealIndex = i + 1;


    std::map<USHORT,std::vector<KDropItemInfo> >::iterator itorVipReward = m_vipEventList.m_mapVipRewardItem.find( nRealIndex );
    if ( itorVipReward == m_vipEventList.m_mapVipRewardItem.end() )
        return;


    m_pkRewardUI->ToggleRender( true );
    m_pkRewardUI->ClearItemList();
    std::vector<KDropItemInfo>::iterator itorItemInfo = itorVipReward->second.begin();
    for ( ; itorItemInfo != itorVipReward->second.end() ; ++itorItemInfo ) {   
        m_pkRewardUI->AddItem( itorItemInfo->m_ItemID / 10, itorItemInfo->m_nDuration );
    }
    m_pkRewardUI->RefreshPage();

    std::map<USHORT,std::pair<int,int> >::iterator itorPayTable = m_vipEventList.m_mapPayTable.find( nRealIndex ); 

    if ( itorPayTable->second.first > m_vipUserInfo.m_nPoint ) {
        m_pkRewardUI->m_pkStaticGuide->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_VIP_VIP_TARGET) );
    }
    else {
        m_pkRewardUI->m_pkStaticGuide->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_VIP_BUY_COMPLETE) );
    }

    m_pkRewardUI->RefreshPage();

    m_pkRewardUI->m_pkImgTitleVipUser->ToggleRender( true);
    m_pkRewardUI->m_pkImgTitleNewBuyUser->ToggleRender( false );

}

void KGCVipReward::ViewNewUserRewardUI()
{
    //if ( m_iCurrentMode != NEW_USER_REWARD_MODE )
    {
        m_iCurrentMode = NEW_USER_REWARD_MODE;
        ViewToggleVipRewardUI( false );
        ViewToggleNewBuyUserRewardUI( true );
        ResizeUIBecausePayTable( 10 );
        SetEventPeriod(m_vipEventList.m_mapEventByVer, NEW_USER_REWARD_MODE );
        m_pkVipBenefitAbout->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_VIP_NEWBUY_BENEFIT ) );
    }
}

void KGCVipReward::ViewVipUserRewardUI()
{    
    //if ( m_iCurrentMode != VIP_USER_REWARD_MODE )
    {
        m_iCurrentMode = VIP_USER_REWARD_MODE;
        ViewToggleVipRewardUI( true );
        ViewToggleNewBuyUserRewardUI( false );
        ResizeUIBecausePayTable( m_nSizeOfPayTable );
        SetEventPeriod(m_vipEventList.m_mapEventByVer, VIP_USER_REWARD_MODE );
        m_pkVipBenefitAbout->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_VIP_VIP_BENEFIT ) );
    }
}

void KGCVipReward::ViewToggleVipRewardUI( bool _view )
{
    m_pkTapVipUserOff->ToggleRender(!_view);
    m_pkTapVipUser->ToggleRender(_view);

    m_pkAboutVipUser->ToggleRender( _view );
    m_pkImgBenefitStr->ToggleRender( _view );

    m_pkBenefitListBG->ToggleRender( _view );
    m_pkTitleVipUser->ToggleRender( _view );

    for( int i = 0; i < m_nSizeOfPayTable; ++i )
    {
        m_apkIntervalBG[i]->ToggleRender( _view );
        m_apkIntervalStr[i]->ToggleRender( _view );
        m_apkViewBenefit[i]->ToggleRender( _view );
    }
}

void KGCVipReward::ViewToggleNewBuyUserRewardUI( bool _view )
{
    m_pkTapNewUserOff->ToggleRender(!_view);
    m_pkTapNewUser->ToggleRender(_view);

    m_pkAboutNewbuyUser->ToggleRender( _view );
    m_pkTitleNewbuyUser->ToggleRender( _view );

    m_pkSizingNewUserBenefitBGBar->ToggleRender( _view );
    m_pkbtnNewUserBenefit->ToggleRender( _view );
    m_pkstaticNewUserBenefit->ToggleRender( _view );
    m_pkSizingNewUserBenefitBG->ToggleRender( _view );
}

void KGCVipReward::OnClose()
{
    Destroy();
}


//On_EVENT_VIPEVENT_LIST_ACK
void KGCVipReward::SetVipEventList(KEVENT_VIPEVENT_LIST_ACK& receiveData )
{
    // Vip이벤트 정보. 가공해서 쓰세요.
    // kRecv.m_mapEventByVer;        // 버전별 이벤트정보 [Ver, 이벤트정보]
    // kRecv.m_mapPayTable;          // 구매금액 테이블 [금액ID, [min이상~max이하] ]
    // kRecv.m_mapRewardByVer;       // 버전별 보상ID [Ver, [금액ID,보상ID] ]
    // kRecv.m_mapNewbieRewardItem;  // 신규 보상그룹 [보상ID,아이템리스트]
    // kRecv.m_mapVipRewardItem;     // VIP 보상그룹 [보상ID,아이템리스트]

    //이벤트정보
    //USHORT                      m_usVersion;    // 이벤트 버전
    //std::pair<time_t,time_t>    m_prEventRange;     // 이벤트 기간(시작날짜~끝날짜)
    //std::map<int,bool>          m_mapEventEnable;   // 이벤트 발동여부(EventType,Enable)


    m_vipEventList = receiveData;
    VipEventUserInfoReq();

    SetEventPeriod(m_vipEventList.m_mapEventByVer, VIP_USER_REWARD_MODE );
    SetPayTable(m_vipEventList.m_mapPayTable);
}

void KGCVipReward::SetNewUserGuideMent()
{
    std::map<int,bool>::iterator itor = m_vipUserInfo.m_mapReward.find( 0 );    //신규유저 정보가 있는지 찾는다.  
    if ( itor != m_vipUserInfo.m_mapReward.end() ) {    //신규유저보상 아이템을 획득했다.
        if ( itor->second )
            m_pkRewardUI->SetDesc( g_pkStrLoader->GetString(STR_ID_VIP_BUY_COMPLETE) );
        else {
            if ( m_vipEventList.m_mapEventByVer[m_vipUserInfo.m_usVersion].m_prEventRange.first < g_kGlobalValue.m_tFirstLoginTime  &&
                g_kGlobalValue.m_tFirstLoginTime < m_vipEventList.m_mapEventByVer[m_vipUserInfo.m_usVersion].m_prEventRange.second  ) {
                    m_pkRewardUI->SetDesc( g_pkStrLoader->GetString(STR_ID_VIP_NEWBUY_TARGET) );
            }
            else
            {
                m_pkRewardUI->SetDesc( g_pkStrLoader->GetString(STR_ID_VIP_NOT_TARGET_NEWBUY) ); 
            }
        }
    }
    else {
        m_pkRewardUI->SetDesc( g_pkStrLoader->GetString(STR_ID_VIP_VIP_TARGET) );
    }
}

//On_EVENT_VIPEVENT_USER_INFO_ACK
void KGCVipReward::SetVipEventUserInfo(KEVENT_VIPEVENT_USER_INFO_ACK& receiveData )
{
    // 버전별 VIP이벤트 유저 달성정보
    // kRecv.m_usVersion; // VIP이벤트 버전
    // kRecv.m_nPoint; // 사용한 캐쉬금액
    // kRecv.m_mapReward; // 획득한 보상 : key값 0은 신규, 1이상은 VIP구매급액ID.
    m_vipUserInfo = receiveData;

    std::wstringstream tempPoint;
    tempPoint << m_vipUserInfo.m_nPoint;

    m_pkMyPurchaseAmount->SetText(tempPoint.str());

    SetNewUserGuideMent();

    SetEventPeriod(m_vipEventList.m_mapEventByVer, VIP_USER_REWARD_MODE );
    SetPayTable(m_vipEventList.m_mapPayTable);
}

//On_EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT
void KGCVipReward::SetVipEventUserRewardUpdate(KEVENT_VIPEVENT_USER_REWARD_UPDATE_NOT& receiveData )
{
    // kRecv : VIP이벤트로 받은 보상아이템 리스트std::vector<KItem>
    m_vipUserRewardInfo = receiveData;
    
    if( m_vipUserRewardInfo.empty() == false ) { 
        VipEventUserInfoReq();
    }

    m_pkstaticNewUserBenefit->SetFontColor( D3DCOLOR_ARGB( 255, 255, 197, 0 ));
    m_pkRewardUI->ToggleRender( true );
    m_pkRewardUI->ClearItemList();
    m_pkRewardUI->m_pkStaticGuide->SetText( g_pkStrLoader->GetString(STR_ID_VIP_BUY_COMPLETE) );
    for ( std::vector<KItem>::iterator itor = receiveData.begin(); itor != receiveData.end() ; ++itor ) {
        m_pkRewardUI->AddItem(itor->m_ItemID / 10, itor->m_nCount );
        g_pItemMgr->m_kInventory.AddItem( *itor, true );
    }

    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateData();

    m_pkRewardUI->m_pkImgTitleVipUser->ToggleRender( false );
    m_pkRewardUI->m_pkImgTitleNewBuyUser->ToggleRender( false ); 

    m_pkRewardUI->RefreshPage();
}

void KGCVipReward::SetEventPeriod(std::map<USHORT,KVipEvent>& _mapEvent, int _nEvnetType )
{
    int nSizeOfperiod = _mapEvent.size();
    std::map<USHORT,KVipEvent>::iterator cmit = _mapEvent.begin();


    for ( int i = 0 ; i < WEEK_LIST_MAX_COUNT ; i++ ) {
        if ( cmit != _mapEvent.end() ) {

            //이벤트 타입에 따라 보이게 한다.
            if (cmit->second.IsEventEnable(_nEvnetType) ){
                CTime startTime(cmit->second.m_prEventRange.first + g_kGlobalValue.m_tmServerSyncTime);
                CTime endTime(cmit->second.m_prEventRange.second + g_kGlobalValue.m_tmServerSyncTime);            

#if defined( NATION_KOREA ) || defined( NATION_USA )
                m_apkPeriodStrFirst[i]->SetText( g_pkStrLoader->GetString( STR_ID_DATE ) );
#else
                m_apkPeriodStrFirst[i]->SetText(g_pkStrLoader->GetReplacedString( STR_ID_VIP_NTH_WEEK, "i", i + 1) );
#endif

                wstring wstrStartMinute;
                wstring wstrEndMinute;
                wchar_t temp[100];

                if ( startTime.GetMinute() == 0 ) {  
                    wstrStartMinute = L"00";
                }
                else {
                    wstrStartMinute = _itow(startTime.GetMinute(), temp, 10);
                }


                if ( endTime.GetMinute() == 0 ) {  
                    wstrEndMinute = L"00";
                }
                else {
                    wstrEndMinute = _itow(endTime.GetMinute(), temp, 10);
                }

                m_apkPeriodStrSecond[i]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_VIP_PERIOD, "iiiisiiis", startTime.GetYear(), startTime.GetMonth(), startTime.GetDay(), startTime.GetHour(), wstrStartMinute.c_str(), endTime.GetMonth(), endTime.GetDay(), endTime.GetHour(), wstrEndMinute.c_str() ));
                //m_apkPeriodStrSecond[i]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_VIP_PERIOD, "iiiiiiiii", startTime.GetYear(), startTime.GetMonth(), startTime.GetDay(), startTime.GetHour(), startTime.GetMinute(), endTime.GetMonth(), endTime.GetDay(), endTime.GetHour(), endTime.GetMinute() ));



                //아직 이벤트 시작 전이라면 4주차 최종 보상을 보여준다.
                std::map<int, std::pair<GCITEMID, int > >::reverse_iterator ritor = m_mapVipFinalReward.rbegin( );
                if ( ritor != m_mapVipFinalReward.rend() ) {
                    m_dwFinalItemID = ritor->second.first;
                    m_pkFinalItemWnd->SetImage( ritor->second.first );
                    m_pkFinalItemWnd->SetCount( ritor->second.second );                    
                }


                //현재 주차는 static의 색을 바꾼다.   255,198,0
                //앞으로 일어날 이벤트는 Lock이미지를 true시킨다.
                //m_mapVipFinalReward   //현재가 몇주차인지를 찾아서 최종보상을 보여준다.
                enum { 
                    INIT_EVENT      = -1,
                    BEFORE_EVENT    = 0,
                    DOING_EVENT     = 1,
                    END_EVENT       = 2,
                    ERROR_STATE     = 3,
                };
                int nCurState = INIT_EVENT;
                CTime currentTime = g_kGlobalValue.m_tmServerTime;// - CTime(g_kGlobalValue.m_tmServerSyncTime);
                if ( currentTime < startTime)  {    
                    nCurState = BEFORE_EVENT;    
                } else if ( startTime <= currentTime && currentTime <= endTime ) {
                    nCurState = DOING_EVENT;
                } else if ( endTime < currentTime ) {
                    nCurState = END_EVENT;
                }

                switch ( nCurState ) 
                {
                case END_EVENT:  //시작 전 구간         //텍스트 흰색   배경 액티브
                    m_apkPeriodStrFirst[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));
                    m_apkPeriodStrSecond[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));
                    m_apkPeriodBG[i]->ToggleRender( true );
                    m_apkPeriodBGLock[i]->ToggleRender( false );
                    break;
                case DOING_EVENT:   //이벤트 중 구간        //노란색 액티브
                    m_apkPeriodStrFirst[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 197, 0 ));
                    m_apkPeriodStrSecond[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 197, 0 ));
                    m_apkPeriodBG[i]->ToggleRender( true );
                    m_apkPeriodBGLock[i]->ToggleRender( false );

                    {
                        //최종 보상 설정
                        std::map<int, std::pair<GCITEMID, int > >::iterator itor = m_mapVipFinalReward.find( i );
                        if ( itor != m_mapVipFinalReward.end() ) {
                            m_dwFinalItemID = itor->second.first;
                            m_pkFinalItemWnd->SetImage( itor->second.first );
                            m_pkFinalItemWnd->SetCount( itor->second.second );                    
                        }
                    }

                    break;

                case BEFORE_EVENT:     //이벤트 종료 구간      //회색   인액티브  
                    m_apkPeriodStrFirst[i]->SetFontColor( D3DCOLOR_ARGB( 255, 168, 138, 133 ));
                    m_apkPeriodStrSecond[i]->SetFontColor( D3DCOLOR_ARGB( 255, 168, 138, 133 ));
                    m_apkPeriodBG[i]->ToggleRender( false );
                    m_apkPeriodBGLock[i]->ToggleRender( true );
                    break;
                }

                m_apkPeriodBG[i]->ToggleRender( true );
                m_apkPeriodBGLock[i]->ToggleRender( false );                
            }
            else {
                m_apkPeriodBG[i]->ToggleRender( true );
                m_apkPeriodBGLock[i]->ToggleRender( false );
            }

            ++cmit;
        }

        //이벤트 기간이 있는 것들만 보여지게 한다. 
        if ( i >= nSizeOfperiod ) {
            m_apkPeriodBG[i]->ToggleRender( false );
            m_apkPeriodBGLock[i]->ToggleRender( false );
            m_apkPeriodStrFirst[i]->ToggleRender( false );
            m_apkPeriodStrSecond[i]->ToggleRender( false );
        }
    }
}

void KGCVipReward::ResizeUIBecausePayTable(int _payTableCount )
{

    int nAddLine = _payTableCount - ITEM_LIST_DEFAULT_VIEW_COUNT;
    if ( nAddLine < 0 )        
        nAddLine = 0;

    float fPeriodCount = static_cast<float>(m_vipEventList.m_mapEventByVer.size());
    float fPeriodBGHeight = static_cast<float>(m_apkPeriodBG[0]->GetHeight());
    DWORD ForWeekHeight = static_cast<DWORD>( 4.0f * fPeriodBGHeight );
    DWORD dwMinusPeriodHeight = static_cast<DWORD>( ForWeekHeight - (fPeriodCount * fPeriodBGHeight) );


    DWORD dwAddHeight = static_cast<DWORD>((m_fInvervalGapY + static_cast<float>(m_apkIntervalBG[0]->GetHeight()) ) * nAddLine);
    m_pkMainBackground->SetHeight( m_dwMainBGHeight + dwAddHeight - dwMinusPeriodHeight );
    m_pkSubBackground->SetHeight( m_dwSubBGHeight + dwAddHeight );
    m_pkBenefitListBG->SetHeight( m_dwBenefitListBGHeight + dwAddHeight );
    m_pkYellowFrame->SetHeight( m_dwYellowFrameBGHeight + dwAddHeight );

    m_pkBtnConfirm->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtBtnConfirmInitPos.x, m_vtBtnConfirmInitPos.y + static_cast<float>(dwAddHeight) - static_cast<float>(dwMinusPeriodHeight)) );
    m_pkImgLine->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtImgLineInitPos.x, m_vtImgLineInitPos.y + static_cast<float>(dwAddHeight)) );
    m_pkImgPeriodStr->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtImgPeriodInitPos.x, m_vtImgPeriodInitPos.y + static_cast<float>(dwAddHeight)) );

    const float fPeriodHeight = static_cast<float>(m_apkPeriodBG[0]->GetHeight());

    for( int i = 0; i < WEEK_LIST_MAX_COUNT; ++i )
    {
        m_apkPeriodBG[i]->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtPeriodBGInitPos.x, 
            m_vtPeriodBGInitPos.y + static_cast<float>(dwAddHeight) + (m_fPeriodGapY + fPeriodHeight) * i) );
        m_apkPeriodBGLock[i]->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtPeriodBGInitPos.x, 
            m_vtPeriodBGInitPos.y + static_cast<float>(dwAddHeight) + (m_fPeriodGapY + fPeriodHeight) * i) );
        m_apkPeriodStrFirst[i]->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtPeriodBGInitPos.x + m_vtPeriodFirstStrOffset.x,
            m_vtPeriodBGInitPos.y + static_cast<float>(dwAddHeight) + m_vtPeriodFirstStrOffset.y + (m_fPeriodGapY + fPeriodHeight) * i) );
        m_apkPeriodStrSecond[i]->SetFixedWindowLocalPos( D3DXVECTOR2(m_vtPeriodBGInitPos.x + m_vtPeriodSecondStrOffset.x, 
            m_vtPeriodBGInitPos.y + static_cast<float>(dwAddHeight) + m_vtPeriodSecondStrOffset.y+ (m_fPeriodGapY + fPeriodHeight) * i) );
    }
}

void KGCVipReward::SetPayTable(std::map<USHORT,std::pair<int,int> >& _mapPayTable )
{

    m_nSizeOfPayTable = static_cast<int>(_mapPayTable.size());

    std::map<USHORT,std::pair<int,int> >::iterator itor = _mapPayTable.begin();
    for (int i = 0 ; itor != _mapPayTable.end() && i < ITEM_LIST_MAX_COUNT ; ++itor, i++ ) {
        if ( i < m_nSizeOfPayTable ) {

            std::wstring wStringTemp;
            wStringTemp += g_pkStrLoader->GetReplacedString( STR_ID_VIP_USED_MONEY, "i", itor->second.first );
            wStringTemp += L" ~ ";
            if ( itor->second.second < 100000000 ) 
                wStringTemp += g_pkStrLoader->GetReplacedString( STR_ID_VIP_USED_MONEY, "i", itor->second.second );
            m_apkIntervalStr[i]->SetText( wStringTemp );


            if ( itor->second.first <= m_vipUserInfo.m_nPoint ) {
                m_apkIntervalStr[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 198, 0 ));
            }
            else {
                m_apkIntervalStr[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));
            }

            m_apkIntervalStr[i]->ToggleRender(true);
            m_apkViewBenefit[i]->ToggleRender(true);
            m_apkIntervalBG[i]->ToggleRender(true);
        }
        else {
            m_apkIntervalStr[i]->ToggleRender(false);
            m_apkViewBenefit[i]->ToggleRender(false);
            m_apkIntervalBG[i]->ToggleRender(false);
        }
    }

    ResizeUIBecausePayTable( m_nSizeOfPayTable );

}

void KGCVipReward::FrameMoveInEnabledState( void )
{
    if ( m_pkRewardUI->IsRenderOn() )
        return;

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    for( int i = 0; i < ITEM_LIST_MAX_COUNT; ++i )
    {  
        //위에 올라왔는지 체크 해서 올라왔으면 버튼의 상태를 오버로 바꿔라.
        if ( m_apkIntervalBG[i]->CheckPosInWindowBound( vMousePos ) )
            m_apkViewBenefit[i]->SetWndMode(D3DWM_HOVER);
        else
            m_apkViewBenefit[i]->SetWndMode(D3DWM_DEFAULT);
    }



    if ( m_pkSizingNewUserBenefitBG->IsRenderOn() == false ) {
        if ( m_pkFinalItemWnd->CheckPosInWindowBound( vMousePos ) ) {
            g_pkUIScene->m_pkItemInformationBox->DrawItemInfoBox(m_dwFinalItemID, vMousePos);
        }
        else {
            g_pkUIScene->m_pkItemInformationBox->DrawItemInfoBox( -1, vMousePos );           
        }
    }
}


void KGCVipReward::LoadFinalVipReward( )
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "VipFinalReward.lua" ) == false )
    {
        MessageBoxA( NULL, "VipFinalReward.lua Load Failed!", "", MB_OK );
        return;
    }

    m_mapVipFinalReward.clear();

    luaMgr.BeginTable( "VIP_FINAL_REWARD" );
    {
        int iItemIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iItemIndex ) ) ) break;
            {
                int nWeek = -1;
                DWORD dwGoodsID = -1;
                int nDuration = -1;

                luaMgr.GetValue( 1, nWeek );
                luaMgr.GetValue( 2, dwGoodsID );
                luaMgr.GetValue( 3, nDuration );

                m_mapVipFinalReward[nWeek] = make_pair(dwGoodsID, nDuration );
                luaMgr.EndTable();
            }
            ++iItemIndex;
        }
    }
    luaMgr.EndTable();

}