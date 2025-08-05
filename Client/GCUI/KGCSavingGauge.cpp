#include "StdAfx.h"
#include "KGCSavingGauge.h"
#include "KncP2PLib/KncCommonDefine.h"

IMPLEMENT_CLASSNAME( KGCSavingGauge );
IMPLEMENT_WND_FACTORY( KGCSavingGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCSavingGauge, "gc_saving_gauge" );

KGCSavingGauge::KGCSavingGauge(void) :
m_vCooltimeGaugeCoords( 0.0f, 0.0f, 0.0f, 0.0f ),
m_vRealGaugeCoords( 0.0f, 0.0f, 0.0f, 0.0f )
{   
    m_bUse = false;
    m_bFullState = true;
    m_bSameStateGauge = true;
    m_fReserveRealAmount = 1.f;
    m_fReserveCooltimeAmount = 1.f;
    m_kCurCharSavingInfo.Init();

    m_pkCooltimeGauge = NULL;
    m_pkRealGauge = NULL;
    m_pkEmptyGauge = NULL;
    //m_pkEmptyCooltimeWatch = NULL;

    LINK_CONTROL( "cooltime_gauge ",	        m_pkCooltimeGauge );
    LINK_CONTROL( "real_gauge",			        m_pkRealGauge );
    //LINK_CONTROL( "empty_cooltime_watch",		m_pkEmptyCooltimeWatch );
    LINK_CONTROL( "empty_gauge",			    m_pkEmptyGauge );

    LoadCharSavingInfo();
}

KGCSavingGauge::~KGCSavingGauge(void)
{
}

void KGCSavingGauge::OnCreate( void )
{
    m_bFullState = true;    
    m_bSameStateGauge = true;
    m_fReserveRealAmount = 1.f;
    m_fReserveCooltimeAmount = 1.f;
    m_kCurCharSavingInfo.Init();
    
    InitGaugeTexCoords();

    m_pkCooltimeGauge->InitState( true, false );
    m_pkRealGauge->InitState( true, false );
    //m_pkEmptyCooltimeWatch->InitState( false, false );
    m_pkEmptyGauge->InitState( false, false );

    InitCurCharSavingInfo();
}

void KGCSavingGauge::FrameMoveInEnabledState( void )
{   
    if( m_bUse == false ) {
        return;
    }

    if( m_bFullState == false ) {
        AutoRecovery();

        if( m_pkRealGauge->GetWidth() == MAX_LEN_SAVING_GAUGE ) {
            m_bFullState = true;
            m_bSameStateGauge = true;
            m_fReserveRealAmount = 1.f;
            m_fReserveCooltimeAmount = 1.f;
        }        
    } 

    if( m_fReserveRealAmount <= 0.f && m_fReserveCooltimeAmount <= 0.f ) {
        if ( o10 < 10 ) {
            m_pkEmptyGauge->ToggleRender( true );
        } else {
            m_pkEmptyGauge->ToggleRender( false );
        }
    } else {
        m_pkEmptyGauge->ToggleRender( false );
    }
}

void KGCSavingGauge::SetGauge( float fAmount_ )
{    
    m_fReserveRealAmount -= fAmount_;
    if( m_fReserveRealAmount > 1.f ) {
        m_fReserveRealAmount = 1.f;
    } else if( m_fReserveRealAmount < 0.f ) {
        m_fReserveRealAmount = 0.f;
    }

    m_kStopWatch.SetStopWatch( static_cast< DWORD >( m_kCurCharSavingInfo.fRecoveryWaitTime ) );

    float fTU2;
    fTU2 = m_vRealGaugeCoords.x + m_fReserveRealAmount * ( m_vRealGaugeCoords.z - m_vRealGaugeCoords.x );
    fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

    m_pkRealGauge->SetWndTexCoords( D3DXVECTOR4( m_vRealGaugeCoords.x,
        m_vRealGaugeCoords.y, fTU2,
        m_vRealGaugeCoords.w ) );
    m_pkRealGauge->SetWidth( static_cast<int>( m_fReserveRealAmount * MAX_LEN_SAVING_GAUGE ) );    

    if( m_bFullState == false && m_bSameStateGauge == false ) {
        m_fReserveCooltimeAmount -= fAmount_;
        if( m_fReserveCooltimeAmount > 1.f ) {
            m_fReserveCooltimeAmount = 1.f;
        } else if( m_fReserveCooltimeAmount < 0.f ) {
            m_fReserveCooltimeAmount = 0.f;
        }

        // 리얼게이지를 모두 소모하게 되면 쿨타임게이지까지 모두 소모되어 회복 패널티가 바로 적용된다.
        if( m_fReserveRealAmount == 0.f ) {
            m_fReserveCooltimeAmount = 0.f;
            // 게이지 모두 소진 시 패널티 쿨타임이 적용된다.
            m_kStopWatch.SetStopWatch( static_cast< DWORD >( m_kCurCharSavingInfo.fPenaltyEmptyCooltime ) );

            // 게이지 모두 소진 시 캐릭터 카툰효과 - 우선 제거 해둠
            //if( g_MyD3D->GetMyPlayer() ) {
            //    g_MyD3D->GetMyPlayer()->SetMagicEffect( EGC_EFFECT_SAVING_EMPTY_STATE, m_kCurCharSavingInfo.fPenaltyEmptyCooltime / 1000.f );
            //}            
        }

        fTU2 = m_vCooltimeGaugeCoords.x + m_fReserveCooltimeAmount * ( m_vCooltimeGaugeCoords.z - m_vCooltimeGaugeCoords.x );
        fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

        m_pkCooltimeGauge->SetWndTexCoords( D3DXVECTOR4( m_vCooltimeGaugeCoords.x,
            m_vCooltimeGaugeCoords.y, fTU2,
            m_vCooltimeGaugeCoords.w ) );
        m_pkCooltimeGauge->SetWidth( static_cast<int>( m_fReserveCooltimeAmount * MAX_LEN_SAVING_GAUGE ) );
    }

    if( m_bFullState ) {
        m_bFullState = false;
    }

    if( m_bSameStateGauge ) {
        m_bSameStateGauge = false;
    }
}

void KGCSavingGauge::AutoRecovery()
{
    if( m_bFullState == false && m_kStopWatch.IsStop() ) {
        //m_pkEmptyCooltimeWatch->ToggleRender( false );
        float fTU2;
        // 실제게이지가 쿨타임 게이지보다 커지면 회복속도 늦게하고 회복도 시키자.
        if( m_pkRealGauge->GetWidth() >= m_pkCooltimeGauge->GetWidth() ) {
            m_bSameStateGauge = true;
            m_fReserveRealAmount += ( 1.f / ( m_kCurCharSavingInfo.fRecoveryTotalTime * GC_FPS_LIMIT ) ) * m_kCurCharSavingInfo.fPenaltyRecoveryRate;

            fTU2 = m_vCooltimeGaugeCoords.x + m_fReserveRealAmount * ( m_vCooltimeGaugeCoords.z - m_vCooltimeGaugeCoords.x );
            fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

            m_pkCooltimeGauge->SetWndTexCoords( D3DXVECTOR4( m_vCooltimeGaugeCoords.x,
                m_vCooltimeGaugeCoords.y, fTU2,
                m_vCooltimeGaugeCoords.w ) );
            m_pkCooltimeGauge->SetWidth( static_cast<int>( m_fReserveRealAmount * MAX_LEN_SAVING_GAUGE ) );

            m_fReserveCooltimeAmount = m_fReserveRealAmount;
        } else {
            m_fReserveRealAmount += 1.f / ( m_kCurCharSavingInfo.fRecoveryTotalTime * GC_FPS_LIMIT );
        }

        // 최대치 넘기지 않도록 설정
        if( m_fReserveRealAmount > 1.f ) {
            m_fReserveRealAmount = 1.f;
        }

        fTU2 = m_vRealGaugeCoords.x + m_fReserveRealAmount * ( m_vRealGaugeCoords.z - m_vRealGaugeCoords.x );
        fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

        m_pkRealGauge->SetWndTexCoords( D3DXVECTOR4( m_vRealGaugeCoords.x,
            m_vRealGaugeCoords.y, fTU2,
            m_vRealGaugeCoords.w ) );
        m_pkRealGauge->SetWidth( static_cast<int>( m_fReserveRealAmount * MAX_LEN_SAVING_GAUGE ) );
        
    }
}

void KGCSavingGauge::LoadCharSavingInfo()
{
    m_mapCharSavingInfo.clear();
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "CharSavingInfo.stg") == false)
        return;

    if ( luaMgr.BeginTable( "CharSavingInfo") == S_OK )
    {
        for( int i = 1; luaMgr.BeginTable( i ) == S_OK; ++i )
        {
            KCharSavingInfo kSavingInfo;
            int iCharID = CID_COMMON;
            LUA_GET_VALUE("Level", iCharID );
            LUA_GET_VALUE_DEF("RECOVERY_TOTAL_TIME", kSavingInfo.fRecoveryTotalTime, 0.f );
            LUA_GET_VALUE_DEF("RECOVER_WAIT_TIME", kSavingInfo.fRecoveryWaitTime, 1.f ); 
            kSavingInfo.fRecoveryWaitTime *= 1000.f; //ms 이기 때문에....
            LUA_GET_VALUE_DEF("PENALTY_RECOVERY_RATE", kSavingInfo.fPenaltyRecoveryRate, 1.f );
            LUA_GET_VALUE_DEF("PENALTY_EMPTY_COOLTIME", kSavingInfo.fPenaltyEmptyCooltime, 1.f );
            kSavingInfo.fPenaltyEmptyCooltime *= 1000.f; //ms 이기 때문에....

            m_mapCharSavingInfo.insert( std::make_pair( iCharID, kSavingInfo ) );

            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }
}

void KGCSavingGauge::InitGaugeTexCoords()
{
    float fTU2;

    m_vCooltimeGaugeCoords = m_pkCooltimeGauge->GetWndTexCoords();
    m_vRealGaugeCoords = m_pkRealGauge->GetWndTexCoords();
    
    fTU2 = m_vRealGaugeCoords.x + 1.f * ( m_vRealGaugeCoords.z - m_vRealGaugeCoords.x );
    fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

    m_pkRealGauge->SetWndTexCoords( D3DXVECTOR4( m_vRealGaugeCoords.x,
        m_vRealGaugeCoords.y, fTU2,
        m_vRealGaugeCoords.w ) );
    m_pkRealGauge->SetWidth( static_cast<int>( MAX_LEN_SAVING_GAUGE ) );

    m_pkCooltimeGauge->SetWndTexCoords( D3DXVECTOR4( m_vCooltimeGaugeCoords.x,
        m_vCooltimeGaugeCoords.y, fTU2,
        m_vCooltimeGaugeCoords.w ) );
    m_pkCooltimeGauge->SetWidth( static_cast<int>( MAX_LEN_SAVING_GAUGE ) );
}

void KGCSavingGauge::InitCurCharSavingInfo()
{
    std::map< int, KCharSavingInfo >::iterator mit = m_mapCharSavingInfo.find( g_MyD3D->GetMyPlayer()->Extra_Char_Num );
    if( mit != m_mapCharSavingInfo.end() ) {
        m_kCurCharSavingInfo = mit->second;
    } else {
        mit = m_mapCharSavingInfo.find( CID_COMMON );
        if( mit != m_mapCharSavingInfo.end() ) {
            m_kCurCharSavingInfo = mit->second;
        }
        // 이래 저래 아니면 이상하게 되는게 차라리 나음...
    }
}

void KGCSavingGauge::SetUseGauge( bool bUse_ )
{
    m_bUse = bUse_;

    m_pkCooltimeGauge->ToggleRender( bUse_ );
    m_pkRealGauge->ToggleRender( bUse_ );
    m_pkEmptyGauge->ToggleRender( bUse_ );
}
