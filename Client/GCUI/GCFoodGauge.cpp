#include "stdafx.h"
#include "GCFoodGauge.h"
#include "DamageManager.h"
#include "KGCSyncObject.h"

IMPLEMENT_CLASSNAME( KGCFoodGauge );
IMPLEMENT_WND_FACTORY( KGCFoodGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCFoodGauge, "gc_food_gauge" );

KGCFoodGauge::KGCFoodGauge( void )
: m_pkFoodNumStr( NULL )
, m_pkFoodNumBack( NULL )
, m_pkGetFoodNumStr( NULL )
, m_pkRedGauge( NULL )
, m_pkBlueGauge( NULL )
, m_iMaxFood( 0 )
, m_iFoodEnum( -1 )
, m_iFoodType( -1 )
, m_iGetFoodEnum( -1 )
, m_iGetFoodType( -1 )
, m_iOldFoodCnt( -1 )
, m_iOldGetFoodCnt( -1 )
, m_bFoodGaugeFirst( true )
{
    LINK_CONTROL( "food_num_str", m_pkFoodNumStr );
    LINK_CONTROL( "food_num_back", m_pkFoodNumBack );
    LINK_CONTROL( "get_food_num_str", m_pkGetFoodNumStr );
    LINK_CONTROL( "red_gauge", m_pkRedGauge );
    LINK_CONTROL( "blue_gauge", m_pkBlueGauge );

    char szTemp[128];
    for(int iLoop=0; iLoop<10; ++iLoop)
    {
        m_pkNumber10[iLoop] = NULL;
        sprintf(szTemp, "num10_%d", iLoop);
        LINK_CONTROL( szTemp, m_pkNumber10[iLoop] );

        m_pkNumber1[iLoop] = NULL;
        sprintf(szTemp, "num1_%d", iLoop);
        LINK_CONTROL( szTemp, m_pkNumber1[iLoop] );
    }
}

KGCFoodGauge::~KGCFoodGauge( void )
{
}

void KGCFoodGauge::InitData()
{
    m_iMaxFood = 0;
    m_iFoodEnum = -1;
    m_iFoodType = -1;
    m_iGetFoodEnum = -1;
    m_iGetFoodType = -1;
    m_iOldFoodCnt = -1;
    m_iOldGetFoodCnt = -1;
}

void KGCFoodGauge::OnCreate( void )
{
    if( m_pkFoodNumBack )
        m_pkFoodNumBack->InitState( false );
    if( m_pkFoodNumStr )
    {
        m_pkFoodNumStr->InitState( false );
        m_pkFoodNumStr->SetAlign( DT_CENTER );
        m_pkFoodNumStr->SetFontColor( 0xffffffff );
        m_pkFoodNumStr->SetFontOutline( true, 0xff000000 );
    }
    if( m_pkGetFoodNumStr )
    {
        m_pkGetFoodNumStr->InitState( false );
        m_pkGetFoodNumStr->SetAlign( DT_CENTER );
        m_pkGetFoodNumStr->SetFontColor( 0xffffffff );
        m_pkGetFoodNumStr->SetFontOutline( true, 0xff000000 );
    }
    if( m_pkRedGauge )
        m_pkRedGauge->InitState( false );
    if( m_pkBlueGauge )
        m_pkBlueGauge->InitState( false );

    if( m_bFoodGaugeFirst )
    {
        g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_BUFF_FOODCHANGE, this, &KGCFoodGauge::UpDateGauge );
        m_bFoodGaugeFirst = false;
    }

    for(int iLoop=0; iLoop<10; ++iLoop)
    {
        if( m_pkNumber10[iLoop] )
            m_pkNumber10[iLoop]->InitState( false );

        if( m_pkNumber1[iLoop] )
            m_pkNumber1[iLoop]->InitState( false );
    }

    InitData();
}

void KGCFoodGauge::UpDateGauge( void )
{
    if( NULL == m_pkFoodNumBack || NULL == m_pkGetFoodNumStr || NULL == m_pkFoodNumStr || NULL == m_pkRedGauge || NULL == m_pkBlueGauge )
        return;

    int iFoodCnt=0, iGetFoodCnt=0;
    switch( m_iFoodType )
    {
    case TYPE_FOOD_DAMAGE:
        iFoodCnt = g_MyD3D->m_pDamageManager->GetDamageInstanceCount( m_iFoodEnum, 1 );
        break;
    case TYPE_FOOD_PLAYER_BUFF:
        iFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iFoodEnum, true );
        break;
    case TYPE_FOOD_MONSTER_BUFF:
        iFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iFoodEnum, false );
        break;
    case TYPE_FOOD_PLAYER_MONSTER_BUFF:
        iFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iFoodEnum, true );
        iFoodCnt += g_pMagicEffect->GetBuffInstanceCount( m_iFoodEnum, false );
    case TYPE_FOOD_SYNCOBJECT:
        iFoodCnt = SiKGCSyncObjectManager()->GetInstanceCountToID( m_iFoodEnum );
        break;
    } 

    switch( m_iGetFoodType )
    {
    case TYPE_GETFOOD_DAMAGE:
        iGetFoodCnt = g_MyD3D->m_pDamageManager->GetDamageInstanceCount( m_iGetFoodEnum, 1 );
        break;
    case TYPE_GETFOOD_PLAYER_BUFF:
        iGetFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iGetFoodEnum, true );
        break;
    case TYPE_GETFOOD_MONSTER_BUFF:
        iGetFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iGetFoodEnum, false );
        break;
    case TYPE_GETFOOD_PLAYER_MONSTER_BUFF:
        iGetFoodCnt = g_pMagicEffect->GetBuffInstanceCount( m_iGetFoodEnum, true );
        iGetFoodCnt += g_pMagicEffect->GetBuffInstanceCount( m_iGetFoodEnum, false );
        break;
    case TYPE_GETFOOD_SYNCOBJECT:
        iFoodCnt = SiKGCSyncObjectManager()->GetInstanceCountToID( m_iFoodEnum );
        break;
    }

    if( (0 < iGetFoodCnt) && (iGetFoodCnt > m_iOldGetFoodCnt) )
    {
        if( 0 > (--iFoodCnt) )
            iFoodCnt = 0;
    }

    bool bShowParticle = false;
    if( m_iOldFoodCnt != iFoodCnt || m_iOldGetFoodCnt != iGetFoodCnt )
        bShowParticle = true;

    m_iOldFoodCnt = iFoodCnt;
    m_iOldGetFoodCnt = iGetFoodCnt;

    D3DXVECTOR2 vPos;
    float fOneFoodSize = float(MAX_BAR_SIZE) / float(m_iMaxFood);
    int iAllFoodWidth = int((iFoodCnt+iGetFoodCnt) * fOneFoodSize);
    int iGetFoodWidth = int(iGetFoodCnt * fOneFoodSize) + 4;
    int iAllFoodPosX = BAR_START_X + MAX_BAR_SIZE - iAllFoodWidth;

    m_pkBlueGauge->SetWidth( iAllFoodWidth);
    m_pkBlueGauge->SetWindowPosX( static_cast<float>(iAllFoodPosX) );
    m_pkBlueGauge->ToggleRender( true );

    m_pkRedGauge->OnSetWidth( iGetFoodWidth );
    m_pkRedGauge->SetWindowPosX( static_cast<float>(iAllFoodPosX-2) );
    m_pkRedGauge->ToggleRender( (0 < iGetFoodCnt) );

    m_pkFoodNumBack->ToggleRender( true );

    m_pkGetFoodNumStr->SetText( iGetFoodCnt );
    m_pkGetFoodNumStr->ToggleRender( true );
    m_pkFoodNumStr->SetText( iFoodCnt );
    m_pkFoodNumStr->ToggleRender( true );

    for(int iLoop=0; iLoop<10; ++iLoop)
    {
        if( m_pkNumber10[iLoop] )
            m_pkNumber10[iLoop]->InitState( false );

        if( m_pkNumber1[iLoop] )
            m_pkNumber1[iLoop]->InitState( false );
    }

    int iRecvItemCnt = 0;
    int iNowFood = (iFoodCnt+iGetFoodCnt);
    if( 26 < iNowFood && iNowFood <= m_iMaxFood )   iRecvItemCnt = 14;
    else if( 24 < iNowFood && iNowFood <= 26 )      iRecvItemCnt = 13;
    else if( 22 < iNowFood && iNowFood <= 24 )      iRecvItemCnt = 12;
    else if( 20 < iNowFood && iNowFood <= 22 )      iRecvItemCnt = 11;
    else if( 18 < iNowFood && iNowFood <= 20 )      iRecvItemCnt = 10;
    else if( 16 < iNowFood && iNowFood <= 18 )      iRecvItemCnt = 9;
    else if( 14 < iNowFood && iNowFood <= 16 )      iRecvItemCnt = 8;
    else if( 12 < iNowFood && iNowFood <= 14 )      iRecvItemCnt = 7;
    else if( 10 < iNowFood && iNowFood <= 12 )      iRecvItemCnt = 6;
    else if( 8 < iNowFood && iNowFood <= 10 )       iRecvItemCnt = 5;
    else if( 6 < iNowFood && iNowFood <= 8 )        iRecvItemCnt = 4;
    else if( 4 < iNowFood && iNowFood <= 6 )        iRecvItemCnt = 3;
    else if( 2 < iNowFood && iNowFood <= 4 )        iRecvItemCnt = 2;
    else if( 0 < iNowFood && iNowFood <= 2 )        iRecvItemCnt = 1;
    if( 0 < iRecvItemCnt )
    {
        int i10 = (iRecvItemCnt/10);
        int i1 = (iRecvItemCnt%10);
        if( m_pkNumber10[i10] )
            m_pkNumber10[i10]->InitState( true );
        if( m_pkNumber1[i1] )
            m_pkNumber1[i1]->InitState( true );
    }

    if( bShowParticle )
    {
        float fParticlePos = float((iAllFoodPosX+iGetFoodWidth) - (iGetFoodWidth/2));
        float fX = (fParticlePos / 400.0f)- 0.48f;
        float fY = (m_pkRedGauge->GetCurrentWindowPos().y / 400.0f) - 0.025f;
        g_ParticleManager->PetDunGaugeEffect( fX, fY );
    }
}

void KGCFoodGauge::SetFoodGauge( int iMaxFood, int iFoodEnum, int iFoodType, int iGetFoodEnum, int iGetFoodType )
{
    m_iMaxFood = iMaxFood;
    m_iFoodEnum = iFoodEnum;
    m_iFoodType = iFoodType;
    m_iGetFoodEnum = iGetFoodEnum;
    m_iGetFoodType = iGetFoodType;
    m_iOldGetFoodCnt = -1;
    m_iOldFoodCnt = -1;

    UpDateGauge();
}