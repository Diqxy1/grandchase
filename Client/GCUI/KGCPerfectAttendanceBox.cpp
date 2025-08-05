#include "stdafx.h"
#include "KGCPerfectAttendanceBox.h"
//
//

//
#include "MyD3D.h"
#include "KGCAttendanceItemWnd.h"
//
#include "GCItemInfoBox.h"

IMPLEMENT_CLASSNAME( KGCPerfectAttendanceBox );
IMPLEMENT_WND_FACTORY( KGCPerfectAttendanceBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPerfectAttendanceBox, "gc_perfect_attendance" );

KGCPerfectAttendanceBox::KGCPerfectAttendanceBox(void) 
: m_iGetItem(NULL)
, m_iSelectedReward(-1)
{
    m_pkExit								= NULL;
    m_pkScrollbar							= NULL;

    for( int i = 0; i < 6; ++i )
        m_apkItem[i]						 = NULL;

    m_pkPerfectAttendanceMark    = NULL;
    m_pkReceiveBtn						= NULL;

    LINK_CONTROL( "exit_button", m_pkExit );
    LINK_CONTROL( "scroll", m_pkScrollbar );
    LINK_CONTROL( "perfect_attendance_mark", m_pkPerfectAttendanceMark );
    LINK_CONTROL( "receive_button", m_pkReceiveBtn );

    m_iScrollPos = 0;
    m_iRewardType = 0;
}

KGCPerfectAttendanceBox::~KGCPerfectAttendanceBox(void)
{
}

void KGCPerfectAttendanceBox::OnCreate( void )
{
    m_pkExit->InitState( true, true, this );
    m_pkExit->SetHotKey( DIK_ESCAPE );

    m_pkScrollbar->InitState( true, true, this );
    m_pkScrollbar->SetScrollPos( 0 );
    m_pkScrollbar->SetScrollRangeMin( 0 );
    m_pkScrollbar->SetScrollRangeMax( 0 );
    m_pkScrollbar->SetScrollGap( 1 );
    m_pkScrollbar->SetScrollPageSize( 2 );

	std::vector<KD3DWnd*> vecBoxes = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_perfect_attendance.stg", "", "perfect_attendance\\box_item", true, 6);

	for( unsigned int i = 0; i < vecBoxes.size(); i++ )
	{
		KGCAttendanceItemWnd* box = (KGCAttendanceItemWnd*)vecBoxes[i];

		float posX = (float)( (i % NUM_ITEM_SCROLL) * box->GetWidth() ) + 3.f ;
		float posY = (float)( (i / NUM_ITEM_SCROLL) * box->GetHeight() );

		box->SetWindowPosX( (float)(posX + 22));
		box->SetWindowPosY( (float)(posY + 65));
		box->InitState( true, true, this );

		m_apkItem[i] = box;
	}

    m_pkPerfectAttendanceMark->InitState( true );
	m_pkReceiveBtn->InitState(true, true, this);
	m_pkReceiveBtn->SetHotKey(DIK_RETURN);
	m_pkReceiveBtn->Lock(true);
}

void KGCPerfectAttendanceBox::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iPos = m_pkScrollbar->GetScrollPos();

        if ( m_iScrollPos != iPos )
        {
            g_KDSound.Play( "73" );
            OnScrollPos( iPos );
        }
        return;
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkExit )
        {
            //m_pkItemInfoBox->ToggleRender( false );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
            SpeakToActionListener( KActionEvent( this, EXIT ) );
            g_KDSound.Play( "31" );
        }
		else if( event.m_pWnd == m_pkReceiveBtn)
		{
			// 아이템 받기 버튼을 클릭했다면..
			if( m_iSelectedReward != -1 )
			{
				m_iGetItem = m_apkItem[m_iSelectedReward]->m_ItemInfo;
				m_apkItem[m_iSelectedReward]->m_bIsGetButtonClick = false;
				g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );

				SpeakToActionListener( KActionEvent( this, SELECT_REWARD_ITEM ) );
				return;
			}
		}
		else
		{
			for( int i = 0; i < NUM_ITEM_WND; ++i )
			{
				KGCAttendanceItemWnd* itemBox = m_apkItem[i];

				if( event.m_pWnd == itemBox && itemBox->m_ItemInfo != NULL)
				{
					if( itemBox->m_bIsGetButtonClick )
					{
						itemBox->m_bIsGetButtonClick = false;
						m_iSelectedReward = -1;
						m_pkReceiveBtn->Lock(true);
						break;
					}

					if( m_iSelectedReward != -1 )
					{
						m_apkItem[m_iSelectedReward]->m_bIsGetButtonClick = false;
						itemBox->m_bIsGetButtonClick = true;
						m_iSelectedReward = i;
						m_pkReceiveBtn->Lock(false);
						break;
					}

					itemBox->m_bIsGetButtonClick = true;
					m_iSelectedReward = i;
					m_pkReceiveBtn->Lock(false);
					break;
				}
			}
		}
    }
}

void KGCPerfectAttendanceBox::FrameMoveInEnabledState()
{
//     for( int i = 0; i < NUM_ITEM_WND; ++i )
//     {
//         // 아이템 받기 버튼을 클릭했다면..
//         if( m_apkItem[i]->m_bIsGetButtonClick )
//         {
//             m_apkItem[i]->m_bIsGetButtonClick = false;
// 
//             m_iGetItem = m_apkItem[i]->m_ItemInfo;
// 
//             //m_pkItemInfoBox->ToggleRender( false );
//             g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
//             SpeakToActionListener( KActionEvent( this, SELECT_REWARD_ITEM ) );
//             return;
//         }
//     }
}

void KGCPerfectAttendanceBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, DESTROYED ) );
}

void KGCPerfectAttendanceBox::SetRewardList( int iWeek_ )
{
    int iTotalPage;
    if( iWeek_ == 0 )
    {
        g_MyD3D->m_kItemMgr.MakePerfectMonthItemList();

        iTotalPage = ( g_MyD3D->m_kItemMgr.GetPerfectMonthBoxItemSize() + NUM_ITEM_SCROLL - 1 ) / NUM_ITEM_SCROLL;
        //m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_PERFECT_ATTENDANCE_DESC ) );   // 월 개근 
    }
    else
    {
        iTotalPage = ( g_MyD3D->m_kItemMgr.GetPerfectWeekItemList().size() + NUM_ITEM_SCROLL - 1 ) / NUM_ITEM_SCROLL;
        //m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_WEEKLY_PERFECT_ATTENDANCE_DESC, "i", iWeek_ ) );   // 주 개근 
    }
    m_iRewardType = iWeek_;
    m_pkScrollbar->SetScrollRangeMax( iTotalPage );
    OnScrollPos( m_pkScrollbar->GetScrollPos() );
}

void KGCPerfectAttendanceBox::OnScrollPos( int iPos )
{
    m_iScrollPos = iPos;
    int iStartIndex = iPos * NUM_ITEM_SCROLL;
    int iInventorySize;
    if( !m_iRewardType )
        iInventorySize = (int)g_MyD3D->m_kItemMgr.GetPerfectMonthBoxItemSize();
    else
        iInventorySize = (int)g_MyD3D->m_kItemMgr.GetPerfectWeekItemList().size();

    for( int i = 0; i < NUM_ITEM_WND; i++ )
    {
        if( iStartIndex + i >= iInventorySize )
        {
            // 아이템 세팅 하지 않는다!
            KItem kTempItem;
            kTempItem.m_ItemID = 0;
            kTempItem.m_nCount= KItem::UNLIMITED_ITEM;
            kTempItem.m_nPeriod = KItem::UNLIMITED_ITEM;
            m_apkItem[i]->SetItem( false, kTempItem );
            continue;
        }
        if( !m_iRewardType )
            m_apkItem[i]->SetItem( true, g_MyD3D->m_kItemMgr.GetPerfectMonthBoxItem( iStartIndex + i ) );
        else
            m_apkItem[i]->SetItem( true, g_MyD3D->m_kItemMgr.GetPerfectWeekItemList()[ iStartIndex + i ] );
    }
}

bool KGCPerfectAttendanceBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->SetRewardList( static_cast<int>(kInfo_.m_lParam) );
    return true;
}

void KGCPerfectAttendanceBox::PostChildDraw( void )
{
    ShowItemInfo();
}

void KGCPerfectAttendanceBox::ShowItemInfo( void )
{
    GCItem* kItem = NULL;
    bool bHover = false;
    D3DXVECTOR2 vecPos( 0.0f, 0.0f );

    POINT pt = g_pkInput->GetMousePos();

    for( int i = 0; i < NUM_ITEM_WND; ++i )
    {
        // 아이템 위에 마우스 오버 되어 있다면..
        if( m_apkItem[i]->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) ) )
        {
            if( m_apkItem[i]->GetHoveredItem( kItem, vecPos ) )
            {
                BREAK_NIL( kItem );
                //m_pkItemInfoBox->SetItem( kItem );
				m_apkItem[i]->ShowItem(true);
                g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
                g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

                bHover = true;
                break;
            }
        }
    }

    if( bHover )
    {
        vecPos.x *= m_fWindowScaleX;
        vecPos.y *= m_fWindowScaleY;
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }

    else
    {
        //m_pkItemInfoBox->ToggleRender( false );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}
