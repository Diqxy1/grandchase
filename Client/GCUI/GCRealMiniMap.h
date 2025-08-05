/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2012년 04월 25일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 실시간 미니맵 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#ifndef _GCREALMINIMAP_H_
#define _GCREALMINIMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MapSystem/GCFantasticMap.h"
#include "../KGCWorldMapManager.h"

#define ORI_WIDTH   164.0f
#define ORI_HEIGHT  89.0f
#define GAP_TAGANDBOX  13.0f
#define OBJ_SIZE 2

#define TYPE_PLAYER 1
#define TYPE_MONSTER 2

struct ANIINFO
{
    float fMinLimit;
    float fMaxLimit;
    float fAniSpeed;
    int iKeepFrame;
    int iSoundID;
    bool bReturnAni;
    ANIINFO()
        : fMinLimit(0.0f)
        , fMaxLimit(0.0f)
        , fAniSpeed(0.0f)
        , iKeepFrame(0)
        , iSoundID(-1)
        , bReturnAni(false)
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

struct COLOR_ANIINFO
{
    D3DXVECTOR3 vMinLimit;
    D3DXVECTOR3 vMaxLimit;
    D3DXVECTOR3 vAniSpeed;
    int iKeepFrame;
    bool bReturnAni;
    COLOR_ANIINFO()
        : vMinLimit(0.0f, 0.0f, 0.0f)
        , vMaxLimit(0.0f, 0.0f, 0.0f)
        , vAniSpeed(0.0f, 0.0f, 0.0f)
        , iKeepFrame(0)
        , bReturnAni(false)
    {}
    void Init()
    {
        vMinLimit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        vMaxLimit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        vAniSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        iKeepFrame = 0;
        bReturnAni = false;
    }
    void SetMinLimit(float fR, float fG, float fB)
    {
        vMinLimit.x = fR; vMinLimit.y = fG; vMinLimit.z = fB;
    }
    void SetMaxLimit(float fR, float fG, float fB)
    {
        vMaxLimit.x = fR; vMaxLimit.y = fG; vMaxLimit.z = fB;
    }
    void SetAniSpeed(float fR, float fG, float fB)
    {
        vAniSpeed.x = fR; vAniSpeed.y = fG; vAniSpeed.z = fB;
    }
    void SetAniInfo(int iKeepFrame_, bool bReturnAni_)
    {
        iKeepFrame = iKeepFrame_;
        bReturnAni = bReturnAni_;
    }
};

struct TEXTUREINFO
{
    DWORD dwColor;
    D3DXVECTOR4 vUV;
    GCDeviceTexture* m_pTex;

    TEXTUREINFO()
        : dwColor(0xffffffff)
        , vUV(0.0f, 0.0f, 0.0f, 0.0f)
        , m_pTex(NULL)
    {}
    ~TEXTUREINFO()
    {
        DestroyTexture();
    }
    void Init()
    {
        dwColor = 0xffffffff;
        vUV = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
        DestroyTexture();
    }
    void DestroyTexture()
    {
        SAFE_RELEASE(m_pTex);
    }
};

struct UNITINFO
{
    int iType;
    int iIndex;
    int iAlpha;
    float fScale;
    float fStaticScale;
    DWORD dwAniColor;
    bool bNoBaseRender;
    D3DXVECTOR2 vPos;
    ANIINFO kScaleAniInfo;
    ANIINFO kAlphaAniInfo;
    COLOR_ANIINFO kColorAniInfo;

    TEXTUREINFO m_kBaseTex;
    std::vector<TEXTUREINFO> m_vecAddTex;

    UNITINFO()
        : iType(-1)
        , iIndex(-1)
        , iAlpha(255)
        , fScale(1.0)
        , fStaticScale(1.0)
        , dwAniColor(0)
        , bNoBaseRender(false)
        , vPos(0.0f, 0.0f)
    {
        kScaleAniInfo.Init();
        kAlphaAniInfo.Init();
        kColorAniInfo.Init();
        m_kBaseTex.Init();
        m_vecAddTex.clear();
    }

    UNITINFO& operator=(const UNITINFO& right)
    {
        iType = right.iType;
        iIndex = right.iIndex;
        iAlpha = right.iAlpha;
        fScale = right.fScale;
        vPos = right.vPos;
        kScaleAniInfo = right.kScaleAniInfo;
        fStaticScale = right.fStaticScale;
        bNoBaseRender = right.bNoBaseRender;
        kAlphaAniInfo = right.kAlphaAniInfo;
        kColorAniInfo = right.kColorAniInfo;
        return *this;
    }

    void Init()
    {
        iType = -1;
        iIndex = -1;
        iAlpha = 255;
        fScale = 1.0f;
        fStaticScale = 1.0f;
        bNoBaseRender = false;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
        kScaleAniInfo.Init();
        kAlphaAniInfo.Init();
        kColorAniInfo.Init();
    }

    void SetScaleAniInfo(ANIINFO& kAniInfo)
    {
        if (0 >= kAniInfo.iKeepFrame)
            return;

        if (0 == kScaleAniInfo.iKeepFrame)
        {
            kScaleAniInfo.fMinLimit = kAniInfo.fMinLimit;
            kScaleAniInfo.fMaxLimit = kAniInfo.fMaxLimit;
            kScaleAniInfo.fAniSpeed = kAniInfo.fAniSpeed;
            kScaleAniInfo.iSoundID = kAniInfo.iSoundID;
            kScaleAniInfo.bReturnAni = kAniInfo.bReturnAni;
        }
        kScaleAniInfo.iKeepFrame = kAniInfo.iKeepFrame;
    }

    void SetAlphaAniInfo(ANIINFO& kAniInfo)
    {
        if (0 >= kAniInfo.iKeepFrame)
            return;

        if (0 == kAlphaAniInfo.iKeepFrame)
        {
            kAlphaAniInfo.fMinLimit = kAniInfo.fMinLimit;
            kAlphaAniInfo.fMaxLimit = kAniInfo.fMaxLimit;
            kAlphaAniInfo.fAniSpeed = kAniInfo.fAniSpeed;
            kAlphaAniInfo.iSoundID = kAniInfo.iSoundID;
            kAlphaAniInfo.bReturnAni = kAniInfo.bReturnAni;
        }
        kAlphaAniInfo.iKeepFrame = kAniInfo.iKeepFrame;
    }

    void SetColorAniInfo(COLOR_ANIINFO& kAniInfo)
    {
        if (0 >= kAniInfo.iKeepFrame)
            return;

        if (0 == kColorAniInfo.iKeepFrame)
        {
            kColorAniInfo.vMinLimit = kAniInfo.vMinLimit;
            kColorAniInfo.vMaxLimit = kAniInfo.vMaxLimit;
            kColorAniInfo.vAniSpeed = kAniInfo.vAniSpeed;
            kColorAniInfo.bReturnAni = kAniInfo.bReturnAni;
        }
        kColorAniInfo.iKeepFrame = kAniInfo.iKeepFrame;
    }

    void SetStaticScale(float fScale)
    {
        fStaticScale = fScale;
    }

    void SetNoBaseRender(bool bRender)
    {
        bNoBaseRender = bRender;
    }
};

class KGCRealMiniMap : public KD3DDialog, public KActionListener
{
public:
    DECLARE_CLASSNAME(KGCRealMiniMap);
    /// Default constructor
    KGCRealMiniMap(void);
    /// Default destructor
    virtual ~KGCRealMiniMap(void);
    /// Copy constructor
    KGCRealMiniMap(const KGCRealMiniMap&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCRealMiniMap& operator=(const KGCRealMiniMap&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    virtual void OnCreate(void);
    virtual void PostChildDraw(void);
    virtual void ActionPerformed(const KActionEvent& event) {};
    virtual void FrameMoveInEnabledState(void);

public:
    void SetRealMiniMapInfo(SMiniMapInfo* pMiniMapInfo);

public:
    void FrameMoveToAnimation();
    float Animation(float fNowValue, float fDefaultValue, ANIINFO& kAniInfo);
    DWORD ColorAnimation(DWORD dwNowValue, COLOR_ANIINFO& kColorInfo);
    void DrawUnitToBase(UNITINFO& kUnitInfo, float fBaseScale, DWORD dwBaseColor);
    void DrawUnitToAddTexture(UNITINFO& kUnitInfo, float fBaseScale, DWORD dwBaseColor, TEXTUREINFO* pAddTex);

private:
    inline float CONVERT_WIDTH(float fWidth)
    {
        return (((m_sMiniMapInfo.m_iWidthUI - 10.0f) / m_vQuestSize.x) * fWidth);
    }
    inline float CONVERT_HEIGHT(float fHeight)
    {
        return ((m_sMiniMapInfo.m_iHeightUI / m_vQuestSize.y) * fHeight);
    }
    inline float CalcResolutionX(float fPos)
    {
        return (fPos * m_fWindowScaleX);
    };
    inline float CalcResolutionY(float fPos)
    {
        return (fPos * m_fWindowScaleY);
    };

private:
    void SetFootHoldInfo();
    void PushFoolHoldData(D3DXVECTOR4& v4BlockPos);
    D3DXVECTOR2 ConverttingCoordinates(float fWorldX, float fWorldY);

private:
    KD3DSizingBox* m_pkBackGround;
    KD3DStatic* m_pkStrEnemyNum;

private:
    SMiniMapInfo                                m_sMiniMapInfo;
    D3DXVECTOR2                                 m_vQuestSize;

private:
    UNITINFO                                    m_vMyPos;
    std::map<std::pair<int, int>, UNITINFO>     m_mapEnemyPos; // <타입, ID>, 정보
    std::map<std::pair<int, int>, UNITINFO>     m_mapAllyPos;  // <타입, ID>, 정보
    std::map<D3DXVECTOR2, D3DCOLOR>             m_mapETCPos;   // 위치, 색상
    std::vector<D3DXVECTOR4>                    m_vecFootHoldPos;
};

DEFINE_WND_FACTORY(KGCRealMiniMap);
DECLARE_WND_FACTORY(KGCRealMiniMap);
DECLARE_WND_FACTORY_NAME(KGCRealMiniMap);

#endif  // _GCREALMINIMAP_H_