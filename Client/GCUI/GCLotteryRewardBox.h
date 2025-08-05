#pragma once

class KGCLotteryRewardBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCLotteryRewardBox );
    /// Default constructor
    KGCLotteryRewardBox( void );
    /// Default destructor
    virtual ~KGCLotteryRewardBox( void );
    /// Copy constructor
    KGCLotteryRewardBox( const KGCLotteryRewardBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLotteryRewardBox& operator=( const KGCLotteryRewardBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum{
		NUM_GRADE = 3
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void PostChildDraw();

	void SetGoods(GCITEMID gcItemID_, int iLevel_, int iGrade_ );
	void SetNothing();
    bool IsNothing( void ) { return m_pTexItem == NULL; }
	GCITEMID GetItemID()	{ return m_gcItemID; }
	int	GetEquipLevel()		{ return m_iLevel; }


private:
	KD3DWnd*				m_pkGrade[NUM_GRADE];
	KD3DWnd*				m_pkImgRect;
	GCDeviceTexture*		m_pTexItem;

	int			m_iGrade;
	int			m_iLevel;
	GCITEMID	m_gcItemID;
};

DEFINE_WND_FACTORY( KGCLotteryRewardBox );
DECLARE_WND_FACTORY( KGCLotteryRewardBox );
DECLARE_WND_FACTORY_NAME( KGCLotteryRewardBox );
