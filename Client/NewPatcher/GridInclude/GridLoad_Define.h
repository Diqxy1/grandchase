  #ifndef __GRIDLOAD_DEFINE_H__
#define __GRIDLOAD_DEFINE_H__

#pragma comment (lib, "wsock32.lib")
#pragma comment (lib, "Iphlpapi.lib")
#pragma comment (lib, "winmm.lib")

////////////////////////////////////////////////////////////////////////////////////////////////
// Define - Message
#define WM_GRIDLOAD_EVENT_MSG	(WM_USER+5000)
#define WM_GRIDDOWN_EVENT_MSG	(WM_USER+10000)

////////////////////////////////////////////////////////////////////////////////////////////////
// Enum
typedef enum eGL_CALLBACK_MSG
{
	eGL_MSG_UNKNOWN = 0x0100,
	eGL_MSG_SUCCESS = 0x0101,
	eGL_MSG_FAIL	= 0x0102,
} eGL_CALLBACK_MSG;

////////////////////////////////////////////////////////////////////////////////////////////////
// Enum - Grid Download SDK
typedef enum _eGDX_CALLBACK_MSG
{
	eGDX_MSG_NULL					= 0x0200,
	eGDX_MSG_JOB_INITIALIZE			= 0x0201,
	eGDX_MSG_JOB_PROGRESS			= 0x0202,
	eGDX_MSG_JOB_FILE_COMPLETE		= 0x0203,
	eGDX_MSG_JOB_COMPLETE			= 0x0204,
	eGDX_MSG_JOB_ERROR				= 0x0205,
	eGDX_MSG_ENGINE_UPDATE_INIT		= 0x0206,
	eGDX_MSG_ENGINE_UPDATE_PROGRESS	= 0x0207,
	eGDX_MSG_ENGINE_UPDATE_COMPLETE	= 0x0208,
	eGDX_MSG_ENGINE_UPDATE_FAIL		= 0x0209,
	eGDX_MSG_MAX					= 0xFFFF,
} eGDX_CALLBACK_MSG;

typedef enum _eGDX_ERROR_MSG
{
	eGDX_MSG_NONE					= 0x0200,
	eGDX_MSG_DISKFULL				= 0x0201,
	eGDX_MSG_TAGFILEDOWNLOADFAIL	= 0x0202,	
	eGDX_MSG_NETWORK_UNPLUGED		= 0x0203,
	eGDX_MSG_NETWORK_RECOVERY		= 0x0204,
	eGDX_MSG_DOWNLOAD_FAIL			= 0x0205,
} eGDX_ERROR_MSG;

typedef enum _eGDX_RESULT
{
	eGDX_RESULT_OK						= 0x0200,
	eGDX_RESULT_ERROR_URL				= 0x0201,
	eGDX_RESULT_ERROR_DIR				= 0x0202,
	eGDX_RESULT_ERROR_STARTED			= 0x0203,
	eGDX_RESULT_ERROR_STOPED			= 0x0204,
	eGDX_RESULT_FAIL					= 0x0205,
	eGDX_RESULT_ALREADY_INITIALIZED		= 0x0206,
	eGDX_RESULT_ERROR_UNPLUGED			= 0x0207,
	eGDX_RESULT_ERROR_INVALIDFILESIZE	= 0x0208,
} eGDX_RESULT;

typedef enum _eGDX_ENGINE_INIT_RESULT
{
	eGDX_INITRET_NULL		= 0x0200,
	eGDX_INITRET_SUCCESS	= 0x0201,
	eGDX_INITRET_FAIL		= 0x0202,
} eGDX_ENGINE_INIT_RESULT;

typedef enum _eGDX_DOWNLOAD_TYPE
{
	eGDX_DFT_UNKNOWN	= 0x00,
	eGDX_DFT_GRID		= 0x01,
	eGDX_DFT_SERVER		= 0x02,
} eGDX_DOWNLOAD_TYPE;

////////////////////////////////////////////////////////////////////////////////////////////////
// Struct - Grid Download SDK
typedef struct _ST_GDX_ENGINEINIT_RESULT
{
	eGDX_ENGINE_INIT_RESULT	m_eResult;
} ST_GDX_ENGINEINIT_RESULT, * PST_GDX_ENGINEINIT_RESULT;

typedef struct _ST_GDX_TIME
{
	int	m_nHours;
	int	m_nMinutes;
	int	m_nSeconds;
} ST_GDX_TIME, *PST_GDX_TIME;

typedef struct _ST_GDX_FILE_PROGRESS
{
	unsigned __int64	m_nFileTotalSize;
	unsigned __int64	m_nFileRecvSize;
	int					m_nTotalFileCount;
	int					m_nCompleteFileCount;
	int					m_nSpeedBps;
	unsigned __int64	m_nProgressTime;
	unsigned __int64	m_nRemainTime;
	int					m_nPercent;	
} ST_GDX_FILE_PROGRESS, * PST_GDX_FILE_PROGRESS;

typedef struct _ST_GDX_FILE_COMPLETE
{
	int		m_nTotalFileCount;
	int		m_nCompleteFileCount;
	char	m_szFileInfo[1024];
} ST_GDX_FILE_COMPLETE, * PST_GDX_FILE_COMPLETE;

typedef struct _ST_GDX_FILE_INITIALIZE
{
	unsigned int		m_nFileCount;
	unsigned __int64	m_nFileSize;
} ST_GDX_FILE_INITIALIZE, * PST_GDX_FILE_INITIALIZE;

typedef struct _ST_GDX_DOWNLOAD_COMPLETE
{
	int		m_nTotalFileCount;
	int		m_nSuccessCount;
	int		m_nFailCount;
} ST_GDX_DOWNLOAD_COMPLETE, * PST_GDX_DOWNLOAD_COMPLETE;

typedef struct _ST_GDX_ERROR
{
	eGDX_ERROR_MSG	m_eType;
	char*			m_pDetail;
} ST_GDX_ERROR, * PST_GDX_ERROR;

typedef struct _ST_GDX_SEND_LOG
{
	char	m_szCPCookie[2048];			// CPCookie 문자열
	char	m_szLogDomain[1024];		// 로그 도메인
	char	m_szGameCode[MAX_PATH];		// 영문 게임코드
	int		m_nIsAgree;					// 그리드 동의 여부
	int		m_nIsPageStyle;				// 0 = 다운로드 페이지, 1 = 런쳐, 2 = CP사 Patcher
	int		m_nIRunType;				// 0 = 풀버전+업데이트, 1 = 업데이트 Only
	int		m_nIReturnGridError;		// 그리드 에러코드
	int		m_nIsAgreeStartPage;		// 시작페이지 동의여부
	int		m_nIReturnLauncherError;
	char	m_szErrorFileName[MAX_PATH];
	char	m_szContentType[MAX_PATH];	// "Full" or "Patch"
} ST_GDX_SEND_LOG, * PST_GDX_SEND_LOG;

typedef struct _ST_GDX_ENGINE_UPDATE_INITIALIZE
{
	char				m_szFileURL[1024];
	unsigned __int64	m_ldFileSize;
} ST_GDX_ENGINE_UPDATE_INITIALIZE, * PST_GDX_ENGINE_UPDATE_INITIALIZE;

typedef struct _ST_GDX_ENGINE_UPDATE_PROGRESS
{
	char				m_szFileURL[1024];
	unsigned __int64	m_ldFileSize;
	unsigned __int64	m_ldReceivedSize;
} ST_GDX_ENGINE_UPDATE_PROGRESS, * PST_GDX_ENGINE_UPDATE_PROGRESS;

////////////////////////////////////////////////////////////////////////////////////////////////
// Type Def
typedef void* HGRIDJOB;
typedef void (__stdcall *PFN_GRID_CB)(eGDX_CALLBACK_MSG a_eMsg, HGRIDJOB a_pParam, void* a_pUserData);
typedef void (__stdcall *PFN_GLOAD_CB)(eGL_CALLBACK_MSG a_eMsg, void* a_pUserData);

////////////////////////////////////////////////////////////////////////////////////////////////
// Grid Download Lib Type Def
typedef eGDX_RESULT (*PFN_GridDown_Initialize)(HWND a_hWnd, PFN_GRID_CB a_pfnCallback, void* a_pUserData);
typedef eGDX_RESULT (*PFN_GridDown_Finalize)();
typedef eGDX_RESULT (*PFN_GridDown_SetConfig)(const char* a_szConfigName);
typedef eGDX_RESULT (*PFN_GridDown_SendLog)(PST_GDX_SEND_LOG a_pstSendLog);

typedef eGDX_RESULT (*PFN_GridDown_CreateJob)(HGRIDJOB& a_hJob); // deprecated
typedef eGDX_RESULT (*PFN_GridDown_CreateJobEx)(HGRIDJOB& a_hJob, const char* a_szNationCode); // new
typedef eGDX_RESULT (*PFN_GridDown_DestroyJob)(HGRIDJOB a_hJob);
typedef eGDX_RESULT (*PFN_GridDown_AddFile)(HGRIDJOB a_hJob, const char* a_szURL, const char* a_szDIR, const char* a_szFileName, const char* a_szNationCode); // deprecated
typedef eGDX_RESULT (*PFN_GridDown_AddFileEx)(HGRIDJOB a_hJob, const char* a_szURL, const char* a_szDIR, const char* a_szFileName, __int64 a_ldFileSize, eGDX_DOWNLOAD_TYPE a_eDownloadType); // new
typedef eGDX_RESULT (*PFN_GridDown_Start)(HGRIDJOB a_hJob, BOOL a_bUseUpload);
typedef eGDX_RESULT (*PFN_GridDown_Stop)(HGRIDJOB a_hJob, BOOL a_bUseUpload);
typedef eGDX_RESULT (*PFN_GridDown_DeleteFiles)(HGRIDJOB a_hJob, BOOL a_bDeleteCompleted);

typedef eGDX_RESULT (*PFN_GridDown_GetInitializeData)(HGRIDJOB a_hJob, PST_GDX_FILE_INITIALIZE a_pstInitializeData);
typedef eGDX_RESULT (*PFN_GridDown_GetProgressData)(HGRIDJOB a_hJob, PST_GDX_FILE_PROGRESS a_pstProgressData);
typedef eGDX_RESULT (*PFN_GridDown_GetFileCompleteData)(HGRIDJOB a_hJob, PST_GDX_FILE_COMPLETE a_pstFileCompleteData);
typedef eGDX_RESULT (*PFN_GridDown_GetJobCompleteData)(HGRIDJOB a_hJob, PST_GDX_DOWNLOAD_COMPLETE a_pstJobCompleteData);
typedef eGDX_RESULT (*PFN_GridDown_GetErrorDetailLen)(HGRIDJOB a_hJob, int* a_pnLen);
typedef eGDX_RESULT (*PFN_GridDown_GetErrorData)(HGRIDJOB a_hJob, PST_GDX_ERROR a_pstError);

typedef eGDX_RESULT (*PFN_GridDown_GetEngineUpdateInitializeData)(PST_GDX_ENGINE_UPDATE_INITIALIZE a_pstInitializeData);
typedef eGDX_RESULT (*PFN_GridDown_GetEngineUpdateProgressData)(PST_GDX_ENGINE_UPDATE_PROGRESS a_pstProgressData);

typedef void* (*PFN_GridDown_GetUserData)();

////////////////////////////////////////////////////////////////////////////////////////////////
// Map Define
#define BEGIN_GRIDLOAD_CB_DISPATCHMAP(funcname) \
	void WINAPI funcname(eGL_CALLBACK_MSG a_eMsg, void* a_pUserData) \
	{ \
		switch(a_eMsg) \
		{

#define BEGIN_GRIDLOAD_MSG_DISPATCHMAP(funcname) \
	LRESULT funcname(WPARAM a_wParam, LPARAM a_lParam) \
	{ \
		eGL_CALLBACK_MSG eMsg = (eGL_CALLBACK_MSG)a_wParam; \
		switch(eMsg) \
		{

#define BEGIN_GRIDLOAD_ATL_MSG_DISPATCHMAP(funcname) \
	LRESULT funcname(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam, BOOL& a_bHandled) \
	{ \
		eGL_CALLBACK_MSG eMsg = (eGL_CALLBACK_MSG)a_wParam; \
		switch(eMsg) \
		{

#define DISPATCH_GRIDLOAD_SUCCESS(funcname) \
	case eGL_MSG_SUCCESS: \
	{ \
		char szSDKFullPath[256] = { 0, }; \
		GridLoad_GetGDSPath(szSDKFullPath); \
		funcname(szSDKFullPath, GridLoad_GetUserData()); \
	} \
	break;

#define DISPATCH_CB_GRIDLOAD_SUCCESS(funcname) \
	case eGL_MSG_SUCCESS: \
	{ \
		char szSDKFullPath[256] = { 0, }; \
		GridLoad_GetGDSPath(szSDKFullPath); \
		funcname(szSDKFullPath, a_pUserData); \
	} \
	break;

#define DISPATCH_GRIDLOAD_FAIL(funcname) \
	case eGL_MSG_FAIL: \
	{ \
		funcname(GridLoad_GetUserData()); \
	} \
	break;

#define DISPATCH_CB_GRIDLOAD_FAIL(funcname) \
	case eGL_MSG_FAIL: \
	{ \
		funcname(a_pUserData); \
	} \
	break;

#define END_GRIDLOAD_ATL_MSG_DISPATCHMAP() \
		} \
		return 0; \
	}

#define END_GRIDLOAD_MSG_DISPATCHMAP() \
		} \
		return 0; \
	}

#define END_GRIDLOAD_CB_DISPATCHMAP() \
		} \
	}

////////////////////////////////////////////////////////////////////////////////////////////////
// Map Define - Grid Download SDK
#define BEGIN_GRID_CB_DISPATCHMAP(funcname) \
	void WINAPI funcname(eGDX_CALLBACK_MSG a_eMsg, HGRIDJOB a_hJob, void* a_pUserData) \
{ \
	HGRIDJOB hGridJob = a_hJob; \
	switch(a_eMsg) \
{

#define BEGIN_GRID_CB_DYNAMIC_DISPATCHMAP(funcname, wrapper) \
	void WINAPI funcname(eGDX_CALLBACK_MSG a_eMsg, HGRIDJOB a_hJob, void* a_pUserData) \
{ \
	HGRIDJOB		hGridJob = a_hJob; \
	CGridDownLib*	pLib = wrapper; \
	switch(a_eMsg) \
{

#define BEGIN_GRID_MSG_DISPATCHMAP(funcname) \
	LRESULT funcname(WPARAM a_wParam, LPARAM a_lParam) \
{ \
	eGDX_CALLBACK_MSG	eMsg = (eGDX_CALLBACK_MSG)a_wParam; \
	HGRIDJOB			hGridJob = (HGRIDJOB)a_lParam; \
	switch(eMsg) \
{

#define BEGIN_GRID_MSG_DYNAMIC_DISPATCHMAP(funcname, wrapper) \
	LRESULT funcname(WPARAM a_wParam, LPARAM a_lParam) \
{ \
	eGDX_CALLBACK_MSG	eMsg = (eGDX_CALLBACK_MSG)a_wParam; \
	HGRIDJOB			hGridJob = (HGRIDJOB)a_lParam; \
	CGridDownLib*		pLib = wrapper; \
	switch(eMsg) \
{

#define BEGIN_GRID_ATL_MSG_DISPATCHMAP(funcname) \
	LRESULT funcname(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam, BOOL& a_bHandled) \
{ \
	eGDX_CALLBACK_MSG	eMsg = (eGDX_CALLBACK_MSG)a_wParam; \
	HGRIDJOB			hGridJob = (HGRIDJOB)a_lParam; \
	switch(eMsg) \
{

#define BEGIN_GRID_ATL_MSG_DYNAMIC_DISPATCHMAP(funcname, wrapper) \
	LRESULT funcname(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam, BOOL& a_bHandled) \
{ \
	eGDX_CALLBACK_MSG	eMsg = (eGDX_CALLBACK_MSG)a_wParam; \
	HGRIDJOB			hGridJob = (HGRIDJOB)a_lParam; \
	CGridDownLib*		pLib = wrapper; \
	switch(eMsg) \
{

#define DISPATCH_GRID_JOBINITIALIZE(funcname) \
		case eGDX_MSG_JOB_INITIALIZE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_INITIALIZE stInitialize = { 0, }; \
	GridDown_GetInitializeData(hGridJob, &stInitialize); \
	funcname(hGridJob, &stInitialize, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_JOBINITIALIZE(funcname) \
		case eGDX_MSG_JOB_INITIALIZE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_INITIALIZE stInitialize = { 0, }; \
	GridDown_GetInitializeData(hGridJob, &stInitialize); \
	funcname(hGridJob, &stInitialize, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_JOBINITIALIZE(funcname) \
		case eGDX_MSG_JOB_INITIALIZE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_INITIALIZE stInitialize = { 0, }; \
	pLib->GetInitializeData(hGridJob, &stInitialize); \
	funcname(hGridJob, &stInitialize, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_JOBINITIALIZE(funcname) \
		case eGDX_MSG_JOB_INITIALIZE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_INITIALIZE stInitialize = { 0, }; \
	pLib->GetInitializeData(hGridJob, &stInitialize); \
	funcname(hGridJob, &stInitialize, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_JOBPROGRESS(funcname) \
		case eGDX_MSG_JOB_PROGRESS: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_PROGRESS stProgress = { 0, }; \
	GridDown_GetProgressData(hGridJob, &stProgress); \
	funcname(hGridJob, &stProgress, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_JOBPROGRESS(funcname) \
		case eGDX_MSG_JOB_PROGRESS: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_PROGRESS stProgress = { 0, }; \
	GridDown_GetProgressData(hGridJob, &stProgress); \
	funcname(hGridJob, &stProgress, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_JOBPROGRESS(funcname) \
		case eGDX_MSG_JOB_PROGRESS: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_PROGRESS stProgress = { 0, }; \
	pLib->GetProgressData(hGridJob, &stProgress); \
	funcname(hGridJob, &stProgress, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_JOBPROGRESS(funcname) \
		case eGDX_MSG_JOB_PROGRESS: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_PROGRESS stProgress = { 0, }; \
	pLib->GetProgressData(hGridJob, &stProgress); \
	funcname(hGridJob, &stProgress, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_JOBFILECOMPLETE(funcname) \
		case eGDX_MSG_JOB_FILE_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_COMPLETE stFileComplete = { 0, }; \
	GridDown_GetFileCompleteData(hGridJob, &stFileComplete); \
	funcname(hGridJob, &stFileComplete, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_JOBFILECOMPLETE(funcname) \
		case eGDX_MSG_JOB_FILE_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_COMPLETE stFileComplete = { 0, }; \
	GridDown_GetFileCompleteData(hGridJob, &stFileComplete); \
	funcname(hGridJob, &stFileComplete, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_JOBFILECOMPLETE(funcname) \
		case eGDX_MSG_JOB_FILE_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_COMPLETE stFileComplete = { 0, }; \
	pLib->GetFileCompleteData(hGridJob, &stFileComplete); \
	funcname(hGridJob, &stFileComplete, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_JOBFILECOMPLETE(funcname) \
		case eGDX_MSG_JOB_FILE_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_FILE_COMPLETE stFileComplete = { 0, }; \
	pLib->GetFileCompleteData(hGridJob, &stFileComplete); \
	funcname(hGridJob, &stFileComplete, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_JOBCOMPLETE(funcname) \
		case eGDX_MSG_JOB_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_DOWNLOAD_COMPLETE stJobComplete = { 0, }; \
	GridDown_GetJobCompleteData(hGridJob, &stJobComplete); \
	funcname(hGridJob, &stJobComplete, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_JOBCOMPLETE(funcname) \
		case eGDX_MSG_JOB_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_DOWNLOAD_COMPLETE stJobComplete = { 0, }; \
	GridDown_GetJobCompleteData(hGridJob, &stJobComplete); \
	funcname(hGridJob, &stJobComplete, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_JOBCOMPLETE(funcname) \
		case eGDX_MSG_JOB_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_DOWNLOAD_COMPLETE stJobComplete = { 0, }; \
	pLib->GetJobCompleteData(hGridJob, &stJobComplete); \
	funcname(hGridJob, &stJobComplete, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_JOBCOMPLETE(funcname) \
		case eGDX_MSG_JOB_COMPLETE: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_DOWNLOAD_COMPLETE stJobComplete = { 0, }; \
	pLib->GetJobCompleteData(hGridJob, &stJobComplete); \
	funcname(hGridJob, &stJobComplete, a_pUserData); \
} \
	break;

#define DISPATCH_GRID_JOBERROR(funcname) \
		case eGDX_MSG_JOB_ERROR: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_ERROR stError = { eGDX_MSG_NONE, NULL }; \
	int nDetailLen = 0; \
	GridDown_GetErrorDetailLen(hGridJob, &nDetailLen); \
	if(nDetailLen > 0) { stError.m_pDetail = new char[nDetailLen]; memset(stError.m_pDetail, 0x00, nDetailLen); } \
	GridDown_GetErrorData(hGridJob, &stError); \
	funcname(hGridJob, &stError, GridDown_GetUserData()); \
	if( stError.m_pDetail ) { delete[] stError.m_pDetail; stError.m_pDetail = NULL; } \
} \
	break;

#define DISPATCH_GRID_CB_JOBERROR(funcname) \
		case eGDX_MSG_JOB_ERROR: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_ERROR stError = { eGDX_MSG_NONE, NULL }; \
	int nDetailLen = 0; \
	GridDown_GetErrorDetailLen(hGridJob, &nDetailLen); \
	if(nDetailLen > 0) { stError.m_pDetail = new char[nDetailLen]; memset(stError.m_pDetail, 0x00, nDetailLen); } \
	GridDown_GetErrorData(hGridJob, &stError); \
	funcname(hGridJob, &stError, a_pUserData); \
	if( stError.m_pDetail ) { delete[] stError.m_pDetail; stError.m_pDetail = NULL; } \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_JOBERROR(funcname) \
		case eGDX_MSG_JOB_ERROR: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_ERROR stError = { eGDX_MSG_NONE, NULL }; \
	int nDetailLen = 0; \
	pLib->GetErrorDetailLen(hGridJob, &nDetailLen); \
	if(nDetailLen > 0) { stError.m_pDetail = new char[nDetailLen]; memset(stError.m_pDetail, 0x00, nDetailLen); } \
	pLib->GetErrorData(hGridJob, &stError); \
	funcname(hGridJob, &stError, pLib->GetUserData()); \
	if( stError.m_pDetail ) { delete[] stError.m_pDetail; stError.m_pDetail = NULL; } \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_JOBERROR(funcname) \
		case eGDX_MSG_JOB_ERROR: \
{ \
	if(hGridJob == NULL) { break; } \
	ST_GDX_ERROR stError = { eGDX_MSG_NONE, NULL }; \
	int nDetailLen = 0; \
	pLib->GetErrorDetailLen(hGridJob, &nDetailLen); \
	if(nDetailLen > 0) { stError.m_pDetail = new char[nDetailLen]; memset(stError.m_pDetail, 0x00, nDetailLen); } \
	pLib->GetErrorData(hGridJob, &stError); \
	funcname(hGridJob, &stError, a_pUserData); \
	if( stError.m_pDetail ) { delete[] stError.m_pDetail; stError.m_pDetail = NULL; } \
} \
	break;

#define DISPATCH_GRID_ENGINE_UPDATE_INIT(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_INIT: \
{ \
	ST_GDX_ENGINE_UPDATE_INITIALIZE stInit = { 0, }; \
	GridDown_GetEngineUpdateInitializeData(&stInit); \
	funcname(&stInit, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_ENGINE_UPDATE_INIT(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_INIT: \
{ \
	ST_GDX_ENGINE_UPDATE_INITIALIZE stInit = { 0, }; \
	GridDown_GetEngineUpdateInitializeData(&stInit); \
	funcname(&stInit, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_INIT(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_INIT: \
{ \
	ST_GDX_ENGINE_UPDATE_INITIALIZE stInit = { 0, }; \
	pLib->GetEngineUpdateInitializeData(&stInit); \
	funcname(&stInit, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_ENGINE_UPDATE_INIT(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_INIT: \
{ \
	ST_GDX_ENGINE_UPDATE_INITIALIZE stInit = { 0, }; \
	pLib->GetEngineUpdateInitializeData(&stInit); \
	funcname(&stInit, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_ENGINE_UPDATE_PROGRESS(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_PROGRESS: \
{ \
	ST_GDX_ENGINE_UPDATE_PROGRESS stProgress = { 0, }; \
	GridDown_GetEngineUpdateProgressData(&stProgress); \
	funcname(&stProgress, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_ENGINE_UPDATE_PROGRESS(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_PROGRESS: \
{ \
	ST_GDX_ENGINE_UPDATE_PROGRESS stProgress = { 0, }; \
	GridDown_GetEngineUpdateProgressData(&stProgress); \
	funcname(&stProgress, GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_PROGRESS(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_PROGRESS: \
{ \
	ST_GDX_ENGINE_UPDATE_PROGRESS stProgress = { 0, }; \
	pLib->GetEngineUpdateProgressData(&stProgress); \
	funcname(&stProgress, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_ENGINE_UPDATE_PROGRESS(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_PROGRESS: \
{ \
	ST_GDX_ENGINE_UPDATE_PROGRESS stProgress = { 0, }; \
	pLib->GetEngineUpdateProgressData(&stProgress); \
	funcname(&stProgress, pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_ENGINE_UPDATE_COMPLETE(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_COMPLETE: \
{ \
	funcname(GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_ENGINE_UPDATE_COMPLETE(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_COMPLETE: \
{ \
	funcname(GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_COMPLETE(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_COMPLETE: \
{ \
	funcname(pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_ENGINE_UPDATE_COMPLETE(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_COMPLETE: \
{ \
	funcname(pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_ENGINE_UPDATE_FAIL(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_FAIL: \
{ \
	funcname(GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_ENGINE_UPDATE_FAIL(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_FAIL: \
{ \
	funcname(GridDown_GetUserData()); \
} \
	break;

#define DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_FAIL(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_FAIL: \
{ \
	funcname(pLib->GetUserData()); \
} \
	break;

#define DISPATCH_GRID_CB_DYNAMIC_ENGINE_UPDATE_FAIL(funcname) \
		case eGDX_MSG_ENGINE_UPDATE_FAIL: \
{ \
	funcname(pLib->GetUserData()); \
} \
	break;

#define END_GRID_ATL_MSG_DISPATCHMAP() \
} \
	return 0; \
}

#define END_GRID_ATL_MSG_DYNAMIC_DISPATCHMAP() \
} \
	return 0; \
}

#define END_GRID_MSG_DISPATCHMAP() \
} \
	return 0; \
}

#define END_GRID_MSG_DYNAMIC_DISPATCHMAP() \
} \
	return 0; \
}

#define END_GRID_CB_DISPATCHMAP() \
} \
}

#define END_GRID_CB_DYNAMIC_DISPATCHMAP() \
} \
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GridLoadLib
#ifndef __GRIDLOAD_H_H__
#define __GRIDLOAD_H_H__

#include <afxwin.h>

#ifdef GL_DLL_EXPORT
	#define GL_DLL  __declspec(dllexport)   // export DLL information
#else
	#define GL_DLL  __declspec(dllimport)   // import DLL information
#endif // #ifdef GL_DLL_EXPORT

BOOL GridLoad_InitializeMsg(HWND a_hWnd, void* a_pUserData);
BOOL GridLoad_InitializeCb(PFN_GLOAD_CB a_pfnCallback, void* a_pUserData);
VOID GridLoad_Finalize();
BOOL GridLoad_LoadLib();
BOOL GridLoad_GetGDSPath(char* a_szFilePath);
void* GridLoad_GetUserData();

#endif // #ifndef __GRIDLOAD_H_H__

////////////////////////////////////////////////////////////////////////////////////////////////
// GridDownLib
class CGridDownLib
{
public:
	eGDX_RESULT SetConfig(const char* a_szConfigName);
	eGDX_RESULT	SendLog(PST_GDX_SEND_LOG a_pstSendLog);

public:
	eGDX_RESULT CreateJobEx(HGRIDJOB& a_hJob, const char* a_szNationCode);
	eGDX_RESULT DestroyJob(HGRIDJOB a_hJob);
	eGDX_RESULT AddFileEx(HGRIDJOB a_hJob, const char* a_szURL, const char* a_szDIR, const char* a_szFileName, __int64 a_ldFileSize, eGDX_DOWNLOAD_TYPE a_eDownloadType); // new
	eGDX_RESULT Start(HGRIDJOB a_hJob, BOOL a_bUseUpload);
	eGDX_RESULT Stop(HGRIDJOB a_hJob, BOOL a_bUseUpload);
	eGDX_RESULT DeleteFiles(HGRIDJOB a_hJob, BOOL a_bDeleteCompleted);

public:
	eGDX_RESULT GetInitializeData(HGRIDJOB a_hJob, PST_GDX_FILE_INITIALIZE a_pstInitializeData);
	eGDX_RESULT GetProgressData(HGRIDJOB a_hJob, PST_GDX_FILE_PROGRESS a_pstProgressData);
	eGDX_RESULT GetFileCompleteData(HGRIDJOB a_hJob, PST_GDX_FILE_COMPLETE a_pstFileCompleteData);
	eGDX_RESULT GetJobCompleteData(HGRIDJOB a_hJob, PST_GDX_DOWNLOAD_COMPLETE a_pstJobCompleteData);
	eGDX_RESULT GetErrorDetailLen(HGRIDJOB a_hJob, int* a_pnLen);
	eGDX_RESULT GetErrorData(HGRIDJOB a_hJob, PST_GDX_ERROR a_pstError);
	eGDX_RESULT GetEngineUpdateInitializeData(OUT PST_GDX_ENGINE_UPDATE_INITIALIZE a_pstInitializeData);
	eGDX_RESULT GetEngineUpdateProgressData(OUT PST_GDX_ENGINE_UPDATE_PROGRESS a_pstProgressData);
	void* GetUserData();
	
public:
	CGridDownLib(const char* a_szDllPath, HWND a_hWnd, void* a_pUserData);
	CGridDownLib(const char* a_szDllPath, PFN_GRID_CB a_pfnCallback, void* a_pUserData);
	virtual ~CGridDownLib();

private:
	eGDX_RESULT Initialize(HWND a_hWnd, PFN_GRID_CB a_pfnCallback, void* a_pUserData);
	eGDX_RESULT Finalize();
	eGDX_RESULT CreateJob(HGRIDJOB& a_hJob);
	eGDX_RESULT AddFile(HGRIDJOB a_hJob, const char* a_szURL, const char* a_szDIR, const char* a_szFileName, const char* a_szNationCode); // deprecated


private:
	void InternalInitialize(const char* a_szDllPath, HWND a_hWnd, PFN_GRID_CB a_pfnCallback, void* a_pUserData);
	void InternalLoadGridDll(const char* a_szDllPath);

private:
	PFN_GridDown_Initialize				m_pfnGridDownInitialize;
	PFN_GridDown_Finalize				m_pfnGridDownFinalize;
	PFN_GridDown_SetConfig				m_pfnGridDownSetConfig;
	PFN_GridDown_SendLog				m_pfnGridDownSendLog;

	PFN_GridDown_CreateJob				m_pfnGridDownCreateJob;
	PFN_GridDown_CreateJobEx			m_pfnGridDownCreateJobEx;
	PFN_GridDown_DestroyJob				m_pfnGridDownDestroyJob;
	PFN_GridDown_AddFile				m_pfnGridDownAddFile;
	PFN_GridDown_AddFileEx				m_pfnGridDownAddFileEx;
	PFN_GridDown_Start					m_pfnGridDownStart;
	PFN_GridDown_Stop					m_pfnGridDownStop;
	PFN_GridDown_DeleteFiles			m_pfnGridDownDeleteFiles;

	PFN_GridDown_GetInitializeData		m_pfnGridDownGetInitializeData;
	PFN_GridDown_GetProgressData		m_pfnGridDownGetProgressData;
	PFN_GridDown_GetFileCompleteData	m_pfnGridDownGetFileCompleteData;
	PFN_GridDown_GetJobCompleteData		m_pfnGridDownGetJobCompleteData;
	PFN_GridDown_GetErrorDetailLen		m_pfnGridDownGetErrorDetailLen;	
	PFN_GridDown_GetErrorData			m_pfnGridDownGetErrorData;

	PFN_GridDown_GetEngineUpdateInitializeData	m_pfnGridDownGetEngineUpdateInitializeData;
	PFN_GridDown_GetEngineUpdateProgressData	m_pfnGridDownGetEngineUpdateProgressData;

	PFN_GridDown_GetUserData			m_pfnGridDownGetUserData;

private:
	HMODULE m_hGridSDK;
};

#endif // #ifndef __GRIDLOAD_DEFINE_H__