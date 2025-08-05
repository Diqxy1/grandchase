#include "stdafx.h"
#include ".\gcquestrankscore.h"
#include "KDSound.h"
//
#define S3_SCORE_TEXT_SCALE 0.7
#define S3_SCORE_TEXT_INTERVAL 20


IMPLEMENT_CLASSNAME( KGCQuestRankScore );
IMPLEMENT_WND_FACTORY( KGCQuestRankScore );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestRankScore, "gc_quest_result_rank_score" );

KGCQuestRankScore::KGCQuestRankScore(void)
{
    m_vPos              = D3DXVECTOR2(25.0f, 4.0f);
    m_fNumWidth         = 16.0f;
    m_fNumHeight        = 20.0f;
    m_fNumInterval      = 12.0f;
    m_iMaxNumOfNumber   = 10;
    m_biScore           = 0;

    m_fCurScore         = 0.0f;
    m_fPrevScore        = 0.0f;

    //m_pkBackGround      = NULL;
    m_pDungeon          = NULL;
    //LINK_CONTROL( "background",        m_pkBackGround );

    for(int i=0;i<MAX_QUEST_RANK_SCORE_NUMBER;++i)
    {
        // 숫자의 UV 좌표를 저장해둔다.
        m_rtNumber[i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE((234 + (i)*16.8f),    512)
                            , CONVERT_PIXEL_TO_TEX_SIZE(157,            512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((234 + (i+1)*16.8f ),  512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((157+21),         512)
                            );
    }

    m_dwColor = 0xffffffff;
}

KGCQuestRankScore::~KGCQuestRankScore(void)
{
    SAFE_RELEASE(m_pDungeon);
}

void KGCQuestRankScore::OnCreate( void )
{
    SAFE_RELEASE(m_pDungeon);
    m_pDungeon = g_pGCDeviceManager2->CreateTexture("dungeon.dds");
    //m_pkBackGround->InitState( true, false, this );
    m_dwColor = 0xffffffff;
    m_biScore = 0;
    m_fCurScore = 0.0f;
    m_fPrevScore = 0.0f;
    m_stScore.SetCurValue(0);
}

void KGCQuestRankScore::OnDestroy( void )
{    
    SAFE_RELEASE(m_pDungeon);
}

void KGCQuestRankScore::FrameMoveInEnabledState( void )
{
    if( !m_stScore.IsComplete() )
    {
        m_stScore.Step();
    }
}

void KGCQuestRankScore::SetNumOfNumber(int iMaxNumOfNumber)
{
    if( MAX_NUM_OF_NUMBER <= iMaxNumOfNumber ) iMaxNumOfNumber = MAX_NUM_OF_NUMBER;
    if( 0 > iMaxNumOfNumber ) iMaxNumOfNumber = 0;

    m_iMaxNumOfNumber = iMaxNumOfNumber;
}

void KGCQuestRankScore::PostChildDraw( void )
{
    g_pGCDeviceManager2->PushState();
    {
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );        
        
        int iScore = (int)m_stScore.GetCurValue();
        DWORD iDivide = MAX_NUM_OF_NUMBER_DIVIDE;

        D3DXVECTOR2 vPos = m_vPos + GetFixedWindowPos();

        m_fPrevScore = m_fCurScore;
        m_fCurScore = static_cast<float>( iScore );
        if( m_fPrevScore != m_fCurScore && g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
		{
            g_KDSound.PlayNoOverlapping("Dungeon_Result_Score", 0, 0);
		}
        /*else
            g_KDSound.Stop("Dungeon_Result_Score");*/

        vPos = GetFixedWindowPos();
        vPos.x *= g_pGCDeviceManager->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager->GetWindowScaleY();

        for(int i=0;i<MAX_NUM_OF_NUMBER;++i)
        {
            if( MAX_NUM_OF_NUMBER-m_iMaxNumOfNumber <= i )
            {                              
                vPos.x += ( S3_SCORE_TEXT_INTERVAL * g_pGCDeviceManager->GetWindowScaleX());
                DrawNumber(vPos, (iScore / iDivide), m_dwColor, (float)S3_SCORE_TEXT_SCALE);                            
            }
            iScore %= iDivide;
            iDivide /= 10;
        }
    }
	g_pGCDeviceManager2->PopState();    
}

void KGCQuestRankScore::DrawNumber(D3DXVECTOR2 vPos, int iNum, DWORD dwColor_, float fScale)
{
    if( iNum >= MAX_QUEST_RANK_SCORE_NUMBER || 0 > iNum  ) iNum = MAX_QUEST_RANK_SCORE_NUMBER-1;

    g_pkUIMgr->RenderNumberScale( "orange_gradation_num", vPos, iNum, fScale, dwColor_, false);
}

void KGCQuestRankScore::SetScore( GCEXPTYPE biScore_ )
{
    m_biScore                = biScore_;
    m_stScore.SetInterpolate(CHANGE_SCORE_FRAME, (float)biScore_, (float)biScore_);
}

void KGCQuestRankScore::SetLinearScore( GCEXPTYPE biScore_ )
{
    m_stScore.SetInterpolate(CHANGE_SCORE_FRAME, (float)m_biScore, (float)biScore_);
    m_biScore = biScore_;
}

void KGCQuestRankScore::SetNumPos(D3DXVECTOR2 vPos)
{
    m_vPos = vPos;
}

void KGCQuestRankScore::SetNumWidth(float fWidth)
{
    m_fNumWidth = fWidth;
}

void KGCQuestRankScore::SetNumHeight(float fHeight)
{
    m_fNumHeight = fHeight;
}

void KGCQuestRankScore::SetNumInterval(float fInterval)
{
    m_fNumInterval = fInterval;
}
