/// @file   Spark.h
/// @author custard@kogstudios.com
///         pch413@kogstudios.com
/// @since  2004-01-08 오후 4:05:22

#ifndef _SPARK_H_
#define _SPARK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"

#define MAX_SPARK_INSTANCE	(512)
#define MAX_SPARK_VERTEX	(512*4)



enum SPARK
{
	HIT_LIGHT          = 0,   ///< 타격 지점 효과
	PARTICLE           = 1,   ///< 파티클
	DUST               = 2,   ///< 먼지
	FIRE               = 3,   ///< 불
	RED_PARTICLE       = 4,   ///< 하트 타격 효과
	LIGHTNING0         = 5,   ///< 라이트닝0
	LIGHTNING1         = 6,   ///< 라이트닝1
	LIGHTNING2         = 7,   ///< 라이트닝2
	LIGHTNING3         = 8,   ///< 라이트닝3
	LIGHTNING4         = 9,   ///< 라이트닝4
	LIGHTNING5         = 10,  ///< 라이트닝5
	LIGHTNING6         = 11,  ///< 라이트닝6
	LIGHTNING7         = 12,  ///< 라이트닝7
	SPARK0             = 13,  ///< 스파크0
	SPARK1             = 14,  ///< 스파크1
	SPARK2             = 15,  ///< 스파크2
	REVERSE_GRAVITY    = 16,  ///< 리버스 그래비티 테두리 효과
	FLASH              = 17,  ///< 섬광 효과
	ENERGY_BEAM0       = 18,  ///< 필살기 집중선0
	ENERGY_BEAM1       = 19,  ///< 필살기 집중선1
	ENERGY_BEAM2       = 20,  ///< 필살기 집중선2
	ENERGY_FOCUS0      = 21,  ///< 기 모으기0
	ENERGY_FOCUS1      = 22,  ///< 기 모으기1
	ENERGY_FOCUS2      = 23,  ///< 기 모으기2
	FOUNTAIN_HEAD0     = 24,  ///< 분수머리0
	FOUNTAIN_HEAD1     = 25,  ///< 분수머리1
	FOUNTAIN_HEAD2     = 26,  ///< 분수머리2
	WATER_PIECE        = 27,  ///< 물방울
	LANCE_MAGIC_CIRCLE = 28,  ///< 파이어드 랜스(창병 3단계 필살기) 마법진 효과
	LANCE_MOVE_CIRCLE  = 29,  ///< 파이어드 랜스(창병 3단계 필살기) 전진 효과
	LANCE_SPRAY        = 30,  ///< 파이어드 랜스(창병 3단계 필살기) 분사 효과
	ANGEL              = 31,  ///< 천사
	ANGEL_EGG          = 32,  ///< 천사의 알
	ANGEL_EGG_PROP     = 33,  ///< 천사의 알 받침대
	ANGEL_EGG_BANG     = 34,  ///< 천사의 알 꽝
	ARROW              = 35,  ///< 화살표 ( 석궁용 )
	INTO_THE_DARKNESS  = 36,  ///< 어둠속으로 ( 가도센 )
	ONE_KILL           = 37,  ///< 일섬 ( 가도센 )
	ONE_KILL_FLASH     = 38,  ///< 일섬의 섬광 ( 가도센 )
	FLOAT_SOUL         = 39,  ///< 떠다니는 혼
	SAND_DUST          = 40,  ///< 오크사원 흙먼지
	ICE_FIRE           = 41,  ///< 파란불
	DARK_FIRE          = 42,  ///< 까만불
	WATER_DUST         = 43,  ///< 물 속 먼지
	BLUE_LIGHTNING0    = 44,  ///< 푸른라이트닝0
	BLUE_LIGHTNING1    = 45,  ///< 푸른라이트닝1
	BLUE_LIGHTNING2    = 46,  ///< 푸른라이트닝2
	BLUE_LIGHTNING3    = 47,  ///< 푸른라이트닝3
	BLUE_LIGHTNING4    = 48,  ///< 푸른라이트닝4
	BLUE_LIGHTNING5    = 49,  ///< 푸른라이트닝5
	BLUE_LIGHTNING6    = 50,  ///< 푸른라이트닝6
	BLUE_LIGHTNING7    = 51,  ///< 푸른라이트닝7
	LEAF_DUST		   = 52,  ///< 나뭇잎 먼지
	MON_RISEN		   = 53,  ///< 몬스터 Risen효과
	DARK_SKULL		   = 54,  ///< 검은 해골
	BLOOD_OUT		   = 55,  ///< 피튀김
    UNDEAD_WALK        = 56,  ///<
	NEPHILIM_ATK1	   = 57,  ///< 네피림 타격 이펙트
	NEPHILIM_ATK2	   = 58,  ///< 네피림 타격 이펙트2
	NEPHILIM_ATK3	   = 59,  ///< 네피림 타격 이펙트3

	NUM_SPARK,
};

struct Effect
{
	BYTE What;        ///< 이펙트의 종류
    int Life;        ///< 이펙트의 수명(매 프레임 1씩 감소)
    int Delay;        ///< 딜레이(지연값)
	float x;          ///< x 위치
    float y;          ///< y 위치
    float Scale;      ///< 크기 조절량
    float Angle;      ///< 각도
    float Target_x;   ///< 목표로 하는 x 위치
    float Target_y;   ///< 목표로 하는 y 위치
    bool IsRight;     ///< 오른쪽 방향인가?
	bool m_IsBlendOne;///< BLEND모드

    bool bTrace;
    float fXTerm;
    float fYTerm;
    int who;
    bool bMonster;

    Effect() { bTrace = false; } 
    void SetTrace( float fXTerm_, float fYTerm_, int who_, bool bMonster_ )
    {
        bTrace = true;
        fXTerm = fXTerm_;
        fYTerm = fYTerm_;
        who = who_;
        bMonster = bMonster_;
    }
};

struct SPARK_BUFFER_OFFSET
{
	int iVertexOffset;
	int iIndexOffset;
};

class GCDeviceTexture;

class KEffector
{
// ------------------------------------------------------------------------------------------
// 생성자 및 소멸자
// ------------------------------------------------------------------------------------------
public:
    /// Default constructor
    KEffector( void );
    /// Default destructor
    ~KEffector( void );

// ------------------------------------------------------------------------------------------
// 디바이스 관련 함수
// ------------------------------------------------------------------------------------------
public:
    /// InitDeviceObjects - KEffector ( old name : Sparks )
    void Load();
	void UnLoad();
	/************************************************************************/
	/*	OnResetDevice -> InitDevice 로 변경
	/************************************************************************/
    //void OnResetDevice( void );
	void InitDevice( void );
 //   /// InvalidateDeviceObjects - KEffector ( old name : Sparks )
	//void InvalidateDevice( void );
 //   /// DeleteDeviceObjects - KEffector ( old name : Sparks )
    void DeleteDevice( void );

// ------------------------------------------------------------------------------------------
// 이펙트 추가 함수
// ------------------------------------------------------------------------------------------
public:
    /// 선택된 이펙트를 추가한다.
    Effect* Add( int What, float x, float y, int Delay, float Scale = 1.0f,
              float Target_x = -100.0f, float Target_y = -100.0f,
              float Angle = 0.0f, bool bIsRight = false, int who = -1 );
	//void Water_Piece( float x, float y, BYTE Delay );
    void Particles( float x, float y, BYTE Delay, BYTE Num, BYTE Scale, int who = -1 , int iEffectKind = PARTICLE );
	void Red_Particles( float x, float y, BYTE Delay, BYTE Num, BYTE Scale );
	void Bim( float x, float y, BYTE Delay, int iWhat_ = SPARK0 );
	void LightBim( float x, float y, BYTE Delay );
	void Light( float x, float y, BYTE Delay, float fScale = 1.0f );
	void BlueLight( float x, float y, BYTE Delay, float fScale = 1.0f );
	void Smogs( float x, float y, int Delay, bool Round=false);
	void WaterSmogs( float x, float y, BYTE Delay, bool Round=false);
	void LeafSmogs( float x, float y, BYTE Delay, bool Round=false);
    void UndeadSmogs( float x, float y, BYTE Delay, bool Round=false);
	void Fires( float x, float y, BYTE Delay, float fSize = 0.3f, int iNum = 20 );
	void IceFires( float x, float y, BYTE Delay, float fSize = 0.3f, int iNum = 20 );
    void LightFires( float x, float y, BYTE Delay, float fSize = 0.3f, int iNum = 20 );
	void DarkFires( float x, float y, BYTE Delay, float fSize = 0.3f, int iNum = 20 );
    /// 분수('잊혀진 도시' 효과) 머리 부분을 추가한다.
    void Fountail_Head( void );
    void AddAngelEggProp( float x, float y, bool bIsBlueTeam );
    void AddSandDust( float x, float y );

// ------------------------------------------------------------------------------------------
// 이펙트 제거 함수
// ------------------------------------------------------------------------------------------    
private:
    /// 선택된 이펙트를 제거한다.
	int SetBuffer(SLVERTEX* pVB,std::map< int, std::vector<Effect*> >::iterator itor);
	bool CheckEnableCreate();
	void RenderCrack(GCDeviceTexture*texture, Effect* );



public:
    /// 모든 이펙트를 제거한다.
    void RemoveAll( void );

// ------------------------------------------------------------------------------------------
// 매 프레임당 호출 함수
// ------------------------------------------------------------------------------------------
public:
    void FrameMove( void );

// ------------------------------------------------------------------------------------------
// 렌더링 함수
// ------------------------------------------------------------------------------------------
public:
    void Render( void );
	void LiveFrameMove( Effect* pEffect, OUT std::vector<Effect> &vecWaitCreateList );

// ------------------------------------------------------------------------------------------
// 멤버 변수
// ------------------------------------------------------------------------------------------
private:
	std::map< int, std::vector<Effect*> > m_mapSpark;

public:
	KSafeArray<GCDeviceTexture*,NUM_SPARK> m_pTexSpark;
	int m_iTotalSpark;

};

#endif // _SPARK_H_
