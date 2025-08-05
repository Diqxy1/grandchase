#include "stdafx.h"
#include ".\kgcpetmessage.h"
//
//
//



//



//

IMPLEMENT_CLASSNAME( KGCPetMessage );
IMPLEMENT_WND_FACTORY( KGCPetMessage );
IMPLEMENT_WND_FACTORY_NAME( KGCPetMessage, "gc_pet_message" );

#define MAX_ALPHA 255
#define DELTA_ALPHA 10
#define ALPHA_COUNT 20

KGCPetMessage::KGCPetMessage(void)
{
	m_pkWhoCallMe = NULL;

	m_iPetMsgShowTime = 0;
	m_iPetMsgShowTimeCount = 0;
	m_iSystemPetMsgTime = 0;

	m_iPosX = 0;
	m_iPosY = 0;
	m_iPetID = -1;

	m_iPetMentState = PET_MENT_INVALID;

	std::vector<int> temp;
	temp.clear();
	m_vPetAutoMsg[PET_MENT_INVALID].first = 0;
	m_vPetAutoMsg[PET_MENT_INVALID].second = temp;	//아무것도 없는 상태는 루아에서 로딩하지 않으므로 생성자에서 초기화 해 줍니다.

	LINK_CONTROL( "pet_say", m_pkPetMsgBox );	
	LINK_CONTROL( "petsay_static", m_pkPetMsg );
	LINK_CONTROL( "say", m_pkSay );

    m_iEvolutionLevel = 0;

    m_iAlpha = MAX_ALPHA;
}

KGCPetMessage::~KGCPetMessage(void)
{
	m_vPetAutoMsg.clear();	
}

void KGCPetMessage::OnCreate( void )
{
	m_pkPetMsgBox->InitState(true, true);
	m_pkSay->InitState(true, true);

	m_pkPetMsg->InitState(true, true);
	m_pkPetMsg->SetFontColor( 0xff000000 );
	m_pkPetMsg->SetMultiLine(true);
	m_pkPetMsg->SetLineSpace( 1.5f );	
}

void KGCPetMessage::PostChildDraw( void )
{

}

void KGCPetMessage::FrameMoveInEnabledState()
{
	if( m_pkWhoCallMe != NULL )
	{
		if( !m_pkWhoCallMe->IsRenderOn() )
		{
			ToggleRender(false);
		}
	}

    // 펫 메세지 박스 위에 마우스가 올라가면
    // 살짝 투명하게 해서 뒤의 내용을 보여준다
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x / m_fWindowScaleX, (float)pt.y  / m_fWindowScaleY);
    if( IsRenderOn() )
    {
        if( CheckPosInWindowBound( vMousePos ) )
        {
            if( m_iAlpha > MAX_ALPHA - ALPHA_COUNT * DELTA_ALPHA )
                m_iAlpha -= DELTA_ALPHA;
        }
        else
        {
            if( m_iAlpha < MAX_ALPHA )
                m_iAlpha += DELTA_ALPHA;
        }
        SetAllWindowColor( D3DCOLOR_ARGB( m_iAlpha, 0xff, 0xff, 0xff ) );
        m_pkPetMsg->SetFontColor( D3DCOLOR_ARGB( m_iAlpha, MAX_ALPHA - m_iAlpha, MAX_ALPHA - m_iAlpha, MAX_ALPHA - m_iAlpha ) );
	}
}

void KGCPetMessage::MsgFrameMove( bool bMsgSizing)
{
	//펫의 시스템 메시지를 던지기 위한 프레임 카운트
	++m_iSystemPetMsgTime;

	if( m_iSystemPetMsgTime > 10000 )
	{
		m_iSystemPetMsgTime = 0;
	}

	//메시지의 지속시간을 체크하여 시간이 지나면 메시지를 POP 해주는 부분
	if( m_iPetMsgShowTimeCount >= m_iPetMsgShowTime )
	{
		if( m_vPetMsg.size() > 0 )
		{
			SMessageInfo sMsgInfo = m_vPetMsg.front();

			m_iPetMsgShowTime = sMsgInfo.iShowTime;
			if( !bMsgSizing )
			{
				//가로사이즈를 고정하는 경우에는 세로 사이즈를 조절해 주어야 합니다.
				int iFontHeight = sMsgInfo.iBoxHeight - 12;
				for( int fixWidth = 0; ; ++fixWidth )
				{
					// 왜 140이지??? 
					if( sMsgInfo.iBoxWidth > 160 )
					{
						sMsgInfo.iBoxWidth -= 160;
						sMsgInfo.iBoxHeight += iFontHeight;
					}
					else
                    {
                        sMsgInfo.iBoxHeight += iFontHeight; // 마지막으로 하나 추가해야 맞다.
						break;
                    }
				}

				sMsgInfo.vSayPos.x = (float)( 160 / 4);
				sMsgInfo.vSayPos.y = (float)( sMsgInfo.iBoxHeight - 2);

				m_pkPetMsgBox->SetSize( 170, sMsgInfo.iBoxHeight );
                m_pkPetMsg->SetWidth( 160 );
				m_pkPetMsg->SetTextAutoMultiline( sMsgInfo.strMsg );
			}
			else
			{
				m_pkPetMsgBox->SetSize( sMsgInfo.iBoxWidth, sMsgInfo.iBoxHeight);
				m_pkPetMsg->SetWidth( sMsgInfo.iBoxWidth );						
				m_pkPetMsg->SetHeight( sMsgInfo.iBoxHeight );
				m_pkPetMsg->SetText ( sMsgInfo.strMsg );
			}
			
			m_pkSay->SetWindowPosDirect( sMsgInfo.vSayPos );

			m_vPetMsg.erase(m_vPetMsg.begin());

			ToggleRender(true);

			m_iPetMsgShowTimeCount = 0;
		}
		else
		{
			ToggleRender(false);
			m_iPetMsgShowTime = 0;
			m_iPetMsgShowTimeCount = 0;
		}		
	}
	else
		++m_iPetMsgShowTimeCount;		

	if( m_iPetMsgShowTime == 0 )
		m_iPetMsgShowTimeCount = 0;	

	//if(  g_MyD3D->m_kMyInfoPet->m_iHungryRateRemainTime < 600 && m_iPetID != -1)	//펫의 만복도가 10 Min 이하로 떨어졌다면...
	SCharInfo STemp = g_kGlobalValue.m_kUserInfo.GetCurrentChar();

	if( m_iPetID != -1 )	//펫의 정보가 없지 않다면,
	{
		//배고프다는 메시지를 20 초마다 한번씩 뿌려줍니다.
		if( m_vPetAutoMsg.find(m_iPetMentState) != m_vPetAutoMsg.end() 
			&& m_iPetMentState != PET_MENT_INVALID )
		{
			if( m_iSystemPetMsgTime % m_vPetAutoMsg[m_iPetMentState].first == 0 &&
				m_vPetAutoMsg[m_iPetMentState].second.size() != 0 )
			{
				int iMent = (int)rand() % (int)m_vPetAutoMsg[m_iPetMentState].second.size();
				PetMessaging(g_pkStrLoader->GetString(m_vPetAutoMsg[m_iPetMentState].second[iMent]), 160);
			}
		}
	}

	//애교만땅의 메시지를 가끔 출력해 주는 부분

}

//펫의 메세지를 설정하는 곳입니다.
//펫의 메세지들을 큐에 넣어두고 지속시간이 지나면 하나씩 보여줍니다.
void KGCPetMessage::PetMessaging( std::wstring strText_, int iShowTime/* = 300 */, bool bNow_ /*= true */)
{
	SMessageInfo sMsgInfo;

	GCDeviceFont* pkFontMgr;
	LONG lFontSize = m_pkPetMsg->GetFontSize();

	if (m_fWindowScaleY != 1.0f)
	{
        if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr;
        else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr14;
        else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr16;
        else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgrTitle;
        else 
            pkFontMgr = g_pkFontMgr;
	}
	else
	{
        if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr_original;
        else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr14_original;
        else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgr16_original;
        else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
            pkFontMgr = g_pkFontMgrTitle_original;
        else 
            pkFontMgr = g_pkFontMgr_original;
	}

	int iWidth = 0;
	int iHeight = pkFontMgr->GetHeight( strText_.c_str() );

	std::wstring strLineHeight = strText_;
	std::wstring strLongest;
	bool bMultiLine = false;
	int iLongest = 0;
	int iOldLongest = 0;

	//여러줄일 경우에는 Height를 계산해 주어야 한다.
	//여러줄일 경우에 줄중에 가장 긴줄을 기준으로 Width를 계산한다.
	while(true)
	{
		if( strLineHeight.find(L'\n') != std::wstring::npos )
		{
			bMultiLine = true;

			iLongest = (int)strLineHeight.find(L"\n");
			if( iLongest > iOldLongest )
			{
				iOldLongest = iLongest;
				strLongest = strLineHeight.substr(0, iLongest );
				iWidth = pkFontMgr->GetWidth( strLongest.c_str() ) + (int)(16 * m_fWindowScaleX);
			}

			strLineHeight.replace(0 ,strLineHeight.find(L'\n') + 1 , L"");
			iHeight += iHeight + 2;	
		}
		else
		{
			if( !bMultiLine )
				iWidth = pkFontMgr->GetWidth( strLineHeight.c_str() ) + (int)(16 * m_fWindowScaleX);
			else
			{
				iLongest = (int)strLineHeight.length();
				if( iOldLongest < iLongest )
					iWidth = pkFontMgr->GetWidth( strLineHeight.c_str() ) + (int)(16 * m_fWindowScaleX);
			}
			break;
		}
	}	

	//메시지 박스의 top과 buttom의 height만큼 더해 줘야함.
	iHeight += (int)(16 * m_fWindowScaleY);

	//말꼬리표의 위치를 잡아줍니다.
	D3DXVECTOR2 vPos;	
	vPos.x = (float)(iWidth / m_fWindowScaleX / 4);
	vPos.y = (float)(iHeight / m_fWindowScaleY - 2);

	//정보를 담습니다.
	sMsgInfo.strMsg		= strText_;
	sMsgInfo.iShowTime	= iShowTime;
	sMsgInfo.iBoxHeight = (int)(iHeight / m_fWindowScaleY);
	sMsgInfo.iBoxWidth	= (int)(iWidth  / m_fWindowScaleX);
	sMsgInfo.vSayPos	= vPos;
	
	//당장 출력해야 하는 것이라면 현재것을 종료시켜버리고 Vector의 제일 앞에 넣습니다.
	if( bNow_ )
	{
		m_vPetMsg.insert(m_vPetMsg.begin(), sMsgInfo);
		m_iPetMsgShowTime = 0;
		m_iPetMsgShowTimeCount = 0;
	}
	else
		m_vPetMsg.push_back(sMsgInfo);
}

void KGCPetMessage::PutStateMessage( int iState_ )
{
	if( m_vPetAutoMsg.find( iState_) == m_vPetAutoMsg.end() )
		return;

	if( m_vPetAutoMsg[iState_].second.size() == 0 )
		return;

	int iMent = (int)rand() % (int)m_vPetAutoMsg[ iState_].second.size();
	PetMessaging(g_pkStrLoader->GetString(m_vPetAutoMsg[ iState_].second[iMent]), 160);
}

void KGCPetMessage::SetMsgPos( int ix_, int iy_ )
{
	D3DXVECTOR2 wndpos;		

	//위치를 수정합니다.
	m_iPosX = ix_;
	m_iPosY = iy_;

	wndpos.x = (float)ix_;
	wndpos.y = (float)iy_;

	//수정된 위치를 적용합니다.	
	SetWindowPosDirect( wndpos );
}

void KGCPetMessage::SetPetID( int iPetID_, int iEvolutionLevel_ ) 
{ 
	if( m_iPetID == iPetID_ && m_iEvolutionLevel == iEvolutionLevel_ )
		return;

    m_iAlpha = MAX_ALPHA;

	m_vPetAutoMsg.clear();
	m_iPetMsgShowTimeCount = 0;
	m_iPetMsgShowTime = 0;

	m_iPetID = iPetID_; 	
    m_iEvolutionLevel = iEvolutionLevel_;
	LoadPetMent(); 
}

void KGCPetMessage::LoadPetMent()
{
	if( m_iPetID == -1 )
	{		
		m_vPetAutoMsg.clear();
		return;
	}

    const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate( m_iPetID, m_iEvolutionLevel );

    std::map<int, std::vector<SPetMent> >::const_iterator mit = sPetTemplate.mapMentList.begin();
    for( ; mit != sPetTemplate.mapMentList.end() ; ++mit ) {

        int iTableName = mit->first;
        int iMentTime = 500;
        std::vector<int> vecMent;
        BOOST_FOREACH( const SPetMent& val, mit->second ) {
            iMentTime = val.iTime;
            vecMent.push_back( val.iStringID );
        }
        m_vPetAutoMsg[iTableName].first = iMentTime + ( ( rand() + 1 ) % 1000 ) ;
        m_vPetAutoMsg[iTableName].second = vecMent;
    }
}

