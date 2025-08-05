#pragma once

//#include "../uifx/D3DSizingBox.h"
#include "GCQuestTile.h"
//#include <map>
//#include <vector>

#define TILE_ZOOM               3.0F
#define MAX_TILE_ZOOM           30.0F
#define MIN_TILE_ZOOM           -30.0F
#define TILE_MOVE_OFFSET        10
#define MINIMAP_BOUND_INTERVAL  15
#define MINIMAP_BLANK_INTERVAL  15
#define GC_TILEMAP_WIDTH        GC_SCREEN_WIDTH
#define GC_TILEMAP_HEIGHT       GC_SCREEN_HEIGHT

class KD3DStatic;
class KGCMissionProgressBox;
class KGCPartyRankInfo;

class KGCQuestTileMap : public KD3DSizingBox
{
public:
    DECLARE_CLASSNAME( KGCQuestTileMap );
    /// Default constructor
    KGCQuestTileMap( void );
    /// Default destructor
    virtual ~KGCQuestTileMap( void );
    /// Copy constructor
    KGCQuestTileMap( const KGCQuestTileMap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestTileMap& operator=( const KGCQuestTileMap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    // 타일의 정보를 저장하고 있는 구조체
    struct stTileInfo
    {
        stTileInfo()
        {
            iD_Path                     = 0;
            iFlag                       = 0;
            iStage                      = 0;
            iRow                        = 0;
            iCol                        = 0;
            iDrawRow                    = 0;
            iDrawCol                    = 0;

            for(int i=0;i<4;++i)
                bArrowFromNeighbor[i]   = false;

            for(int i=0;i<4;++i)
                bArrowColor[i] = false;
        }

        int             iD_Path;
        int             iFlag;
        int             iStage;
        int             iRow;
        int             iCol;
        int             iDrawRow;
        int             iDrawCol;
        KSafeArray<bool,4> bArrowFromNeighbor;
        KSafeArray<bool,4> bArrowColor;
    };

    RECT                     m_rtMaxBound;
    std::vector<stTileInfo>  m_vtMinimapInfo;
    void InitMinimap(void);

    D3DVIEWPORT9        m_viewport;

	void PostChildDraw( void );
    bool IsSideArrowToHere(int iDirection, int iRow, int iCol);
    void FrameMove_ActiveStage(void);
    void SetCheckStageHistory(int iStage);
    void KeyProc(void);

protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );

private:
    bool                m_bCharDir;     // 현재 위치에 표시하는 캐릭터가 보는 방향 <true, Right>, <false, left>이면
    int                 m_iBossCol;     // 보스 스테이지의 Col, 현재 위치에 표시하는 캐릭터의 보는 방향 결정을 위한 값
    bool                m_bTrace;
    bool                m_bEnable;
    int                 m_iOffsetX;
    int                 m_iOffsetY;
    int                 m_iPlusOffX;
    int                 m_iPlusOffY;
    KGCQuestTile        m_Tile;

private:
    KD3DWnd*            m_pkTabImage;
    KD3DStatic*         m_pkMinimapDesc;	

	KGCMissionProgressBox* m_pkMissionBox;
    KGCPartyRankInfo*   m_pPartyRankInfoBox;
public:
    void UpdateDescPos();
    void SetPlayerRankInfo();
};


DEFINE_WND_FACTORY( KGCQuestTileMap );

DECLARE_WND_FACTORY( KGCQuestTileMap );
DECLARE_WND_FACTORY_NAME( KGCQuestTileMap );
