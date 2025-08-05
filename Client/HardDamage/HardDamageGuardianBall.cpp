#include "stdafx.h"
#include ".\harddamageguardianball.h"

CHardDamageGuardianBall::CHardDamageGuardianBall(void)
{
	m_fStart_X = 0.0f;
	m_fStart_Y = 0.0f;
}

CHardDamageGuardianBall::~CHardDamageGuardianBall(void)
{
}

void CHardDamageGuardianBall::BeginWithLocate(CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate(pDamageInstance,fStartX,fStartY);

	m_fStart_X = fStartX;
	m_fStart_Y = fStartY;

}


void CHardDamageGuardianBall::AngleFrameMove(CDamageInstance* pDamageInstance )
{
	float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
	float Angle  = acosf( m_x_Speed / Length );

	
	// �����ð����ȸ� �̵��Ѵ�
	if( pDamageInstance->m_Life > 180 ) 
	{
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
		pDamageInstance->m_Angle += m_Angle_Speed;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;	
	}
	else
	{
		// �Դٸ� ���ٸ� ȿ��
		if( pDamageInstance->m_Life % 5 == 0 )
		{
			pDamageInstance->m_X -= cosf(pDamageInstance->m_Angle + Angle) * Length;
			pDamageInstance->m_Angle += m_Angle_Speed;
			pDamageInstance->m_Y -= sinf(pDamageInstance->m_Angle + Angle) * Length;	
		}
		else
		{
			pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
			pDamageInstance->m_Angle += m_Angle_Speed;
			pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;	
		}
	}

	// ȸ��
	D3DXVECTOR3 vBase, vPos, vOut;
	D3DXMATRIX matRot;

	// �������� ������ȯ
	SETVECTOR3( vBase, 0.0f - m_fStart_X, 0.0f - m_fStart_Y, 0.0f );
	SETVECTOR3( vPos, pDamageInstance->m_X, pDamageInstance->m_Y, 0.0f );
	vPos.x += vBase.x;
	vPos.y += vBase.y;

	// ȸ������
	D3DXMatrixRotationZ( &matRot, 0.1f );
	D3DXVec3TransformCoord( &vOut, &vPos, &matRot );

	// �ٽ� �ǵ���
	pDamageInstance->m_X = vOut.x - vBase.x;
	pDamageInstance->m_Y = vOut.y - vBase.y;
   

}