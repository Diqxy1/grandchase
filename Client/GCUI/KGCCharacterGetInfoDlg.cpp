#include "stdafx.h"
#include "KGCCharGetInfoPromotion.h"
#include "KGCCharGetInfoPanel.h"
#include "GCPromotionChangePanel.h"
#include "KGCCharacterGetInfoDlg.h"
#include "KGCCharacterName.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCharacterGetInfoDlg );
IMPLEMENT_WND_FACTORY( KGCCharacterGetInfoDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterGetInfoDlg, "gc_character_get_info_dlg" );

KGCCharacterGetInfoDlg::KGCCharacterGetInfoDlg( void ) 
{
    m_pkCharacterFrame = NULL;
    m_pkInfoFrame = NULL;
    LINK_CONTROL( "character_frame", m_pkCharacterFrame );
    LINK_CONTROL( "info_frame", m_pkInfoFrame );

    m_pkClassGetMent = NULL;
    LINK_CONTROL( "class_get_ment", m_pkClassGetMent );

    for( int i = 0 ; i < NUM_TITLE_EFFECT ; ++i )
    {
        m_apkTitleEffect[i] = NULL;

        char szTemp[MAX_PATH];

        if( i < NUM_TITLE_EFFECT / 2 )
            sprintf_s(szTemp, MAX_PATH - 1, "effect_L%d", i);
        else
            sprintf_s(szTemp, MAX_PATH - 1, "effect_R%d", i - (NUM_TITLE_EFFECT / 2 ) );

        LINK_CONTROL( szTemp, m_apkTitleEffect[i] );
    }


    m_pkPromotion = NULL;
    LINK_CONTROL( "promotion_change_ment", m_pkPromotion );
    m_pkNewChar = NULL;
    LINK_CONTROL( "character_get_ment", m_pkNewChar );
    m_pkCharGetInfo = NULL;
    LINK_CONTROL( "character_get_info", m_pkCharGetInfo );
    m_pkPromotionChange = NULL;
    LINK_CONTROL( "promotion_change_info", m_pkPromotionChange );

    m_bShow = false;
    m_bNewChar = false;
    m_iCharType = -1;
    m_iPromotion = -1;
    m_dwTimeCheck = 0;

	m_pkImgDrawRect = NULL;
	LINK_CONTROL( "img_draw_rect", m_pkImgDrawRect );

    m_pkCharacterNameTexture = NULL;
    LINK_CONTROL("character_name_img", m_pkCharacterNameTexture);

    m_pkContratulation = NULL;
    LINK_CONTROL("congratulation_ment", m_pkContratulation);
}


KGCCharacterGetInfoDlg::~KGCCharacterGetInfoDlg( void )
{

}

void KGCCharacterGetInfoDlg::OnCreate( void )
{
    m_pkCharacterFrame->ToggleRender( false );
    m_pkInfoFrame->ToggleRender( false );
    m_pkClassGetMent->ToggleRender( false );

    for( int i = 0 ; i < NUM_TITLE_EFFECT ; ++i )
        m_apkTitleEffect[i]->ToggleRender( false );

    m_pkPromotion->ToggleRender( false );
    m_pkNewChar->ToggleRender( false );
    m_pkCharGetInfo->ToggleRender( false );
    m_pkPromotionChange->ToggleRender( false );
    m_pkCharacterNameTexture->ToggleRender( false );
    if (m_pkContratulation)
        m_pkContratulation->ToggleRender( false );
}

void KGCCharacterGetInfoDlg::OnCreateComplete( void )
{
    LoadScript();
}

void KGCCharacterGetInfoDlg::OnDestroy( void )
{
    
    int iChar, iJob, iEnd = GC_CHAR_NUM * NUM_JOB_LEVEL;
    for(int iLoop=0; iLoop<iEnd; ++iLoop)
    {
        iChar = iLoop % GC_CHAR_NUM;
        iJob = iLoop / GC_CHAR_NUM;        

        if( g_kGlobalValue.IsOpenedChar(iChar) == false ||
            g_kGlobalValue.IsOpenedPromotion( iChar, iJob ) == false )
            continue;    

        SAFE_RELEASE(m_SpecialFace[iChar][iJob].pTex);
    }
}

void KGCCharacterGetInfoDlg::FrameMoveInEnabledState( void ){}

void KGCCharacterGetInfoDlg::PostChildDraw( void )
{
    if( m_iCharType < 0 || m_iCharType > GC_CHAR_NUM || m_iPromotion < 0 || m_iPromotion > NUM_JOB_LEVEL-1 ) { 
        return;
    }

    if( false == m_bShow ) { 
        return;
    }

    RenderFace( m_SpecialFace[m_iCharType][m_iPromotion].vecWndPos, m_iCharType, m_iPromotion, false );

    if( m_bNewChar )
        m_pkNewChar->Render();
    else
        m_pkPromotion->Render();

    if (m_pkContratulation)
        m_pkContratulation->Render();
    
    m_pkCharacterNameTexture->Render();

    ++m_dwTimeCheck;
    if( ( GC_FPS_LIMIT * 4) < m_dwTimeCheck )
    {
        m_bShow = false;
        m_dwTimeCheck = 0;
        SetShow(m_bShow);
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
        g_kGlobalValue.m_bShowItemInfoBox = false;
    }
}

void KGCCharacterGetInfoDlg::SetInfo(bool bNewChar, int iCharType, int iPromotion)
{
    if( iCharType < 0 || iCharType > GC_CHAR_NUM || iPromotion < 0 || iPromotion > NUM_JOB_LEVEL-1 )
        return;

    m_bNewChar = bNewChar;
    m_iCharType = iCharType;
    m_iPromotion = iPromotion;    

    if( m_bNewChar )
        m_pkCharGetInfo->SetNewCharInfo(m_iCharType);
    else
        m_pkPromotionChange->SetPromotionInfo(m_iPromotion);

    m_pkCharacterNameTexture->SetCharacterInfo(m_iCharType, m_iPromotion);
}

void KGCCharacterGetInfoDlg::SetShow(bool bShow)
{
    this->ToggleRender( bShow );

    m_pkCharacterFrame->ToggleRender( bShow );
    m_pkInfoFrame->ToggleRender( bShow );

    for( int i = 0 ; i < NUM_TITLE_EFFECT ; ++i )
        m_apkTitleEffect[i]->ToggleRender( bShow );

    //캐릭터 이름 개선
    if( m_bNewChar )
    {
        m_pkNewChar->ToggleRender(bShow);
        m_pkCharacterNameTexture->ToggleRender(bShow);
        m_pkCharGetInfo->SetShow(bShow);
    }
    else
    {
        
        m_pkCharacterNameTexture->ToggleRender(bShow);
#ifdef DECANEE
        if ( m_iCharType == GC_CHAR_ASIN || m_iCharType == GC_CHAR_LIME || m_iCharType == GC_CHAR_DECANEE) {
#else
        if (m_iCharType == GC_CHAR_ASIN || m_iCharType == GC_CHAR_LIME) {
#endif
            m_pkPromotionChange->SetShow(false);
            m_pkPromotion->ToggleRender(false);
            m_pkClassGetMent->ToggleRender( true);
            if (m_pkContratulation)
                m_pkContratulation->ToggleRender( false );
        }
        else { 
            m_pkPromotion->ToggleRender(true);
            m_pkClassGetMent->ToggleRender( false);
            m_pkPromotionChange->SetShow(bShow);
            if (m_pkContratulation)
                m_pkContratulation->ToggleRender( true );
        }
    }

    if( bShow )
    {
        m_dwTimeCheck = 0;

        if( m_bNewChar )
            g_KDSound.Play( "New_Character_Get" );
        else
            g_KDSound.Play( "Promotion_Change" );            
    }

    m_bShow = bShow;
}

void KGCCharacterGetInfoDlg::LoadScript() 
{
    // 필살기 스크립트 및 텍스쳐 로드
    char strFileName[MAX_PATH];
    int iChar, iJob, iEnd = GC_CHAR_NUM * NUM_JOB_LEVEL;
    for(int iLoop=0; iLoop<iEnd; ++iLoop)
    {
        iChar = iLoop % GC_CHAR_NUM;
        iJob = iLoop / GC_CHAR_NUM;

        //진(0,2,3), 지크하트(3)는 버닝모드로 이미지로 처리
        if( (GC_CHAR_JIN == iChar) && (0 == iJob) )
        {   sprintf( strFileName, "playertemplate7_0_burning_0.lua" ); }
        else if( (GC_CHAR_JIN == iChar) && (2 == iJob) )
        {   sprintf( strFileName, "playertemplate7_2_burning_0.lua" ); }
        else if( (GC_CHAR_JIN == iChar) && (3 == iJob) )
        {   sprintf( strFileName, "playertemplate7_3_burning_0.lua" ); }
        else if( (GC_CHAR_SIEG == iChar) && (3 == iJob) )
        {   sprintf( strFileName, "playertemplate8_3_burning_0.lua" ); }
        else
        {   sprintf( strFileName, "PlayerTemplate%d_%d.lua", iChar, iJob ); }

        if( !LoadSpecialFaceScript( strFileName, &m_SpecialFace[iChar][iJob] ) )
        {
            ASSERT("PlayerTemplate야~ 어딧니?");
        }

        if( g_kGlobalValue.IsOpenedChar(iChar) == false ||
            g_kGlobalValue.IsOpenedPromotion( iChar, iJob ) == false )
            continue;    

        m_SpecialFace[iChar][iJob].pTex = g_pGCDeviceManager2->CreateTexture( m_SpecialFace[iChar][iJob].strTexName );
    }
}

bool KGCCharacterGetInfoDlg::LoadSpecialFaceScript( std::string strFileName, SpecialFace *pSpecialFace)
{
    KLuaManager luaMgr;
    if( !GCFUNC::LoadLuaScript( luaMgr, strFileName, false ) )
    {  return false;   }

    LUA_BEGIN_TABLE("Template", return false)
    {
        LUA_BEGIN_TABLE( "SpecialFaceTex", goto NoSpecialFace )
        {
            LUA_GET_VALUE( "Texture" , pSpecialFace->strTexName, return false);

            LUA_BEGIN_TABLE( "GetNewCharPos", return false )
            {
                float fx, fy, fz, fw;
                LUA_GET_VALUE( 1, fx, return false );
                LUA_GET_VALUE( 2, fy, return false );
                LUA_GET_VALUE( 3, fz, return false );
                LUA_GET_VALUE( 4, fw, return false );

                pSpecialFace->vecWndPos.x = fx;
                pSpecialFace->vecWndPos.y = fy;
                pSpecialFace->vecWndPos.z = fz;
                pSpecialFace->vecWndPos.w = fw;
            }
            LUA_END_TABLE( return false );
            LUA_BEGIN_TABLE( "TutorialTexUV", return false )
            {
                LUA_GET_VALUE( 1, pSpecialFace->vecUVLeft.x, return false );
                LUA_GET_VALUE( 2, pSpecialFace->vecUVLeft.y, return false );
                LUA_GET_VALUE( 3, pSpecialFace->vecUVRight.x, return false );
                LUA_GET_VALUE( 4, pSpecialFace->vecUVRight.y, return false );
            }
            LUA_END_TABLE( return false );
        }
        LUA_END_TABLE( goto NoSpecialFace );
    }
    goto EndSpecialFace;

NoSpecialFace:
    LUA_END_TABLE(return false);
    return false;

EndSpecialFace:
    LUA_END_TABLE(return false);
    return true;
}

void KGCCharacterGetInfoDlg::RenderFace(D3DXVECTOR4 vecTempPos, int iChar, int iJob, bool bReverse)
{
    if( m_SpecialFace[ iChar ][ iJob ].pTex == NULL ) {
        return;
    }

	if ( m_pkImgDrawRect == NULL ) return;

    D3DXVECTOR4 vecTempUV = D3DXVECTOR4(m_SpecialFace[iChar][iJob].vecUVLeft.x, m_SpecialFace[iChar][iJob].vecUVLeft.y,
        m_SpecialFace[iChar][iJob].vecUVRight.x, m_SpecialFace[iChar][iJob].vecUVRight.y);

    g_pGCDeviceManager2->DrawInScreenResize(m_SpecialFace[iChar][iJob].pTex,
        vecTempPos.x, vecTempPos.y,
        vecTempPos.z, vecTempPos.w,
        PIXELTOTEXEL(vecTempUV.x),
        PIXELTOTEXEL(vecTempUV.y),
        PIXELTOTEXEL(vecTempUV.z), 
        PIXELTOTEXEL(vecTempUV.w) );
}
