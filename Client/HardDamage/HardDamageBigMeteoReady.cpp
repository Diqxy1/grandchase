#include "stdafx.h"
#include ".\harddamagebigmeteoready.h"
#include "DamageManager.h"
#include "HardDamageBigMeteo.h"
#include "Monster.h"

CHardDamageBigMeteoReady::CHardDamageBigMeteoReady(void)
{
    m_iFrameCount = 0;
    m_iTargetCount = 1;
}

CHardDamageBigMeteoReady::~CHardDamageBigMeteoReady(void)
{
}

void CHardDamageBigMeteoReady::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );

    int iDirection = 1;

    m_iFrameCount = 0;
    m_iTargetCount = 1;
    bRight = true;
    m_iWho = pDamageInstance->m_Who;

    std::map< int, std::pair< D3DXVECTOR2, int > > m_mapMeteoXY;
    m_mapMeteoXY[65] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -2.6f, 2.0f ), 10 );
    m_mapMeteoXY[66] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -1.8f, 2.0f ), 30 );
    m_mapMeteoXY[67] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -1.0f, 2.0f ), 50 );
    m_mapMeteoXY[68] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -0.2f, 2.0f ), 40 );
    m_mapMeteoXY[69] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2(  0.6f, 2.0f ), 20 );

    m_mapMeteoXY[70] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -2.4f, 3.0f ), 15 );
    m_mapMeteoXY[71] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -1.6f, 3.0f ), 35 );
    m_mapMeteoXY[72] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -0.8f, 3.0f ), 45 );
    m_mapMeteoXY[73] = std::pair< D3DXVECTOR2, int >( D3DXVECTOR2( -0.0f, 3.0f ), 25 );

    if( !pDamageInstance->m_IsRight )
    {        
        iDirection = -1;
        bRight = false;
    }

    std::map< int, std::pair< D3DXVECTOR2, int > >::iterator mit;
    for( mit = m_mapMeteoXY.begin() ; mit != m_mapMeteoXY.end() ; ++mit )
    {
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == mit->first )
        {
            pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + ( mit->second.first.x * iDirection );
            pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + mit->second.first.y;
            m_iTargetCount = mit->second.second;
        }
    }
}

void CHardDamageBigMeteoReady::BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

	if ( pMonster == NULL ) return;


    g_MyD3D->MyPlayer[ MAX_PLAYER_NUM ]->vPos.x = fStartX;
    g_MyD3D->MyPlayer[ MAX_PLAYER_NUM ]->vPos.y = fStartY;
    g_MyD3D->MyPlayer[ MAX_PLAYER_NUM ]->bIsRight = pMonster->m_abIsRight[ LATENCY_INDEX ];
    g_MyD3D->MyPlayer[ MAX_PLAYER_NUM ]->cFrame = pMonster->m_aiFrame[ LATENCY_INDEX ] + 50;

    m_iFrameCount = 0;
    m_iWho = pDamageInstance->m_Who;
    bRight = pMonster->m_abIsRight[LATENCY_INDEX];

    pDamageInstance->m_X = fStartX + ( bRight ? -0.1f : 1.0f );
    pDamageInstance->m_Y = fStartY + 2.2f;
    m_iTargetCount = 40; // 40프레임 뒤에 메테떨어짐..-ㅅ - 그냥 조절가능함..
}

void CHardDamageBigMeteoReady::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    //법진 생성후 잠시 있다가 메테오가 나갑니다. 그리고 메테오 마다 시간이 다릅니다.
    ++m_iFrameCount;
    if( m_iFrameCount == m_iTargetCount )
    {
        if( bRight )
        {
            CDamageInstance* pkMeteo = NULL;
            if( pDamageInstance->m_IsMonsterDamage )
                pkMeteo = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK, pDamageInstance->m_ExtraMonsterNum, false );
            else
                pkMeteo = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK, m_iWho, false );

            if ( pkMeteo ) 
            {
                pkMeteo->m_IsRight = bRight;
                pkMeteo->m_X = pDamageInstance->m_X;
                pkMeteo->m_Y = pDamageInstance->m_Y;
                ((CHardDamageBigMeteo*)pkMeteo)->BeginOthers( pkMeteo );
            }
        }
        else
        {
            CDamageInstance* pkMeteo = NULL;
            if( pDamageInstance->m_IsMonsterDamage )
                pkMeteo = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK5, pDamageInstance->m_ExtraMonsterNum, false );
            else
                pkMeteo = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK5, m_iWho, false );

            if ( pkMeteo ) 
            {
                pkMeteo->m_IsRight = bRight;
                pkMeteo->m_X = pDamageInstance->m_X;
                pkMeteo->m_Y = pDamageInstance->m_Y;
                ((CHardDamageBigMeteo*)pkMeteo)->BeginOthers( pkMeteo );
            }
        }        
    }
}