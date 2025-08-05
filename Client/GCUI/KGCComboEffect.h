#ifndef _GCCOMBOEFFECT_H_
#define _GCCOMBOEFFECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"
//#include "Particle/KGCParticleManager.h"
//#include "../GCUtil/GCInterpolate.h"
#include "../GCD3DWndEffect.h"

class KGCComboEffect : public KD3DWnd
{
public:
    KGCComboEffect(void);
    virtual ~KGCComboEffect(void);

    DECLARE_CLASSNAME( KGCComboEffect );

    enum EGCCOMBOEFFECT { EGC_CE_NONE, EGC_CE_GOOD, EGC_CE_GREAT, EGC_CE_WONDERFUL, EGC_CE_PERFECT, EGC_CE_BREAK, NUM_OF_EGC_CE };
    enum EGCCOMBOEFFECT_BG { EGC_CEBG_NONE, EGC_CEBG_BACKYELLOW, EGC_CEBG_BACKRED };

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void InitComboEffect(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow);     // 콤보이펙트의 초기화 함수

    // 콤보이펙트 발동 조건 체크
    void CheckComboState(void);

    inline bool IsTimeToComboStart(EGCCOMBOEFFECT enComboEffect)
    {
        if( 1 == m_iComboState[enComboEffect] )
            return true;
        else
            return false;
    }

    // 콤보이펙트 발동
    void SetShowComboEffect(EGCCOMBOEFFECT enComboEffect, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent);      // 글자 이펙트
    void SetShowComboEffectBG(EGCCOMBOEFFECT_BG enComboEffectBG, int iPosFrame, int iShowFrame, D3DXVECTOR2 vStartPos, D3DXVECTOR2 vEndPos);    // 배경 이펙트

    // 콤보이펙트 숨기기
    void HideComboEffect(void);     // 글자 이펙트
    void HideComboEffectBG(void);   // 배경 이펙트

    // 이펙트 표시 종류별 함수
    void Show_Scale_Blend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow);        // 순차적으로 스케일, 블랜딩이 진행되는 함수
    void Show_ScaleWidthBlend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow);    // 스케일과 블랜드가 동시에 진행되는 함수
    void Show_Pos_ComboEffect_Blend(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow
                                    , EGCCOMBOEFFECT enCalledComboEffect);  // 순차적으로 위치이동, 콤보이펙트호출, 블린드가 진행되는함수

protected:
    bool        m_bIniValue;
    bool        m_bComboStarted;                // 콤보 사용 여부
    KSafeArray<int,NUM_OF_EGC_CE> m_iComboState;   // 콤보 표시 상태를 저장하는 변수

#if !defined( SEASON3_COMBO_SYSTEM )
    int         m_iPerfectCombo;
#endif
    int         m_iOldCombo;

    KD3DWnd*                 m_pkGood;
    KGCD3DWndEffect          m_stGood;

    KD3DWnd*                 m_pkGreat;
    KGCD3DWndEffect          m_stGreat;

    KD3DWnd*                 m_pkWonderful;
    KGCD3DWndEffect          m_stWonderful;

    KD3DWnd*                 m_pkPerfect;
    KGCD3DWndEffect          m_stPerfect;

    KD3DWnd*                 m_pkBreak;
    KGCD3DWndEffect          m_stBreak;

    KD3DWnd*                 m_pkBackYellow;
    KGCD3DWndEffect          m_stBackYellow;

    KD3DWnd*                 m_pkBackRed;
    KGCD3DWndEffect          m_stBackRed;

    KD3DWnd*                 m_pkGood_AfterImage;
    KGCD3DWndEffect          m_stGood_AfterImage;

    KD3DWnd*                 m_pkGreat_AfterImage;
    KGCD3DWndEffect          m_stGreat_AfterImage;

    KD3DWnd*                 m_pkWonderful_AfterImage;
    KGCD3DWndEffect          m_stWonderful_AfterImage;

    KD3DWnd*                 m_pkPerfect_AfterImage;
    KGCD3DWndEffect          m_stPerfect_AfterImage;

    KD3DWnd*                 m_pkBreak_AfterImage;
    KGCD3DWndEffect          m_stBreak_AfterImage;

    EGCCOMBOEFFECT      m_enComboEffect;
    EGCCOMBOEFFECT_BG   m_enComboEffectBG;

    // 콤보 종류별 FrameMove
    void Good(void);
    void Great(void);
    void Wonderful(void);
    void Perfect(void);
    void Break(void);
    void BackYellow(void);
    void BackRed(void);
    void Good_AfterImage();
    void Great_AfterImage();
    void Wonderful_AfterImage();
    void Perfect_AfterImage();
    void Break_AfterImage();
};


DEFINE_WND_FACTORY( KGCComboEffect );
DECLARE_WND_FACTORY( KGCComboEffect );
DECLARE_WND_FACTORY_NAME( KGCComboEffect );

#endif // _GCCOMBOEFFECT_H_
