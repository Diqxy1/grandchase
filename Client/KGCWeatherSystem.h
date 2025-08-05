#pragma once

struct KWeatherInfo
{
    std::string m_strLargeTex;
    std::string m_strMiddleTex;
    std::string m_strSmallTex;

    float m_fSpeedLargeTexY;
    float m_fSpeedMiddleTexY;
    float m_fSpeedSmallTexY;

    float m_fSpeedLargeTexX;
    float m_fSpeedMiddleTexX;
    float m_fSpeedSmallTexX;

    float m_fbalanceX;    

    KWeatherInfo::KWeatherInfo()
    {
        m_strLargeTex = "";
        m_strMiddleTex = "";
        m_strSmallTex = "";
        m_fSpeedLargeTexY = 0.f;
        m_fSpeedMiddleTexY = 0.f;
        m_fSpeedSmallTexY = 0.f;

        m_fSpeedLargeTexX = 0.f;
        m_fSpeedMiddleTexX = 0.f;
        m_fSpeedSmallTexX = 0.f;

        m_fbalanceX = 0.f;             
    }
};

class KGCWeather
{
public:
    KGCWeather(void);
    ~KGCWeather(void);

public:
    void Init( KWeatherInfo kInfo_ );
    bool Render( void );
    void FrameMove( void );
    void DeleteDevice( void );
    bool LoadWeatherInfo( void );
    void SetRender( bool bRender_, DWORD dwTime_ = 0 );
    void SetAutoRenderOff( bool bRender_ ) { m_bAutoRenderOff = bRender_; }

public:
    GCBaseTexture* m_pWeatherLargeTex;
    GCBaseTexture* m_pWeatherMiddleTex;
    GCBaseTexture* m_pWeatherSmallTex;

private:
    SLVERTEX				m_FrontWeatherVertex[4];
    SLVERTEX				m_MiddleWeatherVertex[4];
    SLVERTEX				m_BackWeatherVertex[4];

    D3DXMATRIX m_matWorld;

    float m_fPointX;
    float m_fPointZ;
    float m_fVertexHeight;
    float m_fVertexWidth;

    float m_fCurrentCameraX;
    float m_fPrevCameraX;

    bool m_bRender;
    bool m_bAutoRenderOff;
    KncStopWatch m_kRenderWeatherStopWatch;

public: 
    std::string m_strLargeTex;
    std::string m_strMiddleTex;
    std::string m_strSmallTex;

    float m_fSpeedLargeTexY;
    float m_fSpeedMiddleTexY;
    float m_fSpeedSmallTexY;

    float m_fSpeedLargeTexX;
    float m_fSpeedMiddleTexX;
    float m_fSpeedSmallTexX;

    float m_fbalanceX;

};

class KGCWeatherSystem
{
    DeclareSingleton( KGCWeatherSystem );
public:
    KGCWeatherSystem(void);
    ~KGCWeatherSystem(void);

public:
    void FrameMove( void );
    void DeleteDevice( void );
    bool LoadWeatherInfo( void );
    void SetRender( int iWeatherIndex_, bool bRender_, DWORD dwTime_ = 0 );
    bool Render( void );
    void SetAutoRenderOff( bool bRender_ );
    void SetWeatherIndex( int iIdx_ ) { m_iWeatherIndex = iIdx_; }

private:
    std::map< int, KGCWeather > m_mapWeatherObj;
    int m_iWeatherIndex;

};

DefSingletonInline( KGCWeatherSystem );