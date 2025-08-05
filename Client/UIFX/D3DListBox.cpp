#include "stdafx.h"
#include "D3DListBox.h"
//
//


//
IMPLEMENT_CLASSNAME( KD3DListBox );

#include "GCIconImageManager.h"
#include "KGCEmoticon.h"

KD3DListBox::KD3DListBox( void )
{
    m_fLineSpace    = 1.0f;
	m_iImageIconSize = -1;
    m_uiMaxLine     = DEFAULT_MAX_LINE;
    SetTopIndex(0);
    m_iSelect       = -1;
    m_bShadow       = false;
    m_bChkAddstring = false;
    m_bCheckList    = true;
    m_dwShadowColor = 0xff000000;
    m_dwTransparent = 0xff000000; 
    m_iLineSpNumber = 0;

    Clear();

}

KD3DListBox::~KD3DListBox( void )
{
    Clear();
}

GCDeviceFont* KD3DListBox::GetFontMgr()
{
    if (m_eAlignType == WNDUIAT_EXPANTION && g_pGCDeviceManager->GetWindowScaleY() != 1.0f)
    {
        if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
            return IsDrawFontOutline() ? g_pkFontMgrOutline : g_pkFontMgr;
		else if(SiKGCMultipleLanguages()->GetSmallFontSize() == m_lFontSize) 
			return IsDrawFontOutline() ? g_pkFontMgrOutlineSmall      : g_pkFontMgrSmall;
        else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
            return IsDrawFontOutline() ? g_pkFontMgrOutline14      : g_pkFontMgr14;
        else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
            return IsDrawFontOutline() ? g_pkFontMgrOutline16      : g_pkFontMgr16;
        else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
            return IsDrawFontOutline() ? g_pkFontMgrOutlineTitle   : g_pkFontMgrTitle;
        else 
            return IsDrawFontOutline() ? g_pkFontMgrOutline : g_pkFontMgr;
    }
    else
    {
        //게임중간에 켜지는 UI인 경우-0-;
        if (m_fWindowScaleY != 1.0f)
        {
            if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline_mid_scale : g_pkFontMgr_mid_scale;
			else if(SiKGCMultipleLanguages()->GetSmallFontSize() == m_lFontSize) 
				return IsDrawFontOutline() ? g_pkFontMgrOutlineSmall_mid_scale      : g_pkFontMgrSmall_mid_scale;
            else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline14_mid_scale : g_pkFontMgr14_mid_scale;
            else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline16_mid_scale : g_pkFontMgr16_mid_scale;
            else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutlineTitle_mid_scale : g_pkFontMgrTitle_mid_scale;
            else 
                return IsDrawFontOutline() ? g_pkFontMgrOutline_mid_scale : g_pkFontMgr_mid_scale;
        }
        else
        {
            if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;
			else if(SiKGCMultipleLanguages()->GetSmallFontSize() == m_lFontSize) 
				return IsDrawFontOutline() ? g_pkFontMgrOutlineSmall_original      : g_pkFontMgrSmall_original;
            else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline14_original      : g_pkFontMgr14_original;
            else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutline16_original      : g_pkFontMgr16_original;
            else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
                return IsDrawFontOutline() ? g_pkFontMgrOutlineTitle_original   : g_pkFontMgrTitle_original;
            else 
                return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;
        }	
    }
}

void KD3DListBox::AddString( const std::wstring& strContent,
                            D3DCOLOR dwStrColor /* = 0xffffffff */,
                            bool bAction /* = false */,
                            DWORD lParam,
                            bool bIDGap,
                            std::wstring strReturnMsg,
                            int iMsgType,
							std::wstring strImageIcon
							) // = 0
{
    std::wstring            strIDGap = L"";
    std::wstring::size_type colonPos = 0;

	GCDeviceFont*           pkFontMgr = NULL;
    m_bChkAddstring=true;

	pkFontMgr=GetFontMgr();

    ASSERT( pkFontMgr != NULL );
    if(pkFontMgr == NULL) return;


    strIDGap.erase();
	
    // 아이디 길이를 확인해서 다음 줄부터 쓸 공간을 잡는다.
    if ( bIDGap && ( std::wstring::npos != ( colonPos = strContent.find( L':' ) ) ) && iMsgType != KGCChatManager::CHAT_TYPE_SCREEN_SHOT )
    {
        int iBlankWidth = pkFontMgr->GetWidth( L" " );
#if defined( _PORTUGUESE )
        if( iBlankWidth == 0 ) {
            iBlankWidth = 6;
        }
#endif
        int cnt = pkFontMgr->GetWidth( strContent.substr( 0, colonPos + 1 ).c_str() ) / iBlankWidth;

        strIDGap.insert( strIDGap.begin(), cnt, L' ' );
    }

	int iImageGap = 0;
	if (strImageIcon!=L"" && m_iImageIconSize!=-1)
	{
		iImageGap+=6 + m_iImageIconSize;
	}
 

	//이러고 싶지 않았지만... 태국어는 혼자 갈렵니다.
#if defined(_THAI_LANG)
	std::wstring strNoEmoticon;
	std::vector<KGCEmoticon*> vecEmoticons;
	strNoEmoticon.clear();
	SiKD3DEmoticonString()->clear();
	vecEmoticons = SiKD3DEmoticonString()->GetEmoticonDataVectorFromString( strContent, colonPos , strNoEmoticon);
	SiKD3DEmoticonString()->ArrangeForThaiWordWrap(strNoEmoticon, pkFontMgr,static_cast<int>( GetWidth() * g_pGCDeviceManager->GetWindowScaleX() ), strIDGap , vecEmoticons);
#else
	// [6/17/2008] breadceo. 워드랩!!??
	SiKD3DEmoticonString()->clear();
	SiKD3DEmoticonString()->DivideStringPiece( strContent, colonPos );
 	#if defined( NATION_USA ) || defined( NATION_IDN ) || defined(NATION_PHILIPPINE) || defined( NATION_EU )
 	SiKD3DEmoticonString()->ArrangeForWordWrap( pkFontMgr, static_cast<int>( GetWidth() * g_pGCDeviceManager->GetWindowScaleX() ), strIDGap );
 	#else
	SiKD3DEmoticonString()->Arrange( pkFontMgr, static_cast<int>( GetWidth() * g_pGCDeviceManager->GetWindowScaleX() ), strIDGap );
	#endif
#endif
    const std::vector< std::vector< KD3DEmoticonString::EmotStrPair > >& vecPieces = SiKD3DEmoticonString()->GetPieces();
    std::vector< std::vector< KD3DEmoticonString::EmotStrPair > >::const_iterator vit;
	
    for( vit = vecPieces.begin(); vit != vecPieces.end(); ++vit )
    {
        ADD_STRING_INFO sInfo( dwStrColor, bAction, (*vit), strReturnMsg, iMsgType, m_iLineSpNumber , strImageIcon);
        AddString_( sInfo );
    }

    // 최대로 저장하는 줄을 넘어서면 맨 위에 것들은 지워버린다.
    if ( m_deqStrContent.size() > m_uiMaxLine )
    {
        pop_front();
    }

    m_iTopIndex = GetBackNIndex( GetDisplayLimitLineInv() );
    if ( m_iTopIndex < 0 ) m_iTopIndex = 0;

    m_iLineSpNumber++;
    m_bChkAddstring=false;
}

void KD3DListBox::CalcTopIndex()
{
	m_iTopIndex = GetBackNIndex( GetDisplayLimitLineInv() );
	if ( m_iTopIndex < 0 ) m_iTopIndex = 0;
}

void KD3DListBox::SetTopIndex( int iIndex )
{
    m_iTopIndex = GetFrontNIndex(iIndex+1);
    //0입력?을경우, 맨앞에 것을 찾아야 한다 따라서 +1해줌..ㅡ.,ㅡ
}

// 
const std::wstring KD3DListBox::GetAllMsg( void )
{
    std::wstring strReturn = L"";

    for ( UINT i = 0; i < m_deqStrContent.size(); ++i )
    {
        std::vector< KD3DEmoticonString::EmotStrPair >& vecContent = m_deqStrContent[i];
        std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;
        for( vit = vecContent.begin(); vit != vecContent.end(); ++vit )
        {
            KD3DEmoticonString::EmotStrPair& prContent = (*vit);
            // 이모티콘이면 생까자 ㅎ 
            if( false == prContent.first )
            {
                strReturn += prContent.second.second;
            }
        }
    }

    return strReturn;
}

int KD3DListBox::GetFrontNIndex(int iNum) //앞에서 N번째에 출력한 글의 인덱스를 출력한다.
{
    int iPos=0;

    for (int i = 0; i < (int)m_deqStrContent.size(); i++)
    {
        if (m_bTypeFilter[m_deqMsgType[i]]==true)
        {
            iNum--;
        }
        if (iNum==0) 
        {
            iPos=i;
            break;
        }
    }
    return iPos;
}

int KD3DListBox::GetBackNIndex( int iNum ) //뒤에서 N번째에 출력한 글의 인덱스를 출력한다.
{
    int iPos=0;

    for ( int i = (int)m_deqStrContent.size()-1; i >= 0; i-- )
    {
        if (m_bTypeFilter[m_deqMsgType[i]]==true)
        {
            iNum--;
        }
        if (iNum==0) 
        {
            iPos=i;
            break;
        }
    }
    return iPos;
}

int KD3DListBox::GetViewContentSize() //실제 출력되는 문자의 크기만 찍는 함수
{
    int iCnt=0;
    for ( int i = 0; i < (int)m_deqStrContent.size(); i++ )
    {
        if (m_bTypeFilter[m_deqMsgType[i]]==true)
        {
            iCnt++;
        }
    }
    return iCnt;
}

void KD3DListBox::Clear( void )
{
    m_deqStrReturnMsg.clear();
    m_deqStrContent.clear();
    m_deqStrColor.clear();
    m_deqAction.clear();
    m_deqSplit.clear();
    m_deqMsgType.clear();
    m_deqEmoticon.clear();
	m_deqImageIcon.clear();

	m_iSelect = -1;

    SetTopIndex(0);
    for (int i=0;i<MAX_MSGTYPE;i++)
        m_bTypeFilter[i]=true;
}

void KD3DListBox::SetFilterInfo(int iType, bool bSet )// 필터링 설정
{
    ASSERT(iType >= 0 && iType<=MAX_MSGTYPE);
    m_bTypeFilter[iType]=bSet;
}

bool KD3DListBox::GetFilterInfo(int iType ) // 필터링 정보 받아오는 함수
{
    ASSERT(iType >= 0 && iType<=MAX_MSGTYPE);
    return m_bTypeFilter[iType];
}

void KD3DListBox::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    //D3DXVECTOR2 vMousePos( (float)pt.x / m_fWindowScaleX, (float)pt.y / m_fWindowScaleY);
	D3DXVECTOR2 vMousePosOriginal( (float)pt.x, (float)pt.y );
    BOOL bClick = g_pkInput->BtnUp( KInput::MBLEFT );
	BOOL bRClick = g_pkInput->BtnUp( KInput::MBRIGHT );

    m_iSelect = -1;
    if ( CheckPosInWindowBound( vMousePosOriginal ) )
    {
        UINT iDrawLineCnt=0;
        UINT uiMaxDisplayLine = GetDisplayLimitLine();
        for ( UINT i = m_iTopIndex; i < m_deqAction.size(); ++i )
        {
            if ( iDrawLineCnt >= uiMaxDisplayLine ) break;
            if ( m_bTypeFilter[m_deqMsgType[i]] == true )
            {
                if ( m_deqAction[i] && CheckList( vMousePosOriginal, iDrawLineCnt ) )
                {
                    m_iSelect = i;

                    if ( bClick )
                    {
                        SpeakToActionListener( KActionEvent( this, m_iSelect ) );
                    }
					else if( bRClick )
					{
						KActionEvent event( this, D3DWE_RBTN_SELECT, m_iSelect, 0 );
						SpeakToActionListener( event );
					}
                    break;
                }
                iDrawLineCnt++;
            }
        }

        if (g_pkInput->ISWheelUp())
        {
            SpeakToActionListener( KActionEvent( this, D3DWE_CHAT_WHEEL_UP ) );
        }
        else if (g_pkInput->ISWheelDown())
        {
            SpeakToActionListener( KActionEvent( this, D3DWE_CHAT_WHEEL_DOWN ) );
        }
    }
}


void KD3DListBox::PostDraw( void )
{

    int iUpLineCheck,iDownLineCheck;
	float fLineSpace = m_fLineSpace;

    if ( m_dwTransparent != 0x00000000 )
    {
        ASSERT( m_deqStrContent.size() == m_deqStrColor.size() &&
            m_deqStrContent.size() == m_deqAction.size() );

        UINT uiMaxDisplayLine = GetDisplayLimitLine();
        UINT iDrawLineCnt=0, iDrawEmoticonCnt = 0;
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        float fXPos = vPos.x;

        for ( UINT i = m_iTopIndex; i < m_deqStrContent.size(); ++i )
        {
            if( iDrawLineCnt >= uiMaxDisplayLine ) break;
            if( m_bTypeFilter[m_deqMsgType[i]] == false ) continue;

            bool bHasEmoticon = m_deqEmoticon[i];
            std::vector< KD3DEmoticonString::EmotStrPair >& vecContent = m_deqStrContent[i];
            std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;

			//이미지 아이콘을 출력하는 경우이고...
			if (m_deqImageIcon[i]!=L"" && m_iImageIconSize > 0)
			{
				//여러줄일경우 첫줄만 아이콘 그려준다.
				if (i == 0 || (m_deqSplit[i]!=m_deqSplit[i-1]) )
				{
					GCImageIconTexture* pTexture = SiKGCIconImageManager()->GetTexture(m_deqImageIcon[i]); 

					if( pTexture != NULL )
					{
						g_pGCDeviceManager2->DrawInScreen( pTexture, vPos.x + 3, vPos.y , 
							vPos.x + m_iImageIconSize , vPos.y + m_iImageIconSize , 0.0f, 0.0f, 1.0f, 1.0f );
					}
					vPos.x+=m_iImageIconSize + 6;
				}				
			}

            for( vit = vecContent.begin(); vit != vecContent.end(); ++vit )
            {
                KD3DEmoticonString::EmotStrPair& prContent = (*vit);
                // 이모티콘인 경우
                if( prContent.first == true )
                {
                    if( prContent.second.first != NULL )
                    {
                        DrawEmoticon_( prContent.second.first, vPos );
                    }
                }
                else
                {
                    D3DCOLOR color = m_deqStrColor[i];
                    color <<= 8;
                    color >>= 8;
                    DrawContent_( prContent.second.second, vPos, bHasEmoticon, color );
                }
            }

			//이모티콘 들어간 경우랑..아이콘 마크 들어간 경우.. Y가 더 큰경우 산출한다.
            vPos.y += GetDeltaY(bHasEmoticon);
            vPos.x = fXPos;
            iDrawLineCnt++;
            iDrawEmoticonCnt += ( bHasEmoticon ? 1 : 0 ); // 이모티콘을 그린 줄 수
        }

        if ( -1 == m_iSelect || m_bChkAddstring==true) return;

        //윗줄 아랫중 확인해서 길다랗게 여러줄로 나온거면 다 같이 표시해줍니다.
        KSafeArray<bool,MAX_DISPLAY_LINE> bChkDisPlayLine;
        iDrawLineCnt=0;

        for ( int i = m_iTopIndex; iDrawLineCnt< uiMaxDisplayLine && iDrawLineCnt < MAX_DISPLAY_LINE; i++)
        {
            if ( i < (int)m_deqSplit.size() )
            {   
                //if (m_deqMsgType[i]==5)
                if (m_bTypeFilter[m_deqMsgType[i]]==true)
                {
                    if (m_deqSplit[i]==m_deqSplit[m_iSelect])
                    {
                        bChkDisPlayLine[iDrawLineCnt]=true;
                    }
                    else
                    {
                        bChkDisPlayLine[iDrawLineCnt]=false;
                    }
                    iDrawLineCnt++; 
                }
            }
            else
            {
                //범위 초과한 경우
                bChkDisPlayLine[iDrawLineCnt]=false;
                iDrawLineCnt++; 
            }
        }

        bool bChk=false;
        iUpLineCheck=-1;
        iDownLineCheck=uiMaxDisplayLine-1;
        //바뀐위치를 찾는다.
        for (int i = 0; i < (int)uiMaxDisplayLine; i++ )
        {
            if (bChkDisPlayLine[i]!=bChk)
            {
                if (iUpLineCheck==-1)
                {
                    iUpLineCheck=i;
                    bChk=true;
                }
                else
                {
                    iDownLineCheck=i-1;
                    break;
                }
            }
        }

		g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );

        int iUpEmoticonLineCnt = GetEmoticonLineCnt( m_iTopIndex, m_iTopIndex + iUpLineCheck );
        int iDownEmoticonLineCnt = GetEmoticonLineCnt( m_iTopIndex, m_iTopIndex + iDownLineCheck );

        float fUpLine, fDownLine, fLineGap=1;

		if (m_iImageIconSize>0 && m_iImageIconSize > EMOTICON_UI_SIZE )
		{
			fUpLine = float(iUpLineCheck * m_iImageIconSize);
			fDownLine = float(iDownLineCheck * m_iImageIconSize);
		}
		else
		{
			if (m_iImageIconSize> 0)
			{
				fLineGap = (float)m_iImageIconSize;
			}
			else
			{
				fLineGap = float( GetFontMgr()->GetHeight() ) * fLineSpace;
			}
			fUpLine = float( iUpEmoticonLineCnt * EMOTICON_UI_SIZE ) + 
				float( iUpLineCheck - iUpEmoticonLineCnt ) * fLineGap;
			fDownLine = float( iDownEmoticonLineCnt * EMOTICON_UI_SIZE ) + 
				float( iDownLineCheck - iDownEmoticonLineCnt ) * fLineGap;
		}

		 int iHeight = 0;

		 if( m_deqEmoticon.size() != 0 )
			iHeight = (int)GetDeltaY(m_deqEmoticon[ m_iSelect ], false);
		 else
			iHeight = GetFontMgr()->GetHeight();

		      
		vPos = GetCurrentWindowPos();
		vPos = GetScaledVector(vPos);
        m_select[0].m_vPos.x = (float)int( vPos.x );             m_select[0].m_vPos.y = (float)int( vPos.y + fUpLine );
        m_select[1].m_vPos.x = (float)int( vPos.x + GetWidth() * m_fWindowScaleX); m_select[1].m_vPos.y = (float)int( vPos.y + fUpLine );
        m_select[2].m_vPos.x = (float)int( vPos.x );             m_select[2].m_vPos.y = (float)int( vPos.y + fDownLine + iHeight );
        m_select[3].m_vPos.x = (float)int( vPos.x + GetWidth()  * m_fWindowScaleX); m_select[3].m_vPos.y = (float)int( vPos.y + fDownLine + iHeight );

		g_pGCDeviceManager2->DrawInScreen(NULL,m_select);

        g_pGCDeviceManager2->PopState();
    }
}

float KD3DListBox::GetDeltaY(bool bHasEmoticon, bool bLinespace_)
{
	float fDeltaY1 = 0,fDeltaY2 = 0;
	//이모티콘 들어간 경우랑..아이콘 마크 들어간 경우.. Y가 더 큰경우 산출한다.

	if( bLinespace_ )
		fDeltaY1 = ( bHasEmoticon ? float(EMOTICON_UI_SIZE) : ( float( GetFontMgr()->GetHeight() ) * m_fLineSpace ) );
	else
		fDeltaY1 = ( bHasEmoticon ? float(EMOTICON_UI_SIZE) : float( GetFontMgr()->GetHeight() ));

	//fDeltaY1 = ( bHasEmoticon ? float(EMOTICON_UI_SIZE) : GetFontMgr()->GetHeight(L"테스트"));

	if (m_iImageIconSize!=-1)
	{
		fDeltaY2 = (float)m_iImageIconSize;
	}

	if (fDeltaY1<fDeltaY2)
	{
		fDeltaY1 = fDeltaY2;
	}
	return  fDeltaY1;
}

UINT KD3DListBox::GetDisplayLimitLine( void )
{
    UINT uiRet = 0;
    float fHeight = 0.0f;
    for ( UINT i = m_iTopIndex; i < m_deqStrContent.size(); ++i )
    {
        if( m_bTypeFilter[m_deqMsgType[i]] == false ) continue;
        bool bHasEmoticon = m_deqEmoticon[i];

        fHeight += GetDeltaY(bHasEmoticon);

        if( float( GetHeight() ) * m_fWindowScaleY <= fHeight ) 
            break;

        uiRet++;
    }

    return uiRet;
}

// 그냥 할때는 TopIndex부터 따지면 되지만 추가하는 경우에는 가장뒤에서부터 따져야 옳다
UINT KD3DListBox::GetDisplayLimitLineInv( void )
{
    UINT uiRet = 0;

    if( m_deqStrContent.empty() )
        return uiRet;

    float fHeight = 0.0f;
    for ( int i = (int)m_deqStrContent.size() - 1; i >= 0; --i )
    {
        if( m_bTypeFilter[m_deqMsgType[i]] == false ) continue;
        bool bHasEmoticon = m_deqEmoticon[i];

        fHeight += ( bHasEmoticon ? EMOTICON_UI_SIZE : ( float( GetFontMgr()->GetHeight() ) * m_fLineSpace ) );

        if( float( GetHeight() ) * m_fWindowScaleY <= fHeight ) 
            break;

        uiRet++;
    }

    return uiRet;
}

bool KD3DListBox::CheckList( const D3DXVECTOR2& vPos, int iIndex )
{
    D3DXVECTOR2 vOrigin = GetRelocatedWindowPos();
    float fLine = 0.0f;
    float fHeight = 0.0f;

    for( int i = 0; i < iIndex; ++i )
    {
		fLine += GetDeltaY(m_deqEmoticon[ m_iTopIndex + i ]);        
    }

	fHeight = GetDeltaY(m_deqEmoticon[ m_iTopIndex + iIndex], false);    

    return ( vOrigin.x <= vPos.x && vPos.x < vOrigin.x + GetWidth() * m_fWindowScaleX) &&
           ( vOrigin.y + fLine <= vPos.y && vPos.y < vOrigin.y + fLine + fHeight * m_fWindowScaleY && m_bCheckList);
}

const std::wstring& KD3DListBox::GetReturnMsg( int iIndex ) const
{
    static std::wstring strEmpty;

    if ( iIndex < 0 || iIndex >= (int)m_deqStrReturnMsg.size() ) 
        return strEmpty;

    return m_deqStrReturnMsg[iIndex];
}

void KD3DListBox::AddString_( ADD_STRING_INFO& sInfo )
{
    m_deqStrColor.push_back( sInfo.dwStrColor );
    m_deqAction.push_back( sInfo.bAction );
	m_deqImageIcon.push_back(sInfo.strImageIcon);
    m_deqStrContent.push_back( sInfo.vecContent );
    m_deqStrReturnMsg.push_back( sInfo.strReturnMsg );
    m_deqMsgType.push_back( sInfo.iMsgType );
    m_deqSplit.push_back( sInfo.iLineSpNumber );

    bool bHasEmot = false;
    std::vector< KD3DEmoticonString::EmotStrPair >::const_iterator cvit;
    for( cvit = sInfo.vecContent.begin(); cvit != sInfo.vecContent.end(); ++cvit )
    {
        const KD3DEmoticonString::EmotStrPair& prEmot = (*cvit);
        if( prEmot.first == true && prEmot.second.first != NULL )
        {
            bHasEmot = true;
            break;
        }
    }
    m_deqEmoticon.push_back( bHasEmot );
}

void KD3DListBox::pop_front()
{
    m_deqStrReturnMsg.pop_front();
    m_deqStrContent.pop_front();
    m_deqStrColor.pop_front();
    m_deqAction.pop_front();
    m_deqSplit.pop_front();
    m_deqMsgType.pop_front();
    m_deqEmoticon.pop_front();
	m_deqImageIcon.pop_front();
}

void KD3DListBox::DrawContent_( const std::wstring& strContent, D3DXVECTOR2& vPos, bool bHasEmoticon, DWORD dwColor )
{
    GCDeviceFont* pkFontMgr;

	pkFontMgr = GetFontMgr();

    int iYTerm = ( bHasEmoticon ? static_cast<int>( EMOTICON_UI_SIZE - GetFontMgr()->GetHeight() )  : 0 );

	//젠장 이모티콘 때문에 돌아버릴정도로 복잡해진다..-_-;
	//같이 쓸일이 없을꺼 같긴 하지만.. 난중에 같이 쓰게 되면 오지게 예외 처리 더 찾아서 해줘야 될듯...

	if (m_iImageIconSize >0)
	{
		iYTerm = int((float)(m_iImageIconSize* m_fWindowScaleY - GetFontMgr()->GetHeight()) / 2.0f );
	}

    // draw shadow
    if ( m_bShadow )
    {
        D3DCOLOR color = m_dwShadowColor;
        color <<= 8;
        color >>= 8;

        pkFontMgr->OutTextXY( int( vPos.x ) + 1, int( vPos.y ) + 1 + iYTerm,
            strContent.c_str(), color | m_dwTransparent, NULL, DT_LEFT|DT_TOP|DT_NO_COLOR );
    }


    // draw font
    pkFontMgr->OutTextXY( int( vPos.x ), int( vPos.y + iYTerm ), strContent.c_str(), dwColor | m_dwTransparent );

    vPos.x += (float)pkFontMgr->GetWidth( strContent.c_str() );
}

void KD3DListBox::DrawEmoticon_( KGCEmoticon* pEmoticon, D3DXVECTOR2& vPos )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    pEmoticon->RenderForChatbox( vPos.x, vPos.y, vPos.x + EMOTICON_UI_SIZE, vPos.y + EMOTICON_UI_SIZE );

    if( vMousePos.x >= vPos.x && vMousePos.x <= vPos.x + EMOTICON_UI_SIZE )
    {
        if( vMousePos.y >= vPos.y && vMousePos.y <= vPos.y + EMOTICON_UI_SIZE )
        {
            std::wstring strTooltip;
#if defined(CHAT_EMOTICON_EMPTY_ROLLOVER)
			strTooltip = L""; //태국은 이모티콘 롤오버시에 텍스트를 보여주지 않는다. 
#else
            strTooltip += EMOTICON_SEPRATOR;
            strTooltip += g_pkStrLoader->GetString( pEmoticon->m_iStrID );
            strTooltip += EMOTICON_SEPRATOR;
#endif

            bool bLeft = false;
            D3DXVECTOR2 vOrigin = GetCurrentWindowPos();
            int iTooltipWidth = g_pkFontMgr->GetWidth( strTooltip.c_str() );
            if( int( vPos.x ) + EMOTICON_UI_SIZE + iTooltipWidth >= vOrigin.x + (int)GetWidth() )
            {
                bLeft = true;
            }
            int TooltipXPos = 0;
            if( bLeft == false )
            {
                TooltipXPos = int( vPos.x ) + EMOTICON_UI_SIZE;
            }
            else
            {
                TooltipXPos = int( vPos.x ) - iTooltipWidth;
            }

            g_pkFontMgr->OutTextXY( TooltipXPos, int( vPos.y ) + 5, strTooltip.c_str(), 0xffffffff );
        }
    }

    vPos.x += (float)EMOTICON_UI_SIZE;
}

int KD3DListBox::GetEmoticonLineCnt( int iStart, int iEnd )
{
    if( iStart == iEnd )
        return 0;

    ASSERT( 0 <= iStart && iStart < (int)m_deqEmoticon.size() );
    ASSERT( 0 <= iEnd && iEnd < (int)m_deqEmoticon.size() );

    int iRet = 0;
    for( int i = iStart; i < iEnd; ++i )
    {
        if( m_deqEmoticon[i] == true )
        {
            iRet++;
        }
    }

    return iRet;
}
