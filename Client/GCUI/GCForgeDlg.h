#pragma once

#include "GCForgeItemBreakUp.h"
#include "GCForgeItemCreation.h"
#include "GCForgePageChange.h"
#include "GCForgeItemCraftMachine.h"


class KGCForgeDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCForgeDlg );
    /// Default constructor
    KGCForgeDlg( void );
    /// Default destructor
    virtual ~KGCForgeDlg( void );
    /// Copy constructor
    KGCForgeDlg( const KGCForgeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCForgeDlg& operator=( const KGCForgeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

private:

    KD3DWnd     *m_pkCreateBtn;
    KD3DWnd     *m_pkSelectedCreateBtn;

    KD3DWnd     *m_pkBreakUpBtn;
    KD3DWnd     *m_pkSelectedBreakUpBtn;

    KD3DWnd     *m_pkCloseBtn;

    KGCForgeItemCreation *m_pkItemCreation;

    KGCForgePageChange *m_pkPageChange;

public:
    KGCForgeItemBreakUp* m_pkItemBreakUp;

    KGCForgeItemCraftMachine* m_pkCraftMachine;

    void OnClickCreateBtn();
    void OnClickBrakeUpBtn();

    void OnClickCloseBtn();

    enum EKGCFORGE_STATE{
        EKGCFOGE_CREATE = 0,
        EKGCFOGE_BREAKUP,
    };
    void ChangeState(int iState);

    bool IsEnabledRecipeBar( GCITEMID itemID );
    int  GetForgeCraftState( void ) { return m_pkCraftMachine->GetForgeCraftState(); }

    void SetSelectedMatertial( IN const GCITEMUID itemUID_ );

    void UpdateRecipeUI();

};

DEFINE_WND_FACTORY( KGCForgeDlg );
DECLARE_WND_FACTORY( KGCForgeDlg );
DECLARE_WND_FACTORY_NAME( KGCForgeDlg );
