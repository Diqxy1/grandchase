#pragma once
#include "damage.h"

class CHardDamagePeriotOrb:
	public CDamage
{
public:
	CHardDamagePeriotOrb(void);
	virtual ~CHardDamagePeriotOrb(void);
	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );

private:
	D3DXVECTOR3 m_vPos;
	bool m_bIsArrived;
	float fProcessFrame;
	float fIntervalX;
	float fIntervalY;
    int m_iMaster;
    int m_iGoalBone;
    float m_fOffset_X;
    float m_fRatioHpUP;
};

class MONSTER;
class CHardDamagePeriotEat:
	public CDamage
{
public:
	CHardDamagePeriotEat(void);
	virtual ~CHardDamagePeriotEat(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};


class CHardDamagePeriotDestroyOrb:
	public CDamage
{
public:
	CHardDamagePeriotDestroyOrb(void);
	virtual ~CHardDamagePeriotDestroyOrb(void);
	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );

private:
	int iMaxnum;
	int iPlayerNum;
	std::vector<int> vecLiveIndex;
	MONSTER* pMonster;
};

class CHardDamageHatshepsut:
    public CDamage
{
public:
    CHardDamageHatshepsut(void);
    virtual ~CHardDamageHatshepsut(void);
    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );

private:
    int m_iMonsterType;
    int m_iDamageID;
    char* m_strState;
};

class CHardDamageCatMullRomSpline:
    public CDamage
{
public:
    CHardDamageCatMullRomSpline(void);
    virtual ~CHardDamageCatMullRomSpline(void);
    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void ValkyrieFrameMove( CDamageInstance* pDamageInstance );

private:
    //////////////////////////////////////////////////////////////////////////
    // 이건 꼭 필요한것들 ㅋㅋ 데미지로 나중에 옮기쟈~ 
    int         m_iIndex;
    int         m_iLifeMax;
    float       m_fS;
//////////////////////////////////////////////////////////////////////////
    // 이건 Exp 연출 위해서 만든것 ㅋ 
    
};
