#include "stdafx.h"
#include "GCSphinxFace.h"

IMPLEMENT_CLASSNAME( KGCSphinxFace );
IMPLEMENT_WND_FACTORY( KGCSphinxFace );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxFace, "gc_sphinx_face" );

KGCSphinxFace::KGCSphinxFace( void )
: m_pkSphinxFace(NULL)
{
}

KGCSphinxFace::~KGCSphinxFace( void )
{
}

void KGCSphinxFace::ActionPerformed( const KActionEvent& event )
{

}

void KGCSphinxFace::OnCreate( void )
{

}

void KGCSphinxFace::SetSphinxFace( int iState )
{
    if(m_pkSphinxFace == NULL){
        m_pkSphinxFace = new KGC2DObject();
    }
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    //float x,y;

    //LUA_GET_VALUE_DEF( "x", x, 0.0f );
    //LUA_GET_VALUE_DEF( "y", y, 0.0f );

    m_pkSphinxFace->RemoveAllTexture();
    char	cBuf[256];
    memset(cBuf,'\0',256);
    sprintf(cBuf,"event_sphinx_face_%d.dds",iState);

    m_pkSphinxFace->AddTexture(g_pGCDeviceManager2->CreateTexture(cBuf));
    m_pkSphinxFace->SetAxisOffset( 0.0f, 0.0f,0.0f );
    m_pkSphinxFace->SetSize( 250 , 250  );
    m_pkSphinxFace->CameraMatrixOn();
    m_pkSphinxFace->SetPositionPixel( 300.0f * GC_SCREEN_DIV_WIDTH, 255.0f * GC_SCREEN_DIV_WIDTH );

    if(KGCSphinxDlg::SEFS_ANGRY == iState) //유져가 이긴 상황
    {
        ViewWinPaticle();
    }
    else if(KGCSphinxDlg::SEFS_MOCK == iState) // 유져가 진 상황
    {
        ViewLosePaticle();
    }
}



void KGCSphinxFace::PostDraw( )
{
    if(m_pkSphinxFace)
    {
        m_pkSphinxFace->Render();
    }
}

void KGCSphinxFace::RemoveFaceTexture()
{
    m_pkSphinxFace->RemoveAllTexture();

}

void KGCSphinxFace::ViewWinPaticle()
{
    g_KDSound.Play( "Enchant_Start" );

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_04", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_05", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_06", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_07", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Lose", 0.75f, 0.0f, 0.5f ) , GC_LAYER_UI);
}

void KGCSphinxFace::ViewLosePaticle()
{
    g_KDSound.Play( "94");

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_01", 0.75f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_02", 0.75f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_03", 0.75f, 0.0f, 0.5f ) , GC_LAYER_UI);

}