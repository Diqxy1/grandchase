#pragma once

#include "GCEnum.h"
#include "KGC2DObject.h"
#include "KGCGameModeInterface.h"
#include "Monster.h"
#include "GCGlobalFunction.h"

struct ParticleInfo
{
    std::string szSequence;
    D3DXVECTOR2 vOffset;
    bool        bIsRight;
    int         iTeam;
    float       fHpMaxRidio;
    float       fHpMinRidio;
};

class KGCEggObject : public KGC2DObject
{
public:
	KGCEggObject();
	virtual ~KGCEggObject(){ SAFE_RELEASE( m_pHPBarTex ); }

	virtual bool Damaged( GCCollisionRect<float>& weapon, float fAttackPoint = 0.0f, int iDamageType = -1, int iAttackerIndex = -1, bool bMonsterDamage = false )
	{
#if !defined( ANGELS_EGG_REFORM )
		bool bSameTeam = false;
		if( iAttackerIndex >= 0 )
		{
			if( bMonsterDamage == false )
			{
				ASSERT( !(iAttackerIndex < 0 || iAttackerIndex >= MAX_PLAYER_NUM) );
				bSameTeam = (g_MyD3D->MyPlayer[iAttackerIndex]->m_kUserInfo.iTeam == GetTeam() );
			}
			else
			{
				MONSTER* pMonster = g_kMonsterManager.GetMonster(iAttackerIndex);
				if ( pMonster == NULL )
					return false;

				bSameTeam = (pMonster->m_iTeam == GetTeam() );
			}
		}

		if( bSameTeam == true )
			return false;

		static DWORD dwStartTime = ::timeGetTime();
		// 이부분은 방장만 처리하고 알의 HP와 상태는 방장이 보내주는게 맞다.
		if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
		{
			SetHP( GetHP() - ( g_MyD3D->MyPlayer[iAttackerIndex]->m_fAbility[ABILITY_ATK] * fAttackPoint ) );

			std::stringstream strFunction;
			strFunction<<"state, msg = pcall( AngelsEgg_Damaged,"<< m_iTeam << ", "<< GetHP() / GetMaxHP() <<" ); "
				"if state == false then"
				"	print( msg ) "
				"end";
			GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), strFunction.str() );

			if( ::timeGetTime() - dwStartTime > 500 || GetCurrTexIndex() == 3 )
			{
				ParticleEffect( GetParent()->GetCurrTexIndex() );
				m_bNeedP2PUpdate = true;
				dwStartTime = ::timeGetTime();
			}
		}

		return KGC2DObject::Damaged( weapon, fAttackPoint, iDamageType, iAttackerIndex, bMonsterDamage );
#else
        return false;
#endif
	}

	void SetTeam( int _iTeam ) { m_iTeam = _iTeam; };
	int GetTeam() const { return m_iTeam; }

	void SetHP( float _fHP ) { m_fHP = _fHP; }
	void SetMaxHP( float _fHP ) { m_fHP_max = _fHP; }
	float GetHP() const { return m_fHP; }
	float GetMaxHP() const { return m_fHP_max; }
	void SetBreak( bool _bBreak ) { m_bBreak = _bBreak; }
	bool IsBreak() const { return m_bBreak; }
	void ParticleEffect( int iState );
	KGC2DObject* GetParent() { return (KGC2DObject*)this; }

	bool SendEggInfo();
	void LoadHPBar();
	void HPRender();
	
private:
	float	m_fHP;
	float	m_fHP_max;
	int		m_iTeam;
	bool	m_bNeedP2PUpdate;
	bool	m_bBreak;
	GCDeviceTexture* m_pHPBarTex;
	GCDeviceTexture* m_pHPBarBackTex;
	std::vector< std::pair< float, std::string > > m_vecStateTexture;
};

class KGCAngelsEgg : public KGCGameModeInterface
{
public:
	KGCAngelsEgg(void);
	~KGCAngelsEgg(void);

	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading();
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR();

	HRESULT Render_AngelsEgg();

	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();

	bool LoadScript();
    bool LoadReformScript();
	void SetEggState( int iTeam, int iState, float fHp, float fMaxHP, bool bBreak );
	void HpRender(void);

    void SummonAngelsEgg(int iTeam, int iMonsterType, D3DXVECTOR2 vPos, bool bIsRight, int iSlotNum);

private:
	KGCEggObject* GetEggObject( int iTeam );
	void SetEggHP();

private:

	KSafeArray<KGCEggObject*,TEAM_NUM> m_apkAngelsEggObject;

    bool m_bCheckSetEggHP;

private:
    int   m_iSelectTexture[TEAM_NUM];
    float  m_fBeforeHp[TEAM_NUM];
    bool m_bLoadingComplate[TEAM_NUM];
    D3DXVECTOR2 v2AngelsEggPos[TEAM_NUM];
    std::vector<ParticleInfo> sParticle;

    CParticleEventSeqPTR m_arrayParticle[TEAM_NUM];

    

public:
    void ChangeAngelsEggTexture(int iTeam);
    void SetAngelsEggBeforeHp(int iTeam, float fHp)
    {
        m_fBeforeHp[iTeam] = fHp;
    }
    void SetLoadingComplate(int iTeam, bool bLoad)
    {
        m_bLoadingComplate[iTeam] = bLoad;
    }
    void InitAngelsEggHP(int iTeam)
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(iTeam);
		if ( pMonster == NULL )
			return;

        m_fBeforeHp[iTeam] = pMonster->GetHP();
    }
    void InitAngelsEggTexture(int iTeam);

    int m_iMonSlotNum[TEAM_NUM];
};
