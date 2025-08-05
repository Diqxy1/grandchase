#include "stdafx.h"
#include "GCPetInfoBox.h"
//
#include "MyD3D.h"


//
#include "gcui/GCMyInfoScene.h"
#include "gcui/GCPetWnd.h"
#include "gcui/KGCPetMessage.h"

IMPLEMENT_CLASSNAME( KGCPetInfoBox );
IMPLEMENT_WND_FACTORY( KGCPetInfoBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPetInfoBox, "gc_pet_info_box" );

#define PET_HUNGRY_RATE_GAUGE_WIDTH	253.0f
#define MAX_SATIATION 6000

KGCPetInfoBox::KGCPetInfoBox( void )
{
	m_dwPetUID = 0;

	m_pkStaticPetName = NULL;
	m_pkStaticPetLevel = NULL;
	m_pkStaticPetHungryRate = NULL;
	m_pkStaticSatiation = NULL;
	
	m_pkPetWhiteGauge = NULL;

	m_iFoodCount = 0;
	m_iSatiation = 0;

	for( int i = 0; i < 5; i++ )
	{
		m_apkPetFood[i] = NULL;
	}

	LINK_CONTROL( "pet_info_static_name", m_pkStaticPetName );
	LINK_CONTROL( "pet_info_static_level", m_pkStaticPetLevel );
	LINK_CONTROL( "pet_info_static_hungry_rate", m_pkStaticPetHungryRate );
	LINK_CONTROL( "pet_info_static_satiation", m_pkStaticSatiation );
	

	LINK_CONTROL( "pet_info_white_gauge",	m_pkPetWhiteGauge	);

	LINK_CONTROL( "pet_info_food1", m_apkPetFood[0] );
	LINK_CONTROL( "pet_info_food2", m_apkPetFood[1] );
	LINK_CONTROL( "pet_info_food3", m_apkPetFood[2] );
	LINK_CONTROL( "pet_info_food4", m_apkPetFood[3] );
	LINK_CONTROL( "pet_info_food5", m_apkPetFood[4] );

	m_pkStaticAK = NULL;
	LINK_CONTROL( "pet_info_static_ak", m_pkStaticAK );
	m_pkStaticDF = NULL;
	LINK_CONTROL( "pet_info_static_df", m_pkStaticDF );
	m_pkStaticVT = NULL;
	LINK_CONTROL( "pet_info_static_vt", m_pkStaticVT );

	m_bChange = true;
}

KGCPetInfoBox::~KGCPetInfoBox( void )
{
}

void KGCPetInfoBox::OnCreate( void )
{
	
	m_pkStaticPetLevel->InitState( true, false, NULL );
	m_pkStaticPetHungryRate->InitState( true, false, NULL );
	m_pkStaticSatiation->InitState( true, false, NULL );

	m_pkPetWhiteGauge->InitState(false, false, NULL );

	m_pkStaticSatiation->SetAlign( DT_CENTER );

	m_pkStaticPetName->SetText( g_pkStrLoader->GetString( STR_ID_NO_EQUIP_PET ) );
	m_pkStaticPetLevel->SetText( L"" );
	m_pkStaticPetHungryRate->SetText( g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE ) );
	m_pkStaticSatiation->SetText( L"" );


	for( int i = 0; i < 5; i++ )
	{
		m_apkPetFood[i]->InitState( false );
	}

	m_pkStaticAK->InitState(true , false );
	m_pkStaticDF->InitState(true , false );
	m_pkStaticVT->InitState(true , false );
}

void KGCPetInfoBox::FrameMoveInEnabledState( void )
{
	// 펫이 없을 때
	if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwUID == 0 )
	{
		if( m_dwPetUID == 0 )
			return;

		m_dwPetUID = 0;

		m_pkStaticPetName->SetText( g_pkStrLoader->GetString( STR_ID_NO_EQUIP_PET ) );
		m_pkStaticPetLevel->SetText( L"" );
		m_pkStaticPetHungryRate->SetText( g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE ) );
		m_pkStaticSatiation->SetText( L"" );

		SetHungryRate( 0 );

		m_pkPetWhiteGauge->ToggleRender(false);
	}
	else
	{
		KPetInfo kPetInfo = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;

        kPetInfo.m_iSatiation = g_MyD3D->m_mapPetInfo[kPetInfo.m_dwUID].m_iSatiation;

        DWORD dwPetExp = kPetInfo.m_dwEXP;
        int iPetLevel = GetPetLevel( dwPetExp );
        std::wostringstream strmLevel, strmSatiation;

        strmLevel << g_pkStrLoader->GetString( STR_ID_LEVEL ) << " " << iPetLevel << "." ;
        strmSatiation << m_iOldSatiation << " / " << MAX_SATIATION;

        m_pkStaticPetName->SetText( kPetInfo.m_strName );
        m_pkStaticPetLevel->SetText( strmLevel.str() );
        m_pkStaticPetHungryRate->SetText( g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE ) );
        m_pkStaticSatiation->SetText( strmSatiation.str() );	

		if (m_iOldSatiation <= 0 )
			m_pkPetWhiteGauge->ToggleRender(false);
		else
			m_pkPetWhiteGauge->ToggleRender(true);

		m_pkPetWhiteGauge->SetWidth( (int)( (float)(m_iOldSatiation)/ (float)MAX_SATIATION * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);

		if( m_dwPetUID == kPetInfo.m_dwUID				// 같은펫이고
			&& m_iSatiation == kPetInfo.m_iSatiation	// 만복도도 같다
			&& m_iSatiation == m_iOldSatiation )
		{
			if( m_bChange )
			{
				SetHungryRate( kPetInfo.m_iSatiation );
				GetPetState( kPetInfo.m_iSatiation );
				m_bChange = false;
			}
			return;
		}

		m_bChange = true;

		// 같은 펫인데 만복도가 달라졌다
		if( m_dwPetUID == kPetInfo.m_dwUID 
			&& m_iSatiation != kPetInfo.m_iSatiation )
		{
			m_iOldSatiation = m_iSatiation;
			m_iSatiation = kPetInfo.m_iSatiation;
			m_iProcessSatiation = m_iSatiation - m_iOldSatiation;		

			if( m_iProcessSatiation > 0 )
			{
				std::wstring str1 = g_pkStrLoader->GetString( STR_ID_HUNGRYRATE_MESSAGE3 );
				std::wstring str2 = g_pkStrLoader->GetReplacedString( STR_ID_HUNGRYRATE_MESSAGE4, "i", ( m_iProcessSatiation ) );							
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str1, str2 );

				PetFeeding( m_iSatiation );
			}

			SetHungryRate( m_iOldSatiation );
		}
		else if( m_dwPetUID != kPetInfo.m_dwUID )
		{
			m_dwPetUID = kPetInfo.m_dwUID;
			m_iSatiation = m_iProcessSatiation = kPetInfo.m_iSatiation;
			m_iOldSatiation = m_iSatiation;
			m_iProcessSatiation = m_iSatiation - m_iOldSatiation;	

			SetHungryRate( m_iSatiation );
		}
		else
		{
			FrameMoveHungryRate();
		}	
	}
	ShowPetInfo();
}

void KGCPetInfoBox::FrameMoveHungryRate()
{
	if( m_iOldSatiation >= m_iSatiation )
	{
		m_iOldSatiation = m_iSatiation;
	}
	
	if( m_iOldSatiation == m_iSatiation )
	{
		return;
	}
	
// 	if( m_iOldSatiation == 0 )
// 	{
// 	}
// 	else if( m_iSatiation < 250 && m_iFoodCount == 0 )
// 	{
// 		m_pkPetRedGauge->ToggleRender( true );
// 		m_pkPetGreenGauge->ToggleRender( false );		
// 		m_pkPetRedGauge->SetWidth( (int)( (float)(m_iOldSatiation % 1001 )/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);	//8은 Left와 Right의 폭입니다.
// 		m_pkPetGreenGauge->SetWidth( (int)( (float)(m_iOldSatiation % 1001 )/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH )  + 8);
// 	}
// 	else
// 	{
// 		m_pkPetGreenGauge->ToggleRender( true );		
// 		m_pkPetRedGauge->ToggleRender( false );
// 		m_pkPetGreenGauge->SetWidth( (int)( (float)(m_iOldSatiation % 1001 )/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 	}
// 
// 	if( m_iOldSatiation != m_iSatiation )
// 	{
// 		if( m_iProcessSatiation >= 1000 )
// 		{
// 			m_pkPetWhiteGauge->ToggleRender( true );
// 			m_pkPetWhiteGauge->SetWidth( (int)( 1000.0f/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 		}
// 		else
// 		{
// 			m_pkPetWhiteGauge->ToggleRender( true );
// 			m_pkPetWhiteGauge->SetWidth( (int)( (float)(m_iSatiation % 1001 )/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 		}
// 	}



	m_iFoodCount = ( m_iOldSatiation-1 ) / 1000;


	for( int i = 0; i < 5; i++ )
	{
		if( m_iFoodCount - 1 < i )
			m_apkPetFood[i]->ToggleRender(false);
		else
			m_apkPetFood[i]->ToggleRender( true );
	}	


	if( m_iOldSatiation < m_iSatiation )
	{
		m_iOldSatiation += 5;		
	}
}

void KGCPetInfoBox::SetHungryRate( int iSatiation )
{
	for( int i = 0; i < 5; i++ )
	{
		m_apkPetFood[i]->InitState( false );
	}

//	m_pkPetWhiteGauge->ToggleRender(false);

//	m_pkPetWhiteGauge->SetWidth( 0 );

	int iCount = 0;
	while( iSatiation > 1000 && iCount < 5 )
	{
		m_apkPetFood[iCount]->ToggleRender( true );

		iSatiation -= 1000;
		iCount++;
	}

	m_iFoodCount = iCount;
// 
// 	if( iSatiation == 0 )
// 	{
// 	}
// 	else if( iSatiation < 250 && m_iFoodCount == 0 )
// 	{
// 		m_pkPetRedGauge->ToggleRender( true );
// 
// 		m_pkPetRedGauge->SetWidth( (int)( (float)(iSatiation)/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 		m_pkPetGreenGauge->SetWidth( (int)( (float)(iSatiation)/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 	}
// 	else
// 	{
// 		m_pkPetGreenGauge->ToggleRender( true );
// 		m_pkPetGreenGauge->SetWidth( (int)( (float)(iSatiation)/ (float)MAX_SATIATION * 6.0f * PET_HUNGRY_RATE_GAUGE_WIDTH ) + 8);
// 	}

	m_iFoodCount = iCount;
}

void KGCPetInfoBox::PetFeeding( int iSatiation )
{
	if( iSatiation <= 3000 )
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->PutStateMessage( PET_MENT_FEEDING_3000 );
	}
	else if( iSatiation <= 6000 )
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->PutStateMessage( PET_MENT_FEEDING_6000 );
	}
}

void KGCPetInfoBox::NoMoreFoodAnim()
{
	if( m_iOldSatiation != m_iSatiation )
	{
		m_iOldSatiation = m_iSatiation;
		m_iProcessSatiation = 0;		
	}
}

std::wstring KGCPetInfoBox::GetPetState( int iSatiation )
{
	if( iSatiation >= 1000 )
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->SetMentState( PET_MENT_MYINFO );
		return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE3 );
	}
	else if( iSatiation >= 250 )
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->SetMentState( PET_MENT_MYINFO );
		return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE2 );
	}
	else if( iSatiation == 0 )
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->SetMentState( PET_MENT_HUNGRY_ZERO );
		return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE1 );
	}
	else
	{
		g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->m_pkPetWnd->m_pkPetMsg->SetMentState( PET_MENT_HUNGRY );
		return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE1 );
	}
}

void KGCPetInfoBox::ShowPetInfo( void )
{
	KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
	if (kPet.m_dwID == 0 )
	{
		m_pkStaticAK->SetText( L"" );
		m_pkStaticDF->SetText( L"" );
		m_pkStaticVT->SetText( L"" );
		return;
	}

	std::wostringstream  strmDesc;
	std::wstring         strDesc;

	
	DWORD dwPetExp = kPet.m_dwEXP;
	int iPetLevel = GetPetLevel( dwPetExp );

	// 패널티를 받는 능력치
	int iAtkPlus = 0;
	int iDefPlus = 0;
	int iHPPlus = 0;

	// 능력치를 글로발에서 읽어와서 보여준다
	const SPetTemplate& Pet = g_kGlobalValue.GetPetTemplateFromItemID( kPet.m_dwID, kPet.m_cPromotion );

	int iAtk = (int)( ( Pet.fAtkDefault + iPetLevel * Pet.fAtkConstant * Pet.fEvolutionConst + 0.0005f ) * 100 );
	int iDef = (int)( ( Pet.fDefDefault + iPetLevel * Pet.fDefConstant * Pet.fEvolutionConst + 0.0005f ) * 100 );
	int iHP = (int)( ( Pet.fHPDefault + iPetLevel * Pet.fHPConstant * Pet.fEvolutionConst + 0.0005f ) * 100 );


	std::wostringstream  strmAK , strmDF , strmVT;

	strmDesc << g_pkStrLoader->GetString( STR_ID_DESC_PLAYER_INFO7 ) << Pet_Exp_For_Next( kPet.m_dwEXP ) << std::endl;

	// 공격력 보여주기
	strmAK << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK )<<L" : ";
	if ( iAtkPlus < 0 )
	{
		strmAK << iAtk << '(' << iAtkPlus << ')' << std::endl;
	}
	else if( iAtkPlus == 0 )
	{
		strmAK << iAtk << std::endl;
	}
	else
	{
		strmAK << iAtk << "(+" << iAtkPlus << ')' << std::endl;
	}
	m_pkStaticAK->SetText(strmAK.str());

	// 방어력 보여주기
	strmDF << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE )<<L" : ";
	if ( iDefPlus < 0 )
	{
		strmDF << iDef << '(' << iDefPlus << ')' << std::endl;
	}
	else if( iDefPlus == 0 )
	{
		strmDF << iDef << std::endl;
	}
	else
	{
		strmDF << iDef << "(+" << iDefPlus << ')' << std::endl;
	}
	m_pkStaticDF->SetText(strmDF.str());

	// 생명력 보여주기
	strmVT << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC )<<L" : ";
	if ( iHPPlus < 0 )
	{
		strmVT << iHP << '(' << iHPPlus << ')' << std::endl;
	}
	else if( iHPPlus == 0 )
	{
		strmVT << iHP << std::endl;
	}
	else
	{
		strmVT << iHP << "(+" << iHPPlus << ')' << std::endl;
	}
	m_pkStaticVT->SetText(strmVT.str());

}
