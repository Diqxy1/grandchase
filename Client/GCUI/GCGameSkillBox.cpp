#include "stdafx.h"
#include "GCGameSkillBox.h"
//



//
//
#include "MyD3D.h"
#include "Controls.h"
#include "GCSkill.h"
IMPLEMENT_CLASSNAME( KGCGameSkillBox );
IMPLEMENT_WND_FACTORY( KGCGameSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGameSkillBox, "gc_game_skill_box" );

#define SKILL_IMAGE_OFFSET_Y 25

KGCGameSkillBox::KGCGameSkillBox( void )
: m_bSkillOnly( false )
, m_pkComboSelector( NULL )
, m_pkFrameLockSP4( NULL )
{
    char szTemp[MAX_PATH];
    for ( int iLoop = 0; iLoop < 6; iLoop++ )
	{
		sprintf( szTemp, "slot%d", iLoop );		    LINK_CONTROL( szTemp, m_pkSkillPos[iLoop] );
		sprintf( szTemp, "text%d", iLoop );	    	LINK_CONTROL( szTemp, m_pkText[iLoop] );
		sprintf( szTemp, "coolTime%d", iLoop );		LINK_CONTROL( szTemp, m_pkTextCoolTime[iLoop] );
	}

	for ( int iLoop = 0; iLoop < 5; iLoop++ )
	{
		sprintf( szTemp, "frame%d", iLoop );  	    	LINK_CONTROL( szTemp, m_pkHotKeyFrame[iLoop] );
		sprintf( szTemp, "hotkey%d", iLoop ); 	    	LINK_CONTROL( szTemp, m_pkHotKey[iLoop] );
	}

    LINK_CONTROL( "comboselector",  m_pkComboSelector );

    LINK_CONTROL( "frame_lock_sp4", m_pkFrameLockSP4 );

    m_mapComboFrame.clear();

    m_vOriginalPos = D3DXVECTOR2( 0.f, 0.f );
}

KGCGameSkillBox::~KGCGameSkillBox( void )
{

}

void KGCGameSkillBox::OnCreate( void )
{
    if( false == LoadSkillIcon() )
        return;

    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
    {
        return;
    }

	for ( int iLoop = 0; iLoop < 5; iLoop++ )
	{
		m_pkHotKeyFrame[iLoop]->SetPredraw( true );
	}

    if( m_pkComboSelector )
        m_pkComboSelector->InitState( false );    
    
    if( m_vOriginalPos == D3DXVECTOR2( 0.f, 0.f ) ) {
        m_vOriginalPos = this->GetFixedWindowLocalPos();
    }
}

void KGCGameSkillBox::OnInitialize()
{	
	ToggleRender( true );
	for ( int iLoop = 0 ; iLoop < 5 ; iLoop++ )
	{
		m_pkHotKey[iLoop]->ToggleRender( false );
	}

    std::map< int, std::vector<KD3DWnd*> >::iterator mapIter = m_mapComboFrame.begin();
    for(; mapIter != m_mapComboFrame.end(); ++mapIter)
    {
        std::vector<KD3DWnd*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter)
        {
            if( *vecIter )
                (*vecIter)->ToggleRender( false );
        }
    }

}

void KGCGameSkillBox::OnDestroy( void )
{
	SAFE_RELEASE( m_pItemImageHealingBall );

	for(int iLoop = 0; iLoop < MID_TOTAL; iLoop++)
	{
		SAFE_RELEASE(m_mapSkillTexture[iLoop]);
	}
}

void KGCGameSkillBox::FrameMoveInEnabledState( void )
{	
}

void KGCGameSkillBox::PostDraw( void )
{
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
    {
        return;
    }
    
	int iMyPlayerIdx = g_MyD3D->Get_MyPlayer();
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( g_kGlobalValue.IsTutorialMode() ) {
        iMyPlayerIdx = 0;
        pPlayer = g_MyD3D->MyPlayer[0];
    }

    if( pPlayer == NULL ) {
        return;
    }

    if( m_pkComboSelector )
        m_pkComboSelector->ToggleRender( false );

    CalcComboAlpha();

    int iSkillMotionID = 0;
    int iSkillDuration = 0;
    int iStoringMP = int(g_MyD3D->MyCtrl->Energy);
	for ( int iLoop = 0; iLoop < 5; ++iLoop )
	{
		int iScaleDelta= 0;
		bool bRenderCoolTime = false;
		bool IsEnableSkill = m_pkHotKeyFrame[iLoop]->IsRenderOn();
		iSkillMotionID = pPlayer->GetSpecialMotionID(iLoop + 1, &iSkillDuration);
		
        // UI Toggle 처리
        m_pkHotKey[iLoop]->ToggleRender( false );
		m_pkHotKeyFrame[iLoop]->ToggleRender( false );
        std::map< int, std::vector<KD3DWnd*> >::iterator mapWndIter = m_mapComboFrame.find( iLoop );
        if( mapWndIter != m_mapComboFrame.end() )
        {
            std::vector<KD3DWnd*>::iterator vecWndIter = mapWndIter->second.begin();
            for(; vecWndIter != mapWndIter->second.end(); ++vecWndIter)
            {
                if( (*vecWndIter) )
                {
                    (*vecWndIter)->ToggleRender( false );
                    (*vecWndIter)->SetPredraw( true );
                }
            }
        }

		//연습모드에 수량 안나오도록 수정
		if ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
		{
			iSkillDuration=-1;
		}

		if( iSkillDuration == -1 )
			m_pkText[iLoop]->SetText(L"");
		else
			m_pkText[iLoop]->SetText(iSkillDuration);

        // 핫키스킬 스킬아이콘 처리
        GCHotKeySkillInfo* info = NULL;
        std::vector<GCDeviceTexture*> vecTexture;
		if( iSkillMotionID <= 0 || g_kGlobalValue.IsCharUseSpecial4( g_MyD3D->MyPlayer[iMyPlayerIdx]->GetCurrentChar().iCharType ) ) {
            if( g_kGlobalValue.IsCharUseSpecial4( g_MyD3D->MyPlayer[iMyPlayerIdx]->GetCurrentChar().iCharType ) &&
                g_MyD3D->MyPlayer[iMyPlayerIdx]->MetamorphosisForm == FORM_NORMAL ) {
                if( iSkillDuration == -1 || iLoop >= 3 ) {
                    m_pkText[iLoop]->SetText(L"");
                } else {
                    m_pkText[iLoop]->SetText(iSkillDuration);
                }
            } else {
			    m_pkText[iLoop]->SetText(L"");
            }

			if( (int) pPlayer->m_vecHotKeySkill.size() > iLoop )
			{
                m_pkHotKeyFrame[iLoop]->ToggleRender( true );
                const GCSkill* pSkill = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, 
                    pPlayer->m_vecHotKeySkill[iLoop].m_eSkill );

                // 출력할 스킬아이콘 셋팅
                if ( pPlayer->m_bIsBurnningMode ) 
                    vecTexture.push_back( pPlayer->m_vecHotKeySkill[iLoop].pAddMotionTexture );
                else
                    vecTexture.push_back( pPlayer->m_vecHotKeySkill[iLoop].pTexture );

                // 핫키스킬에서 콤보스킬이 있는지 확인
                info = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, pPlayer->m_vecHotKeySkill[iLoop].m_eSkill )->m_pHotKeySkill;

                if( false == info->m_vecComboSkill.empty() )
                {
                    // 현재 콤보가 콤보스킬스킬이 최초발동이 아닌지 확인
                    pSkill = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, 
                        EGCSkillTree(info->m_vecComboSkill[0].m_iSID) );

                    int iNowComboIdx = pPlayer->GetCurrentComboIdx();
                    if( 0 <= iNowComboIdx && iNowComboIdx < int(info->m_vecComboSkill.size()) )
                    {
                        // 현재 모션과 콤보스킬모션이 같은지 확인
                        GCHotKeySkillInfo *pComboInfo = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, 
                            EGCSkillTree(info->m_vecComboSkill[ iNowComboIdx ].m_iSID) )->m_pHotKeySkill;

                        if( pComboInfo && pPlayer->uiMotion == pComboInfo->m_iMotion )
                        {
                            // 다음 콤보스킬이 있는지 확인
                            if( (iNowComboIdx+1) < int(info->m_vecComboSkill.size()) )
                            {
                                pSkill = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, 
                                    EGCSkillTree(info->m_vecComboSkill[ iNowComboIdx+1 ].m_iSID) );
                            }

                            std::vector< GCComboSkill >::iterator vecComboIter = info->m_vecComboSkill.begin();
                            for(; vecComboIter != info->m_vecComboSkill.end(); ++vecComboIter)
                            {
                                vecTexture.push_back( vecComboIter->m_pTexture );
                            }

                            D3DXVECTOR2 vPos = m_pkHotKeyFrame[iLoop]->GetFixedWindowLocalPos();
                            std::map< int, std::vector<KD3DWnd*> >::iterator mapWndIter = m_mapComboFrame.find( iLoop );
                            if( mapWndIter != m_mapComboFrame.end() )
                            {
                                // 스킬 백그라운드 프레임 출력
                                std::vector<KD3DWnd*>::iterator vecWndIter = mapWndIter->second.begin();
                                for(; vecWndIter != mapWndIter->second.end(); ++vecWndIter)
                                {
                                    if( (*vecWndIter) )
                                    {
                                        vPos.y += 50.0f;
                                        (*vecWndIter)->SetWindowPos( vPos );
                                        (*vecWndIter)->Activate( false );
                                        (*vecWndIter)->ToggleRender( true );
                                    }
                                }

                                // 현재 스킬위치에 셀렉터 출력
                                if( m_pkComboSelector && 
                                    (pSkill->m_pHotKeySkill->m_fMana < g_MyD3D->MyHead->Level) )    // 마나가 충분한지 확인
                                {
                                    vPos = mapWndIter->second[iNowComboIdx]->GetFixedWindowLocalPos();    
                                    vPos.x -= 2.0f;     vPos.y -= 2.0f;
                                    m_pkComboSelector->SetWindowPos( vPos );
                                    m_pkComboSelector->SetAllWindowColor( D3DCOLOR_RGBA( 255, 255, 255, m_iComboSelectorAlpha ) );
                                    m_pkComboSelector->ToggleRender( true );
                                    mapWndIter->second[iNowComboIdx]->Activate( true );
                                }
                            }
                        }
                    }
                }
                // 콤보스킬 처리 끝


				iScaleDelta = -3;

                float fMana = pSkill->m_pHotKeySkill->m_fMana;
#if defined( MATCH_BALANCE_2011_8 )
                if( SiKGCRoomManager()->IsMonsterGameMode() )
                {
                    if( 0.0f < pSkill->m_pHotKeySkill->m_fMonsterModeMana )
                        fMana = pSkill->m_pHotKeySkill->m_fMonsterModeMana;
                }
#endif
				if ( fMana <= g_MyD3D->MyHead->Level )
				{
					iStoringMP = 0;
					m_pkHotKeyFrame[iLoop]->Activate( true );
					m_pkHotKeyFrame[iLoop]->ToggleRender( true );
					m_pkHotKeyFrame[iLoop]->SetWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff) );					
					m_pkHotKey[iLoop]->ToggleRender( true );
				}
				else if ( fMana > 10.0f )
				{
					iStoringMP = 0;
					m_pkHotKeyFrame[iLoop]->Activate( false );
					m_pkHotKeyFrame[iLoop]->SetWindowColor( D3DCOLOR_ARGB( 80, 0xff, 0xff, 0xff) );					
				}
				else
				{
					iStoringMP = 0;
					m_pkHotKeyFrame[iLoop]->Activate( false );
				}			

                if( pPlayer->m_vecHotKeySkill[iLoop].fCoolTime > 0.0f )
				{
					std::wstringstream wstm;
					wstm << (int)pPlayer->m_vecHotKeySkill[iLoop].fCoolTime;
					bRenderCoolTime = true;
					iStoringMP = 0;
				}

				if ( m_bSkillOnly )
				{
					bool bToggle = m_pkHotKeyFrame[iLoop]->IsActive() && !bRenderCoolTime;

					if ( bToggle == true && IsEnableSkill == false )
					{
						const KSafeArray<KD3DUIVertex,4>& pVertex = m_pkSkillPos[iLoop]->GetReLocatedWindowRect();
						float fX = ( pVertex[0].m_vPos.x + pVertex[3].m_vPos.x ) / 2.0f;
						float fY = ( pVertex[0].m_vPos.y + pVertex[3].m_vPos.y ) / 2.0f;
						//g_ParticleManager->CreateSequence( "Skii_Button_Effect", fX, fY );
					}

					m_pkHotKeyFrame[iLoop]->ToggleRender( bToggle );
					m_pkHotKey[iLoop]->ToggleRender( bToggle );

					if ( !m_pkHotKeyFrame[iLoop]->IsRenderOn() )
					{
						continue;
					}
				}
			}
			else
			{
				if( pPlayer->MetamorphosisForm == FORM_NORMAL )
				{
					assert( iSkillMotionID != -1 );
					g_kGlobalValue.m_bSkillMotionIDErrorOccur = true;
				}
				continue;
			}
		}
		else
		{			
			vecTexture.push_back( m_mapSkillTexture[iSkillMotionID] );
		}
		
        {
            D3DCOLOR color = D3DCOLOR_ARGB(255, 200, 200, 200);

            if ( iStoringMP == iLoop + 1 )
            {
                iScaleDelta = 4;
                color = 0xffffffff;
            }

            int iOffSetY = int( m_fWindowScaleY );
            int iNowComboSkill = pPlayer->GetCurrentComboIdx();
            std::vector<GCDeviceTexture*>::iterator vecTexIter = vecTexture.begin();
            for(int iComboLoop=-1; vecTexIter != vecTexture.end(); ++vecTexIter, ++iComboLoop )
            {
                if( (*vecTexIter) )
                {
                    if( info && 
                        0 <= iNowComboSkill && iNowComboSkill < int(info->m_vecComboSkill.size()) )
                    {
                        // 콤보가 있을때 현재 사용중인 콤보아이콘을 밝게
                        GCHotKeySkillInfo *pComboInfo = g_kGlobalValue.GetSkill( pPlayer->GetCurrentChar().iCharType, 
                            EGCSkillTree(info->m_vecComboSkill[ iNowComboSkill ].m_iSID) )->m_pHotKeySkill;

                        if( iComboLoop == iNowComboSkill &&
                            pComboInfo && pPlayer->uiMotion == pComboInfo->m_iMotion &&
                            (*vecTexIter) == info->m_vecComboSkill[ iNowComboSkill ].m_pTexture )
                            color = D3DCOLOR_ARGB(255, 255, 255, 255);
                        else
                            color = D3DCOLOR_ARGB(255, 200, 200, 200);
                    }

                    // 스킬아이콘 출력
                    const KSafeArray<KD3DUIVertex,4>& pVertex = m_pkSkillPos[iLoop]->GetReLocatedWindowRect();
                    g_pGCDeviceManager2->DrawInScreen((*vecTexIter),
                        (pVertex[0].m_vPos.x - iScaleDelta),
                        (pVertex[0].m_vPos.y - iScaleDelta + iOffSetY),
                        (pVertex[3].m_vPos.x + iScaleDelta),
                        (pVertex[3].m_vPos.y + iScaleDelta + iOffSetY),
                        0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, color );
                    iOffSetY += int(50 * m_fWindowScaleY);
                }
            }

            if ( bRenderCoolTime )
            {
                g_pkUIMgr->RenderNumberScale( "combo_num_normal", D3DXVECTOR2(m_pkTextCoolTime[iLoop]->GetRelocatedWindowPos().x,m_pkTextCoolTime[iLoop]->GetRelocatedWindowPos().y) ,
                    (DWORD)pPlayer->m_vecHotKeySkill[iLoop].fCoolTime, 0.5f, 0xffffffff, true, true, false );
            }
        }
    }

    if ( SiKGCRoomManager()->IsNot4Special() )
    {
        m_pkHotKey[3]->ToggleRender( false );
    }

	int iHealingBallIndex = 5;

	if ( pPlayer->m_vecHotKeySkill.empty() )
	{
		m_pkText[iHealingBallIndex]->ToggleRender( false );
		iHealingBallIndex = 3;
	}

	m_pkText[iHealingBallIndex]->ToggleRender( false );
	if ( m_bSkillOnly )
	{		
		return;
	}

	int iItemID;
	int iItemDuration;

	
	const KSafeArray<KD3DUIVertex,4>& pVertex = m_pkSkillPos[iHealingBallIndex]->GetWindowRect();
		
	pPlayer->GetHealingBallInfo(iItemID,iItemDuration);

	if( iItemID == -1)
	{
		m_pkText[iHealingBallIndex]->SetText(L"");
		return;
	}

	m_pkText[iHealingBallIndex]->SetText(iItemDuration);
	m_pkText[iHealingBallIndex]->ToggleRender( true );

	g_pGCDeviceManager2->DrawInScreen(m_pItemImageHealingBall,
		pVertex[0].m_vPos.x * m_fWindowScaleX,
		pVertex[0].m_vPos.y * m_fWindowScaleY,
		pVertex[3].m_vPos.x * m_fWindowScaleX,
		pVertex[3].m_vPos.y * m_fWindowScaleY,
		0.0f, 0.0f, 1.0f, 1.0f);
}

void KGCGameSkillBox::SetSkillOnly()
{
	m_bSkillOnly = true;
}

void KGCGameSkillBox::ResetComboSkill()
{
    // 이전에 로드했던 UI제거
    std::map< int, std::vector<KD3DWnd*> >::iterator mapIter = m_mapComboFrame.begin();
    for(; mapIter != m_mapComboFrame.end(); ++mapIter)
    {
        std::vector<KD3DWnd*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter)
        {
            if( (*vecIter) )
            {
                (*vecIter)->ToggleRender( false );
                (*vecIter)->RemoveAllChild();
            }
        }
        mapIter->second.clear();
    }
    m_mapComboFrame.clear();

    // 모든 핫키슬롯에 장착된 스킬의 콤보스킬수만큼 백그라운드 프레임 생성
    char szWinName[128];

    int iMe = g_MyD3D->Get_MyPlayer();
    if( g_kGlobalValue.IsTutorialMode() ) {
        iMe = 0;
    }

    if( g_MyD3D->IsPlayerIndex( iMe ) )
    {
        for( int iLoopX=0; iLoopX < 5; ++iLoopX )
        {
            if( iLoopX < int(g_MyD3D->MyPlayer[iMe]->m_vecHotKeySkill.size()) )
            {
                GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill( g_MyD3D->MyPlayer[iMe]->GetCurrentChar().iCharType, 
                    g_MyD3D->MyPlayer[iMe]->m_vecHotKeySkill[iLoopX].m_eSkill )->m_pHotKeySkill;

                if( (false == info->m_vecComboSkill.empty()) )
                {
                    std::vector<KD3DWnd*> vecWnd;
                    for(int iLoopY=0; iLoopY < int(info->m_vecComboSkill.size()); ++iLoopY)
                    {
                        sprintf(szWinName, "skillbox%d_%d", iLoopX, iLoopY);
                        KD3DWnd* pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", szWinName, "boxes\\skillbox" );
                        if( pWnd )
                        {
                            pWnd->InitState( false );
                            pWnd->SetOrder_Bottom();
                            vecWnd.push_back( pWnd );
                        }
                    }

                    if( false == vecWnd.empty() )
                        m_mapComboFrame.insert( std::make_pair(iLoopX, vecWnd) );
                }
            }
        }
    }

    m_iComboSelectorAlpha = 0;
    m_bComboSelectorAlpha = false;

    if( m_vOriginalPos == D3DXVECTOR2( 0.f, 0.f ) ) {
        m_vOriginalPos = this->GetFixedWindowLocalPos();
    }    

    
    if( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() ) {
        this->SetFixedWindowLocalPos( D3DXVECTOR2( m_vOriginalPos.x, m_vOriginalPos.y + SKILL_IMAGE_OFFSET_Y ) );
        m_iOffsetY = 0;
    } 
    else{

        if( g_MyD3D->GetMyPlayer()->GetMyCharString() )
        g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->SetChar( g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType );
        // 버닝게이지가 보이면 스킬이미지UI 를 밑으로 내리는거임
        if( g_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->m_pkSpGauge->IsRenderOn() ) {
            m_iOffsetY = SKILL_IMAGE_OFFSET_Y;
        } else {
            m_iOffsetY = 0;
        }
        this->SetFixedWindowLocalPos( D3DXVECTOR2( m_vOriginalPos.x, m_vOriginalPos.y + m_iOffsetY ) );
    }

    if( m_pkFrameLockSP4 ) {
        int iSkillID = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SID_SPECIAL4;
        int iSkillID2 = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SID_SPECIAL4_1;
        if( (g_kGlobalValue.IsQuickSkillPlayer( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType ) == false &&
            g_kGlobalValue.IsHybridPlayer( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType ) == false && 
            SiGCSKT()->IsLearnedSkill( static_cast< EGCSkillTree >( iSkillID ) ) == false && 
			SiGCSKT()->IsLearnedSkill( static_cast< EGCSkillTree >( iSkillID2 ) ) == false ) || 
			SiKGCRoomManager()->IsNot4Special() ) {
                m_pkFrameLockSP4->ToggleRender( true );
        } else {
            m_pkFrameLockSP4->ToggleRender( false );
        }
    }    
}

bool KGCGameSkillBox::LoadSkillIcon( void )
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "MotionEnum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr , "SameSkillImage.stg" ) == false )
        return false;

    std::map<int, int> mapSameMotionID;
    mapSameMotionID.clear();

    LUA_BEGIN_TABLE( "SameSkillImage", return false)
    {
        for( int i = 1 ; ; ++i )
        {
            int iOriginalMotion = -1;
            int iSameMotion = -1;            
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iSameMotion );
                luaMgr.GetValue( 2, iOriginalMotion );
            }
            LUA_END_TABLE( break );
            mapSameMotionID[ iSameMotion ] = iOriginalMotion;            
        }
    }

    std::map<int, int>::iterator mit;

    for(int i = 0; i < 6; i++)
    {
        m_pkText[i]->SetShadow( true );		
    }

    char strTemp[MAX_PATH];
    for(int i = 0; i < g_pGraphicsHelper->GetPlayerMotionNum(); i++)
    {
        mit = mapSameMotionID.find(i);

        if( mit != mapSameMotionID.end() )
            sprintf(strTemp,"SkillImage%d.dds", mit->second);
        else
            sprintf(strTemp,"SkillImage%d.dds",i);

        if(g_pGCDeviceManager->GetMassFileManager()->IsIn(strTemp))
            m_mapSkillTexture[i] = g_pGCDeviceManager2->CreateTextureWithNoLoad( strTemp );
        else
            m_mapSkillTexture[i] = NULL;
    }

    m_pItemImageHealingBall = g_pItemMgr->CreateItemTexture( ITEM_ID_HEALBALL, 0 );

    return true;
}

void KGCGameSkillBox::CalcComboAlpha()
{
    if( m_bComboSelectorAlpha )
    {
        m_iComboSelectorAlpha -= 10;
        if( 0 > m_iComboSelectorAlpha ) 
        {
            m_iComboSelectorAlpha = 0;
            m_bComboSelectorAlpha = false;
        }
    }
    else
    {
        m_iComboSelectorAlpha += 10;
        if( 255 < m_iComboSelectorAlpha )
        {
            m_iComboSelectorAlpha = 255;
            m_bComboSelectorAlpha = true;
        }
    }
}