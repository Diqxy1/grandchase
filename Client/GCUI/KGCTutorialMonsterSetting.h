#ifndef _KGCTUTORIALMONSTERSETTING_H_
#define _KGCTUTORIALMONSTERSETTING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "GCEnum.h"

class KD3DStatic;

class KGCTutorialMonsterSetting : public KD3DWnd,
                                  public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCTutorialMonsterSetting );
    /// Default constructor
    KGCTutorialMonsterSetting( void );
    /// Default destructor
    virtual ~KGCTutorialMonsterSetting( void );
    /// Copy constructor
    KGCTutorialMonsterSetting( const KGCTutorialMonsterSetting& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTutorialMonsterSetting& operator=( const KGCTutorialMonsterSetting& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*    m_pkMonsterChangeLeft;
    KD3DWnd*    m_pkMonsterChangeRight;
    KD3DWnd*    m_pkMonsterLevelChangeLeft;
    KD3DWnd*    m_pkMonsterLevelChangeRight;
    KD3DStatic* m_pMonsterName;
    KD3DStatic* m_pMonsterAiFileName;
    KD3DStatic* m_pMonsterLevel;
    KD3DStatic* m_pMonsterIndex;

    void OnMonsterChangeLeft();
    void OnMonsterChangeRight();
    void OnMonsterLevelChangeLeft();
    void OnMonsterLevelChangeRight();

    int m_iSelectMonsterIndex;
    int m_iMonsterLevel;

    // 챔피언 몬스터 속성 
#define CHAMPION_PROPERTY_SIZE (10)
    KSafeArray<KD3DWnd*,CHAMPION_PROPERTY_SIZE> m_apkCheckBox;
    KSafeArray<KD3DStatic*,CHAMPION_PROPERTY_SIZE> m_apkCheckText;
    KSafeArray<KD3DWnd*,CHAMPION_PROPERTY_SIZE> m_apkCheck;
    DWORD       m_adwProperty[CHAMPION_PROPERTY_SIZE];

    KD3DWnd*    m_pkCheckBoxAll;
    KD3DStatic* m_pkCheckTextAll;
    KD3DWnd*    m_pkCheckAll;

public:
    int GetMonsterIndex(){ return m_iSelectMonsterIndex; };
    int GetMonsterLevel(){ return m_iMonsterLevel; };
    void UpdateMonsterPanel();

    DWORD GetChampionProperty();
};

DEFINE_WND_FACTORY( KGCTutorialMonsterSetting );
DECLARE_WND_FACTORY( KGCTutorialMonsterSetting );
DECLARE_WND_FACTORY_NAME( KGCTutorialMonsterSetting );

#endif  // _KGCTUTORIALMONSTERSETTING_H_
