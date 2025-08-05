#ifndef _GCSTATUSINFOPANEL_H_
#define _GCSTATUSINFOPANEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCUserItemBox;
class KD3DStatic;
class KGCQuestRankScore;
class KGCBadUserInfo;
class KD3DImageIcon;

class KGCStatusInfoPanel : public KD3DWnd,        // extends
                           public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCStatusInfoPanel );
    /// Default constructor
    KGCStatusInfoPanel( void );
    /// Default destructor
    virtual ~KGCStatusInfoPanel( void ) {} ;
    /// Copy constructor
    KGCStatusInfoPanel( const KGCStatusInfoPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCStatusInfoPanel& operator=( const KGCStatusInfoPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	enum{
		GRADE_SS = 0,
		GRADE_S_PLUS = 1,
		GRADE_S = 2,
		GRADE_S_MINUS = 3,	
		GRADE_A_PLUS = 4,
		GRADE_A = 5,
		GRADE_A_MINUS = 6,
		GRADE_B_PLUS = 7,
		GRADE_B = 8,
		GRADE_B_MINUS = 9,
		GRADE_C_PLUS = 10,
		GRADE_C = 11,
		GRADE_C_MINUS = 12,
		GRADE_D_PLUS = 13,
		GRADE_D = 14,
		GRADE_D_MINUS = 15,
		GRADE_E = 16,
		GRADE_F = 17,
		MAX_NUM_GRADE,
	};
    void ResetInfo();
    void SetStartUserInfo( PLAYER* pPlayer = NULL, bool bRoom = false );
    void SetChangeUserInfo( PLAYER* pPlayer = NULL, bool bRoom = false );
    void SetText(); 
    void SetRoomUserInfo ( bool bRoomUserInfo ) { m_bRoomUserInfo = bRoomUserInfo; }
    
protected:
    std::wstring GetDifString(int iDiff_);
    std::wstring GetDifString(float fDiff_);
    std::wstring GetOverString(float fOver_);
    std::wstring GetDifString(float fDiff0_, float fDiff1_);

    std::wstring GetStatusString(int iIndex_, int iChangeVal_, int iStartVal_);
    std::wstring GetStatusString(int iIndex_, float fChangeVal_, float fStartVal_, bool IsOver = false);
    std::wstring GetStatusString(int iIndex_, float fChangeVal0_, float fStartVal0_, float fChangeVal1_, float fStartVal1_);
    
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();

	void SetListAutoDevideStringCnt( int &index, int &AutoDevideStringCnt ); 

	virtual void ActionPerformed( const KActionEvent& );

	void OnScroll(const KActionEvent&);

    void SetPromotionStatus(int iPromotion, int& iAtk, int& iDef, int& iHp, float &fCritical);
	void SetListAutoDevideString( int index);

protected:
    enum {
        TotalAck = 0,
        Atk,
        Def,
        HP,
        SPAtk,
        SPDef,
        Critical,
        CriticalDamage,
        CounterDef,
#if defined( USE_MATCH_STATUS_RATE )
        CounterDefMatch,
#endif
        HPRecovery,
        MPRecovery,
#if defined( USE_MATCH_STATUS_RATE )
        MPRecoveryMatch,
#endif
        HPDrain,
        MPDrain,
        GPAdd,
        EXPAdd,
    	DarkDamage,
        PetMPRecUp,

        ElyosResist,

        Num_Status
    };

protected:

	KD3DScrollbar* m_pkScrollbar;	
    KD3DListBox* m_pkListDesc;
    KD3DListBox* m_pkListStatus;

    int m_iStartGrade, m_iChangeGrade;
    int m_iStartAtk, m_iChangeAtk;
    int m_iStartTotalAtk, m_iChangeTotalAtk;
    int m_iStartDef, m_iChangeDef;
    int m_iStartHP, m_iChangeHP;
    int m_iStartSPAtk, m_iChangeSPAtk;
    int m_iStartSPDef, m_iChangeSPDef;

    int m_iStartHPDrain, m_iChangeHPDrain;
    int m_iStartMPDrain, m_iChangeMPDrain;

    float m_fStartCritical, m_fChangeCritical;
    float m_fStartCriticalDamage, m_fChangeCriticalDamage;
    float m_fStartCounterDef, m_fChangeCounterDef;
    float m_fStartHPRecovery, m_fChangeHPRecovery;
    float m_fStartMPRecovery, m_fChangeMPRecovery;
    float m_fStartGPAdd, m_fChangeGPAdd;
	float m_fStartEXPAdd, m_fChangeEXPAdd;
    int	m_iStartDarkAtkDamage, m_iChangeDarkAtkDamage;
    float m_iStartPetMPRecUp, m_iChangePetMPRecUp;

    float m_fStartCounterDefMatch, m_fChangeCounterDefMatch;
    float m_fStartMPRecoveryMatch, m_fChangeMPRecoveryMatch;
    int m_iElyosResistBuff, m_iChangeElyosResistBuff;
    bool m_bRoomUserInfo;

	std::map<int, int> m_mapListIndexAutoDivideString;

	KD3DWnd* m_pkGrade[MAX_NUM_GRADE];

};

DEFINE_WND_FACTORY( KGCStatusInfoPanel );
DECLARE_WND_FACTORY( KGCStatusInfoPanel );
DECLARE_WND_FACTORY_NAME( KGCStatusInfoPanel );

#endif // _GCINFOPANEL_H_
