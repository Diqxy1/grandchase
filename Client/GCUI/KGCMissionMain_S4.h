#ifndef _GCKGCMISSIONMAIN_S4_H_
#define _GCKGCMISSIONMAIN_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCMissionCharSelector_S4.h"
#include "KGCMissionListBar_S4.h"
#include "KGCMissionSub_S4.h"

#define NUM_MISSION_LIST 10
#define ANI_KNIGHT_SPEED 0.1f
#define ANI_MENT_SPEED 0.08f

class KGCMoveModel;

class KGCMissionMain_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionMain_S4 );
    /// Default constructor
    KGCMissionMain_S4( void );
    /// Default destructor
    virtual ~KGCMissionMain_S4( void );
    /// Copy constructor
    KGCMissionMain_S4( const KGCMissionMain_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionMain_S4& operator=( const KGCMissionMain_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    enum
    {
        TAB_ALL, 
        TAB_NORMAL, 
        TAB_CHARACTER, 
        TAB_EVENT,
        TAB_EPIC,
        TAB_NUM, 
    };
    enum
    {
        TAB_STATE_DESABLE,
        TAB_STATE_ENABLE, 
        TAB_STATE_NUM,
    };
    enum
    {
        ANI_START,
        ANI_END,
    };
    enum
    {
        TITLE_LEVEL,
        TITLE_NAME,
        TITLE_CHAIN,
        TITLE_PROGRASS,
        NUM_TITLE,
    };

private:// 컨트롤 관련
    KD3DWnd                         *m_pkTab[ TAB_NUM ][ TAB_STATE_NUM ];
    KD3DWnd                         *m_pkBtn_Prograss;
    KD3DWnd                         *m_pkBtn_Chain;
    KD3DWnd                         *m_pkBack_NotMission_1;
    KD3DWnd                         *m_pkBack_NotMission_2;
    KD3DStatic                      *m_pkTabCnt[ TAB_NUM ];
    KD3DStatic                      *m_pkStaticTitle[ TAB_NUM ];
    KD3DStatic                      *m_pkStatic_Knight;
    KD3DSizingBox                   *m_pkBack_Knight;
	KGCMoveModel                    *m_pkBack_Model;
    KD3DScrollbar                   *m_pkScrollbar;
    KGCMissionCharSelector_S4       *m_pkCharSelector;
    KGCMissionListBar_S4            *m_pkMissionList[ NUM_MISSION_LIST ];

private:// 정렬 관련
    std::vector< DWORD >            m_vecSorting;
    std::vector< DWORD >            m_vecMissionUID;
    std::vector< DWORD >            m_vecCharMissionUID;

private:// 탭과 리스트바 인덱스 관련
    int                             m_iTabIdx;                  // 전체 : 0, 일반 : 1, 캐릭터 : 2, 이벤트 : 3
    int                             m_iScrollIdx;               // 스크롤바 높이
    int                             m_iListBarIdx;              // 선택한 리스트바 인덱스
    int                             m_iMissiontUIDIdx;          // 선택한 미션 인덱스( m_iScrollIdx + m_iListBarIdx )
    int                             m_iTabCnt[ TAB_NUM ];       // 탭별 미션갯수

private:// 기사단장 애니메이션 관련
    float                           m_fMentRadio;
    float                           m_fModelRadio;
    D3DXVECTOR2                     m_iMent_Pos[2];
    D3DXVECTOR2                     m_iSayBox_Pos[2];
    D3DXVECTOR2                     m_iKnight_Pos[2];

private:// 기타
    bool                            m_bPrograssMode;

private:
    void OnEnterCursor();
    void OnClickTab(int iTabIdx);
    void OnClickListBar(int iListIdx);
    void OnClickPrograssMode( bool bSwitch );

private:
    D3DXVECTOR2 GetTabTextPos( int iTab, bool bEnable );
    int GetTabMission( int iTabIdx, bool bPushUID = false );
    void GetLevelColor( int iMissionLevel, OUT DWORD &dwFontColor, OUT DWORD &dwOutlineColor );
    void SetPrograssBarInfo( int iListBarIdx, int iUIDIdx );
    void SetPrograssModeToggle();
    bool RefreshMissionList();
    void RefreshScrollBar(UINT iElementCnt);
    void AutoMissionSelect( bool bAutoSelect );
    void SetListBarColor( bool bScroll = false );
    void Merge(std::vector< DWORD > *pData, int iLeft, int iMiddle, int iRight);
    void MergeSorting(std::vector< DWORD > *pData, int iLeft, int iRight);

private:
    template<typename T>
    T LinearInterpolation(T &t1, T &t2, float &fRatio);
    void SetInterpolationRadio(float &fRadio, float fSpeed = 0.1f);

public:
    void SetStringMent( int iStringID );
    void SetStringMent( std::wstring strMent );
    void RefreshMainMission( int iCharIdx, bool bAutoSelect = false );
    void RefreshUI();
};

DEFINE_WND_FACTORY( KGCMissionMain_S4 );
DECLARE_WND_FACTORY( KGCMissionMain_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionMain_S4 );

#endif  // _GCKGCMISSIONMAIN_S4_H_