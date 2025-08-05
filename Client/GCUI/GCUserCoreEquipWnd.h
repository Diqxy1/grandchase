#ifndef _GCUSERCOREEQUIPWND_H_
#define _GCUSERCOREEQUIPWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUserEquipWnd.h"

class KGCUserCoreEquipWnd : public KGCUserEquipWnd          // extends
{
public:
	DECLARE_CLASSNAME( KGCUserCoreEquipWnd );
	/// Default constructor
	KGCUserCoreEquipWnd( void );
	/// Default destructor
	virtual ~KGCUserCoreEquipWnd( void );
	/// Copy constructor
	KGCUserCoreEquipWnd( const KGCUserCoreEquipWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCUserCoreEquipWnd& operator=( const KGCUserCoreEquipWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void SetSocketItem( KItem* pInvenItem_ ){ m_pkSocketWnd->SetItem( pInvenItem_ ); }
	KGCSocketWnd* GetSocketWnd(){ return m_pkSocketWnd; }

	KGCSocketWnd* m_pkSocketWnd;

#ifdef WEAPONCHANGE
    KD3DWnd*        m_pWCBack;
    KD3DWnd*        m_pWCLockBtn;
    KD3DWnd*        m_pWCBtn;
    void SetWCBtn( bool bLock_ );
#endif
};

DEFINE_WND_FACTORY( KGCUserCoreEquipWnd );
DECLARE_WND_FACTORY( KGCUserCoreEquipWnd );
DECLARE_WND_FACTORY_NAME( KGCUserCoreEquipWnd );

#endif // _GCUSERCOREEQUIPWND_H_