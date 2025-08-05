#pragma once

class KGCDungeonBossInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonBossInfoS6 );
    /// Default constructor
    KGCDungeonBossInfoS6( void );
    /// Default destructor
    virtual ~KGCDungeonBossInfoS6( void );
    /// Copy constructor
    KGCDungeonBossInfoS6( const KGCDungeonBossInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonBossInfoS6& operator=( const KGCDungeonBossInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void Destroy();
	void SetBossImage( );
	void PreDraw();
	void SetBoss( );
	void SetRecom( );
	void SetRecomUser( );
	void SetRecomLevel( );
	void SetNumberTex( int index, KD3DWnd *pWnd );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
	void SetRecomColor();
    void SetBossRoom( bool bRoom ) { m_bRoom = bRoom; }
    void SetRankImage();
private:

	GCDeviceTexture*    m_pBossTexture;

	//텍스쳐의 기본 시작 좌표
	D3DXVECTOR2 m_vBaseTexStartAxis;

	//텍스쳐 두번째 이미지 시작 좌표
	D3DXVECTOR2 m_vSecondTexStartAxis;

	//텍스쳐 한 이미지의 사이즈
	D3DXVECTOR2 m_vTexSize;

	//그릴 텍스쳐의 좌표
	D3DXVECTOR2 m_vImageTexStartAxis;
	D3DXVECTOR2 m_vImageTexEndtAxis;

	//그릴 UI위치
	D3DXVECTOR2 m_vImagePos;

	//텍스쳐 크기
	D3DXVECTOR2 m_vTextureFullSize;


    KD3DWnd* m_pkDungeonRank; //던전 랭크
	KD3DWnd *m_pkRecomLevelLow_X0; //추천최소레벨 10자리
	KD3DWnd *m_pkRecomLevelLow_0X; //추천최소레벨 1자리
	KD3DWnd *m_pkRecomLevelHigh_X0; //추천최고레벨 10자리
	KD3DWnd *m_pkRecomLevelHigh_0X; //추천최고레벨 1자리
	KD3DWnd *m_pkRecomUserMin; //최소추천 인원
	KD3DWnd *m_pkRecomUserMax; //최대추천 인원

	KD3DWnd *m_pkLevel; //최대추천 인원	
	KD3DWnd *m_pkPlayer; //최대추천 인원
	KD3DWnd *m_pkPlayer_; //최대추천 인원
	KD3DWnd *m_pkLevel_; //최대추천 인원

	//최소레벨,최고레벨
	std::pair<int,int> m_pairCurrentRecomLevel;
	//최소유져, 최대인원
	std::pair<int,int> m_pairCurrentRecomUser;
    

public:

    bool m_bRoom;

//	DWORD m_dwTextColor;
};

DEFINE_WND_FACTORY( KGCDungeonBossInfoS6 );
DECLARE_WND_FACTORY( KGCDungeonBossInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCDungeonBossInfoS6 );
