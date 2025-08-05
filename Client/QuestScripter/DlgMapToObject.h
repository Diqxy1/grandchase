#pragma once

#include "stdafx.h"

//#include "LuaLoader.h"
//#include <vector>
//#include <map>
//#include "GCDeviceManager.h"
//#include "../GC_MapTool/LayerManager.h"
//#include "struct.h"
#include "resource.h"
#include "ToolPalette.h"
#include "ObjectManager.h"
//
#include "MonsterObjectManager.h"
#include "PlayerObjectManager.h"
#include "DecorObjectManager.h"
#include "GateObjectManager.h"
#include "DamageObjectManager.h"
#include "MonSelFunctionManager.h"
#include "AreaAssignFunctionManager.h"
#include "PosAssignFunctionManager.h"


class CQuestScripterDlg;
class CdlgTriggerContent;
class CDlgTGMonsterGenerater;
class CDlgTGPlayerPosition;
class CDlgTGObjectGenerater;


// CDlgMapToObject 대화 상자입니다.

class CDlgMapToObject : public CDialog, public CD3DApplication
{
	DECLARE_DYNAMIC(CDlgMapToObject)

public:
	CDlgMapToObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMapToObject();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGMAPTOOBJECT };

    //CLuaLoader m_LuaLoader;
    HRESULT LoadMapToObject(CString str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    HINSTANCE                   m_hinstance;              //인스턴스 핸들
    HWND                        m_hwndRenderWindow;       //윈도우 모드- 렌더링용 윈도우 핸들
    HWND                        m_hwndRenderFullScreen;   //풀스크린 모드- 렌더링용 윈도우 핸들
    HWND                        m_hWndTopLevelParent;     //MFC최상위 윈도우의 윈도우 핸들
    CD3DFont*                   m_pFont;                  // Font for drawing text

    CString                     m_strMapScript;     // 맵스크립트의 경로
    bool                        m_bLoadScript;      // 스크립트 로드여부
    bool                        m_bLayerMode;
    bool                        bEndThread;
    bool                        IsShiftPushed;
    bool                        bControlPushed;
    float                       m_fEyeX, m_fEyeY;
    float                       m_fWidth, m_fHeight; // 화면의 가로, 세로
    float                       m_fCameraZoom;
    CPoint                      m_ptDragItem;        // 선택한 Object의 화면 위치 저장용
    CPoint                      m_Point;             // 마우스의 클릭한 위치 저장
    CPoint                      m_StartPoint;        // 클릭한 곳의 EyeX, EyeY 위치
    CToolPalette                m_ToolPalette;       // 몬스터 추가삭제용 도구상자
    LPDIRECT3DSWAPCHAIN9        m_pSwapChain1;
    LPDIRECT3DSWAPCHAIN9        m_pSwapChain2;
    CRITICAL_SECTION            cs;                  // 크리티컬 섹션

    HRESULT             Create( HINSTANCE hInstance );
    BOOL                Create(CWnd* pParentWnd, CString strScript);
    HRESULT             InitD3D( HWND hWnd );
    void                CleanUp3D();
    virtual HRESULT     OneTimeSceneInit();
    virtual HRESULT     FinalCleanup();
    virtual HRESULT     InitDeviceObjects();
    virtual HRESULT     DeleteDeviceObjects();
    virtual HRESULT     RestoreDeviceObjects();
    virtual HRESULT     InvalidateDeviceObjects();
    virtual HRESULT     FrameMove(void);
    virtual HRESULT     Render(void);
    HRESULT             RenderText();
    VOID                RenderScene();
    virtual BOOL        PreTranslateMessage(MSG* pMsg);
    void                Get3DWorldPos( D3DVECTOR* PosOut );
    BOOL                ShowWindow(int nCmdShow);

    //************************************************************************
    // 메인 창의 포인터 관련
    CQuestScripterDlg *m_pQuestScriptDlg;
    void SetQuestScripterDlg(CQuestScripterDlg *pQuestScriptDlg);
    //************************************************************************
    
    //************************************************************************
    // 발판용 함수
    void RenderFH();
    bool m_bFoot;
    //************************************************************************

    //************************************************************************
    // 몬스터 추가 모드 혹은, 플레이어 추가 모드 선택 함수
    TG_TYPE                 m_TGType;
    OBJECT_TYPE_EACTION     m_enObjectType_EAction;
    OBJECT_TYPE_CONDITION   m_enObjectType_Condition;
    void SetTGType(TG_TYPE Type);
    void SetObjectType_EAction(OBJECT_TYPE_EACTION enType);
    void SetObjectType_Condition(OBJECT_TYPE_CONDITION enType);
    //************************************************************************

    //************************************************************************
    // 맵에 표시되는 Item들의 정보를 관리하는 TriggerContent 포인터.
    CdlgTriggerContent *m_pDlgTGContent;
    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent);
    //************************************************************************

    //************************************************************************
    // 오브젝트 관리 클래스
    //************************************************************************
    CObjectManager              *m_pObjectManager;
    CMonsterObjectManager       m_MonsterManager;
    CPlayerObjectManager        m_PlayerManager;
    CDecorObjectManager         m_DecorObjectManager;
    CGateObjectManager          m_GateObjectManager;
    CDamageObjectManager        m_DamageObjectManager;

    //************************************************************************
    // 맵 기능 관리 전담 클래스
    //************************************************************************
    CFunctionManager            *m_pFunctionManager;
    CMonSelFunctionManager      m_MonselFunctionManager;
    CAreaAssignFunctionManager  m_AreaFunctionManager;
    CPosAssignFunctionManager   m_PosFunctionManager;

    //************************************************************************
    // 맵에 표시되는 오브젝트 리스트
    //************************************************************************
    std::vector<CMonsterGenerater*>      m_vtMonsterGen;
    std::vector<CPlayerPosition*>        m_vtPlayerPosition;
    std::vector<CObjectGenerater*>       m_vtObjectGen;
    std::vector<CGateObjectGenerater*>   m_vtGateObjectGen;
    std::vector<CAddDamage*>   m_vtDamageObjectGen;

    // 현재 맵의 타입으로 사용할 오브잭트 관리 혹은 맵기능을 변경시킨다.
    void SetMapFunctionMode(TG_TYPE TGType, OBJECT_TYPE_EACTION enObjectType_EAction, OBJECT_TYPE_CONDITION enObjectType_Condition);

    //************************************************************************
    // 화면에 그려질 있는 몬스터(or 플레이어, 오브젝트) 정보들 관리
    void SetMonsterList(std::vector<CBaseEventAction*> &vtMonList);
    void SetPlayerPosition(std::vector<CBaseEventAction*> &vtPlayerList);
    void SetObjectList(std::vector<CBaseEventAction*> &vtObjectList);
    void SetGateObjectList(std::vector<CBaseEventAction*> &vtGateObjectList);
    void SetDamageObjectList(std::vector<CBaseEventAction*> &vtGateObjectList);
    //************************************************************************

    afx_msg void    OnBnClickedOk();
    afx_msg void    OnBnClickedCancel();
    afx_msg LRESULT OnClickToolButton(WPARAM wParam, LPARAM lParam);
    afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void    OnTimer(UINT nIDEvent);
    afx_msg void    OnDestroy();
    afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void    OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg LRESULT OnChangeType(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMonSelFunction(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAreaSelFunction(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMove(int cx, int cy);

    void ResetWindowSize();
};

extern CDlgMapToObject         *g_DlgMapToObject;
extern LPDIRECT3DDEVICE9       g_pd3dDevice;
