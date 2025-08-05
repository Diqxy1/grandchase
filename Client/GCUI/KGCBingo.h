#ifndef _KGC_BINGO_H_
#define _KGC_BINGO_H_
#define MAX_BOX_SIZE 10
class KGCBingo : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCBingo );

	KGCBingo(void);
	virtual ~KGCBingo(void);
	/// Copy constructor
	KGCBingo( const KGCBingo& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBingo& operator=( const KGCBingo& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroy( void );
	void OnExit();
	//빙고인지 체크하는 함수
	void CheckBingo(int iWidth, int iHeight);
	void LineCheckBingo(int iWidth, int iHeight,std::vector<int>& vecBingo, bool bRightCross = false);
	void SetStatic();
	void ClearBingoVector();
	void ShowBingoEffect();
	void DrawBingoEffect();

	inline void SetAccPlayerTime(int AccPlayTime) { m_iAccPlayTime = AccPlayTime;}
	inline const int GetAccPlayerTime() { return m_iAccPlayTime;}
	
	inline void SetTotalBingoCoinCount(int TotalBingoCoinCount) { m_iTotalBingoCoinCount = TotalBingoCoinCount;}
	inline void SetUseBingoCoinCount(int UseBingoCoinCount){ m_iUseBingoCoinCount = UseBingoCoinCount;}
	inline void SetBingoCoinCount(int BingoCoinCount) { m_iRemainCoin = BingoCoinCount;}
	
	inline void SetCorrectQuestions( std::vector<int> CorrectQuestions){ m_vecCorrectQuestions = CorrectQuestions;}
	inline void SetBingoData( std::vector<std::pair<int,int>> BingoData){ m_vecBingoBoardPosXY = BingoData;}
	inline void SetFinishToday(bool FinishToday) { m_bFinishToday = FinishToday;}
	inline void SetCurrentBoxLock(int Width, int Height){ m_pkBingoBox[Width][Height]->Lock(true); }

	inline std::pair<int,int> GetCurrentBingoPosition(){return m_pBingoPosition;}
	inline void PushBingoData(std::pair<int,int> BingoBoardPos) { m_vecBingoBoardPosXY.push_back(BingoBoardPos);}

	inline const int GetRemainCoin() { return m_iRemainCoin;}


private:
	KD3DWnd*		m_pkCloseBtn;
	KD3DWnd*        m_pkActiveBingo;
	KSafeArray<KSafeArray<KD3DWnd*,MAX_BOX_SIZE>,MAX_BOX_SIZE> m_pkBingoBox;   
   
	//각 빙고칸 상태.
	std::vector<int> m_vecWidthBingo; 
	std::vector<int> m_vecHeightBingo;
	std::vector<int> m_vecCrossBingo;

	KD3DStatic*		m_pkStaticRemainCoin;
	KD3DStatic*		m_pkStaticRemainTime;
	KD3DStatic*		m_pkStaticBingoNumber;
	KD3DStatic*		m_pkStaticBingoDesc;
	
	bool    m_bFinishToday;
	bool    m_bPlaybingoSound;
	bool    m_bBingoEffect;

	int     m_iActiveBingoTime;

	int     m_iRemainCoin;
	int     m_iRemainTime;
	
	int    m_iAccPlayTime;
	int    m_iTotalBingoCoinCount;
	int    m_iUseBingoCoinCount;
	int    m_iBingoCoinCount;
	
	std::vector<int>    m_vecCorrectQuestions;
	std::vector<std::pair<int,int> >    m_vecBingoBoardPosXY;
	std::pair<int, int> m_pBingoPosition;	
};

DEFINE_WND_FACTORY( KGCBingo );
DECLARE_WND_FACTORY( KGCBingo );
DECLARE_WND_FACTORY_NAME( KGCBingo );


#endif