// 튜토리얼모드의 새로운 버젼과 이전버젼을 구분한다. #else를 분기점으로 구분이 된다.
#ifndef _KGCMODETUTORIAL_H_
#define _KGCMODETUTORIAL_H_
//#include "define.h"

#include "../gcui/KGCNewTutorialOption.h"
#include "KGCGameModeInterface.h"
////#include <vector>
//#include <map>
//#include "../gcui/GCQuestTile.h"

class KGCSkillCheckBtn;


struct KTutorialMapInfo
{
    EGCGameStage        emGameStage;

    int                 nStringIndex;
    int                 nMapIndex;

    D3DXVECTOR2         vMyPos;
    D3DXVECTOR2         vComPos;

    KTutorialMapInfo()
    {
        Init();
    }

    void Init()
    {
        emGameStage = GC_GS_FOREST_OF_ELF;
        nStringIndex = 0;
        nMapIndex = 0;
        vMyPos.x = 0.f;
        vMyPos.y = 0.f;
        vComPos.x = 0.f;
        vComPos.y = 0.f;
    }
};


class KGCModeTutorial : public KGCGameModeInterface
{
public:
    KGCModeTutorial(void);
    virtual ~KGCModeTutorial(void);

public:
    virtual HRESULT InitAtGame();
    virtual HRESULT InitAtLoading();
    virtual HRESULT CustomFrameMove(float fElapsedTime);
    virtual HRESULT CalcExpAndGP() { return S_OK; }
    virtual bool CheckGameEnd() { return false; }
    virtual void GameEnd() { };

private:
    UINT m_uiCharType;
    UINT m_uiFrameCount;
    std::vector<KGCSkillCheckBtn*> m_vecSkillCheckBtnAnimation;
    std::vector<KGCSkillCheckBtn*> m_vecSkillCheckBtn;

    int m_iMaxMapNum;

    void GetCurMousePosInMap(float& fx, float& fy);				// 현재 맵위의 마우스 좌표를 가져온다.
    bool IsComboIng(void);										// 현재 나의 캐릭터가 콤보를 때리고있나?

protected:
    std::map< EGCGameStage, KTutorialMapInfo > m_mapTutorialInfo;           // 튜토리얼 맵 정보
    std::vector< EGCGameStage >                m_vecSeqTutorial;            // 튜토리얼 맵 순서

public:
    void GetTutoriaInfo(std::map< EGCGameStage, KTutorialMapInfo >& mapTutorialInfo);
    EGCGameStage GetSeqMagStage(int nSeqIndex);
    int GetSeqMapStringID(int nSeqIndex);
    int GetTutorialMapSize() { return m_iMaxMapNum; }

public:
    EGCGameStage    m_emGameStage;   // 튜토리얼의 게임 스테이지
    void SetCurStageMyCharPos(EGCGameStage emGameStage, D3DXVECTOR2 vPos);
    void SetCurStageTutorialCharPos(EGCGameStage emGameStage, D3DXVECTOR2 vPos);
    D3DXVECTOR2 GetCurStageMyCharPos(EGCGameStage emGameStage);
    D3DXVECTOR2 GetCurStageTutorialCharPos(EGCGameStage emGameStage);

    void SetCharType(int uiCharType);		// 캐릭터 타입을 바꿔준다. - 기술표 변경에 쓰인다.
    void CheckTutCharDirection(void);		// 튜토리얼 캐릭터가 내캐릭터를 바로보게 한다.
    void CheckDieOfChar(void);				// 나, 혹은 튜토리얼 캐릭터가 죽어있나 확인처리.
    void KeyProc(void);						// 튜토리얼에서 입력되는 키 처리
    void IncreaseHP(void);					// HP상승

    UINT m_uiPractice_Motion;

    int iToggle_Key_S;

    UINT m_uiPrevCount;
    UINT m_uiGapNextFrame;

    void PracticeCharFrameMove();
    void SetPracticeCharMotion();
    void InitPracticeCharMotion();
    bool LoadTutorialModeCharPosition();
};

#endif  // _KGCMODETUTORIAL_H_
