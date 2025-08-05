#ifndef _KGCASKUSEVIRTUALCASH_H_
#define _KGCASKUSEVIRTUALCASH_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DWnd;
class KD3DStatic;
class KD3DCheckBox;
class KD3DSizingBox;

class KGCAskUseVirtualCash : public KD3DDialog,
	                   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCAskUseVirtualCash );
	/// Default constructor
	KGCAskUseVirtualCash( void );
	/// Default destructor
	virtual ~KGCAskUseVirtualCash( void );
	/// Copy constructor
	KGCAskUseVirtualCash( const KGCAskUseVirtualCash& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAskUseVirtualCash& operator=( const KGCAskUseVirtualCash& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    inline void SetItemInfo( int iItemID, int iPeriod ) { m_iItemID = iItemID; m_iPeriod = iPeriod; }
    void BuyItem( bool bVirtualCash );
    void SetNoDiagBox( bool bNoDiagBox_ ) { m_bNoDiagBox = bNoDiagBox_; }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DStatic*             m_pkTitle;
    KD3DStatic*             m_pkMainContent;
    KD3DCheckBox*           m_pkCheckBox;
    KD3DSizingBox*          m_pkBackGround;
    KD3DWnd*                m_pkOkBtn;
    KD3DWnd*                m_pkCancelBtn;

    int                     m_iItemID;
    int                     m_iPeriod;

    bool                    m_bNoDiagBox;
};

DEFINE_WND_FACTORY( KGCAskUseVirtualCash );
DECLARE_WND_FACTORY( KGCAskUseVirtualCash );
DECLARE_WND_FACTORY_NAME( KGCAskUseVirtualCash );

#endif