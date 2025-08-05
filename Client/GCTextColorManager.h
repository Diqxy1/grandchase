#pragma once

#include "KNCSingleton.h"

class GCTextColorManager
{
    DeclareSingleton( GCTextColorManager );

public:
	enum
	{
		GP_ITEM_COLOR_NUM       = 3,
		CASH_ITEM_COLOR_NUM     = 9,
		NUM_OF_PAINT_COLOR      = 10,
	};

    GCTextColorManager(void);
    ~GCTextColorManager(void);
	void LoadColorScript( std::string strScriptFile );
	D3DCOLOR StringToColor( std::wstring& strColor );
	std::wstring ColorToString( DWORD ColorCode );
	void SetColorNum( int nNum );
	D3DCOLOR GetColor(int index);
	std::wstring GetColorString(int iColorIndex);
	int GetMaxColor(){return m_iNumOfColor;}
	bool IsCashColor(std::wstring strColor);
	bool IsCashColor( int iIndex);
	int GetColorIndex( std::wstring strColor );
private:
	std::vector<std::wstring>	m_vecstrColorList;
	int m_iNumOfColor;

};
DefSingletonInline( GCTextColorManager );
