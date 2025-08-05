#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
#include "MSPacket.h"

class KD3DEdit;
class KD3DStatic;
class KD3DScrollbar;

class GCNateOnMsg : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnMsg);
	GCNateOnMsg(void);
	virtual ~GCNateOnMsg(void);

	enum NATEON_MSG_MODE
	{
		NMM_SEND = 0,
		NMM_SENDED = 1,
		NMM_RECIVE = 2,
	};

	void SetMsgMode(NATEON_MSG_MODE eMode);
	void SetCurMsg(DWORD dwUID);
	std::pair< DWORD, KPaper > GetCurMsg(void) { return m_CurMsg; }
	void AddSendedMsg(DWORD dwUID, KPaper paper);
	void AddReciveMsg(DWORD dwUID, KPaper paper);
	void ClearSendedMsg(void) { m_mapSendedMsg.clear(); }
	void ClearReciveMsg(void) { m_mapReciveMsg.clear(); }

	virtual void ActionPerformed(const KActionEvent& event);
	virtual bool InitDialog(IN const KDialogInfo& kInfo_);

protected:
	virtual void OnCreate(void);

private:

	NATEON_MSG_MODE	m_eMode;

	KD3DWnd*		m_pkTitleSend;
	KD3DWnd*		m_pkTitleSended;
	KD3DWnd*		m_pkTitleRecive;

	KD3DWnd*		m_pkSend;
	KD3DWnd*		m_pkDelete;
	KD3DWnd*		m_pkReply;
	KD3DWnd*		m_pkClose;

	KD3DEdit*		m_pkReciver;
	KD3DEdit*		m_pkMsgContent;
	KD3DStatic*		m_pkOhterReciver;
	KD3DScrollbar*	m_pkScrollBar;

	std::pair< DWORD, KPaper >		m_CurMsg;
	std::map< DWORD, KPaper >			m_mapSendedMsg;
	std::map< DWORD, KPaper >			m_mapReciveMsg;

private:
	void OnSend();
	void OnDelete();
	void OnReply();
	void OnClose();
};

DEFINE_WND_FACTORY(GCNateOnMsg);
DECLARE_WND_FACTORY(GCNateOnMsg);
DECLARE_WND_FACTORY_NAME(GCNateOnMsg);