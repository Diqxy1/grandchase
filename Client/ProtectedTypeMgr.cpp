#if defined(PROTECTEDTYPE_NEW)

std::list<CMemoryBlockBase*> CMemoryBlockBase::m_listMemory;
void CMemoryBlockBase::Clear()
{
	while(!m_listMemory.empty())
	{
		delete m_listMemory.back();
		m_listMemory.pop_back();
	}
}

#else 

#include "stdafx.h"
#include ".\protectedtypemgr.h"
#include "ClientErr.h"


ProtectedTypeMgr* g_pPTypeMgr[ProtectedTypeMgr::PTMK_NUM] = { NULL, };

bool ProtectedTypeMgr::g_bHackSafe = true;

ProtectedTypeMgr::ProtectedTypeMgr(void) : m_ucVerifyCount( 10 ), m_pErrfunc( NULL )
{
    m_strProtectedTypeMgrTitle = L"ProtectedTypeMgr";
}

ProtectedTypeMgr::~ProtectedTypeMgr(void)
{
    DeleteAll();
}

void ProtectedTypeMgr::FrameMove( void )
{
#if defined(_DEBUG) && defined(PROTECTED_TYPE_MGR_TEST)
    assert( m_pErrfunc != NULL );
#endif

    Delete();

    static DWORD s_dwElapsedTime;
    s_dwElapsedTime = ::timeGetTime();

    UCHAR ucCount = 0;
    PTListIter iter;
    for( iter = m_lFrontList.begin(); iter != m_lFrontList.end(); ++ucCount )
    {
        // [7/30/2007 breadceo] 
        // Time Over : 그만 검사한다.
        if( ucCount >= m_ucVerifyCount || ::timeGetTime() - s_dwElapsedTime >= PTM_ELAPSED_TIME_MAX )
            break;

        try
        {
		// iter->m_pVerify를 NULL체크 해서 통계, NULL이면 Verify돌지 말고 삭제 ㅎ자ㅏ.
            if( iter->m_pVerify->Verify() == false )
            {
                g_bHackSafe = false;
                break;
            }
        }
        catch( ... )
        {
			g_kGlobalValue.m_vecProtectedTypeFail.push_back( KncUtil::toNarrowString( iter->m_strLog ) );
			SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_PROTECTEDTYPE_CATCH );    
            assert( !"ProtectedType Mgr Crash" );
            ReserveDelete( (*iter->m_pVerify) );
            break;
        }

        if( iter->m_bSecurity == true )
            iter->m_pVerify->SecurityUpdate();

        m_lBackList.push_back( (*iter) );
        m_lFrontList.pop_front();
        iter = m_lFrontList.begin();

        if( iter == m_lFrontList.end() )
        {
            m_lFrontList.swap( m_lBackList );
            iter = m_lFrontList.begin();
        }
    }

    if( g_bHackSafe == false )
        (*m_pErrfunc)( m_strProtectedTypeMgrTitle );
}

void ProtectedTypeMgr::Register( CVerifyElem verifyElem )
{
#if defined(_DEBUG) && defined(PROTECTED_TYPE_MGR_TEST)
    PTListIter lit = std::find( m_lFrontList.begin(), m_lFrontList.end(), verifyElem );
    assert( lit == m_lFrontList.end() );
#endif

    m_lFrontList.push_back( verifyElem );
}

void ProtectedTypeMgr::Register( bool bSecurity, IVerify& pType, const WCHAR* strLog )
{
#if defined(_DEBUG) && defined(PROTECTED_TYPE_MGR_TEST)
    PTListIter lit = std::find_if( m_lFrontList.begin(), m_lFrontList.end(), boost::bind( &CVerifyElem::m_pVerify, _1 ) == &pType );
    assert( lit == m_lFrontList.end() );
#endif

    m_lFrontList.push_back( CVerifyElem( bSecurity, pType, strLog ) );
}

void ProtectedTypeMgr::ReserveDelete( IVerify& pPType )
{
#if defined(_DEBUG) && defined(PROTECTED_TYPE_MGR_TEST)
    std::list< IVerify* >::iterator lit = std::find( m_lDeleteList.begin(), m_lDeleteList.end(), &pPType );
    assert( lit == m_lDeleteList.end() );
#endif

    m_lDeleteList.push_back( &pPType );
}

void ProtectedTypeMgr::ReserveDelete( std::list< IVerify* > delList )
{
	delList.sort();
    m_lDeleteList.merge( delList );
}

void ProtectedTypeMgr::Register( std::list< CVerifyElem > list )
{
    m_lFrontList.merge( list ,VerifyCompare());
}

void ProtectedTypeMgr::Delete()
{
    if( m_lDeleteList.empty() )
        return;

    if( m_lDeleteList.size() == m_lBackList.size() + m_lFrontList.size() )
    {
        DeleteAll();
    }
    else
    {
        std::list< IVerify* >::iterator lit;
        for( lit = m_lDeleteList.begin(); lit != m_lDeleteList.end(); ++lit )
        {
            Delete( (*lit) );
        }
    }

    m_lDeleteList.clear();
}

void ProtectedTypeMgr::Delete( IVerify*& pPType )
{
	int iDeleteCnt = 0;
	bool bFound = false;
	do
	{
		PTListIter iter = std::find_if( m_lFrontList.begin(), m_lFrontList.end(), boost::bind( &CVerifyElem::m_pVerify, _1 ) == pPType );

		if( iter != m_lFrontList.end() )
		{
			bFound = true;
			m_lFrontList.erase( iter );
			iDeleteCnt++;
		}
		else
		{
			bFound = false;
		}
	}while( bFound );

	bFound = false;
	do
	{
		PTListIter iter = std::find_if( m_lBackList.begin(), m_lBackList.end(), boost::bind( &CVerifyElem::m_pVerify, _1 ) == pPType );

		if( iter != m_lBackList.end() )
		{
			bFound = true;
			m_lBackList.erase( iter );
			iDeleteCnt++;
		}
		else
		{
			bFound = false;
		}
	}while( bFound );

	if( iDeleteCnt > 1 )
	{
	}
}

void ProtectedTypeMgr::DeleteAll()
{
    m_lFrontList.clear();
    m_lBackList.clear();
    m_lDeleteList.clear();
}

void ProtectedTypeMgr::CheckAll( void )
{
    Delete();

    PTListIter frontIter;
    for( frontIter = m_lFrontList.begin(); frontIter != m_lFrontList.end(); ++frontIter )
    {
        if( frontIter->m_pVerify->Verify() == false )
        {
            g_bHackSafe = false;
            break;
        }
    }

    PTListIter backIter;
    for( backIter = m_lBackList.begin(); backIter != m_lBackList.end(); ++backIter )
    {
        if( backIter->m_pVerify->Verify() == false )
        {
            g_bHackSafe = false;
            break;
        }
    }

    if( g_bHackSafe == false )
    {
        if( frontIter != m_lFrontList.end() )
            (*m_pErrfunc)( m_strProtectedTypeMgrTitle );
        else if( backIter != m_lBackList.end() )
            (*m_pErrfunc)( m_strProtectedTypeMgrTitle );
    }
}

#endif