// Compiler :
//  Microsoft(R) Program Maintenance Utility 버전 9.00.30729.01
// Author   :
//  limszone@cj.net limjungho
//////////////////////////////////////////////////////////////////////
#pragma once

#ifdef NMNETCAFEDLL_EXPORTS
#define NMNETCAFEDLL_API __declspec(dllexport)
#else
#define NMNETCAFEDLL_API __declspec(dllimport)
#endif


#if !defined(_LIB) && !defined(_NMNETCAFEDLL)
	#ifndef _NMNETCAFEDLLNAME
		#ifdef WIN64
			#define _NMNETCAFEDLLNAME "NMNetCafeDLLx64"
		#else
			#define _NMNETCAFEDLLNAME "NMNetCafeDLLx86"
		#endif // #ifdef WIN64
	#endif //#ifndef _NMNETCAFELIBNAME

	#ifdef _DLL
		// multithread DLL
		#pragma comment(lib, _NMNETCAFEDLLNAME "_MD")		
	#else				
		// multithread
		#pragma comment(lib, _NMNETCAFEDLLNAME "_MT")		
	#endif // #ifdef _DLL
#endif //#if !defined(_LIB) && !defined(_NMNETCAFEDLL)

namespace Marbleworks
{
	namespace NetCafe
	{
		namespace API
		{
			/*
			 Error Code
			 */
			enum eERROR_NMNCAFE
			{
				ERROR_NMNCAFE_SUCCESS				= 0,		// 성공
				ERROR_NMNCAFE_FAIL,								// 실패
				/// connection
				ERROR_NMNCAFE_CONN_SUCC				= 100,		// PC방 Agent 서버와 연결 및 재연결 성공
				ERROR_NMNCAFE_CONN_FAIL,						// PC방 Agent 서버와 연결 실패 및 끊김
				/// deduction
				ERROR_NMNCAFE_SEND_DEDUCT_FAIL		= 200,		// 차감 패킷 전송 실패
				ERROR_NMNCAFE_SEND_DEDUCT_RECOVER,				// 차감 패킷 전송 복구 성공

				ERROR_NMNCAFE_PARM_INVALID			= 300		// 파라미터값 오류
			};

			/*
			 CallBack Function Define
			 */
			// check login response callback function define
			typedef void (_stdcall *PF_NCAFE_CALLBACK__CHK_LOGIN)(LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param);
			// login response callback function define
			typedef void (_stdcall *PF_NCAFE_CALLBACK__LOGIN)(LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param);
			// logout response callback function define
			typedef void (_stdcall *PF_NCAFE_CALLBACK__LOGOUT)(LPCSTR userCN, UINT result, LPVOID param);
			// expire response callback function define
			typedef void (_stdcall *PF_NCAFE_CALLBACK__EXPIRE)(UINT result, LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime);
			// user alive check callback function define
			typedef bool (_stdcall *PF_NCAFE_CALLBACK__ALIVE)(LPCSTR userCN, ULONG ipAddress);
			// dll error callback function define
			typedef void (_stdcall *PF_NCAFE_CALLBACK__ERROR)(UINT errCode);

			/*
			 CallBack Class Interface Define
			 */
			class CNMNetCafe
			{
			public:
				virtual ~CNMNetCafe(){};

				virtual void OnChkLogin( LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param ) = NULL;
				virtual void OnLogin   ( LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param ) = NULL;
				virtual void OnLogout  ( LPCSTR userCN, UINT result, LPVOID param ) = NULL;
				virtual void OnExpire  ( UINT result, LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime ) = NULL;	
				virtual bool OnAlive   ( LPCSTR userCN, ULONG ipAddress ) = NULL;	
				virtual void OnError   ( UINT errCode ) = NULL;
			};

			/*
			 Netmarble NetCafe API
			 */
			// initialize dll with callback function
			NMNETCAFEDLL_API bool InitializeNMNetCafe( LPCSTR						configPath,
													   PF_NCAFE_CALLBACK__CHK_LOGIN	pfCallbackChkLogin,
													   PF_NCAFE_CALLBACK__LOGIN		pfCallbackLogin,
													   PF_NCAFE_CALLBACK__LOGOUT	pfCallbackLogout,
													   PF_NCAFE_CALLBACK__EXPIRE	pfCallbackExpire,
													   PF_NCAFE_CALLBACK__ALIVE		pfCallbackAlive,
													   PF_NCAFE_CALLBACK__ERROR		pfCallbackError) ;

			// initialize dll with callback class
			NMNETCAFEDLL_API bool InitializeNMNetCafe( LPCSTR						configPath,
													   CNMNetCafe*					instance );

			// statrup dll
			NMNETCAFEDLL_API bool StartupNMNetCafe(void);

			// cleanup dll
			NMNETCAFEDLL_API void CleanupNMNetCafe(void);

			// finalize dll
			NMNETCAFEDLL_API void FinalizeNMNetCafe(void);

			// check login request function
			NMNETCAFEDLL_API int lfNMNetCafeChkLogin( LPCSTR userCN, ULONG ipAddress, LPVOID param = NULL );

			// login request function
			NMNETCAFEDLL_API int lfNMNetCafeLogin( LPCSTR userCN, ULONG ipAddress, UINT playTime = 0, LPVOID param = NULL );

			// logout request function
			NMNETCAFEDLL_API int lfNMNetCafeLogout( LPCSTR userCN, ULONG ipAddress = 0, bool isError = false, LPVOID param = NULL );

		} ///< namespace api
	} ///< namespace ncafe
} ///< namespace marbleworks
