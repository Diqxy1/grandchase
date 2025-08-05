#ifndef _GCCAMERAORTHO_H_
#define    _GCCAMERAORTHO_H_

#include "lua_tinker.h"
//#include "KLuabinder.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GCCollisionRect.h"

class KGCCameraOrtho : public lua_tinker::lua_value
{
public:
    KDeclareLuaInvoke

    KGCCameraOrtho();
    ~KGCCameraOrtho();

    enum
    {
        POS_MODE_NO     =-1,
        POS_MODE_DIRECT = 0,
    };

    void UpdateCameraMatrix();        //Camera matrix를 계산합니다
    void SetZoom( float fZoom , char cZoomPlayer = -1 );
    void SetCameraPosition(float fX, float fY, int iPosMode = -1);
    void FrameMove();
    void FrameMove_DeathPlayer();    //게임중 죽었을 경우의 카메라 움직임...-_-;
    void FrameMove_OnGame();        //게임중의 카메라 움직임
    void ViewModeFrameMove();
    void InitCamera();
    void InitCameraPosition();        //게임 시작시 카메라의 초기 위치 설정
    void SetTargetPlayer(int iTarget);
    void SetTargetMonster(int iTarget);
    void SetNextTargetPlayer(int iWhosNext = -1);

    void SavePrevState(){ m_fPrevWideMode = m_fWideMode; }
    void BackToPrevState(){ m_fWideMode = m_fPrevWideMode; }
    void SetEarthQuake( int iEarthQuake ){ m_iEarthQuake = iEarthQuake; }
    void SetEarthQuakeY( int iEarthQuakeY ){ m_iEarthQuakeY = iEarthQuakeY; }
    void SetShakeCameraFrame( int iShakeCameraFrame );
    void RegisterLuabind();
    void ZoomMessage( bool bZoomMode );
    void SetDirectCameraPosition(float iPosX, float iPosY, int iPosMode = -1 );

    // [1/8/2008] breadceo. for square
    void FrameMoveInSquare();
    void SquareObserveFrameMove();
    void SquareFrameMove();
    void SetNextSquarePeople( bool bLocal = false );
    void SetSquareCamera( float fX, float fY );
    void InitSquareCamera();
    void UpdateCameraMatrixForSquare();
    D3DXVECTOR3* GetCameraCenterPos() { return &m_vCenterPos; }

    // 줌기능
    bool CheckDashZoom();
    bool CheckJumpZoom();
    bool CheckSpecialZoom();

    bool IsMonsterTargetCamera()  {   return m_bMonsterCamera;    }

public: // 카메라 흔들기
    void CameraVib( int iDirction, float fCameraVib, float fDecrease );
private:
    float m_fCameraVib; // 0이면 좌우 진동, 1이면 상하진동
    float m_fDecrease;  // 카메라가 흔들리는 정도
    int m_iDirection;   // 흔들리는 정도가 줄어드는 정도
    D3DXVECTOR3 m_vCenterPos;

private:
    bool m_bFollow;             // 따라갈 긋이냐?
public:
    D3DXMATRIX                    m_matCamera;

    bool    m_bZoomViewMode;        //이게 true가 되면 TargetZoom이 1.5인 효과를 같게됨
    int     m_iHostViewZoomMode;    //Observer 식의 관찰을 원할 때 사용함

    float   m_fWideMode;            //...전체화면 같은 기능을 넣어주기위함
    bool    m_bMonsterCamera;        //몬스터를 따라가는 카메라
    int     m_iTargetMonster;
    int     m_iTargetPlayer;        //따라가고 있는 Player의 index

    int     m_iEarthQuake;
    int     m_iEarthQuakeY;
    int     m_iShakeCameraFrame;
    int     m_iZoomCount;
    float   m_fTargetZoom;
    float   m_fZoom;
    float   m_fLookTermX;
    float   m_fLookTermY;
    float   m_fBoundTermX;
    float   m_fBoundTermY;
    float   m_fFollowRateX;
    float   m_fFollowRateY;

    float   m_fLookX;
    float   m_fLookY;

    float   m_fPrevWideMode;        //이글사이트 때 전의 상태로 돌아가기 위한 저장 변수 

    bool GetFollow() const { return m_bFollow; }
    void Follow(bool val) { m_bFollow = val; }

    FRect   m_rtCamera;
    FRect   m_rtCameraBasic;        //  Zoom 데이터가 적용되지 않았을때 카메라 영역

    // [1/8/2008] breadceo. for square
    DWORD   m_dwSquareUID;
    bool    m_bObserve;

    float   m_fTargetOffsetX;
    float   m_fTargetOffsetY;
    void SetTargetOffsetX( float fOffset )  {   m_fTargetOffsetX = fOffset; }
    void SetTargetOffsetY( float fOffset )  {   m_fTargetOffsetY = fOffset; }

    float   m_fStaticX;
    float   m_fStaticY;
    void SetStaticX( float fStatic )  {   m_fStaticX = fStatic; }
    void SetStaticY( float fStatic )  {   m_fStaticY = fStatic; }

public:// 스테이지 시작시 카메라 초기화처리를 할것인지?? 말것인지??
    bool    m_bQuestStartedOfResetCamera;
    void SetQuestStartedOfResetCamera( bool bInitCamera )   {   m_bQuestStartedOfResetCamera = bInitCamera; }
    bool GetQuestStartedOfResetCamera()                     {   return m_bQuestStartedOfResetCamera; }

};

extern KGCCameraOrtho g_kCamera;

#endif // _GCCAMERAORTHO_H_
