#ifndef _GCO2MANAGER_H_
#define	_GCO2MANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCGameObject.h"
////#include <vector>

#define	MAX_PLAYER_O2		(35.0f)

class KGCO2Region : public KGCGameObject
{
public:
	KGCO2Region();
	~KGCO2Region();
	void Init(float PosX, float PosY, float Radius);
	bool IsIn(int iPlayerIdx);

    void Create( void );
    void Destroy( void );
    void Enable( void );
    void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    void Render( void );
private:
	float m_fRadius;
    SLVERTEX                        V_Water[4];               ///< 맵 전경을 그릴 버텍스 버퍼

    GCDeviceTexture*                                m_Tex_O2Zone;
	CParticleEventSeqPTR	    m_pO2Zone;
};

class KGCO2Manager
{
public:
	KGCO2Manager();
	virtual ~KGCO2Manager();

	void Create();
	void Destroy();
	void FrameMove(float fElapsedTime);
	bool IsInWater(int iPlayerIdx);
	bool IsInWaterMonster(int iMonsterIdx);
	void SetWaterHeight( float fHeight );

	bool IsInSquareWater(float fy_);
private:	
	bool IsInO2Region(int iPlayerIdx);
	void UpdatePlayerPosition();
	void WaterEff();

	float m_fWaterHeight;

	//Player가 물속에 있었는지...현재 프레임과 이전 프레임 정보를 기억..
	KSafeArray<bool,MAX_PLAYER_NUM> m_bInWaterNow;
	KSafeArray<bool,MAX_PLAYER_NUM> m_bInWaterOld;
	
	std::map<int, bool> m_mapInWaterMonsterNow;
	std::map<int, bool> m_mapInWaterMonsterOld;


	//std::vector<KGCO2Region*>	m_vecO2Region;
	CParticleEventSeqPTR		    m_pWaterImpact;
	
	bool m_bEqipO2Mask;
};


#endif // _GCO2MANAGER_H_
