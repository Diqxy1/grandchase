#pragma once

class KGCCoupleGoodbye : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleGoodbye );

	KGCCoupleGoodbye(void);
	virtual ~KGCCoupleGoodbye(void);
	/// Copy constructor
	KGCCoupleGoodbye( const KGCCoupleGoodbye& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleGoodbye& operator=( const KGCCoupleGoodbye& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	//virtual void PostChildDraw( void );
	//virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

	void SetUse( bool bNotify );
	void SetData( std::wstring strMessage );

private:
	void OnOk();
	void OnCancle();

private:
	KD3DStatic*		m_pkDesc;
	KD3DStatic*		m_pkStatic;
	KD3DEdit*		m_pkMessageEdit;
	KD3DStatic*		m_pkMessageStatic;

	KD3DWnd*		m_pkOk;
	KD3DWnd*		m_pkOk_Center;
	KD3DWnd*		m_pkCancle;
};

DEFINE_WND_FACTORY( KGCCoupleGoodbye );
DECLARE_WND_FACTORY( KGCCoupleGoodbye );
DECLARE_WND_FACTORY_NAME( KGCCoupleGoodbye );
