#pragma once
#include "igcsquareobj.h"
#include "DamageDefine.h"


class KGC3DObject;

struct TRACE_PARTICLE_INFO
{
	TRACE_PARTICLE_INFO()
	{
		m_strParticleName.clear();
		m_iBoneIndex = -1;
		m_fXOffset = 0.0f;
		m_fYOffset = 0.0f;
		m_pParticleSequence = NULL;
		m_vecTracePos.x = 0.0f;
		m_vecTracePos.y = 0.0f;
		m_vecTracePos.z = 0.5f;
	}

	std::string m_strParticleName;
	int m_iBoneIndex;
	float m_fXOffset;
	float m_fYOffset;
	D3DXVECTOR3 m_vecTracePos;
	CParticleEventSeqPTR m_pParticleSequence;
};

class KGC3DObject;
class KGCSquare3DObj :	public IGCSquareObj
{
public:
	KGCSquare3DObj(void);
	~KGCSquare3DObj(void);

private:
    KGC3DObject*            m_pObject;
    int                     m_iCurMotion;
    int                     m_iFrameCount;
    std::wstring            m_strNick;
	bool					m_bNextMotion;

public:
    void CreateObject( int iTech_ );
    void SetPosition( const D3DXVECTOR3& vPos );
	D3DXVECTOR3 GetObjectLocalPos(void);
    void Get_BasicMatrix( D3DXMATRIX* pMat );
    void DrawDumpData( void );
    void DrawBodyRect( void );
    void SetEdgeColor( D3DXVECTOR4 vColor );
    D3DXVECTOR3 GetObjectPos( void ) { return m_BasicData.vPos; }
    void ReloadNPCSpeech( void );
    void SetNPCNickName( std::wstring strNick ) { m_strNick = strNick; }
    void DrawNickName( void );
    virtual void Render();
    virtual void FrameMove();
    virtual bool Load();
    virtual bool TimeShareLoading();
    virtual bool Picking( const D3DXVECTOR2& vMousePos , const int iMouseBtn );
	void SetSpeechTimer(float fSpeechTimer) { m_fSpeechTimter = fSpeechTimer; }
	void SetCharType(EGCCharType egcCharType) { m_egcCharType = egcCharType; }
	void SetIsUseNickNameString(bool bUseNickNameString) { m_bUseNickNameString = bUseNickNameString; }
	void SetNickNameStringID( DWORD dwNickNameStringID) { m_dwNickNameStringID = dwNickNameStringID; }
    VOID SetCartoonTexture( EGCCartoonTexture eCartoonTexture_) { m_eCartoonTexture = eCartoonTexture_; }
	void FrameMoveTraceParticle();
    inline DWORD GetNickNameStringID(){ return m_dwNickNameStringID;};
   
    void CreateMeshChildParticle( void );
    CParticleEventSeqPTR CreateSequence( const std::string& name, float x, float y, float z );

    void EndAfterImage();

    MESH_INFO                   m_sMeshInfo;
    std::vector<int>            m_vecStringID;
    std::vector<std::wstring>   m_vecSpeechStr;
	float						m_fSpeechTimter;
	EGCCharType					m_egcCharType;
	bool						m_bUseNickNameString;
	DWORD						m_dwNickNameStringID;
	float						m_fSpeechOffsetX;
	float						m_fSpeechOffsetY;
	int							m_iSpeechDistance;
    EGCCartoonTexture           m_eCartoonTexture;
	float						m_fNicknameOffsetX;
	float						m_fNicknameOffsetY;
	DWORD						m_dwLParam;
	DWORD						m_dwRParam;

	std::vector<TRACE_PARTICLE_INFO*> m_vecTraceParticleInfo;
};
