#include "stdafx.h"
#include "P2P.h"
#include "../uifx/KInput.h"
#include "GCDescBox.h"
#include "GCSizingThreeBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "KGCGPCapsuleDlg.h"
#include "GCItemInfoBox.h"
#include "KDSound.h"

IMPLEMENT_CLASSNAME( KGCGPCapsuleDlg );
IMPLEMENT_WND_FACTORY( KGCGPCapsuleDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGPCapsuleDlg, "gc_gpcapsule_viewer" );

KGCGPCapsuleDlg::KGCGPCapsuleDlg(void)
: m_iCharIndex( 0 )
, m_iCurSelectChar( 0 )
, m_iWeaponCapsuleNum( 0 )
, m_iArmorCapsuleNum( 0 )
, m_iRuneCapsuleNum( 0 )
, m_iAccCapsuleNum( 0 )
, m_iTotalCharNum( GC_CHAR_NUM )
{	
	m_pkClose					= NULL;
	m_pkWeapon					= NULL;
	m_pkArmor					= NULL;
	m_pkAccessory				= NULL;
	m_pkRune					= NULL;	
	m_pkGPCapsuleItemListDlg	= NULL;	
	m_pkGP						= NULL;

	LINK_CONTROL( "btn_close",		m_pkClose );
	LINK_CONTROL( "weapon_image",	m_pkWeapon );
	LINK_CONTROL( "armor_image",	m_pkArmor );
	LINK_CONTROL( "accessory_image",m_pkAccessory );
	LINK_CONTROL( "rune_image",		m_pkRune );
	LINK_CONTROL( "gpcapsule_itemlist", m_pkGPCapsuleItemListDlg);
	LINK_CONTROL( "static_gp_text", m_pkGP);

	// 아 미치겠다 걍 처음에 버튼이름을 통일했어야 하는건데 ㅡㅡ;;
	for( int i = 0 ; i < MAX_CAPSULE_NUM ; i++ )
	{
		m_pkWeaponCapsuleBtn[i] = NULL;
		std::stringstream str;
		str<<"btn_weapon_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkWeaponCapsuleBtn[i] );

		m_pkArmorCapsuleBtn[i] = NULL;
		str.str("");
		str<<"btn_armor_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkArmorCapsuleBtn[i] );

		m_pkAccCapsuleBtn[i] = NULL;
		str.str("");
		str<<"btn_accessory_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkAccCapsuleBtn[i] );

		m_pkRuneCapsuleBtn[i] = NULL;
		str.str("");
		str<<"btn_rune_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkRuneCapsuleBtn[i] );

		//무기 캡슐 레벨 및 GP 텍스트
		m_pkWeaponLvText[i] = NULL;
		str.str("");
		str<<"static_weapon_level"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkWeaponLvText[i] );

		m_pkWeaponGPText[i] = NULL;
		str.str("");
		str<<"static_weapon_price"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkWeaponGPText[i] );

		//방어구 캡슐 레벨 및 GP 텍스트
		m_pkArmorLvText[i] = NULL;
		str.str("");
		str<<"static_armor_level"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkArmorLvText[i] );

		m_pkArmorGPText[i] = NULL;
		str.str("");
		str<<"static_armor_price"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkArmorGPText[i] );

		//악세사리 캡슐 레벨 및 GP 텍스트
		m_pkAccLvText[i] = NULL;
		str.str("");
		str<<"static_accessory_level"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkAccLvText[i] );

		m_pkAccGPText[i] = NULL;
		str.str("");
		str<<"static_accessory_price"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkAccGPText[i] );

		//룬 캡슐 레벨 및 GP 텍스트
		m_pkRuneLvText[i] = NULL;
		str.str("");
		str<<"static_rune_level"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkRuneLvText[i] );

		m_pkRuneGPText[i] = NULL;
		str.str("");
		str<<"static_rune_price"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkRuneGPText[i] );

		m_pkSelectWeaponCapsule[i] = NULL;
		str.str("");
		str<<"select_weapon_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkSelectWeaponCapsule[i] );

		m_pkSelectArmorCapsule[i] = NULL;
		str.str("");
		str<<"select_armor_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkSelectArmorCapsule[i] );

		m_pkSelectAccCapsule[i] = NULL;
		str.str("");
		str<<"select_accessory_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkSelectAccCapsule[i] );

		m_pkSelectRuneCapsule[i] = NULL;
		str.str("");
		str<<"select_rune_capsule"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkSelectRuneCapsule[i] );
	}

	for( int i = 0 ; i < MAX_VERTICAL_LINE ; i++ )
	{
		m_pkLine[i] = NULL;
		std::stringstream str;
		str<<"line"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkLine[i] );
	}

	for( int i = 0 ; i < m_iTotalCharNum ; ++i )
	{
		m_apkChar[i] = NULL;
		std::stringstream str;
		str<<"btn_character"<<i;
		LINK_CONTROL( str.str().c_str(), m_apkChar[i] );

		m_pkCharBtnOutline[i] = NULL;
		str.str("");
		str<<"select_character"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkCharBtnOutline[i] );
	}

	m_apkChar[m_iTotalCharNum] = NULL;
}

KGCGPCapsuleDlg::~KGCGPCapsuleDlg(void)
{
	m_mapCapsuleInfo.clear();
}

void KGCGPCapsuleDlg::OnCreate( void )
{
	m_pkGPCapsuleItemListDlg->InitState(false, true, this);
	m_pkClose->InitState( true, true, this );
	m_pkClose->SetHotKey( DIK_ESCAPE );

	m_pkGP->InitState( true, true );
	m_pkGP->SetAlign( DT_RIGHT );
	m_pkGP->SetText( g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP());

	for( int i = 0; i < MAX_CAPSULE_NUM ; i++ )
	{
		m_pkWeaponCapsuleBtn[i]->InitState( true, true, this );
		m_pkArmorCapsuleBtn[i]->InitState( true, true, this );
		m_pkAccCapsuleBtn[i]->InitState( true, true, this );
		m_pkRuneCapsuleBtn[i]->InitState( true, true, this );

		m_pkWeaponLvText[i]->InitState( true, true );
		m_pkWeaponLvText[i]->SetAlign( DT_CENTER );
		m_pkWeaponLvText[i]->SetShadow(true);	

		m_pkWeaponGPText[i]->InitState( true, true );
		m_pkWeaponGPText[i]->SetAlign( DT_RIGHT );

		m_pkArmorLvText[i]->InitState( true, true );
		m_pkArmorLvText[i]->SetShadow(true);
		m_pkArmorLvText[i]->SetAlign( DT_CENTER );

		m_pkArmorGPText[i]->InitState( true, true );
		m_pkArmorGPText[i]->SetAlign( DT_RIGHT );

		m_pkAccLvText[i]->InitState( true, true );		
		m_pkAccLvText[i]->SetShadow(true);
		m_pkAccLvText[i]->SetAlign( DT_CENTER );

		m_pkAccGPText[i]->InitState( true, true );
		m_pkAccGPText[i]->SetAlign( DT_RIGHT );

		m_pkRuneLvText[i]->InitState( true, true );
		m_pkRuneLvText[i]->SetShadow(true);
		m_pkRuneLvText[i]->SetAlign( DT_CENTER );
		
		m_pkRuneGPText[i]->InitState( true, true );
		m_pkRuneGPText[i]->SetAlign( DT_RIGHT );

		m_pkSelectWeaponCapsule[i]->InitState(false);
		m_pkSelectArmorCapsule[i]->InitState(false);
		m_pkSelectAccCapsule[i]->InitState(false);
		m_pkSelectRuneCapsule[i]->InitState(false);
	}

	for( int i = 0; i < MAX_VERTICAL_LINE; i++ )
	{
		m_pkLine[i]->InitState( true );
	}

	for( int i = 0 ; i < CURRENT_CHAR_NUM ; ++i )
	{
		m_apkChar[i]->InitState( true, true, this );
		m_pkCharBtnOutline[i]->InitState( false, true, this );
	}

	m_pkCharBtnOutline[0]->ToggleRender(true);

	//초기 캡슐 정보는 엘리시스것으로 뿌려준다.
	DrawCapsuleInfo(GC_CHAR_ELESIS);

}

void KGCGPCapsuleDlg::FrameMoveInEnabledState( void )
{
	if( m_pkGPCapsuleItemListDlg->m_iParticleTime != 0 )
		return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	for( int i = 0 ; i < MAX_CAPSULE_NUM ; i++ )
	{
		if( m_pkWeaponCapsuleBtn[i]->CheckPosInWindowBound(vMousePos) )
		{
			if( m_iWeaponCapsuleNum -1 < i )
			{
				m_pkWeaponCapsuleBtn[i]->SetToolTip(g_pkStrLoader->GetString(STR_ID_EMPTY_GP_CAPSULE));
				continue;
			}
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );

				for( int j = 0 ; j < MAX_CAPSULE_NUM ; j++ )
				{
					m_pkSelectWeaponCapsule[j]->ToggleRender(false);
					m_pkSelectArmorCapsule[j]->ToggleRender(false);
					m_pkSelectAccCapsule[j]->ToggleRender(false);
					m_pkSelectRuneCapsule[j]->ToggleRender(false);
				}
				m_pkSelectWeaponCapsule[i]->ToggleRender(true);

				m_pkWeaponCapsuleBtn[i]->SetWndMode( D3DWM_DEFAULT );
				OnClickCapsule( m_iCharIndex, (int)EIK_WEAPON, i, true );
				break;
			}
			else if( g_pkInput->BtnPressed( KInput::MBLEFT ) || g_pkInput->BtnDown( KInput::MBLEFT ) )
			{
				m_pkWeaponCapsuleBtn[i]->SetWndMode( D3DWM_SELECT );
			}
		}
		else if( m_pkArmorCapsuleBtn[i]->CheckPosInWindowBound(vMousePos) )
		{
			if( m_iArmorCapsuleNum -1 < i )
			{
				m_pkArmorCapsuleBtn[i]->SetToolTip(g_pkStrLoader->GetString(STR_ID_EMPTY_GP_CAPSULE));
				continue;
			}
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );
				for( int j = 0 ; j < MAX_CAPSULE_NUM ; j++ )
				{
					m_pkSelectWeaponCapsule[j]->ToggleRender(false);
					m_pkSelectArmorCapsule[j]->ToggleRender(false);
					m_pkSelectAccCapsule[j]->ToggleRender(false);
					m_pkSelectRuneCapsule[j]->ToggleRender(false);
				}
				m_pkSelectArmorCapsule[i]->ToggleRender(true);

				m_pkArmorCapsuleBtn[i]->SetWndMode( D3DWM_DEFAULT );
				OnClickCapsule( m_iCharIndex, (int)EIK_ARMOR, i, true );
				break;
			}
			else if( g_pkInput->BtnPressed( KInput::MBLEFT ) || g_pkInput->BtnDown( KInput::MBLEFT ) )
			{
				m_pkArmorCapsuleBtn[i]->SetWndMode( D3DWM_SELECT );
			}			
		}
		else if( m_pkAccCapsuleBtn[i]->CheckPosInWindowBound(vMousePos) )
		{
			if( m_iAccCapsuleNum -1 < i )
			{
				m_pkAccCapsuleBtn[i]->SetToolTip(g_pkStrLoader->GetString(STR_ID_EMPTY_GP_CAPSULE));
				continue;
			}
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );
				for( int j = 0 ; j < MAX_CAPSULE_NUM ; j++ )
				{					
					m_pkSelectWeaponCapsule[j]->ToggleRender(false);
					m_pkSelectArmorCapsule[j]->ToggleRender(false);
					m_pkSelectAccCapsule[j]->ToggleRender(false);
					m_pkSelectRuneCapsule[j]->ToggleRender(false);
				}
				m_pkSelectAccCapsule[i]->ToggleRender(true);

				m_pkAccCapsuleBtn[i]->SetWndMode( D3DWM_DEFAULT );
				OnClickCapsule( m_iCharIndex, (int)EIK_ACCESSORIES, i, true );
				break;
			}
			else if( g_pkInput->BtnPressed( KInput::MBLEFT ) || g_pkInput->BtnDown( KInput::MBLEFT ) )
			{
				m_pkAccCapsuleBtn[i]->SetWndMode( D3DWM_SELECT );
			}						
		}
		else if( m_pkRuneCapsuleBtn[i]->CheckPosInWindowBound(vMousePos) )
		{
			if( m_iRuneCapsuleNum -1 < i )
			{
				m_pkRuneCapsuleBtn[i]->SetToolTip(g_pkStrLoader->GetString(STR_ID_EMPTY_GP_CAPSULE));
				continue;
			}
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );
				for( int j = 0 ; j < MAX_CAPSULE_NUM ; j++ )
				{					
					m_pkSelectWeaponCapsule[j]->ToggleRender(false);
					m_pkSelectArmorCapsule[j]->ToggleRender(false);
					m_pkSelectAccCapsule[j]->ToggleRender(false);
					m_pkSelectRuneCapsule[j]->ToggleRender(false);
				}
				m_pkSelectRuneCapsule[i]->ToggleRender(true);

				m_pkRuneCapsuleBtn[i]->SetWndMode( D3DWM_DEFAULT );
				OnClickCapsule( m_iCharIndex, (int)EIK_RUNE, i, true );
				break;
			}
			else if( g_pkInput->BtnPressed( KInput::MBLEFT ) || g_pkInput->BtnDown( KInput::MBLEFT ) )
			{
				m_pkRuneCapsuleBtn[i]->SetWndMode( D3DWM_SELECT );
			}									
		}
		else
			continue;
	}
}

void KGCGPCapsuleDlg::ActionPerformed( const KActionEvent& event )
{
	if( m_pkGPCapsuleItemListDlg->m_iParticleTime != 0 )
		return;

	if( event.m_pWnd == m_pkClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "GPC_BUTTON_CLICK" );
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
		this->SetWindowPos(D3DXVECTOR2(150.0f, 60.0f));
		for( int i = 0 ; i < MAX_CAPSULE_NUM ; i++ )
		{
			m_pkSelectWeaponCapsule[i]->ToggleRender(false);
			m_pkSelectArmorCapsule[i]->ToggleRender(false);
			m_pkSelectAccCapsule[i]->ToggleRender(false);
			m_pkSelectRuneCapsule[i]->ToggleRender(false);
		}

		for( int i = 0 ; i < m_iTotalCharNum ; i++ )
		{
			m_pkCharBtnOutline[i]->ToggleRender(false);
		}
		m_pkCharBtnOutline[0]->ToggleRender(true);
		m_pkGPCapsuleItemListDlg->ToggleRender( false );	
		m_iCurSelectChar = 0;
	}

	for( int i = 0 ; i < m_iTotalCharNum ; ++i )
	{
		if( event.m_pWnd == m_apkChar[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		{
			g_KDSound.Play( "GPC_BUTTON_CLICK" );
			if( m_iCurSelectChar == i )
				continue;

			for( int j = 0 ; j < m_iTotalCharNum ; j++ )
				m_pkCharBtnOutline[j]->ToggleRender(false);
			m_pkCharBtnOutline[i]->ToggleRender(true);
			
			for( int j = 0 ; j < MAX_CAPSULE_NUM ; j++ )
			{
				m_pkSelectWeaponCapsule[j]->ToggleRender(false);
				m_pkSelectArmorCapsule[j]->ToggleRender(false);
				m_pkSelectAccCapsule[j]->ToggleRender(false);
				m_pkSelectRuneCapsule[j]->ToggleRender(false);
			}
			m_pkSelectWeaponCapsule[0]->ToggleRender(true);

			if( m_pkGPCapsuleItemListDlg->IsRenderOn() )
				OnClickCapsule( i, 0, 0, true );
			else
				OnClickCapsule( i, 0, 0, false );
			DrawCapsuleInfo(i);

			m_iCurSelectChar = i;
			break;
		}
	}
}

void KGCGPCapsuleDlg::OnCreateComplete()
{
	//초기 캡슐 정보는 엘리시스것으로 뿌려준다.
	DrawCapsuleInfo(GC_CHAR_ELESIS);
	OnClickCapsule( m_iCharIndex, (int)EIK_WEAPON, 0, true );   
}

void KGCGPCapsuleDlg::OnDestroyComplete()
{

}


void KGCGPCapsuleDlg::PostChildDraw()
{

}

void KGCGPCapsuleDlg::SetCapsuleInfo( std::map< std::pair<int,int>, std::vector<KCapsule>>& mapCapsuleInfo_ )
{
#if defined( GP_CAPSULE )
	m_mapCapsuleInfo.clear();
	m_mapCapsuleInfo = mapCapsuleInfo_;
#endif
}

void KGCGPCapsuleDlg::DrawCapsuleInfo( int iCharIndex_ )
{
	if( m_mapCapsuleInfo.empty() )
		return;

	m_iCharIndex = iCharIndex_;

	int iMinLv = 0;
	int iMaxLv = 0;
	WCHAR strText[20];

	std::map< std::pair<int,int>, std::vector<KCapsule> >::iterator itorWeapon = m_mapCapsuleInfo.find(std::pair<int,int>(iCharIndex_, (int)EIK_WEAPON)); 
	std::map< std::pair<int,int>, std::vector<KCapsule> >::iterator itorArmor = m_mapCapsuleInfo.find(std::pair<int,int>( iCharIndex_, (int)EIK_ARMOR)); 
	std::map< std::pair<int,int>, std::vector<KCapsule> >::iterator itorAcc = m_mapCapsuleInfo.find(std::pair<int,int>( iCharIndex_, (int)EIK_ACCESSORIES)); 
	std::map< std::pair<int,int>, std::vector<KCapsule> >::iterator itorRune = m_mapCapsuleInfo.find(std::pair<int,int>( iCharIndex_, (int)EIK_RUNE)); 

	m_iWeaponCapsuleNum = (int)itorWeapon->second.size();
	m_iArmorCapsuleNum = (int)itorArmor->second.size();
	m_iRuneCapsuleNum = (int)itorRune->second.size();
	m_iAccCapsuleNum = (int)itorAcc->second.size();


	if( itorWeapon == m_mapCapsuleInfo.end() || itorArmor == m_mapCapsuleInfo.end() || itorArmor == m_mapCapsuleInfo.end() || itorRune == m_mapCapsuleInfo.end() )
		return;


	for( int i = 0 ; i < (int)itorWeapon->second.size() ; i++ )
	{
		/////////////////////////////////
		// 무기 캡슐 레벨대 및 GP 정보				
		iMinLv = itorWeapon->second[i].m_prLevelRange.first;
		iMaxLv = itorWeapon->second[i].m_prLevelRange.second;		
		swprintf(strText, 20, L"%d~%d",iMinLv,iMaxLv);
		m_pkWeaponLvText[i]->SetText( strText );

		swprintf(strText, 20, L"%dGP",itorWeapon->second[i].m_nPrice );
		m_pkWeaponGPText[i]->SetText( strText );
	}

	for( int i = 0 ; i < (int)itorArmor->second.size() ; i++ )
	{
		/////////////////////////////////
		// 방어구 캡슐 레벨대 및 GP 정보		
		iMinLv = itorArmor->second[i].m_prLevelRange.first;
		iMaxLv = itorArmor->second[i].m_prLevelRange.second;		
		swprintf(strText, 20, L"%d~%d",iMinLv,iMaxLv);		
		m_pkArmorLvText[i]->SetText( strText );

		swprintf(strText, 20, L"%dGP",itorArmor->second[i].m_nPrice);		
		m_pkArmorGPText[i]->SetText( strText );
	}

	for( int i = 0 ; i < (int)itorAcc->second.size() ; i++ )
	{
		//////////////////////////////////
		// 악세사리 캡슐 레벨대 및 GP 정보				
		iMinLv = itorAcc->second[i].m_prLevelRange.first;
		iMaxLv = itorAcc->second[i].m_prLevelRange.second;		
		swprintf(strText, 20, L"%d~%d",iMinLv,iMaxLv);		
		m_pkAccLvText[i]->SetText( strText );
		
		swprintf(strText, 20, L"%dGP",itorAcc->second[i].m_nPrice);		
		m_pkAccGPText[i]->SetText( strText );
	}

	for( int i = 0 ; i < (int)itorRune->second.size() ; i++ )
	{
		/////////////////////////////////
		// 룬 캡슐 레벨대 및 GP 정보				
		iMinLv = itorRune->second[i].m_prLevelRange.first;
		iMaxLv = itorRune->second[i].m_prLevelRange.second;		
		swprintf(strText, 20, L"%d~%d",iMinLv,iMaxLv);		
		m_pkRuneLvText[i]->SetText( strText );
		
		swprintf(strText, 20, L"%dGP",itorRune->second[i].m_nPrice);		
		m_pkRuneGPText[i]->SetText( strText );
	}	
}

void KGCGPCapsuleDlg::OnClickCapsule( int iCharIndex_, int iItemKind_, int iCapsuleIndex_, bool bRender_ )
{
	std::map< std::pair<int,int>, std::vector<KCapsule> >::iterator itor = m_mapCapsuleInfo.find(std::pair<int,int>(iCharIndex_, iItemKind_)); 
	if( itor != m_mapCapsuleInfo.end() )
	{
		m_pkGPCapsuleItemListDlg->SetCapsuleItemList( itor->second[iCapsuleIndex_].m_vecItemList, iCharIndex_, iItemKind_, 
			iCapsuleIndex_, itor->second[iCapsuleIndex_].m_nPrice );
	}	

	if( !m_pkGPCapsuleItemListDlg->IsRenderOn() && bRender_ )
		this->SetWindowPos( D3DXVECTOR2(-10.0f, 60.0f));

	m_pkGPCapsuleItemListDlg->MovePage( 1 );
	m_pkGPCapsuleItemListDlg->ToggleRender( bRender_ );	
}

void KGCGPCapsuleDlg::UpdateGP()
{
	m_pkGP->SetText( g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP());
}

void KGCGPCapsuleDlg::SetParticleTime( int iParticleTime_ )
{ 
	m_pkGPCapsuleItemListDlg->m_iParticleTime = iParticleTime_;
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_03", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_02", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_1);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_01", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_2);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_05", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_06", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_07", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_08", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_04", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Capsule_Open_09", 0.0f, 0.05f, 0.5f ) , GC_LAYER_UPPER_SQUARE_UI_3);
}

void KGCGPCapsuleDlg::SetCapsuleBtnToolTipOff( bool bOff_ )
{
	for( int i = 0 ; i < MAX_CAPSULE_NUM ; i++ )
	{
		m_pkWeaponCapsuleBtn[i]->RenderToolTip( bOff_ );
		m_pkArmorCapsuleBtn[i]->RenderToolTip( bOff_ );
		m_pkAccCapsuleBtn[i]->RenderToolTip( bOff_ );
		m_pkRuneCapsuleBtn[i]->RenderToolTip( bOff_ );
	}
}

void KGCGPCapsuleDlg::DrawObtainedItem( int iItemGrade_, DWORD dwItemID )
{
	
}
