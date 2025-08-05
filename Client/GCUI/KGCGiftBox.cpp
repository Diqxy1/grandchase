#include "stdafx.h"
#include "../MyD3D.h"
#include "KGCGiftBox.h"
#include "KGCGiftBoxState.h"
#include "KGCGiftBoxManager.h"

IMPLEMENT_CLASSNAME( KGCGiftBox );
IMPLEMENT_WND_FACTORY( KGCGiftBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGiftBox, "gc_gift_box" );

KGCGiftBox::KGCGiftBox( void )
: m_pkTimeBack(NULL)
, m_pkNewUserBack(NULL)
, m_pNewUserGiftBox(NULL)
{
    LINK_CONTROL("time_reward_back", m_pkTimeBack);
    LINK_CONTROL("new_user_back",m_pkNewUserBack );
    LINK_CONTROL("gift_box_wnd", m_pNewUserGiftBox );

    for( int i = 0 ; i < (int)m_pEventGiftBox.size() ; ++i )
    {
        m_pEventGiftBox[i] = NULL;
    }
}

KGCGiftBox::~KGCGiftBox( void )
{
}

void KGCGiftBox::OnCreate( void )
{
    g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCGiftBox::Tick );

    m_pkTimeBack->InitState( false, true , this);
    m_pkNewUserBack->InitState( false, true , this);
    m_pNewUserGiftBox->InitState( false, true , this);
    m_pNewUserGiftBox->SetFixedWindowLocalPos( m_pkNewUserBack->GetFixedWindowLocalPos() );
    
    float fStartPosX = m_pNewUserGiftBox->GetFixedWindowLocalPos().x + 2.f;
    float fStartPosY = m_pNewUserGiftBox->GetFixedWindowLocalPos().y + 3.f;

    float fEventUserPosX = m_pkTimeBack->GetFixedWindowLocalPos().x - 27.f;
    float fEventUserPosY = m_pkTimeBack->GetFixedWindowLocalPos().y + 5.f;

    //이벤트 선물상자 박스 세팅
    std::vector<KD3DWnd*> vecSlotList 
        = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_gift_box.stg", "", 
        "gift_box\\gift_box_wnd", true, EVENT_REWARD_DISPLAY_MAX );

    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwWidth = (*iterSlot)->GetWidth();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_pEventGiftBox[i] = static_cast<KGCGiftBoxState*>(*iterSlot);
            m_pEventGiftBox[i]->InitState( false, true, this );

            D3DXVECTOR2 vPos;
            if ( i == 0 ) {
                vPos.x = fStartPosX+ static_cast<float>( (i)*(dwWidth+GIFT_BOX_GAP) );
                vPos.y = fStartPosY;
            }
            else
            {
                vPos.x = fEventUserPosX+ static_cast<float>( (i)*(dwWidth+GIFT_BOX_GAP) );
                vPos.y = fEventUserPosY;
            }

            m_pEventGiftBox[i]->SetFixedWindowLocalPos(vPos);
        }
    }
}

void KGCGiftBox::FrameMoveInEnabledState( void )
{
}

void KGCGiftBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCGiftBox::Tick()
{
    bool bNewUser = SiKGCGiftBoxManager()->IsEvent(GIFT_BOX_NEW_USER);
    bool bEventUser =  SiKGCGiftBoxManager()->IsEvent(GIFT_BOX_EVENT_USER);
    bool bExistNextReward = SiKGCGiftBoxManager()->ExistNextGiftForEvent();

    if ( SiKGCGiftBoxManager()->IsVeiwMode() == false || ( !bNewUser && !bEventUser ) )
    {
        ToggleRender(false);
        return;
    }
    ToggleRender(true);

    {
        m_pNewUserGiftBox->ToggleRender(false);
        m_pEventGiftBox[0]->ToggleRender(bNewUser);
        m_pEventGiftBox[0]->SetGiftBoxInfo(GIFT_BOX_NEW_USER, false );
        m_pEventGiftBox[0]->SetNewUserToolTip(SiKGCGiftBoxManager()->GetNewUserRewardItemID( GIFT_BOX_NEW_USER , 0 ));

        m_pEventGiftBox[1]->ToggleRender(bEventUser);
        m_pEventGiftBox[1]->SetGiftBoxInfo(GIFT_BOX_EVENT_USER, false);
        m_pEventGiftBox[1]->SetNewUserToolTip(SiKGCGiftBoxManager()->GetNewUserRewardItemID( GIFT_BOX_EVENT_USER , 0 ));

        m_pEventGiftBox[2]->ToggleRender(bExistNextReward);
        m_pEventGiftBox[2]->SetNextEventReward( bExistNextReward );
        m_pEventGiftBox[2]->SetGiftBoxInfo(GIFT_BOX_EVENT_USER, false);
        m_pEventGiftBox[2]->SetNewUserToolTip(SiKGCGiftBoxManager()->GetNewUserRewardItemID( GIFT_BOX_EVENT_USER , 1 ));

        m_pkTimeBack->ToggleRender(bEventUser);
        m_pkNewUserBack->ToggleRender(bNewUser);
    }
}

void KGCGiftBox::InitNewUser()
{
    bool bNewUser = SiKGCGiftBoxManager()->IsEvent(GIFT_BOX_NEW_USER);
    bool bNewUserRewardNext = SiKGCGiftBoxManager()->GetNextNewUserReward();
    
    m_pkNewUserBack->ToggleRender(bNewUser && bNewUserRewardNext);
    m_pEventGiftBox[0]->ToggleRender( bNewUser && bNewUserRewardNext );
    m_pEventGiftBox[0]->SetGiftBoxInfo(GIFT_BOX_NEW_USER, false );
}

void KGCGiftBox::InitEventReward()
{
    bool bEventUser = SiKGCGiftBoxManager()->IsEvent(GIFT_BOX_EVENT_USER);
    bool bEventRewardNext = SiKGCGiftBoxManager()->ExistNextGiftForEvent();

    m_pkTimeBack->ToggleRender(bEventUser);
    m_pEventGiftBox[1]->ToggleRender(bEventUser);
    m_pEventGiftBox[1]->SetGiftBoxInfo(GIFT_BOX_EVENT_USER, false);
    m_pEventGiftBox[2]->ToggleRender( bEventUser && bEventRewardNext );

}