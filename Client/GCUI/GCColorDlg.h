#ifndef _GCCOLOR_DLG_H_
#define _GCCOLOR_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

#define GC_COLORDLG_COLORNUM		(10)

class KD3DStatic;
class KD3DColorButton;

enum GC_COLORDLG_TYPE
{
    TYPE_MSGBOX = 0,
    TYPE_GAMEOVERUI,
    MSGBOX_TYPE__NUM,
};
class KGCColorDlg : public KD3DDialog,   // extends
                         public KActionListener // implements
{
public:
	enum
	{
		GP_ITEM_COLOR_NUM = 4,
		CASH_ITEM_COLOR_NUM = 10,
	};
    DECLARE_CLASSNAME( KGCColorDlg );
    /// Default constructor
    KGCColorDlg( void );
    /// Default destructor
    virtual ~KGCColorDlg( void );
    /// Copy constructor
    KGCColorDlg( const KGCColorDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCColorDlg& operator=( const KGCColorDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );	
	void LoadColorScript(std::string strScriptFile);	
	static D3DCOLOR StringToColor(std::wstring& strColor);
	std::wstring IndexToColorString(int iIndex);
	std::wstring GetSelectColor() { return m_strSelColor; }
	bool IsCashColor(std::wstring strColor);
	void SetPalletSize(int iSize);
    void SetOriginPos() { SetAllWindowPos(m_vecOriginPos ); }
    void CancelDlg();
    void SetDlgType(int nDlgType);
    KD3DStatic* GetCaption() { return m_pkCaption;}

protected:
    virtual void OnCreate( void );
	void FrameMoveInEnabledState( void );
	void OnCancel();
    
protected:    
	KSafeArray<KD3DColorButton*,GC_COLORDLG_COLORNUM> m_pkColorButton;    
	KD3DStatic*	m_pkCaption;
	
	std::wstring m_strSelColor;
	std::vector<std::wstring>	m_vecstrColorList;
	int							m_iPalleteSize;
    int                         m_eDlgType;

    D3DXVECTOR2         m_vecOriginPos;
};

extern KGCColorDlg* g_pColorDlg;

DEFINE_WND_FACTORY( KGCColorDlg );
DECLARE_WND_FACTORY( KGCColorDlg );
DECLARE_WND_FACTORY_NAME( KGCColorDlg );

#endif // _GCCOLOR_DLG_H_
