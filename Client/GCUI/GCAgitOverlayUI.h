#pragma once

class KGCAgitInfoBar;
class KGCChatBox;
class KGCChatBoxBackground;
class KGCChatScrollBackground;
class KGCAgitPrePlace;
class KGCAgitOption;
class KGCAgitObjectPlaceCollectUI;
class KGCAgitInviteVisit;
class KGCAgitList;
class KGCSeedPlant;
class KGCHarvestGauge;
class KGCSayBalloonSquare;
class KGCAgitBrownie;
class KGCAgitMove;
class KGCAgitMoneyInfo;
class KGCInviteAgitReqDlg;
class KGCInviteAgitSellBox;
class KGCAgitEnterPass;
class KGCAgitGuestbook;
class KGCAgitTrainingUI;
class KGCAgitCharSetting;
class KGCFairyTreeLevelGauge;

class KGCAgitOverlayUI : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitOverlayUIUI );
    /// Default constructor
    KGCAgitOverlayUI( void );
    /// Default destructor
    virtual ~KGCAgitOverlayUI( void );
    /// Copy constructor
    KGCAgitOverlayUI( const KGCAgitOverlayUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitOverlayUI& operator=( const KGCAgitOverlayUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum ECursorFocus
	{
		ECF_MAINWND,
		ECF_CHATBOX,
		ECF_UNKNOWN,
	};

	enum EM_CHAT_TYPE
	{
		EM_CHAT_NONE,
		EM_CHAT_RETURN,
		EM_CHAT_ESC,
	};


    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void Init( DWORD dwAgitUID );
    void ToggleMainUI( bool bShow_ );
    void CloseAllUI( void );

    void ShowPrePlaceUI( bool bShow_, GCItem* pItem_ = NULL, int iMoneyType_ = 0, int iPrice_ = 0, PAIR_USHORT_DWORD pairIndex_ = std::pair<USHORT, DWORD>(0, 0) );
    void ShowAgitOption( bool bShow_, int iType_ = 0 );
    void ShowAgitPlaceCollectUI( bool bShow_ );
    void ShowAgitInviteVisitUI( bool bShow_ );
    
	void ShowAgitList( bool bShow_ );
	void ShowAgitEnterPass ( DWORD dwAgitID );
	void SetMaxChannel( int nMaxChannel_ );
	void ReqUpdateFavorityPage();

    void ShowInviteAgitReqDlg( bool bShow_ );
    void ShowAgitSellBox( bool bShow_, std::pair<GCITEMID, GCITEMUID> prItemInfo_ = std::pair<GCITEMID, GCITEMUID>(0,0) );
	
	void SetRegisterAgitList( KAgitPageList& PageInfo_, std::vector< KAgitPageInfo >& vecPageList_ );
	void SetFriendAgitList( std::vector< KAgitPageInfo >& vecPageList_ );
	void SetFavoriteAgitList( std::vector< KAgitPageInfo >& vecPageList_ );
	void SetRankAgitList( std::vector< KAgitPageInfo >& vecPageList_ );
	
	void ShowSeedPlant( bool bshow_ );
    void ShowHarvestGauge( bool bShow_, float fPosX_ = -1.0f, float fPosY_ = -1.0f );
    void ShowBrownie( bool bShow_ );
    void ShowAgitMove( bool bshow_ );
    void ShowGuestBook( bool bShow_ );
    void ShowTrainingUI( bool bshow_, GCITEMUID itemUID = 0 );
    void ShowCharSettingUI( bool bShow_, int iCharType_ = GC_CHAR_ELESIS );
    
    void UpdateAgitInfo( void );
    void UpdatePlaceCollectUI( void );
    void UpdatePrePlaceUI( void );
    void UpdateMoneyInfo( void );

	void UpdatShopPage( void );

    void OnExitAgit( void );
    void CloseInviteVisitUI( void );
    bool MouseInUI( D3DXVECTOR2& dxvPos_ );
    bool IsUseKeyCheck( void );
    bool HarvestStop( void );

    void OnBtnBrownie( void );
    void OnCloseBrownie( void );
//	채팅창 관련
	ECursorFocus GetCursorFocus( void ) const;
	void TimeCheck(void ) const;
	void ClearFocus( void );
	void ClearChatEvent( void );
    void ClearChatList();
	void SetEditFocus( void );
	void ReLocate();
	void EndMessageBox();
	void ModalExitMsgBox( void );

	EM_CHAT_TYPE GetChatEventHandling( void ) const { return m_emHandleChatEvent; }
	void SetChatEventHandling( EM_CHAT_TYPE emHandleChatEvent ) { m_emHandleChatEvent = emHandleChatEvent; }

//	말풍선
	void ReLocateBalloon( D3DXVECTOR2 vPos , DWORD dwUID, int BalloonDistance = 50);
	void SetBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor );
	void DeleteBalloonMsg( DWORD dwCharUID);
	void DeleteAllSayBalloon();
	

	KGCSayBalloonSquare* GetSayBalloon() const { return m_pkSayBalloon; }

    void SetMenuButton( DWORD dwAgitUID );
    void OnClickPlaceBtn( void );
    void OnClickShopBtn( void );
    void OnClickInviteBtn( void );
    void OnClickAddFavorite( void );
    void OnClickMsnButton( void );
    void OnClickParty( void );
    void OnClickReturnButton( void );
    void ToggleRenderBrownieBtn( bool bRender_ );

    bool IsRenderOnAgitUI( void );
    bool IsRenderOnHarvestUI( void );
    bool IsRenderOnMoveUI( void ); 
    bool IsRenderOnOptionUI( void ); 

    void SetGuestBookInfo( std::map<int, KGuestMsg>& info_ );
    void AddNewGuestbookMsg( int iMsgNum_, KGuestMsg Msg_ );
    void DeleteGuestBookMsg( std::set<int>& steDeleteID );

    GCITEMUID GetTraingObjUID( void );

    // 요정의 나무 성장 게이지 
    void SetFariyTreeLevelGauge( bool bShow_, int iLevel_ = 0, DWORD dwCurExp_ = 0, DWORD dwNextLevelExp_ = 0, D3DXVECTOR2& dxvPos_ = D3DXVECTOR2( 0.0f, 0.0f ), D3DXVECTOR2& dxvOffset_ = D3DXVECTOR2( 0.0f, 0.0f ) );
    void UpdateFairyTreeLevelGaugePos( void );
    void RefreshCharacterTab( bool bIsTrue );
    KGCAgitCharSetting* GetCharSetting()    { return m_pkAgitCharSetting; }
private:
    // 채팅창 관련 
    KGCChatBox*             m_pkChatBox;   
    KGCChatBoxBackground*   m_pkChatBoxBackGround;
    KGCChatScrollBackground*m_pkChatScrollBackground;
    KD3DSizingBox*          m_pkInputBack;
    KD3DWnd*                m_pkChangeViewBtn;

	EM_CHAT_TYPE			m_emHandleChatEvent;


    bool                m_bDragWindowCheck;
    DWORD               m_dwBeforeScrollHeight;
    DWORD               m_dwBeforeListHeight;
    DWORD               m_dwBeforScrollBackground;
    DWORD               m_dwBeforChatBoxBackground;
    D3DXVECTOR2         m_vBeforePos;


    KGCSayBalloonSquare*    m_pkSayBalloon;
	KD3DWnd*				m_pkBalloonTalkBtn1;
	KD3DWnd*				m_pkBalloonTalkBtn2;

    // 각 UI들     
    KGCAgitInfoBar*              m_pkInfoBar;
    KGCAgitPrePlace*             m_pkPrePlaceUI;
    KGCAgitOption*               m_pkAgitOption;
    KGCAgitObjectPlaceCollectUI* m_pkPlaceCollectUI;
    KGCAgitInviteVisit*          m_pkInviteVisitUI;
    KGCAgitList*                 m_pkAgitList;
	KGCAgitEnterPass*			 m_pkAgitEnterPass;
    KGCSeedPlant*                m_pkSeedPlantUI;
    KGCHarvestGauge*             m_pkHarvestGauge;
    KGCAgitBrownie*              m_pkAgitBrownieUI;
    KGCAgitMove*                 m_pkAgitMoveUI;
    KGCAgitMoneyInfo*            m_pkAgitMoneyInfo;
    KGCInviteAgitReqDlg*         m_pkInviteAgitReqDlg;
    KGCInviteAgitSellBox*        m_pkInviteAgitSellBox;
    KGCAgitGuestbook*            m_pkGuestBook;
    KGCAgitTrainingUI*           m_pkTrainingUI;
    KGCAgitCharSetting*          m_pkAgitCharSetting;
    KGCFairyTreeLevelGauge*      m_pkFairyTreeLevelGauge;

    KD3DWnd*                     m_pkBtnBrownie;

    KD3DWnd*	            m_pkPlaceButton;
    KD3DWnd*	            m_pkShopButton;
    KD3DWnd*	            m_pkInviteButton;
    KD3DWnd*	            m_pkExitButton;
    KD3DWnd*                m_pkAddFavoriteButton;
    KD3DWnd*                m_pkMsnButton;
    KD3DWnd*                m_pkPartyButton;
    KD3DWnd*                m_pkReturnButton;

private:
	KGCBillBoardChatBoard* m_pkWorldBillboardChatBoard;
};

DEFINE_WND_FACTORY( KGCAgitOverlayUI );
DECLARE_WND_FACTORY( KGCAgitOverlayUI );
DECLARE_WND_FACTORY_NAME( KGCAgitOverlayUI );

extern KGCAgitOverlayUI* g_pkAgitOverlayUI;
