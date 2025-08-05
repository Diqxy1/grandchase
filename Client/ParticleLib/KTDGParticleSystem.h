#ifndef _KTDGPARTICLESYSTTEM_H_
#define _KTDGPARTICLESYSTTEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <string>
//#include <vector>
//#include <map>
#include "KJohnStdVector.h"
#include "../GCDeviceLib/MinMax.h"
#include "../GCDeviceLib/GCMatrix.h"
#include "../GCUTIL/KJohnGen.h"
#include "../GCUtil/GCUtil.h"
inline std::string RemoveQuotes(std::string &str)
{
	for (std::string::iterator i = str.begin(); i != str.end(); i++)
	{
		if (*i == '\"')
		{
			i = str.erase(i);
			if (i == str.end())
				break;
		}
	}
	return(str);
}

#define D3DFVF_NEW_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define MAX_PARTICLE 100

class CKTDGMatrix;
class GCDeviceTexture;
class GCDeviceVertexBuffer;

struct TRACE_INFO
{
	D3DXVECTOR3* m_pvPos;
	float m_fParticleTraceTime;
	float m_fXOffset;
	float m_fYOffset;

	TRACE_INFO()
	{
		m_pvPos = NULL;
		m_fParticleTraceTime = 0.0f;
		m_fXOffset = 0.0f;
		m_fYOffset = 0.0f;
	}
};

struct DIRECTION_INFO
{
	bool m_bReverseTexture;		// 방향에 따라 텍스쳐를 뒤집어라
	bool* m_pbIsRight;
	float m_fDirPosX;
	bool m_bDirVelocity;
	bool m_bIsRight;

	DIRECTION_INFO()
	{
		m_bReverseTexture = true;
		m_pbIsRight = NULL;
		m_fDirPosX = 0.0f;
		m_bDirVelocity = false;
		m_bIsRight = true;
	}
};

class CKTDGParticleSystem
{
public:
	class CParticleEvent;

	struct VERTEX_PARTICLE
	{
		D3DXVECTOR3 position;
		D3DCOLOR    color;
		D3DXVECTOR2 tex;
	};

	enum TokenType
	{
		RealNumber = 1,
		Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote,

		KeywordParticleSystem, KeywordEventSequence,
		KeywordRandom, KeywordXYZ, KeywordXY, KeywordColor, KeywordTexture, AlphaBlendMode,

		// system property tokens
		SeqAlphaBlendModeProp,

		// event time tokens
		KeywordFade, KeywordSo, KeywordAt, KeywordFrom, KeywordTo, KeywordInitial, KeywordFinal,

		// particle properties (event types)
		ParticleNumericProp, ParticleVectorProp, ParticleVector2Prop, ParticleColorProp,

		// event sequence props
		SeqNumericProp, SeqVectorProp,

		UnknownToken,
	};

	enum EventType
	{
		EVENT_SIZE = 0,
		EVENT_COLOR,
		EVENT_ROTATE,
		EVENT_VELOCITY,
		EVENT_BLACKHOLE,
		EVENT_ROTATEX,
		EVENT_ROTATEY,
		EVENT_ROTATEZ,
		EVENT_VELOCITYX,
		EVENT_VELOCITYY,
		EVENT_VELOCITYZ,
		EVENT_TIMER,
		EVENT_TYPE_NUM,
	};
	class CParticleEmitterToken
	{
	public:
		CParticleEmitterToken() {}
		virtual ~CParticleEmitterToken() {}

		std::string m_strValue;
		TokenType	m_Type;

		bool IsEmitRate(void) { return(m_strValue.find("EMITRATE") != std::string::npos); }
		bool IsEmitRadius(void) { return(m_strValue.find("EMITRADIUS") != std::string::npos); }
		bool IsLifeTime(void) { return(m_strValue.find("LIFETIME") != std::string::npos); }
		bool IsNumParticles(void) { return(m_strValue.find("NUMPARTICLES") != std::string::npos); }
		bool IsGravity(void) { return(m_strValue.find("GRAVITY") != std::string::npos); }
		bool IsSrcBlendMode(void) { return(m_strValue.find("SOURCEBLENDMODE") != std::string::npos); }
		bool IsDestBlendMode(void) { return(m_strValue.find("DESTBLENDMODE") != std::string::npos); }
		bool IsKillTime(void) { return(m_strValue.find("KILLTIME") != std::string::npos); }
		bool IsKillNum(void) { return(m_strValue.find("KILLNUM") != std::string::npos); }
		bool IsLayer(void) { return(m_strValue.find("LAYER") != std::string::npos); }
	};

	class CParticle
	{
	public:
		CParticle()
		{
			m_fLifetime = 1.0f;
			m_fAge = 0.0f;
			m_fEventTimer = 0.0f;
			m_fEventTimerOld = 0.0f;
			m_fTraceTime = 0.5f;
			m_bReverse = false;

			SETCOLOR(m_Color, 1.0f, 1.0f, 1.0f, 1.0f);
			SETCOLOR(m_ColorStep, 0.0f, 0.0f, 0.0f, 0.0f);
			SETCOLOR(m_FinalColor, 1.0f, 1.0f, 1.0f, 1.0f);

			SETVECTOR2(m_vSize, 1.0f, 1.0f);
			SETVECTOR2(m_vSizeStep, 0.0f, 0.0f);
			SETVECTOR2(m_vFinalSize, 1.0f, 1.0f);

			SETVECTOR3(m_vPos, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vVelocity, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vVelocityStep, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vFinalVelocity, 0.0f, 0.0f, 0.0f);

			SETVECTOR3(m_vRotate, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vRotateStep, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vFinalRotate, 0.0f, 0.0f, 0.0f);

			m_vDirSpeed = 0.0f;
			m_vDirSpeedStep = 0.0f;
			m_vFinalDirSpeed = 0.0f;

			SETVECTOR3(m_BlackHoleSpeed, 0.0f, 0.0f, 0.0f);
			m_BlackHoleTime = 0.0f;
		}
		~CParticle() {}

		inline bool OnFrameMove(float fElapsedTime)
		{
			// age the particle
			m_fAge += fElapsedTime;
			m_fEventTimerOld = m_fEventTimer;
			m_fEventTimer += fElapsedTime;

			// if this particle's age is greater than it's lifetime, it dies.
			if (m_fAge >= m_fLifetime)
				return false; // dead!

			// move particle
			m_vPos += m_vVelocity		* fElapsedTime;

			m_vVelocity += m_vVelocityStep	* fElapsedTime;
			m_Color += m_ColorStep		* fElapsedTime;
			m_vSize += m_vSizeStep		* fElapsedTime;
			m_vRotate += m_vRotateStep	* fElapsedTime;

			D3DXMATRIX	mDir;
			D3DXVECTOR3 dirVec(1.0f, 0.0f, 0.0f);
			D3DXVECTOR4 dirVec4;
			D3DXMatrixIdentity(&mDir);
			D3DXMatrixRotationYawPitchRoll(&mDir, D3DXToRadian(m_vRotate.y), D3DXToRadian(m_vRotate.x), D3DXToRadian(m_vRotate.z));
			D3DXVec3Transform(&dirVec4, &dirVec, &mDir);
			dirVec.x = dirVec4.x;
			dirVec.y = dirVec4.y;
			dirVec.z = dirVec4.z;

			m_vPos += dirVec * m_vDirSpeed * fElapsedTime;
			m_vPos += m_BlackHoleSpeed		* fElapsedTime;

			SETCOLOR(m_ColorStep, 0.0f, 0.0f, 0.0f, 0.0f);;
			SETVECTOR2(m_vSizeStep, 0.0f, 0.0f);
			SETVECTOR3(m_vVelocityStep, 0.0f, 0.0f, 0.0f);
			SETVECTOR3(m_vRotateStep, 0.0f, 0.0f, 0.0f);
			m_vDirSpeedStep = 0.0f;

			m_fTraceTime -= fElapsedTime;

			return true; // particle stays alive
		}

		inline void Die() { m_fAge = m_fLifetime; }

		//Variable
		std::vector<CParticleEvent*>::iterator	m_CurEvent;

		float			m_fLifetime;
		float			m_fAge;
		float			m_fEventTimer;		// different from age
		float			m_fEventTimerOld;		// different from age

		D3DXCOLOR		m_Color;
		D3DXCOLOR		m_ColorStep;
		D3DXCOLOR		m_FinalColor;

		D3DXVECTOR2		m_vSize;
		D3DXVECTOR2		m_vSizeStep;
		D3DXVECTOR2		m_vFinalSize;

		D3DXVECTOR3		m_vPos;				// current position of particle
		D3DXVECTOR3		m_vVelocity;
		D3DXVECTOR3		m_vVelocityStep;
		D3DXVECTOR3		m_vFinalVelocity;

		D3DXVECTOR3		m_vRotate;
		D3DXVECTOR3		m_vRotateStep;
		D3DXVECTOR3		m_vFinalRotate;

		float			m_vDirSpeed;		//로테이션 방향으로 전진속도
		float			m_vDirSpeedStep;
		float			m_vFinalDirSpeed;

		D3DXVECTOR3		m_BlackHoleSpeed;
		float			m_BlackHoleTime;
		float			m_fTraceTime;

		bool			m_bReverse;
	};

	class CParticleEvent
	{
	public:
		CParticleEvent() {}
		virtual ~CParticleEvent() {}

		bool			IsFade() { return m_bFade; }
		void			SetFade(bool fade = true) { m_bFade = fade; }

		virtual void	OnFrameMove(CParticle &part, DIRECTION_INFO& stDir) = 0;
		virtual bool	FadeAllowed() = 0;

		virtual void	SetFinalValue(CParticle &part) = 0;

		CMinMax<float>&	GetActualTime() { return m_ActualTime; }
		void SetActualTime(CMinMax<float> actualTime) { m_ActualTime = actualTime; }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter) = 0;

		void toString(std::string& strOut);
		virtual std::string toStringValue() { return ""; };
		static void ProcessPropEqualsValue(CMinMax<float> &prop,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR2> &prop,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static void ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);
		float GetInterpolationFactor(CParticle &part)
		{
			float t = (part.m_fEventTimer - m_ActualTime.m_Min) / (m_ActualTime.m_Max - m_ActualTime.m_Min);
			if (t > 1.0f)
				t = 1.0f;
			return t;
		}
		EventType GetEventType() { return m_eType; }

	protected:
		bool			m_bFade;
		CMinMax<float>	m_ActualTime;
		EventType		m_eType;
	};

	class CParticleEvent_Color : public CParticleEvent
	{
	public:
		CParticleEvent_Color()
		{
			m_eType = EVENT_COLOR;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_FinalColor = m_Color.GetRandomNumInRange(); }

		CMinMax<D3DXCOLOR>	GetColor() { return m_Color; }
		void				SetColor(const CMinMax<D3DXCOLOR> &data) { m_Color = data; }
		std::string toStringValue() { return m_Color.toString(); }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<D3DXCOLOR> m_Color;
	};

	class CParticleEvent_Size : public CParticleEvent
	{
	public:
		CParticleEvent_Size()
		{
			m_eType = EVENT_SIZE;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalSize = m_Size.GetRandomNumInRange(); }

		CMinMax<D3DXVECTOR2>	GetSize() { return m_Size; }
		void					SetSize(const CMinMax<D3DXVECTOR2> data) { m_Size = data; }
		std::string toStringValue() { return m_Size.toString(); }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<D3DXVECTOR2> m_Size;
	};

	class CParticleEvent_Velocity : public CParticleEvent
	{
	public:
		CParticleEvent_Velocity()
		{
			m_eType = EVENT_VELOCITY;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalVelocity = m_Velocity.GetRandomNumInRange(); }

		CMinMax<D3DXVECTOR3>	GetVelocity() { return m_Velocity; }
		void					SetVelocity(const CMinMax<D3DXVECTOR3> &data) { m_Velocity = data; }
		std::string toStringValue() { return m_Velocity.toString(); }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<D3DXVECTOR3> m_Velocity;
	};

	class CParticleEvent_VelocityX : public CParticleEvent
	{
	public:
		CParticleEvent_VelocityX()
		{
			m_eType = EVENT_VELOCITYX;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalVelocity.x = m_VelocityX.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_VelocityX; }
		void			SetValue(const CMinMax<float> &data) { m_VelocityX = data; }
		std::string toStringValue() { return m_VelocityX.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_VelocityX;
	};

	class CParticleEvent_VelocityY : public CParticleEvent
	{
	public:
		CParticleEvent_VelocityY()
		{
			m_eType = EVENT_VELOCITYY;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalVelocity.y = m_VelocityY.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_VelocityY; }
		void			SetValue(const CMinMax<float> &data) { m_VelocityY = data; }
		std::string toStringValue() { return m_VelocityY.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_VelocityY;
	};

	class CParticleEvent_VelocityZ : public CParticleEvent
	{
	public:
		CParticleEvent_VelocityZ()
		{
			m_eType = EVENT_VELOCITYZ;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalVelocity.z = m_VelocityZ.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_VelocityZ; }
		void			SetValue(const CMinMax<float> &data) { m_VelocityZ = data; }
		std::string toStringValue() { return m_VelocityZ.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_VelocityZ;
	};

	class CParticleEvent_Rotate : public CParticleEvent
	{
	public:
		CParticleEvent_Rotate()
		{
			m_eType = EVENT_ROTATE;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalRotate = m_Rotate.GetRandomNumInRange(); }

		CMinMax<D3DXVECTOR3>	GetVelocity() { return m_Rotate; }
		void					SetVelocity(const CMinMax<D3DXVECTOR3> &data) { m_Rotate = data; }
		std::string toStringValue() { return m_Rotate.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<D3DXVECTOR3> m_Rotate;
	};

	class CParticleEvent_RotateX : public CParticleEvent
	{
	public:
		CParticleEvent_RotateX()
		{
			m_eType = EVENT_ROTATEX;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalRotate.x = m_RotateX.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_RotateX; }
		void			SetValue(const CMinMax<float> &data) { m_RotateX = data; }
		std::string toStringValue() { return m_RotateX.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_RotateX;
	};

	class CParticleEvent_RotateY : public CParticleEvent
	{
	public:
		CParticleEvent_RotateY()
		{
			m_eType = EVENT_ROTATEY;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalRotate.y = m_RotateY.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_RotateY; }
		void			SetValue(const CMinMax<float> &data) { m_RotateY = data; }
		std::string toStringValue() { return m_RotateY.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_RotateY;
	};

	class CParticleEvent_RotateZ : public CParticleEvent
	{
	public:
		CParticleEvent_RotateZ()
		{
			m_eType = EVENT_ROTATEZ;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalRotate.z = m_RotateZ.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_RotateZ; }
		void			SetValue(const CMinMax<float> &data) { m_RotateZ = data; }
		std::string toStringValue() { return m_RotateZ.toString(); }
		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_RotateZ;
	};

	class CParticleEvent_DirSpeed : public CParticleEvent
	{
	public:
		CParticleEvent_DirSpeed() {}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return true; }
		void SetFinalValue(CParticle &part) { part.m_vFinalDirSpeed = m_DirSpeed.GetRandomNumInRange(); }

		CMinMax<float>	GetValue() { return m_DirSpeed; }
		void			SetValue(const CMinMax<float> &data) { m_DirSpeed = data; }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

	protected:
		CMinMax<float> m_DirSpeed;
	};

	class CParticleEvent_BlackHole : public CParticleEvent
	{
	public:
		CParticleEvent_BlackHole()
		{
			m_eType = EVENT_BLACKHOLE;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return false; }
		void SetFinalValue(CParticle &part) {};

		CMinMax<float>& GetEventTimer() { return m_BlackHole; }
		void SetEventTimer(const CMinMax<float> &data) { m_BlackHole = data; }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);
		std::string toStringValue() { return m_BlackHole.toString(); }
	protected:
		CMinMax<float> m_BlackHole;
	};

	class CParticleEvent_EventTimer : public CParticleEvent
	{
	public:
		CParticleEvent_EventTimer()
		{
			m_eType = EVENT_TIMER;
		}

		void OnFrameMove(CParticle &part, DIRECTION_INFO& stDir);
		bool FadeAllowed() { return false; }
		void SetFinalValue(CParticle &part) {}

		CMinMax<float>& GetEventTimer() { return m_EventTimer; }
		void SetEventTimer(const CMinMax<float> &data) { m_EventTimer = data; }

		virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);
		std::string toStringValue() { return m_EventTimer.toString(); }
	protected:
		CMinMax<float> m_EventTimer;
	};

	class CParticleEventSequence
	{
	public:
		CParticleEventSequence();
		~CParticleEventSequence();

		void OnFrameMove(double fTime, float fElapsedTime);
		void OnFrameRender(GCDeviceVertexBuffer* pVB);

		int GetNumFreeParticles() { return m_iMaxParticleNum - (int)m_Particles.size(); }
		CParticle* CreateNewParticle(const D3DXVECTOR3& m_vPartSysPos, bool bReverse = false);
		int GetLiveParticleNum() { return (int)m_Particles.size(); }
		void SetAutoDie() { SetEmitRate(CMinMax<float>(0.0f, 0.0f)); m_bAutoDie = true; }
		bool IsAutoDie() { return m_bAutoDie; }

		const std::string& GetTexName() { return m_strTexFilename; }
		void SetTextureName(const std::string& strTextureName) { m_strTexFilename = strTextureName; }

		void SetName(const std::string& strName) { m_strName = strName; }
		const std::string& GetName() { return m_strName; }

		void SetPosition(float x, float y, float z) { SETVECTOR3(m_Position, x, y, z); }
		void SetPosition(const D3DXVECTOR3& pos) { m_Position = pos; }
		const D3DXVECTOR3& GetPosition() { return m_Position; }

		void SetBlackHolePosition(D3DXVECTOR3* pPos) { m_pBlackHolePosition = pPos; }

		void SetLifetime(CMinMax<float>& lifeTime) { m_Lifetime = lifeTime; }
		CMinMax<float>& GetLifeTime() { return m_Lifetime; }

		void SetEmitRate(CMinMax<float>& emitRate) { m_EmitRate = emitRate; }
		CMinMax<float>& GetEmitRate() { return m_EmitRate; }

		void SetEmitRadius(CMinMax<D3DXVECTOR3>& emitRadius) { m_vEmitRadius = emitRadius; }
		CMinMax<D3DXVECTOR3>& GetEmitRadius() { return m_vEmitRadius; }

		void SetGravity(CMinMax<D3DXVECTOR3>& gravity) { m_vGravity = gravity; }
		CMinMax<D3DXVECTOR3>& GetGravity() { return m_vGravity; }

		void SetMaxParticleNum(int maxParticleNum) { m_iMaxParticleNum = maxParticleNum; }
		int GetMaxParticleNum() { return m_iMaxParticleNum; }

		void SetTrace(TRACE_INFO* pTrace) { m_stTrace = *pTrace; }
		TRACE_INFO* GetTrace() { return &m_stTrace; }

		void SetKillTime(float fKillTime) { m_fKillTime = fKillTime; }
		float GetKillTime() { return m_fKillTime; }

		void SetKillNum(int iKillNum) { m_iKillNum = iKillNum; }
		int GetKillNum() { return m_iKillNum; }

		void SetLayer(int iLayer) { m_iLayer = iLayer; }
		int GetLayer() { return m_iLayer; }

		void SetDirectInfo(DIRECTION_INFO* pDir) { m_stDirect = *pDir; }
		void GetDirectInfo(DIRECTION_INFO& Dir) { Dir = m_stDirect; }

		void SetShow(bool* pbRender) { m_pbRender = pbRender; }
		void SetShow(bool bRender) { m_bRender = bRender; }

		void SetSrcBlendMode(int srcBlend) { m_iSrcBlendMode = srcBlend; }
		int GetSrcBlendMode() { return m_iSrcBlendMode; }

		void SetDestBlendMode(int destBlend) { m_iDestBlendMode = destBlend; }
		int GetDestBlendMode() { return m_iDestBlendMode; }

		KJohnStdVector<CParticle, 200> m_Particles;
		std::vector<CParticleEvent*> m_Events;

		CParticleEventSequence* GetCloneSequence();
		void SetTexture(GCDeviceTexture* pTexture) { m_Texture = pTexture; }
		GCDeviceTexture* GetTexture() { return m_Texture; }

		float GetCurrLifeTime() { return m_fCurrLife; }
		int GetTotalEmitNum() { return m_iTotalEmitNum; }

		void SetTracePosition(D3DXVECTOR3& vPos);

		bool IsSlowCount() const { return m_bSlowCount; }
		void SlowCount(bool bSlowCount) { m_bSlowCount = bSlowCount; }

		float GetAdditinalZRot() { return m_fAdditionalZRotate; }
		void  SetAdditinalZRot(float fZRot) { m_bUserAdditionalZRotate = true; m_fAdditionalZRotate = fZRot; }
		void  SetBeginPos(D3DXVECTOR3& vPos) { m_posBegin = vPos; }
		void  SetInternalBlackHolePos(D3DXVECTOR3& vPos)
		{
			m_bUseInternalBlackholePos = true;
			m_vposInternalBlackhole = vPos;
			m_pBlackHolePosition = &m_vposInternalBlackhole;
		}

		bool IsBlackHoleEventHere();

		bool					m_bRenderForImageTag;

	protected:
		void RunEvent(CParticle &part);

		bool					m_bAutoDie;
		std::string				m_strName;
		std::string				m_strTexFilename;
		GCDeviceTexture*		m_Texture;
		GCMatrix*				m_Matrix;

		D3DXVECTOR3				m_Position;
		D3DXVECTOR3				m_PosDiff;
		D3DXVECTOR3*			m_pBlackHolePosition;
		CMinMax<float>			m_Lifetime;
		CMinMax<float>			m_EmitRate;
		CMinMax<D3DXVECTOR3>	m_vEmitRadius;
		CMinMax<D3DXVECTOR3>	m_vGravity;
		int						m_iMaxParticleNum;
		TRACE_INFO				m_stTrace;
		DIRECTION_INFO			m_stDirect;
		bool*					m_pbRender;
		bool					m_bRender;

		int						m_iSrcBlendMode;
		int						m_iDestBlendMode;
		float					m_fNumNewPartsExcess;
		float					m_fKillTime;
		int						m_iKillNum;
		float					m_fCurrLife;
		int						m_iLayer;

		int						m_iTotalEmitNum;

		bool                    m_bSlowCount;

		bool                    m_bUserAdditionalZRotate;
		float                   m_fAdditionalZRotate;
		D3DXVECTOR3				m_posBegin;

		bool                    m_bUseInternalBlackholePos;
		D3DXVECTOR3				m_vposInternalBlackhole;
	};

	struct BLENDINGMODE
	{
		char	name[25];
		int		mode;
	};

	class CParticleEmitterTokenizer
	{
	public:
		void Tokenize(const char *str);

		static bool ProcessAlphaBlendMode(int &alphablendmode,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static bool ProcessColor(CMinMax<D3DXCOLOR> &c,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static bool ProcessTime(CMinMax<float> &TimeRange, bool &IsFade,
			const float InitialTime, const float FinalTime,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static bool ProcessVector(CMinMax<D3DXVECTOR3> &v,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static bool ProcessVector2(CMinMax<D3DXVECTOR2> &v,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static bool ProcessNumber(CMinMax<float> &number,
			std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter);

		static void NextToken(std::vector<CParticleEmitterToken>::iterator &TokenIter,
			std::vector<CParticleEmitterToken>::iterator &EndIter)
		{
			if (TokenIter == EndIter) throw("Unexpected end-of-file.");
			TokenIter++;
		}

		static const int NUMBLENDINGMODES;
		static const BLENDINGMODE m_BlendingModes[11];

		std::vector<CParticleEmitterToken> m_TokenVector;

	private:
		void AddToken(CParticleEmitterToken token);
		void DetermineTokenType(CParticleEmitterToken &token);
	};

	LPDIRECT3DDEVICE9 m_pd3dDevice;

	// 에러 출력용
	std::wstring m_strLastError;
	std::string m_strName;

	// 파티클 버퍼
	GCDeviceVertexBuffer* m_pVB;
	int m_iVBSize;

	// 모든 이펙트를 풀로 관리
	std::map<std::string, CParticleEventSequence*> m_mapSequencePool;

	// 현재 실행중인 이펙트 (레이어별)
	std::map< int, std::vector<CParticleEventSequence*> > m_mapInstance;

	// 파티클 로딩관련
	void OpenScriptFile(char* filename);
	bool Compile(const char *script);
	bool ProcessParticleSystemBlock(std::vector<CParticleEmitterToken>::iterator &TokenIter,
		std::vector<CParticleEmitterToken>::iterator &EndIter);

	bool ProcessEventSequenceBlock(CParticleEventSequence &seq,
		std::vector<CParticleEmitterToken>::iterator &TokenIter,
		std::vector<CParticleEmitterToken>::iterator &EndIter);

	static CKTDGParticleSystem::CParticleEvent* EventFactory(const std::string& EventName);

	// 파티클 관리
	CKTDGParticleSystem::CParticleEventSequence* CreateSequence(const std::string& name, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void DestroyAllInstance();

	//void DestroyInstance( PARTICLE_UID _dwUID );
	void DestroyInstance(CParticleEventSequence* sequence);

	bool IsLiveInstance(CParticleEventSequence* inst);
	void CheckLifeTime(CParticleEventSequence* pSequence);
	void Clear();
	void FrameMove(float fTime, float fElapsedTime);
	void Render(int iLayer);
	void InitDevice();
	bool SetLayer(CKTDGParticleSystem::CParticleEventSequence* pParticle, int iLayer);
};
extern char* strCParticleEventName[CKTDGParticleSystem::EVENT_TYPE_NUM];

#endif // _KTDGPARTICLESYSTEM_H_
