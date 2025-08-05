// 2011. 7. 18, shmhlove
#ifndef _KGCEXTRAMESH_H_
#define _KGCEXTRAMESH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCGraphicsHelper.h"
#include "GCDeviceMotionFRM.h"

/* 구조체 설계 : SExtraMeshInfo
    이 구조체는 하나의 ExtraMesh에 대한 정보를 담고있다.
    모델파일명, 텍스쳐파일명, 모션인덱스, 사용할 조건과 조건에 대한 데이터
*/
struct SEM_PlayerMotionCondition
{
    SEM_PlayerMotionCondition() 
        : iConditionID( -1 )
        , iPlayerMotionID( -1 )
        , bWeaponToggle( false )
        , bEnable( false )
    {
        strPC_MotionName.clear();
    }

    int iConditionID;
    int iPlayerMotionID;
    bool bWeaponToggle;
    bool bEnable;
    std::string strPC_MotionName;
    std::string strObjName;
};

struct SExtraMeshInfo
{
    // 이 메시를 붙힐 오브젝트 객체
    GCObject* pObj;

    // 메시에 대한 기본 정보
    int         iBoneIdx;
    std::string strModelFile;
    std::string strTextureFile;
    std::string strBaseMotionFile;

    // 컨디션정보
    std::vector< SEM_PlayerMotionCondition > vecCondition_PM;

    // 파티클
};

/*클래스 설계
Set된 SExtraMeshInfo정보를 벡터에 담고 
로드와 프레임무브, 언로드를 담당한다.

로드는 Set할때 받은 오브젝트객체를 이용해서 로드한다.
프레임무브에서는 SExtraMeshInfo에 담겨진 컨디션을 이용해서 조건체크한다.
현재는 캐릭터의 특정모션일때 Toggle되고 애니메이션되는 조건Enum이 GCEnum에 추가되어 있다.

언로드는 언로드함수를 만들어두고 외부에서 호출하거나 소멸자에서 호출하도록 만든다.

이 클래스의 객체는 Global.h에서 들고있도록 한다.
*/
class GCExtraMesh
{
public:
    GCExtraMesh();
    ~GCExtraMesh();

public:
    void LoadExtraMesh( GCObject* pObj_, int iPlayerIdx );
    bool SetExtraMeshInfo( SExtraMeshInfo &MeshInfo );
    void ClearMeshInfo();

public:
    void FrameMove( int iPlayerIdx );

private:
    void CheckConditionPlayerMotion( int iMeshInfoIdx, int iPlayerIdx );

private:
    std::vector< SExtraMeshInfo > m_vecMeshInfo;
};

#endif
_KGCEXTRAMESH_H_