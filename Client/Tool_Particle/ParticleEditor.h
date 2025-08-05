#pragma once

#include "KTDGParticleSystem.h"
#include "TimeLineEditor.h"

// Struct Particle Editor Particle TimeLine
struct SPE_ParticleTimeline
{
	CString m_v2Size;
	CString m_v3Rotate;
	CString m_v3Velocity;
	CString m_v4Color;
	float	m_fTime;

	SPE_ParticleTimeline( const SPE_ParticleTimeline& src )
	{
		*this = src;
	}
	const SPE_ParticleTimeline& operator = ( const SPE_ParticleTimeline& src )
	{
		m_v2Size		= src.m_v2Size;
		m_v3Rotate		= src.m_v3Rotate;
		m_v3Velocity	= src.m_v3Velocity;
		m_v4Color		= src.m_v4Color;
		m_fTime			= src.m_fTime;
	}
};

// Struct Particle Editor 
struct SPE_Particle
{
	CString m_strName;
	CString m_strBlendModeSrc;
	CString m_strBlendModeDest;
	CString m_strTexture;
	
	int		m_iNumParticle;
	float	m_fLifeTime;
	int		m_iLayer;

	CString m_v3EmitRadius;
	CString	m_v2EmitTime;
	CString m_v2EmitRate;
	CString m_v3Gravity;

	std::vector<SPE_ParticleTimeline> m_vecTimeLine;

	SPE_Particle( const SPE_Particle& src )
	{
		*this = src;
	}
	const SPE_Particle& operator = ( const SPE_Particle& src )
	{
		m_strName			= src.m_strName;
		m_strBlendModeSrc	= src.m_strBlendModeSrc;
		m_strBlendModeDest	= src.m_strBlendModeDest;
		m_strTexture		= src.m_strTexture;

		m_iNumParticle		= src.m_iNumParticle;
		m_fLifeTime			= src.m_fLifeTime;
		m_iLayer			= src.m_iLayer;

		m_v3EmitRadius		= src.m_v3EmitRadius;
		m_v2EmitTime		= src.m_v2EmitTime;
		m_v2EmitRate		= src.m_v2EmitRate;
		m_v3Gravity			= src.m_v3Gravity;
	}
};

class CParticleEditor
{
public:
	CParticleEditor();
	~CParticleEditor(void);

public:
	bool LoadScript( const TCHAR* strFileName );
	bool SaveScript( const TCHAR* strFileName );
	void ParticleToWindow();
	void AddParticleSequence( CKTDGParticleSystem::CParticleEventSequence* pSequence );
	void RemoveAllParticle();	
	void SetWorkParticle( CKTDGParticleSystem::CParticleEventSequence* pSequence );

	bool CheckParticleProperty();
	void UpdateParticleProperty();
	void OnNewParticle();
	CKTDGParticleSystem::CParticleEventSequence* OnCreateParticle(CString strScript, bool bAddToSystem = true);

	void Render();
	void FrameMove(float fElapsedTime);
	void SetGravity();
	void SetEmitRadius();
	void SetEmitRate();
	void SetKillTime();
	void CreateSampleParticle();
	bool DeleteParticle(CKTDGParticleSystem::CParticleEventSequence* pSequence);
	
	CKTDGParticleSystem::CParticleEventSequence* FindSequence(CString strName);

	void OnScriptEdit();
	CString SequenceToScript(CKTDGParticleSystem::CParticleEventSequence* pSequence, CString strName = "");
	
public:	// static functions
	// MinMax안으로 넣고 싶지만... Tokenizer를 사용해서리...
	static CMinMax<float>& StrToMinMax( CMinMax<float>& MinMax, CString str );
	static CMinMax<D3DXVECTOR2>& StrToMinMax( CMinMax<D3DXVECTOR2>& MinMax, CString str );
	static CMinMax<D3DXVECTOR3>& StrToMinMax( CMinMax<D3DXVECTOR3>& MinMax, CString str );
	static CMinMax<D3DXCOLOR>& StrToMinMax( CMinMax<D3DXCOLOR>& MinMax, CString str );

public: //변수
	CKTDGParticleSystem*		m_pParticleSystem;
private:
	std::vector<SPE_Particle*>	m_vecParticle;
	std::string					m_strLastError;
	float						m_fTimeTotal;
	float						m_fTimeLast;

	CKTDGParticleSystem::CParticleEventSequence* m_pWorkParticle;
	
};

extern CParticleEditor* g_pParticleEditor;