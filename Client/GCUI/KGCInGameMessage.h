/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2013년 06월 30일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 게임 내 메시지박스( DotaMessage개편 )

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once
#include "GCInGameMsgItemWnd.h"

enum
{
    INGAME_MESSAGE_IMG_0 = 0,      // 하르페
    INGAME_MESSAGE_IMG_1,          // 메리
    INGAME_MESSAGE_IMG_2,          // 야시장
    INGAME_MESSAGE_IMG_3,          // 레이드 : 발리스타
    INGAME_MESSAGE_IMG_4,          // 레이드 : 억제기
    INGAME_MESSAGE_IMG_5,          // 레이드 : 투석기
    INGAME_MESSAGE_IMG_6,          // 레이드 : 차단기
    INGAME_MESSAGE_IMG_7,          // 차원의문 
    MAX_INGAME_MESSAGE_IMG,
};

enum
{
    STR_LOADER_DEFAULT = 0,         // 기본 String로더( g_pkStrLoader )
    STR_LOADER_SUBJECT,             // 달성과제 String로더( SiKGCSubjectManager()->GetStringLoader() )
    MAX_STR_LOADER,
};

struct KGCInGameMessageItem
{
    DWORD dwItemID;
    int iGrade;
    int iPeriod;
    int iCount;
    KGCInGameMessageItem()
        : dwItemID( 0 )
        , iGrade( 0 )
        , iPeriod( 0 )
        , iCount( 0 ) {}
};

struct KInGameMessageAttribute
{
    int iStringID;
    int iFontSize;
    int iFontAlign;
    int iSelectImg;
    int iSelectStrLoader;
    int iSkillID;
    bool bIsAlphaAnim;
    bool bIsOutline;

    int iShowFrame;

    // String에 @로 들어갈 Value값
    int iValue1;
    WCHAR* strValue1;
    WCHAR* strValue2;

    // StringID를 사용하지 않고, 즉시시전으로 출력할 내용
    WCHAR* strDirectString;

    // 아이템정보
    std::vector<KGCInGameMessageItem>  m_vecItemList;

    // 백그라운드 크기
    float fBackWidth;
    float fBackHeight;

    // Y축 출력위치
    float fShowPosY;

    KInGameMessageAttribute()
    {
        Init();
    }

    void Init()
    {
        iStringID = -1;
        iFontSize = SiKGCMultipleLanguages()->GetDefualtFontSize();
        iFontAlign = DT_LEFT;
        iSelectImg = 0;
        iSelectStrLoader = 0;
        iSkillID = -1;
        bIsAlphaAnim = true;
        bIsOutline = true;
        iValue1 = INT_MAX;
        strValue1 = NULL;
        strValue2 = NULL;
        strDirectString = NULL;
        m_vecItemList.clear();
        fBackWidth = 727.0f;
        fBackHeight = 90.0f;
        fShowPosY = 200.0f;
    }

    void SetStrValue1( WCHAR *strValue )    {   strValue1 = strValue;   }
    void SetStrValue2( WCHAR *strValue )    {   strValue2 = strValue;   }
    void SetDirectStr( WCHAR *strValue )    {   strDirectString = strValue; }
};

class KD3DSizingBox;
class KGCInGameMessage : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCInGameMessage );
    /// Default constructor
    KGCInGameMessage( void );
    /// Default destructor
    virtual ~KGCInGameMessage( void );
    /// Copy constructor
    KGCInGameMessage( const KGCInGameMessage& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInGameMessage& operator=( const KGCInGameMessage& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void RegisterLuaBind();

public:
    void SetMessage( KInGameMessageAttribute *pAttribute );

private:
    void SetAlphaAnim( KInGameMessageAttribute *pAttribute );
    void SetDesc( KInGameMessageAttribute *pAttribute );
    void SetImg( KInGameMessageAttribute *pAttribute );
    void SetWndArrangement( KInGameMessageAttribute *pAttribute );
    void SetItem( KInGameMessageAttribute *pAttribute );
    void SetSkill( KInGameMessageAttribute *pAttribute );
    void DestroyItemBox();
    void DestroySkill();

public:
    bool IsStringID( int iStringID );
    void SetStringID( int iStringID );
    int GetStringID();

public:
    void SetShowFrmae( int iFrame );
    int GetShowFrmae();

public:
    bool IsRender();
    int IsRenderToString( int iStringID );

private:
    int                   m_iAlpha;
    int                   m_iStringID;
    int                   m_iShowFrame;
    int                   m_iInShowFrame;
    int                   m_iOutShowFrame;
    float                 m_fAlphaChanger;
    int                   m_iSelectImg;
    bool                  m_bIsAlphaAnim;

private:
    KGCInGameMsgItemWnd   *m_pkSkillWnd;
    std::vector< KGCInGameMsgItemWnd* > m_vecItemWnd;

private:
    KD3DSizingBox         *m_pkBack;
    KD3DSizingBox         *m_pkBack_ItemList;
    KD3DSizingBox         *m_pkBack_Skill;
    KD3DStatic            *m_pkStaticDesc;
    KSafeArray<KD3DWnd*,MAX_INGAME_MESSAGE_IMG>   m_apkImg;
};

DEFINE_WND_FACTORY( KGCInGameMessage );
DECLARE_WND_FACTORY( KGCInGameMessage );
DECLARE_WND_FACTORY_NAME( KGCInGameMessage );