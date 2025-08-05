#pragma once

//#include <vector>
#include <KNCSingleton.h>
//#include "../GCLuaManager/KLuabinder.h"
#include "../GCLuaManager/KLuaManager.h"
#include "../GCDeviceLib/GCDeviceManager.h"

#define FHTERM  0.4f

// T0 : FHVector, T1 : FH
template <class T1>
class KMap
{
public:
    KMap(void);
    virtual ~KMap(void);

    // typedefs
    typedef typename T1 foothold_type;

    virtual void                    RegisterLuabind() = 0;
    virtual void                    Render() = 0;
    virtual void                    FrameMove( float fElapsedTime ) = 0;
    virtual void                    Load() = 0;
    virtual void                    UnLoad() = 0;
    virtual void                    AddDungeonFootHold( KLuaManager& luaMgr ) = 0;
    virtual void                    AddDungeonFootHoldByValue( float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue ) = 0;
    virtual void                    RemoveDungeonFootHold( float PosX, float PosY, void* pParent ) = 0;

    virtual void                    LoadBaseData( KLuaManager& luaMgr ) = 0;

    virtual void                    AddFootHold( foothold_type* _iFootHold );

    virtual inline int              GetBlockWidth() { return m_iFHWidth; }
    virtual inline int              GetBlockHeight() { return m_iFHHeight; }
    virtual inline float            GetPixelWidth() { return m_fWidth; }
    virtual inline float            GetPixelHeight() { return m_fHeight; }
    virtual inline float            GetQuestWidth() { return m_fWidth * 400.0f; }
    virtual inline float            GetQuestHeight() { return m_fHeight * 400.0f; }

    virtual void                    SetBlockWidth( int Width ) = 0;
    virtual void                    SetBlockHeight( int Height ) = 0;

    inline void                     BoundCheck( int& iIndexX, int& iIndexY )
    {
        if( iIndexX <= 0 )
            iIndexX = 1;
        if( iIndexY <= 0 )
            iIndexY = 1;
        if( iIndexX > m_iFHWidth - 1 )
            iIndexX = m_iFHWidth - 1;
        if( iIndexY > m_iFHHeight - 1 )
            iIndexY = m_iFHHeight - 1;
    }

    virtual T1**                    GetFHWorld() { return m_ppFHWorld; }

    virtual void                    SetWorld( int iWidth, int iHeight ) = 0;

    virtual void                    LoadFootHoldData( KLuaManager& luaMgr );

protected:
    // FootHold Pixel Limit
    float                           m_fWidth;
    float                           m_fHeight;

    // FootHold Limit Num
    int                             m_iFHWidth;
    int                             m_iFHHeight;

    // 발판 관련
    foothold_type**                 m_ppFHWorld;
};

#include "KMap.inl"