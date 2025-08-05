#pragma once
#include <vector>
#include "GCCollisionRect.h"
#include "GCDeviceTexture.h"
#include "GCDeviceLib/GCDeviceVertexBuffer.h"


class KGC2DObject
{
public:
	KGC2DObject(void);
	~KGC2DObject(void);
private:

	D3DXVECTOR2	m_vPos;

	D3DXVECTOR4 m_vBoundRect;
	DWORD		m_dwBoundWidth;
	DWORD		m_dwBoundHeight;

	int			m_iDamageType;
	bool		m_bHit;
	bool		m_bCrashed;
	bool		m_bHitCheck;
	bool		m_bPushCheck;
	bool		m_bTeleport;
	bool		m_bPlayerPushed;
	bool		m_bBounceCheckWithMap;
	bool		m_bMoveWithCamera;	//카메라의 움직임과 함께 따라다님 UI와 동일함.
	bool		m_bDynamicFrameMove;
	float		m_fMaxLeftX, m_fMaxRightX;	//아이템이 최대로 이동 가능한 X좌표, 맵은 크기보다 작거나 같으면 됨
	float		m_fXspeed,m_fYspeed;	//아이템 움직임에 물리 적용하기.
	float		m_fXgravity, m_fYgravity;	// 중력 적용하기
	float       m_fXrotation, m_fYrotation, m_fZrotation;
	float		m_fXrotationSpd, m_fYrotationSpd, m_fZrotationSpd;	//회전 속도!! 한번 정하면 계속 돕니다.
	float       m_fXscale, m_fYscale;
	float       m_fXtrans, m_fYtrans;
	float		m_fXaxisOffset, m_fYaxisOffset, m_fZaxisOffset;
	float		m_fFrictionalForceX, m_fFrictionalForceY;	// 마찰력 ( 1.0f ~ 0.0f ) 까지의 값을 가집니다.
	BYTE		m_cAlpha, m_cRed, m_cGreen, m_cBlue;
	float		m_fTexPosX,m_fTexPosY,m_fTexSizeX,m_fTexSizeY;

	GCCollisionRect<float>	m_kCollisionRect;

	//그려질 텍스쳐들
	std::vector< GCDeviceTexture* >		m_vecTex;
	int m_iCurrTexIndex;

	static GCDeviceVertexBuffer*	m_pVertex;

    float       m_fObjOffsetX;

    bool        m_bAgitShopObject;
    int         m_iMoneyType;
    int         m_iPrice;
    int         m_iNowCount;
    int         m_iMaxCount;
    D3DXVECTOR2 m_dxvPricePos;

    GCDeviceTexture* m_pkTexPriceback;
    GCDeviceTexture* m_pkTexPriceIcon;
    D3DXVECTOR4      m_dxvIconUV;
    D3DXVECTOR4      m_dxvBackUV;

    CParticleEventSeqPTR    m_pParticle;    // 아지트 오브젝트 파티클 
    std::string m_strParticleSquence;   // 파티클 이름
    D3DXVECTOR2 m_dxvParticlePos;       // 파티클 위치 

public:
	bool	IsSetTexture(){return !m_vecTex.empty();}

	static void BindLua();

	void FrameMove_Origin();

	virtual void FrameMove();
	virtual void FrameMove_Dynamics();
	virtual void Render();

    void SetPhysics( bool bPhysics_ ) { m_bDynamicFrameMove = bPhysics_; }
    bool IsPhysics() { return m_bDynamicFrameMove; }

	void SetFrictionalForce( float fx, float fy )	{ SetFrictionalForceX( fx ); SetFrictionalForceY( fy ); }
	void SetFrictionalForceX( float fForce );
	void SetFrictionalForceY( float fForce );

	void SetPositionX( float fx )	{ m_vPos.x = fx; };
	void SetPositionY( float fy )	{ m_vPos.y = fy; };
	void SetPosition( float fx, float fy ); 

    void SetPositionPixel( float fx, float fy );

	void SetSize( float fWidth, float fHeight )	{ m_fXscale = fWidth / (400.f * GC_SCREEN_DIV_WIDTH); m_fYscale = fHeight/(400.f * GC_SCREEN_DIV_HEIGHT); };
	void SetWidth( float fWidth )					{ m_fXscale = fWidth/(400.f * GC_SCREEN_DIV_WIDTH); };
	void SetHeight( float fHeight )				{ m_fYscale = fHeight/(400.f * GC_SCREEN_DIV_HEIGHT); };

	void SetScale( float fx, float fy )		{ m_fXscale = fx; m_fYscale = fy; }

	void SetSpeed( float fx, float fy )	{ m_fXspeed = fx; m_fYspeed = fy; };
	void SetSpeedX( float fx )			{ m_fXspeed = fx; }
	void SetSpeedY( float fy )			{ m_fYspeed = fy; }
	D3DXVECTOR2 GetSpeed()	{ return D3DXVECTOR2( m_fXspeed, m_fYspeed ); }

	void SetGravity( float fx, float fy )	{ m_fXgravity = fx; m_fYgravity = fy; };
	void SetGravityX( float fx )			{ m_fXgravity = fx; };
	void SetGravityY( float fy )			{ m_fYgravity = fy; };

	void SetRotation( float fx, float fy, float fz )		{ m_fXrotation = fx; m_fYrotation = fy; m_fZrotation = fz; }
	void SetRotationX( float fx )							{ m_fXrotation = fx; };
	void SetRotationY( float fy )							{ m_fYrotation = fy; };
	void SetRotationZ( float fz )							{ m_fZrotation = fz; };

	void SetRotationSpeed( float fx, float fy, float fz )	{ m_fXrotationSpd = fx; m_fYrotationSpd = fy; m_fZrotationSpd = fz; };
	void SetRotationSpeedX( float fx )						{ m_fXrotationSpd = fx; };
	void SetRotationSpeedY( float fy )						{ m_fYrotationSpd = fy; };
	void SetRotationSpeedZ( float fz )						{ m_fZrotationSpd = fz; };

	void SetAxisOffset( float fx_, float fy_, float fz_ )
	{
		m_fXaxisOffset = fx_; m_fYaxisOffset = fy_; m_fZaxisOffset = fz_;
	}
    void SetTransOffset( float fx_, float fy_ ) {
        m_fXtrans = fx_;
        m_fYtrans = fy_;
    }

	void SetItemVertex( BYTE Alpha = 0xff );
	void SetAlpha( int iAlpha );
	void SetRGB( BYTE R, BYTE G, BYTE B );
    BYTE GetAlpha() { return m_cAlpha; }
    void GetRGB( BYTE& R, BYTE& G, BYTE& B );

	int AddTexture( const char* strID );
	int AddTexture( GCDeviceTexture* pTexture );
	int RemoveAllTexture();
	int RemoveTexture( int iIndex );

	void SetCurrTexIndex( int iIndex);
	int GetCurrTexIndex()	{ return m_iCurrTexIndex; };
    GCDeviceTexture* GetCurrTex( void );

	void CameraMatrixOff()	{ m_bMoveWithCamera = false; };
	void CameraMatrixOn()	{ m_bMoveWithCamera = true;  };

	void SetBouceCheckWithMap( bool bCheck )	{ m_bBounceCheckWithMap = bCheck; };

	virtual bool Damaged( GCCollisionRect<float>& weapon, float fAttackPoint /*= 0.0f*/, int iDamageType /*= -1*/, int iAttackerIndex /*= -1*/, bool bMonsterDamage /*= false*/ );
	GCCollisionRect<float> GetCollisionRect();
	virtual void PushCheck( float& fx, float fy, bool bTeleport );

	D3DXVECTOR2	GetPos()	{ return m_vPos; };
    float       GetPosY()   { return m_vPos.y; };
    float       GetPosX()   { return m_vPos.x; };
	float		GetHeight()	{ return m_fYscale; };
	float		GetWidth()	{ return m_fXscale; };

    void SetObjectOffsetX( float fOffsetX ) { m_fObjOffsetX = fOffsetX; }
    float GetObjectOffsetX( void ) { return m_fObjOffsetX; }

    void SetAgitShop( bool bSet ) { m_bAgitShopObject = bSet; }
    void SetAgitShopInfo( int iMoneyType, int iPrice, D3DXVECTOR2& dxvPricePos, int iNowCount, int iMaxCount );

    bool IsAgitshopObject( void ) { return m_bAgitShopObject; }
    int GetAgitShopMoneyType( void ) { return m_iMoneyType; }
    int GetAgitShopPrice( void ) { return m_iPrice; }

private:
	D3DXVECTOR2*	GetPosition();
	int _AddTexture( const char* strID );

    // 아지트용으로 추가 
    void SetCrashPos( void );
};
