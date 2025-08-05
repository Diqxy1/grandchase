#ifndef _GCKGCDOTAMINIMAP_H_
#define _GCKGCDOTAMINIMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ZONE 10

enum
{
    MINIMAP_1,
    NUM_MIANIMAP,
};

enum MiniMap_1
{
    ZONE_1,     // 세르딘 StartZone
    ZONE_2,     // 세르딘 CoreZone
    ZONE_3,     // 베틀존1
    ZONE_4,     // 베틀존2
    ZONE_5,     // 카나반 CoreZone
    ZONE_6,     // 카나반 StartZone
    MAX_MAP1_ZONE,
};

class KGCDotaMiniMap : public KD3DDialog, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCDotaMiniMap );
    /// Default constructor
    KGCDotaMiniMap( void );
    /// Default destructor
    virtual ~KGCDotaMiniMap( void );
    /// Copy constructor
    KGCDotaMiniMap( const KGCDotaMiniMap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaMiniMap& operator=( const KGCDotaMiniMap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void UpdateMyMark();

private:
    int                     m_iMaxZone;
    int                     m_iAlpha;
    bool                    m_bAlphaSwitch;
    bool                    m_bAniSwitch;
    float                   m_fAniPos;
    KD3DWnd                 *m_pkZoneWnd[MAX_ZONE];
    KD3DWnd                 *m_pkMyMark;
    KD3DWnd                 *m_pkWndCore1;
    KD3DWnd                 *m_pkWndCore2;
};
DEFINE_WND_FACTORY( KGCDotaMiniMap );
DECLARE_WND_FACTORY( KGCDotaMiniMap );
DECLARE_WND_FACTORY_NAME( KGCDotaMiniMap );



#endif  // _GCKGCDOTAMINIMAP_H_

#ifndef _GCKGCDOTATILEMAP_H_
#define _GCKGCDOTATILEMAP_H_

class KGCDotaTileMap : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaTileMap );
    /// Default constructor
    KGCDotaTileMap( void );
    /// Default destructor
    virtual ~KGCDotaTileMap( void );
    /// Copy constructor
    KGCDotaTileMap( const KGCDotaTileMap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaTileMap& operator=( const KGCDotaTileMap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    void SetTextureUV();
    void RenderUI( int &iUIIndex, int iTeam );
    void RenderTexture( GCDeviceTexture* pTex, D3DXVECTOR4 &rectWnd, D3DXVECTOR4 &rectUV );

public:
    void SortRanking();
    void UpdateUI( int &iUIIndex, int iTeam );
    void UpdateFinishUI( int &iUIIndex );
    void UpdateUserRanking();
    void SetUpdateCool(int iFrame) { m_iUpdateCoolTime = m_iNowCoolTime = iFrame; }

private:
    enum
    {
        NUMBER_RECT_LOCAL,
        NUMBER_RECT_DIFF,
        MAX_NUMBER_RECT,
    };

    KD3DSizingBox         *m_pkBack;
    KD3DSizingBox         *m_pkBackList;
    KD3DSizingBox         *m_pkMybar;
    KD3DStatic            *m_pkStatickeytab;
    KD3DStatic            *m_pkStaticNickName[MAX_PLAYER_NUM];
    KD3DStatic            *m_pkStaticPos[MAX_PLAYER_NUM];
    KD3DStatic            *m_pkStaticKillCnt[MAX_PLAYER_NUM];
    KD3DStatic            *m_pkStaticDeathCnt[MAX_PLAYER_NUM];
    KD3DWnd               *m_pkWndRanking[MAX_PLAYER_NUM];
    KD3DWnd               *m_pkWndkeytab;

private:
    int                   m_iNowCoolTime;
    int                   m_iUpdateCoolTime;
    DWORD                 m_dwBackOriHeight;
    float                 m_fTabMarkOriPosY;
    std::list<pair<int, int>> m_listRanking;    // PlyaerIdx, Ranking
    std::vector<std::wstring> m_vecPosName;
private:
    GCDeviceTexture*      m_pImgNumber;
    GCDeviceTexture*      m_pImgTeamMark;
    D3DXVECTOR4           m_v4RectUVTeam[TEAM_NUM];
    D3DXVECTOR4           m_v4RectUVNumber[MAX_NUMBER_RECT][MAX_PLAYER_NUM];
    KGCDotaMiniMap*       m_pkMiniMap[NUM_MIANIMAP];

    std::vector<pair<std::wstring, pair<int, int>>> vecTemp;
};

DEFINE_WND_FACTORY( KGCDotaTileMap );
DECLARE_WND_FACTORY( KGCDotaTileMap );
DECLARE_WND_FACTORY_NAME( KGCDotaTileMap );

#endif  // _GCKGCDOTATILEMAP_H_