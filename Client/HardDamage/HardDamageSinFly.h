#pragma once
#include "damage.h"

class CHardDamageSinFly :
	public CDamage
{
public:
	CHardDamageSinFly(void);
	virtual ~CHardDamageSinFly(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );

	float m_fStartY;	//ó�� ������ ���� Y��ǥ�̴�.
	float m_iTime;		

	float m_fCyclopseY;	//����Ŭ�ӽ� ���� ���� �������� ���� Y��ǥ��. �� �ܿ��� ����������.
};
