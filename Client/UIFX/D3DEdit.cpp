#include "stdafx.h"
#include "D3DEdit.h"
//
//
//
#include "../GCUI/KGCCandidateWnd.h"


//
#include "Procedure.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
//

IMPLEMENT_CLASSNAME( KD3DEdit );


static const KD3DUIVertex g_orgCursor[4] =
{
    KD3DUIVertex( D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f ),
                  D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ),
                  D3DXVECTOR2( 0.0f, 0.0f ) ),
    KD3DUIVertex( D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ),
                  D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ),
                  D3DXVECTOR2( 1.0f, 0.0f ) ),
    KD3DUIVertex( D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ),
                  D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ),
                  D3DXVECTOR2( 0.0f, 1.0f ) ),
    KD3DUIVertex( D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f ),
                  D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ),
                  D3DXVECTOR2( 1.0f, 1.0f ) ),
};

LRESULT CALLBACK KD3DEdit::EditCtrlProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return reinterpret_cast<KD3DEdit*>(
           ::GetWindowLong( hWnd, GWL_USERDATA ) )->D3DEditProc( uMsg, wParam, lParam );
}

KD3DEdit::KD3DEdit( void )
{
	m_pkFontMgrLimtPixelSize = NULL;
    m_fpOrgEditProc = NULL;
    m_hWndEdit      = NULL;
    m_iOffset_X       = 0;
    m_iOffset_Y       = 0;

    m_bHasFocus     = false;
    m_bIsPasswd     = false;

    m_iStart        = 0;
    m_iEnd          = 0;
    m_iLimitSize    = 0;
	m_iLimitLine	= 0;
    m_iLimitByte    = 0;
	m_iType = EDIT_ALL_TYPE;

    ::ZeroMemory( m_strEdit, sizeof( m_strEdit ) );
    ::ZeroMemory( m_strPassWord, sizeof( m_strPassWord ) );
    ::memcpy( m_cursor, g_orgCursor, 4 * sizeof( KD3DUIVertex ) );

    CreateEditCtrl();

    #if defined( NATION_TAIWAN ) || defined(NATION_KOREA)
	SetLimitText( 60 );
    #else
	SetLimitText( MAX_PATH );
    #endif

    m_bSelfFocusCheck = true;

	m_bMultyLineMode = false;
	m_bShift = false;
	m_bAllowNewLine = false;
	m_iTexCount = 0;
	m_iLimitLine_Y = 3;
	m_eSelectIndex = NOT_DETERMINE;
    m_iSelectAutoCreatte = -1;
}

KD3DEdit::~KD3DEdit( void )
{
    DestroyEditCtrl();
}

void KD3DEdit::FrameMoveInEnabledState( void )
{
	ASSERT( g_pkInput != NULL );

	D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if ( m_bSelfFocusCheck && CheckPosInWindowBound( vMousePos ) && g_pkInput->BtnDown( KInput::MBLEFT ) )
	{
		::SetFocus( m_hWndEdit );
	}

	::ZeroMemory( m_strEdit, sizeof( m_strEdit ) );

#ifdef NATION_CHINA
	GetWindowTextW( m_hWndEdit, m_strEdit, MAX_PATH_CHINA );
#else
    if( !CheckLimiteTextByByte() )
    {
	GetWindowTextW( m_hWndEdit, m_strEdit, MAX_PATH );
    }

#endif

	DeleteColorCode();
}

void KD3DEdit::SetPasswd( bool bPasswd, WCHAR wcPassChar )
{
	m_bIsPasswd = bPasswd;
	m_wcPassChar = wcPassChar;
}
const WCHAR* KD3DEdit::GetText( bool bCheckLimit_ )
{
    if ( bCheckLimit_ == false )
        return m_strEdit;
    GCDeviceFont* pkFontMgr = GetFontMgr();

    for ( int i = (int)wcslen( m_strEdit ) - 1; i > 0; i-- )
    {
        if ( pkFontMgr->GetWidth( m_strEdit ) >= (int)m_iLitmitPixelSize )
            m_strEdit[i] = L'\0';
        else
            break;
    }
    return m_strEdit;
}

void KD3DEdit::PostDraw( void )
{
	GCDeviceFont* pkFontMgr = GetFontMgr();

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

	//switch(m_eAlignType)
	//{
	//case WNDUIAT_EXPANTION:
	//	vPos.x *= m_fWindowScaleX;
	//	vPos.y *= m_fWindowScaleY;		
	//	break;
	//case WNDUIAT_POSRIGHTBASE:
	//	//오른쪽에 밀착형으로 처리
	//	vPos.x = GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-vPos.x) * m_fWindowScaleX;
	//	vPos.y *= g_pGCDeviceManager->GetWindowScaleY();		
	//	vPos.y +=m_vGapPos.y;
	//	break;
	//case WNDUIAT_POSRIGHTTOPBASE:
	//	vPos.x = GC_SCREEN_WIDTH*g_pGCDeviceManager->GetWindowScaleX()-(GC_SCREEN_WIDTH-vPos.x) * m_fWindowScaleX;
	//	vPos.y +=m_vGapPos.y;
	//	break;
	//case WNDUIAT_POSZEROBASE:
	//	//그냥 이동만 한다.
	//	vPos.x *= m_fWindowScaleX;
	//	vPos.y *= m_fWindowScaleY;
	//	vPos +=m_vGapPos;
	//	break;
	//case WNDUIAT_POSBOTTOMBASE:
	//	vPos.x *= g_pGCDeviceManager->GetWindowScaleX();
	//	vPos.y = GC_SCREEN_HEIGHT*g_pGCDeviceManager->GetWindowScaleY()-(GC_SCREEN_HEIGHT-vPos.y) * m_fWindowScaleY;
	//	vPos.x +=m_vGapPos.x;
	//	break;
	//}	

    // 바이트 단위로 글자제한 검사
    CheckLimiteTextByByte();

	//병음 입력기 2003에서 뻗는 것 방지. - 중국.
	if ( m_iStart > MAX_PATH - 1)
	{
		m_iStart = MAX_PATH - 1;
	}

	WCHAR text[MAX_PATH] = { 0, };

    WCHAR* pStrTemp = NULL;
    if ( m_bIsPasswd == true )
        pStrTemp = &m_strPassWord[m_iOffset_X];
    else
        pStrTemp = &m_strEdit[m_iOffset_X];

	//Composition String 임시로 포함시키기
	if( wcslen( m_Ime.GetComp() ) > 0 )
	{
		WCHAR temp1[MAX_PATH] = {0,};
		WCHAR temp2[MAX_PATH] = {0,};

		wcscpy( temp1, pStrTemp );
		temp1[m_iStart] = L'\0';

		wcscpy( temp2, &pStrTemp[m_iStart] );

		swprintf( pStrTemp, MAX_PATH, L"%s%s%s", temp1, m_Ime.GetComp(), temp2 );
	}

    else if( m_bMultyLineMode )
    {
        // 멀티라인 시 따로 스크롤바 처리를 하지 않기 때문에 Offset은 항상 0으로 고정시킵니다.
        m_iOffset_X = 0;

        if ( m_bAllowNewLine )
        {
            m_iOffset_Y = 0;
            // 엔터가 허용되는 멀티라인이라면, UI높이만큼만 보이게 수직 스크롤이 되어야 한다.
            int iStrLen = (int)wcslen(m_strEdit);
            int iCurLine = 0;

            // 업이나 다운 안눌렀을때를 위해서 -_-;;
            iCurLine = GetLineByIndex( GetCurCursorIndex() );

            while ( (iCurLine - m_iOffset_Y) >= m_iLimitLine_Y )
            {
                ++m_iOffset_Y;
            }

            int iLine = 0;
            int iSize = (int)wcslen( m_strEdit );
            for ( int i = 0; i < iSize - 1; i++ )
            {
                if( m_strEdit[i] == L'\n' )
                {
                    if ( ++iLine > m_iOffset_Y )
                        break;
                    else if ( iLine == m_iOffset_Y )
                        m_iOffset_X = i + 1;
                }
            }
        }
    }
	else
	{
		if ( pkFontMgr->GetWidth( pStrTemp ) > (int)GetWidth() * m_fWindowScaleX)
		{
			int iOff = 0;
			while ( pkFontMgr->GetWidth( &pStrTemp[iOff] ) > (int)GetWidth() *m_fWindowScaleX)
			{
				iOff++;
			}
			m_iOffset_X += iOff;
		}
	}

	//아래 부등호 코드에 막힐 경우를 대비해 static변수를 만들어 쓴다.
	static int iRealOffset = 0;
	iRealOffset = m_iOffset_X;

    if( !m_bAllowNewLine && m_iOffset_X > m_iStart )
	{
		m_iOffset_X = m_iStart;
	}

	if ( iRealOffset > (int)wcslen(m_strEdit) - 1 )
	{
		//만일에 뻥날 경우를 대비하여..걸리진 않았다만.
		iRealOffset = m_iOffset_X;
	}

    m_iTexCount = (int)wcslen( &m_strEdit[iRealOffset] );

    if ( false == m_bAllowNewLine )
    {
        if ( pkFontMgr->GetWidth( &m_strEdit[iRealOffset] ) > (int)GetWidth() * m_fWindowScaleX )
        {
            DWORD dwLength = 0;
            m_iTexCount = 0;
            while( dwLength < GetWidth() * m_fWindowScaleX )
            {
                WCHAR strTemp = m_strEdit[iRealOffset + m_iTexCount];
                dwLength += pkFontMgr->GetWidth( strTemp );
                m_iTexCount++;
            }
            m_iTexCount--;
        }
    }
    else
    {
        m_iTexCount = 0;
        int iLine = 0;
        int iSize = (int)wcslen( m_strEdit );
        for ( int i = iRealOffset; i < iSize; i++ )
        {
            if( m_strEdit[i] == L'\n' )
            {
                if ( ++iLine >= m_iLimitLine_Y || ( m_iLimitLine > 0 && m_iLimitLine < iLine ))
                    break;
            }

            m_iTexCount++;
        }
    }

	//text에는 조합문자를 제외한 하얀색만 넣는다.(노란색은 따로 출력하기 위해.)
	m_iTexCount -= (int)(wcslen( m_Ime.GetComp() ));

	//맥스라인보다 작을 대만 멀티라인을 허용하고, 맥스라인이 설정안되어 있어도 멀티라인을 허용한다.
	if(m_bMultyLineMode )
	{
		// 멀티라인인 경우 D3DEdit 의 가로 너비에 따라서 개행 문자를 넣어주는 부분이다.
		WCHAR strEdit[MAX_PATH] = {0,};
		if( m_pkFontMgrLimtPixelSize )
		{
			if( m_bIsPasswd == true )
				wcsncpy(strEdit, m_strPassWord, MAX_PATH);
			else
				wcsncpy(strEdit, m_strEdit, MAX_PATH);
			int iCharCnt = 0;

			if( m_pkFontMgrLimtPixelSize->GetWidth(m_strEdit) > (int)GetWidth() )
			{
				// 현재 컨트롤의 문자열 최대길이는 MAX_PATH 이다.
				int iStrCnt = wcslen(m_strEdit);
				if( iStrCnt > MAX_PATH -1)
					iStrCnt = MAX_PATH - 1;

				WCHAR wstrTemp[MAX_PATH] = {0,};
				int iTempCnt = 0;

				// Offset은 어차피 0부터 이므로 처음부터 계산한다.
				for( int i = 0; i < (int)iStrCnt; ++i, ++iCharCnt, ++iTempCnt )
				{
					if( (iCharCnt >= MAX_PATH -1) || (iTempCnt >= MAX_PATH -1) )
						break;

                    if( m_strEdit[i] == L'\n' )
                    {
                        iTempCnt = -1;
                        wcsset(wstrTemp, 0);
                        strEdit[iCharCnt] = L'\n';
						//++iCharCnt;
                        continue;
                    }
                    else if( m_pkFontMgrLimtPixelSize->GetWidth( wstrTemp ) > (int)GetWidth() )
					{
                        PushNewLineAtIndex( i );
 
						iTempCnt = 0;
						wcsset(wstrTemp, 0);
						strEdit[iCharCnt] = L'\n';
						++iCharCnt;

						if( iCharCnt >= MAX_PATH - 1 )
							break;
					}

					wstrTemp[iTempCnt] = m_strEdit[i];
					strEdit[iCharCnt] = m_strEdit[i];
				}
			}
		}

		//wcsncpy_s(text, MAX_PATH, strEdit, MAX_PATH - 1);
        StringCchCopyNW( text, MAX_PATH, &m_strEdit[iRealOffset], m_iTexCount );
	}
	else
	{
		StringCchCopyNW( text, MAX_PATH, &m_strEdit[iRealOffset], m_iTexCount );
	}

	//에디트의 문자열 출력	
    if ( m_bIsPasswd == true )
    {
        ZeroMemory( m_strPassWord, sizeof( m_strPassWord ) );
		int iTexLen = (int)wcslen( text );

		if(iTexLen > MAX_PATH - 1)
			iTexLen = MAX_PATH - 1;

		int i;
        for ( i = 0; i < iTexLen; i++ )
        {
            m_strPassWord[i] = m_wcPassChar;
        }
        m_strPassWord[i] = L'\0';
        pkFontMgr->OutTextMultiline( (int)vPos.x, (int)vPos.y, m_strPassWord, m_dwFontColor , m_fWindowScaleY , 0, 0|0, -1677216, m_fWindowScaleY);
    }
    else
    {
        pkFontMgr->OutTextMultiline( (int)vPos.x, (int)vPos.y, text, m_dwFontColor , m_fWindowScaleY , 0, 0|0, -1677216, m_fWindowScaleY);
    }

	//Composition String 출력 (노랗게 보일려고)
	if( wcslen( m_Ime.GetComp() ) > 0 )
	{		
		pkFontMgr->OutTextMultiline( int(vPos.x) + pkFontMgr->GetWidth(text), (int)vPos.y, m_Ime.GetComp(), 0xffffff00 , m_fWindowScaleY , 0, 0|0, -1677216, m_fWindowScaleY);
	}

	//Candidate Window 세팅
	if( m_Ime.GetIMEState() == CANDS_DO && g_pkUIScene->m_pkCandidateWnd->IsRenderOn() )
	{
		float fPosX = vPos.x + (pkFontMgr->GetWidth(text) - pkFontMgr->GetWidth(&text[m_iStart]));
		float fPosY = vPos.y;

#ifdef NATION_JAPAN
		g_pkUIScene->m_pkCandidateWnd->SetCurrCandidate( m_Ime.GetCandSel() % MAX_CANDIDATE_SIZE );
#endif
		g_pkUIScene->m_pkCandidateWnd->SetCurrCandidate( m_Ime.GetCandSel() - m_Ime.GetCandPageStart() );
		g_pkUIScene->m_pkCandidateWnd->SetPosition( D3DXVECTOR2(fPosX, fPosY),  static_cast<float>(pkFontMgr->GetHeight()) );
		g_pkUIScene->m_pkCandidateWnd->FrameMove();
	}

    if ( m_bHasFocus )
    {
		SendMessage( m_hWndEdit, EM_GETSEL, (WPARAM)&m_iStart, (LPARAM)&m_iEnd );

        if ( m_iStart == m_iEnd )
        {
            float fTime = m_fTime * 2.0f; //깜박거리게 한다.
            if ( fTime - (int)fTime < 0.5f )
            {
                DrawCursor( vPos, m_iOffset_X );
            }
        }
        else
        {
            DrawCursor( vPos, m_iOffset_X, true ); //어떤 영역 드로우 되었을 때
        }
    }
}

void KD3DEdit::OnActivate( void )
{
    if ( IsActive() )
    {
        ::SetFocus( m_hWndEdit );
    }
    else
    {
        ::SetFocus( g_hUIMainWnd );
    }
}

void KD3DEdit::SetFocus( bool bIsFocus)
{
    if ( bIsFocus )
    {
        ::SetFocus( m_hWndEdit );
    }
    else
    {
        ::SetFocus( g_hUIMainWnd );
    }
}

void KD3DEdit::DrawCursor( const D3DXVECTOR2& vCursorPos, int iOffset, bool bSelectDraw )
{
	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );

    WCHAR txt[1000] = {0,};
    WCHAR txt2[1000] = {0,};
    int width = 0;
    D3DXMATRIX mScale, mTrans, mTotal;

    GCDeviceFont* pkFontMgr = GetFontMgr();

    if ( bSelectDraw )
    {
        // TODO : 셀렉션 도중 캐럿위치가 변경되면 디스플레이 오프셋도 바꿔줘야 한다.

        
		// 각 줄별 선택 범위 인덱스를 나눠서 뽑아온다.
		std::vector<POINT> vtStartEndList = GetLineSelectionList(m_iStart, m_iEnd);

		for(int k=0;k<(int)vtStartEndList.size();++k)
		{
			int iSelectWidth = vtStartEndList[k].y - vtStartEndList[k].x;		// m_iEnd - m_iStart
			if( iSelectWidth < 0 )
				iSelectWidth = 0;
			int iSelectPos = vtStartEndList[k].x - iOffset;						// m_iStart - iOffset
			if ( iSelectPos < 0 ) 
				iSelectPos = 0;

			int iCurLine = 0;

            if ( m_bAllowNewLine )
            {
                iSelectPos = vtStartEndList[k].x;
                iCurLine = GetLineByIndex( iSelectPos, m_iOffset_X );
            }
			else if( m_bMultyLineMode )
				iCurLine = GetLineByIndex( iSelectPos );							// 해당 인덱스가 몇번째 라인인지 얻어온다.

            int iWidth = 0;
			/*for(int i=0;i<iSelectWidth;++i)
			{*/
			
			if ( m_bIsPasswd == true )
			{
				wcsncpy( txt, &m_strPassWord[iSelectPos], iSelectWidth );
			}
			else
			{
				wcsncpy( txt, &m_strEdit[iSelectPos], iSelectWidth );
				//iWidth += pkFontMgr->GetWidth(m_strEdit[iSelectPos + iSelectWidth]);
			}   
			iWidth += pkFontMgr->GetWidth(txt);
			/*}*/
	        width = GetLeftSideWidthByIndex(vtStartEndList[k].x);

			//한줄의 범위를 넘어가는 경우 처리.
			if ((iWidth + width) > GetWidth() * m_fWindowScaleX)
			{
				iWidth = (int)(GetWidth() * m_fWindowScaleX) - width;
			}

			::D3DXMatrixScaling( &mScale, (float)iWidth, (float)pkFontMgr->GetHeight() , 1.0f );

			::D3DXMatrixTranslation( &mTrans, vCursorPos.x + width , vCursorPos.y + iCurLine * pkFontMgr->GetFontSize(), 0.0f );
			::D3DXMatrixMultiply( &mTotal, &mScale, &mTrans );

			for ( UINT i = 0; i < 4; ++i )
			{
				::D3DXVec2Transform( &m_cursor[i].m_vPos, (const D3DXVECTOR2*)( &g_orgCursor[i].m_vPos ), &mTotal );
			}
			g_pGCDeviceManager2->DrawInScreen(NULL,m_cursor);
		}
    }
    else
    {
        if ( wcslen(m_Ime.GetComp()) > 0 && m_bIsPasswd == false )
        {
            if( (m_iStart - iOffset - 1) >= 0 )
                wcsncpy( txt, &m_strEdit[iOffset], (m_iStart - iOffset - 1) );

			//조합 중이라면 커서를 맨 마지막으로 옮기자.
#ifdef NATION_CHINA
			width = pkFontMgr->GetWidth(m_strEdit);
#else
			width = pkFontMgr->GetWidth(txt);
			width += (int)wcslen(m_Ime.GetComp()) * m_lFontSize;
#endif

			if ( GetWidth() * m_fWindowScaleX< (DWORD)width )
				width = static_cast<int>( GetWidth() * m_fWindowScaleX );

            // draw quad
            ::D3DXMatrixScaling( &mScale, 2.0f, (float)pkFontMgr->GetHeight() + 2.0f * m_fWindowScaleY, 1.0f );
            ::D3DXMatrixTranslation( &mTrans, vCursorPos.x + (float)width, vCursorPos.y - 1.0f, 0.0f );
            ::D3DXMatrixMultiply( &mTotal, &mScale, &mTrans );
        }
        else
        {
			if( (m_iStart - iOffset) >= 0 )
			{
				if ( m_bIsPasswd == true )
				{
					wcsncpy( txt, &m_strPassWord[iOffset], (m_iStart - iOffset) );
				}
				else
				{
					wcsncpy( txt, &m_strEdit[iOffset], (m_iStart - iOffset) );
				}
			}

			// 멀티라인용 x, y offset 을 구한다.
			int iExtraX = 0;
			int iExtraY = 0;
			if( m_bMultyLineMode )
			{
                iExtraX += GetLeftSideWidthByIndex( (m_iStart ), iOffset );
                iExtraY += (GetLineByIndex( (m_iStart ), iOffset ) * pkFontMgr->GetHeight());
			}
			else
			{
				width = pkFontMgr->GetWidth(txt);
				if ( GetWidth() * m_fWindowScaleX < (DWORD)width )
					width = static_cast<int>( GetWidth() * m_fWindowScaleX );
			}

            // draw caret
            ::D3DXMatrixScaling( &mScale, 2.0f , (float)pkFontMgr->GetHeight() + 2.0f * m_fWindowScaleY, 1.0f );
            ::D3DXMatrixTranslation( &mTrans, vCursorPos.x + (float)width + iExtraX, vCursorPos.y - 1.0f * m_fWindowScaleY + iExtraY, 0.0f );
            ::D3DXMatrixMultiply( &mTotal, &mScale, &mTrans );
        }       

		for ( UINT i = 0; i < 4; ++i )
		{
			::D3DXVec2Transform( &m_cursor[i].m_vPos, (const D3DXVECTOR2*)( &g_orgCursor[i].m_vPos ),
				&mTotal );
		}

		g_pGCDeviceManager2->DrawInScreen(NULL,m_cursor);
    }
    g_pGCDeviceManager2->PopState();
}

void KD3DEdit::CreateEditCtrl( void )
{
    ASSERT( g_hUIMainWnd != NULL );

    m_hWndEdit = ::CreateWindow( L"edit", NULL,
                                 WS_CHILD | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                                 0, 0, 128, 32, g_hUIMainWnd, NULL,
                                 ::GetModuleHandle( NULL ), NULL );
    ASSERT( m_hWndEdit != NULL );

    ::SetWindowLong( m_hWndEdit, GWL_USERDATA, reinterpret_cast<LONG>( this ) );

    m_fpOrgEditProc = reinterpret_cast<WNDPROC>(
        ::SetWindowLong( m_hWndEdit, GWL_WNDPROC, reinterpret_cast<LONG>( EditCtrlProc ) ) );

	m_Ime.Create( m_hWndEdit );
	m_Ime.SetNativeInputMode( true );
}

void KD3DEdit::DestroyEditCtrl( void )
{
    if ( m_hWndEdit != NULL )
    {
		m_Ime.Destroy();
        ::DestroyWindow( m_hWndEdit );
        m_hWndEdit = NULL;
    }
}

bool KD3DEdit::IsAlphaScanCode( BYTE scanCode )
{
    return ( 16 <= scanCode && scanCode <= 25 ) ||  // Q to P
           ( 30 <= scanCode && scanCode <= 38 ) ||  // A to L
           ( 44 <= scanCode && scanCode <= 50 );    // Z to M
}

bool KD3DEdit::CheckLimitSize( )
{
	if( m_iLitmitPixelSize == -1 )
		return true;

	if( m_pkFontMgrLimtPixelSize == NULL )
		return true;

	if( m_pkFontMgrLimtPixelSize->GetWidth(m_strEdit) > m_iLitmitPixelSize )
	{
		return false;
	}	

	return true;
}

LRESULT KD3DEdit::D3DEditProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	if( m_Ime.ImeProc( uMsg, wParam, lParam ) )
	{
		return TRUE;
	}

	static bool bInputLangChanged = false;
	switch ( uMsg )
    {
		case WM_INPUTLANGCHANGE:
		{
			//printf("-----------------------WM_INPUTLANGCHANGE-----------------------\n");
			m_iOffset_X       = 0;
            m_iOffset_Y       = 0;
			m_iStart        = 0;
			m_iEnd          = 0;

			::ZeroMemory( m_strEdit, sizeof( m_strEdit ) );
			::ZeroMemory( m_strPassWord, sizeof( m_strPassWord ) );

			//edit창에서 포커스를 뺏어야 한다.
			m_bHasFocus = false;
			Activate(false);
			bInputLangChanged = true;
		}
        case WM_SETFOCUS:
		{
			g_hIMEWnd = m_hWndEdit;
			if( bInputLangChanged == true )
			{
				bInputLangChanged = false;
				if ( g_hIMEWnd != NULL )
				{
					::SetFocus( g_hIMEWnd );
					SendMessage( g_hIMEWnd, WM_KEYDOWN, VK_RETURN, 0);
				}
			}
			else
			{
				//printf("WM_SETFOCUS! \n");
				//혹시 Ime이전의 것이 있다면 청소해주고, 
				m_Ime.ClearIME();
				//새로 만든다.
				m_Ime.Create(m_hWndEdit);
				m_bHasFocus = true;
			}			
            break;
        }
        case WM_KILLFOCUS:
        {
		    //printf("WM_KILLFOCUS! \n");
            m_Ime.ClearIME();
            m_bHasFocus = false;
            break;
        }
        case WM_CHAR:
        {
            if ( VK_RETURN == wParam )
            {
                if( m_iLimitSize > 0 && static_cast<int>(wcslen(m_strEdit)) >= m_iLimitSize + 1 )
                    return FALSE;

				if( GetLineByIndex( GetCurCursorIndex() ) >= m_iLimitLine - 1 && m_iLimitLine > 0 )
					return FALSE;                

                if ( m_bAllowNewLine )
                {
                    //break;
                    return TRUE;
                }
                else
                {
				    // IME 문자 조합 결정 시에 엔터가 들어올 수 있으므로 조합 중이 아님을 검사한다.
				    if( m_bHasFocus && ( wcslen( m_Ime.GetComp() ) == 0 ) )
				    {
                        AddAutoCreateMsg();
					    SpeakToActionListener( KActionEvent( this, D3DWE_EDIT_RETURN ) );
				    }
                    return TRUE;
                }
            }
            else if ( VK_ESCAPE == wParam )
            {
                if( m_bHasFocus )
                    SpeakToActionListener( KActionEvent( this, D3DWE_EDIT_ESCAPE ) );
                return TRUE;
            }
			else if ( VK_TAB == wParam )
			{				
				if( m_bHasFocus )
					SpeakToActionListener( KActionEvent( this, D3DWE_EDIT_TAB ) );
				return TRUE;
			}

			if ( !IsEditType(wParam ) ) {
				return TRUE;
			}

			if( !CheckLimitSize() && ( VK_BACK != wParam ) )
			{
				return TRUE;
			}
			else
				break;

            break;
        }
		case WM_CONTEXTMENU:
		{
			return FALSE;
		}
        case WM_KEYDOWN: // set state changed to TRUE
        {
            switch ( wParam )
            {
				case VK_SHIFT:
					m_bShift = true;
					break;
                case VK_RETURN:
                    {
                        if( m_iLimitSize > 0 && static_cast<int>(wcslen(m_strEdit)) >= m_iLimitSize )
                            return FALSE;

						if( GetLineByIndex( GetCurCursorIndex() ) >= m_iLimitLine - 1 && m_iLimitLine > 0 )
							return FALSE;

                        if ( m_bAllowNewLine )
                        {
                            PushNewLineAtIndex( m_iStart );

                            return TRUE;
                        }
                        else
                            return TRUE;
                    }
                case VK_ESCAPE:
				case VK_TAB:
                    {
	                    return TRUE;
                    }
                case VK_UP:
                    {
                        FindAutoCreateMsg( true );
                        return TRUE;
                    }
                case VK_DOWN:
                {
                        FindAutoCreateMsg( false );
                    return TRUE;
                }
            }

            ::SendMessage( m_hWndEdit, EM_GETSEL,
                           reinterpret_cast<WPARAM>( &m_iStart ),
                           reinterpret_cast<LPARAM>( &m_iEnd ) );

            BYTE scanCode = ( (BYTE)lParam & 0x00ff0000 ) >> 16;

            if ( m_Ime.GetIMEState() == NOTHING && m_Ime.IsNativeInputMode() &&
                 !( lParam & 0x1000000L ) && // Isn't Extended key?
                 IsAlphaScanCode( scanCode ) )
            {
				m_iStart += 2;
                m_iEnd   += 2;
            }

            break;
        }
        case WM_KEYUP: // set state changed to TRUE
        {
            switch ( wParam )
            {
                case VK_RETURN:
                    {
                        if( m_iLimitSize > 0 && static_cast<int>(wcslen(m_strEdit)) >= m_iLimitSize )
                            return FALSE;

						if( GetLineByIndex( GetCurCursorIndex() ) >= m_iLimitLine - 1 && m_iLimitLine > 0 )
							return FALSE;

                        if ( m_bAllowNewLine )
                        {
                            //break;
                            return TRUE;
                        }
                        else
                            return TRUE;
                    }
                case VK_ESCAPE:
                case VK_TAB:
                {
                    return TRUE;
                }
                case VK_SNAPSHOT:
                {
                    // Edit창에 커서가 가 있을 경우에..
                    // 스크린샷을 찍게 하려고 이런짓을 했음..
                    // 별로 좋은 방법인거 같진 않지만..-ㅅ -;;
                    // Edit 컨트롤로 메세지가 날아가는걸 컷트해서
                    // 거기에서 처리를 해줘야 될거 같은데..ㅠ ㅅ ㅠ
                    // 우선 이렇게 합니다..
                    g_MyD3D->ScreenShotJPEG();
                    return TRUE;
                }
				case VK_UP:
				    {
                        if ( m_bShift )
                        {
                            if ( m_eSelectIndex == NOT_DETERMINE )
                                m_eSelectIndex = USE_START;
                        }
                        else
                            m_eSelectIndex = NOT_DETERMINE;

                        int iCurLine = 0;
                        int iCurWidth = 0;
                        if( m_bMultyLineMode )
                        {
                            iCurLine = GetLineByIndex( GetCurCursorIndex() );							// 해당 인덱스가 몇번째 라인인지 얻어온다.
                            iCurWidth = GetLeftSideWidthByIndex( GetCurCursorIndex() );
                        }
                        else
                            break;

                        // 한줄짜리는 위로 누를 때, 아무것도 안하겠다 
                        if( iCurLine > 0 )
                        {
                            int iLine = 0;
                            int iFontWidth = 0;
                            int iIndex;
                            int iStrLen = (int)wcslen( m_strEdit );
                            for ( int i = 0; i < iStrLen; i++ )
                            {
                                // 인덱스 저장
                                iIndex = i;
                                if ( iLine == iCurLine - 1 && iFontWidth >= iCurWidth )
                                    break;

								if (m_bIsPasswd ==true)
								{
									iFontWidth += GetFontMgr()->GetWidth( m_strPassWord[i] );
								}
								else
								{
									iFontWidth += GetFontMgr()->GetWidth( m_strEdit[i] );
								}                               

                                if ( m_strEdit[i] == L'\n' )
                                {
                                    if ( ++iLine == iCurLine )
                                        break;

                                    iFontWidth = 0;
                                }
                            }

                            if ( m_bShift )
                                GetCurCursorIndex() = iIndex;
                            else
                                m_iStart = m_iEnd = iIndex;
                        }
                        else
                        {
                            if ( m_bShift )
                                GetCurCursorIndex() = 0;
                            else
                                m_iStart = m_iEnd = 0;
                        }

                        SetSelection(m_iStart, m_iEnd);
					}
					break;
				case VK_DOWN:
					{
                        if ( m_bShift )
                        {
                            if ( m_eSelectIndex == NOT_DETERMINE )
                                m_eSelectIndex = USE_END;
                        }
                        else
                            m_eSelectIndex = NOT_DETERMINE;

                        int iCurLine = 0;
                        int iCurWidth = 0;
                        int iLastLineNum = 0;

                        int iStrLen = (int)wcslen( m_strEdit );     // 현재 스트링 길이~

                        if( m_bMultyLineMode )
                        {
                            iCurLine = GetLineByIndex( GetCurCursorIndex() );							// 해당 인덱스가 몇번째 라인인지 얻어온다.
                            iLastLineNum = GetLineByIndex( iStrLen - 1 );                   // 제일 마지막 줄이 몇번째 라인인지 얻어온다.
                            iCurWidth = GetLeftSideWidthByIndex( GetCurCursorIndex() );
                        }
                        else
                            break;

                        // 현재 라인이 마지막 줄이냐?
                        if( iCurLine < iLastLineNum )
                        {
                            int iLine = 0;
                            int iFontWidth = 0;
                            int iIndex;
                            
                            for ( int i = 0; i < iStrLen; i++ )
                            {
                                // 인덱스 저장
                                iIndex = i;
                                if ( iLine == iCurLine + 1 && iFontWidth >= iCurWidth )
                                    break;

                                iFontWidth += GetFontMgr()->GetWidth( m_strEdit[i] );

                                if ( m_strEdit[i] == L'\n' )
                                {
                                    if ( ++iLine > iCurLine + 1 )
                                        break;

                                    iFontWidth = 0;
                                }
                            }

                            if ( m_bShift )
                                GetCurCursorIndex() = iIndex;
                            else
                                m_iStart = m_iEnd = iIndex;
                        }
                        // 마지막 줄이면 커서는 제일 끝으로 ㅋ 
                        else
                        {
                            if ( m_bShift )
                                GetCurCursorIndex() = iStrLen;
                            else
                                m_iStart = m_iEnd = iStrLen;
                        }

                        SetSelection(m_iStart, m_iEnd);
					}
					break;
                case VK_LEFT:
                    if ( !m_bShift )
                        m_eSelectIndex = NOT_DETERMINE;
                    break;
                case VK_RIGHT:
                    if ( !m_bShift )
                        m_eSelectIndex = NOT_DETERMINE;
                    break;
				case VK_SHIFT:
					m_bShift = false;
					break;
            }

            ::SendMessage( m_hWndEdit, EM_GETSEL,
                           reinterpret_cast<WPARAM>( &m_iStart ),
                           reinterpret_cast<LPARAM>( &m_iEnd ) );

            break;
        }
    }
	if ( wParam != 0 && m_iType == EDIT_NUMBER )
		SpeakToActionListener( KActionEvent( this, D3DWE_EDIT_NUMBER ) );
    // call original edit control procedure
    return ::CallWindowProc( m_fpOrgEditProc, m_hWndEdit, uMsg, wParam, lParam );
}

void KD3DEdit::DeleteColorCode()
{
	int iSize = wcslen( m_strEdit );

	for (int i = 0; i < iSize; i++ )
	{
		if ( m_strEdit[i] == L'#' && i + 1 != iSize - 1&& ( m_strEdit[i+1] == L'c' || m_strEdit[i+1] == L'C' ) )
		{
			SetText( L"" );
		}
	}
}

GCDeviceFont* KD3DEdit::GetFontMgr()
{
	if (m_eAlignType == WNDUIAT_EXPANTION && g_pGCDeviceManager->m_windowScaleY != 1.0f)
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
		if (m_fWindowScaleY == g_pGCDeviceManager->GetWindowScaleY() / (1024.0f/768.0f))
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

std::vector<POINT> KD3DEdit::GetLineSelectionList(int iStart, int iEnd)
{
	std::vector<POINT> ptlist;

	int iLineFirst = iStart;
    if ( m_iOffset_X > iLineFirst )
        iLineFirst = m_iOffset_X;

	int iLineEnd = 0;
    int iStrLen = (int)wcslen( m_strEdit );

	int i;
	for( i = iStart; i < iStrLen; i++ )
	{
        // 현재 출력되고 있는 글자보다 앞이나 뒤가 셀렉션 되었다면~ 그려주지 않겠다.
        if ( i < m_iOffset_X )
            continue;

        if( m_strEdit[i] == L'\n' || (i == iEnd) )
		{
			if( iStart <= i && i <= iEnd )
			{
                iLineEnd = i;

				POINT pt;
				pt.x = iLineFirst;
				pt.y = iLineEnd;
				ptlist.push_back( pt );

				if( (i == iEnd) || i >= m_iOffset_X + m_iTexCount )
					break;

                if ( m_strEdit[i] == L'\n' && i + 1 < iStrLen )
                    iLineFirst = i + 1;
                else
                    iLineFirst = i;
			}
		}
	}

	if( ( i == iStrLen ) )
	{
		POINT pt;
		pt.x = iLineFirst;
		pt.y = wcslen( m_strEdit );
		ptlist.push_back( pt );
	}

	return ptlist;
}

int KD3DEdit::GetLineByIndex( const int iIndex, const int iStartIndex /*= 0 */ )
{
    int iCurLine = 0;
    int iSize = (int)wcslen( m_strEdit );
    for( int i = iStartIndex; i < iSize; i++ )
    {
        if( iIndex == i )
            return iCurLine;

        if( m_strEdit[i] == L'\n' )
            ++iCurLine;
    }

    return iCurLine;
}

int KD3DEdit::GetLeftSideWidthByIndex( const int iIndex, const int iStartIndex /*= 0 */ )
{
    int iFontWidth = 0;
    int iSize = (int)wcslen( m_strEdit );
    for( int i = iStartIndex; i < iSize; i++ )
    {
        if( iIndex == i )
            return iFontWidth;

		if (m_bIsPasswd ==true)
		{
			iFontWidth += GetFontMgr()->GetWidth( m_strPassWord[i] );
		}
		else
		{
			iFontWidth += GetFontMgr()->GetWidth( m_strEdit[i] );
		}
       

        if( m_strEdit[i] == L'\n' )
        {
            iFontWidth = 0;
        }
    }

    return iFontWidth;
}

void KD3DEdit::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KD3DEdit>( L, "KD3DEdit" );
    lua_tinker::class_def<KD3DEdit>( L, "GetText",  &KD3DEdit::GetText  );
    lua_tinker::class_inh<KD3DEdit,KD3DWnd>( L );
}

void KD3DEdit::PushThis( lua_State* L )
{
    lua_tinker::push( L, this );
}

void KD3DEdit::PushNewLineAtIndex( int iIndex )
{
    int iSize = wcslen( m_strEdit );
    int iStartCursor = iIndex;
    if ( iSize < MAX_PATH )
    {
        /*
        m_strEdit[0] ~ m_strEdit[m_iStart - 1]
        m_strEdit[m_iStart] => 개행문자
        m_strEdit[m_iStart] ~ m_strEdit[iSize]
        */

        std::wstring strTmp = m_strEdit;
        std::wstring strPost = L"";
        if ( iSize - iStartCursor > 0 )
            strPost = strTmp.substr( iStartCursor, iSize - iStartCursor );

        m_strEdit[iStartCursor] = L'\n';
        wcscpy_s( m_strEdit + iStartCursor + 1, MAX_PATH - (iStartCursor + 1), strPost.c_str() );

        SetText( m_strEdit, false );
        SetSelection( ++iStartCursor, iStartCursor);
    }
}

void KD3DEdit::SetLimitText( int iLimit, int iLimitPixelSize /*= -1*/, int iFontSize /*= 12 */ )
{
    ::SendMessage( m_hWndEdit, EM_LIMITTEXT, (WPARAM)iLimit, 0 );
    m_iLimitSize = iLimit;
    m_iLitmitPixelSize = iLimitPixelSize;

    if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
        m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutline        : g_pkFontMgr;
	else if(SiKGCMultipleLanguages()->GetSmallFontSize() == m_lFontSize) 
		m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutlineSmall      : g_pkFontMgrSmall;
    else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
        m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutline14      : g_pkFontMgr14;
    else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
        m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutline16      : g_pkFontMgr16;
    else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
        m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutlineTitle   : g_pkFontMgrTitle;
    else 
        m_pkFontMgrLimtPixelSize = IsDrawFontOutline() ? g_pkFontMgrOutline        : g_pkFontMgr;

    // 멀티라인 허용되는 EditBox에서는 몇줄까지 쳐지는지 필요함!
    if ( m_bAllowNewLine )
    {
        m_iLimitLine_Y = int( float( GetHeight() ) / float( m_pkFontMgrLimtPixelSize->GetFontSize() ) );
    }
}

int& KD3DEdit::GetCurCursorIndex()
{
    //if ( m_iEnd < m_iStart )
        //SwapCursor();

    return ( m_eSelectIndex == USE_END ) ? m_iEnd : m_iStart;
}

void KD3DEdit::SwapCursor( void )
{
    int tmp;
    tmp = m_iStart;
    m_iStart = m_iEnd;
    m_iEnd = tmp;
}

bool KD3DEdit::CheckLimiteTextByByte()
{
    if( m_iLimitByte > 0 )
    {
        char szTemp[MAX_PATH];
        GetWindowTextW( m_hWndEdit, m_strEdit, MAX_PATH );
        WideCharToMultiByte(CP_ACP, 0, m_strEdit, -1, szTemp, MAX_PATH, NULL, NULL);

        std::string szEditText(szTemp);
        if( int(szEditText.size()) >= m_iLimitByte )
        {
            int iEnd = wcslen(m_strEdit) - 1;
            memset(&m_strEdit[ iEnd ], 0, sizeof(WCHAR)*(MAX_PATH-iEnd));
            SetWindowTextW( m_hWndEdit, m_strEdit );
            ::SendMessage( m_hWndEdit, EM_SETSEL, m_iLimitByte, m_iLimitByte );
            m_iStart = m_iEnd = m_iLimitByte;
        }
        return true;
    }
    return false;
}

// 입력된 채팅내용 자동완성 시스템
void KD3DEdit::AddAutoCreateMsg()
{
    std::wstring szText = GetText();
    if( 0 < int(szText.size()) )
    {
        bool bAccStr = true;

        // 직전에 선택된 문자랑 중복되느냐!!!
        if( 0 <= m_iSelectAutoCreatte && m_iSelectAutoCreatte < int(m_listAutoCreate.size()) )
        {
            std::list< std::wstring >::iterator listIter = m_listAutoCreate.begin();
            for(int iLoop=0; iLoop<m_iSelectAutoCreatte; ++listIter, ++iLoop);

            if( *listIter == szText )
                bAccStr = false;
        }

        if( bAccStr )
        {
            // 입력 갯수를 초과하였느냐!!
            if( int(m_listAutoCreate.size()) > MAX_AUTO_CREATE )
                m_listAutoCreate.erase( m_listAutoCreate.begin() );

            // 누적해
            m_listAutoCreate.push_back( szText );
        }
    }
}

void KD3DEdit::FindAutoCreateMsg( bool bIsUp )
{
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_LOGIN )
        return;

    if( m_bHasFocus && 0 < int(m_listAutoCreate.size()) )
    {
        if( bIsUp )
        {
            if( --m_iSelectAutoCreatte < 0 )
                m_iSelectAutoCreatte = 0;
        }
        else
        {
            if( ++m_iSelectAutoCreatte >= int(m_listAutoCreate.size()) )
                m_iSelectAutoCreatte = int(m_listAutoCreate.size()) - 1;
        }

        std::list< std::wstring >::iterator listIter = m_listAutoCreate.begin();
        for(int iLoop=0; iLoop<m_iSelectAutoCreatte; ++listIter, ++iLoop);

        if( listIter != m_listAutoCreate.end() )
            SetText( listIter->c_str() );
    }
}

bool KD3DEdit::IsEditType( WPARAM wParam )
{
	if ( m_iType == EDIT_ALL_TYPE )
		return true;

	if ( m_iType == EDIT_NUMBER ) {
		if ( wParam >= 48 && wParam <= 57 || wParam == 8)
			return true;
		else
			return false;
	}

	return false;
}

int KD3DEdit::GetCursorIndex()
{
    return ( m_eSelectIndex == USE_END ) ? m_iEnd : m_iStart;
}
