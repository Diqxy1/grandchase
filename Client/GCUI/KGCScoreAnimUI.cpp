#include "stdafx.h"
#include ".\KGCScoreAnimUI.h"

#define ALPHA_WEIGHT 5
#define TEXT_INTERVAL 30.0f
#define TEXT_SIZE_WIDTH     36.571428f
#define TEXT_SIZE_HEIGHT    46.666666f

IMPLEMENT_CLASSNAME( KGCScoreAnimUI );
IMPLEMENT_WND_FACTORY( KGCScoreAnimUI );
IMPLEMENT_WND_FACTORY_NAME( KGCScoreAnimUI, "gc_score_anim_ui" );

KGCScoreAnimUI::KGCScoreAnimUI(void)
{
    m_fNumWidth         = TEXT_SIZE_WIDTH;
    m_fNumHeight        = TEXT_SIZE_HEIGHT;    
    m_pTex = NULL;
}

KGCScoreAnimUI::~KGCScoreAnimUI(void)
{
}

void KGCScoreAnimUI::OnCreate( void )
{
    SAFE_RELEASE(m_pTex);
    m_pTex = g_pGCDeviceManager2->CreateTexture("letter.dds");
    m_dwColor = 0xffffffff;
    m_bRender = false;
    m_bAlphaAnim = false;
    m_bIsCompleteAnim = false;
    m_bStartAlphaAnim = false;
    m_bEndAlphaAnim = false;
    m_iAlphaCount = 255;
    m_iMoveWeight = 0;
    m_fNumInterval = TEXT_INTERVAL;
    m_fScale = 1.0f;
    m_iLetterTextCount = 1;
    m_vPos = D3DXVECTOR2(0.0f, 0.0f);
    m_vOrigivalPos = m_vPos;
    m_vecLetterText.clear();
}

void KGCScoreAnimUI::OnDestroy( void )
{    
    SAFE_RELEASE(m_pTex);
    m_bRender = false;
}

void KGCScoreAnimUI::AnimationMoveY(float fPosY, int iWeight, bool bAlphaAnim )
{
    float fTemp = 0.0f;
    float fTemp2 = 0.0f;
    D3DXVECTOR2 vTemp;

    if( fPosY > 0 )
        fPosY += m_vOrigivalPos.y;
    else
        fPosY -= m_vOrigivalPos.y;

    if( fPosY > m_vOrigivalPos.y )
    {
        if( fPosY > 0 )
            vTemp.y =  m_vOrigivalPos.y + ((fPosY - m_vOrigivalPos.y)/1.2f);
        else
            vTemp.y =  m_vOrigivalPos.y + ((fPosY + m_vOrigivalPos.y)/1.2f);

        fTemp = m_vPos.y + iWeight;
        if( fTemp > fPosY )
        {
            m_vPos.y = fPosY;
            m_bIsCompleteAnim = true;
        }
        else   
            m_vPos.y = fTemp;

        if( vTemp.y < m_vPos.y && bAlphaAnim )
            m_bStartAlphaAnim = true;
    }
    else if( fPosY < m_vOrigivalPos.y )
    {
        if( fPosY > 0 )
            vTemp.y =  fPosY - ((m_vOrigivalPos.y - fPosY)/1.2f);
        else
            vTemp.y =  fPosY - ((m_vOrigivalPos.y + fPosY)/1.2f);

        fTemp = m_vPos.y - iWeight;
        if( fTemp < fPosY )
        {
            m_vPos.y = fPosY;
            m_bIsCompleteAnim = true;
        }
        else
            m_vPos.y = fTemp;

        if( vTemp.y > m_vPos.y && bAlphaAnim )
            m_bStartAlphaAnim = true;
    }
}

void KGCScoreAnimUI::SetAnimMoveY( float fPosY, float iWeight, bool bAlphaAnim )
{
    if( m_vecLetterText.empty() == false )
        strcpy(Letter_Text, m_vecLetterText[0].c_str());
    m_bRender = true;
    m_vDst.y = fPosY;
    m_iMoveWeight = (int)iWeight;
    m_bAlphaAnim = bAlphaAnim;
}

void KGCScoreAnimUI::SetLetter(std::string letter, float fScale )
{
    //strcpy(Letter_Text, letter);
    m_vecLetterText.push_back(letter);
    m_fScale = fScale;
    m_fNumInterval = TEXT_INTERVAL;
    m_fNumInterval *= fScale;
}

void KGCScoreAnimUI::DrawLetter()
{
    int iSize = (int)strlen(Letter_Text);

    float fTemp = 0.0f;
    float fScaleX = 0.0f;
    float fScaleY = 0.0f;

    fScaleX=g_pGCDeviceManager->GetWindowScaleX() * m_fScale;
    fScaleY=g_pGCDeviceManager->GetWindowScaleY() * m_fScale;

    D3DXVECTOR2 vPos = m_vPos + GetFixedWindowPos();
    vPos.x *= g_pGCDeviceManager->GetWindowScaleX();
    vPos.y *= g_pGCDeviceManager->GetWindowScaleY();
    fTemp = vPos.x;

    for(int i = 0 ; i < iSize ; i++)
    {               
        if(Letter_Text[i]>'A'-1 && Letter_Text[i]<'H'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'A')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'A'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = 0;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = .1666f;
        }else if(Letter_Text[i]>'G' && Letter_Text[i]<'O'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'H')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'H'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .1666f;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = .3333f;
        }else if(Letter_Text[i]>'N' && Letter_Text[i]<'V'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'O')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'O'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .3333f;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = .5f;
        }else if(Letter_Text[i]>'U' && Letter_Text[i]<'Z+3'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'V')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'V'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .5f;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = .6666f;
        }else if(Letter_Text[i]>'0'-1 && Letter_Text[i]<'7'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'0')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'0'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .6666f;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = .8333f;
        }else if(Letter_Text[i]>'6'){
            m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)(Letter_Text[i]-'7')/7.0f;
            m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(Letter_Text[i]-'7'+1)/7.0f;
            m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .8333f;
            m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = 1.0f;
        }else{

            int iTemp = -1;
            if( Letter_Text[i] == '/' )
                iTemp = 3;
            else if( Letter_Text[i] == '+' )
                iTemp = 4;
            else if( Letter_Text[i] == '-' )
                iTemp = 5;
            else if( Letter_Text[i] == '%' )
                iTemp = 6;

            if( iTemp != -1 ){
                m_rectNumRender[0].m_vTex.x = m_rectNumRender[2].m_vTex.x = (float)iTemp/7.0f;
                m_rectNumRender[1].m_vTex.x = m_rectNumRender[3].m_vTex.x = (float)(iTemp+1)/7.0f;
                m_rectNumRender[0].m_vTex.y = m_rectNumRender[1].m_vTex.y = .8333f;
                m_rectNumRender[2].m_vTex.y = m_rectNumRender[3].m_vTex.y = 1.0f;
            }
            else
            {
                if( Letter_Text[i] == '\n' )
                {                    
                    vPos.x = fTemp;
                    vPos.y += (m_fNumHeight*fScaleY);
                }
                else if( Letter_Text[i]==' ' )
                {
                    vPos.x += (m_fNumWidth*fScaleX);
                }

                continue;
            }
        }


        SETVECTOR4( m_rectNumRender[0].m_vPos, vPos.x, vPos.y, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[1].m_vPos, vPos.x + m_fNumWidth*fScaleX, vPos.y, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[2].m_vPos, vPos.x, vPos.y + m_fNumHeight*fScaleY, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[3].m_vPos, vPos.x + m_fNumWidth*fScaleX, vPos.y + m_fNumHeight*fScaleY, 0.0f, 1.0f );

        m_rectNumRender[0].m_diff = m_dwColor;
        m_rectNumRender[1].m_diff = m_dwColor;
        m_rectNumRender[2].m_diff = m_dwColor;
        m_rectNumRender[3].m_diff = m_dwColor;

        g_pGCDeviceManager2->DrawInScreen(m_pTex, m_rectNumRender);

        vPos.x += (m_fNumInterval * g_pGCDeviceManager->GetWindowScaleX());
    }
}

bool KGCScoreAnimUI::IsCompleteMoveAnim() 
{ 
    return m_bIsCompleteAnim; 
}

bool KGCScoreAnimUI::IsCompleteAlphaAnim()
{ 
    return m_bEndAlphaAnim; 
}

void KGCScoreAnimUI::FrameMoveInEnabledState( void )
{
    if( m_iMoveWeight == 0 )
        return;
    AnimationMoveY( m_vDst.y, m_iMoveWeight, m_bAlphaAnim );

    if( IsCompleteAlphaAnim() && IsCompleteMoveAnim() )
    {
        if( m_vecLetterText.size() > 1 && m_iLetterTextCount < (int)m_vecLetterText.size() )
        {
            if( m_iLetterTextCount < (int)m_vecLetterText.size())
                strcpy(Letter_Text, m_vecLetterText[m_iLetterTextCount].c_str());
            m_vPos = m_vOrigivalPos;
            m_iAlphaCount = 255;
            m_bIsCompleteAnim = false;
            m_bEndAlphaAnim = false;
            m_iLetterTextCount++;
        }
    }    
}

void KGCScoreAnimUI::PostChildDraw( void )
{
    if( m_bRender == false )
        return;

    if( m_bStartAlphaAnim )
    {
        m_iAlphaCount -= ALPHA_WEIGHT;
        if( m_iAlphaCount <= 0 )
        {
            m_iAlphaCount = 0;
            m_bEndAlphaAnim = true;
        }            
        m_dwColor = D3DCOLOR_RGBA( 255, 255, 255, m_iAlphaCount );
    }

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

        DrawLetter();
    }
    g_pGCDeviceManager2->PopState();    
}
