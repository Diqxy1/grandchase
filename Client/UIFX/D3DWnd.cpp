#include "stdafx.h"
//
//#include "D3DWnd.h"
//
//
#include "../GCUI/GCToolTip.h"
//
//
#include "../MyD3D.h"
//
#include "ClientErr.h"

float       g_fUITime = 0.0f;

KD3DWnd*    KD3DWnd::m_pkMouseFocusedWnd;
KD3DWnd*    KD3DWnd::m_pkMouseHoverWnd;
int KD3DWndEventProcedure::ms_GlobalUID = 0;

IMPLEMENT_CLASSNAME( KD3DWnd );

int KD3DWnd::ms_iAccUID = 0;

int KD3DWnd::GetFreeUID()
{
    return ms_iAccUID++;
}

KD3DWnd::KD3DWnd( void ) : m_vCreatePos( 0.0f, 0.0f ),
                           m_vCurrentPos( 0.0f, 0.0f ),
                           m_vWindowPos( 0.0f, 0.0f ),
                           m_vDestroyPos( 0.0f, 0.0f ),
						   m_vGapPos(0.0f,0.0f),
                           m_iOldJoystickKey(0)
{
    m_UID                  = GetFreeUID();
    m_dwWidth               = 0;
    m_dwHeight              = 0;

    m_strWndName.erase();
    //m_strFriend.erase();
    m_strToolTip.erase();
    m_strHotKey.erase();

    m_pUIScene              = NULL;
    m_pParent               = NULL;
    m_vecChild.clear();

    m_pMotionSpline         = SPLINE_LINEAR;
    m_pAlphaSpline          = SPLINE_LINEAR;
    m_pActionListener       = NULL;

    m_eState                = D3DWS_DISABLED;
    m_eMode                 = D3DWM_DEFAULT;

    for( int i = 0 ; i < (int)m_aiWndTex.size() ; ++i )
    {
        m_aiWndTex[i] = -1;
    }
    for( int i = 0; i < (int)m_avTexCoords.size(); ++i)
    {
        m_avTexCoords[i].x = 0.0f;
        m_avTexCoords[i].y = 0.0f;
        m_avTexCoords[i].z = 0.0f;
        m_avTexCoords[i].w = 0.0f;
    }

//    ::D3DXMatrixIdentity( &m_mScale );
//    ::D3DXMatrixIdentity( &m_mTrans );
//    ::D3DXMatrixIdentity( &m_mTotal );

    m_fTime                 = 0.0f;
    m_fMoveBegTime          = 0.0f;
    m_fMoveGapTime          = 0.0f;

    SetFontSize( DEFAULT_FONT_SIZE );
    m_dwFontColor           = 0xffffffff/*D3DCOLOR_XRGB( 244, 244, 244 )*/;
    m_dwWndColor            = 0xffffffff;

    m_mapControlLink.clear();

    m_dwBooleanState        = 0;
    m_dwBooleanState        |= EWND_RENDER;
    m_dwBooleanState        |= EWND_AUTOCREATE;
    m_dwBooleanState        |= EWND_BTNEVENTCATCH;
    m_dwBooleanState        |= EWND_TOOLTIPRENDER;

    m_vecHotKey.clear();
    m_setHotJostickKey.clear();

    m_vTrans                = D3DXVECTOR2(0.0f, 0.0f);

    m_dwFontOutlineColor    = 0xFF000000;

    m_fWindowScaleX=1.0f;
    m_fWindowScaleY=1.0f;

	m_pCache				= NULL;
	m_eAlignType	= WNDUIAT_EXPANTION; //늘이는게 기본임
	m_vGapPos = D3DXVECTOR2(0.0f,0.0f);
    m_dwBeforeClickTime = 0;

    SetModifyAlpha( false );
    m_eToolTipWndAlignType = WNDUIAT_EXPANTION;

	m_dwOptionalData = 0;
}

KD3DWnd::~KD3DWnd( void )
{
    DisconnectAllEventProcedure();
    RemoveAllChild( true );
}

void KD3DWnd::SetWndTexCoords( const D3DXVECTOR4& vTexCoords )
{
	
    if ( vTexCoords.x <= vTexCoords.z && vTexCoords.y <= vTexCoords.w )
    {
        // 가로
        m_rect[0].m_vTex = D3DXVECTOR2( vTexCoords.x, vTexCoords.y ); // ( tu1, tv1 )
        m_rect[1].m_vTex = D3DXVECTOR2( vTexCoords.z, vTexCoords.y ); // ( tu2, tv1 )
        m_rect[2].m_vTex = D3DXVECTOR2( vTexCoords.x, vTexCoords.w ); // ( tu1, tv2 )
        m_rect[3].m_vTex = D3DXVECTOR2( vTexCoords.z, vTexCoords.w ); // ( tu2, tv2 )
    }
    else
    {
        // 세로
        m_rect[0].m_vTex = D3DXVECTOR2( vTexCoords.x, vTexCoords.y ); // ( tu1, tv1 )
        m_rect[1].m_vTex = D3DXVECTOR2( vTexCoords.x, vTexCoords.w ); // ( tu1, tv2 )
        m_rect[2].m_vTex = D3DXVECTOR2( vTexCoords.z, vTexCoords.y ); // ( tu2, tv1 )
        m_rect[3].m_vTex = D3DXVECTOR2( vTexCoords.z, vTexCoords.w ); // ( tu2, tv2 )
    }
    SetRecalcRect( true );
}

void KD3DWnd::SetWndTexCoords( ED3DWndMode eMode, const D3DXVECTOR4& vTexCoords )
{
    m_avTexCoords[ eMode ] = vTexCoords;
    SetWndTexCoords( m_avTexCoords[ eMode ] );
}

D3DXVECTOR2 KD3DWnd::GetScaledVector(D3DXVECTOR2 vPos) const
{
	switch(m_eAlignType)
	{
	case WNDUIAT_EXPANTION:
		vPos.x *= m_fWindowScaleX;
		vPos.y *= m_fWindowScaleY;		
		break;
	case WNDUIAT_POSRIGHTBASE:
		//오른쪽에 밀착형으로 처리
		vPos.x = GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-vPos.x) * m_fWindowScaleX;
		vPos.y *= m_fWindowScaleY;		
		vPos.y +=m_vGapPos.y;
		break;
	case WNDUIAT_POSRIGHTTOPBASE:
		vPos.x = GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-vPos.x) * m_fWindowScaleX;
		vPos.y *= m_fWindowScaleY;		
		vPos.y +=m_vGapPos.y;
		break;
	case WNDUIAT_POSZEROBASE:
		//그냥 이동만 한다.
		vPos.x *= m_fWindowScaleX;
		vPos.y *= m_fWindowScaleY;
		vPos +=m_vGapPos;
		break;
	case WNDUIAT_POSBOTTOMBASE:
		vPos.x *= m_fWindowScaleX;
		vPos.y = GC_SCREEN_HEIGHT*g_pGCDeviceManager->GetWindowScaleY()-(GC_SCREEN_HEIGHT-vPos.y) * m_fWindowScaleY;
		vPos.x +=m_vGapPos.x;
		break;
	}
	return vPos;
}

bool KD3DWnd::CheckPosInWindowBound( const D3DXVECTOR2& vPos ) const
{
	// 그랜드체이스에 포커스가 없으면 무조건 실패
	if( !g_MyD3D->m_bHasFocus )
		return false;

	//D3DXVECTOR2 vOrigin = GetFixedWindowPos();
	if( m_pParent != NULL ) {
		if( m_pParent->IsClipChild() && m_pParent->CheckPosInWindowBound( vPos ) == false ) {
			return false;
		}
	}

	//D3DXVECTOR2 vOrigin = GetRelocatedWindowPos();
	D3DXVECTOR2 vOrigin = GetFixedWindowPos();
	//vOrigin +=m_vGapPos;
	if( WNDUIAT_EXPANTION != m_eAlignType )
	{
		return ( vOrigin.x + m_vGapPos.x <= vPos.x && vPos.x < vOrigin.x + m_vGapPos.x + m_dwWidth ) &&
			( vOrigin.y + m_vGapPos.y <= vPos.y && vPos.y < vOrigin.y + m_vGapPos.y + m_dwHeight );
	}

	return ( vOrigin.x * g_pGCDeviceManager->GetWindowScaleX() <= vPos.x && 
		vPos.x < vOrigin.x * g_pGCDeviceManager->GetWindowScaleX() + m_dwWidth * g_pGCDeviceManager->GetWindowScaleX() ) &&
		( vOrigin.y* g_pGCDeviceManager->GetWindowScaleY() <= vPos.y && 
		vPos.y < vOrigin.y* g_pGCDeviceManager->GetWindowScaleY() + m_dwHeight* g_pGCDeviceManager->GetWindowScaleY() );
}

bool KD3DWnd::CheckPosInWindowBoundWithChild( const D3DXVECTOR2& vPos, bool bChildVisible/* = false*/ ) const
{
    if ( CheckPosInWindowBound( vPos ) ) return true;

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        if( bChildVisible == true && m_vecChild[i]->IsRenderOn() == false ) continue;
        if ( m_vecChild[i]->CheckPosInWindowBoundWithChild( vPos ) ) return true;
    }

    return false;
}

namespace {
    void ChangeUVtoTexel( D3DXVECTOR4& vUV )
    {
        float fMagicNumber=0.5f;
        if (vUV.x>vUV.z)
        {
            fMagicNumber*=-1.0f;
        }
        vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
        vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

        fMagicNumber=0.5f;
        if (vUV.y>vUV.w)
        {
            fMagicNumber*=-1.0f;
        }
        vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
        vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

        TEXEL_BOUND(vUV.x);
        TEXEL_BOUND(vUV.y);
        TEXEL_BOUND(vUV.z);
        TEXEL_BOUND(vUV.w);
    }

	void Compensation(float& value_)
	{

		if (value_ < 0.0f)
		{
			value_ = 0.0f;
			return;
		}

		if (value_ > 1.0f)
		{
			value_ = 1.0f;
		}
	}
}

std::string KD3DWnd::GetUniqueWndName( KD3DWnd* pkParents, const std::string& strWndName )
{
    if( pkParents == NULL )
        return strWndName;

    if( pkParents->GetChildByName( strWndName ) == NULL )
        return strWndName;

    for( int i = 0 ;; ++i )
    {
        std::stringstream stm;
        stm<<strWndName<<i;

        if( pkParents->GetChildByName( stm.str() ) == NULL )
            return stm.str();
    }

    return strWndName;
}

HRESULT KD3DWnd::LoadWnd( KLuaManager* pkLuaMgr, KD3DWnd* pkParent, std::string strWndName )
{
    HRESULT hr = S_OK;
    // read values
    if (strWndName.empty())
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "name",        strWndName ) ) )   strWndName = "";
        strWndName = GetUniqueWndName( pkParent, strWndName );
    }
    strWndName = GetUniqueWndName( pkParent, strWndName );
    SetWindowName( strWndName );

    std::string strFriend;
    if ( FAILED( hr = pkLuaMgr->GetValue( "friend",      strFriend ) ) )    strFriend = "";


    DWORD dwWidth = 0, dwHeight = 0;
    if ( FAILED( hr = pkLuaMgr->GetValue( "width",       dwWidth ) ) )      dwWidth = 0;
    if ( FAILED( hr = pkLuaMgr->GetValue( "height",      dwHeight ) ) )     dwHeight = 0;
    SetWidthHeight( dwWidth, dwHeight );

    if ( FAILED( hr = pkLuaMgr->GetValue( "def_tex_idx", m_aiWndTex[D3DWM_DEFAULT] ) ) )   m_aiWndTex[D3DWM_DEFAULT] = -1;
    //if ( FAILED( hr = pkLuaMgr->GetValue( "pixelvalue", bPixelValue ) ) )   bPixelValue = 0;
    if ( FAILED( hr = pkLuaMgr->GetValue( "def_tu1",     m_avTexCoords[D3DWM_DEFAULT].x ) ) )       m_avTexCoords[D3DWM_DEFAULT].x = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "def_tv1",     m_avTexCoords[D3DWM_DEFAULT].y ) ) )       m_avTexCoords[D3DWM_DEFAULT].y = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "def_tu2",     m_avTexCoords[D3DWM_DEFAULT].z ) ) )       m_avTexCoords[D3DWM_DEFAULT].z = 1.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "def_tv2",     m_avTexCoords[D3DWM_DEFAULT].w ) ) )       m_avTexCoords[D3DWM_DEFAULT].w = 1.0f;

    if ( FAILED( hr = pkLuaMgr->GetValue( "hov_tex_idx", m_aiWndTex[D3DWM_HOVER] ) ) )   m_aiWndTex[D3DWM_HOVER] = -1;

    if( m_aiWndTex[D3DWM_HOVER] == -1 )
    {
        m_aiWndTex[D3DWM_HOVER] = m_aiWndTex[D3DWM_DEFAULT];
        m_avTexCoords[D3DWM_HOVER] = m_avTexCoords[D3DWM_DEFAULT];
    }
    else
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "hov_tu1",     m_avTexCoords[D3DWM_HOVER].x ) ) )       m_avTexCoords[D3DWM_HOVER].x = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "hov_tv1",     m_avTexCoords[D3DWM_HOVER].y ) ) )       m_avTexCoords[D3DWM_HOVER].y = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "hov_tu2",     m_avTexCoords[D3DWM_HOVER].z ) ) )       m_avTexCoords[D3DWM_HOVER].z = 1.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "hov_tv2",     m_avTexCoords[D3DWM_HOVER].w ) ) )       m_avTexCoords[D3DWM_HOVER].w = 1.0f;
    }

    if ( FAILED( hr = pkLuaMgr->GetValue( "sel_tex_idx", m_aiWndTex[D3DWM_SELECT] ) ) )   m_aiWndTex[D3DWM_SELECT] = -1;

    if( m_aiWndTex[D3DWM_SELECT] == -1 )
    {
        m_aiWndTex[D3DWM_SELECT] = m_aiWndTex[D3DWM_DEFAULT];
        m_avTexCoords[D3DWM_SELECT] = m_avTexCoords[D3DWM_DEFAULT];
    }
    else
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "sel_tu1",     m_avTexCoords[D3DWM_SELECT].x ) ) )       m_avTexCoords[D3DWM_SELECT].x = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "sel_tv1",     m_avTexCoords[D3DWM_SELECT].y ) ) )       m_avTexCoords[D3DWM_SELECT].y = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "sel_tu2",     m_avTexCoords[D3DWM_SELECT].z ) ) )       m_avTexCoords[D3DWM_SELECT].z = 1.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "sel_tv2",     m_avTexCoords[D3DWM_SELECT].w ) ) )       m_avTexCoords[D3DWM_SELECT].w = 1.0f;
    }

    if ( FAILED( hr = pkLuaMgr->GetValue( "act_tex_idx", m_aiWndTex[D3DWM_ACTIVE] ) ) )   m_aiWndTex[D3DWM_ACTIVE] = -1;

    if( m_aiWndTex[D3DWM_ACTIVE] == -1 )
    {
        m_aiWndTex[D3DWM_ACTIVE] = m_aiWndTex[D3DWM_DEFAULT];
        m_avTexCoords[D3DWM_ACTIVE] = m_avTexCoords[D3DWM_DEFAULT];
    }
    else
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "act_tu1",     m_avTexCoords[D3DWM_ACTIVE].x ) ) )       m_avTexCoords[D3DWM_ACTIVE].x = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "act_tv1",     m_avTexCoords[D3DWM_ACTIVE].y ) ) )       m_avTexCoords[D3DWM_ACTIVE].y = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "act_tu2",     m_avTexCoords[D3DWM_ACTIVE].z ) ) )       m_avTexCoords[D3DWM_ACTIVE].z = 1.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "act_tv2",     m_avTexCoords[D3DWM_ACTIVE].w ) ) )       m_avTexCoords[D3DWM_ACTIVE].w = 1.0f;
    }

    if ( FAILED( hr = pkLuaMgr->GetValue( "loc_tex_idx", m_aiWndTex[D3DWM_LOCK] ) ) )   m_aiWndTex[D3DWM_LOCK] = -1;

    if( m_aiWndTex[D3DWM_LOCK] == -1 )
    {
        m_aiWndTex[D3DWM_LOCK] = m_aiWndTex[D3DWM_DEFAULT];
        m_avTexCoords[D3DWM_LOCK] = m_avTexCoords[D3DWM_DEFAULT];
    }
    else
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "loc_tu1",     m_avTexCoords[D3DWM_LOCK].x ) ) )       m_avTexCoords[D3DWM_LOCK].x = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "loc_tv1",     m_avTexCoords[D3DWM_LOCK].y ) ) )       m_avTexCoords[D3DWM_LOCK].y = 0.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "loc_tu2",     m_avTexCoords[D3DWM_LOCK].z ) ) )       m_avTexCoords[D3DWM_LOCK].z = 1.0f;
        if ( FAILED( hr = pkLuaMgr->GetValue( "loc_tv2",     m_avTexCoords[D3DWM_LOCK].w ) ) )       m_avTexCoords[D3DWM_LOCK].w = 1.0f;
    }

    ChangeUVtoTexel(m_avTexCoords[D3DWM_DEFAULT]);
    ChangeUVtoTexel(m_avTexCoords[D3DWM_HOVER]);
    ChangeUVtoTexel(m_avTexCoords[D3DWM_SELECT]);
    ChangeUVtoTexel(m_avTexCoords[D3DWM_ACTIVE]);
    ChangeUVtoTexel(m_avTexCoords[D3DWM_LOCK]);


    if ( FAILED( hr = pkLuaMgr->GetValue( "create_x",    m_vCreatePos.x ) ) )  m_vCreatePos.x = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "create_y",    m_vCreatePos.y ) ) )  m_vCreatePos.y = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "window_x",    m_vWindowPos.x ) ) )  m_vWindowPos.x = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "window_y",    m_vWindowPos.y ) ) )  m_vWindowPos.y = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "destroy_x",   m_vDestroyPos.x ) ) ) m_vDestroyPos.x = 0.0f;
    if ( FAILED( hr = pkLuaMgr->GetValue( "destroy_y",   m_vDestroyPos.y ) ) ) m_vDestroyPos.y = 0.0f;
   
    if( pkParent == NULL )
    {
        if ( FAILED( hr = pkLuaMgr->GetValue( "move_gap",    m_fMoveGapTime ) ) ) m_fMoveGapTime = 0.0f;
    }
    else
        m_fMoveGapTime = pkParent->GetMoveGapTime();
    
    //if ( FAILED( hr = pkLuaMgr->GetValue( "motion",      strMotion ) ) )    strMotion = "linear";
    //if ( FAILED( hr = pkLuaMgr->GetValue( "alpha",       strAlpha ) ) )     strAlpha = "linear";

    bool bAutoCreate = true;
    if ( FAILED( hr = pkLuaMgr->GetValue( "auto_create", bAutoCreate ) ) )     bAutoCreate = true;
    SetAutoCreate( bAutoCreate );

    bool bPredraw = false;
    if ( FAILED( hr = pkLuaMgr->GetValue( "predraw",     bPredraw ) ) )        bPredraw = false;
    SetPredraw( bPredraw );

    int iHotKey = -1;
    if ( FAILED( hr = pkLuaMgr->GetValue( "hotkey",      iHotKey ) ) )         iHotKey = -1;
    SetHotKey( iHotKey );

    std::string strHotKey;
    if ( FAILED( hr = pkLuaMgr->GetValue( "hotkeystyle", strHotKey ) ) )       strHotKey = "";
    SetHotKeyStyle( strHotKey );

    bool	bRender = true;
    if ( FAILED( hr = pkLuaMgr->GetValue( "visible",     bRender ) ) )         bRender = true;
    ToggleRender( bRender );

    m_pParent = pkParent;

    SetWndTexCoords( m_avTexCoords[D3DWM_DEFAULT] );

    if( m_pParent != NULL )
    {
        m_pParent->AddChild( this );
    }

    return hr;
}

void KD3DWnd::Register( const KD3DWndClass& rkWndClass )
{    
	SetWindowName( (std::string)rkWndClass.strWndName );
    //SetFriendWindowName( rkWndClass.strFriend );

    SetWidthHeight( rkWndClass.dwWidth, rkWndClass.dwHeight );

    if ( rkWndClass.pParent != NULL )
    {
        ((KD3DWnd *)(rkWndClass.pParent))->AddChild( this );
    }

    m_vCreatePos    = rkWndClass.vCreatePos;
    m_vWindowPos    = rkWndClass.vWindowPos;
    m_vDestroyPos   = rkWndClass.vDestroyPos;
	m_vGapPos = D3DXVECTOR2(0.0f,0.0f);

    m_fMoveGapTime  = rkWndClass.fMoveGap;

    m_aiWndTex[D3DWM_DEFAULT]  = rkWndClass.iDefTex;
    m_aiWndTex[D3DWM_HOVER]    = rkWndClass.iHovTex;
    m_aiWndTex[D3DWM_SELECT]   = rkWndClass.iSelTex;
    m_aiWndTex[D3DWM_ACTIVE]   = rkWndClass.iActTex;
    m_aiWndTex[D3DWM_LOCK]     = rkWndClass.iLocTex;

    m_avTexCoords[D3DWM_DEFAULT] = rkWndClass.vDefTexCoords;
    m_avTexCoords[D3DWM_HOVER]  = rkWndClass.vHovTexCoords;
    m_avTexCoords[D3DWM_SELECT] = rkWndClass.vSelTexCoords;
    m_avTexCoords[D3DWM_ACTIVE] = rkWndClass.vActTexCoords;
    m_avTexCoords[D3DWM_LOCK]   = rkWndClass.vLocTexCoords;

    for ( UINT i = D3DWM_HOVER; i < NUM_WINDOW_MODE; ++i )
    {
        if (-1 == m_aiWndTex[i] )
        {
            m_aiWndTex[i]   = m_aiWndTex[D3DWM_DEFAULT];
            m_avTexCoords[i] = m_avTexCoords[D3DWM_DEFAULT];
        }
    }

    m_eModePrev = m_eMode = D3DWM_DEFAULT;
    SetWndTexCoords( m_avTexCoords[D3DWM_DEFAULT] );
}

void KD3DWnd::Create( bool bAuto )
{
    if ( D3DWS_DISABLED != GetWndState() ) return;

	// 개개 Create에서 따로 설정해주면 씹을수 있도록 미리 설정
    if (m_eAlignType == WNDUIAT_EXPANTION)
    {
        m_fWindowScaleX=g_pGCDeviceManager->GetWindowScaleX();
        m_fWindowScaleY=g_pGCDeviceManager->GetWindowScaleY();
    }
    else
    {
        m_fWindowScaleX=1.0f;
        m_fWindowScaleY=1.0f;
    }

    OnCreate();
    CallFunction( "OnCreate", true );
    //SendEvent( KActionEvent( this, D3DWE_CREATE ) );

    SetWndState( D3DWS_CREATE, false );
    m_fMoveBegTime  = g_fUITime;

//    ::D3DXMatrixIdentity( &m_mScale );
//    ::D3DXMatrixIdentity( &m_mTrans );
//    ::D3DXMatrixIdentity( &m_mTotal );

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        if ( bAuto )
        {
            if ( m_vecChild[i]->IsAutoCreate() ) 
                m_vecChild[i]->Create( true );
        }
        else
        {
            m_vecChild[i]->Create();
        }

    }

    m_dwBooleanState |= EWND_CREATED;
 }

void KD3DWnd::Destroy( void )
{
    if ( D3DWS_ENABLED != GetWndState() ) return;

    OnDestroy();
    CallFunction( "OnDestroy", true );
    //SendEvent( KActionEvent( this, D3DWE_DESTROY ) );

    SetWndState( D3DWS_DESTROY, false );
    m_fMoveBegTime  = m_fTime;

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        m_vecChild[i]->Destroy();
    }

    m_dwBooleanState &= ~EWND_CREATED;
    SetModifyAlpha( false );
}

HRESULT KD3DWnd::FrameMove( void )
{
    m_fTime = g_fUITime;

    if ( IsRenderOn() == false )
        return S_OK;

    UpdateWindowBountWithMosuePos();

    PushTable( false );

    KD3DWnd& kWnd = *this;
    BOOST_SCOPE_EXIT( (&kWnd) ){
        kWnd.PopTable( false );
    } BOOST_SCOPE_EXIT_END;

    // 2007.11.12 wony
    // 툴팁에 대한 제어 ( 윈도우의 스테이트에 관계 없이 툴팁을 보여주기 위해
    // FrameMove에서 툴팁을 띄워줌..
    CheckToolTip();

    if ( D3DWS_DISABLED == GetWndState() )
    {
        FrameMoveInDisabledState();
        //CallFunction( "FrameMoveInDisabledState" );
        return S_OK;
    }

    if( !IsWndUpdateLock() && m_vecChild.size() != 0)
    {
        for ( int i = (int)m_vecChild.size() - 1 ; i >= 0 ; i-- )
        {
            m_vecChild[i]->FrameMove();
        }
    }

    switch ( GetWndState() )
    {
        case D3DWS_CREATE:
        {
            FrameMoveInCreateState();
            //CallFunction( "FrameMoveInCreateState" );

            float t;
            ED3DWndState eState = GetWndState();
            if ( 0.0f == m_fMoveGapTime )
            {
                t = 1.0f;
                eState = D3DWS_ENABLED;
            }
            else
            {
                t = ( m_fTime - m_fMoveBegTime ) / m_fMoveGapTime;
                if ( 1.0f <= t || m_fTime < m_fMoveBegTime )
                {
                    t = 1.0f;
                    eState = D3DWS_ENABLED;
                }
            }

            if( eState == D3DWS_ENABLED ) {
                SetWndState( D3DWS_ENABLED );
            }

            if( !IsModifyAlpha() )
                TransformWindow( m_vCreatePos, m_vWindowPos, t, true );
            break;
        }
        case D3DWS_ENABLED:
        {
            if( g_pkUIScene->GetReverse() )
            {
                if ( IsSelfInputCheck() && !IsWndUpdateLock() ) 
                    UpdateWndMode();

                SetCurrentWindowLocalPos( m_vWindowPos );

                if ( IsRenderOn() == false ) return S_OK;

                FrameMoveInEnabledState();
                CallFunction( "FrameMoveInEnabledState" );

                if( !IsModifyAlpha() ) {
                    TransformWindow( m_vCurrentPos, m_vCurrentPos, 1.0f, true );
                }
            }
            break;
        }
        case D3DWS_DESTROY:
        {
            if( g_pkUIScene->GetReverse() )
            {
                FrameMoveInDestroyState();
                //CallFunction( "FrameMoveInDestroyState" );

                float t;
                ED3DWndState eState = GetWndState();

                if ( 0.0f == m_fMoveGapTime )
                {
                    t = 1.0f;
                    eState = D3DWS_DISABLED;
                }
                else
                {
                    t = ( m_fTime - m_fMoveBegTime ) / m_fMoveGapTime;
                    if ( 1.0f <= t || m_fTime < m_fMoveBegTime )
                    {
                        t = 1.0f;
                        eState = D3DWS_DISABLED;
                    }
                }

                if( eState == D3DWS_DISABLED ) {
                    SetWndState( D3DWS_DISABLED );
                }

                if( !IsModifyAlpha() )
                    TransformWindow( m_vWindowPos, m_vDestroyPos, t, false );
            }
            return S_OK;
            //break;
		}
	}

	return S_OK;
}

void KD3DWnd::ReCalcReLocatedRect()
{
	for (int i=0;i<4;i++)
	{
		m_RelocatedDrawingrect[i]=m_rect[i];
	}

	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	float fTemp;
	switch(m_eAlignType)
	{
	case WNDUIAT_EXPANTION:
		for (int i=0;i<4;i++)
		{	
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.x*m_fWindowScaleX) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.x=fTemp;
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.y*m_fWindowScaleY) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.y=fTemp;
		}
		break;
	case WNDUIAT_POSRIGHTBASE:
		for (int i=0;i<4;i++)
		{
			//m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;
			//오른쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x=GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-m_RelocatedDrawingrect[i].m_vPos.x)* m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;

			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.x) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.x = fTemp;
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.y) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.y = fTemp;

		}
		break;
	case WNDUIAT_POSRIGHTTOPBASE:
		for (int i=0;i<4;i++)
		{
//			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;
			//오른쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x=GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-m_RelocatedDrawingrect[i].m_vPos.x) * m_fWindowScaleX;		
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;

			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.x) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.x = fTemp;
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.y) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.y = fTemp;
		}
		break;
		//그냥 이동만 한다.
	case WNDUIAT_POSZEROBASE:
		for (int i=0;i<4;i++)
		{
			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;

			m_RelocatedDrawingrect[i].m_vPos.x+=m_vGapPos.x;
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;

			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.x) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.x = fTemp;
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.y) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.y = fTemp;
		}
		break;
	case WNDUIAT_POSBOTTOMBASE:
		for (int i=0;i<4;i++)
		{
			//아래쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			//m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;

			m_RelocatedDrawingrect[i].m_vPos.y=GC_SCREEN_HEIGHT*g_pGCDeviceManager->GetWindowScaleY()-(GC_SCREEN_HEIGHT-m_RelocatedDrawingrect[i].m_vPos.y)* m_fWindowScaleY;						
			m_RelocatedDrawingrect[i].m_vPos.x+=m_vGapPos.x;

			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.x) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.x = fTemp;
			fTemp = static_cast<long>(m_RelocatedDrawingrect[i].m_vPos.y) - 0.5f;
			m_RelocatedDrawingrect[i].m_vPos.y = fTemp;
		}
		break;
	}

	//스케일값이 적용되었을시에 반 텍셀씩 침범하는 케이스가 생긴다.(이 부분에 대한 보정은 초기에 한다.0.5씩 안쪽으로 읽어 드립)
	//하지만 1:1 대응일는 이부분데 대한 값을 복원시켜줘야 한다.
	//1024->Texture의 가로 세로 길이가 각각 512 씩이다 즉 0.5/512->1/1024
	float fGuardTexLen = 1.0f/1024.0f;
	if (m_fWindowScaleX == 1.0f)
	{
		if ( m_RelocatedDrawingrect[0].m_vTex.x <= m_RelocatedDrawingrect[3].m_vTex.x)
		{
			m_RelocatedDrawingrect[0].m_vTex.x -= fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[0].m_vTex.x);
			m_RelocatedDrawingrect[3].m_vTex.x += fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[3].m_vTex.x);

			if (m_RelocatedDrawingrect[0].m_vTex.y <= m_RelocatedDrawingrect[3].m_vTex.y)
			{
				m_RelocatedDrawingrect[2].m_vTex.x -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.x);
				m_RelocatedDrawingrect[1].m_vTex.x += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.x);
			}
			else
			{
				m_RelocatedDrawingrect[2].m_vTex.x += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.x);
				m_RelocatedDrawingrect[1].m_vTex.x -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.x);
			}	
		}
		else
		{
			m_RelocatedDrawingrect[0].m_vTex.x += fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[0].m_vTex.x);
			m_RelocatedDrawingrect[3].m_vTex.x -= fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[3].m_vTex.x);

			if (m_RelocatedDrawingrect[0].m_vTex.y <= m_RelocatedDrawingrect[3].m_vTex.y)
			{
				m_RelocatedDrawingrect[2].m_vTex.x -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.x);
				m_RelocatedDrawingrect[1].m_vTex.x += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.x);
			}
			else
			{
				m_RelocatedDrawingrect[2].m_vTex.x += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.x);
				m_RelocatedDrawingrect[1].m_vTex.x -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.x);
			}
			
		}

	}

	

	if (m_fWindowScaleY == 1.0f)
	{
		if (m_RelocatedDrawingrect[0].m_vTex.y <= m_RelocatedDrawingrect[3].m_vTex.y)
		{
			m_RelocatedDrawingrect[0].m_vTex.y -= fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[0].m_vTex.y);
			m_RelocatedDrawingrect[3].m_vTex.y += fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[3].m_vTex.y);
			if (m_RelocatedDrawingrect[0].m_vTex.x <= m_RelocatedDrawingrect[3].m_vTex.x)
			{
				m_RelocatedDrawingrect[1].m_vTex.y -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.y);
				m_RelocatedDrawingrect[2].m_vTex.y += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.y);

			}
			else
			{
				m_RelocatedDrawingrect[1].m_vTex.y += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.y);
				m_RelocatedDrawingrect[2].m_vTex.y -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.y);
			}
		}
		else
		{
			m_RelocatedDrawingrect[0].m_vTex.y += fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[0].m_vTex.y);
			m_RelocatedDrawingrect[3].m_vTex.y -= fGuardTexLen;
			Compensation(m_RelocatedDrawingrect[3].m_vTex.y);
			if (m_RelocatedDrawingrect[0].m_vTex.x <= m_RelocatedDrawingrect[3].m_vTex.x)
			{
				m_RelocatedDrawingrect[1].m_vTex.y -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.y);
				m_RelocatedDrawingrect[2].m_vTex.y += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.y);
			}
			else
			{
				m_RelocatedDrawingrect[1].m_vTex.y += fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[1].m_vTex.y);
				m_RelocatedDrawingrect[2].m_vTex.y -= fGuardTexLen;
				Compensation(m_RelocatedDrawingrect[2].m_vTex.y);
			}
			
		}
		
	}


}
HRESULT KD3DWnd::Render( void )
{
    if ( false == IsRenderOn() ) return S_OK;
    if ( true == IsNotDraw() ) return S_OK;
    if ( D3DWS_DISABLED == GetWndState() ) return S_OK;
    // Draw child windows (predraw)
    for ( UINT i = 0; i < m_vecChild.size(); i++ )
    {
        if ( m_vecChild[i]->IsPredraw() ) 
            m_vecChild[i]->Render();
    }

	ReCalcReLocatedRect();
	PreDraw();

    m_pCache = g_pkUIMgr->DrawUIInScreen(m_pCache,m_aiWndTex[m_eMode],m_RelocatedDrawingrect.as_array());

    PostDraw();

    // Draw child windows (postdraw)
    g_pGCDeviceManager->PushState();
    if( IsClipChild() ) {
        g_pGCDeviceManager->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );

        D3DXVECTOR2 vPos = GetRelocatedWindowPos();
        DWORD dwHeight = GetHeight();
        DWORD dwWidth = GetWidth();
        if( m_eAlignType == WNDUIAT_EXPANTION ) {
            dwHeight = static_cast<DWORD>( dwHeight * m_fWindowScaleY );
            dwWidth = static_cast<DWORD>( dwWidth * m_fWindowScaleX );
        }
        RECT rect;
        rect.left   = static_cast<long>( vPos.x );
        rect.top    = static_cast<long>( vPos.y );
        rect.right  = static_cast<long>( vPos.x ) + dwWidth;
        rect.bottom = static_cast<long>( vPos.y ) + dwHeight;

        g_pGCDeviceManager->SetScissorRect( &rect );
    }
    for ( int i = 0; i < (int)m_vecChild.size(); i++ )
    {
        if ( false == m_vecChild[i]->IsPredraw() )           
            m_vecChild[i]->Render();
    }
    g_pGCDeviceManager->PopState();

    PostChildDraw();

    return S_OK;
}

void KD3DWnd::SetWindowZOrder( ED3DWndZOrder eZOrder )
{
    // ignore if child window
    if ( m_pParent != NULL ) return;

    switch ( eZOrder )
    {
        case D3DWZ_NOTOPMOST:
        {
            m_dwBooleanState &= ~EWND_TOPMOST;
            if ( m_pUIScene != NULL ) m_pUIScene->SetActiveWindow( this );
            break;
        }
        case D3DWZ_TOP:
        {
            if ( m_pUIScene != NULL ) m_pUIScene->SetActiveWindow( this );
            break;
        }
        case D3DWZ_TOPMOST:
        {
            m_dwBooleanState |= EWND_TOPMOST;
            if ( m_pUIScene != NULL ) m_pUIScene->SetActiveWindow( this );
            break;
        }
    }
}

void KD3DWnd::Activate( bool bActive )
{
    if( bActive )
        m_dwBooleanState |= EWND_ACTIVE;
    else
        m_dwBooleanState &= ~EWND_ACTIVE;

    if ( IsActive() )
    {
        m_eModePrev = m_eMode = D3DWM_ACTIVE;
        SetWndTexCoords( m_avTexCoords[D3DWM_ACTIVE] );
    }
    else
    {
        m_eModePrev = m_eMode = D3DWM_DEFAULT;
        SetWndTexCoords( m_avTexCoords[D3DWM_DEFAULT] );
    }
    
    OnActivate();
    CallFunction( "OnActivate", true );

    /*for ( UINT i = 0; i < m_vecChild.size(); i++ )
    {
        m_vecChild[i]->Activate( m_bActive );
    }*/

    OnActivateComplete();
    CallFunction( "OnActivateComplete", true );
}

void KD3DWnd::AddChild( KD3DWnd* pChild )
{
    if( pChild->GetWindowName().empty() )
    {
        pChild->SetWindowName( GetUniqueWndName(this, "noname" ) );
    }
    pChild->m_pParent = this;
    m_vecChild.push_back( pChild );
    m_mapChild[pChild->GetWindowName()] = pChild;
    pChild->SetAlignType( GetAlignType() );

    if( m_mapControlLink.find(pChild->GetWindowName()) != m_mapControlLink.end() )
    {
        *m_mapControlLink[pChild->GetWindowName()] = pChild;
        //KLOG((0, "이미 정의 되어 있는 Wnd"));
    }
}

void KD3DWnd::RemoveAllChild( bool bWithoutLua /*= false*/ )
{
    m_mapChild.clear();
    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        m_vecChild[i]->RemoveAllChild( bWithoutLua );

        if( bWithoutLua == false ) {
            lua_State* L = KGCLuabinder::getInstance().GetLuaState();
            CHECK_STACK(L)

                if ( PushTable() )
                {
                    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
                    lua_pushnil(L);
                    lua_setfield(L, -2, m_vecChild[i]->GetWindowNamePtr() );
                }

                PopTable();
        }

        boost::unordered_map<std::string, KD3DWnd*>::iterator mit = m_mapChild.find( m_vecChild[i]->GetWindowName() );
        if( mit != m_mapChild.end() )
        {
            m_mapChild.erase(mit);
        }
        g_pkUIMgr->RemoveUID( m_vecChild[i]->GetUID() );
        SAFE_DELETE( m_vecChild[i] );
    }
    m_vecChild.clear();
}

bool KD3DWnd::DeleteChildByName( const char* strName )
{
    for( int i = 0 ; i < (int)m_vecChild.size() ; ++i )
    {
        if( m_vecChild[i]->GetWindowName().compare( strName ) != 0 )
            continue;

        DeleteChild(i);
        return true;
    }

    return false;
}

bool KD3DWnd::DeleteChildByPtr( KD3DWnd* pWnd )
{
    for( int i = 0 ; i < (int)m_vecChild.size() ; ++i )
    {
        if( m_vecChild[i] != pWnd )
            continue;

        DeleteChild(i);
        return true;
    }

    return false;
}

void KD3DWnd::DeleteChild( int iIndex )
{
    boost::unordered_map<std::string, KD3DWnd*>::iterator mit = m_mapChild.find( m_vecChild[iIndex]->GetWindowName() );
    if( mit != m_mapChild.end() )
    {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L);

        if ( PushTable() )
        {
            lua_State* L = KGCLuabinder::getInstance().GetLuaState();
            lua_pushstring(L, mit->second->GetWindowNamePtr() );
            lua_pushnil(L);
            lua_settable( L, -3 );
        }
        PopTable();
        m_mapChild.erase(mit);
    }

    if( iIndex < 0 || iIndex >= (int)m_vecChild.size() )
    {
        //ASSERT( "You can`t delete invalid child index!!" );
        return;
    }

    g_pkUIMgr->RemoveUID( m_vecChild[iIndex]->GetUID() );
    SAFE_DELETE( m_vecChild[iIndex] );
    m_vecChild.erase( m_vecChild.begin() + iIndex );
}

bool KD3DWnd::IsAllEnabled( void ) const
{
    if ( 0 == m_vecChild.size() ) return D3DWS_ENABLED == GetWndState();

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        if ( false == m_vecChild[i]->IsAllEnabled() ) return false;
    }

    return D3DWS_ENABLED == GetWndState();
}

bool KD3DWnd::IsAllDisabled( void ) const
{
    if ( 0 == m_vecChild.size() ) return D3DWS_DISABLED == GetWndState();

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        if ( false == m_vecChild[i]->IsAllDisabled() ) return false;
    }

    return D3DWS_DISABLED == GetWndState();
}

void KD3DWnd::SpeakToActionListener( const KActionEvent& event, bool bSendLua_ /*= true*/ )
{
	if(SiKGCDragManager()->GetDragState() == KGCDragManager::KGDM_DRAG_PROGRESS) return;

#if !defined(__PATH__)
    if( g_kGlobalValue.m_bUIInfo )
    {
        WCHAR wszWndInfo[2048];
        std::ostringstream strmSystem;
        strmSystem << event.m_pWnd->ms_szClassName << " : " << event.m_pWnd->m_strWndName << "(Code : " << event.m_dwCode << ", LPARAM : " << event.m_dwlParam << ", LPARAM2 : " << event.m_dwlParam2;
        swprintf(wszWndInfo, L"%s", CA2W(strmSystem.str().c_str()) );
        std::wstring wstrWndInfo( wszWndInfo );
        g_pkChatManager->AddSystemMsg( 0, wstrWndInfo );
        g_pkFontMgr->OutTextXY( 40, 150, wstrWndInfo.c_str(), 0xff0000ff );
    }
#endif
    switch( event.m_dwCode ) {
            case KD3DWnd::D3DWE_BUTTON_DOWN:
                //case KD3DWnd::D3DWE_CURSOR_ENTER:
                //case KD3DWnd::D3DWE_CURSOR_LEAVE:
            case KD3DWnd::D3DWE_BUTTON_CLICK:
            case KD3DWnd::D3DWE_SCENE_CHANGE:
            case KD3DWnd::D3DWE_CREATE:
            case KD3DWnd::D3DWE_CREATE_COMPLETE:
            case KD3DWnd::D3DWE_DESTROY:
            case KD3DWnd::D3DWE_DESTROY_COMPLETE:
            case KD3DWnd::D3DWE_MESSAGEBOX_OPEN:
            case KD3DWnd::D3DWE_MESSAGEBOX_CLOSE:
                break;
            default:
                if( event.m_dwCode >= KD3DWnd::K3DWE_USER )
                    break;
                if( IsIgnoreOffenEvent() )
                    return;
                bSendLua_ = false;
                break;
    }
    SendEvent( event, bSendLua_ );
    if( bSendLua_ ) {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L)
            if( PushTable() )
            {
                lua_getfield( L, -1, "ActionListener" );
                if( lua_type( L, -1 ) == LUA_TFUNCTION )
                {
                    lua_getfield( L, -3, event.m_pWnd->GetWindowName().c_str() );
                    lua_tinker::push( L, event.m_dwCode );
                    lua_tinker::push( L, event.m_dwlParam );
                    lua_tinker::push( L, event.m_dwlParam2 );
                    if( lua_pcall( L, 4, LUA_MULTRET, 0 ) != 0 )
                    {
                        OKBOXA( lua_tostring(L, -1) , GetWindowName().c_str() );
                        lua_pop(L, 1 );
                    }
                }
                else
                {
                    lua_pop(L, 1 );
                }
            }
            PopTable();
    }

    if ( NULL != m_pActionListener )
    {
        m_pActionListener->ActionPerformed( event );
    }
}
/*
bool KD3DWnd::IsHangulLeadByte( const char* szText, std::size_t iIndex ) const
{
    // NOTE by parkch :
    // IsDBCSLeadByteEx 함수가 TRUE라는 것은 테스트되는 값이
    // "다음 바이트 문자와 함께 2바이트 문자를 이룰 수 있는 코드"라는 의미이다.
    // 따라서 다음 바이트를 IsDBCSLeadByteEx 함수로 테스트하게 되면 TRUE가 나올 수도 있고 FALSE가 나올 수도 있다.
    // 그러므로, 정확한 테스트를 위해서는 전체 문자열을 다 받아서 시작되는 앞부분부터 차례대로 테스트해야 한다.
    // 순차적으로 테스트하면서 IsDBCSLeadByteEx 함수가 TRUE를 리턴하게 되면 다음 바이트는 건너뛰고 검사를 계속해야 한다.
    // 이러한 내용은 MSDN을 찾아봐도 나오지 않는다. (ㅡ.ㅡ;)

    // check valid index
    if ( iIndex < 0 || std::strlen( szText ) <= iIndex ) return false;

    // find & test
    bool bLeadByte = false;
    for ( std::size_t i = 0; szText[i] != '\0'; ++i )
    {
        if ( bLeadByte )
        {
            bLeadByte = false; // skip
        }
        else
        {
            if ( ::IsDBCSLeadByteEx( GC_CODE_PAGE_IDENTIFIER, szText[i] ) ) // test
            {
                bLeadByte = true;
            }
        }

        if ( iIndex == i ) break; // found
    }

    return bLeadByte; // result
}
*/

void KD3DWnd::SetWndMode( ED3DWndMode eMode, bool bExceptChild_ /*= true*/, bool bSendEvent_ /*= true */ )
{
    if( 0 > eMode || eMode >= NUM_WINDOW_MODE )
    {
        START_LOG( clog, "윈도우 모드 인덱스 오바(dpdwnd.cpp, Line = 1104), Mode = " << eMode );
    }
    else SetWndTexCoords( m_avTexCoords[eMode] );

    if ( bExceptChild_ == false )
    {
		for ( std::vector<KD3DWnd*>::iterator vit = m_vecChild.begin() ; vit != m_vecChild.end(); ++vit )
        {
            (*vit)->SetWndMode( eMode, bExceptChild_, false );
        }
    }
    m_eModePrev = m_eMode;
    m_eMode = eMode;
}

void KD3DWnd::ResetWndMode( void )
{
	if( m_eMode != D3DWM_LOCK )
		m_eMode = ( IsActive() ? D3DWM_ACTIVE : D3DWM_DEFAULT );

    SetWndTexCoords( m_avTexCoords[m_eMode] );

    m_eModePrev = m_eMode;
}

KD3DWnd::ED3DWndMode KD3DWnd::UpdateWndMode( void )
{
    if(m_pkMouseFocusedWnd != NULL && m_pkMouseFocusedWnd != this)
        return m_eMode;

    ED3DWndMode eMode = GetWndMode();
    KD3DWnd* pWnd = this;
    void (KD3DWnd::*pSetWndModeFunc)(ED3DWndMode, bool, bool);
    pSetWndModeFunc = &KD3DWnd::SetWndMode;

    BOOST_SCOPE_EXIT( (&eMode)(&m_eMode)(&pWnd)(&pSetWndModeFunc) ) {
        if( m_eMode != eMode && m_eMode != D3DWM_LOCK ) {
        //if( m_eMode != eMode && m_eMode != D3DWM_LOCK) {
            (pWnd->*pSetWndModeFunc)(eMode,true,true);
        }
    } BOOST_SCOPE_EXIT_END;

	if( m_eMode != D3DWM_LOCK )
		eMode = ( IsActive() ? D3DWM_ACTIVE : D3DWM_DEFAULT );

    bool bCheckBound = CheckMousePosInWindowBound();

    if( bCheckBound == false )
    {
        //if( m_eMode == D3DWM_HOVER || m_eMode == D3DWM_ACTIVE )
        if( IsHoverPrev() )
        {
            SpeakToActionListener( KActionEvent( this, D3DWE_CURSOR_LEAVE ) );
            SetHoverPrev( false );
        }
    }
    else {
        g_pkUIMgr->SetCursorOnUI();
    }

	if ( IsLocked() == false )
    {
        //단축키 설정시 처리
        for ( int i = 0; i < (int)m_vecHotKey.size(); i++ )
        {
            if( m_vecHotKey[i] >= 0 && g_pkInput->IsDown( m_vecHotKey[i] ) )
            {
                //단축키 스타일이 
                //if( !m_strHotKey.compare( "downup" ) )
                //{
                g_pkInput->IgnoreDown( m_vecHotKey[i] , 30);
                SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_CLICK ) );
                ReleaseCapture();
                //}
                return m_eModePrev = m_eMode;
            }
        }

#ifdef USE_JOYSTICK
        int iKey = g_MyD3D->GetJoystickKey();
        if( m_setHotJostickKey.find(iKey) != m_setHotJostickKey.end() && m_iOldJoystickKey != iKey){
            SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_CLICK ) );
            ReleaseCapture();
            //}
            m_iOldJoystickKey = iKey;
            return m_eModePrev = m_eMode;
        }
        else{
            m_iOldJoystickKey = iKey;
        }


#endif

    }   

    // 클릭을 했는데 드레그를 하다가 내 영역을 벗어나면 클릭 취소
    if( m_pkMouseFocusedWnd == this && !bCheckBound )
    {
        ReleaseCapture();
        eMode = D3DWM_DEFAULT;
        SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_UP ) );
    }

    if ( ( IsLocked() == false && bCheckBound) || m_pkMouseFocusedWnd == this )
    {
        if( eMode != D3DWM_ACTIVE )
            eMode = D3DWM_HOVER;

        if ( false == IsHoverPrev() )
        {
            SetHoverPrev( true );
            if ( IsButtonEventCatch() ) 
            {
                SpeakToActionListener( KActionEvent( this, D3DWE_CURSOR_ENTER ) );
            }
        }


        // 역순 프레임무브일때만 마우스 이벤트 전달
        if( g_pkUIScene->GetReverse() )
        {
            if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
        	{
            	eMode = D3DWM_SELECT;
            	SetCapture(this);
            	if( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_DOWN ) );
        	}

            if( g_pkInput->BtnDown( KInput::MBRIGHT ) )
            {
                eMode = D3DWM_SELECT;
                SetCapture(this);
                if( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_RBUTTON_DOWN ) );
            }

            if ( g_pkInput->BtnPressed( KInput::MBLEFT ) )
            {
                eMode = D3DWM_SELECT;
                if ( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_PRESS ), false );
            }

            if ( g_pkInput->BtnPressed( KInput::MBRIGHT ) )
            {
                eMode = D3DWM_SELECT;
                if ( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_RBUTTON_PRESS ), false );
            }

            if( g_pkInput->BtnUp( KInput::MBRIGHT ) )
            {
                if( m_pkMouseFocusedWnd == this )
                {
                    ReleaseCapture();
                    eMode = D3DWM_ACTIVE;

                    if(bCheckBound)
                        SpeakToActionListener( KActionEvent( this, D3DWE_RBUTTON_CLICK  ) );
                    else 
                        SpeakToActionListener( KActionEvent( this, D3DWE_RBUTTON_UP ) );
                }
                else if(m_pkMouseFocusedWnd == NULL)
                {
                    //eMode = D3DWM_ACTIVE;
                    if ( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_RBUTTON_UP ) );
                }
            }

            if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
            {
                if( m_pkMouseFocusedWnd == this )
                {
                    ReleaseCapture();
                    eMode = D3DWM_ACTIVE;

                    if( bCheckBound )
                    {
                        if ( 0 == m_dwBeforeClickTime || ::timeGetTime() - m_dwBeforeClickTime > DOUBLE_CLICK_GAP )
                        {
                            // 그냥 클릭
                            SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_CLICK  ) );
                            m_dwBeforeClickTime = ::timeGetTime();
                        }
                        else
                        {
                            // 더블 클릭
                            SpeakToActionListener( KActionEvent( this, D3DWE_DOUBLE_CLICK  ) );
                            m_dwBeforeClickTime = 0;
                        }
                    }
                    else 
                        SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_UP ) );
                   }
                else if(m_pkMouseFocusedWnd == NULL)
                {
                    //eMode = D3DWM_ACTIVE;
                    if ( IsButtonEventCatch() ) SpeakToActionListener( KActionEvent( this, D3DWE_BUTTON_UP ) );
                }
            }
        }
    }
    else
    {
        SetHoverPrev( false );
    }

    return eMode;
}

void KD3DWnd::TransformWindow( const D3DXVECTOR2& vA, const D3DXVECTOR2& vB, float t, bool bCreate )
{
    D3DXVECTOR2 vPos;
    ::D3DXVec2Lerp( &vPos, &vA, &vB, (*m_pMotionSpline)( t ) );
    SetCurrentWindowLocalPos( vPos );

    m_vTrans = GetCurrentWindowLocalPos();

    if ( NULL != m_pParent )
        m_vTrans += m_pParent->m_vTrans;

    m_rect[0].m_vPos.x = m_vTrans.x;             m_rect[0].m_vPos.y = m_vTrans.y;
    m_rect[1].m_vPos.x = m_vTrans.x + GetWidth(); m_rect[1].m_vPos.y = m_vTrans.y;
    m_rect[2].m_vPos.x = m_vTrans.x;             m_rect[2].m_vPos.y = m_vTrans.y + GetHeight();
    m_rect[3].m_vPos.x = m_vTrans.x + GetWidth(); m_rect[3].m_vPos.y = m_vTrans.y + GetHeight();

    BYTE alpha = static_cast<BYTE>( 255.0f * (*m_pAlphaSpline)( bCreate ? t : ( 1.0f - t ) ) );

    for ( UINT i = 0; i < 4; ++i )
    {
        m_rect[i].m_diff = ( m_dwWndColor & D3DCOLOR_ARGB( alpha, 0xff, 0xff, 0xff ) );
    }
    SetRecalcRect( true );
}

void KD3DWnd::SetCapture(KD3DWnd* pkWnd)
{
    m_pkMouseFocusedWnd = pkWnd;
}
void KD3DWnd::ReleaseCapture()
{
    m_pkMouseFocusedWnd = NULL;
}

void KD3DWnd::Initialize( void )
{
    OnInitialize();
    for(int i = 0; i < (int)m_vecChild.size(); i++)
    {
        m_vecChild[i]->Initialize();
    }
    OnPostChildInitialize();
}

void KD3DWnd::InitState( bool bRender, bool bSelfUpdate, KActionListener* pActionListener )
{
    ToggleRender( bRender ); 
    SetSelfInputCheck( bSelfUpdate );
    AddActionListener( pActionListener );
}

void KD3DWnd::SetWidthDirect( DWORD dwWidth )
{
    m_dwWidth = dwWidth;
    TransformWindow( m_vWindowPos, m_vWindowPos, 1, true );
}

void KD3DWnd::SetHeightDirect( DWORD dwHeight )
{
    m_dwHeight = dwHeight;
    TransformWindow( m_vWindowPos, m_vWindowPos, 1, true );
}

void KD3DWnd::SetWindowPosDirect( const D3DXVECTOR2& vWindowPos )
{
    SetAllWindowPos( vWindowPos );

    TransformWindow( vWindowPos, vWindowPos, 1, true );
    for(int i = 0; i < (int)m_vecChild.size(); i++)
    {
        m_vecChild[i]->SetWindowPosDirect(m_vecChild[i]->GetFixedWindowLocalPos());
    }
}

void KD3DWnd::SetWindowPosDirectFloat( const float fx_, const float fy_ )
{
    SetWindowPosDirect( D3DXVECTOR2(fx_,fy_) );
}

void KD3DWnd::ForceSetWindowColor( D3DCOLOR dwWndColor )
{
    m_dwWndColor = dwWndColor;
    
    for ( UINT i = 0; i < 4; ++i )
    {
        m_rect[i].m_diff = ( m_dwWndColor & D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
    }
    SetRecalcRect( true );
}
void KD3DWnd::SetAllWindowColor( D3DCOLOR dwWndColor )
{
    m_dwWndColor = dwWndColor;

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        m_vecChild[i]->SetAllWindowColor( dwWndColor );
    }
}

void KD3DWnd::SetAllWindowAlpha( int iAlpha_ )
{
    SetModifyAlpha( true );
    for ( UINT i = 0; i < 4; ++i )
    {
        m_rect[i].m_diff = ( m_dwWndColor & D3DCOLOR_RGBA( 255, 255, 255, iAlpha_ ) );
    }

    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        m_vecChild[i]->SetAllWindowAlpha( iAlpha_ );
    }
    SetRecalcRect( true );
}


void KD3DWnd::CheckToolTip()
{
    // 버튼의 범위안에 마우스를 올렸다면 툴팁을 보여준다!!
    if( CheckMousePosInWindowBound() )
    {
        if( m_strToolTip.empty() == false && IsRenderOn() && IsRenderToolTip() )
        {
			g_pkUIScene->m_pkToolTip->ShowToolTip( this->GetUID(), m_strToolTip, m_vToolTipOffset, true, m_eToolTipWndAlignType, m_eToolTipAlignType  );
        }
    }
}

void KD3DWnd::SetFontOutline( bool bFontOutline/* = false*/, D3DCOLOR OutlineColor/* = 0xFF000000*/ )
{
    if( bFontOutline )
        m_dwBooleanState |= EWND_FONTOUTLINE;
    else
        m_dwBooleanState &= ~EWND_FONTOUTLINE;

    m_dwFontOutlineColor = OutlineColor;
}

void KD3DWnd::CopyWindowPosInfo( const KD3DWnd* pSrc_ )
{
    SetWidth( pSrc_->GetWidth() );
    SetHeight( pSrc_->GetHeight() );
    SetAllWindowPos( pSrc_->GetFixedWindowLocalPos() );
}

void KD3DWnd::Update( void )
{
    OnUpdate();
    CallFunction( "OnUpdate", true );

	for ( UINT i = 0; i < m_vecChild.size(); ++i )
	{
		m_vecChild[i]->Update();
	}
}

void KD3DWnd::BindToLua()
{
 //	return;

    static bool bBinded = false;
    if( bBinded == true ) {
        bBinded = true;
        return;
    }
	// 바인딩
    {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L)
        lua_tinker::class_add<KD3DWnd>( L, "KD3DWnd" );
        lua_tinker::class_def<KD3DWnd>( L, "GetChildByName",                &KD3DWnd::GetChildByName  );
        lua_tinker::class_def<KD3DWnd>( L, "GetClassName",                  &KD3DWnd::GetClassName  );
        lua_tinker::class_def<KD3DWnd>( L, "SetWindowName",                 &KD3DWnd::SetWindowNamePtr  );
        lua_tinker::class_def<KD3DWnd>( L, "GetWindowName",                 &KD3DWnd::GetWindowNamePtr  );
        lua_tinker::class_def<KD3DWnd>( L, "ToggleRender",                  &KD3DWnd::ToggleRender  );
        lua_tinker::class_def<KD3DWnd>( L, "IsRenderOn",                    &KD3DWnd::IsRenderOn  );
        lua_tinker::class_def<KD3DWnd>( L, "InitState",                     &KD3DWnd::InitState  );
        lua_tinker::class_def<KD3DWnd>( L, "SetSelfInputCheck",             &KD3DWnd::SetSelfInputCheck  );
        lua_tinker::class_def<KD3DWnd>( L, "AddActionListener",             &KD3DWnd::AddActionListener  );
        lua_tinker::class_def<KD3DWnd>( L, "GetWidth",                      &KD3DWnd::GetWidth  );
        lua_tinker::class_def<KD3DWnd>( L, "GetHeight",                     &KD3DWnd::GetHeight  );
        lua_tinker::class_def<KD3DWnd>( L, "SetWidthDirect",                &KD3DWnd::SetWidthDirect  );
        lua_tinker::class_def<KD3DWnd>( L, "SetHeightDirect",               &KD3DWnd::SetHeightDirect  );
        lua_tinker::class_def<KD3DWnd>( L, "SetWidth",                      &KD3DWnd::SetWidth  );
        lua_tinker::class_def<KD3DWnd>( L, "SetHeight",                     &KD3DWnd::SetHeight  );
        lua_tinker::class_def<KD3DWnd>( L, "SetWindowPosDirect",            &KD3DWnd::SetWindowPosDirectFloat  );
        lua_tinker::class_def<KD3DWnd>( L, "AddChild",                      &KD3DWnd::AddChildFromLua  );
        lua_tinker::class_def<KD3DWnd>( L, "SetWndState",                   &KD3DWnd::SetWndState  );
        lua_tinker::class_def<KD3DWnd>( L, "Create",                        &KD3DWnd::Create  );
        lua_tinker::class_def<KD3DWnd>( L, "Destroy",                       &KD3DWnd::Destroy  );
        lua_tinker::class_def<KD3DWnd>( L, "DeleteChild",                   &KD3DWnd::DeleteChild  );
        lua_tinker::class_def<KD3DWnd>( L, "DeleteChildByName",             &KD3DWnd::DeleteChildByName  );
        lua_tinker::class_def<KD3DWnd>( L, "DeleteChildByPtr",              &KD3DWnd::DeleteChildByPtr  );
        lua_tinker::class_def<KD3DWnd>( L, "RemoveAllChild",                &KD3DWnd::RemoveAllChild  );
        lua_tinker::class_def<KD3DWnd>( L, "Lock",                          &KD3DWnd::Lock );
        lua_tinker::class_def<KD3DWnd>( L, "Activate",                      &KD3DWnd::Activate );
        lua_tinker::class_def<KD3DWnd>( L, "GetFixedWindowLocalPos",        &KD3DWnd::GetFixedWindowLocalPos );
        lua_tinker::class_def<KD3DWnd>( L, "GetFixedWindowPos",             &KD3DWnd::GetFixedWindowPos );
        lua_tinker::class_def<KD3DWnd>( L, "GetCurrentWindowLocalPos",      &KD3DWnd::GetCurrentWindowLocalPos );
        lua_tinker::class_def<KD3DWnd>( L, "GetCurrentWindowPos",           &KD3DWnd::GetCurrentWindowPos );
        lua_tinker::class_def<KD3DWnd>( L, "GetRelocatedWindowPos",         &KD3DWnd::GetRelocatedWindowPos );
        lua_tinker::class_def<KD3DWnd>( L, "SetWindowPos",                  &KD3DWnd::SetWindowPos );
        lua_tinker::class_def<KD3DWnd>( L, "SetWindowPosDirect",            &KD3DWnd::SetWindowPosDirect );
        lua_tinker::class_def<KD3DWnd>( L, "SetFontOutline",                &KD3DWnd::SetFontOutline );
        lua_tinker::class_def<KD3DWnd>( L, "SetFontColor",                  &KD3DWnd::SetFontColor );
        lua_tinker::class_def<KD3DWnd>( L, "GetFontColor",                  &KD3DWnd::GetFontColor );
        lua_tinker::class_def<KD3DWnd>( L, "SetPredraw",                    &KD3DWnd::SetPredraw );
        lua_tinker::class_def<KD3DWnd>( L, "IsPredraw",                     &KD3DWnd::IsPredraw );
        lua_tinker::class_def<KD3DWnd>( L, "CheckPosInWindowBound",         &KD3DWnd::CheckPosInWindowBound );
        lua_tinker::class_def<KD3DWnd>( L, "Create",                        &KD3DWnd::Create );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_Next",                 &KD3DWnd::SetOrder_Next );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_Before",               &KD3DWnd::SetOrder_Before );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_Top",                  &KD3DWnd::SetOrder_Top );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_Bottom",               &KD3DWnd::SetOrder_Bottom );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_NextbyName",           &KD3DWnd::SetOrder_NextbyName );
        lua_tinker::class_def<KD3DWnd>( L, "SetOrder_BeforebyName",         &KD3DWnd::SetOrder_BeforebyName );
        lua_tinker::class_def<KD3DWnd>( L, "GetUID",                        &KD3DWnd::GetUID );
        lua_tinker::class_def<KD3DWnd>( L, "SetHotKey",                     &KD3DWnd::SetHotKey );
        lua_tinker::class_def<KD3DWnd>( L, "SetAlpha",                      &KD3DWnd::SetAllWindowAlpha );
        lua_tinker::class_def<KD3DWnd>( L, "SetToolTip",                    &KD3DWnd::SetToolTip );
    }
}

bool KD3DWnd::PushTable( bool bPushParent /*= true*/ )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    //CHECK_STACK(L)

    lua_checkstack( L, lua_gettop(L) + 5 );					// 스택 사이즈를 체크 합니다. ( 작으면 여유공간을 확보 합니다. )
    if( m_pParent == NULL ) {
        lua_getfield( L, LUA_GLOBALSINDEX, "scene_part" );

        if( lua_type(L, -1) != LUA_TTABLE ) {
            //ASSERT( !"PushTable" );
            return false;
        }
    }

    if( bPushParent )
    {
        if( m_pParent != NULL )
        {
            if( m_pParent->PushTable() == false ) {
                //ASSERT( !"PushTable" );
                return false;
            }

        }
    }
    else
    {
        if( lua_gettop( L ) < 1 ) {
            //ASSERT( !"PushTable" );
            return false;
        }
    }
    
    if( lua_type( L, -1 ) != LUA_TTABLE ) {
        //ASSERT( !"PushTable" );
        return false;
    }

    lua_getfield( L, -1, GetWindowName().c_str() );

    if( lua_type(L, -1) != LUA_TTABLE ) {
        //ASSERT( !"PushTable" );
        return false;
    }

    return true;
}

void KD3DWnd::PopTable( bool bPopParents_ )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    //CHECK_STACK(L)
    if( lua_gettop(L) > 0 )
    {
        lua_pop( L, 1 );
        if( bPopParents_ && m_pParent )
            m_pParent->PopTable();

        if( m_pParent == NULL )
            lua_pop( L, 1 );
    }
}

void KD3DWnd::PushThis(lua_State* L)
{
    lua_tinker::push( L, this );
}

bool KD3DWnd::RegistToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    //CHECK_STACK(L)
    lua_checkstack( L, lua_gettop(L) + 7 );

    if( lua_istable( L, lua_gettop(L) ) == false )
        return false;

    lua_pushstring( L, GetWindowName().c_str() );
    lua_newtable(L);
    lua_settable( L, -3 );
    lua_getfield( L, -1, GetWindowName().c_str() );
    lua_pushstring( L, GetClassName() );
    lua_setfield( L, -2, "_uitype" );
    lua_pushstring( L, "_class" );
    //lua_pushlightuserdata( L, (void*)pkWnd );
    //PushThis( L );

    lua_tinker::ptr2lua<lua_tinker::base_type<KD3DWnd>::type>::invoke(L, this );
    lua_tinker::meta_push( L, GetClassName() );

    if( lua_istable(L,-1) == false )
    {
        lua_pop(L,2);
        lua_tinker::push( L, this );
    }
    else
    {
        lua_setmetatable(L, -2);
    }

    lua_settable( L, -3 );


    if( m_pParent != NULL )
    {
        lua_pushstring( L, "_parent" );

        if( lua_istable( L, -4 ) ) {
            lua_getfield( L, -4, m_pParent->GetWindowName().c_str() );

            if( lua_istable(L,-1) == false )
            {
                lua_pop(L,2);
            }
            else
            {
                lua_settable( L, -3 );
            }
        }
        else {
            lua_pop( L, 1 );
        }
    }

    return true;
}

void KD3DWnd::AddChildFromLua( KD3DWnd* pChild )
{
    if( pChild->GetWindowName().empty() )
    {
        int iCount = 0;
        
        while( true )
        {
            std::stringstream stm;
            stm<<"noname"<<iCount;
            if( m_mapChild.find( stm.str() ) == m_mapChild.end() )
            {
                break;
            }
            ++iCount;
        }
        std::stringstream stm;
        stm<<"noname"<<iCount;
        pChild->SetWindowName( stm.str() );
    }
    AddChildWithRegistToLua( pChild );
}

D3DXVECTOR2 KD3DWnd::AdjustIntoScreen()
{
	D3DXVECTOR2 vEndPos( GC_SCREEN_FLOAT_WIDTH * g_pGCDeviceManager->GetWindowScaleX() , GC_SCREEN_FLOAT_HEIGHT * g_pGCDeviceManager->GetWindowScaleY() );
	D3DXVECTOR2 vCurrentPos = GetCurrentWindowPos();
	
	if( vCurrentPos.x + GetWidth() > vEndPos.x )
	{
		vCurrentPos.x = vEndPos.x - GetWidth();

	}

	if( vCurrentPos.y + GetHeight() > vEndPos.y )
	{
		vCurrentPos.y = vEndPos.y - GetHeight();

	}

	if( vCurrentPos.x < 0.0f )
	{
		vCurrentPos.x = 0.0f;
	}

	if( vCurrentPos.y < 0.0f )
	{
		vCurrentPos.y = 0.0f;
	}

	SetWindowPosDirect( vCurrentPos );

	return vCurrentPos;

	
}


void KD3DWnd::__SetWndState( ED3DWndState eState, bool bPushTable /*= false*/ )
{
    if( bPushTable )
    {
        PushTable( false );
    }

    KD3DWnd& kWnd = *this;
    BOOST_SCOPE_EXIT( (&kWnd)(&bPushTable) ){
        if( bPushTable ) {
            kWnd.PopTable( false );
        }
    } BOOST_SCOPE_EXIT_END;

    m_eState = eState;
    if(GetWndState() == D3DWS_ENABLED)
    {
        TransformWindow( m_vWindowPos, m_vWindowPos, 1.0f, true );
        OnCreateComplete();
        CallFunction( "OnCreateComplete" );
        //SendEvent( KActionEvent( this, D3DWE_CREATE_COMPLETE ) );
    }
    else if(GetWndState() == D3DWS_DISABLED)
    {            
        TransformWindow( m_vDestroyPos, m_vDestroyPos, 1.0f, false );

        //마우스 포커스를 먹다가 사라져 버릴경우.. 먹통이 되는 현상을 막기위한 코드임
        if( m_pkMouseFocusedWnd == this )
            m_pkMouseFocusedWnd = NULL;

        OnDestroyComplete();
        CallFunction( "OnDestroyComplete" );
        //SendEvent( KActionEvent( this, D3DWE_DESTROY_COMPLETE ) );
        DisconnectAllEventProcedure();
    }
}

void KD3DWnd::__SetChildWndState( ED3DWndState eState, bool bPushTable /*= false */ )
{
    for ( UINT i = 0; i < m_vecChild.size(); ++i )
    {
        m_vecChild[i]->__SetWndState( eState, bPushTable );
        m_vecChild[i]->__SetChildWndState( eState, bPushTable );
    }
}

void KD3DWnd::SetChildOrder( IN KD3DWnd* pScrWnd_, IN KD3DWnd* pTargetWnd_, bool bNext )
{
    //일단 두 윈도우 포인터가 유효해야 함.
    ChildContainer::iterator itorScr = std::find( m_vecChild.begin(), m_vecChild.end(), pScrWnd_ );
    if( itorScr == m_vecChild.end() )
        return;

    ChildContainer::iterator itorTarget = std::find( m_vecChild.begin(), m_vecChild.end(), pTargetWnd_ );
    if( itorTarget == m_vecChild.end() )
        return;

    m_vecChild.erase( itorScr );

    // erase 함수 때문에 iterator가 무효화 되었음으로 재 검색
    itorTarget = std::find( m_vecChild.begin(), m_vecChild.end(), pTargetWnd_ );
    m_vecChild.insert( (bNext?++itorTarget:itorTarget), pScrWnd_ );
}

void KD3DWnd::SetChildOrderTop( IN KD3DWnd* pScrWnd_ )
{
    ChildContainer::iterator itorScr = std::find( m_vecChild.begin(), m_vecChild.end(), pScrWnd_ );
    if( itorScr == m_vecChild.end() )
        return;

    m_vecChild.erase( itorScr );
    m_vecChild.push_back( pScrWnd_ );
}

void KD3DWnd::SetChildOrderBottom( IN KD3DWnd* pScrWnd_ )
{
    ChildContainer::iterator itorScr = std::find( m_vecChild.begin(), m_vecChild.end(), pScrWnd_ );
    if( itorScr == m_vecChild.end() )
        return;

    m_vecChild.erase( itorScr );
    m_vecChild.insert( m_vecChild.begin(), pScrWnd_ );
}

void KD3DWnd::SetOrder_Next( IN KD3DWnd* pWnd_ )
{
    if( m_pParent == NULL || pWnd_ == NULL ) return;

    m_pParent->SetChildOrder( this, pWnd_, true );
}

void KD3DWnd::SetOrder_Before( IN KD3DWnd* pWnd_ )
{
    if( m_pParent == NULL || pWnd_ == NULL ) return;

    m_pParent->SetChildOrder( this, pWnd_, false);
}

void KD3DWnd::SetOrder_Top()
{
    if( m_pParent == NULL ) return;

    m_pParent->SetChildOrderTop( this );
}

void KD3DWnd::SetOrder_Bottom()
{
    if( m_pParent == NULL ) return;

    m_pParent->SetChildOrderBottom( this );
}

void KD3DWnd::SetOrder_NextbyName( IN const char* strName_ )
{
    SetOrder_Next( m_pParent->GetChildByName( strName_ ) );
}

void KD3DWnd::SetOrder_BeforebyName( IN const char* strName_ )
{
    SetOrder_Before( m_pParent->GetChildByName( strName_ ) );
}

void KD3DWnd::SendEvent( const KActionEvent& kEvent_, bool bSendLua_ /*= true*/ )
{
    g_pkUIMgr->SendEvent( kEvent_, bSendLua_ );
}

void KD3DWnd::DisconnectAllEventProcedure()
{
    g_pkUIMgr->DisconnectAllEventProcedure( GetUID() ); 
}

void KD3DWnd::CallFunction( const char* strFunctionName, bool bPushTable /*= false */ )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L);

    if( bPushTable ) {
        if( PushTable() == false ) {
            //ASSERT( !"table is wrong!" );
        }
    }

    do 
    {
        if( !lua_istable( L, -1 ) )
            break;

        lua_getfield( L, -1, strFunctionName );
        if( lua_type( L, -1 ) != LUA_TFUNCTION ) {
            lua_pop( L, 1 );
            break;
        }
        lua_getfield( L, -3, GetWindowName().c_str() );
        if( lua_pcall( L, 1, 0, 0 ) != 0 )
        {
#ifndef __PATH__
            //  OKBOXA( lua_tostring(L, -1) , GetWindowName().c_str() );
#endif
            lua_pop(L, 1 );
        }

    } while (false);

    if( bPushTable )
        PopTable();
}

void KD3DWnd::AddChildWithRegistToLua( KD3DWnd* pChild )
{
    AddChild(pChild);
    PushTable();
    pChild->RegistToLua();
    pChild->OnLoad();
    pChild->PopTable();
}

void KD3DWnd::SetCurrentWindowLocalPos( const D3DXVECTOR2& vCurrentPos )
{
    if( m_vCurrentPos == vCurrentPos )
        return;

    m_vCurrentPos = vCurrentPos;
}


void KD3DWnd::DisconnectEventProcedure( DWORD dwCode_ )
{
    g_pkUIMgr->DisconnectEventProcedure_Code( GetUID(), dwCode_ );
    DisconnectGlobalEventProcedure( dwCode_ );
}

void KD3DWnd::DisconnectGlobalEventProcedure( DWORD dwCode_ )
{
    g_pkUIMgr->DisconnectGlobalEventProcedure( GetUID(), dwCode_ );
}

void KD3DWnd::UpdateWindowBountWithMosuePos()
{
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    SetMouseInWindow( CheckPosInWindowBound( vMousePos ) );
}

void KD3DWnd::SetHotKey( int iHotKey )
{
    if ( iHotKey < 0 )
        return;
    std::vector<int>::iterator vit = std::find( m_vecHotKey.begin(), m_vecHotKey.end(), iHotKey );
    if ( vit == m_vecHotKey.end() )
        m_vecHotKey.push_back( iHotKey );
    int iJoystickKey =0;
    if(g_kGlobalValue.GetMatchingKey(iJoystickKey, iHotKey))
    {
        SetHotJoystickKey(iJoystickKey);
    }
}
void KD3DWnd::SetHeightDirectInverse( DWORD dwHeight, DWORD dwOriHeight_ )
{
    m_dwHeight = dwHeight;
    TransformWindowInverse( m_vWindowPos, m_vWindowPos, 1, true, m_dwWidth, dwOriHeight_ );
}

void KD3DWnd::SetWidthDirectInverse( DWORD dwWidth, DWORD dwOriWidth_ )
{
    m_dwWidth = dwWidth;
    TransformWindowInverse( m_vWindowPos, m_vWindowPos, 1, true, dwOriWidth_, m_dwWidth );
}

void KD3DWnd::TransformWindowInverse( const D3DXVECTOR2& vA, const D3DXVECTOR2& vB, float t, bool bCreate, DWORD dwOriWidth_, DWORD dwOriHeight_ )
{
    if( dwOriWidth_ == -1 ) {
        dwOriWidth_ = GetWidth();
    }

    if( dwOriHeight_ == -1 ) {
        dwOriWidth_ = GetHeight();
    }

    D3DXVECTOR2 vPos;
    ::D3DXVec2Lerp( &vPos, &vA, &vB, (*m_pMotionSpline)( t ) );
    SetCurrentWindowLocalPos( vPos );

    m_vTrans = GetCurrentWindowLocalPos();

    if ( NULL != m_pParent )
        m_vTrans += m_pParent->m_vTrans;

    m_rect[0].m_vPos.x = m_vTrans.x + dwOriWidth_ - GetWidth();     m_rect[0].m_vPos.y = m_vTrans.y + dwOriHeight_ - GetHeight();
    m_rect[1].m_vPos.x = m_vTrans.x + dwOriWidth_;                  m_rect[1].m_vPos.y = m_vTrans.y + dwOriHeight_ - GetHeight();
    m_rect[2].m_vPos.x = m_vTrans.x + dwOriWidth_ - GetWidth();     m_rect[2].m_vPos.y = m_vTrans.y + dwOriHeight_;
    m_rect[3].m_vPos.x = m_vTrans.x + dwOriWidth_;                  m_rect[3].m_vPos.y = m_vTrans.y + dwOriHeight_;

    BYTE alpha = static_cast<BYTE>( 255.0f * (*m_pAlphaSpline)( bCreate ? t : ( 1.0f - t ) ) );

    for ( UINT i = 0; i < 4; ++i )
    {
        m_rect[i].m_diff = ( m_dwWndColor & D3DCOLOR_ARGB( alpha, 0xff, 0xff, 0xff ) );
    }
    SetRecalcRect( true );
}

int KD3DWnd::GetMemorySize()
{
    int nTotalSize = 0;

#if !defined( __PATH__ )

    for ( int i = 0; i < (int)m_vecChild.size(); ++i ) 
    {
        nTotalSize += m_vecChild[i]->GetMemorySize();
    }

    nTotalSize += sizeof( KD3DWnd );
#endif

    return nTotalSize;
}

int KD3DWnd::GetChildSize()
{
    int nTotalSize = 0;

#if !defined( __PATH__ )

    for ( int i = 0; i < (int)m_vecChild.size(); ++i ) 
    {
        nTotalSize += m_vecChild[i]->GetChildSize();
    }

    nTotalSize += m_vecChild.size();
#endif

    return nTotalSize;
}

void KD3DWnd::GetLogData( FILE* pLogFile )
{
#if !defined( __PATH__ )
    int nMemorySize = GetMemorySize();
    int nChildSize = GetChildSize();

    if ( pLogFile ) 
    {
        fprintf( pLogFile, "%s\t%d\t%d\t\n", this->m_strWndName.c_str(), nMemorySize, nChildSize );

        for ( int i = 0; i < (int)m_vecChild.size(); ++i ) 
        {
            m_vecChild[i]->GetLogData(pLogFile);
        }
    }    
#endif 
}

void KD3DWnd::MoveWndOnTop( KD3DWnd* ChildWnd_ )
{
	if (ChildWnd_ == NULL)
		return;

	if (m_vecChild.empty())
		return;

	std::vector<KD3DWnd*>::iterator vit = m_vecChild.begin();
	while (vit != m_vecChild.end())
	{
		if ( (*vit) == ChildWnd_ )
		{
			m_vecChild.push_back(ChildWnd_);
			m_vecChild.erase(vit);
			return;
		}
		++vit;
	}

}

void KD3DWnd::SetWndTex( ED3DWndMode eWnd_ , int iTex_ )
{
    m_aiWndTex[eWnd_] = iTex_;
}


IMPLEMENT_CLASSNAME( KD3DMoveWnd );

void KD3DMoveWnd::ReCalcReLocatedRect()
{
	for (int i=0;i<4;i++)
	{
		m_RelocatedDrawingrect[i]=m_rect[i];
	}

	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	switch(m_eAlignType)
	{
	case WNDUIAT_EXPANTION:
		for (int i=0;i<4;i++)
		{
			m_RelocatedDrawingrect[i].m_vPos.x=m_RelocatedDrawingrect[i].m_vPos.x*m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y=m_RelocatedDrawingrect[i].m_vPos.y*m_fWindowScaleY;
		}
		break;
	case WNDUIAT_POSRIGHTBASE:
		for (int i=0;i<4;i++)
		{
			//m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;
			//오른쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x=GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-m_RelocatedDrawingrect[i].m_vPos.x)* m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;
		}
		break;
	case WNDUIAT_POSRIGHTTOPBASE:
		for (int i=0;i<4;i++)
		{
			//			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;
			//오른쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x=GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-m_RelocatedDrawingrect[i].m_vPos.x) * m_fWindowScaleX;		
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;
		}
		break;
		//그냥 이동만 한다.
	case WNDUIAT_POSZEROBASE:
		for (int i=0;i<4;i++)
		{
			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;

			m_RelocatedDrawingrect[i].m_vPos.x+=m_vGapPos.x;
			m_RelocatedDrawingrect[i].m_vPos.y+=m_vGapPos.y;
		}
		break;
	case WNDUIAT_POSBOTTOMBASE:
		for (int i=0;i<4;i++)
		{
			//아래쪽에 밀착형으로 처리
			m_RelocatedDrawingrect[i].m_vPos.x*= m_fWindowScaleX;
			//m_RelocatedDrawingrect[i].m_vPos.y*= m_fWindowScaleY;

			m_RelocatedDrawingrect[i].m_vPos.y=GC_SCREEN_HEIGHT*g_pGCDeviceManager->GetWindowScaleY()-(GC_SCREEN_HEIGHT-m_RelocatedDrawingrect[i].m_vPos.y)* m_fWindowScaleY;						
			m_RelocatedDrawingrect[i].m_vPos.x+=m_vGapPos.x;
		}
		break;
	}
}

IMPLEMENT_CLASSNAME( KD3DWndActiveChild );

KD3DWnd::ED3DWndMode KD3DWndActiveChild::UpdateWndMode( void )
{
	KD3DWnd::ED3DWndMode eMode = __super::UpdateWndMode();

	for ( std::vector<KD3DWnd*>::iterator vit = m_vecChild.begin() ; vit != m_vecChild.end(); ++vit )
	{
		(*vit)->SetWndMode( eMode, false, false );
	}

	return eMode;
}