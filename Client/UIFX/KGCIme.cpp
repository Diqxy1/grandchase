#include "stdafx.h"
#include "KGCIme.h"

//
#include "../GCUI/KGCCandidateWnd.h"

HKL			KGCIme::s_hklCurrent	= NULL;
HIMC		KGCIme::s_hImcDef		= NULL;   // Default input context
HINSTANCE	KGCIme::s_hDllIme		= NULL;
bool		KGCIme::s_bIMEAble		= true;
bool		KGCIme::s_bHKLChanged	= false;
EIMEMode	KGCIme::s_ImeMode		= IMEUI_MODE_OFF;
std::vector<KGCIme::KInputLocale>	KGCIme::s_Locale;
WCHAR		KGCIme::s_aszIndicator[5][3] = // String to draw to indicate current input locale
{
	L"En",			//아직은 쓰지 않지만. 필요할 수 있기에 미리 정의한다.
		L"\x7B80",		//이 변수는 Input Locale Indicator를 윈도우에 의지하지 않고 스스로 그리기 위한
		L"\x7E41",		//것들이다. 렌더링할 때 이 글자를 렌더링하면 어떤 나라 Ime인지 인식할 수 있다.
		L"\xAC00",
		L"\x3042",
};
// Points to an indicator string that corresponds to current input locale
LPWSTR    KGCIme::s_wszCurrIndicator = KGCIme::s_aszIndicator[0];



// Traditional Chinese IME
UINT (WINAPI * KGCIme::_GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT )
= NULL;
BOOL (WINAPI * KGCIme::_ShowReadingWindow)( HIMC, BOOL )
= NULL;


KGCIme::KGCIme(void)
{
	m_hEdit		= NULL;
	m_eIMEState = NOTHING;
    m_iMaxCadidateSize = MAX_CANDIDATE_SIZE; //Default 9로 설정

	ZeroMemory( m_strComp, sizeof(m_strComp) );
	ZeroMemory( m_strResult, sizeof(m_strResult) );
}

KGCIme::~KGCIme(void)
{
}


void KGCIme::SetImeApi()
{
	char szImeFile[MAX_PATH + 1] = {0,};

	if( ImmGetIMEFileNameA( s_hklCurrent, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) == 0 )
		return;

	if( s_hDllIme )
	{
		_GetReadingString = NULL;
		_ShowReadingWindow = NULL;

		FreeLibrary( s_hDllIme );
		s_hDllIme = NULL;
	}		

	s_hDllIme = LoadLibraryA( szImeFile );

	if ( s_hDllIme == NULL )
		return;

	_GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
		( GetProcAddress( s_hDllIme, "GetReadingString" ) );
	_ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
		( GetProcAddress( s_hDllIme, "ShowReadingWindow" ) );	

	if( _ShowReadingWindow )
	{
		if ( NULL != ( s_hImcDef ) )
		{
			//printf("show Reading Wnd FALSE!\n");
			_ShowReadingWindow( s_hImcDef, false );			
		}
	}

}

void KGCIme::Create( HWND hWnd )
{
	m_hEdit = hWnd;

	CheckImeMode(m_hEdit); 
	SetImeApi();

	ImmAssociateContext( m_hEdit, s_hImcDef );
	s_bIMEAble = true;

	static HIMC s_hPrevImc = NULL;
	if( s_hPrevImc == s_hImcDef )
	{
		//printf("Imc was same\n");
		return;
	}
	else
	{
		//printf("Imc is diffrent!\n");
		s_hPrevImc = s_hImcDef;
	}

	//Composition Wnd 숨기기
	COMPOSITIONFORM cf;
	cf.dwStyle = CFS_FORCE_POSITION;
	cf.ptCurrentPos.x = 2000;
	cf.ptCurrentPos.y = 2000;
	SetRect(&cf.rcArea,0,0,0,0);
	ImmSetCompositionWindow(  s_hImcDef, &cf );

	////Candidate Wnd 숨기기
	//CANDIDATEFORM cdf;
	//cdf.dwStyle = CFS_FORCE_POSITION;
	//cdf.ptCurrentPos.x = 2000;
	//cdf.ptCurrentPos.y = 2000;
	//SetRect(&cdf.rcArea,0,0,0,0);
	//ImmSetCandidateWindow( s_hImcDef , &cdf );

	ClearIME();  //변수들 초기화. 버그 방지!!!
}

void KGCIme::Destroy()
{
	if( s_hDllIme )
	{
		_GetReadingString = NULL;
		_ShowReadingWindow = NULL;

		FreeLibrary( s_hDllIme );
		s_hDllIme = NULL;
	}

	if( s_hImcDef )
		s_hImcDef = NULL;

	ImmAssociateContext( m_hEdit, NULL );



}

void KGCIme::GetCand( int iIndex, WCHAR* strCand )
{
	if(m_vecCandidate.empty()) 
	{
		wcscpy( strCand, L"" );
	}
	else 
	{
		CANDIDATELIST* candidateList = (CANDIDATELIST*)&m_vecCandidate[0];

		if(iIndex >= (int)candidateList->dwCount)
		{
			wcscpy( strCand, L"" );
		} 
		else
		{
			wcscpy( strCand, (WCHAR*)(&m_vecCandidate[0] + candidateList->dwOffset[iIndex]) );
		}
	}
}

DWORD KGCIme::GetCandCount()
{
	//Candidate 전체 개수 리턴
	if( m_vecCandidate.empty() )
		return 0;
	else
	{
		return ((CANDIDATELIST*)&m_vecCandidate[0])->dwSize;
	}
}

DWORD KGCIme::GetCandSel()
{
	//선택되어 있는 Candidate Index 리턴
	if(m_vecCandidate.empty()) 
	{
		return -1;
	} 
	else 
	{
		DWORD dwCandSel = 0;
		dwCandSel = ((CANDIDATELIST*)&m_vecCandidate[0])->dwSelection;
		//printf("dwCandSel: %d\n" , dwCandSel);

		return dwCandSel;
	}
}

DWORD KGCIme::GetCandPageStart()
{
	//Candidate Window의 첫번째 Candidate String의 Index 리턴
	if( m_vecCandidate.empty() )
	{
		return 0;
	}
	else
	{		
		return (DWORD)((GetCurrentPage() - 1) * m_iMaxCadidateSize);
	}
}

DWORD KGCIme::GetCandPageSize()
{
	//Candidate Window에서 하나의 Page당 보여질 수 있는 Candidate String의 개수 리턴
	if( m_vecCandidate.empty() )
		return 0;
	else
		return ((CANDIDATELIST*)&m_vecCandidate[0])->dwPageSize;
}

int KGCIme::GetTotalPage()
{
	int iTotalPage = 0;
	//페이지 세팅
	iTotalPage = GetCandCount()/m_iMaxCadidateSize;
	if( GetCandCount() % m_iMaxCadidateSize != 0 )
		iTotalPage++;

	return iTotalPage;

}

int KGCIme::GetCurrentPage()
{
	int iCurrentPage = 0;
	iCurrentPage = GetCandSel()/m_iMaxCadidateSize + 1;  //현재 몇 페이지인가 알아보고

	int iTotalPage = GetTotalPage();
	if( iTotalPage < iCurrentPage )
		iCurrentPage = iTotalPage;	

	return iCurrentPage;

}

void KGCIme::SetNativeInputMode( bool bNative )
{
	DWORD   dwConversion;
	DWORD   dwSentence;

	HIMC hImc = NULL;
	if( hImc = ImmGetContext( m_hEdit ) )
	{
		ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );
		DWORD dwOldConv = dwConversion;
#if false //defined( _ENGLISH )
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_ALPHANUMERIC;
#else
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= ( bNative ? IME_CMODE_NATIVE : IME_CMODE_ROMAN );
#endif
		if( dwOldConv != dwConversion )	// 별수없다 반복 막자 ㅠ 다를때만 다시 세팅하라고 
			ImmSetConversionStatus( hImc, dwConversion, 0 );

	}
	ImmReleaseContext( m_hEdit, hImc );	
}

bool KGCIme::IsNativeInputMode()
{
	DWORD   dwConversion;
	DWORD   dwSentence;

	HIMC hImc = ImmGetContext( m_hEdit );

	ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );

	ImmReleaseContext( m_hEdit, hImc );

	return ( ( dwConversion & IME_CMODE_NATIVE ) != 0 );
}

void KGCIme::ClearIME( void )
{
	if ( s_hImcDef != NULL )
	{
		//printf("Clear IME~! \n");
		ImmNotifyIME( s_hImcDef, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );

		ZeroMemory(m_strComp, sizeof( m_strComp ));
		ZeroMemory(m_strResult, sizeof( m_strResult ));
		m_vecCandidate.clear();
		SetIMEState( NOTHING );
	}
}


void KGCIme::CancelIME()
{
	if(s_hImcDef != NULL) 
	{
		ImmNotifyIME( s_hImcDef , NI_COMPOSITIONSTR , CPS_CANCEL , 0 );
	}
}

//아직은 쓰지 않는 함수지만 우선 넣어두었다. IME를 끄고, 키는 기능이 있다.
void KGCIme::IMEEnable( HWND hWnd , bool bEnable )
{	

	if ( bEnable )
	{
		if(s_hImcDef)
		{
			ImmAssociateContext(hWnd,s_hImcDef);
			s_bIMEAble = true;			
		}
	}
	else
	{
		s_hImcDef = ImmGetContext( hWnd );
		if(s_hImcDef)
		{
			ImmAssociateContext( hWnd , NULL );
			s_bIMEAble = false;
		}
		ImmReleaseContext(hWnd,s_hImcDef);
	}
}

LRESULT KGCIme::ImeProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(!s_bIMEAble)
	{
		//printf("Ime is not able!\n");
		return FALSE;
	}

	switch( uMsg )
	{	
	case WM_IME_NOTIFY:						//IME창 관련 메시지
		{
			//printf("WM_IME_NOTIFY: %x\n" , DWORD(wParam) );
			switch( DWORD(wParam) )
			{
			case IMN_SETCONVERSIONMODE:	 //전환모드가 변했을 때 날아옴
				//printf("IMN_SETCONVERSIONMODE \n");
				CheckImeMode(m_hEdit);
#if false //defined( _ENGLISH )
				SetNativeInputMode(true);
#endif
				break;
			case IMN_SETOPENSTATUS:		 //open status가 변했을 때 날아옴
				//printf("IMN_SETOPENSTATUS \n");
				CheckImeMode(m_hEdit);
				break;
			case IMN_CHANGECANDIDATE:
			case IMN_OPENCANDIDATE:
				{
					//printf("open Candidate Wnd \n");

					if( s_hImcDef != NULL )
					{ 
						int iLen = ImmGetCandidateList( s_hImcDef, 0, NULL, 0 );
						if(iLen > 0) 
						{
							SetIMEState(CANDS_DO);
							g_pkUIScene->m_pkCandidateWnd->ToggleRender( true );
							g_pkUIScene->m_pkCandidateWnd->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );

							m_vecCandidate.clear();
							m_vecCandidate.resize(iLen);
							ImmGetCandidateList(s_hImcDef, 0, (CANDIDATELIST*)&m_vecCandidate[0], iLen);

							/*잘못된 인덱스가 넘어온다면 그냥 끈다. 
							(문자도 없는 곳의 인덱스가 넘어오는 경우가 있다 ...) 
							또 전체 항목 끝번호보다도 더 인덱스가 큰 경우도 해당한다.*/
							WCHAR strTest[MAX_PATH] = { 0, };
							GetCand( GetCandSel(), strTest );
							if( wcslen(strTest) == 0 )
							{
								//printf("Ime Err Index\n");
								ImmNotifyIME( s_hImcDef , NI_COMPOSITIONSTR, CPS_CANCEL, 0 );

								ZeroMemory(m_strComp, sizeof( m_strComp ));
								ZeroMemory(m_strResult, sizeof( m_strResult ));

								m_vecCandidate.clear();
								SetIMEState( NOTHING );
								g_pkUIScene->m_pkCandidateWnd->ToggleRender( false );
								::SendMessage(m_hEdit , WM_INPUTLANGCHANGE,(WPARAM)0,(LPARAM)GetKeyboardLayout(0));


								return TRUE;
							}


							int iPageStart = GetCandPageStart();
							//UI에 현재페이지의 Candidate 세팅
							for( int i = 0 ; i < m_iMaxCadidateSize ; i++ )
							{
								if( iPageStart + i < (int)GetCandCount() )
								{
									WCHAR strTemp[MAX_PATH] = { 0, };
									WCHAR strCand[MAX_PATH] = { 0, };
									GetCand( iPageStart + i, strTemp );
									swprintf( strCand, MAX_PATH, L"%d. %s", i + 1, strTemp );
									g_pkUIScene->m_pkCandidateWnd->SetCandidate( i, strCand );
								}
								else
								{
									g_pkUIScene->m_pkCandidateWnd->SetCandidate( i, L"" );
								}
							}

							//g_pkUIScene->m_pkCandidateWnd->SetPage( GetCurrentPage(), GetTotalPage() );
						}						
					}

					return TRUE;
				}
			case IMN_CLOSECANDIDATE:
				{
					//printf("IMN_CLOSECANDIDATE\n");
					m_vecCandidate.clear();
					g_pkUIScene->m_pkCandidateWnd->ToggleRender( false );
					return TRUE;
				}
			case IMN_PRIVATE:
				{
					//printf("IMN_PRIVATE\n");
					//여기에서 return 해 주지 않으면 reading Window가 뜰 것이다.
					return TRUE;		
				}

				return FALSE;

			}
		}
	case WM_IME_SETCONTEXT:
		{
			//printf("WM_IME_SETCONTEXT\n");
			// We don't want anything to display, so we have to clear this
			lParam = 0;
			return TRUE;
		}
	case WM_IME_STARTCOMPOSITION:			//조합시작 메시지
		{
			//printf("WM_IME_STARTCOMPOSITION\n");

			SetIMEState(COMPS_START);
			return TRUE;
		}
	case WM_IME_COMPOSITION:				//조합관련 메시지
		{
			//printf("WM_IME_COMPOSITION\n");

			// 컴포지션 윈도우 먼데로 옮기기
			COMPOSITIONFORM cf;
			cf.dwStyle = CFS_FORCE_POSITION;
			cf.ptCurrentPos.x = 2000;
			cf.ptCurrentPos.y = 2000;
			SetRect(&cf.rcArea, 0, 0, 0, 0);
			ImmSetCompositionWindow( s_hImcDef, &cf );

			BOOL	flags;
			BOOL	stringValues;

			flags = ( BOOL ) lParam;
			//카렛은 사용하지 않는다.즉, 커서는 항상 움직이도록 셋팅한다.
			stringValues = flags & ~( CS_INSERTCHAR | CS_NOMOVECARET );


			if( m_eIMEState == COMPS_START )
				SetIMEState(COMPS_DO);

			if ( GetPrimaryLanguage() == LANG_JAPANESE
				|| GetPrimaryLanguage() == LANG_CHINESE 
              //|| GetPrimaryLanguage() == LANG_KOREAN  //---테스트를 위하여 한국은 한문 쓰는거 막기 위해서 표시해주지 않는다.
                )			
			{
				SetIMEState(COMPS_START);

				ZeroMemory(m_strComp, sizeof( m_strComp ));
				ZeroMemory(m_strResult, sizeof( m_strResult ));

				if( s_hImcDef )
				{
					if(stringValues & GCS_COMPSTR)
						ImmGetCompositionString( s_hImcDef, GCS_COMPSTR, m_strComp, sizeof(m_strComp) );

					if(stringValues & GCS_RESULTSTR)
						ImmGetCompositionString( s_hImcDef, GCS_RESULTSTR, m_strResult, sizeof(m_strResult) );

					if ( !(stringValues & GCS_COMPSTR ) && !(stringValues & GCS_RESULTSTR) )
					{
						//printf("There is no composition act in IME\n");
						//ResetInputLocale();
						//return TRUE;
					}					
				}
			}

			if ( GetPrimaryLanguage() == LANG_JAPANESE )
			{
				//printf("LANG_JAPANESE\n");
				//조합 중이라면 Ime처리만 하도록 TRUE를 리턴한다.
				if( stringValues & GCS_COMPSTR )
				{
					if (wcslen(m_strResult) > 0)
					{
						return FALSE;
					}

					return TRUE;
				}
				else if( stringValues & GCS_RESULTSTR ) //조합이 끝났다면 그 다음 과정으로 넘어가도록 FALSE를 리턴한다.
				{
					return FALSE;
				}

				return TRUE;
			}
			if ( GetPrimaryLanguage() == LANG_CHINESE )
			{
				//printf("LANG_CHINESE\n");
				if (wcslen(m_strResult) > 0 )
				{
					return FALSE;
				}

				return TRUE;
			}

			return FALSE;
		}
	case WM_IME_ENDCOMPOSITION:				//조합 완료 메시지
		{
			//printf("WM_IME_ENDCOMPOSITION\n");
			//중국 일부 IME같은 경우 Candidate창도 살아있고 
			//조합문자도 남아있는데 바로 여기로 넘어오는 경우가 있다 ..ㅡ.ㅡ..
			memset( m_strComp, 0, sizeof(WCHAR)*MAX_PATH );
			m_vecCandidate.clear();
			g_pkUIScene->m_pkCandidateWnd->ToggleRender( false );


			SetIMEState( NOTHING );
			return FALSE;
		}
	}

	return FALSE;
}

void KGCIme::OnFocusIn( HWND hWnd )
{
	if( hWnd == NULL )
		return;

	if(s_bIMEAble)
	{
		ImmAssociateContext( hWnd, s_hImcDef );				
	}
	else
	{
		ImmAssociateContext( hWnd, NULL );
	}
}


void KGCIme::OnFocusOut( HWND hWnd )
{
	if( hWnd == NULL )
		return;

	//전역으로 , 포커스가 다른 프로그램으로 옮겨갈 때 불려져야 한다.
	ImmAssociateContext( hWnd , NULL );
}


EIMEMode KGCIme::CheckImeMode( HWND hWnd )
{
	CheckInputLocale();

	bool bIme = ImmIsIME( s_hklCurrent ) != 0;
	bool bChineseIME = ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

	if( NULL != ( s_hImcDef = ImmGetContext( hWnd ) ) )
	{
		if( bChineseIME )
		{
			//printf("bChineseIME\n");
			DWORD dwConvMode, dwSentMode;
			ImmGetConversionStatus( s_hImcDef, &dwConvMode, &dwSentMode );
			s_ImeMode = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_MODE_ON : IMEUI_MODE_ENGLISH;
		}
		else
		{
			//printf("Not Chinese IME \n");

			s_ImeMode = ( bIme && ImmGetOpenStatus( s_hImcDef ) != 0 ) ? IMEUI_MODE_ON : IMEUI_MODE_OFF;
		}
		ImmReleaseContext( hWnd, s_hImcDef );
	}
	else
		s_ImeMode = IMEUI_MODE_OFF;

	//switch(s_ImeMode) 
	//{
	//case IMEUI_MODE_OFF:		//printf("IMEUI_MODE_OFF\n");		break;
	//case IMEUI_MODE_ON:			//printf("IMEUI_MODE_ON\n");		break;
	//case IMEUI_MODE_ENGLISH:	//printf("IMEUI_MODE_ENGLISH\n"); break;
	//}

	return	s_ImeMode;
}

void KGCIme::GetInputLocale()
{
	//담을 input locale identifiers 갯수를 얻는다.
	UINT cKL = GetKeyboardLayoutList( 0, NULL ); 
	s_Locale.clear();
	HKL *phKL = new HKL[cKL];   //그만큼 이제 버퍼를 만들고, 

	//printf("The Number of Input locale is : %u\n" , cKL );

	if( phKL )
	{
		//이제는 실제로 담는다.
		GetKeyboardLayoutList( cKL, phKL );
		for( UINT i = 0; i < cKL; i++ )
		{
			KInputLocale Locale;

			// Filter out East Asian languages that are not IME.
			if( ( PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_CHINESE ||
				PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_JAPANESE ||
				PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_KOREAN ) &&
				!ImmIsIME( phKL[i] ) )
				continue;			

			Locale.m_hKL = phKL[i];
			WCHAR wszDesc[128] = L"";
			switch( PRIMARYLANGID( LOWORD( phKL[i] ) ) )
			{
				// Simplified Chinese
			case LANG_CHINESE:
				switch( SUBLANGID( LOWORD( phKL[i] ) ) )
				{
				case SUBLANG_CHINESE_SIMPLIFIED:
					wcsncpy( Locale.m_wszLangAbb, s_aszIndicator[INDICATOR_CHS] , 3 );
					break;
				case SUBLANG_CHINESE_TRADITIONAL:
					wcsncpy( Locale.m_wszLangAbb, s_aszIndicator[INDICATOR_CHT] , 3 );
					break;
				default:    // unsupported sub-language
					GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
					Locale.m_wszLangAbb[0] = wszDesc[0];
					Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
					Locale.m_wszLangAbb[2] = L'\0';
					break;
				}
				break;
				// Korean
			case LANG_KOREAN:
				wcsncpy( Locale.m_wszLangAbb, s_aszIndicator[INDICATOR_KOREAN] , 3 );
				break;
				// Japanese
			case LANG_JAPANESE:
				wcsncpy( Locale.m_wszLangAbb, s_aszIndicator[INDICATOR_JAPANESE] , 3 );
				break;         
			default:
				// A non-IME language.  Obtain the language abbreviation
				// and store it for rendering the indicator later.
				GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
				Locale.m_wszLangAbb[0] = wszDesc[0];
				Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
				Locale.m_wszLangAbb[2] = L'\0';
				break;
			}

			GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SLANGUAGE, wszDesc, 128 );
			//wcsncpy( Locale.m_wszLang, wszDesc , 64 );	
			_snwprintf(Locale.m_wszLang,  64, L"%s",  wszDesc);

			s_Locale.push_back( Locale );
		}
		SAFE_DELETE_ARRAY( phKL );
	}
}

void KGCIme::CheckInputLocale()
{
	static HKL hklPrev = 0;
	s_hklCurrent = GetKeyboardLayout( 0 );

	//printf("GetKeyboardLayout(0)!!\n");

	if ( hklPrev == s_hklCurrent )
	{
		//printf("But it was same Layout\n");
		s_bHKLChanged = false;
		return;
	}
	else
	{
		s_bHKLChanged = true;
	}

	hklPrev = s_hklCurrent;
	switch ( GetPrimaryLanguage() )
	{
		// Simplified Chinese
	case LANG_CHINESE:
		switch ( GetSubLanguage() )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHS];
			break;
		case SUBLANG_CHINESE_TRADITIONAL:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHT];
			break;
		default:    // unsupported sub-language
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
			break;
		}
		break;
		// Korean
	case LANG_KOREAN:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_KOREAN];
		break;
		// Japanese
	case LANG_JAPANESE:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_JAPANESE];
		break;
	default:
		// A non-IME language.  Obtain the language abbreviation
		// and store it for rendering the indicator later.
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
	}

	// If non-IME, use the language abbreviation.
	if( s_wszCurrIndicator == s_aszIndicator[INDICATOR_NON_IME] )
	{
		WCHAR wszLang[5];
		GetLocaleInfoW( MAKELCID( LOWORD( s_hklCurrent ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszLang, 5 );
		s_wszCurrIndicator[0] = wszLang[0];
		s_wszCurrIndicator[1] = towlower( wszLang[1] );
	}
}


void KGCIme::ResetInputLocale()
{
	//printf("ResetInputLocale \n");

	WCHAR	strKLID[100] = {0,};
	HKL     hklNow = NULL;

	GetKeyboardLayoutName(strKLID);
	hklNow = GetKeyboardLayout(0);

	if(hklNow != s_hklCurrent)
	{
		//printf("Err! hkl is diffrent: \n");
		s_hklCurrent = hklNow;
		UnloadKeyboardLayout(hklNow);
	}
	else
		UnloadKeyboardLayout(s_hklCurrent);


	LoadKeyboardLayout(strKLID , KLF_ACTIVATE | KLF_REPLACELANG | KLF_SUBSTITUTE_OK);

}
