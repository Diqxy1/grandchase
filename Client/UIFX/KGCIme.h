#ifndef _KGCIME_H_
#define _KGCIME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d9.h>
#include <imm.h>
//#include <string>
////#include <vector>
//#include "define.h"


enum EIMEState
{
	NOTHING,
	COMPS_START,
	COMPS_DO,
	READING_DO,
	CANDS_DO,
};
enum EIMEMode
{
	IMEUI_MODE_OFF, 
	IMEUI_MODE_ON, 
	IMEUI_MODE_ENGLISH, 
};

//En, 중국, 대만, 한국, 일본
enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };

class KGCIme
{
public:
	KGCIme(void);
	~KGCIme(void);

public:
	bool IsNativeInputMode();
	void SetNativeInputMode( bool bNative );
	void Create( HWND hWnd );
	void Destroy();

	EIMEState GetIMEState(){return m_eIMEState;}

	void GetCand( int iIndex, WCHAR* strCand );
	const WCHAR* GetComp(){return m_strComp;}
	const WCHAR* GetResult(){return m_strResult;}

	DWORD GetCandCount();
	DWORD GetCandSel();
	DWORD GetCandPageStart();
	DWORD GetCandPageSize();

	int GetTotalPage();
	int GetCurrentPage();

	void SetIMEState(EIMEState eState){m_eIMEState = eState;}
	void SetComp(const WCHAR* strComp){wcscpy(m_strComp, strComp);}
	void SetResult(const WCHAR* strResult){wcscpy(m_strResult, strResult);}
	void ClearIME();

	LRESULT ImeProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

public:

	/*2006-06-30 이후 새로 도입한 함수들 -> IME개량!! DXUT에서 가져온 함수들이다.
	static으로 선언한 이유는, Ime모드는 프로그램 하나당 하나라는 가정에서이다.
	edit창 별로 다른 것들은 static함수로 처리하면 안 된다.
	- >(참고로 Ime클래스 싱글턴으로 하면 에러많다. 왠만하면 그리하지 마세요 )*/

	static void SetImeApi();	
	static void IMEEnable( HWND hWnd , bool bEnable ); 

	static WORD GetLanguage() { return LOWORD( s_hklCurrent ); }
	static WORD GetPrimaryLanguage(){return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }
	static WORD GetSubLanguage(){return SUBLANGID( LOWORD( s_hklCurrent ) ); }

	static void GetInputLocale();						//현재 존재하는 ime 모두 가져와서 s_Locale 변수에 넣는다.
	static void CheckInputLocale();						//선택된 ime가 뭔지 파악한다.
	static void ResetInputLocale();						//현재 쓰는 ime를 잠시 껐다 킨다. 바보같은 Ime들이 있어서...

	static EIMEMode CheckImeMode( HWND hWnd );			//현재 무슨 모드인지 - 영어인가 아니면 native인가 - 체크한다.
	static void CancelIME();							//ime작동을 막는다. (임시방편,끄는 건 아니다.)

	// Function pointers: Traditional Chinese IME
	static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

	static void	OnFocusIn( HWND hWnd );
	static void	OnFocusOut( HWND hWnd );

    int GetMaxCadidateSize()
    {
        return m_iMaxCadidateSize;
    }

    void SetMaxCadidateSize(int MaxCadidateSize)
    {
       m_iMaxCadidateSize = MaxCadidateSize;
    }
private:
	EIMEState			m_eIMEState;

	HWND				m_hEdit;

	WCHAR				m_strComp[MAX_PATH];
	WCHAR				m_strResult[MAX_PATH];
	std::vector<BYTE>	m_vecCandidate;
    int                 m_iMaxCadidateSize;

	struct KInputLocale
	{
		HKL   m_hKL;            // Keyboard layout
		WCHAR m_wszLangAbb[3];  // Language abbreviation
		WCHAR m_wszLang[64];    // Localized language name
	};

	static	bool		s_bHKLChanged;		//key board layout이 바뀌었는가?
	static	bool		s_bIMEAble;			//IME가 사용가능한가? 아닌가?
	static	HKL			s_hklCurrent;       // Current keyboard layout of the process
	static	HINSTANCE	s_hDllIme;			// Instance handle of the current IME module
	static	EIMEMode	s_ImeMode;          // IME global state , Mode.

	static  std::vector< KInputLocale > s_Locale; // Array of loaded keyboard layout on system

	static  WCHAR		s_aszIndicator[5][3];  // String to draw to indicate current input locale
	static	LPWSTR		s_wszCurrIndicator;    // Points to an indicator string that corresponds to current input locale

public:
	static	HIMC		s_hImcDef;			// Default input context

    static LPWSTR GetIndicator()
    {
        return s_wszCurrIndicator;
    }
};

#endif // _KGCIME_H_

// KeyboardLayout으로 부터 적합한 멀티바이트 CodePage를 찾아내는 함수들...
// 현재는 미사용이나 추후 필요할지도 몰라 주석처리해서 박아둠

///*----------------------------------------------------------------------------*/ 
//static int	GetCharsetFromLang( LANGID langid )
//{
//    switch( PRIMARYLANGID(langid) )
//    {
//    case LANG_JAPANESE:
//        return SHIFTJIS_CHARSET;
//    case LANG_KOREAN:
//        return HANGEUL_CHARSET;
//    case LANG_CHINESE:
//        switch( SUBLANGID(langid) )
//        {
//        case SUBLANG_CHINESE_SIMPLIFIED:
//            return GB2312_CHARSET;
//        case SUBLANG_CHINESE_TRADITIONAL:
//            return CHINESEBIG5_CHARSET;
//        default:
//            return ANSI_CHARSET;
//        }
//    case LANG_GREEK:
//        return GREEK_CHARSET;
//    case LANG_TURKISH:
//        return TURKISH_CHARSET;
//    case LANG_HEBREW:
//        return HEBREW_CHARSET;
//    case LANG_ARABIC:
//        return ARABIC_CHARSET;
//    case LANG_ESTONIAN:
//    case LANG_LATVIAN:
//    case LANG_LITHUANIAN:
//        return BALTIC_CHARSET;
//    case LANG_THAI:
//        return THAI_CHARSET;
//    case LANG_CZECH:
//    case LANG_HUNGARIAN:
//    case LANG_POLISH:
//    case LANG_CROATIAN:
//    case LANG_MACEDONIAN:
//    case LANG_ROMANIAN:
//    case LANG_SLOVAK:
//    case LANG_SLOVENIAN:
//        return EASTEUROPE_CHARSET;
//    case LANG_RUSSIAN:
//    case LANG_BELARUSIAN:
//    case LANG_BULGARIAN:
//    case LANG_UKRAINIAN:
//        return RUSSIAN_CHARSET;
//    default:
//        return ANSI_CHARSET;
//    }
//}
//
///*----------------------------------------------------------------------------*/ 
//static int	GetCodePageFromCharset( int charset )
//{
//    switch( charset )
//    {
//    case SHIFTJIS_CHARSET:
//        return 932;
//    case HANGUL_CHARSET:
//        return 949;
//    case GB2312_CHARSET:
//        return 936;
//    case CHINESEBIG5_CHARSET:
//        return 950;
//    case GREEK_CHARSET:
//        return 1253;
//    case TURKISH_CHARSET:
//        return 1254;
//    case HEBREW_CHARSET:
//        return 1255;
//    case ARABIC_CHARSET:
//        return 1256;
//    case BALTIC_CHARSET:
//        return 1257;
//    case THAI_CHARSET:
//        return 874;
//    case EASTEUROPE_CHARSET:
//        return 1250;
//    default:
//        return 1252;
//    }
//}
//
///*----------------------------------------------------------------------------*/ 
//static int	GetCodePageFromLang( LANGID langid )
//{
//    return GetCodePageFromCharset(GetCharsetFromLang(langid));
//}
//
///*----------------------------------------------------------------------------*/ 