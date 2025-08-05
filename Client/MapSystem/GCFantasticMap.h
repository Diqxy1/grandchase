#pragma once

#include "KMap.h"

#define FOOTHOLDSIZE (0.05f)
#define FOOTHOLDTERM (0.02f)

#include "GCFootHold.h"
#include "GCFHVector.h"
////#include <vector>
#include <KNCSingleton.h>
#include "PlayerComp.h"
#include "../MyD3D.h"

struct KNextBlockCheck
{
    bool bIsRight;
    float fX;
    float fY;
    float fWidth;

    KNextBlockCheck()
    {
        fX = 0;
        fY = 0;
        fWidth = 0;
    }
};

//#ifdef _DEBUG
//class KGC2DRenderer;
//#endif

class KGCO2Manager;
class CWave;

class KGCFantasticMap : public KMap<KGCFootHold>
{
    DeclareSingleton(KGCFantasticMap);
public:
    KGCFantasticMap(void);
    virtual ~KGCFantasticMap(void);

    virtual void            RegisterLuabind();
    virtual void            Render(){ Render(GC_LAYER_SKY); Render(GC_LAYER_BLOCK); }
    virtual void            Render( int iLayerNum_ );
    virtual void            FrameMove( float fElapsedTime );
    virtual void            Load();
    virtual void            UnLoad();
    virtual void            AddDungeonFootHold( KLuaManager& luaMgr );
    virtual void            AddDungeonFootHoldByValue( float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue );
    virtual void            AddDungeonFootHoldByValueBind( float PosX, float PosY, DWORD Prop );
    virtual void            RemoveDungeonFootHold( float PosX, float PosY, void* pParent = NULL);
    virtual void            LoadMatchMap( BYTE *Field_Information, char Num_Width, char Num_Height );
    virtual void            LoadBaseData( KLuaManager& luaMgr );
    virtual void            LoadFootHoldData( KLuaManager& luaMgr );
    virtual void            SetBlockWidth( int Width )
    {
        m_iFHWidth = Width;
        m_fWidth = static_cast<float>(m_iFHWidth * FOOTHOLDSIZE);
    }
    virtual void            SetBlockHeight( int Height )
    {
        m_iFHHeight = Height;
        m_fHeight = static_cast<float>(m_iFHHeight * FOOTHOLDSIZE);
    }
    inline bool            BoundCheckX( int iIndexX )
    {
        return !( iIndexX < 0 || iIndexX >= m_iFHWidth );
    }
    inline bool            BoundCheckY( int iIndexY )
    {
        return !( iIndexY < 0 || iIndexY >= m_iFHHeight );
    }

    bool                    AdjustUnitPosition( KGCContectInfo& kInfo_ );
    bool                    CheckPierceBlockXPos( float fBeforeX, OUT float& fAfterX, float fPosY );
    bool                    CheckPierceBlockYPos( float fPosX, float fBeforeY, OUT float& fAfterY );
    bool                    CrashCheck( float fX, float fY, D3DXVECTOR2& dxvRet = D3DXVECTOR2(0.f,-1.f), DWORD dwProperty = 0x11111111 );

    bool                    CrashCheck_Index( int i_X, int i_Y, D3DXVECTOR2& dxvRet = D3DXVECTOR2(0.f,-1.f), DWORD dwProperty = 0x11111111 );
    float                   IndexBetweenCheck( int StartIndex, int EndIndex, int YIndex, DWORD dwProp = foothold_type::FHP_JUST_FOOTHOLD );
    float                   IndexBetweenYCheck( int XIndex, int StartindexY, int EndindexY );
    //DWORD                   GetColorDWORD( BYTE r, BYTE g, BYTE b );
    //void                    SetColor( DWORD dwColor, int iKeepFrame = 0 );

    float                   GetRandomPosX(){ return randf() * m_fWidth; }
    float                   GetRandomPosY(){ return randf() * m_fHeight; }
    float                   GetHalfPosX() { return m_fWidth / 2; }
    float                   GetWorldHeight()    { return m_fHeight; }
    float                   GetWorldWidth()     { return m_fWidth; }

    D3DXVECTOR2             GetPositionNear( float fX, float fY );
    bool                    IsExistNextBlock( const KNextBlockCheck& kNextCheck );

    bool                    IsLastBottomPosition( float fX, float fY );
    bool                    GetSafePosition( float& fX, float& fY, bool bTagMotion );
    bool                    IsAvailableGoDown( float fX, float fY );
    bool                    IsAvailableGoSide( float fX, float fY );
    bool                    IsAvailableJumpUp( float fX, float fY );
	bool					IsDeadBlock( float fX, float fY );

    bool                    EnableMove( int iIndexX, int iIndexY );

    float                   FindShadowPos( float fX, float fY );

    virtual void            SetWorld( int iWidth, int iHeight );

    // X좌표에서 가장 낮은 블록의 위치를 돌려줍니다
    float                   GetLastBottomPosition( float fX, float fY );

//#ifdef _DEBUG
//public:
//    KGC2DRenderer*          GetDebugFH() { return m_pDebugFH; }
//
//protected:
//    KGC2DRenderer*          m_pDebugFH;
//#endif


    // 물 관련 함수들
    void Water_FrameMove( void );
    void Water_Render( void );
    bool IsWaterStage();
    void CreateWater();
    void SetWaterHeight( float fHeight );
    void ClearWater(void);

private:
    bool                            m_bNothingFootHold;
    std::vector< std::pair< int, int > > m_vecLastButtom;
    
    float                           m_fWaterHeight;
    GCDeviceTexture*                m_pTexWater;
    KSafeArray<SLVERTEX,4> V_Water;               ///< 맵 전경을 그릴 버텍스 버퍼
    void SetO2Region();
    CWave*                          m_pWave;
    KGCO2Manager*                   m_pkO2Manager;

public:
    KGCO2Manager* GetO2Manager(){ return m_pkO2Manager; }

    // 아지트 관련 
public:
    bool IsValidBlock( float fX, float fY, D3DXVECTOR2& dxvRet = D3DXVECTOR2(0.f,-1.f) );
    bool IsVaildBlockIndex( int iX, int iY, D3DXVECTOR2& dxvRet = D3DXVECTOR2(0.f,-1.f) );

public:
    D3DXVECTOR2 CheckNearFootHold( int iBlockType, float fX, float fY, int iOffsetX, int iOffsetY, bool bRight );

public:
    void SetLayerMoveSpeed( int iLayer, char *szPartName, float fSpeedX, float fSpeedY );
    float GetLayerMoveSpeedX( int iLayer, char *szPartName );
    float GetLayerMoveSpeedY( int iLayer, char *szPartName );

public:
    void RemoveDungeonFootHoldToBind( float PosX, float PosY );
};

DefSingletonInline(KGCFantasticMap);
