#include "stdafx.h"

#include "KTDGParticleSystem.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include "GCClientErrorManager.h"
#include "KNC/ClientErr.h"

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

boost::pool<> CKTDGParticleSystem::CParticle::bpool(sizeof(CKTDGParticleSystem::CParticle));
boost::pool<> CKTDGParticleSystem::CParticleEventSequence::bpool(sizeof(CKTDGParticleSystem::CParticleEventSequence));

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
		part.m_Color = m_Color.GetRandomNumInRangeTable();
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
		part.m_vSize = m_Size.GetRandomNumInRangeTable();
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
		part.m_vVelocity = m_Velocity.GetRandomNumInRangeTable();

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
		part.m_vVelocity.x = m_VelocityX.GetRandomNumInRangeTable();
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
		part.m_vVelocity.y = m_VelocityY.GetRandomNumInRangeTable();
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
		part.m_vVelocity.z = m_VelocityZ.GetRandomNumInRangeTable();
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
		part.m_vRotate = m_Rotate.GetRandomNumInRangeTable();
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
		part.m_vRotate.x = m_RotateX.GetRandomNumInRangeTable();
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
		part.m_vRotate.y = m_RotateY.GetRandomNumInRangeTable();
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
		part.m_vRotate.z = m_RotateZ.GetRandomNumInRangeTable();
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
		part.m_vDirSpeed = m_DirSpeed.GetRandomNumInRangeTable();
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
	part.m_BlackHoleTime = m_BlackHole.GetRandomNumInRangeTable();
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
	part.m_fEventTimer = m_EventTimer.GetRandomNumInRangeTable();
}

///////////////////////////////////////////////////////////////////////////////
// Particle Event Sequence
///////////////////////////////////////////////////////////////////////////////

CKTDGParticleSystem::CParticleEventSequence::CParticleEventSequence() : m_bSlowCount(false)
{
	m_nBone = -1;

	m_bAutoDie = false;

	SETVECTOR3(m_Position, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_RotateOffset, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_PosDiff, 0.0f, 0.0f, 0.0f);
	m_pBlackHolePosition = &m_Position;

	m_pbRender = NULL;
	m_bRender = true;

	m_fNumNewPartsExcess = 0.0f;
	m_fKillTime = 0.0f;
	m_fCurrLife = 0.0f;
	m_iKillNum = 0;
	m_iTotalEmitNum = 0;

	m_iLayer = 5;

	m_bUserAdditionalZRotate = false;
	m_fAdditionalZRotate = 0.f;
	SETVECTOR3(m_posBegin, 0.0f, 0.0f, 0.0f); // 데미지 생성 위치.
	m_bUseInternalBlackholePos = false;
	SETVECTOR3(m_vposInternalBlackhole, 0.0f, 0.0f, 0.0f);

	m_bRenderForImageTag = true;

	m_dwUID = UINT_MAX;
}

CKTDGParticleSystem::CParticleEventSequence::~CParticleEventSequence()
{
	while (!m_Particles.empty())
	{
		if (m_Particles.back())
			delete m_Particles.back();
		m_Particles.pop_back();
	}

	m_dwUID = UINT_MAX;
}

void CKTDGParticleSystem::CParticleEventSequence::SetTracePosition(D3DXVECTOR3* vPos)
{
	//====================================================================================
	// 2009.02.09 : Jemitgge
	// Comment : 빵사장님 이상하면 고쳐주시지 그랬어요 ㅠㅠ 파티클의 offset도
	//			 변화한 각도만큼 움직여 줘야한다...
	try {
		if (vPos && m_stTrace.m_pvPos)
			*vPos = *m_stTrace.m_pvPos;
	}
	catch (...) {
		//  데이터가 거의 없음
		SiGCClientErrorManager()->ErrorCollect(KEventErr::ERR_CLIENTRPT, KClientErr::CE_PARTICLE_TRACE_POS);
		return;
	}

	if (m_stTrace.m_fSpeedAngle != 0.0f)
	{
		D3DXVECTOR3 vOffsetPos(m_stTrace.m_fXOffset, m_stTrace.m_fYOffset, 0.5f);
		D3DXVECTOR3 vDestPos;
		D3DXMATRIX matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationZ(&matRot, m_stTrace.m_fSpeedAngle);
		D3DXVec3TransformCoord(&vDestPos, &vOffsetPos, &matRot);

		m_stTrace.m_fXOffset = vDestPos.x;
		m_stTrace.m_fYOffset = vDestPos.y;
	}

	vPos->x += m_stTrace.m_fXOffset;
	vPos->y += m_stTrace.m_fYOffset;
	vPos->z = 0.5f;
}

bool CKTDGParticleSystem::CParticleEventSequence::IsBlackHoleEventHere()
{
	return (m_pParticleEventSeq->m_Events.end() != std::find_if(m_pParticleEventSeq->m_Events.begin(), m_pParticleEventSeq->m_Events.end(),
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
		SetTracePosition(&vNewPos);

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

	std::list<CParticle*>::iterator listEnd = m_Particles.end();
	std::list<CParticle*>::iterator listIter = m_Particles.begin();
	while (listIter != listEnd)
	{
		CParticle* pCParticle = (*listIter);
		if (pCParticle->OnFrameMove(fElapsedTime) == false)
		{
			SAFE_DELETE(*listIter);
			listIter = m_Particles.erase(listIter);
			listEnd = m_Particles.end();
			continue;
		}
		else
		{
			//중력 적용
			pCParticle->m_vVelocity += GetGravity().GetRandomNumInRangeTable() * fElapsedTime;

			//블랙홀 포인트
			if (pCParticle->m_BlackHoleTime > 0.0f)
			{
				if (m_pBlackHolePosition != NULL)
					pCParticle->m_BlackHoleSpeed = (*m_pBlackHolePosition - pCParticle->m_vPos) / pCParticle->m_BlackHoleTime;
				else
				{
					D3DXVECTOR3 hole(0.0f, 0.0f, 0.0f);
					pCParticle->m_BlackHoleSpeed = (hole - pCParticle->m_vPos) / pCParticle->m_BlackHoleTime;
				}
			}

			//이벤트 적용
			RunEvent(*(*listIter));
		}

		// 파티클 Trace 처리
		// Sequence의 변화량만큼 변화시켜주라
		if (m_stTrace.m_pvPos != NULL && pCParticle->m_fTraceTime > 0.0f)
		{
			// [10/22/2007] breadceo.Trace Offset 아무데도 안쓰고 이 의미로 쓰면 이상하다
			pCParticle->m_vPos.x += m_PosDiff.x;
			pCParticle->m_vPos.y += m_PosDiff.y;
		}

		pCParticle->m_vOffsetRotate = m_RotateOffset;
		listIter++;
	}

	//파티클 생성
	float fEmitRateThisFrame = m_EmitRate.GetRandomNumInRangeTable();

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

	/*bool bNoRender = ( m_bRender == false || ( m_pbRender != NULL && (*m_pbRender) == false ) || m_bRenderForImageTag == false );

	if( !bNoRender )*/
	{
		for (int q = 0; q < iNumNewParts; q++)
		{
			if (GetMaxParticleNum() > (int)m_Particles.size())
				CreateNewParticle(vNewPos, bReverse);
		}
	}
	m_iTotalEmitNum += iNumNewParts;
}

void CKTDGParticleSystem::CParticleEventSequence::OnFrameRender(D3DXMATRIX * mat)
{
	if (m_bRender == false || (m_pbRender != NULL && (*m_pbRender) == false) || m_bRenderForImageTag == false)
		return;

	int iSize = (int)m_Particles.size();
	ASSERT(iSize <= MAX_PARTICLE);

	GCDeviceTexture * pTexture = GetTexture();
	if (pTexture == NULL)
		return;

	static SLVERTEX V[MAX_PARTICLE * 4];
	SLVERTEX *pVertices = V;
	D3DXMATRIX finalMat;
	D3DXVECTOR4 v4temp;

	float fTextureCenterX = pTexture->GetWidth() / 2.0f;
	float fTextureCenterY = pTexture->GetHeight() / 2.0f;

	// Render each particle
	float fLeft = 0.0f;
	float fRight = 1.0f;
	for (std::list<CParticle*>::iterator i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		// Render each particle a bunch of times to get a blurring effect
		CParticle * part = (*i);

		/*if( !bRender && part->m_fLifetime > 3.0f )
			continue;*/

		D3DXMATRIX matTrans;
		D3DXMATRIX matRot;
		D3DXMATRIX matScale;

		D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(part->m_vRotate.y + part->m_vOffsetRotate.y), D3DXToRadian(part->m_vRotate.x + part->m_vOffsetRotate.x), D3DXToRadian(part->m_vRotate.z + part->m_vOffsetRotate.z));
		D3DXMatrixScaling(&matScale, part->m_vSize.x, part->m_vSize.y, 1.0f);

		if (m_bUserAdditionalZRotate)
		{
			// 매트릭스의 포지션을 빼고
			// 추가 로테이션
			// 손의 위치로 포지션 이동.

			D3DXMatrixTranslation(&matTrans, m_posBegin.x, m_posBegin.y, m_posBegin.z);
			D3DXMATRIX matZRot;
			D3DXMatrixRotationZ(&matZRot, m_fAdditionalZRotate);

			finalMat = matScale * matRot * matZRot * matTrans * (*mat);
		}
		else // 리르4 (노바) 활에 이펙트 계산을 위해서 이런 삽질을 했습니다.. 죄송합니다..ㅜㅜ
		{
			D3DXMatrixTranslation(&matTrans, part->m_vPos.x, part->m_vPos.y, part->m_vPos.z);

			finalMat = matScale * matRot * matTrans * (*mat);
		}

		if (!part->m_bReverse)
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
		pVertices->Pos.x = -1.0f * fTextureCenterX;
		pVertices->Pos.y = fTextureCenterY;
		pVertices->Pos.z = 0.0f;
		pVertices->Diff = (DWORD)part->m_Color;
		pVertices->tu = fLeft;
		pVertices->tv = 0.0f;
		D3DXVec3TransformCoord(&pVertices->Pos, &pVertices->Pos, &finalMat);
		pVertices++;

		//2
		pVertices->Pos.x = -1.0f * fTextureCenterX;
		pVertices->Pos.y = -1.0f * fTextureCenterY;
		pVertices->Pos.z = 0.0f;
		pVertices->Diff = (DWORD)part->m_Color;
		pVertices->tu = fLeft;
		pVertices->tv = 1.0f;
		D3DXVec3TransformCoord(&pVertices->Pos, &pVertices->Pos, &finalMat);
		pVertices++;

		//3
		pVertices->Pos.x = fTextureCenterX;
		pVertices->Pos.y = fTextureCenterY;
		pVertices->Pos.z = 0.0f;
		pVertices->Diff = (DWORD)part->m_Color;
		pVertices->tu = fRight;
		pVertices->tv = 0.0f;
		D3DXVec3TransformCoord(&pVertices->Pos, &pVertices->Pos, &finalMat);
		pVertices++;

		//6
		pVertices->Pos.x = fTextureCenterX;
		pVertices->Pos.y = -1.0f * fTextureCenterY;
		pVertices->Pos.z = 0.0f;
		pVertices->Diff = (DWORD)part->m_Color;
		pVertices->tu = fRight;
		pVertices->tv = 1.0f;
		D3DXVec3TransformCoord(&pVertices->Pos, &pVertices->Pos, &finalMat);
		pVertices++;
	}

	g_pGCDeviceManager2->DrawInWorld(pTexture, V, iSize);
}

CKTDGParticleSystem::CParticle* CKTDGParticleSystem::CParticleEventSequence::CreateNewParticle(const D3DXVECTOR3& vPartSysPos, bool bReverse)
{
	CParticle * part = NULL;
	part = new CParticle();

	part->m_fLifetime = GetLifeTime().GetRandomNumInRangeTable();
	part->m_vPos = vPartSysPos + GetEmitRadius().GetRandomNumInRangeTable();

	// process any initial events
	for (std::list<CParticleEvent*>::iterator i = m_pParticleEventSeq->m_Events.begin(); i != m_pParticleEventSeq->m_Events.end(); i++)
	{
		if ((*i)->IsFade() == true) (*i)->SetFinalValue(*part);

		if ((*i)->GetActualTime().m_Min == 0.0f) (*i)->OnFrameMove(*part, m_stDirect);
	}

	part->m_bReverse = bReverse;
	part->m_fTraceTime = m_stTrace.m_fParticleTraceTime;
	m_Particles.push_back(part);

	return part;
}

void CKTDGParticleSystem::CParticleEventSequence::RunEvent(CParticle &part)
{
	std::list<CParticleEvent*>::reverse_iterator listREndIter = m_pParticleEventSeq->m_Events.rend();
	for (std::list<CParticleEvent*>::reverse_iterator i = m_pParticleEventSeq->m_Events.rbegin();
		i != listREndIter; i++)
	{
		CMinMax<float> &actualtime = (*i)->GetActualTime();

		if (actualtime.m_Max < part.m_fEventTimerOld)
			break;

		if (part.m_fEventTimer >= actualtime.m_Min)
		{
			if ((*i)->IsFade() == true)
			{
				if (part.m_fEventTimer < actualtime.m_Max)
					(*i)->OnFrameMove(part, m_stDirect);
			}
			else if (part.m_fEventTimerOld < actualtime.m_Min)
			{
				(*i)->OnFrameMove(part, m_stDirect);
			}
		}
	}
}

void CKTDGParticleSystem::CParticleEventSequence::SetParticleEventSeq(CParticleEventSeq * seq)
{
	m_pParticleEventSeq = seq;
	SetEmitRate(seq->GetEmitRate());
	SetKillTime(seq->GetKillTime());
	SetKillNum(seq->GetKillNum());
	SetLayer(seq->GetLayer());
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CParticleEventSeq::GetSequence()
{
	CParticleEventSequence* clone = NULL;

	clone = new CParticleEventSequence();

	clone->SetParticleEventSeq(this);

	return clone;
}

int CKTDGParticleSystem::CParticleEventSeq::GetMemorySize()
{
	int nMemorySize = 0;
#if !defined( __PATH__ )
	nMemorySize += sizeof(this);

	//     if ( m_Texture )
	//         nMemorySize += m_Texture->GetDeviceSize();
#endif

	return nMemorySize;
}

//  디버깅용 코드
//  나중에 삭제 하세요.
void CKTDGParticleSystem::CParticleEventSequence::NewDummyParticleEventSeq()
{
	if (!m_pParticleEventSeq)
	{
		m_pParticleEventSeq = new CParticleEventSeq;
	}
}

void CKTDGParticleSystem::CParticleEventSequence::DestoryParticleEventSeq()
{
	SAFE_DELETE(m_pParticleEventSeq);
}

int CKTDGParticleSystem::CParticleEventSequence::GetMemorySize()
{
	int nMemorySize = 0;
#if !defined( __PATH__ )
	nMemorySize += sizeof(this);
#endif

	return nMemorySize;
}
//  디버깅용 코드

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

void CKTDGParticleSystem::OpenScriptFile(const char* filename)
{
	//위에서 넘어오는 파일이름은 풀경로라서 따로 써준다-)-
	std::string strFileName;
	strFileName = filename;
	int ifindpt = strFileName.rfind("Stage\\");
	if (ifindpt != -1)
		strFileName = strFileName.substr(ifindpt + 6, strFileName.length() - ifindpt - 6);
	GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), strFileName.c_str());
	std::string strFile = filename;
	std::string strFile2;
	strFile2 = strFile.substr(strFile.find_last_of("\\") + 1);

	if (g_pGCDeviceManager2->GetMassFileManager()->IsIn(strFile2))
	{
		GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), strFile2);

		if (!memfile.IsLoaded())
			return;

		Compile(memfile.GetDataPointer());
	}
	else
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
		SAFE_DELETE_ARRAY(script);
		_close(fhdl);
	}

	boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
	for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
	{
		std::stable_sort(itor->second->m_Events.begin(), itor->second->m_Events.end(), CParticleEvent::CompareMax);
	}
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

	FILE *fp = fopen("DllList.txt", "a");
	if (fp == NULL)
		return false;

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
			CParticleEventSeq *newseq = new CParticleEventSeq();

			if (!ProcessEventSequenceBlock(*newseq, TokenIter, EndIter))
			{
				SAFE_DELETE(newseq);
			}
			else
			{
				if (!GC_GLOBAL_DEFINE::bThreadLoadTexture)
				{
					newseq->SetTexture(g_pGCDeviceManager->CreateTexture(newseq->GetTexName()));
				}

#if !defined( __PATH__ )
				if (m_mapSequencePool.find(newseq->GetName()) != m_mapSequencePool.end())
				{
					//assert(0&&"Duplication is in Particle Script!"&& newseq->GetName().c_str() );
					std::ostringstream stm;
					stm << "Duplication is in Particle Script!" << "  Particle : " << newseq->GetName();
					//MessageBoxA( NULL, stm.str().c_str(), "CKTDGParticleSystem::ProcessParticleSystemBlock()", NULL );

					fputs(newseq->GetName().c_str(), fp);
					fputs("\n", fp);
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
	fclose(fp);
	CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
	return(true);
}

bool CKTDGParticleSystem::ProcessEventSequenceBlock(CParticleEventSeq &seq,
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
			catch (...)
			{
				SAFE_DELETE(NewEvent);
				assert(0 && "Unhandled exception creating event!");
			}
		}
		break;

		default:
		{
			assert(0 && &"Unknown Type...");
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
	//for (int i=0;i<MAX_PARTICLE;i++)
	//	CParticle::m_listParticle.push_back(new CParticle());

	m_dwMaxUID = 0;
}

void CKTDGParticleSystem::Clear()
{
	DestroyAllInstance();

	boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
	for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
	{
		while (!itor->second->m_Events.empty())
		{
			if (itor->second->m_Events.back())
				delete itor->second->m_Events.back();
			itor->second->m_Events.pop_back();
		}
		SAFE_DELETE(itor->second);
	}

	m_mapSequencePool.clear();

	m_dwMaxUID = 0;
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::CreateSequence(const std::string& name, float x, float y, float z)
{
	CParticleEventSequence* retval = NULL;

	boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
	itor = m_mapSequencePool.find(name);
	if (itor == m_mapSequencePool.end())
		return NULL;

	retval = itor->second->GetSequence();
	retval->SetPosition(x, y, z);

	//  오버플러우 되면 0부터 설정되지만 42억개니깐 충분하겠지..
	retval->SetUID(m_dwMaxUID);
	m_dwMaxUID++;

	if (m_dwMaxUID == UINT_MAX)
		m_dwMaxUID = 0;

	m_mapInstance[retval->GetLayer()][itor->second].push_back(retval);
	m_mapUIDInstance.insert(std::make_pair(retval->GetUID(), retval));

	if (GC_GLOBAL_DEFINE::bThreadLoadTexture)
	{
		// 텍스쳐 생성되어 있지 않다면 로드 한다.
		if (!itor->second->GetTexture())
		{
			itor->second->SetTexture(g_pGCDeviceManager->CreateTextureThread(itor->second->GetTexName(), false, true));
		}
	}

	return retval;
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGParticleSystem::GetSequence(PARTICLE_UID _dwUID)
{
	std::map<PARTICLE_UID, CParticleEventSequence*>::iterator pos = m_mapUIDInstance.find(_dwUID);
	if (pos == m_mapUIDInstance.end())
		return NULL;

	return pos->second;
}

void CKTDGParticleSystem::DestroyInstance(PARTICLE_UID _dwUID)
{
	std::map<PARTICLE_UID, CParticleEventSequence*>::iterator pos = m_mapUIDInstance.find(_dwUID);
	if (pos == m_mapUIDInstance.end())
		return;

	CParticleEventSequence* sequence = pos->second;

	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator itor;
	itor = m_mapInstance.find(sequence->GetLayer());

	if (itor != m_mapInstance.end())
	{
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq;
		itor_seq = itor->second.find(sequence->GetParticleEventSeq());
		if (itor_seq != itor->second.end())
		{
			std::list<CParticleEventSequence*>::iterator i;
			i = std::find(itor_seq->second.begin(), itor_seq->second.end(), sequence);
			if (i != itor_seq->second.end())
			{
				itor_seq->second.erase(i);
				m_mapUIDInstance.erase(_dwUID);

				SAFE_DELETE(sequence);
			}
		}
	}
}

void CKTDGParticleSystem::DestroyAllInstance()
{
	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator itor;

	for (itor = m_mapInstance.begin(); itor != m_mapInstance.end(); ++itor)
	{
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >& mapseq = itor->second;
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq;

		for (itor_seq = mapseq.begin(); itor_seq != mapseq.end(); itor_seq++)
		{
			std::list<CParticleEventSequence*>& listinstance = itor_seq->second;
			while (!listinstance.empty())
			{
				if (listinstance.back())
				{
					delete listinstance.back();
				}
				listinstance.pop_back();
			}
		}
		mapseq.clear();
	}

	m_mapInstance.clear();
	m_mapUIDInstance.clear();
}

bool CKTDGParticleSystem::IsLiveInstance(PARTICLE_UID _dwUID)
{
	return m_mapUIDInstance.find(_dwUID) != m_mapUIDInstance.end();
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
	PROFILE_SET("CKTDGParticleSystem::FrameMove");

	CParticleEventSequence* pSequence = NULL;

	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator itor = m_mapInstance.begin();

	std::list<int> clear_layer;
	while (itor != m_mapInstance.end())
	{
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > &mapseq = itor->second;
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq = mapseq.begin();

		std::list<CParticleEventSeq*> clear_seq;

		while (itor_seq != mapseq.end())
		{
			std::list<CParticleEventSequence*>& listinstance = itor_seq->second;
			std::list<CParticleEventSequence*>::iterator vit = listinstance.begin();

			while (vit != listinstance.end())
			{
				pSequence = *vit;
				pSequence->OnFrameMove(fTime, fElapsedTime);

				if (pSequence->IsAutoDie() == false)
				{
					CheckLifeTime(pSequence);
				}

				if (pSequence->IsAutoDie() && pSequence->GetLiveParticleNum() == 0)
				{
					m_mapUIDInstance.erase(pSequence->GetUID());
					vit = listinstance.erase(vit);

					SAFE_DELETE(pSequence);
				}
				else
				{
					vit++;
				}
			}

			if (listinstance.empty())
				clear_seq.push_back(itor_seq->first);

			itor_seq++;
		}

		while (!clear_seq.empty())
		{
			mapseq.erase(clear_seq.back());
			clear_seq.pop_back();
		}

		if (mapseq.empty())
			clear_layer.push_back(itor->first);

		itor++;
	}

	while (!clear_layer.empty())
	{
		m_mapInstance.erase(clear_layer.back());
		clear_layer.pop_back();
	}
}

void CKTDGParticleSystem::Render(int iLayer)
{
	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator itor;
	itor = m_mapInstance.find(iLayer);
	if (itor == m_mapInstance.end())
		return;

	D3DXMATRIX mat;
	g_pd3dDevice->GetTransform(D3DTS_WORLD, &mat);

	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		if (m_bZEnable)
		{
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
			g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		}
		else
		{
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		{
			boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > &mapseq = itor->second;
			boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq = mapseq.begin();

			while (itor_seq != mapseq.end())
			{
				g_pGCDeviceManager2->PushState();
				{
					g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, itor_seq->first->GetSrcBlendMode());
					g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, itor_seq->first->GetDestBlendMode());

					std::list<CParticleEventSequence*>& listinstance = itor_seq->second;
					std::list<CParticleEventSequence*>::iterator i = listinstance.begin();
					while (i != listinstance.end())
					{
						(*i)->OnFrameRender(&mat);
						i++;
					}
				}
				g_pGCDeviceManager2->PopState();
				itor_seq++;
			}
		}
	}
	g_pGCDeviceManager2->PopState();
}

// 2007/2/20. iridology. pParticle을 iLayer로 옮기는 함수
bool CKTDGParticleSystem::SetLayer(CKTDGParticleSystem::CParticleEventSequence* pParticle, int iLayer)
{
	if (pParticle == NULL) return false;
	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator mit = m_mapInstance.find(pParticle->GetLayer());
	if (mit != m_mapInstance.end())
	{
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq = mit->second.find(pParticle->GetParticleEventSeq());
		if (itor_seq != mit->second.end())
		{
			std::list<CParticleEventSequence*>& vecSeq = itor_seq->second;
			std::list<CParticleEventSequence*>::iterator vit = std::find(vecSeq.begin(), vecSeq.end(), pParticle);

			if (vit == vecSeq.end())
				return false;

			vecSeq.erase(vit);
			pParticle->SetLayer(iLayer);
			m_mapInstance[iLayer][pParticle->GetParticleEventSeq()].push_back(pParticle);
			return true;
		}
	}
	return false;
}

void CKTDGParticleSystem::SetZEnable(bool bEnable)
{
	m_bZEnable = bEnable;
}

void CKTDGParticleSystem::ReleaseAllTexture()
{
	boost::unordered_map<std::string, CParticleEventSeq*>::iterator mit;

	for (mit = m_mapSequencePool.begin(); mit != m_mapSequencePool.end(); ++mit)
	{
		if (mit->second->GetTexture() == NULL)
			continue;

		mit->second->GetTexture()->Release();
		mit->second->SetTexture(NULL);
	}
}

int CKTDGParticleSystem::GetMemorySize()
{
	int nMemorySize = 0;

#if !defined( __PATH__ )
	boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
	for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
	{
		nMemorySize += itor->second->GetMemorySize();
	}

	boost::unordered_map< int, boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> > >::iterator itorInst;

	for (itorInst = m_mapInstance.begin(); itorInst != m_mapInstance.end(); ++itorInst)
	{
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >& mapseq = itorInst->second;
		boost::unordered_map< CParticleEventSeq*, std::list<CParticleEventSequence*> >::iterator itor_seq;

		for (itor_seq = mapseq.begin(); itor_seq != mapseq.end(); itor_seq++)
		{
			std::list<CParticleEventSequence*>& listinstance = itor_seq->second;
			std::list<CParticleEventSequence*>::iterator itor_list = listinstance.begin();

			for (; itor_list != listinstance.end(); ++itor_list)
			{
				nMemorySize += (*itor_list)->GetMemorySize();
			}
		}
	}
#endif

	return nMemorySize;
}

void CKTDGParticleSystem::GetLoadedTexture(int& nTotal, int& nLoad)
{
	nTotal = 0;
	nLoad = 0;
#if !defined( __PATH__ )
	nTotal = (int)m_mapSequencePool.size();

	boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
	for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
	{
		if (itor->second->GetTexture())
		{
			nLoad++;
		}
	}
#endif
}

void CKTDGParticleSystem::LogUseParticleList()
{
#if !defined( __PATH__ )
	int nTotal = 0;
	int nLoad = 0;

	nTotal = (int)m_mapSequencePool.size();

	FILE* pFile = NULL;
	pFile = fopen("UseParticleList.txt", "wb");
	if (pFile)
	{
		boost::unordered_map<std::string, CParticleEventSeq*>::iterator itor;
		for (itor = m_mapSequencePool.begin(); itor != m_mapSequencePool.end(); ++itor)
		{
			bool bTexLoad = false;
			bool bPartLoad = false;

			if (itor->second->GetTexture())
			{
				bTexLoad = true;
				bPartLoad = itor->second->GetTexture()->IsLoaded();
			}

			fprintf(pFile, "%s\t%d\t%s\t%d\n", itor->first.c_str(), (int)bPartLoad, itor->second->GetTexName().c_str(), (int)bTexLoad);
		}

		fprintf(pFile, "=======================================\n\n");
		fprintf(pFile, "Paritcle Texture : %d/%d\n", nLoad, nTotal);
		fclose(pFile);
	}
#endif
}

CKTDGParticleSystem::CParticleEventSeq::CParticleEventSeq()
	:m_Texture(NULL),
	m_iMaxParticleNum(0),
	m_iSrcBlendMode(D3DBLEND_SRCALPHA),
	m_iDestBlendMode(D3DBLEND_DESTALPHA),
	m_iLayer(5)
{
	m_Lifetime.m_Min = 0.0f;
	m_Lifetime.m_Max = 0.0f;
	m_EmitRate.m_Min = 0.0f;
	m_EmitRate.m_Max = 0.0f;

	SETVECTOR3(m_vEmitRadius.m_Min, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vEmitRadius.m_Max, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vGravity.m_Min, 0.0f, 0.0f, 0.0f);
	SETVECTOR3(m_vGravity.m_Max, 0.0f, 0.0f, 0.0f);
}

CKTDGParticleSystem::CParticleEventSeq::~CParticleEventSeq()
{
	m_Texture = NULL;
}