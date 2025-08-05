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


	int			m_iMaxBoss; //���� �ִ� ����

    GCDeviceTexture* m_pTex;

	KD3DWnd 	*m_pkBossPoint; //���� ȭ��ǥ

	float		m_fBossPointBetween; //���� �̹��� ���� ����

	void SetPosBetween(int iMaxBoss);
	void SetPosBossPoint(int iNowBoss);
	void SetPosBossInfo(int iNowBoss, bool bLockBossInfo);
	void SetBossBigImage( int iNowBoss);
	void SetInitBossInfo(int iNowBoss, bool bLockBossInfo);
	int IsFinalBoss(int index_)/*���̳� ������� ���̳� ���� �ε����� �Ѱ��ְ� �ƴ϶�� �ڽ��� �ε����� �����Ѵ�. */;
};

DEFINE_WND_FACTORY( KGCGawibawiboGameBossInfo );
DECLARE_WND_FACTORY( KGCGawibawiboGameBossInfo );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGameBossInfo );
