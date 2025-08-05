#pragma once

class KGCAttributeSelectBox : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAttributeSelectBox );
    /// Default constructor
    KGCAttributeSelectBox( void );
    /// Default destructor
    virtual ~KGCAttributeSelectBox( void );
    /// Copy constructor
    KGCAttributeSelectBox( const KGCAttributeSelectBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAttributeSelectBox& operator=( const KGCAttributeSelectBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
    {
		ATTRIBUTE_RANDOM_MAX = 3,
		ATTRIBUTE_SELECT_MAX = 12,
	};

	enum 
    {
		STATE_SELECT      = 0,    // 속성 선택 가능한 상황
		STATE_RANDOM      = 1,    // 속성 랜덤 돌릴때
        STATE_MIGRATION   = 2,    // 속성 마이그레이션 상황 ( 랜덤으로 돌린다. )
        STATE_SINGLE_RANDOM = 3,  // 속성 한개씩 랜덤 돌릴때 
	};

    enum ITEM_CASH_TYPE 
    { 
        USE_CASH = 0,
        USE_GP = 1,
    };

    enum 
    {
        UI_HEIGHT_OFFSET     = 41,
        UI_MAIN_FRAME_HEIGHT = 481, 
        UI_SUB_FRAME_HEIGHT = 338,
        UI_DESC_Y_POS = 372,
        UI_MIG_CONFIRM_Y_POS = 442,
    };

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    // 속성 리스트를 셋팅한다.
    void SetAttributeList(std::vector<KAttributeInfo> &attributelist ,const std::set<DWORD> &randomlist, const bool bTypeCheck = false );

public:

    // 속성 랜덤 UI를 띄울때 해당 UI 출력한다. ( 마이그레이션 상황에서도 호출된다. )
    void SetAttributeInfoRandom( GCITEMUID dwItemUID, const bool bUseSingle, const bool bUseMigration );

    // 속성을 선택할 수 있는 상황에서 호출 ( Cash아이템 최초 선택이거나 초기화 하였을때 )
    void SetAttributeInfoSelect(KEVENT_ATTRIBUTE_LIST_ACK arg_);
    


    // 속성 테이블 번호를 셋팅한다. ( 서버 패킷 처리용 )
    void SetAttributeTableID ( GCITEMUID dwItemUID, int iTableID_ );


    // 속성 랜덤으로 돌릴때 해당 함수 호출 된다.
    void SetAttributeInfoLevel( GCITEMUID dwItemUID, int iTableID_ );    
    
    // 속성 마이그레이션시( 성장 개편 ) 속성 테이블 번호를 셋팅한다.
    void SetAttributeMigrationInfo( const int iTableID_ = -1 );    
    bool SetAttributeMigrationList( const int iTableID_ = -1 );

    // 속성 개별 랜덤 주문서 사용 상태일때 호출
    void SetAttributeInfoSingle( const int iTableID_ = -1 );
    bool SetAttributeListSingle( const int iTableID_ = -1 );

public:
    void SetResultSingle( int nSlotID );    // 개별 랜덤 주문서 결과 항목을 출력한다.

    bool GetSelectSlot( int nSlotID, int& nSelect, bool& bRandom ); // SlotID를 넘기면 선택될 ID를 넘겨받는다.

    // 체크 박스를 초기화한다.
    void InitAttributeList();
    // UI를 셋팅한다. ( bSet True 설정하면 속성 마이그레이션 버전이다. )
    void SetAttributeMigrationUI();
    // UI를 조절 한다. ( 마이그레이션에서는 사용 안함 )
    void SetAttributeBtn();


    // 속성 테이블 정보를 서버에서 사용하는 변수로 변경한다.
    void ConvertList(KGCAttributeTable &kItemAttributeTable,std::vector<KAttributeInfo> &attributelist ); 
    // 속성 글자의 색상을 가져온다.
    DWORD GetAttributeStaticColor( char cType, float fValue, const bool bUseMigration_ = false);    

    void SetItemUID(GCITEMUID dwItemUID){m_dwItemUID = dwItemUID;}
    bool SetItemInfo( GCITEMUID dwItemUID );


    int GetRandomItemCount();
    int GetGPRandomItemCount();
    int GetInitRandomItemCount();
    int GetSingleRandomItemCount();
    

public:
    // 속성 마이그레이션을 한다. ( m_pkBtnAttributeMigrationConfirm )
    void OnClickAttributeMigration( void );    

    // 초기화 버튼 클릭시 적용 ( m_pkBtnInitAttribute )
    void OnInitAttributeBtn( void );
    
    // 속성을 랜덤으로 돌린다. ( m_pkRandComplete, m_pkRand )
    void OnBtnRandom();     
    
    // 체크박스 클릭시 처리 ( 체크박스 클릭시 처리 )
    void OnAttributeCheckBoxClick( const KActionEvent& event );
    
    // 확인 버튼 클릭시 호출( 속성 선택시 사용된다.  m_pkSelectComplete )
    void OnBtnSelect(); 

    // 닫기 버튼 ( m_pkRandClose, m_pkClose )
    void OnClose(); 


public:

    KD3DWnd *m_pkClose;             // 닫기 버튼
    KD3DWnd *m_pkRand;              // 캐쉬 아이템 랜덤 버튼( 속성 선택 가능할때 사용하는 버튼 )
    KD3DWnd *m_pkRandClose;         // 속성 랜덤시 확인 버튼
    KD3DWnd *m_pkRandComplete;      // 속성 랜덤( 랜덤 선택 )
    KD3DWnd *m_pkSelectComplete;    // 속성 선택시 속성 고정 하는 버튼
    KD3DWnd *m_pkBtnInitAttribute;  // 초기화 버튼
    KD3DWnd *m_pkBtnAttributeMigrationConfirm;  // 속성 마이그레이션 버튼

	KSafeArray<KD3DCheckBox*, ATTRIBUTE_SELECT_MAX> m_arrSelectCheckBox; 
	KSafeArray<KD3DCheckBox*, ATTRIBUTE_RANDOM_MAX> m_arrRandomCheckBox; 

	KD3DStatic* m_pkTitleStatic;
	KD3DStatic* m_pkDescStatic;
    KD3DStatic* m_pkNomalStatic;
    KD3DStatic* m_pkSpecialStatic;
    KD3DStatic* m_pkItemStatic;
    KD3DStatic* m_pkItemNumStatic;
    KD3DStatic* m_pkInitItemNumStatic;

	std::vector<std::wstring> m_vecTitleString;
	std::vector<std::wstring> m_vecDescString;    


	int m_iSelectState;

	GCITEMID  m_dwItemID;   // 사용될 아이템 - ex)랜덤속성사용권
    GCITEMID  m_InitItemID; // 초기화에 사용될 아이템 
	GCITEMUID m_dwItemUID;  // 사용할 아이템 - ex)엘리시스 투구.

	KItem *m_pkItemInfo;

	std::map<int,KAttributeInfo> m_mapSelectInfo;

	int m_iMaxSelectCnt;

    int m_iPrice; // 주문서 사용갯수

    bool m_bFirstSelect; //파티클을 띄우기 위해 사용함. true이면 파티클 생성 없음.
    bool m_bFirstSendPacket; //패킷을 여러번 보내지 않기 위해

    int m_nTableID;    
 
private:
    ITEM_CASH_TYPE  m_ItemCashType;
    std::vector< KAttributeInfo > m_vecInitAttribute;    // 아이템이 처음 가지고 있는 속성 

private:
    std::set< int >                 m_SetEnableMigration;
    std::vector< KAttributeInfo >   m_vecMigrationItemAttribute;

private:
    KD3DSizingBox*  m_pkMainBack;
    KD3DSizingBox*  m_pkSubBack;
    KD3DSizingBox*  m_pkUseItemlBack;
    KD3DSizingBox*  m_pkUseItemTitleBack;
    


};

DEFINE_WND_FACTORY( KGCAttributeSelectBox );
DECLARE_WND_FACTORY( KGCAttributeSelectBox );
DECLARE_WND_FACTORY_NAME( KGCAttributeSelectBox );
