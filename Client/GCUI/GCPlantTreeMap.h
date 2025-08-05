#pragma once
#include <stdafx.h>

class KGCPlantTreeMap : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPlantTreeMap );
    /// Default constructor
    KGCPlantTreeMap( void );
    /// Default destructor
    virtual ~KGCPlantTreeMap( void );
    /// Copy constructor
    KGCPlantTreeMap( const KGCPlantTreeMap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlantTreeMap& operator=( const KGCPlantTreeMap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetWorldMapIndex( int iIndex );
    enum EPlantTreeWorld{
        EPTW_BERMESIAH = 1,//   -- 베르메시아
        EPTW_SILVER_LAND = 2,// -- 실버랜드
        EPTW_ELLIA = 3,//       -- 엘리아 
        EPTW_ARCHIMEDIA = 4,//        -- 아케메디아   
        EPTW_NUM
    };
private:
    std::map<int,KD3DWnd*> m_mapWorldMapWnd;
};

DEFINE_WND_FACTORY( KGCPlantTreeMap );
DECLARE_WND_FACTORY( KGCPlantTreeMap );
DECLARE_WND_FACTORY_NAME( KGCPlantTreeMap );
