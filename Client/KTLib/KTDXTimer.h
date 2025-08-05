// KTDXTimer.h: interface for the CKTDXTimer class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KTDXTIMER_H__DA566AF2_7E6F_4C09_AC49_FABA3BED20BA__INCLUDED_)
//#define AFX_KTDXTIMER_H__DA566AF2_7E6F_4C09_AC49_FABA3BED20BA__INCLUDED_

#ifndef _KTDXTIMER_H_
#define _KTDXTIMER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTDXTimer
{
	public:
		enum TIMER_TYPE
		{
			LOOP,
			ONESHOT,
		};

		struct str_TimerData
		{
			int			id;
			TIMER_TYPE	type;
			float		fElapsedTime;
			float		fAddTime;
            float       fStartDelay;
            float       fEndDelay;
			bool		timeOut;
		};

	public:
		CKTDXTimer();
		virtual ~CKTDXTimer();

		virtual HRESULT FrameMove( float fElapsedTime, void* pframeData = NULL );

		void SetTimer( int id, int time, TIMER_TYPE type = ONESHOT, int startDelay = 0, int EndDelay = 0 );
		bool DestroyTimer( int id );
		bool CheckTimer( int id );

		void AutoFlow( bool autoFlow ){ m_AutoFlow = autoFlow; }
		void TimeFlow( float fElapsedTime );

	private:
		vector<str_TimerData*> m_TimerList;
		vector<int> m_DeleteTimerList;
		bool m_AutoFlow;
};

#endif // _KTDXTIMER_H_
