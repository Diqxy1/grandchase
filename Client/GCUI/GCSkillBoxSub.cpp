#include "stdafx.h"
//
#include ".\gcskillboxsub.h"
#include "GCSKT.h"
#include "gcskill.h"
//
//
//



IMPLEMENT_CLASSNAME( KGCSkillBoxSub );
IMPLEMENT_WND_FACTORY( KGCSkillBoxSub );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillBoxSub, "gc_skillbox_sub" );

EGCSkillTree KGCSkillBoxSub::RequestLearnSkill = (EGCSkillTree)0;
DWORD	KGCSkillBoxSub::m_dwUntrainItemID = 0;

KGCSkillBoxSub::KGCSkillBoxSub(void)
: alpha( 0xFF)
, bIncrease( false )
, m_pkActive( NULL )
, m_pkNormal( NULL )
, m_pkLocked( NULL )
, m_pkPlus( NULL )
, m_pkMinus( NULL )
, m_pkAlert( NULL )
, m_pkLock( NULL )
, m_pkOpen( NULL )
, m_eSkillID( (EGCSkillTree)0 )
{
	LINK_CONTROL( "alert",	m_pkAlert );
	LINK_CONTROL( "normal", m_pkNormal );
	LINK_CONTROL( "active", m_pkActive );
	LINK_CONTROL( "locked", m_pkLocked );

	LINK_CONTROL( "btn_plus", m_pkPlus );
	LINK_CONTROL( "btn_minus", m_pkMinus );
	LINK_CONTROL( "check",		m_pkCheck );
    LINK_CONTROL( "lock", m_pkLock );
    LINK_CONTROL( "open", m_pkOpen );
	
}

KGCSkillBoxSub::~KGCSkillBoxSub(void)
{
}

void KGCSkillBoxSub::OnCreate()
{
	m_pkPlus->InitState( true, true, this );
	m_pkMinus->InitState( true, true, this );
	m_pkCheck->InitState( false, true, this );
	m_pkAlert->InitState( false );

#if defined(NATION_LATINAMERICA)
    m_pkOpen->InitState( false );
#else
    if ( m_pkOpen )
        m_pkOpen->InitState( false, true, this );
#endif
    if( m_pkLock ) {
        m_pkLock->InitState( false );
    }    

#if !defined(NATION_KOREA)
	m_pkPlus->SetToolTip( g_pkStrLoader->GetString(STR_ID_SKILLTREE3) );
	m_pkMinus->SetToolTip( g_pkStrLoader->GetString(STR_ID_SKILLTREE4) );
#endif

	m_apkLine.clear();
	for( int i = 0 ; true ; ++i )
	{
		char strTemp[MAX_PATH];
		sprintf(strTemp,"line%d",i + 1);

		if ( GetChildByName( strTemp ) )
		{
			m_apkLine.push_back( GetChildByName( strTemp ) );
		}
		else
		{
			break;
		}
		
	}


	for( unsigned int i = 0 ; i < m_apkLine.size() ; ++i )
	{
		m_apkLine[i]->SetPredraw( true );		
	}
}

void KGCSkillBoxSub::PostChildDraw()
{
	m_kSkillImage.Render();
}

void KGCSkillBoxSub::SetSkill( EGCSkillTree eSkillID )
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( eSkillID );

	if( pSkill == NULL )
		return;

	m_eLearnableSkillID = m_eSkillID = eSkillID;	

	m_kSkillImage.RemoveAllTexture();

	m_kSkillImage.CameraMatrixOn();
	float vPosX = ( this->GetFixedWindowPos().x + 8.0f ) / (400.0f * GC_SCREEN_DIV_WIDTH);
	float vPosY = -( this->GetFixedWindowPos().y - 36.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f ;

	m_kSkillImage.SetPosition( vPosX, vPosY );
	m_kSkillImage.SetScale( 0.1f*GC_SCREEN_OLD_DIV_WIDTH, 0.1f*GC_SCREEN_OLD_DIV_WIDTH );
	//  m_kSkillImage.AddTexture( "asdfasdf" );
	m_kSkillImage.AddTexture( pSkill->GetTextureFileName().c_str() );

	m_pkAlert->ToggleRender( pSkill->isNew() && pSkill->GetLv() <= g_kGlobalValue.m_kUserInfo.GetCurrentChar( pSkill->GetCharType() ).iLevel );

	SetBoxState();
}

void KGCSkillBoxSub::SetLineType( std::set<int> iType )
{
	for( unsigned int i = 0 ; i < m_apkLine.size() ; ++i )
	{
		m_apkLine[i]->ToggleRender( iType.find( i ) != iType.end()  );
	}
}

void KGCSkillBoxSub::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkPlus,	KD3DWnd::D3DWE_BUTTON_CLICK, OnPlus		);
	GCWND_MSG_MAP( m_pkMinus,	KD3DWnd::D3DWE_BUTTON_CLICK, OnMinus	);

    if ( m_pkOpen )
	    GCWND_MSG_MAP( m_pkOpen,	KD3DWnd::D3DWE_BUTTON_CLICK, OnOpen 	);

	if( m_pkCheck == event.m_pWnd )
	{
		KActionEvent event2 = event;
		event2.m_pWnd = this;		
		SpeakToActionListener( event2 );
	}
}

void KGCSkillBoxSub::OnOpen()
{
#if defined( __OPEN_TEST__ )
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY1 ),
        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY2 ) );
    return;
#endif

    const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eLearnableSkillID );

    //     if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
    //     {
    //         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_GROUP_OPEN_ERROR3 ),
    //             L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    //         return;
    //     }

    if ( !SiGCSKT()->IsSkillTrainable( m_eSkillID ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_TRAIN_CONDITION_ERROR ), 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    // 인벤토리를 디분다 있으면
    if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_OPEN_ULTIMATE_SKILL ) == true )
    {
//         if ( m_pkOpen )
//             m_pkOpen->Lock( true );
        
        RequestLearnSkill = m_eLearnableSkillID;
        KP2P::GetInstance()->Send_SkillOpenUseReq( KGCSkillBoxSub::GetRequestedSkill() );
    }
    else // 없으면 산다
    {

#ifdef GAME_CASH
    #ifdef USE_VIP_POINT
            g_pItemMgr->BuyCashItemS6( ITEM_ID_OPEN_ULTIMATE_SKILL );
    #else
            g_pkUIScene->EnableBuyCashItemWnd( ITEM_ID_OPEN_ULTIMATE_SKILL, false, true );
    #endif
#else    
        g_pItemMgr->BuyItem( ITEM_ID_OPEN_ULTIMATE_SKILL, 1, false );
#endif
    }

    return;
}

void KGCSkillBoxSub::OnPlus()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eLearnableSkillID );

	if ( SiGCSKT()->IsUnlockAll() )
	{
		SiGCSKT()->_D_TrainSkill( pSkill->GetCharType(), pSkill->GetPromotion(), m_eLearnableSkillID );
		SiGCSKT()->_D_Equip_Skill( pSkill->GetCharType(), pSkill->GetPromotion(), SiGCSKT()->GetSetting( pSkill->GetCharType(), pSkill->GetPromotion() ),m_eLearnableSkillID );
		g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
		return;
	}

    assert( pSkill != NULL );

    int iMaxSP = SiGCSKT()->GetMaxSPinPromotion( (int)pSkill->GetCharType(), pSkill->GetPromotion() );
    int iUsedSP = SiGCSKT()->GetUsedSPinPromotion( (int)pSkill->GetCharType(), pSkill->GetPromotion() );

	if( SiGCSKT()->GetExtraSP( (int)pSkill->GetCharType() ) < pSkill->GetLearnSP() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOMORE_SKILL_POINT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}
    else if( pSkill->GetLearnSP() > 0  && iMaxSP > iUsedSP && iMaxSP < iUsedSP + pSkill->GetLearnSP() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
	else if( pSkill->GetLearnSP() > 0  && iMaxSP <= iUsedSP )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}    
    else
    {
#if defined( NO_MSG_BOX_SKILL_LEARN_AND_REMOVE )
        m_pkPlus->Lock( true );
        RequestLearnSkill = m_eLearnableSkillID;
        KP2P::GetInstance()->Send_SkillTrainingReq( KGCSkillBoxSub::GetRequestedSkill() );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE1, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_LEARN_SKILL, 0, 0, false, true );
        RequestLearnSkill = m_eLearnableSkillID;
#endif
    }
}

void KGCSkillBoxSub::OnMinus()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );

	if ( SiGCSKT()->IsUnlockAll() )
	{
		SiGCSKT()->_D_UnEquip_Skill( pSkill->GetCharType(), pSkill->GetPromotion(), SiGCSKT()->GetSetting( pSkill->GetCharType(), pSkill->GetPromotion() ),m_eLearnableSkillID );
		SiGCSKT()->_D_UnTrainSkill( pSkill->GetCharType(), pSkill->GetPromotion(), m_eSkillID );
		g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
		return;
	}
	
	std::pair<bool,DWORD> UntrainItem = SiGCSKT()->IsExistSkillUntrainItem();

	m_dwUntrainItemID = UntrainItem.second;
    if( false == UntrainItem.first )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
    else
    {
        const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );
        assert( pSkill != NULL );

        const SkillSet& setNextID = pSkill->GetNextSet();
        for( std::set< EGCSkillTree >::const_iterator sit = setNextID.begin(); sit != setNextID.end(); ++sit ) {
            const GCSkill* pNextSkill = SiGCSKT()->GetSkill( *sit );
            if( pNextSkill && pNextSkill->m_bOrCheckPreList && SiGCSKT()->IsLearnedSkill( *sit ) ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                return;
            }
        }

#if defined( NO_MSG_BOX_SKILL_LEARN_AND_REMOVE )
        m_pkMinus->Lock( true );
	    RequestLearnSkill = m_eSkillID;
        KP2P::GetInstance()->Send_RemoveSkill( KGCSkillBoxSub::GetRequestedSkill(), KGCSkillBoxSub::GetUntrainItemID() );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE15, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_REMOVE_SKILL, 0, 0, false, true );
        RequestLearnSkill = m_eSkillID;
#endif
    }
}

void KGCSkillBoxSub::SetBoxState()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );

#if defined( LOCK_ASIN_HIDDEN_SKILL )
    m_pkLock->ToggleRender( false );
    m_kSkillImage.SetAlpha( 255 );    
#endif

	if( SiGCSKT()->IsLearnedSkill( m_eSkillID ) == true )
	{
		m_pkLocked->ToggleRender( false );
		m_pkNormal->ToggleRender( true );
		m_pkActive->ToggleRender( false );

		m_pkPlus->Lock( true );
		m_pkMinus->Lock( false );
		m_kSkillImage.SetRGB( 0xFF, 0xFF, 0xFF );
		ToggleRender( true );
		
		const SkillSet set = pSkill->GetNextSet();

		for ( SkillSet::const_iterator sit = set.begin() ; sit != set.end(); sit++ )
		{
            if (SiGCSKT() == NULL)
                continue;

			if ( SiGCSKT()->GetSkill(*sit)->m_bAccSkill )
			{
				if ( SiGCSKT()->IsSkillTrainable( *sit ) )
				{
					m_pkPlus->Lock( false );				
					m_eLearnableSkillID = *sit;
					break;
				}
				else if( SiGCSKT()->IsLearnedSkill( *sit ) )
				{
					ToggleRender( false );
					break;
				}
			}
		}
	}
	else
	{
        bool bLickSkill = false;
        const SkillSet setLimitList = pSkill->GetLimitSet();
        for( SkillSet::const_iterator sit = setLimitList.begin(); sit != setLimitList.end(); ++sit ) {
            if ( SiGCSKT()->IsLearnedSkill( *sit ) ) {
                bLickSkill = true;
                break;
            }
        }

		ToggleRender( !pSkill->m_bAccSkill );
		
		m_pkLocked->ToggleRender( true );
		m_pkNormal->ToggleRender( false );
		m_pkActive->ToggleRender( false );

		m_eLearnableSkillID = m_eSkillID;
		if( SiGCSKT()->IsSkillTrainable( m_eSkillID ) == false || bLickSkill )
		{			
			m_pkLocked->ToggleRender( true );
			m_pkNormal->ToggleRender( false );
			m_pkPlus->Lock( true );
			m_kSkillImage.SetRGB( 50, 50, 50 );
		}
		else
		{
			m_pkPlus->Lock( false );
			//m_kSkillImage.SetRGB( 50, 50, 50 );
			m_kSkillImage.SetRGB( 0xFF, 0xFF, 0xFF );
		}
		m_pkMinus->Lock( true );
	}
	
	if( pSkill->IsCash() == true )
	{
		m_pkPlus->Lock( true );
		m_pkMinus->Lock( true );
	}

	for( unsigned int i = 0 ; i < m_apkLine.size() ; ++i )
	{
		m_apkLine[i]->Activate( m_pkMinus->IsLocked() == false || m_pkPlus->IsLocked() == false );
		for( int j = 0 ; j < (int)m_apkLine[i]->GetNumChild() ; ++j )
		{
			m_apkLine[i]->GetChild(j)->Activate( m_pkMinus->IsLocked() == false || m_pkPlus->IsLocked() == false );
		}
	}

	if ( SiGCSKT()->IsUnlockAll() )
	{
		m_pkAlert->ToggleRender( false );
	}

    if ( m_pkOpen )
    {
        m_pkOpen->ToggleRender( false );
        //m_pkOpen->Lock( true );
    }

    if( SiGCSKT()->IsSkillMissionSkillID( m_eSkillID ) ) {
        m_pkPlus->ToggleRender( false );
        m_pkMinus->ToggleRender( false );

        //  안배웠으면 
        if( m_pkOpen && !SiGCSKT()->IsLearnedSkill( m_eSkillID ) ) 
        {

        m_pkOpen->ToggleRender( true );


            if ( SiGCSKT()->IsSkillTrainable( m_eSkillID ) )
            {
                //m_pkOpen->Lock ( false );   
            }
        }

#if defined( LOCK_ASIN_HIDDEN_SKILL )
        m_pkLocked->ToggleRender( true );
        m_pkNormal->ToggleRender( false );
        m_kSkillImage.SetAlpha( 0 );
        m_pkLock->ToggleRender( true );
#endif
    }
}

void KGCSkillBoxSub::Update()
{
	SetBoxState();
	if( SiGCSKT()->IsLearnedSkill( m_eSkillID ) == true )
	{
		const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );
		if ( pSkill->m_bPassive )
		{
			SiGCSKT()->EquipSkill( m_eSkillID, SiGCSKT()->GetSetting(pSkill->GetCharType(),pSkill->GetPromotion() ) );
		}
	}
}

EGCSkillTree KGCSkillBoxSub::GetRequestedSkill()
{
	return RequestLearnSkill;
}

void KGCSkillBoxSub::FrameMoveInEnabledState()
{
	ASSERT( g_pkInput != NULL );

	D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if( CheckPosInWindowBound( vMousePos ) == true && m_pkNormal->IsRenderOn() )
	{
		m_pkActive->ToggleRender( true );
	}
	else
	{
		m_pkActive->ToggleRender( false );
	}

	if( alpha>=0xFF - 6 )
		bIncrease = false;

	if( alpha<=0x00 + 6 )
		bIncrease = true;

	bIncrease?alpha+=5:alpha-=5;

	if( m_pkAlert->IsRenderOn() )
	{
		m_pkAlert->ForceSetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
	}

	m_pkPlus->RenderToolTip( !m_pkPlus->IsLocked() );
	m_pkMinus->RenderToolTip( !m_pkMinus->IsLocked() );
}

void KGCSkillBoxSub::SetCheck( bool bCheck )
{
	m_pkCheck->ToggleRender( bCheck );

    if ( m_pkOpen )
        m_pkOpen->ToggleRender( false );

	if ( bCheck )
	{
		m_pkPlus->ToggleRender( IsAccSkill() );
		m_pkMinus->ToggleRender( IsAccSkill() );
	}
	else
	{
        if( SiGCSKT()->IsSkillMissionSkillID( m_eSkillID ) ) {
            m_pkPlus->ToggleRender( false );
            m_pkMinus->ToggleRender( false );

            //  안배웠으면 
            if( m_pkOpen && !SiGCSKT()->IsLearnedSkill( m_eSkillID ) ) 
            {
                m_pkOpen->ToggleRender( true );
            }

        } else {
		    m_pkPlus->ToggleRender( true );
		    m_pkMinus->ToggleRender( true );
        }
	}
	if ( SiGCSKT()->GetSkill(m_eSkillID)->m_bPassive )
	{
		m_pkCheck->ToggleRender( false );	

        if( SiGCSKT()->IsSkillMissionSkillID( m_eSkillID ) ) {
            m_pkPlus->ToggleRender( false );
            m_pkMinus->ToggleRender( false );
            
            //  안배웠으면 
            if( m_pkOpen && !SiGCSKT()->IsLearnedSkill( m_eSkillID ) ) 
            {
                m_pkOpen->ToggleRender( true );
            }


        } else {
            m_pkPlus->ToggleRender( true );
            m_pkMinus->ToggleRender( true );
        }
	}	
}

void KGCSkillBoxSub::SetRequestedSkill( EGCSkillTree eSkillID )
{
	RequestLearnSkill = eSkillID;
}

DWORD KGCSkillBoxSub::GetUntrainItemID()
{
	return m_dwUntrainItemID;
}

void KGCSkillBoxSub::SetUntrainItemID( DWORD dwItemID )
{
	m_dwUntrainItemID = dwItemID;
}

bool KGCSkillBoxSub::IsAccSkill()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill(m_eSkillID);
	if ( SiGCSKT()->GetSkill(m_eSkillID)->m_bAccSkill )
		return true;

	const SkillSet set = pSkill->GetNextSet();
	for ( SkillSet::const_iterator sit = set.begin() ; sit != set.end(); sit++ )
	{
		if ( SiGCSKT()->GetSkill(*sit)->m_bAccSkill )
		{			
			return true;
		}
	}

	return false;

}