#include "stdafx.h"
#include "GCGuildMemberBox.h"
//
//
#include "../MyD3D.h"
#include "../Procedure.h"

//
//


#include "KGCUIHelper.h"
#include "../GCRenderManager.h"

IMPLEMENT_CLASSNAME( KGCGuildMemberBox );
IMPLEMENT_WND_FACTORY( KGCGuildMemberBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMemberBox, "gc_guild_member_box" );

KGCGuildMemberBox* g_pkGuildMemberBox = NULL;

KGCGuildMemberBox::KGCGuildMemberBox( void )
{
    g_pkGuildMemberBox = this;

    m_ucCharType    = 0;

    m_pkID          = NULL;
    m_pkClass       = NULL;
    m_pkWinRate     = NULL;
    m_pkContPoint   = NULL;
    m_pkInfo        = NULL;

    for( int i = 0 ; i < (int)m_apkGrade.size() ; ++i )
    {
        m_apkGrade[i] = NULL;
    }

    LINK_CONTROL( "player_id_static",       m_pkID );
    LINK_CONTROL( "player_class_static",    m_pkClass );
    LINK_CONTROL( "player_win_rate_static", m_pkWinRate );
    LINK_CONTROL( "player_cont_static",     m_pkContPoint );
    LINK_CONTROL( "player_info",            m_pkInfo );

    LINK_CONTROL( "player_grade_vagabond",  m_apkGrade[0] );
    LINK_CONTROL( "player_grade_mercenary", m_apkGrade[1] );
    LINK_CONTROL( "player_grade_royal",     m_apkGrade[2] );
    LINK_CONTROL( "player_grade_holy",      m_apkGrade[3] );

    m_iExtraCharNum = 0;
}

KGCGuildMemberBox::~KGCGuildMemberBox( void )
{
    g_pkGuildMemberBox = NULL;
}

void KGCGuildMemberBox::OnCreate( void )
{        
    SetSelfInputCheck( true );

    m_pkID->SetSelfInputCheck( false );
    m_pkClass->SetSelfInputCheck( false );
    m_pkWinRate->SetSelfInputCheck( false );
    m_pkContPoint->SetSelfInputCheck( false );
    m_pkInfo->SetSelfInputCheck( false );
    m_pkContPoint->SetOffset( D3DXVECTOR2( -20.0f, 0.0f ) );

    for ( int i = 0; i < GC_GRADE_NUM; ++i )
    {
        m_apkGrade[i]->SetSelfInputCheck( false );
        m_apkGrade[i]->ToggleRender( false );
    }

    m_pkClass->SetText( L"--" );
    m_pkWinRate->SetText( L"--%" );
}

void KGCGuildMemberBox::OnActivate( void )
{
    if ( IsActive() )
    {		
        if ( g_vecUserInfo.size() != 0 ) 
		{
			// UserInfo로 KDetailUserInfo를 만들어서 전하자..
			SetGuildMemberInfo( g_kGlobalValue.m_kUserInfo );			
		}
    }
}

void KGCGuildMemberBox::FrameMoveInEnabledState( void )
{
    PLAYER* pPlayer = &g_MyD3D->m_TempPlayer;//[GUILD_MEMBER_INDEX];

    pPlayer->cFrame++;
	//pPlayer->AutoSetExtra();

	int Motion = pPlayer->GetCurFormTemplate().SELECT;

	if( g_bDetailUserInfoUpdate )
	{
		SetGuildMemberInfo( g_kDetailUserInfo );
		g_bDetailUserInfoUpdate = false;
		g_MyD3D->m_TempPlayer.cFrame = 0;
	}

	GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerSelectMotion((CHAR_IDENTY)static_cast<char>( pPlayer->Extra_Char_Num ) );
	if(pMotion == NULL) 
		return;
    if ( pPlayer->cFrame > (int)pMotion->GetNum_Frame() - 1 )
    {
        pPlayer->cFrame = (int)pMotion->GetNum_Frame() - 1;
    }
}

void KGCGuildMemberBox::PostDraw( void )
{
    if ( !g_MyD3D->m_TempPlayer.m_pObject )
    {
        g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
    }	

    if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum )
    {
        int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
        g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iSelectChar );
        g_MyD3D->m_TempPlayer.RemovePlayerObject();
        g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
        g_MyD3D->m_TempPlayer.cFrame = 0;
        m_iExtraCharNum = g_MyD3D->m_TempPlayer.Extra_Char_Num;
    }

    PLAYER* pPlayer = &g_MyD3D->m_TempPlayer;//[GUILD_MEMBER_INDEX];
	
    D3DXVECTOR2 vPos( GetFixedWindowPos() );

    float f_x = ( vPos.x / 400.0f ) - 1.0f;
    float f_y = ( -vPos.y / 300.0f * 0.75f ) + 0.75f;
    D3DXMATRIX matScale, matTrans, matWorld;

    // 길드 마크를 그린다.
    //g_MyD3D->m_KGCGuildMark.Render( g_kGlobalValue.m_kUserInfo.aiGuildMark[0], g_kGlobalValue.m_kUserInfo.aiGuildMark[1]
				//					, g_kGlobalValue.m_kUserInfo.aiGuildMark[2], f_x + 0.37f, f_y - 0.132f, 0.127f );

    float Scale = .26f;
    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerSelectMotion( (CHAR_IDENTY)static_cast<char>( pPlayer->Extra_Char_Num ) );

	g_pGCDeviceManager2->Flush();
	if(pMotion == NULL) return;

    D3DXMatrixScaling( &matScale ,Scale, Scale, Scale );  
    D3DXMatrixTranslation( &matTrans, pMotion->GetFrameData(pPlayer->cFrame).Plus_x * Scale + .103f + f_x 
        ,pMotion->GetFrameData(pPlayer->cFrame).Pos_y * Scale + -.237f + f_y, 50 );

    matWorld = matScale * matTrans;
    pPlayer->m_pObject->SetWorldMat( &matWorld );

    if ( pPlayer->cFrame > pMotion->GetNum_Frame() - 1 )
    {
        pPlayer->cFrame = pMotion->GetNum_Frame() - 1;
    }
	pPlayer->m_pObject->SetFrame( &pMotion->GetFrameData( pPlayer->cFrame ) );
    pPlayer->m_pObject->SetShellMatrixScale( 0.26f );
    g_RenderManager->Render( false , pPlayer->m_pObject );

}

void KGCGuildMemberBox::SetGuildMemberInfo( const SUserInfo& info )
{
	SUserInfo* pUserInfo = &g_kGlobalValue.m_kUserInfo;

	KDetailUserInfo MyInfo;
	MyInfo.m_bMale = pUserInfo->bMan;
	MyInfo.m_iWin  = pUserInfo->GetCurrentChar().iWin;
	MyInfo.m_cLiveState = 0;
	MyInfo.m_kGuildUserInfo.m_dwContributePoint = pUserInfo->iGuildScore;
	MyInfo.m_kGuildUserInfo.m_dwGuildUID = pUserInfo->iGuildID;
	MyInfo.m_kGuildUserInfo.m_strLogin =  pUserInfo->strLogin;
	MyInfo.m_kGuildUserInfo.m_cMemberLevel = 0;
	MyInfo.m_strLogin = pUserInfo->strLogin;
	MyInfo.m_strNickName = pUserInfo->strNickName;
    MyInfo.m_cKorGrade = pUserInfo->GetUserGrade();

	//MyBestCharacter
	MyInfo.m_cCurrentCharType   = (char)pUserInfo->GetCurrentChar().iCharType;
	MyInfo.m_biCurrentCharExp   = (DWORD)pUserInfo->GetCurrentChar().biExp;
	
    SCharInfo kCharInfo = pUserInfo->GetCurrentChar();
	for( int i = 0; i < (int)kCharInfo.vecItemInfo.size(); i++)
	{
        KEquipItemInfo kItem;
        kCharInfo.vecItemInfo[i].Copy( kItem );
		MyInfo.m_vecCurCharEquip.push_back( kItem );
	}
	
	//MyInfo.m_cBestCharType = g_kGlobalValue.m_kUserInfo.G
	SetGuildMemberInfo( MyInfo );
}

void KGCGuildMemberBox::SetGuildMemberInfo( const KDetailUserInfo& info )
{
    std::wostringstream strmID;
    strmID << g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str() << L" " << static_cast<int>( Exp_2_Level( info.m_biCurrentCharExp ) ) << L"  " << L"#c" + info.m_strNickColor + info.m_strNickName.c_str() + L"#cx";
    m_pkID->SetText( strmID.str() );

    std::wostringstream strmContPoint;
    strmContPoint << g_pkStrLoader->GetString( STR_ID_GUILD_CONT_POINT ) << info.m_kGuildUserInfo.m_dwContributePoint;
    m_pkContPoint->SetText( strmContPoint.str() );

    m_ucCharType = info.m_cCurrentCharType;
    
    g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( m_ucCharType );
	g_MyD3D->m_TempPlayer.AutoSetExtra();

    //SetCharacterGrade( (char)GetCharGrade( info.m_iWin ) );
    SetCharacterGrade( info.m_cKorGrade );

	g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo.clear();

	SUserItem ItemInfo;
    for ( int i = 0; i < (int)info.m_vecCurCharEquip.size(); i++ )
    {
        ItemInfo = info.m_vecCurCharEquip[i];
		ItemInfo.dwItemUID = 0;
		g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo.push_back( ItemInfo );
    }
	g_pItemMgr->EquipBaseItem( &g_MyD3D->m_TempPlayer.GetCurrentChar() );
    g_MyD3D->m_TempPlayer.RemovePlayerObject();
	g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);

    g_MyD3D->m_TempPlayer.cFrame = 0;
}

void KGCGuildMemberBox::SetCharacterGrade( char cGrade )
{
    g_pkUIHelper->SetCharacterGrade( m_apkGrade, cGrade );
}
