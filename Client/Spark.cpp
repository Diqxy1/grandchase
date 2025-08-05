#include "stdafx.h"
#pragma warning(disable:4786)

//
//



#include "Spark.h"
#include "Monster.h"

#include "GCCameraOrtho.h"




// ------------------------------------------------------------------------------------------
// 인라인 함수 선언
// ------------------------------------------------------------------------------------------
inline void SetDiffuse( SLVERTEX* v, BYTE a = 0xff, BYTE r = 0xff, BYTE g = 0xff, BYTE b = 0xff )
{
    v[0].Diff = v[1].Diff = v[2].Diff = v[3].Diff = D3DCOLOR_ARGB( a, r, g, b );
}

// ------------------------------------------------------------------------------------------
// 생성자 및 소멸자
// ------------------------------------------------------------------------------------------

/// Default constructor
KEffector::KEffector( void )
{
	for( int i=0 ; i<NUM_SPARK ; ++i )
		m_pTexSpark[i] = NULL;
}

/// Default destructor
KEffector::~KEffector( void )
{
    // empty
	RemoveAll();
}

bool KEffector::CheckEnableCreate()
{
	m_iTotalSpark = 0;
	std::map< int, std::vector<Effect*> >::iterator itor;
	for( itor=m_mapSpark.begin() ; itor!=m_mapSpark.end() ; ++itor )
	{
		m_iTotalSpark += (int)itor->second.size();
	}

	if( m_iTotalSpark >= MAX_SPARK_INSTANCE )
		return false;

    return true;
}

// ------------------------------------------------------------------------------------------
// 디바이스 관련 함수
// ------------------------------------------------------------------------------------------
void KEffector::Load()
{
	char szTextureFileName[50];

    for( int i=0 ; i<NUM_SPARK ; ++i )
    {
		sprintf(szTextureFileName,"Spark%02d.dds",i);
		m_pTexSpark[i] = g_pGCDeviceManager2->CreateTexture( szTextureFileName );
    }

	/************************************************************************/
	/*	Load 할때 VB, IB가 없다면 복구
	/************************************************************************/
	/*if( NULL == m_pVB )
		m_pVB->OnResetDevice();

	if( NULL == m_pIB )
		m_pIB->OnResetDevice();*/
}

void KEffector::DeleteDevice()
{
	UnLoad();
}

/************************************************************************/
/*	Create 할때 한번 불리는 부분
/************************************************************************/
//void KEffector::OnResetDevice( void )
void KEffector::InitDevice( void )
{
	
}

/// 모든 텍스쳐 해제
void KEffector::UnLoad()
{
	for( int i=0 ; i<NUM_SPARK ; ++i )
	{
		SAFE_RELEASE(m_pTexSpark[i]);
	}
}

// ------------------------------------------------------------------------------------------
// 이펙트 추가 함수
// ------------------------------------------------------------------------------------------
Effect* KEffector::Add( int What,
                     float x,
                     float y,
                     int Delay,
                     float Scale    /* = 1.0f */,
                     float Target_x /* = -100.0f */,
                     float Target_y /* = -100.0f */,
                     float Angle    /* = 0.0f */,
                     bool bIsRight  /* = false */,
					 int who )
{
	if( CheckEnableCreate() == false )
		return NULL;

	if( who >= 0 && who < MAX_PLAYER_NUM )
	{
		if( HIT_LIGHT == What || PARTICLE == What )
		{
			if( g_MyD3D->MyPlayer[who]->m_Elemental == ITEM_ELEMENTAL_FIRE
				&& g_MyD3D->MyPlayer[who]->m_StrongLevel >= 5 )
			{
				return NULL;
			}
		}
	}

    if ( HIT_LIGHT == What )
	{
		Bim( x, y, Delay );
	}

#if defined(RESOURCE_GRAPH)
	char tmp1[256];		
	sprintf(tmp1,"SPARK_%d",What);			
	g_pGCDeviceManager2->AddRelation(tmp1,"STATE_"+g_MyD3D->m_pStateMachine->GetStateString());	
#endif
	// 이펙트 객체 추가
    Effect* pEffect = new Effect;
	m_mapSpark[What].push_back( pEffect );

	// 이펙트 정보 세팅
    switch ( What )
    {
        case FOUNTAIN_HEAD0:
        case FOUNTAIN_HEAD1:
        case FOUNTAIN_HEAD2:
        {
            pEffect->What     = What;
            pEffect->x        = x;
            pEffect->y        = y;
            pEffect->Target_x = x;
            pEffect->Target_y = y;
            pEffect->Scale    = 1.5f;
            pEffect->Delay    = Delay;
            pEffect->Angle    = 0.0f;
            pEffect->Life     = 1;
            return NULL;
        }
    }

    pEffect->What    = What;
    pEffect->x       = x;
    pEffect->y       = y;
    pEffect->Scale   = Scale;
    pEffect->Delay   = Delay;
    pEffect->IsRight = bIsRight;

    if( -100.0f == Target_x )
    {
        pEffect->Target_x = x;
        pEffect->Target_y = y;
    }
    else
    {
        pEffect->Target_x = Target_x;
        pEffect->Target_y = Target_y;
    }

    if( HIT_LIGHT == What  ) 
		pEffect->Angle = 2.0f * D3DX_PI;
    else if( 0.0f == Angle ) 
		pEffect->Angle = 2.0f * D3DX_PI * randf();
    else
		pEffect->Angle = Angle;

    switch( pEffect->What )
    {
        case LIGHTNING0: case LIGHTNING1: case LIGHTNING2: case LIGHTNING3:
        case LIGHTNING4: case LIGHTNING5: case LIGHTNING6: case LIGHTNING7:
		case BLUE_LIGHTNING0: case BLUE_LIGHTNING1: case BLUE_LIGHTNING2: case BLUE_LIGHTNING3:
		case BLUE_LIGHTNING4: case BLUE_LIGHTNING5: case BLUE_LIGHTNING6: case BLUE_LIGHTNING7:
        {
            pEffect->Life = 32;
            break;
        }
        case ENERGY_BEAM0: case ENERGY_BEAM1: case ENERGY_BEAM2:
		case NEPHILIM_ATK3:
        {
            pEffect->Life = 6;
            break;
        }
        case LANCE_MAGIC_CIRCLE:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 10;
            break;
        }
        case LANCE_MOVE_CIRCLE:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 20;
            break;
        }
        case LANCE_SPRAY:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 2;
            break;
        }
        case ANGEL: case ANGEL_EGG_PROP: case ANGEL_EGG_BANG:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 1;
            break;
        }
        case ANGEL_EGG:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 6;
            break;
        }
        case ARROW:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 50;
            break;
        }
        case INTO_THE_DARKNESS:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 90;
            break;
        }
        case ONE_KILL:
        {
            pEffect->Angle = 0.0f;
            pEffect->Life = 40;
            break;
        }
        case ONE_KILL_FLASH:
        {
            pEffect->Angle = Angle;
            pEffect->Life = 20;
            break;
        }
        case FLOAT_SOUL:
        {
            unsigned char Index = LATENCY+SHOWLATENCY-1;

            // TODO : 영혼 라이프 초기화...
      //      for( int i=0 ; i<MAX_MONSTER ; ++i )
      //      {
      //          if(g_kMonsterManager.m_mapMonster[i]->m_bLive && g_kMonsterManager.m_mapMonster[i]->Now_Monster == MON_GADOSEN)
      //          {
      //              if(g_kMonsterManager.m_mapMonster[i]->m_abIsRight[Index])
      //                  pEffect->Target_x=g_kMonsterManager.m_mapMonster[i]->m_afX[Index]-.84f;
      //              else 
						//pEffect->Target_x=g_kMonsterManager.m_mapMonster[i]->m_afX[Index]-1.16f;

      //              pEffect->Target_y=g_kMonsterManager.m_mapMonster[i]->m_afY[Index]+.25f;

      //              break;
      //          }
      //      }
            pEffect->Life = 50;
            break;
        }
		case MON_RISEN:
			{
				pEffect->m_IsBlendOne = true;
				pEffect->Life = 50;
				pEffect->Angle = 0.0f;
			}
			break;
		case DARK_SKULL:
			pEffect->Life = 255;
			pEffect->Angle = 0;
			break;
		case NEPHILIM_ATK1:
		case NEPHILIM_ATK2:
			pEffect->Life = 15;
			pEffect->Angle = 0;
			break;
        default:
        {
            pEffect->Life = 20;
            break;
        }
    }

    return pEffect;
}

void KEffector::Bim( float x, float y, BYTE Delay, int iWhat_ )
{
    for ( int i = 0; i < 5; i++ )
    {
        float ang   = 2.0f * D3DX_PI * randf();
        float len   = 0.2f + 0.2f * randf();
        float randX = x - 0.5f * len * ::sinf( ang );
        float randY = y + 0.5f * len * ::cosf( ang );

        Add( iWhat_, randX, randY, Delay, 2.0f * len, randX, randY, ang );
    }
}

void KEffector::LightBim( float x, float y, BYTE Delay )
{
    for ( int i = 0; i < 30; i++ )
    {
        float ang   = 2.0f * D3DX_PI * randf();
        float len   = 1.0f + 1.0f * randf();
        float randX = x - 0.4f * len * ::sinf( ang );
        float randY = y + 0.4f * len * ::cosf( ang );

        Add( ENERGY_BEAM0, randX, randY, Delay, 2.0f * len, randX, randY, ang );
    }
}

void KEffector::Smogs( float x, float y, int Delay, bool Round )
{
    if(Round){
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.2f * len * ::cosf( ang );
            float randOrgY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f)-.1f;
            float randScal = 0.2f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + len * ( ::sinf( ang ) + 1.0f )-.1f;
            
            Add( DUST, randOrgX, randOrgY, Delay, randScal, randTgtX, randTgtY );
        }
    }else{    
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.5f * len * ::cosf( ang );
            float randScal = 0.2f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f );

            Add( DUST, randOrgX, y, Delay, randScal, randTgtX, randTgtY );
        }
    }
}

void KEffector::WaterSmogs( float x, float y, BYTE Delay, bool Round )
{
    if(Round)
	{
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.2f * len * ::cosf( ang );
            float randOrgY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f)-.1f;
            float randScal = 0.2f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + len * ( ::sinf( ang ) + 1.0f )-.1f;
            
            Add( WATER_DUST, randOrgX, randOrgY, Delay, randScal, randTgtX, randTgtY );
        }
    }
	else
	{    
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.5f * len * ::cosf( ang );
            float randScal = 0.2f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f );
			
            Add( WATER_DUST, randOrgX, y, Delay, randScal, randTgtX, randTgtY );
        }
    }
}

void KEffector::LeafSmogs( float x, float y, BYTE Delay, bool Round )
{
	if(Round)
	{
		for ( int i = 0; i < 10; i++ )
		{
			float ang      = 2.0f * D3DX_PI * randf();
			float len      = 0.2f * randf();
			float randOrgX = x + 0.2f * len * ::cosf( ang );
			float randOrgY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f)-.1f;
			float randScal = 0.1f + 0.3f * randf();
			float randTgtX = x + len * ::cosf( ang );
			float randTgtY = y + len * ( ::sinf( ang ) + 1.0f )-.1f;

			Add( LEAF_DUST, randOrgX, randOrgY, Delay, randScal, randTgtX, randTgtY );
		}
	}
	else
	{    
		for ( int i = 0; i < 10; i++ )
		{
			float ang      = 2.0f * D3DX_PI * randf();
			float len      = 0.2f * randf();
			float randOrgX = x + 0.5f * len * ::cosf( ang );
			float randScal = 0.1f + 0.3f * randf();
			float randTgtX = x + len * ::cosf( ang );
			float randTgtY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f );

			Add( LEAF_DUST, randOrgX, y, Delay, randScal, randTgtX, randTgtY );
		}
	}
}

void KEffector::UndeadSmogs( float x, float y, BYTE Delay, bool Round )
{
    if(Round)
    {
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.2f * len * ::cosf( ang );
            float randOrgY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f)-.1f;
            float randScal = 0.1f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + len * ( ::sinf( ang ) + 1.0f )-.1f;

            Add( UNDEAD_WALK, randOrgX, randOrgY, Delay, randScal, randTgtX, randTgtY );
        }
    }
    else
    {    
        for ( int i = 0; i < 10; i++ )
        {
            float ang      = 2.0f * D3DX_PI * randf();
            float len      = 0.2f * randf();
            float randOrgX = x + 0.5f * len * ::cosf( ang );
            float randScal = 0.1f + 0.3f * randf();
            float randTgtX = x + len * ::cosf( ang );
            float randTgtY = y + 0.2f * len * ( ::sinf( ang ) + 1.0f );

            Add( UNDEAD_WALK, randOrgX, y, Delay, randScal, randTgtX, randTgtY );
        }
    }
}

void KEffector::Light( float x, float y, BYTE Delay, float fScale /* = 1.0f */ )
{
    if ( 0.5f > randf() )
    {
        Add( LIGHTNING0, x, y, Delay, 0.3f + 0.3f * randf(), x, y );
    }

    Add( LIGHTNING4, x, y, Delay, fScale * ( 0.5f + 0.5f * randf() ), x, y );
}

void KEffector::BlueLight( float x, float y, BYTE Delay, float fScale )
{
	if ( 0.5f > randf() )
	{
		Add( BLUE_LIGHTNING0, x, y, Delay, 0.3f + 0.3f * randf(), x, y );
	}

	Add( BLUE_LIGHTNING4, x, y, Delay, fScale * ( 0.5f + 0.5f * randf() ), x, y );
}

void KEffector::Particles( float x, float y, BYTE Delay, BYTE Num, BYTE Scale, int who , int iEffectKind )
{
	for ( int i=0 ; i<Num ; ++i )
	{
		float ang      = 2.0f * D3DX_PI * randf();
		float len      = ( 0.1f + 0.05f * Scale ) * randf();
		float randScal = 0.02f * Scale * randf();
		float randTgtX = x + len * ::cosf( ang );
		float randTgtY = y + len * ::sinf( ang );

		Add( iEffectKind , x, y, Delay, randScal, randTgtX, randTgtY, 0.0f, false, who );
	}
}

void KEffector::Red_Particles( float x, float y, BYTE Delay, BYTE Num, BYTE Scale )
{
    for( int i=0 ; i<Num ; ++i )
    {
        float ang      = 2.0f * D3DX_PI * randf();
        float len      = ( 0.1f + 0.05f * Scale ) * randf();
        float randScal = 0.02f * Scale * randf();
        float randTgtX = x + len * ::cosf( ang );
        float randTgtY = y + len * ::sinf( ang );

        Add( RED_PARTICLE, x, y, Delay, randScal, randTgtX, randTgtY );
    }
}

void KEffector::Fires( float x, float y, BYTE Delay, float fSize/*0.3f*/, int iNum/*20*/ )
{
    for( int i=0 ; i<iNum ; ++i )
    {
        float ang      = 2.0f * D3DX_PI * randf();
		float len      = randf() * 0.5f;
        float randScal = ( randf() * fSize ) + 0.1f;
        float randTgtX = x + len * ::cosf( ang );
        float randTgtY = y + len * ::sinf( ang );

        Add( FIRE, x, y, Delay, randScal, randTgtX, randTgtY );
    }
}

void KEffector::IceFires( float x, float y, BYTE Delay, float fSize/*0.3f*/, int iNum/*20*/ )
{
    for( int i=0 ; i<iNum ; ++i )
    {
		float ang      = 2.0f * D3DX_PI * randf();
		float len      = randf() * 0.5f;
		float randScal = ( randf() * fSize ) + 0.1f;
		float randTgtX = x + len * ::cosf( ang );
		float randTgtY = y + len * ::sinf( ang );
		
        Add( ICE_FIRE, x, y, Delay, randScal, randTgtX, randTgtY );
    }
}

void KEffector::LightFires( float x, float y, BYTE Delay, float fSize/*0.3f*/, int iNum/*20*/ )
{
    for( int i=0 ; i<iNum ; ++i )
    {
        float ang      = 2.0f * D3DX_PI * randf();
        float len      = randf() * 0.5f;
        float randScal = ( randf() * fSize ) + 0.1f;
        float randTgtX = x + len * ::cosf( ang );
        float randTgtY = y + len * ::sinf( ang );

        Add( FLASH, x, y, Delay, randScal, randTgtX, randTgtY );
    }
}
void KEffector::DarkFires( float x, float y, BYTE Delay, float fSize/*0.3f*/, int iNum/*20*/ )
{
    for( int i=0 ; i<iNum ; ++i )
    {
		float ang      = 2.0f * D3DX_PI * randf();
		float len      = randf() * 0.5f;
		float randScal = ( randf() * fSize ) + 0.1f;
		float randTgtX = x + len * ::cosf( ang );
		float randTgtY = y + len * ::sinf( ang );
		
        Add( DARK_FIRE, x, y, Delay, randScal, randTgtX, randTgtY );
    }
}

void KEffector::AddAngelEggProp( float x, float y, bool bIsBlueTeam )
{
    //AddAngelEgg( x, y + 0.05f, bIsBlueTeam );
	Add( ANGEL_EGG, x, y + 0.05f, 0, 0.814f, -100.0f, -100.0f, 0.0f, bIsBlueTeam );
    Add( ANGEL_EGG_PROP, x, y, 0 );
}

void KEffector::AddSandDust( float x, float y )
{
    for ( int i = 0; i < 5; i++ )
    {
        float ang      = 2.0f * D3DX_PI * randf();
        float len      = 0.2f * randf();
        float randOrgX = x + 0.5f * len * ::cosf( ang );
        float randScal = 0.2f + 0.3f * randf();
        float randTgtX = x + len * ::cosf( ang );
        float randTgtY = y + 0.1f * len * ( ::sinf( ang ) + 1.0f );

        Add( SAND_DUST, randOrgX, y, 0, randScal, randTgtX, randTgtY );
    }
}

// ------------------------------------------------------------------------------------------
// 이펙트 제거 함수
// ------------------------------------------------------------------------------------------    
void KEffector::RemoveAll( void )
{
	std::map< int, std::vector<Effect*> >::iterator itor;

	for( itor=m_mapSpark.begin() ; itor!=m_mapSpark.end() ; ++itor )
	{
		for( int i=0 ; i<(int)itor->second.size() ; ++i )
		{
			SAFE_DELETE( itor->second[i] );
		}
		
		itor->second.clear();
	}

	m_mapSpark.clear();
}

void KEffector::LiveFrameMove( Effect* pEffect, OUT std::vector<Effect> &vecWaitCreateList )
{
	if ( pEffect == NULL ) return;
	
	--pEffect->Life;

	pEffect->x += 0.1f * ( pEffect->Target_x - pEffect->x );
	pEffect->y += 0.1f * ( pEffect->Target_y - pEffect->y );

	switch( pEffect->What )
	{
	case SAND_DUST:
		pEffect->Angle += 0.001f * ( 2.0f * D3DX_PI - pEffect->Angle );
		break;
	case HIT_LIGHT: case PARTICLE: case DUST: case WATER_DUST: case LEAF_DUST: case UNDEAD_WALK:
		pEffect->Angle += 0.1f * ( 2.0f * D3DX_PI - pEffect->Angle );
		break;
	case LIGHTNING0: case LIGHTNING1: case LIGHTNING2: case LIGHTNING3:
		pEffect->What = LIGHTNING0 + ( 31 - pEffect->Life ) / 8;
		break;
	case BLUE_LIGHTNING0: case BLUE_LIGHTNING1: case BLUE_LIGHTNING2: case BLUE_LIGHTNING3:
		pEffect->What = BLUE_LIGHTNING0 + ( 31 - pEffect->Life ) / 8;
		break;
	case LIGHTNING4: case LIGHTNING5: case LIGHTNING6: case LIGHTNING7:
		pEffect->What = LIGHTNING4 + ( 31 - pEffect->Life ) / 8;
		break;
	case BLUE_LIGHTNING4: case BLUE_LIGHTNING5: case BLUE_LIGHTNING6: case BLUE_LIGHTNING7:
		pEffect->What = BLUE_LIGHTNING4 + ( 31 - pEffect->Life ) / 8;
		break;
	case SPARK0: case SPARK1: case SPARK2:
		pEffect->What = SPARK0 + ( 19 - pEffect->Life ) / 4;
		if ( 11 > pEffect->Life ) 
			pEffect->Life = 0;
		break;
	case ENERGY_BEAM0: case ENERGY_BEAM1:
		pEffect->What = ENERGY_BEAM0 + ( 5 - pEffect->Life ) / 2;
		break;
	case FOUNTAIN_HEAD0: case FOUNTAIN_HEAD1: case FOUNTAIN_HEAD2:
		{
			float x = pEffect->x - 0.3f + 0.6f * randf();
			float y = pEffect->y;
			int Delay = pEffect->Delay;

			Effect temp;
			temp.What     = WATER_PIECE;
			temp.x        = x;
			temp.y        = y;
			temp.Delay    = Delay;
			temp.Scale    = 0.1f + 0.1f * randf();
			temp.Target_x = x;
			temp.Target_y = y - 0.4f * randf();
			temp.Angle    = 12.6f * randf();
			temp.IsRight  = false;
			temp.who = -1;
			
			//Add( WATER_PIECE, x, y, Delay, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf() );
			vecWaitCreateList.push_back(temp);
		}

		break;
	case ANGEL: case ANGEL_EGG_BANG:
		if ( ( pEffect->Target_x - pEffect->x < 0.0001f ) &&
			( pEffect->Target_y - pEffect->y < 0.0001f ) )
		{
			pEffect->Life = 0;
		}
		else
		{
			++pEffect->Life;
			float x = pEffect->x - 0.1f + 0.2f * randf();
			float y = pEffect->y;

			Effect temp;
			temp.What     = WATER_PIECE;
			temp.x        = x;
			temp.y        = y;
			temp.Delay    = 0;
			temp.Scale    = 0.1f + 0.1f * randf();
			temp.Target_x = x;
			temp.Target_y = y - 0.4f * randf();
			temp.Angle    = 12.6f * randf();
			temp.IsRight  = false;
			temp.who = -1;


			vecWaitCreateList.push_back(temp);
			//Add( WATER_PIECE, x, y, 0, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf() );
		}
		break;
	case ANGEL_EGG_PROP:
		++pEffect->Life;
		break;
	case ANGEL_EGG:
		{
			if ( 1 == ++pEffect->Life ) 
				break;

			float fLife = ( false == pEffect->IsRight ? g_MyD3D->m_fEggHP1 : g_MyD3D->m_fEggHP2 );

			if ( 0.0f >= fLife )
			{
				pEffect->Life = 1;
				//Particles( pEffect->x, pEffect->y + 0.2f, 0, 20, 15 );
				Effect temp;
				for ( int i=0 ; i<20 ; ++i )
				{
					float ang      = 2.0f * D3DX_PI * randf();
					float len      = ( 0.1f + 0.05f * 15 ) * randf();
					float randScal = 0.02f * 15 * randf();
					float randTgtX = pEffect->x + len * ::cosf( ang );
					float randTgtY = (pEffect->y+0.2f) + len * ::sinf( ang );

					temp.What     = PARTICLE;
					temp.x        = pEffect->x;
					temp.y        = pEffect->y+0.2f;
					temp.Delay    = 0;
					temp.Scale    = randScal;
					temp.Target_x = randTgtX;
					temp.Target_y = randTgtY;
					temp.Angle    = 0.f;
					temp.IsRight  = false;
					temp.who = -1;
					
					vecWaitCreateList.push_back(temp);

					//Add( iEffectKind , x, y, Delay, randScal, randTgtX, randTgtY, 0.0f, false, who );
				}
				//LightBim( pEffect->x, pEffect->y + 0.2f, 0 );

				for ( int i = 0; i < 30; i++ )
				{
					float ang   = 2.0f * D3DX_PI * randf();
					float len   = 1.0f + 1.0f * randf();
					float randX = pEffect->x - 0.4f * len * ::sinf( ang );
					float randY = (pEffect->y + 0.2f) + 0.4f * len * ::cosf( ang );

					temp.What     = ENERGY_BEAM0;
					temp.x        = randX;
					temp.y        = randY;
					temp.Delay    = 0;
					temp.Scale    = 2.0f * len;
					temp.Target_x = randX;
					temp.Target_y = randY;
					temp.Angle    = ang;
					temp.IsRight  = false;
					temp.who = -1;

					vecWaitCreateList.push_back(temp);
					//Add( ENERGY_BEAM0, randX, randY, Delay, 2.0f * len, randX, randY, ang );
				}


				if( true == g_MyD3D->m_bNormalEgg )
				{
					temp.What     = ANGEL;
					temp.x        = pEffect->x;
					temp.y        = pEffect->y+0.2f;
					temp.Delay    = 0;
					temp.Scale    = 1.0f;
					temp.Target_x = pEffect->x;
					temp.Target_y = pEffect->y+0.5f;
					temp.Angle    = 0.f;
					temp.IsRight  = false;
					temp.who = -1;


					vecWaitCreateList.push_back(temp);

					//Add( ANGEL, pEffect->x, pEffect->y+0.2f, 0, 1.0f, pEffect->x, pEffect->y+0.5f );	
				}
				else
				{
					temp.What     = ANGEL_EGG_BANG;
					temp.x        = pEffect->x;
					temp.y        = pEffect->y+0.2f;
					temp.Delay    = 0;
					temp.Scale    = 1.0f;
					temp.Target_x = pEffect->x;
					temp.Target_y = pEffect->y + 0.7f ;
					temp.Angle    = 0.f;
					temp.IsRight  = false;
					temp.who = -1;

					vecWaitCreateList.push_back(temp);
					//Add( ANGEL_EGG_BANG, pEffect->x, pEffect->y + 0.2f, 0, 1.0f, pEffect->x, pEffect->y + 0.7f );
				}
			}
			else
			{
				if(fLife>.8f) 
					pEffect->Life=6;
				else if(fLife>.6f)
				{
					if(pEffect->Life!=5) 
						g_KDSound.Play( "105" );
					pEffect->Life=5;
				}
				else if(fLife>.4f)
				{
					if(pEffect->Life!=4) 
						g_KDSound.Play( "105" );
					pEffect->Life=4;
				}
				else if(fLife>.2f) 
				{
					if(pEffect->Life!=3) 
						g_KDSound.Play( "105" );
					pEffect->Life=3;
				}else 
					pEffect->Life=2;
			}
		}
		break;
	case FLOAT_SOUL:
		// TODO : 영혼 FrameMove...
		// Life 조절... 이동 조절...
		pEffect->Scale=.4f+pEffect->Life*.012f;
		break;
	case MON_RISEN:
		pEffect->Scale += 0.015f;
		break;
	} //end of switch
}

// ------------------------------------------------------------------------------------------
// 매 프레임당 호출 함수
// ------------------------------------------------------------------------------------------
void KEffector::FrameMove( void )
{
    PROFILE_SET("KEffector::FrameMove");

    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

	std::vector<Effect> vecWaitCreateList;

	std::map< int, std::vector<Effect*> >::iterator itor = m_mapSpark.begin();
    for ( ; itor != m_mapSpark.end() ; ++itor )
    {
		std::vector<Effect*>::iterator vit = itor->second.begin();
		while ( vit != itor->second.end() )
		{
			Effect* pEffect = *vit;

			if( pEffect == NULL || pEffect == (Effect*)(0x01)) { 
				vit = itor->second.erase( vit );
				continue;
			}

			if( pEffect->Delay > 0 )
				--pEffect->Delay;

			if( pEffect->Delay > 0 )
				continue;

			if( pEffect->Life > 0 )
			{
				LiveFrameMove ( pEffect , vecWaitCreateList);
			}
			else
			{
				SAFE_DELETE( *vit );
				vit = itor->second.erase( vit );
				continue;
			}

			++vit;

		} //end of for

    } //end fo while

	std::vector<Effect>::iterator vit = vecWaitCreateList.begin();
	for ( ; vit != vecWaitCreateList.end() ; ++vit ) {
		for (int i = 0 ; i <30 ; i++)
			Add( vit->What, vit->x, vit->y, vit->Delay, vit->Scale, vit->Target_x, vit->Target_y, vit->Angle, vit->IsRight, vit->who );
	}


	//std::map< int, std::vector<Effect*> >::iterator mit = m_mapSpark.begin();
	//
	//while ( mit != m_mapSpark.end() ) {
	//	if ( mit->second.empty() == true ) {
	//		mit = m_mapSpark.erase(mit);
	//	}
	//	else
	//		++mit;
	//}
}

// ------------------------------------------------------------------------------------------
// 버퍼 세팅
// ------------------------------------------------------------------------------------------
int KEffector::SetBuffer(SLVERTEX* pVB,std::map< int, std::vector<Effect*> >::iterator itor)
{

	int iVertexOffset = 0;
	int rendersize=0;

	std::vector<Effect*>::iterator vit = itor->second.begin();

	for( ; vit != itor->second.end()  ; ++vit )
	{
		Effect* pEffect = *vit;
        
        if( pEffect == NULL || pEffect == (Effect*)(0x01)) { 
            continue;
        }

		// 초기화
		pVB[iVertexOffset].SetSLVER( -0.15f, 0.15f, 0, 0.0f, 0.0f);
		pVB[iVertexOffset+1].SetSLVER( 0.15f, 0.15f, 0, 1.0f, 0.0f);
		pVB[iVertexOffset+2].SetSLVER( -0.15f, -0.15f, 0, 0.0f, 1.0f);
		pVB[iVertexOffset+3].SetSLVER( 0.15f, -0.15f, 0, 1.0f, 1.0f);

		D3DXMATRIX mat1, mat2, mat3;

		switch( pEffect->What )
		{
		case ENERGY_BEAM0: case ENERGY_BEAM1: case ENERGY_BEAM2: 
			{
				UINT alpha = 24 * pEffect->Life;
				SetDiffuse( &pVB[iVertexOffset], ( 0xff < alpha ? 0xff : static_cast<BYTE>( alpha ) ) );
				pEffect->m_IsBlendOne = false;

				D3DXMatrixScaling( &mat2, 0.05f * pEffect->Scale, pEffect->Scale, 1.0f );
			}
			break;
		case DUST: case WATER_DUST: case SAND_DUST: case LEAF_DUST: case UNDEAD_WALK:
			{
				UINT alpha = 24 * pEffect->Life;
				SetDiffuse( &pVB[iVertexOffset], ( 0xff < alpha ? 0xff : static_cast<BYTE>( alpha ) ) );
				pEffect->m_IsBlendOne = false;

				D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			}
			break;
		case LIGHTNING0: case LIGHTNING1: case LIGHTNING2: case LIGHTNING3:
		case LIGHTNING4: case LIGHTNING5: case LIGHTNING6: case LIGHTNING7:
			pEffect->m_IsBlendOne = true;
			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case LANCE_MOVE_CIRCLE:
			{
				BYTE a = static_cast<BYTE>( 255.0f * ( pEffect->Life / 20.0f ) );
				SetDiffuse( &pVB[iVertexOffset], a );
				if( false == pEffect->IsRight )
				{
					pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 0.0f;
					pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 1.0f;
				}
				else
				{
					pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 1.0f;
					pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 0.0f;
				}

				pEffect->m_IsBlendOne = false;

				D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale + 0.1f * ( 20 - pEffect->Life ), 1.0f );
			}
			break;
		case LANCE_MAGIC_CIRCLE:
			{
				BYTE a = static_cast<BYTE>( 255.0f * ( pEffect->Life / 20.0f ) );
				SetDiffuse( &pVB[iVertexOffset], a );
				if( false == pEffect->IsRight )
				{
					pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 0.0f;
					pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 1.0f;
				}
				else
				{
					pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 1.0f;
					pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 0.0f;
				}

				pEffect->m_IsBlendOne = false;

				float fScale = pEffect->Scale + 0.1f * ( 10 - pEffect->Life );
				D3DXMatrixScaling( &mat3, fScale, fScale, 1.0f );
				D3DXMatrixTranslation( &mat1, ( true == pEffect->IsRight ? -0.05f : 0.05f ) * ( 10 - pEffect->Life ), 0.0f, 0.0f );
				D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
			}
			break;
		case LANCE_SPRAY:
			if( false == pEffect->IsRight )
			{
				pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 0.0f;
				pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 1.0f;
			}
			else
			{
				pVB[iVertexOffset].tu = pVB[iVertexOffset+2].tu = 1.0f;
				pVB[iVertexOffset+1].tu = pVB[iVertexOffset+3].tu = 0.0f;
			}
			
			pEffect->m_IsBlendOne = false;
			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case ANGEL_EGG_PROP:
			pEffect->m_IsBlendOne = false;
			D3DXMatrixTranslation( &mat1, 0.0f, .083f, 0.0f );
			D3DXMatrixScaling( &mat3, pEffect->Scale, 0.5f * pEffect->Scale, 1.0f );
			D3DXMatrixMultiply( &mat2, &mat1, &mat3 );
			break;
		case WATER_PIECE: case ANGEL: case ANGEL_EGG_BANG:
		case FOUNTAIN_HEAD0: case FOUNTAIN_HEAD1: case FOUNTAIN_HEAD2:
			pEffect->m_IsBlendOne = false;
			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case ANGEL_EGG:
			switch( pEffect->Life )
			{
			case 1: // 완전히 깨진 알
				pVB[iVertexOffset].tu = 0.0000f; pVB[iVertexOffset].tv = 0.6641f;
				pVB[iVertexOffset+1].tu = 0.4688f; pVB[iVertexOffset+1].tv = 0.6641f;
				pVB[iVertexOffset+2].tu = 0.0000f; pVB[iVertexOffset+2].tv = 1.0000f;
				pVB[iVertexOffset+3].tu = 0.4688f; pVB[iVertexOffset+3].tv = 1.0000f;
				D3DXMatrixScaling( &mat3, pEffect->Scale, pEffect->Scale * 0.7167f, 1.0f );
				break;
			case 2: case 3: case 4: case 5: case 6: // 깨지기 전 또는 금이 간 상태의 알
				pVB[iVertexOffset].tu = 0.0000f; pVB[iVertexOffset].tv = 0.0000f;
				pVB[iVertexOffset+1].tu = 0.4688f; pVB[iVertexOffset+1].tv = 0.0000f;
				pVB[iVertexOffset+2].tu = 0.0000f; pVB[iVertexOffset+2].tv = 0.6641f;
				pVB[iVertexOffset+3].tu = 0.4688f; pVB[iVertexOffset+3].tv = 0.6641f;
				D3DXMatrixScaling( &mat3, pEffect->Scale, pEffect->Scale * 1.4167f, 1.0f );
				break;
			}
			pEffect->m_IsBlendOne = false;

			D3DXMatrixTranslation( &mat1, 0.0f, 0.15f, 0.0f );
			D3DXMatrixMultiply( &mat2, &mat1, &mat3 );
			break;
		case INTO_THE_DARKNESS:
			switch( pEffect->Life % 4 )
			{
			case 0:
				pVB[iVertexOffset].tu = 0.5f; pVB[iVertexOffset].tv = 0.5f;
				pVB[iVertexOffset+1].tu = 1.0f; pVB[iVertexOffset+1].tv = 0.5f;
				pVB[iVertexOffset+2].tu = 0.5f; pVB[iVertexOffset+2].tv = 1.0f;
				pVB[iVertexOffset+3].tu = 1.0f; pVB[iVertexOffset+3].tv = 1.0f;
				break;
			case 1:
				pVB[iVertexOffset].tu = 0.5f; pVB[iVertexOffset].tv = 0.0f;
				pVB[iVertexOffset+1].tu = 1.0f; pVB[iVertexOffset+1].tv = 0.0f;
				pVB[iVertexOffset+2].tu = 0.5f; pVB[iVertexOffset+2].tv = 0.5f;
				pVB[iVertexOffset+3].tu = 1.0f; pVB[iVertexOffset+3].tv = 0.5f;
				break;
			case 2:
				pVB[iVertexOffset].tu = 0.0f; pVB[iVertexOffset].tv = 0.5f;
				pVB[iVertexOffset+1].tu = 0.5f; pVB[iVertexOffset+1].tv = 0.5f;
				pVB[iVertexOffset+2].tu = 0.0f; pVB[iVertexOffset+2].tv = 1.0f;
				pVB[iVertexOffset+3].tu = 0.5f; pVB[iVertexOffset+3].tv = 1.0f;
				break;
			case 3:
				pVB[iVertexOffset].tu = 0.0f; pVB[iVertexOffset].tv = 0.0f;
				pVB[iVertexOffset+1].tu = 0.5f; pVB[iVertexOffset+1].tv = 0.0f;
				pVB[iVertexOffset+2].tu = 0.0f; pVB[iVertexOffset+2].tv = 0.5f;
				pVB[iVertexOffset+3].tu = 0.5f; pVB[iVertexOffset+3].tv = 0.5f;
				break;
			}
			pEffect->m_IsBlendOne = false;
			D3DXMatrixTranslation( &mat1, ( pEffect->IsRight ? 0.135f : -0.135f ), -0.152f, 0.0f );
			D3DXMatrixScaling( &mat3, pEffect->Scale, pEffect->Scale, 1.0f );
			D3DXMatrixMultiply( &mat2, &mat1, &mat3 );
			break;
		case ONE_KILL:
			pEffect->m_IsBlendOne = false;

			if( pEffect->Life < 11 )
			{
				D3DXMatrixScaling( &mat2, ( 10 - pEffect->Life ) * pEffect->Scale * 0.1283f,
					pEffect->Life * pEffect->Scale * 0.0648f, 1.0f );
			}
			else
			{
				D3DXMatrixScaling( &mat2, pEffect->Scale * 1.283f, pEffect->Scale * 0.648f, 1.0f );
			}
			break;
		case ONE_KILL_FLASH:
			if( pEffect->Life < 11 )
			{
				BYTE color = static_cast<BYTE>( 25.5f * pEffect->Life );
				SetDiffuse( &pVB[iVertexOffset], 0xff, color, color, color );
			}

			pEffect->m_IsBlendOne = true;

			D3DXMatrixScaling( &mat2, ( 20 - pEffect->Life ) * pEffect->Scale * 0.25f,
				pEffect->Life * pEffect->Scale * 0.04f, 1.0f );

			break;
		case FLOAT_SOUL:
			pEffect->m_IsBlendOne = true;

			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case DARK_FIRE:
			SetDiffuse( &pVB[iVertexOffset], 0xff, 12 * pEffect->Life, 12 * pEffect->Life, 12 * pEffect->Life );
			pEffect->m_IsBlendOne = false;
			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case REVERSE_GRAVITY:
		case SPARK0: case SPARK1: case SPARK2:
			SetDiffuse( &pVB[iVertexOffset], 0xff, 12 * pEffect->Life, 12 * pEffect->Life, 12 * pEffect->Life );
			pEffect->m_IsBlendOne = true;
			D3DXMatrixScaling( &mat2, 0.6f * pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		case ARROW:
			pEffect->m_IsBlendOne = false;
			D3DXMatrixScaling( &mat1, pEffect->Scale * 0.129f, pEffect->Scale * 0.408f, 1.0f );
			D3DXMatrixTranslation( &mat3, 0.0f, 0.152f, 0.0f );
			D3DXMatrixMultiply( &mat2, &mat1, &mat3 );
			break;
		case DARK_SKULL:
		case MON_RISEN:
			{
				pEffect->m_IsBlendOne = false;
				UINT alpha = 10 * pEffect->Life;
				SetDiffuse( &pVB[iVertexOffset], ( 0xff < alpha ? 0xff : static_cast<BYTE>( alpha ) ) );
				D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			}
			break;
		default:
			SetDiffuse( &pVB[iVertexOffset], 0xff, 12 * pEffect->Life, 12 * pEffect->Life, 12 * pEffect->Life );
			pEffect->m_IsBlendOne = true;
			D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale, 1.0f );
			break;
		} //end of switch

		if( pEffect->Delay > 0 )
			continue;

		if( pEffect->bTrace == true )
		{
			if( pEffect->bMonster == true )
			{
				MONSTER* pMonster = g_kMonsterManager.GetMonster( pEffect->who );
				if(pMonster != NULL ){
					if( pMonster->m_abIsRight[0] )
						pEffect->x = pMonster->m_afX[0]-1.0f + pEffect->fXTerm;
					else
						pEffect->x = pMonster->m_afX[0]-1.0f - pEffect->fXTerm;
					pEffect->y = pMonster->m_afY[0] + pEffect->fYTerm;
				}
			}
			else
			{
				if( g_MyD3D->MyPlayer[pEffect->who]->bIsRight )
					pEffect->x = g_MyD3D->MyPlayer[pEffect->who]->vPos.x-1.0f + pEffect->fXTerm;
				else
					pEffect->x = g_MyD3D->MyPlayer[pEffect->who]->vPos.x-1.0f - pEffect->fXTerm;
				pEffect->y = g_MyD3D->MyPlayer[pEffect->who]->vPos.y-0.2f + pEffect->fYTerm;
			}
		}

		D3DXMatrixRotationZ( &mat3, pEffect->Angle );
		D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
		D3DXMatrixTranslation( &mat2, pEffect->x, pEffect->y - 0.3f, 0.3f );
		D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
		D3DXMatrixMultiply( &mat1, &mat3, &g_kCamera.m_matCamera );

		D3DXVec3TransformCoord( &pVB[iVertexOffset].Pos, &pVB[iVertexOffset].Pos, &mat1 );
		D3DXVec3TransformCoord( &pVB[iVertexOffset+1].Pos, &pVB[iVertexOffset+1].Pos, &mat1 );
		D3DXVec3TransformCoord( &pVB[iVertexOffset+2].Pos, &pVB[iVertexOffset+2].Pos, &mat1 );
		D3DXVec3TransformCoord( &pVB[iVertexOffset+3].Pos, &pVB[iVertexOffset+3].Pos, &mat1 );

		iVertexOffset += 4;
		++rendersize;
	}
	return rendersize;
}

void KEffector::Render()
{
	//SetBuffer();
	static SLVERTEX V[MAX_SPARK_VERTEX];	
	Effect* pEffect = NULL;	

	std::map< int, std::vector<Effect*> >::iterator itor;
	for( itor=m_mapSpark.begin() ; itor!=m_mapSpark.end() ; ++itor )
	{
		int rendersize =SetBuffer(V,itor);
		//실제 렌더링 해야할 사이즈
		if(rendersize > 0 )
		{	

			pEffect = itor->second[0];

			g_pGCDeviceManager2->PushState();
			if( !pEffect->m_IsBlendOne )
			{
                g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
				g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			}
			else if( pEffect->m_IsBlendOne )
			{
				g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
				g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			}

			g_pGCDeviceManager2->DrawInWorld(m_pTexSpark[itor->first],V,rendersize);

			// 천사의 알일때 금도 그려라
			if( ANGEL_EGG == itor->first )
			{
				for( int i=0 ; i<(int)itor->second.size() ; ++i )
				{
					pEffect = itor->second[i];

					if( pEffect->Life < 6 && pEffect->Life > 1 )
						RenderCrack(m_pTexSpark[ANGEL_EGG], pEffect );
				}
			}
            g_pGCDeviceManager2->PopState();
		}
	}
}

void KEffector::RenderCrack( GCDeviceTexture*texture,Effect* pEffect )
{
	SLVERTEX v[4];
	D3DXMATRIX mat1, mat2, mat3;

	v[0].SetSLVER( -0.15f, 0.15f, 0, 0.0f, 0.0f);
	v[1].SetSLVER( 0.15f, 0.15f, 0, 1.0f, 0.0f);
	v[2].SetSLVER( -0.15f, -0.15f, 0, 0.0f, 1.0f);
	v[3].SetSLVER( 0.15f, -0.15f, 0, 1.0f, 1.0f);

	switch ( pEffect->Life )
	{
	case 2:
		v[0].tu = 0.5078f; v[0].tv = 0.0000f;
		v[1].tu = 1.0000f; v[1].tv = 0.0000f;
		v[2].tu = 0.5078f; v[2].tv = 0.2165f;
		v[3].tu = 1.0000f; v[3].tv = 0.2165f;
		break;
	case 3:
		v[0].tu = 0.5078f; v[0].tv = 0.2165f;
		v[1].tu = 1.0000f; v[1].tv = 0.2165f;
		v[2].tu = 0.5078f; v[2].tv = 0.4375f;
		v[3].tu = 1.0000f; v[3].tv = 0.4375f;
		break;
	case 4:
		v[0].tu = 0.5078f; v[0].tv = 0.4375f;
		v[1].tu = 1.0000f; v[1].tv = 0.4375f;
		v[2].tu = 0.5078f; v[2].tv = 0.6563f;
		v[3].tu = 1.0000f; v[3].tv = 0.6563f;
		break;
	case 5:
		v[0].tu = 0.5078f; v[0].tv = 0.6563f;
		v[1].tu = 1.0000f; v[1].tv = 0.6563f;
		v[2].tu = 0.5078f; v[2].tv = 0.8711f;
		v[3].tu = 1.0000f; v[3].tv = 0.8711f;
		break;
	}

	D3DXMatrixScaling( &mat2, pEffect->Scale, pEffect->Scale * 0.4365f, 1.0f );
	D3DXMatrixTranslation( &mat3, 0.004f, 0.181f, 0.0f );
	D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
	D3DXMatrixTranslation( &mat2, pEffect->x, pEffect->y - 0.3f, 0.3f );
	D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
	D3DXMatrixMultiply( &mat1, &mat3, &g_kCamera.m_matCamera );
	
	g_pGCDeviceManager2->DrawInWorld(texture,&mat1,v);
}

