#pragma once
#include "ParticleLib/KTDGParticleSystem.h"

class KGCCoupleMark : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCCoupleMark );

	KGCCoupleMark(void);
	virtual ~KGCCoupleMark(void);
	/// Copy constructor
	KGCCoupleMark( const KGCCoupleMark& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleMark& operator=( const KGCCoupleMark& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum ECoupleMarkKind{
		GCCM_MARK_YELLOW = 0,
		GCCM_MARK_PUPPLE,
		GCCM_MARK_RED,
		GCCM_MARK_NUM,
	};

	enum ECoupleMarkState{
		GCCM_ACTIVE = 0,
		GCCM_LOCKED,
		GCCM_AFTER_EFFECT,
		GCCM_STATE_NUM,
	};

	enum ECoupleMarkSize{
		GCCM_MINI_SIZE      = 0,
		GCCM_NORMAL_SIZE    = 1,
	};

public:
	void OnCreate( void );
	void FrameMoveInEnabledState(void);
    void FrameMoveInDisabledState( void );

	void SetSize( ECoupleMarkSize eSize );
	void SetCoupleUID( DWORD dwMyUID, DWORD dwCoupleUID );

	void MarkDisable();
    void DeleteParticle();

	void ForceSetHeart( ECoupleMarkKind eKind );
	void ForceShowParticle( ECoupleMarkSize eSize );

	ECoupleMarkKind GetCurrentMark();

private:
	void CreateParticle( ECoupleMarkSize eSize );
	void ToggleMark( ECoupleMarkKind eKind = GCCM_MARK_NUM );

private:
	static ECoupleMarkKind ms_eLastUsedMark;
	static std::map<DWORD, KGCCoupleMark::ECoupleMarkKind> ms_mapCouple;
	bool				m_bForceMode;
	DWORD				m_dwMyUID;
	DWORD				m_dwPartnerUID;
	ECoupleMarkState	m_eCoupleState;
	ECoupleMarkKind		m_eKind;
	ECoupleMarkSize		m_eSize;

	std::vector< CParticleEventSeqPTR > m_vecParticle;

	KSafeArray<KD3DWnd*,GCCM_MARK_NUM> m_pkMark;
	KD3DWnd*			m_pkLocked;
};

DEFINE_WND_FACTORY( KGCCoupleMark );
DECLARE_WND_FACTORY( KGCCoupleMark );
DECLARE_WND_FACTORY_NAME( KGCCoupleMark );
