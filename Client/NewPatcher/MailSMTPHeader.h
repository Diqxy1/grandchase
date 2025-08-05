#ifndef _MAILSMTPHEADER_H
#define _MAILSMTPHEADER_H

//메일을 보내는데 갖춰져야 하는 자료
//data~라고 시작되는 값을 받기위해서 MailMIME.dll을 이용한다.
typedef struct{
	char *from;			//보내는 사람
	char *to;			//받는사람(,으로 구분)
	char *cc;			//참조
	char *bcc;			//숨은참조
	char *dataHead1;	//변할수 있는 헤더- Date, Message-ID, To, Cc, (Bcc)
	char *dataHead2;	//변치않는 헤더
	char *dataBody;		//본문
	char *dataAttach;	//attach file
    char *password;     //Password. ID는 보내는 사람의 메일 주소를 이용한다
	const char *dataEnd;		//끝부분 정의
} _Mail;

//메일의 주소를 구분할 때
#define DEFAULT_SEPARATOR ','

//함수형식의 선언
typedef void (*ENDPROC)(BOOL bSuccess);
//bSuccess :TRUE이면 작업성공, FALSE이면 작업실패 - 상세요인은 SMTP_GetLastMsg
typedef void (*ALERTPROC)(int iStep, int iProgressStep);
//iStep	:	0-전체단계를 iProgressStep에 넣어준다.
//			1-메일프로그램 초기화중
//			2-서버연결중
//			3-서버와 통신중
//			4-메일보내는 중
//			5-작업완료
//iProgressStep	: 전체단계에서 어느정도 진행이 되었는가.(프로그래스바에 SetPos값이라고나 할까.)

//메일을 보낸다.
//return 0이면 실행실패, 그외이면 실행성공. 성공한 경우는 fnEnd가 호출되어야 작업끝
extern "C" _declspec(dllexport) long SMTP_SendMail(HWND hwndOwner, unsigned long ulSMTPAddr
								, _Mail *pMail, ENDPROC fnEnd, ALERTPROC fnAlert=NULL);
//	hwndOwner	: 이 dll이 포함되어 있는 윈도우.
//	ulSMTPAddr	: SMTP서버의 주소
//	pMail		: 보내는 메일에 대한 정보(자세한 사항은 _Mail구조체 참고)
//	fnEnd		: 작업이 끝났을 때 호출할 함수. PbCont에 따라 작업이 성공했는지 알수 있다.
//	fnAlert		: 진행정도에 따라 호출되는 함수. NULL이면 진행정도 알리지 않음.


//메일보내기를 중단한다. 그래도 결과는 fnEnd로 온다.
extern "C" _declspec(dllexport) void SMTP_Cancel();

#ifndef INADDR_NONE
#define INADDR_NONE             0xffffffff
#endif

//문자열 형식의 주소를 ("210.111.23.194" or "mail.perceptcom.com") ULONG의 값으로 바꿈.
//잘못된 형식이면 INADDR_NONE을 리턴.
extern "C" _declspec(dllexport) unsigned long SMTP_TransAddr(char *strAddr);

//pbCont가 FALSE로 바뀌었을 경우 자세한 정보를 얻을수 있음.
extern "C" _declspec(dllexport) char *SMTP_GetLastMsg();

#endif
