
#ifndef _GCTUTORIALOPTIONNEW_H_
#define _GCTUTORIALOPTIONNEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../gcui/KGCTutorialChar.h"
#include "../gcui/KGCTutorialMap.h"

class KGCNewTutorialOption : public KD3DDialog, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCNewTutorialOption );
    /// Default constructors
    KGCNewTutorialOption( void );
    /// Default destructor
    virtual ~KGCNewTutorialOption( void );
    /// Copy constructor
    KGCNewTutorialOption( const KGCNewTutorialOption& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNewTutorialOption& operator=( const KGCNewTutorialOption& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum ETutorialOptionWndResult
    {
        CANCLE,
        STARTTUTORIAL,
    };
    enum
    {
        OFF_SKILL,
        ON_SKILL,
    };

    void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroyComplete( void );
    virtual void PostChildDraw( void );

protected:
    void OnClickOk(void);
    void OnClickClose(void);
    void OnClickSkill(void);
    void InitTutorialChar( void );
    void SetTutorialCharacter();
    void ReleaseCharResource();
    void SetTutorialCharSound();
    void LoadingTutorial(void);
    void OnClickSubBtnSkillA();
    void OnClickSubBtnSkillB();

public:
    void LoadTutorial( void );    
    void SetCancle(bool bCancle){   m_bSkillTreeCancle = bCancle;   }
    void RenderSubSkillBtn( bool bRender_ );

    KGCTutorialChar* GetCharSelectBox()     { return m_pkChar; }

protected:
    KD3DWnd*       m_pkWndBackGround;
    KD3DWnd*       m_pkButtonOk;
    KD3DWnd*       m_pkButtonCancel;
    KD3DWnd*       m_pkButtonSkill;
    KD3DWnd*       m_pkWhiteBackground;
    KD3DWnd*       m_pkSubBtnSkillA;
    KD3DWnd*       m_pkSubBtnSkillB;

    KGCTutorialMap*m_pkMap;
    KGCTutorialChar*m_pkChar;

    bool m_bFirstTutorial;
    bool m_bSkillTreeCancle;
    int m_RenderLoading;
    int m_iMySelCharIndex;
    int m_iJobState;
    int m_iExcharMySelCharIndex;
    int m_iExcharJobState;
    int m_iBackgroundAlphaCounter;
    bool m_bSkillTreeClose;

    bool m_bIsSelectedSkillB;

private:
    GCDeviceTexture*    m_pTutorialLoadingBG;
    GCDeviceTexture*    m_pTutorialLoadingDiscBG;
    GCDeviceTexture*    m_pTutorialLoadingChar;
    GCDeviceTexture*    m_pTutorialLoadingDisc;

public:
    int GetMySelectCharID() { return m_iMySelCharIndex; }
    int GetMySelectJobID() { return m_iJobState; }
    void ToggleSkillTreeBackGround( bool bRender );
    void SetSkillTreeState(bool bSkillTreeClose) { m_bSkillTreeClose = bSkillTreeClose;}
};

DEFINE_WND_FACTORY( KGCNewTutorialOption );
DECLARE_WND_FACTORY( KGCNewTutorialOption );
DECLARE_WND_FACTORY_NAME( KGCNewTutorialOption );

#endif  // _GCTUTORIALOPTIONNEW_H_
