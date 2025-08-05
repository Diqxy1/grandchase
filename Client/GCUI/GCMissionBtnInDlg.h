#pragma once
//#include "../uifx/D3DWnd.h"

class KGCMissionBtnInDlg:   public KD3DWnd,
                            public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCMissionBtnInDlg );
    /// Default constructor
    KGCMissionBtnInDlg( void );
    /// Default destructor
    virtual ~KGCMissionBtnInDlg( void );
    /// Copy constructor
    KGCMissionBtnInDlg( const KGCMissionBtnInDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionBtnInDlg& operator=( const KGCMissionBtnInDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
    virtual void OnCreate();
    virtual void ActionPerformed(const KActionEvent& event );

    void OnComplete( DWORD dwCode );
    void OnFailed( DWORD dwCode );

public:
    void SetMissionID( DWORD dwMissionID ) { m_dwMissionID = dwMissionID; }
    void SetSelectionItem( DWORD dwItemID ) { m_dwSelectionID = dwItemID; }
    void SetComplete( bool bIsComplete = false );
    void SetFailed( bool bIsFailed = true );
    void SetCheckCompleteBtn( bool bComplete ) {   m_bComplete = bComplete;    }
        
    void SetCompleteToolTip( const std::wstring& strComment );
    bool IsCompletBntEnable() { return m_pkComplete->IsRenderOn(); }
    
    

protected:
    bool    m_bComplete;
    DWORD   m_dwMissionID;
    DWORD   m_dwSelectionID;
    KD3DWnd*    m_pkComplete;
    KD3DWnd*    m_pkFailed;
    KD3DWnd*    m_pkDisComplete;
};

DEFINE_WND_FACTORY( KGCMissionBtnInDlg );
DECLARE_WND_FACTORY( KGCMissionBtnInDlg );
DECLARE_WND_FACTORY_NAME( KGCMissionBtnInDlg );
