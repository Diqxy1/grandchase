#include "stdafx.h"
//
//



#include "DamageManager.h"

#include "Spark.h"

#include "GCStateMachine.h"
#include "Headup Display.h"
#include "KTDGFrustum.h"

#include "Stage.h"
//

//



#include "GCCameraOrtho.h"
//
//

//


#include "MapSystem/KGCLayerManager.h"
#include "KGCRoomManager.h"

#define FIELD_INFO(p)       Get_Field_Information(p)
#define LOW_INFO(p)         Get_Low_Block(p)
#define LOW_STAND_INFO(p)   Get_Low_Stand_Block(p)

/// 손 구멍의 위치
static const D3DXVECTOR2 gs_vHole[] =
{
    D3DXVECTOR2( -0.826f, 1.470f ),
    D3DXVECTOR2( -0.314f, 1.470f ),
    D3DXVECTOR2(  0.196f, 1.470f ),
    D3DXVECTOR2(  0.722f, 1.470f ),
    D3DXVECTOR2(  1.249f, 1.470f ),
    D3DXVECTOR2(  1.783f, 1.470f ),
    D3DXVECTOR2( -0.785f, 0.341f ),
    D3DXVECTOR2( -0.246f, 0.341f ),
    D3DXVECTOR2(  0.273f, 0.341f ),
    D3DXVECTOR2(  0.802f, 0.341f ),
    D3DXVECTOR2(  1.315f, 0.341f ),
    D3DXVECTOR2(  1.825f, 0.341f ),
};

#define HOLE_NUM        (sizeof( gs_vHole ) / sizeof( D3DXVECTOR2 ))    ///< 손 구멍의 개수
#define DUMMY_HAND_NUM  (3)                                             ///< 더미 손의 개수
#define PUSH_HAND_NUM   (2)                                             ///< 미는 손의 개수

// ------------------------------------------------------------------------------------------
// 생성자 및 소멸자
// ------------------------------------------------------------------------------------------

/// Default constructor
STAGE::STAGE( )
{
    Scroll_x=0;
    Color_r=255;
    Color_g=255;
    Color_b=255;
    Color_Stop_Count=0;
    Num_Block=0;

    // Init Texture
    for( int i = 0 ; i < (int)m_pTexLevel1.size() ; ++i )
    {
        m_pTexLevel1[i] = NULL;
    }
    m_pTexSky = NULL;
    m_pTexWater = NULL;
    m_pTexFront = NULL;
    m_pTexSand = NULL;

    m_fScrollY = 0.0f;
    m_bUpScroll = true;
    m_bDeathScrollY = false;

    m_bLava = true;
    m_iLava = 0;
    m_fLavaTrans1 = 0.0f;
    m_fLavaTrans2 = 0.0f;

    m_iSandFrame = 0;
    for( int i = 0 ; i < (int)m_afSandDrop.size() ; ++i )
    {
        m_afSandDrop[i] = 0.0f;
    }

    for( int i = 0; i < NUM_TOTAL_STAGE; i++ )
        m_iDustCount[i] = 0;

    RegisterLuabind();

    m_pdwTexStageNum = NULL;

    m_pShiningDust = NULL;
    m_vecDustPosition = NULL;

    for( int i = 0; i < (int)m_vecMapStreamData.size(); ++i )
        m_vecMapStreamData[i].Clear();
}

/// Default destructor
STAGE::~STAGE( void )
{
    // empty
    KGCLayerManager::ReleaseInstance();
    KGCFantasticMap::ReleaseInstance();
}

/// RestoreDeviceObjects - STAGE
/// @param void
/// @return void
//void STAGE::OnResetDevice()
void STAGE::InitDevice()
{
    
    V_BackGround[0].SetSLVER(-1.0f,.75f,0,0,0);
    V_BackGround[1].SetSLVER(1.0f,.75f,0,1.0f,0);
    V_BackGround[2].SetSLVER(-1.0f,-.75f,0,0,1.0f);
    V_BackGround[3].SetSLVER(1.0f,-.75f,0,1.0f,1.0f);
    

    V_Level_Back[0].SetSLVER(0,0.75f,0,0,0);
    V_Level_Back[1].SetSLVER(1.5f,0.75f,0,1.0f,0);
    V_Level_Back[2].SetSLVER(0,-0.75f,0,0,1.0f);
    V_Level_Back[3].SetSLVER(1.5f,-0.75f,0,1.0f,1.0f);         
}

/// DeleteDeviceObjects - STAGE
/// @param void
/// @return void
void STAGE::DeleteDevice( void )
{
    Release();
}

unsigned char STAGE::Get_Field_Information(int Index, bool bFileLoad/* = false*/)
{
    if(Index>Num_Width*Num_Height*900-1 || Index<0){
        return 0;
    }

    // 투명 발판 하드코딩 -_- 밟긴 밟아야지...
    if( (Field_Information[Index] == 13 || 
         Field_Information[Index] == 14 )
        && !bFileLoad )
        return 1;

    return Field_Information[Index];
}

char STAGE::Get_Low_Block(int Index)
{
    if(Index<0){
        Index=0;
    }else if(Index>Num_Width*30-1){
        Index=Num_Width*30-1;
    }
    return Low_Block[Index];
}

char STAGE::Get_Low_Stand_Block(int Index)
{
    if( Index < 0 )
    {
        Index = 0;
    }
    else if( Index > Num_Width * 30 - 1 )
    {
        Index = Num_Width * 30 - 1;
    }
    return Low_Stand_Block[Index];
}

void STAGE::Release()
{
    for(int i = 0; i < MAX_MAP_BLOCK; i++)
    {
        SAFE_RELEASE(m_pTexLevel1[i]);  
    }
    SAFE_RELEASE(m_pTexFront);
    SAFE_RELEASE(m_pTexSky);
    SAFE_RELEASE(m_pTexSand);
    SAFE_RELEASE(m_pTexWater);

    Num_Width = 0;
    Num_Height = 0;


    // 그려지고 있는 파티클도 지워 줍니다. 
    ClearParticle();

    // 파티클 정보 삭제
    for( int i = 0; i < NUM_TOTAL_STAGE; i++ )
	{
        if( m_pMapInfo[i].m_iNameID != -1 ) { 
			m_pMapInfo[i].m_vecParticleInfo.clear();
        }
	}

    SAFE_DELETE_ARRAY( m_pdwTexStageNum );

    SAFE_DELETE_ARRAY( m_pShiningDust );
    SAFE_DELETE_ARRAY( m_vecDustPosition );
    for( int i = 0; i < NUM_TOTAL_STAGE; i++ )
        m_iDustCount[i] = 0;

    m_vecTexture.clear();
}

/// 스테이지를 로드한다.
/// @param void
/// @return S_OK
HRESULT STAGE::Load_Stage( void )
{
    Color_r = 255;
    Color_g = 255;
    Color_b = 255;

    Event=0;

    SLVERTEX *v = NULL;
    
    V_BackGround[0].SetSLVER(-1.0f,.75f,0,0,0);
    V_BackGround[1].SetSLVER(1.0f,.75f,0,1.0f,0);
    V_BackGround[2].SetSLVER(-1.0f,-.75f,0,0,1.0f);
    V_BackGround[3].SetSLVER(1.0f,-.75f,0,1.0f,1.0f);    

    // *.Stg Load
    char FileName[50] = "";
    sprintf(FileName,"%02d.stg", (int)SiKGCRoomManager()->GetRoomInfo().ucStage);

    GCMemoryFile MemFile(g_pGCDeviceManager2->GetMassFileManager(), FileName );
    if( MemFile.IsLoaded() )
    {
        MemFile.ReadFile( &Num_Width , sizeof(char));
        MemFile.ReadFile( &Num_Height, sizeof(char));
        MemFile.ReadFile( (void*)&Field_Information , sizeof(char)*900*Num_Width*Num_Height );
    }

#if !defined( __PATH__ )
    FILE *pFile = fopen("MapData.txt", "wt");
    if( NULL != pFile )
    {
        fprintf(pFile, "%d\n", Num_Width );
        fprintf(pFile, "%d\n", Num_Height );
        for(int iLoop=0; iLoop<(900*Num_Width*Num_Height); ++iLoop)
        {
            if( 0 < iLoop && (iLoop%(Num_Width*30)) == 0 )
                fprintf(pFile, "\n");
            fprintf(pFile, "%d ", Field_Information[iLoop] );
        }
        fclose( pFile );
    }
#endif

    // 지형에 특이사항이 있는지 검사한다.
    for(unsigned char i=0;i<MAX_MAP_PORTAL;i++){
        PortalPosition[i].x=0;
        PortalPosition[i].y=0;
    }
    //====================================================================================
    // 2007.01.09 : Asirion
    // 1 : 밟는 발판
    // 2 : 왼쪽으로 미는 발판
    // 3 : 오른쪽으로 미는 발판
    // 4 : 안쓰임
    // 5~8 : 타운포탈 출구
    // 9~12 : 타운포탈 입구
    // 입구 - 4 : 해당 출구( 9번의 출구는 5번 )
    // 왜 이따구로 짰는지는 묻지마라...안구에 습기찬다-_-;
    for(unsigned char ix=0;ix<30*Num_Width;ix++)
    {
        Low_Stand_Block[ix]=-1;
        Low_Block[ix]=-1;
        for(int iy=0;iy<30*Num_Height;iy++)
        {
            // 제일 낮은 지형을 검사한다.
            if(Low_Stand_Block[ix]==-1 && FIELD_INFO(ix+iy*30*Num_Width)==1)
            {
                Low_Stand_Block[ix]=iy;                    
            }
            // 제일 낮은 의미있는 블럭을 검사한다.
            if(Low_Block[ix]==-1 && FIELD_INFO(ix+iy*30*Num_Width)==4 )
            {
                Low_Block[ix]=iy;                    
            }
            // 타운포탈 목표 지점을 검사한다.
            if(FIELD_INFO(ix+iy*30*Num_Width)>8 && FIELD_INFO(ix+iy*30*Num_Width)<13)
            {
                PortalPosition[Field_Information[ix+iy*30*Num_Width]-9].x=(float)(ix)/20.0f;
                PortalPosition[Field_Information[ix+iy*30*Num_Width]-9].y=(float)(iy)*.05f+.049f;
            }

            // 제일 낮은 의미있는 블럭을 검사한다.
            if(Low_Block[ix]==-1 && FIELD_INFO(ix+iy*30*Num_Width)==4 ){
                Low_Block[ix]=iy;                    
            }

            // 타운포탈 목표 지점을 검사한다.
            if(FIELD_INFO(ix+iy*30*Num_Width)>8 && FIELD_INFO(ix+iy*30*Num_Width)<13)
            {
                PortalPosition[Field_Information[ix+iy*30*Num_Width]-9].x=(float)(ix)/20.0f;
                PortalPosition[Field_Information[ix+iy*30*Num_Width]-9].y=(float)(iy)*.05f+.049f;
            }
        }
    }

    // 레이어1 발판 로딩
	LoadFootholdTexture();

    // Stage 초기화 ( 해당 맵정보를 불러옴 )
    m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].Clear();
    RegisterLuabind();
    LoadParticle();

    DWORD dwMapProperty = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty;

    // 프론트 배경 텍스쳐 로딩
	LoadFrontBGTexture();
    
    // 스카이 배경 텍스쳐 로딩
	LoadSkyTexture();

    // 반딧불 카운트 0으로 초기화
    for( int i = 0; i < NUM_TOTAL_STAGE; i++ )
        m_iDustCount[i] = 0;

    // 반딧불
    if( dwMapProperty & MP_SHINING_DUST )
    {
        if( KGCLuabinder::getInstance().DoString("ShiningPosition()", strlen("ShiningPosition()")) == E_FAIL )
        {
            MessageBoxA( NULL, "MapInfo.stg::ShiningPosition() Failed", NULL, MB_OK );
            exit(0);
        }

        for( int i = 0; i < m_iDustCount[SiKGCRoomManager()->GetRoomInfo().ucStage]; i++ )
        {
            for( unsigned char ii = 0; ii < 8; ii++ )
            {
                m_pShiningDust[i].sx[ii]=randf()*.5f + m_vecDustPosition[i].x;
                m_pShiningDust[i].sy[ii]=randf()*.5f + m_vecDustPosition[i].y;
                m_pShiningDust[i].Angle[ii] = 0;
                m_pShiningDust[i].Angle_Speed[ii] = randf()*.04f-.02f;
            }
        }
    }

    if ( dwMapProperty & MP_LAVA )
    {
        // 용암 관련 변수 초기화
        m_bLava = true;
        m_iLava = 0;
        m_fLavaTrans1 = 0.0f;
        m_fLavaTrans2 = 0.0f;
    }

    if ( dwMapProperty & MP_PUSHINGHAND )
    {
        // 더미 손 초기화
        m_vecHand.clear();
        for ( int i = 0; i < DUMMY_HAND_NUM; i++ )
        {
            KHand kHand;
            kHand.iHole     = GetRandomHole();
            kHand.iAlpha    = ::rand() % 256;
            kHand.bInc      = true;
            kHand.bPush     = false;
            m_vecHand.push_back( kHand );
        }

        // 미는 손 초기화
        InitPushHand();
    }

    if ( dwMapProperty & MP_SMOG1 )
    {
        m_fSmogTrans = 0.0f;
    }

    if ( dwMapProperty & MP_SMOG2 )
    {
        m_fSmogTrans = 0.0f;
    }

    if( dwMapProperty & MP_SAND_DROP )
    {
        m_iSandFrame = 0;
        for ( int i = 0; i < sizeof( m_afSandDrop ) / sizeof( float ); i++ )
        {
            m_afSandDrop[i] = 2.0f * randf();
        }

        m_pTexSand = g_pGCDeviceManager2->CreateTexture( "sand.dds" );
    }

    // 맵 화면 스크롤값 초기화
    // 'X-마스의 바벨'에서 사용하는 값들
    m_fScrollY       = 0.0f;
    m_bUpScroll      = true;
    m_bDeathScrollY  = false;

    SiKGCFantasticMap()->LoadMatchMap( Field_Information.as_array(), Num_Width, Num_Height );
    return S_OK;
}

void STAGE::Frame_Move()
{
    PROFILE_SET( "STAGE::Frame_Move" );

    if(Color_Stop_Count) 
        Color_Stop_Count--;
    else
    {
        if(Color_r<251) Color_r+=5;
        if(Color_g<251) Color_g+=5;
        if(Color_b<251) Color_b+=5;
    }

    // Exp 결과를 보여주기 시작할 것....
    if(!g_MyD3D->MyHead->Show_Count && IsDetermineWin)
    {
        if(g_MyD3D->MyHead->Score_Pitch[0]<0)
        {
            for(unsigned char i=0;i<MAX_PLAYER_NUM;i++)
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
                {
                    int Life=(int)g_MyD3D->MyPlayer[i]->m_cLife;
                    if(Life<0)
                        Life=0;
                }
            }

            for(unsigned char i=0;i<MAX_PLAYER_NUM;i++) {
                g_MyD3D->MyHead->Score_Pitch[i]=3.0f;
                g_MyD3D->MyHead->Score_Bright[i]=0;
            }
        }
    }

    DWORD dwColorSky = D3DCOLOR_ARGB( 0, Color_r, Color_g, Color_b );
    unsigned char f_r,f_g,f_b;
    f_r = 51 + (unsigned char)( (float)Color_r * 0.8f );
    f_g = 51 + (unsigned char)( (float)Color_g * 0.8f );
    f_b = 51 + (unsigned char)( (float)Color_b * 0.8f );
    DWORD dwColorNormal = D3DCOLOR_ARGB( 0, f_r, f_g, f_b );

    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        SiKGCLayerManager()->SetColor( dwColorNormal, dwColorSky );
        SiKGCFantasticMap()->FrameMove( GC_ELAPSED_TIME );
        return;
    }
    else
    {
        DWORD dwMapProperty = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty;

        //맵의 뒷배경(하늘)을 움직일 때 사용하는 변수를 변화시킨다.
        //사격 대회장일 경우에는 뒷배경이 좀더 천천히 움직이게 한다.
        if ( MP_SCROLL_NORMAL & dwMapProperty ) Scroll_x -= 0.00025f;
        else if ( MP_SCROLL_FAST & dwMapProperty ) Scroll_x += 0.0036f;
        else if ( MP_SCROLL_SLOW & dwMapProperty )  Scroll_x -= 0.0009f;
        else Scroll_x -= 0.001f;

        if ( Scroll_x < 1.0f ) Scroll_x += 1.0f;

        //반딧불 보여주기
        if( dwMapProperty & MP_SHINING_DUST )
        {
            for( int i=0 ; i<m_iDustCount[SiKGCRoomManager()->GetRoomInfo().ucStage] ; i++)
            {
                for(unsigned char ii=0 ; ii<8 ; ii++)
                {
                    m_pShiningDust[i].Angle[ii] += m_pShiningDust[i].Angle_Speed[ii];
                    if(m_pShiningDust[i].Angle[ii] < 0) 
                        m_pShiningDust[i].Angle[ii] += 6.283f;
                    else if( m_pShiningDust[i].Angle[ii] > 6.283f) 
                        m_pShiningDust[i].Angle[ii] -= 6.283f;

                    m_pShiningDust[i].Angle_Speed[ii] += randf() * 0.002f - 0.001f;
                    if(m_pShiningDust[i].Angle_Speed[ii] > 0.03f || m_pShiningDust[i].Angle_Speed[ii] < -0.03f) 
                        m_pShiningDust[i].Angle_Speed[ii] = 0;
                }
            }
        }

        if( dwMapProperty & MP_STREAM )
        {
            // 물줄기
            Event++;
            if(Event==5 || Event==55 || Event==105 || Event==155 ) g_KDSound.Play( "101" );
            if(Event==1290 && g_kGlobalValue.m_kUserInfo.bHost)
            {        
                NPC TransNPC;
                TransNPC.Init();
                TransNPC.What=2+BEGIN_NPC;
                g_MyD3D->m_NPC.PushNPC_Action(&TransNPC);
            }
            if(Event>1300){
                Event=0;
            }
            if(o5 == 0)
            {            
                if(Event>0 && Event<100)
                {
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, 1.0f, 0.191f,false,0);
                }

                if(Event>50 && Event<150)
                {
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, 1.783f, 0.243f ,false,0);
                }

                if(Event>100 && Event<200)
                {
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, 2.712f, 0.304f ,false,0);
                }

                if(Event>150 && Event<250)
                {
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, 4.08f, 0.232f ,false,0);
                }

                if(Event>200 && Event<300)
                {
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, 5.394f, 0.509f ,false,0);
                }
            }
        }

        if ( MP_AUTOSCROLL & dwMapProperty )
        {
            // 오토스크롤
            m_fScrollY += 0.001f * ( true == m_bUpScroll ? 1.0f : -1.0f );

            // 오토스크롤 방향 전환
            if ( m_fScrollY > 2.9675f || m_fScrollY < 0.0f )
            {
                m_bUpScroll = !m_bUpScroll;
            }

            // 오토스크롤에 의한 플레이어 처리
            int idx = g_MyD3D->Get_MyPlayer();
            m_bDeathScrollY = false;
            if ( true == m_bUpScroll ) // 윗방향 스크롤일 때
            {
                if ( g_kCamera.m_fLookY > m_fScrollY + 0.77f )
                {
                    if ( false == g_MyD3D->MyPlayer[idx]->IsContact )
                    {
                        // 점프를 막는다.
                        if(g_MyD3D->MyPlayer[idx]->y_Speed>0)  
                            g_MyD3D->MyPlayer[idx]->y_Speed = 0;
                    }
                    else
                    {
                        // 달려서 올라간다면 떨어뜨린다.
                        if ( 0.0f != g_MyD3D->MyPlayer[idx]->x_Speed )
                        {
                            m_bDeathScrollY = true;
                        }
                    }
                }
                else if ( g_kCamera.m_fLookY < m_fScrollY - 0.5f )
                {
                    // 아래쪽에 떨어졌을 때
                    m_bDeathScrollY = true;
                }
            }
            else // 아래방향 스크롤일 때
            {
                if ( g_kCamera.m_fLookY > m_fScrollY + 0.77f )
                {
                    if ( false == g_MyD3D->MyPlayer[idx]->IsContact )
                    {
                        // 점프를 막는다.
                        if(g_MyD3D->MyPlayer[idx]->y_Speed > 0)  
                            g_MyD3D->MyPlayer[idx]->y_Speed = 0;
                    }
                    m_bDeathScrollY = true;
                }
                else if ( g_kCamera.m_fLookY < m_fScrollY - 0.5f )
                {
                    // 아래쪽에 떨어졌을 때
                    m_bDeathScrollY = true;
                }
            }

            // 오토스크롤 동기화
            if ( true == g_kGlobalValue.m_kUserInfo.bHost )
            {
                static int cnt = 0;

                if ( (GC_FPS_LIMIT * 3) == ++cnt ) // 3초 한번씩 보낸다. ( 55 fps )
                {
                    cnt = 0;
                    NPC TransNPC;
                    TransNPC.Init();
                    TransNPC.IsRight = m_bUpScroll;
                    TransNPC.y = m_fScrollY;
                    TransNPC.What=2+BEGIN_NPC;
                    g_MyD3D->m_NPC.PushNPC_Action(&TransNPC);
                }
            }
        }

        if( dwMapProperty & MP_LAVA )
        {
            // 용암 밝기를 바꾼다.
            if ( true == m_bLava )
            {
                m_iLava += 3;
                if ( 400 < m_iLava ) m_bLava = false;
            }
            else
            {
                m_iLava -= 3;
                if ( 0 > m_iLava ) m_bLava = true;
            }

            // 용암을 이동시킨다.
            m_fLavaTrans1 += 0.002f;
            if ( m_fLavaTrans1 > 3.0f ) m_fLavaTrans1 -= 3.0f;
            m_fLavaTrans2 -= 0.002f;
            if ( m_fLavaTrans2 < 0 ) m_fLavaTrans2 += 3.0f;
        }

        if ( MP_PUSHINGHAND & dwMapProperty )
        {
            // 더미 손들을 움직인다.
            for ( int i = 0; i < (int)m_vecHand.size(); i++ )
            {
                if ( true == m_vecHand[i].bInc )
                {
                    if ( ( m_vecHand[i].iAlpha += 3 ) > 255 )
                    {
                        m_vecHand[i].iAlpha = 255;
                        m_vecHand[i].bInc   = false;
                    }
                }
                else
                {
                    if ( ( m_vecHand[i].iAlpha -= 3 ) < 0 )
                    {
                        m_vecHand[i].iHole  = GetRandomHole();
                        m_vecHand[i].iAlpha = 0;
                        m_vecHand[i].bInc   = true;
                    }
                }
            }

            // 미는 손을 움직인다.
            for ( int i = 0; i < (int)m_vecPushHand.size(); i++ )
            {
                if ( true == m_vecPushHand[i].bPush )
                {
                    if ( true == m_vecPushHand[i].bInc )
                    {
                        if ( ( m_vecPushHand[i].iAlpha += 3 ) > 255 )
                        {
                            m_vecPushHand[i].iAlpha += 25;

                            if ( m_vecPushHand[i].iAlpha < 600 )
                            {
                                // 충돌을 체크한다.
                                CheckHandCollision( i );
                            }
                            else
                            {
                                m_vecPushHand[i].iAlpha  = 600;
                                m_vecPushHand[i].bInc    = false;
                            }
                        }
                    }
                    else
                    {
                        if ( m_vecPushHand[i].iAlpha > 255 )
                        {
                            m_vecPushHand[i].iAlpha -= 25;
                            if ( m_vecPushHand[i].iAlpha < 255 ) m_vecPushHand[i].iAlpha = 255;
                        }

                        if ( ( m_vecPushHand[i].iAlpha -= 3 ) < 0 ) ResetPushHand( i );
                    }
                }
            }
        }

        if ( dwMapProperty & MP_SMOG1 )
        {
            m_fSmogTrans += 0.005f;

            if ( m_fSmogTrans > 2.0f ) m_fSmogTrans -= 2.0f;
        }

        if ( dwMapProperty & MP_SMOG2 )
        {
            m_fSmogTrans += 0.0025f;

            if ( m_fSmogTrans > 2.0f ) m_fSmogTrans -= 2.0f;
        }

        if( dwMapProperty &  MP_SAND_DROP )
        {
            static int cnt = 0;

            if ( 6 == ++cnt ) cnt = 0;

            if ( 0 == cnt )
            {
                if ( 4 == ++m_iSandFrame ) m_iSandFrame = 0;
            }

            for ( int i = 0; i < sizeof( m_afSandDrop ) / sizeof( float ); i++ )
            {
                m_afSandDrop[i] += 0.002f;
                if ( m_afSandDrop[i] > 1.0f )
                {
                    if ( 0 == ::rand() % 2 ) m_afSandDrop[i] += 0.002f;
                }
                if ( m_afSandDrop[i] > 2.0f )
                {
                    m_afSandDrop[i] = 0.0f;
                }
            }
        }

        for( int iter = 0; iter < (int)m_vecMapStreamData.size(); ++iter )
        {
            MapStreamData& m_MapStreamData = m_vecMapStreamData[iter];

            if( m_MapStreamData.iEvent != -1 )
            {
                m_MapStreamData.iEvent++;
                for( int i = 0; i < (int)m_MapStreamData.vecStreamSoundEvent.size(); ++i)
                {
                    if( m_MapStreamData.iEvent == m_MapStreamData.vecStreamSoundEvent[i] )
                        g_KDSound.Play( "101" );
                }
                if( m_MapStreamData.iEvent == m_MapStreamData.iSyncTiming && g_kGlobalValue.m_kUserInfo.bHost )
                {
                    NPC TransNPC;
                    TransNPC.Init();
                    TransNPC.What=3+BEGIN_NPC+iter;
                    g_MyD3D->m_NPC.PushNPC_Action(&TransNPC);
                }
                else if( m_MapStreamData.iEvent > m_MapStreamData.iEventInitTime )
                    m_MapStreamData.iEvent = 0;

                if( o5 == 0 )
                {
                    for( int i = 0; i < (int)m_MapStreamData.vecSD.size(); ++i )
                    {
                        StreamData& sd = m_MapStreamData.vecSD[i];

                        if( m_MapStreamData.iEvent > sd.iEventStart && m_MapStreamData.iEvent < sd.iEventEnd )
                        {
                            CDamageInstance* pDamage = g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM, sd.fX, sd.fY ,false,0);
                            if( pDamage != NULL )
                            {
                                pDamage->m_Life = sd.iLife;
                                pDamage->m_pOriginalDamage->m_Start_Life = sd.iLife;
                                pDamage->m_pOriginalDamage->m_iCrashStart = sd.iLife;
                                pDamage->m_pOriginalDamage->m_fDMYSpeed = sd.fYSpeed;
                                pDamage->m_pOriginalDamage->m_iJumpPossibleFrame = sd.iJumpFrame;
                            }
                        }
                    }
                }
            }
        }

        for( int i = 0; i < (int)m_vecAS.size(); ++i )
        {
            AmbientSound& AS = m_vecAS[i];

            if( AS.iEventInitTime != -1 )
            {
                AS.iEvent++;

                if( AS.iEvent == AS.iEventInitTime )
                {
                    int iRate = (int)( rand() / ( RAND_MAX / (double)100 ) );
                    std::stringstream str;
                    str << AS.iSoundID;
                    if( iRate < AS.iRate )
                        g_KDSound.Play( str.str().c_str() );

                    AS.iEvent = 0;
                }
            }
        }
    }
}

void STAGE::RenderSky()
{
    if (SiKGCRoomManager()->IsMonsterGameMode())
        return;

    DWORD dwMapProperty = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty;

    if (SiKGCRoomManager()->GetRoomInfo().ucStage < 0)
        return;

    static bool s_bSendErr = false;
    if (m_pTexSky == NULL)
    {
        if (s_bSendErr == false)
        {
            s_bSendErr = true;
        }
        return;
    }

    D3DXMATRIX  mat1;

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

        if (g_MyD3D->m_KGCOption.Is1Back())
        {

            D3DXMatrixTranslation(&mat1, 0, 0, 3.0f);
            //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);

            if (Color_Stop_Count < 51)
            {
                if ((dwMapProperty & MP_SCROLL_SLOW)
                    || (dwMapProperty & MP_SCROLL_NORMAL)
                    || (dwMapProperty & MP_SCROLL_FAST))
                {
                    V_BackGround[0].tu = V_BackGround[2].tu = Scroll_x;
                    V_BackGround[1].tu = V_BackGround[3].tu = Scroll_x + 1.0f;
                }
                V_BackGround[0].Diff = D3DCOLOR_ARGB(255, Color_r, Color_g, Color_b);
                V_BackGround[1].Diff = D3DCOLOR_ARGB(255, Color_r, Color_g, Color_b);
                V_BackGround[2].Diff = D3DCOLOR_ARGB(255, Color_r, Color_g, Color_b);
                V_BackGround[3].Diff = D3DCOLOR_ARGB(255, Color_r, Color_g, Color_b);
                //V_BackGround->Unlock();
            }

            //g_pd3dDevice->SetFVF( FVF_LVERTEX );
            //g_pd3dDevice->SetStreamSource( 0, V_BackGround->GetBuffer(), 0, sizeof(SLVERTEX) );
            //if(g_MyD3D->m_pStateMachine->GetState() != GS_ROOM) 
              //  m_pTexSky->SetDeviceTexture();            

            // ¹è°æ ±×¸®±â 
            g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
            g_pGCDeviceManager2->DrawInWorld(m_pTexSky, &mat1, V_BackGround);
        }
    }
    g_pGCDeviceManager2->PopState();

    D3DXMatrixIdentity(&mat1);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
}

void STAGE::RenderBack1()
{
    if (SiKGCRoomManager()->IsMonsterGameMode())
        return;

    if (SiKGCRoomManager()->GetRoomInfo().ucStage < 0)
        return;

    D3DXMATRIX  mat1, mat2, mat3;

    g_pd3dDevice->SetFVF(FVF_LVERTEX);

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, (DWORD)g_MyD3D->m_KGCOption.IsShadow());
        g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//(DWORD)g_MyD3D->m_KGCOption.IsShadow() );


        g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

        g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);


        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
            g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

            D3DXVECTOR3 point;
            float radius;

            if (Color_Stop_Count < 51)
            {
                //SLVERTEX *v = NULL;
                //v = (SLVERTEX*)V_Level_Back->Lock( (BYTE*)v );
                //{
                unsigned char f_r, f_g, f_b;
                f_r = 50 + (unsigned char)((float)Color_r / 255.0f * 205.0f);
                f_g = 50 + (unsigned char)((float)Color_g / 255.0f * 205.0f);
                f_b = 50 + (unsigned char)((float)Color_b / 255.0f * 205.0f);
                V_Level_Back[0].Diff = D3DCOLOR_ARGB(255, f_r, f_g, f_b);
                V_Level_Back[1].Diff = D3DCOLOR_ARGB(255, f_r, f_g, f_b);
                V_Level_Back[2].Diff = D3DCOLOR_ARGB(255, f_r, f_g, f_b);
                V_Level_Back[3].Diff = D3DCOLOR_ARGB(255, f_r, f_g, f_b);
                //}
                //V_Level_Back->Unlock();
            }

            //g_pd3dDevice->SetStreamSource( 0, V_Level_Back->GetBuffer(), 0, sizeof(SLVERTEX) );

            g_MyD3D->m_pFrustum->Construct(1000.0f);
            char Texture_Count = -1;
            for (int iy = 0; iy < Num_Height; iy++)
            {
                for (int ix = 0; ix < Num_Width; ix++)
                {
                    ++Texture_Count;
                    if (m_pTexLevel1[Texture_Count] == NULL)
                        continue;

                    D3DXMatrixTranslation(&mat2, (-1.0f + 1.5f * (float)ix), (0.26f + 1.5f * (float)iy), 2.0f);
                    D3DXMatrixMultiply(&mat1, &mat2, &g_kCamera.m_matCamera);

                    point.x = mat1._41 + 1.25f;
                    point.y = mat1._42;
                    point.z = 0.5f;
                    radius = 1.25f;
                    if (g_MyD3D->m_pFrustum->CheckSphere(point, radius) == true)
                    {
                        //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
//						m_pTexLevel1[Texture_Count]->SetDeviceTexture();						
                        g_pGCDeviceManager2->DrawInWorld(m_pTexLevel1[Texture_Count], &mat1, V_Level_Back);
                    }
                }
            }
        }
        g_pGCDeviceManager2->PopState();

        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

            if (m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty & MP_PUSHINGHAND)
            {
                //				SetTexture( MP_PUSHINGHAND );

                g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
                g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
                g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

                // ´õ¹Ì ¼ÕµéÀ» ±×¸°´Ù.
                for (int i = 0; i < (int)m_vecHand.size(); i++)
                {
                    ::D3DXMatrixTranslation(&mat1, gs_vHole[m_vecHand[i].iHole].x,
                        gs_vHole[m_vecHand[i].iHole].y, 0.8f);
                    ::D3DXMatrixMultiply(&mat1, &mat1, &g_kCamera.m_matCamera);
                    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
                    BYTE clr = static_cast<BYTE>(m_vecHand[i].iAlpha);
                    float fSize = 0.125f + 0.1f * (m_vecHand[i].iAlpha / 255.0f);
                    //g_MyD3D->Draw_Billboard( fSize, fSize, 0.01f, 0.01f, 0.49f, 0.49f, clr, clr, clr, clr );
                    g_pGCDeviceManager2->DrawInWorld(GetTexture(MP_PUSHINGHAND), &mat1, fSize, fSize, 0.01f, 0.01f, 0.49f, 0.49f, 0, 0, 0, D3DCOLOR_ARGB(clr, clr, clr, clr));
                }

                // ¹Ì´Â ¼ÕÀ» ±×¸°´Ù.
                for (int i = 0; i < (int)m_vecPushHand.size(); i++)
                {
                    ::D3DXMatrixTranslation(&mat1, gs_vHole[m_vecPushHand[i].iHole].x,
                        gs_vHole[m_vecPushHand[i].iHole].y, 0.8f);
                    ::D3DXMatrixMultiply(&mat1, &mat1, &g_kCamera.m_matCamera);
                    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1 );
                    BYTE clr = static_cast<BYTE>(255 < m_vecPushHand[i].iAlpha ? 255 : m_vecPushHand[i].iAlpha);
                    float fSize;
                    if (m_vecPushHand[i].iAlpha < 256)
                    {
                        fSize = 0.125f + 0.1f * (m_vecPushHand[i].iAlpha / 255.0f);
                    }
                    else
                    {
                        fSize = 0.125f + 0.2f * ((400 < m_vecPushHand[i].iAlpha ? 400 : m_vecPushHand[i].iAlpha) / 255.0f);
                    }
                    g_pGCDeviceManager2->DrawInWorld(GetTexture(MP_PUSHINGHAND), &mat1, fSize, fSize, 0.01f, 0.01f, 0.49f, 0.49f, 0, 0, 0, D3DCOLOR_ARGB(clr, clr, clr, clr));
                    //g_MyD3D->Draw_Billboard( fSize, fSize, 0.01f, 0.01f, 0.49f, 0.49f, clr, clr, clr, clr );
                }
            }
        }
        g_pGCDeviceManager2->PopState();
    }
    g_pGCDeviceManager2->PopState();

    D3DXMatrixIdentity(&mat1);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
}

GCDeviceTexture* STAGE::GetTexture( DWORD dwflag_ )
{
    DWORD dwMapProperty = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty;

    for( int i = 0; i < (int)m_vecTexture.size(); i++ )
    {
        if( m_pdwTexStageNum[i] == dwflag_ )
        {
            m_pTexFront = m_vecTexture[i];

            if ( m_pTexFront != NULL )
            {
				return m_pTexFront;
            }
            break;
        }
    }
	return NULL;
}

void STAGE::FrontMap_Render()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
        return;

    DWORD dwMapProperty = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty;

    if ( !g_MyD3D->m_KGCOption.Is3Back() ) 
        return;

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE, FALSE );

        if ( dwMapProperty & MP_SHINING_DUST )
        {
            ON_RENDER_SHINING_DUST(GetTexture( MP_SHINING_DUST ));
        }

        if ( dwMapProperty & MP_LAVA )
        {
            ON_RENDER_LAVA(GetTexture( MP_LAVA ));
        }
        
        if ( dwMapProperty & MP_SMOG1 )
        {
            ON_RENDER_SMOG(GetTexture( MP_SMOG1 ));
        }

        if ( dwMapProperty & MP_SMOG2 )
        {
            ON_RENDER_SMOG(GetTexture( MP_SMOG2 ));
        }

        if ( dwMapProperty & MP_SAND_DROP )
        {
            ON_RENDER_SAND_DROP(GetTexture( MP_SAND_DROP ));
        }
    }
    g_pGCDeviceManager2->PopState();
}

void STAGE::RenderMap()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
        return;

#if defined( _DEBUG ) || !defined( __PATH__ )
    {
        if( g_kGlobalValue.m_bRenderFootHold )
        {
            g_pGCDeviceManager2->PushState();
            {
                g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

                g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );             


                for( int i = 0; i < Num_Width * Num_Height * 900; ++i )
                {
                    if( Field_Information[i] != 0 )
                    {
                        int iX = i % ( 30 * Num_Width );
                        int iY = i / ( 30 * Num_Width );

                        float fX = (float)iX / 20.0f;
                        float fY = (float)iY / 20.0f;

                        fX -= 1.0f;
                        fY -= 0.5f;

                        GCCollisionRect<float> fRect;
                        fRect.m_Left = fX;
                        fRect.m_Top = fY + 0.05f;
                        fRect.m_Right = fX + 0.05f;
                        fRect.m_Bottom = fY;

                        D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                        SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top, 0.4f );
                        SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top, 0.4f );
                        SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom, 0.4f );
                        SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom, 0.4f );

                        // 2012.04.20, shmhlove : 아.,.. 블럭보기 졸라 힘들ㄴ빱?테두리 시커멓게 그려서 블럭을 보여줄래!!
                        D3DXVECTOR3 Exlefttop, Exrighttop, Exleftbottom, Exrightbottom;
                        Exlefttop=lefttop;              Exlefttop.x +=0.005f; Exlefttop.y -=0.005f;
                        Exrighttop=righttop;            Exrighttop.x -=0.005f; Exrighttop.y -=0.005f;
                        Exleftbottom=leftbottom;        Exleftbottom.x +=0.005f; Exleftbottom.y +=0.005f;
                        Exrightbottom=rightbottom;      Exrightbottom.x -=0.005f; Exrightbottom.y +=0.005f;
                        g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                            lefttop,  righttop, leftbottom, rightbottom, 0,0,0,D3DCOLOR_ARGB(200,0, 0, 0 ));

                        switch( Field_Information[i] )
                        {
                        case 1:
							g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
								Exlefttop,  Exrighttop, Exleftbottom, Exrightbottom, 0,0,0,D3DCOLOR_ARGB(200,0, 0, 0 ));
                            //g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 0, 0, 0 );
                            break;
                        case 2:
							g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
								Exlefttop,  Exrighttop, Exleftbottom, Exrightbottom, 0,0,0,D3DCOLOR_ARGB(200,0, 255, 0 ));
                            //g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 0, 255, 0 );
                            break;
                        case 3:
							g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
								Exlefttop,  Exrighttop, Exleftbottom, Exrightbottom, 0,0,0,D3DCOLOR_ARGB(200,0, 0, 255 ));
                            //g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 0, 0, 255 );
                            break;
                        default:
							g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
								Exlefttop,  Exrighttop, Exleftbottom, Exrightbottom, 0,0,0,D3DCOLOR_ARGB(200,255, 0, 0 ));
                            //g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 255, 0, 0 );
                            break;
                        }
                    }
                }
            }
            g_pGCDeviceManager2->PopState();
        }
    }
#endif
}

// 플레이어가 아래키를 눌러서 떨어질 수 있는가?
bool STAGE::IsEnableNoCheck(float x, float y)
{
    if( SiKGCRoomManager()->IsMonsterGameMode() == false )
    {
        if( IsLowStandBlock(x, y) == true )
            return false;

        short i_X,i_Y;
        i_X=(unsigned char)(x*20.0f);
        i_Y=(unsigned char)(y*20.0f);
        if(LOW_STAND_INFO(i_X)+1>i_Y) return false;
        return true;
    }
    else
    {
        return SiKGCFantasticMap()->IsAvailableGoDown( x, y );
    }
}

// 플레이어가 점프했을 때 올라갈 수 있는가?
bool STAGE::IsEnableNoJump( float x, float y )
{
    return SiKGCFantasticMap()->IsAvailableJumpUp( x, y );    
}

bool STAGE::IsDeadBlock(float x,float y)
{
	return SiKGCFantasticMap()->IsDeadBlock(x,y);

}

float STAGE::Get_LowHeight(float x, float y)
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        return SiKGCFantasticMap()->FindShadowPos( x, y );
    }
    else
    {
        short Index,Com_y=-1,i_X,i_Y,XPerLine=30*Num_Width;
        float Interpolation;    

        i_X=(int)(x*20.0f);
        i_Y=(int)(y*20.0f);


        if(i_X<0) i_X=0;
        else if(i_X>Num_Width*30-1) i_X=Num_Width*30-1;
        if(i_Y>Num_Height*30-1) i_Y=Num_Height*30-1;

        // i_Y값 찾아내기 
        while(1){
            // 제일 낮은 지형보다도 밑에 있다면 강제로 올린다.
            if(i_Y<LOW_STAND_INFO(i_X)){
                /*
                i_Y=Low_Stand_Block[i_X];
                break;
                */
                return -.1f;
            }
            Index=i_X+i_Y*XPerLine;
            if(FIELD_INFO(Index)==1) break;
            i_Y--;
        }

        // 디디기.............................
        // 블럭의 반을 안 넘었음
        if((float)((short)(x*20.0f))/20.0f+.025f>x)
        {
            if(i_X>0)
            {
                Interpolation=(float)((short)(x*20.0f))/20.0f+.025f-x;
                if(i_Y<Num_Height*30-1)
                    if(FIELD_INFO(i_X-1+(i_Y+1)*XPerLine)==1) Com_y=i_Y+1;
                if(Com_y==-1 && i_Y>0) 
                    if(FIELD_INFO(i_X-1+(i_Y-1)*XPerLine)==1) Com_y=i_Y-1;
            }
            // 블럭의 반을 넘었음.
        }else{
            if(i_X<XPerLine-1)
            {
                Interpolation=x-(float)((short)(x*20.0f))/20.0f-.025f;
                if(i_Y<Num_Height*30-1)
                    if(FIELD_INFO(i_X+1+(i_Y+1)*XPerLine)==1) Com_y=i_Y+1;
                if(Com_y==-1 && i_Y>0) 
                    if(FIELD_INFO(i_X+1+(i_Y-1)*XPerLine)==1) Com_y=i_Y-1;
            }
        }

        if(Com_y==-1) return (float)(i_Y)*.05f+.049f;
        else return ((float)i_Y*.05f+(float)(Com_y-i_Y)*.05f*Interpolation*20.0f)+.049f;
    }
}


// 아이템이 떨어질 위치나 알이 이동할 랜덤 위치를 리턴한다.
D3DXVECTOR2 STAGE::Get_Random_XY( bool CheckSafePoint )
{
    D3DXVECTOR2 Vec(0.0f, 0.0f);
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        if( CheckSafePoint )
        {
            // 안전한 지대 검사를 할경우 최저지대가 선택된다.
            Vec = Check_SafePostionX( SiKGCFantasticMap()->GetRandomPosX(), SiKGCFantasticMap()->GetRandomPosY() );
        }
        else
        {
            Vec.x = SiKGCFantasticMap()->GetRandomPosX();
            Vec.y = SiKGCFantasticMap()->GetRandomPosY();
        }
    }
    else
    {
        if( CheckSafePoint )
        {
            // 안전한 지대 검사를 할경우 최저지대가 선택된다.
            Vec = Check_SafePostionX( randf() * 1.5f * (float) Num_Width );
        }
        else
        {
            D3DXVECTOR3 Vec2;
            bool        Continue = true;

            while( Continue )
            {
                //바운드를 주지 않으면 완전 랜덤
                //바운드란 X좌표를 말하는 것 같다.
                Vec.x = randf() * 1.5f * (float)Num_Width;
                //랜덤하게 높이를 결정한 후
                Vec.y = randf() * 1.5f;

                //그 높이의 바닥을 찾는다
                while(true)
                {
                    Vec2 = Check_Contact( Vec.x, Vec.y, 0 );
                    if( Vec2.y == -1.0f )
                    {
                        Vec.y -= 0.05f;
                        if( Vec.y < 0 ) 
                            break;

                        continue;
                    }
                    else
                    {
                        Vec.y = Vec2.y;
                        Continue = false;
                        break;
                    }
                }
            }
        }
    }
    return Vec;
}

D3DXVECTOR2 STAGE::Check_SafePostionX( float x, float y/* = 0.0f*/ )
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        return SiKGCFantasticMap()->GetPositionNear( x, y );
    }
    else
    {
        // 처음 시작 위치나 아레나 시 떨어질 위치를 구할 때 
        // 안전한 디딤발이 있는 X 좌표인지 검사한다.
        // 현재 위치를 포함한 가장 가까운 안전지대의 xy좌표를 출력한다.
        bool IsLSide = true;
        float R = 0.05f;
        float L = 0.0f;

        int iStart;
        int iPlus;

        iStart = 0;
        iPlus = 1;

        while ( 1 )
        {
            int i_X;

            if ( IsLSide )
            {
                i_X = static_cast<int>( ( x + L ) * 20.0f );

                L -= 0.05f;

                if ( static_cast<int>( ( x + R ) * 20.0f ) < 30 * Num_Width )
                {
                    IsLSide = false;
                }
                else
                {
                    if ( static_cast<int>( ( x + L ) * 20.0f ) < 0.0f )
                    {
                        return D3DXVECTOR2( 0.0f, 0.0f ); // 이 경우는 사실상 에러임
                    }
                }
            }
            else
            {
                i_X = static_cast<int>( ( x + R ) * 20.0f );

                R += 0.05f;

                if ( static_cast<int>( ( x + L ) * 20.0f ) > -1 )
                {
                    IsLSide = true;
                }
                else
                {
                    if ( static_cast<int>( ( x + R ) * 20.0f ) > 30 * Num_Width - 1 )
                    {
                        return D3DXVECTOR2( 0.0f, 0.0f ); // 이 경우는 사실상 에러임
                    }
                }
            }

            int h = iStart;
            bool bNotFirst = false;
            while ( h > -1 && h < 30 * Num_Height )
            {
                if ( bNotFirst ) h += iPlus;

                bNotFirst = true;

                // 디딜 수 있는 것을 찾았다.
                if ( FIELD_INFO( i_X + h * 30 * Num_Width ) == 1 )
                {
                    // 디딜 수 있는 것을 주위로 양쪽에 적어도 각각 1개씩의 디딤발이 있어야한다.
                    // 높이차는 +-2씩이다.
                    // 왼쪽부터 찾자
                    bool LFound = false;
                    bool RFound = false;

                    if ( i_X > 0 )
                    {
                        for ( int h2 = h - 2; h2 < h + 3; h2++ )
                        {
                            if ( h2 > -1 && h2 < 30 * Num_Height )
                            {
                                // 찾았다.
                                if ( FIELD_INFO( i_X - 1 + h2 * 30 * Num_Width ) == 1 )
                                {
                                    LFound = true;
                                    break;
                                }
                            }
                        }
                    }

                    if ( i_X < 30 * Num_Width - 1 )
                    {
                        for ( int h2 = h - 1; h2 < h + 3; h2++ )
                        {
                            if ( h2 > -1 && h2 < 30 * Num_Height )
                            {
                                // 찾았다.
                                if ( FIELD_INFO( i_X + 1 + h2 * 30 * Num_Width ) == 1 )
                                {
                                    RFound = true;
                                    break;
                                }
                            }
                        }
                    }

                    if ( LFound && RFound )
                    {
                        return D3DXVECTOR2( (float)i_X / 20.0f, (float)h / 20.0f );
                    }
                }
            }
        }

        return D3DXVECTOR2( 0.0f, 0.0f );
    }
}

bool STAGE::IsExistNextBlock( const KNextBlockCheck& kNextCheck )
{
    return SiKGCFantasticMap()->IsExistNextBlock( kNextCheck );
}

bool STAGE::IsLowStandBlock(float x, float y)
{
    int i_X             = (int)(x * 20.0f);
    int i_Y             = (int)(y * 20.0f);

    if( Get_Low_Stand_Block(i_X) + 2 > i_Y && Get_Low_Stand_Block(i_X) - 2 < i_Y )
        return true;
    return false;
}

bool STAGE::IsInvBlock(float x, float y)
{
    int i_X = (int)(x*20.0f);
    int i_Y = (int)(y*20.0f);

    int XPerLine = 30 * Num_Width;
    int Index = i_X + i_Y * XPerLine;

    if( Get_Field_Information(Index, true) == 13 )
        return true;
    else
        return false;
}

bool STAGE::IsDoorUnlockBlock( float x, float y )
{
    int i_X = (int)(x*20.0f);
    int i_Y = (int)(y*20.0f);

    int XPerLine = 30 * Num_Width;
    int Index = i_X + i_Y * XPerLine;

    if( Get_Field_Information(Index, true) == 14 )
        return true;
    else
        return false;
}

bool STAGE::Check_Contact( KGCContectInfo& kInfo_ )
{
    return SiKGCFantasticMap()->AdjustUnitPosition( kInfo_ );
}

D3DXVECTOR3 STAGE::Check_Contact(float x, float y, unsigned char NoCheck, bool bTagMotion/* = false*/, int* pContactX, int* pContactY, bool bNotCheckSideFH )
{
    D3DXVECTOR3 Vec;
    Vec.x           = 0;
    Vec.y           = -1.0f;
    Vec.z           = 0;
    int Index,Com_y = -1, i_X, i_Y, XPerLine = 30 * Num_Width;
    float Interpolation;    
    //if(y>=1.5f) y=1.49f;

    i_X             = (int)(x * 20.0f);
    i_Y             = (int)(y * 20.0f);
    Index           = i_X + i_Y * XPerLine;
    if( Index > XPerLine * 30 * Num_Height - 1 )
        return Vec;

    ///*
    /// 좌우 막아내기
    if( false == bNotCheckSideFH )
    {
        if( i_Y < Num_Height * 30 - 1 )
        {
            if( i_X > 0 )
            {
                Index = i_X - 1 + ( i_Y + 1 ) * XPerLine;
                if(FIELD_INFO(Index)==3)    
                {
                    Vec.x=(float)(i_X)/20.0f;
                    Vec.z=3.0f;
                    x=Vec.x+.025f;
                    i_X=(int)(x*20.0f);
                }
                else if(FIELD_INFO(Index)>4 && FIELD_INFO(Index)<9)
                {
                    Vec.z=(float)Field_Information[Index];
                }
            }

            if(i_X<XPerLine-1)
            {
                Index=i_X+1+(i_Y+1)*XPerLine;
                if(FIELD_INFO(Index)==2)
                {
                    Vec.x=(float)(i_X)/20.0f;
                    Vec.z=2.0f;
                    x=Vec.x+.025f;
                    i_X=(int)(x*20.0f);
                }       
            }

            Index=i_X+(i_Y+1)*XPerLine;
            if(FIELD_INFO(Index)==3)    
            {
                Vec.x=(float)(i_X+1)/20.0f;
                Vec.z=3.0f;
                x=Vec.x+.025f;
                i_X=(int)(x*20.0f);
            }
            else if(FIELD_INFO(Index)==2)   
            {
                Vec.x=(float)(i_X-1)/20.0f;
                Vec.z=2.0f;
                x=Vec.x+.025f;
                i_X=(int)(x*20.0f);         
            }
            else if(FIELD_INFO(Index)>4 && FIELD_INFO(Index)<9) 
            {
                Vec.z=(float)FIELD_INFO(Index);
            }
        }
    }

    /**/

    if( !bTagMotion && LOW_STAND_INFO(i_X)-1>i_Y) 
    {        
        if(g_MyD3D->m_pStateMachine->GetState() != GS_GAME || LOW_INFO(i_X)==-1) 
            i_Y = LOW_STAND_INFO(i_X);
        else
        {
            // 게임 중일 때...

            Vec.y=-1.0f;
            return Vec;
        }
    }

    if( !bTagMotion && Get_Low_Stand_Block(i_X) + 2 > i_Y && Get_Low_Stand_Block(i_X) - 2 < i_Y ) 
        NoCheck = 0;

    if( !bTagMotion )
    {
        Index = i_X + i_Y * XPerLine;

        if(i_Y<Num_Height*30-1)
        {
            if(FIELD_INFO(Index)!=1 && FIELD_INFO(i_X+(i_Y+1)*XPerLine)==1) 
            {
                i_Y++;
                Index=i_X+i_Y*XPerLine;
            }
        }

        if(i_Y>0)
        {
            if(FIELD_INFO(Index)!=1 && FIELD_INFO(i_X+(i_Y-1)*XPerLine)==1) 
            {
                i_Y--;
                Index=i_X+i_Y*XPerLine;
            }
        }

        // 디디기.............................
        // 블럭의 반을 안 넘었음
        if((float)((short)(x*20.0f))/20.0f+.025f>x)
        {
            if(i_X>0)
            {
                Interpolation=(float)((int)(x*20.0f))/20.0f+.025f-x;
                if(i_Y<Num_Height*30-1)
                    if(FIELD_INFO(i_X-1+(i_Y+1)*XPerLine)==1) 
                        Com_y=i_Y+1;
                if(Com_y==-1 && i_Y>0) 
                    if(FIELD_INFO(i_X-1+(i_Y-1)*XPerLine)==1) 
                        Com_y=i_Y-1;
            }
            // 블럭의 반을 넘었음.
        }
        else
        {
            if(i_X<XPerLine-1)
            {
                Interpolation=x-(float)((int)(x*20.0f))/20.0f-.025f;
                if(i_Y<Num_Height*30-1)
                    if(FIELD_INFO(i_X+1+(i_Y+1)*XPerLine)==1) Com_y=i_Y+1;
                if(Com_y==-1 && i_Y>0) 
                    if(FIELD_INFO(i_X+1+(i_Y-1)*XPerLine)==1) Com_y=i_Y-1;
            }
        }   

        if(FIELD_INFO(Index)==1)
        {
            if(Com_y==-1) Vec.y=(float)(i_Y)*.05f+.049f;
            else
            {
                //Vec.y=((float)i_Y*.05f*(.025f-Interpolation)+(float)Com_y*.05f*Interpolation)/.025f+.049f;
                Vec.y=((float)i_Y*.05f+(float)(Com_y-i_Y)*.05f*Interpolation*20.0f)+.049f;
            }
        }

        if(NoCheck) 
            Vec.y=-1.0f;
    }

    if( pContactX != NULL )
        *pContactX = i_X;
    if( pContactY != NULL )
        *pContactY = i_Y;

    return Vec;
}

// ------------------------------------------------------------------------------------------
// 스테이지6(불의 사원)과 관련된 멤버 함수
// ------------------------------------------------------------------------------------------

/// 미는 손과 더미 손의 중복을 제거한다.
/// @param void
/// @return void
void STAGE::RemoveDuplicateHole( void )
{
    std::set<int>   setPushHandHole;

    for ( int i = 0; i < (int)m_vecPushHand.size(); i++ )
    {
        setPushHandHole.insert( m_vecPushHand[i].iHole );
    }

	int i;
    for ( i = 0; i < (int)m_vecHand.size(); i++ )
    {
        if ( setPushHandHole.find( m_vecHand[i].iHole ) != setPushHandHole.end() )
		{
			break;
    }
    }

    if ( i != m_vecHand.size() )
    {
        m_vecHand[i].iHole  = GetRandomHole();
        m_vecHand[i].iAlpha = 0;
        m_vecHand[i].bInc   = true;
        m_vecHand[i].bPush  = false;
    }
}

/// 중복이 되지 않게 랜덤하게 구멍을 골라낸다.
/// @param void
/// @return 선택된 구멍의 인덱스
int STAGE::GetRandomHole( void )
{
    std::set<int>   setAlreadyExist;

	int i;
    for ( i = 0; i < (int)m_vecHand.size(); i++ )
    {
        setAlreadyExist.insert( m_vecHand[i].iHole );
    }

    for ( i = 0; i < (int)m_vecPushHand.size(); i++ )
    {
        setAlreadyExist.insert( m_vecPushHand[i].iHole );
    }

    do
    {
        i = ::rand() % HOLE_NUM;
    } while ( setAlreadyExist.find( i ) != setAlreadyExist.end() );

    return i;
}

/// 밀어내는 손을 초기화한다.
/// @param void
/// @return void
void STAGE::InitPushHand( void )
{
    m_vecPushHand.clear();
    for ( int i = 0; i < PUSH_HAND_NUM; i++ )
    {
        m_vecPushHand.push_back( KHand() );
    }

    for ( int i = 0; i < (int)m_vecPushHand.size(); i++ )
    {
        ResetPushHand( i, true );
    }
}

/// 밀어내는 손을 리셋한다.
/// @param iIdx 밀어내는 손의 인덱스
/// @param bInit 초기화될 때인가?
/// @return void
void STAGE::ResetPushHand( int iIdx, bool bInit )
{
    // 미는 손 초기화
    m_vecPushHand[iIdx].iHole   = 0;
    m_vecPushHand[iIdx].iAlpha  = ( ( true == g_kGlobalValue.m_kUserInfo.bHost && true == bInit ) ? ::rand() % 256 : 0 );
    m_vecPushHand[iIdx].bInc    = true;
    m_vecPushHand[iIdx].bPush   = false;

    // 방장일 경우 미는 손에 대한 정보를 보낸다.
    if ( true == g_kGlobalValue.m_kUserInfo.bHost )
    {
        m_vecPushHand[iIdx].iHole = GetRandomHole();
        m_vecPushHand[iIdx].bPush = true;

        NPC kTransNPC;
        kTransNPC.Init();
        kTransNPC.x         = (float)iIdx;
        kTransNPC.Action    = m_vecPushHand[iIdx].iHole;
        kTransNPC.HP        = (float)m_vecPushHand[iIdx].iAlpha;
        kTransNPC.IsRight   = m_vecPushHand[iIdx].bInc;
        kTransNPC.What      = 2 + BEGIN_NPC;
        g_MyD3D->m_NPC.PushNPC_Action( &kTransNPC );
    }
}

/// 손과의 충돌을 검사한다.
/// @param 밀어내는 손의 인덱스
/// @return void
void STAGE::CheckHandCollision( int iIdx )
{
    int iMyIdx;

    if ( 7 != ( iMyIdx = g_MyD3D->Get_MyPlayer() ) &&
        21 != g_MyD3D->MyPlayer[iMyIdx]->uiMotion &&
        0 == g_MyD3D->MyPlayer[iMyIdx]->Super )
    {
        D3DXVECTOR2 vMyPos( g_MyD3D->MyPlayer[iMyIdx]->vPos.x - 1.0f,
            g_MyD3D->MyPlayer[iMyIdx]->vPos.y - 0.45f );
        D3DXVECTOR2 vDiff = vMyPos - gs_vHole[m_vecPushHand[iIdx].iHole];

        if ( 0.2f > ::D3DXVec2Length( &vDiff ) )
        {
            g_MyD3D->MySparks->LightBim( g_MyD3D->MyPlayer[iMyIdx]->vPos.x - 1.0f,
                g_MyD3D->MyPlayer[iMyIdx]->vPos.y, 0 );
            g_MyD3D->MyPlayer[iMyIdx]->Direct_Motion_Input( 10 );
            g_MyD3D->MyPlayer[iMyIdx]->Ah_Sound( 0 );

            if ( true == g_MyD3D->MyPlayer[iMyIdx]->IsContact )
            {
                g_MyD3D->MyPlayer[iMyIdx]->NoCheckContact = 40;
                g_MyD3D->MyPlayer[iMyIdx]->y_Speed        = -0.01f;

                if ( 1.0f > g_MyD3D->MyPlayer[iMyIdx]->vPos.y ) m_bDeathScrollY = true;
            }
        }
    }
}

void STAGE::SetStage( char cStage, bool bInitMonSlot/*, float fWaterHeight*/ )
{
    SiKGCRoomManager()->GetRoomInfo().ucStage = (unsigned char)cStage;
}

int STAGE::GetStage()
{
    return SiKGCRoomManager()->GetRoomInfo().ucStage;
}

const WCHAR* STAGE::GetStageName( int iStage_ ) const
{
    if( 0 <= iStage_ && iStage_ < NUM_TOTAL_STAGE )
    {
        return g_pkStrLoader->GetString( m_pMapInfo[iStage_].m_iNameID ).c_str();
    }
    else
    {
        return L"준비중..";
    }
}

void STAGE::ON_RENDER_SHINING_DUST(GCDeviceTexture * texture)
{
	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    D3DXMATRIX mat1, mat2, mat3;

    for( int i = 0; i < m_iDustCount[SiKGCRoomManager()->GetRoomInfo().ucStage]; i++ )
    {
        for( unsigned char ii = 0; ii < 8; ii++ )
        {
            D3DXMatrixTranslation( &mat1, 0.2f, 0, .8f );
            D3DXMatrixRotationZ(&mat2, m_pShiningDust[i].Angle[ii]);
            D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
            D3DXMatrixTranslation( &mat2, m_pShiningDust[i].sx[ii],m_pShiningDust[i].sy[ii], 0 );
            D3DXMatrixMultiply( &mat1, &mat3, &mat2 );
            D3DXMatrixMultiply( &mat2, &mat1, &g_kCamera.m_matCamera );
            g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat2);
            float Big;
            Big = m_pShiningDust[i].Angle_Speed[ii];
            if( Big < 0 ) 
                Big *= 1.0f;

			g_pGCDeviceManager2->DrawInWorld(texture,&mat2,
				0.03f-Big*.5f,.03f-Big*.5f,.0156f,.0313f,.2968f,.3125f);
            //g_MyD3D->Draw_Billboard( 0.03f-Big*.5f,.03f-Big*.5f,.0156f,.0313f,.2968f,.3125f);
        }
    }
    g_pGCDeviceManager2->PopState();
}

void STAGE::ON_RENDER_LAVA(GCDeviceTexture * texture)
{
    static float s_afPosTab[][2] =
    {
        { -3.5f, -0.309f, },
        { -2.5f, -0.309f, },
        { -1.5f, -0.309f, },
        { -0.5f, -0.309f, },
        {  0.5f, -0.309f, },
        {  1.5f, -0.309f, },
    };

    static float s_afTexTab1[][4] =
    {
        { 0.51f, 0.045f, 1.00f, 0.25f, }, // A
        { 0.00f, 0.545f, 0.50f, 0.75f, }, // B
        { 0.50f, 0.545f, 1.00f, 0.75f, }, // C
        { 0.51f, 0.045f, 1.00f, 0.25f, }, // A
        { 0.00f, 0.545f, 0.50f, 0.75f, }, // B
        { 0.50f, 0.545f, 1.00f, 0.75f, }, // C
    };

    static float s_afTexTab2[][4] =
    {
        { 0.51f, 0.295f, 1.00f, 0.50f, }, // A'
        { 0.00f, 0.795f, 0.50f, 1.00f, }, // B'
        { 0.50f, 0.795f, 1.00f, 1.00f, }, // C'
        { 0.51f, 0.295f, 1.00f, 0.50f, }, // A'
        { 0.00f, 0.795f, 0.50f, 1.00f, }, // B'
        { 0.50f, 0.795f, 1.00f, 1.00f, }, // C'
    };

    D3DXMATRIX mat1, mat2, mat3;

	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    BYTE clr = ( m_iLava < 0 ? 0 : ( m_iLava > 255 ? 255 : static_cast<BYTE>(m_iLava) ) );

    // 아래 용암 그리기
    for ( int i = 0; i < 6; i++ )
    {
        D3DXMatrixTranslation( &mat1, s_afPosTab[i][0] + m_fLavaTrans1, s_afPosTab[i][1], 0.8f );
        D3DXMatrixMultiply( &mat2, &mat1, &g_kCamera.m_matCamera );

		g_pGCDeviceManager2->DrawInWorld(texture,&mat2,
			1.0f, 0.375f, s_afTexTab2[i][0], s_afTexTab2[i][1],s_afTexTab2[i][2], s_afTexTab2[i][3]);
       // g_MyD3D->Draw_Billboard( 1.0f, 0.375f, s_afTexTab2[i][0], s_afTexTab2[i][1],
         //   s_afTexTab2[i][2], s_afTexTab2[i][3] );
    }

    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

    // 위 용암 그리기
    for ( int i = 0; i < 6; i++ )
    {
        D3DXMatrixTranslation( &mat1, s_afPosTab[i][0] + m_fLavaTrans2, s_afPosTab[i][1], 0.8f );
        D3DXMatrixMultiply( &mat2, &mat1, &g_kCamera.m_matCamera );
		g_pGCDeviceManager2->DrawInWorld(texture,&mat2,
			1.0f, 0.375f, s_afTexTab2[i][0], s_afTexTab2[i][1], s_afTexTab2[i][2], s_afTexTab2[i][3],0,0,0,
			D3DCOLOR_ARGB(255, clr, clr, clr));
       // g_MyD3D->Draw_Billboard( 1.0f, 0.375f, s_afTexTab2[i][0], s_afTexTab2[i][1],
        //    s_afTexTab2[i][2], s_afTexTab2[i][3], 255, clr, clr, clr );
    }
    g_pGCDeviceManager2->PopState();
}

void STAGE::ON_RENDER_SMOG(GCDeviceTexture * texture)
{
    D3DXMATRIX mat1, mat2, mat3;

	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    D3DXMatrixTranslation( &mat1, 0.5f, -0.275f, 0.8f );
    D3DXMatrixMultiply( &mat2, &mat1, &g_kCamera.m_matCamera );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat2 );

	g_pGCDeviceManager2->DrawInWorld(texture,&mat2,
		Num_Width*3.0f, 0.4191f, 0.0f - m_fSmogTrans , 0.0f, 2.0f - m_fSmogTrans, 1.0f);

    //g_MyD3D->Draw_Billboard( Num_Width*3.0f, 0.4191f, 0.0f - m_fSmogTrans , 0.0f, 2.0f - m_fSmogTrans, 1.0f );

    g_pGCDeviceManager2->PopState();
}

void STAGE::ON_RENDER_SAND_DROP(GCDeviceTexture * texture)
{
	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    static float s_fTabTextrCoords[4][2] =
    {
        { 0.00f, 0.25f },
        { 0.25f, 0.50f },
        { 0.50f, 0.75f },
        { 0.75f, 1.00f },
    };

    static float s_fTabScaleTrans[4][4] =
    {
        { 0.046f, 3.000f, -0.058f, 0.612f, },
        { 0.058f, 4.600f,  0.122f, 0.624f, },
        { 0.056f, 5.073f,  0.922f, 0.581f, },
        { 0.043f, 2.879f,  1.096f, 0.600f, },
    };
    D3DXMATRIX mat1, mat2, mat3;
    for ( int i = 0; i < sizeof( m_afSandDrop ) / sizeof( float ); i++ )
    {
        if ( m_afSandDrop[i] <= 1.0f )
        {
            BYTE alpha;

            ::D3DXMatrixTranslation( &mat1, 0.0f, -0.5f, 0.0f );
            if ( m_afSandDrop[i] > 0.9f )
            {
                ::D3DXMatrixScaling( &mat2,
                    s_fTabScaleTrans[i][0] * ( 1.0f - m_afSandDrop[i] ) / 0.1f,
                    s_fTabScaleTrans[i][1] * 0.2f, 1.0f );
                alpha = static_cast<BYTE>( 255.0f * ( 1.0f - m_afSandDrop[i] ) / 0.1f );
            }
            else
            {
                ::D3DXMatrixScaling( &mat2,
                    s_fTabScaleTrans[i][0] * ( m_afSandDrop[i] > 0.2f ? 0.2f : m_afSandDrop[i] ) / 0.2f,
                    s_fTabScaleTrans[i][1] * ( m_afSandDrop[i] > 0.2f ? 0.2f : m_afSandDrop[i] ), 1.0f );
                alpha = static_cast<BYTE>( 255.0f * ( m_afSandDrop[i] > 0.2f ? 0.2f : m_afSandDrop[i] ) / 0.2f );
                if ( m_afSandDrop[i] > 0.2f && 0 == o5 )
                {
                    g_MyD3D->MySparks->AddSandDust( s_fTabScaleTrans[i][2],
                        s_fTabScaleTrans[i][3] - s_fTabScaleTrans[i][1] * 0.2f + 0.3f );
                }
            }
            ::D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
            ::D3DXMatrixTranslation( &mat2, s_fTabScaleTrans[i][2], s_fTabScaleTrans[i][3], 0.8f );
            ::D3DXMatrixMultiply( &mat1, &mat3, &mat2 );
            ::D3DXMatrixMultiply( &mat2, &mat1, &g_kCamera.m_matCamera );
            
			g_pGCDeviceManager2->DrawInWorld(m_pTexSand,&mat2,
				1.0f, 1.0f,s_fTabTextrCoords[m_iSandFrame][0], 0.0f,s_fTabTextrCoords[m_iSandFrame][1], 1.0f,0,0,0,
				D3DCOLOR_ARGB(alpha,255,255,255) );
            //g_MyD3D->Draw_Billboard( 1.0f, 1.0f,
             //   s_fTabTextrCoords[m_iSandFrame][0], 0.0f,
              //  s_fTabTextrCoords[m_iSandFrame][1], 1.0f, alpha );
        }
    }
    g_pGCDeviceManager2->PopState();
}

void STAGE::RegisterLuabind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<STAGE>( L, "STAGE" );
    lua_tinker::class_def<STAGE>( L, "SetShiningPosition", &STAGE::SetShiningPosition );
    lua_tinker::class_def<STAGE>( L, "SetShiningDustNum", &STAGE::SetShiningDustNum );
    lua_tinker::class_def<STAGE>( L, "LoadMapInfo", &STAGE::LoadMapInfo );

    lua_tinker::class_add<KGCCameraOrtho>( L, "KGCCameraOrtho" );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_bZoomViewMode",  &KGCCameraOrtho::m_bZoomViewMode );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_iHostViewZoomMode",  &KGCCameraOrtho::m_iHostViewZoomMode );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_iWideMode",  &KGCCameraOrtho::m_fWideMode );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fFollowRateX",  &KGCCameraOrtho::m_fFollowRateX );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fFollowRateY",  &KGCCameraOrtho::m_fFollowRateY );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fLookTermX",  &KGCCameraOrtho::m_fLookTermX );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fLookTermY",  &KGCCameraOrtho::m_fLookTermY );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fBoundTermY",  &KGCCameraOrtho::m_fBoundTermY );
        lua_tinker::class_mem<KGCCameraOrtho>( L, "m_fBoundTermX",  &KGCCameraOrtho::m_fBoundTermX );;

    lua_tinker::decl( L,"STAGE", this);

    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "MapInfo.lua" ) == false )
        return;

}

void STAGE::SetShiningPosition( int iMapNum, int addCount, float fx_, float fy_ )
{
    if( SiKGCRoomManager()->GetRoomInfo().ucStage != iMapNum )
        return;

    if( addCount >= 0 && addCount < m_iDustCount[iMapNum] )
    {
        m_vecDustPosition[ addCount ].x = fx_;
        m_vecDustPosition[ addCount ].y = fy_;
    }
}

void STAGE::SetShiningDustNum( int iMapNum, int DustCount )
{
    if( SiKGCRoomManager()->GetRoomInfo().ucStage != iMapNum )
        return;

    if( iMapNum < NUM_TOTAL_STAGE )
        m_iDustCount[iMapNum] = DustCount;

    // new 되어있을지도 모르니 지워줌
    SAFE_DELETE_ARRAY(m_pShiningDust);
    SAFE_DELETE_ARRAY(m_vecDustPosition);

    // 반딧불 관련 init
    if( m_iDustCount[iMapNum] != 0 )
    {
        m_pShiningDust = new ShingDust[ m_iDustCount[iMapNum] ];
        m_vecDustPosition = new D3DXVECTOR2[ m_iDustCount[iMapNum] ];

        ASSERT( m_pShiningDust != NULL );
        ASSERT( m_vecDustPosition != NULL );

        ZeroMemory( m_pShiningDust, sizeof( ShingDust ) * m_iDustCount[iMapNum] );
        ZeroMemory( m_vecDustPosition, sizeof( D3DXVECTOR2) * m_iDustCount[iMapNum] );
    }
}

void STAGE::LoadMapInfo()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    KLuaManager luaMgr(L);
    if( !LoadScript(luaMgr) )
    {
        MessageBox(NULL, L"MapInfo Script Load Error", L"Error in Loading Script", MB_OK);
        exit(0);
    }
}

void STAGE::LoadParticle()
{
    // 파티클 Attatch 한 만큼 생성
    for( int i = 0; i < (int)m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo.size(); i++ )
    {
        CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence(
            m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo[i].m_strParticleName,
            m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo[i].m_fParticlex,
            m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo[i].m_fParticley,
            0.5f );// z는 항상 0.5f;

        if ( m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo[i].m_bOnUI == true )
        {
            g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
        }

        m_vecSequence.push_back( pParticle );
    }
}

struct CameraMember
{
    int iHostViewZoomMode;
    int iWideMode;
    float fFollowRateX;
    float fFollowRateY;
    float fBoundTermX;
    float fBoundTermY;
    float fLookTermX;
    float fLookTermY;

    CameraMember()
    {
        iHostViewZoomMode = 0;
        iWideMode = 0;
        fFollowRateX = 0.0f;
        fFollowRateY = 0.0f;
        fBoundTermX = 0.0f;
        fBoundTermY = 0.0f;
        fLookTermX = 0.0f;
        fLookTermY = 0.0f;
    }
};

bool STAGE::LoadScript( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE( "GlobalCamera", return false)
    {
        LUA_GET_VALUE( "HostViewZoomMode", g_kCamera.m_iHostViewZoomMode, return false )
        LUA_GET_VALUE( "WideMode", g_kCamera.m_fWideMode, return false )
        LUA_GET_VALUE( "FollowRateX", g_kCamera.m_fFollowRateX, return false )
        LUA_GET_VALUE( "FollowRateY", g_kCamera.m_fFollowRateY, return false )
        LUA_GET_VALUE( "BoundX", g_kCamera.m_fBoundTermX, return false )
        LUA_GET_VALUE( "BoundY", g_kCamera.m_fBoundTermY, return false )
        LUA_GET_VALUE( "LookX", g_kCamera.m_fLookTermX, return false )
        LUA_GET_VALUE( "LookY", g_kCamera.m_fLookTermY, return false )
    }
    LUA_END_TABLE(return false);

    static bool isFirst = true;

    const char* TableName;
    const char* SequenceName;

    // 처음 읽을 때에는 맵스트링을 다 읽어야함
    if( isFirst )
    {
        // 맵 스트링 세팅
        for( int i = 0; i < NUM_TOTAL_STAGE; i++ )
        {
            std::wstringstream WTableName;
            WTableName << L"Stage" << i;
            TableName = GCUTIL_STR::GCStrWideToChar( WTableName.str().c_str() );

            LUA_BEGIN_TABLE( TableName, return false )
            {
                LUA_GET_VALUE( "StageNameID", m_pMapInfo[i].m_iNameID, return false )
            }
            LUA_END_TABLE(return false)
        }
        isFirst = false;
        return true;
    }
    else
    {
        CameraMember GlobalCam;
        LUA_BEGIN_TABLE( "GlobalCamera", return false)
        {
            LUA_GET_VALUE( "HostViewZoomMode", GlobalCam.iHostViewZoomMode, return false )
            LUA_GET_VALUE( "WideMode", GlobalCam.iWideMode, return false )
            LUA_GET_VALUE( "FollowRateX", GlobalCam.fFollowRateX, return false )
            LUA_GET_VALUE( "FollowRateY", GlobalCam.fFollowRateY, return false )
            LUA_GET_VALUE( "BoundX", GlobalCam.fBoundTermX, return false )
            LUA_GET_VALUE( "BoundY", GlobalCam.fBoundTermY, return false )
            LUA_GET_VALUE( "LookX", GlobalCam.fLookTermX, return false )
            LUA_GET_VALUE( "LookY", GlobalCam.fLookTermY, return false )
        }
        LUA_END_TABLE(return false)

        if( SiKGCRoomManager()->GetRoomInfo().ucStage >= NUM_TOTAL_STAGE )
        {
            MessageBox(NULL, L"MapInfo Overflow", L"Stage > NUM_TOTAL_STAGE", MB_OK);
            exit(0);
        }

        std::wstringstream WTableName;
        WTableName << L"Stage" << SiKGCRoomManager()->GetRoomInfo().ucStage;
        TableName = GCUTIL_STR::GCStrWideToChar( WTableName.str().c_str() );

        LUA_BEGIN_TABLE( TableName, return false )
        {
            LUA_GET_VALUE( "BGTextureNum", m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_iSkyID, return false )
            LUA_GET_VALUE( "StageNameID", m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_iNameID, return false )
            LUA_GET_VALUE( "MapProperty", m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty, return false )
            wchar_t WSequenceName[16];
            LUA_BEGIN_TABLE( "Particle", return false )
            {
                int SequenceIndex = 1;

                while( 1 )
                {
                    wsprintf(WSequenceName, L"Sequence%d", SequenceIndex++);
                    SequenceName = GCUTIL_STR::GCStrWideToChar( WSequenceName );

                    if( FAILED(luaMgr.BeginTable( SequenceName )) )
                        break;

                    PARTICLE_INFO   tempParticle;
                    LUA_GET_VALUE("SequenceName", tempParticle.m_strParticleName, return false )
                    LUA_GET_VALUE("X", tempParticle.m_fParticlex, return false )
                    LUA_GET_VALUE("Y", tempParticle.m_fParticley, return false )
                    LUA_GET_VALUE_DEF("OnUI", tempParticle.m_bOnUI, false )

                    m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_vecParticleInfo.push_back( tempParticle );

                    if( FAILED(luaMgr.EndTable()) )
                        break;
                }
            }
            LUA_END_TABLE(return false)
            // 카메라 테이블
            LUA_BEGIN_TABLE( "Camera", return false)
            {
                LUA_GET_VALUE_DEF( "HostViewZoomMode", g_kCamera.m_iHostViewZoomMode, GlobalCam.iHostViewZoomMode )
                LUA_GET_VALUE_DEF( "WideMode", g_kCamera.m_fWideMode, (float)GlobalCam.iWideMode )
                LUA_GET_VALUE_DEF( "FollowRateX", g_kCamera.m_fFollowRateX, GlobalCam.fFollowRateX )
                LUA_GET_VALUE_DEF( "FollowRateY", g_kCamera.m_fFollowRateY, GlobalCam.fFollowRateY )
                LUA_GET_VALUE_DEF( "BoundX", g_kCamera.m_fBoundTermX, GlobalCam.fBoundTermX )
                LUA_GET_VALUE_DEF( "BoundY", g_kCamera.m_fBoundTermY, GlobalCam.fBoundTermY )
                LUA_GET_VALUE_DEF( "LookX", g_kCamera.m_fLookTermX, GlobalCam.fLookTermX )
                LUA_GET_VALUE_DEF( "LookY", g_kCamera.m_fLookTermY, GlobalCam.fLookTermY )
            }
            LUA_END_TABLE(return false)

            LoadStreamTable( luaMgr );
            LoadAmbientTable( luaMgr );
        }
        LUA_END_TABLE(return false)
        return true;
    }
}

void STAGE::LoadStreamTable( KLuaManager& luaMgr )
{
    for( int i = 0; i < (int)m_vecMapStreamData.size(); ++i )
        m_vecMapStreamData[i].Clear();

    LUA_BEGIN_TABLE( "Stream", return )
    {
        for( int iter = 1; ; ++iter )
        {
            LUA_BEGIN_TABLE( iter, break )
            {
                MapStreamData m_MapStreamData;
                m_MapStreamData.iEvent = 0;

                LUA_GET_VALUE( "INIT_TIME", m_MapStreamData.iEventInitTime, return );
                LUA_GET_VALUE( "SYNC_TIMING", m_MapStreamData.iSyncTiming, return );
                LUA_BEGIN_TABLE( "SOUND_EVENT", return )
                {
                    for( int i = 1; ; ++i )
                    {
                        int iSound;
                        LUA_GET_VALUE_NOASSERT( i, iSound, break );
                        m_MapStreamData.vecStreamSoundEvent.push_back( iSound );
                    }
                }
                LUA_END_TABLE( return );
                for( int i = 1; ; ++i )
                {
                    LUA_BEGIN_TABLE( i, break )
                    {
                        StreamData sd;

                        LUA_GET_VALUE_DEF( "EVENT_START", sd.iEventStart, -1 );
                        LUA_GET_VALUE_DEF( "EVENT_END", sd.iEventEnd, -1 );
                        LUA_GET_VALUE_DEF( "X", sd.fX, 0.0f );
                        LUA_GET_VALUE_DEF( "Y", sd.fY, 0.0f );
                        LUA_GET_VALUE_DEF( "LIFE", sd.iLife, 255 );
                        LUA_GET_VALUE_DEF( "Y_SPEED", sd.fYSpeed, 0.03f );
                        LUA_GET_VALUE_DEF( "JUMP_FRAME", sd.iJumpFrame, 0 );

                        m_MapStreamData.vecSD.push_back( sd );
                    }
                    LUA_END_TABLE( return );
                }
                m_vecMapStreamData.push_back( m_MapStreamData );
            }
            LUA_END_TABLE( break );
        }
    }
    LUA_END_TABLE( return );
}

void STAGE::LoadAmbientTable( KLuaManager& luaMgr )
{
    m_vecAS.clear();

    LUA_BEGIN_TABLE( "AmbientSound", return )
    {
        for( int i = 1; ; ++i )
        {
            LUA_BEGIN_TABLE(i, break )
            {
                AmbientSound AS;

                LUA_GET_VALUE_DEF( "ID", AS.iSoundID, 0 );
                LUA_GET_VALUE_DEF( "INIT_TIME", AS.iEventInitTime, -1 );
                LUA_GET_VALUE_DEF( "RATE", AS.iRate, 0 );

                m_vecAS.push_back( AS );
            }
            LUA_END_TABLE( break );
        }
    }
    LUA_END_TABLE( return );
}

void STAGE::InitializeStreamEvent( int iIndex )
{
    if( iIndex < 0 || iIndex > (int)m_vecMapStreamData.size() )
        return;

     MapStreamData& m_MapStreamData = m_vecMapStreamData[iIndex];

     if ( &m_MapStreamData == NULL )
         return;

    // Initialize Stream Event
    m_MapStreamData.iEvent = 0;
}

float STAGE::GetXLimit()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() ) {
        return SiKGCFantasticMap()->GetPixelWidth();
    }
    else {
        return 1.5f * (float)g_MyD3D->MyStg->Num_Width;
    }
}

float STAGE::GetYLimit()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        return SiKGCFantasticMap()->GetPixelHeight();
    }
    else
    {
        return 1.5f * (float)g_MyD3D->MyStg->Num_Height;
    }
}

void STAGE::LoadFootholdTexture()
{
	// 레이어1 발판 로딩
	for(unsigned char ix=0 ; ix<Num_Width*Num_Height ; ++ix)
	{
		std::string& strFoodHoldName = SiKGCRoomManager()->GetStageFootHoldTextureName(ix, true);
		m_pTexLevel1[ix] = g_pGCDeviceManager2->CreateTexture( strFoodHoldName );
	}
}

void STAGE::LoadFrontBGTexture()
{
	if( m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty != 0 )
	{
		DWORD flag = 0x00000001;

		for( int i = 1; i < 33; i++ )
		{
			if( m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty & flag )
			{
				char FrontTextureFileName[50];
				sprintf( FrontTextureFileName, "Front%d.dds", i );

				m_vecTexture.push_back(
					g_pGCDeviceManager2->CreateTexture( FrontTextureFileName ) );
			}
			flag = flag << 1;
		}

		int texCount = 0;
		flag = 0x00000001;
		m_pdwTexStageNum = new DWORD[ m_vecTexture.size() ];

		for( int i = 0; i < 32; i++ )
		{
			if( m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_dwMapProperty & flag )
			{
				m_pdwTexStageNum[texCount] = flag;
				texCount++;
			}
			flag = flag << 1;
		}
	}
}

void STAGE::LoadSkyTexture()
{
	int iSkyID = m_pMapInfo[SiKGCRoomManager()->GetRoomInfo().ucStage].m_iSkyID;
	if( iSkyID != -1)
	{
		std::string& strSkyTextureName = SiKGCRoomManager()->GetStageSkyTextureName(iSkyID, true);
		m_pTexSky = g_pGCDeviceManager2->CreateTexture( strSkyTextureName );
	}
}

bool STAGE::Check_Wall(float fX, float fY, float fOffSet)
{
    int i_X             = (int)(fX * 20.0f);
    int i_Y             = (int)(fY * 20.0f);
    int XPerLine = 30 * Num_Width, Index;

    // 현재블럭확인
    Index = i_X + i_Y * XPerLine;
    if(FIELD_INFO(Index)==3)    
        return true;
    if(FIELD_INFO(Index)==2)
        return true;

    // 오프셋 발판들도 확인
    for(int iLoop=1; iLoop<=fOffSet; ++iLoop)
    {
        // 왼쪽 옆 블럭확인
        Index = i_X - iLoop + i_Y * XPerLine;
        if(FIELD_INFO(Index)==3)
            return true;
        if(FIELD_INFO(Index)==2)
            return true;

        // 오른쪽 옆 블럭확인
        Index = i_X + iLoop + i_Y * XPerLine;
        if(FIELD_INFO(Index)==3)
            return true;
        if(FIELD_INFO(Index)==2)
            return true;
    }

    return false;
}

void STAGE::ClearParticle( void )
{
    if( m_vecSequence.empty() == false ) { 
        g_ParticleManager->DeleteSequence( m_vecSequence );
    }
}