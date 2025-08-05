#ifndef _D3DLISTBOX_H_
#define _D3DLISTBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

//#include "D3DWnd.h"
//#include "GCChatManager.h"
//#include "KD3DEmoticonString.h"
//#include <deque>
//#include <string>

#define MAX_MSGTYPE               200       // 리스트에서 필터링 할수 있는 최대 타입수
#define D3DWE_CHAT_WHEEL_UP      20001      // 클릭 되는 라인수랑 같이 넘어가긴 하지만.. 라인수가 2만 넘진 않겠지-_-;
#define D3DWE_CHAT_WHEEL_DOWN    20002
#define D3DWE_RBTN_SELECT		 20003

struct ADD_STRING_INFO
{
    D3DCOLOR dwStrColor;
    bool bAction;
    const std::vector< KD3DEmoticonString::EmotStrPair >& vecContent; // 참조만 해도 충분
    std::wstring strReturnMsg;
    int iMsgType;
    int iLineSpNumber;
	std::wstring strImageIcon;

	ADD_STRING_INFO( D3DCOLOR Color_, bool bAction_, const std::vector< KD3DEmoticonString::EmotStrPair >& vecContent_, std::wstring strReturnMsg_, int iMsgType_, int iLineSpNumber_ ,std::wstring strImageIcon_)
        : dwStrColor( Color_ )
        , bAction( bAction_ )
        , vecContent( vecContent_ )
        , strReturnMsg( strReturnMsg_ )
        , iMsgType( iMsgType_ )
        , iLineSpNumber( iLineSpNumber_ )
		, strImageIcon( strImageIcon_ )		
    {

    }
};

class KD3DListBox : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DListBox );
    /// Default constructor
    KD3DListBox( void );
    /// Default destructor
    virtual ~KD3DListBox( void );
    /// Copy constructor
    KD3DListBox( const KD3DListBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DListBox& operator=( const KD3DListBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_MAX_LINE_EX		=	1000,
        DEFAULT_MAX_LINE    = 200,
        MAX_DISPLAY_LINE    = 200,
        FIXED_ID_GAP        = 11,
    };

public:
    inline void SetTransparent( BYTE alpha )
    { m_dwTransparent = D3DCOLOR_ARGB( alpha, 0x00, 0x00, 0x00 ); }

    inline void SetShadow( bool bShadow )           { m_bShadow = bShadow; }
    inline void SetShadowColor( D3DCOLOR dwColor )  { m_dwShadowColor = dwColor; }

    inline void SetLineSpace( float fLineSpace )    { m_fLineSpace = fLineSpace; }
	inline void SetiImageIconSize( float iImageIconSize )    { m_iImageIconSize = static_cast<int>(iImageIconSize); }
    inline float GetLineSpace( void ) const         { return m_fLineSpace; }

    inline void SetMaxLine( UINT uiMaxLine )        { m_uiMaxLine = uiMaxLine; }
    inline UINT GetMaxLine( void ) const            { return m_uiMaxLine; }

    inline void SetCheckList(bool bChecklist)       { m_bCheckList=bChecklist; }
    inline int GetTopIndex( void ) const            { return m_iTopIndex; }

    inline UINT GetCount( void ) const              { return (UINT)m_deqStrContent.size(); }

    inline bool GetCheckList(void) const {return m_bCheckList;}

	float GetDeltaY(bool bHasEmoticon, bool bLinespace_=true);
    UINT GetDisplayLimitLine( void );
    UINT GetDisplayLimitLineInv( void );

    void AddString( const std::wstring& strContent,
        D3DCOLOR dwStrColor = 0xffffffff,
        bool bAction = false,
        DWORD lParam = 0,
        bool bIDGap = false,
        std::wstring strReturnMsg = L"",
        int iMsgType = 0,
		std::wstring strImageIcon = L""
		);
    void Clear( void );

    const std::wstring& GetReturnMsg( int iIndex ) const;
    void SetTopIndex( int iIndex );
	void CalcTopIndex();

    int GetViewContentSize(); //실제 출력되는 문자의 크기만 찍는 함수
    int GetFrontNIndex(int iNum); //앞에서 N번째에 출력한 글의 인덱스를 출력한다.
    int GetBackNIndex(int iNum); //뒤에서 N번째에 출력한 글의 인덱스를 출력한다.
    const std::wstring GetAllMsg( void );

    void SetFilterInfo(int iType, bool bSet ); // 필터링 설정
    bool GetFilterInfo(int iType ); // 필터링 정보 받아오는 함수

    void pop_front();

protected:
    bool CheckList( const D3DXVECTOR2& vPos, int iIndex );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    void AddString_( ADD_STRING_INFO& sInfo );
	virtual GCDeviceFont* GetFontMgr();
    void DrawContent_( const std::wstring& strContent, D3DXVECTOR2& vPos, bool bHasEmoticon, DWORD dwColor );
    void DrawEmoticon_( KGCEmoticon* pEmoticon, D3DXVECTOR2& vPos );
    int GetEmoticonLineCnt( int iStart, int iEnd );

protected:
    float                   m_fLineSpace;
    UINT                    m_uiMaxLine;
    int                     m_iTopIndex;
    int                     m_iSelect;
    int                     m_iLineSpNumber; //라인 고유 넘버, 길게 두줄로 표현됐을때 롤오버 하는 경우, 같이 롤오버 되도록 만들기 위해 사용
    bool                    m_bShadow;
    bool                    m_bChkAddstring;  //AddString 과정중에 Draw 처리때문에 크러쉬 발생 된다. 그래서 그거 확인
    D3DCOLOR                m_dwShadowColor;
    D3DCOLOR                m_dwTransparent;

    std::deque< std::vector< KD3DEmoticonString::EmotStrPair > > m_deqStrContent;
    std::deque<std::wstring> m_deqStrReturnMsg;     // 클릭시 리턴되는 메시지
    std::deque<D3DCOLOR>    m_deqStrColor;
    std::deque<bool>        m_deqAction;
    std::deque<int>         m_deqSplit;             // 분리여부 같은 글 쪼개진거 클릭시 판별에 사용
    std::deque<int>         m_deqMsgType;           // 메시지 타입에 따른 필터링 기능시 사용
    std::deque<bool>        m_deqEmoticon;          // 이모티콘이 하나라도 있는지
	std::deque<std::wstring> m_deqImageIcon;		// 각 줄에 이미지 아이콘 스트링 ID

    KSafeArray<bool,MAX_MSGTYPE> m_bTypeFilter; // 필터링 되는 목록 여부 확인
    bool                    m_bCheckList;               // 마우스 클릭기능 사용시, 사용여부

    KD3DUIVertex            m_select[4];
    UINT                    m_uiMaxDisplayLine;

	int m_iImageIconSize;
};

DEFINE_WND_FACTORY( KD3DListBox );

#endif // _D3DLISTBOX_H_
