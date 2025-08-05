#pragma once

#include "stdafx.h"
//#include <vector>
//#include "../GCLuaManager/KLuaManager.h"
//#include "../GCLuaManager/KLuabinder.h"
//#include "../GCDeviceLib/GCDeviceShader.h"
//#include "../GCDeviceLib/GCDeviceManager.h"


// [8/23/2007 breadceo] for Map Rendering

class IGCRenderObj
{
public:
    virtual int GetLayer() const = 0;
    virtual bool IsAlpha() = 0;
    virtual HRESULT Render() = 0;

    virtual EGCRENDER_STATE GetRenderPass() const { return EGCRENDER_STATE_INVALID; }
    virtual GCDeviceTexture* GetRenderTexture(){ return NULL; }
};

extern LPDIRECT3DDEVICE9       g_pd3dDevice;

#define LAST_LAYER (19)
#define MAX_RENDERING_PARTS_PER_LAYER (256)
#define MAX_PART_VERTEX (MAX_RENDERING_PARTS_PER_LAYER * 4 + 1)
#define MAX_PART_INDEX  (MAX_RENDERING_PARTS_PER_LAYER * 6 + 1)

/************************************************************************/
/*	CLayer 가 해야하는 일들

1. LayerPart를 가지고 관리. (O)
2. Player의 움직임에 따라 입체감있게 스크롤 시켜 주기.
3. Layer Whole Size. (O)
4. LayerPart들 Z-Order 관리. (O)
5. $$$ 이동 X -> "그리는 위치를 조정" 해서 입체감을 주자. $$$

$$$식$$$
나 Scroll / 기준전체 Scroll * 기준점의 위치 ( 나 Scroll : 현재 Layer 크기 - 화면 크기 , 기준전체 Scroll : 기준 Layer 크기 - 화면에 보이는 크기 * Zoom )
90 / 100 * 50
/************************************************************************/

template <class T>
class CRenderPartPair : public std::pair< std::string, std::vector<T*> >
{
public:
    CRenderPartPair(void) : std::pair< std::string, std::vector<T*> >() {}
    ~CRenderPartPair(void) { second.clear(); }

    void push_back( T* second_ )
    {
        second.push_back( second_ );
    }
};

// [8/9/2007 breadceo] 구조체에 담는다.
struct VBIBIndexData
{
    DWORD dwVertexCount;
    DWORD dwIndexCount;
    WORD  wVertexIndex;
    DWORD dwRenderSize;
    DWORD dwSumRenderSize;
    DWORD dwVBIndex;
    DWORD dwIBIndex;

    VBIBIndexData()
    {
        dwVertexCount = 0;
        dwIndexCount = 0;
        wVertexIndex = 0;
        dwRenderSize = 0;
        dwSumRenderSize = 0;
        dwVBIndex = 0;
        dwIBIndex = 0;
    }
};

// T0 : LayerPart, T1 Manager
template <class T0, class T1>
class KLayer : public IGCRenderObj
{
private:
    explicit KLayer(void) {}
public:
    KLayer( T1* pManager );
    virtual ~KLayer(void);

    // 구현해 줘야 하는 것
    virtual void Process() = 0;                         // 스크롤이나 기타 등등을 처리
    virtual void FrameMove( float fElapsedTime ) = 0;   // FrameMove

    virtual void SetZoffset();
    virtual void SetBufferType1();

    // IGCRenderObj
    virtual HRESULT Render();
    virtual int GetLayer() const { return m_iLayerNum; }
    virtual bool IsAlpha(){ return false; }
    virtual EGCRENDER_STATE GetRenderPass() const
    {
        return EGCRENDER_STATE_FAR_MAP_LINEAR; 
    }

    virtual void SetLayerXSize( float XSize ) { m_fLayerSizeX = XSize; }
    virtual void SetLayerYSize( float YSize ) { m_fLayerSizeY = YSize; }
    virtual void SetLayerNum( int LayerNum ) { m_iLayerNum = LayerNum; }

    virtual float GetLayerXSize() { return m_fLayerSizeX; }
    virtual float GetLayerYSize() { return m_fLayerSizeY; }

    virtual void AddParts( KLuaManager& luaMgr ) = 0;

    virtual void init();
    virtual void clear();

    virtual void SetShow( bool Show ) { m_bShow = Show; }
    virtual bool GetShow() { return m_bShow; }

    virtual void AddPartsErrFunc( std::string partName_ );

    virtual void SetVBIBIndexData( IN const SLVERTEX* pScrolledVertex, OUT VBIBIndexData& kIndexData, OUT SLVERTEX* pVertex, OUT WORD* pIndex );

    virtual void CreateBuffer() = 0;

protected:
    float                   m_fLayerSizeX;
    float                   m_fLayerSizeY;
    int                     m_iLayerNum;
    bool                    m_bShow;

    // 텍스쳐 별로 소팅해서 그리기 위해
    std::vector< CRenderPartPair<T0> >    m_vecRenderPart;
    // VertexBuffer
    GCDeviceVertexBuffer*   m_pVBPartsRender;
    // IndexBuffer
    GCDeviceIndexBuffer*    m_pIBPartsRender;
    // DP Info
    struct DPInfo
    {
        DWORD               m_dwVBIndex;
        DWORD               m_dwIBIndex;
        DWORD               m_dwRenderSize;
        GCDeviceTexture*    m_pTexture;
    };
    std::vector<DPInfo>     m_vecDPInfo;
    std::vector<T0*>        m_vecPart;

    T1*                     m_pLayerManager;
};

#include "KLayer.inl"
