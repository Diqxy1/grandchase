#include "stdafx.h"
#include "GCDungeonBossInfoS6.h"
#include "KGCWorldMapManager.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCDungeonBossInfoS6 );
IMPLEMENT_WND_FACTORY( KGCDungeonBossInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonBossInfoS6, "gc_dungeon_boss_info_s6" );

KGCDungeonBossInfoS6::KGCDungeonBossInfoS6( void )
: m_pBossTexture(NULL)

, m_vTexSize(234.f, 117.f)
, m_vBaseTexStartAxis(1.f,1.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(238.f, 120.f)

, m_pkRecomLevelLow_X0(NULL)
, m_pkRecomLevelLow_0X(NULL)
, m_pkRecomLevelHigh_X0(NULL)
, m_pkRecomLevelHigh_0X(NULL)
, m_pkRecomUserMin(NULL)
, m_pkRecomUserMax(NULL)
, m_pkLevel(NULL)
, m_pkPlayer(NULL)
, m_pkLevel_(NULL)
, m_pkPlayer_(NULL)
, m_bRoom( false )
, m_pkDungeonRank(NULL)
{
	LINK_CONTROL("level_low_x0",m_pkRecomLevelLow_X0);
	LINK_CONTROL("level_low_0x",m_pkRecomLevelLow_0X);
	LINK_CONTROL("level_high_x0",m_pkRecomLevelHigh_X0);
	LINK_CONTROL("level_high_0x",m_pkRecomLevelHigh_0X);
	LINK_CONTROL("recom_low_0x",m_pkRecomUserMin);
	LINK_CONTROL("recom_high_0x",m_pkRecomUserMax);
	LINK_CONTROL("level",m_pkLevel);
	LINK_CONTROL("player",m_pkPlayer);
	LINK_CONTROL("level_",m_pkLevel_);
	LINK_CONTROL("recom_",m_pkPlayer_);
    LINK_CONTROL("rank_img", m_pkDungeonRank);

	m_pairCurrentRecomLevel.first = -1;
	m_pairCurrentRecomLevel.second = -1;
	m_pairCurrentRecomUser.first = -1;
	m_pairCurrentRecomUser.second = -1;
}

KGCDungeonBossInfoS6::~KGCDungeonBossInfoS6( void )
{
}

void KGCDungeonBossInfoS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCDungeonBossInfoS6::OnCreate( void )
{
    SetBoss();
}

void KGCDungeonBossInfoS6::Destroy()
{
	SAFE_RELEASE( m_pBossTexture );
}

void KGCDungeonBossInfoS6::SetBoss( ) 
{
    SetBossImage();
    SetRecom();
}

void KGCDungeonBossInfoS6::SetBossImage( )
{
	int iFileNum = 0;
	int iPictureNum = 0;
	bool bHellMod = false;

	m_pkLevel->InitState(true);
    m_pkPlayer->InitState(true);
    m_pkLevel_->InitState(true);
    m_pkPlayer_->InitState(true);

    bool bOK = false;
    if ( m_bRoom ) 
    {
	    bOK = SiKGCRoomListManager()->GetRoomBossBigImageInfo(iFileNum, iPictureNum);
    }
    else
    {
        bOK = SiKGCRoomListManager()->GetBossBigImageInfo(iFileNum, iPictureNum, bHellMod);
    }

    if ( !bOK ) 
    {
        SAFE_RELEASE( m_pBossTexture );
        return;
    }


	char cBuf[256];
	memset(cBuf,'\0',256);

    if ( m_bRoom ) 
    {
        sprintf(cBuf,"room_dungeon_boss_image_%d.dds",iFileNum);
    }
    else
    {
    	sprintf(cBuf,"dungeon_boss_image_%d.dds",iFileNum);
    }

	SAFE_RELEASE( m_pBossTexture );

	m_pBossTexture = g_pGCDeviceManager2->CreateTexture(cBuf);

    if ( m_bRoom ) 
    {
        m_vTexSize = D3DXVECTOR2(255.f, 127.f);
        m_vBaseTexStartAxis = D3DXVECTOR2(0.f,0.f);
        m_vTextureFullSize = D3DXVECTOR2(512.f,512.f);
        m_vSecondTexStartAxis= D3DXVECTOR2(256.f, 128.f);
    }
    else
    {
        m_vTexSize = D3DXVECTOR2(234.f, 117.f);
        m_vBaseTexStartAxis = D3DXVECTOR2(1.f,1.f);
        m_vTextureFullSize = D3DXVECTOR2(512.f,512.f);
        m_vSecondTexStartAxis= D3DXVECTOR2(238.f, 120.f);
    }


	//보스 텍스쳐 위치 정하기
    if ( m_bRoom ) 
    {
        int nX = iPictureNum / 4;
        int nY = iPictureNum % 4;

        m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;

        m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;

        m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
        m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    }
    else
    {
        m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x ) / m_vTextureFullSize.x;

        m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(iPictureNum) ) ) / m_vTextureFullSize.y;

        m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x ) / m_vTextureFullSize.x;
        m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(iPictureNum) ) ) / m_vTextureFullSize.y;
    }
}

void KGCDungeonBossInfoS6::PreDraw()
{
	if(m_pBossTexture == NULL) return;

	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

	//UI표시할 위치 정하기
	m_vImagePos.x = vPos.x;
	m_vImagePos.y = vPos.y;

    int nWidth = GetWidth();
    int nHeight = GetHeight();

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->DrawInScreen(m_pBossTexture,
		m_vImagePos.x, 
		m_vImagePos.y,
		m_vImagePos.x + nWidth * m_fWindowScaleX, 
		m_vImagePos.y + nHeight * m_fWindowScaleY,
		m_vImageTexStartAxis.x,
		m_vImageTexStartAxis.y, 
		m_vImageTexEndtAxis.x,
		m_vImageTexEndtAxis.y,
		false,10.0f,D3DCOLOR_ARGB(255,255,255,255), false);
	g_pGCDeviceManager2->PopState();
}

void KGCDungeonBossInfoS6::SetRecom()
{
	SetRecomLevel();
	SetRecomUser();
	SetRecomColor();
	m_pkRecomLevelLow_X0->ToggleRender(true); //추천최소레벨 10자리
	m_pkRecomLevelLow_0X->ToggleRender(true); //추천최소레벨 1자리
	m_pkRecomLevelHigh_X0->ToggleRender(true); //추천최고레벨 10자리
	m_pkRecomLevelHigh_0X->ToggleRender(true); //추천최고레벨 1자리
	m_pkRecomUserMin->ToggleRender(true); //최소추천 인원
	m_pkRecomUserMax->ToggleRender(true); //최대추천 인원

    SetRankImage(); 
}

void KGCDungeonBossInfoS6::SetRecomUser()
{
    int nLevel = 0;
	int nGameMode = 0;
    if ( m_bRoom ) 
    {
        nLevel = SiKGCRoomManager()->GetDungeonLevel();
		nGameMode = static_cast<int>(SiKGCRoomManager()->GetGameMode());
    }
    else
    {
        nLevel = SiKGCRoomListManager()->GetCurrentLevel();
		nGameMode = static_cast<int>(SiKGCWorldMapManager()->GetGameMode());
    }

    std::pair<int,int> pTemp(SiKGCRoomListManager()->GetRecomUser( nGameMode ,nLevel ));
    if( pTemp.first != m_pairCurrentRecomUser.first){
        SetNumberTex(pTemp.first , m_pkRecomUserMin);
    }
    if( pTemp.second != m_pairCurrentRecomUser.second){
        SetNumberTex(pTemp.second , m_pkRecomUserMax);
    }
    m_pairCurrentRecomUser.first = pTemp.first;
    m_pairCurrentRecomUser.second = pTemp.second;
}

void KGCDungeonBossInfoS6::SetRecomLevel()
{
    int nLevel = 0;
    EGCGameMode emGameMode = GC_GM_INVALID; 
    if ( m_bRoom ) 
    {
        emGameMode = SiKGCRoomManager()->GetGameMode();
        nLevel = SiKGCRoomManager()->GetDungeonLevel();
    }
    else
    {
        emGameMode = SiKGCWorldMapManager()->GetGameMode();
        nLevel = SiKGCRoomListManager()->GetCurrentLevel();
    }


	QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( emGameMode, nLevel );
	
	if( kQuestFileInfo.iLowLevel != m_pairCurrentRecomLevel.first){
		int iNum = kQuestFileInfo.iLowLevel / 10;
		SetNumberTex(iNum , m_pkRecomLevelLow_X0);
		iNum = kQuestFileInfo.iLowLevel % 10;
		SetNumberTex(iNum , m_pkRecomLevelLow_0X);
	}

	if( kQuestFileInfo.iHighLevel != m_pairCurrentRecomLevel.second){
		int iNum = kQuestFileInfo.iHighLevel / 10;
		SetNumberTex(iNum , m_pkRecomLevelHigh_X0);
		iNum = kQuestFileInfo.iHighLevel % 10;
		SetNumberTex(iNum , m_pkRecomLevelHigh_0X);
	}

	m_pairCurrentRecomLevel.first = kQuestFileInfo.iLowLevel;
	m_pairCurrentRecomLevel.second = kQuestFileInfo.iHighLevel;
}

void KGCDungeonBossInfoS6::SetRecomColor()
{
    EGCGameMode emGameMode = GC_GM_INVALID; 
    if ( m_bRoom ) 
    {
        emGameMode = SiKGCRoomManager()->GetGameMode();
    }
    else
    {
        emGameMode = SiKGCWorldMapManager()->GetGameMode();
    }


	DWORD dwColor = 0xffffffff;
	if ( SiKGCWorldMapManager()->IsHeroDungeonMode(emGameMode) ) {
		dwColor = 0xffffD200;
	}
	else if ( SiKGCWorldMapManager()->IsRaidHeroDungeon(emGameMode) ) {
		dwColor = 0xffff7e00;
	}

	m_pkRecomLevelLow_X0->SetWindowColor(dwColor);
	m_pkRecomLevelLow_0X->SetWindowColor(dwColor);
	m_pkRecomLevelHigh_0X->SetWindowColor(dwColor);
	m_pkRecomLevelHigh_X0->SetWindowColor(dwColor);
	m_pkRecomUserMin->SetWindowColor(dwColor);
	m_pkRecomUserMax->SetWindowColor(dwColor);

	m_pkLevel_->SetWindowColor(dwColor);
	m_pkLevel->SetWindowColor(dwColor);
	m_pkPlayer->SetWindowColor(dwColor);
	m_pkPlayer_->SetWindowColor(dwColor);
}

void KGCDungeonBossInfoS6::SetNumberTex( int index, KD3DWnd *pWnd )
{
	if(pWnd == NULL) return;

	int x_index = index % 10 ; 
    
    float tu, tv, tu1, tv1;

    if ( m_bRoom ) 
    {
	    tu = static_cast<float>(x_index) * 23.f + 3.f;
	    tv = 383.f;
	    tu1 = static_cast<float>(x_index) * 23.f + 23.f ;
	    tv1 = 403.f;
    }
    else
    {
        tu = static_cast<float>(x_index) * 24.f + 264.f;
        tv = 456.f;
        tu1 = static_cast<float>(x_index) * 24.f + 276.f ;
        tv1 = 468.f;
    }    
    
    D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCDungeonBossInfoS6::ChangeUVtoTexel( D3DXVECTOR4& vUV )
{
	float fMagicNumber=0.5f;
	if (vUV.x>vUV.z)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
	vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (vUV.y>vUV.w)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
	vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

	TEXEL_BOUND(vUV.x);
	TEXEL_BOUND(vUV.y);
	TEXEL_BOUND(vUV.z);
    TEXEL_BOUND(vUV.w);
}

void KGCDungeonBossInfoS6::SetRankImage()
{
    m_pkDungeonRank->InitState(true);

    int iRank = 0;
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        iRank = SiKGCWorldMapManager()->GetCurrentDungeonRank( true , true);
    else
        iRank = SiKGCWorldMapManager()->GetCurrentDungeonRank( false , true);


    if ( iRank < 0 ) {
        m_pkDungeonRank->ToggleRender(false);
        return;
    }
    if ( iRank > 7 )
        iRank = 0;

    int nX = iRank / 8;
    int nY = iRank % 8;

    D3DXVECTOR2 vTexSize = D3DXVECTOR2(86.f, 38.f);
    D3DXVECTOR2 vSecondTexStartAxis = D3DXVECTOR2(93.f, 69.f);
    D3DXVECTOR2 vBackImgTexStartAxis;
    D3DXVECTOR2 vBackImgTexEndtAxis;
    D3DXVECTOR2 vBaseTexStartAxis = D3DXVECTOR2(7.f, 11.f);

    vBackImgTexStartAxis.x = ( vBaseTexStartAxis.x + ((vSecondTexStartAxis.x - vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    vBackImgTexStartAxis.y = ( vBaseTexStartAxis.y + ((vSecondTexStartAxis.y - vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    vBackImgTexEndtAxis.x = ( vBaseTexStartAxis.x + vTexSize.x + ((vSecondTexStartAxis.x - vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    vBackImgTexEndtAxis.y = ( vBaseTexStartAxis.y + vTexSize.y + ((vSecondTexStartAxis.y - vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;

    m_pkDungeonRank->SetWndTexCoords( D3DXVECTOR4( vBackImgTexStartAxis.x, vBackImgTexStartAxis.y , vBackImgTexEndtAxis.x, vBackImgTexEndtAxis.y ) );
    m_pkDungeonRank->ToggleRender(true);

}