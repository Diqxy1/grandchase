#ifndef _KGCCATEGORYBOX_H_
#define _KGCCATEGORYBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCCategoryBox: public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCategoryBox );
    /// Default constructor
    KGCCategoryBox( void );
    /// Default destructor
    virtual ~KGCCategoryBox( void );
    /// Copy constructor
    KGCCategoryBox( const KGCCategoryBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCategoryBox& operator=( const KGCCategoryBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EGAMEMODE_CATEGORY
    {
        ECATEGORY_ALL = 0,          // 전체모드
        ECATEGORY_MATCH,            // 대전
        ECATEGORY_TAGMATCH,         // 태그매치
        ECATEGORY_MONSTERCRUSADER,  // 몬스터원정대 (보스러쉬)
        ECATEGORY_MONSTERHUNT,      // 몬스터대전
        ECATEGORY_DEATHMATCH,       // 데스매치
        ECATEGORY_GUILD,            // 유저가 속한 길드방
		ECATEGORY_ANGLE_EGG,        // 천사 알깨기      
        ECATEGORY_DOTA,             // 점령전
        ECATEGORY_FATAL_DEATHMATCH, // 페이탈 데스매치

        NUM_CATEGORY,
    };

    enum EDUNGEON_LEVEL
    {
        LEVEL_1 = 0,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4,
        LEVEL_5,

        NUM_LEVEL,
    };

public:
    virtual void FrameMoveInEnabledState();

    void UpdateCategory( char cCategory );
    void UpdateLevel( int nLevel, bool bClear = false );
    std::wstring GetCategoryName();
    static EGCGameModeCategory GetGMCfromEnum( KGCRoomListManager::EFILTER_CATEGORY eGMC );

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );

    KSafeArray<KD3DWnd*, KGCRoomListManager::NUM_CATEGORY > m_apkCategoryList;
    KSafeArray<KD3DWnd*, NUM_LEVEL > m_apkLevelList;

    // 명계열차 임시난이도 작업
    KD3DWnd*    m_apkEventHellMode;

    int m_iCategoryNameID;

    KD3DStatic*     m_pkLevelText;
    KD3DSizingBox*  m_pkLevelBack;
    KD3DWnd*        m_pkLevelImg;
    KGCCheckBox*    m_pkClearCheck;
};

DEFINE_WND_FACTORY( KGCCategoryBox );
DECLARE_WND_FACTORY( KGCCategoryBox );
DECLARE_WND_FACTORY_NAME( KGCCategoryBox );

#endif //_KGCCATEGORYBOX_H_
