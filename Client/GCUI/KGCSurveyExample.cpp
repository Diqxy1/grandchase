#include "stdafx.h"
#include "KGCSurveyExample.h"

IMPLEMENT_CLASSNAME( KGCSurveyExample );
IMPLEMENT_WND_FACTORY( KGCSurveyExample );
IMPLEMENT_WND_FACTORY_NAME( KGCSurveyExample, "gc_survey_example" );

KGCSurveyExample::KGCSurveyExample( void )
: m_pkCheckBox( NULL )
, m_pkSelectBox( NULL )
, m_pkHoverBox( NULL )
, m_pkEditAnswer( NULL )
, m_pkStaticText( NULL )
, m_iExampleType( KGCSurveyDlg::QUESTION_NORMAL )
, m_iNumber( 0 )
, m_pkImgExample( NULL )
, m_bIsSetExample( false )
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < ESURVEY_EXAMPLE_NUM; i++ ) {
        m_pkExampleNum[i] = NULL;
        sprintf( str, "lc_num%d", i );  
        LINK_CONTROL( str, m_pkExampleNum[i] );
    }

    LINK_CONTROL( "exam_checkback", m_pkCheckBox );
    LINK_CONTROL( "exam_sel_box", m_pkSelectBox );
    LINK_CONTROL( "exam_hover_box", m_pkHoverBox );

    LINK_CONTROL( "lc_edit_answer", m_pkEditAnswer );
    LINK_CONTROL( "lc_static_text", m_pkStaticText );

    LINK_CONTROL( "lc_imageicon", m_pkImgExample );

}

KGCSurveyExample::~KGCSurveyExample( void )
{
}

void KGCSurveyExample::OnCreate( void )
{
    for( int i = 0; i < ESURVEY_EXAMPLE_NUM; i++ ) {
        if( m_pkExampleNum[i] != NULL ) {
            m_pkExampleNum[i]->InitState( false );
        }
    }

    if( m_pkCheckBox != NULL ) {
        m_pkCheckBox->InitState( false, true, this );
        m_pkCheckBox->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
    }

    if( m_pkHoverBox != NULL ) {
        m_pkHoverBox->InitState( false );
    }

    if( m_pkSelectBox != NULL ) {
        m_pkSelectBox->InitState( false );
    }

    if( m_pkEditAnswer != NULL ) {
        m_pkEditAnswer->InitState( false, true, this );
        m_pkEditAnswer->SetMultiLine( true );
        m_pkEditAnswer->SetNewLine( true );
        m_pkEditAnswer->SetLimitText( 200 );
        m_pkEditAnswer->SetLimitLine( 12 );
    }

    if( m_pkStaticText != NULL ) {
        m_pkStaticText->InitState( false );
        
    }

    if( m_pkImgExample != NULL ) {
        m_pkImgExample->InitState( false );
    }

}

void KGCSurveyExample::ActionPerformed( const KActionEvent& event )
{

}

void KGCSurveyExample::SetExampleNumber( int iNum_ )
{
    m_iNumber = iNum_;
    for( int i = 0; i < ESURVEY_EXAMPLE_NUM; i++ ) {
        m_pkExampleNum[i]->ToggleRender( false );

        if( i == m_iNumber ) {
            m_pkExampleNum[i]->ToggleRender( true );
        }
    }
}

void KGCSurveyExample::SetExampleType( int iType_ )
{
    m_iExampleType = iType_;
    switch( iType_ ) {
        case KGCSurveyDlg::QUESTION_NORMAL:
            {
                if( m_pkCheckBox != NULL ) {
                    m_pkCheckBox->ToggleRender( true );
                }
            }
            break;
        case KGCSurveyDlg::QUESTION_IMAGE:
            {
                if( m_pkCheckBox != NULL ) {
                    m_pkCheckBox->ToggleRender( false );
                }

            }
            break;
        case KGCSurveyDlg::QUESTION_INPUT:
            {
                if ( m_pkEditAnswer != NULL && m_pkStaticText != NULL ) {
                    m_pkEditAnswer->ToggleRender( true );
                    m_pkStaticText->ToggleRender( true );
                }                
            }
            break;
        default:
            break;
    }
}

void KGCSurveyExample::RenderHoverBox( bool bRender_ )
{
    if( m_pkHoverBox != NULL ) {
        m_pkHoverBox->ToggleRender( bRender_ );
    }
}

void KGCSurveyExample::RenderSelectBox( bool bRender_ )
{
    if( m_pkSelectBox != NULL ) {
        m_pkSelectBox->ToggleRender( bRender_ );
    }

    if( m_iNumber < ESURVEY_EXAMPLE_NUM && m_pkExampleNum[m_iNumber] != NULL ) {
            m_pkExampleNum[m_iNumber]->Lock( bRender_ );
    }
}

void KGCSurveyExample::SetCheckBoxTitle( std::wstring wsTitle_ )
{
    if ( m_pkCheckBox != NULL ) {
        m_pkCheckBox->SetText( wsTitle_ );
    }
}

void KGCSurveyExample::SetCheckBoxLock( bool bLock_, D3DCOLOR dwColor_ /*= 0xFFFFFFFF*/, D3DCOLOR OutlineColor /*= D3DCOLOR_ARGB( 255, 51, 76, 97 )*/ )
{
    if ( m_pkCheckBox != NULL ) {
        m_pkCheckBox->SetCheck( bLock_ );

        SetFontColor( dwColor_ );
        SetFontOutline( true, OutlineColor );

    }
}

bool KGCSurveyExample::IsRenderOnSelectBox()
{
    if( m_pkSelectBox != NULL ) {
        return m_pkSelectBox->IsRenderOn();
    } else {
        return false;
    }
}

void KGCSurveyExample::InitInfo()
{
    if( m_pkHoverBox != NULL && m_pkSelectBox != NULL ) {
        m_pkHoverBox->ToggleRender( false );
        m_pkSelectBox->ToggleRender( false );
    }

    for( int i = 0; i < ESURVEY_EXAMPLE_NUM; i++ ) {
        if( m_pkExampleNum[i] != NULL ) {
            m_pkExampleNum[i]->ToggleRender( false );
            m_pkExampleNum[i]->Lock( false );
        }
    }

    m_bIsSetExample = false;
}   

void KGCSurveyExample::SetExampleInfo( int iType_, std::wstring& wstrExample_ )
{
    switch( iType_ ) {
        case KGCSurveyDlg::QUESTION_NORMAL:
            {
                if( m_pkCheckBox != NULL ) {
                    m_pkCheckBox->ToggleRender( true );
                    m_pkCheckBox->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
                    m_pkCheckBox->SetFontOutline( true, D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
                    m_pkCheckBox->SetText( wstrExample_ );
                    m_bIsSetExample = true;
                }                
            }
            break;
        case KGCSurveyDlg::QUESTION_IMAGE:
            {
                if( m_pkImgExample != NULL ) {
                    m_pkImgExample->ToggleRender( true );
                    m_pkImgExample->SetTexNameUseMassFile( wstrExample_ );
                    m_bIsSetExample = true;
                }
            }
            break;
        case KGCSurveyDlg::QUESTION_INPUT:
            {
                if( m_pkEditAnswer != NULL && m_pkStaticText != NULL ) {
                    m_pkStaticText->ToggleRender( true );
                    m_pkStaticText->SetText( L"0 / 200" );
                    m_pkStaticText->SetAlign( DT_CENTER );
                    m_pkEditAnswer->ToggleRender( true );
                    m_pkEditAnswer->SetText( wstrExample_.c_str() );
                    m_pkEditAnswer->SetFocus();
                    m_bIsSetExample = true;
                }
            }
        default:
            break;
    }
}

void KGCSurveyExample::FrameMoveInEnabledState()
{
    if ( m_pkEditAnswer != NULL && m_pkStaticText != NULL ) {
        WCHAR strTemp[20];
        std::wstring strCurNum = m_pkEditAnswer->GetText();
        swprintf( strTemp, 20, L"(%d / %d)", strCurNum.length(), 200 );		
        m_pkStaticText->SetText(strTemp);

    }
    
}

void KGCSurveyExample::GetEditAnswer( std::wstring& wstrAnswer_ )
{
    wstrAnswer_ = m_pkEditAnswer->GetText();
}

void KGCSurveyExample::SetFontColor( D3DCOLOR dwColor_ )
{
    if ( m_pkCheckBox != NULL ) {
        m_pkCheckBox->SetFontColor( dwColor_ );
    }
}

void KGCSurveyExample::SetFontOutline( bool bFontOutline /*= false*/, D3DCOLOR OutlineColor /*= 0xFF000000 */ )
{
    if ( m_pkCheckBox != NULL ) {
        m_pkCheckBox->SetFontOutline( bFontOutline, OutlineColor );
    }
}
