#include "stdafx.h"
#include "GCTextColorManager.h"

ImplementSingleton( GCTextColorManager )

GCTextColorManager::GCTextColorManager(void)
: m_iNumOfColor(0)
{
	std::string strScript = "ColorTable.lua";
	LoadColorScript( strScript );

}

GCTextColorManager::~GCTextColorManager(void)
{
}


void GCTextColorManager::LoadColorScript( std::string strScriptFile )
{
	m_vecstrColorList.clear();

	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr , strScriptFile ) == false )
		return ;

	std::string str;
	LUA_BEGIN_TABLE( "RoomNameColor", return)
	{
		for(int i = 1; i <= NUM_OF_PAINT_COLOR; i++ )
		{
			LUA_GET_VALUE(i,str, return)
				m_vecstrColorList.push_back( GCUTIL_STR::GCStrCharToWide( str.c_str() ) );
		}
	}
	LUA_END_TABLE(return)
}

D3DCOLOR GCTextColorManager::StringToColor( std::wstring& strColor )
{
	int iCol;
	WCHAR strTemp[7] = { 0, };
	wcsncpy( strTemp, strColor.c_str(), 6 );
	swscanf( strTemp, L"%x", &iCol );
	return ( 0xFF000000 | iCol );
}

std::wstring GCTextColorManager::ColorToString( DWORD ColorCode )
{
	int iCol;
	std::wstringstream tempColor;
	for ( auto element : m_vecstrColorList ) {
		tempColor.str(L"");
		tempColor << element;
		WCHAR strTemp[7] = { 0, };
		wcsncpy(strTemp, tempColor.str().c_str(), 6);
		swscanf(strTemp, L"%x", &iCol);
		if (ColorCode == (0xFF000000 | iCol))
			return tempColor.str();
	}
	return L"FFFFFF";
}

D3DCOLOR GCTextColorManager::GetColor(int index)
{
	if( m_iNumOfColor > index && index >= 0 )
	{
		if( (int)m_vecstrColorList.size() > index  )
			return StringToColor( m_vecstrColorList[index] );
	}

	return 0xFFFFFFFF; // ¾øÀ¸¸é Èò»öÀÌ´Ù.
}

void GCTextColorManager::SetColorNum( int nNum )
{
	m_iNumOfColor = nNum;
}

std::wstring GCTextColorManager::GetColorString(int iColorIndex)
{
	if( m_iNumOfColor <= iColorIndex || iColorIndex < 0 )
		return L"FFFFFF";

	return m_vecstrColorList[iColorIndex];
}

bool GCTextColorManager::IsCashColor(std::wstring strColor)
{
	if( strColor == L"FFFFFF" )
		return false;

	for( int i = 0; i < (int)m_vecstrColorList.size() && i < GP_ITEM_COLOR_NUM + 1; ++i )
	{
		if( m_vecstrColorList[i] == strColor )
			return false;
	}
	return true;
}

bool GCTextColorManager::IsCashColor( int iIndex)
{
	if( iIndex < GP_ITEM_COLOR_NUM + 1 )
		return false;

	return true;
}


int GCTextColorManager::GetColorIndex( std::wstring strColor )
{
	for( int i = 0; i < (int)m_vecstrColorList.size(); ++i )
	{
		if( m_vecstrColorList[i] == strColor )
		{
			return i;
		}
	}
	return -1;
}

