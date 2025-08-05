#include "stdafx.h"
#include "GCOlympicGawibawiboGameLose.h"

IMPLEMENT_CLASSNAME( KGCOlympicGawibawiboGameLose );
IMPLEMENT_WND_FACTORY( KGCOlympicGawibawiboGameLose );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicGawibawiboGameLose, "gc_gawibawibo_game_lose_info_box_olympic" );

KGCOlympicGawibawiboGameLose::KGCOlympicGawibawiboGameLose( void )
:	m_pkBtnOK(NULL)
,	m_pkBtnRetry(NULL)
,	m_pkStaticTitle1(NULL)
,	m_pkStaticTitle2(NULL)
,	m_pkStaticClear(NULL)
,	m_pkStaticRetry(NULL)
,	m_iCoinCount(0)
,	m_pkBtnCheck(NULL)
,	m_iResult(0)
{
    LINK_CONTROL("btn_ok",m_pkBtnOK);
    LINK_CONTROL("btn_retry",m_pkBtnRetry);
    LINK_CONTROL("btn_check",m_pkBtnCheck);
    LINK_CONTROL("static_title1",m_pkStaticTitle1);
    LINK_CONTROL("static_title2",m_pkStaticTitle2);
    LINK_CONTROL("static_clear",m_pkStaticClear);
    LINK_CONTROL("static_rechallenge",m_pkStaticRetry);
}

KGCOlympicGawibawiboGameLose::~KGCOlympicGawibawiboGameLose( void )
{
}

void KGCOlympicGawibawiboGameLose::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkBtnRetry )
        {
            if(m_iCoinCount > 0){
                m_ActionClickFunc();
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_ERR_9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }
        }
        else if( event.m_pWnd == m_pkBtnOK )
        {
            m_ActionClickFunc();
        }
        else if(event.m_pWnd == m_pkBtnCheck)
        {
            m_ActionClickFunc();
        }
    }


}

void KGCOlympicGawibawiboGameLose::OnCreate( void )
{
    m_pkBtnOK->InitState(true,true,this);

#if defined( NATION_KOREA )
    m_pkBtnRetry->InitState(false);
#else
    m_pkBtnRetry->InitState(true,true,this);
#endif

    m_pkBtnCheck->InitState(true,true,this);

    m_pkStaticTitle1->InitState(true,false,this);
    m_pkStaticTitle2->InitState(true,false,this);
    m_pkStaticClear->InitState(true,false,this);
    m_pkStaticRetry->InitState(true,false,this);

    SetResultInfo();
}

void KGCOlympicGawibawiboGameLose::SetResultInfo()
{

    m_pkStaticTitle1->SetAlign(DT_CENTER);
    m_pkStaticTitle2->SetAlign(DT_CENTER);

    // 각 상황에 맞는 버튼을 보여 줍니당~!

    if(m_iResult == ER_LOSE)
    {
#if defined( NATION_KOREA )
        m_pkStaticTitle1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_1 ));
        m_pkStaticTitle2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_2 ));
        m_pkStaticClear->SetAlign( DT_CENTER );
        m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_GAWIBAWIBO_RETRY ));
        m_pkStaticRetry->SetText(L"");    
#else
        m_pkStaticTitle1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_1 ));
        m_pkStaticTitle2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_2 ));
        m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_3 ));
        m_pkStaticRetry->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_4 ));    
#endif

        m_pkBtnOK->ToggleRender(true);

#if defined( NATION_KOREA )
        m_pkBtnRetry->ToggleRender(false);
#else
        m_pkBtnRetry->ToggleRender(true);
#endif
        m_pkBtnCheck->ToggleRender(false);
    }
    else if(m_iResult == ER_WIN)
    {
        m_pkStaticTitle1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_WIN_1 ));
        m_pkStaticTitle2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_WIN_2 ));

#if defined( NATION_KOREA )
        m_pkStaticClear->SetAlign( DT_CENTER );
        m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_GAWIBAWIBO_WIN ));
        m_pkStaticRetry->SetText(L"");    
#else
        m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_WIN_3 ));
        m_pkStaticRetry->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_WIN_4 ));    
#endif

        m_pkBtnOK->ToggleRender(false);
        m_pkBtnRetry->ToggleRender(false);
        m_pkBtnCheck->ToggleRender(true);

    }
    else
    {
        m_pkStaticTitle1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_DRAW_1 ));
        m_pkStaticTitle2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_DRAW_2 ));
        m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_DRAW_3 ));
        m_pkStaticRetry->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_DRAW_4 ));   
    

        m_pkBtnOK->ToggleRender(false);
        m_pkBtnRetry->ToggleRender(false);
        m_pkBtnCheck->ToggleRender(true);
    }

}

void KGCOlympicGawibawiboGameLose::SetRetryCoin(int iCoin_)
{
    m_iCoinCount = iCoin_;
}
