#ifndef _ENUM
#define _ENUM( ID )     ID,
#define __ENUM__
#pragma once

//이곳에 새로운 스테이트를 추가하고자 할 때에는 StateResource.lua 파일에도 같이 넣어주어야 함.
enum GAME_STATE
{
#endif //_ENUM
    _ENUM( GS_INIT_LOADING          )
    _ENUM( GS_LOGO                  )
    _ENUM( GS_LOGIN                 )
    _ENUM( GS_GUILD                 )
    _ENUM( GS_SHOP_CASH             )   // 캐쉬 상점
    _ENUM( GS_ROOM                  )	// 던전,대전 방
    _ENUM( GS_GAME                  )	// 던전, 대전 게임중
    _ENUM( GS_MY_INFO               )	// 장비창
    _ENUM( GS_OPTION                )
    _ENUM( GS_MY_INFO_FROM_ROOM     )	// 대기방에서 진입한 장비창	
    _ENUM( GS_GAME_LOADING          )	// 게임 로딩중
    _ENUM( GS_GAME_END              )	// 게임 종료중
    _ENUM( GS_TUTORIAL              )   // 연습모드 로딩
    _ENUM( GS_REPLAY                )
    _ENUM( GS_SERVER                )
    _ENUM( GS_WORLDMAP              )	// 던전모드 맵 선택창
    _ENUM( GS_MATCH                 )	// 대전모드 방리스트 보는데...
    _ENUM( GS_SQUARE_LOADING        )
    _ENUM( GS_SQUARE                )
    _ENUM( GS_SQUARE_END            )
    _ENUM( FAIL_N_SHUTDOWN          ) //스테이트 변경 실패시 클라이언트 종료가 필요할 때...
    _ENUM( GS_EXITE_GAME            ) //exit(0)/terminate()/WM_CLOSE 등으로 게임이 종료 될 때 

#if defined USE_COORDI_SHOP
    _ENUM( GS_COORDI_SHOP           ) //코디샵 
#endif

	_ENUM( GS_CHAR_SELECT          ) //케릭터선택 씬
	_ENUM( GS_CHAR_GAIN				) //케릭터획득 씬
	_ENUM( GS_INTRO                 )	//인드로 영상

#ifndef DISABLE_MAP_SHOP // Kawan>
    _ENUM( GS_SHOP_GP               )	// GP 상점(대륙별 상점)
#endif

	_ENUM( GS_CHAR_SELECT_FROM_ROOM )
    _ENUM( GS_REGISTER              )
    _ENUM( NUM_GAME_STATE           )   

#ifdef __ENUM__
};
#endif // __ENUM__

#ifdef _ENUM
#undef _ENUM
#undef __ENUM__
#endif //_ENUM
