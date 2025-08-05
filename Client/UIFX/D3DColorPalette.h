#ifndef _GCCOLORPALETTE_H_
#define _GCCOLORPALETTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DColorButton;
class KD3DSizingBox;

#define MAX_COLOR_BUTTON 10

class KD3DColorPalette : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KD3DColorPalette );
    /// Default constructor
    KD3DColorPalette( void );
    /// Default destructor
    virtual ~KD3DColorPalette( void );
    /// Copy constructor
    KD3DColorPalette( const KD3DColorPalette& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DColorPalette& operator=( const KD3DColorPalette& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        DEFAULT_COLOR_INDEX     = 0,
        GP_ITEM_COLOR_NUM       = 3,
        CASH_ITEM_COLOR_NUM     = 9,
        NUM_OF_PAINT_COLOR      = 10,
        ON_NO_PAINT_MASSAGEBOX  = 1001
    };

    bool            m_bEnable;
    KD3DWnd*        m_pkCheck;
    KD3DWnd*        m_apkColor[MAX_COLOR_BUTTON];

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    int GetColorIndex();
    std::wstring GetColorString(void);
    D3DCOLOR GetColor();

    std::wstring m_strSelColor;

	bool IsCashColor( std::wstring strColor );
    bool IsHaveColorPaint();
    void CheckPaletteButtons( bool bCheck ) { m_bButtonsCheck = bCheck; }
    void EnablePaletteButtons( bool bEnable );
    int GetEnablePaletteButtons(void) { return m_bEnable; }
	void SetColorIndex( int iIndex );
	void SetColorString( std::wstring strColor );
	void SetColorNum(int iNum);

protected:
    bool m_bButtonsCheck;
    int m_iColorIndex;
};

DEFINE_WND_FACTORY( KD3DColorPalette );
DECLARE_WND_FACTORY( KD3DColorPalette );
DECLARE_WND_FACTORY_NAME( KD3DColorPalette );

#endif  // _GCCOLORPALETTE_H_
