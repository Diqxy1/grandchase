// PetAI.h: interface for the CPetAI class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_PETAI_H__1B8387BA_EA26_4824_A35A_86EB71D85158__INCLUDED_)
//#define AFX_PETAI_H__1B8387BA_EA26_4824_A35A_86EB71D85158__INCLUDED_

#ifndef _PETAI_H_
#define _PETAI_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatencyTransfer.h"

#define AI_SEND_GAP 0.3f

struct PositionDesc;
class PLAYER;
class GCSquarePeople;

struct KPetAIMasterInfo
{
    bool bIsRight;
    PositionDesc* m_posDesc;
    D3DXVECTOR3* vPos;

    KPetAIMasterInfo( PLAYER* pPlayer );
    KPetAIMasterInfo( GCSquarePeople* pPeople );

    void Update( PLAYER* pPlayer );
    void Update( GCSquarePeople* pPeople );

    ~KPetAIMasterInfo(){}
};

class CPetAI  
{
	public:
		struct EffectSequence
		{
			int				versionNum;
			std::string		effectName;
			CMinMax<float>	emitRate;
			bool			trace;
		};

		struct MagicSequence
		{
			std::string		magicName;
			float			magicRate;
			CMinMax<float>	magicDelay;
		};

		struct AISequence
		{
			std::string				templetName;
			bool					bActive;
            bool                    bFollowPlayerRight;

			float					fYGap;
			float					fXGap;
			float					fTrackRate;		//따라다니는 정도

			float					fMoveXScale;
			float					fMoveYScale;
			float					fMoveSpeed;			
			float					fMoveRotate;
			float					fMoveRotateSpeed;

			float					fSizeX;
			float					fSizeY;
			KSafeArray<std::string,PM_END> motionAnimList;
			vector<MagicSequence>	magicSeqList;
			vector<EffectSequence>	effectSeqList;
		};

		struct AIObject
		{
			AIObject()
            : m_pMasterInfo(NULL)
			{
                ID              = 0;
                iMaster         = 0;
                AI              = NULL;

                bEnable         = false;
                bDie            = false;
                bMonsterMaster  = false;

                fNowMoveTime    = 0.0f;

                fTracking = false;            

                SETVECTOR3(pos,0.0f,0.0f,0.0f);
                bMagicStartThisFrame = false;
                motion = PM_FLY1;
                for( int i = 0 ; i < (int)motionAnimList.size() ; ++i )
                {
                    motionAnimList[i] = NULL;
                }

                bXArrived = false;
                bYArrived = false;
                bRight = false;

                fMagicDelay = 0.0f;
                effSeqList2.clear();

                criticalSection;
                beforeFrame = 0;
                m_pMasterInfo = NULL;


				vCenterPos.x = 0.0f;
				vCenterPos.y = 0.0f;
                bShow = true;
			}
            ~AIObject()
            {
                SAFE_DELETE( m_pMasterInfo );
            }
			int							ID;
			AISequence*					AI;

            bool                        bShow;
			bool						bEnable;
			bool						bDie;
			int							iMaster;
			int							bMonsterMaster;
			
			D3DXVECTOR2					vCenterPos;			//현재 중심 위치
			float						fNowMoveTime;

			bool						fTracking;            

            D3DXVECTOR3					pos;
			bool						bMagicStartThisFrame;
            PET_MOTION					motion;
			KSafeArray<CKTDGAnim::CAnimInstance*,PM_END> motionAnimList;
            
			bool						bXArrived;
            bool						bYArrived;            
            bool						bRight;

			float						fMagicDelay;
			vector<CParticleEventSeqPTR> effSeqList2;

			CRITICAL_SECTION			criticalSection;
            int							beforeFrame;
            KPetAIMasterInfo*           m_pMasterInfo;
		};
		
		struct ItemPack
		{
			std::wstring		itemName;
			std::string		petName;
		};

	public:
		CPetAI();
		virtual ~CPetAI();

		void Free();
		
		void LoadPetItemFromLuaScript( const char* strStringFile, const char* fileName );
        std::vector< int > CreatePetinItem( const WCHAR* itemName, int age, int master, bool bMonsterMaster = false , int iItemID = -1);
        void LoadTempletFromLuaScript( const char* fileName, const char* templetName );
		AIObject* AddAIObject( int ID, const char* templetName, int masterNum, bool bMonsterMaster = false );
		void ClearAIObject( AIObject* aiObject );
        void ClearAIObject();
		void ClearAIObject( int iMaster );
		
		void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
        void PacketMove( LTP_PET_AI_MOVE* movePacket );

		void AllPetDisable();
		void AllPetEnable();
		
        AIObject* GetAIObject( int ID );

        void Show( std::vector< int > vecID, bool bShow );

        bool IsOldPet( DWORD dwItemID );

	private:
        void Magic( AIObject* aiObject );
        void Update( AIObject* aiObject );
        void ShowUpdate( AIObject* aiObject );
		
        void StartUpMagic( AIObject* aiObject, int magicNum );
        void ProcessMagic( AIObject* aiObject, int magicNum );
		
		float					m_fElapsedTime;
		
		vector<AISequence*>		m_AISeqList;
		vector<AIObject*>		m_AIObjectList;
		
        float					m_fNowSendTime;
		
        LTP_PET_AI_MOVE         m_MovePacket;
        LTP_PET_AI_MOVE_PACK    m_MovePacketList;

		vector<ItemPack>		m_ItemList;

        bool IsUsePlayer() const;
        void UpdateMasterInfo( AIObject* pAI );
};

extern CPetAI* g_pkPetAI;

#endif // _PETAI_H_
