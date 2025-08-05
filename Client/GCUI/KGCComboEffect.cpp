#include "stdafx.h"
#include ".\kgccomboeffect.h"
#include "MyD3D.h"
#include "Headup Display.h"
#include "..\KGCSubjectManager.h"





IMPLEMENT_CLASSNAME( KGCComboEffect );
IMPLEMENT_WND_FACTORY( KGCComboEffect );
IMPLEMENT_WND_FACTORY_NAME( KGCComboEffect, "gc_on_combo_effect" );

#if defined( SEASON3_COMBO_SYSTEM )

#define GC_CE_SCALE_FRAME           (int)(GC_FPS_LIMIT*0.3f)
#define GC_CE_SHOW_FRAME            (int)(GC_FPS_LIMIT*3.0f)
#define GC_CEBG_POS_FRAME           (int)(GC_FPS_LIMIT*0.25f)
#define GC_CEBG_SHOW_FRAME          (int)(GC_FPS_LIMIT*3.2f)
#define GC_CE_SHOW_FRAME_BACK       (int)(GC_FPS_LIMIT*3.5f)
#define GC_CE_SHOW_FRAME_BACK_SE    (int)(GC_FPS_LIMIT*3.0f)
#define GC_CEBG_POS_HIDE_INTERVAL   (500)
#define GC_CE_MAX_SCALE_BIG         400
#define GC_CE_MAX_SCALE_SMALL       300
#define GC_CE_MIN_SACLE             100
#define GC_CMOBO_GOOD               10
#define GC_CMOBO_GREAT              15
#define GC_CMOBO_WONDERFUL          20
#define GC_CMOBO_PERFECT            25

#else

#define GC_CE_SCALE_FRAME           (int)(GC_FPS_LIMIT*0.3f)
#define GC_CE_SHOW_FRAME            (int)(GC_FPS_LIMIT*1.2f)
#define GC_CEBG_POS_FRAME           (int)(GC_FPS_LIMIT*0.25f)
#define GC_CEBG_SHOW_FRAME          (int)(GC_FPS_LIMIT*1.5f)
#define GC_CE_SHOW_FRAME_BACK       (int)(GC_FPS_LIMIT*1.7f)
#define GC_CE_SHOW_FRAME_BACK_SE    (int)(GC_FPS_LIMIT*1.2f)
#define GC_CEBG_POS_HIDE_INTERVAL   (500)
#define GC_CE_MAX_SCALE_BIG         400
#define GC_CE_MAX_SCALE_SMALL       300
#define GC_CE_MIN_SACLE             100
#define GC_CMOBO_GOOD               10
#define GC_CMOBO_GREAT              15
#define GC_CMOBO_WONDERFUL          20
#define GC_CMOBO_PERFECT            25
#define GC_COMBO_PERFECT_NEXT       10

#endif


KGCComboEffect::KGCComboEffect(void)
{
    m_bIniValue                 = false;
    m_pkGood                    = NULL;
    m_pkGreat                   = NULL;
    m_pkWonderful               = NULL;
    m_pkPerfect                 = NULL;
    m_pkBreak                   = NULL;
    m_pkBackYellow              = NULL;
    m_pkBackRed                 = NULL;
    m_pkGood_AfterImage         = NULL;
    m_pkGreat_AfterImage        = NULL;
    m_pkWonderful_AfterImage    = NULL;
    m_pkPerfect_AfterImage      = NULL;
    m_pkBreak_AfterImage        = NULL;
#if !defined( SEASON3_COMBO_SYSTEM )
    m_iPerfectCombo             = GC_CMOBO_PERFECT;
#endif

    LINK_CONTROL( "combo_back_red",             m_pkBackRed );
    LINK_CONTROL( "combo_back_yellow",          m_pkBackYellow );
    LINK_CONTROL( "combo_perfect",              m_pkPerfect );
    LINK_CONTROL( "combo_wonderful",            m_pkWonderful );
    LINK_CONTROL( "combo_good",                 m_pkGood );
    LINK_CONTROL( "combo_great",                m_pkGreat );
    LINK_CONTROL( "combo_break",                m_pkBreak );
    LINK_CONTROL( "combo_good_afterimage",      m_pkGood_AfterImage );    
    LINK_CONTROL( "combo_great_afterimage",     m_pkGreat_AfterImage );    
    LINK_CONTROL( "combo_wonderful_afterimage", m_pkWonderful_AfterImage );    
    LINK_CONTROL( "combo_perfect_afterimage",   m_pkPerfect_AfterImage );    
    LINK_CONTROL( "combo_break_afterimage",     m_pkBreak_AfterImage );


    m_enComboEffect         = EGC_CE_NONE;
    m_enComboEffectBG       = EGC_CEBG_NONE;
    m_bComboStarted         = false;
    m_iOldCombo             = 0;

    for(int i=0;i<NUM_OF_EGC_CE;++i)
        m_iComboState[i] = 0;
}

KGCComboEffect::~KGCComboEffect(void)
{
}

void KGCComboEffect::InitComboEffect(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow)
{
    if( !pEffectInfo || !pWindow ) return;

    pWindow->SetSelfInputCheck(false);
    pWindow->ToggleRender(false);
    if( !m_bIniValue )
    {
        pEffectInfo->InitValue( (int)pWindow->GetFixedWindowLocalPos().x, (int)pWindow->GetFixedWindowLocalPos().y
                                , (int)pWindow->GetWidth(), (int)pWindow->GetHeight() );
    }
    else
    {
        pWindow->SetFixedWindowLocalPos( pEffectInfo->GetOriginalPos() );
        pWindow->SetWidthDirect(pEffectInfo->GetOriginalWidth());
        pWindow->SetHeightDirect(pEffectInfo->GetOriginalHeight());
    }
}

void KGCComboEffect::OnCreate( void )
{
    InitComboEffect(&m_stGood,                      m_pkGood);
    InitComboEffect(&m_stGreat,                     m_pkGreat);
    InitComboEffect(&m_stWonderful,                 m_pkWonderful);
    InitComboEffect(&m_stPerfect,                   m_pkPerfect);
    InitComboEffect(&m_stBreak,                     m_pkBreak);
    InitComboEffect(&m_stBackYellow,                m_pkBackYellow);
    InitComboEffect(&m_stBackRed,                   m_pkBackRed);
    InitComboEffect(&m_stGood_AfterImage,           m_pkGood_AfterImage);
    InitComboEffect(&m_stGreat_AfterImage,          m_pkGreat_AfterImage);
    InitComboEffect(&m_stWonderful_AfterImage,      m_pkWonderful_AfterImage);
    InitComboEffect(&m_stPerfect_AfterImage,        m_pkPerfect_AfterImage);
    InitComboEffect(&m_stBreak_AfterImage,          m_pkBreak_AfterImage);

    if( !m_bIniValue ) m_bIniValue = true;
}

void KGCComboEffect::OnDestroy( void )
{

}

void KGCComboEffect::CheckComboState(void)
{
    // 콤보를 보여주고 있는 상태인 경우 콤보이펙트를 처리한다.
    // 아니라면 콤보 이펙트 정보를 초기화한다.
    if( g_MyD3D->MyHead->ShowCombo )
    {
        m_bComboStarted = true;
#if !defined( SEASON3_COMBO_SYSTEM )
        if( m_iComboState[EGC_CE_GOOD] < 3 )
            m_iComboState[EGC_CE_GOOD] += (GC_CMOBO_GOOD <= g_MyD3D->MyHead->Combo_Count);

        if( m_iComboState[EGC_CE_GREAT] < 3 )
            m_iComboState[EGC_CE_GREAT] += (GC_CMOBO_GREAT <= g_MyD3D->MyHead->Combo_Count);

        if( m_iComboState[EGC_CE_WONDERFUL] < 3 )
            m_iComboState[EGC_CE_WONDERFUL] += (GC_CMOBO_WONDERFUL <= g_MyD3D->MyHead->Combo_Count);

        if( m_iComboState[EGC_CE_PERFECT] < 3 )
            m_iComboState[EGC_CE_PERFECT] += (m_iPerfectCombo <= g_MyD3D->MyHead->Combo_Count);

        // 콤보가 25 이상이고, 25 이후 10단위로 퍼펙트 띄워준다.
        if((g_MyD3D->MyHead->Combo_Count-m_iPerfectCombo) >= 0
            && !IsTimeToComboStart(EGC_CE_PERFECT)
            )
        {
            m_iComboState[EGC_CE_PERFECT] = 1;
            m_iPerfectCombo         += GC_COMBO_PERFECT_NEXT;
        }
#else
        int iCombo = g_MyD3D->MyHead->Combo_Count - m_iOldCombo;
        if ( iCombo > 0 && iCombo <= 2 )
        {
            m_iComboState[EGC_CE_GOOD] = 1;
        }
        else if ( iCombo > 2 && iCombo <= 4 )
        {
            m_iComboState[EGC_CE_WONDERFUL] = 1;
        }
        else if ( iCombo > 4 )
        {
            m_iComboState[EGC_CE_PERFECT] = 1;
        }
#endif
        // 실제 콤보 수에 따라 숫자 이펙트 다르게!!
    }
    else
    {
        if( m_bComboStarted )
        {
            m_bComboStarted = false;

            for(int i=0;i<NUM_OF_EGC_CE;++i)
                m_iComboState[i] = 0;

#if !defined( SEASON3_COMBO_SYSTEM )
            m_iPerfectCombo = GC_CMOBO_PERFECT;
            m_iComboState[EGC_CE_BREAK] = 1;
#else
            m_iComboState[EGC_CE_BREAK] = 1;
#endif
        }
    }

    m_iOldCombo = g_MyD3D->MyHead->Combo_Count;

    //************************************************************************
    // 콤보들이 사용할 수 있는 상태인지 확인하고, 사용한다.
    //************************************************************************
    if( IsTimeToComboStart(EGC_CE_GOOD) )
    {
        HideComboEffect();
        HideComboEffectBG();
        SetShowComboEffect(EGC_CE_GOOD, GC_CE_SCALE_FRAME, GC_CE_SHOW_FRAME, GC_CE_MAX_SCALE_BIG, GC_CE_MIN_SACLE);

        m_iComboState[EGC_CE_GOOD]++;
    }

    if( IsTimeToComboStart(EGC_CE_GREAT) )
    {
        HideComboEffect();
        HideComboEffectBG();
        SetShowComboEffect(EGC_CE_GREAT, GC_CE_SCALE_FRAME, GC_CE_SHOW_FRAME, GC_CE_MAX_SCALE_BIG, GC_CE_MIN_SACLE);

        m_iComboState[EGC_CE_GREAT]++;
    }

    if( IsTimeToComboStart(EGC_CE_WONDERFUL) )
    {
        HideComboEffect();
        HideComboEffectBG();
        SetShowComboEffectBG(EGC_CEBG_BACKYELLOW, GC_CEBG_POS_FRAME, GC_CE_SHOW_FRAME_BACK
            , D3DXVECTOR2(-GC_CEBG_POS_HIDE_INTERVAL, (float)m_stBackYellow.GetOriginalPos().y)
            , m_stBackYellow.GetOriginalPos() );

        m_iComboState[EGC_CE_WONDERFUL]++;
    }

    if( IsTimeToComboStart(EGC_CE_PERFECT) )
    {
        HideComboEffect();
        HideComboEffectBG();
        SetShowComboEffectBG(EGC_CEBG_BACKRED, GC_CEBG_POS_FRAME, GC_CE_SHOW_FRAME_BACK
            , D3DXVECTOR2(-GC_CEBG_POS_HIDE_INTERVAL, (float)m_stBackRed.GetOriginalPos().y)
            , m_stBackRed.GetOriginalPos() );

        m_iComboState[EGC_CE_PERFECT]++;
    }

    if( IsTimeToComboStart(EGC_CE_BREAK) )
    {
        HideComboEffect();
        HideComboEffectBG();
        //#if defined( _DEBUG )
        //	KLuaManager luaMgr;
        //	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        //
        //	int scaleFrame = 0, showFrame = 0, minSacle = 0, maxScale = 0;
        //	LUA_GET_VALUE_DEF( "scaleFrame", scaleFrame, 1 );
        //	LUA_GET_VALUE_DEF( "showFrame", showFrame, 1 );
        //    LUA_GET_VALUE_DEF( "maxScale", maxScale, 1 );
        //	LUA_GET_VALUE_DEF( "minScale", minSacle, 1 );        	
        //#endif
        
        SetShowComboEffect(EGC_CE_BREAK, 10, 30, GC_CE_MAX_SCALE_BIG, GC_CE_MIN_SACLE);
        m_iComboState[EGC_CE_BREAK]++;
    }
}


void KGCComboEffect::FrameMoveInEnabledState( void )
{
    CheckComboState();

    // 콤보 이펙트용 FrameMove
    switch(m_enComboEffect)
    {
        case EGC_CE_GOOD:
            Good();
            break;
        case EGC_CE_GREAT:
            Great();
            break;
        case EGC_CE_WONDERFUL:
            Wonderful();
            break;
        case EGC_CE_PERFECT:
            Perfect();
            break;
        case EGC_CE_BREAK:
            Break();
            break;
        default:
            break;
    }

    // 콤보 이펙트 배경용 FrameMove
    switch( m_enComboEffectBG )
    {
        case EGC_CEBG_BACKYELLOW:
            BackYellow();
            break;
        case EGC_CEBG_BACKRED:
            BackRed();
            break;
        default:
            break;
    }
}

void KGCComboEffect::PostChildDraw( void )
{

}

void KGCComboEffect::Show_Scale_Blend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow)
{
    if( !pEffectInfo || !pWindow ) return;

    // 콤보이펙트의 표시 여부를 확인하는 부분
    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // 콤보 이펙트의 스케일을 진행하는 부분
        if( !pEffectInfo->IsEndOfScale() )
        {
            pEffectInfo->ScaleStep();

            // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함...
            const float fChangeX = (float)(pEffectInfo->GetCurrentWidth()/2 - pEffectInfo->GetOriginalWidth()/2);
            const float fChangeY = (float)(pEffectInfo->GetCurrentHeight()/2 - pEffectInfo->GetOriginalHeight()/2);

            pWindow->SetWindowPosDirect( pEffectInfo->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));
            pWindow->SetWidthDirect( pEffectInfo->GetCurrentWidth() );
            pWindow->SetHeightDirect( pEffectInfo->GetCurrentHeight() );
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 123, 0xff, 0xff, 0xff));
        }
        else
        {
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff));
        }
        pWindow->ToggleRender(true);
    }
    else if( !pEffectInfo->IsEndOfBlend() )
    {
        pEffectInfo->BlendStep();
        pWindow->SetWindowColor(D3DCOLOR_ARGB( (int)pEffectInfo->GetCurrentBlend(), 0xff, 0xff, 0xff));
    }
    
    if( pEffectInfo->IsEndOfBlend() )
    {
        HideComboEffect();
        HideComboEffectBG();
    }
}

void KGCComboEffect::Show_ScaleWidthBlend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow)
{
    if( !pEffectInfo || !pWindow ) return;

    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // 콤보 이펙트의 스케일을 진행하는 부분
        if( !pEffectInfo->IsEndOfScale() )
        {
            pEffectInfo->ScaleStep();

            // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함...
            const float fChangeX = (float)(pEffectInfo->GetCurrentWidth()/2 - pEffectInfo->GetOriginalWidth()/2);
            const float fChangeY = (float)(pEffectInfo->GetCurrentHeight()/2 - pEffectInfo->GetOriginalHeight()/2);

            pWindow->SetWindowPosDirect( pEffectInfo->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));
            pWindow->SetWidthDirect((int)pEffectInfo->GetCurrentWidth());
            pWindow->SetHeightDirect((int)pEffectInfo->GetCurrentHeight());
        }

        if( !pEffectInfo->IsEndOfBlend() )
        {
            pEffectInfo->BlendStep();
            pWindow->SetWindowColor(D3DCOLOR_ARGB( (int)pEffectInfo->GetCurrentBlend(), 0xff, 0xff, 0xff));
        }

        pWindow->ToggleRender(true);
    }

    if( pEffectInfo->IsEndOfShow() )
    {
        HideComboEffect();
        HideComboEffectBG();
    }
}

void KGCComboEffect::Show_Pos_ComboEffect_Blend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow, EGCCOMBOEFFECT enCalledComboEffect)
{
    // 콤보이펙트의 표시 여부를 확인하는 부분
    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // 콤보 이펙트의 스케일을 진행하는 부분
        if( !pEffectInfo->IsEndOfPos() )
        {
            pEffectInfo->PosStep();
            pWindow->SetWindowPosDirect(pEffectInfo->GetCurrentPos());

            if( pEffectInfo->IsEndOfPos() )
                SetShowComboEffect(enCalledComboEffect, GC_CE_SCALE_FRAME, GC_CE_SHOW_FRAME_BACK_SE, GC_CE_MAX_SCALE_SMALL, GC_CE_MIN_SACLE);
        }

        pWindow->ToggleRender(true);
    }
    else if( !pEffectInfo->IsEndOfBlend() )
    {
        if( m_stWonderful.IsEndOfShow() )
        {
            pEffectInfo->BlendStep();
            pWindow->SetWindowColor(D3DCOLOR_ARGB( (int)pEffectInfo->GetCurrentBlend(), 0xff, 0xff, 0xff));
        }
    }
    
    if( pEffectInfo->IsEndOfBlend() )
    {
        HideComboEffect();
        HideComboEffectBG();
    }
}

void KGCComboEffect::Good(void)
{
    Show_Scale_Blend(&m_stGood, m_pkGood);

    if( m_stGood.IsEndOfScale() ) Good_AfterImage();
    if( m_stGood_AfterImage.IsEndOfShow() ) m_enComboEffect = EGC_CE_NONE;
}

void KGCComboEffect::Good_AfterImage()
{
    Show_ScaleWidthBlend(&m_stGood_AfterImage, m_pkGood_AfterImage);
}

void KGCComboEffect::Great(void)
{
    Show_Scale_Blend(&m_stGreat, m_pkGreat);

    if( m_stGreat.IsEndOfScale() ) Great_AfterImage();
    if( m_stGreat_AfterImage.IsEndOfShow() ) m_enComboEffect = EGC_CE_NONE;
}

void KGCComboEffect::Great_AfterImage()
{
    Show_ScaleWidthBlend(&m_stGreat_AfterImage, m_pkGreat_AfterImage);
}

void KGCComboEffect::Wonderful(void)
{
    Show_Scale_Blend(&m_stWonderful, m_pkWonderful);

    if( m_stWonderful.IsEndOfScale() ) Wonderful_AfterImage();
    if( m_stWonderful_AfterImage.IsEndOfShow() ) m_enComboEffect = EGC_CE_NONE;
}

void KGCComboEffect::Wonderful_AfterImage()
{
    Show_ScaleWidthBlend(&m_stWonderful_AfterImage, m_pkWonderful_AfterImage);
}

void KGCComboEffect::Perfect(void)
{
    Show_Scale_Blend(&m_stPerfect, m_pkPerfect);

    if( m_stPerfect.IsEndOfScale() ) Perfect_AfterImage();
    if( m_stPerfect_AfterImage.IsEndOfShow() ) m_enComboEffect = EGC_CE_NONE;
}

void KGCComboEffect::Perfect_AfterImage()
{
    Show_ScaleWidthBlend(&m_stPerfect_AfterImage, m_pkPerfect_AfterImage);
}

void KGCComboEffect::Break(void)
{
    Show_Scale_Blend(&m_stBreak, m_pkBreak);

    if( m_stBreak.IsEndOfScale() ) Break_AfterImage();
    if( m_stBreak_AfterImage.IsEndOfShow() ) m_enComboEffect = EGC_CE_NONE;
}

void KGCComboEffect::Break_AfterImage()
{
    SiKGCSubjectManager()->SetComboBreak();
    Show_ScaleWidthBlend(&m_stBreak_AfterImage, m_pkBreak_AfterImage);
}

void KGCComboEffect::BackYellow(void)
{
    Show_Pos_ComboEffect_Blend(&m_stBackYellow, m_pkBackYellow, EGC_CE_WONDERFUL);

    if( m_stBackYellow.IsEndOfBlend() ) m_enComboEffectBG = EGC_CEBG_NONE;
}

void KGCComboEffect::BackRed(void)
{
    Show_Pos_ComboEffect_Blend(&m_stBackRed, m_pkBackRed, EGC_CE_PERFECT);
    
    if( m_stBackRed.IsEndOfBlend() ) m_enComboEffectBG = EGC_CEBG_NONE;
}

void KGCComboEffect::SetShowComboEffect(EGCCOMBOEFFECT enComboEffect, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent)
{
    if ( !g_kGlobalValue.m_bComboEffectShow ) 
        return;

    KGCD3DWndEffect   *pEffect        = NULL;
    KD3DWnd             *pkWindow       = NULL;
    KGCD3DWndEffect   *pEffect_AfterImage  = NULL;
    KD3DWnd             *pkWindow_AfterImage = NULL;

    switch(enComboEffect)
    {
    case EGC_CE_GOOD:
        {
            pEffect                 = &m_stGood;
            pkWindow                = m_pkGood;
            pEffect_AfterImage      = &m_stGood_AfterImage;
            pkWindow_AfterImage     = m_pkGood_AfterImage;
        }
        break;
    case EGC_CE_GREAT:
        {
            pEffect                 = &m_stGreat;
            pkWindow                = m_pkGreat;
            pEffect_AfterImage      = &m_stGreat_AfterImage;
            pkWindow_AfterImage     = m_pkGreat_AfterImage;
        }
        break;
    case EGC_CE_WONDERFUL:
        {
            pEffect                 = &m_stWonderful;
            pkWindow                = m_pkWonderful;
            pEffect_AfterImage      = &m_stWonderful_AfterImage;
            pkWindow_AfterImage     = m_pkWonderful_AfterImage;
        }
        break;
    case EGC_CE_PERFECT:
        {
            pEffect                 = &m_stPerfect;
            pkWindow                = m_pkPerfect;
            pEffect_AfterImage      = &m_stPerfect_AfterImage;
            pkWindow_AfterImage     = m_pkPerfect_AfterImage;

            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("perfect01", -0.55f, 0.0f, 0.5F), GC_LAYER_UI);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("perfect02", -0.55f, 0.0f, 0.5F), GC_LAYER_UI);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("perfect03", -0.55f, 0.0f, 0.5F), GC_LAYER_UI);
        }
        break;
    case EGC_CE_BREAK:
        {
            pEffect                 = &m_stBreak;
            pkWindow                = m_pkBreak;
            pEffect_AfterImage      = &m_stBreak_AfterImage;
            pkWindow_AfterImage     = m_pkBreak_AfterImage;
        }
        break;
    default:
        break;
    }

    if( pEffect && pkWindow && pEffect_AfterImage && pkWindow_AfterImage )
    {
        pEffect->SetFramePerAlterationScale(iScaleFrame, iShowFrame, iStartScalePercent, iDestScalePercent);
        pEffect->SetFramePerAlterationBlend(iScaleFrame, iShowFrame, 255, 0);

        // 처음 시작 값 설정
        pkWindow->SetWidthDirect((DWORD)pEffect->GetCurrentWidth());
        pkWindow->SetHeightDirect((DWORD)pEffect->GetCurrentHeight());
        
        // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함... 
        const float fChangeX = (float)(pEffect->GetCurrentWidth()/2 - pEffect->GetOriginalWidth()/2);
        const float fChangeY = (float)(pEffect->GetCurrentHeight()/2 - pEffect->GetOriginalHeight()/2);
        pkWindow->SetWindowPosDirect( pEffect->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));

        pEffect_AfterImage->SetFramePerAlterationBlend(iScaleFrame, iShowFrame, 255, 0);
        pEffect_AfterImage->SetFramePerAlterationScale(iScaleFrame, iShowFrame, 100, 140);
        pkWindow_AfterImage->SetWindowPosDirect(pEffect_AfterImage->GetOriginalPos());
    }

    m_enComboEffect     = enComboEffect;
}

void KGCComboEffect::SetShowComboEffectBG(EGCCOMBOEFFECT_BG enComboEffectBG, int iPosFrame, int iShowFrame, D3DXVECTOR2 vStartPos, D3DXVECTOR2 vEndPos)
{
    if ( !g_kGlobalValue.m_bComboEffectShow ) 
        return;

    KGCD3DWndEffect   *pEffect        = NULL;
    KD3DWnd             *pkWindow       = NULL;

    switch(enComboEffectBG)
    {
    case EGC_CEBG_BACKYELLOW:
        pEffect        = &m_stBackYellow;
        pkWindow       = m_pkBackYellow;
        break;
    case EGC_CEBG_BACKRED:
        pEffect        = &m_stBackRed;
        pkWindow       = m_pkBackRed;
        break;
    default:
        break;
    }

    if( pEffect && pkWindow )
    {
        pEffect->SetFramePerAlterationPos(iPosFrame, iShowFrame, vStartPos, vEndPos);
        pkWindow->SetWindowPosDirect(vStartPos);
        pEffect->SetFramePerAlterationBlend(iPosFrame, iShowFrame, 255, 0);
        pkWindow->SetWindowColor(D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff));
    }

    m_enComboEffectBG = enComboEffectBG;
}

void KGCComboEffect::HideComboEffect(void)
{
    m_pkGood->ToggleRender(false);
    m_pkGreat->ToggleRender(false);
    m_pkWonderful->ToggleRender(false);
    m_pkPerfect->ToggleRender(false);
    m_pkGood_AfterImage->ToggleRender(false);
    m_pkGreat_AfterImage->ToggleRender(false);
    m_pkWonderful_AfterImage->ToggleRender(false);
    m_pkPerfect_AfterImage->ToggleRender(false);
    m_pkBreak->ToggleRender(false);
    m_pkBreak_AfterImage->ToggleRender(false);

    m_stGood.SetComplete();
    m_stGreat.SetComplete();
    m_stWonderful.SetComplete();
    m_stPerfect.SetComplete();
    m_stGood_AfterImage.SetComplete();
    m_stGreat_AfterImage.SetComplete();
    m_stWonderful_AfterImage.SetComplete();
    m_stPerfect_AfterImage.SetComplete();
    m_stBreak.SetComplete();
    m_stBreak_AfterImage.SetComplete();

    m_enComboEffect     = EGC_CE_NONE;
    m_enComboEffectBG   = EGC_CEBG_NONE;
}

void KGCComboEffect::HideComboEffectBG(void)
{
    m_pkBackYellow->ToggleRender(false);
    m_pkBackRed->ToggleRender(false);

    m_stBackYellow.SetComplete();
    m_stBackRed.SetComplete();
    
    m_enComboEffect     = EGC_CE_NONE;
    m_enComboEffectBG   = EGC_CEBG_NONE;
}
