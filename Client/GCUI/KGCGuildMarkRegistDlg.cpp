#include "StdAfx.h"
#include "KGCGuildMarkRegistDlg.h"
#include "MyD3D.h"
#include "GCIconImageManager.h"
#include "ximage.h"

IMPLEMENT_CLASSNAME( KGCGuildMarkRegistDlg );
IMPLEMENT_WND_FACTORY( KGCGuildMarkRegistDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMarkRegistDlg, "gc_guildmark_regist_dlg" );

#define TEMPFILE_FORUPLOAD L"icontemp.png"

KGCGuildMarkRegistDlg::KGCGuildMarkRegistDlg(void)
: m_pPreViewTexture( NULL )
, m_pkOKBtn( NULL )
, m_pkCancelBtn( NULL )
, m_pkFileDialogOpenBtn( NULL )
, m_pkStaticFilePath( NULL )
, m_pkPreViewBackGround( NULL )
, m_pkImageIcon(NULL)
{
    LINK_CONTROL("btn_ok", m_pkOKBtn );
    LINK_CONTROL("btn_cancel", m_pkCancelBtn );
    LINK_CONTROL("btn_filedialog_open", m_pkFileDialogOpenBtn );
    LINK_CONTROL("guildmark_preview_background", m_pkPreViewBackGround );
    LINK_CONTROL("static_file_path", m_pkStaticFilePath );
    LINK_CONTROL("filedialog", m_pkFileDialog );
	LINK_CONTROL("imageicon", m_pkImageIcon);
}

KGCGuildMarkRegistDlg::~KGCGuildMarkRegistDlg(void)
{
}

void KGCGuildMarkRegistDlg::OnCreate( void )
{
    m_pkPreViewBackGround->InitState(true, true, this );
    m_pkOKBtn->InitState( true, true, this );
    m_pkOKBtn->Lock( true );
    m_pkCancelBtn->InitState( true, true, this );
    m_pkFileDialogOpenBtn->InitState( true, true, this );
    m_pkStaticFilePath->InitState( true, true, this );
	m_pkImageIcon->InitState( true, true, this );
	m_pkFileDialog->InitState(false,true,this);
}

void KGCGuildMarkRegistDlg::OnCreateComplete( void )
{
    //만약 내가 길드 마크가 원래 있다면 그걸 미리 보여줄까? 아니라면 안나오게 냅둬
    //m_pPreViewTexture
    m_strTextureFileName = L"";
    m_pkStaticFilePath->SetText(L"");
    m_pkStaticFilePath->SetToolTip(L"");

    DragAcceptFiles( g_MyD3D->Get_hWnd(), TRUE );

	//테스트코드// 길드마크서버에다가 아이콘으로 다운로드 요청을 시도시킨다.
	//SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);
	//m_pkImageIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);
}

void KGCGuildMarkRegistDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCancelBtn,     KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkOKBtn,         KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkFileDialogOpenBtn,          KD3DWnd::D3DWE_BUTTON_CLICK, OnFileDialogOpen );

    GCWND_MSG_MAP( m_pkFileDialog,          KGCFileFindDialog::FET_OK,      FileDialogOK );
    GCWND_MSG_MAP( m_pkFileDialog,          KGCFileFindDialog::FET_CANCEL,  FileDialogCancel );
}

void KGCGuildMarkRegistDlg::OnDestroyComplete( void )
{
    DragAcceptFiles( g_MyD3D->Get_hWnd(), FALSE );
}

void KGCGuildMarkRegistDlg::FrameMoveInEnabledState( void )
{

}

void KGCGuildMarkRegistDlg::OnOK()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_CHANGE_TITLE), 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_CHANGE_ALERT_MSG), KGCUIScene::GC_MBOX_GUILD_MARK_UPLOAD_ALERT, KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true );
}

void KGCGuildMarkRegistDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCGuildMarkRegistDlg::OnFileDialogOpen()
{
    //m_pkCommonFileDialog->InitFileDialog();
    m_pkFileDialog->ToggleRender( true );
    m_pkOKBtn->Lock( true );
    m_pkCancelBtn->Lock( true );
}

void KGCGuildMarkRegistDlg::FileDialogOK()
{
    m_pkFileDialog->ToggleRender( false );
    m_pkCancelBtn->Lock( false );
    SelectTexture( m_pkFileDialog->GetSelectedFileName() );
}

void KGCGuildMarkRegistDlg::FileDialogCancel()
{
    m_pkFileDialog->ToggleRender( false );
    m_pkCancelBtn->Lock( false );

    if( !m_strTextureFileName.empty() && IsEnableRegist() )
    {
       m_pkOKBtn->Lock( false );
    }
}

void KGCGuildMarkRegistDlg::SelectTexture( std::wstring strTextureFileName_ )
{
	//기존에 꺼 있으면 지웁니다.
    if (m_strTextureFileName!=L"" && m_strTextureFileName!=strTextureFileName_)
	{
		SiKGCIconImageManager()->RemoveIconImage(m_strTextureFileName);
		m_strTextureFileName = L"";
	}
	SiKGCIconImageManager()->LoadIconImageFromRealPath(strTextureFileName_);
	m_pkImageIcon->SetTextureName(strTextureFileName_);
	m_strTextureFileName = strTextureFileName_;

    bool bEnableRegist = IsEnableRegist();
    
    if( !bEnableRegist )
    {
		SiKGCIconImageManager()->RemoveIconImage(m_strTextureFileName);
        m_strTextureFileName = L"";
        //길드마크로 사용할 수 없는 이미지 형식 메세지 띄아주기.
    }
    
    m_pkOKBtn->Lock( !bEnableRegist );
    m_pkStaticFilePath->SetForceWordWrapText( m_strTextureFileName, bEnableRegist );

	//툴팁 버그 일으켜서 일단 기능 Off
    //m_pkStaticFilePath->SetToolTip( m_strTextureFileName );
}

void KGCGuildMarkRegistDlg::DragTexture( std::string strTextureFileName_ )
{
    m_strTextureFileName = GCUTIL_STR::GCStrCharToWide( strTextureFileName_.c_str() );
    
    SelectTexture( m_strTextureFileName );
}

bool KGCGuildMarkRegistDlg::IsEnableDrag()
{
    return true;//!m_pkCommonFileDialog->IsRenderOn();
}

bool KGCGuildMarkRegistDlg::IsEnableRegist()
{
    //return ( m_pPreViewTexture && m_pPreViewTexture->IsLoaded() && m_setImageFileFormatFilterList.end() != m_setImageFileFormatFilterList.find( m_pPreViewTexture->GetImageFileFormat() ) );
	return ( SiKGCIconImageManager()->GetTexture(m_strTextureFileName) &&  SiKGCIconImageManager()->GetTexture(m_strTextureFileName)->IsLoaded());
}

void KGCGuildMarkRegistDlg::UploadGuildMark()
{
	LPD3DXBUFFER pBuffer = NULL;

	KItem* pInvenItem = NULL;

	if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(  m_dwMarkChangeItemID , &pInvenItem) == false)
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5), 
			g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MARK_CHANGE_FAIL4), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		return;
	}

	//포멧 변환 및 저장..
	/*if( D3DXSaveTextureToFileInMemory( &pBuffer, D3DXIFF_TGA, pTexture, NULL ) == D3D_OK )
	{
	SAFE_RELEASE( pTexture );
	D3DXCreateTextureFromFileInMemoryEx( pd3dDevice, pBuffer->GetBufferPointer(), pBuffer->GetBufferSize(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_DXT5, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &pTexture );
	ASSERT( m_pLightMap != NULL );
	SAFE_RELEASE( pBuffer );
	D3DXSaveTextureToFile( "icontemp2.png", D3DXIFF_PNG, pTexture, NULL );
	}*/

	DeleteFileW(TEMPFILE_FORUPLOAD);
	//포멧에 맞게 재저장합니다.
	//D3DXSaveTextureToFile( TEMPFILE_FORUPLOAD ,D3DXIFF_PNG,SiKGCIconImageManager()->GetTexture(m_strTextureFileName)->GetDeviceTexture(),NULL);
	CxImage cx;
	cx.Load(m_strTextureFileName.c_str());
	cx.Resample(SIZE_IMAGEICON,SIZE_IMAGEICON);
	cx.Save(TEMPFILE_FORUPLOAD,CXIMAGE_FORMAT_PNG);

	//읽어들인다.
	FILE* fpRead;
	int iStart, iEnd, iFileSize;

	fpRead = _wfopen( TEMPFILE_FORUPLOAD , L"rb");
	iStart = ftell(fpRead);
	fseek(fpRead, 0,SEEK_END );
	iEnd = ftell(fpRead);
	fseek( fpRead, 0, SEEK_SET );

	iFileSize = iEnd - iStart;

	BYTE* ucFileBuffer = new BYTE[ iFileSize ];
	int iReadSize = fread( ucFileBuffer, sizeof(BYTE), iFileSize, fpRead );
	fclose( fpRead );

	KSerializer ks;
	KSerBuffer  kbuff;

	ks.BeginWriting( &kbuff );
	ks.PutRaw( ucFileBuffer, iFileSize );
	ks.EndWriting();
	KP2P::GetInstance()->Send_GuildMarkUpload(kbuff,L"png", pInvenItem->m_ItemUID);
	DeleteFileW(TEMPFILE_FORUPLOAD);
}
