#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../GCItemManager.h"
#include "KGCTitleSlotRow.h"

class KGCTitleSlotRow;
class KD3DScrollbar;

struct STitleItem
{
	int		m_iPrevTitleID;
	int		m_iMissionID;
	int		m_iExtraMissionID;
	int		m_iTitleItemID;
	float	m_fHpRecoveryIncrease;
	float	m_fStateConfuseDecrease;
	float	m_fArenaTimeIncrease;
	KGCTitleSlotRow::TITLE_SLOT_BTN		m_eState;
	GCDeviceTexture*	m_pTexture;
	STitleItem();
	void SetTexture( int iTitleID = -1 );
	void ReleaseTexture();
};

class KGCTitleWindow : public KD3DWnd,          // extends
                        public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTitleWindow );
    /// Default constructor
    KGCTitleWindow( void );
    /// Default destructor
    virtual ~KGCTitleWindow( void );
    /// Copy constructor
    KGCTitleWindow( const KGCTitleWindow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTitleWindow& operator=( const KGCTitleWindow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum { NUM_OF_TITLE_ROW = 4 };

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );

    void RefreshTitleIndex( int iIndex );
    void RefreshAllTitleIndex( void );
    void ClearAllTitleIndex( void );

    int GetFirstTitleIndex( void );     // 왼쪽 위에 가장 첫번째 칭호의 인덱스 번호 리턴

    void RefreshTitleList(void);
    int GetSlotIndexByIndex(int iIndex);
    void LoadingItemList(void);

    bool IsTitleItemID(const int iItemID);

    bool IsTitleItemListIndex(const int iTitleID);
    int GetTitleItemCount(void) { return (int)m_mapTitleItem.size(); }

	// 칭호 아이템의 능력치 상승 값을 리턴 해줍니다.
	float GetTitleItemAbilityPercent(const int iTitleID, const COLLECTION_TITLE_ABILITY_TYPE enType);

	// 칭호의 index를 넘겨주면, fAbilityValue에 그 칭호의 능력치 상승 값을 적용해 리턴합니다.
	float GetTitleCalcDecreaseAbility(const int iIndex, const float fAbilityValue);

	// 칭호의 아이디를 넘겨주면, 해당 칭호에 현재 몇 %의 패널티가 적용되어 있는지 계산해줍니다.
	float GetTitleDecreasePercentByItemID(const int iTitleID);	

	bool IsCompleteCTMissionAndNotGet( void );

	void CreateTitleShopImage(void);
	void DestroyTitleShoImage(void);

	bool IsCurrentUseMissionID( const int iTitleID );

private:
    KSafeArray<KGCTitleSlotRow*,NUM_OF_TITLE_ROW> m_pkTitleSlotRow;

    KD3DWnd*                m_pkClose;
    KD3DScrollbar*          m_pkScrollBar;

    std::vector< int >         m_vecTitleList;				// <미션아이디, 소유상태>

	typedef std::map<int, STitleItem> MapTitleItem;
	std::map< int, STitleItem >		m_mapTitleItem;

    KD3DWnd*    m_pkEmoticonBtn;
    KD3DWnd*    m_pkSelTitleBtn;
    KD3DWnd*    m_pkPetBtn;
    KD3DWnd*    m_pkMonsterCardBook;

    int         m_iRow;

private:
	// GetTitleItemAbilityPercent 의 내부에서 사용되며, 능력치 상승 값을 리턴해줍니다.
	float GetTitleHpRecoveryPercent(const int iTitleID);
	float GetTitleStateConfusePercent(const int iTitleID);
	float GetTitleArenaTimePercent(const int iTitleID);

	// 칭호의 index를 넘겨주면 0~index 까지의 칭호중 없는 칭호 개수를 리턴합니다.
	int GetNotMyPreviousTitleCount(const int iTitleID);
};

DEFINE_WND_FACTORY( KGCTitleWindow );
DECLARE_WND_FACTORY( KGCTitleWindow );
DECLARE_WND_FACTORY_NAME( KGCTitleWindow );
