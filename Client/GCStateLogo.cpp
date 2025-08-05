#include "stdafx.h"
//
#include "../MyD3D.h"

#include "../KMci.h"

//
#include "GCStateLogo.h"


#include "GCStateMachine.h"

//
#include "GCUI/KGCLoginScene.h"

#include "GCUI/KGCRegisterScene.h"

KGCStateLogo::KGCStateLogo(void)
{
    m_pKOGLogo              = NULL;
    m_pPublisherLogo        = NULL;
    m_pPanhoLogo			= NULL;

    m_dwLastFrameTime       = 0;
    m_eLogoState            = STATE_INIT;
    m_fPitch                = 0.0f;
    m_bPublisherLogoState   = false;
    m_bPanhoLogoState		= false;

    D3DXMatrixIdentity( &m_matWorld );

    InitDevice();
}

KGCStateLogo::~KGCStateLogo(void)
{
}

HRESULT KGCStateLogo::InitDevice()
{
    D3DXMATRIX matTrans;
    ::D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 0.8f );
    m_matWorld = matTrans;

    return S_OK;
}

bool KGCStateLogo::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{

#ifdef _START_WITHOUT_SERVER_
    return true;
#endif
    g_MyD3D->m_pStateMachine->Clear_SavedState();

#if !defined( BEAN_FUN )
    HRESULT hr = StartUpServer();
    if ( hr != S_OK )
    {
        if( hr == E_FAIL )
        {
            g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL1 ).c_str(),
                                    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL2 ).c_str() );
        }
        else if( hr == E_ABORT )
        {
            g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_WEB_BROWSER_INIT1 ).c_str(),
                                    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_WEB_BROWSER_INIT2 ).c_str() );
        }
		else
		{
			g_MyD3D->ExitWithMsgDlg( L"", L"" );
		}
        return false;
    }
#endif

    return true;
}

bool KGCStateLogo::OnInit()
{
	//g_kGlobalValue.ReportforGCLOG(SiKGCMultipleLanguages()->GetLanguageType());
    //맨 처음 로딩이 끝나고 자연스럽게 logo 화면으로 옮겨 간다.
    g_pkUIScene->m_pkLoginScene->ToggleRender(false);
    g_pkUIScene->m_pkRegisterScene->ToggleRender(false);

    if( SUCCEEDED( KGCStateLogo::LoadTexture() ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

HRESULT KGCStateLogo::LoadTexture()
{
    m_pKOGLogo = g_pGCDeviceManager2->CreateTexture( "LOGO.DDS" );
	//m_pKOGLogo = g_pGCDeviceManager2->CreateTexture( "logo_christmas.dds" );

    #if defined( NATION_JAPAN )
        #define PUBLISHER_LOGO ("nexon_logo.dds")
    #elif defined( NATION_CHINA )
        #define PUBLISHER_LOGO ("cyou.dds")
	#elif defined( NATION_USA ) || defined( NATION_THAILAND ) || defined( NATION_IDN ) || defined( NATION_EU )
		#define PUBLISHER_LOGO ("logo2.dds" )
    #elif defined( NATION_LATINAMERICA )
        #define PUBLISHER_LOGO ("lin_logo.dds")
    #elif defined (NATION_BRAZIL)
        #define PUBLISHER_LOGO ("title_logo.dds")
    #endif

    #if defined( PUBLISHER_LOGO )
    {
        m_pPublisherLogo = g_pGCDeviceManager2->CreateTexture(PUBLISHER_LOGO);
        m_bPublisherLogoState = true;
    }
    #endif

#ifdef NATION_CHINA
    m_pPanhoLogo = g_pGCDeviceManager2->CreateTexture("logo2.dds");
    m_bPanhoLogoState = true;
#endif	

    return S_OK;
}

bool KGCStateLogo::FrameMove( float fElapsedTime )
{
    static bool bLogoMusicPlay = true;
    static float fWait = 0.0f;
    g_pkInput->UpdateInput();

    //엔터키 또는 마우스 왼쪽버튼을 누르면 로고화면 스킵
    if( ( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->BtnDown(KInput::MBLEFT) )
        && m_eLogoState != STATE_ELSE )
    {
        if( m_bPublisherLogoState || m_bPanhoLogoState )
            m_eLogoState = STATE_LOGO;
        else 
            m_eLogoState = STATE_OUT;

        m_fPitch = 0.0f;
    }

    if( m_dwLastFrameTime == 0 )
        m_dwLastFrameTime = timeGetTime();

    if( m_eLogoState == 0 && !m_bPublisherLogoState && !m_bPanhoLogoState )  //퍼블리셔를 사용하지 않는다면 KOG로고만.
        m_eLogoState = STATE_LOGO;

	//DWORD dwSoundNum = 0;

    switch( m_eLogoState )
    {
        case STATE_INIT:
        {
            m_fPitch += (timeGetTime() - m_dwLastFrameTime) / 1500.0f;; //0.7초에 걸려 한 단계 진행되도록 설정.

            if( m_fPitch > 0.99f )
            {
                m_fPitch = 1.0f;
                m_eLogoState = STATE_WAIT;
            }
            else if( bLogoMusicPlay && m_fPitch > 0.05 )
            {
#if defined( NATION_USA )
				g_KMci.Play( GC_BGM_USA_LOGO, false );
#else
                g_KMci.Play( GC_BGM_LOGO, false );
                //g_KDSound.Play( "XMas_Logo_Lupus" );
#endif
                bLogoMusicPlay = false;
            }
            break;
        }
        case STATE_WAIT:
        {
            fWait += (timeGetTime() - m_dwLastFrameTime) / 1500.0f;;

            if( fWait > 1.0f )
                m_eLogoState = STATE_READY;

            break;
        }
        case STATE_READY:
        {
            m_fPitch -= (timeGetTime() - m_dwLastFrameTime) / 1500.0f;;
            if ( m_fPitch < 0.01f )
            {
                m_fPitch = 0.0f;
                fWait = 0.0f;
                bLogoMusicPlay = true;
#ifdef NATION_CHINA
                if ( m_bPanhoLogoState ) {
                    m_bPanhoLogoState = false;
                    m_eLogoState = STATE_INIT;
                }
                else
                    m_eLogoState = STATE_LOGO;
#else
                m_eLogoState = STATE_LOGO;
#endif
            }
            break;
        }
        case STATE_LOGO:        //시작 상태
        {
            m_bPublisherLogoState = false;
            m_bPanhoLogoState = false;
            m_fPitch += (timeGetTime() - m_dwLastFrameTime) / 1500.0f;; //0.7초에 걸려 한 단계 진행되도록 설정.

            if( m_fPitch > 0.99f )
            {
                m_fPitch = 1.0f;
                m_eLogoState = STATE_PROGRESS;
            }
            else if( bLogoMusicPlay && m_fPitch > 0.05f )
            {
#if !defined(NATION_THAILAND) && !defined( NATION_IDN ) //태국에서는 2차로고때는 소리 출력하지 않는다. 인도네시아도.
                g_KMci.Play( GC_BGM_LOGO, false );
                //g_KDSound.Play( "XMas_Logo_Lupus" );
                //g_KDSound.Play( "XMas_Logo_Rin" );
                bLogoMusicPlay = false;
#endif
            }
            break;
        }
        case STATE_PROGRESS:        //진행 상태
        {
            fWait += (timeGetTime() - m_dwLastFrameTime) / 1500.0f;;

            if( fWait > 1.0f )
			{
				//dwSoundNum  = static_cast< DWORD>(1226 +  (rand() % 3) );
// 				dwSoundNum  = 1226;
// 				g_KDSound.Play( dwSoundNum, 0 );
                m_eLogoState = STATE_OUT;
			}
            break;
        }
        case STATE_OUT:        //나가기 상태
        {
            m_fPitch -= (timeGetTime() - m_dwLastFrameTime) / 1500.0f;
            if ( m_fPitch < 0.01f )
            {
				g_MyD3D->m_pStateMachine->GoIntro();
                DeleteDevice();
                return true;
            }
            break;
        }
    }
    m_dwLastFrameTime = timeGetTime();

    return true;
}

bool KGCStateLogo::Render()
{
    if ( BeginScene() )
    {
        g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );

        BYTE color = static_cast<BYTE>( 255.0f * m_fPitch );

//#ifndef SHADER_REPLACEMENT
		g_pGCDeviceManager2->PushState();
		{

			g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,          FALSE );
			g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
			g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
//#endif

            if ( m_bPanhoLogoState && m_pPanhoLogo != NULL ) {
                g_pGCDeviceManager2->DrawInWorld(m_pPanhoLogo,&m_matWorld, 
                    4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f,0,0,0,D3DCOLOR_ARGB(color,255,255,255));
            }
            else if( m_bPublisherLogoState && m_pPublisherLogo != NULL )
            {
#if !defined( NATION_THAILAND )
				g_pGCDeviceManager2->DrawInWorld(m_pPublisherLogo,&m_matWorld, 
                    4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f,0,0,0,D3DCOLOR_ARGB(color,255,255,255));
#else
                D3DXMATRIX mat1, mat2, mat3;
                mat1 = m_matWorld;
                D3DXMatrixScaling( &mat2, 1.8f, 1.8f, 1.0f );
                mat1 *= mat2;
                D3DXMatrixTranslation( &mat2, 0.0f, -0.1f, 0.0f );
                mat1 *= mat2;
				g_pGCDeviceManager2->DrawInWorld(m_pPublisherLogo,&mat1, 
                    4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f,0,0,0,D3DCOLOR_ARGB(color,255,255,255));
#endif
				//m_pPublisherLogo->SetDeviceTexture();
				//g_MyD3D->Draw_Billboard( 5.0f, 4.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f, color );
			}
			else
            {
				g_pGCDeviceManager2->DrawInWorld(m_pKOGLogo,&m_matWorld, 
					4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f,0,0,0,D3DCOLOR_ARGB(color,255,255,255));
				//m_pKOGLogo->SetDeviceTexture();
				//g_MyD3D->Draw_Billboard( 4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f, color );
			}		
//#ifndef SHADER_REPLACEMENT
		}
		g_pGCDeviceManager2->PopState();
//#endif

        /*g_MyD3D->m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

        g_pGCDeviceManager2->SetRenderStateSet();
        g_pGCDeviceManager2->SetTextureStageStateSet();
        g_pGCDeviceManager2->SetSamplerStateSet();
        {
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,          FALSE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

            g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

            if( m_bPublisherLogoState && m_pPublisherLogo != NULL )
            {
                m_pPublisherLogo->SetDeviceTexture();
                g_MyD3D->Draw_Billboard( 5.0f, 4.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f, color );
            }
            else
            {
                m_pKOGLogo->SetDeviceTexture();
                g_MyD3D->Draw_Billboard( 4.0f, 3.0f, -2.18095f, -1.58023f, 3.18096f, 2.68023f, color );
            }
        }
        KD3DDeviceState::PopSamplerStateSet();
        KD3DDeviceState::PopTextureStageStateSet();
        KD3DDeviceState::PopRenderStateSet();
        g_MyD3D->m_pd3dDevice->SetTexture( 0, NULL );*/
/*		g_pGCDeviceManager2->Flush();*/
        EndScene();
    }

    return true;
}

void KGCStateLogo::DeleteDevice()
{
    SAFE_RELEASE( m_pKOGLogo );
    SAFE_RELEASE( m_pPublisherLogo );
    SAFE_RELEASE( m_pPanhoLogo );
}

bool KGCStateLogo::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateLogo::OnSetupFSM()
{
	//#if defined( LOGIN_FIRST )
	//if( g_kGlobalValue.m_bLoginFirst )
	//{
	ON_SETUP_FSM( GS_LOGIN,         FAIL_N_SHUTDOWN );
    ON_SETUP_FSM( GS_REGISTER,      FAIL_N_SHUTDOWN );

	//}
	//else
	//#else
	//{
	ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_INTRO,        FAIL_N_SHUTDOWN );
	//}
	//#endif

    m_eStateForFail = FAIL_N_SHUTDOWN;
}

void KGCStateLogo::OnDestroy()
{

}
