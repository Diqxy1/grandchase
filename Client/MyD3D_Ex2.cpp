#include "stdafx.h"
#include "KMci.h"
#include "GCStateGame.h"
#include "Headup Display.h"
#include "Stage.h"
#include "Monster.h"
#include "Controls.h"
#include "Item.h"
#include "KGCGameBoard.h"
#include "jpeg/header/ximage.h"
#include "KGCRoomManager.h"
#include "KGCChannelManager.h"
#include "Procedure.h"
#include "GCCoupleSystem.h"
#include "./Square/GCSquare.h"
#include "KGCDeathMatch.h"

static int g_aiSysopMsgID[] =
{
    STR_ID_AUTO_MESSAGE0,
    STR_ID_AUTO_MESSAGE1,
    STR_ID_AUTO_MESSAGE2,
    STR_ID_AUTO_MESSAGE3,
    STR_ID_AUTO_MESSAGE4,
    STR_ID_AUTO_MESSAGE5,
    STR_ID_AUTO_MESSAGE6,
    STR_ID_AUTO_MESSAGE7,
    STR_ID_AUTO_MESSAGE8,
    STR_ID_AUTO_MESSAGE9,
    STR_ID_AUTO_MESSAGE10,
    STR_ID_AUTO_MESSAGE11,
    STR_ID_AUTO_MESSAGE12,
    STR_ID_AUTO_MESSAGE13,
    STR_ID_AUTO_MESSAGE14,
    STR_ID_AUTO_MESSAGE15,
    STR_ID_AUTO_MESSAGE16,
    STR_ID_AUTO_MESSAGE17,
    STR_ID_AUTO_MESSAGE18,
    STR_ID_AUTO_MESSAGE19,
    STR_ID_AUTO_MESSAGE20,
    STR_ID_AUTO_MESSAGE21,
    STR_ID_AUTO_MESSAGE22,
    STR_ID_AUTO_MESSAGE23,
    STR_ID_AUTO_MESSAGE24,
    STR_ID_AUTO_MESSAGE25,
    STR_ID_AUTO_MESSAGE26,
    STR_ID_AUTO_MESSAGE27,
    STR_ID_AUTO_MESSAGE28,
    STR_ID_AUTO_MESSAGE29,
    STR_ID_AUTO_MESSAGE30,
    STR_ID_AUTO_MESSAGE31,
    STR_ID_AUTO_MESSAGE32,
    STR_ID_AUTO_MESSAGE33,
    STR_ID_AUTO_MESSAGE34,
    STR_ID_AUTO_MESSAGE35,
    STR_ID_AUTO_MESSAGE36,
    STR_ID_AUTO_MESSAGE37,
    STR_ID_AUTO_MESSAGE38,
    STR_ID_AUTO_MESSAGE39,
    STR_ID_AUTO_MESSAGE40,
    STR_ID_AUTO_MESSAGE41,
    STR_ID_AUTO_MESSAGE42,
    STR_ID_AUTO_MESSAGE43,
#if defined(NATION_CHINA)
    STR_ID_AUTO_MESSAGE44,
    STR_ID_AUTO_MESSAGE45,
    STR_ID_AUTO_MESSAGE46,
    STR_ID_AUTO_MESSAGE47,
    STR_ID_AUTO_MESSAGE48,
    STR_ID_AUTO_MESSAGE49,
    STR_ID_AUTO_MESSAGE50,
    STR_ID_AUTO_MESSAGE51,
    STR_ID_AUTO_MESSAGE52,
    STR_ID_AUTO_MESSAGE53,
    STR_ID_AUTO_MESSAGE54,
#endif // end NATION_CHINA
    STR_ID_AUTO_MESSAGE55,
    STR_ID_AUTO_MESSAGE56,
};

#define NUM_SYSOP_MESSAGE (sizeof(g_aiSysopMsgID)/sizeof(int))

const WCHAR* RobotMessage( void )
{
    static DWORD dwTickTime = ::timeGetTime();

    // 2분에 한번씩 도움말을 리턴
    if ( dwTickTime + 120000 < ::timeGetTime() )
    {
        dwTickTime = ::timeGetTime();

#ifdef NATION_KOREA
        // 미션 홍보를 위해서
        switch( ::rand()%2 )
        {
        case 1:
            return g_pkStrLoader->GetString( g_aiSysopMsgID[ 42 + ::rand()%2 ] ).c_str();
            break;
        case 0:
            return g_pkStrLoader->GetString( g_aiSysopMsgID[::rand()%NUM_SYSOP_MESSAGE] ).c_str();
            break;
        }
#else
        return g_pkStrLoader->GetString( g_aiSysopMsgID[::rand()%NUM_SYSOP_MESSAGE] ).c_str();
#endif
    }

    return NULL;
}

// 스크린샷
HRESULT CMyD3DApplication::ScreenShot()
{
    // 파일 이름
    WCHAR fileName[MAX_PATH];
    tm *newtime;
    time_t aclock;
    time( &aclock );                 /* Get time in seconds */
    newtime = localtime( &aclock );
    swprintf( fileName, MAX_PATH, L"ScreenShot\\GrandChase%04d%02d%02d_%02d%02d%02d.PNG",
        newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec );

    IDirect3DSurface9* frontbuf = NULL;// this is our pointer to the memory location
    // containing our copy of the front buffer

    HRESULT hr = S_OK;
    D3DDISPLAYMODE mode = { 0, };

    // now we get display mode of device
    if (FAILED(hr = m_pd3dDevice->GetDisplayMode( 0, &mode)))
    {
        return E_FAIL;
    }

    // now we create the image that our screen shot will be copied into
    if (FAILED(hr = m_pd3dDevice->CreateOffscreenPlainSurface(mode.Width,
        mode.Height,
        D3DFMT_A8R8G8B8,
        D3DPOOL_SCRATCH,
        &frontbuf, NULL)))
    {
        return E_FAIL;
    }

    if (NULL == frontbuf) return E_FAIL;

    // now we copy the front buffer into our surface
    if (FAILED(hr = m_pd3dDevice->GetFrontBufferData( 0, frontbuf)))
    {
        frontbuf->Release(); // release the surface so there is no memory leak
        return E_FAIL;
    }

    // now we calculate rectangle of guild mark
    RECT rect;
    ::GetClientRect(m_hWnd, &rect);
    ::ClientToScreen(m_hWnd, reinterpret_cast<POINT*>(&rect.left));
    rect.right = static_cast<LONG>(rect.left + W_Width);
    rect.bottom = static_cast<LONG>(rect.top + W_Height);

    // now write our screen shot to a bitmap file
    if (FAILED(hr = ::D3DXSaveSurfaceToFile(fileName,
        D3DXIFF_PNG,
        frontbuf,
        NULL,
        &rect)))
    {
        frontbuf->Release();
        return E_FAIL;
    }

    // release the surface so there is no memory leak
    frontbuf->Release();

    if ( g_pkChatManager != NULL && g_pkStrLoader != NULL)
    {
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString(STR_ID_SNAPSHOT_SAVE, "ss", GCUTIL_STR::GCStrCharToWide( g_strCurrentPath.c_str() ), fileName ), KGCChatManager::CHAT_TYPE_SCREEN_SHOT );
    }


    return S_OK;
}

void CMyD3DApplication::Clear_PlayerData()
{
    for( int i = 0 ; i < MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO; ++i )
        MyPlayer[i]->ClearPlayerData();
    SiKGCRoomManager()->GetRoomInfo().usUsers = 0;
    SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo.clear();
    SiKGCRoomManager()->GetRoomInfo().dwHostUID = 0;

    SiGCSquare()->CloseSquare();
}

void CMyD3DApplication::PlayGameBGM()
{
    // 퀘스트는 Manager에서 알아서 함다
    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
        return;
    if( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
        return;
    if( GC_GMC_AGIT == SiKGCRoomManager()->GetGameModeCategory() )
        return;

    if( MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS )
    {
        g_KMci.Play( GC_BGM_X_MAS, true );
    }
    else if( MyStg->GetStage() == GC_GS_GUILD )
    {
        g_KMci.Play( GC_BGM_LETS_DO_IT, true );
    }
    else if( SiKGCRoomManager()->GetGameMode() == GC_GM_SURVIVAL_MODE )
    {
        g_KMci.Play( GC_BGM_NEW_WORLD, true );
    }
    else if( SiKGCRoomManager()->GetGameMode() == GC_GM_MONSTER_CRUSADER )
    {
        g_KMci.Play( GC_BGM_INTO_THE_DARKNESS, true );
    }
    else
    {
        g_KMci.Play( GC_BGM_CASTLE, true );
    }
}

// 10초간 평균 프레임이 48프레임 이하라면 그래픽 품질을 조절해 준다
#define MIN_FPS 45
#define MAX_SECOND 60
#define MIN_QUALITYCOUNT 3
void CMyD3DApplication::AutoSetPerformance( int iFPS )
{
    if( m_KGCOption.IsAutoGraphicQuality() )
    {
        if( m_pStateMachine->GetState() == GS_GAME && g_MyD3D->Story >= ( GC_FPS_LIMIT * 5 ) )
        {
            m_iFPS_for_AutoGraphicQuality += iFPS;
            m_iFPSCount_for_AutoGraphicQuality++;

            if( m_iFPSCount_for_AutoGraphicQuality >= MAX_SECOND )
            {
                int iAveFPS = (int)( m_iFPS_for_AutoGraphicQuality / m_iFPSCount_for_AutoGraphicQuality );
                if( iAveFPS < MIN_FPS && m_KGCOption.IsAutoGraphicQuality() )
                {
                    m_KGCOption.Quality_Change( -1,true );
                }

                m_iFPS_for_AutoGraphicQuality = 0;
                m_iFPSCount_for_AutoGraphicQuality = 0;
            }

            if( m_bInitFPS_for_AutoGraphicQuality )
                m_bInitFPS_for_AutoGraphicQuality = false;
        }
        else if( !m_bInitFPS_for_AutoGraphicQuality )
        {
            m_bInitFPS_for_AutoGraphicQuality = true;

            m_iFPS_for_AutoGraphicQuality = 0;
            m_iFPSCount_for_AutoGraphicQuality = 0;

            int iQualityCount = m_KGCOption.GetQualityCount();
            if( iQualityCount < MIN_QUALITYCOUNT && m_iLowQualityCounter < 5 )
            {
                m_iLowQualityCounter++;
                m_KGCOption.Quality_Change( MIN_QUALITYCOUNT - iQualityCount);
            }
        }
    }
}

void CMyD3DApplication::SetAveFPS( int iFPS )
{
    // 게임중일때만 체크한다. 스테이지 진입후 최초 5초이후에 fps 갱신한다.
    if( m_pStateMachine->GetState() == GS_GAME && g_MyD3D->Story >= ( GC_FPS_LIMIT * 5 ) )       
    {
        int nStoryGap = g_MyD3D->Story - m_nLastFPSTick;

        //  1초마다 FPS 갱신
        if ( nStoryGap >= GC_FPS_LIMIT )
        {
            DWORD dwTotalFrame = m_iAveFPS_in_PlayGame * m_iAveFPSCount_in_PlayGame + iFPS;
            m_iAveFPSCount_in_PlayGame++;
            m_iAveFPS_in_PlayGame = (int)( (float)dwTotalFrame / (float)m_iAveFPSCount_in_PlayGame );
            m_nLastFPSTick = g_MyD3D->Story;

            if( m_bInitAveFPS_in_PlayGame )
                m_bInitAveFPS_in_PlayGame = false;
        }
    }
    else if( !m_bInitAveFPS_in_PlayGame )
    {
        SiGCClientErrorManager()->Send_AveFPSReport( m_iAveFPS_in_PlayGame );
        ClearAveFPS();
    }
}

int CMyD3DApplication::GetAveFPS()
{
    return m_iAveFPS_in_PlayGame;
}

void CMyD3DApplication::ClearAveFPS()
{
    m_nLastFPSTick = 0;
    m_bInitAveFPS_in_PlayGame = true;
    m_iAveFPS_in_PlayGame = 0;
    m_iAveFPSCount_in_PlayGame = 0;
}

void CMyD3DApplication::ScreenShotJPEG()
{
    if( g_MyD3D->m_pStateMachine->GetState() > GS_LOGO )
    {
        if( MakeJPEGFile() == false )
        {
            if ( g_pkChatManager != NULL )
            {
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_SNAPSHOT_SAVE_FAILED), KGCChatManager::CHAT_TYPE_SCREEN_SHOT );
            }
        }
    }
}

bool CMyD3DApplication::MakeJPEGFile()
{
    DWORD currentTime = timeGetTime();
    if ( currentTime - m_dwLastSaveScreenShot < 1000 )
        return true;
    else
        m_dwLastSaveScreenShot = currentTime;

    CreateDirectoryA("./ScreenShot", NULL);

    WCHAR fileName[MAX_PATH];
    SYSTEMTIME st;
    GetLocalTime( &st );
    swprintf( fileName, MAX_PATH, L"ScreenShot\\First_%04d%02d%02d_%02d%02d%02d.JPG", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

    RECT rect;
    GetClientRect( m_hWnd, &rect );
    HDC hScrDC = GetDC( m_hWnd );
    HDC hMemDC = CreateCompatibleDC( hScrDC );
    HBITMAP hBitmap = CreateCompatibleBitmap( hScrDC, rect.right - rect.left, rect.bottom - rect.top );
    HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

    BitBlt( hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hScrDC, rect.left, rect.top, SRCCOPY );
    SelectObject( hMemDC, hOldBitmap );

    CxImage cimage;
    if ( cimage.CreateFromHBITMAP( hBitmap ) )
    {
        int jpegQuality = ( g_MyD3D->m_KGCOption.GetQualityCount() > 5 ) ? 99 : 75;
        cimage.SetJpegQuality( jpegQuality );
        cimage.Save( fileName, CXIMAGE_FORMAT_JPG );
    }
    else
    {
        DeleteObject( hBitmap );
        DeleteDC( hMemDC );
        ReleaseDC( m_hWnd, hScrDC );
        return false;
    }

    DeleteObject( hBitmap );
    DeleteDC( hMemDC );
    ReleaseDC( m_hWnd, hScrDC );

    if ( g_pkChatManager != NULL )
    {
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_SNAPSHOT_SAVE, "ss", GCUTIL_STR::GCStrCharToWide( g_strCurrentPath.c_str() ), fileName ), KGCChatManager::CHAT_TYPE_SCREEN_SHOT, false, false );
        g_KDSound.Play( "473" ); // 사진찍는소리 찰칵~
    }

    if( ::IsClipboardFormatAvailable( CF_BITMAP ) == TRUE )
    {
        if( ::OpenClipboard( m_hWnd ) )
        {
            ::EmptyClipboard();
            ::CloseClipboard();
        }
    }

    m_wstrScreenShotFileName = fileName;

    return true;
}
#if defined(USE_JOYSTICK)
void CMyD3DApplication::ReadJoystick(  )
{
    MyCtrl->ReadJoystick();
}

int CMyD3DApplication::GetJoystickKey(  )
{
    ReadJoystick();
    return MyCtrl->GetJoystickKey();
}

int CMyD3DApplication::GetJoystickSelectedKey( int index)
{
    return MyCtrl->GetJoystickSelectedKey(index);
}

void CMyD3DApplication::SetJoystickKey( BYTE key, int sel )
{
    for(int i = 0;i<NUM_USE_KEY + NUM_USE_SKILL_KEY;++i){
        if( m_KGCOption.m_JoystickKeyTable[i] == key ){
            m_KGCOption.m_JoystickKeyTable[i] = m_KGCOption.m_JoystickKeyTable[sel];
            break;
        }
    }

    m_KGCOption.m_JoystickKeyTable[sel] = key;
    SetJoystickKey(false);
}

void CMyD3DApplication::SetJoystickKey( bool bDefualt )
{
    for(int i = 0;i<NUM_USE_KEY + NUM_USE_SKILL_KEY;++i){
        MyCtrl->m_aiSettingGamePad[i] = m_KGCOption.m_JoystickKeyTable[i];
    }

    MyCtrl->SetVirtualGamePad(bDefualt);

}


#endif
void CMyD3DApplication::UpdateKeySetting( bool bDefault )
{
    if( bDefault )
    {
        MyCtrl->m_keyUp             = DIK_UP;
        MyCtrl->m_keyDown           = DIK_DOWN;
        MyCtrl->m_keyLeft           = DIK_LEFT;
        MyCtrl->m_keyRight          = DIK_RIGHT;
        MyCtrl->m_keyAttack         = DIK_Z;
        MyCtrl->m_keyUseItem        = DIK_LMENU;
        MyCtrl->m_keyChangeItem     = DIK_X;
        MyCtrl->m_keyChangeSkill    = DIK_SPACE;
        MyCtrl->m_keySpecialSkillKey= DIK_C;
#ifdef WEAPONCHANGE
        MyCtrl->m_keyWeaponSwap     = DIK_LCONTROL;
#endif

#if defined( USE_ONGAME_INVENTORY )
        MyCtrl->m_keyGameInventory = DIK_I;
#endif

        MyCtrl->m_keyCameraZoom = DIK_B;
#if !defined(__PATH__)
        MyCtrl->m_keyMonsterDirection = DIK_P;
#endif

        MyCtrl->m_keyProvocation    = DIK_V;
        MyCtrl->m_keyUsePetSkill    = DIK_LSHIFT;

		MyCtrl->m_vecKeySkill[0]	= DIK_A;
		MyCtrl->m_vecKeySkill[1]	= DIK_S;
		MyCtrl->m_vecKeySkill[2]	= DIK_D;
		MyCtrl->m_vecKeySkill[3]	= DIK_F;
		MyCtrl->m_vecKeySkill[4]	= DIK_G;
    }
    else
    {
        MyCtrl->m_keyUp             = m_KGCOption.m_KeyTable[0];
        MyCtrl->m_keyDown           = m_KGCOption.m_KeyTable[1];
        MyCtrl->m_keyLeft           = m_KGCOption.m_KeyTable[2];
        MyCtrl->m_keyRight          = m_KGCOption.m_KeyTable[3];
        MyCtrl->m_keyAttack         = m_KGCOption.m_KeyTable[4];
        MyCtrl->m_keyProvocation    = m_KGCOption.m_KeyTable[5];
        MyCtrl->m_keyChangeItem     = m_KGCOption.m_KeyTable[6];
        MyCtrl->m_keyUseItem        = m_KGCOption.m_KeyTable[7];
        MyCtrl->m_keyChangeSkill    = m_KGCOption.m_KeyTable[8];
        MyCtrl->m_keyUsePetSkill    = m_KGCOption.m_KeyTable[9];
        MyCtrl->m_keySpecialSkillKey= m_KGCOption.m_KeyTable[10];
#ifdef WEAPONCHANGE
        MyCtrl->m_keyWeaponSwap     = m_KGCOption.m_KeyTable[11];
        MyCtrl->m_vecKeySkill[0]	= m_KGCOption.m_KeyTable[12];
        MyCtrl->m_vecKeySkill[1]	= m_KGCOption.m_KeyTable[13];
        MyCtrl->m_vecKeySkill[2]	= m_KGCOption.m_KeyTable[14];
        MyCtrl->m_vecKeySkill[3]	= m_KGCOption.m_KeyTable[15];
        MyCtrl->m_vecKeySkill[4]	= m_KGCOption.m_KeyTable[16];
#elif USE_ONGAME_INVENTORY
        MyCtrl->m_keyWeaponSwap     = m_KGCOption.m_KeyTable[11];
        MyCtrl->m_keyGameInventory  = m_KGCOption.m_KeyTable[12];
        MyCtrl->m_vecKeySkill[0]	= m_KGCOption.m_KeyTable[13];
        MyCtrl->m_vecKeySkill[1]	= m_KGCOption.m_KeyTable[14];
        MyCtrl->m_vecKeySkill[2]	= m_KGCOption.m_KeyTable[15];
        MyCtrl->m_vecKeySkill[3]	= m_KGCOption.m_KeyTable[16];
        MyCtrl->m_vecKeySkill[4]	= m_KGCOption.m_KeyTable[17];
#else
        MyCtrl->m_vecKeySkill[0]	= m_KGCOption.m_KeyTable[11];
        MyCtrl->m_vecKeySkill[1]	= m_KGCOption.m_KeyTable[12];
        MyCtrl->m_vecKeySkill[2]	= m_KGCOption.m_KeyTable[13];
        MyCtrl->m_vecKeySkill[3]	= m_KGCOption.m_KeyTable[14];
        MyCtrl->m_vecKeySkill[4]	= m_KGCOption.m_KeyTable[15];
#endif

        MyCtrl->m_keyCameraZoom = DIK_B;
#if !defined(__PATH__)
        MyCtrl->m_keyMonsterDirection = DIK_P;
#endif
    }

#if defined(USE_JOYSTICK)
    MyCtrl->SetVirtualGamePadKey();
#endif
}

void GetAllFileName( const std::string& strPath, std::stringstream& stm, const std::string& strfolderName )
{
    HANDLE          handle;
    WIN32_FIND_DATAA finddata;
    CHAR            searchPath[256];
    sprintf( searchPath, "%s\\*.*", strPath.c_str() );
    handle = FindFirstFileA( searchPath, &finddata );

    if (handle != INVALID_HANDLE_VALUE)
    {
        // check all files and directories
        do
        {
            if( strcmp( finddata.cFileName, "." ) == 0 )
                continue;
            if( strcmp( finddata.cFileName, ".." ) == 0 )
                continue;

            if( finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                std::string strFolderPath = strPath;
                strFolderPath += finddata.cFileName;
                GetAllFileName( strFolderPath, stm, finddata.cFileName );
            }
            else
            {
                if( strfolderName == "" )
                {
                    stm << finddata.cFileName << "( " << finddata.nFileSizeLow << " bytes )" << "\n";
                }
                else
                {
                    stm << strfolderName << "\\" << finddata.cFileName << "( " << finddata.nFileSizeLow << " bytes )" << "\n";
                }
            }
        } while( FindNextFileA( handle, &finddata ) );
        FindClose(handle);
    }
}

void CMyD3DApplication::MakeClientFileList( LPVOID lpvState )
{
    FILE* file = fopen( "clientfilelist.txt", "w" );
    std::stringstream stm;
    stm << "------------------------------FileList----------------------\n";
    GetAllFileName( g_strCurrentPath, stm, "" );
    stm << "----------------------------FileList End---------------------\n";
    fwrite( stm.str().c_str(), stm.str().size(), 1, file );
    fclose( file );
#if !defined(BUG_TRAP)
    AddFile( lpvState, "clientfilelist.txt", "File List" );
#endif
}

void CMyD3DApplication::OnOptionButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OPTION );
    }
}

void CMyD3DApplication::OnCoupleButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        if( SiKGCCoupleSystem()->Am_I_Solo() )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM11 ) );
        else
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_VIEWER );
    }
}

void CMyD3DApplication::OnMissionButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG );
    }
}

void CMyD3DApplication::OnSkillTreeButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
#if defined( NATION_KOREA )
		if( g_kGlobalValue.ServerInfo.CheckServerType( ST_LOCK_SKILL_TREE )  && ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH ) )
#else
            if( g_kGlobalValue.ServerInfo.CheckServerType( ST_LOCK_SKILL_TREE ) == true )
#endif
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LIMITED_FUNC_IN_PVP_SERVER ),L"" );
            return;
        }


#if !defined(NATION_USA) && !defined(NATION_IDN)
        if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_CREATE_LIMIT ) )
#endif
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE );

			int iPromotion;
            if( g_kGlobalValue.IsTempUseSpecial4Char( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) && 
                g_kGlobalValue.IsQuickSkillPlayer( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) == false && 
                g_kGlobalValue.IsHybridPlayer( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) == false ) {
				iPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentPromotion();
			} else {
				iPromotion = g_pkUIScene->m_pkSkillTree->GetEndSkillOpenPromotion( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel );
			}            
			g_pkUIScene->m_pkSkillTree->SetSelectedChar( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, iPromotion, true );
		}
    }
}

void CMyD3DApplication::OnOpenSkillTreeByCheat()
{
#if !defined(__PATH__)
    if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_PLAYING )
    {
        if( g_kGlobalValue.IsTutorialMode() ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true ); 
            g_pkUIScene->m_pkSkillTree->SetSelectedChar( g_MyD3D->MyPlayer[0]->m_kUserInfo.GetCurrentChar().iCharType, 
                g_MyD3D->MyPlayer[0]->m_kUserInfo.GetCurrentChar().iPromotionLevel, true );
        } else {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true ); 

            int iPromotion;
            if( g_kGlobalValue.IsTempUseSpecial4Char( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) && 
                g_kGlobalValue.IsQuickSkillPlayer( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) == false && 
                g_kGlobalValue.IsHybridPlayer( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) == false ) {
                    iPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentPromotion();
            } else {
                iPromotion = g_pkUIScene->m_pkSkillTree->GetEndSkillOpenPromotion( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel );
            }            
            g_pkUIScene->m_pkSkillTree->SetSelectedChar( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, iPromotion, true );
        }        

        g_MyD3D->SetCheatOpenSkillUI( true );
    }
#endif
}

void CMyD3DApplication::OnMSNButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT 
        )
    {
        // 만약 메시지 서버에 접속되어있지 않다면 메시지 서버에 접속시도
        JIF( KP2P::GetInstance()->m_kMsgServer != NULL );
        if( KP2P::GetInstance()->m_kMsgServer->ConnectToMsgServerAgain() == false )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_FRIEND_ERROR5 ).c_str(),
                g_pkStrLoader->GetString( STR_ID_FRIEND_ERROR6 ).c_str(),
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        }

        if( !g_kGlobalValue.m_setAddGuildMemberData.empty() )
        {
            KP2P::GetInstance()->m_kMsgServer->Send_AddGuildMemberListReq(g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID,
                g_kGlobalValue.m_kGuildInfo.m_dwUID, g_kGlobalValue.m_setAddGuildMemberData);
        }

        if( !g_kGlobalValue.m_setDelGuildMemberData.empty() )
        {
            KP2P::GetInstance()->m_kMsgServer->Send_DelGuildMemberListReq(g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID,
                g_kGlobalValue.m_kGuildInfo.m_dwUID, g_kGlobalValue.m_setDelGuildMemberData);
        }	

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MSN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        else
            g_pkUIScene->MessageBox( KGCUIScene::GC_MSN_DLG );
    }
}

void CMyD3DApplication::OnCalendarButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        if( g_kGlobalValue.m_bExistCalendar )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ATTENDANCECALENDAR );
            KP2P::GetInstance()->Send_OpenCalendarNot();
        }
        else // 달력정보를 얻어오는데 실패했습니다._서버 이동을 하거나 게임을 다시 실행시켜_주세요.
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LOAD_CALENDAR_ERROR ) );
    }
}

void CMyD3DApplication::OnMatchButton()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH )
        return;

    DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
    if( dwChannelUID != 0xFFFFFFFF && dwChannelUID != SiKGCChannelManager()->GetDungeonChannelUID() )
    {
        // 이미 채널에 접속 중이라면!
        g_MyD3D->m_pStateMachine->GoMatch();
    }
    else
    {
        // 대전 채널 리스트를 얻어온다
        std::vector<KChannelInfo>& vecChannelInfo = SiKGCChannelManager()->GetMatchChannelList();
        DWORD dwFindChannelUID = 0xFFFFFFFF;

        //용섬 서버면 용섬 채널로 들어갑니다.
        if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
        {
            //용섬 채널을 찾습니다.
            for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
            {
                if( (EChannelType)vecChannelInfo[i].m_cType == CT_INDIGO )
                {
                    dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
                    break;
                }
            }

            // 그래도 없으면-ㅅ - 어쩔수없다.. 아무동작 하지 않음..
            if( dwFindChannelUID == 0xFFFFFFFF )
                return;
        }
        //길드전 서버면 채널을 찾는다.
        else if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
        {
            //길드전 서버 채널 리스트 첫 서버 로그인시 채널 등급을 받아온다. 채널등급은 GradeToChannelType에서 채널 타입을 가져온다.
            //길드전 에서 채널에 맞는 상수 K값을 서버에서 비교해서 넘겨오는데 
            int iUserGrade =  static_cast<int>(g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade);
            EChannelType eChannelType = GradeToChannelType( iUserGrade );

            //적당한 채널을 찾는다.
            for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
            {
                if( (EChannelType)vecChannelInfo[i].m_cType == eChannelType )
                {
                    dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
                    break;
                }
            }

            if( dwFindChannelUID == 0xFFFFFFFF )
                return;
        }
        else
        {
            // 매번 대전 누를때마다 채널 선택하니 너무 귀찮다..=ㅅ =
            // 알아서 자기계급 채널에 접속을 하고 원하는 경우 채널변경을 눌러서 바꾸도록 하겠음..
            // 채널에 접속되어 있지 않으면 채널 선택!
            //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CHANNEL_LIST );

            // [3/13/2008] breadceo. 이전에 접속해있던 대전채널
            //dwFindChannelUID = SiKGCChannelManager()->GetPrevMatchChannel();
            // 없으면 맞는 채널을 찾는다
            //if( dwFindChannelUID == 0xFFFFFFFF )
            //{
            //    // 유져의 계급
            //    int iUserGrade = (int)g_kGlobalValue.m_kUserInfo.GetUserGrade();
            //    EChannelType eChannelType = GradeToChannelType( iUserGrade );

            //    // 적당한 채널을 찾자..
            //    for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
            //    {
            //        if( (EChannelType)vecChannelInfo[i].m_cType == eChannelType )
            //        {
            //            dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
            //            break;
            //        }
            //    }
            //}

            // 맞는 채널이 없다면? 자유채널을 찾고..
            if( dwFindChannelUID == 0xFFFFFFFF )
            {
                for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
                {
                    if( (EChannelType)vecChannelInfo[i].m_cType == CT_AUTOMATCH )
                    {
                        dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
                        break;
                    }
                }

                // 그래도 없으면-ㅅ - 어쩔수없다.. 아무동작 하지 않음..
				if( dwFindChannelUID == 0xFFFFFFFF ) {
					for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
					{
						if( (EChannelType)vecChannelInfo[i].m_cType == CT_UNLIMITED )
						{
							dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
							break;
						}
					}
					if( dwFindChannelUID == 0xFFFFFFFF ) 
	                    return;
				}
            }
        }

        // 채널에 접속한다!
        if( SiKGCChannelManager()->GetCurrentChannel() == dwFindChannelUID )
            return;

        // 올바른 채널이 아니면 처리X
        if( SiKGCChannelManager()->GetChannel( dwFindChannelUID ) == NULL )
            return;

        if( g_MyD3D->m_pStateMachine->GoMatch( dwFindChannelUID ) == true )
        {
            std::wstring strChannelName = SiKGCChannelManager()->GetCurrentChannelName();
            ::StringCchPrintfW( g_Selected_Channel_Name, (int)strChannelName.size() + 1, L"%s", strChannelName.c_str() );

            if ( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
            {
                g_pkUIScene->RoomButtonSetAutoStart( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) );
            }

        }
    }
}

void CMyD3DApplication::SetMatchChannel( EChannelType eType )
{
    //if( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH )
    //    return;

    DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
    DWORD dwFindChannelUID = 0xFFFFFFFF;

    // 대전 채널 리스트를 얻어온다
    std::vector<KChannelInfo>& vecChannelInfo = SiKGCChannelManager()->GetMatchChannelList();

    for( int i = 0; i < (int)vecChannelInfo.size(); ++i )
    {
        if( (EChannelType)vecChannelInfo[i].m_cType == eType )
        {
            dwFindChannelUID = vecChannelInfo[i].m_dwChannelUID;
            break;
        }
    }

    // 채널에 접속한다!
    if( SiKGCChannelManager()->GetCurrentChannel() == dwFindChannelUID )
        return;

    // 올바른 채널이 아니면 처리X
    if( SiKGCChannelManager()->GetChannel( dwFindChannelUID ) == NULL )
        return;

    if( g_MyD3D->m_pStateMachine->GoMatch( dwFindChannelUID ) == true )
    {
        std::wstring strChannelName = SiKGCChannelManager()->GetCurrentChannelName();
        ::StringCchPrintfW( g_Selected_Channel_Name, (int)strChannelName.size() + 1, L"%s", strChannelName.c_str() );

        if ( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
        {
            g_pkUIScene->RoomButtonSetAutoStart( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) );
        }
    }

}

void CMyD3DApplication::OnSquareButton()
{
    if(g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CANT_ENTER_SQUARE_IN_INDIGO), L"",
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true  );
    }else
    {
        g_pkUIScene->m_pkSquareEnterWndS6->SetSquareKind(KSquareInfo::ST_NORMAL);
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SQUARE_ENTER_S6 );
    }
}

void CMyD3DApplication::OnItemCreateAndBreakUpButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        if ( SiKGCPartyManager()->IsParty() )
            SiKGCPartyManager()->LeaveParty();

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FORGE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, 0, true, false );
    }
}

void CMyD3DApplication::OnHeroShopButton()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_HERO_DUNGEON_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );                
    }    
}

void CMyD3DApplication::OnReplayRecordBtn()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY && g_kGlobalValue.IsCapturing() == false )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_RECORD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );        
    }
}

void CMyD3DApplication::AutoConnectToGameServer()
{
    // ENU_VERIFY_ACCOUNT_ACK를 받고나서..
    // 자동으로 게임서버로 접속하는 기능을 만드려고 했는데..
    // 현재 그리 이상한 모양이 아니라 우선 기능 구현을 보류한다


    //// 게임서버에 접속되어 있지 않다면?
    //if( SiKP2P()->IsConnectedToGameServer() == false )
    //{
    //    // 서버 리스트 중 적당한 곳에 자동으로 접속을 한다!!
    //    int iNumServer = g_kGlobalValue.ServerList.size();

    //    for( int i = 0; i < iNumServer; ++i )
    //    {
    //        KServerInfo kServerInfo = g_kGlobalValue.ServerList[i];
    //        kServerInfo->m_iMaxUserNum
    //    }



    //    KEVENT_VERIFY_ACCOUNT_REQ kReq;
    //    kReq.m_strLogin     = g_kGlobalValue.m_kLoginInfo.strID;
    //    kReq.m_strPasswd    = g_kGlobalValue.m_kLoginInfo.strPass;
    //    kReq.m_bMale        = g_kGlobalValue.m_kLoginInfo.bMale;
    //    kReq.m_iAge         = g_kGlobalValue.m_kUserInfo.iAge;
    //    kReq.m_iVersion     = SERVER_VERSION;
    //    kReq.m_iP2PVersion  = CLIENT_P2P_VER;
    //    kReq.m_dwAuthType   = g_kGlobalValue.m_dwAuthType;
    //    kReq.m_dwAuthTick   = g_kGlobalValue.m_dwAuthTick;
    //    g_MyD3D->GetClientHarwareInfo( kReq.m_kStatClientInfo );

    //    #if defined( _DEBUG ) || !defined( __PATH__ ) || defined( NETMARBLE_BUY_TEST )
    //    {
    //        kReq.m_dwChecksum = 0;
    //    }
    //    #else
    //    {
    //        kReq.m_dwChecksum = GetExeCheckSum( "Main.exe" );
    //    }
    //    #endif

    //    Result_Connect_GameServer = INT_MAX;

    //    // 게임 서버 접속
    //    KP2P::GetInstance()->ConnectToGameServer( m_kServerInfo, kReq, 10000 );
    //    g_MyD3D->WaitForServerAck( Result_Connect_GameServer, INT_MAX, 15000, TIME_OUT_VALUE );

    //    // 0 ( 인증 성공 )
    //    // 1 ( 잘못된 패스워드 )
    //    // 2 ( 이중 접속 )
    //    // 3 ( 길드정보 잘못됨 )
    //    // 4 ( 프로토콜 버전 다름 )
    //    // 5 ( 불량 유저 )
    //    // 7 ( login 크기가 0이거나 인증 실패 )
    //    // 8 ( 신규 유저 추가 실패 )
    //    // 9 ( esports 예선 신청자가 아님. )
    //    // 10 ( 실행파일 checksum이 무효함 )
    //    // 11 ( 알 수 없는 오류. 대만 인증 실패)
    //    // 12 ( 존재하지 않는 login )
    //    // 13 ( 인증 tick Count가 다르다. )
    //    switch( Result_Connect_GameServer )
    //    {
    //    case 0: // 접속 성공
    //        {
    //            g_kGlobalValue.SendLoadingTime();
    //            g_kGlobalValue.SendLoginServerConTime();

    //            return true;
    //        }
    //    case 1: // 존재하지 않는 아이디 또는 패스워드가 틀림
    //    case 7: // login 크기가 0이거나 인증 실패
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2 ) );
    //            break;
    //        }
    //    case 2: // 이중접속 시도
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT2 ) );
    //            break;
    //        }
    //    case 3: // 잘못된 길드
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GUILD_INFO1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GUILD_INFO2 ) );
    //            break;
    //        }
    //    case 4: // 패치가 되지 않았음 ( 클라이언트 체크섬이 디비에 등록되어 있지 않다 )
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION2 ) );
    //            break;
    //        }
    //    case 5: // 계정 중지
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ACCOUNT_BLOCK1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ACCOUNT_BLOCK2 ) );
    //            break;
    //        }
    //    case 9: // 대회서버인데 대회참가자가 아님
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST2 ) );
    //            break;
    //        }
    //    case 10: // 실행파일 체크섬이 유효하지 않음.
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE2 ) );
    //            break;
    //        }
    //    case 11: // Gash 오류
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GASH ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_NOMATCH_SERVER ) );
    //            break;
    //        }
    //    case 13: // 12세 미만의 사용자 제한.
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_YOUNGER_BOY_CONNECT1 ),
    //                g_pkStrLoader->GetString( STR_ID_YOUNGER_BOY_CONNECT2 ) );
    //            break;
    //        }
    //    case 8:
    //    case TIME_OUT_VALUE:
    //    default:        // 타임아웃 또는 알 수 없는 에러
    //        {
    //            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1 ),
    //                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2 ) );
    //            break;
    //        }
    //    }

    //    return false;
    //}
}

void CMyD3DApplication::GoChannel()
{
    // 채널이 없다?
    if( true )//SiKGCChannelManager()->GetMatchChannelList().empty() )
    {
        //  서버에 접속이 안되어 있다.
        if ( !KP2P::GetInstance()->IsConnectedToGameServer() )
        {
            g_MyD3D->m_pStateMachine->GoServer();
            return;
        }

        // Result_UserList 이건 그냥 변수만들기 귀찮아서 썼음..-ㅅ -
        Result_UserList = INT_MAX;
        // 게임서버에 접속 후에 채널 리스트를 한번 요청한다
        KP2P::GetInstance()->Send_ChannelInfoListReq( false );
        g_MyD3D->WaitForServerAck( Result_UserList, INT_MAX, 10000, TIME_OUT_VALUE );

        if ( Result_UserList == TIME_OUT_VALUE )
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_CHANNEL_LIST_ACK, Result_UserList );

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, 
                g_pkStrLoader->GetString(STR_ID_ENTER_CHANNEL_ERROR6),
                g_pkStrLoader->GetString(STR_ID_ENTER_CHANNEL_ERROR7) + strTemp );
            return;
        }
    }

    // 대전을 강조하기 위해서 대전화면으로 바로 간다..
    // 월드맵으로 바로갈 수도있음..


    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
    {
        g_MyD3D->OnMatchButton();
        return;
    }
#if defined( NATION_CHINA )
	//중국은 그냥 월드맵 기본이다
    if ( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_DIO  ||
        g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_EDEL  ||
        g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() != STATE_FIRST_LOGIN ) {
            g_MyD3D->m_pStateMachine->GoWorldMap();
    }
    else {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_TUTORIAL_ENTER_CONFIRM_MSG ), L"",
            KGCUIScene::GC_MBOX_USE_TUTORIAL_GUIDE_ENTER, 0, 0, false, true);
    }

	return;
#endif

    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
    {
        g_MyD3D->OnMatchButton();
        return;
    }
	else if ( !g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_FREE) )
    {
        g_MyD3D->m_pStateMachine->GoWorldMap();
        return;
	}
	else 
    {
        if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_CLEAR_PRIVATE_DNGEON2 ));
            return;
        }
        else {
            g_MyD3D->OnMatchButton();
        }
        return;
	}

	return;
}

#define GRADE_BRONZE_VAGABOND   1   // 동 떠돌이기사단
#define GRADE_SILVER_VAGABOND   2   // 은 떠돌이기사단
#define GRADE_GOLD_VAGABOND     3   // 금 떠돌이기사단
#define GRADE_BRONZE_MERCENARY  4   // 동 용병기사단
#define GRADE_SILVER_MERCENARY  5   // 은 용병기사단
#define GRADE_GOLD_MERCENARY    6   // 금 용병기사단
#define GRADE_BRONZE_ROYAL      7   // 동 왕립기사단
#define GRADE_SILVER_ROYAL      8   // 은 왕립기사단
#define GRADE_GOLD_ROYAL        9   // 금 왕립기사단
#define GRADE_BRONZE_GOD        10  // 동 신의기사단
#define GRADE_SILVER_GOD        11  // 은 신의기사단
#define GRADE_GOLD_GOD          12  // 금 신의기사단


EChannelType CMyD3DApplication::GradeToChannelType( int iGrade )
{
    EChannelType eChannelType = CT_UNLIMITED;
    bool bGuildServer = false;
    bGuildServer = g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD );

#if defined( NATION_BRAZIL)
    //GRADE_BRONZE_VAGABOND    동 떠돌이기사단
    //GRADE_SILVER_VAGABOND    은 떠돌이기사단
    if( iGrade <= GRADE_GOLD_VAGABOND )
        eChannelType = CT_BEGINNER;
    //GRADE_GOLD_VAGABOND      금 떠돌이기사단
    //GRADE_BRONZE_MERCENARY   동 용병기사단
    //GRADE_SILVER_MERCENARY   은 용병기사단
    else if( iGrade <= GRADE_BRONZE_ROYAL )
        eChannelType = CT_ROOKIE;
    //GRADE_GOLD_MERCENARY     금 용병기사단
    //GRADE_BRONZE_ROYAL       동 왕립기사단	
    else if( iGrade <= GRADE_BRONZE_GOD && !bGuildServer)
        eChannelType = CT_EXPERT;
    //GRADE_SILVER_ROYAL       은 왕립기사단
    //GRADE_GOLD_ROYAL         금 왕립기사단
    //GRADE_BRONZE_GOD         동 신의기사단
    //GRADE_SILVER_GOD         은 신의기사단
    //GRADE_GOLD_GOD           금 신의기사단
    //길드전 자유 채널
    else if( iGrade == CT_GUILD_UNLIMITED )
        eChannelType = CT_GUILD_UNLIMITED;
    //길드전 신의 채널
    else if ( iGrade == CT_GUILD_MASTER)
        eChannelType = CT_GUILD_MASTER;
    //길드전 왕립 채널
    else if ( iGrade == CT_GUILD_EXPERT )
        eChannelType = CT_GUILD_EXPERT;
    //길드전 용병 채널
    else if ( iGrade == CT_GUILD_ROOKIE )
        eChannelType = CT_GUILD_ROOKIE;
    //길드전 떠돌이 채널
    else if ( iGrade == CT_GUILD_BEGINNER )
        eChannelType = CT_GUILD_BEGINNER;
    else
        eChannelType = CT_MASTER;

#else
    //GRADE_BRONZE_VAGABOND    동 떠돌이기사단
    //GRADE_SILVER_VAGABOND    은 떠돌이기사단
    if( iGrade <= GRADE_SILVER_VAGABOND )
        eChannelType = CT_BEGINNER;
    //GRADE_GOLD_VAGABOND      금 떠돌이기사단
    //GRADE_BRONZE_MERCENARY   동 용병기사단
    //GRADE_SILVER_MERCENARY   은 용병기사단
    else if( iGrade <= GRADE_SILVER_MERCENARY )
        eChannelType = CT_ROOKIE;
    //GRADE_GOLD_MERCENARY     금 용병기사단
    //GRADE_BRONZE_ROYAL       동 왕립기사단
    else if( iGrade <= GRADE_BRONZE_ROYAL )
        eChannelType = CT_EXPERT;
    //GRADE_SILVER_ROYAL       은 왕립기사단
    //GRADE_GOLD_ROYAL         금 왕립기사단
    //GRADE_BRONZE_GOD         동 신의기사단
    //GRADE_SILVER_GOD         은 신의기사단
    //GRADE_GOLD_GOD           금 신의기사단
    else if ( bGuildServer ) 
    {
        //길드전 자유 채널
        if( iGrade == CT_GUILD_UNLIMITED )
            eChannelType = CT_GUILD_UNLIMITED;
        //길드전 신의 채널
        else if ( iGrade == CT_GUILD_MASTER)
            eChannelType = CT_GUILD_MASTER;
        //길드전 왕립 채널
        else if ( iGrade == CT_GUILD_EXPERT )
            eChannelType = CT_GUILD_EXPERT;
        //길드전 용병 채널
        else if ( iGrade == CT_GUILD_ROOKIE )
            eChannelType = CT_GUILD_ROOKIE;
        //길드전 떠돌이 채널
        else if ( iGrade == CT_GUILD_BEGINNER )
            eChannelType = CT_GUILD_BEGINNER;
    }
    else
        eChannelType = CT_MASTER;
#endif

    return eChannelType;
}

bool CMyD3DApplication::IsProtectRing( int iItemID )
{
    if (iItemID == ITEMID_PROTECT_RING ||
        iItemID == ITEMID_PROTECT_RING_PC ||
        iItemID == ITEMID_PROTECT_RING_10 ||
        iItemID == ITEMID_PROTECT_RING_QUESTPACKAGE ||
        iItemID == ITEMID_PROTECT_RING_DISCOUNT ||
        iItemID == ITEMID_PROTECT_RING_10_DISCOUNT ||
        iItemID == ITEMID_PROTECT_RING_SINGLE || //��ȣ�� ���� 1��¥��
        iItemID == ITEMID_EVENT_PEPERO || // �̺�Ʈ ������ �̺�Ʈ ������ ����ÿ� 
        iItemID == ITEMID_LISNAR_PRAY ||
        iItemID == ITEMID_HEITAROS_PRAY ||
        iItemID == ITEMID_VITTAR_POTION_OF_RESURRECTION ||
        iItemID == ITEM_HENIR_RESS_POT)
        return true;
    else
        return false;
}

void CMyD3DApplication::ChargeLocalPlayerMana( float fAmount_, float fRate )
{
    if ( fAmount_ == 0.0f )
        return;

#if !defined(NATION_IDN) && !defined(NATION_PHILIPPINE)
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && 
        ( g_MyD3D->GetMyPlayer()->IsSpecialSkillMotion() || g_MyD3D->GetMyPlayer()->m_bNoRecoveryMpMotion ) ) {
        return;
    }
#endif

    MyHead->Level += fAmount_ * ( m_fIncMPRatio + fRate );

    if( g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP() )
    {
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    }

    if ( g_MyD3D->MyHead->Level < 0.0f )
    {
        g_MyD3D->MyHead->Level = 0.0f;
    }
}

EGCGameModeKind CMyD3DApplication::GetGameCategoryKind( EGCGameModeCategory eModeCategory )
{
    switch(eModeCategory){
        case GC_GMC_MATCH:
        case GC_GMC_TAG_MATCH:
        case GC_GMC_GUILD_BATTLE:
        case GC_GMC_INDIGO:
        case GC_GMC_DEATHMATCH:
        case GC_GMC_FATAL_DEATHMATCH:
            {
                return GC_GMK_PLAYER;
            }
        case GC_GMC_DUNGEON:
        case GC_GMC_MONSTER_CRUSADER:
        case GC_GMC_MONSTER_HUNT:
        case GC_GMC_DOTA:
            {
                return GC_GMK_MONSTER;
            }
        case GC_GMC_TUTORIAL:
            {
                return GC_GMK_ETC;
            }
    }
    return GC_GMK_INVALID;
}

bool CMyD3DApplication::IsCharacterIndex( const int iCharIdx_ ) const
{
    if( ( GC_CHAR_NUM <= iCharIdx_ ) || ( 0 > iCharIdx_ ) )
        return false;

    return true;
}

bool CMyD3DApplication::IsPlayerIndex( const int iPlayer ) const
{
    if( (MAX_PLAYER_NUM <= iPlayer) || (0 > iPlayer) )
        return false;

    return true;
}

bool CMyD3DApplication::IsDungeonPlayerIndex( const int iPlayer ) const
{ 
    if( (MAX_DUNGEON_PLAYER_NUM <= iPlayer) || (0 > iPlayer) ) 
        return false; 

    return true; 
}


std::pair<int,KGC2DObject*> CMyD3DApplication::Add2DObject( KGC2DObject* obj )
{
    m_vec2DObject.push_back( obj );

    return std::make_pair( m_vec2DObject.size() - 1, obj );
}

void CMyD3DApplication::Erase2DObject( int iIndex )
{
    if( iIndex < 0 || iIndex >= (int)m_vec2DObject.size() )
    {
        ASSERT(!"Invalid Index in 2DObject list!!" );
        return;
    }

    SAFE_DELETE( m_vec2DObject[iIndex] );
    m_vec2DObject.erase( m_vec2DObject.begin() + iIndex );
}

void CMyD3DApplication::Clear2DObject()
{
    m_vec2DObject.clear();
}

bool CMyD3DApplication::GetScreenShotFileName( std::wstring& strFileName_ )
{
    if( m_wstrScreenShotFileName.empty() ) {
        return false;
    }

    strFileName_ = m_wstrScreenShotFileName;

    std::wstring::size_type pos = 0; 
    std::wstring::size_type offset = 0; 
    std::wstring wstrReplace = L"";
    std::wstring wstrPattern = L"ScreenShot\\";

    while((pos = strFileName_.find( wstrPattern, offset ) ) != std::wstring::npos) { 
        strFileName_.replace( strFileName_.begin() + pos, strFileName_.begin() + pos + wstrPattern.size(), wstrReplace ); 
        offset = pos + wstrReplace.size(); 
    }  

    return true;
}