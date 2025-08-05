#ifndef _KGCAI_H_
#define _KGCAI_H_

#include "KGCRoute.h"
#include <d3dx9.h>
////#include <vector>

// AI 시나리오
// Step1 : 판단할 시간이 되었다면 판단을 시도한다, 단 공격하는 중일땐 새로 판단하면 안된다.
// Step2 : 타게팅을 시도한다
// Step3 : 타게팅이 실패라면 중단하고 기존의 행동을 계속한다
// Step4 : 타게팅이 성공했다면 공격할지 체크한다
// Step5 : 선택한 공격이 있다면 공격을 시도하고 공격을 마치면 WAIT상태로 들어간다.
// Step6 : 선택한 공격이 없다면 이동할지 체크한다.
//         이동할거라면 경로를 탐색하고 달릴지 걸을지 선택하고 행한다.
// Step7 : 어떤것도 선택하지 못했다면 ALONG 상태에 들어갈지 체크한다.

struct ATTACK_INFO
{
	float m_fRate;
	float m_fRangeX;
	float m_fRangeY;

	std::string m_strName;
};

class CAIObject  
{
public:
	CAIObject(){}
	virtual ~CAIObject(){}

public:
	virtual void AICommand(DWORD nParam1, DWORD nParam2) = 0;
};

class KGCAI
{
public:
	KGCAI(CAIObject* pObj);
	~KGCAI(void);

public:
	void Init();
	HRESULT Render();
	HRESULT FrameMove();

	void AddAttack();

	void SetDelayTime( float fTime ){ m_fDelayTime = fTime; }
	void SetTargetRate( float fRate ){ m_fTargetRate = fRate; }
	void SetTargetRange( float fDistX, float fDistY ){ m_fTargetRangeX = fDistX; m_fTargetRangeY = fDistY; }
	void SetMoveRate( float fRate ){ m_fMoveRate = fRate; }
	void SetRunRate( float fRate ){ m_fRunRate = fRate; }
	void SetRouteRange( int iRange ){ m_iRouteRange = iRange; }
	void SetAlongRate( float fRate ){ m_fAlongRate = fRate; }

private:
	CAIObject* m_pObj;

	// 경로
	std::vector<ROUTE> m_vecRoute;

	// 공격리스트
	std::vector<ATTACK_INFO> m_vecAttackInfo;

	// 판단을 시도할 시간간격
	float m_fDelayTime;

	//타게팅 관련
	float m_fTargetRate;
	float m_fTargetRangeX;
	float m_fTargetRangeY;

	// 이동 관련
	float m_fMoveRate;
	float m_fRunRate;
	int m_iRouteRange;

	// Along관련
	float m_fAlongRate;
};

#endif //_KGCAI_H_
