#ifndef _GCAGIT_OBJECT_ACTION_H_
#define _GCAGIT_OBJECT_ACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ProtectedType.h"
#include "GCCollisionRect.h"

class KGCAgitObjectAction
{
public:
    KGCAgitObjectAction( void );
    EMPTY_VIRTUAL_DESTRUCTOR( KGCAgitObjectAction );

private:
    UNUSED_COPY_CONSTRUCTOR( KGCAgitObjectAction );
    UNUSED_ASSIGNMENT_OPERATOR( KGCAgitObjectAction );

public:
    virtual void Create( void ) = 0;
    virtual void Destroy( void ) = 0;
    virtual void Enable( void ) = 0;
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME ) = 0;
    virtual void Render( void ) = 0;
    virtual void DoAction( const std::vector<DWORD>& vecParam ) = 0;

public:
    virtual void SetPosX( float fPosX ) { m_fPosX = fPosX; }
    virtual void SetPosY( float fPosY ) { m_fPosY = fPosY; }
    
    virtual float GetPosX( void ) const { return m_fPosX; }
    virtual float GetPosY( void ) const { return m_fPosY; }

    virtual void SetWidth( float fWidth ) { m_fWidth = fWidth; }
    virtual void SetHeight( float fHeight ) { m_fHeight = fHeight; }
    
    virtual float GetWidth( void ) { return m_fWidth; }
    virtual float GetHeight( void ) { return m_fHeight; }
    
    inline void SetObjType( EM_AGIT_OBJECT_ACTION eObjType_ ) { m_eObjType = eObjType_; }
    inline EM_AGIT_OBJECT_ACTION GetObjType( void ) const { return m_eObjType; }
    
    virtual void EnableLive( bool bLive ) { m_bLive = bLive; }
    virtual bool IsLive( void ) const { return m_bLive; }
    
    virtual void InitEnable( bool bInit ) { m_bInitEnable = bInit; }
    virtual bool IsInitEnable( void ) const { return m_bInitEnable; }

    virtual GCCollisionRect<float> GetCollisionRect( void );

    virtual DWORD GetUserUID() { return UINT_MAX; }
    
    virtual void SetItemUID( const GCITEMUID& _itemUID ) { m_dwItemUID = _itemUID; }
    virtual GCITEMUID GetItemUID() { return m_dwItemUID; }


protected:
    EM_AGIT_OBJECT_ACTION      m_eObjType;
   
    float           m_fPosX;
    float           m_fPosY;
    float           m_fWidth;
    float           m_fHeight;
    bool            m_bLive;
    bool            m_bInitEnable;

    GCITEMUID       m_dwItemUID;
};


#endif // _GCAGIT_OBJECT_ACTION_H_