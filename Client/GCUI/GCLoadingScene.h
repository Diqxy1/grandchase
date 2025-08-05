#ifndef _GCLOADINGSCENE_H_
#define _GCLOADINGSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

//====================================================================================
// 2006.11.19 : Asirion
// 매치에서 6, 인디고에서 2
#define MAX_LOADING_CHAR 8

class KGCCharLoadWnd;
class GCDeviceMotionFRM;
class KGCGaugeS6;

class KGCBillBoard
{
public:
    KGCBillBoard();

    virtual ~KGCBillBoard();

    GCDeviceTexture* m_pTexture;    

    KSafeArray<D3DXVECTOR2,2> m_vPos;
    KSafeArray<D3DXVECTOR2,2> m_vTexUV;

	void SetSize( D3DXVECTOR2 vSize	)
	{
		m_vPos[1] = m_vPos[0] + vSize;
	}
	void SetPos( D3DXVECTOR2 vPos )
	{
		D3DXVECTOR2 vSize = m_vPos[1] - m_vPos[0];

		m_vPos[0] = vPos;
		m_vPos[1] = vPos + vSize;
	}
	


    void Render();
    void Release();
};

class KGCLoadingScene : public KGCScene
{
public:
	DECLARE_CLASSNAME( KGCLoadingScene );
	/// Default constructor
	KGCLoadingScene( void );
	/// Default destructor
	virtual ~KGCLoadingScene( void );
	/// Copy constructor
	KGCLoadingScene( const KGCLoadingScene& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLoadingScene& operator=( const KGCLoadingScene& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void SetGameMode();
    void SetLoadingComplete( DWORD dwUID_ );
	void SetWaiting( bool bIsWaiting );

    void PostDraw( void );
    void PostChildDraw();
    
    void ReleaseTempResource();
protected:
    virtual void InitScene() {}
	virtual void OnCreate();
    void ClearUID();
    void OnIndigo();
    void OnMatch();
	void OnMiniGame();
    void OnAgit();
    void LoadAdTexture();
    void ReleaseAdTexture();
    void CreateMotion( int nChar, int nPromotion );


    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
protected:
	KD3DWnd* m_pMatchLoad;
	KD3DWnd* m_pWorldCupLoad;
	KD3DWnd* m_pIndigoLoad;
	KD3DWnd* m_pMinigameLoad;
	KD3DWnd* m_pNowLoading;
	KD3DWnd* m_pWaiting;
    KD3DWnd* m_pVS;
    KD3DWnd* m_pAgitLoad;
    KD3DWnd* m_pTip;
    
    KD3DStatic* m_pTipTitle;
    KD3DStatic* m_pTipContents;

    KSafeArray<KGCCharLoadWnd*,MAX_LOADING_CHAR> m_apkCharLoadingWnd;

    KSafeArray<D3DXMATRIX,MAX_LOADING_CHAR> m_aMatPetTransform;
    KSafeArray<int,MAX_LOADING_CHAR> m_aiPetFrame;

#if defined( LOADING_CHAR_MOTION )
    GCDeviceMotionFRM*              m_pMotionIntro[GC_CHAR_NUM][NUM_JOB_LEVEL];
#else
    GCDeviceMotionFRM*              m_pMotionIntro;
#endif

    std::vector<KGCBillBoard> m_vecBillBoard;

public:
    void SetLoadingState( std::pair<DWORD,int> prLoadingState_ );
	void OnAutoMatch();
};

DEFINE_WND_FACTORY( KGCLoadingScene );
DECLARE_WND_FACTORY( KGCLoadingScene );
DECLARE_WND_FACTORY_NAME( KGCLoadingScene );

class KGCAutoMatchLoadingBarS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCAutoMatchLoadingBarS6 );
	/// Default constructor
	KGCAutoMatchLoadingBarS6( void );
	/// Default destructor
	virtual ~KGCAutoMatchLoadingBarS6( void );
	/// Copy constructor
	KGCAutoMatchLoadingBarS6( const KGCAutoMatchLoadingBarS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAutoMatchLoadingBarS6& operator=( const KGCAutoMatchLoadingBarS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		AMLB_GAGE_START_LEFT = 0,
		AMLB_GAGE_START_RIGHT = 1,
	};

public:

	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetInfo( DWORD dwUserUID, std::wstring wstrUserNick, int iProgressBarType );
	bool IsUserUID( DWORD dwUserUID ) { return m_dwUserUID == dwUserUID;}
	void SetProgress( int iProgress );
private:
	int m_iGageType;
	KGCGaugeS6* m_pkGauge;
	KD3DStatic* m_pkUserNick;
	DWORD m_dwUserUID;
};

DEFINE_WND_FACTORY( KGCAutoMatchLoadingBarS6 );
DECLARE_WND_FACTORY( KGCAutoMatchLoadingBarS6 );
DECLARE_WND_FACTORY_NAME( KGCAutoMatchLoadingBarS6 );

class KGCAutoMatchLoadingS6 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCAutoMatchLoadingS6 );
	/// Default constructor
	KGCAutoMatchLoadingS6( void );
	/// Default destructor
	virtual ~KGCAutoMatchLoadingS6( void );
	/// Copy constructor
	KGCAutoMatchLoadingS6( const KGCAutoMatchLoadingS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAutoMatchLoadingS6& operator=( const KGCAutoMatchLoadingS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		AML_LOADINGBAR_MAX_CNT = 6,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetProgress( PAIR_DWORD_INT piUserInfo );
	void SetUserInfo( void );
	void InitUserInfo( void );
	void OnCloseLoading( const KActionEvent& event);
	void FrameMoveInEnabledState( );
private:
	KSafeArray<KGCAutoMatchLoadingBarS6*,AML_LOADINGBAR_MAX_CNT> m_saBar;
	time_t m_tmCloseCheck;
};

DEFINE_WND_FACTORY( KGCAutoMatchLoadingS6 );
DECLARE_WND_FACTORY( KGCAutoMatchLoadingS6 );
DECLARE_WND_FACTORY_NAME( KGCAutoMatchLoadingS6 );

#endif // _GCLOADINGSCENE_H_
