// KGCAnimManager.h: interface for the KGCAnimManager class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCANIMMANAGER_H__BB3B67F4_AA65_4911_B112_9FC5B23D366D__INCLUDED_)
//#define AFX_KGCANIMMANAGER_H__BB3B67F4_AA65_4911_B112_9FC5B23D366D__INCLUDED_

#ifndef _KGCANIMMANAGER_H_
#define _KGCANIMMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KTDGAnim.h"
#include "../Particle/KGCParticleManager.h"
//#include <list>

struct PositionDesc;

class KGCAnimManager  
{
    public:
        struct ItemAnim
        {
            std::vector<std::string>  itemName;
            std::string             sequenceName;
            CKTDGAnim::CAnimSequence*   animSeq;
            CKTDGAnim::CAnimInstance*   animInst;

            CKTDGAnim::PlayType     playType;

            CMinMax<float>          posX;
            CMinMax<float>          posY;
            CMinMax<float>          posZ;

            CMinMax<float>          scaleX;
            CMinMax<float>          scaleY;

            CMinMax<float>          rotX;
            CMinMax<float>          rotY;
            CMinMax<float>          rotZ;

            EMIT_POSITION_TYPE      positionType;
            CHAR_IDENTY             charID;
            int                     enchantLevel;
            bool                    dependLookAt;
            
            // 겹치는 아이템들을 처리하기 위한 변수. 0이 기본값이고 따로 처리하지 않음, player.h에서 사용한다.
            int                     positionIndex;

            bool                    angledirection;
			bool                    LookAtRotate;
            ItemAnim()
            : sequenceName("")
            , animSeq(NULL)
            , animInst(NULL)
            , playType(CKTDGAnim::PT_ONE_PASS)
            , positionType(EPT_BODY_CENTER)
            , charID(CID_COMMON)
            , enchantLevel(0)
            , dependLookAt(false)
            , positionIndex(0)
            , angledirection(false)
            , LookAtRotate(false)
            {
                itemName.clear();

            }
        };
        
        struct DamageAnim
        {
            DamageAnim()
            : sequenceNameLeft("")
            , sequenceNameRight("")
            , animSeqLeft(NULL)
            , animSeqRight(NULL)
            , playType(CKTDGAnim::PT_ONE_PASS)
            , charID(CID_COMMON)
            , enchantLevel(0)
            {
                itemName.clear();
            }
			std::vector<std::string>  itemName;
            std::string             sequenceNameLeft;
            std::string             sequenceNameRight;

            CKTDGAnim::CAnimSequence*   animSeqLeft;
            CKTDGAnim::CAnimSequence*   animSeqRight;

            CKTDGAnim::PlayType     playType;

            CMinMax<float>          posX;
            CMinMax<float>          posY;
            CMinMax<float>          posZ;

            CMinMax<float>          scaleX;
            CMinMax<float>          scaleY;

            CMinMax<float>          rotX;
            CMinMax<float>          rotY;
            CMinMax<float>          rotZ;

            CHAR_IDENTY             charID;
            int                     enchantLevel;
        };

        struct SkillAnim
        {
			SkillAnim()
            : animSeqLeft(NULL)
            , animSeqRight(NULL)
            , maxTime(0)
            , emitRemainTime( 0.f )
            , isRight( true )
            , enchantLevel(0)
			{
				remainTime = 0.0f;
			}
            std::vector<std::string>  itemName;
            std::string             sequenceNameLeft;
            std::string             sequenceNameRight;

            CKTDGAnim::CAnimSequence*   animSeqLeft;
            CKTDGAnim::CAnimSequence*   animSeqRight;

            CKTDGAnim::PlayType     playType;

            CMinMax<float>          posX;
            CMinMax<float>          posY;
            CMinMax<float>          posZ;

            CMinMax<float>          scaleX;
            CMinMax<float>          scaleY;

            CMinMax<float>          rotX;
            CMinMax<float>          rotY;
            CMinMax<float>          rotZ;

            EMIT_POSITION_TYPE      positionType;
            CHAR_IDENTY             charID;

            float                   maxTime;
            float                   remainTime;
            CMinMax<float>          emitRate;
            float                   emitRemainTime;

            bool                    isRight;
            D3DXVECTOR3             pos;
            int                     enchantLevel;
        };

    public:
	    KGCAnimManager();
	    virtual ~KGCAnimManager();

        void LoadFromLuaScript( char* stringfile, char* filename );

        void StartGame();
        void EndGame();
		void DeleteSkillInst( SkillAnim* pInst );
		void DeleteInst( CKTDGAnim::CAnimInstance* pInst );

        void FrameMove( float fElapsedTime = 0.01818f );
		void Render();
        
        // outAnim에 push한 eff의 갯수를 리턴 ㅋ
        int GetNewItemAnim( std::string itemName, CHAR_IDENTY charID, std::vector<ItemAnim>& outAnim, int enchantLevel = 0 , bool bSquare_  = false);
        void FrameUpdateItemAnim( ItemAnim* anim, PositionDesc* posDesc, int life );

        void GetNewDamageAnim( std::string itemName, CHAR_IDENTY charID, std::vector<DamageAnim>& outAnim, int enchantLevel = 0 );
        void FrameUpdateDamageAnim( DamageAnim* anim, const D3DXVECTOR3& impactPos, bool right );

        void GetNewSkillAnim( std::string itemName, CHAR_IDENTY charID, std::vector<SkillAnim*>& outAnim, int enchantLevel = 0 );
        void FrameUpdateSkillAnim( SkillAnim* anim, PositionDesc* posDesc );

        CKTDGAnim::CAnimInstance* GetAnimSequence( char* sequence );
		CKTDGAnim* GetManager(){ return m_pKTDGAnim; }

        float GetZPos() const;

        void InitDeviceObjects();
        void DeleteDeviceObjects();

        void SetEnchantLevel( std::string itemName, int iEnchantLevel );
        //bool IsItemAnim( std::string itemName );
        bool IsItemAnim( GCITEMID itemID );

        CKTDGAnim::CAnimInstance*                       m_pAnim;

    private:
		EMIT_POSITION_TYPE ParsePosition( std::string str );

        CKTDGAnim*      m_pKTDGAnim;

        std::vector<ItemAnim>   m_ItemAnimTemplet;
        std::vector<DamageAnim> m_DamageAnimTemplet;
        std::vector<SkillAnim>  m_SkillAnimTemplet;
        std::vector<SkillAnim*> m_SkillAnimInstance;
};

extern KGCAnimManager g_AnimManager;

#endif // _KGCANIMMANAGER_H_
