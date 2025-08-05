#ifndef _GCMESSAGEBOX_H_
#define _GCMESSAGEBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCMessageBox : public KD3DDialog,      // extends
                      public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCMessageBox );
    /// Default constructor
    KGCMessageBox( void );
    /// Default destructor
    virtual ~KGCMessageBox( void );
    /// Copy constructor
    KGCMessageBox( const KGCMessageBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator

    const KGCMessageBox& operator=( const KGCMessageBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    
public:
    enum EMessageBoxResult
    {
        MBR_OK,
        MBR_YES,
        MBR_NO,
        MBR_CREATED,
        MBR_DESTROYED,
		MBR_CHACK,
		MBR_CANSEL,
		MBR_JOIN_USER,
		MBR_USER_INFO,
    };

	enum EMessageBoxBtnType
	{
		MBT_OK,
		MBT_YES_NO,
		MBT_CHECK_CANSEL,
		MBT_JOIN_CANSEL,
		MBT_INFO_CANSEL,
	};

	enum EMessageBoxSendEvent
	{
		MBSE_NONE = 0,
		MBSE_D3DWE_AGREEMENT_OK,

	};
public:
	void SetType( int iType );
	void SetText( const std::wstring& strText1, const std::wstring& strText2 );
    void SetText( int iIndex, const std::wstring& strText );
	const std::wstring& GetText( int iIndex ) const;

    void SetUse( DWORD dwUse );        
    DWORD GetUse( void ) const;

	void SetlParam( KEVETPARM lParam ) { m_dwlParam = lParam; }
	void SetlParam2( KEVETPARM lParam2 ) { m_ilParam2 = lParam2; }
	KEVETPARM GetlParam( void ) { return m_dwlParam; }	
	KEVETPARM GetlParam2( void ) { return m_ilParam2; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    int        m_iBtnType;
    DWORD       m_dwUse;
    KSafeArray<KD3DStatic*,4> m_pkText;    
    KD3DWnd*    m_pkOK;
    KD3DWnd*    m_pkYes;
    KD3DWnd*    m_pkSlash;
	KD3DWnd*    m_pkNo;
	KD3DWnd*    m_pkCheck;
	KD3DWnd*    m_pkCansel;
	KD3DWnd*    m_pkJoinUser;
	KD3DWnd*    m_pkUserInfo;
	KEVETPARM	m_dwlParam;
	KEVETPARM	m_ilParam2;
	KD3DSizingBox*			m_pkBackDesc;

    bool        m_bIsSizingBox;

};

DEFINE_WND_FACTORY( KGCMessageBox );
DECLARE_WND_FACTORY( KGCMessageBox );
DECLARE_WND_FACTORY_NAME( KGCMessageBox );

#endif // _GCMESSAGEBOX_H_