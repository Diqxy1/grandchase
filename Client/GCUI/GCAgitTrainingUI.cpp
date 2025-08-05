#include "stdafx.h"
#include "GCAgitTrainingUI.h"
#include "Message.h"

IMPLEMENT_CLASSNAME( KGCAgitTrainingUI );
IMPLEMENT_WND_FACTORY( KGCAgitTrainingUI );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitTrainingUI, "gc_agit_training" );

KGCAgitTrainingUI::KGCAgitTrainingUI( void )
{
    m_pkComboChar = NULL;
    LINK_CONTROL( "charCombobox", m_pkComboChar );

    m_pkStaticChar = NULL;
    m_pkStaticLevel = NULL;
    m_pkStaticBrokenTile = NULL;
    m_pkStaticRemainTile = NULL;
    m_pkStaticLevelInfo = NULL;
    m_pkStaticBrokenTileInfo = NULL;
    m_pkStaticRemainTileInfo = NULL;
    LINK_CONTROL( "staticChar", m_pkStaticChar );
    LINK_CONTROL( "staticLevel", m_pkStaticLevel );
    LINK_CONTROL( "staticBrokenTile", m_pkStaticBrokenTile );
    LINK_CONTROL( "staticRemainTile", m_pkStaticRemainTile );
    LINK_CONTROL( "staticLevelInfo", m_pkStaticLevelInfo );
    LINK_CONTROL( "staticBrokenTileInfo", m_pkStaticBrokenTileInfo );
    LINK_CONTROL( "staticRemainTileInfo", m_pkStaticRemainTileInfo );

    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btnOK", m_pkBtnOK );
    LINK_CONTROL( "btnCancle", m_pkBtnCancle );
    LINK_CONTROL( "btnClose", m_pkBtnClose );

    m_vecCharInfo.clear();
    m_iComboCurSel = -1;
    m_itemUID = KItem::INVALID_ITEM;
}

KGCAgitTrainingUI::~KGCAgitTrainingUI( void )
{
}

void KGCAgitTrainingUI::OnCreate( void )
{
    m_pkComboChar->InitState( true, true, this );

    m_pkStaticChar->InitState( true );
    m_pkStaticLevel->InitState( true );
    m_pkStaticBrokenTile->InitState( true );
    m_pkStaticRemainTile->InitState( true );
    
    m_pkStaticChar->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticLevel->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticBrokenTile->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticRemainTile->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );

    m_pkStaticChar->SetFontSpace( 0.8f );
    m_pkStaticLevel->SetFontSpace( 0.8f );
    m_pkStaticBrokenTile->SetFontSpace( 0.8f );
    m_pkStaticRemainTile->SetFontSpace( 0.8f );

    m_pkStaticLevelInfo->InitState( true );
    m_pkStaticBrokenTileInfo->InitState( true );
    m_pkStaticRemainTileInfo->InitState( true );

    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkBtnClose->InitState( true, true, this );

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCAgitTrainingUI::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkComboChar && event.m_dwCode == KD3DWnd::D3DWE_COMBO_SELCHANGE )
        OnComboChange();

    GCWND_MSG_MAP( m_pkBtnClose,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkBtnCancle,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkBtnOK,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnOK );
}

void KGCAgitTrainingUI::InitUI( GCITEMUID itemUID )
{   
    if( itemUID == KItem::INVALID_ITEM )
        return;

    m_pkStaticChar->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TRAINING_CHAR_SELECT ) );
    m_pkStaticLevel->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TRAINING_EXPLEVEL ) );
    m_pkStaticBrokenTile->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TRAINING_READY_TILE ) );
    m_pkStaticRemainTile->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TRAINING_REMAIN_TILE ) );

    m_vecCharInfo.clear();
    m_vecCharInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo;
    m_iComboCurSel = -1;
    m_itemUID = itemUID;

    SetComboList();
    SetEXPLevelInfo();
    SetBrokenTileInfo( SiKGCAgitGameManager()->GetBrokenTrainingTile( m_itemUID ) );
    SetRemainTileInfo();
}

void KGCAgitTrainingUI::OnClose( void )
{
    this->ToggleRender( false );
}

void KGCAgitTrainingUI::OnOK( void )
{
    if( m_iComboCurSel < 0 || m_iComboCurSel >= static_cast<int>( m_vecCharInfo.size() ) )
        return;

    if( SiKGCAgitGameManager()->GetBrokenTrainingTile( m_itemUID ) <= 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_USE_TRAINING_OBJ_FAIL13 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return;
    }
#if defined (NATION_BRAZIL) || defined(NATION_PHILIPPINE)
    else if ( m_vecCharInfo[ m_iComboCurSel ].iLevel == MAX_LEVEL )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_USE_TRAINING_OBJ_FAIL12 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return;
    }
#endif

    SiKGCAgitGameManager()->SetBrokenTileNum( SiKGCAgitGameManager()->GetBrokenTrainingTile( m_itemUID ) );

    Result_UseTrainingObject = INT_MAX;
    KP2P::GetInstance()->Send_UseTrainingObjReq( m_vecCharInfo[ m_iComboCurSel ].iCharType, m_itemUID );
    g_MyD3D->WaitForServerAck( Result_UseTrainingObject, INT_MAX, 3000, TIME_OUT_VALUE );

    if( Result_UseTrainingObject == 0 ) { 
        int iPlayerIdx = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iPlayerIdx) )
        {
            KGC_PID_BROAD_AGIT_BREAK kPacket;
            kPacket.Type = GC_PID_BROAD_AGIT_BREAK;
            kPacket.dwUID = g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.dwUID;
            kPacket.iMonID = SiKGCAgitGameManager()->ConvertMonsterIDToCharType( m_vecCharInfo[ m_iComboCurSel ].iCharType );
            kPacket.iDiscCnt = SiKGCAgitGameManager()->GetBrokenTrainingTile( m_itemUID );

            D3DXVECTOR2 dxvPos = SiKGCAgitGameManager()->GetTrainingObjPos( m_itemUID );
            kPacket.vPos.x = dxvPos.x + 0.114f;
            kPacket.vPos.y = dxvPos.y + 0.1f;

            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_BROAD_AGIT_BREAK" );
        }
    }

    this->ToggleRender( false );
}

void KGCAgitTrainingUI::SetComboList( void )
{
    m_pkComboChar->DeleteAllItem();

    m_pkComboChar->SetComboStatic( GCFUNC::GetCharName( 0 ) );
    for( std::vector<SCharInfo>::iterator vit = m_vecCharInfo.begin() ; vit != m_vecCharInfo.end() ; ++vit )
        m_pkComboChar->AddString( GCFUNC::GetCharName( vit->iCharType ) );

    m_iComboCurSel = 0; 
}

void KGCAgitTrainingUI::SetEXPLevelInfo( void )
{
    if( m_iComboCurSel < 0 || m_iComboCurSel >= static_cast<int>( m_vecCharInfo.size() ) )
        return;

    int iLevel = m_vecCharInfo[ m_iComboCurSel ].iLevel;
    float fExpPercent= abs( Exp_For_Percent( m_vecCharInfo[ m_iComboCurSel ].biExp ) );

    m_pkStaticLevelInfo->SetText( g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_EXP_INFO, "id", iLevel, fExpPercent ) );
}

std::wstring KGCAgitTrainingUI::GetCharName( int iChaarType_ )
{
    switch( iChaarType_ )
    {
    case GC_CHAR_ELESIS:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ELESIS );
    case GC_CHAR_LIRE:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
    case GC_CHAR_ARME:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
    case GC_CHAR_LAS:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
    case GC_CHAR_RYAN:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
    case GC_CHAR_RONAN:
        return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
    case GC_CHAR_AMY:
        return g_pkStrLoader->GetString( STR_ID_AMY );
    case GC_CHAR_JIN:
        return g_pkStrLoader->GetString( STR_ID_JIN );
    case GC_CHAR_SIEG:
        return g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
    case GC_CHAR_MARI:
        return g_pkStrLoader->GetString( STR_ID_MARI_NAME );
    case GC_CHAR_DIO:
        return g_pkStrLoader->GetString( STR_ID_DIO_NAME );
    case GC_CHAR_ZERO:
        return g_pkStrLoader->GetString( STR_ID_ZERO_NAME );
    case GC_CHAR_LEY:
        return g_pkStrLoader->GetString( STR_ID_LEY_NAME );
    case GC_CHAR_LUPUS:
        return g_pkStrLoader->GetString( STR_ID_LUPUS_NAME );
    case GC_CHAR_RIN:
        return g_pkStrLoader->GetString( STR_ID_RIN_NAME );
    case GC_CHAR_ASIN:
        return g_pkStrLoader->GetString( STR_ID_ASIN_NAME );
    case GC_CHAR_LIME:
        return g_pkStrLoader->GetString( STR_ID_LIME_NAME );
    case GC_CHAR_EDEL:
        return g_pkStrLoader->GetString( STR_ID_EDEL_NAME );
    case GC_CHAR_BEIGAS:
        return g_pkStrLoader->GetString( STR_ID_BEIGAS_NAME );
	case GC_CHAR_UNO:
		return g_pkStrLoader->GetString( STR_ID_UNO_NAME );
#ifdef PERYTON // Kawan>
    case GC_CHAR_PERYTON:
        return g_pkStrLoader->GetString( STR_ID_PERYTON_NAME );
#endif
#ifdef ALICE //
    case GC_CHAR_ALICE:
        return g_pkStrLoader->GetString( STR_ID_ALICE_NAME );
#endif
#ifdef DECANEE //
    case GC_CHAR_DECANEE:
        return g_pkStrLoader->GetString(STR_ID_DECANEE_NAME);
#endif
    default:
        return std::wstring();
    }
}

void KGCAgitTrainingUI::OnComboChange( void )
{
    m_iComboCurSel = m_pkComboChar->GetCurSel();
    SetEXPLevelInfo();
}

void KGCAgitTrainingUI::SetBrokenTileInfo( int iNumtile_ )
{
    if( iNumtile_ > MAX_TILE )
        iNumtile_ = MAX_TILE;

    std::wostringstream stm;
    stm << iNumtile_ << L" / " << MAX_TILE;
    m_pkStaticBrokenTileInfo->SetText( stm.str() );
}

void KGCAgitTrainingUI::SetRemainTileInfo( void )
{
    KItem* pTile = NULL;

    g_pItemMgr->FindInventoryForItemID( 60532, &pTile );    // 격파 아이템 ID : 605320
    if( NULL == pTile )
    {
        m_pkStaticBrokenTileInfo->SetNumber( 0 );
        return;
    }

    m_pkStaticRemainTileInfo->SetNumber( pTile->m_nCount );
}