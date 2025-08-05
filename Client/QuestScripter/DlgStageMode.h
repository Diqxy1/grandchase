#pragma once
#include "MyTreeCtrl.h"
#include <vector>
#include <map>
using namespace std;

#include "struct.h"

extern "C" 
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


#include "KLuaManager.h"
#include "DlgSequence.h"
#include "DlgStage.h"
#include "DlgStageLv.h"
#include "DlgStageMonster.h"
#include "DlgInfo.h"
#include "DlgMapToObject.h"
#include "DataManager.h"
// CDlgStageMode 대화 상자입니다.

class CDlgStageMode : public CDialog
{
	DECLARE_DYNAMIC(CDlgStageMode)

public:
	CDlgStageMode(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgStageMode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STAGE_MODE };

    CDlgQuest           m_dlgQuest;                 // 퀘스트 정보 표시창
    CDlgSequence        m_dlgSequence;              // 시퀀스 정보 표시창
    CDlgStage           m_dlgStage;                 // 스테이지 정보 표시창
    CDlgStageLv         m_dlgStageLv;               // 스테이지 레벨 정보 표시창
    CDlgStageMonster    m_dlgStageMonster;          // 스테이지 몬스터 정보 표시창
    CDlgMapToObject     m_dlgMapToObject;           // 캐릭터 추가화면(DirectX) 표시창
    CDlgInfo            m_dlgInfo;                  // 팝업 창 정보 표시창

    CStatic                                 m_stInfo;           // 어떤 메시지 처리가 일어났는지 기록하는 윈도우
    CStatic                                 m_stMain;           // 정보표시창을 표시할 창
    HACCEL                                  m_hAccelTable;      // 단축키 사용을 위해 사용하는 Accel 핸들
    HTREEITEM                               m_hSelectedBefore;  // 현재 트리의 바로 전에 선택한 트리. (Jaeho.Modify ReDraw시나 ItemMove시에 SelectBefore의 처리)
    Quest                                   m_Quest;            // 퀘스트 자료구조 ( m_ScriptTree 구조랑 연동해서 동작 )
    CEditTreeCtrl                           m_ScriptTree;       // Quest 자료구조를 트리 형태로 표시하는 클래스
    map< int, vector<CString> >             m_NamesOfLevel;     // 트리 레벨별 추가 가능 인자 리스트
    map< int, vector<NamesOfLevel> >        m_NamesOfLevel_Def; // 트리 레벨별 추가 가능 인자 리스트의 Enum 변수
    vector<CString>                         m_vtEnumMonster;    // 몬스터 Enum 정보
    map<CString, CString>                   m_mapMonsterImage;  // <몬스터ID, 파일경로>

    NamesOfLevel                            m_LastUse_ClipBoard;        // 복사, 잘라내기를 해놓은 가장 최신의 자료구조 명.
    Quest                                   m_Quest_ClipBoard;          // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
    Sequence                                m_Sequence_ClipBoard;       // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
    Stage                                   m_Stage_ClipBoard;          // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
    StageLv                                 m_StageLv_ClipBoard;        // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
    Stage_mon                               m_StageMon_ClipBoard;       // 복사, 잘라내기, 붙여넣기를 위해서 사용하는 임시 구조
    CDataManager                            m_Datamanager;

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
    void MakeTreeBySequence(Sequence &sequence, int iLevel);    // Sequence의 자료구조를 참고하여 Tree창에 트리구조를 만든다.
    void MakeTreeByStage(Stage &stage, int iLevel);             // Stage의 자료구조를 참고하여 Tree창에 트리구조를 만든다.
    void MakeTreeByStageLv(StageLv &stagelv, int iLevel);       // StageLv의 자료구조를 참고하여 Tree창에 트리구조를 만든다.
    void MakeTreeByStage_mon(Stage_mon &stage_mon, int iLevel); // Stage_mon의 자료구조를 참고하여 Tree창에 트리구조를 만든다.

    void ReDrawTree(void);          // 트리표시창에 트리 정보를 다시 그린다.
    HRESULT LoadQuestFromLua(KLuaManager &luaMgr, Quest *quest);                              // 루아로 저장된 Quest 정보를 로드합니다.
    HRESULT LoadStageFromLua(KLuaManager &luaMgr, CString strStage, Stage *stage);            // 루아로 저장된 Stage 정보를 로드합니다.
    HRESULT LoadSequenceFromLua(KLuaManager &luaMgr, CString strSequence, Sequence *sequence);   // 루아로 저장된 Sequence 정보를 로드합니다.
    HRESULT LoadStageLvFromLua(KLuaManager &luaMgr, CString strStageLv, StageLv *stagelv);      // 루아로 저장된 StageLv 정보를 로드합니다.
    HRESULT LoadStageMonFromLua(KLuaManager &luaMgr, CString strStageMon, Stage_mon *stage_mon); // 루아로 저장된 StageMon 정보를 로드합니다.

    HRESULT LoadPreLoad(void);                      // Enum 정보들을 루아를 통해 불러오는 함수

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    virtual BOOL OnInitDialog();
};
