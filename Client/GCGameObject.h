#ifndef _GCGAMEOJBECT_H_
#define _GCGAMEOJBECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <string>
//#include "define.h"
//#include "KAssert.h"
#include "ProtectedType.h"
//#include "GCEnum.h"
#include "GCCollisionRect.h"

class KGCGameObject
{
public:
    KGCGameObject( void );
    EMPTY_VIRTUAL_DESTRUCTOR( KGCGameObject );

private:
    UNUSED_COPY_CONSTRUCTOR( KGCGameObject );
    UNUSED_ASSIGNMENT_OPERATOR( KGCGameObject );

public:
    virtual void Create( void ) = 0;
    virtual void Destroy( void ) = 0;
    virtual void Enable( void ) = 0;
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME ) = 0;
    virtual void Render( void ) = 0;

public:
    virtual void SetPosX( float fPosX ) { m_fPosX = fPosX; }
    virtual float GetPosX( void ) const { return m_fPosX; }
    virtual void SetPosY( float fPosY ) { m_fPosY = fPosY; }
    virtual float GetPosY( void ) const { return m_fPosY; }
    virtual void SetWidth( float fWidth ) { m_fWidth = fWidth; }
    virtual void SetHeight( float fHeight ) { m_fHeight = fHeight; }
    virtual float GetWidth( void ) { return m_fWidth; }
    virtual float GetHeight( void ) { return m_fHeight; }
    inline void SetObjType( EGCObjType eObjType_ ) { m_eObjType = eObjType_; }
    inline EGCObjType GetObjType( void ) const { return m_eObjType; }
    virtual void EnableLive( bool bLive ) { m_bLive = bLive; }
    virtual bool IsLive( void ) const { return m_bLive; }
    inline void SetIndex( int iIndex ) { m_iIndex = iIndex; }
    inline int GetIndex( void ) const { return m_iIndex; }
    inline void InitEnable( bool bInit ) { m_bInitEnable = bInit; }
    inline bool IsInitEnable( void ) const { return m_bInitEnable; }
    virtual bool IsCrashCheck(void) const { return m_bCrashCheck; }
    inline void SetCrashCheck(bool bCrashCheck) { m_bCrashCheck = bCrashCheck; }
    virtual GCCollisionRect<float> GetCollisionRect( void );
    virtual bool OnDamage( int iPlayerIndex, float fAttackPower
                            , EGCAttackType eAttackType, EGCAttackDirection eAttackDirection
                            , float fPushTargetX, float fPushTargetY, bool bSpecialAttack ) { return true; }

protected:
    bool            m_bCrashCheck;      // 캐릭터에게 밀리느냐 마느냐를 체크한다.
    float           m_fPosX;
    float           m_fPosY;
    float           m_fWidth;
    float           m_fHeight;
    EGCObjType      m_eObjType;
    bool            m_bLive;
    bool            m_bInitEnable;
    int             m_iIndex;
};

class KGCGameObjectFactory
{
public:
    EMPTY_DEFAULT_CONSTRUCTOR( KGCGameObjectFactory );
    EMPTY_VIRTUAL_DESTRUCTOR( KGCGameObjectFactory );

private:
    UNUSED_COPY_CONSTRUCTOR( KGCGameObjectFactory );
    UNUSED_ASSIGNMENT_OPERATOR( KGCGameObjectFactory );

public:
    virtual KGCGameObject* CreateObject( void ) const = 0;
};

#define DEFINE_GAME_OBJ_FACTORY(o) \
        class o##Factory : public KGCGameObjectFactory \
        { \
        public: \
            virtual KGCGameObject* CreateObject( void ) const \
            { \
                return new o##(); \
            } \
        }

#define DECLARE_GAME_OBJ_FACTORY(o)          extern o##Factory g_##o##Factory
#define IMPLEMENT_GAME_OBJ_FACTORY(o)        o##Factory g_##o##Factory

#define DECLARE_GAME_OBJ_FACTORY_NAME(o)     extern char g_sz##o##FactoryName[]
#define IMPLEMENT_GAME_OBJ_FACTORY_NAME(o,s) char g_sz##o##FactoryName[] = (s)

#define GET_GAME_OBJ_FACTORY_NAME(o)         (g_sz##o##FactoryName)
#define GET_GAME_OBJ_FACTORY_PTR(o)          (&g_##o##Factory)

#endif // _GCGAMEOJBECT_H_