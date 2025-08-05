#pragma once

//	UI에서 서버에서 받은 정보를 바탕으로
//	가지고 있을 데이터들
struct KGCSocksTreeDlgProperty
{
    KGCSocksTreeDlgProperty()
        : m_pkBtnHangupSocks(0)
        , m_pkToggleHangupSocks(0)
        , m_pkStaticSocksGetInfo(0)
        , m_pkStaticSocksPercent(0)
        , m_pkStaticSocksNumberPos(0)
        , m_pkGuageSocks(0)
    {}

    void InitData()
    {
        m_duration			=	0;
        m_dwPeriodSec		=	0;
        m_dwRemainSec		=	0;
        m_remainTick		=	0;
        m_materialGoodsID	=	0;
        m_materialRequire	=	0;
        m_bIsHangup			=	false;
        m_dwSocksPercent	=	0;
        m_dwGuageWidth		=	0;
        m_bIsCanCollect		=	false;
    }

    void InitData(	const std::string&	strColor_
        )
    {
        m_strColor				=	strColor_;
        this->InitData();
    }

    void AddParticleOne( const std::string& rName_, float x_, float y_ );
    void AddCollectParticle();
    void AddCanCollectParticle();
    void DeleteParticle();


    KD3DWnd*		m_pkBtnHangupSocks;			//	양말 걸기 버튼
    KD3DWnd*		m_pkToggleHangupSocks;		//	트리에 걸려있는 양말
    KD3DStatic*		m_pkStaticSocksGetInfo;		//	각 양말들 시간 정보
    KD3DStatic*		m_pkStaticSocksPercent;		//	양말 재료 남은 퍼센트
    KD3DStatic*		m_pkStaticSocksNumberPos;	//	양말 숫자 찍을 위치 정하려고 만든것. 아무 텍스트도 넣지 않는다.
    KGCSizingThreeBox*	m_pkGuageSocks;			//	양말 재료 퍼센트
    //	UI는 한번 초기화 되면 신경쓸 필요가 없다.


    DWORD			m_duration;					//	양말 숫자
    DWORD			m_dwPeriodSec;				//	걷을 수 있는 주기
    DWORD			m_dwRemainSec;				//	서버로 부터 받은 걷기 위해 남은 시간
    DWORD			m_remainTick;				//	서버로 부터 받은 걷기 위해 남은 시간을 받았을때의 GetTickCount();
    GCITEMID		m_materialGoodsID;			//	양말 재료 아이디
    DWORD			m_materialRequire;			//	양말 재료 요구량
    bool			m_bIsHangup;				//	양말이 걸려있는가?
    DWORD			m_dwSocksPercent;			//	양말재료 아이템수 / 양말재료 요구량
    DWORD			m_dwGuageWidth;				//	양말 게이지 UI 픽셀 길이
    bool			m_bIsCanCollect;			//	양말을 걷을수 있는 상태이다.

    std::string		m_strColor;					//	양말 색상 UI 초기화 할때 필요하다.

    std::vector<CParticleEventSeqPTR>	m_vecParticle;
};


class KGCSocksTreeDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSocksTreeDlg );
    /// Default constructor
    KGCSocksTreeDlg( void );
    /// Default destructor
    virtual ~KGCSocksTreeDlg( void );
    /// Copy constructor
    KGCSocksTreeDlg( const KGCSocksTreeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSocksTreeDlg& operator=( const KGCSocksTreeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:	//	override
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    virtual void FrameMoveInDisabledState();
    virtual void PostChildDraw();

private:	//	이 함수들 나중에 디버깅하기 편하려고 public으로 바꾸면
    //	3대가 저주를 받게 된다.
    void CheckParticle();

    void SendSocksHangupReqOne( const GCITEMID socksGoodsID_ );
    void SendSocksHangupReqVector( const std::vector< GCITEMID >& vecSocksGoodsID_ );

    void LinkControlSocks();
    void InitStateSocks();

    void UpdateSocksUI();

    void OnUIClickEvent( const KActionEvent& event );
    void OnUIEnterEvent( const KActionEvent& event );

    void SyncData( const KUserSocksInfo& dataFromServer_, bool timeDataSkip =false );

    void CalcRemainSec( KGCSocksTreeDlgProperty& socksProperty_ );
    void CheckGetSome();
    bool CheckEnoughSocksMaterial();

    //	파티클
public:	
    void CreateParticle_YellowSocksDone();
    void SetPushHangUpBtn( bool bPush_ ) { m_bPushHangUpBtn = bPush_; }
    void SetPushDetachBtn( bool bPush_ ) { m_bPushDetachBtn = bPush_; }

private:
    void ClearParticle();
    void AddParticle( const std::string& strParticle_, float fx_, float fy_ );


public:		//	서버에서 값을 얻어오는 함수들
    void OnEventSocksInfoNot( const KUserSocksInfo& kUserSocksInfo_ );
    void OnEventSocksHangUpAck( KSocksResult& kSocksResult_ );
    void OnEventSocksCollectAck( KSocksResult& kSocksResult_ );
    void OnEventSocksMaterialExchangeACK( KSocksExchangeData& kRecv_ );
    void OnEventSocksHangupCompleteNot( const KSocksResult& kSocksResult_ );
    void OnEventSocksScriptInfoNot( const KUserSocksInfo& kUserSocksInfo_ );    

private:
    KD3DWnd*		m_pkBtnClose;
    KD3DWnd*		m_pkBtnHangupSocksAll;
    KD3DWnd*		m_pkBtnGetSocksAll;
    KD3DWnd*        m_pkTitle;                  //  타이틀 
    KD3DStatic*		m_pkStaticInfoMain;

    KD3DStatic*		m_pkStaticNameRed;
    KD3DStatic*		m_pkStaticNameBlue;
    KD3DStatic*		m_pkStaticNameYellow;

    std::map< GCITEMID, KGCSocksTreeDlgProperty >				m_mapSocksTreeDlgProperty;
    KUserSocksInfo												m_socksInfoFromServer;

    bool            m_bPushHangUpBtn;
    bool            m_bPushDetachBtn;

    std::vector<CParticleEventSeqPTR>	m_vecParticle;
};

DEFINE_WND_FACTORY( KGCSocksTreeDlg );
DECLARE_WND_FACTORY( KGCSocksTreeDlg );
DECLARE_WND_FACTORY_NAME( KGCSocksTreeDlg );
