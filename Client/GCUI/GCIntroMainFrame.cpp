#include "stdafx.h"
#include "GCIntroMainFrame.h"
#include "KMci.h"

IMPLEMENT_CLASSNAME( KGCIntroMainFrame );
IMPLEMENT_WND_FACTORY( KGCIntroMainFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCIntroMainFrame, "gc_intro_main_frame" );

KGCIntroMainFrame::KGCIntroMainFrame( void )
{
	m_pkStaticDifficult = NULL;
	LINK_CONTROL( "static_difficult", m_pkStaticDifficult );

	m_pkStaticSetDifficult = NULL;
	LINK_CONTROL( "static_set_difficult", m_pkStaticSetDifficult );

	for (int i=0 ; i<DFIFFICULT_MAX_LEVEL ; i++ )
	{
		m_pkDifficultStar[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "difficult_star%d", i );
		LINK_CONTROL( temp, m_pkDifficultStar[i] );
	}


	for (int i=0 ; i<GC_CHAR_NUM ; i++ )
	{
		m_pkImgCharName[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "char_name%d", i );
		LINK_CONTROL( temp, m_pkImgCharName[i] );

		m_pkCharImage[i] = NULL;
		sprintf( temp, "char_image%d", i );
		LINK_CONTROL( temp, m_pkCharImage[i] );
	}

	m_pkStaticWeapon= NULL;
	LINK_CONTROL( "static_weapon", m_pkStaticWeapon );

	m_pkStaticCharDesc= NULL;
	LINK_CONTROL( "static_desc", m_pkStaticCharDesc );

	m_pkMovieDrawRect= NULL;
	LINK_CONTROL( "movie_draw_rect", m_pkMovieDrawRect );

	m_pkBtnMoviePlay= NULL;
	LINK_CONTROL( "btn_movie_play", m_pkBtnMoviePlay );

	m_iCharType = -1;

    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        m_pkPromotionImage[i] = NULL;
        char temp[MAX_PATH];

        if( i<NUM_JOB_LEVEL ) { 
            sprintf_s( temp, MAX_PATH-1, "job_%d", i );
            LINK_CONTROL( temp, m_pkPromotionImage[i] );
        }
        else { 
            LINK_CONTROL( "job_1_2", m_pkPromotionImage[i] );
        }
    }

    for( int i=0 ; i<NUM_JOB_LEVEL-1 ; ++i ) { 
        m_pkPromotionStep[i] = NULL;
        char temp[MAX_PATH];
        sprintf_s( temp, MAX_PATH-1, "job_step%d", i );
        LINK_CONTROL( temp, m_pkPromotionStep[i] );
    }
}

KGCIntroMainFrame::~KGCIntroMainFrame( void )
{
}

void KGCIntroMainFrame::ActionPerformed( const KActionEvent& event )
{
	if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN)
	{
			
		if (event.m_pWnd == m_pkBtnMoviePlay)
		{
			PlayMovie();
		}
	}

}

void KGCIntroMainFrame::OnCreate( void )
{
	m_pkStaticDifficult->InitState(true);
	m_pkStaticSetDifficult->InitState(true);
	
	for (int i=0 ; i<DFIFFICULT_MAX_LEVEL ; i++)
	{
		m_pkDifficultStar[i]->InitState(true);
	}


	for (int i=0 ; i<GC_CHAR_NUM ; i++)
	{
		m_pkImgCharName[i]->InitState(false);
		m_pkCharImage[i]->InitState(false);
	}

	m_pkStaticWeapon->InitState(true);
	m_pkStaticCharDesc->InitState(true);
	m_pkMovieDrawRect->InitState(true);

#if !defined(NATION_PHILIPPINE)
	m_pkBtnMoviePlay->InitState(true,true,this);
#endif

	m_pkStaticDifficult->SetText(g_pkStrLoader->GetString( STR_ID_CHARACTER_DIFFICULT ));
#if defined(NATION_USA) || defined(NATION_PHILIPPINE)

#else
	m_pkStaticDifficult->SetFontSpace( 1 );
#endif

    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        m_pkPromotionImage[i]->InitState( false );
    }

    for( int i=0 ; i<NUM_JOB_LEVEL-1 ; ++i ) { 
        m_pkPromotionStep[i]->InitState( false );
    }
}

void KGCIntroMainFrame::SetIntroMainFrame(const int& iCharType_ )
{
	if (iCharType_<0 || iCharType_>=GC_CHAR_NUM)
		return;
	m_iCharType = iCharType_;
	SetDifficultLevel_();
	SetJopAndPromotionName();
	SetCharDesc();
	SetCharWeaponInfo();
	PlayMovie();
}

void KGCIntroMainFrame::SetDifficultLevel_()
{
	for (int i=0 ; i<DFIFFICULT_MAX_LEVEL ; i++)
	{
		m_pkDifficultStar[i]->SetWndMode(D3DWM_DEFAULT);
	}

	int iDifficultLevel = g_kGlobalValue.GetCharDifficuleLevel(m_iCharType);
	
	if (iDifficultLevel >= DFIFFICULT_MAX_LEVEL || iDifficultLevel<0)
		return;

	switch(iDifficultLevel)
	{
	case 0:
		m_pkStaticSetDifficult->SetText(g_pkStrLoader->GetString(STR_ID_DIFFICULT_LOW));
		break;
	case 1:
		m_pkStaticSetDifficult->SetText(g_pkStrLoader->GetString(STR_ID_DIFFICULT_NORMAL));
		break;
	case 2:
		m_pkStaticSetDifficult->SetText(g_pkStrLoader->GetString(STR_ID_DIFFICULT_HIGH));
		break;
	}
	for (int i=0 ; i<= iDifficultLevel ; i++)
	{
		m_pkDifficultStar[i]->SetWndMode(D3DWM_HOVER);
	}

}

void KGCIntroMainFrame::SetJopAndPromotionName()
{
	for (int i=0 ; i<GC_CHAR_NUM ; i++)
	{
		m_pkImgCharName[i]->ToggleRender(false);
		m_pkCharImage[i]->ToggleRender(false);
	}
    m_pkImgCharName[m_iCharType]->ToggleRender(true);
    m_pkCharImage[m_iCharType]->ToggleRender(true);

    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        m_pkPromotionImage[i]->ToggleRender( false );
    }

    for( int i=0 ; i<NUM_JOB_LEVEL-1 ; ++i ) { 
        m_pkPromotionStep[i]->ToggleRender( false );
    }

    for( int i=0 ; i<NUM_JOB_LEVEL ; ++i ) { 
        if( g_kGlobalValue.IsOpenedPromotion( m_iCharType, i ) ) { 
#ifdef DECANEE
            if( (m_iCharType == GC_CHAR_ASIN || m_iCharType == GC_CHAR_LIME || m_iCharType == GC_CHAR_DECANEE) && i>0 ) {
#else
			if ((m_iCharType == GC_CHAR_ASIN || m_iCharType == GC_CHAR_LIME) && i > 0) {
#endif
                m_pkPromotionImage[NUM_JOB_LEVEL]->ToggleRender( true );
                m_pkPromotionImage[NUM_JOB_LEVEL]->SetToolTip( g_kGlobalValue.GetJopName( m_iCharType, i ) );
            }
            else {
                m_pkPromotionImage[i]->ToggleRender( true );
                m_pkPromotionImage[i]->SetToolTip( g_kGlobalValue.GetJopName( m_iCharType, i ) );
            }

            if( i > 0 ) { 
                m_pkPromotionStep[i-1]->ToggleRender( true );
            }
        }
    }
}

void KGCIntroMainFrame::ReadyToPlayMovie()
{
    D3DXVECTOR2 vPos(GetFixedWindowPos());
    vPos += m_pkMovieDrawRect->GetFixedWindowLocalPos();
    vPos.x *= m_fWindowScaleX;
    vPos.y *= m_fWindowScaleY;

	

    g_KMci.GetShowForMovie().SetVideoWinPos(vPos, static_cast<DWORD>(m_pkMovieDrawRect->GetWidth()*m_fWindowScaleX), static_cast<DWORD>(m_pkMovieDrawRect->GetHeight()*m_fWindowScaleY));

}

void KGCIntroMainFrame::SetCharDesc()
{

	switch(m_iCharType)
	{
	case GC_CHAR_ELESIS:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR0_DESC));
		return;
	case GC_CHAR_LIRE:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR1_DESC));
		return;
	case GC_CHAR_ARME:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR2_DESC));
		return;
	case GC_CHAR_LAS:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR3_DESC));
		return;
	case GC_CHAR_RYAN:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR4_DESC));
		return;
	case GC_CHAR_RONAN:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR5_DESC));
		return;
	case GC_CHAR_AMY:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR6_DESC));
		return;
	case GC_CHAR_JIN:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR7_DESC));
		return;
	case GC_CHAR_SIEG:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR8_DESC));
		return;
	case GC_CHAR_MARI:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR9_DESC));
		return;
	case GC_CHAR_DIO:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR10_DESC));
		return;
	case GC_CHAR_ZERO:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR11_DESC));
		return;
	case GC_CHAR_LEY:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR12_DESC));
		return;
	case GC_CHAR_LUPUS:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR13_DESC));
		return;
	case GC_CHAR_RIN:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR14_DESC));
		return;
	case GC_CHAR_ASIN:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR15_DESC));
		return;
    case GC_CHAR_LIME:
        m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR16_DESC));
        return;
    case GC_CHAR_EDEL:
        m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR17_DESC));
        return;
    case GC_CHAR_BEIGAS:
        m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR18_DESC));
        return;
	case GC_CHAR_UNO:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR19_DESC));
		return;
		return;
#ifdef PERYTON // Kawan>
	case GC_CHAR_PERYTON:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR20_DESC));
		return;
#endif
#ifdef ALICE // Kawan>
	case GC_CHAR_ALICE:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR21_DESC));
		return;
#endif
#ifdef DECANEE // Kawan>
	case GC_CHAR_DECANEE:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR22_DESC));
		return;
#endif
	default:
		m_pkStaticCharDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_CHAR16_DESC));
		return;
	}
}

void KGCIntroMainFrame::FrameMoveInEnabledState()
{
#if !defined( NATION_PHILIPPINE )
	if (!g_KMci.GetShowForMovie().IsPlaying())
		m_pkBtnMoviePlay->ToggleRender(true);
#endif
}


void KGCIntroMainFrame::PlayMovie()
{
	if (m_iCharType<0 || m_iCharType>=GC_CHAR_NUM )
		return;
	ReadyToPlayMovie();
	g_KMci.GetShowForMovie().Play(m_iCharType,false);
	m_pkBtnMoviePlay->ToggleRender(false);

}

void KGCIntroMainFrame::SetCharWeaponInfo()
{
	switch(m_iCharType)
	{
	case GC_CHAR_ELESIS:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR0_WEAPON));
		return;
	case GC_CHAR_LIRE:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR1_WEAPON));
		return;
	case GC_CHAR_ARME:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR2_WEAPON));
		return;
	case GC_CHAR_LAS:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR3_WEAPON));
		return;
	case GC_CHAR_RYAN:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR4_WEAPON));
		return;
	case GC_CHAR_RONAN:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR5_WEAPON));
		return;
	case GC_CHAR_AMY:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR6_WEAPON));
		return;
	case GC_CHAR_JIN:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR7_WEAPON));
		return;
	case GC_CHAR_SIEG:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR8_WEAPON));
		return;
	case GC_CHAR_MARI:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR9_WEAPON));
		return;
	case GC_CHAR_DIO:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR10_WEAPON));
		return;
	case GC_CHAR_ZERO:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR11_WEAPON));
		return;
	case GC_CHAR_LEY:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR12_WEAPON));
		return;
	case GC_CHAR_LUPUS:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR13_WEAPON));
		return;
	case GC_CHAR_RIN:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR14_WEAPON));
		return;
	case GC_CHAR_ASIN:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR15_WEAPON));
		return;
	case GC_CHAR_LIME:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR16_WEAPON));
        return;
    case GC_CHAR_EDEL:
        m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR17_WEAPON));
		return;
    case GC_CHAR_BEIGAS:
        m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR18_WEAPON));
        return;
	case GC_CHAR_UNO:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR19_WEAPON));
		return;
#ifdef PERYTON // Kawan>
	case GC_CHAR_PERYTON:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR20_WEAPON));
		return;
#endif
#ifdef ALICE // Kawan>
	case GC_CHAR_ALICE:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR21_WEAPON));
		return;
#endif
#ifdef DECANEE // Kawan>
	case GC_CHAR_DECANEE:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR22_WEAPON));
		return;
#endif
	default:
		m_pkStaticWeapon->SetText(g_pkStrLoader->GetString(STR_ID_CHAR16_WEAPON));
		return;
	}

}
