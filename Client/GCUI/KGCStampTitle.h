#pragma once

class KGCStampTitle : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCStampTitle );
    /// Default constructor
    KGCStampTitle( void );
    /// Default destructor
    virtual ~KGCStampTitle( void );
    /// Copy constructor
    KGCStampTitle( const KGCStampTitle& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCStampTitle& operator=( const KGCStampTitle& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetUiType( int iID );
    void SetStampImage( );
    void PostDraw();
    void SetTexPos();
    bool GetStampImageInfo( OUT int &iFileNum, OUT int &iIndexNum , IN int iUitype );
private:
    int m_iUitype;

    int m_iIndex;

    GCDeviceTexture*    m_pStampTexture;

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

DEFINE_WND_FACTORY( KGCStampTitle );
DECLARE_WND_FACTORY( KGCStampTitle );
DECLARE_WND_FACTORY_NAME( KGCStampTitle );
