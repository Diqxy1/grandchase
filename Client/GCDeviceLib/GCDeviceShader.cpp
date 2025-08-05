#include "stdafx.h"
#include ".\GCDeviceShader.h"

//

#include "dxutil.h"
#include "shader.h"

#if defined( USE_SHADER_LOAD_HLSL )
#include "../GCUtil/GCFileMoniter.h"
bool bReLoad = false;
void ReLoadShader()
{
	bReLoad	= true;
}
#endif

GCDeviceShader::GCDeviceShader(LPDIRECT3DDEVICE9 pd3ddevice, std::string strFileName)
{
	m_eDeviceType = GCDT_SHADER;
	m_strDeviceID   = strFileName;
	m_pEffect = NULL;
	m_bBegin = false;
	m_pD3DDevice = pd3ddevice;
}

GCDeviceShader::~GCDeviceShader(void)
{
	SAFE_RELEASE(m_pEffect);
}

void GCDeviceShader::OnResetDevice()
{
	if( m_pEffect )
		m_pEffect->OnResetDevice();
	else
	{
		Load();
		SetTechnique( const_cast<char*>(m_strTechnique.c_str()) );
	}
}

#if defined( USE_SHADER_LOAD_HLSL )
void GCDeviceShader::OnReLoadShader()
{
	if( m_pEffect )
	{
		m_pEffect->OnLostDevice();
		SAFE_RELEASE(m_pEffect);
		m_pEffect = NULL;
	}
	Load();
}
#endif

void GCDeviceShader::OnLostDevice()
{
	if( m_bBegin )
	{
		assert(!"T_T");
	}
	m_pEffect->OnLostDevice();
}

bool GCDeviceShader::Load()
{
	if( m_pEffect != NULL )
		return false;

	DWORD dwShaderFlags = 0; // D3DXFX_NOT_CLONEABLE
	bool bLoadCompiled = true;

#if defined( USE_SHADER_LOAD_HLSL )
	char szBuf[MAX_PATH] = { 0, };
	::GetCurrentDirectoryA( MAX_PATH, szBuf );

	// 리소스 폴더에 GrandChase.kfx파일이 있는지 확인
	if( NULL != ::SearchPathA(szBuf, m_strDeviceID.c_str(), NULL, 0, NULL, NULL) )
		bLoadCompiled = false;
#endif

	bool bNotLoaded = false;
	if( bLoadCompiled )
	{
		// 컴파일된 Shader 로드
		if( S_OK != D3DXCreateEffect(m_pD3DDevice, g_effect, sizeof(g_effect), NULL,NULL,dwShaderFlags,NULL,&m_pEffect,NULL) )
			bNotLoaded = true;
	}
	else
	{
		// HLSL파일 로드
		if( S_OK != D3DXCreateEffectFromFileA( m_pD3DDevice, m_strDeviceID.c_str(), NULL, NULL, dwShaderFlags, NULL, &m_pEffect, NULL ) )
			bNotLoaded = true;
	}

	if( bNotLoaded )
	{
		assert(!"Shader 읽기 실패");
		m_eDeviceState = GCDS_DISABLE;
		return false;
	}

	m_eDeviceState = GCDS_LOADED;
	return true;
}

void GCDeviceShader::UnLoad()
{
	if( m_bBegin )
	{
		assert(!"T_T");
	}
	SAFE_RELEASE(m_pEffect);
	m_eDeviceState = GCDS_NOT_LOADED;
}

bool GCDeviceShader::SetTechnique( char* strTechnique )
{
	if( m_pEffect == NULL )
		return false;

	HRESULT hr;
	if( ( hr = m_pEffect->SetTechnique( strTechnique ) ) != S_OK )
		return false;

	m_strTechnique = strTechnique;

	return true;
}

void GCDeviceShader::BeginPass( UINT iIndex )
{
	if( m_pEffect == NULL )
		return;

	if( m_bBegin )
	{
		assert( !"Nested BeginPsss!!!");
		return;
	}

	UINT uiPass;
	if( m_pEffect->Begin(&uiPass, 0) != S_OK )
	{
		assert( !"Effect Begin Failed!!" );
		return;
	}

	if( ( m_pEffect->BeginPass(iIndex) ) != S_OK )
	{
		assert( !"Effect BeginPass Failed!!" );
		return;
	}

	m_bBegin = true;
}

void GCDeviceShader::EndPass()
{
	if( m_pEffect == NULL || m_bBegin == false )
		return;

	if( m_pEffect->EndPass() != S_OK )
	{
		assert( !"Effect EndPass Failed!!" );
		return;
	}

	if( m_pEffect->End() != S_OK )
	{
		assert( !"Effect End Failed!!" );
		return;
	}

	m_bBegin = false;
}

#if defined( USE_SHADER_LOAD_HLSL )
void GCDeviceShader::RegisterFileMoniter()
{
	char strDir[ MAX_PATH ] = { 0, };
	GetCurrentDirectoryA(MAX_PATH - 1, strDir);
	strcat(strDir, "\\");
	strcat(strDir, m_strDeviceID.c_str());
	SiKGCFileMoniter()->RegisterFile( strDir, ReLoadShader );
}

void GCDeviceShader::CheckFileMonitor()
{
	if( false == m_bBegin && bReLoad )
	{
		OnReLoadShader();
		bReLoad = false;
	}
}
#endif
