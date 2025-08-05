#pragma once


class KGCTransferMissionScrollGuideGPCash;
class KGCTransferMissionScrollGuideCash;


class KGCTransferMissionScrollGuideDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTransferMissionScrollGuideDlg );
    /// Default constructor
    KGCTransferMissionScrollGuideDlg( void );
    /// Default destructor
    virtual ~KGCTransferMissionScrollGuideDlg( void );
    /// Copy constructor
    KGCTransferMissionScrollGuideDlg( const KGCTransferMissionScrollGuideDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTransferMissionScrollGuideDlg& operator=( const KGCTransferMissionScrollGuideDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum MSGT_Type {
        MSGT_GP_CASH = 0,
        MSGT_CASH,
		MSGT_GP,
        MSGT_MAX
    };


private:
    KD3DStatic*         m_pkStaticMent;

    KGCTransferMissionScrollGuideCash* m_pkTransferMissionScrollGuideCash;
    KGCTransferMissionScrollGuideGPCash* m_pkTransferMissionScrollGuideGpCash;

    DWORD m_dwGPMissionItemID;
    DWORD m_dwCashMissionItemID;

    bool m_bGotMsg;
    int  m_nGuideType;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();

    void SetMissionGuideInfo( KEVENT_JOB_MISSION_GUIDE_NOT& kRecv_ );

    void ShowMissionGuide();


};

DEFINE_WND_FACTORY( KGCTransferMissionScrollGuideDlg );
DECLARE_WND_FACTORY( KGCTransferMissionScrollGuideDlg );
DECLARE_WND_FACTORY_NAME( KGCTransferMissionScrollGuideDlg );
