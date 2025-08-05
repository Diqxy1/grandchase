#include "stdafx.h"
#include "KGCMonsterCrusaderPanel.h"
#include "KGCRoomManager.h"
//
#include "GCChangeBox.h"
//
#include "MyD3D.h"
#include "KGCUIHelper.h"


#ifdef TEXTURE_WIDTH
#undef TEXTURE_WIDTH
#endif
#define TEXTURE_WIDTH       (256.0f)

#ifdef TEXTURE_HEIGHT
#undef TEXTURE_HEIGHT
#endif
#define TEXTURE_HEIGHT      (256.0f)

IMPLEMENT_CLASSNAME( KGCMonsterCrusaderPanel );
IMPLEMENT_WND_FACTORY( KGCMonsterCrusaderPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCrusaderPanel, "gc_monster_crusader_panel" );

KGCMonsterCrusaderPanel::KGCMonsterCrusaderPanel(void)
{
    m_pkMonsterChangeBox = NULL;

    LINK_CONTROL( "monster_change_box", m_pkMonsterChangeBox );

    m_iSelectMonsterIndex = 0;
    m_iListSize = 0;

    m_pMonsterTexture = NULL;
}

KGCMonsterCrusaderPanel::~KGCMonsterCrusaderPanel(void)
{
    SAFE_RELEASE( m_pMonsterTexture );
}

void KGCMonsterCrusaderPanel::Destroy()
{
    SAFE_RELEASE( m_pMonsterTexture );
}

void KGCMonsterCrusaderPanel::OnCreate( void )
{
    m_pkMonsterChangeBox->InitState( true, true, this );
	m_iListSize = (int)g_kGlobalValue.m_mapMonsterCrusaderInfo.size();
}

void KGCMonsterCrusaderPanel::ActionPerformed( const KActionEvent& event )
{
    if( g_kGlobalValue.m_kUserInfo.bHost ||
        g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) // 방장인 경우만 조작 가능
    {
#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
        if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
        {
            // 방장인 내가 아닌 옵저버가 있다면 방장 기능을 상실한다.
            if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
                return;
        }
#endif

        if( event.m_pWnd == m_pkMonsterChangeBox )
        {
            if( g_pkUIHelper->GetUILockTime( LOCK_ROOM ) == 0 )
            {
                switch ( event.m_dwCode )
                {
                    case KGCChangeBox::RIGHT_BUTTON: // 오른쪽 버튼을 클릭했을 때
                    {
                        ChangeMonster( true );
                        g_KDSound.Play( "31" );
                        break;
                    }
                    case KGCChangeBox::LEFT_BUTTON: // 왼쪽 버튼을 클릭했을 때
                    {
                        ChangeMonster( false );
                        g_KDSound.Play( "31" );
                        break;
                    }
                }
            }
        }
    }
}

void KGCMonsterCrusaderPanel::PostChildDraw( void )
{

    if( m_pMonsterTexture )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        
		g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
			g_pGCDeviceManager2->DrawInScreen(m_pMonsterTexture,
				vPos.x, vPos.y,
				vPos.x + TEXTURE_WIDTH * m_fWindowScaleX, vPos.y + TEXTURE_HEIGHT * m_fWindowScaleY,
				m_vTextureCoord.w, m_vTextureCoord.x, m_vTextureCoord.y, m_vTextureCoord.z,false,10.0f,0xffffffff);
		}
		g_pGCDeviceManager2->PopState();

    }

    //std::wstringstream strmInfo;
    //GCEXPTYPE biExp = GetExpFromLevel( g_MyD3D->GetMyPlayer()->GetCurrentChar().iLevel );
    //int iGP = GetGPFromLevel( g_MyD3D->GetMyPlayer()->GetCurrentChar().iLevel );

    //GCEXPTYPE biGetExp = (GCEXPTYPE)( biExp * g_kGlobalValue.m_mapMonsterCrusaderInfo[m_iSelectMonsterIndex].fExpRatio );
    //int iGetGP = (int)( iGP * g_kGlobalValue.m_mapMonsterCrusaderInfo[m_iSelectMonsterIndex].fGPRatio );

    //if( biGetExp < 1 ) biGetExp = 1;
    //if( iGetGP < 1 ) iGetGP = 1;

    //strmInfo << g_pkStrLoader->GetString( STR_ID_MAP_BOX_TEXT1 ) << L" " << biGetExp;
    //strmInfo << std::endl;
    //strmInfo << g_pkStrLoader->GetString( STR_ID_MAP_BOX_TEXT2 ) << L" " << iGetGP;

    //g_pkFontMgrOutline->OutTextMultiline( (int)( 563 * m_fWindowScaleX ), (int)( 249 * m_fWindowScaleY), strmInfo.str().c_str(), 0xFFFFFFFF, 1.2f * m_fWindowScaleY, NULL, DT_LEFT | DT_TOP, 0xFF000000 );
}

void KGCMonsterCrusaderPanel::UpdateGameData()
{
    for( int i = 0; i < m_iListSize; ++i )
    {
        if( g_kGlobalValue.m_mapMonsterCrusaderInfo[i].iMonsterID == SiKGCRoomManager()->GetRoomInfo().iMonsterID )
        {
            m_iSelectMonsterIndex = i;
            break;
        }
    }

    // 보스 몬스터 텍스쳐를 로드한다!
    int iTexIndex = g_kGlobalValue.m_mapMonsterCrusaderInfo[m_iSelectMonsterIndex].iTextureIndex;
    SAFE_RELEASE( m_pMonsterTexture );
    char strTextureName[MAX_PATH];
    sprintf( strTextureName, "MonsterCrusader%d.dds", (int)( iTexIndex / 4.0f + 1 ) );

    m_pMonsterTexture = g_pGCDeviceManager2->CreateTexture( strTextureName );

    m_vTextureCoord.w = 0.0f + ( iTexIndex % 2 ) * 0.5f;
    m_vTextureCoord.x = 0.0f + (int)( ( iTexIndex % 4 ) / 2 ) * 0.5f;
    m_vTextureCoord.y = 0.5f + ( iTexIndex % 2 ) * 0.5f;
    m_vTextureCoord.z = 0.5f + (int)( ( iTexIndex % 4 ) / 2 ) * 0.5f;

    // 방장은 몬스터 변경 가능
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
        m_pkMonsterChangeBox->EnableChangeBox( g_MyD3D->IsLocalPlayerObserver() );
    else
        m_pkMonsterChangeBox->EnableChangeBox( g_kGlobalValue.m_kUserInfo.bHost );

    // 몬스터 이름 표시
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ g_kGlobalValue.m_mapMonsterCrusaderInfo[m_iSelectMonsterIndex].iMonsterID ];
    m_pkMonsterChangeBox->SetText( g_pkStrLoader->GetString( kMonsterTemplate.iNameID ) );
}

void KGCMonsterCrusaderPanel::ChangeMonster( bool bRight )
{
	m_iListSize = (int)g_kGlobalValue.m_mapMonsterCrusaderInfo.size();

    if( bRight )
        m_iSelectMonsterIndex = ( m_iSelectMonsterIndex + 1 ) % m_iListSize;
    else
        m_iSelectMonsterIndex = ( m_iSelectMonsterIndex - 1 + m_iListSize ) % m_iListSize;

    SiKGCRoomManager()->GetRoomInfo().iMonsterID = g_kGlobalValue.m_mapMonsterCrusaderInfo[m_iSelectMonsterIndex].iMonsterID;
    SiKGCRoomManager()->GetRoomInfo().iMonsterCount = 1;

    SiKGCRoomManager()->SendRoomInfo();
    UpdateGameData();
    g_pkUIScene->RoomButtonSetStartDelay( 219 );

    g_pkUIHelper->SetUILockTime( LOCK_ROOM, BUTTON_LOCK_FRAME );
}

void KGCMonsterCrusaderPanel::FrameMoveInEnabledState()
{
}

