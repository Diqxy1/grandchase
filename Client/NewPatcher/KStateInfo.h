#pragma once

enum
{
	WM_ON_START = WM_USER + 1000,
	WM_ON_INVALIDATE,
    WM_ON_PROGRESS,     
	WM_ON_ERROR,
	WM_ON_COMPLETE,	
};


enum KPATCER_MSGID
{
    STR_ID_PATCH_START                            = 0,        //GrandChase 패치를 시작합니다.
    STR_ID_NOT_INSTALL_GRANDCHASE                 = 1,        //GrandChase가 설치 되어 있지 않습니다.
    STR_ID_CAN_NOT_READ_REG                       = 2,        //레지스트리 정보 읽기 오류.
    STR_ID_CAN_NOT_CONNECT_PATCH_SERVER           = 3,        //패치 서버에 접속할 수 없습니다.
    STR_ID_CAN_NOT_DOWNLOAD_PATCH_PATH            = 4,        //패치 경로파일 내려받기 오류.
    STR_ID_CAN_NOT_READ_PATCH_PATH                = 5,        //패치 경로파일 읽기 오류.
    STR_ID_DOWNLOAD_VERSION_FILE                  = 6,        //버젼 파일 다운로드.
    STR_ID_CAN_NOT_DOWNLOAD_VERSION_FILE          = 7,        //버젼 파일 내려받기 오류.
    STR_ID_CAN_NOT_READ_VERSION_FILE              = 8,        //버젼 파일 읽기 오류.
    STR_ID_CHECK_VERSION_DIFF                     = 9,        //다운로드 목록 작성.
    STR_ID_CAN_NOT_CHECK_VERSION_DIFF             = 10,       //다운로드 목록 작성 오류.
    STR_ID_CAN_NOT_UPDATE_VERSION_FILE            = 11,       //버젼 파일 업데이트 오류.
    STR_ID_CAN_NOT_DOWNLOAD_PATCH_FILE            = 12,       //파일 다운로드 오류.
    STR_ID_DOWNLOAD_PATCH_FILE                    = 13,       //파일 다운로드.
    STR_ID_PATCH_END                              = 14,       //GrandChase 패치가 완료되었습니다.
    STR_ID_BLANK                                  = 15,       //" "
    STR_ID_FILE                                   = 16,       //파일
    STR_ID_TOTAL                                  = 17,       //전체
    STR_ID_FILE_DOWNLOAD                          = 18,       //파일 다운로드
    STR_ID_FILE_COMPARE                           = 19,       //파일을 비교 중입니다.
    STR_ID_PROGRAM_PATH_ERR                       = 20,       //프로그램 경로를 읽어올 수 없습니다.
    STR_ID_PATCH_SERVER_INFO_ERR                  = 21,       //패치 서버의 경로가 잘못되었습니다.
	STR_ID_PATCH_COMPLETE_RESTART                 = 22,       //패치를 완료하였습니다. 게임을 다시 실행해주세요.
	STR_ID_DOWNLOADING_PATCH_FILE				  = 23,		  //파일을 받는 중입니다.
	STR_ID_CAN_NOT_FIND_DOWNLOADED_LIST			  = 24,		  //다운로드 리스트 오류.
	STR_ID_MAIN_EXE_VERIFY_FAIL					  = 25,		  //Main.exe 파일 바이러스/해킹 의심 
    STR_ID_GRID_AGGRIMENT                         = 26,       //게임 업데이트를 '초고속 다운로드'로 진행합니다.\n(초고속 다운로드는 다운로드 중일 때만 PC자원을 공유합니다.)
	STR_ID_DOWN_LANGINFO						  = 27,		  //언어 설정 파일 다운로드 오류
	STR_ID_PACKAGE_SIZE							  = 28,
	STR_ID_DOWNLOAD_TIME					      = 29,
	STR_ID_DOWNLOAD_SPEED						  = 30,
	STR_ID_SERVER_SELECLT						  = 31,



    MAX_NUM_STRING,         
};


