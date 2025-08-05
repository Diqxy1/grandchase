#pragma once
#include "GCAgitObjectList.h"
#include "GCAgitObjectContainer.h"

class KGC2DObject;
class KGCObjectAndMotion;

struct KAgitObjectPos;
struct KAgitCharacterPos;
struct LTP_AGIT_DICE_OBJECT_STATE;
struct LTP_AGIT_FLAG_OBJECT_STATE;

inline void WorldToScreen( float wx, float wy, int& sx, int& sy ) 
{
	sx = (int)( ( wx + 1.0f ) * GC_SCREEN_DIV_WIDTH_HALF + 0.5f );
  sy = (int)( ( -wy + 0.75f ) * GC_SCREEN_DIV_WIDTH_HALF + 0.5f );
}

struct KFlowerPotInfo
{
    KFlowerPot      m_FlowerPot;
    D3DXVECTOR2     m_dxvPos;
    KGC2DObject*    m_p2DObject;

    bool            m_bPalnt;       // 씨앗 심은 아이인가??
    bool            m_bHarvest;     // 수확 가능한 아이인가??

    KFlowerPotInfo( void )
    {
        m_dxvPos.x = 0.0f;
        m_dxvPos.y = 0.0f;
        m_p2DObject = NULL;

        m_bPalnt = false;
        m_bHarvest = false;
    }

    ~KFlowerPotInfo( void )
    {
        m_p2DObject = NULL;
    }
};

struct kMapInfo
{
    std::string m_strMapFile;
    D3DXVECTOR2 m_dxvFairyTreePos;
    int         m_iMapNameString;
};

struct KFruitInfo
{
    std::string m_strTex;
    D3DXVECTOR2 m_dxvPos;
};

struct KFairyTreeInfo
{
    int                                 m_iLevel;
    std::string                         m_strTree;
    std::vector< KAgitFootHold >        m_vecFootHold;
    std::vector< KFruitInfo >           m_vecFruits;
    std::vector< KAgitObjectParticle >  m_vecParticle;
    D3DXVECTOR2                         m_dxvGaugePos;

    KFairyTreeInfo( void )
    {
        m_iLevel = 0 ;
        m_strTree.clear();
        m_vecFootHold.clear();
        m_vecFruits.clear();
        m_vecParticle.clear();
        m_dxvGaugePos = D3DXVECTOR2( 0.0f, 0.0f );
    }
};

struct KFairyTreeDisplay
{
    KGC2DObject*                m_p2DObject;    // 나무 
    std::vector< KGC2DObject* > m_vecFruits;    // 과일 
    KFairyTree                  m_TreeInfo;     // 나무 정보 
    KFairyTreeFruitBuff         m_kBuffInfo;    // 버프 정보 
    std::vector< CParticleEventSeqPTR > m_vecParticle; // 파티클 

    KFairyTreeDisplay( void )
    {
        m_p2DObject = NULL;
        m_vecFruits.clear();
    }

    ~KFairyTreeDisplay( void )
    {
        m_p2DObject->RemoveAllTexture();
        SAFE_DELETE( m_p2DObject );

        for( std::vector< KGC2DObject* >::iterator it = m_vecFruits.begin() ; it != m_vecFruits.end() ; ++it )
        {
            (*it)->RemoveAllTexture();
            SAFE_DELETE( (*it) );
        }
        m_vecFruits.clear();

        std::vector< CParticleEventSeqPTR >::iterator vit;
        for( vit = m_vecParticle.begin() ; vit != m_vecParticle.end() ; ++vit )
        {
            g_ParticleManager->DeleteSequence( (*vit) );
            (*vit) = NULL;
        }
        m_vecParticle.clear();

    }
};

struct KAgitCharacterMotion 
{
    int m_iIndex;
    int m_iStringID;
    std::vector< std::pair< std::string, std::string > > m_vecManCharMotion;    // < State 이름, Motion 이름 >
    std::vector< std::pair< std::string, std::string > > m_vecGirlCharMotion;

    KAgitCharacterMotion( void )
    {
        m_iIndex = -1;
        m_iStringID = -1;
    }
};

class KGCAgitGameManager
{	
    DeclareSingleton( KGCAgitGameManager );

public:
    KGCAgitGameManager( void );
    virtual ~KGCAgitGameManager( void );

	void UpdateCountPos();
    void SetAgitInfo( KAgitInfo& info_ );
    void SetAgitUserInfo( std::map<DWORD,KAgitUserInfo>& mapAgitUser );
    void SetURIpPort( PAIR_DWORD_USHORT prURServerIpPort ) { m_prURServerIpPort = prURServerIpPort; }
    PAIR_DWORD_USHORT GetURIpPort( void ) { return m_prURServerIpPort; }
    void SetTRIpPort( PAIR_DWORD_USHORT prTRServerIpPort ) { m_prTRServerIpPort = prTRServerIpPort; }
    PAIR_DWORD_USHORT GetTRIpPort( void ) { return m_prTRServerIpPort; }
    KAgitInfo GetAgitInfo( void ) { return m_kAgitInfo; }
    void SetAgitOption( KAgitOption& Info_ );

    void EnterMyAgit( void );
    void EnterAgit( DWORD dwUID, std::wstring strPassword_=L"" );
    void SetMyAgitOption( std::wstring strName_, UCHAR ucMaxUser_, UCHAR ucOpenType_, std::wstring strPassword_, UCHAR ucInviteType_, std::wstring strProfile_ );
    void InitAgitObjectPos( void );
    void UpdateAgitObjectPos( const std::vector<KAgitObjectPos>& vecPos_ );
    void InitBeforeEnterAgit( void );
    void SetP2PConnectAtEnter( void );
    void OnInsertUser( DWORD dwAgitUID, KAgitUserInfo& userInfo );
    void OnDestroyUser( DWORD dwAgitUID, DWORD dwUserUID );
    void LoadUserResource( DWORD dwUserUID );
    void ReleaseUserResource( DWORD dwUserUID );

    void UpdateAgitHost( int nPlayerIndex );
    
    bool LoadObjectInfo( char* strFile );
    bool LoadSpecialObjectInfo( char* strFile );
	
    bool LoadCashObjectLimitCount( const char* strFile_ );	
    bool LoadExceptAgitObjectID( const char* strFile_ );

    bool LoadMapInfo( void );
    void SetObject( std::vector< KAgitObjectPos > &vecObject, std::vector< KAgitCharacterPos > & vecCharter );
    void ClearObject( void );
    void ClearObjectInfo( void );
    void ClearPosObject();
    void RenderAfterUI( void );
    void FrameMoveObject( void );

    bool CheckPickToObjBound( KGC2DObject* pObj, D3DXVECTOR2 dxvMousePos );
    bool CheckPickToObjBound( KGC2DObject* pObj, D3DXVECTOR2 dxvMousePos, float fXWeight );


    void SetObjectPickOff( void );
    bool IsObjectPick( void ) { return m_kObjContainer.IsObjectPick(); }
    KGC2DObject* GetPick2DObject( void ) { return m_pPick2DObject; }

    void RemovePickedObject();
    bool PickUpObject( GCITEMUID itemUID, const D3DXVECTOR2& dxvMousePos = D3DXVECTOR2( -1.f, -1.f ) );
    void UpdatePickedObjectPos( const D3DXVECTOR2& dxvMousePos );

    bool GetObjectInPos( const D3DXVECTOR2& dxvPickPos, OUT GCITEMUID& itemUID_ );
    bool PickUpObject( const D3DXVECTOR2& dxvMousePos );
    void SetAgitEditState( bool bEdit );
    bool GetAgitEditState( void ) { return m_bAgitEdit; }
    void SaveAgitObject();

    void Render( int iLayer_ );
    void FrameMove( void );

    void SetAgitShop( void );
    void SetAgitShopCatalog( std::map<PAIR_USHORT_DWORD,KManufactureItem>& mapItemInfo, std::map<PAIR_USHORT_DWORD,PAIR_INT>& mapItemPos );
    void SetAgitShopMaterial( std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterial );
    void BuyAgitShopObject( GCITEMID ItemID, PAIR_USHORT_DWORD pairIndex = std::pair<USHORT, DWORD>(0, 0) );
    void SetMapCatalog( KEVENT_AGIT_MAP_CATALOGUE_ACK& catalog );
    KEVENT_AGIT_MAP_CATALOGUE_ACK& GetMapCatalog( void ) { return m_kMapCatalog; }

	void UpdateChatBalloon();

    void RefreshInvenObject( void );
    
    bool IsUserInAgit( DWORD dwUserUID );
    void BanUser( DWORD dwBanUserUID );
    void OnBanUser( DWORD dwAgitUID );

    // 오브젝트 배치 테스트용
    bool IsPrePlace( void ) { return m_bPrePlace; }
    void SetPrePlace( bool bSet );

    std::wstring GetAgitName( void ) { return m_kAgitInfo.m_strName; }
    UCHAR GetOpenType( void ) { return m_kAgitInfo.m_ucOpenType; }
    int GetToday( void ) { return m_kAgitInfo.m_nTodayVisiter; }
    int GetTotal( void ) { return m_kAgitInfo.m_nTotalVisiter; }

    void RegisterAgitList( KAgitOption& Info_ );
    void UnRegisterAgitList( void );
    bool GetRegisterState( void ) { return m_bRegistAgitList; }

    bool GetExitGame( void ) const { return m_bExitGame; }
    void SetExitGame( bool bSet ) { m_bExitGame = bSet; }

    bool IsUseKeyCheck( void );

    std::map<PAIR_USHORT_DWORD,KManufactureItem>& GetShopItem( void ) { return m_mapAgitShopObject; }
    std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& GetShopItemMaterial( void ) { return m_mapAgitShopObjectMaterial; }

    const std::map< DWORD, KAgitObjectInfo >& GetAgitObjectMap( void ) { return m_kObjInfoList.GetObjectInfoList(); }
    const std::set< GCITEMID >& GetExeceptAgitObjectSet( void ) { return m_setExceptObjectID; }
    std::map< GCITEMID, int > GetEditableObjectMap( void ) {
        return m_kObjContainer.GetHideObjectCount();
    }

    bool ChecKBoundToflowerPot( D3DXVECTOR2& dxvPos ); //  화분 UID , 씨앗 UID 반환
    bool CheckPlantEnable( void );
    
    std::pair<GCITEMUID, GCITEMUID > GetNowPlantPotUID( void ) { return m_prNowPlantPot; }
    bool SetFlowerPotInfo( KFlowerPot& info_ );
    bool SetSeedInfo( KItem* pSeed_ );
    void ClearNowPlantPot( void );

    void SetUserNameMap( void );
    std::map<DWORD, std::wstring>& GetUserNameMap( void ) { return m_mapUserName; }
    std::wstring GetOwnerName( void );

    void SortObjectForY();

    void BeforeWatchAgit( void );   // 이사 매뉴 중 다른 아지트 구경하기에서 사용 
    void AfterWatchAgit( void );    
    void SetWatchState( bool bState ) { m_bWatch = bState; } // 구경중 상태인가?
    bool GetWatchState( void ) { return m_bWatch; }
    void LeaveWatchAgit( void );    // 구경하기 끝나고 돌아올 때 
    int GetAgitUserNum( void );
    void MoveAgit( void );
    GCITEMUID GetHideObjectUID( GCITEMID itemID_ );

    std::string GetMapFileName( DWORD dwMapID );    
    D3DXVECTOR2 GetFairyTreePos( DWORD dwMapID );
    int         GetMapNameString( DWORD dwMapID );

    // 요정의 나무 
    bool LoadFairyTreeInfo( void );
    void SetFairyTreeInfo( KFairyTree& info, DWORD dwUID = 0x00000000 );
    void ClearFairyTreeInfo( void );
    bool CheckBoundToFairyTree( D3DXVECTOR2& dxvPos );   
    bool EnableWateringFairyTree( void );   // 물 줄수 있나?
    void SetFairyTreeBuff( void );
    void SetFairyTreeBuffInfo( KFairyTreeFruitBuff& buffInfo );
    void SetFairyTreeLevelTable( std::map<int, DWORD>& table_ );
    bool GetFairyTreeExpInfo( const int iLevel_, const DWORD dwCurExp, OUT DWORD* pdwCurExp, OUT DWORD* pdwNextExp );
    
    bool IsNeedUpdate() { return m_kObjContainer.IsNeedUpdate(); }
    bool IsPosUpdated() { return m_kObjContainer.IsPosUpdated(); }

	DWORD GetAgitUID() { return m_kAgitInfo.m_dwUID; }
	bool IsAgitOwner();
	bool IsSameAgit( DWORD dwAigtUID );
    bool IsHideObject( GCITEMUID itemUID_ );

    bool AddDummyObject( GCITEMID itemID_ );
    void RemoveDummyObject();
    void BuyDummyObject();

	void SetMaxChannel( int nMaxChannel_ );

    void RollbackObjectPos();
    void UpdateLatencyInfo( const LTP_AGIT_OBJECT_POS& kPacket );

    void SetInviteAgitInfo( KInviteAgitNot& kInfo_ ) { m_kInviteAgitInfo = kInfo_; }
    KInviteAgitNot GetInviteAgitIno() { return m_kInviteAgitInfo; }
    int GetInvenItemCount( GCITEMID itemID_ );
    int GetCashItemLimitCount( GCITEMID itemID_ );

    void UpdateAgitVisitorCount( KEVENT_AGIT_VISITER_COUNT_BROAD countInfo );
    void RollBackBeforeEdit( void );    // 에디트 이전 상태로 되돌림

    
    bool CheckBoundToGuestBook( D3DXVECTOR2& dxvPos );  // 낙서장
    int ConvertMonsterIDToCharType( int iCharType );    // 격파 몬스터 ID를 CharType으로 얻기

    bool SetTrainingObjInfo( KTrainingObj& info_, bool ImgUpdate_ = true );
    bool CheckBoundToTraining( D3DXVECTOR2& dxvPos );
    int  GetBrokenTrainingTile( GCITEMUID itemUID );
    void ClearTrainingTileImg( GCITEMUID itemUID );
    D3DXVECTOR2 GetTrainingObjPos( GCITEMUID itemUID );

    void SetBrokenTileNum( int iNum ) { m_iBrokenTile = iNum; }
    int  GetBeforeBrokenTile( void ) { return m_iBrokenTile; }

    // 캐릭터 배치 
    std::map<int, KAgitCharacterPos> GetCharacterObjInfo( void ) { return m_kAgitInfo.m_mapCharacterPos; }
    void SetCharacterObject( const std::map<int, KAgitCharacterPos>& mapInfo );
    void InitAgitCharacter( void );
    void DestroyAgitCharacter( void );
    void ClearAgitCharacter( void );
    int CreatePlaceCharacter( const int iCharType, float fPosX, float fPosY );
    void DeletePlaceCharacter( const int iCharType );
    void RemovePickCharacter( void );
    bool PickUpCharacter( const D3DXVECTOR2& dxvMousePos );
    bool PickUpCharacter( const int iCharType, const D3DXVECTOR2& dxvMousePos );
    void UpdatePickedCharacterPos( const D3DXVECTOR2& dxvMousePos );
    void SetCharacterPickOff( void );
    bool IsCharacterPick( void ) { return m_iPickCharIndex != -1; }
    int  GetAgitCharacterMonType( int iCharType );
    void RenderPickCharacter( void );
    std::pair<int, int>  CheckPickToAgitChar( D3DXVECTOR2& dxvMousePos_ ); // <인덱스, 캐릭터 타입 >
    bool IsOpenAgitCharacter( const int iCharType_ );
    int GetCharacterPick( void ) { return m_iPickCharIndex; }

    KAgitCharacterPos GetAgitCharacter( int iCharType_ );
    void SaveAgitCharacterPos( void );
    void UpdateAgitCharacterInfo( KAgitCharacterPos& Info_ );
    void UpdateAgitCharacterMotion( int iCharType_ );
    void UpdateAgitCharacterPos( const std::vector<KSimpleAgitCharPos>& vecPos_ );
    void UpdateAgitCharacterPos( KAgitCharacterPos& Info_ );
    void UpdateAgitCharacterEquip( KAgitCharacterPos& Info_ );

    void SetAgitCharacterPos( const int iCharType_, const int iPosX_, const int iPosY_ );

    bool LoadAgitCharacterMotionInfo( void );
    std::vector< KAgitCharacterMotion >& GetCharMotionList( void ) { return m_vecCharacterMotionInfo; }
    int  GetCharacterMotionID( int iCharType_ );
    bool GetAgitCharacterState( IN int iCharType_, IN int iMotionID_, OUT std::string& strState_ );
    bool GetAgitCharacterMotion( IN int iCharType_, IN int iMotionID_, OUT std::string& strMotion_ );

    std::wstring GetAgitCharacterMent( int iCharType_, int iSpeechType_ );
    void SetAgitCharacterSayBaloon( int iCharType_, int iUID_, KAgitCharacterPos::SPEECH_TYPE eMentType_ );

    bool CheckEnableSitDownObject( D3DXVECTOR2& dxvPos_ );
    int GetPlaceCharacterUID( void );
    void ClearPlaceCharUID( void );

    int GetCharTypeFromAgitMon( int iMon_ );

    bool CheckEnableLieObject( D3DXVECTOR2& dxvPos_ );
	
    bool IsAgitObject( GCITEMID itemID_ );	

    bool IsSpecialObject( GCITEMID _itemID );
    KGCAgitObjectAction* GetObjectAction( GCITEMID _itemID, GCITEMUID _itemUID );

    void UpdateDiceObject( const LTP_AGIT_DICE_OBJECT_STATE& kPacket );
    void UpdateFlagObject( const LTP_AGIT_FLAG_OBJECT_STATE& kPacket );
    bool IsHaveFlag( DWORD dwUserID );

    bool IsAgitEntered() { return m_bAgitEntered; };
    void SetAgitEnterd( IN bool bAgitEntered_ ) { m_bAgitEntered = bAgitEntered_; };

    USHORT GetAgitMaxCharPosCount() { return m_usMaxCharPosCount; };
    void SetAgitMaxCharPosCount( USHORT usMaxCharPosCount ) { m_usMaxCharPosCount = usMaxCharPosCount; };
    USHORT GetAgitCurCharPosCount();

private:
    KAgitInfo   m_kAgitInfo;                     // 입장하는 아지트 정보 
    PAIR_DWORD_USHORT   m_prURServerIpPort;      // 사용할 UDP릴레이 < IP, Port >
    PAIR_DWORD_USHORT   m_prTRServerIpPort;      // 사용할 TCP릴레이 < IP, Port >
    std::map<DWORD,KAgitUserInfo> m_mapAgitUser; // 아지트안에 있는 유저들 정보
    std::map<DWORD, std::wstring> m_mapUserName; // 아지트에 있는 유저들 이름 
    bool    m_bRegistAgitList;                   // 내 아지트가 아지트 리스트에 등록되어 있는가??
    bool    m_bExitGame;

    // 오브젝트 관련 변수 
    KGCAgitObjectInfoList m_kObjInfoList;
    KGCAgitObjectContainer m_kObjContainer;

    std::map< int, std::vector< KGC2DObject* > > m_map2DObj;            // 화면에 그릴 때 사용 < 레이어, 해당 레이어에 그려질 오브젝트 >
    std::pair< GCITEMUID, GCITEMUID > m_prNowPlantPot;  // 현재 심으려는 화분 UID, 씨앗 UID 

    bool            m_bAgitEdit;
    bool            m_bObjectPick;
    KGC2DObject*    m_pPick2DObject;
    D3DXVECTOR2     m_vecPickOffset;

    bool            m_bNowShop;
    bool            m_bShopEdit;
    std::map<GCITEMID, int >                        m_mapCashItemLimitCount;                // 아지트 캐시 아이템 보유 한도 
    std::set< GCITEMID >                            m_setExceptObjectID;                    // 유저가 조작 할 수 없는 아지트 오브젝트 
    std::map<PAIR_USHORT_DWORD,KManufactureItem>    m_mapAgitShopObject;                    // 상정 오브젝트 정보 
    std::map<PAIR_USHORT_DWORD,PAIR_INT>            m_mapAgitShopObjectPos;                 // 상점 오브젝트 위치 
    std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> > m_mapAgitShopObjectMaterial;   // 상점 오브젝트 구매 조건 
    

    bool            m_bPrePlace;

    // 맵 관련  
    std::map< DWORD, kMapInfo >     m_mapMapInfo;       // 맵 로딩시 사용하는 정보 < 맵ID, 판맵 이름 > 
    KEVENT_AGIT_MAP_CATALOGUE_ACK   m_kMapCatalog;      // 구입 가능한 맵 리스들 
    std::map< GCITEMUID, std::pair<int,int> > m_mapBeforeWatchPos;
    std::map< GCITEMUID, std::pair<int,int> > m_mapTranjection;
    bool    m_bWatch;

    // 요정의 나무 
    std::map< int, KFairyTreeInfo >     m_mapFairyTreeInfo;
    std::map< int, DWORD >              m_mapFairyTreeExpTable;
    KFairyTreeDisplay                   m_kFairyTree;
    enum { NUM_FAIRY_TREE_BUFF = 22 };

    KInviteAgitNot                      m_kInviteAgitInfo;

    // 훈련장
    int     m_iBrokenTile;

    // 배치 캐릭터 
    int                                 m_iPickCharIndex;
    int                                 m_iPlaceCharUID;
    std::vector< KAgitCharacterMotion > m_vecCharacterMotionInfo;

    // 아지트 입장/퇴장 상태를 판단하기 위해 저장 해 둔다
    bool                                m_bAgitEntered;

    // 캐릭터 배치 최대 개수
    USHORT                              m_usMaxCharPosCount;
};
    
DefSingletonInline( KGCAgitGameManager );
