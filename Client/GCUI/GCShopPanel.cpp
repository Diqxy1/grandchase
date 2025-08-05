#include "stdafx.h"
#include "GCShopPanel.h"
#include "GCShopTab.h"
//
//
//
#include "../MyD3D.h"



//
//

IMPLEMENT_CLASSNAME( KGCShopPanel );
IMPLEMENT_WND_FACTORY( KGCShopPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCShopPanel, "gc_shop_panel" );

KGCShopPanel::KGCShopPanel( void )
{

    m_pkTabArmor    = NULL;
    m_pkTabSet      = NULL;
    m_pkTabWeapon   = NULL;
    m_pkTabAccessory = NULL;
    m_pkTabTraining = NULL;

    m_pkCurActiveTab = NULL;

    for( int i = 0 ; i < (int)m_apkShopTabList.size() ; ++i )
    {
       m_apkShopTabList[i] = NULL;
    }
    for( int i = 0 ; i < (int)m_apkNewGoods.size() ; ++i )
    {
        m_apkNewGoods[i] = NULL;
    }

    m_pkItemBox     = NULL;

    m_eShopTabType = STT_ARMOR;

    m_ucCharNum = 0;

    m_mapTabType.clear();

    LINK_CONTROL( "armor_tab",      m_pkTabArmor );
    LINK_CONTROL( "set_tab",        m_pkTabSet );
    LINK_CONTROL( "weapon_tab",     m_pkTabWeapon );
    LINK_CONTROL( "accessory_tab",  m_pkTabAccessory );
    LINK_CONTROL( "training_tab",   m_pkTabTraining );
    LINK_CONTROL( "shop_item_box",  m_pkItemBox );
	
	char caName[MAX_PATH];

	for( UINT i = 0 ; i < KGCShopItemBox::NUM_ITEM_WND ; i++ )
	{
		sprintf( caName, "new_goods%d", i);
		LINK_CONTROL( caName,     m_apkNewGoods[i] );
	}
    
}

KGCShopPanel::~KGCShopPanel( void )
{
    m_mapTabType.clear();
}

void KGCShopPanel::OnActivateComplete( void )
{
    m_pkTabArmor->Activate( false );
    m_pkTabSet->Activate( false );
    m_pkTabWeapon->Activate( false );
    m_pkTabAccessory->Activate( false );
    m_pkTabTraining->Activate( false );

    if ( m_pkCurActiveTab != NULL ) SetActiveShopTab( m_pkCurActiveTab );
}

void KGCShopPanel::OnCreate( void )
{
    m_pkTabArmor->SetSelfInputCheck( false );
    m_pkTabSet->SetSelfInputCheck( false );
    m_pkTabWeapon->SetSelfInputCheck( false );
    m_pkTabAccessory->SetSelfInputCheck( false );
    m_pkTabTraining->SetSelfInputCheck( false );

    m_pkItemBox->SetSelfInputCheck( false );
    m_pkItemBox->SetButtonEventCatch( false );
    m_pkItemBox->AddActionListener( this );

    m_apkShopTabList[0] = m_pkTabArmor;
    m_apkShopTabList[1] = m_pkTabSet;
    m_apkShopTabList[2] = m_pkTabWeapon;
    m_apkShopTabList[3] = m_pkTabAccessory;
    m_apkShopTabList[4] = m_pkTabTraining;

    m_mapTabType[m_pkTabArmor]      = STT_ARMOR;
    m_mapTabType[m_pkTabSet]        = STT_SET;
    m_mapTabType[m_pkTabWeapon]     = STT_WEAPON;
    m_mapTabType[m_pkTabAccessory]  = STT_ACCESSORY;
    m_mapTabType[m_pkTabTraining]   = STT_TRAINING;

    m_pkCurActiveTab = m_pkTabArmor;
    m_eShopTabType = STT_ARMOR;

    for ( int i = 0; i < KGCShopItemBox::NUM_ITEM_WND; ++i )
    {
        m_apkNewGoods[i]->SetSelfInputCheck( false );
        m_apkNewGoods[i]->ToggleRender( false );
    }

    m_ucCharNum = g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex;
}

void KGCShopPanel::FrameMoveInEnabledState( void )
{
    // 샵 탭을 클릭했는지 확인한다.
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
    {
        for ( int i = 0; i < NUM_SHOP_TAB; ++i )
        {
            if ( m_apkShopTabList[i]->CheckPosInWindowBound( vMousePos ) )
            {
                g_KDSound.Play( "31" );
                SetActiveShopTab( m_apkShopTabList[i] );
                break;
            }
        }
    }

    // 캐릭터가 변경되었다면 표시되는 리스트도 리셋한다.
    if ( m_ucCharNum != g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex )
    {
        m_pkItemBox->ResetScrollbar( 0 );
    }
    m_ucCharNum = g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex;
}

void KGCShopPanel::ActionPerformed( const KActionEvent& event )
{
}

void KGCShopPanel::FilterGoodsItem( EShopTapType eShopTabType )
{
    m_eShopTabType = eShopTabType;
    m_pkItemBox->ResetScrollbar( 0 );
}

void KGCShopPanel::SetShopClerkStr( const GCItem* item )
{
    std::wostringstream strmDesc;


    strmDesc << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << item->iAttack << std::endl
             << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << item->iDefence << std::endl
             << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << item->iHealth << std::endl
             << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << static_cast<int>( item->iNeedLevelForEquippingItem ) << std::endl;
}

void KGCShopPanel::SetActiveShopTab( KGCShopTab* pkActiveTab )
{
    static int iTabDescID[] =
    {
        STR_ID_KIND_ARMOR_DESC,
        STR_ID_KIND_SET_DESC,
        STR_ID_KIND_WEAPON_DESC,
        STR_ID_KIND_ACCESSORY_DESC,
        STR_ID_KIND_DOCUMENT_DESC,
    };

    if ( false == pkActiveTab->IsActive() )
    {
        typedef std::vector<KD3DWnd*>::iterator WndListItr;

        KGCShopTab* pkTab[] =
        {
            m_pkTabArmor,
            m_pkTabSet,
            m_pkTabWeapon,
            m_pkTabAccessory,
            m_pkTabTraining,
        };

        // 자식 윈도우 리스트에서 탭들만 제거한다.
        WndListItr itrItemBox = std::find( m_vecChild.begin(), m_vecChild.end(), m_pkItemBox );
        ASSERT( itrItemBox != m_vecChild.end() );
        m_vecChild.erase( itrItemBox + 1, itrItemBox + 1 + NUM_SHOP_TAB );

        // 탭 순서를 재정렬하고 선택된 탭을 활성화한다.
        std::vector<KD3DWnd*> vecTabList;
        for ( UINT i = 0; i < sizeof( pkTab ) / sizeof( KGCShopTab* ); ++i )
        {
            if ( pkTab[i] == pkActiveTab )
            {
                pkTab[i]->Activate( true );
                FilterGoodsItem( m_mapTabType[pkTab[i]] );
            }
            else
            {
                vecTabList.push_back( pkTab[i] );
                pkTab[i]->Activate( false );
            }
        }
        vecTabList.push_back( pkActiveTab );

        // 재정렬된 탭들을 원래 리스트에 삽입한다.
        itrItemBox = std::find( m_vecChild.begin(), m_vecChild.end(), m_pkItemBox );
        ASSERT( itrItemBox != m_vecChild.end() );
        m_vecChild.insert( itrItemBox + 1, vecTabList.begin(), vecTabList.end() );

        m_pkCurActiveTab = pkActiveTab;

        m_pkItemBox->SetSetItemView( pkActiveTab == m_pkTabSet );
    }
}
