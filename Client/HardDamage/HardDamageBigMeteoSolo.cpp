#include "stdafx.h"
#include ".\harddamagebigmeteosolo.h"
#include "DamageManager.h"
#include "HardDamageBigMeteo.h"
#include "Monster.h"

CHardDamageBigMeteoSolo::CHardDamageBigMeteoSolo(void)
{
    m_iFrameCount = 0;
    m_iTargetCount = 1;
}

CHardDamageBigMeteoSolo::~CHardDamageBigMeteoSolo(void)
{
}


void CHardDamageBigMeteoSolo::BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY )
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

void CHardDamageBigMeteoSolo::FrameMove( CDamageInstance* pDamageInstance )
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

            pkMeteo->m_IsRight = bRight;
            pkMeteo->m_X = pDamageInstance->m_X;
            pkMeteo->m_Y = pDamageInstance->m_Y;
            ((CHardDamageBigMeteo*)pkMeteo)->BeginOthers( pkMeteo );
        }
        else
        {
            CDamageInstance* pkMeteo = NULL;
            if( pDamageInstance->m_IsMonsterDamage )
                pkMeteo = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK5, pDamageInstance->m_ExtraMonsterNum, false );
            else
                pkMeteo = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK5, m_iWho, false );

            pkMeteo->m_IsRight = bRight;
            pkMeteo->m_X = pDamageInstance->m_X;
            pkMeteo->m_Y = pDamageInstance->m_Y;
            ((CHardDamageBigMeteo*)pkMeteo)->BeginOthers( pkMeteo );
        }        
    }
}