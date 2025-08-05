#include "stdafx.h"
#include "TextureLoadThread.h"

extern LPDIRECT3DDEVICE9        g_pd3dDevice;

void KTextureLoadThread::ProcessIntEvent( TexLoadData sTexLoadData )
{
	if ( sTexLoadData.strTexName == "" )
		return;

	if ( sTexLoadData.pMemFile == NULL )
		return;

	sTexLoadData.pMemFile->LoadMemoryFile();
	if ( !sTexLoadData.pMemFile->IsLoaded() )
	{
		return;
	}

	HRESULT hr = S_OK;
	hr = D3DXCreateTextureFromFileInMemoryEx( g_pd3dDevice, (void*)sTexLoadData.pMemFile->GetDataPointer(), sTexLoadData.pMemFile->GetFileSize(),
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, sTexLoadData.bLowQuality ? D3DFMT_DXT3 : D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
		&sTexLoadData.TexInfo,
		NULL, &sTexLoadData.m_pTexture );

	if( FAILED(hr) )
	{
		sTexLoadData.m_pTexture = NULL;
		assert(sTexLoadData.m_pTexture != NULL);

		// #if !defined( __PATH__ )
		//         MessageBoxA( NULL, "TextureLoadThread.cpp Error", "", MB_OK );
		// #endif
	}

	SAFE_DELETE( sTexLoadData.pMemFile );

	SiKTextureLoadManager()->AddCompTexture( sTexLoadData.strTexName, sTexLoadData );
}
