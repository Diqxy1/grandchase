#ifndef _KGC_QUIZ_H_
#define _KGC_QUIZ_H_

#define MAX_QUESTION 4

class KGCQuiz : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCQuiz );

	KGCQuiz(void);
	virtual ~KGCQuiz(void);
	/// Copy constructor
	KGCQuiz( const KGCQuiz& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCQuiz& operator=( const KGCQuiz& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroy( void );

	void OnExit();
	void OnClose();

	void SetQuizInfo(KBingoQuestionInfo kQuestion);
	void SetStatic();

	void DrawAnswerResult();
	void InPutAnswerAndResurrection(bool bResurrection);

	void GetPreAnswerVectorclear(); 

	//FramMove에 들어갈 함수들
	void CalAnswerRemainTime();
	void OnAnswerResult();
	void OnInputAnswer();
	void DrawClockEffect();
	void OnNoAnswerQuiz();


	inline void SetResolved(bool bResolve){m_bResolved =bResolve;}
	inline void SetRemainDrawAnswerResult(int Time){ m_DrawAnswerTime = Time;}
	inline void SetResurrectionBonus(int Bonus){m_iReseructionBouns = Bonus;}
	inline void SetRemainTime(int RemainTime){ m_iRemainTime = RemainTime;}

	inline const int GetAnswer(){return m_iAnswer;}
	inline const int GetRemainTime(){return m_iRemainTime;}
	inline const bool GetResolved() { return m_bResolved;}

	void LockQuestionList(bool bLock);

	void ShowReserructionEffect();
	void ShowClockEffect();

	void OnHintSearch();

private:
	KD3DWnd*        m_pkClose;
	KD3DWnd*        m_pkCircle;
	KD3DWnd*        m_pkCross;
	KD3DWnd*        m_pkQuiz;
	KD3DWnd*        m_pkHintBtn;

	//애들 클래스로 다시 만들어야될듯..<-시간 지나고 보니 내가 이때 무슨 생각으로 이걸 적어놨는지 모르겠다
	KD3DStatic*		m_pkStaticQuiz;
	KD3DStatic*     m_pkStaticRemainResuretion;
	KD3DStatic*     m_pkStaticRemainTime;


	KSafeArray<KD3DWnd*,MAX_QUESTION>        m_pkCheck;
	KSafeArray<KD3DStatic*,MAX_QUESTION>        m_pkStaticQuestion;
	KSafeArray<KD3DSizingBoxBtn*,MAX_QUESTION>        m_pkQuestion;
	KSafeArray<std::wstring,MAX_QUESTION>        m_QuestionString;

	//현재 시간을 받는다.
	DWORD                                       m_dwAccTimeNot;

	bool            m_bResolved;
	bool m_bResurrection;
	bool m_bClockEffect;

	int m_iAnswer;
	int m_iReseructionBouns;    //부활 찬스
	int m_DrawAnswerTime;    //남은 시간
	int m_iRemainTime;
	int m_iQuizNumber;
	int m_iCorrectAnswer;
	int m_iMaxReseructionNum;

	std::map <int, std::wstring> m_mapChoiceWstring;

	std::vector<int> m_vecPreAnwer;

	std::wstring m_wstrQuestion;

	CParticleEventSeqPTR	m_pClockEffect1;
	CParticleEventSeqPTR	m_pClockEffect2;

};

DEFINE_WND_FACTORY( KGCQuiz );
DECLARE_WND_FACTORY( KGCQuiz );
DECLARE_WND_FACTORY_NAME( KGCQuiz );


#endif