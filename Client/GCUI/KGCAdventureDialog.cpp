#include "stdafx.h"
#include "KGCAdventureReward.h"
#include "KGCAdventureDialog.h"


IMPLEMENT_CLASSNAME( KGCAdventureDialog );
IMPLEMENT_WND_FACTORY( KGCAdventureDialog );
IMPLEMENT_WND_FACTORY_NAME( KGCAdventureDialog, "event_gc_adventure_dlg" );

KGCAdventureDialog::KGCAdventureDialog( void )
:m_pkBtnClose( NULL )
, m_apkAdventureReward(NULL)
, m_pkWholeContinentBtn(NULL)
, m_pkWholeContinentName(NULL)
, m_pkStaticMent(NULL)
{
    LINK_CONTROL("btn_close",m_pkBtnClose);
    LINK_CONTROL("reward",m_apkAdventureReward);
    LINK_CONTROL( "whole_continent_btn", m_pkWholeContinentBtn );
    LINK_CONTROL( "whole_continent_name", m_pkWholeContinentName );
    LINK_CONTROL( "static_ment", m_pkStaticMent);

    for( int i =0; i < GCAdventureManager::CONTINENT_MAX; i++){
        char strTemp[ MAX_PATH ] = "";
        m_saContinentBtn[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "continent_btn%d", i);
        LINK_CONTROL(strTemp,m_saContinentBtn[i]);

        m_saContinentName[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "continent_name%d", i);
        LINK_CONTROL(strTemp,m_saContinentName[i]);

        m_saContinentActBtn[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "continent_active_btn%d", i);
        LINK_CONTROL(strTemp,m_saContinentActBtn[i]);
    }
    m_vecContinentReward.clear();

}

KGCAdventureDialog::~KGCAdventureDialog( void )
{
}

void KGCAdventureDialog::OnCreate( void )
{
    m_pkBtnClose->SetHotKey(DIK_ESCAPE);
    m_pkBtnClose->InitState( true, true, this);
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCAdventureDialog::OnClickClose );
    m_apkAdventureReward->InitState( false, true, this);
    m_pkWholeContinentName->InitState( false, true, this);
    
    m_pkStaticMent->InitState( true, false, this );
    m_pkStaticMent->SetText(L"");
    m_pkStaticMent->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));
    m_pkWholeContinentBtn->InitState( true, true, this);
    m_pkWholeContinentBtn->DisconnectAllEventProcedure();
    m_pkWholeContinentBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCAdventureDialog::OnClickWholeContinent );

    //대륙 이름들과 버튼 초기화
    InitContinentBtnAndName();
}

void KGCAdventureDialog::OnCreateComplete( void )
{
    SelectContinent( GCAdventureManager::CONTINENT_BERMESIAH );
    SiGCAdventureManager()->SetCurrentContinent( GCAdventureManager::CONTINENT_BERMESIAH );
    m_pkStaticMent->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_GC_ADVENTURE_BALOON_DESC ) );
}

void KGCAdventureDialog::ActionPerformed( const KActionEvent& event )
{

}

void KGCAdventureDialog::OnClickClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAdventureDialog::InitContinentBtnAndName( void )
{
    for(int i =0;i<GCAdventureManager::CONTINENT_MAX;i++){
        m_saContinentBtn[i]->InitState(true,true,this);
        m_saContinentBtn[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCAdventureDialog::OnSelectContinentBtnList );
        m_saContinentActBtn[i]->InitState(false,true,this);
        m_saContinentName[i]->InitState(false,true,this);
    }
}

void KGCAdventureDialog::OnSelectContinentBtnList( const KActionEvent& event )
{
    g_KDSound.Play( "31" );
    SiGCAdventureManager()->SetShowItemInfo( true );
    //UI에서 해당 대륙 버튼을 클릭시 Render 꺼줄 것들과 현재 대륙 설정
    for(int i =0;i<GCAdventureManager::CONTINENT_MAX;i++)
    {
        if( m_saContinentBtn[i] == event.m_pWnd )
        {
            SiGCAdventureManager()->SetCurrentContinent( i );
            m_apkAdventureReward->WholeContinentRender( false );
            m_apkAdventureReward->OffCheckContinentImage();
            m_pkWholeContinentName->ToggleRender( false );
            SelectContinent(i);
            break;
        }
    }
}

void KGCAdventureDialog::SelectContinent( int iIndex )
{
    m_vecContinentReward.clear();

    SiGCAdventureManager()->SetCurrentContinent( iIndex );
    //선택된 대륙에 맞는 보상들을 세팅 하고 보여주는 역활을 하는곳
    for (int i =0; i < GCAdventureManager::CONTINENT_MAX; i++)
    {
        if ( i == iIndex )
        {
            m_saContinentName[i]->ToggleRender( true );
            m_apkAdventureReward->SetContinentInfo( iIndex ); //선택된 대륙 전리품 정보 설정
            
            if ( true == SiGCAdventureManager()->GetContinentRewardInfo( iIndex , m_vecContinentReward ) )
            {
                m_apkAdventureReward->SetRewardItemBoxInfo( m_vecContinentReward ); //선택된 대륙 보상 정보 설정
            }
            else
            {
                continue;
            }

            m_apkAdventureReward->CheckContinentRewardBtn(iIndex); //보상을 획득 할 수 있는지 체크 하기 
            m_apkAdventureReward->ToggleRender( true );
            m_saContinentActBtn[i]->ToggleRender( true );
        }
        else
        {
            m_saContinentName[i]->ToggleRender( false );
            m_saContinentActBtn[i]->ToggleRender( false );
            m_apkAdventureReward->CheckContinentRewardBtn(iIndex);
        }
    }

}

void KGCAdventureDialog::OnClickWholeContinent()
{
    g_KDSound.Play( "31" );
    SiGCAdventureManager()->SetShowItemInfo( false );
    SiGCAdventureManager()->SetCurrentContinent( WHOLE_CONTINENT );
    m_apkAdventureReward->WholeContinentRender( true );
    m_apkAdventureReward->CheckContinentReward();
    m_apkAdventureReward->SetFinalRewardItemBoxInfo();
    m_apkAdventureReward->CheckContinentRewardBtn( WHOLE_CONTINENT );
    
    m_pkWholeContinentName->ToggleRender( true );

    for (int i =0; i < GCAdventureManager::CONTINENT_MAX; i++)
    {
        m_saContinentName[i]->ToggleRender( false );
        m_saContinentActBtn[i]->ToggleRender( false );
    }
}

void KGCAdventureDialog::LockRewardBtn()
{
    m_apkAdventureReward->RewardBtnWndModeLock();
}