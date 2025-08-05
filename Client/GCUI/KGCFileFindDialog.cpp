#include "stdafx.h"
#include "KGCFileFindDialog.h"


IMPLEMENT_CLASSNAME( KGCFileFindDialog );
IMPLEMENT_WND_FACTORY( KGCFileFindDialog );
IMPLEMENT_WND_FACTORY_NAME( KGCFileFindDialog, "gc_filedialog" );

KGCFileFindDialog::KGCFileFindDialog(void)
{
	m_pkBackGround1 = NULL;
	m_pkBackGround2 = NULL;
	m_pkBackGround3 = NULL;
	m_pkBackGround4 = NULL;
	m_pkBackGround5 = NULL;
	m_pkBackGround6 = NULL;

	m_pkOpenBtn     = NULL;      
	m_pkCancelBtn   = NULL;      
	m_pkupFolderBtn = NULL;      
	m_pkupPrevBtn   = NULL;      

	LINK_CONTROL( "background1",				m_pkBackGround1 );
	LINK_CONTROL( "background2",				m_pkBackGround2 );
	LINK_CONTROL( "background3",				m_pkBackGround3 );
	LINK_CONTROL( "background4",				m_pkBackGround4  );
	LINK_CONTROL( "background5",				m_pkBackGround5 );
	LINK_CONTROL( "background6",				m_pkBackGround6 );
	LINK_CONTROL( "OpenBtn",					m_pkOpenBtn );
	LINK_CONTROL( "CancelBtn",					m_pkCancelBtn );
	LINK_CONTROL( "Close_btn",					m_pkCloseBtn );
	LINK_CONTROL( "upfolderBtn",				m_pkupFolderBtn );
	LINK_CONTROL( "prevBtn",					m_pkupPrevBtn );

	LINK_CONTROL( "static_name1",				m_pkStaticName1 );
	LINK_CONTROL( "static_name2",				m_pkStaticName2 );
	LINK_CONTROL( "static_filter",				m_pkStaticFilter );

	LINK_CONTROL( "DirStatic",					m_pkStaticDir);


	LINK_CONTROL( "edit_file_path",   m_pkEdit );
	LINK_CONTROL( "filelist",   m_pkList );
	LINK_CONTROL( "guild_general_list_scroll", m_pkScroll );

	m_mapEvent["edit_file_path"]   = &KGCFileFindDialog::OnEdit;
	m_mapEvent["filelist"]   = &KGCFileFindDialog::OnList;
	m_mapEvent["guild_general_list_scroll"] = &KGCFileFindDialog::OnScroll;
	//m_mapEvent["CancelBtn"] = &KGCFileFindDialog::OnCancel;

}

KGCFileFindDialog::~KGCFileFindDialog(void)
{

}

void KGCFileFindDialog::OnCreate( void )
{
	SetSelfInputCheck( true );

	ASSERT( m_pkEdit != NULL );
	ASSERT( m_pkList != NULL );
	ASSERT( m_pkScroll != NULL );

	m_pkBackGround1->InitState( true, false, NULL);
	m_pkBackGround2->InitState( true, false, NULL);
	m_pkBackGround3->InitState( true, false, NULL);
	m_pkBackGround4->InitState( true, false, NULL);

	m_pkOpenBtn->InitState( true, true, this);
	m_pkCancelBtn->InitState( true, true, this);
	m_pkupPrevBtn->InitState(true, true, this);
	m_pkupFolderBtn->InitState( true, true, this);
	m_pkCloseBtn->InitState( true, true, this);

    m_pkList->InitState( true, true, this );
	m_pkList->SetFontSize(SiKGCMultipleLanguages()->GetLargeFontSize());
    m_pkList->SetLineSpace( 1.0f );

    m_pkScroll->InitState(true, true, this);
    m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
    m_pkScroll->SetScrollRangeMax( 0 );
    m_pkList->SetMaxLine(700);
    m_pkList->SetiImageIconSize(32);
    //m_pkList->SetFontOutline(true);

    /*m_pkComboBox->InitState(true,true,this);
    m_pkComboBox->SetComboBoxColor(D3DCOLOR_ARGB( 255, 0, 0, 0), D3DCOLOR_ARGB(255,50,50,50));*/
    m_pkStaticName1->ToggleRender(true);
    m_pkStaticName1->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));

    m_pkStaticName2->ToggleRender(true);
    m_pkStaticName2->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));

    m_pkStaticFilter->ToggleRender(true);
    m_pkStaticFilter->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));

    m_pkEdit->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));
    m_pkEdit->AddActionListener( this );
    m_pkEdit->SetSelfInputCheck( false );

    m_pkCloseBtn->InitState( true, true, this);

    m_pkBackGround5->InitState( true, false, NULL);
    m_pkBackGround6->InitState( true, false, NULL);

    m_pkStaticDir->ToggleRender(true);
    m_pkStaticDir->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));	
    m_pkStaticDir->SetFontSize(SiKGCMultipleLanguages()->GetLargeFontSize());
    m_pkStaticDir->SetAlign(DT_CENTER);

	//Static 테스트용
	//m_pkStaticFilter->SetIconImage(L"driver.dds");

	m_pkCancelBtn->SetHotKey( DIK_ESCAPE );

	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"driver.dds");
	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"desktop.dds");
	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"folder.dds");
	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"fileicon.dds");
	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"mycomputer.dds");
	SiKGCIconImageManager()->LoadIconImageFromMassFile(L"mydocument.dds");

	m_pkStaticName1->SetText(g_pkStrLoader->GetString(STR_ID_FILEDIALOG_STATIC_FILENAME));
	m_pkStaticName2->SetText(g_pkStrLoader->GetString(STR_ID_FILEDIALOG_STATIC_FILETYPE));
	m_pkStaticFilter->SetText(g_pkStrLoader->GetString(STR_ID_FILEDIALOG_STATIC_FILTER));

	m_strSelectFile=L"";
	m_FileDlgManager.SetSpecialfolder_DesktopDirectory();

	UpdateList();
}
void KGCFileFindDialog::UpdateList()
{
	m_pkList->Clear();
	m_vecstrDirList.clear();
	m_vecstrFileList.clear();
	m_vecstrDirList = m_FileDlgManager.GetDirectoryList();

	std::wstring strFileName, strDirName;
	std::vector<std::wstring> vecFileList = m_FileDlgManager.GetFileList();

	//확장자 필터링
	//원래 File System Manager 쪽에 .extension()함수로 체크하는게 올바르다..-_- 시간없어서 여기 쳐넣고 난중에 옮겨야 될것 같음.
	for (int i=0;i<(int)vecFileList.size();i++)
	{
		strFileName=vecFileList[i].substr(vecFileList[i].find_last_of(L'\\') + 1,vecFileList[i].length());
		if (strFileName.length() >= 5)
		{
			 strFileName = strFileName.substr(strFileName.length()-4,4);
			 GCUTIL_STR::ToLower(strFileName);

			 if (strFileName==L".jpg" || strFileName ==L".png" || strFileName ==L".bmp" ) //|| strFileName ==L".tif")
			 {
				 m_vecstrFileList.push_back(vecFileList[i]);
				 continue;
			}
		}

		if (strFileName.length() >= 6)
		{
			strFileName = strFileName.substr(strFileName.length()-5,5);
			GCUTIL_STR::ToLower(strFileName);

			if (strFileName==L".jpeg" ) //|| strFileName ==L".tiff")
			{
				m_vecstrFileList.push_back(vecFileList[i]);
				continue;
			}
		}
	}


	//기본적으로 그냥폴더임
	std::wstring strCurrectDir = L"folder.dds";
	std::wstring strDirectoryName = m_FileDlgManager.GetCurrentDirectoryName(); //폴더이름
	if (m_FileDlgManager.IsRootDirectory()==true)
	{
		strCurrectDir=L"driver.dds";
	}
	else
	{
		strDirectoryName=strDirectoryName.substr(strDirectoryName.find_last_of(L'\\') + 1,strDirectoryName.length());
	}

	if (m_FileDlgManager.IsRootDirectory()==false && 
		m_FileDlgManager.GetCurrentDirectoryType() != kFileDlgManager::EDT_DESKTOPFOLDER
	&&  m_FileDlgManager.GetCurrentDirectoryType() != kFileDlgManager::EDT_DEVICELIST
		)
	{
		m_pkList->AddString( L"..", 0xff000000,
			true, 0, false, L"..", 0 ,L"folder.dds");
	}

	//내문서일때 이미지
	if (m_FileDlgManager.GetCurrentDirectoryType() == kFileDlgManager::EDT_MYDOCUMENT)
	{
		strCurrectDir=L"mydocument.dds";
		strDirectoryName = g_pkStrLoader->GetString(STR_ID_FILEDIALOG_MYDOCUMENT);
	}

	//바탕화면일때는 디렉토리 목록 추가.
	if (m_FileDlgManager.GetCurrentDirectoryType() == kFileDlgManager::EDT_DESKTOPFOLDER)
	{
		m_pkList->AddString( g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DEVICELIST), 0xff000000,
			true, 0, false, g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DEVICELIST), 0 ,L"mycomputer.dds");

		m_pkList->AddString( g_pkStrLoader->GetString(STR_ID_FILEDIALOG_MYDOCUMENT), 0xff000000,
			true, 0, false, g_pkStrLoader->GetString(STR_ID_FILEDIALOG_MYDOCUMENT), 0 ,L"mydocument.dds");
		strCurrectDir=L"desktop.dds";
		strDirectoryName = g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DESKTOP);
	}

	//내컴퓨터일때는 디렉토리 목록 추가.
	if (m_FileDlgManager.GetCurrentDirectoryType() == kFileDlgManager::EDT_DEVICELIST)
	{
		for (int i=0;i<(int)m_vecstrDirList.size();i++)
		{
			m_pkList->AddString( m_vecstrDirList[i], 0xff000000,
				true, 0, false, m_vecstrDirList[i], 0 ,L"driver.dds");
		}
		strCurrectDir=L"mycomputer.dds";
		strDirectoryName = g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DEVICELIST);
	}

	for (int i=0;i<(int)m_vecstrDirList.size();i++)
	{
		//strDirName = strDirList[i].substr(0,strDirList[i].find_last_of("\\"));
		strDirName=m_vecstrDirList[i].substr(m_vecstrDirList[i].find_last_of(L'\\') + 1,m_vecstrDirList[i].length());
		m_pkList->AddString( strDirName, 0xff000000,
							 true, 0, false, m_vecstrDirList[i], 0 ,L"folder.dds");
	}

	for (int i=0;i<(int)m_vecstrFileList.size();i++)
	{
		strFileName=m_vecstrFileList[i].substr(m_vecstrFileList[i].find_last_of(L'\\') + 1,m_vecstrFileList[i].length());
		m_pkList->AddString( strFileName, 0xff000000,
			true, 0, false, m_vecstrFileList[i], 0 ,L"fileicon.dds");
	}

    m_pkStaticDir->SetIconImage(strCurrectDir);
    m_pkStaticDir->SetText(strDirectoryName);

    m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
    m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );
	m_pkScroll->SetScrollPos(0);
	m_pkList->SetTopIndex(0);
}

void KGCFileFindDialog::OnScroll( const KActionEvent& event_ )
{
	UNREFERENCED_PARAMETER( event_ );

	int iTop = m_pkScroll->GetScrollPos();
	if ( m_pkList->GetTopIndex() != iTop )
	{
		g_KDSound.Play( "73" );
		m_pkList->SetTopIndex( iTop );
	}
}

void KGCFileFindDialog::OnList( const KActionEvent& event_ )
{
	int iIndex;

	iIndex = event_.m_dwCode;
	std::wstring strClickedMsg = m_pkList->GetReturnMsg( iIndex );    
	std::wstring strEditMsg = L"";
	if( !strClickedMsg.empty() && m_pkList->GetCheckList()==true )
	{	
		//빌어먹을 리스트 컨트롤이 존내 엽기라서 더블클릭 이벤트 연결이 힘들다..
		//선택 됐는데 한번 더 클릭하면.. 그냥 확인으로 인식..
		strEditMsg = m_pkEdit->GetText();

		strClickedMsg=strClickedMsg.substr(strClickedMsg.find_last_of('\\') + 1,strClickedMsg.length());

		//필터 했더니 경로가 다 날라간 경우는.. 특별한 경우다. 그래서 원래 경로 복원한다.
		if (strClickedMsg==L"")
		{
            strClickedMsg = m_pkList->GetReturnMsg( iIndex );    
        }

        if (strClickedMsg == strEditMsg)
        {
            OnOK(); //<--OnOk가 먹혀야 됨
        }
        else
        {
            m_pkEdit->SetText(strClickedMsg.c_str(),false);
            m_pkEdit->SetFocus();	
        }	
	}
}

void KGCFileFindDialog::OnEdit( const KActionEvent& event_ )
{

}


void KGCFileFindDialog::FrameMoveInEnabledState( void )
{
}

void KGCFileFindDialog::OnOK()
{
	std::wstring strEditMsg = m_pkEdit->GetText();

	std::wstring strDirName,strFileName;

	//이동 가능한 경로인지 확인하고 이동한다.
	if (m_FileDlgManager.MovePath( strEditMsg )==true)
	{
		UpdateList();
	}

	// 내컴퓨터로 이동한다. <--- 내 컴퓨터, 내 문서 등의 이름의 폴더를 임의로 가지고 있는 거지 같이 특이한 경우 문제가 된다.
	// 극소수로 추정되기 때문에 예외 처리 안해준다.
	if (strEditMsg==g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DEVICELIST))
	{//리스트 컨트롤 클릭해서 내컴퓨터 가지는 경우는... 바탕화면에서 밖에 없다.
		m_FileDlgManager.SetSpecialfolder_DeviceList();
		UpdateList();
	}

	if (strEditMsg==g_pkStrLoader->GetString(STR_ID_FILEDIALOG_MYDOCUMENT))
	{
		m_FileDlgManager.SetSpecialfolder_MyDocumentDirectory();
		UpdateList();
	}

	//파일인지 조사해서 맞으면 Accept한다.
	for (int i=0;i<(int)m_vecstrFileList.size();i++)
	{
		strFileName=m_vecstrFileList[i].substr(m_vecstrFileList[i].find_last_of(L'\\') + 1,m_vecstrFileList[i].length());
		if (strEditMsg == strFileName )
		{
			m_strSelectFile = m_vecstrFileList[i];
			SpeakToActionListener( KActionEvent( this, FET_OK ) );
			break;
		}		
	}
	m_pkEdit->SetText(L"");	
}

void KGCFileFindDialog::OnCancel()
{
	SpeakToActionListener( KActionEvent( this, FET_CANCEL ) );
}

void KGCFileFindDialog::OnUpFolder()
{
	m_FileDlgManager.MoveToBrachDirectory();
	UpdateList();
}

void KGCFileFindDialog::OnPrevBtn()
{
	m_FileDlgManager.HistoryBack();
	UpdateList();
}

void KGCFileFindDialog::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkupFolderBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnUpFolder );
	GCWND_MSG_MAP( m_pkupPrevBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevBtn );
	GCWND_MSG_MAP( m_pkOpenBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
	GCWND_MSG_MAP( m_pkCancelBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );
	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );

	if (m_mapEvent.find(event.m_pWnd->GetWindowName())!= m_mapEvent.end())
	{
		(this->*m_mapEvent[event.m_pWnd->GetWindowName()])( event );
	}		
}

void KGCFileFindDialog::OnCreateComplete()
{

}

void KGCFileFindDialog::OnDestroyComplete()
{

}


void KGCFileFindDialog::PostChildDraw()
{

}

std::wstring KGCFileFindDialog::GetSelectedDir( void )
{
    return m_FileDlgManager.GetCurrentDirectoryName();   
}

void KGCFileFindDialog::OnReplayDir()
{
    std::wstring strEditMsg = m_pkEdit->GetText();

    std::wstring strDirName,strFileName;

    //이동 가능한 경로인지 확인하고 이동한다.
    if (m_FileDlgManager.MovePath( strEditMsg )==true)
    {
        UpdateList();
    }

    // 내컴퓨터로 이동한다. <--- 내 컴퓨터, 내 문서 등의 이름의 폴더를 임의로 가지고 있는 거지 같이 특이한 경우 문제가 된다.
    // 극소수로 추정되기 때문에 예외 처리 안해준다.
    if (strEditMsg==g_pkStrLoader->GetString(STR_ID_FILEDIALOG_DEVICELIST))
    {//리스트 컨트롤 클릭해서 내컴퓨터 가지는 경우는... 바탕화면에서 밖에 없다.
        m_FileDlgManager.SetSpecialfolder_DeviceList();
        UpdateList();
    }

    if (strEditMsg==g_pkStrLoader->GetString(STR_ID_FILEDIALOG_MYDOCUMENT))
    {
        m_FileDlgManager.SetSpecialfolder_MyDocumentDirectory();
        UpdateList();
    }
}