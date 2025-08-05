#include "stdafx.h"
#include "GCGawibawiboGameBossInfo.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboGameBossInfo );
IMPLEMENT_WND_FACTORY( KGCGawibawiboGameBossInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboGameBossInfo, "gc_gawibawibo_game_boss_info" );

#define MAX_BOSS_POS	(622.0f )
#define FINAL_BOSS		10

KGCGawibawiboGameBossInfo::KGCGawibawiboGameBossInfo( void )
:	m_iMaxBoss(0)
,   m_pTex(NULL)
,	m_fBossPointBetween(0.0f)
,	m_pkBossPoint(NULL)
{
	KD3DWnd	*pWnd = NULL;

	char	cBuf[10];
	memset(cBuf,'\0',10);

	std::string uiname;	

	for(int i = 1;i<10;i++)
	{
		m_mapBossBtn[i] = NULL;
		m_mapBossBk[i] = NULL;
		m_mapBossClear[i] = NULL;

		itoa(i,cBuf,10);
		
		uiname = "bk_boss";
		uiname += cBuf;
		LINK_CONTROL(uiname.c_str(), m_mapBossBk[i]);

		uiname = "clear_boss";
		uiname += cBuf;
		LINK_CONTROL(uiname.c_str(),  m_mapBossClear[i]);

		uiname = "btn_boss";
		uiname += cBuf;
		LINK_CONTROL(uiname.c_str(), m_mapBossBtn[i]);

	}
	m_mapBossBtn[FINAL_BOSS] = NULL;

	
	LINK_CONTROL( "btn_bossfinal",   m_mapBossBtn[FINAL_BOSS] );	
	LINK_CONTROL( "bk_boss_point",   m_pkBossPoint );	
}

KGCGawibawiboGameBossInfo::~KGCGawibawiboGameBossInfo( void )
{
}

void KGCGawibawiboGameBossInfo::ActionPerformed( const KActionEvent& event )
{

}

void KGCGawibawiboGameBossInfo::OnCreate( void )
{
		
}

void KGCGawibawiboGameBossInfo::Destroy()
{
    SAFE_RELEASE(m_pTex);
}

void KGCGawibawiboGameBossInfo::SetPosBetween(int iMaxBoss)
{
	m_iMaxBoss = iMaxBoss;
	m_fBossPointBetween = MAX_BOSS_POS/m_iMaxBoss;
}

void KGCGawibawiboGameBossInfo::SetPosBossPoint(int iNowBoss)
{
	D3DXVECTOR2 vpos;
	vpos.x = iNowBoss * m_fBossPointBetween +13;
	vpos.y = 324;
	m_pkBossPoint->SetWindowPos(vpos);
}

int KGCGawibawiboGameBossInfo::IsFinalBoss(int index_)//파이널 보스라면 파이널 보스 인덱스를 넘겨주고 아니라면 자신의 인덱스를 리턴한다.
{
	return index_ == m_iMaxBoss ? FINAL_BOSS : index_ ;
}

void KGCGawibawiboGameBossInfo::SetInitBossInfo(int iNowBoss, bool bLockBossInfo)
{

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
	//float setoffsetx = 0.0f;
	//float setoffsety = 0.0f;
	//LUA_GET_VALUE_DEF( "setoffsetx", setoffsetx, 0.0f );    
	//LUA_GET_VALUE_DEF( "setoffsety", setoffsety, 0.0f );    


	D3DXVECTOR2 vpos,vposbtn;
	vpos.x = iNowBoss * m_fBossPointBetween;
	vpos.y = 346.0f;

	if(m_iMaxBoss < iNowBoss)
	{
		m_mapBossBtn[iNowBoss]->ToggleRender(false);
		m_mapBossBk[iNowBoss]->ToggleRender(false);
		m_mapBossClear[iNowBoss]->ToggleRender(false);
		return;
	}
	else if(m_iMaxBoss == iNowBoss)
	{
		m_mapBossBtn[iNowBoss]->ToggleRender(false);
	}

	// 아래 순서 유의 하십시오
	//********************************************
	vpos.x = m_fBossPointBetween * iNowBoss;
	float fHeight = m_mapBossBtn[IsFinalBoss(iNowBoss)]->GetHeight()/2.0f;
	float fWidth = m_mapBossBtn[IsFinalBoss(iNowBoss)]->GetWidth()/2.0f;
	vposbtn.x = vpos.x-fWidth +30.0f;
	vposbtn.y = vpos.y-fHeight +25.0f;

	m_mapBossBtn[IsFinalBoss(iNowBoss)]->SetWindowPos(vposbtn);
	
	m_mapBossBk[iNowBoss]->SetWindowPos(vpos);
	vpos.x += -6.0f;
	vpos.y += -5.0f;
	m_mapBossClear[iNowBoss]->SetWindowPos(vpos);
	//*********************************************
}


void KGCGawibawiboGameBossInfo::SetPosBossInfo(int iNowBoss, bool bLockBossInfo)
{
    if (m_mapBossBtn[IsFinalBoss(iNowBoss)])
	    m_mapBossBtn[IsFinalBoss(iNowBoss)]->ToggleRender(true);
    if (m_mapBossBk[iNowBoss])
	    m_mapBossBk[iNowBoss]->ToggleRender((iNowBoss != m_iMaxBoss)); //최종 보스는 뒷배경 안쓴다.
    if (m_mapBossClear[iNowBoss])
	    m_mapBossClear[iNowBoss]->ToggleRender(bLockBossInfo); // 현재 활성화 된 놈 빼고는 엑스표 쳐준다.
    if (m_mapBossBtn[iNowBoss])
	    m_mapBossBtn[iNowBoss]->Lock(bLockBossInfo); //현재 활성화 된 보스만 쓴다.
}



void KGCGawibawiboGameBossInfo::SetBossBigImage( int iNowBoss)
{

    SAFE_RELEASE(m_pTex);

	char	cBuf[256];
	memset(cBuf,'\0',256);
	if(iNowBoss != m_iMaxBoss)
		sprintf(cBuf,"rockpaperscissors_B%d.dds",iNowBoss);
	else
		sprintf(cBuf,"rockpaperscissors_Bfinal.dds");

    m_pTex = g_pGCDeviceManager2->CreateTexture(cBuf);	
}


void KGCGawibawiboGameBossInfo::SetInfo( int iNowBoss,int iMaxBoss)
{
	iNowBoss++;
	if(iMaxBoss != m_iMaxBoss){
		if(iMaxBoss > 0)
			SetPosBetween(iMaxBoss);
		if(iNowBoss > 0){	
			for(int i = 1;i <10 ;i++)
			{
				SetInitBossInfo(i,(i < iNowBoss));				
			}
		}
	}
	if(iNowBoss > 0){	
		for(int i = 1;i <= m_iMaxBoss ;i++)
		{
			SetPosBossInfo(i,(i < iNowBoss));				
		}
		SetPosBossPoint(iNowBoss);
		SetBossBigImage(iNowBoss);
	}
}

void KGCGawibawiboGameBossInfo::PreDraw( void )
{
    if ( m_pTex )
    {
        D3DXVECTOR2 vPos = GetRelocatedWindowPos();
        vPos.x += GetWidth() * m_fWindowScaleX;
        vPos.y += GetHeight() * m_fWindowScaleY;
        vPos.x -= m_pTex->GetWidth() * m_fWindowScaleX;
        vPos.y -= m_pTex->GetHeight() * m_fWindowScaleY;

        float fNumOffsetX = /*51.f * */g_pGCDeviceManager2->GetWindowScaleX();
        float fNumOffsetY = /*-100.f * */g_pGCDeviceManager2->GetWindowScaleY();
        float fNumOffsetY2 = /*-80.f * */g_pGCDeviceManager2->GetWindowScaleY();

        g_pGCDeviceManager2->DrawInScreen( m_pTex, vPos.x+fNumOffsetX, vPos.y+fNumOffsetY, 
            fNumOffsetX+ vPos.x + m_pTex->GetWidth() * m_fWindowScaleX , vPos.y +fNumOffsetY2+ m_pTex->GetHeight() * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );
    }
}