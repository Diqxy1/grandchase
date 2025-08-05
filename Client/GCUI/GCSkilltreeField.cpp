#include "stdafx.h"
#include ".\gcskilltreefield.h"
//
#include "GCSKT.h"
#include "GCSkill.h"
//
#include "GCMyInfoScene.h"


//

IMPLEMENT_CLASSNAME( KGCSkilltreeField );
IMPLEMENT_WND_FACTORY( KGCSkilltreeField );
IMPLEMENT_WND_FACTORY_NAME( KGCSkilltreeField, "gc_skill_tree_field" );

KGCSkilltreeField::KGCSkilltreeField(void)
: m_iCurSet( 0 )
, m_eRolloveredSkillID( (EGCSkillTree)0 )
, m_pSque(NULL)
{
}

KGCSkilltreeField::~KGCSkilltreeField(void)
{
}

void KGCSkilltreeField::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxSub" ) == 0 )
        {
            OnClickSubSkillBox( (KGCSkillBoxSub*)event.m_pWnd );
        }
        else if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxMain" ) == 0 )
        {
            OnClickMainSkillBox( (KGCSkillBoxMain*)event.m_pWnd );
        }
        else if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBox" ) == 0 || strcmp( event.m_pWnd->GetClassName(), "KGCDummySkillBox" ) == 0 )
        {
            OnClickSkillBox( (KGCSkillBox*)event.m_pWnd );
        }
        Update( m_iCurSet );
    }
	else if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
	{
		if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxSub" ) == 0 )
		{
			OnHoverSubSkillBox( (KGCSkillBoxSub*)event.m_pWnd );
		}
		else if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxMain" ) == 0 )
		{
			OnHoverMainSkillBox( (KGCSkillBoxMain*)event.m_pWnd );
		}
		else if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBox" ) == 0 || strcmp( event.m_pWnd->GetClassName(), "KGCDummySkillBox" ) == 0 )
		{
			OnHoverSkillBox( (KGCSkillBox*)event.m_pWnd );
		}
	}
	else if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
	{
		if( strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxSub" ) == 0 || 
			strcmp( event.m_pWnd->GetClassName(), "KGCSkillBoxMain" ) == 0 || 
			strcmp( event.m_pWnd->GetClassName(), "KGCSkillBox" ) == 0 || 
			strcmp( event.m_pWnd->GetClassName(), "KGCDummySkillBox" ) == 0 )
		{
			g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( false );
		}
	}
}

void KGCSkilltreeField::CreateMainSkillBox( EGCSkillGroup eGroup, float x, float y, int iTitleID, bool bCreateSub, int iCharIdx, int iPromotionIdx )
{
    KGCSkillBoxMain* pWnd = GetSkillBoxMain();

    pWnd->SetWindowPosDirect( D3DXVECTOR2( x, y ) );
    pWnd->SetSkillGroup( eGroup );
    pWnd->InitState( true, true, this );
    pWnd->SetTitleID( iTitleID );

    const GCSkill* pSkill = SiGCSKT()->GetBaseSkill( eGroup );
    if( pSkill != NULL )
        pWnd->SetSkill( SiGCSKT()->GetBaseSkill( eGroup )->GetID() );

    m_mapBaseSkill.insert( std::make_pair( eGroup, pWnd ) );

    if( bCreateSub == false )
    {
        return;
    }

    //if( pSkill != NULL)
    //{
    //    const SkillSet& NextSkill = pSkill->GetNextSet();
    //    SkillSet::const_iterator lit;
    //    for( lit = NextSkill.begin(); lit != NextSkill.end() ; ++lit )
    //    {
    //        const GCSkill* pNext = SiGCSKT()->GetSkill( *lit );
    //        if( pNext == NULL )
    //            continue;

    //        const GCSkillUIInfo& sInfo = pNext->GetUIinfo();
    //        CreateSubSkillBox( *lit, sInfo.x, sInfo.y, sInfo.lineType );
    //    }
    //}
    //else
    {
		if( pSkill == NULL)
		{
			pWnd->SetBlank();
		}
        
        // 베이스 스킬이 없는 스킬들
        std::set< EGCSkillTree > setSkills = SiGCSKT()->GetSkillIDsInGroup( eGroup );
        const GCSkill* pGroupSkill;
        for( std::set< EGCSkillTree >::iterator sit = setSkills.begin(); sit != setSkills.end() ; ++sit )
        {
            pGroupSkill = SiGCSKT()->GetSkill( *sit );

            if( pGroupSkill == NULL )
                continue;
			if( pGroupSkill->IsBase() )
			{
				continue;
			}

			//4단 필 On/Off를 위한 함수. 전 케릭터가 전부 오픈 되었을시에 제거될 부분
			if( !g_kGlobalValue.IsTempUseSpecial4Char( iCharIdx ) && !g_kGlobalValue.IsHybridPlayer( iCharIdx ) )
			{
				if( GCFUNC::Is4SpSkill(iCharIdx,pGroupSkill->GetID()) )
					continue;
			}
			
            // 2011_3_3, shmhlove, 눈물의 하드코딩 이유..ㅠㅠ
            // 제로일차전직 작업 중 기본전직 스킬과 연계된 일차전직 스킬이 필요하게 되었어요~
            // 문제는 연계할려면 같은 그룹으로 묶여야하는데
            // UI출력부분에 있어서 같은 그룹이면 전직에 관계없이 스킬박스가 출력되버리지요~
            // 이 문제를 해결할려면 마족캐릭터 전용 전직정보 값을 InitSkillTree.stg파일에 추가해서 처리하면 깔끔하나
            // 그렇게 되면 "서버 + DB + 클라" 모두 작업이 필요하고 작업 Cost 및 프로그램 퍼포먼스도 커지게 되요..

            // 기획자와 상의해서 이번만 같은 그룹스킬로 연계하도록하고 앞으로는 이런 연계스킬은 넣지 않도록 결정했어요!!

            // 여기서 말한 연계는 
            // 기본전직에서 어떤스킬을 사용한다고 핫슬롯에 끼우고, 일차전직에서 어떤스킬을 사용한다고 핫슬롯에 끼우려고 할때
            // 두 스킬이 연계되어 있다면 예전에 끼운스킬이 핫슬롯에서 빠져야합니다.
            // 죄송죄송
            if( GC_CHAR_ZERO == iCharIdx )
            {
                if( 0 == iPromotionIdx && 
                    ( SID_ZERO2_POSIONFORCE_SPECIAL_LV1 == pGroupSkill->GetID() ||
                      SID_ZERO2_POSIONFORCE_SPECIAL_LV2 == pGroupSkill->GetID() ||
                      SID_ZERO2_POSIONFORCE_SPECIAL_LV3 == pGroupSkill->GetID() ) )
                {
                    continue;
                }
                if( 1 == iPromotionIdx && 
                    ( SID_ZERO1_CLASSICALFORCE_SPECIAL_LV1 == pGroupSkill->GetID() ||
                      SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2 == pGroupSkill->GetID() ||
                      SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3 == pGroupSkill->GetID() ||
                      SID_ZERO1_ICEFORCE_SPECIAL_LV1 == pGroupSkill->GetID() ||
                      SID_ZERO1_ICEFORCE_SPECIAL_LV2 == pGroupSkill->GetID() ||
                      SID_ZERO1_ICEFORCE_SPECIAL_LV3 == pGroupSkill->GetID() ||
                      SID_ZERO1_FIREFORCE_SPECIAL_LV1 == pGroupSkill->GetID() ||
                      SID_ZERO1_FIREFORCE_SPECIAL_LV2 == pGroupSkill->GetID() ||
                      SID_ZERO1_FIREFORCE_SPECIAL_LV3 == pGroupSkill->GetID() ) )
                {
                    continue;
                }
            }

            CreateSubSkillBox( pGroupSkill->GetID(), pGroupSkill->GetUIinfo().x, pGroupSkill->GetUIinfo().y, pGroupSkill->GetUIinfo().setLineType );
        }
    }
}

void KGCSkilltreeField::CreateSubSkillBox( EGCSkillTree eSkillID, float x, float y, std::set<int> iLineType )
{
    if( m_mapSubSkillBox.find( eSkillID ) != m_mapSubSkillBox.end() )
        return;

    KGCSkillBoxSub* pWnd = GetSubSkillBox();

    if( pWnd == NULL )
        return;

    pWnd->SetWindowPosDirect( D3DXVECTOR2( x, y ) );
    pWnd->SetSkill( eSkillID );
    pWnd->SetLineType( iLineType );
    pWnd->InitState( true, true, this );

    m_mapSubSkillBox.insert( std::map< int, KGCSkillBoxSub* >::value_type( eSkillID, pWnd ) );

   // const GCSkill* pSkill = SiGCSKT()->GetSkill( eSkillID );

   // if( pSkill != NULL)
   // {
   //     //pWnd->SetToolTip( SiGCSKT()->GetString( pSkill->GetTitleID() ) );
   //     const SkillSet& NextSkill = pSkill->GetNextSet();
   //     SkillSet::const_iterator lit;
   //     for( lit = NextSkill.begin(); lit != NextSkill.end() ; ++lit )
   //     {
   //         const GCSkill* pNext = SiGCSKT()->GetSkill( *lit );
   //         if( pNext == NULL )
   //             continue;
			//if ( pNext->GetGroupID() != pSkill->GetGroupID() )
			//{
			//	continue;
			//}
			//

   //         const GCSkillUIInfo& sInfo = pNext->GetUIinfo();
   //         CreateSubSkillBox( *lit, sInfo.x, sInfo.y, sInfo.lineType );
   //     }
   // }
}

void KGCSkilltreeField::ClearAllChild()
{
    //더미는 예외다.
    std::map< EGCSkillTree, KGCSkillBox* >::iterator mit;
    for( mit = m_mapSkillBox.begin() ; mit != m_mapSkillBox.end() ; ++mit )
    {
        if( mit->first > SiGCSKT()->GetMaxSID() )
        {
            mit->second->ToggleRender( false );
            m_qDummySkillBoxPool.push( (KGCDummySkillBox*)mit->second );
            mit = m_mapSkillBox.erase( mit );

            if( mit != m_mapSkillBox.begin() )
                --mit;
        }
    }
    m_listDummySkillBox.clear();

    std::list< KGCSkillBoxGroup* >::iterator lit;
    for( lit = m_listSkillGroup.begin() ; lit != m_listSkillGroup.end() ; ++lit )
    {
        m_qSkillGroupPool.push( *lit );
        (*lit)->ToggleRender( false );
    }
    m_listSkillGroup.clear();


    std::map< int, KGCSkillBoxMain* >::iterator mitSkillBoxMain;
    for( mitSkillBoxMain = m_mapBaseSkill.begin(); mitSkillBoxMain != m_mapBaseSkill.end() ; ++mitSkillBoxMain )
    {
        mitSkillBoxMain->second->ToggleRender( false );
        m_qBaseSkillPool.push( mitSkillBoxMain->second );
    }
    m_mapBaseSkill.clear();
    std::map< int, KGCSkillBoxSub* >::iterator mitSkillBoxSub;
    for( mitSkillBoxSub = m_mapSubSkillBox.begin(); mitSkillBoxSub != m_mapSubSkillBox.end() ; ++mitSkillBoxSub )
    {
        mitSkillBoxSub->second->ToggleRender( false );
        m_qSubSkillBoxPool.push( mitSkillBoxSub->second );
    }
    m_mapSubSkillBox.clear();
    std::map< EGCSkillTree, KGCSkillBox* >::iterator mitSkillBox;
    for( mitSkillBox = m_mapSkillBox.begin(); mitSkillBox != m_mapSkillBox.end() ; ++mitSkillBox )
    {
        mitSkillBox->second->ToggleRender( false );
        m_qSkillBoxPool.push( mitSkillBox->second );
    }
    m_mapSkillBox.clear();
}

void KGCSkilltreeField::Update( int iSetID )
{
    m_iCurSet = iSetID;
    // 스킬 정보를 업데이트 한다.
    std::map< int, KGCSkillBoxMain* >::iterator mitmain;
    std::map< int, KGCSkillBoxSub* >::iterator mitsub;

    for( mitsub = m_mapSubSkillBox.begin(); mitsub != m_mapSubSkillBox.end() ; ++mitsub )
    {
        mitsub->second->SetCheck( false );
        mitsub->second->Update();
        //if( mitsub->second->IsActive() == true )
        //    MakeTopChild( mitsub->second );
#if !defined( LOCK_ASIN_HIDDEN_SKILL )
        if( mitsub->second->IsRenderOn() ) {            
            const GCSkill* pSkill = SiGCSKT()->GetSkill( mitsub->second->GetSkill() );
            int iCharLv = 999;
            if( pSkill ) {
                iCharLv = pSkill->GetLv();
            }

            std::map< DWORD, int > mapSkillMissionInfo;
            SiGCSKT()->GetSkillMissionInfo( mapSkillMissionInfo );
//             for( std::map< DWORD, int >::iterator mit = mapSkillMissionInfo.begin(); mit != mapSkillMissionInfo.end(); ++mit ) {     
//                 if( mitsub->second->GetSkill() == mit->second && 
//                     g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel >= iCharLv &&
//                     SiGCSKT()->IsLearnedSkill( mitsub->second->GetSkill() ) == false ) {
//                     static std::vector<CParticleEventSeqPTR> vecParticle;
//                     D3DXVECTOR2 vPos = mitsub->second->GetCurrentWindowPos();
//                     vecParticle.push_back( g_ParticleManager->CreateSequence("Skill_Tree_Effect_01" , ( ( vPos.x  ) / 400 )  - 1, ( ( - vPos.y  ) / 400 ) + 1 , 0.5F) );
//                     g_ParticleManager->SetLayer( vecParticle.back(), GC_LAYER_UI);	
//                 }
//             }
        }
#endif
    }

    for( mitmain = m_mapBaseSkill.begin(); mitmain != m_mapBaseSkill.end(); ++mitmain )
    {
        const GCSkill* pSkill = SiGCSKT()->GetEquipedSkill( mitmain->second->GetSkillGroup(), iSetID );
        if( pSkill != NULL )
        {
            if( SiGCSKT()->IsLearnedSkill( pSkill->GetID() ) == true )
            {
                mitmain->second->SetSkill( pSkill->GetID() );
                if( (mitsub = m_mapSubSkillBox.find( pSkill->GetID() )) != m_mapSubSkillBox.end() )
                {
                    mitsub->second->SetCheck( true );
                }
            }
            else
            {
                mitmain->second->SetBlank();
                SiGCSKT()->UnequipSkill( mitmain->second->GetSkillGroup(), m_iCurSet );

                g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
            }
        }
        else
        {
            mitmain->second->SetBlank();
        }
        mitmain->second->Update();
    }

    std::map< EGCSkillTree, KGCSkillBox*>::iterator mitbox;
    std::map< EGCSkillGroup, bool > mapDummyActivation;
    std::map< EGCSkillGroup, KGCSkillBox*> mapDummy;
    for( mitbox = m_mapSkillBox.begin() ; mitbox != m_mapSkillBox.end() ; ++mitbox )
    {
        if( mapDummyActivation.find( mitbox->second->GetGroup() ) == mapDummyActivation.end() )
            mapDummyActivation[ mitbox->second->GetGroup() ] = true;
        mitbox->second->SetActive( SiGCSKT()->IsEquipedSkill( mitbox->second->GetSkill(), m_iCurSet ) );

        const GCSkill* pSkill = SiGCSKT()->GetSkill( mitbox->second->GetSkill() );
        if( strcmp( mitbox->second->GetClassName(), "KGCDummySkillBox" ) == 0 || ( pSkill != NULL && pSkill->IsBase() ) )
            mapDummy[mitbox->second->GetGroup()] = mitbox->second;

        mapDummyActivation[ mitbox->second->GetGroup() ] = mapDummyActivation[ mitbox->second->GetGroup() ] && !mitbox->second->IsActive();

		
    }

    std::map< EGCSkillGroup, KGCSkillBox*>::iterator mitDummy;
    for( mitDummy = mapDummy.begin() ; mitDummy != mapDummy.end() ; ++mitDummy )
    {
        mitDummy->second->SetActive( mapDummyActivation[ mitDummy->second->GetGroup() ] );
    }

	g_pkUIScene->m_pkSkillTree->m_pkSkillSlotBox->Update();
	SiGCSKT()->SetHotKeyIndexToEmpty();
}

void KGCSkilltreeField::OnClickSubSkillBox( KGCSkillBoxSub* pWnd )
{
    const GCSkill* pSkill = SiGCSKT()->GetSkill( pWnd->GetSkill() );

    if( pSkill == NULL )
    {
        //스킬 정보가 없습니다.
        ASSERT( !"스킬 정보가 없음" );
        return;
    }

    if( m_mapBaseSkill.find( pSkill->GetGroupID() ) == m_mapBaseSkill.end() )
    {
        // 슬롯이 없습니다.
        ASSERT( !"베이스 슬롯이 없음" );
        return;
    }

#if defined( LOCK_ASIN_HIDDEN_SKILL )
    if( SiGCSKT()->IsSkillMissionSkillID( pSkill->GetID() ) ) {
        return;
    }
#endif

    if( m_mapBaseSkill[pSkill->GetGroupID()]->GetEquipedSkill() == pSkill->GetID() )
    {
        OnClickMainSkillBox( m_mapBaseSkill[pSkill->GetGroupID()] );
    }
    else
    {
        if( SiGCSKT()->IsLearnedSkill( pSkill->GetID() ) == false )
            return;

        SiGCSKT()->EquipSkill( pWnd->GetSkill(), m_iCurSet );
        m_mapBaseSkill[pSkill->GetGroupID()]->SetSkill( pWnd->GetSkill() );
        MakeTopChild( pWnd );

        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    }
}

void KGCSkilltreeField::OnClickMainSkillBox( KGCSkillBoxMain* pWnd )
{
    //장착 해제 시도
    const GCSkill* pSkill = SiGCSKT()->GetBaseSkill( pWnd->GetSkillGroup() );

    if( pSkill == NULL )
    {
        pWnd->SetBlank();
    }
    else
    {
        pWnd->SetSkill( pSkill->GetID() );
    }
    SiGCSKT()->UnequipSkill( pWnd->GetSkillGroup(), m_iCurSet );
}

KD3DWnd* KGCSkilltreeField::GetMouseRolloverBox()
{
    ASSERT( g_pkInput != NULL );
	EGCSkillTree eSkill = SiGCSKT()->GetMaxSID();
	KD3DWnd* pRet = NULL;

    D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    // 스킬 정보를 업데이트 한다.
    std::map< int, KGCSkillBoxMain* >::reverse_iterator mitmain;
    for( mitmain = m_mapBaseSkill.rbegin(); mitmain != m_mapBaseSkill.rend(); ++mitmain )
    {
        if( mitmain->second->CheckPosInWindowBound( vMousePos ) && mitmain->second->IsRenderOn() )
        {
            eSkill = mitmain->second->GetEquipedSkill();
            pRet = (KD3DWnd*)mitmain->second;
        }
    }

    std::map< int, KGCSkillBoxSub* >::reverse_iterator mitsub;
    for( mitsub = m_mapSubSkillBox.rbegin(); mitsub != m_mapSubSkillBox.rend() ; ++mitsub )
    {
        if( mitsub->second->CheckPosInWindowBound( vMousePos ) && mitsub->second->IsRenderOn() )
        {
            eSkill = mitsub->second->GetSkill();
            pRet = (KD3DWnd*)mitsub->second;
        }
    }	

    std::map< EGCSkillTree, KGCSkillBox* >::reverse_iterator mitbox;
    for( mitbox = m_mapSkillBox.rbegin() ; mitbox != m_mapSkillBox.rend() ; ++mitbox )
    {
        if( mitbox->second->CheckPosInWindowBound( vMousePos ) && mitbox->second->IsRenderOn() )
        {
            eSkill = mitbox->second->GetSkill();
			pRet = (KD3DWnd*)mitbox->second;
        }
    }

	

    m_eRolloveredSkillID = eSkill;
    return pRet;
}

EGCSkillTree KGCSkilltreeField::GetMouseRolloverSkillID()
{
    return m_eRolloveredSkillID;
}

void KGCSkilltreeField::MakeTopChild( KD3DWnd* pWnd )
{
    std::vector<KD3DWnd*>::iterator vit = std::find( m_vecChild.begin(), m_vecChild.end(), pWnd );

    if( vit == m_vecChild.end() )
        return;

    m_vecChild.erase( vit );
    m_vecChild.push_back( pWnd );
}

void KGCSkilltreeField::UnEquipMainSkills()
{
    // 스킬 정보를 업데이트 한다.
    std::map< int, KGCSkillBoxMain* >::iterator mitmain;
    for( mitmain = m_mapBaseSkill.begin(); mitmain != m_mapBaseSkill.end(); ++mitmain )
    {
        OnClickMainSkillBox( mitmain->second );
    }
}

void KGCSkilltreeField::CreateSkillBoxGroup( EGCSkillGroup eGroup, float x, float y, DWORD dwWidth, DWORD dwHeight, int iTitleID )
{
    KGCSkillBoxGroup* pWnd = GetSkillBoxGroup();

    pWnd->ToggleRender( true );
    pWnd->SetWindowPosDirect( D3DXVECTOR2( x, y ) );
    pWnd->SetSize( dwWidth, dwHeight );
    pWnd->SetTitle( SiGCSKT()->GetString( iTitleID ) );

    m_listSkillGroup.push_back( pWnd );

    std::vector< EGCSkillTree > vecSkills = SiGCSKT()->GetSkillIDsInGroupInVector( eGroup );
    const GCSkill* pGroupSkill;
    float f_gab = (float)dwHeight *337.0f / 480.0f + 5.0f;	// 마법의 숫자 6 : 스킬 배치를 위하여... 나중에 스킬 바뀌면 알아서 조절하세용. 
    // 너비 기준으로 마법의 숫자(?) 만들길레 높이 기준으로 바꿈-_-; 3페이지 짜리가 생겨서.

    float f_x = x - f_gab / 2.0f + 10.0f;

    const GCSkill* pBase = SiGCSKT()->GetBaseSkill( eGroup );

    ////아예 스킬이 하나도 없는 경우 그냥 기본 스킬박스만 그려준다.
    // 풀에 누적 돼서 일단 뺍니다.
    //if( vecSkills.empty()==true && pBase == NULL)
    //{
    //	f_x += f_gab;
    //	CreateDummySkillBox( eGroup, f_x, y + (float)dwHeight / 4.0f );		
    //}

    for( std::vector< EGCSkillTree >::iterator sit = vecSkills.begin(); sit != vecSkills.end() ; ++sit )
    {
        pGroupSkill = SiGCSKT()->GetSkill( *sit );

        if( sit == vecSkills.begin() && pBase == NULL )
        {
            f_x += f_gab;
            // Base 스킬이 없으면 스킬을 벗기 위한 박스를 만들어 넣어주자
            CreateDummySkillBox( eGroup, f_x, y + pGroupSkill->GetUIinfo().y );
        }

        if( pGroupSkill == NULL )
            continue;

        f_x += f_gab;

        CreateSkillBox( pGroupSkill->GetID(), f_x, y + pGroupSkill->GetUIinfo().y );
    }
}

void KGCSkilltreeField::CreateSkillBox( EGCSkillTree eSkillID, float x, float y )
{
    if( m_mapSkillBox.find( eSkillID ) != m_mapSkillBox.end() )
        return;

    KGCSkillBox* pWnd = GetSkillBox();
    pWnd->SetWindowPosDirect( D3DXVECTOR2( x, y ) );

    if( pWnd == NULL )
    {
        ASSERT( !"템플릿 UI 생성 실패, 파일이나 Factory를 확인해 보기!!" );
        return;
    }

    pWnd->InitState( true, true, this );
    pWnd->SetWindowPosDirect( D3DXVECTOR2( x - (float)(pWnd->GetWidth() / 2), y ) );
    pWnd->SetSkill( eSkillID );
    MakeTopChild( pWnd );

    m_mapSkillBox.insert( std::map< EGCSkillTree, KGCSkillBox*	>::value_type( eSkillID, pWnd ) );
}

void KGCSkilltreeField::CreateDummySkillBox( EGCSkillGroup eGroupID, float x, float y )
{
    KGCDummySkillBox* pWnd = GetDummySkillBox();
    pWnd->SetWindowPosDirect( D3DXVECTOR2( x, y ) );

    if( pWnd == NULL )
    {
        ASSERT( !"템플릿 UI 생성 실패, 파일이나 Factory를 확인해 보기!!" );
        return;
    }

    pWnd->InitState( true, true, this );
    pWnd->SetWindowPosDirect( D3DXVECTOR2( x - (float)(pWnd->GetWidth() / 2), y ) );
    pWnd->SetSkill( SiGCSKT()->GetMaxSID() );
    pWnd->SetGroup( eGroupID );
    MakeTopChild( pWnd );

    m_listDummySkillBox.push_back( pWnd );
    m_mapSkillBox.insert( std::map< EGCSkillTree, KGCSkillBox*	>::value_type( (EGCSkillTree)(SiGCSKT()->GetMaxSID() + m_listDummySkillBox.size()) , (KGCDummySkillBox*)pWnd ) );
}

void KGCSkilltreeField::OnClickSkillBox( KGCSkillBox* pWnd )
{
    // 더미의 경우에는 그냥 벗긴다.
    if( pWnd->GetSkill() >= SiGCSKT()->GetMaxSID() )
    {
        SiGCSKT()->UnequipSkill( pWnd->GetGroup(), m_iCurSet );
        return;
    }

    const GCSkill* pSkill = SiGCSKT()->GetSkill( pWnd->GetSkill() );

    if( pSkill == NULL )
    {
        //스킬 정보가 없습니다.
        ASSERT( !"스킬 정보가 없음" );
        return;
    }

    //베이스 스킬이면 그냥 벗긴다.
    if( pSkill->IsBase() == true )
    {
        SiGCSKT()->UnequipSkill( pWnd->GetGroup(), m_iCurSet );
        return;
    }

    if( SiGCSKT()->IsEquipedSkill( pSkill->GetID(), m_iCurSet ) == false )
    {
        if( SiGCSKT()->IsLearnedSkill( pSkill->GetID() ) == false )
            return;

        SiGCSKT()->EquipSkill( pWnd->GetSkill(), m_iCurSet );

        //g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    }
}

void KGCSkilltreeField::HideSubSkillBox()
{
    ToggleRender_SubSkillBox( false );
}

void KGCSkilltreeField::ShowSubSkillBox()
{
    ToggleRender_SubSkillBox( true );
}

void KGCSkilltreeField::ToggleRender_SubSkillBox( bool bRender )
{
    std::map< int, KGCSkillBoxSub* >::iterator mitSubSkillBox;
    for( mitSubSkillBox = m_mapSubSkillBox.begin(); mitSubSkillBox != m_mapSubSkillBox.end() ; ++mitSubSkillBox )
    {
        mitSubSkillBox->second->ToggleRender( bRender );
    }

    std::map< EGCSkillTree, KGCSkillBox*>::iterator mitSkillBox;
    for( mitSkillBox = m_mapSkillBox.begin(); mitSkillBox != m_mapSkillBox.end() ; ++mitSkillBox )
    {
        mitSkillBox->second->ToggleRender( bRender );
    }
}

KGCSkillBoxMain* KGCSkilltreeField::GetSkillBoxMain()
{
    KGCSkillBoxMain* pReturn = NULL;
    if( m_qBaseSkillPool.size() == 0 )
        pReturn = (KGCSkillBoxMain*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_main", "ui_skillbox_main.stg", "skillbox_main", true, true );
    else
    {
        pReturn = m_qBaseSkillPool.front();
        m_qBaseSkillPool.pop();
    }

    ASSERT( pReturn != NULL );

    return pReturn;
}

KGCSkillBoxSub* KGCSkilltreeField::GetSubSkillBox()
{

    KGCSkillBoxSub* pReturn = NULL;
    if( m_qSubSkillBoxPool.size() == 0 )
    {
        pReturn = (KGCSkillBoxSub*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_sub", "ui_skillbox_sub.stg", "skillbox_sub", true, true );
    }
    else
    {
        pReturn = m_qSubSkillBoxPool.front();
        m_qSubSkillBoxPool.pop();
    }

    ASSERT( pReturn != NULL );

    return pReturn;
}

KGCSkillBox* KGCSkilltreeField::GetSkillBox()
{

    KGCSkillBox* pReturn = NULL;
    if( m_qSkillBoxPool.size() == 0 )
    {
        pReturn = (KGCSkillBox*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox", "ui_skillbox.stg", "skillbox", true, true );
    }
    else
    {
        pReturn = m_qSkillBoxPool.front();
        m_qSkillBoxPool.pop();
    }

    ASSERT( pReturn != NULL );

    return pReturn;
}

KGCDummySkillBox* KGCSkilltreeField::GetDummySkillBox()
{
    KGCDummySkillBox* pReturn = NULL;
    if( m_qDummySkillBoxPool.size() == 0 )
    {
        pReturn = (KGCDummySkillBox*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_dummy", "ui_skillbox.stg", "skillbox_dummy", true, true );
    }
    else
    {
        pReturn = m_qDummySkillBoxPool.front();
        m_qDummySkillBoxPool.pop();
    }

    ASSERT( pReturn != NULL );

    return pReturn;
}

KGCSkillBoxGroup* KGCSkilltreeField::GetSkillBoxGroup()
{
    KGCSkillBoxGroup* pReturn = NULL;
    if( m_qSkillGroupPool.size() == 0 )
    {
        pReturn = (KGCSkillBoxGroup*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_group", "ui_skillbox_group.stg", "skillbox_group", true, true );
    }
    else
    {
        pReturn = m_qSkillGroupPool.front();
        m_qSkillGroupPool.pop();
    }

    ASSERT( pReturn != NULL );

    return pReturn;
}

void KGCSkilltreeField::OnLoad()
{
//     //  메모리 사용량 문제로 인해삭제 처리
//     for( int i = 0 ; i < 15 ; ++i )
//     {
//         m_qBaseSkillPool.push( (KGCSkillBoxMain*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_main", "ui_skillbox_main.stg", "skillbox_main" ) );
//         m_qBaseSkillPool.back()->ToggleRender( false );
//         m_qSubSkillBoxPool.push( (KGCSkillBoxSub*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_sub", "ui_skillbox_sub.stg", "skillbox_sub" ) );
//         m_qSubSkillBoxPool.back()->ToggleRender( false );
//         m_qSkillGroupPool.push( (KGCSkillBoxGroup*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_group", "ui_skillbox_group.stg", "skillbox_group" ) );
//         m_qSkillGroupPool.back()->ToggleRender( false );
//         m_qDummySkillBoxPool.push( (KGCDummySkillBox*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox_dummy", "ui_skillbox.stg", "skillbox_dummy" ) );
//         m_qDummySkillBoxPool.back()->ToggleRender( false );
//     }
//     for( int i = 0 ; i < 40 ; ++i )
//     {
//         m_qSkillBoxPool.push( (KGCSkillBox*)g_pkUIMgr->CreateTemplateUI( this, "gc_skillbox", "ui_skillbox.stg", "skillbox" ) ); m_qSkillBoxPool.back()->ToggleRender( false );
//     }
}

void KGCSkilltreeField::OnHoverSubSkillBox( KGCSkillBoxSub* pWnd )
{
#if defined( LOCK_ASIN_HIDDEN_SKILL )
    if( SiGCSKT()->IsSkillMissionSkillID( pWnd->GetSkill() ) ) {
        return;
    }
#endif
	OnSetDescBoxInfo( pWnd->GetSkill(), pWnd );	

	const GCSkill* pSkill = SiGCSKT()->GetSkill( pWnd->GetSkill() );

	if( pSkill )
	{
		static std::vector<CParticleEventSeqPTR> vecParticle;
		SkillSet setBlingBlingIcon;
		setBlingBlingIcon = pSkill->GetPrevSet();			
		for ( unsigned int i = 0 ; i < vecParticle.size(); i++ )
		{
			g_ParticleManager->DeleteSequence( vecParticle[i] );
		}
		vecParticle.clear();
	
		SkillSet::iterator sit = setBlingBlingIcon.begin();

		while( sit != setBlingBlingIcon.end() )
		{
			if ( !SiGCSKT()->IsLearnedSkill( *sit ) && m_mapSubSkillBox.find( *sit ) != m_mapSubSkillBox.end())
			{
				D3DXVECTOR2 vPos = m_mapSubSkillBox[*sit]->GetCurrentWindowPos();
				// 				D3DXVECTOR2 vPos(GetRelocatedWindowPos());
				// 				vPos += m_mapSubSkillBox[*sit]->GetFixedWindowLocalPos();
				vPos.x += m_mapSubSkillBox[*sit]->GetWidth()/2;
				vPos.y += m_mapSubSkillBox[*sit]->GetHeight()/2;

				m_pSque = g_ParticleManager->CreateSequence("Skill_TreeBox_Effect_01");
				m_pSque->SetPosition(PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z);

				vecParticle.push_back( m_pSque );
				//vecParticle.push_back( g_ParticleManager->CreateSequence("Skill_Tree_Effect_01" , 0.5  ,0.5 , 0.5F) );
				g_ParticleManager->SetLayer( vecParticle.back(), GC_LAYER_UI);							
			}
			sit ++;
		}
	}	
}

void KGCSkilltreeField::OnHoverMainSkillBox( KGCSkillBoxMain* pWnd )
{
	OnSetDescBoxInfo( pWnd->GetSkill(), pWnd );	
}

void KGCSkilltreeField::OnHoverSkillBox( KGCSkillBox* pWnd )
{
	OnSetDescBoxInfo( pWnd->GetSkill(), pWnd );	

	const GCSkill* pSkill = SiGCSKT()->GetSkill( pWnd->GetSkill() );

	if( pSkill )
	{
		static std::vector<CParticleEventSeqPTR> vecParticle;
		SkillSet setBlingBlingIcon;
		setBlingBlingIcon = pSkill->GetPrevSet();			
		for ( unsigned int i = 0 ; i < vecParticle.size(); i++ )
		{
			g_ParticleManager->DeleteSequence( vecParticle[i] );
		}
		vecParticle.clear();

		SkillSet::iterator sit = setBlingBlingIcon.begin();

		while( sit != setBlingBlingIcon.end() )
		{
			if ( !SiGCSKT()->IsLearnedSkill( *sit ) && m_mapSkillBox.find( *sit ) != m_mapSkillBox.end())
			{
				D3DXVECTOR2 vPos = m_mapSkillBox[*sit]->GetCurrentWindowPos();
				// 				D3DXVECTOR2 vPos(GetRelocatedWindowPos());
				// 				vPos += m_mapSubSkillBox[*sit]->GetFixedWindowLocalPos();
				vPos.x += m_mapSkillBox[*sit]->GetWidth()/2;
				vPos.y += m_mapSkillBox[*sit]->GetHeight()/2;

				m_pSque = g_ParticleManager->CreateSequence("Skill_TreeBox_Effect_02");
				m_pSque->SetPosition(PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z);

				vecParticle.push_back( m_pSque );
				//vecParticle.push_back( g_ParticleManager->CreateSequence("Skill_Tree_Effect_01" , 0.5  ,0.5 , 0.5F) );
				g_ParticleManager->SetLayer( vecParticle.back(), GC_LAYER_UI);							
			}
			sit ++;
		}
	}
}

void KGCSkilltreeField::OnSetDescBoxInfo( EGCSkillTree eSkill, KD3DWnd* pWnd )
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( eSkill );
	if( pSkill != NULL )
	{
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( true );
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetTitle( g_pkUIScene->m_pkSkillTree->GetSkillTitle( pSkill ) );
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetDesc( SiGCSKT()->GetString( pSkill->GetDescID() ) );
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetMiddleDesc( g_pkUIScene->m_pkSkillTree->GetMiddleDesc( pSkill ) );
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetTargetPosition( pWnd->GetCurrentWindowPos(), pWnd->GetWidth(), pWnd->GetHeight() );
	}
	else
	{
		g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( false );
	}
}

void KGCSkilltreeField::ClearParticle()
{
	if (m_pSque)
		g_ParticleManager->DeleteSequence(m_pSque);
}

KGCSkillBox* KGCSkilltreeField::GetTrainableSkillBox()
{
    if ( m_mapSkillBox.empty() )
        return NULL;

    std::map< EGCSkillTree, KGCSkillBox* >::iterator itSB = m_mapSkillBox.begin();
    for ( ; itSB != m_mapSkillBox.end(); ++itSB ) {
        if ( SiGCSKT()->IsSkillTrainable( (*itSB).first ) == true )
            return (*itSB).second;
    }

    return NULL;
}

KGCSkillBoxSub* KGCSkilltreeField::GetTrainableSubSkillBox()
{
    if ( m_mapSubSkillBox.empty() )
        return NULL;

    std::map< int, KGCSkillBoxSub* >::iterator itSSB = m_mapSubSkillBox.begin();
    for ( ; itSSB != m_mapSubSkillBox.end(); ++itSSB ) {
        if ( SiGCSKT()->IsSkillTrainable( static_cast<EGCSkillTree>( (*itSSB).first ) ) == true )
            return (*itSSB).second;
    }

    return NULL;
}
