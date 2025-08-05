#pragma once

class KGCCoupleMessage : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleMessage );

	KGCCoupleMessage(void);
	virtual ~KGCCoupleMessage(void);
	/// Copy constructor
	KGCCoupleMessage( const KGCCoupleMessage& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleMessage& operator=( const KGCCoupleMessage& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum ECoupleMessageBoxResult
	{
		CMBR_OK,
		CMBR_CANCLE,
		CMBR_YES,
		CMBR_NO,
	};

	enum ECoupleMessageBoxUse
	{
		CMBR_USE_OK_ONLY,
		CMBR_USE_OK_CANCLE,
	};

public:
	virtual void OnCreate( void );
	//virtual void PostChildDraw( void );
	//virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

	void SetMessage( std::wstring strMsg );
	void SetButton( ECoupleMessageBoxUse eUse );
	void SetUse( const int eUse );
	KGCUIScene::EGCMsgBoxUse GetUse() { return m_eUse; }

private:
	void OnOk();
	void OnCancle();

private:
	KGCUIScene::EGCMsgBoxUse m_eUse;
	KD3DStatic*		m_pkMessage;
	KD3DWnd*		m_pkOk;
	KD3DWnd*		m_pkCancle;
	KD3DWnd*		m_pkOk_Center;
};

DEFINE_WND_FACTORY( KGCCoupleMessage );
DECLARE_WND_FACTORY( KGCCoupleMessage );
DECLARE_WND_FACTORY_NAME( KGCCoupleMessage );
