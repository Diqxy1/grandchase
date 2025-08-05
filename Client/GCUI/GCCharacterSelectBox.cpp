#include "stdafx.h"
#include "GCCharacterLockBox.h"
#include "GCCharacterHoverBox.h"
#include "GCCharacterSelectBox.h"
#include ".\gcskilltreedlg.h"

IMPLEMENT_CLASSNAME( KGCCharacterSelectBox );
IMPLEMENT_WND_FACTORY( KGCCharacterSelectBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterSelectBox, "gc_character_select_box" );

KGCCharacterSelectBox::KGCCharacterSelectBox( void )
:	m_pkHoverBox(NULL)
,	m_pkLockBox(NULL)
,	m_pkClose(NULL)
,	m_pkStaticDesc(NULL)
,	m_pkBackDesc(NULL)
,	m_iState(NUM_GAME_STATE)
,	m_bNonStopMode(false)
,	m_pkTitle(NULL)

{
    LINK_CONTROL( "character_hover_box",   m_pkHoverBox );	
    LINK_CONTROL( "character_lock_box",   m_pkLockBox );	
    LINK_CONTROL( "close_btn",   m_pkClose );	
    LINK_CONTROL( "description_box",   m_pkBackDesc );	
    LINK_CONTROL( "static_descript",   m_pkStaticDesc );	
    LINK_CONTROL( "Title",   m_pkTitle );	
}

KGCCharacterSelectBox::~KGCCharacterSelectBox( void )
{
}

void KGCCharacterSelectBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnClose();
        return;
    }

}

void KGCCharacterSelectBox::OnCreate( void )
{
    m_pkHoverBox->InitState(true,true,this);
    m_pkLockBox->InitState(true,true,this);
    m_pkClose->InitState(true,true,this);
    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkStaticDesc->InitState(true,false,false);    
    m_pkBackDesc->InitState(true,false,false);

    m_pkStaticDesc->SetText( g_pkStrLoader->GetString( STR_ID_CHARACTER_SELECT_OTHER ));

}

int KGCCharacterSelectBox::GetSelectCharacter(){
    int iChar = static_cast<int>(g_kGlobalValue.m_kUserInfo.cCharIndex);
    SCharInfo &userinfo = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iChar];

    return userinfo.iCharType;
}


void KGCCharacterSelectBox::FrameMoveInEnabledState( void )
{
    if( m_iState != g_MyD3D->m_pStateMachine->GetState() )
    {
        OnClose();
        return;
    }
}

void KGCCharacterSelectBox::InitCharacterSelectBox(GAME_STATE state, boost::function2<void,int,int> a_Setfunc
                                                   ,boost::function0<void> a_UpdateFunc,D3DXVECTOR2 vPos,
                                                   STATE_POSITION iStatePos, EXPERT_SKILL es, 
                                                   DWORD dwParam1, DWORD dwParam2, int iSelectCharIdx,
                                                   PRIVATE_CAHR_TYPE emCharState, int nPrivateCharType )
{
    m_SetFunc = a_Setfunc;
    m_UpdateFunc  = a_UpdateFunc;
    m_iState = state;
    m_bNonStopMode = g_pkUIScene->IsStop();

    int iOpenedChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
	int iCloseChar = g_kGlobalValue.GetOpenedCharacterNum() - iOpenedChar;

//#if defined(NATION_BRAZIL)
//	//브라질 대회서버일때 땜빵 코드, 디오,제로는 제외한다. 시간없어서 쳐바름..ㅡㅠㅡ
//	if (g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) == true)
//	{
//		iCloseChar-=2;
//	}
//#endif

    if(es == EXPERT_SKILL_OPEN_CHARACTER_ALL ||
        es == EXPERT_SKILL_NEED_LEVEL||
        es == EXPERT_SKILL_PROMOTION )
    {
        iCloseChar = 0;
    }

    D3DXVECTOR2 vPosHover(m_pkHoverBox->InitBox(a_Setfunc, m_UpdateFunc, es, dwParam1, dwParam2, iSelectCharIdx));
    D3DXVECTOR2 vPosLock(m_pkLockBox->InitBox(es, dwParam1, dwParam2));
    D3DXVECTOR2 vPosClose;
    D3DXVECTOR2 vPosBackDesc;
    D3DXVECTOR2 vPosStaticDesc;

    D3DXVECTOR2 sizebox(vPosHover);


    if(iCloseChar ==0 || iStatePos == STATE_POSITION_NONE){
        m_pkLockBox->ToggleRender(false);
        //vPosLock.y = 0.0f;
    }
    else
    {
        m_pkLockBox->ToggleRender(true);
    }


    sizebox.y = vPosHover.y + vPosLock.y - 8.0f;

    SetWidth(static_cast<DWORD>(sizebox.x));
    SetHeight(static_cast<DWORD>(sizebox.y));

    vPosLock.x = (vPosHover.x - vPosLock.x)/2.0f-2.0f;
    vPosHover.x = 0.0f;
    vPosClose.x = 0.0f;
    sizebox.x = 0.0f;

    if(iStatePos != STATE_POSITION_DOWN)
    {
        //A¶½E ¼ø¼­ ºA²i¸e AI≫oCØAu
        vPosHover.y = sizebox.y - vPosHover.y;
        vPosLock.y = 0.0f;
        vPosClose.y = sizebox.y - 6.0f;
        sizebox.y *= -1.0f;
    }
    else
    {
        vPosHover.y = 0.0f;
        vPosLock.y = sizebox.y - vPosLock.y - 3.0f;
        vPosClose.y = -26.0f;
    }

    if( es == EXPERT_SKILL_MISSION )
        sizebox = vPos;
    else
        sizebox += vPos;

    SetWindowPosDirect(sizebox);

    m_pkHoverBox->SetWindowPosDirect(vPosHover);
    m_pkLockBox->SetWindowPosDirect(vPosLock);
    m_pkClose->SetWindowPosDirect(vPosClose);
    if(m_pkTitle != NULL && es == EXPERT_SKILL_PROMOTION ){
        //int tilteposx =0;
        //KLuaManager luaMgr;
        //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        //LUA_GET_VALUE_DEF( "move_x", tilteposx, 0);

        vPosHover.y += -25.0f; 

        m_pkTitle->SetWidth(253);

        m_pkTitle->SetWindowPosDirect(vPosHover);
        m_pkTitle->ToggleRender(true);

    }
    else if(m_pkTitle != NULL)
    {
        m_pkTitle->ToggleRender(false);
    }

    //float closePosX =0.0f;
    //float closePosY =0.0f;
    //float closePosZ =0.0f;

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "move_x", closePosX, 0.0f );
    //LUA_GET_VALUE_DEF( "move_y", closePosY, 0.0f );
    //LUA_GET_VALUE_DEF( "move_z", closePosZ, 0.0f );

    vPosBackDesc.x = 73.0f;
    vPosBackDesc.y = vPosClose.y;

    vPosStaticDesc.x = 73.0f + 8.0f;
    vPosStaticDesc.y = vPosClose.y+ 6.0f;

    m_pkStaticDesc->SetWindowPosDirect(vPosStaticDesc);
    m_pkBackDesc->SetWindowPosDirect(vPosBackDesc);

	SetWindowZOrder(D3DWZ_TOPMOST);

    switch( g_MyD3D->m_pStateMachine->GetState() )
    {
    case GS_ROOM:
        g_pkRoomChatBox->m_pkEdit->Activate( false );
        break;
    case GS_SHOP_CASH:
        g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus( false );
        break;
    case GS_SHOP_GP:
        g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus( false );
        break;
    case GS_MATCH:
        g_pkLobbyChatBox->m_pkEdit->Activate( false );
        break;
    case GS_WORLDMAP:
        g_pkWorldmapChatBox->m_pkEdit->Activate( false );
        break;
}
}

void KGCCharacterSelectBox::OnClose()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    g_kGlobalValue.m_bShowItemInfoBox = false;
}
