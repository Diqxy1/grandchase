#include "stdafx.h"
#include "GCNewCharacterSelect.h"

IMPLEMENT_CLASSNAME( KGCNewCharacterSelect );
IMPLEMENT_WND_FACTORY( KGCNewCharacterSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCNewCharacterSelect, "gc_new_character_select" );

KGCNewCharacterSelect::KGCNewCharacterSelect( void )
: m_pkSboxBackground(NULL)
, m_pkImgSelectCharacter(NULL)
, m_pkSboxBackgroundCaption(NULL)
, m_pkEditNickname(NULL)
, m_pkBtnNicknameConfirm(NULL)
, m_pkListboxDesc(NULL)
, m_pkScrollDesc(NULL)
, m_pkStaticComment(NULL)
, m_pkSboxEditNicknameBackground(NULL)
, m_nViewIndexOfCurrent(-1)
, m_iTopIndex(0)
, m_nOpacity(0)
, m_bAlphaAnimate(false)
, m_nViewIndexOfBefore(0)
, m_bMovingAnimate(false)
, m_nMovingFrame(0)
, m_pkStaticTooltipNameInsert(NULL)
, m_pkImgTooltipNameInsert(NULL)
, m_pkStaticCharacterDifficult(NULL)
, m_pkStaticNicknameStr(NULL)
, m_iSelectCharNum(0)
, m_fFixPosX(0.0f)
, m_pkStaticWeaponTypeStr(NULL)
#if defined (TOOLTIP_FOR_SUPPORT_NEW_CHARACTER)
, m_pkMyStateBackground(NULL)
, m_pkStaticMyState(NULL)
#endif
{

    for(int iLoop = 0; iLoop < NUM_MAX_CHARACTER; ++iLoop)
    {
        m_pkBtnCharSmall[iLoop] = NULL;
        m_pkBtnCharSmallOver[iLoop] = NULL;
        m_pkImgCharBig[iLoop] = NULL; 
        m_pkImgCharName[iLoop] = NULL;
        m_pkImgWeaponType[iLoop] = NULL;
    }

    LINK_CONTROL( "background",		        m_pkSboxBackground             );
    LINK_CONTROL( "img_select_character_mark",   m_pkImgSelectCharacter      );

    char szTemp[256];
    for(int iLoop = 0; iLoop < NUM_MAX_CHARACTER; ++iLoop)
    {
        sprintf(szTemp, "btn_char_small_0%d", iLoop);   LINK_CONTROL( szTemp, m_pkBtnCharSmall[iLoop]   );
        sprintf(szTemp, "btn_char_small_0%d_over", iLoop);   LINK_CONTROL( szTemp, m_pkBtnCharSmallOver[iLoop]   );
        sprintf(szTemp, "img_char_big_0%d", iLoop);     LINK_CONTROL( szTemp, m_pkImgCharBig[iLoop]     );
        sprintf(szTemp, "img_char_name_0%d", iLoop);   LINK_CONTROL( szTemp, m_pkImgCharName[iLoop]    );
        sprintf(szTemp, "img_weapon_type0%d", iLoop);  LINK_CONTROL( szTemp, m_pkImgWeaponType[iLoop]  );
    }

    LINK_CONTROL( "background_caption",             m_pkSboxBackgroundCaption           );
    LINK_CONTROL( "edit_nickname",                  m_pkEditNickname                );
    LINK_CONTROL( "btn_nickname_confirm",           m_pkBtnNicknameConfirm          );
    LINK_CONTROL( "listbox_desc",                   m_pkListboxDesc                 );
    LINK_CONTROL( "scroll_desc",                    m_pkScrollDesc                  );
    LINK_CONTROL( "static_comment",                 m_pkStaticComment               );
    LINK_CONTROL( "img_edit_nickname_background",   m_pkSboxEditNicknameBackground  );
    LINK_CONTROL( "static_tooltip_name_insert",     m_pkStaticTooltipNameInsert     );
    LINK_CONTROL( "img_tooltip_name_insert",        m_pkImgTooltipNameInsert        );
    LINK_CONTROL( "static_nickname_str",            m_pkStaticNicknameStr        );
    LINK_CONTROL( "static_weapon_type_str",         m_pkStaticWeaponTypeStr        );

    LINK_CONTROL( "static_difficult",         m_pkStaticCharacterDifficult        );
    for( int i=0 ; i<NUM_CHAR_DIFFCULT ; ++i ) { 
        m_apkIconDifficult[i] = NULL;
        char szTemp[128] = { 0, };

        sprintf_s( szTemp, 127, "icon_diff%d", i );
        LINK_CONTROL( szTemp, m_apkIconDifficult[i] );
    }

#if defined( TOOLTIP_FOR_SUPPORT_NEW_CHARACTER )
    LINK_CONTROL( "background_my_state",            m_pkMyStateBackground        );
    LINK_CONTROL( "my_state_string",            m_pkStaticMyState        );
#endif
}


KGCNewCharacterSelect::~KGCNewCharacterSelect( void )
{

}

void KGCNewCharacterSelect::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnNicknameConfirm,          KD3DWnd::D3DWE_BUTTON_CLICK,    OnConfirmNickName );    
    GCWND_MSG_MAP( m_pkEditNickname,                KD3DWnd::D3DWE_EDIT_RETURN,     OnConfirmNickName );
    GCWND_MSG_MAP( m_pkEditNickname,                KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditNick );

    for( int i = 0 ; i < m_iSelectCharNum ; ++i ) {
        if( m_pkBtnCharSmall[i] == NULL )
            continue;

#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
#endif

        if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && m_pkBtnCharSmall[i] == event.m_pWnd ) {
            ClickCharacterBtn( (EGCSelectCharType)i );
        }
    }

    if( event.m_pWnd == m_pkScrollDesc || event.m_pWnd == m_pkListboxDesc )
    {
        OnScroll();
    }
}

void KGCNewCharacterSelect::KeydownEditNick()
{   
    // 얻은 글자에 특수글자가 포함되어 있으면 툴팁을 표시한다.

#ifdef NATION_KOREA   

    std::wstring strCheckNewNickname = m_pkEditNickname->GetText();  //글자를 얻는다.  

	m_pkBtnNicknameConfirm->Lock(false);
    m_pkStaticTooltipNameInsert->ToggleRender(false);
    m_pkImgTooltipNameInsert->ToggleRender(false);

    if( strCheckNewNickname.size() == 0 )
        return;

    if(!SiKSlangFilter()->FilterCheck( strCheckNewNickname, KSlangFilter::SLANG_FILTER_NICKNAME )) {
        m_pkStaticTooltipNameInsert->ToggleRender(true);
        m_pkImgTooltipNameInsert->ToggleRender(true);   
        m_pkStaticTooltipNameInsert->SetText(g_pkStrLoader->GetString( STR_ID_INCLUDE_SlANG_WOARD ).c_str());
		m_pkBtnNicknameConfirm->Lock(true);
    }
    if(!SiKSlangFilter()->ValidCheckNickName( strCheckNewNickname)) {
        m_pkStaticTooltipNameInsert->ToggleRender(true);
        m_pkImgTooltipNameInsert->ToggleRender(true);   
        m_pkStaticTooltipNameInsert->SetText(g_pkStrLoader->GetString( STR_ID_INCLUDE_DISTINCT_WOARD ).c_str());
		m_pkBtnNicknameConfirm->Lock(true);
    }
#endif

}

void KGCNewCharacterSelect::OnClickEditNick()
{
    m_pkEditNickname->SetText(L"");
    m_pkEditNickname->SetFocus();
}

void KGCNewCharacterSelect::ClickCharacterBtn( EGCSelectCharType _character )
{
    if ( m_bAlphaAnimate || m_nViewIndexOfCurrent == _character ) {
        return;
    }
    m_nViewIndexOfBefore = m_nViewIndexOfCurrent;
    m_nViewIndexOfCurrent = (int)_character;
    UpdateBtnState( _character );
    UpdateSelectCharacterInformation(_character);
    g_KDSound.Play( "MP" );
}

void KGCNewCharacterSelect::UpdateBtnState( EGCSelectCharType _character )
{   
    //m_nViewIndexOfCurrent의 이미지를 over이미지로 만들어라. 나머지는 보통상태.
    for ( int i = 0 ; i < m_iSelectCharNum ; ++i )
    {
#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_PHILIPPINE)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
#endif
        if( i == _character ){
            m_pkBtnCharSmallOver[i]->ToggleRender(true);
        }
        else {
            m_pkBtnCharSmallOver[i]->ToggleRender(false);
        }
    }
}

void KGCNewCharacterSelect::UpdateBtnPosition()
{
    for ( int i = 0 ; i < NUM_MAX_CHARACTER ; ++i )
    {

#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_PHILIPPINE)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
#endif
        m_pkBtnCharSmall[i]->SetWindowPos(m_vtFixedWindowPos[m_nIndexOfCharacterPos[i]]);
        m_pkBtnCharSmallOver[i]->SetWindowPos(m_vtFixedWindowPos[m_nIndexOfCharacterPos[i]]);

        if ( m_nIndexOfCharacterPos[i] < NUM_VIEW_CHARACTER ) {
            m_pkBtnCharSmall[i]->ToggleRender(true);
        }
        else {
            m_pkBtnCharSmall[i]->ToggleRender(false);
        }
    }
}


void KGCNewCharacterSelect::OnScroll()
{
    int iTop = m_pkScrollDesc->GetScrollPos();
    if ( m_pkListboxDesc->GetTopIndex() != iTop )
    {
        g_KDSound.Play( "73" );
        m_pkListboxDesc->SetTopIndex( iTop );
    }
}



void KGCNewCharacterSelect::OnCreate( void )
{    
    //if( m_pkBtnCharSmall[CHAR_LIME]->IsRenderOn() )
    //{
    //    m_iSelectCharNum = 7;
    //    m_fFixPosX = 0.0f;
    //}
    if( m_pkBtnCharSmall[CHAR_RIN]->IsRenderOn() )
    {
        m_iSelectCharNum = 6;
        m_fFixPosX = 0.0f;
    }
    else if( m_pkBtnCharSmall[CHAR_LUPUS]->IsRenderOn() )
    {
        m_iSelectCharNum = 5;
        m_fFixPosX = 0.0f;
    }
    else if( m_pkBtnCharSmall[CHAR_SIEG]->IsRenderOn() )
    {
        m_iSelectCharNum = 4;
        m_fFixPosX = 0.0f;
    }
    else
    {
        m_iSelectCharNum = 3;
        m_fFixPosX = (float)m_pkBtnCharSmall[CHAR_ELESIS]->GetWidth()/2.0f;
        m_pkBtnCharSmall[CHAR_LUPUS]->InitState( false );
        m_pkBtnCharSmallOver[CHAR_LUPUS]->InitState( false );
        m_pkBtnCharSmall[CHAR_SIEG]->InitState( false );
        m_pkBtnCharSmallOver[CHAR_SIEG]->InitState( false );
    }

    for( int i = 0; i <  m_iSelectCharNum; ++i)
    {
#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_PHILIPPINE)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
#endif
        m_pkBtnCharSmall[i]->InitState( true, true, this );
        m_pkBtnCharSmallOver[i]->InitState( false );
    }

    InitButtonPos();

#if defined ( NATION_KOREA ) || defined (NATION_BRAZIL) || defined(NATION_PHILIPPINE)
    ChangeCharPosition();
#endif 

    m_pkEditNickname->InitState( true, true, this );

    m_pkListboxDesc->InitState( true, false, this ); 
    m_pkListboxDesc->Clear();    
    m_pkListboxDesc->SetLineSpace( 1.0f );   //1.2
    m_pkStaticComment->ToggleRender( true );

    m_pkBtnNicknameConfirm->InitState( true, true, this );  
    m_pkSboxEditNicknameBackground->InitState(true); 
    m_pkSboxBackground->InitState( true );       
    m_pkSboxBackgroundCaption->InitState( true );    

    m_pkImgSelectCharacter->ToggleRender( true );

    m_pkScrollDesc->InitState( true, true, this );

    for( int i = 0 ; i < NUM_MAX_CHARACTER ; ++i ) {
        m_pkImgCharBig[i]->ToggleRender( false );
        m_pkImgCharName[i]->ToggleRender( false );
        m_pkImgWeaponType[i]->ToggleRender( false );
    }

    for( int i=0 ; i<NUM_CHAR_DIFFCULT ; ++i ) { 
        m_apkIconDifficult[i]->InitState( false );
    }

    EGCSelectCharType nDefault = CHAR_ELESIS;
#if defined ( NATION_KOREA ) || defined (NATION_BRAZIL)

    nDefault = CHAR_RIN;
    //nDefault = CHAR_ASIN; 
    //nDefault = CHAR_LIME;
#elif defined (NATION_BRAZIL)
    nDefault = CHAR_RIN; 
#endif 

    m_nViewIndexOfCurrent = nDefault;
    UpdateSelectCharacterInformation( nDefault );
    UpdateBtnState( nDefault );

    m_pkStaticComment->SetFontColor(D3DCOLOR_ARGB(255, 121, 136, 148));
    m_pkStaticComment->SetAlign( DT_CENTER );
    m_pkStaticComment->SetText( g_pkStrLoader->GetString( STR_ID_OTHER_CHARACTER_PLAYABLE ));
    m_pkEditNickname->SetText(g_pkStrLoader->GetString( STR_ID_ENTER_YOUR_NICKNAME ).c_str());

    m_pkStaticComment->SetAlign(DT_RIGHT);

#ifdef NATION_KOREA
    m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE/2 );
#else
    m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE );
#endif 

    m_pkStaticTooltipNameInsert->SetFontColor(D3DCOLOR_ARGB(255, 144, 158, 169));

    m_pkStaticTooltipNameInsert->ToggleRender( false );
    m_pkImgTooltipNameInsert->ToggleRender( false );

    m_pkStaticCharacterDifficult->ToggleRender( false );
    m_pkStaticWeaponTypeStr->ToggleRender( false );

#if defined( NEW_CHARACTER_SELECT_BOX_NEW ) 
    m_pkStaticCharacterDifficult->ToggleRender( true );
    m_pkStaticCharacterDifficult->SetText(g_pkStrLoader->GetString( STR_ID_CHARACTER_DIFFICULT ));
    m_pkStaticCharacterDifficult->SetFontColor( D3DCOLOR_ARGB(255, 177, 212, 229) );       
    m_pkStaticCharacterDifficult->SetFontOutline( true, D3DCOLOR_ARGB(255, 6, 17, 28) );
    m_pkStaticCharacterDifficult->SetFontSpace( 1 );
#else
    m_pkStaticWeaponTypeStr->ToggleRender( true );
    m_pkStaticWeaponTypeStr->SetText(g_pkStrLoader->GetString( STR_ID_WEAPON_TYPE ));
    m_pkStaticWeaponTypeStr->SetFontColor( D3DCOLOR_ARGB(255, 177, 212, 229) );       
    m_pkStaticWeaponTypeStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 6, 17, 28) );
    m_pkStaticWeaponTypeStr->SetFontSpace( 1 );
#endif
    

    

    m_pkStaticNicknameStr->ToggleRender( true );
    m_pkStaticNicknameStr->SetText(g_pkStrLoader->GetString( STR_ID_NICKNAME ));
    m_pkStaticNicknameStr->SetFontColor( D3DCOLOR_ARGB(255, 177, 212, 229) );       
    m_pkStaticNicknameStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 6, 17, 28) );
    m_pkStaticNicknameStr->SetFontSpace( 1 );

#if defined( TOOLTIP_FOR_SUPPORT_NEW_CHARACTER )
    if ( m_pkStaticMyState ) {
        m_pkStaticMyState->InitState( true );
        m_pkStaticMyState->SetLineSpace( 1.0f );
        m_pkStaticMyState->SetAlign( DT_CENTER );
        m_pkStaticMyState->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_NEW_CHARACTER_TOOLTIP) );
    }
#endif
}

void KGCNewCharacterSelect::UpdateSelectCharacterInformation( EGCSelectCharType _character )
{
    m_nCharacterType = _character;

    if( m_bAlphaAnimate ) {
        //아직 에니메이션 중이므로 에니메이션을 끝낸다.
        for( int i = 0 ; i < m_iSelectCharNum ; ++i ) {

#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_PHILIPPINE)
            if ( i == CHAR_LUPUS || i == CHAR_SIEG)
                continue;
#endif
            if( !(_character == i || m_nViewIndexOfBefore == i) ) {
                m_pkImgCharBig[i]->ToggleRender( false );
                m_pkImgCharName[i]->ToggleRender( false );
            }
        }
    }

    m_bAlphaAnimate = true;

    for( int i = 0 ; i < m_iSelectCharNum ; ++i ) {

#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_PHILIPPINE)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
#endif
        if( _character == i ) {
            //알파 에니메이션 시작 플래그를 true로 설정한다.
            m_nOpacity = 0;

            m_pkImgCharBig[i]->ToggleRender( true );
            m_pkImgCharName[i]->ToggleRender( true );
#if !defined( NEW_CHARACTER_SELECT_BOX_NEW ) 
            m_pkImgWeaponType[i]->ToggleRender( true );
#endif
        }
#if !defined( NEW_CHARACTER_SELECT_BOX_NEW )
        else {
            m_pkImgWeaponType[i]->ToggleRender( false );
        }
#endif
    }

    m_pkListboxDesc->Clear();
    switch(_character) {
        case CHAR_ELESIS:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_ELESIS_INTRODUCE ));
            break;
        case CHAR_LIRE:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_LIRE_INTRODUCE ));
            break;
        case CHAR_ARME:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_ARME_INTRODUCE ));
            break;
        case CHAR_SIEG:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_SIEG_INTRODUCE ));
            break;            
        case CHAR_LUPUS:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_LUPUS_INTRODUCE ));
            break;  
        case CHAR_RIN:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_RIN_INTRODUCE ) );
            break;
        //case CHAR_ASIN:
        //    m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_ASIN1_PROMOTION_DESC ) );
        //    break;
        //case CHAR_LIME:
        //    m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_LIME_INTRODUCE ) );
        //    break;
        default:
            m_pkListboxDesc->AddString( g_pkStrLoader->GetString( STR_ID_ELESIS_INTRODUCE ));
            break;
    }

#if defined( NEW_CHARACTER_SELECT_BOX_NEW )
    SetDiffcultIcon( _character );
#endif

    m_pkListboxDesc->SetTopIndex( 0 );
    ResetScrolPos();

    //Select마크를 선택한 캐릭터가 있는 곳으로 이동 시킨다.
    SetSelectMark(_character);

}


void KGCNewCharacterSelect::ResetScrolPos()
{
    m_pkScrollDesc->SetScrollPageSize( m_pkListboxDesc->GetDisplayLimitLine() );
    m_pkScrollDesc->SetScrollRangeMax( m_pkListboxDesc->GetCount() );
    m_iTopIndex = m_pkListboxDesc->GetTopIndex();
    m_pkScrollDesc->SetScrollPos( m_iTopIndex );
}

void KGCNewCharacterSelect::OnCreateComplete( void )
{
    ResetScrolPos();

    m_strNickName.erase();
}


void KGCNewCharacterSelect::InitButtonPos()
{    
    //3개의 캐릭터중 처음에 있는 캐릭터 위치 기준으로 다음 캐릭터들의 위치를 정렬한다.
    DWORD dwGap = BTN_GAP_WIDTH;
    DWORD dwWidth = m_pkBtnCharSmall[CHAR_ELESIS]->GetWidth();
    static D3DXVECTOR2 vtFixedWindowLocalPos = m_pkBtnCharSmall[CHAR_ELESIS]->GetFixedWindowLocalPos();
    D3DXVECTOR2 vtSelectUIPos = m_pkImgSelectCharacter->GetFixedWindowLocalPos();

    m_vtOffsetOfSelectUI.x = vtSelectUIPos.x - vtFixedWindowLocalPos.x;
    m_vtOffsetOfSelectUI.y = vtSelectUIPos.y - vtFixedWindowLocalPos.y;

    D3DXVECTOR2 vtTempPos;
    vtFixedWindowLocalPos.x += m_fFixPosX;
    m_vtFixedWindowPos[CHAR_ELESIS] = vtFixedWindowLocalPos;


    vtTempPos = vtFixedWindowLocalPos;

    for( int i = 0 ; i < m_iSelectCharNum ; ++i ) {
#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN)
        if ( i == CHAR_LUPUS || i == CHAR_SIEG)
            continue;
        if (i == CHAR_RIN) {
            int iTemp = 3;
            vtTempPos.x = (vtFixedWindowLocalPos.x + (float)(dwWidth + dwGap) * iTemp);
        }
        else
        {
            vtTempPos.x = (vtFixedWindowLocalPos.x + (float)(dwWidth + dwGap) * i );
        }
#else
    vtTempPos.x = ( vtFixedWindowLocalPos.x + (float)(dwWidth + dwGap) * i );
#endif
        


        if ( i < NUM_VIEW_CHARACTER ) {
            m_vtFixedWindowPos[i] = vtTempPos;
            m_pkBtnCharSmall[i]->ToggleRender(true);
        }
        else {
            m_vtFixedWindowPos[i].x = -1000;
            m_pkBtnCharSmall[i]->ToggleRender(false);
        }
        m_nIndexOfCharacterPos[i] = i;
        m_pkBtnCharSmall[i]->SetWindowPos(m_vtFixedWindowPos[m_nIndexOfCharacterPos[i]]);
        m_pkBtnCharSmallOver[i]->SetWindowPos(m_vtFixedWindowPos[m_nIndexOfCharacterPos[i]]);
    }

    m_pkImgSelectCharacter->SetWindowPos(m_vtFixedWindowPos[m_nIndexOfCharacterPos[CHAR_ELESIS]]);
}

void KGCNewCharacterSelect::FrameMoveInEnabledState( void )
{
    // 키를 누르는 소리
    if( m_pkEditNickname->HasFocus())
    {
        for ( int i = 0; i < 256; i++ )
        {
            if ( g_pkInput->IsDown( i ) ){
                g_KDSound.Play( "71" );
                KeydownEditNick();
            }
        }
    }

    // 마우스 휠 돌릴때 스크롤 처리
    if ( 0 != g_pkInput->GetZMov() )
    {
        if( g_pkInput->GetZMov() > 0 )
        {
            if( --m_iTopIndex < 0 )
                ++m_iTopIndex;
            else
                g_KDSound.Play( "73" );

            m_pkScrollDesc->SetScrollPos(m_iTopIndex);
        }
        else 
        {
            if( ++m_iTopIndex > m_pkScrollDesc->GetScrollRangeMax() )
                --m_iTopIndex;
            else
                g_KDSound.Play( "73" );

            m_pkScrollDesc->SetScrollPos(m_iTopIndex);
        }

        OnScroll();
    }


    //알파 에니메이션 처리
    if ( m_bAlphaAnimate ) {
        if ( m_nOpacity <= 255 ) {
            m_nOpacity += ANIMATE_SPEED;
            if( m_nOpacity > 255 ) {
                if( m_nViewIndexOfCurrent != m_nViewIndexOfBefore ) {
                    m_pkImgCharBig[m_nViewIndexOfBefore]->ToggleRender( false );
                    m_pkImgCharName[m_nViewIndexOfBefore]->ToggleRender( false );
                }
                m_nOpacity = 255;
                m_bAlphaAnimate = false;
            }
            if( m_nViewIndexOfCurrent != m_nViewIndexOfBefore ) {
                m_pkImgCharBig[m_nViewIndexOfBefore]->SetAllWindowAlpha( 255 - m_nOpacity );
                m_pkImgCharName[m_nViewIndexOfBefore]->SetAllWindowAlpha( 255 - m_nOpacity );
            }

            m_pkImgCharBig[m_nViewIndexOfCurrent]->SetAllWindowAlpha( m_nOpacity );
            m_pkImgCharName[m_nViewIndexOfCurrent]->SetAllWindowAlpha( m_nOpacity );
           
        }
    }


    //툴팁 처리
#if defined (TOOLTIP_FOR_SUPPORT_NEW_CHARACTER)
    static int m_iAlphaCnt = 0;
    static int m_iSwitch = 8;

    m_iAlphaCnt += m_iSwitch;
    if( 255 <= m_iAlphaCnt || m_iAlphaCnt <= 0 )
    {
        m_iAlphaCnt = (255<=m_iAlphaCnt) ? 255:0;
        m_iSwitch *= -1;
    }

    if( m_pkStaticMyState->IsRenderOn() ) {
        m_pkStaticMyState->SetAllWindowAlpha( m_iAlphaCnt );
        m_pkMyStateBackground->SetAllWindowAlpha( m_iAlphaCnt );
    }
#endif
}


void KGCNewCharacterSelect::OnConfirmNickName()
{
    ::SetFocus(g_hUIMainWnd);
    m_strNickName = m_pkEditNickname->GetText();
    g_KDSound.Play( "31" );

    if( CheckValidNickname() == false )
        return;

    // 등록 확인창 띄움.
    std::wostringstream strmText;
    strmText << g_pkStrLoader->GetString( STR_ID_NICK_NAME_REG_AGREE1 ) << L"_";
    strmText << g_pkStrLoader->GetReplacedString( STR_ID_CHANGE_NICK_NAME_AGREE3 , "L" , m_strNickName );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,strmText.str(), L"",KGCUIScene::GC_MBOX_USE_NICK_NAME_AGREE, 0, 0, false, true );

}

bool KGCNewCharacterSelect::IsValidNickName()
{    
    if ( m_strNickName.find( L"#c" ) != std::string::npos || m_strNickName.find( L"#C" ) != std::string::npos )
        return false;
#if defined (NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH)
    if ( m_strNickName.empty() || g_pkFontMgr->wStringRealLength( m_strNickName.c_str() ) > GC_NICKNAME_SIZE )
        return false;
#else
    if ( m_strNickName.empty() || g_pkFontMgr->GetWidth( m_strNickName.c_str() ) > ( (12 / 2)*GC_NICKNAME_SIZE*m_fWindowScaleX ))
        return false;
#endif

    return true;
}


bool KGCNewCharacterSelect::CheckValidNickname()
{
    if ( IsValidNickName() == false )
    {
        if( m_strNickName.empty() )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        else
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    // 2008.02.16 Jaeho
    // 닉네임에 욕설이 포함되어 있는지 체크.
    if( !SiKSlangFilter()->FilterCheck( m_strNickName, KSlangFilter::SLANG_FILTER_NICKNAME ) )
    {
//         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
//             g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_CHAR ),
//             g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false , true );
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    // 2011.02.15 chanseon
    // 닉네임에 특수문자가 포함되어 있는지 체크.
    // 2012-08-20 서버와 같이 Nickname 같은 조건으로 체크 하도록 변경 (이전 함수는 쓰지 못하도록 주석처리) by Bishop
    if( !SiKSlangFilter()->ValidCheckNickName( m_strNickName ) )
    {
//         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
//             g_pkStrLoader->GetString( STR_ID_INVALID_NICKNAME ),
//             g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false , true );
        return false;
    }

    return true;
}


const std::wstring& KGCNewCharacterSelect::GetNickName( void ) const
{
    return m_strNickName;
}

int KGCNewCharacterSelect::GetSelectCharacterType()
{ 
    switch(m_nCharacterType)
    {
    case CHAR_ELESIS:
        m_nCharacterType = CHAR_ELESIS;
        break;
    case CHAR_LIRE:
        m_nCharacterType = CHAR_LIRE;
        break;
    case CHAR_ARME:
        m_nCharacterType = CHAR_ARME;
        break;
    case CHAR_SIEG:
        m_nCharacterType = CHAR_SIEG;
        break;
    case CHAR_LUPUS:
        m_nCharacterType = CHAR_LUPUS;
        break;
    case CHAR_RIN:
#if defined (NATION_KOREA)
        m_nCharacterType = CHAR_RIN;
#else
        m_nCharacterType = static_cast<int>(CHAR_RIN - 2);
#endif
        break;
    //case CHAR_ASIN:
    //    m_nCharacterType = CHAR_ASIN;
    //    break;
    //case CHAR_LIME:
    //    m_nCharacterType = CHAR_LIME;
    //    break;
    default:
        m_nCharacterType = CHAR_ELESIS;
        break;
    }
    return m_nCharacterType;
}

void KGCNewCharacterSelect::SetSelectMark(EGCSelectCharType _characte)
{
    if( _characte < m_iSelectCharNum ) {

        D3DXVECTOR2 vtTemp = m_vtFixedWindowPos[m_nIndexOfCharacterPos[_characte]];

        vtTemp.x += m_vtOffsetOfSelectUI.x;
        vtTemp.y += m_vtOffsetOfSelectUI.y;

        m_pkImgSelectCharacter->ToggleRender(true);
        m_pkImgSelectCharacter->SetWindowPos(vtTemp);
    }
    else {
        m_pkImgSelectCharacter->ToggleRender(false);
    }
}

void KGCNewCharacterSelect::SetDiffcultIcon( EGCSelectCharType charType_ )
{
    for( int i=0 ; i<NUM_CHAR_DIFFCULT ; ++i ) { 
        m_apkIconDifficult[i]->ToggleRender( false );
    }

    switch( charType_ ) { 
        case CHAR_ELESIS:
            m_apkIconDifficult[ CHAR_DIFFICULT1 ]->ToggleRender( true );
            break;
        case CHAR_LIRE:
            m_apkIconDifficult[ CHAR_DIFFICULT2 ]->ToggleRender( true );
            break;
        case CHAR_ARME:
            m_apkIconDifficult[ CHAR_DIFFICULT2 ]->ToggleRender( true );
            break;
        case CHAR_SIEG:
            m_apkIconDifficult[ CHAR_DIFFICULT1 ]->ToggleRender( true );
            break;
        case CHAR_LUPUS:
            m_apkIconDifficult[ CHAR_DEFFICULT0 ]->ToggleRender( true );
            break;
        case CHAR_RIN:
            m_apkIconDifficult[ CHAR_DIFFICULT1 ]->ToggleRender( true );
            break;
        //case CHAR_ASIN:
        //    m_apkIconDifficult[ CHAR_DEFFICULT0 ]->ToggleRender( true );
        //    break;
        //case CHAR_LIME:
        //    m_apkIconDifficult[ CHAR_DIFFICULT1 ]->ToggleRender( true );
        //    break;
        default:
            m_apkIconDifficult[ CHAR_DIFFICULT1 ]->ToggleRender( true );
            break;
    }
}

void KGCNewCharacterSelect::ChangeCharPosition()
{
    // 한국 순서 - 린 / 루퍼스 / 엘리 / 리르 / 아르 / 지크 
#if defined( NATION_KOREA )
    m_nIndexOfCharacterPos[CHAR_ELESIS] = CHAR_ARME;
    m_nIndexOfCharacterPos[CHAR_LIRE]   = CHAR_SIEG;
    m_nIndexOfCharacterPos[CHAR_ARME]   = CHAR_LUPUS;
    m_nIndexOfCharacterPos[CHAR_SIEG]   = CHAR_RIN;
    m_nIndexOfCharacterPos[CHAR_LUPUS]  = CHAR_LIRE;
    m_nIndexOfCharacterPos[CHAR_RIN]    = CHAR_ELESIS;

    //m_nIndexOfCharacterPos[CHAR_ELESIS] = CHAR_SIEG;
    //m_nIndexOfCharacterPos[CHAR_LIRE]   = CHAR_LUPUS;
    //m_nIndexOfCharacterPos[CHAR_ARME]   = CHAR_RIN;
    //m_nIndexOfCharacterPos[CHAR_SIEG]   = CHAR_ASIN;
    //m_nIndexOfCharacterPos[CHAR_LUPUS]  = CHAR_LIRE;
    //m_nIndexOfCharacterPos[CHAR_RIN]    = CHAR_ARME;
    //m_nIndexOfCharacterPos[CHAR_ASIN]    = CHAR_ELESIS;
#elif defined (NATION_BRAZIL)
	//  루퍼스랑 엘리시스 위치 바꾸기
	m_nIndexOfCharacterPos[CHAR_ELESIS] = CHAR_RIN;
    m_nIndexOfCharacterPos[CHAR_RIN] = CHAR_ELESIS;
#elif defined (NATION_PHILIPPINE)
    //  린이랑 지크하트 위치 바꾸기
    m_nIndexOfCharacterPos[CHAR_SIEG] = CHAR_RIN;
    m_nIndexOfCharacterPos[CHAR_RIN] = CHAR_SIEG;
#endif
    
   
    UpdateBtnPosition();
}