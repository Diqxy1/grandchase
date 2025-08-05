;#ifndef __MESSAGES_H__
;#define __MESSAGES_H__
;


LanguageNames =
    (
        Japanease  = 0x0411:Messages_JAPANESE
        Korean  = 0x0412:Messages_KOR
    )


;////////////////////////////////////////
;// Eventlog categories
;//
;// Categories always have to be the first entries in a message file!
;//

MessageId       = 1
SymbolicName    = CATEGORY_ONE
Severity        = Success
Language        = Japanease
First category event
.
Language		= Korean
첫번째 카테고리
.

MessageId       = +1
SymbolicName    = CATEGORY_TWO
Severity        = Success
Language        = Japanease
Second category event
.
Language		= Korean
두번째 카테고리
.

;////////////////////////////////////////
;// Events
;//

MessageId       = +1
SymbolicName    = EVENT_STARTED_BY
Language        = Japanease
The app %1 has been started successfully by user %2
.
Language		= Korean
%1 어플리케이션이 %2 유저에 의해서 시작되었습니다.
.

MessageId       = +1
SymbolicName    = EVENT_BACKUP
Language        = Japanease
You should backup your data regulary!
.
Language		= Korean
자주 백업하시오
.


;////////////////////////////////////////
;// Additional messages
;//

MessageId       = 1000
SymbolicName    = IDS_HELLO
Language        = Japanease
Hello World!
.
Language		= Korean
하잉~
.


MessageId       = +1
SymbolicName    = IDS_GREETING
Language        = Japanease
Hello %1. How do you do?
.
Language		= Korean
하잉 %1. 방가방가~!
.

;
;#endif  //__MESSAGES_H__
;
