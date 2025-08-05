#include "stdafx.h"
//
//
//#include ".\kd3demoticonstring.h"
#include "KGCEmoticon.h"

#include "GCItemManager.h"

ImplementSingleton( KD3DEmoticonString );

KD3DEmoticonString::KD3DEmoticonString(void)
{
    m_vecStrDivide.clear();
    m_vecStrPieces.clear();
}

KD3DEmoticonString::~KD3DEmoticonString(void)
{
    m_vecStrDivide.clear();
    m_vecStrPieces.clear();
}


//채팅창에 들어갈 문자열에서 이모티콘이 들어간 부분을 공백으로 치환한다. 
//그리고 그 글짜 칸위치와 같은 위치에 이모티콘을 넣는다.
std::vector<KGCEmoticon*> KD3DEmoticonString::GetEmoticonDataVectorFromString( const std::wstring& strContent, int iIDGapSize , std::wstring& strNoEmoticon)
{
	std::vector<KGCEmoticon*> ret;
	std::wstring::size_type curIdx = 0;
	std::wstring strRet=L"";
	while( curIdx < strContent.size() )
	{
		if( strContent[curIdx] == EMOTICON_SEPRATOR && iIDGapSize < (int)curIdx )
		{
			std::wstring::size_type tmpIdx = curIdx + 1;
			if( tmpIdx < strContent.size() - 1 )
			{
				// find next '/'
				std::wstring::size_type nextIdx;
				nextIdx = strContent.find( EMOTICON_SEPRATOR, tmpIdx );
				if( nextIdx != std::wstring::npos )
				{
					assert( nextIdx >= tmpIdx );
					std::wstring strEmot = strContent.substr( tmpIdx, nextIdx-curIdx-1 );
					KGCEmoticon* pEmot = SiKGCEmoticonManager()->GetEmoticonDataByCommand( strEmot );
					if( NULL != pEmot )
					{
						// found /haha/
						ret.push_back(pEmot);
						curIdx = nextIdx + 1;
						strRet+=L" ";
						continue;
					}
				}
			}
		}
		strRet+=strContent[curIdx];
		ret.push_back(NULL);
		curIdx++;
	}
	strNoEmoticon=strRet;
	return ret;
}

void KD3DEmoticonString::DivideStringPiece( const std::wstring& strContent, int iIDGapSize )
{
    std::wstring::size_type curIdx = 0;
    EmotStrPair temppair;
    while( curIdx < strContent.size() )
    {
        temppair.first = false; // string
        temppair.second.first = NULL;

        if( strContent[curIdx] == EMOTICON_SEPRATOR && iIDGapSize < (int)curIdx )
        {
            std::wstring::size_type tmpIdx = curIdx + 1;
            if( tmpIdx < strContent.size() - 1 )
            {
                // find next '/'
                std::wstring::size_type nextIdx;
                nextIdx = strContent.find( EMOTICON_SEPRATOR, tmpIdx );
                if( nextIdx != std::wstring::npos )
                {
                    assert( nextIdx >= tmpIdx );
                    std::wstring strEmot = strContent.substr( tmpIdx, nextIdx-curIdx-1 );
                    KGCEmoticon* pEmot = SiKGCEmoticonManager()->GetEmoticonDataByCommand( strEmot );
                    if( NULL != pEmot )
                    {
                        if( false == temppair.second.second.empty() )
                        {
                            m_vecStrDivide.push_back( temppair );
                            temppair.second.second = L"";
                        }

                        // found /haha/
                        EmotStrPair pairtoadd;
                        pairtoadd.first = true; // emoticon
                        pairtoadd.second.first = pEmot;
                        pairtoadd.second.second = strEmot;
                        m_vecStrDivide.push_back( pairtoadd );
                        curIdx = nextIdx + 1;
                        continue;
                    }
                }
            }
        }
        //else if( strContent[curIdx] == EMOTICON_NEWLINE )
        //{
        //    m_vecStrDivide.push_back( temppair );
        //    temppair.second.second = L"";
        //    curIdx++;
        //    continue;
        //}
        temppair.second.second += strContent[curIdx];
        curIdx++;
    }

    if( false == temppair.second.second.empty() )
    {
        m_vecStrDivide.push_back( temppair );
    }
}

void KD3DEmoticonString::Dump( std::wstringstream& stm )
{
    std::vector< EmotStrPair >::iterator vit;
    for( vit = m_vecStrDivide.begin(); vit != m_vecStrDivide.end(); ++vit )
    {
        EmotStrPair& pr = (*vit);
        if( pr.first == true )
        {
            stm << L"[Emoticon] ";
        }
        stm << pr.second.second << std::endl;
    }
}

void KD3DEmoticonString::clear()
{
    m_vecStrDivide.clear();
    m_vecStrPieces.clear();
}

// [6/16/2008] breadceo. 퍼포먼스 쑤레기일지도..
void KD3DEmoticonString::Arrange( GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring& strIDGap )
{
    m_vecStrPieces.clear();

    std::vector< KD3DEmoticonString::EmotStrPair > vecTemp;

    std::wstring strSplit = L"";
    int iEmotCount = 0;
    int iStrWidth = 0;
    std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;
    for( vit = m_vecStrDivide.begin(); vit != m_vecStrDivide.end(); )
    {
        bool bIter = true;
        int iEmotWidth = EMOTICON_UI_SIZE * iEmotCount;

        KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
        if( true == prEmot.first )
        {
            if( false == strSplit.empty() )
            {
                vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strSplit ) ) );
            }

            // 하나 더 들어갈 수 있나
            iEmotWidth = EMOTICON_UI_SIZE * (int)( iEmotCount + 1 );
            if( iStrWidth + pkFontMgr->GetWidth( strSplit.c_str() ) + iEmotWidth > iWidth  )
            {
                // newline이 되면 모든값을 초기화
                strSplit.erase( 0, strSplit.size() );
                strSplit = strIDGap + strSplit;
                iEmotCount = 0;
                m_vecStrPieces.push_back( vecTemp );
                vecTemp.clear();
                iStrWidth = 0;
            }
            vecTemp.push_back( std::make_pair( true, std::make_pair( prEmot.second.first, L"" ) ) );
            iStrWidth += pkFontMgr->GetWidth( strSplit.c_str() );
            strSplit.erase( 0, strSplit.size() );
            iEmotCount++;
        }
        else
        {
            for( std::wstring::size_type i = 0; i < (int)prEmot.second.second.size(); ++i )
            {
                wchar_t& wchar = prEmot.second.second[i];
                strSplit += wchar;

                if( EMOTICON_NEWLINE == prEmot.second.second[i] ||
                    iStrWidth + pkFontMgr->GetWidth( strSplit.c_str() ) + iEmotWidth > iWidth )
                {
                    prEmot.second.second.erase( 0, i + 1 );                    
                    std::wstring strTemp = strSplit.substr( 0, strSplit.size() - 1 );
					#ifdef STATIC_WORDWRAP
                    if( false == strSplit.empty() )
                    {
                        strTemp += L'-';
                    }
                    #endif

                    vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strTemp ) ) );
                    strSplit.erase( 0, strSplit.size() - 1 );

                    // 다음 줄부터는 아이디만큼의 띄워주고 출력한다.
                    #ifndef STATIC_WORDWRAP
                    strSplit = strIDGap + strSplit;
                    #endif
                    iEmotCount = 0;
                    m_vecStrPieces.push_back( vecTemp );
                    vecTemp.clear();
                    iStrWidth = 0;

                    bIter = false; // 다시와야되!
                    break;
                }
            }
        }

        if( true == bIter )
        {
            ++vit;
		}
    }

    if( false == strSplit.empty() )
    {
        vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strSplit ) ) );
    }

    if( false == vecTemp.empty() )
    {
        m_vecStrPieces.push_back( vecTemp );
    }
    m_vecStrDivide.clear();
}

#if defined(NATION_THAILAND)
void KD3DEmoticonString::ArrangeForThaiWordWrap( const std::wstring& strContent, GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring strIDGap , std::vector<KGCEmoticon*> vecEmoticons)
{
	m_vecStrPieces.clear();
	int cutpoint;
	int iPt = 0;
	std::vector<bool> vecbCut = BreakStringTH(strContent);
	std::vector< KD3DEmoticonString::EmotStrPair > vecTemp;
	cutpoint=0;
	bool bFirstLine=true;
	for ( std::wstring::size_type i = 0; i < strContent.size(); i++ )
	{
		if (vecbCut[i]==true) //Break Point 찍기 편하라고 쪼갬-_-;
		{
			cutpoint=i;
		}

		if ((i - cutpoint) > 10 ) //10글짜 이상 차이나면 그냥 출력 되도록 하심
		{
			cutpoint=i;
		}

		if ( iPt == 0 && vecEmoticons[i]==NULL && ( strContent[i] == L' ' || strContent[i] == L'\t' ) )
			continue;

		int iCalcedWidth=pkFontMgr->GetWidth( strContent.substr(iPt,i-iPt+1).c_str() );

		for (int j=0;j<(int)(i-iPt);j++)
		{
			if (vecEmoticons[j+iPt]!=NULL)
				iCalcedWidth+=EMOTICON_UI_SIZE;
		}

		if (bFirstLine==false)
		{
			iCalcedWidth += pkFontMgr->GetWidth( strIDGap.c_str() );
		}

		if ( iCalcedWidth > iWidth )
		{
			std::wstring strText = strContent.substr(iPt,cutpoint-iPt);
			std::wstring strDivideText;	
			vecTemp.clear();

			if (strText.empty()==false)
			{
				strDivideText.clear();
				if (bFirstLine==false)
				{
					strDivideText=strIDGap;
				}
				for (int j=0;j<(int)strText.length();j++)
				{
					if (vecEmoticons[iPt+j]!=NULL)
					{
						vecTemp.push_back(std::make_pair(false,std::make_pair((KGCEmoticon*)NULL,strDivideText)));
						strDivideText.clear();
						vecTemp.push_back(std::make_pair(true,std::make_pair(vecEmoticons[iPt+j], L"" )));
					}
					else
					{
						strDivideText+=strText[j];
					}					
				}			

				//이모티콘 없는 경우.
				if (strDivideText.empty()==false)
				{
					vecTemp.push_back(std::make_pair(false,std::make_pair((KGCEmoticon*)NULL,strDivideText)));
					strDivideText.clear();
				}
			}
			m_vecStrPieces.push_back(vecTemp);
			bFirstLine=false;
			iPt=cutpoint;
		}
	}

	if ( (strContent.size() - iPt) !=0)
	{
		std::wstring strText = strContent.substr(iPt,strContent.size()-iPt);
		std::wstring strDivideText;	
		vecTemp.clear();
		if (strText.empty()==false)
		{
			strDivideText.clear();
			if (bFirstLine==false)
			{
				strDivideText=strIDGap;
			}
			for (int j=0;j<(int)strText.length();j++)
			{
				if (vecEmoticons[iPt+j]!=NULL)
				{
					vecTemp.push_back(std::make_pair(false,std::make_pair((KGCEmoticon*)NULL,strDivideText)));
					strDivideText.clear();
					vecTemp.push_back(std::make_pair(true,std::make_pair(vecEmoticons[iPt+j], L"" )));
				}
				else
				{
					strDivideText+=strText[j];
				}					
			}			

			//이모티콘 없는 경우.
			if (strDivideText.empty()==false)
			{
				vecTemp.push_back(std::make_pair(false,std::make_pair((KGCEmoticon*)NULL,strDivideText)));
				strDivideText.clear();
			}
		}		
		m_vecStrPieces.push_back(vecTemp);
	}

	//이건 작업용인가 왜 클리어 해주는겨..-_-;
	m_vecStrDivide.clear();	
}
#endif

#if defined( NATION_USA ) || defined( NATION_IDN ) || defined(NATION_PHILIPPINE) || defined( NATION_EU )
void KD3DEmoticonString::ArrangeForWordWrap( GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring strIDGap )
{
	m_vecStrPieces.clear();

	std::vector< KD3DEmoticonString::EmotStrPair > vecTemp;

	std::wstring strSplit = L"";
	int iEmotCount = 0;
	int iStrWidth = 0;

	std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;
	for( vit = m_vecStrDivide.begin(); vit != m_vecStrDivide.end(); )
	{
		bool bIter = true;
		int iEmotWidth = EMOTICON_UI_SIZE * iEmotCount;

		KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
		if( true == prEmot.first )
		{
			if( false == strSplit.empty() )
			{
				vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strSplit ) ) );
			}

			// 하나 더 들어갈 수 있나
			iEmotWidth = EMOTICON_UI_SIZE * (int)( iEmotCount + 1 );
			if( iStrWidth + pkFontMgr->GetWidth( strSplit.c_str() ) + iEmotWidth > iWidth  )
			{
				// newline이 되면 모든값을 초기화
				strSplit.erase( 0, strSplit.size() );
				strSplit = strIDGap + strSplit;
				iEmotCount = 0;
				m_vecStrPieces.push_back( vecTemp );
				vecTemp.clear();
				iStrWidth = 0;
			}
			vecTemp.push_back( std::make_pair( true, std::make_pair( prEmot.second.first, L"" ) ) );
			iStrWidth += pkFontMgr->GetWidth( strSplit.c_str() );
			strSplit.erase( 0, strSplit.size() );
			iEmotCount++;
		}
		else
		{
			int iLastSpace = 0;		
			for( std::wstring::size_type i = 0; i < (int)prEmot.second.second.size(); ++i )
			{
				wchar_t& wchar = prEmot.second.second[i];
				strSplit += wchar;

				// 스페이스면 저장, 뉴라인이라도 일단 저장
				if( EMOTICON_SPACE == prEmot.second.second[i] || EMOTICON_NEWLINE == prEmot.second.second[i] )
					iLastSpace = strSplit.size() - 1;

				// 줄바꿈일땐
				if( EMOTICON_NEWLINE == prEmot.second.second[i] ||
				    iStrWidth + pkFontMgr->GetWidth( strSplit.c_str() ) + iEmotWidth > iWidth )
				{
					std::wstring strTemp = strSplit.substr( 0, iLastSpace );

					// 공백문자 하나도 없을때만 - 해주자
					if( iLastSpace == 0 && EMOTICON_NEWLINE != prEmot.second.second[i])
					{
						iLastSpace = strSplit.size() - 1;
						strTemp = strSplit.substr( 0, iLastSpace );
						strTemp += L'-';
					}

					vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strTemp ) ) );
					strSplit.erase( 0, iLastSpace );

					// 다음 줄부터는 아이디만큼의 띄워주고 출력한다.
					strSplit = strIDGap + strSplit;

					iEmotCount = 0;
					m_vecStrPieces.push_back( vecTemp );
					vecTemp.clear();
					iStrWidth = 0;
					prEmot.second.second.erase( 0, i + 1 );                    

					bIter = false; // 다시와야되!
					break;
				}
			}
		}

		if( true == bIter )
		{
			++vit;
		}
	}

	if( false == strSplit.empty() )
	{
		vecTemp.push_back( std::make_pair( false, std::make_pair( (KGCEmoticon*)NULL, strSplit ) ) );
	}

	if( false == vecTemp.empty() )
	{
		m_vecStrPieces.push_back( vecTemp );
	}
	m_vecStrDivide.clear();
}
#endif

bool KD3DEmoticonString::IsEmoticonHasMessage( const std::wstring& strContent )
{
    m_vecStrDivide.clear();
    DivideStringPiece( strContent );
    std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;
    for( vit = m_vecStrDivide.begin(); vit != m_vecStrDivide.end(); ++vit )
    {
        KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
        if( prEmot.first == true )
        {
            m_vecStrDivide.clear();
            return true;
        }
    }
    m_vecStrDivide.clear();
    return false;
}

void KD3DEmoticonString::CheckForSend( std::wstring& strContent )
{
    std::wstring strAfter(L"");
    clear();
    DivideStringPiece( strContent );
    std::vector< KD3DEmoticonString::EmotStrPair >::iterator vit;
    for( vit = m_vecStrDivide.begin(); vit != m_vecStrDivide.end(); ++vit )
    {
        KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
        if( prEmot.first == true )
        {
            KGCEmoticon* pEmoticon = prEmot.second.first;
            if( pEmoticon != NULL )
            {
                // 없으면 사용할 수 없지.
                if( false == g_pItemMgr->FindInventoryForItemID( pEmoticon->m_iItemID ) )
                {
                    strAfter += prEmot.second.second; // 걍 그 글자 넣는다
                }
                else
                {
                    strAfter += EMOTICON_SEPRATOR;
                    strAfter += prEmot.second.second; // /그글자/
                    strAfter += EMOTICON_SEPRATOR;
                }
            }
        }
        else
        {
            strAfter += prEmot.second.second;
        }
    }
    clear();
    strContent = strAfter;
}
