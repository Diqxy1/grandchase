#ifndef _GCKGCDOTAPOINTOVERLAY_H_
#define _GCKGCDOTAPOINTOVERLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCDotaPointOverLay : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaPointOverLay );
    /// Default constructor
    KGCDotaPointOverLay( void );
    /// Default destructor
    virtual ~KGCDotaPointOverLay( void );
    /// Copy constructor
    KGCDotaPointOverLay( const KGCDotaPointOverLay& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaPointOverLay& operator=( const KGCDotaPointOverLay& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

    enum
    {
        ITEM_ATK,
        ITEM_HP,
        ITEM_CRI,
        ITEM_NPCATK,
        ITEM_NPCHP,
        NUM_ITEM,
    };

private:
    int                   m_iMyPoint;
    KD3DStatic            *m_pkStaticMyPoint;
    KD3DStatic            *m_pkStaticItem[5];
    SDotaTemplate         *m_pCurDotaInfo;
};

DEFINE_WND_FACTORY( KGCDotaPointOverLay );
DECLARE_WND_FACTORY( KGCDotaPointOverLay );
DECLARE_WND_FACTORY_NAME( KGCDotaPointOverLay );

#endif  // _GCKGCDOTAPOINTOVERLAY_H_




#ifndef _GCKGCDOTACOREHPBAR_H_
#define _GCKGCDOTACOREHPBAR_H_

#define MAX_HP_WND_SIZE 325.0f
#define CANABAN_HP_BAR_ORI_POS 6.0f

class KGCDotaCoreHpBar : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaCoreHpBar );
    /// Default constructor
    KGCDotaCoreHpBar( void );
    /// Default destructor
    virtual ~KGCDotaCoreHpBar( void );
    /// Copy constructor
    KGCDotaCoreHpBar( const KGCDotaCoreHpBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaCoreHpBar& operator=( const KGCDotaCoreHpBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd               *m_pkGauge[TEAM_NUM];
    SDotaTemplate         *m_pCurDotaInfo;
};

DEFINE_WND_FACTORY( KGCDotaCoreHpBar );
DECLARE_WND_FACTORY( KGCDotaCoreHpBar );
DECLARE_WND_FACTORY_NAME( KGCDotaCoreHpBar );

#endif  // _GCKGCDOTACOREHPBAR_H_


#ifndef _GCKGCDOTACOUNT_H_
#define _GCKGCDOTACOUNT_H_

#define MAX_COUNT 5

class KGCDotaCount : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaCount );
    /// Default constructor
    KGCDotaCount( void );
    /// Default destructor
    virtual ~KGCDotaCount( void );
    /// Copy constructor
    KGCDotaCount( const KGCDotaCount& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaCount& operator=( const KGCDotaCount& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd               *m_pkNumber[MAX_COUNT];
    int                   iNowCount;
    int                   iAlphaCnt;

public:
    void UpdateCount( int iCount = 0 );
};

DEFINE_WND_FACTORY( KGCDotaCount );
DECLARE_WND_FACTORY( KGCDotaCount );
DECLARE_WND_FACTORY_NAME( KGCDotaCount );

#endif  // _GCKGCDOTACOUNT_H_



#ifndef _GCKGCDOTAITEMIMG_H_
#define _GCKGCDOTAITEMIMG_H_

#include "GCItemImgWnd.h"

class KGCDotaItemImg : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaItemImg );
    /// Default constructor
    KGCDotaItemImg( void );
    /// Default destructor
    virtual ~KGCDotaItemImg( void );
    /// Copy constructor
    KGCDotaItemImg( const KGCDotaItemImg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaItemImg& operator=( const KGCDotaItemImg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void CreateItemImg();
    void DeleteItemImg();
    void ShowItemImg(int iGameItem);

private:
    int m_iAlpha;
    int m_iViewCoolTime;
    int m_iSelectItem;

    KGCItemImgWnd                 *m_pkImgSelectItem;
    KD3DStatic                    *m_pkItemDesc;
    std::vector<GCDeviceTexture*>  m_vecItemImg;
};

DEFINE_WND_FACTORY( KGCDotaItemImg );
DECLARE_WND_FACTORY( KGCDotaItemImg );
DECLARE_WND_FACTORY_NAME( KGCDotaItemImg );

#endif  // _GCKGCDOTAITEMIMG_H_


#ifndef _GCKGCDOTAGUIDE_H_
#define _GCKGCDOTAGUIDE_H_

#include "GCItemImgWnd.h"

class KGCDotaGuide : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaGuide );
    /// Default constructor
    KGCDotaGuide( void );
    /// Default destructor
    virtual ~KGCDotaGuide( void );
    /// Copy constructor
    KGCDotaGuide( const KGCDotaGuide& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaGuide& operator=( const KGCDotaGuide& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

    enum
    {
        GUIDE_TOP,
        GUIDE_MIDDLE,
        GUIDE_BOTTOM,
        MAX_GUIDE,
    };
    enum
    {
        GUIDE_BACK_RED,
        GUIDE_BACK_BLUE,
        MAX_COLOR,
    };
    enum
    {
        IMG_NORMAL,
        IMG_GUARDIANTOWER_RED,
        IMG_GUARDIANTOWER_BLUE,
        IMG_GUARDTOWER_RED,
        IMG_GUARDTOWER_BLUE,
        IMG_CORE,
        IMG_SHOP_RED,
        IMG_SHOP_BLUE,
        MAX_IMG,
    };
    enum
    {
        ANI_SPEED = 10,
        ANI_SHOW_GAP = 202,
    };

private:
    GCDeviceTexture               *m_pkTexImg[MAX_IMG];
    KGCItemImgWnd                 *m_pkWndImg[MAX_GUIDE];
    KD3DStatic                    *m_pkStatic[MAX_GUIDE];
    KD3DWnd                       *m_pkWndBack[MAX_COLOR][MAX_GUIDE];

private:
    int                           m_iNumAddGuidex;
    int                           m_NowGuideID[MAX_GUIDE];
    bool                          m_bShow;
    std::map<int, bool>           m_mapOneTimeShow;
    SDotaTemplate                 *m_pCurDotaInfo;

public:
    void AddGuide( int iGuideID );
    void ShowGuide( bool bShow ) { m_bShow = bShow; }
    bool GetShowGuide() { return m_bShow; }
    int GetAddGuideNum() { return m_iNumAddGuidex; }

private:
    void CreateTexture( void );
    void DestroyTexture( void );
    void SetGuideSlot( int iGuideID, int iSlotID );
    int GetGuideIconIndex( int iGuideID );
    int GetBackColor( int iGuideID );
    void SetOneTimeGuide();
    bool CheckDuplicationGuide( int iGuideID );
};

DEFINE_WND_FACTORY( KGCDotaGuide );
DECLARE_WND_FACTORY( KGCDotaGuide );
DECLARE_WND_FACTORY_NAME( KGCDotaGuide );

#endif  // _GCKGCDOTAGUIDE_H_