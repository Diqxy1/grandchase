// QuestScripterDlg.h : 헤더 파일
//************************************************************************
// name         : 최재호 (Jaeho Choi)
// date         : 2007.10.22
// lastupdate   : 2007.10.22
// comment      : 안녕하세요... 퀘스트 스크립트를 만든 제작자 입니다.
//                퀘스트 스크립트를 사용하기 전에 몇가지 숙지 해야할 것이
//                있습니다.
//                //먼저 반드시 필요한 파일들의 목록입니다.//
//                [1]
//                Data/Script 폴더에 "PartsCategoryList.lua" 파일이 반드시
//                있어야 합니다. 이 파일은 맵툴에서 사용되는 파일 형식
//                입니다. 맵에서 사용되는 Obj들의 텍스쳐를 모아두는 파일로써
//                반드시 필요합니다. 만약 이파일이 없다면 맵을 정상적으로
//                표시하지 못할 것입니다.
//                [2]
//                Data/Shader 폴더안의 파일도 반드시 필요하며, 블랜딩이나
//                그 외의 효과에 사용합니다.
//                (현재는 블랜딩에 밖에 안씁니다.)
//                [3]
//                텍스쳐 폴더 안에 들어있는 bmp 파일들도 반드시 필요
//                합니다. 각각의 bmp 파일은 PreLoadScript 안에 스크립트
//                형식으로 제공됩니다.
//                DlgMapToObject.h / cpp 파일은 아직 정리가 안되서 매우매우
//                보기 힘든 소스이기도 합니다... 시간날때 마다 수정하고
//                하겠지만 혹시 그전에 저보다 먼저 수정하시는 분이 있으시다면
//                죄송해요 ㅡ.ㅡ;;; 아마 추후에 맵툴이랑 합쳐지지 않을까
//                하는 생각도 들지만... 어떻게 될지 모르겠네요~!!!
//                그럼 이만... :)
//************************************************************************

#pragma once
#include "stdafx.h"
#include "DlgFileList.h"
#include "MyTreeCtrl.h"
#include <ImportKncDebug.h>
#include "DataManager.h"
#include "DlgQuest.h"
#include "DlgStage.h"
#include "DlgTrigger.h"
#include "DlgMapToObject.h"
#include "DlgInfo.h"

//#include <vector>
//#include <map>

#include "struct.h"

#define MAX_REGISTER_INDEX               5
#define QUEST_SCRIPTER_AUTOSAVE_TIME     60000

// CQuestScripterDlg 대화 상자
class CQuestScripterDlg : public CDialog
{
	// 생성
public:
	CQuestScripterDlg(CWnd* pParent = NULL);	// 표준 생성자
	~CQuestScripterDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_QUESTSCRIPTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

	bool                m_bCreated;                     // 창 생성여부
	bool                m_bAutoSave;                    // 자동 저장 여부

public:

	CDataManager        m_Datamanager;              // Redo, Undo 용 데이터 관리 매니져

	//************************************************************************
	// 자식 창
	//************************************************************************
	CDlgQuest           m_dlgQuest;                 // 퀘스트 정보 표시창
	CDlgStage           m_dlgStage;                 // 스테이지 정보 표시창

	//************************************************************************
	// 팝업 창
	//************************************************************************
	CDlgTrigger         m_dlgTrigger;               // 트리거 에디터
	CDlgMapToObject     m_dlgMapToObject;           // 캐릭터 추가화면(DirectX) 표시창
	CDlgInfo            m_dlgInfo;                  // 팝업 창 정보 표시창
	CDlgFileList        m_dlgFileList;              // 파일 리스트 팝업창

	CStatic                                 m_stInfo;           // 어떤 메시지 처리가 일어났는지 기록하는 윈도우
	CStatic                                 m_stMain;           // 정보표시창을 표시할 창

	HACCEL                                  m_hAccelTable;      // 단축키 사용을 위해 사용하는 Accel 핸들
	HTREEITEM                               m_hSelectedBefore;  // 현재 트리의 바로 전에 선택한 트리. (Jaeho.Modify ReDraw시나 ItemMove시에 SelectBefore의 처리)
	Quest                                   m_Quest;            // 퀘스트 자료구조 ( m_ScriptTree 구조랑 연동해서 동작 )
	CEditTreeCtrl                           m_ScriptTree;       // Quest 자료구조를 트리 형태로 표시하는 클래스
	std::map< int, std::vector<CString> >             m_NamesOfLevel;     // 트리 레벨별 추가 가능 인자 리스트
	std::map< int, std::vector<NamesOfLevel> >        m_NamesOfLevel_Def; // 트리 레벨별 추가 가능 인자 리스트의 Enum 변수

	CString                                 m_strLastLoadFile;          // 마지막으로 불러온 파일 이름
	NamesOfLevel                            m_LastUse_ClipBoard;        // 복사, 잘라내기를 해놓은 가장 최신의 자료구조 명.
	Quest                                   m_Quest_ClipBoard;          // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
	Stage                                   m_Stage_ClipBoard;          // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조

	HTREEITEM OnAddchild_Enter();           // 트리에서 하위구조를 추가해준다. (추가할 수 있는 하위구조중 가장 상단에 배치된것 추가.)
	HTREEITEM OnAddSibling_SpaceBar();      // 트리에서 이웃구조를 추가해준다. (추가할 수 있는 이웃구조중 가장 상단에 배치된것 추가.)
	void AddChild(HTREEITEM hSelected, HTREEITEM hNewChild, NamesOfLevel NOL_Of_NewChild);  // Tree에서 생성된 새로운 자식정보를 Quest에 넣어주는 함수
	void AddSibling(HTREEITEM hSelected, HTREEITEM hNewChild, HTREEITEM hParent);           // Tree에서 생성된 새로운 이웃정보를 Quest에 넣어주는 함수
	void MoveSibling(HTREEITEM hSelected, HTREEITEM hNewPos);                               // 자료를 트리 내부에서 이동할 경우 사용.

	void SaveSelectedBefore(void);                  // 바로 이전 트리 정보를 저장하는 부분.
	void ChangeDialog(HTREEITEM hSelected);         // 정보입력 창을 변경하는 부분.
	void HideAllDialog(void);                       // 정보입력 창을 모두 숨기는 부분.

	HTREEITEM MadeTree(NamesOfLevel NOL, int iNumOfLv);         // 현재 선택된 트리구조에서 트리타입, 트리Index를 인자로 넣어 트리추가
	void MadeTree(Quest &quest);                                // Quest의 자료구조를 참고하여 Tree창에 트리구조를 만든다.
	void MakeTreeByStage(Stage &stage, int iLevel);             // Stage의 자료구조를 참고하여 Tree창에 트리구조를 만든다.

	void ReDrawTree(void);          // 트리표시창에 트리 정보를 다시 그린다.
	HRESULT LoadQuestFromLua(KLuaManager &luaMgr, Quest *quest);                              // 루아로 저장된 Quest 정보를 로드합니다.
	HRESULT LoadStageFromLua(KLuaManager &luaMgr, CString strStage, Stage **stage);            // 루아로 저장된 Stage 정보를 로드합니다.

	HRESULT LoadPreLoad(void);                      // Enum 정보들을 루아를 통해 불러오는 함수
	void SetWindowMainTitle(CString str);           // 윈도우 타이틀 변경 함수
	LRESULT ShowMapToObjectWindow(int iType, int iPaletteMode, WPARAM wParam, LPARAM lParam);// MapToObject 윈도우를 연다.

	// 파일로 부터 스크립트를 로드 하는 함수
	void LoadFromFile(TCHAR *szFileName);            // 스테이지만 로드 (부분로드)
	void LoadFromFileFull(TCHAR *szFileName);        // 퀘스트 로드 (전체로드)

	void SaveToFile(TCHAR *szFileName);              // 파일로 저장 합니다. (선택 된 트리와 하위 트리까지 복사합니다.)

	// 최근 사용한 파일을 레지스트리에 저장하고 읽어 들이는 함수
	void WriteToRegisterRecent(int iIndex, char *szFileName);
	void ReadFromRegisterRecent(int iIndex, char *szFileName);

	//************************************************************************
	// 트리거 카테고리 정보를 로드하기 위해 있는 것
	//************************************************************************
	HRESULT LoadFromFile_Trigger(KLuaManager &luaMgr, CCategory **pCategory, int iStage);
	HRESULT LoadFromFile_Event(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage);
	HRESULT LoadFromFile_Condition(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage);
	HRESULT LoadFromFile_Action(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage);

	//************************************************************************
	// 전체 스테이지에서 트리거이름이 중복되는게 있나 확인
	//************************************************************************
	bool FindEqualTriggerName(std::wstring strTGName);

	//************************************************************************
	// 트리거 명으로 트리거를 찾는다.
	//************************************************************************
	CBaseTrigger *FindTriggerByName(std::wstring strTGName);

	//************************************************************************
	// TGOnOff 트리거의 트리거 명을 strTGChanged로 변경한다.
	//************************************************************************
	void ChangeTGOnOffName(std::wstring strTGChanged, std::wstring strTGName);

	//************************************************************************
	// 비슷한 트리거명을 가진 녀석들의 총 갯수 리턴
	//************************************************************************
	void GetCntOfSimilarTrigger(std::wstring strTGName, std::vector<std::wstring> *vtSimilar);

	void ReSizeChildWindow(void);

	// 트리거 타입으로 트리거 리스트를 얻는다.
	void GetTGEActionListByType(TG_EACTION enType, std::vector<CBaseEventAction*> &vtList);
	void GetTGConditionListByType(TG_CONDITION enType, std::vector<CBaseCondition*> &vtList);

	int StaticInfoWithMessageBox(CString strText, CString strCaption, UINT nType);

	HRESULT SaveScriptForServer(HANDLE hHandle, CString strFileName, int iLevel, int iTab = 0);
	bool LoadQuestSignForServerScript(KLuaManager &luaMgr, HANDLE hWrite);
	HRESULT SaveScriptForServer_Subject(HANDLE hHandle, CString strFileName, CStringA strQuestFileName, int iQuestID, int iLevel, int iTab = 0);
	bool LoadQuestSignForServerScript_Subject(KLuaManager &luaMgr, HANDLE hWrite);

	// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	void OnUpdateFlush(CMenu *pMenu);
	afx_msg void OnTreeNew();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddchildQ();
	afx_msg void OnSiblingchildQ();
	afx_msg void OnRemarkQ();
	afx_msg void OnDeletesubtreeQ();
	afx_msg void OnDeleteentiretreeQ();
	afx_msg void OnInfoStruct();
	afx_msg void OnFileSave();
	afx_msg void OnSaveToFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLoadFromFileFull();
	afx_msg void OnLoadFromFile();
	afx_msg void On57604();
	afx_msg void OnEditRefresh();
	afx_msg LRESULT OnMonsterAddWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayerAddWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMonsterSeldWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAreaSelWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnObjectAddWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGateObjectAddWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDamageObjectAddWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeSequence(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeSequence();
	afx_msg LRESULT OnEnableMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSvaeToFileFull();
	afx_msg LRESULT OnChangeDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShiftTreeSelect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTreeCut();
	afx_msg void OnTreeCopy();
	afx_msg void OnTreePaste();
	afx_msg LRESULT OnAddchild_Enter_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddSibling_SpaceBar_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddchildQ_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddSiblingQ_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMoveSibling_forMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInfoStruct_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveToFile_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveToFileFull_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadFromFile_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadFromFileFull_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewTree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCutTree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCopyTree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPasteTree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT LoadIconLayerPart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeTreeFocus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnHelp();
	afx_msg void OnCollapseAllEntireTree();
	afx_msg void OnExpandAllTree();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCustomDirectory();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnFocusWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadFromFileListFull(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveServerScriptFromFileList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilelist();
	afx_msg void OnFileSavetoserverscriptByQuestsign();
	afx_msg void OnFileSavetoserverscriptBySubject();
	afx_msg void OnSvaeToSameFileFull();
	afx_msg void OnFileAutosave();
	afx_msg LRESULT OnPosAsel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPosSelWindow(WPARAM wParam, LPARAM lParam);
};
