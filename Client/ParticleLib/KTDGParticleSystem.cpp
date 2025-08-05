#include "stdafx.h"
#include "../GCDeviceLib/GCMatrix.h"
#include "../GCDeviceLib/KD3DDeviceState.h"
#include "../GCDeviceLib/GCDeviceManager.h"

#include "KTDGParticleSystem.h"
#include <fstream>
//#include <assert.h>
//#include <algorithm>
//#include <boost/bind.hpp>

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

///////////////////////////////////////////////////////////////////////////////
// Particle Event
///////////////////////////////////////////////////////////////////////////////
/*
EVENT_SIZE = 0,
EVENT_COLOR,
EVENT_ROTATE,
EVENT_ROTATEX,
EVENT_ROTATEY,
EVENT_ROTATEZ,
EVENT_VELOCITY,
EVENT_VELOCITYX,
EVENT_VELOCITYY,
EVENT_VELOCITYZ,
EVENT_BLACKHOLE,
EVENT_TYPE_NUM,
*/

#include <stdio.h>
#include <sys/stat.h>

__int64 getFileSize(char *filename) {
	struct _stati64 statbuf;

	if (_stati64(filename, &statbuf))
	{
		return -1;
	}

	return statbuf.st_size;
}

char* strCParticleEventName[CKTDGParticleSystem::EVENT_TYPE_NUM] =
{
	"size",
	"color",
	"rotate",
	"velocity",
	"BlackHoleSpeed",
	"rotateX",
	"rotateY",
	"rotateZ",
	"velocityX",
	"velocityY",
	"velocityZ",
	"EventTimer",
};

void CKTDGParticleSystem::CParticleEvent::toString(std::string& strOut)
{
	std::stringstream stream;

	if (m_ActualTime.m_Min == m_ActualTime.m_Max)	//at
	{
		stream << "at " << m_ActualTime.m_Min << " " << strCParticleEventName[m_eType] << " = " << toStringValue();
	}
	else	// fade
	{
		stream << "fade so from " << m_ActualTime.m_Min << " to " <<
			m_ActualTime.m_Max << " " << strCParticleEventName[m_eType] << " = " << toStringValue();
	}
	strOut = stream.str();
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<float> &prop,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) assert(0 && "Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessNumber(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) assert(0 && "Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessVector(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXVECTOR2> &prop,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) assert(0 && "Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessVector2(prop, TokenIter, EndIter);
}

void CKTDGParticleSystem::CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// next token should be =
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	if (TokenIter->m_Type != Equals) assert(0 && "Expecting = after property.");

	// next token should be a number
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	CParticleEmitterTokenizer::ProcessColor(prop, TokenIter, EndIter);
}

bool CKTDGParticleSystem::CParticleEvent_Color::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("COLOR") == std::string::npos) assert(0 && "Expecting Color!");
	ProcessPropEqualsValue(m_Color, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Color::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_Color = m_Color.GetRandomNumInRange();
	else
	{
		SetFinalValue(part);
		D3DXCOLOR	fRemainColor;

		fRemainColor.r = part.m_FinalColor.r - part.m_Color.r;
		fRemainColor.g = part.m_FinalColor.g - part.m_Color.g;
		fRemainColor.b = part.m_FinalColor.b - part.m_Color.b;
		fRemainColor.a = part.m_FinalColor.a - part.m_Color.a;

		part.m_ColorStep.r = fRemainColor.r / fRemainTime;
		part.m_ColorStep.g = fRemainColor.g / fRemainTime;
		part.m_ColorStep.b = fRemainColor.b / fRemainTime;
		part.m_ColorStep.a = fRemainColor.a / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_Size::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("SIZE") == std::string::npos) assert(0 && "Expecting Size!");
	ProcessPropEqualsValue(m_Size, TokenIter, EndIter);

	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Size::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vSize = m_Size.GetRandomNumInRange();
	else
	{
		SetFinalValue(part);
		D3DXVECTOR2	fRemainSize;

		fRemainSize.x = part.m_vFinalSize.x - part.m_vSize.x;
		fRemainSize.y = part.m_vFinalSize.y - part.m_vSize.y;

		part.m_vSizeStep.x = fRemainSize.x / fRemainTime;
		part.m_vSizeStep.y = fRemainSize.y / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_Velocity::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("VELOCITY") == std::string::npos) assert(0 && "Expecting Velocity!");
	ProcessPropEqualsValue(m_Velocity, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Velocity::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
	{
		part.m_vVelocity = m_Velocity.GetRandomNumInRange();

		// 방향성 적용
		if (stDir.m_bDirVelocity == true)
		{
			bool bRight;
			if (stDir.m_pbIsRight != NULL)
			{
				bRight = *stDir.m_pbIsRight;
			}
			else
			{
				bRight = stDir.m_bIsRight;
			}

			if (bRight == false)
			{
				part.m_vVelocity.x *= -1.0f;
			}
		}
	}
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		D3DXVECTOR3	fRemainVelocity;

		// 방향성 적용
		if (stDir.m_bDirVelocity == true)
		{
			bool bRight;
			if (stDir.m_pbIsRight != NULL)
			{
				bRight = *stDir.m_pbIsRight;
			}
			else
			{
				bRight = stDir.m_bIsRight;
			}

			if (bRight == false)
			{
				part.m_vFinalVelocity.x *= -1;
			}
		}

		fRemainVelocity = part.m_vFinalVelocity - part.m_vVelocity;
		part.m_vVelocityStep = fRemainVelocity / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_VelocityX::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("VELOCITYX") == std::string::npos) assert(0 && "Expecting VelocityX!");
	ProcessPropEqualsValue(m_VelocityX, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_VelocityX::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vVelocity.x = m_VelocityX.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainVelocityX;

		fRemainVelocityX = part.m_vFinalVelocity.x - part.m_vVelocity.x;

		part.m_vVelocityStep.x = fRemainVelocityX / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_VelocityY::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("VELOCITYY") == std::string::npos) assert(0 && "Expecting VelocityY!");
	ProcessPropEqualsValue(m_VelocityY, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_VelocityY::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vVelocity.y = m_VelocityY.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainVelocityY;

		fRemainVelocityY = part.m_vFinalVelocity.y - part.m_vVelocity.y;

		part.m_vVelocityStep.y = fRemainVelocityY / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_VelocityZ::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("VELOCITYZ") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_VelocityZ, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_VelocityZ::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vVelocity.z = m_VelocityZ.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainVelocityZ;

		fRemainVelocityZ = part.m_vFinalVelocity.z - part.m_vVelocity.z;

		part.m_vVelocityStep.z = fRemainVelocityZ / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_Rotate::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("ROTATE") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_Rotate, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_Rotate::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vRotate = m_Rotate.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		D3DXVECTOR3	fRemainRotate;

		fRemainRotate.x = part.m_vFinalRotate.x - part.m_vRotate.x;
		fRemainRotate.y = part.m_vFinalRotate.y - part.m_vRotate.y;
		fRemainRotate.z = part.m_vFinalRotate.z - part.m_vRotate.z;

		part.m_vRotateStep.x = fRemainRotate.x / fRemainTime;
		part.m_vRotateStep.y = fRemainRotate.y / fRemainTime;
		part.m_vRotateStep.z = fRemainRotate.z / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_RotateX::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("ROTATEX") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_RotateX, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_RotateX::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vRotate.x = m_RotateX.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainRotateX;

		fRemainRotateX = part.m_vFinalRotate.x - part.m_vRotate.x;

		part.m_vRotateStep.x = fRemainRotateX / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_RotateY::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("ROTATEY") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_RotateY, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_RotateY::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vRotate.y = m_RotateY.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainRotateY;

		fRemainRotateY = part.m_vFinalRotate.y - part.m_vRotate.y;

		part.m_vRotateStep.y = fRemainRotateY / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_RotateZ::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("ROTATEZ") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_RotateZ, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_RotateZ::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vRotate.z = m_RotateZ.GetRandomNumInRange();
	else
	{
		if (fRemainTime < part.m_fEventTimer - part.m_fEventTimerOld)	//ElapsedTime보다 적게 남아있으면
		{
			fRemainTime = part.m_fEventTimer - part.m_fEventTimerOld;
		}
		SetFinalValue(part);
		float fRemainRotateZ;

		fRemainRotateZ = part.m_vFinalRotate.z - part.m_vRotate.z;

		part.m_vRotateStep.z = fRemainRotateZ / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_DirSpeed::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("DIRSPEED") == std::string::npos) assert(0 && "Expecting VelocityZ!");
	ProcessPropEqualsValue(m_DirSpeed, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_DirSpeed::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	float fRemainTime = m_ActualTime.m_Max - part.m_fEventTimer;

	if (!IsFade() || fRemainTime == 0)
		part.m_vDirSpeed = m_DirSpeed.GetRandomNumInRange();
	else
	{
		SetFinalValue(part);
		float fRemainDirSpeed;

		fRemainDirSpeed = part.m_vFinalDirSpeed - part.m_vDirSpeed;

		part.m_vDirSpeedStep = fRemainDirSpeed / fRemainTime;
	}
}

bool CKTDGParticleSystem::CParticleEvent_BlackHole::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("BLACKHOLESPEED") == std::string::npos) assert(0 && "Expecting EventTimer!");
	ProcessPropEqualsValue(m_BlackHole, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_BlackHole::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	part.m_BlackHoleTime = m_BlackHole.GetRandomNumInRange();
}

bool CKTDGParticleSystem::CParticleEvent_EventTimer::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_strValue.find("EVENTTIMER") == std::string::npos) assert(0 && "Expecting EventTimer!");
	ProcessPropEqualsValue(m_EventTimer, TokenIter, EndIter);
	return(true);
}

void CKTDGParticleSystem::CParticleEvent_EventTimer::OnFrameMove(CParticle &part, DIRECTION_INFO& stDir)
{
	part.m_fEventTimer = m_EventTimer.GetRandomNumInRange();
}

///////////////////////////////////////////////////////////////////////////////
// Particle Event Sequence
///////////////////////////////////////////////////////////////////////////////

CKTDGParticleSystem::CParticleEventSequence::CParticleEventSequence() : m_bSlowCount(false)
{
	m_bAutoDie = false;
	m_Texture = NULL;
	m_Matrix = new GCMatrix();

	SETVECTOR3(m_Position, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_PosDiff, 0.0f, 0.0f, 0.0f);
	m_pBlackHolePosition = NULL;

	m_Lifetime.m_Min = 0.0f;
	m_Lifetime.m_Max = 0.0f;
	m_EmitRate.m_Min = 0.0f;
	m_EmitRate.m_Max = 0.0f;

	SETVECTOR3(m_vEmitRadius.m_Min, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vEmitRadius.m_Max, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vGravity.m_Min, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vGravity.m_Max, 0.0f, 0.0f, 0.0f);

	m_iMaxParticleNum = 0;
	m_pbRender = NULL;
	m_bRender = true;

	m_iSrcBlendMode = D3DBLEND_SRCALPHA;
	m_iDestBlendMode = D3DBLEND_DESTALPHA;

	m_fNumNewPartsExcess = 0.0f;
	m_fKillTime = 0.0f;
	m_fCurrLife = 0.0f;
	m_iKillNum = 0;
	m_iTotalEmitNum = 0;
	m_iLayer = 5;		// 디폴트 레이어 : 캐릭터와 맵이펙트 사이

	m_bUserAdditionalZRotate = false;
	m_fAdditionalZRotate = 0.f;
	SETVECTOR3(m_posBegin, 0.0f, 0.0f, 0.0f); // 데미지 생성 위치.
	m_bUseInternalBlackholePos = false;
	SETVECTOR3(m_vposInternalBlackhole, 0.0f, 0.0f, 0.0f);

	m_bRenderForImageTag = true;
}

CKTDGParticleSystem::CParticleEventSequence::~CParticleEventSequence()
{
	m_Particles.clear();
	m_Events.clear();
	m_Texture = NULL;
	SAFE_DELETE(m_Matrix);
}

void CKTDGParticleSystem::CParticleEventSequence::SetTracePosition(D3DXVECTOR3& vPos)
{
	vPos = *m_stTrace.m_pvPos;
	vPos.x += m_stTrace.m_fXOffset;
	vPos.y += m_stTrace.m_fYOffset;
	vPos.z = 0.5f;
}

bool CKTDGParticleSystem::CParticleEventSequence::IsBlackHoleEventHere()
{
	return (m_Events.end() != std::find_if(m_Events.begin(), m_Events.end(),
		boost::bind(&CParticleEvent::GetEventType, _1) == EVENT_BLACKHOLE));
}

void CKTDGParticleSystem::CParticleEventSequence::OnFrameMove(double fTime, float fElapsedTime)
{
	if (m_bSlowCount)
		return;

	m_fCurrLife += fElapsedTime;

	// 이전 위치 저장
	D3DXVECTOR3 vBefore = m_Position;
	D3DXVECTOR3 vNewPos = vBefore;

	bool bReverse = false;

	// 시퀀스 Trace 처리
	if (m_stTrace.m_pvPos != NULL)
		SetTracePosition(vNewPos);

	// 방향성 적용
	bool bRight;
	if (m_stDirect.m_pbIsRight != NULL)
	{
		bRight = *m_stDirect.m_pbIsRight;
	}
	else
	{
		bRight = m_stDirect.m_bIsRight;
	}
	if (m_stDirect.m_fDirPosX != 0.0f)
	{
		if (bRight == true)
			vNewPos.x -= m_stDirect.m_fDirPosX;
		else
			vNewPos.x += m_stDirect.m_fDirPosX;
	}
	if (m_stDirect.m_bReverseTexture == true)
	{
		bReverse = !bRight;
	}

	m_PosDiff = vNewPos - vBefore;
	m_Position = vNewPos;

	// ++i 밑에 있다.
	for (std::vector<CParticle>::iterator i = m_Particles.begin(); i != m_Particles.end(); )
	{
		if ((*i).OnFrameMove(fElapsedTime) == false)
		{
			i = m_Particles.erase(i);
			continue;
		}
		else
		{
			//중력 적용
			(*i).m_vVelocity += m_vGravity.GetRandomNumInRange() * fElapsedTime;

			//블랙홀 포인트
			if ((*i).m_BlackHoleTime > 0.0f)
			{
				if (m_pBlackHolePosition != NULL)
					(*i).m_BlackHoleSpeed = (*m_pBlackHolePosition - (*i).m_vPos) / (*i).m_BlackHoleTime;
				else
				{
					D3DXVECTOR3 hole(0.0f, 0.0f, 0.0f);
					(*i).m_BlackHoleSpeed = (hole - (*i).m_vPos) / (*i).m_BlackHoleTime;
				}
			}

			//이벤트 적용
			RunEvent((*i));
		}

		// 파티클 Trace 처리
		// Sequence의 변화량만큼 변화시켜주라
		if (m_stTrace.m_pvPos != NULL && (*i).m_fTraceTime > 0.0f)
		{
			// [10/22/2007] breadceo.Trace Offset 아무데도 안쓰고 이 의미로 쓰면 이상하다
			(*i).m_vPos.x += m_PosDiff.x;
			(*i).m_vPos.y += m_PosDiff.y;
		}

		i++;
	}

	//파티클 생성
	float fEmitRateThisFrame = m_EmitRate.GetRandomNumInRange();

	int iNumNewParts = static_cast<int>(fEmitRateThisFrame * fElapsedTime);
	m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fElapsedTime) - iNumNewParts;

	if (m_fNumNewPartsExcess > 1.0f)
	{
		iNumNewParts += (int)m_fNumNewPartsExcess;
		m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
	}

	if (m_iKillNum > 0)
	{
		if (m_iTotalEmitNum + iNumNewParts > m_iKillNum)
			iNumNewParts = m_iKillNum - m_iTotalEmitNum;
	}

	for (int q = 0; q < iNumNewParts; q++)
	{
		if (m_iMaxParticleNum > (int)m_Particles.size())
			CreateNewParticle(vNewPos, bReverse);
	}
	m_iTotalEmitNum += iNumNewParts;
}

void CKTDGParticleSystem::CParticleEventSequence::OnFrameRender(GCDeviceVertexBuffer* pVB)
{
	if (m_bRender == false || (m_pbRender != NULL && (*m_pbRender) == false) || m_bRenderForImageTag == false)
		return;

	VERTEX_PARTICLE* pVertices = NULL;
	D3DXMATRIX finalMat;
	D3DXVECTOR4 v4temp;

	pVertices = (VERTEX_PARTICLE*)pVB->Lock((BYTE*)pVertices);

	int iSize = (int)m_Particles.size();
	//	ASSERT( iSize <= MAX_PARTICLE );
	if (m_Texture == NULL)
		return;

	float fTextureCenterX = m_Texture->GetWidth() / 2.0f;
	float fTextureCenterY = m_Texture->GetHeight() / 2.0f;

	// Render each particle
	float fLeft = 0.0f;
	float fRight = 1.0f;
	for (int q = 0; q < iSize; q++)
	{
		// Render each particle a bunch of times to get a blurring effect
		CParticle &part = m_Particles[q];

		m_Matrix->Move(part.m_vPos.x, part.m_vPos.y, part.m_vPos.z);
		m_Matrix->Rotate(D3DXToRadian(part.m_vRotate.x), D3DXToRadian(part.m_vRotate.y), D3DXToRadian(part.m_vRotate.z));
		m_Matrix->Scale(part.m_vSize.x, part.m_vSize.y, 1.0f);

		//finalMat= m_Matrix->GetBillBoardMatrix();
		D3DXMATRIX matMid = m_Matrix->GetBillBoardMatrix();

		if (m_bUserAdditionalZRotate)
		{
			// 매트릭스의 포지션을 빼고
			// 추가 로테이션
			// 손의 위치로 포지션 이동.

			D3DXMATRIX matTrans;
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation(&matTrans, -m_Matrix->GetXPos(), -m_Matrix->GetYPos(), -m_Matrix->GetZPos());

			D3DXMATRIX matZRot;
			D3DXMatrixIdentity(&matZRot);
			D3DXMatrixRotationZ(&matZRot, m_fAdditionalZRotate);

			D3DXMATRIX matTrans1;
			D3DXMatrixIdentity(&matTrans1);
			D3DXMatrixTranslation(&matTrans1, m_posBegin.x, m_posBegin.y, m_posBegin.z);

			finalMat = matMid * matTrans * matZRot * matTrans1;
		}
		else // 리르4 (노바) 활에 이펙트 계산을 위해서 이런 삽질을 했습니다.. 죄송합니다..ㅜㅜ
		{
			finalMat = matMid;
		}

		if (!part.m_bReverse)
		{
			fLeft = 0.0f;
			fRight = 1.0f;
		}
		else
		{
			fLeft = 1.0f;
			fRight = 0.0f;
		}
		//1
		pVertices->position.x = -1.0f * fTextureCenterX;
		pVertices->position.y = fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fLeft;
		pVertices->tex.y = 0.0f;
		pVertices++;

		//2
		pVertices->position.x = -1.0f * fTextureCenterX;
		pVertices->position.y = -1.0f * fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fLeft;
		pVertices->tex.y = 1.0f;
		pVertices++;

		//3
		pVertices->position.x = fTextureCenterX;
		pVertices->position.y = fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fRight;
		pVertices->tex.y = 0.0f;
		pVertices++;

		//4
		pVertices->position.x = fTextureCenterX;
		pVertices->position.y = fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fRight;
		pVertices->tex.y = 0.0f;
		pVertices++;

		//5
		pVertices->position.x = -1.0f * fTextureCenterX;
		pVertices->position.y = -1.0f * fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fLeft;
		pVertices->tex.y = 1.0f;
		pVertices++;

		//6
		pVertices->position.x = fTextureCenterX;
		pVertices->position.y = -1.0f * fTextureCenterY;
		pVertices->position.z = 0.0f;
		D3DXVec3Transform(&v4temp, &pVertices->position, &finalMat);
		pVertices->position.x = v4temp.x;
		pVertices->position.y = v4temp.y;
		pVertices->position.z = v4temp.z;
		pVertices->color = (DWORD)part.m_Color;
		pVertices->tex.x = fRight;
		pVertices->tex.y = 1.0f;
		pVertices++;
	}

	pVB->Unlock();

	m_Texture->SetDeviceTexture();
	g_pd3dDevice->SetStreamSource(0, pVB->GetBuffer(), 0, sizeof(VERTEX_PARTICLE));
	g_pd3dDevice->SetFVF(D3DFVF_NEW_PARTICLE);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, iSize * 2);
}

CKTDGParticleSystem::CParticle* CKTDGParticleSystem::CParticleEventSequence::CreateNewParticle(const D3DXVECTOR3& vPartSysPos, bool bReverse)
{
	CParticle part;

	part.m_fLifetime = m_Lifetime.GetRandomNumInRange();
	part.m_vPos = vPartSysPos + m_vEmitRadius.GetRandomNumInRange();

	// process any initial events
	for (std::vector<CParticleEvent*>::iterator i = m_Events.begin(); i != m_Events.end(); i++)
	{
		if ((*i)->IsFade() == true)
			(*i)->SetFinalValue(part);

		if ((*i)->GetActualTime().m_Min == 0.0f)
		{
			(*i)->OnFrameMove(part, m_stDirect);
		}
	}

	part.m_bReverse = bReverse;

	part.m_fTraceTime = m_stTrace.m_fParticleTraceTime;
	m_Particles.push_back(part);

	std::vector<CParticle>::iterator j = m_Particles.end() - 1;

	return &(*j);
}

void CKTDGParticleSystem::CParticleEventSequence::RunEvent(CParticle &part)
{
	for (std::vector<CParticleEvent*>::iterator i = m_Events.begin(); i != m_Events.end(); i++)
	{
		if ((*i)->IsFade() == true)
		{
			if (part.m_fEventTimer >= (*i)->GetActualTime().m_Min
				&& part.m_fEventTimer < (*i)->GetActualTime().m_Max)
			{
				(*i)->OnFrameMove(part, m_stDirect);
			}
		}
		else
		{
			if (part.m_fEventTimerOld < (*i)->GetActualTime().m_Min
				&& part.m_fEventTimer >= (*i)->GetActualTime().m_Min)
			{
				(*i)->OnFrameMove(part, m_stDirect);
			}
		}
	}
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CParticleEventSequence::GetCloneSequence()
{
	CParticleEventSequence* clone = new CParticleEventSequence();

	clone->SetName(GetName());
	clone->SetTextureName(GetTexName());
	clone->SetPosition(GetPosition());
	clone->SetLifetime(GetLifeTime());
	clone->SetEmitRate(GetEmitRate());
	clone->SetKillTime(GetKillTime());
	clone->SetKillNum(GetKillNum());
	clone->SetEmitRadius(GetEmitRadius());
	clone->SetGravity(GetGravity());
	clone->SetMaxParticleNum(GetMaxParticleNum());
	clone->SetTrace(GetTrace());
	clone->SetSrcBlendMode(GetSrcBlendMode());
	clone->SetDestBlendMode(GetDestBlendMode());
	clone->SetLayer(GetLayer());
	clone->m_Texture = m_Texture;

	for (int i = 0; i < (int)m_Events.size(); i++)
	{
		clone->m_Events.push_back(m_Events[i]);
	}

	return clone;
}

///////////////////////////////////////////////////////////////////////////////
// CParticleEmitterTokenizer
///////////////////////////////////////////////////////////////////////////////

void CKTDGParticleSystem::CParticleEmitterTokenizer::Tokenize(const char *str)
{
	enum CurState
	{
		InWhiteSpace = 1,
		InText,
		InQuote,
		InComment
	};

	CurState				cs = InWhiteSpace;
	const char*				p = str;
	CParticleEmitterToken	token;

	while ((*p) != 0)
	{
		switch (cs)
		{
		case InWhiteSpace:
		{
			// if this letter is not whitespace,
			if (!isspace(*p))
			{
				// add it to the running buffer
				token.m_strValue = (*p);
				// switch to appropriate case
				if ((*p) == '/' && *(p + 1) == '/') // '//'는 주석
					cs = InComment;
				else
					cs = (*p == '\"') ? InQuote : InText;
			} // end if
		}
		break;

		case InText:
		{
			// if this letter is whitespace
			if (isspace(*p))
			{
				// add the completed token to the vector
				AddToken(token);
				// switch to whitespace case
				cs = InWhiteSpace;
			}
			else
			{
				// if this letter is a token terminator
				if ((*p) == '(' || (*p) == ')' || (*p) == ',' || (*p) == '\"' || (*p) == '{' || (*p) == '}' || (*p) == '/')
				{
					if ((*p) == '/' && *(p + 1) == '/')
						cs = InComment;
					else
					{
						// add the completed token to the vector
						DetermineTokenType(token);
						AddToken(token);

						// if it was a quote, transition to InQuote state
						if ((*p) == '\"')
							cs = InQuote;
						// otherwise, process this one char as a token
						else
						{
							token.m_strValue = (*p);
							AddToken(token);
							token.m_strValue = "";
						}
					}
				}
				else
				{
					// add this letter to the work in progress token
					token.m_strValue += (*p);
				} // end if
			} // end if
		}
		break;

		case InComment:
		{
			// C++ style comments - skip everything until end of line
			if (*p == '\n')
				token.m_strValue = ""; cs = InWhiteSpace;
		}
		break;

		case InQuote:
		{
			// unconditionally add this letter to the token until we hit a close quote
			token.m_strValue += (*p);
			if (*p == '\"')
			{
				AddToken(token);
				// go back to whitespace
				cs = InWhiteSpace;
			}
		}
		break;
		}
		p++;
	}
	AddToken(token);
}

void CKTDGParticleSystem::CParticleEmitterTokenizer::AddToken(CParticleEmitterToken token)
{
	if (token.m_strValue.size())
	{
		DetermineTokenType(token);
		m_TokenVector.push_back(token);
	}
}

void CKTDGParticleSystem::CParticleEmitterTokenizer::DetermineTokenType(CParticleEmitterToken &token)
{
	token.m_Type = UnknownToken;

	// these things are easy to see...
	if (isdigit(token.m_strValue[0]) || token.m_strValue[0] == '-') { token.m_Type = RealNumber; return; }
	if (token.m_strValue[0] == '=') { token.m_Type = Equals; return; }
	if (token.m_strValue[0] == ',') { token.m_Type = Comma; return; }
	if (token.m_strValue[0] == '(') { token.m_Type = OpenParen; return; }
	if (token.m_strValue[0] == ')') { token.m_Type = CloseParen; return; }
	if (token.m_strValue[0] == '{') { token.m_Type = OpenBrace; return; }
	if (token.m_strValue[0] == '}') { token.m_Type = CloseBrace; return; }
	if (token.m_strValue[0] == '\"') { token.m_Type = Quote; return; }

	// if we got here, it's not a quote... so convert it to uppercase.
	//MakeUpperCase(token.m_strValue);
	strupr((char*)token.m_strValue.c_str());

	// keywords are easy to figure out, just look for the text...
	if (token.m_strValue.find("PARTICLESYSTEM") != std::string::npos) { token.m_Type = KeywordParticleSystem;	return; }
	if (token.m_strValue.find("EVENTSEQUENCE") != std::string::npos) { token.m_Type = KeywordEventSequence;	return; }
	if (token.m_strValue.find("RANDOM") != std::string::npos) { token.m_Type = KeywordRandom;			return; }
	if (token.m_strValue.find("XYZ") != std::string::npos) { token.m_Type = KeywordXYZ;				return; }
	if (token.m_strValue.find("RGBA") != std::string::npos) { token.m_Type = KeywordColor;			return; }
	if (token.m_strValue.find("FADE") != std::string::npos) { token.m_Type = KeywordFade;				return; }

	if (token.m_strValue.find("INITIAL") != std::string::npos) { token.m_Type = KeywordInitial;			return; }
	if (token.m_strValue.find("FINAL") != std::string::npos) { token.m_Type = KeywordFinal;			return; }
	if (token.m_strValue.find("TEXTURE") != std::string::npos) { token.m_Type = KeywordTexture;			return; }
	if (token.m_strValue.find("FROM") != std::string::npos) { token.m_Type = KeywordFrom;				return; }

	// these two keywords are embedded in other words, so we've got to be careful.
	if (token.m_strValue.find("XY") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordXY; return; }
	if (token.m_strValue.find("SO") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordSo; return; }
	if (token.m_strValue.find("AT") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordAt; return; }
	if (token.m_strValue.find("TO") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordTo; return; }

	// now it gets harder...
	if (token.m_strValue.find("D3DBLEND_") != std::string::npos) { token.m_Type = AlphaBlendMode; return; }
	if (token.m_strValue.find("SOURCEBLENDMODE") != std::string::npos ||
		token.m_strValue.find("DESTBLENDMODE") != std::string::npos)
	{
		token.m_Type = SeqAlphaBlendModeProp; return;
	}

	if (token.m_strValue.find("LIFETIME") != std::string::npos ||
		token.m_strValue.find("EMITRATE") != std::string::npos ||
		token.m_strValue.find("NUMPARTICLES") != std::string::npos ||
		token.m_strValue.find("KILLTIME") != std::string::npos ||
		token.m_strValue.find("KILLNUM") != std::string::npos ||
		token.m_strValue.find("LAYER") != std::string::npos)
	{
		token.m_Type = SeqNumericProp; return;
	}

	if (token.m_strValue.find("GRAVITY") != std::string::npos ||
		token.m_strValue.find("EMITRADIUS") != std::string::npos)
	{
		token.m_Type = SeqVectorProp; return;
	}

	if (token.m_strValue.find("DIRSPEED") != std::string::npos ||
		token.m_strValue.find("BLACKHOLESPEED") != std::string::npos ||
		token.m_strValue.find("EVENTTIMER") != std::string::npos)
	{
		token.m_Type = ParticleNumericProp; return;
	}

	if (token.m_strValue.find("SIZE") != std::string::npos)
	{
		token.m_Type = ParticleVector2Prop; return;
	}

	if (token.m_strValue.find("VELOCITY") != std::string::npos ||
		token.m_strValue.find("ROTATE") != std::string::npos)
	{
		token.m_Type = ParticleVectorProp; return;
	}

	if (token.m_strValue.find("COLOR") != std::string::npos)
	{
		token.m_Type = ParticleColorProp; return;
	}
}

const int CKTDGParticleSystem::CParticleEmitterTokenizer::NUMBLENDINGMODES = 11;
const CKTDGParticleSystem::BLENDINGMODE CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[11] =
{
	{ "D3DBLEND_ZERO",			D3DBLEND_ZERO			},
	{ "D3DBLEND_ONE",			D3DBLEND_ONE			},
	{ "D3DBLEND_SRCCOLOR",		D3DBLEND_SRCCOLOR		},
	{ "D3DBLEND_INVSRCCOLOR",	D3DBLEND_INVSRCCOLOR	},
	{ "D3DBLEND_SRCALPHA",		D3DBLEND_SRCALPHA		},
	{ "D3DBLEND_INVSRCALPHA",	D3DBLEND_INVSRCALPHA	},
	{ "D3DBLEND_DESTALPHA",		D3DBLEND_DESTALPHA		},
	{ "D3DBLEND_INVDESTALPHA",	D3DBLEND_INVDESTALPHA	},
	{ "D3DBLEND_DESTCOLOR",		D3DBLEND_DESTCOLOR		},
	{ "D3DBLEND_INVDESTCOLOR",	D3DBLEND_INVDESTCOLOR	},
	{ "D3DBLEND_SRCALPHASAT",	D3DBLEND_SRCALPHASAT	}
};

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessNumber(CMinMax<float> &number, std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// the first token is either the random keyword, or it's an actual number.
	switch (TokenIter->m_Type)
	{
	case KeywordRandom:
	{
		// parse random number into minmax
		NextToken(TokenIter, EndIter);
		if (TokenIter->m_Type != OpenParen) assert(0 && "Expecting opening paren after Random keyword.");
		NextToken(TokenIter, EndIter);
		if (TokenIter->m_Type != RealNumber) assert(0 && "Expecting first number within Random(...).");
		number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
		NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != Comma) assert(0 && "Expecting comma within Random(...).");
		NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != RealNumber) assert(0 && "Expecting second number within Random(...).");
		number.m_Max = (float)atof(TokenIter->m_strValue.c_str());
		NextToken(TokenIter, EndIter);
		if (TokenIter->m_Type != CloseParen) assert(0 && "Missing close paren on Random(...).");
		NextToken(TokenIter, EndIter);
	}
	break;

	case RealNumber:
		// min and max both equal realnumber
		if (TokenIter->m_Type != RealNumber) assert(0 && "Expecting number.");

		number.m_Max = number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
		NextToken(TokenIter, EndIter);
		break;

	default: assert(0 && "Expecting either Random(...) or a number value.");
	}
	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector(CMinMax<D3DXVECTOR3> &v,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a XYZ keyword.
	if (TokenIter->m_Type != KeywordXYZ) assert(0 && "Expecting XYZ(...)!");

	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) assert(0 && "Expecting ( after XYZ!");

	CMinMax<float> x;
	CMinMax<float> y;
	CMinMax<float> z;

	NextToken(TokenIter, EndIter);
	ProcessNumber(x, TokenIter, EndIter);

	if (TokenIter->m_Type != Comma) assert(0 && "Vector components must be seperated by a comma.");

	NextToken(TokenIter, EndIter);
	ProcessNumber(y, TokenIter, EndIter);

	if (TokenIter->m_Type != Comma) assert(0 && "Vector components must be seperated by a comma.");

	NextToken(TokenIter, EndIter);
	ProcessNumber(z, TokenIter, EndIter);

	if (TokenIter->m_Type != CloseParen) assert(0 && "Expecting ) to close vector.");

	NextToken(TokenIter, EndIter);

	v.m_Min = D3DXVECTOR3(x.m_Min, y.m_Min, z.m_Min);
	v.m_Max = D3DXVECTOR3(x.m_Max, y.m_Max, z.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector2(CMinMax<D3DXVECTOR2> &v,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a XYZ keyword.
	if (TokenIter->m_Type != KeywordXY) assert(0 && "Expecting XY(...)!");
	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) assert(0 && "Expecting ( after XY!");

	CMinMax<float> x;
	CMinMax<float> y;

	NextToken(TokenIter, EndIter); ProcessNumber(x, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) assert(0 && "Vector components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(y, TokenIter, EndIter);
	if (TokenIter->m_Type != CloseParen) assert(0 && "Expecting ) to close vector.");
	NextToken(TokenIter, EndIter);

	v.m_Min = D3DXVECTOR2(x.m_Min, y.m_Min);
	v.m_Max = D3DXVECTOR2(x.m_Max, y.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessColor(CMinMax<D3DXCOLOR> &c,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// this token needs to be a RGBA keyword.
	if (TokenIter->m_Type != KeywordColor)
		assert(0 && "Expecting RGBA(...)!");
	NextToken(TokenIter, EndIter);

	if (TokenIter->m_Type != OpenParen) assert(0 && "Expecting ( after RGBA!");

	CMinMax<float> r;
	CMinMax<float> g;
	CMinMax<float> b;
	CMinMax<float> a;

	NextToken(TokenIter, EndIter); ProcessNumber(r, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) assert(0 && "Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(g, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) assert(0 && "Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(b, TokenIter, EndIter);
	if (TokenIter->m_Type != Comma) assert(0 && "Color components must be seperated by a comma.");
	NextToken(TokenIter, EndIter); ProcessNumber(a, TokenIter, EndIter);
	if (TokenIter->m_Type != CloseParen) assert(0 && "Expecting ) to close vector.");
	NextToken(TokenIter, EndIter);

	SETCOLOR(c.m_Min, r.m_Min, g.m_Min, b.m_Min, a.m_Min);
	SETCOLOR(c.m_Max, r.m_Max, g.m_Max, b.m_Max, a.m_Max);

	return(true);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessAlphaBlendMode(int &alphablendmode,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	if (TokenIter->m_Type != AlphaBlendMode)
		assert(0 && "Expecting alpha blend mode (D3DBLEND_ZERO, D3DBLEND_ONE, etc.).");
	for (int q = 0; q < NUMBLENDINGMODES; q++)
	{
		// careful here... must match on both string and size
		// (i.e., to differentiate between D3DBLEND_SRCALPHA and D3DBLEND_SRCALPHASAT).
		if (TokenIter->m_strValue.find(m_BlendingModes[q].name) != std::string::npos &&
			TokenIter->m_strValue.size() == strlen(m_BlendingModes[q].name))
		{
			alphablendmode = m_BlendingModes[q].mode;
			NextToken(TokenIter, EndIter);
			return(true);
		}
	}

	assert(0 && "Invalid alpha blending mode!");
	return(false);
}

bool CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessTime(CMinMax<float> &TimeRange,
	bool &IsFade, const float InitialTime, const float FinalTime,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	// determine if this time specification is a fade directive
	if (TokenIter->m_Type == KeywordFade)
	{
		// it is... the next token must be "so"
		NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != KeywordSo)
			assert(0 && "Expecting \"so\" after \"fade\".");

		// flip the fade flag on
		IsFade = true;

		// move to next token (so that we land on "at" for the code below)
		NextToken(TokenIter, EndIter);
	}
	else
		IsFade = false; // just to be safe

	switch (TokenIter->m_Type)
	{
	case KeywordFrom:
	{
		// easy, just grab the time
		NextToken(TokenIter, EndIter);
		ProcessNumber(TimeRange, TokenIter, EndIter);

		float startTime = TimeRange.m_Min;

		if (TokenIter->m_Type != KeywordTo)
			assert(0 && "Expecting \"To\" after \"From\".");

		// easy, just grab the time
		NextToken(TokenIter, EndIter);
		ProcessNumber(TimeRange, TokenIter, EndIter);

		TimeRange.m_Min = startTime;
	}
	break;

	case KeywordAt:
	{
		// easy, just grab the time
		NextToken(TokenIter, EndIter);
		ProcessNumber(TimeRange, TokenIter, EndIter);
	}
	break;

	case KeywordInitial:
	{
		// use initial time that was passed in.
		if (IsFade) assert(0 && "Cannot use \"fade so\" modifier on \"initial\" times.");
		TimeRange.m_Min = InitialTime;
		TimeRange.m_Max = InitialTime;
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	}
	break;

	case KeywordFinal:
	{
		// use final time that was passed in.
		TimeRange.m_Min = InitialTime;
		TimeRange.m_Max = FinalTime;
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	}
	break;

	default: assert(0 && "Expected time specifier: \"[fade so] at\", \"initial\", or \"[fade so] final\"");
	}

	return(true);
}

void CKTDGParticleSystem::OpenScriptFile(char* filename)
{
	char charIn = 0;
	long filesize = 0;
	char* script = NULL;

	int fhdl;
	errno_t erno = _sopen_s(&fhdl, filename, _O_RDONLY | _O_TEXT, _SH_DENYWR, _S_IREAD);
	if (erno != 0)
	{
		_close(fhdl);
		return;
	}
	filesize = _filelength(fhdl);
	script = new char[filesize + 10];
	ZeroMemory(script, sizeof(char) * (filesize + 10));
	_read(fhdl, script, filesize);
	Compile(script);
	delete[] script;
	_close(fhdl);
}

bool CKTDGParticleSystem::Compile(const char *script)
{
	m_strLastError = L"0 error(s), you're good to go!";

	try
	{
		CParticleEmitterTokenizer tokenizer;

		// parse the character string into tokens.
		tokenizer.Tokenize(script);

		std::vector<CParticleEmitterToken>::iterator TokenIter = tokenizer.m_TokenVector.begin();
		std::vector<CParticleEmitterToken>::iterator EndIter = tokenizer.m_TokenVector.end();

		if (tokenizer.m_TokenVector.size() < 2) assert(0 && L"This script is too small to be valid.");

		// the first three tokens out of the gate should be
		// ParticleSystem, followed by a name and version number, then
		// an open brace.
		if (TokenIter->m_Type != KeywordParticleSystem) { assert(0 && L"First word must be ParticleSystem"); }
		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != Quote) { assert(0 && L"Must name particle system"); }

		m_strName = RemoveQuotes(TokenIter->m_strValue);
		if (m_strName.compare("KTDGParticleScript") != 0)
		{
			assert(0 && L"ParticleSystem Name Unmatched");
		}

		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != RealNumber) { assert(0 && L"Must have version number"); }

		m_strName = RemoveQuotes(TokenIter->m_strValue);

		if (m_strName.compare("2.00") != 0)
		{
			assert(0 && L"ParticleSystem Version Unmatched");
		}

		CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

		if (TokenIter->m_Type != OpenBrace) { assert(0 && L"Missing opening brace for ParticleSystem block"); }

		ProcessParticleSystemBlock(TokenIter, EndIter);
	}
	catch (WCHAR *e)
	{
		m_strLastError = e;
		MessageBoxW(NULL, m_strLastError.c_str(), NULL, NULL);
		return(false);
	}
	catch (const WCHAR *e)
	{
		m_strLastError = e;
		MessageBoxW(NULL, m_strLastError.c_str(), NULL, NULL);
		return(false);
	}
	catch (...)
	{
		m_strLastError = L"Unhandled exception in CParticleEmitter::Compile.";
		MessageBoxW(NULL, m_strLastError.c_str(), NULL, NULL);
		return(false);
	}

	return true;
}

bool CKTDGParticleSystem::ProcessParticleSystemBlock(std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	bool StartedProcessingSequences = false;
	// move past the opening brace...
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	while (TokenIter->m_Type != CloseBrace)
	{
		CParticleEmitterToken savedtoken;
		savedtoken = *TokenIter;

		// the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
		// or an EventSequence.
		switch (TokenIter->m_Type)
		{
		case KeywordEventSequence:
		{
			StartedProcessingSequences = true;
			CParticleEventSequence *newseq = new CParticleEventSequence();

			if (!ProcessEventSequenceBlock(*newseq, TokenIter, EndIter))
			{
				delete newseq;
			}
			else
			{
				newseq->SetTexture(g_pGCDeviceManager->CreateTexture(newseq->GetTexName()));
#ifdef	_DEBUG
				if (m_mapSequencePool[newseq->GetName()])
				{
					assert(0 && "Duplication is in Particle Script!"&& newseq->GetName().c_str());
				}
#endif	//_DEBUG

				m_mapSequencePool[newseq->GetName()] = newseq;
			}
		}
		break;

		default:
		{
			char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Particle System Block!", TokenIter->m_strValue.c_str());
			assert(0 && &buf[0]);
		}
		break;
		}
	}
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::ProcessEventSequenceBlock(CParticleEventSequence &seq,
	std::vector<CParticleEmitterToken>::iterator &TokenIter,
	std::vector<CParticleEmitterToken>::iterator &EndIter)
{
	bool StartedProcessingEvents = false;

	// move past the event sequence keyword...
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	// next token should be the name of the sequence...
	if (TokenIter->m_Type != Quote) assert(0 && "Must name particle sequence block!");
	seq.SetName(RemoveQuotes(TokenIter->m_strValue));
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	// next token should be opening brace...
	if (TokenIter->m_Type != OpenBrace) assert(0 && "Expected opening brace for particle sequence block!");
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	while (TokenIter->m_Type != CloseBrace)
	{
		CParticleEmitterToken savedtoken;
		savedtoken = *TokenIter;

		// the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
		// or an EventSequence.
		switch (TokenIter->m_Type)
		{
		case SeqNumericProp:
		{
			if (StartedProcessingEvents) assert(0 && "Cannot specify any sequence properties after specifying events.");
			CMinMax<float> number;
			// the next 2 tokens should be an equals, and a number.
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			if (TokenIter->m_Type != Equals) assert(0 && "Expected equals sign!");
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			CParticleEmitterTokenizer::ProcessNumber(number, TokenIter, EndIter);

			if (savedtoken.IsEmitRate()) { seq.SetEmitRate(number); }
			else if (savedtoken.IsLifeTime()) { seq.SetLifetime(number); }
			else if (savedtoken.IsNumParticles()) { seq.SetMaxParticleNum((int)number.m_Max); }
			else if (savedtoken.IsKillTime()) { seq.SetKillTime(number.m_Max); }
			else if (savedtoken.IsKillNum()) { seq.SetKillNum(static_cast<int>(number.m_Max)); }
			else if (savedtoken.IsLayer()) { seq.SetLayer(static_cast<int>(number.m_Max)); }
			else
			{
				assert(0 && "Unknown sequence numeric property!");
			}
		}
		break;

		case SeqVectorProp:
		{
			if (StartedProcessingEvents) assert(0 && "Cannot specify any sequence properties after specifying events.");
			CMinMax<D3DXVECTOR3> v;
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			if (TokenIter->m_Type != Equals) assert(0 && "Expected equals sign!");
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);

			if (savedtoken.IsEmitRadius()) { seq.SetEmitRadius(v); }
			else if (savedtoken.IsGravity()) { seq.SetGravity(v); }
			else
			{
				assert(0 && "Unknown sequence vector property!");
			}
		}
		break;

		case SeqAlphaBlendModeProp:
		{
			if (StartedProcessingEvents) assert(0 && "Cannot specify any sequence properties after specifying events.");
			int alphablendmode;
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			if (TokenIter->m_Type != Equals) assert(0 && "Expected equals sign!");
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			CParticleEmitterTokenizer::ProcessAlphaBlendMode(alphablendmode, TokenIter, EndIter);

			if (savedtoken.IsSrcBlendMode()) { seq.SetSrcBlendMode(alphablendmode); }
			else if (savedtoken.IsDestBlendMode()) { seq.SetDestBlendMode(alphablendmode); }
			else
			{
				assert(0 && "Unknown sequence alpha blending mode property!");
			}
		}
		break;

		case KeywordTexture:
		{
			// TODO
			if (StartedProcessingEvents) assert(0 && "Cannot specify any sequence properties after specifying events.");
			CMinMax<D3DXVECTOR3> v;
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
			if (TokenIter->m_Type != Equals) assert(0 && "Expected equals sign!");
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

			if (TokenIter->m_Type != Quote) assert(0 && "Expected filename after texture sequence property.");
			RemoveQuotes(TokenIter->m_strValue);

			seq.SetTextureName(TokenIter->m_strValue);
			CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
		}
		break;

		case KeywordFade:
		case KeywordAt:
		case KeywordFrom:
		case KeywordTo:
		case KeywordInitial:
		case KeywordFinal:
		{
			StartedProcessingEvents = true;

			bool IsFade = false;
			CMinMax<float> TimeRange;

			// parse the time range section of the event line
			CParticleEmitterTokenizer::ProcessTime(TimeRange, IsFade, 0, seq.GetLifeTime().m_Max, TokenIter, EndIter);

			if (TokenIter->m_Type != ParticleNumericProp &&
				TokenIter->m_Type != ParticleVectorProp &&
				TokenIter->m_Type != ParticleVector2Prop &&
				TokenIter->m_Type != ParticleColorProp)
				assert(0 && "Expecting particle property after time specifier!");

			CParticleEvent * NewEvent = NULL;
			try
			{
				// create the appropriate event
				NewEvent = EventFactory(TokenIter->m_strValue);

				if (!NewEvent) assert(0 && "Unknown event type, or there was an error creating this event.");

				// let the event parse the rest of its properties from the token stream.
				if (IsFade && !NewEvent->FadeAllowed()) assert(0 && "Fading is not supported on this event.");
				NewEvent->ProcessTokenStream(TokenIter, EndIter);
				NewEvent->SetActualTime(TimeRange);
				NewEvent->SetFade(IsFade);
				seq.m_Events.push_back(NewEvent);
			}
			catch (char *e)
			{
				SAFE_DELETE(NewEvent);
				assert(0 && e);
			}
			catch (...)
			{
				SAFE_DELETE(NewEvent);
				assert(0 && "Unhandled exception creating event!");
			}
		}
		break;

		default:
		{
			char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Sequence Block!", TokenIter->m_strValue.c_str());
			assert(0 && &buf[0]);
		}
		break;
		}
	}

	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	return(true);
}

CKTDGParticleSystem::CParticleEvent* CKTDGParticleSystem::EventFactory(const std::string& EventName)
{
	CParticleEvent *event = NULL;

	if (EventName.find("SIZE") != std::string::npos)			event = new CParticleEvent_Size();
	else if (EventName.find("EVENTTIMER") != std::string::npos)	event = new CParticleEvent_EventTimer();
	else if (EventName.find("COLOR") != std::string::npos)		event = new CParticleEvent_Color();
	else if (EventName.find("VELOCITYX") != std::string::npos)	event = new CParticleEvent_VelocityX();
	else if (EventName.find("VELOCITYY") != std::string::npos)	event = new CParticleEvent_VelocityY();
	else if (EventName.find("VELOCITYZ") != std::string::npos)	event = new CParticleEvent_VelocityZ();
	else if (EventName.find("VELOCITY") != std::string::npos)	event = new CParticleEvent_Velocity();
	else if (EventName.find("ROTATEX") != std::string::npos)	event = new CParticleEvent_RotateX();
	else if (EventName.find("ROTATEY") != std::string::npos)	event = new CParticleEvent_RotateY();
	else if (EventName.find("ROTATEZ") != std::string::npos)	event = new CParticleEvent_RotateZ();
	else if (EventName.find("ROTATE") != std::string::npos)		event = new CParticleEvent_Rotate();
	else if (EventName.find("DIRSPEED") != std::string::npos)	event = new CParticleEvent_DirSpeed();
	else if (EventName.find("BLACKHOLESPEED") != std::string::npos)	event = new CParticleEvent_BlackHole();

	return(event);
}

void CKTDGParticleSystem::InitDevice()
{
	m_pVB = NULL;
	m_iVBSize = MAX_PARTICLE * 6 * sizeof(VERTEX_PARTICLE);

	m_pVB = g_pGCDeviceManager->CreateVertexBuffer("CKTDGParticleSystem_m_pVB", m_iVBSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFVF_NEW_PARTICLE,
		D3DPOOL_DEFAULT,
		NULL);
}

void CKTDGParticleSystem::Clear()
{
	DestroyAllInstance();

	std::map<std::string, CParticleEventSequence*>::iterator itor;
	for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
	{
		for (int j = 0; j < (int)itor->second->m_Events.size(); ++j)
		{
			SAFE_DELETE(itor->second->m_Events[j]);
		}
		SAFE_DELETE(itor->second);
	}

	m_mapSequencePool.clear();
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CreateSequence(const std::string& name, float x, float y, float z)
{
	CParticleEventSequence* retval = NULL;

	std::map<std::string, CParticleEventSequence*>::iterator itor;
	itor = m_mapSequencePool.find(name);
	if (itor == m_mapSequencePool.end())
		return NULL;

	retval = itor->second->GetCloneSequence();
	retval->SetPosition(x, y, z);

	m_mapInstance[retval->GetLayer()].push_back(retval);

	return retval;
}

void CKTDGParticleSystem::DestroyInstance(CParticleEventSequence* sequence)
{
	std::map< int, std::vector<CParticleEventSequence*> >::iterator itor;
	for (itor = m_mapInstance.begin(); itor != m_mapInstance.end(); ++itor)
	{
		std::vector<CParticleEventSequence*>& vecInstance = itor->second;
		std::vector<CParticleEventSequence*>::iterator i;
		for (i = vecInstance.begin(); i != vecInstance.end(); i++)
		{
			if ((*i) == sequence)
			{
				vecInstance.erase(i);
				SAFE_DELETE(sequence);

				if (vecInstance.size() == 0)
					m_mapInstance.erase(itor);

				return;
			}
		}
	}
}

void CKTDGParticleSystem::DestroyAllInstance()
{
	std::map< int, std::vector<CParticleEventSequence*> >::iterator itor;

	for (itor = m_mapInstance.begin(); itor != m_mapInstance.end(); ++itor)
	{
		std::vector<CParticleEventSequence*>& vecInstance = itor->second;
		for (int i = 0; i < (int)vecInstance.size(); i++)
		{
			SAFE_DELETE(vecInstance[i]);
		}

		vecInstance.clear();
	}

	m_mapInstance.clear();
}

bool CKTDGParticleSystem::IsLiveInstance(CParticleEventSequence* inst)
{
	std::map< int, std::vector<CParticleEventSequence*> >::iterator itor;

	for (itor = m_mapInstance.begin(); itor != m_mapInstance.end(); ++itor)
	{
		std::vector<CParticleEventSequence*>& vecInstance = itor->second;
		std::vector<CParticleEventSequence*>::iterator i;

		for (i = vecInstance.begin(); i != vecInstance.end(); i++)
		{
			if (inst == (*i))
				return true;
		}
	}

	return false;
}

void CKTDGParticleSystem::CheckLifeTime(CParticleEventSequence* pSequence)
{
	// 라이프타임이 다되면 죽어라
	float fKillTime = pSequence->GetKillTime();
	if (fKillTime != 0.0f)
	{
		if (pSequence->GetCurrLifeTime() > fKillTime)
		{
			pSequence->SetAutoDie();
		}
	}

	if (pSequence->GetKillNum() > 0)
	{
		if (pSequence->GetTotalEmitNum() >= pSequence->GetKillNum())
		{
			pSequence->SetAutoDie();
		}
	}
}

void CKTDGParticleSystem::FrameMove(float fTime, float fElapsedTime)
{
	CParticleEventSequence* pSequence = NULL;

	std::map< int, std::vector<CParticleEventSequence*> >::iterator itor = m_mapInstance.begin();
	while (itor != m_mapInstance.end())
	{
		std::vector<CParticleEventSequence*>& vecInstance = itor->second;

		for (int i = static_cast<int>(vecInstance.size()) - 1; i >= 0; --i)
		{
			pSequence = vecInstance[i];
			pSequence->OnFrameMove(fTime, fElapsedTime);

			if (pSequence->IsAutoDie() == false)
			{
				CheckLifeTime(pSequence);
			}

			if (pSequence->IsAutoDie() && pSequence->GetLiveParticleNum() == 0)
			{
				vecInstance.erase(vecInstance.begin() + i);
				SAFE_DELETE(pSequence);
			}
		}

		if (vecInstance.size() == 0)
			itor = m_mapInstance.erase(itor);
		else
			++itor;
	}
}

void CKTDGParticleSystem::Render(int iLayer)
{
	std::map< int, std::vector<CParticleEventSequence*> >::iterator itor;
	itor = m_mapInstance.find(iLayer);
	if (itor == m_mapInstance.end())
		return;

	KD3DDeviceState::PushRenderStateSet();
	{
		KD3DDeviceState::PushRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		KD3DDeviceState::PushRenderState(D3DRS_ZENABLE, FALSE);
		KD3DDeviceState::PushRenderState(D3DRS_ZWRITEENABLE, FALSE);
		KD3DDeviceState::PushRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		KD3DDeviceState::PushRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		KD3DDeviceState::PushRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
		KD3DDeviceState::PushTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		{
			std::vector<CParticleEventSequence*>& vecInstance = itor->second;
			std::vector<CParticleEventSequence*>::iterator i;
			for (i = vecInstance.begin(); i != vecInstance.end(); i++)
			{
				KD3DDeviceState::PushRenderState(D3DRS_SRCBLEND, (*i)->GetSrcBlendMode());
				KD3DDeviceState::PushRenderState(D3DRS_DESTBLEND, (*i)->GetDestBlendMode());
				(*i)->OnFrameRender(m_pVB);
				KD3DDeviceState::PopRenderState(D3DRS_DESTBLEND);
				KD3DDeviceState::PopRenderState(D3DRS_SRCBLEND);
			}
		}
		KD3DDeviceState::PopTextureStageState(0, D3DTSS_ALPHAOP);
	}
	KD3DDeviceState::PopRenderStateSet();
}

// 2007/2/20. iridology. pParticle을 iLayer로 옮기는 함수
bool CKTDGParticleSystem::SetLayer(CKTDGParticleSystem::CParticleEventSequence* pParticle, int iLayer)
{
	std::map< int, std::vector<CParticleEventSequence*> >::iterator mit = m_mapInstance.find(pParticle->GetLayer());
	if (mit != m_mapInstance.end())
	{
		std::vector<CParticleEventSequence*>& vecSeq = mit->second;
		std::vector<CParticleEventSequence*>::iterator vit = std::find(vecSeq.begin(), vecSeq.end(), pParticle);

		if (vit == vecSeq.end())
			return false;

		vecSeq.erase(vit);
		pParticle->SetLayer(iLayer);
		m_mapInstance[iLayer].push_back(pParticle);
		return true;
	}

	return false;
}