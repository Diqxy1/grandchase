#include "stdafx.h"
#include "D3DStatic.h"

extern std::vector<bool> BreakStringTH(std::wstring strOriginalstring);
//
IMPLEMENT_CLASSNAME( KD3DStatic );

KD3DStatic::KD3DStatic( void ) : m_vOffset( 0.0f, 0.0f )
#if defined( STATIC_WORDWRAP )
, m_bWordWrap( true )
#else
, m_bWordWrap( false )
#endif
, m_bAutoScroll( false )
, m_iScrollFrame( 0 )
, m_iScrollFrameCount( 0 )
, m_iScrollSpeed( 10 )
, m_iLineSize( 0 )
, m_bAutoMultiLine( false )
, m_fFontSpace( 0 )
{
	m_strText.erase();
	m_bShadow       = false;
	m_bMultiLine    = false;
	m_fLineSpace    = 1.0f;
	m_dwShadowColor = 0xff000000;
	m_dwAlign       = DT_LEFT;
	m_iLineNum      = 0;
	m_iAutoMultiLineLimit = -1;
	m_strIconImage = L"";
    m_pkIconTexture = NULL;
    SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
}

KD3DStatic::~KD3DStatic( void )
{
	m_strText.erase();
}

void KD3DStatic::PostDraw( void )
{
	if ( m_strText.empty() ) return;

	D3DXVECTOR2 vPos( GetCurrentWindowPos() + m_vOffset );
	GCDeviceFont* pkFontMgr = GetFontMgr();


	float fLineSpace = m_fLineSpace;
	RECT rt;
	DWORD dw,dh;

	dw = GetWidth();
	dh = GetHeight();

    if( m_pkIconTexture != NULL )
    {
        float fImageDy = float(SIZE_IMAGEICON - dh);
        g_pGCDeviceManager2->DrawInScreen( m_pkIconTexture
            ,vPos.x * m_fWindowScaleX
            ,(vPos.y - fImageDy) * m_fWindowScaleY
            ,(vPos.x + SIZE_IMAGEICON) * m_fWindowScaleX
            ,(vPos.y - fImageDy + SIZE_IMAGEICON) * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );

        vPos.x+= SIZE_IMAGEICON;
        //vPos.y+= SIZE_IMAGEICON;
    }

	dw = (DWORD)(dw * m_fWindowScaleX);
	dh = (DWORD)(dh * m_fWindowScaleY);

	vPos = GetScaledVector(vPos);

	rt.left = (DWORD)vPos.x;
	rt.right = rt.left + dw;
	rt.top = (DWORD)vPos.y;
	rt.bottom = rt.top + dh;

//본섭이 아닐경우에 인덱스코드 변수가 비어있으면 wstr에 텍스트를 입력하고, 인덱스 값이 들어있으면 인덱스 코드를 출력하도록 한다. 
#if !defined( __PATH__)
    //본섭이 아닐경우에 시프트키를 누르면 인덱스코드 찾아오는 함수
    ShowStringID();

	wstring wstr;
	if(m_strStringID.empty())
	{
		wstr = m_strText;
	}else{
		wstr = m_strStringID;
	}
#else
	wstring& wstr = m_strText;
#endif

	if ( m_bMultiLine )
	{
		if ( m_bShadow )
		{
			pkFontMgr->OutTextMultiline( (int)(vPos.x + 1), (int)(vPos.y + 1), wstr.c_str(), m_dwShadowColor, fLineSpace * m_fWindowScaleY, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign) | DT_NO_COLOR , 0xff000000 , m_fWindowScaleY, m_fFontSpace );
		}

		if( IsDrawFontOutline() )
		{
			pkFontMgr->OutTextMultiline( (int)vPos.x, (int)vPos.y, wstr.c_str(), m_dwFontColor, fLineSpace * m_fWindowScaleY, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign), m_dwFontOutlineColor , m_fWindowScaleY, m_fFontSpace );
		}
		else
		{
			pkFontMgr->OutTextMultiline( (int)vPos.x, (int)vPos.y, wstr.c_str(), m_dwFontColor, fLineSpace * m_fWindowScaleY, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign) ,0xff000000, m_fWindowScaleY, m_fFontSpace );
		}
	}
	else
	{
		if ( m_bShadow )
		{
            pkFontMgr->OutTextXY((int)(vPos.x + 1), (int)(vPos.y + 1), wstr.c_str(), m_dwShadowColor, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign) | DT_NO_COLOR, 0xff000000, GCDeviceFontBase::OUTLINE|GCDeviceFontBase::INLINE, m_fFontSpace );
		}

		if( IsDrawFontOutline() )
		{
    		pkFontMgr->OutTextXY( (int)vPos.x, (int)vPos.y, wstr.c_str(), m_dwFontColor, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign), m_dwFontOutlineColor, GCDeviceFontBase::OUTLINE|GCDeviceFontBase::INLINE, m_fFontSpace );
		}
		else
		{
			pkFontMgr->OutTextXY( (int)vPos.x, (int)vPos.y, wstr.c_str(), m_dwFontColor, &rt, (IsAutoScrollEnable()?DT_LEFT:m_dwAlign), 0xff000000, GCDeviceFontBase::OUTLINE|GCDeviceFontBase::INLINE, m_fFontSpace );
		}
	}
}

void KD3DStatic::SetText( int iText )
{
	m_bAutoMultiLine = false;
	WCHAR strText[MAX_PATH];
	swprintf(strText, MAX_PATH, L"%d",iText);
	m_strOriginal = strText;
	m_strText = strText;
    m_iLineNum = 1;
}


void KD3DStatic::SetTextAutoMultiline( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ )
{
    FrameMoveInDestroyState();

    m_bAutoMultiLine    =   true;
    m_bMultiLine        =   true;
    m_strOriginal       =   strText;

    m_strText.erase();
    if( strText.empty() ){
        m_iLineNum = 0;
        return;
    }

#if defined(STATIC_THAI_WORDWRAP)
    this->WordWrapTHAI( strText, bChkSpeicalChar, bSpaceCharTrim_ );
#else
    if( m_bWordWrap ){
        this->WordWrapNormal( strText, bChkSpeicalChar, bSpaceCharTrim_ );
    }else{
        this->WordWrapDoNotUse( strText, bChkSpeicalChar, bSpaceCharTrim_ );
    }
#endif
}


//------------------------------------------------------------------------
//  WordWrap~블라블라~()
//  워드랩 목적에 따라서 private함수로 분기
//  여기서 부터
//------------------------------------------------------------------------
void KD3DStatic::WordWrapTHAI( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ )
{
#if defined(NATION_THAILAND) // BreakStringTH() 이 함수를 링크 할수 없어서 컴파일 에러가 난다. 해서 국가 디파인
    GCDeviceFont* pkFontMgr = GetFontMgr();

    // 멀티라인이므로 스태틱의 넓이만큼 쓰고 다음줄로 넘긴다.
    // 적당히 스페이스 1칸 정도 여유를 주자.
    int iWidthLimit = static_cast<int>(GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ));

    if (m_strIconImage!=L""){
        iWidthLimit-=SIZE_IMAGEICON;
    }

    CString src = strText.c_str();

    //특수 문자 변형 처리 해줄경우
    if ( bChkSpeicalChar ){
        src.Replace( L"\\n", L"\n" );
        src.Replace( L'\\', L'\n' ); // sort파일에서 아이템 설명글은 '\'가 개행문자다
    }

    std::wstring strConvert( src );
    std::wstring strSplit;   
    strSplit.reserve( strText.size() + 10 );	//줄바꿈 문자를 예상해서 사이즈보다 10 많게 Reserve.
    strSplit.erase();

    m_iLineNum = 0;		//생성자에서만 초기화 해주고 있네요? 문자열을 갱신할 때도 초기화 해 줍시다.
    if ( strConvert.size() != 0 )
        m_iLineNum++;

    int iLineStart = 0;
    int iLastSpace = 0;



    int cutpoint;
    int iPt = 0;
    std::vector<bool> vecbCut = BreakStringTH(strConvert);

    cutpoint=0;
    for ( std::wstring::size_type i = 0; i < strConvert.size(); i++ )
    {
        if (vecbCut[i]==true) //Break Point 찍기 편하라고 쪼갬-_-;
        {
            cutpoint=i;
        }

        if ((i - cutpoint) > 10 ) //10글짜 이상 차이나면 그냥 출력 되도록 하심
        {
            cutpoint=i;
        }

        if ( iPt == 0 && ( strConvert[i] == L' ' || strConvert[i] == L'\t' ) )
            continue;

        if ( strConvert[i] == L'\n' )
        {
            m_strText += strConvert.substr(iPt,i-iPt);
            iPt=i;
            m_iLineNum++;
            continue;
        }

        if ( pkFontMgr->GetWidth( strConvert.substr(iPt,i-iPt+1).c_str() ) > iWidthLimit )
        {
            m_strText += strConvert.substr(iPt,cutpoint-iPt)+L'\n';
            iPt=cutpoint;
            m_iLineNum++;
        }
    }

    if ( (strConvert.size() - iPt) !=0)
    {
        m_strText += strConvert.substr(iPt,strConvert.size()-iPt);
    }

    //마지막에 줄바꿈 문자 들어가는 경우 발생되어, 예외처리 추가
    if (m_strText[m_strText.length()-1]=='\n')
    {
        m_strText=m_strText.substr(0,m_strText.length()-1);
        m_iLineNum--;
    }
#endif
}


void KD3DStatic::WordWrapNormal( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ )
{
    GCDeviceFont* pkFontMgr = GetFontMgr();

    // 멀티라인이므로 스태틱의 넓이만큼 쓰고 다음줄로 넘긴다.
    // 적당히 스페이스 1칸 정도 여유를 주자.
    int iWidthLimit = static_cast<int>(GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ));

    if (m_strIconImage!=L""){
        iWidthLimit-=SIZE_IMAGEICON;
    }

    CString src = strText.c_str();

    //특수 문자 변형 처리 해줄경우
    if ( bChkSpeicalChar ){
        src.Replace( L"\\n", L"\n" );
        src.Replace( L'\\', L'\n' ); // sort파일에서 아이템 설명글은 '\'가 개행문자다
    }

    std::wstring strConvert( src );
    std::wstring strSplit;   
    strSplit.reserve( strText.size() + 10 );	//줄바꿈 문자를 예상해서 사이즈보다 10 많게 Reserve.
    strSplit.erase();

    m_iLineNum = 0;		//생성자에서만 초기화 해주고 있네요? 문자열을 갱신할 때도 초기화 해 줍시다.
    if ( strConvert.size() != 0 )
        m_iLineNum++;

    int iLineStart = 0;
    int iLastSpace = 0;

    for( std::wstring::size_type i = 0; i < (int)strConvert.size(); i++ )
    {
        //마지막 문자이면 마지막 문자까지 적용시켜 줘야죠~
        if( i == strConvert.size() - 1 )
        {
            m_strText.append( strConvert.substr( iLineStart, i - iLineStart + 1) );
            break;
        }

        if (m_iAutoMultiLineLimit!=-1 && m_iAutoMultiLineLimit<m_iLineNum)
        {
            m_strText=m_strText.substr(0,m_strText.size()-2)+L"…";
            break;
        }

        //공백문자면 마지막 공백 문자위치를 체크 해 둡니다. for Wordwrap
        if ( strSplit.empty() == true && ( strConvert[i] == L' ' || strConvert[i] == L'\t' ) )
        {
            iLastSpace = i;
            continue;
        }

        //줄바꿈 문자면 줄 시작 위치를 갱신하면서 줄을 바꿔 줍니다.
        if ( strConvert[i] == L'\n' )
        {
            m_strText.append( strConvert.substr( iLineStart, i - iLineStart ) + L'\n' );
            iLineStart = i + 1;
            m_iLineNum++;
            continue;
        }

        //UI의 폭을 넘어섰다면, Wordwrap을 위해서 줄시작에서 마지막 공백문자까지의 위치로 줄을 바꿔줍시다.
        if ( pkFontMgr->GetWidth( strConvert.substr( iLineStart, i - iLineStart + 1).c_str() ) <= iWidthLimit )
            continue;

        //UI의 폭이 우연스레 너무 좁아서 글자가 하나도 못들어갈 폭이라면?? Assert 한방 날려줍니다.
        if( i - iLineStart == 0 )
        {
            ASSERT( !"UI가 너무 좁아요!!!" );
            continue;
        }
        //그런데!! 라인에서 공백문자가 한번도 없었다면???????? WordWap을 사용하지 않고 _로 이어줍니다.
        else if( ( iLineStart >= iLastSpace || i - iLastSpace > 20 ) && m_bWordWrap )
        {
            m_strText.append( strConvert.substr( iLineStart, i - iLineStart + 1) + L"_\n" );
            iLineStart = i + 1;
        }
        else
        {
            m_strText.append( strConvert.substr( iLineStart, (m_bWordWrap?iLastSpace - iLineStart:i - iLineStart) + 1) + L'\n' );
            iLineStart = (m_bWordWrap?iLastSpace:i) + 1;
        }
        m_iLineNum++;
    }

}

void KD3DStatic::WordWrapDoNotUse( const std::wstring& strText , bool bChkSpeicalChar, bool bSpaceCharTrim_ )
{
    GCDeviceFont* pkFontMgr = GetFontMgr();

    // 멀티라인이므로 스태틱의 넓이만큼 쓰고 다음줄로 넘긴다.
    // 적당히 스페이스 1칸 정도 여유를 주자.
    int iWidthLimit = static_cast<int>(GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ));

    if (m_strIconImage!=L""){
        iWidthLimit-=SIZE_IMAGEICON;
    }

    CString src = strText.c_str();

    //특수 문자 변형 처리 해줄경우
    if ( bChkSpeicalChar ){
        src.Replace( L"\\n", L"\n" );
        src.Replace( L'\\', L'\n' ); // sort파일에서 아이템 설명글은 '\'가 개행문자다
    }

    std::wstring strConvert( src );
    std::wstring strSplit;   
    strSplit.reserve( strText.size() + 10 );	//줄바꿈 문자를 예상해서 사이즈보다 10 많게 Reserve.
    strSplit.erase();

    m_iLineNum = 0;		//생성자에서만 초기화 해주고 있네요? 문자열을 갱신할 때도 초기화 해 줍시다.
    if ( strConvert.size() != 0 )
        m_iLineNum++;

    int iLineStart = 0;
    int iLastSpace = 0;

    for ( std::wstring::size_type i = 0; i < strConvert.size(); i++ )
    {
        if ( bSpaceCharTrim_ && strSplit.empty() == true && ( strConvert[i] == L' ' || strConvert[i] == L'\t' ) )
            continue;
        strSplit += strConvert[i];
        if ( strConvert[i] == L'\n' )
        {
            m_strText += strSplit;
            strSplit.erase();
            m_iLineNum++;
            continue;
        }

        if ( pkFontMgr->GetWidth( strSplit.c_str() ) > iWidthLimit )
        {
            if( strConvert[i] == L'#' && (strConvert[i+1] == L'c'||strConvert[i+1] == L'C') )
            {
                if( strConvert[i+2] == L'x' || strConvert[i+2] == L'X' )
                {
                    strSplit += strConvert.substr( i+1, 2 );
                    i += 2;
                }
                else
                {
                    if( i + 8 < strConvert.size() )
                    {
                        strSplit += strConvert.substr( i+1, 7 );
                        i += 7;
                    }
                }
            }
            else
            {
                strSplit += L'\n';
                m_strText += strSplit;
                strSplit.erase();
                m_iLineNum++;
            }
        }
    }

    if ( false == strSplit.empty() )
    {
        m_strText += strSplit;
    }

    //마지막에 줄바꿈 문자 들어가는 경우 발생되어, 예외처리 추가
    if (m_strText[m_strText.length()-1]=='\n')
    {
        m_strText=m_strText.substr(0,m_strText.length()-1);
        m_iLineNum--;
    }
}
//------------------------------------------------------------------------
//  WordWrap~블라블라~()
//  워드랩 목적에 따라서 private함수로 분기
//  여기 까지
//------------------------------------------------------------------------


void KD3DStatic::SetTextPtr( const wchar_t* strText )
{
    SetText( strText );
}

void KD3DStatic::SetText( const std::string& strText )
{
    SetText( KncUtil::toWideString( strText ) );
}

void KD3DStatic::SetText( const std::wstring& strText )
{
	m_bAutoMultiLine = false;
	m_strOriginal = strText;

    if( strText.empty() )
        m_iLineNum = 0;
    else
        m_iLineNum = 1;

	//m_iStringID = g_pkStrLoader->FindID( &strText );

#if defined( STATIC_WORDWRAP ) || defined(STATIC_THAI_WORDWRAP)
    if ( m_bWordWrap )
	{
		GCDeviceFont* pkFontMgr = GetFontMgr();
		std::wstring strOrigin = strText;
		std::wstring strConvert;
		int iWidthLimit = (int)(GetWidth() * g_pGCDeviceManager->GetWindowScaleX()) - pkFontMgr->GetWidth( L' ' );
		int iLineStart = 0;
		bool bIgnore = false;

		for( int i = 0 ; i < (int)strOrigin.size() && iWidthLimit > 0 ; ++i )
		{
			//마지막 문자이면 마지막 문자까지 적용시켜 줘야죠~
			if( i == strOrigin.size() - 1 && bIgnore == false )
			{
				strConvert.append( strOrigin.substr( iLineStart, i - iLineStart + 1) );
				break;
			}

			//줄바꿈 문자면 문자열의 시작점을 수정합니다.
			if ( strOrigin[i] == L'\n' )
			{
				m_bMultiLine = true;
				if( bIgnore == true )
				{
					strConvert.append( L"\n" );
				}
				else
				{
					strConvert.append( strOrigin.substr( iLineStart, i - iLineStart ) + L'\n' );
				}
				iLineStart = i + 1;
				bIgnore = false;
				continue;
			}

			if ( bIgnore == true )
				continue;

			if ( pkFontMgr->GetWidth( strOrigin.substr( iLineStart, i - iLineStart + 1 ).c_str() ) >= iWidthLimit && i > 3 )
			{
				strConvert.append( strOrigin.substr(iLineStart, (i - iLineStart + 1) - 3 ).append(L"…" ) );
				iLineStart = i+ 1;
				bIgnore = true;
				continue;
			}
		}
		if( iWidthLimit > 0 )
		{
			m_strText = strConvert;
		}
		else
		{
			m_strText = strOrigin;
		}
	}
    else
    {
        m_strText = strText;
    }
#else
	{
		m_strText = strText;
	}
#endif
}
void KD3DStatic::SetText()
{
	m_bAutoMultiLine = false;
	m_strText.clear();
	m_strOriginal.clear();
    m_iLineNum = 0;
}

GCDeviceFont* KD3DStatic::GetFontMgr()
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

GCDeviceFont* KD3DStatic::GetFontMgr( const int& iFontSize_ )
{
	if(SiKGCMultipleLanguages()->GetDefualtFontSize() == iFontSize_) 
		return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;
	else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == iFontSize_) 
		return IsDrawFontOutline() ? g_pkFontMgrOutline14_original      : g_pkFontMgr14_original;
	else if(SiKGCMultipleLanguages()->GetLargeFontSize() == iFontSize_) 
		return IsDrawFontOutline() ? g_pkFontMgrOutline16_original      : g_pkFontMgr16_original;
	else if(SiKGCMultipleLanguages()->GetTitleFontSize() == iFontSize_) 
		return IsDrawFontOutline() ? g_pkFontMgrOutlineTitle_original   : g_pkFontMgrTitle_original;

	return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;

}

void KD3DStatic::Create( bool bAuto )
{
	KD3DWnd::Create( bAuto );
	//SetWidth(GetWidth()*1.5f);
	//SetHeight(GetHeight()*1.5f);
	    
}

void KD3DStatic::SetWidth( DWORD dwWidth )
{
	KD3DWnd::SetWidth( dwWidth );
	if( m_bAutoMultiLine )
		SetTextAutoMultiline( m_strOriginal );
	else
		SetText( m_strOriginal );
}

void KD3DStatic::SetWidthDirect( DWORD dwWidth )
{
	KD3DWnd::SetWidthDirect( dwWidth );
	if( m_bAutoMultiLine )
		SetTextAutoMultiline( m_strOriginal );
	else
		SetText( m_strOriginal );
}

void KD3DStatic::FrameMoveInEnabledState()
{
	FrameMoveAutoScroll();
}

void KD3DStatic::FrameMoveAutoScroll()
{
	if( IsAutoScrollEnable() == false )
		return;

	GCDeviceFont* pkFontMgr = GetFontMgr();
	int iWidthLimit = static_cast<int>( GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ) );
	if( ++m_iScrollFrameCount > m_iScrollSpeed )
	{
		m_iScrollFrameCount = 0;
		++m_iScrollFrame;
	}

	m_iLineSize = 0;
	for( int i = 0 ; i < (int)m_strOriginal.size() ; ++i )
	{
		if( pkFontMgr->GetWidth( m_strOriginal.substr(0, i).c_str() ) <= iWidthLimit )
			continue;

		m_iLineSize = i - 1;
		break;
	}

	if( m_iLineSize == 0 )
		return;

	if( m_iScrollFrame >= (int)m_strOriginal.size() )
		m_iScrollFrame = -1 * m_iLineSize;

	std::wstring strScroll;
	if( m_iScrollFrame >= 0 )
	{
		strScroll = m_strOriginal.substr( m_iScrollFrame, m_iLineSize );
	}
	else
	{
		if (m_iLineSize + m_iScrollFrame > 0)
		{
			strScroll.append( GetSpaceFromWidth( iWidthLimit - pkFontMgr->GetWidth( m_strOriginal.substr( 0, m_iLineSize + m_iScrollFrame ).c_str() ) ) );
			strScroll.append( m_strOriginal.substr( 0, m_iLineSize + m_iScrollFrame ) );

		}	
	}

	m_strText = strScroll;
}

bool KD3DStatic::IsAutoScrollEnable()
{
	if( m_bAutoScroll == false )
		return false;

	//오토스크롤이 활성화 되어 있으면 UI의 폭에 맞춰서 폭이 넘쳐나는 녀석들을 옆으로 슬슬슬 밀어 줍니다.
	//\n 과 같은 문자열이 있으면 기능이 활성화 되지 않는 것으로 하겠습니다.
	if( m_strOriginal.find( '\n' ) != m_strOriginal.npos )
		return false;

	GCDeviceFont* pkFontMgr = GetFontMgr();
	int iWidthLimit = static_cast<int>( GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ) );
	if( pkFontMgr->GetWidth( m_strOriginal.c_str() ) <= iWidthLimit )
		return false;

	return true;
}

std::wstring KD3DStatic::GetSpaceFromWidth( DWORD dwWidth )
{
    //DWORD dwSpaceWidth = GetFontMgr()->GetWidth( L' ' );

    //   if( dwSpaceWidth == 0 ) { 
    //       return std::wstring();
    //   }

    //std::wstring strSpace;
    //DWORD dwSpaceCount = dwWidth / dwSpaceWidth;
    //   
    //   if( dwSpaceCount == 0 ) { 
    //       return std::wstring();
    //   }

    //strSpace.reserve( dwSpaceCount );
    //for( int i = 0 ; i < static_cast<int>( dwSpaceCount ) ; ++i )
    //{
    //	strSpace +=  L' ';
    //}
    //return strSpace;

    DWORD dwSpaceWidth = GetFontMgr()->GetWidth( L' ' );
    if( dwSpaceWidth == 0 ) { 
        return std::wstring();
    }

    DWORD dwSpaceCount = dwWidth / dwSpaceWidth;
    if( dwSpaceCount == 0 ) { 
        return std::wstring();
    }

    std::wostringstream strmSpace;
    for( int i = 0 ; i < static_cast<int>( dwSpaceCount ) ; ++i )
    {
        strmSpace << L' ';
    }
    return strmSpace.str();
}

void KD3DStatic::SetForceWordWrapText( const std::wstring& strText, bool bWordWrap )
{
	if( false == bWordWrap )
	{
		SetText( strText );
		return;
	}

	m_bAutoMultiLine = false;
	m_strOriginal = strText;
	GCDeviceFont* pkFontMgr = GetFontMgr();
	std::wstring strOrigin = strText;
	std::wstring strConvert;
	int iWidthLimit = static_cast<int>( GetWidth()*m_fWindowScaleX - pkFontMgr->GetWidth( L' ' ) );
	int iLineStart = 0;
	bool bIgnore = false;

	for( int i = 0 ; i < (int)strOrigin.size() && iWidthLimit > 0 ; ++i )
	{
		//마지막 문자이면 마지막 문자까지 적용시켜 줘야죠~
		if( i == strOrigin.size() - 1 && bIgnore == false )
		{
			strConvert.append( strOrigin.substr( iLineStart, i - iLineStart + 1) );
			break;
		}

		//줄바꿈 문자면 문자열의 시작점을 수정합니다.
		if ( strOrigin[i] == L'\n' )
		{
			m_bMultiLine = true;
			if( bIgnore == true )
			{
				strConvert.append( L"\n" );
			}
			else
			{
				strConvert.append( strOrigin.substr( iLineStart, i - iLineStart ) + L'\n' );
			}
			iLineStart = i + 1;
			bIgnore = false;
			continue;
		}

		if ( bIgnore == true )
			continue;

		if ( pkFontMgr->GetWidth( strOrigin.substr( iLineStart, i - iLineStart + 1 ).c_str() ) >= iWidthLimit && i > 3 )
		{
			strConvert.append( strOrigin.substr(iLineStart, (i - iLineStart + 1) - 3 ).append(L"…" ) );
			iLineStart = i+ 1;
			bIgnore = true;
			continue;
		}
	}
	if( iWidthLimit > 0 )
	{
		m_strText = strConvert;
	}
	else
	{
		m_strText = strOrigin;
	}
	
}

//시프트키가 눌러졌을 경우 텍스트 대신 인덱스코드를 가지고 올지를 결정함.
//본섭은 적용되지 않음. 

#if !defined( __PATH__ )

void KD3DStatic::ShowStringID() 
{
	if(GetAsyncKeyState(VK_CONTROL))
	{
		if( m_strStringID.empty() == false )
			return;

		wchar_t temp[MAX_PATH] = {0,};
		_itow_s( g_pkStrLoader->FindID(&m_strText), temp, MAX_PATH, 10 );
		m_strStringID = temp;

		if( L"-1" == m_strStringID ){
			m_strStringID.clear();
			for( size_t i=0; i<m_strWndName.size(); ++i ){
				m_strStringID += static_cast<wchar_t>(m_strWndName[i]);
			}
		}
	}
	else
	{
		if( m_strStringID.empty() == true )
			return;

		m_strStringID.clear();
	}
}
#endif

void KD3DStatic::MakeAutoScrollString()
{
	if( IsAutoScrollEnable() == false )
		return;

	GCDeviceFont* pkFontMgr = GetFontMgr();
	int iWidthLimit = static_cast<int>( GetWidth() * m_fWindowScaleX ) - pkFontMgr->GetWidth( L' ' );
	
	m_iLineSize = 0;
	for( int i = 0 ; i < (int)m_strOriginal.size() ; ++i )
	{
		if( pkFontMgr->GetWidth( m_strOriginal.substr(0, i).c_str() ) <= iWidthLimit )
			continue;

		m_iLineSize = i - 1;
		break;
	}

	if( m_iLineSize == 0 )
		return;

	if( m_iScrollFrame >= (int)m_strOriginal.size() )
		m_iScrollFrame = -1 * m_iLineSize;

	std::wstring strScroll;
	if( m_iScrollFrame >= 0 )
	{
		strScroll = m_strOriginal.substr( m_iScrollFrame, m_iLineSize );
	}
	else
	{
		if ((m_iLineSize + m_iScrollFrame)>0) //이 경우가 발생해서 크러쉬가 발생된다.
		{
			strScroll.append( GetSpaceFromWidth( iWidthLimit - pkFontMgr->GetWidth( m_strOriginal.substr( 0, m_iLineSize + m_iScrollFrame ).c_str() ) ) );
			strScroll.append( m_strOriginal.substr( 0, m_iLineSize + m_iScrollFrame ) );
		}
	}

	m_strText = strScroll;
}

void KD3DStatic::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KD3DStatic>( L, "KD3DStatic" );
    lua_tinker::class_def<KD3DStatic>( L,  "SetText",               &KD3DStatic::SetTextPtr );
    lua_tinker::class_def<KD3DStatic>( L,  "SetTextID",             &KD3DStatic::SetTextFromID );
    lua_tinker::class_def<KD3DStatic>( L,  "GetOriginalText",       &KD3DStatic::GetOriginalTextPtr );
    lua_tinker::class_def<KD3DStatic>( L,  "GetText",               &KD3DStatic::GetTextPtr );
    lua_tinker::class_def<KD3DStatic>( L,  "SetShadow",             &KD3DStatic::SetShadow );
    lua_tinker::class_def<KD3DStatic>( L,  "SetShadowColor",        &KD3DStatic::SetShadowColor );
    lua_tinker::class_def<KD3DStatic>( L,  "SetMultiLine",          &KD3DStatic::SetMultiLine );
    lua_tinker::class_def<KD3DStatic>( L,  "SetWordWrap",           &KD3DStatic::SetWordWrap );
    lua_tinker::class_def<KD3DStatic>( L,  "SetLineSpace",          &KD3DStatic::SetLineSpace );
    lua_tinker::class_def<KD3DStatic>( L,  "SetAutoScroll",         &KD3DStatic::SetAutoScroll );
    lua_tinker::class_def<KD3DStatic>( L,  "SetScrollSpeed",        &KD3DStatic::SetScrollSpeed );
    lua_tinker::class_def<KD3DStatic>( L,  "SetAutoMultiLineLimit", &KD3DStatic::SetAutoMultiLineLimit );
    lua_tinker::class_def<KD3DStatic>( L,  "GetLineSpace",          &KD3DStatic::GetLineSpace );
    lua_tinker::class_def<KD3DStatic>( L,  "SetOffset",             &KD3DStatic::SetOffset );
    lua_tinker::class_def<KD3DStatic>( L,  "GetOffset",             &KD3DStatic::GetOffset );
    lua_tinker::class_def<KD3DStatic>( L,  "SetAlign",              &KD3DStatic::SetAlign );
    lua_tinker::class_def<KD3DStatic>( L,  "SetNumber",             &KD3DStatic::SetNumber );
    lua_tinker::class_inh<KD3DStatic,KD3DWnd>( L );
}

void KD3DStatic::SetTextFromID( int iID )
{
    SetText( g_pkStrLoader->GetString( iID ) );
}

void KD3DStatic::PushThis(lua_State* L)
{
    lua_tinker::push( L, this );
}

void KD3DStatic::SetNumber( const int iNum_ )
{
    std::wstringstream stm;
    stm<<iNum_;
    SetText( stm.str() );
}

void KD3DStatic::SetIconImage( std::wstring wstr )
{
    m_strIconImage = wstr;

    if( wstr.empty() ) {
        SAFE_RELEASE( m_pkIconTexture );
    }
    else {
        m_pkIconTexture = SiKGCIconImageManager()->GetTexture(m_strIconImage);
    }
}

POINT KD3DStatic::GetStringWidthAndHeightInPixel(bool bSetSize_ /*= true*/, int iFontSize/* = 0*/)
{
	GCDeviceFont* pkFontMgr;
    if( iFontSize )
        pkFontMgr = GetFontMgr( iFontSize );
    else
        pkFontMgr = GetFontMgr(SiKGCMultipleLanguages()->GetDefualtFontSize());

	int iWidth = 0;
	int iHeight = pkFontMgr->GetHeight( m_strText.c_str() );

	std::wstring strLongest;
	bool bMultiLine = false;
	int iLongest = 0;
	int iOldLongest = 0;


	std::wstring temStr = m_strText;
	int iTotalHeight = iHeight;
	while(true)
	{
		if( temStr.find(L'\n') != std::wstring::npos )
		{
			bMultiLine = true;

			iLongest = (int)temStr.find(L"\n");
			if( iLongest > iOldLongest )
			{
				iOldLongest = iLongest;
				strLongest = temStr.substr(0, iLongest );
				iWidth = pkFontMgr->GetWidth( strLongest.c_str() );
			}

			temStr.replace(0 ,temStr.find(L'\n') + 1 , L"");
			iTotalHeight += iHeight + (2 * static_cast<int>(GetLineSpace()));
		}
		else
		{
			if( !bMultiLine )
				iWidth = pkFontMgr->GetWidth( temStr.c_str() );
			else
			{
				iLongest = (int)temStr.length();
				if( iOldLongest < iLongest )
					iWidth = pkFontMgr->GetWidth( temStr.c_str() );
			}
			break;
		}
	}

	if (bSetSize_)
	{
		SetWidthDirect(iWidth);
		SetHeightDirect(iTotalHeight);
	}
	
	POINT stWidthHeight;
	stWidthHeight.x = iWidth;
	stWidthHeight.y = iTotalHeight;

	return stWidthHeight;
	
}
