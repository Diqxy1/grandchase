#ifndef _GCPASSWORDBOX_H_
#define _GCPASSWORDBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DEdit;

class KGCPasswordBox : public KD3DDialog,      // extends
                       public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCPasswordBox );
    /// Default constructor
    KGCPasswordBox( void );
    /// Default destructor
    virtual ~KGCPasswordBox( void );
    /// Copy constructor
    KGCPasswordBox( const KGCPasswordBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPasswordBox& operator=( const KGCPasswordBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EPasswordBoxResult
    {
        PBR_OK,
        PBR_CANCEL,
        PBR_CREATED,
        PBR_DESTROYED,
    };

public:
    const std::wstring& GetPassword( void ) const;

    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
    KD3DWnd*    m_pkOK;
    KD3DWnd*    m_pkCancel;
    KD3DEdit*   m_pkPasswd;
    std::wstring m_strPassword;

    DWORD       m_dwUse;
    DWORD		m_dwlParam;
    int			m_ilParam2;

public:
    void SetlParam( DWORD lParam ) { m_dwlParam = lParam; }
    void SetlParam2( int lParam2 ) { m_ilParam2 = lParam2; }
    DWORD GetlParam( void ) { return m_dwlParam; }	
    int GetlParam2( void ) { return m_ilParam2; }

    void SetUse( DWORD dwUse ) { m_dwUse = dwUse; }
    DWORD GetUse( void ) { return m_dwUse; }
};

DEFINE_WND_FACTORY( KGCPasswordBox );
DECLARE_WND_FACTORY( KGCPasswordBox );
DECLARE_WND_FACTORY_NAME( KGCPasswordBox );

#endif // _GCPASSWORDBOX_H_