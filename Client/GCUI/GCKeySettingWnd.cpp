#include "stdafx.h"
#include "GCKeySettingWnd.h"
//
//

//
#include "../MyD3D.h"
#include "Controls.h"

#include "KDInput.h"
#include "utf16_cvt.h" // UnitCode 텍스트 파일을 만들기 위해.

IMPLEMENT_CLASSNAME( KGCKeySettingWnd );
IMPLEMENT_WND_FACTORY( KGCKeySettingWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCKeySettingWnd, "gc_key_setting_wnd" );


#define KEYTABLESIZE 71

static const int s_iKey[KEYTABLESIZE] =
{
    0x02, // 1
    0x03, // 2
    0x04, // 3
    0x05, // 4
    0x06, // 5
    0x07, // 6
    0x08, // 7
    0x09, // 8
    0x0A, // 9
    0x0B, // 0
    0x0C, // -
    0x0D, // =
    0x10, // Q
    0x11, // W
    0x12, // E
    0x13, // R
    0x14, // T
    0x15, // Y
    0x16, // U
    0x17, // I
    0x18, // O
    0x19, // P
    0x1A, // [
    0x1B, // ]
    0x1D, // Left Ctrl
    0x1E, // A
    0x1F, // S
    0x20, // D
    0x21, // F
    0x22, // G
    0x23, // H
    0x24, // J
    0x25, // K
    0x26, // L
    0x27, // ;
    0x28, // '
    0x2A, // Left Shift
    0x2B, // '\'
    0x2C, // Z
    0x2D, // X
    0x2E, // C
    0x2F, // V
    0x30, // B
    0x31, // N
    0x32, // M
    0x33, // ,
    0x34, // .
    0x35, // /
    0x36, // Right Shift
    0x37, // NUMPAD *
    0x38, // Left Alt
    0x39, // Space
    0x3A, // Caps
    0x47, // NUMPAD 7
    0x48, // NUMPAD 8
    0x49, // NUMPAD 9
    0x4B, // NUMPAD 4
    0x4C, // NUMPAD 5
    0x4D, // NUMPAD 6
    0x4F, // NUMPAD 1
    0x50, // NUMPAD 2
    0x51, // NUMPAD 3
    0x52, // NUMPAD 0
    0xC8, // Up
    0xCB, // Left
    0xCD, // Right
    0xD0, // Down

    0x4A, // NUMPAD -
    0x4E, // NUMPAD +
    0x53, // NUMPAD .
    0xB8, // Right Alt
};

std::vector<std::wstring> s_vecStrKey;


static const std::wstring s_strJoystickKey[EJK_MAX_NUM] =
{
    L"↑",
    L"↓",
    L"←",
    L"→",
    L"①",
    L"②",
    L"③",
    L"④",
    L"⑤",
    L"⑥",
    L"⑦",
    L"⑧",
    L"⑨",
    L"⑩",
    L"⑪",
    L"⑫",
    L"⑬",
};


KGCKeySettingWnd::KGCKeySettingWnd(void)
{
    for( int i = 0; i < NUM_USE_KEY; ++i )
    {
        m_pkKeyStr[i] = NULL;
        m_pkKey[i] = NULL;
        m_pkKeyBack[i] = NULL;

        m_pkGrith[i] = NULL;
    }

    m_pkDefaultKeyBtn = NULL;
    m_pkCustomKeyBtn = NULL;
    m_pkStatus = NULL;
    m_pkKeyboardRadio = NULL;
    m_pkjoystickRadio = NULL;

    m_pkKeyboardRadioStatic = NULL;
    m_pkjoystickRadioStatic = NULL;

    LINK_CONTROL( "key_up_static", m_pkKeyStr[0] );
    LINK_CONTROL( "key_down_static", m_pkKeyStr[1] );
    LINK_CONTROL( "key_left_static", m_pkKeyStr[2] );
    LINK_CONTROL( "key_right_static", m_pkKeyStr[3] );
    LINK_CONTROL( "key_attack_static", m_pkKeyStr[4] );
    LINK_CONTROL( "key_provocation_static", m_pkKeyStr[5] );
    LINK_CONTROL( "key_changeitem_static", m_pkKeyStr[6] );
    LINK_CONTROL( "key_useitem_static", m_pkKeyStr[7] );
    LINK_CONTROL( "key_changeskill_static", m_pkKeyStr[8] );
    LINK_CONTROL( "key_petskill_static", m_pkKeyStr[9] );
    LINK_CONTROL( "key_chargingskill_static", m_pkKeyStr[10] );

    LINK_CONTROL( "key_up", m_pkKey[0] );
    LINK_CONTROL( "key_down", m_pkKey[1] );
    LINK_CONTROL( "key_left", m_pkKey[2] );
    LINK_CONTROL( "key_right", m_pkKey[3] );
    LINK_CONTROL( "key_attack", m_pkKey[4] );
    LINK_CONTROL( "key_provocation", m_pkKey[5] );
    LINK_CONTROL( "key_changeitem", m_pkKey[6] );
    LINK_CONTROL( "key_useitem", m_pkKey[7] );
    LINK_CONTROL( "key_changeskill", m_pkKey[8] );
    LINK_CONTROL( "key_petskill", m_pkKey[9] );
    LINK_CONTROL( "key_chargingskill", m_pkKey[10] );

    LINK_CONTROL( "key_girth1", m_pkGrith[0] );
    LINK_CONTROL( "key_girth2", m_pkGrith[1] );
    LINK_CONTROL( "key_girth3", m_pkGrith[2] );
    LINK_CONTROL( "key_girth4", m_pkGrith[3] );
    LINK_CONTROL( "key_girth5", m_pkGrith[4] );
    LINK_CONTROL( "key_girth6", m_pkGrith[5] );
    LINK_CONTROL( "key_girth7", m_pkGrith[6] );
    LINK_CONTROL( "key_girth8", m_pkGrith[7] );
    LINK_CONTROL( "key_girth9", m_pkGrith[8] );
    LINK_CONTROL( "key_girth10", m_pkGrith[9] );
    LINK_CONTROL( "key_girth11", m_pkGrith[10] );

    LINK_CONTROL( "key_back1", m_pkKeyBack[0] );
    LINK_CONTROL( "key_back2", m_pkKeyBack[1] );
    LINK_CONTROL( "key_back3", m_pkKeyBack[2] );
    LINK_CONTROL( "key_back4", m_pkKeyBack[3] );
    LINK_CONTROL( "key_back5", m_pkKeyBack[4] );
    LINK_CONTROL( "key_back6", m_pkKeyBack[5] );
    LINK_CONTROL( "key_back7", m_pkKeyBack[6] );
    LINK_CONTROL( "key_back8", m_pkKeyBack[7] );
    LINK_CONTROL( "key_back9", m_pkKeyBack[8] );
    LINK_CONTROL( "key_back10", m_pkKeyBack[9] );
    LINK_CONTROL( "key_back11", m_pkKeyBack[10] );

    LINK_CONTROL( "key_default_button", m_pkDefaultKeyBtn );
    LINK_CONTROL( "key_custom_button", m_pkCustomKeyBtn );

    LINK_CONTROL( "key_default_selected", m_pkDefaultKeySelected );
    LINK_CONTROL( "key_custom_selected", m_pkCustomKeySelected );

    LINK_CONTROL( "status_static", m_pkStatus );


    LINK_CONTROL( "keyboard_radio", m_pkKeyboardRadio );
    LINK_CONTROL( "joystick_radio", m_pkjoystickRadio );

    LINK_CONTROL( "keyboard_radio_static", m_pkKeyboardRadioStatic );
    LINK_CONTROL( "joystick_radio_static", m_pkjoystickRadioStatic );


#ifdef WEAPONCHANGE
    LINK_CONTROL( "key_weaponswap_static", m_pkKeyStr[11] );
    LINK_CONTROL( "key_weaponswap", m_pkKey[11] );
    LINK_CONTROL( "key_girth12", m_pkGrith[11] );
    LINK_CONTROL( "key_back12", m_pkKeyBack[11] );
#endif
	LoadKeySettingString();
    m_iSelectedKey = -1;
}

KGCKeySettingWnd::~KGCKeySettingWnd(void)
{
}

void KGCKeySettingWnd::LoadKeySettingString( void )
{
	s_vecStrKey.clear();
	std::string strFileName = "key_str.stg";

	std::wifstream file;
	file.open( strFileName.c_str() , std::ios::binary | std::ios::in );

	IMBUE_NULL_CODECVT( file );
	if( file.is_open() == false )
		return;

	wchar_t GarvStr[2048];
	file.read( GarvStr, 1 );

	std::wstring str;
	while( std::getline( file, str, L'\n' ) ) 
	{
		std::wstring strString = str.substr( 0, str.find( L"\n" ) - 2 ); // \t 다음부터 \n 까지
		while( true )
		{
			if( strString.find( L"\r" ) == strString.npos )
				break;
			strString.erase( strString.find( L"\r" ) );
		}
		g_pkStrLoader->Replacer( strString, L"\\n", L"\r\n" );
		s_vecStrKey.push_back(strString);
	}

	file.close();
	return;
}

void KGCKeySettingWnd::OnCreate( void )
{
    m_pkKeyStr[0]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_UP ) );
    m_pkKeyStr[1]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_DOWN ) );
    m_pkKeyStr[2]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_LEFT ) );
    m_pkKeyStr[3]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_RIGHT ) );
    m_pkKeyStr[4]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_ATTACK ) );
    m_pkKeyStr[5]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_PROVACATION ) );
    m_pkKeyStr[6]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_CHANGEITEM ) );
    m_pkKeyStr[7]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_USEITEM ) );
    m_pkKeyStr[8]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_CHANGESKILL ) );
    m_pkKeyStr[9]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_PETSKILL ) );
    m_pkKeyStr[10]->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_CHARGINGSKILL ) );
#ifdef WEAPONCHANGE
    m_pkKeyStr[11]->SetText( g_pkStrLoader->GetString( 8295 ) );
#endif

	SetDefaultKey( !g_MyD3D->m_KGCOption.m_bEnableKeySetting );
    if( g_MyD3D->m_KGCOption.m_bEnableKeySetting )
    {
        m_pkStatus->SetText( L"" );
    }
    else
    {
        m_pkStatus->SetText( L"" );
    }

    for( int i = 0; i < NUM_USE_KEY; ++i )
    {
        m_pkKey[i]->InitState( true, true, this );
        m_pkKey[i]->SetAlign( DT_CENTER );
        m_pkKey[i]->SetFontColor( 0xffffffff ); // 생성 시 흰색으로..

        m_pkKeyStr[i]->SetAlign( DT_CENTER );

        m_pkGrith[i]->InitState( false );
    }

    m_pkDefaultKeyBtn->InitState( g_MyD3D->m_KGCOption.m_bEnableKeySetting, true, this );
    m_pkCustomKeyBtn->InitState( !g_MyD3D->m_KGCOption.m_bEnableKeySetting, true, this );


    m_pkDefaultKeySelected->InitState( !g_MyD3D->m_KGCOption.m_bEnableKeySetting, false, NULL );
    m_pkCustomKeySelected->InitState( g_MyD3D->m_KGCOption.m_bEnableKeySetting, false, NULL );

    m_pkStatus->SetAlign( DT_CENTER );

    m_iSelectedKey = -1;
#ifdef USE_JOYSTICK
    if(m_pkKeyboardRadio){
        m_pkKeyboardRadio->InitState(true,true,this);
        m_pkKeyboardRadio->Lock(true);
        g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_KEYBOARD);
    }
    if(m_pkKeyboardRadioStatic){
        m_pkKeyboardRadioStatic->InitState( true, true, this );
        m_pkKeyboardRadioStatic->SetText(L"Keyboard");
        m_pkKeyboardRadioStatic->SetAlign( DT_CENTER );
    }    
    if(m_pkjoystickRadio){
        m_pkjoystickRadio->InitState(true,true,this);
        m_pkjoystickRadio->Lock(false);
    }
    if(m_pkjoystickRadioStatic){
        m_pkjoystickRadioStatic->InitState( true, true, this );
        m_pkjoystickRadioStatic->SetText(L"Joystick");
        m_pkjoystickRadioStatic->SetAlign( DT_CENTER );
    }   
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE, this, &KGCKeySettingWnd::UpdateJoystickKey );

    
#else
    if(m_pkKeyboardRadio)
        m_pkKeyboardRadio->InitState(false,false,NULL);
    if(m_pkjoystickRadio)
        m_pkjoystickRadio->InitState(false,false,NULL);
#endif
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE, this, &KGCKeySettingWnd::UpdateKey );

#if defined(NATION_THAILAND)
	m_pkKeyStr[10]->ToggleRender(false);
	m_pkKeyBack[10]->ToggleRender(false);
	m_pkKey[10]->ToggleRender(false);
#endif
}

void KGCKeySettingWnd::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        for( int i = 0; i < NUM_USE_KEY; ++i )
        {   
            if( m_iSelectedKey == i )
                continue;

            if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
            {
                if( false == g_MyD3D->m_KGCOption.m_bEnableKeySetting )
                {
                    m_pkGrith[i]->ToggleRender( false );
                }
                else
                {
                    m_pkGrith[i]->ToggleRender( true );
                    g_KDSound.Play( "30" );
                    m_pkGrith[i]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xaa, 0xaa, 0xaa ) );
                }
            }
        }
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
    {
        for( int i = 0; i < NUM_USE_KEY; ++i )
        {
            if( m_iSelectedKey == i )
                continue;

            if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
            {
                m_pkGrith[i]->ToggleRender( false );
                g_KDSound.Play( "30" );

                m_pkGrith[i]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
            }
        }
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        // 디폴트 키설정
        if( event.m_pWnd == m_pkDefaultKeyBtn )
        {
            g_MyD3D->m_KGCOption.m_bEnableKeySetting = false;

#ifdef USE_JOYSTICK
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
                SetDefaultKey( true );
            else
                SetDefaultJoystickKey(true);
#else
            SetDefaultKey( true );
#endif

            m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_DEFAULT_KEY ) );

            if( m_iSelectedKey != -1 )
            {
                m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
                m_pkGrith[m_iSelectedKey]->ToggleRender( false );
                
                m_iSelectedKey = -1;
            }

            m_pkStatus->ToggleRender( true );

            m_pkDefaultKeySelected->ToggleRender( true );
            m_pkCustomKeySelected->ToggleRender( false );
            m_pkDefaultKeyBtn->ToggleRender( false );
            m_pkCustomKeyBtn->ToggleRender( true );

            g_KDSound.Play( "31" );
        }
        // 나만의 키설정
        else if( event.m_pWnd == m_pkCustomKeyBtn )
        {
            g_MyD3D->m_KGCOption.m_bEnableKeySetting = true;
#ifdef USE_JOYSTICK
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
                SetDefaultKey( false );
            else
                SetDefaultJoystickKey(false);
#else
            SetDefaultKey( false );
#endif

            m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_KEY ) );

            if( m_iSelectedKey != -1 )
            {
                m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
                m_pkGrith[m_iSelectedKey]->ToggleRender( false );

                m_iSelectedKey = -1;
            }

            m_pkStatus->ToggleRender( true );

            m_pkDefaultKeySelected->ToggleRender( false );
            m_pkCustomKeySelected->ToggleRender( true );
            m_pkDefaultKeyBtn->ToggleRender( true );
            m_pkCustomKeyBtn->ToggleRender( false );

            g_KDSound.Play( "31" );
        }
        else if(event.m_pWnd == m_pkKeyboardRadio || event.m_pWnd == m_pkKeyboardRadioStatic){
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_JOYSTICK){
                m_pkKeyboardRadio->Lock(true);
                m_pkjoystickRadio->Lock(false);
                g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_KEYBOARD);
                SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
            }
        }
#if defined(USE_JOYSTICK)
        else if(event.m_pWnd == m_pkjoystickRadio || event.m_pWnd == m_pkjoystickRadioStatic){
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD){
                m_pkjoystickRadio->Lock(true);
                m_pkKeyboardRadio->Lock(false);
                g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_JOYSTICK);
                SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
            }
        }
#endif
        else if( g_MyD3D->m_KGCOption.m_bEnableKeySetting )
        {
            for( int i = 0; i < NUM_USE_KEY; ++i )
            {
                if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
                {
#ifdef USE_JOYSTICK
                    if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
                        SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
                    else
                        SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
#else
                    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
#endif
                    SetSelectInput(i);

                }
            }
        }

		
    }
}

void KGCKeySettingWnd::SetSelectInput(int iIndex)
{
    if( m_iSelectedKey != -1 )
    {
        m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        m_pkGrith[m_iSelectedKey]->ToggleRender( false );

        m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
    }

    m_iSelectedKey = iIndex;

    m_pkKey[m_iSelectedKey]->SetFontColor( 0x99999999 );
    m_pkGrith[m_iSelectedKey]->ToggleRender( true );

    m_pkStatus->ToggleRender( true );
    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_PLEASE_INPUT_KEY ) );

    g_KDSound.Play( "31" );

}

void KGCKeySettingWnd::CheckKeyInputEvent()
{
    static int siCounter = 0;

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_KEY ) )
    {
        for( int i = 0; i < 256; ++i )
        {
            if( g_pkInput->IsDown( i ) )
            {
                int iKey = GetKeyIndex( i );
                if( iKey != -1 )
                {
                    // 만약 이미 사용중인 키를 선택했을 경우 두개를 swap한다
                    BYTE keyTemp = g_MyD3D->m_KGCOption.m_KeyTable[m_iSelectedKey];
                    g_MyD3D->m_KGCOption.m_KeyTable[m_iSelectedKey] = (BYTE)i;

                    for( int j = 0; j < ( NUM_USE_KEY + NUM_USE_SKILL_KEY ); ++j )
                    {
                        if( j == m_iSelectedKey )
                            continue;

                        if( g_MyD3D->m_KGCOption.m_KeyTable[j] == (BYTE)i )
                        {
                            g_MyD3D->m_KGCOption.m_KeyTable[j] = keyTemp;
                        }
                    }

                    m_pkGrith[m_iSelectedKey]->ToggleRender( false );
                    m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
                    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_KEY ) );
                    m_iSelectedKey = -1;

                    SetDefaultKey( false );
                }
                else
                {
                    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_CANNOT_USE_KEY ) );
                }
                break;
            }

            BYTE a = static_cast<BYTE>( 55.0f + 200.0f * static_cast<float>( siCounter < 20 ? siCounter : 40 - siCounter ) / static_cast<float>( 20 ) );
            m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( a, 0xff, 0xff, 0xff ) );
        }

        if( siCounter == 40 )
            siCounter = 0;
        else
            siCounter++;
    }
}
#ifdef USE_JOYSTICK
void KGCKeySettingWnd::CheckJoystickIputEvent()
{
    static int siJoystickCounter = 0;
    static bool bJoystickDelayInput = false;

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_KEY ))
    {
        int iKey = g_MyD3D->GetJoystickKey();
        if(bJoystickDelayInput){
            if(iKey == -1){
                bJoystickDelayInput = false;
                return;
            }
            return;
        }

        if(iKey != -1){
            g_MyD3D->SetJoystickKey(static_cast<BYTE>(iKey),m_iSelectedKey);
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
            m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            if(m_iSelectedKey + 1 >= NUM_USE_KEY)
                m_iSelectedKey = -1;
            else
                SetSelectInput(m_iSelectedKey+1);
            SetDefaultJoystickKey( false);
            bJoystickDelayInput = true;
            return;
        }

        BYTE a = static_cast<BYTE>( 55.0f + 200.0f * static_cast<float>( siJoystickCounter < 20 ? siJoystickCounter : 40 - siJoystickCounter ) / static_cast<float>( 20 ) );
        m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( a, 0xff, 0xff, 0xff ) );
        if( siJoystickCounter == 40 )
            siJoystickCounter = 0;
        else
            siJoystickCounter++;

    }
}

void KGCKeySettingWnd::CheckJoystickIputEvent_Frame()
{

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_KEY )){return;}


    int iKey = g_MyD3D->GetJoystickKey();

    if(iKey == -1){
        for( int i = 0; i < NUM_USE_KEY; ++i )
        {
            m_pkGrith[i]->ToggleRender( false );
        }


        return;
    }
    int iVirtureKeyIndex = g_MyD3D->MyCtrl->GetJoystickVirtualKey(iKey);

    if( iVirtureKeyIndex < NUM_USE_KEY )
        m_pkGrith[iVirtureKeyIndex]->ToggleRender( true );

}

void KGCKeySettingWnd::SetDefaultJoystickKey( bool bDefault )
{
    if( bDefault )
    {
        if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_KEY ) )
        {
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        }
        m_iSelectedKey = -1;
    }
    g_MyD3D->SetJoystickKey(bDefault);

    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
}

#endif
void KGCKeySettingWnd::FrameMoveInEnabledState()
{
#ifdef USE_JOYSTICK
    if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
        CheckKeyInputEvent();
    else{
        CheckJoystickIputEvent();
        CheckJoystickIputEvent_Frame();
    }
#else
    CheckKeyInputEvent();
#endif
}

int KGCKeySettingWnd::GetKeyIndex( int iKey )
{
    for( int i = 0; i < KEYTABLESIZE; ++i )
    {
        if( s_iKey[i] == iKey )
            return i;
    }
    return -1;
}


void KGCKeySettingWnd::SetDefaultKey( bool bDefault )
{
    if( bDefault )
    {
        if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_KEY ) )
        {
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        }
        m_iSelectedKey = -1;
    }
    g_MyD3D->UpdateKeySetting( bDefault );
    
    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
}

void KGCKeySettingWnd::UpdateKey()
{
    KSafeArray<int,NUM_USE_KEY> aKey;

    aKey[0]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyUp )          ;
    aKey[1]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyDown )        ;
    aKey[2]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyLeft )        ;
    aKey[3]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyRight )       ;
    aKey[4]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyAttack )      ;
    aKey[5]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyProvocation ) ;
    aKey[6]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyChangeItem )  ;
    aKey[7]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyUseItem )     ;
    aKey[8]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyChangeSkill ) ;
    aKey[9]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyUsePetSkill ) ;
    aKey[10]= GetKeyIndex( g_MyD3D->MyCtrl->m_keySpecialSkillKey );
#if defined(WEAPONCHANGE)
    aKey[11]= GetKeyIndex( g_MyD3D->MyCtrl->m_keyWeaponSwap );
#endif

    //키 셋팅 파일이 손상 되었을 지 모른다.
    for ( int i = 0 ; i < NUM_USE_KEY; i++ )
    {
        if ( aKey[i] == -1 )
        {
            g_MyD3D->UpdateKeySetting( true );
            break;
        }
    }


    m_pkKey[0]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyUp ) ] );
    m_pkKey[1]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyDown ) ] );
    m_pkKey[2]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyLeft ) ] );
    m_pkKey[3]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyRight ) ] );
    m_pkKey[4]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyAttack ) ] );
    m_pkKey[5]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyProvocation ) ] );
    m_pkKey[6]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyChangeItem ) ] );
    m_pkKey[7]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyUseItem ) ] );
    m_pkKey[8]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyChangeSkill ) ] );
    m_pkKey[9]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyUsePetSkill ) ] );
    m_pkKey[10]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keySpecialSkillKey ) ] );
#if defined(WEAPONCHANGE)
    m_pkKey[11]->SetText( s_vecStrKey[ GetKeyIndex( g_MyD3D->MyCtrl->m_keyWeaponSwap ) ] );
#endif

    diks[ g_MyD3D->MyCtrl->m_keyUp ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyDown ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyLeft ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyRight ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyAttack ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyProvocation ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyChangeItem ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyUseItem ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyChangeSkill ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyUsePetSkill ] = false;
    diks[ g_MyD3D->MyCtrl->m_keySpecialSkillKey ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyWeaponSwap ] = false;
    diks[ g_MyD3D->MyCtrl->m_keyGameInventory ] = false;

    diks[ g_MyD3D->MyCtrl->m_keyCameraZoom ] = false;

#if !defined(__PATH__)
    diks[ g_MyD3D->MyCtrl->m_keyMonsterDirection ] = false;
#endif

    Old_diks[ g_MyD3D->MyCtrl->m_keyUp ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyDown ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyLeft ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyRight ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyAttack ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyProvocation ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyChangeItem ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyUseItem ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyChangeSkill ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyUsePetSkill ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keySpecialSkillKey ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyWeaponSwap ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyGameInventory ] = false;
    Old_diks[ g_MyD3D->MyCtrl->m_keyCameraZoom ] = false;

#if !defined(__PATH__)
    Old_diks[ g_MyD3D->MyCtrl->m_keyMonsterDirection ] = false;
#endif
}

#if defined(USE_JOYSTICK)
void KGCKeySettingWnd::UpdateJoystickKey()
{
    g_MyD3D->SetJoystickKey(!g_MyD3D->m_KGCOption.m_bEnableKeySetting);

    for(int i = 0; i < NUM_USE_KEY; ++i){
        m_pkKey[i]->SetText( s_strJoystickKey[  g_MyD3D->MyCtrl->GetJoystickSelectedKey(i) ] );
    }
}
#endif

std::wstring KGCKeySettingWnd::GetKeyStr( BYTE cKey )
{
    return s_vecStrKey[ GetKeyIndex( cKey ) ];
}



IMPLEMENT_CLASSNAME( KGCSkillKeySettingWnd );
IMPLEMENT_WND_FACTORY( KGCSkillKeySettingWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillKeySettingWnd, "gc_key_setting_wnd2" );


KGCSkillKeySettingWnd::KGCSkillKeySettingWnd( void )
{

	char aTemp[MAX_PATH];
	for ( int i = 0; i < NUM_USE_SKILL_KEY; i ++ )
	{
		sprintf( aTemp, "key_back%d", i + 1 );
		LINK_CONTROL( aTemp, m_pkKeyBack[i] );
		sprintf( aTemp, "key_girth%d", i + 1 );
		LINK_CONTROL( aTemp, m_pkGrith[i] );
		sprintf( aTemp, "key_%d", i + 1 );
		LINK_CONTROL( aTemp, m_pkKey[i] );
		sprintf( aTemp, "key_stati%d", i + 1 );
		LINK_CONTROL( aTemp, m_pkKeyStr[i] );
	}

    m_pkKeyboardRadio = NULL;
    m_pkjoystickRadio = NULL;

    m_pkKeyboardRadioStatic = NULL;
    m_pkjoystickRadioStatic = NULL;

	LINK_CONTROL( "key_default_button", m_pkDefaultKeyBtn );
	LINK_CONTROL( "key_custom_button", m_pkCustomKeyBtn );

	LINK_CONTROL( "key_default_selected", m_pkDefaultKeySelected );
	LINK_CONTROL( "key_custom_selected", m_pkCustomKeySelected );


	LINK_CONTROL( "option_close", m_pkClose );
	LINK_CONTROL( "status_static", m_pkStatus );

    LINK_CONTROL( "keyboard_radio", m_pkKeyboardRadio );
    LINK_CONTROL( "joystick_radio", m_pkjoystickRadio );

    LINK_CONTROL( "keyboard_radio_static", m_pkKeyboardRadioStatic );
    LINK_CONTROL( "joystick_radio_static", m_pkjoystickRadioStatic );

}

KGCSkillKeySettingWnd::~KGCSkillKeySettingWnd(){}


void KGCSkillKeySettingWnd::UpdateKey()
{
	KSafeArray<int,NUM_USE_SKILL_KEY> aKey;

	for( unsigned int i = 0; i <  g_MyD3D->MyCtrl->m_vecKeySkill.size(); i++ )
	{
		aKey[i]= KGCKeySettingWnd::GetKeyIndex( g_MyD3D->MyCtrl->m_vecKeySkill[i] );
	}
	
	//키 셋팅 파일이 손상 되었을 지 모른다.
	for ( int i = 0 ; i < NUM_USE_SKILL_KEY; i++ )
	{
		if ( aKey[i] == -1 )
		{
			g_MyD3D->UpdateKeySetting( true );
			break;
		}
	}

	for( unsigned int i = 0; i <  g_MyD3D->MyCtrl->m_vecKeySkill.size(); i++ )
	{
		m_pkKey[i]->SetText( s_vecStrKey[ KGCKeySettingWnd::GetKeyIndex( g_MyD3D->MyCtrl->m_vecKeySkill[i] ) ] );
		diks[ g_MyD3D->MyCtrl->m_vecKeySkill[i] ] = false;
		Old_diks[ g_MyD3D->MyCtrl->m_vecKeySkill[i] ] = false;
	}	
}
#if defined(USE_JOYSTICK)
void KGCSkillKeySettingWnd::UpdateJoystickKey()
{
    g_MyD3D->SetJoystickKey(!g_MyD3D->m_KGCOption.m_bEnableKeySetting);

//    for(int i = NUM_USE_KEY; i < NUM_USE_KEY + NUM_USE_SKILL_KEY ; ++i){
    for(int i = 0; i < NUM_USE_SKILL_KEY ; ++i){
        m_pkKey[i]->SetText( s_strJoystickKey[  g_MyD3D->MyCtrl->GetJoystickSelectedKey(i+NUM_USE_KEY) ] );
    }
}
#endif
void KGCSkillKeySettingWnd::SetDefaultKey( bool bDefault )
{
    if( bDefault )
    {
        if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_SKILL_KEY ) )
        {
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        }
        m_iSelectedKey = -1;
    }

	g_MyD3D->UpdateKeySetting( bDefault );

    KGCKeySettingWnd* pKeySettingWnd = (KGCKeySettingWnd*)this->GetParent()->GetChildByName("key_setting_wnd");
    if( pKeySettingWnd )
	    pKeySettingWnd->SetDefaultKey( bDefault );

    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
}

void KGCSkillKeySettingWnd::FrameMoveInEnabledState()
{
#ifdef USE_JOYSTICK
    if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
        CheckKeyInputEvent();
    else{
        CheckJoystickIputEvent();
        CheckJoystickIputEvent_Frame();
    }
#else
    CheckKeyInputEvent();
#endif
}

void KGCSkillKeySettingWnd::OnCreate( void )
{
	for( int i = 0 ; i < NUM_USE_SKILL_KEY; i++ )
	{
		m_pkKeyStr[i]->SetText( g_pkStrLoader->GetString( 6372 + i ) );
	}
	
	SetDefaultKey( !g_MyD3D->m_KGCOption.m_bEnableKeySetting );

	UpdateKey();
	if( g_MyD3D->m_KGCOption.m_bEnableKeySetting )
	{
		m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_KEY ) );
	}
	else
	{
		m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_DEFAULT_KEY ) );
	}

	for( int i = 0; i < NUM_USE_SKILL_KEY; ++i )
	{
		m_pkKey[i]->InitState( true, true, this );
		m_pkKey[i]->SetAlign( DT_CENTER );
		m_pkKey[i]->SetFontColor( 0xffffffff ); // 생성 시 흰색으로..

		m_pkKeyStr[i]->SetAlign( DT_CENTER );

		m_pkGrith[i]->InitState( false );
	}

	m_pkDefaultKeyBtn->InitState( g_MyD3D->m_KGCOption.m_bEnableKeySetting, true, this );
	m_pkCustomKeyBtn->InitState( !g_MyD3D->m_KGCOption.m_bEnableKeySetting, true, this );

	m_pkDefaultKeySelected->InitState( !g_MyD3D->m_KGCOption.m_bEnableKeySetting, false, NULL );
	m_pkCustomKeySelected->InitState( g_MyD3D->m_KGCOption.m_bEnableKeySetting, false, NULL );

	m_pkStatus->SetAlign( DT_CENTER );

	m_iSelectedKey = -1;

	m_pkClose->InitState( true, true, this );

#ifdef USE_JOYSTICK
    if(m_pkKeyboardRadio){
        m_pkKeyboardRadio->InitState(true,true,this);
        m_pkKeyboardRadio->Lock(true);
        g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_KEYBOARD);

    }
    if(m_pkKeyboardRadioStatic){
        m_pkKeyboardRadioStatic->InitState( true, true, this );
        m_pkKeyboardRadioStatic->SetText(L"Keyboard");
        m_pkKeyboardRadioStatic->SetAlign( DT_CENTER );
    }    
    if(m_pkjoystickRadio){
        m_pkjoystickRadio->InitState(true,true,this);
        m_pkjoystickRadio->Lock(false);
    }
    if(m_pkjoystickRadioStatic){
        m_pkjoystickRadioStatic->InitState( true, true, this );
        m_pkjoystickRadioStatic->SetText(L"Joystick");
        m_pkjoystickRadioStatic->SetAlign( DT_CENTER );
    }   

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE, this, &KGCSkillKeySettingWnd::UpdateJoystickKey );
#else
    if(m_pkKeyboardRadio)
        m_pkKeyboardRadio->InitState(false,false,NULL);
    if(m_pkjoystickRadio)
        m_pkjoystickRadio->InitState(false,false,NULL);
#endif

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE, this, &KGCSkillKeySettingWnd::UpdateKey );

	((KD3DStatic*)GetChildByName("keysetting_static"))->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING ) );
	((KD3DStatic*)GetChildByName("keysetting_static"))->SetAlign( DT_CENTER );
}

void KGCSkillKeySettingWnd::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
	{
        for( int i = 0; i < NUM_USE_SKILL_KEY; ++i )
        {   
            if( m_iSelectedKey == i )
                continue;

            if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
            {
                if( false == g_MyD3D->m_KGCOption.m_bEnableKeySetting )
                {
                    m_pkGrith[i]->ToggleRender( false );
                }
                else
                {
                    m_pkGrith[i]->ToggleRender( true );
                    g_KDSound.Play( "30" );
                    m_pkGrith[i]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xaa, 0xaa, 0xaa ) );
                }
            }
        }
	}

	if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
	{
        for( int i = 0; i < NUM_USE_SKILL_KEY; ++i )
        {
            if( m_iSelectedKey == i )
                continue;

            if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
            {
                m_pkGrith[i]->ToggleRender( false );
                g_KDSound.Play( "30" );

                m_pkGrith[i]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
            }
        }
	}

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		// 디폴트 키설정
		if( event.m_pWnd == m_pkDefaultKeyBtn )
		{
#ifdef USE_JOYSTICK
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
                SetDefaultKey( true );
            else
                SetDefaultJoystickKey(true);
#else
            SetDefaultKey( true );
#endif
			g_MyD3D->m_KGCOption.m_bEnableKeySetting = false;
			m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_DEFAULT_KEY ) );

			if( m_iSelectedKey != -1 )
			{
				m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
				m_pkGrith[m_iSelectedKey]->ToggleRender( false );

				m_iSelectedKey = -1;
			}

			m_pkStatus->ToggleRender( true );

			m_pkDefaultKeySelected->ToggleRender( true );
			m_pkCustomKeySelected->ToggleRender( false );
			m_pkDefaultKeyBtn->ToggleRender( false );
			m_pkCustomKeyBtn->ToggleRender( true );

			g_KDSound.Play( "31" );
		}
		// 나만의 키설정
		else if( event.m_pWnd == m_pkCustomKeyBtn )
		{
#ifdef USE_JOYSTICK
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD)
                SetDefaultKey( false );
            else
                SetDefaultJoystickKey(false);
#else
            SetDefaultKey( false );
#endif
			g_MyD3D->m_KGCOption.m_bEnableKeySetting = true;
			m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_KEY ) );

			if( m_iSelectedKey != -1 )
			{
				m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
				m_pkGrith[m_iSelectedKey]->ToggleRender( false );

				m_iSelectedKey = -1;
			}

			m_pkStatus->ToggleRender( true );

			m_pkDefaultKeySelected->ToggleRender( false );
			m_pkCustomKeySelected->ToggleRender( true );
			m_pkDefaultKeyBtn->ToggleRender( true );
			m_pkCustomKeyBtn->ToggleRender( false );

			g_KDSound.Play( "31" );
		}
        else if(event.m_pWnd == m_pkKeyboardRadio || event.m_pWnd == m_pkKeyboardRadioStatic){
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_JOYSTICK){
                m_pkKeyboardRadio->Lock(true);
                m_pkjoystickRadio->Lock(false);
                g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_KEYBOARD);
                SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
            }
        }
#if defined(USE_JOYSTICK)
        else if(event.m_pWnd == m_pkjoystickRadio || event.m_pWnd == m_pkjoystickRadioStatic){
            if(g_MyD3D->m_KGCOption.GetSettingDeviceType() == KGCOptionManager::EGCOM_KEYBOARD){
                m_pkjoystickRadio->Lock(true);
                m_pkKeyboardRadio->Lock(false);
                g_MyD3D->m_KGCOption.SetSettingDeviceType(KGCOptionManager::EGCOM_JOYSTICK);
                SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
            }
        }
#endif
		else if( g_MyD3D->m_KGCOption.m_bEnableKeySetting )
		{
			for( int i = 0; i < NUM_USE_SKILL_KEY; ++i )
			{
				if( event.m_pWnd == m_pkKey[i] || event.m_pWnd == m_pkKeyBack[i] )
				{
#ifdef USE_JOYSTICK
                    if(m_pkKeyboardRadio && m_pkKeyboardRadio->IsLocked() == true)
                        SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
                    else if(m_pkKeyboardRadio && m_pkKeyboardRadio->IsLocked() == true)
                        SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
                    else
                        SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
#else
                    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_KEY_TEXT_UPDATE));
#endif
                    SetSelectInput(i);
				}
			}
		}

		if ( m_pkClose == event.m_pWnd )
		{
			SpeakToActionListener( event );
		}

	}
}

void KGCSkillKeySettingWnd::SetSelectInput(int iIndex)
{
    if( m_iSelectedKey != -1 )
    {
        m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        m_pkGrith[m_iSelectedKey]->ToggleRender( false );

        m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
    }

    m_iSelectedKey = iIndex;

    m_pkKey[m_iSelectedKey]->SetFontColor( 0x99999999 );
    m_pkGrith[m_iSelectedKey]->ToggleRender( true );

    m_pkStatus->ToggleRender( true );
    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_PLEASE_INPUT_KEY ) );

    g_KDSound.Play( "31" );

}

void KGCSkillKeySettingWnd::CheckKeyInputEvent()
{
    static int siCounter = 0;

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_SKILL_KEY ) )
    {
        for( int i = 0; i < 256; ++i )
        {
            if( g_pkInput->IsDown( i ) )
            {
                int iKey = KGCKeySettingWnd::GetKeyIndex( i );
                if( iKey != -1 )
                {
                    // 만약 이미 사용중인 키를 선택했을 경우 두개를 swap한다
                    BYTE keyTemp = g_MyD3D->m_KGCOption.m_KeyTable[m_iSelectedKey + NUM_USE_KEY];
                    g_MyD3D->m_KGCOption.m_KeyTable[m_iSelectedKey + NUM_USE_KEY] = (BYTE)i;

                    for( int j = 0; j < ( NUM_USE_SKILL_KEY + NUM_USE_KEY ); ++j )
                    {
                        if( j == m_iSelectedKey + NUM_USE_KEY )
                            continue;

                        if( g_MyD3D->m_KGCOption.m_KeyTable[j] == (BYTE)i )
                        {
                            g_MyD3D->m_KGCOption.m_KeyTable[j] = keyTemp;
                        }
                    }

                    m_pkGrith[m_iSelectedKey]->ToggleRender( false );
                    m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
                    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING_KEY ) );
                    m_iSelectedKey = -1;

                    SetDefaultKey( false );
                }
                else
                {
                    m_pkStatus->SetText( g_pkStrLoader->GetString( STR_ID_KEY_CANNOT_USE_KEY ) );
                }
                break;
            }

            BYTE a = static_cast<BYTE>( 55.0f + 200.0f * static_cast<float>( siCounter < 20 ? siCounter : 40 - siCounter ) / static_cast<float>( 20 ) );
            m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( a, 0xff, 0xff, 0xff ) );
        }

        if( siCounter == 40 )
            siCounter = 0;
        else
            siCounter++;
    }
}

#ifdef USE_JOYSTICK
void KGCSkillKeySettingWnd::CheckJoystickIputEvent()
{
    static int siJoystickCounter = 0;
    static bool bJoystickDelayInput = false;

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_SKILL_KEY ))
    {
        int iKey = g_MyD3D->GetJoystickKey();
        if(bJoystickDelayInput){
            if(iKey == -1){
                bJoystickDelayInput = false;
                return;
            }
            return;
        }

        if(iKey != -1){
            g_MyD3D->SetJoystickKey(static_cast<BYTE>(iKey),m_iSelectedKey+NUM_USE_KEY);
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
            m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            if(m_iSelectedKey + 1 >= NUM_USE_SKILL_KEY)
                m_iSelectedKey = -1;
            else
                SetSelectInput(m_iSelectedKey+1);
            SetDefaultJoystickKey( false);
            bJoystickDelayInput = true;
            return;
        }

        BYTE a = static_cast<BYTE>( 55.0f + 200.0f * static_cast<float>( siJoystickCounter < 20 ? siJoystickCounter : 40 - siJoystickCounter ) / static_cast<float>( 20 ) );
        m_pkGrith[m_iSelectedKey]->SetAllWindowColor( D3DCOLOR_ARGB( a, 0xff, 0xff, 0xff ) );
        if( siJoystickCounter == 40 )
            siJoystickCounter = 0;
        else
            siJoystickCounter++;

    }
}

void KGCSkillKeySettingWnd::CheckJoystickIputEvent_Frame()
{

    if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_SKILL_KEY )){return;}


    int iKey = g_MyD3D->GetJoystickKey();

    if(iKey == -1){
        for( int i = 0; i < NUM_USE_SKILL_KEY; ++i )
        {
            m_pkGrith[i]->ToggleRender( false );
        }


        return;
    }
    int iVirtureKeyIndex = g_MyD3D->MyCtrl->GetJoystickVirtualKey(iKey) - NUM_USE_KEY;

    if( -1 < iVirtureKeyIndex &&  iVirtureKeyIndex < NUM_USE_SKILL_KEY )
        m_pkGrith[iVirtureKeyIndex]->ToggleRender( true );

}

void KGCSkillKeySettingWnd::SetDefaultJoystickKey( bool bDefault )
{
    if( bDefault )
    {
        if( m_iSelectedKey > -1 && m_iSelectedKey < ( NUM_USE_SKILL_KEY ) )
        {
            m_pkGrith[m_iSelectedKey]->ToggleRender( false );
            m_pkKey[m_iSelectedKey]->SetFontColor( 0xffffffff );
        }
        m_iSelectedKey = -1;
    }
    g_MyD3D->SetJoystickKey(bDefault);

    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE));
}

#endif