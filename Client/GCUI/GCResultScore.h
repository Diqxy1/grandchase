#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KGCResultScore : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCResultScore );
	/// Default constructor
	KGCResultScore( void );
	/// Default destructor
	virtual ~KGCResultScore( void );
	/// Copy constructor
	KGCResultScore( const KGCResultScore& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCResultScore& operator=( const KGCResultScore& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		TEAM_CANABAN,
		TEAM_SERDIN,
		TEAM_NUM,
	};
protected:
	void ActionPerformed(const KActionEvent& event ) {};
	void PostChildDraw();
	void OnCreate();

public:
	void Init();
	void SetKill( int iKill );
	void SetDeath( int iDeath );
	void SetScore( int iKill, int iDeath );

protected:

protected:
    int		m_iKill;
	int		m_iDeath;
};

DEFINE_WND_FACTORY( KGCResultScore );
DECLARE_WND_FACTORY( KGCResultScore );
DECLARE_WND_FACTORY_NAME( KGCResultScore );