#ifndef _KGCTOOLTIP_H_
#define _KGCTOOLTIP_H_

//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class KD3DSizingBox;
class GCDeviceFont;

class KGCToolTip : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCToolTip );
    /// Default constructor
    KGCToolTip( void );
    /// Default destructor
    virtual ~KGCToolTip( void );
    /// Copy constructor
    KGCToolTip( const KGCToolTip& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCToolTip& operator=( const KGCToolTip& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void ShowToolTip( KD3DWnd::uid_type WndUID, std::wstring& strText, D3DXVECTOR2 vToolTipOffset, bool bMultiLine = false, ED3DWndUIAlignType eWndAlignType = WNDUIAT_EXPANTION, ED3DWndToolTipAlignType eAlignType = ED3DWND_TOOLTIP_ALIGN_NORMAL);
	void SetNormalText( const std::wstring& strText );
	void SetMultiText( const std::wstring& strText );

	GCDeviceFont* GetFontMgr(void);
	GCDeviceFont* GetFontMgr(int iFontSize);
	
    void FrameMove();

    void DeleteParent(){ m_ParentUID = -1; }
	void SetTextAlign( DWORD alignType );

protected:
    virtual void OnCreate();

private:
    KD3DStatic* m_pkTitle;
    KD3DSizingBox* m_pkBack;
    KD3DWnd::uid_type m_ParentUID; // Parent에 마우스가 올려져 있는지 체크하기 위해서 포인터를 가지고 있는다
};

DEFINE_WND_FACTORY( KGCToolTip );
DECLARE_WND_FACTORY( KGCToolTip );
DECLARE_WND_FACTORY_NAME( KGCToolTip );

#endif //_KGCTOOLTIP_H_
