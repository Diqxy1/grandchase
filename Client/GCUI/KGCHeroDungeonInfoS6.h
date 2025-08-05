#pragma once

class KGCHeroDungeonInfoListS6;
class KGCHeroDungeonOpentimeBoxS6;

class KGCHeroDungeonInfoS6 : public KD3DDialog,
												  public KActionListener
													
{
public:
	DECLARE_CLASSNAME( KGCHeroDungeonInfoS6 );
	/// Default constructor
	KGCHeroDungeonInfoS6( void );
	/// Default destructor
	virtual ~KGCHeroDungeonInfoS6( void );
	/// Copy constructor
	KGCHeroDungeonInfoS6( const KGCHeroDungeonInfoS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHeroDungeonInfoS6& operator=( const KGCHeroDungeonInfoS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
	void OnExit();

public:
	int GetListViewCount(int iIndex_);

private:
	enum
	{
		EMAX_DUNGEON_LIST_NUM = 11,
	};    

	KSafeArray<KGCHeroDungeonInfoListS6*, EMAX_DUNGEON_LIST_NUM> m_pkDungeonInfoSlot;
	
	KD3DWnd* m_pkBtnClose;
	KD3DWnd* m_pkBtnLeft;
	KD3DWnd* m_pkBtnRight;
};

DEFINE_WND_FACTORY( KGCHeroDungeonInfoS6 );
DECLARE_WND_FACTORY( KGCHeroDungeonInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonInfoS6 );