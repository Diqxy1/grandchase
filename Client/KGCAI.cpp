#include "stdafx.h"
#include "KGCAI.h"
//
//
//
//

KGCAI::KGCAI(CAIObject* pObj)
{
	m_pObj = pObj;
	Init();
}

KGCAI::~KGCAI(void)
{
}

void KGCAI::Init()
{
	m_vecRoute.clear();
	m_vecAttackInfo.clear();

	m_fDelayTime = 0.0f;
	m_fTargetRate = 0.0f;
	m_fTargetRangeX = 0.0f;
	m_fTargetRangeY = 0.0f;
	m_fMoveRate = 0.0f;
	m_fRunRate = 0.0f;
	m_iRouteRange = 0;
	m_fAlongRate = 0.0f;
}

void KGCAI::AddAttack()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    KLuaManager luaMgr(L);
	ATTACK_INFO stAttackInfo;

	LUA_GET_VALUE( "RATE", stAttackInfo.m_fRate, return )
	LUA_GET_VALUE_DEF( "ATTACK_NAME", stAttackInfo.m_strName, "" )

	LUA_BEGIN_TABLE( "RANGE", return )
	{
		LUA_GET_VALUE( 1, stAttackInfo.m_fRangeX, return )
		LUA_GET_VALUE( 2, stAttackInfo.m_fRangeY, return )
	}
	LUA_END_TABLE( return )

	m_vecAttackInfo.push_back( stAttackInfo );
}

HRESULT KGCAI::FrameMove()
{
	return S_OK;
}

HRESULT KGCAI::Render()
{
	/*
	static GCDeviceTexture* pNullTexture = g_pGCDeviceManager2->CreateTexture("임시로 사용하는 널 텍스쳐");
	pNullTexture->SetDeviceTexture();

	for( int i=0 ; i<(int)m_vecRoute.size() ; ++i )
	{
		float x = ( m_vecRoute[i].m_iX / 20.0f )-1.0f;
		float y = ( m_vecRoute[i].m_iY / 20.0f )-0.3f;

		D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
		SETVECTOR3(lefttop,		x-0.05f,	y+0.05f,	0.4f );
		SETVECTOR3(righttop,	x+0.05f,	y+0.05f,	0.4f );
		SETVECTOR3(leftbottom,	x-0.05f,	y-0.05f,	0.4f );
		SETVECTOR3(rightbottom,	x+0.05f,	y-0.05f,	0.4f );
		g_MyD3D->Draw_Billboard3(lefttop,righttop,leftbottom,rightbottom);
	}
	*/
	return S_OK;
}