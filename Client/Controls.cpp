#include "stdafx.h"
//
#include "MyD3D.h"


#include "KDInput.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Headup Display.h"

//
//
//
#include "GCStateMachine.h"
#include "GCCameraOrtho.h"
#include "Stage.h"
#include "GCUI/KGCSquareOverlayUI.h"
#include "buff/KGCBuffManager.h"
#include "KGCRoomManager.h"
#include "Message.h"
#include "./KncP2PLib/KncP2P.h"
#include "KGCAgitGameManager.h"
#include "KGCMultipleLanguages.h"

#include "GCUI/GCGameOverlayUI.h"
#include "Monster.h"
#include "KGCChannelManager.h"
#include "GCAutoMatchManager.h"

POINT ptCursor  = { 0, 0 };
POINT ptOldCursor = { 0, 0 };
bool MPushR     = false;
bool MOld_PushR = false;
bool MPushL     = false;
bool MOld_PushL = false;


#define PRINTCONTROL(p) if( g_kGlobalValue.m_bShowControls )\
    START_LOG(cout, "\n" << (LPCTSTR)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) << L", " << #p);


Controls::Controls( void )
{
    for( int i = 0 ; i < (int)m_aptexMouse.size() ; ++i )
    {
        m_aptexMouse[i] = NULL;
    }

    // NOTE : 나중에 키 커스터마이징이 가능하도록 수정함. by parkch 2005-09-14
    m_keyUp				= DIK_UP;
    m_keyDown			= DIK_DOWN;
    m_keyLeft			= DIK_LEFT;
    m_keyRight			= DIK_RIGHT;
    m_keyAttack			= DIK_Z;
    m_keyUseItem		= DIK_C;
    m_keyChangeItem		= DIK_X;
    m_keyProvocation	= DIK_V;

    //	m_keyRebirth		= DIK_R;	//	곧 없어짐
    m_keyExit			= DIK_ESCAPE;
    m_keyChangeSkill	= DIK_SPACE;
    m_keyUsePetSkill	= DIK_LSHIFT;

    m_keySpecialSkillKey= DIK_A;
    m_keyWeaponSwap = DIK_LCONTROL;
    m_keyGameInventory = DIK_I;
    m_keyCameraZoom = DIK_B;
#if !defined(__PATH__)
    m_keyMonsterDirection = DIK_P;
#endif


    // 단축슬롯 작업하면서 추가 함. crey21.
    // 단축 슬롯 ITEM KEY
#if defined(NATION_THAILAND)
    m_keyShowShortCut	= DIK_MINUS;
#else
    m_keyShowShortCut	= DIK_GRAVE;
#endif

    k_ShortCut_Item		= KEY_UP;		// 키가 안 눌린 경우에만 KEY_UP. 나머지는 슬롯 넘버
    k_FreshSkillNum			= 0;
    k_MonsterAction     = 0;

    m_vecKeyShortCutItem.clear();
    m_vecKeyShortCutItem.push_back( DIK_Q );
    m_vecKeyShortCutItem.push_back( DIK_W );
    m_vecKeyShortCutItem.push_back( DIK_E );
    m_vecKeyShortCutItem.push_back( DIK_R );
    m_vecKeyShortCutItem.push_back( DIK_T );

    // 단축 슬롯 EMOTICON KEY
    k_ShortCut_Emot		= KEY_UP;		// 키가 안 눌린 경우에만 KEY_UP. 나머지는 슬롯 넘버
    k_Game_Item         = KEY_UP;		// 키가 안 눌린 경우에만 KEY_UP. 나머지는 슬롯 넘버
    k_ShortCut_MacroCommunity= KEY_UP;

    m_vecKeyShortCutEmot.clear();
    m_vecKeyShortCutEmot.push_back( DIK_1 );
    m_vecKeyShortCutEmot.push_back( DIK_2 );
    m_vecKeyShortCutEmot.push_back( DIK_3 );
    m_vecKeyShortCutEmot.push_back( DIK_4 );
    m_vecKeyShortCutEmot.push_back( DIK_5 );
    m_vecKeyShortCutEmot.push_back( DIK_6 );
    m_vecKeyShortCutEmot.push_back( DIK_7 );
    m_vecKeyShortCutEmot.push_back( DIK_8 );
    m_vecKeyShortCutEmot.push_back( DIK_9 );
    m_vecKeyShortCutEmot.push_back( DIK_0 );

    m_vecKeySkill.clear();               m_vecKeyCnt.clear();
    m_vecKeySkill.push_back( DIK_A );    m_vecKeyCnt.push_back( 0 );
    m_vecKeySkill.push_back( DIK_S );    m_vecKeyCnt.push_back( 0 );
    m_vecKeySkill.push_back( DIK_D );    m_vecKeyCnt.push_back( 0 );
    m_vecKeySkill.push_back( DIK_F );    m_vecKeyCnt.push_back( 0 );
    m_vecKeySkill.push_back( DIK_G );    m_vecKeyCnt.push_back( 0 );

#if defined( USE_AUTO_MP_SPECIAL )
    m_vecAutoMPSpecial.clear();
    m_vecAutoMPSpecial.push_back( false );
    m_vecAutoMPSpecial.push_back( false );
    m_vecAutoMPSpecial.push_back( false );
#endif

    m_vecKeyMonsterAction.clear();
    m_vecKeyMonsterAction.push_back( DIK_LBRACKET );
    m_vecKeyMonsterAction.push_back( DIK_RBRACKET );
    m_vecKeyMonsterAction.push_back( DIK_BACKSLASH );

#if defined(USE_JOYSTICK)
    InitVirtualGamePadKey();
    SetVirtualGamePadKey();

#endif

    k_Fresh_Up          = false;
    k_Up                = false;
    k_Down              = false;
    k_Fresh_Down        = false;
    k_Left              = false;
    k_Right             = false;
    k_Fresh_Punch       = false;
    k_Fresh_Punch_Curse = false;
    k_Fresh_Punch_Charging= false;
    k_Fresh_Fire        = false;
    k_Push_Burnning		= false; //진 - 아이템 선택 버튼이 눌려진 상태인지
    k_Use_Item          = false;
    k_keyAIConnection   = false;
    k_Select_Item       = false;
    k_Change_SkillType	= false;
    k_Use_PetSkill		= false;
    k_Fresh_Right_Dash  = false;
    k_Fresh_Left_Dash   = false;
    k_Fresh_Exit        = false;
    k_Fresh_Joke        = false;
    k_Fresh_Right       = false;
    k_Fresh_Left        = false;
    k_Fresh_Keep_Left   = false;
    k_Fresh_Keep_Right  = false;
    k_Fresh_Down_Dash	= false;
    k_Fresh_Up_Dash		= false;
    k_Fresh_Double_X	= false;
    k_Fresh_Double_Z	= false;
    k_Fresh_Double_C	= false;
    k_Fresh_Left_Right_Sync  = false;
    k_ShowShortCut		= false;
    k_Fresh_Left_Right  = false;
    k_Fresh_Right_Left  = false;
    k_Fresh_Skill_Key	= false;
    k_Skill_Key			= false;
    k_SkillNum    = 0;
    k_Push_X_Charging    = false;
    k_Push_Up_Charging   = false;
    k_DotaShop          = false;
    k_keyEmbarkation    = false;

    m_x                 = 0.0f;
    m_y                 = 0.0f;
    Drag_m_x            = 0.0f;
    Drag_m_y            = 0.0f;
    Energy              = 0.0f;

    Left_Dash_Count     = 0;
    Right_Dash_Count    = 0;
    Up_Dash_Count		= 0;
    Down_Dash_Count		= 0;
    Double_Z_Count		= 0;
    Double_C_Count		= 0;
    Double_X_Count		= 0;
    X_Count		= 0;
    Mouse_Power         = 0;
    //잔
    Burnning_X_Count	= 0;
    Skill_Key_Count		= 0;

    m_bSleep            = false;
    m_dwSleepFrame      = 0;

    k_Game_Inventory    = false;
    k_keyCameraZoom     = false;

    for ( int j = 0; j < GUARD_TOTAL; ++j )
        for ( int i = 0; i < BLOCK_ENABLE_FRAME; ++i )
            m_bUserInputGuard[j][i] = false;

	m_iCheckForConfusion2 = 0;
}
#if defined(USE_JOYSTICK)
void Controls::ReadJoystick( void )
{
    ReadImmediateJoystickData();
}
#endif
// Direct Input에서 실제 입력을 받아 구조체에 입력한다.
void Controls::Into_Packet( void )
{
    static DWORD dwHotCountTime = ::timeGetTime();

    ReadImmediateData();
#if defined(USE_JOYSTICK)
    SetGamePadKey();
#endif

    if ( g_MyD3D->m_pStateMachine->IsStateRoom() )
    {
        CheckSleepFramemove();
    }

    if(ptOldCursor.x!=ptCursor.x || ptOldCursor.y!=ptCursor.y)
    {
        Mouse_Power=255;
        ptOldCursor.x=ptCursor.x;
        ptOldCursor.y=ptCursor.y;
    }

    if( g_MyD3D->Reserve_InputFocus )
    {
        for( int t = 0; t < 256; ++t )
        {
            if( diks[t] )
            {
                g_MyD3D->Reserve_InputFocus = false;
                break;
            }
        }
    }

    // 마우스 위치 정보
    m_x =  (float)( ptCursor.x - W_X ) / (float)( W_Width  ) * 2.0f - 1.0f;
    m_y = -(float)( ptCursor.y - W_Y ) / (float)( W_Height ) * 1.5f + 0.75f;

    // 밖으로 벗어나지 못하게 한다.
    if( m_x < -1.0f  ) m_x = -1.0f;
    if( m_x >  1.0f  ) m_x =  1.0f;
    if( m_y < -0.75f ) m_y = -0.75f;
    if( m_y >  0.75f ) m_y =  0.75f;

    if( MPushL )
    {
    }
    else if( !MOld_PushL )
    {
        Drag_m_x    = m_x;
        Drag_m_y    = m_y;
    }

    // 초기화
    k_Fresh_Down        = false;
    k_Fresh_Up          = false;
    k_Fresh_Punch       = false;
    k_Fresh_Punch_Curse = false;
    k_Fresh_Punch_Charging= false;
    k_Fresh_Fire        = false;
    k_Fresh_Joke        = false;
    k_Fresh_Left_Dash   = false;
    k_Fresh_Right_Dash  = false;
    k_Fresh_Up_Dash		= false;
    k_Fresh_Down_Dash	= false;
    k_Fresh_Double_X	= false;
    k_Fresh_Double_Z	= false;
    k_Fresh_Double_C	= false;
    k_Select_Item       = false;
    k_Change_SkillType  = false;
    k_Use_Item          = false;
    k_keyAIConnection   = false;
    k_Use_PetSkill		= false;
    k_Fresh_Exit        = false;
    k_Fresh_Left        = false;
    k_Fresh_Right       = false;
    k_Fresh_Keep_Right  = false;
    k_Fresh_Keep_Left   = false;
    k_Fresh_Left_Right_Sync  = false;
    k_ShowShortCut		= false;
    k_Fresh_Left_Right  = false;
    k_Fresh_Right_Left  = false;
    k_Fresh_Skill_Key	= false;
    k_Skill_Key			= false;
    k_keyEmbarkation    = false;
    k_SkillNum	= 0;
    k_Game_Item = KEY_UP;
    k_ShortCut_MacroCommunity = KEY_UP;

    m_bUserInputGuard[GUARD_RIGHT][o5] = false;
    m_bUserInputGuard[GUARD_LEFT][o5] = false;

    // 게임 중일때만 && 포커스를 잃었을 경우 || 채팅모드일 경우에는 유저 입력을 체크하지 않는다.
    if( IsUseKeyCheck() ) 
    {
        InitDirectionKey();
        return;
    }



    //채팅 모드가 아니고,게임 중이면 IME를 사용하지 않는다. 광장도 추가
    if (( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && !g_MyD3D->MyHead->IsCheatMode)
        || (  g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE && KGCSquareOverlayUI::ECF_MAINWND==g_pkSquareOverlayUI->GetCursorFocus()))
    {
        KGCIme::CancelIME();
    }

    if( Right_Dash_Count > 0 )
    {
        Right_Dash_Count--;
    }
    if( Left_Dash_Count  > 0 )
    {
        Left_Dash_Count--;
    }
    if( Up_Dash_Count > 0 )
    {
        Up_Dash_Count--;
    }
    if( Down_Dash_Count > 0 )
    {
        Down_Dash_Count--;
    }

    if( Double_X_Count > 0 )
    {
        Double_X_Count--;
    }

    if ( Double_Z_Count > 0 )
    {
        Double_Z_Count--;
    }

    if ( Double_C_Count > 0 )
    {
        Double_C_Count--;
    }

    if( X_Count > 0 )
    {
        X_Count--;
    }

    if( diks[m_keyUp] && !Old_diks[m_keyUp] )
    {
        if( Up_Dash_Count )
        {
            Up_Dash_Count = 0;

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
                k_Fresh_Down_Dash = true;			
                k_Down = true;
            } else {
                k_Fresh_Up_Dash = true;			
                k_Up = true;
            }
        }
        else
        {            
            Up_Dash_Count = 15;
            Down_Dash_Count = 0;

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
                k_Down        = true;
            } else {
                k_Up        = true;
            }
        }
        k_Fresh_Up  = true;
    }

    if( diks[m_keyUp] )
    {
        k_Push_Up_Charging = true;
    }
    else
    {
        k_Push_Up_Charging = false;
    }

    if( !diks[m_keyUp] && Old_diks[m_keyUp] )
    {
        if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
            k_Down = false;
        } else {
            k_Up = false;
        }
    }

    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
    {
        if( diks[m_keySpecialSkillKey] )
        {
            if( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurFormTemplate().WAIT ||
                g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurFormTemplate().FATAL_WAIT)
            {
                Skill_Key_Count++;
            }

            k_Skill_Key = true;

            if( !Old_diks[m_keySpecialSkillKey] )
            {
                if( Double_C_Count )
                {
                    Double_C_Count = 0;
                    k_Fresh_Double_C = true;
                }
                else
                {
                    Double_C_Count = 20;
                }

                k_Fresh_Skill_Key = true;
            }
        }
        else
        {
            Skill_Key_Count = 0;
            k_Skill_Key = false;
        }
    }

    if( diks[m_keyProvocation] && !Old_diks[m_keyProvocation] )
	{
		k_Fresh_Joke = true;
	}

    if( diks[m_keyDown] && !Old_diks[m_keyDown] )
    {
        if( Down_Dash_Count )
        {
            // 조금 전에도 눌렀음
            Down_Dash_Count = 0;

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
                k_Up              = true;
                k_Fresh_Up_Dash   = true;
            } else {
                k_Fresh_Down_Dash = true;
                k_Down            = true;
            }            
        }
        else
        {            
            Down_Dash_Count = 15;
            Up_Dash_Count = 0;

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
                k_Up          = true;
            } else {
                k_Down          = true;
            }
        }
        k_Fresh_Down    = true;		
    }

    if( !diks[m_keyDown] && Old_diks[m_keyDown] )
    {
        if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) ) {
            k_Up = false;
        } else {
            k_Down = false;
        }        
    }

	//컨퓨젼2 를 위한 변수
	
	if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
	{
		int iRanForConfusion2 = rand()%2;
		if (iRanForConfusion2 != m_iCheckForConfusion2)
		{
			m_iCheckForConfusion2 = iRanForConfusion2;
			InitDirectionKey();	
			switch(m_iCheckForConfusion2)
			{
			case 0:
				Right_Dash_Count = 0;
				break;
			case 1:
				Left_Dash_Count = 0;
				break;
			}
		}

	}

    if( diks[m_keyRight] && !Old_diks[m_keyRight] )
    {
        if( Right_Dash_Count )
        {
            // 조금전에도 눌렀음
            Right_Dash_Count = 0;

            // 좌우 키바꾸기 마법에 걸린 상태일때
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
                g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
            {
                k_Left              = true;
                k_Fresh_Left_Dash   = true;
            }
			else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
			{
				switch(m_iCheckForConfusion2)
				{
				case 0:
					k_Left              = true;
					k_Fresh_Left_Dash   = true;
					break;
				case 1:
					k_Right             = true;
					k_Fresh_Right_Dash  = true;
					break;
				}
			}
            else
            {
                k_Right             = true;
                k_Fresh_Right_Dash  = true;
            }
        }
        else
        {
            // 일반적인 누름 
            Right_Dash_Count = 15;
            Left_Dash_Count = 0;

            if( Left_Dash_Count > 7 )
            {
                // 왼쪽을 누르고 오른쪽을 누른것
                k_Fresh_Left_Right = true;
            }

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
                g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
			{
                k_Left = true;
			}
			else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
			{
				switch(m_iCheckForConfusion2)
				{
				case 0:
					k_Left              = true;
					break;
				case 1:
					k_Right             = true;
					break;
				}
			}
            else
			{
                k_Right = true;
			}
        }
        k_Fresh_Right = true;
        // 5프레임 전 키까지 받아둔다!
        // 마구 연타해서 막는건 쫌.. 근데 좀 더 쉽게 해주고 싶다... 대충 요럴때 막쟈!!
        if ( false == k_Fresh_Right_Dash || ( o5 < 2 ) ) {
            // 피격 모션 취하고 있을 때는 키 입력 받지 말자.
            if( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->IsDamagedMotion() == false ) {
                m_bUserInputGuard[GUARD_RIGHT][o5] = true;
            }            
        }
    }

    if( !diks[m_keyRight] && Old_diks[m_keyRight] )
    {
        if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
            g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
		{
            k_Left = false;
		}
		else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
		{
			switch(m_iCheckForConfusion2)
			{
			case 0:
				k_Left              = false;
				break;
			case 1:
				k_Right             = false;
				break;
			}
		}
        else
		{
            k_Right = false;
		}
    }

    if( diks[m_keyRight] && Old_diks[m_keyRight] )
    {
        k_Fresh_Keep_Right  = true;
    }

    if( diks[m_keyLeft] && !Old_diks[m_keyLeft] )
    {		
        if( Left_Dash_Count )
        {
            // 조금전에도 눌렀음
            Left_Dash_Count     = 0;

            // 좌우 키바꾸기 마법에 걸린 상태일때
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
                g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
            {
                k_Right             = true;
                k_Fresh_Right_Dash  = true;
            }
			else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
			{
				switch(m_iCheckForConfusion2)
				{
				case 0:
					k_Right             = true;
					k_Fresh_Right_Dash  = true;			
					break;
				case 1:
					k_Left              = true;
					k_Fresh_Left_Dash   = true;
					break;
				}
			}
            else
            {
                k_Left              = true;
                k_Fresh_Left_Dash   = true;
            }
        }
        else
        {
            // 일반적인 누름
            Left_Dash_Count     = 15;
            Right_Dash_Count    = 0;

            if( Right_Dash_Count > 7 )
            {
                // 왼쪽을 누르고 오른쪽을 누른것
                k_Fresh_Right_Left = true;
            }

            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
                g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
			{
                k_Right = true;
			}
			else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
			{
				switch(m_iCheckForConfusion2)
				{
				case 0:
					k_Right             = true;		
					break;
				case 1:
					k_Left              = true;
					break;
				}
			}
            else
			{
                k_Left = true;
			}
        }
        k_Fresh_Left = true;

        // 마구 연타해서 막는건 쫌.. 근데 좀 더 쉽게 해주고 싶다... 대충 요럴때 막쟈!!
        if ( false != k_Fresh_Left_Dash || ( o5 < 2 ) )
            m_bUserInputGuard[GUARD_LEFT][o5] = true;
    }

    if( !diks[m_keyLeft] && Old_diks[m_keyLeft] )
    {
        if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) ||
            g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION_REINFORCEMENT ) )
		{
            k_Right = false;
		}
		else if (g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION2 ))
		{
			switch(m_iCheckForConfusion2)
			{
			case 0:
				k_Right             = false;		
				break;
			case 1:
				k_Left              = false;
				break;
			}
		}
        else
		{
            k_Left = false;
		}
    }

    if( diks[m_keyLeft] && Old_diks[m_keyLeft] )
    {
        k_Fresh_Keep_Left  = true;
    }

    //====================================================================================
    // 2009.03.16 : Jemitgge
    // Comment : 무기 훔치기 걸리면... Z,X 키 몬누른다..
    if (  g_MyD3D->m_pStateMachine->GetState() == GS_GAME && 
        ( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_DROP_WEAPON ) || 
        g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_EDEL1_UNIQUE_SKILL1_STEAL_WEAPON ) ) )
    {
        Double_Z_Count	= false;
        k_Fresh_Double_Z= false;
        Double_C_Count  = 0;
        k_Fresh_Double_C= false;
        k_Fresh_Punch	= false;
        k_Fresh_Punch_Curse = false;
        k_Fresh_Fire	= false;
        g_Hot_Count		= 0;
        k_Push_Burnning = false;
        k_Fresh_Double_X = false;
        Skill_Key_Count = 0;
        k_Fresh_Skill_Key = false;
        k_Skill_Key = false;

        if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_EDEL1_UNIQUE_SKILL1_STEAL_WEAPON ) ) {
            // 액션 키 (차지)
            if( diks[m_keyAttack] ) {
                k_Fresh_Punch_Charging = true;
            } else {
              Energy = 0.0f;
            }
        } else {
            k_Fresh_Punch_Charging = false;
        }



        goto DISABLEACTIOINKEY;
    }

    if( g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ] != NULL && g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->CheckPetTransformMotion() ) { 
        goto DISABLEACTIOINKEY;
    }

    // 액션 키 (단타)
    if( diks[m_keyAttack] && !Old_diks[m_keyAttack] )
    {
        if( dwHotCountTime + 200 > ::timeGetTime() )
        {
            g_Hot_Count++;
        }
        else
        {
            g_Hot_Count = 0;
        }
        dwHotCountTime = ::timeGetTime();

        if( Double_Z_Count )
        {
            Double_Z_Count = 0;
            k_Fresh_Double_Z = true;
        }
        else
        {
            Double_Z_Count = 20;
        }

        k_Fresh_Punch = true;
        k_Fresh_Punch_Curse = true;
    }

    // 오토 MP 필살기 사용 확인
    int iAutoMPSpecial = 0;
#if defined( USE_AUTO_MP_SPECIAL )
    if( false == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Is_SP_Character() )
    {
        for(int iLoop=0; iLoop < int(m_vecAutoMPSpecial.size()); ++iLoop)
        {
            if( m_vecAutoMPSpecial[iLoop] )
            {
                k_Fresh_Fire = false;
                iAutoMPSpecial = iLoop + 1;
            }
        }
    }
#endif

    if( 0 == iAutoMPSpecial )
    {
        // 액션 키 (차지)
        if( diks[m_keyAttack] )
        {
            k_Fresh_Punch_Charging = true;
        }
        else
        {
            //if( !diks[DIK_Z] && Old_diks[DIK_Z] )
            if( !diks[m_keyAttack] && Old_diks[m_keyAttack] )
            {
                k_Fresh_Fire = true;
            }
            else if( !k_Fresh_Fire )
            {
                Energy = 0.0f;
            }
        }
    }
    // MP 오토 필살기 처리
    else
    {
        Energy += 0.03f;
        if( Energy > g_MyD3D->MyHead->Level )
        {
            Energy = g_MyD3D->MyHead->Level;
        }
        if( iAutoMPSpecial <= Energy )
        {
            k_Fresh_Fire = true;
            std::vector< bool >::iterator vecIter = m_vecAutoMPSpecial.begin();
            for(; vecIter != m_vecAutoMPSpecial.end(); ++vecIter)
            {
                (*vecIter) = false;
            }
        }
    }

    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
    {
        if( diks[m_keyChangeItem] )
        {
            if( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurFormTemplate().WAIT ||
                g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurFormTemplate().FATAL_WAIT)
            {
                Burnning_X_Count++;

                if( Burnning_X_Count >= 10 )
                {
                    k_Push_Burnning = true;
                }			
            }			
            k_Push_X_Charging = true;
        }
        else
        {
            Burnning_X_Count = 0;
            k_Push_Burnning = false;
            k_Push_X_Charging = false;
        }

        if( diks[m_keyChangeItem] && !Old_diks[m_keyChangeItem] )
        {
            if( Double_X_Count )
            {
                // 조금 전에도 눌렀음
                Double_X_Count = 0;
                k_Fresh_Double_X = true;
            }
            else
            {
                Double_X_Count = 20;
            }

            //====================================================================================
            // 2009.02.19 : Jemitgge
            // Comment : X 눌러서 일정 시간 까지 계속 눌렀던 효과 주고 싶다.. 그래서 머;;
            X_Count = 20;

            k_Select_Item = true;
        }
    }

	if( k_FreshSkillNum )
	{
		k_FreshSkillNum = 0;
	}
	else
	{
		for (unsigned int i = 0; i < m_vecKeySkill.size(); ++i )
		{
			if( diks[m_vecKeySkill[i]] && !Old_diks[m_vecKeySkill[i]] )
			{
				k_FreshSkillNum = i + 1;
#if defined( USE_AUTO_MP_SPECIAL )
                if( i < int(m_vecAutoMPSpecial.size()) )
                {
                    bool bInput = m_vecAutoMPSpecial[i];
                    std::vector< bool >::iterator vecIter = m_vecAutoMPSpecial.begin();
                    for(; vecIter != m_vecAutoMPSpecial.end(); ++vecIter)
                    {
                        (*vecIter) = false;
                    }
                    if( bInput )
                    {
                        Energy = 0.0f;
                    }
                    m_vecAutoMPSpecial[i] = !bInput;
                }
#endif
			}			
		}		
	}

    for (unsigned int i = 0; i < m_vecKeySkill.size(); ++i )
    {
        if( diks[m_vecKeySkill[i]] )
        {
            k_SkillNum = i + 1;
        }			
        if( !diks[m_vecKeySkill[i]] && Old_diks[m_vecKeySkill[i]] )
        {
            if( m_vecKeyCnt[i] == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurrentComboIdx() )
                ++m_vecKeyCnt[i];
        }
    }		



    if( k_MonsterAction )
    {
        k_MonsterAction = 0;
    }
    else
    {
        for (unsigned int i = 0; i < m_vecKeyMonsterAction.size(); ++i )
        {
            if( diks[m_vecKeyMonsterAction[i]] && !Old_diks[m_vecKeyMonsterAction[i]] )
            {
                switch ( i )
                {
                    // 0 -> 이전 모션
                case 0:
                    --g_kGlobalValue.m_iMonsterActionIndex;
                    break;
                    // 1 -> 다음 모션
                case 1:
                    ++g_kGlobalValue.m_iMonsterActionIndex;
                    break;
                    // 2 -> 현재 모션
                case 2:
                    g_kGlobalValue.m_iMonsterActionIndex;
                    break;
                default:
                    break;
                }

                k_MonsterAction = 1;
            }
        }

#if !defined(__PATH__)
        if( diks[ m_keyMonsterDirection ] && !Old_diks[ m_keyMonsterDirection ] )
        {
            if( g_kGlobalValue.m_kUserInfo.bHost && g_kGlobalValue.m_bMonsterAction )
            {
                for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
                    mit->second->SetIsRight( !mit->second->GetIsRight() );
                }
            }
        }
#endif
    }


#if defined( WEAPONCHANGE )
    if ( diks[m_keyWeaponSwap] && !Old_diks[m_keyWeaponSwap] )
    {
        bool bCheck = false;
        PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
        if ( pPlayer )
        {
            std::map<char,std::pair<bool,GCITEMUID> >::iterator mit = g_kGlobalValue.m_mapChangeWeapon.find(pPlayer->GetCurrentChar().iCharType);
            if ( mit != g_kGlobalValue.m_mapChangeWeapon.end() )
            {
                if ( mit->second.first )
                {
                    std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit1 = pPlayer->m_mapSwapWeapon.find( pPlayer->GetCurrentChar().iCharType);
                    if ( mit1 != pPlayer->m_mapSwapWeapon.end() )
                    {
                        if ( mit1->second.first != false || mit1->second.second.second != 0 )
                            bCheck = true;
                    }
                }
            }
        }

#if defined( USE_ONGAME_INVENTORY )
        if( KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_GAME_INVENTORY ) ) { 
            bCheck = false;
        }
#endif

        if( bCheck && g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON &&
            !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_WEAPONCHANGE ) &&
            !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SIEG1_SKILL_CASH_SPECIAL3_BUFF )
            )
        {
            if ( pPlayer->MetamorphosisForm == FORM_NORMAL &&
                (pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WAIT ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().FATAL_WAIT ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
                )
            {
                // 알리자
                KGC_PID_WEAPONCHANGE kPacket;
                kPacket.Type = GC_PID_WEAPONCHANGE;
                kPacket.dwUID = pPlayer->m_kUserInfo.dwUID;
                SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                KGCPC_MEMO( "KGC_PID_WEAPONCHANGE" );

                if ( g_MyD3D->GetMyPlayer() )
                {                 
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_LAS_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_LAS_MOVE_SPEEDUP ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILLEFFECT_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILLEFFECT_MOVE_SPEEDUP ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RYAN_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RYAN_MOVE_SPEEDUP ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_JUMP_UP10, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_JUMP_UP10 ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RONAN3_SPECIAL4_TEMPEST, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RONAN3_SPECIAL4_TEMPEST ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_AMY4_MELOMODE_SPECIAL3_BUFF, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_AMY4_MELOMODE_SPECIAL3_BUFF ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_BOOST_HEAL, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_BOOST_HEAL ) );
                    g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RIN2_GODDESS_UNIQUE_TEAM_BUFF, false, 0 != g_pMagicEffect->IsMagicEffect(g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RIN2_GODDESS_UNIQUE_TEAM_BUFF));
                }
            }
        }
    }
#endif

DISABLEACTIOINKEY:

    if( !diks[m_keyExit] && Old_diks[m_keyExit] )
    {
        k_Fresh_Exit = true;
    }

    if( diks[m_keyUseItem] && !Old_diks[m_keyUseItem] )
    {		
        k_Use_Item = true;
    }

    if( diks[m_keyChangeSkill] && !Old_diks[m_keyChangeSkill] )
    {
        {
            k_Change_SkillType = true;
            k_keyAIConnection = true;
            if( g_MyD3D->EnableKeyInput() )
                k_keyEmbarkation = true;

            // 일본은 용사의 섬에서는 신필살기를 사용할 수 없습니다.
#if defined( NATION_JAPAN )
            {
                k_Change_SkillType = !g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) ;
            }
#endif	// defined( NATION_JAPAN )
        }
        k_DotaShop = true;
    }

    if( diks[m_keyUsePetSkill] && !Old_diks[m_keyUsePetSkill] )
	{		
		k_Use_PetSkill = true;
	}

    if( GC_GMC_DUNGEON != SiKGCRoomManager()->GetGameModeCategory() ||
        SiGCAutoMatchManager()->IsNotPetSpecial() || 
        ( SiKGCWorldMapManager()->IsPrivateDungeon( SiKGCRoomManager()->GetGameMode() ) ) ||
        SiKGCRoomManager()->IsFatalGameMode() )
    { 
        k_Use_PetSkill = false;
    }

    if( !Old_diks[m_keyRight] && diks[m_keyRight] && !Old_diks[m_keyLeft] && diks[m_keyLeft] ) // for usb 이상
    {
        k_Fresh_Left_Right_Sync = true; // 왼쪽이 눌려있는데 오른쪽 눌림
    }

	if( diks[m_keyShowShortCut] && !Old_diks[m_keyShowShortCut] )
	{
		k_ShowShortCut = true;
	}

#if defined( USE_ONGAME_INVENTORY ) 
    if( diks[ m_keyGameInventory ] && !Old_diks[ m_keyGameInventory ] ) { 
        if( g_MyD3D->GetMyPlayer() != NULL && 
            false == g_MyD3D->GetMyPlayer()->IsDie() && 
            true == g_MyD3D->GetMyPlayer()->IsWaitMotion() ) { 
            k_Game_Inventory = !k_Game_Inventory;
        }
    }
#endif

    if( diks[ m_keyCameraZoom ] && !Old_diks[ m_keyCameraZoom ] ) 
    {
        k_keyCameraZoom = true;
    }

	SetShortCutItemKey();
	SetShortCutEmotKey();
}

void Controls::InitDevice()
{
    m_aptexMouse[0]= g_pGCDeviceManager2->CreateTexture("Cursor0.DDS");


    STICKYKEYS sk;
    sk.cbSize = sizeof(STICKYKEYS);
    SystemParametersInfo( SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0 );

    m_stickykeys = sk;

    if( (sk.dwFlags & SKF_STICKYKEYSON) == 0 )
    {
        sk.dwFlags &= ~(SKF_CONFIRMHOTKEY | SKF_HOTKEYACTIVE);
        SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0 );
    }

}

void Controls::DeleteDevice( void )
{
    for ( int i = 0; i < NUM_MOUSE_TEXTURE; ++i )
    {
        SAFE_RELEASE( m_aptexMouse[i] );
    }

    SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &m_stickykeys, 0 );

}

// 마우스 커서를 렌더링한다.
void Controls::Mouse_Render( void )
{
    D3DXMATRIX mat;

    ASSERT( g_pd3dDevice != NULL );

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );    

	::D3DXMatrixTranslation( &mat,
		m_x + 0.06f / g_pGCDeviceManager2->GetWindowScaleX(),
		m_y - 0.06f / g_pGCDeviceManager2->GetWindowScaleY(),
		0.11f );

	g_pGCDeviceManager2->DrawInWorld(m_aptexMouse[0] ,&mat,
		0.12f / g_pGCDeviceManager2->GetWindowScaleX(), 0.12f / g_pGCDeviceManager2->GetWindowScaleY(),
		0.0078125f, 0.0078125f, 1.0078125f, 1.0078125f, 0,0,0,
		D3DCOLOR_ARGB(Mouse_Power,255,255,255));	
    g_pGCDeviceManager2->PopState();

    // [8/27/2007 breadceo] 마우스 커서에 위치정보 표시
    if( g_kGlobalValue.m_bRenderPosition )
    {
#define DRAW_INFO g_pGCDeviceManager2->Draw_Text( m_x, m_y + 0.15f + fYTerm, stm.str().c_str(), 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true )
        // 마우스의 위치를 현재 게임내 좌표로
        float fX, fY;
        POINT ptMPos = g_pkInput->GetMousePos();
        fX = (float)ptMPos.x;
        fY = (float)ptMPos.y;
        Picking( fX, fY );
        float fYTerm = 0.0f;
        std::wstringstream stm;
        stm.precision( 3 );

        // Player
        stm.str( std::wstring() );
        stm << L"Player Position : ( " << fX << L", " << fY << L" )";
        DRAW_INFO;
        fYTerm += 0.03f;

        stm.str( std::wstring() );
        stm << L"Particle Sequence : ( " << fX - 1.0f << L", " << fY - 0.34f << L" )";
        DRAW_INFO;
        fYTerm += 0.03f;

        stm.str( std::wstring() );
        float fRelPosX = fX / g_MyD3D->MyStg->GetXLimit();
        float fRelPosY = fY;
        stm << L"Quest Pos : ( " << fRelPosX << L", " << fRelPosY << L" )";
        DRAW_INFO;
        fYTerm += 0.03f;
    }
}

void Controls::Picking( float& fX, float& fY )
{
    D3DXVECTOR3 vtMPos = D3DXVECTOR3(fX, fY, 0);

    D3DXMATRIX	m_matProj;
    g_MyD3D->m_pd3dDevice->GetTransform( D3DTS_PROJECTION,	&m_matProj );

    D3DVIEWPORT9 vp;
    g_MyD3D->m_pd3dDevice->GetViewport ( &vp );

    //Note: 마우스 좌표에 투영행렬을 역으로 적용하여 공간좌표를 만든다 (=카메라 좌표계)
    D3DXVECTOR3 v;
    v.x = ((  (((vtMPos.x-vp.X)*2.0f/vp.Width ) - 1.0f)) - m_matProj._31 ) / m_matProj._11;
    v.y = ((- (((vtMPos.y-vp.Y)*2.0f/vp.Height) - 1.0f)) - m_matProj._32 ) / m_matProj._22;
    v.z =  1.0f;

    v.x += 1.0f;
    v.x *= 0.5f;
    v.y += 0.75f;
    v.y /= 1.5f;

    //(0,1)    (1,1)   rt.lefttop    rt.righttop
    //-----------           -----------
    //|         |           |         |
    //|      .  |    ->     |      .  |
    //|         |           |         |
    //-----------           -----------
    //(0,0)    (1,0)    rt.leftbtm   rt.rightbtm

    fX = 1.0f + g_kCamera.m_rtCamera.left + ( g_kCamera.m_rtCamera.right - g_kCamera.m_rtCamera.left ) * v.x;
    fY = 0.75f + g_kCamera.m_rtCamera.bottom + ( g_kCamera.m_rtCamera.top - g_kCamera.m_rtCamera.bottom ) * v.y;
    // 잘 안찝혀서 이렇게 해준다
    fY += 0.07f + ( g_kCamera.m_fZoom - 1.0f ) * -0.15f;
}

void Controls::Picking( D3DXVECTOR3& vPos )
{
    Picking( vPos.x, vPos.y );
    vPos.z = 0;
}

void Controls::Picking( D3DXVECTOR2& vPos )
{
    Picking( vPos.x, vPos.y );
}
#if defined(USE_JOYSTICK)
void Controls::SetGamePadKey( void )
{
    for(int i = 0;i<EJK_MAX_NUM;++i){
        if( GamePad[m_aiGamePad[i]] ){
            GamePad[m_aiGamePad[i]] = false;
            diks[m_aiKeyGamePad[i]] = true;
        }
    }
}

int Controls::GetJoystickKey( void )
{
    for(int i = 0;i<EJK_MAX_NUM;++i){
        if( GamePad[i] ){
            GamePad[i] = false;
            return i;
        }
    }

    return -1;
}

int Controls::GetJoystickSelectedKey( int index )
{
    if(index<0 || index >= EJK_MAX_NUM) return 0;

    return m_aiGamePad[index];
}

int Controls::GetJoystickVirtualKey( int index )
{
    if(index<0 || index >= EJK_MAX_NUM) return 0;

    for(int i = 0;i<EJK_MAX_NUM;++i){
        if( m_aiSettingGamePad[i] == static_cast<BYTE>(index) ){
            return i;
        }
    }

    return 0;
}

void Controls::SetJoystickKey( BYTE byteKey , int Select)
{
    for(int i = 0;i<EJK_MAX_NUM;++i){
        if( m_aiSettingGamePad[i] == byteKey ){
            m_aiSettingGamePad[i] = m_aiSettingGamePad[Select];
            break;
        }
    }
    
    m_aiSettingGamePad[Select] = byteKey;

    SetVirtualGamePad(false);
}

void Controls::InitVirtualGamePadKey( )
{
    for(int i = 0;i<EJK_MAX_NUM;++i){
        m_aiGamePad[i] = i;
    }
    for(int i = 0;i<EJK_MAX_NUM;++i){
        m_aiSettingGamePad[i] = i;
    }
}

void Controls::SetVirtualGamePad( bool bDefault )
{
    if(bDefault){
        for(int i = 0;i<EJK_MAX_NUM;++i){
            m_aiGamePad[i] = i;
        }
    }
    else{
        for(int i = 0;i<EJK_MAX_NUM;++i){
            m_aiGamePad[i] = m_aiSettingGamePad[i];
        }
    }

}

void Controls::SetVirtualGamePadKey( void )
{
    m_aiKeyGamePad[EJK_UP] = m_keyUp;
    m_aiKeyGamePad[EJK_DOWN] = m_keyDown;
    m_aiKeyGamePad[EJK_LEFT] = m_keyLeft;
    m_aiKeyGamePad[EJK_RIGHT] = m_keyRight;
    m_aiKeyGamePad[EJK_KEY_1] = m_keyAttack;
    m_aiKeyGamePad[EJK_KEY_2] = m_keyProvocation;
    m_aiKeyGamePad[EJK_KEY_3] = m_keyChangeItem;
    m_aiKeyGamePad[EJK_KEY_4] = m_keyUseItem;
    m_aiKeyGamePad[EJK_KEY_5] = m_keyChangeSkill;
    m_aiKeyGamePad[EJK_KEY_6] = m_keyUsePetSkill;
    m_aiKeyGamePad[EJK_KEY_7] = m_keySpecialSkillKey;
    m_aiKeyGamePad[EJK_KEY_8] = m_keyWeaponSwap;
    m_aiKeyGamePad[EJK_KEY_SELECT] = m_vecKeySkill[0];
    m_aiKeyGamePad[EJK_KEY_START] = m_vecKeySkill[1];
    m_aiKeyGamePad[EJK_KEY_DIGITAL_L] = m_vecKeySkill[2];
    m_aiKeyGamePad[EJK_KEY_DIGITAL_R] = m_vecKeySkill[3];
    m_aiKeyGamePad[EJK_KEY_DIGITAL] = m_vecKeySkill[4];
}
#endif


// 단축 슬롯 키 입력에 따른 값을 k_ShortCutItem_* 변수에 매핑한다.
void Controls::SetShortCutItemKey( void )
{
    if( k_ShortCut_Item == KEY_UP )
    {
        for (unsigned int i = 0; i < m_vecKeyShortCutItem.size(); ++i )
        {
            if( diks[m_vecKeyShortCutItem[i]] )
            {
                k_ShortCut_Item = i;
            }			
        }		
    }
    else
    {
        k_ShortCut_Item = KEY_UP;
    }
}
// 단축 슬롯 키 입력에 따른 값을 k_ShortCutEmot_* 변수에 매핑한다.
void Controls::SetShortCutEmotKey( void )
{
    if( k_ShortCut_Emot == KEY_UP || k_Game_Item == KEY_UP || k_ShortCut_MacroCommunity  == KEY_UP )
    {
        for (unsigned int i = 0; i < m_vecKeyShortCutEmot.size(); ++i )
        {
            // Press
            if( diks[m_vecKeyShortCutEmot[i]] )
            {
                if( k_ShortCut_Emot == KEY_UP )
                    k_ShortCut_Emot = i;
            }			

            // DownUp
            if( diks[m_vecKeyShortCutEmot[i]] && !Old_diks[m_vecKeyShortCutEmot[i]] )
            {
                if( k_Game_Item == KEY_UP )
                    k_Game_Item = i;
                if( k_ShortCut_MacroCommunity == KEY_UP )
                    k_ShortCut_MacroCommunity = i;
            }
        }
    }
}

void Controls::UpdateMousePos( D3DXVECTOR3 vPos )
{
    UpdateMousePos( vPos.x, vPos.y );
}

void Controls::UpdateMousePos( D3DXVECTOR2 vPos )
{
    UpdateMousePos( vPos.x, vPos.y );
}

void Controls::UpdateMousePos( float fX, float fY )
{
    Picking( fX, fY );
    m_vMousePos.x = fX;
    m_vMousePos.y = fY;
    m_vMousePos.z = 0;
}

void Controls::CheckSleepFramemove()
{
#ifndef FORCE_KICK_SLEEP_USER
    return;
#endif

    // 이번 프레임은 입력이 없었다.
    if( memcmp( Old_diks, diks, 256 ) == 0 && 
        ptCursor.x == ptOldCursor.x && ptCursor.y == ptOldCursor.y )
    {
        m_dwSleepFrame++;

        // 16500 = GC_FPS_LIMIT * 60 * 5
        if( m_dwSleepFrame >= 18000 )
            //if( m_dwSleepFrame >= 30 ) //for test
        {
            m_bSleep = true;
        }
    }
    else
    {
        NoSleep();
    }
}

void Controls::NoSleep()
{
    m_dwSleepFrame = 0;
    m_bSleep = false;
}

void Controls::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L);
    lua_tinker::class_add<Controls>( L, "Controls" );
    lua_tinker::class_mem<Controls>( L, "Up",   &Controls::k_Up );
    lua_tinker::class_mem<Controls>( L, "Down",   &Controls::k_Down );
    lua_tinker::class_mem<Controls>( L, "Left",   &Controls::k_Left );
    lua_tinker::class_mem<Controls>( L, "Right",   &Controls::k_Right );

    lua_tinker::class_mem<Controls>( L, "Fresh_Up",   &Controls::k_Fresh_Up );
    lua_tinker::class_mem<Controls>( L, "Fresh_Down",   &Controls::k_Fresh_Down );
    lua_tinker::class_mem<Controls>( L, "Fresh_Left",   &Controls::k_Fresh_Left );
    lua_tinker::class_mem<Controls>( L, "Fresh_Right",   &Controls::k_Fresh_Right );
    lua_tinker::class_mem<Controls>( L, "Fresh_Keep_Left",   &Controls::k_Fresh_Keep_Left );
    lua_tinker::class_mem<Controls>( L, "Fresh_Keep_Right",   &Controls::k_Fresh_Keep_Right );

    lua_tinker::class_mem<Controls>( L, "Fresh_Punch",   &Controls::k_Fresh_Punch );
    lua_tinker::class_mem<Controls>( L, "Fresh_Punch_Charging",   &Controls::k_Fresh_Punch_Charging );
    lua_tinker::class_mem<Controls>( L, "Fresh_Fire",   &Controls::k_Fresh_Fire );

    lua_tinker::class_mem<Controls>( L, "Push_Burnning",   &Controls::k_Push_Burnning );
    lua_tinker::class_mem<Controls>( L, "Fresh_Right_Dash",   &Controls::k_Fresh_Right_Dash );
    lua_tinker::class_mem<Controls>( L, "Fresh_Left_Dash",   &Controls::k_Fresh_Left_Dash );
    lua_tinker::class_mem<Controls>( L, "Fresh_Down_Dash",   &Controls::k_Fresh_Down_Dash );
    lua_tinker::class_mem<Controls>( L, "Fresh_Up_Dash",   &Controls::k_Fresh_Up_Dash );
    lua_tinker::class_mem<Controls>( L, "Fresh_Double_X",   &Controls::k_Fresh_Double_X );
    lua_tinker::class_mem<Controls>( L, "Fresh_Double_Z",   &Controls::k_Fresh_Double_Z );
    lua_tinker::class_mem<Controls>( L, "Fresh_Double_C",   &Controls::k_Fresh_Double_C );
    lua_tinker::class_mem<Controls>( L, "Fresh_Left_Right_Sync",   &Controls::k_Fresh_Left_Right_Sync );
    lua_tinker::class_mem<Controls>( L, "Fresh_Left_Right",   &Controls::k_Fresh_Left_Right );
    lua_tinker::class_mem<Controls>( L, "Fresh_Right_Left",   &Controls::k_Fresh_Right_Left );
    lua_tinker::class_mem<Controls>( L, "Fresh_Skill_Key",   &Controls::k_Fresh_Skill_Key );
    lua_tinker::class_mem<Controls>( L, "Skill_Key",   &Controls::k_Skill_Key );
    lua_tinker::class_mem<Controls>( L, "SkillNum",   &Controls::k_SkillNum );
    lua_tinker::class_mem<Controls>( L, "Push_X_Charging",   &Controls::k_Push_X_Charging );
    lua_tinker::class_mem<Controls>( L, "Push_Up_Charging",   &Controls::k_Push_Up_Charging );


    lua_tinker::class_mem<Controls>( L, "Fresh_Joke",   &Controls::k_Fresh_Joke );
    lua_tinker::class_mem<Controls>( L, "Fresh_Pet",   &Controls::k_Use_PetSkill );
    lua_tinker::class_mem<Controls>( L, "Select_Item",  &Controls::k_Select_Item );

    lua_tinker::class_mem<Controls>( L, "mousex",  &Controls::m_x );
    lua_tinker::class_mem<Controls>( L, "mousey",  &Controls::m_y );

    lua_tinker::class_mem<Controls>( L, "Number",  &Controls::k_ShortCut_Emot );
    lua_tinker::class_mem<Controls>( L, "keyAIConnection",  &Controls::k_keyAIConnection );
    lua_tinker::class_mem<Controls>( L, "keyEmbarkation",  &Controls::k_keyEmbarkation );


    lua_tinker::class_def<Controls>( L,  "IsBlackMagic", &Controls::GetIsBlackMagic  );

    ;    

    lua_tinker::decl( L, "g_kControls", this );
}

bool Controls::GetIsBlackMagic()
{
    return g_MyD3D->MyHead->IsBlackMagic;
}


bool Controls::IsUseKeyCheck()
{
    // 튜토리얼이나 게임모드가 아닐때
    if( (g_MyD3D->m_pStateMachine->GetState() != GS_GAME) && (g_MyD3D->m_pStateMachine->GetState() != GS_TUTORIAL))
        return false;

    //	아지트 게임모드 일때.
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        if ( SiKGCAgitGameManager()->IsUseKeyCheck() ) 
        {
            return true;
        }
        else
            return false;
    }
    //	일반적인 게임모드
    else
    {
        if( g_MyD3D->m_bHasFocus && !g_MyD3D->MyHead->IsCheatMode )
            return false;
    }

    return true;
}

void Controls::InitDirectionKey()
{
	k_Up = false;
	k_Down = false;		
	k_Left = false;		
	k_Right = false;
    Energy = 0.0f;
	ZeroMemory( &diks, sizeof(diks) );
}

void Controls::CheckChange()
{
    if( k_Fresh_Punch_Charging )
    {
        Energy += 0.03f;
        if( Energy > g_MyD3D->MyHead->Level )
        {
            Energy = g_MyD3D->MyHead->Level;
        }
    }
}