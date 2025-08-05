#pragma once

class KGCSingleImage : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSingleImage );
    /// Default constructor
    KGCSingleImage( void );
    /// Default destructor
    virtual ~KGCSingleImage( void );
    /// Copy constructor
    KGCSingleImage( const KGCSingleImage& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSingleImage& operator=( const KGCSingleImage& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetInfomation( D3DXVECTOR2 vTexSize, D3DXVECTOR2 vSecondTexStartAxis, std::string strFileName, int iIndex, int iTotlaIndex );
    void DrawImage( );
    void PostDraw();
private:
    int m_iTotalNumInTexture;
    int m_iCurrentIndex;
    int m_iFileNum;
    std::string strFileName;

    GCDeviceTexture*    m_pTexture;

    //텍스쳐의 기본 시작 좌표
    D3DXVECTOR2 m_vBaseTexStartAxis;

    //텍스쳐 두번째 이미지 시작 좌표
    D3DXVECTOR2 m_vSecondTexStartAxis;

    //텍스쳐 한 이미지의 사이즈
    D3DXVECTOR2 m_vTexSize;

    //그릴 텍스쳐의 좌표
    D3DXVECTOR2 m_vImageTexStartAxis;
    D3DXVECTOR2 m_vImageTexEndtAxis;

    //그릴 UI위치
    D3DXVECTOR2 m_vImagePos;

    //텍스쳐 크기
    D3DXVECTOR2 m_vTextureFullSize;

};

DEFINE_WND_FACTORY( KGCSingleImage );
DECLARE_WND_FACTORY( KGCSingleImage );
DECLARE_WND_FACTORY_NAME( KGCSingleImage );
