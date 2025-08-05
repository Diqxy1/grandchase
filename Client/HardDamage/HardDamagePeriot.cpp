#include "stdafx.h"
#include ".\HardDamagePeriot.h"


#include "KGCRoomManager.h"

#include "Monster.h"
#include "gcui/GCGameOverlayUI.h"
#include "Headup Display.h"
#include "gcui/GCHeadupDisplay.h"
#include "gcui/GCOnGameExpBar.h"
#include "DamageManager.h"

CHardDamagePeriotOrb::CHardDamagePeriotOrb(void)
:fProcessFrame(95.0f)
,fIntervalX(0.0f)
,fIntervalY(0.0f)
,m_bIsArrived(false)
,m_iMaster(0)
,m_iGoalBone(10)
,m_fOffset_X(0.23f)
,m_fRatioHpUP(0.05f)
{
	
}

CHardDamagePeriotOrb::~CHardDamagePeriotOrb(void)
{
}

void CHardDamagePeriotOrb::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    if ( pDamageInstance->m_What == DT_HATSHEPSUT_COCCON_BALL )
    {
        fProcessFrame = 95.0f;
        m_iMaster = 0;
        m_iGoalBone = 26;
        m_fOffset_X = 0.23f;
        m_fRatioHpUP = 0.5f;
    }
	CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

    m_iMaster = pDamageInstance->m_ExtraMonsterNum;
    if( !g_kMonsterManager.IsMonsterIndex( m_iMaster ) ) { 
        return;
    }

    MONSTER* pMonster = g_kMonsterManager.GetMonster( m_iMaster );

	if ( pMonster->m_bDie || pMonster->m_fHP <= 0.0f )
	{
		pDamageInstance->m_Life = 1;
		return;
	}

	// 손 포즈~
	m_vPos = pMonster->GetBonePos( m_iGoalBone );

	float fDistanceX = ASMfabs(m_vPos.x - pDamageInstance->m_X);
	float fDistanceY = ASMfabs(m_vPos.y - pDamageInstance->m_Y);
	fIntervalX = fDistanceX / fProcessFrame;
	fIntervalY = fDistanceY / fProcessFrame;
}

void CHardDamagePeriotOrb::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	// 110 프레임 동안 m_vPos에 들어가쟈~ 

	MONSTER* pMonster = g_kMonsterManager.GetMonster( m_iMaster );

	if ( pMonster->m_bDie || pMonster->m_fHP <= 0.0f )
	{
		pDamageInstance->m_Life = 0;
		return;
	}

	// 손 포즈~
	m_vPos = pMonster->GetBonePos( m_iGoalBone );
	m_vPos.x += ( pMonster->GetIsRight() ? m_fOffset_X : -m_fOffset_X );

	if ( false == m_bIsArrived )
	{
		float fDistanceX = ASMfabs(m_vPos.x - pDamageInstance->m_X);
		float fDistanceY = ASMfabs(m_vPos.y - pDamageInstance->m_Y);

		pDamageInstance->m_X += fIntervalX * ( ( pDamageInstance->m_X < m_vPos.x ) ? 1.0f : -1.0f );
		pDamageInstance->m_Y += fIntervalY * ( ( pDamageInstance->m_Y < m_vPos.y ) ? 1.0f : -1.0f );

		if( fDistanceX <= fIntervalX )
		{
			pDamageInstance->m_X = m_vPos.x;
		}
		if ( fDistanceY <= fIntervalY )
		{
			pDamageInstance->m_Y = m_vPos.y;
		}

		if ( ASMfabs( pDamageInstance->m_X - m_vPos.x ) <= 0.05f && ASMfabs( pDamageInstance->m_Y - m_vPos.y ) <= 0.05f && !m_bIsArrived )
		{
			m_bIsArrived = true;
			pDamageInstance->m_pOriginalDamage->m_bNoPassDamage = false;
			pMonster->AddCountValue(1);
		}

        if ( pDamageInstance->m_What == DT_HATSHEPSUT_COCCON_BALL )
        {
            if ( pDamageInstance->m_Life == 1 )
            {
                pMonster->m_fHP += (pMonster->m_fHPPoint * m_fRatioHpUP);

                if ( pMonster->m_fHP > pMonster->m_fHPPoint )
                    pMonster->m_fHP = pMonster->m_fHPPoint;
            }
        }
	}
	else
	{
		pDamageInstance->m_X = m_vPos.x;
		pDamageInstance->m_Y = m_vPos.y;

		if ( pDamageInstance->m_Life == 1 )
		{
			pMonster->m_fHP += (pMonster->m_fHPPoint * m_fRatioHpUP);

			if ( pMonster->m_fHP > pMonster->m_fHPPoint )
				pMonster->m_fHP = pMonster->m_fHPPoint;
		}
	}
}

CHardDamagePeriotEat::CHardDamagePeriotEat( void )
{

}

CHardDamagePeriotEat::~CHardDamagePeriotEat( void )
{

}

void CHardDamagePeriotEat::Begin( CDamageInstance* pDamageInstance )
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
	if( pMonster != NULL ) {
		pMonster->SetCountValue( 0 );
	}
}

void CHardDamagePeriotEat::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	if( pDamageInstance->m_ExtraMonsterNum == Monsteri )
		return;

	if ( pDamageInstance->InsertAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ) == false ) {
		return;
	}
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if( pMonster != NULL ) {

        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

		if ( pMonster->GetMonsterType() == MON_MYSTONE )
		{
			float fStartX = pMonster->m_afX[LATENCY_INDEX];
			float fStartY = pMonster->m_afY[LATENCY_INDEX];

			g_MyD3D->m_pDamageManager->MonsterAddWithLocate( DT_PERIOT_MONSTER_BALL , pDamageInstance->m_ExtraMonsterNum, fStartX, fStartY );
			pMonster->SetState( "PERIOT_SUCK", true );
		}
	}
	return;
}

bool CHardDamagePeriotEat::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	return false;
}


CHardDamagePeriotDestroyOrb::CHardDamagePeriotDestroyOrb( void )
{
	pMonster = NULL;
}

CHardDamagePeriotDestroyOrb::~CHardDamagePeriotDestroyOrb( void )
{

}

void CHardDamagePeriotDestroyOrb::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate(pDamageInstance, fStartX, fStartY);

	vecLiveIndex.clear();
	iPlayerNum = 0;
	iMaxnum = 0;
	int bossIndex = 0;

	switch( pDamageInstance->m_What )
	{
	case DT_PERIOT_FIRE_DESTROY_ORB:
		iMaxnum = 8;
		bossIndex = 0;
		break;
	case DT_THANATOS2_DARKSPEAR_SHOOTER:
		iMaxnum = 4;
		bossIndex = 1;
		break;
	}

	for(int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && ((char)g_MyD3D->MyPlayer[i]->m_cLife) > 0)
		{
			vecLiveIndex.push_back( i );
			iPlayerNum++;
		}
	}

	pMonster = g_kMonsterManager.m_mapMonster[bossIndex];

	return;
}

void CHardDamagePeriotDestroyOrb::NotAngleFrameMove( CDamageInstance* pDamageInstance )
{
	float fAngle = 0.0f;
	int iFirePerFreame = 1;
	int iBonePos = 0;
	char* SoundID = NULL;
	int iDamageID = -1;
	float fTermX = 0.0f;

	if ( pMonster->m_bDie || pMonster->m_fHP < 0.0f || iPlayerNum == 0 )
	{
		pDamageInstance->m_Life = 1;
		return;
	}

	switch( pDamageInstance->m_What )
	{
	case DT_PERIOT_FIRE_DESTROY_ORB:
		iFirePerFreame = 18;
		iBonePos = 4;
		SoundID = "998026";
		iDamageID = DT_PERIOT_HOMMING_MISSILE_DESTROY;
		fTermX = 0.2f;
		break;
	case DT_THANATOS2_DARKSPEAR_SHOOTER:
		iFirePerFreame = 18;
		iBonePos = 33;
		SoundID = "995006";
		iDamageID = DT_THANATOS2_DARKSPEAR;
		fTermX = 0.05f;
		break;
	}

	if ( pDamageInstance->m_Life % iFirePerFreame == 0 && iMaxnum > 0 && iPlayerNum != 0 )
	{
		pMonster->SetPlayerTargetManual( vecLiveIndex[iMaxnum % iPlayerNum] );
		D3DXVECTOR3 vFirePos = pMonster->GetBonePos( iBonePos );

		float fOffsetX = vFirePos.x + (pMonster->GetIsRight() ? fTermX : -fTermX) - pMonster->GetX();
		float fOffsetY = vFirePos.y - pMonster->GetY();

		float fX = pMonster->GetTargetX() - ( pMonster->GetX() + fOffsetX );
		float fY = pMonster->GetTargetY() + 0.15f - ( pMonster->GetY() + fOffsetY );

		float fLength = ASMsqrt( powf( fX, 2.0f ) + powf( fY, 2.0f ) );
		float fAngle = asinf( fY / fLength );

		fAngle = fAngle * (pMonster->GetIsRight() ? 1.0f : -1.0f);
		if ( ( pMonster->GetX() > pMonster->GetTargetX() && pMonster->GetIsRight() ) ||
			 ( pMonster->GetX() < pMonster->GetTargetX() && !pMonster->GetIsRight() ) )
		{
			fAngle = fAngle * -1.0f + D3DX_PI;
		}

		pMonster->PlaySound(SoundID);
		pMonster->AddDamageWithLocateAngle( iDamageID, vFirePos.x + (pMonster->GetIsRight() ? fTermX : -fTermX), vFirePos.y, true, fAngle );
		iMaxnum--;
	}
}

CHardDamageHatshepsut::CHardDamageHatshepsut( void )
:m_iMonsterType(MON_HATSHEPSUT_COCOON)
,m_iDamageID(DT_HATSHEPSUT_COCCON_BALL)
,m_strState("SUCK")
{

}

CHardDamageHatshepsut::~CHardDamageHatshepsut( void )
{

}

void CHardDamageHatshepsut::Begin( CDamageInstance* pDamageInstance )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if ( (pMonster->GetMonsterType() == m_iMonsterType || pMonster->GetMonsterType() == MON_ANGRYBOSS_HATSHEPSUT_COCOON) && 
			pMonster->IsLive() && !pMonster->m_bDie && pMonster->m_fHP > 0.0f )
		{
			g_MyD3D->m_pDamageManager->MonsterAddWithLocate( m_iDamageID, pMonster->m_iMonsterIndex, pMonster->GetX(), pMonster->GetY() );
			pMonster->SetState( m_strState, true );
			break;
		}
		else
			continue;
	}
}

bool CHardDamageHatshepsut::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    return false;
}

void CHardDamageHatshepsut::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( Monsteri );
    if( pMonster )
    {
        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

    return;
}

CHardDamageCatMullRomSpline::CHardDamageCatMullRomSpline( void )
{

}

CHardDamageCatMullRomSpline::~CHardDamageCatMullRomSpline( void )
{

}

void CHardDamageCatMullRomSpline::Begin( CDamageInstance* pDamageInstance )
{
    m_vecMileStone.clear();
    CDamage::Begin( pDamageInstance );
    m_iLifeMax = pDamageInstance->m_Life;
    m_fS = 0.0f;
    m_iIndex = 0;
}

void CHardDamageCatMullRomSpline::ValkyrieFrameMove( CDamageInstance* pDamageInstance )
{

    D3DXVECTOR3 pOut;
    int iSection = m_vecMileStone.size() - 3;

    float fRealPosX = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
    float fRealPosY = g_kLocalPlayerLatency->kRenderData[0].vPos.y + 0.15f;

    m_vecMileStone[iSection+1].x = fRealPosX;
    m_vecMileStone[iSection+1].y = fRealPosY;
    m_vecMileStone[iSection+2].x = fRealPosX;
    m_vecMileStone[iSection+2].y = fRealPosY;

    if ( m_fS >= 1.0f )
    {
        if ( m_iIndex + 1 + 3 < (int)m_vecMileStone.size() )
        {
            ++m_iIndex;
        }

        else
        {
            m_vecMileStone[m_iIndex] = m_vecMileStone[m_iIndex + 1];
            m_vecMileStone[m_iIndex + 1] = m_vecMileStone[m_iIndex + 2];
            m_vecMileStone[m_iIndex + 2] = m_vecMileStone[m_iIndex + 3];
        }

        m_fS = 0.0f;
    }

    D3DXVec3CatmullRom( &pOut, &m_vecMileStone[m_iIndex], &m_vecMileStone[m_iIndex + 1], &m_vecMileStone[m_iIndex + 2], &m_vecMileStone[m_iIndex + 3], m_fS);
    m_fS += (1.0f) / (float)(m_iLifeMax / iSection);

    pDamageInstance->m_X = pOut.x;
    pDamageInstance->m_Y = pOut.y;

    if ( pDamageInstance->m_Life == 1 )
    {
        if( g_MyD3D->Get_MyPlayer() == pDamageInstance->m_Who )
        {
            g_ParticleManager->CreateSequence( "Exp_Add_Effect_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 5.5f );
            g_ParticleManager->CreateSequence( "Exp_Add_Effect_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 5.5f );
            g_MyD3D->MyHead->SetRenderExpGp( g_MyD3D->Get_MyPlayer(), static_cast<int>(pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer) );
            g_pkGameOverlayUI->GetHeadUp()->m_pkExpBar->AddVirtualExp( pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer );
            g_KDSound.Play( "991027" );
        }
    }
}
