#include "stdafx.h"
#include ".\gcquestranktotalscore.h"
//


IMPLEMENT_CLASSNAME( KGCQuestRankTotalScore );
IMPLEMENT_WND_FACTORY( KGCQuestRankTotalScore );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestRankTotalScore, "gc_quest_result_rank_total_score" );

KGCQuestRankTotalScore::KGCQuestRankTotalScore(void)
{
    m_bInitValue         = false;
    m_iNumOfStars       = 0;
    //m_pkBackGround      = NULL;
    for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
    {
        m_pkStar[i] = NULL;
    }

    //LINK_CONTROL( "background",         m_pkBackGround );
    LINK_CONTROL( "star0",              m_pkStar[0] );
    LINK_CONTROL( "star1",              m_pkStar[1] );
    LINK_CONTROL( "star2",              m_pkStar[2] );
    LINK_CONTROL( "star3",              m_pkStar[3] );
    LINK_CONTROL( "star4",              m_pkStar[4] );
}

KGCQuestRankTotalScore::~KGCQuestRankTotalScore(void)
{
}

void KGCQuestRankTotalScore::OnCreate( void )
{
    //m_pkBackGround->InitState( true, false, this );
    for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
    {
		m_pkStar[i]->InitState( true, false, this );
		m_pkStar[i]->SetWndMode(D3DWM_DEFAULT );
    }

    InitShowGradeStarEffect();
}

void KGCQuestRankTotalScore::InitShowGradeStarEffect(void)
{
    if( !m_bInitValue )
    {
        for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
        {
            m_Star[i].InitValue( (int)m_pkStar[i]->GetFixedWindowLocalPos().x, (int)m_pkStar[i]->GetFixedWindowLocalPos().y
                , (int)m_pkStar[i]->GetWidth(), (int)m_pkStar[i]->GetHeight() );
        }
        m_bInitValue = true;
    }
    else
    {
        for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
        {
            m_pkStar[i]->SetFixedWindowLocalPos( m_Star[i].GetOriginalPos() );
            m_pkStar[i]->SetWidthDirect(m_Star[i].GetOriginalWidth());
            m_pkStar[i]->SetHeightDirect(m_Star[i].GetOriginalHeight());
        }
    }
}

void KGCQuestRankTotalScore::OnDestroy( void )
{

}

void KGCQuestRankTotalScore::FrameMoveInEnabledState( void )
{
    for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
        FrameMove_GradeStarEffect(&m_Star[i], m_pkStar[i]);
}

void KGCQuestRankTotalScore::PostChildDraw( void )
{

}

void KGCQuestRankTotalScore::SetTotalScore(int iNumOfStars)
{
    if( 0 > iNumOfStars ) iNumOfStars = 0;
    if( (MAX_QUEST_RANK_TOTAL_SCORE) < iNumOfStars ) iNumOfStars = (MAX_QUEST_RANK_TOTAL_SCORE);    // iNumOfStart 는 인덱스가 아니므로 -1을 하지 않았음.

    for(int i=0;i<MAX_QUEST_RANK_TOTAL_SCORE;++i)
    {
        if( iNumOfStars > i )
        {
            m_pkStar[i]->SetWndMode(D3DWM_SELECT);
            SetShowGradeStarEffect(i, (int)(GC_FPS_LIMIT*0.3f + 3.0f*i), (int)(GC_FPS_LIMIT*1.2f + 2.0f*i), 2000, 100);
        }
        else
        {
            m_pkStar[i]->SetWndMode(D3DWM_DEFAULT);
        }
        m_pkStar[i]->ToggleRender(false);
    }

    m_iNumOfStars = iNumOfStars;
}

void KGCQuestRankTotalScore::SetShowGradeStarEffect(int iGrade, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent)
{
    KGCD3DWndEffect     *pEffect        = NULL;
    KD3DWnd             *pkWindow       = NULL;

    if( IsValidQuestRankTotalScoreIndex(iGrade) )
    {
        pEffect                 = &m_Star[iGrade];
        pkWindow                = m_pkStar[iGrade];
    }

    if( pEffect && pkWindow )
    {
        pEffect->SetFramePerAlterationScale(iScaleFrame, iShowFrame, iStartScalePercent, iDestScalePercent);

        // 처음 시작 값 설정
        pkWindow->SetWidthDirect((DWORD)pEffect->GetCurrentWidth() );
        pkWindow->SetHeightDirect((DWORD)pEffect->GetCurrentHeight() );

        // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함... 
        const float fChangeX = (float)(pEffect->GetCurrentWidth()/2 - pEffect->GetOriginalWidth()/2);
        const float fChangeY = (float)(pEffect->GetCurrentHeight()/2 - pEffect->GetOriginalHeight()/2);
        pkWindow->SetWindowPosDirect( pEffect->GetOriginalPos() - D3DXVECTOR2( fChangeX, fChangeY ) );
    }

}

void KGCQuestRankTotalScore::FrameMove_GradeStarEffect(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow)
{
    if( !pEffectInfo || !pWindow ) return;

    // 표시 여부를 확인하는 부분
    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // 스케일을 진행하는 부분
        if( !pEffectInfo->IsEndOfScale() )
        {
            pEffectInfo->ScaleStep();        

            // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함... 
            const float fChangeX = (float)(pEffectInfo->GetCurrentWidth()/2 - pEffectInfo->GetOriginalWidth()/2);
            const float fChangeY = (float)(pEffectInfo->GetCurrentHeight()/2 - pEffectInfo->GetOriginalHeight()/2);

            pWindow->SetWindowPosDirect( pEffectInfo->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));
            pWindow->SetWidthDirect((int)pEffectInfo->GetCurrentWidth());
            pWindow->SetHeightDirect((int)pEffectInfo->GetCurrentHeight());
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 123, 0xff, 0xff, 0xff));

            if( pEffectInfo->IsEndOfScale() )
                g_KDSound.Play( "136" );
        }
        else
        {
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff));
        }
        pWindow->ToggleRender( pEffectInfo->GetCurrentWidth() < 300 );
    }

    if( pEffectInfo->IsEndOfShow() )
    {
        pWindow->ToggleRender(true);
    }
}

bool KGCQuestRankTotalScore::IsValidQuestRankTotalScoreIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < MAX_QUEST_RANK_TOTAL_SCORE )
        return true;

    return false;
}
