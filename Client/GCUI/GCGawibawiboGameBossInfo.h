#pragma once




class KGCGawibawiboGameBossInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:

    DECLARE_CLASSNAME( KGCGawibawiboGameBossInfo );
    /// Default constructor
    KGCGawibawiboGameBossInfo( void );
    /// Default destructor
    virtual ~KGCGawibawiboGameBossInfo( void );
    /// Copy constructor
    KGCGawibawiboGameBossInfo( const KGCGawibawiboGameBossInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGawibawiboGameBossInfo& operator=( const KGCGawibawiboGameBossInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetInfo( int iNowBoss,int iMaxBoss = -1);
	void PreDraw( void );
	void Destroy();
private:
	std::map<int,KD3DWnd*>	m_mapBossBtn;
	std::map<int,KD3DWnd*>	m_mapBossBk;
	std::map<int,KD3DWnd*>	m_mapBossClear;


	int			m_iMaxBoss; //보스 최대 갯수

    GCDeviceTexture* m_pTex;

	KD3DWnd 	*m_pkBossPoint; //보스 화살표

	float		m_fBossPointBetween; //보스 이미지 사이 간격

	void SetPosBetween(int iMaxBoss);
	void SetPosBossPoint(int iNowBoss);
	void SetPosBossInfo(int iNowBoss, bool bLockBossInfo);
	void SetBossBigImage( int iNowBoss);
	void SetInitBossInfo(int iNowBoss, bool bLockBossInfo);
	int IsFinalBoss(int index_)/*파이널 보스라면 파이널 보스 인덱스를 넘겨주고 아니라면 자신의 인덱스를 리턴한다. */;
};

DEFINE_WND_FACTORY( KGCGawibawiboGameBossInfo );
DECLARE_WND_FACTORY( KGCGawibawiboGameBossInfo );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGameBossInfo );
