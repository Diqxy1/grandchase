#pragma once

struct MARK_ING_ARROW_ANI 
{
    float       fSpeed;
    float       fAniTimeCnt;
    KD3DWnd*    pWnd;
};

class KGCCharacterHoverBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharacterHoverBox );
    /// Default constructor
    KGCCharacterHoverBox( void );
    /// Default destructor
    virtual ~KGCCharacterHoverBox( void );
    /// Copy constructor
    KGCCharacterHoverBox( const KGCCharacterHoverBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterHoverBox& operator=( const KGCCharacterHoverBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

	D3DXVECTOR2 InitBox(boost::function2<void,int,int> a_Setfunc, boost::function0<void> a_Updatefunc, int es = 0,
                        DWORD dwParam1=0, DWORD dwParam2=0, int iSelectCharIdx = -2, 
                        int nPrivateCharState = 0, int nPrivateCharType = -1);
	void FrameMoveInEnabledState( void );
	bool SetCharNum_KeyDown(int key, int baseKey=-100);
    bool IsPromotionCharacter(int es, int iItemID, int iBasePromotionLevel, const SCharInfo& cCharInfo_ );
    void InitCharPrivate( int iCharType, int iCount, D3DXVECTOR4 vMovePos, int nPrivateCharState, int nPrivateCharType );
    void InitCharSeal( int iCharType, int iCount, D3DXVECTOR4 vMovePos );
    void MarkArrowAnimation();

    std::map<int,KD3DWnd*>	m_mapSelectChar;
	KD3DSizingBox*			m_pkBack;
	boost::function2<void,int,int>				m_SetFunc;
	boost::function0<void>				m_UpdateFunc;

	int	m_iState;

	int						m_iCharKeySelect;
    int						m_iSelectCharIdx;
    int m_iSwitch;
    int m_iAlphaCnt;
    int m_iInputkeyDelayTime;
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkMarkMissionNew;
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkMarkMissionComplate;
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkMarkSeal;
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkMarkIngArrow;

    std::map<int, MARK_ING_ARROW_ANI>   m_mapArrowAniInfo;
};

DEFINE_WND_FACTORY( KGCCharacterHoverBox );
DECLARE_WND_FACTORY( KGCCharacterHoverBox );
DECLARE_WND_FACTORY_NAME( KGCCharacterHoverBox );
