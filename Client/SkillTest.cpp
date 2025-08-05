#include "stdafx.h"
#ifdef _DEBUG
#include "./UnitTest/UnitTest++.h"
//
//
#include "GCSKT.h"
#include "MyD3D.h"

SUITE( SKillTest )
{
    // 1 stage 개념의 테스트이므로 여기서 스크립트 로딩하고 해제.

    struct SkillTestFixture
    {
        SkillTestFixture() {}
        ~SkillTestFixture() {}

        static void Init()
        {
            if( ms_pTestSKT == NULL )
            {
                ms_pTestSKT = new GCSKT();
            }
        }
        static void Release()
        {
            if( ms_pTestSKT != NULL )
            {
                delete ms_pTestSKT;
                ms_pTestSKT = NULL;
            }
        }
        static GCSKT* ms_pTestSKT;
    };
    GCSKT* SkillTestFixture::ms_pTestSKT = NULL;

    TEST_FIXTURE( SkillTestFixture, SkillLoadTest )
    {
        SkillTestFixture::Init();

        KLuaManager kSkillTreeLua;
        // for load lua..
        GCDeviceManagerWrapper* pGCDeviceManager = new GCDeviceManagerWrapper( g_pd3dDevice );
        pGCDeviceManager->GetMassFileManager()->AddDirectory("./Stage/");
		GCFUNC::LoadLuaScript(kSkillTreeLua, "Enum.stg" );
#if defined( NATION_PHILIPPINE )
        GCFUNC::LoadLuaScript( kSkillTreeLua, "InitSkillTree_ph.lua" );
#else
        GCFUNC::LoadLuaScript( kSkillTreeLua, "InitSkillTree.lua" );
#endif

        CHECK( ms_pTestSKT->LoadSkillScript( kSkillTreeLua ) );
        
        delete pGCDeviceManager;
    }

    TEST_FIXTURE( SkillTestFixture, SettingSkill )
    {
        // 점프 어택과 슈퍼대쉬를 배워 놓는다
        ms_pTestSKT->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_JUMP_ATK1 );
        ms_pTestSKT->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_SUPER_DASH );
    }

    TEST_FIXTURE( SkillTestFixture, SkillTrainTest )
    {
        // 슈퍼대쉬공격을 배울수 있나?
        CHECK( ms_pTestSKT->IsSkillTrainable( SID_ELESIS1_DASH_ATK1 ) );
    }

    TEST_FIXTURE( SkillTestFixture, FinishSkillTest )
    {
        // 마지막 테스트에는 불러줘야한다.
        SkillTestFixture::Release();
    }
}
#endif
