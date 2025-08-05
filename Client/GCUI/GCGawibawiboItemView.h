#pragma once

class KGCGawibawiboItemView : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGawibawiboItemView );
    /// Default constructor
    KGCGawibawiboItemView( void );
    /// Default destructor
    virtual ~KGCGawibawiboItemView( void );
    /// Copy constructor
    KGCGawibawiboItemView( const KGCGawibawiboItemView& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGawibawiboItemView& operator=( const KGCGawibawiboItemView& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void	SetReward(const std::map< std::pair< DWORD, DWORD >, VEC_REWARD > &reward){m_mapWinReward.clear();m_mapWinReward = reward;}
	void	SetClearCount(int cnt_){m_iClearCnt = cnt_;}
	void	SetBossIndex(int index_){m_iSelectBoss = index_;}
	void	SetMaxBossCnt(int cnt_){m_iMaxBossCnt = cnt_;}
	void	SetRewardSetting();
	void	SetBossSetting();
	void	SetView();
	void	SetDefaultRwardItem(int index_){m_iDefaultRewardItem = index_;}
	void	PostChildDraw();

private:
	std::map<int, KD3DWnd*>		m_mapBossImage;
	std::map<int, KD3DWnd*>		m_mapBossBox;
	KD3DSizingBox*				m_pkBossBoxBK;
	KD3DSizingBox*				m_pkBossFrame;


	std::map<int, KD3DWnd*>		m_mapItemBox;
	KD3DSizingBox*				m_pkItemFrame;
	KD3DSizingBox*				m_pkItemBoxBK;

	KD3DStatic*					m_staticDescriptoin1;
	KD3DStatic*					m_staticDescriptoin2;
	KD3DSizingBox*				m_pkStringBox;

	KD3DWnd*					m_pkTitle;
	KD3DWnd*					m_pkBtnOK;
	
	int							m_iClearCnt;
	int							m_iSelectBoss;
	int							m_iMaxBossCnt;
	std::map< std::pair< DWORD, DWORD >, VEC_REWARD > m_mapWinReward; // 승리 보상 : < 스테이지, 몬 번호 >, 보상리스트

	int							m_iDefaultRewardItem;
	
	std::vector<GCDeviceTexture*>	m_vecItemImage; //아이템 이미지들
	void ClearItemImage();
	void SetBtnLock();
	void DrawItemInfo();
};

DEFINE_WND_FACTORY( KGCGawibawiboItemView );
DECLARE_WND_FACTORY( KGCGawibawiboItemView );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboItemView );
