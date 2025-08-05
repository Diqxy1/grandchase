#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>

class KD3DStatic;

class GCTalkList : public KD3DWnd,
					public KActionListener,
					public boost::noncopyable
{
public:
	DECLARE_CLASSNAME( GCTalkList );
	GCTalkList(void);
	virtual ~GCTalkList(void);

	virtual void ActionPerformed( const KActionEvent& event );

	enum
	{
		MAX_TALK_LIST = 3,
	};


	// 맨위 대화창 종류
	void LeftTalkList( void );
	void RightTalkList( void );
	void RefreshTalkList( void );									// TalkList를 다시 글비니다.

	// TalkList 추가 삭제
	void AddTalkList( std::pair<DWORD, DWORD> Key, std::wstring wstrTalkListName, std::wstring wstrTalkListNameColor );
	void DeleteTalkList( std::pair<DWORD, DWORD> UID );

	// TalkList 선택 상태 여부
	void InitSelectState( void );									// 선택 상태들을 초기화 시켜버립니다.
	void SelectTalkList( const int iSlotIndex );
	void SelectTalkList( std::pair< DWORD, DWORD > Key );

	bool IsTalkListIndex( int iIndex );								// m_iTalkListIndex로 설정 가능한 인덱스인지 체크합니다.
	bool IsTalkListSlotIndex( const int iSlotIndex );				// TalkList의 Slot 범위 인지 체크합니다.

	void SetTalkListTitle( const int iSlotIndex, const std::wstring wstrNickName, std::wstring wstrNickColor );
	void ToggleRenderTalkList( const int iSlotIndex, const bool bShow );
	void SetFirstIndexByKey( std::pair< DWORD, DWORD > Key );		// 첫번째 인덱스를 Key로 설정하고 Scroll을 맞춥니다.

	std::pair<DWORD, DWORD> GetTalkListKey( const int iIndex );
	int GetTalkListCount( void ) { return (int)m_vecTalkList.size(); }
	std::pair<DWORD, DWORD> GetCurTalkKey( void );
	void SetCurTalkKey( std::pair<DWORD, DWORD> Key );

	void SetNewTalk( const std::pair<DWORD, DWORD> CurTalkKey, const bool bVisible );
	void FrameMoveAlert( void );
	void DeleteAllTalkList();

protected:
	void OnCreate(void);
	void FrameMoveInEnabledState(void);

private:

	int						m_iTalkListIndex;
	std::pair<DWORD, DWORD> m_CurTalkKey;
	std::vector< std::pair< std::pair<DWORD, DWORD>, std::pair<std::wstring, std::wstring> > >	m_vecTalkList;		// Key, TalkListName

	KD3DWnd*				m_pkTalkListLeft;
	KD3DWnd*				m_pkTalkListRight;

	// 대화창 리스트를 관리 해주는 버튼
	KSafeArray<KD3DWnd*,MAX_TALK_LIST> m_pkTalkBack;
	KSafeArray<KD3DStatic*,MAX_TALK_LIST> m_pkTalkTitle;
	KSafeArray<KD3DWnd*,MAX_TALK_LIST> m_pkTalkClose;
	KSafeArray<KD3DWnd*,MAX_TALK_LIST> m_pkAlert;
	std::pair<DWORD, DWORD> m_TalkKey[MAX_TALK_LIST];
};

DEFINE_WND_FACTORY( GCTalkList );
DECLARE_WND_FACTORY( GCTalkList );
DECLARE_WND_FACTORY_NAME( GCTalkList );
