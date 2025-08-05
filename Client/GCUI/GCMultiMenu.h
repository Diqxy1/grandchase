#pragma once
//#include "uifx/D3DWnd.h"
//#include "uifx/ActionListener.h"
//#include "uifx/D3DStatic.h"
//#include "../uifx/D3DSizingBox.h"

class KGCMultiMenu : public KD3DWnd,
                     public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCMultiMenu );

    KGCMultiMenu(void);
    ~KGCMultiMenu(void);
    /// Copy constructor
    KGCMultiMenu( const KGCMultiMenu& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMultiMenu& operator=( const KGCMultiMenu& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum {
		K3DWE_MENU_CLICKED		= 200001,
	};

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    //////////////////////////////////////////////////////////////////////////
    // 오전 10:47 2008-02-21 jemitgge
    // 닉네임 하나만 있음 될듯,,
    //////////////////////////////////////////////////////////////////////////
    void SetIsRenderMsgBox( bool bRender );
    bool GetIsRenderMsgBox( void ) const { return m_bRenderMsgBox; }
    //void SetIndex( int iIndex_ );

	void AddMenu( int iKey, std::wstring strStatic, D3DCOLOR color = 0xffffffff, bool bActive = true, ED3DWndUIAlignType eWndAlignType = WNDUIAT_EXPANTION);
	void ClearMenu();

private:
	void PopupHeightChange();
	void PopupWidthChange( int iWidth );
private:
	std::map< int, int > m_mapMenuKey;
	KD3DSizingBox* m_pSizingBox;
    //std::wstring m_strBadGuy;
    bool        m_bRenderMsgBox;
	int			m_iWidth;
    //int         m_iIndex;
};

DEFINE_WND_FACTORY( KGCMultiMenu );
DECLARE_WND_FACTORY( KGCMultiMenu );
DECLARE_WND_FACTORY_NAME( KGCMultiMenu );
