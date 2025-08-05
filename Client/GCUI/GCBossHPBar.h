#ifndef _GCBOSSHPBAR_H_
#define _GCBOSSHPBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
class KGCHPGauge;

class KGCBossHPBar : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCBossHPBar );
    /// Default constructor
    KGCBossHPBar( void );
    /// Default destructor
    virtual ~KGCBossHPBar( void );
    /// Copy constructor
    KGCBossHPBar( const KGCBossHPBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBossHPBar& operator=( const KGCBossHPBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum 
    {
        EM_BOSS_GORGOS = 0,
        EM_BOSS_GAIKOZU,
        EM_BOSS_PATUSEI,
        EM_BOSS_ELENA,
        EM_BOSS_KAMIKI,
        EM_BOSS_GADOSEN,
        EM_BOSS_SANGSUN,
        EM_BOSS_KAKAIRU,
        EM_BOSS_KAZEAZE_PRETRANS,
        EM_BOSS_KAZEAZE_TRANS,
        EM_BOSS_BASILISK,
        EM_BOSS_ENT,
        EM_BOSS_WENDY,
        EM_BOSS_STONE_GOLEM,
        EM_BOSS_CYCLOPSE,
        EM_BOSS_BLACKFAIRYQUEEN,
        EM_BOSS_KNIGHTORC,
        EM_BOSS_DARKAMON,
        EM_BOSS_RENASIEN,
        EM_BOSS_OCTUS,
        EM_BOSS_HARPYQUEEN,
        EM_BOSS_ORCLORD,
        EM_BOSS_TROLL,
        EM_BOSS_DRILLMON,
        EM_BOSS_ELIZABETH,
        EM_BOSS_LICH,
        EM_BOSS_PARADOM,
        EM_BOSS_BULTAROS,
        EM_BOSS_GODOFCIRCLE,
        EM_BOSS_ACIENTENT,
        EM_BOSS_KROKO,
        EM_BOSS_ZIG,
        EM_BOSS_HAAS,
        EM_BOSS_ANTOINETTE,
        EM_BOSS_JURIOR,
        EM_BOSS_PERIOT,
        EM_BOSS_DRAKEWORM,
        EM_BOSS_VICTOR,
        EM_BOSS_DIVINETREE,
        EM_BOSS_TANATOSS1,
        EM_BOSS_TANATOSS2,
        EM_BOSS_TANATOSS3,
        EM_BOSS_SETESH,
        EM_BOSS_HATSHEPSUT,
        EM_BOSS_ELDER_KUNG,
        EM_BOSS_TALIN,
        EM_BOSS_SANDRULER,
        EM_BOSS_SENTRYKEEPER1,
        EM_BOSS_SENTRYKEEPER2,
        EM_BOSS_ARON,
        EM_BOSS_NEMOPHILLA,
        EM_BOSS_VULCANUS,
        EM_BOSS_VAIRNE,
        EM_BOSS_DRALL,
        EM_BOSS_ZDLER,
        EM_BOSS_HZDLER,
        EM_BOSS_DUEL,
        EM_BOSS_CASILLAS,
        EM_BOSS_CAPTAIN,
        EM_BOSS_FIREHEATER_UNDER,
        EM_BOSS_FIREHEATER_UPPER,
        EM_BOSS_ASTAROT,
        EM_BOSS_DAHLIA,
        EM_BOSS_CHROMOS,
        EM_BOSS_WOMANMASTER,
        EM_BOSS_NINETAILS,
        EM_BOSS_BERKAS,
        EM_BOSS_KUFU,
        EM_BOSS_KARTE,
        EM_BOSS_BARAKUP,
        EM_BOSS_ASTAROT_PHASE1,
        EM_BOSS_ASTAROT_PHASE2,
        EM_BOSS_ASTAROT_PHASE3,
        EM_BOSS_BETTLESHIP,
        EM_BOSS_TALIN_ROID,        
        EM_BOSS_PENCERGUARD,
        EM_BOSS_GATEKEEPER,         
        EM_BOSS_REDARACHNIA,
        EM_BOSS_TARIC,
        EM_BOSS_MAGICIAN,
        EM_BOSS_SHOTER,
        EM_BOSS_CHIEFGUARD,
        EM_BOSS_COOKING_MGR,
        EM_BOSS_JONJALLE,
        EM_BOSS_BUTLER_HEAD,
        EM_BOSS_GIGANTIC_REDARACHNIA,        
        EM_BOSS_SUVALLU_COUNT,
        EM_BOSS_GIGANTIC_GREYARACHNIA,        
        EM_BOSS_ARACHNE,
        EM_BOSS_VIOLETARACHNIA,
        EM_BOSS_ROCKFIST_GOLIATH,
        EM_BOSS_STONESPIRIT_GOLIATH,
        EM_BOSS_GRANDROCK_TROKA,
        EM_BOSS_ROCKSPIRIT_GOLIATH,
        EM_BOSS_SCULLWING_SCYLER,
        EM_BOSS_SHADOWFOREST_WARRIOR,
        EM_BOSS_SHADOWFOREST_BERSERKER,
        EM_BOSS_SHADOWFOREST_LAKLAK,
        EM_BOSS_HUNTINGFIELD_MAGICIAN,
        EM_BOSS_HUNTINGFIELD_LOKLOK,
        EM_BOSS_GROUNDOFDESPAIR_PERSUER,
        EM_BOSS_GROUNDOFDESPAIR_OVERWATCHER,
        EM_BOSS_GROUNDOFDESPAIR_MORIDE,
        EM_BOSS_DEMENSIONFRONTIER_SHAMAN,
        EM_BOSS_DEMENSIONFRONTIER_MAGICIAN,
        EM_BOSS_DEMENSIONFRONTIER_GARUDE,
        EM_BOSS_INFINITY_WOODEN,
        EM_BOSS_INFINITY_SHARK_ARON,
        EM_BOSS_INFINITY_FURIUS_STORM,
        EM_BOSS_INFINITY_DARK_STORM,
        EM_BOSS_INFINITY_KUNGJI_GUARDIAN,
        EM_BOSS_INFINITY_HOOD_SCORPION,
        EM_BOSS_INFINITY_GIANT_FRILLZARD,
        EM_BOSS_INFINITY_POWER_SHOVEL,
        EM_BOSS_INFINITY_SPIDER_GUARD_MACHINE,
        EM_BOSS_INFINITY_COWNAT_DARKSPELL,
        EM_BOSS_INFINITY_COWNAT_DARKSWORD,
        EM_BOSS_INFINITY_BLACK_FAIRY_QUEEN,
        EM_BOSS_EXTINCTION_KAZEAZE,
        EM_BOSS_EXTINCTION_GAIKOZU,
        EM_BOSS_EXTINCTION_GORGOS,
        EM_BOSS_EXTINCTION_KAMIKI,
        EM_BOSS_EXTINCTION_GADOSEN,
        EM_BOSS_HENIR_OBEZAAR,
        EM_BOSS_GOLDEN_SLIMEKING,
        EM_BOSS_RONAN_ATONEMENT,
        EM_BOSS_RONAN_ATONEMENT_1,
        EM_BOSS_PROJECT_MARK_BOSS,
        EM_BOSS_CAOS_FURIA,
        EM_BOSS_OLDGROGOS,
        EM_BOSS_OLDBASILISK,
        EM_BOSS_OLDGADOSEN,
        EM_BOSS_OLDKAZEAZE,
        EM_BOSS_OLDKAZEAZE1,
        EM_BOSS_SANGSUN1,
        EM_BOSS_DOLL,
        EM_BOSS_HARPE_MISTICAL,
        EM_BOSS_ROSSO,
        EM_BOSS_FURIA1,
        EM_BOSS_DIO_YOKAI,
        EM_BOSS_TIMERIC,
        EM_BOSS_GRANDIEL_HALLOWEEN,
        EM_BOSS_HANOUT,
        EM_BOSS_NEW_RECLUSE,
        EM_BOSS_NEW_DARK_RECLUSE,
        EM_BOSS_NEW_MALICE,
		EM_BOSS_RAIDFIRE_HELIO,
		EM_BOSS_4TH_ARENA,
        EM_BOSS_ANGRYBOSS_IGNIZ,
        EM_BOSS_ANGRYBOSS_EVILIS,
        EM_BOSS_DUN_ALICE_BOSS,
        EM_BOSS_DUN_EASTER_CHOCO_SLIME,
        EM_BOSS_CRIPTA_BOSS,
        EM_BOSS_ANGRYBOSS_IGNIZ_ICE,
        EM_BOSS_HENIR_CRIMSON_EYE,
        EM_BOSS_HENIR_RONAN,
        EM_BOSS_HENIR_LASS,
        EM_BOSS_SLIMEKING_ICE,
        EM_BOSS_SLIMEKING_FLAME,
        EM_BOSS_SLIMEKING_THUNDER,
        EM_BOSS_SLIMEKING_DEMON,
        EM_BOSS_GCORIGIN_BERD_PHASE_01,
        EM_BOSS_GCORIGIN_BERD_PHASE_02,
        EM_BOSS_GCORIGIN_BERD_PHASE_03,
        EM_BOSS_HARQUION_COLISEU,
        EM_BOSS_GCORIGIN_SEIR_PHASE_01,
        EM_BOSS_GCORIGIN_RAN_PHASE_02,
        EM_BOSS_GCORIGIN_RAN_PHASE_03,
        EM_BOSS_SIZE = 168,
    };

public:

    void InitBossMon();
    void EnableHPBar( bool bEnable, int iMonsterType );
    void HideBossHPBar();
    int GetBossIndex( int nBossType );

    bool IsEnable( void ) { return m_bEnable; }
    void SetNowHP( float fNowHP );
    void SetMaxHP( float fMaxHP );

    inline float GetNowHP( void ) const { return m_fNowHP; }
    inline float GetMaxHP( void ) const { return m_fMaxHP; }

    void SetAnimationPos( float startY_, float endY_ ) { m_fOffset_Start_Y = startY_, m_fOffset_End_Y = endY_; }
    void RenderOffHPBar();

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw();

protected:
    float       m_fNowHP;
    float       m_fMaxHP;
    bool        m_bEnable;
    float       m_fTimeParam;
    float       m_fOffset_Start_Y;
    float       m_fOffset_End_Y;

    int         m_nBossType;                // 현재 활성화 되어 있는 보스 UI Enum
    int         m_nMonType;                 // 현재 활성화 되어 있는 몬스터 Enum

    std::map<int,int>   m_mapBossHPInfo;    //  MonEnum, BossHpEnum;

    struct BossImageHPInfo {
        int			nBossImageIndex;
        std::string	strTexName;
        int			nTexTU1;
        int			nTexTV1;
        int			nTexTU2;
        int			nTexTV2;
    };

    KSafeArray<BossImageHPInfo, EM_BOSS_SIZE>  m_pkBoss;

    KGCHPGauge* m_pkHPGauge;
    GCDeviceTexture* m_pkBossImageTexture;

    //그림 텍스쳐의 좌표
    D3DXVECTOR2 m_vImageTexStartAxis;
    D3DXVECTOR2 m_vImageTexEndtAxis;

    //그림 사이즈
    int nXSize;
    int nYSize;
};

DEFINE_WND_FACTORY( KGCBossHPBar );
DECLARE_WND_FACTORY( KGCBossHPBar );
DECLARE_WND_FACTORY_NAME( KGCBossHPBar );

#endif // _GCBOSSHPBAR_H_
