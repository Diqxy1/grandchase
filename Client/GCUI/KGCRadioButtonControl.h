#pragma once

class KGCRadioButtonControl	:	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRadioButtonControl );
	/// Default constructor
	KGCRadioButtonControl( void );
	/// Default destructor
	virtual ~KGCRadioButtonControl( void );
	/// Copy constructor
	KGCRadioButtonControl( const KGCRadioButtonControl& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRadioButtonControl& operator=( const KGCRadioButtonControl& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        D3DWE_CHECK_ENABLED = KD3DWnd::K3DWE_USER + 1,
        D3DWE_CHECK_DISABLED,
    };

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	virtual void PostChildDraw( void );
	//virtual void OnDestroyComplete( void );

private:
    KD3DWnd*        m_pkCheckBack;
    KD3DWnd*        m_pkCheckSign;

    KD3DStatic*     m_pkStatic;

public:
    void SetStatic(std::wstring& wstr, DWORD dwAlgin = DT_LEFT );
    bool IsChecked();
    void RenderCheckSign(bool bRender );
};

DEFINE_WND_FACTORY( KGCRadioButtonControl );
DECLARE_WND_FACTORY( KGCRadioButtonControl );
DECLARE_WND_FACTORY_NAME( KGCRadioButtonControl );
