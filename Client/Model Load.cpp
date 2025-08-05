#include "stdafx.h"
//#include "P2P.h"
#include "MyD3D.h"
#include "GCStateInitLoading.h"
#include "Models.h"
//#include <sstream>
#include <iomanip>
#include "KStats.h"
//#include "GCDeviceManager2.h"

KSafeArray<KAngleBoneFromFile,100> temp_A_Bone;

Models::Models()
{
	/*
// 기사 프로필
	Force[CID_ELESIS1]=0;
	Char_Height[CID_ELESIS1]=0;

	I_Sound[CID_ELESIS1].ATK[0]=13;
	I_Sound[CID_ELESIS1].ATK[1]=14;
	I_Sound[CID_ELESIS1].ATK[2]=13;
	I_Sound[CID_ELESIS1].ATK[3]=14;
	I_Sound[CID_ELESIS1].ATK[4]=13;
	I_Sound[CID_ELESIS1].AH=18;
	I_Sound[CID_ELESIS1].AHH=19;
	I_Sound[CID_ELESIS1].AHHH=91;
    I_Sound[CID_ELESIS1].STAND=114;

	
	I_Armor[CID_ELESIS1].x=-.058f;
	I_Armor[CID_ELESIS1].y=.0009f;//.12
	I_Armor[CID_ELESIS1].z=.84f;
	I_Armor_Ex[CID_ELESIS1].x=-.058f;
	I_Armor_Ex[CID_ELESIS1].y=.0009f;
	I_Armor_Ex[CID_ELESIS1].z=.14f;

// 궁수 프로필
	Force[CID_LIRE1]=8;
	Char_Height[CID_LIRE1]=-.045f;

	I_Sound[CID_LIRE1].ATK[0]=20;
	I_Sound[CID_LIRE1].ATK[1]=21;
	I_Sound[CID_LIRE1].ATK[2]=20;
	I_Sound[CID_LIRE1].ATK[3]=21;
	I_Sound[CID_LIRE1].ATK[4]=20;
	I_Sound[CID_LIRE1].AH=25;
	I_Sound[CID_LIRE1].AHH=26;
	I_Sound[CID_LIRE1].AHHH=92;
    I_Sound[CID_LIRE1].STAND=115;

	I_Armor[CID_LIRE1].x=0;
	I_Armor[CID_LIRE1].y=.12f;
	I_Armor[CID_LIRE1].z=.8f;
	I_Armor_Ex[CID_LIRE1].x=0;
	I_Armor_Ex[CID_LIRE1].y=.12f;
	I_Armor_Ex[CID_LIRE1].z=.2f;

// 마법사 프로필
	Force[CID_ARME1]=15;
	Char_Height[CID_ARME1]=-.049f;

	I_Sound[CID_ARME1].ATK[0]=66;
	I_Sound[CID_ARME1].ATK[1]=67;
	I_Sound[CID_ARME1].ATK[2]=66;
	I_Sound[CID_ARME1].ATK[3]=67;
	I_Sound[CID_ARME1].ATK[4]=66;
	I_Sound[CID_ARME1].AH=58;
	I_Sound[CID_ARME1].AHH=59;
	I_Sound[CID_ARME1].AHHH=93;
    I_Sound[CID_ARME1].STAND=116;

	I_Armor[CID_ARME1].x=.061f;
	I_Armor[CID_ARME1].y=-.022f;
	I_Armor[CID_ARME1].z=.237f;
	I_Armor_Ex[CID_ARME1].x=-.04f;
	I_Armor_Ex[CID_ARME1].y=-.04f;
	I_Armor_Ex[CID_ARME1].z=.08f;

	//Las
	Force[CID_LAS1]=15;
	Char_Height[CID_LAS1]=0;

	I_Sound[CID_LAS1].ATK[0]=183;
	I_Sound[CID_LAS1].ATK[1]=184;
	I_Sound[CID_LAS1].ATK[2]=185;
	I_Sound[CID_LAS1].ATK[3]=183;
	I_Sound[CID_LAS1].ATK[4]=187;
	I_Sound[CID_LAS1].AH=192;
	I_Sound[CID_LAS1].AHH=193;
	I_Sound[CID_LAS1].AHHH=194;
	I_Sound[CID_LAS1].STAND=190;


	I_Armor[CID_LAS1].x=-.058f;
	I_Armor[CID_LAS1].y=.0009f;//.12
	I_Armor[CID_LAS1].z=-.5f;
	I_Armor_Ex[CID_LAS1].x=-.058f;
	I_Armor_Ex[CID_LAS1].y=.0009f;
	I_Armor_Ex[CID_LAS1].z=.12f;

// 창병 프로필
    Force[CID_ELESIS2]=0;
    
    I_Sound[CID_ELESIS2].ATK[0]=13;
    I_Sound[CID_ELESIS2].ATK[1]=14;
	I_Sound[CID_ELESIS2].ATK[2]=13;
	I_Sound[CID_ELESIS2].ATK[3]=14;
	I_Sound[CID_ELESIS2].ATK[4]=13;
    I_Sound[CID_ELESIS2].AH=18;
    I_Sound[CID_ELESIS2].AHH=19;
    I_Sound[CID_ELESIS2].AHHH=91;
    I_Sound[CID_ELESIS2].STAND=114;

    I_Armor[CID_ELESIS2].x=.036f;
    I_Armor[CID_ELESIS2].y=.022f;
    I_Armor[CID_ELESIS2].z=.995f;
    I_Armor_Ex[CID_ELESIS2].x=.036f;
    I_Armor_Ex[CID_ELESIS2].y=.022f;
    I_Armor_Ex[CID_ELESIS2].z=.1f;    


// 석궁 프로필    
    Force[CID_LIRE2]=0;
    
    I_Sound[CID_LIRE2].ATK[0]=20;
    I_Sound[CID_LIRE2].ATK[1]=21;
	I_Sound[CID_LIRE2].ATK[2]=20;
	I_Sound[CID_LIRE2].ATK[3]=21;
	I_Sound[CID_LIRE2].ATK[4]=20;
    I_Sound[CID_LIRE2].AH=25;
    I_Sound[CID_LIRE2].AHH=26;
    I_Sound[CID_LIRE2].AHHH=92;
    I_Sound[CID_LIRE2].STAND=115;    
    
    I_Armor[CID_LIRE2].x=0;
    I_Armor[CID_LIRE2].y=.12f;
    I_Armor[CID_LIRE2].z=.8f;
    I_Armor_Ex[CID_LIRE2].x=0;
    I_Armor_Ex[CID_LIRE2].y=.12f;
    I_Armor_Ex[CID_LIRE2].z=.2f;


    // 연금술사 프로필
    Force[CID_ARME2]=15;
    
    I_Sound[CID_ARME2].ATK[0]=66;
    I_Sound[CID_ARME2].ATK[1]=67;
	I_Sound[CID_ARME2].ATK[2]=66;
	I_Sound[CID_ARME2].ATK[3]=67;
	I_Sound[CID_ARME2].ATK[4]=66;
    I_Sound[CID_ARME2].AH=58;
    I_Sound[CID_ARME2].AHH=59;
    I_Sound[CID_ARME2].AHHH=93;
    I_Sound[CID_ARME2].STAND=116;
    
    I_Armor[CID_ARME2].x=.061f;
    I_Armor[CID_ARME2].y=-.022f;
    I_Armor[CID_ARME2].z=.237f;
    I_Armor_Ex[CID_ARME2].x=-.04f;
    I_Armor_Ex[CID_ARME2].y=-.04f;
    I_Armor_Ex[CID_ARME2].z=.08f;    

    // 자객 프로필
    Force[CID_LAS2]=0;
    Char_Height[CID_LAS2]=0;

    I_Sound[CID_LAS2].ATK[0]=183;
    I_Sound[CID_LAS2].ATK[1]=184;
    I_Sound[CID_LAS2].ATK[2]=185;
    I_Sound[CID_LAS2].ATK[3]=183;
    I_Sound[CID_LAS2].ATK[4]=187;
    I_Sound[CID_LAS2].AH=192;
    I_Sound[CID_LAS2].AHH=193;
    I_Sound[CID_LAS2].AHHH=194;
    I_Sound[CID_LAS2].STAND=190;


    I_Armor[CID_LAS2].x=-.058f;
    I_Armor[CID_LAS2].y=.0009f;//.12
    I_Armor[CID_LAS2].z=.70f;
    I_Armor_Ex[CID_LAS2].x=-.058f;
    I_Armor_Ex[CID_LAS2].y=.0009f;
    I_Armor_Ex[CID_LAS2].z=.12f;


	I_Sound[CID_ELESIS3].ATK[0]=13;
	I_Sound[CID_ELESIS3].ATK[1]=14;
	I_Sound[CID_ELESIS3].ATK[2]=13;
	I_Sound[CID_ELESIS3].ATK[3]=14;
	I_Sound[CID_ELESIS3].ATK[4]=13;
	I_Sound[CID_ELESIS3].AH=18;
	I_Sound[CID_ELESIS3].AHH=19;
	I_Sound[CID_ELESIS3].AHHH=91;
	I_Sound[CID_ELESIS3].STAND=114;


	I_Armor[CID_ELESIS3].x=-.058f;
	I_Armor[CID_ELESIS3].y=.0009f;//.12
	I_Armor[CID_ELESIS3].z=.84f;
	I_Armor_Ex[CID_ELESIS3].x=-.058f;
	I_Armor_Ex[CID_ELESIS3].y=.0009f;
	I_Armor_Ex[CID_ELESIS3].z=.14f;
	*/

    for( int i = 0 ; i < (int)m_pMeshPlayerShadow.size() ; ++i )
    {
        m_pMeshPlayerShadow[i] = NULL;
    }
}

void Models::DeleteDevice()
{
	for(int i = 0; i < GC_CHAR_NUM; i++)
        SAFE_DELETE( m_pMeshPlayerShadow[i] );
}

void Models::InitDevice()
{
	for(int i = 0; i < GC_CHAR_NUM; i++)
	{
		char strFileName[MAX_PATH];
		sprintf(strFileName,"Shad%03d.p3m",i);
		m_pMeshPlayerShadow[i] = new GCObject();
		m_pMeshPlayerShadow[i]->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( strFileName ), g_pGCDeviceManager2->CreateTexture("SHADOW.DDS") );
	}
}

