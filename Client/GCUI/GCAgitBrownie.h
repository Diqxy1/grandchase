#pragma once

enum 
{ 
    PARTICLE_EYE0   = 0, 
    PARTICLE_EYE1   = 1, 
    PARTICLE_MOUSE  = 2, 
    PARTICLE_BROW   = 3, 
    PARTICLE_CHIC   = 4,

    NUM_PARTICLE    
};

enum 
{
    FACE_NORMAL     = 0,
    FACE_HAPPY      = 1,
    FACE_SAD0       = 2,
    FACE_SAD1       = 3,
    FACE_SURPRISE   = 4,
    FACE_SHY0       = 5,
    FACE_SHY1       = 6,
    FACE_UGLY0      = 7, 
    FACE_UGLY1      = 8,

    NUM_FACE

};

enum 
{ 
    GREETING0 = 0, 
    GREETING1 = 1, 
    GREETING2 = 2, 
    GREETING3 = 3, 
    GREETING4 = 4,

    NUM_GREETING
};

struct KBrownieParticleInfo
{
    int         m_iPosIndex;
    std::string m_strSquence;
    D3DXVECTOR2 m_dxvPos;
};

struct KBrownieFace
{
    int m_iFaceIndex;
    std::vector< KBrownieParticleInfo > m_vecParticle;
};

struct KBrownieGreetingInfo
{
    int m_iNormalFaceIndex;
    int m_iNormalStringIndex;
    std::map< int, std::vector< std::pair< int, int > > > m_mapGreetingList;    // 대화 번호 < 사용 표정 인덱스, 스트링 인덱스  >
};

struct KAgitQuestionInfo
{
    int m_iQuestionString;
    int m_iAnswerString;
    int m_iFaceIndex;
};

class KGCAgitBrownie : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { BTN_GREETING = 0, BTN_OPTION = 1, BTN_MOVE =2, BTN_QUESTION = 3, BTN_NEXT = 4, BTN_CLOSE = 5, NUM_BTN = 6 };
    enum { STATIC_GREETING = 0, STATIC_OPTION = 1, STATIC_MOVE = 2, STATIC_QUESTION = 3, STATIC_MENT = 4, NUM_STATIC = 5 };
    enum { QUESTION0 = 0, QUESTION1 = 1, QUESTION2 = 2, QUESTION3 = 3, NUM_QUESTION = 4 };

public:
    DECLARE_CLASSNAME( KGCAgitBrownie );
    /// Default constructor
    KGCAgitBrownie( void );
    /// Default destructor
    virtual ~KGCAgitBrownie( void );
    /// Copy constructor
    KGCAgitBrownie( const KGCAgitBrownie& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitBrownie& operator=( const KGCAgitBrownie& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KSafeArray< KD3DWnd*, NUM_BTN >         m_apkBtn;
    KSafeArray< KD3DStatic*, NUM_STATIC >   m_apkStatic;
    KD3DSizingBox*                          m_pkActiveFrame;
    KD3DWnd*                                m_pkImgBrownie;
    
    int     m_iCurSelBtn;
    int     m_iGreetingType;
    
    bool    m_bGreeting;
    int     m_iGreetingString;
    int     m_iGreetingFace;
    int     m_iCurFace;

    std::vector< KBrownieFace >             m_vecFaceList;              // 표정 정보들 저장
    KBrownieGreetingInfo                    m_kBrownieGreeting;         // 도우미 대화 정보 저장 - 추후 늘리시오 
    KSafeArray< CParticleEventSeqPTR, NUM_PARTICLE > m_apParticle;  // 파티클 관리용 


    // 도움말 관련
    KSafeArray< KD3DStatic*, NUM_QUESTION > m_apkQuestion;
    KD3DSizingBox*  m_pkQuestionFrame;
    KD3DSizingBox*  m_pkSelectQuestion;

    std::vector< KAgitQuestionInfo >    m_vecQuestionList;      // 도움말 정보 

public:
    void InitUI( void );
    void ClearAllParticle( void );
    bool LoadBrowniInfo( void );

private:
    void SetGreetingType( void );
    void OnMenuBtn( int iIndex_ );
    void OnNextBtn( void );
    void OnCloseBtn( void );

    void OnGreeting( void );
    void OnOption( void );
    void OnMove( void );
    void OnQuestion( void );

    // 파티클 관련 
    void AddParticle( std::string& strSquence_, float fPosX_, float fPosY_, int iParticleIndex_ );
    void DeleteParticle( int iParticleIndex_ );
    void SetFace( int iFaceIndex_ );

    // 도움말 관련 
    void SetQuestionInfo( void );
    void OnSelectQuestion( int iIndex_ );
    void ToggleActiveFrame( int iIndex_ );
    void ToggleQuestion( bool bShow_ );
};

DEFINE_WND_FACTORY( KGCAgitBrownie );
DECLARE_WND_FACTORY( KGCAgitBrownie );
DECLARE_WND_FACTORY_NAME( KGCAgitBrownie );
