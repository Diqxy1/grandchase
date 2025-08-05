#pragma once

//#include "define.h"
#include <d3dx9math.h>
//#include <vector>
#include "GCGraphicsHelper.h"

class GCObject;
class GCDeviceMotionFRM;

// [12/3/2007] breadceo.일단은 Damage에 붙일 3D Model을 위한 클래스



class KGC3DObject
{
public:
    KGC3DObject(void);
    virtual ~KGC3DObject(void);

    void Init();
	void Init( int iTech_ );
    void Release();
    void FrameMove();
    void AddMesh( const std::string& strMesh, const std::string& strTex,std::string strID = "", DWORD technique_ = -1 );
    void AddMotion( const std::string& strMotion );
    void AddMotion( UINT uiMotion );

    // [12/3/2007] breadceo.Getter/Setter
    bool GetRender() const { return m_bRender; }
    void SetRender(bool val);
    float GetScale() const { return m_fScale; }
    void SetScale(float val); 
    bool IsRight() const { return m_bIsRight; }
    void IsRight(bool val) { m_bIsRight = val; }
    D3DXVECTOR2 GetPos() const { return m_vPos; }
    void SetPos( const D3DXVECTOR2& val ) { m_vPos = val; }
    D3DXVECTOR2 GetMotionPos() const;
    UINT GetMotion() const { return m_uiMotion; }
    void SetMotionFromIndex(UINT val);
    void SetMotionFromMotionID(UINT val);
    D3DXVECTOR3 GetLocalPos() const { return m_vLocalPos; }
    void SetLocalPos(D3DXVECTOR3 val) { m_vLocalPos = val; }
    bool GetIsLive() const { return m_bIsLive; }
    void SetIsLive(bool val) { m_bIsLive = val; }
    GCObject* GetGCObject() { return m_pkModel; }
    GCDeviceMotionFRM* GetMotionPtr() const;
    void SetFrame( unsigned short cFrame ) { m_ucFrame = cFrame; }
    unsigned short GetFrame() const { return m_ucFrame; }
	void SetShellMatrixScale(float val);
    void SetModelZAxis( float fZ ) { m_fZaxis = fZ; }
	void SetSlowCount( unsigned char Slow_Count ) { m_Slow_Count = Slow_Count; }
	void SetCartoon( EGCCartoonTexture eCartoon ) { m_eCartoon = eCartoon; }
	void SetTechnique( int iTech_ );

protected:
    GCObject*   m_pkModel;
    std::vector< GCDeviceMotionFRM* > m_vecMotion;
    bool        m_bRender;
    float       m_fScale;
    bool        m_bIsRight;
    unsigned short        m_ucFrame;
    UINT        m_uiMotion;
    D3DXVECTOR2 m_vPos;
    D3DXVECTOR3 m_vLocalPos;
    EGCCartoonTexture m_eCartoon;
    bool        m_bIsLive;
    GCDeviceMotionFRM* m_pCurMotion;
    float       m_fZaxis;
	unsigned char m_Slow_Count;
};
