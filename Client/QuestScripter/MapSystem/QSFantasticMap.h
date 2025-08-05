#pragma once

#include "stdafx.h"
#include "KMap.h"

#define FOOTHOLDSIZE (0.05f)
#define FOOTHOLDTERM (0.02f)

#include "QSFootHold.h"
#include "QSFHVector.h"
//#include "../MapSystem/GCFantasticMap.h"
//#include <vector>

//#include "PlayerComp.h"
//#include "../MyD3D.h"

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

class KGC2DRenderer;

class KQSFantasticMap : public KMap<KQSFootHold>
{
    DeclareSingleton(KQSFantasticMap);
public:
    KQSFantasticMap(void);
    ~KQSFantasticMap(void);

    virtual void            RegisterLuabind();
    virtual void            Render();
    virtual void            FrameMove( float fElapsedTime );
    virtual void            Load();
    virtual void            UnLoad();
    virtual void            AddDungeonFootHold( KLuaManager& luaMgr );
	virtual void            AddDungeonFootHoldByValue( float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue ) ;
	virtual void            RemoveDungeonFootHold( float PosX, float PosY, void* pParent );
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
    virtual void            SetWorld( int iWidth, int iHeight );
    void                    DebugRender();
    void Draw_Billboard3( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, D3DXVECTOR3& pos3, D3DXVECTOR3& pos4, 
                          BYTE Power, BYTE R, BYTE G, BYTE B );

    D3DXMATRIX m_matCamera;
    FRect_Quest m_rtCamera;

    void SetCameraMatrix(D3DXMATRIX matCamera) { m_matCamera = matCamera; }
    void SetCameraRect(FRect_Quest rtCamera) { m_rtCamera = rtCamera; }
};

DefSingletonInline(KQSFantasticMap);
