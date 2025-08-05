#include "stdafx.h"
#include "KGCSquareEnterWndS6.h"

IMPLEMENT_CLASSNAME( KGCSquareEnterWndS6 );
IMPLEMENT_WND_FACTORY( KGCSquareEnterWndS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareEnterWndS6, "gc_square_enter_ui_s6" );

KGCSquareEnterWndS6::KGCSquareEnterWndS6( void )
: m_pkLowQuality(NULL)
, m_pkHighQuality(NULL)
, m_pkClose(NULL)
, m_pkLowQualityStatic(NULL)
, m_pkHighQualityStatic(NULL)
, m_pkChannelBar(NULL)
, m_bIsGuild(false)
, m_pkTitleChannelNormal(NULL)
, m_pkTitleChannelGuild(NULL)
, m_dwUpdateFrame(0)
{
    LINK_CONTROL("low_quality_check", m_pkLowQuality);
    LINK_CONTROL("high_quality_check", m_pkHighQuality);
    LINK_CONTROL("close_btn", m_pkClose);
    LINK_CONTROL("low_quality_static" , m_pkLowQualityStatic);
    LINK_CONTROL("high_qulity_static ", m_pkHighQualityStatic);
    LINK_CONTROL( "channel_bar", m_pkChannelBar );
    LINK_CONTROL( "title_channel_normal", m_pkTitleChannelNormal);
    LINK_CONTROL( "title_channel_guild" , m_pkTitleChannelGuild);

    for( int i = 0 ; i < (int)m_pChannelBar.size() ; ++i )
    {
        m_pChannelBar[i] = NULL;
    }
}

KGCSquareEnterWndS6::~KGCSquareEnterWndS6( void )
{
}

void KGCSquareEnterWndS6::OnCreate()
{
    m_pkClose->InitState( true, true, this );
    m_pkClose->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCSquareEnterWndS6::OnClose);
    m_pkClose->SetHotKey(DIK_ESCAPE);
    
    m_pkHighQuality->InitState(true,true,this);
    m_pkHighQuality->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCSquareEnterWndS6::OnHighQuality);
    
    m_pkLowQuality->InitState(true,true,this);
    m_pkLowQuality->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCSquareEnterWndS6::OnLowQuality);

    m_pkLowQualityStatic->SetText(  g_pkStrLoader->GetString( STR_ID_LOW_QULITY ));
    m_pkHighQualityStatic->SetText(  g_pkStrLoader->GetString( STR_ID_HIGH_QULITY ));

    m_pkTitleChannelNormal->InitState(false);
    m_pkTitleChannelGuild->InitState(false);

    m_pkChannelBar->InitState( false );
    float fStartPosX = m_pkChannelBar->GetFixedWindowLocalPos().x;
    float fStartPosY = m_pkChannelBar->GetFixedWindowLocalPos().y;

    // 슬롯 생성
    std::vector<KD3DWnd*> vecSlotList = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_square_enter_ui_s6.stg", "", "square_enter_ui_s6\\channel_bar", true, MAX_CHANNEL_SLOT );
    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwHeight = (*iterSlot)->GetHeight();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_pChannelBar[i] = static_cast<KGCSquareChannelBar*>(*iterSlot);
            m_pChannelBar[i]->InitState( true, true, this );

            D3DXVECTOR2 vPos;
            vPos.x = fStartPosX;
            vPos.y = fStartPosY + static_cast<float>( (i)*(dwHeight+BAR_GAP) );
            m_pChannelBar[i]->SetFixedWindowLocalPos(vPos);
        }
    }
}

void KGCSquareEnterWndS6::OnCreateComplete()
{
    m_pkTitleChannelNormal->ToggleRender(!m_bIsGuild);
    m_pkTitleChannelGuild->ToggleRender(m_bIsGuild);

    if( true == g_MyD3D->m_KGCOption.GetSquareQuality() ) {
        OnHighQuality();
    }
    else {
        OnLowQuality();
    }
}


void KGCSquareEnterWndS6::ActionPerformed(const KActionEvent& event )
{
}

void KGCSquareEnterWndS6::FrameMoveInEnabledState()
{
    m_dwUpdateFrame++;

    if( m_dwUpdateFrame >= GC_FPS_LIMIT * 10 )
    {
        UpdateList();
    }
}

void KGCSquareEnterWndS6::OnHighQuality()
{
    m_pkHighQuality->SetCheck( true );
    m_pkLowQuality->SetCheck( false );
    g_MyD3D->m_KGCOption.SetSquareQuality( true );
}

void KGCSquareEnterWndS6::OnLowQuality()
{
    m_pkHighQuality->SetCheck( false );
    m_pkLowQuality->SetCheck( true );
    g_MyD3D->m_KGCOption.SetSquareQuality( false );
}

void KGCSquareEnterWndS6::OnClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCSquareEnterWndS6::UpdateChannelBar()
{
    std::vector< KSquareInfo > vecSquareInfo = SiGCSquare()->GetSquareInfo();

    ASSERT( vecSquareInfo.size() > 0 );

    int iEnd = (int)vecSquareInfo.size() > MAX_CHANNEL_SLOT 
        ? MAX_CHANNEL_SLOT : (int)vecSquareInfo.size();

    int iCnt = 0;
    bool bEnableNum = false;
    //CLOSE 아닌 리스트만 ChannelBar에 담는다.
    for( int i = 0; i < iEnd; ++i )
    {
        if ( vecSquareInfo[i].m_nStatus != SQUARE_CHANNEL_CLOSE )
            bEnableNum = true;
        else
            bEnableNum = false;

        //Cnt 사이즈 체크 
        if ( (iCnt > i || iCnt < 0 ) ) 
            continue;
        
        if ( m_pChannelBar[iCnt] != NULL && bEnableNum) {
            m_pChannelBar[iCnt]->ToggleRender( true );
            m_pChannelBar[iCnt]->SetSquareChannelInfo( vecSquareInfo[i], i );
            ++iCnt;
        }
    }
}

void KGCSquareEnterWndS6::SetSquareKind(int iKind_)
{
    switch( (KSquareInfo::SQUARE_TYPE)iKind_ )
    {
    case KSquareInfo::ST_GUILD:
        m_bIsGuild = true;
        break;	
    case KSquareInfo::ST_NORMAL:
    default:
        m_bIsGuild = false;		
    }	
    SiGCSquare()->SetSquareType(iKind_);
}

void KGCSquareEnterWndS6::UpdateList()
{
    KP2P::GetInstance()->Send_SquareListReq( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID );
    m_dwUpdateFrame = 0;	
}

bool KGCSquareEnterWndS6::InitDialog( IN const KDialogInfo& kInfo_ )
{
    UpdateList();
    return true;
}