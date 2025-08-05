#ifndef _GCCHATBOX_H_
#define _GCCHATBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../GCChatManager.h"
//#include "../UIFX/D3DEdit.h"
//#include "../UIFX/D3DListBox.h"
//#include "../UIFX/D3DScrollbar.h"
////#include <vector>
//#include <map>
//#include <string>
#include "GCPairPlaySystem.h"
#include "GCUI/KGCColorItemUse.h"
#include "KGCRoomManager.h"
#include "GCStateMachine.h"


#define CHAT_BOX_WHEEL_GAP      1

enum{
	KCB_CHAT_ALL = 0,
	KCB_CHAT_WHISPER,
	KCB_CHAT_GUILD,
	KCB_CHAT_NOMAL,
	KCB_CHAT_MAX_NUM,
};

enum{
	KCB_CHAT_BTN_WHISPER = 0,
	KCB_CHAT_BTN_GUILD,
	KCB_CHAT_BTN_NOMAL,
	KCB_CHAT_BTN_MAX_NUM,
};


class KGCCheckBox;
class KD3DColorPalette;

class KGCChatBoxOption : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCChatBoxOption );
	/// Default constructor
	KGCChatBoxOption( void );
	/// Default destructor
	virtual ~KGCChatBoxOption( void );
	/// Copy constructor
	KGCChatBoxOption( const KGCChatBoxOption& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCChatBoxOption& operator=( const KGCChatBoxOption& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	std::map<int,bool> GetCheck ( );
	void ClickCheckBox ( int iType );
	void InitOptionText ( );
	void OnClickCheckBox ( const KActionEvent& event );
private:
	KSafeArray< KGCCheckBox*, KCB_CHAT_MAX_NUM > m_saCheckList;
};

DEFINE_WND_FACTORY( KGCChatBoxOption );
DECLARE_WND_FACTORY( KGCChatBoxOption );
DECLARE_WND_FACTORY_NAME( KGCChatBoxOption );

class KGCChatBoxChatKind : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCChatBoxChatKind );
	/// Default constructor
	KGCChatBoxChatKind( void );
	/// Default destructor
	virtual ~KGCChatBoxChatKind( void );
	/// Copy constructor
	KGCChatBoxChatKind( const KGCChatBoxChatKind& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCChatBoxChatKind& operator=( const KGCChatBoxChatKind& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetBtnPos( );
	void OnClickMenuBtn( const KActionEvent& event );
	void SetBtnOnOff( bool bOn );
	void SetBtnNum( int iSel );
	int GetBtnNum( ){ m_iSelBtn;}
	void OnClickOpenMenuBtn( );

private:
	KSafeArray< KD3DWnd*, KCB_CHAT_BTN_MAX_NUM > m_saViewCurrentList; 
	KSafeArray< KD3DWnd*, KCB_CHAT_BTN_MAX_NUM > m_saBtnList;

	KD3DWnd* m_pkOptionBtn;
	KD3DSizingBox* m_pkBack;

	int m_iSelBtn;
};

DEFINE_WND_FACTORY( KGCChatBoxChatKind );
DECLARE_WND_FACTORY( KGCChatBoxChatKind );
DECLARE_WND_FACTORY_NAME( KGCChatBoxChatKind );


class KD3DColorButton;
class KGCCheckButton;
class KGCChatBox : public KD3DWnd,          // extends
                   public KActionListener   // implements
                   //public KGCColorItemUse   // implements
{
public:
    DECLARE_CLASSNAME( KGCChatBox );
    /// Default constructor
    KGCChatBox( void );
    /// Default destructor
    virtual ~KGCChatBox( void );
    /// Copy constructor
    KGCChatBox( const KGCChatBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChatBox& operator=( const KGCChatBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    
public:
    enum EChatBoxType
    {
        CBT_NONE,
        CBT_LOBBY,
        CBT_GUILD,
        CBT_ROOM,
        CBT_SQUARE,
        CBT_WORLDMAP,

#if defined( USE_AGIT_SYSTEM )
        CBT_AGIT,
#endif
    };

    enum EChatType
    {
        CT_ALL = 0,
        CT_WHISPER,
        CT_GUILD,
        CT_GENERAL,

        CT_NUM,
    };

	enum {
		CTSIZE_MINIMUM = 0,
		CTSIZE_NOMAL = 1,
		CTSIZE_MAXIMUM= 2,
	};

public:
    void SetText( const std::wstring& strText );
    //void OnColorBtn( const KActionEvent& event_ );
    //virtual bool IsUseColorItemCash();
    //virtual void SetColor( std::wstring strColor );
    //virtual bool IsUseColorItem() { return m_strSelColor != L"FFFFFF"; }
    void SetFocus( void );
    void AddChat( const std::wstring& strChat, int iChatType = KGCChatManager::CHAT_TYPE_NORMAL,
        std::wstring strReturnMsg = L"", DWORD dwColor = 0xFFFFFFFF );
    void OnEdit( const KActionEvent& event_ );
    void OnList( const KActionEvent& event_ );
    void OnUserList( const std::wstring& strNickName );
    void OnScroll( const KActionEvent& event_ );
    void ClearChatBox( GCPairPlaySystem::PPS_CHAT_TYPE eType );
    virtual void ActionPerformed( const KActionEvent& event );
    void InitColorButton();
    //void OffColorButton();
    void SetScrollHeight(DWORD dwHeight);
	void MoveListandScroolPosY(int iDeltaY);
    void SetListHeight(DWORD dwHeight);
    void Clear();
    void SetFilteringListInfo(int itype, bool bSet);
    bool GetFilteringListInfo(int itype);
    void ChangeChatTypePos( EChatType eType_, bool bFoldOpen_ );
    void ChangeChatTypePos( KD3DWnd* pWnd_, bool bFoldOpen_ );
    void OnClickColorBtn();

    inline bool HasEditFocus() const
    {
        return m_pkEdit->HasFocus();
    }
    // 광장 채팅 움직이는거 아름답게 맞춰 줄려고 외부에서 쓴다.
    FLOAT GetScrollPosX()
    {
        return m_pkScroll->GetCurrentWindowLocalPos().x;
    }
	FLOAT GetScrollPosY()
	{
		return m_pkScroll->GetCurrentWindowLocalPos().y;
	}
    DWORD GetScrollBtnGap(){
        return m_pkScroll->GetButtonSize();
    }
    DWORD GetScrollHeight(){
        return m_pkScroll->GetHeight();
    }
    DWORD GetListHeight(){
        return m_pkList->GetHeight();
    }

    void SetChatListClick(bool bClick); //체크리스트 클릭하지 않도록 처리하는 함수
    void ToggleShowList( bool bShow );
    bool m_bIsChatColorEnabled;
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void InitChatTypeValues();
    int GetChatTypeEnum( KD3DWnd* pWnd_ );
public:
    KD3DEdit*       m_pkEdit;
protected:
    EChatBoxType    m_eChatBoxType;

    KD3DListBox*    m_pkList;
    KD3DScrollbar*  m_pkScroll;
	
	KD3DWnd*		m_pkOptionBtn;
	KD3DColorPalette*			m_pkPaint_Btn;
    KD3DWnd*        m_pkColorBtn;
	KD3DWnd*		m_pkSizeUpBtn;
	KD3DWnd*		m_pkSizeDownBtn;
	KD3DSizingBox*  m_pkChatListBack;
	KD3DSizingBox*  m_pkScrollBack;
	KD3DSizingBox*  m_pkEditBack;
	int				m_iCurSize;

    D3DCOLOR        m_Color;
    std::wstring    m_strSelColor;
	
	KGCChatBoxOption*		m_pkOptionList;
	KGCChatBoxChatKind*		m_pkKindList;

    //KD3DColorButton* m_pkColorButton;
    //KD3DColorButton* m_pkColorButtonFFFFFF;

    typedef void (KGCChatBox::*HANDLER)( const KActionEvent& event_ );
    std::map<std::string, HANDLER>  m_mapEvent;

	//채팅 필터링용 버튼
    KSafeArray<D3DXVECTOR2,CT_NUM>         m_vChatTypeOribinalPos;
    KSafeArray<KGCCheckButton*,CT_NUM>     m_pkChatType;
    int                 m_iCurChatType;
    bool                m_bChatBtnFold;
	bool				m_bPrevChatBtnFold;

    KSafeArray<std::string,CT_NUM>  m_aChatUIName;
    KSafeArray<std::wstring,CT_NUM> m_aPreMark;
    KSafeArray<int, CT_NUM>         m_aChatType;
    std::wstring        m_strPreMark;

    bool                m_bFirstCrerate;

	std::wstring		m_strWhisperNickname;
	
	
public:
	const std::wstring GetEditStr( void ) const;

    void FoldingChatBox( bool bFold_ );
    std::wstring GetCurrentPreMark();
    void OnChatBtn( const KActionEvent& event_ );
    void FilterCheckMessageBox(int state, bool bCheck );
	void OnClickOptionBtn();
	void OnClickOption( const KActionEvent& event );
	void ChangeChatType( const KActionEvent& event );
	void OnClickSizeUpBtn();
	void OnClickSizeDownBtn();
	void ChangeChatSize(int iSize);
	void ToggleChatList ( bool bShow );
	int GetCurrentSize(){return m_iCurSize;}
    bool IsNewChatBox();
};

extern KGCChatBox* g_pkGuildChatBox;
extern KGCChatBox* g_pkRoomChatBox;
extern KGCChatBox* g_pkSquareChatBox;
extern KGCChatBox* g_pkLobbyChatBox;
extern KGCChatBox* g_pkWorldmapChatBox;
extern KGCChatBox* g_pkGuildLobbyChatBox;

#if defined( USE_AGIT_SYSTEM )
extern KGCChatBox* g_pkAgitChatBox;
#endif

DEFINE_WND_FACTORY( KGCChatBox );
DECLARE_WND_FACTORY( KGCChatBox );
DECLARE_WND_FACTORY_NAME( KGCChatBox );

#endif // _GCCHATBOX_H_
