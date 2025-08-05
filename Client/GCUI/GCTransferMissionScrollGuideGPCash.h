#pragma once

class KGCTransferMissionScrollItemWnd;

class KGCTransferMissionScrollGuideGPCash : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTransferMissionScrollGuideGPCash );
    /// Default constructor
    KGCTransferMissionScrollGuideGPCash( void );
    /// Default destructor
    virtual ~KGCTransferMissionScrollGuideGPCash( void );
    /// Copy constructor
    KGCTransferMissionScrollGuideGPCash( const KGCTransferMissionScrollGuideGPCash& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTransferMissionScrollGuideGPCash& operator=( const KGCTransferMissionScrollGuideGPCash& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnClose;

    //KD3DStatic*         m_pkStatic;

    KGCTransferMissionScrollItemWnd*     m_pkItemBoxGP;
    KGCTransferMissionScrollItemWnd*     m_pkItemBoxCash;



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();

    void SetMissionItemID( DWORD& dwGPMissionItemID, DWORD& dwCashMissionItemID );



};

DEFINE_WND_FACTORY( KGCTransferMissionScrollGuideGPCash );
DECLARE_WND_FACTORY( KGCTransferMissionScrollGuideGPCash );
DECLARE_WND_FACTORY_NAME( KGCTransferMissionScrollGuideGPCash );
