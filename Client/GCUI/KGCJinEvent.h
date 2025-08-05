#ifndef _KGCJINEVENT_H_
#define _KGCJINEVENT_H_

class KGCItemImgWnd;

class KGCJinEvent : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCJinEvent );
    /// Default constructor
    KGCJinEvent( void );
    /// Default destructor
    virtual ~KGCJinEvent( void );
    /// Copy constructor
    KGCJinEvent( const KGCJinEvent& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCJinEvent& operator=( const KGCJinEvent& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum { NUM_OF_JE_STAR = 5, };
	enum { JIN_EVENT_ITEMID = 13923, };

public:
    virtual void FrameMoveInEnabledState();

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void Initialize();
	virtual void PostChildDraw();

public:
	void SetTotalCount( int iTotalCount );
	int GetTotalCount() { return m_iTotalCnt; }

	void SetPollCount( int iPollCount)  { m_iPollCount = iPollCount; }
	int GetPollCount() { return m_iPollCount; }
	void SetItemImg( DWORD dwItemID);
	void OnClickUpArrowBtn();
	void OnClickDownArrowBtn();
	void OnExit();
	void OnOk();
	void RefreshMyItemCnt(void);

private:
	KD3DSizingBox*		m_pkitem_Img_View;
	KGCItemImgWnd*		m_pkCount_View;
	KSafeArray<KD3DWnd*,NUM_OF_JE_STAR> m_pkStar;
	KD3DWnd*			m_pkOk;
	KD3DWnd*			m_pkCancle;
	KD3DWnd*			m_pkUp;
	KD3DWnd*			m_pkDown;
	KSafeArray<KD3DStatic*,NUM_OF_JE_STAR> m_pkStatic;

	int					m_iMyItemCnt;
	int					m_iTotalCnt;
	int					m_iPollCount;
	int					m_iGoalCount[NUM_OF_JE_STAR];
};

DEFINE_WND_FACTORY( KGCJinEvent );
DECLARE_WND_FACTORY( KGCJinEvent );
DECLARE_WND_FACTORY_NAME( KGCJinEvent );

#endif //_KGCJINEVENT_H_