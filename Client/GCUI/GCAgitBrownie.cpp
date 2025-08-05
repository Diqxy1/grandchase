#include "stdafx.h"
#include "GCAgitBrownie.h"

IMPLEMENT_CLASSNAME( KGCAgitBrownie );
IMPLEMENT_WND_FACTORY( KGCAgitBrownie );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitBrownie, "gc_agit_brownie" );

KGCAgitBrownie::KGCAgitBrownie( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;
    LINK_CONTROL( "btnGreeting",    m_apkBtn[ BTN_GREETING ] );
    LINK_CONTROL( "btnOption",      m_apkBtn[ BTN_OPTION ] );
    LINK_CONTROL( "btnMove",        m_apkBtn[ BTN_MOVE ] );
    LINK_CONTROL( "btnQuestion",    m_apkBtn[ BTN_QUESTION ] );
    LINK_CONTROL( "btnNext",        m_apkBtn[ BTN_NEXT ] );
    LINK_CONTROL( "btnClose",       m_apkBtn[ BTN_CLOSE ] );

    for( int i=0 ; i<NUM_STATIC ; ++i )
        m_apkStatic[i] = NULL;
    LINK_CONTROL( "staticGreeting", m_apkStatic[ STATIC_GREETING ] );
    LINK_CONTROL( "staticOption",   m_apkStatic[ STATIC_OPTION ] );
    LINK_CONTROL( "staticMove",     m_apkStatic[ STATIC_MOVE ] );
    LINK_CONTROL( "staticQuestion", m_apkStatic[ STATIC_QUESTION ] );
    LINK_CONTROL( "staticMent",     m_apkStatic[ STATIC_MENT ] );

    m_pkActiveFrame = NULL;
    m_pkImgBrownie = NULL;
    LINK_CONTROL( "activeFrame",    m_pkActiveFrame );
    LINK_CONTROL( "imgBrownie",     m_pkImgBrownie );

    m_iGreetingType = GREETING0;
    m_iCurSelBtn = BTN_GREETING;
    m_bGreeting = false;
    m_iGreetingString = 0;
    m_iGreetingFace = 0;
    m_iCurFace = -1;

    for( int i=0 ; i<NUM_PARTICLE ; ++i )
        m_apParticle[i] = NULL;

    for( int i=0 ; i<NUM_QUESTION ; ++i )
    {
        m_apkQuestion[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "staticQuestion%d", i );
        LINK_CONTROL( szTemp, m_apkQuestion[i] );
    }

    m_pkQuestionFrame = NULL;
    m_pkSelectQuestion = NULL;
    LINK_CONTROL( "guestionFrame", m_pkQuestionFrame );
    LINK_CONTROL( "questionSelect", m_pkSelectQuestion );
}

KGCAgitBrownie::~KGCAgitBrownie( void )
{
    ClearAllParticle();
}

void KGCAgitBrownie::OnCreate( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_STATIC ; ++i )
    {
        m_apkStatic[i]->InitState( true, false, NULL );
        m_apkStatic[i]->SetAlign( DT_LEFT );
    }

    m_apkStatic[ STATIC_MENT ]->SetMultiLine( true );
    m_apkStatic[ STATIC_MENT ]->SetShadow( false );

    m_pkActiveFrame->InitState( false, false, NULL );
    m_pkImgBrownie->InitState( true, false, NULL );

    for( int i=0 ; i<NUM_QUESTION ; ++i )
        m_apkQuestion[i]->InitState( true, true, this );
    m_pkQuestionFrame->InitState( false, false, NULL );
    m_pkSelectQuestion->InitState( false, false, NULL );

    LoadBrowniInfo();

    m_apkBtn[ BTN_CLOSE ]->SetHotKey( DIK_ESCAPE );
}

void KGCAgitBrownie::ActionPerformed( const KActionEvent& event )
{
    for( int i=0 ; i<BTN_NEXT ; ++i )
    {
        if( event.m_pWnd == m_apkBtn[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnMenuBtn( i );
    }

    for( int i=0 ; i<NUM_QUESTION ; ++i )
    {
        if( event.m_pWnd == m_apkQuestion[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnSelectQuestion( i );
        else if ( event.m_pWnd == m_apkQuestion[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
            ToggleActiveFrame( i );
    }

    GCWND_MSG_MAP( m_apkBtn[ BTN_NEXT ],        KD3DWnd::D3DWE_BUTTON_CLICK,    OnNextBtn );
    GCWND_MSG_MAP( m_apkBtn[ BTN_CLOSE ],       KD3DWnd::D3DWE_BUTTON_CLICK,    OnCloseBtn );
}

void KGCAgitBrownie::PostChildDraw( void )
{
    g_ParticleManager->Render( GC_LAYER_AZIT_NPC );
}

void KGCAgitBrownie::InitUI( void )
{
    for( int i=0 ; i<NUM_STATIC ; ++i )
        m_apkStatic[i]->SetText( L"" );
    m_pkActiveFrame->ToggleRender( false );
    
    m_bGreeting = false;
    m_iGreetingString = 0;
    m_iGreetingFace = 0;
    m_iCurFace = -1;

    // 메뉴들 
    m_apkStatic[ STATIC_OPTION ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_OPTION ) );
    m_apkStatic[ STATIC_MOVE ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_MOVE ) );
    m_apkStatic[ STATIC_QUESTION ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_QUESTION ) );   
    m_apkStatic[ STATIC_OPTION ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
    m_apkStatic[ STATIC_MOVE ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
    m_apkStatic[ STATIC_QUESTION ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );

    // 1번 매뉴는 랜덤 
    SetGreetingType();

    // 기본 얼굴 + 대사 
    m_bGreeting = false;
    SetFace( m_kBrownieGreeting.m_iNormalFaceIndex );
    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString(m_kBrownieGreeting.m_iNormalStringIndex) );

    m_iGreetingString = -1;
    m_iGreetingFace = -1;

    // 도우말 세팅
    SetQuestionInfo();
}

void KGCAgitBrownie::ClearAllParticle( void )
{
    for( int i=0 ; i<NUM_PARTICLE ; ++i )
        DeleteParticle(i);
}

bool KGCAgitBrownie::LoadBrowniInfo( void )
{
    // 스크립트에서 도우미 정보 로드 
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "AgitBrownieInfo.stg" ) == false )
        return false;

    if( SUCCEEDED( luaMgr.BeginTable( "AgitBrownieInfo" ) ) )
    {

        // 표정 정보 리스트 읽어오기 
        if( SUCCEEDED( luaMgr.BeginTable( "FaceList" ) ) )
        {
            int iFaceIndex = 1;
            while( true )
            {
                if( FAILED( luaMgr.BeginTable( iFaceIndex++ ) ) )
                    break;

                KBrownieFace tempFace;
                luaMgr.GetValue( "FaceIndex",   tempFace.m_iFaceIndex );

                if( SUCCEEDED( luaMgr.BeginTable( "ParticleList" ) ) ) 
                {
                    int iParticleIndex = 1;
                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iParticleIndex++ ) ) )
                            break;

                        KBrownieParticleInfo tempParticle;
                        LUA_GET_VALUE_DEF( 1, tempParticle.m_iPosIndex, 0 );
                        LUA_GET_VALUE_DEF( 2, tempParticle.m_strSquence, "" );
                        LUA_GET_VALUE_DEF( 3, tempParticle.m_dxvPos.x, 0.0f );
                        LUA_GET_VALUE_DEF( 4, tempParticle.m_dxvPos.y, 0.0f );

                        tempFace.m_vecParticle.push_back( tempParticle );
                        luaMgr.EndTable();  // 파티클하나 정보 끝 
                    }
                }
                luaMgr.EndTable();  // 파티클 리스트 끝

                m_vecFaceList.push_back( tempFace );
                luaMgr.EndTable();  // 표정 하나 정보 끝
            }
        }
        luaMgr.EndTable();  // 표정 리스트 끝

        // 도우미 정보 불러오기 
        // 일단 하나에만 저장함. 나중에 늘리시오 
        if( SUCCEEDED( luaMgr.BeginTable( "BrownieList" ) ) )
        {
            int iBrownieIndex = 1;
            while( true )
            {
                if( FAILED( luaMgr.BeginTable( iBrownieIndex++ ) ) )
                    break;

                luaMgr.GetValue( "NormalFace", m_kBrownieGreeting.m_iNormalFaceIndex  );
                luaMgr.GetValue( "NormalString", m_kBrownieGreeting.m_iNormalStringIndex );

                if( SUCCEEDED( luaMgr.BeginTable( "GreetingList" ) ) )
                {
                    int iGreetingIndex = 1;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iGreetingIndex++ ) ) )
                            break;

                        int iListIndex;
                        std::vector< std::pair< int, int > > vecTemp;
                        luaMgr.GetValue( "GreetingIndex",   iListIndex );

                        if( SUCCEEDED( luaMgr.BeginTable( "FaceInfo" ) ) )
                        {
                            int iFaceInfoIndex = 1;
                            while( true )
                            {
                                if( FAILED( luaMgr.BeginTable( iFaceInfoIndex++ ) ) )
                                    break;

                                std::pair<int, int> temp;
                                LUA_GET_VALUE_DEF( 1, temp.first, 0 );
                                LUA_GET_VALUE_DEF( 2, temp.second, 0 );

                                vecTemp.push_back( temp );
                                luaMgr.EndTable();
                            }
                        }
                        luaMgr.EndTable(); // 대화 정보 끝 
                        m_kBrownieGreeting.m_mapGreetingList[ iListIndex ] = vecTemp;
                        luaMgr.EndTable();  // 대화 하나 정보 끝 
                    }
                }
                luaMgr.EndTable();  // 대화  리스트 끝 
                luaMgr.EndTable();  // 도우미 하나 정보 끝 
            }
        }
        luaMgr.EndTable();  // 도우미 정보 리스트 끝

        // 도움말 정보 
        if( SUCCEEDED( luaMgr.BeginTable( "QuestionList" ) ) )
        {
            int iQuestIndex = 1;

            while( true )
            {
                if( FAILED( luaMgr.BeginTable( iQuestIndex++ ) ) )
                    break;

                KAgitQuestionInfo tempInfo;

                luaMgr.GetValue( "QuestionString",  tempInfo.m_iQuestionString );
                luaMgr.GetValue( "AnswerString",    tempInfo.m_iAnswerString );
                luaMgr.GetValue( "FaceIndex",       tempInfo.m_iFaceIndex );

                m_vecQuestionList.push_back( tempInfo );

                luaMgr.EndTable();  // 도움말 정보 하나 끝 
            }
        }
        luaMgr.EndTable();  // 도움말 정보 끝 
    }
    luaMgr.EndTable(); //전체 끝

    return true;
}

void KGCAgitBrownie::SetGreetingType( void )
{
    srand( time(  NULL ) );
    int iRate = rand() % 100;

    // 타입 정하고 
    if( 0 <= iRate && iRate < 40 )           // 40%
        m_iGreetingType = GREETING0;
    else if( 40 <= iRate && iRate < 60 )     // 20%
        m_iGreetingType = GREETING1;
    else if( 60 <= iRate && iRate < 80 )     // 20%
        m_iGreetingType = GREETING2;
    else if( 80 <= iRate && iRate < 90 )     // 10%
        m_iGreetingType = GREETING3;
    else                                     // 10%
        m_iGreetingType = GREETING4;

    // 메뉴 스트링 정한다. 
    switch( m_iGreetingType )
    {
    case GREETING0:
        m_apkStatic[ STATIC_GREETING ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_GREETING0 ) );
        break;

    case GREETING1:
        m_apkStatic[ STATIC_GREETING ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_GREETING1 ) );
        break;

    case GREETING2:
        m_apkStatic[ STATIC_GREETING ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_GREETING2 ) );
        break;

    case GREETING3:
        m_apkStatic[ STATIC_GREETING ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_GREETING3 ) );
        break;

    case GREETING4:
        m_apkStatic[ STATIC_GREETING ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_MENU_GREETING4 ) );
        break;
    }

    m_apkStatic[ STATIC_GREETING ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
}

void KGCAgitBrownie::OnMenuBtn( int iIndex_ )
{
    m_iCurSelBtn = iIndex_;

    // 버튼따라 할일 처리 
    switch( m_iCurSelBtn )
    {
    case BTN_GREETING:
        OnGreeting();
        break;

    case BTN_OPTION:
        OnOption();
        break;

    case BTN_MOVE:
        OnMove();
        break;

    case BTN_QUESTION:
        OnQuestion();
        break;
    }


    // 활성 버튼 프레임 
    D3DXVECTOR2 dxvPos = m_apkBtn[ m_iCurSelBtn ]->GetFixedWindowLocalPos();
    m_pkActiveFrame->SetWindowPos( dxvPos );
    m_pkActiveFrame->ToggleRender( true );
}

void KGCAgitBrownie::OnNextBtn( void )
{

}

void KGCAgitBrownie::OnCloseBtn( void )
{
    SpeakToActionListener( KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK) );
    ClearAllParticle();
    ToggleQuestion( false );
}

void KGCAgitBrownie::OnGreeting( void )
{
    if( false == m_bGreeting )
    {
        std::map< int, std::vector< std::pair<int, int> > >::iterator mit = m_kBrownieGreeting.m_mapGreetingList.find( m_iGreetingType );   
        if( mit == m_kBrownieGreeting.m_mapGreetingList.end() )
            return;

        int iStringCount = static_cast<int>( mit->second.size() );

        srand( time(NULL) );
        int iStringIndex = rand() % iStringCount;

        m_iGreetingString = mit->second[ iStringIndex ].second;
        m_iGreetingFace = mit->second[ iStringIndex ].first;
        m_bGreeting = true;
    }

    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString( m_iGreetingString ) );

    if( m_iCurFace != m_iGreetingFace )
        SetFace( m_iGreetingFace );

    g_pkAgitOverlayUI->ShowAgitOption( false );
    g_pkAgitOverlayUI->ShowAgitMove( false );
    ToggleQuestion( false );
}

void KGCAgitBrownie::OnOption( void )
{
    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_OPTION_MENT ) );
    SetFace( m_kBrownieGreeting.m_iNormalFaceIndex );
    
    g_pkAgitOverlayUI->ShowAgitOption( true );
    g_pkAgitOverlayUI->ShowAgitMove( false );
    ToggleQuestion( false );
}

void KGCAgitBrownie::OnMove( void )
{
    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_AGIT_MOVE_MENT ) );
    SetFace( m_kBrownieGreeting.m_iNormalFaceIndex );

    g_pkAgitOverlayUI->ShowAgitOption( false );
    ToggleQuestion( false );

    g_pkAgitOverlayUI->ShowAgitMove( true );
}

void KGCAgitBrownie::OnQuestion( void )
{
    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_AGIT_BROWNIE_QEUSTION_MENT ) );
    SetFace( m_kBrownieGreeting.m_iNormalFaceIndex );
    
    g_pkAgitOverlayUI->ShowAgitOption( false );
    g_pkAgitOverlayUI->ShowAgitMove( false );
    ToggleQuestion( !m_pkQuestionFrame->IsRenderOn() );
}

void KGCAgitBrownie::AddParticle( std::string& strSquence_, float fPosX_, float fPosY_, int iParticleIndex_ )
{
    DeleteParticle( iParticleIndex_ );

    //float fX = 0.0f;
    //float fY = 0.0f;
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "PosX", fX, 0.0f );
    //LUA_GET_VALUE_DEF( "PosY", fY, 0.0f );

    m_apParticle[ iParticleIndex_ ] = g_ParticleManager->CreateSequence( strSquence_, fPosX_, fPosY_, 0.5f );
    g_ParticleManager->SetLayer( m_apParticle[ iParticleIndex_ ], GC_LAYER_AZIT_NPC );
}

void KGCAgitBrownie::DeleteParticle( int iParticleIndex_ )
{
    g_ParticleManager->DeleteSequence( m_apParticle[ iParticleIndex_ ] );
    m_apParticle[ iParticleIndex_ ] = NULL;
}

void KGCAgitBrownie::SetFace( int iFaceIndex_ )
{
    if( iFaceIndex_ == m_iCurFace )
        return;

    if( iFaceIndex_ < FACE_NORMAL || NUM_FACE <= iFaceIndex_ )
        return;
    
    ClearAllParticle();
    for( std::vector< KBrownieFace >::iterator it = m_vecFaceList.begin() ; it != m_vecFaceList.end() ; ++it )
    {
        if( it->m_iFaceIndex == iFaceIndex_ )
        {
            for( std::vector< KBrownieParticleInfo >::iterator vit = it->m_vecParticle.begin() ; vit != it->m_vecParticle.end() ; ++vit )
                AddParticle( vit->m_strSquence, vit->m_dxvPos.x, vit->m_dxvPos.y, vit->m_iPosIndex );

            break;
        }
    }

    m_iCurFace = iFaceIndex_;
}

void KGCAgitBrownie::ToggleQuestion( bool bShow_ )
{
    for( int i=0 ; i<NUM_QUESTION ; ++i )
        m_apkQuestion[i]->ToggleRender( bShow_ );

    m_pkQuestionFrame->ToggleRender( bShow_ );
    m_pkQuestionFrame->SetAllWindowAlpha( 180 );
    m_pkSelectQuestion->ToggleRender( false );
}

void KGCAgitBrownie::SetQuestionInfo( void )
{
    for( int i=0 ; i<NUM_QUESTION ; ++i )
    {
        m_apkQuestion[i]->SetText( g_pkStrLoader->GetString( m_vecQuestionList[i].m_iQuestionString ) );
        m_apkQuestion[i]->ToggleRender( false );
    }
}

void KGCAgitBrownie::OnSelectQuestion( int iIndex_ )
{
    m_apkStatic[ STATIC_MENT ]->SetTextAutoMultiline( g_pkStrLoader->GetString( m_vecQuestionList[iIndex_].m_iAnswerString ) );
    SetFace( m_vecQuestionList[iIndex_].m_iFaceIndex );
}

void KGCAgitBrownie::ToggleActiveFrame( int iIndex_ )
{
    D3DXVECTOR2 dxvStaticPos = m_apkQuestion[iIndex_]->GetFixedWindowLocalPos();
    D3DXVECTOR2 dxvSelectPos = m_pkSelectQuestion->GetFixedWindowLocalPos();

    m_pkSelectQuestion->SetWindowPos( D3DXVECTOR2( dxvSelectPos.x, dxvStaticPos.y - 5.0f ) );
    m_pkSelectQuestion->ToggleRender( true );
}

