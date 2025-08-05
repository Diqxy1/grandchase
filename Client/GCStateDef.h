#ifndef _ENUM
#define _ENUM( ID )     ID,
#define __ENUM__
#pragma once

//�̰��� ���ο� ������Ʈ�� �߰��ϰ��� �� ������ StateResource.lua ���Ͽ��� ���� �־��־�� ��.
enum GAME_STATE
{
#endif //_ENUM
    _ENUM( GS_INIT_LOADING          )
    _ENUM( GS_LOGO                  )
    _ENUM( GS_LOGIN                 )
    _ENUM( GS_GUILD                 )
    _ENUM( GS_SHOP_CASH             )   // ĳ�� ����
    _ENUM( GS_ROOM                  )	// ����,���� ��
    _ENUM( GS_GAME                  )	// ����, ���� ������
    _ENUM( GS_MY_INFO               )	// ���â
    _ENUM( GS_OPTION                )
    _ENUM( GS_MY_INFO_FROM_ROOM     )	// ���濡�� ������ ���â	
    _ENUM( GS_GAME_LOADING          )	// ���� �ε���
    _ENUM( GS_GAME_END              )	// ���� ������
    _ENUM( GS_TUTORIAL              )   // ������� �ε�
    _ENUM( GS_REPLAY                )
    _ENUM( GS_SERVER                )
    _ENUM( GS_WORLDMAP              )	// ������� �� ����â
    _ENUM( GS_MATCH                 )	// ������� �渮��Ʈ ���µ�...
    _ENUM( GS_SQUARE_LOADING        )
    _ENUM( GS_SQUARE                )
    _ENUM( GS_SQUARE_END            )
    _ENUM( FAIL_N_SHUTDOWN          ) //������Ʈ ���� ���н� Ŭ���̾�Ʈ ���ᰡ �ʿ��� ��...
    _ENUM( GS_EXITE_GAME            ) //exit(0)/terminate()/WM_CLOSE ������ ������ ���� �� �� 

#if defined USE_COORDI_SHOP
    _ENUM( GS_COORDI_SHOP           ) //�ڵ� 
#endif

	_ENUM( GS_CHAR_SELECT          ) //�ɸ��ͼ��� ��
	_ENUM( GS_CHAR_GAIN				) //�ɸ���ȹ�� ��
	_ENUM( GS_INTRO                 )	//�ε�� ����

#ifndef DISABLE_MAP_SHOP // Kawan>
    _ENUM( GS_SHOP_GP               )	// GP ����(����� ����)
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
