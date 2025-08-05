#ifndef _D3DSTATIC_H_
#define _D3DSTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"
//#include <string>
//#include "FontManager.h"

class KD3DStatic : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DStatic );
    /// Default constructor
    KD3DStatic( void );
    /// Default destructor
    virtual ~KD3DStatic( void );
    /// Copy constructor
    KD3DStatic( const KD3DStatic& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DStatic& operator=( const KD3DStatic& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    inline void SetShadow( bool bShadow )               { m_bShadow = bShadow; }
    inline void SetShadowColor( D3DCOLOR dwColor )      { m_dwShadowColor = dwColor; }
    inline void SetMultiLine( bool bMultiLine )         { m_bMultiLine = bMultiLine; }
	inline void SetWordWrap( bool bWordWrap )			{ m_bWordWrap = bWordWrap;	 }
    inline void SetLineSpace( float fLineSpace )        { m_fLineSpace = fLineSpace; }
	inline void SetAutoScroll( bool bAutoScroll )		{ m_bAutoScroll = bAutoScroll; }
	inline void SetScrollSpeed( int iSpeed )			{ m_iScrollSpeed = iSpeed; }
	inline void SetAutoMultiLineLimit(int iAutoMultiLineLimit) {m_iAutoMultiLineLimit = iAutoMultiLineLimit;}
    inline float GetLineSpace(void) { return m_fLineSpace; }
    inline void SetOffset( const D3DXVECTOR2& vOffset ) { m_vOffset = vOffset; }
    inline D3DXVECTOR2 GetOffset( void ) { return m_vOffset; }
    inline const std::wstring& GetText( void ) const     { return m_strText; }
    inline const wchar_t* GetTextPtr() const { return m_strText.c_str(); }
    inline int  GetLineNum() { return m_iLineNum; }
    inline void SetLineNum( const int iLineNum ) { m_iLineNum = iLineNum; }
    inline bool GetAutoScroll( void )                   { return m_bAutoScroll;         }
    inline void SetFontSpace(float fFontSpace)          { m_fFontSpace = fFontSpace;    }
    inline float GetFontSpace( void )                   { return m_fFontSpace;          }
	inline bool IsEmpty()	{ return ( GetText() == L"" ); }

#if !defined( __PATH__ )
	inline void ShowStringID();
#endif
public:
	void Create( bool bAuto = false );
    virtual void PostDraw( void );
    virtual void SetText();
    virtual void SetText( int iText );
    virtual void SetText( const std::string& strText );
    virtual void SetText( const std::wstring& strText );
    virtual void SetTextPtr( const wchar_t* strText );
    virtual void SetNumber( const int iNum_ );
    virtual void SetTextAutoMultiline( const std::wstring& strText , bool bChkSpeicalChar = true,  bool bSpaceCharTrim = true);
    virtual void SetAlign(DWORD dwAlign) { m_dwAlign = dwAlign; }
	virtual void SetWidth(DWORD dwWidth );
	virtual void SetWidthDirect(DWORD dwWidth );
    virtual void SetForceWordWrapText( const std::wstring& strText, bool bWordWrap );

	POINT GetStringWidthAndHeightInPixel(bool bSetSize_ = true, int iFontSize = 0);

	void SetIconImage(std::wstring wstr);
//protected:
    virtual GCDeviceFont* GetFontMgr();
	GCDeviceFont* GetFontMgr(const int& iFontSize_ );
    inline std::wstring GetOriginalText() const
    {
        return m_strOriginal;
    }

    const wchar_t* GetOriginalTextPtr() const
    {
        return m_strOriginal.c_str();
    }

protected:
	virtual void FrameMoveInEnabledState();
	void FrameMoveAutoScroll();

	bool IsAutoScrollEnable();

	std::wstring GetSpaceFromWidth( DWORD dwWidth );

protected:
    GCImageIconTexture*     m_pkIconTexture;

    float       m_fLineSpace;
    bool        m_bShadow;
	bool		m_bAutoMultiLine;
    bool        m_bMultiLine;
	bool		m_bWordWrap;
	bool		m_bAutoScroll;
	std::wstring m_strOriginal;
    std::wstring m_strText;
    D3DXVECTOR2 m_vOffset;
    D3DCOLOR    m_dwShadowColor;
    DWORD       m_dwAlign;
    int         m_iLineNum;
	int			m_iScrollFrame;
	int			m_iScrollFrameCount;
	int			m_iLineSize;
	int			m_iScrollSpeed;
	int         m_iAutoMultiLineLimit;
    float       m_fFontSpace;

	std::wstring	m_strIconImage;

#if !defined( __PATH__ )
	std::wstring m_strStringID;
#endif

	static bool m_bIDMode;

public:
	void MakeAutoScrollString();
    virtual void BindToLua();
    void SetTextFromID( int iID );
    virtual void PushThis(lua_State* L);

private:
    void WordWrapTHAI( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ );
    void WordWrapNormal( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ );
    void WordWrapDoNotUse( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ );
};

DEFINE_WND_FACTORY( KD3DStatic );

#endif // _D3DSTATIC_H_
