#ifndef _KGC_PROGRESSBTN_H_
#define _KGC_PROGRESSBTN_H_
#pragma once

class KGCProgressBtn : public KD3DWnd//, // extends
	//public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCProgressBtn );

	KGCProgressBtn(void);
	virtual ~KGCProgressBtn(void);
	//virtual void ActionPerformed( const KActionEvent& event );
	/// Copy constructor
	KGCProgressBtn( const KGCProgressBtn& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCProgressBtn& operator=( const KGCProgressBtn& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void FrameMoveInEnabledState( void );
	virtual void OnCreate( void );	
//	virtual void PostDraw( void );

	KD3DMoveWnd* m_pkBackImg;

public:
	
	void SetProgressTime(DWORD dwStartTime,DWORD dwLimitTime = 0)
	{
		m_dwStartTime=dwStartTime;
		if (dwLimitTime==0)
		{
			dwLimitTime=dwStartTime+ TIME_FOR_PREESURE;
		}
		m_dwLimitTime=dwLimitTime;
	}
	void SetSameExtentionSize ( float &fCurrentSize, float fMaxSize, float &fCurrentTexSize, float fMaxTexSize );
	enum {
		KUIPROGBTN_FINISH = K3DWE_USER + 1,
	};

private:
	//DWORD m_dwElapsedTime;
	DWORD m_dwStartTime;
	DWORD m_dwLimitTime;
	DWORD m_iOriginalWidth;
	D3DXVECTOR4 m_avTexDef;
	DWORD m_dwPreWidth;
	DWORD m_dwPreTexWidth;
};

DEFINE_WND_FACTORY( KGCProgressBtn );
DECLARE_WND_FACTORY( KGCProgressBtn );
DECLARE_WND_FACTORY_NAME( KGCProgressBtn );
#endif