#include "StdAfx.h"
#include "GCUINumber.h"

IMPLEMENT_CLASSNAME( KGCUINumber );
IMPLEMENT_WND_FACTORY( KGCUINumber );
IMPLEMENT_WND_FACTORY_NAME( KGCUINumber, "gc_kgcuinumber" );

KGCUINumber::KGCUINumber(void)
: m_bShadow( true )
, m_bSelected( false )
{
	m_strType		 = "very_small_num";
	m_bMinus		 = false;
    m_bWriteRight	 = true;
	m_eAlignType	 = ALIGN_INVALID;
    m_bAnim			 = false;
	m_iNumber		 = 0;
    m_iPrecision	 = -1;
	m_iNumSize		 = 0;
	m_fInterval		 = -1.0f;   
	m_fScale		 = 1.0f;
	m_vStandardPos   = D3DXVECTOR2(0.f, 0.f);
}

KGCUINumber::~KGCUINumber(void)
{
}

void KGCUINumber::PostChildDraw( void )
{
	//정렬기능을 사용할때와 사용하지 않을때를 구분한다. 차이점은 두번째 인자의 + m_vStandardPos 밖에 없다.
	//정렬 할때 m_bWriteRight는 무조건 true이다.
	
	if( m_eAlignType == ALIGN_INVALID )
	{
		if( m_iPrecision == -1 ) { 
			g_pkUIMgr->RenderNumber( m_strType, GetRelocatedWindowPos() , m_iNumber,m_bWriteRight, m_bShadow );
		}
		else {
			g_pkUIMgr->RenderNumber( m_strType, GetRelocatedWindowPos(), m_iNumber, m_bWriteRight, m_bShadow,
			-1, true, m_iPrecision, true );
		}
	}
	else
	{
		CalculatePos();

		if( m_iPrecision == -1 && m_fScale == 1.0f)  
			g_pkUIMgr->RenderNumber( m_strType, GetRelocatedWindowPos() + m_vStandardPos, m_iNumber,true, m_bShadow,
			m_fInterval );
		if( m_iPrecision != -1 && m_fScale == 1.0f ) 
			g_pkUIMgr->RenderNumber( m_strType, GetRelocatedWindowPos() + m_vStandardPos, m_iNumber, true, m_bShadow,
			m_fInterval, true, m_iPrecision, true );
		if( m_fScale != 1.0f )
		{
			g_pkUIMgr->RenderNumberScale( m_strType, GetRelocatedWindowPos() + m_vStandardPos, m_iNumber, m_fScale,
			-1, m_bShadow, true, true, m_fInterval);
		}

	}
}

void KGCUINumber::SetStringType( std::string strType, bool bShadow )
{
	m_strType = strType;
    m_bShadow = bShadow;
}

void KGCUINumber::SetNumber( int iNumber )
{
	m_bMinus = iNumber < 0;
	m_iTargetNumber = ::abs( iNumber );
    if( !m_bAnim ) {
        m_iNumber = m_iTargetNumber;
    }

	char str[MAX_PATH] = {0,};
	sprintf(str, "%d", iNumber);
	m_iNumSize = (int)strlen(str);
}

bool KGCUINumber::GetShadow( )
{
    return m_bShadow;
}

void KGCUINumber::SetShadow( bool bEnable )
{
    m_bShadow = bEnable;
}

bool KGCUINumber::IsSelcted( )
{
    return m_bSelected;
}

void KGCUINumber::SetSelcted( bool bSelecte )
{
    m_bSelected = bSelecte;
}


void KGCUINumber::SetWriteAlign( EGCAlignType eAlignType)
{
	m_eAlignType = eAlignType;
}

void KGCUINumber::SetWriteToRight( bool bEnable )
{
	m_bWriteRight = bEnable;
}

void KGCUINumber::CalculatePos()
{
	float fwidth = (float)GetNumWidth();

	if( fwidth <= 0.0f)
		return;

	switch (m_eAlignType)
	{
		/*GetRelocatedWindowPos()가 UI박스의 좌상단 좌표이며, 정렬 분류에 따라 연산된 값을 m_vStandardPos에 담아온다.

		ALIGN_LEFT_SIDE                           ALIGN_CENTER_SIDE                        ALIGN_RIGHT_SIDE
		---------------------------------------------------------------------------------------------------
		|가나다라								      가나다라    								  가나다라|
		---------------------------------------------------------------------------------------------------
		*/
		case ALIGN_LEFT_SIDE :
			m_vStandardPos.x = 0.f;
			break;
		
		case ALIGN_CENTER_SIDE :
			//(UI박스의 너비 * 0.5f) - (글자너비 * 글자개수 * 0.5f)
			m_vStandardPos.x = this->GetWidth() * 0.5f - fwidth * m_iNumSize * 0.5f;
			break;
		
		case ALIGN_RIGHT_SIDE :
			//UI박스의 너비 - 글자너비 * 글자개수 
		    float width = (float)this->GetWidth();

            if ( m_fInterval != -1.0f )
                fwidth = m_fInterval;

            width -= fwidth * m_iNumSize;
            m_vStandardPos.x = (float)width;
			break;
	}
}

int KGCUINumber::GetNumWidth()
{
	std::map<std::string, KD3DUINumInfo>::iterator itor =  g_pkUIMgr->m_mapNumInfo.find(m_strType);
	
	if( itor == g_pkUIMgr->m_mapNumInfo.end())
		return -1;

	KD3DUINumInfo &KNumInfo = itor->second;
	return KNumInfo.dwWidth;
}