#include "stdafx.h"
#include ".\igcbone.h"
#include "../GCUTIL/KJohnGen.h"
IGCBone::IGCBone(void)
{
	//m_pAngleBone        = NULL;
	//m_pPositionBone     = NULL;
	m_dwNumPositionBone = NULL;
	m_dwNumAngleBone    = NULL;
}

IGCBone::~IGCBone(void)
{
	//ASSERT( m_pAngleBone == NULL && "Please Delete m_pAngleBone On Child Class" );
	//ASSERT( m_pPositionBone == NULL && "Please Delete m_pAngleBone On Child Class" );
}
