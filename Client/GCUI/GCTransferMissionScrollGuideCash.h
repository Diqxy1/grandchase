#pragma once

class KGCTransferMissionScrollItemWnd;

class KGCTransferMissionScrollGuideCash : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTransferMissionScrollGuideCash );
    /// Default constructor
    KGCTransferMissionScrollGuideCash( void );
    /// Default destructor
    virtual ~KGCTransferMissionScrollGuideCash( void );
    /// Copy constructor
    KGCTransferMissionScrollGuideCash( const KGCTransferMissionScrollGuideCash& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTransferMissionScrollGuideCash& operator=( const KGCTransferMissionScrollGuideCash& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnClose;

    //    KD3DStatic*         m_pkStatic;

    KGCTransferMissionScrollItemWnd*     m_pkItemBoxCash;


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();

    void SetMissionItemID( DWORD& dwCashMissionItemID );



};

DEFINE_WND_FACTORY( KGCTransferMissionScrollGuideCash );
DECLARE_WND_FACTORY( KGCTransferMissionScrollGuideCash );
DECLARE_WND_FACTORY_NAME( KGCTransferMissionScrollGuideCash );
