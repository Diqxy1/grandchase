#pragma once
#include "KNCSingleton.h"
#include "ParticleLib/KTDGParticleSystem.h"

class PLAYER;
struct KCoupleInfo;
struct SCoupleInfo;
class GCSquarePeople;
typedef std::vector<SUserItem>  KUserItemVector;
typedef std::vector<GCITEMID>   KItemIDVector;

struct SCoupleParticle
{
	std::string strName;
	D3DXVECTOR3 vPos;
	int iLayer;

	SCoupleParticle()
		:vPos( 0, 0, 0 )
		, iLayer( (int)GC_LAYER_CUSTOM )
	{

	}
};

struct SCoupleMesh
{
	std::string strMesh;
	std::string strTexture;
	D3DXVECTOR3 vTranslation;
	D3DXVECTOR3 vScale;
	D3DXVECTOR3 vRotation;
	SCoupleMesh()
		:vTranslation(0, 0, 0)
		,vScale(0, 0, 0 )
		,vRotation(0, 0, 0 )
	{
	
	}
};

struct SCoupleBG
{
	std::string strMesh;
	std::string strTexture;
};

struct SCoupleItemInfo
{
	int iItemID;
	std::vector<SCoupleParticle> vecParticle;
	std::vector<SCoupleMesh> vecMesh;
	SCoupleBG sBackground;
};

struct SCoupleInfoSimple
{
	DWORD dwUID;
	DWORD dwPartnerUID;
	std::wstring strCoupleNickName;
	std::wstring strCoupleNickColor;
	KUserItemVector vecItemInfo;

	SCoupleInfoSimple() {};
	SCoupleInfoSimple( const KSquareUserInfo& src_ );
	SCoupleInfoSimple( const KInDoorUserInfo& src_ );
	SCoupleInfoSimple( const SCoupleInfo& src_, bool bPartner );
	SCoupleInfoSimple( const KCoupleInfo& src_, bool bPartner );
};

struct SCoupleInfo
{
	DWORD	dwUID;
	DWORD	dwPartnerUID;
	int		iCharType;
	int		iPartnerChar;
	bool	bPosition;
	bool	bPartnerPosition;
	std::wstring	strNickName;
	std::wstring	strNickColor;
	std::wstring	strPartnerNick;
	std::wstring	strPartnerNickColor;
	KUserItemVector vecItemInfo;
	KUserItemVector vecPartnerItem;
	time_t                      m_tmRegDate;

	void ConvertItemList( std::vector<KCoupleEquip> vecItemList, bool bPartner );
	SCoupleInfo& operator= ( const KCoupleInfo& _src );
	SCoupleInfo()
	{
		dwUID = 0;
		dwPartnerUID = 0;
		iCharType = 0;
		iPartnerChar = 0;
		bPosition = true;
		bPartnerPosition = false;
		m_tmRegDate = 0;
	}
	void Init();
};

class KGCCoupleSystem
{
	DeclareSingleton( KGCCoupleSystem );
public:
	KGCCoupleSystem(void);
	~KGCCoupleSystem(void);

	enum GCCoupleState {
		GCCS_SOLO,
		GCCS_WAIT,
		GCCS_COUPLE,
		GCCS_COUPLE_WND,
		GCCS_IN_GAME,
		GCCS_DISCONNECT,
	};

	bool Am_I_Solo();

	DWORD GetUID_of_my_honey();
	DWORD GetCoupleUID( DWORD dwUID );
	bool CheckCoupleWithMe( DWORD dwUID, bool bSameTeam );
	bool IsCoupleDay();
	std::wstring GetNickName_of_my_honey();
	std::wstring GetNickColor_of_my_honey();

	DWORD GetRingItemID( bool bMyCouple );
	DWORD GetRingID( DWORD dwUserUID );

	bool Are_you_Solo( DWORD dwUID );
	bool Are_you_My_Honey( DWORD dwUID );
	std::wstring GetNickName_of_your_honey( DWORD dwUID );
	std::wstring GetNickColor_of_your_honey(DWORD dwUID);

	std::wstring GetNickName( bool bMyCouple, bool bPartner );

	int GetMyCoupleDay();
	int GetYourCoupleDay();

	void AddCoupleUserInfo( SCoupleInfoSimple sCoupleInfo );
	void RemoveCoupleUserInfo( DWORD dwUserUID );
	void ClearCoupleUserInfo();

	void SetMyCoupleFullInfo( KCoupleInfo& kCoupleInfo );
	void SetTempCoupleFullInfo( SCoupleInfo& sCoupleInfo );
	void InitMyCoupleInfo();
	void InitTempCoupleInfo();

	void SendPropose( DWORD dwPartnerUID, DWORD dwRingID, GCITEMUID dwRingUID, std::wstring strMessage );
	void SendAccept( bool bAccept );
	void SendDenyToday();
	void SendDenyThisUser( DWORD dwUserUID );
	void SendGoodbye( std::wstring strMsg );
	void SendEquipItemChange( KSimpleItem kItem );
	void SendEquipItemChange( DWORD dwItemID );
	void SendCharacterChange( int iCharType );
	void SendRingChange( DWORD dwItemID, GCITEMUID dwItemUID, std::wstring strMessage );
	void SendRingAccept( bool bAccept );
	void ApplyEquipItemChange();
	void FixEquipItemChange( const std::vector<KCoupleEquip>& vecEquipList );

	void RecvEquipItemChange( const std::vector<DWORD>& vecItem );
	void RecvChangeCharacter( int iCharType , bool bPartners );
	void RecvExpiredItemList( const std::vector<KSimpleItem>& vecItems );
	void RecvChangeCoupleRing( const KSimpleItem& kItem, bool bInCoupleInven );
	void RecvCoupleChat( std::wstring strMessage );

	void ReadyToInsertInven( DWORD dwItemID, GCITEMUID dwItemUID, bool bEquipDirect = true );
	void SendInsertToInven();

	void AddItemToInven( const KSimpleItem& kItem );

	KUserItemVector GetEquipItem( bool bRight, bool bMyCouple );
	KUserItemVector	GetMyEquipItem();

	std::vector<KSimpleItem> GetRingList( bool bInventory, bool bCoupleInventory, bool bFilterOverlap = false );
	std::vector<KSimpleItem> GetCoupleItem( int iCol, int iRow, int iCurrentLine );
	bool IsInCoupleInven( GCITEMUID dwItemUID );
	bool IsEquipedCoupleItem( DWORD dwItemID );
	int	GetCoupleItemNum();
	int GetCoupleCharacter( bool bMyCouple, bool bPartner );
	bool GetPosition( bool bMyCouple );
	bool GetTempPosition();
	bool IsCoupleChatExist();
	std::wstring GetCoupleChat();
	void RingAbility( PLAYER* pPlayer, short& shAttack, short& shDefence, short& shHpInc );

	void NearCoupleEffect( PLAYER* pPlayer );
	void NearCoupleEffect( GCSquarePeople* pPlayer );

	void SetState( GCCoupleState eState );
	GCCoupleState GetState() { return m_eState; }

	void SetPartnerState( GCCoupleState eState ) { m_ePartnerState = eState; };
	GCCoupleState GetPartnerState() { return m_ePartnerState; }

	void _DeleteNearEffect( DWORD dwUID );

	std::pair<bool, SCoupleItemInfo> CoupleItemScript( int iItemID );

private:
	
	void _EquipBaseItem( int iCharType, KUserItemVector& vecItem, bool bForceEquipAll = false );
	KUserItemVector _FilterBaseItem( const KUserItemVector& vecItem );
	void _FilterEquipedItem( std::vector<KSimpleItem>& vecItems );
	void _RemoveItemInCoupleInven( GCITEMUID dwUID );
	void _UnEquipItem( DWORD dwItemID );
	void _SendEquipItemChange();

private:
	bool	m_bIamSolo;
	bool	m_bTodayDeny;
	GCCoupleState	m_eState;
	GCCoupleState	m_ePartnerState;
	KUserItemVector m_MyEquip;
	std::set<DWORD>						m_setDenyUsers;
	std::vector<KSimpleItem>			m_vecCoupleInven;
	std::map< DWORD, SCoupleInfoSimple >	m_mapCoupleUserInfo;
	std::map< DWORD, std::set< CParticleEventSeqPTR> >	m_mapParticle;
	std::queue< std::wstring >			m_qCoupleChat;

	SCoupleInfo		m_sMyCoupleInfo;
	SCoupleInfo		m_sTempCoupleInfo;

public:
    bool GetCoupleItemVector( DWORD dwUserUID, IN KUserItemVector** ppVecCoupleItem_ );
};

DefSingletonInline( KGCCoupleSystem );
