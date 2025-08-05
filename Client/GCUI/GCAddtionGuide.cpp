#include "stdafx.h"
#include "GCAddtionGuide.h"
#include "MyD3D.h"

#include <stdarg.h>

IMPLEMENT_CLASSNAME( KGCAddtionGuide );
IMPLEMENT_WND_FACTORY( KGCAddtionGuide );
IMPLEMENT_WND_FACTORY_NAME( KGCAddtionGuide, "gc_addtionguide" );

KGCAddtionGuide::KGCAddtionGuide( void )
: m_iAlpha( 0 )
, m_iShowFrame( 0 )
, m_iFadeInFrame( 0 )
, m_iFadeOutFrame( 0 )
, m_fAlphaChange( 0.0f ) 
, m_pkBack( NULL )
, m_pkStaticDesc( NULL )
, m_bIsNearPortal(false)
, m_bExperienceTutorial(false)
{

	m_eState = STATE_TUTORIAL;
	m_eAniState = OPENING;
    m_eGuideUserState = STATE_DEFAULT;

	m_eTutorialMotion = STATE_DIRECTION;

    LINK_CONTROL( "back",   m_pkBack );
	for (int i=0 ; i<MAX_KEY ; i++ )
	{
		m_arrpKey[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "key%d", i );
		LINK_CONTROL( temp, m_arrpKey[i] );

	}
    LINK_CONTROL( "static_desc", m_pkStaticDesc );

	m_pkGaugeBar = NULL;
	m_pkGauge = NULL;

	LINK_CONTROL( "gauge_bar", m_pkGaugeBar );
	LINK_CONTROL( "gauge", m_pkGauge );

	LINK_CONTROL( "help", m_pkBntHelp );
	m_vBntHelpOrginPos = D3DXVECTOR2(3,9);

	m_pkMpKeyLeft = NULL;
	m_pkApKeyLeft = NULL;
	m_pkKeyRight = NULL;
	m_pkBntExit = NULL;
	m_pkAddExplain = NULL;
	LINK_CONTROL( "mp_left", m_pkMpKeyLeft );
	LINK_CONTROL( "key_right", m_pkKeyRight );
	LINK_CONTROL( "ap_left", m_pkApKeyLeft );
	LINK_CONTROL( "exit", m_pkBntExit );

	LINK_CONTROL( "add_explain", m_pkAddExplain );

	m_bPassedTutorial = true;

	m_fMotionTimer = 0.0f;

	m_dwOrigineGaugeWidth = 77;

	m_bLockKey = false;

	m_iTutorialTempTime = 0;
}

KGCAddtionGuide::~KGCAddtionGuide( void )
{
}

void KGCAddtionGuide::ActionPerformed(const KActionEvent& event)
{

	GCWND_MSG_MAP( m_pkBntExit, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkBntHelp, KD3DWnd::D3DWE_BUTTON_CLICK, OnHelp );

	//KeyEvent();

}

void KGCAddtionGuide::OnCreate( void )
{
    m_pkBack->InitState( false );
	for (int i=0 ; i<MAX_KEY ; i++ )
	{
		m_arrpKey[i]->InitState(false,false,this);
		m_arrKeyPos[i] = m_arrpKey[i]->GetFixedWindowPos();
		m_arrKeyWidthHeigh[i].x = static_cast<float>(m_arrpKey[i]->GetWidth());
		m_arrKeyWidthHeigh[i].y = static_cast<float>(m_arrpKey[i]->GetHeight());
	}
    m_pkStaticDesc->InitState( false );
	m_pkStaticDesc->SetFontSpace(2);

	m_pkGaugeBar->InitState(false);
	m_pkGauge->SetSelfInputCheck(false);

	m_pkBntHelp->InitState(false,true,this);
	m_pkBntHelp->SetAlignType(WNDUIAT_POSBOTTOMBASE);
	m_pkBntExit->InitState(false,true,this);
	m_pkAddExplain->InitState(false);
	m_pkMpKeyLeft->InitState(false);
	m_pkKeyRight->InitState(false);
	m_pkApKeyLeft->InitState(false);

	m_vGaugeTexCoord = m_pkGauge->GetWndTexCoords();

}

UINT KGCAddtionGuide::CheckTheMon()
{
	if (m_eState != STATE_TUTORIAL)
		return -1;

#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )

	m_bCheckNearMonster = false;			
	m_bCheckNearSpecialMon = false;		//EGCMonster::MON_TRIAL_MUSHGOLEM2
	int iLoop;

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if (pMonster == NULL)
			continue;

		if( !pMonster->IsLive() )
			continue;       

		float fDist = DISTANCE( pMonster->m_afX[ LATENCY_INDEX ], pMonster->m_afY[ LATENCY_INDEX ]
		, g_MyD3D->GetMyPlayer()->vPos.x, g_MyD3D->GetMyPlayer()->vPos.y );

		iLoop = mit->first;

		if ( fDist < 0.4f )
		{
			m_bCheckNearMonster = true;
			break;
		}
	}

	MONSTER* pMonster = g_kMonsterManager.GetMonster(iLoop);
	if( pMonster  != NULL )	{
		if (pMonster->GetMonsterType() == MON_TRIAL_MUSHGOLEM2 )
			m_bCheckNearSpecialMon = true;
	}

	return iLoop;
}

void KGCAddtionGuide::CheckSpecialMonster()
{
#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )

    m_bCheckNearMonster = false;			
    m_bCheckNearSpecialMon = false;		//EGCMonster::MON_TRIAL_MUSHGOLEM2
    int iLoop;

    for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
        MONSTER *pMonster = mit->second;
        if (pMonster == NULL)
            continue;

        if( !pMonster->IsLive() )
            continue;       

        float fDist = DISTANCE( pMonster->m_afX[ LATENCY_INDEX ], pMonster->m_afY[ LATENCY_INDEX ]
        , g_MyD3D->GetMyPlayer()->vPos.x, g_MyD3D->GetMyPlayer()->vPos.y );

        iLoop = mit->first;

        if ( fDist < 0.6f )
        {
            m_bCheckNearMonster = true;
            break;
        }
    }

    MONSTER* pMonster = g_kMonsterManager.GetMonster(iLoop);
    if( pMonster  != NULL )	{
        if (pMonster->GetMonsterType() == MON_MUSHROOM)
            m_bCheckNearSpecialMon = true;
    }

    if ( m_bCheckNearMonster && m_bCheckNearSpecialMon)
    {
        ToggleRenderAll(true);
        m_iShowFrame = 318;
        m_eAniState = CHECK_MON;
        m_eState = STATE_CHECKMON;
        m_eTutorialMotion = ASDFG;
        m_pkBack->SetAllWindowAlpha( 255 );
        m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
        m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( 12034 ) );
        SetShownKey(5,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_G );
    }
    else
    {
        m_eAniState = NOTHING;
        m_eTutorialMotion = STATE_NOTHING;
        m_eState = STATE_RENDER_OFF;
        ToggleRenderAll(false);
        m_iShowFrame = 0;
    }
}

void KGCAddtionGuide::SettingScene()
{
	ToggleRenderAll( false );

	switch(m_eState)
	{
    case STATE_TUTORIAL:
    case STATE_CHECKMON:
		KeyChangeMotion();

		if (m_eAniState != NOTHING)
		{
			for (int i=0 ; i< MAX_KEY ; i++)
			{
				if (m_arrpKey[i]->IsRenderOn())
					m_arrpKey[i]->SetAllWindowAlpha( m_iAlpha );
			}
		}
		

		m_pkBack->SetAllWindowAlpha( m_iAlpha );
		m_pkBack->ToggleRender(true);
		m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(m_iAlpha, 255, 249, 217) );
		m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0) );
		m_pkStaticDesc->ToggleRender(true);
		break;
	case STATE_GAME_ON:

		m_pkApKeyLeft->SetAllWindowAlpha(m_iAlpha);
		m_pkApKeyLeft->ToggleRender(true);
		if (g_MyD3D->GetMyPlayer()->Is_Hybrid_Character())
		{
			m_pkAddExplain->SetAllWindowAlpha(m_iAlpha);
			m_pkAddExplain->ToggleRender(true);
		}
	
		m_pkBntExit->SetAllWindowAlpha(m_iAlpha);
		m_pkBntExit->ToggleRender(true);
		m_pkKeyRight->SetAllWindowAlpha(m_iAlpha);
		m_pkKeyRight->ToggleRender(true);
	case STATE_GAME_OFF:
		if (m_eAniState != ENDING)
			m_pkBntHelp->SetAllWindowAlpha(m_iAlpha);
		m_pkBntHelp->ToggleRender(true);
        break;
    case STATE_RENDER_OFF:
        if (m_eAniState == NOTHING)
        {
            for (int i=0 ; i< MAX_KEY ; i++)
            {
                if (m_arrpKey[i]->IsRenderOn())
                    m_arrpKey[i]->ToggleRender(false);
            }
        }
		break;
	}
	
}

void KGCAddtionGuide::FrameMoveInEnabledState( void )
{
	if (m_iShowFrame > -10)
		--m_iShowFrame;

	switch(m_eAniState)
	{
	case OPENING:
		if( m_iFadeInFrame < m_iShowFrame ) {
			m_iAlpha += static_cast<int>( m_fAlphaChange + 0.25f );	
		}
		else
		{
			m_eAniState = NOTHING;
			m_iAlpha = 255;
		}

		if( 255 <= m_iAlpha ) {
			m_iAlpha = 255;
		}
		break;
    case CHECK_MON:
        if( m_iFadeInFrame < m_iShowFrame ) {
            m_iAlpha += static_cast<int>( m_fAlphaChange + 0.25f );	
        }
        else
        {
            m_eAniState = NOTHING;
            m_iAlpha = 255;
        }

        if( 255 <= m_iAlpha ) {
            m_iAlpha = 255;
        }
        break;
	case ENDING:
		if( m_iFadeOutFrame > m_iShowFrame ) {
			m_iAlpha -= static_cast<int>( m_fAlphaChange + 10.0f );
		}

		if( 0 >= m_iAlpha ) { 
			OnHide();
			return;
		}
		break;
	case NOTHING:
		break;
	default:
		break;
	}

	SettingScene();
}

void KGCAddtionGuide::ToggleGauge(bool bToggle_ , int iMonIndex_ /*= -1*/)
{
	if (bToggle_)
	{
		m_pkGaugeBar->SetAllWindowAlpha(m_iAlpha)		;
		m_pkGauge->SetAllWindowAlpha(m_iAlpha);

		MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonIndex_);
		if( pMonster  != NULL )	{

			MONSTER * pMon = pMonster; 
			GCCollisionRect<float> fRect;
			if (pMon->GetAttackBoundRectNum() == 1)
				fRect = pMon->GetAttackBoundRect(0);
			else
				fRect = pMon->GetAttackBoundRect(pMon->GetAttackBoundRectNum()/2);

			D3DXVECTOR3 pos(fRect.m_Left, fRect.m_Top*0.7f,1);

			D3DXMATRIX matBasic, mat1, mat2;
			D3DXMatrixIdentity( &matBasic );
			D3DXMatrixIdentity( &mat1 );
			D3DXMatrixIdentity( &mat2 );
			D3DXMatrixTranslation( &mat1, pos.x , pos.y , 1.f );
			D3DXMatrixMultiply( &mat2, &matBasic, &mat1 );
			D3DXMatrixMultiply( &matBasic, &mat2, &g_kCamera.m_matCamera );

			D3DXVECTOR3 dxvTransPos;
			D3DXVec3TransformCoord( &dxvTransPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &matBasic );
			dxvTransPos.x += 1.0f;
			dxvTransPos.y = 0.75f - dxvTransPos.y;
			dxvTransPos.x *= 400.0f;
			dxvTransPos.y *= 400.0f;

			D3DXVECTOR2 vOriginBar = m_pkGaugeBar->GetCurrentWindowPos();
			D3DXVECTOR2 vOriginGauge = m_pkGauge->GetCurrentWindowPos();

			m_pkGaugeBar->SetWindowPosDirectFloat(dxvTransPos.x  ,dxvTransPos.y );
			m_pkGauge->SetWindowPosDirectFloat((vOriginGauge.x + (dxvTransPos.x - vOriginBar.x)) 
				,(vOriginGauge.y + (dxvTransPos.y - vOriginBar.y)) );

		}	

		SetGauge(g_MyD3D->MyCtrl->Energy / 3.0f);
		
	}

	m_pkGaugeBar->ToggleRender(bToggle_);
	m_pkGauge->ToggleRender(bToggle_);
}

void KGCAddtionGuide::ShowAddtionGuide( int iShowFrame_ )
{
	m_bLockKey = false;

    if( iShowFrame_ < 0 ) { 
        OnHide();
    }

	if (m_bPassedTutorial)
		m_eState = STATE_GAME_OFF;
	else
		m_eState = STATE_TUTORIAL;

    m_iAlpha = 0;
    m_iShowFrame = iShowFrame_;
    m_iFadeInFrame = m_iShowFrame - static_cast<int>( m_iShowFrame / 5.0f );
    m_iFadeOutFrame =  m_iShowFrame ;
    m_fAlphaChange = 255.0f / static_cast<float>( m_iShowFrame / 5.0f );

	m_pkStaticDesc->SetMultiLine( true );
    this->SetAllWindowAlpha( 0 );
    this->ToggleRender( true );
}

void KGCAddtionGuide::RegistKey(int iKey_ , int iInput_)
{
	if ( diks[iInput_] )
	{
		m_arrpKey[iKey_]->SetWndMode(D3DWM_SELECT);
	}
	else if ( Old_diks[iInput_]&& !diks[iInput_] )
	{
		m_arrpKey[iKey_]->SetWndMode(D3DWM_HOVER);
	}
	else
	{
		m_arrpKey[iKey_]->SetWndMode(D3DWM_DEFAULT);
	}
}

void KGCAddtionGuide::SetShownKey(int iCount_ , ...)
{
	for (int i=0 ; i<MAX_KEY ; i++)
		m_arrpKey[i]->ToggleRender(false);

	va_list Ap;
	va_start(Ap , iCount_);
	for (int i=0 ; i<iCount_ ; i++)
		m_arrpKey[va_arg(Ap,int)]->ToggleRender(true);

	va_end(Ap);
	
}
void KGCAddtionGuide::KeyEvent()
{
	if (m_bLockKey)
		return;

	if (!IsRenderOn())
		return;

	switch(m_eState)
	{
	case STATE_TUTORIAL:
		RegistKey(KEY_UP , DIK_UP);
		RegistKey(KEY_LEFT , DIK_LEFT);
		RegistKey(KEY_DOWN , DIK_DOWN);
		RegistKey(KEY_RIGHT , DIK_RIGHT);

		RegistKey(KEY_A , DIK_A);
		RegistKey(KEY_S , DIK_S);
		RegistKey(KEY_D , DIK_D);
		RegistKey(KEY_F , DIK_F);
		RegistKey(KEY_Z , DIK_Z);
		RegistKey(KEY_G , DIK_G);
		break;
	case STATE_GAME_OFF:
		if ( diks[DIK_F12] )
			m_eState = STATE_GAME_ON;
		break;
	case STATE_GAME_ON:
		if (diks[DIK_ESCAPE] || (diks[DIK_F12] && !Old_diks[DIK_F12]))
			m_eAniState = ENDING;
		break;
	}
}

void KGCAddtionGuide::SetGauge(float fGauge_ )
{
	DWORD dwWidth =  static_cast<DWORD>(m_dwOrigineGaugeWidth * fGauge_);
	m_pkGauge->SetWidthDirect(dwWidth);

	float fTU2 = m_vGaugeTexCoord.x + fGauge_ * ( m_vGaugeTexCoord.z - m_vGaugeTexCoord.x );
	fTU2 = static_cast<int>( fTU2 * 512.0f ) / 512.0f;
	m_pkGauge->SetWndTexCoords( D3DXVECTOR4( m_vGaugeTexCoord.x,
		m_vGaugeTexCoord.y, fTU2,
		m_vGaugeTexCoord.w ) );

}

void KGCAddtionGuide::OnHide( void )
{
    m_iAlpha = 255;
    m_iShowFrame = 0;
    m_iFadeInFrame = 0;
    m_iFadeOutFrame = 0;
    m_fAlphaChange = 0.0f;

// 	if (m_eState == STATE_TUTORIAL)
// 		m_bPassedTutorial = true;
	m_eState = STATE_GAME_OFF;
	m_eAniState = NOTHING;
   
}

void KGCAddtionGuide::ToggleRenderAll( bool bRender)
{

	m_pkBntHelp->ToggleRender(bRender);
	for (int i=0 ; i<MAX_KEY ; i++)
		m_arrpKey[i]->ToggleRender(bRender);
	m_pkBntExit->ToggleRender(bRender);

	m_pkBack->ToggleRender(bRender);
	m_pkStaticDesc->ToggleRender(bRender);
	m_pkGaugeBar->ToggleRender(bRender);
	m_pkGauge->ToggleRender(bRender);

	m_pkMpKeyLeft->ToggleRender(bRender);
	m_pkApKeyLeft->ToggleRender(bRender);
	m_pkAddExplain->ToggleRender(bRender);
	m_pkKeyRight->ToggleRender(bRender);

}

void KGCAddtionGuide::IinitAtGoWorldMap()
{
	m_iAlpha = 0;
	m_iShowFrame = 0;
	m_iFadeInFrame = 0;
	m_iFadeOutFrame = 0;
	m_fAlphaChange = 0.0f;

	m_eState = STATE_GAME_OFF;

	m_eAniState = OPENING;

	m_eTutorialMotion = STATE_DIRECTION;

	ToggleRenderAll( false );

}

void KGCAddtionGuide::OnExit()
{
	m_eAniState = ENDING;
}

void KGCAddtionGuide::OnHelp()
{
	m_eAniState = OPENING;
	m_eState = STATE_GAME_ON;
}

void KGCAddtionGuide::KeyChangeMotion()
{
	//InitKey();
	ToggleGauge(false);
	UINT iMonsterIndex = CheckTheMon();

	if (!m_bCheckNearSpecialMon)
		m_iTutorialTempTime = 0;

	switch(m_eTutorialMotion)
	{
	case DIRECTION_Z:
		LerpForMotion(KEY_UP,KEY_Z);
		LerpForMotion(KEY_LEFT,KEY_Z);
		LerpForMotion(KEY_DOWN,KEY_Z);
		LerpForMotion(KEY_RIGHT,KEY_Z);
		CanToSetChangMotion(STATE_Z);
		SetShownKey(5,KEY_Z, KEY_UP , KEY_LEFT , KEY_DOWN , KEY_RIGHT );
		break;
	case DIRECTION_ASDF:
		LerpForMotion(KEY_UP,KEY_A);
		LerpForMotion(KEY_LEFT,KEY_S);
		LerpForMotion(KEY_DOWN,KEY_D);
		LerpForMotion(KEY_RIGHT,KEY_F);
		LerpForMotion(NONE_KEY,KEY_G);
		CanToSetChangMotion(STATE_ASDF);
		SetShownKey(9,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_G , KEY_UP , KEY_LEFT , KEY_DOWN , KEY_RIGHT );
		break;
	case ASDF_DIRECTION:
		LerpForMotion(KEY_A,KEY_UP);
		LerpForMotion(KEY_S,KEY_LEFT);
		LerpForMotion(KEY_D,KEY_DOWN);
		LerpForMotion(KEY_F,KEY_RIGHT);
		LerpForMotion(KEY_G,NONE_KEY);
		CanToSetChangMotion(STATE_DIRECTION);
		SetShownKey(9,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_G, KEY_UP , KEY_LEFT , KEY_DOWN , KEY_RIGHT );
		break;
	case ASDF_Z:
		LerpForMotion(KEY_A,KEY_Z);
		LerpForMotion(KEY_S,KEY_Z);
		LerpForMotion(KEY_D,KEY_Z);
		LerpForMotion(KEY_F,KEY_Z);
		LerpForMotion(KEY_G,KEY_Z);
		CanToSetChangMotion(STATE_Z);
		SetShownKey(6,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_G, KEY_Z);
		break;
	case Z_DIRECTION:
		LerpForMotion(KEY_Z,KEY_DOWN);
		LerpForMotion(KEY_LEFT,KEY_LEFT,KEY_DOWN);
		LerpForMotion(KEY_RIGHT,KEY_RIGHT,KEY_DOWN);
		LerpForMotion(KEY_UP,KEY_UP,KEY_DOWN);
		CanToSetChangMotion(STATE_DIRECTION);
		SetShownKey(5,KEY_UP , KEY_LEFT , KEY_DOWN , KEY_RIGHT ,KEY_Z);
		break;
	case Z_ASDF:
		LerpForMotion(KEY_Z,KEY_A);
		LerpForMotion(KEY_S,KEY_S,KEY_A);
		LerpForMotion(KEY_D,KEY_D,KEY_A);
		LerpForMotion(KEY_F,KEY_F,KEY_A);
		LerpForMotion(KEY_G,KEY_G,KEY_A);
		CanToSetChangMotion(STATE_ASDF);
		SetShownKey(6,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_Z , KEY_G);
		break;
    case ASDFG:
        LerpForMotion(KEY_A,KEY_Z);
        LerpForMotion(KEY_S,KEY_Z);
        LerpForMotion(KEY_D,KEY_Z);
        LerpForMotion(KEY_F,KEY_Z);
        LerpForMotion(KEY_G,KEY_Z);
        CanToSetChangMotion(STATE_ASDF);
        SetShownKey(5,KEY_A , KEY_S , KEY_D , KEY_F , KEY_G);
        break;
	case STATE_DIRECTION:
		if (m_bCheckNearMonster)
		{
			if (m_bCheckNearSpecialMon)
			{
				m_eTutorialMotion = DIRECTION_ASDF;
			}
			else
			{
				m_eTutorialMotion = DIRECTION_Z;
			}
			break;
		}

		if (m_bMonsterisInTheDungeon)
		{
            if ( m_bIsNearPortal == true )
            {
                m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_TUTORIAL_GUIDE_SYSTEM_MSG_2 ) );
            }
            else
            {
#if defined (USE_NEWBIE_GUIDE)
                m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_TUTORIAL_GUIDE_SYSTEM_MSG_1 ) );
#else
				m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE1 ) );
#endif
            }
		}
		else
		{
			m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE5 ) );
		}
		SetShownKey(4,KEY_UP , KEY_LEFT , KEY_DOWN , KEY_RIGHT);
		break;
	case STATE_Z:
		if (m_bCheckNearMonster)
		{
			if (m_bCheckNearSpecialMon)
			{

				m_eTutorialMotion = Z_ASDF;
				break;

			}
		}
		else
		{
			m_eTutorialMotion = Z_DIRECTION;
			break;
		}
		if (m_bCheckNearSpecialMon)			
		{
			ToggleGauge(true,iMonsterIndex);
			m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE3 ) );
		}
		else
		{
			m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE2 ) );
		}
		SetShownKey(1,KEY_Z);
		break;
	case STATE_ASDF:
		if (m_bCheckNearMonster)
		{
			if (m_bCheckNearSpecialMon)
			{

				if ( m_iTutorialTempTime >= MIDDLE_TERM_TIME)
				{
					m_eTutorialMotion = ASDFG;
					m_iTutorialTempTime = 0;
				}
				else
				{
					m_iTutorialTempTime++;
				}

			}
			else
			{
				m_eTutorialMotion = ASDFG;
				break;
			}
		}
		else
		{
			m_eTutorialMotion = ASDF_DIRECTION;
			break;
		}

		if ( g_MyD3D->GetMyPlayer()->Is_Hybrid_Character() )
			m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE6 ) );
		else
			m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ADDTIONGUIDE4 ) );
		SetShownKey(5,KEY_A , KEY_S , KEY_D , KEY_F ,KEY_G);
		break;
    case STATE_NOTHING:
        break;
	}



}

void KGCAddtionGuide::LerpForMotion(int iSetKey_ ,int iGoalKey_ , int iStartKey_ )
{
// 	D3DXVECTOR2 vOutPos;
// 	D3DXVECTOR2 vOutWidthHeigh;
// 	if (iStartKey_ == NONE_KEY )
// 	{
//		if (iSetKey_ != NONE_KEY)
// 		{
//	 		vOutWidthHeigh = D3DXVECTOR2(static_cast<float>(m_arrpKey[iSetKey_]->GetWidth()),
// 				static_cast<float>(m_arrpKey[iSetKey_]->GetHeight()));
// 			vOutPos = m_arrKeyPos[iSetKey_];
// 		}
// 	}
// 	else
// 	{
// 		vOutWidthHeigh = D3DXVECTOR2(static_cast<float>(m_arrpKey[iStartKey_]->GetWidth()),
// 			static_cast<float>(m_arrpKey[iStartKey_]->GetHeight()));
// 		vOutPos = m_arrKeyPos[iStartKey_];	
// 	}
// 
// 	D3DXVec2Lerp( &vOutPos , &vOutPos , &m_arrKeyPos[iGoalKey_] , m_fMotionTimer);
// 	D3DXVec2Lerp( &vOutWidthHeigh , &vOutWidthHeigh , &m_arrKeyWidthHeigh[iGoalKey_] , m_fMotionTimer);
// 	m_arrpKey[iSetKey_]->SetWindowPosDirect(vOutPos);
// 	m_arrpKey[iSetKey_]->SetWidthDirect(static_cast<DWORD>(vOutWidthHeigh.x));
// 	m_arrpKey[iSetKey_]->SetHeightDirect(static_cast<DWORD>(vOutWidthHeigh.y));
// 
// 	m_arrpKey[iGoalKey_]->SetWindowPosDirect(vOutPos);
// 	m_arrpKey[iGoalKey_]->SetWidthDirect(static_cast<DWORD>(vOutWidthHeigh.x));
// 	m_arrpKey[iGoalKey_]->SetHeightDirect(static_cast<DWORD>(vOutWidthHeigh.y));

	int iAlpha = static_cast<int>(m_fMotionTimer*255);
	if (iSetKey_ != NONE_KEY)
		m_arrpKey[iSetKey_]->SetAllWindowAlpha(255 - iAlpha);
	if (iGoalKey_ != NONE_KEY)
		m_arrpKey[iGoalKey_]->SetAllWindowAlpha(iAlpha);
}

void KGCAddtionGuide::InitKey()
{
	for (int i=0 ; i<MAX_KEY ; i++)
	{
		m_arrpKey[i]->SetWindowPos(m_arrKeyPos[i]);
		m_arrpKey[i]->SetWidth(static_cast<DWORD>(m_arrKeyWidthHeigh[i].x));
		m_arrpKey[i]->SetHeight(static_cast<DWORD>(m_arrKeyWidthHeigh[i].y));
	}
}

void KGCAddtionGuide::CanToSetChangMotion(TUTORIAL_MOTION eMotion_ )
{
	if (m_fMotionTimer >= 1.0f)
	{
		m_eTutorialMotion = eMotion_;
		m_fMotionTimer = 0.0f;
		return;
	}
	else
	{
		m_fMotionTimer += 0.05f;
	}
}

void KGCAddtionGuide::OnInitialize()
{
	m_pkBntHelp->SetWindowScale(g_pkGameOverlayUI->Getdv2GameUIScale());
	D3DXVECTOR2 vPos(m_vBntHelpOrginPos);
	vPos.y -= ( GC_SCREEN_HEIGHT * ( m_fWindowScaleY/g_pkGameOverlayUI->Getdv2GameUIScale().y - 1) );
	m_pkBntHelp->SetAllWindowPos(vPos);
}

void KGCAddtionGuide::SetNearPortal( bool bIsNearPortal)
{
    m_bIsNearPortal = bIsNearPortal;
}

void KGCAddtionGuide::EndTrialForestDungeon()
{
    KP2P::GetInstance()->Send_TutorialClearNOT( );
    m_eGuideUserState= STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT;
    int iPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayer ) )
    {
        // 갈려면 니가 소환했던 몬스터는 정리하고 가라~
        g_MyD3D->MyPlayer[iPlayer]->SetSummonMonsterHP_SendHost( 0.0f, -1, true );
#if defined( DUNGEONPLAYSTATE )
        // 던전플레이통계( 강제종료 )
        g_kGlobalValue.AddDungeonPlayState( KGCGlobal::DUNPLAY_STATE_LEAVE );
        g_MyD3D->MyPlayer[iPlayer]->SendDungeonPlayState( KGCGlobal::DUNPLAY_STATE_LEAVE );
#endif
    }

    g_MyD3D->m_pStateMachine->OnExitGame();
}

void KGCAddtionGuide::SetGuideUserState( int eState)
{
    m_eGuideUserState = eState;
}

int KGCAddtionGuide::GetGuideUserState()
{
    return m_eGuideUserState;
}