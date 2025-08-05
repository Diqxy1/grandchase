#pragma once

#if defined(PROTECTEDTYPE_NEW)
#include "ProtectedType_new.h"


#else

#include <WTypes.h>

#define PTM_NORMAL_REG( p, q ) g_pPTypeMgr[ProtectedTypeMgr::PTMK_NORMAL]->Register( p, q, L#q )
#define PTM_ITEMS_REG( p, q ) g_pPTypeMgr[ProtectedTypeMgr::PTMK_ITEMS]->Register( p, q, L#q )

#define PTM_NORMAL_DEL( q ) g_pPTypeMgr[ProtectedTypeMgr::PTMK_NORMAL]->ReserveDelete( q )
#define PTM_ITEMS_DEL( q ) g_pPTypeMgr[ProtectedTypeMgr::PTMK_ITEMS]->ReserveDelete( q )

#define PTM_LIST_ADD( p, q ) push_back( CVerifyElem(p, q, L#q) )

#define PTM_ELAPSED_TIME_MAX ((DWORD)18)

class IVerify
{
public:
    IVerify() {}
    virtual ~IVerify() {}

    virtual bool Verify() const = 0;
    virtual void SecurityUpdate() = 0;
};

class CVerifyElem
{
public:
    typedef bool securityupdate;

    securityupdate  m_bSecurity;
    IVerify*        m_pVerify;

    std::wstring    m_strLog;

    CVerifyElem( securityupdate bSecurity, IVerify& pVerify, const WCHAR * strLog )
        : m_bSecurity( bSecurity ), m_pVerify( &pVerify ), m_strLog( strLog ) 
    {}

	bool operator < ( const CVerifyElem& _Right )
    {
        // reference by std::pair::operator <
        return (m_bSecurity < _Right.m_bSecurity ||
            !(_Right.m_bSecurity < m_bSecurity) && m_pVerify < _Right.m_pVerify);
    }


	CVerifyElem(const CVerifyElem &src)
	{
		this->m_bSecurity = src.m_bSecurity;
		this->m_pVerify = src.m_pVerify;
		this->m_strLog = src.m_strLog;
	}

	const CVerifyElem& operator =(const CVerifyElem &src)
	{
		this->m_bSecurity = src.m_bSecurity;
		this->m_pVerify = src.m_pVerify;
		this->m_strLog = src.m_strLog;
		return *this;
	}
private:
    CVerifyElem() {}
};

struct VerifyCompare :
	public std::binary_function<CVerifyElem, CVerifyElem, bool>
{
	bool operator()(const CVerifyElem& lhs, const CVerifyElem& rhs) const
	{
		return (lhs.m_bSecurity < rhs.m_bSecurity ||
			!(rhs.m_bSecurity < lhs.m_bSecurity) && lhs.m_pVerify < rhs.m_pVerify);
	}
};


class ProtectedTypeMgr
{
public:
    enum PT_MGR_KIND
    {
        PTMK_NORMAL = 0,
        PTMK_ITEMS = 1,

        PTMK_NUM,
    };

    typedef void (*ERRORFUNC)( std::wstring strLog );
    typedef std::list< CVerifyElem >::iterator PTListIter;

public:
    ProtectedTypeMgr(void);
    ~ProtectedTypeMgr(void);

    static bool g_bHackSafe;

    void Register( bool bSecurity, IVerify& pType, const WCHAR * strLog );
    void Register( CVerifyElem verifyPair );
    void Register( std::list< CVerifyElem > list );
    void ReserveDelete( IVerify& pPType );
    void ReserveDelete( std::list< IVerify* > pPType );

    void SetVerifyCount( const UCHAR& Count_ ) { m_ucVerifyCount = Count_; }
    UCHAR GetVerifyCount() { return m_ucVerifyCount; }

    void RegisterErrfunc( const ERRORFUNC pf )
    {
        m_pErrfunc = pf;
    }

    void FrameMove( void );

    void CheckAll( void );

    size_t GetFrontSize() const { return m_lFrontList.size(); }
    size_t GetBackSize() const { return m_lBackList.size(); }

private:
    void Delete();
    void Delete( IVerify*& verifyPair );
    void DeleteAll();

    std::list< CVerifyElem > m_lFrontList;       // Verify 불리지 않은 녀석들
    std::list< CVerifyElem > m_lBackList;        // Verify 불린 녀석들
    std::list< IVerify* >    m_lDeleteList;      // 지워져야할 녀석들

    UCHAR                   m_ucVerifyCount;    // 한번에 Verify 할 수

    ERRORFUNC               m_pErrfunc;
    std::wstring            m_strProtectedTypeMgrTitle;
};

extern ProtectedTypeMgr* g_pPTypeMgr[ProtectedTypeMgr::PTMK_NUM];

#endif
