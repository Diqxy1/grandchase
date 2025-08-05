#pragma once

//#include "../uifx/D3DWnd.h"
//#include <boost/noncopyable.hpp>

class KD3DStatic;
class KD3DEdit;

class GCBlockToAddFriend : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME( GCBlockToAddFriend );
	GCBlockToAddFriend(void);
	virtual ~GCBlockToAddFriend(void);

	void SetFriendNickName( const DWORD dwUID, const std::wstring wstrNickName );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState();
	virtual void OnDestroyComplete();
	void OnOK();
	void OnCancel();

private:
	KD3DWnd* m_pkOK;
	KD3DWnd* m_pkCancel;
	KD3DStatic* m_pkTitle;
	KD3DStatic* m_pkDesc;
	KD3DStatic* m_pkDesc2;

	DWORD			m_dwUID;
	std::wstring	m_wstrNickName;
};

DEFINE_WND_FACTORY( GCBlockToAddFriend );
DECLARE_WND_FACTORY( GCBlockToAddFriend );
DECLARE_WND_FACTORY_NAME( GCBlockToAddFriend );
