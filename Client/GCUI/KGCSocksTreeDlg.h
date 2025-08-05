#pragma once

//	UI���� �������� ���� ������ ��������
//	������ ���� �����͵�
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


    KD3DWnd*		m_pkBtnHangupSocks;			//	�縻 �ɱ� ��ư
    KD3DWnd*		m_pkToggleHangupSocks;		//	Ʈ���� �ɷ��ִ� �縻
    KD3DStatic*		m_pkStaticSocksGetInfo;		//	�� �縻�� �ð� ����
    KD3DStatic*		m_pkStaticSocksPercent;		//	�縻 ��� ���� �ۼ�Ʈ
    KD3DStatic*		m_pkStaticSocksNumberPos;	//	�縻 ���� ���� ��ġ ���Ϸ��� �����. �ƹ� �ؽ�Ʈ�� ���� �ʴ´�.
    KGCSizingThreeBox*	m_pkGuageSocks;			//	�縻 ��� �ۼ�Ʈ
    //	UI�� �ѹ� �ʱ�ȭ �Ǹ� �Ű澵 �ʿ䰡 ����.


    DWORD			m_duration;					//	�縻 ����
    DWORD			m_dwPeriodSec;				//	���� �� �ִ� �ֱ�
    DWORD			m_dwRemainSec;				//	������ ���� ���� �ȱ� ���� ���� �ð�
    DWORD			m_remainTick;				//	������ ���� ���� �ȱ� ���� ���� �ð��� �޾������� GetTickCount();
    GCITEMID		m_materialGoodsID;			//	�縻 ��� ���̵�
    DWORD			m_materialRequire;			//	�縻 ��� �䱸��
    bool			m_bIsHangup;				//	�縻�� �ɷ��ִ°�?
    DWORD			m_dwSocksPercent;			//	�縻��� �����ۼ� / �縻��� �䱸��
    DWORD			m_dwGuageWidth;				//	�縻 ������ UI �ȼ� ����
    bool			m_bIsCanCollect;			//	�縻�� ������ �ִ� �����̴�.

    std::string		m_strColor;					//	�縻 ���� UI �ʱ�ȭ �Ҷ� �ʿ��ϴ�.

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

private:	//	�� �Լ��� ���߿� ������ϱ� ���Ϸ��� public���� �ٲٸ�
    //	3�밡 ���ָ� �ް� �ȴ�.
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

    //	��ƼŬ
public:	
    void CreateParticle_YellowSocksDone();
    void SetPushHangUpBtn( bool bPush_ ) { m_bPushHangUpBtn = bPush_; }
    void SetPushDetachBtn( bool bPush_ ) { m_bPushDetachBtn = bPush_; }

private:
    void ClearParticle();
    void AddParticle( const std::string& strParticle_, float fx_, float fy_ );


public:		//	�������� ���� ������ �Լ���
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
    KD3DWnd*        m_pkTitle;                  //  Ÿ��Ʋ 
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
