#include "GCCharNickNameChangeDlg.h"

IMPLEMENT_CLASSNAME( KGCCharNickNameChangeDlg );
IMPLEMENT_WND_FACTORY( KGCCharNickNameChangeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCharNickNameChangeDlg, "gc_char_nickname_change_dlg" );

KGCCharNickNameChangeDlg::KGCCharNickNameChangeDlg( void )
{
    m_pkEditNickname = NULL;
    LINK_CONTROL( "edit_nickname", m_pkEditNickname );

    m_pkBtnConfirm = NULL;
    LINK_CONTROL( "btn_ok", m_pkBtnConfirm );

    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkSpacialChar = NULL;
    LINK_CONTROL( "spacialchar", m_pkSpacialChar );

    m_pkCussWord = NULL;
    LINK_CONTROL( "cussword", m_pkCussWord );


    m_pkNicdiscrptionFirst = NULL;
    LINK_CONTROL( "nick_discrption1", m_pkNicdiscrptionFirst );

    m_pkNicdiscrptionSecond = NULL;
    LINK_CONTROL( "nick_discrption2", m_pkNicdiscrptionSecond );

    m_pkWhiteBackground = NULL;
    LINK_CONTROL( "background_white", m_pkWhiteBackground );

    m_iMode = -1;
    m_iCurrentCharType = -1;
    m_itemUID = 0;
}

KGCCharNickNameChangeDlg::~KGCCharNickNameChangeDlg( void )
{

}

void KGCCharNickNameChangeDlg::OnCreate( void )
{
    m_pkWhiteBackground->InitState( false, false, NULL );

    m_pkBtnConfirm->InitState( true, true, this );
    m_pkEditNickname->InitState( true,true,this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkNicdiscrptionFirst->InitState( true, true, this );
    m_pkNicdiscrptionSecond->InitState( true, true, this );

#ifdef NATION_KOREA
    m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE/2 );
#else
    m_pkEditNickname->SetLimitText(GC_NICKNAME_SIZE );
#endif 

    m_pkEditNickname->SetFontColor( D3DCOLOR_ARGB(255, 70, 78, 99) );
    LoadDefaultValue();

    m_pkBtnClose->SetHotKey(DIK_ESCAPE);
}

bool KGCCharNickNameChangeDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    SetMode( (int)kInfo_.m_lParam );
    SetCurrentType( (int)kInfo_.m_lParam2 );

    // 캐릭터 선택씬에서 열경우
    if( m_iMode == 0 )
    {
        BackGroundEnable( true );
    }
    else
        BackGroundEnable( false );

    return true;
}

void KGCCharNickNameChangeDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose,                    KD3DWnd::D3DWE_BUTTON_CLICK,    Onclose );
    GCWND_MSG_MAP( m_pkEditNickname,                KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditNick );
    GCWND_MSG_MAP( m_pkBtnConfirm,                  KD3DWnd::D3DWE_BUTTON_CLICK,    OnChageConfirm );
}

void KGCCharNickNameChangeDlg::FrameMoveInEnabledState( void )
{

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
}

void KGCCharNickNameChangeDlg::KeydownEditNick()
{
    std::wstring strCheckNewNickname = m_pkEditNickname->GetText();

    m_pkCussWord->ToggleRender(false);
    m_pkSpacialChar->ToggleRender(false);

    if( strCheckNewNickname.empty() )
    {
        SetNickChangeBtn( false );
        return;
    }

    if(!SiKSlangFilter()->FilterCheck( strCheckNewNickname, KSlangFilter::SLANG_FILTER_NICKNAME )) 
    {
        m_pkCussWord->ToggleRender(true);
        SetNickChangeBtn( false );
        return;
    }
    if(!SiKSlangFilter()->ValidCheckNickName( strCheckNewNickname)) 
    {
        m_pkSpacialChar->ToggleRender(true);
        SetNickChangeBtn( false );
        return;
    }

    SetNickChangeBtn( true );

}

void KGCCharNickNameChangeDlg::OnClickEditNick( void )
{
    m_pkEditNickname->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
    m_pkEditNickname->SetFocus();
}

void KGCCharNickNameChangeDlg::LoadDefaultValue( void )
{
    m_pkEditNickname->SetText(g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ).c_str()); 
    m_pkEditNickname->SetFocus( false );

    m_pkNicdiscrptionFirst->SetText(g_pkStrLoader->GetString( STR_ID_CHAR_NICK_STR1 ).c_str());
    m_pkNicdiscrptionFirst->SetAlign( DT_CENTER );
    m_pkNicdiscrptionSecond->SetText(g_pkStrLoader->GetString( STR_ID_CHAR_NICK_STR2 ).c_str());
    m_pkNicdiscrptionSecond->SetAlign( DT_CENTER );

    m_pkNicdiscrptionFirst->ToggleRender(true);
    m_pkNicdiscrptionSecond->ToggleRender(true);
    m_pkSpacialChar->ToggleRender( false );
    m_pkCussWord->ToggleRender( false );
}

void KGCCharNickNameChangeDlg::Onclose( void )
{
    m_itemUID = 0;
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCCharNickNameChangeDlg::OnChageConfirm( void )
{
    std::wstring strCharNewNickname  = m_pkEditNickname->GetText();

    if( strCharNewNickname == L"")
        return;

    Result_CharNickChange = INT_MAX;

    GCITEMID itemID = KGCItemManager::ITEM_CHAR_NICK_CHANGE_ITEM * 10;
    GCITEMUID itemUID = 0;

    if ( m_iMode == KEVENT_CHANGE_CHAR_NICKNAME_REQ::UT_USE_ITEM )
    {
        itemUID = m_itemUID;
    }


    KP2P::GetInstance()->Send_CharNickNameChageReq(m_iMode, m_iCurrentCharType, strCharNewNickname, itemID, itemUID );
    g_MyD3D->WaitForServerAck( Result_CharNickChange, INT_MAX, 30000, TIME_OUT_VALUE );

    if( Result_CharNickChange == 0 ) //성공하면 창 닫기
        Onclose();
}

void KGCCharNickNameChangeDlg::SetNickChangeBtn(  bool bRender )
{
    if( bRender )
    {
        m_pkBtnConfirm->Lock( false );
    }
    else
    {
        m_pkBtnConfirm->Lock( true );
    }
}

void KGCCharNickNameChangeDlg::BackGroundEnable( bool bRender /*= true */ )
{
    m_pkWhiteBackground->ForceSetWindowColor( D3DCOLOR_ARGB( 180, 0x00, 0x00, 0x00 ) );
    m_pkWhiteBackground->ToggleRender( bRender );
}

void KGCCharNickNameChangeDlg::OnDestroy()
{
    BackGroundEnable( false );
}
