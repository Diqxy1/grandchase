#ifndef _GCTUTORIALCHAR_H_
#define _GCTUTORIALCHAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "KGCTutorialJob.h"

#define ANI_SPEED       0.1f
#define GAP_ROLLOVER    5.0f
#define TEX_WIDTH       512
#define TEX_HEIGHT      512

#define MAX_CHAR_LINE   12
#define MAX_ICON_LINE   2

class KGCCharacterName;

struct SpecialFace
{
    GCDeviceTexture* pTex;
    std::string strTexName;
    D3DXVECTOR2 vecUVLeft;
    D3DXVECTOR2 vecUVRight;
    D3DXVECTOR4 vecWndPos;

    SpecialFace() :pTex(NULL) {}
};

struct SpecialFaceAni
{
    float fRadio;
    D3DXVECTOR4 vecStartPos;
    D3DXVECTOR4 vecEndPos;
    SpecialFaceAni() :fRadio(1.0f) {}
};

class KGCTutorialChar : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME(KGCTutorialChar);
    /// Default constructor
    KGCTutorialChar(void);
    /// Default destructor
    virtual ~KGCTutorialChar(void);
    /// Copy constructor
    KGCTutorialChar(const KGCTutorialChar&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCTutorialChar& operator=(const KGCTutorialChar&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreateComplete(void);
    virtual void OnDestroyComplete(void);
    virtual void PreDraw(void);

public:
    enum { CHAR, JOB, };
    enum { MY_JOB, EXT_JOB, };
    enum { MY_SELECTOR, EXT_SELECTOR, };
    enum { MYTAP1, MYTAP2, EXTTAP1, EXTTAP2, };
    enum { LEFT_0, RIGHT_0, LEFT_1, RIGHT_1, };
    enum { CHAR_ICON_GAP = 2 };

public:
    void CharOpenSkill();
    void SetJop(int iIndex, bool bCreate = false);
    void SelectChar(int iIndex, bool bCreate = false);
    void SetPlayerTap(bool bIndex, bool bCreate = false);
    void SetCharSelector(int iCharIdx);

    inline bool GetPlayerTab() { return m_bPlayerTap; };
    inline int GetMyChar() { return m_iMySelCharIndex; };                               // 내가 선택한 캐릭
    inline int GetExtChar() { return m_iExtSelCharIndex; };                             // 연습용으로 선택된 캐릭
    inline int GetMyJobLevel() { return m_iJobState[m_iMySelCharIndex][MY_JOB]; };   // 내가 선택한 캐릭의 현재 전직
    inline int GetExtJobLevel() { return m_iJobState[m_iExtSelCharIndex][EXT_JOB]; }; // 연습용으로 선택된 캐릭의 현재 전직
    bool LoadSpecialFaceScript(std::string strFileName, SpecialFace* pSpecialFace);       // playertemlate.stg파일 파싱

private:// 애니메이션 관련
    template<typename T>
    T LinearInterpolation(T& t1, T& t2, float& fRatio);                                // 선형보간
    void SetInterpolationRadio(float& fRadio);                                         // 보간률 셋팅
    void RenderFace(D3DXVECTOR4 vecTempPos, int iChar, int iJob, bool bReverse);          // 필살기 이미지 랜더
    void SetShowAnimation(bool bShow, bool bDirection, SpecialFace* pOri);               // 필살기 이미지 애니메이션 설정
    void SetRolloverAnimation(bool bShow, bool bdirection, SpecialFace* pOri);            // 마우스 롤오버 애니메이션 설정

private:
    KD3DStatic* m_pkStaticMySel;
    KD3DStatic* m_pkStaticExtSel;
    KSafeArray<KD3DWnd*, 2> m_pkWndBox;
    KSafeArray<KD3DWnd*, 4> m_pkWndTab;
    KSafeArray<KD3DWnd*, 2> m_pkWndBigFace;
    KSafeArray<KD3DWnd*, 2> m_pkWndSelector;
    KGCTutorialJob* m_pkJob;

    KSafeArray<KD3DWnd*, GC_CHAR_NUM > m_pkWndCharFace;
    SpecialFace m_SpecialFace[GC_CHAR_NUM][NUM_JOB_LEVEL];
    std::set< int > m_setOpenCharSkill;

private:
    bool m_bPlayerTap;
    int m_iMySelCharIndex;
    int m_iExtSelCharIndex;
    int m_iBeforeMySelIdx[2];
    int m_iBeforeExtSelIdx[2];
    int m_iJobState[GC_CHAR_NUM][2];
    SpecialFaceAni m_Ani[4];

    KD3DWnd* m_pkCharIconArrayRect;

    KGCCharacterName* m_pkCharNamePlayer;
    KGCCharacterName* m_pkCharNameCom;
};

DEFINE_WND_FACTORY(KGCTutorialChar);
DECLARE_WND_FACTORY(KGCTutorialChar);
DECLARE_WND_FACTORY_NAME(KGCTutorialChar);

#endif  // _GCTUTORIALCHAR_H_
