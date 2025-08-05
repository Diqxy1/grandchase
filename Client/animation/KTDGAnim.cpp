#include "stdafx.h"
// KTDGAnim.cpp: implementation of the CKTDGAnim class.
//create
//////////////////////////////////////////////////////////////////////

//
//
#include "../MyD3D.h"

#include "KTDGAnim.h"
#include "KTDGFrustum.h"
//

//
#include "GCCameraOrtho.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKTDGAnim::CKTDGAnim( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice; 
}

CKTDGAnim::~CKTDGAnim()
{
	DeleteDeviceObjects();
}
D3DBLEND CKTDGAnim::StrToBlendOpt( std::string pStrBlend_ )
{
    if( pStrBlend_.compare( "ZERO" ) == 0 )
        return D3DBLEND_ZERO;
    else if( pStrBlend_.compare( "ONE" ) == 0 )
        return D3DBLEND_ONE;
    else if( pStrBlend_.compare( "SRCCOLOR" ) == 0 )
        return D3DBLEND_SRCCOLOR;
    else if( pStrBlend_.compare( "INVSRCCOLOR" ) == 0 )
        return D3DBLEND_INVSRCCOLOR;
    else if( pStrBlend_.compare( "SRCALPHA" ) == 0 )
        return D3DBLEND_SRCALPHA;
    else if( pStrBlend_.compare( "INVSRCALPHA" ) == 0 )
        return D3DBLEND_INVSRCALPHA;
    else if( pStrBlend_.compare( "DESTALPHA" ) == 0 )
        return D3DBLEND_DESTALPHA;
    else if( pStrBlend_.compare( "INVDESTALPHA" ) == 0 )
        return D3DBLEND_INVDESTALPHA ;
    else if( pStrBlend_.compare( "DESTCOLOR" ) == 0 )
        return D3DBLEND_DESTCOLOR;
    else if( pStrBlend_.compare( "INVDESTCOLOR" ) == 0 )
        return D3DBLEND_INVDESTCOLOR;
    return D3DBLEND_ZERO;
}
bool CKTDGAnim::Compile( char* filename )
{
	DeleteDeviceObjects();
    KLuaManager             luaMgr;
	GCMemoryFile MemFile(g_pGCDeviceManager2->GetMassFileManager(),filename );

	if( !MemFile.IsLoaded())
		return false;

	if( FAILED( luaMgr.DoString( MemFile.GetDataPointer(), MemFile.GetFileSize() ) ) )
		return false;
  
    int iKomFileNum = 0;
    // Animation에 관한 기본 세팅 Load
    luaMgr.BeginTable( "ANIM_DESC" );
    {
        luaMgr.GetValue( "KOMFILE_NUM", iKomFileNum );
        luaMgr.BeginTable( "KOMFILE_NAME" );
        {
            std::string strKomfileName = "";
            for ( int i = 1; i <= iKomFileNum; i++ )
            {
                luaMgr.GetValue( i, strKomfileName );
                std::string strFullPathName = ".\\Texture\\anim\\" + strKomfileName;
                g_pGCDeviceManager2->GetMassFileManager()->AddMassFile( strFullPathName );
            }
        }
        luaMgr.EndTable();
    }
    luaMgr.EndTable();

    char strFrameName[128] = {0,};
    char strTexture[128] = {0,};
    luaMgr.BeginTable( "FRAME_DESC" );
    {
        for( int i=1 ; ; ++i )
        {
            if( FAILED( luaMgr.BeginTable(i) ) )
				break;
            
            luaMgr.GetValue( 1, strFrameName, 128 );
            luaMgr.GetValue( 2, strTexture, 128 );

            if ( !GC_GLOBAL_DEFINE::bThreadLoadAnimTex ) 
            {
                g_pGCDeviceManager2->CreateTexture(strTexture,true);
            }

            luaMgr.EndTable();
            m_mapFramePool[strFrameName] = new CAnimFrame( m_pd3dDevice, strFrameName, strTexture , NULL );
            ZeroMemory( strFrameName, sizeof(char)*128 );
            ZeroMemory( strTexture, sizeof(char)*128 );
        }
    }
    luaMgr.EndTable();

    int iSequenceFrameNum = 0;
    int iGap = 0;
    char strSequenceFrameCount[64] = {0,};
    char strSequenceName[128] = {0,};
    std::string strSequenceFrameName;
    std::string strSrcBlend;
    std::string strDestBlend;
    if( FAILED( luaMgr.BeginTable( "SQUENCE_DESC" ) ) ) return false;
    {
        for ( int i=1 ; ; ++i )
        {
            if( FAILED( luaMgr.BeginTable(i) ) ) 
				break; 

            if( FAILED( luaMgr.GetValue( "NAME", strSequenceName, 128 )) )
                break;

            LUA_GET_VALUE_DEF( "SRCBLEND", strSrcBlend, "SRCALPHA" );
            LUA_GET_VALUE_DEF( "DESTBLEND", strDestBlend, "INVSRCALPHA" );

            CAnimSequence* pSequence = new CAnimSequence( m_pd3dDevice, strSequenceName );
            pSequence->m_SrcBlendMode = StrToBlendOpt( strSrcBlend );
            pSequence->m_DestBlendMode = StrToBlendOpt( strDestBlend );

            if( FAILED( luaMgr.GetValue( "SEQUENCE_FRAME_NUM", iSequenceFrameNum)))    // num 정의 없으면 새방식
            {
                if( luaMgr.BeginTable("FRAME") == S_OK )
                {
                    for( int j=1 ; ; ++j )
                    {
                        if( luaMgr.BeginTable( j ) == E_FAIL )
                            break;

                        luaMgr.GetValue( 1, strSequenceFrameName );
                        LUA_GET_VALUE_DEF( 2, iGap, 180 );
                        FrameData* pFrameData = new FrameData;
                        pFrameData->frame = GetFrame( strSequenceFrameName );
                        pFrameData->fElapsedTime = (float)iGap/1000.0f;

                        pSequence->AddFrameData( pFrameData );	
						strSequenceFrameName.clear();
						luaMgr.EndTable();
                    }
                    luaMgr.EndTable();  
                }
                luaMgr.EndTable();
            }
            else
            {
                for ( int j = 0; j < iSequenceFrameNum; j++  )
                {
                    sprintf( strSequenceFrameCount, "FRAME%d", j );
                    luaMgr.BeginTable( strSequenceFrameCount );
                    {
                        luaMgr.GetValue( 1, strSequenceFrameName );
                        luaMgr.GetValue( 2, iGap );
                        FrameData* pFrameData = new FrameData;
                        pFrameData->frame = GetFrame( strSequenceFrameName );
                        pFrameData->fElapsedTime = (float)iGap/1000.0f;
            
                        pSequence->AddFrameData( pFrameData );	
						strSequenceFrameName.clear();
					}
                    luaMgr.EndTable();
					ZeroMemory( strSequenceFrameCount, sizeof(char)*64 );
                }
                luaMgr.EndTable();
            }

            AddSequence( pSequence );
			ZeroMemory( strSequenceName, sizeof(char)*128 );
			strSequenceFrameName.clear();
			strSrcBlend.clear();
			strDestBlend.clear();
        }
    }
    luaMgr.EndTable();

	return true;
}

HRESULT CKTDGAnim::FrameMove( float fElapsedTime, void* pframeData )
{
    PROFILE_SET("CKTDGAnim::FrameMove");

	for( int i = (int)m_AnimInstance.size()-1; i >= 0  ; --i )
	{
		if( m_AnimInstance[i]->GetState() != AS_DIE )
			m_AnimInstance[i]->FrameMove( fElapsedTime, pframeData );
		else if( m_AnimInstance[i]->GetState() == AS_DIE )
			DeleteInst( m_AnimInstance[i] );
	}

	return S_OK;
}

HRESULT CKTDGAnim::Render( bool renderEnv )
{
    g_pGCDeviceManager2->PushState();
    {
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        

        g_MyD3D->m_pFrustum->Construct( 1000.0f );

        for( int i = 0; i < (int)m_AnimInstance.size(); i++ )
        {
            if( m_AnimInstance[i]->GetState() != AS_DIE )
			{
				m_AnimInstance[i]->ApplyDiffuse();
				m_AnimInstance[i]->Render( renderEnv );
			}
        }
    }
    g_pGCDeviceManager2->PopState();
	return S_OK;
}

void CKTDGAnim::AddTexture( GCDeviceTexture* texture )
{
	if( texture == NULL )
		return;

	m_mapTexturePool[texture->GetDeviceID()] = texture;
}

void CKTDGAnim::DeleteTexture( GCDeviceTexture* texture )
{
	map<string, GCDeviceTexture*>::iterator itor;

	itor = m_mapTexturePool.find( texture->GetDeviceID() );

	if( itor != m_mapTexturePool.end() )
	{
		itor->second->Release();
		m_mapTexturePool.erase( itor );
	}
}

GCDeviceTexture* CKTDGAnim::GetTexture( const string& filename )
{
	strupr( (char*)filename.c_str() );

	std::map<std::string, GCDeviceTexture*>::iterator itor;

	itor = m_mapTexturePool.find( filename );
	if( itor == m_mapTexturePool.end() )
	{

        GCDeviceTexture* pTexture = NULL;

        if ( GC_GLOBAL_DEFINE::bThreadLoadAnimTex ) 
        {
            pTexture = g_pGCDeviceManager2->CreateTextureThread(filename, true, true);
        }
        else
        {
            pTexture = g_pGCDeviceManager2->CreateTexture(filename, true);
        }

		m_mapTexturePool[filename] = pTexture;
		return pTexture;
	}
	
	return itor->second;
}

void CKTDGAnim::ReleaseTexture()
{
	map<string, GCDeviceTexture*>::iterator itor;

	for( itor = m_mapTexturePool.begin() ; itor != m_mapTexturePool.end() ; ++itor )
	{
		SAFE_RELEASE(itor->second);
	}

	m_mapTexturePool.clear();
}

void CKTDGAnim::DeleteFrame( CAnimFrame* frame )
{
	map<string, CAnimFrame*>::iterator itor;
	itor = m_mapFramePool.find(frame->GetName());

	if( itor != m_mapFramePool.end() )
	{
		SAFE_DELETE(itor->second);
		m_mapFramePool.erase(itor);
	}
}

CKTDGAnim::CAnimFrame* CKTDGAnim::GetFrame( std::string name )
{
	std::map<std::string, CAnimFrame*>::iterator itor;
	itor = m_mapFramePool.find( name );
	if( itor == m_mapFramePool.end() )  // 새 프레임~ 먼저 프레임 정의 안하고 시퀀스에 바로 .dds 쓰자~
    {
        if( name.find('.') == std::string::npos )
            return NULL;
        else
        {
            if ( !GC_GLOBAL_DEFINE::bThreadLoadAnimTex ) 
            {
                g_pGCDeviceManager2->CreateTexture(name, true);
            }

            std::string fileName = name;
            std::string frameName = name.erase( name.find('.'));
            return ( new CAnimFrame( m_pd3dDevice, frameName.c_str(), fileName.c_str() , NULL ) );
        }
    }
	return itor->second;
}
CKTDGAnim::CAnimFrame* CKTDGAnim::GetFrame( const char* pStrName_ )
{
    std::string strName = pStrName_;
	return GetFrame(strName);
}
void CKTDGAnim::ClearFramePool()
{
	map<string, CAnimFrame*>::iterator itor;
	for( itor=m_mapFramePool.begin() ; itor != m_mapFramePool.end() ; ++itor )
		SAFE_DELETE( itor->second );

	m_mapFramePool.clear();
}

void CKTDGAnim::AddSequence( CAnimSequence* sequence )
{
	m_SequencePool[sequence->GetName()] = sequence;
}

void CKTDGAnim::DeleteSequence( CAnimSequence* sequence )
{
	m_SequencePool.erase( sequence->GetName() );
}

CKTDGAnim::CAnimSequence* CKTDGAnim::GetSequence( const string& name )
{
	std::map<std::string, CAnimSequence*>::iterator itor;
	itor = m_SequencePool.find(name);
	if( itor == m_SequencePool.end() )
		return NULL;

	return itor->second;
}

void CKTDGAnim::ClearSequencePool()
{	
	map<string, CAnimSequence*>::iterator itor;

	for( itor = m_SequencePool.begin() ; itor != m_SequencePool.end() ; ++itor )
	{
		SAFE_DELETE( itor->second );
	}

	m_SequencePool.clear();
}

CKTDGAnim::CAnimInstance* CKTDGAnim::CreateInst( CAnimSequence* sequence )
{
	CKTDGAnim::CAnimInstance* inst;

	if( sequence == NULL )
		return NULL;
#if defined(RESOURCE_GRAPH)	
	g_pGCDeviceManager2->AddRelation("ANIM_"+sequence->GetName(),"STATE_"+g_MyD3D->m_pStateMachine->GetStateString());	
#endif

	//런타임 로드
	for( int i = 0; i < (int)sequence->m_FrameList.size(); i++ )
	{
        sequence->m_FrameList[i]->frame->SetTexture( GetTexture(sequence->m_FrameList[i]->frame->GetTexFileName() ) );
		
        if( sequence->m_FrameList[i]->frame->GetTexture()->IsLoaded())
        {
		    if( sequence->m_FrameList[i]->frame->GetBuild() == false )
            {
			    sequence->m_FrameList[i]->frame->BuildFrame();
            }
        }
	}

	inst = new CKTDGAnim::CAnimInstance( m_pd3dDevice, sequence );

	m_AnimInstance.push_back( inst );
	//Sort();

	return inst;
}

void CKTDGAnim::DeleteInst( CAnimInstance* instance )
{
	if( instance == NULL )
		return;

	for( int i=(int)m_AnimInstance.size()-1 ; i >= 0 ; --i )
	{
		if( m_AnimInstance[i] == instance )
		{
			SAFE_DELETE(instance);
			m_AnimInstance.erase(m_AnimInstance.begin()+i);
		}
	}
}

void CKTDGAnim::ClearInstList()
{
	for( int i = 0; i < (int)m_AnimInstance.size(); i++ )
	{
		SAFE_DELETE(m_AnimInstance[i]);
	}

	m_AnimInstance.clear();
}

HRESULT	CKTDGAnim::DeleteDeviceObjects()
{
	ClearInstList();
	ClearSequencePool();
	ClearFramePool();

	return S_OK;
}

void CKTDGAnim::ToLast( CAnimInstance* instance )
{
	for( int i = 0; i < (int)m_AnimInstance.size(); i++ )
	{
		if( m_AnimInstance[i] == instance )
		{
			m_AnimInstance.erase(m_AnimInstance.begin()+i);
			m_AnimInstance.push_back(instance);
		}
	}
}

void CKTDGAnim::Sort()
{
	std::sort(m_AnimInstance.begin(),m_AnimInstance.end(),LessZ());	
}

//////////////////////////////////////////////////////////////////////
// CAnimFrame
//////////////////////////////////////////////////////////////////////

CKTDGAnim::CAnimFrame::CAnimFrame( LPDIRECT3DDEVICE9 pd3dDevice,const char* pStrName, const char* pStrTexture, RECT* rect )
{
	//HRESULT hr;

	m_pd3dDevice	= pd3dDevice;
	m_strTextureFileName = pStrTexture;
	m_pTexture		= NULL;
	m_Name			= pStrName;
	m_pRect			= rect;
	m_bBuild		= false;
	BuildFrame();
}

CKTDGAnim::CAnimFrame::~CAnimFrame()
{
	m_pd3dDevice	= NULL;
	m_pTexture		= NULL;
}

void CKTDGAnim::CAnimFrame::Render( DWORD dwDiffuse )
{
	if( m_bBuild == false )
		BuildFrame( dwDiffuse );

	D3DXMATRIX mat;
	m_pd3dDevice->GetTransform(D3DTS_WORLD,&mat);
	g_pGCDeviceManager2->DrawInWorld(m_pTexture,&mat,m_pFrameVB.as_array() );
}

void CKTDGAnim::CAnimFrame::BuildFrame( DWORD dwDiffuse )
{
	bool IsValidFrame = false;
	
	if( m_pTexture ){
		IsValidFrame = ( m_pTexture->GetWidth() == 0.0f || m_pTexture->GetHeight() == 0.0f ) ? false : true;
	}

	if(	IsValidFrame ){
		// 1----2
		// |    |
		// 3----4

		struct LocalStruct_FrameInitData
		{
			float		Pos_x, Pos_y, Pos_z;
			DWORD		Diff;
			float		tu, tv;
		};

		float	rt_texture_left		=	m_pRect ? static_cast<float>( m_pRect->left/m_pTexture->GetWidth()) : 0.0f;
		float	rt_texture_top		=	m_pRect ? static_cast<float>( m_pRect->top/m_pTexture->GetHeight()) : 0.0f;
		float	rt_texture_bottom	=	m_pRect ? static_cast<float>( m_pRect->bottom/m_pTexture->GetHeight()) : 1.0f;
		float	rt_texture_right	=	m_pRect ? static_cast<float>( m_pRect->right/m_pTexture->GetWidth()) : 1.0f;

		LocalStruct_FrameInitData	InitData[] = {
			/*1*/	{ -(m_pTexture->GetWidth()/2.0f),  (m_pTexture->GetHeight()/2.0f), 0.0f,	//xyz
						dwDiffuse,							//Diff
						rt_texture_left,	//tu
						rt_texture_top	//tv
			},
			/*2*/	{  (m_pTexture->GetWidth()/2.0f),  (m_pTexture->GetHeight()/2.0f), 0.0f,
						dwDiffuse,
						rt_texture_right,
						rt_texture_top
			},
			/*3*/	{ -(m_pTexture->GetWidth()/2.0f), -(m_pTexture->GetHeight()/2.0f), 0.0f,
						dwDiffuse,
						rt_texture_left,
						rt_texture_bottom
			},
			/*4*/	{  (m_pTexture->GetWidth()/2.0f), -(m_pTexture->GetHeight()/2.0f), 0.0f,
						dwDiffuse,
						rt_texture_right,
						rt_texture_bottom
			},
		};

		for( size_t i=0; i<sizeof(InitData)/sizeof(LocalStruct_FrameInitData); ++i ){
			SLVERTEX*	pVertices	=	&m_pFrameVB[i];
			pVertices->Pos.x		=	InitData[i].Pos_x;
			pVertices->Pos.y		=	InitData[i].Pos_y;
			pVertices->Pos.z		=	InitData[i].Pos_z;
			pVertices->Diff			=	InitData[i].Diff;

			pVertices->tu			=	InitData[i].tu;
			pVertices->tv			=	InitData[i].tv;
		}

		m_bBuild = true;
	}

	return;
}









//////////////////////////////////////////////////////////////////////
// CAnimSequence
//////////////////////////////////////////////////////////////////////

CKTDGAnim::CAnimSequence::CAnimSequence( LPDIRECT3DDEVICE9 pd3dDevice, const char* pStrName_ )
{
	m_pd3dDevice	= pd3dDevice;
	m_MaxFrame		= 0;
	m_Name			= pStrName_;
}

CKTDGAnim::CAnimSequence::~CAnimSequence()
{
	for( int i = 0; i < (int)m_FrameList.size(); i++ )
	{
		SAFE_DELETE(m_FrameList[i]);
	}

	m_FrameList.clear();
}

void CKTDGAnim::CAnimSequence::Render( int frame )
{
	m_FrameList[frame]->frame->Render();
}














//////////////////////////////////////////////////////////////////////
// CAnimInstance
//////////////////////////////////////////////////////////////////////
CKTDGAnim::CAnimInstance::CAnimInstance( LPDIRECT3DDEVICE9 pd3dDevice, CAnimSequence* animSequence )
{
	m_pd3dDevice		= pd3dDevice;
	m_pAnimSequence		= animSequence;	
	m_PlayType			= PT_ONE_PASS;
	m_AnimState			= AS_STOP;
	m_NowFrame			= 0;
	m_FrameSpeed		= 1.0f;
	m_TimeAdd			= 0.0f;

	m_SrcBlendMode		= animSequence->m_SrcBlendMode; 
	m_DestBlendMode		= animSequence->m_DestBlendMode;
	m_bShow				= true;

    m_UseAddMatrix      = false;

    m_dwDiffuse         = 0xFFFFFFFF;
}

CKTDGAnim::CAnimInstance::~CAnimInstance()
{
	m_pd3dDevice		= NULL;
	m_pAnimSequence		= NULL;	
	m_PlayType			= PT_ONE_PASS;
	m_AnimState			= AS_DIE;
	m_NowFrame			= 0;
	m_FrameSpeed		= 0.0f;
	m_TimeAdd			= 0.0f;
}


template < typename T >
void MinMaxLimit( const T t_min, T t_val, const T t_max )
{
    if( t_val > t_max ) {
		ASSERT( !"Potal index overflow" );
    }
    if( t_val < t_min ) {
		ASSERT( !"Potal index underflow" );
    }
}

void CKTDGAnim::CAnimInstance::FrameMove( float fElapsedTime, void* pframeData )
{
	if( m_AnimState != AS_PLAY )
		return;

	m_TimeAdd += fElapsedTime * m_FrameSpeed;
	if( m_pAnimSequence->m_FrameList[m_NowFrame]->fElapsedTime > m_TimeAdd )
		return;

	m_TimeAdd = 0.0f;
	switch( m_PlayType )
	{
	case PT_ONE_PASS:
		{				
			if( m_pAnimSequence->GetMaxFrame() > m_NowFrame )
				m_NowFrame++;
			else if( m_pAnimSequence->GetMaxFrame() == m_NowFrame )
				Stop();
		}
		break;
	case PT_ONE_PASS_AUTO_DIE:
		{				
			if( m_pAnimSequence->GetMaxFrame() > m_NowFrame )
				m_NowFrame++;
			else if( m_pAnimSequence->GetMaxFrame() == m_NowFrame )
				Die();
		}
		break;
	case PT_COUNTER_PASS:
		{				
			if( 0 < m_NowFrame )
				m_NowFrame--;
			else if( 0 == m_NowFrame )
				Stop();
		}
		break;
	case PT_COUNTER_PASS_AUTO_DIE:
		{				
			if( 0 < m_NowFrame )
				m_NowFrame--;
			else if( 0 == m_NowFrame )
				Die();
		}
		break;
	case PT_LOOP:
		{
			if( m_pAnimSequence->GetMaxFrame() > m_NowFrame )
				m_NowFrame++;
			else if( m_pAnimSequence->GetMaxFrame() == m_NowFrame )
				m_NowFrame = 0;
		}
		break;
	case PT_COUNTER_LOOP:
		{
			if( 0 < m_NowFrame )
				m_NowFrame--;
			else if( 0 == m_NowFrame )
				m_NowFrame = m_pAnimSequence->GetMaxFrame();
		}
		break;
	case PT_PING_PONG:
		{
			if( m_PingPongCounter == false )
			{
				if( m_pAnimSequence->GetMaxFrame() > m_NowFrame )
					m_NowFrame++;
				else if( m_pAnimSequence->GetMaxFrame() == m_NowFrame )
				{
					m_NowFrame--;
					m_PingPongCounter = true;
				}
			}
			else
			{
				if( 0 < m_NowFrame )
					m_NowFrame--;
				else if( 0 == m_NowFrame )
				{
					m_NowFrame++;
					m_PingPongCounter = false;
				}
			}
		}
		break;
	}
}

void CKTDGAnim::CAnimInstance::Render( bool renderEnv )
{
	if( m_AnimState == AS_STOP || m_AnimState == AS_DIE || m_bShow == false )
		return;

    // 2007/4/2. iridology. 이곳에서 죽는 녀석이 있어서.. 
    if( m_pAnimSequence->GetMaxFrame() < m_NowFrame )
        return;

    //  빌드 프레임때문에 먼저 돌려야 한다.
    if( m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetTexture()->IsLoaded() == false )
    {
        m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetTexture()->GetThreadTexture();

        if ( m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetTexture()->IsLoaded() == false )
            return;
    }

    if ( !m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetBuild() )
    {
        m_pAnimSequence->m_FrameList[m_NowFrame]->frame->BuildFrame();
    }

	g_pGCDeviceManager2->PushState();
	
    g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, m_SrcBlendMode );
	g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, m_DestBlendMode );
	
	D3DXMATRIX matView, matTranspose;
	D3DXMATRIX matbill, matFinal;

	/*m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixTranspose(&matTranspose, &matView);
	D3DXMatrixIdentity(&matbill);
	
	matbill._11 = matTranspose._11;
	matbill._12 = matTranspose._12;
	matbill._13 = matTranspose._13;
	matbill._21 = matTranspose._21;
	matbill._22 = matTranspose._22;
	matbill._23 = matTranspose._23;
	matbill._31 = matTranspose._31;
	matbill._32 = matTranspose._32;
	matbill._33 = matTranspose._33;*/
	
	matFinal = m_KTDGMatrix.GetScaleMatrix() * 
				m_KTDGMatrix.GetRotMatrix() * 
				m_KTDGMatrix.GetTransMatrix();

    D3DXMatrixMultiply( &matFinal, &matFinal, &g_kCamera.m_matCamera );
    if( m_UseAddMatrix == true )
        D3DXMatrixMultiply( &matFinal, &matFinal, &m_AddMatrix );
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matFinal);
	
    D3DXVECTOR3 point;
    float       radius;

    point.x = matFinal._41;
    point.y = matFinal._42;
    point.z = 0.5f;

    if( this->GetRealWidth() > this->GetRealHeight() )
    {
        radius = this->GetRealWidth()*2.0f/*/2.0f * 1.5f*/;
    }
    else
    {
        radius = this->GetRealHeight()*2.0f/*/2.0f * 1.5f*/;
    }

    if( g_MyD3D->m_pFrustum->CheckSphere( point, radius ) == true )
    {
        m_pAnimSequence->m_FrameList[m_NowFrame]->frame->Render( m_dwDiffuse );
    }

    g_pGCDeviceManager2->PopState();

	/*D3DXMatrixIdentity( &matFinal );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matFinal );*/
}

void CKTDGAnim::CAnimInstance::Start( PlayType playtype, float speed )
{
	if( m_AnimState != AS_STOP )
		return;

	m_PlayType		= playtype;
	m_FrameSpeed	= speed;
	m_AnimState		= AS_PLAY;
	
	m_TimeAdd		= 0.0f;


	switch( m_PlayType )
	{
		case PT_ONE_PASS:
		case PT_ONE_PASS_AUTO_DIE:
		case PT_LOOP:
		case PT_PING_PONG:
			m_NowFrame = 0;
			m_PingPongCounter = false;
			break;

		case PT_COUNTER_PASS:
		case PT_COUNTER_PASS_AUTO_DIE:
		case PT_COUNTER_LOOP:
			m_NowFrame = m_pAnimSequence->GetMaxFrame();
			break;
	}
}

void CKTDGAnim::CAnimInstance::Stop()
{
	if( m_AnimState == AS_STOP || m_AnimState == AS_DIE )
		return;

	m_AnimState = AS_STOP;
	m_NowFrame	= 0;
	m_TimeAdd	= 0.0f;
}

void CKTDGAnim::CAnimInstance::Wait()
{
	if( m_AnimState == AS_PLAY )
		m_AnimState = AS_WAIT;
	else if( m_AnimState == AS_WAIT )
		m_AnimState = AS_PLAY;
	else
		return;
}

void CKTDGAnim::CAnimInstance::SetDiffuse( DWORD dwDiffuse )
{
    m_dwDiffuse = dwDiffuse;
}


float CKTDGAnim::CAnimInstance::GetRealWidth()
{ 
	MinMaxLimit( 0, m_NowFrame, m_pAnimSequence->GetMaxFrame() );
	return m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetWidth() * m_KTDGMatrix.GetXScale();
}
float CKTDGAnim::CAnimInstance::GetRealHeight()
{ 
	MinMaxLimit( 0, m_NowFrame, m_pAnimSequence->GetMaxFrame() );
	return m_pAnimSequence->m_FrameList[m_NowFrame]->frame->GetHeight() * m_KTDGMatrix.GetYScale();
}

void CKTDGAnim::CAnimInstance::ApplyDiffuse( void )
{
	m_pAnimSequence->m_FrameList[m_NowFrame]->frame->BuildFrame( m_dwDiffuse );
}
