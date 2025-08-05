#ifndef _GCFOODGAUGE_H_
#define _GCFOODGAUGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BAR_SIZE 256
#define BAR_START_X 80

class KGCSizingThreeBox;
class KGCFoodGauge : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCFoodGauge );
    /// Default constructor
    KGCFoodGauge( void );
    /// Default destructor
    virtual ~KGCFoodGauge( void );
    /// Copy constructor
    KGCFoodGauge( const KGCFoodGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFoodGauge& operator=( const KGCFoodGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void ){};
    virtual void ActionPerformed( const KActionEvent& event ){};

public:
    void InitData();
    void SetFoodGauge( int iMaxFood, int iFoodEnum, int iFoodType, int iGetFoodEnum, int iGetFoodType );
    void UpDateGauge();

public:
    enum
    {
        TYPE_FOOD_DAMAGE,
        TYPE_FOOD_PLAYER_BUFF,
        TYPE_FOOD_MONSTER_BUFF,
        TYPE_FOOD_PLAYER_MONSTER_BUFF,
        TYPE_FOOD_SYNCOBJECT,
    };
    enum
    {
        TYPE_GETFOOD_DAMAGE,
        TYPE_GETFOOD_PLAYER_BUFF,
        TYPE_GETFOOD_MONSTER_BUFF,
        TYPE_GETFOOD_PLAYER_MONSTER_BUFF,
        TYPE_GETFOOD_SYNCOBJECT,
    };

private:
    KD3DWnd             *m_pkFoodNumBack;
    KD3DStatic          *m_pkGetFoodNumStr;
    KD3DStatic          *m_pkFoodNumStr;
    KD3DWnd             *m_pkBlueGauge;
    KGCSizingThreeBox   *m_pkRedGauge;
    
    KD3DWnd *m_pkNumber1[10];
    KD3DWnd *m_pkNumber10[10];

private:
    int             m_iMaxFood;         // 음식 수
    int             m_iFoodEnum;        // 음식 번호
    int             m_iFoodType;        // 음식 표현방식
    int             m_iGetFoodEnum;     // 먹은 음식 번호
    int             m_iGetFoodType;     // 먹은 음식 표현방식
    int             m_iCheckLifeTime;   // 임의 값

private:
    int             m_iOldFoodCnt;
    int             m_iOldGetFoodCnt;
    bool            m_bFoodGaugeFirst;
};

DEFINE_WND_FACTORY( KGCFoodGauge );
DECLARE_WND_FACTORY( KGCFoodGauge );
DECLARE_WND_FACTORY_NAME( KGCFoodGauge );

#endif  // _GCFOODGAUGE_H_