// NetHackCheck.h: interface for the CNetHackCheck class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_NETHACKCHECK_H__1E505403_C3EB_4F0C_9537_9BBE61470C5F__INCLUDED_)
//#define AFX_NETHACKCHECK_H__1E505403_C3EB_4F0C_9537_9BBE61470C5F__INCLUDED_

#ifndef _NETHACKCHECK_H_
#define _NETHACKCHECK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatencyTransfer.h"

class CNetHackCheck  
{
    public:
	    CNetHackCheck();
	    virtual ~CNetHackCheck();

        void FrameMove( float fElapsedTime );

    private:
        float   m_fElapsedTime;
        float   m_fCheckElapsed;
};

#endif // _NETHACKCHECK_H_
