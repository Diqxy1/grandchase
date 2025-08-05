#include "stdafx.h"
#include "GCColorDlg.h"


//
//
//


//
//
//
//
//


IMPLEMENT_CLASSNAME( KGCColorDlg );
IMPLEMENT_WND_FACTORY( KGCColorDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCColorDlg, "gc_color_box" );

extern KGCColorDlg* g_pColorDlg = NULL;

KGCColorDlg::KGCColorDlg( void )
{
    g_pColorDlg = this;
    m_iPalleteSize = 10;

    LINK_CONTROL("caption", m_pkCaption );

    char strTemp[MAX_PATH];

    for(int i = 0; i < GC_COLORDLG_COLORNUM; i++ )
    {
        sprintf(strTemp,"color_button%d",i);
        LINK_CONTROL(strTemp, m_pkColorButton[i] );
    }
    m_vecOriginPos.x = 496.0f;
    m_vecOriginPos.y = 242.0f;
}

KGCColorDlg::~KGCColorDlg( void )
{
    // empty
}

void KGCColorDlg::OnCreate( void )
{
    std::string strScript = "ColorTable.lua";
    LoadColorScript(strScript);
}

std::wstring KGCColorDlg::IndexToColorString(int iIndex)
{
	if (m_vecstrColorList.empty()==true)
	{
		std::string strScript = "ColorTable.lua";
		LoadColorScript(strScript);
	}


	if ( static_cast<int>( m_vecstrColorList.size() )< iIndex || 0 > iIndex || m_vecstrColorList.empty()==true)
	{
#if defined(NATION_PHILIPPINE)
		if(g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
			return L"FCFF00";
#endif			
		return L"0xFFFFFFFF";
			
	}
	else
	{
		return m_vecstrColorList[iIndex];
	}
}

D3DCOLOR KGCColorDlg::StringToColor( std::wstring& strColor )
{
    int iCol;
    WCHAR strTemp[7] = { 0, };
    wcsncpy( strTemp, strColor.c_str(), 6 );
    swscanf( strTemp, L"%x", &iCol );
    return ( 0xFF000000 | iCol );
}

void KGCColorDlg::LoadColorScript( std::string strScriptFile )
{
	m_vecstrColorList.clear();

	KGCLuaManager luaMgr;// = KGCLuaManager::getInstance();

	if( GCFUNC::LoadLuaScript( luaMgr , strScriptFile ) == false )
		return ;

	std::string str;
	LUA_BEGIN_TABLE( "RoomNameColor", return )
	{
		for(int i = 1; i <= 10; ++i )
		{
			LUA_GET_VALUE( i, str, return )
				m_vecstrColorList.push_back( GCUTIL_STR::GCStrCharToWide(str.c_str() ) );
		}
	}
	LUA_END_TABLE( return )


#if defined(NATION_PHILIPPINE)
		if(g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
		{
			m_vecstrColorList[5] = L"FCFF00";
		}

#endif		

	for( int i = 0; i < GC_COLORDLG_COLORNUM; ++i )
	{
		m_pkColorButton[i]->InitState( true, i < m_iPalleteSize, this );
		m_pkColorButton[i]->EnableColor( i < m_iPalleteSize );
		m_pkColorButton[i]->SetColor( StringToColor(m_vecstrColorList[i]) );
	}
	m_pkCaption->SetText( g_pkStrLoader->GetString( STR_ID_TEXT_COLOR) );
	m_pkCaption->SetAlign( DT_CENTER );		
	m_eDlgType=TYPE_MSGBOX;
}

void KGCColorDlg::ActionPerformed( const KActionEvent& event )
{   
    if( event.m_dwCode != D3DWE_BUTTON_CLICK )
        return;

	for(int i = 0; i < GC_COLORDLG_COLORNUM; i++ )
    {
        if( m_pkColorButton[i] != event.m_pWnd )
            continue;

        m_strSelColor = m_vecstrColorList[i];
		g_MyD3D->m_KGCOption.SetColorIndex(i);
		
        switch(m_eDlgType)
        {
            case TYPE_MSGBOX:
                    SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );
                    break;
            case TYPE_GAMEOVERUI:
                    g_pkUIScene->SetLastColor(m_strSelColor);
                    InitState(false,false,NULL);
                    break;
        }
	}
}

void KGCColorDlg::SetDlgType(int nDlgType)
{
    m_eDlgType=nDlgType;
}
void KGCColorDlg::SetPalletSize(int iSize)
{
    m_iPalleteSize = iSize;
}

void KGCColorDlg::CancelDlg()
{
    OnCancel();
}
void KGCColorDlg::OnCancel()
{
    switch(m_eDlgType)
    {
    case TYPE_MSGBOX:
        SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_CANCEL ) );
        break;
    case TYPE_GAMEOVERUI:
        ToggleRender(false);
        break;
    }
}

void KGCColorDlg::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	if( ( g_pkInput->BtnUp( KInput::MBLEFT ) && !CheckPosInWindowBound( vMousePos ) )
        || g_pkInput->IsDown( DIK_ESCAPE )
        )
    {
        OnCancel();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    }
}

bool KGCColorDlg::IsCashColor(std::wstring strColor)
{
    if( strColor == L"FFFFFF" )
        return false;
    // 대만에서는 GC 컬러링 아이템이 없다.
//#ifdef NATION_TAIWAN
//    for( int i = 0; i < (int)m_vecstrColorList.size(); i++ )
//    {
//        if( m_vecstrColorList[i] == strColor )
//            return true;
//    }
//    return false;
//#else
    for( int i = 0; i < (int)m_vecstrColorList.size() && i < (GP_ITEM_COLOR_NUM + 1); i++ )
    {
        if( m_vecstrColorList[i] == strColor )
            return false;
    }
    return true;
//#endif
}

bool KGCColorDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    if( kInfo_.m_lParam )    //Cash
        this->SetPalletSize( KGCColorDlg::CASH_ITEM_COLOR_NUM );
    else
        this->SetPalletSize( KGCColorDlg::GP_ITEM_COLOR_NUM );

    return true;
}