#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
#include "GCNateOn.h"
#include "MSPacket.h"

struct NATE_TALK_BOX
{
public:

	NATE_TALK_BOX()
	{
		m_bNew = false;
	}

	enum
	{
		MAX_TALKBOX_STR_ROW = 500,
	};

	void InsertTalk(const std::wstring& wstrTalk, DWORD dwColor)
	{
		vTalkContents.push_back(std::make_pair(wstrTalk, dwColor));
		if ((int)vTalkContents.size() > MAX_TALKBOX_STR_ROW)
			vTalkContents.pop_front();
	}

	void ClearTalk(void) { vTalkContents.clear(); }
	void InsertTalker(const DWORD dwUID, const std::wstring wstrNickName = L"", const std::wstring wstrNickColor = L"")
	{
		if (mapTalker.count(dwUID))
			mapTalker[dwUID] = std::make_pair(wstrNickName, wstrNickColor);
		else
			mapTalker.insert(std::make_pair(dwUID, std::make_pair(wstrNickName, wstrNickColor)));
	}
	void DeleteTalker(const DWORD dwUID)
	{
		if (mapTalker.count(dwUID))
			mapTalker.erase(dwUID);
	}
	void ClearTalker(void) { mapTalker.clear(); }

	std::map< DWORD, std::pair<std::wstring, std::wstring> >& GetTalker(void)
	{
		return mapTalker;
	}

	std::wstring GetTalker(const DWORD dwUID)
	{
		std::map< DWORD, std::pair<std::wstring, std::wstring> >::iterator it = mapTalker.begin();
		for (; it != mapTalker.end(); it++)
		{
			if (it->first == dwUID)
			{
				return it->second.first;
			}
		}

		return L"";
	}

	std::wstring GetTalkerColor(const DWORD dwUID)
	{
		std::map< DWORD, std::pair<std::wstring, std::wstring> >::iterator it = mapTalker.begin();
		for (; it != mapTalker.end(); it++)
		{
			if (it->first == dwUID)
			{
				return it->second.second;
			}
		}

		return L"FFFFFF";
	}

	DWORD GetTalker(const std::wstring wstrTalker)
	{
		std::map< DWORD, std::pair<std::wstring, std::wstring> >::iterator it = mapTalker.begin();
		for (; it != mapTalker.end(); it++)
		{
			if (it->second.first == wstrTalker)
			{
				return it->first;
			}
		}

		return 0;
	}

	std::set<DWORD> GetTalkerUID(void)
	{
		std::set<DWORD> temp;
		std::map< DWORD, std::pair< std::wstring, std::wstring> >::iterator it = mapTalker.begin();
		for (; it != mapTalker.end(); it++)
			temp.insert(it->first);

		return temp;
	}

	std::deque< std::pair< std::wstring, DWORD > >& GetTalkContent(void)
	{
		return vTalkContents;
	}

	int GetTalkerCount(void)
	{
		return (int)mapTalker.size();
	}

	bool IsAllTalkerEqual(std::vector< DWORD >& vtTalker)
	{
		if (vtTalker.size() != mapTalker.size())
			return false;

		for (int i = 0; i < (int)vtTalker.size(); ++i)
		{
			if (0 >= mapTalker.count(vtTalker[i]))
				return false;
		}

		return true;
	}

	bool IsNew(void) { return m_bNew; }
	void SetNew(bool bNew) { m_bNew = bNew; }

private:
	std::deque< std::pair< std::wstring, DWORD > >		vTalkContents;	// < str, color >
	std::map< DWORD, std::pair< std::wstring,std::wstring> >	mapTalker;		// < UID, NickName >
	bool												m_bNew;
};

class KD3DScrollbar;
class KD3DListBox;
class KD3DEdit;
class KD3DStatic;

class GCNateOnChat : public KD3DWnd,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnChat);
	GCNateOnChat(void);
	virtual ~GCNateOnChat(void);

	virtual void ActionPerformed(const KActionEvent& event);

	void AddChatString(std::pair<DWORD, DWORD> UID, std::wstring& strChat, DWORD dwColor);

	NATE_TALK_BOX* GetTalkBox(std::pair<DWORD, DWORD> UID);
	void AddTalkBox(std::pair<DWORD, DWORD> UID, NATE_TALK_BOX& vtTalkBox);
	void DeleteTalkBox(std::pair<DWORD, DWORD> UID);
	void RefreshTalkBox(std::pair<DWORD, DWORD> UID);
	void ClearTalkListBox(void);
	std::pair<DWORD, DWORD> GetAllTalkerEqual(std::vector< DWORD >& vtTalker);

	int GetCountTalkBox(void) { return (int)m_mapTalkBox.size(); }
	void SetCurTalkKey(std::pair<DWORD, DWORD> prCurRoomKey) { m_prCurTalkKey = prCurRoomKey; }
	std::pair<DWORD, DWORD> GetCurTalkKey(void) { return m_prCurTalkKey; }

	void RefreshTalkerList(std::pair<DWORD, DWORD> UID);

	KD3DEdit* GetEditor(void) { return m_pkTalkEditor; }
	void EnableEditor(bool bEnable);
	void OnInputEmoticon(void);
	std::wstring GetUserListBoxNickName(const int iIndex);
	std::wstring GetUserNickName(const DWORD dwUID);

	void ClearChatWindow();
	void FocusClose();

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete(void);
	virtual void FrameMoveInEnabledState();

private:
	bool						m_bEnableEditor;
	KD3DScrollbar*				m_pkScroll;
	KD3DScrollbar*				m_pkUserScroll;
	KD3DWnd*					m_pkEmoticonSelect;
	KD3DWnd*					m_pkInvite;
	KD3DListBox*				m_pkTalkBox;
	KD3DListBox*				m_pkUserList;
	KD3DEdit*					m_pkTalkEditor;

	std::pair<DWORD, DWORD>		m_prCurTalkKey;

	std::map< std::pair<DWORD, DWORD>, NATE_TALK_BOX > m_mapTalkBox;		// wstring 대신 대화 내용
};

DEFINE_WND_FACTORY(GCNateOnChat);
DECLARE_WND_FACTORY(GCNateOnChat);
DECLARE_WND_FACTORY_NAME(GCNateOnChat);
