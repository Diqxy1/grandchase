#pragma once

#include <d3dx9.h>
#include "../GCCameraOrtho.h"
//#include "../GCDeviceManager2.h"

#define DIR_NUM (16)
#define TYPE_NUM (4)
#define QT_NUM_OF_ARROW                 4
#define QT_NUM_OF_NUMBER                10
#define QT_NUM_OF_STAGE_BACK            4
#define CONVERT_PIXEL_TO_TEX_SIZE(Size, TSize)  ( (float)Size/(float)TSize )
#define ARROW_IN_ZOOM (55)

class KGCQuestTile
{
public:
    /// Default constructor
    KGCQuestTile( void );
    /// Default destructor
    virtual ~KGCQuestTile( void );
    /// Copy constructor
    KGCQuestTile( const KGCQuestTile& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestTile& operator=( const KGCQuestTile& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum QT_STAGE_BACK { STAGE_DISABLE = -1, STAGE_ACTIVE, START, BOSS };
    enum QT_ARROW_TYPE { ARROW_RIGHT = 1, ARROW_BOTTOM = 2, ARROW_LEFT = 3, ARROW_TOP = 4 };

    //////////////////////////////////////////////////////////////////////////
    // 위치와 너비 설정용 함수들
    //////////////////////////////////////////////////////////////////////////
    D3DXVECTOR2     GetPos(void);                           // 위치 반환
    D3DXVECTOR2     GetWidthHeight(void);                   // 너비높이 반환
    float           GetWidth(void);                         // 너비 반환
    float           GetHeight(void);                        // 높이 반환
    void            SetPos(D3DXVECTOR2 vPos, D3DXVECTOR2 vScale);               // 위치를 설정한다.
    void            SetPos(float fX, float fY,float fWindowScaleX, float fWindowScaleY);
    void            SetWidthHeight(D3DXVECTOR2 vWH);        // 너비와 높이를 설정한다.
    void            SetWidth(float fWidth);                 // 너비를 설정한다.
    void            SetHeight(float fHeight);               // 높이를 설정한다.
    
    //////////////////////////////////////////////////////////////////////////
    // 화살표의 표시여부를 설정해주는 함수들
    //////////////////////////////////////////////////////////////////////////
    void SetArrow( int iDir );                              // 화살표를 방향을 설정한다.
    void SetArrowBoth( QT_ARROW_TYPE enDir, bool bBoth );   // enDir 방향에 화살표를 쌍방향 화살표로 설정
    void SetArrowBothAll( bool bBoth );
    void SetArrowIn( QT_ARROW_TYPE enDir, bool bShow );     // 이웃타일에서 내타일로 들어오는 화살표 설정
    void SetArrowInAll( bool bShow );
    void SetArrowOut( QT_ARROW_TYPE enDir, bool bShow );    // 내타일에서 이웃타일로 나가는 화살표 설정
    void SetArrowOutAll( bool bShow );
    void SetArrowColorType( QT_ARROW_TYPE enDir, bool bShow );    // 화살표 타입을 컬러로 설정한다.
    void SetArrowColorTypeAll( bool bShow );

    //////////////////////////////////////////////////////////////////////////
    // 줌인용 변수
    //////////////////////////////////////////////////////////////////////////
    float           m_fArrowZoom;                           // 화살표의 Zoom 값
    float           m_fTileZoom;                            // 가장기본적인 타일 전체의 Zoom 값
    float           m_fStageBGZoom;                         // 스테이지 배경 Zoom 값
    float           m_fNumberZoom;                          // 번호 Zoom 값

    //////////////////////////////////////////////////////////////////////////
    // 타일의 기본적인 설정을 하는 부분
    //////////////////////////////////////////////////////////////////////////
    void SetStage( int iStage );                            // 스테이지 번호를 설정한다.
    void SetStageBackType( int iType );                     // 스테이지 배경 종류를 설정한다.
    void SetBrightNum( bool bBrightNum );
    void SetCurStage(bool bCurStage) { m_bCurStage = bCurStage; }

    void OnCreate( void );
    void FrameMove_CurTile(void);
    void SetCharDirection( bool bRight );
    void Draw(void);

    FRect GetZoomRect(void);                                // Tile과 StageBackType Zoom을 동시에 적용한 Rect
    FRect GetTileZoomRect(void);                            // Tile의 줌을 적용한 Rect

protected:
    void Draw_Arrow_In( QT_ARROW_TYPE enDir );       // 들어오는 화살표 - 이웃타일에서 내쪽방향으로 화살표가 들어온 경우
    void Draw_Arrow_Out(QT_ARROW_TYPE enDir);        // 나가는 화살표   - 내타일에서 이웃타일쪽으로 화살표가 나가는 경우
    void Draw_Arrow_InOut( QT_ARROW_TYPE enDir );    // 내쪽으로 들어오는 화살표와 나가는 화살표를 그려준다. (위의 2함수를 조건에 따라 사용)

    void Draw_Arrow(void);                           // 화살표를 그린다.
    void Draw_StageBack(void);                       // 타일 배경 그림을 그린다.
    void Draw_Number_1(void);                        // 1자리 번호를 그린다.
    void Draw_Number_10(void);                       // 10자리 번호를 그린다.

    void GetArrowDrawRect( QT_ARROW_TYPE enDir, FRect &rtArrow );       // 화살표의 방향에 따라 뿌려줘야하는 범위를 리턴
    void GetTArrowInDrawRect( QT_ARROW_TYPE enDir, FRect &rtTArrow );    // 화살표의 방향에 따라 뿌려줘야하는 텍스쳐 범위 리턴
    void GetTArrowOutDrawRect( QT_ARROW_TYPE enDir, FRect &rtTArrow );   // 화살표의 방향에 따라 뿌려줘야하는 텍스쳐 범위 리턴

    float Percent(float fPercent, float fOriginal);         // Original 숫자의 fPercent(100) 만큼의 숫자를 리턴
    void ZoomWidth(float fPercent, FRect &rtRect);          // rtRect의 너비를 fPercent만큼 줄인다.
    void ZoomHeight(float fPercent, FRect &rtRect);         // rtRect의 높이를 fPercent만큼 줄인다.
    void ZoomWidthHeight(float fPercent, FRect &rtRect);    // rtRect의 너비와 높이를 fPercent만큼 줄인다.

protected:
    int                     m_iDirection;                   // 화살표 방향 플래그 0~15 까지 있다.
    KSafeArray<bool,QT_NUM_OF_ARROW> m_bDirBoth;    // 쌍방향 화살표의 사용여부
    KSafeArray<bool,QT_NUM_OF_ARROW> m_bArrowIn;    // 바깥으로 나가는 화살표인가? 안쪽으로 들어오는 화살표인가?
    KSafeArray<bool,QT_NUM_OF_ARROW> m_bArrowOut;   // 바깥으로 나가는 화살표인가? 안쪽으로 들어오는 화살표인가?
    KSafeArray<bool,QT_NUM_OF_ARROW> m_bArrowColorType;  // 화살표의 종류(회색인가 컬러인가?)
    bool                    m_bCurStage;                    // 현재 위치 표시 여부
    int                     m_iStageBackType;               // 타일 배경 타입
    int                     m_iStage;                       // 스테이지 번호
    bool                    m_bBrightNum;                   // 활성화된 번호를 사용한다.
    bool                    m_bCharDir;                     // 현재 스테이지 표시 캐릭터가 보는 방향 플래그
    bool                    m_bCurStageY_State;             // 현재 스테이지 표시 상하로 움직임을 제어하는 플래그
    int                     m_iCurStageY;                   // 현재 스테이지 표시가 상하로 움직일때 Y 값

    FRect                   m_rtRender;                     // 타일이 그려지는 기본 영역
    GCDeviceTexture         *m_pDungeon;                    // 던젼 미니맵용 텍스쳐
    GCDeviceTexture         *m_pML_Dungeon;                    // 다국어용 보스 미니맵용 텍스쳐

    // tu, tv 좌표
    KSafeArray<FRect,QT_NUM_OF_STAGE_BACK> m_rtStageBack;
    FRect                   m_rtCurStage;
	float					m_fWindowScaleX;
	float					m_fWindowScaleY;
    KSafeArray<KSafeArray<FRect,2>,2> m_rtArrow[QT_NUM_OF_ARROW];          // 화살표 전체                               - [화살표 수][Both][흑백]
    KSafeArray<KSafeArray<FRect,2>,2> m_rtTArrowOut[QT_NUM_OF_ARROW];      // 내타일에서 이웃타일로 나가는 화살표       - [화살표 수][Both][흑백]
    KSafeArray<KSafeArray<FRect,2>,2> m_rtTArrowIn[QT_NUM_OF_ARROW];       // 이웃타일에서 내타일로 들어오는 화살표     - [화살표 수][Both][흑백]
    KSafeArray<KSafeArray<FRect,2>,QT_NUM_OF_NUMBER> m_rtNumber;        // right down left, top .. 뒷자리 배열은 Bright와 Gray 숫자 2종류가 있기 때문.

};
