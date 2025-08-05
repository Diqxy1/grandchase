#include "stdafx.h"
#include "GCAgitObjectPlaceCollectUI.h"
#include "GCAgitObjectSlot.h"
#include "KGCAgitGameManager.h"

IMPLEMENT_CLASSNAME( KGCAgitObjectPlaceCollectUI );
IMPLEMENT_WND_FACTORY( KGCAgitObjectPlaceCollectUI );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitObjectPlaceCollectUI, "gc_object_place_collect" );

KGCAgitObjectPlaceCollectUI::KGCAgitObjectPlaceCollectUI( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSlot[i] = NULL;

        char szTemp[128];
        sprintf_s( szTemp, 127, "slot%d", i );
        LINK_CONTROL( szTemp, m_apkSlot[i] );
    }

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;
    LINK_CONTROL( "btnFirst",   m_apkBtn[ BTN_FIRST ] );
    LINK_CONTROL( "btnPrev",    m_apkBtn[ BTN_PREV ] );
    LINK_CONTROL( "btnEnd",     m_apkBtn[ BTN_END ] );
    LINK_CONTROL( "btnNext",    m_apkBtn[ BTN_NEXT ] );
    LINK_CONTROL( "btnClose",   m_apkBtn[ BTN_CLOSE ] );

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i] = NULL;
    LINK_CONTROL( "tabObjectDefault",   m_apkTab[ TAB_OBJ_DEFAULT ] );
    LINK_CONTROL( "tabObjectActive",    m_apkTab[ TAB_OBJ_ACTIVE ] );
    LINK_CONTROL( "tabCharacterDefault",m_apkTab[ TAB_CHAR_DEFAULT ] );
    LINK_CONTROL( "tabCharacterActive", m_apkTab[ TAB_CHAR_ACTIVE ] );

    m_pkStaticPage = NULL;
    LINK_CONTROL( "staticPage", m_pkStaticPage );

    m_pkRollback = NULL;
    LINK_CONTROL( "btnRollback", m_pkRollback );

    m_iCurPage = 0;
    m_iTotalPage = 0;
    m_iCurSlot = -1;

    m_eCurTab = TAB_OBJ_ACTIVE;
}

KGCAgitObjectPlaceCollectUI::~KGCAgitObjectPlaceCollectUI( void )
{

}

void KGCAgitObjectPlaceCollectUI::OnCreate( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i]->InitState( true, true, this );

    m_apkTab[TAB_CHAR_DEFAULT]->ToggleRender( false );
    m_apkTab[TAB_CHAR_ACTIVE]->ToggleRender( false );

    m_pkRollback->InitState( true, true, this );
    m_pkStaticPage->InitState( true, false, NULL );
    m_pkStaticPage->SetAlign( DT_CENTER );

    m_apkBtn[ BTN_CLOSE ]->SetHotKey( DIK_ESCAPE );
}

void KGCAgitObjectPlaceCollectUI::ActionPerformed( const KActionEvent& event )
{
     for( int i=0 ; i<NUM_TAB ; ++i )
     {
         if( event.m_pWnd == m_apkTab[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
             OnClickTab( static_cast<TAB_TYPE>( i ) );
     }

    for( int i=0 ; i<BTN_CLOSE ; ++i )
    {
        if( event.m_pWnd == m_apkBtn[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnClickPageBtn( static_cast<BTN_TYPE>( i ) );
    }

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        if( event.m_pWnd == m_apkSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN )
        {
            if( m_iCurSlot != -1 )
                m_apkSlot[m_iCurSlot]->SetActive( false );

            OnClickSlot( i );
        }
    }

    GCWND_MSG_MAP( m_apkBtn[ BTN_CLOSE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkRollback, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRollback );
}

void KGCAgitObjectPlaceCollectUI::InitUI( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->Clear();

    m_iCurPage = 0;
    m_iTotalPage = 0;

    SetObjectInfo( SiKGCAgitGameManager()->GetEditableObjectMap() );
    SetCharacterInfo( SiKGCAgitGameManager()->GetCharacterObjInfo() );

    OnClickTab( TAB_OBJ_DEFAULT );
    SiKGCAgitGameManager()->SetAgitEditState( true );
}

void KGCAgitObjectPlaceCollectUI::UpdateData( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->Clear();

    SetObjectInfo( SiKGCAgitGameManager()->GetEditableObjectMap() );
    SetCharacterInfo( SiKGCAgitGameManager()->GetCharacterObjInfo() );

    SiKGCAgitGameManager()->SetAgitEditState( true );
}

void KGCAgitObjectPlaceCollectUI::SetObjectInfo( std::map< GCITEMID, int >& mapObj_ )
{
    m_vec2DObject.clear();

    if( mapObj_.empty() )
        return;

    std::map< GCITEMID, int >::iterator mit;
    for( mit = mapObj_.begin() ; mit != mapObj_.end() ; ++mit ) {
        m_vec2DObject.push_back( std::pair<GCITEMID,int>(mit->first, mit->second) );
    }
}

void KGCAgitObjectPlaceCollectUI::SetCharacterInfo( std::map<int, KAgitCharacterPos>& mapChar_ )
{
    m_vecCharObject.clear();

    if( mapChar_.empty() )
        return;

    std::map<int, KAgitCharacterPos>::iterator mit;
    for( mit = mapChar_.begin() ; mit != mapChar_.end() ; ++mit )
        m_vecCharObject.push_back( std::make_pair( mit->first, mit->second ) );
}

void KGCAgitObjectPlaceCollectUI::InitPage( void )
{
    int iCount = 0;
    switch( m_eCurTab )
    {
    case TAB_OBJ_ACTIVE:
        iCount = static_cast<int>( m_vec2DObject.size() );
        break;

    case TAB_CHAR_ACTIVE:
        iCount = static_cast<int>( g_kGlobalValue.m_kUserInfo.vecCharInfo.size() );
        break;
    }

    m_iTotalPage = iCount / NUM_SLOT;
    
    if( iCount % NUM_SLOT != 0 )
        ++m_iTotalPage;

    if( m_iTotalPage == 0 )
        m_iCurPage = 0;
    else
        m_iCurPage = 1;

    SetPageStatic();
    SetSlot( m_iCurPage );
}

void KGCAgitObjectPlaceCollectUI::SetSlot( int iPage_ )
{
    if( iPage_ < 1 || iPage_ > m_iTotalPage )
        return;

    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->Clear();

    switch( m_eCurTab )
    {
    case TAB_OBJ_ACTIVE:
        {
            int iStartIndex = ( m_iCurPage - 1 ) * NUM_SLOT ;
            for( int i=0 ; i<NUM_SLOT ; ++i )
            {
                if( (iStartIndex+i) >= static_cast<int>(m_vec2DObject.size() ))
                    break;

                m_apkSlot[ i ]->SetObjectSlot( m_vec2DObject[ iStartIndex + i ].first, m_vec2DObject[ iStartIndex + i ].second );
            }
        }
        break;

    case TAB_CHAR_ACTIVE:
        {
            int iStartIndex = ( m_iCurPage - 1 ) * NUM_SLOT;
            for( int i=0 ; i<NUM_SLOT ; ++i )
            {
                if( (iStartIndex+i) >= static_cast<int>( g_kGlobalValue.m_kUserInfo.vecCharInfo.size() ) )
                    break;

                int iCharType = static_cast<int>( g_kGlobalValue.m_kUserInfo.vecCharInfo[iStartIndex+i].iCharType );
                bool bLock = false;

                std::map< int, KAgitCharacterPos >::iterator mit = SiKGCAgitGameManager()->GetCharacterObjInfo().find( iCharType );
                if( mit == SiKGCAgitGameManager()->GetCharacterObjInfo().end() )
                    bLock = true;

                m_apkSlot[ i ]->SetCharSlot( iCharType, bLock );
            }
        }
        break;
    }
}

void KGCAgitObjectPlaceCollectUI::SetSlot( void )
{
    SetSlot( m_iCurPage );
}

void KGCAgitObjectPlaceCollectUI::SetPageStatic( void )
{
    std::wostringstream stm;
    stm << m_iCurPage << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText( stm.str() );
}

void KGCAgitObjectPlaceCollectUI::ClearSlot( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSlot[i]->SetActive( false );
        m_apkSlot[i]->Clear();
    }
}

void KGCAgitObjectPlaceCollectUI::OnClickSlot( int iIndex_ )
{
    if( iIndex_ < 0 || iIndex_ >= NUM_SLOT )
        return;

    if( TAB_OBJ_ACTIVE == m_eCurTab )
    {
        if( (( m_iCurPage - 1 ) * NUM_SLOT + iIndex_) >= (int)m_vec2DObject.size() )
            return;

        if( m_vec2DObject.empty() )
            return;

        if( m_vec2DObject[ ( m_iCurPage - 1 ) * NUM_SLOT + iIndex_ ].second <= 0 )
            return;
    }
    else if( TAB_CHAR_ACTIVE == m_eCurTab )
    {
        if( GC_CHAR_INVALID == m_apkSlot[ iIndex_ ]->GetCharType() )
            return;
    }

    m_iCurSlot = iIndex_;

    if( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType  == m_apkSlot[ iIndex_ ]->GetCharType() )
        m_apkSlot[ m_iCurSlot ]->SetActive( true );

    g_KDSound.Play( "984029" );

    switch( m_eCurTab )
    {
    case TAB_OBJ_ACTIVE:
        {
            GCITEMUID itemUID = SiKGCAgitGameManager()->GetHideObjectUID( m_vec2DObject[ ( m_iCurPage - 1 ) * NUM_SLOT + iIndex_ ].first );
            SiKGCAgitGameManager()->PickUpObject( itemUID );
        }
        break;

    case TAB_CHAR_ACTIVE:
        {
            if( true == m_apkSlot[ m_iCurSlot ]->IsCharLock() ) {  // 잠겨 있음 -> 오픈 요청 
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHARACTER_OPEN, "i", 50 ), L"", 
                    KGCUIScene::GC_MBOX_USE_AGIT_CHARACTER_OPEN, m_apkSlot[ m_iCurSlot ]->GetCharType(), 0, true, true );
            }
            else {
                 // 아지트 캐릭터 최대 보유 가능 개수를 초과 하면 설치 시도를 못하게 수정
                 if ( SiKGCAgitGameManager()->GetAgitMaxCharPosCount() <= SiKGCAgitGameManager()->GetAgitCurCharPosCount() ) {
                     g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_CHARACTER_SER_POS_FAIL18 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
                 }
                 else {
                    const POINT& ptMouse = g_pkInput->GetMousePos();
                    D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
                    SiKGCAgitGameManager()->PickUpCharacter( m_apkSlot[ m_iCurSlot ]->GetCharType(), dxvMousePos );
                }
            }
        }
        break;
    }
}

void KGCAgitObjectPlaceCollectUI::OnClickTab( TAB_TYPE tabType_ )
{
    if( tabType_ < 0 || tabType_ >= NUM_TAB ) 
        return;

    switch( tabType_ )
    {
    case TAB_OBJ_DEFAULT:
        m_apkTab[ TAB_OBJ_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_OBJ_ACTIVE ]->ToggleRender( true );
        m_apkTab[ TAB_CHAR_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_CHAR_ACTIVE ]->ToggleRender( false );
        m_eCurTab = TAB_OBJ_ACTIVE;
        break;

    case TAB_CHAR_DEFAULT:
        m_apkTab[ TAB_OBJ_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_OBJ_ACTIVE ]->ToggleRender( false );
        m_apkTab[ TAB_CHAR_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_CHAR_ACTIVE ]->ToggleRender( true );
        m_eCurTab = TAB_CHAR_ACTIVE;
        break;
    }

    ClearSlot();
    InitPage();
}

void KGCAgitObjectPlaceCollectUI::OnClickPageBtn( BTN_TYPE btnType_ )
{
    if( btnType_ < 0 || btnType_ >= BTN_CLOSE )
        return;

    if( m_iTotalPage <= 0 )
        return;

    switch( btnType_ )
    {
    case BTN_FIRST:
        m_iCurPage = 1;
        break;

    case BTN_PREV:
        if( m_iCurPage > 1 )
            --m_iCurPage;
        break;

    case BTN_END:
        m_iCurPage = m_iTotalPage;
        break;

    case BTN_NEXT:
        if( m_iCurPage < m_iTotalPage )
            ++m_iCurPage;
        break;
    }

    if( m_iCurSlot != -1 )
    {
        m_apkSlot[ m_iCurSlot ]->SetActive( false );
        m_iCurSlot = -1 ;
    }

    SetPageStatic();
    g_KDSound.Play( "984040" );
    SetSlot( m_iCurPage );
}

void KGCAgitObjectPlaceCollectUI::OnClose( void )
{
    // 드래그 중이었으면 회수하고 
    SiKGCAgitGameManager()->RemovePickedObject();
    SiKGCAgitGameManager()->RemovePickCharacter();

    // 선택은 끄고 
    if( m_iCurSlot != -1 )
        m_apkSlot[ m_iCurSlot ]->SetActive( false );

    this->ToggleRender( false );
    SiKGCAgitGameManager()->SetAgitEditState( false );

    // 에디드 모드 끌 때 배치정보 저장합세 
    SiKGCAgitGameManager()->SaveAgitObject();
    SiKGCAgitGameManager()->SaveAgitCharacterPos();

    g_pkAgitOverlayUI->ToggleMainUI( true );
    g_pkAgitOverlayUI->ToggleRenderBrownieBtn( true );

    // 캐릭터 세팅 열려있으면 닫아 줍시다 
    g_pkAgitOverlayUI->ShowCharSettingUI( false );
}

void KGCAgitObjectPlaceCollectUI::OnClickRollback()
{
    SiKGCAgitGameManager()->RollbackObjectPos();
}

void KGCAgitObjectPlaceCollectUI::RefreshCharTab( bool bIsTrue )
{
    if ( bIsTrue )
    {
        m_apkTab[ TAB_OBJ_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_OBJ_ACTIVE ]->ToggleRender( false );
        m_apkTab[ TAB_CHAR_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_CHAR_ACTIVE ]->ToggleRender( true );
        m_eCurTab = TAB_CHAR_ACTIVE;
    }
    else
    {
        m_apkTab[ TAB_OBJ_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_OBJ_ACTIVE ]->ToggleRender( true );
        m_apkTab[ TAB_CHAR_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_CHAR_ACTIVE ]->ToggleRender( false );
        m_eCurTab = TAB_OBJ_ACTIVE;
    }

    ClearSlot();
    InitPage();
}
