// NMWebCookieCtrlLib.h: interface for the NMWebCookieCtrlLib class.
/**
@file	NMWebCookieCtrlLib.h
@brief	넷마블 웹쿠키 콘트롤 라이브러리 헤더
@author	Heo Jaemin<judge@cj.net>
@date 	2010.01.07
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMWEBCOOKIECTRLLIB_H__542B5D9C_85E8_4F27_8C0F_D4628E4DCB3C__INCLUDED_)
#define AFX_NMWEBCOOKIECTRLLIB_H__542B5D9C_85E8_4F27_8C0F_D4628E4DCB3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 넷마블 웹쿠키 콘트롤 Library 자동추가.(Library 경로는 프로젝트 설정에 추가)
#ifndef _NMWEBCOOKIECTRLMODULE
#if defined(_DLL)
#ifdef _DEBUG
// Debug Multithread Thread DLL 
#define NMWEBCOOKIECTRLMODULE_NAME	"NMWebCookieCtrlLib_MDd"
#else
// Multithread Thread DLL
#define NMWEBCOOKIECTRLMODULE_NAME	"NMWebCookieCtrlLib_MD"
#endif
#else
#ifdef _DEBUG
// Debug Multithread Thread
#define NMWEBCOOKIECTRLMODULE_NAME	"NMWebCookieCtrlLib_MTd"
#else
// Multithread Thread
#define NMWEBCOOKIECTRLMODULE_NAME	"NMWebCookieCtrlLib_MT"
#endif
#endif
// 라이브러리 링크
#pragma comment(lib, NMWEBCOOKIECTRLMODULE_NAME)
#undef NMWEBCOOKIECTRLMODULE_NAME
#endif

/**
@defgroup GROUPERRCODE 오류코드
@{
*/
/// @brief 넷마블 웹쿠키 콘트롤 라이브러리 오류코드
enum ERROR_NMWEBCOOKIECTRL 
{
	ERROR_NMWEBCOOKIECTRL_SUCCESS = 0,				///< 성공.
	ERROR_NMWEBCOOKIECTRL_INVALIDPARAM = 1000,		///< 잘못된 파라미터
	ERROR_NMWEBCOOKIECTRL_REFRESHTIME_ISTOOSMALL,	///< 쿠키 갱신 주기가 너무 짧음.(10초 이하)
	ERROR_NMWEBCOOKIECTRL_ALREADYINIT,				///< 이미 초기화 되었음.
	ERROR_NMWEBCOOKIECTRL_ALREADYREFRESHSTARTED,	///< 웹 쿠키 갱신 백그라운드 쓰레드가 이미 시작되었음.
	ERROR_NMWEBCOOKIECTRL_FAIL_CREATETHREAD,		///< 웹 쿠키 갱신 백그라운드 쓰레드를 시작할 수 없음.
	ERROR_NMWEBCOOKIECTRL_FAIL_SETCOOKIE,			///< 웹 쿠키값 설정 실패
	ERROR_NMWEBCOOKIECTRL_NOT_INITED,				///< 넷마블 웹 쿠키 콘트롤 라이브러리가 초기화되지 않았음.
	ERROR_NMWEBCOOKIECTRL_NOT_EXISTSCOOKIE,			///< 쿠키값이 존재하지 않아 웹 쿠키 갱신 백그라운드 쓰레드를 시작할 수 없음.
};
/**
@}
*/

//////////////////////////////////////////////////////////////////////
/**
@defgroup GROUPEAPIFUNC API 함수
@{
*/
/**
@brif - 넷마블 웹 쿠키 콘트롤을 초기화한다.
@returns 성공 하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 값을 반환하고, 실패하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 을 제외한 @ref ERROR_NMWEBCOOKIECTRL 의 값중 하나를 반환한다.
@param[in] szCookieDomain - 웹쿠키를 설정할 domain을 입력
@param[in] szCookieURL - 웹쿠키값을 활성화 및 갱신할 URL을 입력.
*/
ERROR_NMWEBCOOKIECTRL _cdecl NMWebCookieCtrl_Init(const char* szCookieDomain, const char* szCookieURL);
/**
@brif - 넷마블 웹 쿠키 콘트롤을 파괴한다.
*/
void _cdecl	NMWebCookieCtrl_Destroy();

/**
@brif - 쿠키 갱신 백그라운드 쓰레드를 시작한다.
@returns 성공 하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 값을 반환하고, 실패하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 을 제외한 @ref ERROR_NMWEBCOOKIECTRL 의 값중 하나를 반환한다.
@param[in] dwSecForRefresh - 쿠키 갱신 타임 주기(초단위)를 설정한다. 
*/
ERROR_NMWEBCOOKIECTRL _cdecl NMWebCookieCtrl_StartCookieRefresh(DWORD dwSecForRefresh);

/**
@brif - 웹쿠키값을 설정한다.
@returns 성공 하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 값을 반환하고, 실패하였을 경우 @ref ERROR_NMWEBCOOKIECTRL_SUCCESS 을 제외한 @ref ERROR_NMWEBCOOKIECTRL 의 값중 하나를 반환한다.
@param[in] szName - 설정할 웹쿠키의 이름을 입력.
@param[in] szValue - 설정할 웹쿠키의 값을 입력.
*/
ERROR_NMWEBCOOKIECTRL _cdecl NMWebCookieCtrl_SetDomainCookie(LPCSTR szName, LPCSTR szValue);

/**
@brif - 주어진 에러코드를 디파인 명으로 변환한다.
@returns 디파인 명으로 변환된 에러코드 문자열.
@param[in] errCode - 디파인 명으로 변환할 에러코드를 입력.
*/
const char* _cdecl NMWebCookieCtrl_GetErrorCode2String(ERROR_NMWEBCOOKIECTRL errCode);
/**
@}
*/

#endif // !defined(AFX_NMWEBCOOKIECTRLLIB_H__542B5D9C_85E8_4F27_8C0F_D4628E4DCB3C__INCLUDED_)
