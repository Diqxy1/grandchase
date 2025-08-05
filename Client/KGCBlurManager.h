#pragma once


#include "GCDeviceTexture.h"
#include "KD3DDeviceState.h"

class KGCBlurManager
{
public:
    KGCBlurManager(void);
    ~KGCBlurManager(void);

    void    BeginBlur(void);        //위치는 비긴씬 바로 다음이다.
    void    EndBlur(void);          //위치는 블러 하지 않을 것들 이전이다.(예:UI렌더링 이전)

    //D3DApplication 관련 함수 
    void    InitDeviceObjects();
    void    InvalidateDeviceObjects(void);
    void    DeleteDeviceObjects(void);
    void    RestoreDeviceObjects();

    bool    IsEnable(void)          { return m_enable; }
    void    Enable(bool flg);

    bool    IsCapable() { return m_bCapable; }

    void    BlurEnd();

    //블러 알파 값 조작
    void    SetBlurAlpha(float alpha);
    float   GetBlurAlpha(void); 

    void    SetMaxBlurAlpha(float alpha);
    void    SetMinBlurAlpha(float alpha);

    float   GetMaxBlurAlpha(void)   { return m_maxAlpha; }
    float   GetMinBlurAlpha(void)   { return m_minAlpha; }

    void    AddBlurAlpha(float alpha);

public:
    //블러 텍스쳐
    LPDIRECT3DSURFACE9  m_pBlurDepthSurface;
    LPDIRECT3DSURFACE9  m_pBlurTargetSurface;
    LPDIRECT3DTEXTURE9  m_pBlurTexture;

    //누적 블러 텍스쳐
    LPDIRECT3DSURFACE9  m_pBlurAccumSurface;
    LPDIRECT3DTEXTURE9  m_pBlurAccumTexture;

    //프론트버퍼 백업용 포인터
    LPDIRECT3DSURFACE9  m_pBackDepthSurface;
    LPDIRECT3DSURFACE9  m_pBackTargetSurface;

    BLURVERTEX          m_aAccumVertex[4];
    BLURVERTEX          m_aRenderVertex[4];

    bool    m_bCapable;
    bool    m_enable;
    bool    m_firstEnable;

    float   m_blurAlpha;
    float   m_maxAlpha;
    float   m_minAlpha;


    bool    InitBlur();

    void    ApplyAccumBlur(void);
    void    UpdateAccumBlur(void);
    void    ApplyBlur(void);

    bool    IsFirstEnable();
};
