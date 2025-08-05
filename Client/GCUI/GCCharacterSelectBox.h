#pragma once

class KGCCharacterHoverBox;
class KGCCharacterLockBox;



class KGCCharacterSelectBox : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharacterSelectBox );
    /// Default constructor
    KGCCharacterSelectBox( void );
    /// Default destructor
    virtual ~KGCCharacterSelectBox( void );
    /// Copy constructor
    KGCCharacterSelectBox( const KGCCharacterSelectBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterSelectBox& operator=( const KGCCharacterSelectBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );

    void OnClose();
	KGCCharacterHoverBox*	m_pkHoverBox;
	KGCCharacterLockBox*	m_pkLockBox;
	KD3DWnd*	m_pkClose;
	KD3DStatic*				m_pkStaticDesc;
	KD3DSizingBox*			m_pkBackDesc;
	KD3DWnd*	m_pkTitle;


	GAME_STATE				m_iState;
	bool					m_bNonStopMode;
	boost::function2<void,int,int>				m_SetFunc;
	boost::function0<void>					m_UpdateFunc;


	enum STATE_POSITION {
		STATE_POSITION_DOWN = -1,
		STATE_POSITION_NONE	= 0,
		STATE_POSITION_DEFAULT = 1
		
	};

	enum EXPERT_SKILL {
		EXPERT_SKILL_DEFAUT =0,
		EXPERT_SKILL_SKILL_OPEN_CHARACTER,
		EXPERT_SKILL_NOT_SAVE,
        EXPERT_SKILL_OPEN_CHARACTER_ALL,
        EXPERT_SKILL_NEED_LEVEL,
		EXPERT_SKILL_PROMOTION,
		EXPERT_SKILL_MISSION,
		EXPERT_SKILL_DEPOT_ADD_COMMON,
        EXPERT_SKILL_VIRTUAL_DEPOT, 
	};

    enum DISPLAY_TYPE {
        DISPLAY_MY_CHAR = 0,
        DISPLAY_ALL_CHAR
    };

    //  캐릭터 전용 던전 관련 캐릭터 Display -> 동작 관련해서 나중에 정리좀 하자 제발...
    enum PRIVATE_CAHR_TYPE
    {
        PRIVATE_CHAR_NONE   = 0,  //  기존과 동일
        PRIVATE_CHAR_NORMAL = 1,  //  일반 모드 ( 졸업하지 않은 캐릭터는 선택할 수 없다. ) 
        PRIVATE_CHAR_ONLY   = 2,  //  특정 캐릭터만 선택 가능( 캐릭터 전용 던전 )
    };

    void InitCharacterSelectBox(GAME_STATE state, boost::function2<void,int,int> a_Setfunc,
        boost::function0<void> a_UpdateFunc, D3DXVECTOR2 vPos,
        STATE_POSITION iStatePos=STATE_POSITION_DEFAULT,EXPERT_SKILL es =EXPERT_SKILL_DEFAUT, 
        DWORD dwParam1 =0 , DWORD dwParam2 =0, int iSelectIdx = -2,
        PRIVATE_CAHR_TYPE emCharState = PRIVATE_CHAR_NONE, int nPrivateCharType = GC_CHAR_INVALID );

	int GetSelectCharacter();
};

DEFINE_WND_FACTORY( KGCCharacterSelectBox );
DECLARE_WND_FACTORY( KGCCharacterSelectBox );
DECLARE_WND_FACTORY_NAME( KGCCharacterSelectBox );
