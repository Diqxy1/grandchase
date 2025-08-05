#pragma once
#include "GCObjectAndMotion.h"

class KGCRoomUserCharImgS6;
class KGCRoomUserPotionS6;
class KGCPingAntenna;
class KGCCountryGroup;
class KGCObjectAndMotion;

class KGCRoomUserSlotS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomUserSlotS6 );
    /// Default constructor
    KGCRoomUserSlotS6( void );
    /// Default destructor
    virtual ~KGCRoomUserSlotS6( void );
    /// Copy constructor
    KGCRoomUserSlotS6( const KGCRoomUserSlotS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomUserSlotS6& operator=( const KGCRoomUserSlotS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_USER_STATE_READY = 0,
        EM_USER_STATE_SKILL,
        EM_USER_STATE_EQUIP,
        EM_USER_STATE_MISSION,
		EM_USER_STATE_CHANGE,
        EM_USER_STATE_SIZE,
    };

    enum 
    {
        EM_LEVEL_NUM = 2,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void PostChildDraw( void );

public:
    void OnClickOpen();
    void OnClickClose();
    void OnClickKick();
    void OnHurryUpBtn();
    void OnCharSelBtn();

public:
	void SetGradeTex(int index, KD3DWnd* pWnd);
    void SetNumberTex( int index, KD3DWnd *pWnd );
    void ChangeUVtoTexel( D3DXVECTOR4& vUV );
    
    void ChangeOpenSlot( bool bOpenClick );
    bool EnableOpenSlotClieck();

public:
    void SetPingState( DWORD dwPing_ );
    void Send_ChangeRoomUserInfoReq(int iChar);
    
public:
    void Destroy();
    void UpdatePlayerData( int nPlayerIndex, bool bAddMotion = false );
    void UpdateSlot( bool bSlot );
    void UpdateMyPlayer();
    void UpdateCharData( PLAYER* pPlayer );
#ifdef ENCHANT_EFFECT
    void UpdateEnchantEffect(PLAYER* pPlayer);
    void ClearEnchantEffect();
#endif
    void UpdateCharPet( PLAYER* pPlayer );
    void UpdateEtcUI( PLAYER* pPlayer );
	void UpdateMark( PLAYER* pPlayer );
    void ClearPlayerData();
    void UpdatIdleState( PLAYER* pPlayer );
    void UpdateState( int nState );    
    void SetIndex( int nIndex ) { m_iIndex = nIndex; }
    int GetIndex() { return m_iIndex; }
    bool IsTeamMode();
    void SetVitalityInfo();
    void OnChangeLeader();
    void SetExtraRoomObject( bool bAddMotion = false );

public:
    KSafeArray<KD3DWnd*,EM_USER_STATE_SIZE>     m_apkUserState;
    KSafeArray<KD3DWnd*,EM_LEVEL_NUM>           m_apkUserLevel;    
    KD3DWnd*                                    m_pkLevelMark;
    KD3DWnd*                                    m_pkHostMark;
    KD3DWnd*                                    m_pkOpenMark;
    KD3DWnd*                                    m_pkCloseMark;
    KD3DWnd*                                    m_pkOpenMark2;
    KD3DWnd*                                    m_pkCloseMark2;
    KD3DWnd*                                    m_pkCloseMarkPanel;
    KD3DWnd*                                    m_pkKickBtn;
    KD3DWnd*                                    m_pkSlotBack;
    KD3DWnd*                                    m_pkMySlotBack;
    KGCCommonItemWnd*                           m_pkPetCard;
    KGCRoomUserCharImgS6*                       m_pkCharImg;
#ifdef POTION_ROOM_SHOW
    KGCRoomUserPotionS6*                        m_pkPotionSlot;
#endif
    KD3DStatic*                                 m_pkCharName;
    KGCPingAntenna*                             m_pkPingAntenna;
    KGCCountryGroup*                            m_pkCountryGroup;
	KD3DWnd*                                    m_pkIdleState;
	KGCProgressBtn*		                        m_pkHurryUpBtn;
    KD3DWnd*                                    m_pkBackVitality;
    KD3DWnd*                                    m_pkGuageVitality;
    KD3DWnd*                                    m_pkGuageVitalityRect;
    KD3DStatic*                                 m_pkStaticVitality;
    KD3DWnd*                                    m_pkFlag;
	KD3DWnd*			                        m_pkCharSelBtn;
	KD3DWnd*			                        m_pkRank;
	KD3DWnd*			                        m_pkDisableImg;
	KD3DWnd*			                        m_pkCharBanMark;
	KD3DWnd*			                        m_pkRankBanMark;
    KD3DWnd*			                        m_pkChangeLeader;

    PLAYER                                      m_pkTempPlayer;

public:
    int                                         m_iIndex;
    bool                                        m_bInitChar;
    KPet*                                       m_pkPet;
    int                                         m_iCharType;
    bool                                        m_bBaseEquip;
    bool                                        m_bAnimation;

#ifdef SHOW_SBTAINROOM
private:
    int GetDrawItemID();
#endif

#ifdef ENCHANT_EFFECT
    bool    m_bRenderEnchatEffect;
    std::vector< CParticleEventSeqPTR > m_vecEnchantParticle;
#endif
#ifdef SHOW_SBTAINROOM
protected:
    float       m_fXTerm;
    float       m_fYTerm;
    float       m_fWidth;
    float       m_fHeight;
    bool        m_bLinearFilter;
#endif
    void SetDrawItemTexture( KPetInfo pPetInfo_ );
    RoomAnimationInfo   m_pkRoomAnimation;
    KGCObjectAndMotion* m_pkExtraObject;
};

DEFINE_WND_FACTORY( KGCRoomUserSlotS6 );
DECLARE_WND_FACTORY( KGCRoomUserSlotS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomUserSlotS6 );
