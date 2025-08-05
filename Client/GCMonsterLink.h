//#ifndef _GCMONSTERLINK_H_
//#define _GCMONSTERLINK_H_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include "GCGameObject.h"
//#include "Packet.h"
//#include "Monster.h"
//
//class KGCMonsterLink : public KGCGameObject
//{
//public:
//	KGCMonsterLink(void);
//	virtual ~KGCMonsterLink(void);
//
//	void Lock( bool );
//	void SetMonster(int);
//
//	inline void SetRebirthTime(float fTime)
//	{
//		m_fRebirthTime = fTime;
//	}
//	inline void SetColor(std::string strColor)
//	{
//		m_strColor = strColor;
//	}
//	inline void SetDoor( int iDoor )
//	{
//		m_iDoor = iDoor;
//	}
//
//private:
//	UNUSED_COPY_CONSTRUCTOR( KGCMonsterLink );
//	UNUSED_ASSIGNMENT_OPERATOR( KGCMonsterLink );
//
//public:
//	virtual void Create( void );
//	virtual void Destroy( void );
//	virtual void Enable( void ){}
//	virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
//	virtual void Render( void );
//
//private:
//	CKTDGAnim::CAnimInstance* m_pNormal;
//	CKTDGAnim::CAnimInstance* m_pCrash;
//	CParticleEventSeqPTR m_pLockParticle;
//	CParticleEventSeqPTR m_pRebirth;
//
//	std::string m_strColor;
//	int m_iMonster;			//연결될 몬스터 인덱스
//	int m_iDoor;			//연결될 성문
//	float m_fRebirthTime;	//다시 Lock되는 시간
//	float m_fElapsedTime;	//UnLock 경과시간
//	float m_fMonPosX;		//몬스터 죽은위치X
//	float m_fMonPosY;		//몬스터 죽은위치Y
//	bool m_bLock;
//	bool m_bComplete;
//};
//
//DEFINE_GAME_OBJ_FACTORY( KGCMonsterLink );
//DECLARE_GAME_OBJ_FACTORY( KGCMonsterLink );
//DECLARE_GAME_OBJ_FACTORY_NAME( KGCMonsterLink );
//
//#endif // _GCMONSTERLINK_H_
