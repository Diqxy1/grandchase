#include "stdafx.h"
#include "GCEclipseOldBooks.h"

IMPLEMENT_CLASSNAME( KGCEclipseOldBooks );
IMPLEMENT_WND_FACTORY( KGCEclipseOldBooks );
IMPLEMENT_WND_FACTORY_NAME( KGCEclipseOldBooks, "gc_eclipse_old_books" );

KGCEclipseOldBooks::KGCEclipseOldBooks( void )
: m_pkBtnCombination( NULL )
, m_pkBtnClose( NULL )
, m_pkStaticDesc( NULL )
, m_pkImgRewardItem( NULL )
, m_pkBookForm( NULL )
, m_pkBtnReadBook( NULL )
, m_bAllAcquired( false )
, m_dwRewardItemID( 0 )
, m_iParticleLife( 0 )
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < MAX_OLD_BOOKS_NUM; ++i )
    {
        m_pkOldBooks[i] = NULL;
        sprintf( str, "old_book_%d", i );  
        LINK_CONTROL( str, m_pkOldBooks[i] ); 
    }

    LINK_CONTROL( "btn_combination",        m_pkBtnCombination );
    LINK_CONTROL( "btn_close",              m_pkBtnClose );
    LINK_CONTROL( "btn_read_book",          m_pkBtnReadBook );

    LINK_CONTROL( "static_desc",            m_pkStaticDesc );

    LINK_CONTROL( "img_reward_item",        m_pkImgRewardItem );    

    LINK_CONTROL( "book_form",              m_pkBookForm );

    LINK_CONTROL( "all_acquired_old_book",  m_pkAllAcquiredOldBook );

}

KGCEclipseOldBooks::~KGCEclipseOldBooks( void )
{
}

void KGCEclipseOldBooks::OnCreate( void )
{
    for( int i = 0; i < PARTICLE_NUM; i++ ) {
        m_pParticle[i] = NULL;
    }

    m_pkBtnCombination->InitState( true, true, this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBtnReadBook->InitState( false, true, this );

    m_pkAllAcquiredOldBook->InitState( false );
    
    m_pkStaticDesc->InitState( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_EVENT_BOOK_DESC ) );

    m_pkImgRewardItem->InitState( true );
    m_pkImgRewardItem->SetItemInfo( m_dwRewardItemID );
    
    m_pkBookForm->InitState( false, true, this );        
    
    LoadOldBookData();

    for( int i = 0; i < MAX_OLD_BOOKS_NUM; i++ ) {
        m_pkOldBooks[i]->InitState( true, true, this );
        m_pkOldBooks[i]->Lock( true );
    }

    CheckAcquiredOldBooks();

    SetCompetionEffect();  
    ShowCompetionEffect( m_bAllAcquired );
}

void KGCEclipseOldBooks::OnDestroy( void )
{
    m_iParticleLife = 0;

    for( int i = 0; i < PARTICLE_NUM; i++ ) {
        if( m_pParticle[i] ) {
            g_ParticleManager->DeleteSequence( m_pParticle[i] );
            m_pParticle[i] = NULL;
        }
    }
}

void KGCEclipseOldBooks::ActionPerformed( const KActionEvent& event )
{
    for( int i = 0; i < MAX_OLD_BOOKS_NUM; i++ ) {
        if( event.m_pWnd == m_pkOldBooks[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {

            std::map< int, KBookData >::iterator mit = m_mapOldBooksData.find( i );
            if( mit != m_mapOldBooksData.end() ) {
                m_pkBookForm->ToggleRender( true );

                m_pkBookForm->SetCurrentPage(i);
                m_pkBookForm->ShowBookPage( mit->second.m_strDesc, mit->second.m_strAudio, mit->second.m_strImgFileName );
            }                
        }
    }

    if( event.m_pWnd == m_pkBtnReadBook && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        m_pkBookForm->ToggleRender( true );
        m_pkBookForm->ShowBookAllPage();
    }

    _GCWND_MSG_MAP( m_pkBtnClose,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    _GCWND_MSG_MAP( m_pkBtnCombination,		KD3DWnd::D3DWE_BUTTON_CLICK, OnCombination );
}

void KGCEclipseOldBooks::PostChildDraw( void )
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

        g_ParticleManager->Render( GC_LAYER_GACHA );     
    }
    g_pGCDeviceManager2->PopState();
}

void KGCEclipseOldBooks::FrameMoveInEnabledState( void )
{
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    if( m_pkBookForm->IsRenderOn() == false &&
        m_pkImgRewardItem->CheckPosInWindowBound( vMousePos ) ) {
        GCItem* kItem = NULL;
        kItem = g_pItemMgr->GetItemData( m_dwRewardItemID );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( m_pkImgRewardItem->GetCurrentWindowPos() );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
    }

    if( m_iParticleLife > 0 ) {
        m_iParticleLife--;

        if( m_iParticleLife == 45 ) {
            m_pkAllAcquiredOldBook->ToggleRender( true );
        }
        else if( m_iParticleLife == 0 ) {            
            KP2P::GetInstance()->Send_EclipseCollectRewardReq();
            g_MyD3D->WaitForServerAck( Result_ReceiveChristmasReward, INT_MAX, 1000, -1 );
        }
    }

    if( m_pkBookForm->IsRenderOn() ) {
        ShowCompetionEffect( false );
    } else {
        ShowCompetionEffect( m_bAllAcquired );
    }
}


void KGCEclipseOldBooks::CheckAcquiredOldBooks()
{
    if( m_bAllAcquired ) {
        m_pkAllAcquiredOldBook->ToggleRender( true );
        m_pkBtnCombination->ToggleRender( false );
        m_pkBtnReadBook->ToggleRender( true );
        for( int i = 0; i < MAX_OLD_BOOKS_NUM; i++ ) {
            m_pkOldBooks[i]->Lock( false );
        }
    } else {
        m_pkAllAcquiredOldBook->ToggleRender( false );
        m_pkBtnReadBook->ToggleRender( false );
        int iCheck = 0;
        // 인벤에서 획득한 고서가 있는지 검사한다.
        std::map< int, KBookData >::iterator mit;
        for( int i = 0; i < MAX_OLD_BOOKS_NUM; i++ ) {
            mit = m_mapOldBooksData.find( i );
            if( mit != m_mapOldBooksData.end() && g_pItemMgr->FindInventoryForItemID( mit->second.m_dwItemID ) ) {
                m_pkOldBooks[i]->Lock( false );
                iCheck++;
            } else {
                m_pkOldBooks[i]->Lock( true );
            }
        }

        if( iCheck == MAX_OLD_BOOKS_NUM ) {
            m_pkBtnCombination->Lock( false );
        } else {
            m_pkBtnCombination->Lock( true );
        }
    }    
}

void KGCEclipseOldBooks::OnClose()
{
    if( m_pkBookForm->IsRenderOn() ) {
        return;
    }

    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEclipseOldBooks::OnCombination()
{
    if( m_iParticleLife == 0 ) {
        m_iParticleLife = 100;
        ShowCombinationEffect();
    }
}

void KGCEclipseOldBooks::LoadOldBookData()
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "ui_sign.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "OldBooksData.lua", false) == false ||
        !m_mapOldBooksData.empty() )
    {
        return;
    }

    m_mapOldBooksData.clear();
    std::vector< KBookData > vecBookData;

    int scrollBackStyle, scrollPieceTextureIndex;
    std::string strAllAcquiredOldBook;

    LUA_GET_VALUE_DEF("ScrollBackStyle", scrollBackStyle, -1 );
    m_pkBookForm->SetBackStyle( scrollBackStyle );

    LUA_GET_VALUE_DEF("ScrollPieceTextureIndex", scrollPieceTextureIndex, -1 );

    if ( scrollPieceTextureIndex != -1 )
    {
        for( int i = 0; i < MAX_OLD_BOOKS_NUM; i++ ) {
            m_pkOldBooks[i]->SetWndTex( D3DWM_DEFAULT,  scrollPieceTextureIndex );
            m_pkOldBooks[i]->SetWndTex( D3DWM_HOVER,    scrollPieceTextureIndex );
            m_pkOldBooks[i]->SetWndTex( D3DWM_SELECT,   scrollPieceTextureIndex );
            m_pkOldBooks[i]->SetWndTex( D3DWM_ACTIVE,   scrollPieceTextureIndex );
            m_pkOldBooks[i]->SetWndTex( D3DWM_LOCK,     scrollPieceTextureIndex );
        }
    }

    LUA_GET_VALUE_DEF("AllAcquiredOldBook",  strAllAcquiredOldBook, "ui_oldbook_origin.dds" );
    m_pkAllAcquiredOldBook->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( strAllAcquiredOldBook.c_str() ) );

    LUA_BEGIN_TABLE("OldBooksData",return);
    {
        KBookData kBookData;
        int iIndex;
        int strDesc;
        int strAudio;
        std::string strImageFileName;

        for(int i = 1; ; i++)
        {
            LUA_BEGIN_TABLE(i,break)
            {
                LUA_GET_VALUE_DEF("Index",          iIndex, -1 );
                LUA_GET_VALUE_DEF("ItemID",         kBookData.m_dwItemID, -1 );
                LUA_GET_VALUE_DEF("Desc",           strDesc, 0 );
                LUA_GET_VALUE_DEF("ImageFileName",  strImageFileName, "" );
                LUA_GET_VALUE_DEF("Audio",          strAudio, -1 );

                kBookData.m_strDesc = strDesc;
                kBookData.m_strImgFileName = GCUTIL_STR::GCStrCharToWide( strImageFileName.c_str() );
                kBookData.m_strAudio = strAudio;

                m_mapOldBooksData.insert( std::make_pair( iIndex, kBookData ) );

                vecBookData.push_back( kBookData );
            }
            LUA_END_TABLE(return);
            
        }
    }
    LUA_END_TABLE(return);
    
    m_pkBookForm->SetBookFormData( vecBookData );
}

void KGCEclipseOldBooks::SetRewardItemInfo( std::vector<KDropItemInfo>& vecRewardItem_ )
{
    // 일단 보상이 현재는 한개고 UI도 그렇게 되 있어서 하나만 담겠음
    if( !vecRewardItem_.empty() ) {
        m_dwRewardItemID = vecRewardItem_[0].m_ItemID / 10;
    }

    m_pkImgRewardItem->SetItemInfo( m_dwRewardItemID );
}

void KGCEclipseOldBooks::ShowCombinationEffect()
{
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Puzzle_Shine_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_GACHA);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Puzzle_Shine_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_GACHA);    
}

void KGCEclipseOldBooks::SetCompetionEffect()
{
    std::stringstream stm;
   
    for( int i = 0; i < PARTICLE_NUM; i++ ) {        
        stm << "Puzzle_Shine_After_" << i + 1;
        if( m_pParticle[i] == NULL ) {
            m_pParticle[i] = g_ParticleManager->CreateSequence( stm.str(), 0.0f, 0.0f, 0.5f );
        }

        if( m_pParticle[i] != NULL ) {
            g_ParticleManager->SetLayer( m_pParticle[i] , GC_LAYER_GACHA);
        }

        stm.str("");
    }    
}

void KGCEclipseOldBooks::ShowCompetionEffect( bool bShow_ )
{
    for( int i = 0; i < PARTICLE_NUM; i++ ) {
        if( m_pParticle[i] != NULL ) {
            m_pParticle[i]->SetShow( bShow_ );
        }
    }
}