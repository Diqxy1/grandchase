#pragma once

class KGCShortMenuCharInfo : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCShortMenuCharInfo );
	/// Default constructor
	KGCShortMenuCharInfo( void );
	/// Default destructor
	virtual ~KGCShortMenuCharInfo( void );
	/// Copy constructor
	KGCShortMenuCharInfo( const KGCShortMenuCharInfo& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShortMenuCharInfo& operator=( const KGCShortMenuCharInfo& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate();
	virtual void PostChildDraw();

private:

	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
	void SetGradeTex(int index, KD3DWnd* pWnd);

	enum
	{
		GRADE_SS = 0,
		GRADE_S_PLUS = 1,
		GRADE_S = 2,
		GRADE_S_MINUS = 3,	
		GRADE_A_PLUS = 4,
		GRADE_A = 5,
		GRADE_A_MINUS = 6,
		GRADE_B_PLUS = 7,
		GRADE_B = 8,
		GRADE_B_MINUS = 9,
		GRADE_C_PLUS = 10,
		GRADE_C = 11,
		GRADE_C_MINUS = 12,
		GRADE_D_PLUS = 13,
		GRADE_D = 14,
		GRADE_D_MINUS = 15,
		GRADE_E = 16,
		GRADE_F = 17,
		MAX_NUM_GRADE,
	};

	KD3DStatic*	m_pkCharLevel;
	KD3DStatic*	m_pkNickname;
	KD3DStatic* m_pkTotalAtk;
	KD3DStatic* m_pkTotalAtkVal;
	KD3DStatic* m_pkMatchRank;
	KD3DStatic*	m_pkWin;
	KD3DStatic* m_pkNoneRank;
	KD3DWnd*	m_pkMatchRankMark;
	KSafeArray<KD3DWnd*, MAX_NUM_GRADE> m_saCharGrade;
};

DEFINE_WND_FACTORY( KGCShortMenuCharInfo );
DECLARE_WND_FACTORY( KGCShortMenuCharInfo );
DECLARE_WND_FACTORY_NAME( KGCShortMenuCharInfo );