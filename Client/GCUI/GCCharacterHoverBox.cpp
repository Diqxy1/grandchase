#include "stdafx.h"
#include "GCCharacterHoverBox.h"
#include "GCCharacterLockBox.h"
#include ".\gcskilltreedlg.h"


IMPLEMENT_CLASSNAME( KGCCharacterHoverBox );
IMPLEMENT_WND_FACTORY( KGCCharacterHoverBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterHoverBox, "gc_character_hover_box" );

KGCCharacterHoverBox::KGCCharacterHoverBox( void )
:	m_pkBack(NULL)
,m_SetFunc(NULL)
,m_UpdateFunc(NULL)
,m_iState(0)
,m_iCharKeySelect(0)
,m_iAlphaCnt( 0 )
,m_iSwitch( 8 )
,m_iSelectCharIdx(-1)
,m_iInputkeyDelayTime(0)
{
	m_mapSelectChar[-1] = NULL;
	LINK_CONTROL("character_common_icon",m_mapSelectChar[-1]);
    for( int i = 0 ; i < GC_CHAR_NUM; ++i)
    {
        m_mapSelectChar[i] = NULL;
        char strChar[128] = {0,};
        sprintf(strChar, "character_icon%d",i);
        LINK_CONTROL( strChar,          m_mapSelectChar[i] );

        m_pkMarkMissionNew[i] = NULL;
        sprintf(strChar, "mark_mission_new%d",i);
        LINK_CONTROL( strChar,          m_pkMarkMissionNew[i] );

        m_pkMarkMissionComplate[i] = NULL;
        sprintf(strChar, "mark_mission_complate%d",i);
        LINK_CONTROL( strChar,          m_pkMarkMissionComplate[i] );

        m_pkMarkSeal[i] = NULL;
        sprintf(strChar, "mark_event_seal%d",i);
        LINK_CONTROL( strChar,          m_pkMarkSeal[i] );

        m_pkMarkIngArrow[i] = NULL;
        sprintf(strChar, "mark_event_ing_arrow%d",i);
        LINK_CONTROL( strChar,          m_pkMarkIngArrow[i] );
    }


    LINK_CONTROL("back",m_pkBack);
    m_mapArrowAniInfo.clear();
}

KGCCharacterHoverBox::~KGCCharacterHoverBox( void )
{
}

void KGCCharacterHoverBox::OnCreate( void )
{
    std::map<int,KD3DWnd*>::iterator it = m_mapSelectChar.begin();
    for(;it != m_mapSelectChar.end();it++){
        if(it->second != NULL){
            it->second->InitState(true,true,this);
        }
    }

    for(int iLoop=0; iLoop<GC_CHAR_NUM; ++iLoop)
    {
        if( m_pkMarkMissionNew[iLoop] )
            m_pkMarkMissionNew[iLoop]->InitState(false, false);
        if( m_pkMarkMissionComplate[iLoop] )
            m_pkMarkMissionComplate[iLoop]->InitState(false, false);
        if( m_pkMarkSeal[iLoop] )
            m_pkMarkSeal[iLoop]->InitState(false, false);
        if( m_pkMarkIngArrow[iLoop] )
            m_pkMarkIngArrow[iLoop]->InitState(false, false);
    }

    m_pkBack->InitState(true,false,NULL);
}

bool KGCCharacterHoverBox::IsPromotionCharacter( int es, int iItemID, int iBasePromotionLevel, const SCharInfo& cCharInfo_ )
{
    if(es != 5) 
        return false;

    if( cCharInfo_.iPromotionLevel >= iBasePromotionLevel ) 
        return true;

    KEVENT_ITEM_CHARPROMOTION_LEVEL_NOT::iterator mIter = g_kGlobalValue.m_mapItemCharpromotionLevel.find(iItemID*10);
    if(mIter ==  g_kGlobalValue.m_mapItemCharpromotionLevel.end() ) 
        return true;

    std::map<char, DWORD> &mIterCharPromotion = mIter->second;
    std::map<char, DWORD>::iterator mIterLevel = mIterCharPromotion.find( cCharInfo_.iCharType );
    if(mIterLevel == mIterCharPromotion.end()) 
        return true;

    if( mIterLevel->second > static_cast<DWORD>( cCharInfo_.iLevel ) )
        return true;

    return false;
}

D3DXVECTOR2 KGCCharacterHoverBox::InitBox(boost::function2<void,int,int> a_Setfunc, boost::function0<void> a_Updatefunc,
                                          int es, DWORD dwParam1, DWORD dwParam2, int iSelectCharIdx, 
                                          int nPrivateCharState, int nPrivateCharType  )
{

    m_iState = es;
    m_SetFunc = NULL;
    m_UpdateFunc = NULL;

    float movex = 60.0f;
    float movey = 60.0f;
    float movenx = 7.0f;
    float moveny = 6.0f;

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "hover_move_x", movex, 0.0f );
    //LUA_GET_VALUE_DEF( "hover_move_y", movey, 0.0f );
    //LUA_GET_VALUE_DEF( "hover_move_nx", movenx, 0.0f );
    //LUA_GET_VALUE_DEF( "hover_move_ny", moveny, 0.0f );

    D3DXVECTOR2 vpos;

    std::map<int, KD3DWnd*>::iterator mIter = m_mapSelectChar.begin();
    KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

    for(;mIter != m_mapSelectChar.end();mIter++)
    {
        if(mIter->second != NULL)
        {
            mIter->second->SetWndMode(D3DWM_DEFAULT);
            mIter->second->ToggleRender(false);
        }
    }

    int iChar = static_cast<int>(g_kGlobalValue.m_kUserInfo.cCharIndex);
    SCharInfo &userinfo = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iChar];
    m_iSelectCharIdx = ( -1 > iSelectCharIdx ) ? userinfo.iCharType : iSelectCharIdx;

    std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( m_iSelectCharIdx );
    if( mapSealedIter != g_kGlobalValue.m_mapSealedCharInfo.end() )
    {
        if( mapSealedIter->second.m_bIsSealed )
        {
            std::map< int, KSealedCharInfo >::iterator mapIter = g_kGlobalValue.m_mapSealedCharInfo.begin();
            for(; mapIter != g_kGlobalValue.m_mapSealedCharInfo.end(); ++mapIter)
            {
                if( false == mapIter->second.m_bIsSealed )
                {
                    m_iSelectCharIdx = mapIter->second.m_nCharType;
                    break;
                }
            }
            // 다 봉인되있으면?? 선택가능한 케릭이 없을 수 있을까? 없지없지.
        }
    }
    
    m_iCharKeySelect = m_iSelectCharIdx;
    m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);

    // 부가 UI초기화
    for(int iLoop=0; iLoop<GC_CHAR_NUM; ++iLoop)
    {
        if( m_pkMarkMissionNew[iLoop] )
            m_pkMarkMissionNew[iLoop]->ToggleRender( false );
        if( m_pkMarkMissionComplate[iLoop] )
            m_pkMarkMissionComplate[iLoop]->ToggleRender( false );
        if( m_pkMarkSeal[iLoop] )
        {
            m_pkMarkSeal[iLoop]->ToggleRender( false );
            m_pkMarkSeal[iLoop]->Lock( false );
        }
        
        if( m_pkMarkIngArrow[iLoop] )
            m_pkMarkIngArrow[iLoop]->ToggleRender( false );
    }

    int iOpendChar = 0;
    KCharInfoVector::iterator vIter = vecChar.begin();
    if(es == KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL)
    {
        mIter = m_mapSelectChar.begin();
        for(;mIter != m_mapSelectChar.end();mIter++)
        {
            if(mIter->second != NULL && g_kGlobalValue.IsOpenedChar(mIter->first))
            {
                vpos.x = ((movex * (iOpendChar%4))+movenx);
                vpos.y = ((movey * (iOpendChar/4))+moveny);

                mIter->second->SetWindowPos(vpos);
                mIter->second->ToggleRender(true);
                iOpendChar++;
            }
        }
    }
    else if ( es == KGCCharacterSelectBox::EXPERT_SKILL_DEPOT_ADD_COMMON ) {
		mIter = m_mapSelectChar.find(-1);
		if(mIter != m_mapSelectChar.end())
		{
			vpos.x = ((movex * (iOpendChar%4))+movenx) ;
			vpos.y = ((movey * (iOpendChar/4))+moveny);

			mIter->second->SetWindowPos(vpos);
			mIter->second->ToggleRender(true);

			iOpendChar++;
		}
		for(;vIter != vecChar.end();vIter++)
		{
			mIter = m_mapSelectChar.find(vIter->iCharType);
			if(mIter != m_mapSelectChar.end())
			{
				vpos.x = ((movex * (iOpendChar%4))+movenx) ;
				vpos.y = ((movey * (iOpendChar/4))+moveny);

				mIter->second->SetWindowPos(vpos);
				mIter->second->ToggleRender(true);

				iOpendChar++;
			}
		}

	}
    else
    {
        for(;vIter != vecChar.end();vIter++)
        {
            mIter = m_mapSelectChar.find(vIter->iCharType);
            if(mIter != m_mapSelectChar.end())
            {
                if(es == KGCCharacterSelectBox::EXPERT_SKILL_SKILL_OPEN_CHARACTER && g_pkUIScene->m_pkSkillTree->CheckSkillOpenChar( vIter->iCharType ) == false ){
                    if(m_iCharKeySelect == vIter->iCharType)
                        m_iCharKeySelect = 0;
                    continue;
                }
                
                if(es == KGCCharacterSelectBox::EXPERT_SKILL_NEED_LEVEL && vIter->iLevel < static_cast<int>(dwParam1) ){
                    if(m_iCharKeySelect == vIter->iCharType)
                        m_iCharKeySelect = 0;
                    continue;
                }
                
                if( IsPromotionCharacter( es, static_cast<int>(dwParam2), static_cast<int>(dwParam1), *vIter ) ){
                    if(m_iCharKeySelect == vIter->iCharType)
                        m_iCharKeySelect = 0;
                    continue;
                }
                vpos.x = ((movex * (iOpendChar%4))+movenx) ;
                vpos.y = ((movey * (iOpendChar/4))+moveny);

                mIter->second->SetWindowPos(vpos);
                mIter->second->ToggleRender(true);

                // 미션완료UI
                bool bComplete = false;
                if(es == KGCCharacterSelectBox::EXPERT_SKILL_MISSION && 0 < g_kGlobalValue.m_kUserMission.GetCompleteMissionNum(vIter->iCharType))
                {
                    m_pkMarkMissionComplate[vIter->iCharType]->SetWindowPos(vpos);
                    m_pkMarkMissionComplate[vIter->iCharType]->ToggleRender( (bComplete = true) );
                }

                // 새로운 미션UI
                if(es == KGCCharacterSelectBox::EXPERT_SKILL_MISSION && g_kGlobalValue.m_kUserMission.IsNewMission(vIter->iCharType) && !bComplete)
                {
                    D3DXVECTOR2 vMissionUIPos = vpos;
                    vMissionUIPos.x += mIter->second->GetWidth() - m_pkMarkMissionNew[vIter->iCharType]->GetWidth();
                    m_pkMarkMissionNew[vIter->iCharType]->SetWindowPos(vMissionUIPos);
                    m_pkMarkMissionNew[vIter->iCharType]->ToggleRender(true);
                }

                iOpendChar++;
            }
        }
    }

    int iCount = 0;
    std::map<int,KD3DWnd*>::iterator mapWinIter = m_mapSelectChar.begin();
    for(; mapWinIter != m_mapSelectChar.end(); ++mapWinIter)
    {
        if( mapWinIter->second->IsRenderOn() )
        {
            InitCharPrivate( mapWinIter->first, iCount, D3DXVECTOR4(movex, movey, movenx, moveny), nPrivateCharState, nPrivateCharType );
            InitCharSeal( mapWinIter->first, iCount, D3DXVECTOR4(movex, movey, movenx, moveny) );

            iCount++;
        }
    }

    //int x = 0;
    //int y = 0;
    //int with = 0;
    //int height = 0;

    //LUA_GET_VALUE_DEF( "hover_pos_x", x, 0 );
    //LUA_GET_VALUE_DEF( "hover_pos_y", y, 0 );
    //LUA_GET_VALUE_DEF( "hover_size_x", with, 0 );
    //LUA_GET_VALUE_DEF( "hover_size_y", height, 0 );

    int sizey = (iOpendChar-1)/4;

    sizey = 77+ (60 *sizey);
    m_pkBack->SetSize(258,sizey);

    m_SetFunc = a_Setfunc;
    m_UpdateFunc = a_Updatefunc;

    D3DXVECTOR2 vec(258,static_cast<float>(sizey));
    return vec;
}

// key값이 0이 될때 
bool KGCCharacterHoverBox::SetCharNum_KeyDown(int key, int iCursorChar)
{
    if(iCursorChar == -100){ 
        m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_DEFAULT);
        iCursorChar = m_iCharKeySelect;
    }
    int iNowCursorChar = iCursorChar;
    iNowCursorChar += key > 0 ? 1 : -1;

 //   if(iKey < 0) return false;
 //   if(iKey >= static_cast<int>(m_mapSelectChar.size())) return false;

	if (g_kGlobalValue.IsQuickSkillPlayer(iNowCursorChar)==true && 
		g_kGlobalValue.ServerInfo.CheckServerType( ST_NO_AP_CHAR ) == true)
	{
		return false;
	}

    std::map<int,KD3DWnd*>::iterator mIter = m_mapSelectChar.find( iNowCursorChar ); 
    if(mIter == m_mapSelectChar.end())
        return false;   

    if( mIter->second->IsRenderOn() == true )
    {
        if(key >0)	key--;
        else		key++;

        if(key == 0)
        {
            std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( iNowCursorChar );
            if( mapSealedIter->second.m_bIsSealed || m_mapSelectChar[iNowCursorChar]->IsLocked() )
            {
                if( g_pkInput->IsDown(DIK_LEFT) 
#ifdef USE_JOYSTICK 
                    || g_MyD3D->GetJoystickKey() == EJK_LEFT 
#endif 
                    )
                    key = -1;
                else if( g_pkInput->IsDown(DIK_RIGHT) 
#ifdef USE_JOYSTICK 
                    || g_MyD3D->GetJoystickKey() == EJK_RIGHT 
#endif 
                    )
                    key = 1;
                else if( g_pkInput->IsDown(DIK_UP) 
#ifdef USE_JOYSTICK  
                    || g_MyD3D->GetJoystickKey() == EJK_UP 
#endif 
                    )
                    key = -4;
                else if( g_pkInput->IsDown(DIK_DOWN) 
#ifdef USE_JOYSTICK  
                    || g_MyD3D->GetJoystickKey() == EJK_DOWN
#endif 
                    )
                    key = 4;
            }
            else
            {
                m_iCharKeySelect = iNowCursorChar;
                return true;
            }
        }
    }

    return SetCharNum_KeyDown(key, iNowCursorChar);
}

void KGCCharacterHoverBox::FrameMoveInEnabledState( void )
{
    if( g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
        return;

    //int iChar = static_cast<int>(g_kGlobalValue.m_kUserInfo.cCharIndex);
    //SCharInfo &userinfo = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iChar];
    //m_mapSelectChar[userinfo.iCharType]->SetWndMode(D3DWM_ACTIVE);
    m_mapSelectChar[m_iSelectCharIdx]->SetWndMode(D3DWM_ACTIVE);
    m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);

    MarkArrowAnimation();

    if(g_pkInput->IsDown( DIK_LEFT) 
#ifdef USE_JOYSTICK
        || (g_MyD3D->GetJoystickKey() == EJK_LEFT && m_iInputkeyDelayTime <= 0)
#endif
        )
    {
#ifdef USE_JOYSTICK
        m_iInputkeyDelayTime = 30;
#endif
        SetCharNum_KeyDown(-1);
        m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);
    }
    else if(g_pkInput->IsDown( DIK_RIGHT)
#ifdef USE_JOYSTICK
        || (g_MyD3D->GetJoystickKey() == EJK_RIGHT && m_iInputkeyDelayTime <= 0)
#endif
        )
    {
#ifdef USE_JOYSTICK
        m_iInputkeyDelayTime = 30;
#endif
        SetCharNum_KeyDown(1);
        m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);
    }
    else if(g_pkInput->IsDown( DIK_UP)
#ifdef USE_JOYSTICK
        || (g_MyD3D->GetJoystickKey() == EJK_UP && m_iInputkeyDelayTime <= 0 )
#endif
        )
    {
#ifdef USE_JOYSTICK
        m_iInputkeyDelayTime = 30;
#endif
        SetCharNum_KeyDown(-4);
        m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);
    }
    else if(g_pkInput->IsDown( DIK_DOWN)
#ifdef USE_JOYSTICK
        || (g_MyD3D->GetJoystickKey() == EJK_DOWN && m_iInputkeyDelayTime <= 0 )
#endif
        )
    {
#ifdef USE_JOYSTICK
        m_iInputkeyDelayTime = 30;
#endif
        SetCharNum_KeyDown(4);
        m_mapSelectChar[m_iCharKeySelect]->SetWndMode(D3DWM_ACTIVE);
    }
    else if((g_pkInput->IsDown( DIK_RETURN) 
#ifdef USE_JOYSTICK
        || (g_MyD3D->GetJoystickKey() == EJK_KEY_4 )
#endif
        )
        && !g_pkInput->BtnDown())
    {
        m_iInputkeyDelayTime = 30;
        if ( m_iState == KGCCharacterSelectBox::EXPERT_SKILL_NOT_SAVE || 
             m_iState == KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL || 
             m_iState == KGCCharacterSelectBox::EXPERT_SKILL_PROMOTION || 
             m_iState == KGCCharacterSelectBox::EXPERT_SKILL_MISSION || 
             m_iState == KGCCharacterSelectBox::EXPERT_SKILL_DEPOT_ADD_COMMON )
        {
            std::map<int, KD3DWnd*>::iterator mIter = m_mapSelectChar.begin();
            for( ; mIter != m_mapSelectChar.end() ; mIter++)
            {
                if(mIter->first == m_iCharKeySelect )
                {
                    m_iSelectCharIdx = m_iCharKeySelect;
                    int iPromotion = g_pkUIScene->m_pkSkillTree->GetEndSkillOpenPromotion( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel );
                    m_SetFunc(m_iCharKeySelect,iPromotion);
                    m_UpdateFunc();
                    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(false);
                    return;
                }
            }
        }
        else
        {
            KCharInfoVector::iterator vIter =	g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.begin();
            for(int i= 0;vIter !=g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.end() ;vIter++){
                if(vIter->iCharType == m_iCharKeySelect){
                    if(m_iState != KGCCharacterSelectBox::EXPERT_SKILL_NOT_SAVE )
                    {
#if !defined ( __PATH__ ) 
                        MessageBoxA( NULL, "Error InGame Change Character", "Error", MB_OK );                            
                        return;
#endif 
//                         g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = static_cast<char>(i);
//                         g_kGlobalValue.m_kUserInfo.cCharIndex = static_cast<char>(i);
//                         g_kGlobalValue.SetSavedCharType(static_cast<char>(i));
                    }
                    
                    int iPromotion = g_pkUIScene->m_pkSkillTree->GetEndSkillOpenPromotion( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel );                    
                    m_SetFunc(m_iCharKeySelect,iPromotion);
                    m_UpdateFunc();
                    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(false);
                    
                    return;
                }
                i++;
            }
        }
        
    }
    else{
        if(--m_iInputkeyDelayTime <0) m_iInputkeyDelayTime =0;
    }

    if( 6 == m_iState )
    {
        m_iAlphaCnt += m_iSwitch;
        if( 255 <= m_iAlphaCnt || m_iAlphaCnt <= 0 )
        {
            m_iAlphaCnt = (255<=m_iAlphaCnt) ? 255:0;
            m_iSwitch *= -1;
        }

        for(int iLoop=0; iLoop<GC_CHAR_NUM; ++iLoop)
        {
            if( m_pkMarkMissionNew[iLoop]->IsRenderOn() )
                m_pkMarkMissionNew[iLoop]->SetAllWindowAlpha( m_iAlphaCnt );
            if( m_pkMarkMissionComplate[iLoop]->IsRenderOn() )
                m_pkMarkMissionComplate[iLoop]->SetAllWindowAlpha( m_iAlphaCnt );
        }
    }
}

void KGCCharacterHoverBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for(std::map<int,KD3DWnd*>::iterator mIt = m_mapSelectChar.begin(); mIt != m_mapSelectChar.end();mIt++)
        {
            if( event.m_pWnd != mIt->second)
                continue;

            switch( event.m_dwCode )
            {
            case KD3DWnd::D3DWE_CURSOR_ENTER:
                {
                    std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( mIt->first );
                    if( mapSealedIter != g_kGlobalValue.m_mapSealedCharInfo.end() )
                    {
                        if( mapSealedIter->second.m_bIsSealed )
                        {
                            event.m_pWnd->Lock( true );
                            continue;
                        }
                    }
                }
                break;
            case KD3DWnd::D3DWE_BUTTON_CLICK:
                {
                    if( event.m_pWnd == mIt->second)
                    {
                        std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( mIt->first );
                        if( mapSealedIter != g_kGlobalValue.m_mapSealedCharInfo.end() )
                        {
                            if( mapSealedIter->second.m_bIsSealed )
                                continue;
                        }

                        event.m_pWnd->Lock( false );
                        mIt->second->SetWndMode(D3DWM_DEFAULT);


				        if (g_kGlobalValue.ServerInfo.CheckServerType( ST_NO_AP_CHAR ) )
				        {
					        if (g_kGlobalValue.IsQuickSkillPlayer(mIt->first) )
					        {
						        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LIMITED_FUNC_IN_PVP_SERVER ),L"" );
						        continue;
					        }
				        }

				        KCharInfoVector::iterator vIter =	g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.begin();
				        for(int i= 0;vIter !=g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.end() ;vIter++){
					        if(vIter->iCharType == mIt->first){
						        if( m_iState != KGCCharacterSelectBox::EXPERT_SKILL_NOT_SAVE && 
                                    m_iState != KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL && 
                                    m_iState != KGCCharacterSelectBox::EXPERT_SKILL_PROMOTION && 
                                    m_iState != KGCCharacterSelectBox::EXPERT_SKILL_MISSION && 
                                    m_iState != KGCCharacterSelectBox::EXPERT_SKILL_DEPOT_ADD_COMMON &&
                                    m_iState != KGCCharacterSelectBox::EXPERT_SKILL_VIRTUAL_DEPOT )
                                {
#if !defined ( __PATH__ ) 
                                    MessageBoxA( NULL, "Error InGame Change Character", "Error", MB_OK );                            
                                    return;
#endif 
// 							        g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = static_cast<char>(i);
// 		                            int iCharIndex = mIt->first + GC_CHAR_NUM * g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iPromotionLevel;
//       		                    SetPlayerTemplate( g_MyD3D->m_TempPlayer.m_setSkill, iCharIndex, g_MyD3D->m_TempPlayer.GetCurFormTemplate() );                            
// 							        g_kGlobalValue.m_kUserInfo.cCharIndex = static_cast<char>(i);
// 							        g_kGlobalValue.SetSavedCharType(static_cast<char>(i));
						        }
						        break;
					        }
					        i++;
				        }
                int iPromotion = g_pkUIScene->m_pkSkillTree->GetEndSkillOpenPromotion( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel );
				        m_SetFunc(mIt->first,iPromotion);
				        m_UpdateFunc();
				        g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(false);
			        }

                }
                break;
            }
        }
    }
}

void KGCCharacterHoverBox::InitCharPrivate( int iCharType, int iCount, D3DXVECTOR4 vMovePos, int nPrivateCharState, int nPrivateCharType )
{
    std::map<int,KD3DWnd*>::iterator mapCharUIIter = m_mapSelectChar.find( iCharType );
    std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( iCharType );

    if ( mapCharUIIter == m_mapSelectChar.end() ) 
        return;

    if ( mapSealedIter == g_kGlobalValue.m_mapSealedCharInfo.end() )
        return;

    if ( !m_pkMarkSeal[ iCharType ] ) 
        return;

    bool bMarkRender = false;
    bool bLock = false;
    std::wstring strToolTipMsg;

    D3DXVECTOR2 vCharUIPos;
    D3DXVECTOR2 vEventUIPos;
    mapCharUIIter->second->GetCurrentWindowPos();
    vCharUIPos.x = ((vMovePos.x * (iCount%4)) + vMovePos.z);
    vCharUIPos.y = ((vMovePos.y * (iCount/4)) + vMovePos.w);

    strToolTipMsg = g_pkStrLoader->GetString( STR_ID_NO_CLEAR_PRIVATE_DUNGEON );

    switch( nPrivateCharState ) 
    {
    case KGCCharacterSelectBox::PRIVATE_CHAR_NONE:
        break;
    case KGCCharacterSelectBox::PRIVATE_CHAR_NORMAL:
        // 일반 모드 ( 졸업하지 않은 캐릭터는 선택할 수 없다. ) 
        {
            if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( iCharType ) )
            {   
                bMarkRender = true;
            }
        }
        break;
    case KGCCharacterSelectBox::PRIVATE_CHAR_ONLY:
        //  특정 캐릭터만 선택 가능( 캐릭터 전용 던전 )
        {
            if ( iCharType != nPrivateCharType ) 
            {
                bMarkRender = true;
            }
        }
        break;
    }

    if ( bMarkRender ) 
    {
        vEventUIPos = vCharUIPos;
        vEventUIPos.x += 3.0f;
        vEventUIPos.y += 37.0f;
        m_pkMarkSeal[iCharType]->SetWindowPos( vEventUIPos );
        m_pkMarkSeal[iCharType]->ToggleRender( true );
        m_pkMarkSeal[iCharType]->ClearToolTip();
        m_pkMarkSeal[iCharType]->SetToolTip( strToolTipMsg.c_str(), 0.0f, 0.0f, KD3DWnd::WNDUIAT_EXPANTION, KD3DWnd::ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT );


        m_pkMarkSeal[iCharType]->Lock( true );
        mapCharUIIter->second->Lock( true );
    }
}

void KGCCharacterHoverBox::InitCharSeal( int iCharType, int iCount, D3DXVECTOR4 vMovePos )
{
    std::map<int,KD3DWnd*>::iterator mapCharUIIter = m_mapSelectChar.find( iCharType );
    std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( iCharType );

    if ( mapCharUIIter == m_mapSelectChar.end() ) 
        return;

    if ( mapSealedIter == g_kGlobalValue.m_mapSealedCharInfo.end() )
        return;
    
    D3DXVECTOR2 vCharUIPos;
    D3DXVECTOR2 vEventUIPos;
    mapCharUIIter->second->GetCurrentWindowPos();
    vCharUIPos.x = ((vMovePos.x * (iCount%4)) + vMovePos.z);
    vCharUIPos.y = ((vMovePos.y * (iCount/4)) + vMovePos.w);

    // 캐릭터 봉인 이벤트 진행 중UI면
    if( mapSealedIter->second.m_bIsEventTerm )
    {
        vEventUIPos = vCharUIPos;
        if( m_pkMarkIngArrow[iCharType] )
        {
            vEventUIPos.x += 21.0f;
            vEventUIPos.y += 42.0f;
            m_pkMarkIngArrow[iCharType]->SetWindowPos( vEventUIPos );
            m_pkMarkIngArrow[iCharType]->ToggleRender( true );
            m_pkMarkIngArrow[iCharType]->SetWndMode( KD3DWnd::D3DWM_DEFAULT );

            std::map<int, MARK_ING_ARROW_ANI>::iterator mapAniInfo = m_mapArrowAniInfo.find( iCharType );
            if( mapAniInfo == m_mapArrowAniInfo.end() )
            {
                MARK_ING_ARROW_ANI kMarkIngArrowAni;
                kMarkIngArrowAni.pWnd = m_pkMarkIngArrow[iCharType];
                kMarkIngArrowAni.fAniTimeCnt = 0.0f;
                kMarkIngArrowAni.fSpeed = 0.1f;
                m_mapArrowAniInfo.insert( std::make_pair( iCharType, kMarkIngArrowAni) );
            }
            else
                mapAniInfo->second.fAniTimeCnt = 0.0f;
        }
    }

    if ( !m_pkMarkSeal[ iCharType ] ) 
        return;

    bool bMarkRender = false;
    bool bLock = false;
    std::wstring strToolTipMsg;


    // 캐릭터 봉인중.
    if ( mapSealedIter->second.m_bIsSealed ) 
    {
        bMarkRender = true;
        bLock = true;

        strToolTipMsg = g_pkStrLoader->GetString( STR_ID_CHAR_SEAL_MSG_1 );
    }
    // 캐릭터 봉인 이벤트 중.
    else if( mapSealedIter->second.m_bIsEventTerm )
    {
        // 이미 다른곳에 봉인되어있다.
        if ( !m_pkMarkSeal[iCharType]->IsLocked() )
        {
            bMarkRender = true;
            bLock = false;

            tm *pTime = localtime( &mapSealedIter->second.m_tmObjectTime );
            int iMonth = pTime->tm_mon+1, iDay = pTime->tm_mday;            

            strToolTipMsg = g_pkStrLoader->GetReplacedString( STR_ID_CHAR_SEAL_MSG_2, "liii", GCFUNC::GetCharName( iCharType ), 
                                                              iMonth, iDay, mapSealedIter->second.m_nObjectLevel );
        }
    }

    if ( bMarkRender ) 
    {
        vEventUIPos = vCharUIPos;
        vEventUIPos.x += 3.0f;
        vEventUIPos.y += 37.0f;
        m_pkMarkSeal[iCharType]->SetWindowPos( vEventUIPos );
        m_pkMarkSeal[iCharType]->ToggleRender( true );
        m_pkMarkSeal[iCharType]->ClearToolTip();
        m_pkMarkSeal[iCharType]->SetToolTip( strToolTipMsg.c_str(), 0.0f, 0.0f, KD3DWnd::WNDUIAT_EXPANTION, KD3DWnd::ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT );


        m_pkMarkSeal[iCharType]->Lock( bLock );
        mapCharUIIter->second->Lock( bLock );
    }
}

void KGCCharacterHoverBox::MarkArrowAnimation()
{
    std::map<int, MARK_ING_ARROW_ANI>::iterator mapAniIter = m_mapArrowAniInfo.begin();
    for(; mapAniIter != m_mapArrowAniInfo.end(); ++mapAniIter)
    {
        if( NULL == mapAniIter->second.pWnd )
            continue;

        if( false == mapAniIter->second.pWnd->IsRenderOn() )
            continue;

        std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( mapAniIter->first );
        if( mapSealedIter == g_kGlobalValue.m_mapSealedCharInfo.end() )
            continue;
        
        if( false == mapSealedIter->second.m_bIsEventTerm )
        {
            mapAniIter->second.pWnd->ToggleRender( false );
            continue;
        }

        mapAniIter->second.fAniTimeCnt += mapAniIter->second.fSpeed;

        if( mapAniIter->second.fAniTimeCnt > static_cast<float>(SHRT_MAX) )
            mapAniIter->second.fAniTimeCnt = 0.0f;

        mapAniIter->second.pWnd->SetWndMode( static_cast<ED3DWndMode>( static_cast<int>(mapAniIter->second.fAniTimeCnt)%4 ) );
    }
}