#include "stdafx.h"
#include ".\GCFantasticMap.h"

#include "KGCLayerManager.h"

//
//
#include "GCCameraOrtho.h"

#include "GCO2Manager.h"
#include "Wave.h"
#include "KGCRoomManager.h"
#include "../gcui/GCGameOverlayUI.h"
#include "../QuestGameManager.h"

#include "GCCameraOrtho.h"
#include "GCStateMachine.h"

extern GCDeviceShader* g_pRenderState;

#define MAP_ARRAY ((foothold_type**)m_ppFHWorld)

ImplementSingleton(KGCFantasticMap);

KGCFantasticMap::KGCFantasticMap(void) : KMap<KGCFootHold>()
{
    RegisterLuabind();

//#ifdef _DEBUG
//    m_pDebugFH = NULL;
//#endif
    m_bNothingFootHold      = true;
    m_fWaterHeight  = 0.0f;
    m_pkO2Manager   = NULL;
    m_pTexWater     = NULL;
    m_pWave         = NULL;
}

KGCFantasticMap::~KGCFantasticMap(void)
{
    UnLoad();
    
    SAFE_RELEASE( m_pTexWater );
    SAFE_DELETE( m_pkO2Manager );
    SAFE_DELETE( m_pWave );
}

void KGCFantasticMap::RegisterLuabind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCFantasticMap>( L, "KGCFantasticMap" );
    lua_tinker::class_def<KGCFantasticMap>( L, "Load",  &KGCFantasticMap::Load );
    //lua_tinker::class_def<KGCFantasticMap>( L, "GetRandomPosX",  KGCFantasticMap::GetRandomPosX );
    //lua_tinker::class_def<KGCFantasticMap>( L, "GetRandomPosY",  KGCFantasticMap::GetRandomPosY );
    lua_tinker::class_def<KGCFantasticMap>( L, "SetWorld",  &KGCFantasticMap::SetWorld );
    lua_tinker::class_def<KGCFantasticMap>( L, "AddDungeonFootHold",  &KGCFantasticMap::AddDungeonFootHold );
    //lua_tinker::class_def<KGCFantasticMap>( L, "SetColor",  KGCFantasticMap::SetColor );
    //lua_tinker::class_def<KGCFantasticMap>( L, "GetColorDWORD",  KGCFantasticMap::GetColorDWORD );

    lua_tinker::class_def<KGCFantasticMap>( L, "SetLayerMoveSpeed",  &KGCFantasticMap::SetLayerMoveSpeed );
    lua_tinker::class_def<KGCFantasticMap>( L, "GetLayerMoveSpeedX",  &KGCFantasticMap::GetLayerMoveSpeedX );
    lua_tinker::class_def<KGCFantasticMap>( L, "GetLayerMoveSpeedY",  &KGCFantasticMap::GetLayerMoveSpeedY );

    lua_tinker::class_def<KGCFantasticMap>( L, "AddDungeonFootHoldByValue",  &KGCFantasticMap::AddDungeonFootHoldByValue );
    lua_tinker::class_def<KGCFantasticMap>( L, "RemoveDungeonFootHoldToBind",  &KGCFantasticMap::RemoveDungeonFootHoldToBind );
    lua_tinker::class_def<KGCFantasticMap>( L, "AddDungeonFootHoldByValueBind",  &KGCFantasticMap::AddDungeonFootHoldByValueBind );

    lua_tinker::class_def<KGCFantasticMap>( L, "GetWorldWidth",   &KGCFantasticMap::GetWorldHeight );
    lua_tinker::class_def<KGCFantasticMap>( L, "GetWorldHeight",  &KGCFantasticMap::GetWorldHeight );

    lua_tinker::decl( L, "pMap", this );
}

void KGCFantasticMap::Load()
{
//#ifdef _DEBUG
//    m_pDebugFH = new KGC2DRenderer();
//#endif

    m_ppFHWorld = new foothold_type*[m_iFHWidth];
    for( int i = 0 ; i < m_iFHWidth; i++ )
    {
        m_ppFHWorld[i] = new foothold_type[m_iFHHeight];
    }
}

void KGCFantasticMap::UnLoad()
{
//#ifdef _DEBUG
//    SAFE_DELETE( m_pDebugFH );
//#endif

    if( m_ppFHWorld != NULL )
    {
        for( int i = 0 ; i < m_iFHWidth; i++ )
        {
			SAFE_DELETE_ARRAY( m_ppFHWorld[i] );
        }

        SAFE_DELETE_ARRAY( m_ppFHWorld );

        m_iFHWidth      = 0;
        m_iFHHeight     = 0;
    }
}

//DWORD KGCFantasticMap::GetColorDWORD( BYTE r, BYTE g, BYTE b )
//{
//    return D3DCOLOR_RGBA( r, g, b, 255 );
//}

//void KGCFantasticMap::SetColor( DWORD dwColor, int iKeepFrame )
//{
//    SiKGCLayerManager()->SetColor( dwColor, iKeepFrame );
//}

void KGCFantasticMap::AddDungeonFootHold( KLuaManager& luaMgr )
{
    if( m_ppFHWorld == NULL )
        return;

    int PosX = 0;
    int PosY = 0;
    DWORD Prop = 0;
    std::string strValue;

    LUA_GET_VALUE_DEF( 1, PosX, 0 );
    LUA_GET_VALUE_DEF( 2, PosY, 0 );
    LUA_GET_VALUE_DEF( 3, Prop, foothold_type::FHP_JUST_FOOTHOLD );
    LUA_GET_VALUE_DEF( 4, strValue, "" );

    AddDungeonFootHoldByValue( static_cast<float>(PosX*FOOTHOLDSIZE), static_cast<float>((PosY+1)*FOOTHOLDSIZE), Prop, NULL, strValue );

//#ifdef _DEBUG
//    GCDeviceTexture* pTexture = g_pGCDeviceManager->CreateTexture( "Extra0.dds" );
//    m_pDebugFH->AddChild( pTexture, FHTemp->GetX(), FHTemp->GetY()-FOOTHOLDSIZE, FHTemp->GetX()+FOOTHOLDSIZE, FHTemp->GetY(), 100, 0.0f, 1.0f, 1.0f, 0.0f );
//#endif
}

void KGCFantasticMap::AddDungeonFootHoldByValue( float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue )
{    
    foothold_type FHTemp;

    FHTemp.SetFootHoldType( foothold_type::FHT_HEAD );
    FHTemp.SetFootHoldProp( Prop );
    FHTemp.SetPos( PosX, PosY );
    FHTemp.SetParent( (KGCObj*)pParent );
    FHTemp.SetValue( strValue );

    if( FHTemp.GetIndexX() >= m_iFHWidth ||
        FHTemp.GetIndexY() >= m_iFHHeight )
        return;

    if( m_ppFHWorld[ FHTemp.GetIndexX() ][ FHTemp.GetIndexY() ].GetFootHoldType() == foothold_type::FHT_INIT )
    {
        if( FHTemp.GetFootHoldProp() == foothold_type::FHP_NO_ACTION )
            FHTemp.SetFootHoldType( foothold_type::FHT_INIT );
        
        m_ppFHWorld[ FHTemp.GetIndexX() ][ FHTemp.GetIndexY() ] = FHTemp;
        m_bNothingFootHold = false;
    }
}

void KGCFantasticMap::AddDungeonFootHoldByValueBind( float PosX, float PosY, DWORD Prop )
{
    AddDungeonFootHoldByValue( PosX, PosY, Prop, NULL, "" );
}

void KGCFantasticMap::RemoveDungeonFootHold( float PosX, float PosY, void* pParent )
{
    foothold_type FHTemp;

    FHTemp.SetPos( PosX, PosY );

    if( FHTemp.GetIndexX() >= m_iFHWidth ||
        FHTemp.GetIndexY() >= m_iFHHeight )
        return;

    if( pParent == (void*)m_ppFHWorld[ FHTemp.GetIndexX() ][ FHTemp.GetIndexY() ].GetParent() )
    {
        m_ppFHWorld[ FHTemp.GetIndexX() ][ FHTemp.GetIndexY() ].SetFootHoldType( foothold_type::FHT_INIT );
        m_ppFHWorld[ FHTemp.GetIndexX() ][ FHTemp.GetIndexY() ].SetFootHoldProp( foothold_type::FHP_JUST_FOOTHOLD );
    }

}

void KGCFantasticMap::RemoveDungeonFootHoldToBind( float PosX, float PosY )
{
    RemoveDungeonFootHold( PosX, PosY, NULL );
}

bool KGCFantasticMap::CheckPierceBlockXPos( float fBeforeX, OUT float& fAfterX, float fPosY )
{
    if( fBeforeX == fAfterX )
        return false;
   
    float fStartX, fEndX;
    DWORD dwCheckProp;

    bool bIsRight;
    if( fAfterX > fBeforeX )
    {
        fStartX = fBeforeX;
        fEndX = fAfterX;
        dwCheckProp = foothold_type::FHP_NO_COME_LEFT;
        bIsRight = true;
    }
    else
    {
        fStartX = fAfterX;
        fEndX = fBeforeX;
        dwCheckProp = foothold_type::FHP_NO_COME_RIGHT;
        bIsRight = false;
    }

    int StartIndex = (int)( TO_INDEX(fStartX) );
    int EndIndex = (int)( TO_INDEX(fEndX) );
    int YIndex = (int)( TO_INDEX(fPosY) );

    if( StartIndex == EndIndex )
        return false;

    if( EndIndex < 0 || StartIndex < 0 )
    {
        fAfterX = 0.0f;
        return true;
    }

    float fBlockedX;
    if( (fBlockedX = IndexBetweenCheck( StartIndex, EndIndex, YIndex, dwCheckProp )) != -1.0f )
    {
        if( true == bIsRight && dwCheckProp == foothold_type::FHP_NO_COME_LEFT )
        {
            fAfterX = fBlockedX - FOOTHOLDSIZE;
            return true;
        }
        else if( false == bIsRight && dwCheckProp == foothold_type::FHP_NO_COME_RIGHT )
        {
            fAfterX = fBlockedX + FOOTHOLDSIZE;
            return true;
        }
        return false;
    }

    if( EndIndex >= m_iFHWidth || StartIndex >= m_iFHWidth )
    {
        fAfterX = m_iFHWidth * FOOTHOLDSIZE - 0.0125f;
        return true;
    }

    return false;
}

bool KGCFantasticMap::CheckPierceBlockYPos( float fPosX, float fBeforeY, OUT float& fAfterY )
{
    if( fBeforeY == fAfterY )
        return false;

    float fStartY, fEndY;

    if( fAfterY > fBeforeY )
    {
        fStartY = fBeforeY;
        fEndY = fAfterY;
    }
    else
    {
        fStartY = fAfterY;
        fEndY = fBeforeY;
    }

    int StartIndex = (int)( TO_INDEX(fStartY) );
    int EndIndex = (int)( TO_INDEX(fEndY) );
    int XIndex = (int)( TO_INDEX(fPosX) );

    if( StartIndex == EndIndex )
        return false;

    if( EndIndex < 0 || StartIndex < 0 )
    {
        fAfterY = 0.0f;
        return true;
    }

    float fBlockedY;
    if( (fBlockedY = IndexBetweenYCheck( XIndex, StartIndex, EndIndex )) != -1.0f )
    {
        fAfterY = fBlockedY - FOOTHOLDSIZE;
        return true;
    }

    return false;
}

bool KGCFantasticMap::AdjustUnitPosition( KGCContectInfo& kInfo_ )
{
    if( m_ppFHWorld == NULL )
        return false;

    int i_X, i_Y;
    i_X = static_cast<int>( TO_INDEX(kInfo_.m_x) );
    i_Y = static_cast<int>( TO_INDEX(kInfo_.m_y) );
    //int i_HeadY = static_cast<int>( TO_INDEX( kInfo_.m_HeadY ) );

    BoundCheck( i_X, i_Y );
    //BoundCheck( i_X, i_HeadY );

    if( i_Y < m_iFHHeight - 1 )         // 위에 한칸이라도 있어야 한다
    {
        if( false == kInfo_.m_bNotCheckSideFH )
        {
            // 왼쪽으로 뚫고 지나가는것을 막으려고 함
            if( i_X > 0 && kInfo_.m_bTagMotion == false && MAP_ARRAY[ i_X - 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )   // [ 왼쪽위 ] 막혔나?
            {
                if( i_X < m_iFHWidth - 1 && MAP_ARRAY[ i_X + 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_INIT )                // [ 오른쪽위 ] 막혔나?
                {
                    if( MAP_ARRAY[ i_X - 1 ][ i_Y + 1 ].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT )    // 왼쪽발판이 지나갈 수 없는 발판이라면..(오른쪽으로 갈려고 할때는 체크안해야지!!)
                    {
                        if( kInfo_.m_xSpeed <= 0.0f )
                        {
                            kInfo_.m_x = (float)(MAP_ARRAY[ i_X - 1 ][ i_Y + 1 ].GetX() + FOOTHOLDSIZE + FOOTHOLDTERM);
                            if( kInfo_.m_xSpeed != 0.0f )
                                kInfo_.m_xSpeed += 0.0075f;
                        }
                    }
                }
            }

            // 오른쪽으로 뚫고 지나가는것을 막으려고 함
            if( i_X < m_iFHWidth - 1 && kInfo_.m_bTagMotion == false && MAP_ARRAY[ i_X + 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD ) // 오른쪽이 막혔나?(왼쪽으로 갈려고 할때는 체크안해야지!!)
            {
                if( i_X > 0 && MAP_ARRAY[ i_X - 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_INIT )
                {
                    if( MAP_ARRAY[ i_X + 1 ][ i_Y + 1 ].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT )
                    {
                        if( kInfo_.m_xSpeed >= 0.0f )
                        {
                            kInfo_.m_x = (float)(MAP_ARRAY[ i_X + 1 ][ i_Y + 1 ].GetX() - FOOTHOLDSIZE - FOOTHOLDTERM);
                            if( kInfo_.m_xSpeed != 0.0f )
                                kInfo_.m_xSpeed -= 0.012f;
                        }
                    }
                }
            }
        }

        // 위로 뚫고 지나가는것 막기 
        //if( i_HeadY < m_iFHHeight -1 && kInfo_.m_bTagMotion == false && 
        //    ( MAP_ARRAY[ i_X ][ i_HeadY - 1 ].GetFootHoldType() == foothold_type::FHT_HEAD || 
        //      MAP_ARRAY[ i_X ][ i_HeadY ].GetFootHoldType() == foothold_type::FHT_HEAD || 
        //      MAP_ARRAY[ i_X ][ i_HeadY + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
        //  )
        //{
        //    if( i_HeadY > 0 && MAP_ARRAY[ i_X ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_INIT )
        //    {
        //        bool bCheck = false;

        //        if( MAP_ARRAY[ i_X ][ i_HeadY - 1 ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_UP ) 
        //        {
        //            kInfo_.m_y = (float)(MAP_ARRAY[ i_X ][ i_HeadY - 1 ].GetY() - FOOTHOLDSIZE - FOOTHOLDTERM);
        //            bCheck = true;
        //        }
        //        else if( MAP_ARRAY[ i_X ][ i_HeadY ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_UP ) 
        //        {
        //            kInfo_.m_y = (float)(MAP_ARRAY[ i_X ][ i_HeadY ].GetY() - FOOTHOLDSIZE - FOOTHOLDTERM);
        //            bCheck = true;
        //        }
        //        else if( MAP_ARRAY[ i_X ][ i_HeadY + 1 ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_UP )  
        //        {
        //            kInfo_.m_y = (float)(MAP_ARRAY[ i_X ][ i_HeadY + 1 ].GetY() - FOOTHOLDSIZE - FOOTHOLDTERM);
        //            bCheck = true;
        //        }

        //        if( bCheck && kInfo_.m_ySpeed > 0.0f )
        //            kInfo_.m_ySpeed = 0.0f;
        //    }
        //} 

        if( 0.0f < kInfo_.m_ySpeed )
        {
            // 살짝 윗 발판까지 체크해볼 수 있도록 하자( FOOTHOLDSIZE 더해준거.. 타이밍 정말 아찔하게 맞아 떨어지면 통과할 수도 있다. )
            float fAfterHead = kInfo_.m_y + kInfo_.m_ySpeed + kInfo_.m_fHeight + FOOTHOLDSIZE;
            float fBeforeHead = kInfo_.m_y + kInfo_.m_fHeight;
            if( CheckPierceBlockYPos( kInfo_.m_x, fBeforeHead, fAfterHead ) )
            {
                kInfo_.m_y = fAfterHead - kInfo_.m_fHeight;
                kInfo_.m_ySpeed = 0.0f;
                kInfo_.m_cCheck = 1;

                i_Y = static_cast<int>( TO_INDEX(kInfo_.m_y) );
                BoundCheck( i_X, i_Y );

                return false;
            }
        }

        // 지금 디딘것은 밟을 수 없지만, 위쪽에 있는게 밟을 수 있다면, 위쪽에 있는걸로 하자.
        if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldType() == foothold_type::FHT_INIT &&
            MAP_ARRAY[ i_X ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
        {
            i_Y++;
        }

    }

    if( i_Y > 0 )
    {
        // 지금 디딘것은 밟을 수 없지만, 아래쪽에 있는게 밟을 수 있다면, 아래쪽에 있는걸로 하자.
        if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldType() == foothold_type::FHT_INIT &&
            MAP_ARRAY[ i_X ][ i_Y - 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
        {
            i_Y--;
        }
    }

    int iSideBlockY = -1;
    float fInterpolation = 0.0f;
    // ---------------------------------------------------------------디디기 y 보간-------------------------------------------
    fInterpolation = abs( ( kInfo_.m_x - FOOTHOLDSIZE * i_X ) / FOOTHOLDSIZE - 0.5f );

    if( kInfo_.m_x - FOOTHOLDSIZE * i_X < FOOTHOLDSIZE * 0.5f ) // 블럭의 반을 안 넘었음
    {
        if( i_X > 0 )
        {
            // 내 왼쪽에 밟을게 없다!!!
            if( MAP_ARRAY[ i_X - 1 ][ i_Y ].GetFootHoldType() == foothold_type::FHT_INIT )
            {
                //fInterpolation = ( (FOOTHOLDSIZE * 0.5f) - ( kInfo_.m_x - FOOTHOLDSIZE * ((int)(kInfo_.m_x / FOOTHOLDSIZE)) ) ) / FOOTHOLDSIZE; //큰거에서 작은거 뺀거다
                if( i_Y < m_iFHHeight - 1 )     // 왼쪽 위쪽에 밟을게 있나? 그럼 그것과 보간하자
                {
                    if( MAP_ARRAY[ i_X - 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
                    {
                        iSideBlockY = i_Y + 1;
                    }
                }
                if( iSideBlockY == - 1 && i_Y > 0 ) // 왼쪽 아래쪽에 밟을게 있나? 그럼 그것과 보간하자
                {
                    if( MAP_ARRAY[ i_X - 1 ][ i_Y - 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
                    {
                        iSideBlockY = i_Y - 1;
                    }
                }
            }
        }
    }
    else // 블럭의 반을 넘었음.
    {
        if( i_X < m_iFHWidth - 1 )
        {
            // 내 오른쪽에 밟을게 없다!!!
            if( MAP_ARRAY[ i_X + 1 ][ i_Y ].GetFootHoldType() == foothold_type::FHT_INIT )
            {
                //fInterpolation = (( kInfo_.m_x - FOOTHOLDSIZE * ((int)(kInfo_.m_x / FOOTHOLDSIZE)) ) - (FOOTHOLDSIZE * 0.5f)) / FOOTHOLDSIZE; //큰거에서 작은거 뺀거다
                if( i_Y < m_iFHHeight - 1 )     // 오른쪽 위쪽에 밟을게 있나? 그럼 그것과 보간하자
                {
                    if( MAP_ARRAY[ i_X + 1 ][ i_Y + 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
                    {
                        iSideBlockY = i_Y + 1;
                    }
                }
                if( iSideBlockY == -1 && i_Y > 0 ) // 오른쪽 아래쪽에 밟을게 있나? 그럼 그것과 보간하자
                {
                    if( MAP_ARRAY[ i_X + 1 ][ i_Y - 1 ].GetFootHoldType() == foothold_type::FHT_HEAD )
                    {
                        iSideBlockY = i_Y - 1;
                    }
                }
            }
        }
    }
    // -------------------------------------------- 디디기 보간 대상 선택 완료-------------------------------------------

    if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldType() == foothold_type::FHT_HEAD )
    {
        if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT && kInfo_.m_bTagMotion == false )
        {
            kInfo_.m_x += 0.125f;

            if( kInfo_.m_xSpeed < 0.0f )
                kInfo_.m_xSpeed += 0.0075f;

            return false;
        }
        if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT && kInfo_.m_bTagMotion == false )
        {
            kInfo_.m_x += -0.125f;

            if( kInfo_.m_xSpeed > 0.0f )
                kInfo_.m_xSpeed -= 0.012f;

            return false;
        }

        if( (MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_DOWN) && kInfo_.m_bTagMotion == false )
        {
            kInfo_.m_cCheck = 0;
            if( iSideBlockY == -1 ) // 옆에 밟을 것이 없으므로 발 위치를 발판위(40%) 위치에 둔다.
            {
                kInfo_.m_y = MAP_ARRAY[ i_X ][ i_Y ].GetY() + FOOTHOLDTERM;
            }
            else
            {
                kInfo_.m_y = (float)MAP_ARRAY[ i_X ][ i_Y ].GetY() * (1.0f - fInterpolation) + (FOOTHOLDSIZE * iSideBlockY) * (fInterpolation) + FOOTHOLDTERM;
            }

            kInfo_.m_iBlockX = i_X;
            kInfo_.m_iBlockY = i_Y;
            return true;
        }
		else if ( (MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_UP) && kInfo_.m_bTagMotion == false )
		{
			return GetSafePosition( kInfo_.m_x, kInfo_.m_y, kInfo_.m_bTagMotion );
		}
        else
        {
            if( kInfo_.m_cCheck )           // 발판을 뚫고 다닐수 있는 상태라면...
                return GetSafePosition( kInfo_.m_x, kInfo_.m_y, kInfo_.m_bTagMotion );

            if( iSideBlockY == -1 ) // 옆에 밟을 것이 없으므로 발 위치를 발판위(40%) 위치에 둔다.
            {
                kInfo_.m_y = MAP_ARRAY[ i_X ][ i_Y ].GetY() + FOOTHOLDTERM;
            }
            else
            {   
                kInfo_.m_y = (float)MAP_ARRAY[ i_X ][ i_Y ].GetY() * (1.0f - fInterpolation) + (FOOTHOLDSIZE * iSideBlockY) * (fInterpolation) + FOOTHOLDTERM;
            }

            kInfo_.m_iBlockX = i_X;
            kInfo_.m_iBlockY = i_Y;
            return true;
        }
    }
    else
    {
        if( g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE && ( g_pkQuestManager->IsPlatformer() || ( kInfo_.m_bPlayer &&  g_pkQuestManager->IsFallingDie() ) ) )
            return false;
        return GetSafePosition( kInfo_.m_x, kInfo_.m_y, kInfo_.m_bTagMotion );
    }

    return false;
}

D3DXVECTOR2 KGCFantasticMap::GetPositionNear( float fX, float fY )
{
    D3DXVECTOR2 vecRet( 0.0f, -1.0f );
    if( m_ppFHWorld == NULL )
        return vecRet;

    if( m_bNothingFootHold )
        return vecRet;

    int iStartX, iStartY;
    iStartX = static_cast<int>( TO_INDEX(fX) );
    iStartY = static_cast<int>( TO_INDEX(fY) );
    BoundCheck( iStartX, iStartY );

    int iBlockX;
    int iBlockY;

    int iPlusCount = 0;
    while(true)
    {
        for(int y = -iPlusCount; y < 1+iPlusCount; y++ )
        {
            for(int x = -iPlusCount; x < 1+iPlusCount; x++)
            {
                
                if( y != -iPlusCount && y != iPlusCount && ( x != -iPlusCount && x != iPlusCount ) )
                {
                    continue;
                }
                iBlockX = iStartX + x;
                iBlockY = iStartY + y;

                // Index Error Check
                if( iBlockX < 0 || iBlockX > m_iFHWidth - 1 || iBlockY < 0 || iBlockY > m_iFHHeight - 1 )
                    continue;

                if( MAP_ARRAY[ iBlockX ][ iBlockY ].GetFootHoldType() == foothold_type::FHT_INIT )
                    continue;

                if( (   ( true == EnableMove( iBlockX - 1, iBlockY - 1 ) ) ||
                        ( true == EnableMove( iBlockX - 1, iBlockY + 0 ) ) ||
                        ( true == EnableMove( iBlockX - 1, iBlockY + 1 ) ) &&
                    (   ( true == EnableMove( iBlockX + 1, iBlockY - 1 ) ) ||
                        ( true == EnableMove( iBlockX + 1, iBlockY + 0 ) ) ||
                        ( true == EnableMove( iBlockX + 1, iBlockY + 1 ) ) ) ) )
                {
                    vecRet.x = (float)MAP_ARRAY[ iBlockX ][ iBlockY ].GetX();
                    vecRet.y = (float)MAP_ARRAY[ iBlockX ][ iBlockY ].GetY() + FOOTHOLDTERM;
                    return vecRet;
                }
            }

        }
        iPlusCount++;
    }
}

bool KGCFantasticMap::IsExistNextBlock( const KNextBlockCheck& kNextCheck )
{
    int iIndexX, iIndexY;
    iIndexX = static_cast<int>( TO_INDEX(kNextCheck.fX) );
    iIndexY = static_cast<int>( TO_INDEX(kNextCheck.fY) );
 

    if( kNextCheck.bIsRight )
        ++iIndexX;
    else
        --iIndexX;

	BoundCheck( iIndexX, iIndexY );

    if( iIndexY > m_iFHHeight - 2 )
        iIndexY = m_iFHHeight - 2;


    // 좌우에 벽에 닿았을때.. 넘어가지 않게 해 준다 ( 바운딩 박스의 좌우 55%의 크기가 벽밖으로 넘어가지 않게 한다 )
    if( kNextCheck.fX > m_fWidth - ( kNextCheck.fWidth * 0.55f ) ||
        kNextCheck.fX < ( kNextCheck.fWidth * 0.55f ))
    {
        return false;
    }

	bool bRet = (MAP_ARRAY[iIndexX][iIndexY + 1].GetFootHoldType()) ||
				(MAP_ARRAY[iIndexX][iIndexY ].GetFootHoldType()) ||
				(MAP_ARRAY[iIndexX][iIndexY - 1].GetFootHoldType());

    if( kNextCheck.bIsRight )
    {
        bRet &= !( MAP_ARRAY[iIndexX][iIndexY + 1].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT ) && 
               !( MAP_ARRAY[iIndexX][iIndexY].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT ) && 
               !( MAP_ARRAY[iIndexX][iIndexY - 1].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT );
			   
    }
    else
    {
        bRet &= !( MAP_ARRAY[iIndexX][iIndexY + 1].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT ) &&
               !( MAP_ARRAY[iIndexX][iIndexY].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT ) &&
               !( MAP_ARRAY[iIndexX][iIndexY - 1].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT );
    }

    return bRet;
}

void KGCFantasticMap::Render( int iLayerNum_ )
{
    SiKGCLayerManager()->NoAlphaRender( iLayerNum_ );

//#ifdef _DEBUG
//    if( m_pDebugFH != NULL )
//    {
//        SiKGCRenderer()->AddRenderObj( static_cast<IGCRenderObj*>(m_pDebugFH) );
//    }
//#endif
}

bool KGCFantasticMap::EnableMove( int iIndexX, int iIndexY )
{
    if( m_ppFHWorld == NULL )
        return false;

    if( iIndexX < 0 || iIndexX > m_iFHWidth - 1 )
        return false;

    if( iIndexY < 0 || iIndexY > m_iFHHeight - 1 )
        return false;
    
    if( MAP_ARRAY[iIndexX][iIndexY].GetFootHoldType() == foothold_type::FHT_INIT )
        return false;

    return true;
}

bool KGCFantasticMap::CrashCheck( float fX, float fY, D3DXVECTOR2& dxvRet /*= D3DXVECTOR2(0.f,-1.f)*/, DWORD dwProperty/* = 0x11111111*/ )
{
    dxvRet = D3DXVECTOR2(0.f, -1.f);
    if( m_ppFHWorld == NULL ) {
        return false;
    }

    int i_X, i_Y;
    i_X = static_cast<int>( TO_INDEX(fX) );
    i_Y = static_cast<int>( TO_INDEX(fY) );
    return CrashCheck_Index(i_X, i_Y, dxvRet, dwProperty );
}

bool  KGCFantasticMap::CrashCheck_Index( int i_X, int i_Y, D3DXVECTOR2& dxvRet, DWORD dwProperty )
{
    dxvRet = D3DXVECTOR2(0.0f, -1.0f);
    BoundCheck( i_X, i_Y );

	if ( dwProperty != 0x11111111 )
	{
		if ( !(MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldProp() & dwProperty) )
            return false;
	}
	
    if( MAP_ARRAY[ i_X ][ i_Y ].GetFootHoldType() == foothold_type::FHT_INIT )
    {
        if( !m_vecLastButtom.empty() && i_Y < m_vecLastButtom[i_X].second && m_vecLastButtom[i_X].second< m_iFHHeight )
        {
            i_Y = m_vecLastButtom[i_X].second;
        }
        else
        {
            return false;
        }
    }

//SKIP_CHECK:
    dxvRet.x = MAP_ARRAY[ i_X ][ i_Y ].GetX();
    // [8/29/2007 breadceo] 0.02 = FOOHOLDSIZE * FHTERM
    dxvRet.y = MAP_ARRAY[ i_X ][ i_Y ].GetY() + FOOTHOLDTERM;

    return true;
}

float KGCFantasticMap::IndexBetweenCheck( int StartindexX, int EndindexX, int YIndex, DWORD dwProp/* = foothold_type::FHP_JUST_FOOTHOLD*/ )
{
    if( m_ppFHWorld == NULL )
        return 0.0f;

    BoundCheck( StartindexX, YIndex );
    BoundCheck( EndindexX, YIndex );

    if( YIndex > m_iFHHeight - 2 )
        YIndex = m_iFHHeight - 2;

    if( dwProp & foothold_type::FHP_NO_COME_RIGHT )
    {
        for( int i = EndindexX; i >= StartindexX; --i )
        {
            if( MAP_ARRAY[ i ][ YIndex + 1 ].GetFootHoldType() == foothold_type::FHT_INIT )
                continue;

            if( MAP_ARRAY[ i ][ YIndex + 1 ].GetFootHoldProp() & dwProp || dwProp == foothold_type::FHP_JUST_FOOTHOLD )
            {
                return (float)(i * FOOTHOLDSIZE);
            }
        }
    }
    else if( dwProp & foothold_type::FHP_NO_COME_LEFT )
    {
        for( int i = StartindexX; i <= EndindexX; ++i )
        {
            if( MAP_ARRAY[ i ][ YIndex + 1 ].GetFootHoldType() == foothold_type::FHT_INIT )
                continue;

            if( MAP_ARRAY[ i ][ YIndex + 1 ].GetFootHoldProp() & dwProp || dwProp == foothold_type::FHP_JUST_FOOTHOLD )
            {
                return (float)(i * FOOTHOLDSIZE);
            }
        }
    }


    return -1.0f;
}

float KGCFantasticMap::IndexBetweenYCheck( int XIndex, int StartindexY, int EndindexY )
{
    if( m_ppFHWorld == NULL )
        return 0.0f;

    BoundCheck( XIndex, StartindexY );
    BoundCheck( XIndex, EndindexY );

    // Y축 범위보정
    if( m_iFHHeight <= StartindexY )
        StartindexY = m_iFHHeight - 1;
    if( m_iFHHeight <= EndindexY )
        EndindexY = m_iFHHeight - 1;
    if( 0 > StartindexY )
        StartindexY = 0;
    if( 0 > EndindexY )
        EndindexY = 0;
    
    // X축 범위보정
    if( 0 > XIndex )
        XIndex = 0;
    if( m_iFHWidth <= XIndex )
        XIndex = m_iFHWidth - 1;

    // 검사루프를 위한 크기반전
    if( StartindexY > EndindexY )
    {
        int iTemp = StartindexY;
        StartindexY = EndindexY;
        EndindexY = StartindexY;
    }

    // 발판 검사
    for( int iLoop = StartindexY; iLoop <= EndindexY; ++iLoop )
    {
        if( MAP_ARRAY[ XIndex ][ iLoop ].GetFootHoldType() == foothold_type::FHT_INIT )
            continue;

        if( foothold_type::FHP_NO_JUMP_UP & MAP_ARRAY[ XIndex ][ iLoop ].GetFootHoldProp() )
        {
            return (float)(iLoop * FOOTHOLDSIZE);
        }
    }

    return -1.0f;
}

void KGCFantasticMap::FrameMove( float fElapsedTime )
{
    SiKGCLayerManager()->FrameMove( fElapsedTime );
}

void KGCFantasticMap::Water_FrameMove( void )
{
    if( IsWaterStage() == true && m_pWave != NULL )
    {
        m_pWave->FrameMove();

        // 퀘스트 다이얼로그 상태일때는 산소게이지 깎지마라
        if( ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() &&
            !g_pkGameOverlayUI->IsShowDialogue() &&
            !g_pkQuestManager->m_bClearStage ) ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
        {
            m_pkO2Manager->FrameMove( GC_ELAPSED_TIME );
        }
    }
}

bool KGCFantasticMap::IsLastBottomPosition( float fX, float fY )
{
    if( m_ppFHWorld == NULL )
        return false;

    if( m_vecLastButtom.empty() )
        return false;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) ) - 1;
    BoundCheck( iIndexX, iIndexY );

    std::vector< std::pair< int, int > >::iterator vit;
    vit = std::find( m_vecLastButtom.begin(), m_vecLastButtom.end(),
        std::make_pair( iIndexX, iIndexY ) );

    return (vit != m_vecLastButtom.end() || iIndexY < m_vecLastButtom[iIndexX].second );
}

float KGCFantasticMap::GetLastBottomPosition( float fX, float fY )
{
    if( m_vecLastButtom.empty() )
        return -10.0f;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) );
    BoundCheck( iIndexX, iIndexY );

    return m_vecLastButtom[iIndexX].second * FOOTHOLDSIZE;
}

bool KGCFantasticMap::IsAvailableGoDown( float fX, float fY )
{
    if( m_ppFHWorld == NULL )
        return false;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) );
    BoundCheck( iIndexX, iIndexY );

    if( !m_vecLastButtom.empty() && m_vecLastButtom[iIndexX].second == iIndexY )
        return false;

    if( MAP_ARRAY[ iIndexX ][ iIndexY ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_DOWN )
    {
            return false;
    }
    return true;
}

bool KGCFantasticMap::IsAvailableGoSide( float fX, float fY )
{
    if( m_ppFHWorld == NULL )
        return false;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) );
    int iLeftX = iIndexX - 1;
    int iRightX = iIndexX + 1;

    BoundCheck( iLeftX, iIndexY );
    BoundCheck( iRightX, iIndexY );

    if( MAP_ARRAY[ iLeftX ][ iIndexY ].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT || 
        MAP_ARRAY[ iRightX ][ iIndexY ].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT )
    {
        return false;
    }
    return true;
}

bool KGCFantasticMap::IsAvailableJumpUp( float fX, float fY )
{
    if( m_ppFHWorld == NULL )
        return false;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) );
    BoundCheck( iIndexX, iIndexY );

    if( MAP_ARRAY[ iIndexX ][ iIndexY ].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_UP )
    {
        return false;
    }
    return true;
}

bool KGCFantasticMap::IsDeadBlock( float fX, float fY )
{
	if( m_ppFHWorld == NULL )
		return false;

	int iIndexX = static_cast<int>( TO_INDEX( fX ) );
	int iIndexY = static_cast<int>( TO_INDEX( fY ) );
	BoundCheck( iIndexX, iIndexY );

	if( !m_vecLastButtom.empty() && m_vecLastButtom[iIndexX].second == iIndexY )
		return false;

    if( MAP_ARRAY[ iIndexX ][ iIndexY ].GetFootHoldProp() & foothold_type::FHP_DEAD_FOOTHOLD )
	{
			return true;
		}
	return false;
}


bool KGCFantasticMap::GetSafePosition( float& fX, float& fY, bool bTagMotion )
{
    int iIndexX = (int)( TO_INDEX(fX) );
    int iIndexY = (int)( TO_INDEX(fY) );
    BoundCheck( iIndexX, iIndexY );

    if( IsLastBottomPosition( fX, fY ) == true && false == bTagMotion )
    {
        fY = m_vecLastButtom[iIndexX].second * FOOTHOLDSIZE + FOOTHOLDTERM;
        return true;
    }
    else
        return false;
}

float KGCFantasticMap::FindShadowPos( float fX, float fY )
{
    int iBlockX,iBlockY;
    iBlockX = (int)( TO_INDEX(fX) );
    iBlockY = (int)( TO_INDEX(fY) );
    BoundCheck( iBlockX, iBlockY );
    
    if( iBlockY + 2 > m_iFHHeight )
        iBlockY = m_iFHHeight - 2;

    for(int y = iBlockY+1; y >= 0; y--)
    {
        if( MAP_ARRAY[ iBlockX ][ y ].GetFootHoldType() == foothold_type::FHT_HEAD )
        {
            float fBlockY = MAP_ARRAY[ iBlockX ][ y ].GetY();
            return (float)(fBlockY+(FOOTHOLDSIZE * 0.5f)/* - FOOTHOLDSIZE*0.5f*/); // 한블럭 위에 서 있으므로.. 이렇게 해줍시다.;
        }
    }

    // [8/31/2007 breadceo] 말도 안되는 값
    return -10.0f;
}

void KGCFantasticMap::LoadBaseData( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("BASE_DATA", return )
    {
        int Width, Height;
        LUA_GET_VALUE("WORLD_WIDTH", Width, return );
        LUA_GET_VALUE("WORLD_HEIGHT", Height, return );

        SetWorld( Width, Height );

        int iLayerTot, iGamePlayLayer;
        LUA_GET_VALUE("LAYERTOTAL", iLayerTot, return );
        LUA_GET_VALUE("GAMEPLAYLAYER", iGamePlayLayer, return );

        SiKGCLayerManager()->SetMaxLayerNum( iLayerTot );
        SiKGCLayerManager()->SetGamePlayLayer( iGamePlayLayer );
        SiKGCLayerManager()->SetLayer();
    }
    LUA_END_TABLE( return );
}

void KGCFantasticMap::LoadFootHoldData( KLuaManager& luaMgr )
{
    if( m_ppFHWorld == NULL )
        return ;

    m_bNothingFootHold = true;
    KMap<KGCFootHold>::LoadFootHoldData( luaMgr );

    m_vecLastButtom.clear();

    for( int i = 0; i < m_iFHWidth; ++i )
	{
		int j = 0;
        for( j = 0; j < m_iFHHeight; ++j )
        {
            if( m_ppFHWorld[i][j].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_DOWN )
            {
                m_vecLastButtom.push_back( std::make_pair( m_ppFHWorld[i][j].GetIndexX(), m_ppFHWorld[i][j].GetIndexY() ) );
                    break;
                }
            }

        if( j == m_iFHHeight )
        {
            m_vecLastButtom.push_back( std::make_pair( i, -1 ) );
        }
    }

    ASSERT( (int)m_vecLastButtom.size() == m_iFHWidth );
}

void KGCFantasticMap::SetWorld( int iWidth, int iHeight )
{
    // Jaeho 2007.09.21
    // m_iFHWidth가 변경되면 UnLoad 함수 메모리 해지 할때 문제 생겨서 한번 해줬어요~
    UnLoad();

    m_iFHWidth = iWidth;
    m_iFHHeight = iHeight+1;

    m_fWidth = iWidth * FOOTHOLDSIZE;
    m_fHeight = iHeight * FOOTHOLDSIZE;

    Load();
}

void KGCFantasticMap::CreateWater()
{
    // 리소스 한번 해제 하고..
    SAFE_RELEASE( m_pTexWater );
    SAFE_DELETE( m_pkO2Manager );
    SAFE_DELETE( m_pWave );

    m_pkO2Manager = new KGCO2Manager;
    m_pWave = new CWave();

    m_pkO2Manager->Create();
    m_pTexWater = g_pGCDeviceManager2->CreateTexture( "WATER.DDS" );

    m_pWave->Init( m_fWaterHeight );

    m_pkO2Manager->SetWaterHeight( m_fWaterHeight );
    SetO2Region();

    BYTE Alpha = 255;

    V_Water[0].SetSLVER( 0.0f             ,-0.75f + m_fWaterHeight,  0, 0,     0);
    V_Water[1].SetSLVER( 1.5f * GetBlockWidth() ,-0.75f + m_fWaterHeight,  0, 1.0f,  0);
    V_Water[2].SetSLVER( 0.0f             ,-0.75f + 0.0f,  0, 0,     1.0f);
    V_Water[3].SetSLVER( 1.5f * GetBlockWidth() ,-0.75f + 0.0f,  0, 1.0f,  1.0f);
    V_Water[0].Diff = V_Water[1].Diff = V_Water[2].Diff = V_Water[3].Diff = D3DCOLOR_ARGB(Alpha,255,255,255);

	if ( SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST28 )
		g_pkGameOverlayUI->EnableO2Bar( true );
}

void KGCFantasticMap::SetWaterHeight( float fHeight )
{
    m_fWaterHeight = fHeight;
}

void KGCFantasticMap::Water_Render( void )
{
    if( m_pTexWater == NULL )
        return;

    D3DXMATRIX mat1, mat2;

    //물그리기.. Texture가 NULL인지 검사해서 그리는 루틴이라...
    //따로.. if(Stage == ..)하지 않음
    g_pGCDeviceManager2->PushState();
    {

        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );        

        D3DXMatrixTranslation( &mat2, (-1.0f + 1.5f * 0.0f), (0.26f + 1.5f * (float)0.0f), 0.5f );
        D3DXMatrixMultiply( &mat1, &mat2, &g_kCamera.m_matCamera );

		g_pGCDeviceManager2->DrawInWorld(m_pTexWater,&mat1,V_Water.as_array());

        g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_kCamera.m_matCamera );
        if( m_pWave != NULL )
            m_pWave->Render();
    }
    g_pGCDeviceManager2->PopState();	
}

bool KGCFantasticMap::IsWaterStage()
{
    if( m_fWaterHeight == 0.0f )
        return false;

    return true;
}

void KGCFantasticMap::SetO2Region()
{
    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            g_MyD3D->MyPlayer[i]->m_fO2 = MAX_PLAYER_O2;
        }
    }
}

void KGCFantasticMap::ClearWater(void)
{
    g_pkGameOverlayUI->EnableO2Bar( false );

    SAFE_RELEASE( m_pTexWater );
    SAFE_DELETE( m_pkO2Manager );
    SAFE_DELETE( m_pWave );

    m_fWaterHeight = 0.0f;
}

void KGCFantasticMap::LoadMatchMap( BYTE *Field_Information, char Num_Width, char Num_Height )
{
    if( Field_Information == NULL )
        return;

    SetWorld( Num_Width * 30, Num_Height * 30 );
    DWORD dwProp = 0;
    for( int x = 0 ; x < Num_Width*30 ; ++x ) {
        for( int y = 0 ; y < Num_Height*30 ; ++y ) {

            if( 0 == Field_Information[x+y*30*Num_Width] )
                continue;

            dwProp = foothold_type::FHP_JUST_FOOTHOLD;

            if( 1 == Field_Information[x+y*30*Num_Width] )
                dwProp += foothold_type::FHP_NO_JUMP_DOWN;
            
            AddDungeonFootHoldByValue( x * 0.05f, y * 0.05f, dwProp, NULL, "" );
        }
    }

    m_vecLastButtom.clear();

    for( int i = 0; i < m_iFHWidth; ++i )
    {
        int j = 0;
        for( j = 0; j < m_iFHHeight; ++j )
        {
            if( m_ppFHWorld[i][j].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_DOWN )
            {
                m_vecLastButtom.push_back( std::make_pair( m_ppFHWorld[i][j].GetIndexX(), m_ppFHWorld[i][j].GetIndexY() ) );
                break;
            }
        }

        if( j == m_iFHHeight )
        {
            m_vecLastButtom.push_back( std::make_pair( i, -1 ) );
        }
    }

    ASSERT( (int)m_vecLastButtom.size() == m_iFHWidth );
}

bool KGCFantasticMap::IsValidBlock( float fX, float fY, D3DXVECTOR2& dxvRet /*= D3DXVECTOR2(0.f,-1.f)*/ )
{
    if( m_ppFHWorld == NULL )
        return false;

    int iIndexX = static_cast<int>( TO_INDEX( fX ) );
    int iIndexY = static_cast<int>( TO_INDEX( fY ) );

    return IsVaildBlockIndex( iIndexX, iIndexY, dxvRet );
    //if( iIndexY >= m_vecLastButtom[ iIndexX ].second )
    //    return true;
    //else
    //    return false;
}

bool KGCFantasticMap::IsVaildBlockIndex( int iX, int iY, D3DXVECTOR2& dxvRet /*= D3DXVECTOR2(0.f,-1.f)*/ )
{
    BoundCheck( iX, iY );

    if( MAP_ARRAY[ iX ][ iY ].GetFootHoldType() == foothold_type::FHT_INIT ) 
    {
        if( !m_vecLastButtom.empty() && iY < m_vecLastButtom[iX].second && m_vecLastButtom[iX].second< m_iFHHeight )
        {
            iY = m_vecLastButtom[iX].second;
        }
        else if( MAP_ARRAY[ iX ][ iY].GetFootHoldProp() != foothold_type::FHP_NO_ACTION )
            return false;
    }

    dxvRet.x = MAP_ARRAY[ iX ][ iY ].GetX();
    dxvRet.y = MAP_ARRAY[ iX ][ iY ].GetY() + FOOTHOLDTERM;

    return true;
}

D3DXVECTOR2 KGCFantasticMap::CheckNearFootHold( int iBlockType, float fX, float fY, int iOffsetX, int iOffsetY, bool bRight )
{
    int iStartX = static_cast<int>( TO_INDEX( fX ) );
    int iStartY = static_cast<int>( TO_INDEX( fY ) );
    int iEndX, iEndY;

    BoundCheck( iStartX, iStartY );
    
    // X축 End찾기
    iEndX = ( bRight ? m_iFHWidth : 0 );
    for(int iLoop=iStartX; ( ( bRight ) ? (iLoop < m_iFHWidth) : (iLoop >= 0) ); ( bRight ? ++iLoop : --iLoop ) )
    {
        if( MAP_ARRAY[ iLoop ][ iStartY ].GetFootHoldType() != foothold_type::FHT_INIT )
        continue;

        iEndX = ( bRight ? iLoop-2 : iLoop+2 );
        break;
    }

    // Y축 Start찾기
    for( int iLoop = iStartY; iLoop >= 0; --iLoop )
    {
        if( MAP_ARRAY[ iStartX ][ iLoop ].GetFootHoldType() == foothold_type::FHT_INIT )
            continue;

        // 현재 서있는 블럭보다 2높거나 낮은 블럭부터
        iStartY = (iOffsetY < 0) ? (iLoop - 2) : (iLoop + 2);
        break;
    }

    // Offset 적용
    iEndX = ( bRight ? iEndX + iOffsetX : iEndX - iOffsetX );
    iEndY = iStartY + iOffsetY;

    if(iStartY > iEndY)
    {
        int iTemp = iStartY;
        iStartY = iEndY;
        iEndY = iTemp;
    }

    BoundCheck( iStartX, iStartY );
    BoundCheck( iEndX, iEndY );

    // 가까운 발판이 어딧는지 보자...
    float fNear = 999999.0f;
    int iNearX = 0, iNearY = 0;
    for(int iLoopY = iStartY; iLoopY<iEndY; ++iLoopY)
    {
        for(int iLoopX = iStartX; ( bRight ) ? (iLoopX <= iEndX) : (iLoopX >= iEndX); ( bRight ? ++iLoopX : --iLoopX ) )
        {
            if( MAP_ARRAY[ iLoopX ][ iLoopY ].GetFootHoldType() != foothold_type::FHT_INIT )
            {
                D3DXVECTOR2 vPos = D3DXVECTOR2( MAP_ARRAY[ iLoopX ][ iLoopY ].GetX(), MAP_ARRAY[ iLoopX ][ iLoopY ].GetY() );

                vPos.x = bRight ? vPos.x - fX : fX - vPos.x;
                vPos.y = (iOffsetY < 0) ? fY - vPos.y : vPos.y - fY;
                
                float fLength = D3DXVec2Length( &vPos );
                if( fNear > fLength )
                {
                    fNear = fLength;
                    iNearX = iLoopX;    iNearY = iLoopY;
                }
            }
        }
    }

    if( 0 == iNearX && 0 == iNearY )
        return D3DXVECTOR2( 0.0f, 0.0f );

    return D3DXVECTOR2( MAP_ARRAY[ iNearX ][ iNearY ].GetX(), MAP_ARRAY[ iNearX ][ iNearY ].GetY() );
}

void KGCFantasticMap::SetLayerMoveSpeed( int iLayer, char *szPartName, float fSpeedX, float fSpeedY )
{
    SiKGCLayerManager()->SetLayerMoveSpeed(iLayer, szPartName, fSpeedX, fSpeedY);
}

float KGCFantasticMap::GetLayerMoveSpeedX( int iLayer, char *szPartName )
{
    return SiKGCLayerManager()->GetLayerMoveSpeedX(iLayer, szPartName);
}

float KGCFantasticMap::GetLayerMoveSpeedY( int iLayer, char *szPartName )
{
    return SiKGCLayerManager()->GetLayerMoveSpeedY(iLayer, szPartName);
}