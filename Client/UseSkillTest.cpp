#include "stdafx.h"
#ifdef _DEBUG
#include "./UnitTest/UnitTest++.h"
#include "GCSKT.h"
#include "MyD3D.h"
//


SUITE( Elesis1JumpAtkSkillTest )
{
    // 2 stage 개념의 테스트이므로 게임내 객체를 이용하자.

    TEST( SettingElesis1JumpAtSkill )
    {
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_JUMP_ATK1 );
    }

    TEST( SkillTrainedTest )
    {
        CHECK( g_kGlobalValue.CheckTrainedSkill( g_MyD3D->GetMyPlayer(), SID_ELESIS1_JUMP_ATK1 ) );
    }
}

SUITE( SkillResourceChangeTemplateSetting )
{
    TEST( SettingComboSkill )
    {
        //SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_JUMP_ATK1 );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_SUPER_DASH );
        //SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_COMBO_A );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_CRITICAL_ATK );
        //SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_DASH_ATK1 );
        //SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_DASH_ATK2 );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_DASH_ATK3 );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_FAST_ATK );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_SPECIAL_ATK1 );
        SiGCSKT()->_D_TrainSkill( GC_CHAR_ELESIS, 0, SID_ELESIS1_SPECIAL_ATK2 );
        SiGCSKT()->_D_InsertLocalPlayerSkillSet( g_kGlobalValue.m_kUserInfo.dwUID, GC_CHAR_ELESIS, 0 );
    }

    TEST( LoadSkillResource )
    {
        SiGCSKT()->LoadingResource( g_kGlobalValue.m_kUserInfo.dwUID );
        g_KDSound.FullLoadfromLoadList();
    }
}

#endif