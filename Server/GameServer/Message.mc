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
ù��° ī�װ�
.

MessageId       = +1
SymbolicName    = CATEGORY_TWO
Severity        = Success
Language        = Japanease
Second category event
.
Language		= Korean
�ι�° ī�װ�
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
%1 ���ø����̼��� %2 ������ ���ؼ� ���۵Ǿ����ϴ�.
.

MessageId       = +1
SymbolicName    = EVENT_BACKUP
Language        = Japanease
You should backup your data regulary!
.
Language		= Korean
���� ����Ͻÿ�
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
����~
.


MessageId       = +1
SymbolicName    = IDS_GREETING
Language        = Japanease
Hello %1. How do you do?
.
Language		= Korean
���� %1. �氡�氡~!
.

;
;#endif  //__MESSAGES_H__
;
