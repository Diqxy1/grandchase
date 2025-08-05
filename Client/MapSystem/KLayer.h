#pragma once

//#include <vector>
//#include "../GCLuaManager/KLuaManager.h"
//#include "../GCLuaManager/KLuabinder.h"
#include "../GCDeviceLib/GCDeviceShader.h"
#include "../GCDeviceLib/GCDeviceManager.h"
//#include <math.h>
//#include <algorithm>
//#include <sstream>
#include <d3d9types.h>

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
/*	CLayer �� �ؾ��ϴ� �ϵ�

1. LayerPart�� ������ ����. (O)
2. Player�� �����ӿ� ���� ��ü���ְ� ��ũ�� ���� �ֱ�.
3. Layer Whole Size. (O)
4. LayerPart�� Z-Order ����. (O)
5. $$$ �̵� X -> "�׸��� ��ġ�� ����" �ؼ� ��ü���� ����. $$$

$$$��$$$
�� Scroll / ������ü Scroll * �������� ��ġ ( �� Scroll : ���� Layer ũ�� - ȭ�� ũ�� , ������ü Scroll : ���� Layer ũ�� - ȭ�鿡 ���̴� ũ�� * Zoom )
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

// [8/9/2007 breadceo] ����ü�� ��´�.
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

    // ������ ��� �ϴ� ��
    virtual void Process() = 0;                         // ��ũ���̳� ��Ÿ ����� ó��
    virtual void FrameMove( float fElapsedTime ) = 0;   // FrameMove

    virtual void SetZoffset();    

    // IGCRenderObj
	virtual HRESULT Render(){return S_OK;};
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


protected:
    float                   m_fLayerSizeX;
    float                   m_fLayerSizeY;
    int                     m_iLayerNum;
    bool                    m_bShow;

    // �ؽ��� ���� �����ؼ� �׸��� ����
    std::vector< CRenderPartPair<T0> >    m_vecRenderPart;
    
    
    // DP Info
    struct DPInfo
    {
        DWORD               m_dwVBIndex;
        DWORD               m_dwIBIndex;
        DWORD               m_dwRenderSize;
        GCDeviceTexture*    m_pTexture;
    };
    std::vector<DPInfo>     m_vecDPInfo;
    T1*                     m_pLayerManager;

public:
    std::vector<T0*>        m_vecPart;
};

#include "KLayer.inl"
