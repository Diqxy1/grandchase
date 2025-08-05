#include "stdafx.h"
#include ".\harddamagecannon.h"



#include "Monster.h"

static float fCannonRandom[] = // 캐논 난수표
{
	  0.753343f,  0.486029f,  0.799091f,  
	  0.427381f,  0.56419f,   0.98645f,   
	  0.929319f,  0.575756f,  0.41142f,   
	  0.593707f,  0.89935f,   0.597644f,  
	  0.975768f,  0.752907f,  0.862978f,  
	  0.952788f,  0.412061f,  0.833308f,  
	  0.585345f,  0.663503f,  0.970354f,  
	  0.774438f,  0.399274f,  0.548235f,  
	  0.54207f,   0.983917f,  0.496936f,  
	  0.615561f,  0.747734f,  0.408155f,  
};

CHardDamageCannon::CHardDamageCannon(void)
{
	//m_fStartY = 0.0f;
	//m_iTime   = 0;
}

CHardDamageCannon::~CHardDamageCannon(void)
{
}

void CHardDamageCannon::Begin( CDamageInstance* pDamageInstance )
{
    static int iRand = 0;
    int RandomSize = sizeof(fCannonRandom)/sizeof(float) ;

    //난수표를 이용하여 처음 초기 x,y스피드를 다양하게 조절한다.
    m_x_Speed *= (fCannonRandom[iRand] + 0.1f);
    m_y_Speed *= (fCannonRandom[(RandomSize - 1) - iRand] + 0.1f);

    if( iRand >= RandomSize - 1)
        iRand = 0;
    else
        iRand++;

    pDamageInstance->m_IsMonsterDamage = true;
    CDamage::Begin( pDamageInstance );
    pDamageInstance->m_Angle = m_Start_Angle;
    pDamageInstance->m_IsMonsterDamage = false;
}

void CHardDamageCannon::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{

	MONSTER* pMonster = g_kMonsterManager.GetMonster(Monsteri);

	if ( pMonster == NULL ) return;

    if( pMonster->m_iMonsterType == 27 )
        return;

    return CDamage::CrashMonster( pDamageInstance, Monsteri );
}
