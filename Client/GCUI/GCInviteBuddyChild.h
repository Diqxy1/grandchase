#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
#include "GCBuddyTree.h"

class KD3DCheckBox;
class KD3DStatic;

class GCInviteBuddyChild : public KD3DWnd,
	    				   public KActionListener,
						   public boost::noncopyable
{
public:
	DECLARE_CLASSNAME( GCInviteBuddyChild );
	GCInviteBuddyChild(void);
	virtual ~GCInviteBuddyChild(void);

	// functions
	void SetData( const FRIEND_UI_DATA& kData );
	void GetData( FRIEND_UI_DATA& kData ) { kData = m_kUIData; }

	enum
	{
		IBC_BUDDY_CHILD_CHECK = 9000,
	};

protected:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void PostChildDraw();

private:
	KD3DWnd* m_pkICon;
	KD3DStatic* m_pkName;
	FRIEND_UI_DATA m_kUIData;
	bool m_bFirst;
	KSafeArray<D3DXVECTOR2,2> m_vPos;
	KD3DCheckBox* m_pkCheck;
};

DEFINE_WND_FACTORY( GCInviteBuddyChild );
DECLARE_WND_FACTORY( GCInviteBuddyChild );
DECLARE_WND_FACTORY_NAME( GCInviteBuddyChild );