#ifndef _MAILMIMEHEADER_H
#define _MAILMIMEHEADER_H

//Head1의 내용을 만들어서 받아온다.
extern "C" _declspec(dllexport) char *MIME_MakeHead1(char *to, char *cc=NULL);
//to			받는사람
//cc			참조(bcc는 실제 본문에는 들어가지 않는다.)

//Head2의 내용을 만들어서 받아온다.
extern "C" _declspec(dllexport) char *MIME_MakeHead2(char *fromName, char *fromEMail
													 , char *subject, int priority);
//fromName		보낸사람 이름
//fromEMail		보낸사람 전자메일
//subject		제목
//priority		편지우선순위	(0- 높음(1), 1-일반(3), 2-낮음(5))

//본문을 만든다
extern "C" _declspec(dllexport) char *MIME_MakeBody(char *Text, int TextType);
//Text			본문
//TextType		본문형식		(0-text, 1-html, 2-html/text)

//첨부파일을 변환한다.
extern "C" _declspec(dllexport) char *MIME_MakeAttach(char **AttachFile, int cAttachFile);
//AttachFile	첨부파일의 이름들(경로까지 포함)
//cAttachFile	첨부파일 개수

//Data의 마지막부분을 받아온다.
extern "C" _declspec(dllexport) const char *MIME_GetEnd();

//MailMIME dll에서 할당한 메모리는 free한다.(Head1, Head2, Body, Attach)
extern "C" _declspec(dllexport) void MIME_Free(char *&mem);

#endif