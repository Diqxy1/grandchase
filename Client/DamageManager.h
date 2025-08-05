#ifndef _DAMAGEMANAGER_H_
#define _DAMAGEMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Damage.h"
#include "GCDeviceLib/GCDeviceIndexBuffer.h"

#define MAX_DAMAGE_INSTANCE (512)
#define MAX_DAMAGE_VERTEX	(boost::mpl::multiplies<boost::mpl::int_<MAX_DAMAGE_INSTANCE>, boost::mpl::int_<4>>::next::value)
#define RANDOM_NUMBER		(static_cast<float>( ::rand() ) / static_cast<float>( RAND_MAX ))

struct OwnerData
{
    int Who;
    float x;
    float y;
    float Body_Angle;
    UINT Now_Motion;
    int Frame;
};

class CDamageManager
{

public:
		CDamageManager(void);
		~CDamageManager(void);
        
		void LoadFireInfo();
        bool LoadFireInfoTable( KLuaManager& luaMgr, std::vector<CDamage*>& Info );
        bool LoadFireInfoSingleObject( KLuaManager& luaMgr, CDamage* pDamage );
        void ReloadFireInfo();
		
		OUT CDamageInstance* Add( int What, int Who, bool NoSlowCount = false , int ExtraMonsterNum = -1, IN CDamageInstance* pDamageInstance_ = NULL , bool bAlwaysRight = false);
		OUT CDamageInstance* AddWithAngle( int What, int Who, float fStartAngle ,bool NoSlowCount = false, int ExtraMonsterNum = -1, IN CDamageInstance* pDamageInstance_ = NULL );
		OUT CDamageInstance* AddWithLocate( int What, int Who, float fStartX , float fStartY ,bool NoSlowCount = false, int ExtraMonsterNum = -1, 
											IN CDamageInstance* pDamageInstance_ = NULL, int HitWho = -1, DIRECTION_INFO* stDirect = NULL,
											OBJ_TYPE emWhoType = OBJ_TYPE_PLAYER, OBJ_TYPE emHitWhoType = OBJ_TYPE_PLAYER );
        OUT CDamageInstance* PetAdd( int What, int Who, bool NoSlowCount = false , int ExtraMonsterNum = -1, float fAtkRatio = 1.0f, bool bPetTrace = false, bool bPlayerTrace = false );
        OUT CDamageInstance* PetAddWithAngle( int What, int Who, float fStartAngle ,bool NoSlowCount = false, int ExtraMonsterNum = -1, float fAtkRatio = 1.0f );
        OUT CDamageInstance* PetAddWithLocate( int What, int Who, float fStartX , float fStartY ,bool NoSlowCount = false, int ExtraMonsterNum = -1, float fAtkRatio = 1.0f );
		//몬스터가 Damage만들 때.
        CDamageInstance* MonsterAddLastPlayers( int What, int LastPlayeri, int Monsteri, bool NoSlowCount = false );
		CDamageInstance* MonsterAdd( int What, int Monsteri, bool NoSlowCount = false );
		CDamageInstance* MonsterAddWithAngle( int What, int Monsteri, float fStartAngle ,bool NoSlowCount = false );
        CDamageInstance* MonsterAddWithLocate( int What, int Monsteri, float fStartX , float fStartY ,bool NoSlowCount = false );
        CDamageInstance* MonsterAddWithTargetPos( int What, int Monsteri, float fStartX , float fStartY, float fTartgetPosX, float fTartgetPosY, std::string strAction = "" );
		bool MakeDamage(int What , CDamageInstance* pDamageInstance);
		void ControlVelocityForReplay( CDamage* pDamage , int What );

		//이 아래 함수는 반드시 루아로 빼고 말 것이다.
		void CallEffAtCrash(CDamageInstance* pDamageInstance,int HitWho,bool IsCrashPlayer, float _fHitX = 0.0f, float _fHitY = 0.0f );
		void CallEffAtGroundCrash(CDamageInstance* pDamageInstance);
		void CallEffAtTime(CDamageInstance* pDamageInstance);
		void CallEffAtEffectCrash(CDamageInstance* pDamageInstance, CDamageInstance* pTargetInstance);
		void CallEffAtEffectCrashForce(CDamageInstance* pDamageInstance, CDamageInstance* pTargetInstance);
		void CallAnim( CDamageInstance* pDamageInstance , std::string strAnim , float fScaleX , float fScaleY );

		void Remove( CDamageInstance* pDamageInstance );
		void RemoveAll();

		void FrameMove();
		void Render();

        void CrashWithUpGround( CDamageInstance* pDamageInstance, float fOffsetX=0.0f, float fOffsetY=0.0f );
		void CrashWithPlayer( CDamageInstance* pDamageInstance );
		void CrashWithMonster( CDamageInstance* pDamageInstance );
        void CrashWithObject( CDamageInstance* pDamageInstance );        
		void CrashWithMinigameObj( CDamageInstance* pDamageInstance );
		void CrashWithEffect( CDamageInstance* pDamageInstance );
		bool CrashWithWeapon( int iPlayerIndex, GCCollisionRect<float>* rtWeapon);
		void CrashWithMonsterAttack( CDamageInstance* pDamageInstance );
        bool CrashWithMonsterPhsAtk( int iMonsterIndex, GCCollisionRect<float>* rtWeapon );
		void CrashWith2DObject( CDamageInstance* pDamageInstance );

		void Load( int iLoadLevel = -1 );
		void UnLoad( void );
		void InitDevice( void );
		void DeleteDevice();

		BOOL IsExistDamage( DAMAGE_TYPE eDamageType ); //현재 존재하는 데미지인가?
        BOOL IsExistDamage( CDamageInstance* pDamageInstance ); //현재 존재하는 데미지인가?
        bool IsMonsterDamageToType( int iMonsterIdx, int iDamageType );
        void DeleteInstance( CDamageInstance* pDamageInstance );        
        void DeleteInstance( int iDamageEnum_, int iPlayerIndex_ );
        void DeleteMonsterDamageToType( int iMonsterIdx, int iDamageType, int iEraseLife = 0 );
		void DeleteMonsterInstance( );
        BOOL SetDamageLife( int iDamageType, int iLife );// 데미지 시간조절
        int GetDamageLife( int iDamageType );// 데미지 시간얻기
        void ActivateTrigger( CDamageInstance* pDamageInstance, int iTriggerIndex );
		
        inline CDamage* GetOriginalDamage( int What )
        {
            if( static_cast<int>(m_vecInfo.size()) <= What )
            {
                ASSERT( static_cast<int>(m_vecInfo.size()) > What );
                return NULL;
            }

            return m_vecInfo[What];
        }

        std::vector< CDamage* >		m_vecInfo;				//데미지 이펙트 정보
		GCDeviceTexture**			m_apTexture; 		///< 데미지 이펙트를 그리기위한 텍스처
		KSafeArray<int,MAX_PLAYER_NUM> m_iPlayerIndex;
		std::map<int, int> m_mapMonsterIndex;

        void DamagedFireHardcordingPreProcess( int What, const OwnerData& owner, CDamageInstance* pDamageInstance );

private:
		void CheckAndChange( int& What , int Who );                 //목걸이를 끼거나 할 때 인스턴스 바꾸기
        void CheckMonsterAndChange( int&What, int iMonsterType );   // 특정 몬스터의 인스턴스 바꾸기 CheckAndChange의 몬스터판..
		bool CheckEnableCreate();						// 생성가능한지 검사
        void DeleteMonsterDamage( int iMonsterIdx, int iDamageType );

		// 정점/인덱스 버퍼 세팅
		int SetBuffer(SLVERTEX* pVB, std::map< int, std::vector<CDamageInstance*> >::iterator itor);

        inline int LifeToAlpha( int iLife_, int iLifeToDecreaseAlpha_ )
        {
            int iAlpha = 255/iLifeToDecreaseAlpha_*iLife_;
            if ( 255 < iAlpha )
                iAlpha = 255;
			if ( iAlpha < 0 )
				iAlpha = 0;
            return iAlpha;
        }

public:
        void LoadMeshInfo( KLuaManager& luaMgr, MESH_INFO& meshInfo );
        void CreateMeshAnimation( CDamageInstance* pDamage );
        void CreateMeshAnimationForPrevLoad( int iWhat, int iPidx );

        void LoadCrashParticle( KLuaManager& luaMgr, std::vector< CRASH_PARTICLE >& vecCrashInfo );
		void LoadAutoDamage( KLuaManager& luaMgr, std::vector< AUTO_DAMAGE >& vecAutoDamageInfo );
        void LoadTargetAutoDamage( KLuaManager& luaMgr, std::vector< TARGET_AUTO_DAMAGE >& vecTargetAutoDamageInfo );
		void LoadTimeSound( KLuaManager& luaMgr, std::vector< TIME_SOUND >& vecTimesound );
        void LoadTrigger( KLuaManager& luaMgr, std::vector< DAMAGE_TRIGGER >& vecDamageTrigger );
        void LoadOnlyMotionDamage( KLuaManager& luaMgr, OnlyMotionDamage& kOnlyMotion );

private:
        std::map< int, std::vector<CDamageInstance*> > m_mapDamageInstance; // 키값 : 데미지인덱스, 데미지생성자, 데이터 : 키값이 생성한 데미지 리스트
                                                                                              // 몬스터 생성 데미지 키값 : who + monster index

		int m_i_MAX_DAMAGE_TEX_NUM;

public:
		//라이트닝 볼트 옛날 하드 코딩... 정말 없애고 싶었지만 이 오묘함을 재현할 수 없어 결국...ㅠㅠ 
		//복구한다....ㅡ.ㅡ....
		bool Add_LighteningBolt( int PosWho, CDamageInstance* pDamageInstance );	
        void EffectDust( float fX, float fY, CDamageInstance* pDamageInstance, int iLatency = 0, bool bRound = false );
        std::map< int, std::vector<CDamageInstance*> >& GetDamageInstancePool() { return m_mapDamageInstance; }

        void MakeSoundLoadList();
        void MakeSoundLoadList( std::vector<CDamage*>& vecInfo );
        bool IsCrashCheckPerFrame( CDamageInstance* pDamageInstance );

		bool PushCheckWithPlayer( int iPlayerIndex );
		bool PushCheckWithMonster( int iMonsterIndex );

        bool IsAlreadyCrashDamage( CDamageInstance* pDamageInstance, int iHitWho ); // 이미 충돌되고 있는 데미지 인지

        void GetKeyDamageInstance( CDamageInstance* pDamageInstance, std::pair< int, int >& prKeyDamageInstance );


public:
		bool m_bIsSP75BTexture;
		//데미지의 범위를 표시하는 텍스쳐를 리로드하는 함수
		void ReloadSpecial75( bool bIsBlackTex );
        bool CheckBoundary( CDamageInstance* pDamageInstance );
        void ReflectionBoundary( CDamageInstance* pDamageInstance );
        bool CheckLastBottom( float fPosX, float fPosY );

        D3DXVECTOR2 SyncDamagePosAtCrashPos( CDamageInstance* pDamageInstance, int HitWho, bool bIsCrashPlayer );

private:
        OUT CDamageInstance* CreateDamage( int What, int Who, bool NoSlowCount , int ExtraMonsterNum , IN CDamageInstance* pDamageInstance_, DIRECTION_INFO* stDirect = NULL );

        KLuaManager* m_pLuaMgr;
private:
    std::map< std::pair<int, int>, int > m_mapEraseMonDamageList;   // pair<몬스터ID, 데미지ID>, 제거Life

public:
    int GetDamageInstanceCount( int iDamageEnum, int iLife = 0 );
    D3DXVECTOR2 GetDamageInstancePos( int iDamageEnum );
    void CheckTargetAutoDamage( CDamageInstance *pDamageInstance, int iHitWho = -1, bool IsCrashPlayer = false );
    bool AddTargetAutoDamage( CDamageInstance *pDamageInstance, TARGET_AUTO_DAMAGE *pAutoInfo, float fPosX, float fPosY, int iHitWho, bool bHitMonster );
    void CheckSyncPosDamageAtCrash( CDamageInstance *pDamageInstance );
    bool CheckDamagePerFrame(CDamageInstance *pDamageInstance );
};

#endif // _DAMAGEMANAGER_H_

