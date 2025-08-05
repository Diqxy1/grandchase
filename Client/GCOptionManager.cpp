#include "stdafx.h"
#include "GCOptionManager.h"
#include "MyD3D.h"

#include "GCUI/GCChatBox.h"
#include "KMci.h"


#include "GCStateMachine.h"

#include "GCUI/GCGraphicQuality.h"
#include "GCUI/GCOptionSound.h"
#include "GCUI/GCOptionMusic.h"
#include "GCRenderManager.h"
//
#include "GCUI/GCOption.h"
#include "KGCWorldMapManager.h"
#include "GCUI/GCItemInformationBoxPair.h"
#include "KGCWeatherSystem.h"


#define READ_OPTION(p,q,r)  ReadFile(DBF, &p, q, &temp, NULL);\
                            if( temp == 0 )\
                            {\
                                p = r;\
                            }

KGCOptionManager::KGCOptionManager(void)
: m_iSettingDeviceType(EGCOM_KEYBOARD)
{
    m_iQualityCount = 5;
    m_bSoundEnable = true;
    m_bMusicEnable = true;

    m_fSoundVolume = 0.0f;
    m_fMusicVolume = 0.0f;

    m_bMotionBlur = false;
    m_bEdgeRender = true;

    m_bEnableKeySetting = false;
    m_bSkillEffect = true;
    m_bUsingVS = true;

    m_bQualitySquare = true;
    m_bDenyInviteMsg = false;
    m_bSquareItemEffect = true;
    m_ucLastState = 0;
    m_ucLastWorldMap = (BYTE)WORLDMAP_UPPERMAP;

	m_fScaleX = 1.0f;
    m_fScaleY = 1.0f;

    m_dwBasicMode = 0;
    m_dwCurrentMode = 0;
    m_dwFullScreenMode = 0;
	m_dwScreenMode = 0;
	m_iIndexColorTable = 0;
    m_dwPrevMode = 0;
	m_dwCurrentModeOld = 0;
	m_dwFullScreenModeOld = 0;

    m_eSampleinfo = D3DMULTISAMPLE_4_SAMPLES;//D3DMULTISAMPLE_NONE;
        m_eSampleinfoOld = D3DMULTISAMPLE_4_SAMPLES;//D3DMULTISAMPLE_NONE;

    m_bAlphaAni = true;
	m_bPetViewHide = false;
    m_bBasicEquipEnable = false;
    m_bRoomAnimationEnable = true;

    for(int i = 0;i<NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i){
        m_JoystickKeyTable[i] = static_cast<BYTE>(i);
    }
}


KGCOptionManager::~KGCOptionManager(void)
{
}

void KGCOptionManager::SetSound()
{
    g_pOptionSound->SetEnable(m_bSoundEnable, m_fSoundVolume);
}

void KGCOptionManager::SetMusic()
{
    g_pOptionMusic->SetEnable(m_bMusicEnable, m_fMusicVolume);
}

void KGCOptionManager::Save_Option()
{
    HANDLE DBF;
    DWORD temp;

    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    g_MyD3D->Default_Width=(unsigned short)pDevice->modes[pDevice->dwCurrentMode].Width;
    g_MyD3D->Default_Height=(unsigned short)pDevice->modes[pDevice->dwCurrentMode].Height;

#if defined(NO_USE_WINDOW_MODE) && defined(__PATH__)
    g_MyD3D->Default_Fullscreen=true;    
#else
    if(g_MyD3D->IsWindowMode())
        g_MyD3D->Default_Fullscreen=false;
    else
        g_MyD3D->Default_Fullscreen=true;
#endif

    m_bMusicEnable = g_KMci.IsPlay();
    m_bSoundEnable = Is_Sound;
    if( g_pOptionMusic != NULL )
    m_fMusicVolume = g_pOptionMusic->GetVolume();
    if( g_pOptionSound != NULL )
    m_fSoundVolume = g_pOptionSound->GetVolume();
    m_bWhisperFilter = g_pkChatManager->GetWhisperFilter();

    int iVer = GC_OPTION_VERSION;

    if(DBF=CreateFileA("OptionVer2.dat", GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
    {
        WriteFile(DBF, &iVer, sizeof(int), &temp, NULL);
        WriteFile(DBF, &m_bIsCartoon, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &g_MyD3D->Default_Width, sizeof(unsigned short), &temp, NULL);
        WriteFile(DBF, &g_MyD3D->Default_Height, sizeof(unsigned short), &temp, NULL);
        WriteFile(DBF, &g_MyD3D->Default_Fullscreen, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bJoystic, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bShadow, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_b3Back, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bModulate2X, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_b1Back, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bLinearFilter, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bSoundEnable, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bMusicEnable, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bWhisperFilter, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bHitEffect, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bMotionBlur, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bAutoGraphicQuality, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_fSoundVolume, sizeof(float), &temp, NULL);
        WriteFile(DBF, &m_fMusicVolume, sizeof(float), &temp, NULL);
        WriteFile(DBF, &m_bSaveID, sizeof(bool), &temp, NULL );
        WriteFile(DBF, &m_wcsUserID, sizeof(WCHAR) * MAX_LOGIN_ID_SIZE, &temp, NULL );
        WriteFile(DBF, &m_bQualitySquare, sizeof(bool), &temp, NULL );
        WriteFile(DBF, &m_bSquareItemEffect, sizeof(bool), &temp, NULL );
        WriteFile(DBF, &m_bDenyInviteMsg, sizeof(bool), &temp, NULL );
        WriteFile(DBF, &m_ucLastState, sizeof(BYTE), &temp, NULL);
        WriteFile(DBF, &m_ucLastWorldMap, sizeof(BYTE), &temp, NULL);
        WriteFile(DBF, &m_dwCurrentMode, sizeof(DWORD), &temp, NULL);
        WriteFile(DBF, &m_dwFullScreenMode, sizeof(DWORD), &temp, NULL );
        
#if defined (NATION_THAILAND)
		WriteFile(DBF, &m_iFunBoxPostfix, sizeof(int), &temp, NULL );
#else
		WriteFile(DBF, &m_eSampleinfo, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL ); //SaveÆÄÀÏ ¼ø¼­ À¯Áö À§ÇØ¼­ ÀÏ´Ü Ãß°¡ÇÔ ÇØ»óµµ ÂÊ ÄÚµå ÇÕÃÄÁ®¾ßµÊ.
#endif
#if defined (SAVE_CHATCOLOR_INFO)
		WriteFile(DBF, &m_iIndexColorTable, sizeof(int), &temp, NULL );
#endif
#if defined (ITEMBOX_ANI_TOGGLE)
        WriteFile(DBF, &m_bAlphaAni, sizeof(bool), &temp, NULL );
#endif

        CloseHandle(DBF);
    }


    if(DBF=CreateFileA("ResoultionSettingVer3.dat", GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
	{
		WriteFile(DBF, &m_dwScreenMode, sizeof(DWORD), &temp, NULL);
		WriteFile(DBF, &m_eSampleinfo, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL );
		CloseHandle(DBF);
	}

    if(DBF=CreateFileA("KeySettingVer2.dat", GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
    {
        BYTE tempKey;
		DWORD dwVer = 10001;
		WriteFile(DBF, &dwVer, sizeof(DWORD), &temp, NULL);
        WriteFile(DBF, &m_bEnableKeySetting, sizeof(bool), &temp, NULL);
        for( int i=0; i<NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i )
        {
            tempKey = m_KeyTable[i];
            WriteFile(DBF, &tempKey, sizeof(BYTE), &temp, NULL);
        }
        CloseHandle(DBF);
    }

    if(DBF=CreateFileA("JoystickSettingVer2.dat", GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
    {
        BYTE tempKey;
		DWORD dwVer = 10001;
		WriteFile(DBF, &dwVer, sizeof(DWORD), &temp, NULL);
        WriteFile(DBF, &m_bEnableKeySetting, sizeof(bool), &temp, NULL);
        for( int i=0; i<NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i )
        {
            tempKey = m_JoystickKeyTable[i];
            WriteFile(DBF, &tempKey, sizeof(BYTE), &temp, NULL);
        }
        CloseHandle(DBF);
    }

    if (DBF = CreateFileA("OtherOptions.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
    {
        BYTE tempKey;
        DWORD dwVer = 10001;
        WriteFile(DBF, &m_bSkillEffect, sizeof(bool), &temp, NULL);
        m_bUsingVS = SiKGCRenderManager()->GetUsingVS();
        WriteFile(DBF, &m_bUsingVS, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bPetViewHide, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bBasicEquipEnable, sizeof(bool), &temp, NULL);
        WriteFile(DBF, &m_bRoomAnimationEnable, sizeof(bool), &temp, NULL);
        CloseHandle(DBF);
    }
}

void KGCOptionManager::SetUsingVS()
{
    SiKGCRenderManager()->SetUsingVS(m_bUsingVS);
}

void KGCOptionManager::Quality_Change( int More, bool bIsAuto )
{
    m_iQualityCount += More;

    if (m_iQualityCount < 0)
    {
        m_iQualityCount = 0;
        return;
    }

    if (m_iQualityCount > 6)
    {
        m_iQualityCount = 6;
        return;
    }

    m_bLinearFilter = true;
    m_b1Back = true;
    m_b3Back = true;
    m_bShadow = true;
    m_bIsCartoon = true;
    m_bModulate2X = true;
    m_bMotionBlur = true;

    if (m_iQualityCount < 1)
    {
        m_bLinearFilter = false;
    }

    if (m_iQualityCount < 2)
    {
        m_b1Back = false;
    }

    if (m_iQualityCount < 3)
    {
        m_b3Back = false;
        SiKGCWeatherSystem()->SetAutoRenderOff(true);
    }
    else {
        SiKGCWeatherSystem()->SetAutoRenderOff(false);
    }

    if (m_iQualityCount < 4)
    {
        m_bShadow = false;
    }

    if (m_iQualityCount < 5)
    {
        m_bIsCartoon = false; // 일반 렌더링
        m_bModulate2X = false;
    }

    if (m_iQualityCount <= 5)
    {
        // 2007/2/13. iridology. 최상위 품질이 아니면 모션블러 끄자
        m_bMotionBlur = false;
    }

    if (m_iQualityCount == 5)
    {
        if (EnableModulate2X)
            m_bModulate2X = true;
        else
            m_bModulate2X = false;

        m_bIsCartoon = false;
    }

    if (m_iQualityCount >= 6)
    {
        if (EnableMultiMap)
            m_bIsCartoon = true;
        else
            m_bIsCartoon = false;
    }

    g_pkUIScene->m_pkOption->m_pkSkillEffectCheck->ToggleRender( m_bSkillEffect );
    g_pkUIScene->m_pkOption->m_pkMotionBlurCheck->ToggleRender( m_bMotionBlur );

    if(m_bLinearFilter)
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    }
    else
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    }

    switch( m_iQualityCount )
    {
    case 6:
        SiKGCRenderManager()->SetRenderTech(0);
        break;
    case 5:
        SiKGCRenderManager()->SetRenderTech(1);
        break;
    default:
        SiKGCRenderManager()->SetRenderTech(2);
        break;
    }

    static int aiQltID[] =
    {
        STR_ID_GRAPHIC_QUALITY_VERY_BAD,
            STR_ID_GRAPHIC_QUALITY_BAD,
            STR_ID_GRAPHIC_QUALITY_NORMAL,
            STR_ID_GRAPHIC_QUALITY_GOOD,
            STR_ID_GRAPHIC_QUALITY_GREAT,
            STR_ID_GRAPHIC_QUALITY_VERY_GREAT,
            STR_ID_GRAPHIC_QUALITY_BEST,
    };

    switch ( g_MyD3D->m_pStateMachine->GetState() )
    {
    case GS_WORLDMAP:
        {
            if( g_pkWorldmapChatBox != NULL )
            {
                g_pkWorldmapChatBox->AddChat( g_pkStrLoader->GetString( aiQltID[m_iQualityCount] ), KGCChatManager::CHAT_TYPE_ADMIN );
            }
            break;
        }
    case GS_MATCH:
        {
            if( g_pkLobbyChatBox != NULL )
            {
                g_pkLobbyChatBox->AddChat( g_pkStrLoader->GetString( aiQltID[m_iQualityCount] ), KGCChatManager::CHAT_TYPE_ADMIN );
            }
            break;
        }
    case GS_ROOM:
        {
            if ( g_pkRoomChatBox != NULL )
            {
                g_pkRoomChatBox->AddChat( g_pkStrLoader->GetString( aiQltID[m_iQualityCount] ), KGCChatManager::CHAT_TYPE_ADMIN );
            }
            break;
        }
    case GS_GUILD:
        {
            if ( g_pkGuildChatBox != NULL )
            {
                g_pkGuildChatBox->AddChat( g_pkStrLoader->GetString( aiQltID[m_iQualityCount] ), KGCChatManager::CHAT_TYPE_ADMIN );
            }
            break;
        }
    case GS_GAME:
        {
            // °ÔÀÓ µµÁß¿¡ ±×·¡ÇÈ Ä÷¸®Æ¼ º¯°æ ÇßÀ» °æ¿ì
            // ±×·¡ÇÈ Ç°Áú ¶³¾îÁú ¶§ : "* ÄèÀûÇÑ ÇÃ·¹ÀÌ¸¦ À§ÇØ ±×·¡ÇÈ Ç°ÁúÀ» Á¶Àý ÇÕ´Ï´Ù."
            if( bIsAuto )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_AUTO_GRAPHIC_QUALITY_MESSAGE1 ), KGCChatManager::CHAT_TYPE_INVITE );

            // ÇöÀç ±×·¡ÇÈ Ç°Áú Ç¥½Ã
            g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( aiQltID[m_iQualityCount] ), KGCChatManager::CHAT_TYPE_INVITE );

            // ÀÌ±îÁö ±×·¡ÇÈ Ä÷¸®Æ¼°¡ ³·¾ÆÁ³´Ù¸é Àú»ç¾çÀÏ °¡´É¼ºÀÌ ³ô´Ù
            if( bIsAuto )
            {
                if( m_iQualityCount < 4 )
                {
                    // ±×·£µåÃ¼ÀÌ½º ÀÌ¿Ü¿¡ ´Ù¸¥ ÇÁ·Î±×·¥ÀÌ ½ÇÇàµÇ¾î ÀÖ´Ù¸é Á¾·á½ÃÄÑ ÁÖ¼¼¿ä.
                    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_AUTO_GRAPHIC_QUALITY_MESSAGE2 ), KGCChatManager::CHAT_TYPE_INVITE );

                    // ÀÚµ¿ ±×·¡ÇÈ Ç°Áú Á¶Àý »ç¿ë ¿©ºÎ´Â ¿É¼ÇÃ¢¿¡¼­ Á¶Àý ÇÒ ¼ö ÀÖ½À´Ï´Ù.
                    if( rand() % 4 == 0 )
                        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_AUTO_GRAPHIC_QUALITY_MESSAGE3 ), KGCChatManager::CHAT_TYPE_INVITE );
                }
            }
        }
    }

    g_pGraphicQuality->SetQuality( m_iQualityCount );
}

void KGCOptionManager::Load_Option()
{
    // Default ¼³Á¤À» ÇÑ´Ù.(ÆÄÀÏÀÌ ¾øÀ» ½Ã¿¡...)
#if defined(NATION_CHINA)
    m_bModulate2X = false;
    m_bUsingVS = false;
    m_bAutoGraphicQuality = false;
#else
    m_bAutoGraphicQuality = true;
#endif

    m_bIsCartoon = false;
    g_MyD3D->Default_Width = static_cast<USHORT>(GC_SCREEN_WIDTH);
    g_MyD3D->Default_Height = static_cast<USHORT>(GC_SCREEN_HEIGHT);

#if defined( NATION_TAIWAN ) || defined( NATION_CHINA )
    g_MyD3D->Default_Fullscreen = false;
#else
    g_MyD3D->Default_Fullscreen = true;
#endif
    m_bJoystic = false;
    m_bShadow = true;
    m_b3Back = true;
    m_b1Back = true;
    m_bLinearFilter = true;

    m_bSoundEnable = true;
    m_bMusicEnable = true;

    m_fSoundVolume = -2500.0f;
    m_fMusicVolume = -2500.0f;

    m_bWhisperFilter = false;
    m_bHitEffect = true;
    m_bSaveID = false;
    m_bMotionBlur = true;
    m_bQualitySquare = true;
    m_bDenyInviteMsg = false;
	m_iIndexColorTable = 0;
    m_ucLastState = 0;
    m_eSampleinfo = D3DMULTISAMPLE_4_SAMPLES;//D3DMULTISAMPLE_NONE;
    
    m_ucLastState = 0;
    m_ucLastWorldMap = (BYTE)WORLDMAP_UPPERMAP;


    HANDLE DBF;
    DWORD temp;

    int iVer = 0;

    if((DBF=CreateFileA("OptionVer2.dat", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))!=INVALID_HANDLE_VALUE)
    {
        ReadFile(DBF, &iVer, sizeof(int), &temp, NULL);
		//printf( "Ver = %d\n", iVer);
        switch(iVer)
        {
        case 10002:
		case 10003:
            LoadOptionData_10002(DBF);
            break;
        }
		CloseHandle(DBF);
    }

    // ±Ó¼Ó¸» Â÷´Ü ±â´ÉÀÌ µé¾î°¡ ÀÖ´Â ³ª¶ó¸¸ Â÷´Ü ¼³Á¤
    g_pkChatManager->SetWhisperFilter( m_bWhisperFilter );

    m_iQualityCount=0;
	if( m_bMotionBlur ) m_iQualityCount = 6;
    else if( m_bIsCartoon ) m_iQualityCount=5;
    else if( m_bModulate2X ) m_iQualityCount=4;
    else if( m_bShadow ) m_iQualityCount=3;
    else if( m_b3Back ) m_iQualityCount=2;
    else if( m_b1Back ) m_iQualityCount=1;
    else if( m_bLinearFilter ) m_iQualityCount=0;

    switch( m_iQualityCount )
    {
    case 6:
        SiKGCRenderManager()->SetRenderTech(1);//0);
        break;
    case 5:
        SiKGCRenderManager()->SetRenderTech(1);
        break;
    default:
        SiKGCRenderManager()->SetRenderTech(2);
        break;
    }

    m_bEnableKeySetting = false;

    m_KeyTable[0] = DIK_UP;
    m_KeyTable[1] = DIK_DOWN;
    m_KeyTable[2] = DIK_LEFT;
    m_KeyTable[3] = DIK_RIGHT;
    m_KeyTable[4] = DIK_Z;
	m_KeyTable[5] = DIK_V;
    m_KeyTable[6] = DIK_X;
    m_KeyTable[7] = DIK_LALT;
    m_KeyTable[8] = DIK_SPACE;
    m_KeyTable[9] = DIK_LSHIFT;
	m_KeyTable[10] = DIK_C;
	m_KeyTable[11] = DIK_N;

#ifdef WEAPONCHANGE 
    m_KeyTable[12] = DIK_LCONTROL;
    m_KeyTable[13] = DIK_A;
	m_KeyTable[14] = DIK_S;
	m_KeyTable[15] = DIK_D;
	m_KeyTable[16] = DIK_F;
	m_KeyTable[17] = DIK_G;
#else
    m_KeyTable[10] = DIK_A;
    m_KeyTable[11] = DIK_A;
    m_KeyTable[12] = DIK_S;
    m_KeyTable[13] = DIK_D;
    m_KeyTable[14] = DIK_F;
    m_KeyTable[15] = DIK_G;
#endif

    for(int i = 0;i<NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i){
        m_JoystickKeyTable[i] = static_cast<BYTE>(i);
    }

	if((DBF=CreateFileA("ResoultionSettingVer3.dat", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))!=INVALID_HANDLE_VALUE)
	{
		ReadFile(DBF, &m_dwScreenMode, sizeof(DWORD), &temp, NULL);
		ReadFile(DBF, &m_eSampleinfo, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL );
		CloseHandle(DBF);
	}
	else
	{
		m_dwScreenMode = INT_MAX;
	}

    if((DBF=CreateFileA("KeySettingVer2.dat", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))!=INVALID_HANDLE_VALUE)
    {
        BYTE tempKey;
		DWORD dwVer;
		ReadFile(DBF, &dwVer, sizeof(DWORD), &temp, NULL);
        ReadFile(DBF, &m_bEnableKeySetting, sizeof(bool), &temp, NULL);
        for( int i=0; i<NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i )
        {
            if( ReadFile(DBF, &tempKey, sizeof(BYTE), &temp, NULL) )
			{
				m_KeyTable[i] = tempKey;
			}
		}
        CloseHandle(DBF);
    }

    if((DBF=CreateFileA("JoystickSettingVer2.dat", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))!=INVALID_HANDLE_VALUE)
    {
        BYTE tempKey;
		DWORD dwVer;
		ReadFile(DBF, &dwVer, sizeof(DWORD), &temp, NULL);
        ReadFile(DBF, &m_bEnableKeySetting, sizeof(bool), &temp, NULL);
		for (int i = 0; i < NUM_USE_KEY + NUM_USE_SKILL_KEY; ++i)
		{
			if (ReadFile(DBF, &tempKey, sizeof(BYTE), &temp, NULL))
			{
				m_JoystickKeyTable[i] = tempKey;
			}
		}
        CloseHandle(DBF);
    }

    if ((DBF = CreateFileA("OtherOptions.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
        BYTE tempKey;
        DWORD dwVer;
        ReadFile(DBF, &m_bSkillEffect, sizeof(bool), &temp, NULL);
        ReadFile(DBF, &m_bUsingVS, sizeof(bool), &temp, NULL);
        ReadFile(DBF, &m_bPetViewHide, sizeof(bool), &temp, NULL);
        ReadFile(DBF, &m_bBasicEquipEnable, sizeof(bool), &temp, NULL);
        ReadFile(DBF, &m_bRoomAnimationEnable, sizeof(bool), &temp, NULL);
        CloseHandle(DBF);
    }
}

void KGCOptionManager::InitDevice( LPDIRECT3DDEVICE9 pd3dDevice )
{
    m_pd3dDevice = pd3dDevice;
}
#if defined (NATION_THAILAND)
void KGCOptionManager::SetSaveFunboxPostFix( bool bSave, int iFunboxPostFix)
{
	m_bSaveID = bSave;
	if( bSave )
		m_iFunBoxPostfix= iFunboxPostFix;
	else
		m_iFunBoxPostfix=0;
}
#endif

void KGCOptionManager::SetSaveID( bool bSave, WCHAR* wcsUserID )
{
    m_bSaveID = bSave;
    if( bSave )
        wcscpy( m_wcsUserID, wcsUserID );
    else
        ZeroMemory( m_wcsUserID, sizeof(WCHAR) * MAX_LOGIN_ID_SIZE );
}

bool KGCOptionManager::IsCartoon()
{
    if ( SiKGCRenderManager()->GetUsingVS() )
        return true;
  
    return false;
}

D3DMULTISAMPLE_TYPE KGCOptionManager::GetSampleInfoFromNum(int iNum)
{
	D3DMULTISAMPLE_TYPE eRet;
	switch(iNum)
	{
	case 1:
		eRet = D3DMULTISAMPLE_2_SAMPLES;
		break;
	case 2:
		eRet = D3DMULTISAMPLE_4_SAMPLES;
		break;
    case 3:
        eRet = D3DMULTISAMPLE_8_SAMPLES;
        break;
	default:
		eRet = D3DMULTISAMPLE_NONE;
		break;
	}

	return eRet;
}


std::wstring KGCOptionManager::GetStringSampleInfo(D3DMULTISAMPLE_TYPE eSampleinfo)
{
	std::wstring strResult =  L"";
	switch( eSampleinfo )
	{
	case D3DMULTISAMPLE_NONE: //Áö¿ø ¾ÈÇÔ
		strResult = L"None Antialiasing";
		break;
	case D3DMULTISAMPLE_2_SAMPLES: //2¹è
		strResult = L"MultiSampling 2x";
		break;
	case D3DMULTISAMPLE_4_SAMPLES: //4¹è
		strResult = L"MultiSampling 4x";
		break;
    case D3DMULTISAMPLE_8_SAMPLES:
        strResult = L"MultiSampling 8x";

        break;
	}
	return strResult;
}

int KGCOptionManager::GetSampleInfoNum()
{
	int iRet = 0;
	switch( m_eSampleinfo )
	{
	case D3DMULTISAMPLE_NONE: //Áö¿ø ¾ÈÇÔ
		iRet=0;
		break;
	case D3DMULTISAMPLE_2_SAMPLES: //2¹è
		iRet=1;
		break;
	case D3DMULTISAMPLE_4_SAMPLES: //4¹è
		iRet=2;
		break;
    case D3DMULTISAMPLE_8_SAMPLES: //4¹è
        iRet=3;
        break;
	}
	return iRet;
}

D3DMULTISAMPLE_TYPE KGCOptionManager::GetSampleInfo()
{
	return m_eSampleinfo;
}


int KGCOptionManager::GetNumOfResolution()
{
    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    return g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwNumModes;
}

bool KGCOptionManager::GetResolution( IN int iMode, OUT D3DModeInfo& sResolution )
{
    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    if( iMode < 0 || iMode >= GetNumOfResolution() )
        return false;

    sResolution = pDevice->modes[iMode];

    return  true;
}

bool KGCOptionManager::IsEnableResolution( DWORD width_, DWORD height_ )
{
    //DWORD dwRatio = width_*100/height_;
    if (   (width_ == 800 && height_ == 600) 
		|| (width_ == 1024 && height_ == 768) 
		|| (width_ == 1280 && height_ == 800) 
		|| (width_ == 1280 && height_ == 1024) 
		|| (width_ == 1366 && height_ == 768)
		|| (width_ == 1440 && height_ == 900) 
		|| (width_ == 1600 && height_ == 900)) 
        return true;

#if defined( NATION_USA )
    if ( ( width_ == 1680 && height_ == 1050 ) ) {
        return true;
    }
#endif

        return false;
}

BOOL KGCOptionManager::SetSampleing( D3DMULTISAMPLE_TYPE eSample )
{
	if (!(eSample == D3DMULTISAMPLE_NONE || eSample == D3DMULTISAMPLE_2_SAMPLES || eSample == D3DMULTISAMPLE_4_SAMPLES || eSample == D3DMULTISAMPLE_8_SAMPLES))
	{
        eSample = D3DMULTISAMPLE_4_SAMPLES;//D3DMULTISAMPLE_NONE;
	}

	if (g_MyD3D->ChangeSampleing(eSample)==true)
		g_MyD3D->m_KGCOption.m_eSampleinfo = eSample;
	else
		return FALSE;

	return TRUE;


}

bool KGCOptionManager::SetResolution( int iMode )
{
    if( iMode < 0 || iMode >= GetNumOfResolution() )
        return false;

    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    m_dwPrevMode = m_dwCurrentMode;
    if( g_MyD3D->IsWindowMode() == TRUE ) {
        int iLocalWidth  = ::GetSystemMetrics( SM_CXSCREEN );
        int iLocalHeight = ::GetSystemMetrics( SM_CYSCREEN );

        if( static_cast<int>( pDevice->modes[iMode].Width ) > iLocalWidth )
        {  
            return false;
        }
        if( static_cast<int>( pDevice->modes[iMode].Height ) > iLocalHeight )
        {
            return false;
        }
		m_dwCurrentMode = iMode;
		m_dwFullScreenMode = iMode;
		m_dwScreenMode = iMode;

    }
	else {
		m_dwCurrentMode = iMode;
		m_dwFullScreenMode = iMode;
		m_dwScreenMode = iMode;
	}

    g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwCurrentMode = m_dwCurrentMode;

	g_MyD3D->m_KGCOption.m_fScaleX = pDevice->modes[m_dwCurrentMode].Width / GC_SCREEN_FLOAT_WIDTH;
	g_MyD3D->m_KGCOption.m_fScaleY = pDevice->modes[m_dwCurrentMode].Height / GC_SCREEN_FLOAT_HEIGHT;

    return true;
}

//¿©±â¼­ ¿É¼Ç °ª¿¡ ¸ÂÃß¾î ¼³Á¤ÇÑ´Ù.
void KGCOptionManager::InitResolutionOption()
{
    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    m_dwBasicMode = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwBasicMode;
	m_dwDefaultMode = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwDefaultMode;

    g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwCurrentMode = m_dwDefaultMode;

	//±âÁ¸ ¿É¼ÇÆÄÀÏ¿¡ Á¤º¸°¡ ÀÖ´Â °æ¿ì¿¡´Â °¡Á® ¿Â´Ù. (800x600)ÀÏ¶© Á¦¿Ü
	if (m_dwScreenMode==INT_MAX)
	{
		if (m_dwCurrentModeOld != m_dwBasicMode  &&
			m_dwFullScreenModeOld != m_dwBasicMode)
		{
			m_dwScreenMode = m_dwCurrentModeOld;		
		}
		else
		{
			m_dwScreenMode = m_dwDefaultMode;
		}
	}

    //  ÇöÀç ¼³Á¤µÈ ½Ã½ºÅÛ ÇØ»óµµ º¸´Ù Å« ÇØ»óµµ·Î ¼³Á¤µÇ¾î ÀÖ´Ù¸é 800/600À¸·Î ¼³Á¤ÇÑ´Ù.
    int iLocalWidth  = ::GetSystemMetrics( SM_CXSCREEN );
    int iLocalHeight = ::GetSystemMetrics( SM_CYSCREEN );

    if( static_cast<int>( pDevice->modes[m_dwScreenMode].Width ) > iLocalWidth || 
        static_cast<int>( pDevice->modes[m_dwScreenMode].Height ) > iLocalHeight )
    {  
        m_dwScreenMode = m_dwBasicMode;
    }

    m_dwCurrentMode = m_dwScreenMode;
    m_dwFullScreenMode = m_dwScreenMode;



    if( m_dwCurrentMode >= g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwNumModes )
        m_dwCurrentMode = m_dwDefaultMode;

    if( m_dwFullScreenMode >= g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwNumModes )
        m_dwFullScreenMode = m_dwDefaultMode;

    SetResolution( m_dwFullScreenMode );
}

DWORD KGCOptionManager::GetCurrentMode()
{
    return (g_MyD3D->IsWindowMode() == TRUE ? m_dwCurrentMode : m_dwFullScreenMode);
}

D3DXVECTOR2 KGCOptionManager::GetWindowScale()
{
    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    float fScaleX = pDevice->modes[GetCurrentMode()].Width / GC_SCREEN_FLOAT_WIDTH;
    float fScaleY = pDevice->modes[GetCurrentMode()].Height / GC_SCREEN_FLOAT_HEIGHT;

    return D3DXVECTOR2( fScaleX, fScaleY );
}

float KGCOptionManager::GetWindowScaleX()
{

    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    return pDevice->modes[GetCurrentMode()].Width / GC_SCREEN_FLOAT_WIDTH;
}

float KGCOptionManager::GetWindowSacleY()
{

    DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
    D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

    return pDevice->modes[GetCurrentMode()].Height / GC_SCREEN_FLOAT_HEIGHT;
}

void KGCOptionManager::LoadOptionData_10002( HANDLE& DBF )
{
    DWORD temp;

    ReadFile(DBF, &m_bIsCartoon, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &g_MyD3D->Default_Width, sizeof(unsigned short), &temp,NULL); //<---ÀÌ°Ç µµµ¥Ã¼ Á¤Ã¼°¡ ¹»±î-_-;
    ReadFile(DBF, &g_MyD3D->Default_Height, sizeof(unsigned short), &temp,NULL);
    ReadFile(DBF, &g_MyD3D->Default_Fullscreen, sizeof(bool), &temp, NULL);

#if defined( NATION_TAIWAN ) || defined(NATION_CHINA)
    g_MyD3D->Default_Fullscreen = false;
#elif defined(NO_USE_WINDOW_MODE) && defined(__PATH__)
    g_MyD3D->Default_Fullscreen = true;
#endif

	g_MyD3D->Default_Fullscreen = false;

    if(g_MyD3D->Default_Fullscreen)
        g_MyD3D->SetWindowMode( FALSE );
    else
        g_MyD3D->SetWindowMode( TRUE );

    ReadFile(DBF, &m_bJoystic, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bShadow, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_b3Back, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bModulate2X, sizeof(bool), &temp, NULL);

    if(temp==0)
    {
        // µÚ¿¡ µû¶ó ºÙ´Â Á¤º¸°¡ ¾øÀ» ¶§ µðÆúÆ® ¼³Á¤ÇÑ´Ù.
        if(EnableModulate2X)
            m_bModulate2X=true;
        else
            m_bModulate2X=false;
    }

    ReadFile(DBF, &m_b1Back, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bLinearFilter, sizeof(bool), &temp, NULL);

    if(temp==0)
    {
        // µÚ¿¡ µû¶ó ºÙ´Â Á¤º¸°¡ ¾øÀ» ¶§ µðÆúÆ® ¼³Á¤ÇÑ´Ù.
        m_b1Back = true;
        m_bLinearFilter = true;
    }

    ReadFile(DBF, &m_bSoundEnable, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bMusicEnable, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bWhisperFilter, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bHitEffect, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bMotionBlur, sizeof(bool), &temp, NULL);
    ReadFile(DBF, &m_bAutoGraphicQuality, sizeof(bool), &temp, NULL);

    ReadFile(DBF, &m_fSoundVolume, sizeof(float), &temp, NULL);
    ReadFile(DBF, &m_fMusicVolume, sizeof(float), &temp, NULL);

    ReadFile(DBF, &m_bSaveID, sizeof(bool), &temp, NULL );

    if( temp != 0 )
    {
        ReadFile(DBF, m_wcsUserID, sizeof(WCHAR) * MAX_LOGIN_ID_SIZE, &temp, NULL );
    }
    else//ID ÀúÀå°ü·ÃÀÌ ¾øÀ¸¸é..
    {
        m_bSaveID = false;
        ZeroMemory( m_wcsUserID, sizeof(WCHAR) * MAX_LOGIN_ID_SIZE );
    }


    READ_OPTION(m_bSkillEffect, sizeof(bool), true);

    READ_OPTION(m_bQualitySquare, sizeof(bool), true );
    READ_OPTION(m_bSquareItemEffect, sizeof(bool), true );
    READ_OPTION(m_bDenyInviteMsg, sizeof(bool), false );

    ReadFile(DBF, &m_ucLastState, sizeof(BYTE), &temp, NULL);
    ReadFile(DBF, &m_ucLastWorldMap, sizeof(BYTE), &temp, NULL);

    ReadFile(DBF, &m_dwCurrentModeOld, sizeof(DWORD), &temp, NULL);
    ReadFile(DBF, &m_dwFullScreenModeOld, sizeof(DWORD), &temp, NULL);




#if defined (NATION_THAILAND)
    ReadFile(DBF, &m_iFunBoxPostfix, sizeof(int), &temp, NULL );
    ReadFile(DBF, &m_eSampleinfoOld, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL );
#else
    ReadFile(DBF, &m_eSampleinfoOld, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL );
    ReadFile(DBF, &m_eSampleinfoOld, sizeof(D3DMULTISAMPLE_TYPE), &temp, NULL );

#endif
    // ¿É¼Ç ¼ø¼­¸¦ ÁöÄÑÁÖÁö ¾Ê°í ¾û¸ÁÀ¸·Î ²¿¿©¼­ ÀÏ´ÜÀº Áßº¹ ¸®µå°°Àº°É ±×´ë·Î À¯Áö½ÃÅ²´Ù.
    // m_dwFullScreenModeOld¸¸ÀÌ ÇØ»óµµ ¸ðµå·Î Àû¿ëµÈ´Ù.  CurrentMode´Â ¸®µåÇØµµ ÀÇ¹Ì°¡ ¾øÀ½.
    // ÇÏÁö¸¸ Á¤»óÀûÀ¸·Î ÀúÀå µÈ´Ù¸é, °°Àº °ªÀ» °¡Áö°Ô µÉ °ÍÀÌ´Ù.

#if defined (SAVE_CHATCOLOR_INFO)
    ReadFile(DBF, &m_iIndexColorTable, sizeof(int), &temp, NULL );
#endif
#if defined (ITEMBOX_ANI_TOGGLE)
    ReadFile(DBF, &m_bAlphaAni, sizeof(bool), &temp, NULL );
#endif
}