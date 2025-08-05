#ifndef _ENUM
#define _ENUM( e, desc )    e,
#endif

_ENUM( FATAL            , 페이탈이 되었다. 죽을지경 )
_ENUM( DIE_FALL         , 떨어져 죽었다 )
_ENUM( DIE_O2           , 산소 부족으로 사망했다 )
_ENUM( USE_PETSKILL     , 펫 스킬 사용했다 )
_ENUM( USE_EMOTICON     , 이모티콘 사용했다 )
_ENUM( CHAT_FILTER      , 욕하다가 걸렸다 )
_ENUM( CHAT_TOO_MANY    , 너무 지끼다가 걸렸다 )
_ENUM( BACK_ATTACK      , 백어택 성공 )
_ENUM( PLAY_TIME        , 접속 시간 기록 )
_ENUM( SPECIAL_SKILL    , 필살기 사용 )
_ENUM( OPEN_EXIT_UI     , 제발 끄지 말아줘!! )
_ENUM( MSN_ADD_FRIEND   , 우왕 친구를 사귀었어요 )
_ENUM( MSN_BLOCK_FRIEND , 이자식은 친구가 아냐!! )
_ENUM( COUNTER_ATTACK   , 카운터 어택을 맞았어요 )
_ENUM( COMBO_RECORD     , 내가 콤보를 이만큼 했음 )
_ENUM( COMBO_BREAK      , 악! 콤보 깨졌다!! )
_ENUM( HIT_COUNT        , 때렸음! )
_ENUM( KILL_MONSTER     , 몬스터를 죽였다. )
_ENUM( KILL_CHAMPION    , 챔피언 몬스터를 죽였다. )
_ENUM( DAMAGED          , 맞아서 아퐈~ )

#ifdef _ENUM
#undef _ENUM
#endif