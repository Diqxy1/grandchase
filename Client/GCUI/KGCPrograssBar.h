#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KNCSingleton.h"
#define GAP_PG_PIXEL   2.0f

/*
    1. 텍스쳐
        @ SetTexture : 텍스쳐이름
        @ SetBG[XXX]UV : Front, Center, Back UV 좌표값
        @ SetBar[XXX]UV : Front, Center, Back UV 좌표값

    2. 출력위치 및 크기
        @ SetRenderPos : 출력위치
        @ SetWidth : 넓이
        @ SetHeight : 높이

    3. 게이지 상태
        @ SetDataMaxValue : 최대값 설정
        @ SetDataMinValue : 최소값 설정
        @ SetDataNowValue : 현재값 설정

    4. 월드 좌표계로 출력시 레이어 설정( 스크린좌표계 이용시 해당없음 )
        @ SetLayer : 공용 레이어 번호 사용

    @ BG와 BAR는 Center텍스쳐를 늘리는 방식으로 처리하고있음.
*/

struct RENDERINFO_NOWVALUE
{
    std::string strType;
    float fOffsetX;
    float fOffsetY;
    DWORD dwNumber;
    DWORD dwColor;

    RENDERINFO_NOWVALUE()
    {
        strType.clear();
        fOffsetX = 0.0f;
        fOffsetY = 0.0f;
        dwNumber = 0;
        dwColor = 0xffffffff;
    }
};

struct PG_ANIINFO
{
    float fMinLimit;
    float fMaxLimit;
    float fAniSpeed;
    int iKeepFrame;
    int iSoundID;
    bool bReturnAni;
    PG_ANIINFO()
        : fMinLimit( 0.0f )
        , fMaxLimit( 0.0f )
        , fAniSpeed( 0.0f )
        , iKeepFrame( 0 )
        , iSoundID( -1 )
        , bReturnAni( false )
    {}
    void Init()
    {
        fMinLimit = 0.0f;
        fMaxLimit = 0.0f;
        fAniSpeed = 0.0f;
        iKeepFrame = 0;
        iSoundID = -1;
        bReturnAni = false;
    }
};

struct PG_COLOR_ANIINFO
{
    D3DXVECTOR3 vMinLimit;
    D3DXVECTOR3 vMaxLimit;
    D3DXVECTOR3 vAniSpeed;
    int iKeepFrame;
    bool bReturnAni;
    PG_COLOR_ANIINFO()
        : vMinLimit( 0.0f, 0.0f, 0.0f )
        , vMaxLimit( 0.0f, 0.0f, 0.0f )
        , vAniSpeed( 0.0f, 0.0f, 0.0f )
        , iKeepFrame( 0 )
        , bReturnAni( false )
    {}
    void Init()
    {
        vMinLimit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
        vMaxLimit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
        vAniSpeed = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
        iKeepFrame = 0;
        bReturnAni = false;
    }
    void SetMinLimit( float fR, float fG, float fB )
    {   vMinLimit.x = fR; vMinLimit.y = fG; vMinLimit.z = fB;   }
    void SetMaxLimit( float fR, float fG, float fB )
    {   vMaxLimit.x = fR; vMaxLimit.y = fG; vMaxLimit.z = fB;   }
    void SetAniSpeed( float fR, float fG, float fB )
    {   vAniSpeed.x = fR; vAniSpeed.y = fG; vAniSpeed.z = fB;   }
    void SetAniInfo( int iKeepFrame_, bool bReturnAni_ )
    {
        iKeepFrame = iKeepFrame_;
        bReturnAni = bReturnAni_;
    }
};

struct POS_ANI
{
    D3DXVECTOR2 vMinLimit;
    D3DXVECTOR2 vMaxLimit;
    D3DXVECTOR2 vAniSpeed;
    int iKeepFrame;
    bool bReturnAni;
    POS_ANI()
        : vMinLimit( 0.0f, 0.0f )
        , vMaxLimit( 0.0f, 0.0f )
        , vAniSpeed( 0.0f, 0.0f )
        , iKeepFrame( 0 )
        , bReturnAni( false )
    {}
};

class KGCPrograssBar
{
public:
    enum
    {
        PG_FRONT, PG_CENTER, PG_BACK,
        PG_NUM,
    };
    enum
    {
        TARGET_UI         = 0,
        TARGET_WORLD      = 1, 
        TARGET_PLAYER     = 2, 
        TARGET_MONSTER    = 3,
        TARGET_EMBARK     = 4,
        TARGET_NUM,
    };
	enum 
	{
		NORMAL = 0,
		HIDDEN = 1,
		OPENING = 2,
		HIDDING = 3
	};

	static const float	m_fCheckDistance;
private:

	int			m_iNowState;
	D3DXVECTOR2		m_vHidTargetPos;
	D3DXVECTOR2		m_vOpenTargetPos;
	float			m_fStateAnitime;

    GCDeviceTexture* m_pBackTex;
    GCDeviceTexture* m_pBarTex;
    GCDeviceTexture* m_pAddBackTex;
    GCDeviceTexture* m_pAddFrontTex;
    D3DXVECTOR4      m_dx4BGUV[PG_NUM];
    D3DXVECTOR4      m_dx4BarUV[PG_NUM];
    D3DXVECTOR4      m_dx4AddBackTexUV;
    D3DXVECTOR4      m_dx4AddFrontTexUV;
    D3DXVECTOR4      m_dx4Pos;
    D3DXVECTOR4      m_dx4PosToBarOffset;
    D3DXVECTOR3      m_dx3ParticleTracePos;
    D3DXVECTOR4      m_dx4AddBackTexPos;
    D3DXVECTOR4      m_dx4AddFrontTexPos;
    int              m_iMinValue;
    int              m_iMaxValue;
    int              m_iNowValue;
    int              m_iTargetType;
    int              m_iTargetUnitIndex;
    int              m_iLayer;

    RENDERINFO_NOWVALUE m_kNumberInfo;
    std::map<int, CParticleEventSeqPTR>  m_mapParticle;

    int             m_iString;
    D3DXVECTOR2     m_vStringOffset;

public:
    int             m_iCoolTime;
    int             m_iNowTime;
    int             m_iAccValue;

public:
    bool            m_bIsRender;
    bool            m_bIsRenderNumber;
    bool            m_bIsAddFrontTexRender;
    bool            m_bIsAddBackTexRender;
    bool            m_bVertical;        // 세로로 차는 게이지
    bool            m_bVerticalEx;      // 세로로 차는 게이지 ( 텍스쳐 잘라서 그림 )

public:
    int             m_iBarAlpha;
    int             m_iBackAlpha;
    int             m_iAddTexFrontAlpha;
    int             m_iAddTexBackAlpha;
    float           m_fAddTexFrontScale;
    float           m_fAddTexBackScale;
    DWORD           m_dwAddTexFrontColor;
    DWORD           m_dwAddTexBackColor;
    D3DXVECTOR2     m_vNowPosAni;
    PG_ANIINFO      m_kBarAlphaAni;
    PG_ANIINFO      m_kBackAlphaAni;
    POS_ANI         m_kPosAni;

public:
    PG_ANIINFO      m_kAddTexFrontAlphaAni;
    PG_ANIINFO      m_kAddTexBackAlphaAni;
    PG_ANIINFO      m_kAddTexFrontScaleAni;
    PG_ANIINFO      m_kAddTexBackScaleAni;
    PG_COLOR_ANIINFO m_kAddTexFrontColorAni;
    PG_COLOR_ANIINFO m_kAddTexBackColorAni;

public:
    KGCPrograssBar();
    ~KGCPrograssBar();

public://텍스쳐설정
    void ClearAllResource();
	void SetOpenTargetPos(float fPosX_ , float fPosY_ ) { m_vOpenTargetPos = D3DXVECTOR2(fPosX_, fPosY_); }
	void SetHidTargetPos(float fPosX_ , float fPosY_ ) { m_vHidTargetPos = D3DXVECTOR2(fPosX_, fPosY_); }
	void CheckAndChangeState();
	int GetNowMyState()		{ return m_iNowState; }
	void SetNowMyState(int iState_ );	

    void SetDefaultTextureToScreen();
    void SetDefaultTextureToWorld();
    void SetBackTexture(char *szTexName);
    void SetBarTexture(char *szTexName);
    void SetBGFrontUV( float fFrontX, float fFrontY, float fFrontZ, float fFrontW )
    {
        m_dx4BGUV[PG_FRONT] = D3DXVECTOR4(fFrontX, fFrontY, fFrontZ, fFrontW);
    };
    void SetBGCenterUV( float fCenterX, float fCenterY, float fCenterZ, float fCenterW )
    {
        m_dx4BGUV[PG_CENTER] = D3DXVECTOR4(fCenterX, fCenterY, fCenterZ, fCenterW);
    };
    void SetBGBackUV( float fBackX, float fBackY, float fBackZ, float fBackW )
    {
        m_dx4BGUV[PG_BACK] = D3DXVECTOR4(fBackX, fBackY, fBackZ, fBackW);
    };
    void SetBarFrontUV( float fFrontX, float fFrontY, float fFrontZ, float fFrontW )
    {
        m_dx4BarUV[PG_FRONT] = D3DXVECTOR4(fFrontX, fFrontY, fFrontZ, fFrontW);
    };
    void SetBarCenterUV( float fCenterX, float fCenterY, float fCenterZ, float fCenterW )
    {
        m_dx4BarUV[PG_CENTER] = D3DXVECTOR4(fCenterX, fCenterY, fCenterZ, fCenterW);
    };
    void SetBarBackUV( float fBackX, float fBackY, float fBackZ, float fBackW )
    {
        m_dx4BarUV[PG_BACK] = D3DXVECTOR4(fBackX, fBackY, fBackZ, fBackW);
    };

public://추가 텍스쳐 설정
    void SetAddFrontTexture(char *szTexName);
    void SetAddFrontTextrueUV( float fX, float fY, float fZ, float fW )
    {
        m_dx4AddFrontTexUV = D3DXVECTOR4(fX, fY, fZ, fW);
    }
    void SetAddFrontTextrueRenderOffset(float fX, float fY)
    {
        m_dx4AddFrontTexPos.x = fX;
        m_dx4AddFrontTexPos.y = fY;
    }
    float GetAddFrontTextrueRenderPosX(void)          {  return m_dx4AddFrontTexPos.x;          }
    float GetAddFrontTextrueRenderPosY(void)          {  return m_dx4AddFrontTexPos.y;          }
    void SetAddFrontTextrueWidth(float fWidth)        {  m_dx4AddFrontTexPos.z = fWidth;        }
    void SetAddFrontTextrueHeight(float fHeight)      {  m_dx4AddFrontTexPos.w = fHeight;       }
    float GetAddFrontTextrueWidth(void)               {  return m_dx4AddFrontTexPos.z;          }
    float GetAddFrontTextrueHeight(void)              {  return m_dx4AddFrontTexPos.w;          }
    void SetAddFrontTextureToggleRender( bool bShow ) { m_bIsAddFrontTexRender = bShow;         }
    bool GetAddFrontTextureToggleRender()             { return m_bIsAddFrontTexRender;          }

    void SetVertical( bool bVertical )                { m_bVertical = bVertical; }
    bool GetVertical()                                { return m_bVertical; }
    void SetVerticalEx( bool bVerticalEx )            { m_bVerticalEx = bVerticalEx; }
    bool GetVerticalEx()                              { return m_bVerticalEx; }

    void SetAddBackTexture(char *szTexName);
    void SetAddBackTextrueUV( float fX, float fY, float fZ, float fW )
    {
        m_dx4AddBackTexUV = D3DXVECTOR4(fX, fY, fZ, fW);
    }
    void SetAddBackTextrueRenderOffset(float fX, float fY)
    {
        m_dx4AddBackTexPos.x = fX;
        m_dx4AddBackTexPos.y = fY;
    }
    float GetAddBackTextrueRenderPosX(void)          {  return m_dx4AddBackTexPos.x;          }
    float GetAddBackTextrueRenderPosY(void)          {  return m_dx4AddBackTexPos.y;          }
    void SetAddBackTextrueWidth(float fWidth)        {  m_dx4AddBackTexPos.z = fWidth;        }
    void SetAddBackTextrueHeight(float fHeight)      {  m_dx4AddBackTexPos.w = fHeight;       }
    float GetAddBackTextrueWidth(void)               {  return m_dx4AddBackTexPos.z;          }
    float GetAddBackTextrueHeight(void)              {  return m_dx4AddBackTexPos.w;          }
    void SetAddBackTextureToggleRender( bool bShow ) { m_bIsAddBackTexRender = bShow;          }
    bool GetAddBackTextureToggleRender()             { return m_bIsAddBackTexRender;           }

public://프로그래스바 출력위치
    void SetRenderPos(float fX, float fY)
    {
        m_dx4Pos.x = fX;
        m_dx4Pos.y = fY;
    }
    float GetRenderPosX(void)          {  return m_dx4Pos.x;          }
    float GetRenderPosY(void)          {  return m_dx4Pos.y;          }

    void SetPosToBarOffset(float fX, float fY, float fZ, float fW)
    {
        m_dx4PosToBarOffset.x = fX;
        m_dx4PosToBarOffset.y = fY;
        m_dx4PosToBarOffset.z = fZ;
        m_dx4PosToBarOffset.w = fW;
    }

public://프로그래스바 크기
    void SetWidth(float fWidth)        {  m_dx4Pos.z = fWidth;        }
    void SetHeight(float fHeight)      {  m_dx4Pos.w = fHeight;       }
    float GetWidth(void)               {  return m_dx4Pos.z;          }
    float GetHeight(void)              {  return m_dx4Pos.w;          }

public://게이지 정보( 최대, 최소, 현재 )
    void SetMaxValue(int iMaxValue)    {   m_iMaxValue = iMaxValue;   }
    void SetMinValue(int iMinValue)    {   m_iMinValue = iMinValue;   }
    void SetNowValue(int iNowValue)    {   m_iNowValue = iNowValue;   }
    int GetMaxValue(void)              {   return m_iMaxValue;        }
    int GetMinValue(void)              {   return m_iMinValue;        }
    int GetNowValue(void)              {   return m_iNowValue;        }

public:
    void SetTargetType(int iTarget)    {   m_iTargetType = iTarget;   }
    int GetTargetType()                {   return m_iTargetType;      }
    void SetTargetUnitIdx(int iIndex)  {   m_iTargetUnitIndex = iIndex;}
    int GetTargetUnitIdx()             {   return m_iTargetUnitIndex;  }

public:// Z값과 텍스쳐 알파처리등 정상동작 안함... 
       // 걍 UI레이어로 그립시다.
    void SetDrawLayer(int iLayer)          {   m_iLayer = iLayer;         }
    int GetDrawLayer()                     {   return m_iLayer;           }

public:
    void SetText( int iStringID )       {   m_iString = iStringID;     }
    int GetText()                       {   return m_iString;          }
    void SetTextOffset( float fX, float fY )
    {   m_vStringOffset.x = fX; m_vStringOffset.y = fY; }
    float GetTextOffsetX()               {  return m_vStringOffset.x; }
    float GetTextOffsetY()               {  return m_vStringOffset.y; }

public:
    void StartCoolTime( int iCoolTime, int iAccValue )
    {
        m_iCoolTime = iCoolTime;
        m_iAccValue = iAccValue;
    }
    void StopCoolTime()
    {
        m_iCoolTime = 0;
        m_iAccValue = 0;
    }
    void ToggleRender( bool bRender )
    {
        m_bIsRender = bRender;
    }
    bool GetRender()
    {
        return m_bIsRender;
    }

public:// ScreenRender에만 적용됨.
    void SetRenderNumber( bool bRender, char *szType, DWORD dwNumber, float fOffsetX, float fOffsetY, DWORD dwColor );

public:
    void AddParticle( int iID, char *strSeqeunce, float fOffsetX, float fOffsetY );
    void DeleteParticle( int iID );
    void ClearParticle();

public:// 게이지 애니메이션
    void SetBarAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni );
    void SetBackAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni );
    void SetAddTexFrontAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni );
    void SetAddTexBackAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni );
    void SetAddTexFrontScaleAni( int iKeepFrame, float fMinLimit, float fMaxLimit, float fAniSpeed, int iSoundID, bool bReturnAni );
    void SetAddTexBackScaleAni( int iKeepFrame, float fMinLimit, float fMaxLimit, float fAniSpeed, int iSoundID, bool bReturnAni );
    void SetAddTexFrontColorAni( PG_COLOR_ANIINFO &kColorAniInfo );
    void SetAddTexBackColorAni( PG_COLOR_ANIINFO &kColorAniInfo );
    void SetPosAni( float fMinX, float fMaxX, float fAniSpeedX, float fMinY, float fMaxY, float fAniSpeedY, int iKeepFrame );
    float Animation( float fNowValue, float fDefaultValue, PG_ANIINFO &kAniInfo );
    DWORD ColorAnimation( DWORD dwNowValue, PG_COLOR_ANIINFO &kColorInfo );
    void PosAnimation();

public:
    static void RegisterLuabind();

public://출력
    void DrawPrograssBar( int iLayer );
    void DrawScreenBackGround( D3DXVECTOR4 vRenderPos );
    void DrawScreenBar( D3DXVECTOR4 vRenderPos );
    void DrawScreenAddBackTextrue( D3DXVECTOR4 vRenderPos );
    void DrawScreenAddFrontTextrue( D3DXVECTOR4 vRenderPos );
    void DrawWorldBackGround( D3DXVECTOR4 vRenderPos );
    void DrawWorldBar( D3DXVECTOR4 vRenderPos );
    void DrawWorldAddBackTextrue( D3DXVECTOR4 vRenderPos );
    void DrawWorldAddFrontTextrue( D3DXVECTOR4 vRenderPos );

public:
    void FrameMove();
};


class KGCInGamePrograssBar
{
    DeclareSingleton( KGCInGamePrograssBar );
public:
    KGCInGamePrograssBar();
    ~KGCInGamePrograssBar();

public:
    void ClearALLData();
    void ClearData( int iID );

public:
    void Render( int iLayer );
    void FrameMove();

public:
    void RegisterLuabind();

public:
    KGCPrograssBar* CreatePrograssBar( int iID );
    KGCPrograssBar* GetInstanceToID( int iID );
	void            SetAllWindowAlpha( int iAlpha_ );

public:
    bool            IsCreatePrograssBarFromID( int iID );

private:
    std::map<DWORD, KGCPrograssBar*> m_mapPG;
};

DefSingletonInline( KGCInGamePrograssBar );