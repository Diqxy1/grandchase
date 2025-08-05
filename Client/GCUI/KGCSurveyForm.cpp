#include "stdafx.h"
#include "KGCSurveyForm.h"

IMPLEMENT_CLASSNAME( KGCSurveyForm );
IMPLEMENT_WND_FACTORY( KGCSurveyForm );
IMPLEMENT_WND_FACTORY_NAME( KGCSurveyForm, "gc_survey_form" );

struct sQuestion;

KGCSurveyForm::KGCSurveyForm( void )
: m_pkStaticIntroduce( NULL )
, m_pkStaticReward( NULL)
, m_pkStaticPage( NULL )
, m_pkBtnNext( NULL )
, m_pkBtnDone( NULL )
, m_pkExampleInput( NULL )
, m_iFormType( KGCSurveyDlg::QUESTION_NORMAL )
, m_iMaxNumOfAnswer( 1 )
, m_iCurQuestionNum( 0 )
, m_iQuestionPage( 0 )
{
    m_deqAnswer.clear();
    m_mapSurveyQuestion.clear();
    m_mapNumberAnswer.clear();
    m_mapStringAnswer.clear();

    LINK_CONTROL( "lc_static_introduce", m_pkStaticIntroduce );
    LINK_CONTROL( "lc_static_reward", m_pkStaticReward );
    LINK_CONTROL( "lc_static_page", m_pkStaticPage );
    LINK_CONTROL( "lc_btn_next", m_pkBtnNext );
    LINK_CONTROL( "lc_btn_done", m_pkBtnDone );

    char str[MAX_PATH] = {0,};
    for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
        m_pkExample[i] = NULL;
        sprintf( str, "survey_example%d", i );
        LINK_CONTROL( str, m_pkExample[i] );

        m_pkExampleImg[i] = NULL;
        sprintf( str, "survey_example_img%d", i );  
        LINK_CONTROL( str, m_pkExampleImg[i] );
    }

    LINK_CONTROL( "survey_example_input", m_pkExampleInput );

}

KGCSurveyForm::~KGCSurveyForm( void )
{    
}

void KGCSurveyForm::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnNext,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnNext );
    GCWND_MSG_MAP( m_pkBtnDone,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnDone );
}

void KGCSurveyForm::OnCreate( void )
{
    m_pkBtnNext->InitState( true, true, this );
    m_pkBtnDone->InitState( false, true, this );

    for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
        m_pkExample[i]->InitState( false, true, this );
        m_pkExampleImg[i]->InitState( false, true, this );
    }

    m_pkStaticIntroduce->InitState( true );
    m_pkStaticReward->InitState( true );
    m_pkStaticReward->SetAlign( DT_CENTER );

    m_pkStaticPage->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
    m_pkStaticPage->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
    m_pkStaticPage->InitState( true );
    m_pkExampleInput->InitState( false, true, this );
    
}

void KGCSurveyForm::SetFormType( int iType_, unsigned int unExampleSize_ )
{    
    m_deqAnswer.clear();
    m_iFormType = iType_;
    switch( iType_ ) {
        case KGCSurveyDlg::QUESTION_NORMAL:
            {
                m_pkExampleInput->ToggleRender( false );
                for( unsigned int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) { // 일단 전부 render off 시킨 후
                    m_pkExample[i]->ToggleRender( false );
                    m_pkExample[i]->SetCheckBoxLock( false );
                    m_pkExampleImg[i]->ToggleRender( false );
                }
                for( unsigned int i = 0; i < unExampleSize_ && i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {   // 필요한 만큼 render on
                    m_pkExample[i]->ToggleRender( true );
                }
            }
            break;
        case KGCSurveyDlg::QUESTION_IMAGE:
            {
                m_pkExampleInput->ToggleRender( false );
                for( unsigned int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                    m_pkExample[i]->ToggleRender( false );
                    m_pkExampleImg[i]->ToggleRender( false );
                }
                for( unsigned int i = 0; i < unExampleSize_ && i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                    m_pkExampleImg[i]->ToggleRender( true );
                }
            }
            break;
        case KGCSurveyDlg::QUESTION_INPUT:
            {
                m_pkExampleInput->ToggleRender( true );
                for( unsigned int i = 0; i < unExampleSize_ && i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                    m_pkExample[i]->ToggleRender( false );
                    m_pkExampleImg[i]->ToggleRender( false );
                    m_pkBtnNext->Lock( false );
                    m_pkBtnDone->Lock( false );
                }
            }
            break;
        default:
            break;
    }
}

void KGCSurveyForm::FrameMoveInEnabledState( void )
{
    if ( g_pkInput->IsDown( DIK_ESCAPE ) ) {
        this->ToggleRender(false);
        ClearData();
        g_pkUIScene->m_pkSurveyDlg->OnNo();

        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    }

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    if( m_iFormType == KGCSurveyDlg::QUESTION_NORMAL ) {
        for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
            RenderExampleBox( m_pkExample[i], vMousePos, i );
        }
    } else if ( m_iFormType == KGCSurveyDlg::QUESTION_IMAGE ) {
        for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
            RenderExampleBox( m_pkExampleImg[i], vMousePos, i );
        }
    }
    //else if ( m_iFormType == KGCSurveyDlg::QUESTION_INPUT ) {
    //    // 주관식 답안이 빈칸이어도 다음/완료 버튼 활성화되도록 설정
    //    RenderEditBox();
    //}
}

void KGCSurveyForm::RenderExampleBox( KGCSurveyExample* pkExample_, const D3DXVECTOR2& vMousePos_, int iIdx_ )
{
    if( pkExample_->IsSetExample() == false ) { 
        return;
    }

    pkExample_->RenderHoverBox( false );
    if( pkExample_->CheckPosInWindowBound( vMousePos_ ) ) {
        if ( !pkExample_->IsRenderOnSelectBox() ) {
            pkExample_->RenderHoverBox( true );
        }

        if( g_pkInput->BtnDown( KInput::MBLEFT ) ) {

            std::deque< std::wstring >::iterator dit;
            WCHAR szIndex[MAX_PATH] = {0,};
            std::wstring wstr;
            wstr = _itow( iIdx_ + 1, szIndex, 10 );

            if ( !pkExample_->IsRenderOnSelectBox() ) { // select가 아닐 때
                pkExample_->RenderSelectBox( true );
                if ( m_iFormType == KGCSurveyDlg::QUESTION_NORMAL || m_iFormType == KGCSurveyDlg::QUESTION_IMAGE ) {
                    pkExample_->SetCheckBoxLock( true, D3DCOLOR_ARGB( 255, 255, 230, 154 ), D3DCOLOR_ARGB( 255, 19, 35, 51 ) );
                }

                if ( !pkExample_->IsRenderOn() ) {
                    return;
                }

                dit = find( m_deqAnswer.begin(), m_deqAnswer.end(), wstr );
                if ( dit == m_deqAnswer.end() ) {
                    m_deqAnswer.push_back( wstr );
                    if ( static_cast<int>( m_deqAnswer.size() ) > m_iMaxNumOfAnswer ) {
                        switch ( m_iFormType ) {
                        case KGCSurveyDlg::QUESTION_NORMAL:
                            {
                                m_pkExample[ _wtoi( m_deqAnswer.begin()->c_str() ) - 1 ]->RenderSelectBox( false );
                                m_pkExample[ _wtoi( m_deqAnswer.begin()->c_str() ) - 1 ]->SetCheckBoxLock( false );
                            }
                            break;
                        case KGCSurveyDlg::QUESTION_IMAGE:
                            {
                                m_pkExampleImg[ _wtoi( m_deqAnswer.begin()->c_str() ) - 1 ]->RenderSelectBox( false );
                                m_pkExampleImg[ _wtoi( m_deqAnswer.begin()->c_str() ) - 1 ]->SetCheckBoxLock( false );
                            }
                            break;
                        default:
                            break;
                        }
                        m_deqAnswer.pop_front();
                    }
                }

            }
            else {  // select 일 때
                pkExample_->RenderSelectBox( false  );
                if ( m_iFormType == KGCSurveyDlg::QUESTION_NORMAL || m_iFormType == KGCSurveyDlg::QUESTION_IMAGE ) {
                    pkExample_->SetCheckBoxLock( false );
                }

                dit = find( m_deqAnswer.begin(), m_deqAnswer.end(), wstr );
                if ( dit != m_deqAnswer.end() ) {
                    m_deqAnswer.erase( dit );
                }
            }

            if( static_cast< int >( m_deqAnswer.size() ) == m_iMaxNumOfAnswer ) {
                switch ( m_iFormType ) {
                    case KGCSurveyDlg::QUESTION_NORMAL:
                        {
                            m_pkBtnNext->Lock( false );
                            m_pkBtnDone->Lock( false );
                        }
                        break;
                    case KGCSurveyDlg::QUESTION_IMAGE:
                        {
                            m_pkBtnNext->Lock( false );
                            m_pkBtnDone->Lock( false );
                        }
                        break;
                    default:
                        break;
                }
            }
            else {
                switch ( m_iFormType ) {
                    case KGCSurveyDlg::QUESTION_NORMAL:
                        {
                            m_pkBtnNext->Lock( true );
                            m_pkBtnDone->Lock( true );
                        }
                        break;
                    case KGCSurveyDlg::QUESTION_IMAGE:
                        {
                            m_pkBtnNext->Lock( true );
                            m_pkBtnDone->Lock( true );
                        }
                        break;
                    default:
                        break;
                }
            }

        }

    }
}

void KGCSurveyForm::SetExampleText( int iExampleCnt_, std::map< int, std::wstring >& mapExampleText_ )
{
    if ( mapExampleText_.size() != iExampleCnt_ ) {
        std::map< int, std::wstring >::iterator itm;
        for ( itm = mapExampleText_.begin(); itm != mapExampleText_.end(); itm++ ) {
            for ( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++) {
                m_pkExample[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
                m_pkExample[i]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
                m_pkExample[i]->SetCheckBoxTitle( itm->second );
            }
        }
    }
}

void KGCSurveyForm::SetSurveyData( int iSurveyType_, int iDataFirst_, int iDataSecond_, std::map< int, KSurveyQuestion >& mapSurveyQuestion_ )
{
    m_iSurveyType = iSurveyType_;
    m_iDataFirst = iDataFirst_;
    m_iDataSecond = iDataSecond_;
    m_mapSurveyQuestion = mapSurveyQuestion_;
}

void KGCSurveyForm::SetQuestionText( std::wstring& wsQuestion_ )
{
    m_pkStaticIntroduce->SetFontColor( D3DCOLOR_ARGB( 255, 255, 218, 129 ) );
    m_pkStaticIntroduce->SetFontOutline( true, D3DCOLOR_ARGB( 255, 19, 35, 51 ) );
    m_pkStaticIntroduce->SetTextAutoMultiline( wsQuestion_ );
    m_pkStaticIntroduce->ToggleRender( true );
}

// 문제 그려주는 함수
void KGCSurveyForm::RenderQuestion( int iNum_ ) // 파라미터는 문제번호
{
    m_deqAnswer.clear();
    InitExampleInfo();
    m_iQuestionPage++;
    m_iCurQuestionNum = iNum_;

    // 마지막 문제 들어오면 완료 버튼 그려주자.
    if( m_iQuestionPage == m_mapSurveyQuestion.size() ) {
        m_pkBtnDone->Lock( true );
        m_pkBtnDone->ToggleRender( true );
        m_pkBtnNext->ToggleRender( false );
    }
    else {
        m_pkBtnDone->ToggleRender( false );
        m_pkBtnNext->Lock( true );
        m_pkBtnNext->ToggleRender( true );
    }

    std::map< int, KSurveyQuestion >::iterator mit = m_mapSurveyQuestion.find( iNum_ );

    if( mit != m_mapSurveyQuestion.end() ) {
        /*
            1 - 100   : 스트링 객관식 문제유형      -> 0
            101 - 200 : 이미지 객관식 문제 유형     -> 1
            201       : 주관식                      -> 2
        */
        int iQuestionType = ( mit->second.m_iQuestionType - 1 ) / 100;
        SetNumOfAnswer( ( mit->second.m_iQuestionType ) % 100 );

        SetFormType( iQuestionType, mit->second.m_mapExample.size() );
        SetQuestionText( mit->second.m_wsQuestion );

        char str[MAX_PATH] = {0,};
        sprintf( str, "%d", m_iQuestionPage );
        std::string strTemp = str;

        SetQuestionNumberText( g_pkStrLoader->GetString( STR_ID_SURVEY_INTRODUCE_SURVEY_NUM ) + KncUtil::toWideString( strTemp ) );

        SetPageNumberText( m_iQuestionPage, static_cast< int >( m_mapSurveyQuestion.size() ) );

        switch( iQuestionType ) {
            case KGCSurveyDlg::QUESTION_NORMAL:
                {
                    m_pkExampleInput->ToggleRender( false );
                    for( unsigned int i = 0; i < mit->second.m_mapExample.size() && i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                        m_pkExampleImg[i]->ToggleRender( false );
                        m_pkExample[i]->ToggleRender( true );
                        m_pkExample[i]->SetCheckBoxLock( false );
                        std::map< int, std::wstring >::iterator mitEx = mit->second.m_mapExample.find( i + 1 );
                        if( mitEx != mit->second.m_mapExample.end() ) {
                            m_pkExample[i]->SetExampleInfo( iQuestionType, mitEx->second );
                            m_pkExample[i]->SetExampleNumber( mitEx->first - 1 );
                        }
                    }
                }
                break;
            case KGCSurveyDlg::QUESTION_IMAGE:
                {
                    m_pkExampleInput->ToggleRender( false );
                    for( unsigned int i = 0; i < mit->second.m_mapExample.size() && i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                        m_pkExampleImg[i]->ToggleRender( true );
                        m_pkExample[i]->ToggleRender( false );
                        std::map< int, std::wstring >::iterator mitEx = mit->second.m_mapExample.find( i + 1 );
                        if( mitEx != mit->second.m_mapExample.end() ) {
                            m_pkExampleImg[i]->SetExampleInfo( iQuestionType, mitEx->second );
                            m_pkExampleImg[i]->SetExampleNumber( mitEx->first - 1 );
                        }
                    }
                }
                break;
            case KGCSurveyDlg::QUESTION_INPUT:
                {
                    
                    m_pkExampleInput->ToggleRender( true );
                    std::wstring strTemp = L"";
                    m_pkExampleInput->SetExampleInfo( iQuestionType, strTemp );
                    for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
                        m_pkExampleImg[i]->ToggleRender( false );
                        m_pkExample[i]->ToggleRender( false );
                    }
                }
                break;
            default:
                break;
        }
    } else {
        // 문제가 없다는 메시지 박스 출력
    }
}

void KGCSurveyForm::OnNext()
{
    g_KDSound.Play( "31" );
    if ( m_iFormType == KGCSurveyDlg::QUESTION_NORMAL || m_iFormType == KGCSurveyDlg::QUESTION_IMAGE ) {
        if( m_deqAnswer.empty() )
            return;

        std::deque< std::wstring >::iterator itd;
        std::vector< int > vecAnswer;
        for ( itd = m_deqAnswer.begin(); itd != m_deqAnswer.end(); ++itd ) {
            vecAnswer.push_back( _wtoi( itd->c_str() ) );
        }
        m_mapNumberAnswer.insert( std::make_pair( m_iCurQuestionNum, vecAnswer ) );

    }
    else if ( m_iFormType == KGCSurveyDlg::QUESTION_INPUT ) {
        std::wstring wstr;
        m_pkExampleInput->GetEditAnswer( wstr );
        m_mapStringAnswer.insert( std::make_pair( m_iCurQuestionNum, wstr ) );
    }

    std::map< int, KSurveyQuestion >::iterator itm = m_mapSurveyQuestion.begin();
    for ( int i = 1; i <= m_iQuestionPage; ++i ) {
        itm++;
    }
    RenderQuestion( itm->first );
}

void KGCSurveyForm::OnDone()
{
    g_KDSound.Play( "31" );
    if ( m_iFormType == KGCSurveyDlg::QUESTION_NORMAL || m_iFormType == KGCSurveyDlg::QUESTION_IMAGE ) {
        if( m_deqAnswer.empty() )
            return;

        std::deque< std::wstring >::iterator itd;
        std::vector< int > vecAnswer;
        for ( itd = m_deqAnswer.begin(); itd != m_deqAnswer.end(); ++itd ) {
            vecAnswer.push_back( _wtoi( itd->c_str() ) );
        }
        m_mapNumberAnswer.insert( std::make_pair( m_iCurQuestionNum, vecAnswer ) );

    }
    else if ( m_iFormType == KGCSurveyDlg::QUESTION_INPUT ) {
        std::wstring wstr;
        m_pkExampleInput->GetEditAnswer( wstr );
        m_mapStringAnswer.insert( std::make_pair( m_iCurQuestionNum, wstr ) );
    }

    this->ToggleRender(false);

    // 응답 전송하기
    KEVENT_SURVEY_REWARD_REQ kRewardPacket;
    kRewardPacket.m_nType = m_iSurveyType;
    kRewardPacket.m_nDataFirst = m_iDataFirst;
    kRewardPacket.m_nDataSecond = m_iDataSecond;
    kRewardPacket.m_mapNumberAnswer = m_mapNumberAnswer;
    kRewardPacket.m_mapStringAnswer = m_mapStringAnswer;

    ClearData();
    KP2P::GetInstance()->Send_SurveyRewardReq( kRewardPacket );
    g_pkUIScene->m_pkSurveyDlg->OnNo();

    if ( m_iSurveyType == 0 ) {
        g_pkUIScene->m_pkSurveyDlg->SetHaveQuestion( false );
    }
}

void KGCSurveyForm::InitExampleInfo()
{
    m_pkExampleInput->InitInfo();

    for( int i = 0; i < KGCSurveyExample::ESURVEY_EXAMPLE_NUM; i++ ) {
        m_pkExample[i]->InitInfo();
        m_pkExampleImg[i]->InitInfo();
    }
}

void KGCSurveyForm::SetQuestionNumberText( std::wstring wsNumberText_ )
{
    m_pkStaticReward->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
    m_pkStaticReward->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 79, 97 ) );
    m_pkStaticReward->SetText( wsNumberText_ );
    m_pkStaticReward->SetAlign( DT_CENTER );
    m_pkStaticReward->ToggleRender( true );

}

void KGCSurveyForm::SetPageNumberText( int iCurrentPage_, int iLastPage_ )
{
    char str[MAX_PATH] = {0,};
    sprintf( str, "%d / %d", iCurrentPage_, iLastPage_ );
    m_pkStaticPage->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
    m_pkStaticPage->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 79, 97 ) );
    m_pkStaticPage->SetText( KncUtil::toWideString( std::string( str ) ) );
    m_pkStaticPage->SetAlign( DT_CENTER );
}

void KGCSurveyForm::GetUserAnswers( std::map< int, std::vector< int > >& mapNumberAnswer_, std::map< int, std::wstring >& mapStringAnswer_ )
{
    mapNumberAnswer_ = m_mapNumberAnswer;
    mapStringAnswer_ = m_mapStringAnswer;

}

void KGCSurveyForm::ClearData()
{
    m_iMaxNumOfAnswer = 1;
    m_iCurQuestionNum = 0;
    m_iQuestionPage = 0;

    m_mapNumberAnswer.clear();
    m_mapStringAnswer.clear();
    m_deqAnswer.clear();
    m_mapSurveyQuestion.clear();

}

void KGCSurveyForm::RenderEditBox()
{
    std::wstring wstr;
    m_pkExampleInput->GetEditAnswer( wstr );

    if ( wstr.size() == 0 ) {
        m_pkBtnNext->Lock( true );
        m_pkBtnDone->Lock( true );
    }
    else {
        m_pkBtnNext->Lock( false );
        m_pkBtnDone->Lock( false );
    }
}

int KGCSurveyForm::GetFirstQuestionNumber()
{
    std::map< int, KSurveyQuestion >::iterator itm = m_mapSurveyQuestion.begin();

    if ( itm != m_mapSurveyQuestion.end() ) {
        return itm->first;
    }
    return -1;
}
