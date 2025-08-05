#include "stdafx.h"
#include "GCStateInitLoading.h"
#include "DamageManager.h"
#include "Spark.h"
#include "Controls.h"
#include "Headup Display.h"
#include "KTDGFrustum.h"
#include "Item.h"
#include "Monster.h"
#include "Stage.h"
#include "PetAI.h"
#include "GCCameraOrtho.h"
#include "GCUI/GCOptionSound.h"
#include "GCUtil/GCElapsedTime.h"
#include "../mission/GCMissionManager.h"
#include "../mission/GCCTMissionManager.h"
#include "GCDropItemManager.h"
#include "Square/GCSquare.h"
#include "KNeonSign.h"

#include "KGCEmoticon.h"

#include "KGCMinigameManager.h"
#include "KGCMath.h"
#include "Buff/KGCBuffManager.h"
#include "GCSKT.h"
#include "GCRenderManager.h"

#include "KGCPlayerCommon.h"
#include "SlangFilter.h"
#include "KGCObjectEgo.h"
#include "KGCAgitGameManager.h"
#include "KGCSocialMotionManager.h"
#include "KGCMultipleLanguages.h"
#include "KGCDungeonInfoManager.h"
#include "KGCFaceLift.h"
#include "GCGrowingSkillManager.h"

KGCStateInitLoading* g_pStateInitLoading = NULL;

//extern std::vector<ManufactureRecipe>    g_vecManufactureRecipes;

#define UPDATE_RATE 200

KGCStateInitLoading::KGCStateInitLoading(void)
{
	g_pStateInitLoading = this;
	m_iLoadedResource	 = 0;

	for (int i = 0; i < 3; ++i)
	{
		m_pLoadingTexture.push_back(NULL);
	}

	m_pLoadingBarTexture = NULL;
	m_pLoadingExtraTexture= NULL;
	m_pLoadingBGTexture = nullptr;

	m_iJobStep			 = 0;

	InitDevice();
}

KGCStateInitLoading::~KGCStateInitLoading(void)
{
	g_pStateInitLoading = NULL;
}

bool KGCStateInitLoading::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	return true;
}

HRESULT KGCStateInitLoading::InitDevice()
{
	return LoadTexture_FromMassFile();
}

HRESULT KGCStateInitLoading::LoadTexture_FromMassFile()
{
	g_pGCDeviceManager2->GetMassFileManager()->AddMassFile("./Texture/loading" EXT); // Kawan>
	std::vector<std::wstring> vecStr = g_kGlobalValue.GetLoadingFileName();

	if( vecStr.size() == 4 )
	{
		m_pTexLoadingTexture_from_massfile.push_back(g_pGCDeviceManager2->CreateTexture(KncUtil::toNarrowString(vecStr[0])));
		m_pTexLoadingTexture_from_massfile.push_back(g_pGCDeviceManager2->CreateTexture(KncUtil::toNarrowString(vecStr[1])));
		m_pTexLoadingTexture_from_massfile.push_back(g_pGCDeviceManager2->CreateTexture(KncUtil::toNarrowString(vecStr[2])));

		//m_pLoadingBGTexture = g_pGCDeviceManager2->CreateTexture("bg_loading.dds");
		m_pTexLoadingBarTexture_from_massfile = g_pGCDeviceManager2->CreateTexture(KncUtil::toNarrowString(vecStr[3]));
		return S_OK;
	}

	return E_FAIL;
}

bool KGCStateInitLoading::LoadResource()
{	
	if (m_iJobStep==0)
	{
		GCElapsedTime kElapsedTime( L"InitLoading" );
		ShowloadingBar(13);
	}

	//��� ���ҽ��� �ε��Ѵ�. 
	//�ϳ��ϳ� �ε��� �� ���� ShowloadingBar�Լ��� �����Ȳ �����ش�.

	//�������� �̸� �����Ѵ�.
	std::string strAnimScript;
	std::string strMusicScript;
	std::string strMusicPath;
	std::string strSoundScript;
	std::string strSoundPath;
	std::string strParticleScript;
	std::string strItemScript;

	// ���� �۾����� ó�� �ϰ� ���� Step�� ó�� �Ѵ�.
	switch(m_iJobStep)
	{
	case 1:
		//			g_kGlobalValue.ReportforGCLOG("InitClass");
		g_MyD3D->InitClasses();
		MAKE_LOAD_PROFILE( "InitClass" );
		ShowloadingBar(1);
		break;
	case 2:
		ShowWindow(g_MyD3D->m_hWnd, SW_SHOW);
		Render(); //Render First.
		MAKE_LOAD_PROFILE("LoadResource - Before");			
		g_MyD3D->InitUIFiles("ui_sign.stg");
		MAKE_LOAD_PROFILE( "InitUI" );
		ShowloadingBar(14);
		break;
	case 3:
		g_kGlobalValue.SetVIPItemList();
		g_MyD3D->m_kItemMgr.LoadItemData();
		MAKE_LOAD_PROFILE( "LoadItemData" );

		MAKE_LOAD_PROFILE( "AddMassFile" );
		ShowloadingBar(3);
		break;
	case 4:	
		//			g_kGlobalValue.ReportforGCLOG("fileDownloadManager");

		g_MyD3D->m_fileDownloadManager.MakeThread();
		g_MyD3D->m_fileDownloadManager.Start();
		MAKE_LOAD_PROFILE( "m_fileDownloadManager" );

		//==========================================================
		// RegisterLuabind

		MONSTER::RegisterLuaBind();
		PLAYER::RegisterLuabind();

		KDSound::RegisterLuabind();
		g_kCamera.RegisterLuabind();

		g_MyD3D->MyCtrl->RegisterLuaBind();
		KGCPlayerCommon::RegisterLuaBind();
#ifdef __ORIGIN_DEV__ && defined(SKILL_TREE)
		SiGCSKT()->RegisterLuabind();
#endif
		MAKE_LOAD_PROFILE( "RegisterLuaBind" );

#if defined( USE_SOCIAL_MOTION )
		SiKGCSocialMotionManager()->LoadSocialMotionInfo("SocialMotionInfo.stg");
		MAKE_LOAD_PROFILE( "SocialMotionInfo" );
#endif

		GCSquare::SquareInit();
		SiGCSquare()->LoadSquareBaseEquipScript();
		MAKE_LOAD_PROFILE( "LoadSquareBaseEquipScript" );

		if( SiKGCMissionManager()->LoadScript() == false )
			g_MyD3D->ExitWithMsgDlg( L"Mission Script Load Failure!!" , L"" );

		MAKE_LOAD_PROFILE( "SiKGCMissionManager" );
		LoadCTMissionString();            
		LoadSkillString();
#if defined( USE_AWAKENING_SKILL_TREE )
		// �Ž�ųƮ�� �ε�κ�
		LoadSkillTreeScirpt();
#endif

		SiKNeonSign()->LoadNeonSign();
		MAKE_LOAD_PROFILE( "StringEtc" );

		SiKGCDungeonInfoManager()->LoadStringFile();
		SiKGCDungeonInfoManager()->LoadScriptFile();
		MAKE_LOAD_PROFILE( "SiKGCDungeonInfoManager" );

		KGCPrograssBar::RegisterLuabind();
		SiKGCInGamePrograssBar()->RegisterLuabind();

		KGCEmbarkObject::RegisterLuaBind();
		SiKGCEmbarkManager()->RegisterLuaBind();
		MAKE_LOAD_PROFILE( "SiKGCEmbarkManager" );

		KGCSubject::RegisterLuaBind();
		SiKGCSubjectManager()->RegisterLuaBind();
		MAKE_LOAD_PROFILE( "SiKGCSubjectManager" );

		KGCWall::RegisterLuaBind();
		SiKGCWallManager()->RegisterLuaBind();
		MAKE_LOAD_PROFILE( "SiKGCWallManager" );

		g_MyD3D->m_KGCGuildMark.InitDevice();
		g_pGraphicsHelper->OnInitDevice();

		g_RenderManager->SetUsingVS( true );
		MAKE_LOAD_PROFILE( "InitGraphicsHelper" );
		ShowloadingBar(8);
		break;
	case 5:
		g_MyD3D->MyStg->InitDevice();
		g_MyD3D->MySparks->InitDevice();
		g_MyD3D->m_pDamageManager->InitDevice();
		g_ParticleManager->InitDevice( g_pd3dDevice, &g_kCamera.m_matCamera );

		for( int i = 0; i < MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO; i++ ) 
		{
			g_MyD3D->MyPlayer[i]->InitDevice();
		}

		g_MyD3D->MyCtrl->InitDevice();
		g_MyD3D->MyHead->InitDevice();
		ShowloadingBar(1);
		MAKE_LOAD_PROFILE( "InitDevice" );
		break;
	case 6:
		g_MyD3D->MyItms->InitDevice();
		MAKE_LOAD_PROFILE( "initMyItems" );
		ShowloadingBar(1);
		break;
	case 7:
		MAKE_LOAD_PROFILE( "Monster" );
		ShowloadingBar(1);
		break;
	case 8:
		SiKGCEmoticonManager()->LoadFromLuaScript("EmoticonList.stg");
		MAKE_LOAD_PROFILE( "LoadEmoticon" );
		SiKGCObjectEgoMgr()->LoadFromLuaScript("EgoItemList.stg");
		MAKE_LOAD_PROFILE( "LoadEgoItem" );
		SiKGCFaceLift()->LoadFromLuaScript("SetFaceLift.stg");
		MAKE_LOAD_PROFILE("SetFaceLift");
		g_kGlobalValue.LoadStatsMIDList("CharacterStats_MID_List.stg");
		MAKE_LOAD_PROFILE( "LoadCharacterStatsMidList" );     

		SiGCGrowingSkillManager()->LoadFromLuaScript("SetGrowingSkill.stg");
		MAKE_LOAD_PROFILE( "SetGrowingSkill" );

#if defined( USE_AGIT_SYSTEM )
		SiKGCAgitGameManager()->LoadObjectInfo("AgitObjectInfo.stg");
		MAKE_LOAD_PROFILE( "AgitObjectInfo" );

		SiKGCAgitGameManager()->LoadSpecialObjectInfo("AgitSpecialObjectInfo.stg");
		MAKE_LOAD_PROFILE( "AgitSpecialObjectInfo" );
#endif

		ShowloadingBar(1);
		break;
	case 9:
		g_MyD3D->m_pkUIMgr->InitDeviceObjects();
		MAKE_LOAD_PROFILE( "LoadUITexture" );
		ShowloadingBar(1);
		break;
	case 10:
		g_AnimManager.InitDeviceObjects();
		strAnimScript = "AnimEff.stg";
		g_AnimManager.LoadFromLuaScript( "etc.stg", (char*) strAnimScript.c_str() );
		MAKE_LOAD_PROFILE( "LoadAnimManager" );
		ShowloadingBar(12);
		break;
	case 11:
		strAnimScript = "GCMapAnim.stg";
		g_MyD3D->m_pMapAnim = new CKTDGAnim(  g_MyD3D->m_pd3dDevice );
		g_MyD3D->m_pMapAnim->Compile( (char*)strAnimScript.c_str() );
		MAKE_LOAD_PROFILE( "LoadMapAnim" );
		ShowloadingBar(14);
		break;
	case 12:
		strAnimScript = "GCPetAnim.stg";
		g_MyD3D->m_pPetAnim = new CKTDGAnim(  g_MyD3D->m_pd3dDevice );
		g_MyD3D->m_pPetAnim->Compile( (char*)strAnimScript.c_str() );
		MAKE_LOAD_PROFILE( "LoadPetAnim" );
		ShowloadingBar(3);
		break;
	case 13:
		strParticleScript = g_strCurrentPath + "Stage\\GCNewMapParticleScript.stg";
		strItemScript = "ParticleEff.stg";
		g_ParticleManager->Load( (char*)strParticleScript.c_str(), (char*)strItemScript.c_str() );

		{
			KLuaManager luaMgr;
			if( GCFUNC::LoadLuaScript( luaMgr, "particles.lua" ) )
			{
				if( SUCCEEDED( luaMgr.BeginTable("ParticleFiles") ) )
				{
					std::string strFileName;
					for(int i = 1;SUCCEEDED(luaMgr.GetValue(i,strFileName)); i++)
					{
						g_ParticleManager->CompileScript( strFileName.c_str() );
					}
					luaMgr.EndTable();
				}            
			}
		}
		MAKE_LOAD_PROFILE( "LoadParticle" );


		g_kGlobalValue.LoadCharGainParticleInfo( "CharGainParticleInfo.lua" );

#ifdef ENCHANT_EFFECT
#if defined( LOAD_ENCHANT_EFFECT_FROM_SCRIPT )
		g_kGlobalValue.LoadEnchantEffectInfo( "EnchantEffect.stg" );
		MAKE_LOAD_PROFILE( "EnchantEffect" );
#endif
#endif

		ShowloadingBar(19);

		break;
	case 14:
		strAnimScript = "GCUIAnim.stg";
		g_MyD3D->m_pUIAnim = new CKTDGAnim(  g_MyD3D->m_pd3dDevice );
		g_MyD3D->m_pUIAnim->Compile( (char*)strAnimScript.c_str() );
		ShowloadingBar(1);
		MAKE_LOAD_PROFILE( "LoadUIAnim" );
		break;
	case 15:
#ifndef __PATH__ 
		g_MyD3D->m_kGuideMgr.RegisterFileMoniter();
#endif
		g_MyD3D->m_kGuideMgr.LoadScript();
		MAKE_LOAD_PROFILE( "GuideMgr Load Script" );
		ShowloadingBar(1);
		break;
	case 16:
		// PetItem ����
		g_pkPetAI = new CPetAI();
		g_pkPetAI->LoadPetItemFromLuaScript( "etc.stg", "PetItem.stg" );
		MAKE_LOAD_PROFILE( "LoadPetAI" );
		ShowloadingBar(1);
		break;
	case 17:
		g_MyD3D->m_pFrustum = new CKTDGFrustum( g_MyD3D->m_pd3dDevice );
		MAKE_LOAD_PROFILE( "KTDGFrustum" );
		ShowloadingBar(1);
		break;
	case 18:
		// ���� �ʱ�ȭ	
		strMusicScript = "music.stg";
		strMusicPath = g_strCurrentPath + "Data\\Music\\";
#ifdef MCI
		g_KMci.Init( g_hInstance, NULL, strMusicScript.c_str(), strMusicPath.c_str() );
		g_KMci.Play( GC_BGM_MAIN_THEME, false );
		g_KMci.Stop();
#endif

#ifdef DSHOW
		g_KMci.Init( g_hInstance, g_MyD3D->m_hWnd, strMusicScript.c_str(), strMusicPath.c_str() );
#endif
		ShowloadingBar( 1 );
		MAKE_LOAD_PROFILE( "InitMusic" );
		break;

	case 19:
		strSoundScript = "sound.stg";
		// ���� �ʱ�ȭ
		if( g_KDSound.Init( g_MyD3D->Get_hWnd() ) == false )
		{
			EnableSound = false;
			Is_Sound = false;
		}
		else
		{
			if( g_KDSound.LoadFromLuaScript( 100, strSoundScript.c_str(), g_strCurrentPath.c_str() ) == false )
			{
				MessageBoxA( NULL, "sound.stg Load Failed!", NULL, MB_OK );
				EnableSound = false;
				Is_Sound = false;
			}
			else
			{
				EnableSound = true;
				Is_Sound = true;
			}
		}
		MAKE_LOAD_PROFILE( "InitSound" );
		ShowloadingBar(1);
		break;
	case 20:
		// �ɼǿ��� �ε��ߴ� ���� ���� ����
		g_MyD3D->m_KGCOption.SetSound();
		g_MyD3D->m_KGCOption.SetMusic();
		g_MyD3D->m_KGCOption.SetUsingVS();
		// ���� �ε��� ���������� ���� �÷��� ���ϰ� �Ѵ�..
		g_pOptionSound->SetEnable( Is_Sound, g_pOptionSound->GetVolume() );

		//g_pMeshEffectManager->Init( g_pd3dDevice, "MeshEffect.lua" );

		ShowloadingBar( 1 );
		MAKE_LOAD_PROFILE( "InitOption" );
		break;
	case 21:
		g_MyD3D->m_bKeepFPS60 = true;

		//LoadManufactureScript();

		SiKGCDropItemManager()->Init();
		g_kGlobalValue.m_kRanking.LoadRankgerReward();

		// �̴ϰ��� ���� �ʱ�ȭ
		SiKGCMinigameManager()->BindLua();

		SiKGCMath()->BindLua();

		// ���� �ʱ�ȭ
		g_pMagicEffect->LoadFromLuaScript( "BuffInfo.stg" );

		// ����ġ ~~~
		g_kGlobalValue.LoadAdjustLevelInfo();
		g_kGlobalValue.SetCashQuantityList();

		//Sistema de Skin de Dano
		g_MyD3D->m_kFontSkinMgr.SetFontSkinList();
		g_MyD3D->m_kRoomAnimationMgr.SetRoomAnimationList();
		g_MyD3D->m_kCutInSkinMgr.SetCutInSkinList();

		g_kGlobalValue.GetEventScheduleList();

		g_kGlobalValue.GetCharacterCollectionList();
		g_kGlobalValue.GetVIPCollectionList();

		ShowloadingBar( 1 );
		MAKE_LOAD_PROFILE( "LastEtc" );

		std::vector<std::pair <int, int> > vecNickNameCheck;
		std::vector<std::pair <int, int> > vecUnicodeCheck;

		vecNickNameCheck.clear();
		vecUnicodeCheck.clear();

		if ( g_MyD3D->LoadLanguageFilterScript( vecNickNameCheck, vecUnicodeCheck ) )
		{
			std::vector<std::pair <int, int>>::iterator vit = vecNickNameCheck.begin();
			if ( vit != vecNickNameCheck.end() )
			{
				for ( vit ; vit != vecNickNameCheck.end() ; vit++)
				{
					SiKSlangFilter()->AddNickNameLanguage( vit->first, vit->second );
				}
			}

			std::vector<std::pair <int, int>>::iterator vit2 = vecUnicodeCheck.begin();
			if ( vit2 != vecUnicodeCheck.end() )
			{
				for ( vit2 ; vit2 != vecUnicodeCheck.end() ; vit2++)
				{
					SiKSlangFilter()->AddLanguage( vit2->first, vit2->second );
				}
				SiKSlangFilter()->SetEnableUnicode( true );
			}
			else 
			{
				SiKSlangFilter()->SetEnableUnicode( false );
			}
		}
		else
		{
			::MessageBoxA( NULL, "Language Filter Script �б� ����", NULL, NULL );
		}

		SiKSlangFilter()->LoadFilterFromKOM("Total.stg", 1, KSlangFilter::SLANG_FILTER_CHAT);
		SiKSlangFilter()->LoadFilterFromKOM("Total.stg", 1, KSlangFilter::SLANG_FILTER_NICKNAME);
		MAKE_LOAD_PROFILE( "NickNameUniCodeCheck" );

#ifdef _LOADING_TIMECHECK_
		if ( g_pFile ) 
			fclose( g_pFile );

		FileOutLevelUpStageCalc();	
#endif
		g_kGlobalValue.RunLogProgram("9");
		break;
	}
	m_iJobStep++;



	return true;
}

bool KGCStateInitLoading::ShowloadingBar(int iLoadedResource)
{
	m_iLoadedResource += iLoadedResource;
	std::wstringstream strLoadingfinal;
	strLoadingfinal << g_pkStrLoader->GetString( STR_ID_LSLOADING_TEXT ) << L" " << m_iLoadedResource << L"%";
	strLoadingPercent = strLoadingfinal.str();
	if ( m_iLoadedResource > ALL_RESOURCE_LOADED )
		return false;

	return true;
}


HRESULT KGCStateInitLoading::FinishLoading()
{
	HRESULT hr = g_MyD3D->RestoreDeviceObjects();

	if( SUCCEEDED(hr) )
	{
		return S_OK;
	}
	else
	{
		DeleteDevice();
		g_MyD3D->InvalidateDeviceObjects();
		g_MyD3D->DeleteDeviceObjects();
		SAFE_RELEASE(g_MyD3D->m_pd3dDevice);

		return hr;
	}
	return S_OK;
}

bool KGCStateInitLoading::FrameMove( float fElapsedTime )
{
	if( m_iLoadedResource >= ALL_RESOURCE_LOADED)
	{
		if ( FAILED(FinishLoading()) )
		{
			KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INITIALIZE_FAIL1 ).c_str(), g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INITIALIZE_FAIL2 ).c_str(), KMsgBox::MBOX_OK );
			SendMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
		}

		//g_MyD3D->m_pStateMachine->GoLogin(); //�ε��� ��� ������ Logo ȭ������ ü�����Ѵ�.

		g_MyD3D->m_pStateMachine->Clear_SavedState();

		HRESULT hr = StartUpServer();
		if (hr != S_OK)
		{
			if (hr == E_FAIL)
			{
				g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL1).c_str(), g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL2).c_str());
			}
			else if (hr == E_ABORT)
			{
				g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_WEB_BROWSER_INIT1).c_str(), g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_WEB_BROWSER_INIT2).c_str());
			}
			else
			{
				g_MyD3D->ExitWithMsgDlg(L"", L"");
			}
			return false;
		}

		g_MyD3D->m_pkUIMgr->MoveUIScene("main");
		g_MyD3D->m_pStateMachine->GoLogin();

		//���ø� ����..
		if (g_MyD3D->m_KGCOption.SetSampleing(g_MyD3D->m_KGCOption.GetSampleInfo()) == FALSE)
		{
			g_MyD3D->RestoreResoultionSetting();
		}

		g_MyD3D->SetResolution(TRUE); //�ػ� �ٷ� ���� �մϴ�.

		if ( g_MyD3D->Default_Fullscreen )
		{
			::SendMessage( g_MyD3D->m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );
		}

		DeleteDevice();
	}
	else
	{       
		LoadResource();        
	}
	return true;
}

bool KGCStateInitLoading::Render()
{
	if (BeginScene())
	{
		g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0L);

		D3DXVECTOR4 rtWholeUv = D3DXVECTOR4(0, 0, 512, 512);
		D3DXVECTOR4 rtPartUv1 = D3DXVECTOR4(0, 0, 512, 256);
		D3DXVECTOR4 rtPartUv2 = D3DXVECTOR4(0, 256, 512, 512);

		RECT rtBackGround1 = { 0,0,512,512 };
		RECT rtBackGround2 = { 512,0,1024,512 };
		RECT rtBackGround3 = { 0,512,512,768 };
		RECT rtBackGround4 = { 512,512,1024,768 };
		RECT rtBackExtra = { 0,640,128,768 };
		RECT rtBarLeftcircle = { 0, 759, 1024, 768 };

		D3DXVECTOR4 rtBarLeftcircleUv = D3DXVECTOR4(3.0f, 3.0f, 457.0f, 32.0f);
		RECT rtBar = { 0, 759, 1024, 768 };
		D3DXVECTOR4 rtBarUv = D3DXVECTOR4(9.0f, 40.0f, 449.0f, 55.0f);

		int iLocalWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int iLocalHeight = ::GetSystemMetrics(SM_CYSCREEN);

		if (iLocalWidth < GC_SCREEN_WIDTH || iLocalHeight < GC_SCREEN_HEIGHT)
		{
			rtBackGround1.left = 0;
			rtBackGround1.top = 0;
			rtBackGround1.right = 400;
			rtBackGround1.bottom = 400;
			rtBackGround2.left = 400;
			rtBackGround2.top = 0;
			rtBackGround2.right = 800;
			rtBackGround2.bottom = 400;
			rtBackGround3.left = 0;
			rtBackGround3.top = 400;
			rtBackGround3.right = 400;
			rtBackGround3.bottom = 600;
			rtBackGround4.left = 400;
			rtBackGround4.top = 400;
			rtBackGround4.right = 800;
			rtBackGround4.bottom = 600;
		}
		g_pGCDeviceManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

#if defined(LOADING_IMAGE_FROM_SERVER_SCRIPT)
		Render2D(m_pTexLoadingTexture_from_massfile[0]->GetDeviceTexture(), rtBackGround1, rtWholeUv);
		Render2D(m_pTexLoadingTexture_from_massfile[1]->GetDeviceTexture(), rtBackGround2, rtWholeUv);
		Render2D(m_pTexLoadingTexture_from_massfile[2]->GetDeviceTexture(), rtBackGround3, rtPartUv1);
		Render2D(m_pTexLoadingTexture_from_massfile[2]->GetDeviceTexture(), rtBackGround4, rtPartUv2);
		Render2D(m_pTexLoadingBarTexture_from_massfile->GetDeviceTexture(), rtBarLeftcircle, rtBarLeftcircleUv);
		Render2D(m_pTexLoadingBarTexture_from_massfile->GetDeviceTexture(), rtBar, rtBarUv, ((float)(m_iLoadedResource + 1) / (float)ALL_RESOURCE_LOADED));
#else
		Render2D(m_pLoadingTexture[0], rtBackGround1, rtWholeUv);
		Render2D(m_pLoadingTexture[1], rtBackGround2, rtWholeUv);
		Render2D(m_pLoadingTexture[2], rtBackGround3, rtPartUv1);
		Render2D(m_pLoadingTexture[2], rtBackGround4, rtPartUv2);
		Render2D(m_pLoadingBarTexture, rtBarLeftcircle, rtBarLeftcircleUv);
		Render2D(m_pLoadingBarTexture, rtBar, rtBarUv, ((float)(m_iLoadedResource + 1) / (float)ALL_RESOURCE_LOADED));
#endif

		if (m_pLoadingExtraTexture)
			Render2D(m_pLoadingExtraTexture, rtBackExtra, rtWholeUv);

		g_pGCDeviceManager2->PushState();
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pkFontMgrOutline->OutTextXY((980 - g_pkFontMgr->GetWidth(strLoadingPercent.c_str())), (GC_SCREEN_FLOAT_HEIGHT - 29) * g_pGCDeviceManager2->GetWindowScaleY(), strLoadingPercent.c_str());

		g_pGCDeviceManager2->PopState();

		EndScene();
	}
	g_MyD3D->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

void KGCStateInitLoading::Render2D(LPDIRECT3DTEXTURE9 pTexture , RECT rtScreen ,D3DXVECTOR4 rtUv, float fWidthRatio, bool bRotate)
{
	float fRight = (float)rtScreen.left + (float)(rtScreen.right - rtScreen.left ) * fWidthRatio;

	if(!bRotate)
		rtUv += D3DXVECTOR4(0.5f,0.5f,0.5f,0.5f);
	else
	{
		rtUv += D3DXVECTOR4(0.5f,-0.5f,0.5f,-0.5f);
	}
	rtUv /= 512.0f;

	m_rect[0].m_vPos = D3DXVECTOR4((float)rtScreen.left ,(float)rtScreen.top , 0.8f , 1.0f);
	m_rect[0].m_diff = 0xffffffff;	
	m_rect[0].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.y);

	m_rect[1].m_vPos = D3DXVECTOR4(		fRight			,(float)rtScreen.top , 0.8f , 1.0f);
	m_rect[1].m_diff = 0xffffffff;
	if( !bRotate )
		m_rect[1].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.y);
	else
		m_rect[1].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.w);

	m_rect[2].m_vPos = D3DXVECTOR4((float)rtScreen.left ,(float)rtScreen.bottom , 0.8f , 1.0f);
	m_rect[2].m_diff = 0xffffffff;
	if( !bRotate )
		m_rect[2].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.w);
	else
		m_rect[2].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.y);


	m_rect[3].m_vPos = D3DXVECTOR4(		fRight			,(float)rtScreen.bottom , 0.8f , 1.0f);
	m_rect[3].m_diff = 0xffffffff;
	m_rect[3].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.w);

	if (g_pGCDeviceManager->GetWindowScaleX() > 1.0f || g_pGCDeviceManager->GetWindowScaleY() > 1.0f)
	{
		for (int i=0 ;i<4;i++)
		{
			m_rect[i].m_vPos.x = m_rect[i].m_vPos.x*g_pGCDeviceManager->GetWindowScaleX();
			m_rect[i].m_vPos.y = m_rect[i].m_vPos.y*g_pGCDeviceManager->GetWindowScaleY();
		}
	}

	g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,          FALSE );
	g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_MyD3D->m_pd3dDevice->SetTexture( 0, pTexture );	
	g_MyD3D->m_pd3dDevice->SetFVF( KD3DUIVertex::FVF );
	g_MyD3D->m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_rect, sizeof( KD3DUIVertex ) );
}


void KGCStateInitLoading::DeleteDevice()
{
#if defined(LOADING_IMAGE_FROM_SERVER_SCRIPT)
	std::vector<GCDeviceTexture*>::iterator vit = m_pTexLoadingTexture_from_massfile.begin();
	for (; vit != m_pTexLoadingTexture_from_massfile.end() ; ++vit)
	{
		SAFE_RELEASE( *vit );
	}
	m_pTexLoadingTexture_from_massfile.clear();

	SAFE_RELEASE( m_pTexLoadingBarTexture_from_massfile );
#else
	for ( int i = 0 ; i < 3 ; ++i)
		SAFE_RELEASE( m_pLoadingTexture[i] );
	SAFE_RELEASE( m_pLoadingBarTexture );
#endif
	SAFE_RELEASE( m_pLoadingExtraTexture );

}

bool KGCStateInitLoading::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateInitLoading::OnSetupFSM()
{
	ON_SETUP_FSM( GS_LOGIN, FAIL_N_SHUTDOWN );
	m_eStateForFail = FAIL_N_SHUTDOWN;
}

void KGCStateInitLoading::OnDestroy()
{

}

void KGCStateInitLoading::LoadCTMissionString()
{
	std::string strCTMissionScript( "initCTMission.lua" );
	std::string strCTMissionString( "str_CTMission.stg" );

	KLuaManager* kCTMissionLua = new KLuaManager();
	if(SiKGCMultipleLanguages()->UseMultipleLanguages())
	{
		strCTMissionString = SiKGCMultipleLanguages()->ConvertNationStringFileNameA("str_CTMission_");
	}

	GCFUNC::LoadLuaScript( *kCTMissionLua, strCTMissionScript );
	if( SiKGCCTMissionManager()->LoadMissionFile( strCTMissionScript, kCTMissionLua ) == false )	//�Լ� ���ο��� Shared_Ptr�� �Ҵ��ϱ� ������ LuaManager ���� ��;
	{
		g_MyD3D->ExitWithMsgDlg( L"CollectionMission Script Load Failure!!" , L"" );
	}

	if(SiKGCCTMissionManager()->LoadMissionStringFromKOM( strCTMissionString ) == false )
	{
		g_MyD3D->ExitWithMsgDlg( L"CollectionMission String Load Failure!!" , L"" );
	}
}

void KGCStateInitLoading::LoadSkillString()
{
	std::string strSkillString( "str_Skill" );

	KLuaManager kSkillTreeLua;
	GCFUNC::LoadLuaScript( kSkillTreeLua, "Enum.stg" );
	GCFUNC::LoadLuaScript( kSkillTreeLua, "MotionEnum.stg" );
	GCFUNC::LoadLuaScript( kSkillTreeLua, "BuffEnum.stg" );

	if (SiKGCMultipleLanguages()->UseMultipleLanguages())
	{
		strSkillString += "_";
		strSkillString += SiKGCMultipleLanguages()->GetLanguageTypeNUM();
	}
	strSkillString += ".stg";

#if defined(SKILL_TREE)

	for( int i = 0; i < GC_CHAR_NUM; i++ ) 
	{
		for( int j = 0; j < NUM_JOB_LEVEL; j++ ) 
		{
			std::stringstream strSkillScript;
			strSkillScript << "InitSkillTree" << i << "_" << j << ".lua";

			if( GCFUNC::LoadLuaScript( kSkillTreeLua, strSkillScript.str() ) ) 
			{
				if( SiGCSKT()->LoadSkillScript( kSkillTreeLua ) == false )
				{
					g_MyD3D->ExitWithMsgDlg( L"SkillTree Script Load Failure!!" , L"" );
				}
			}             
		}
	}
	GCFUNC::LoadLuaScript( kSkillTreeLua, "SkillResource.lua" );
	GCFUNC::LoadLuaScript( kSkillTreeLua, "SkillGroup.lua" );        

	if( SiGCSKT()->LoadSkillResource( kSkillTreeLua ) == false )
	{
		g_MyD3D->ExitWithMsgDlg( L"SkillRes Script Load Failure!!" , L"" );
	}

	if( SiGCSKT()->LoadSkillGroupScript( kSkillTreeLua ) == false )
	{
		g_MyD3D->ExitWithMsgDlg( L"SkillGroup Script Load Failure!!" , L"" );
	}

	if (SiGCSKT()->LoadSkillStringFromKOM(strSkillString) == false)
	{
		g_MyD3D->ExitWithMsgDlg(L"SkillTree String Load Failure!!", L"");
	}
#endif
}

bool KGCStateInitLoading::OnInit()
{
	return true;
}