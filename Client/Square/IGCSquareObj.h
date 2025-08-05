#pragma once
// [12/27/2007] breadceo. 
// 광장 오브젝트임. 나중에 케릭터 말고 다른 애들 뛰어댕기거나 할까봐..

#include <d3dx9math.h>
#include "SquareEnum.h"
#include "Packet.h"
#include "MyD3D.h"

class IGCSquareObj : public KGCObj
{
public:
    IGCSquareObj(void)
        : m_eType(SOT_INVALID)
        , m_eSubType(SOST_INVALID)
        , m_bInit( false )
        , m_bRender( false )
    {
        m_BasicData.Init();
        ZeroMemory( &m_vRect, sizeof(D3DXVECTOR2) );
    }
    virtual ~IGCSquareObj(void) {}

    virtual void FrameMove() {}
    virtual void Render() {}
    virtual void ShadowRender() {}
    virtual bool Load() = 0;
    virtual bool TimeShareLoading() = 0;
    virtual bool Picking( const D3DXVECTOR2& vMousePos , const int iMouseBtn ) { return false; }

    // getter/setter
    SQUARE_OBJ_TYPE GetType() const { return m_eType; }
    void SetType( SQUARE_OBJ_TYPE eType ) { m_eType = eType; }
    SQUARE_OBJ_SUBTYPE GetSubType() const { return m_eSubType; }
    void SetSubType( SQUARE_OBJ_SUBTYPE eSubType ) { m_eSubType = eSubType; }
    D3DXVECTOR3 GetPos() const { return m_BasicData.vPos; }
    void SetPos( const D3DXVECTOR3& vPos ) { m_BasicData.vPos = vPos; }
    bool GetInit() const { return m_bInit; }
    void SetInit( bool bInit ) { m_bInit = bInit; }
    bool GetIsRight() const { return m_BasicData.bIsRight; }
    void SetIsRight( bool bIsRight ) { m_BasicData.bIsRight = bIsRight; }
    USHORT GetFrame() const { return m_BasicData.cFrame; }
    void SetFrame( USHORT ucFrame ) { m_BasicData.cFrame = ucFrame; }
    UINT GetMotion() const { return m_BasicData.usMotion; }
    void SetMotion( UINT uiMotion ) { m_BasicData.usMotion = uiMotion; }
    bool GetRender() const { return m_bRender; }
    virtual void SetRender( bool bRender ) { m_bRender = bRender; }
    void SetBodyRect( const D3DXVECTOR2& vRect ) { m_vRect = vRect; }
    KRenderData* GetRenderData() { return &m_BasicData; }

private:
	D3DXVECTOR3*	GetPosition() { return &m_BasicData.vPos; }
public:
	static void BindLua()
    {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
		lua_tinker::class_add<IGCSquareObj>( L, "IGCSquareObj" );
			//	lua_tinker::class_mem<>( L, "iCurrTexIndex", 	&GCSquare::m_iCurrTexIndex );
			lua_tinker::class_def<IGCSquareObj>( L, "GetPos", 		&IGCSquareObj::GetPosition );;
	}

protected:
    SQUARE_OBJ_TYPE m_eType;
    SQUARE_OBJ_SUBTYPE m_eSubType;
    bool        m_bInit;
    KRenderData m_BasicData;
    bool        m_bRender;
    D3DXVECTOR2 m_vRect;
};
