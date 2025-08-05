#ifndef _LTPACKET_H_
#define _LTPACKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TypeDef.h"
#include "Packet.h"

#pragma pack( push, 1 )


enum LT_PACKET_LIST
{
	ELTP_BASIC = 0,
    ELTP_QUEST_PROGRESS,
    ELTP_QUEST_STAGE_FIREBALL,
    ELTP_QUEST_STAGE_TRAP, 
    ELTP_AI_SPACE_JUMP,
    ELTP_AI_MOVE,
    ELTP_AI_MOVE_PACK,
	ELTP_PET_AI_MOVE,
    ELTP_PET_AI_MOVE_PACK,
	ELTP_KAIDO_FIREBALL,
	ELTP_FASTPOTAL_MOVE,
	ELTP_MONSTER_GENERATE,
	ELTP_GAIKOZ_SLASH,
	ELTP_KAMIKI_LIGHTNING,
	ELTP_KAMIKI_FIRE,
	ELTP_KAMIKI_ICE,
	ELTP_ELLENA_MOVE,
	ELTP_MAGICEFF,
	ELTP_QUEST_TIME,
	ELTP_GAME_ARENA,
	ELTP_ENT_FRUIT,
	ELTP_MAGIC_DAMAGE_SYNCH,	//마법 공격 받았을때 SYNCH를 위한 패킷
	ELTP_INSECTIVOROUS_BREEDING,
	ELTP_INSECTIVOROUS_SPIT,
	ELTP_GOLEM_TOKEN,
    ELTP_EMOTICON,
    ELTP_DAMAGE,
    ELTP_JUNGSU_REQ,
    ELTP_JUNGSU_ACK,
    ELTP_TITLE_STRING,
    ELTP_BUFF,
    ELTP_BUILD_PORTAL,
    ELTP_CHANGE_HP_INFO,
    ELTP_CHANGE_OBJECT_HP_INFO,
    ELTP_AGIT_OBJECT_POS,
    ELTP_AGIT_DICE_OBJECT_STATE,
    ELTP_AGIT_FLAG_OBJECT_STATE,
    ELTP_CHANGE_PLAYER_HP,

    ELTP_MATCH_QUICK_MODE,
    ELTP_PLAYER_QUEST_RANK,
	ELTP_END,
};

// packet.h로 옮김
//struct LTP_BASIC : public KGC_PACKET_HEADER
//{
//    UCHAR       	packetType;
//    USHORT			size;	
//    float			latencyTime;
//};
//

struct LTP_ARENA : public LTP_BASIC
{
	int m_iChar;
};

struct LTP_MAGIC_DAMAGE_SYNCH : public LTP_BASIC
{
	LTP_MAGIC_DAMAGE_SYNCH()
	{
		packetType = ELTP_MAGIC_DAMAGE_SYNCH;
		size = sizeof(LTP_MAGIC_DAMAGE_SYNCH);
		fTime = 0.0f;
        dwAttakeUnit = 0;
	}
	//공격자
	bool bIsAttakeUnitPlayer;
	DWORD dwAttakeUnit;

	//맞은 사람
	bool bIsDamagedUnitPlayer;
	int iDamagedUnit;
	int iMagicEffect;
    int iMagicLevel;
	//시간
	float fTime;
};

struct LTP_QUEST_PROGRESS : public LTP_BASIC
{
    int iCode;        
    int iExtra;
    int iDiceNo;
};

struct LTP_QUEST_STAGE_FIREBALL : public LTP_BASIC
{
    int iFireBallNum;
};

struct LTP_QUEST_STAGE_TRAP : public LTP_BASIC
{
    UCHAR           ucTrapType;
    USHORT          usPosX;
    USHORT          usPosY;
};

struct LTP_AI_SPACE_JUMP : public LTP_BASIC
{
    int iAI_ID;
    int iTarget;
};

struct LTP_AI_MOVE : public LTP_BASIC
{
    int             iMonsterIndex;
	UCHAR			ucMonsterID;
    UCHAR           ucMotion;
    UCHAR           ucFrame;

    int				iNowTarget;
    UCHAR			ucKamikiMagic;

    float           fPosX;
    float           fPosY;
    float           fYSpeed;
    float           fHP;

    bool            bXArrived;
    bool            bYArrived;   
    bool            bRun;
    bool            bRight;
    bool            bDie;
    bool            bAIEnable;

	int				iResurrectIndex;
};

struct LTP_AI_MOVE_PACK : public LTP_BASIC
{
	UCHAR ucActiveNum;
	std::map<int, LTP_AI_MOVE> movePacket;
};

struct LTP_PET_AI_MOVE : public LTP_BASIC
{
    SHORT		sAI_ID;

	bool		bEnable;
	bool		bDie;
	D3DXVECTOR2	vCenterPos;
	float		fTotalTime;
	bool		fTracking;	
	
	D3DXVECTOR2	pos;
	bool		bMagicStartThisFrame;
	int			motion;
	
	bool		bXArrived;
	bool		bYArrived;            
	bool		bRight;
	float		fMagicDelay;
};

struct LTP_PET_AI_MOVE_PACK : public LTP_BASIC
{
	UCHAR ucActiveNum;
    KSafeArray<LTP_PET_AI_MOVE,MAX_PET_NUM> movePacket;
};

struct LTP_KAIDO_FIREBALL : public LTP_BASIC
{
    float x;
	float y;

	bool bRight;
};

struct LTP_FASTPOTAL_MOVE : public LTP_BASIC
{
	D3DXVECTOR3 vecInpotalPos;
	D3DXVECTOR3 vecOutPotalPos;
};

//다양한 마법 이펙트에 관한 싱크를 위해서 사용합니다.
//잘 써봅시다..-_-;
//변수가 많이 부족할듯 합니다. 차후에 계속 추가합시다.
struct LTP_MAGICEFF_SYNCH : public LTP_BASIC
{
	//ELTP_MAGICEFF
	int iMagicEffType;
	D3DXVECTOR3 vecPos;
};

struct LTP_MONSTER_GENERATE : public LTP_BASIC
{
	int		iMonsterIndex;				//MonsterArray에서의 위치
	int		iMonsterType;				//바뀔 몬스터
	int		iMonsterLv;				    //몬스터의 레벨 설정
	float	fx;
	float	fy;
    bool    bRight;
    bool    bRandom;                    // 랜덤 포지션에 나올껀가?

    LTP_MONSTER_GENERATE()
    {
    };
};

struct LTP_GAIKOZ_SLASH : public LTP_BASIC
{
    float x;
	float y;
	
	bool bRight;
};

struct LTP_KAMIKI_LIGHTNING : public LTP_BASIC
{
	int monID;
	int lightningNum;
    float x;
	float y;
};

struct LTP_KAMIKI_FIRE : public LTP_BASIC
{
	int monID;
	int fireNum;
    float x;
	float y;
};

struct LTP_KAMIKI_ICE : public LTP_BASIC
{
	int monID;
	int iceNum;
    float x;
	float y;
};

struct LTP_ELLENA_MOVE : public LTP_BASIC
{
	D3DXVECTOR3 inPos;
	D3DXVECTOR3 outPos;
};

struct LTP_QUEST_TIME : public LTP_BASIC
{
	float fTime;
    bool bEnd;
};

struct LTP_ENT_FRUIT : public LTP_BASIC
{
	float x;
	float y;
	bool bRight;
	int Angle;
	int AngleSpeed;
	int iMon;
};

struct LTP_INSECTIVOROUS_BREEDING : public LTP_BASIC
{
	float x;
	float y;
	bool bRight;
	int iAngle;
	int iMon;
};

struct LTP_INSECTIVOROUS_SPIT : public LTP_BASIC
{
	float x;
	float y;
	bool bRight;
	int iAngle;
	float fSpeed;
	int iMon;
};

struct LTP_GOLEM_TOKEN : public LTP_BASIC
{
	float x;
	float y;
	bool bRight;
	int iAngle;
	int iMon;
};

struct LTP_EMOTICON : public LTP_BASIC
{
    LTP_EMOTICON()
    {
        packetType = ELTP_EMOTICON;
        size = sizeof(LTP_EMOTICON);
        
    }
    int iCharIndex;
    int iItemID;
};

struct LTP_DAMAGE : public LTP_BASIC
{
    LTP_DAMAGE()
    {
        packetType = ELTP_DAMAGE;
        size = sizeof(LTP_DAMAGE);
        fx = fy = 0.0f;
        latencyTime  = GC_ELAPSED_TIME;

    }
    int iCharIndex;
    int iDamage;
    float fx,fy;
    bool bMonsterDamage;
    bool bAbsoluePos;
};

struct LTP_JUNGSU_REQ : public LTP_BASIC
{
    LTP_JUNGSU_REQ()
    {
        packetType = ELTP_JUNGSU_REQ;
        size = sizeof(LTP_JUNGSU_REQ);
    }
    int iCharIndex;
    int iJungSuType;
    int iSlotIndex;
};


struct LTP_JUNGSU_ACK : public LTP_BASIC
{
    LTP_JUNGSU_ACK()
    {
        packetType = ELTP_JUNGSU_ACK;
        size = sizeof(LTP_JUNGSU_ACK);
    }
    int iCharIndex;
    int iJungSuType;
    int iSlotIndex;
};

struct LTP_TITLE_STRING : public LTP_BASIC
{
    LTP_TITLE_STRING()
    {
        packetType = ELTP_TITLE_STRING;
        size = sizeof(LTP_TITLE_STRING);
        latencyTime  = GC_ELAPSED_TIME;
    }
    int iStringID;
    int iCharIndex;
    bool bSameTeamOnly;
};

struct LTP_BUFF : public LTP_BASIC
{
    LTP_BUFF()
    {
        packetType = ELTP_BUFF;
        size = sizeof(LTP_BUFF);
		iBuffLv = 1;
        bClearBuff = false;
        dwAttackPlayerUnit = 0;
    }
    int iBuffIndex;
    int iTargetIndex;
    float fBuffTime;
	int iBuffLv;
    bool bClearBuff;
    int iWho;
    DWORD dwAttackPlayerUnit;
};

struct LTP_BUILD_PORTAL : public LTP_BASIC
{
    LTP_BUILD_PORTAL()
    {
        packetType = ELTP_BUILD_PORTAL;
        size = sizeof(LTP_BUILD_PORTAL);
        latencyTime = GC_ELAPSED_TIME;
    }
    D3DXVECTOR2	vPos;
    int iObjectIndex;
    UCHAR cPairID;
    int iDamage;
};

struct LTP_CHANGE_HP_INFO : public LTP_BASIC
{
	LTP_CHANGE_HP_INFO()
	{
		packetType = ELTP_CHANGE_HP_INFO;
		size = sizeof(LTP_CHANGE_HP_INFO);
		latencyTime = GC_ELAPSED_TIME;
		Init();
		
	}
	int iAttackerIndex;
	int iTargetIndex;
	float fDamage;
    float fHitPosX;
    float fHitPosY;
	DWORD dwAttackType;
    float fMonsterMaxHP;

	void Init()
	{
		iTargetIndex    = 1;
		iAttackerIndex  = 1;
		fDamage         = 0.0f;
        fHitPosX        = 0.0f;
        fHitPosY        = 0.0f;
		dwAttackType    = 0;
        fMonsterMaxHP = 0.f;
	}
};

struct LTP_CHANGE_OBJECT_HP_INFO : public LTP_BASIC
{
    LTP_CHANGE_OBJECT_HP_INFO()
    {
        packetType = ELTP_CHANGE_OBJECT_HP_INFO;
        size = sizeof(LTP_CHANGE_OBJECT_HP_INFO);
        latencyTime = GC_ELAPSED_TIME;
        Init();

    }
    int iSlotIndex;

    void Init()
    {
        iSlotIndex     = 0;
    }
};

struct LTP_AGIT_OBJECT_POS : public LTP_BASIC
{
    LTP_AGIT_OBJECT_POS()
    {
        packetType = ELTP_AGIT_OBJECT_POS;
        size = sizeof(LTP_AGIT_OBJECT_POS);
        latencyTime = GC_ELAPSED_TIME;
        Init();
    }

    void Init()
    {
        m_dwSenderUID = 0;
        m_itemUID = 0;
        m_iPosX = -1;
        m_iPosY = -1;
        m_cAlpha = 255;
        m_R = 255;
        m_G = 255;
        m_B = 255;
        m_bPhysics = true;
    }

    DWORD       m_dwSenderUID;
    GCITEMUID   m_itemUID;
    int         m_iPosX;
    int         m_iPosY;
    BYTE        m_cAlpha;
    BYTE        m_R, m_G, m_B;
    bool        m_bPhysics;
};

struct LTP_AGIT_DICE_OBJECT_STATE : public LTP_BASIC
{
    LTP_AGIT_DICE_OBJECT_STATE()
    {
        packetType = ELTP_AGIT_DICE_OBJECT_STATE;
        size = sizeof(LTP_AGIT_DICE_OBJECT_STATE);
        latencyTime = GC_ELAPSED_TIME;
        Init();
    } 


    void Init()
    {
        m_dwItemUID = UINT_MAX;
        m_dwUserUID = UINT_MAX;
        m_nDiceNum1 = -1;
        m_nDiceNum2 = -1;
    }

    GCITEMUID m_dwItemUID;
    DWORD m_dwUserUID;
    int   m_nDiceNum1;
    int   m_nDiceNum2;
};

struct LTP_AGIT_FLAG_OBJECT_STATE : public LTP_BASIC
{
    LTP_AGIT_FLAG_OBJECT_STATE()
    {
        packetType = ELTP_AGIT_FLAG_OBJECT_STATE;
        size = sizeof(LTP_AGIT_FLAG_OBJECT_STATE);
        latencyTime = GC_ELAPSED_TIME;
        Init();
    } 


    void Init()
    {
        m_dwItemUID = UINT_MAX;
        m_dwUserUID = UINT_MAX;
        m_bUpdate = false;
    }

    GCITEMUID m_dwItemUID;
    DWORD m_dwUserUID;
    bool m_bUpdate;
};

struct LTP_CHANGE_PLAYER_HP : public LTP_BASIC
{
    LTP_CHANGE_PLAYER_HP()
    {
        packetType = ELTP_CHANGE_PLAYER_HP;
        size = sizeof(LTP_CHANGE_PLAYER_HP);
        latencyTime = GC_ELAPSED_TIME;
        Init();
    }

    void Init()
    {
        m_dwUserUID = UINT_MAX;
        m_fChangeHP = 0.0f;
        m_iSkillID = -1;
    }

    DWORD m_dwUserUID;
    float m_fChangeHP;
    int m_iSkillID;
};

struct LTP_MATCH_QUICK_MODE : public LTP_BASIC
{
    LTP_MATCH_QUICK_MODE()
    {
        packetType = ELTP_MATCH_QUICK_MODE;
        size = sizeof(LTP_MATCH_QUICK_MODE);
        cQuickType = FAST_MODE;
        nLife = 0;
    }

    enum 
    {
        FAST_MODE = 0,
        FATAL_MODE = 1,        
    };
    
    char cQuickType;
    int  nLife;

};

struct LTP_PLAYER_QUEST_RANK : public LTP_BASIC
{
    LTP_PLAYER_QUEST_RANK()
    {
        packetType = ELTP_PLAYER_QUEST_RANK;
        size = sizeof(LTP_PLAYER_QUEST_RANK);

        iPlayerIndex = 0;
        iRankGrade = 0;
    }

    int iPlayerIndex;
    int iRankGrade;
};

#pragma pack( pop )

#endif // _LTPACKET_H_
