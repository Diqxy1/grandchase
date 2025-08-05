#pragma once

#include <KNCSingleton.h>
//#include <vector>

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCDeviceTexture;
class KGC3DObject;

class KGC3DObjectMgr
{
    DeclareSingleton( KGC3DObjectMgr );

public:
    KGC3DObjectMgr(void);
    virtual ~KGC3DObjectMgr(void);

    void Add3DObject( KGC3DObject* pObj );
    void Remove3DObject( KGC3DObject* pObj );
    void Add3DObjectForPrevLoad( KGC3DObject* pObj );

    // [12/5/2007] breadceo.방 or 다음스테이지로 돌아갔을때 불러줄 함수
    void ReleaseGame();
    void ReleaseStage();

    // [12/5/2007] breadceo.하드디스크 스왑을 없애기 위해 미리 로딩
    void PreLoadMesh( const std::string& strMesh );
    void PreLoadMotion( const std::string& strMotion );
    void PreLoadTex( const std::string& strTex );

    void FrameMove();

private:
    // [12/5/2007] breadceo.한 스테이지가 넘어가면 지워져야할 리스트
    std::vector< KGC3DObject* > m_vec3DObject;
    // [12/5/2007] breadceo.방으로 돌아가면 지워져야할 리스트
    std::vector< KGC3DObject* > m_vec3DObjectPrevLoad;
    std::vector< GCDeviceMeshP3M* > m_vecPreLoadMesh;
    std::vector< GCDeviceMotionFRM* > m_vecPreLoadMotion;
    std::vector< GCDeviceTexture* > m_vecPreLoadTex;
};

DefSingletonInline( KGC3DObjectMgr );
