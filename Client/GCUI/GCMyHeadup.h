#ifndef _GCMYHEADUP_H_
#define _GCMYHEADUP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"
#include "GCSizingThreeBox.h"

class KGCGameSkillBox;
class GCDeviceTexture;
class KD3DStatic;
class GCSPGauge;
class KGCSavingGauge;

class KGCMyHeadup : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCMyHeadup );
    /// Default constructor
    KGCMyHeadup( void );
    /// Default destructor
    virtual ~KGCMyHeadup( void );
    /// Copy constructor
    KGCMyHeadup( const KGCMyHeadup& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMyHeadup& operator=( const KGCMyHeadup& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:    
	void OnInitialize();
	void CharFaceLock( int iChar ){ m_apkCharFace[iChar]->Lock( true ); }

protected:
    enum
    {
        MAX_LEN_HP_GAUGE		= 205,
        MAX_LEN_MP_GAUGE		= 199,
        NUM_STATUS_BAR			= 6,
		MAX_LEN_PET_MP_GAUGE	= 105,		
    };

protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void SetHP( float fHP, bool bHPStop = false );
    void SetMP( float fMP );
    void SetMPGather( float fMPGather );    
	void SetPlayerSkillType( int iType );

	void SetPetMP( float fMP );
	bool IsEnableUsePetSkill( int iIndex_ );

protected:    
    int         m_iPlayerIdx;
    int         m_iMPGatherLevel;
    D3DXVECTOR2 m_vHUDPos;

    KD3DWnd*    m_pkHUDMainPart;
    KD3DWnd*    m_pkHUDMainPart4;
    KD3DWnd*    m_pkHUDSubPart;
    KGCSizingThreeBox*    m_pkHUDChatBar;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkChar;

	KSafeArray<KD3DWnd*,2> m_pkSkillType;		//2개 이상 안될거 같으니깐..하드...ㅋㅋㅋㅋ uk!!
	

    KD3DWnd*    m_pkBlackMP;
    KD3DWnd*    m_pkWhiteMPGauge;
    KD3DWnd*    m_pkWhiteMP4Gauge;
    KD3DWnd*    m_pkMPFull;

    // HP
    D3DXVECTOR4 m_vHPGaugeTexCoords;
    KD3DWnd*    m_pkHPGauge;

    // MP
    D3DXVECTOR4 m_vMPGaugeTexCoords;
    D3DXVECTOR4 m_vMPGaugeTexCoordsEx;
	D3DXVECTOR4 m_vAPGaugeTexCoords;
    D3DXVECTOR4 m_vAPGaugeTexCoordsEx;
    D3DXVECTOR4 m_vMPGatherTexCoords;
    D3DXVECTOR4 m_vMPGatherTexCoordsEx;
	D3DXVECTOR4 m_vAPGatherTexCoords;
    D3DXVECTOR4 m_vAPGatherTexCoordsEx;
    KD3DWnd*    m_pkMPGauge;
	KD3DWnd*    m_pkAPGauge;
    KD3DWnd*    m_pkMPGather;
	KD3DWnd*    m_pkAPGather;
    KD3DWnd*    m_pkMPGaugeEx;
    KD3DWnd*    m_pkMPGatherEx;
    KD3DWnd*    m_pkBlackGather;
    KD3DWnd*    m_pkAPGaugeSP4;
    KD3DWnd*    m_pkAPGatherSP4;
    KD3DWnd*    m_pkAPBack;
	
    KD3DWnd*    m_pkMP1st;
    KD3DWnd*    m_pkMP2nd;
    KD3DWnd*    m_pkMP3rd;

    KD3DWnd*    m_pkMP1stEx;
    KD3DWnd*    m_pkMP2ndEx;
    KD3DWnd*    m_pkMP3rdEx;
    KD3DWnd*    m_pkMP4thEx;

    KD3DWnd*    m_pkLockMP4Mark;

	// 펫 MP
	D3DXVECTOR4 m_vPetMPGaugeTexCoords;
//	D3DXVECTOR4 m_vPetMPGatherTexCoords;
	KD3DWnd*    m_pkPetMPBack;
	KD3DWnd*    m_pkPetMPGauge;
	KD3DWnd*    m_pkPetMPGather;
	KD3DWnd*	m_pkPetLogo;
    KD3DWnd*    m_pkPetMPGageDisable;


	KD3DWnd*    m_pkHPStop;

    KD3DWnd*        m_pkDeathKill;
    KGCUINumber*    m_pkDeathKillNumber;

    // Item cursor
    KSafeArray<KD3DWnd*,NUM_TAKE_ITEM> m_apkItemCursor;
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_apkCharFace;

    KD3DStatic* m_pkRemainSpecial;
    std::wstring m_strRemainSpecial;
    //KD3DWnd*    m_pkBgReMain;

    KSafeArray<KD3DStatic*,NUM_TAKE_ITEM> m_pkNextTagTime;

    std::vector< int > m_vecCharSlot;

    DWORD   m_dwNextTagTime[NUM_TAKE_ITEM];
    int     GetNextTagTimeSec( DWORD dwTime );
public:
    GCSPGauge* m_pkSpGauge;
    KGCSavingGauge* m_pkSavingGauge;
	KGCGameSkillBox* m_pGameSkillBox;	

    void SetRemainString( std::wstring& strRemain );
    void SetNextTagTime( int iCharType, DWORD dwTIme );
	void SetSPAmount(float fAmount );

    void SetChar( int iChar );
    void ToggleAllUI_Base( int iChar );
    void ToggleAllUI_Dungeon64( int iChar );
    void ToggleBlackGauge( bool bShow );

public:
    GCDeviceTexture*    m_pkDun64CharLife;
    GCDeviceTexture*    m_pkDun64GameItem;
    GCDeviceTexture*    m_pkDun64X;
};

DEFINE_WND_FACTORY( KGCMyHeadup );
DECLARE_WND_FACTORY( KGCMyHeadup );
DECLARE_WND_FACTORY_NAME( KGCMyHeadup );

#endif // _GCMYHEADUP_H_
